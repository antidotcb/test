#include "LobbyList.h"
#include "Lobby.h"
#include "Error.h"
#include "LobbyLoad.h"
#include "Controls.h"
#include "Lobbies.h"


namespace testapp {
namespace states {

using namespace galaxy::api;


LobbyList::LobbyList(const std::string& _username)
    : username { _username }, lobbies{ std::make_unique<Lobbies>() } {}


void LobbyList::RequestLobbyList() {
    Matchmaking()->AddRequestLobbyListStringFilter(Lobby::FIELD_GUID, Lobby::GAME_GUID, LOBBY_COMPARISON_TYPE_EQUAL);
    Matchmaking()->RequestLobbyList();
}


void LobbyList::Init() {
    SetupKeys({
        { { Input::UP }, { [this]() {
                lobbies->Selection().Prev();
            }, "Select" } },

        { { Input::DOWN }, { [this]() {
                lobbies->Selection().Next();
            }, "Select" } },

        { { 'N', 'n' }, { [this]() {
                CreateLobby();
            }, "Create lobby" } },

        { { 'U', 'u' }, { [this]() {
                RequestLobbyList();
            }, "Update lobby list" } },

        { { Input::ENTER }, { [this]() {
                JoinLobby();
            }, "Join lobby" } },

        { { Input::ESCAPE }, { [this]() {
                GoToPrevious();
            }, "Return" } },
    });
}


void LobbyList::Print() {
    State::Print();
    Controls::Username(username);
    lobbies->Print();
}


void LobbyList::Enter() {
    RequestLobbyList();
    auto registrar = GalaxyFactory::GetInstance()->GetListenerRegistrar();
    if ( registrar )
        registrar->Register(ILobbyDataListener::GetListenerType(), static_cast<ILobbyDataListener*>(this));
}


void LobbyList::Leave() {
    auto registrar = GalaxyFactory::GetInstance()->GetListenerRegistrar();
    if ( registrar ) {
        registrar->Unregister(ILobbyDataListener::GetListenerType(), static_cast<ILobbyDataListener*>(this));
    }
}


void LobbyList::OnLobbyList(uint32_t lobbyCount, bool ioFailure) {
    if ( !ioFailure ) {
        for ( auto i = 0u; i < lobbyCount; ++i ) {
            auto lobby_id = Matchmaking()->GetLobbyByIndex(i);
            Matchmaking()->RequestLobbyData(lobby_id);
        }
    }

    // TODO: get rid of extinct lobbies
}


void LobbyList::OnLobbyDataUpdated(const GalaxyID& lobbyID, const GalaxyID& memberID) {
    if ( !lobbyID.IsValid() || memberID.IsValid() ) {
        return; // ignore member updates, only accept lobby updates
    }

    if ( !lobbies->Exists(lobbyID) ) {
        lobbies->Add(lobbyID);
    }

    lobbies->Update(lobbyID);
}


void LobbyList::CreateLobby() {
    GoToNext(std::make_unique<LobbyLoad>(username));
}


void LobbyList::JoinLobby() {
    auto lobby = lobbies->ByID(lobbies->Selection().Id());
    if ( lobby ) {
        GoToNext(std::make_unique<LobbyLoad>(username, lobby));
    } else {
        throw std::logic_error("Invalid selection.");
    }
}


} // namespace states
} // namespace testapp

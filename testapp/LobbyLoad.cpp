#include "LobbyLoad.h"
#include "LobbyList.h"
#include "Error.h"
#include "LobbyMain.h"
#include "Controls.h"
#include "Lobby.h"


namespace testapp {
namespace states {

using namespace galaxy::api;


LobbyLoad::LobbyLoad(const std::string& _username, const std::shared_ptr<Lobby>& _lobby)
    : username{ _username }, lobby{ _lobby }, isHost{ false } {}


void LobbyLoad::Init() {
    try {
        if ( !lobby ) {
            Lobby::Create();
        } else {
            lobby->Join();
        }
    }
    catch ( IError& error ) {
        throw Error(error);
    }
}


void LobbyLoad::Print() {
    State::Print();
    Controls::Username(username);
    Controls::Progressbar(isHost ? "Entering lobby" : "Creating lobby");
}


const char* LobbyLoad::DecodeEnterResult(const LobbyEnterResult& result) {
    switch ( result ) {
        case LOBBY_ENTER_RESULT_SUCCESS:
            return "";
        case LOBBY_ENTER_RESULT_LOBBY_IS_FULL:
            return "Lobby is full";
        case LOBBY_ENTER_RESULT_LOBBY_DOES_NOT_EXIST:
            return "Lobby does not exist";
        case LOBBY_ENTER_RESULT_ERROR:
            return "Unexpected error";
        default:
            return "Unknown error";
    }
}


void LobbyLoad::OnLobbyCreated(const GalaxyID& lobbyID, LobbyCreateResult result) {
    if ( LOBBY_CREATE_RESULT_SUCCESS != result ) {
        GoToNext(std::make_unique<Error>("Failed to create lobby"), false);
        return;
    }

    lobby = std::make_unique<Lobby>(lobbyID);
    isHost = true;
}


void LobbyLoad::OnLobbyEntered(const GalaxyID& lobbyID, LobbyEnterResult result) {
    if ( !lobby || lobbyID != lobby->GetId() ) {
        return;
    }

    if ( LOBBY_ENTER_RESULT_SUCCESS != result ) {
        GoToNext(std::make_unique<Error>("Failed to join lobby", DecodeEnterResult(result)), false);
        return;
    }

    lobby->Setup(username, isHost);

    GoToNext(std::make_unique<LobbyMain>(username, lobby), false);
}


} // namespace states
} // namespace testapp

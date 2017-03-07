#include "LobbyMain.h"
#include "Members.h"
#include "Lobby.h"
#include "Controls.h"
#include "Error.h"
#include <sstream>
#include <iostream>


namespace testapp {
namespace states {
using namespace galaxy::api;


LobbyMain::LobbyMain(const std::string& _username, const std::shared_ptr<Lobby>& _lobby)
    : username{ _username }, lobby { _lobby }, members{ std::make_unique<Members>() } {}


void LobbyMain::Init() {
    SetupKeys({
        { { Input::ESCAPE } , { [this]() {
                    LeaveLobby();
                    GoToPrevious();
                }, "Exit" }
        },
        { { Input::ENTER } , { [this]() {
                    WriteMessage();
                }, "Message" }
        },
        { { Input::DOWN } ,{ [this]() {
                    members->Selection().Next();
                }, "Select target" }
        },
        { { Input::UP } ,{ [this]() {
                    members->Selection().Prev();
                }, "Select target" }
        },
    });

    members->Add(lobby->GetId(), "Everyone");

    if ( lobby->IsValid() && lobby->GetHost() != username ) {
        auto hostId = lobby->GetHostId();
        members->Add(hostId, lobby->GetHost());
    }
}


void LobbyMain::Print() {
    State::Print();
    Controls::Username(username);
    lobby->Print();
    members->Print();

    for ( auto& msg : messages ) {
        std::cout << msg << std::endl;
    }
}


void LobbyMain::LeaveLobby() const {
    lobby->Leave();
}


void LobbyMain::OnLobbyLeft(const GalaxyID& lobbyID, bool ioFailure) {
    if ( ioFailure ) {
        // TODO: add IO failure handling
    }

    if ( lobbyID != lobby->GetId() ) {
        return; // TODO: add some logging about notify about wrong lobbyID
    }

    GoToPrevious();
}


void LobbyMain::OnP2PPacketAvailable(uint32_t msgSize, uint8_t channel) {
    std::vector<char> buffer(msgSize, '\0');

    GalaxyID sender_id;
    auto size = 0u;
    auto result = Networking()->PeekP2PPacket(&buffer.front(), msgSize, &size, sender_id, channel);

    if ( result && msgSize == size ) {
        auto message = std::string(&buffer.front(), size);
        AddMessage(sender_id, message);
    } else {
        // TODO Add error handling    
    }
}


void LobbyMain::OnLobbyMemberStateChanged(const GalaxyID& lobbyID, const GalaxyID& memberID, LobbyMemberStateChange memberStateChange) {
    if ( lobbyID != lobby->GetId() ) {
        return; // TODO: add some logging about notify about wrong lobbyID
    }

    static const std::map<LobbyMemberStateChange, std::string> state_message = {
        { LOBBY_MEMBER_STATE_CHANGED_BANNED," banned." },
        { LOBBY_MEMBER_STATE_CHANGED_DISCONNECTED," disconnected." },
        { LOBBY_MEMBER_STATE_CHANGED_KICKED," kicked." },
        { LOBBY_MEMBER_STATE_CHANGED_LEFT," left." }
    };

    if ( LOBBY_MEMBER_STATE_CHANGED_ENTERED != memberStateChange ) {
        auto member_name = members->ByID(memberID);
        if ( !member_name.empty() ) {
            auto message = member_name + state_message.at(memberStateChange);
            AddMessage(message);
        }
        members->Remove(memberID);
    }
}


void LobbyMain::OnLobbyDataUpdated(const GalaxyID& lobbyID, const GalaxyID& memberID) {
    if ( lobbyID != lobby->GetId() ) {
        return; // TODO: add some logging about notify about wrong lobbyID
    }

    if ( !memberID.IsValid() ) {
        lobby->Update();
        return; // only lobby update, no info about member
    }

    auto member_name = lobby->RetrieveMemberName(memberID);
    if ( !member_name.empty() ) {
        members->Add(memberID, member_name);
        auto message = member_name + " joined.";
        AddMessage(message);
    }

    // TODO: Add error handling, cannot retrieve member name
}


void LobbyMain::OnLobbyMessageReceived(const GalaxyID& lobbyID, const GalaxyID& senderID, uint32_t msgID, uint32_t msgSize) {
    if ( lobbyID != lobby->GetId() ) {
        return; // TODO: add some logging about notify about wrong lobbyID
    }

    if ( !msgSize ) {
        return; // no information, call to GetLobbyMessage result in error
    }

    std::vector<char> buffer(msgSize, '\0');
    auto sender_id = senderID;
    auto result_size = Matchmaking()->GetLobbyMessage(lobbyID, msgID, sender_id, &buffer.front(), msgSize);
    if ( result_size == msgSize ) {
        std::string message(std::begin(buffer), std::end(buffer));
        AddMessage(sender_id, message);
    }
}


std::string LobbyMain::ReadMessage() const {
    std::cout << username << ":";
    std::string message;
    std::getline(std::cin, message);
    return message;
}


void LobbyMain::AddMessage(const std::string& message) {
    messages.push_back(message);
    static const auto MESSAGE_LIMIT = 16u;
    if ( messages.size() > MESSAGE_LIMIT )
        messages.pop_front();
}


void LobbyMain::AddMessage(const GalaxyID& sender_id, const std::string& message) {
    if ( !members->Exists(sender_id) ) {
        return; // TODO: message received after disconnect?
    }

    auto named_message = members->ByID(sender_id) + ":" + message;
    AddMessage(named_message);
}


void LobbyMain::SendMessageTo(const GalaxyID& target_id, const std::string& message) {
    auto data = message.c_str();
    auto data_size = static_cast<uint32_t>(message.size());

    if ( target_id == lobby->GetId() ) {
        Matchmaking()->SendLobbyMessage(target_id, data, data_size);
    } else {
        if ( target_id.IsValid() ) {
            auto store_message = username + ":" + message;
            AddMessage(store_message);
            Networking()->SendP2PPacket(target_id, data, data_size, P2P_SEND_RELIABLE);
        }
    }
}


void LobbyMain::WriteMessage() {
    auto target_id = members->Selection().Id();
    auto message = ReadMessage();

    if ( !message.empty() ) {
        SendMessageTo(target_id, message);
    }
}


} // namespace states
} // namespace testapp

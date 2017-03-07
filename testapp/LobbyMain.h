#pragma once

#include <deque>
#include <galaxy/GalaxyApi.h>
#include "State.h"


namespace testapp {

class Lobby;
class Members;


namespace states {


class LobbyMain : public State,
                  galaxy::api::SelfRegisteringListener<galaxy::api::INetworkingListener>,
                  galaxy::api::SelfRegisteringListener<galaxy::api::ILobbyMemberStateListener>,
                  galaxy::api::SelfRegisteringListener<galaxy::api::ILobbyDataListener>,
                  galaxy::api::SelfRegisteringListener<galaxy::api::ILobbyLeftListener>,
                  galaxy::api::SelfRegisteringListener<galaxy::api::ILobbyMessageListener>
{
public:
    LobbyMain(const std::string& username, const std::shared_ptr<Lobby>& lobby);

protected:
    void Init() override;
    void Print() override;

    void OnLobbyLeft(const galaxy::api::GalaxyID& lobbyID, bool ioFailure) override;
    void OnP2PPacketAvailable(uint32_t msgSize, uint8_t channel) override;
    void OnLobbyMemberStateChanged(const galaxy::api::GalaxyID& lobbyID, const galaxy::api::GalaxyID& memberID,
                                   galaxy::api::LobbyMemberStateChange memberStateChange) override;
    void OnLobbyDataUpdated(const galaxy::api::GalaxyID& lobbyID, const galaxy::api::GalaxyID& memberID) override;
    void OnLobbyMessageReceived(const galaxy::api::GalaxyID& lobbyID, const galaxy::api::GalaxyID& senderID, uint32_t messageID, uint32_t messageLength) override;

private:
    void LeaveLobby() const;

    std::string ReadMessage() const;
    void WriteMessage();
    void AddMessage(const std::string& message);
    void AddMessage(const galaxy::api::GalaxyID& sender_id, const std::string& message);
    void SendMessageTo(const galaxy::api::GalaxyID& target_id, const std::string& message);

    const std::string username;
    std::shared_ptr<Lobby> lobby;
    std::deque<std::string> messages;
    std::unique_ptr<Members> members;
};

}

}

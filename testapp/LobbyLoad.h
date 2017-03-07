#pragma once

#include <galaxy/GalaxyApi.h>
#include "State.h"


namespace testapp {

class Lobby;


namespace states {


class LobbyLoad : public State,
                  galaxy::api::SelfRegisteringListener<galaxy::api::ILobbyCreatedListener>,
                  galaxy::api::SelfRegisteringListener<galaxy::api::ILobbyEnteredListener>
{
public:
    explicit LobbyLoad(const std::string& name, const std::shared_ptr<Lobby>& lobby = nullptr);

protected:
    void Init() override;
    void Print() override;

    static const char* DecodeEnterResult(const galaxy::api::LobbyEnterResult& result);

    void OnLobbyCreated(const galaxy::api::GalaxyID& lobbyID, galaxy::api::LobbyCreateResult result) override;
    void OnLobbyEntered(const galaxy::api::GalaxyID& lobbyID, galaxy::api::LobbyEnterResult result) override;

private:
    const std::string username;

    std::shared_ptr<Lobby> lobby;

    bool isHost;
};

} // namespace states
} // namespace testapp

#pragma once

#include <galaxy/GalaxyApi.h>
#include "State.h"


namespace testapp {

class Lobbies;


namespace states {

class LobbyList : public State,
                  galaxy::api::ILobbyDataListener,
                  galaxy::api::SelfRegisteringListener<galaxy::api::ILobbyListListener>
{
public:
    explicit LobbyList(const std::string& username);

protected:
    void Init() override;
    void Print() override;

    void Enter() override;
    void Leave() override;

    void OnLobbyList(uint32_t lobbyCount, bool ioFailure) override;
    void OnLobbyDataUpdated(const galaxy::api::GalaxyID& lobbyID, const galaxy::api::GalaxyID& memberID) override;

private:
    static void RequestLobbyList();

    void CreateLobby();
    void JoinLobby();

    const std::string username;
    std::unique_ptr<Lobbies> lobbies;
};


} // namespace states
} // namespace testapp

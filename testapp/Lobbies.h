#pragma once

#include <memory>
#include <galaxy/GalaxyApi.h>
#include "Selector.h"


namespace testapp {

class Lobby;


class Lobbies
{
public:
    using LobbyPtr = std::shared_ptr<Lobby>;
    using Container = std::map<galaxy::api::GalaxyID, LobbyPtr>;

    Lobbies();

    void Add(galaxy::api::GalaxyID id);
    bool Exists(const galaxy::api::GalaxyID& id) const;

    LobbyPtr ByID(const galaxy::api::GalaxyID& id) const;

    void Print() const;
    void Update(const galaxy::api::GalaxyID& id);

    using Selector = Selector<galaxy::api::GalaxyID, LobbyPtr>;
    Selector& Selection();

private:
    size_t GetValidCount() const;

    Container lobbies;
    Selector selector;
};

}

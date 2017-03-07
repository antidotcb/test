#pragma once

#include <galaxy/GalaxyApi.h>
#include "Selector.h"


namespace testapp {

class Lobby;

class Members
{
public:
    Members();

    std::string& ByID(const galaxy::api::GalaxyID& id);
    const std::string& ByID(const galaxy::api::GalaxyID& id) const;
    galaxy::api::GalaxyID ByName(const std::string& name) const;

    void Add(const galaxy::api::GalaxyID& id, const std::string& name);
    bool Exists(const galaxy::api::GalaxyID& id) const;
    void Remove(const galaxy::api::GalaxyID& id);

    void Print() const;

    using Selector = Selector<galaxy::api::GalaxyID, std::string>;

    Selector& Selection();
private:
    using Container = std::map<galaxy::api::GalaxyID, std::string>;

    Container members;
    Selector selector;
};

}

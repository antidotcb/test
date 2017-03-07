#include "Members.h"
#include <iostream>
#include <string>
#include <algorithm>


namespace testapp {

Members::Members()
    : selector(members) {}


std::string& Members::ByID(const galaxy::api::GalaxyID& id) {
    return members.at(id);
}


const std::string& Members::ByID(const galaxy::api::GalaxyID& id) const {
    return members.at(id);
}


galaxy::api::GalaxyID Members::ByName(const std::string& name) const {
    auto byName = [&name](auto it) {
                return it.second == name;
            };

    auto result = std::find_if(std::begin(members), std::end(members), byName);

    if ( result == std::end(members) ) {
        return galaxy::api::GalaxyID();
    }

    return result->first;
}


bool Members::Exists(const galaxy::api::GalaxyID& id) const {
    return members.find(id) != std::end(members);
}


void Members::Remove(const galaxy::api::GalaxyID& id) {
    auto backup = selector.Id();
    members.erase(id);
    selector.Select(backup);
}


void Members::Add(const galaxy::api::GalaxyID& id, const std::string& name) {
    auto backup = selector.Id();
    members[id] = name;
    selector.Select(backup);
}


void Members::Print() const {
    std::cout << "Members:" << std::endl;
    auto selected_id = selector.Id();

    for ( auto& lobby : members ) {
        auto& lobby_id = lobby.first;
        auto& member_name = lobby.second;
        std::cout << ( selected_id == lobby_id ? ">" : " " ) << member_name << std::endl;
    }
    std::cout << std::endl;
}


Members::Selector& Members::Selection() {
    return selector;
}

} // namespace testapp

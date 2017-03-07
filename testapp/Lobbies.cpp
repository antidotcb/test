#include "Lobbies.h"
#include <iostream>
#include <algorithm>
#include "Lobby.h"


namespace testapp {

Lobbies::Lobbies()
    : selector(this->lobbies) {}


void Lobbies::Add(galaxy::api::GalaxyID lobbyID) {
    auto lobby = std::make_shared<Lobby>(lobbyID);
    auto backup = selector.Id();
    lobbies.emplace(lobbyID, std::move(lobby));
    selector.Select(backup);
}


bool Lobbies::Exists(const galaxy::api::GalaxyID& lobbyID) const {
    return lobbies.find(lobbyID) != std::end(lobbies);
}


Lobbies::LobbyPtr Lobbies::ByID(const galaxy::api::GalaxyID& id) const {
    return Exists(id) ? lobbies.at(id) : nullptr;
}


size_t Lobbies::GetValidCount() const {
    auto isValid = [](auto& iter) {
                auto& obj = iter.second;
                return obj->IsValid();
            };

    return std::count_if(std::begin(lobbies),
                         std::end(lobbies),
                         isValid);
}


void Lobbies::Print() const {
    if ( !GetValidCount() ) {
        std::cout << "No lobbies found." << std::endl;
        return;
    }

    auto selected_id = selector.Id();

    for ( const auto& lobby : lobbies ) {
        auto& lobby_id = lobby.first;
        auto& lobby_ptr = lobby.second;
        if ( lobby_ptr->IsValid() ) {
            std::cout << ( selected_id == lobby_id ? ">" : " " );
            lobby_ptr->Print();
        }
    }
}


void Lobbies::Update(const galaxy::api::GalaxyID& id) {
    return lobbies.at(id)->Update();
}


Lobbies::Selector& Lobbies::Selection() {
    return selector;
}

} // namespace testapp

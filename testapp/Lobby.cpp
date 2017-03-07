#include "Lobby.h"
#include <iostream>
#include <vector>

#include <galaxy/GalaxyApi.h> // some magic with my IDE - need to include it again, even it's already in Lobby.h


namespace testapp {

using namespace galaxy::api;


Lobby::Lobby(const galaxy::api::GalaxyID& _lobbyID)
    : lobbyID{ _lobbyID } {}


void Lobby::Create() {
    Matchmaking()->CreateLobby(LOBBY_TYPE_PUBLIC, 2u, true, LOBBY_TOPOLOGY_TYPE_FCM);
}


void Lobby::Join() const {
    if ( Matchmaking()->IsLobbyJoinable(lobbyID) ) {
        Matchmaking()->JoinLobby(lobbyID);
    }
}


void Lobby::Update() {
    host = GetLobbyData(FIELD_NAME);
    guid = GetLobbyData(FIELD_GUID);
}


void Lobby::Print() const {
    std::cout << "[Host:" << host << "]" << std::endl;
}


bool Lobby::IsValid() const {
    return lobbyID.IsValid() &&
            !GetHost().empty() &&
            GAME_GUID == GetGuid();
}


GalaxyID Lobby::GetId() const {
    return lobbyID;
}


std::string Lobby::GetLobbyData(const char* key, const uint32_t value_size) const {
    std::vector<char> buffer(value_size, 0);
    Matchmaking()->GetLobbyDataCopy(lobbyID, key, &buffer.front(), value_size);
    return std::string(&buffer.front(), strlen(&buffer.front()));
}


std::string Lobby::GetHost() const {
    return host;
}


std::string Lobby::GetGuid() const {
    return guid;
}


void Lobby::Setup(const std::string& username, bool isHost) const {
    if ( isHost ) {
        Matchmaking()->SetLobbyData(lobbyID, FIELD_NAME, username.c_str());
        Matchmaking()->SetLobbyData(lobbyID, FIELD_GUID, GAME_GUID);
    }
    Matchmaking()->SetLobbyMemberData(lobbyID, FIELD_NAME, username.c_str());
}


std::string Lobby::RetrieveMemberName(const galaxy::api::GalaxyID& memberID) const {
    std::vector<char> buffer(VALUE_SIZE, 0);
    galaxy::api::Matchmaking()->GetLobbyMemberDataCopy(lobbyID, memberID, FIELD_NAME, &buffer.front(), VALUE_SIZE);
    return std::string(&buffer.front(), strlen(&buffer.front()));
}


galaxy::api::GalaxyID Lobby::GetHostId() const {
    return Matchmaking()->GetLobbyOwner(lobbyID);
}


void Lobby::Leave() const {
    Matchmaking()->LeaveLobby(lobbyID);
}

} // namespace testapp

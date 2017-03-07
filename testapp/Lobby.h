#pragma once

#include <string>
#include <galaxy/GalaxyID.h>


namespace std {

template <>
struct hash<galaxy::api::GalaxyID>
{
    std::size_t operator()(const galaxy::api::GalaxyID& k) const {
        return std::hash<uint64_t>()(k.ToUint64());
    }
};

}


namespace testapp {

class Lobby
{
public:
    constexpr static auto GAME_GUID = "{A0C25FC9-80CD-48EE-9E3D-5354F0CB69FD}";
    constexpr static auto FIELD_GUID = "GUID";
    constexpr static auto FIELD_NAME = "HOST";
    constexpr static auto VALUE_SIZE = 256u;

    explicit Lobby(const galaxy::api::GalaxyID& lobbyID);

    static void Create();

    void Join() const;
    void Leave() const;
    void Print() const;
    void Update();

    bool IsValid() const;

    galaxy::api::GalaxyID GetId() const;
    std::string GetHost() const;
    std::string GetGuid() const;

    void Setup(const std::string& username, bool isHost) const;

    std::string RetrieveMemberName(const galaxy::api::GalaxyID& memberID) const;
    galaxy::api::GalaxyID GetHostId() const;

private:
    std::string GetLobbyData(const char* key, const uint32_t value_size = VALUE_SIZE) const;

    galaxy::api::GalaxyID lobbyID;

    std::string guid;
    std::string host;
};

}

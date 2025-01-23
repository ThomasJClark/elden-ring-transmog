#include "ertransmogrify_net.hpp"

#include <elden-x/session.hpp>

#include <spdlog/spdlog.h>

#include <steam/isteamfriends.h>
#include <steam/isteamnetworkingmessages.h>
#include <steam/isteamuser.h>

#include <algorithm>
#include <array>
#include <map>
#include <span>

static auto net_state_by_steam_id =
    std::map<unsigned long long, ertransmogrify::vfx::player_state_st>{};

static const auto empty_state = ertransmogrify::vfx::player_state_st{};

static constexpr int steam_networking_channel_transmog = 100069;

void ertransmogrify::net::send_messages(const ertransmogrify::vfx::player_state_st &state)
{
    auto session_manager = er::CS::CSSessionManagerImp::instance();
    if (!session_manager || session_manager->player_entries().empty())
    {
        return;
    }

    auto local_player_steam_id = SteamUser()->GetSteamID().ConvertToUint64();

    // Send the local player's transmog selections to every connected player in the current session
    for (auto &entry : session_manager->player_entries())
    {
        // Don't send messages to ourself
        if (entry.steam_id == local_player_steam_id)
        {
            continue;
        }

        SteamNetworkingIdentity steam_id;
        steam_id.SetSteamID64(entry.steam_id);

        auto result = SteamNetworkingMessages()->SendMessageToUser(
            steam_id, &state, sizeof(state), k_nSteamNetworkingSend_Reliable,
            steam_networking_channel_transmog);
        if (result != k_EResultOK)
        {
            SPDLOG_ERROR("Error {} sending Steam networking message to user {}", (int)result,
                         entry.steam_id);
        }
    }
}

void ertransmogrify::net::receive_messages()
{
    static auto message_buffer = std::array<SteamNetworkingMessage_t *, 128>{};

    auto message_count = SteamNetworkingMessages()->ReceiveMessagesOnChannel(
        steam_networking_channel_transmog, message_buffer.data(), message_buffer.size());
    auto messages = std::span{message_buffer.data(), (size_t)message_count};

    for (auto &message : messages)
    {
        if (message->GetSize() < sizeof(ertransmogrify::vfx::player_state_st))
        {
            continue;
        }

        auto steam_id = message->m_identityPeer.GetSteamID64();
        auto &net_state =
            *reinterpret_cast<const ertransmogrify::vfx::player_state_st *>(message->GetData());
        if (net_state_by_steam_id.insert_or_assign(steam_id, net_state).second)
        {
            auto steam_name = SteamFriends()->GetFriendPersonaName(steam_id);
            if (steam_name)
                SPDLOG_INFO("Received transmog state from Steam user {}", steam_name);
            else
                SPDLOG_INFO("Received transmog state from Steam user {}", steam_id);
        }

        message->Release();
    }

    // Remove entries for players who aren't connected anymore
    auto session_manager = er::CS::CSSessionManagerImp::instance();
    auto player_entries = session_manager ? session_manager->player_entries()
                                          : std::span<er::CS::CSSessionManagerImp::player_entry>{};

    for (auto it = net_state_by_steam_id.begin(); it != net_state_by_steam_id.end();)
    {
        if (std::none_of(player_entries.begin(), player_entries.end(),
                         [&](auto entry) { return entry.steam_id == it->first; }))
        {
            it = net_state_by_steam_id.erase(it);
        }
        else
        {
            it++;
        }
    }
}

const ertransmogrify::vfx::player_state_st &ertransmogrify::net::get_net_player_state(
    unsigned long long steam_id)
{
    auto entry = net_state_by_steam_id.find(steam_id);
    if (entry != net_state_by_steam_id.end())
    {
        return entry->second;
    }

    return empty_state;
}

#pragma once

#include <cstdint>

#include "internal/WorldChrMan.hpp"

namespace players
{
static constexpr size_t net_player_max = 6;
static constexpr std::byte ceremony_type_none = (std::byte)0;

typedef int32_t ApplySpEffectFn(CS::ChrIns *, uint32_t speffect_id, bool unk);
typedef int32_t ClearSpEffectFn(CS::ChrIns *, uint32_t speffect_id);
typedef void SpawnOneShotVFXOnChrFn(CS::ChrIns *, int32_t dummy_poly_id, int32_t sfx_id, void *unk);

void initialize();
CS::PlayerIns *get_main_player();
CS::NetPlayer *get_net_players();
bool has_item_in_inventory(CS::PlayerIns *, int32_t item_id);
bool has_speffect(CS::PlayerIns *, int32_t speffect_id);
extern ApplySpEffectFn *apply_speffect;
extern ClearSpEffectFn *clear_speffect;
extern SpawnOneShotVFXOnChrFn *spawn_one_shot_sfx_on_chr;
std::byte get_ceremony_type();
};
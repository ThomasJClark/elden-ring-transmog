#pragma once

#include <cstdint>

#include "../internal/WorldChrMan.hpp"

namespace players
{
static constexpr size_t net_player_max = 6;
static constexpr unsigned char ceremony_type_none = (unsigned char)0;

typedef int ApplySpEffectFn(CS::ChrIns *, unsigned int speffect_id, bool unk);
typedef int ClearSpEffectFn(CS::ChrIns *, unsigned int speffect_id);
typedef void SpawnOneShotVFXOnChrFn(CS::ChrIns *, int dummy_poly_id, int sfx_id, void *unk);

void initialize();
CS::PlayerIns *get_main_player();
CS::NetPlayer *get_net_players();
bool has_item_in_inventory(CS::PlayerIns *, int item_id);
bool has_speffect(CS::PlayerIns *, int speffect_id);
extern ApplySpEffectFn *apply_speffect;
extern ClearSpEffectFn *clear_speffect;
extern SpawnOneShotVFXOnChrFn *spawn_one_shot_sfx_on_chr;
unsigned char get_ceremony_type();
};
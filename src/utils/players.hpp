#pragma once

#include <elden-x/chr/chr.hpp>
#include <elden-x/chr/player.hpp>

namespace players
{
typedef int ApplySpEffectFn(from::CS::ChrIns *, unsigned int speffect_id, bool unk);
typedef int ClearSpEffectFn(from::CS::ChrIns *, unsigned int speffect_id);
typedef void SpawnOneShotVFXOnChrFn(from::CS::ChrIns *, int dummy_poly_id, int sfx_id, void *unk);

void initialize();
bool has_item_in_inventory(from::CS::PlayerIns *, int item_id);
bool has_speffect(from::CS::PlayerIns *, int speffect_id);
extern ApplySpEffectFn *apply_speffect;
extern ClearSpEffectFn *clear_speffect;
extern SpawnOneShotVFXOnChrFn *spawn_one_shot_sfx_on_chr;
};
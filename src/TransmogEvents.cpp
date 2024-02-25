#include <iostream>
#include <tga/paramdefs.h>

#include "ModUtils.hpp"
#include "TransmogEvents.hpp"
#include "TransmogVFX.hpp"

using namespace TransmogEvents;
using namespace std;

static CS::WorldChrManImp **world_chr_man_addr = nullptr;

/**
 * If the given item ID in the players inventory is a transmog good, look up the corresponding
 * armor piece and update the transmog VFX to show it.
 */
static EquipParamProtector *try_apply_transmog_item(int32_t item_id)
{

    return nullptr;
}

namespace CS
{
struct InGameStep;
};

static void (*init_move_map)(CS::InGameStep *) = nullptr;
static void (*finish_move_map)(CS::InGameStep *) = nullptr;

/**
 * When loading into a map, update the transmog VFX based on the character's inventory
 */
static void init_move_map_detour(CS::InGameStep *in_game_step)
{
    init_move_map(in_game_step);
}

/**
 * When loading out of a map, reset the transmog VFX
 */
static void finish_move_map_detour(CS::InGameStep *in_game_step)
{
    TransmogVFX::reset_transmog();
    finish_move_map(in_game_step);
}

void TransmogEvents::initialize(CS::WorldChrManImp **world_chr_man_addr)
{
    ::world_chr_man_addr = world_chr_man_addr;

    // TODO AOB
    // InGameStep::STEP_MoveMap_Init
    ModUtils::hook(
        {
            .offset = 0xac0200,
        },
        init_move_map_detour, init_move_map);

    // TODO AOB
    // InGameStep::STEP_MoveMap_Finish
    ModUtils::hook(
        {
            .offset = 0xabfdb0,
        },
        finish_move_map_detour, finish_move_map);
}
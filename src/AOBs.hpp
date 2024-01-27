#pragma once

#include "GameMemory.hpp"

struct ParamList;
struct WorldChrMan;
struct MsgRepository;
struct GameDataMan;

static const auto param_list_aob = GameMemory::ScanArgs<ParamList *>(
    {.aob = {0x48, 0x8B, 0x0D, -1, -1, -1,   -1,   0x48, 0x85, 0xC9, 0x0F,
             0x84, -1,   -1,   -1, -1, 0x45, 0x33, 0xC0, 0xBA, 0x90},
     .relative_offsets = {{0x3, 0x7}}});

static const auto world_chr_man_aob = GameMemory::ScanArgs<WorldChrMan *>(
    {.aob = {0x48, 0x8B, 0x05, -1, -1, -1, -1, 0x48, 0x85, 0xC0, 0x74, 0x0F, 0x48, 0x39, 0x88},
     .relative_offsets = {{0x3, 0x7}}});

static const auto msg_repository_aob = GameMemory::ScanArgs<MsgRepository *>(
    {.aob = {0x48, 0x8B, 0x3D, -1, -1, -1, -1, 0x44, 0x0F, 0xB6, 0x30, 0x48, 0x85, 0xFF, 0x75},
     .relative_offsets = {{0x3, 0x7}}});

static const auto game_data_man_aob =
    GameMemory::ScanArgs<GameDataMan *>({.aob = {0x48, 0x8B, 0x05, -1, -1, -1, -1, 0x48, 0x85, 0xC0,
                                                 0x74, 0x05, 0x48, 0x8B, 0x40, 0x58, 0xC3, 0xC3},
                                         .relative_offsets = {{0x3, 0x7}}});

// static const auto item_gib_aob =
//     GameMemory::ScanArgs<ItemGibFn>({.aob = {0x8B, 0x02, 0x83, 0xF8, 0x0A}, .offset = -0x52});

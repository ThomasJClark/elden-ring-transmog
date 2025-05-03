#include "ertransmogrify_shop.hpp"
#include "ertransmogrify_config.hpp"
#include "ertransmogrify_messages.hpp"
#include "ertransmogrify_vfx.hpp"
#include "utils/players.hpp"

#include <elden-x/chr/world_chr_man.hpp>
#include <elden-x/gamedata/game_data_man.hpp>
#include <elden-x/params.hpp>
#include <elden-x/utils/modutils.hpp>

#include <spdlog/spdlog.h>

#include <unordered_map>
#include <unordered_set>

using namespace ertransmogrify;
using namespace std;

static constexpr unsigned short invisible_icon_id = 3142;

static const unordered_set<unsigned long long> exluded_protector_ids = {
    // Skip Grass Hair Ornament, which is a cut helmet that's missing an icon
    920000,

    // Skip the Roundtable Set, which is an unobtainable set present in Reforged that doesn't
    // have a model
    955000, 955100, 955200, 955300, 956100};

// Include protectors with missing names if they're from known mods or cut content. We filter out
// nameless rows to exclude NPC duplicates and other junk, but we should skip this filtering for
// content that might not have localization in the user's current language.
static const unordered_set<unsigned long long> known_protector_ids = {
    // Brave's set
    700000, 700100, 700200, 700300, 701000, 702000,

    // Deathbed Smalls
    1930300,

    // Millicent's set
    1950100, 1950200, 1950300, 1970100, 1970200,

    // Berserker Armor - https://www.nexusmods.com/eldenring/mods/729
    964400, 964500, 964600, 964700, 965400, 965500};

const map<unsigned long long, unsigned long long> shop::dlc_transformation_goods_by_protector_id = {
    {5040100, 2002010},  // Rock Heart (chest, includes full body)
    {5040200, 2002010},  // Rock Heart (arms, invisible)
    {5040300, 2002010},  // Rock Heart (legs, invisible)
    {5050100, 2002020},  // Priestess Heart (chest, includes full body)
    {5050200, 2002020},  // Priestess Heart (arms, invisible)
    {5050300, 2002020},  // Priestess Heart (legs, invisible)
    {5170100, 2002030},  // Lamenter's Mask (chest, includes full body)
    {5170200, 2002030},  // Lamenter's Mask (arms, invisible)
    {5170300, 2002030},  // Lamenter's Mask (legs, invisible)
};

typedef void AddRemoveItemFn(unsigned long long item_type, unsigned int item_id, int quantity);
static AddRemoveItemFn *add_remove_item = nullptr;

#pragma pack(push, 1)
struct FindShopMenuResult {
    unsigned char shop_type;
    unsigned char padding[3];
    int id;
    er::paramdef::shop_lineup_param *row;
};

struct FindShopLineupParamResult {
    unsigned char shop_type;
    unsigned char padding[3];
    int id;
    er::paramdef::shop_lineup_param *row;
};

struct FindEquipParamGoodsResult {
    int id;
    int unknown;
    er::paramdef::equip_param_goods_st *row;
};
#pragma pack(pop)

static er::paramdef::shop_lineup_param transmog_head_shop_menu = {0};
static er::paramdef::shop_lineup_param transmog_chest_shop_menu = {0};
static er::paramdef::shop_lineup_param transmog_arms_shop_menu = {0};
static er::paramdef::shop_lineup_param transmog_legs_shop_menu = {0};

static unordered_map<int, er::paramdef::shop_lineup_param> transmog_shop_lineups;
static unordered_map<int, er::paramdef::equip_param_goods_st> transmog_goods;

static FindShopMenuResult *(*get_shop_menu)(FindShopMenuResult *result,
                                            unsigned char shop_type,
                                            int begin_id,
                                            int end_id);

FindShopMenuResult *get_shop_menu_detour(FindShopMenuResult *result,
                                         unsigned char shop_type,
                                         int begin_id,
                                         int end_id) {
    switch (begin_id) {
        case shop::transmog_head_shop_menu_id:
            result->shop_type = (unsigned char)0;
            result->id = shop::transmog_head_shop_menu_id;
            result->row = &transmog_head_shop_menu;
            break;
        case shop::transmog_chest_shop_menu_id:
            result->shop_type = (unsigned char)0;
            result->id = shop::transmog_chest_shop_menu_id;
            result->row = &transmog_chest_shop_menu;
            break;
        case shop::transmog_arms_shop_menu_id:
            result->shop_type = (unsigned char)0;
            result->id = shop::transmog_arms_shop_menu_id;
            result->row = &transmog_arms_shop_menu;
            break;
        case shop::transmog_legs_shop_menu_id:
            result->shop_type = (unsigned char)0;
            result->id = shop::transmog_legs_shop_menu_id;
            result->row = &transmog_legs_shop_menu;
            break;
        default:
            get_shop_menu(result, shop_type, begin_id, end_id);
    }

    return result;
}

static void (*get_shop_lineup_param)(FindShopLineupParamResult *result,
                                     unsigned char shop_type,
                                     int id);

/**
 * Check if a protector is available for transmogrification
 */
static inline bool is_protector_unlocked(int goods_id) {
    // If this config option is true, any armor can be chosen without obtaining it first
    if (config::include_unobtained_armor) {
        return true;
    }

    auto main_player = er::CS::WorldChrManImp::instance()->main_player;

    // If the player already chose a transmog, show it even if it's not unlocked. This can happen
    // if they discard the armor piece or change their include_unobtained_armor setting and restart
    // the game.
    if (players::has_item_in_inventory(main_player, shop::item_type_goods_begin + goods_id)) {
        return true;
    }

    auto protector_id = shop::get_protector_id_for_transmog_good(goods_id);

    // Invisible is always unlocked
    if (shop::is_invisible_protector_id(protector_id)) {
        return true;
    }

    // DLC transformations are unlocked by the corresponding goods that activate them, such as
    // the Priestess Heart for dragon mode
    if (shop::dlc_transformation_goods_by_protector_id.contains(protector_id)) {
        auto goods_id = shop::dlc_transformation_goods_by_protector_id.at(protector_id);
        return players::has_item_in_inventory(main_player, shop::item_type_goods_begin + goods_id);
    }

    // Otherwise, only show armor pieces that the player has in their inventory
    if (players::has_item_in_inventory(main_player,
                                       shop::item_type_protector_begin + protector_id)) {
        return true;
    }

    return false;
}

static void get_shop_lineup_param_detour(FindShopLineupParamResult *result,
                                         unsigned char shop_type,
                                         int id) {
    if (shop_type == unsigned char(0) && id >= shop::transmog_head_shop_menu_id &&
        id < shop::transmog_legs_shop_menu_id + shop::transmog_shop_max_size) {
        auto entry = transmog_shop_lineups.find(id);
        if (entry != transmog_shop_lineups.end()) {
            auto &row = entry->second;
            if (is_protector_unlocked(row.equipId)) {
                result->shop_type = shop_type;
                result->id = id;
                result->row = &row;
                return;
            }
        }

        result->row = nullptr;
        return;
    }

    get_shop_lineup_param(result, shop_type, id);
}

static void (*get_equip_param_goods)(FindEquipParamGoodsResult *result, int id);

void get_equip_param_goods_detour(FindEquipParamGoodsResult *result, int id) {
    if (id >= shop::transmog_goods_start_id && id < shop::transmog_goods_end_id) {
        auto transmog_good = transmog_goods.find(id);
        if (transmog_good != transmog_goods.end()) {
            result->id = transmog_good->first;
            result->row = &transmog_good->second;
            result->unknown = 3;
            return;
        }
    }

    get_equip_param_goods(result, id);
}

static void (*open_regular_shop)(void *, unsigned long long, unsigned long long);

static void open_regular_shop_detour(void *unk,
                                     unsigned long long begin_id,
                                     unsigned long long end_id) {
    bool is_transmog_shop = false;

    switch (begin_id) {
        case shop::transmog_head_shop_menu_id:
            msg::set_active_transmog_shop_protector_category(shop::protector_category_head);
            is_transmog_shop = true;
            break;
        case shop::transmog_chest_shop_menu_id:
            msg::set_active_transmog_shop_protector_category(shop::protector_category_chest);
            is_transmog_shop = true;
            break;
        case shop::transmog_arms_shop_menu_id:
            msg::set_active_transmog_shop_protector_category(shop::protector_category_arms);
            is_transmog_shop = true;
            break;
        case shop::transmog_legs_shop_menu_id:
            msg::set_active_transmog_shop_protector_category(shop::protector_category_legs);
            is_transmog_shop = true;
            break;
        default:
            msg::set_active_transmog_shop_protector_category(-1);
    }

    open_regular_shop(unk, begin_id, end_id);

    if (is_transmog_shop) {
        auto game_data_man = er::CS::GameDataMan::instance();
        if (!game_data_man) {
            return;
        }

        // Override the default sort type for the transmog shop to sort by item type
        game_data_man->menu_system_save_load->sorts[er::sort_index_all_items] =
            er::menu_sort::item_type_ascending;

        // Update shop icons to the male or female variants based on the current player
        // character's body type
        auto show_male_icons = game_data_man->player_game_data->body_type ==
                               er::CS::PlayerGameData::body_type_en::type_a;
        for (auto &[goods_id, goods] : transmog_goods) {
            auto protector_id = ertransmogrify::shop::get_protector_id_for_transmog_good(goods_id);
            if (protector_id == -1 || shop::is_invisible_protector_id(protector_id) ||
                shop::dlc_transformation_goods_by_protector_id.contains(protector_id)) {
                continue;
            }

            auto [protector, protector_exists] = er::param::EquipParamProtector[protector_id];
            if (!protector_exists) {
                continue;
            }

            goods.iconId = show_male_icons ? protector.iconIdM : protector.iconIdF;
        }
    }
}

static bool (*add_inventory_from_shop)(int *new_item_id, int quantity) = nullptr;

static bool add_inventory_from_shop_detour(int *item_id_address, int quantity) {
    auto result = add_inventory_from_shop(item_id_address, quantity);

    auto item_id = *item_id_address;
    if (item_id < shop::item_type_goods_begin || item_id >= shop::item_type_goods_end) {
        return result;
    }

    auto transmog_protector_id =
        shop::get_protector_id_for_transmog_good(item_id - shop::item_type_goods_begin);
    if (transmog_protector_id == -1) {
        return result;
    }

    auto main_player = er::CS::WorldChrManImp::instance()->main_player;

    long long prev_transmog_protector_id = -1;

    // Remove any other items of the same category in the player's inventory, so there's
    // only one item for each slot
    auto &transmog_protector = er::param::EquipParamProtector[transmog_protector_id].first;
    for (auto [protector_id, protector] : er::param::EquipParamProtector) {
        if (protector_id != transmog_protector_id &&
            protector.protectorCategory == transmog_protector.protectorCategory) {
            auto goods_id = shop::get_transmog_goods_id_for_protector(protector_id);
            if (players::has_item_in_inventory(main_player,
                                               shop::item_type_goods_begin + goods_id)) {
                add_remove_item(shop::item_type_goods_begin, goods_id, -1);
                prev_transmog_protector_id = protector_id;
            }
        }
    }

    // If any DLC transformation protector is chosen, automatically apply the other protectors in
    // the set. DLC transformations are made up of sets of 3 linked protectors that must be used
    // together.
    auto transformation_goods_id_it =
        shop::dlc_transformation_goods_by_protector_id.find(transmog_protector_id);
    if (transformation_goods_id_it != shop::dlc_transformation_goods_by_protector_id.end()) {
        for (auto [protector_id, protector] : er::param::EquipParamProtector) {
            if (protector.protectorCategory != shop::protector_category_head &&
                protector.protectorCategory != transmog_protector.protectorCategory) {
                auto goods_id = shop::get_transmog_goods_id_for_protector(protector_id);
                auto other_transformation_goods_id_it =
                    shop::dlc_transformation_goods_by_protector_id.find(protector_id);

                if (other_transformation_goods_id_it !=
                        shop::dlc_transformation_goods_by_protector_id.end() &&
                    other_transformation_goods_id_it->second ==
                        transformation_goods_id_it->second) {
                    SPDLOG_INFO("DLC transformation protector {} chosen, also adding {}",
                                transmog_protector_id, protector_id);
                    add_remove_item(shop::item_type_goods_begin, goods_id, +1);
                } else {
                    add_remove_item(shop::item_type_goods_begin, goods_id, -1);
                }
            }
        }
    }
    // Likewise, if a DLC transformation protector is removed, also remove the other two protectors
    else if (shop::dlc_transformation_goods_by_protector_id.contains(prev_transmog_protector_id)) {
        SPDLOG_INFO("DLC transformation protector {} removed! Removing entire set.",
                    prev_transmog_protector_id);

        for (auto [protector_id, protector] : er::param::EquipParamProtector) {
            if (shop::dlc_transformation_goods_by_protector_id.contains(protector_id)) {
                auto goods_id = shop::get_transmog_goods_id_for_protector(protector_id);
                add_remove_item(shop::item_type_goods_begin, goods_id, -1);
            }
        }
    }

    // Ensure the undo transmog effect isn't applied, so the new item is applied
    players::clear_speffect(main_player, vfx::undo_transmog_speffect_id);

    return result;
}

void shop::initialize() {
    add_remove_item = modutils::scan<AddRemoveItemFn>({
        .aob = "8b 99 90 01 00 00"  // mov ebx, [rcx + 0x190] ; param->hostModeCostItemId
               "41 83 c8 ff"        // or r8d, -1
               "8b d3"              // mov edx, ebx
               "b9 00 00 00 40"     // mov ecx, shop::item_type_goods_begin
               "e8 ?? ?? ?? ??",    // call AddRemoveItem
        .offset = 17,
        .relative_offsets = {{1, 5}},
    });
    if (add_remove_item == nullptr) {
        throw runtime_error("Couldn't find AddRemoveItem");
    }

    // Add a shop to "buy" armor pieces for each category. Note: these params control the title
    // and and icon for the shop, but otherwise aren't used for determining shop inventory.
    transmog_head_shop_menu.menuTitleMsgId = msg::menu_text_transmog_head;
    transmog_head_shop_menu.menuIconId = 5;

    transmog_chest_shop_menu.menuTitleMsgId = msg::menu_text_transmog_chest;
    transmog_chest_shop_menu.menuIconId = 5;

    transmog_arms_shop_menu.menuTitleMsgId = msg::menu_text_transmog_arms;
    transmog_arms_shop_menu.menuIconId = 5;

    transmog_legs_shop_menu.menuTitleMsgId = msg::menu_text_transmog_legs;
    transmog_legs_shop_menu.menuIconId = 5;

    // Hook get_shop_menu() to return the above shop menus. The player can select an appearance
    // by buying an item from one of these shops for $0, since this is easier than making a
    // custom menu.
    modutils::hook(
        {
            // Note - the mov instructions are 44 or 45 depending on if this is the Japanese or
            // international .exe, and the stack offset is either -10 or -08. This pattern works
            // for both versions.
            .aob = "?? 8b 4e 14"      // mov r9d, [rsi + 14]
                   "?? 8b 46 10"      // mov r8d, [rsi + 10]
                   "33 d2"            // xor edx, edx
                   "48 8d 4d ??"      // lea rcx, [rbp + ??]
                   "e8 ?? ?? ?? ??",  // call FindShopMenu
            .offset = 14,
            .relative_offsets = {{1, 5}},
        },
        get_shop_menu_detour, get_shop_menu);

    // Add goods and shop entries for every armor piece the player can buy
    for (auto [protector_id, protector_row] : er::param::EquipParamProtector) {
        auto goods_id = get_transmog_goods_id_for_protector(protector_id);

        bool invisible_protector = shop::is_invisible_protector_id(protector_id);
        bool dlc_transformation_protector =
            dlc_transformation_goods_by_protector_id.contains(protector_id);

        unsigned char sort_group_id;
        unsigned short icon_id;
        if (invisible_protector) {
            sort_group_id = 121;
            icon_id = invisible_icon_id;
        } else if (dlc_transformation_protector) {
            auto &goods = er::param::EquipParamGoods[dlc_transformation_goods_by_protector_id.at(
                                                         protector_id)]
                              .first;
            sort_group_id = 150;
            icon_id = goods.iconId;
        } else {
            sort_group_id = (unsigned char)(120 + protector_row.sortGroupId / 10);
            icon_id = protector_row.iconIdM;
        }

        transmog_goods[goods_id] = {
            .sortId = protector_row.sortId,
            .iconId = icon_id,
            .maxNum = 1,
            .goodsType = 13,  // goodsType 13 is hidden from the inventory menu
            .sortGroupId = sort_group_id,
            .saleValue = 0,
        };

        if (exluded_protector_ids.contains(protector_id)) {
            continue;
        }

        // Skip invalid items, and cut items that don't have a name (these are usually
        // duplicates used for NPCs)
        auto [protector_name, protector_is_dlc] = msg::get_protector_data(protector_id);
        if (!dlc_transformation_protector && !known_protector_ids.contains(protector_id) &&
            (protector_name == msg::cut_item_prefix || protector_name.empty())) {
            continue;
        }

        // Skip cut items, if configured to do so
        if (!config::include_cut_armor && protector_name.starts_with(msg::cut_item_prefix)) {
            SPDLOG_DEBUG("Skipping cut content protector {}", protector_id);
            continue;
        }

        // Skip DLC items, if configured to do so
        if (!config::include_dlc_armor && (protector_is_dlc || dlc_transformation_protector)) {
            SPDLOG_DEBUG("Skipping DLC protector {}", protector_id);
            continue;
        }

        int shop_lineup_param_id = -1;
        switch (protector_row.protectorCategory) {
            case protector_category_head:
                shop_lineup_param_id = shop::transmog_head_shop_menu_id + protector_id / 100;
                break;
            case protector_category_chest:
                shop_lineup_param_id = shop::transmog_chest_shop_menu_id + protector_id / 100;
                break;
            case protector_category_arms:
                shop_lineup_param_id = shop::transmog_arms_shop_menu_id + protector_id / 100;
                break;
            case protector_category_legs:
                shop_lineup_param_id = shop::transmog_legs_shop_menu_id + protector_id / 100;
                break;
        }

        transmog_shop_lineups[shop_lineup_param_id] = {
            .equipId = static_cast<int>(goods_id),
            .equipType = 3,
        };
    }

    // Hook get_equip_param_goods() to return the above items
    modutils::hook(
        {
            .aob = "41 8d 50 03"         // lea edx, [r8 + 3]
                   "e8 ?? ?? ?? ??"      // call SoloParamRepositoryImp::GetParamResCap
                   "48 85 c0"            // test rax rax
                   "0f 84 ?? ?? ?? ??",  // jz end_lbl
            .offset = -106,
        },
        get_equip_param_goods_detour, get_equip_param_goods);

    // Hook get_shop_lineup_param() to return the above shop entries
    modutils::hook(
        {
            .aob = "48 8d 15 ?? ?? ?? ??"  // lea rdx, [shop_lineup_param_indexes]
                   "45 33 c0"              // xor r8d, r8d
                   "?? ?? ??"              // ???
                   "e8 ?? ?? ?? ??"        // call SoloParamRepositoryImp::GetParamResCap
                   "48 85 c0"              // test rax, rax
                   "74 ??",                // jz end_lbl
            .offset = -129,
        },
        get_shop_lineup_param_detour, get_shop_lineup_param);

    // Hook add_inventory_from_shop() to apply the transmog VFX when a shop item is chosen
    modutils::hook(
        {
            .aob = "e8 ?? ?? ?? ??"  // call ????????
                   "84 c0"           // test al, al
                   "74 d8"           // je start_label
                   "b0 01",          // mov al, 1
            .offset = -173,
        },
        add_inventory_from_shop_detour, add_inventory_from_shop);

    // Hook OpenRegularShop() to perform some memory hacks when opening up the a transmog shop
    modutils::hook(
        {
            .aob = "4c 8b 49 18"            // mov    r9, [rcx + 0x18]
                   "48 8b d9"               // mov    rbx,rcx
                   "48 8d 4c 24 20"         // lea    rcx, [rsp + 0x20]
                   "e8 ?? ?? ?? ??"         // call   OpenRegularShopInner
                   "48 8d 4c 24 20"         // lea    rcx, [rsp + 0x20]
                   "0f 10 00"               // movups xmm0, [rax]
                   "c7 43 10 05 00 00 00",  // mov    [rbx + 0x10], 5
            .offset = -6,
        },
        open_regular_shop_detour, open_regular_shop);
}

void shop::remove_transmog_goods(signed char protector_category) {
    for (auto [protector_id, protector] : er::param::EquipParamProtector) {
        if (protector_category == -1 || protector.protectorCategory == protector_category) {
            auto goods_id = get_transmog_goods_id_for_protector(protector_id);
            add_remove_item(shop::item_type_goods_begin, goods_id, -1);
        }
    }
}

void shop::add_transmog_good(unsigned long long protector_id) {
    auto goods_id =
        (int)(shop::item_type_goods_begin + get_transmog_goods_id_for_protector(protector_id));
    add_inventory_from_shop(&goods_id, 1);
}

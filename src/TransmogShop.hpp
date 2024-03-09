#pragma once

#include <cstdint>

namespace TransmogShop
{
static constexpr uint32_t item_type_protector_begin = 0x10000000;
static constexpr uint32_t item_type_goods_begin = 0x40000000;
static constexpr uint32_t item_type_goods_end = 0x50000000;
static constexpr uint8_t protector_category_head = 0;
static constexpr uint8_t protector_category_body = 1;
static constexpr uint8_t protector_category_arms = 2;
static constexpr uint8_t protector_category_legs = 3;
static constexpr int64_t bare_head_protector_id = 10000;
static constexpr int64_t bare_body_protector_id = 10100;
static constexpr int64_t bare_arms_protector_id = 10200;
static constexpr int64_t bare_legs_protector_id = 10300;
static constexpr int64_t transmog_goods_start_id = 6900000;
static constexpr int64_t transmog_goods_end_id = 7000000;
static constexpr int64_t transmog_head_shop_menu_id = 4000000;
static constexpr int64_t transmog_body_shop_menu_id = 4100000;
static constexpr int64_t transmog_arms_shop_menu_id = 4200000;
static constexpr int64_t transmog_legs_shop_menu_id = 4300000;
static constexpr int64_t transmog_shop_max_size = 100000;

/**
 * Hook the internal get_*_param() functions to return the new params used for the transmog shops
 */
void initialize();

/**
 * Remove transmog shop items from the player's inventory
 */
void remove_transmog_goods(int8_t protector_category = -1);

/**
 * Add a transmog good to the player's inventory
 */
void add_transmog_good(uint64_t protector_id);

inline bool is_invisible_protector_id(int64_t protector_id)
{
    return protector_id == bare_head_protector_id || protector_id == bare_body_protector_id ||
           protector_id == bare_arms_protector_id || protector_id == bare_legs_protector_id;
}

inline int64_t get_transmog_goods_id_for_protector(int64_t protector_id)
{
    return transmog_goods_start_id + protector_id / 100;
}

inline int64_t get_protector_id_for_transmog_good(int64_t goods_id)
{
    if (goods_id >= transmog_goods_start_id && goods_id < transmog_goods_end_id)
    {
        return 100 * (goods_id - transmog_goods_start_id);
    }

    return -1;
}

} // namespace TransmogShop

#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <tga/paramdefs.h>

typedef std::map<std::wstring, std::map<std::uint64_t, void *>> ParamMap;

namespace TransmogShop
{
static const std::int64_t transmog_head_shop_menu_id = 4000000;
static const std::int64_t transmog_body_shop_menu_id = 4100000;
static const std::int64_t transmog_arms_shop_menu_id = 4200000;
static const std::int64_t transmog_legs_shop_menu_id = 4300000;
static const std::int64_t transmog_shop_max_size = 100000;

/**
 * Hook the internal get_*_param() functions to return the new params used for the transmog shops
 */
void initialize(ParamMap &params);
void deinitialize();

} // namespace TransmogShop

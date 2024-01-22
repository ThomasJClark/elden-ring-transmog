#include "ExportMod.hpp"
#include "GameHook.hpp"

static GameHook game_hook;
static GetMessageFn get_message_override;

static void initialize_mod()
{
    game_hook.initialize("eldenring.exe");
    game_hook.hook_get_message(get_message_override);
}

static wchar_t const *get_message_override(MsgRepository *msg_repository, std::uint32_t unknown,
                                           MsgBndId bnd_id, std::int32_t msg_id)
{
    if (bnd_id == MsgBndId::weapon_name)
    {
        return L"WEAPON NAME";
    }
    else if (bnd_id == MsgBndId::weapon_caption)
    {
        return L"WEAPON CAPTION";
    }

    return game_hook.get_message_original(msg_repository, unknown, bnd_id, msg_id);
}

static void deinitialize_mod()
{
    game_hook.unhook_get_message();
}

export_mod(initialize_mod, deinitialize_mod);

#include <functional>
#include <iomanip>
#include <iostream>

#include "BaseMod.hpp"
#include "GameHook.hpp"
#include "MinHook.h"

#pragma pack(1)

static GetMessageFn *get_message_original;

class EldenRingLoadoutMod : public BaseMod
{
  public:
    void initialize()
    {
        game_hook.initialize("eldenring.exe");

        auto mh_status = MH_Initialize();
        if (mh_status != MH_OK)
        {
            throw std::runtime_error(MH_StatusToString(mh_status));
        }

        mh_status = MH_CreateHook(game_hook.get_message, &get_message_override,
                                  reinterpret_cast<void **>(&get_message_original));
        if (mh_status != MH_OK)
        {
            throw std::runtime_error(MH_StatusToString(mh_status));
        }

        mh_status = MH_EnableHook(game_hook.get_message);
        if (mh_status != MH_OK)
        {
            throw std::runtime_error(MH_StatusToString(mh_status));
        }
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

        return get_message_original(msg_repository, unknown, bnd_id, msg_id);
    }

    void deinitialize()
    {
        auto mh_status = MH_DisableHook(game_hook.get_message);
        if (mh_status != MH_OK)
        {
            throw std::runtime_error(MH_StatusToString(mh_status));
        }

        mh_status = MH_RemoveHook(game_hook.get_message);
        if (mh_status != MH_OK)
        {
            throw std::runtime_error(MH_StatusToString(mh_status));
        }
    }

  private:
    GameHook game_hook;
};

EXPORT_MOD(EldenRingLoadoutMod);

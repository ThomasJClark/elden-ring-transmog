#include <iomanip>
#include <iostream>

#include "BaseMod.hpp"
#include "GameHook.hpp"

#pragma pack(1)

static const std::vector<int> g_msg_repository_aob = {0x48, 0x8B, 0x3D, -1,   -1,   -1,   -1,  0x44,
                                                      0x0F, 0xB6, 0x30, 0x48, 0x85, 0xFF, 0x75};

typedef const wchar_t *getMessage_t(void *MsgRepository, uint32_t unk, uint32_t bndId, int msgId);
static getMessage_t *getMessage = nullptr;

extern "C" struct MsgRepositoryCategory
{
};

extern "C" struct MsgRepository
{
    void **vftable_ptr;
    void *categories;
    std::uint32_t version_count;
    std::uint32_t category_capacity;
    std::byte unknown[8];
};

void simple_hex_dump(void *addr, size_t count)
{
    for (int i = 0; i < count; i++)
    {
        if (i % 16 == 0)
            std::cout << std::hex << std::setfill('0') << std::setw(4) << i << " ";
        std::cout << std::hex << std::setfill('0') << std::setw(2)
                  << (int)reinterpret_cast<std::byte *>(addr)[i];
        if (i % 16 == 15)
            std::cout << std::endl;
        else if (i % 8 == 7)
            std::cout << "  ";
        else
            std::cout << " ";
    }
}

class EldenRingLoadoutMod : public BaseMod
{
  public:
    void initialize()
    {
        game_hook.initialize("eldenring.exe");
        msg_repository_address =
            reinterpret_cast<MsgRepository **>(game_hook.scan(g_msg_repository_aob, {{3, 7}}));
    }

    void update()
    {
        std::cout << "msg_repository_address " << msg_repository_address << std::endl;
        if (msg_repository_address == nullptr)
        {
            return;
        }

        auto msg_repository = *msg_repository_address;
        std::cout << "msg_repository " << msg_repository << std::endl;
        if (msg_repository == nullptr)
        {
            return;
        }

        std::cout << "msg_repository.categories " << msg_repository->categories << std::endl;
        std::cout << "msg_repository.version_count " << msg_repository->version_count << std::endl;
        std::cout << "msg_repository.category_capacity " << msg_repository->category_capacity
                  << std::endl;
        std::cout << std::endl;
    }

    void deinitialize()
    {
    }

  private:
    GameHook game_hook;
    MsgRepository **msg_repository_address;
};

EXPORT_MOD(EldenRingLoadoutMod);

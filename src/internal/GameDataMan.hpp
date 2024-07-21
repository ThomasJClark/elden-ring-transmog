#include <array>

// There are many other options, I only care about the sort by type option
enum class menu_sort : unsigned int
{
    item_type_ascending = 0x80005141
};

size_t sort_index_all_items = 12;

namespace CS
{

class CSMenuSystemSaveLoad
{
  public:
    virtual ~CSMenuSystemSaveLoad() = default;

    unsigned char unk[0x1438];

    std::array<menu_sort, 20> sorts;
};

class GameDataMan
{
  public:
    virtual ~GameDataMan() = default;

    unsigned char unk[0x58];

    CSMenuSystemSaveLoad *menu_system_save_load;
};

}
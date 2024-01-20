#include <cmath>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <unordered_set>

#include "tga/paramdefs.h"

#include "BaseMod.hpp"
#include "GameHook.hpp"

using namespace std::chrono_literals;

/**
 * Experimental mod to fix the spell scaling display in The Convergence (mostly for testing that
 * the calculator works)
 */
class ConvergenceSpellScalingMod : public BaseMod
{
  public:
    void initialize()
    {
        std::this_thread::sleep_for(15s);

        game_hook.initialize("eldenring.exe");
        auto equipParamWeapon = game_hook.get_param(L"EquipParamWeapon");
        auto reinforceParamWeapon = game_hook.get_param(L"ReinforceParamWeapon");
        auto atkParamPc = game_hook.get_param(L"AtkParam_Pc");

        std::unordered_set<uint64_t> catalystReinforceTypeIds;

        for (auto [id, row] : equipParamWeapon->rows)
        {
            auto equipParamWeaponRow = reinterpret_cast<EquipParamWeapon *>(row);

            if (equipParamWeaponRow->enableMagic || equipParamWeaponRow->enableMiracle)
            {
                // Update all catalysts to have 100 base attack. Spell scaling is calculated like
                // attack power but with 100 base, so this repurposes the attack power calculation
                // to show spell scaling for each damage type.
                equipParamWeaponRow->attackBasePhysics = 100;
                equipParamWeaponRow->attackBaseMagic = 100;
                equipParamWeaponRow->attackBaseFire = 100;
                equipParamWeaponRow->attackBaseThunder = 100;
                equipParamWeaponRow->attackBaseDark = 100;

                catalystReinforceTypeIds.insert(equipParamWeaponRow->reinforceTypeId);

                std::wcout << "Patched " << equipParamWeapon->name << " " << id << std::endl;
            }
        }

        for (auto [id, row] : reinforceParamWeapon->rows)
        {
            auto reinforceParamWeaponRow = reinterpret_cast<ReinforceParamWeapon *>(row);

            auto reinforceTypeId = id - id % 100;
            if (catalystReinforceTypeIds.contains(reinforceTypeId))
            {
                // Set a base reinforcement equal to the normal physical/magic/fire/lightning/holy
                // reinforcement rate. This allows melee damage to increase from upgrading the
                // catalyst without impacting the attack power.
                reinforceParamWeaponRow->baseAtkRate = reinforceParamWeaponRow->physicsAtkRate;

                // Remove reinforcement for catalyst base attack, so it always stays at 100
                reinforceParamWeaponRow->physicsAtkRate = 1.0f;
                reinforceParamWeaponRow->magicAtkRate = 1.0f;
                reinforceParamWeaponRow->fireAtkRate = 1.0f;
                reinforceParamWeaponRow->thunderAtkRate = 1.0f;
                reinforceParamWeaponRow->darkAtkRate = 1.0f;

                std::wcout << "Patched " << reinforceParamWeapon->name << " " << id << std::endl;
            }
        }

        // Adjust motion values so catalysts don't actually deal a massive amount of damage on
        // melee  hits
        for (auto [id, row] : atkParamPc->rows)
        {
            auto attackParamRow = reinterpret_cast<AtkParam_Pc *>(row);

            if (id >= 3300000 && id < 3500000)
            {
                // Allow damage to increase from catalyst upgrades without impacting the attack
                // power.
                attackParamRow->isAddBaseAtk = 1;

                // Most catalysts have base damage of about 25. Multiplying the motion value by
                // 0.25 cancels out increasing in base same to 100.
                attackParamRow->atkPhysCorrection =
                    static_cast<uint16_t>(std::round(0.25 * attackParamRow->atkPhysCorrection));

                // Clear out all other motion values, so catalyts don't do an instance of melee
                // damage for every damage type.
                attackParamRow->atkMagCorrection = 0;
                attackParamRow->atkFireCorrection = 0;
                attackParamRow->atkThunCorrection = 0;
                attackParamRow->atkDarkCorrection = 0;

                std::wcout << "Patched " << atkParamPc->name << " " << id << std::endl;
            }
        }
    }

  private:
    GameHook game_hook;
};

EXPORT_MOD(ConvergenceSpellScalingMod);

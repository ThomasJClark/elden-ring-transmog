using SoulsFormats;

/**
 * Utilities for generating EMEVD events
 *
 * These events are normally written in DarkScript/MattScript. However, to make this mod
 * automatically generated (which is necessary due to large number of entries necessary to transmog
 * any combination of armors), we can generate the instructions using SoulsFormats.
 */
class TransmogEMEVDUtils
{
    /**
     * Returns an EMEVD instruction to intialize an event to undo a single transmog.
     */
    public static EMEVD.Instruction BuildUntransmogInitializeInstruction(
        int eventSlotId,
        int eventId,
        int armorType,
        int armorId,
        int grantedArmorId
    )
    {
        var binaryWriter = new BinaryWriterEx(false);
        binaryWriter.WriteInt32(eventSlotId);
        binaryWriter.WriteInt32(eventId);
        binaryWriter.WriteInt32(armorType);
        binaryWriter.WriteInt32(armorId);
        binaryWriter.WriteInt32(grantedArmorId);

        return new EMEVD.Instruction(2000, 0) // InitializeEvent
        {
            ArgData = binaryWriter.FinishBytes()
        };
    }

    /**
     * Returns an EMEVD event that undoes a transmog when an speffect is applied to the player
     * character, and applies a visual effect
     *
     * Original source:
     *
     *     $Event(id, Default, function(X0_4, X4_4, X8_4) {
     *         WaitFor(PlayerHasArmorEquipped(X0_4, X4_4, -1) && CharacterHasSpEffect(10000, spEffectId));
     *         if (X0_4 == ArmorType.Head) {
     *             SpawnOneshotSFX(TargetEntityType.Character, 10000, 907, 300552);
     *         } else if (X0_4 == ArmorType.Body) {
     *             SpawnOneshotSFX(TargetEntityType.Character, 10000, 190, 300552);
     *         } else if (X0_4 == ArmorType.Arms) {
     *             SpawnOneshotSFX(TargetEntityType.Character, 10000, 192, 300552);
     *             SpawnOneshotSFX(TargetEntityType.Character, 10000, 1, 300552);
     *             SpawnOneshotSFX(TargetEntityType.Character, 10000, 2, 300552);
     *         } else if (X0_4 == ArmorType.Legs) {
     *             SpawnOneshotSFX(TargetEntityType.Character, 10000, 193, 300552);
     *             SpawnOneshotSFX(TargetEntityType.Character, 10000, 212, 300552);
     *             SpawnOneshotSFX(TargetEntityType.Character, 10000, 5, 300552);
     *             SpawnOneshotSFX(TargetEntityType.Character, 10000, 6, 300552);
     *         }
     *
     *         SpawnOneshotSFX(TargetEntityType.Character, 10000, 2, 636505);
     *
     *         RemoveItemFromPlayer(ItemType.Armor, X4_4, 1);
     *         DirectlyGivePlayerItem(ItemType.Armor, X8_4, 6001, 1);
     *         RestartEvent();
     *     });
     */
    public static EMEVD.Event BuildUntransmogEvent(int eventId, int spEffectId)
    {
        return new EMEVD.Event(eventId, EMEVD.Event.RestBehaviorType.Default)
        {
            Parameters = new List<EMEVD.Parameter>
            {
                new(0, 1, 0, 4), // armorType (X0_4)
                new(3, 4, 0, 4), // armorType (X0_4)
                new(6, 4, 0, 4), // armorType (X0_4)
                new(9, 4, 0, 4), // armorType (X0_4)
                new(14, 4, 0, 4), // armorType (X0_4)
                new(0, 4, 4, 4), // armorId (X4_4)
                new(20, 4, 4, 4), // armorId (X4_4)
                new(21, 4, 8, 4) // grantedArmorId (X8_4)
            },
            Instructions = new List<EMEVD.Instruction>
            {
                // IfPlayerHasArmorEquipped
                new(3, 34)
                {
                    // AND_01, armorType, armorType, -1
                    ArgData = new byte[]
                    {
                        0x01,
                        0x00,
                        0x00,
                        0x00,
                        0x00,
                        0x00,
                        0x00,
                        0x00,
                        0xFF,
                        0xFF,
                        0xFF,
                        0xFF
                    }
                },
                // IfCharacterHasSpEffect
                new(4, 5)
                {
                    // AND_01, 10000, spEffectId, true, Equal, 1
                    ArgData = new byte[]
                    {
                        0x01,
                        0x00,
                        0x00,
                        0x00,
                        0x10,
                        0x27,
                        0x00,
                        0x00,
                        (byte)((spEffectId >> 0) & 0xff),
                        (byte)((spEffectId >> 8) & 0xff),
                        (byte)((spEffectId >> 16) & 0xff),
                        (byte)((spEffectId >> 24) & 0xff),
                        0x01,
                        0x00,
                        0x00,
                        0x00,
                        0x00,
                        0x00,
                        0x80,
                        0x3F
                    }
                },
                // IfConditionGroup
                new(0, 0)
                {
                    // MAIN, PASS, AND_01
                    ArgData = new byte[] { 0x00, 0x01, 0x01, 0x00 }
                },
                // SkipIfUnsignedComparison
                new(1000, 10)
                {
                    // 2, NotEqual, armorType, ArmorType.Head
                    ArgData = new byte[]
                    {
                        0x02,
                        0x01,
                        0x00,
                        0x00,
                        0x00,
                        0x00,
                        0x00,
                        0x00,
                        0x00,
                        0x00,
                        0x00,
                        0x00
                    }
                },
                // SpawnOneshotSFX
                new(2006, 3)
                {
                    // TargetEntityType.Character, 10000, 907, 300552
                    ArgData = new byte[]
                    {
                        0x02,
                        0x00,
                        0x00,
                        0x00,
                        0x10,
                        0x27,
                        0x00,
                        0x00,
                        0x8B,
                        0x03,
                        0x00,
                        0x00,
                        0x08,
                        0x96,
                        0x04,
                        0x00
                    }
                },
                // SkipUnconditionally
                new(1000, 3)
                {
                    // 13
                    ArgData = new byte[] { 0x0D, 0x00, 0x00, 0x00 }
                },
                // SkipIfUnsignedComparison
                new(1000, 10)
                {
                    // 2, NotEqual, armorType, ArmorType.Body
                    ArgData = new byte[]
                    {
                        0x02,
                        0x01,
                        0x00,
                        0x00,
                        0x00,
                        0x00,
                        0x00,
                        0x00,
                        0x01,
                        0x00,
                        0x00,
                        0x00
                    }
                },
                // SpawnOneshotSFX
                new(2006, 3)
                {
                    // TargetEntityType.Character, 10000, 190, 300552
                    ArgData = new byte[]
                    {
                        0x02,
                        0x00,
                        0x00,
                        0x00,
                        0x10,
                        0x27,
                        0x00,
                        0x00,
                        0xBE,
                        0x00,
                        0x00,
                        0x00,
                        0x08,
                        0x96,
                        0x04,
                        0x00
                    }
                },
                // SkipUnconditionally
                new(1000, 3)
                {
                    // 10
                    ArgData = new byte[] { 0x0A, 0x00, 0x00, 0x00 }
                },
                // SkipIfUnsignedComparison
                new(1000, 10)
                {
                    // 4, NotEqual, armorType, ArmorType.Arms
                    ArgData = new byte[]
                    {
                        0x04,
                        0x01,
                        0x00,
                        0x00,
                        0x00,
                        0x00,
                        0x00,
                        0x00,
                        0x02,
                        0x00,
                        0x00,
                        0x00
                    }
                },
                // SpawnOneshotSFX
                new(2006, 3)
                {
                    // TargetEntityType.Character, 10000, 192, 300552
                    ArgData = new byte[]
                    {
                        0x02,
                        0x00,
                        0x00,
                        0x00,
                        0x10,
                        0x27,
                        0x00,
                        0x00,
                        0xC0,
                        0x00,
                        0x00,
                        0x00,
                        0x08,
                        0x96,
                        0x04,
                        0x00
                    }
                },
                // SpawnOneshotSFX
                new(2006, 3)
                {
                    // TargetEntityType.Character, 10000, 1, 300552
                    ArgData = new byte[]
                    {
                        0x02,
                        0x00,
                        0x00,
                        0x00,
                        0x10,
                        0x27,
                        0x00,
                        0x00,
                        0x01,
                        0x00,
                        0x00,
                        0x00,
                        0x08,
                        0x96,
                        0x04,
                        0x00
                    }
                },
                // SpawnOneshotSFX
                new(2006, 3)
                {
                    // TargetEntityType.Character, 10000, 2, 300552
                    ArgData = new byte[]
                    {
                        0x02,
                        0x00,
                        0x00,
                        0x00,
                        0x10,
                        0x27,
                        0x00,
                        0x00,
                        0x02,
                        0x00,
                        0x00,
                        0x00,
                        0x08,
                        0x96,
                        0x04,
                        0x00
                    }
                },
                // SkipUnconditionally
                new(1000, 3)
                {
                    // 5
                    ArgData = new byte[] { 0x05, 0x00, 0x00, 0x00 }
                },
                // SkipIfUnsignedComparison
                new(1000, 10)
                {
                    // 4, NotEqual, armorType, ArmorType.Legs
                    ArgData = new byte[]
                    {
                        0x04,
                        0x01,
                        0x00,
                        0x00,
                        0x00,
                        0x00,
                        0x00,
                        0x00,
                        0x03,
                        0x00,
                        0x00,
                        0x00
                    }
                },
                // SpawnOneshotSFX
                new(2006, 3)
                {
                    // TargetEntityType.Character, 10000, 193, 300552
                    ArgData = new byte[]
                    {
                        0x02,
                        0x00,
                        0x00,
                        0x00,
                        0x10,
                        0x27,
                        0x00,
                        0x00,
                        0xC1,
                        0x00,
                        0x00,
                        0x00,
                        0x08,
                        0x96,
                        0x04,
                        0x00
                    }
                },
                // SpawnOneshotSFX
                new(2006, 3)
                {
                    // TargetEntityType.Character, 10000, 212, 300552
                    ArgData = new byte[]
                    {
                        0x02,
                        0x00,
                        0x00,
                        0x00,
                        0x10,
                        0x27,
                        0x00,
                        0x00,
                        0xD4,
                        0x00,
                        0x00,
                        0x00,
                        0x08,
                        0x96,
                        0x04,
                        0x00
                    }
                },
                // SpawnOneshotSFX
                new(2006, 3)
                {
                    // TargetEntityType.Character, 10000, 5, 300552
                    ArgData = new byte[]
                    {
                        0x02,
                        0x00,
                        0x00,
                        0x00,
                        0x10,
                        0x27,
                        0x00,
                        0x00,
                        0x05,
                        0x00,
                        0x00,
                        0x00,
                        0x08,
                        0x96,
                        0x04,
                        0x00
                    }
                },
                // SpawnOneshotSFX
                new(2006, 3)
                {
                    // TargetEntityType.Character, 10000, 6, 300552
                    ArgData = new byte[]
                    {
                        0x02,
                        0x00,
                        0x00,
                        0x00,
                        0x10,
                        0x27,
                        0x00,
                        0x00,
                        0x06,
                        0x00,
                        0x00,
                        0x00,
                        0x08,
                        0x96,
                        0x04,
                        0x00
                    }
                },
                // SpawnOneshotSFX
                new(2006, 3)
                {
                    // TargetEntityType.Character, 10000, 2, 636505
                    ArgData = new byte[]
                    {
                        0x02,
                        0x00,
                        0x00,
                        0x00,
                        0x10,
                        0x27,
                        0x00,
                        0x00,
                        0x02,
                        0x00,
                        0x00,
                        0x00,
                        0x59,
                        0xB6,
                        0x09,
                        0x00
                    }
                },
                // RemoveItemFromPlayer
                new(2003, 24)
                {
                    // ItemType.Armor, armorType, 1
                    ArgData = new byte[]
                    {
                        0x01,
                        0x00,
                        0x00,
                        0x00,
                        0x00,
                        0x00,
                        0x00,
                        0x00,
                        0x01,
                        0x00,
                        0x00,
                        0x00
                    }
                },
                // DirectlyGivePlayerItem
                new(2003, 43)
                {
                    // ItemType.Armor, grantedArmorId, 6001, 1
                    ArgData = new byte[]
                    {
                        0x01,
                        0x00,
                        0x00,
                        0x00,
                        0x00,
                        0x00,
                        0x00,
                        0x00,
                        0x71,
                        0x17,
                        0x00,
                        0x00,
                        0x01,
                        0x00,
                        0x00,
                        0x00
                    }
                },
                // EndUnconditionally
                new(1000, 4)
                {
                    // EventEndType.Restart
                    ArgData = new byte[] { 0x01, 0x00, 0x00, 0x00 }
                }
            }
        };
    }
}

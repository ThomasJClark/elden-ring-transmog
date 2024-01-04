using SoulsFormats;

/**
 * Utilities for generating EMEVD events
 *
 * These events are normally written in DarkScript/MattScript. However, to make this mod
 * automatically generated (which is necessary due to large number of entries necessary to transmog
 * any combination of armors), we can generate the instructions using SoulsFormats.
 *
 * See https://soulsmods.github.io/emedf/er-emedf.html
 */
class TransmogEventUtils
{
    public static readonly sbyte MAIN = 0;
    public static readonly sbyte OR_01 = -1;
    public static readonly sbyte EQUAL = 0;
    public static readonly byte OFF = 0;
    public static readonly byte CONDITION_STATE_FAIL = 0;
    public static readonly sbyte EXECUTION_END_TYPE_RESTART = 1;
    public static readonly byte ITEM_TYPE_ARMOR = 1;
    public static readonly byte ITEM_TYPE_GOOD = 3;
    public static readonly int ENTITY_TYPE_CHARACTER = 2;
    public static readonly uint ENTITY_PLAYER = 10000;
    public static readonly byte EVENT_FLAG = 0;

    public static EMEVD.Instruction IfPlayerHasArmorEquipped(
        sbyte resultConditionGroup,
        byte armorType,
        int armorItemId
    )
    {
        var binaryWriter = new BinaryWriterEx(false);
        binaryWriter.WriteSByte(resultConditionGroup);
        binaryWriter.WriteByte(armorType);
        binaryWriter.WriteByte(0);
        binaryWriter.WriteByte(0);
        binaryWriter.WriteInt32(armorItemId);
        binaryWriter.WriteInt32(-1);
        return new()
        {
            Bank = 3,
            ID = 34,
            ArgData = binaryWriter.FinishBytes()
        };
    }

    public static EMEVD.Instruction IfCharacterHasSpEffect(
        sbyte resultConditionGroup,
        uint targetEntityId,
        int spEffectId,
        bool shouldHave,
        sbyte comparisonType,
        float numberOfTargetCharacters
    )
    {
        var binaryWriter = new BinaryWriterEx(false);
        binaryWriter.WriteSByte(resultConditionGroup);
        binaryWriter.WriteByte(0);
        binaryWriter.WriteByte(0);
        binaryWriter.WriteByte(0);
        binaryWriter.WriteUInt32(targetEntityId);
        binaryWriter.WriteInt32(spEffectId);
        binaryWriter.WriteByte(shouldHave ? (byte)1 : (byte)0);
        binaryWriter.WriteSByte(comparisonType);
        binaryWriter.WriteByte(0);
        binaryWriter.WriteByte(0);
        binaryWriter.WriteSingle(numberOfTargetCharacters);
        return new()
        {
            Bank = 4,
            ID = 5,
            ArgData = binaryWriter.FinishBytes()
        };
    }

    public static EMEVD.Instruction SkipIfConditionGroupStateUncompiled(
        byte numberOfSkippedLines,
        byte desiredConditionGroupState,
        sbyte targetConditionGroup
    )
    {
        var binaryWriter = new BinaryWriterEx(false);
        binaryWriter.WriteByte(numberOfSkippedLines);
        binaryWriter.WriteByte(desiredConditionGroupState);
        binaryWriter.WriteSByte(targetConditionGroup);
        binaryWriter.WriteByte(0);
        return new()
        {
            Bank = 1000,
            ID = 1,
            ArgData = binaryWriter.FinishBytes()
        };
    }

    public static EMEVD.Instruction EndUnconditionally(sbyte executionEndType)
    {
        var binaryWriter = new BinaryWriterEx(false);
        binaryWriter.WriteSByte(executionEndType);
        binaryWriter.WriteByte(0);
        binaryWriter.WriteByte(0);
        binaryWriter.WriteByte(0);
        return new()
        {
            Bank = 1000,
            ID = 4,
            ArgData = binaryWriter.FinishBytes()
        };
    }

    public static EMEVD.Instruction WaitFixedTimeFrames(int numberOfFrames)
    {
        var binaryWriter = new BinaryWriterEx(false);
        binaryWriter.WriteInt32(numberOfFrames);
        return new()
        {
            Bank = 1001,
            ID = 1,
            ArgData = binaryWriter.FinishBytes()
        };
    }

    public static EMEVD.Instruction SkipIfEventFlag(
        byte numberOfSkippedLines,
        byte desiredFlagState,
        byte targetEventFlagType,
        int targetEventFlagId
    )
    {
        var binaryWriter = new BinaryWriterEx(false);
        binaryWriter.WriteByte(numberOfSkippedLines);
        binaryWriter.WriteByte(desiredFlagState);
        binaryWriter.WriteByte(targetEventFlagType);
        binaryWriter.WriteByte(0);
        binaryWriter.WriteInt32(targetEventFlagId);
        return new()
        {
            Bank = 1003,
            ID = 1,
            ArgData = binaryWriter.FinishBytes()
        };
    }

    public static EMEVD.Instruction InitializeEvent(
        int eventSlotId,
        int eventId,
        params int[] parameters
    )
    {
        var binaryWriter = new BinaryWriterEx(false);
        binaryWriter.WriteInt32(eventSlotId);
        binaryWriter.WriteInt32(eventId);
        binaryWriter.WriteInt32s(parameters);
        return new()
        {
            Bank = 2000,
            ID = 0,
            ArgData = binaryWriter.FinishBytes()
        };
    }

    public static EMEVD.Instruction RemoveItemFromPlayer(byte itemType, int itemId, int number)
    {
        var binaryWriter = new BinaryWriterEx(false);
        binaryWriter.WriteByte(itemType);
        binaryWriter.WriteByte(0);
        binaryWriter.WriteByte(0);
        binaryWriter.WriteByte(0);
        binaryWriter.WriteInt32(itemId);
        binaryWriter.WriteInt32(number);
        return new()
        {
            Bank = 2003,
            ID = 24,
            ArgData = binaryWriter.FinishBytes()
        };
    }

    public static EMEVD.Instruction DirectlyGivePlayerItem(
        byte itemType,
        int itemId,
        uint baseEventFlagId,
        uint numberOfUsedFlagBits
    )
    {
        var binaryWriter = new BinaryWriterEx(false);
        binaryWriter.WriteByte(itemType);
        binaryWriter.WriteByte(0);
        binaryWriter.WriteByte(0);
        binaryWriter.WriteByte(0);
        binaryWriter.WriteInt32(itemId);
        binaryWriter.WriteUInt32(baseEventFlagId);
        binaryWriter.WriteUInt32(numberOfUsedFlagBits);
        return new()
        {
            Bank = 2003,
            ID = 43,
            ArgData = binaryWriter.FinishBytes()
        };
    }

    public static EMEVD.Instruction SetEventFlag(
        byte targetEventFlagType,
        int targetEventFlagId,
        byte desiredFlagState
    )
    {
        var binaryWriter = new BinaryWriterEx(false);
        binaryWriter.WriteByte(targetEventFlagType);
        binaryWriter.WriteByte(0);
        binaryWriter.WriteByte(0);
        binaryWriter.WriteByte(0);
        binaryWriter.WriteInt32(targetEventFlagId);
        binaryWriter.WriteByte(desiredFlagState);
        binaryWriter.WriteByte(0);
        binaryWriter.WriteByte(0);
        binaryWriter.WriteByte(0);
        return new()
        {
            Bank = 2003,
            ID = 66,
            ArgData = binaryWriter.FinishBytes()
        };
    }

    /**
     * For debugging
     */
    public static EMEVD.Instruction DisplayBanner(byte bannerType)
    {
        var binaryWriter = new BinaryWriterEx(false);
        binaryWriter.WriteByte(bannerType);
        binaryWriter.WriteByte(0);
        binaryWriter.WriteByte(0);
        binaryWriter.WriteByte(0);
        return new()
        {
            Bank = 2007,
            ID = 2,
            ArgData = binaryWriter.FinishBytes()
        };
    }
}

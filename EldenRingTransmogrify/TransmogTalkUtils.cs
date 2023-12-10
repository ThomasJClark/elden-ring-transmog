using SoulsFormats;

/**
 * Utilities for updating dialogue trees (talkesd).
 *
 * This is the entrypoint to get into the transmog menus or undo transmogrification from a Site of
 * Grace or the mirror in Roundtable Hold. Normally these menus can be manually editing using
 * fifthmatt's ESDTool, but doing so in code allows this mod to be automatically generated with
 * many variants and frequent compatibility updates. ESDTool also seems to choke on some mods, like
 * Elden Ring Reforged.
 *
 * (I don't actually understand how most of this bytecode works, I just looked at the ESDTool
 * output and tried to automate the same changes)
 */
class TransmogTalkUtils
{
    private static readonly int ADD_TALK_LIST_DATA_BANK = 1;
    private static readonly int ADD_TALK_LIST_DATA_COMMAND = 19;
    private static readonly int CUSTOM_BANK = 6;

    public static void AddTransmogMenuOption(
        Dictionary<long, Dictionary<long, ESD.State>> stateGroups,
        int transmogStateGroupID,
        int targetTalkTextID,
        int transmogTalkTextID
    )
    {
        // Find the state for the menu, and update it to have a Transmogrify Armor option.
        foreach (var (stateGroupID, stateGroup) in stateGroups)
        {
            // The menu is indentified by looking for a known vanilla menu option
            var talkListState = stateGroup.Values.FirstOrDefault(
                (state) =>
                    state.EntryCommands.Any(command => IsAddTalkListData(command, targetTalkTextID))
            );

            if (talkListState != null)
            {
                var transmogMenuOptionID = 69;
                var transmogStateID = 0x100;
                var initialStateID = 0x01;

                // Add a new option to the menu that opens the Transmogrify Armor menu specified
                // in transmogStateGroupID
                talkListState.EntryCommands.Add(
                    AddTalkListData(transmogMenuOptionID, transmogTalkTextID, -1)
                );
                stateGroup[0x08].Conditions.Add(
                    TalkListEntryResultCondition(transmogMenuOptionID, transmogStateID)
                );
                stateGroup[transmogStateID] = AwaitCall(transmogStateGroupID, initialStateID);

                return;
            }
        }

        throw new Exception("Couldn't find state to add menu option to");
    }

    private static bool IsAddTalkListData(ESD.CommandCall command, int talkTextId)
    {
        if (
            command.CommandBank == ADD_TALK_LIST_DATA_BANK
            && command.CommandID == ADD_TALK_LIST_DATA_COMMAND
        )
        {
            BinaryReaderEx talkTextIdArg = new(false, command.Arguments[1]);
            talkTextIdArg.Skip(1);
            return talkTextId == talkTextIdArg.ReadInt32();
        }

        return false;
    }

    private static ESD.CommandCall AddTalkListData(int menuId, int talkTextId, int unk)
    {
        BinaryWriterEx menuOptionIdArg = new(false);
        menuOptionIdArg.WriteByte(0x82);
        menuOptionIdArg.WriteInt32(menuId);
        menuOptionIdArg.WriteByte(0xa1);

        BinaryWriterEx talkTextIdArg = new(false);
        talkTextIdArg.WriteByte(0x82);
        talkTextIdArg.WriteInt32(talkTextId);
        talkTextIdArg.WriteByte(0xa1);

        BinaryWriterEx unkArg = new(false);
        unkArg.WriteByte(0x82);
        unkArg.WriteInt32(unk);
        unkArg.WriteByte(0xa1);

        return new(
            ADD_TALK_LIST_DATA_BANK,
            ADD_TALK_LIST_DATA_COMMAND,
            menuOptionIdArg.FinishBytes(),
            talkTextIdArg.FinishBytes(),
            unkArg.FinishBytes()
        );
    }

    private static ESD.Condition TalkListEntryResultCondition(int menuOptionID, int stateID)
    {
        BinaryWriterEx evaluator = new(false);
        evaluator.WriteBytes(new byte[] { 0x57, 0x84, 0x82 });
        evaluator.WriteInt32(menuOptionID);
        evaluator.WriteBytes(new byte[] { 0x95, 0xa1 });
        return new(stateID, evaluator.FinishBytes());
    }

    private static ESD.State AwaitCall(int stateGroupID, int nextStateID)
    {
        return new()
        {
            Conditions = { new(nextStateID, new byte[] { 0xb9, 0xba, 0x96, 0xa1 }) },
            EntryCommands = { new(CUSTOM_BANK, stateGroupID) }
        };
    }
}

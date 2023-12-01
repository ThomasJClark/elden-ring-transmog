using SoulsFormats;

string gamePath = @"C:\Program Files (x86)\Steam\steamapps\common\ELDEN RING\Game";
string inputPath = Path.GetFullPath(args.Length > 0 ? args[0] : gamePath);
string modPath = Path.GetFullPath(args.Length > 1 ? args[1] : "mods/ertransmog");

bool isInputVanilla = gamePath == inputPath;
bool isInputConvergence = inputPath.ToUpper().Contains("CONVERGENCE");
bool isInputReforged = inputPath.ToUpper().Contains("ERR");
bool isInputDetailedItemDescriptions = inputPath.ToUpper().Contains("DETAILED");

string armorFileName = @"N:\GR\data\Param\param\GameParam\EquipParamProtector.param";
string materialSetFileName = @"N:\GR\data\Param\param\GameParam\EquipMtrlSetParam.param";
string shopLineupFileName = @"N:\GR\data\Param\param\GameParam\ShopLineupParam.param";
string spEffectFileName = @"N:\GR\data\Param\param\GameParam\SpEffectParam.param";
string armorNameFileName = @"N:\GR\data\INTERROOT_win64\msg\engUS\ProtectorName.fmg";
string armorInfoFileName = @"N:\GR\data\INTERROOT_win64\msg\engUS\ProtectorInfo.fmg";
string armorCaptionFileName = @"N:\GR\data\INTERROOT_win64\msg\engUS\ProtectorCaption.fmg";
string talkTextFileName = @"N:\GR\data\INTERROOT_win64\msg\engUS\EventTextForTalk.fmg";
string commonTalkFileName = @"N:\GR\data\INTERROOT_win64\script\talk\m00_00_00_00\t000001000.esd";
string roundtableTalkFileName =
    @"N:\GR\data\INTERROOT_win64\script\talk\m11_10_00_00\t608001110.esd";

int itemTypeArmor = 1;

var GetBinderFile = (BND4 bnd, string fileName) => bnd.Files.Find(f => f.Name == fileName)!;

Console.WriteLine($"Reading vanilla data from \"{inputPath}\"...");

var paramDefs = Directory
    .GetFiles(Path.Combine("Paramdex", "ER", "Defs"))
    .Select(PARAMDEF.XmlDeserialize);

// Read the input data from the unpacked game folder
Console.WriteLine($"  Reading item names...");
BND4 itemMsgBnd = BND4.Read(Path.Combine(inputPath, "msg", "engus", "item.msgbnd.dcx"));
FMG armorNames = FMG.Read(GetBinderFile(itemMsgBnd, armorNameFileName).Bytes);
FMG armorInfos = FMG.Read(GetBinderFile(itemMsgBnd, armorInfoFileName).Bytes);
FMG armorCaptions = FMG.Read(GetBinderFile(itemMsgBnd, armorCaptionFileName).Bytes);

BND4 vanillaItemMsgBnd = isInputVanilla
    ? itemMsgBnd
    : BND4.Read(Path.Combine(gamePath, "msg", "engus", "item.msgbnd.dcx"));
FMG vanillaArmorNames = isInputVanilla
    ? armorNames
    : FMG.Read(GetBinderFile(vanillaItemMsgBnd, armorNameFileName).Bytes);

Console.WriteLine($"  Reading menu text...");
BND4 menuMsgBnd = BND4.Read(Path.Combine(inputPath, "msg", "engus", "menu.msgbnd.dcx"));
FMG talkTexts = FMG.Read(GetBinderFile(menuMsgBnd, talkTextFileName).Bytes);

Console.WriteLine($"  Reading params...");
BND4 paramBnd = SFUtil.DecryptERRegulation(Path.Combine(inputPath, "regulation.bin"));
PARAM armor = PARAM.Read(GetBinderFile(paramBnd, armorFileName).Bytes);
PARAM materialSets = PARAM.Read(GetBinderFile(paramBnd, materialSetFileName).Bytes);
PARAM shopLineups = PARAM.Read(GetBinderFile(paramBnd, shopLineupFileName).Bytes);
PARAM spEffects = PARAM.Read(GetBinderFile(paramBnd, spEffectFileName).Bytes);

BND4 vanillaParamBnd = isInputVanilla
    ? paramBnd
    : SFUtil.DecryptERRegulation(Path.Combine(gamePath, "regulation.bin"));
PARAM vanillaArmor = isInputVanilla
    ? armor
    : PARAM.Read(GetBinderFile(vanillaParamBnd, armorFileName).Bytes);

Console.WriteLine($"  Reading dialogue trees...");
BND4 commonTalkBnd = BND4.Read(
    Path.Combine(inputPath, "script", "talk", "m00_00_00_00.talkesdbnd.dcx")
);
BND4 roundtableTalkBnd = BND4.Read(
    Path.Combine(inputPath, "script", "talk", "m11_10_00_00.talkesdbnd.dcx")
);
ESD commonTalk = ESD.Read(GetBinderFile(commonTalkBnd, commonTalkFileName).Bytes);
ESD roundtableTalk = ESD.Read(GetBinderFile(roundtableTalkBnd, roundtableTalkFileName).Bytes);

vanillaArmor.ApplyParamdefCarefully(paramDefs);
armor.ApplyParamdefCarefully(paramDefs);
armor.Rows = new List<PARAM.Row>(armor.Rows);

int nextMaterialSetId = 6900000;
materialSets.ApplyParamdefCarefully(paramDefs);
materialSets.Rows = new List<PARAM.Row>(materialSets.Rows);

int nextShopLineupId = 4000000;
shopLineups.ApplyParamdefCarefully(paramDefs);
shopLineups.Rows = new List<PARAM.Row>(shopLineups.Rows);

spEffects.ApplyParamdefCarefully(paramDefs);
spEffects.Rows = new List<PARAM.Row>(spEffects.Rows);

Console.WriteLine("  Reading events...");
var commonEmevdPath = Path.Combine(inputPath, "event/common.emevd.dcx");
var commonEmevd = EMEVD.Read(
    DCX.Decompress(File.ReadAllBytes(commonEmevdPath), out DCX.Type commonEvevdDcxType)
);
var transmogFuncEmevd = EMEVD.Read(
    DCX.Decompress(File.ReadAllBytes("event/transmog_func.emevd.dcx"))
);

Console.WriteLine("Summary (before):");
Console.WriteLine($"  Armor: {armor.Rows.Count}");
Console.WriteLine($"  Material sets: {materialSets.Rows.Count}");
Console.WriteLine($"  Shop lineups: {shopLineups.Rows.Count}");

// For more efficient cell access
int materialSetItemTypeIdx = materialSets.GetCellIndex("materialCate01");
int materialSetItemIdIdx = materialSets.GetCellIndex("materialId01");
int materialSetItemNumIdx = materialSets.GetCellIndex("itemNum01");
int shopLineupItemTypeIdx = shopLineups.GetCellIndex("equipType");
int shopLineupItemIdIdx = shopLineups.GetCellIndex("equipId");
int shopLineupMaterialIdx = shopLineups.GetCellIndex("mtrlId");
int armorProtectorCategoryIdx = armor.GetCellIndex("protectorCategory");

// Bare head/body/arms/legs
int[] bareArmorIds = new int[] { 10000, 10100, 10200, 10300 };

// Cut armor to generate IDs for, but not include in the mod. These were included in previous
// versions of the mod, so their IDs need to be skipped so save files remain compatible.
int[] skippedArmorIds = new int[] { 610000, 610100, 610200, 610300 };

/**
 * Add a new armor piece with the stats of the first armor and the appearance of the second
 */
PARAM.Row AddArmor(int id, PARAM.Row baseArmorRow, PARAM.Row targetArmorRow)
{
    var armorRow = new PARAM.Row(baseArmorRow) { ID = id };

    // Is the target armor bare head/body/etc.? If so, instead of a transmogrified armor this is
    // an invisible version of the base armor
    var isInvisible = bareArmorIds.Contains(targetArmorRow.ID);

    for (var i = 0; i < armorRow.Cells.Count; i++)
    {
        if (
            armorRow.Cells[i].Def.InternalName == "equipModelId"
            || armorRow.Cells[i].Def.InternalName == "useFaceScale"
            || armorRow.Cells[i].Def.InternalName == "equipModelGender"
            || armorRow.Cells[i].Def.InternalName.StartsWith("invisibleFlag")
            || (
                !isInvisible
                && (
                    armorRow.Cells[i].Def.InternalName == "iconIdM"
                    || armorRow.Cells[i].Def.InternalName == "iconIdF"
                )
            )
        )
        {
            armorRow.Cells[i].Value = targetArmorRow.Cells[i].Value;
        }
    }

    if (isInvisible)
    {
        armorNames[armorRow.ID] = $"Invisible {armorNames[baseArmorRow.ID]}";
        armorInfos[armorRow.ID] = armorInfos[baseArmorRow.ID];
        armorCaptions[armorRow.ID] = armorCaptions[baseArmorRow.ID];
    }
    else
    {
        armorNames[armorRow.ID] =
            $"{armorNames[targetArmorRow.ID]} (Transmogrified {armorNames[baseArmorRow.ID]})";
        armorInfos[armorRow.ID] = armorInfos[targetArmorRow.ID];

        // Armor pieces in The Convergence and Elden Ring Reforged have effects in the description.
        // Copy that over to the new armor, so the description is still accurate.
        if (isInputConvergence || isInputReforged)
        {
            Predicate<string> isEffect = (line) =>
                line.StartsWith("Increases ")
                || line.StartsWith("Decreases ")
                || line.StartsWith("Reduces ")
                || line.StartsWith("Strengthens ")
                || line.StartsWith("Raises ");

            var baseDescription = armorCaptions[baseArmorRow.ID].Split("\n");
            var targetDescription = armorCaptions[targetArmorRow.ID].Split("\n");

            armorCaptions[armorRow.ID] =
                String.Join("\n", targetDescription.TakeWhile(line => !isEffect(line))).Trim()
                + "\n\n"
                + String
                    .Join(
                        "\n",
                        baseDescription
                            .SkipWhile(line => !isEffect(line))
                            .TakeWhile(line => isEffect(line))
                    )
                    .Trim()
                + "\n\n"
                + String
                    .Join(
                        "\n",
                        targetDescription
                            .SkipWhile(line => !isEffect(line))
                            .SkipWhile(line => isEffect(line))
                    )
                    .Trim();
        }
        // Similarly, Detailed Item Descriptions adds explicit passive effects to armor descriptions
        else if (isInputDetailedItemDescriptions)
        {
            var baseDescription = armorCaptions[baseArmorRow.ID].Split("\n");
            var targetDescription = armorCaptions[targetArmorRow.ID].Split("\n");

            armorCaptions[armorRow.ID] =
                String
                    .Join("\n", targetDescription.TakeWhile(line => !line.StartsWith("<font")))
                    .TrimEnd()
                + "\n\n"
                + String.Join("\n", baseDescription.SkipWhile(line => !line.StartsWith("<font")));
        }
        else
        {
            armorCaptions[armorRow.ID] = armorCaptions[targetArmorRow.ID];
        }
    }

    armor.Rows.Add(armorRow);
    return armorRow;
}

/**
 * Add a new material set to EquipMtrlSetParam with the given item
 */
PARAM.Row AddMaterialSet(int itemType, int itemId)
{
    var materialSetRow = new PARAM.Row(nextMaterialSetId++, null, materialSets.AppliedParamdef);
    materialSetRow[materialSetItemTypeIdx].Value = itemType;
    materialSetRow[materialSetItemIdIdx].Value = itemId;
    materialSetRow[materialSetItemNumIdx].Value = 1;
    materialSets.Rows.Add(materialSetRow);
    return materialSetRow;
}

/**
 * Add a new shop item for the given item and the given material set
 */
PARAM.Row AddShopLineup(int itemType, int itemId, int materialSet)
{
    var shopLineupRow = new PARAM.Row(nextShopLineupId++, null, shopLineups.AppliedParamdef);
    shopLineupRow[shopLineupItemTypeIdx].Value = itemType;
    shopLineupRow[shopLineupItemIdIdx].Value = itemId;
    shopLineupRow[shopLineupMaterialIdx].Value = materialSet;
    shopLineups.Rows.Add(shopLineupRow);
    return shopLineupRow;
}

Console.WriteLine("Adding event handlers...");

// Add a speffect that triggers an event to undo all equipped transmogs
var undoTransmogEffect = new PARAM.Row(250, "", spEffects.AppliedParamdef);
undoTransmogEffect["effectTargetSelf"].Value = (byte)1;
undoTransmogEffect["effectTargetFriend"].Value = (byte)1;
undoTransmogEffect["effectTargetPlayer"].Value = (byte)1;
undoTransmogEffect["effectTargetSelfTarget"].Value = (byte)1;
spEffects.Rows.Add(undoTransmogEffect);

var nextEventId = 9007101;

// Create an event to undo equipped transmogs when the above effect is applied
EMEVD.Event undoTransmogEvent = new(nextEventId++);
commonEmevd.Events.Add(undoTransmogEvent);
undoTransmogEvent.Instructions.Add(
    TransmogEMEVDUtils.IfCharacterHasSpEffect(
        TransmogEMEVDUtils.MAIN,
        TransmogEMEVDUtils.ENTITY_PLAYER,
        undoTransmogEffect.ID,
        true,
        TransmogEMEVDUtils.EQUAL,
        1.0f
    )
);

// Create an event that runs after an armor piece is successfully untransmogged, to spawn SFX
EMEVD.Event postUndoTransmogEvent = transmogFuncEmevd.Events.Find(evt => evt.ID == 0)!;
postUndoTransmogEvent.ID = nextEventId++;
commonEmevd.Events.Add(postUndoTransmogEvent);

EMEVD.Event initializeEvent = commonEmevd.Events.Find(evt => evt.ID == 0)!;
initializeEvent.Instructions.Add(TransmogEMEVDUtils.InitializeEvent(0, (int)undoTransmogEvent.ID));

Console.WriteLine("Generating transmogrified armor...");

// Use the vanilla data to determine valid armor. This keeps save files compatible, and keeps
// the number of rows in line with the 16 bit limit because custom armor sets aren't included.
PARAM.Row[] validArmorRows = vanillaArmor.Rows
    .Where(
        row =>
            row.ID >= 10000
            && vanillaArmorNames[row.ID] != null
            // Exclude unobtainable armor
            && !vanillaArmorNames[row.ID].StartsWith("[ERROR]")
            // Don't allow transmogs to or from altered pieces, to avoid hitting the 16 bit row
            // limit
            && !vanillaArmorNames[row.ID].EndsWith(" (Altered)")
    )
    .ToArray();

Dictionary<int, PARAM.Row> armorById = armor.Rows.ToDictionary(row => row.ID);

// Base armor determines the stats of the transmogrified armor
int i = 100;
foreach (var baseArmorRow in validArmorRows)
{
    var baseProtectorCategory = (byte)baseArmorRow[armorProtectorCategoryIdx].Value;

    // Empty slots aren't obtainable as items, so they can't be the base armor
    if (bareArmorIds.Contains(baseArmorRow.ID))
    {
        continue;
    }

    if (skippedArmorIds.Contains(baseArmorRow.ID))
    {
        // Still increment the ID counter, so IDs are the same as previous versions of the mod
        // that didn't skip these armor pieces
        i++;
        continue;
    }

    var baseMaterialSet = AddMaterialSet(itemTypeArmor, baseArmorRow.ID);

    // Target armor determines the appearance of the transmogrified armor
    int j = 0;
    foreach (var targetArmorRow in validArmorRows)
    {
        var targetProtectorCategory = (byte)targetArmorRow[armorProtectorCategoryIdx].Value;

        // Allow transmogs to different armor pieces of the same category
        // (e.g. helmet => different helmet)
        if (targetArmorRow == baseArmorRow || baseProtectorCategory != targetProtectorCategory)
        {
            continue;
        }

        j++;

        if (
            !armorById.ContainsKey(baseArmorRow.ID)
            || !armorById.ContainsKey(targetArmorRow.ID)
            || skippedArmorIds.Contains(targetArmorRow.ID)
        )
        {
            continue;
        }

        // Generate a (relatively) stable ID. Assuming future patches/mods only add armor
        // pieces at the end of the current list, this should make save files forward compatible
        // with later updates.
        int transmogrifiedArmorID = 100000 * i + 100 * j;

        // Copy the armor piece, changing the name and decription
        var transmogrifiedArmorRow = AddArmor(
            transmogrifiedArmorID,
            armorById[baseArmorRow.ID],
            armorById[targetArmorRow.ID]
        );

        // Add a shop item to create the transmogrified armor
        AddShopLineup(itemTypeArmor, transmogrifiedArmorRow.ID, baseMaterialSet.ID);

        // Add event instructions to undo the transmogrified armor when an spEffect is applied
        // to the player
        undoTransmogEvent.Instructions.AddRange(
            new List<EMEVD.Instruction>()
            {
                TransmogEMEVDUtils.IfPlayerHasArmorEquipped(
                    TransmogEMEVDUtils.OR_01,
                    baseProtectorCategory,
                    transmogrifiedArmorRow.ID
                ),
                TransmogEMEVDUtils.SkipIfConditionGroupStateUncompiled(
                    3,
                    TransmogEMEVDUtils.CONDITION_STATE_FAIL,
                    TransmogEMEVDUtils.OR_01
                ),
                TransmogEMEVDUtils.RemoveItemFromPlayer(
                    TransmogEMEVDUtils.ITEM_TYPE_ARMOR,
                    transmogrifiedArmorRow.ID,
                    1
                ),
                TransmogEMEVDUtils.DirectlyGivePlayerItem(
                    TransmogEMEVDUtils.ITEM_TYPE_ARMOR,
                    baseArmorRow.ID,
                    6001,
                    1
                ),
                TransmogEMEVDUtils.InitializeEvent(
                    transmogrifiedArmorRow.ID,
                    (int)postUndoTransmogEvent.ID,
                    baseProtectorCategory
                ),
                TransmogEMEVDUtils.WaitFixedTimeFrames(0)
            }
        );
    }

    i++;
}

undoTransmogEvent.Instructions.Add(
    TransmogEMEVDUtils.EndUnconditionally(TransmogEMEVDUtils.EXECUTION_END_TYPE_RESTART)
);

// Add the menu text used to open the transmogrify screen
talkTexts[69000000] = isInputReforged
    ? "<img src='img://SB_ERR_Grace_AlterGarments.png' height='32' width='32' vspace='-16'/> Transmogrify armor"
    : "Transmogrify armor";

talkTexts[69000001] = isInputReforged
    ? "<img src='img://SB_ERR_Cosmetics.png' height='32' width='32' vspace='-16'/> Untransmogrify equipped armor"
    : "Untransmogrify equipped armor";

Console.WriteLine("Summary (after):");
Console.WriteLine($"  Armor: {armor.Rows.Count}");
Console.WriteLine($"  Material sets: {materialSets.Rows.Count}");
Console.WriteLine($"  Shop lineups: {shopLineups.Rows.Count}");

Console.WriteLine($"Writing mod data to \"{modPath}\"...");

// Write the modified data to the mod folder
Console.WriteLine($"  Writing item names...");
GetBinderFile(itemMsgBnd, armorNameFileName).Bytes = armorNames.Write();
GetBinderFile(itemMsgBnd, armorInfoFileName).Bytes = armorInfos.Write();
GetBinderFile(itemMsgBnd, armorCaptionFileName).Bytes = armorCaptions.Write();
itemMsgBnd.Write(Path.Combine(modPath, "msg", "engus", "item.msgbnd.dcx"));

Console.WriteLine($"  Writing menu text...");
GetBinderFile(menuMsgBnd, talkTextFileName).Bytes = talkTexts.Write();
menuMsgBnd.Write(Path.Combine(modPath, "msg", "engus", "menu.msgbnd.dcx"));

Console.WriteLine($"  Writing params...");
GetBinderFile(paramBnd, armorFileName).Bytes = armor.Write();
GetBinderFile(paramBnd, materialSetFileName).Bytes = materialSets.Write();
GetBinderFile(paramBnd, shopLineupFileName).Bytes = shopLineups.Write();
GetBinderFile(paramBnd, spEffectFileName).Bytes = spEffects.Write();
SFUtil.EncryptERRegulation(Path.Combine(modPath, "regulation.bin"), paramBnd);

Console.WriteLine("  Writing events...");
Directory.CreateDirectory(Path.Combine(modPath, "event"));
File.WriteAllBytes(
    Path.Combine(modPath, "event/common.emevd.dcx"),
    DCX.Compress(commonEmevd.Write(), commonEvevdDcxType)
);

Console.WriteLine("  Writing dialogue trees...");
GetBinderFile(commonTalkBnd, commonTalkFileName).Bytes = commonTalk.Write();
GetBinderFile(roundtableTalkBnd, roundtableTalkFileName).Bytes = roundtableTalk.Write();
commonTalkBnd.Write(Path.Combine(modPath, "script", "talk", "m00_00_00_00.talkesdbnd.dcx"));
roundtableTalkBnd.Write(Path.Combine(modPath, "script", "talk", "m11_10_00_00.talkesdbnd.dcx"));

Console.WriteLine("Done");

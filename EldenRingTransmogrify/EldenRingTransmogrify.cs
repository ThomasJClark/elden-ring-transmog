using SoulsFormats;

string gamePath = @"C:\Program Files (x86)\Steam\steamapps\common\ELDEN RING\Game";
string inputPath = Path.GetFullPath(args.Length > 0 ? args[0] : gamePath);
string modPath = Path.GetFullPath(args.Length > 1 ? args[1] : "mods/ertransmog");

bool isInputVanilla = gamePath == inputPath;
bool isInputConvergence = inputPath.ToUpper().Contains("CONVERGENCE");
bool isInputReforged = inputPath.ToUpper().Contains("ERR");
bool isInputDetailedItemDescriptions = inputPath.ToUpper().Contains("DETAILED");

string armorFileName = @"N:\GR\data\Param\param\GameParam\EquipParamProtector.param";
string itemsFileName = @"N:\GR\data\Param\param\GameParam\EquipParamGoods.param";
string materialSetFileName = @"N:\GR\data\Param\param\GameParam\EquipMtrlSetParam.param";
string shopLineupFileName = @"N:\GR\data\Param\param\GameParam\ShopLineupParam.param";
string spEffectFileName = @"N:\GR\data\Param\param\GameParam\SpEffectParam.param";
string spEffectVfxFileName = @"N:\GR\data\Param\param\GameParam\SpEffectVfxParam.param";
string armorNameFileName = @"N:\GR\data\INTERROOT_win64\msg\engUS\ProtectorName.fmg";
string armorInfoFileName = @"N:\GR\data\INTERROOT_win64\msg\engUS\ProtectorInfo.fmg";
string armorCaptionFileName = @"N:\GR\data\INTERROOT_win64\msg\engUS\ProtectorCaption.fmg";
string itemNameFileName = @"N:\GR\data\INTERROOT_win64\msg\engUS\GoodsName.fmg";
string itemInfoFileName = @"N:\GR\data\INTERROOT_win64\msg\engUS\GoodsInfo.fmg";
string itemCaptionFileName = @"N:\GR\data\INTERROOT_win64\msg\engUS\GoodsCaption.fmg";
string talkTextFileName = @"N:\GR\data\INTERROOT_win64\msg\engUS\EventTextForTalk.fmg";
string mapTextFileName = @"N:\GR\data\INTERROOT_win64\msg\engUS\EventTextForMap.fmg";
string menuTextFileName = @"N:\GR\data\INTERROOT_win64\msg\engUS\GR_MenuText.fmg";

// string commonTalkFileName = @"N:\GR\data\INTERROOT_win64\script\talk\m00_00_00_00\t000001000.esd";
// string roundtableTalkFileName =
//     @"N:\GR\data\INTERROOT_win64\script\talk\m11_10_00_00\t608001110.esd";

int itemTypeArmor = 1;
int itemTypeGood = 3;
int invisibleIconId = 3142;

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
FMG itemNames = FMG.Read(GetBinderFile(itemMsgBnd, itemNameFileName).Bytes);
FMG itemInfos = FMG.Read(GetBinderFile(itemMsgBnd, itemInfoFileName).Bytes);
FMG itemCaptions = FMG.Read(GetBinderFile(itemMsgBnd, itemCaptionFileName).Bytes);

BND4 vanillaItemMsgBnd = isInputVanilla
    ? itemMsgBnd
    : BND4.Read(Path.Combine(gamePath, "msg", "engus", "item.msgbnd.dcx"));
FMG vanillaArmorNames = isInputVanilla
    ? armorNames
    : FMG.Read(GetBinderFile(vanillaItemMsgBnd, armorNameFileName).Bytes);

Console.WriteLine($"  Reading menu text...");
BND4 menuMsgBnd = BND4.Read(Path.Combine(inputPath, "msg", "engus", "menu.msgbnd.dcx"));
FMG talkTexts = FMG.Read(GetBinderFile(menuMsgBnd, talkTextFileName).Bytes);
FMG mapTexts = FMG.Read(GetBinderFile(menuMsgBnd, mapTextFileName).Bytes);
FMG menuTexts = FMG.Read(GetBinderFile(menuMsgBnd, menuTextFileName).Bytes);

Console.WriteLine($"  Reading params...");
BND4 paramBnd = SFUtil.DecryptERRegulation(Path.Combine(inputPath, "regulation.bin"));
PARAM armor = PARAM.Read(GetBinderFile(paramBnd, armorFileName).Bytes);
PARAM items = PARAM.Read(GetBinderFile(paramBnd, itemsFileName).Bytes);
PARAM materialSets = PARAM.Read(GetBinderFile(paramBnd, materialSetFileName).Bytes);
PARAM shopLineups = PARAM.Read(GetBinderFile(paramBnd, shopLineupFileName).Bytes);
PARAM spEffects = PARAM.Read(GetBinderFile(paramBnd, spEffectFileName).Bytes);
PARAM spEffectVfx = PARAM.Read(GetBinderFile(paramBnd, spEffectVfxFileName).Bytes);

List<PARAM> allParams = new() { armor, items, materialSets, shopLineups, spEffects, spEffectVfx, };
foreach (var param in allParams)
{
    param.ApplyParamdefCarefully(paramDefs);
    param.Rows = new List<PARAM.Row>(param.Rows);
}

BND4 vanillaParamBnd = isInputVanilla
    ? paramBnd
    : SFUtil.DecryptERRegulation(Path.Combine(gamePath, "regulation.bin"));
PARAM vanillaArmor = isInputVanilla
    ? armor
    : PARAM.Read(GetBinderFile(vanillaParamBnd, armorFileName).Bytes);
vanillaArmor.ApplyParamdefCarefully(paramDefs);

// Console.WriteLine($"  Reading dialogue trees...");
// BND4 commonTalkBnd = BND4.Read(
//     Path.Combine(inputPath, "script", "talk", "m00_00_00_00.talkesdbnd.dcx")
// );
// BND4 roundtableTalkBnd = BND4.Read(
//     Path.Combine(inputPath, "script", "talk", "m11_10_00_00.talkesdbnd.dcx")
// );
// ESD commonTalk = ESD.Read(GetBinderFile(commonTalkBnd, commonTalkFileName).Bytes);
// ESD roundtableTalk = ESD.Read(GetBinderFile(roundtableTalkBnd, roundtableTalkFileName).Bytes);

int nextMaterialSetId = 6900000;
int startTransmogHeadShopLineupId = 4000000;
int nextTransmogHeadShopLineupId = startTransmogHeadShopLineupId;
int startTransmogBodyShopLineupId = 4100000;
int nextTransmogBodyShopLineupId = startTransmogBodyShopLineupId;
int startTransmogArmsShopLineupId = 4200000;
int nextTransmogArmsShopLineupId = startTransmogArmsShopLineupId;
int startTransmogLegsShopLineupId = 4300000;
int nextTransmogLegsShopLineupId = startTransmogLegsShopLineupId;
int nextSpEffectId = 690000;
int nextSpEffectVfxId = 690000;

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
int shopLineupNameIdx = shopLineups.GetCellIndex("nameMsgId");
int shopLineupIconIdx = shopLineups.GetCellIndex("iconId");
int shopLineupMaterialIdx = shopLineups.GetCellIndex("mtrlId");
int shopLineupMenuTitleIdx = shopLineups.GetCellIndex("menuTitleMsgId");
int shopLineupMenuIconIdx = shopLineups.GetCellIndex("menuIconId");
int armorProtectorCategoryIdx = armor.GetCellIndex("protectorCategory");
int armorIconIdIdx = armor.GetCellIndex("iconIdM");
int itemGoodsTypeIdx = items.GetCellIndex("goodsType");
int itemSaleValueIdx = items.GetCellIndex("saleValue");
int itemMaxNumIdx = items.GetCellIndex("maxNum");

// Bare head/body/arms/legs
int[] bareArmorIds = new int[] { 10000, 10100, 10200, 10300 };

// Cut armor to generate IDs for, but not include in the mod. These were included in previous
// versions of the mod, so their IDs need to be skipped so save files remain compatible.
int[] skippedArmorIds = new int[] { 610000, 610100, 610200, 610300 };

// Add the menu text used to open the transmogrify screen
talkTexts[69000000] = isInputReforged
    ? "<img src='img://SB_ERR_Grace_AlterGarments.png' height='32' width='32' vspace='-16'/> Transmogrify armor"
    : "Transmogrify armor";
talkTexts[69000010] = "Transmogrify Head";
talkTexts[69000011] = "Transmogrify Body";
talkTexts[69000012] = "Transmogrify Arms";
talkTexts[69000013] = "Transmogrify Legs";
talkTexts[69000020] = "Untransmogrify equipped armor";

int transmogHeadMenuTextId = 690000;
int transmogBodyMenuTextId = 690001;
int transmogArmsMenuTextId = 690002;
int transmogLegsMenuTextId = 690003;
menuTexts[transmogHeadMenuTextId] = "Transmogrify Head";
menuTexts[transmogBodyMenuTextId] = "Transmogrify Body";
menuTexts[transmogArmsMenuTextId] = "Transmogrify Arms";
menuTexts[transmogLegsMenuTextId] = "Transmogrify Legs";

mapTexts[690000] = "Untransmogrified armor has been returned to your inventory";

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
        armorRow[armorIconIdIdx].Value = invisibleIconId;
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
PARAM.Row AddTransmogShopLineup(
    int id,
    int targetArmorId,
    int transmogrifiedArmorId,
    int materialSet
)
{
    var shopLineupRow = new PARAM.Row(id, null, shopLineups.AppliedParamdef);
    shopLineupRow[shopLineupItemTypeIdx].Value = itemTypeArmor;
    shopLineupRow[shopLineupItemIdIdx].Value = transmogrifiedArmorId;
    shopLineupRow[shopLineupMaterialIdx].Value = materialSet;

    if (shopLineupRow.ID == startTransmogBodyShopLineupId)
    {
        shopLineupRow[shopLineupMenuTitleIdx].Value = transmogBodyMenuTextId;
        shopLineupRow[shopLineupMenuIconIdx].Value = 5;
    }
    else if (shopLineupRow.ID == startTransmogArmsShopLineupId)
    {
        shopLineupRow[shopLineupMenuTitleIdx].Value = transmogArmsMenuTextId;
        shopLineupRow[shopLineupMenuIconIdx].Value = 5;
    }
    else if (shopLineupRow.ID == startTransmogLegsShopLineupId)
    {
        shopLineupRow[shopLineupMenuTitleIdx].Value = transmogLegsMenuTextId;
        shopLineupRow[shopLineupMenuIconIdx].Value = 5;
    }

    if (!bareArmorIds.Contains(targetArmorId))
    {
        shopLineupRow[shopLineupNameIdx].Value = targetArmorId;
    }

    shopLineups.Rows.Add(shopLineupRow);
    return shopLineupRow;
}

/**
 * Add a new shop entry for pseudo-transmog, to "buy" a placeholder item for an armor piece
 */
PARAM.Row AddPseudoTransmogShopLineup(int itemId, PARAM.Row armorRow)
{
    var shopLineupRow = new PARAM.Row(
        nextTransmogHeadShopLineupId++,
        null,
        shopLineups.AppliedParamdef
    );
    shopLineupRow[shopLineupItemTypeIdx].Value = itemTypeGood;
    shopLineupRow[shopLineupItemIdIdx].Value = itemId;
    if (bareArmorIds.Contains(armorRow.ID))
    {
        shopLineupRow[shopLineupIconIdx].Value = invisibleIconId;
    }
    else
    {
        shopLineupRow[shopLineupIconIdx].Value = armorRow[armorIconIdIdx].Value;
    }
    if (shopLineupRow.ID == startTransmogHeadShopLineupId)
    {
        shopLineupRow[shopLineupMenuTitleIdx].Value = transmogHeadMenuTextId;
        shopLineupRow[shopLineupMenuIconIdx].Value = 5;
    }
    shopLineups.Rows.Add(shopLineupRow);
    return shopLineupRow;
}

PARAM.Row AddPseudoTransmogEffect(PARAM.Row armorRow)
{
    var transmogVfx = new PARAM.Row(
        nextSpEffectVfxId++,
        $"[Transmog] {armorNames[armorRow.ID]}",
        spEffectVfx.AppliedParamdef
    );
    transmogVfx["transformProtectorId"].Value = armorRow.ID;
    spEffectVfx.Rows.Add(transmogVfx);

    var transmogEffect = new PARAM.Row(
        nextSpEffectId++,
        $"[Transmog] {armorNames[armorRow.ID]}",
        spEffects.AppliedParamdef
    );
    transmogEffect["vfxId"].Value = transmogVfx.ID;
    transmogEffect["effectEndurance"].Value = -1.0f;
    transmogEffect["effectTargetSelf"].Value = (byte)1;
    transmogEffect["effectTargetFriend"].Value = (byte)1;
    transmogEffect["effectTargetEnemy"].Value = (byte)1;
    transmogEffect["effectTargetPlayer"].Value = (byte)1;
    transmogEffect["effectTargetOpposeTarget"].Value = (byte)1;
    transmogEffect["effectTargetFriendlyTarget"].Value = (byte)1;
    transmogEffect["effectTargetSelfTarget"].Value = (byte)1;

    foreach (var cell in transmogEffect.Cells)
    {
        if (cell.Def.InternalName.StartsWith("vowType"))
            cell.Value = (byte)1;
    }

    spEffects.Rows.Add(transmogEffect);

    return transmogEffect;
}

PARAM.Row AddPseudoTransmogItem(int itemId, PARAM.Row armorRow)
{
    var transmogItem = new PARAM.Row(
        itemId,
        $"[Transmog] {armorNames[armorRow.ID]}",
        items.AppliedParamdef
    );

    transmogItem[itemGoodsTypeIdx].Value = 1; // key item
    transmogItem[itemSaleValueIdx].Value = 0;
    transmogItem[itemMaxNumIdx].Value = 1;
    items.Rows.Add(transmogItem);

    if (bareArmorIds.Contains(armorRow.ID))
    {
        itemNames[transmogItem.ID] = "Invisible";
        itemInfos[transmogItem.ID] = "";
        itemCaptions[transmogItem.ID] = "";
    }
    else
    {
        itemNames[transmogItem.ID] = armorNames[armorRow.ID];
        itemInfos[transmogItem.ID] = armorInfos[armorRow.ID];
        itemCaptions[transmogItem.ID] = armorCaptions[armorRow.ID];
    }

    return transmogItem;
}

Console.WriteLine("Adding event handlers...");

// Add a speffect that triggers an event to undo all equipped transmogs
var undoTransmogEffect = new PARAM.Row(250, "Undo transmog", spEffects.AppliedParamdef);
undoTransmogEffect["effectTargetSelf"].Value = (byte)1;
undoTransmogEffect["effectTargetFriend"].Value = (byte)1;
undoTransmogEffect["effectTargetPlayer"].Value = (byte)1;
undoTransmogEffect["effectTargetSelfTarget"].Value = (byte)1;
spEffects.Rows.Add(undoTransmogEffect);

EMEVD.Event initializeEvent = commonEmevd.Events.Find(evt => evt.ID == 0)!;
EMEVD.Event undoTransmogEvent = new(9007101);
EMEVD.Event postUndoTransmogEvent = transmogFuncEmevd.Events.Find(evt => evt.ID == 9007102)!;
EMEVD.Event buyPseudoTransmogEvent = transmogFuncEmevd.Events.Find(evt => evt.ID == 9007103)!;
EMEVD.Event applyPseudoTransmogEvent = transmogFuncEmevd.Events.Find(evt => evt.ID == 9007104)!;

initializeEvent.Instructions.Add(
    TransmogEMEVDUtils.InitializeEvent(0, (int)undoTransmogEvent.ID, 0)
);

commonEmevd.Events.Add(undoTransmogEvent);
commonEmevd.Events.Add(postUndoTransmogEvent);
commonEmevd.Events.Add(buyPseudoTransmogEvent);
commonEmevd.Events.Add(applyPseudoTransmogEvent);

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

Console.WriteLine("Generating transmogrified armor...");

List<PARAM.Row> validArmorRows = new();
foreach (var row in vanillaArmor.Rows)
{
    if (
        row.ID >= 10000
        && vanillaArmorNames[row.ID] != null
        && !vanillaArmorNames[row.ID].StartsWith("[ERROR]")
        && !vanillaArmorNames[row.ID].EndsWith(" (Altered)")
    )
    {
        validArmorRows.Add(armor[row.ID]);
    }
}

// Generate transmogs for modded armor sets (e.g. new sets added in The Convergence) after vanilla
// armor, to keep IDs of vanilla transmogs compatible with previous version of this mod
foreach (var row in armor.Rows)
{
    if (
        row.ID >= 10000
        && vanillaArmor[row.ID] == null
        && armorNames[row.ID] != null
        && !armorNames[row.ID].StartsWith("[ERROR]")
        && !armorNames[row.ID].EndsWith(" (Altered)")
        && armorInfos[row.ID] != null
        && armorCaptions[row.ID] != null
    )
    {
        validArmorRows.Add(row);
    }
}

Dictionary<int, PARAM.Row> armorById = armor.Rows.ToDictionary(row => row.ID);

Dictionary<int, int> pseudoTransmogEffectsByItemId = new();

/**
 * For helmets only, transmogrification is implemented by adding an effect to the player
 * that changes the visual appearance of their head. Generating actual transmogs for
 * all helmets would take up too many param rows, and the engine conveniently has support
 * for transmogging heads as a vestige of Dark Souls dragon form.
 */
void AddPseudoTransmogs()
{
    // Put vanilla and modded armor params in two different ID ranges, so future vanilla armor can
    // be added without conflicitng with mods.
    int armorIndex = 100;
    int moddedArmorIndex = 750;

    foreach (var armorRow in validArmorRows)
    {
        // If this armor isn't present, it's a vanilla piece that was removed in a mod. Increment
        // the ID for save file compatibility and skip it
        if (armorRow == null)
        {
            armorIndex++;
            continue;
        }

        // Only pseud-transmog helms
        var protectorCategory = (byte)armorRow[armorProtectorCategoryIdx].Value;
        var isPseudoTransmog = protectorCategory == 0;
        if (!isPseudoTransmog)
        {
            continue;
        }

        var isModded = vanillaArmor[armorRow.ID] == null;
        var index = isModded ? moddedArmorIndex++ : armorIndex++;

        // Add a speffect that makes the player character appear to be wearing the target armor
        var pseudoTransmogEffect = AddPseudoTransmogEffect(armorRow);

        // Add a placeholder item that the player can buy in a fake shop to activate the transmog
        var pseudoTransmogItem = AddPseudoTransmogItem(690000 + armorIndex, armorRow);
        AddPseudoTransmogShopLineup(pseudoTransmogItem.ID, armorRow);

        pseudoTransmogEffectsByItemId[pseudoTransmogItem.ID] = pseudoTransmogEffect.ID;

        // When the player aquires the item, remove it and set an event flag.
        var eventFlagId = 690000 + index;
        initializeEvent.Instructions.Add(
            TransmogEMEVDUtils.InitializeEvent(
                index,
                (int)buyPseudoTransmogEvent.ID,
                pseudoTransmogItem.ID,
                eventFlagId
            )
        );

        // When the above event is set, apply the speffect that transmogrifies the player's head.
        initializeEvent.Instructions.Add(
            TransmogEMEVDUtils.InitializeEvent(
                index,
                (int)applyPseudoTransmogEvent.ID,
                eventFlagId,
                pseudoTransmogEffect.ID
            )
        );

        // Remove the event flag in the undo transmog event
        undoTransmogEvent.Instructions.AddRange(
            new List<EMEVD.Instruction>()
            {
                TransmogEMEVDUtils.SkipIfEventFlag(
                    2,
                    TransmogEMEVDUtils.OFF,
                    TransmogEMEVDUtils.EVENT_FLAG,
                    eventFlagId
                ),
                TransmogEMEVDUtils.SetEventFlag(
                    TransmogEMEVDUtils.EVENT_FLAG,
                    eventFlagId,
                    TransmogEMEVDUtils.OFF
                ),
                TransmogEMEVDUtils.InitializeEvent(
                    armorIndex,
                    (int)postUndoTransmogEvent.ID,
                    protectorCategory
                ),
                TransmogEMEVDUtils.WaitFixedTimeFrames(0)
            }
        );
    }
}

/**
 * For most armor, add a new protector row for each valid combination of transmogs, and a
 * crafting recipe to turn the base armor into the transmogrified armor.
 */
void AddTransmogs()
{
    // Put vanilla and modded armor params in two different ID ranges, so future vanilla armor can
    // be added without conflicitng with mods.
    int baseArmorIndex = 100;
    int moddedBaseArmorIndex = 750;
    foreach (var baseArmorRow in validArmorRows)
    {
        // If this armor isn't present, it's a vanilla piece that was removed in a mod. Increment the
        // ID for save file compatibility and skip it
        if (baseArmorRow == null)
        {
            baseArmorIndex++;
            continue;
        }

        var baseProtectorCategory = (byte)baseArmorRow[armorProtectorCategoryIdx].Value;
        var pseudoTransmog = baseProtectorCategory == 0;

        if (skippedArmorIds.Contains(baseArmorRow.ID))
        {
            // Still increment the ID counter, so IDs are the same as previous versions of the mod
            // that didn't skip these armor pieces
            baseArmorIndex++;
            continue;
        }

        // Empty slots aren't obtainable as items, so they can't be the base armor
        if (bareArmorIds.Contains(baseArmorRow.ID))
        {
            continue;
        }

        var isBaseModded = vanillaArmor[baseArmorRow.ID] == null;
        var baseIndex = isBaseModded ? moddedBaseArmorIndex++ : baseArmorIndex++;

        var baseMaterialSet = AddMaterialSet(itemTypeArmor, baseArmorRow.ID);

        // Target armor determines the appearance of the transmogrified armor
        int targetArmorIndex = 100;
        int moddedTargetArmorIndex = 750;
        foreach (var targetArmorRow in validArmorRows)
        {
            // If this armor isn't present, it's a vanilla piece that was removed in a mod. Increment the
            // ID for save file compatibility and skip it
            if (targetArmorRow == null)
            {
                targetArmorIndex++;
                continue;
            }

            var targetProtectorCategory = (byte)targetArmorRow[armorProtectorCategoryIdx].Value;

            // Allow transmogs to different armor pieces of the same category
            // (e.g. helmet => different helmet)
            if (targetArmorRow == baseArmorRow || baseProtectorCategory != targetProtectorCategory)
            {
                continue;
            }

            var isTargetModded = vanillaArmor[targetArmorRow.ID] == null;
            var targetIndex = isTargetModded ? moddedTargetArmorIndex++ : targetArmorIndex++;

            if (
                !armorById.ContainsKey(baseArmorRow.ID)
                || !armorById.ContainsKey(targetArmorRow.ID)
                || skippedArmorIds.Contains(targetArmorRow.ID)
                // Generate invisible helms, even though they are transmogged through the speffect
                // system. Previous version fo the mod supported invisible helms, so we need to
                // generate the params for backwards compatibility.
                || (pseudoTransmog && !bareArmorIds.Contains(targetArmorRow.ID))
            )
            {
                continue;
            }

            var isTargetArmorModded = vanillaArmor[targetArmorRow.ID] == null;

            // Generate a (relatively) stable ID. Assuming future patches/mods only add armor
            // pieces at the end of the current list, this should make save files forward compatible
            // with later updates.
            int transmogrifiedArmorID = 100000 * baseIndex + 100 * targetIndex;

            // Copy the armor piece, changing the name and decription
            var transmogrifiedArmorRow = AddArmor(
                transmogrifiedArmorID,
                armorById[baseArmorRow.ID],
                armorById[targetArmorRow.ID]
            );

            if (!pseudoTransmog)
            {
                // Add a shop item to create the transmogrified armor
                int shopLineupId = -1;
                if (baseProtectorCategory == 1)
                {
                    shopLineupId = nextTransmogBodyShopLineupId++;
                }
                else if (baseProtectorCategory == 2)
                {
                    shopLineupId = nextTransmogArmsShopLineupId++;
                }
                else if (baseProtectorCategory == 3)
                {
                    shopLineupId = nextTransmogLegsShopLineupId++;
                }
                AddTransmogShopLineup(
                    shopLineupId,
                    targetArmorRow.ID,
                    transmogrifiedArmorRow.ID,
                    baseMaterialSet.ID
                );
            }

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
    }
}

AddPseudoTransmogs();
AddTransmogs();

undoTransmogEvent.Instructions.Add(
    TransmogEMEVDUtils.EndUnconditionally(TransmogEMEVDUtils.EXECUTION_END_TYPE_RESTART)
);

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
GetBinderFile(itemMsgBnd, itemNameFileName).Bytes = itemNames.Write();
GetBinderFile(itemMsgBnd, itemInfoFileName).Bytes = itemInfos.Write();
GetBinderFile(itemMsgBnd, itemCaptionFileName).Bytes = itemCaptions.Write();
itemMsgBnd.Write(Path.Combine(modPath, "msg", "engus", "item.msgbnd.dcx"));

Console.WriteLine($"  Writing menu text...");
GetBinderFile(menuMsgBnd, talkTextFileName).Bytes = talkTexts.Write();
GetBinderFile(menuMsgBnd, mapTextFileName).Bytes = mapTexts.Write();
GetBinderFile(menuMsgBnd, menuTextFileName).Bytes = menuTexts.Write();
menuMsgBnd.Write(Path.Combine(modPath, "msg", "engus", "menu.msgbnd.dcx"));

Console.WriteLine($"  Writing params...");
foreach (var param in allParams)
{
    param.Rows.Sort((a, b) => a.ID - b.ID);
}
GetBinderFile(paramBnd, armorFileName).Bytes = armor.Write();
GetBinderFile(paramBnd, itemsFileName).Bytes = items.Write();
GetBinderFile(paramBnd, materialSetFileName).Bytes = materialSets.Write();
GetBinderFile(paramBnd, shopLineupFileName).Bytes = shopLineups.Write();
GetBinderFile(paramBnd, spEffectFileName).Bytes = spEffects.Write();
GetBinderFile(paramBnd, spEffectVfxFileName).Bytes = spEffectVfx.Write();
SFUtil.EncryptERRegulation(Path.Combine(modPath, "regulation.bin"), paramBnd);

Console.WriteLine("  Writing events...");
Directory.CreateDirectory(Path.Combine(modPath, "event"));
File.WriteAllBytes(
    Path.Combine(modPath, "event/common.emevd.dcx"),
    DCX.Compress(commonEmevd.Write(), commonEvevdDcxType)
);

// Console.WriteLine("  Writing dialogue trees...");
// GetBinderFile(commonTalkBnd, commonTalkFileName).Bytes = commonTalk.Write();
// GetBinderFile(roundtableTalkBnd, roundtableTalkFileName).Bytes = roundtableTalk.Write();
// commonTalkBnd.Write(Path.Combine(modPath, "script", "talk", "m00_00_00_00.talkesdbnd.dcx"));
// roundtableTalkBnd.Write(Path.Combine(modPath, "script", "talk", "m11_10_00_00.talkesdbnd.dcx"));

Console.WriteLine("Done");

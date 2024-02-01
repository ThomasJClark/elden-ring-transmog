using SoulsFormats;

string gamePath = @"C:\Program Files (x86)\Steam\steamapps\common\ELDEN RING\Game";

TransmogCommandLineOptions options = CommandLine.Parser.Default
    .ParseArguments<TransmogCommandLineOptions>(args)
    .Value;

bool isInputVanilla = gamePath == options.InputPath;
bool isInputConvergence = options.InputPath.ToUpper().Contains("CONVERGENCE");
bool isInputReforged = options.InputPath.ToUpper().Contains("ERR");
bool isInputDetailedItemDescriptions = options.InputPath.ToUpper().Contains("DETAILED");

string armorFileName = @"N:\GR\data\Param\param\GameParam\EquipParamProtector.param";
string itemsFileName = @"N:\GR\data\Param\param\GameParam\EquipParamGoods.param";
string materialSetFileName = @"N:\GR\data\Param\param\GameParam\EquipMtrlSetParam.param";
string shopLineupFileName = @"N:\GR\data\Param\param\GameParam\ShopLineupParam.param";
string spEffectFileName = @"N:\GR\data\Param\param\GameParam\SpEffectParam.param";
string spEffectVfxFileName = @"N:\GR\data\Param\param\GameParam\SpEffectVfxParam.param";
string armorNameFileName = @"N:\GR\data\INTERROOT_win64\msg\engUS\ProtectorName.fmg";
string commonTalkFileName = @"N:\GR\data\INTERROOT_win64\script\talk\m00_00_00_00\t000001000.esd";
string roundtableTalkFileName =
    @"N:\GR\data\INTERROOT_win64\script\talk\m11_10_00_00\t608001110.esd";

int itemTypeArmor = 1;
int itemTypeGood = 3;
int invisibleIconId = 3142;

var GetBinderFile = (BND4 bnd, string fileName) => bnd.Files.Find(f => f.Name == fileName)!;

Console.WriteLine($"Reading vanilla data from \"{options.InputPath}\"...");

var paramDefs = Directory
    .GetFiles(Path.Combine("Paramdex", "ER", "Defs"))
    .Select(PARAMDEF.XmlDeserialize);

// Read the input data from the unpacked game folder
Console.WriteLine($"  Reading item names...");

BND4 vanillaItemMsgBnd = BND4.Read(Path.Combine(gamePath, "msg", "engus", "item.msgbnd.dcx"));
FMG vanillaArmorNames = FMG.Read(GetBinderFile(vanillaItemMsgBnd, armorNameFileName).Bytes);

Dictionary<string, TransmogMessageUtils.TransmogMessages> itemMessagesByLang =
    TransmogMessageUtils.ReadLocalizedMessages(
        options.InputPath,
        "item.msgbnd.dcx",
        new[]
        {
            TransmogMessageUtils.ArmorNameFileName,
            TransmogMessageUtils.ArmorInfoFileName,
            TransmogMessageUtils.ArmorCaptionFileName,
            TransmogMessageUtils.ItemNameFileName,
            TransmogMessageUtils.ItemInfoFileName,
            TransmogMessageUtils.ItemCaptionFileName
        }
    );

Console.WriteLine($"  Reading menu text...");

Dictionary<string, TransmogMessageUtils.TransmogMessages> menuMessagesByLang =
    TransmogMessageUtils.ReadLocalizedMessages(
        options.InputPath,
        "menu.msgbnd.dcx",
        new[]
        {
            TransmogMessageUtils.TalkTextFileName,
            TransmogMessageUtils.MapTextFileName,
            TransmogMessageUtils.MenuTextFileName
        }
    );

Console.WriteLine($"  Reading params...");
BND4 paramBnd = SFUtil.DecryptERRegulation(Path.Combine(options.InputPath, "regulation.bin"));
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

Console.WriteLine($"  Reading dialogue trees...");
BND4 commonTalkBnd = BND4.Read(
    Path.Combine(options.InputPath, "script", "talk", "m00_00_00_00.talkesdbnd.dcx")
);
BND4 roundtableTalkBnd = BND4.Read(
    Path.Combine(options.InputPath, "script", "talk", "m11_10_00_00.talkesdbnd.dcx")
);
ESD commonTalk = ESD.Read(GetBinderFile(commonTalkBnd, commonTalkFileName).Bytes);
ESD roundtableTalk = ESD.Read(GetBinderFile(roundtableTalkBnd, roundtableTalkFileName).Bytes);
ESD transmogTalk = ESD.Read("mods/common/script/talk/t000001000.esd");

int nextMaterialSetId = 6900000;
int startTransmogHeadShopLineupId = 4000000;
int nextTransmogHeadShopLineupId = startTransmogHeadShopLineupId;
int startTransmogBodyShopLineupId = 4100000;
int nextTransmogBodyShopLineupId = startTransmogBodyShopLineupId;
int startTransmogArmsShopLineupId = 4200000;
int nextTransmogArmsShopLineupId = startTransmogArmsShopLineupId;
int startTransmogLegsShopLineupId = 4300000;
int nextTransmogLegsShopLineupId = startTransmogLegsShopLineupId;
int nextSpEffectId = 690001;
int nextSpEffectVfxId = 690001;

Console.WriteLine("  Reading events...");
var commonEmevdPath = Path.Combine(options.InputPath, "event/common.emevd.dcx");
var commonEmevd = EMEVD.Read(
    DCX.Decompress(File.ReadAllBytes(commonEmevdPath), out DCX.Type commonEvevdDcxType)
);
var transmogFuncEmevd = EMEVD.Read(
    DCX.Decompress(File.ReadAllBytes("mods/common/event/transmog_func.emevd.dcx"))
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
int itemIconIdIdx = items.GetCellIndex("iconId");

List<int> bareArmorIds =
    new()
    {
        10000, // Head
        10100, // Body
        10200, // Arms
        10300, // Legs
    };

// Cut armor to generate IDs for, but not include in the mod. These were included in previous
// versions of the mod, so their IDs need to be skipped so save files remain compatible.
List<int> skippedArmorIds =
    new()
    {
        610000, // Ragged Hat
        610100, // Ragged Armor
        610200, // Ragged Gloves
        610300 // Ragged Loincloth
    };

if (isInputConvergence)
{
    skippedArmorIds.Add(963200); // Bloodsoaked Manchettes (given a new ID in The Convergence)
}

// Add the menu text used to open the transmogrify screen
int transmogTalkTextId = 69000000;
int transmogHeadMenuTextId = 690000;
int transmogBodyMenuTextId = 690001;
int transmogArmsMenuTextId = 690002;
int transmogLegsMenuTextId = 690003;

foreach (var (_, FMGs, Messages) in menuMessagesByLang.Values)
{
    var talkTexts = FMGs[TransmogMessageUtils.TalkTextFileName];
    if (isInputReforged)
    {
        talkTexts[transmogTalkTextId] =
            $"<img src='img://SB_ERR_Grace_AlterGarments.png' height='32' width='32' vspace='-16'/> {Messages["Transmogrify armor"]}";
        talkTexts[69000010] =
            $"<img src='img://SB_ERR_A_Mind' height='32' width='32' vspace='-16'/> {Messages["Transmogrify head"]}";
        talkTexts[69000011] =
            $"<img src='img://SB_ERR_A_Vigor' height='32' width='32' vspace='-16'/> {Messages["Transmogrify body"]}";
        talkTexts[69000012] =
            $"<img src='img://SB_ERR_A_Strength' height='32' width='32' vspace='-16'/> {Messages["Transmogrify arms"]}";
        talkTexts[69000013] =
            $"<img src='img://SB_ERR_A_Endurance' height='32' width='32' vspace='-16'/> {Messages["Transmogrify legs"]}";
        talkTexts[69000020] =
            $"<img src='img://SB_ERR_Grace_AlterGarments.png' height='32' width='32' vspace='-16'/> {Messages["Untransmogrify equipped armor"]}";
    }
    else
    {
        talkTexts[transmogTalkTextId] = Messages["Transmogrify armor"];
        talkTexts[69000010] = Messages["Transmogrify head"];
        talkTexts[69000011] = Messages["Transmogrify body"];
        talkTexts[69000012] = Messages["Transmogrify arms"];
        talkTexts[69000013] = Messages["Transmogrify legs"];
        talkTexts[69000020] = Messages["Untransmogrify equipped armor"];
    }

    var menuTexts = FMGs[TransmogMessageUtils.MenuTextFileName];
    menuTexts[transmogHeadMenuTextId] = Messages["Transmogrify Head"];
    menuTexts[transmogBodyMenuTextId] = Messages["Transmogrify Body"];
    menuTexts[transmogArmsMenuTextId] = Messages["Transmogrify Arms"];
    menuTexts[transmogLegsMenuTextId] = Messages["Transmogrify Legs"];

    var mapTexts = FMGs[TransmogMessageUtils.MapTextFileName];
    mapTexts[690000] = Messages["Untransmogrified armor has been returned to your inventory"];
}

/**
 * Add a new armor piece with the stats of the first armor and the appearance of the second
 */
PARAM.Row AddArmor(int id, PARAM.Row baseArmorRow, PARAM.Row targetArmorRow)
{
    var armorRow = new PARAM.Row(baseArmorRow) { ID = id, Name = null };

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
    }

    if (!options.SkipText)
    {
        TransmogMessageUtils.SetArmorText(
            itemMessagesByLang,
            armorRow.ID,
            (lang, messages) =>
            {
                var (baseArmorName, baseArmorInfo, baseArmorCaption) =
                    TransmogMessageUtils.GetArmorText(messages.FMGs, baseArmorRow.ID);

                var (targetArmorName, targetArmorInfo, targetArmorCaption) =
                    TransmogMessageUtils.GetArmorText(messages.FMGs, targetArmorRow.ID);

                if (isInvisible)
                {
                    return (
                        lang == "engus"
                            ? $"Invisible {baseArmorName}"
                            : $"{baseArmorName} [{messages.Messages["Invisible"]}]",
                        baseArmorInfo,
                        baseArmorCaption
                    );
                }

                string armorName =
                    lang == "engus"
                        ? $"{targetArmorName} (Transmogrified {baseArmorName})"
                        : $"{targetArmorName} [{baseArmorName}]";
                string armorInfo = targetArmorCaption;
                string armorCaption = targetArmorCaption;

                // Armor pieces in The Convergence and Elden Ring Reforged have effects in the description.
                // Copy that over to the new armor, so the description is still accurate.
                if (isInputConvergence || isInputReforged)
                {
                    bool isEffect(string line) =>
                        line.StartsWith("Increases ")
                        || line.StartsWith("Decreases ")
                        || line.StartsWith("Reduces ")
                        || line.StartsWith("Strengthens ")
                        || line.StartsWith("Raises ")
                        || line.Contains("If you ")
                        || line.Contains("When you ");

                    var baseDescription = (baseArmorCaption ?? "").Split("\n");
                    var targetDescription = (targetArmorCaption ?? "").Split("\n");

                    armorCaption =
                        string.Join("\n", targetDescription.TakeWhile(line => !isEffect(line)))
                            .Trim()
                        + "\n\n"
                        + string.Join(
                                "\n",
                                baseDescription
                                    .SkipWhile(line => !isEffect(line))
                                    .TakeWhile(line => isEffect(line))
                            )
                            .Trim()
                        + "\n\n"
                        + string.Join(
                                "\n",
                                targetDescription
                                    .SkipWhile(line => !isEffect(line))
                                    .SkipWhile(line => isEffect(line))
                            )
                            .Trim();
                }

                return (armorName, armorInfo, armorCaption);
            }
        );
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
    var transmogVfx = new PARAM.Row(nextSpEffectVfxId++, null, spEffectVfx.AppliedParamdef);
    transmogVfx["transformProtectorId"].Value = armorRow.ID;
    transmogVfx["isVisibleDeadChr"].Value = (byte)1;
    spEffectVfx.Rows.Add(transmogVfx);

    var transmogEffect = new PARAM.Row(nextSpEffectId++, null, spEffects.AppliedParamdef);
    transmogEffect["vfxId"].Value = transmogVfx.ID;
    transmogEffect["equipWeightChangeRate"].Value = 1.0f;
    transmogEffect["hpRecoverRate"].Value = 1.0f;
    transmogEffect["soulRate"].Value = 1.0f;
    transmogEffect["fallDamageRate"].Value = 1.0f;
    transmogEffect["allItemWeightChangeRate"].Value = 1.0f;
    transmogEffect["grabityRate"].Value = 1.0f;
    transmogEffect["soulStealRate"].Value = 1.0f;
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

PARAM.Row AddPseudoTransmogShopItem(int itemId, PARAM.Row armorRow)
{
    var transmogItem = new PARAM.Row(itemId, null, items.AppliedParamdef);

    transmogItem[itemGoodsTypeIdx].Value = 1; // key item
    transmogItem[itemSaleValueIdx].Value = 0;
    transmogItem[itemMaxNumIdx].Value = 1;
    items.Rows.Add(transmogItem);

    if (!options.SkipText)
    {
        TransmogMessageUtils.SetItemText(
            itemMessagesByLang,
            transmogItem.ID,
            (lang, messages) =>
            {
                if (bareArmorIds.Contains(armorRow.ID))
                {
                    return (messages.Messages["Invisible"], "", "");
                }

                return TransmogMessageUtils.GetArmorText(messages.FMGs, armorRow.ID);
            }
        );
    }

    return transmogItem;
}

PARAM.Row AddPseudoTransmogEffectItem(int itemId)
{
    var transmogItem = new PARAM.Row(itemId, null, items.AppliedParamdef);

    transmogItem[itemGoodsTypeIdx].Value = 12; // info item
    transmogItem[itemSaleValueIdx].Value = 0;
    transmogItem[itemMaxNumIdx].Value = 2;
    transmogItem[itemIconIdIdx].Value = 11;
    items.Rows.Add(transmogItem);

    TransmogMessageUtils.SetItemText(
        itemMessagesByLang,
        transmogItem.ID,
        (lang, messages) => ("Transmogrification Orb", "", "")
    );

    return transmogItem;
}

Console.WriteLine("Adding dialogue trees...");

var (transmogStateGroupID, transmogStateGroup) = transmogTalk.StateGroups.First();

commonTalk.StateGroups[transmogStateGroupID] = transmogStateGroup;
TransmogTalkUtils.AddTransmogMenuOption(
    commonTalk.StateGroups,
    (int)transmogStateGroupID,
    15000395, // "Sort Chest"
    transmogTalkTextId // "Transmogrify armor"
);

roundtableTalk.StateGroups[transmogStateGroupID] = transmogStateGroup;
TransmogTalkUtils.AddTransmogMenuOption(
    roundtableTalk.StateGroups,
    (int)transmogStateGroupID,
    26080000, // "Apply cosmetics"
    transmogTalkTextId // "Transmogrify armor"
);

Console.WriteLine("Adding event handlers...");

// Add a speffect that triggers an event to undo all equipped transmogs
var undoTransmogEffect = new PARAM.Row(250, "Undo transmog", spEffects.AppliedParamdef);
undoTransmogEffect["effectTargetSelf"].Value = (byte)1;
undoTransmogEffect["effectTargetFriend"].Value = (byte)1;
undoTransmogEffect["effectTargetPlayer"].Value = (byte)1;
undoTransmogEffect["effectTargetSelfTarget"].Value = (byte)1;
spEffects.Rows.Add(undoTransmogEffect);

EMEVD.Event initializeEvent = commonEmevd.Events.Find(evt => evt.ID == 0)!;
EMEVD.Event postUndoTransmogEvent = transmogFuncEmevd.Events.Find(evt => evt.ID == 9007102)!;
EMEVD.Event buyPseudoTransmogEvent = transmogFuncEmevd.Events.Find(evt => evt.ID == 9007103)!;
EMEVD.Event applyPseudoTransmogEvent = transmogFuncEmevd.Events.Find(evt => evt.ID == 9007104)!;
EMEVD.Event undoPseudoTransmogEvent = new(9007105);
EMEVD.Event undoTransmogEvent = new(9007106);

var initializeInstructions = new List<EMEVD.Instruction>
{
    TransmogEventUtils.InitializeEvent(0, (int)undoTransmogEvent.ID, 0)
};

commonEmevd.Events.Add(postUndoTransmogEvent);
commonEmevd.Events.Add(buyPseudoTransmogEvent);
commonEmevd.Events.Add(applyPseudoTransmogEvent);
commonEmevd.Events.Add(undoPseudoTransmogEvent);
commonEmevd.Events.Add(undoTransmogEvent);

undoTransmogEvent.Instructions.Add(
    TransmogEventUtils.IfCharacterHasSpEffect(
        TransmogEventUtils.MAIN,
        TransmogEventUtils.ENTITY_PLAYER,
        undoTransmogEffect.ID,
        true,
        TransmogEventUtils.EQUAL,
        1.0f
    )
);

if (isInputConvergence)
{
    int hideHeadgearFlagId = 68505;
    undoTransmogEvent.Instructions.AddRange(
        new List<EMEVD.Instruction>()
        {
            TransmogEventUtils.SkipIfEventFlag(
                2,
                TransmogEventUtils.OFF,
                TransmogEventUtils.EVENT_FLAG,
                hideHeadgearFlagId
            ),
            TransmogEventUtils.SetEventFlag(
                TransmogEventUtils.EVENT_FLAG,
                hideHeadgearFlagId,
                TransmogEventUtils.OFF
            ),
            TransmogEventUtils.InitializeEvent(9999, (int)postUndoTransmogEvent.ID, 0),
            TransmogEventUtils.WaitFixedTimeFrames(0)
        }
    );
}

Console.WriteLine("Generating transmogrified armor...");

List<PARAM.Row> validArmorRows = new();
List<int> vanillaArmorIds = new();
foreach (var row in vanillaArmor.Rows)
{
    if (
        row.ID >= 10000
        && vanillaArmorNames[row.ID] != null
        && !vanillaArmorNames[row.ID].StartsWith("[ERROR]")
        && !vanillaArmorNames[row.ID].EndsWith(" (Altered)")
    )
    {
        vanillaArmorIds.Add(row.ID);
        validArmorRows.Add(armor[row.ID]);
    }
}

// Generate transmogs for modded armor sets (e.g. new sets added in The Convergence) after vanilla
// armor, to keep IDs of vanilla transmogs compatible with previous version of this mod
if (!isInputVanilla)
{
    foreach (var row in armor.Rows)
    {
        var (armorName, armorInfo, armorCaption) = TransmogMessageUtils.GetArmorText(
            itemMessagesByLang["engus"].FMGs,
            row.ID
        );

        if (
            row.ID >= 10000
            && vanillaArmor[row.ID] == null
            && armorName != null
            && !armorName.StartsWith("[ERROR]")
            && !armorName.EndsWith(" (Altered)")
            && armorInfo != null
            && armorCaption != null
        )
        {
            validArmorRows.Add(row);
        }
    }

    // Generate transmogs for cut armor sets restored by mods last, to keep IDs of vanilla transmogs
    // compatible with previous version of this mod
    var restoredArmorIds = Array.Empty<int>();
    if (isInputConvergence)
    {
        restoredArmorIds = new[]
        {
            1930300, // Deathbed Smalls
            1950100, // Millicent's Robe
            1950200, // Millicent's Gloves
            1950300, // Millicent's Boots
            1970100, // Millicent's Tunic
            1970200, // Golden Prosthetic
        };
    }
    else if (isInputReforged)
    {
        restoredArmorIds = new[]
        {
            700000, // Brave's Cord Circlet
            700100, // Brave's Battlewear
            700200, // Brave's Bracer
            700300, // Brave's Legwraps
            701000, // Brave's Leather Helm
            920000, // Grass Hair Ornament
            1930300, // Deathbed Smalls
            1950100, // Millicent's Clothes
            1950200, // Millicent's Gloves
            1950300, // Millicent's Boots
            1970200, // Valkyrie's Prosthesis
        };
    }

    foreach (var row in armor.Rows)
    {
        if (restoredArmorIds.Contains(row.ID))
        {
            validArmorRows.Add(row);
        }
    }
}

Dictionary<int, PARAM.Row> armorById = armor.Rows.ToDictionary(row => row.ID);

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

        var isModded = !vanillaArmorIds.Contains(armorRow.ID);
        var index = isModded ? moddedArmorIndex++ : armorIndex++;

        // Add a speffect that makes the player character appear to be wearing the target armor
        var pseudoTransmogEffect = AddPseudoTransmogEffect(armorRow);

        // Add a placeholder item that the player can buy in a fake shop to activate the transmog
        var pseudoTransmogShopItem = AddPseudoTransmogShopItem(690000 + index, armorRow);
        AddPseudoTransmogShopLineup(pseudoTransmogShopItem.ID, armorRow);

        // Add a persistent item stored in the player's inventory to indicate their transmog
        // selection. Event flags accomplish the same thing, but they don't work with Seamless
        // Co-op or during pseudo-mutliplayer missions
        var pseudoTransmogEffectItem = AddPseudoTransmogEffectItem(49000 + index);
        if (!options.SkipEvents)
        {
            // When the player buys the key item, exchange it for the effect item and remove any
            // other effect items
            initializeInstructions.Add(
                TransmogEventUtils.InitializeEvent(
                    index,
                    (int)buyPseudoTransmogEvent.ID,
                    pseudoTransmogShopItem.ID,
                    pseudoTransmogEffectItem.ID
                )
            );

            // When the player has the effect item in their inventory, apply the speffect that
            // transforms their head
            initializeInstructions.Add(
                TransmogEventUtils.InitializeEvent(
                    index,
                    (int)applyPseudoTransmogEvent.ID,
                    pseudoTransmogEffectItem.ID,
                    pseudoTransmogEffect.ID
                )
            );

            // Remove the placeholder item when a different pseudo-transmog is applied
            undoPseudoTransmogEvent.Instructions.Add(
                TransmogEventUtils.RemoveItemFromPlayer(
                    TransmogEventUtils.ITEM_TYPE_GOOD,
                    pseudoTransmogEffectItem.ID,
                    1
                )
            );

            // Remove the placeholder item when the "Untransmogrify equipped armor" menu item is
            // selected
            undoTransmogEvent.Instructions.AddRange(
                new List<EMEVD.Instruction>()
                {
                    TransmogEventUtils.IfPlayerHasdoesntHaveItem(
                        TransmogEventUtils.OR_01,
                        TransmogEventUtils.ITEM_TYPE_GOOD,
                        pseudoTransmogEffectItem.ID,
                        1
                    ),
                    TransmogEventUtils.SkipIfConditionGroupStateUncompiled(
                        2,
                        TransmogEventUtils.CONDITION_STATE_FAIL,
                        TransmogEventUtils.OR_01
                    ),
                    TransmogEventUtils.RemoveItemFromPlayer(
                        TransmogEventUtils.ITEM_TYPE_GOOD,
                        pseudoTransmogEffectItem.ID,
                        1
                    ),
                    TransmogEventUtils.InitializeEvent(
                        armorRow.ID,
                        (int)postUndoTransmogEvent.ID,
                        protectorCategory
                    ),
                    TransmogEventUtils.WaitFixedTimeFrames(0)
                }
            );
        }
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
        // If this armor isn't present or is in this list, it's a vanilla piece that was patched
        // out of the game or removed in a mod. Increment the ID for save file compatibility and
        // skip it
        if (baseArmorRow == null || skippedArmorIds.Contains(baseArmorRow.ID))
        {
            baseArmorIndex++;
            continue;
        }

        // Empty slots aren't obtainable as items, so they can't be the base armor
        if (bareArmorIds.Contains(baseArmorRow.ID))
        {
            continue;
        }

        var baseProtectorCategory = (byte)baseArmorRow[armorProtectorCategoryIdx].Value;
        var isPseudoTransmog = baseProtectorCategory == 0;
        var isBaseModded = !vanillaArmorIds.Contains(baseArmorRow.ID);
        var baseIndex = isBaseModded ? moddedBaseArmorIndex++ : baseArmorIndex++;

        var baseMaterialSet = AddMaterialSet(itemTypeArmor, baseArmorRow.ID);

        // Target armor determines the appearance of the transmogrified armor
        int targetArmorIndex = 1;
        int moddedTargetArmorIndex = 651;
        foreach (var targetArmorRow in validArmorRows)
        {
            if (targetArmorRow == null)
            {
                continue;
            }

            var targetProtectorCategory = (byte)targetArmorRow[armorProtectorCategoryIdx].Value;

            // Allow transmogs to different armor pieces of the same category
            // (e.g. helmet => different helmet)
            if (targetArmorRow == baseArmorRow || baseProtectorCategory != targetProtectorCategory)
            {
                continue;
            }

            var isTargetModded = !vanillaArmorIds.Contains(targetArmorRow.ID);
            var targetIndex = isTargetModded ? moddedTargetArmorIndex++ : targetArmorIndex++;

            if (
                !armorById.ContainsKey(baseArmorRow.ID)
                || !armorById.ContainsKey(targetArmorRow.ID)
                || skippedArmorIds.Contains(targetArmorRow.ID)
                // Generate invisible helms, even though they are transmogged through the speffect
                // system. Previous version fo the mod supported invisible helms, so we need to
                // generate the params for backwards compatibility.
                || (isPseudoTransmog && !bareArmorIds.Contains(targetArmorRow.ID))
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

            if (!isPseudoTransmog)
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
                    TransmogEventUtils.IfPlayerHasArmorEquipped(
                        TransmogEventUtils.OR_01,
                        baseProtectorCategory,
                        transmogrifiedArmorRow.ID
                    ),
                    TransmogEventUtils.SkipIfConditionGroupStateUncompiled(
                        3,
                        TransmogEventUtils.CONDITION_STATE_FAIL,
                        TransmogEventUtils.OR_01
                    ),
                    TransmogEventUtils.RemoveItemFromPlayer(
                        TransmogEventUtils.ITEM_TYPE_ARMOR,
                        transmogrifiedArmorRow.ID,
                        1
                    ),
                    TransmogEventUtils.DirectlyGivePlayerItem(
                        TransmogEventUtils.ITEM_TYPE_ARMOR,
                        baseArmorRow.ID,
                        6001,
                        1
                    ),
                    TransmogEventUtils.InitializeEvent(
                        transmogrifiedArmorRow.ID,
                        (int)postUndoTransmogEvent.ID,
                        baseProtectorCategory
                    ),
                    TransmogEventUtils.WaitFixedTimeFrames(0)
                }
            );
        }
    }
}

AddPseudoTransmogs();
AddTransmogs();

undoTransmogEvent.Instructions.Add(
    TransmogEventUtils.EndUnconditionally(TransmogEventUtils.EXECUTION_END_TYPE_RESTART)
);

// Initialize our events at the start of the init event, so they're before the
// "EndIf(!PlayerIsInOwnWorld());". This allows the events to run in pseudo-multiplayer invasions
// (and maybe Seamless Co-op?)
initializeInstructions.AddRange(initializeEvent.Instructions);
initializeEvent.Instructions = initializeInstructions;

Console.WriteLine("Summary (after):");
Console.WriteLine($"  Armor: {armor.Rows.Count}");
Console.WriteLine($"  Material sets: {materialSets.Rows.Count}");
Console.WriteLine($"  Shop lineups: {shopLineups.Rows.Count}");

if (!options.DryRun)
{
    Console.WriteLine($"Writing mod data to \"{options.ModPath}\"...");

    // Write the modified data to the mod folder
    if (!options.SkipText)
    {
        Console.WriteLine($"  Writing item names...");
        TransmogMessageUtils.WriteLocalizedMessages(
            options.ModPath,
            "item.msgbnd.dcx",
            itemMessagesByLang
        );

        Console.WriteLine($"  Writing menu text...");
        TransmogMessageUtils.WriteLocalizedMessages(
            options.ModPath,
            "menu.msgbnd.dcx",
            menuMessagesByLang
        );
    }

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
    SFUtil.EncryptERRegulation(Path.Combine(options.ModPath, "regulation.bin"), paramBnd);

    if (!options.SkipEvents)
    {
        Console.WriteLine("  Writing events...");
        Directory.CreateDirectory(Path.Combine(options.ModPath, "event"));
        File.WriteAllBytes(
            Path.Combine(options.ModPath, "event/common.emevd.dcx"),
            DCX.Compress(commonEmevd.Write(), commonEvevdDcxType)
        );
    }

    Console.WriteLine("  Writing dialogue trees...");
    GetBinderFile(commonTalkBnd, commonTalkFileName).Bytes = commonTalk.Write();
    GetBinderFile(roundtableTalkBnd, roundtableTalkFileName).Bytes = roundtableTalk.Write();
    commonTalkBnd.Write(
        Path.Combine(options.ModPath, "script", "talk", "m00_00_00_00.talkesdbnd.dcx")
    );
    roundtableTalkBnd.Write(
        Path.Combine(options.ModPath, "script", "talk", "m11_10_00_00.talkesdbnd.dcx")
    );
}

Console.WriteLine("Done");

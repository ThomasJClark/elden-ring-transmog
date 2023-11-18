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
string armorNameFileName = @"N:\GR\data\INTERROOT_win64\msg\engUS\ProtectorName.fmg";
string armorInfoFileName = @"N:\GR\data\INTERROOT_win64\msg\engUS\ProtectorInfo.fmg";
string armorCaptionFileName = @"N:\GR\data\INTERROOT_win64\msg\engUS\ProtectorCaption.fmg";
string talkTextFileName = @"N:\GR\data\INTERROOT_win64\msg\engUS\EventTextForTalk.fmg";

int itemTypeArmor = 1;

var GetBinderFile = (BND4 bnd, string fileName) => bnd.Files.Find(f => f.Name == fileName)!;

Console.WriteLine($"Reading vanilla data from \"{inputPath}\"...");

Dictionary<string, PARAMDEF> paramDefs = Directory
    .GetFiles(Path.Combine("..", "Paramdex", "ER", "Defs"))
    .Select(PARAMDEF.XmlDeserialize)
    .ToDictionary(paramDef => paramDef.ParamType);

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

BND4 vanillaParamBnd = isInputVanilla
    ? paramBnd
    : SFUtil.DecryptERRegulation(Path.Combine(gamePath, "regulation.bin"));
PARAM vanillaArmor = isInputVanilla
    ? armor
    : PARAM.Read(GetBinderFile(vanillaParamBnd, armorFileName).Bytes);

vanillaArmor.ApplyParamdef(paramDefs[vanillaArmor.ParamType]);
armor.ApplyParamdef(paramDefs[armor.ParamType]);
armor.Rows = new List<PARAM.Row>(armor.Rows);

int nextMaterialSetId = 6900000;
materialSets.ApplyParamdef(paramDefs[materialSets.ParamType]);
materialSets.Rows = new List<PARAM.Row>(materialSets.Rows);

int nextShopLineupId = 4000000;
shopLineups.ApplyParamdef(paramDefs[shopLineups.ParamType]);
shopLineups.Rows = new List<PARAM.Row>(shopLineups.Rows);

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

// Bare head/body/etc/arms/legs
int[] bareArmorIds = new int[] { 10000, 10100, 10200, 10300 };

/**
 * Add a new armor piece with the stats of the first armor and the appearance of the second
 */
PARAM.Row AddArmor(int id, PARAM.Row baseArmorRow, PARAM.Row targetArmorRow)
{
    var armorRow = new PARAM.Row(baseArmorRow);
    armorRow.ID = id;

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

var armorById = armor.Rows.ToDictionary(row => row.ID);

// Base armor determines the stats of the transmogrified armor
int i = 100;
foreach (var baseArmorRow in validArmorRows)
{
    // Empty slots aren't obtainable as items, so they can't be the base armor
    if (bareArmorIds.Contains(baseArmorRow.ID))
    {
        continue;
    }

    var baseMaterialSet = AddMaterialSet(itemTypeArmor, baseArmorRow.ID);
    var baseProtectorCategory = (byte)baseArmorRow[armorProtectorCategoryIdx].Value;

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

        if (!armorById.ContainsKey(baseArmorRow.ID) || !armorById.ContainsKey(targetArmorRow.ID))
        {
            continue;
        }

        // Don't allow transmogging helms or gauntlets except to make them invisible. This is
        // to avoid hitting the 16 bit row limit, and because these pieces are light and
        // have a smaller impact on defense.
        if (
            (baseProtectorCategory == 0 || baseProtectorCategory == 2)
            && !bareArmorIds.Contains(targetArmorRow.ID)
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

        // Add a shop item to create the transmogrified armor, and to turn the transmogrified
        // armor back into the base armor
        var transmogrifiedMaterialSet = AddMaterialSet(itemTypeArmor, transmogrifiedArmorRow.ID);
        AddShopLineup(itemTypeArmor, transmogrifiedArmorRow.ID, baseMaterialSet.ID);
        AddShopLineup(itemTypeArmor, baseArmorRow.ID, transmogrifiedMaterialSet.ID);
    }

    i++;
}

// Add the menu text used to open the transmogrify screen
talkTexts[69000000] = isInputReforged
    ? "<img src='img://SB_ERR_Grace_AlterGarments.png' height='32' width='32' vspace='-16'/> Transmogrify equipment"
    : "Transmogrify equipment";

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
SFUtil.EncryptERRegulation(Path.Combine(modPath, "regulation.bin"), paramBnd);

Console.WriteLine("Done");

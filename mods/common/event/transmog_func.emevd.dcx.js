// ==EMEVD==
// @docs    er-common.emedf.json
// @compress    DCX_KRAK
// @game    Sekiro
// @string    
// @linked    []
// @version    3.4.1
// ==/EMEVD==

/**
 * Spawn SFX on the player after undoing a transmog. This event handler is automatically appended
 * to common.emevd.dcx when the mod is generated.
 */
$Event(9007102, Default, function (X0_4) {
    const soundId = 302061;
    const armorPieceSfxId = 302603;
    const characterSfxId = 643041;
 
    PlaySE(10000, SoundType.SFX, soundId);

    if (X0_4 == ArmorType.Head) {
        SpawnOneshotSFX(TargetEntityType.Character, 10000, 907, armorPieceSfxId);
    } else if (X0_4 == ArmorType.Body) {
        SpawnOneshotSFX(TargetEntityType.Character, 10000, 190, armorPieceSfxId);
    } else if (X0_4 == ArmorType.Arms) {
        SpawnOneshotSFX(TargetEntityType.Character, 10000, 192, armorPieceSfxId);
        SpawnOneshotSFX(TargetEntityType.Character, 10000, 1, armorPieceSfxId);
        SpawnOneshotSFX(TargetEntityType.Character, 10000, 2, armorPieceSfxId);
    } else if (X0_4 == ArmorType.Legs) {
        SpawnOneshotSFX(TargetEntityType.Character, 10000, 193, armorPieceSfxId);
        SpawnOneshotSFX(TargetEntityType.Character, 10000, 212, armorPieceSfxId);
        SpawnOneshotSFX(TargetEntityType.Character, 10000, 5, armorPieceSfxId);
        SpawnOneshotSFX(TargetEntityType.Character, 10000, 6, armorPieceSfxId);
    }

    // SpawnOneshotSFX(TargetEntityType.Character, 10000, 2, characterSfxId);


    if (X0_4 != ArmorType.Head) {
        DisplayStatusMessage(690000, Enabled);
    }

    EndEvent();
});

/**
 * Give the player an event flag when they "buy" a pseudo-transmog placeholder item
 */
$Event(9007103, Default, function(X0_4, X4_4) {
    WaitFor(PlayerHasItem(ItemType.Goods, X0_4));
    RemoveItemFromPlayer(ItemType.Goods, X0_4, 1);
    BatchSetEventFlags(690000, 690999, OFF);
    SetEventFlag(TargetEventFlagType.EventFlag, 68505, OFF); // Convergence "hide helmet" flag 
    SetEventFlag(TargetEventFlagType.EventFlag, X4_4, ON);
    SaveRequest();
    RestartEvent();
});

/**
 * Assign a speffect to the player to pseudo-transmogrify their head based on the above
 * event flag
 */
$Event(9007104, Default, function(X0_4, X4_4) {
    WaitForEventFlag(ON, TargetEventFlagType.EventFlag, X0_4);
    SetSpEffect(10000, X4_4);
    WaitForEventFlag(OFF, TargetEventFlagType.EventFlag, X0_4);
    ClearSpEffect(10000, X4_4);
    RestartEvent();
});

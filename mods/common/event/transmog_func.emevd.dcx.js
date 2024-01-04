// ==EMEVD==
// @docs    er-common.emedf.json
// @compress    DCX_KRAK
// @game    Sekiro
// @string    
// @linked    []
// @version    3.4.1
// ==/EMEVD==

const postUndoTransmogEvent = 9007102;
const buyPseudoTransmogEvent = 9007103;
const applyPseudoTransmogEvent = 9007104;
const undoPseudoTransmogEvent = 9007105;

/**
 * Spawn SFX on the player after undoing a transmog. This event handler is automatically appended
 * to common.emevd.dcx when the mod is generated.
 */
$Event(postUndoTransmogEvent, Default, function (X0_4) {
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
 * Give the player a placeholder item when they "buy" a pseudo-transmog
 */
$Event(buyPseudoTransmogEvent, Default, function(X0_4, X4_4) {
    WaitFor(PlayerHasItem(ItemType.Goods, X0_4));
 
    // Remove any existing placeholder items
    InitializeEvent(0, undoPseudoTransmogEvent, 0);
    WaitFor(ElapsedFrames(1));

    // Unset the Convergence "hide helmet" flag 
    SetEventFlag(TargetEventFlagType.EventFlag, 68505, OFF);
    
    // Exchange the shop item for the pseudo-transmog placeholder item
    RemoveItemFromPlayer(ItemType.Goods, X0_4, 1);
    DirectlyGivePlayerItem(ItemType.Goods, X4_4, 6001, 1);

    SaveRequest();
    RestartEvent();
});

/**
 * Assign a speffect to the player to pseudo-transmogrify their head
 */
$Event(applyPseudoTransmogEvent, Default, function(X0_4, X4_4) {
    WaitFor(PlayerHasItem(ItemType.Goods, X0_4));
    SetSpEffect(10000, X4_4);
    WaitFor(!PlayerHasItem(ItemType.Goods, X0_4));
    ClearSpEffect(10000, X4_4);
    RestartEvent();
});

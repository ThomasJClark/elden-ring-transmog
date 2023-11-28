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
$Event(0, Default, function (X0_4) {
  if (X0_4 == ArmorType.Head) {
    SpawnOneshotSFX(TargetEntityType.Character, 10000, 907, 300552);
  } else if (X0_4 == ArmorType.Body) {
    SpawnOneshotSFX(TargetEntityType.Character, 10000, 190, 300552);
  } else if (X0_4 == ArmorType.Arms) {
    SpawnOneshotSFX(TargetEntityType.Character, 10000, 192, 300552);
    SpawnOneshotSFX(TargetEntityType.Character, 10000, 1, 300552);
    SpawnOneshotSFX(TargetEntityType.Character, 10000, 2, 300552);
  } else if (X0_4 == ArmorType.Legs) {
    SpawnOneshotSFX(TargetEntityType.Character, 10000, 193, 300552);
    SpawnOneshotSFX(TargetEntityType.Character, 10000, 212, 300552);
    SpawnOneshotSFX(TargetEntityType.Character, 10000, 5, 300552);
    SpawnOneshotSFX(TargetEntityType.Character, 10000, 6, 300552);
  }
  SpawnOneshotSFX(TargetEntityType.Character, 10000, 2, 636505);
  EndEvent();
});

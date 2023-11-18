# Elden Ring Transmogrify Mod

Change your gear's appearance without changing its stats, or hide your helm or other armor parts. Compatible with game version 1.10, The Convergence Mod, and Elden Ring Reforged.

## Installation & Usage

See [the Nexus Mods page](https://www.nexusmods.com/eldenring/mods/3596).

## Development

### Params

This mod is mostly generated automatically. A C# program using [SoulsFormats](https://github.com/soulsmods/SoulsFormats) generates modded param entries for each armor set with different models, icons, and names.

To regenerate this data, install the .NET SDK and run the project. This will create the files `regulation.bin`, `msg\item.msgbnd.dcx`, and `msg\menu.msgbnd.dcx` in the `mods\ertransmog` folder.

```
dotnet run --project EldenRingTransmogrify
```

To generate versions of the params for different mods, include the path to the mod and the output path.

```
dotnet run --project EldenRingTransmogrify ../ConvergenceER/mod mods/ertransmog-convergence/
```

### Dialogue trees

The "Transmogrify armor" menu option is added by modifying several `talkesd`s using [esdtool](https://github.com/thefifthmatt/ESDLang). The source code is included in `mods\...\script\talk\src`, with the altered portions marked by "START MOD" and "END MOD".

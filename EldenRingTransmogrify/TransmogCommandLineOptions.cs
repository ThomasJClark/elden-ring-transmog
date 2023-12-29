using CommandLine;

public class TransmogCommandLineOptions
{
    [Value(0)]
    public required string InputPath { get; set; } =
        @"C:\Program Files (x86)\Steam\steamapps\common\ELDEN RING\Game";

    [Value(1)]
    public required string ModPath { get; set; } = "mods/ertransmogrify";

    [Option(longName: "dryRun")]
    public required bool DryRun { get; set; }

    [Option(longName: "skipText")]
    public required bool SkipText { get; set; }

    [Option(longName: "skipEvents")]
    public required bool SkipEvents { get; set; }
}

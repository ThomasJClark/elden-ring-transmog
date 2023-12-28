#pragma warning disable IL2026
using System.Text.Json;
using SoulsFormats;

/**
 * Manages new and modified message strings for the mod, including localized versions
 */
class TransmogMessageUtils
{
    public record TransmogMessages(
        BND4 BND,
        Dictionary<string, FMG> FMGs,
        Dictionary<string, string> Messages
    );

    private readonly static string[] SupportedLangs =
    {
        "deude",
        "engus",
        "frafr",
        "itait",
        "porbr",
        "spaar",
        "spaes",
        // "jpnjp",
        // "korkr",
        // "polpl",
        // "rusru",
        // "thath",
        // "zhocn",
        // "zhotw",
    };

    // item text
    public readonly static string ArmorNameFileName = "ProtectorName.fmg";
    public readonly static string ArmorInfoFileName = "ProtectorInfo.fmg";
    public readonly static string ArmorCaptionFileName = "ProtectorCaption.fmg";
    public readonly static string ItemNameFileName = "GoodsName.fmg";
    public readonly static string ItemInfoFileName = "GoodsInfo.fmg";
    public readonly static string ItemCaptionFileName = "GoodsCaption.fmg";

    // menu text
    public readonly static string TalkTextFileName = "EventTextForTalk.fmg";
    public readonly static string MapTextFileName = "EventTextForMap.fmg";
    public readonly static string MenuTextFileName = "GR_MenuText.fmg";

    /**
     * Read an input msgbnd file for every supported language, storing the results in a dictionary
     */
    public static Dictionary<string, TransmogMessages> ReadLocalizedMessages(
        string inputPath,
        string bndName,
        string[] fmgNames
    )
    {
        return SupportedLangs.ToDictionary(
            lang => lang,
            lang =>
            {
                var bnd = BND4.Read(GetMsgbndPath(inputPath, lang, bndName));
                var fmgs = fmgNames.ToDictionary(
                    name => name,
                    name =>
                    {
                        var bndFile = bnd.Files.Find(f => f.Name.EndsWith(@$"\{name}"))!;
                        return FMG.Read(bndFile.Bytes);
                    }
                );

                var messages = JsonSerializer.Deserialize<Dictionary<string, string>>(
                    File.ReadAllText(Path.Combine("mods", "common", "msg", $"{lang}.json"))
                )!;

                return new TransmogMessages(bnd, fmgs, messages);
            }
        );
    }

    /**
     * Write a modded msgbnd file for every supported language
     */
    public static void WriteLocalizedMessages(
        string outputPath,
        string bndName,
        Dictionary<string, TransmogMessages> messagesByLang
    )
    {
        foreach (var (lang, messages) in messagesByLang)
        {
            foreach (var binderFile in messages.BND.Files)
            {
                var fileName = binderFile.Name[(binderFile.Name.LastIndexOf(@"\") + 1)..];
                if (messages.FMGs.ContainsKey(fileName))
                {
                    binderFile.Bytes = messages.FMGs[fileName].Write();
                }
            }

            Console.WriteLine($"    {lang}");
            messages.BND.Write(GetMsgbndPath(outputPath, lang, bndName));
        }
    }

    private static string GetMsgbndPath(string path, string lang, string bndName)
    {
        return Path.Combine(path, "msg", lang, bndName);
    }

    public static (string, string, string) GetArmorText(Dictionary<string, FMG> fmgs, int id)
    {
        return (
            fmgs[ArmorNameFileName][id],
            fmgs[ArmorInfoFileName][id],
            fmgs[ArmorCaptionFileName][id]
        );
    }

    public static void SetItemText(
        Dictionary<string, TransmogMessages> messagesByLang,
        int id,
        Func<string, TransmogMessages, (string, string, string)> fn
    )
    {
        foreach (var (lang, messages) in messagesByLang)
        {
            (
                messages.FMGs[ItemNameFileName][id],
                messages.FMGs[ItemInfoFileName][id],
                messages.FMGs[ItemCaptionFileName][id]
            ) = fn(lang, messages);
        }
    }

    public static void SetArmorText(
        Dictionary<string, TransmogMessages> messagesByLang,
        int id,
        Func<string, TransmogMessages, (string, string, string)> fn
    )
    {
        foreach (var (lang, messages) in messagesByLang)
        {
            (
                messages.FMGs[ArmorNameFileName][id],
                messages.FMGs[ArmorInfoFileName][id],
                messages.FMGs[ArmorCaptionFileName][id]
            ) = fn(lang, messages);
        }
    }
}

#nullable disable
using System.Collections.Generic;
using System.Linq;

namespace SoulsFormats
{
    /// <summary>
    /// A simple string container used throughout the series.
    /// </summary>
    public class FMG : SoulsFile<FMG>
    {
        /// <summary>
        /// The strings contained in this FMG.
        /// </summary>
        public Dictionary<int, string> Entries;

        /// <summary>
        /// Indicates file format; 0 - DeS, 1 - DS1/DS2, 2 - DS3/BB.
        /// </summary>
        public FMGVersion Version;

        /// <summary>
        /// FMG file endianness. (Big = true)
        /// </summary>
        public bool BigEndian;

        /// <summary>
        /// Creates an empty FMG configured for DS1/DS2.
        /// </summary>
        public FMG()
        {
            Entries = new Dictionary<int, string>();
            Version = FMGVersion.DarkSouls1;
            BigEndian = false;
        }

        /// <summary>
        /// Creates an empty FMG configured for the specified version.
        /// </summary>
        public FMG(FMGVersion version)
        {
            Entries = new Dictionary<int, string>();
            Version = version;
            BigEndian = Version == FMGVersion.DemonsSouls;
        }

        /// <summary>
        /// Deserializes file data from a stream.
        /// </summary>
        protected override void Read(BinaryReaderEx br)
        {
            br.AssertByte(0);
            BigEndian = br.ReadBoolean();
            Version = br.ReadEnum8<FMGVersion>();
            br.AssertByte(0);

            br.BigEndian = BigEndian;
            bool wide = Version == FMGVersion.DarkSouls3;

            int fileSize = br.ReadInt32();
            br.AssertByte(1);
            br.AssertByte((byte)(Version == FMGVersion.DemonsSouls ? 0xFF : 0x00));
            br.AssertByte(0);
            br.AssertByte(0);
            int groupCount = br.ReadInt32();
            int stringCount = br.ReadInt32();

            if (wide)
                br.AssertInt32(0xFF);

            long stringOffsetsOffset;
            if (wide)
                stringOffsetsOffset = br.ReadInt64();
            else
                stringOffsetsOffset = br.ReadInt32();

            if (wide)
                br.AssertInt64(0);
            else
                br.AssertInt32(0);

            Entries = new Dictionary<int, string>(groupCount);
            for (int i = 0; i < groupCount; i++)
            {
                int offsetIndex = br.ReadInt32();
                int firstID = br.ReadInt32();
                int lastID = br.ReadInt32();

                if (wide)
                    br.AssertInt32(0);

                br.StepIn(stringOffsetsOffset + offsetIndex * (wide ? 8 : 4));
                {
                    for (int j = 0; j < lastID - firstID + 1; j++)
                    {
                        long stringOffset;
                        if (wide)
                            stringOffset = br.ReadInt64();
                        else
                            stringOffset = br.ReadInt32();

                        int id = firstID + j;
                        string text = stringOffset != 0 ? br.GetUTF16(stringOffset) : null;
                        Entries[id] = text;
                    }
                }
                br.StepOut();
            }
        }

        /// <summary>
        /// Serializes file data to a stream.
        /// </summary>
        protected override void Write(BinaryWriterEx bw)
        {
            bw.BigEndian = BigEndian;
            bool wide = Version == FMGVersion.DarkSouls3;

            bw.WriteByte(0);
            bw.WriteBoolean(bw.BigEndian);
            bw.WriteByte((byte)Version);
            bw.WriteByte(0);

            bw.ReserveInt32("FileSize");
            bw.WriteByte(1);
            bw.WriteByte((byte)(Version == FMGVersion.DemonsSouls ? 0xFF : 0x00));
            bw.WriteByte(0);
            bw.WriteByte(0);
            bw.ReserveInt32("GroupCount");
            bw.WriteInt32(Entries.Count);

            if (wide)
                bw.WriteInt32(0xFF);

            if (wide)
                bw.ReserveInt64("StringOffsets");
            else
                bw.ReserveInt32("StringOffsets");

            if (wide)
                bw.WriteInt64(0);
            else
                bw.WriteInt32(0);

            int groupCount = 0;
            var entriesList = Entries.ToList();
            entriesList.Sort((e1, e2) => e1.Key.CompareTo(e2.Key));
            for (int i = 0; i < entriesList.Count; i++)
            {
                bw.WriteInt32(i);
                bw.WriteInt32(entriesList[i].Key);
                while (
                    i < entriesList.Count - 1 && entriesList[i + 1].Key == entriesList[i].Key + 1
                )
                    i++;
                bw.WriteInt32(entriesList[i].Key);

                if (wide)
                    bw.WriteInt32(0);

                groupCount++;
            }
            bw.FillInt32("GroupCount", groupCount);

            if (wide)
                bw.FillInt64("StringOffsets", bw.Position);
            else
                bw.FillInt32("StringOffsets", (int)bw.Position);

            for (int i = 0; i < entriesList.Count; i++)
            {
                if (wide)
                    bw.ReserveInt64($"StringOffset{i}");
                else
                    bw.ReserveInt32($"StringOffset{i}");
            }

            for (int i = 0; i < entriesList.Count; i++)
            {
                string text = entriesList[i].Value;

                if (wide)
                    bw.FillInt64($"StringOffset{i}", text == null ? 0 : bw.Position);
                else
                    bw.FillInt32($"StringOffset{i}", text == null ? 0 : (int)bw.Position);

                if (text != null)
                    bw.WriteUTF16(entriesList[i].Value, true);
            }

            bw.FillInt32("FileSize", (int)bw.Position);
        }

        /// <summary>
        /// Returns the string with the given ID, or null if not present.
        /// </summary>
        public string this[int id]
        {
            get => Entries.ContainsKey(id) ? Entries[id] : null;
            set => Entries[id] = value;
        }

        /// <summary>
        /// Indicates the game this FMG is for, and thus the format it will be written in.
        /// </summary>
        public enum FMGVersion : byte
        {
            /// <summary>
            /// Demon's Souls
            /// </summary>
            DemonsSouls = 0,

            /// <summary>
            /// Dark Souls 1 and Dark Souls 2
            /// </summary>
            DarkSouls1 = 1,

            /// <summary>
            /// Bloodborne and Dark Souls 3
            /// </summary>
            DarkSouls3 = 2,
        }
    }
}

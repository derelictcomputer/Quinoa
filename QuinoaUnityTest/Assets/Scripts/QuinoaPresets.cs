using UnityEngine;
using System;
using System.Collections.Generic;
using System.IO;

namespace DerelictComputer
{
    public class QuinoaPresets
    {
        [Serializable]
        public class Preset
        {
            public double Speed;
            public double Pitch;
            public int WindowSize;
            public double Start;
            public double Length;
            public float Smoothness;
        }

        [Serializable]
        public class PresetContainer
        {
            public List<Preset> Presets = new List<Preset>();

            public int Count
            {
                get { return Presets.Count; }
            }

            public void Add(Preset p)
            {
                Presets.Add(p);
            }

            public Preset GetPreset(int idx)
            {
                if (idx < 0 || idx >= Presets.Count)
                {
                    return null;
                }

                return Presets[idx];
            }
        }

        private const string FileName = "QuinoaPresets.json";

        private static QuinoaPresets _instance;

        public static QuinoaPresets Instance
        {
            get
            {
                if (_instance == null)
                {
                    _instance = new QuinoaPresets();
                    _instance.Load();
                }

                return _instance;
            }
        }

        private PresetContainer _presets = new PresetContainer();

        public void Add(Quinoa q)
        {
            var p = new Preset
            {
                Speed = q.Speed,
                Pitch = q.Pitch,
                WindowSize = q.WindowSize,
                Start = q.Start,
                Length = q.Length,
                Smoothness = q.Smoothness
            };
            _presets.Add(p);
        }

        public Preset Get(int idx)
        {
            return _presets.GetPreset(idx);
        }

        public void Save()
        {
            File.WriteAllText(Path.Combine(Application.streamingAssetsPath, FileName), JsonUtility.ToJson(_presets, true));
        }

        public void Load()
        {
            if (!File.Exists(Path.Combine(Application.streamingAssetsPath, FileName)))
            {
                for (int i = 0; i < 10; i++)
                {
                    _presets.Add(new Preset());
                }
                return;
            }

            var json = File.ReadAllText(Path.Combine(Application.streamingAssetsPath, FileName));
            _presets = JsonUtility.FromJson<PresetContainer>(json);
        }
    }
}
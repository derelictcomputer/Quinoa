using System;
using System.Runtime.InteropServices;
using UnityEngine;

namespace DerelictComputer
{
    [RequireComponent(typeof(AudioSource))]
    public class Quinoa : MonoBehaviour
    {
        public AudioClip Clip;

        [Range(0, 9)] public int PresetIdx;

        public bool SavePresets;

        public bool ReloadPresets;

        private IntPtr _ptr = IntPtr.Zero;

        public double Speed
        {
            get { return _speed; }
            set
            {
                _speed = Math.Max(0.01, Math.Min(value, 8));
                if (_ptr == IntPtr.Zero) return;
                Quinoa_SetSpeed(_ptr, _speed);
                QuinoaPresets.Instance.Get(PresetIdx).Speed = _speed;
            }
        }

        public double Pitch
        {
            get { return _pitch; }
            set
            {
                _pitch = Math.Max(0.01, Math.Min(value, 8));
                if (_ptr == IntPtr.Zero) return;
                Quinoa_SetPitch(_ptr, _pitch);
                QuinoaPresets.Instance.Get(PresetIdx).Pitch = _pitch;
            }
        }

        public int WindowSize
        {
            get { return _windowSize; }
            set
            {
                _windowSize = Math.Max(128, Math.Min(value, 512));
                if (_ptr == IntPtr.Zero) return;
                Quinoa_SetWindowSize(_ptr, _windowSize);
                QuinoaPresets.Instance.Get(PresetIdx).WindowSize = _windowSize;
            }
        }

        public double Start
        {
            get { return _start; }
            set
            {
                _start = Math.Max(0.0, Math.Min(value, 0.9));
                if (_ptr == IntPtr.Zero) return;
                Quinoa_SetStart(_ptr, _start);
                QuinoaPresets.Instance.Get(PresetIdx).Start = _start;
            }
        }

        public double Length
        {
            get { return _length; }
            set
            {
                _length = Math.Max(0.1, Math.Min(value, 1));
                if (_ptr == IntPtr.Zero) return;
                Quinoa_SetLength(_ptr, _length);
                QuinoaPresets.Instance.Get(PresetIdx).Length = _length;
            }
        }

        public float Smoothness
        {
            get { return _smoothness; }
            set
            {
                _smoothness = Math.Max(0, Math.Min(value, 1));
                if (_ptr == IntPtr.Zero) return;
                Quinoa_SetWindowSmoothness(_ptr, _smoothness);
                QuinoaPresets.Instance.Get(PresetIdx).Smoothness = _smoothness;
            }
        }


        [DllImport("QuinoaUnity")]
        private static extern IntPtr Quinoa_New(double sampleRate);
        [DllImport("QuinoaUnity")]
        private static extern void Quinoa_Delete(IntPtr q);
        [DllImport("QuinoaUnity")]
        private static extern void Quinoa_SetSample(IntPtr q, float[] sample, int numSamples, double sampleRate);
        [DllImport("QuinoaUnity")]
        private static extern void Quinoa_ProcessBuffer(IntPtr q, float[] buffer, int numSamples, int numChannels);
        [DllImport("QuinoaUnity")]
        private static extern void Quinoa_SetSpeed(IntPtr q, double speed);
        [DllImport("QuinoaUnity")]
        private static extern void Quinoa_SetPitch(IntPtr q, double pitch);
        [DllImport("QuinoaUnity")]
        private static extern void Quinoa_SetWindowSize(IntPtr q, int winSize);
        [DllImport("QuinoaUnity")]
        private static extern void Quinoa_SetStart(IntPtr q, double start);
        [DllImport("QuinoaUnity")]
        private static extern void Quinoa_SetLength(IntPtr q, double length);
        [DllImport("QuinoaUnity")]
        private static extern void Quinoa_SetWindowSmoothness(IntPtr q, float smoothness);

        private int _presetIdx = -1;
        private double _speed = 1;
        private double _pitch = 1;
        private double _start = 0;
        private double _length = 1;
        private float _smoothness = 0;
        private int _windowSize = 256;

        private void OnEnable()
        {
            if (_ptr == IntPtr.Zero)
            {
                _ptr = Quinoa_New(AudioSettings.outputSampleRate);
                Debug.Log(AudioSettings.outputSampleRate);
            }

            // create a dummy clip and start playing it so 3d positioning works
            var dummyClip = AudioClip.Create("dummyclip", 1, 1, AudioSettings.outputSampleRate, false);
            dummyClip.SetData(new float[] { 1 }, 0);
            var audioSource = GetComponent<AudioSource>();
            audioSource.clip = dummyClip;
            audioSource.loop = true;
            audioSource.Play();

            float[] d = new float[Clip.samples * Clip.channels];
            if (Clip.GetData(d, 0))
            {
                float[] c = new float[Clip.samples];

                for (int i = 0; i < c.Length; ++i)
                {
                    c[i] = d[i*Clip.channels];
                }

                Quinoa_SetSample(_ptr, c, c.Length, Clip.frequency);
                Debug.Log(Clip.frequency);
            }

            Quinoa_SetSpeed(_ptr, Speed);
            Quinoa_SetPitch(_ptr, Pitch);
            Quinoa_SetWindowSize(_ptr, WindowSize);
            Quinoa_SetStart(_ptr, Start);
            Quinoa_SetLength(_ptr, Length);
            Quinoa_SetWindowSmoothness(_ptr, Smoothness);
        }

        private void OnDisable()
        {
            if (_ptr != IntPtr.Zero)
            {
                Quinoa_Delete(_ptr);
            }
        }

        private void OnValidate()
        {
            if (PresetIdx != _presetIdx)
            {
                _presetIdx = PresetIdx;
                var p = QuinoaPresets.Instance.Get(_presetIdx);
                if (p != null)
                {
                    Speed = p.Speed;
                    Pitch = p.Pitch;
                    WindowSize = p.WindowSize;
                    Start = p.Start;
                    Length = p.Length;
                    Smoothness = p.Smoothness;
                }
            }
            if (SavePresets)
            {
                SavePresets = false;
                QuinoaPresets.Instance.Save();
            }
            if (ReloadPresets)
            {
                ReloadPresets = false;
                QuinoaPresets.Instance.Load();
            }
        }

        private void OnAudioFilterRead(float[] buffer, int numChannels)
        {
            if (_ptr == IntPtr.Zero)
            {
                return;
            }

            Quinoa_ProcessBuffer(_ptr, buffer, buffer.Length, numChannels);
        }
    }
}
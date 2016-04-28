using System;
using System.Runtime.InteropServices;
using UnityEngine;

[RequireComponent(typeof(AudioSource))]
public class Quinoa : MonoBehaviour
{
    public AudioClip Clip;
    [Range(0.01f, 10f)] public double Speed;
    [Range(0.01f, 10f)] public double Pitch;
    [Range(128, 1024)] public int WindowSize;
    [Range(0f, 0.9f)] public double Start;
    [Range(0.1f, 1f)] public double Length;
    [Range(0f, 1f)] public float Smoothness;

    private IntPtr _ptr = IntPtr.Zero;

    [DllImport("QuinoaUnity")]
    private static extern IntPtr Quinoa_New(double sampleRate);
    [DllImport("QuinoaUnity")]
    private static extern void Quinoa_Delete(IntPtr q);
    [DllImport("QuinoaUnity")]
    private static extern void Quinoa_SetSample(IntPtr q, float[] sample, int numSamples);
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

    private void OnEnable()
    {
        if (_ptr == IntPtr.Zero)
        {
            _ptr = Quinoa_New(AudioSettings.outputSampleRate);
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
            Quinoa_SetSample(_ptr, d, d.Length);
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

    private void Update()
    {
        if (_ptr == IntPtr.Zero)
        {
            return;
        }

        Quinoa_SetSpeed(_ptr, Speed);
        Quinoa_SetPitch(_ptr, Pitch);
        Quinoa_SetWindowSize(_ptr, WindowSize);
        Quinoa_SetStart(_ptr, Start);
        Quinoa_SetLength(_ptr, Length);
        Quinoa_SetWindowSmoothness(_ptr, Smoothness);
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

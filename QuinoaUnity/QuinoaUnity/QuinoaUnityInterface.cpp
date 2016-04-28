#include "Quinoa.h"

#if _MSC_VER // this is defined when compiling with Visual Studio
#define EXPORT_API __declspec(dllexport) // Visual Studio needs annotating exported functions with this
#else
#define EXPORT_API // XCode does not need annotating exported functions, so define is empty
#endif

using DCSynths::Quinoa;

extern "C"
{
	EXPORT_API Quinoa *Quinoa_New(double sampleRate)
	{
		return new Quinoa(sampleRate);
	}

	EXPORT_API void Quinoa_Delete(Quinoa *q)
	{
		if (q) delete q;
	}

	EXPORT_API void Quinoa_SetSample(Quinoa *q, const float *sample, int numSamples)
	{
		if (q) q->SetSample(sample, numSamples);
	}

	EXPORT_API void Quinoa_ProcessBuffer(Quinoa *q, float *buffer, int numSamples, int numChannels)
	{
		if (q) q->ProcessBufferMultiplicative(buffer, numSamples, numChannels);
	}

	EXPORT_API void Quinoa_SetSpeed(Quinoa *q, double speed)
	{
		if (q) q->SetSpeed(speed);
	}

	EXPORT_API void Quinoa_SetPitch(Quinoa *q, double pitch)
	{
		if (q) q->SetPitch(pitch);
	}

	EXPORT_API void Quinoa_SetWindowSize(Quinoa *q, int windowSize)
	{
		if (q) q->SetWindowSize(windowSize);
	}

	EXPORT_API void Quinoa_SetStart(Quinoa *q, double start)
	{
		if (q) q->SetStart(start);
	}

	EXPORT_API void Quinoa_SetLength(Quinoa *q, double length)
	{
		if (q) q->SetLength(length);
	}

	EXPORT_API void Quinoa_SetWindowSmoothness(Quinoa *q, float smoothness)
	{
		if (q) q->SetWindowSmoothness(smoothness);
	}
}
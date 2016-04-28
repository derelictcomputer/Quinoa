/*
  ==============================================================================

    Quinoa.cpp
    Created: 22 Apr 2016 2:33:17pm
    Author:  charl_000

  ==============================================================================
*/

#include "Quinoa.h"
#include <math.h>

#define PI 3.1415927f

namespace DCSynths
{
	QuinoaGrain::QuinoaGrain(): _wPos(0)
	{
	}

	void QuinoaGrain::Init(size_t sStart, double wStart, double pitch, float windowSmooth, size_t sLen, const std::vector<float>& samples, const std::vector<float>& window1, const std::vector<float>& window2)
	{
		_wPos = 0;

		_grainTable.resize(window1.size());

		double wPos = 0;
		float interp, s1, s2, w1, w2, w3;
		size_t sIdx;

		for (size_t i = 0; i < _grainTable.size(); ++i)
		{
			interp = static_cast<float>(wStart + wPos);

			// get the samples
			sIdx = static_cast<size_t>(interp);
			interp -= sIdx;
			if (sIdx >= sLen) sIdx -= sLen;
			s1 = samples[sIdx + sStart];
			if (++sIdx >= sLen) sIdx = 0;
			s2 = samples[sIdx + sStart];

			// get the envelope level
			w1 = window1[i];
			if (i + 1 < window1.size()) w2 = window1[i + 1];
			else w2 = window1[0];
			w3 = w1 + (w2 - w1) * interp;
			w1 = window2[i];
			if (i + 1 < window2.size()) w2 = window2[i + 1];
			else w2 = window2[0];
			w2 = w1 + (w2 - w1) * interp;
			w1 = w3 + (w2 - w3) * windowSmooth;

			_grainTable[i] = (s1 + (s2 - s1) * interp) * w1;

			wPos += pitch;
		}
	}

	bool QuinoaGrain::HasSample() const
	{
		return _wPos < _grainTable.size();
	}

	float QuinoaGrain::GetSample()
	{
		if (HasSample()) return _grainTable[_wPos++];
		return 0;
	}

	Quinoa::Quinoa(double sampleRate): 
		_sampleRate(sampleRate), 
		_sPos(0), 
		_speed(0), 
		_pitch(0), 
		_start(0), 
		_length(0),
		_windowSmooth(0)
	{
		SetWindowSize(512);
	}

	Quinoa::~Quinoa()
	{
	}

	void Quinoa::Play(int note)
	{
	}

	void Quinoa::Stop()
	{
	}

	void Quinoa::ProcessBufferAdditive(float* buffer, int numSamples, int numChannels)
	{
		if (_samples.size() == 0)
		{
			return;
		}

		auto sLen = static_cast<size_t>(_length * _samples.size());
		auto sStart = static_cast<size_t>(_start * _samples.size());

		// make length authoritative so we don't go beyond the buffer
		if (sStart + sLen >= _samples.size())
		{
			sStart = _samples.size() - sLen;
		}

		float sample;

		for (int i = 0; i < numSamples; i += numChannels)
		{
			if (!_grain.HasSample())
			{
				_grain.Init(sStart, _sPos, _pitch, _windowSmooth, sLen, _samples, _window1, _window2);
			}

			sample = _grain.GetSample();

			_sPos += _speed;
			if (_sPos >= sLen) _sPos -= sLen;

			for (int j = 0; j < numChannels; ++j)
			{
				buffer[i + j] += sample;
			}
		}
	}

	void Quinoa::ProcessBufferMultiplicative(float* buffer, int numSamples, int numChannels)
	{
		if (_samples.size() == 0)
		{
			return;
		}

		auto sLen = static_cast<size_t>(_length * _samples.size());
		auto sStart = static_cast<size_t>(_start * _samples.size());

		// make length authoritative so we don't go beyond the buffer
		if (sStart + sLen >= _samples.size())
		{
			sStart = _samples.size() - sLen;
		}

		float sample;

		for (int i = 0; i < numSamples; i += numChannels)
		{
			if (!_grain.HasSample())
			{
				_grain.Init(sStart, _sPos, _pitch, _windowSmooth, sLen, _samples, _window1, _window2);
			}

			sample = _grain.GetSample();

			_sPos += _speed;
			if (_sPos >= sLen) _sPos -= sLen;

			for (int j = 0; j < numChannels; ++j)
			{
				buffer[i + j] *= sample;
			}
		}
	}

	void Quinoa::SetSample(const float* sample, int numSamples)
	{
		_sPos = 0;
		_samples.resize(numSamples);

		for (int i = 0; i < numSamples; ++i)
		{
			_samples[i] = sample[i];
		}
	}

	void Quinoa::SetSpeed(double speed)
	{
		_speed = speed;
	}

	void Quinoa::SetPitch(double pitch)
	{
		_pitch = pitch;
	}

	void Quinoa::SetWindowSize(int windowSize)
	{
		if (windowSize == _window1.size()) return;

		_window1.resize(windowSize);
		_window2.resize(windowSize);

		for (int i = 0; i < windowSize; ++i)
		{
			_window1[i] = 1.0f; // square window
			_window2[i] = 0.5f * (1.0f - cos((2.0f * PI * i) / (windowSize - 1))); // bell window
		}
	}

	void Quinoa::SetStart(double start)
	{
		_start = fmax(0.0, fmin(start, 1.0));
	}

	void Quinoa::SetLength(double length)
	{
		_length = fmax(0.0, fmin(length, 1.0));
	}

	void Quinoa::SetWindowSmoothness(float smoothness)
	{
		_windowSmooth = fmax(0.0f, fmin(smoothness, 1.0f));
	}
}

/*
  ==============================================================================

    Quinoa.h
    Created: 22 Apr 2016 2:33:17pm
    Author:  charl_000

  ==============================================================================
*/

#ifndef QUINOA_H_INCLUDED
#define QUINOA_H_INCLUDED
#include <vector>

namespace DCSynths
{
	enum class BufferProcessType
	{
		Overwrite = 0,
		Additive,
		Multiplicative
	};

	class QuinoaGrain
	{
	public:
		QuinoaGrain();

		void Init(size_t sStart, double wStart, double pitch, float windowSmooth, size_t sLen, const std::vector<float>& samples, const std::vector<float>& window1, const std::vector<float>& window2);

		bool HasSample() const;

		float GetSample();
	private:
		std::vector<float> _grainTable;
		size_t _wPos;
	};

	class Quinoa
	{
	public:
		explicit Quinoa(double sampleRate);

		~Quinoa();

		void Play(int note);

		void Stop();

		void ProcessBuffer(float *buffer, int numSamples, int numChannels, BufferProcessType processType);
		
		void SetSample(const float *sample, int numSamples, double sampleRate);

		void SetSpeed(double speed);

		void SetPitch(double pitch);

		void SetWindowSize(int windowSize);

		void SetStart(double start);

		void SetLength(double length);

		void SetWindowSmoothness(float smoothness);
	private:
		double _sampleRate;
		double _sampleRateRatio;
		std::vector<float> _samples;
		std::vector<float> _window1;
		std::vector<float> _window2;
		QuinoaGrain _grain;
		double _sPos;
		double _speed;
		double _pitch;
		double _start;
		double _length;
		float _windowSmooth;
	};
}

#endif  // QUINOA_H_INCLUDED

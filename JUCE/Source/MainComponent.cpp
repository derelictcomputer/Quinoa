/*
  ==============================================================================

    This file was auto-generated!

  ==============================================================================
*/

#ifndef MAINCOMPONENT_H_INCLUDED
#define MAINCOMPONENT_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"
#include "Quinoa.h"

//==============================================================================
/*
    This component lives inside our window, and this is where you should put all
    your controls and content.
*/
class MainContentComponent   : public AudioAppComponent, public Button::Listener
{
public:
	//==============================================================================
    MainContentComponent()
    {
		setAudioChannels(0, 2);

		_afMan.registerBasicFormats();

		addAndMakeVisible(_loadButton);
		_loadButton.setButtonText("Load");
		_loadButton.addListener(this);

		addAndMakeVisible(_speedSlider);
		_speedSlider.setValue(1.0);

		addAndMakeVisible(_pitchSlider);
		_pitchSlider.setValue(1.0);

		addAndMakeVisible(_lengthSlider);
		_lengthSlider.setRange(0.1, 1.0);
		_lengthSlider.setValue(1.0);

		addAndMakeVisible(_startSlider);
		_startSlider.setRange(0.0, 0.9);
		_startSlider.setValue(0.0);

		addAndMakeVisible(_smoothSlider);
		_smoothSlider.setRange(0.0, 1.0);
		_smoothSlider.setValue(1.0);

        setSize (800, 600);
    }

    ~MainContentComponent()
    {
        shutdownAudio();
    }

    //==============================================================================
    void prepareToPlay (int samplesPerBlockExpected, double sampleRate) override
    {
      if (_sampler == nullptr)
      {
       _sampler = new DCSynths::Quinoa(sampleRate);
      }
      _sampleRate = sampleRate;
    }

    void getNextAudioBlock (const AudioSourceChannelInfo& bufferToFill) override
    {
		if (!_fileSampleBuffer.hasBeenCleared() && _fileSampleBuffer.getNumSamples() > 0)
		{
			int numSamples = _fileSampleBuffer.getNumSamples();
      _sampler->SetSample(_fileSampleBuffer.getReadPointer(0), numSamples, _sampleRate);
			_sampler->Play(0);
			_fileSampleBuffer.clear();
		}

		_sampler->SetSpeed(_speedSlider.getValue());
		_sampler->SetPitch(_pitchSlider.getValue());
		_sampler->SetStart(_startSlider.getValue());
		_sampler->SetLength(_lengthSlider.getValue());
		_sampler->SetWindowSmoothness(_smoothSlider.getValue());

        bufferToFill.clearActiveBufferRegion();

		for (int cIdx = 0; cIdx < bufferToFill.buffer->getNumChannels(); ++cIdx)
		{
			auto channel = bufferToFill.buffer->getWritePointer(cIdx, bufferToFill.startSample);

			if (cIdx == 0)
      {
        _sampler->ProcessBuffer(channel, bufferToFill.numSamples, 1, DCSynths::BufferProcessType::Overwrite);
      }
			else
      {
        memcpy(channel, bufferToFill.buffer->getReadPointer(0, bufferToFill.startSample), bufferToFill.numSamples * sizeof(float));
      }
		}
		
    }

    void releaseResources() override
    {
        // This will be called when the audio device stops, or when it is being
        // restarted due to a setting change.

        // For more details, see the help for AudioProcessor::releaseResources()
    }

    //==============================================================================
    void paint (Graphics& g) override
    {
        // (Our component is opaque, so we must completely fill the background with a solid colour)
        g.fillAll (Colours::black);


        // You can add your drawing code here!
    }

    void resized() override
    {
		_loadButton.setBounds(0, 0, getWidth(), 40);
		_speedSlider.setBounds(0, 100, getWidth(), 40);
		_pitchSlider.setBounds(0, 200, getWidth(), 40);
		_startSlider.setBounds(0, 300, getWidth(), 40);
		_lengthSlider.setBounds(0, 400, getWidth(), 40);
		_smoothSlider.setBounds(0, 500, getWidth(), 40);
    }

	void buttonClicked(Button* button) override
	{
		if (button != &_loadButton)
		{
			return;
		}

		shutdownAudio();
		FileChooser fc("Select a wav file", File::nonexistent, "*.wav");

		if (fc.browseForFileToOpen())
		{
			const File f(fc.getResult());
			ScopedPointer<AudioFormatReader> afr(_afMan.createReaderFor(f));

			if (afr != nullptr)
			{
				int numSamples = afr->lengthInSamples / afr->numChannels;
				_fileSampleBuffer.setSize(1, numSamples);
				afr->read(&_fileSampleBuffer, 0, numSamples, 0, true, false);

				setAudioChannels(0, 2);
				
				//_sampler->SetSpeed(0.5);
			}
		}
	}

private:
	ScopedPointer<DCSynths::Quinoa> _sampler;
  double _sampleRate;
	TextButton _loadButton;
	Slider _speedSlider;
	Slider _pitchSlider;
	Slider _startSlider;
	Slider _lengthSlider;
	Slider _smoothSlider;
	AudioBuffer<float> _fileSampleBuffer;
	AudioFormatManager _afMan;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainContentComponent)
};


// (This function is called by the app startup code to create our main component)
Component* createMainContentComponent()     { return new MainContentComponent(); }


#endif  // MAINCOMPONENT_H_INCLUDED

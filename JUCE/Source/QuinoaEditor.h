/*
  ==============================================================================

    QuinoaEditor.h
    Created: 22 Apr 2016 2:34:14pm
    Author:  charl_000

  ==============================================================================
*/

#ifndef QUINOAEDITOR_H_INCLUDED
#define QUINOAEDITOR_H_INCLUDED

#include "../JuceLibraryCode/JuceHeader.h"

//==============================================================================
/*
*/
class QuinoaEditor    : public Component
{
public:
    QuinoaEditor();
    ~QuinoaEditor();

    void paint (Graphics&);
    void resized();

private:
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (QuinoaEditor)
};


#endif  // QUINOAEDITOR_H_INCLUDED

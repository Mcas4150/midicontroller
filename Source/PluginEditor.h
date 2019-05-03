/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class MidiControllerAudioProcessorEditor  : public AudioProcessorEditor,
                                            private Timer
{
public:
    MidiControllerAudioProcessorEditor (MidiControllerAudioProcessor&);
    ~MidiControllerAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    void timerCallback() override;
    void setNoteNumber (int noteNumber);
    void logMessage (const String& m);
    void addMessageToList (const MidiMessage& message);
    void addMessageToBuffer (const MidiMessage& message);
    
  

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    MidiControllerAudioProcessor& processor;
    TextButton bassDrumButton;
    Slider filter1CutoffSlider;
    Slider filter1ResonanceSlider;
    ComboBox filter1TypeMenu;
    TextEditor midiMessagesBox;
    
    
    int midiChannel = 8;
    double startTime;
    MidiBuffer midiBuffer;        // [1]
    double sampleRate = 44100.0;  // [2]
    int previousSampleNumber = 0;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MidiControllerAudioProcessorEditor)
};

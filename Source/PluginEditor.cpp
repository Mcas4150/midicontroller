/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
MidiControllerAudioProcessorEditor::MidiControllerAudioProcessorEditor (MidiControllerAudioProcessor& p)
    : AudioProcessorEditor (&p),
      processor (p),
      startTime (Time::getMillisecondCounterHiRes() * 0.001)
{
    addAndMakeVisible (bassDrumButton);
    bassDrumButton.setButtonText ("Bass Drum (36)");
    bassDrumButton.onClick = [this] { setNoteNumber (36); };
    
    addAndMakeVisible (filter1CutoffSlider);
    filter1CutoffSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    filter1CutoffSlider.setRange (0, 127, 1);
    filter1CutoffSlider.onValueChange = [this]
    {
        auto message = MidiMessage::controllerEvent (midiChannel, 50, (int) filter1CutoffSlider.getValue());
        message.setTimeStamp (Time::getMillisecondCounterHiRes() * 0.001 - startTime);
        addMessageToBuffer (message);
    };
    
    addAndMakeVisible (filter1ResonanceSlider);
    filter1ResonanceSlider.setSliderStyle(Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    filter1ResonanceSlider.setRange (0, 127, 1);
    filter1ResonanceSlider.onValueChange = [this]
    {
        auto message = MidiMessage::controllerEvent (midiChannel, 56, (int) filter1ResonanceSlider.getValue());
        message.setTimeStamp (Time::getMillisecondCounterHiRes() * 0.001 - startTime);
        addMessageToBuffer (message);
    };
    
    filter1TypeMenu.addItem("24dB LP", 1);
    filter1TypeMenu.addItem("12db LP", 2);
    filter1TypeMenu.addItem("24dB BP", 3);
    filter1TypeMenu.addItem("12dB BP", 4);
    filter1TypeMenu.addItem("12dB HP", 5);
    filter1TypeMenu.addItem("Sin(X)>LP", 6);
    filter1TypeMenu.addItem("Waveshaper", 7);
    filter1TypeMenu.addItem("Dual", 8);
    filter1TypeMenu.addItem("FM-Filter", 9);
    filter1TypeMenu.addItem("S&H->L12dB", 10);
//    filter1TypeMenu.setJustificationType(Justification::centred);
    addAndMakeVisible(&filter1TypeMenu);
    
    addAndMakeVisible (midiMessagesBox);
    midiMessagesBox.setMultiLine (true);
    midiMessagesBox.setReturnKeyStartsNewLine (true);
    midiMessagesBox.setReadOnly (true);
    midiMessagesBox.setScrollbarsShown (true);
    midiMessagesBox.setCaretVisible (false);
    midiMessagesBox.setPopupMenuEnabled (true);
    midiMessagesBox.setColour (TextEditor::backgroundColourId, Colour (0x32ffffff));
    midiMessagesBox.setColour (TextEditor::outlineColourId, Colour (0x1c000000));
    midiMessagesBox.setColour (TextEditor::shadowColourId, Colour (0x16000000));
    
    setSize (1000, 600);
    startTimer (1);
}

MidiControllerAudioProcessorEditor::~MidiControllerAudioProcessorEditor()
{
}

//==============================================================================
void MidiControllerAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::white);
    g.setFont (15.0f);
  
}

void MidiControllerAudioProcessorEditor::resized()
{
    auto halfWidth = getWidth() / 2;
    
    auto buttonsBounds = getLocalBounds().withWidth (halfWidth).reduced (10) /  4;
   
    bassDrumButton   .setBounds (buttonsBounds.getX(), 10,  buttonsBounds.getWidth(), 20);
    filter1CutoffSlider     .setBounds (buttonsBounds.getX() , 160, buttonsBounds.getWidth(), 80);
    filter1ResonanceSlider     .setBounds (buttonsBounds.getX(), 260, buttonsBounds.getWidth(), 80);
    filter1TypeMenu .setBounds (buttonsBounds.getX(), 340, buttonsBounds.getWidth(), 40);
    
    midiMessagesBox.setBounds (getLocalBounds().withWidth (halfWidth).withX (halfWidth).reduced (10));
}

void MidiControllerAudioProcessorEditor::setNoteNumber (int noteNumber)
{
    auto message = MidiMessage::noteOn (midiChannel, noteNumber, (uint8) 100);
    message.setTimeStamp (Time::getMillisecondCounterHiRes() * 0.001 - startTime);
    addMessageToList (message);
}

void MidiControllerAudioProcessorEditor::logMessage (const String& m)
{
    midiMessagesBox.moveCaretToEnd();
    midiMessagesBox.insertTextAtCaret (m + newLine);
}

void  MidiControllerAudioProcessorEditor::addMessageToList (const MidiMessage& message)
{
    auto time = message.getTimeStamp();
    
    auto hours   = ((int) (time / 3600.0)) % 24;
    auto minutes = ((int) (time / 60.0)) % 60;
    auto seconds = ((int) time) % 60;
    auto millis  = ((int) (time * 1000.0)) % 1000;
    
    auto timecode = String::formatted ("%02d:%02d:%02d.%03d",
                                       hours,
                                       minutes,
                                       seconds,
                                       millis);
    
    logMessage (timecode + "  -  " + message.getDescription());
}

void MidiControllerAudioProcessorEditor::addMessageToBuffer (const MidiMessage& message)
{
    auto timestamp = message.getTimeStamp();
    auto sampleNumber =  (int) (timestamp * sampleRate);
    midiBuffer.addEvent (message, sampleNumber);
}

void MidiControllerAudioProcessorEditor::timerCallback()
{
    auto currentTime = Time::getMillisecondCounterHiRes() * 0.001 - startTime;
    auto currentSampleNumber = (int) (currentTime * sampleRate); // [4]
    MidiBuffer::Iterator iterator (midiBuffer);
    MidiMessage message;
    int sampleNumber;
    while (iterator.getNextEvent (message, sampleNumber)) // [5]
    {
        if (sampleNumber > currentSampleNumber)           // [6]
            break;
        message.setTimeStamp (sampleNumber / sampleRate); // [7]
        addMessageToList (message);
    }
    midiBuffer.clear (previousSampleNumber, currentSampleNumber - previousSampleNumber); // [8]
    previousSampleNumber = currentSampleNumber;                                          // [9]
}
//
//void MidiControllerAudioProcessorEditor::handleMessage (int ccNumber, const)

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
    
    addAndMakeVisible (volumeSlider);
    volumeSlider.setRange (0, 127, 1);
    volumeSlider.onValueChange = [this]
    {
        auto message = MidiMessage::controllerEvent (midiChannel, 50, (int) volumeSlider.getValue());
        message.setTimeStamp (Time::getMillisecondCounterHiRes() * 0.001 - startTime);
        addMessageToList (message);
    };
    
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
    
    setSize (800, 300);
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
    
    auto buttonsBounds = getLocalBounds().withWidth (halfWidth).reduced (10);
   
    bassDrumButton   .setBounds (buttonsBounds.getX(), 10,  buttonsBounds.getWidth(), 20);
    volumeSlider     .setBounds (buttonsBounds.getX(), 220, buttonsBounds.getWidth(), 20);
    
    midiMessagesBox.setBounds (getLocalBounds().withWidth (halfWidth).withX (halfWidth).reduced (10));
}


static String getMidiMessageDescription (const MidiMessage& m)
{
    if (m.isNoteOn())           return "Note on "          + MidiMessage::getMidiNoteName (m.getNoteNumber(), true, true, 3);
    if (m.isNoteOff())          return "Note off "         + MidiMessage::getMidiNoteName (m.getNoteNumber(), true, true, 3);
    if (m.isProgramChange())    return "Program change "   + String (m.getProgramChangeNumber());
    if (m.isPitchWheel())       return "Pitch wheel "      + String (m.getPitchWheelValue());
    if (m.isAftertouch())       return "After touch "      + MidiMessage::getMidiNoteName (m.getNoteNumber(), true, true, 3) +  ": " + String (m.getAfterTouchValue());
    if (m.isChannelPressure())  return "Channel pressure " + String (m.getChannelPressureValue());
    if (m.isAllNotesOff())      return "All notes off";
    if (m.isAllSoundOff())      return "All sound off";
    if (m.isMetaEvent())        return "Meta event";
    
    if (m.isController())
    {
        String name (MidiMessage::getControllerName (m.getControllerNumber()));
        
        if (name.isEmpty())
            name = "[" + String (m.getControllerNumber()) + "]";
        
        return "Controller " + name + ": " + String (m.getControllerValue());
    }
    
    return String::toHexString (m.getRawData(), m.getRawDataSize());
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
    
    logMessage (timecode + "  -  " + getMidiMessageDescription (message));
    }

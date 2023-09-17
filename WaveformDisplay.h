/*
  ==============================================================================

    WaveformDisplay.h
    Created: 14 Mar 2020 3:50:16pm
    Author:  Admin

  ==============================================================================
*/
#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class WaveformDisplay : public juce::Component,
    public juce::ChangeListener
{
public:
    /** WaveformDisplay Constructor */
    WaveformDisplay (bool _deckFlag,
                    juce::AudioFormatManager& formatManager,
                    juce::AudioThumbnailCache& thumbCache);
    ~WaveformDisplay() override;

    /** Paint function - Draws waveform */
    void paint(juce::Graphics&) override;
    /** Set bounds of child components */
    void resized() override;
    /** Repaints the waveform */
    void changeListenerCallback(juce::ChangeBroadcaster* source) override;
    /** Loads audio url into the waveform*/
    void loadURL(juce::URL audioURL);
    /** Set the relative position of the playhead */
    void setPositionRelative(double pos);
private:
    bool deckFlag;
    bool fileLoaded;
    double position;
    juce::String fileName;
    juce::AudioThumbnail audioThumb;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(WaveformDisplay)
};

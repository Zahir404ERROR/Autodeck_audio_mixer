/*
  ==============================================================================

    AudioTrack.h
    Created: 21 Feb 2022 12:58:51am
    Author:  Admin

  ==============================================================================
*/


#pragma once
#include <JuceHeader.h>

class AudioTrack
{
public:
    /**Audio Track object*/
    AudioTrack(juce::File _trackFile);
    juce::File trackFile;
    juce::URL URL;
    juce::String title;
    juce::String length;

    /**Object Title Checker*/
    bool operator==(const juce::String& other) const;
};
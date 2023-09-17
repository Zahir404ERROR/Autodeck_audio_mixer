/*
  ==============================================================================

    AudioTrack.cpp
    Created: 21 Feb 2022 12:58:51am
    Author:  Admin

  ==============================================================================
*/

#include "AudioTrack.h"
#include <filesystem>
// Initialize constructor for AudioTrack object
AudioTrack::AudioTrack (juce::File _trackFile) : trackFile(_trackFile),
                        title(_trackFile.getFileNameWithoutExtension()),
                        URL(juce::URL{ _trackFile })
{
}

// Compares two titles
bool AudioTrack::operator== (const juce::String& other) const
{
    return title == other;
}
/*
  ==============================================================================

    DJAudioPlayer.h
    Created: 13 Mar 2020 4:22:22pm
    Author:  Admin

  ==============================================================================
*/

#pragma once
#include "../JuceLibraryCode/JuceHeader.h"

class DJAudioPlayer : public juce::AudioSource
{
public:
    DJAudioPlayer(juce::AudioFormatManager& _formatManager);
    ~DJAudioPlayer();

    void prepareToPlay(int samplesPerBlockExpected, double sampleRate) override;
    void getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill) override;
    void releaseResources() override;

    void loadURL(juce::URL audioURL);
    void play();
    void stop();
    void setPositionRelative(double pos);
    void setGain(double gain);
    void setSpeed(double ratio);
    double getPositionRelative();
    double getLengthInSeconds();
    void setWetLevel(float wetLevel);
    void setDryLevel(float dryLevel);
    void setDamping(float dampingAmount);
    void setRoomSize(float roomSize);

    
private:
    void setPosition(double posInSecs);
    juce::AudioFormatManager& formatManager;
    std::unique_ptr<juce::AudioFormatReaderSource> readerSource;
    juce::AudioTransportSource audioTransportSource;
    juce::ResamplingAudioSource audioResampleSource{ &audioTransportSource, false, 2 };
    juce::ReverbAudioSource audioReverbSource{ &audioResampleSource, false };
    juce::Reverb::Parameters audioReverbParameters;
};
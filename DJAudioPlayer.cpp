/*
==============================================================================

DJAudioPlayer.cpp
Created: 13 Mar 2020 4:22:22pm
Author:  Admin

==============================================================================
*/


#include "DJAudioPlayer.h"
DJAudioPlayer::DJAudioPlayer(juce::AudioFormatManager& _formatManager)
: formatManager(_formatManager)
{
	audioReverbParameters.wetLevel = 0;
	audioReverbParameters.dryLevel = 1.0;
	audioReverbParameters.damping = 0;
	audioReverbParameters.roomSize = 0;
	audioReverbSource.setParameters(audioReverbParameters);
}

DJAudioPlayer::~DJAudioPlayer()
{

}

void DJAudioPlayer::prepareToPlay(int samplesPerBlockExpected, double sampleRate)
{
	audioResampleSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
	audioTransportSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
	audioReverbSource.prepareToPlay(samplesPerBlockExpected, sampleRate);
}

void DJAudioPlayer::getNextAudioBlock(const juce::AudioSourceChannelInfo& bufferToFill)
{
	audioReverbSource.getNextAudioBlock(bufferToFill);
}

void DJAudioPlayer::releaseResources()
{
	audioResampleSource.releaseResources();
	audioTransportSource.releaseResources();
	audioReverbSource.releaseResources();
}

void DJAudioPlayer::loadURL(juce::URL audioURL)
{
	auto* reader = formatManager.createReaderFor(audioURL.createInputStream(false));
	if (reader != nullptr) // good file!
	{
		std::unique_ptr<juce::AudioFormatReaderSource> newSource(new juce::AudioFormatReaderSource(reader,
		true));
		audioTransportSource.setSource(newSource.get(), 0, nullptr, reader->sampleRate);
		readerSource.reset(newSource.release());
	}
}

void DJAudioPlayer::play()
{
	audioTransportSource.start();
}

void DJAudioPlayer::stop()
{
	audioTransportSource.stop();
}

double DJAudioPlayer::getPositionRelative()
{
	return audioTransportSource.getCurrentPosition() / audioTransportSource.getLengthInSeconds();
}

double DJAudioPlayer::getLengthInSeconds()
{
	return audioTransportSource.getLengthInSeconds();
}


void DJAudioPlayer::setPosition(double posInSecs)
{
	audioTransportSource.setPosition(posInSecs);
}

void DJAudioPlayer::setPositionRelative(double pos)
{
	if (pos < 0 || pos > 1.0)
	{
		std::cout << "DJAudioPlayer::setPositionRelative pos should be between 0 and 1" << std::endl;
	}
	else {
		double posInSecs = audioTransportSource.getLengthInSeconds() * pos;
		setPosition(posInSecs);
	}
}

void DJAudioPlayer::setGain(double gain)
{
	if (gain < 0 || gain > 1.0)
	{
		std::cout << "DJAudioPlayer::setGain gain should be between 0 and 1" << std::endl;
	}
	else {
		audioTransportSource.setGain(gain);
	}
}

void DJAudioPlayer::setSpeed(double ratio)
{
	if (ratio < 0.25 || ratio > 4.0)
	{
		std::cout << "DJAudioPlayer::setSpeed ratio should be between 0 and 100" << std::endl;
	}
	else {
		audioResampleSource.setResamplingRatio(ratio);
	}
}


void DJAudioPlayer::setRoomSize(float size)
{
	if (size >= 0 || size  <= 1.0)
	{
		audioReverbParameters.roomSize = size;
		audioReverbSource.setParameters(audioReverbParameters);
	}
}

void DJAudioPlayer::setDamping(float dampingAmt)
{
	if (dampingAmt >= 0 || dampingAmt <= 1.0)
	{
		audioReverbParameters.damping = dampingAmt;
		audioReverbSource.setParameters(audioReverbParameters);
	}
}

void DJAudioPlayer::setWetLevel(float wetLevel)
{
	if (wetLevel >= 0 || wetLevel <= 1.0)
	{
		audioReverbParameters.wetLevel = wetLevel;
		audioReverbSource.setParameters(audioReverbParameters);
	}
}

void DJAudioPlayer::setDryLevel(float dryLevel)
{
	if (dryLevel >= 0 || dryLevel <= 1.0)
	{
		audioReverbParameters.dryLevel = dryLevel;
		audioReverbSource.setParameters(audioReverbParameters);
	}
}




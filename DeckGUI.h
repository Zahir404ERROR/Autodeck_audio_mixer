/*
  ==============================================================================

	DeckGUI.h
	Created: 13 Mar 2020 6:44:48pm
	Author:  Admin

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "DJAudioPlayer.h"
#include "WaveformDisplay.h"

//Creating custom rotation slider by editing the LoookAndFeel
class OtherLookAndFeel : public juce::LookAndFeel_V4
{
public:
	bool deckFlag = true;

	OtherLookAndFeel()
	{
		setColour(juce::Slider::thumbColourId, juce::Colour(0xff3EDBF0));
	}

	void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos,
		const float rotaryStartAngle, const float rotaryEndAngle, juce::Slider&) override
	{
		auto radius = (float)juce::jmin(width / 2, height / 2) - 4.0f;
		auto centreX = (float)x + (float)width * 0.5f;
		auto centreY = (float)y + (float)height * 0.5f;
		auto rx = centreX - radius;
		auto ry = centreY - radius;
		auto rw = radius * 2.0f;
		auto angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

		juce::Path p;
		auto pointerLength = radius * 0.8f;
		auto pointerThickness = 5.0f;
		g.setColour(juce::Colours::orange);
		p.addEllipse(-pointerThickness * 0.5f - 7, -radius-13, width / 3.5-4, width / 3);
		g.addTransform(juce::AffineTransform::rotation(angle).translated(centreX, centreY));
		// pointer 
		g.setColour(findColour(juce::ResizableWindow::backgroundColourId));
		g.fillPath(p);
	}
};

//==============================================================================
class DeckGUI : public juce::Component,
				public juce::Button::Listener,
				public juce::Slider::Listener,
				public juce::FileDragAndDropTarget,
				public juce::Timer
{
public:
	/* DeckGUI Constructor*/
	DeckGUI(bool _deckFlag,
			DJAudioPlayer* player,
			juce::AudioFormatManager& formatManager,
			juce::AudioThumbnailCache& thumbCache);

	~DeckGUI() override;

	/* Draws DeckGUI Components, backgrounds and outlines */
	void paint(juce::Graphics&) override;
	/* Draws over another paint function - used for waveform slider*/
	void paintOverChildren(juce::Graphics&) override;
	void resized() override;
	/**Implement Button::Listener*/
	void buttonClicked(juce::Button* button) override;
	/**Implement Slider::Listener */
	void sliderValueChanged(juce::Slider* slider) override;
	/** Allows Files to be dragged over the component*/
	bool isInterestedInFileDrag(const juce::StringArray& file) override;
	/**loads dragged files onto the deck*/
	void filesDropped(const juce::StringArray& file, int a, int b) override;
	/**Changes waveform based on the relative position of the audio file*/
	void timerCallback() override;


private:
	int id;
	bool deckFlag;
	OtherLookAndFeel otherLookAndFeel; 
	
	//Initialize Buttons
	juce::TextButton playButton{ "Play" };
	juce::TextButton stopButton{ "Pause" };
	juce::TextButton loadButton{ "Load" };
	
	//Initialize Slider
	juce::Slider volSlider;
	juce::Slider posSlider;
	juce::Slider speedSlider;
	juce::Slider wetSlider;
	juce::Slider drySlider;
	juce::Slider dampingSlider;
	juce::Slider roomSlider;
	juce::Slider waveformSlider;

	juce::Label volLabel;
	juce::Label speedLabel;
	juce::Label wetLabel;
	juce::Label dryLabel;
	juce::Label dampingLabel;
	juce::Label roomLabel;
	juce::Label posLabel;

	double position;
	double discAngle=0;
	int diskPositionFlag= 0;
	DJAudioPlayer* player;
	WaveformDisplay waveformDisplay;
	/* Loads audio track into a player from the deck*/
	void loadFile(juce::URL audioURL);
	friend class PlaylistComponent;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(DeckGUI)
};



/*
  ==============================================================================

	DeckGUI.cpp
	Created: 13 Mar 2020 6:44:48pm
	Author:  Admin

  ==============================================================================
*/


#include <JuceHeader.h>
#include "DeckGUI.h"

//==============================================================================
DeckGUI::DeckGUI (bool _deckFlag,
				  DJAudioPlayer* _player,
				  juce::AudioFormatManager& formatManager,
				  juce::AudioThumbnailCache& thumbCache
				 ): deckFlag(_deckFlag),
					player(_player),
					waveformDisplay(deckFlag, formatManager, thumbCache)
{
	setLookAndFeel(&otherLookAndFeel);

	// Here we add and make the Deck components visible
	// - Buttons
	addAndMakeVisible(playButton);
	addAndMakeVisible(stopButton);
	addAndMakeVisible(loadButton);
	// - Sliders
	addAndMakeVisible(volSlider);
	addAndMakeVisible(speedSlider);
	addAndMakeVisible(wetSlider);
	addAndMakeVisible(drySlider);
	addAndMakeVisible(dampingSlider);
	addAndMakeVisible(roomSlider);
	// - Labels
	addAndMakeVisible(volLabel);	
	addAndMakeVisible(speedLabel);
	addAndMakeVisible(posLabel);
	// - Waveform Display
	addAndMakeVisible(waveformDisplay);
	addAndMakeVisible(waveformSlider);
	addAndMakeVisible(posSlider);


	// Here we add listeners to Buttons and Sliders
	// - Buttons
	playButton.addListener(this);
	stopButton.addListener(this);
	loadButton.addListener(this);
	// - Sliders
	volSlider.addListener(this);
	speedSlider.addListener(this);
	wetSlider.addListener(this);
	drySlider.addListener(this);
	dampingSlider.addListener(this);
	roomSlider.addListener(this);
	posSlider.addListener(this);
	waveformSlider.addListener(this);



	// Default Controller Values
	double defaultVolValue = 50; // Default Volume Value 
	double defaultSpeedValue = 1.0; // Default Speed Value 

	// Volume Controller
	// - Volume slider  
	volSlider.setRange(0, 100); // Range
	volSlider.setNumDecimalPlacesToDisplay(0); // Round to nearest integer
	volSlider.setTextBoxStyle ( juce::Slider::TextBoxBelow,
								false,
								50,
								volSlider.getTextBoxHeight() ); // Volume Textbox
	volSlider.setValue(defaultVolValue); // Set Volume Value
	volSlider.setSliderStyle(Slider::SliderStyle::LinearBarVertical);
	if (deckFlag == true)
	{
		volSlider.setColour(0x1001310, juce::Colour(0xff3EDBF0));
		wetSlider.setColour(0x1001310, juce::Colour(0xff3EDBF0));
		drySlider.setColour(0x1001310, juce::Colour(0xff3EDBF0));
		roomSlider.setColour(0x1001310, juce::Colour(0xff3EDBF0));
		dampingSlider.setColour(0x1001310, juce::Colour(0xff3EDBF0));
	}
	else
	{
		volSlider.setColour(0x1001310, juce::Colours::red);
		wetSlider.setColour(0x1001310, juce::Colours::red);
		drySlider.setColour(0x1001310, juce::Colours::red);
		roomSlider.setColour(0x1001310, juce::Colours::red);
		dampingSlider.setColour(0x1001310, juce::Colours::red);
	}

	// - Volume label
	volLabel.setText("Volume", juce::dontSendNotification);
	volLabel.setJustificationType(juce::Justification::centred);
	volLabel.attachToComponent(&volSlider, false);


	// Speed Controller
	// - Speed Slider
	speedSlider.setRange(0.25, 4.0); 
	speedSlider.setNumDecimalPlacesToDisplay(2);
	speedSlider.setTextBoxStyle(juce::Slider::TextBoxBelow,
		false,
		50,
		speedSlider.getTextBoxHeight() );
	speedSlider.setMouseDragSensitivity(80);
	speedSlider.setValue(defaultSpeedValue);
	speedSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	speedSlider.setSkewFactorFromMidPoint(defaultSpeedValue);
	// Speed label
	speedLabel.setText("Speed :", juce::dontSendNotification);
	speedLabel.setJustificationType(juce::Justification::centred);
	speedLabel.attachToComponent(&speedSlider, false);


	// Reverb Controller
	// - Wet Slider
	wetSlider.setRange(0, 1.0);
	wetSlider.setNumDecimalPlacesToDisplay(2);
	wetSlider.setTextBoxStyle(juce::Slider::TextBoxBelow,
		false,
		50,
		wetSlider.getTextBoxHeight());
	wetSlider.setMouseDragSensitivity(80);
	wetSlider.setValue(0);
	wetSlider.setSliderStyle(Slider::SliderStyle::LinearBarVertical);
	// Wet label
	wetLabel.setText("Wet :", juce::dontSendNotification);
	wetLabel.setJustificationType(juce::Justification::centred);
	wetLabel.attachToComponent(&wetSlider, false);
	// - Dry Slider
	drySlider.setRange(0, 1.0);
	drySlider.setNumDecimalPlacesToDisplay(2);
	drySlider.setTextBoxStyle(juce::Slider::TextBoxBelow,
		false,
		50,
		drySlider.getTextBoxHeight());
	drySlider.setMouseDragSensitivity(80);
	drySlider.setValue(1);
	drySlider.setSliderStyle(Slider::SliderStyle::LinearBarVertical);
	// Dry label
	dryLabel.setText("Dry :", juce::dontSendNotification);
	dryLabel.setJustificationType(juce::Justification::centred);
	dryLabel.attachToComponent(&drySlider, false);
	// - Damping Slider
	dampingSlider.setRange(0, 1.0);
	dampingSlider.setNumDecimalPlacesToDisplay(2);
	dampingSlider.setTextBoxStyle(juce::Slider::TextBoxBelow,
		false,
		50,
		dampingSlider.getTextBoxHeight());
	dampingSlider.setMouseDragSensitivity(80);
	dampingSlider.setSliderStyle(Slider::SliderStyle::LinearBarVertical);
	// Damping label
	dampingLabel.setText("Damp :", juce::dontSendNotification);
	dampingLabel.setJustificationType(juce::Justification::centred);
	dampingLabel.attachToComponent(&dampingSlider, false);
	// - Room Slider
	roomSlider.setRange(0, 1.0);
	roomSlider.setNumDecimalPlacesToDisplay(2);
	roomSlider.setTextBoxStyle(juce::Slider::TextBoxBelow,
		false,
		50,
		roomSlider.getTextBoxHeight());
	roomSlider.setMouseDragSensitivity(80);
	roomSlider.setValue(0);
	roomSlider.setSliderStyle(Slider::SliderStyle::LinearBarVertical);
	// Room label
	roomLabel.setText("Room :", juce::dontSendNotification);
	roomLabel.setJustificationType(juce::Justification::centred);
	roomLabel.attachToComponent(&roomSlider, false);

	// Position Controller
	// - Position Slider
	posSlider.setRange(0.0, 1.0);
	posSlider.setSliderStyle(Slider::SliderStyle::LinearBar);
	posSlider.setNumDecimalPlacesToDisplay(2);
	posSlider.setTextBoxStyle(juce::Slider::TextBoxLeft,
		false,
		50,
		posSlider.getTextBoxHeight()
	);
	// - Position label
	posLabel.attachToComponent(&posSlider, true);

	waveformSlider.setRange(0.0, 1.0);
	waveformSlider.setSliderStyle(Slider::SliderStyle::LinearBar);
	waveformSlider.setNumDecimalPlacesToDisplay(2);
	waveformSlider.setTextBoxStyle(juce::Slider::NoTextBox,
		true,
		50,
		waveformSlider.getTextBoxHeight()
	);
	waveformSlider.setColour(0x1001310, juce::Colour::fromRGBA(0, 0, 0, 0));

	startTimerHz(75);
}

DeckGUI::~DeckGUI()
{
	setLookAndFeel(nullptr);
	stopTimer();
}

void DeckGUI::paintOverChildren(juce::Graphics& g) 
{
	if (deckFlag ==true)
	{
		//Wavedform position slider overlay Left (Blue)
		g.setColour(juce::Colour::fromRGBA(0, 150, 128, 150));
		juce::Rectangle<int> area(0, 0, position * getWidth(), getHeight() / 9);
		g.fillRect(area);
	}
	else
	{
		//Wavedform position slider overlay Right (Red)
		g.setColour(juce::Colour::fromRGBA(250, 0, 8, 150));
		juce::Rectangle<int> area(0, 0, position * getWidth(), getHeight() / 9);
		g.fillRect(area);
	}
}

void DeckGUI::paint(juce::Graphics& g)
{
	/* This demo code just fills the component's background and
	   draws some placeholder text to get you started.

	   You should replace everything in this method with your own
	   drawing code..
	*/

	g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));   // clear the background
	
	

	g.setColour(juce::Colours::grey);
	g.drawRect(getLocalBounds(), 1);   // draw an outline around the component
	juce::Path p;
	if (deckFlag ==true)
	{
		//Left Deck (Blue)
		//Speed Rotary Slider Background
		g.setColour(juce::Colour(0xff3EDBF0));
		g.fillEllipse(7, 1.517 * getHeight() / 4 + 95, getWidth() / 6.95, getWidth() / 6.95);
		//Rotation Disc Background and outline
		g.setColour(juce::Colour(0xff3EDBF0));
		g.fillEllipse(getWidth() / 2 - 100, getHeight() / 2 - 200, 200, 200);
		g.drawEllipse(getWidth() / 2 - 100, getHeight() / 2 - 200, 200, 200, 1.0f);
		//Rotation Disc inner circle and pointer
		g.setColour(juce::Colour(0xff051367));
		g.fillEllipse(getWidth() / 2.145, getHeight() / 3.195, 35, 35);
		// Pointer
		g.fillPath(p);
	}
	else
	{
		//Right Deck (Red)
		//Speed Rotary Slider Background
		g.setColour(juce::Colours::red);
		g.fillEllipse(7, 1.517 * getHeight() / 4 + 95, getWidth() / 6.95, getWidth() / 6.95);
		//Rotation Disc Background and outline
		g.setColour(juce::Colours::red);
		g.fillEllipse(getWidth() / 2 - 100, getHeight() / 2 - 200, 200, 200);
		g.drawEllipse(getWidth() / 2 - 100, getHeight() / 2 - 200, 200, 200, 1.0f);
		//Rotation Disc inner circle and pointer
		g.setColour(juce::Colour(0xff5F0000));
		g.fillEllipse(getWidth() / 2.145 , getHeight() / 3.195 , 35, 35);
		// Pointer
		g.fillPath(p);
	}

	auto pointerLength = 100;
	auto pointerThickness = 3.0f;
	p.addRectangle(-pointerThickness * 0.5f, -100, pointerThickness, pointerLength);
	//p.applyTransform(juce::AffineTransform::rotation(discAngle).translated());
	p.applyTransform(juce::AffineTransform::rotation(discAngle));
	p.applyTransform(juce::AffineTransform::translation(getWidth() / 2, getHeight() / 2 - 100));

	// pointer
	g.fillPath(p);
}

//Deck Layout Set
void DeckGUI::resized()
{
	auto width = getWidth();
	auto height = getHeight();
	auto buttonsXPos = width / 3;
	auto buttonsYPos = 2.2 * height / 4;
	auto buttonsWidth = width / 7;
	auto buttonsHeight = height / 12;
	auto slidersXPos = 75;
	auto slidersYPos = 1.2 * height / 4;
	auto slidersWidth= width - 75;
	auto slidersHeight = height / 8;

	waveformDisplay.setBounds(0, 0, width, height/9);
	waveformSlider.setBounds(0, 0, width, height / 9);

	volSlider.setBounds(7, 1.4*height / 9, width/9, 3 * height / 9);
	speedSlider.setBounds(5, slidersYPos + 140, slidersWidth/5.7, 1.3*slidersHeight);
	drySlider.setBounds(slidersXPos + 372, slidersYPos - 93, slidersWidth/9, 1.95 * slidersHeight);
	wetSlider.setBounds(slidersXPos + 320, slidersYPos - 93, slidersWidth/9, 1.95 * slidersHeight);
	dampingSlider.setBounds(slidersXPos + 372, slidersYPos + 85, slidersWidth/9, 1.95 * slidersHeight);
	roomSlider.setBounds(slidersXPos + 320, slidersYPos + 85, slidersWidth/9, 1.95*slidersHeight);
	playButton.setBounds((0.5*width) / 2, buttonsYPos, buttonsWidth, buttonsHeight);
	stopButton.setBounds((0.86 * width) / 2, buttonsYPos, buttonsWidth, buttonsHeight);
	loadButton.setBounds((1.22 * width) / 2, buttonsYPos, buttonsWidth, buttonsHeight);
}

//Button Clicked functions
void DeckGUI::buttonClicked(juce::Button* button)
{
	// Play Button
	if (button == &playButton)
	{
		std::cout << "Play button was clicked " << std::endl;
		player->play();
		diskPositionFlag = 1;
	}
			repaint();

	// Stop Button
	if (button == &stopButton)
	{
		std::cout << "Stop button was clicked " << std::endl;
		player->stop();
		diskPositionFlag = 2;
	}
	
	// Load Button
	if (button == &loadButton)
	{
		juce::FileChooser chooser{ "Select a file" };
		if (chooser.browseForFileToOpen())
		{
			loadFile(juce::URL{ chooser.getResult() });
		}
	}
}

// Slider 
void DeckGUI::sliderValueChanged(juce::Slider* slider)
{
	if (slider == &volSlider)
	{
		player->setGain(slider->getValue()/100);
	}
	
	if (slider == &drySlider)
	{
		player->setDryLevel(slider->getValue());
	}

	if (slider == &wetSlider)
	{
		player->setWetLevel(slider->getValue());
	}

	if (slider == &dampingSlider)
	{
		player->setDamping(slider->getValue());
	}

	if (slider == &roomSlider)
	{
		player->setRoomSize(slider->getValue());
	}

	if (slider == &posSlider)
	{
		player->setPositionRelative(slider->getValue());
	}
	
	if (slider == &speedSlider)
	{
		player->setSpeed(slider->getValue());
	}

	if (slider == &waveformSlider)
	{
		player->setPositionRelative(slider->getValue());
	}
}

bool DeckGUI::isInterestedInFileDrag(const juce::StringArray& files)
{
	return true;
}

//Loads dragged audio files into the deck
void DeckGUI::filesDropped(const juce::StringArray& file, int a, int b)
{
	if (file.size() == 1)
	{
		//We are only loading an individual file so the file is in file[0]
		loadFile(juce::URL{ juce::File{file[0]} });
	}
}

// Loads audio file into audio player and waveform
void DeckGUI::loadFile(juce::URL audioURL)
{
	player->loadURL(audioURL);
	waveformDisplay.loadURL(audioURL);
}

void DeckGUI::timerCallback()
{
	if (player->getPositionRelative() > 0)
	{
		waveformDisplay.setPositionRelative(player->getPositionRelative());
	}
	if (player->getPositionRelative() > 0 && diskPositionFlag==1) 
	{
		discAngle = discAngle + (2.5 * (3.14159265358979323846264338327950288419716939937510582097494459230781640 / 180));
		repaint();
	}
	else if (diskPositionFlag == 2) {
		repaint();
	}
	else if (diskPositionFlag == 0) {
		discAngle = 0;
		repaint();
	}
	else {
		discAngle = 0;
		repaint();
	}

	position = player->getPositionRelative();
	repaint();
}


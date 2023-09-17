/*
  ==============================================================================

    PlaylistComponent.cpp
    Created: 9 Feb 2022 10:45:42pm
    Author:  Admin

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PlaylistComponent.h"

//==============================================================================
PlaylistComponent::PlaylistComponent (DeckGUI* _leftDeck,
                                      DeckGUI* _rightDeck,
                                      DJAudioPlayer* _metadataParsingPlayer
                                      ) : leftGUIDeck(_leftDeck),
                                      rightGUIDeck(_rightDeck),
                                      metadataParsingPlayer(_metadataParsingPlayer)
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

    // add components
    addAndMakeVisible(searchBar);
    addAndMakeVisible(tracksPlaylist);
    addAndMakeVisible(addTracksBtn);
    addAndMakeVisible(addToLPlayerBtn);
    addAndMakeVisible(addToRlayerBtn);

    // attach listeners
    searchBar.addListener(this);
    addTracksBtn.addListener(this);
    addToLPlayerBtn.addListener(this);
    addToRlayerBtn.addListener(this);

    // Search Bar 
    // search bar return key listener
    searchBar.onReturnKey = [this] { searchPlaylist(searchBar.getText()); };
    // search bar text when bar is empty
    searchBar.setTextToShowWhenEmpty("Search for your Track here (Press Enter to Search)",
                                    juce::Colours::orange);

    // Setup table model and load Playlist into table 
    tracksPlaylist.getHeader().addColumn("Track Name", 1, 1);
    tracksPlaylist.getHeader().addColumn("Track Length", 2, 1);
    tracksPlaylist.getHeader().addColumn("Delete", 3, 1);
    tracksPlaylist.setModel(this);
    loadPlaylist();
}

PlaylistComponent::~PlaylistComponent()
{
    // Saves Playlist
    savePlaylist();
}

//Paint function for drawing components
void PlaylistComponent::paint(juce::Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll(getLookAndFeel().findColour(juce::ResizableWindow::backgroundColourId));   // clear the background
    
    // draw an outline around the component
    g.setColour(juce::Colours::darkgrey);
    g.drawRect(getLocalBounds(), 1);   

    g.setColour(juce::Colours::white);
    g.setFont(14.0f);

}

void PlaylistComponent::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

    //Define bounds of each child component
    searchBar.setBounds(0, 0 * getHeight() / 16, getWidth(), getHeight() / 16 + 14);
    tracksPlaylist.setBounds(0, (1 * getHeight() / 16)+14, getWidth()-(getWidth() / 7), 13 * getHeight() / 16+14);
    addTracksBtn.setBounds(getWidth() - (getWidth() / 7), getHeight() / 15 + 14, getWidth() / 7, getHeight() / 9);
    addToLPlayerBtn.setBounds(getWidth() - (getWidth() / 7), getHeight() / 15 + 44, getWidth() / 7, getHeight() / 9);
    addToRlayerBtn.setBounds(getWidth() - (getWidth() / 7), getHeight() / 15 + 74, getWidth() / 7, getHeight() / 9);
    //set Table column width of each column of the table 
    tracksPlaylist.getHeader().setColumnWidth(1, 14 * getWidth() / 20+19);
    tracksPlaylist.getHeader().setColumnWidth(2, 2 * getWidth() / 20-10);
    tracksPlaylist.getHeader().setColumnWidth(3,  getWidth() / 20);
}

// This method is where the background of rows are painted 
void PlaylistComponent::paintRowBackground (juce::Graphics& g,
                                            int rowNumber,
                                            int width,
                                            int height,
                                            bool rowIsSelected)
{    
    if (rowIsSelected)
    {
        // When row is selected change background color to orange
        g.fillAll(juce::Colours::orange);
    }
    else
    {
        // If not selected the background is dark grey
        g.fillAll(juce::Colours::darkgrey);
    }
}

// Draws and defines table cells for each column
void PlaylistComponent::paintCell (juce::Graphics& g,
                                   int rowNumber,
                                   int columnId,
                                   int width,
                                   int height,
                                   bool rowIsSelected)
{
    if (rowNumber < getNumRows())
    {
        // Draws cells of the first column in the playlist
        if (columnId == 1)
        {
            g.drawText(audioTracks[rowNumber].title,
                2,
                0,
                width - 4,
                height,
                juce::Justification::centredLeft,
                true
            );
        }
        // Draws cells of the second column in the playlist
        if (columnId == 2)
        {
            g.drawText(audioTracks[rowNumber].length,
                2,
                0,
                width - 4,
                height,
                juce::Justification::centred,
                true
            );
        }
    }
}
juce::Component* PlaylistComponent::refreshComponentForCell (int rowNumber,
                                                             int columnId,
                                                             bool flagRowSelected,
                                                             Component* existingComponentToUpdate)
{
    // Draws cells of the third column in the playlist
    if (columnId == 3)
    {
        if (existingComponentToUpdate == nullptr)
        {
            // Draws text button for removing tracks from the playlist
            juce::TextButton* deleteTrackBtn = new juce::TextButton{ "x" };
            juce::String trackID{ std::to_string(rowNumber) };
            // Sets ID for each button within the playlist
            deleteTrackBtn->setComponentID(trackID);

            // Listeners for genrated buttons
            deleteTrackBtn->addListener(this);
            existingComponentToUpdate = deleteTrackBtn;
        }
    }
    return existingComponentToUpdate;
}

// Gets the number of tracks within the playlist
int PlaylistComponent::getNumRows()
{
    return audioTracks.size();
}

// Buttons parsing method
void PlaylistComponent::buttonClicked(juce::Button* button)
{
    if (button == &addToLPlayerBtn)
    {
        // Loads file into left deck
        loadIntoDeck(leftGUIDeck);
    }
    else if (button == &addToRlayerBtn)
    {
        // Loads file into right deck
        loadIntoDeck(rightGUIDeck);
    }
    else if (button == &addTracksBtn)
    {
        // adds tracks button
        addTracksIntoPlaylist();
        tracksPlaylist.updateContent();
    }
    else
    {
        // Deletes track from the playlist
        int trackID = std::stoi(button->getComponentID().toStdString());
        deleteTrack(trackID);
        tracksPlaylist.updateContent();
    }
}

// Loads file into deck
void PlaylistComponent::loadIntoDeck(DeckGUI* deckGUI)
{
    int selectedRow { tracksPlaylist.getSelectedRow() };
    if (selectedRow != -1)
    {
        deckGUI->loadFile(audioTracks[selectedRow].URL);
        deckGUI->diskPositionFlag = 0;
    }
    else
    {
        // An warning alert to user when no file is being passed through
        juce::AlertWindow::showMessageBox (juce::AlertWindow::AlertIconType::WarningIcon,
                                           "WARNING",
                                           "Please select a track to add to deck",
                                           "OK",
                                            nullptr);
    }
}

// Imports tracks into playlists
void PlaylistComponent::addTracksIntoPlaylist()
{
    juce::FileChooser fileChooser{ "Choose Track Files" };
    if (fileChooser.browseForMultipleFilesToOpen())
    {
        for (const juce::File& trackFile : fileChooser.getResults())
        {
            juce::String fileNameWithoutExtension
            { 
                trackFile.getFileNameWithoutExtension() 
            };
            
            AudioTrack newTrackObj{ trackFile };
            juce::URL trackURL{ trackFile };
            newTrackObj.length = getTrackLength(trackURL);
            audioTracks.push_back(newTrackObj);
        }
    }
}

// Deletes track from a playlist
void PlaylistComponent::deleteTrack(int id)
{
    audioTracks.erase(audioTracks.begin() + id);
}

// Finds audio track length
juce::String PlaylistComponent::getTrackLength(juce::URL audioTrackURL)
{
    // Loads track into the metadata parsing player
    metadataParsingPlayer->loadURL(audioTrackURL);
    // Gets track length in seconds
    double trackLengthSec{ metadataParsingPlayer->getLengthInSeconds() };
    // Converts track length from seconds to minutes
    juce::String trackLengthMin { convertSecToMin(trackLengthSec) };
    return trackLengthMin;
}

// Converts seconds to minutes
juce::String PlaylistComponent::convertSecToMin(double trackLengthSec)
{
    // Round seconds to nearest integer value for computation
    int trackLengthSecRounded { int(std::round(trackLengthSec)) };
    // Converts sec to min and save as string
    juce::String finalTrackLengthMin{ std::to_string(trackLengthSecRounded / 60) };
    // Find Remaing track seconds and save as string
    juce::String finalTrackLengthSec{ std::to_string(trackLengthSecRounded % 60) };

    // Return track length timestamp
    return juce::String{ finalTrackLengthMin + ":" + finalTrackLengthSec };
}

// Searches for track trough the playlist
void PlaylistComponent::searchPlaylist(juce::String searchInput)
{
    if (searchInput != "")
    {
        //Search through playlist with search text 
        int trackRowNumber = findTrack(searchInput);
        //Select the row which maches the search text
        tracksPlaylist.selectRow(trackRowNumber);
    }
    else
    {
        //If search text is not within the playlist it deselects all rows
        tracksPlaylist.deselectAllRows();
    }
}

int PlaylistComponent::findTrack(juce::String searchInput)
{
    // finds index where track title contains searchText
    auto iterator = find_if (audioTracks.begin(), audioTracks.end(),
                            [&searchInput](const AudioTrack& audioTrackobj)
                            { return audioTrackobj.title.contains(searchInput); });
    int index;
    if (iterator != audioTracks.end())
    {
        index = std::distance(audioTracks.begin(), iterator);
    }
    return index;
}

// Save playlist tracks into a CSV file
void PlaylistComponent::savePlaylist()
{
    // Open file in write mode
    std::ofstream playlistCSV ("playlistLibrary.csv");

    // Save track path and length into the CSV file
    for (AudioTrack& audioTrack : audioTracks)
    {
        playlistCSV << audioTrack.trackFile.getFullPathName() << "," << audioTrack.length << std::endl;
    }
}

// Loads playlist from CSV file
void PlaylistComponent::loadPlaylist()
{
    // Open file in read mode
    std::ifstream playlistCSV("playlistLibrary.csv");
    std::string trackPath;
    std::string trackLength;

    if (playlistCSV.is_open())
    {
        // Reads each line of CSV file and pushes the tracks objects into the audioTracks vector
        while (getline(playlistCSV, trackPath, ',')) {
            juce::File trackFile{ trackPath };
            AudioTrack newTrackObj{ trackFile };

            getline(playlistCSV, trackLength);
            newTrackObj.length = trackLength;
            audioTracks.push_back(newTrackObj);
        }
    }
    playlistCSV.close();
}


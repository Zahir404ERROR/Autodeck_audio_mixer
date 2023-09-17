/*
  ==============================================================================

    PlaylistComponent.h
    Created: 9 Feb 2022 10:45:42pm
    Author:  Admin

  ==============================================================================
*/


#pragma once

#include <JuceHeader.h>
#include <vector>
#include <algorithm>
#include <fstream>
#include "AudioTrack.h"
#include "DeckGUI.h"
#include "DJAudioPlayer.h"

//==============================================================================
/*
*/
class PlaylistComponent : public juce::Component,
                          public juce::Button::Listener,
                          public juce::TableListBoxModel,
                          public juce::TextEditor::Listener
{
public:
    /** PlaylistComponent Constructor */
    PlaylistComponent ( DeckGUI* _leftDeck,
                        DeckGUI* _rightDeck,
                        DJAudioPlayer* _playerForParsingMetaData);
    ~PlaylistComponent() override;
    /** Draws background and outline */
    void paint(juce::Graphics&) override;
    /** Define bounds of child components */
    void resized() override;
    /** Gets playlist size */
    int getNumRows() override;
    /** Paints background of rows */
    void paintRowBackground(juce::Graphics& g,
                            int rowNumber,
                            int width,
                            int height,
                            bool rowIsSelected
                            ) override;
    /** Paints table cells for each column */
    void paintCell(juce::Graphics& g,
                    int rowNumber,
                    int columnId,
                    int width,
                    int height,
                    bool rowIsSelected
                    ) override;
    /** Generating delete track buttons */
    Component* refreshComponentForCell(int rowNumber,
                                       int columnId,
                                       bool flagRowSelected,
                                       Component* existingComponentToUpdate) override;
    /** Button clicked parsing system */
    void buttonClicked(juce::Button* button) override;

private:
    std::vector<AudioTrack> audioTracks;
    juce::TextEditor searchBar;
    juce::TableListBox tracksPlaylist;
    juce::TextButton addTracksBtn{ "ADD TRACKS" };
    juce::TextButton addToLPlayerBtn{ "ADD TO LEFT DECK" };
    juce::TextButton addToRlayerBtn{ "ADD TO RIGHT DECK" };
    juce::String getTrackLength(juce::URL audioURL);
    juce::String convertSecToMin(double seconds);

    DeckGUI* leftGUIDeck;
    DeckGUI* rightGUIDeck;
    DJAudioPlayer* metadataParsingPlayer;

    /** Adds tracks into playlist */
    void addTracksIntoPlaylist();
    /** Loads playlist from CSV*/
    void loadPlaylist();
    /** Searches playlist for a track */
    void searchPlaylist(juce::String searchInput);
    /** Deletes a track from the playlist */
    void deleteTrack(int trackID);
    /** Saves playlist as a CSV file */
    void savePlaylist();
    /** Loads tracks into a deck */
    void loadIntoDeck(DeckGUI* deck);
    /** Finds index of a track in a playlist*/
    int findTrack(juce::String searchInput);

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PlaylistComponent)
};

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "OrionGlobalVars.h"
#include "GlobalCoefficients.h"

#include <iostream>
#include <array>

#define NUM_TABS 4

using namespace std;
using namespace juce;
//==============================================================================

OrionaudioAudioProcessorEditor::OrionaudioAudioProcessorEditor (OrionaudioAudioProcessor& p)
: AudioProcessorEditor (&p), processor (p),
AppDir("Applications",DrawableButton::ButtonStyle::ImageFitted),
DeskDir("Desktop",DrawableButton::ButtonStyle::ImageFitted),
DownDir("Downloads",DrawableButton::ButtonStyle::ImageFitted),
MusicDir("Music",DrawableButton::ButtonStyle::ImageFitted),
DocDir("Documents",DrawableButton::ButtonStyle::ImageFitted),
HomDir("Home",DrawableButton::ButtonStyle::ImageFitted),
UpBut("Up",DrawableButton::ImageOnButtonBackground),
filebrowser(1|4|8|32,File::getSpecialLocation(File::SpecialLocationType::userHomeDirectory),nullptr,nullptr),
mainlist("main", dynamic_cast<ListBoxModel*> (&maindir)), startTime(Time::getMillisecondCounterHiRes()*0.001)
{
    
    menuBar.reset(new OrionMenuBar(&p, this));
    addAndMakeVisible(menuBar.get());
    
    primaryPane.reset(new PrimaryPaneComponent(&p, this));
    addAndMakeVisible(primaryPane.get());
    
    sidePanel.reset(new SidePanelComponent(&p, this));
    addAndMakeVisible(sidePanel.get());
    
    tabComponents.reset(new TabComponentHolder(p));
    addAndMakeVisible(tabComponents.get());
    
    
    arrangementWindow.reset(new ArrangementWindowComponent(&p, this));
    addAndMakeVisible(arrangementWindow.get());

    
    //indices = {kickButton.index, snareButton.index, clapButton.index, percButton.index, HiHatButton.index, cymbalButton.index, snapButton.index};
    for (int i=0; i<7; i++)
    {
        indices[i] = i;
    }
    backgroundImage.reset(new ImageComponent("backgroundImage"));
    backgroundImage->setImage(ImageCache::getFromMemory(BinaryData::orionBackground_png, BinaryData::orionBackground_pngSize), RectanglePlacement::fillDestination);
    addAndMakeVisible(backgroundImage.get());
    
    // DROPDOWN BUTTON
    Image downImage = ImageCache::getFromMemory(BinaryData::arrow_down_png, BinaryData::arrow_down_pngSize);
    Image upImage = ImageCache::getFromMemory(BinaryData::arrow_up_png, BinaryData::arrow_up_pngSize);
    dropDownButton.reset(new ImageButton());
    dropDownButton->setClickingTogglesState(true);
    dropDownButton->setImages(false, true, true, upImage, 1.f, Colours::transparentBlack, upImage, 1.f, Colours::transparentBlack, downImage, 1.f, Colours::transparentBlack);
    //dropDownButton->onStateChange = [&] { editor->updateDropDownState(dropDownButton->getToggleState()); };
    dropDownButton->onClick = [&] {
        updateDropDownState(dropDownButton->getToggleState());
    };
    addAndMakeVisible(dropDownButton.get());
    
    
   
    
    
    // RESIZE BUTTON
    downImage = ImageCache::getFromMemory(BinaryData::ResizeOff_png, BinaryData::ResizeOff_pngSize);
    upImage = ImageCache::getFromMemory(BinaryData::ResizeOn_png, BinaryData::ResizeOn_pngSize);
    resizeButton.reset(new ImageButton());
    resizeButton->setClickingTogglesState(true);
    resizeButton->setImages(false, true, true, downImage, 1.f, Colours::transparentBlack, downImage, 1.f, Colours::transparentBlack, upImage, 1.f, Colours::transparentBlack);
    resizeButton->onClick = [&] {
        updateResizeViewState(resizeButton->getToggleState());
    };
    addAndMakeVisible(resizeButton.get());
    
    
    
    
    std::unique_ptr<Drawable> buttonOff;
    std::unique_ptr<Drawable> buttonOn;
        
    buttonOff = Drawable::createFromImageData(BinaryData::arrow_down_png, BinaryData::arrow_down_pngSize);
    buttonOn = Drawable::createFromImageData(BinaryData::arrow_up_png, BinaryData::arrow_up_pngSize);
    buttonOn = Drawable::createFromImageData(BinaryData::appdir_png, BinaryData::appdir_pngSize);
    
    AppDir.setImages(buttonOn.get(), buttonOn.get(), buttonOn.get());
    //AppDir.onStateChange = [this]/*capture this event 执行后面{}的指令*/{ appdirClicked(); };
    AppDir.setColour(TextButton::buttonColourId, Colours::darkgrey);
    AppDir.setEnabled(true);//防止用户多次按
    addAndMakeVisible(&AppDir);
    
    DeskDir.setImages(buttonOn.get(), buttonOn.get(), buttonOn.get());
    // DeskDir.onStateChange = [this]/*capture this event 执行后面{}的指令*/{ deskdirClicked(); };
    DeskDir.setColour(TextButton::buttonColourId, Colours::darkgrey);
    DeskDir.setEnabled(true);//防止用户多次按
    addAndMakeVisible(&DeskDir);
    
    DownDir.setImages(buttonOn.get(), buttonOn.get(), buttonOn.get());
    //DownDir.onStateChange = [this]/*capture this event 执行后面{}的指令*/{ downdirClicked(); };
    DownDir.setColour(TextButton::buttonColourId, Colours::darkgrey);
    DownDir.setEnabled(true);//防止用户多次按
    addAndMakeVisible(&DownDir);
    
    MusicDir.setImages(buttonOn.get(), buttonOn.get(), buttonOn.get());
    // MusicDir.onStateChange = [this]/*capture this event 执行后面{}的指令*/{ musicdirClicked(); };
    MusicDir.setColour(TextButton::buttonColourId, Colours::darkgrey);
    MusicDir.setEnabled(true);//防止用户多次按
    addAndMakeVisible(&MusicDir);
    
    DocDir.setImages(buttonOn.get(), buttonOn.get(), buttonOn.get());
    // DocDir.onStateChange = [this]/*capture this event 执行后面{}的指令*/{ docdirClicked(); };
    DocDir.setColour(TextButton::buttonColourId, Colours::darkgrey);
    DocDir.setEnabled(true);//防止用户多次按
    addAndMakeVisible(&DocDir);
    
    
    HomDir.setImages(buttonOn.get(), buttonOn.get(), buttonOn.get());
    //HomDir.onStateChange = [this]/*capture this event 执行后面{}的指令*/{ homedirClicked(); };
    HomDir.setColour(TextButton::buttonColourId, Colours::darkgrey);
    HomDir.setEnabled(true);//防止用户多次按
    addAndMakeVisible(&HomDir);
    
    //set up up button's images
    Path arrowPath;
    arrowPath.addArrow ({ 50.0f, 100.0f, 50.0f, 0.0f }, 40.0f, 100.0f, 50.0f);
    
    DrawablePath arrowImage;
    arrowImage.setFill (Colours::black.withAlpha (0.4f));
    arrowImage.setPath (arrowPath);
    UpBut.setImages(&arrowImage);
    addAndMakeVisible(&UpBut);
    
    formatManager.registerBasicFormats();
    
    
    
    fileBrowser.reset(new DraggableFileBrowserComponent());
    addAndMakeVisible(fileBrowser.get());
    
    // BACK BUTTON
    backButton.reset(new ImageButton());
    backButton->setAlwaysOnTop(true);
    Image backImageOff = ImageCache::getFromMemory(BinaryData::arrow_left_png, BinaryData::arrow_left_pngSize);
    Image backImageOn = ImageCache::getFromMemory(BinaryData::arrow_leftOn_png, BinaryData::arrow_leftOn_pngSize);
    backButton->setImages(false, true, true, backImageOff, 1.f, Colours::transparentBlack, backImageOff, 1.f, Colours::transparentBlack, backImageOn, 1.f, Colours::transparentBlack);
    backButton->onClick = [&] {
        //fileBrowser->backButton->onClick();
//        //fileBrowser->scanDirectory(fileBrowser->currentDirectory.getDirectory().getParentDirectory());
//        fileBrowser->scannerThread.stopThread(-1);
//        fileBrowser->windowComponent.setBounds(getLocalBounds());
//        fileBrowser->windowComponent.setNewDirectory(currentDirectory);
//        for (int i = 0; i < fileBrowser->windowComponent.fileComponents.size(); i++){
//            windowComponent.fileComponents[i]->addListener(this);
//        }
    };
    addAndMakeVisible(backButton.get());
    

    

    
    meterInput.reset(new CircularMeter());
    meterInput->updaterFunction = [this] {
        return processor.getInputLevel();
    };
    addAndMakeVisible(meterInput.get());
    

    
    constrainer.setFixedAspectRatio((float)OrionGlobalWidth/OrionGlobalHeight);
    constrainer.setSizeLimits((float)OrionGlobalWidth / 2, (float)OrionGlobalHeight / 2, (float)OrionGlobalWidth * 2, (float)OrionGlobalHeight * 2);
    constrainer.setSizeLimits((float)OrionGlobalWidth / 2, (float)OrionGlobalHeight / 2, (float)OrionGlobalWidth * 2, (float)OrionGlobalHeight * 2);
    
    cornerComponent.reset(new OrionResizableCornerComponent<OrionaudioAudioProcessorEditor>(this, this, &constrainer));
    //addAndMakeVisible(cornerComponent.get());
    
    
    setSize (OrionGlobalWidth, OrionGlobalHeight);
    setSize(getWidth(), (getHeight()/18) * 25);
    constrainer.setFixedAspectRatio((float)getWidth()/getHeight());
    setSize(getWidth(), (getHeight()/25) * 18);
    constrainer.setFixedAspectRatio((float)getWidth()/getHeight());
    
    
    //    meterLeft.reset(new CircularMeter());
    //    meterLeft->updaterFunction = [this] {
    //        return processor.getOutputLevel(0);
    //    };
    //    addAndMakeVisible(meterLeft.get());
    //
    //    meterRight.reset(new CircularMeter());
    //    meterRight->updaterFunction = [this] {
    //        return processor.getOutputLevel(1);
    //    };
    //    addAndMakeVisible(meterRight.get());
    
}



OrionaudioAudioProcessorEditor::~OrionaudioAudioProcessorEditor()
{
}


//==============================================================================
void OrionaudioAudioProcessorEditor::tabComponentChanged(int serial)
{
    //    tabComponent[serial]->setBounds(0, (getHeight()/3)*2, getWidth(), getHeight()/3);
    //
    //    addAndMakeVisible(tabComponent[serial].get());
}
//==============================================================================

void OrionaudioAudioProcessorEditor::paint (Graphics& g)
{
    //************************************DO NOT PUT ANY SET BOUNDS METHODS OR ANYTHING OTHER THAN DRAWING METHODS********************************
    //*********************************************************************************************************************************************
    //*********************************************************************************************************************************************
    //*********************************************************************************************************************************************
    
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
        
}

void OrionaudioAudioProcessorEditor::resized()
{
    
    
    //int width = getWidth();
    //int height = getHeight();
    
    
    //auto area = getLocalBounds();
    //cout << "............................................."<< endl;
    //cout << "Height: "<< getHeight() << endl;

    
//    // MENU BAR
//    auto menuBarArea = area;
//    menuBarArea = menuBarArea.removeFromTop(getHeight() * .1);
//    menuBarArea.removeFromLeft(getWidth() / 6);
//    menuBar->setBounds(menuBarArea);
//    
//    // PRIMARY PANE
//    auto primaryPaneArea = area;
//    primaryPaneArea.setY(menuBar->getBottom());
//    if (dropDownVisible)
//        primaryPaneArea.setBottom(tabComponents->getY());
//    else
//        primaryPaneArea.setBottom(getHeight());
//    primaryPaneArea.removeFromLeft(getWidth() / 6);
//    if (arrangementWindowVisible)
//        arrangementWindow->setBounds(primaryPaneArea);
//    else
//        primaryPane->setBounds(primaryPaneArea);
//
//    
//    // SIDE PANEL
//    area = getLocalBounds();
//    area = area.removeFromLeft(getWidth() / 6);
//    area.setHeight(h);
//    sidePanel->setBounds(area);
    
    
    
    
    float unite = 0.0;
    unite = getHeight()/36;
    
    // MENU BAR
    auto area = Rectangle<int>(getWidth()/6, 0, getWidth() * 5 / 6, 3 * unite);
    menuBar->setBounds(area);
    
    // SIDE PANEL
    area = Rectangle<int>(0, 0, getWidth()/6, getHeight());
    sidePanel->setBounds(area);
    
    // PRIMARY PANE
    area = Rectangle<int>(sidePanel->getWidth(), 3 * unite, getWidth() - sidePanel->getWidth(), getHeight() - 3 * unite);
    primaryPane->setBounds(area);
    
    // ARRANGEMENT WINDOW
    arrangementWindow->setBounds(area);
    arrangementWindow->setVisible(arrangementWindowVisible);
    
    // DROPDOWN WINDOW
    if (dropDownVisible)
    {
        area = Rectangle<int>(0, getHeight() - 14 * unite, getWidth(), 14 * unite);
        tabComponents->setBounds(area);
    }
    setUIScale(float(getWidth() / float(orion::defaultWidth)));
    
    // Back BUTTON
    area = Rectangle<int>(0.5 * unite, getHeight() - 2 * unite, 2 * unite, 2 * unite);
    backButton->setBounds(area);
    
    // DROPDOWN BUTTON
    area = Rectangle<int>(10 * unite, getHeight() - 2 * unite, 2 * unite, 2 * unite);
    dropDownButton->setBounds(area);
    
    
    // RESIZE BUTTON
    area = Rectangle<int>(getWidth() - 2.5 * unite, getHeight() - 2 * unite, 2 * unite, 2 * unite);
    resizeButton->setBounds(area);

    
    
    
    
    // CORNER COMPONENT
    cornerComponent->setBounds(getWidth() - (getWidth() * .03), getHeight() - (getWidth() * .03), (getWidth() * .03), (getWidth() * .03));
    

    
    //************************************DO NOT PUT ANY SET BOUNDS METHODS OR ANYTHING OTHER THAN DRAWING METHODS********************************
    //*********************************************************************************************************************************************
    //*********************************************************************************************************************************************
    //*********************************************************************************************************************************************
    
    
}

void OrionaudioAudioProcessorEditor::addMessageToList (const MidiMessage& message)
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
    //logMessage (timecode + "  -  " + getMidiMessageDescription (message));
}

void OrionaudioAudioProcessorEditor::drumButtonClicked(int midiNote, int tabIndex, bool isDown)
{
    if(isDown)
    {
        //std::cout<<"Down"<<std::endl;
        
   
        processor.getSampler()->noteOn(1, midiNote, 120);
        if (processor.getMidiOutput() != nullptr)
            processor.getMidiOutput()->sendMessageNow(MidiMessage::noteOn(1, midiNote, 1.f));
        
        tabComponents->setCurrentTab(tabIndex);
        
        instrumetSerial = tabIndex;
        
        //--------------------------------
        primaryPane->setInstrumetsVolumeSliderValue(instrumentsVolumeCoefficient[instrumetSerial]);
        primaryPane->setInstrumetsPanSliderValue(instrumentsPanCoefficient[instrumetSerial]);

//        /* Set Solo Button Image */
//        if(instrumentsSoloStates[instrumetSerial])
//        {
//            primaryPane->setInstrumetsSoloButtonImage(true);
//        }
//        else
//        {
//           primaryPane->setInstrumetsSoloButtonImage(false);
//        }
        
        /* Set Mute Button Image */
        if(instrumentsMuteStates[instrumetSerial])
        {
            primaryPane->setInstrumetsMuteButtonImage(true);
        }
        else
        {
            primaryPane->setInstrumetsMuteButtonImage(false);
            primaryPane->waveWiggle->waveColor = Colour(0xff3AE6D1);
            //primaryPane->waveWiggle->setVisible(true);
            primaryPane->waveWiggle->startAnimation();
        }
        
        //--------------------------------
        
            
    }
    else
    {
        processor.getSampler()->noteOff(1, midiNote, 0, true /*有淡出*/);
        globalOutputMeterL = 0;
        globalOutputMeterR = 0;
        if (processor.getMidiOutput() != nullptr)
            processor.getMidiOutput()->sendMessageNow(MidiMessage::noteOff(1, midiNote, 0.f));
    }
    
}

void OrionaudioAudioProcessorEditor::dropDownButtonClicked()
{
//            dropDown = !dropDown;
//            std::unique_ptr<Drawable> buttonBackground;
//            if(dropDown)
//            {
//                    setSize (orion::defaultWidth * getUIScale(), orion::defaultHeight * getUIScale());
//
//                    buttonBackground = Drawable::createFromImageData(BinaryData::arrow_up_png, BinaryData::arrow_up_pngSize);
//            }
//
//            else
//            {
//                    setSize (orion::defaultWidth * getUIScale(), orion::defaultHeight * getUIScale() * 2/3);
//                    buttonBackground = Drawable::createFromImageData(BinaryData::arrow_down_png, BinaryData::arrow_down_pngSize);
//            }
}

void OrionaudioAudioProcessorEditor::appdirClicked()
{
    //change the below directory
    whichdir = 1;
    changed = 1;
    
}

void OrionaudioAudioProcessorEditor::deskdirClicked()
{
    whichdir = 2;
    changed = 1;
}

void OrionaudioAudioProcessorEditor::downdirClicked()
{
    whichdir = 3;
    changed = 1;
}

void OrionaudioAudioProcessorEditor::musicdirClicked()
{
    whichdir = 4;
    changed = 1;
}

void OrionaudioAudioProcessorEditor::docdirClicked()
{
    whichdir = 5;
    changed = 1;
}

void OrionaudioAudioProcessorEditor::homedirClicked()
{
    whichdir = 6;
    changed = 1;
}

void OrionaudioAudioProcessorEditor::upbutClicked()
{
    filebrowser.goUp();
    
};

void OrionaudioAudioProcessorEditor::setDefaultSize()
{
    setSize(orion::defaultWidth, orion::defaultHeight);
}

void OrionaudioAudioProcessorEditor::updateDropDownState(bool newState)
{
    DBG((int)newState);
    dropDownVisible = newState;
    tabComponents->setVisible(dropDownVisible);
    resized();
}

void OrionaudioAudioProcessorEditor::updateResizeViewState(bool newState)
{
    DBG((int)newState);
    resizeViewVisible = newState;
    tabComponents->setVisible(dropDownVisible);
    resized();
}

void OrionaudioAudioProcessorEditor::toggleArrangmentWindow(bool windowVisible)
{
    arrangementWindowVisible = windowVisible;
    arrangementWindow->setVisible(windowVisible);
    primaryPane->setVisible(!windowVisible);
    resized();
}

bool OrionaudioAudioProcessorEditor::getDropdownVisible()
{
    return dropDownVisible;
}

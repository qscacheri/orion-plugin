/*
  ==============================================================================

    SimpleSynth.cpp
    Created: 29 May 2019 9:07:54am
    Author:  Lily H

  ==============================================================================
*/

#include "SimpleSynth.h"

#define MAX_VOICES 7


void SimpleSynth::setup(double sr) {
    sampleRate = sr;
    // add voices to our sampler
    
    for (int i = 0; i < MAX_VOICES; i++) {
        auto* voice = new OrionSamplerVoice(sampleRate,i);
        //auto* voice = new SamplerVoice();
        switch(i)
        {
            case KICK:
                voice->setMidiNote(36);
                break;
            case SNARE:
                voice->setMidiNote(38);
                break;
            case CLAP:
                voice->setMidiNote(39);
                break;
            case PERC:
                voice->setMidiNote(41);
                break;
            case HIHAT:
                voice->setMidiNote(42);
                break;
            case CYMBAL:
                voice->setMidiNote(43);
                break;
            case SNAP:
                voice->setMidiNote(46);
                break;
            default:
                break;
        }
        addVoice(voice);
        
    }
    audioFormatManager.registerBasicFormats();
    loadSamples();
    
}

void SimpleSynth::loadSamples()
{
   
    
    if(getNumSounds() != 0)
    {
        for(int i = 0; i < getNumSounds(); i++)
        {
            removeSound(i);
        }
    }
    int MidiNote;
    audiofolder =  File::getSpecialLocation(File::globalApplicationsDirectory).getChildFile("Orion").getChildFile("OrionSampler").getChildFile("OrionSampler").getChildFile("Contents").getChildFile("Resources").getChildFile("audio").getChildFile("demo");
    File* file;
    String dir;
    String filename;
    for(int i = 0; i < MAX_VOICES; i++)
    {
        switch(i)
        {
            case KICK:
                MidiNote = 36;
                dir = "Kicks";
                filename = "trell_kick.wav";
                break;
            case SNARE:
                MidiNote = 38;
                dir = "Snares";
                filename = "Snare.wav";
                break;
            case CLAP:
                MidiNote = 39;
                dir = "Claps";
                filename = "Clap2.wav";
                break;
            case PERC:
                 MidiNote = 41;
                 dir = "Percs";
                 filename = "basic-perc.wav";
                break;
            case HIHAT:
                 MidiNote = 42;
                dir = "HHO";
                filename = "HiHatOp(11).wav";
                //filename = "test.wav";
                break;
            case CYMBAL:
                 MidiNote = 43;
                dir = "Crash";
                filename = "Crash_01.wav";
                break;
            case SNAP:
                 MidiNote = 46;
                dir = "Snaps";
                filename = "HHPSNP1.WAV";
                break;
            default:
                break;
                
        }
        file = new File(audiofolder.getChildFile(dir).getChildFile(filename));
        reader = audioFormatManager.createReaderFor(*file);
        BigInteger note;
        note.setBit(MidiNote);
        OrionSamplerSound *sampler = new OrionSamplerSound(String(i), *reader, note, MidiNote, 0.0f, 10.0f, 10.0f);
        addSound(sampler);
        
        reader = nullptr;
        delete file;
    }
    
    
    
}

void SimpleSynth::changeSamples(int index,const String &f,int midi)//index should imply which instrument it is, but in this case index is the sequence
{
    /*
    String path = f.unquoted();
    if (File::isAbsolutePath (path))
    {
        // probably a file
        //file = new File (path);
        std::cout<<"so you are a file\n";
    }
    else{
        std::cout<<"no you are not\n";
    }
     */
    
    if(getNumSounds() != 0 and index < getNumSounds())
    {
        //std::cout<<"who did you lose "<<getNumSounds()<<" "<<index<<"\n";
        removeSound(index);
    }
    //const String f = files[0];
    file = new File(f);
    reader = audioFormatManager.createReaderFor(*file);
    BigInteger note;
    note.setBit(midi);
    OrionSamplerSound *sampler = new OrionSamplerSound(String(index), *reader, note, midi, 0.0f, 10.0f, 10.0f);
    addSound(sampler);
    reader = nullptr;
    delete file;

   
    
}


void SimpleSynth::noteOn(int midiChannel,
                         int midiNoteNumber,
                         float velocity)
{
    //const ScopedLock sl(lock);
    //std::cout<<"number of sounds"<<getNumSounds();
    for(int j = 0; j < getNumSounds(); j++)
    {
        auto sound = getSound(j);
        if(sound->appliesToNote(midiNoteNumber) && sound->appliesToChannel(midiChannel))
        {
            for(int i = 0; i < getNumVoices(); i++)
            {
                if(auto* voice = dynamic_cast<OrionSamplerVoice*>(getVoice(i)))
                {
                    //std::cout<<"can play sound "<<voice->canPlayOrionSound(midiNoteNumber);
                    //std::cout<<"current sound " << voice->getCurrentlyPlayingSound();
                    if(voice->canPlayOrionSound(midiNoteNumber) && voice->getCurrentlyPlayingSound())
                    {
                        //std::cout<<"stopped voice"<<midiNoteNumber<<voice->canPlayOrionSound(midiNoteNumber);
                        stopVoice(voice, 0.0f, false);
                    }
                }
            }
            
            for(int i = 0; i < getNumVoices(); i++)
            {
                if(auto* voice = dynamic_cast<OrionSamplerVoice*> (getVoice(i)))
                {
                    if(voice->canPlayOrionSound(midiNoteNumber) && !voice->getCurrentlyPlayingSound())
                    {
                        //Logger::outputDebugString("Start Voice: " + std::to_string(i) + " with Sound: " + std::to_string(j));
                        startVoice(voice, sound.get(), midiChannel, midiNoteNumber, velocity);
                    }
                }
            }
        }
    }
}



/*
  ==============================================================================

    GlobalCoefficients.h
    Created: 16 Jun 2020 8:54:22pm
    Author:  Lei Li

  ==============================================================================
*/
#include "../JuceLibraryCode/JuceHeader.h"

#ifdef __COEFFICIENTSMAIN__

const double globalPi = 3.14159265;

const int instrumentAmount = 8;

const int KickPitch  = 36;
const int SnarePitch = 38;
const int ClapPitch  = 40;
const int PercPitch  = 41;
const int SnapPitch  = 43;
const int HHCPitch   = 45;
const int HHOPitch   = 47;
const int CrashPitch = 48;

const String instrumentName[8] = {"Kick",   "Snare", "Clap",   "Perc",   "Snap",   "HHO",    "HHC",    "Crash"};
int sidechainIndex[8]          = {-1,       -1,       -1,       -1,       -1,       -1,       -1,       -1};
bool InstrumentMakeNoise[8]    = {false,    false,    false,    false,    false,    false,    false,    false};

// Global Smaple Rate
double globalSampleRate = 44100;


// Master Meters
float globalOutputMeterL = 0.0f;
float globalOutputMeterR = 0.0f;

// Instruments
int   instrumetClickedSerial = 0;
int   instrumetNoteOnSerial = 0;
//int   instrumetOffSerial = 0;
float masterVolumeCoefficient = 0.75f;
bool  instrumentsOnStates          [8] = {false, false, false, false, false, false, false, false};
bool  instrumentsSoloStates        [8] = {false, false, false, false, false, false, false, false};
bool  instrumentsMuteStates        [8] = {false, false, false, false, false, false, false, false};
float instrumentsVolumeCoefficient [8] = {0.75f, 0.75f, 0.75f, 0.75f, 0.75f, 0.75f, 0.75f, 0.75f};
float instrumentsPanCoefficient    [8] = {0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f, 0.00f};
File  instrumentSamplePathes       [8];

// Dropdown Type
int dropdownTabSerial = 0;
bool dropDownVisible = false;


// EQ




// Envelope
bool  envSwitches                  [8] = {false, false, false, false, false, false, false, false};

float envAttackCoefficient         [8] = {0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f};
float envDecayCoefficient          [8] = {0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f, 0.2f};
float envSustainCoefficient        [8] = {0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f};
float envReleaseCoefficient        [8] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};

float envAttackBendCoefficient     [8] = {-5.0f, -5.0f, -5.0f, -5.0f, -5.0f, -5.0f, -5.0f, -5.0f};
float envDecayBendCoefficient      [8] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
float envSustainBendCoefficient    [8] = {0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f};
float envReleaseBendCoefficient    [8] = {-5.0f, -5.0f, -5.0f, -5.0f, -5.0f, -5.0f, -5.0f, -5.0f};


// FX Switch
bool  compSwitches                 [8] = {false, false, false, false, false, false, false, false};
bool  reverbSwitches               [8] = {false, false, false, false, false, false, false, false};
bool  delaySwitches                [8] = {false, false, false, false, false, false, false, false};

// Compressor
float compressorThreshCoefficient  [8] = {-30.5f, -30.5f, -30.5f, -30.5f, -30.5f, -30.5f, -30.5f, -30.5f};
float compressorRatioCoefficient   [8] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
float compressorAttackCoefficient  [8] = {0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f};
float compressorReleaseCoefficient [8] = {0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f};
float compressorGainCoefficient    [8] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

// Reverb
float reverbDryCoefficient         [8] = {0.4f, 0.4f, 0.4f, 0.4f, 0.4f, 0.4f, 0.4f, 0.4f};
float reverbPredelayCoefficient    [8] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f};
float reverbDecayCoefficient       [8] = {0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f};
float reverbSizeCoefficient        [8] = {0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f};
float reverbColorCoefficient       [8] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};

// Delay
float delayDryWetCoefficient       [8] = {0.3f, 0.3f, 0.3f, 0.3f, 0.3f, 0.3f, 0.3f, 0.3f};
float delayTimeCoefficient         [8] = {0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f, 0.1f};
float delayFeedbackCoefficient     [8] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
float delayColorCoefficient        [8] = {0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f};
float delayPanCoefficient          [8] = {0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f};


#else
extern const double globalPi;
extern const int instrumentAmount;

extern const int KickPitch;
extern const int SnarePitch;
extern const int ClapPitch;
extern const int PercPitch;
extern const int SnapPitch;
extern const int HHCPitch;
extern const int HHOPitch;
extern const int CrashPitch;

extern const String instrumentName[];
extern int  sidechainIndex[];
extern bool InstrumentMakeNoise[];

// Global Smaple Rate
extern double globalSampleRate;

// Master Meters
extern float globalOutputMeterL;
extern float globalOutputMeterR;

// Instruments
extern int   instrumetClickedSerial;
extern int   instrumetNoteOnSerial;
//extern int   instrumetOffSerial;
extern float masterVolumeCoefficient;
extern bool  instrumentsOnStates[]; 
extern bool  instrumentsSoloStates[];
extern bool  instrumentsMuteStates[];
extern float instrumentsVolumeCoefficient[];
extern float instrumentsPanCoefficient[];
extern File  instrumentSamplePathes[];

// Dropdown Type
extern int dropdownTabSerial;
extern bool dropDownVisible;

// Envelope
extern bool  envSwitches                [];

extern float envAttackCoefficient       [];
extern float envDecayCoefficient        [];
extern float envSustainCoefficient      [];
extern float envReleaseCoefficient      [];

extern float envAttackBendCoefficient   [];
extern float envDecayBendCoefficient    [];
extern float envSustainBendCoefficient  [];
extern float envReleaseBendCoefficient  [];


// FX Switch
extern bool  compSwitches   [];
extern bool  reverbSwitches [];
extern bool  delaySwitches  [];

// Compressor
extern float compressorThreshCoefficient  [];
extern float compressorRatioCoefficient   [];
extern float compressorAttackCoefficient  [];
extern float compressorReleaseCoefficient [];
extern float compressorGainCoefficient    [];

// Reverb
extern float reverbDryCoefficient         [];
extern float reverbPredelayCoefficient    [];
extern float reverbDecayCoefficient       [];
extern float reverbSizeCoefficient        [];
extern float reverbColorCoefficient       [];

// Delay
extern float delayDryWetCoefficient       [];
extern float delayTimeCoefficient         [];
extern float delayFeedbackCoefficient     [];
extern float delayColorCoefficient        [];
extern float delayPanCoefficient          [];

#endif




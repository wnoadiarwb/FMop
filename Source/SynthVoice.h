/*
  ==============================================================================

    SynthVoice.h
    Created: 27 Apr 2021 10:16:39pm
    Author:  Otto Benson

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "SynthSound.h"

class SynthVoice : public juce::SynthesiserVoice
{
public:
    bool canPlaySound (juce::SynthesiserSound* sound) override;
    void startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition) override;
    void stopNote (float velocity, bool allowTailOff) override;
    void pitchWheelMoved (int newPitchWheelValue) override;
    void controllerMoved (int controllerNumber, int newControllerValue) override;
    void aftertouchChanged (int newAftertouchValue) override;
    void channelPressureChanged (int newChannelPressureValue) override;
    void renderNextBlock (juce::AudioBuffer< float > &outputBuffer, int startSample, int numSamples) override;
    void prepareToPlay (double sampleRate, int samplesPerBlock, int outputChannels);
    void setCurrentPlaybackSampleRate (double newRate) override;
//    void isPlayingChannel (int midiChannel) override;
    
    void updateADSR (const float attack, const float decay, const float sustain, const float release);
    void updateFM(const float depth, const float freq, const float cfreq, const int midiChoice,const int zeroChoice );
    void updateFMADSR (const float attack, const float decay, const float sustain, const float release);
    void updateGain (const float gain);
    
private:
    
    //ADSR
    juce::ADSR adsr;
    juce::ADSR::Parameters adsrParams;
    juce::ADSR FMadsr;
    juce::ADSR::Parameters FMadsrParams;
    
    juce::AudioBuffer<float> synthBuffer;
    
    
    float fmMod { 0.0f };
    float fmDepth { 0.0f };
    int lastMidiNote { 0 };
    
    // modulator oscillator
    juce::dsp::Oscillator<float> mosc { [](float x) {return std::sin (x); }};
    // carrier oscillator
    juce::dsp::Oscillator<float> cosc { [](float x) {return std::sin (x); }};
    
    juce::dsp::Gain<float> gain;
    bool isPrepared { false };
    
    juce::dsp::ProcessorChain<juce::dsp::Reverb> fxChain;
    juce::dsp::ProcessorChain<juce::dsp::Phase<float>> phaseMod;
    
};


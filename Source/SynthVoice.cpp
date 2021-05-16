/*
  ==============================================================================

    SynthVoice.cpp
    Created: 27 Apr 2021 10:56:39pm
    Author:  Otto Benson

  ==============================================================================
*/

#include "SynthVoice.h"

bool SynthVoice::canPlaySound (juce::SynthesiserSound* sound)
{
    //checks for returned synthesizer sound 
    return dynamic_cast<juce::SynthesiserSound*>(sound) != nullptr;
}

void SynthVoice::startNote (int midiNoteNumber, float velocity, juce::SynthesiserSound *sound, int currentPitchWheelPosition)
{
    // change osc frequency according to midi input note
//    cosc.setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber) +  fmMod);
    cosc.setFrequency(juce::MidiMessage::getMidiNoteInHertz(midiNoteNumber) + fmMod);
    //+ fmMod * FMadsr.getNextSample()
    lastMidiNote = midiNoteNumber;
    
    FMadsr.noteOn();
    adsr.noteOn();
}
void SynthVoice::stopNote (float velocity, bool allowTailOff)
{
    adsr.noteOff();
    FMadsr.noteOff();
    
    if (! allowTailOff || ! adsr.isActive())
        clearCurrentNote();
}
void SynthVoice::pitchWheelMoved (int newPitchWheelValue)
{
    
}
void SynthVoice::controllerMoved (int controllerNumber, int newControllerValue)
{
    
}
void SynthVoice::aftertouchChanged (int newAftertouchValue)
{
    
}
void SynthVoice::channelPressureChanged (int newChannelPressureValue)
{
    
}
void SynthVoice::renderNextBlock (juce::AudioBuffer< float > &outputBuffer, int startSample, int numSamples)
{
    jassert (isPrepared);

    if (! isVoiceActive())
        return;
    
    synthBuffer.setSize(outputBuffer.getNumChannels(), numSamples, false, false, true);
    synthBuffer.clear();
    

    juce::dsp::AudioBlock<float> audioBlock { synthBuffer };
//    original output buffer
//    juce::dsp::AudioBlock<float> audioBlock {outputBuffer};
//    mosc.process (juce::dsp::ProcessContextReplacing<float> (audioBlock));

    for (int ch = 0; ch < synthBuffer.getNumChannels(); ++ch)
    {
        for (int s = startSample; s < numSamples; ++s)
        {
            fmMod = mosc.processSample(synthBuffer.getSample(ch, s)) * fmDepth * FMadsr.getNextSample();
        }
    }
    
    //process carrier oscillator in audioblock
    cosc.process (juce::dsp::ProcessContextReplacing<float> (audioBlock));
    //process gain in audioblock
    gain.process(juce::dsp::ProcessContextReplacing<float> (audioBlock));
    
    //reverb
//    fxChain.process(juce::dsp::ProcessContextReplacing<float> (audioBlock));
    
    adsr.applyEnvelopeToBuffer(synthBuffer, 0
                               , synthBuffer.getNumSamples());
    
    for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
    {
        outputBuffer.addFrom(channel, startSample, synthBuffer, channel, 0, numSamples);
        if (! adsr.isActive())
            clearCurrentNote();
    }
    
}
void SynthVoice::prepareToPlay (double sampleRate, int samplesPerBlock, int outputChannels)
{
    adsr.setSampleRate (sampleRate);
    FMadsr.setSampleRate(sampleRate);
    
    juce::dsp::ProcessSpec spec;
    spec.maximumBlockSize = samplesPerBlock;
    spec.sampleRate = sampleRate;
    spec.numChannels = outputChannels;
    
    mosc.prepare (spec);
//    ****
//    mosc.setFrequency(200);
    cosc.prepare (spec);
    gain.prepare (spec);
    
    isPrepared = true;

}

void SynthVoice::updateADSR (const float attack, const float decay, const float sustain, const float release)
{
    adsrParams.attack = attack;
    adsrParams.decay = decay;
    adsrParams.sustain = sustain; 
    adsrParams.release = release;
    
    adsr.setParameters (adsrParams);
}
void SynthVoice::updateFMADSR (const float attack, const float decay, const float sustain, const float release)
{
    FMadsrParams.attack = attack;
    FMadsrParams.decay = decay;
    FMadsrParams.sustain = sustain;
    FMadsrParams.release = release;
    
    FMadsr.setParameters (FMadsrParams);
}

void SynthVoice::setCurrentPlaybackSampleRate (double newRate)
{
    // stops execution because prepare to play was not called
    jassert (isPrepared);
    
}
    
void SynthVoice::updateGain(const float g)
{
    gain.setGainLinear (g);
}

void SynthVoice::updateFM(const float depth, const float freq, const float cfreq, const int midiChoice,const int zeroChoice )
{
    mosc.setFrequency (freq);
    fmDepth = depth;
    
    auto currentFreq = juce::MidiMessage::getMidiNoteInHertz (lastMidiNote) + fmMod;
    
    if(midiChoice){
        currentFreq = cfreq + fmMod;
    }
    
//    THROUGH ZERO FM
//    evaluates to current freq or -current freq if greater than 0
    if(!zeroChoice){
        cosc.setFrequency (currentFreq >= 0 ? currentFreq : currentFreq * -1.0f);
    }
    else{
        //    Prevents negative frequencies
        cosc.setFrequency (currentFreq >= 0 ? currentFreq : currentFreq * 0.0f);
    }
    

}

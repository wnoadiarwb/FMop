/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FMopAudioProcessor::FMopAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  juce::AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", juce::AudioChannelSet::stereo(), true)
                     #endif
                       ), apvts(*this, nullptr, "Parameters", createParams()) //create parameters
#endif
 

{
    // adds synth voice to the synthesizer
    synth.addSound(new SynthSound());
    synth.addVoice(new SynthVoice());
}

FMopAudioProcessor::~FMopAudioProcessor()
{
}

//==============================================================================
const juce::String FMopAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool FMopAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool FMopAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool FMopAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double FMopAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int FMopAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int FMopAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FMopAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String FMopAudioProcessor::getProgramName (int index)
{
    return {};
}

void FMopAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void FMopAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    synth.setCurrentPlaybackSampleRate(sampleRate);
    
    for (int i = 0; i < synth.getNumVoices(); i++)
    {
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            voice->prepareToPlay (sampleRate, samplesPerBlock, getTotalNumOutputChannels());
        }
    }
}

void FMopAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FMopAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void FMopAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    
    for(int i = 0; i < synth.getNumVoices(); ++i){
        if (auto voice = dynamic_cast<SynthVoice*>(synth.getVoice(i)))
        {
            auto& cfreq = *apvts.getRawParameterValue("CFREQ");
            auto& midichoice = *apvts.getRawParameterValue("MIDICHOICE");
            auto& gain = *apvts.getRawParameterValue("GAIN");
            
            auto& attack = *apvts.getRawParameterValue("ATTACK");
            auto& decay = *apvts.getRawParameterValue("DECAY");
            auto& sustain = *apvts.getRawParameterValue("SUSTAIN");
            auto& release = *apvts.getRawParameterValue("RELEASE");
            
            auto& depth = *apvts.getRawParameterValue("MODDEPTH");
            auto& freq = *apvts.getRawParameterValue("MODFREQ");
            
            auto& fmattack = *apvts.getRawParameterValue("FMATTACK");
            auto& fmdecay = *apvts.getRawParameterValue("FMDECAY");
            auto& fmsustain = *apvts.getRawParameterValue("FMSUSTAIN");
            auto& fmrelease = *apvts.getRawParameterValue("FMRELEASE");
            
            auto& throughzero = *apvts.getRawParameterValue("THRUZERO");
            
            voice->updateGain(gain.load());
            voice->updateADSR(attack.load(), decay.load(), sustain.load(), release.load());
//            voice->updateFM(depth.load(), freq.load(), cfreq.load(), throughzero);
            voice->updateFM(depth.load(), freq.load(), cfreq.load(), midichoice,throughzero );
            
            voice->updateFMADSR(fmattack.load(), fmdecay.load(), fmsustain.load(), fmrelease.load());
        }
    }
    
    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
    
    

}

//==============================================================================
bool FMopAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* FMopAudioProcessor::createEditor()
{
    return new FMopAudioProcessorEditor (*this);
}

//==============================================================================
void FMopAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void FMopAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FMopAudioProcessor();
}

// value tree
juce::AudioProcessorValueTreeState::ParameterLayout FMopAudioProcessor::createParams()
{
    
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> params;
//    params.push_back(std::make_unique<juce::AudioParameter> )
    
//    parameter layout for gain
    params.push_back (std::make_unique<juce::AudioParameterFloat>("GAIN", "Gain", 0.0f, 1.0f, 0.5f));
    
//    FM
    params.push_back(std::make_unique<juce::AudioParameterFloat>("MODFREQ", "Mod Frequency", juce::NormalisableRange<float> {0.0f, 1000.0f, }, 50.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("MODDEPTH", "Mod Depth", juce::NormalisableRange<float> {0.0f, 1000.0f, }, 50.0f));
    
    
    
    // Carrier Freq - float
    params.push_back(std::make_unique<juce::AudioParameterFloat>("CFREQ", "Carrier Freq", juce::NormalisableRange<float> {0.0f, 20000.0f, }, 0.0f));
    
    // Attack - float
    params.push_back(std::make_unique<juce::AudioParameterFloat>("ATTACK", "Attack", juce::NormalisableRange<float> {0.001f, 1.0f, }, 0.1f));
    // Decay - float
    params.push_back(std::make_unique<juce::AudioParameterFloat>("DECAY", "Decay", juce::NormalisableRange<float> {0.001f, 1.0f, }, 0.1f));
    // Sustain - float - default 1.0
    params.push_back(std::make_unique<juce::AudioParameterFloat>("SUSTAIN", "Sustain", juce::NormalisableRange<float> {0.001f, 1.0f, }, 1.0f));
    // Release - float
    params.push_back(std::make_unique<juce::AudioParameterFloat>("RELEASE", "Release", juce::NormalisableRange<float> {0.001f, 3.0f, }, 0.4f));
    
    // FM ADSR
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FMATTACK", "FM Attack", juce::NormalisableRange<float> {0.0f, 1.0f, }, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FMDECAY", "FM Decay", juce::NormalisableRange<float> {0.0f, 1.0f, }, 0.0f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FMSUSTAIN", "FM Sustain", juce::NormalisableRange<float> {0.001f, 1.0f, }, 0.01f));
    params.push_back(std::make_unique<juce::AudioParameterFloat>("FMRELEASE", "FM Release", juce::NormalisableRange<float> {0.001f, 3.0f, }, 0.001f));
    
    params.push_back(std::make_unique<juce::AudioParameterChoice>("THRUZERO", "Through Zero", juce::StringArray {"Through Zero", "To Zero"}, 0));
    params.push_back(std::make_unique<juce::AudioParameterChoice>("MIDICHOICE", "MIDI Choice", juce::StringArray {"MIDI", "Slider"}, 0));
    
    
    return { params.begin(), params.end()};
}

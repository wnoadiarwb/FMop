/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
FMopAudioProcessorEditor::FMopAudioProcessorEditor (FMopAudioProcessor& p)
    : AudioProcessorEditor (&p), audioProcessor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (600, 250);
    
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    carrierAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "CFREQ", carrierFreqSlider);
    attackAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "ATTACK", attackSlider);
    decayAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "DECAY", decaySlider);
    sustainAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "SUSTAIN", sustainSlider);
    releaseAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "RELEASE", releaseSlider);
    
    gainAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "GAIN", gainSlider);
    
    freqAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "MODFREQ", FMfreqSlider);
    modAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "MODDEPTH", FMmodSlider);
    
    FMattackAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "FMATTACK", FMattackSlider);
    FMdecayAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "FMDECAY", FMdecaySlider);
    FMsustainAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "FMSUSTAIN", FMsustainSlider);
    FMreleaseAttachment = std::make_unique<SliderAttachment>(audioProcessor.apvts, "FMRELEASE", FMreleaseSlider);
    
    
    setSliderParams(carrierFreqSlider, carrierFreqLabel, "Freq", 0.0f, 20000.0f, 0.0f);
    setSliderParams(attackSlider, attacklabel, "Attack", 0.001f, 1.0f, 0.1f);
    setSliderParams(decaySlider, decaylabel, "Decay", 0.001f, 1.0f, 0.1f);
    setSliderParams(sustainSlider, sustainlabel, "Sustain", 0.001f, 1.0f, 1.0f);
    setSliderParams(releaseSlider, releaselabel, "Release", 0.001f, 3.0f, 0.4f);
    
    setSliderParams(gainSlider, gainlabel, "Gain", 0.0f, 1.0f, 0.5f);
    
    setSliderParams(FMfreqSlider, FMfreqlabel, "Mod Freq", 0.0f, 1000.0f, 50.0f);
    setSliderParams(FMmodSlider, FMmodlabel, "Mod Amount", 0.0f, 1000.0f, 50.0f);
    
    setSliderParams(FMattackSlider, FMattacklabel, "FM Attack", 0.0f, 1.0f, 0.0f);
    setSliderParams(FMdecaySlider, FMdecaylabel, "FM Decay", 0.0f, 1.0f, 0.0f);
    setSliderParams(FMsustainSlider, FMsustainlabel, "FM Sustain", 0.001f, 1.0f, 0.01f);
    setSliderParams(FMreleaseSlider, FMreleaselabel, "FM Release", 0.001f, 3.0f, 0.001f);

    using ComboBoxAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;
    throughZeroAttachment = std::make_unique<ComboBoxAttachment>(audioProcessor.apvts, "THRUZERO", throughZeroBox);
    midiChoiceAttachment = std::make_unique<ComboBoxAttachment>(audioProcessor.apvts, "MIDICHOICE", midiChoiceBox);
    
    addAndMakeVisible(throughZeroBox);
    juce::StringArray zchoices {"Through Zero", "To Zero"};
    throughZeroBox.addItemList(zchoices, 1);
    throughZeroBox.setText("Through/To Zero FM");
    
    addAndMakeVisible(midiChoiceBox);
    juce::StringArray mchoices {"MIDI", "Slider"};
    midiChoiceBox.addItemList(mchoices, 1);
    midiChoiceBox.setText("Toggle MIDI Pitch");
    
    
}

FMopAudioProcessorEditor::~FMopAudioProcessorEditor()
{
}

//==============================================================================
void FMopAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    auto bounds = getLocalBounds().reduced(0);
    auto fmRect = bounds.removeFromLeft(295).removeFromTop(300);
    g.fillAll (juce::Colours::white);

    
    g.setColour (juce::Colours::lightgreen);
    g.fillRect(fmRect.toFloat());
    
    g.setColour (juce::Colours::orange);
    g.setFont (15.0f);
     
    g.drawFittedText ("Chowning FM Operator v1.1\nOtto Benson - 2021", 0, 220, getWidth(), 30, juce::Justification::right, 2);
    
    g.setColour (juce::Colours::black);
}

void FMopAudioProcessorEditor::resized()
{
    auto sliderLeft = 350;
    
    midiChoiceBox.setBounds(sliderLeft, 20, getWidth() - sliderLeft - 80, 20);
    carrierFreqSlider.setBounds(sliderLeft, 60, getWidth() - sliderLeft - 10, 20);
    
    attackSlider.setBounds(sliderLeft, 80, getWidth() - sliderLeft - 10, 20);
    decaySlider.setBounds(sliderLeft, 100, getWidth() - sliderLeft - 10, 20);
    sustainSlider.setBounds(sliderLeft, 120, getWidth() - sliderLeft - 10, 20);
    releaseSlider.setBounds(sliderLeft, 140, getWidth() - sliderLeft - 10, 20);
    
    
    
    gainSlider.setBounds(sliderLeft, 180, getWidth() - sliderLeft - 10, 20);
    
    FMmodSlider.setBounds(70,20, getWidth() - sliderLeft - 30, 20);
    FMfreqSlider.setBounds(70,40, getWidth() - sliderLeft - 30, 20);
    FMattackSlider.setBounds(70,80, getWidth() - sliderLeft - 30, 20);
    FMdecaySlider.setBounds(70,100, getWidth() - sliderLeft - 30, 20);
    FMsustainSlider.setBounds(70,120, getWidth() - sliderLeft - 30, 20);
    FMreleaseSlider.setBounds(70,140, getWidth() - sliderLeft - 30, 20);
    throughZeroBox.setBounds (70, 180, getWidth() - sliderLeft - 80, 20);
    
    
}

void FMopAudioProcessorEditor::setSliderParams (juce::Slider& slider, juce::Label& label, const std::string text, float low, float high, float value)
{
    addAndMakeVisible (slider);
    slider.setRange(low, high);
    slider.setValue (value); // [5]
    slider.setColour(juce::Slider::textBoxTextColourId, juce::Colours::black);
    
    
    addAndMakeVisible (label);
    label.setText (text, juce::dontSendNotification);
    label.attachToComponent (&slider, true);
    label.setColour (juce::Label::textColourId, juce::Colours::black);
}

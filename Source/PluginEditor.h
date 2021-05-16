/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class FMopAudioProcessorEditor  : public juce::AudioProcessorEditor
                                
{
public:
    FMopAudioProcessorEditor (FMopAudioProcessor&);
    ~FMopAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
//    void sliderValueChanged (juce::Slider* slider) override; // [3]
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    void setSliderParams (juce::Slider& slider, juce::Label& label, const std::string text,float low, float high, float value);
    
    juce::Slider carrierFreqSlider;
    juce::Label carrierFreqLabel;
    
    juce::Slider frequencySlider;
    juce::Label  frequencyLabel;
    juce::Slider durationSlider;
    juce::Label  durationLabel;
    
    juce::Slider attackSlider;
    juce::Label  attacklabel;
    juce::Slider decaySlider;
    juce::Label  decaylabel;
    juce::Slider sustainSlider;
    juce::Label  sustainlabel;
    juce::Slider releaseSlider;
    juce::Label  releaselabel;
    
    juce::Slider gainSlider;
    juce::Label  gainlabel;
    
    juce::Slider FMfreqSlider;
    juce::Label  FMfreqlabel;
    juce::Slider FMmodSlider;
    juce::Label  FMmodlabel;
    
    juce::Slider FMattackSlider;
    juce::Label  FMattacklabel;
    juce::Slider FMdecaySlider;
    juce::Label  FMdecaylabel;
    juce::Slider FMsustainSlider;
    juce::Label  FMsustainlabel;
    juce::Slider FMreleaseSlider;
    juce::Label  FMreleaselabel;
    
//    enum class ZeroState{
//        Through,
//        Off
//    };
//    ZeroState zeroState { ZeroState::Through };
//    juce::TextButton throughZeroButton { "Through Zero" };
    juce::ComboBox throughZeroBox;
    juce::ComboBox midiChoiceBox;
//    juce::ToggleButton throughZero;
    
    // clean up the slider attachment code
    using SliderAttachment = juce::AudioProcessorValueTreeState::SliderAttachment;
    
    std::unique_ptr<SliderAttachment> carrierAttachment;
    
    std::unique_ptr<SliderAttachment> attackAttachment;
    std::unique_ptr<SliderAttachment> decayAttachment;
    std::unique_ptr<SliderAttachment> sustainAttachment;
    std::unique_ptr<SliderAttachment> releaseAttachment;
    
    std::unique_ptr<SliderAttachment> gainAttachment;
    
    std::unique_ptr<SliderAttachment> freqAttachment;
    std::unique_ptr<SliderAttachment> modAttachment;
    
    std::unique_ptr<SliderAttachment> FMattackAttachment;
    std::unique_ptr<SliderAttachment> FMdecayAttachment;
    std::unique_ptr<SliderAttachment> FMsustainAttachment;
    std::unique_ptr<SliderAttachment> FMreleaseAttachment;
    
    using ComboBoxAttachment = juce::AudioProcessorValueTreeState::ComboBoxAttachment;
    
    std::unique_ptr<ComboBoxAttachment> throughZeroAttachment;
    std::unique_ptr<ComboBoxAttachment> midiChoiceAttachment;
    
    FMopAudioProcessor& audioProcessor;
    
//    juce::Label throughZeroLabel { "Through Zero", "To Zero" };
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FMopAudioProcessorEditor)
};

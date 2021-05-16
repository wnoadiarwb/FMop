/*
  ==============================================================================

    SynthSound.h
    Created: 27 Apr 2021 10:16:32pm
    Author:  Otto Benson

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

class SynthSound : public juce::SynthesiserSound
{
    public:
        
        bool appliesToNote (int midiNoteNumber) override
        {
            return true;
        }
        
        bool appliesToChannel (int midiChannel) override
        {
            return true;
        }
        
    private:
    
};

//
// Created by Joshua Hodge on 6/22/26.
//

#pragma once

#include "SamplerLookAndFeel.h"
#include "juce_audio_processors/juce_audio_processors.h"


class Knob : public juce::Component
{
public:
    Knob (const juce::String& text,
        juce::AudioProcessorValueTreeState& apvts,
        const juce::ParameterID& parameterID);

    ~Knob() override = default;

    void resized() override;

    juce::Slider& getSlider() { return slider; }
    juce::Label& getLabel() { return label; }

private:
    KnobLookAndFeel sliderLF;
    juce::Slider slider;
    juce::Label label;
    juce::AudioProcessorValueTreeState::SliderAttachment attachment;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Knob)
};

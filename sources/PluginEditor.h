#pragma once

#include "juce_gui_basics/juce_gui_basics.h"
#include "SamplerLookAndFeel.h"
#include "Knob.h"
#include "PluginProcessor.h"

class SamplerAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit SamplerAudioProcessorEditor(SamplerAudioProcessor&);
    ~SamplerAudioProcessorEditor() override = default;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    static constexpr int windowWidth = 688;
    static constexpr int windowHeight = 516;
    SamplerAudioProcessor& audioProcessor;

    Knob decayKnob { "Decay", audioProcessor.apvts, "decay" };
    Knob reverbKnob { "Reverb", audioProcessor.apvts, "reverb" };

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SamplerAudioProcessorEditor)
};

#pragma once

#include "juce_gui_basics/juce_gui_basics.h"
#include "SamplerLookAndFeel.h"
#include "PluginProcessor.h"

class SamplerAudioProcessorEditor : public juce::AudioProcessorEditor
{
public:
    explicit SamplerAudioProcessorEditor(SamplerAudioProcessor&);
    ~SamplerAudioProcessorEditor() override;

    void paint(juce::Graphics&) override;
    void resized() override;

private:
    // 3. Now let's abstract out width and height so we only need to change in one place if we change the background
    static constexpr int windowWidth = 688;
    static constexpr int windowHeight = 516;
    SamplerAudioProcessor& audioProcessor;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SamplerAudioProcessorEditor)
};

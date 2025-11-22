#pragma once

#include "juce_audio_processors/juce_audio_processors.h"

// 1. Must add the audio formats library that contains the Synthesiser Class
#include "juce_audio_formats/juce_audio_formats.h"

class SamplerAudioProcessor final : public juce::AudioProcessor
{
public:
    SamplerAudioProcessor();

    bool hasEditor() const override { return true; }
    const juce::String getName() const override { return JucePlugin_Name; }
    bool acceptsMidi() const override { return true; }
    bool producesMidi() const override { return false; }
    bool isMidiEffect() const override { return false; }
    double getTailLengthSeconds() const override { return 0.0; }
    int getNumPrograms() override { return 1; }
    int getCurrentProgram() override { return 0; }
    void setCurrentProgram(int) override { }
    const juce::String getProgramName(int) override { return {}; }
    void changeProgramName(int, const juce::String&) override { }

    bool isBusesLayoutSupported(const BusesLayout& layouts) const override;
    void prepareToPlay(double sampleRate, int samplesPerBlock) override;
    void releaseResources() override { }
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;
    juce::AudioProcessorEditor* createEditor() override;

    juce::AudioProcessorValueTreeState apvts;

private:
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    void clearUnusedOutputChannels(juce::AudioBuffer<float>& buffer) const;

    /* 2. We'll create a Synthesiser object.  A better name for this would probably be MIDI Playback Engine
     * Since this engine is used for both synths and samplers
     * Synthesiser Class: https://docs.juce.com/master/classjuce_1_1Synthesiser.html */

    juce::Synthesiser synth;

    // 3. We'll make this a polyphonic synth.  Since this won't change we can make this constexpr
    static constexpr auto numVoices = 8;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SamplerAudioProcessor)
};

#pragma once

#include "juce_audio_processors/juce_audio_processors.h"
#include "juce_audio_formats/juce_audio_formats.h"
#include <BinaryData.h>

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
    void prepareToPlay(double newSampleRate, int samplesPerBlock) override;
    void releaseResources() override { }
    void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&) override;
    void getStateInformation(juce::MemoryBlock& destData) override;
    void setStateInformation(const void* data, int sizeInBytes) override;
    juce::AudioProcessorEditor* createEditor() override;

    juce::AudioProcessorValueTreeState apvts;

private:
    static juce::AudioProcessorValueTreeState::ParameterLayout createParameterLayout();
    void clearUnusedOutputChannels(juce::AudioBuffer<float>& buffer) const;

    // 3. Now we need to make this a member function
    juce::SamplerSound* loadSound(const juce::String& name,
                                    int originalMidiNote,
                                    const std::vector<int>& midiNoteSet,
                                    const void* data,
                                    size_t sizeInBytes);

    juce::Synthesiser synth;
    static constexpr auto numVoices = 8;

    juce::AudioFormatManager formatManager;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SamplerAudioProcessor)
};

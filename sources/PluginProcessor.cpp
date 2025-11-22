#include "PluginProcessor.h"
#include "PluginEditor.h"

SamplerAudioProcessor::SamplerAudioProcessor() :
    AudioProcessor(BusesProperties().withOutput("Output", juce::AudioChannelSet::stereo(), true)),
    apvts(*this, nullptr, "Parameters", createParameterLayout())
{
    for (int i = 0; i < numVoices; ++i)
    {
        synth.addVoice(new juce::SamplerVoice);
    }

    //! LOAD ONE SOUND FROM BINARY DATA

    /* 1. The AudioFormatManager adds the ability to handle different types of audio formats, such as WAV and AIFF
     * AudioFormatManager Class: https://docs.juce.com/master/classjuce_1_1AudioFormatManager.html */

    juce::AudioFormatManager formatManager;

    // 2. You must call registerBasicFormats() so the format manager can register handlers for these file types

    formatManager.registerBasicFormats();

    /* 3. First we need the sound we're trying to load.  We can use the MemoryInputStream class
     * This would first start as auto sound = juce::MemoryInputStream(BinaryData::c5_wav, BinaryData::c5_wavSize, false);
     * It needs to become a unique ptr later because createReaderFor() takes in a unique ptr
     * MemoryInputStream Class: https://docs.juce.com/master/classjuce_1_1MemoryInputStream.html */

    auto inputStream = std::make_unique<juce::MemoryInputStream>(BinaryData::c5_wav,
                                                               BinaryData::c5_wavSize,
                                                               false);

    /* 4. Now that we've registered the formats, we use a JUCE MemoryInputStream to try and read in the first sound
     * We use std::move to pass ownership from our PluginProcessor class to the Format Manager.
     * Otherwise, both the PluginProcessor and AudioFormat objects would own the inputStream...forbidden by unique ptr
     * Remember to check that the reader is not null before using it
     * MemoryInputStream Class: https://docs.juce.com/master/classjuce_1_1MemoryInputStream.html */

    if (auto reader = formatManager.createReaderFor(std::move(inputStream)))
    {
        // 6. I'll add some local variables because I will want to bring this out into its own function later
        const juce::String name = "C5";
        int originalMidiNote = 60; // Middle C

        // 7. This enables us to be able to load the sound at multiple pitches if we want.
        // Think of BigInteger as a set of all 128 midi notes, and I can switch each one on or off to trigger the same sample
        juce::BigInteger midiNote;
        midiNote.setBit(originalMidiNote);

        /* 5. Now we create a SamplerSound that contains our wav file data and assigns it to a midi note with attack, release
         * and max sample length of time (in seconds)
         * SamplerSound Class: https://docs.juce.com/master/classjuce_1_1SamplerSound.html */

        auto sound = new juce::SamplerSound(name, *reader, midiNote, originalMidiNote, 0.0, 0.1, 10.0);

        // 6. Now we'll load the sound into the synth
        synth.addSound(sound);
    }
}

bool SamplerAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    return layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo();
}

void SamplerAudioProcessor::prepareToPlay(double newSampleRate, int maximumBlockSize)
{
    synth.setCurrentPlaybackSampleRate(newSampleRate);
}

void SamplerAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    clearUnusedOutputChannels(buffer);
    synth.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());
}

void SamplerAudioProcessor::clearUnusedOutputChannels(juce::AudioBuffer<float>& buffer) const
{
    for ( auto i = getTotalNumInputChannels(); i < getTotalNumOutputChannels(); ++i) {
        buffer.clear(i, 0, buffer.getNumSamples());
    }
}

void SamplerAudioProcessor::getStateInformation(juce::MemoryBlock& destData)
{
    copyXmlToBinary(*apvts.copyState().createXml(), destData);
}

void SamplerAudioProcessor::setStateInformation(const void* data, int sizeInBytes)
{
    std::unique_ptr<juce::XmlElement> xml(getXmlFromBinary(data, sizeInBytes));
    if (xml.get() != nullptr && xml->hasTagName(apvts.state.getType()))
    {
        apvts.replaceState(juce::ValueTree::fromXml(*xml));
    }
}

juce::AudioProcessorEditor* SamplerAudioProcessor::createEditor()
{
    return new SamplerAudioProcessorEditor(*this);
}

juce::AudioProcessorValueTreeState::ParameterLayout SamplerAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;
    return layout;
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SamplerAudioProcessor();
}
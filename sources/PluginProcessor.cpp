#include "PluginProcessor.h"
#include "PluginEditor.h"


juce::SamplerSound* SamplerAudioProcessor::loadSound(const juce::String& name,
                                    int originalMidiNote,
                                    const std::vector<int>& midiNoteSet,
                                    const void* data,
                                    size_t sizeInBytes)
{
    // 1. We can pull all of this other stuff from the constructor to here.
    auto inputStream = std::make_unique<juce::MemoryInputStream>(data, sizeInBytes, false);

    if (auto reader = formatManager.createReaderFor(std::move(inputStream)))
    {
        juce::BigInteger midiNotes;

        for (auto note : midiNoteSet)
        {
            midiNotes.setBit(note);
        }

        return new juce::SamplerSound(name, *reader, midiNotes, originalMidiNote, 0.0, 0.1, 10.0);
    }

    return nullptr;
}


SamplerAudioProcessor::SamplerAudioProcessor() :
    AudioProcessor(BusesProperties().withOutput("Output", juce::AudioChannelSet::stereo(), true)),
    apvts(*this, nullptr, "Parameters", createParameterLayout())
{
    for (int i = 0; i < numVoices; ++i)
    {
        synth.addVoice(new juce::SamplerVoice);
    }

    formatManager.registerBasicFormats();

    synth.addSound(loadSound("C5", 72, { 24, 25, 36, 37, 48, 49, 60, 61, 72, 73 }, BinaryData::c5_wav, BinaryData::c5_wavSize));
    synth.addSound(loadSound("D5", 74, { 26, 27, 38, 39, 50, 51, 62, 63, 74, 75, 86, 87, 98, 99 }, BinaryData::d5_wav, BinaryData::d5_wavSize));
    synth.addSound(loadSound("E5", 76, { 28, 40, 52, 64, 76, 88, 100 }, BinaryData::e5_wav, BinaryData::e5_wavSize));
    synth.addSound(loadSound("F5", 77, { 29, 30, 41, 42, 53, 54, 65, 66, 77, 78, 89, 90, 101, 102 }, BinaryData::f5_wav, BinaryData::f5_wavSize));
    synth.addSound(loadSound("G5", 79, { 31, 32, 43, 44, 55, 56, 67, 68, 79, 80, 91, 92, 103, 104 }, BinaryData::g5_wav, BinaryData::g5_wavSize));
    synth.addSound(loadSound("A5", 81, { 21, 22, 33, 34, 45, 46, 57, 58, 69, 70, 81, 82, 93, 94, 105, 106 }, BinaryData::a5_wav, BinaryData::a5_wavSize));
    synth.addSound(loadSound("B5", 83, { 23, 35, 47, 59, 71, 83, 95, 107 }, BinaryData::b5_wav, BinaryData::b5_wavSize));
    synth.addSound(loadSound("C6", 84, { 84, 85, 96, 97, 108 }, BinaryData::c6_wav, BinaryData::c6_wavSize));
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
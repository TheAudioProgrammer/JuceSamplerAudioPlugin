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

    juce::AudioFormatManager formatManager;
    formatManager.registerBasicFormats();

    auto inputStream = std::make_unique<juce::MemoryInputStream>(BinaryData::c5_wav,
                                                               BinaryData::c5_wavSize,
                                                               false);

    if (auto reader = formatManager.createReaderFor(std::move(inputStream)))
    {
        const juce::String name = "C5";
        int originalMidiNote = 60;

        /* 1. Now let's demo how we can use this sample for multiple notes with a vector of notes we want the sample
         * to trigger for.  This is where the original midi note variable comes into play.  The engine will use this
         * to determine how much to pitch up / down the original sample to play in key. */
        std::vector<int> midiNoteSet = { 60, 61, 62, 63, 64, 65, 66, 67 };

        juce::BigInteger midiNotes;

        // Now we turn on all the midi notes we want to trigger with this sample
        for (auto note : midiNoteSet)
        {
            midiNotes.setBit(note);
        }

        auto sound = new juce::SamplerSound(name, *reader, midiNotes, originalMidiNote, 0.0, 0.1, 10.0);

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
#include "PluginProcessor.h"
#include "PluginEditor.h"


juce::SamplerSound* SamplerAudioProcessor::loadSound(const juce::String& name,
                                    int originalMidiNote,
                                    const std::vector<int>& midiNoteSet,
                                    const void* data,
                                    size_t sizeInBytes)
{
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
        midiPlaybackEngine.addVoice(new juce::SamplerVoice);
    }

    formatManager.registerBasicFormats();

    midiPlaybackEngine.addSound(loadSound("C5", 72, { 24, 25, 36, 37, 48, 49, 60, 61, 72, 73 }, BinaryData::c5_wav, BinaryData::c5_wavSize));
    midiPlaybackEngine.addSound(loadSound("D5", 74, { 26, 27, 38, 39, 50, 51, 62, 63, 74, 75, 86, 87, 98, 99 }, BinaryData::d5_wav, BinaryData::d5_wavSize));
    midiPlaybackEngine.addSound(loadSound("E5", 76, { 28, 40, 52, 64, 76, 88, 100 }, BinaryData::e5_wav, BinaryData::e5_wavSize));
    midiPlaybackEngine.addSound(loadSound("F5", 77, { 29, 30, 41, 42, 53, 54, 65, 66, 77, 78, 89, 90, 101, 102 }, BinaryData::f5_wav, BinaryData::f5_wavSize));
    midiPlaybackEngine.addSound(loadSound("G5", 79, { 31, 32, 43, 44, 55, 56, 67, 68, 79, 80, 91, 92, 103, 104 }, BinaryData::g5_wav, BinaryData::g5_wavSize));
    midiPlaybackEngine.addSound(loadSound("A5", 81, { 21, 22, 33, 34, 45, 46, 57, 58, 69, 70, 81, 82, 93, 94, 105, 106 }, BinaryData::a5_wav, BinaryData::a5_wavSize));
    midiPlaybackEngine.addSound(loadSound("B5", 83, { 23, 35, 47, 59, 71, 83, 95, 107 }, BinaryData::b5_wav, BinaryData::b5_wavSize));
    midiPlaybackEngine.addSound(loadSound("C6", 84, { 84, 85, 96, 97, 108 }, BinaryData::c6_wav, BinaryData::c6_wavSize));
}

bool SamplerAudioProcessor::isBusesLayoutSupported(const BusesLayout& layouts) const
{
    return layouts.getMainOutputChannelSet() == juce::AudioChannelSet::stereo();
}

void SamplerAudioProcessor::prepareToPlay(double newSampleRate, int maximumBlockSize)
{
    midiPlaybackEngine.setCurrentPlaybackSampleRate(newSampleRate);
}

void SamplerAudioProcessor::processBlock(juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    clearUnusedOutputChannels(buffer);
    midiPlaybackEngine.renderNextBlock(buffer, midiMessages, 0, buffer.getNumSamples());

    // 2. Now let's get the decay parameter from our parameter layout
    auto decay = apvts.getRawParameterValue("decay")->load();

    /* 3. Since our decay param is 0.0 - 100.0 but our envelope function expects a value of 0.1 - 1.0,
     * we will normalize our decay here. */
    auto normalized = decay * 0.01f;

    // 4. Now let's update each Sampler Sound with our new decay...
    for (int i = 0; i < midiPlaybackEngine.getNumSounds(); ++i)
    {
        if (auto* sound = dynamic_cast<juce::SamplerSound*>(midiPlaybackEngine.getSound(i).get()))
        {
            sound->setEnvelopeParameters({ 0.0f, decayTime, 0.0f, 0.05f });
        }
    }
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
    /* 1. We can use the GenericAudioProcessorEditor to show and test our parameters without needing to create
     * an entire custom UI */
    return new juce::GenericAudioProcessorEditor(*this);
}

juce::AudioProcessorValueTreeState::ParameterLayout SamplerAudioProcessor::createParameterLayout()
{
    juce::AudioProcessorValueTreeState::ParameterLayout layout;

    auto valueToPercentAsString = [](float value, int) { return juce::String(int(value)) + "%"; };

    layout.add(std::make_unique<juce::AudioParameterFloat>(
        juce::ParameterID { "decay", 1 },
        "Decay",
        juce::NormalisableRange<float>(0.0f, 100.0f),
        0.0f,
        juce::AudioParameterFloatAttributes().
        withStringFromValueFunction(valueToPercentAsString)));

    return layout;
}

juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new SamplerAudioProcessor();
}
#include "PluginEditor.h"

SamplerAudioProcessorEditor::SamplerAudioProcessorEditor(SamplerAudioProcessor& p) :
    AudioProcessorEditor(&p),
    audioProcessor(p)
{
    setOpaque(true);
    setSize(windowWidth, windowHeight);
    addAndMakeVisible(decayKnob);
}

SamplerAudioProcessorEditor::~SamplerAudioProcessorEditor()
{
}

void SamplerAudioProcessorEditor::paint(juce::Graphics& g)
{
    g.drawImage(Images::getBackground(), { 0, 0, windowWidth, windowHeight });
}

void SamplerAudioProcessorEditor::resized()
{
    decayKnob.setTopLeftPosition(451, 91);
}
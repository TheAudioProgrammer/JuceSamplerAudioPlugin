#include "PluginEditor.h"

SamplerAudioProcessorEditor::SamplerAudioProcessorEditor(SamplerAudioProcessor& p) :
    AudioProcessorEditor(&p),
    audioProcessor(p)
{
    setOpaque(true);

    // 1. We'll set the window size to 50% of the image size
    setSize(windowWidth, windowHeight);
}

SamplerAudioProcessorEditor::~SamplerAudioProcessorEditor()
{
}

void SamplerAudioProcessorEditor::paint(juce::Graphics& g)
{
    // 2. Now let's draw the image on the window
    g.drawImage(Images::getBackground(), { 0, 0, windowWidth, windowHeight });
}

void SamplerAudioProcessorEditor::resized()
{
}
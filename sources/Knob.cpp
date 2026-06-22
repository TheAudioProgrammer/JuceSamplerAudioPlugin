//
// Created by Joshua Hodge on 6/22/26.
//

#include "Knob.h"

Knob::Knob(const juce::String& text,
           juce::AudioProcessorValueTreeState& apvts,
           const juce::ParameterID& parameterID) :
    sliderLF(Images::getKnob(), 129), attachment(apvts, parameterID.getParamID(), slider)
{
    setSize(76, 107);

    slider.setSliderStyle(juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    slider.setTextBoxStyle(juce::Slider::TextBoxBelow, false, getWidth(), 14);
    slider.setLookAndFeel(&sliderLF);
    addAndMakeVisible(slider);

    label.setText(text, juce::NotificationType::dontSendNotification);
    label.setJustificationType(juce::Justification::horizontallyCentred);
    label.setFont(Fonts::getBoldFont());
    label.setColour(juce::Label::textColourId, Colors::textMid);
    addAndMakeVisible(label);
}

void Knob::resized()
{
    label.setBounds(0, 0, getWidth(), 19);
    slider.setBounds(0, 19, getWidth(), 88);
}

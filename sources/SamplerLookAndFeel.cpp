//
// Created by Joshua Hodge on 6/22/26.
//

#include "SamplerLookAndFeel.h"

KnobLookAndFeel::KnobLookAndFeel(const juce::Image& image, int frames)
: filmstrip(image), numFrames(frames)
{

}

void KnobLookAndFeel::drawRotarySlider(juce::Graphics& g,
    [[maybe_unused]] int x,
    [[maybe_unused]] int y,
    [[maybe_unused]] int width,
    [[maybe_unused]] int height,
    float sliderPosProportional,
    [[maybe_unused]] float rotaryStartAngle,
    [[maybe_unused]] float rotaryEndAngle,
    [[maybe_unused]] juce::Slider& slider)
{
    const int imageWidth = filmstrip.getWidth();
    const int imageHeight = filmstrip.getHeight() / numFrames;
    const int frameIndex = static_cast<int>(sliderPosProportional * static_cast<float>(numFrames - 1));

    g.drawImage(filmstrip, 0, 0, imageWidth / 2, imageHeight / 2,
                0, frameIndex * imageHeight, imageWidth, imageHeight);
}


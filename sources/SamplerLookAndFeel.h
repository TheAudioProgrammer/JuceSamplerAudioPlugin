//
// Created by Joshua Hodge on 5/5/26.
//

#pragma once

#include <juce_graphics/juce_graphics.h>
#include <BinaryData.h>

namespace Colors
{
    const juce::Colour textBright { 209, 209, 209 };
    const juce::Colour textMid { 178, 179, 178 };
    const juce::Colour textDim { 156, 157, 156 };
}

namespace Fonts
{
    static const juce::Typeface::Ptr typefaceRegular = juce::Typeface::createSystemTypefaceFor
    (BinaryData::InterRegular_ttf, BinaryData::InterRegular_ttfSize);
    static const juce::Typeface::Ptr typefaceMedium = juce::Typeface::createSystemTypefaceFor
    (BinaryData::InterMedium_ttf, BinaryData::InterMedium_ttfSize);
    static const juce::Typeface::Ptr typefaceBold = juce::Typeface::createSystemTypefaceFor
    (BinaryData::InterSemiBold_ttf, BinaryData::InterSemiBold_ttfSize);

    inline juce::Font getRegularFont(const float height = 14.0f)
    {
        return { juce::FontOptions(typefaceRegular).withHeight(height) };
    }

    inline juce::Font getMediumFont(const float height = 14.0f)
    {
        return { juce::FontOptions(typefaceBold).withHeight(height) };
    }

    inline juce::Font getBoldFont(const float height = 14.0f)
    {
        return { juce::FontOptions(typefaceBold).withHeight(height) };
    }
} // namespace Fonts

namespace Images
{
    inline juce::Image getBackground()
    {
        return juce::ImageCache::getFromMemory(BinaryData::BG_1376x1032_1_png, BinaryData::BG_1376x1032_1_pngSize);
    }

    inline juce::Image getKnob()
    {
        return juce::ImageCache::getFromMemory(BinaryData::Knob_152x132_129_png, BinaryData::Knob_152x132_129_pngSize);
    }
}
/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class VSTdistAudioProcessorEditor  : public juce::AudioProcessorEditor
{
public:
    VSTdistAudioProcessorEditor (VSTdistAudioProcessor&, juce::AudioProcessorValueTreeState&);
    ~VSTdistAudioProcessorEditor() override;

    //==============================================================================
    void paint (juce::Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    VSTdistAudioProcessor& audioProcessor;

    juce::Slider inputFilterSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> inputCutoffFrequencyAttachment;
    juce::Label inputFilterLabel;

    juce::Slider outputFilterSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> outputCutoffFrequencyAttachment;
    juce::Label outputFilterLabel;

    juce::Slider gainSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> gainAttachment;
    juce::Label gainLabel;

    juce::Slider volSlider;
    std::unique_ptr<juce::AudioProcessorValueTreeState::SliderAttachment> volAttachment;
    juce::Label volLabel;

    juce::ToggleButton siliconButton;
    std::unique_ptr < juce::AudioProcessorValueTreeState::ButtonAttachment> siliconAttachment;
    juce::Label siliconButtonLabel;

    juce::ToggleButton capButton;
    std::unique_ptr < juce::AudioProcessorValueTreeState::ButtonAttachment> capAttachment;
    juce::Label capButtonLabel;

    juce::ToggleButton softButton;
    std::unique_ptr < juce::AudioProcessorValueTreeState::ButtonAttachment> softAttachment;
    juce::Label softButtonLabel;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (VSTdistAudioProcessorEditor)
};

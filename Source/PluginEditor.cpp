/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
VSTdistAudioProcessorEditor::VSTdistAudioProcessorEditor (VSTdistAudioProcessor& p, juce::AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), audioProcessor (p)

{
    addAndMakeVisible(inputFilterSlider);
    inputFilterSlider.setSliderStyle(
        juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    inputCutoffFrequencyAttachment.reset(
        new juce::AudioProcessorValueTreeState::SliderAttachment(
            vts, "input_cutoff_frequency", inputFilterSlider));

    addAndMakeVisible(inputFilterLabel);
    inputFilterLabel.setText("Input Cutoff Freq.",
        juce::dontSendNotification);
    
       
    addAndMakeVisible(outputFilterSlider);
    outputFilterSlider.setSliderStyle(
        juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    outputCutoffFrequencyAttachment.reset(
        new juce::AudioProcessorValueTreeState::SliderAttachment(
            vts, "output_cutoff_frequency", outputFilterSlider));

    addAndMakeVisible(outputFilterLabel);
    outputFilterLabel.setText("Output Cutoff Freq.",
        juce::dontSendNotification);

    addAndMakeVisible(gainSlider);
    gainSlider.setSliderStyle(
        juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    gainAttachment.reset(
        new juce::AudioProcessorValueTreeState::SliderAttachment(
            vts, "gain", gainSlider));

    addAndMakeVisible(gainLabel);
    gainLabel.setText("Gain",
        juce::dontSendNotification);

    addAndMakeVisible(volSlider);
    volSlider.setSliderStyle(
        juce::Slider::SliderStyle::RotaryHorizontalVerticalDrag);
    volAttachment.reset(
        new juce::AudioProcessorValueTreeState::SliderAttachment(
            vts, "vol", volSlider));

    addAndMakeVisible(volLabel);
    volLabel.setText("Volume",
        juce::dontSendNotification);
    
    addAndMakeVisible(siliconButton);
    siliconAttachment.reset(
        new juce::AudioProcessorValueTreeState::
        ButtonAttachment(vts, "silicon", siliconButton));

    addAndMakeVisible(siliconButtonLabel);
    siliconButtonLabel.setText("1:Silicio 0:Germanio", juce::dontSendNotification);

    addAndMakeVisible(capButton);
    capAttachment.reset(
        new juce::AudioProcessorValueTreeState::
        ButtonAttachment(vts, "cap", capButton));

    addAndMakeVisible(capButtonLabel);
    capButtonLabel.setText("feedback cap", juce::dontSendNotification);

    addAndMakeVisible(softButton);
    softAttachment.reset(
        new juce::AudioProcessorValueTreeState::
        ButtonAttachment(vts, "soft", softButton));

    addAndMakeVisible(softButtonLabel);
    softButtonLabel.setText("Soft Clipping", juce::dontSendNotification);
    
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (580, 250);
}

VSTdistAudioProcessorEditor::~VSTdistAudioProcessorEditor()
{
}

//==============================================================================
void VSTdistAudioProcessorEditor::paint (juce::Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (juce::ResizableWindow::backgroundColourId));

    g.setColour (juce::Colours::white);
    g.setFont (15.0f);
    g.drawFittedText ("V2.0", getLocalBounds(), juce::Justification::bottomRight, 1);
}

void VSTdistAudioProcessorEditor::resized()
{
    inputFilterSlider.setBounds({ 15, 35, 100, 100 });
    inputFilterSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 20);
   inputFilterLabel.setBounds({ inputFilterSlider.getX() -10,
                                    inputFilterSlider.getY() - 40,
                                    200, 50 });

   outputFilterSlider.setBounds({ 315, 35, 100, 100 });
   outputFilterSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 20);
   outputFilterLabel.setBounds({ outputFilterSlider.getX() -10,
                                    outputFilterSlider.getY() - 40,
                                    200, 50 });

   gainSlider.setBounds({ 165, 35, 100, 100 });
   gainSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 20);
   gainLabel.setBounds({ gainSlider.getX() + 30 ,
                                    gainSlider.getY() - 40,
                                    200, 50 });

   volSlider.setBounds({ 465, 35, 100, 100 });
   volSlider.setTextBoxStyle(juce::Slider::TextEntryBoxPosition::TextBoxBelow, false, 50, 20);
   volLabel.setBounds({ volSlider.getX() + 20 ,
                                    volSlider.getY() - 40,
                                    200, 50 });


   siliconButton.setBounds(
       { gainSlider.getX(),
        gainSlider.getY() +
         gainSlider.getHeight() + 15,
        30, 50 });

   siliconButtonLabel.setBounds(
       { gainSlider.getX() + siliconButton.getWidth(),
        siliconButton.getY(),
        gainSlider.getWidth() - siliconButton.getWidth(),
        siliconButton.getHeight() });

   capButton.setBounds(
       { outputFilterSlider.getX(),
        outputFilterSlider.getY() +
         outputFilterSlider.getHeight() + 15,
        30, 50 });

   capButtonLabel.setBounds(
       { outputFilterSlider.getX() + capButton.getWidth(),
        capButton.getY(),
        outputFilterSlider.getWidth() - capButton.getWidth(),
        capButton.getHeight() });

   softButton.setBounds(
       { inputFilterSlider.getX(),
        inputFilterSlider.getY() +
         inputFilterSlider.getHeight() + 15,
        30, 50 });

   softButtonLabel.setBounds(
       { inputFilterSlider.getX() + softButton.getWidth() ,
        softButton.getY(),
        inputFilterSlider.getWidth() - softButton.getWidth(),
        softButton.getHeight() });

    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
}

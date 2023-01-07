/*
  ==============================================================================

    This file contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
VSTdistAudioProcessor::VSTdistAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
    : AudioProcessor(BusesProperties()
#if ! JucePlugin_IsMidiEffect
#if ! JucePlugin_IsSynth
        .withInput("Input", juce::AudioChannelSet::stereo(), true)
#endif
        .withOutput("Output", juce::AudioChannelSet::stereo(), true)
#endif
    ),
#endif
    parameters(*this,
        nullptr,
        juce::Identifier("Nosesiestoimporta"),
        { std::make_unique<juce::AudioParameterFloat>(
            "input_cutoff_frequency",
            "Input Cutoff Frequency",
            juce::NormalisableRange{20.f,
                                    20000.f,
                                    0.1f,
                                    0.2f,
                                    false},
            700.f),

        std::make_unique<juce::AudioParameterFloat>(
            "output_cutoff_frequency",
            "Output Cutoff Frequency",
            juce::NormalisableRange{20.f,
                                    20000.f,
                                    0.1f,
                                    0.2f,
                                    false},
            1500.f),

        std::make_unique<juce::AudioParameterFloat>(
            "gain",
            "Gain",
            juce::NormalisableRange{-10.f,
                                    45.f,
                                    0.1f,
                                    1.0f,
                                    false},
            0.f),

        std::make_unique<juce::AudioParameterFloat>(
            "vol",
            "Vol",
            juce::NormalisableRange{-60.f,
                                    24.f,
                                    0.1f,
                                    1.0f,
                                    false},
            0.f),


        std::make_unique<juce::AudioParameterBool>(
            "silicon",
            "Silicon",
            false),

        std::make_unique<juce::AudioParameterBool>(
            "cap",
            "Cap",
            false),
        
        std::make_unique<juce::AudioParameterBool>(
            "soft",
            "Soft",
            false)

        }) {

    inputCutoffFrequencyParameter =
        parameters.getRawParameterValue("input_cutoff_frequency");
    outputCutoffFrequencyParameter =
        parameters.getRawParameterValue("output_cutoff_frequency");
    gainParameter =
        parameters.getRawParameterValue("gain");
    volParameter =
        parameters.getRawParameterValue("vol");
    siliconParameter = parameters.getRawParameterValue("silicon");
    capParameter = parameters.getRawParameterValue("cap");
    softParameter = parameters.getRawParameterValue("soft");
}

VSTdistAudioProcessor::~VSTdistAudioProcessor()
{
}

//==============================================================================
const juce::String VSTdistAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool VSTdistAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool VSTdistAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool VSTdistAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double VSTdistAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int VSTdistAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int VSTdistAudioProcessor::getCurrentProgram()
{
    return 0;
}

void VSTdistAudioProcessor::setCurrentProgram (int index)
{
}

const juce::String VSTdistAudioProcessor::getProgramName (int index)
{
    return {};
}

void VSTdistAudioProcessor::changeProgramName (int index, const juce::String& newName)
{
}

//==============================================================================
void VSTdistAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    
    procesar.setSamplingRate(static_cast<float>(sampleRate));
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
}

void VSTdistAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool VSTdistAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    juce::ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    // Some plugin hosts, such as certain GarageBand versions, will only
    // load plugins that support stereo bus layouts.
    if (layouts.getMainOutputChannelSet() != juce::AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != juce::AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void VSTdistAudioProcessor::processBlock (juce::AudioBuffer<float>& buffer, juce::MidiBuffer& midiMessages)
{
    juce::ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());
    

    const auto inputCutoffFrequency = inputCutoffFrequencyParameter->load();
    const auto outputCutoffFrequency = outputCutoffFrequencyParameter->load();
    const auto gain = gainParameter->load();
    const auto vol = volParameter->load();
    const auto silicon = *siliconParameter < 0.5f ? false : true;
    const auto cap = *capParameter < 0.5f ? false : true;
    const auto soft = *softParameter < 0.5f ? false : true;
    
    procesar.setInputCutoffFrequency(inputCutoffFrequency);
    procesar.setOutputCutoffFrequency(outputCutoffFrequency);
    procesar.setGain(gain);
    procesar.setVol(vol);
    procesar.setSilicon(silicon);
    procesar.setCap(cap);
    procesar.setSoft(soft);
    
    procesar.processBlock(buffer, midiMessages);

}

//==============================================================================
bool VSTdistAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

juce::AudioProcessorEditor* VSTdistAudioProcessor::createEditor()
{
    return new VSTdistAudioProcessorEditor (*this, parameters);
}

//==============================================================================
void VSTdistAudioProcessor::getStateInformation (juce::MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.

    auto state = parameters.copyState();
    std::unique_ptr<juce::XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);
}

void VSTdistAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    std::unique_ptr<juce::XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(parameters.state.getType()))
            parameters.replaceState(juce::ValueTree::fromXml(*xmlState));

}

//==============================================================================
// This creates new instances of the plugin..
juce::AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new VSTdistAudioProcessor();
}

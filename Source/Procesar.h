#pragma once
#include <vector>
#include "JuceHeader.h"


class Procesar {
public:
	// setters
	void setSilicon(bool silicon);
	void setCap(bool cap);
	void setSoft(bool soft);
	void setInputCutoffFrequency(float inputCutoffFrequency);
	void setOutputCutoffFrequency(float OutputCutoffFrequency);
	void setGain(float gain);
	void setVol(float vol);
	void setSamplingRate(float samplingRate);

	void processBlock(juce::AudioBuffer<float>&, juce::MidiBuffer&);

private:
	bool silicon;
	bool cap;
	bool soft;
	float inputCutoffFrequency;
	float outputCutoffFrequency;
	float gain;
	float vol;
	float samplingRate;

	std::vector<float> dnBuffer;
	std::vector<float> dnBuffer2;
	std::vector<float> dnBuffer3;

};
#include "Procesar.h"

void Procesar::setSilicon(bool silicon) {
	this->silicon = silicon;
}

void Procesar::setCap(bool cap) {
	this->cap = cap;
}

void Procesar::setSoft(bool soft) {
	this->soft = soft;
}


void Procesar::setInputCutoffFrequency(float inputCutoffFrequency) {
	this->inputCutoffFrequency = inputCutoffFrequency;
}

void Procesar::setOutputCutoffFrequency(float outputCutoffFrequency) {
	this->outputCutoffFrequency = outputCutoffFrequency;
}
void Procesar::setGain(float gain) {
	this->gain = gain;
}

void Procesar::setVol(float vol) {
	this->vol = vol;
}

void Procesar::setSamplingRate(float samplingRate) {
	this->samplingRate = samplingRate;
}


void Procesar::processBlock(juce::AudioBuffer<float>& buffer,
	juce::MidiBuffer&) {


	constexpr auto PI = 3.14159265359f;

	dnBuffer.resize(buffer.getNumChannels(), 0.f);
	dnBuffer2.resize(buffer.getNumChannels(), 0.f);
	dnBuffer3.resize(buffer.getNumChannels(), 0.f);

	const auto tan = std::tan(PI * inputCutoffFrequency / samplingRate);
	const auto a1 = (tan - 1.f) / (tan + 1.f);

	const auto tanOutput = std::tan(PI * outputCutoffFrequency / samplingRate);
	const auto a1Output = (tanOutput - 1.f) / (tanOutput + 1.f);

	const auto tanC = std::tan(PI * 1500 / samplingRate);
	const auto a1C = (tanC - 1.f) / (tanC + 1.f);
	/*
	//modelo del clipeo de diodo 1n4148 y 10k (DOD 250)
	float vInSilicio[15] = { 0,0.075,0.181,0.246,0.406,0.534,0.681,0.893,1.130,1.360,1.780,2.250,2.840,3.500,5.020 };
	float vOutSilicio[15] = { 0,0.075,0.179,0.238,0.358,0.401,0.433,0.460,0.479,0.494,0.512,0.526,0.541,0.553,0.572 };
	//modelo del clipeo diodo de germanio y 10k (distortion +)
	float vInGermanio[15] = { 0,0.060,0.096,0.175,0.238,0.353,0.431,0.707,0.850,1.190,1.750,2.520,3.050,4.370,5.020 };
	float vOutGermanio[15] = { 0,0.040,0.059,0.086,0.100,0.119,0.128,0.151,0.160,0.177,0.198,0.221,0.234,0.262,0.275 };

	float vIn[15] = {};
	float vOut[15] = {};



	if (silicon == 1)
	{
		for (int i = 0; i < 15; i++)
		{
			vIn[i] = vInSilicio[i];
			vOut[i] = vOutSilicio[i];
		}

	}
	else
	{
		for (int i = 0; i < 15; i++)
		{
			vIn[i] = vInGermanio[i];
			vOut[i] = vOutGermanio[i];
		}

	}
	*/

	gain = juce::Decibels::decibelsToGain(gain, -60.0f);
	vol = juce::Decibels::decibelsToGain(vol, -60.0f);

	for (auto channel = 0; channel < buffer.getNumChannels(); ++channel) {
		auto channelSamples = buffer.getWritePointer(channel);
		for (auto i = 0; i < buffer.getNumSamples(); ++i) {

			//Pasa altos de entrada
			const auto inputSample = channelSamples[i];
			const auto allpassFilteredSample = a1 * inputSample +
				dnBuffer[channel];
			dnBuffer[channel] = inputSample - a1 * allpassFilteredSample;
			const auto filterOutput =
				0.5f * (inputSample + -1.f * allpassFilteredSample);

			float filterOutput3;

			if (cap) //si hay capacitor
			{
				//Pasa bajos (capacitor de 100pF)
				const auto allpassFilteredSample3 = a1C * filterOutput +
					dnBuffer3[channel];
				dnBuffer3[channel] = filterOutput - a1C * allpassFilteredSample3;
				filterOutput3 =
					0.5f * (filterOutput + 1.f * allpassFilteredSample3);
			}
			else 
			{
				filterOutput3 = filterOutput;
			}

			//Distorsion

			float señal =  gain * filterOutput3;
			//float auxiliar;
			float outDistor = 0;

			/*
			int signo = 0;  //supongo positivo

			
			if (señal < 0) {
				signo = 1;
				señal = -señal;
			}

			if (señal >= vIn[14])
				señal = vIn[14];  //simula hard-clipping del AO

			for (int j = 0; j <= 13; j++)    //interpolo 
			{
				if (señal >= vIn[j] && señal <= vIn[j + 1])
				{
					auxiliar = ((vOut[j] - vOut[j + 1]) / ((vIn[j] - vIn[j + 1])) * (señal - vIn[j])) + vOut[j];

					if (signo)
						auxiliar = -auxiliar;

					if (soft) //soft clipping
						auxiliar += 2*filterOutput3; //agrego un poco de la señal 

					outDistor = auxiliar ;
				}
			}
			*/

			//en lugar de tabla uso paroximaciones a la curva



			if (señal >= 5.0f)
				señal = 5.0f;  //simula hard-clipping del AO
			if (señal <= -5.0f)
				señal = -5.0f;  //simula hard-clipping del AO

			if (silicon == 1)
			{
				outDistor = 0.355 * atan(3.6 * señal) + señal / 140; 
			}
			else
			{
				outDistor = 0.115 * atan(3.5 * señal) + señal / 50;

			}


			//Pasa bajos de salida
			const auto allpassFilteredSample2 = a1Output * outDistor +
				dnBuffer2[channel];
			dnBuffer2[channel] = outDistor - a1Output * allpassFilteredSample2;
			const auto filterOutput2 =
				0.5f * (outDistor + allpassFilteredSample2);


			channelSamples[i] = vol*filterOutput2;
		}
	}

}
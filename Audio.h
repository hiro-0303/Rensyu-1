#pragma once
#include "Global.h"
#include <xact3.h>

class Audio
{
	IXACT3Engine*		xactEngine;
	IXACT3WaveBank*		waveBank;
	IXACT3SoundBank*	soundBank;
	void*			mapWaveBank;
	void*			soundBankData;

	HRESULT InitEngine();
	HRESULT LoadWaveBank(char* fileName);
	HRESULT LoadSoundBank(char* fileName);
public:
	Audio();
	~Audio();

	HRESULT Load(char* waveBankFileName, char* soundBankFileName);
	HRESULT Audio::Play(char* cueName);
	HRESULT Audio::Stop(char* cueName);
};


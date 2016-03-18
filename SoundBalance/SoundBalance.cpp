// SoundBalance.cpp : I have Unilateral hearing loss (UHL). In order for me to hear sound at the
// same volume in each ear I need to either lower the volume on the left channel or increase the
// volume on the right channel. This program solves my problem by toggling between an even sound
// balance (when using speakers) or 29% on left channel (when using headphones).
//

#include "stdafx.h"
#include <stdio.h>
#include <windows.h>
#include <mmdeviceapi.h>
#include <endpointvolume.h>

#define PERCENTAGE .29f

void openEndpoint();
void closeEndpoint();
float getMasterVolume();
float getLeftVolume();
float getRightVolume();
void setLeftVolume(const float const &volume);
void setRightVolume(const float const &volume);

IAudioEndpointVolume *endpointVolume = NULL;

int main(int argc, char* argv[])
{
	openEndpoint();
	float master = getMasterVolume();
	float left = getLeftVolume();
	float right = getRightVolume();


	float l = 0;
	float r = 0;
	// if sound balance is even, set left channel to 'PERCENTAGE' of the master volume.
	if (left == right)
	{
		l = master * PERCENTAGE;
		r = master;
		setLeftVolume(l);
		setRightVolume(r);
	}
	else
	{
		l = master;
		r = master;
		setLeftVolume(l);
		setRightVolume(r);
	}

	printf("Sound Balance: [ %.0f%% | %.0f%% ]\n", l * 100, r * 100);
	closeEndpoint();
	return 0;
}

// Gets the left channel's volume
float getLeftVolume() {
	float left = 0;
	endpointVolume->GetChannelVolumeLevelScalar(0, &left);
	return left;
}

// Gets the right channel's volume
float getRightVolume() {
	float right = 0;
	endpointVolume->GetChannelVolumeLevelScalar(1, &right);
	return right;
}

// Gets the master volume
float getMasterVolume() {
	float master = 0;
	endpointVolume->GetMasterVolumeLevelScalar(&master);
	return master;
}

// Sets the left channel's volume
void setLeftVolume(const float const &volume) {
	endpointVolume->SetChannelVolumeLevelScalar(0, volume, NULL);
}

// Sets the right channel's volume
void setRightVolume(const float const &volume) {
	endpointVolume->SetChannelVolumeLevelScalar(1, volume, NULL);
}

// Initializes the endpointVolume
void openEndpoint() {
	CoInitialize(NULL);
	IMMDeviceEnumerator *deviceEnumerator = NULL;
	CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_INPROC_SERVER, __uuidof(IMMDeviceEnumerator), (LPVOID *)&deviceEnumerator);
	IMMDevice *defaultDevice = NULL;

	deviceEnumerator->GetDefaultAudioEndpoint(eRender, eConsole, &defaultDevice);
	deviceEnumerator->Release();
	deviceEnumerator = NULL;

	endpointVolume = NULL;
	defaultDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_INPROC_SERVER, NULL, (LPVOID *)&endpointVolume);
	defaultDevice->Release();
	defaultDevice = NULL;
}

// Closes the endpointVolume
void closeEndpoint() {
	endpointVolume->Release();
	CoUninitialize();
}

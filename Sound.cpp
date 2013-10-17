/*
	This file is part of the Sound library.
	Copyright (C) 2008-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2008-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2008-2012 Ralf Petring <ralf@petring.net>
	
	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the 
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "Sound.h"
#include "Buffer.h"
#include "Listener.h"
#include "Source.h"
#include "SoundInternals.h"

#include <SDL.h>

#include <Util/Macros.h>
#include <Util/References.h>

#include <algorithm>
#include <cstddef>
#include <iostream>
#include <cstdint>
#include <vector>

namespace Sound {

static ALCdevice *myDevice=nullptr;
static ALCcontext * myContext=nullptr;
//    if (alcIsExtensionPresent(nullptr, "ALC_ENUMERATION_EXT") != AL_TRUE) {
//        std::cout << "Enumeration not available!";
//        return;
//    }

//    // defaultDeviceName contains the name of the default
//    // device
//    const ALCchar *defaultDeviceName;
//    defaultDeviceName = alcGetString(nullptr, ALC_DEFAULT_DEVICE_SPECIFIER);
//    std::cout << "\nDevice:"<< defaultDeviceName ;

//! [static]
std::string getDeviceName(){
	const ALCchar *actualDeviceName;
	actualDeviceName = alcGetString(myDevice, ALC_DEVICE_SPECIFIER);
	return std::string(actualDeviceName);
}

//! [static]
bool checkErrorStatus(const char * file, int line, const std::string & msg) {
	const ALenum error = alGetError();
	if (error == AL_NO_ERROR) {
		return true;
	}
	std::cerr << "AL ERROR (";
	const ALchar * errorString = alGetString(error);
	if(errorString != nullptr) {
		std::cerr << errorString;
	} else {
		std::cerr << error;
	}
	std::cerr << "): " << msg << " at " << file << ":" << line << std::endl;
	return false;
}

static void checkALCErrorStatus(const char * file, int line, ALCdevice * device) {
	const ALCenum error = alcGetError(device);
	if(error == ALC_NO_ERROR) {
		return;
	}
	std::cerr << "ALC ERROR (";
	const ALchar * errorString = alcGetString(device, error);
	if(errorString != nullptr) {
		std::cerr << errorString;
	} else {
		std::cerr << error;
	}
	std::cerr << ") at " << file << ":" << line << std::endl;
}

//! [static]
bool initSoundSystem(){
	if(myContext != nullptr) { // already initialized?
		return true;
	}

	// Open the default device
	myDevice = alcOpenDevice(nullptr);
	checkALCErrorStatus(__FILE__, __LINE__, myDevice);
	if(myDevice == nullptr) {
		return false;
	}

	// Create Context
	myContext = alcCreateContext(myDevice, nullptr);
	checkALCErrorStatus(__FILE__, __LINE__, myDevice);
	if(myContext == nullptr) {
		return false;
	}

	alcMakeContextCurrent(myContext);
	checkALCErrorStatus(__FILE__, __LINE__, myDevice);

	if(!checkErrorStatus(__FILE__, __LINE__, "initSoundSystem()")){
		shutDownSoundSystem();
		return false;
	}
	return true;
}

//! [static]
void shutDownSoundSystem(){
	alcMakeContextCurrent(nullptr);
	alcDestroyContext(myContext);
	alcCloseDevice(myDevice);
	myContext = nullptr;
	myDevice = nullptr;
}

//! [static]
Listener * getListener()	{
	return Listener::getInstance();
}

//! [static]
Source * createSource(){
	return Source::create();
}

//! [static] factory
Buffer * createNoise(unsigned int freq, unsigned int size) {
	Util::Reference<Buffer> b(Buffer::create());
	if(b.isNull())
		return nullptr;

	std::vector<uint8_t> noise;
	noise.reserve(size);
	unsigned int j = 1;
	for (unsigned int i = 0; i < size; ++i) {
		j = (j * 1234393 + i) % 0xffffff;
		noise.push_back(static_cast<uint8_t>(j) % 256);
	}
	b->setData(AL_FORMAT_MONO8, noise.data(), noise.size(),freq);

	return checkErrorStatus(__FILE__, __LINE__) ? b.detachAndDecrease() : nullptr;
}

//! [static] factory
Buffer * createRectangleSound(unsigned int width, unsigned int freq, unsigned int size) {
	Util::Reference<Buffer> b(Buffer::create());
	if(b.isNull())
		return nullptr;

	std::vector<uint8_t> noise;
	noise.reserve(size);
	uint8_t value = 0;
	for (unsigned int i = 0; i < size; ++i) {
		if(i % width == 0) {
			value = 255 - value;
		}
		noise.push_back(value);
	}
	b->setData(AL_FORMAT_MONO8, noise.data(), noise.size(), freq);

	return checkErrorStatus(__FILE__, __LINE__) ? b.detachAndDecrease() : nullptr;
}

//! [static] factory
Buffer * createSilence(unsigned int freq,unsigned int size) {
	Util::Reference<Buffer> b(Buffer::create());
	if(b.isNull())
		return nullptr;

	const std::vector<uint8_t> data(size, 128);
	b->setData(AL_FORMAT_MONO8, data.data(), data.size(), freq);

	return checkErrorStatus(__FILE__, __LINE__) ? b.detachAndDecrease() : nullptr;
}

//! [static] factory
Buffer * loadWAV(const std::string & filename) {
	Util::Reference<Buffer> b(Buffer::create());
	if(b.isNull())
		return nullptr;

	SDL_AudioSpec wav_spec;
	Uint32 wav_length;
	Uint8 *wav_buffer;

	if ( SDL_LoadWAV(filename.c_str(), &wav_spec, &wav_buffer, &wav_length) == nullptr ) {
		WARN("Could not open : "+ filename+ std::string( SDL_GetError()) );
		return nullptr;
	}

	unsigned int format=0;
	unsigned int freq = static_cast<unsigned int>(wav_spec.freq);
	if (wav_spec.format == AUDIO_U8 ) {
		if (wav_spec.channels==1)
			format=AL_FORMAT_MONO8;
		else
			format=AL_FORMAT_STEREO8;
	} else if (wav_spec.format == AUDIO_S16SYS ) {
		if (wav_spec.channels==1)
			format=AL_FORMAT_MONO16;
		else
			format=AL_FORMAT_STEREO16;
	}
	if (format==0) {
		WARN("Uninplemented WAV-format. Please add conversion routine ;).");
		return nullptr;
	} else
		b->setData(format,wav_buffer,wav_length,freq);

//    b->setData(AL_FORMAT_MONO8,noise,size,freq);
	SDL_FreeWAV(wav_buffer);

	return checkErrorStatus(__FILE__, __LINE__) ? b.detachAndDecrease() : nullptr;
}

}

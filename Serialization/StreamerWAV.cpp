/*
	This file is part of the Util library.
	Copyright (C) 2007-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2007-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2007-2012 Ralf Petring <ralf@petring.net>
	Copyright (C) 2019 Sascha Brandt <sascha@brandt.graphics>
	
	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the 
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "StreamerWAV.h"
#include "Serialization.h"
#include "../Buffer.h"
#include "../SoundInternals.h"
#include "../Sound.h"
#include <Util/Factory/Factory.h>
#include <Util/IO/FileName.h>
#include <Util/IO/FileUtils.h>
#include <Util/IO/TemporaryDirectory.h>
#include <Util/Macros.h>
#include <Util/StringUtils.h>
#include <Util/LibRegistry.h>
#include <cstddef>

#define DR_WAV_IMPLEMENTATION
#define DR_WAV_NO_STDIO
#include <dr_wav.h>

static bool libNameInitailized = [](){
	Util::LibRegistry::registerLibVersionString("dr_wav","dr_wav v0.10.1 (http://mackron.github.io/dr_libs)"); 
	return true;
}();

namespace Sound {

struct ReadContext {
	ReadContext(std::istream& in) : in(in) {
		in.seekg(0, std::ios::end);
		size = in.tellg();
		in.seekg(0, std::ios::beg);
	}
	std::istream& in;
	size_t size;
};

static inline size_t read_iostream(void* pUserData, void* pBufferOut, size_t bytesToRead) {
	ReadContext& ctx = *static_cast<ReadContext*>(pUserData);
	size_t bytesRead = std::min(bytesToRead, ctx.size - ctx.in.tellg());
	if(ctx.in.read(static_cast<char*>(pBufferOut), bytesRead))
		return bytesRead;
	WARN("Error while reading stream.");
	return 0;
}

static inline drwav_bool32 seek_iostream(void* pUserData, int offset, drwav_seek_origin origin) {
	ReadContext& ctx = *static_cast<ReadContext*>(pUserData);
	return ctx.in.seekg(offset, (origin == drwav_seek_origin_current) ? std::ios_base::cur : std::ios_base::beg) ? true : false;
}

Util::Reference<Buffer> StreamerWAV::loadAudio(std::istream & input) {
	Util::Reference<Buffer> b(Buffer::create());
	if(b.isNull()) {
		return nullptr;
	}
	
	ReadContext ctx(input);		
	drwav wav;
	if(!drwav_init(&wav, &read_iostream, &seek_iostream, &ctx)) {
		WARN("Error reading WAV data.");
		return nullptr;
	}
	
	std::vector<int16_t> wav_buffer(wav.totalPCMFrameCount * wav.channels);
	drwav_uint64 read_frames = drwav_read_pcm_frames_s16(&wav, wav.totalPCMFrameCount, wav_buffer.data());
	size_t wav_length = read_frames * wav.channels * sizeof(int16_t);	
	b->setData((wav.channels==1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16, wav_buffer.data(), wav_length, wav.sampleRate);
	
	drwav_uninit(&wav);
	return b;
}

bool StreamerWAV::init() {
	Serialization::registerAudioLoader("wav", Util::ObjectCreator<StreamerWAV>());
	return true;
}

}

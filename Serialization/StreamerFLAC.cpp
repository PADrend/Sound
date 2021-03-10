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
#include "StreamerFLAC.h"
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

#define DR_FLAC_IMPLEMENTATION
#define DR_FLAC_NO_STDIO
#include <dr_flac.h>

static bool libNameInitailized = [](){
	Util::LibRegistry::registerLibVersionString("dr_flac","dr_flac v0.12.0 (http://mackron.github.io/dr_libs)"); 
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

static inline drflac_bool32 seek_iostream(void* pUserData, int offset, drflac_seek_origin origin) {
	ReadContext& ctx = *static_cast<ReadContext*>(pUserData);
	return ctx.in.seekg(offset, (origin == drflac_seek_origin_current) ? std::ios_base::cur : std::ios_base::beg) ? true : false;
}

Util::Reference<Buffer> StreamerFLAC::loadAudio(std::istream & input) {
	Util::Reference<Buffer> b(Buffer::create());
	if(b.isNull()) {
		return nullptr;
	}
	
	ReadContext ctx(input);
	
	drflac_uint32 channels;
	drflac_uint32 sampleRate;
	drflac_uint64 frameCount;
	drflac_int16* data = drflac_open_and_read_pcm_frames_s16(&read_iostream, &seek_iostream, &ctx, &channels, &sampleRate, &frameCount, nullptr);
	if(!data) {
		WARN("Error reading FLAC data.");
		return nullptr;
	}
	
	size_t size = frameCount * channels * sizeof(int16_t);	
	b->setData((channels==1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16, data, static_cast<uint32_t>(size), sampleRate);
	
	DRFLAC_FREE(data);
	return b;
}

bool StreamerFLAC::init() {
	Serialization::registerAudioLoader("flac", Util::ObjectCreator<StreamerFLAC>());
	return true;
}

}

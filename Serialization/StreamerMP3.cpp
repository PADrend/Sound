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
#include "StreamerMP3.h"
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

#define DR_MP3_IMPLEMENTATION
#define DR_MP3_NO_STDIO
#include <dr_mp3.h>

static bool libNameInitailized = [](){
	Util::LibRegistry::registerLibVersionString("dr_mp3","dr_mp3 v0.4.7 (http://mackron.github.io/dr_libs)"); 
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

static inline drmp3_bool32 seek_iostream(void* pUserData, int offset, drmp3_seek_origin origin) {
	ReadContext& ctx = *static_cast<ReadContext*>(pUserData);
	return ctx.in.seekg(offset, (origin == drmp3_seek_origin_current) ? std::ios_base::cur : std::ios_base::beg) ? true : false;
}

Util::Reference<Buffer> StreamerMP3::loadAudio(std::istream & input) {
	Util::Reference<Buffer> b(Buffer::create());
	if(b.isNull()) {
		return nullptr;
	}
	
	ReadContext ctx(input);
	drmp3_config config = {0,0};
	drmp3_uint64 frameCount;
	drmp3_int16* data = drmp3_open_and_read_s16(&read_iostream, &seek_iostream, &ctx, &config, &frameCount);	
	if(!data) {
		WARN("Error reading mp3 data.");
		return nullptr;
	}
	
	size_t mp3_size = frameCount * config.outputChannels * sizeof(int16_t);	
	b->setData((config.outputChannels==1) ? AL_FORMAT_MONO16 : AL_FORMAT_STEREO16, data, mp3_size, config.outputSampleRate);
	
	drmp3_free(data);
	return b;
}

bool StreamerMP3::init() {
	Serialization::registerAudioLoader("mp3", Util::ObjectCreator<StreamerMP3>());
	return true;
}

}

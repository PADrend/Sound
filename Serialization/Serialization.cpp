/*
	This file is part of the Sound library.
	Copyright (C) 2007-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2007-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2007-2012 Ralf Petring <ralf@petring.net>
	Copyright (C) 2019 Sascha Brandt <sascha@brandt.graphics>
	
	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the 
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "Serialization.h"
#include "AbstractAudioStreamer.h"
#include "../Buffer.h"
#include <Util/Factory/Factory.h>
#include <Util/IO/FileName.h>
#include <Util/IO/FileUtils.h>
#include <Util/Macros.h>
#include <algorithm>
#include <cctype>
#include <cstddef>
#include <memory>
#include <cstdint>

namespace Sound {
namespace Serialization {
using namespace Util;

typedef Factory<AbstractAudioStreamer *, std::string, std::function<AbstractAudioStreamer * ()>, FallbackPolicies::NULLFallback> streamer_factory_t;

//! Local singleton function for factory creating streams loading bitmaps
static streamer_factory_t & getLoaderFactory() {
	static streamer_factory_t factory;
	return factory;
}

bool registerAudioLoader(const std::string & extension, std::function<AbstractAudioStreamer * ()> loaderCreator) {
	return getLoaderFactory().registerType(extension, loaderCreator);
}

static std::string toLower(const std::string & extension) {
	std::string lowerExtension(extension);
	std::transform(extension.begin(), extension.end(), lowerExtension.begin(), ::tolower);
	return lowerExtension;
}

Reference<Buffer> loadAudio(const Util::FileName & url) {
	std::unique_ptr<AbstractAudioStreamer> loader(getLoaderFactory().create(toLower(url.getEnding())));
	if (loader.get() == nullptr) {
		WARN("No loader available.");
		return nullptr;
	}
	auto stream = FileUtils::openForReading(url);
	if(!stream) {
		WARN("Error opening stream for reading. Path: " + url.toString());
		return nullptr;
	}
	return loader->loadAudio(*stream);
}

Reference<Buffer> loadAudio(const std::string & extension, const std::string & data) {
	std::unique_ptr<AbstractAudioStreamer> loader(getLoaderFactory().create(toLower(extension)));
	if (loader.get() == nullptr) {
		WARN("No loader available.");
		return nullptr;
	}
	std::istringstream stream(data);
	return loader->loadAudio(stream);
}

}
}

/*
	This file is part of the Sound library.
	Copyright (C) 2019 Sascha Brandt <sascha@brandt.graphics>
	
	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the 
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef SOUND_SERIALIZATION_H
#define SOUND_SERIALIZATION_H

#include <Util/References.h>
#include <functional>
#include <iosfwd>
#include <string>

namespace Util {
class FileName;
} /* Util */

namespace Sound {
class AbstractAudioStreamer;
class Buffer;

namespace Serialization {
	
/**
 * Load a single audio file from the given address.
 * The type of the audio file is determined by the file extension.
 *
 * @param file Address to the file containing the audio data.
 * @return A single audio file.
 */
Util::Reference<Buffer> loadAudio(const Util::FileName & url);

/**
 * Create a audio buffer from the given data.
 * The type of the bitmap has to be given as parameter.
 *
 * @param extension File extension specifying the type of the bitmap.
 * @param data Bitmap data.
 * @return A single bitmap.
 */
Util::Reference<Buffer> loadAudio(const std::string & extension, const std::string & data);

/**
 * Register a new streamer for the given file extension that supports loading of bitmaps.
 *
 * @param extension File extension specifying the type of the bitmap.
 * @param loaderCreator A function that creates a new streamer when called.
 * @retval @c true if the streamer was registered successfully.
 * @retval @c false if another streamer for the extension already existed.
 */
bool registerAudioLoader(const std::string & extension, std::function<AbstractAudioStreamer * ()> loaderCreator);

} /* Serialization */
} /* Sound */

#endif // SOUND_SERIALIZATION_H

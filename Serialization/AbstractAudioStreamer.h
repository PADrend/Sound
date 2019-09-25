/*
	This file is part of the Util library.
	Copyright (C) 2019 Sascha Brandt <sascha@brandt.graphics>
	
	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the 
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef SOUND_ABSTRACTAUDIOSTREAMER_H_
#define SOUND_ABSTRACTAUDIOSTREAMER_H_

#include <Util/Serialization/AbstractStreamer.h>
#include <Util/Macros.h>
#include <Util/References.h>
#include <cstdint>
#include <iosfwd>
#include <string>

namespace Sound {
class Buffer;

/**
 * Interface for classes that are capable of converting between audio data and streams.
 */
class AbstractAudioStreamer : public Util::AbstractStreamer {
	public:
		virtual ~AbstractAudioStreamer() {
		}

		/**
		 * Load audio data from the given stream.
		 *
		 * @param input Use the data from the stream beginning at the preset position.
		 * @return Buffer object. The caller is responsible for the memory deallocation.
		 */
		virtual Util::Reference<Buffer> loadAudio(std::istream & /*input*/) = 0;

	protected:
		//! Creation is only possible in subclasses.
		AbstractAudioStreamer() : AbstractStreamer() {
		}
};

}

#endif /* SOUND_ABSTRACTAUDIOSTREAMER_H_ */

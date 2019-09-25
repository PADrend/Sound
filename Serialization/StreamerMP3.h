/*
	This file is part of the Sound library.
	Copyright (C) 2019 Sascha Brandt <sascha@brandt.graphics>
	
	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the 
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef SOUND_STREAMERMP3_H_
#define SOUND_STREAMERMP3_H_

#include "AbstractAudioStreamer.h"
#include <Util/References.h>
#include <cstdint>
#include <iosfwd>
#include <string>

namespace Sound {

class StreamerMP3 : public AbstractAudioStreamer {
	public:
		StreamerMP3() : AbstractAudioStreamer() { }
		virtual ~StreamerMP3() = default;

		Util::Reference<Buffer> loadAudio(std::istream & input) override;

		static bool init();
};
}

#endif /* SOUND_STREAMERMP3_H_ */

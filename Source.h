/*
	This file is part of the Sound library.
	Copyright (C) 2008-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2008-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2008-2012 Ralf Petring <ralf@petring.net>
	
	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the 
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef SOURCE_H
#define SOURCE_H

#include "Buffer.h"

#include <Util/References.h>
#include <Util/ReferenceCounter.h>

#include <deque>
#include <cstdint>

namespace Sound {

/***
 **  Source
 **/
class Source : public Util::ReferenceCounter<Source> {
	public:
		SOUNDAPI static Source * create();
		SOUNDAPI ~Source();

		uint32_t getSourceId()const				{	return sourceId;	}


		SOUNDAPI void enqueueBuffer(Buffer * buffer);
		SOUNDAPI void dequeueBuffers(int num);

		SOUNDAPI void setAttribute_i(unsigned int attrib,int value);
		SOUNDAPI void setAttribute_f(unsigned int attrib,float value);
		SOUNDAPI void setAttribute_3f(unsigned int attrib,float v1,float v2,float v3);
		SOUNDAPI void getAttribute_3f(unsigned int attrib,float & v1,float & v2,float & v3);
		SOUNDAPI int getAttribute_i(unsigned int attrib)const;

		// ---
		SOUNDAPI void play();
		SOUNDAPI void stop();
		SOUNDAPI void pause();
		SOUNDAPI void rewind();

		// ---

		SOUNDAPI void setPitch(float f);
		SOUNDAPI void setGain(float f);
		SOUNDAPI void setMinGain(float f);
		SOUNDAPI void setMaxGain(float f);
		SOUNDAPI void setRolloffFactor(float f);
		SOUNDAPI void setConeOuterGain(float f);
		SOUNDAPI void setConeInnerAngle(float f);
		SOUNDAPI void setConeOuterAngle(float f);
		SOUNDAPI void setReferenceDistance(float f);

		SOUNDAPI void setPosition(float x,float y,float z);
		SOUNDAPI void setVelocity(float x,float y,float z);
		SOUNDAPI void setDirection(float x,float y,float z);

		SOUNDAPI void setSourceIsRelative(bool b);
		SOUNDAPI void setLooping(bool b);

		SOUNDAPI bool isLooping()const;
		SOUNDAPI bool isPaused()const;
		SOUNDAPI bool isPlaying()const;
		SOUNDAPI bool isStopped()const;
		SOUNDAPI bool isSourceRelative()const;

		SOUNDAPI int getNumBuffersQueued()const;
		SOUNDAPI int getNumBuffersProcessed()const;
		SOUNDAPI int getSourceState()const;
		SOUNDAPI void getDirection(float &x,float &y,float& z);
		SOUNDAPI void getPosition(float &x,float &y,float& z);
		SOUNDAPI void getVelocity(float &x,float &y,float& z);

	private:
		Source(uint32_t _sourceId);
		uint32_t sourceId;
		std::deque<Util::Reference<Buffer> > buffers;
};
}
#endif // SOURCE_H

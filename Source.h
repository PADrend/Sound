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
		static Source * create();
		~Source();

		uint32_t getSourceId()const				{	return sourceId;	}


		void enqueueBuffer(Buffer * buffer);
		void dequeueBuffers(int num);

		void setAttribute_i(unsigned int attrib,int value);
		void setAttribute_f(unsigned int attrib,float value);
		void setAttribute_3f(unsigned int attrib,float v1,float v2,float v3);
		void getAttribute_3f(unsigned int attrib,float & v1,float & v2,float & v3);
		int getAttribute_i(unsigned int attrib)const;

		// ---
		void play();
		void stop();
		void pause();
		void rewind();

		// ---

		void setPitch(float f);
		void setGain(float f);
		void setMinGain(float f);
		void setMaxGain(float f);
		void setRolloffFactor(float f);
		void setConeOuterGain(float f);
		void setConeInnerAngle(float f);
		void setConeOuterAngle(float f);
		void setReferenceDistance(float f);

		void setPosition(float x,float y,float z);
		void setVelocity(float x,float y,float z);
		void setDirection(float x,float y,float z);

		void setSourceIsRelative(bool b);
		void setLooping(bool b);

		bool isLooping()const;
		bool isPaused()const;
		bool isPlaying()const;
		bool isStopped()const;
		bool isSourceRelative()const;

		int getNumBuffersQueued()const;
		int getNumBuffersProcessed()const;
		int getSourceState()const;
		void getDirection(float &x,float &y,float& z);
		void getPosition(float &x,float &y,float& z);
		void getVelocity(float &x,float &y,float& z);

	private:
		Source(uint32_t _sourceId);
		uint32_t sourceId;
		std::deque<Util::Reference<Buffer> > buffers;
};
}
#endif // SOURCE_H

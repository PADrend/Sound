/*
	This file is part of the Sound library.
	Copyright (C) 2008-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2008-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2008-2012 Ralf Petring <ralf@petring.net>
	
	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the 
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "Buffer.h"
#include "Sound.h"
#include "SoundInternals.h"
#include <cstddef>
#include <sstream>

namespace Sound {

//! [static] factory
Buffer * Buffer::create() {
	checkErrorStatus(__FILE__, __LINE__);
	uint32_t bufferId;
	alGenBuffers( 1, &bufferId );
	if(!checkErrorStatus(__FILE__, __LINE__, "Could not create Buffer.") || bufferId==0 )
		return nullptr;
	return new Buffer(bufferId);
}

//! [ctor]
Buffer::Buffer(uint32_t _bufferId):bufferId(_bufferId) {
	//ctor
}

//! [dtor]
Buffer::~Buffer() {
	if(bufferId>0) {
		alDeleteSources(1, &bufferId);
		bufferId=0;
	}
//	std::cout << " ~buffer ";
	//dtor
}

void Buffer::setData(unsigned int format,const void * buffer,unsigned int byte,unsigned int freq) {
	alBufferData(bufferId, static_cast<ALenum>(format), buffer, static_cast<ALsizei>(byte), static_cast<ALsizei>(freq));
}

std::string Buffer::toString()const {
	std::ostringstream s;
	s << "Buffer("<<getBufferId()<<","<<getChannels()<<"ch,"  <<getFrequency()<<"hz,"<<getDataSize()<<"byte)";
	return s.str();
}

int Buffer::getAttribute_i(unsigned int attrib)const {
	int v=0;
	alGetBufferi(bufferId, static_cast<ALenum>(attrib), &v);
	return v;
}

int Buffer::getFrequency()const 			{	return getAttribute_i(AL_FREQUENCY);	}
int Buffer::getBits()const 					{	return getAttribute_i(AL_BITS);	}
int Buffer::getChannels()const 				{	return getAttribute_i(AL_CHANNELS);	}
int Buffer::getDataSize()const 					{	return getAttribute_i(AL_SIZE);	}

}

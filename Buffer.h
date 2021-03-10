/*
	This file is part of the Sound library.
	Copyright (C) 2008-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2008-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2008-2012 Ralf Petring <ralf@petring.net>
	
	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the 
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef BUFFER_H
#define BUFFER_H

#include <Util/ReferenceCounter.h>
#include <cstdint>
#include <string>

namespace Sound {

/***
 **  Buffer
 **/
class Buffer : public Util::ReferenceCounter<Buffer> {
	public:
		SOUNDAPI static Buffer * create();
		SOUNDAPI ~Buffer();

		uint32_t getBufferId()const		{	return bufferId;	}

		SOUNDAPI std::string toString()const;

		// ---
		SOUNDAPI int getAttribute_i(unsigned int attrib)const;
		SOUNDAPI void setData(unsigned int format,const void*buffer,unsigned int byte,unsigned int freq);

		SOUNDAPI int getFrequency()const;
		SOUNDAPI int getBits()const;
		SOUNDAPI int getChannels()const;
		SOUNDAPI int getDataSize()const;

	private:
		uint32_t bufferId;

		Buffer(uint32_t _bufferId);
};
}
#endif // BUFFER_H

/*
	This file is part of the Sound library.
	Copyright (C) 2008-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2008-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2008-2012 Ralf Petring <ralf@petring.net>
	
	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the 
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef SOUND_H
#define SOUND_H

#include <string>

namespace Sound {
class Buffer;
class Listener;
class Source;

bool initSoundSystem();
void shutDownSoundSystem();

bool checkErrorStatus(const char * file, int line, const std::string & msg = "");

std::string getDeviceName();

Listener * getListener();
Source * createSource();
Buffer * createNoise(unsigned int freq,unsigned int size);
Buffer * createRectangleSound(unsigned int width, unsigned int freq,unsigned int size);
Buffer * createSilence(unsigned int freq,unsigned int size);
Buffer * loadWAV(const std::string & filename);

}

#endif // SOUND_H

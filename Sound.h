/*
	This file is part of the Sound library.
	Copyright (C) 2008-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2008-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2008-2012 Ralf Petring <ralf@petring.net>
	Copyright (C) 2019 Sascha Brandt <sascha@brandt.graphics>
	
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

SOUNDAPI bool initSoundSystem();
SOUNDAPI void shutDownSoundSystem();

SOUNDAPI bool checkErrorStatus(const char * file, int line, const std::string & msg = "");

SOUNDAPI std::string getDeviceName();

SOUNDAPI Listener * getListener();
SOUNDAPI Source * createSource();
SOUNDAPI Buffer * createNoise(unsigned int freq,unsigned int size);
SOUNDAPI Buffer * createRectangleSound(unsigned int width, unsigned int freq,unsigned int size);
SOUNDAPI Buffer * createSilence(unsigned int freq,unsigned int size);

}

#endif // SOUND_H

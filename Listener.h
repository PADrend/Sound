/*
	This file is part of the Sound library.
	Copyright (C) 2008-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2008-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2008-2012 Ralf Petring <ralf@petring.net>
	
	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the 
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#ifndef SOUND_LISTENER_H
#define SOUND_LISTENER_H

namespace Sound {

/***
 **  Listener
 **/
class Listener{
	public:
		SOUNDAPI static Listener * getInstance();

		SOUNDAPI int getDistanceModel();
		SOUNDAPI float getDopplerFactor();
		SOUNDAPI float getGain();
		SOUNDAPI void getOrientation(float & dirX,float & dirY,float & dirZ,float & upX,float & upY,float & upZ);
		SOUNDAPI void getPosition(float & x,float & y,float & z);
		SOUNDAPI float getSpeedOfSound();
		SOUNDAPI void getVelocity(float & x,float & y,float & z);
		SOUNDAPI void setDistanceModel(int modelType);
		SOUNDAPI void setDopplerFactor(float f);
		SOUNDAPI void setGain(float f);
		SOUNDAPI void setOrientation(float dirX,float dirY,float dirZ,float upX,float upY,float upZ);
		SOUNDAPI void setPosition(float x,float y,float z);
		SOUNDAPI void setSpeedOfSound(float f);
		SOUNDAPI void setVelocity(float x,float y,float z);

	private:
		Listener();
		~Listener();
};
}
#endif // SOUND_LISTENER_H

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
		static Listener * getInstance();

		int getDistanceModel();
		float getDopplerFactor();
		float getGain();
		void getOrientation(float & dirX,float & dirY,float & dirZ,float & upX,float & upY,float & upZ);
		void getPosition(float & x,float & y,float & z);
		float getSpeedOfSound();
		void getVelocity(float & x,float & y,float & z);
		void setDistanceModel(int modelType);
		void setDopplerFactor(float f);
		void setGain(float f);
		void setOrientation(float dirX,float dirY,float dirZ,float upX,float upY,float upZ);
		void setPosition(float x,float y,float z);
		void setSpeedOfSound(float f);
		void setVelocity(float x,float y,float z);

	private:
		Listener();
		~Listener();
};
}
#endif // SOUND_LISTENER_H

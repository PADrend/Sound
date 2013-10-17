/*
	This file is part of the Sound library.
	Copyright (C) 2008-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2008-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2008-2012 Ralf Petring <ralf@petring.net>
	
	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the 
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "Listener.h"
#include "SoundInternals.h"

namespace Sound {

//! [static] singleton
Listener * Listener::getInstance() {
	static auto instance(new Listener);
	return instance;
}

//! [ctor]
Listener::Listener(){
	//ctor
}

//! [dtor]
Listener::~Listener(){
	//dtor
}

void Listener::setGain(float f){
	alListenerf(AL_GAIN,f);
}

void Listener::setPosition(float x,float y,float z){
	alListener3f( AL_POSITION,x,y,z);
}

void Listener::setVelocity(float x,float y,float z){
	alListener3f( AL_VELOCITY,x,y,z);
}

void Listener::setOrientation(float dirX,float dirY,float dirZ,float upX,float upY,float upZ){
	float v[]={dirX,dirY,dirZ,upX,upY,upZ};
	alListenerfv( AL_ORIENTATION,v );
}

float Listener::getGain(){
	float v=0;
	alGetListenerf(AL_GAIN,&v);
	return v;
}

void Listener::getPosition(float & x,float & y,float & z){
	alGetListener3f(AL_POSITION,&x,&y,&z);
}

void Listener::getVelocity(float & x,float & y,float & z){
	alGetListener3f(AL_VELOCITY,&x,&y,&z);
}

void Listener::getOrientation(float & dirX,float & dirY,float & dirZ,float & upX,float & upY,float & upZ){
	float v[6];
	alGetListenerfv( AL_ORIENTATION,v );
	dirX=v[0],	dirY=v[1],	dirZ=v[2];
	upX=v[3],	upY=v[4],	upZ=v[5];
}

void Listener::setDopplerFactor(float f){
	alDopplerFactor(f);
}

void Listener::setSpeedOfSound(float f){
	alSpeedOfSound(f);
}

void Listener::setDistanceModel(int modelType){
	alDistanceModel(modelType);
}

float Listener::getDopplerFactor(){
	float v=0;
	alGetListenerf(AL_DOPPLER_FACTOR,&v);
	return v;
}

float Listener::getSpeedOfSound(){
	float v=0;
	alGetListenerf(AL_SPEED_OF_SOUND,&v);
	return v;
}

int Listener::getDistanceModel(){
	int v=0;
	alGetListeneri(AL_DISTANCE_MODEL,&v);
	return v;
}

}

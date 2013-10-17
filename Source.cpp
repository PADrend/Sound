/*
	This file is part of the Sound library.
	Copyright (C) 2008-2012 Benjamin Eikel <benjamin@eikel.org>
	Copyright (C) 2008-2012 Claudius Jähn <claudius@uni-paderborn.de>
	Copyright (C) 2008-2012 Ralf Petring <ralf@petring.net>
	
	This library is subject to the terms of the Mozilla Public License, v. 2.0.
	You should have received a copy of the MPL along with this library; see the 
	file LICENSE. If not, you can obtain one at http://mozilla.org/MPL/2.0/.
*/
#include "Source.h"
#include "SoundInternals.h"
#include "Buffer.h"
#include "Sound.h"
#include <Util/References.h>
#include <cstddef>

namespace Sound {

//! [static] factory
Source * Source::create() {
	checkErrorStatus(__FILE__, __LINE__);
	uint32_t sourceId;
	alGenSources( 1, &sourceId );
	if(!checkErrorStatus(__FILE__, __LINE__, "Could not create Source.") || sourceId==0 ){
		return nullptr;
	}
	return new Source(sourceId);
}

//! [ctor]
Source::Source(uint32_t _sourceId):sourceId(_sourceId){
	//ctor
}

//! [dtor]
Source::~Source(){
	if (sourceId!=0){
		alDeleteSources(1, &sourceId);
		sourceId=0;
	}
//	std::cout << " ~source ";

	//dtor
}

void Source::enqueueBuffer(Buffer * buffer){
	ALuint bId=buffer->getBufferId();
	alSourceQueueBuffers(sourceId,1,&bId);
	buffers.push_back(buffer);
}

void Source::dequeueBuffers(int num){
	checkErrorStatus(__FILE__, __LINE__);
	auto removedBuffers=new ALuint[num];
	alSourceUnqueueBuffers(sourceId,num,removedBuffers);
	delete []removedBuffers;
	if(checkErrorStatus(__FILE__, __LINE__, "Source::dequeueBuffers")){ // successful?
		buffers.erase(buffers.begin(),buffers.begin()+num);
	}
}

void Source::play(){
	alSourcePlay( sourceId );
}

void Source::stop(){
	alSourceStop( sourceId );
}

void Source::pause(){
	alSourcePause( sourceId );
}

void Source::rewind(){
	alSourceRewind( sourceId );
}

void Source::setAttribute_f(unsigned int attrib,float value){
	alSourcef(sourceId, static_cast<ALenum>(attrib), value);
}

void Source::setAttribute_i(unsigned int attrib,int value){
	alSourcei( sourceId, static_cast<ALenum>(attrib),value );
}

void Source::setAttribute_3f(unsigned int attrib,float v1,float v2,float v3){
	alSource3f( sourceId,static_cast<ALenum>(attrib),v1,v2,v3 );
}

void Source::getAttribute_3f(unsigned int attrib,float & v1,float & v2,float & v3){
	alGetSource3f( sourceId,static_cast<ALenum>(attrib),&v1,&v2,&v3 );
}

int Source::getAttribute_i(unsigned int attrib)const{
	int v=0;
	alGetSourcei(sourceId,static_cast<ALenum>(attrib),&v);
	return v;
}
void Source::setPitch(float f)						{	setAttribute_f(AL_PITCH,f);	}
void Source::setGain(float f)						{	setAttribute_f(AL_GAIN,f);	}
void Source::setMinGain(float f)					{	setAttribute_f(AL_MIN_GAIN,f);	}
void Source::setMaxGain(float f)					{	setAttribute_f(AL_MAX_GAIN,f);	}
void Source::setRolloffFactor(float f)				{	setAttribute_f(AL_ROLLOFF_FACTOR,f);	}
void Source::setConeOuterGain(float f)				{	setAttribute_f(AL_CONE_OUTER_GAIN,f);	}
void Source::setConeInnerAngle(float f)				{	setAttribute_f(AL_CONE_INNER_ANGLE,f);	}
void Source::setConeOuterAngle(float f)				{	setAttribute_f(AL_CONE_OUTER_ANGLE,f);	}
void Source::setReferenceDistance(float f) 			{	setAttribute_f(AL_REFERENCE_DISTANCE,f);	}

void Source::setPosition(float x,float y,float z) 	{	setAttribute_3f(AL_POSITION,x,y,z);	}
void Source::setVelocity(float x,float y,float z) 	{	setAttribute_3f(AL_VELOCITY,x,y,z);	}
void Source::setDirection(float x,float y,float z)	{	setAttribute_3f(AL_DIRECTION,x,y,z);	}

void Source::setSourceIsRelative(bool b)			{	setAttribute_i(AL_SOURCE_RELATIVE,b?AL_TRUE:AL_FALSE);	}
void Source::setLooping(bool b)						{	setAttribute_i(AL_LOOPING,b?AL_TRUE:AL_FALSE);	}

bool Source::isLooping()const						{	return getAttribute_i(AL_LOOPING)==AL_TRUE;	}
bool Source::isSourceRelative()const				{	return getAttribute_i(AL_SOURCE_RELATIVE)==AL_TRUE;	}
bool Source::isPlaying()const						{	return getAttribute_i(AL_SOURCE_STATE)==AL_PLAYING;	}
bool Source::isPaused()const						{	return getAttribute_i(AL_SOURCE_STATE)==AL_PAUSED;	}
bool Source::isStopped()const						{	return getAttribute_i(AL_SOURCE_STATE)==AL_STOPPED;	}

int Source::getNumBuffersQueued()const				{	return getAttribute_i(AL_BUFFERS_QUEUED);	}
int Source::getNumBuffersProcessed()const			{	return getAttribute_i(AL_BUFFERS_PROCESSED);	}
int Source::getSourceState()const					{	return getAttribute_i(AL_SOURCE_STATE);	}
void Source::getDirection(float &x,float &y,float& z) {	getAttribute_3f(AL_DIRECTION,x,y,z);	}
void Source::getPosition(float &x,float &y,float& z) {	getAttribute_3f(AL_POSITION,x,y,z);	}
void Source::getVelocity(float &x,float &y,float& z) {	getAttribute_3f(AL_VELOCITY,x,y,z);	}

}

#include "oscmodel.hpp"
#include "common.hpp"

const unsigned int OscModel::freq_max = 120;
const unsigned int OscModel::glide_max = 200;
const unsigned int OscModel::level_max = 100;
const int OscModel::tune_max = 50;
const int OscModel::tune_min = -50;

const unsigned int AnalogOscModel::width_max = 99;

const unsigned int DigitalOscModel::shape_max = 127;
const unsigned int DigitalOscModel::fm_in_max = 100;
const unsigned int DigitalOscModel::ring_in_max = 100;

OscModel::OscModel(QObject * parent): QObject(parent){
	mFreq = 64;
	mGlide = 0;
	mLevel = 50;
	mTune = 0;
}

void OscModel::set_freq(unsigned int freq){
	if(in_range_and_new<unsigned int>(freq, mFreq, freq_max)){
		mFreq = freq;
		emit(freq_changed(mFreq));
	}
}


void OscModel::set_tune(int tune){
	if(in_range_and_new<int>(tune, mTune, tune_max, tune_min)){
		mTune = tune;
		emit(tune_changed(mTune));
	}
}

void OscModel::set_glide(unsigned int glide){
	if(in_range_and_new<unsigned int>(glide, mGlide, glide_max)){
		mGlide = glide;
		emit(glide_changed(mGlide));
	}
}

void OscModel::set_level(unsigned int level){
	if(in_range_and_new<unsigned int>(level, mLevel, level_max)){
		mLevel = level;
		emit(level_changed(mLevel));
	}
}


AnalogOscModel::AnalogOscModel(QObject * parent): OscModel(parent){
	mShape = saw;
	mWidth = 50;
}

void AnalogOscModel::set_shape(shapes shape){
	if(shape != mShape){
		mShape = shape;
		emit(shape_changed(mShape));
	}
}

void AnalogOscModel::set_width(unsigned int width){
	if(in_range_and_new<unsigned int>(width, mWidth, 99)){
		mWidth = width;
		emit(width_changed(mWidth));
	}
}

void AnalogOscModel::sync(bool s){
	if(s != mSyncing){
		mSyncing = s;
		emit(sync_changed(mSyncing));
	}
}


DigitalOscModel::DigitalOscModel(QObject * parent): OscModel(parent){
	mShape = 0;
	mFmIn = 0;
	mRingIn = 0;
	mShapeMod = none;
}


void DigitalOscModel::set_shape(unsigned int shape){
	if(in_range_and_new<unsigned int>(shape, mShape, 127)){
		mShape = shape;
		emit(shape_changed(mShape));
	}
}

void DigitalOscModel::set_fm_in(unsigned int fm){
	if(in_range_and_new<unsigned int>(fm, mFmIn, 100)){
		mFmIn = fm;
		emit(fm_in_changed(mFmIn));
	}
}

void DigitalOscModel::set_ring_in(unsigned int ring){
	if(in_range_and_new<unsigned int>(ring, mRingIn, 100)){
		mRingIn = ring;
		emit(ring_in_changed(mRingIn));
	}
}

void DigitalOscModel::set_shape_mod(shape_mod mod){
	if(mod != mShapeMod){
		mShapeMod = mod;
		emit(shape_mod_changed(mShapeMod));
	}
}


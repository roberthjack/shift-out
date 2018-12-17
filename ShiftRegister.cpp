/***** ShiftRegister.cpp *****/

#include "ShiftRegister.h"

ShiftRegister::ShiftRegister(){};

ShiftRegister::ShiftRegister(unsigned int dataPin, unsigned int clockPin, unsigned int latchPin, unsigned int maxSize){
	setup (dataPin, clockPin, latchPin, maxSize);
};

bool ShiftRegister::setup(unsigned int dataPin, unsigned int clockPin, unsigned int latchPin, unsigned int maxSize)
{
	mData_Pin = dataPin;
	mClock_Pin = clockPin;
	mLatch_Pin = latchPin;
	data.resize(maxSize);
	return true;
}

bool ShiftRegister::dataSent()
{
	return gState == kIdle;
}

void ShiftRegister::render(BelaContext* context)
{
	for(unsigned int n = 0; n < context->digitalFrames; ++n)
	{
		render(context, n);
	}
}

void ShiftRegister::render(BelaContext* context, unsigned int n)
{
	if(!pinModeSet)
	{
		pinMode(context, 0, mData_Pin,OUTPUT);
		pinMode(context, 0, mClock_Pin,OUTPUT);
		pinMode(context, 0, mLatch_Pin,OUTPUT);
		pinModeSet = true;
	}
	bool latchValue = 0;
	bool dataValue = 0;
	bool clockValue = 0;
	if(gState == kStart)
	{
		latchValue = 1;
		dataValue = 0;
		clockValue = 0;
		gState = kTransmitting;
		gCurrentDataFrame = 0;
	} else if(gState == kTransmitting)
	{
		latchValue = 1;
		dataValue = data[gCurrentDataFrame / 2];
		if((gCurrentDataFrame % 2) == 0)
		{
			clockValue = 0;
		} else {
			clockValue = 1;
		}
		gCurrentDataFrame++;
		if(gCurrentDataFrame == 2 * currentLength)
		{
			gState = kStop;
		}
	} else if(gState == kStop || gState == kIdle)
	{
		latchValue = 0;
		dataValue = 0;
		clockValue = 0;
		gState = kIdle;
	}
	digitalWriteOnce(context, n, mLatch_Pin, latchValue);
	digitalWriteOnce(context, n, mData_Pin, dataValue);
	digitalWriteOnce(context, n, mClock_Pin, clockValue);
}

void ShiftRegister::sendData(bool* dataBuffer, unsigned int length)
{
	// should we check for length and expand `data` if needed? That wouldn't be RT-safe
	currentLength = std::min(length, data.size());
	for(unsigned int n = 0; n < currentLength; ++n)
	{
		data[n] = dataBuffer[n];
	}
	gState = kStart;
}

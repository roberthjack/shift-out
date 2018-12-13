/***** ShiftOut.cpp *****/

#include <ShiftOut.h>

ShiftRegister::ShiftRegister(){};

ShiftRegister::ShiftRegister(BelaContext* context, unsigned int dataPin, unsigned int clockPin, unsigned int latchPin, unsigned int maxSize){
	setup (context, dataPin, clockPin, latchPin, maxSize);
};

bool ShiftRegister::setup(BelaContext* context, unsigned int dataPin, unsigned int clockPin, unsigned int latchPin, unsigned int maxSize)
{
	mData_Pin = dataPin;
	mClock_Pin = clockPin;
	mLatch_Pin = latchPin;
	data.resize(maxSize);
	pinMode(context, 0, mData_Pin,OUTPUT);
    pinMode(context, 0, mClock_Pin,OUTPUT);
	pinMode(context, 0, mLatch_Pin,OUTPUT);
	return true;
}

bool ShiftRegister::dataSent()
{
	return gState == kIdle;
}

void ShiftRegister::render(BelaContext* context, int number_of_bits)
{
	for(unsigned int n = 0; n < context->digitalFrames; ++n)
	{
		render(context, n, number_of_bits);
	}
}

void ShiftRegister::render(BelaContext* context, unsigned int n, int number_of_bits)
{
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
		if(gCurrentDataFrame == 2 * number_of_bits)
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

void ShiftRegister::sendData(bool* dataBuffer, unsigned int length, unsigned int startingFrame)
{
	// should we check for length and expand `data` if needed? That wouldn't be RT-safe
	for(unsigned int n = 0; n < std::min(length, data.size()); ++n)
	{
		data[n] = dataBuffer[n];
	}
	gState = kStart;
}
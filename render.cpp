/***** render.cpp *****/

#include <Bela.h>
#include <ShiftOut.h>

// Number of chips used
#define TotalIC 3
#define number_of_bits TotalIC * 8

// Array for storing the values that are written to the register
bool data[number_of_bits] = {1,0,1,0,1,0,1,0};

//Pin DS of 74HC595
int mData_Pin = 3;
//Pin ST_CP of 74HC595
int mLatch_Pin = 4;
//Pin SH_CP of 74HC595
int mClock_Pin = 5;

// For cycling through the LEDs
int gCounter = 0;
int interval = 2000;
int litLED = 0;

ShiftRegister shiftRegister;


bool setup(BelaContext *context, void *userData)
{
	shiftRegister.setup(context, mData_Pin, mClock_Pin, mLatch_Pin, number_of_bits);
	return true;
}


void render(BelaContext *context, void *userData)
{
	
	// Push reading to register at the first frame of the next available block
	// initialise in setup with the mLatch/mData/mClock pins
	// if(shiftRegister.dataSent())
	// 	shiftRegister.sendData(data, number_of_bits, 0);
	// shiftRegister.render(context, number_of_bits);
	
	// Push reading to register at the first frame possible
	for(unsigned int n = 0; n < context->digitalFrames; ++n)
	{
		shiftRegister.render(context, n, number_of_bits);
		if(shiftRegister.dataSent())
			shiftRegister.sendData(data, number_of_bits, n);
	}

	
	// ========CYCLE THROUGH LEDS=========
	if(gCounter > interval){
		for (int j=0;j<number_of_bits;j++){
			// Clear all LEDs
			data[j] = 0;
		}
		data[litLED] = 1;
		litLED += 1;
		if(litLED >= number_of_bits){
			litLED = 0;
		}
		gCounter = 0;
		rt_printf("Interval passed, litLED %d\n", litLED);
	}
	
	gCounter += context->audioFrames;
	// ===================================
	
}

void cleanup(BelaContext *context, void *userData)
{

}
/***** ShiftOut.h *****/

#include <Bela.h>
#include <vector>

class ShiftRegister 
{
	private:
		unsigned int mData_Pin;
		unsigned int mClock_Pin;
		unsigned int mLatch_Pin;
		
		enum {
			kStart,
			kTransmitting,
			kStop,
			kIdle,
		};
		
		int gState = kStop;
		int gCurrentDataFrame = 0;
		std::vector<bool> data;
		unsigned int currentLength = 0;
		bool pinModeSet = false;

	public:

		ShiftRegister();
		ShiftRegister(unsigned int dataPin, unsigned int clockPin, unsigned int latchPin, unsigned int maxSize);
		bool setup(unsigned int dataPin, unsigned int clockPin, unsigned int latchPin, unsigned int maxSize);
		bool dataSent();
		void render(BelaContext* context);
		void render(BelaContext* context, unsigned int n);
		void sendData(bool* dataBuffer, unsigned int length);

};

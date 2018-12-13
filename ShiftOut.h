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


	public:

		ShiftRegister();
		ShiftRegister(BelaContext* context, unsigned int dataPin, unsigned int clockPin, unsigned int latchPin, unsigned int maxSize);
		bool setup(BelaContext* context, unsigned int dataPin, unsigned int clockPin, unsigned int latchPin, unsigned int maxSize);
		bool dataSent();
		void render(BelaContext* context, int number_of_bits);
		void render(BelaContext* context, unsigned int n, int number_of_bits);
		void sendData(bool* dataBuffer, unsigned int length, unsigned int startingFrame);

};
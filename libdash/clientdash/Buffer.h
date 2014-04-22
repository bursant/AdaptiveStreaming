#pragma once
#include <queue>
#include "IBufferItem.h"
#include "IBuffer.h"
#include "Multithreading.h"

using namespace std;

class Buffer : public IBuffer
{
	public:
		Buffer(int buffer_size);
		~Buffer();
		void push(IBufferItem* item);
		IBufferItem* front();
		void pop();
		void setEnd(bool value);
		uint32_t size();

	private:
		queue<IBufferItem *> buffer;
        bool end;
        uint32_t buffer_size;
		mutable CRITICAL_SECTION monitorMutex;
		mutable CONDITION_VARIABLE full;
		mutable CONDITION_VARIABLE empty;
};
#pragma once
#include <queue>
#include "IBufferItem.h"
#include "IBuffer.h"

using namespace std;

class Buffer : public IBuffer
{
	public:
		Buffer(int size);
		~Buffer();
		void push(IBufferItem* item);
		IBufferItem* front();
		void pop();
		void setEnd(bool value);
		int size();

	private:
		queue<IBufferItem *> buffer;
        bool end;
        int size;
}
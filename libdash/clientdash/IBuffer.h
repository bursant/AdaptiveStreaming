#pragma once
#include <queue>
#include "IBufferItem.h"

class IBuffer
{
	public:
		virtual void push(IBufferItem* item) = 0;
		virtual IBufferItem* front() = 0;
		virtual void pop() = 0;
		virtual void setEnd(bool value) = 0;
		virtual int size() = 0;
}
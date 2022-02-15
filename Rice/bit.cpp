#include "bit.h"

void bitStream::writeBit(int x)
{
	// индекс блока в памяти для значения (сдвиг на 3 тк uint8_t)
	uint32_t index = this->bitPos >> 3;

	if (index < this->numBytes)
	{
		// битовая магия 🐱‍👤
		uint32_t bit = 7 - (this->bitPos & 7);
		uint32_t mask = 0xff ^ (1 << bit);
		uint32_t set = (x & 1) << bit;
		this->bytePoint[index] = (this->bytePoint[index] & mask) | set;
		++this->bitPos;
	}
}

int bitStream::readBit()
{
	uint32_t x;
	uint32_t index = this->bitPos >> 3;

	if (index < this->numBytes)
	{
		uint32_t bit = 7 - (this->bitPos & 7);
		x = (this->bytePoint[index] >> bit) & 1;
		++this->bitPos;
	}
	else
	{
		x = 0;
	}

	return x;
}

bitStream::bitStream(uint8_t* buffer, uint32_t bytes) :
	bytePoint(buffer),
	bitPos(0),
	numBytes(bytes)
{}

bitStream::bitStream(uint8_t * bytePoint, uint32_t bitPos, uint32_t numBytes) :
	bytePoint(bytePoint),
	bitPos(bitPos),
	numBytes(numBytes)
{}

void bitStream::setPos(uint32_t pos)
{
	bitPos = pos;
}

uint32_t bitStream::getPos()
{
	return bitPos;
}


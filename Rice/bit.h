#pragma once
#include <cstdint>

class bitStream {
private:
	uint8_t* bytePoint;
	uint32_t bitPos;
	uint32_t numBytes;
public:
	bitStream(uint8_t* buffer, uint32_t bytes);
	bitStream(uint8_t* bytePoint, uint32_t bitPos, uint32_t numBytes);

	void setPos(uint32_t pos);
	uint32_t getPos();

	void writeBit(int x);
	int readBit();
};

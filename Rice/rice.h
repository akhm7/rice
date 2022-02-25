#pragma once
#include <cstdint>
#include <fstream>
#include <vector>

#include "bit.h"



class rice
{
private:
	const int _history = 16; // ��� ���������� ��������������� �������� ��� ���������� mean
	const int _threshold = 8; // ����� ����������� �����, ������ ����������� ������ �� ������

	int _inputSize = 0;

	bitStream* stream;

	uint8_t* _originalData;
	uint8_t* _compressedData;
	uint8_t* _decompressedData;


	const char* _outputEncodeFile = "compressed.bin";
	const char* _outputDecodeFile = "decompressed.bin";


	int numBits(uint32_t x)
	{
		/* 0x80000000 � ��������������� ��������, � ������� ������� ���������� ���� ��� �� �����
		�� ���� ����, ��� ��� ����, ����� ������ ������, ����� ���������� �������� & ����� ����� 0,
		� ����� ����� ��� ��������� ��� ���������.
		����� ���� �������� �������� (32), �������� ������ ��������� ���������� ����� ������� �������
		�������� �����. (���� ������ ����� ������ ��� uint32_t, �� ���������� ����.)
		*/

		int n;
		for (n = 32; !(x & 0x80000000) && (n > 0); --n)
			x <<= 1;
		return n;
	}

public:
	rice(const char* inputFile);
	// � ���� ����� ����� setPos(8) ������������ ��� �������� ������� �� 8. �.�. ������� ������ �������� k
	// ��������� ��� ����������� �������������. ���� ��������� �������, ������� � �� ����������, � �������
	// ��� ������������� ���������� ������� ������ ������������� ������� �����, ��� "���������" �� �������,
	// ������ (� ������� ������ ������ �������� �����), �� ���������� � ��������� ���, �� ���������� �����.
	void encodeWord(uint32_t x, int k);
	uint32_t decodeWord(int k);

	int compress(uint8_t* input, uint8_t* output);
	void decompress(uint8_t* input, uint8_t* output, uint32_t inputSize, uint32_t outputSize);

};


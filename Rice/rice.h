#pragma once
#include <cstdint>
#include <fstream>
#include <vector>

#include "bit.h"



class rice
{
private:
	const int _history = 16; // мин количество рассматриваемых значений для вычисления mean
	const int _threshold = 8; // порог кодируемого слова, ставим ограничение выхода за предел

	int _inputSize = 0;

	bitStream* stream;

	uint8_t* _originalData;
	uint8_t* _compressedData;
	uint8_t* _decompressedData;


	const char* _outputEncodeFile = "compressed.bin";
	const char* _outputDecodeFile = "decompressed.bin";


	int numBits(uint32_t x)
	{
		/* 0x80000000 – вспомогательное значение, с помощью которой выделяется один бит из числа
		за счет того, что все биты, кроме самого левого, после применения операции & будут равны 0,
		а самый левый бит останется без изменений.
		Знаем макс битность значения (32), обратным циклом вычисляем количество битов битовым сдвигом
		текущего числа. (если задано число больше чем uint32_t, то возвращает ноль.)
		*/

		int n;
		for (n = 32; !(x & 0x80000000) && (n > 0); --n)
			x <<= 1;
		return n;
	}

public:
	rice(const char* inputFile);
	// в коде часто виден setPos(8) используемый для смещения позиции на 8. т.к. вначале храним значение k
	// требуемый для дальнейшего декодирования. Есть некоторые ньюансы, которые я не рассмотрел, к примеру
	// при декодировании необходимо указать размер оригинального размера файла, что "запрещено" по условию,
	// задачи (в декодер дается только название файла), но реализовал к сожалению так, но переделать можно.
	void encodeWord(uint32_t x, int k);
	uint32_t decodeWord(int k);

	int compress(uint8_t* input, uint8_t* output);
	void decompress(uint8_t* input, uint8_t* output, uint32_t inputSize, uint32_t outputSize);

};


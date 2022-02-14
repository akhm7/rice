#include "rice.h"

rice::rice(const char* inputFile)
{
	std::ifstream in(inputFile, std::ios::binary);

	std::vector<uint8_t>* buffer = new std::vector<uint8_t>(std::istreambuf_iterator<char>(in), {});

	_originalData = (uint8_t*)buffer->data();

	_inputSize = buffer->size();

	_compressedData = new uint8_t[_inputSize];
	_decompressedData = new uint8_t[_inputSize];
	int __outSize = compress(_originalData, _compressedData);
	decompress(_compressedData, _decompressedData, __outSize, _inputSize);
}
void rice::encodeWord(uint32_t x, int k)
{
	int j;
	uint32_t q = x >> k;

	if (q > _threshold)
	{
		for (j = 0; j < _threshold; ++j)
			stream->writeBit(1);

		q -= _threshold;

		int o = numBits(q);

		for (j = 0; j < o; ++j)
			stream->writeBit(1);

		stream->writeBit(0);

		for (j = o - 2; j >= 0; --j)
			stream->writeBit((q >> j) & 1);
	}
	else
	{
		for (uint32_t i = 0; i < q; ++i)
			stream->writeBit(1);

		stream->writeBit(0);
	}

	for (j = k - 1; j >= 0; --j)
		stream->writeBit((x >> j) & 1);
}

int rice::compress(uint8_t* input, uint8_t* output)
{
	uint32_t i, x, k, n, wordSize = 8;
	uint32_t* hgm = new uint32_t[_history];
	int j;

	uint32_t incount = _inputSize / (wordSize >> 3);

	if (incount == 0)
		return 0;

	stream = new bitStream(output, _inputSize + 1);
	k = 0;

	for (i = 0; (i < _history) && (i < incount); ++i)
	{
		n = numBits(input[i]);
		k += n;
	}

	k = (k + (i >> 1)) / i;

	if (k == 0)
		k = 1;

	output[0] = k;
	stream->setPos(8);

	for (i = 0; (i < incount) && ((stream->getPos() >> 3) <= _inputSize); ++i)
	{
		if (i >= _history)
		{
			k = 0;

			for (j = 0; j < _history; ++j)
				k += hgm[j];

			k = (k + (_history >> 1)) / _history;
		}

		x = input[i];
		encodeWord(x, k);
		hgm[i % _history] = numBits(x);
	}

	if (i < incount)
	{
		output[0] = 0;
		stream->setPos(8);

		for (i = 0; i < incount; ++i)
		{
			x = input[i];

			for (j = wordSize - 1; j >= 0; --j)
				stream->writeBit((x >> j) & 1);

		}
	}

	int outSize = (stream->getPos() + 7) >> 3;

	std::ofstream outCompress(_outputEncodeFile, std::ios::binary);
	outCompress.write((char*)output, outSize);
	outCompress.close();

	return outSize;
}

uint32_t rice::decodeWord(int k)
{
	uint32_t x;
	uint32_t q = 0;
	int i;

	while (stream->readBit())
		++q;

	if (q > _threshold)
	{
		int o = q - _threshold;
		x = 1;

		for (i = 0; i < o - 1; ++i)
			x = (x << 1) | stream->readBit();

		x += _threshold;
	}
	else
		x = q;

	for (i = k - 1; i >= 0; --i)
		x = (x << 1) | stream->readBit();

	return x;
}


void rice::decompress(uint8_t* input, uint8_t* output, uint32_t inputSize, uint32_t outputSize)
{

	uint32_t i, x, k, wordSize = 8;
	uint32_t* hgm = new uint32_t[_history];
	int j;

	uint32_t outputCount = outputSize / (wordSize >> 3);

	if (outputCount == 0)
	{
		return;
	}
	stream = new bitStream(input, inputSize);
	k = input[0];
	stream->setPos(8);

	if (k == 0)
	{
		for (i = 0; i < outputCount; ++i)
		{
			x = 0;

			for (j = wordSize - 1; j >= 0; --j)
			{
				x = (x << 1) | stream->readBit();
			}

			output[i] = x;
		}
	}
	else
	{
		for (i = 0; i < outputCount; ++i)
		{
			if (i >= _history)
			{
				k = 0;

				for (j = 0; j < _history; ++j)
				{
					k += hgm[j];
				}

				k = (k + (_history >> 1)) / _history;
			}

			x = decodeWord(k);
			output[i] = x;
			hgm[i % _history] = numBits(x);
		}
	}
	std::ofstream outCompress(_outputDecodeFile, std::ios::binary);
	outCompress.write((char*)output, outputSize);
	outCompress.close();
}
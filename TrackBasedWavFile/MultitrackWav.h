// Header file for the .tbwav type. RIFF based
#pragma once
#include "common.h"

#define WAVE_FORMAT_PCM 0x0001 
#define WAVE_FORMAT_IEEE_FLOAT 0x0003
#define WAVE_FORMAT_EXTENSIBLE 0xFFFE
#define MULTITRACK_WAVE_FOURCC 'MTWV'
#define RIFF_FOURCC 'RIFF'

typedef unsigned long DWORD;
typedef unsigned char BYTE;
typedef DWORD FOURCC;

typedef struct 
{
	FOURCC chunk_id;
	DWORD chunk_size;
	BYTE *chunk_data; 
} CHUNK;

typedef struct
{
	FOURCC chunk_id;
	DWORD chunk_size;
	union 
	{
		FOURCC fccType;
		BYTE *chunk_Data;
	} riff_chunk_data;
} RIFF_CHUNK;

class MultitrackWav 
{

private:
	RIFF_CHUNK riff_chunk;
	CHUNK format_chunk;
	CHUNK data_chunk;




public:
	MultitrackWav() 
	{
		riff_chunk = GenerateRIFFChunk();
		
	}
	
	RIFF_CHUNK GenerateRIFFChunk() 
	{
		RIFF_CHUNK generated;
		generated.chunk_id = RIFF_FOURCC;
		generated.riff_chunk_data.fccType = MULTITRACK_WAVE_FOURCC;
	}

	
	CHUNK CreateFormatChunk(int format, int nChannels, float nSamplesPerSecond, float nBytesPerSecond, int nBlockAlignment, int bitdepth)
	{
		CHUNK generated;
		generated.chunk_id = 'fmt';
		generated.chunk_size = 16;
		generated.chunk_data = (BYTE*) malloc(generated.chunk_size);
		// TODO: Write format chunk
	}

	DWORD CalculateChunkSizes() 
	{
		// returns the size of necessary data chunks
		DWORD TotalFileSize = 0;
		DWORD PCMDataSize = 0;
	}
	
};
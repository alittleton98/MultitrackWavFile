/*
Header file for the.mtwv type.RIFF based.
File format is meant to be similar to the WAVE format, but includes multiple "tracks".
A "track" in this case, is a container for one or multiple channels of audio with a defined location in the soundfield (ie. Left, Right, Center, etc.) 
*/ 

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
	// RIFF Chunk
	BYTE riff_chunk_id; // RIFF
	union riff_chunk_size { DWORD file_size; };
	BYTE riff_formatchunk_id; // MTWV 
	
	// format chunk 
	BYTE format_chunk_id; // 'fmt'
	union format_chunk_size { DWORD format_header_size; }; // size of the format header chunk
	union track_channel_count { int nChannels; }; // number of channels to be accounted for 
	union track_count { int nTracks; }; // number of "tracks" packed within the file
	union track_sample_rate { DWORD IntendedSamplesPerSecond; }; // number of samples that are to be read for each track
	union file_sample_rate { DWORD TotalSamplesPerSecond; }; // number of total samples read per second. Should be track_sample_rate * track_count * track_channel_count, but should NOT exceed 192000 
	union file_data_rate { DWORD TotalBytesPerSecond; }; // total bytes read per second by the device
	union track_block_alignment { int IntendedBlockSize; }; // block size per track in bytes
	union file_block_alignment { int TotalBlockSize; }; // block size for all tracks read
	union bit_depth { int nBitsPerSample; }; // Number of bits per sample. This is agnostic of either sample rate

	// data chunk
	BYTE data_chunk_id; // 'data'
	
	
} mtwv_header;



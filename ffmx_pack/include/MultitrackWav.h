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
	union riff_chunk_size { DWORD value; };
	riff_chunk_size FileSize;
	BYTE riff_formatchunk_id; // MTWV 
	
	// format chunk 
	BYTE format_chunk_id; // 'fmt'
	union format_chunk_size { DWORD value; }; // size of the format header chunk
	format_chunk_size FormatChunkSize;
	BYTE format_type;
	union track_channel_count { int value; }; // number of channels to be accounted for
	track_channel_count nChannels;
	union track_count { int value; }; // number of "tracks" packed within the file
	track_count nTracks;
	union track_sample_rate { DWORD value; }; // number of samples that are to be read for each track
	track_sample_rate IntendedSamplesPerSecond;
	union file_sample_rate { DWORD value; }; // number of total samples read per second. Should be track_sample_rate * track_count, but should NOT exceed 192000
	file_sample_rate FileSampleRate;
	union file_data_rate { DWORD value; }; // total bytes read per second by the device
	file_data_rate TotalBytesPerSecond;
	union track_block_alignment { int value; }; // block size per track in bytes
	track_block_alignment IntendedBlockAlignment;
	union file_block_alignment { int value; }; // block size for all tracks read
	file_block_alignment FileBlockAlignment;
	union bit_depth { int value; }; // Number of bits per sample. This is agnostic of either sample rate
	bit_depth nBitsPerSample;

	// data chunk
	BYTE data_chunk_id; // 'data'
	union data_chunk_size { DWORD value; };
	data_chunk_size DataChunkSize;
	// Any bits after this shall be considered audio data.
	
} mtwv_header;

// Creates file at path with header chunks
void CreateMTWVNative(BYTE format_type, int nChannels, int nTracks, DWORD IntendedSamplesPerSecond, int bitDepth, std::string path)
{
	mtwv_header header;
	std::ofstream untitledMTWV;
	
	// Set non-union calculated variables
	header.riff_chunk_id = RIFF_FOURCC;
	header.FileSize.value = 0; // Set to 0 as default. Replace after data is ready
	header.riff_formatchunk_id = MULTITRACK_WAVE_FOURCC;
	header.format_chunk_id = 'fmt';
	// Following chunk size is NOT equivalent to the regular WAVE format chunk size as there are extra parameters 
	header.FormatChunkSize.value = sizeof(mtwv_header::format_type) + sizeof(mtwv_header::track_channel_count) + sizeof(mtwv_header::track_count) + sizeof(mtwv_header::track_sample_rate) + sizeof(mtwv_header::file_sample_rate) + sizeof(mtwv_header::file_data_rate) + sizeof(mtwv_header::track_block_alignment) + sizeof(mtwv_header::file_block_alignment) + sizeof(mtwv_header::bit_depth);
	header.format_type = format_type;
	header.nChannels.value = nChannels;
	header.nTracks.value = nTracks;
	header.IntendedSamplesPerSecond.value = IntendedSamplesPerSecond;
	header.FileSampleRate.value = header.IntendedSamplesPerSecond.value * header.nTracks.value; // Should not exceed 192kHz
	header.TotalBytesPerSecond.value = (header.FileSampleRate.value * header.nChannels.value * bitDepth) / 8;
	header.IntendedBlockAlignment.value = (header.nChannels.value * bitDepth) / 8;
	header.FileBlockAlignment.value = (header.IntendedBlockAlignment.value * header.nTracks.value); // no division by 8 since it is done for IntendedBlockAlignment
	header.nBitsPerSample.value = bitDepth;
	header.data_chunk_id = 'data';

	// Assumed to have a proper file name
	untitledMTWV.open(path, std::ios::out);
	// Write data in order from the header
	untitledMTWV << header.riff_chunk_id;

}


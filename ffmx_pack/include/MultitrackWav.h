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
	BYTE format_type;
	union track_channel_count { int nChannels; }; // number of channels to be accounted for 
	union track_count { int nTracks; }; // number of "tracks" packed within the file
	union track_sample_rate { DWORD IntendedSamplesPerSecond; }; // number of samples that are to be read for each track
	union file_sample_rate { DWORD TotalSamplesPerSecond; }; // number of total samples read per second. Should be track_sample_rate * track_count, but should NOT exceed 192000 
	union file_data_rate { DWORD TotalBytesPerSecond; }; // total bytes read per second by the device
	union track_block_alignment { int IntendedBlockSize; }; // block size per track in bytes
	union file_block_alignment { int TotalBlockSize; }; // block size for all tracks read
	union bit_depth { int nBitsPerSample; }; // Number of bits per sample. This is agnostic of either sample rate

	// data chunk
	BYTE data_chunk_id; // 'data'
	union data_chunk_size { DWORD data_chunk_size; };
	// Any bits after this shall be considered audio data.
	
} mtwv_header;

// Creates file at path with header chunks
FILE CreateMTWVNative(BYTE format_type, int nChannels, int nTracks, DWORD IntendedSamplesPerSecond, int bitDepth, std::string path)
{
	FILE mtwvFIle;
	mtwv_header header;
	std::ofstream untitledMTWV;
	
	// Set non-union calculated variables
	header.riff_chunk_id = RIFF_FOURCC;
	header.riff_formatchunk_id = MULTITRACK_WAVE_FOURCC;
	header.format_chunk_id = 'fmt';
	header.format_type = format_type;
	header.data_chunk_id = 'data';

	// Set union calculated variables (denoted with Pascal case)
	mtwv_header::riff_chunk_size RiffChunkSize;
	RiffChunkSize.file_size = 0; // Set to 0 as default. Replace after data is ready
	mtwv_header::format_chunk_size FormatChunkSize;
	// Following chunk size is NOT equivalent to the regular WAVE format chunk size as there are extra parameters 
	FormatChunkSize.format_header_size = sizeof(mtwv_header::format_type) + sizeof(mtwv_header::track_channel_count) + sizeof(mtwv_header::track_count) + sizeof(mtwv_header::track_sample_rate) + sizeof(mtwv_header::file_sample_rate) + sizeof(mtwv_header::file_data_rate) + sizeof(mtwv_header::track_block_alignment) + sizeof(mtwv_header::file_block_alignment) + sizeof(mtwv_header::bit_depth); 
	mtwv_header::track_channel_count TrackChannelCount;
	TrackChannelCount.nChannels = nChannels;
	mtwv_header::track_count TrackCount;
	TrackCount.nTracks = nTracks;
	mtwv_header::track_sample_rate TrackSampleRate;
	TrackSampleRate.IntendedSamplesPerSecond = IntendedSamplesPerSecond;
	mtwv_header::file_sample_rate FileSampleRate;
	FileSampleRate.TotalSamplesPerSecond = TrackSampleRate.IntendedSamplesPerSecond * TrackCount.nTracks; // this should not exceed 192kHz
	mtwv_header::file_data_rate FileDataRate;
	mtwv_header::track_block_alignment TrackBlockAlign;
	TrackBlockAlign.IntendedBlockSize = (TrackChannelCount.nChannels * bitDepth)/8;
	mtwv_header::file_block_alignment FileBlockAlign;
	FileBlockAlign.TotalBlockSize = (TrackChannelCount.nChannels * TrackCount.nTracks * bitDepth) / 8;
	FileDataRate.TotalBytesPerSecond = (FileSampleRate.TotalSamplesPerSecond * TrackCount.nTracks * TrackChannelCount.nChannels * bitDepth)/8;
	mtwv_header::bit_depth BitDepth;
	BitDepth.nBitsPerSample = bitDepth;

	// Assumed to have a proper file name
	untitledMTWV.open(path, std::ios::out);
	// Write data in order from the header
	


	return mtwvFIle;
}


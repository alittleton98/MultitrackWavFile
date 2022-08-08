# Multitrack Wav File

.mtwv is a file format based on the .wav but holds multiple "tracks" of n channels rather than a single track of n channels. Intended as a means of grouping multiple audio files of the same format together under a uniform timing \[0, n-1] where n is the last sample in the timeline. The end of the timeline could be the end of the longest file in the case where all tracks begin at the word clock's \[0] or the end of the last playing file in the case the tracks have starting positions a minimum of \[n-2] samples from the end of the intended time.

### Project

To use the file format, all that's required is the Multitrack.h file. The Formatter.cpp file is intended to demonstrate and test the functionality of the file.&#x20;

### RIFF Format

| Size | Name                                  | Value         |
| ---- | ------------------------------------- | ------------- |
| 4    | riff\_chunk.chunk\_id                 | 'RIFF'        |
| 4    | riff\_chunk.chunk\_size               | TotalFileSize |
| 4    | riff\_chunk.riff\_chunk\_data.fccType | 'MTWV'        |
| 4    | format\_chunk.chunk\_id               | 'fmt'         |

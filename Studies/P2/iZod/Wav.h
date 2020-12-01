#ifndef WAV_H
#define WAV_H
#include "FileAudio.h"

class Wav : public FileAudio {
private:
	int freq;
	bool lossless;
public:
	Wav(const std::string& t, int s, int f, bool l);
	Wav* clone() const;
	bool qualita() const;
	bool isLossless() const;
	bool operator==(const FileAudio& f) const;
};
#endif

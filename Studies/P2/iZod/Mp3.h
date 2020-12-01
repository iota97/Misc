#ifndef MP3_H
#define MP3_H
#include "FileAudio.h"

class Mp3 : public FileAudio {
private:
	int bitRate;
public:
	Mp3(const std::string& t, int s, int r);
	Mp3* clone() const;
	bool qualita() const;
	bool operator==(const FileAudio& f) const;
	int getBitRate() const;
};
#endif

#include "Mp3.h"

Mp3::Mp3(const std::string& t, int s, int r) : FileAudio(t, s), bitRate(r) {}

Mp3* Mp3::clone() const {
	return new Mp3(*this);
}

bool Mp3::qualita() const {
	return bitRate >= 192;
}

bool Mp3::operator==(const Mp3& m) const {
	return FileAudio::operator==(m) && bitRate == m.bitRate;
}

int Mp3::getBitRate() const {
	return bitRate;
}

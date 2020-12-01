#include "Mp3.h"

Mp3::Mp3(const std::string& t, int s, int r) : FileAudio(t, s), bitRate(r) {}

Mp3* Mp3::clone() const {
	return new Mp3(*this);
}

bool Mp3::qualita() const {
	return bitRate >= 192;
}

bool Mp3::operator==(const FileAudio& f) const {
	return FileAudio::operator==(f) && bitRate == static_cast<const Mp3&>(f).bitRate;
}

int Mp3::getBitRate() const {
	return bitRate;
}

#include "Wav.h"

Wav::Wav(const std::string& t, int s, int f, bool l) : FileAudio(t, s), freq(f), lossless(l) {}

Wav* Wav::clone() const {
	return new Wav(*this);
}

bool Wav::qualita() const {
	return freq >= 96;
}

bool Wav::isLossless() const {
	return lossless;
}

bool Wav::operator==(const FileAudio& f) const {
	return FileAudio::operator==(f) && freq == static_cast<const Wav&>(f).freq && lossless == static_cast<const Wav&>(f).lossless;
}

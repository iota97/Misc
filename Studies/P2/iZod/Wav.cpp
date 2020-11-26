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

bool Wav::operator==(const Wav& w) const {
	return FileAudio::operator==(w) && freq == w.freq && lossless == w.lossless;
}

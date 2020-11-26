#include "IZod.h"

IZod::Brano::Brano(FileAudio* p) {
	ptr = p->clone();
}

IZod::Brano::Brano(const Brano& b) {
	ptr = b.ptr->clone();
}

IZod::Brano& IZod::Brano::operator=(const Brano& b) {
	if (this != &b) {
		delete ptr;
		ptr = b.ptr->clone();
	}
	return *this;
};

IZod::Brano::~Brano() {
	delete ptr;
}

std::vector<Mp3> IZod::mp3(double dim, int br) const {
	std::vector<Mp3> aux;
	for (auto& it : vec) {
		Mp3* p = dynamic_cast<Mp3*>(it.ptr);
		if (p && p->getSize() >= dim && p->getBitRate() >= br) {
			aux.push_back(*p);
		}
	}
	return aux;
}

std::vector<FileAudio*> IZod::braniQual() const {
	std::vector<FileAudio*> aux;
	for (auto& it : vec) {
		if (it.ptr->qualita()) {
			Wav* p = dynamic_cast<Wav*>(it.ptr);
			if (!p || p->isLossless())
				aux.push_back(it.ptr);
		}
	}
	return aux;
}
void IZod::insert(Mp3* m) {
	for (auto& it : vec) {
		Mp3* p = dynamic_cast<Mp3*>(it.ptr);
		if (p && *p == *m) return;
	}
	vec.push_back(Brano(m));
}

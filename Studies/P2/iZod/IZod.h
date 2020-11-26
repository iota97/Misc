#ifndef IZOD_H
#define IZOD_H

#include <vector>
#include "Mp3.h"
#include "Wav.h"

class IZod {
private:
	class Brano {
	private:
		FileAudio* ptr;
	public:
		Brano(FileAudio* p);
		Brano(const Brano& b);
		Brano& operator=(const Brano& b);
		~Brano();
	friend class IZod;
	};
	std::vector<Brano> vec;
public:
	std::vector<Mp3> mp3(double dim, int br) const;
	std::vector<FileAudio*> braniQual() const;
	void insert(Mp3* m);
};
#endif

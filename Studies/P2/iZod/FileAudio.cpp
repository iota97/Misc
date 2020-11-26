#include "FileAudio.h"

FileAudio::FileAudio(const std::string& t, int s) : title(t), size(s) {}

bool FileAudio::operator==(const FileAudio& f) const {
	return title == f.title && size == f.size;
}

int FileAudio::getSize() const {
	return size;
}

#ifndef FILEAUDIO_H
#define FILEAUDIO_H
#include <string>

class FileAudio {
private:
	std::string title;
	int size;
public:
	FileAudio(const std::string& t, int s);
	virtual ~FileAudio() {}
	virtual FileAudio* clone() const = 0;
	virtual bool qualita() const = 0;
	bool operator==(const FileAudio& f) const;
	int getSize() const;
};
#endif

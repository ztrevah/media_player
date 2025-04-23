// models/Media.h
#ifndef MEDIA_H
#define MEDIA_H

#include <string>

class MediaFile {
private:
    std::string filePath_;
public:
    MediaFile(const std::string& filePath = "");
    std::string getFilePath() const;

    void setFilePath(const std::string& filePath);
};

#endif // MEDIA_H
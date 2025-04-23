// models/Media.cpp
#include "../inc/MediaFile.h"

MediaFile::MediaFile(const std::string& filePath) : filePath_(filePath) {}

std::string MediaFile::getFilePath() const {
    return filePath_;
}
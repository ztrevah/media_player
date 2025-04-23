#ifndef METADATA_SERVICE_H
#define METADATA_SERVICE_H

#include "../interfaces/IMetadataService.h"

class MetadataService : public IMetadataService {
public:
    MetadataService();
    ~MetadataService();
    AudioMetadata getAudioMetadata(MediaFile mediaFile);
    void updateAudioMetadata(MediaFile mediaFile, AudioMetadata metadata);
    VideoMetadata getVideoMetadata(MediaFile mediaFile);
    void updateVideoMetadata(MediaFile mediaFile, VideoMetadata);
};

class MetadataServiceException : public std::exception {
private:
    std::string message;

public:
    MetadataServiceException(const std::string& msg) : message(msg) {}

    const char* what() const noexcept override {
        return message.c_str();
    }
};

#endif
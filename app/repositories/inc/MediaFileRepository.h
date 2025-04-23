#ifndef MEDIA_FILE_REPOSITORY_H
#define MEDIA_FILE_REPOSITORY_H

#include <string>
#include <vector>
#include "../interfaces/IMediaFileRepository.h"
#include "../../models/inc/MediaFile.h"

class MediaFileRepository : public IMediaFileRepository {
private:
    std::vector<MediaFile> __mediaFileList;
    void loadMediaFileList();
public:
    MediaFileRepository(std::string repositoryDirectory = ".");
    ~MediaFileRepository();

    std::vector<MediaFile> getAll();
    std::vector<MediaFile> getAllAudioFile();
    std::vector<MediaFile> get(int pageNumber, int pageSize);
    MediaFile getByFilePath(std::string filePath);
    MediaFile getAudioFileByFilePath(std::string filePath);
};

class MediaFileRepositoryException : public std::exception {
private:
    std::string message; // Store the custom error message

public:
    // 2. Constructor that takes a custom message
    MediaFileRepositoryException(const std::string& msg) : message(msg) {}

    // 3. Override the what() virtual function
    const char* what() const noexcept override {
        return message.c_str(); // Return the stored message as a C-style string
    }
};

#endif
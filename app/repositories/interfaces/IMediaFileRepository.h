#ifndef I_MEDIA_FILE_REPOSITORY_H
#define I_MEDIA_FILE_REPOSITORY_H

#include <string>
#include <vector>
#include "IRepository.h"
#include "../../models/inc/MediaFile.h"

class IMediaFileRepository : public IRepository {
public:
    IMediaFileRepository(std::string repositoryDirectory = ".") : IRepository(repositoryDirectory) {};
    ~IMediaFileRepository() {};

    virtual std::vector<MediaFile> getAll() = 0;
    virtual std::vector<MediaFile> getAllAudioFile() = 0;
    virtual std::vector<MediaFile> get(int pageNumber, int pageSize) = 0;
    virtual MediaFile getByFilePath(std::string filePath) = 0;
    virtual MediaFile getAudioFileByFilePath(std::string filePath) = 0;
};

#endif
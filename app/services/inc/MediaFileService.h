#ifndef MEDIA_FILE_SERVICE_H
#define MEDIA_FILE_SERVICE_H

#include <vector>
#include <string>

#include "../interfaces/IMediaFileService.h"
#include "../../models/inc/MediaFile.h"
#include "../../repositories/interfaces/IMediaFileRepository.h"

class MediaFileService : public IMediaFileService {
    IMediaFileRepository* __mediaFileRepository;
public:
    MediaFileService(IMediaFileRepository* mediaFileRepository);
    ~MediaFileService();
    std::vector<MediaFile> getAllMediaFiles();
    std::vector<MediaFile> getAllAudioFiles();
    std::vector<MediaFile> getMediaFiles(int pageNumber, int pageSize);
    MediaFile getMediaFileByPath(std::string filePath);
};

#endif
#ifndef I_MEDIA_FILE_SERVICE_H
#define I_MEDIA_FILE_SERVICE_H

#include <vector>
#include <string>
#include "../../models/inc/MediaFile.h"

class IMediaFileService {
public:
    virtual ~IMediaFileService() {};
    virtual std::vector<MediaFile> getAllMediaFiles() = 0;
    virtual std::vector<MediaFile> getMediaFiles(int pageNumber, int pageSize) = 0;
    virtual MediaFile getMediaFileByPath(std::string filePath) = 0;
};

#endif
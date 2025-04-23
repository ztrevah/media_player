// controllers/MediaFileListController.h
#ifndef MEDIA_FILE_LIST_CONTROLLER_H
#define MEDIA_FILE_LIST_CONTROLLER_H

#include <string>
#include <vector>
#include "../../models/inc/MediaFile.h"
#include "BaseController.h"
#include "../../Application.h"
#include "../../services/interfaces/IMediaFileService.h"

class MediaFileListController : public BaseController {
private:
    IMediaFileService* __mediaFileService;
    int __pageSize;
    
public:
    MediaFileListController(IMediaFileService* mediaFileService, int pageSize = 10);
    ~MediaFileListController();
    std::vector<MediaFile> getMediaList() const;
    std::vector<MediaFile> getMediaList(int pageNumber) const;
    size_t getMediaListSize() const;
    int getPageSize() const;
    void setPageSize(int pageSize);
    void openView(IView* previousView = nullptr);
    void closeView();

    void openMetadataView(MediaFile mediaFile);
    void openMediaPlayView(MediaFile mediaFile);
};

#endif // MEDIA_FILE_LIST_CONTROLLER_H
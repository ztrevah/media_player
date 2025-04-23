// controllers/MetadataController.h
#ifndef METADATA_CONTROLLER_H
#define METADATA_CONTROLLER_H

#include <string>
#include "BaseController.h"
#include "../../models/inc/AudioMetadata.h"
#include "../../models/inc/VideoMetadata.h"
#include "../../services/interfaces/IMetadataService.h"


class MetadataController : public BaseController {
    IMetadataService* __metadataService;
    MediaFile __mediaFile;
public:
    MetadataController(IMetadataService* metadataService, const MediaFile& mediaFile);
    ~MetadataController();
    MediaFile getCurrentMediaFile() const;
    AudioMetadata getAudioMetadata(const std::string& filePath);
    VideoMetadata getVideoMetadata(const std::string& filePath);
    bool upsertAudioMetadata(const std::string& filePath, AudioMetadata& audioMetadata);
    bool upsertVideoMetadata(const std::string& filePath, VideoMetadata& videoMetadata);

    void openView(IView* previousView = nullptr);
    void closeView();
};

#endif // METADATA_CONTROLLER_H
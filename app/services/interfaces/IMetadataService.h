#ifndef I_METADATA_SERVICE_H
#define I_METADATA_SERVICE_H

#include "../../models/inc/AudioMetadata.h"
#include "../../models/inc/VideoMetadata.h"
#include "../../models/inc/MediaFile.h"

class IMetadataService {
public:
    virtual ~IMetadataService() {};
    virtual AudioMetadata getAudioMetadata(MediaFile mediaFile) = 0;
    virtual void updateAudioMetadata(MediaFile mediaFile, AudioMetadata metadata) = 0;
    virtual VideoMetadata getVideoMetadata(MediaFile mediaFile) = 0;
    virtual void updateVideoMetadata(MediaFile mediaFile, VideoMetadata) = 0;
};

#endif
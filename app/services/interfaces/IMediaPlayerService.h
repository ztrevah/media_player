#ifndef I_MEDIA_PLAYER_SERVICE_H
#define I_MEDIA_PLAYER_SERVICE_H

#include <vector>
#include <string>
#include "../../models/inc/MediaFile.h"
#include "../../models/inc/Playlist.h"

class IMediaPlayerService {
public:
    virtual ~IMediaPlayerService() {};
    virtual MediaFile getNextMediaFile(std::string mediaFilePath) = 0;
    virtual MediaFile getNextMediaFile(std::string mediaFilePath, std::string playlistName) = 0;
    virtual MediaFile getPreviousMediaFile(std::string mediaFilePath) = 0;
    virtual MediaFile getPreviousMediaFile(std::string mediaFilePath, std::string playlistName) = 0;
    virtual MediaFile getMediaFile(std::string filePath) = 0;
    virtual MediaFile getDefaultMedia() = 0;
    virtual Playlist getPlaylist(std::string name) = 0;
};

#endif
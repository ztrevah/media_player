#ifndef I_PLAYLIST_SERVICE_H
#define I_PLAYLIST_SERVICE_H

#include <vector>
#include <string>
#include "../../models/inc/Playlist.h"
#include "../../models/inc/MediaFile.h"

class IPlaylistService {
public:
    virtual ~IPlaylistService() {};
    virtual std::vector<Playlist> getAllPlaylists() = 0;
    virtual Playlist getPlaylistByName(std::string name) = 0;
    virtual Playlist addPlaylist(std::string name, std::vector<MediaFile>) = 0;
    virtual Playlist removePlaylist(std::string name) = 0;
    virtual Playlist updatePlaylist(std::string name, std::vector<MediaFile>) = 0;
};

#endif
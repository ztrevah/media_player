#ifndef PLAYLIST_SERVICE_H
#define PLAYLIST_SERVICE_H

#include <vector>
#include <string>
#include "../../models/inc/Playlist.h"
#include "../../models/inc/MediaFile.h"

#include "../interfaces/IPlaylistService.h"
#include "../../repositories/interfaces/IPlaylistRepository.h"

class PlaylistService : public IPlaylistService {
private:
    IPlaylistRepository* __playlistRepository;
public:
    PlaylistService(IPlaylistRepository* playlistRepository);
    ~PlaylistService();
    std::vector<Playlist> getAllPlaylists();
    Playlist getPlaylistByName(std::string name);
    Playlist addPlaylist(std::string name, std::vector<MediaFile> mediaFileList);
    Playlist removePlaylist(std::string name);
    Playlist updatePlaylist(std::string name, std::vector<MediaFile> mediaFileList);
};

#endif
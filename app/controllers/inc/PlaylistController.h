// controllers/PlaylistController.h
#ifndef PLAYLIST_CONTROLLER_H
#define PLAYLIST_CONTROLLER_H

#include <string>
#include <vector>
#include "../../models/inc/Playlist.h"
#include "../../models/inc/MediaFile.h"
#include "BaseController.h"
#include "../../Application.h"
#include "../../services/interfaces/IPlaylistService.h"

class PlaylistController : public BaseController {
private:
    IPlaylistService* __playlistService;
public:
    PlaylistController(IPlaylistService* playlistService);
    ~PlaylistController();
    Playlist getPlaylist(const std::string& name) const;
    std::vector<Playlist> getPlaylists() const;
    Playlist createPlaylist(const std::string& name, const std::vector<MediaFile>& media);
    Playlist updatePlaylist(const std::string& name, const std::vector<MediaFile>& media);
    Playlist deletePlaylist(const std::string& name);

    void openView(IView* previousViews = nullptr);
    void closeView();

    void openMetadataView(MediaFile mediaFile);
    void openMediaPlayView(std::string playlistName, std::string filePath = "");
};

#endif // PLAYLIST_CONTROLLER_H
#include "../inc/PlaylistService.h"

PlaylistService::PlaylistService(IPlaylistRepository* playlistRepository) {
    __playlistRepository = playlistRepository;
}

PlaylistService::~PlaylistService() {

}

std::vector<Playlist> PlaylistService::getAllPlaylists() {
    return __playlistRepository->getAll();
}

Playlist PlaylistService::getPlaylistByName(std::string name) {
    return __playlistRepository->getByName(name);
}

Playlist PlaylistService::addPlaylist(std::string name, std::vector<MediaFile> mediaFileList) {
    return __playlistRepository->add(name, mediaFileList);
}

Playlist PlaylistService::removePlaylist(std::string name) {
    return __playlistRepository->remove(name);
}

Playlist PlaylistService::updatePlaylist(std::string name, std::vector<MediaFile> mediaFileList) {
    return __playlistRepository->update(name, mediaFileList);
}

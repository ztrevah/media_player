#include "../inc/MediaPlayerService.h"

#include <iostream>

MediaPlayerService::MediaPlayerService(IMediaFileRepository* mediaFileRepository, IPlaylistRepository* playlistRepository) :
    __mediaFileRepository(mediaFileRepository), __playlistRepository(playlistRepository) 
{

}

MediaPlayerService::~MediaPlayerService() {

}

MediaFile MediaPlayerService::getNextMediaFile(std::string mediaFilePath) {
    int currentTrackIndex = -1;
    auto mediaList = __mediaFileRepository->getAllAudioFile();
    for(size_t i=0;i<mediaList.size();i++) {
        if(mediaFilePath == mediaList[i].getFilePath()) {
            currentTrackIndex = i;
            break;
        }
    }

    if(currentTrackIndex == -1) {
        throw MediaPlayerServiceException("Error: Cannot get next track of the track: " + mediaFilePath);
    }

    return mediaList[(currentTrackIndex + 1) % mediaList.size()];
}

MediaFile MediaPlayerService::getNextMediaFile(std::string mediaFilePath, std::string playlistName) {
    Playlist p = __playlistRepository->getByName(playlistName);
    
    int currentTrackIndex = -1;
    auto mediaList = p.getMediaList();
    for(size_t i=0;i<mediaList.size();i++) {
        if(mediaFilePath == mediaList[i].getFilePath()) {
            currentTrackIndex = i;
            break;
        }
    }

    if(currentTrackIndex == -1) {
        throw MediaPlayerServiceException("Error: Cannot get next track of the track: " + mediaFilePath 
            + " in the playlist " + playlistName + ".");
    }

    return mediaList[(currentTrackIndex + 1) % mediaList.size()];
}

MediaFile MediaPlayerService::getPreviousMediaFile(std::string mediaFilePath) {
    int currentTrackIndex = -1;
    auto mediaList = __mediaFileRepository->getAllAudioFile();
    for(size_t i=0;i<mediaList.size();i++) {
        if(mediaFilePath == mediaList[i].getFilePath()) {
            currentTrackIndex = i;
            break;
        }
    }

    if(currentTrackIndex == -1) {
        throw MediaPlayerServiceException("Error: Cannot get next track of the track: " + mediaFilePath);
    }

    return mediaList[(currentTrackIndex - 1) % mediaList.size()];
}

MediaFile MediaPlayerService::getPreviousMediaFile(std::string mediaFilePath, std::string playlistName) {
    Playlist p = __playlistRepository->getByName(playlistName);
    
    int currentTrackIndex = -1;
    auto mediaList = p.getMediaList();
    for(size_t i=0;i<mediaList.size();i++) {
        if(mediaFilePath == mediaList[i].getFilePath()) {
            currentTrackIndex = i;
            break;
        }
    }

    if(currentTrackIndex == -1) {
        throw MediaPlayerServiceException("Error: Cannot get next track of the track: " + mediaFilePath 
            + " in the playlist " + playlistName + ".");
    }

    return mediaList[(currentTrackIndex - 1) % mediaList.size()];
}

MediaFile MediaPlayerService::getMediaFile(std::string filePath) {
    // std::cerr << "oke" << "\n";
    return __mediaFileRepository->getAudioFileByFilePath(filePath);
}

MediaFile MediaPlayerService::getDefaultMedia() {
    if(__mediaFileRepository->getAllAudioFile().size() == 0) {
        return MediaFile("");
    }
    
    return __mediaFileRepository->getAllAudioFile()[0];
}

Playlist MediaPlayerService::getPlaylist(std::string name) {
    return __playlistRepository->getByName(name);
}

#ifndef MEDIA_PLAYER_SERVICE_H
#define MEDIA_PLAYER_SERVICE_H

#include <string>

#include "../interfaces/IMediaPlayerService.h"
#include "../../repositories/interfaces/IMediaFileRepository.h"
#include "../../repositories/interfaces/IPlaylistRepository.h"

class MediaPlayerService : public IMediaPlayerService {
private:
    IMediaFileRepository* __mediaFileRepository;
    IPlaylistRepository* __playlistRepository;
public:
    MediaPlayerService(IMediaFileRepository* mediaFileRepository, IPlaylistRepository* playlistRepository);
    ~MediaPlayerService();

    MediaFile getNextMediaFile(std::string currentMediaFile);
    MediaFile getNextMediaFile(std::string currentMediaFile, std::string currentPlaylist);
    MediaFile getPreviousMediaFile(std::string currentMediaFile);
    MediaFile getPreviousMediaFile(std::string currentMediaFile, std::string currentPlaylist);
    MediaFile getMediaFile(std::string filePath);
    MediaFile getDefaultMedia();
    Playlist getPlaylist(std::string name);
};


class MediaPlayerServiceException : public std::exception {
    private:
        std::string message;
    
    public:
        MediaPlayerServiceException(const std::string& msg) : message(msg) {}
    
        const char* what() const noexcept override {
            return message.c_str();
        }
    };
    

#endif
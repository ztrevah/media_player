#ifndef LAST_PLAY_STATE_H
#define LAST_PLAY_STATE_H

#include "MediaFile.h"

class MediaPlayState {
    private:
        bool is_playing;
        long pause_at;
        MediaFile played_media;
        std::string playlist_from;
    
    public:
        // Constructor
        MediaPlayState();
    
        MediaPlayState(bool playing, long pauseTime, const MediaFile &media, const std::string& playlistFrom = "");
    
        ~MediaPlayState();
    
        // Getters
        bool isPlaying() const;
        long getPauseAt() const;
        MediaFile getPlayedMedia() const;
        std::string getPlaylistFrom() const;
    
        // Setters
        void setIsPlaying(bool playing);
        void setPauseAt(long pauseTime);
        void setPlayedMedia(const MediaFile &media);
        void setPlaylistFrom(const std::string& playlistFrom);
};

#endif
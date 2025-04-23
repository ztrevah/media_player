#include "../inc/MediaPlayState.h"

MediaPlayState::MediaPlayState() : is_playing(false), pause_at(0), played_media(MediaFile()) {}
    
// Constructor with parameters
MediaPlayState::MediaPlayState(bool playing, long pauseTime, const MediaFile &media, const std::string& playlistFrom) 
    : is_playing(playing), pause_at(pauseTime), played_media(media), playlist_from("") {}

MediaPlayState::~MediaPlayState() {
    
}

// Getters
bool MediaPlayState::isPlaying() const { return is_playing; }
long MediaPlayState::getPauseAt() const { return pause_at; }
MediaFile MediaPlayState::getPlayedMedia() const { return played_media; }
std::string MediaPlayState::getPlaylistFrom() const {return playlist_from; }

// Setters
void MediaPlayState::setIsPlaying(bool playing) { is_playing = playing; }
void MediaPlayState::setPauseAt(long pauseTime) { pause_at = pauseTime; }
void MediaPlayState::setPlayedMedia(const MediaFile &media) { played_media = media; }
void MediaPlayState::setPlaylistFrom(const std::string& playlistFrom) { playlist_from = playlistFrom; }
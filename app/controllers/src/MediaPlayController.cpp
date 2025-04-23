#include "../inc/MediaPlayController.h"
#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <chrono>
#include <thread>
#include "../../views/inc/MediaPlayView.h"
#include <fstream>

MediaPlayController* MediaPlayController::instance = nullptr;

MediaPlayController* MediaPlayController::getInstance() {
    if(instance == nullptr) {
        instance = new MediaPlayController;
    }
    return instance;
}

void MediaPlayController::removeInstance() {
    if(instance) {
        delete instance;
        instance = nullptr;
    }
}

MediaPlayController::MediaPlayController(const MediaPlayState& playState) : BaseController() {
    this->__mediaPlayerService = Application::getMediaPlayerService();
    this->__currentMusic = nullptr;
    this->__currentVolume = MIX_MAX_VOLUME;
    this->__autoNext = false;
    
    Mix_VolumeMusic(__currentVolume);

    this->__playState.setIsPlaying(false);
    this->__playState.setPauseAt(playState.getPauseAt());

    if(playState.getPlaylistFrom().empty()) {
        this->__playState.setPlaylistFrom("");
        try {
            this->__playState.setPlayedMedia(__mediaPlayerService->getMediaFile(playState.getPlayedMedia().getFilePath()));
        } catch (std::exception& e) {
            this->__playState.setPlayedMedia(__mediaPlayerService->getDefaultMedia());
        }
        return;
    }
    try {
        Playlist existingPlaylist = __mediaPlayerService->getPlaylist(playState.getPlaylistFrom());
        int i = 0;
        for(const auto &f: existingPlaylist.getMediaList()) {
            if(f.getFilePath() == playState.getPlayedMedia().getFilePath()) break;
            i++;
        }
        
        this->__playState.setPlaylistFrom(existingPlaylist.getName());
        this->__playState.setPlayedMedia(existingPlaylist.getMediaList().at(i));
    } catch(std::exception &e) {
        this->__playState.setPlaylistFrom("");
        this->__playState.setPlayedMedia(__mediaPlayerService->getDefaultMedia());
    }
}

MediaPlayController::~MediaPlayController() {
    Mix_HaltMusic();
    if(__currentMusic) {
        Mix_FreeMusic(__currentMusic);
        __currentMusic = nullptr;
    }
    __autoNext = false;
    if(this->__playThread.joinable()) {
        this->__playThread.join();
    }

    closeView();
    instance = nullptr;
}

void MediaPlayController::openView(IView* previousView) {
    if(this->view == nullptr) {
        this->view = new MediaPlayView(this, previousView);
    }
    this->view->display();

    closeView();
}

void MediaPlayController::closeView() {
    if(this->view != nullptr) {
        delete view;
        view = nullptr;
    }
}

MediaPlayState MediaPlayController::getPlayState() const { return this->__playState; }
// void MediaPlayController::setPlayState(const MediaPlayState& playState) { this->__playState = playState; }

Mix_Music* MediaPlayController::getCurrentMusic() const { return this->__currentMusic; }
// void MediaPlayController::set__CurrentMusic(Mix_Music* __currentMusic) { this->__currentMusic = __currentMusic; }

int MediaPlayController::getCurrentVolume() const { return this->__currentVolume; }
// void MediaPlayController::setCurrentVolume(int currentVolume) { this->__currentVolume = currentVolume; }

// const std::string& MediaPlayController::getConfigDirectory() const { return this->configDirectory; }
// void MediaPlayController::setConfigDirectory(const std::string& configDirectory) { this->configDirectory = configDirectory; }

void MediaPlayController::play() {
    std::unique_lock<std::mutex> lock(__playMutex);
    
    if (__playState.isPlaying()) {
        Mix_ResumeMusic();
        return;
    }
    
    else {
        if (__playThread.joinable()) {
            __playThread.join();
        }
        __playThread = std::thread([this]() { playCurrentTrack(); });
        __playThread.detach();
    } 
}

void MediaPlayController::play(const std::string& filePath) {
    std::unique_lock<std::mutex> lock(__playMutex);

    try {
        MediaFile mediaFile = __mediaPlayerService->getMediaFile(filePath);
        __playState.setPlayedMedia(mediaFile);
        __playState.setPauseAt(0);
        __playState.setPlaylistFrom("");
    } catch (std::exception &e) {
        return;
    }
    
    if (__playState.isPlaying()) {
        __playState.setIsPlaying(false);
        __autoNext = false;
        Mix_HaltMusic();
        if (__playThread.joinable()) {
            __playThread.join();
        }
    }

    lock.unlock();
    play();
}

void MediaPlayController::playFromPlaylist(const std::string& playlistName, const std::string& filePath) {
    std::unique_lock<std::mutex> lock(__playMutex);

    try {
        Playlist p = __mediaPlayerService->getPlaylist(playlistName);
        if(p.getMediaList().size() == 0) return;
        if(filePath.empty()) {
            __playState.setPauseAt(0);
            __playState.setPlayedMedia(p.getMediaList()[0]);
            __playState.setPlaylistFrom(playlistName);

        }

        else {
            int currentTrackIndex = -1;
            for(size_t i=0;i<p.getMediaList().size();i++) {
                if(p.getMediaList()[i].getFilePath() == filePath) {
                    currentTrackIndex = i;
                    break;
                }
            }
            if(currentTrackIndex == -1) {
                return;
            }
    
            __playState.setPauseAt(0);
            __playState.setPlayedMedia(p.getMediaList()[currentTrackIndex]);
            __playState.setPlaylistFrom(playlistName);
        }
    } catch (std::exception &e) {
        return;
    }

    if (__playState.isPlaying()) {
        __playState.setIsPlaying(false);
        __autoNext = false;
        Mix_HaltMusic();
        if (__playThread.joinable()) {
            __playThread.join();
        }
    }
    
    lock.unlock();
    play();
}

void MediaPlayController::playCurrentTrack() {
    std::unique_lock<std::mutex> lock(__playMutex);

    __autoNext = true;
    if (__currentMusic != nullptr) {
        Mix_FreeMusic(__currentMusic);
        __currentMusic = nullptr;
    }
    __currentMusic = Mix_LoadMUS(__playState.getPlayedMedia().getFilePath().c_str());
    if (__currentMusic == nullptr) {
        std::cerr << "Mix_LoadMUS Error: " << Mix_GetError() << std::endl;
        return;
    }
    __playState.setIsPlaying(true);
    Mix_PlayMusic(__currentMusic, 0);

    savePlayState();
    lock.unlock();

    while (Mix_PlayingMusic()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    if(__autoNext) {
        lock.lock();

        __playState.setIsPlaying(false);
        Mix_FreeMusic(__currentMusic);
        __currentMusic = nullptr;

        try {
            if(__playState.getPlaylistFrom().empty()) {
                __playState.setPlayedMedia(__mediaPlayerService->getNextMediaFile(__playState.getPlayedMedia().getFilePath()));
            }

            else {
                __playState.setPlayedMedia(__mediaPlayerService->getNextMediaFile(
                    __playState.getPlayedMedia().getFilePath(),
                    __playState.getPlaylistFrom()
                ));
            }

            __playThread = std::thread([this]() { playCurrentTrack(); });
            __playThread.detach();
            
        } catch(std::exception &e) {
            return;
        }
    }
}

void MediaPlayController::pause() {
    std::unique_lock<std::mutex> lock(__playMutex);
    if (__playState.isPlaying()) {
        if (Mix_PausedMusic()) {
            Mix_ResumeMusic();
        } else {
            Mix_PauseMusic();
        }
    }
}

void MediaPlayController::next() {
    std::unique_lock<std::mutex> lock(__playMutex);

    try {
        if(__playState.getPlaylistFrom().empty()) {
            __playState.setPlayedMedia(__mediaPlayerService->getNextMediaFile(__playState.getPlayedMedia().getFilePath()));
        }

        else {
            __playState.setPlayedMedia(__mediaPlayerService->getNextMediaFile(
                __playState.getPlayedMedia().getFilePath(),
                __playState.getPlaylistFrom()
            ));
        }
    } catch(std::exception &e) {
        return;
    }

    if (__playState.isPlaying()) {
        __playState.setIsPlaying(false);
        __autoNext = false;
        Mix_HaltMusic();
        if (__playThread.joinable()) {
            __playThread.join();
        }
    }
    
    lock.unlock();
    play();
}

void MediaPlayController::previous() {
    std::unique_lock<std::mutex> lock(__playMutex);

    try {
        if(__playState.getPlaylistFrom().empty()) {
            __playState.setPlayedMedia(__mediaPlayerService->getPreviousMediaFile(__playState.getPlayedMedia().getFilePath()));
        }

        else {
            __playState.setPlayedMedia(__mediaPlayerService->getPreviousMediaFile(
                __playState.getPlayedMedia().getFilePath(),
                __playState.getPlaylistFrom()
            ));
        }
    } catch(std::exception &e) {
        return;
    }

    if (__playState.isPlaying()) {
        __playState.setIsPlaying(false);
        __autoNext = false;
        Mix_HaltMusic();
        if (__playThread.joinable()) {
            __playThread.join();
        }
    }
    lock.unlock();
    play();
}

bool MediaPlayController::changeVolume(bool increasing) {
    if(increasing) {
        __currentVolume = std::min(__currentVolume + 32, MIX_MAX_VOLUME);
    }
    else {
        __currentVolume = std::max(__currentVolume - 32, 0);
    }
    Mix_VolumeMusic(__currentVolume);
    return true;
}

bool MediaPlayController::setVolumeByPercent(int percent) {
    if(percent > 100) percent = 100;
    if(percent < 0) percent = 0;
    __currentVolume = (int) ((percent / 100.0) * (MIX_MAX_VOLUME));
    Mix_VolumeMusic(__currentVolume);
    return true;
}

std::pair<int,int> MediaPlayController::getCurrentTime() {
    if (__currentMusic == nullptr) {
        return {-1, -1};
    }

    int totalLength = (int) Mix_MusicDuration(__currentMusic);
    int positionSeconds = (int) Mix_GetMusicPosition(__currentMusic);
    return {positionSeconds, totalLength};
}

void MediaPlayController::savePlayState() {
    Application::savePlayState(__playState);
}

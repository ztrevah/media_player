#ifndef PLAYER_CONTROLLER_H
#define PLAYER_CONTROLLER_H

#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <SDL2/SDL_mixer.h> 
#include "MediaFileListController.h"
#include "PlaylistController.h"
#include "BaseController.h"
#include "../../models/inc/MediaPlayState.h"
#include "../../services/interfaces/IMediaPlayerService.h"

class MediaPlayController : public BaseController {
private:
    IMediaPlayerService* __mediaPlayerService;
    // std::string configDirectory; // Store config directory
    MediaPlayState __playState;
    Mix_Music* __currentMusic;
    std::thread __playThread;
    std::mutex __playMutex;
    int __currentVolume;
    bool __autoNext;
    void playCurrentTrack();
    void savePlayState();
    MediaPlayController(const MediaPlayState& playState = Application::getLastPlayState());
    static MediaPlayController* instance;
public:
    ~MediaPlayController();
    static MediaPlayController* getInstance();
    static void removeInstance();
    void play();
    void play(const std::string& filePath);
    void playFromPlaylist(const std::string& playlistName, const std::string& filePath = "");
    void pause();
    void next();
    void previous();

    MediaPlayState getPlayState() const;
    Mix_Music* getCurrentMusic() const;
    int getCurrentVolume() const;
    
    bool changeVolume(bool increasing);
    bool setVolumeByPercent(int percent);
    std::pair<int, int> getCurrentTime();

    void openView(IView* previousView = nullptr);
    void closeView();
};

#endif // PLAYER_CONTROLLER_H
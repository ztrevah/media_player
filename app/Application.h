// Application.h
#ifndef APPLICATION_H
#define APPLICATION_H

#include <string>
#include <map>
#include <vector>
#include "controllers/inc/BaseController.h"
#include "models/inc/Playlist.h"
#include "models/inc/MediaPlayState.h"

#include "services/interfaces/IMediaFileService.h"
#include "services/interfaces/IMediaPlayerService.h"
#include "services/interfaces/IMetadataService.h"
#include "services/interfaces/IPlaylistService.h"

#include "repositories/interfaces/IMediaFileRepository.h"
#include "repositories/interfaces/IPlaylistRepository.h"

class BoardConnector;

class Application {
private:
    static std::string __startDirectory;
    static std::vector<BaseController*> __controllerList;
    static MediaPlayState __lastPlayState;
    static IMediaFileRepository* __mediaFileRepository;
    static IPlaylistRepository* __playlistRepository;
    static IMediaPlayerService* __mediaPlayerService;
    BoardConnector* __boardConnector;
    static void createConfigDirectory();
public:
    Application(const std::string& startDirectory);
    ~Application();
    void start();
    static std::string getConfigDirectory();
    static MediaPlayState getLastPlayState();
    static std::string getStartDirectory();

    static IMediaFileRepository* getMediaFileRepository();
    static IPlaylistRepository* getPlaylistRepository();
    static IMediaPlayerService* getMediaPlayerService();

    static void savePlayState(const MediaPlayState& playState);
};

#endif // APPLICATION_H
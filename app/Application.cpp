#include "Application.h"
#include <iostream>
#include <filesystem>
#include <fstream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#include "controllers/inc/HomeController.h"
#include "controllers/inc/MediaPlayController.h"
#include "utils/inc/utils.h"
#include "repositories/inc/MediaFileRepository.h"
#include "repositories/inc/PlaylistRepository.h"
#include "services/inc/MediaPlayerService.h"
#include "board/inc/BoardConnector.h"


std::string Application::__startDirectory = "./";

std::vector<BaseController*> Application::__controllerList = {};

IMediaFileRepository* Application::__mediaFileRepository = nullptr;

IPlaylistRepository* Application::__playlistRepository = nullptr;

IMediaPlayerService* Application::__mediaPlayerService = nullptr;

MediaPlayState Application::__lastPlayState = MediaPlayState();

std::string Application::getConfigDirectory() {
    return Application::__startDirectory + "/.media_player_cli"; // Config directory within the starting directory
}

void Application::createConfigDirectory() {
    std::string configDir = getConfigDirectory();
    if (!std::filesystem::exists(configDir)) {
        std::filesystem::create_directories(configDir);
    }
}

Application::Application(const std::string& startDirectory) {
    Application::__startDirectory = startDirectory;
    createConfigDirectory();
    __mediaFileRepository = new MediaFileRepository(startDirectory);
    std::string playlistDir = getConfigDirectory() + "/playlists";
    if (!std::filesystem::exists(playlistDir)) {
        std::filesystem::create_directories(playlistDir);
    }
    __playlistRepository = new PlaylistRepository(playlistDir);

    __mediaPlayerService = new MediaPlayerService(__mediaFileRepository, __playlistRepository);

    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        exit(0);
        // Handle error appropriately (e.g., exit application)
    }
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL_Init Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        exit(0);
    }
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        std::cerr << "Mix_OpenAudio Error: " << Mix_GetError() << std::endl;
        SDL_Quit();
        exit(0);
    }
    // Initialize SDL_ttf
    if (TTF_Init() < 0) {
        std::cerr << "SDL_ttf Error: " << SDL_GetError() << std::endl;
        SDL_Quit();
        exit(0);
    }

    __boardConnector = new BoardConnector("/dev/ttyACM0");
}

Application::~Application() {
    if(__boardConnector) {
        delete __boardConnector;
        __boardConnector = nullptr;
    }
    if(__mediaFileRepository) {
        delete __mediaFileRepository;
        __mediaFileRepository = nullptr;
    }
    if(__playlistRepository) {
        delete __playlistRepository;
        __playlistRepository = nullptr;
    }
    if(__mediaPlayerService) {
        delete __mediaPlayerService;
        __mediaPlayerService = nullptr;
    }    
    MediaPlayController::removeInstance();

    Mix_HaltMusic();
    Mix_CloseAudio();
    SDL_Quit();
    TTF_Quit();
}

void Application::start() {
    __boardConnector->startConnection();
    BaseController* new_home_controller = new HomeController;
    new_home_controller->openView();
    delete new_home_controller;
}

MediaPlayState Application::getLastPlayState() {
    std::string configFilePath = Application::getConfigDirectory() + "/player_config.txt";
    std::ifstream configFile(configFilePath);

    __lastPlayState = MediaPlayState();

    if (configFile.is_open()) {
        std::string filePathLine;
        std::string secondsLine;
        std::string playlistNameLine;

        if (std::getline(configFile, filePathLine) 
            && std::getline(configFile, secondsLine) 
            && std::getline(configFile, playlistNameLine)) 
        {
            __lastPlayState.setPlayedMedia(MediaFile(filePathLine));

            try {
                __lastPlayState.setPauseAt(std::stoi(secondsLine));
            } catch (const std::invalid_argument& e) {
                std::cerr << "Error: Invalid second value in config file." << std::endl;
            } catch (const std::out_of_range& e){
                std::cerr << "Error: second value out of range in config file." << std::endl;
            }

            __lastPlayState.setPlaylistFrom(playlistNameLine);

        } else {
            std::cerr << "Error: Config file is incomplete." << std::endl;
        }

        configFile.close();
    } else {
        std::cerr << "Error: Could not open config file: " << configFilePath << std::endl;
    }

    return Application::__lastPlayState;
}

std::string Application::getStartDirectory() {
    return __startDirectory;
}

void Application::savePlayState(const MediaPlayState& playState) {
    std::ofstream outfile(getConfigDirectory() + "/player_config.txt");
    if (outfile.is_open()) {
        outfile << playState.getPlayedMedia().getFilePath() << std::endl;
        outfile << playState.getPauseAt() << std::endl;
        outfile << playState.getPlaylistFrom() << std::endl;
        outfile.close();
    } else {
        std::cerr << "Error: Unable to save media player state into player_config.txt\n";
    }
}

IMediaFileRepository* Application::getMediaFileRepository() {
    return __mediaFileRepository;
}
IPlaylistRepository* Application::getPlaylistRepository() {
    return __playlistRepository;   
}

IMediaPlayerService* Application::getMediaPlayerService() {
    return __mediaPlayerService;
}

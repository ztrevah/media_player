// controllers/PlaylistController.cpp
#include "../inc/PlaylistController.h"
#include "../../views/inc/PlaylistView.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <filesystem>

#include "../../Application.h"
#include "../../controllers/inc/MetadataController.h"
#include "../../controllers/inc/MediaPlayController.h"
#include "../../services/inc/MetadataService.h"

PlaylistController::PlaylistController(IPlaylistService* playlistService): BaseController(), __playlistService(playlistService) {
    
}

PlaylistController::~PlaylistController() {
    closeView();
}

void PlaylistController::openView(IView* previousView) {
    if(this->view == nullptr) {
        this->view = new PlaylistView(this, previousView);
    }    
    this->view->display();
    
    closeView();
}

void PlaylistController::closeView() {
    if(this->view != nullptr) {
        delete view;
        view = nullptr;
    }
}

Playlist PlaylistController::getPlaylist(const std::string& name) const {
    return __playlistService->getPlaylistByName(name);
}

Playlist PlaylistController::createPlaylist(const std::string& name, const std::vector<MediaFile>& media) {
    return __playlistService->addPlaylist(name, media);
}

Playlist PlaylistController::updatePlaylist(const std::string& name, const std::vector<MediaFile>& media) {
    return __playlistService->updatePlaylist(name, media);
}

Playlist PlaylistController::deletePlaylist(const std::string& name) {
    return __playlistService->removePlaylist(name);
}

std::vector<Playlist> PlaylistController::getPlaylists() const {
    return __playlistService->getAllPlaylists();
}


void PlaylistController::openMetadataView(MediaFile mediaFile) {
    MetadataService* service = new MetadataService();
    BaseController* controller = new MetadataController(service, mediaFile);
    controller->openView(this->view);

    delete controller;
    delete service;
}

void PlaylistController::openMediaPlayView(std::string playlistName, std::string filePath) {
    MediaPlayController* controller = MediaPlayController::getInstance();
    controller->playFromPlaylist(playlistName, filePath);
    controller->openView(this->view);
}
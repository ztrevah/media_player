#include "../inc/HomeController.h"

#include "../../Application.h"
#include "../../views/inc/HomeView.h"
#include "../inc/MediaFileListController.h"
#include "../inc/MediaPlayController.h"
#include "../inc/MetadataController.h"
#include "../inc/PlaylistController.h"
#include "../../services/inc/MediaFileService.h"
#include "../../services/inc/PlaylistService.h"
#include "../../services/inc/MetadataService.h"
#include "../../services/inc/MediaPlayerService.h"

HomeController::HomeController() : BaseController() {
    // view = new HomeView(this);
}

HomeController::~HomeController() {
    closeView();
}

void HomeController::openView(IView* previousView) {
    if(view == nullptr) {
        view = new HomeView(this, previousView);
    }
    view->display();

    closeView();
}

void HomeController::closeView() {
    if(this->view != nullptr) {
        delete view;
        view = nullptr;
    }
}

void HomeController::openMediaFileListView() {
    MediaFileService* service = new MediaFileService(Application::getMediaFileRepository());
    BaseController* controller = new MediaFileListController(service);
    controller->openView(this->view);

    delete controller;
    delete service;
}

void HomeController::openPlaylistView() {
    PlaylistService* service = new PlaylistService(Application::getPlaylistRepository());
    BaseController* controller = new PlaylistController(service);
    controller->openView(this->view);

    delete controller;
    delete service;
}

void HomeController::openMediaPlayView() {
    MediaPlayController* controller = MediaPlayController::getInstance();
    controller->openView(this->view);
}   
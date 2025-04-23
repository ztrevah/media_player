// controllers/MediaController.cpp
#include "../inc/MediaFileListController.h"
#include <iostream>
#include <filesystem>

#include "../../views/inc/MediaFileListView.h"
#include "../inc/MetadataController.h"
#include "../../services/inc/MetadataService.h"
#include "../../controllers/inc/MediaPlayController.h"

MediaFileListController::MediaFileListController(IMediaFileService* mediaFileService, int pageSize)
    : BaseController(), __mediaFileService(mediaFileService), __pageSize(pageSize) {
    
}

MediaFileListController::~MediaFileListController() {
    closeView();
}

void MediaFileListController::openView(IView* previousView) {
    if(this->view == nullptr) {
        view = new MediaFileListView(this, previousView);
    }
    this->view->display();

    closeView();
}

void MediaFileListController::closeView() {
    if(this->view != nullptr) {
        delete view;
        view = nullptr;
    }
}

void MediaFileListController::openMetadataView(MediaFile mediaFile) {
    MetadataService* service = new MetadataService();
    BaseController* controller = new MetadataController(service, mediaFile);
    controller->openView(this->view);

    delete controller;
    delete service;
}

void MediaFileListController::openMediaPlayView(MediaFile mediaFile) {
    MediaPlayController* controller = MediaPlayController::getInstance();
    controller->play(mediaFile.getFilePath());
    controller->openView(this->view);
}

std::vector<MediaFile> MediaFileListController::getMediaList() const {
    return __mediaFileService->getAllMediaFiles();
}

size_t MediaFileListController::getMediaListSize() const {
    return __mediaFileService->getAllMediaFiles().size();
}

std::vector<MediaFile> MediaFileListController::getMediaList(int pageNumber) const {
    return __mediaFileService->getMediaFiles(pageNumber, __pageSize);
}

int MediaFileListController::getPageSize() const {
    return __pageSize;
}

void MediaFileListController::setPageSize(int pageSize) {
    __pageSize = pageSize;
}
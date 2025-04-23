// controllers/MetadataController.cpp
#include "../inc/MetadataController.h"
#include <iostream>
#include <taglib/tag.h>
#include <taglib/fileref.h>
#include <taglib/mpegfile.h>
#include <taglib/flacfile.h>
#include <taglib/wavfile.h>
#include <taglib/tpropertymap.h>

#include "../../views/inc/MetadataView.h"
#include "../../utils/inc/utils.h"

MetadataController::MetadataController(IMetadataService* metadataService, const MediaFile& mediaFile) 
    :  BaseController(), __metadataService(metadataService), __mediaFile(mediaFile) {

}

MetadataController::~MetadataController() {
    closeView();
}

void MetadataController::openView(IView* previousView) {
    if(this->view == nullptr) {
        this->view = new MetadataView(this, previousView);
    }
    this->view->display();

    closeView();
}
void MetadataController::closeView() {
    if(this->view != nullptr) {
        delete view;
        view = nullptr;
    }
}

MediaFile MetadataController::getCurrentMediaFile() const {
    return __mediaFile;
}

AudioMetadata MetadataController::getAudioMetadata(const std::string& filePath) { // [cite: 2]
    return __metadataService->getAudioMetadata(__mediaFile);
}

VideoMetadata MetadataController::getVideoMetadata(const std::string& filePath) {
    return __metadataService->getVideoMetadata(__mediaFile);
}

bool MetadataController::upsertAudioMetadata(const std::string& filePath, AudioMetadata& audioMetadata) {
    try {
        __metadataService->updateAudioMetadata(__mediaFile, audioMetadata);
        return true;
    } catch (std::exception &e) {
        return false;
    }
}

bool MetadataController::upsertVideoMetadata(const std::string& filePath, VideoMetadata& videoMetadata) {
    try {
        __metadataService->updateVideoMetadata(__mediaFile, videoMetadata);
        return true;
    } catch (std::exception &e) {
        return false;
    }
}
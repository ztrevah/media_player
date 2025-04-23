// models/Playlist.cpp
#include "../inc/Playlist.h"

Playlist::Playlist(const std::string& name, const std::vector<MediaFile>& mediaList)
    : name_(name), mediaFileList_(mediaList) {}

std::string Playlist::getName() const {
    return name_;
}

const std::vector<MediaFile>& Playlist::getMediaList() const {
    return mediaFileList_;
}

void Playlist::setName(const std::string &name) {
    this->name_ = name;
}

void Playlist::setMediaFileList(const std::vector<MediaFile> &mediaFileList) {
    this->mediaFileList_ = mediaFileList;
}

void Playlist::addMediaFile(const MediaFile &mediaFile) {
    this->mediaFileList_.push_back(mediaFile);
}

void Playlist::removeMediaFile(const MediaFile &mediaFile) {
    for(auto i=mediaFileList_.begin();i!=mediaFileList_.end();i++) {
        if((*i).getFilePath() == mediaFile.getFilePath()) {
            this->mediaFileList_.erase(i);
            return;
        }
    }
}

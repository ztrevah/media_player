// models/Playlist.h
#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <string>
#include <vector>
#include "MediaFile.h"

class Playlist {
private:
    std::string name_;
    std::vector<MediaFile> mediaFileList_;
public:
    Playlist(const std::string& name, const std::vector<MediaFile>& mediaList = {});
    std::string getName() const;
    const std::vector<MediaFile>& getMediaList() const;
    void setName(const std::string &name);
    void setMediaFileList(const std::vector<MediaFile> &mediaFileList);
    void addMediaFile(const MediaFile &mediaFile);
    void removeMediaFile(const MediaFile &mediaFile);
};

#endif // PLAYLIST_H
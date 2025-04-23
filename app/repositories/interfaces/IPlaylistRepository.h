#ifndef I_PLAYLIST_REPOSITORY_H
#define I_PLAYLIST_REPOSITORY_H

#include <string>
#include <vector>
#include "IRepository.h"
#include "../../models/inc/MediaFile.h"
#include "../../models/inc/Playlist.h"

class IPlaylistRepository : public IRepository {
public:
    IPlaylistRepository(std::string repositoryDirectory = ".") : IRepository(repositoryDirectory) {};
    ~IPlaylistRepository() {};

    virtual std::vector<Playlist> getAll() = 0;
    virtual Playlist getByName(std::string name) = 0;
    virtual Playlist add(std::string name, std::vector<MediaFile> mediaFileList) = 0;
    virtual Playlist remove(std::string name) = 0;
    virtual Playlist update(std::string name, std::vector<MediaFile> mediaFileList) = 0;
};

#endif
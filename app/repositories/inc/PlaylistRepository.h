#ifndef PLAYLIST_REPOSITORY_H
#define PLAYLIST_REPOSITORY_H

#include <string>
#include <vector>
#include "../interfaces/IPlaylistRepository.h"
#include "../../models/inc/MediaFile.h"
#include "../../models/inc/Playlist.h"

class PlaylistRepository : public IPlaylistRepository {
private:
    std::vector<Playlist> __playlists;
    void loadPlaylists();
    void save();
public:
    PlaylistRepository(std::string repositoryDirectory = ".");
    ~PlaylistRepository();

    std::vector<Playlist> getAll();
    Playlist getByName(std::string name);
    Playlist add(std::string name, std::vector<MediaFile> mediaFileList);
    Playlist remove(std::string name);
    Playlist update(std::string name, std::vector<MediaFile> mediaFileList);
};

class PlaylistRepositoryException : public std::exception {
private:
    std::string message; // Store the custom error message

public:
    // 2. Constructor that takes a custom message
    PlaylistRepositoryException(const std::string& msg) : message(msg) {}

    // 3. Override the what() virtual function
    const char* what() const noexcept override {
        return message.c_str(); // Return the stored message as a C-style string
    }
};

#endif
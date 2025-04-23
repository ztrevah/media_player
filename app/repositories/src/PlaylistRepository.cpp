#include "../inc/PlaylistRepository.h"

#include <filesystem>
#include <fstream>
#include <iostream>

void PlaylistRepository::loadPlaylists() {
    __playlists.clear();
    for (const auto& entry : std::filesystem::directory_iterator(__repositoryDirectory)) {
        if (entry.is_regular_file() && entry.path().extension() == ".txt") {
            std::ifstream file(entry.path());
            if (file.is_open()) {
                std::string line;
                std::vector<MediaFile> mediaList;
                while (std::getline(file, line)) {
                    mediaList.emplace_back(line);
                }
                __playlists.emplace_back(entry.path().stem().string(), mediaList);
                file.close();
            }
        }
    }
}

PlaylistRepository::PlaylistRepository(std::string repositoryDirectory) : IPlaylistRepository(repositoryDirectory) {
    loadPlaylists();
}

PlaylistRepository::~PlaylistRepository() {

}

std::vector<Playlist> PlaylistRepository::getAll() {
    loadPlaylists();
    return __playlists;
}

Playlist PlaylistRepository::getByName(std::string name) {
    for(const auto &p: __playlists) {
        if(p.getName() == name) {
            return p;
        }
    }

    throw PlaylistRepositoryException("Playlist \"" + name + "\" not found.");
}

Playlist PlaylistRepository::add(std::string name, std::vector<MediaFile> mediaFileList) {
    try {
        Playlist p = getByName(name);
    } catch(std::exception &e) {
        Playlist p(name, mediaFileList);
        __playlists.push_back(p);
        save();
        return p;
    }
    
    throw PlaylistRepositoryException("Playlist \"" + name + "\" has already existed.");
}

Playlist PlaylistRepository::remove(std::string name) {
    Playlist p = getByName(name);
    for(auto i=__playlists.begin();i!=__playlists.end(); i++) {
        if((*i).getName() == name) {
            Playlist p = *i;
            __playlists.erase(i);
            try {
                std::string fileToDelete = __repositoryDirectory + "/" + p.getName() + ".txt";
                if (std::filesystem::exists(fileToDelete)) {
                    if (std::filesystem::is_regular_file(fileToDelete)) {
                        std::filesystem::remove(fileToDelete);
                    } 
                }
                return p;
            } catch (const std::filesystem::filesystem_error& e) {
                throw PlaylistRepositoryException("Filesystem error occurred.");
            }
        }
    }

    throw PlaylistRepositoryException("Playlist \"" + name + "\" not found.");
}

Playlist PlaylistRepository::update(std::string name, std::vector<MediaFile> mediaFileList) {
    Playlist p = getByName(name);
    for(auto i=__playlists.begin();i!=__playlists.end(); i++) {
        if((*i).getName() == name) {
            (*i).setMediaFileList(mediaFileList);
            save();
            return *i;
        }
    }

    throw PlaylistRepositoryException("Playlist \"" + name + "\" not found.");
}

void PlaylistRepository::save() {
    for(const auto &p: __playlists) {
        std::ofstream file(__repositoryDirectory + "/" + p.getName() + ".txt");
        if (file.is_open()) {
            for (const auto& media : p.getMediaList()) {
                file << media.getFilePath() << std::endl;
            }
            file.close();
        } else {
            throw PlaylistRepositoryException("Error: Unable to save playlist.\n");
        }
    }
   
}


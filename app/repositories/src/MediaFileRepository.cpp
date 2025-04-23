#include "../inc/MediaFileRepository.h"

#include <filesystem>

MediaFileRepository::MediaFileRepository(std::string repositoryDirectory) : IMediaFileRepository(repositoryDirectory) {
    loadMediaFileList();
}

MediaFileRepository::~MediaFileRepository() {

}

void MediaFileRepository::loadMediaFileList() {
    std::vector<MediaFile> mediaFileList;
    for (const auto& entry : std::filesystem::recursive_directory_iterator(__repositoryDirectory)) {
        if (entry.is_regular_file()) {
            std::string extension = entry.path().extension().string();
            if (extension == ".mp3" || extension == ".wav" || extension == ".flac" ||
                extension == ".mp4" || extension == ".avi" || extension == ".mkv") { // Basic audio/video check
                mediaFileList.emplace_back(entry.path().string());
            }
        }
    }
    
    __mediaFileList = mediaFileList;
}

std::vector<MediaFile> MediaFileRepository::getAll() {
    loadMediaFileList();
    return __mediaFileList;
}

std::vector<MediaFile> MediaFileRepository::getAllAudioFile() {
    loadMediaFileList();
    std::vector<MediaFile> res;
    for(const auto& f: getAll()) {
        std::string extension = f.getFilePath().substr(f.getFilePath().find_last_of('.'));
        if (extension == ".mp3" || extension == ".wav" || extension == ".flac") {
            res.push_back(f);
        }
    }
    return res;
}

std::vector<MediaFile> MediaFileRepository::get(int pageNumber, int pageSize) {
    if(__mediaFileList.size() == 0) { return __mediaFileList; }
    
    int startIndex = (pageNumber - 1) * pageSize;
    int endIndex = std::min(startIndex + pageSize, (int)__mediaFileList.size());

    if(startIndex > endIndex) { return {}; }

    std::vector<MediaFile> res(__mediaFileList.begin() + startIndex, __mediaFileList.begin() + endIndex);

    return res;
}

MediaFile MediaFileRepository::getByFilePath(std::string filePath) {
    for(const auto &f: __mediaFileList) {
        if(f.getFilePath() == filePath) return f;
    }

    throw MediaFileRepositoryException("Cannot find the media file: " + filePath);
}

MediaFile MediaFileRepository::getAudioFileByFilePath(std::string filePath) {
    for(const auto &f: getAllAudioFile()) {
        if(f.getFilePath() == filePath) return f;
    }

    throw MediaFileRepositoryException("Cannot find the media file: " + filePath);
}

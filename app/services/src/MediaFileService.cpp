#include "../inc/MediaFileService.h"

MediaFileService::MediaFileService(IMediaFileRepository* mediaFileRepository) {
    __mediaFileRepository = mediaFileRepository;
}

MediaFileService::~MediaFileService() {

}

std::vector<MediaFile> MediaFileService::getAllMediaFiles() {
    return __mediaFileRepository->getAll();
}

std::vector<MediaFile> MediaFileService::getAllAudioFiles() {
    return __mediaFileRepository->getAllAudioFile();
}

std::vector<MediaFile> MediaFileService::getMediaFiles(int pageNumber, int pageSize) {
    return __mediaFileRepository->get(pageNumber, pageSize);
}

MediaFile MediaFileService::getMediaFileByPath(std::string filePath) {
    return __mediaFileRepository->getByFilePath(filePath);
}

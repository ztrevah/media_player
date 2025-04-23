#ifndef I_REPOSITORY_H
#define I_REPOSITORY_H

#include <string>
#include <filesystem>

class IRepository {
protected:
    std::string __repositoryDirectory;

public:
    IRepository(std::string repositoryDirectory = ".") : __repositoryDirectory(repositoryDirectory) {
        if (!std::filesystem::exists(repositoryDirectory)) {
            std::filesystem::create_directories(repositoryDirectory);
        }
    };
    virtual ~IRepository() {};
    const std::string& getRepositoryDirectory() const { return __repositoryDirectory; }
};

#endif
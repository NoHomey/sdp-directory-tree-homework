#include "../../DirectoryTree/DirectoryTree.h"
#include <cstring>

DirectoryTree::FilesMapper::FilesInDirectory::FilesInDirectory(const Directory* directory) noexcept
: directory{directory}, files{} { }

DirectoryTree::FilesMapper::Files* DirectoryTree::FilesMapper::getFilesForDirectory(const Directory* directory) noexcept {
    const std::size_t directoriesWithFilesCount = mapped.size();
    for(std::size_t index = 0; index < directoriesWithFilesCount; ++index) {
        if(mapped[index].directory == directory) {
            return &mapped[index].files;
        }
    }
    return nullptr;
}

const DirectoryTree::FilesMapper::Files* DirectoryTree::FilesMapper::getFilesForDirectory(const Directory* directory) const noexcept {
    return const_cast<FilesMapper* const>(this)->getFilesForDirectory(directory);
}

void DirectoryTree::FilesMapper::addFileToDirectory(const Directory* directory, const char* fileName) {
    Files* filesForDirectory = getFilesForDirectory(directory);
    if(filesForDirectory) {
        for(Files::File* file = filesForDirectory->first; file; file = file->next) {
            if(!std::strcmp(file->name(), fileName)) {
                file->markAsFound();
                return;
            }
        }
        filesForDirectory->insert(fileName);
    } else {
        if(!mapped.getAllocatorPtr()) {
            mapped.setAllocator(&DirectoryTree::allocator, initialFilesMapperCapacity);
        }
        mapped.push({directory});
        mapped[mapped.size() - 1].files.insert(fileName);
    }
}

std::size_t DirectoryTree::FilesMapper::countOfDirectoriesWithFiles() const noexcept {
    return mapped.size();
}

void DirectoryTree::FilesMapper::sortFileNames() noexcept {
    const std::size_t countOfDirectoriesWithFiles = this->countOfDirectoriesWithFiles();
    for(std::size_t index = 0; index < countOfDirectoriesWithFiles; ++index) {
        DirectoryTree::mergeSortFiles(mapped[index].files.first);
    }
}

void DirectoryTree::FilesMapper::markAllFilesAsDeleted() noexcept {
    const std::size_t countOfDirectoriesWithFiles = this->countOfDirectoriesWithFiles();
    for(std::size_t index = 0; index < countOfDirectoriesWithFiles; ++index) {
        for(Files::File* file = mapped[index].files.first; file; file = file->next) {
            file->markAsDeleted();
        }
    }
}

void DirectoryTree::FilesMapper::eraseAllDeletedFiles() noexcept {
    std::size_t countOfDirectoriesWithFiles = this->countOfDirectoriesWithFiles();
    std::size_t index = 0;
    while(index < countOfDirectoriesWithFiles) {
        Files& files = mapped[index].files;
        Files::File* file = files.first;
        while(file && file->isDeleted()) {
            files.first = file->next;
            Files::File::deleteFile(file);
            file = files.first;
        }
        if(file) {
            Files::File* prev = file;
            file = file->next;
            while(file) {
                if(file->isDeleted()) {
                    prev->next = file->next;
                    Files::File::deleteFile(file);
                    file = prev->next;
                } else {
                    file = file->next;
                }
            }
            ++index;
        } else {
            mapped.removeAtIndex(index);
            --countOfDirectoriesWithFiles;
        }
    }
}
#include "../../DirectoryTree/DirectoryTree.h"

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
        filesForDirectory->insert(fileName);
    } else {
        mapped.push({directory});
        mapped[mapped.size() - 1].files.insert(fileName);
    }
}
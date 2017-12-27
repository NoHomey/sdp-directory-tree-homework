#include "../../DirectoryTree.h"

DirectoryTree::FilesMapper::Files::Files() noexcept
: Files{nullptr} {}

DirectoryTree::FilesMapper::Files::Files(Files&& other) noexcept
: Files{other.first} { }

void DirectoryTree::FilesMapper::Files::insert(const char* fileName) {
    first = File::newFile(fileName, first);
}

DirectoryTree::FilesMapper::Files::Files(File* first) noexcept
: first{first} { }

void DirectoryTree::FilesMapper::Files::null() noexcept {
    first = nullptr;
}
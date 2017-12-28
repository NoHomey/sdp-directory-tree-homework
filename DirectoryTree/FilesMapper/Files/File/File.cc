#include "File.h"
#include <cstring>
#include <cassert>

DirectoryTree::FilesMapper::Files::File* DirectoryTree::FilesMapper::Files::File::newFile(const char* name, File* next) {
    assert(name && name[0]);
    const std::size_t bytesForName = std::strlen(name) + 1;
    char* ptr = reinterpret_cast<char*>(DirectoryTree::allocator.allocate(sizeof(File) + sizeof(FileStatus) + bytesForName));
    new (ptr) File{next};
    reinterpret_cast<File*>(ptr)->markAsNew();
    std::memcpy(ptr + sizeof(File) + sizeof(FileStatus), name, bytesForName);
    return reinterpret_cast<File*>(ptr);
}

const char* DirectoryTree::FilesMapper::Files::File::name() const noexcept {
    return reinterpret_cast<const char*>(this) + sizeof(File) + sizeof(FileStatus);
}

bool DirectoryTree::FilesMapper::Files::File::isNew() const noexcept {
    return isFileWithStatus(fileStatusNew);
}

bool DirectoryTree::FilesMapper::Files::File::isDeleted() const noexcept {
    return isFileWithStatus(fileStatusDeleted);
}

bool DirectoryTree::FilesMapper::Files::File::isFound() const noexcept  {
    return isFileWithStatus(fileStatusFound);
}

void DirectoryTree::FilesMapper::Files::File::markAsDeleted() noexcept {
    setFileStatus(fileStatusDeleted);
}

void DirectoryTree::FilesMapper::Files::File::markAsFound() noexcept {
    setFileStatus(fileStatusFound);
}

void DirectoryTree::FilesMapper::Files::File::markAsNew() noexcept  {
    setFileStatus(fileStatusNew);
}

bool DirectoryTree::FilesMapper::Files::File::isFileWithStatus(const FileStatus fileStatus) const noexcept {
    return *reinterpret_cast<const FileStatus*>(reinterpret_cast<const char*>(this) + sizeof(File)) == fileStatus;
}

void DirectoryTree::FilesMapper::Files::File::setFileStatus(const FileStatus fileStatus) noexcept {
    *reinterpret_cast<FileStatus*>(reinterpret_cast<char*>(this) + sizeof(File)) = fileStatus;
}

DirectoryTree::FilesMapper::Files::File::File(File* next) noexcept
: next{next} { }
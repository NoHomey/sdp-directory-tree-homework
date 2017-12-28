#include "Directory.h"
#include <cstring>
#include <cassert>

DirectoryTree::Directory* DirectoryTree::Directory::newDirectory(const char* name, const std::size_t nameLength) {
    assert(name && name[0]);
    const std::size_t bytesNeeded = sizeof(Directory) + nameLength + 1;
    char* ptr = reinterpret_cast<char*>(DirectoryTree::allocator.allocate(bytesNeeded));
    new (ptr) Directory;
    std::memcpy(ptr + sizeof(Directory), name, nameLength);
    ptr[sizeof(Directory) + nameLength] = '\0';
    return reinterpret_cast<Directory*>(ptr);
}

void DirectoryTree::Directory::deleteDirectory(Directory* directory) {
    assert(directory);
    const std::size_t bytesToRelease = sizeof(Directory) + std::strlen(directory->name()) + 1;
    DirectoryTree::allocator.release(directory, bytesToRelease);
}

const char* DirectoryTree::Directory::name() const noexcept {
    return reinterpret_cast<const char*>(this) + sizeof(Directory);
}

DirectoryTree::Directory::Directory() noexcept
: next{nullptr}, child{nullptr} { }
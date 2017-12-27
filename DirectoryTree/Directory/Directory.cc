#include "Directory.h"
#include <cstring>
#include <cassert>

ChunkAllocator DirectoryTree::Directory::allocator;

DirectoryTree::Directory* DirectoryTree::Directory::newDirectory(const char* name, const std::size_t nameLength) {
    assert(name && name[0]);
    char* ptr = reinterpret_cast<char*>(allocator.allocate(sizeof(Directory) + nameLength + 1));
    new (ptr) Directory{nameLength};
    std::memcpy(ptr + sizeof(Directory), name, nameLength);
    ptr[sizeof(Directory) + nameLength] = '\0';
    return reinterpret_cast<Directory*>(ptr);
}

const char* DirectoryTree::Directory::name() const noexcept {
    return reinterpret_cast<const char*>(this) + sizeof(Directory);
}

DirectoryTree::Directory::Directory(const std::size_t nameLength) noexcept
: next{nullptr}, child{nullptr}, nameLength{nameLength} { }
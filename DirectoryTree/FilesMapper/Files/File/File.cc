#include "File.h"
#include <cstring>
#include <cassert>

ChunkAllocator DirectoryTree::FilesMapper::Files::File::allocator;

DirectoryTree::FilesMapper::Files::File* DirectoryTree::FilesMapper::Files::File::newFile(const char* name, File* next) {
    assert(name && name[0]);
    const std::size_t bytesForName = std::strlen(name) + 1;
    char* ptr = reinterpret_cast<char*>(allocator.allocate(sizeof(File) + bytesForName));
    new (ptr) File{bytesForName - 1, next};
    std::memcpy(ptr + sizeof(File), name, bytesForName);
    return reinterpret_cast<File*>(ptr);
}

const char* DirectoryTree::FilesMapper::Files::File::name() const noexcept {
    return reinterpret_cast<const char*>(this) + sizeof(File);
}

DirectoryTree::FilesMapper::Files::File::File(const std::size_t nameLength, File* next) noexcept
: next{next}, nameLength{nameLength} { }
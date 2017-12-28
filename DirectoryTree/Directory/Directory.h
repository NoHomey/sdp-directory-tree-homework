#pragma once

#include "../DirectoryTree.h"

struct DirectoryTree::Directory {
public:
    static Directory* newDirectory(const char* name, const std::size_t nameLength);

    static void deleteDirectory(Directory* directory);

public:
    ~Directory() noexcept = default;

    Directory(const Directory& other) = delete;

    Directory(Directory&& other) noexcept = delete;

    Directory& operator=(const Directory& other) = delete;

    Directory& operator=(Directory&& other) noexcept = delete;

public:
    const char* name() const noexcept;

private:
    Directory() noexcept;

public:
    static ChunkAllocator allocator;

public:
    Directory* next;

    Directory* child;
};
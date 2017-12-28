#pragma once

#include "../../../DirectoryTree.h"

struct DirectoryTree::FilesMapper::Files::File {
public:
   static File* newFile(const char* name, File* next = nullptr); 

public:
    ~File() noexcept = default;

    File(const File& other) = delete;

    File(File&& other) noexcept = delete;

    File& operator=(const File& other) = delete;

    File& operator=(File&& other) noexcept = delete;

public:
    const char* name() const noexcept;

private:
    File(File* next = nullptr) noexcept;

public:
    File* next;
};
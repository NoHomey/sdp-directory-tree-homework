#pragma once

#include "../../../DirectoryTree.h"

struct DirectoryTree::FilesMapper::Files::File {
public:
   static File* newFile(const char* name, File* next = nullptr); 

   static void deleteFile(File* file);

public:
    ~File() noexcept = default;

    File(const File& other) = delete;

    File(File&& other) noexcept = delete;

    File& operator=(const File& other) = delete;

    File& operator=(File&& other) noexcept = delete;

public:
    const char* name() const noexcept;

    bool isNew() const noexcept;

    bool isDeleted() const noexcept;

    bool isFound() const noexcept;

public:
    void markAsDeleted() noexcept;

    void markAsFound() noexcept;

    void markAsNew() noexcept;

private:
    File(File* next = nullptr) noexcept;

private:
    using FileStatus = unsigned char;

private:
    bool isFileWithStatus(const FileStatus fileStatus) const noexcept;

    void setFileStatus(const FileStatus fileStatus) noexcept;

private:
    const FileStatus fileStatusNew = 1;

    const FileStatus fileStatusFound = 0;

    const FileStatus fileStatusDeleted = 2;

public:
    File* next;
};
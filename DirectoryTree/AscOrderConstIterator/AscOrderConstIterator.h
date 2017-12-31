#pragma once

#include "../DirectoryTree.h"
#include "FixedCapacityStackOfChars/FixedCapacityStackOfChars.thd"
#include "../../FixedCapacityStack/FixedCapacityStack.thd"
#include "../../Pair.thd"

class DirectoryTree::AscOrderConstIterator {
    friend DirectoryTree;

public:
    ~AscOrderConstIterator() noexcept;

    AscOrderConstIterator(const AscOrderConstIterator& other) = delete;

    AscOrderConstIterator(AscOrderConstIterator&& other) noexcept;

    AscOrderConstIterator& operator=(const AscOrderConstIterator& other) = delete;

    AscOrderConstIterator& operator=(AscOrderConstIterator&& other) noexcept = delete;

public:
    explicit operator bool() const noexcept;

    bool operator==(const AscOrderConstIterator& other) const noexcept;

    bool operator!=(const AscOrderConstIterator& other) const noexcept;

    Pair<const char*, char> operator*() const noexcept;

public:
    AscOrderConstIterator& operator++() noexcept;

    //AscOrderConstIterator operator++(int) noexcept;

private:
    static char getFileStatusMark(const DirectoryTree::FilesMapper::Files::File* file) noexcept;

private:
    AscOrderConstIterator() noexcept;

    AscOrderConstIterator(const DirectoryTree* directoryTree) noexcept;

    void init() noexcept;

    void addToFilePath(const char* str) noexcept;

    void setFileName() noexcept;

    void addDirToPath(const Directory* dir) noexcept;

    void removeFromFilePath() noexcept;
    
    void moveDownInDirectory(const DirectoryTree::Directory* directory) noexcept;

    void moveUp() noexcept;

    bool isValid() const noexcept;

    void null() noexcept;

    void release();

    bool safeRelease() noexcept;

private:
    const DirectoryTree* directoryTree;

    FixedCapacityStackOfChars<ChunkAllocator> filePath;

    FixedCapacityStack<const DirectoryTree::Directory*, ChunkAllocator> path;

    const DirectoryTree::FilesMapper::Files::File* file;
};
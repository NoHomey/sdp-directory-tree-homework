#pragma once

#include "../DirectoryTree.h"
#include "FixedCapacityStackOfChars/FixedCapacityStackOfChars.h"
#include "../../FixedCapacityStack/FixedCapacityStack.thd"

class DirectoryTree::AscOrderConstIterator {
    friend DirectoryTree;

public:
    AscOrderConstIterator() = delete;

    ~AscOrderConstIterator() noexcept;

    AscOrderConstIterator(const AscOrderConstIterator& other) = delete;

    AscOrderConstIterator(AscOrderConstIterator&& other) noexcept = default;

    AscOrderConstIterator& operator=(const AscOrderConstIterator& other) = delete;

    AscOrderConstIterator& operator=(AscOrderConstIterator&& other) noexcept = delete;

public:
    explicit operator bool() const noexcept;

    bool operator==(const AscOrderConstIterator& other) const noexcept;

    bool operator!=(const AscOrderConstIterator& other) const noexcept;

    bool operator!() const noexcept;

    const char* operator*() const noexcept;

public:
    AscOrderConstIterator& operator++() noexcept;

    //AscOrderConstIterator operator++(int) noexcept;

private:
    AscOrderConstIterator(const DirectoryTree* directoryTree) noexcept;

    void init() noexcept;

    void setFileName() noexcept;

    void addDirToPath(const Directory* dir) noexcept;

    void removeFromPath(const std::size_t toRemoveLength) noexcept;
    
    void moveDownInDirectory(const DirectoryTree::Directory* directory) noexcept;

    void moveUp() noexcept;

    bool isValid() const noexcept;

    void release() noexcept;

private:
    const DirectoryTree* directoryTree;

    char* memoryForFilePath;

    const DirectoryTree::Directory** memoryForPath;

    FixedCapacityStackOfChars filePath;

    FixedCapacityStack<const DirectoryTree::Directory*> path;

    const DirectoryTree::FilesMapper::Files::File* file;
};
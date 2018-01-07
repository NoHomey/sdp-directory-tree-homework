#pragma once

#include "../../DirectoryTree.h"
#include "../../../FileMatcher/FileMatcher.thd"

class DirectoryTree::AscOrderConstIteratorMatchingFileName {
    friend DirectoryTree;

public:
    ~AscOrderConstIteratorMatchingFileName() noexcept = default;

    AscOrderConstIteratorMatchingFileName(const AscOrderConstIteratorMatchingFileName& other) = delete;

    AscOrderConstIteratorMatchingFileName(AscOrderConstIteratorMatchingFileName&& other) noexcept = default;

    AscOrderConstIteratorMatchingFileName& operator=(const AscOrderConstIteratorMatchingFileName& other) = delete;

    AscOrderConstIteratorMatchingFileName& operator=(AscOrderConstIteratorMatchingFileName&& other) noexcept = delete;

public:
    explicit operator bool() const noexcept;

    bool operator==(const AscOrderConstIteratorMatchingFileName& other) const noexcept;

    bool operator!=(const AscOrderConstIteratorMatchingFileName& other) const noexcept;

    Pair<const char*, char> operator*() const noexcept;

    const char* currentFileName() const noexcept;

public:
    AscOrderConstIteratorMatchingFileName& operator++() noexcept;

    //AscOrderConstIteratorMatchingFileName operator++(int) noexcept;

    void release();

    bool safeRelease() noexcept;

private:
    AscOrderConstIteratorMatchingFileName() noexcept = delete;

    AscOrderConstIteratorMatchingFileName(const DirectoryTree* directoryTree, const char* pattern);

    void moveUntilMatchIsFound() noexcept;

private:
    AscOrderConstIterator iterator;

    FileMatcher<ChunkAllocator> fileMatcher;
};
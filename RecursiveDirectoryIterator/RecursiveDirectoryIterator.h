#pragma once

#include "../ChunkAllocator/ChunkAllocator.h"
#include "../DynamicArray/DynamicArray.thd"
#include <dirent.h>

class RecursiveDirectoryIterator {
public:
    RecursiveDirectoryIterator(const char* directoryName);

    ~RecursiveDirectoryIterator() noexcept = default;

    RecursiveDirectoryIterator(const RecursiveDirectoryIterator& other) = delete;

    RecursiveDirectoryIterator(RecursiveDirectoryIterator&& other) noexcept = delete;

    RecursiveDirectoryIterator& operator=(const RecursiveDirectoryIterator& other) = delete;

    RecursiveDirectoryIterator& operator=(RecursiveDirectoryIterator&& other) noexcept = delete;

public:
    explicit operator bool() const noexcept;

    const char* operator*() const noexcept;

public:
    RecursiveDirectoryIterator& operator++();

private:
    class DirectoryEntryStack {
    public:
        DirectoryEntryStack(ChunkAllocator& allocator) noexcept;

        ~DirectoryEntryStack() noexcept = default;

        DirectoryEntryStack(const DirectoryEntryStack& other) = delete;

        DirectoryEntryStack(DirectoryEntryStack&& other) noexcept = delete;

        DirectoryEntryStack& operator=(const DirectoryEntryStack& other) = delete;

        DirectoryEntryStack& operator=(DirectoryEntryStack&& other) noexcept = delete;

    public:
        bool isEmpty() const noexcept;

        const char* top() const noexcept;

    public:
        void push(const char* directoryEntryName);

        void pop();

    private:
        struct DirectoryEntry {
        public:
            DirectoryEntry(DirectoryEntry* prev) noexcept;

            ~DirectoryEntry() noexcept = default;

            DirectoryEntry(const DirectoryEntry& other) = delete;

            DirectoryEntry(DirectoryEntry&& other) noexcept = delete;

            DirectoryEntry& operator=(const DirectoryEntry& other) = delete;

            DirectoryEntry& operator=(DirectoryEntry&& other) noexcept = delete;

        public:
            const char* name() const noexcept;

        public:
            DirectoryEntry* prev;
        };

    private:
        DirectoryEntry* lastDirectoryEntry;

        ChunkAllocator& allocator;
    };

private:
    static bool isSpecialDirectory(const char* directoryName) noexcept;

    static bool isDirectoryEmpty(DIR* directoryEntryStream);

    static bool isDirectoryDelimiter(const char* directoryEntryName) noexcept;

private:
    bool isValid() const noexcept;

    void walkUntilFileIsReached();

    void release() noexcept;

private:
    const std::size_t initialFilePathCapacity = 4096 + 1;

private:
    ChunkAllocator allocator;

    DynamicArray<char, ChunkAllocator> filePath;

    DirectoryEntryStack directoryEntryStack;
};
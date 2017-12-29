#pragma once

#include "../DynamicArray/DynamicArray.thd"

class ChunkAllocator {
public:
    ChunkAllocator() noexcept;

    ~ChunkAllocator() noexcept;

    ChunkAllocator(const std::size_t chunkSize) noexcept;

    ChunkAllocator(const ChunkAllocator& other) = delete;

    ChunkAllocator(ChunkAllocator&& other) noexcept;

    ChunkAllocator& operator=(const ChunkAllocator& other) = delete;

    ChunkAllocator& operator=(ChunkAllocator&& other) noexcept;

public:
    void* allocate(const std::size_t bytes);

    void release(void* ptr, const std::size_t bytes);

    bool safeRelease(void* ptr, const std::size_t bytes) noexcept;

    bool reallocate(void* ptr, const std::size_t oldSizeInBytes, const std::size_t newSizeInBytes);

    void releaseAllMemory() noexcept;

private:
    static const std::size_t defaultChunkSize = 1 << 16;

private:
    bool releaseFromLast(void* ptr, const std::size_t bytes) noexcept;

    bool mergeUnusedMemory(void* ptr, const std::size_t bytes) noexcept;

    void deleteAllAllocatedChunk() noexcept;

    void null() noexcept;

private:
    struct Chunk;

private:
    struct UnusedMemory {
        UnusedMemory(char* ptr, const std::size_t bytes) noexcept;

        ~UnusedMemory() noexcept = default;

        UnusedMemory(const UnusedMemory& other) noexcept = default;

        UnusedMemory(UnusedMemory&& other) noexcept = default;

        UnusedMemory& operator=(const UnusedMemory& other) noexcept = default;

        UnusedMemory& operator=(UnusedMemory&& other) noexcept = default;

        char* ptr;

        std::size_t bytes;
    };

private:
    void addUnusedMemory(UnusedMemory&& memory);

private:
    Chunk* first;

    Chunk* last;

    std::size_t nextChunkSize;

    DynamicArray<UnusedMemory, ChunkAllocator> unusedMemory;
};

#include "Chunk/Chunk.h"
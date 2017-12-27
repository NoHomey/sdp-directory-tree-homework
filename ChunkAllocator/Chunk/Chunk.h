#pragma once

#include "../ChunkAllocator.h"

struct ChunkAllocator::Chunk {
public:
    Chunk(const std::size_t chunkCapacity) noexcept;

    ~Chunk() noexcept = default;

    Chunk(const Chunk& other) = delete;

    Chunk(Chunk&& other) noexcept;

    Chunk& operator=(const Chunk& other) = delete;

    Chunk& operator=(Chunk&& other) noexcept = delete;

public:
    Chunk* next;

    std::size_t used;

    const std::size_t capacity;
};
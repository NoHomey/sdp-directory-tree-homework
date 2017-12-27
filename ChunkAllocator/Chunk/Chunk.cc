#include "Chunk.h"

ChunkAllocator::Chunk::Chunk(const std::size_t chunkCapacity) noexcept
: next{nullptr}, used{0}, capacity{chunkCapacity} { }

ChunkAllocator::Chunk::Chunk(Chunk&& other) noexcept
: next{other.next}, used{other.used}, capacity{other.capacity} { }
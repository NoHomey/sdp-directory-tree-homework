#include "ChunkAllocator.h"
#include <cassert>
#include <new>

ChunkAllocator::UnusedMemory::UnusedMemory(char* ptr, const std::size_t bytes) noexcept
: ptr{ptr}, bytes{bytes} { }

ChunkAllocator::ChunkAllocator() noexcept
: ChunkAllocator{defaultChunkSize} { }

ChunkAllocator::~ChunkAllocator() noexcept {
    deleteAllAllocatedChunk();
    #ifndef NDEBUG
    null();
    #endif
}

ChunkAllocator::ChunkAllocator(const std::size_t chunkSize) noexcept
: first{nullptr}, last{nullptr}, nextChunkSize{chunkSize}, unusedMemory{} {
    assert(chunkSize >= defaultChunkSize);
}

ChunkAllocator::ChunkAllocator(ChunkAllocator&& other) noexcept
: first{other.first}, last{other.last}, nextChunkSize{other.nextChunkSize} { }

ChunkAllocator& ChunkAllocator::operator=(ChunkAllocator&& other) noexcept {
    if(this != &other) {
        deleteAllAllocatedChunk();
        first = other.first;
        last = other.last;
        nextChunkSize = other.nextChunkSize;
        other.null();
    }
    return *this;
}

void* ChunkAllocator::allocate(const std::size_t bytes) {
    char* bestFit = nullptr;
    std::size_t minLeft = -1;
    std::size_t index = 0;
    const std::size_t unusedChunksCount = unusedMemory.size();
    for(std::size_t i = 0; index < unusedChunksCount; ++i) {
        if(unusedMemory[i].bytes == bytes) {
            bestFit = unusedMemory[i].ptr;
            index = i;
            break;
        }
        if(unusedMemory[i].bytes > bytes) {
            const std::size_t left = unusedMemory[i].bytes - bytes;
            if(left < minLeft) {
                minLeft = left;
                bestFit = unusedMemory[i].ptr;
                index = i;
            }
        }
    }
    if(bestFit) {
        if(unusedMemory[index].bytes > bytes) {
            unusedMemory[index].bytes -= bytes;
            unusedMemory[index].ptr += bytes;
        } else {
            unusedMemory.removeAtIndex(index);
        }
    }
    if(!first || (!bestFit && ((last->capacity - last->used) < bytes))) {
        if(bytes > nextChunkSize) {
            nextChunkSize = bytes;
        }
        char* newChunk = new char[sizeof(Chunk) + nextChunkSize];
        new (newChunk) Chunk{nextChunkSize};
        Chunk* next = reinterpret_cast<Chunk*>(newChunk);
        if(last && (last->capacity - last->used)) {
            unusedMemory.push({
                reinterpret_cast<char*>(last) + sizeof(Chunk) + last->used,
                last->capacity - last->used
            });
        }
        last = first ? (last->next = next) : (first = next);
        bestFit = newChunk + sizeof(Chunk);
        last->used += bytes;
    }
    if(!bestFit) {
        bestFit = reinterpret_cast<char*>(last) + sizeof(Chunk) + last->used;
        last->used += bytes;
    }
    return bestFit;
}

void ChunkAllocator::release(void* ptr, const std::size_t bytes) {
    assert(last && ptr && bytes);
    if((last->used >= bytes) && ((reinterpret_cast<char*>(last) + sizeof(Chunk) + last->used - bytes) == ptr)) {
        last->used -= bytes;
    } else {
        unusedMemory.push({reinterpret_cast<char*>(ptr), bytes});
    }
}

void ChunkAllocator::deleteAllAllocatedChunk() noexcept {
    Chunk* chunk;
    while(first) {
        chunk = first;
        first = first->next;
        //chunk->~Chunk(); trivial default destructor
        delete[] reinterpret_cast<char*>(chunk);
    }
}

void ChunkAllocator::null() noexcept {
    first = last = nullptr;
}


#include "ChunkAllocator.h"
#include <cassert>
#include <new>
#include <utility>

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
: first{other.first}, last{other.last}, nextChunkSize{other.nextChunkSize}, unusedMemory{std::move(other.unusedMemory)} {
    other.null();
}

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
    assert(bytes);
    char* bestFit = nullptr;
    std::size_t minLeft = -1;
    std::size_t index = 0;
    const std::size_t unusedChunksCount = unusedMemory.size();
    for(std::size_t i = 0; i < unusedChunksCount; ++i) {
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
        Chunk* old = last;
        last = first ? (last->next = next) : (first = next);
        bestFit = newChunk + sizeof(Chunk);
        last->used += bytes;
        if(old && (old->capacity - old->used)) {
            addUnusedMemory({
                reinterpret_cast<char*>(old) + sizeof(Chunk) + old->used,
                old->capacity - old->used
            });
        }
    }
    if(!bestFit) {
        bestFit = reinterpret_cast<char*>(last) + sizeof(Chunk) + last->used;
        last->used += bytes;
    }
    return bestFit;
}

void ChunkAllocator::release(void* ptr, const std::size_t bytes) {
    assert(last && ptr && bytes);
    if(!releaseFromLast(ptr, bytes)) {
        addUnusedMemory({reinterpret_cast<char*>(ptr), bytes});
    }
}

bool ChunkAllocator::safeRelease(void* ptr, const std::size_t bytes) noexcept {
    assert(last && ptr && bytes);
    if(releaseFromLast(ptr, bytes)) {
        return true;
    }
    if(mergeUnusedMemory(ptr, bytes)) {
        return true;
    }
    if(unusedMemory.getAllocatorPtr() && (unusedMemory.capacity() > unusedMemory.size())) {
        unusedMemory.push({reinterpret_cast<char*>(ptr), bytes});
        return true;
    }
    return false;
}

bool ChunkAllocator::reallocate(void* ptr, const std::size_t oldSizeInBytes, const std::size_t newSizeInBytes) {
    assert(last && ptr && oldSizeInBytes && newSizeInBytes);
    if(oldSizeInBytes == newSizeInBytes) {
        return true;
    }
    if((reinterpret_cast<char*>(ptr) + oldSizeInBytes) == (reinterpret_cast<char*>(last) + sizeof(Chunk) + last->used)) {
        if(newSizeInBytes >= oldSizeInBytes) {
            last->used += (newSizeInBytes - oldSizeInBytes);
        } else {
            last->used -= (oldSizeInBytes - newSizeInBytes);
        }
        return true;
    }
    return false;
}

void ChunkAllocator::releaseAllMemory() noexcept {
    deleteAllAllocatedChunk();
    null();
}

bool ChunkAllocator::releaseFromLast(void* ptr, const std::size_t bytes) noexcept {
    assert(last && ptr && bytes);
    if((last->used >= bytes) && ((reinterpret_cast<char*>(last) + sizeof(Chunk) + last->used - bytes) == ptr)) {
        last->used -= bytes;
        return true;
    }
    return false;
}

bool ChunkAllocator::mergeUnusedMemory(void* ptr, const std::size_t bytes) noexcept {
    const std::size_t unusedChunksCount = unusedMemory.size();
    char* memory = reinterpret_cast<char*>(ptr);
    const char* addressAfterCurrentPointer = memory + bytes;
    for(std::size_t i = 0; i < unusedChunksCount; ++i) {
        if(unusedMemory[i].ptr == addressAfterCurrentPointer) {
            unusedMemory[i].ptr = memory;
            unusedMemory[i].bytes += bytes;
            return true;
        }
        if((unusedMemory[i].ptr + bytes) == memory) {
            unusedMemory[i].bytes += bytes;
            return true;
        }
    }
    return false;
}

void ChunkAllocator::deleteAllAllocatedChunk() noexcept {
    Chunk* chunk;
    unusedMemory.safeRelease();
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

void ChunkAllocator::addUnusedMemory(UnusedMemory&& memory) {
    if(!mergeUnusedMemory(memory.ptr, memory.bytes)) {
        if(!unusedMemory.getAllocatorPtr()) {
            const std::size_t one256OfNextChunkSize = nextChunkSize >> 8;
            const std::size_t unusedFromLast = last->capacity - last->used;
            const std::size_t min = one256OfNextChunkSize <= unusedFromLast ? one256OfNextChunkSize : unusedFromLast;
            unusedMemory.setAllocator(this, min);
        }
        unusedMemory.push(std::move(memory));
    }
}
#include "../RecursiveDirectoryIterator.h"
#include <cassert>
#include <cstring>

RecursiveDirectoryIterator::DirectoryEntryStack::DirectoryEntryStack(ChunkAllocator& allocator) noexcept
: lastDirectoryEntry{nullptr}, allocator{allocator} { }

bool RecursiveDirectoryIterator::DirectoryEntryStack::isEmpty() const noexcept {
    return !lastDirectoryEntry;
}

const char* RecursiveDirectoryIterator::DirectoryEntryStack::top() const noexcept {
    assert(!isEmpty());
    return lastDirectoryEntry->name();
}

void RecursiveDirectoryIterator::DirectoryEntryStack::push(const char* directoryEntryName) {
    assert(directoryEntryName && directoryEntryName[0]);
    const std::size_t bytesForName = std::strlen(directoryEntryName) + 1;
    const std::size_t bytesNeeded = sizeof(DirectoryEntry) + bytesForName;
    char* ptr = reinterpret_cast<char*>(allocator.allocate(bytesNeeded));
    new (ptr) DirectoryEntry{lastDirectoryEntry};
    std::memcpy(ptr + sizeof(DirectoryEntry), directoryEntryName, bytesForName);
    lastDirectoryEntry = reinterpret_cast<DirectoryEntry*>(ptr);
}

void RecursiveDirectoryIterator::DirectoryEntryStack::pop() {
    assert(!isEmpty());
    DirectoryEntry* last = lastDirectoryEntry;
    lastDirectoryEntry = last->prev;
    const std::size_t bytesForName = std::strlen(last->name()) + 1;
    allocator.release(last, sizeof(DirectoryEntry) + bytesForName);
}

RecursiveDirectoryIterator::DirectoryEntryStack::DirectoryEntry::DirectoryEntry(DirectoryEntry* prev) noexcept
: prev{prev} { }

const char* RecursiveDirectoryIterator::DirectoryEntryStack::DirectoryEntry::name() const noexcept {
    return reinterpret_cast<const char*>(this) + sizeof(DirectoryEntry);
}
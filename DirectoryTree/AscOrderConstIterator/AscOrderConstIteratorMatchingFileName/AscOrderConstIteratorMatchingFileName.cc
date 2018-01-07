#include "AscOrderConstIteratorMatchingFileName.h"
#include <cstring>

DirectoryTree::AscOrderConstIteratorMatchingFileName::operator bool() const noexcept {
    return static_cast<bool>(iterator);
}

bool DirectoryTree::AscOrderConstIteratorMatchingFileName::operator==(const AscOrderConstIteratorMatchingFileName& other) const noexcept {
    return (iterator == other.iterator) && (!std::strcmp(fileMatcher.getPattern(), other.fileMatcher.getPattern()));
}

bool DirectoryTree::AscOrderConstIteratorMatchingFileName::operator!=(const AscOrderConstIteratorMatchingFileName& other) const noexcept {
    return !operator==(other);
}

Pair<const char*, char> DirectoryTree::AscOrderConstIteratorMatchingFileName::operator*() const noexcept {
    return *iterator;
}

const char* DirectoryTree::AscOrderConstIteratorMatchingFileName::currentFileName() const noexcept {
    return iterator.currentFileName();
}

DirectoryTree::AscOrderConstIteratorMatchingFileName& DirectoryTree::AscOrderConstIteratorMatchingFileName::operator++() noexcept {
    if(iterator) {
        ++iterator;
    }
    moveUntilMatchIsFound();
    return *this;
}

bool DirectoryTree::AscOrderConstIteratorMatchingFileName::shouldMatchFileNameOnly(const char* pattern) noexcept {
    assert(pattern && pattern[0]);
    for(std::size_t index = 0; pattern[index]; ++index) {
        if(pattern[index] == '/') {
            return false;
        }
    }
    return true;
}

DirectoryTree::AscOrderConstIteratorMatchingFileName::AscOrderConstIteratorMatchingFileName(const DirectoryTree* directoryTree, const char* pattern)
: iterator{directoryTree},
fileMatcher(FileMatcher<ChunkAllocator>::createFileMatcher(pattern, &DirectoryTree::allocator)),
matchFileNameOnly{shouldMatchFileNameOnly(pattern)} {
    moveUntilMatchIsFound();
}

void DirectoryTree::AscOrderConstIteratorMatchingFileName::release() {
    iterator.release();
    fileMatcher.release();
}

bool DirectoryTree::AscOrderConstIteratorMatchingFileName::safeRelease() noexcept {
    return iterator.safeRelease() && fileMatcher.safeRelease();
}

const char* DirectoryTree::AscOrderConstIteratorMatchingFileName::toMatch() const noexcept {
    return matchFileNameOnly ? currentFileName() : (*iterator).first;
}

void DirectoryTree::AscOrderConstIteratorMatchingFileName::moveUntilMatchIsFound() noexcept {
    while(iterator && !fileMatcher.match(toMatch())) {
        ++iterator;
    }
}
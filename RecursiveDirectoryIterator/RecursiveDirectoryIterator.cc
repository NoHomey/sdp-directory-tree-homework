#include "RecursiveDirectoryIterator.h"
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>

RecursiveDirectoryIterator::RecursiveDirectoryIterator(const char* directoryName)
: allocator{}, filePath{initialFilePathCapacity, &allocator}, directoryEntryStack{allocator} {
    directoryEntryStack.push(directoryName);
    walkUntilFileIsReached();
}

const char* RecursiveDirectoryIterator::operator*() const noexcept {
    return filePath.data();
}

RecursiveDirectoryIterator::operator bool() const noexcept {
    return isValid();
}

RecursiveDirectoryIterator& RecursiveDirectoryIterator::operator++() {
    if(isValid()) {
        removeFromFilePath();
        walkUntilFileIsReached();
    }
    return *this;
}

bool RecursiveDirectoryIterator::isSpecialDirectory(const char* directoryName) noexcept {
    assert(directoryName && directoryName[0]);
    if(directoryName[0] == '.') {
        if(directoryName[1] == '\0') {
            return true;
        } else if((directoryName[1] == '.') && (directoryName[2] == '\0')) {
            return true;
        }
    }
    return false;
}

bool RecursiveDirectoryIterator::isDirectoryEmpty(DIR* directoryEntryStream) {
    assert(directoryEntryStream);
    struct dirent* directoryEntry = readdir(directoryEntryStream);
    while(directoryEntry) {
        if(!isSpecialDirectory(directoryEntry->d_name)) {
            return false;
        }
        directoryEntry = readdir(directoryEntryStream);
    }
    //check errno
    return true;
}

bool RecursiveDirectoryIterator::isDirectoryDelimiter(const char* directoryEntryName) noexcept {
    assert(directoryEntryName && directoryEntryName[0]);
    return (directoryEntryName[0] == '/') && (directoryEntryName[1] == '/') && (directoryEntryName[2] == '\0'); 
}

bool RecursiveDirectoryIterator::isValid() const noexcept {
    return !directoryEntryStack.isEmpty();
}

void RecursiveDirectoryIterator::removeFromFilePath() noexcept {
    while(filePath[filePath.size() - 1] != '/') {
        filePath.pop();
    }
}

void RecursiveDirectoryIterator::walkUntilFileIsReached() {
    while(!directoryEntryStack.isEmpty()) {
        const char* topEntryName = directoryEntryStack.top();
        if(isDirectoryDelimiter(topEntryName)) {
            filePath.pop();
            removeFromFilePath();
            directoryEntryStack.pop();
            continue;
        }
        if(!filePath.isEmpty()) {
            const std::size_t size = filePath.size();
            if(size > 1) {
                filePath.pop();
                if(filePath[size - 2] != '/') {
                    filePath.push('/');
                }
            }
        }
        for(std::size_t index = 0; topEntryName[index] != '\0'; ++index) {
            filePath.push(topEntryName[index]);
        }
        filePath.push('\0');
        directoryEntryStack.pop();
        errno = 0;
        DIR* directoryEntryStream = opendir(filePath.data());
        if(directoryEntryStream) {
            directoryEntryStack.push("//");
            if(!isDirectoryEmpty(directoryEntryStream)) {
                rewinddir(directoryEntryStream);
                struct dirent* directoryEntry = readdir(directoryEntryStream);
                while(directoryEntry) {
                    if(!isSpecialDirectory(directoryEntry->d_name)) {
                        directoryEntryStack.push(directoryEntry->d_name);
                    }
                    directoryEntry = readdir(directoryEntryStream);
                }
            }
            //errno = 0;
            closedir(directoryEntryStream);
        } else {
            if((errno == ENOENT) || (errno == EACCES) || (errno == ELOOP)) {
                removeFromFilePath();
                continue;
            }
            if(errno == ENOTDIR) {
                return;
            }
            assert(false);
        }
    }
    release();
}

void RecursiveDirectoryIterator::release() noexcept {
    filePath.safeRelease();
    allocator.releaseAllMemory();
}
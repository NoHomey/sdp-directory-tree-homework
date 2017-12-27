#include "DirectoryTree.h"
#include <cstring>
#include <cassert>
#include <iostream>
#include "../FixedCapacityCircularArrayQueue/FixedCapacityCircularArrayQueue.thd"

DirectoryTree::DirectoryTree(const char* rootDirectory)
: rootDirectoryNameLength{std::strlen(rootDirectory)},
root{Directory::newDirectory(rootDirectory, rootDirectoryNameLength)},
maxPathLength{0}, maxPathDepth{0} { }

void DirectoryTree::insert(const char* path) {
    updatePathCounters(path);
    Pair<DirectoryTree::Directory*, const char*> directoryPath = findDirectoryPath(path);
    Directory* lastFoundDirectory = directoryPath.first;
    Directory* currentDirectory = lastFoundDirectory;
    Directory* directoryToInsert = nullptr;
    const char* currentPath = directoryPath.second;
    while(true) {
        const char* positionOfDirectorySeparator = std::strchr(currentPath, '/');
        if(positionOfDirectorySeparator) {
            const std::size_t directoryNameEnd = positionOfDirectorySeparator - currentPath;
            if(directoryToInsert) {
                currentDirectory = currentDirectory->child = Directory::newDirectory(currentPath, directoryNameEnd);
            } else {
                directoryToInsert = Directory::newDirectory(currentPath, directoryNameEnd);
                currentDirectory = directoryToInsert;
            }
            currentPath += (directoryNameEnd + 1);
        } else {
            break;
        }
    }
    if(directoryToInsert) {
        if(lastFoundDirectory->child) {
            if(std::strcmp(directoryToInsert->name(), lastFoundDirectory->child->name()) < 0) {
                directoryToInsert->next = lastFoundDirectory->child;
                lastFoundDirectory->child = directoryToInsert;
            } else {
                Directory* prev = lastFoundDirectory->child;
                Directory* iter = lastFoundDirectory->child->next;
                bool inserted = false;
                for(; iter; iter = iter->next) {
                    if(std::strcmp(directoryToInsert->name(), iter->name()) < 0) {
                        directoryToInsert->next = iter;
                        prev->next = directoryToInsert;
                        inserted = true;
                        break;
                    }
                    prev = iter;
                }
                if(!inserted) {
                    prev->next = directoryToInsert;
                }
            }
        } else {
            lastFoundDirectory->child = directoryToInsert;
        }
    }
    filesMapper.addFileToDirectory(currentDirectory, currentPath);
}

std::size_t DirectoryTree::findPathDepth(const char* path) noexcept {
    std::size_t depth = 0;
    for(std::size_t index = 0; path[index] != '\0'; ++index) {
        if(path[index] == '/') {
            ++depth;
        }
    }
    return depth;
}

bool DirectoryTree::areDirectoryNamesEqual(const char* searchedName, const std::size_t searchedLength, const char* directoryName) noexcept {
    std::size_t index = 0;
    for(; index < searchedLength; ++index) {
        if(searchedName[index] != directoryName[index]) {
            return false;
        }
    }
    return directoryName[index] == '\0';
}

void DirectoryTree::updateMaxPathLength(const char* path) noexcept {
    const std::size_t pathLength = rootDirectoryNameLength + 1 + std::strlen(path);
    if(pathLength > maxPathLength) {
        maxPathLength = pathLength;
    }
}

void DirectoryTree::updateMaxPathDepth(const char* path) noexcept {
    const std::size_t pathDepth = 1 + findPathDepth(path);
    if(pathDepth > maxPathDepth) {
        maxPathDepth = pathDepth;
    }
}

void DirectoryTree::updatePathCounters(const char* path) noexcept {
    updateMaxPathLength(path);
    updateMaxPathDepth(path);
}

Pair<DirectoryTree::Directory*, const char*> DirectoryTree::findDirectoryPath(const char* path) const noexcept {
    Directory* directoryPath = root;
    const char* currentPath = path;
    bool foundChild = true;
    while(foundChild) {
        foundChild = false;
        const char* positionOfDirectorySeparator = std::strchr(currentPath, '/');
        if(positionOfDirectorySeparator) {
            const std::size_t directoryNameEnd = positionOfDirectorySeparator - currentPath;
            for(Directory* entry = directoryPath->child; entry; entry = entry->next) {
                if(areDirectoryNamesEqual(currentPath, directoryNameEnd, entry->name())) {
                    currentPath += (directoryNameEnd + 1);
                    directoryPath = entry;
                    foundChild = true;
                    break;
                }
            }
        }
    }
    return {directoryPath, currentPath};
}

DirectoryTree::AscOrderConstIterator DirectoryTree::ascOrderFirst() const noexcept {
    return AscOrderConstIterator::constructIterator(this);
}

std::size_t DirectoryTree::findTreeDepth() const {
    const std::size_t countOfDirectoriesWithFiles = filesMapper.countOfDirectoriesWithFiles();
    const Directory* data[countOfDirectoriesWithFiles];
    FixedCapacityCircularArrayQueue<const Directory*> bfsQueue{countOfDirectoriesWithFiles, data};
    std::size_t depth = 1;
    bfsQueue.enqueue(root);
    while(!bfsQueue.isEmpty()) {
        const std::size_t currentWaveCount = bfsQueue.size();
        for(std::size_t counter = 0; counter < currentWaveCount; ++counter) {
            const Directory* dir = bfsQueue.dequeue();
            for(const Directory* child = dir->child; child; child = child->next) {
                if(child->child) {
                    bfsQueue.enqueue(child);
                }
            }
        }
        ++depth;
    }
    return depth;
}
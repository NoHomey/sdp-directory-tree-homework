#include "DirectoryTree.h"
#include <cstring>
#include <cassert>
#include <iostream>

DirectoryTree::DirectoryTree(const char* rootDirectory)
: root{Directory::newDirectory(rootDirectory, std::strlen(rootDirectory))} { }

void DirectoryTree::insert(const char* path) {
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

bool DirectoryTree::areDirectoryNamesEqual(const char* searchedName, const std::size_t searchedLength, const char* directoryName) noexcept {
    std::size_t index = 0;
    for(; index < searchedLength; ++index) {
        if(searchedName[index] != directoryName[index]) {
            return false;
        }
    }
    return directoryName[index] == '\0';
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

void DirectoryTree::print() const noexcept {
    char buffer[30];
    print(root, buffer, 0);
}

void DirectoryTree::print(const Directory* dir, char* buffer, const std::size_t pos) const noexcept {
    if(!dir) {
        return;
    }
    const std::size_t nameLength = std::strlen(dir->name());
    std::memcpy(buffer + pos, dir->name(), nameLength);
    buffer[pos + nameLength] = '/';
    const FilesMapper::Files* filesForDir = filesMapper.getFilesForDirectory(dir);
    if(filesForDir) {
        for(const FilesMapper::Files::File* iter = filesForDir->iter(); iter; iter = iter->next) {
            std::memcpy(buffer + pos + nameLength + 1, iter->name(), std::strlen(iter->name()) + 1);
            std::cout << buffer << std::endl;
        }
    }
    print(dir->child, buffer, pos + nameLength + 1);
    print(dir->next, buffer, pos);
}
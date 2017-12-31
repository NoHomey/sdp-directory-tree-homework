#include "DirectoryTree.h"
#include <cstring>
#include <cassert>
#include "../FixedCapacityStack/FixedCapacityStack.thd"

ChunkAllocator DirectoryTree::allocator;

DirectoryTree::DirectoryTree(const char* rootDirectory)
: rootDirectoryNameLength{std::strlen(rootDirectory)},
root{Directory::newDirectory(rootDirectory, rootDirectoryNameLength)},
maxPathLength{rootDirectoryNameLength + 1}, maxPathDepth{1} { }

void DirectoryTree::insert(const char* path) {
    updatePathCounters(path);
    Pair<DirectoryTree::Directory*, const char*> directoryPath = findDirectoryPath(path);
    Directory* lastFoundDirectory = directoryPath.first;
    Directory* currentDirectory = lastFoundDirectory;
    Directory* directoryToInsert = nullptr;
    const char* currentPath = directoryPath.second;
    const char* positionOfDirectorySeparator = std::strchr(currentPath, '/');
    while(positionOfDirectorySeparator) {
        const std::size_t directoryNameEnd = positionOfDirectorySeparator - currentPath;
        if(directoryToInsert) {
            currentDirectory = currentDirectory->child = Directory::newDirectory(currentPath, directoryNameEnd);
        } else {
            directoryToInsert = Directory::newDirectory(currentPath, directoryNameEnd);
            currentDirectory = directoryToInsert;
        }
        currentPath += (directoryNameEnd + 1);
        positionOfDirectorySeparator = std::strchr(currentPath, '/');
    }
    if(directoryToInsert) {
        if(lastFoundDirectory->child) {
            directoryToInsert->next = lastFoundDirectory->child;
            lastFoundDirectory->child = directoryToInsert;
        } else {
            lastFoundDirectory->child = directoryToInsert;
        }
    }
    filesMapper.addFileToDirectory(currentDirectory, currentPath);
}

DirectoryTree::AscOrderConstIterator DirectoryTree::ascOrderFirst() const {
    return filesMapper.countOfDirectoriesWithFiles() ? AscOrderConstIterator{this} : AscOrderConstIterator{};
}

void DirectoryTree::sort() {
    if(maxPathDepth > 1) {
        sortDirectories();
    }
    filesMapper.sortFileNames();
}

void DirectoryTree::markAllFilesAsDeleted() noexcept {
    filesMapper.markAllFilesAsDeleted();
}

void DirectoryTree::eraseAllDeletedFiles() {
    filesMapper.eraseAllDeletedFiles();
    if(maxPathDepth == 1) {
        return;
    }
    FixedCapacityStack<Directory*, ChunkAllocator> path{maxPathDepth, &allocator};
    maxPathDepth = 1;
    maxPathLength = rootDirectoryNameLength;
    std::size_t currentPathDepth = 1;
    std::size_t currentPathLength = rootDirectoryNameLength;
    path.push(root);
    bool incrementCounters = false;
    while(!path.isEmpty()) {
        Directory* directory = path.top();
        std::size_t currentDirectoryLength = 0;
        if(incrementCounters) {
            currentDirectoryLength = std::strlen(directory->name());
            currentPathLength += currentDirectoryLength;
            ++currentPathDepth;
        }
        incrementCounters = true;
        if(directory->child) {
            path.push(directory->child);
        } else {
            Directory* currentDirectory = path.pop();
            Directory* nextDirectory = currentDirectory->next;
            const FilesMapper::Files* filesForCurrentDirectory = filesMapper.getFilesForDirectory(currentDirectory);
            if(filesForCurrentDirectory) {
                std::size_t maxFileNameLength = 0;
                for(const FilesMapper::Files::File* file = filesForCurrentDirectory->first; file; file = file->next) {
                    const std::size_t fileNameLength = std::strlen(file->name());
                    if(fileNameLength > maxFileNameLength) {
                        maxFileNameLength = fileNameLength;
                    }
                }
                const std::size_t totalCurrentPathLenght = currentPathLength + maxFileNameLength + currentPathDepth;
                if(totalCurrentPathLenght > maxPathLength) {
                    maxPathLength = totalCurrentPathLenght;
                }
                if(currentPathDepth > maxPathDepth) {
                    maxPathDepth = currentPathDepth;
                }
            } else {
                Directory* parentDirectory = path.top();
                if(parentDirectory->child == currentDirectory) {
                    parentDirectory->child = nextDirectory;
                } else {
                    Directory* prev = parentDirectory->child;
                    while(prev->next != currentDirectory) {
                        prev = prev->next;
                    }
                    prev->next = nextDirectory;
                }
                assert(currentDirectory != root);
                Directory::deleteDirectory(currentDirectory);
            }
            --currentPathDepth;
            currentPathLength -= currentDirectoryLength ? currentDirectoryLength : std::strlen(currentDirectory->name());
            if(nextDirectory) {
                path.push(nextDirectory);
            } else {
                Directory* parentDirectory = path.top();
                incrementCounters = parentDirectory->child;
                if(parentDirectory->child) {
                    while(!path.isEmpty()) {
                        parentDirectory = path.pop();
                        --currentPathDepth;
                        currentPathLength -= std::strlen(parentDirectory->name());
                        if(parentDirectory->next) {
                            path.push(parentDirectory->next);
                            break;
                        }
                    }
                }
            }
        }
    }
    path.release();
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

template<typename ListNode>
bool DirectoryTree::isLessThan(ListNode* left, ListNode* right) noexcept {
    assert(left && right);
    return std::strcmp(left->name(), right->name()) < 0;
}

template<typename ListNode>
std::size_t DirectoryTree::sortPairsAndFindListLength(ListNode*& first) noexcept {
    assert(first);
    if(!first->next) {
        return 1;
    }
    if(isLessThan(first->next, first)) {
        ListNode* second = first->next;
        first->next = second->next;
        second->next = first;
        first = second;
    }
    ListNode* prev = first->next;
    ListNode* current = prev->next;
    std::size_t count = 2;
    while(current && current->next) {
        if(isLessThan(current->next, current)) {
            prev->next = current->next;
            current->next = current->next->next;
            prev->next->next = current;
        }
        prev = prev->next->next;
        current = prev->next;
        count += 2;
    }
    return count + (!!current);
}

template<typename ListNode>
ListNode* DirectoryTree::cut(ListNode* first, const std::size_t n) noexcept {
    assert(first && n);
    ListNode* next = nullptr;
    std::size_t counter = n;
    while(counter && first) {
        next = first;
        first = first->next;
        --counter;
    }
    if(next) {
        next->next = nullptr;
    }
    return first;
}

template<typename ListNode>
ListNode* DirectoryTree::advanceAndReturnPrev(ListNode*& listNode) noexcept {
    assert(listNode);
    ListNode* prev = listNode;
    listNode = listNode->next;
    return prev;
}

template<typename ListNode>
Pair<ListNode*, ListNode*> DirectoryTree::merge(ListNode* left, ListNode* right) noexcept {
    assert(left && right);
    ListNode* first = advanceAndReturnPrev(isLessThan(right, left) ? right : left);
    ListNode* last = first;
    while(left && right) {
        last = last->next = advanceAndReturnPrev(isLessThan(right, left) ? right : left);
    }
    while(left) {
        last = last->next = advanceAndReturnPrev(left);
    }
    while(right) {
        last = last->next = advanceAndReturnPrev(right);
    }
    return {first, last};
}

template<typename ListNode>
void DirectoryTree::mergeSort(ListNode*& first) noexcept {
    const std::size_t listLength = sortPairsAndFindListLength(first);
    for(std::size_t step = 2; step < listLength; step <<= 1) {
        ListNode* next = first;
        ListNode **last = &first;
        while(next) {
            ListNode* leftSubList = next;
            ListNode* rightSubList = cut(leftSubList, step);
            if(!rightSubList) {
                break;
            }
            next = cut(rightSubList, step);
            Pair<ListNode*, ListNode*> merged = merge(leftSubList, rightSubList);
            *last = merged.first;
            merged.second->next = next;
            last = &(merged.second->next);
        }
    }
}

void DirectoryTree::mergeSortFiles(FilesMapper::Files::File*& first) noexcept {
    mergeSort<FilesMapper::Files::File>(first);
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

void DirectoryTree::sortDirectories() {
    const std::size_t pathDepth = maxPathDepth - 1;
    FixedCapacityStack<Directory*, ChunkAllocator> path{pathDepth, &allocator};
    path.push(root);
    while(!path.isEmpty()) {
        Directory* dir = path.pop();
        if(dir->next && dir->next->child) {
            path.push(dir->next);
        }
        if(dir->child->child) {
            path.push(dir->child);
        }
        mergeSort<Directory>(dir->child);
    }
    path.release();
}
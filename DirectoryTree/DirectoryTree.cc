#include "DirectoryTree.h"
#include <cstring>
#include <cassert>
#include "../FixedCapacityStack/FixedCapacityStack.thd"

ChunkAllocator DirectoryTree::allocator;

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

DirectoryTree::AscOrderConstIterator DirectoryTree::ascOrderFirst() const noexcept {
    return filesMapper.countOfDirectoriesWithFiles() ? AscOrderConstIterator{this} : AscOrderConstIterator{};
}

void DirectoryTree::sort() noexcept {
    if(maxPathDepth > 1) {
        sortDirectories();
    }
    filesMapper.sortFileNames();
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

void DirectoryTree::sortDirectories() noexcept {
    const std::size_t pathLength = maxPathDepth - 1;
    const std::size_t neededBytesOfMemory = pathLength * sizeof(Directory*);
    Directory** neededMemory = reinterpret_cast<Directory**>(allocator.allocate(neededBytesOfMemory));
    FixedCapacityStack<Directory*> path{pathLength, neededMemory};
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
    allocator.release(neededMemory, neededBytesOfMemory);
}
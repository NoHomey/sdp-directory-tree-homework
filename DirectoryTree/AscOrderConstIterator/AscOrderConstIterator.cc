#include "AscOrderConstIterator.h"
#include <new>

DirectoryTree::AscOrderConstIterator::~AscOrderConstIterator() noexcept {
    delete[] memoryBlock;
    if(memoryForPath) {
        delete[] memoryForPath;
    }
}

DirectoryTree::AscOrderConstIterator::operator bool() const noexcept {
    return isValid();
}

bool DirectoryTree::AscOrderConstIterator::operator==(const AscOrderConstIterator& other) const noexcept {
    return (directoryTree == other.directoryTree) && (file == other.file);
}

bool DirectoryTree::AscOrderConstIterator::operator!=(const AscOrderConstIterator& other) const noexcept {
    return !operator==(other);
}

bool DirectoryTree::AscOrderConstIterator::operator!() const noexcept {
    return !isValid();
}

const char* DirectoryTree::AscOrderConstIterator::operator*() const noexcept {
    assert(isValid());
    return filePath.data();
}

DirectoryTree::AscOrderConstIterator& DirectoryTree::AscOrderConstIterator::operator++() noexcept {
    if(isValid()) {
        removeFromPath(file->name());
        file = file->next;
        if(file) {
            setFileName();
        } else {
            const Directory* dir = path.top();
            if(dir->child) {
                moveDownInDirectory(dir->child);
            } else {
                moveUp();
            }
        }
    }
    return *this;
}

DirectoryTree::AscOrderConstIterator DirectoryTree::AscOrderConstIterator::constructIterator(const DirectoryTree* directoryTree) {
    try {
        return {directoryTree, new char[directoryTree->maxPathLength + 1 + sizeof(DirectoryTree::Directory*) * directoryTree->maxPathDepth]};
    } catch(const std::bad_alloc& error) {
        return {directoryTree, new char[directoryTree->maxPathLength + 1], new const DirectoryTree::Directory*[directoryTree->maxPathDepth]};
    }
}

DirectoryTree::AscOrderConstIterator::AscOrderConstIterator(const DirectoryTree* directoryTree, char* memoryBlock) noexcept
: directoryTree{directoryTree},
memoryBlock{memoryBlock},
memoryForPath{nullptr},
filePath{directoryTree->maxPathLength + 1, memoryBlock},
path{directoryTree->maxPathDepth, reinterpret_cast<const DirectoryTree::Directory**>(memoryBlock + directoryTree->maxPathLength + 1)},
file{nullptr} {
    init();
}

DirectoryTree::AscOrderConstIterator::AscOrderConstIterator(const DirectoryTree* directoryTree, char* memoryForFilePath, const DirectoryTree::Directory** memoryForPath) noexcept
: directoryTree{directoryTree},
memoryBlock{memoryForFilePath},
memoryForPath{memoryForPath},
filePath{directoryTree->maxPathLength + 1, memoryForFilePath},
path{directoryTree->maxPathDepth, memoryForPath},
file{nullptr} {
    init();
}

void DirectoryTree::AscOrderConstIterator::init() noexcept {
    moveDownInDirectory(directoryTree->root);
}

void DirectoryTree::AscOrderConstIterator::setFileName() noexcept {
    filePath.push(file->name());
    filePath.push('\0');
}

void DirectoryTree::AscOrderConstIterator::addDirToPath(const Directory* dir) noexcept {
    path.push(dir);
    filePath.push(dir->name());
    filePath.push('/');
}

void DirectoryTree::AscOrderConstIterator::removeFromPath(const char* toRemove) noexcept {
    filePath.pop(std::strlen(toRemove) + 1);
}

void DirectoryTree::AscOrderConstIterator::moveDownInDirectory(const DirectoryTree::Directory* directory) noexcept {
    const Directory* dir = directory;
    const FilesMapper::Files* filesForDir;
    do {
        addDirToPath(dir);
        filesForDir = directoryTree->filesMapper.getFilesForDirectory(dir);
        dir = dir->child;
    } while(!filesForDir);
    file = filesForDir->firstFile();
    setFileName();
}

void DirectoryTree::AscOrderConstIterator::moveUp() noexcept {
    while(!path.isEmpty()) {
        const Directory* dir = path.pop();
        removeFromPath(dir->name());
        if(dir->next) {
            moveDownInDirectory(dir->next);
            return;
        }
    }
}

bool DirectoryTree::AscOrderConstIterator::isValid() const noexcept {
    return file;
}
#include "AscOrderConstIterator.h"
#include <new>

DirectoryTree::AscOrderConstIterator::~AscOrderConstIterator() noexcept {
    release();
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
        removeFromPath(file->nameLength);
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
    } else {
        release();
    }
    return *this;
}

DirectoryTree::AscOrderConstIterator::AscOrderConstIterator(const DirectoryTree* directoryTree) noexcept
: directoryTree{directoryTree},
memoryForFilePath{reinterpret_cast<char*>(
    DirectoryTree::allocator.allocate(directoryTree->maxPathLength + 1)
)},
memoryForPath{reinterpret_cast<const DirectoryTree::Directory**>(
    DirectoryTree::allocator.allocate(directoryTree->maxPathDepth * sizeof(const DirectoryTree::Directory*))
)},
filePath{directoryTree->maxPathLength + 1, memoryForFilePath},
path{directoryTree->maxPathDepth, memoryForPath},
file{nullptr} {
    init();
}

void DirectoryTree::AscOrderConstIterator::init() noexcept {
    moveDownInDirectory(directoryTree->root);
}

void DirectoryTree::AscOrderConstIterator::setFileName() noexcept {
    filePath.push(file->name(), file->nameLength);
    filePath.push('\0');
}

void DirectoryTree::AscOrderConstIterator::addDirToPath(const Directory* dir) noexcept {
    path.push(dir);
    filePath.push(dir->name(), dir->nameLength);
    filePath.push('/');
}

void DirectoryTree::AscOrderConstIterator::removeFromPath(const std::size_t toRemoveLength) noexcept {
    filePath.pop(toRemoveLength + 1);
}

void DirectoryTree::AscOrderConstIterator::moveDownInDirectory(const DirectoryTree::Directory* directory) noexcept {
    const Directory* dir = directory;
    const FilesMapper::Files* filesForDir;
    do {
        addDirToPath(dir);
        filesForDir = directoryTree->filesMapper.getFilesForDirectory(dir);
        dir = dir->child;
    } while(!filesForDir);
    file = filesForDir->first;
    setFileName();
}

void DirectoryTree::AscOrderConstIterator::moveUp() noexcept {
    while(!path.isEmpty()) {
        const Directory* dir = path.pop();
        removeFromPath(dir->nameLength);
        if(dir->next) {
            moveDownInDirectory(dir->next);
            return;
        }
    }
}

bool DirectoryTree::AscOrderConstIterator::isValid() const noexcept {
    return file;
}

void DirectoryTree::AscOrderConstIterator::release() noexcept {
    if(memoryForPath) {
        DirectoryTree::allocator.release(memoryForPath, directoryTree->maxPathDepth * sizeof(const DirectoryTree::Directory*));
        DirectoryTree::allocator.release(memoryForFilePath, directoryTree->maxPathLength + 1);
        memoryForPath = nullptr;
        memoryForFilePath = nullptr;
    }
}
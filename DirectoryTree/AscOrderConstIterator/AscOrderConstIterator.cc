#include "AscOrderConstIterator.h"
#include <new>
#include <utility>

DirectoryTree::AscOrderConstIterator::AscOrderConstIterator() noexcept
: directoryTree{nullptr},
filePath{},
path{},
file{nullptr} { }

DirectoryTree::AscOrderConstIterator::AscOrderConstIterator(AscOrderConstIterator&& other) noexcept
: directoryTree{other.directoryTree},
filePath{std::move(other.filePath)},
path{std::move(other.path)},
file{other.file} {
    other.null();
}

DirectoryTree::AscOrderConstIterator::~AscOrderConstIterator() noexcept {
    safeRelease();
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

Pair<const char*, char> DirectoryTree::AscOrderConstIterator::operator*() const noexcept {
    assert(isValid());
    return {filePath.data(), getFileStatusMark(file)};
}

char DirectoryTree::AscOrderConstIterator::getFileStatusMark(const DirectoryTree::FilesMapper::Files::File* file) noexcept {
    assert(file);
    return file->isFound() ? '\0' : (file->isNew() ? '+' : '-');
}

DirectoryTree::AscOrderConstIterator& DirectoryTree::AscOrderConstIterator::operator++() noexcept {
    if(isValid()) {
        removeFromFilePath();
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
        safeRelease();
    }
    return *this;
}

DirectoryTree::AscOrderConstIterator::AscOrderConstIterator(const DirectoryTree* directoryTree) noexcept
: directoryTree{directoryTree},
filePath{directoryTree->maxPathLength + 1, &DirectoryTree::allocator},
path{directoryTree->maxPathDepth, &DirectoryTree::allocator},
file{nullptr} {
    init();
}

void DirectoryTree::AscOrderConstIterator::init() noexcept {
    moveDownInDirectory(directoryTree->root);
}

void DirectoryTree::AscOrderConstIterator::addToFilePath(const char* str) noexcept {
    for(std::size_t index = 0; str[index]; ++index) {
        filePath.push(str[index]);
    }
}

void DirectoryTree::AscOrderConstIterator::setFileName() noexcept {
    addToFilePath(file->name());
    filePath.push('\0');
}

void DirectoryTree::AscOrderConstIterator::addDirToPath(const Directory* dir) noexcept {
    path.push(dir);
    addToFilePath(dir->name());
    filePath.push('/');
}

void DirectoryTree::AscOrderConstIterator::removeFromFilePath() noexcept {
    if(filePath.top() == '/') {
        filePath.pop();
    }
    while(filePath.top() != '/') {
        filePath.pop();
    }
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
        removeFromFilePath();
        if(dir->next) {
            moveDownInDirectory(dir->next);
            return;
        }
    }
    safeRelease();
}

bool DirectoryTree::AscOrderConstIterator::isValid() const noexcept {
    return file;
}

void DirectoryTree::AscOrderConstIterator::null() noexcept {
    directoryTree = nullptr;
    file = nullptr;
}

void DirectoryTree::AscOrderConstIterator::release() {
    if(directoryTree) {
        filePath.release();
        path.release();
        null();
    }
}

bool DirectoryTree::AscOrderConstIterator::safeRelease() noexcept {
    if(directoryTree) {
        if(filePath.safeRelease() && path.safeRelease()) {
            null();
            return true;
        }
        return false;
    }
    return true;
}
#include "AscOrderConstIterator.h"
#include <new>
#include <utility>

DirectoryTree::AscOrderConstIterator::AscOrderConstIterator() noexcept
: directoryTree{nullptr},
memoryForFilePath{nullptr},
memoryForPath{nullptr},
filePath{},
path{},
file{nullptr} {

}

DirectoryTree::AscOrderConstIterator::AscOrderConstIterator(AscOrderConstIterator&& other) noexcept
: directoryTree{other.directoryTree},
memoryForFilePath{other.memoryForFilePath},
memoryForPath{other.memoryForPath},
filePath{std::move(other.filePath)},
path{std::move(other.path)},
file{other.file} {
    other.null();
}

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
    release();
}

bool DirectoryTree::AscOrderConstIterator::isValid() const noexcept {
    return file;
}

void DirectoryTree::AscOrderConstIterator::null() noexcept {
    directoryTree = nullptr;
    memoryForFilePath = nullptr;
    memoryForPath = nullptr;
    file = nullptr;
}

void DirectoryTree::AscOrderConstIterator::release() noexcept {
    if(memoryForPath) {
        DirectoryTree::allocator.release(memoryForPath, directoryTree->maxPathDepth * sizeof(const DirectoryTree::Directory*));
        DirectoryTree::allocator.release(memoryForFilePath, directoryTree->maxPathLength + 1);
        null();
    }
}
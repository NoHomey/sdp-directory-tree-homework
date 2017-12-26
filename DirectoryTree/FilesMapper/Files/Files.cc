#include "../../DirectoryTree.h"
#include <cstring>

DirectoryTree::FilesMapper::Files::Files() noexcept
: Files{nullptr, nullptr} {}

DirectoryTree::FilesMapper::Files::Files(Files&& other) noexcept
: Files{other.first, other.last} { }

const DirectoryTree::FilesMapper::Files::File* DirectoryTree::FilesMapper::Files::firstFile() const noexcept {
    return first;
}

DirectoryTree::FilesMapper::Files::File* DirectoryTree::FilesMapper::Files::firstFile() noexcept {
    return first;
}

void DirectoryTree::FilesMapper::Files::insert(const char* fileName) {
    if(first) {
        if(std::strcmp(fileName, first->name()) < 0) {
            first = File::newFile(fileName, first);
        } else if(std::strcmp(fileName, last->name()) > 0) {
            last = last->next = File::newFile(fileName);
        } else {
            File* prev = first;
            for(File* iter = first->next; iter; iter = iter->next) {
                if(std::strcmp(fileName, iter->name()) < 0) {
                    prev->next = File::newFile(fileName, iter);
                    return;
                }
                prev = iter;
            }
        }
    } else {
        first = last = File::newFile(fileName);
    }
}

DirectoryTree::FilesMapper::Files::Files(File* first, File* last) noexcept
: first{first}, last{last} { }

void DirectoryTree::FilesMapper::Files::null() noexcept {
    first = last = nullptr;
}
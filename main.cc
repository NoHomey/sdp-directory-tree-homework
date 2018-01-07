#include "RecursiveDirectoryIterator/RecursiveDirectoryIterator.h"
#include "DirectoryTree/DirectoryTree.h"
#include "FileMatcher/FileMatcher.thd"
#include "OperatorAllocator/OperatorAllocator.h"
#include <iostream>
#include <utility>
#include <cstring>

const std::size_t initalCommandCapacity = 300;

template<typename DirectoryTreeConstIterator>
void print(DirectoryTreeConstIterator&& iterator) {
    for(DirectoryTreeConstIterator it = std::move(iterator); it; ++it) {
        Pair<const char*, char> fileInfo = *it;
        if(fileInfo.second) {
            std::cout << fileInfo.second << ' ';
        }
        std::cout << fileInfo.first << std::endl;
    }
}

bool matchesAnything(const char* pattern) noexcept {
    assert(pattern && pattern[0]);
    for(std::size_t index = 0; pattern[index]; ++index) {
        if(pattern[index] != '*') {
            return false;
        }
    }
    return true;
}

void walk(const char* dir, DirectoryTree& tree) {
    RecursiveDirectoryIterator dirIterator{dir};
    for(; dirIterator; ++dirIterator) {
        tree.insert(*dirIterator);
    }
}

bool isQuitCommand(const char* command) noexcept {
    assert(command && command[0]);
    return !std::strcmp(command, "quit");
}

bool isRefreshCommand(const char* command) noexcept {
    assert(command && command[0]);
    return !std::strcmp(command, "refresh");
}

bool isPrintCommand(const char* command) noexcept {
    assert(command && command[0]);
    return !std::strcmp(command, "print");
}

Pair<bool, const char*> isPrintWithPatternCommand(const char* command) noexcept {
    assert(command && command[0]);
    const char* print = "print";
    for(std::size_t index = 0; print[index]; ++index) {
        if(command[index] != print[index]) {
            return {false, static_cast<const char*>(nullptr)};
        }
    }
    if((command[5] != ' ') && (command[5] != '\t')) {
        return {false, static_cast<const char*>(nullptr)};
    }
    std::size_t index = 6;
    while((command[index] == ' ') || (command[index] == '\t')) {
        ++index;
    }
    return {command[index] != '\0', command + index};
}

int main(const int argc, const char** argv) {
    assert(argc > 1);
    const char* dir = argv[1];
    DirectoryTree tree{dir};
    OperatorAllocator defaultAllocator;
    DynamicArray<char, OperatorAllocator> command{initalCommandCapacity, &defaultAllocator};
    walk(dir, tree);

    while(true) {
        const char nextChar = std::cin.get();
        if(nextChar == '\n') {
            command.push('\0');
            if(isQuitCommand(command.data())) {
                break;
            } else if(isRefreshCommand(command.data())) {
                tree.markAllFilesAsDeleted();
                walk(dir, tree);
            } else if(isPrintCommand(command.data())) {
                tree.sort();
                print(tree.ascOrderFirst());
                tree.eraseAllDeletedFiles();
            } else {
                Pair<bool, const char*> res = isPrintWithPatternCommand(command.data());
                if(!res.first) {
                    tree.sort();
                    if(res.second) {
                        if(argc == 3) {
                            print(tree.ascOrderFirstMatchingFileName(argv[2]));
                        } else {
                            print(tree.ascOrderFirst());
                        }
                        tree.eraseAllDeletedFiles();
                    } else {
                        std::cout << "Error, unknown command: " << command.data() << std::endl;
                    }
                } else {
                    tree.sort();
                    print(tree.ascOrderFirstMatchingFileName(res.second));
                    tree.eraseAllDeletedFiles();
                }
            }
            command.empty();
        } else {
            command.push(nextChar);
        }
    }

    return 0;
}
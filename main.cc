#include "RecursiveDirectoryIterator/RecursiveDirectoryIterator.h"
#include "DirectoryTree/DirectoryTree.h"
#include <iostream>

int main() {
    {
        const char* dir = "./__test__";
        RecursiveDirectoryIterator deepTreeDir{dir};
        DirectoryTree tree{dir};
        for(; deepTreeDir; ++deepTreeDir) {
            tree.insert(*deepTreeDir);
        }
        tree.sort();
        for(DirectoryTree::AscOrderConstIterator it = tree.ascOrderFirst(); it; ++it) {
            Pair<const char*, char> fileInfo = *it;
            if(fileInfo.second) {
                std::cout << fileInfo.second << ' ';
            }
            std::cout << fileInfo.first << std::endl;
        }
        std::cout << "--------------------------------------------" << std::endl;
        const char* pattern = "file.txt";
        for(DirectoryTree::AscOrderConstIteratorMatchingFileName it = tree.ascOrderFirstMatchingFileName(pattern); it; ++it) {
            Pair<const char*, char> fileInfo = *it;
            if(fileInfo.second) {
                std::cout << fileInfo.second << ' ';
            }
            std::cout << fileInfo.first << std::endl;
        }
    }

    /*DirectoryTree tree{"./root"};

    tree.sort();

    tree.markAllFilesAsDeleted();

    tree.sort();

    std::cout << "-------------------------------------" << std::endl;

    for(DirectoryTree::AscOrderConstIterator it = tree.ascOrderFirst(); it; ++it) {
        Pair<const char*, char> fileInfo = *it;
        if(fileInfo.second) {
            std::cout << fileInfo.second << ' ';
        }
        std::cout << fileInfo.first << std::endl;
    }

    tree.eraseAllDeletedFiles();

    std::cout << "-------------------------------------" << std::endl;

    for(DirectoryTree::AscOrderConstIterator it = tree.ascOrderFirst(); it; ++it) {
        Pair<const char*, char> fileInfo = *it;
        if(fileInfo.second) {
            std::cout << fileInfo.second << ' ';
        }
        std::cout << fileInfo.first << std::endl;
    }*/

    return 0;
}
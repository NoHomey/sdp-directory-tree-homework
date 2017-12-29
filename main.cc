#include "RecursiveDirectoryIterator/RecursiveDirectoryIterator.h"
#include "DirectoryTree/DirectoryTree.h"
#include <iostream>

int main() {
    {
        RecursiveDirectoryIterator notExist{"./root"};
        for(; notExist; ++notExist) {
            std::cout << *notExist << std::endl;
        }
    }
    {
        RecursiveDirectoryIterator emptyDir{"./__test__/emptyDir"};
        for(; emptyDir; ++emptyDir) {
            std::cout << *emptyDir << std::endl;
        }
    }
    {
        RecursiveDirectoryIterator emptyDirTree{"./__test__/emptyDirTree"};
        for(; emptyDirTree; ++emptyDirTree) {
            std::cout << *emptyDirTree << std::endl;
        }
    }
    {
        RecursiveDirectoryIterator flatDir{"./__test__/flatDir"};
        for(; flatDir; ++flatDir) {
            std::cout << *flatDir << std::endl;
        }
    }
    {
        RecursiveDirectoryIterator deepTreeDir{"./__test__/deepTreeDir"};
        for(; deepTreeDir; ++deepTreeDir) {
            std::cout << *deepTreeDir << std::endl;
        }
    }
    {
        RecursiveDirectoryIterator it{"/home/ivo/sdp"};
        for(; it; ++it) {
            std::cout << *it << std::endl;
        }
    }
    {
        RecursiveDirectoryIterator it{"/home/ivo/fmi-bachelor-the-math-part"};
        for(; it; ++it) {
            std::cout << *it << std::endl;
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
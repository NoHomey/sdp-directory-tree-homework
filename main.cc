#include "DirectoryTree/DirectoryTree.h"
#include <iostream>

int main() {
    DirectoryTree tree{"./root"};
    tree.insert("f");
    tree.insert("f2");
    tree.insert("b/f");
    tree.insert("a/c/f");
    tree.insert("a/e/f");
    tree.insert("a/e/f/f");
    tree.insert("a/d/f");
    tree.insert("c/f/f");
    tree.insert("c/e/f/f");
    tree.insert("c/e/f/f/f");
    tree.insert("c/d/f/f");
    tree.insert("d/f/f");
    tree.insert("d/e/f/f");
    tree.insert("d/e/f/f/f");
    tree.insert("d/d/f/f");
    tree.insert("a/e/a/c/d/f");

    tree.sort();

    tree.markAllFilesAsDeleted();

    tree.insert("b/f");
    tree.insert("a/e/f");
    tree.insert("d/e/f/f");
    tree.insert("d/f/f");
    tree.insert("a/e/a/c/d/f");
    tree.insert("i/e/f/f");
    tree.insert("i/f/f");

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
    }

    return 0;
}
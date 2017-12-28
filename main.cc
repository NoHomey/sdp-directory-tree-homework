#include "DirectoryTree/DirectoryTree.h"
#include <iostream>

int main() {
    DirectoryTree tree{"./root"};
    tree.insert("f1");
    tree.insert("f2");
    tree.insert("file");
    tree.insert("file2");
    tree.insert("a/f");
    tree.insert("a/a/f");
    tree.insert("aa/f");
    tree.insert("a/aa/f");
    tree.insert("a/sort/a/f");
    tree.insert("a/sort/a/f1");
    tree.insert("a/sort/a/f3");
    tree.insert("a/sort/a/f2");
    tree.insert("a/sort/b/f");
    tree.insert("a/sort/d/f1");
    tree.insert("a/sort/c/f3");
    tree.insert("a/sort/f/f2");
    tree.insert("a/sort/b/f");
    tree.insert("a/sort/c/f1");
    tree.insert("a/sort/b/f3");
    tree.insert("a/sort/d/f2");

    tree.sort();

    for(DirectoryTree::AscOrderConstIterator it = tree.ascOrderFirst(); it; ++it) {
        Pair<const char*, char> fileInfo = *it;
        if(fileInfo.second) {
            std::cout << fileInfo.second << ' ';
        }
        std::cout << fileInfo.first << std::endl;
    }

    tree.markAllFilesAsDeleted();

    tree.insert("file");
    tree.insert("file2");
    tree.insert("a/f");
    tree.insert("a/a/f");
    tree.insert("aa/f");
    tree.insert("a/aa/f");
    tree.insert("a/sort/a/f");
    tree.insert("a/sort/a/f1");
    tree.insert("a/sort/a/f3");
    tree.insert("a/sort/a/f2");
    tree.insert("a/sort/c/f1");
    tree.insert("a/sort/b/f3");
    tree.insert("a/sort/d/f2");
    tree.insert("a/sort/d/f3");
    tree.insert("a/sort/e/f2");
    tree.insert("a/sort/c/f2");
    tree.insert("a/sort/b/f");
    tree.insert("a/sort/d/file");

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
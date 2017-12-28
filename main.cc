#include "DirectoryTree/DirectoryTree.h"
#include <iostream>

int main() {
    DirectoryTree tree{"./root"};
    tree.insert("file");
    tree.insert("file1");
    tree.insert("file2");
    tree.insert("f1/file");
    tree.insert("f2/file1");
    tree.insert("f1/file2");
    tree.insert("f2/file");
    tree.insert("f1/file1");
    tree.insert("f1/file3");
    tree.insert("aa/f1/a");
    tree.insert("aaa/f2/b");
    tree.insert("a/f4/g");
    tree.insert("b/f1/a");
    tree.insert("b/f1/b");
    tree.insert("b/f2/f3/c");
    tree.insert("b/f1/d");
    tree.insert("b/f3/x");
    tree.insert("b/f4/y");
    tree.insert("b/f6/z");
    tree.insert("b/f3/y");
    tree.insert("b/f4/z");
    tree.insert("b/f6/x");
    tree.insert("b/f3/z");
    tree.insert("b/f4/x");
    tree.insert("b/f6/y");
    tree.insert("b/f5/x");
    tree.insert("b/f5/y");
    tree.insert("b/f5/z");
    tree.insert("b/b/b");
    tree.insert("b/sort/aaabb");
    tree.insert("b/sort/aa");
    tree.insert("b/sort/aaa");
    tree.insert("b/sort/a");
    tree.insert("b/sort/bbaa");
    tree.insert("b/sort/b");
    tree.insert("b/sort/baa");
    tree.insert("b/sort/bbbbb");
    tree.insert("b/sort/bbbbbc");
    tree.insert("b/sort/aaccb");
    tree.insert("b/sort/aacccc");
    tree.insert("b/f/f");
    tree.insert("b/b/b/b");
    tree.insert("b/f/f/f");
    tree.insert("b/b/b/b/b");
    tree.insert("b/f/f/f/f");
    tree.insert("b/b/b/bb");
    tree.insert("b/f/f/ff");
    tree.insert("b/a");
    tree.insert("b/a/b");
    tree.insert("b/a/b/c");
    tree.insert("b/a/b/c/d");
    tree.insert("b/a/b/c/d/f");
    tree.insert("b/a/b/c/d/f/g");
    tree.insert("b/a/b/c/d/f/g/e");
    tree.insert("level/a/f/f");
    tree.insert("level/b/f/f");
    tree.insert("level/c/f/f");
    tree.insert("level/d/f/f");
    tree.insert("level/f/f/f");
    tree.insert("level/g/f/f");
    tree.insert("level/h/f/f");
    tree.insert("level/i/f/f");
    tree.insert("level/j/f/f");
    tree.insert("level/k/f/f");
    tree.insert("level/l/f/f");
    tree.insert("level/m/f/f");
    tree.insert("level/n/f/f");
    tree.insert("level/o/f/f");
    tree.insert("level/p/f/f");
    tree.insert("level/q/f/f");
    tree.insert("level/r/f/f");
    tree.insert("level/s/f/f");
    tree.insert("level/t/f/f");
    tree.insert("level/v/f/f");
    tree.insert("level/u/f/f");
    tree.insert("level/w/f/f");
    tree.insert("level/x/f/f");
    tree.insert("level/y/f/f");
    tree.insert("level/z/f/f");
    
    tree.sort();

    for(DirectoryTree::AscOrderConstIterator it = tree.ascOrderFirst(); it; ++it) {
        std::cout << *it << std::endl;
    }

    return 0;
}
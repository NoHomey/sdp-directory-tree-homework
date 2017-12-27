/*#include "ChunkAllocator/ChunkAllocator.thd"
#include <iostream>
#include <cstring>

int main() {
    ChunkAllocator allocator;
    char* str = reinterpret_cast<char*>(allocator.allocate(10));
    std::memcpy(str, "Some Text ", 10);
    std::memcpy(allocator.allocate(19), "\nHellow World !!!", 19);

    std::cout << str << std::endl;

    return 0;
}*/

#include "DirectoryTree/DirectoryTree.h"
#include <iostream>

int main() {
    DirectoryTree tree{"./root"};
    tree.insert("file");
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

    for(DirectoryTree::AscOrderConstIterator it = tree.ascOrderFirst(); it; ++it) {
        std::cout << *it << std::endl;
    }

    std::cout << tree.findTreeDepth() << std::endl;

    return 0;
}
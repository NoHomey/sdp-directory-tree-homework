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

    tree.print();

    return 0;
}
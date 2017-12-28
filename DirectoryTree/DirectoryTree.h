#pragma once

#include "../Pair.thd"
#include "../DynamicArray/DynamicArray.thd"
#include "../ChunkAllocator/ChunkAllocator.h"

class DirectoryTree {
public:
    DirectoryTree(const char* rootDirectory);

    ~DirectoryTree() noexcept = default;

    DirectoryTree(const DirectoryTree& other) = delete;

    DirectoryTree(DirectoryTree&& other) noexcept = delete;

    DirectoryTree& operator=(const DirectoryTree& other) = delete;

    DirectoryTree& operator=(DirectoryTree&& other) noexcept = delete;

public:
    void insert(const char* path);

    void sort() noexcept;

public:
    class AscOrderConstIterator;

    AscOrderConstIterator ascOrderFirst() const noexcept;

private:
    struct Directory;

    class FilesMapper {
    public:
        struct Files {
        public:
            Files() noexcept;

            ~Files() noexcept = default;

            Files(const Files& other) = delete;

            Files(Files&& other) noexcept;

            Files& operator=(const Files& other) = delete;

            Files& operator=(Files&& other) noexcept = delete;
        
        public:
            struct File;

        public:
            void insert(const char* fileName);

        private:
            Files(File* first) noexcept;

        private:
            void null() noexcept;

        public:
            File* first;
        };

    public:
        FilesMapper() noexcept = default;

        ~FilesMapper() noexcept = default;

        FilesMapper(const FilesMapper& other) = delete;

        FilesMapper(FilesMapper&& other) noexcept = delete;

        FilesMapper& operator=(const FilesMapper& other) = delete;

        FilesMapper& operator=(FilesMapper&& other) noexcept = delete;

    public:
        Files* getFilesForDirectory(const Directory* directory) noexcept;

        const Files* getFilesForDirectory(const Directory* directory) const noexcept;

        void addFileToDirectory(const Directory* directory, const char* fileName);

        std::size_t countOfDirectoriesWithFiles() const noexcept;

        void sortFileNames() noexcept;

    private:
        struct FilesInDirectory {
        public:
            FilesInDirectory(const Directory* directory) noexcept;

            ~FilesInDirectory() noexcept = default;

            FilesInDirectory(const FilesInDirectory& other) = delete;

            FilesInDirectory(FilesInDirectory&& other) noexcept = default;

            FilesInDirectory& operator=(const FilesInDirectory& other) = delete;

            FilesInDirectory& operator=(FilesInDirectory&& other) noexcept = delete;

        public:
            const Directory* directory;

            Files files;
        };

    public:
        static const std::size_t initialFilesMapperCapacity = 1 << 9;

    private:
        DynamicArray<FilesInDirectory, ChunkAllocator> mapped;
    };

private:
    static std::size_t findPathDepth(const char* path) noexcept;

    static bool areDirectoryNamesEqual(const char* searchedName, const std::size_t searchedLength, const char* directoryName) noexcept;

private:
    template<typename ListNode>
    static bool isLessThan(ListNode* left, ListNode* right) noexcept;

    template<typename ListNode>
    static std::size_t sortPairsAndFindListLength(ListNode*& first) noexcept;

    template<typename ListNode>
    static ListNode* cut(ListNode* first, const std::size_t n) noexcept;

    template<typename ListNode>
    static ListNode* advanceAndReturnPrev(ListNode*& listNode) noexcept;

    template<typename ListNode>
    static Pair<ListNode*, ListNode*> merge(ListNode* left, ListNode* right) noexcept;

    template<typename ListNode>
    static void mergeSort(ListNode*& first) noexcept;

    static void mergeSortFiles(FilesMapper::Files::File*& first) noexcept;

private:
    void updateMaxPathLength(const char* path) noexcept;

    void updateMaxPathDepth(const char* path) noexcept;

    void updatePathCounters(const char* path) noexcept;

    Pair<Directory*, const char*> findDirectoryPath(const char* path) const noexcept;

    void sortDirectories() noexcept;

private:
    static ChunkAllocator allocator;

private:
    const std::size_t rootDirectoryNameLength;

    Directory* root;

    FilesMapper filesMapper;

    std::size_t maxPathLength;

    std::size_t maxPathDepth;
};

#include "Directory/Directory.h"
#include "FilesMapper/Files/File/File.h"
#include "AscOrderConstIterator/AscOrderConstIterator.h"
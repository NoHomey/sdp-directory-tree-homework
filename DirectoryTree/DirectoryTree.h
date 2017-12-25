#pragma once

#include "../Pair.thd"
#include "../DynamicArray/DynamicArray.thd"

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

    void print() const noexcept;

private:
    struct Directory;

    class FilesMapper {
    public:
        class Files {
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
            const File* iter() const noexcept;

        public:
            void insert(const char* fileName);

        private:
            Files(File* first, File* last) noexcept;

        private:
            void null() noexcept;

        private:
            File* first;

            File* last;
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

    private:
        DynamicArray<FilesInDirectory> mapped;
    };

private:
    static bool areDirectoryNamesEqual(const char* searchedName, const std::size_t searchedLength, const char* directoryName) noexcept;

private:
    Pair<Directory*, const char*> findDirectoryPath(const char* path) const noexcept; 

    void print(const Directory* dir, char* buffer, const std::size_t pos) const noexcept;

private:
    Directory* root;

    FilesMapper filesMapper;
};

#include "Directory/Directory.h"
#include "FilesMapper/Files/File/File.h"
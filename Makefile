PHONY: all clean

Objects = Chunk ChunkAllocator DirectoryEntryStack RecursiveDirectoryIterator \
Directory File Files FilesMapper AscOrderConstIterator DirectoryTree

mainFile = main.cc

all: $(addsuffix .o,$(Objects))
	$(COMPILER) $(mainFile) $^

clean:
	rm -f *.o

CXXFLAGS = -g -O0 -std=c++14 -Wall -Wextra -Wpedantic 

COMPILER = $(CXX) $(CPPFLAGS) $(CXXFLAGS)

SRC_DIR = .

dirDeps = $(SRC_DIR)/$(1)/$(2)/$(2).cc

define endl


endef

define obj
$(2).o: $(call dirDeps,$(1),$(2)) $(addsuffix .o,$(3))
	$(COMPILER) -c ./$(1)/$(2)/$(2).cc
endef

$(eval $(call obj,ChunkAllocator,Chunk))
$(eval $(call obj,.,ChunkAllocator,Chunk))
$(eval $(call obj,RecursiveDirectoryIterator,DirectoryEntryStack))
$(eval $(call obj,.,RecursiveDirectoryIterator,DirectoryEntryStack))
$(eval $(call obj,DirectoryTree,Directory))
$(eval $(call obj,DirectoryTree/FilesMapper/Files,File))
$(eval $(call obj,DirectoryTree/FilesMapper,Files))
$(eval $(call obj,DirectoryTree,FilesMapper))
$(eval $(call obj,DirectoryTree,AscOrderConstIterator))
$(eval $(call obj,.,DirectoryTree,Directory,ChunkAllocator))
#include <cassert>
#include "../ChunkAllocator/ChunkAllocator.h"
#include "FileMatcher.thd"

int main() {
    ChunkAllocator allocator;

    {
        const char* pattern = "abc";
        auto matcher = FileMatcher<ChunkAllocator>::createFileMatcher(pattern, &allocator);
        assert(matcher.match("abc"));
        assert(!matcher.match("abcabc"));
        assert(!matcher.match("abcd"));
    }

    {
        const char* pattern = "abc?";
        auto matcher = FileMatcher<ChunkAllocator>::createFileMatcher(pattern, &allocator);
        assert(matcher.match("abca"));
        assert(!matcher.match("abcabc"));
        assert(matcher.match("abcd"));
    }

    {
        const char* pattern = "*.??";
        auto matcher = FileMatcher<ChunkAllocator>::createFileMatcher(pattern, &allocator);
        assert(matcher.match("ab.ca"));
        assert(!matcher.match("abc.abc"));
        assert(!matcher.match("abc.d"));
    }

    {
        const char* pattern = "a*b*c";
        auto matcher = FileMatcher<ChunkAllocator>::createFileMatcher(pattern, &allocator);
        assert(matcher.match("abc"));
        assert(matcher.match("aaabbbc"));
        assert(matcher.match("aaabbbcccbc"));
    }

    {
        const char* pattern = "a**b***c.cc";
        auto matcher = FileMatcher<ChunkAllocator>::createFileMatcher(pattern, &allocator);
        assert(matcher.match("abc.cc"));
        assert(matcher.match("aaabbbc.cc"));
        assert(matcher.match("aaabbbcccbc.cc"));
    }

    {
        const char* pattern = "a**b***c.cc";
        auto matcher = FileMatcher<ChunkAllocator>::createFileMatcher(pattern, &allocator);
        assert(!matcher.match("dabc.cc"));
        assert(!matcher.match("daaabbbcd.cc"));
        assert(!matcher.match("aaabbbcccbc.dcc"));
    }

    {
        const char* pattern = "a*c.cc";
        auto matcher = FileMatcher<ChunkAllocator>::createFileMatcher(pattern, &allocator);
        assert(matcher.match("aaaaaaccccabccccc.cc"));
        assert(matcher.match("aaaabbbc.cc"));
        assert(matcher.match("aaabbbcccbababababaacc.cc"));
    }

    {
        const char* pattern = "a*c.??";
        auto matcher = FileMatcher<ChunkAllocator>::createFileMatcher(pattern, &allocator);
        assert(matcher.match("aaaaaaccccabccccc.cc"));
        assert(matcher.match("aaaabbbc.io"));
        assert(matcher.match("aaabbbcccbababababaacc.ef"));
    }

    {
        const char* pattern = "ivo*";
        auto matcher = FileMatcher<ChunkAllocator>::createFileMatcher(pattern, &allocator);
        assert(matcher.match("ivo"));
        assert(matcher.match("ivo.cc"));
        assert(matcher.match("ivo.txt"));
    }

    {
        const char* pattern = "*i?v?o*";
        auto matcher = FileMatcher<ChunkAllocator>::createFileMatcher(pattern, &allocator);
        assert(matcher.match("iivvo"));
        assert(matcher.match("iiiiivvvo.cc"));
        assert(matcher.match("iiiiivvvooooooo"));
    }

    {
        const char* pattern = "*";
        auto matcher = FileMatcher<ChunkAllocator>::createFileMatcher(pattern, &allocator);
        assert(matcher.match("iivvo"));
        assert(matcher.match("iiiiivvvo.cc"));
        assert(matcher.match("iiiiivvvooooooo"));
    }

    {
        const char* pattern = "?*?";
        auto matcher = FileMatcher<ChunkAllocator>::createFileMatcher(pattern, &allocator);
        assert(matcher.match("iv"));
        assert(matcher.match("ico"));
        assert(matcher.match("iivvo"));
        assert(matcher.match("iiiiivvvo.cc"));
        assert(matcher.match("iiiiivvvooooooo"));
    }

    {
        const char* pattern = "*?*?*";
        auto matcher = FileMatcher<ChunkAllocator>::createFileMatcher(pattern, &allocator);
        assert(matcher.match("ii"));
        assert(matcher.match("iiv"));
        assert(matcher.match("iivo"));
        assert(matcher.match("iivvo"));
        assert(matcher.match("iivvoo"));
        assert(matcher.match("iivvooo"));
    }

    {
        const char* pattern = "*****?******?****?";
        auto matcher = FileMatcher<ChunkAllocator>::createFileMatcher(pattern, &allocator);
        assert(matcher.match("iii"));
        assert(matcher.match("iiv.cc"));
        assert(matcher.match("iivo"));
        assert(matcher.match("iivvo"));
        assert(matcher.match("iivvoo"));
        assert(matcher.match("iivvooo"));
    }


    {   
        const char* pattern = "*****?**a*b***?****?";
        auto matcher = FileMatcher<ChunkAllocator>::createFileMatcher(pattern, &allocator);
        assert(matcher.match("iabii"));
        assert(matcher.match("iabbiv.cc"));
        assert(matcher.match("iaabbivo"));
        assert(matcher.match("iiaaaabbvvo"));
        assert(matcher.match("iivabvoo"));
        assert(matcher.match("iivvababaooo"));
    }

    {   
        const char* pattern = "*a*b*c*d*";
        auto matcher = FileMatcher<ChunkAllocator>::createFileMatcher(pattern, &allocator);
        assert(matcher.match("abcd"));
        assert(matcher.match("aabcd"));
        assert(matcher.match("aabcdd"));
        assert(matcher.match("aabcdbcd"));
        assert(matcher.match("aaaaabbbbbcccccddddd"));
        assert(matcher.match("abcddbcabcd"));
    }

    return 0;
}
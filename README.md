static-string-hash
==================

compile-time string hashing, based on murmurhash.

With C++11 constexpr and user-defined-literals facilities it is possible
to create compile-time string hashing functions.

As of writing this, with murmurhash3 it is possible to generate either
a 32- or 128-bit hashes. My preference was to have a 64-bit hash, so
I took 32-bit murmurhash3 algorithm (https://en.wikipedia.org/wiki/MurmurHash#Algorithm)
and tweaked it into a 64-bit version using constants taken from the 128-bit version.
Specifically:
c1 := 0x87c37b91114253d5
c2 := 0x4cf5ad432745937f
r1 := 31
r2 := 27
m := 5
n := 0x52dce729
I haven't evaluated whether this set makes sense, so use it at your own risk...

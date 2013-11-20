/*******************************************************************************
 * Copyright (c) 2013 Wojciech Migda
 * All rights reserved
 * Distributed under the terms of the MIT license
 *******************************************************************************
 *
 * Filename: StaticStringHash.hpp
 *
 * Description:
 *      description
 *
 * Authors:
 *          Wojciech Migda (wm)
 *
 *******************************************************************************
 * History:
 * --------
 * Date         Who  Ticket     Description
 * ----------   ---  ---------  ------------------------------------------------
 * 2013-11-20   wm              Initial version
 *
 ******************************************************************************/


#ifndef STATICSTRINGHASH_HPP_
#define STATICSTRINGHASH_HPP_

#include <cstdint>
#include <cstddef>
#include <climits>

static_assert(CHAR_BIT == 8, "8-bit byte architecture is required");

namespace StaticStringHash
{

namespace Detail
{

typedef std::uint64_t ull_t;

/*
 * read enough bytes of string's tail, byte order is reverted
 */
constexpr ull_t tail_to_block(const char * str, const std::size_t size)
{
    return
          (size >= 8 ? (ull_t)str[7] << 7 * CHAR_BIT : 0)
        | (size >= 7 ? (ull_t)str[6] << 6 * CHAR_BIT : 0)
        | (size >= 6 ? (ull_t)str[5] << 5 * CHAR_BIT : 0)
        | (size >= 5 ? (ull_t)str[4] << 4 * CHAR_BIT : 0)
        | (size >= 4 ? (ull_t)str[3] << 3 * CHAR_BIT : 0)
        | (size >= 3 ? (ull_t)str[2] << 2 * CHAR_BIT : 0)
        | (size >= 2 ? (ull_t)str[1] << 1 * CHAR_BIT : 0)
        | (size >= 1 ? (ull_t)str[0] << 0 * CHAR_BIT : 0);
}

/*
 * read 8 bytes to form an ull_t block, native byte order
 */
constexpr ull_t get_block(const char * str)
{
    return
          ((ull_t)str[7] << 0 * CHAR_BIT)
        | ((ull_t)str[6] << 1 * CHAR_BIT)
        | ((ull_t)str[5] << 2 * CHAR_BIT)
        | ((ull_t)str[4] << 3 * CHAR_BIT)
        | ((ull_t)str[3] << 4 * CHAR_BIT)
        | ((ull_t)str[2] << 5 * CHAR_BIT)
        | ((ull_t)str[1] << 6 * CHAR_BIT)
        | ((ull_t)str[0] << 7 * CHAR_BIT);
}

/*
 * Righ-Shift-and-Xor
 */
constexpr ull_t rsx(const ull_t block, const std::size_t n)
{
    return block ^ (block >> n);
}

/*
 * xor wrapper
 */
constexpr ull_t fxor(const ull_t lhs, const ull_t rhs)
{
    return lhs ^ rhs;
}

/*
 * finalizer piece
 */
constexpr ull_t finalizer(const ull_t block)
{
    return
        rsx(rsx(rsx(block, 33) * 0xff51afd7ed558ccd, 33) * 0xc4ceb9fe1a85ec53, 33);
}

/*
 * Rotate Left
 */
constexpr ull_t rol(const ull_t block, const std::size_t n)
{
    return (block << n) | (block >> (64 - n));
}

/*
 * mixing of block contents, uses c1, c2, and r1 constants
 */
constexpr ull_t mix_block(const ull_t block)
{
    return (rol(block * 0x87c37b91114253d5, 31)) * 0x4cf5ad432745937f;
}

/*
 * mixing of hash contents against block, uses r2, m, and n constants
 */
constexpr ull_t mix_hash(const ull_t hash, const ull_t block)
{
    return rol(block ^ hash, 27) * 5 + 0x52dce729;
}

/*
 * processing of a single block
 */
constexpr ull_t do_block(const ull_t hash, const ull_t block)
{
    return mix_hash(hash, mix_block(block));
}

/*
 * recursive processing of all blocks
 */
constexpr ull_t do_blocks(const ull_t hash, const char * str, const std::size_t size)
{
    return
        size == 0 ?
            do_block(hash, get_block(str)) :
            do_blocks(hash, str, size - sizeof (ull_t));
}

/*
 * processing of the tail
 */
constexpr ull_t do_tail(const ull_t hash, const char *str, const std::size_t size)
{
    return
        size == 0 ?
            hash :
            fxor(
                hash,
                mix_block(
                    tail_to_block(str, size)
                )
            );
}

/*
 * main hash function, processes all blocks and the tail
 */
constexpr ull_t hash(const ull_t seed, const char * str, const std::size_t size)
{
    return
        finalizer(
            fxor(
                do_tail(
                    size >= sizeof (ull_t) ?
                        do_blocks(seed, str, size & ((ull_t)-1 << sizeof (ull_t))) :
                        seed,
                    str + (size & ((ull_t)-1 << sizeof (ull_t))),
                    size % sizeof (ull_t)
                ),
                size
            )
        );
}

} // namespace Detail

} // namespace StaticStringHash

constexpr unsigned long long operator"" _hash(char const * const i_str, std::size_t const i_size)
{
    return StaticStringHash::Detail::hash(0xBADC0FFEDEADBULL, i_str, i_size);
}

#endif /* STATICSTRINGHASH_HPP_ */

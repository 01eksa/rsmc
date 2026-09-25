// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright (c) 2026 01eksa

/**
 * @file utils.h
 * @brief Utils, mostly type extensions. Private header.
 *
 */

#ifndef RSMC_UTILS_H
#define RSMC_UTILS_H

#include "rsmc/abstractions.h"
#include "rsmc/core.h"
#include <assert.h>

#ifdef __cplusplus
extern "C" {
#endif

#if defined(_MSC_VER)
#include <intrin.h>
#define popcnt64(x)  __popcnt64(x)
#elif defined(__GNUC__) || defined(__clang__)
#define popcnt64(x)  __builtin_popcountll(x)
#else
static inline int popcnt64(uint64_t x) {
    x = x - ((x >> 1) & 0x5555555555555555ULL);
    x = (x & 0x3333333333333333ULL) + ((x >> 2) & 0x3333333333333333ULL);
    x = (x + (x >> 4)) & 0x0F0F0F0F0F0F0F0FULL;
    return (x * 0x0101010101010101ULL) >> 56;
}
#endif

/**
 * Converts player type to cell type without validation
 *
 * @param player RsmcPlayer to convert.
 * @return corresponding RsmcBoardCell (White or Black).
 */
static inline RsmcBoardCell rsmc_player_to_cell(const RsmcPlayer player)
{
    static_assert(RsmcPlayerBlack + 1 == RsmcBoardCellBlack, "cannot convert black player to cell");
    static_assert(RsmcPlayerWhite + 1 == RsmcBoardCellWhite, "cannot convert white player to cell");
    return player + 1;
}

/**
 * Converts player to opposite color without validation.
 *
 * @param player player to get opposite for.
 * @return opposite color player.
 */
static inline RsmcPlayer rsmc_player_opposite(const RsmcPlayer player)
{
    static_assert(RsmcPlayerBlack == !RsmcPlayerWhite && RsmcPlayerWhite == !RsmcPlayerBlack,
                  "cannot toggle players with !");
    return (RsmcPlayer)!player;
}

/**
 * Adds two coords and returns the result.
 *
 * @param left left coords.
 * @param right right coords.
 * @return left + right.
 */
static inline RsmcCoords rsmc_coords_add(const RsmcCoords left, const RsmcCoords right)
{
    RsmcCoords result;
    result.x = (int8_t)(left.x + right.x);
    result.y = (int8_t)(left.y + right.y);
    return result;
}

/**
 * Subtracts coords and returns the result.
 *
 * @param left left coords.
 * @param right right coords.
 * @return left - right.
 */
static inline RsmcCoords rsmc_coords_sub(const RsmcCoords left, const RsmcCoords right)
{
    RsmcCoords result;
    result.x = (int8_t)(left.x - right.x);
    result.y = (int8_t)(left.y - right.y);
    return result;
}

#ifdef __cplusplus
}
#endif

#endif // RSMC_UTILS_H

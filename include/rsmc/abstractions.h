// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright (c) 2026 01eksa

/**
 * @file abstractions.h
 * @brief Thin abstract layer for rsmc.
 *
 */

#ifndef RSMC_ABSTRACTIONS_H
#define RSMC_ABSTRACTIONS_H

#include "core.h"
#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Board cell (empty, black, white)
 */
typedef uint8_t RsmcBoardCell;
enum {
    RsmcBoardCellEmpty = 0,
    RsmcBoardCellBlack = 1,
    RsmcBoardCellWhite = 2,
    RsmcBoardCellCount = 3,
};
/**
 * @brief Checks if number is a valid RsmcBoardCell.
 *
 * @param cell RsmcBoardCell to validate.
 * @return true if cell is valid, false if not.
 */
static inline bool rsmc_board_cell_is_valid(const RsmcBoardCell cell)
{
    return cell < RsmcBoardCellCount;
}

/**
 * @brief Stores board in 2-dimensional array of all RsmcBoardCell.
 */
typedef struct {
    RsmcBoardCell cells[RsmcBoardSize][RsmcBoardSize];
} RsmcClassicBoard;
static_assert(sizeof(RsmcClassicBoard) == 64, "unexpected padding in RsmcBoard");

/**
 * @brief Stores coords for the board.
 * Coords might be invalid and negative.
 */
typedef struct {
    int8_t x;
    int8_t y;
} RsmcCoords;
static_assert(sizeof(RsmcCoords) == 2, "unexpected padding in RsmcCoords");
/**
 * @brief Checks if coords are valid for RsmcBoard.
 *
 * @param coords RsmcCoords to validate.
 * @return true if coords are valid, false if not.
 */
static inline bool rsmc_coords_is_valid(const RsmcCoords coords)
{
    return coords.x >= RsmcMinCoord && coords.y >= RsmcMinCoord && coords.x <= RsmcMaxCoord &&
           coords.y <= RsmcMaxCoord;
}

/**
 * @brief Converts RsmcCoords to integer index.
 *
 * @param coords RsmcCoords to convert.
 * @return -1 if coords are invalid or index between 0 and 63.
 */
static inline int rsmc_coords_to_index(const RsmcCoords coords)
{
    int index = coords.y * RsmcBoardSize + coords.x;
    if (index < 0 || index >= RsmcBoardArea)
        index = -1;

    return index;
}

/**
 * @brief Converts cell index to RsmcBitMask
 *
 * @param index cell index to convert.
 * @return 0 if index is invalid or bitmask.
 */
static inline RsmcBitMask rsmc_index_to_bitmask(const uint8_t index)
{
    if (index >= RsmcBoardArea || index >= sizeof(uint64_t) * 8)
        return 0;
    return (uint64_t)1 << index;
}

static inline RsmcBoardCell rsmc_board_get_cell(const RsmcBoard board, const RsmcCoords coords)
{
    const RsmcBitMask mask = rsmc_index_to_bitmask(rsmc_coords_to_index(coords));
    if (board.black & mask)
        return RsmcBoardCellBlack;
    if (board.white & mask)
        return RsmcBoardCellWhite;
    return RsmcBoardCellEmpty;
}

static inline RsmcBoard rsmc_board_set_cell(RsmcBoard board, const RsmcCoords coords,
                                            const RsmcBoardCell cell)
{
    const RsmcBitMask mask = rsmc_index_to_bitmask(rsmc_coords_to_index(coords));
    board.black &= ~mask;
    board.white &= ~mask;

    if (cell == RsmcBoardCellBlack) {
        board.black |= mask;
    } else if (cell == RsmcBoardCellWhite) {
        board.white |= mask;
    }

    return board;
}

static inline RsmcClassicBoard rsmc_classic_board_from_board(const RsmcBoard board)
{
    RsmcClassicBoard classic_board;
    for (int y = 0; y < RsmcBoardSize; y++) {
        for (int x = 0; x < RsmcBoardSize; x++) {
            classic_board.cells[y][x] =
                rsmc_board_get_cell(board, (RsmcCoords){.x = (int8_t)x, .y = (int8_t)y});
        }
    }

    return classic_board;
}

static inline RsmcBoard rsmc_board_from_classic_board(const RsmcClassicBoard classic_board)
{
    RsmcBoard board = {0};
    for (int y = 0; y < RsmcBoardSize; y++) {
        for (int x = 0; x < RsmcBoardSize; x++) {
            const RsmcCoords coords = {.x = (int8_t)x, .y = (int8_t)y};
            if (classic_board.cells[y][x] == RsmcBoardCellBlack) {
                board.black |= rsmc_index_to_bitmask(rsmc_coords_to_index(coords));
            } else if (classic_board.cells[y][x] == RsmcBoardCellWhite) {
                board.white |= rsmc_index_to_bitmask(rsmc_coords_to_index(coords));
            }
        }
    }

    return board;
}

#ifdef __cplusplus
}
#endif

#endif // RSMC_ABSTRACTIONS_H

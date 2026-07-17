/**
 * @file utils.h
 * @brief Utils, mostly type extensions. Private header.
 *
 */

#ifndef RSMC_UTILS_H
#define RSMC_UTILS_H

#include "rsmc/types.h"
#include <assert.h>

#ifdef __cplusplus
extern "C" {
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
    return (RsmcPlayer)!player; // assume exactly two players (RsmcPlayerWhite/RsmcPlayerBlack).
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

/**
 * Returns mutable pointer to cell based on coords without validation.
 *
 * @param board a pointer to RsmcBoard.
 * @param coords coords of a cell.
 * @return a mutable pointer to the cell.
 */
static inline RsmcBoardCell *rsmc_cell_at(RsmcBoard *board, const RsmcCoords coords)
{
    return &board->cells[coords.y][coords.x];
}

/**
 * Returns constant pointer to cell based on coords without validation.
 *
 * @param board a constant pointer to RsmcBoard.
 * @param coords a coords of a cell.
 * @return a constant pointer to the cell.
 */
static inline const RsmcBoardCell *rsmc_cell_at_const(const RsmcBoard *board,
                                                      const RsmcCoords coords)
{
    return &board->cells[coords.y][coords.x];
}

#ifdef __cplusplus
}
#endif

#endif // RSMC_UTILS_H

/**
 * @file rsmc.h
 * @brief Reversi stateless model in C - API.
 *
 */

#ifndef RSMC_RSMC_H
#define RSMC_RSMC_H

#include "rsmc/abstractions.h"
#include "rsmc/core.h"
#include "rsmc/version.h"

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * rsmc version as uint32_t.
 * @return major, minor and patch in one number.
 */
RsmcVersion rsmc_version(void);

/**
 * Start position for Reversi.
 */
const static RsmcBoard RsmcStartPosition = {.black = 0x810000000, .white = 0x1008000000};

/**
 * Returns board with Reversi start position.
 * @return RsmcStartPosition.
 */
RsmcBoard rsmc_get_start_position(void);

/**
 * @brief Set piece and flip opponent's pieces if move is valid.
 *
 * @param board current RsmcBoard.
 * @param cell_bit_mask RsmcBitMask of cell where you want to place a piece.
 * @param player RsmcPlayer to move.
 * @return new RsmcBoard after move (or old if move is invalid).
 */
RsmcBoard rsmc_apply_move(RsmcBoard board, RsmcBitMask cell_bit_mask, RsmcPlayer player);

/**
 * @brief Calculates score of both players.
 *
 * @param board RsmcBoard.
 * @return RsmcPlayersScore which contains score for both players.
 */
RsmcPlayersScore rsmc_get_players_score(RsmcBoard board);
/**
 * @brief Checks if move is valid.
 *
 * @param board a constant pointer to RsmcBoard.
 * @param cell_bit_mask RsmcBitMask of cell where you want to place a piece.
 * @param player RsmcPlayer to check a move for.
 * @return true if move is valid, false if not (or coords / player is invalid)
 */
bool rsmc_is_move_valid(RsmcBoard board, RsmcBitMask cell_bit_mask, RsmcPlayer player);

/**
 * @brief Finds all valid moves for given player.
 *
 * @param board RsmcBoard.
 * @param player RsmcPlayer to search valid moves for.
 * @return RsmcBitMap, where 1 represents valid moves.
 */
RsmcBitMap rsmc_get_valid_moves(RsmcBoard board, RsmcPlayer player);

/** @brief Analyzes current game state: finds player score and game status.
 *
 * @param board a constant pointer to RsmcBoard.
 * @return current RsmcGameState.
 */
RsmcGameState rsmc_get_game_state(RsmcBoard board);

#ifdef __cplusplus
}
#endif

#endif // RSMC_RSMC_H

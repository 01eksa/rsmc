/**
 * @file rsmc.h
 * @brief Reversi stateless model in C - API.
 *
 */

#ifndef RSMC_RSMC_H
#define RSMC_RSMC_H
#include "rsmc/types.h"

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Sets start position on the board.
 *
 * @param board a pointer to RsmcBoard, which will be changed.
 */
void rsmc_set_start_position(RsmcBoard *board);

/**
 * @brief Set piece and flip opponent's pieces if move is valid.
 *
 * @param board a pointer to RsmcBoard, which will be changed if success.
 * @param coords RsmcCoords of cell where you want to place a piece.
 * @param player RsmcPlayer to make a move for.
 * @return true if success, false if move is impossible or coords / player are invalid.
 */
bool rsmc_apply_move(RsmcBoard *board, RsmcCoords coords, RsmcPlayer player);

/**
 * @brief Calculates score of both players in one pass.
 *
 * @param board a constant pointer to RsmcBoard.
 * @return RsmcPlayersScore which contains score for both players.
 */
RsmcPlayersScore rsmc_get_players_score(const RsmcBoard *board);

/**
 * @brief Checks if move is valid.
 *
 * @param board a constant pointer to RsmcBoard.
 * @param coords RsmcCoords of cell where you want to place a piece.
 * @param player RsmcPlayer to check a move for.
 * @return true if move is valid, false if not (or coords / player is invalid)
 */
bool rsmc_is_move_valid(const RsmcBoard *board, RsmcCoords coords, RsmcPlayer player);

/**
 * @brief Finds all valid moves.
 *
 * @param board a constant pointer to RsmcBoard.
 * @param player RsmcPlayer to search valid moves for.
 * @return RsmcMoves, where counts will be 0 if there are no legal moves or player is invalid.
 */
RsmcMoves rsmc_get_valid_moves(const RsmcBoard *board, RsmcPlayer player);

/** @brief Analyzes current game state: finds player score and game status.
 *
 * @param board a constant pointer to RsmcBoard.
 * @return current RsmcGameState.
 */
RsmcGameState rsmc_get_game_state(const RsmcBoard *board);

#ifdef __cplusplus
}
#endif

#endif // RSMC_RSMC_H

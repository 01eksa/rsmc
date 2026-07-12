/**
 * @file rsmc.h
 * @brief Types and constants for rsmc.
 *
 */

#ifndef RSMC_TYPES_H
#define RSMC_TYPES_H

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// enums

enum {
    /**
     * @brief Size of one side of the board.
     */
    RsmcBoardSize = 8,
    /**
     * @brief Minimal possible value for index on the board.
     */
    RsmcMinCoord = 0,
    /**
     * @brief Maximal possible value for index on the board.
     */
    RsmcMaxCoord = RsmcBoardSize - 1,
    /**
     * @brief Maximal possible count of valid moves.
     * Proven by Takizawa, Hiroki in 2023.
     */
    RsmcMaxValidMoves = 34, // theoretical max count of valid moves
};

/**
 * @brief Player's color.
 */
typedef uint8_t RsmcPlayer;
enum {
    RsmcPlayerWhite = 0,
    RsmcPlayerBlack = 1,
    RsmcPlayerCount = 2,
};
/**
 * @brief Checks if number is a valid RsmcPlayer.
 *
 * @param player RsmcPlayer to validate.
 * @return true if player is valid, false if not.
 */
static inline bool rsmc_player_is_valid(const RsmcPlayer player)
{
    return player < RsmcPlayerCount;
}

/**
 * @brief Game status (continue, draw, white win, black win).
 */
typedef uint8_t RsmcGameStatus;
enum {
    RsmcGameStatusContinue = 0,
    RsmcGameStatusDraw = 1,
    RsmcGameStatusWhiteWin = 2,
    RsmcGameStatusBlackWin = 3,
    RsmcGameStatusCount = 4,
};
/**
 * @brief Checks if number is a valid RsmcGameStatus.
 *
 * @param status RsmcGameStatus to validate.
 * @return true if status is valid, false if not.
 */
static inline bool rsmc_game_status_is_valid(const RsmcGameStatus status)
{
    return status < RsmcGameStatusCount;
}

/**
 * @brief Board cell (empty, white, black)
 */
typedef uint8_t RsmcBoardCell;
enum {
    RsmcBoardCellEmpty = 0,
    RsmcBoardCellWhite = 1,
    RsmcBoardCellBlack = 2,
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

// structs

/**
 * @brief Stores score fot both players.
 */
typedef struct {
    uint8_t white_score;
    uint8_t black_score;
} RsmcPlayersScore;
static_assert(sizeof(RsmcPlayersScore) == 2, "unexpected padding in RsmcPlayersScore");

/**
 * @brief Stores score for both players and game status.
 */
typedef struct {
    RsmcPlayersScore score;
    RsmcGameStatus game_status;
} RsmcGameState;
static_assert(sizeof(RsmcGameState) == 3, "unexpected padding in RsmcGameState");

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
    return coords.x >= 0 && coords.y >= 0 && coords.x < RsmcBoardSize && coords.y < RsmcBoardSize;
}

/**
 * @brief Stores board in 2-dimensional array of all RsmcBoardCell.
 */
typedef struct {
    RsmcBoardCell cells[RsmcBoardSize][RsmcBoardSize];
} RsmcBoard;
static_assert(sizeof(RsmcBoard) == 64, "unexpected padding in RsmcBoard");

/**
 * @brief Stores valid moves.
 */
typedef struct {
    /**
     * @brief Count of valid moves.
     */
    uint8_t count;
    /**
     * Valid moves, only on indexes [0, count)
     */
    RsmcCoords coords[RsmcMaxValidMoves];
} RsmcMoves;
static_assert(sizeof(RsmcMoves) == 69, "unexpected padding in RsmcMoves");

#ifdef __cplusplus
}
#endif

#endif // RSMC_TYPES_H

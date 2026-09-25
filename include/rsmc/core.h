/**
 * @file rsmc.h
 * @brief Types and constants for rsmc.
 *
 */

#ifndef RSMC_CORE_H
#define RSMC_CORE_H

#include "version.h"

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef uint32_t RsmcVersion;
static inline RsmcVersion rsmc_build_version(void)
{
    const RsmcVersion version = 0;
    static_assert((RSMC_VERSION_MAJOR & 0xFF) == RSMC_VERSION_MAJOR, "Major version out of range");
    static_assert((RSMC_VERSION_MINOR & 0xFF) == RSMC_VERSION_MINOR, "Minor version out of range");
    static_assert((RSMC_VERSION_PATCH & 0xFFFF) == RSMC_VERSION_PATCH, "Patch version out of range");
    return version | RSMC_VERSION_PATCH  | (RSMC_VERSION_MINOR << 16) | (RSMC_VERSION_MAJOR << 24);
}

// enums

enum {
    /**
     * @brief Size of one side of the board.
     */
    RsmcBoardSize = 8,
    /**
     * @brief Count of cells on the board.
     */
    RsmcBoardArea = RsmcBoardSize * RsmcBoardSize,
    /**
     * @brief Minimal possible value for index on the 2-dimensional board.
     */
    RsmcMinCoord = 0,
    /**
     * @brief Maximal possible value for index on the 2-dimensional board.
     */
    RsmcMaxCoord = RsmcBoardSize - 1,
    /**
     * @brief Minimal possible value for index on the 1-dimensional board.
     */
    RsmcMinCellIndex = 0,
    /**
     * @brief Maximal possible value for index on the 1-dimensional board.
     */
    RsmcMaxCellIndex = RsmcBoardArea - 1,
    /**
     * @brief Maximal possible count of valid moves.
     * Proven by Takizawa, Hiroki in 2023.
     */
    RsmcMaxValidMoves = 34,
};

/**
 * @brief Player's color.
 */
typedef uint8_t RsmcPlayer;
enum {
    RsmcPlayerBlack = 0,
    RsmcPlayerWhite = 1,
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
 * @brief Game status (continue, draw, black win, white win).
 */
typedef uint8_t RsmcGameStatus;
enum {
    RsmcGameStatusContinue = 0,
    RsmcGameStatusDraw = 1,
    RsmcGameStatusBlackWin = 2,
    RsmcGameStatusWhiteWin = 3,
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

// structs

/**
 * Stores score for one player.
 */
typedef uint8_t RsmcPlayerScore;
/**
 * @brief Stores score fot both players.
 */
typedef struct {
    RsmcPlayerScore black_score;
    RsmcPlayerScore white_score;
} RsmcPlayersScore;
static_assert(sizeof(RsmcPlayersScore) == sizeof(RsmcPlayerScore) * 2, "unexpected padding in RsmcPlayersScore");

/**
 * @brief Stores score for both players and game status.
 */
typedef struct {
    RsmcPlayersScore score;
    RsmcGameStatus game_status;
} RsmcGameState;
static_assert(sizeof(RsmcGameState) == 3, "unexpected padding in RsmcGameState");

typedef uint64_t RsmcBitMap;
typedef uint64_t RsmcBitMask;

/**
 * @brief Stores board as two RsmcBitMap for each player pieces.
 */
typedef struct {
    union {
        struct {
            RsmcBitMap black;
            RsmcBitMap white;
        };
        RsmcBitMap by_player[2];
    };
} RsmcBoard;
static_assert(sizeof(RsmcBoard) == sizeof(uint64_t) * 2, "unexpected padding in RsmcBoard");

#ifdef __cplusplus
}
#endif

#endif // RSMC_CORE_H

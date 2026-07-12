#ifndef RSMC_TYPES_H
#define RSMC_TYPES_H
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// enums

enum {
    RsmcBoardSize = 8,
    RsmcMinCoord = 0,
    RsmcMaxCoord = RsmcBoardSize - 1,
    RsmcMaxValidMoves = 34, // theoretical max count of valid moves
};

typedef uint8_t RsmcPlayer;
enum {
    RsmcPlayerWhite = 0,
    RsmcPlayerBlack = 1,
    RsmcPlayerCount = 2,
};
static inline bool rsmc_player_is_valid(const RsmcPlayer player)
{
    return player < RsmcPlayerCount;
}

typedef uint8_t RsmcGameStatus;
enum {
    RsmcGameStatusContinue = 0,
    RsmcGameStatusDraw = 1,
    RsmcGameStatusWhiteWin = 2,
    RsmcGameStatusBlackWin = 3,
    RsmcGameStatusCount = 4,
};
static inline bool rsmc_game_status_is_valid(const RsmcGameStatus status)
{
    return status < RsmcGameStatusCount;
}

typedef uint8_t RsmcBoardCell;
enum {
    RsmcBoardCellEmpty = 0,
    RsmcBoardCellWhite = 1,
    RsmcBoardCellBlack = 2,
    RsmcBoardCellCount = 3,
};
static inline bool rsmc_board_cell_is_valid(const RsmcBoardCell cell)
{
    return cell < RsmcBoardCellCount;
}

// structs

typedef struct {
    uint8_t white_score;
    uint8_t black_score;
} RsmcPlayersScore;
_Static_assert(sizeof(RsmcPlayersScore) == 2, "unexpected padding in RsmcPlayersScore");

typedef struct {
    RsmcPlayersScore score;
    RsmcGameStatus game_status;
} RsmcGameState;
_Static_assert(sizeof(RsmcGameState) == 3, "unexpected padding in RsmcGameState");

typedef struct {
    int8_t x;
    int8_t y;
} RsmcCoords;
_Static_assert(sizeof(RsmcCoords) == 2, "unexpected padding in RsmcCoords");
bool static inline rsmc_coords_is_valid(const RsmcCoords coords)
{
    return coords.x >= 0 && coords.y >= 0 && coords.x < RsmcBoardSize && coords.y < RsmcBoardSize;
}

typedef struct {
    RsmcBoardCell cells[RsmcBoardSize][RsmcBoardSize];
} RsmcBoard;
_Static_assert(sizeof(RsmcBoard) == 64, "unexpected padding in RsmcBoard");

typedef struct {
    uint8_t count;
    RsmcCoords coords[RsmcMaxValidMoves];
} RsmcMoves;
_Static_assert(sizeof(RsmcMoves) == 69, "unexpected padding in RsmcMoves");

// type extensions

static inline RsmcPlayer rsmc_cell_to_player(const RsmcBoardCell cell)
{
    return cell - 1;
}

static inline RsmcBoardCell rsmc_player_to_cell(const RsmcPlayer player)
{
    return player + 1;
}

// Assumes exactly two players (RsmcPlayerWhite/RsmcPlayerBlack).
static inline RsmcPlayer rsmc_player_opposite(const RsmcPlayer player)
{
    return (RsmcPlayer)!player;
}

static inline RsmcCoords rsmc_coords_add(const RsmcCoords left, const RsmcCoords right)
{
    RsmcCoords result;
    result.x = (int8_t)(left.x + right.x);
    result.y = (int8_t)(left.y + right.y);
    return result;
}

static inline RsmcCoords rsmc_coords_sub(const RsmcCoords left, const RsmcCoords right)
{
    RsmcCoords result;
    result.x = (int8_t)(left.x - right.x);
    result.y = (int8_t)(left.y - right.y);
    return result;
}

static inline RsmcBoardCell *rsmc_cell_at(RsmcBoard *board, const RsmcCoords coords)
{
    return &board->cells[coords.y][coords.x];
}

#ifdef __cplusplus
}
#endif

#endif // RSMC_TYPES_H

#ifndef RSMC_TYPES_H
#define RSMC_TYPES_H
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// types

enum RsmcBoardLimits {
    RsmcBoardSize = 8,
    RsmcMinCoord = 0,
    RsmcMaxCoord = RsmcBoardSize - 1
};

typedef enum {
    RsmcPlayerWhite,
    RsmcPlayerBlack
} RsmcPlayer;

typedef enum {
    RsmcGameStatusContinue,
    RsmcGameStatusDraw,
    RsmcGameStatusWhiteWin,
    RsmcGameStatusBlackWin
} RsmcGameStatus;

typedef enum {
    RsmcBoardCellEmpty,
    RsmcBoardCellWhite,
    RsmcBoardCellBlack
} RsmcBoardCell;

typedef struct {
    int8_t x;
    int8_t y;
} RsmcCoords;

typedef struct {
    RsmcBoardCell cells[RsmcBoardSize][RsmcBoardSize];
} RsmcBoard;

typedef struct {
    uint8_t count;
    RsmcCoords coords[34];
} RsmcMoves;

// type extensions

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

#ifdef __cplusplus
}
#endif

#endif // RSMC_TYPES_H

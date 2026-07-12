#ifndef RSMC_DIRECTIONS_H
#define RSMC_DIRECTIONS_H
#include "rsmc/types.h"

#ifdef __cplusplus
extern "C" {
#endif

static const RsmcCoords RsmcUp = {0, -1};
static const RsmcCoords RsmcDown = {0, 1};
static const RsmcCoords RsmcLeft = {-1, 0};
static const RsmcCoords RsmcRight = {1, 0};
static const RsmcCoords RsmcUpRight = {1, -1};
static const RsmcCoords RsmcDownRight = {1, 1};
static const RsmcCoords RsmcUpLeft = {-1, -1};
static const RsmcCoords RsmcDownLeft = {-1, 1};

static const RsmcCoords RsmcDirections[] = {
    RsmcUp, RsmcDown, RsmcLeft, RsmcRight, RsmcUpRight, RsmcDownRight, RsmcUpLeft, RsmcDownLeft,
};
static const int RsmcDirectionsCount = sizeof(RsmcDirections) / sizeof(RsmcCoords);

#ifdef __cplusplus
}
#endif

#endif // RSMC_DIRECTIONS_H

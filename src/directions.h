#ifndef RSMC_DIRECTIONS_H
#define RSMC_DIRECTIONS_H
#include "rsmc/types.h"

#ifdef __cplusplus
extern "C" {
#endif

static const RsmcCoords RSMC_UP = {0, -1};
static const RsmcCoords RSMC_DOWN = {0, 1};
static const RsmcCoords RSMC_LEFT = {-1, 0};
static const RsmcCoords RSMC_RIGHT = {1, 0};
static const RsmcCoords RSMC_UP_RIGHT = {1, -1};
static const RsmcCoords RSMC_DOWN_RIGHT = {1, 1};
static const RsmcCoords RSMC_UP_LEFT = {-1, -1};
static const RsmcCoords RSMC_DOWN_LEFT = {-1, 1};

#ifdef __cplusplus
}
#endif

#endif // RSMC_DIRECTIONS_H

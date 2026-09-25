/**
 * @file directions.h
 * @brief RsmcCoords constants for directions. Private header.
 */

#ifndef RSMC_DIRECTIONS_H
#define RSMC_DIRECTIONS_H
#include "rsmc/abstractions.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef uint8_t RsmcDirectionNum;
enum {
    RsmcUpNum = 3,
    RsmcDownNum = 5,
    RsmcLeftNum = 1,
    RsmcRightNum = 7,
    RsmcUpRightNum = 6,
    RsmcDownRightNum = 8,
    RsmcUpLeftNum = 0,
    RsmcDownLeftNum = 2,
    RsmcDirectionsCount = 8,
};

static const RsmcDirectionNum RsmcDirectionsNum[] = {
    RsmcUpNum,      RsmcDownNum,      RsmcLeftNum,   RsmcRightNum,
    RsmcUpRightNum, RsmcDownRightNum, RsmcUpLeftNum, RsmcDownLeftNum};

static inline uint8_t rsmc_direction_to_number(const RsmcCoords direction)
{
    return 3 * (direction.x + 1) + (direction.y + 1);
}

static inline uint64_t rsmc_shift_in_direction(const uint64_t mask,
                                               const RsmcDirectionNum direction)
{
    const uint64_t NotX0 = 0xFEFEFEFEFEFEFEFE;
    const uint64_t NotX7 = 0x7F7F7F7F7F7F7F7F;

    switch (direction) {
        case RsmcDownNum:
            return mask << 8;
        case RsmcUpNum:
            return mask >> 8;
        case RsmcRightNum:
            return (mask << 1) & NotX0;
        case RsmcLeftNum:
            return (mask >> 1) & NotX7;
        case RsmcDownRightNum:
            return (mask << 9) & NotX0;
        case RsmcUpRightNum:
            return (mask >> 7) & NotX0;
        case RsmcDownLeftNum:
            return (mask << 7) & NotX7;
        case RsmcUpLeftNum:
            return (mask >> 9) & NotX7;
        default:
            return 0;
    }
}

#ifdef __cplusplus
}
#endif

#endif // RSMC_DIRECTIONS_H

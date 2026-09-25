// SPDX-License-Identifier: LGPL-2.1-or-later
// Copyright (c) 2026 01eksa

/**
 * @file rsmc.c
 * @brief Reversi stateless model in C - implementation.
 *
 */

#include "rsmc.h"
#include "private/directions.h"
#include "private/utils.h"

RsmcVersion rsmc_version(void)
{
    return rsmc_build_version();
}

RsmcBitMap rsmc_get_valid_moves(const RsmcBoard board, const RsmcPlayer player)
{
    if (!rsmc_player_is_valid(player))
        return 0;

    RsmcBitMap map = 0;
    const RsmcBitMask empty = ~(board.black | board.white);

    for (int i = 0; i < RsmcDirectionsCount; i++) {
        const RsmcDirectionNum dir = RsmcDirectionsNum[i];

        RsmcBitMask flood = 0;
        RsmcBitMask gen = board.by_player[player];
        const RsmcBitMask pro = board.by_player[rsmc_player_opposite(player)];

        for (int j = 0; j < 6; j++) {
            gen = rsmc_shift_in_direction(gen, dir) & pro;
            flood |= gen;
        }

        map |= rsmc_shift_in_direction(flood, dir) & empty;
    }

    return map;
}

RsmcBoard rsmc_get_start_position()
{
    return RsmcStartPosition;
}

RsmcBoard rsmc_apply_move(RsmcBoard board, const RsmcBitMask cell_bit_mask, const RsmcPlayer player)
{
    if (!rsmc_player_is_valid(player))
        return board;

    if (popcnt64(cell_bit_mask) != 1) {
        return board;
    }

    if (board.black & cell_bit_mask || board.white & cell_bit_mask) {
        return board;
    }

    const RsmcPlayer opponent = rsmc_player_opposite(player);
    uint64_t flips = 0;

    for (int i = 0; i < RsmcDirectionsCount; i++) {
        const RsmcDirectionNum dir = RsmcDirectionsNum[i];

        uint64_t flood = 0;
        uint64_t gen = cell_bit_mask;
        const uint64_t pro = board.by_player[rsmc_player_opposite(player)];

        for (int j = 0; j < 6; j++) {
            gen = rsmc_shift_in_direction(gen, dir) & pro;
            flood |= gen;
        }

        if (rsmc_shift_in_direction(flood, dir) & board.by_player[player])
            flips |= flood;
    }

    if (flips)
        flips |= cell_bit_mask;

    board.by_player[player] |= flips;
    board.by_player[opponent] &= ~flips;

    return board;
}

RsmcPlayersScore rsmc_get_players_score(const RsmcBoard board)
{
    const RsmcPlayersScore result = {
        .black_score = popcnt64(board.black),
        .white_score = popcnt64(board.white),
    };
    return result;
}

bool rsmc_is_move_valid(const RsmcBoard board, const RsmcBitMask cell_bit_mask,
                        const RsmcPlayer player)
{
    if (!rsmc_player_is_valid(player))
        return false;

    if (board.black & cell_bit_mask || board.white & cell_bit_mask) {
        return false;
    }

    return rsmc_get_valid_moves(board, player) & cell_bit_mask;
}

RsmcGameState rsmc_get_game_state(const RsmcBoard board)
{
    const bool game_finished = !(rsmc_get_valid_moves(board, RsmcPlayerBlack) ||
                                 rsmc_get_valid_moves(board, RsmcPlayerWhite));
    const RsmcPlayersScore score = rsmc_get_players_score(board);
    RsmcGameStatus game_status;

    if (!game_finished) {
        game_status = RsmcGameStatusContinue;
    } else if (score.black_score > score.white_score) {
        game_status = RsmcGameStatusBlackWin;
    } else if (score.white_score > score.black_score) {
        game_status = RsmcGameStatusWhiteWin;
    } else {
        game_status = RsmcGameStatusDraw;
    }

    const RsmcGameState result = {.score = score, .game_status = game_status};
    return result;
}

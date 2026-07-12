#ifndef RSMC_RSMC_H
#define RSMC_RSMC_H
#include "rsmc/types.h"

#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

void rsmc_set_start_position(RsmcBoard *board);

bool rsmc_apply_move(RsmcBoard *board, RsmcCoords coords, RsmcPlayer player);

uint8_t rsmc_get_player_score(const RsmcBoard *board, RsmcPlayer player);

RsmcPlayersScore rsmc_get_players_score(const RsmcBoard *board);

bool rsmc_is_move_valid(const RsmcBoard *board, RsmcCoords coords, RsmcPlayer player);

RsmcMoves rsmc_get_valid_moves(const RsmcBoard *board, RsmcPlayer player);

RsmcGameStatus rsmc_get_game_status(const RsmcBoard *board);

RsmcGameState rsmc_get_game_state(const RsmcBoard *board);

#ifdef __cplusplus
}
#endif

#endif // RSMC_RSMC_H

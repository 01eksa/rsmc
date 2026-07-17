/**
 * @file rsmc.h
 * @brief Reversi stateless model in C - implementation.
 *
 */

#include <string.h>

#include "directions.h"
#include "rsmc.h"
#include "utils.h"

static uint8_t count_flips_in_direction(const RsmcBoard *board, RsmcCoords coords_to_check,
                                        const RsmcCoords direction, const RsmcBoardCell player_cell,
                                        const RsmcBoardCell opponent_cell)
{
    uint8_t cells_to_flip = 0;

    while (true) {
        coords_to_check = rsmc_coords_add(coords_to_check, direction);

        if (!rsmc_coords_is_valid(coords_to_check)) {
            break;
        }

        const RsmcBoardCell cell_to_check = *rsmc_cell_at_const(board, coords_to_check);

        if (cell_to_check == opponent_cell) {
            cells_to_flip++;
            continue;
        }

        if (cell_to_check == player_cell && cells_to_flip) {
            return cells_to_flip;
        }

        break;
    }

    return 0;
}

void rsmc_set_start_position(RsmcBoard *board)
{
    memset(board->cells, RsmcBoardCellEmpty, sizeof(board->cells));
    board->cells[RsmcBoardSize / 2 - 1][RsmcBoardSize / 2 - 1] = RsmcBoardCellWhite;
    board->cells[RsmcBoardSize / 2][RsmcBoardSize / 2] = RsmcBoardCellWhite;
    board->cells[RsmcBoardSize / 2][RsmcBoardSize / 2 - 1] = RsmcBoardCellBlack;
    board->cells[RsmcBoardSize / 2 - 1][RsmcBoardSize / 2] = RsmcBoardCellBlack;
}

bool rsmc_apply_move(RsmcBoard *board, const RsmcCoords coords, const RsmcPlayer player)
{
    if (!rsmc_coords_is_valid(coords) || !rsmc_player_is_valid(player)) {
        return false;
    }

    RsmcBoardCell *asked_cell = rsmc_cell_at(board, coords);

    if (*asked_cell != RsmcBoardCellEmpty) {
        return false;
    }

    const RsmcBoardCell player_cell = rsmc_player_to_cell(player);
    const RsmcPlayer opponent = rsmc_player_opposite(player);
    const RsmcBoardCell opponent_cell = rsmc_player_to_cell(opponent);

    bool flipped = false;

    for (int i = 0; i < RsmcDirectionsCount; i++) {
        const RsmcCoords direction = RsmcDirections[i];
        RsmcCoords current_coords = coords;
        const uint8_t cells_to_flip =
            count_flips_in_direction(board, current_coords, direction, player_cell, opponent_cell);

        if (cells_to_flip > 0) {
            flipped = true;

            for (uint8_t j = 0; j < cells_to_flip; j++) {
                current_coords = rsmc_coords_add(current_coords, direction);
                RsmcBoardCell *cell_to_flip = rsmc_cell_at(board, current_coords);
                *cell_to_flip = player_cell;
            }
        }
    }

    if (flipped) {
        *asked_cell = player_cell;
    }

    return flipped;
}

RsmcPlayersScore rsmc_get_players_score(const RsmcBoard *board)
{
    uint8_t count[3] = {0};
    for (int y = 0; y < RsmcBoardSize; y++) {
        for (int x = 0; x < RsmcBoardSize; x++) {
            if (rsmc_board_cell_is_valid(board->cells[y][x])) {
                count[board->cells[y][x]]++;
            }
        }
    }

    const RsmcPlayersScore result = {
        .white_score = count[RsmcBoardCellWhite],
        .black_score = count[RsmcBoardCellBlack],
    };
    return result;
}

bool rsmc_is_move_valid(const RsmcBoard *board, const RsmcCoords coords, const RsmcPlayer player)
{
    if (!rsmc_coords_is_valid(coords) || !rsmc_player_is_valid(player)) {
        return false;
    }

    const RsmcBoardCell asked_cell = *rsmc_cell_at_const(board, coords);

    if (asked_cell != RsmcBoardCellEmpty) {
        return false;
    }

    const RsmcBoardCell player_cell = rsmc_player_to_cell(player);
    const RsmcPlayer opponent = rsmc_player_opposite(player);
    const RsmcBoardCell opponent_cell = rsmc_player_to_cell(opponent);

    for (int i = 0; i < RsmcDirectionsCount; i++) {
        const RsmcCoords direction = RsmcDirections[i];
        const uint8_t cells_to_flip =
            count_flips_in_direction(board, coords, direction, player_cell, opponent_cell);

        if (cells_to_flip > 0) {
            return true;
        }
    }

    return false;
}

RsmcMoves rsmc_get_valid_moves(const RsmcBoard *board, const RsmcPlayer player)
{
    RsmcMoves result = {0};

    if (!rsmc_player_is_valid(player)) {
        return result;
    }

    const int8_t board_size = RsmcBoardSize;
    for (int8_t y = 0; y < board_size; y++) {
        for (int8_t x = 0; x < board_size; x++) {
            const RsmcCoords coords = {x, y};
            if (rsmc_is_move_valid(board, coords, player) && result.count < RsmcMaxValidMoves) {
                result.coords[result.count++] = coords;
            }
        }
    }

    return result;
}

RsmcGameState rsmc_get_game_state(const RsmcBoard *board)
{
    bool game_finished = true;
    RsmcPlayersScore score = {0};

    const int8_t board_size = RsmcBoardSize;
    for (int8_t y = 0; y < board_size; y++) {
        for (int8_t x = 0; x < board_size; x++) {
            const RsmcCoords coords = {x, y};
            const RsmcBoardCell cell = *rsmc_cell_at_const(board, coords);

            switch (cell) {
                case RsmcBoardCellBlack:
                    score.black_score++;
                    break;
                case RsmcBoardCellWhite:
                    score.white_score++;
                    break;
                case RsmcBoardCellEmpty:
                    if (game_finished && (rsmc_is_move_valid(board, coords, RsmcPlayerWhite) ||
                                          rsmc_is_move_valid(board, coords, RsmcPlayerBlack))) {
                        game_finished = false;
                    }
                    break;
                default:;
            }
        }
    }

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

    const RsmcGameState result = {score, game_status};
    return result;
}

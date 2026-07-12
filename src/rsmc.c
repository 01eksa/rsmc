#include <memory.h>

#include "directions.h"
#include "rsmc.h"

void rsmc_set_start_position(RsmcBoard *board)
{
    memset(board->cells, RsmcBoardCellEmpty, sizeof(board->cells));
    board->cells[RsmcBoardSize / 2 - 1][RsmcBoardSize / 2 - 1] = RsmcBoardCellBlack;
    board->cells[RsmcBoardSize / 2][RsmcBoardSize / 2] = RsmcBoardCellBlack;
    board->cells[RsmcBoardSize / 2][RsmcBoardSize / 2 - 1] = RsmcBoardCellWhite;
    board->cells[RsmcBoardSize / 2 - 1][RsmcBoardSize / 2] = RsmcBoardCellWhite;
}

bool rsmc_apply_move(RsmcBoard *board, const RsmcCoords coords, const RsmcPlayer player)
{
    if (!rsmc_coords_is_valid(coords)) {
        return false;
    }

    RsmcBoardCell *asked_cell = rsmc_cell_at(board, coords);

    if (*asked_cell != RsmcBoardCellEmpty) {
        return false;
    }

    const RsmcBoardCell player_cell = rsmc_player_to_cell(player);
    const RsmcPlayer opposite_player = rsmc_player_opposite(player);
    const RsmcBoardCell opposite_player_cell = rsmc_player_to_cell(opposite_player);

    bool flipped = false;

    for (int i = 0; i < RsmcDirectionsCount; i++) {
        const RsmcCoords direction = RsmcDirections[i];
        RsmcCoords coords_to_check = coords;
        uint8_t cells_to_flip = 0;

        while (true) {
            coords_to_check = rsmc_coords_add(coords_to_check, direction);

            if (!rsmc_coords_is_valid(coords_to_check)) {
                break;
            }

            const RsmcBoardCell cell_to_check = *rsmc_cell_at(board, coords_to_check);

            if (cell_to_check == opposite_player_cell) {
                cells_to_flip++;
                continue;
            }

            if (cell_to_check == player_cell && cells_to_flip) {
                flipped = true;
                for (uint8_t j = 0; j < cells_to_flip; j++) {
                    const RsmcCoords coords_to_flip = rsmc_coords_sub(coords_to_check, direction);
                    RsmcBoardCell *cell_to_flip = rsmc_cell_at(board, coords_to_flip);
                    *cell_to_flip = player_cell;
                }
                break;
            }

            break;
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

uint8_t rsmc_get_player_score(const RsmcBoard *board, const RsmcPlayer player)
{
    const RsmcPlayersScore score = rsmc_get_players_score(board);
    return player == RsmcPlayerWhite ? score.white_score : score.black_score;
}

#include "rsmc.h"

RsmcPlayersScore rsmc_get_players_score(const RsmcBoard *board)
{
    uint8_t count[3] = {0};
    for (int y = 0; y < RsmcBoardSize; y++) {
        for (int x = 0; x < RsmcBoardSize; x++) {
            if (board->cells[y][x] < RsmcBoardCellCount) {
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

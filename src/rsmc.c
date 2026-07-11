#include "rsmc.h"

RsmcPlayersScore rsmc_get_players_score(const RsmcBoard *board)
{
    RsmcPlayersScore result = {0, 0};
    for (int y = 0; y < RsmcBoardSize; y++) {
        for (int x = 0; x < RsmcBoardSize; x++) {
            switch (board->cells[y][x]) {
                case RsmcBoardCellWhite:
                    result.white_score++;
                    break;
                case RsmcBoardCellBlack:
                    result.black_score++;
                    break;
                default:;
            }
        }
    }

    return result;
}

uint8_t rsmc_get_player_score(const RsmcBoard *board, const RsmcPlayer player)
{
    const RsmcPlayersScore score = rsmc_get_players_score(board);
    return player == RsmcPlayerWhite ? score.white_score : score.black_score;
}

#include <stdbool.h>
#include <string.h>

#include "rsmc.h"
#include "unity.h"

const uint8_t RsmcBoardArea = RsmcBoardSize * RsmcBoardSize;

void setUp(void)
{
}

void tearDown(void)
{
}

void moves_to_grid(const RsmcMoves *moves, uint8_t grid[RsmcBoardSize][RsmcBoardSize])
{
    memset(grid, 0, RsmcBoardArea);
    for (int i = 0; i < moves->count; i++) {
        grid[moves->coords[i].y][moves->coords[i].x] = 1;
    }
}

void test_rsmc_set_start_position_should_place_pieces_correct(void)
{
    RsmcBoard board;
    rsmc_set_start_position(&board);

    const RsmcBoardCell expected[RsmcBoardSize][RsmcBoardSize] = {
        {0},
        {0},
        {0},
        {0, 0, 0, RsmcBoardCellWhite, RsmcBoardCellBlack, 0, 0, 0},
        {0, 0, 0, RsmcBoardCellBlack, RsmcBoardCellWhite, 0, 0, 0},
        {0},
        {0},
        {0},
    };
    TEST_ASSERT_EQUAL_UINT8_ARRAY((const uint8_t *)board.cells, (const uint8_t *)expected,
                                  sizeof(board.cells));
}

void test_rsmc_apply_move_should_return_true_place_right_and_flip_opponent_pieces(void)
{
    const RsmcBoardCell cells[RsmcBoardSize][RsmcBoardSize] = {
        {0},
        {0},
        {0},
        {0, 0, 0, RsmcBoardCellWhite, RsmcBoardCellBlack, 0, 0, 0},
        {0, 0, 0, RsmcBoardCellBlack, RsmcBoardCellWhite, 0, 0, 0},
        {0},
        {0},
        {0},
    };
    RsmcBoard board;
    memcpy(board.cells, cells, sizeof(board.cells));
    const RsmcCoords coords = {2, 3};

    const bool result = rsmc_apply_move(&board, coords, RsmcPlayerBlack);
    const RsmcBoardCell expected[RsmcBoardSize][RsmcBoardSize] = {
        {0},
        {0},
        {0},
        {0, 0, RsmcBoardCellBlack, RsmcBoardCellBlack, RsmcBoardCellBlack, 0, 0, 0},
        {0, 0, 0, RsmcBoardCellBlack, RsmcBoardCellWhite, 0, 0, 0},
        {0},
        {0},
        {0},
    };

    TEST_ASSERT_EQUAL(result, true);
    TEST_ASSERT_EQUAL_UINT8_ARRAY((const uint8_t *)board.cells, (const uint8_t *)expected,
                                  sizeof(board.cells));
}

void test_rsmc_apply_move_should_return_false_and_change_nothing(void)
{
    RsmcBoard board;
    rsmc_set_start_position(&board);
    const RsmcCoords coords = {3, 3};
    const bool result = rsmc_apply_move(&board, coords, RsmcPlayerBlack);
    const RsmcBoardCell expected[RsmcBoardSize][RsmcBoardSize] = {
        {0},
        {0},
        {0},
        {0, 0, 0, RsmcBoardCellWhite, RsmcBoardCellBlack, 0, 0, 0},
        {0, 0, 0, RsmcBoardCellBlack, RsmcBoardCellWhite, 0, 0, 0},
        {0},
        {0},
        {0},
    };

    TEST_ASSERT_EQUAL(result, false);
    TEST_ASSERT_EQUAL_UINT8_ARRAY((const uint8_t *)board.cells, (const uint8_t *)expected,
                                  sizeof(board.cells));
}

void test_rsmc_apply_move_should_return_false_for_invalid_player_and_change_nothing(void)
{
    RsmcBoard board;
    rsmc_set_start_position(&board);
    const RsmcCoords coords = {2, 3};
    const bool result = rsmc_apply_move(&board, coords, 100);
    const RsmcBoardCell expected[RsmcBoardSize][RsmcBoardSize] = {
        {0},
        {0},
        {0},
        {0, 0, 0, RsmcBoardCellWhite, RsmcBoardCellBlack, 0, 0, 0},
        {0, 0, 0, RsmcBoardCellBlack, RsmcBoardCellWhite, 0, 0, 0},
        {0},
        {0},
        {0},
    };

    TEST_ASSERT_EQUAL(result, false);
    TEST_ASSERT_EQUAL_UINT8_ARRAY((const uint8_t *)board.cells, (const uint8_t *)expected,
                                  sizeof(board.cells));
}

void test_rsmc_apply_move_should_return_false_for_invalid_coords_and_change_nothing(void)
{
    RsmcBoard board;
    rsmc_set_start_position(&board);
    const RsmcCoords coords_1 = {-1, 0};
    const RsmcCoords coords_2 = {0, -1};
    const RsmcCoords coords_3 = {8, 0};
    const RsmcCoords coords_4 = {0, 8};

    const RsmcBoardCell expected[RsmcBoardSize][RsmcBoardSize] = {
        {0},
        {0},
        {0},
        {0, 0, 0, RsmcBoardCellWhite, RsmcBoardCellBlack, 0, 0, 0},
        {0, 0, 0, RsmcBoardCellBlack, RsmcBoardCellWhite, 0, 0, 0},
        {0},
        {0},
        {0},
    };

    const bool result_1 = rsmc_apply_move(&board, coords_1, RsmcPlayerBlack);
    TEST_ASSERT_EQUAL(result_1, false);
    TEST_ASSERT_EQUAL_UINT8_ARRAY((const uint8_t *)board.cells, (const uint8_t *)expected,
                                  sizeof(board.cells));

    const bool result_2 = rsmc_apply_move(&board, coords_2, RsmcPlayerBlack);
    TEST_ASSERT_EQUAL(result_2, false);
    TEST_ASSERT_EQUAL_UINT8_ARRAY((const uint8_t *)board.cells, (const uint8_t *)expected,
                                  sizeof(board.cells));

    const bool result_3 = rsmc_apply_move(&board, coords_3, RsmcPlayerBlack);
    TEST_ASSERT_EQUAL(result_3, false);
    TEST_ASSERT_EQUAL_UINT8_ARRAY((const uint8_t *)board.cells, (const uint8_t *)expected,
                                  sizeof(board.cells));

    const bool result_4 = rsmc_apply_move(&board, coords_4, RsmcPlayerBlack);
    TEST_ASSERT_EQUAL(result_4, false);
    TEST_ASSERT_EQUAL_UINT8_ARRAY((const uint8_t *)board.cells, (const uint8_t *)expected,
                                  sizeof(board.cells));


}

void test_rsmc_get_players_score_should_return_right_score(void)
{
    const RsmcBoardCell cells[RsmcBoardSize][RsmcBoardSize] = {
        {0},
        {0},
        {0},
        {0, 0, RsmcBoardCellBlack, RsmcBoardCellBlack, RsmcBoardCellBlack, 0, 0, 0},
        {0, 0, 0, RsmcBoardCellBlack, RsmcBoardCellWhite, 0, 0, 0},
        {0},
        {0},
        {0},
    };
    RsmcBoard board;
    memcpy(board.cells, cells, sizeof(board.cells));
    const RsmcPlayersScore expected = {1, 4};

    const RsmcPlayersScore actual = rsmc_get_players_score(&board);

    TEST_ASSERT_EQUAL(actual.white_score, expected.white_score);
    TEST_ASSERT_EQUAL(actual.black_score, expected.black_score);
}

void test_rsmc_is_move_valid_should_return_true(void)
{
    const RsmcBoardCell cells[RsmcBoardSize][RsmcBoardSize] = {
        {0},
        {0},
        {0},
        {0, 0, 0, RsmcBoardCellWhite, RsmcBoardCellBlack, 0, 0, 0},
        {0, 0, 0, RsmcBoardCellBlack, RsmcBoardCellWhite, 0, 0, 0},
        {0},
        {0},
        {0},
    };
    RsmcBoard board;
    memcpy(board.cells, cells, sizeof(board.cells));
    const RsmcCoords coords = {2, 3};

    const bool result = rsmc_is_move_valid(&board, coords, RsmcPlayerBlack);

    TEST_ASSERT_EQUAL(result, true);
}

void test_rsmc_is_move_valid_should_return_false(void)
{
    const RsmcBoardCell cells[RsmcBoardSize][RsmcBoardSize] = {
        {0},
        {0},
        {0},
        {0, 0, 0, RsmcBoardCellWhite, RsmcBoardCellBlack, 0, 0, 0},
        {0, 0, 0, RsmcBoardCellBlack, RsmcBoardCellWhite, 0, 0, 0},
        {0},
        {0},
        {0},
    };
    RsmcBoard board;
    memcpy(board.cells, cells, sizeof(board.cells));
    const RsmcCoords coords = {3, 3};

    const bool result = rsmc_is_move_valid(&board, coords, RsmcPlayerBlack);

    TEST_ASSERT_EQUAL(result, false);
}

void test_rsmc_is_move_valid_should_return_false_for_invalid_player(void)
{
    const RsmcBoardCell cells[RsmcBoardSize][RsmcBoardSize] = {
        {0},
        {0},
        {0},
        {0, 0, 0, RsmcBoardCellWhite, RsmcBoardCellBlack, 0, 0, 0},
        {0, 0, 0, RsmcBoardCellBlack, RsmcBoardCellWhite, 0, 0, 0},
        {0},
        {0},
        {0},
    };
    RsmcBoard board;
    memcpy(board.cells, cells, sizeof(board.cells));
    const RsmcCoords coords = {2, 3};

    const bool result = rsmc_is_move_valid(&board, coords, 100);

    TEST_ASSERT_EQUAL(result, false);
}

void test_rsmc_is_move_valid_should_return_false_for_invalid_coords(void)
{
    const RsmcBoardCell cells[RsmcBoardSize][RsmcBoardSize] = {
        {0},
        {0},
        {0},
        {0, 0, 0, RsmcBoardCellWhite, RsmcBoardCellBlack, 0, 0, 0},
        {0, 0, 0, RsmcBoardCellBlack, RsmcBoardCellWhite, 0, 0, 0},
        {0},
        {0},
        {0},
    };
    RsmcBoard board;
    memcpy(board.cells, cells, sizeof(board.cells));
    const RsmcCoords coords_1 = {-1, 0};
    const RsmcCoords coords_2 = {0, -1};
    const RsmcCoords coords_3 = {8, 0};
    const RsmcCoords coords_4 = {0, 8};

    const bool result_1 = rsmc_is_move_valid(&board, coords_1, RsmcPlayerBlack);
    const bool result_2 = rsmc_is_move_valid(&board, coords_2, RsmcPlayerBlack);
    const bool result_3 = rsmc_is_move_valid(&board, coords_3, RsmcPlayerBlack);
    const bool result_4 = rsmc_is_move_valid(&board, coords_4, RsmcPlayerBlack);

    TEST_ASSERT_EQUAL(result_1, false);
    TEST_ASSERT_EQUAL(result_2, false);
    TEST_ASSERT_EQUAL(result_3, false);
    TEST_ASSERT_EQUAL(result_4, false);
}

void test_rsmc_get_valid_moves_should_return_4_valid_moves(void)
{
    const RsmcBoardCell cells[RsmcBoardSize][RsmcBoardSize] = {
        {0},
        {0},
        {0},
        {0, 0, 0, RsmcBoardCellWhite, RsmcBoardCellBlack, 0, 0, 0},
        {0, 0, 0, RsmcBoardCellBlack, RsmcBoardCellWhite, 0, 0, 0},
        {0},
        {0},
        {0},
    };
    RsmcBoard board;
    memcpy(board.cells, cells, sizeof(board.cells));

    uint8_t expected_grid[RsmcBoardSize][RsmcBoardSize];
    const RsmcMoves expected_moves = {
        4,
        {{2, 3}, {3, 2}, {4, 5}, {5, 4}},
    };
    moves_to_grid(&expected_moves, expected_grid);

    uint8_t actual_grid[RsmcBoardSize][RsmcBoardSize];
    const RsmcMoves actual_moves = rsmc_get_valid_moves(&board, RsmcPlayerBlack);
    moves_to_grid(&actual_moves, actual_grid);

    TEST_ASSERT_EQUAL(expected_moves.count, actual_moves.count);
    TEST_ASSERT_EQUAL_UINT8_ARRAY((const uint8_t *)expected_grid, (const uint8_t *)actual_grid,
                                  RsmcBoardArea);
}

void test_rsmc_get_valid_moves_should_return_0_valid_moves(void)
{
    const RsmcBoardCell cells[RsmcBoardSize][RsmcBoardSize] = {
        {0, 0, 0, 0, RsmcBoardCellWhite, RsmcBoardCellWhite, RsmcBoardCellWhite,
         RsmcBoardCellWhite},
        {0, 0, 0, 0, 0, 0, 0, RsmcBoardCellWhite},
        {0, 0, 0, 0, 0, 0, 0, RsmcBoardCellWhite},
        {RsmcBoardCellBlack, RsmcBoardCellBlack, RsmcBoardCellBlack, RsmcBoardCellBlack,
         RsmcBoardCellBlack, RsmcBoardCellBlack, RsmcBoardCellWhite, RsmcBoardCellWhite},
        {0, 0, 0, RsmcBoardCellBlack, RsmcBoardCellBlack, 0, 0, 0},
        {0},
        {0},
        {0},
    };
    RsmcBoard board;
    memcpy(board.cells, cells, sizeof(board.cells));

    uint8_t expected_grid[RsmcBoardSize][RsmcBoardSize];
    const RsmcMoves expected_moves = {
        0,
        {},
    };
    moves_to_grid(&expected_moves, expected_grid);

    uint8_t actual_grid[RsmcBoardSize][RsmcBoardSize];
    const RsmcMoves actual_moves = rsmc_get_valid_moves(&board, RsmcPlayerBlack);
    moves_to_grid(&actual_moves, actual_grid);

    TEST_ASSERT_EQUAL(expected_moves.count, actual_moves.count);
    TEST_ASSERT_EQUAL_UINT8_ARRAY((const uint8_t *)expected_grid, (const uint8_t *)actual_grid,
                                  RsmcBoardArea);
}

void test_rsmc_get_valid_moves_should_return_0_for_invalid_player(void)
{
    const RsmcBoardCell cells[RsmcBoardSize][RsmcBoardSize] = {
        {0},
        {0},
        {0},
        {0, 0, 0, RsmcBoardCellWhite, RsmcBoardCellBlack, 0, 0, 0},
        {0, 0, 0, RsmcBoardCellBlack, RsmcBoardCellWhite, 0, 0, 0},
        {0},
        {0},
        {0},
    };
    RsmcBoard board;
    memcpy(board.cells, cells, sizeof(board.cells));

    uint8_t expected_grid[RsmcBoardSize][RsmcBoardSize];
    const RsmcMoves expected_moves = {0};
    moves_to_grid(&expected_moves, expected_grid);

    uint8_t actual_grid[RsmcBoardSize][RsmcBoardSize];
    const RsmcMoves actual_moves = rsmc_get_valid_moves(&board, 100);
    moves_to_grid(&actual_moves, actual_grid);

    TEST_ASSERT_EQUAL(expected_moves.count, actual_moves.count);
    TEST_ASSERT_EQUAL_UINT8_ARRAY((const uint8_t *)expected_grid, (const uint8_t *)actual_grid,
                                  RsmcBoardArea);
}

void test_rsmc_get_game_state_should_return_continue_and_valid_score(void)
{
    const RsmcBoardCell cells[RsmcBoardSize][RsmcBoardSize] = {
        {0},
        {0},
        {0},
        {RsmcBoardCellBlack, RsmcBoardCellBlack, RsmcBoardCellBlack, RsmcBoardCellBlack,
         RsmcBoardCellWhite, RsmcBoardCellWhite, RsmcBoardCellWhite, RsmcBoardCellWhite},
        {RsmcBoardCellBlack, RsmcBoardCellBlack, RsmcBoardCellBlack, RsmcBoardCellBlack,
         RsmcBoardCellWhite, RsmcBoardCellWhite, RsmcBoardCellWhite, RsmcBoardCellWhite},
        {0},
        {0},
        {0},
    };
    RsmcBoard board;
    memcpy(board.cells, cells, sizeof(board.cells));

    const RsmcGameStatus expected_game_status = RsmcGameStatusContinue;
    const RsmcPlayersScore expected_score = {8, 8};

    const RsmcGameState actual_state = rsmc_get_game_state(&board);

    TEST_ASSERT_EQUAL(expected_game_status, actual_state.game_status);
    TEST_ASSERT_EQUAL(expected_score.white_score, actual_state.score.white_score);
    TEST_ASSERT_EQUAL(expected_score.black_score, actual_state.score.black_score);
}

void test_rsmc_get_game_state_should_return_draw_and_valid_score(void)
{
    const RsmcBoardCell cells[RsmcBoardSize][RsmcBoardSize] = {
        {0, 0, 0, 0, RsmcBoardCellWhite, RsmcBoardCellWhite, RsmcBoardCellWhite,
         RsmcBoardCellWhite},
        {0, 0, 0, 0, 0, 0, 0, RsmcBoardCellWhite},
        {0, 0, 0, 0, 0, 0, 0, RsmcBoardCellWhite},
        {RsmcBoardCellBlack, RsmcBoardCellBlack, RsmcBoardCellBlack, RsmcBoardCellBlack,
         RsmcBoardCellBlack, RsmcBoardCellBlack, RsmcBoardCellWhite, RsmcBoardCellWhite},
        {0, 0, 0, RsmcBoardCellBlack, RsmcBoardCellBlack, 0, 0, 0},
        {0},
        {0},
        {0},
    };
    RsmcBoard board;
    memcpy(board.cells, cells, sizeof(board.cells));

    const RsmcGameStatus expected_game_status = RsmcGameStatusDraw;
    const RsmcPlayersScore expected_score = {8, 8};

    const RsmcGameState actual_state = rsmc_get_game_state(&board);

    TEST_ASSERT_EQUAL(expected_game_status, actual_state.game_status);
    TEST_ASSERT_EQUAL(expected_score.white_score, actual_state.score.white_score);
    TEST_ASSERT_EQUAL(expected_score.black_score, actual_state.score.black_score);
}

void test_rsmc_get_game_state_should_return_white_win_and_valid_score(void)
{
    const RsmcBoardCell cells[RsmcBoardSize][RsmcBoardSize] = {
        {0},
        {0},
        {0},
        {0, 0, 0, RsmcBoardCellWhite, RsmcBoardCellWhite, 0, 0, 0},
        {0, 0, 0, RsmcBoardCellWhite, RsmcBoardCellWhite, 0, 0, 0},
        {0},
        {0},
        {0},
    };
    RsmcBoard board;
    memcpy(board.cells, cells, sizeof(board.cells));

    const RsmcGameStatus expected_game_status = RsmcGameStatusWhiteWin;
    const RsmcPlayersScore expected_score = {4, 0};

    const RsmcGameState actual_state = rsmc_get_game_state(&board);

    TEST_ASSERT_EQUAL(expected_game_status, actual_state.game_status);
    TEST_ASSERT_EQUAL(expected_score.white_score, actual_state.score.white_score);
    TEST_ASSERT_EQUAL(expected_score.black_score, actual_state.score.black_score);
}

void test_rsmc_get_game_state_should_return_black_win_and_valid_score(void)
{
    const RsmcBoardCell cells[RsmcBoardSize][RsmcBoardSize] = {
        {
            RsmcBoardCellBlack,
            RsmcBoardCellBlack,
            RsmcBoardCellBlack,
            RsmcBoardCellWhite,
            RsmcBoardCellWhite,
            RsmcBoardCellWhite,
            RsmcBoardCellWhite,
            RsmcBoardCellWhite,
        },
        {
            RsmcBoardCellBlack,
            RsmcBoardCellBlack,
            RsmcBoardCellBlack,
            RsmcBoardCellBlack,
            RsmcBoardCellBlack,
            RsmcBoardCellBlack,
            RsmcBoardCellBlack,
            RsmcBoardCellBlack,
        },
        {
            RsmcBoardCellBlack,
            RsmcBoardCellBlack,
            RsmcBoardCellBlack,
            RsmcBoardCellBlack,
            RsmcBoardCellBlack,
            RsmcBoardCellBlack,
            RsmcBoardCellBlack,
            RsmcBoardCellBlack,
        },
        {
            RsmcBoardCellBlack,
            RsmcBoardCellBlack,
            RsmcBoardCellBlack,
            RsmcBoardCellBlack,
            RsmcBoardCellBlack,
            RsmcBoardCellBlack,
            RsmcBoardCellBlack,
            RsmcBoardCellBlack,
        },
        {
            RsmcBoardCellBlack,
            RsmcBoardCellBlack,
            RsmcBoardCellBlack,
            RsmcBoardCellBlack,
            RsmcBoardCellBlack,
            RsmcBoardCellBlack,
            RsmcBoardCellBlack,
            RsmcBoardCellBlack,
        },
        {
            RsmcBoardCellBlack,
            RsmcBoardCellWhite,
            RsmcBoardCellWhite,
            RsmcBoardCellWhite,
            RsmcBoardCellWhite,
            RsmcBoardCellWhite,
            RsmcBoardCellWhite,
            RsmcBoardCellWhite,
        },
        {
            RsmcBoardCellBlack,
            RsmcBoardCellWhite,
            RsmcBoardCellWhite,
            RsmcBoardCellWhite,
            RsmcBoardCellWhite,
            RsmcBoardCellWhite,
            RsmcBoardCellWhite,
            RsmcBoardCellWhite,
        },
        {
            RsmcBoardCellBlack,
            RsmcBoardCellWhite,
            RsmcBoardCellWhite,
            RsmcBoardCellWhite,
            RsmcBoardCellWhite,
            RsmcBoardCellWhite,
            RsmcBoardCellWhite,
            RsmcBoardCellWhite,
        },
    };
    RsmcBoard board;
    memcpy(board.cells, cells, sizeof(board.cells));

    const RsmcGameStatus expected_game_status = RsmcGameStatusBlackWin;
    const RsmcPlayersScore expected_score = {26, 38};

    const RsmcGameState actual_state = rsmc_get_game_state(&board);

    TEST_ASSERT_EQUAL(expected_game_status, actual_state.game_status);
    TEST_ASSERT_EQUAL(expected_score.white_score, actual_state.score.white_score);
    TEST_ASSERT_EQUAL(expected_score.black_score, actual_state.score.black_score);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_rsmc_set_start_position_should_place_pieces_correct);

    RUN_TEST(test_rsmc_apply_move_should_return_true_place_right_and_flip_opponent_pieces);
    RUN_TEST(test_rsmc_apply_move_should_return_false_and_change_nothing);
    RUN_TEST(test_rsmc_apply_move_should_return_false_for_invalid_player_and_change_nothing);
    RUN_TEST(test_rsmc_apply_move_should_return_false_for_invalid_coords_and_change_nothing);

    RUN_TEST(test_rsmc_get_players_score_should_return_right_score);

    RUN_TEST(test_rsmc_is_move_valid_should_return_true);
    RUN_TEST(test_rsmc_is_move_valid_should_return_false);
    RUN_TEST(test_rsmc_is_move_valid_should_return_false_for_invalid_player);
    RUN_TEST(test_rsmc_is_move_valid_should_return_false_for_invalid_coords);

    RUN_TEST(test_rsmc_get_valid_moves_should_return_4_valid_moves);
    RUN_TEST(test_rsmc_get_valid_moves_should_return_0_valid_moves);
    RUN_TEST(test_rsmc_get_valid_moves_should_return_0_for_invalid_player);

    RUN_TEST(test_rsmc_get_game_state_should_return_continue_and_valid_score);
    RUN_TEST(test_rsmc_get_game_state_should_return_draw_and_valid_score);
    RUN_TEST(test_rsmc_get_game_state_should_return_white_win_and_valid_score);
    RUN_TEST(test_rsmc_get_game_state_should_return_black_win_and_valid_score);

    return UNITY_END();
}
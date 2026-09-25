#include <stdbool.h>
#include <string.h>

#include "rsmc.h"
#include "unity.h"

void setUp(void)
{
}

void tearDown(void)
{
}

void test_rsmc_set_start_position_should_place_pieces_correct(void)
{
    const RsmcBoard board = rsmc_get_start_position();

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

    const RsmcClassicBoard actual = rsmc_classic_board_from_board(board);

    TEST_ASSERT_EQUAL_UINT8_ARRAY((const uint8_t *)actual.cells, (const uint8_t *)expected,
                                  sizeof(actual.cells));
}

void test_rsmc_apply_move_should_place_right_and_flip_opponent_pieces(void)
{
    const RsmcBoard board = rsmc_get_start_position();
    const RsmcCoords coords = {.x = 2, .y = 3};
    const RsmcBitMask cell_mask = rsmc_index_to_bitmask(rsmc_coords_to_index(coords));

    const RsmcBoard new_board = rsmc_apply_move(board, cell_mask, RsmcPlayerBlack);
    TEST_ASSERT(board.black != new_board.black && board.white != new_board.white);

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
    const RsmcClassicBoard actual = rsmc_classic_board_from_board(new_board);

    TEST_ASSERT_EQUAL_UINT8_ARRAY((const uint8_t *)actual.cells, (const uint8_t *)expected,
                                  sizeof(actual.cells));
}

void test_rsmc_apply_move_should_change_nothing_for_invalid_move(void)
{
    const RsmcBoard board = rsmc_get_start_position();
    const RsmcCoords coords = {.x = 3, .y = 3};
    const RsmcBitMask cell_mask = rsmc_index_to_bitmask(rsmc_coords_to_index(coords));

    const RsmcBoard new_board = rsmc_apply_move(board, cell_mask, RsmcPlayerBlack);
    TEST_ASSERT_EQUAL(board.black, new_board.black);
    TEST_ASSERT_EQUAL(board.white, new_board.white);
}

void test_rsmc_apply_move_should_change_nothing_for_invalid_player(void)
{
    const RsmcBoard board = rsmc_get_start_position();
    const RsmcCoords coords = {.x = 2, .y = 3};
    const RsmcBitMask cell_mask = rsmc_index_to_bitmask(rsmc_coords_to_index(coords));

    const RsmcBoard new_board = rsmc_apply_move(board, cell_mask, 100);
    TEST_ASSERT_EQUAL(board.black, new_board.black);
    TEST_ASSERT_EQUAL(board.white, new_board.white);
}

void test_rsmc_apply_move_should_change_nothing_for_invalid_masks(void)
{
    const RsmcBoard board = rsmc_get_start_position();
    const RsmcBitMask cell_mask_1 = 0b0101;
    const RsmcBitMask cell_mask_2 = 0b10000000001;
    const RsmcBitMask cell_mask_3 = 0xF3FFFFFF;

    const RsmcBoard result_1 = rsmc_apply_move(board, cell_mask_1, RsmcPlayerBlack);
    TEST_ASSERT_EQUAL(board.black, result_1.black);
    TEST_ASSERT_EQUAL(board.white, result_1.white);

    const RsmcBoard result_2 = rsmc_apply_move(board, cell_mask_2, RsmcPlayerBlack);
    TEST_ASSERT_EQUAL(board.black, result_2.black);
    TEST_ASSERT_EQUAL(board.white, result_2.white);

    const RsmcBoard result_3 = rsmc_apply_move(board, cell_mask_3, RsmcPlayerBlack);
    TEST_ASSERT_EQUAL(board.black, result_3.black);
    TEST_ASSERT_EQUAL(board.white, result_3.white);
}

void test_rsmc_get_players_score_should_return_right_score(void)
{
    const RsmcClassicBoard classic_board = {{
        {0},
        {0},
        {0},
        {0, 0, RsmcBoardCellBlack, RsmcBoardCellBlack, RsmcBoardCellBlack, 0, 0, 0},
        {0, 0, 0, RsmcBoardCellBlack, RsmcBoardCellWhite, 0, 0, 0},
        {0},
        {0},
        {0},
    }};
    const RsmcBoard board = rsmc_board_from_classic_board(classic_board);
    const RsmcPlayersScore expected = {.black_score = 4, .white_score = 1};
    const RsmcPlayersScore actual = rsmc_get_players_score(board);

    TEST_ASSERT_EQUAL(expected.white_score, actual.white_score);
    TEST_ASSERT_EQUAL(expected.black_score, actual.black_score);
}

void test_rsmc_is_move_valid_should_return_true(void)
{
    const RsmcBoard board = rsmc_get_start_position();
    const RsmcCoords coords = {.x = 2, .y = 3};
    const RsmcBitMask cell_mask = rsmc_index_to_bitmask(rsmc_coords_to_index(coords));

    const bool result = rsmc_is_move_valid(board, cell_mask, RsmcPlayerBlack);

    TEST_ASSERT_EQUAL(result, true);
}

void test_rsmc_is_move_valid_should_return_false(void)
{
    RsmcBoard board = rsmc_get_start_position();
    const RsmcCoords coords = {.x = 3, .y = 3};
    const RsmcBitMask cell_mask = rsmc_index_to_bitmask(rsmc_coords_to_index(coords));

    const bool result = rsmc_is_move_valid(board, cell_mask, RsmcPlayerBlack);

    TEST_ASSERT_EQUAL(result, false);
}

void test_rsmc_is_move_valid_should_return_false_for_invalid_player(void)
{
    RsmcBoard board = rsmc_get_start_position();
    const RsmcCoords coords = {.x = 2, .y = 3};
    const RsmcBitMask cell_mask = rsmc_index_to_bitmask(rsmc_coords_to_index(coords));

    const bool result = rsmc_is_move_valid(board, cell_mask, 100);

    TEST_ASSERT_EQUAL(result, false);
}

void test_rsmc_is_move_valid_should_return_false_for_invalid_coords(void)
{
    RsmcBoard board = rsmc_get_start_position();
    const RsmcBitMask cell_mask_1 = 0b0101;
    const RsmcBitMask cell_mask_2 = 0b10000000001;
    const RsmcBitMask cell_mask_3 = 0xF3FFFFFF;

    const bool result_1 = rsmc_is_move_valid(board, cell_mask_1, RsmcPlayerBlack);
    const bool result_2 = rsmc_is_move_valid(board, cell_mask_2, RsmcPlayerBlack);
    const bool result_3 = rsmc_is_move_valid(board, cell_mask_3, RsmcPlayerBlack);

    TEST_ASSERT_EQUAL(result_1, false);
    TEST_ASSERT_EQUAL(result_2, false);
    TEST_ASSERT_EQUAL(result_3, false);
}

void test_rsmc_get_valid_moves_should_return_4_valid_moves(void)
{
    const RsmcBoard board = rsmc_get_start_position();

    RsmcBitMap expected = 0;
    const int expected_moves_count = 4;
    const RsmcCoords expected_moves[] = {
        {.x = 2, .y = 3}, {.x = 3, .y = 2}, {.x = 4, .y = 5}, {.x = 5, .y = 4}};
    for (int i = 0; i < expected_moves_count; ++i) {
        expected |= rsmc_index_to_bitmask(rsmc_coords_to_index(expected_moves[i]));
    }

    const RsmcBitMap actual = rsmc_get_valid_moves(board, RsmcPlayerBlack);

    TEST_ASSERT_EQUAL(expected, actual);
}

void test_rsmc_get_valid_moves_should_return_0_valid_moves(void)
{
    const RsmcClassicBoard classic_board = {{
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
    }};

    const RsmcBoard board = rsmc_board_from_classic_board(classic_board);
    const RsmcBitMap expected = 0;
    const RsmcBitMap actual = rsmc_get_valid_moves(board, RsmcPlayerBlack);

    TEST_ASSERT_EQUAL(expected, actual);
}

void test_rsmc_get_valid_moves_should_return_0_for_invalid_player(void)
{
    RsmcBoard board = rsmc_get_start_position();

    const RsmcBitMap expected = 0;
    const RsmcBitMap actual = rsmc_get_valid_moves(board, 100);

    TEST_ASSERT_EQUAL(expected, actual);
}

void test_rsmc_get_game_state_should_return_continue_and_valid_score(void)
{
    const RsmcClassicBoard classic_board = {{
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
    }
    };
    const RsmcBoard board = rsmc_board_from_classic_board(classic_board);

    const RsmcGameStatus expected_game_status = RsmcGameStatusContinue;
    const RsmcPlayersScore expected_score = {.black_score = 8, .white_score = 8};

    const RsmcGameState actual_state = rsmc_get_game_state(board);

    TEST_ASSERT_EQUAL(expected_game_status, actual_state.game_status);
    TEST_ASSERT_EQUAL(expected_score.white_score, actual_state.score.white_score);
    TEST_ASSERT_EQUAL(expected_score.black_score, actual_state.score.black_score);
}

void test_rsmc_get_game_state_should_return_draw_and_valid_score(void)
{
   const RsmcClassicBoard classic_board = {{
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
    }
   };
    const RsmcBoard board = rsmc_board_from_classic_board(classic_board);

    const RsmcGameStatus expected_game_status = RsmcGameStatusDraw;
    const RsmcPlayersScore expected_score = {.black_score = 8, .white_score = 8};

    const RsmcGameState actual_state = rsmc_get_game_state(board);

    TEST_ASSERT_EQUAL(expected_game_status, actual_state.game_status);
    TEST_ASSERT_EQUAL(expected_score.white_score, actual_state.score.white_score);
    TEST_ASSERT_EQUAL(expected_score.black_score, actual_state.score.black_score);
}

void test_rsmc_get_game_state_should_return_white_win_and_valid_score(void)
{
    const RsmcClassicBoard classic_board = {{
        {0},
        {0},
        {0},
        {0, 0, 0, RsmcBoardCellWhite, RsmcBoardCellWhite, 0, 0, 0},
        {0, 0, 0, RsmcBoardCellWhite, RsmcBoardCellWhite, 0, 0, 0},
        {0},
        {0},
        {0},
    }
    };
    const RsmcBoard board = rsmc_board_from_classic_board(classic_board);

    const RsmcGameStatus expected_game_status = RsmcGameStatusWhiteWin;
    const RsmcPlayersScore expected_score = {.black_score = 0, .white_score = 4};

    const RsmcGameState actual_state = rsmc_get_game_state(board);

    TEST_ASSERT_EQUAL(expected_game_status, actual_state.game_status);
    TEST_ASSERT_EQUAL(expected_score.white_score, actual_state.score.white_score);
    TEST_ASSERT_EQUAL(expected_score.black_score, actual_state.score.black_score);
}

void test_rsmc_get_game_state_should_return_black_win_and_valid_score(void)
{
    const RsmcClassicBoard classic_board = {{
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
    }
    };
    const RsmcBoard board = rsmc_board_from_classic_board(classic_board);

    const RsmcGameStatus expected_game_status = RsmcGameStatusBlackWin;
    const RsmcPlayersScore expected_score = {.black_score = 38, .white_score = 26};

    const RsmcGameState actual_state = rsmc_get_game_state(board);

    TEST_ASSERT_EQUAL(expected_game_status, actual_state.game_status);
    TEST_ASSERT_EQUAL(expected_score.white_score, actual_state.score.white_score);
    TEST_ASSERT_EQUAL(expected_score.black_score, actual_state.score.black_score);
}

int main(void)
{
    UNITY_BEGIN();

    RUN_TEST(test_rsmc_set_start_position_should_place_pieces_correct);

    RUN_TEST(test_rsmc_apply_move_should_place_right_and_flip_opponent_pieces);
    RUN_TEST(test_rsmc_apply_move_should_change_nothing_for_invalid_move);
    RUN_TEST(test_rsmc_apply_move_should_change_nothing_for_invalid_player);
    RUN_TEST(test_rsmc_apply_move_should_change_nothing_for_invalid_masks);

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
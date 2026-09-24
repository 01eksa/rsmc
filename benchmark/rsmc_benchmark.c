#include "rsmc.h"
#include "utils.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define RSMC_BENCHMARK_MIN_GAMES 100000

static const char Help[] =
    "Usage: rsmcbench [OPTIONS] [GAMES]\n\n"
    "Simple benchmark for rsmc library. Simulates random Reversi games\n"
    "and measures execution time, games per second, and moves per second.\n\n"
    "Arguments:\n"
    "  [GAMES]                 Number of games to simulate [default: 100000, min: 100000]\n\n"
    "Options:\n"
    "  -h, --help              Print help information\n\n"
    "Examples:\n"
    "  $ rsmcbench\n"
    "  $ rsmcbench 500000\n";
;

typedef struct {
    unsigned long games;
    uint64_t moves;
    double seconds;
} Performance;

static int simulate_game(void)
{
    int moves_played = 0;
    RsmcBoard board;
    rsmc_set_start_position(&board);
    RsmcPlayer player = RsmcPlayerBlack;

    while (true) {
        const RsmcMoves moves = rsmc_get_valid_moves(&board, player);
        if (!moves.count) {
            const RsmcGameStatus status = rsmc_get_game_state(&board).game_status;
            if (status != RsmcGameStatusContinue)
                return moves_played;
            player = !player;
            continue;
        }

        const RsmcCoords move = moves.coords[xorshift32() % moves.count];
        rsmc_apply_move(&board, move, player);

        player = !player;
        moves_played++;
    }
}

static Performance measure_games(const unsigned long games)
{
    volatile uint64_t moves = 0;
    const double start = sec_now();

    for (unsigned long i = 0; i < games; i++) {
        moves += simulate_game();
    }

    const double end = sec_now();
    const Performance result = {.games = games, .moves = moves, .seconds = end - start};
    return result;
}

int main(int argc, char **argv)
{
    unsigned long games = RSMC_BENCHMARK_MIN_GAMES;
    if (argc == 2) {
        if (strcmp(argv[1], "--help") == 0 || strcmp(argv[1], "-h") == 0) {
            printf(Help);
            return 0;
        }
        games = strtoul(argv[1], NULL, 10);
        if (games < RSMC_BENCHMARK_MIN_GAMES) {
            printf("Unacceptable value. Run rsmcbench --help for more information.\n");
            return 1;
        }
    }

    printf("Benchmark for rsmc v0.1.0\n");
    printf("Games: %lu\n", games);
    const Performance measurements = measure_games(games);
    printf("Time:  %fs\n", measurements.seconds);

    const double gps = (double)measurements.games / measurements.seconds;
    const double mps = (double)measurements.moves / measurements.seconds;

    printf("games per second: %.0f\n", gps);
    printf("moves per second: %.0f\n", mps);

    return 0;
}

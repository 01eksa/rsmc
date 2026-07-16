# RSMC
Reversi stateless model in C
---
[![License: LGPL v2.1](https://img.shields.io/badge/License-LGPL_v2.1-blue.svg)](https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html)
[![Latest Stable Version](https://img.shields.io/github/v/release/01eksa/rsmc)](https://github.com/01eksa/rsmc/releases)
[![Tests](https://github.com/01eksa/rsmc/actions/workflows/tests.yml/badge.svg)](https://github.com/01eksa/rsmc/actions)
---

# Why RSMC?
* **Simplicity:** rsmc is written in a fully functional style and does just what you expect.
* **Performance:** rsmc is fast and memory-efficient. It gives you all functionality without any overhead.
* **Predictability:** rsmc doesn't allocate the memory and always validates input data. The only case with UB is call with an invalid pointer.

# Quick Start
## Precompiled binary
You can find ready-to-use binaries for Linux (x64), Windows (x64) and macOS (ARM64) [here](https://github.com/01eksa/rsmc/releases).

## Include via CMake
```cmake
include(FetchContent)
FetchContent_Declare(rsmc
        GIT_REPOSITORY https://github.com/01eksa/rsmc.git
        GIT_TAG v0.1.0 # or whatever version you need
)
FetchContent_MakeAvailable(rsmc)

target_link_libraries(your_project PRIVATE rsmc)
```

## Build from source
Clone repository:
```bash
git clone https://github.com/01eksa/rsmc.git
cd rsmc
```

Build:
```bash
cmake -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build --config Release
```

Run tests:
```bash
ctest --test-dir build -C Release -V --timeout 120
```

# API
You can see [full documentation here](https://01eksa.github.io/rsmc/).

## Example
```c++
#include "rsmc.h"
#include <assert.h>

RsmcCoords ask_move(const RsmcMoves valid_moves)
{
    // ask move with validation
}

void show_game_state(const RsmcGameState state, const RsmcBoard *board)
{
    // show game state
}

void show_game_result(const RsmcGameStatus result)
{
    // greet players after game
}

int main(void)
{
    // first setup
    RsmcPlayer current_player = RsmcPlayerBlack;

    RsmcBoard board;
    rsmc_set_start_position(&board);
    RsmcGameState current_state;

    while (true) {
        // process moves
        const RsmcMoves valid_moves = rsmc_get_valid_moves(&board, current_player);

        if (valid_moves.count > 0) {
            const RsmcCoords chosen_move = ask_move(valid_moves);
            const bool ok = rsmc_apply_move(&board, chosen_move, current_player);
            assert(ok); // chosen_move is guaranteed valid, taken from valid_moves
        }

        // update game state
        current_state = rsmc_get_game_state(&board);
        show_game_state(current_state, &board);

        if (current_state.game_status != RsmcGameStatusContinue) {
            break; // game end
        }

        current_player = !current_player; // toggling guaranteed by API
    }

    show_game_result(current_state.game_status);

    return 0;
}
```

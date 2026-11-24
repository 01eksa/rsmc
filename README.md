# RSMC

Reversi stateless model in C
---
[![License: LGPL v2.1](https://img.shields.io/badge/License-LGPL_v2.1-blue.svg)](https://www.gnu.org/licenses/old-licenses/lgpl-2.1.html)
[![Latest Stable Version](https://img.shields.io/github/v/release/01eksa/rsmc)](https://github.com/01eksa/rsmc/releases)
[![Tests](https://github.com/01eksa/rsmc/actions/workflows/tests.yml/badge.svg)](https://github.com/01eksa/rsmc/actions)
[![CodeQL](https://github.com/01eksa/rsmc/actions/workflows/codeql.yml/badge.svg)](https://github.com/01eksa/rsmc/actions/workflows/codeql.yml)
---

# Why RSMC?

* **Simplicity:** rsmc is stateless, written in a fully functional style and does just what you expect.
* **Performance:** rsmc is fast and memory-efficient. It gives you all basic functionality without any overhead.
* **Safety:** zero dynamic allocations and zero pointers since 1.0, so it's completely memory-safe.
* **Versatility:** you can use it as it is or build your own wrapper around it: make a state model or use it in other
  languages (check out [RsmcSharp](https://github.com/01eksa/RsmcSharp)).

# Quick Start

## Precompiled binary

You can find ready-to-use binaries [here](https://github.com/01eksa/rsmc/releases).

### Compatibility:

|       | Linux | Windows                        | macOS             |
|-------|-------|--------------------------------|-------------------|
| x64   | Any*  | Windows 10 / 11 / Server 2022+ | macOS 15+ (Intel) |
| ARM64 | Any*  | Windows 11 ARM64               | macOS 15+ (ARM64) |

\* Linux binaries are built on Ubuntu 26.04 in freestanding mode (zero libc dependencies) and are intended to run on any distribution. They are tested on Alpine 3.24 (`musl`) and Ubuntu 20.04 (older `glibc`) on both `x64` and `ARM64`.\
\
I tested rsmc on a few architectures. On x64, AVX2 support provides significantly better performance, so I recommend
using the x86-64-v3 binary if your CPU supports it. Minimal requirements: any AMD Ryzen, Intel starting with Haswell
(4th gen).\
On ARM, the difference is less than 2%, so I recommend ARMv8-A for maximum compatibility.\
\
If you need support for other operating systems or architectures, you can build it from source.

## Include via CMake

```cmake
include(FetchContent)
FetchContent_Declare(rsmc
        GIT_REPOSITORY https://github.com/01eksa/rsmc.git
        GIT_TAG v1.1.0 # or whatever version you need
)
FetchContent_MakeAvailable(rsmc)

target_link_libraries(your_project PRIVATE rsmc)
```

## Build from source using presets (cmake 3.29+ required)

Clone repository:

```shell
git clone https://github.com/01eksa/rsmc.git
cd rsmc
```

### Release (clang + lld are required):

Build release

```shell
cmake --workflow --preset build-release
```

Run tests

```shell
ctest --preset release
```

Run benchmark:

```shell
build/release/benchmark/rsmcbench
```

Pack release:

```shell
cpack --preset release-tgz # or release-zip if you need .zip instead of .tar.gz
```

### Debug (system defaults):

Build:

```shell
cmake --workflow --preset build-debug
```

Run tests:

```shell
ctest --preset debug
```

Run benchmark:

```shell
build/debug/benchmark/rsmcbench
```

# API

You can see [full documentation here](https://01eksa.github.io/rsmc/).

## Example

See full code [here](https://github.com/01eksa/rsmc-example).

```c++
#include "rsmc.h"

#include <stdbool.h>
#include <stdio.h>

#define CLEAR "\033[2J\033[H"

static RsmcBitMask ask_move(const RsmcBitMap valid_moves, const RsmcPlayer player)
{
    // ask move with validation
}

static void show_game_state(const RsmcGameState state, const RsmcBoard board,
                            const RsmcBitMask valid_moves)
{
    // show game state
}

static void show_game_result(const RsmcGameStatus result)
{
    // greet players after game
}

int main(void)
{
    // first setup
    RsmcPlayer current_player = RsmcPlayerBlack;
    RsmcBoard board = rsmc_get_start_position();
    RsmcGameState current_state = rsmc_get_game_state(board);

    do {
        // process moves
        const RsmcBitMap valid_moves = rsmc_get_valid_moves(board, current_player);
        show_game_state(current_state, board, valid_moves);

        if (valid_moves) {
            const RsmcBitMask chosen_move = ask_move(valid_moves, current_player);
            board = rsmc_apply_move(board, chosen_move, current_player);
        }

        // update info about game state
        current_player = !current_player; // toggling guaranteed by API
        current_state = rsmc_get_game_state(board);
    } while (current_state.game_status == RsmcGameStatusContinue);

    show_game_result(current_state.game_status);
    return 0;
}
```

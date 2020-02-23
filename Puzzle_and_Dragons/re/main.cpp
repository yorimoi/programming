/* https://youtu.be/WUHzFk5X-aQ */

#include <iostream>
#include <ctime>
#include <random>

struct Vec2 {
    int x, y;
};

// Constant
constexpr int BOARD_WIDTH     = 6;
constexpr int BOARD_HEIGHT    = 5;
constexpr int MIN_CHAIN_COUNT = 3;
constexpr Vec2 DIRECTIONS[2]  = {{1, 0}/*Right*/, {0, 1}/*Down*/};

// Drop kind
enum DropKind : int {
    NONE = 0,
    FIRE,
    WATER,
    WOOD,
    LIGHT,
    DARK,
    MAX,
};

int BOARD[BOARD_HEIGHT][BOARD_WIDTH];
int CAN_ERASE[BOARD_HEIGHT][BOARD_WIDTH];
Vec2 CURSOR;
bool holding = false;
bool during_animation = false;

#include <unistd.h>
#include <termios.h>

char getch(void) {
    char buf = 0;
    struct termios old = {};

    if (tcgetattr(0, &old) < 0) {
        perror("tcsetattr()");
    }
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0) {
        perror("tcsetattr ICANON");
    }
    if (read(0, &buf, 1) < 0) {
        perror ("read()");
    }
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0) {
        perror ("tcsetattr ~ICANON");
    }

    return buf;
}

int check_chain(Vec2 pos, Vec2 dir, bool erase = false) {
    if (BOARD[pos.y][pos.x] == DropKind::NONE) {
        return 0;
    }

    if (erase) {
        CAN_ERASE[pos.y][pos.x] = true;
    }

    int chain = 1;
    Vec2 new_pos = {
        pos.x + dir.x,
        pos.y + dir.y,
    };

    while (true) {
        if ((BOARD_WIDTH <= new_pos.x) || (BOARD_HEIGHT <= new_pos.y)
                || (BOARD[new_pos.y][new_pos.x] != BOARD[pos.y][pos.x])) {
            break;
        }

        if (erase) {
            CAN_ERASE[new_pos.y][new_pos.x] = true;
        }
        ++chain;
        new_pos.x += dir.x;
        new_pos.y += dir.y;
    }

    return chain;
}

void erase_drops(void) {
    // Initialize
    for (int y = 0; y < BOARD_HEIGHT; ++y) {
        for (int x = 0; x < BOARD_WIDTH; ++x) {
            CAN_ERASE[y][x] = false;
        }
    }

    for (int y = 0; y < BOARD_HEIGHT; ++y) {
        for (int x = 0; x < BOARD_WIDTH; ++x) {
            for (Vec2 d: DIRECTIONS) {
                Vec2 pos = {x, y};
                int chain = check_chain(pos, d);

                if (MIN_CHAIN_COUNT <= chain) {
                    check_chain(pos, d, true);
                    during_animation = true;
                }
            }
        }
    }

    for (int y = 0; y < BOARD_HEIGHT; ++y) {
        for (int x = 0; x < BOARD_WIDTH; ++x) {
            if (CAN_ERASE[y][x]) {
                BOARD[y][x] = DropKind::NONE;
            }
        }
    }
}

int main(void) {
    // Drop color
    char dropColor[DropKind::MAX][5+1] = {
        "\x1b[30m", //NONE
        "\x1b[31m", //FIRE
        "\x1b[32m", //WATER
        "\x1b[33m", //WOOD
        "\x1b[34m", //LIGHT
        "\x1b[35m", //DARK
    };

    std::mt19937 mt;
    std::random_device rnd;
    mt.seed(rnd());  // Not MinGW GCC
    std::cout << "\x1b[H\x1b[2J" << std::endl;

    for (int y = 0; y < BOARD_HEIGHT; ++y) {
        for (int x = 0; x < BOARD_WIDTH; ++x) {
            BOARD[y][x] = mt() % (DropKind::MAX - 1) + 1;
        }
    }

    while (true) {
        std::cout << "\x1b[H+------------+\n";
        for (int y = 0; y < BOARD_HEIGHT; ++y) {
            std::cout << "|";
            for (int x = 0; x < BOARD_WIDTH; ++x) {
                if (holding && (x == CURSOR.x) && (y == CURSOR.y)) {
                    std::cout << dropColor[BOARD[y][x]] << "◎\x1b[0m";
                } else {
                    std::cout << dropColor[BOARD[y][x]] << "●\x1b[0m";
                }
            }
            if (!during_animation && y == CURSOR.y) {
                std::cout << "|←\n";
            } else {
                std::cout << "| \n";
            }
        }
        std::cout << "+------------+\n";
        if (during_animation) {
            std::cout << std::string(CURSOR.x*2+3, ' ') << std::endl;
        } else {
            std::cout << std::string(CURSOR.x*2+1, ' ') <<  "↑ " << std::endl;
        }

        if (!during_animation) {
            Vec2 last_cursor = CURSOR;
            switch (getch()) {
                case 'h': CURSOR.x--; break;
                case 'j': CURSOR.y++; break;
                case 'k': CURSOR.y--; break;
                case 'l': CURSOR.x++; break;
                case ' ':
                          holding = !holding;
                          if (!holding) {
                              erase_drops();
                          }
                          break;
                case 'q': return 0;
            }
            if (CURSOR.x < 0 || BOARD_WIDTH <= CURSOR.x
                    || CURSOR.y < 0 || BOARD_HEIGHT <= CURSOR.y) {
                CURSOR = last_cursor;
                continue;
            }

            // Swap
            if (holding) {
                int tmp = BOARD[last_cursor.y][last_cursor.x];
                BOARD[last_cursor.y][last_cursor.x] = BOARD[CURSOR.y][CURSOR.x];
                BOARD[CURSOR.y][CURSOR.x] = tmp;
            }
        } else {
            time_t t = time(NULL);
            while (t == time(NULL)) {}

            bool droped = false;
            for (int y = BOARD_HEIGHT-2; 0 <= y; --y) {
                for (int x = 0; x < BOARD_WIDTH; ++x) {
                    if ((BOARD[y][x] != DropKind::NONE)
                            && (BOARD[y+1][x] == DropKind::NONE)) {
                        BOARD[y+1][x] = BOARD[y][x];
                        BOARD[y][x]   = DropKind::NONE;
                        droped = true;
                    }
                }
            }

            for (int x = 0; x < BOARD_WIDTH; ++x) {
                if (BOARD[0][x] == DropKind::NONE) {
                    BOARD[0][x] = mt() % (DropKind::MAX - 1) + 1;
                    droped = true;
                }
            }

            if (!droped) {
                during_animation = false;
                erase_drops();
            }
        }
    }

    return 0;
}


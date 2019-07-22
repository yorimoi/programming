
var turn = 1;
var piece;
var cells = [];
var weights = [];
var cpu = 2;
var gameover = false;
var pass = false;

window.onload = function() {

    piece = [
        document.getElementById('cell'),
        document.getElementById('black'),
        document.getElementById('white')];

    init_field();

    draw();
}

function draw() {
    let board = document.getElementById('board');

    // Clean board
    while(board.firstChild) {
        board.removeChild(board.firstChild);
    }

    let name = "CPU";
    if(turn == 1) {
        if(document.getElementById('name').value != "")
            name = document.getElementById('name').value;
        else
            name = "Guest";
    }

    document.getElementById('turn').innerHTML = name + "'s turn";
    document.getElementById('turn_color').style.background = turn==1 ? "#000": "#FFF";

    for(let y=1; y<=8; ++y) {
        for(let x=1; x<=8; ++x) {
            let cell = piece[cells[y][x]].cloneNode(true);
            if(x==1) cell.style.clear = "left";
            board.appendChild(cell);

            if(cells[y][x] == 0 && turn == 1) {
                let _x = x,
                    _y = y;
                cell.onclick = function() {
                    if(check_cells(_x, _y, true)) {
                        let current_turn = turn_change();
                        draw();
                        setTimeout(function() {
                            if(current_turn == cpu)
                                cpu_move();
                        }, 1000);
                    }
                };
            }
        }
    }
}

function check_cells(x, y, flip) {
    let ret = 0;
    for(let dy=-1; dy<=1; ++dy) {
        for(let dx=-1; dx<=1; ++dx) {
            if(dx == 0 && dy == 0) {
                continue;
            }

            let nx = x + dx,
                ny = y + dy,
                n  = 0;
            while(cells[ny][nx] == 3 - turn) {
                ++n;
                nx += dx;
                ny += dy;
            }
            if(n > 0 && cells[ny][nx] == turn) {
                ret += n;

                if(flip) {
                    cells[y][x] = turn;
                    nx = x + dx;
                    ny = y + dy;
                    while(cells[ny][nx] == 3 - turn) {
                        cells[ny][nx] = turn;
                        nx += dx;
                        ny += dy;
                    }
                }
            }
        }
    }
    return ret;
}

function turn_change() {
    turn = 3 - turn;

    for(let y=1; y<=8; ++y) {
        for(let x=1; x<=8; ++x) {
            if(cells[y][x] == 0  && check_cells(x, y, false)) {
                if(!gameover)
                    return cpu;
                return;
            }
        }
    }

    // PASS
    turn = 3 - turn;
    for(let y=1; y<=8; ++y) {
        for(let x=1; x<=8; ++x) {
            if(cells[y][x] == 0  && check_cells(x, y, false)) {
                let msg = document.getElementById('message');
                msg.style.visibility = "visible";
                setTimeout(function() {
                    msg.style.visibility = "hidden";
                }, 3000);
                if(turn == cpu)
                    cpu_move();
                return;
            }
        }
    }

    // GAME OVER
    let black = 0,
        white = 0;
    gameover = true;
    for(let y=1; y<=8; ++y) {
        for(let x=1; x<=8; ++x) {
            if(cells[y][x] == 1)
                ++black;
            else if(cells[y][x] == 2)
                ++white;
        }
    }
    let msg = document.getElementById('message');
    let score = "BLACK&nbsp;" + black + " : WHITE&nbsp;" + white + "&nbsp;&nbsp;";
    let result = black > white ? "BLACK WIN!" : white > black ? "WHITE WIN!" : "DRAW";
    msg.innerHTML = score + result;
    msg.style.visibility = "visible";
}

function cpu_move() {
    let weight = -127;
    let x2, y2;
    if(turn == cpu) {
        for(let y=1; y<=8; ++y) {
            for(let x=1; x<=8; ++x) {
                if(cells[y][x] == 0  && check_cells(x, y, false)) {
                    if(weights[y][x] > weight) {
                        weight = weights[y][x];
                        x2 = x;
                        y2 = y;
                    }
                }
            }
        }
    }

    check_cells(x2, y2, true);
    turn_change();
    draw();
}

function init_field() {
    for(let y=0; y<10; ++y) {
        cells[y]   = [];
        weights[y] = [];
        for(let x=0; x<10; ++x) {
            cells[y][x]   = 0;
            weights[y][x] = 0;
        }
    }

    cells[4][5] = cells[5][4] = 1;
    cells[4][4] = cells[5][5] = 2;

    // CPU weights
    weights[1][1] = weights[1][8] = weights[8][1] = weights[8][8] = 30;

    weights[1][3] = weights[1][6] = weights[8][3] = weights[8][6] =
    weights[3][1] = weights[3][3] = weights[3][6] = weights[3][8] =
    weights[6][1] = weights[6][3] = weights[6][6] = weights[6][8] = 0;

    weights[1][4] = weights[1][5] = weights[3][4] = weights[3][5] =
    weights[4][1] = weights[5][1] = weights[4][8] = weights[5][8] =
    weights[4][3] = weights[4][4] = weights[4][5] = weights[4][6] =
    weights[5][3] = weights[5][4] = weights[5][5] = weights[5][6] =
    weights[6][4] = weights[6][5] = weights[8][4] = weights[8][5] = -1;

    weights[2][3] = weights[2][4] = weights[2][5] = weights[2][6] =
    weights[7][3] = weights[7][4] = weights[7][5] = weights[7][6] =
    weights[3][2] = weights[4][2] = weights[5][2] = weights[6][2] =
    weights[3][7] = weights[4][7] = weights[5][7] = weights[6][7] = -3;

    weights[1][2] = weights[1][7] = weights[2][1] = weights[2][8] =
    weights[7][1] = weights[7][8] = weights[8][2] = weights[8][7] = -12;

    weights[2][2] = weights[2][7] = weights[7][2] = weights[7][7] = -15;


    gameover = false;
    turn = 1;
    pass = false;

    let msg = document.getElementById('message');
    msg.style.visibility = "hidden";
    msg.innerHTML = "PASS";
}

window.addEventListener('click', function(e) {
    if(e.target.id == "button_init") {
        init_field();
        draw();
    }
});

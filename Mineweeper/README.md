# Minesweeper
Simplistic Terminal Minesweeper  
![Screenshot](https://user-images.githubusercontent.com/29778890/53677459-88b82600-3cf3-11e9-92b5-10f27cee9c4d.png)


## Environment
* Linux, WSL

## Installation
`git clone https://github.com/yorimoi/minesweeper.git`  
`cd minesweeper`  
`make`  
`./ms` or `sudo make install`  

## Usage
```
$ ms --help
Simplistic Terminal Minesweeper
default: -w 9 -h 9 -m 10
Usage: ms [-w width] [-h height] [-m mines] [--help]
  -w,--width    <width>
  -h,--height   <height>
  -m,--mine     <mines>
  --help        Show this help message.
Commands:
  Move cursor:  Arrow keys, wasd or hjkl
  Mark flag:    'f' key
  Mark ?:       '?' key
  Exit:         'q' key
  Open tiles:   Other keys
```

## License
MIT

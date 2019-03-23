#include <stdio.h>
#include <windows.h>

/* define {{{*/
#define C_0  16
#define CS0  17
#define D_0  18
#define DS0  19
#define E_0  20
#define F_0  21
#define FS0  23
#define G_0  24
#define GS0  26
#define A_0  27
#define AS0  29
#define B_0  30

#define C_1  32
#define CS1  34
#define D_1  36
#define DS1  38
#define E_1  41
#define F_1  43
#define FS1  46
#define G_1  49
#define GS1  51
#define A_1  55
#define AS1  58
#define B_1  61

#define C_2  65
#define CS2  69
#define D_2  73
#define DS2  77
#define E_2  82
#define F_2  87
#define FS2  92
#define G_2  98
#define GS2  103
#define A_2  110
#define AS2  116
#define B_2  123

#define C_3  130
#define CS3  138
#define D_3  146
#define DS3  155
#define E_3  164
#define F_3  174
#define FS3  185
#define G_3  196
#define GS3  207
#define A_3  220
#define AS3  233
#define B_3  246

#define C_4  261
#define CS4  277
#define D_4  293
#define DS4  311
#define E_4  329
#define F_4  349
#define FS4  370
#define G_4  392
#define GS4  415
#define A_4  440
#define AS4  466
#define B_4  493

#define C_5  523
#define CS5  554
#define D_5  587
#define DS5  622
#define E_5  659
#define F_5  698
#define FS5  740
#define G_5  784
#define GS5  830
#define A_5  880
#define AS5  932
#define B_5  987

#define C_6  1046
#define CS6  1108
#define D_6  1174
#define DS6  1244
#define E_6  1318
#define F_6  1396
#define FS6  1480
#define G_6  1568
#define GS6  1661
#define A_6  1760
#define AS6  1864
#define B_6  1975

#define C_7  2093
#define CS7  2217
#define D_7  2349
#define DS7  2489
#define E_7  2637
#define F_7  2793
#define FS7  2960
#define G_7  3136
#define GS7  3322
#define A_7  3520
#define AS7  3729
#define B_7  3951
/*}}}*/

void main(){
  Beep( G_5, 500 );
  Beep( G_5, 200 );
  Beep( C_6, 500 );
  Beep( D_6, 500 );
  Beep( E_6, 500 );
  Beep( F_6, 500 );
  Beep( G_6, 200 );
  Sleep(300);
  Beep( C_7, 1000);

  Beep( B_6, 500 );
  Beep( A_6, 200 );
  Beep( A_6, 500 );
  Beep( G_6, 200 );
  Sleep(300);
  Beep( FS6, 200 );
  Beep( FS6, 200 );
  Beep( A_6, 200 );
  Beep( G_6, 500 );
  Beep( E_6, 800 );

  Beep( E_6, 200 );
  Sleep(200);
  Beep( E_6, 200 );
  Beep( E_6, 800 );
  Beep( FS6, 500 );
  Beep( GS6, 500 );
  Beep( A_6, 800 );
}

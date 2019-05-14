#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int main(int argc, char **argv) {
    if(argc < 2) {
        fprintf(stderr, "Invalid argument\n");
        return 1;
    }

    time_t end = time(NULL) + atoi(argv[1]); // secound

    char anim[23][30] = {
        "●ヽ(´･ω･`)ﾉ●",
        "●ヽ(･ω･` )ﾉ● ",
        "　●(ω･`ﾉ●    ",
        "　　(･`ﾉ● )    ",
        "　　(●　　)●  ",
        "●ヽ(　　　)ﾉ● ",
        "　●( 　　´)ﾉ●",
        "　　( 　 ´ﾉ●  ",
        "　　(　ﾉ● )    ",
        "　　●,´･ω)   ",
        "●ヽ( ´･ω･)ﾉ●",
        "●ヽ(´･ω･`)ﾉ●",
        "●ヽ(･ω･` )ﾉ● ",
        "　●(ω･`ﾉ●    ",
        "　　(･`ﾉ● )    ",
        "　　(●　　)●  ",
        "●ヽ(　　　)ﾉ● ",
        "　●( 　　´)ﾉ●",
        "　　( 　 ´ﾉ●  ",
        "　　(　ﾉ● )    ",
        "　　●,´･ω)   ",
        "●ヽ( ´･ω･)ﾉ●",
    };

    printf("NOW LOADING...\n");

    int i = 0;
    while(1) {
        printf("\r%s", anim[i]);
        usleep(0.1 * 1000 * 1000);  // 0.1 sec
        fflush(stdout);
        if(i<23) i++;
        else     i=0;
        if(time(NULL) >= end)
            break;
    }

    printf("\n");

    return 0;
}

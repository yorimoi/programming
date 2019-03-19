/*------------------------
$ gcc --version
gcc (GCC) 8.2.1 20181127
------------------------*/
//main(i){for(;i<101;puts(i++%5?"":"Buzz"))printf(i%3?i%5?"%d":0:"Fizz",i);}
//echo "P" | head -c 1 > file.c && gcc -DP='main(i){for(;i<101;puts("Buzz"-i*i++%5))printf(i%3?i%5?"%d":0:"Fizz",i);}' file.c
main(i){for(;i<101;puts("Buzz"-i*i++%5))printf(i%3?i%5?"%d":0:"Fizz",i);}

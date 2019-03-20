/*253byteは異次元すぎる*/
/*
#define f(v)for(v=0;v<k;++v)
#define d(q,n)case q:r(c+n,c+2*n);
z[4225],i,j,k=65;r(p,c){if(!z[c]){z[p]=z[c]=1;f(p)switch(rand()%4){d(0,-k)d(1,k)d(2,-1)d(3,1)}}}main(){f(i)z[i]=z[i+4160]=z[i*k]=z[i*k+64]=z[4157]=1;r(67,132);f(i)f(j)putchar(33-z[i*k+j]);}
*/
r,y,c[31][31];
d(x,y){
  int b=0;
  for(;;){
    if((r=rand()%4)>2){
      if(c[y][x-2]){
        c[y][x-1]=c[y][x-2]=0;
        d(x-2,y);
      }else b|=8;
    }else if(r>1){
      if(c[y+2][x]){
        c[y+1][x]=c[y+2][x]=0;
        d(x,y+2);
      }else b|=4;
    }else if(r>0){
      if(c[y][x+2]){
        c[y][x+1]=c[y][x+2]=0;
        d(x+2,y);
      }else b|=2;
    }else{
      if(c[y-2][x]){
        c[y-1][x]=c[y-2][x]=0;
        d(x,y-2);
      }else b|=1;
    }
    if(14<b)return;
  }
}
main(x){
  for(;++y<30;srand(time(0)))
    for(x=1;x<30;)
      c[y][x++]=1;
  d(2,2);
  for(y=0;y++<31;puts(""))for(x=0;x<31;putchar(c[y-1][x++]?33:32));
}

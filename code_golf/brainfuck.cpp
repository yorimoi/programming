#include<iostream>
#include<stack>
#include<list>
#define c(x) case x:
#define b break;
using namespace std;
main(int _,char**v){
list<char>a(9);
auto d=a.begin();
stack<char*>l,r;
for(char*w=v[1],*o=w;*o;o++)
switch(*o){
c('+')(*d)++;b
c('-')(*d)--;b
c('>')d++;b
c('<')d--;b
c('[')l.push(o);if(!*d)o=r.top(),r.pop();b
c(']')r.push(o);if(*d)o=l.top()-1,l.pop();b
c('.')cout<<*d;b
c(',')cin>>*d;}}

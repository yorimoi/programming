#include <windows.h>

void key_down(unsigned char code);
void key_up(unsigned char code);
void send_key(unsigned char code);
void send_str(char *str);


int main()
{
  Sleep(5000);
  // memo
  // 取りこぼしてる
  // a-zA-Z0-9まで表示成功
  send_str("testTEST09!\"#$%&'(),./\\");

  return 0;
}

void key_down(unsigned char code)
{
  keybd_event(code, 0, KEYEVENTF_EXTENDEDKEY, 0);
}

void key_up(unsigned char code)
{
  keybd_event(code, 0, KEYEVENTF_EXTENDEDKEY | KEYEVENTF_KEYUP, 0);
}

void send_key(unsigned char code)
{
  key_down(code);
  key_up(code);
}

void send_str(char *str)
{
  while(*str) {
    if('a'<=*str && 'z'>=*str)
      send_key(*str-32);
    else if('A'<=*str && 'Z'>=*str) {
      key_down(VK_SHIFT);
      send_key(*str);
      key_up(VK_SHIFT);
    }
    else
      send_key(*str);
    *str++;
  }
}

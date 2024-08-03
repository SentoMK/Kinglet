#include <conio.h>
#include <windows.h>
#include <stdio.h>

HANDLE hStdin;
DWORD fdwSaveOldMode;


void die(const char *s) {
  perror(s);
  exit(1);
}

void disableRawMode() {
  SetConsoleMode(hStdin, fdwSaveOldMode);
}

void enableRawMode() {
  hStdin = GetStdHandle(STD_INPUT_HANDLE);
  if (hStdin == INVALID_HANDLE_VALUE) die("GetStdHandle");

  if (!GetConsoleMode(hStdin, &fdwSaveOldMode)) die("GetConsoleMode");
  atexit(disableRawMode);

  DWORD fdwMode = fdwSaveOldMode;
  fdwMode &= ~(ENABLE_ECHO_INPUT | ENABLE_LINE_INPUT | ENABLE_PROCESSED_INPUT);
  fdwMode &= ~(ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT);

  if (!SetConsoleMode(hStdin, fdwMode)) die("SetConsoleMode");
}

int main() {
  enableRawMode();

  while (1) {
    char c = '\0';
    if (_kbhit()) {
      c = _getch();
      if (iscntrl(c)) {
        printf("%d\r\n", c);
      } else {
        printf("%d ('%c')\r\n", c, c);
      }
      if (c == 'q') break;
    }
  }

  return 0;
}

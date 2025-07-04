#include <stdio.h>

int readkey(void); /* 環境依存 */

int main(void) {
	int key;

	while(1) {
		key = readkey();
		if(key == 'q') {
			break;
		}
	}
	return 0;
}



#if defined(__linux__)
#	include <termios.h>
#	include <unistd.h>
#elif defined(_WIN32) || defined(_WIN64)
#	include<windows.h>
#endif

/* 以降には環境依存（linuxとWindowsに対応している）コードを記述しています */

int readkey(void) {
	int key;

/*	非カノニカルモードにしてとエコー無効化 */
#if defined(__linux__)
	struct termios termios;
	tcgetattr(STDIN_FILENO, &termios);
	termios.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &termios);
#elif defined(_WIN32) || defined(_WIN64)
	DWORD mode;
	GetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), &mode);
	mode &= ~(ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT);
	SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), mode);
#endif

	key = getchar();

/*	カノニカルモードにしてとエコー有効化 */
#if defined(__linux__)
	termios.c_lflag |= (ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &termios);
#elif defined(_WIN32) || defined(_WIN64)
	mode |= (ENABLE_LINE_INPUT | ENABLE_ECHO_INPUT);
	SetConsoleMode(GetStdHandle(STD_INPUT_HANDLE), mode);
#endif

	return key;
}

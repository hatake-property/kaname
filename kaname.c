#include <stdio.h>
#include <string.h>

int readkey(void); /* 環境依存 */

char path[256];

int main(int argc, char **argv) {
	int key;

	if(argc < 2) {
		FILE *file;
		for(int i = 1; 1; i ++) {
			sprintf(path, "名称未設定-%d.txt", i);
			file = fopen(path, "r");
			if(!file) {
				break;
			}
			fclose(file);
		}
	} else if(argc == 2) {
		strcpy(path, argv[1]);
	} else {
		fputs("コマンドライン引数が多すぎます", stderr);
		return 1;
	}

	while(1) {
		key = readkey();
		switch(key) {
			case 'q':
				goto EXIT_SUCCESS;
			case '-':
				char cmd[1024];
				gets(cmd);
				if(strcmp(cmd, "quit") == 0) {
					goto EXIT_SUCCESS;
				}
				break;
		}
	}

EXIT_SUCCESS:
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

#define TOKEN_BUFFER 1024
#define TOKEN_STR_BUFFER 256

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct {
	char *str;
	size_t size_of;
	enum {
		TT_WORD,
		TT_BLANK,
	} type;
} token_s;

void free_token(void);
int readkey(void); /* 環境依存 */

char path[256];
struct token_s *token;
int size_of_token;

int main(int argc, char **argv) {
	int key;

//	ファイルの内容を取得
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
		size_of_token = TOKEN_BUFFER;
		token = malloc(sizeof(struct token) * size_of_token);
		if(!token) {
			free_token();
			return -1;
		}
		token[0].size_of = TOKEN_STR_BUFFER;
		token[0].str = malloc(sizeof(char) * token[0].size_of);
		strcpy(token[0].str, "\n");
		if(size_of_token < 2) {
			void *p;
			size_of_token += TOKEN_BUFFER;
			p = realloc(token, size_of_token);
			if(!p) {
				free_token();
				return -1;
			}
			for(int i = size_of_token - TOKEN_BUFFER; i < size_of_token; i ++) {
				token[i].size_of = TOKEN_STR_BUFFER;
				token[i].str = malloc(sizeof(char) * token[i].size_of);
			}
		}
		token[1].str = NULL;
	} else if(argc == 2) {
		FILE *file;
		file = fopen(path, "r");
		if(file) {
		//	ファイルの内容をトークン化する
		} else {
			size_of_token = TOKEN_BUFFER;
			token = malloc(sizeof(struct token) * size_of_token);
			if(!token) {
				free_token();
				return -1;
			}
			token[0].size_of = TOKEN_STR_BUFFER;
			token[0].str = malloc(sizeof(char) * token[0].size_of);
			strcpy(token[0].str, "\n");
			if(size_of_token < 2) {
				void *p;
				size_of_token += TOKEN_BUFFER;
				p = realloc(token, size_of_token);
				if(!p) {
					free_token();
					return -1;
				}
				for(int i = size_of_token - TOKEN_BUFFER; i < size_of_token; i ++) {
					token[i].size_of = TOKEN_STR_BUFFER;
					token[i].str = malloc(sizeof(char) * token[i].size_of);
				}
			}
			token[1].str = NULL;
		}
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

void free_token(void) {
	for(int i = 0; i < size_of_token; i ++) {
		free(token[i].str);
	}
	free(token);
}

/* 以降には環境依存（linuxとWindowsに対応している）コードを記述しています */

#if defined(__linux__)
#	include <termios.h>
#	include <unistd.h>
#elif defined(_WIN32) || defined(_WIN64)
#	include<windows.h>
#endif

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

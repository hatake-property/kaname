#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct color_s {
	unsigned char background;
	unsigned char foreground;
	struct {
		unsigned char blink	: 1;
		unsigned char bold	: 1;
		unsigned char italic	: 1;
		unsigned char underline	: 1;
	} is;
};

struct theme_s {
	struct color_s blank;
	struct color_s newline;
	struct color_s word;
	char name[256];
};

struct token_s {
	char *str;
	size_t size_of;
	enum {
		TT_WORD,
		TT_BLANK,
		TT_NEWLINE,
	} type;
};

void free_proj(void);
int readkey(void); /* 環境依存 */
void realloc_token(void);
void realloc_token_str(struct token_s *t);
void set_color(const struct color_s *color); /* 環境依存 */

char path[256];
struct theme_s *theme;
int size_of_theme;
int used_theme;
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
		size_of_token = 0;
		token = NULL;
		realloc_token();
		strcpy(token[0].str, "\n");
		token[0].type = TT_NEWLINE;
	} else if(argc == 2) {
		FILE *file;
		file = fopen(path, "r");
		if(file) {
		//	ファイルの内容をトークン化する
			char ch;
			int i, j = 0;
			size_of_token = TOKEN_BUFFER;
			token = malloc(sizeof(struct token) * size_of_token);
			if(!token) {
				free_proj();
				return -1;
			}
			for(int i = 0; i < size_of_token; i ++) {
				token[i].size_of = TOKEN_STR_BUFFER;
				token[i].str = malloc(sizeof(char) * token[i].size_of);
			}
			while((ch = fgetc(file)) != EOF) {
				if(i >= size_of_token) {
					realloc_token();
				}
				if(j >= token[i].size_of - 1) {
					realloc_token_str();
				}
				token[i].str[j] = ch;
				if(token[i].str[j] == '\n') {
					token[i].type = TT_NEWLINE;
					token[i].str[j + 1] = '\n';
					i ++;
					j = 0;
				} else if(token[i].str[j] == ' ' && token[i].str[j] != ' ') {
					token[i].type = TT_BLANK;
					token[i].str[j + 1] = '\n';
					i ++;
					j = 0;
				} else if(token[i].str[j] != ' ' && token[i].str[j] == ' ') {
					token[i].type = TT_WORD;
					token[i].str[j + 1] = '\n';
					i ++;
					j = 0;
				} else {
					j ++;
				}
			}
		} else {
			token = NULL;
			size_of_token = 0;
			realloc_token();
			strcpy(token[i].str, "\n");
			token[0].type = TT_NEWLINE;
		}
		strcpy(path, argv[1]);
	} else {
		fputs("コマンドライン引数が多すぎます", stderr);
		return 1;
	}

//	themeを設定
	size_of_theme;
	theme = malloc(sizeof(struct theme_s) * size_of_theme);
	strcpy(theme[0].name, "dark");
	theme[0].blank.background = 0x10;
	theme[0].blank.foreground = 0x10;
	theme[0].blank.is.blink = 0;
	theme[0].blank.is.bold = 0;
	theme[0].blank.is.italic = 0;
	theme[0].blank.is.underline = 0;
	theme[0].newline.background = 0x10;
	theme[0].newline.foreground = 0x10;
	theme[0].newline.is.blink = 0;
	theme[0].newline.is.bold = 0;
	theme[0].newline.is.italic = 0;
	theme[0].newline.is.underline = 0;
	theme[0].word.background = 0x10;
	theme[0].word.foreground = 0xe7;
	theme[0].word.is.blink = 0;
	theme[0].word.is.bold = 0;
	theme[0].word.is.italic = 0;
	theme[0].word.is.underline = 0;
	strcpy(theme[1].name, "light");
	theme[1].blank.background = 0xe7;
	theme[1].blank.foreground = 0x10;
	theme[1].blank.is.blink = 0;
	theme[1].blank.is.bold = 0;
	theme[1].blank.is.italic = 0;
	theme[1].blank.is.underline = 0;
	theme[1].newline.background = 0xe7;
	theme[1].newline.foreground = 0xe7;
	theme[1].newline.is.blink = 0;
	theme[1].newline.is.bold = 0;
	theme[1].newline.is.italic = 0;
	theme[1].newline.is.underline = 0;
	theme[1].word.background = 0xe7;
	theme[1].word.foreground = 0xe7;
	theme[1].word.is.blink = 0;
	theme[1].word.is.bold = 0;
	theme[1].word.is.italic = 0;
	theme[1].word.is.underline = 0;
	used_theme = 0;

	while(1) {
		for(int i = 0; i < size_of_token; i ++) {
			switch(token[i].type) {
				case TT_BLANK:
					set_color(theme[used_theme].blank);
					break;
				case TT_NEWLINE:
					set_color(theme[used_theme].newline);
					break;
				case TT_WORD:
					set_color(theme[used_theme].word);
					break;
			}
			printf("%s", token[i].str);
		}
		key = readkey();
		switch(key) {
			case 'q':
				goto EXIT_SUCCESS;
			case '-':
				char cmd[1024];
				fgets(cmd, 1024, stdin);
				if(strcmp(cmd, "quit") == 0) {
					goto EXIT_SUCCESS;
				} else if(strcmp(cmd, "theme") == 0) {
					printf("theme:%s", theme[used_theme].name);
				} else if(strcmp(cmd, "theme:dark") == 0) {
					used_theme = 0;
				} else if(strcmp(cmd, "theme:light") == 0) {
					used_theme = 1;
				}
				break;
		}
	}

EXIT_SUCCESS:
	free_proj();
	return 0;
}

void free_proj(void) {
	free(theme);
	for(int i = 0; i < size_of_token; i ++) {
		free(token[i].str);
	}
	free(token);
}

void get_default_black_color()

void realloc_token(void) {
	void *p;
	int old_memory;

	old_memory = size_of_token;
	size_of_token += 1024;
	p = realloc(token, sizeof(struct token_s) * size_of_token);
	if(!p) {
		free_proj();
		fputs("メモリリークが発生しました");
		return -1;
	}
	for(int i = old_memory; i < size_of_token; i ++) {
		token[i].str = NULL;
		token[i].size_of = 0;
		realloc_token_str(token[i]);
	}
}

void realloc_token_str(struct token_s *t) {
	void *p;

	old_memory = t.size_of;
	t.size_of += 256;
	p = realloc(t.str, sizeof(char) * size_of_token);
	if(!p) {
		free_proj();
		fputs("メモリリークが発生しました");
		return -1;
	}
	t.str[0] = '\0';
}

/* 以降には環境依存（linuxとWindowsに対応している）コードを記述しています */

#if defined(__linux__)
#	include <termios.h>
#	include <unistd.h>
#elif defined(_WIN32) || defined(_WIN64)
#	include<windows.h>
int can_use_anis = 0;
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

void set_color(const struct color_s *color) {
#if defined(_WIN32) || defined(_WIN64)
	DWORD mode = 0;
	GetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), &mode);
	mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(GetStdHandle(STD_OUTPUT_HANDLE), mode);
#endif
	printf(
		"\e[48;5;%d;38;5;%dm%s%s%s%s",
		color->background,
		color->foreground,
		color->is.blink == 1 ? "\e[5m" : "",
		color->is.bold == 1 ? "\e[1m" : "",
		color->is.italic == 1 ? "\e[3m" : "",
		color->is.underline == 1 ? "\e[4m" : ""
	);
}


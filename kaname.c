#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
void realloc_token(void);
void realloc_token_str(struct token_s *t);

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
	get_theme_default_dark(&theme[0]);
	get_theme_default_light(&theme[1]);
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

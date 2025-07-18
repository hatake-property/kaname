#ifndef KANAME_COLOR
#define KANAME_COLOR

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

void get_theme_default_dark(struct theme_s *theme);
void get_theme_default_light(struct theme_s *theme);

#endif

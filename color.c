#include "color.h"

void get_theme_default_dark(struct theme_s *theme){
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
    return;
}

void get_theme_default_light(struct theme_s *theme){
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
    return;
}

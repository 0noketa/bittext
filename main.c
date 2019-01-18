#include <stdio.h>
#include <string.h>
#include "bittext.h"


void put_plane(bittext_plane_t plane)
{
	bittext_plane_t mask;
	int i;

	for (i = 0; i < BITTEXT_PLANE_BITS; ++i)
	{
		mask = plane & (BITTEXT_PLANE_HBIT >> i);
		putchar('0' + !!mask);
	}
}

void dump_plane(bittext_plane_t plane)
{
	put_plane(plane);

	puts("");
}

void put_word(bittext_word_t wrd)
{
	bittext_word_t mask;
	int i;

	for (i = 0; i < BITTEXT_WORD_BITS; ++i)
	{
		mask = wrd & (BITTEXT_WORD_HBIT >> i);
		putchar('0' + !!mask);
	}
}

void dump_word(bittext_word_t wrd)
{
	int i;

	put_word(wrd);
	putchar(':');

	for (i = 0; i < BITTEXT_WORD_COLS; ++i)
		putchar(bittext_word_char(wrd, i));

	puts("");
}

void dump_page(bittext_page_t *page)
{
	int i, j;

	for (i = 0; i < BITTEXT_CODE_MAX; ++i)
	{
		bittext_plane_t plane = (*page)[i];

		if (plane)
		{
			put_plane(plane);
			putchar(bittext_word_s_char(i));
			puts("");
		}
	}
}

int main()
{

static char *src = "Hello world. this is taro.";

	bittext_page_t page;
	bittext_word_t wrd;
	bittext_plane_t plane;
	char buf[BITTEXT_PLANE_BITS + 1];

	bittext_page_init(&page, src);
	puts(src);
	dump_page(&page);
	bittext_page_str(&page, buf);
	puts(buf);

	wrd = bittext_word_create("is  ");
	dump_word(wrd);
	plane = bittext_page_find(&page, wrd);
	dump_plane(plane);

	wrd = bittext_word_create("L");
	dump_word(wrd);
	plane = bittext_page_find(&page, wrd);
	dump_plane(plane);

	wrd = bittext_word_create("ABCDE");
	dump_word(wrd);
	plane = bittext_page_find(&page, wrd);
	dump_plane(plane);

	return 0;
}

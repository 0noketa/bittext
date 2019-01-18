#include <stdio.h>
#include <string.h>
#include "bittext.h"


#define PLANE_SIZE ((sizeof (bittext_plane_t)) * 8)

void dump(int idx, char *src, bittext_plane_t pos, int offset)
{
	int i, ch;
	bittext_plane_t mask;

	printf("line: %d\n", idx);

	for (i = 0; i < 256 && src[i]; ++i)
	{
		ch = src[i] == '\t' ? ' ' : src[i];

		putchar(ch);
	}

	puts("");

	for (i = 0; i < offset; ++i)
		putchar(' ');

	for (mask = BITTEXT_PLANE_HBIT; mask; mask >>= 1)
	{
		ch = (pos & mask) ? '^' : ' ';

		putchar(ch);
	}

	puts("");
}


int main(int argc, char *argv[])
{
	char line[PLANE_SIZE * 2 + 4];
	char *p;
	bittext_page_t page;
	bittext_word_t ptn;
	bittext_plane_t pos;
	int i = 0;

	if (argc <= 1)
	{
		puts("search [word] < src.txt");

		return 0;
	}

	ptn = bittext_word_create(argv[1]);

	while (!feof(stdin))
	{
		if (0 < fgets(line, PLANE_SIZE + 2, stdin))
		{
			if (p = strchr(line, '\r')) *p = 0;
			if (p = strchr(line, '\n')) *p = 0;

			bittext_page_init(&page, line);

			pos = bittext_page_find(&page, ptn);

			if (pos)
				dump(i, line, pos, 0);

			++i;
		}
	}

	return 0;
}

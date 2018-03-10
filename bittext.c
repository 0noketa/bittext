/*
* bitboard for case-insencitive texts
*/

#include "bittext.h"


bittext_plane_t bittext_plane_index(bittext_plane_t plane)
{
	bittext_plane_t mask;

	for (mask = BITTEXT_PLANE_HBIT; !!mask && !(plane & mask); mask >>= 1);

	return mask;
}

bittext_plane_t bittext_plane_lastindex(bittext_plane_t plane)
{
	bittext_plane_t mask;

	for (mask = 1; !!mask && !(plane & mask); mask <<= 1);

	return mask;
}

bittext_plane_t bittext_plane_new(char *s, char c)
{
	bittext_plane_t r = 0;
	int i;

	for (i = 0; i < BITTEXT_PLANE_BITS && s[i]; ++i)
		if (s[i] == c)
			r |= BITTEXT_PLANE_HBIT >> i;

	return r;
}

int bittext_word_s_code(char c)
{
	return
		 ('A' <= c && c <= 'Z') ? c - 'A'
		:('a' <= c && c <= 'z') ? c - 'a'
		:(c == '.') ? BITTEXT_CODE_DOT
		: BITTEXT_CODE_MAX;
}

int bittext_word_s_char(int code)
{
	return
		 (code < 26) ? code + 'A'
		:(code == BITTEXT_CODE_DOT) ? '.'
		: ' ';
}

bittext_word_t bittext_word_new(char *s)
{
	bittext_word_t r = 0;
	int i;

	for (i = 0; i < BITTEXT_WORD_COLS && s[i]; ++i)
		r = (r << BITTEXT_WORD_CBITS) | bittext_word_s_code(s[i]);

	for (; i < BITTEXT_WORD_COLS; ++i)
		r = (r << BITTEXT_WORD_CBITS) | BITTEXT_CODE_MAX;

	return r;
}

int bittext_word_code(bittext_word_t wrd, int idx)
{
	int d = (BITTEXT_WORD_COLS - 1 - idx) * BITTEXT_WORD_CBITS;
	int code = 0 <= idx && idx < BITTEXT_WORD_COLS
		? (wrd >> d) & 0x1F
		: BITTEXT_CODE_MAX;

	return code;
}

int bittext_word_char(bittext_word_t wrd, int idx)
{
	return bittext_word_s_char(
			bittext_word_code(wrd, idx));
}

int bittext_page_init(bittext_page_t *this, char *s)
{
	int i, c;

	if (!this || !s) return 0;

	for (c = 0; c < BITTEXT_CODE_MAX; ++c)
		(*this)[c] = 0;

	(*this)[BITTEXT_CODE_MAX] = ~0;

	for (i = 0; i < BITTEXT_PLANE_BITS && s[i]; ++i)
	{
		c = bittext_word_s_code(s[i]);

		if (c < 26 || c == BITTEXT_CODE_DOT)
			(*this)[c] |= BITTEXT_PLANE_HBIT >> i;
	}

	return 1;
}

bittext_plane_t bittext_page_find(bittext_page_t *this, bittext_word_t wrd)
{
	bittext_plane_t r;
	int i, c;

	c = bittext_word_code(wrd, 0);

	if (c == BITTEXT_CODE_MAX) return 0;

	r = (*this)[c];

	for (i = 1; i < BITTEXT_WORD_COLS; ++i)
	{
		c = bittext_word_code(wrd, i);

		if (c == BITTEXT_CODE_MAX) break;

		r &= (*this)[c] << i;
	}

	return r;
}

int bittext_page_code(bittext_page_t *this, bittext_plane_t pos)
{
	int i;

	for (i = 0; i < BITTEXT_CODE_MAX && !((*this)[i] & pos); ++i);

	return i;
}

int bittext_page_char(bittext_page_t *this, bittext_plane_t pos)
{
	int c = bittext_page_code(this, pos);

	return bittext_word_s_char(c);
}

int bittext_page_str(bittext_page_t *this, char *s)
{
	int i;

	if (!this || !s) return 0;

	for (i = 0; i < BITTEXT_PLANE_BITS; ++i)
		s[i] = bittext_page_char(this, BITTEXT_PLANE_HBIT >> i);

	s[i] = 0;

	return 1;
}



/*
* bitboard for case-insencitive texts
*
* 2018-03-10
*/
/*
* types
*   char: usual ascii.
*   code: internal charcode.
*   word: pattern to search. packed codes.
*   plane: 1 dimentional bitboard for every character. such as plane for 'A'.
*   page: plane * sizeof(cset).
*/
#ifndef bittext__h
#define bittext__h


#include <stdint.h>


/*
* change them if want to add some character.
* _MAX must be placed at  the last.
*/
#define BITTEXT_CODE_DOT 26
#define BITTEXT_CODE_MAX 27

/* from ascii. 6 bits per char. */
typedef uint32_t bittext_word_t;

typedef uint64_t bittext_plane_t;

/*
* every array indicates places of single character like bitmap.
* special characters:
*   BITTEXT_CODE_DOT = periods.
*   BITTEXT_CODE_MAX = filled plane for iteration. as character in word_t, treated as space.
* if no character is true, they are spaces.
*/
typedef bittext_plane_t bittext_page_t[BITTEXT_CODE_MAX + 1];


#define BITTEXT_PLANE_BITS \
	((sizeof (bittext_plane_t)) * 8)

#define BITTEXT_PLANE_HBIT \
	(((bittext_plane_t) 1) << (BITTEXT_PLANE_BITS - 1))


#define BITTEXT_WORD_COLS \
	( 16 <= (sizeof (bittext_word_t)) ? 21  \
	: 8 <= (sizeof (bittext_word_t)) ? 10  \
	: 4 == (sizeof (bittext_word_t)) ? 5  \
	: 2 == (sizeof (bittext_word_t)) ? 2  \
	: 1)

#define BITTEXT_WORD_CBITS 6

#define BITTEXT_WORD_BITS (BITTEXT_WORD_CBITS * BITTEXT_WORD_COLS)

#define BITTEXT_WORD_HBIT \
	(((bittext_word_t) 1) << (BITTEXT_WORD_BITS - 1))


bittext_plane_t bittext_plane_index(bittext_plane_t plane);
bittext_plane_t bittext_plane_lastindex(bittext_plane_t plane);
bittext_plane_t bittext_plane_new(char *s, char c);

int bittext_word_s_code(char c);
bittext_word_t bittext_word_new(char *s);
int bittext_word_code(bittext_word_t wrd, int idx);
int bittext_word_char(bittext_word_t wrd, int idx);

int bittext_page_init(bittext_page_t *this, char *s);
bittext_plane_t bittext_page_find(bittext_page_t *this, bittext_word_t wrd);
int bittext_page_code(bittext_page_t *this, bittext_plane_t pos);
int bittext_page_char(bittext_page_t *this, bittext_plane_t pos);
int bittext_page_str(bittext_page_t *this, char *s);


#endif

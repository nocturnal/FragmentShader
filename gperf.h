/* ANSI-C code produced by gperf version 3.0.1 */
/* Command-line: gperf -c -C -l -L ANSI-C -t -7 -m 100 gperf.in  */
/* Computed positions: -k'5' */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
#error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gnu-gperf@gnu.org>."
#endif

#line 1 "gperf.in"
struct field_t { const char *name; int index; };

#define TOTAL_KEYWORDS 9
#define MIN_WORD_LENGTH 4
#define MAX_WORD_LENGTH 14
#define MIN_HASH_VALUE 4
#define MAX_HASH_VALUE 16
/* maximum key range = 13, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
hash (register const char *str, register unsigned int len)
{
  static const unsigned char asso_values[] =
    {
      17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
      17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
      17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
      17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
      17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
      17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
      17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
      17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
      17, 17, 17, 17, 17, 17, 17, 17, 17, 17,
      17, 17, 17, 17, 17,  0, 17, 17, 17, 17,
      17, 17,  2, 17, 17, 17, 17, 17,  1, 17,
      17, 17, 17, 17,  0, 17, 17, 17, 17, 17,
      17, 17, 17, 17, 17, 17, 17, 17
    };
  register int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[4]];
      /*FALLTHROUGH*/
      case 4:
        break;
    }
  return hval;
}

#ifdef __GNUC__
__inline
#endif
const struct field_t *
in_word_set (register const char *str, register unsigned int len)
{
  static const unsigned char lengthtable[] =
    {
       0,  0,  0,  0,  4,  5,  6,  0,  0,  9,  0, 11, 12,  0,
      14, 14, 14
    };
  static const struct field_t wordlist[] =
    {
      {""}, {""}, {""}, {""},
#line 11 "gperf.in"
      {"list", GPERF_LIST},
#line 3 "gperf.in"
      {"clear", GPERF_CLEAR},
#line 10 "gperf.in"
      {"invert", GPERF_INVERT},
      {""}, {""},
#line 8 "gperf.in"
      {"draw_line", GPERF_DRAW_LINE},
      {""},
#line 7 "gperf.in"
      {"draw_spline", GPERF_DRAW_SPLINE},
#line 6 "gperf.in"
      {"draw_polygon", GPERF_DRAW_POLYGON},
      {""},
#line 9 "gperf.in"
      {"draw_rectangle", GPERF_DRAW_RECTANGLE},
#line 5 "gperf.in"
      {"set_line_color", GPERF_SET_LINE_COLOR},
#line 4 "gperf.in"
      {"set_fill_color", GPERF_SET_FILL_COLOR}
    };

  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register int key = hash (str, len);

      if (key <= MAX_HASH_VALUE && key >= 0)
        if (len == lengthtable[key])
          {
            register const char *s = wordlist[key].name;

            if (*str == *s && !memcmp (str + 1, s + 1, len - 1))
              return &wordlist[key];
          }
    }
  return 0;
}

#include "character.h"

#define IS_DELIMITER 1
#define IS_DIGIT 2
#define IS_INITIAL 4
#define IS_LETTER 8
#define IS_PECULIAR_IDENTIFIER 16
#define IS_SPECIAL_INITIAL 32
#define IS_SUBSEQUENT 64
#define IS_SPECIAL_SUBSEQUENT 128
#define IS_WHITESPACE 256

const int table[256] = {
	//   0 <null>
	0,
	//   1 <start of heading>
	0,
	//   2 <start of text>
	0,
	//   3 <end of text>
	0,
	//   4 <end of transmission>
	0,
	//   5 <enquiry>
	0,
	//   6 <acknowledgement>
	0,
	//   7 <bell>
	0,
	//   8 <backspace>
	0,
	//   9 <horizontal tab>
	IS_WHITESPACE + IS_DELIMITER,
	//  10 <line feed>
	IS_WHITESPACE + IS_DELIMITER,
	//  11 <vertical tab>
	IS_WHITESPACE + IS_DELIMITER,
	//  12 <form feed>
	IS_WHITESPACE + IS_DELIMITER,
	//  13 <carriage return>
	IS_WHITESPACE + IS_DELIMITER,
	//  14 <shift out / x-on>
	0,
	//  15 <shift in / x-off>
	0,
	//  16 <data line escape>
	0,
	//  17 <device control 1>
	0,
	//  18 <device control 2>
	0,
	//  19 <device control 3>
	0,
	//  20 <device control 4>
	0,
	//  21 <negative acknowledgement>
	0,
	//  22 <synchronous idle>
	0,
	//  23 <end of transmit block>
	0,
	//  24 <cancel>
	0,
	//  25 <end of medium>
	0,
	//  26 <substitute>
	0,
	//  27 <escape>
	0,
	//  28 <file separator>
	0,
	//  29 <group separator>
	0,
	//  30 <record separator>
	0,
	//  31 <unit separator>
	0,
	//  32 <space>
	IS_WHITESPACE + IS_DELIMITER,
	//  33 !
	IS_SPECIAL_INITIAL + IS_INITIAL + IS_SUBSEQUENT,
	//  34 " <double q>
	IS_DELIMITER,
	//  35 #
	0,
	//  36 $
	IS_SPECIAL_INITIAL + IS_INITIAL + IS_SUBSEQUENT,
	//  37 %
	IS_SPECIAL_INITIAL + IS_INITIAL + IS_SUBSEQUENT,
	//  38 &
	IS_SPECIAL_INITIAL + IS_INITIAL + IS_SUBSEQUENT,
	//  39 ' <single q>
	0,
	//  40 (
	IS_DELIMITER,
	//  41 )
	IS_DELIMITER,
	//  42 *
	IS_SPECIAL_INITIAL + IS_INITIAL + IS_SUBSEQUENT,
	//  43 +
	IS_PECULIAR_IDENTIFIER + IS_SPECIAL_SUBSEQUENT + IS_SUBSEQUENT,
	//  44 , <comma>
	0,
	//  45 - <hypen>
	IS_PECULIAR_IDENTIFIER + IS_SPECIAL_SUBSEQUENT + IS_SUBSEQUENT,
	//  46 . <period>
	IS_SPECIAL_SUBSEQUENT + IS_SUBSEQUENT,
	//  47 /
	IS_SPECIAL_INITIAL + IS_INITIAL + IS_SUBSEQUENT,
	//  48 0
	IS_DIGIT + IS_SUBSEQUENT,
	//  49 1
	IS_DIGIT + IS_SUBSEQUENT,
	//  50 2
	IS_DIGIT + IS_SUBSEQUENT,
	//  51 3
	IS_DIGIT + IS_SUBSEQUENT,
	//  52 4
	IS_DIGIT + IS_SUBSEQUENT,
	//  53 5
	IS_DIGIT + IS_SUBSEQUENT,
	//  54 6
	IS_DIGIT + IS_SUBSEQUENT,
	//  55 7
	IS_DIGIT + IS_SUBSEQUENT,
	//  56 8
	IS_DIGIT + IS_SUBSEQUENT,
	//  57 9
	IS_DIGIT + IS_SUBSEQUENT,
	//  58 :
	IS_SPECIAL_INITIAL + IS_INITIAL + IS_SUBSEQUENT,
	//  59 ;
	IS_DELIMITER,
	//  60 <
	IS_SPECIAL_INITIAL + IS_INITIAL + IS_SUBSEQUENT,
	//  61 =
	IS_SPECIAL_INITIAL + IS_INITIAL + IS_SUBSEQUENT,
	//  62 >
	IS_SPECIAL_INITIAL + IS_INITIAL + IS_SUBSEQUENT,
	//  63 ?
	IS_SPECIAL_INITIAL + IS_INITIAL + IS_SUBSEQUENT,
	//  64 @
	0,
	//  65 A
	IS_LETTER + IS_INITIAL + IS_SUBSEQUENT,
	//  66 B
	IS_LETTER + IS_INITIAL + IS_SUBSEQUENT,
	//  67 C
	IS_LETTER + IS_INITIAL + IS_SUBSEQUENT,
	//  68 D
	IS_LETTER + IS_INITIAL + IS_SUBSEQUENT,
	//  69 E
	IS_LETTER + IS_INITIAL + IS_SUBSEQUENT,
	//  70 F
	IS_LETTER + IS_INITIAL + IS_SUBSEQUENT,
	//  71 G
	IS_LETTER + IS_INITIAL + IS_SUBSEQUENT,
	//  72 H
	IS_LETTER + IS_INITIAL + IS_SUBSEQUENT,
	//  73 I
	IS_LETTER + IS_INITIAL + IS_SUBSEQUENT,
	//  74 J
	IS_LETTER + IS_INITIAL + IS_SUBSEQUENT,
	//  75 K
	IS_LETTER + IS_INITIAL + IS_SUBSEQUENT,
	//  76 L
	IS_LETTER + IS_INITIAL + IS_SUBSEQUENT,
	//  77 M
	IS_LETTER + IS_INITIAL + IS_SUBSEQUENT,
	//  78 N
	IS_LETTER + IS_INITIAL + IS_SUBSEQUENT,
	//  79 O
	IS_LETTER + IS_INITIAL + IS_SUBSEQUENT,
	//  80 P
	IS_LETTER + IS_INITIAL + IS_SUBSEQUENT,
	//  81 Q
	IS_LETTER + IS_INITIAL + IS_SUBSEQUENT,
	//  82 R
	IS_LETTER + IS_INITIAL + IS_SUBSEQUENT,
	//  83 S
	IS_LETTER + IS_INITIAL + IS_SUBSEQUENT,
	//  84 T
	IS_LETTER + IS_INITIAL + IS_SUBSEQUENT,
	//  85 U
	IS_LETTER + IS_INITIAL + IS_SUBSEQUENT,
	//  86 V
	IS_LETTER + IS_INITIAL + IS_SUBSEQUENT,
	//  87 W
	IS_LETTER + IS_INITIAL + IS_SUBSEQUENT,
	//  88 X
	IS_LETTER + IS_INITIAL + IS_SUBSEQUENT,
	//  89 Y
	IS_LETTER + IS_INITIAL + IS_SUBSEQUENT,
	//  90 Z
	IS_LETTER + IS_INITIAL + IS_SUBSEQUENT,
	//  91 [
	0,
	/*  92 \ */
	0,
	//  93 ]
	0,
	//  94 ^
	IS_SPECIAL_INITIAL + IS_INITIAL + IS_SUBSEQUENT,
	//  95 _
	IS_SPECIAL_INITIAL + IS_INITIAL + IS_SUBSEQUENT,
	//  96 `
	0,
	//  97 a
	IS_LETTER + IS_INITIAL + IS_SUBSEQUENT,
	//  98 b
	IS_LETTER + IS_INITIAL + IS_SUBSEQUENT,
	//  99 c
	IS_LETTER + IS_INITIAL + IS_SUBSEQUENT,
	// 100 d
	IS_LETTER + IS_INITIAL + IS_SUBSEQUENT,
	// 101 e
	IS_LETTER + IS_INITIAL + IS_SUBSEQUENT,
	// 102 f
	IS_LETTER + IS_INITIAL + IS_SUBSEQUENT,
	// 103 g
	IS_LETTER + IS_INITIAL + IS_SUBSEQUENT,
	// 104 h
	IS_LETTER + IS_INITIAL + IS_SUBSEQUENT,
	// 105 i
	IS_LETTER + IS_INITIAL + IS_SUBSEQUENT,
	// 106 j
	IS_LETTER + IS_INITIAL + IS_SUBSEQUENT,
	// 107 k
	IS_LETTER + IS_INITIAL + IS_SUBSEQUENT,
	// 108 l
	IS_LETTER + IS_INITIAL + IS_SUBSEQUENT,
	// 109 m
	IS_LETTER + IS_INITIAL + IS_SUBSEQUENT,
	// 110 n
	IS_LETTER + IS_INITIAL + IS_SUBSEQUENT,
	// 111 o
	IS_LETTER + IS_INITIAL + IS_SUBSEQUENT,
	// 112 p
	IS_LETTER + IS_INITIAL + IS_SUBSEQUENT,
	// 113 q
	IS_LETTER + IS_INITIAL + IS_SUBSEQUENT,
	// 114 r
	IS_LETTER + IS_INITIAL + IS_SUBSEQUENT,
	// 115 s
	IS_LETTER + IS_INITIAL + IS_SUBSEQUENT,
	// 116 t
	IS_LETTER + IS_INITIAL + IS_SUBSEQUENT,
	// 117 u
	IS_LETTER + IS_INITIAL + IS_SUBSEQUENT,
	// 118 v
	IS_LETTER + IS_INITIAL + IS_SUBSEQUENT,
	// 119 w
	IS_LETTER + IS_INITIAL + IS_SUBSEQUENT,
	// 120 x
	IS_LETTER + IS_INITIAL + IS_SUBSEQUENT,
	// 121 y
	IS_LETTER + IS_INITIAL + IS_SUBSEQUENT,
	// 122 z
	IS_LETTER + IS_INITIAL + IS_SUBSEQUENT,
	// 123 {
	0,
	// 124 |
	0,
	// 125 }
	0,
	// 126 ~
	IS_SPECIAL_INITIAL + IS_INITIAL + IS_SUBSEQUENT,
	// 127 <del>
	0
};

bool is_delimiter(char c)
{
	return table[(unsigned char)c] & IS_DELIMITER;
}

bool is_digit(char c)
{
	return table[(unsigned char)c] & IS_DIGIT;
}

bool is_initial(char c)
{
	return table[(unsigned char)c] & IS_INITIAL;
}

bool is_letter(char c)
{
	return table[(unsigned char)c] & IS_LETTER;
}

bool is_peculiar_identifier(char c)
{
	return table[(unsigned char)c] & IS_PECULIAR_IDENTIFIER;
}

bool is_special_initial(char c)
{
	return table[(unsigned char)c] & IS_SPECIAL_INITIAL;
}

bool is_subsequent(char c)
{
	return table[(unsigned char)c] & IS_SUBSEQUENT;
}

bool is_special_subsequent(char c)
{
	return table[(unsigned char)c] & IS_SPECIAL_SUBSEQUENT;
}

bool is_whitespace(char c)
{
	return table[(unsigned char)c] & IS_WHITESPACE;
}

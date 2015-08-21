/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton implementation for Bison's Yacc-like parsers in C

   Copyright (C) 1984, 1989, 1990, 2000, 2001, 2002, 2003, 2004, 2005, 2006
   Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor,
   Boston, MA 02110-1301, USA.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* C LALR(1) parser skeleton written by Richard Stallman, by
   simplifying the original so-called "semantic" parser.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output.  */
#define YYBISON 1

/* Bison version.  */
#define YYBISON_VERSION "2.3"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 1

/* Using locations.  */
#define YYLSP_NEEDED 0

/* Substitute the variable and function names.  */
#define yyparse scparse
#define yylex   sclex
#define yyerror scerror
#define yylval  sclval
#define yychar  scchar
#define yydebug scdebug
#define yynerrs scnerrs


/* Tokens.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
   /* Put the tokens into the symbol table, so that GDB and other debuggers
      know about them.  */
   enum yytokentype {
     T_ALL = 258,
     T_ANY = 259,
     T_BOTH = 260,
     T_BYP = 261,
     T_BYPASS = 262,
     T_CLASSIFY = 263,
     T_CLEAR = 264,
     T_CONFIG = 265,
     T_COUNTERS = 266,
     T_CRC_CCITT = 267,
     T_CRC = 268,
     T_CRC_32 = 269,
     T_CRC_NONE = 270,
     T_DISABLE = 271,
     T_DROP = 272,
     T_DEST = 273,
     T_DEFAULT = 274,
     T_ENABLE = 275,
     T_EXIT = 276,
     T_ENCAP = 277,
     T_EGRESS = 278,
     T_ETHERNET = 279,
     T_ETXCONFIG = 280,
     T_FPGA = 281,
     T_FORWARD = 282,
     T_FLOW = 283,
     T_FILE = 284,
     T_FIRMWARE = 285,
     T_GLOBAL = 286,
     T_GET = 287,
     T_GTP = 288,
     T_HDLC = 289,
     T_HASH = 290,
     T_IP = 291,
     T_INFO = 292,
     T_INNER = 293,
     T_KEY = 294,
     T_LIF = 295,
     T_LINE = 296,
     T_L4_PORT = 297,
     T_LENGTH = 298,
     T_MODE = 299,
     T_MAC = 300,
     T_MMS = 301,
     T_NONE = 302,
     T_NORMAL = 303,
     T_NETMASK = 304,
     T_OFFSET = 305,
     T_OC = 306,
     T_OC192 = 307,
     T_OC48 = 308,
     T_OUTER = 309,
     T_PACKET = 310,
     T_PROTOCOL = 311,
     T_PPP = 312,
     T_PAYLOAD = 313,
     T_PORT = 314,
     T_PORTS = 315,
     T_QUIT = 316,
     T_RANGE = 317,
     T_RATE = 318,
     T_RCLOAD = 319,
     T_READ = 320,
     T_REGISTER = 321,
     T_RESTORE = 322,
     T_RULE = 323,
     T_RULESET1 = 324,
     T_RULESET2 = 325,
     T_SAMPLE = 326,
     T_SET = 327,
     T_STATS = 328,
     T_SHOW = 329,
     T_SYSTEM = 330,
     T_SOURCE = 331,
     T_SAVE = 332,
     T_SCRAMBLE = 333,
     T_SETTINGS = 334,
     T_SONNET = 335,
     T_SDH = 336,
     T_TO = 337,
     T_TRAFFIC = 338,
     T_TRANSFER = 339,
     T_TCP = 340,
     T_UPDATE = 341,
     T_UDP = 342,
     T_VLAN = 343,
     T_WRITE = 344,
     T_INVALID_CHAR = 345,
     T_EOS = 346,
     T_INT = 347,
     T_WORD_WL = 348,
     T_STRING_WL = 349,
     T_MACADDR_WL = 350,
     T_IPADDR = 351
   };
#endif
/* Tokens.  */
#define T_ALL 258
#define T_ANY 259
#define T_BOTH 260
#define T_BYP 261
#define T_BYPASS 262
#define T_CLASSIFY 263
#define T_CLEAR 264
#define T_CONFIG 265
#define T_COUNTERS 266
#define T_CRC_CCITT 267
#define T_CRC 268
#define T_CRC_32 269
#define T_CRC_NONE 270
#define T_DISABLE 271
#define T_DROP 272
#define T_DEST 273
#define T_DEFAULT 274
#define T_ENABLE 275
#define T_EXIT 276
#define T_ENCAP 277
#define T_EGRESS 278
#define T_ETHERNET 279
#define T_ETXCONFIG 280
#define T_FPGA 281
#define T_FORWARD 282
#define T_FLOW 283
#define T_FILE 284
#define T_FIRMWARE 285
#define T_GLOBAL 286
#define T_GET 287
#define T_GTP 288
#define T_HDLC 289
#define T_HASH 290
#define T_IP 291
#define T_INFO 292
#define T_INNER 293
#define T_KEY 294
#define T_LIF 295
#define T_LINE 296
#define T_L4_PORT 297
#define T_LENGTH 298
#define T_MODE 299
#define T_MAC 300
#define T_MMS 301
#define T_NONE 302
#define T_NORMAL 303
#define T_NETMASK 304
#define T_OFFSET 305
#define T_OC 306
#define T_OC192 307
#define T_OC48 308
#define T_OUTER 309
#define T_PACKET 310
#define T_PROTOCOL 311
#define T_PPP 312
#define T_PAYLOAD 313
#define T_PORT 314
#define T_PORTS 315
#define T_QUIT 316
#define T_RANGE 317
#define T_RATE 318
#define T_RCLOAD 319
#define T_READ 320
#define T_REGISTER 321
#define T_RESTORE 322
#define T_RULE 323
#define T_RULESET1 324
#define T_RULESET2 325
#define T_SAMPLE 326
#define T_SET 327
#define T_STATS 328
#define T_SHOW 329
#define T_SYSTEM 330
#define T_SOURCE 331
#define T_SAVE 332
#define T_SCRAMBLE 333
#define T_SETTINGS 334
#define T_SONNET 335
#define T_SDH 336
#define T_TO 337
#define T_TRAFFIC 338
#define T_TRANSFER 339
#define T_TCP 340
#define T_UPDATE 341
#define T_UDP 342
#define T_VLAN 343
#define T_WRITE 344
#define T_INVALID_CHAR 345
#define T_EOS 346
#define T_INT 347
#define T_WORD_WL 348
#define T_STRING_WL 349
#define T_MACADDR_WL 350
#define T_IPADDR 351




/* Copy the first part of user declarations.  */
#line 10 "parser.y"


#define YYERROR_VERBOSE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ctype.h>

#include "cli.h"
#include "utils/errprint.h"
#include "lexer.h"
#include "database.h"
#include "parserops.h"
#include "utils/watch_list.h"


#define OPT_DEFAULT         0x0002


/*******************************************************************
 *
 * Description:     Macro definitions
 *
 *******************************************************************/
#define YYPARSE_PARAM pparm
#define PARSER_PARAMS           ((struct parser_params *)YYPARSE_PARAM)
#define YYLEX_PARAM             PARSER_PARAMS->scanner
#define PREFIX_LEN_2_MASK(len)  ((0xFFFFFFFFUL << (32 - len)) & 0xFFFFFFFFUL)

#ifndef MIN
#define MIN(a,b)                (((a) <= (b)) ? (a) : (b))
#endif
#ifndef MAX
#define MAX(a,b)                (((a) >= (b)) ? (a) : (b))
#endif

struct cmdbuffers {
  	char cmd[512];
  	struct cmdbuffers *next;
};

void yyerror(char *s);
static char *StrToUpper(char *str);
static int portrange(int a, int b);


/* Enabling traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif

/* Enabling verbose error messages.  */
#ifdef YYERROR_VERBOSE
# undef YYERROR_VERBOSE
# define YYERROR_VERBOSE 1
#else
# define YYERROR_VERBOSE 0
#endif

/* Enabling the token table.  */
#ifndef YYTOKEN_TABLE
# define YYTOKEN_TABLE 0
#endif

#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 62 "parser.y"
{
    int ival;   /* For returning values */
    unsigned int uval;
    struct vlanmap *vptr;
    void **wlptr;   /* watch list entry */
}
/* Line 193 of yacc.c.  */
#line 353 "y.tab.c"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif



/* Copy the second part of user declarations.  */


/* Line 216 of yacc.c.  */
#line 366 "y.tab.c"

#ifdef short
# undef short
#endif

#ifdef YYTYPE_UINT8
typedef YYTYPE_UINT8 yytype_uint8;
#else
typedef unsigned char yytype_uint8;
#endif

#ifdef YYTYPE_INT8
typedef YYTYPE_INT8 yytype_int8;
#elif (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
typedef signed char yytype_int8;
#else
typedef short int yytype_int8;
#endif

#ifdef YYTYPE_UINT16
typedef YYTYPE_UINT16 yytype_uint16;
#else
typedef unsigned short int yytype_uint16;
#endif

#ifdef YYTYPE_INT16
typedef YYTYPE_INT16 yytype_int16;
#else
typedef short int yytype_int16;
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif ! defined YYSIZE_T && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned int
# endif
#endif

#define YYSIZE_MAXIMUM ((YYSIZE_T) -1)

#ifndef YY_
# if YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(msgid) dgettext ("bison-runtime", msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(msgid) msgid
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YYUSE(e) ((void) (e))
#else
# define YYUSE(e) /* empty */
#endif

/* Identity function, used to suppress warnings about constant conditions.  */
#ifndef lint
# define YYID(n) (n)
#else
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static int
YYID (int i)
#else
static int
YYID (i)
    int i;
#endif
{
  return i;
}
#endif

#if ! defined yyoverflow || YYERROR_VERBOSE

/* The parser invokes alloca or malloc; define the necessary symbols.  */

# ifdef YYSTACK_USE_ALLOCA
#  if YYSTACK_USE_ALLOCA
#   ifdef __GNUC__
#    define YYSTACK_ALLOC __builtin_alloca
#   elif defined __BUILTIN_VA_ARG_INCR
#    include <alloca.h> /* INFRINGES ON USER NAME SPACE */
#   elif defined _AIX
#    define YYSTACK_ALLOC __alloca
#   elif defined _MSC_VER
#    include <malloc.h> /* INFRINGES ON USER NAME SPACE */
#    define alloca _alloca
#   else
#    define YYSTACK_ALLOC alloca
#    if ! defined _ALLOCA_H && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#     ifndef _STDLIB_H
#      define _STDLIB_H 1
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's `empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (YYID (0))
#  ifndef YYSTACK_ALLOC_MAXIMUM
    /* The OS might guarantee only one guard page at the bottom of the stack,
       and a page size can be as small as 4096 bytes.  So we cannot safely
       invoke alloca (N) if N exceeds 4096.  Use a slightly smaller number
       to allow for a few compiler-allocated temporary stack slots.  */
#   define YYSTACK_ALLOC_MAXIMUM 4032 /* reasonable circa 2006 */
#  endif
# else
#  define YYSTACK_ALLOC YYMALLOC
#  define YYSTACK_FREE YYFREE
#  ifndef YYSTACK_ALLOC_MAXIMUM
#   define YYSTACK_ALLOC_MAXIMUM YYSIZE_MAXIMUM
#  endif
#  if (defined __cplusplus && ! defined _STDLIB_H \
       && ! ((defined YYMALLOC || defined malloc) \
	     && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef _STDLIB_H
#    define _STDLIB_H 1
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined _STDLIB_H && (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* ! defined yyoverflow || YYERROR_VERBOSE */


#if (! defined yyoverflow \
     && (! defined __cplusplus \
	 || (defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yytype_int16 yyss;
  YYSTYPE yyvs;
  };

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (sizeof (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (sizeof (yytype_int16) + sizeof (YYSTYPE)) \
      + YYSTACK_GAP_MAXIMUM)

/* Copy COUNT objects from FROM to TO.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(To, From, Count) \
      __builtin_memcpy (To, From, (Count) * sizeof (*(From)))
#  else
#   define YYCOPY(To, From, Count)		\
      do					\
	{					\
	  YYSIZE_T yyi;				\
	  for (yyi = 0; yyi < (Count); yyi++)	\
	    (To)[yyi] = (From)[yyi];		\
	}					\
      while (YYID (0))
#  endif
# endif

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack)					\
    do									\
      {									\
	YYSIZE_T yynewbytes;						\
	YYCOPY (&yyptr->Stack, Stack, yysize);				\
	Stack = &yyptr->Stack;						\
	yynewbytes = yystacksize * sizeof (*Stack) + YYSTACK_GAP_MAXIMUM; \
	yyptr += yynewbytes / sizeof (*yyptr);				\
      }									\
    while (YYID (0))

#endif

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  2
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   388

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  102
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  48
/* YYNRULES -- Number of rules.  */
#define YYNRULES  183
/* YYNRULES -- Number of states.  */
#define YYNSTATES  371

/* YYTRANSLATE(YYLEX) -- Bison symbol number corresponding to YYLEX.  */
#define YYUNDEFTOK  2
#define YYMAXUTOK   351

#define YYTRANSLATE(YYX)						\
  ((unsigned int) (YYX) <= YYMAXUTOK ? yytranslate[YYX] : YYUNDEFTOK)

/* YYTRANSLATE[YYLEX] -- Bison symbol number corresponding to YYLEX.  */
static const yytype_uint8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,    98,    97,   100,    99,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,   101,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     1,     2,     3,     4,
       5,     6,     7,     8,     9,    10,    11,    12,    13,    14,
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24,
      25,    26,    27,    28,    29,    30,    31,    32,    33,    34,
      35,    36,    37,    38,    39,    40,    41,    42,    43,    44,
      45,    46,    47,    48,    49,    50,    51,    52,    53,    54,
      55,    56,    57,    58,    59,    60,    61,    62,    63,    64,
      65,    66,    67,    68,    69,    70,    71,    72,    73,    74,
      75,    76,    77,    78,    79,    80,    81,    82,    83,    84,
      85,    86,    87,    88,    89,    90,    91,    92,    93,    94,
      95,    96
};

#if YYDEBUG
/* YYPRHS[YYN] -- Index of the first RHS symbol of rule number YYN in
   YYRHS.  */
static const yytype_uint16 yyprhs[] =
{
       0,     0,     3,     4,     7,     9,    11,    14,    16,    18,
      20,    22,    24,    26,    28,    30,    37,    43,    48,    54,
      60,    67,    70,    73,    76,    77,    79,    80,    82,    84,
      86,    90,    98,   100,   102,   106,   108,   112,   114,   116,
     126,   136,   145,   154,   163,   172,   185,   195,   197,   199,
     201,   203,   205,   207,   209,   211,   213,   215,   217,   220,
     223,   225,   227,   230,   235,   240,   244,   248,   253,   255,
     258,   260,   262,   265,   268,   270,   273,   287,   301,   317,
     334,   348,   362,   370,   377,   379,   381,   391,   401,   409,
     419,   429,   436,   438,   440,   442,   444,   447,   450,   452,
     456,   460,   462,   464,   466,   468,   472,   474,   478,   482,
     484,   486,   488,   490,   492,   494,   496,   498,   502,   504,
     506,   508,   510,   512,   514,   516,   518,   520,   524,   526,
     528,   530,   532,   534,   538,   540,   542,   544,   546,   548,
     550,   552,   554,   556,   558,   560,   562,   564,   566,   568,
     570,   573,   582,   591,   600,   609,   618,   627,   633,   640,
     647,   654,   660,   662,   664,   666,   668,   670,   672,   674,
     676,   678,   687,   697,   705,   710,   718,   725,   735,   744,
     752,   759,   761,   763
};

/* YYRHS -- A `-1'-separated list of the rules' RHS.  */
static const yytype_int16 yyrhs[] =
{
     103,     0,    -1,    -1,   103,   104,    -1,    91,    -1,   105,
      -1,     1,    91,    -1,   109,    -1,   114,    -1,   129,    -1,
     132,    -1,   133,    -1,   141,    -1,   146,    -1,   136,    -1,
      40,    67,    10,   106,   107,    91,    -1,    40,    77,    10,
     107,    91,    -1,    40,    74,    10,    91,    -1,    40,    74,
      10,    29,    91,    -1,    40,    74,    75,    37,    91,    -1,
      40,    86,    30,    51,   115,    91,    -1,    25,    91,    -1,
      21,    91,    -1,    61,    91,    -1,    -1,    19,    -1,    -1,
      94,    -1,    20,    -1,    16,    -1,     7,    74,    91,    -1,
       7,    72,    41,   112,    44,   113,    91,    -1,    92,    -1,
     110,    -1,    92,    97,    92,    -1,   111,    -1,   111,    98,
     112,    -1,    48,    -1,     6,    -1,    40,    72,     8,    51,
     115,    69,   119,   125,    91,    -1,    40,    72,     8,    51,
     115,    70,   120,   128,    91,    -1,    40,     9,     8,    51,
     115,    69,   119,    91,    -1,    40,     9,     8,    51,   115,
      70,   120,    91,    -1,    40,    74,     8,    51,   115,    69,
     119,    91,    -1,    40,    74,     8,    51,   115,    70,   120,
      91,    -1,    40,    72,    51,   115,    55,    43,    62,   116,
     117,    97,   118,    91,    -1,    40,    32,    51,   115,    55,
      43,    62,   116,    91,    -1,    92,    -1,    92,    -1,    92,
      -1,    92,    -1,    92,    -1,     3,    -1,    92,    -1,     3,
      -1,    85,    -1,    87,    -1,    92,    -1,    99,    92,    -1,
      49,    96,    -1,    92,    -1,     4,    -1,    56,   121,    -1,
      76,    36,    96,   122,    -1,    18,    36,    96,   122,    -1,
      76,    42,   123,    -1,    18,    42,   123,    -1,    55,    43,
      62,   116,    -1,   124,    -1,   124,   125,    -1,    92,    -1,
       4,    -1,    50,   126,    -1,    39,    94,    -1,   127,    -1,
     127,   128,    -1,    40,    72,     8,    51,   115,    69,   119,
      70,   120,    27,    44,    19,    91,    -1,    40,    72,     8,
      51,   115,    69,   119,    70,   120,    27,    44,    17,    91,
      -1,    40,    72,     8,    51,   115,    69,   119,    70,   120,
      27,    44,    82,    88,   130,    91,    -1,    40,    72,     8,
      51,   115,    69,   119,    70,   120,    27,    44,    82,    88,
     130,    71,    91,    -1,    40,    72,     8,    51,   115,    69,
     119,    70,   120,    27,    44,    71,    91,    -1,    40,    74,
      27,    44,    51,   115,    69,   119,    70,   120,    27,    44,
      91,    -1,    40,    72,    51,   115,    71,   131,    91,    -1,
      40,    74,    51,   115,    71,    91,    -1,    92,    -1,    92,
      -1,    40,    72,    51,   115,    24,    76,    45,    95,    91,
      -1,    40,    72,    51,   115,    24,    18,    45,    95,    91,
      -1,    40,    74,    51,   115,    84,    44,    91,    -1,    40,
      72,    33,    51,   115,    35,    18,   134,    91,    -1,    40,
      72,    33,    51,   115,    35,    44,   135,    91,    -1,    40,
      74,    33,    51,   115,    91,    -1,    92,    -1,    38,    -1,
      54,    -1,     5,    -1,    94,    91,    -1,   140,    91,    -1,
      93,    -1,    93,    98,   137,    -1,    93,   100,    93,    -1,
      92,    -1,    96,    -1,    94,    -1,   137,    -1,    93,   101,
     137,    -1,   138,    -1,    93,   101,    92,    -1,    93,   101,
      94,    -1,     3,    -1,     9,    -1,    10,    -1,    11,    -1,
      16,    -1,    17,    -1,    19,    -1,    20,    -1,    20,   101,
      93,    -1,    22,    -1,    23,    -1,    27,    -1,    29,    -1,
      31,    -1,    32,    -1,    36,    -1,    37,    -1,    43,    -1,
      43,   101,    92,    -1,    44,    -1,    45,    -1,    47,    -1,
      49,    -1,    50,    -1,    50,   101,    92,    -1,    59,    -1,
      60,    -1,    56,    -1,    62,    -1,    63,    -1,    64,    -1,
      65,    -1,    72,    -1,    73,    -1,    74,    -1,    76,    -1,
      77,    -1,    86,    -1,    88,    -1,    89,    -1,   139,    -1,
     139,   140,    -1,    40,    72,    51,   115,    41,    63,   142,
      91,    -1,    40,    72,    51,   115,    41,    44,   143,    91,
      -1,    40,    72,    51,   115,    22,    44,   144,    91,    -1,
      40,    72,    51,   115,    80,    78,   108,    91,    -1,    40,
      72,    51,   115,    58,    78,   108,    91,    -1,    40,    72,
      51,   115,    13,    44,   145,    91,    -1,    40,    74,    51,
      79,    91,    -1,    40,    74,    51,   115,    11,    91,    -1,
      40,    74,    51,   115,    28,    91,    -1,    40,    74,    51,
     115,    10,    91,    -1,    40,    77,    51,    79,    91,    -1,
      52,    -1,    53,    -1,    80,    -1,    81,    -1,    57,    -1,
      34,    -1,    12,    -1,    14,    -1,    15,    -1,    40,    51,
     115,    26,    66,   147,    65,    91,    -1,    40,    51,   115,
      26,    66,   147,    89,   148,    91,    -1,    40,    51,   115,
      74,    26,    28,    91,    -1,    40,    55,    28,    91,    -1,
      40,    72,    51,   115,    28,   108,    91,    -1,    40,    32,
      51,   115,    28,    91,    -1,    40,    72,    51,   115,    28,
      85,    46,   149,    91,    -1,    40,    32,    51,   115,    28,
      85,    46,    91,    -1,    40,    72,    51,   115,    83,   108,
      91,    -1,    40,    32,    51,   115,    83,    91,    -1,    92,
      -1,    92,    -1,    92,    -1
};

/* YYRLINE[YYN] -- source line where rule number YYN was defined.  */
static const yytype_uint16 yyrline[] =
{
       0,   122,   122,   123,   128,   138,   148,   162,   163,   164,
     165,   166,   167,   168,   169,   170,   243,   265,   273,   281,
     289,   297,   305,   309,   316,   319,   325,   328,   333,   337,
     347,   351,   356,   366,   370,   385,   385,   390,   394,   403,
     444,   473,   495,   517,   539,   561,   575,   585,   595,   605,
     615,   625,   634,   639,   648,   653,   657,   661,   671,   680,
     685,   695,   713,   732,   753,   774,   794,   813,   834,   838,
     878,   888,   907,   927,   994,   998,  1027,  1042,  1057,  1072,
    1087,  1102,  1110,  1118,  1127,  1137,  1152,  1160,  1168,  1182,
    1190,  1198,  1207,  1217,  1221,  1225,  1235,  1243,  1259,  1278,
    1300,  1323,  1342,  1361,  1382,  1401,  1422,  1441,  1465,  1487,
    1506,  1525,  1544,  1563,  1582,  1601,  1620,  1639,  1660,  1679,
    1698,  1717,  1736,  1755,  1774,  1793,  1812,  1831,  1854,  1873,
    1892,  1911,  1930,  1949,  1972,  1991,  2010,  2029,  2048,  2067,
    2087,  2106,  2125,  2144,  2163,  2182,  2201,  2220,  2239,  2259,
    2263,  2281,  2289,  2297,  2305,  2313,  2321,  2329,  2337,  2345,
    2353,  2361,  2371,  2375,  2380,  2384,  2389,  2393,  2398,  2402,
    2406,  2416,  2424,  2432,  2440,  2448,  2456,  2464,  2472,  2480,
    2488,  2497,  2507,  2517
};
#endif

#if YYDEBUG || YYERROR_VERBOSE || YYTOKEN_TABLE
/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "$end", "error", "$undefined", "T_ALL", "T_ANY", "T_BOTH", "T_BYP",
  "T_BYPASS", "T_CLASSIFY", "T_CLEAR", "T_CONFIG", "T_COUNTERS",
  "T_CRC_CCITT", "T_CRC", "T_CRC_32", "T_CRC_NONE", "T_DISABLE", "T_DROP",
  "T_DEST", "T_DEFAULT", "T_ENABLE", "T_EXIT", "T_ENCAP", "T_EGRESS",
  "T_ETHERNET", "T_ETXCONFIG", "T_FPGA", "T_FORWARD", "T_FLOW", "T_FILE",
  "T_FIRMWARE", "T_GLOBAL", "T_GET", "T_GTP", "T_HDLC", "T_HASH", "T_IP",
  "T_INFO", "T_INNER", "T_KEY", "T_LIF", "T_LINE", "T_L4_PORT", "T_LENGTH",
  "T_MODE", "T_MAC", "T_MMS", "T_NONE", "T_NORMAL", "T_NETMASK",
  "T_OFFSET", "T_OC", "T_OC192", "T_OC48", "T_OUTER", "T_PACKET",
  "T_PROTOCOL", "T_PPP", "T_PAYLOAD", "T_PORT", "T_PORTS", "T_QUIT",
  "T_RANGE", "T_RATE", "T_RCLOAD", "T_READ", "T_REGISTER", "T_RESTORE",
  "T_RULE", "T_RULESET1", "T_RULESET2", "T_SAMPLE", "T_SET", "T_STATS",
  "T_SHOW", "T_SYSTEM", "T_SOURCE", "T_SAVE", "T_SCRAMBLE", "T_SETTINGS",
  "T_SONNET", "T_SDH", "T_TO", "T_TRAFFIC", "T_TRANSFER", "T_TCP",
  "T_UPDATE", "T_UDP", "T_VLAN", "T_WRITE", "T_INVALID_CHAR", "T_EOS",
  "T_INT", "T_WORD_WL", "T_STRING_WL", "T_MACADDR_WL", "T_IPADDR", "'-'",
  "','", "'/'", "'.'", "'='", "$accept", "input", "line", "command",
  "default", "configfile_wl", "enable_or_disable", "bypasscmds", "port",
  "ports", "portlist", "byp_or_normal", "filtercmds", "oc_no", "range_no",
  "pktlen_min", "pktlen_max", "ruleno1", "ruleno2", "ruleprotocol",
  "ipprefixormask", "rulel4port", "setrule1option_wl",
  "setrule1options_wl", "ruleoffset", "setrule2option_wl",
  "setrule2options_wl", "forwardcmds", "vlanid", "sample_rate",
  "transfercmds", "gtpcmds", "gtpdestports", "hash_source", "switchcmds",
  "wordcomma_wl", "worddot_wl", "bcmcmd_wl", "bcmcmds_wl", "occmds",
  "line_rate", "line_mode", "encap_mode", "crc_mode", "fpgacmds",
  "fpga_reg", "fpga_reg_data", "mms_size", 0
};
#endif

# ifdef YYPRINT
/* YYTOKNUM[YYLEX-NUM] -- Internal token number corresponding to
   token YYLEX-NUM.  */
static const yytype_uint16 yytoknum[] =
{
       0,   256,   257,   258,   259,   260,   261,   262,   263,   264,
     265,   266,   267,   268,   269,   270,   271,   272,   273,   274,
     275,   276,   277,   278,   279,   280,   281,   282,   283,   284,
     285,   286,   287,   288,   289,   290,   291,   292,   293,   294,
     295,   296,   297,   298,   299,   300,   301,   302,   303,   304,
     305,   306,   307,   308,   309,   310,   311,   312,   313,   314,
     315,   316,   317,   318,   319,   320,   321,   322,   323,   324,
     325,   326,   327,   328,   329,   330,   331,   332,   333,   334,
     335,   336,   337,   338,   339,   340,   341,   342,   343,   344,
     345,   346,   347,   348,   349,   350,   351,    45,    44,    47,
      46,    61
};
# endif

/* YYR1[YYN] -- Symbol number of symbol that rule YYN derives.  */
static const yytype_uint8 yyr1[] =
{
       0,   102,   103,   103,   104,   104,   104,   105,   105,   105,
     105,   105,   105,   105,   105,   105,   105,   105,   105,   105,
     105,   105,   105,   105,   106,   106,   107,   107,   108,   108,
     109,   109,   110,   111,   111,   112,   112,   113,   113,   114,
     114,   114,   114,   114,   114,   114,   114,   115,   116,   117,
     118,   119,   119,   120,   120,   121,   121,   121,   122,   122,
     123,   124,   124,   124,   124,   124,   124,   124,   125,   125,
     126,   127,   127,   127,   128,   128,   129,   129,   129,   129,
     129,   129,   129,   129,   130,   131,   132,   132,   132,   133,
     133,   133,   134,   135,   135,   135,   136,   136,   137,   137,
     138,   139,   139,   139,   139,   139,   139,   139,   139,   139,
     139,   139,   139,   139,   139,   139,   139,   139,   139,   139,
     139,   139,   139,   139,   139,   139,   139,   139,   139,   139,
     139,   139,   139,   139,   139,   139,   139,   139,   139,   139,
     139,   139,   139,   139,   139,   139,   139,   139,   139,   140,
     140,   141,   141,   141,   141,   141,   141,   141,   141,   141,
     141,   141,   142,   142,   143,   143,   144,   144,   145,   145,
     145,   146,   146,   146,   146,   146,   146,   146,   146,   146,
     146,   147,   148,   149
};

/* YYR2[YYN] -- Number of symbols composing right hand side of rule YYN.  */
static const yytype_uint8 yyr2[] =
{
       0,     2,     0,     2,     1,     1,     2,     1,     1,     1,
       1,     1,     1,     1,     1,     6,     5,     4,     5,     5,
       6,     2,     2,     2,     0,     1,     0,     1,     1,     1,
       3,     7,     1,     1,     3,     1,     3,     1,     1,     9,
       9,     8,     8,     8,     8,    12,     9,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     2,     2,
       1,     1,     2,     4,     4,     3,     3,     4,     1,     2,
       1,     1,     2,     2,     1,     2,    13,    13,    15,    16,
      13,    13,     7,     6,     1,     1,     9,     9,     7,     9,
       9,     6,     1,     1,     1,     1,     2,     2,     1,     3,
       3,     1,     1,     1,     1,     3,     1,     3,     3,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     3,     1,     1,
       1,     1,     1,     3,     1,     1,     1,     1,     1,     1,
       1,     1,     1,     1,     1,     1,     1,     1,     1,     1,
       2,     8,     8,     8,     8,     8,     8,     5,     6,     6,
       6,     5,     1,     1,     1,     1,     1,     1,     1,     1,
       1,     8,     9,     7,     4,     7,     6,     9,     8,     7,
       6,     1,     1,     1
};

/* YYDEFACT[STATE-NAME] -- Default rule to reduce with in state
   STATE-NUM when YYTABLE doesn't specify something else to do.  Zero
   means the default is an error.  */
static const yytype_uint8 yydefact[] =
{
       2,     0,     1,     0,   109,     0,   110,   111,   112,   113,
     114,   115,   116,     0,   118,   119,     0,   120,   121,   122,
     123,   124,   125,     0,   126,   128,   129,   130,   131,   132,
     136,   134,   135,     0,   137,   138,   139,   140,   141,   142,
     143,   144,   145,   146,   147,   148,     4,   101,    98,   103,
     102,     3,     5,     7,     8,     9,    10,    11,    14,   104,
     106,   149,     0,    12,    13,     6,     0,     0,     0,    22,
      21,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,    23,     0,     0,     0,    96,   103,   150,    97,
       0,    30,   117,     0,     0,    47,     0,     0,    24,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    26,     0,
       0,   127,   133,    98,    99,   100,   107,   108,   105,    32,
      33,    35,     0,     0,     0,     0,     0,   174,    25,    26,
       0,     0,     0,     0,     0,    17,     0,     0,     0,     0,
       0,    27,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,    18,     0,
       0,   157,     0,     0,     0,     0,     0,    19,    16,   161,
       0,    34,    36,    38,    37,     0,     0,     0,     0,   176,
       0,   180,   181,     0,     0,    15,     0,     0,     0,     0,
       0,     0,     0,    29,    28,     0,     0,     0,     0,     0,
       0,    85,     0,     0,     0,     0,     0,     0,    91,   160,
     158,   159,    83,     0,    20,    31,    52,    51,     0,    54,
      53,     0,     0,     0,     0,     0,   173,     0,     0,     0,
       0,   168,   169,   170,     0,   167,   166,     0,     0,     0,
       0,   175,   164,   165,     0,   162,   163,     0,     0,     0,
      82,     0,   179,     0,     0,     0,    88,    41,    42,   178,
      48,     0,   171,   182,     0,    61,     0,     0,     0,     0,
       0,    68,     0,    71,     0,     0,    74,     0,    92,     0,
      95,    93,    94,     0,   156,   153,     0,     0,   183,     0,
     152,   151,     0,   155,   154,    43,    44,     0,    46,   172,
       0,     0,     0,    55,    56,    57,    62,     0,     0,     0,
      69,    39,    73,    70,    72,    75,    40,    89,    90,    87,
      86,   177,    49,     0,     0,     0,    60,    66,     0,     0,
       0,    65,     0,     0,     0,     0,    64,    67,     0,    63,
      50,     0,     0,    59,    58,     0,     0,     0,     0,    45,
       0,    77,    76,    80,     0,    81,    84,     0,     0,    78,
      79
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_int16 yydefgoto[] =
{
      -1,     1,    51,    52,   129,   142,   206,    53,   120,   121,
     122,   185,    54,    96,   271,   333,   351,   228,   231,   316,
     346,   337,   281,   282,   324,   286,   287,    55,   367,   212,
      56,    57,   289,   293,    58,    59,    60,    61,    62,    63,
     257,   254,   247,   244,    64,   193,   274,   299
};

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
#define YYPACT_NINF -256
static const yytype_int16 yypact[] =
{
    -256,     1,  -256,   -64,  -256,   129,  -256,  -256,  -256,  -256,
    -256,  -256,   -62,   -21,  -256,  -256,    -5,  -256,  -256,  -256,
    -256,  -256,  -256,   141,    20,  -256,  -256,  -256,  -256,    47,
    -256,  -256,  -256,    12,  -256,  -256,  -256,  -256,  -256,  -256,
    -256,  -256,  -256,  -256,  -256,  -256,  -256,  -256,    76,    75,
    -256,  -256,  -256,  -256,  -256,  -256,  -256,  -256,  -256,  -256,
    -256,    95,    79,  -256,  -256,  -256,   121,    87,    89,  -256,
    -256,   177,   139,   122,   166,   196,   102,   153,    48,   204,
     134,   146,  -256,   149,   155,   127,  -256,  -256,  -256,  -256,
     157,  -256,  -256,   199,   122,  -256,    14,   160,   234,   203,
     205,   122,   206,   -20,   214,   208,    41,   218,   167,   181,
     212,  -256,  -256,   164,  -256,  -256,  -256,  -256,  -256,   168,
    -256,   169,   220,   122,    24,   207,   244,  -256,  -256,   167,
     122,   122,   211,   122,   180,  -256,   221,   122,   183,    57,
     184,  -256,   185,   186,   122,   187,   157,    43,   142,    61,
     235,   189,   191,   253,   193,   171,   250,   242,   243,    -4,
      15,   -38,   245,   215,   197,   217,   114,   175,  -256,   122,
     201,  -256,   209,   210,   213,   216,   246,  -256,  -256,  -256,
     219,  -256,  -256,  -256,  -256,   222,     4,    10,   251,  -256,
     236,  -256,  -256,    58,   223,  -256,     4,    10,    -2,   195,
       2,   254,   257,  -256,  -256,   259,   224,   118,   194,   241,
     114,  -256,   225,   114,   226,     4,    10,   227,  -256,  -256,
    -256,  -256,  -256,   228,  -256,  -256,  -256,  -256,   229,  -256,
    -256,   230,   231,   232,   237,   233,  -256,   161,   125,   238,
      71,  -256,  -256,  -256,   240,  -256,  -256,   247,   239,   248,
     249,  -256,  -256,  -256,   255,  -256,  -256,   256,   232,   258,
    -256,   260,  -256,   261,   262,     4,  -256,  -256,  -256,  -256,
    -256,   263,  -256,  -256,   264,  -256,    11,   265,   110,    10,
     107,    25,   266,  -256,   267,   252,   125,   268,  -256,   269,
    -256,  -256,  -256,   271,  -256,  -256,   272,   273,  -256,   274,
    -256,  -256,   275,  -256,  -256,  -256,  -256,   270,  -256,  -256,
     276,   277,   280,  -256,  -256,  -256,  -256,   279,   278,   277,
    -256,  -256,  -256,  -256,  -256,  -256,  -256,  -256,  -256,  -256,
    -256,  -256,  -256,   281,    10,   -44,  -256,  -256,   232,   282,
     -44,  -256,   283,   284,   285,   287,  -256,  -256,    37,  -256,
    -256,   286,   288,  -256,  -256,   289,   291,   292,   296,  -256,
     294,  -256,  -256,  -256,   295,  -256,  -256,    65,   297,  -256,
    -256
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int16 yypgoto[] =
{
    -256,  -256,  -256,  -256,  -256,   198,   -97,  -256,  -256,  -256,
     163,  -256,  -256,    99,  -255,  -256,  -256,  -181,  -197,  -256,
     -28,    -1,  -256,    42,  -256,  -256,    49,  -256,  -256,  -256,
    -256,  -256,  -256,  -256,  -256,   140,  -256,  -256,   305,  -256,
    -256,  -256,  -256,  -256,  -256,  -256,  -256,  -256
};

/* YYTABLE[YYPACT[STATE-NUM]].  What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule which
   number is the opposite.  If zero, do what YYDEFACT says.
   If YYTABLE_NINF, syntax error.  */
#define YYTABLE_NINF -1
static const yytype_uint16 yytable[] =
{
     238,     2,     3,   302,     4,   344,   207,   226,     5,   134,
       6,     7,     8,   229,   201,   237,   239,     9,    10,   264,
      11,    12,    13,    14,    15,   208,    16,    65,    17,   275,
      18,   203,    19,    20,   263,   204,   245,    21,    22,    68,
     125,    23,   240,   276,    24,    25,    26,   310,    27,   183,
      28,    29,   149,   311,   355,   345,   356,    30,   108,   246,
      31,    32,    33,    34,    35,    36,    37,   172,   173,   214,
      69,   135,   202,    38,    39,    40,   290,    41,    42,   150,
     277,   278,   317,   347,   307,   174,    70,    43,   126,    44,
      45,   184,    46,    47,    48,    49,   227,    50,     4,   109,
     205,   280,   230,    82,     6,     7,     8,   151,   357,   291,
      99,     9,    10,   259,    11,    12,   261,    14,    15,   358,
     138,    80,    17,   234,    18,   292,    19,    20,   175,   283,
     203,    21,    22,    95,   204,   100,   368,   343,    24,    25,
      26,   176,    27,   318,    28,    29,   188,   235,    81,   319,
      71,    30,   189,   101,    31,    32,   369,    34,    35,    36,
      37,   102,    90,   103,   284,   275,    86,    38,    39,    40,
      89,    41,    42,    72,    83,   285,    84,    85,    91,   276,
     104,    43,    92,    44,    45,    93,   105,    47,    48,    87,
      94,    50,    73,   124,    97,   313,    74,   314,   252,   253,
     132,    66,   315,    67,   106,   139,    98,   241,    75,   242,
     243,   186,   187,    76,    95,    77,   277,   278,    78,   116,
     113,   117,   148,   114,   157,   118,   111,    79,   107,   155,
     156,   279,   167,   158,   110,   159,   170,   280,   112,   160,
     196,   197,   113,   180,   215,   216,   255,   256,   115,   119,
     123,   127,   161,   128,   130,   140,   131,   133,   136,   137,
     143,   141,    83,   144,   147,   145,   162,   146,   217,   163,
     153,   168,   169,   152,   171,   177,   178,   179,   190,   181,
     191,   194,   164,   192,   195,   198,   199,   200,   209,   211,
     223,   165,   218,   210,   166,   213,   265,   232,   233,   248,
     219,   220,   249,   258,   221,   250,   339,   222,   312,   182,
     224,   352,   349,   225,   236,   251,   260,   262,   341,   266,
     267,   268,   269,   320,   270,   273,   348,   154,   272,     0,
     288,   294,   360,     0,   296,   325,     0,     0,   295,     0,
     334,   298,   338,   297,   323,     0,   300,   301,     0,   303,
       0,   304,   305,   306,   308,   309,     0,   321,     0,   326,
     327,   322,   328,   329,   330,   331,    88,   332,     0,   336,
       0,     0,   335,     0,   340,   350,     0,   359,   342,   354,
     361,   353,   362,   363,   364,   365,     0,   366,   370
};

static const yytype_int16 yycheck[] =
{
     197,     0,     1,   258,     3,    49,    44,     3,     7,    29,
       9,    10,    11,     3,    18,   196,    18,    16,    17,   216,
      19,    20,    21,    22,    23,    63,    25,    91,    27,     4,
      29,    16,    31,    32,   215,    20,    34,    36,    37,   101,
      26,    40,    44,    18,    43,    44,    45,    36,    47,     6,
      49,    50,    28,    42,    17,    99,    19,    56,    10,    57,
      59,    60,    61,    62,    63,    64,    65,    10,    11,   166,
      91,    91,    76,    72,    73,    74,     5,    76,    77,    55,
      55,    56,   279,   338,   265,    28,    91,    86,    74,    88,
      89,    48,    91,    92,    93,    94,    92,    96,     3,    51,
      85,    76,    92,    91,     9,    10,    11,    83,    71,    38,
       8,    16,    17,   210,    19,    20,   213,    22,    23,    82,
      79,   101,    27,    65,    29,    54,    31,    32,    71,     4,
      16,    36,    37,    92,    20,    33,    71,   334,    43,    44,
      45,    84,    47,    36,    49,    50,    85,    89,   101,    42,
       9,    56,    91,    51,    59,    60,    91,    62,    63,    64,
      65,     8,    41,    10,    39,     4,    91,    72,    73,    74,
      91,    76,    77,    32,    98,    50,   100,   101,    91,    18,
      27,    86,    93,    88,    89,     8,    33,    92,    93,    94,
      51,    96,    51,    94,    28,    85,    55,    87,    80,    81,
     101,    72,    92,    74,    51,   106,    10,    12,    67,    14,
      15,    69,    70,    72,    92,    74,    55,    56,    77,    92,
      93,    94,   123,    83,    13,    85,    92,    86,    75,   130,
     131,    70,   133,    22,    30,    24,   137,    76,    92,    28,
      69,    70,    93,   144,    69,    70,    52,    53,    93,    92,
      51,    91,    41,    19,    51,    37,    51,    51,    44,    51,
      79,    94,    98,    51,    44,    97,    55,    98,   169,    58,
      26,    91,    51,    66,    91,    91,    91,    91,    43,    92,
      91,    28,    71,    92,    91,    35,    44,    44,    43,    92,
      44,    80,    91,    78,    83,    78,    69,    46,    62,    45,
      91,    91,    45,    62,    91,    46,    27,    91,    43,   146,
      91,    27,   340,    91,    91,    91,    91,    91,   319,    91,
      91,    91,    91,   281,    92,    92,    44,   129,    91,    -1,
      92,    91,    44,    -1,    95,   286,    -1,    -1,    91,    -1,
      70,    92,    62,    95,    92,    -1,    91,    91,    -1,    91,
      -1,    91,    91,    91,    91,    91,    -1,    91,    -1,    91,
      91,    94,    91,    91,    91,    91,    61,    92,    -1,    92,
      -1,    -1,    96,    -1,    96,    92,    -1,    91,    97,    92,
      91,    96,    91,    91,    88,    91,    -1,    92,    91
};

/* YYSTOS[STATE-NUM] -- The (internal number of the) accessing
   symbol of state STATE-NUM.  */
static const yytype_uint8 yystos[] =
{
       0,   103,     0,     1,     3,     7,     9,    10,    11,    16,
      17,    19,    20,    21,    22,    23,    25,    27,    29,    31,
      32,    36,    37,    40,    43,    44,    45,    47,    49,    50,
      56,    59,    60,    61,    62,    63,    64,    65,    72,    73,
      74,    76,    77,    86,    88,    89,    91,    92,    93,    94,
      96,   104,   105,   109,   114,   129,   132,   133,   136,   137,
     138,   139,   140,   141,   146,    91,    72,    74,   101,    91,
      91,     9,    32,    51,    55,    67,    72,    74,    77,    86,
     101,   101,    91,    98,   100,   101,    91,    94,   140,    91,
      41,    91,    93,     8,    51,    92,   115,    28,    10,     8,
      33,    51,     8,    10,    27,    33,    51,    75,    10,    51,
      30,    92,    92,    93,   137,    93,    92,    94,   137,    92,
     110,   111,   112,    51,   115,    26,    74,    91,    19,   106,
      51,    51,   115,    51,    29,    91,    44,    51,    79,   115,
      37,    94,   107,    79,    51,    97,    98,    44,   115,    28,
      55,    83,    66,    26,   107,   115,   115,    13,    22,    24,
      28,    41,    55,    58,    71,    80,    83,   115,    91,    51,
     115,    91,    10,    11,    28,    71,    84,    91,    91,    91,
     115,    92,   112,     6,    48,   113,    69,    70,    85,    91,
      43,    91,    92,   147,    28,    91,    69,    70,    35,    44,
      44,    18,    76,    16,    20,    85,   108,    44,    63,    43,
      78,    92,   131,    78,   108,    69,    70,   115,    91,    91,
      91,    91,    91,    44,    91,    91,     3,    92,   119,     3,
      92,   120,    46,    62,    65,    89,    91,   119,   120,    18,
      44,    12,    14,    15,   145,    34,    57,   144,    45,    45,
      46,    91,    80,    81,   143,    52,    53,   142,    62,   108,
      91,   108,    91,   119,   120,    69,    91,    91,    91,    91,
      92,   116,    91,    92,   148,     4,    18,    55,    56,    70,
      76,   124,   125,     4,    39,    50,   127,   128,    92,   134,
       5,    38,    54,   135,    91,    91,    95,    95,    92,   149,
      91,    91,   116,    91,    91,    91,    91,   119,    91,    91,
      36,    42,    43,    85,    87,    92,   121,   120,    36,    42,
     125,    91,    94,    92,   126,   128,    91,    91,    91,    91,
      91,    91,    92,   117,    70,    96,    92,   123,    62,    27,
      96,   123,    97,   120,    49,    99,   122,   116,    44,   122,
      92,   118,    27,    96,    92,    17,    19,    71,    82,    91,
      44,    91,    91,    91,    88,    91,    92,   130,    71,    91,
      91
};

#define yyerrok		(yyerrstatus = 0)
#define yyclearin	(yychar = YYEMPTY)
#define YYEMPTY		(-2)
#define YYEOF		0

#define YYACCEPT	goto yyacceptlab
#define YYABORT		goto yyabortlab
#define YYERROR		goto yyerrorlab


/* Like YYERROR except do call yyerror.  This remains here temporarily
   to ease the transition to the new meaning of YYERROR, for GCC.
   Once GCC version 2 has supplanted version 1, this can go.  */

#define YYFAIL		goto yyerrlab

#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)					\
do								\
  if (yychar == YYEMPTY && yylen == 1)				\
    {								\
      yychar = (Token);						\
      yylval = (Value);						\
      yytoken = YYTRANSLATE (yychar);				\
      YYPOPSTACK (1);						\
      goto yybackup;						\
    }								\
  else								\
    {								\
      yyerror (YY_("syntax error: cannot back up")); \
      YYERROR;							\
    }								\
while (YYID (0))


#define YYTERROR	1
#define YYERRCODE	256


/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#define YYRHSLOC(Rhs, K) ((Rhs)[K])
#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)				\
    do									\
      if (YYID (N))                                                    \
	{								\
	  (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;	\
	  (Current).first_column = YYRHSLOC (Rhs, 1).first_column;	\
	  (Current).last_line    = YYRHSLOC (Rhs, N).last_line;		\
	  (Current).last_column  = YYRHSLOC (Rhs, N).last_column;	\
	}								\
      else								\
	{								\
	  (Current).first_line   = (Current).last_line   =		\
	    YYRHSLOC (Rhs, 0).last_line;				\
	  (Current).first_column = (Current).last_column =		\
	    YYRHSLOC (Rhs, 0).last_column;				\
	}								\
    while (YYID (0))
#endif


/* YY_LOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

#ifndef YY_LOCATION_PRINT
# if YYLTYPE_IS_TRIVIAL
#  define YY_LOCATION_PRINT(File, Loc)			\
     fprintf (File, "%d.%d-%d.%d",			\
	      (Loc).first_line, (Loc).first_column,	\
	      (Loc).last_line,  (Loc).last_column)
# else
#  define YY_LOCATION_PRINT(File, Loc) ((void) 0)
# endif
#endif


/* YYLEX -- calling `yylex' with the right arguments.  */

#ifdef YYLEX_PARAM
# define YYLEX yylex (&yylval, YYLEX_PARAM)
#else
# define YYLEX yylex (&yylval)
#endif

/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)			\
do {						\
  if (yydebug)					\
    YYFPRINTF Args;				\
} while (YYID (0))

# define YY_SYMBOL_PRINT(Title, Type, Value, Location)			  \
do {									  \
  if (yydebug)								  \
    {									  \
      YYFPRINTF (stderr, "%s ", Title);					  \
      yy_symbol_print (stderr,						  \
		  Type, Value); \
      YYFPRINTF (stderr, "\n");						  \
    }									  \
} while (YYID (0))


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_value_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_value_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (!yyvaluep)
    return;
# ifdef YYPRINT
  if (yytype < YYNTOKENS)
    YYPRINT (yyoutput, yytoknum[yytype], *yyvaluep);
# else
  YYUSE (yyoutput);
# endif
  switch (yytype)
    {
      default:
	break;
    }
}


/*--------------------------------.
| Print this symbol on YYOUTPUT.  |
`--------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_symbol_print (FILE *yyoutput, int yytype, YYSTYPE const * const yyvaluep)
#else
static void
yy_symbol_print (yyoutput, yytype, yyvaluep)
    FILE *yyoutput;
    int yytype;
    YYSTYPE const * const yyvaluep;
#endif
{
  if (yytype < YYNTOKENS)
    YYFPRINTF (yyoutput, "token %s (", yytname[yytype]);
  else
    YYFPRINTF (yyoutput, "nterm %s (", yytname[yytype]);

  yy_symbol_value_print (yyoutput, yytype, yyvaluep);
  YYFPRINTF (yyoutput, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_stack_print (yytype_int16 *bottom, yytype_int16 *top)
#else
static void
yy_stack_print (bottom, top)
    yytype_int16 *bottom;
    yytype_int16 *top;
#endif
{
  YYFPRINTF (stderr, "Stack now");
  for (; bottom <= top; ++bottom)
    YYFPRINTF (stderr, " %d", *bottom);
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)				\
do {								\
  if (yydebug)							\
    yy_stack_print ((Bottom), (Top));				\
} while (YYID (0))


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yy_reduce_print (YYSTYPE *yyvsp, int yyrule)
#else
static void
yy_reduce_print (yyvsp, yyrule)
    YYSTYPE *yyvsp;
    int yyrule;
#endif
{
  int yynrhs = yyr2[yyrule];
  int yyi;
  unsigned long int yylno = yyrline[yyrule];
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %lu):\n",
	     yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      fprintf (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr, yyrhs[yyprhs[yyrule] + yyi],
		       &(yyvsp[(yyi + 1) - (yynrhs)])
		       		       );
      fprintf (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)		\
do {					\
  if (yydebug)				\
    yy_reduce_print (yyvsp, Rule); \
} while (YYID (0))

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args)
# define YY_SYMBOL_PRINT(Title, Type, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef	YYINITDEPTH
# define YYINITDEPTH 200
#endif

/* YYMAXDEPTH -- maximum size the stacks can grow to (effective only
   if the built-in stack extension method is used).

   Do not make this value too large; the results are undefined if
   YYSTACK_ALLOC_MAXIMUM < YYSTACK_BYTES (YYMAXDEPTH)
   evaluated with infinite-precision integer arithmetic.  */

#ifndef YYMAXDEPTH
# define YYMAXDEPTH 10000
#endif



#if YYERROR_VERBOSE

# ifndef yystrlen
#  if defined __GLIBC__ && defined _STRING_H
#   define yystrlen strlen
#  else
/* Return the length of YYSTR.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static YYSIZE_T
yystrlen (const char *yystr)
#else
static YYSIZE_T
yystrlen (yystr)
    const char *yystr;
#endif
{
  YYSIZE_T yylen;
  for (yylen = 0; yystr[yylen]; yylen++)
    continue;
  return yylen;
}
#  endif
# endif

# ifndef yystpcpy
#  if defined __GLIBC__ && defined _STRING_H && defined _GNU_SOURCE
#   define yystpcpy stpcpy
#  else
/* Copy YYSRC to YYDEST, returning the address of the terminating '\0' in
   YYDEST.  */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static char *
yystpcpy (char *yydest, const char *yysrc)
#else
static char *
yystpcpy (yydest, yysrc)
    char *yydest;
    const char *yysrc;
#endif
{
  char *yyd = yydest;
  const char *yys = yysrc;

  while ((*yyd++ = *yys++) != '\0')
    continue;

  return yyd - 1;
}
#  endif
# endif

# ifndef yytnamerr
/* Copy to YYRES the contents of YYSTR after stripping away unnecessary
   quotes and backslashes, so that it's suitable for yyerror.  The
   heuristic is that double-quoting is unnecessary unless the string
   contains an apostrophe, a comma, or backslash (other than
   backslash-backslash).  YYSTR is taken from yytname.  If YYRES is
   null, do not copy; instead, return the length of what the result
   would have been.  */
static YYSIZE_T
yytnamerr (char *yyres, const char *yystr)
{
  if (*yystr == '"')
    {
      YYSIZE_T yyn = 0;
      char const *yyp = yystr;

      for (;;)
	switch (*++yyp)
	  {
	  case '\'':
	  case ',':
	    goto do_not_strip_quotes;

	  case '\\':
	    if (*++yyp != '\\')
	      goto do_not_strip_quotes;
	    /* Fall through.  */
	  default:
	    if (yyres)
	      yyres[yyn] = *yyp;
	    yyn++;
	    break;

	  case '"':
	    if (yyres)
	      yyres[yyn] = '\0';
	    return yyn;
	  }
    do_not_strip_quotes: ;
    }

  if (! yyres)
    return yystrlen (yystr);

  return yystpcpy (yyres, yystr) - yyres;
}
# endif

/* Copy into YYRESULT an error message about the unexpected token
   YYCHAR while in state YYSTATE.  Return the number of bytes copied,
   including the terminating null byte.  If YYRESULT is null, do not
   copy anything; just return the number of bytes that would be
   copied.  As a special case, return 0 if an ordinary "syntax error"
   message will do.  Return YYSIZE_MAXIMUM if overflow occurs during
   size calculation.  */
static YYSIZE_T
yysyntax_error (char *yyresult, int yystate, int yychar)
{
  int yyn = yypact[yystate];

  if (! (YYPACT_NINF < yyn && yyn <= YYLAST))
    return 0;
  else
    {
      int yytype = YYTRANSLATE (yychar);
      YYSIZE_T yysize0 = yytnamerr (0, yytname[yytype]);
      YYSIZE_T yysize = yysize0;
      YYSIZE_T yysize1;
      int yysize_overflow = 0;
      enum { YYERROR_VERBOSE_ARGS_MAXIMUM = 5 };
      char const *yyarg[YYERROR_VERBOSE_ARGS_MAXIMUM];
      int yyx;

# if 0
      /* This is so xgettext sees the translatable formats that are
	 constructed on the fly.  */
      YY_("syntax error, unexpected %s");
      YY_("syntax error, unexpected %s, expecting %s");
      YY_("syntax error, unexpected %s, expecting %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s");
      YY_("syntax error, unexpected %s, expecting %s or %s or %s or %s");
# endif
      char *yyfmt;
      char const *yyf;
      static char const yyunexpected[] = "syntax error, unexpected %s";
      static char const yyexpecting[] = ", expecting %s";
      static char const yyor[] = " or %s";
      char yyformat[sizeof yyunexpected
		    + sizeof yyexpecting - 1
		    + ((YYERROR_VERBOSE_ARGS_MAXIMUM - 2)
		       * (sizeof yyor - 1))];
      char const *yyprefix = yyexpecting;

      /* Start YYX at -YYN if negative to avoid negative indexes in
	 YYCHECK.  */
      int yyxbegin = yyn < 0 ? -yyn : 0;

      /* Stay within bounds of both yycheck and yytname.  */
      int yychecklim = YYLAST - yyn + 1;
      int yyxend = yychecklim < YYNTOKENS ? yychecklim : YYNTOKENS;
      int yycount = 1;

      yyarg[0] = yytname[yytype];
      yyfmt = yystpcpy (yyformat, yyunexpected);

      for (yyx = yyxbegin; yyx < yyxend; ++yyx)
	if (yycheck[yyx + yyn] == yyx && yyx != YYTERROR)
	  {
	    if (yycount == YYERROR_VERBOSE_ARGS_MAXIMUM)
	      {
		yycount = 1;
		yysize = yysize0;
		yyformat[sizeof yyunexpected - 1] = '\0';
		break;
	      }
	    yyarg[yycount++] = yytname[yyx];
	    yysize1 = yysize + yytnamerr (0, yytname[yyx]);
	    yysize_overflow |= (yysize1 < yysize);
	    yysize = yysize1;
	    yyfmt = yystpcpy (yyfmt, yyprefix);
	    yyprefix = yyor;
	  }

      yyf = YY_(yyformat);
      yysize1 = yysize + yystrlen (yyf);
      yysize_overflow |= (yysize1 < yysize);
      yysize = yysize1;

      if (yysize_overflow)
	return YYSIZE_MAXIMUM;

      if (yyresult)
	{
	  /* Avoid sprintf, as that infringes on the user's name space.
	     Don't have undefined behavior even if the translation
	     produced a string with the wrong number of "%s"s.  */
	  char *yyp = yyresult;
	  int yyi = 0;
	  while ((*yyp = *yyf) != '\0')
	    {
	      if (*yyp == '%' && yyf[1] == 's' && yyi < yycount)
		{
		  yyp += yytnamerr (yyp, yyarg[yyi++]);
		  yyf += 2;
		}
	      else
		{
		  yyp++;
		  yyf++;
		}
	    }
	}
      return yysize;
    }
}
#endif /* YYERROR_VERBOSE */


/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

/*ARGSUSED*/
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
static void
yydestruct (const char *yymsg, int yytype, YYSTYPE *yyvaluep)
#else
static void
yydestruct (yymsg, yytype, yyvaluep)
    const char *yymsg;
    int yytype;
    YYSTYPE *yyvaluep;
#endif
{
  YYUSE (yyvaluep);

  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yytype, yyvaluep, yylocationp);

  switch (yytype)
    {

      default:
	break;
    }
}


/* Prevent warnings from -Wmissing-prototypes.  */

#ifdef YYPARSE_PARAM
#if defined __STDC__ || defined __cplusplus
int yyparse (void *YYPARSE_PARAM);
#else
int yyparse ();
#endif
#else /* ! YYPARSE_PARAM */
#if defined __STDC__ || defined __cplusplus
int yyparse (void);
#else
int yyparse ();
#endif
#endif /* ! YYPARSE_PARAM */






/*----------.
| yyparse.  |
`----------*/

#ifdef YYPARSE_PARAM
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void *YYPARSE_PARAM)
#else
int
yyparse (YYPARSE_PARAM)
    void *YYPARSE_PARAM;
#endif
#else /* ! YYPARSE_PARAM */
#if (defined __STDC__ || defined __C99__FUNC__ \
     || defined __cplusplus || defined _MSC_VER)
int
yyparse (void)
#else
int
yyparse ()

#endif
#endif
{
  /* The look-ahead symbol.  */
int yychar;

/* The semantic value of the look-ahead symbol.  */
YYSTYPE yylval;

/* Number of syntax errors so far.  */
int yynerrs;

  int yystate;
  int yyn;
  int yyresult;
  /* Number of tokens to shift before error messages enabled.  */
  int yyerrstatus;
  /* Look-ahead token as an internal (translated) token number.  */
  int yytoken = 0;
#if YYERROR_VERBOSE
  /* Buffer for error messages, and its allocated size.  */
  char yymsgbuf[128];
  char *yymsg = yymsgbuf;
  YYSIZE_T yymsg_alloc = sizeof yymsgbuf;
#endif

  /* Three stacks and their tools:
     `yyss': related to states,
     `yyvs': related to semantic values,
     `yyls': related to locations.

     Refer to the stacks thru separate pointers, to allow yyoverflow
     to reallocate them elsewhere.  */

  /* The state stack.  */
  yytype_int16 yyssa[YYINITDEPTH];
  yytype_int16 *yyss = yyssa;
  yytype_int16 *yyssp;

  /* The semantic value stack.  */
  YYSTYPE yyvsa[YYINITDEPTH];
  YYSTYPE *yyvs = yyvsa;
  YYSTYPE *yyvsp;



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N))

  YYSIZE_T yystacksize = YYINITDEPTH;

  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;


  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yystate = 0;
  yyerrstatus = 0;
  yynerrs = 0;
  yychar = YYEMPTY;		/* Cause a token to be read.  */

  /* Initialize stack pointers.
     Waste one element of value and location stack
     so that they stay on the same level as the state stack.
     The wasted elements are never initialized.  */

  yyssp = yyss;
  yyvsp = yyvs;

  goto yysetstate;

/*------------------------------------------------------------.
| yynewstate -- Push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
 yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;

 yysetstate:
  *yyssp = yystate;

  if (yyss + yystacksize - 1 <= yyssp)
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYSIZE_T yysize = yyssp - yyss + 1;

#ifdef yyoverflow
      {
	/* Give user a chance to reallocate the stack.  Use copies of
	   these so that the &'s don't force the real ones into
	   memory.  */
	YYSTYPE *yyvs1 = yyvs;
	yytype_int16 *yyss1 = yyss;


	/* Each stack pointer address is followed by the size of the
	   data in use in that stack, in bytes.  This used to be a
	   conditional around just the two extra args, but that might
	   be undefined if yyoverflow is a macro.  */
	yyoverflow (YY_("memory exhausted"),
		    &yyss1, yysize * sizeof (*yyssp),
		    &yyvs1, yysize * sizeof (*yyvsp),

		    &yystacksize);

	yyss = yyss1;
	yyvs = yyvs1;
      }
#else /* no yyoverflow */
# ifndef YYSTACK_RELOCATE
      goto yyexhaustedlab;
# else
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
	goto yyexhaustedlab;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
	yystacksize = YYMAXDEPTH;

      {
	yytype_int16 *yyss1 = yyss;
	union yyalloc *yyptr =
	  (union yyalloc *) YYSTACK_ALLOC (YYSTACK_BYTES (yystacksize));
	if (! yyptr)
	  goto yyexhaustedlab;
	YYSTACK_RELOCATE (yyss);
	YYSTACK_RELOCATE (yyvs);

#  undef YYSTACK_RELOCATE
	if (yyss1 != yyssa)
	  YYSTACK_FREE (yyss1);
      }
# endif
#endif /* no yyoverflow */

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;


      YYDPRINTF ((stderr, "Stack size increased to %lu\n",
		  (unsigned long int) yystacksize));

      if (yyss + yystacksize - 1 <= yyssp)
	YYABORT;
    }

  YYDPRINTF ((stderr, "Entering state %d\n", yystate));

  goto yybackup;

/*-----------.
| yybackup.  |
`-----------*/
yybackup:

  /* Do appropriate processing given the current state.  Read a
     look-ahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to look-ahead token.  */
  yyn = yypact[yystate];
  if (yyn == YYPACT_NINF)
    goto yydefault;

  /* Not known => get a look-ahead token if don't already have one.  */

  /* YYCHAR is either YYEMPTY or YYEOF or a valid look-ahead symbol.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token: "));
      yychar = YYLEX;
    }

  if (yychar <= YYEOF)
    {
      yychar = yytoken = YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else
    {
      yytoken = YYTRANSLATE (yychar);
      YY_SYMBOL_PRINT ("Next token is", yytoken, &yylval, &yylloc);
    }

  /* If the proper action on seeing token YYTOKEN is to reduce or to
     detect an error, take that action.  */
  yyn += yytoken;
  if (yyn < 0 || YYLAST < yyn || yycheck[yyn] != yytoken)
    goto yydefault;
  yyn = yytable[yyn];
  if (yyn <= 0)
    {
      if (yyn == 0 || yyn == YYTABLE_NINF)
	goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the look-ahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);

  /* Discard the shifted token unless it is eof.  */
  if (yychar != YYEOF)
    yychar = YYEMPTY;

  yystate = yyn;
  *++yyvsp = yylval;

  goto yynewstate;


/*-----------------------------------------------------------.
| yydefault -- do the default action for the current state.  |
`-----------------------------------------------------------*/
yydefault:
  yyn = yydefact[yystate];
  if (yyn == 0)
    goto yyerrlab;
  goto yyreduce;


/*-----------------------------.
| yyreduce -- Do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     `$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];


  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
        case 3:
#line 124 "parser.y"
    {
        cliFlushInput();
    ;}
    break;

  case 4:
#line 129 "parser.y"
    {
        wl_cleanup(PARSER_PARAMS->wl);
        if (!PARSER_PARAMS->loading_file)
        {
            //printf(CLI_PROMPT);
	    //printf("### ");
            fflush(stdout);
        }
    ;}
    break;

  case 5:
#line 139 "parser.y"
    {
        wl_cleanup(PARSER_PARAMS->wl);
        if (!PARSER_PARAMS->loading_file)
        {
            //printf(CLI_PROMPT);
	    //printf("### ");
            fflush(stdout);
        }
    ;}
    break;

  case 6:
#line 149 "parser.y"
    {
        wl_cleanup(PARSER_PARAMS->wl);
        yyerrok;
        if (!PARSER_PARAMS->loading_file)
        {
            //printf(CLI_PROMPT);
	    //printf("### ");
            fflush(stdout);
        }
    ;}
    break;

  case 15:
#line 171 "parser.y"
    {
        char filename[256];
        
        FILE *fp;
        struct cmdbuffers *pheader, *pnew;
        
        if((pheader = malloc(sizeof(struct cmdbuffers))) == NULL)
        {
            errprint("Out of memory in malloc cmdbuffers.\n");
            return -1;
        }
        
        memset(pheader, 0, sizeof(struct cmdbuffers));
        pheader->next = NULL;
        
        if((yyvsp[(4) - (6)].ival) & OPT_DEFAULT)
            cli_reset_default_config(PARSER_PARAMS);
      	else
      	{
        	cli_reset_default_config(PARSER_PARAMS);
        	
        	if ((yyvsp[(5) - (6)].wlptr) == NULL)
          		strcpy(filename, DEFAULT_CONFIG_FILE);
        	else
        	{
          		strcpy(filename, WL_PTR((yyvsp[(5) - (6)].wlptr)));
          		wl_free_entry((yyvsp[(5) - (6)].wlptr));
        	}
			
        	if((fp = fopen(filename,"r")) == NULL)
        	{
          		errprint("Can't open %s to read.\n", filename);
          		break;
        	}
        	else
        	{
          		while(!feof(fp))
          		{
            		if((pnew = malloc(sizeof(struct cmdbuffers))) == NULL)
            		{ 
              			errprint("Out of memory in malloc cmdbuffers.\n");
              			return -1;
            		}
			
            		memset(pnew, 0, sizeof(struct cmdbuffers));
            		if (pheader->next == NULL)
            		{
              			pnew->next = NULL;
              			pheader->next = pnew;
            		}
            		else
            		{
              			pnew->next = pheader->next;
              			pheader->next = pnew;
            		} 
          			
            		fgets(pnew->cmd, 512, fp);
          		}//while
				
          		fclose(fp);
				
          		pnew = pheader->next;
							while(pnew != NULL)
          		{
            		pheader = pnew;
            		set_input_buff(pnew->cmd, NULL, PARSER_PARAMS->scanner);
            		pnew = pnew->next;
            		free(pheader);
          		}//while
        	}//else
      	}
		;}
    break;

  case 16:
#line 244 "parser.y"
    {
    	if((yyvsp[(4) - (5)].wlptr) == NULL)
    	{
      		if(cli_save_config_file(PARSER_PARAMS, DEFAULT_CONFIG_FILE) < 0)
      		{
        		errprint("Error saving configurations.\n");
        		break;
      		}
    	}
    	else
    	{
      		if(cli_save_config_file(PARSER_PARAMS, WL_PTR((yyvsp[(4) - (5)].wlptr))) < 0)
      		{
        		errprint("Error saving configurations to file %s.\n", WL_PTR((yyvsp[(4) - (5)].wlptr)));
        		wl_free_entry((yyvsp[(4) - (5)].wlptr));
        		break;
      		}
      		
      		wl_free_entry((yyvsp[(4) - (5)].wlptr));
    	}
  	;}
    break;

  case 17:
#line 266 "parser.y"
    {
    	if(cli_show_config(PARSER_PARAMS) < 0)
    	{
      		errprint("Could not show configurations.\n");
      		break;
    	}
  	;}
    break;

  case 18:
#line 274 "parser.y"
    {
        if(cli_show_config_file() < 0)
        {
      		errprint("Could not show configurations.\n");
      		break;
    	}
    ;}
    break;

  case 19:
#line 282 "parser.y"
    {
        if (cli_show_system() < 0)
      	{
        	errprint("Could not show system information.\n");
        	break;
      	}
    ;}
    break;

  case 20:
#line 290 "parser.y"
    {
      	if (cli_update_firmware((yyvsp[(5) - (6)].ival)) < 0)
      	{
        	errprint("Could not update firmware.\n");
        	break;
      	}
    ;}
    break;

  case 21:
#line 298 "parser.y"
    {
        if (cli_etxconfig() < 0)
        {
            errprint("Could not edit config file.\n");
            break;
        }
    ;}
    break;

  case 22:
#line 306 "parser.y"
    {
	YYACCEPT;
    ;}
    break;

  case 23:
#line 310 "parser.y"
    {
	YYACCEPT;
    ;}
    break;

  case 24:
#line 316 "parser.y"
    {
        (yyval.ival) = 0;
    ;}
    break;

  case 25:
#line 320 "parser.y"
    {
        (yyval.ival) = OPT_DEFAULT;
    ;}
    break;

  case 26:
#line 325 "parser.y"
    {
        (yyval.wlptr) = NULL;
    ;}
    break;

  case 27:
#line 329 "parser.y"
    {
        (yyval.wlptr) = (yyvsp[(1) - (1)].wlptr);
    ;}
    break;

  case 28:
#line 334 "parser.y"
    {
      	(yyval.ival) = ENABLED;
    ;}
    break;

  case 29:
#line 338 "parser.y"
    {
        (yyval.ival) = DISABLED;
    ;}
    break;

  case 30:
#line 348 "parser.y"
    {
        cli_bypass_get(PARSER_PARAMS);
    ;}
    break;

  case 31:
#line 352 "parser.y"
    {
        cli_bypass_set(PARSER_PARAMS, (yyvsp[(4) - (7)].ival), (yyvsp[(6) - (7)].ival));
    ;}
    break;

  case 32:
#line 357 "parser.y"
    {		   	
        if ( (yyvsp[(1) - (1)].ival) < 0 || (yyvsp[(1) - (1)].ival) > MAX_BYPASS_PORTS-1 )			
        {				
            errprint("Valid port range is: 0 - %d\n", MAX_BYPASS_PORTS-1);				
	    YYERROR;			
	}			
	(yyval.ival) = (yyvsp[(1) - (1)].ival);		
    ;}
    break;

  case 33:
#line 367 "parser.y"
    {		    
	      (yyval.ival) = 1 << (yyvsp[(1) - (1)].ival);	    
	  ;}
    break;

  case 34:
#line 371 "parser.y"
    {		    
	      if( (yyvsp[(1) - (3)].ival) < 0 || (yyvsp[(1) - (3)].ival) > MAX_BYPASS_PORTS-1)		    
		    {			    
		        errprint("Invalid port %d.\n", (yyvsp[(1) - (3)].ival));			    
		        YYERROR;		    
		    }             
	      if((yyvsp[(3) - (3)].ival) < 0 || (yyvsp[(3) - (3)].ival) > MAX_BYPASS_PORTS-1)		    
	      {			    
			errprint("Invalid port %d.\n", (yyvsp[(1) - (3)].ival));			    
		        YYERROR;		    
	      }		    
	      (yyval.ival) = portrange((yyvsp[(1) - (3)].ival), (yyvsp[(3) - (3)].ival));	    
	  ;}
    break;

  case 36:
#line 386 "parser.y"
    {		    
	      (yyval.ival) = (yyvsp[(1) - (3)].ival) | (yyvsp[(3) - (3)].ival);	    
	  ;}
    break;

  case 37:
#line 391 "parser.y"
    {
      	(yyval.ival) = ENABLED;
    ;}
    break;

  case 38:
#line 395 "parser.y"
    {
        (yyval.ival) = DISABLED;
    ;}
    break;

  case 39:
#line 404 "parser.y"
    {
      	rule1options_t opts;
      	
      	memset(&opts, 0, sizeof(rule1options_t));
      	opts.flag = WL_PTR_OF((yyvsp[(8) - (9)].wlptr), rule1options_t)->flag;
	
      	if(opts.flag & RULE_OPT_PROTOCOL)
        	opts.protocol = WL_PTR_OF((yyvsp[(8) - (9)].wlptr), rule1options_t)->protocol;
      	
      	if(opts.flag & RULE_OPT_SRCIP)
      	{
        	opts.sourceip = WL_PTR_OF((yyvsp[(8) - (9)].wlptr), rule1options_t)->sourceip;
        	opts.sourceip_mask = WL_PTR_OF((yyvsp[(8) - (9)].wlptr), rule1options_t)->sourceip_mask;
      	}
      	
      	if(opts.flag & RULE_OPT_DESTIP)
      	{
        	opts.destip = WL_PTR_OF((yyvsp[(8) - (9)].wlptr), rule1options_t)->destip;
        	opts.destip_mask = WL_PTR_OF((yyvsp[(8) - (9)].wlptr), rule1options_t)->destip_mask;
      	}
      	
      	if(opts.flag & RULE_OPT_SRCL4PORT)
        	opts.sourcel4port = WL_PTR_OF((yyvsp[(8) - (9)].wlptr), rule1options_t)->sourcel4port;
      	
      	if(opts.flag & RULE_OPT_DESTL4PORT)
        	opts.destl4port = WL_PTR_OF((yyvsp[(8) - (9)].wlptr), rule1options_t)->destl4port;
      	
      	if(opts.flag&RULE_OPT_PACKETLEN)
        	opts.pktlen_range = WL_PTR_OF((yyvsp[(8) - (9)].wlptr), rule1options_t)->pktlen_range;
      	
      	opts.id = (yyvsp[(7) - (9)].ival);
      	
      	if (opts.id == MAX_RULE_NO)
      	{    
        	errprint("Specify the correct rule index please (0 - %d)\n", MAX_RULE_NO-1);
        	break;
      	}
      	else
        	cli_fpga_set_ruleset1(PARSER_PARAMS, (yyvsp[(5) - (9)].ival), &opts);
    ;}
    break;

  case 40:
#line 445 "parser.y"
    {
      	int i;
      	rule2options_t opts;
      	
      	memset(&opts, 0, sizeof(rule2options_t));
      	opts.flag = WL_PTR_OF((yyvsp[(8) - (9)].wlptr), rule2options_t)->flag;
      	
      	if(opts.flag & RULE_OPT_OFFSET)
        	opts.offset = WL_PTR_OF((yyvsp[(8) - (9)].wlptr), rule2options_t)->offset;
      	
      	if(opts.flag & RULE_OPT_KEY)
      	{
        	for(i=0; i< MAX_KEY_NO; i++)
        	{
          		opts.key[i] = WL_PTR_OF((yyvsp[(8) - (9)].wlptr), rule2options_t)->key[i];
          		opts.mask[i] = WL_PTR_OF((yyvsp[(8) - (9)].wlptr), rule2options_t)->mask[i];
        	}
      	}
        
      	opts.id = (yyvsp[(7) - (9)].ival);
      	if (opts.id == MAX_RULE_NO)
      	{    
        	errprint("Specify the correct rule index please (0 - %d)\n", MAX_RULE_NO-1);
        	break;
      	}
      	else
        	cli_fpga_set_ruleset2(PARSER_PARAMS, (yyvsp[(5) - (9)].ival), &opts);
    ;}
    break;

  case 41:
#line 474 "parser.y"
    {
      	int i;
        
      	if ((yyvsp[(7) - (8)].ival) == MAX_RULE_NO)
      	{
        	for(i=0; i < MAX_RULE_NO; i++)
          		if(cli_fpga_clear_ruleset1(PARSER_PARAMS, (yyvsp[(5) - (8)].ival), i) < 0)
          		{   
            		errprint("Error clear filter rules.\n");
            		break;
          		}
      	}
      	else
      	{    
        	if(cli_fpga_clear_ruleset1(PARSER_PARAMS, (yyvsp[(5) - (8)].ival), (yyvsp[(7) - (8)].ival)) < 0)
        	{   
          		errprint("Error clear filter rules.\n");
          		break;
        	}
      	}
    ;}
    break;

  case 42:
#line 496 "parser.y"
    {
      	int i;
        
      	if ((yyvsp[(7) - (8)].ival) == MAX_RULE_NO)
      	{
        	for(i=0; i < MAX_RULE_NO; i++)
          		if(cli_fpga_clear_ruleset2(PARSER_PARAMS, (yyvsp[(5) - (8)].ival), i) < 0)
          		{   
            		errprint("Error clear filter rules.\n");
            		break;
          		}
      	}
      	else
      	{    
        	if(cli_fpga_clear_ruleset2(PARSER_PARAMS, (yyvsp[(5) - (8)].ival), (yyvsp[(7) - (8)].ival)) < 0)
        	{   
          		errprint("Error clear filter rules.\n");
          		break;
        	}
      	}
    ;}
    break;

  case 43:
#line 518 "parser.y"
    {
      	int i;
            
      	if ((yyvsp[(5) - (8)].ival) == MAX_RULE_NO)
      	{
        	for(i=0; i < MAX_RULE_NO; i++)
        		if(cli_get_filter_ruleset1(PARSER_PARAMS, (yyvsp[(5) - (8)].ival), i) < 0)
        		{   
          			errprint("Error clear filter rules.\n");
          			break;
       	 		}
      	}
      	else
      	{    
        	if(cli_get_filter_ruleset1(PARSER_PARAMS, (yyvsp[(5) - (8)].ival), (yyvsp[(7) - (8)].ival)) < 0)
        	{   
          		errprint("Error clear filter rules.\n");
          		break;
        	}    
      	}
    ;}
    break;

  case 44:
#line 540 "parser.y"
    {
      	int i;
            
      	if ((yyvsp[(5) - (8)].ival) == MAX_RULE_NO)
      	{
        	for(i=0; i < MAX_RULE_NO; i++)
        		if(cli_get_filter_ruleset2(PARSER_PARAMS, (yyvsp[(5) - (8)].ival), i) < 0)
        		{   
          			errprint("Error clear filter rules.\n");
          			break;
        		}
      	}
      	else
      	{    
          	if(cli_get_filter_ruleset2(PARSER_PARAMS, (yyvsp[(5) - (8)].ival), (yyvsp[(7) - (8)].ival)) < 0)
          	{   
            	errprint("Error clear filter rules.\n");
            	break;
          	}    
      	}
    ;}
    break;

  case 45:
#line 562 "parser.y"
    {
      	if ((yyvsp[(9) - (12)].ival) > (yyvsp[(11) - (12)].ival))
      	{
        	errprint("Wrong order of packet length range.\n");
        	break;
      	}
      
      	if (cli_set_pktlen_range(PARSER_PARAMS, (yyvsp[(4) - (12)].ival), (yyvsp[(8) - (12)].ival), (yyvsp[(9) - (12)].ival), (yyvsp[(11) - (12)].ival)) < 0)
      	{
        	errprint("Error set packet length range.\n");
        	break;
      	}
    ;}
    break;

  case 46:
#line 576 "parser.y"
    {
      	if (cli_get_pktlen_range(PARSER_PARAMS, (yyvsp[(4) - (9)].ival), (yyvsp[(8) - (9)].ival)) < 0)
      	{
        	errprint("Error get packet length range.\n");
        	break;
      	}
    ;}
    break;

  case 47:
#line 586 "parser.y"
    {
        if ((yyvsp[(1) - (1)].ival) < 0 || (yyvsp[(1) - (1)].ival) > MAX_OC_NUM)
        {
          	errprint("Invalid oc number, must between 0 - %d.\n", MAX_OC_NUM);
          	YYERROR;
        }
        (yyval.ival) = (yyvsp[(1) - (1)].ival);
    ;}
    break;

  case 48:
#line 596 "parser.y"
    {
      	if ((yyvsp[(1) - (1)].ival) < 0 || (yyvsp[(1) - (1)].ival) > (MAX_PKT_RANGE-1))
      	{
        	errprint("valid range number is 0-%d.\n", (MAX_PKT_RANGE-1));
        	YYERROR;
      	}
      	(yyval.ival) = (yyvsp[(1) - (1)].ival);
    ;}
    break;

  case 49:
#line 606 "parser.y"
    {
      	if ((yyvsp[(1) - (1)].ival) < 0 || (yyvsp[(1) - (1)].ival) > 1518)
      	{
        	errprint("valid range number is 0-1518.\n");
        	YYERROR;
      	}
      	(yyval.ival) = (yyvsp[(1) - (1)].ival);
    ;}
    break;

  case 50:
#line 616 "parser.y"
    {
      	if ((yyvsp[(1) - (1)].ival) < 0 || (yyvsp[(1) - (1)].ival) > 1518)
      	{
        	errprint("valid range number is 0-1518.\n");
        	YYERROR;
      	}
      	(yyval.ival) = (yyvsp[(1) - (1)].ival);
    ;}
    break;

  case 51:
#line 626 "parser.y"
    {
      	if( (yyvsp[(1) - (1)].ival) < 0 || (yyvsp[(1) - (1)].ival) > MAX_RULE_NO - 1)
      	{
       	errprint("valid number is: 0-%d\n", MAX_RULE_NO - 1);
        	YYERROR;      
      	}
      	(yyval.ival) = (yyvsp[(1) - (1)].ival);
    ;}
    break;

  case 52:
#line 635 "parser.y"
    {
        (yyval.ival) = MAX_RULE_NO ;
    ;}
    break;

  case 53:
#line 640 "parser.y"
    {
      	if( (yyvsp[(1) - (1)].ival) < 0 || (yyvsp[(1) - (1)].ival) > MAX_RULE_NO - 1)
      	{
        	errprint("valid number is: 0-%d\n", MAX_RULE_NO - 1);
        	YYERROR;      
      	}
      	(yyval.ival) = (yyvsp[(1) - (1)].ival);
    ;}
    break;

  case 54:
#line 649 "parser.y"
    {
        (yyval.ival) = MAX_RULE_NO ;
    ;}
    break;

  case 55:
#line 654 "parser.y"
    {
      	(yyval.ival) = 0x06;
    ;}
    break;

  case 56:
#line 658 "parser.y"
    {
      	(yyval.ival) = 0x11;
    ;}
    break;

  case 57:
#line 662 "parser.y"
    {
      	if ( (yyvsp[(1) - (1)].ival) <0 || (yyvsp[(1) - (1)].ival) > 255 )
      	{
        	errprint("Valid protocol is: 0 - 255\n");
        	YYERROR;
      	}
      	(yyval.ival) = (yyvsp[(1) - (1)].ival);
    ;}
    break;

  case 58:
#line 672 "parser.y"
    {
      	if ((yyvsp[(2) - (2)].ival) < 0 || (yyvsp[(2) - (2)].ival) > 32)
      	{
          	errprint("Invalid prefix length %d\n", (yyvsp[(2) - (2)].ival));
            YYERROR;                          
        }
        (yyval.ival) = PREFIX_LEN_2_MASK((yyvsp[(2) - (2)].ival));
    ;}
    break;

  case 59:
#line 681 "parser.y"
    {   
        (yyval.ival) = (yyvsp[(2) - (2)].uval);
    ;}
    break;

  case 60:
#line 686 "parser.y"
    {
      	if ( (yyvsp[(1) - (1)].ival) <0 || (yyvsp[(1) - (1)].ival) > 65535 )
      	{
        	errprint("Valid protocol is: 0x0000 - 0xFFFF\n");
        	YYERROR;
      	}
      	(yyval.ival) = (yyvsp[(1) - (1)].ival);
    ;}
    break;

  case 61:
#line 696 "parser.y"
    {
      	rule1options_t *opts;   
        if((opts = malloc(sizeof(rule1options_t))) == NULL)
        {     
            errprint("Out of memory in setruleoption_wl.\n");     
            YYERROR;    
        } 
      	
        memset(opts, 0, sizeof(rule1options_t));
        
        if(((yyval.wlptr) = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in setruleoption_wl.\n");
            free(opts);
          	YYERROR;
        } 
    ;}
    break;

  case 62:
#line 714 "parser.y"
    {   
        rule1options_t *opts;   
        if((opts = malloc(sizeof(rule1options_t))) == NULL)
        {     
            errprint("Out of memory in setruleoption_wl.\n");     
            YYERROR;    
        }
        
        opts->protocol = (yyvsp[(2) - (2)].ival);    
        opts->flag = RULE_OPT_PROTOCOL;   
            
        if(((yyval.wlptr) = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in setruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        } 
    ;}
    break;

  case 63:
#line 733 "parser.y"
    {
        rule1options_t *opts;
		
        if((opts = malloc(sizeof(rule1options_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
        
        opts->sourceip = (yyvsp[(3) - (4)].uval);
        opts->sourceip_mask = (yyvsp[(4) - (4)].ival);
        opts->flag = RULE_OPT_SRCIP;
		
        if(((yyval.wlptr) = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in setruleoption_wl.\n");
          	free(opts); 
            YYERROR;  
        }
    ;}
    break;

  case 64:
#line 754 "parser.y"
    {
        rule1options_t *opts;
            
        if((opts = malloc(sizeof(rule1options_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
        
        opts->destip = (yyvsp[(3) - (4)].uval);
        opts->destip_mask = (yyvsp[(4) - (4)].ival);
        opts->flag = RULE_OPT_DESTIP;
		
        if(((yyval.wlptr) = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in setruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
    ;}
    break;

  case 65:
#line 775 "parser.y"
    {
        rule1options_t *opts;
            
        if((opts = malloc(sizeof(rule1options_t))) == NULL)
        {     
            errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;  
        } 
		
        opts->sourcel4port = (yyvsp[(3) - (3)].ival);
        opts->flag = RULE_OPT_SRCL4PORT;
		
        if(((yyval.wlptr) = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {     
            errprint("Watch list full in setruleoption_wl.\n");
          	free(opts);   
            YYERROR;  
        }
    ;}
    break;

  case 66:
#line 795 "parser.y"
    {   
        rule1options_t *opts; 
  
        if((opts = malloc(sizeof(rule1options_t))) == NULL)   
        {     
            errprint("Out of memory in setruleoption_wl.\n");     
            YYERROR;    
        }
        opts->destl4port = (yyvsp[(3) - (3)].ival);    
        opts->flag = RULE_OPT_DESTL4PORT;   

        if(((yyval.wlptr) = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)    
        {     
            errprint("Watch list full in setruleoption_wl.\n");     
            free(opts);     
            YYERROR;    
        }       
    ;}
    break;

  case 67:
#line 814 "parser.y"
    {
    	rule1options_t *opts;
    	
        if((opts = malloc(sizeof(rule1options_t))) == NULL)
        {
            errprint("Out of memory in setruleoption_wl.\n");
            YYERROR;
        }
        
        opts->pktlen_range = (yyvsp[(4) - (4)].ival);
        opts->flag = RULE_OPT_PACKETLEN;
        
        if(((yyval.wlptr) = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
            errprint("Watch list full in setruleoption_wl.\n");
            free(opts);
            YYERROR; 
        }
    ;}
    break;

  case 68:
#line 835 "parser.y"
    {
        (yyval.wlptr) = (yyvsp[(1) - (1)].wlptr);  
    ;}
    break;

  case 69:
#line 839 "parser.y"
    {
        if (WL_PTR_OF((yyvsp[(1) - (2)].wlptr), rule1options_t)->flag & RULE_OPT_PROTOCOL)
        {
          	WL_PTR_OF((yyvsp[(2) - (2)].wlptr), rule1options_t)->protocol = WL_PTR_OF((yyvsp[(1) - (2)].wlptr), rule1options_t)->protocol;
        }
		
        if (WL_PTR_OF((yyvsp[(1) - (2)].wlptr), rule1options_t)->flag & RULE_OPT_SRCIP)
        {
          	WL_PTR_OF((yyvsp[(2) - (2)].wlptr), rule1options_t)->sourceip = WL_PTR_OF((yyvsp[(1) - (2)].wlptr), rule1options_t)->sourceip;
          	WL_PTR_OF((yyvsp[(2) - (2)].wlptr), rule1options_t)->sourceip_mask = WL_PTR_OF((yyvsp[(1) - (2)].wlptr), rule1options_t)->sourceip_mask;
        }
		
        if (WL_PTR_OF((yyvsp[(1) - (2)].wlptr), rule1options_t)->flag & RULE_OPT_DESTIP)
        {
          	WL_PTR_OF((yyvsp[(2) - (2)].wlptr), rule1options_t)->destip = WL_PTR_OF((yyvsp[(1) - (2)].wlptr), rule1options_t)->destip;
          	WL_PTR_OF((yyvsp[(2) - (2)].wlptr), rule1options_t)->destip_mask = WL_PTR_OF((yyvsp[(1) - (2)].wlptr), rule1options_t)->destip_mask;
        }
		
        if (WL_PTR_OF((yyvsp[(1) - (2)].wlptr), rule1options_t)->flag & RULE_OPT_SRCL4PORT)
        {
          	WL_PTR_OF((yyvsp[(2) - (2)].wlptr), rule1options_t)->sourcel4port = WL_PTR_OF((yyvsp[(1) - (2)].wlptr), rule1options_t)->sourcel4port;
        }
		
        if (WL_PTR_OF((yyvsp[(1) - (2)].wlptr), rule1options_t)->flag & RULE_OPT_DESTL4PORT)
        {
          	WL_PTR_OF((yyvsp[(2) - (2)].wlptr), rule1options_t)->destl4port = WL_PTR_OF((yyvsp[(1) - (2)].wlptr), rule1options_t)->destl4port;
        } 
		
        if (WL_PTR_OF((yyvsp[(1) - (2)].wlptr), rule1options_t)->flag & RULE_OPT_PACKETLEN)
        {
          	WL_PTR_OF((yyvsp[(2) - (2)].wlptr), rule1options_t)->pktlen_range = WL_PTR_OF((yyvsp[(1) - (2)].wlptr), rule1options_t)->pktlen_range;
        }
        
        WL_PTR_OF((yyvsp[(2) - (2)].wlptr), rule1options_t)->flag |= WL_PTR_OF((yyvsp[(1) - (2)].wlptr), rule1options_t)->flag;
        wl_free_entry((yyvsp[(1) - (2)].wlptr));
        
        (yyval.wlptr) = (yyvsp[(2) - (2)].wlptr);
    ;}
    break;

  case 70:
#line 879 "parser.y"
    {
        if ((yyvsp[(1) - (1)].ival) < 0 || (yyvsp[(1) - (1)].ival) >19)
        {
          	errprint("Invalid offset, must between 0 - 19.\n");
          	YYERROR;
        }
        (yyval.ival) = (yyvsp[(1) - (1)].ival);
    ;}
    break;

  case 71:
#line 889 "parser.y"
    {
        rule2options_t *opts;
		
        if ((opts = malloc(sizeof(rule2options_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        memset(opts, 0, sizeof(rule2options_t));
      	
        if (((yyval.wlptr) = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
    ;}
    break;

  case 72:
#line 908 "parser.y"
    {
        rule2options_t *opts;
		
        if ((opts = malloc(sizeof(rule2options_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        opts->offset = (yyvsp[(2) - (2)].ival);
        opts->flag = RULE_OPT_OFFSET;
		
        if (((yyval.wlptr) = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
    ;}
    break;

  case 73:
#line 928 "parser.y"
    {
        int i, j, len;
        char key_word_fs, key_word_ls, key[256];
        rule2options_t *opts;
        
        if((opts = malloc(sizeof(rule2options_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
        memset(opts->key, 0, MAX_KEY_NO);
        memset(opts->mask, 0xff, MAX_KEY_NO);
        
        /* do the process here */
        strcpy(key, WL_PTR((yyvsp[(2) - (2)].wlptr)));
        StrToUpper(key);
        //errprint("key is: %s\n", key);
        
        len = strlen(key);
        //errprint("key length is: %d\n", len);
		
        if ((len > MAX_KEY_LEN) || (len == 0) || (len % 2 != 0))
        {
          	errprint("Wrong key word length, must between 1 - 20 Bytes.\n");
          	YYERROR;
        }
        
        for (i=0; i < len /2; i++)
          	opts->mask[i] = 0x0;
    	
        for (i=0,j=0; i<len-1; )
        {
          	if ((key[i] >= 0x30) && (key[i] <= 0x39))
            	key_word_fs = key[i] - 0x30;
          	else if ((key[i] >= 0x41) && (key[i] <= 0x46))
            	key_word_fs = key[i] - 0x37;
          	else
          	{
            	errprint("Isn't a valid heximal digital!\n");
            	YYERROR;
          	}
			
          	if ((key[i+1] >= 0x30) && (key[i+1] <= 0x39))
            	key_word_ls = key[i+1] - 0x30;
          	else if ((key[i+1] >= 0x41) && (key[i+1] <= 0x46))
            	key_word_ls = key[i+1] - 0x37;
          	else
          	{
            	errprint("Isn't a valid heximal digital!\n");
            	YYERROR;
          	}
      		
          	opts->key[j] = ((key_word_fs << 4) & 0xf0)|(key_word_ls & 0xf);
          	j++;
          	i+=2;     
        }     
        opts->flag = RULE_OPT_KEY;
    
        if(((yyval.wlptr) = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in setruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
    ;}
    break;

  case 74:
#line 995 "parser.y"
    {
        (yyval.wlptr) = (yyvsp[(1) - (1)].wlptr);  
    ;}
    break;

  case 75:
#line 999 "parser.y"
    {
        int i;
		
        if (WL_PTR_OF((yyvsp[(1) - (2)].wlptr), rule2options_t)->flag & RULE_OPT_OFFSET)
        {
          	WL_PTR_OF((yyvsp[(2) - (2)].wlptr), rule2options_t)->offset = WL_PTR_OF((yyvsp[(1) - (2)].wlptr), rule2options_t)->offset;
        }
		
        if (WL_PTR_OF((yyvsp[(1) - (2)].wlptr), rule2options_t)->flag & RULE_OPT_KEY)
        {
          	for (i=0; i<MAX_KEY_NO; i++)
          	{
            	WL_PTR_OF((yyvsp[(2) - (2)].wlptr), rule2options_t)->key[i] = WL_PTR_OF((yyvsp[(1) - (2)].wlptr), rule2options_t)->key[i];
            	WL_PTR_OF((yyvsp[(2) - (2)].wlptr), rule2options_t)->mask[i] = WL_PTR_OF((yyvsp[(1) - (2)].wlptr), rule2options_t)->mask[i];
          	}
        }
    	
        WL_PTR_OF((yyvsp[(2) - (2)].wlptr), rule2options_t)->flag |= WL_PTR_OF((yyvsp[(1) - (2)].wlptr), rule2options_t)->flag;
        wl_free_entry((yyvsp[(1) - (2)].wlptr));
    	
        (yyval.wlptr) = (yyvsp[(2) - (2)].wlptr);
    ;}
    break;

  case 76:
#line 1028 "parser.y"
    {
      	unsigned int fwd_mode;
      	
      	//T:OP => "000"
      	fwd_mode = 0x0000;
      	
      	{
          	if (cli_set_forward_mode(PARSER_PARAMS, (yyvsp[(5) - (13)].ival), (yyvsp[(7) - (13)].ival), (yyvsp[(9) - (13)].ival), fwd_mode) < 0)
          	{
            	errprint("Error set forward mode for classify rule.\n");
            	break;
          	}
       }
    ;}
    break;

  case 77:
#line 1043 "parser.y"
    {
      	unsigned int fwd_mode;
      	
      	//T:OP => "1xx"
      	fwd_mode = 0x4000;
      	
      	{
          	if (cli_set_forward_mode(PARSER_PARAMS, (yyvsp[(5) - (13)].ival), (yyvsp[(7) - (13)].ival), (yyvsp[(9) - (13)].ival), fwd_mode) < 0)
          	{
            	errprint("Error set forward mode for classify rule.\n");
            	break;
          	}
      	}
    ;}
    break;

  case 78:
#line 1058 "parser.y"
    {
      	unsigned int fwd_mode;
      	
      	//T:OP => "001"
      	fwd_mode = 0x1000 | (yyvsp[(14) - (15)].ival);
      	
      	{
          	if (cli_set_forward_mode(PARSER_PARAMS, (yyvsp[(5) - (15)].ival), (yyvsp[(7) - (15)].ival), (yyvsp[(9) - (15)].ival), fwd_mode) < 0)
          	{
            	errprint("Error set forward mode for classify rule.\n");
            	break;
          	}
      	}
    ;}
    break;

  case 79:
#line 1073 "parser.y"
    {
      	unsigned int fwd_mode;
      	
      	//T:OP => "011"
      	fwd_mode = 0x3000 | (yyvsp[(14) - (16)].ival);
      	
      	{
          	if (cli_set_forward_mode(PARSER_PARAMS, (yyvsp[(5) - (16)].ival), (yyvsp[(7) - (16)].ival), (yyvsp[(9) - (16)].ival), fwd_mode) < 0)
          	{
            	errprint("Error set forward mode for classify rule.\n");
            	break;
          	}
      	}
    ;}
    break;

  case 80:
#line 1088 "parser.y"
    {
      	unsigned int fwd_mode;
      	
      	//T:OP => "010"
      	fwd_mode = 0x2000;
      	
      	{
          	if (cli_set_forward_mode(PARSER_PARAMS, (yyvsp[(5) - (13)].ival), (yyvsp[(7) - (13)].ival), (yyvsp[(9) - (13)].ival), fwd_mode) < 0)
          	{
            	errprint("Error set forward mode for classify rule.\n");
            	break;
          	}
      	}
    ;}
    break;

  case 81:
#line 1103 "parser.y"
    {
      	if (cli_get_forward_mode(PARSER_PARAMS, (yyvsp[(6) - (13)].ival), (yyvsp[(8) - (13)].ival), (yyvsp[(10) - (13)].ival)) < 0)
      	{
        	errprint("Error get forward mode.\n");
        	break;
      	}
    ;}
    break;

  case 82:
#line 1111 "parser.y"
    {
      	if (cli_set_sample_rate(PARSER_PARAMS, (yyvsp[(4) - (7)].ival), (yyvsp[(6) - (7)].ival)) < 0)
      	{
        	errprint("Error set sample rate.\n");
        	break;
      	}
    ;}
    break;

  case 83:
#line 1119 "parser.y"
    {
      	if (cli_get_sample_rate(PARSER_PARAMS, (yyvsp[(4) - (6)].ival)) < 0)
      	{
        	errprint("Error get sample rate.\n");
        	break;
      	}
    ;}
    break;

  case 84:
#line 1128 "parser.y"
    {
    	if ((yyvsp[(1) - (1)].ival) < 1 || (yyvsp[(1) - (1)].ival) > 4095)
    	{
      		errprint("Invalid vlan id, should be 1-4095!\n");
      		YYERROR;
    	}
    	(yyval.ival) = (yyvsp[(1) - (1)].ival);
  	;}
    break;

  case 85:
#line 1138 "parser.y"
    {
    	if ((yyvsp[(1) - (1)].ival) < 1 || (yyvsp[(1) - (1)].ival) > 99)
    	{
      		errprint("Invalid sample rate, should be 1-99!\n");
      		YYERROR;
    	}
    	(yyval.ival) = (yyvsp[(1) - (1)].ival);
  	;}
    break;

  case 86:
#line 1153 "parser.y"
    {
    	if (cli_set_eth_mac(PARSER_PARAMS, (yyvsp[(4) - (9)].ival), 0, WL_PTR((yyvsp[(8) - (9)].wlptr))) < 0)
    	{
      		errprint("Error set transfer mode.\n");
      		break;
    	}
  	;}
    break;

  case 87:
#line 1161 "parser.y"
    {
    	if (cli_set_eth_mac(PARSER_PARAMS, (yyvsp[(4) - (9)].ival), 1, WL_PTR((yyvsp[(8) - (9)].wlptr))) < 0)
    	{
      		errprint("Error set transfer mode.\n");
      		break;
    	}
  	;}
    break;

  case 88:
#line 1169 "parser.y"
    {
    	if (cli_get_eth_mac(PARSER_PARAMS, (yyvsp[(4) - (7)].ival)) < 0)
    	{
      		errprint("Error get transfer mode.\n");
      		break;
    	}
  	;}
    break;

  case 89:
#line 1183 "parser.y"
    {
    	if (cli_set_gtp_dest_ports(PARSER_PARAMS, (yyvsp[(5) - (9)].ival), (yyvsp[(8) - (9)].ival)) < 0)
    	{
      		errprint("Error set transfer mode.\n");
      		break;
    	}
  	;}
    break;

  case 90:
#line 1191 "parser.y"
    {
    	if (cli_set_gtp_hash_mode(PARSER_PARAMS, (yyvsp[(5) - (9)].ival), (yyvsp[(8) - (9)].ival)) < 0)
    	{
      		errprint("Error set gtp hash mode.\n");
      		break;
    	}
  	;}
    break;

  case 91:
#line 1199 "parser.y"
    {
    	if (cli_get_gtp_settings(PARSER_PARAMS, (yyvsp[(5) - (6)].ival)) < 0)
    	{
      		errprint("Error get transfer mode.\n");
      		break;
    	}
  	;}
    break;

  case 92:
#line 1208 "parser.y"
    {
			if ((yyvsp[(1) - (1)].ival) < 1 || (yyvsp[(1) - (1)].ival) > 24)
    		{
      			errprint("Invalid vlan id, should be 1-24!\n");
      			YYERROR;
    		}
    		(yyval.ival) = (yyvsp[(1) - (1)].ival);
		;}
    break;

  case 93:
#line 1218 "parser.y"
    {
		(yyval.ival) = 0x1;
	;}
    break;

  case 94:
#line 1222 "parser.y"
    {
		(yyval.ival) = 0x2;
	;}
    break;

  case 95:
#line 1226 "parser.y"
    {
		(yyval.ival) = 0x3;
	;}
    break;

  case 96:
#line 1236 "parser.y"
    {
  		if (cli_bcm_cmds(PARSER_PARAMS, WL_PTR((yyvsp[(1) - (2)].wlptr))) < 0)
        {
                errprint("Error send switch cmds.\n");
                break;
        }
  	;}
    break;

  case 97:
#line 1244 "parser.y"
    {
  		bcm_cmds_t opts;
      	
      	memset(&opts, 0, sizeof(bcm_cmds_t));
      	strcpy(opts.cmds, WL_PTR_OF((yyvsp[(1) - (2)].wlptr), bcm_cmds_t)->cmds);
      	
      	
  		if (cli_bcm_cmds_v2(PARSER_PARAMS, &opts) < 0)
    	{
      		errprint("Error send switch cmds.\n");
      		break;
    	}
  	;}
    break;

  case 98:
#line 1260 "parser.y"
    {
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, WL_PTR((yyvsp[(1) - (1)].wlptr)));
		
        if (((yyval.wlptr) = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	;}
    break;

  case 99:
#line 1279 "parser.y"
    {
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, WL_PTR_OF((yyvsp[(3) - (3)].wlptr), bcm_cmds_t)->cmds);
        strcat(opts->cmds, ",");
        strcat(opts->cmds, WL_PTR((yyvsp[(1) - (3)].wlptr))); /* add it to the end */
		
        if (((yyval.wlptr) = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	;}
    break;

  case 100:
#line 1301 "parser.y"
    {
		bcm_cmds_t *opts;
		
        	if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        	{
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        	}
		
        strcpy(opts->cmds, WL_PTR((yyvsp[(1) - (3)].wlptr)));
        strcat(opts->cmds, ".");
        strcat(opts->cmds, WL_PTR((yyvsp[(3) - (3)].wlptr))); /* add it to the end */
	
        if (((yyval.wlptr) = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	;}
    break;

  case 101:
#line 1324 "parser.y"
    {
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        sprintf(opts->cmds, "%d", (yyvsp[(1) - (1)].ival));
		
        if (((yyval.wlptr) = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	;}
    break;

  case 102:
#line 1343 "parser.y"
    {
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        sprintf(opts->cmds, "%d.%d.%d.%d", (((yyvsp[(1) - (1)].uval) >> 24) & 0xff),(((yyvsp[(1) - (1)].uval) >> 16) & 0xff),(((yyvsp[(1) - (1)].uval) >> 8) & 0xff),(((yyvsp[(1) - (1)].uval) >> 0) & 0xff) );
		
        if (((yyval.wlptr) = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	;}
    break;

  case 103:
#line 1362 "parser.y"
    {
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "\"");
        strcat(opts->cmds, WL_PTR((yyvsp[(1) - (1)].wlptr))); /* add it to the end */
	strcat(opts->cmds, "\"");
		
        if (((yyval.wlptr) = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	;}
    break;

  case 104:
#line 1383 "parser.y"
    {
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, WL_PTR_OF((yyvsp[(1) - (1)].wlptr), bcm_cmds_t)->cmds);
		
        if (((yyval.wlptr) = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	;}
    break;

  case 105:
#line 1402 "parser.y"
    {
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, WL_PTR((yyvsp[(1) - (3)].wlptr)));
        strcat(opts->cmds, "=");
        strcat(opts->cmds, WL_PTR_OF((yyvsp[(3) - (3)].wlptr), bcm_cmds_t)->cmds); /* add it to the end */
		
        if (((yyval.wlptr) = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	;}
    break;

  case 106:
#line 1423 "parser.y"
    {
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, WL_PTR_OF((yyvsp[(1) - (1)].wlptr), bcm_cmds_t)->cmds);
		
        if (((yyval.wlptr) = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	;}
    break;

  case 107:
#line 1442 "parser.y"
    {
		char tmpint[128];
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
        
		memset(tmpint, 0, sizeof(char)*128);
        strcpy(opts->cmds, WL_PTR((yyvsp[(1) - (3)].wlptr)));
        strcat(opts->cmds, "=");
        sprintf(tmpint, "%d", (yyvsp[(3) - (3)].ival));
        strcat(opts->cmds, tmpint); /* add it to the end */
		
        if (((yyval.wlptr) = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	;}
    break;

  case 108:
#line 1466 "parser.y"
    {
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, WL_PTR((yyvsp[(1) - (3)].wlptr)));
        strcat(opts->cmds, "=\"");
        strcat(opts->cmds, WL_PTR((yyvsp[(3) - (3)].wlptr))); /* add it to the end */
		strcat(opts->cmds, "\"");
		
        if (((yyval.wlptr) = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	;}
    break;

  case 109:
#line 1488 "parser.y"
    {
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "all");
		
        if (((yyval.wlptr) = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	;}
    break;

  case 110:
#line 1507 "parser.y"
    {
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "clear");
		
        if (((yyval.wlptr) = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	;}
    break;

  case 111:
#line 1526 "parser.y"
    {
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "config");
		
        if (((yyval.wlptr) = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	;}
    break;

  case 112:
#line 1545 "parser.y"
    {
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "counters");
		
        if (((yyval.wlptr) = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	;}
    break;

  case 113:
#line 1564 "parser.y"
    {
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "disable");
		
        if (((yyval.wlptr) = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	;}
    break;

  case 114:
#line 1583 "parser.y"
    {
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "drop");
		
        if (((yyval.wlptr) = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	;}
    break;

  case 115:
#line 1602 "parser.y"
    {
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "default");
		
        if (((yyval.wlptr) = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	;}
    break;

  case 116:
#line 1621 "parser.y"
    {
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "enable");
		
        if (((yyval.wlptr) = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	;}
    break;

  case 117:
#line 1640 "parser.y"
    {
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
	  
        strcpy(opts->cmds, "enable");
        strcat(opts->cmds, "=");
        strcat(opts->cmds, WL_PTR((yyvsp[(3) - (3)].wlptr))); /* add it to the end */
	  
        if (((yyval.wlptr) = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	;}
    break;

  case 118:
#line 1661 "parser.y"
    {
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "encap");
		
        if (((yyval.wlptr) = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	;}
    break;

  case 119:
#line 1680 "parser.y"
    {
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "egress");
		
        if (((yyval.wlptr) = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	;}
    break;

  case 120:
#line 1699 "parser.y"
    {
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "forward");
		
        if (((yyval.wlptr) = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	;}
    break;

  case 121:
#line 1718 "parser.y"
    {
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "file");
		
        if (((yyval.wlptr) = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	;}
    break;

  case 122:
#line 1737 "parser.y"
    {
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "global");
		
        if (((yyval.wlptr) = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	;}
    break;

  case 123:
#line 1756 "parser.y"
    {
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "get");
		
        if (((yyval.wlptr) = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	;}
    break;

  case 124:
#line 1775 "parser.y"
    {
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "ip");
		
        if (((yyval.wlptr) = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	;}
    break;

  case 125:
#line 1794 "parser.y"
    {
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "info");
		
        if (((yyval.wlptr) = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	;}
    break;

  case 126:
#line 1813 "parser.y"
    {
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "length");
		
        if (((yyval.wlptr) = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	;}
    break;

  case 127:
#line 1832 "parser.y"
    {
		char tmpint[128];
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		memset(tmpint, 0, sizeof(char)*128);
		sprintf(tmpint, "%d", (yyvsp[(3) - (3)].ival));
		
        strcpy(opts->cmds, "length=");
		strcat(opts->cmds, tmpint);
		
        if (((yyval.wlptr) = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	;}
    break;

  case 128:
#line 1855 "parser.y"
    {
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "mode");
		
        if (((yyval.wlptr) = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	;}
    break;

  case 129:
#line 1874 "parser.y"
    {
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "mac");
		
        if (((yyval.wlptr) = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	;}
    break;

  case 130:
#line 1893 "parser.y"
    {
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "none");
		
        if (((yyval.wlptr) = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	;}
    break;

  case 131:
#line 1912 "parser.y"
    {
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "netmask");
		
        if (((yyval.wlptr) = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	;}
    break;

  case 132:
#line 1931 "parser.y"
    {
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "offset");
		
        if (((yyval.wlptr) = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	;}
    break;

  case 133:
#line 1950 "parser.y"
    {
		char tmpint[128];
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		memset(tmpint, 0, sizeof(char)*128);
		sprintf(tmpint, "%d", (yyvsp[(3) - (3)].ival));
		
        strcpy(opts->cmds, "offset=");
		strcat(opts->cmds, tmpint);
		
        if (((yyval.wlptr) = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	;}
    break;

  case 134:
#line 1973 "parser.y"
    {
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "port");
		
        if (((yyval.wlptr) = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	;}
    break;

  case 135:
#line 1992 "parser.y"
    {
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
	 
        strcpy(opts->cmds, "ports");
	 
        if (((yyval.wlptr) = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	;}
    break;

  case 136:
#line 2011 "parser.y"
    {
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "protocol");
		
        if (((yyval.wlptr) = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	;}
    break;

  case 137:
#line 2030 "parser.y"
    {
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "range");
		
        if (((yyval.wlptr) = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	;}
    break;

  case 138:
#line 2049 "parser.y"
    {
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "rate");
		
        if (((yyval.wlptr) = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	;}
    break;

  case 139:
#line 2068 "parser.y"
    {
				bcm_cmds_t *opts;
				
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
				
        strcpy(opts->cmds, "rcload");
				
        if (((yyval.wlptr) = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	;}
    break;

  case 140:
#line 2088 "parser.y"
    {
				bcm_cmds_t *opts;
				
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "read");
		
        if (((yyval.wlptr) = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	;}
    break;

  case 141:
#line 2107 "parser.y"
    {
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "set");
		
        if (((yyval.wlptr) = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	;}
    break;

  case 142:
#line 2126 "parser.y"
    {
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "stats");
		
        if (((yyval.wlptr) = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	;}
    break;

  case 143:
#line 2145 "parser.y"
    {
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "show");
		
        if (((yyval.wlptr) = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	;}
    break;

  case 144:
#line 2164 "parser.y"
    {
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "source");
		
        if (((yyval.wlptr) = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	;}
    break;

  case 145:
#line 2183 "parser.y"
    {
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "save");
		
        if (((yyval.wlptr) = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	;}
    break;

  case 146:
#line 2202 "parser.y"
    {
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "update");
		
        if (((yyval.wlptr) = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	;}
    break;

  case 147:
#line 2221 "parser.y"
    {
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "vlan");
		
        if (((yyval.wlptr) = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	;}
    break;

  case 148:
#line 2240 "parser.y"
    {
		bcm_cmds_t *opts;
		
        if ((opts = malloc(sizeof(bcm_cmds_t))) == NULL)
        {
          	errprint("Out of memory in setruleoption_wl.\n");
          	YYERROR;
        }
		
        strcpy(opts->cmds, "write");
		
        if (((yyval.wlptr) = wl_add_entry(PARSER_PARAMS->wl, opts)) == NULL)
        {
          	errprint("Watch list full in sertruleoption_wl.\n");
          	free(opts);
          	YYERROR;
        }
	;}
    break;

  case 149:
#line 2260 "parser.y"
    {
        (yyval.wlptr) = (yyvsp[(1) - (1)].wlptr);
    ;}
    break;

  case 150:
#line 2264 "parser.y"
    {
        char str_tmp[128];
        
        memset(str_tmp, 0, 128*(sizeof(char)));
        str_tmp[0] = 32;  //blank space
        strcat(str_tmp, WL_PTR_OF((yyvsp[(2) - (2)].wlptr), bcm_cmds_t)->cmds);
        
    	strcat(WL_PTR_OF((yyvsp[(1) - (2)].wlptr), bcm_cmds_t)->cmds, str_tmp);
        wl_free_entry((yyvsp[(2) - (2)].wlptr));
    	
        (yyval.wlptr) = (yyvsp[(1) - (2)].wlptr);
    ;}
    break;

  case 151:
#line 2282 "parser.y"
    {
    	if(cli_set_oc_line_rate(PARSER_PARAMS, (yyvsp[(4) - (8)].ival), (yyvsp[(7) - (8)].ival)) < 0)
    	{
      		errprint("Error set oc line mode.\n");
      		break;
    	}
  	;}
    break;

  case 152:
#line 2290 "parser.y"
    {
      	if(cli_set_oc_line_mode(PARSER_PARAMS, (yyvsp[(4) - (8)].ival), (yyvsp[(7) - (8)].ival)) < 0)
      	{
        	errprint("Error set oc line mode.\n");
        	break;
      	}
    ;}
    break;

  case 153:
#line 2298 "parser.y"
    {
      	if (cli_set_oc_encap_mode(PARSER_PARAMS, (yyvsp[(4) - (8)].ival), (yyvsp[(7) - (8)].ival)) < 0)
      	{
        	errprint("Error set OC encapsulate mode.\n");
        	break;
      	}
    ;}
    break;

  case 154:
#line 2306 "parser.y"
    {
      	if (cli_set_oc_sonnet_scramble_enable(PARSER_PARAMS, (yyvsp[(4) - (8)].ival), (yyvsp[(7) - (8)].ival)) < 0)
      	{
        	errprint("Error setting sonnet scramble enable.\n");
        	break;
      	}
    ;}
    break;

  case 155:
#line 2314 "parser.y"
    {
      	if (cli_set_oc_payload_scramble_enable(PARSER_PARAMS, (yyvsp[(4) - (8)].ival), (yyvsp[(7) - (8)].ival)) < 0)
      	{
        	errprint("Error setting payload scramble enable.\n");
        	break;
      	}
    ;}
    break;

  case 156:
#line 2322 "parser.y"
    {
      	if (cli_set_oc_crc_mode(PARSER_PARAMS, (yyvsp[(4) - (8)].ival), (yyvsp[(7) - (8)].ival)) < 0)
      	{
        	errprint("Error setting oc crc mode.\n");
        	break;
      	}
    ;}
    break;

  case 157:
#line 2330 "parser.y"
    {
      	if (cli_get_oc_settings(PARSER_PARAMS) < 0)
      	{
        	errprint("Error getting oc settings.\n");
        	break;
      	} 
    ;}
    break;

  case 158:
#line 2338 "parser.y"
    {
      	if (cli_palermo_get_stats((yyvsp[(4) - (6)].ival)) < 0)
      	{
        	errprint("Error getting oc counters.\n");
        	break;
      	}
    ;}
    break;

  case 159:
#line 2346 "parser.y"
    {
      	if (cli_get_framer_stats((yyvsp[(4) - (6)].ival)) < 0)
      	{
        	errprint("Error getting oc flow info.\n");
        	break;
      	}
    ;}
    break;

  case 160:
#line 2354 "parser.y"
    {
      	if (cli_get_framer_running_cfg((yyvsp[(4) - (6)].ival)) < 0)
      	{
        	errprint("Error getting oc flow info.\n");
        	break;
      	}
    ;}
    break;

  case 161:
#line 2362 "parser.y"
    {
      	if (cli_save_oc_settings(PARSER_PARAMS) < 0)
      	{
        	errprint("Error save oc settings to profile.\n");
        	break;
      	}
    ;}
    break;

  case 162:
#line 2372 "parser.y"
    {
    	(yyval.ival) = OC_LINERATE_192C;
  	;}
    break;

  case 163:
#line 2376 "parser.y"
    {
    	(yyval.ival) = OC_LINERATE_48C;
  	;}
    break;

  case 164:
#line 2381 "parser.y"
    {
    	(yyval.ival) = OC_LINE_SONNET;
  	;}
    break;

  case 165:
#line 2385 "parser.y"
    {
    	(yyval.ival) = OC_LINE_SDH;
  	;}
    break;

  case 166:
#line 2390 "parser.y"
    { 
    	(yyval.ival) = OC_ENCAP_PPP;
  	;}
    break;

  case 167:
#line 2394 "parser.y"
    {
    	(yyval.ival) = OC_ENCAP_HDLC;
  	;}
    break;

  case 168:
#line 2399 "parser.y"
    {
    	(yyval.ival) = OC_CRC_CCITT;
  	;}
    break;

  case 169:
#line 2403 "parser.y"
    {
    	(yyval.ival) = OC_CRC_32;
  	;}
    break;

  case 170:
#line 2407 "parser.y"
    {
    	(yyval.ival) = OC_CRC_NONE;
  	;}
    break;

  case 171:
#line 2417 "parser.y"
    {
      	if (cli_fpga_reg_read((yyvsp[(3) - (8)].ival), (yyvsp[(6) - (8)].ival)) < 0)
      	{
        	errprint("Error reading fpga register.\n");
        	break;
      	}
    ;}
    break;

  case 172:
#line 2425 "parser.y"
    {
      	if (cli_fpga_reg_write((yyvsp[(3) - (9)].ival), (yyvsp[(6) - (9)].ival), (yyvsp[(8) - (9)].ival)) < 0)
      	{
        	errprint("Error writting fpga register.\n");
        	break;
      	}
    ;}
    break;

  case 173:
#line 2433 "parser.y"
    {
      	if (cli_get_fpga_stats((yyvsp[(3) - (7)].ival)) < 0)
      	{
        	errprint("Error getting fpga stats.\n");
        	break;
      	}
    ;}
    break;

  case 174:
#line 2441 "parser.y"
    {
      	if (cli_get_packet_stats() < 0)
      	{
        	errprint("Error getting packet stats.\n");
        	break;
      	}
    ;}
    break;

  case 175:
#line 2449 "parser.y"
    {
    	if (cli_fpga_set_flow_enable(PARSER_PARAMS, (yyvsp[(4) - (7)].ival), (yyvsp[(6) - (7)].ival)) < 0)
    	{
    		errprint("Error setting flow function.\n");
    		break;
    	}
    ;}
    break;

  case 176:
#line 2457 "parser.y"
    {
    	if (cli_fpga_get_flow_enable(PARSER_PARAMS, (yyvsp[(4) - (6)].ival)) < 0)
    	{
    		errprint("Error setting flow function.\n");
    		break;
    	}
    ;}
    break;

  case 177:
#line 2465 "parser.y"
    {
    	if (cli_fpga_set_flow_mms(PARSER_PARAMS, (yyvsp[(4) - (9)].ival), (yyvsp[(8) - (9)].ival)) < 0)
    	{
    		errprint("Error setting flow function.\n");
    		break;
    	}
    ;}
    break;

  case 178:
#line 2473 "parser.y"
    {
    	if (cli_fpga_get_flow_mms(PARSER_PARAMS, (yyvsp[(4) - (8)].ival)) < 0)
    	{
    		errprint("Error setting flow function.\n");
    		break;
    	}
    ;}
    break;

  case 179:
#line 2481 "parser.y"
    {
    	if (cli_fpga_set_traffic_enable(PARSER_PARAMS, (yyvsp[(4) - (7)].ival), (yyvsp[(6) - (7)].ival)) < 0)
    	{
    		errprint("Error setting flow function.\n");
    		break;
    	}
    ;}
    break;

  case 180:
#line 2489 "parser.y"
    {
    	if (cli_fpga_get_traffic_enable(PARSER_PARAMS, (yyvsp[(4) - (6)].ival)) < 0)
    	{
    		errprint("Error setting flow function.\n");
    		break;
    	}
    ;}
    break;

  case 181:
#line 2498 "parser.y"
    {
      	if ((yyvsp[(1) - (1)].ival) < 0 || (yyvsp[(1) - (1)].ival) > 0xffffffff)
      	{
        	errprint("Valid register range is: 0 - 0xffffffff\n");
        	YYERROR;
      	}
      	(yyval.ival) = (yyvsp[(1) - (1)].ival);
    ;}
    break;

  case 182:
#line 2508 "parser.y"
    {
      	if ( (yyvsp[(1) - (1)].ival) < 0 || (yyvsp[(1) - (1)].ival) > 0xffffffff )
      	{
        	errprint("Valid fpga reg data is: 0x0000 - 0xffffffff\n");
        	YYERROR;
      	}
      	(yyval.ival) = (yyvsp[(1) - (1)].ival);
    ;}
    break;

  case 183:
#line 2518 "parser.y"
    {
		if ( (yyvsp[(1) - (1)].ival) < 500 || (yyvsp[(1) - (1)].ival) > 1500 )
      	{
        	errprint("Valid MMS is: 500 - 1500\n");
        	YYERROR;
      	}
      	(yyval.ival) = (yyvsp[(1) - (1)].ival);
	;}
    break;


/* Line 1267 of yacc.c.  */
#line 4752 "y.tab.c"
      default: break;
    }
  YY_SYMBOL_PRINT ("-> $$ =", yyr1[yyn], &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);

  *++yyvsp = yyval;


  /* Now `shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */

  yyn = yyr1[yyn];

  yystate = yypgoto[yyn - YYNTOKENS] + *yyssp;
  if (0 <= yystate && yystate <= YYLAST && yycheck[yystate] == *yyssp)
    yystate = yytable[yystate];
  else
    yystate = yydefgoto[yyn - YYNTOKENS];

  goto yynewstate;


/*------------------------------------.
| yyerrlab -- here on detecting error |
`------------------------------------*/
yyerrlab:
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
#if ! YYERROR_VERBOSE
      yyerror (YY_("syntax error"));
#else
      {
	YYSIZE_T yysize = yysyntax_error (0, yystate, yychar);
	if (yymsg_alloc < yysize && yymsg_alloc < YYSTACK_ALLOC_MAXIMUM)
	  {
	    YYSIZE_T yyalloc = 2 * yysize;
	    if (! (yysize <= yyalloc && yyalloc <= YYSTACK_ALLOC_MAXIMUM))
	      yyalloc = YYSTACK_ALLOC_MAXIMUM;
	    if (yymsg != yymsgbuf)
	      YYSTACK_FREE (yymsg);
	    yymsg = (char *) YYSTACK_ALLOC (yyalloc);
	    if (yymsg)
	      yymsg_alloc = yyalloc;
	    else
	      {
		yymsg = yymsgbuf;
		yymsg_alloc = sizeof yymsgbuf;
	      }
	  }

	if (0 < yysize && yysize <= yymsg_alloc)
	  {
	    (void) yysyntax_error (yymsg, yystate, yychar);
	    yyerror (yymsg);
	  }
	else
	  {
	    yyerror (YY_("syntax error"));
	    if (yysize != 0)
	      goto yyexhaustedlab;
	  }
      }
#endif
    }



  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse look-ahead token after an
	 error, discard it.  */

      if (yychar <= YYEOF)
	{
	  /* Return failure if at end of input.  */
	  if (yychar == YYEOF)
	    YYABORT;
	}
      else
	{
	  yydestruct ("Error: discarding",
		      yytoken, &yylval);
	  yychar = YYEMPTY;
	}
    }

  /* Else will try to reuse look-ahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:

  /* Pacify compilers like GCC when the user code never invokes
     YYERROR and the label yyerrorlab therefore never appears in user
     code.  */
  if (/*CONSTCOND*/ 0)
     goto yyerrorlab;

  /* Do not reclaim the symbols of the rule which action triggered
     this YYERROR.  */
  YYPOPSTACK (yylen);
  yylen = 0;
  YY_STACK_PRINT (yyss, yyssp);
  yystate = *yyssp;
  goto yyerrlab1;


/*-------------------------------------------------------------.
| yyerrlab1 -- common code for both syntax error and YYERROR.  |
`-------------------------------------------------------------*/
yyerrlab1:
  yyerrstatus = 3;	/* Each real token shifted decrements this.  */

  for (;;)
    {
      yyn = yypact[yystate];
      if (yyn != YYPACT_NINF)
	{
	  yyn += YYTERROR;
	  if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYTERROR)
	    {
	      yyn = yytable[yyn];
	      if (0 < yyn)
		break;
	    }
	}

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
	YYABORT;


      yydestruct ("Error: popping",
		  yystos[yystate], yyvsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  if (yyn == YYFINAL)
    YYACCEPT;

  *++yyvsp = yylval;


  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", yystos[yyn], yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturn;

/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturn;

#ifndef yyoverflow
/*-------------------------------------------------.
| yyexhaustedlab -- memory exhaustion comes here.  |
`-------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  /* Fall through.  */
#endif

yyreturn:
  if (yychar != YYEOF && yychar != YYEMPTY)
     yydestruct ("Cleanup: discarding lookahead",
		 yytoken, &yylval);
  /* Do not reclaim the symbols of the rule which action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
		  yystos[*yyssp], yyvsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif
#if YYERROR_VERBOSE
  if (yymsg != yymsgbuf)
    YYSTACK_FREE (yymsg);
#endif
  /* Make sure YYID is used.  */
  return YYID (yyresult);
}


#line 2527 "parser.y"


#include "flex.h"
   
/* dummy declaration to silence warning caused by incorrectly
   generated declaration in flex.h */
static int yy_init_globals (yyscan_t yyscanner )
{
    (void) yyscanner;
    (void) yy_init_globals;
    return 0;
}

/*******************************************************************
 *
 * Description:     Parser error function.
 *
 * Modified args:   NONE
 *
 * Return value:    N/A.
 *
 *******************************************************************/
void scerror(char *s)
{
    printf("%s\n", s);
}

static char *StrToUpper(char *str)
{
    char *p = str;

    while (*p != '\0')
    {
        *p = toupper(*p);
        p++;
    }

    return str;
}

/*******************************************************************
 *
 * Description:     Make a port mask with all ports from
 *                  MIN(a,b) to MAX(a,b) in it.
 *
 * Modified args:   None
 *
 * Return value:    port mask.
 *
 *******************************************************************/
static int portrange(int a, int b)
{
    int start;
    int stop;
    int i;
    int mask = 0;

    start = (a < b) ? a : b;
    stop  = (a < b) ? b : a;

    for (i = start; i <= stop; i++)
    {
        mask |= 1 << i;
    }

    return mask;
}


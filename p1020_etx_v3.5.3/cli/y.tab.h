/* A Bison parser, made by GNU Bison 2.3.  */

/* Skeleton interface for Bison's Yacc-like parsers in C

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




#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
typedef union YYSTYPE
#line 62 "parser.y"
{
    int ival;   /* For returning values */
    unsigned int uval;
    struct vlanmap *vptr;
    void **wlptr;   /* watch list entry */
}
/* Line 1529 of yacc.c.  */
#line 248 "y.tab.h"
	YYSTYPE;
# define yystype YYSTYPE /* obsolescent; will be withdrawn */
# define YYSTYPE_IS_DECLARED 1
# define YYSTYPE_IS_TRIVIAL 1
#endif




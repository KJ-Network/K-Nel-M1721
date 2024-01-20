/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison implementation for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

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

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

/* All symbols defined below should begin with yy or YY, to avoid
   infringing on user name space.  This should be done even for local
   variables, as they might otherwise be expanded by user macros.
   There are some unavoidable exceptions within include files to
   define necessary library symbols; they are noted "INFRINGES ON
   USER NAME SPACE" below.  */

/* Identify Bison output, and Bison version.  */
#define YYBISON 30802

/* Bison version string.  */
#define YYBISON_VERSION "3.8.2"

/* Skeleton name.  */
#define YYSKELETON_NAME "yacc.c"

/* Pure parsers.  */
#define YYPURE 0

/* Push parsers.  */
#define YYPUSH 0

/* Pull parsers.  */
#define YYPULL 1




/* First part of user prologue.  */
#line 20 "../scripts/dtc-aosp/dtc/dtc-parser.y"

#include <stdio.h>
#include <inttypes.h>

#include "dtc.h"
#include "srcpos.h"

extern int yylex(void);
extern void yyerror(char const *s);
#define ERROR(loc, ...) \
	do { \
		srcpos_error((loc), "Error", __VA_ARGS__); \
		treesource_error = true; \
	} while (0)

extern struct dt_info *parser_output;
extern bool treesource_error;

#line 90 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"

# ifndef YY_CAST
#  ifdef __cplusplus
#   define YY_CAST(Type, Val) static_cast<Type> (Val)
#   define YY_REINTERPRET_CAST(Type, Val) reinterpret_cast<Type> (Val)
#  else
#   define YY_CAST(Type, Val) ((Type) (Val))
#   define YY_REINTERPRET_CAST(Type, Val) ((Type) (Val))
#  endif
# endif
# ifndef YY_NULLPTR
#  if defined __cplusplus
#   if 201103L <= __cplusplus
#    define YY_NULLPTR nullptr
#   else
#    define YY_NULLPTR 0
#   endif
#  else
#   define YY_NULLPTR ((void*)0)
#  endif
# endif

#include "dtc-parser.h"
/* Symbol kind.  */
enum yysymbol_kind_t
{
  YYSYMBOL_YYEMPTY = -2,
  YYSYMBOL_YYEOF = 0,                      /* "end of file"  */
  YYSYMBOL_YYerror = 1,                    /* error  */
  YYSYMBOL_YYUNDEF = 2,                    /* "invalid token"  */
  YYSYMBOL_DT_V1 = 3,                      /* DT_V1  */
  YYSYMBOL_DT_PLUGIN = 4,                  /* DT_PLUGIN  */
  YYSYMBOL_DT_MEMRESERVE = 5,              /* DT_MEMRESERVE  */
  YYSYMBOL_DT_LSHIFT = 6,                  /* DT_LSHIFT  */
  YYSYMBOL_DT_RSHIFT = 7,                  /* DT_RSHIFT  */
  YYSYMBOL_DT_LE = 8,                      /* DT_LE  */
  YYSYMBOL_DT_GE = 9,                      /* DT_GE  */
  YYSYMBOL_DT_EQ = 10,                     /* DT_EQ  */
  YYSYMBOL_DT_NE = 11,                     /* DT_NE  */
  YYSYMBOL_DT_AND = 12,                    /* DT_AND  */
  YYSYMBOL_DT_OR = 13,                     /* DT_OR  */
  YYSYMBOL_DT_BITS = 14,                   /* DT_BITS  */
  YYSYMBOL_DT_DEL_PROP = 15,               /* DT_DEL_PROP  */
  YYSYMBOL_DT_DEL_NODE = 16,               /* DT_DEL_NODE  */
  YYSYMBOL_DT_PROPNODENAME = 17,           /* DT_PROPNODENAME  */
  YYSYMBOL_DT_LITERAL = 18,                /* DT_LITERAL  */
  YYSYMBOL_DT_CHAR_LITERAL = 19,           /* DT_CHAR_LITERAL  */
  YYSYMBOL_DT_BYTE = 20,                   /* DT_BYTE  */
  YYSYMBOL_DT_STRING = 21,                 /* DT_STRING  */
  YYSYMBOL_DT_LABEL = 22,                  /* DT_LABEL  */
  YYSYMBOL_DT_REF = 23,                    /* DT_REF  */
  YYSYMBOL_DT_INCBIN = 24,                 /* DT_INCBIN  */
  YYSYMBOL_25_ = 25,                       /* ';'  */
  YYSYMBOL_26_ = 26,                       /* '/'  */
  YYSYMBOL_27_ = 27,                       /* '{'  */
  YYSYMBOL_28_ = 28,                       /* '}'  */
  YYSYMBOL_29_ = 29,                       /* '='  */
  YYSYMBOL_30_ = 30,                       /* '>'  */
  YYSYMBOL_31_ = 31,                       /* '['  */
  YYSYMBOL_32_ = 32,                       /* ']'  */
  YYSYMBOL_33_ = 33,                       /* '('  */
  YYSYMBOL_34_ = 34,                       /* ','  */
  YYSYMBOL_35_ = 35,                       /* ')'  */
  YYSYMBOL_36_ = 36,                       /* '<'  */
  YYSYMBOL_37_ = 37,                       /* '?'  */
  YYSYMBOL_38_ = 38,                       /* ':'  */
  YYSYMBOL_39_ = 39,                       /* '|'  */
  YYSYMBOL_40_ = 40,                       /* '^'  */
  YYSYMBOL_41_ = 41,                       /* '&'  */
  YYSYMBOL_42_ = 42,                       /* '+'  */
  YYSYMBOL_43_ = 43,                       /* '-'  */
  YYSYMBOL_44_ = 44,                       /* '*'  */
  YYSYMBOL_45_ = 45,                       /* '%'  */
  YYSYMBOL_46_ = 46,                       /* '~'  */
  YYSYMBOL_47_ = 47,                       /* '!'  */
  YYSYMBOL_YYACCEPT = 48,                  /* $accept  */
  YYSYMBOL_sourcefile = 49,                /* sourcefile  */
  YYSYMBOL_header = 50,                    /* header  */
  YYSYMBOL_headers = 51,                   /* headers  */
  YYSYMBOL_memreserves = 52,               /* memreserves  */
  YYSYMBOL_memreserve = 53,                /* memreserve  */
  YYSYMBOL_devicetree = 54,                /* devicetree  */
  YYSYMBOL_nodedef = 55,                   /* nodedef  */
  YYSYMBOL_proplist = 56,                  /* proplist  */
  YYSYMBOL_propdef = 57,                   /* propdef  */
  YYSYMBOL_propdata = 58,                  /* propdata  */
  YYSYMBOL_propdataprefix = 59,            /* propdataprefix  */
  YYSYMBOL_arrayprefix = 60,               /* arrayprefix  */
  YYSYMBOL_integer_prim = 61,              /* integer_prim  */
  YYSYMBOL_integer_expr = 62,              /* integer_expr  */
  YYSYMBOL_integer_trinary = 63,           /* integer_trinary  */
  YYSYMBOL_integer_or = 64,                /* integer_or  */
  YYSYMBOL_integer_and = 65,               /* integer_and  */
  YYSYMBOL_integer_bitor = 66,             /* integer_bitor  */
  YYSYMBOL_integer_bitxor = 67,            /* integer_bitxor  */
  YYSYMBOL_integer_bitand = 68,            /* integer_bitand  */
  YYSYMBOL_integer_eq = 69,                /* integer_eq  */
  YYSYMBOL_integer_rela = 70,              /* integer_rela  */
  YYSYMBOL_integer_shift = 71,             /* integer_shift  */
  YYSYMBOL_integer_add = 72,               /* integer_add  */
  YYSYMBOL_integer_mul = 73,               /* integer_mul  */
  YYSYMBOL_integer_unary = 74,             /* integer_unary  */
  YYSYMBOL_bytestring = 75,                /* bytestring  */
  YYSYMBOL_subnodes = 76,                  /* subnodes  */
  YYSYMBOL_subnode = 77                    /* subnode  */
};
typedef enum yysymbol_kind_t yysymbol_kind_t;




#ifdef short
# undef short
#endif

/* On compilers that do not define __PTRDIFF_MAX__ etc., make sure
   <limits.h> and (if available) <stdint.h> are included
   so that the code can choose integer types of a good width.  */

#ifndef __PTRDIFF_MAX__
# include <limits.h> /* INFRINGES ON USER NAME SPACE */
# if defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stdint.h> /* INFRINGES ON USER NAME SPACE */
#  define YY_STDINT_H
# endif
#endif

/* Narrow types that promote to a signed type and that can represent a
   signed or unsigned integer of at least N bits.  In tables they can
   save space and decrease cache pressure.  Promoting to a signed type
   helps avoid bugs in integer arithmetic.  */

#ifdef __INT_LEAST8_MAX__
typedef __INT_LEAST8_TYPE__ yytype_int8;
#elif defined YY_STDINT_H
typedef int_least8_t yytype_int8;
#else
typedef signed char yytype_int8;
#endif

#ifdef __INT_LEAST16_MAX__
typedef __INT_LEAST16_TYPE__ yytype_int16;
#elif defined YY_STDINT_H
typedef int_least16_t yytype_int16;
#else
typedef short yytype_int16;
#endif

/* Work around bug in HP-UX 11.23, which defines these macros
   incorrectly for preprocessor constants.  This workaround can likely
   be removed in 2023, as HPE has promised support for HP-UX 11.23
   (aka HP-UX 11i v2) only through the end of 2022; see Table 2 of
   <https://h20195.www2.hpe.com/V2/getpdf.aspx/4AA4-7673ENW.pdf>.  */
#ifdef __hpux
# undef UINT_LEAST8_MAX
# undef UINT_LEAST16_MAX
# define UINT_LEAST8_MAX 255
# define UINT_LEAST16_MAX 65535
#endif

#if defined __UINT_LEAST8_MAX__ && __UINT_LEAST8_MAX__ <= __INT_MAX__
typedef __UINT_LEAST8_TYPE__ yytype_uint8;
#elif (!defined __UINT_LEAST8_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST8_MAX <= INT_MAX)
typedef uint_least8_t yytype_uint8;
#elif !defined __UINT_LEAST8_MAX__ && UCHAR_MAX <= INT_MAX
typedef unsigned char yytype_uint8;
#else
typedef short yytype_uint8;
#endif

#if defined __UINT_LEAST16_MAX__ && __UINT_LEAST16_MAX__ <= __INT_MAX__
typedef __UINT_LEAST16_TYPE__ yytype_uint16;
#elif (!defined __UINT_LEAST16_MAX__ && defined YY_STDINT_H \
       && UINT_LEAST16_MAX <= INT_MAX)
typedef uint_least16_t yytype_uint16;
#elif !defined __UINT_LEAST16_MAX__ && USHRT_MAX <= INT_MAX
typedef unsigned short yytype_uint16;
#else
typedef int yytype_uint16;
#endif

#ifndef YYPTRDIFF_T
# if defined __PTRDIFF_TYPE__ && defined __PTRDIFF_MAX__
#  define YYPTRDIFF_T __PTRDIFF_TYPE__
#  define YYPTRDIFF_MAXIMUM __PTRDIFF_MAX__
# elif defined PTRDIFF_MAX
#  ifndef ptrdiff_t
#   include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  endif
#  define YYPTRDIFF_T ptrdiff_t
#  define YYPTRDIFF_MAXIMUM PTRDIFF_MAX
# else
#  define YYPTRDIFF_T long
#  define YYPTRDIFF_MAXIMUM LONG_MAX
# endif
#endif

#ifndef YYSIZE_T
# ifdef __SIZE_TYPE__
#  define YYSIZE_T __SIZE_TYPE__
# elif defined size_t
#  define YYSIZE_T size_t
# elif defined __STDC_VERSION__ && 199901 <= __STDC_VERSION__
#  include <stddef.h> /* INFRINGES ON USER NAME SPACE */
#  define YYSIZE_T size_t
# else
#  define YYSIZE_T unsigned
# endif
#endif

#define YYSIZE_MAXIMUM                                  \
  YY_CAST (YYPTRDIFF_T,                                 \
           (YYPTRDIFF_MAXIMUM < YY_CAST (YYSIZE_T, -1)  \
            ? YYPTRDIFF_MAXIMUM                         \
            : YY_CAST (YYSIZE_T, -1)))

#define YYSIZEOF(X) YY_CAST (YYPTRDIFF_T, sizeof (X))


/* Stored state numbers (used for stacks). */
typedef yytype_uint8 yy_state_t;

/* State numbers in computations.  */
typedef int yy_state_fast_t;

#ifndef YY_
# if defined YYENABLE_NLS && YYENABLE_NLS
#  if ENABLE_NLS
#   include <libintl.h> /* INFRINGES ON USER NAME SPACE */
#   define YY_(Msgid) dgettext ("bison-runtime", Msgid)
#  endif
# endif
# ifndef YY_
#  define YY_(Msgid) Msgid
# endif
#endif


#ifndef YY_ATTRIBUTE_PURE
# if defined __GNUC__ && 2 < __GNUC__ + (96 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_PURE __attribute__ ((__pure__))
# else
#  define YY_ATTRIBUTE_PURE
# endif
#endif

#ifndef YY_ATTRIBUTE_UNUSED
# if defined __GNUC__ && 2 < __GNUC__ + (7 <= __GNUC_MINOR__)
#  define YY_ATTRIBUTE_UNUSED __attribute__ ((__unused__))
# else
#  define YY_ATTRIBUTE_UNUSED
# endif
#endif

/* Suppress unused-variable warnings by "using" E.  */
#if ! defined lint || defined __GNUC__
# define YY_USE(E) ((void) (E))
#else
# define YY_USE(E) /* empty */
#endif

/* Suppress an incorrect diagnostic about yylval being uninitialized.  */
#if defined __GNUC__ && ! defined __ICC && 406 <= __GNUC__ * 100 + __GNUC_MINOR__
# if __GNUC__ * 100 + __GNUC_MINOR__ < 407
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")
# else
#  define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN                           \
    _Pragma ("GCC diagnostic push")                                     \
    _Pragma ("GCC diagnostic ignored \"-Wuninitialized\"")              \
    _Pragma ("GCC diagnostic ignored \"-Wmaybe-uninitialized\"")
# endif
# define YY_IGNORE_MAYBE_UNINITIALIZED_END      \
    _Pragma ("GCC diagnostic pop")
#else
# define YY_INITIAL_VALUE(Value) Value
#endif
#ifndef YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
# define YY_IGNORE_MAYBE_UNINITIALIZED_END
#endif
#ifndef YY_INITIAL_VALUE
# define YY_INITIAL_VALUE(Value) /* Nothing. */
#endif

#if defined __cplusplus && defined __GNUC__ && ! defined __ICC && 6 <= __GNUC__
# define YY_IGNORE_USELESS_CAST_BEGIN                          \
    _Pragma ("GCC diagnostic push")                            \
    _Pragma ("GCC diagnostic ignored \"-Wuseless-cast\"")
# define YY_IGNORE_USELESS_CAST_END            \
    _Pragma ("GCC diagnostic pop")
#endif
#ifndef YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_BEGIN
# define YY_IGNORE_USELESS_CAST_END
#endif


#define YY_ASSERT(E) ((void) (0 && (E)))

#if !defined yyoverflow

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
#    if ! defined _ALLOCA_H && ! defined EXIT_SUCCESS
#     include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
      /* Use EXIT_SUCCESS as a witness for stdlib.h.  */
#     ifndef EXIT_SUCCESS
#      define EXIT_SUCCESS 0
#     endif
#    endif
#   endif
#  endif
# endif

# ifdef YYSTACK_ALLOC
   /* Pacify GCC's 'empty if-body' warning.  */
#  define YYSTACK_FREE(Ptr) do { /* empty */; } while (0)
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
#  if (defined __cplusplus && ! defined EXIT_SUCCESS \
       && ! ((defined YYMALLOC || defined malloc) \
             && (defined YYFREE || defined free)))
#   include <stdlib.h> /* INFRINGES ON USER NAME SPACE */
#   ifndef EXIT_SUCCESS
#    define EXIT_SUCCESS 0
#   endif
#  endif
#  ifndef YYMALLOC
#   define YYMALLOC malloc
#   if ! defined malloc && ! defined EXIT_SUCCESS
void *malloc (YYSIZE_T); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
#  ifndef YYFREE
#   define YYFREE free
#   if ! defined free && ! defined EXIT_SUCCESS
void free (void *); /* INFRINGES ON USER NAME SPACE */
#   endif
#  endif
# endif
#endif /* !defined yyoverflow */

#if (! defined yyoverflow \
     && (! defined __cplusplus \
         || (defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL \
             && defined YYSTYPE_IS_TRIVIAL && YYSTYPE_IS_TRIVIAL)))

/* A type that is properly aligned for any stack member.  */
union yyalloc
{
  yy_state_t yyss_alloc;
  YYSTYPE yyvs_alloc;
  YYLTYPE yyls_alloc;
};

/* The size of the maximum gap between one aligned stack and the next.  */
# define YYSTACK_GAP_MAXIMUM (YYSIZEOF (union yyalloc) - 1)

/* The size of an array large to enough to hold all stacks, each with
   N elements.  */
# define YYSTACK_BYTES(N) \
     ((N) * (YYSIZEOF (yy_state_t) + YYSIZEOF (YYSTYPE) \
             + YYSIZEOF (YYLTYPE)) \
      + 2 * YYSTACK_GAP_MAXIMUM)

# define YYCOPY_NEEDED 1

/* Relocate STACK from its old location to the new one.  The
   local variables YYSIZE and YYSTACKSIZE give the old and new number of
   elements in the stack, and YYPTR gives the new location of the
   stack.  Advance YYPTR to a properly aligned location for the next
   stack.  */
# define YYSTACK_RELOCATE(Stack_alloc, Stack)                           \
    do                                                                  \
      {                                                                 \
        YYPTRDIFF_T yynewbytes;                                         \
        YYCOPY (&yyptr->Stack_alloc, Stack, yysize);                    \
        Stack = &yyptr->Stack_alloc;                                    \
        yynewbytes = yystacksize * YYSIZEOF (*Stack) + YYSTACK_GAP_MAXIMUM; \
        yyptr += yynewbytes / YYSIZEOF (*yyptr);                        \
      }                                                                 \
    while (0)

#endif

#if defined YYCOPY_NEEDED && YYCOPY_NEEDED
/* Copy COUNT objects from SRC to DST.  The source and destination do
   not overlap.  */
# ifndef YYCOPY
#  if defined __GNUC__ && 1 < __GNUC__
#   define YYCOPY(Dst, Src, Count) \
      __builtin_memcpy (Dst, Src, YY_CAST (YYSIZE_T, (Count)) * sizeof (*(Src)))
#  else
#   define YYCOPY(Dst, Src, Count)              \
      do                                        \
        {                                       \
          YYPTRDIFF_T yyi;                      \
          for (yyi = 0; yyi < (Count); yyi++)   \
            (Dst)[yyi] = (Src)[yyi];            \
        }                                       \
      while (0)
#  endif
# endif
#endif /* !YYCOPY_NEEDED */

/* YYFINAL -- State number of the termination state.  */
#define YYFINAL  6
/* YYLAST -- Last index in YYTABLE.  */
#define YYLAST   138

/* YYNTOKENS -- Number of terminals.  */
#define YYNTOKENS  48
/* YYNNTS -- Number of nonterminals.  */
#define YYNNTS  30
/* YYNRULES -- Number of rules.  */
#define YYNRULES  85
/* YYNSTATES -- Number of states.  */
#define YYNSTATES  149

/* YYMAXUTOK -- Last valid token kind.  */
#define YYMAXUTOK   279


/* YYTRANSLATE(TOKEN-NUM) -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex, with out-of-bounds checking.  */
#define YYTRANSLATE(YYX)                                \
  (0 <= (YYX) && (YYX) <= YYMAXUTOK                     \
   ? YY_CAST (yysymbol_kind_t, yytranslate[YYX])        \
   : YYSYMBOL_YYUNDEF)

/* YYTRANSLATE[TOKEN-NUM] -- Symbol number corresponding to TOKEN-NUM
   as returned by yylex.  */
static const yytype_int8 yytranslate[] =
{
       0,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    47,     2,     2,     2,    45,    41,     2,
      33,    35,    44,    42,    34,    43,     2,    26,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,    38,    25,
      36,    29,    30,    37,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,    31,     2,    32,    40,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,     2,     2,     2,     2,     2,     2,     2,
       2,     2,     2,    27,    39,    28,    46,     2,     2,     2,
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
      15,    16,    17,    18,    19,    20,    21,    22,    23,    24
};

#if YYDEBUG
/* YYRLINE[YYN] -- Source line where rule number YYN was defined.  */
static const yytype_int16 yyrline[] =
{
       0,   109,   109,   117,   121,   128,   129,   139,   142,   149,
     153,   161,   165,   170,   181,   200,   213,   220,   228,   231,
     238,   242,   246,   250,   258,   262,   266,   270,   274,   290,
     300,   308,   311,   315,   322,   338,   343,   362,   376,   383,
     384,   385,   392,   396,   397,   401,   402,   406,   407,   411,
     412,   416,   417,   421,   422,   426,   427,   428,   432,   433,
     434,   435,   436,   440,   441,   442,   446,   447,   448,   452,
     453,   462,   471,   475,   476,   477,   478,   483,   486,   490,
     498,   501,   505,   513,   517,   521
};
#endif

/** Accessing symbol of state STATE.  */
#define YY_ACCESSING_SYMBOL(State) YY_CAST (yysymbol_kind_t, yystos[State])

#if YYDEBUG || 0
/* The user-facing name of the symbol whose (internal) number is
   YYSYMBOL.  No bounds checking.  */
static const char *yysymbol_name (yysymbol_kind_t yysymbol) YY_ATTRIBUTE_UNUSED;

/* YYTNAME[SYMBOL-NUM] -- String name of the symbol SYMBOL-NUM.
   First, the terminals, then, starting at YYNTOKENS, nonterminals.  */
static const char *const yytname[] =
{
  "\"end of file\"", "error", "\"invalid token\"", "DT_V1", "DT_PLUGIN",
  "DT_MEMRESERVE", "DT_LSHIFT", "DT_RSHIFT", "DT_LE", "DT_GE", "DT_EQ",
  "DT_NE", "DT_AND", "DT_OR", "DT_BITS", "DT_DEL_PROP", "DT_DEL_NODE",
  "DT_PROPNODENAME", "DT_LITERAL", "DT_CHAR_LITERAL", "DT_BYTE",
  "DT_STRING", "DT_LABEL", "DT_REF", "DT_INCBIN", "';'", "'/'", "'{'",
  "'}'", "'='", "'>'", "'['", "']'", "'('", "','", "')'", "'<'", "'?'",
  "':'", "'|'", "'^'", "'&'", "'+'", "'-'", "'*'", "'%'", "'~'", "'!'",
  "$accept", "sourcefile", "header", "headers", "memreserves",
  "memreserve", "devicetree", "nodedef", "proplist", "propdef", "propdata",
  "propdataprefix", "arrayprefix", "integer_prim", "integer_expr",
  "integer_trinary", "integer_or", "integer_and", "integer_bitor",
  "integer_bitxor", "integer_bitand", "integer_eq", "integer_rela",
  "integer_shift", "integer_add", "integer_mul", "integer_unary",
  "bytestring", "subnodes", "subnode", YY_NULLPTR
};

static const char *
yysymbol_name (yysymbol_kind_t yysymbol)
{
  return yytname[yysymbol];
}
#endif

#define YYPACT_NINF (-44)

#define yypact_value_is_default(Yyn) \
  ((Yyn) == YYPACT_NINF)

#define YYTABLE_NINF (-1)

#define yytable_value_is_error(Yyn) \
  0

/* YYPACT[STATE-NUM] -- Index in YYTABLE of the portion describing
   STATE-NUM.  */
static const yytype_int8 yypact[] =
{
      14,    27,    61,    14,     8,    18,   -44,   -44,    37,     8,
      40,     8,    64,   -44,   -44,   -12,    37,   -44,    50,    52,
     -44,   -44,   -12,   -12,   -12,   -44,    51,   -44,    -4,    78,
      53,    54,    55,    17,     2,    30,    38,    -3,   -44,    66,
     -44,   -44,    70,    72,    50,    50,   -44,   -44,   -44,   -44,
     -12,   -12,   -12,   -12,   -12,   -12,   -12,   -12,   -12,   -12,
     -12,   -12,   -12,   -12,   -12,   -12,   -12,   -12,   -12,   -44,
       3,    73,    50,   -44,   -44,    78,    59,    53,    54,    55,
      17,     2,     2,    30,    30,    30,    30,    38,    38,    -3,
      -3,   -44,   -44,   -44,    82,    83,    44,     3,   -44,    74,
       3,   -44,   -44,   -12,    76,    79,   -44,   -44,   -44,   -44,
     -44,    80,   -44,   -44,   -44,   -44,   -44,   -10,    36,   -44,
     -44,   -44,   -44,    85,   -44,   -44,   -44,    75,   -44,   -44,
      21,    71,    88,    -6,   -44,   -44,   -44,   -44,   -44,    11,
     -44,   -44,   -44,    37,   -44,    77,    37,    81,   -44
};

/* YYDEFACT[STATE-NUM] -- Default reduction number in state STATE-NUM.
   Performed when YYTABLE does not specify something else to do.  Zero
   means the default is an error.  */
static const yytype_int8 yydefact[] =
{
       0,     0,     0,     5,     7,     3,     1,     6,     0,     0,
      16,     7,     0,    39,    40,     0,     0,    10,     0,     2,
       8,     4,     0,     0,     0,    73,     0,    42,    43,    45,
      47,    49,    51,    53,    55,    58,    65,    68,    72,     0,
      18,    11,     0,     0,     0,     0,    74,    75,    76,    41,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     0,
       0,     0,     0,     0,     0,     0,     0,     0,     0,     9,
      80,     0,     0,    14,    12,    46,     0,    48,    50,    52,
      54,    56,    57,    61,    62,    60,    59,    63,    64,    66,
      67,    70,    69,    71,     0,     0,     0,     0,    19,     0,
      80,    15,    13,     0,     0,     0,    21,    31,    83,    23,
      85,     0,    82,    81,    44,    22,    84,     0,     0,    17,
      30,    20,    32,     0,    24,    33,    27,     0,    77,    35,
       0,     0,     0,     0,    38,    37,    25,    36,    34,     0,
      78,    79,    26,     0,    29,     0,     0,     0,    28
};

/* YYPGOTO[NTERM-NUM].  */
static const yytype_int8 yypgoto[] =
{
     -44,   -44,   -44,   103,    99,   104,   -44,   -43,   -44,   -21,
     -44,   -44,   -44,    -8,    63,     9,   -44,    65,    67,    68,
      69,    62,    26,     4,    22,    23,   -19,   -44,    20,    28
};

/* YYDEFGOTO[NTERM-NUM].  */
static const yytype_uint8 yydefgoto[] =
{
       0,     2,     3,     4,    10,    11,    19,    41,    70,    98,
     117,   118,   130,    25,    26,    27,    28,    29,    30,    31,
      32,    33,    34,    35,    36,    37,    38,   133,    99,   100
};

/* YYTABLE[YYPACT[STATE-NUM]] -- What to do in state STATE-NUM.  If
   positive, shift that token.  If negative, reduce the rule whose
   number is the opposite.  If YYTABLE_NINF, syntax error.  */
static const yytype_uint8 yytable[] =
{
      16,    73,    74,    46,    47,    48,    13,    14,    39,    50,
      58,    59,   120,     8,   140,   121,   141,     1,    94,    95,
      96,    15,    12,    66,   122,    97,   142,    56,    57,   102,
       9,    22,    60,    51,    23,    24,    62,    63,    61,    13,
      14,    67,    68,   134,   135,   143,   144,    91,    92,    93,
     123,   136,     5,   108,    15,    13,    14,   124,   125,   126,
     127,     6,    83,    84,    85,    86,    18,   128,    42,   106,
      15,    40,   129,   107,    43,    44,   109,    40,    45,   112,
      64,    65,    81,    82,    87,    88,    49,    89,    90,    21,
      52,    69,    53,    71,    54,    72,    55,   103,   101,   104,
     105,   115,   111,   131,   116,   119,     7,   138,   132,   139,
      20,   146,   114,    17,    76,    75,   148,    80,     0,    77,
     113,    78,   137,    79,     0,   110,     0,     0,     0,     0,
       0,     0,     0,     0,     0,   145,     0,     0,   147
};

static const yytype_int16 yycheck[] =
{
       8,    44,    45,    22,    23,    24,    18,    19,    16,    13,
       8,     9,    22,     5,    20,    25,    22,     3,    15,    16,
      17,    33,     4,    26,    34,    22,    32,    10,    11,    72,
      22,    43,    30,    37,    46,    47,     6,     7,    36,    18,
      19,    44,    45,    22,    23,    34,    35,    66,    67,    68,
      14,    30,    25,    96,    33,    18,    19,    21,    22,    23,
      24,     0,    58,    59,    60,    61,    26,    31,    16,    25,
      33,    27,    36,    29,    22,    23,    97,    27,    26,   100,
      42,    43,    56,    57,    62,    63,    35,    64,    65,    25,
      12,    25,    39,    23,    40,    23,    41,    38,    25,    17,
      17,    25,    28,    18,    25,    25,     3,    36,    33,    21,
      11,    34,   103,     9,    51,    50,    35,    55,    -1,    52,
     100,    53,   130,    54,    -1,    97,    -1,    -1,    -1,    -1,
      -1,    -1,    -1,    -1,    -1,   143,    -1,    -1,   146
};

/* YYSTOS[STATE-NUM] -- The symbol kind of the accessing symbol of
   state STATE-NUM.  */
static const yytype_int8 yystos[] =
{
       0,     3,    49,    50,    51,    25,     0,    51,     5,    22,
      52,    53,     4,    18,    19,    33,    61,    53,    26,    54,
      52,    25,    43,    46,    47,    61,    62,    63,    64,    65,
      66,    67,    68,    69,    70,    71,    72,    73,    74,    61,
      27,    55,    16,    22,    23,    26,    74,    74,    74,    35,
      13,    37,    12,    39,    40,    41,    10,    11,     8,     9,
      30,    36,     6,     7,    42,    43,    26,    44,    45,    25,
      56,    23,    23,    55,    55,    65,    62,    66,    67,    68,
      69,    70,    70,    71,    71,    71,    71,    72,    72,    73,
      73,    74,    74,    74,    15,    16,    17,    22,    57,    76,
      77,    25,    55,    38,    17,    17,    25,    29,    55,    57,
      77,    28,    57,    76,    63,    25,    25,    58,    59,    25,
      22,    25,    34,    14,    21,    22,    23,    24,    31,    36,
      60,    18,    33,    75,    22,    23,    30,    61,    36,    21,
      20,    22,    32,    34,    35,    61,    34,    61,    35
};

/* YYR1[RULE-NUM] -- Symbol kind of the left-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr1[] =
{
       0,    48,    49,    50,    50,    51,    51,    52,    52,    53,
      53,    54,    54,    54,    54,    54,    54,    55,    56,    56,
      57,    57,    57,    57,    58,    58,    58,    58,    58,    58,
      58,    59,    59,    59,    60,    60,    60,    60,    60,    61,
      61,    61,    62,    63,    63,    64,    64,    65,    65,    66,
      66,    67,    67,    68,    68,    69,    69,    69,    70,    70,
      70,    70,    70,    71,    71,    71,    72,    72,    72,    73,
      73,    73,    73,    74,    74,    74,    74,    75,    75,    75,
      76,    76,    76,    77,    77,    77
};

/* YYR2[RULE-NUM] -- Number of symbols on the right-hand side of rule RULE-NUM.  */
static const yytype_int8 yyr2[] =
{
       0,     2,     3,     2,     4,     1,     2,     0,     2,     4,
       2,     2,     3,     4,     3,     4,     0,     5,     0,     2,
       4,     2,     3,     2,     2,     3,     4,     2,     9,     5,
       2,     0,     2,     2,     3,     1,     2,     2,     2,     1,
       1,     3,     1,     1,     5,     1,     3,     1,     3,     1,
       3,     1,     3,     1,     3,     1,     3,     3,     1,     3,
       3,     3,     3,     3,     3,     1,     3,     3,     1,     3,
       3,     3,     1,     1,     2,     2,     2,     0,     2,     2,
       0,     2,     2,     2,     3,     2
};


enum { YYENOMEM = -2 };

#define yyerrok         (yyerrstatus = 0)
#define yyclearin       (yychar = YYEMPTY)

#define YYACCEPT        goto yyacceptlab
#define YYABORT         goto yyabortlab
#define YYERROR         goto yyerrorlab
#define YYNOMEM         goto yyexhaustedlab


#define YYRECOVERING()  (!!yyerrstatus)

#define YYBACKUP(Token, Value)                                    \
  do                                                              \
    if (yychar == YYEMPTY)                                        \
      {                                                           \
        yychar = (Token);                                         \
        yylval = (Value);                                         \
        YYPOPSTACK (yylen);                                       \
        yystate = *yyssp;                                         \
        goto yybackup;                                            \
      }                                                           \
    else                                                          \
      {                                                           \
        yyerror (YY_("syntax error: cannot back up")); \
        YYERROR;                                                  \
      }                                                           \
  while (0)

/* Backward compatibility with an undocumented macro.
   Use YYerror or YYUNDEF. */
#define YYERRCODE YYUNDEF

/* YYLLOC_DEFAULT -- Set CURRENT to span from RHS[1] to RHS[N].
   If N is 0, then set CURRENT to the empty location which ends
   the previous symbol: RHS[0] (always defined).  */

#ifndef YYLLOC_DEFAULT
# define YYLLOC_DEFAULT(Current, Rhs, N)                                \
    do                                                                  \
      if (N)                                                            \
        {                                                               \
          (Current).first_line   = YYRHSLOC (Rhs, 1).first_line;        \
          (Current).first_column = YYRHSLOC (Rhs, 1).first_column;      \
          (Current).last_line    = YYRHSLOC (Rhs, N).last_line;         \
          (Current).last_column  = YYRHSLOC (Rhs, N).last_column;       \
        }                                                               \
      else                                                              \
        {                                                               \
          (Current).first_line   = (Current).last_line   =              \
            YYRHSLOC (Rhs, 0).last_line;                                \
          (Current).first_column = (Current).last_column =              \
            YYRHSLOC (Rhs, 0).last_column;                              \
        }                                                               \
    while (0)
#endif

#define YYRHSLOC(Rhs, K) ((Rhs)[K])


/* Enable debugging if requested.  */
#if YYDEBUG

# ifndef YYFPRINTF
#  include <stdio.h> /* INFRINGES ON USER NAME SPACE */
#  define YYFPRINTF fprintf
# endif

# define YYDPRINTF(Args)                        \
do {                                            \
  if (yydebug)                                  \
    YYFPRINTF Args;                             \
} while (0)


/* YYLOCATION_PRINT -- Print the location on the stream.
   This macro was not mandated originally: define only if we know
   we won't break user code: when these are the locations we know.  */

# ifndef YYLOCATION_PRINT

#  if defined YY_LOCATION_PRINT

   /* Temporary convenience wrapper in case some people defined the
      undocumented and private YY_LOCATION_PRINT macros.  */
#   define YYLOCATION_PRINT(File, Loc)  YY_LOCATION_PRINT(File, *(Loc))

#  elif defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL

/* Print *YYLOCP on YYO.  Private, do not rely on its existence. */

YY_ATTRIBUTE_UNUSED
static int
yy_location_print_ (FILE *yyo, YYLTYPE const * const yylocp)
{
  int res = 0;
  int end_col = 0 != yylocp->last_column ? yylocp->last_column - 1 : 0;
  if (0 <= yylocp->first_line)
    {
      res += YYFPRINTF (yyo, "%d", yylocp->first_line);
      if (0 <= yylocp->first_column)
        res += YYFPRINTF (yyo, ".%d", yylocp->first_column);
    }
  if (0 <= yylocp->last_line)
    {
      if (yylocp->first_line < yylocp->last_line)
        {
          res += YYFPRINTF (yyo, "-%d", yylocp->last_line);
          if (0 <= end_col)
            res += YYFPRINTF (yyo, ".%d", end_col);
        }
      else if (0 <= end_col && yylocp->first_column < end_col)
        res += YYFPRINTF (yyo, "-%d", end_col);
    }
  return res;
}

#   define YYLOCATION_PRINT  yy_location_print_

    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT(File, Loc)  YYLOCATION_PRINT(File, &(Loc))

#  else

#   define YYLOCATION_PRINT(File, Loc) ((void) 0)
    /* Temporary convenience wrapper in case some people defined the
       undocumented and private YY_LOCATION_PRINT macros.  */
#   define YY_LOCATION_PRINT  YYLOCATION_PRINT

#  endif
# endif /* !defined YYLOCATION_PRINT */


# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)                    \
do {                                                                      \
  if (yydebug)                                                            \
    {                                                                     \
      YYFPRINTF (stderr, "%s ", Title);                                   \
      yy_symbol_print (stderr,                                            \
                  Kind, Value, Location); \
      YYFPRINTF (stderr, "\n");                                           \
    }                                                                     \
} while (0)


/*-----------------------------------.
| Print this symbol's value on YYO.  |
`-----------------------------------*/

static void
yy_symbol_value_print (FILE *yyo,
                       yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  FILE *yyoutput = yyo;
  YY_USE (yyoutput);
  YY_USE (yylocationp);
  if (!yyvaluep)
    return;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/*---------------------------.
| Print this symbol on YYO.  |
`---------------------------*/

static void
yy_symbol_print (FILE *yyo,
                 yysymbol_kind_t yykind, YYSTYPE const * const yyvaluep, YYLTYPE const * const yylocationp)
{
  YYFPRINTF (yyo, "%s %s (",
             yykind < YYNTOKENS ? "token" : "nterm", yysymbol_name (yykind));

  YYLOCATION_PRINT (yyo, yylocationp);
  YYFPRINTF (yyo, ": ");
  yy_symbol_value_print (yyo, yykind, yyvaluep, yylocationp);
  YYFPRINTF (yyo, ")");
}

/*------------------------------------------------------------------.
| yy_stack_print -- Print the state stack from its BOTTOM up to its |
| TOP (included).                                                   |
`------------------------------------------------------------------*/

static void
yy_stack_print (yy_state_t *yybottom, yy_state_t *yytop)
{
  YYFPRINTF (stderr, "Stack now");
  for (; yybottom <= yytop; yybottom++)
    {
      int yybot = *yybottom;
      YYFPRINTF (stderr, " %d", yybot);
    }
  YYFPRINTF (stderr, "\n");
}

# define YY_STACK_PRINT(Bottom, Top)                            \
do {                                                            \
  if (yydebug)                                                  \
    yy_stack_print ((Bottom), (Top));                           \
} while (0)


/*------------------------------------------------.
| Report that the YYRULE is going to be reduced.  |
`------------------------------------------------*/

static void
yy_reduce_print (yy_state_t *yyssp, YYSTYPE *yyvsp, YYLTYPE *yylsp,
                 int yyrule)
{
  int yylno = yyrline[yyrule];
  int yynrhs = yyr2[yyrule];
  int yyi;
  YYFPRINTF (stderr, "Reducing stack by rule %d (line %d):\n",
             yyrule - 1, yylno);
  /* The symbols being reduced.  */
  for (yyi = 0; yyi < yynrhs; yyi++)
    {
      YYFPRINTF (stderr, "   $%d = ", yyi + 1);
      yy_symbol_print (stderr,
                       YY_ACCESSING_SYMBOL (+yyssp[yyi + 1 - yynrhs]),
                       &yyvsp[(yyi + 1) - (yynrhs)],
                       &(yylsp[(yyi + 1) - (yynrhs)]));
      YYFPRINTF (stderr, "\n");
    }
}

# define YY_REDUCE_PRINT(Rule)          \
do {                                    \
  if (yydebug)                          \
    yy_reduce_print (yyssp, yyvsp, yylsp, Rule); \
} while (0)

/* Nonzero means print parse trace.  It is left uninitialized so that
   multiple parsers can coexist.  */
int yydebug;
#else /* !YYDEBUG */
# define YYDPRINTF(Args) ((void) 0)
# define YY_SYMBOL_PRINT(Title, Kind, Value, Location)
# define YY_STACK_PRINT(Bottom, Top)
# define YY_REDUCE_PRINT(Rule)
#endif /* !YYDEBUG */


/* YYINITDEPTH -- initial size of the parser's stacks.  */
#ifndef YYINITDEPTH
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






/*-----------------------------------------------.
| Release the memory associated to this symbol.  |
`-----------------------------------------------*/

static void
yydestruct (const char *yymsg,
            yysymbol_kind_t yykind, YYSTYPE *yyvaluep, YYLTYPE *yylocationp)
{
  YY_USE (yyvaluep);
  YY_USE (yylocationp);
  if (!yymsg)
    yymsg = "Deleting";
  YY_SYMBOL_PRINT (yymsg, yykind, yyvaluep, yylocationp);

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  YY_USE (yykind);
  YY_IGNORE_MAYBE_UNINITIALIZED_END
}


/* Lookahead token kind.  */
int yychar;

/* The semantic value of the lookahead symbol.  */
YYSTYPE yylval;
/* Location data for the lookahead symbol.  */
YYLTYPE yylloc
# if defined YYLTYPE_IS_TRIVIAL && YYLTYPE_IS_TRIVIAL
  = { 1, 1, 1, 1 }
# endif
;
/* Number of syntax errors so far.  */
int yynerrs;




/*----------.
| yyparse.  |
`----------*/

int
yyparse (void)
{
    yy_state_fast_t yystate = 0;
    /* Number of tokens to shift before error messages enabled.  */
    int yyerrstatus = 0;

    /* Refer to the stacks through separate pointers, to allow yyoverflow
       to reallocate them elsewhere.  */

    /* Their size.  */
    YYPTRDIFF_T yystacksize = YYINITDEPTH;

    /* The state stack: array, bottom, top.  */
    yy_state_t yyssa[YYINITDEPTH];
    yy_state_t *yyss = yyssa;
    yy_state_t *yyssp = yyss;

    /* The semantic value stack: array, bottom, top.  */
    YYSTYPE yyvsa[YYINITDEPTH];
    YYSTYPE *yyvs = yyvsa;
    YYSTYPE *yyvsp = yyvs;

    /* The location stack: array, bottom, top.  */
    YYLTYPE yylsa[YYINITDEPTH];
    YYLTYPE *yyls = yylsa;
    YYLTYPE *yylsp = yyls;

  int yyn;
  /* The return value of yyparse.  */
  int yyresult;
  /* Lookahead symbol kind.  */
  yysymbol_kind_t yytoken = YYSYMBOL_YYEMPTY;
  /* The variables used to return semantic value and location from the
     action routines.  */
  YYSTYPE yyval;
  YYLTYPE yyloc;

  /* The locations where the error started and ended.  */
  YYLTYPE yyerror_range[3];



#define YYPOPSTACK(N)   (yyvsp -= (N), yyssp -= (N), yylsp -= (N))

  /* The number of symbols on the RHS of the reduced rule.
     Keep to zero when no symbol should be popped.  */
  int yylen = 0;

  YYDPRINTF ((stderr, "Starting parse\n"));

  yychar = YYEMPTY; /* Cause a token to be read.  */

  yylsp[0] = yylloc;
  goto yysetstate;


/*------------------------------------------------------------.
| yynewstate -- push a new state, which is found in yystate.  |
`------------------------------------------------------------*/
yynewstate:
  /* In all cases, when you get here, the value and location stacks
     have just been pushed.  So pushing a state here evens the stacks.  */
  yyssp++;


/*--------------------------------------------------------------------.
| yysetstate -- set current state (the top of the stack) to yystate.  |
`--------------------------------------------------------------------*/
yysetstate:
  YYDPRINTF ((stderr, "Entering state %d\n", yystate));
  YY_ASSERT (0 <= yystate && yystate < YYNSTATES);
  YY_IGNORE_USELESS_CAST_BEGIN
  *yyssp = YY_CAST (yy_state_t, yystate);
  YY_IGNORE_USELESS_CAST_END
  YY_STACK_PRINT (yyss, yyssp);

  if (yyss + yystacksize - 1 <= yyssp)
#if !defined yyoverflow && !defined YYSTACK_RELOCATE
    YYNOMEM;
#else
    {
      /* Get the current used size of the three stacks, in elements.  */
      YYPTRDIFF_T yysize = yyssp - yyss + 1;

# if defined yyoverflow
      {
        /* Give user a chance to reallocate the stack.  Use copies of
           these so that the &'s don't force the real ones into
           memory.  */
        yy_state_t *yyss1 = yyss;
        YYSTYPE *yyvs1 = yyvs;
        YYLTYPE *yyls1 = yyls;

        /* Each stack pointer address is followed by the size of the
           data in use in that stack, in bytes.  This used to be a
           conditional around just the two extra args, but that might
           be undefined if yyoverflow is a macro.  */
        yyoverflow (YY_("memory exhausted"),
                    &yyss1, yysize * YYSIZEOF (*yyssp),
                    &yyvs1, yysize * YYSIZEOF (*yyvsp),
                    &yyls1, yysize * YYSIZEOF (*yylsp),
                    &yystacksize);
        yyss = yyss1;
        yyvs = yyvs1;
        yyls = yyls1;
      }
# else /* defined YYSTACK_RELOCATE */
      /* Extend the stack our own way.  */
      if (YYMAXDEPTH <= yystacksize)
        YYNOMEM;
      yystacksize *= 2;
      if (YYMAXDEPTH < yystacksize)
        yystacksize = YYMAXDEPTH;

      {
        yy_state_t *yyss1 = yyss;
        union yyalloc *yyptr =
          YY_CAST (union yyalloc *,
                   YYSTACK_ALLOC (YY_CAST (YYSIZE_T, YYSTACK_BYTES (yystacksize))));
        if (! yyptr)
          YYNOMEM;
        YYSTACK_RELOCATE (yyss_alloc, yyss);
        YYSTACK_RELOCATE (yyvs_alloc, yyvs);
        YYSTACK_RELOCATE (yyls_alloc, yyls);
#  undef YYSTACK_RELOCATE
        if (yyss1 != yyssa)
          YYSTACK_FREE (yyss1);
      }
# endif

      yyssp = yyss + yysize - 1;
      yyvsp = yyvs + yysize - 1;
      yylsp = yyls + yysize - 1;

      YY_IGNORE_USELESS_CAST_BEGIN
      YYDPRINTF ((stderr, "Stack size increased to %ld\n",
                  YY_CAST (long, yystacksize)));
      YY_IGNORE_USELESS_CAST_END

      if (yyss + yystacksize - 1 <= yyssp)
        YYABORT;
    }
#endif /* !defined yyoverflow && !defined YYSTACK_RELOCATE */


  if (yystate == YYFINAL)
    YYACCEPT;

  goto yybackup;


/*-----------.
| yybackup.  |
`-----------*/
yybackup:
  /* Do appropriate processing given the current state.  Read a
     lookahead token if we need one and don't already have one.  */

  /* First try to decide what to do without reference to lookahead token.  */
  yyn = yypact[yystate];
  if (yypact_value_is_default (yyn))
    goto yydefault;

  /* Not known => get a lookahead token if don't already have one.  */

  /* YYCHAR is either empty, or end-of-input, or a valid lookahead.  */
  if (yychar == YYEMPTY)
    {
      YYDPRINTF ((stderr, "Reading a token\n"));
      yychar = yylex ();
    }

  if (yychar <= YYEOF)
    {
      yychar = YYEOF;
      yytoken = YYSYMBOL_YYEOF;
      YYDPRINTF ((stderr, "Now at end of input.\n"));
    }
  else if (yychar == YYerror)
    {
      /* The scanner already issued an error message, process directly
         to error recovery.  But do not keep the error token as
         lookahead, it is too special and may lead us to an endless
         loop in error recovery. */
      yychar = YYUNDEF;
      yytoken = YYSYMBOL_YYerror;
      yyerror_range[1] = yylloc;
      goto yyerrlab1;
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
      if (yytable_value_is_error (yyn))
        goto yyerrlab;
      yyn = -yyn;
      goto yyreduce;
    }

  /* Count tokens shifted since error; after three, turn off error
     status.  */
  if (yyerrstatus)
    yyerrstatus--;

  /* Shift the lookahead token.  */
  YY_SYMBOL_PRINT ("Shifting", yytoken, &yylval, &yylloc);
  yystate = yyn;
  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END
  *++yylsp = yylloc;

  /* Discard the shifted token.  */
  yychar = YYEMPTY;
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
| yyreduce -- do a reduction.  |
`-----------------------------*/
yyreduce:
  /* yyn is the number of a rule to reduce with.  */
  yylen = yyr2[yyn];

  /* If YYLEN is nonzero, implement the default value of the action:
     '$$ = $1'.

     Otherwise, the following line sets YYVAL to garbage.
     This behavior is undocumented and Bison
     users should not rely upon it.  Assigning to YYVAL
     unconditionally makes the parser a bit smaller, and it avoids a
     GCC warning that YYVAL may be used uninitialized.  */
  yyval = yyvsp[1-yylen];

  /* Default location. */
  YYLLOC_DEFAULT (yyloc, (yylsp - yylen), yylen);
  yyerror_range[1] = yyloc;
  YY_REDUCE_PRINT (yyn);
  switch (yyn)
    {
  case 2: /* sourcefile: headers memreserves devicetree  */
#line 110 "../scripts/dtc-aosp/dtc/dtc-parser.y"
                {
			parser_output = build_dt_info((yyvsp[-2].flags), (yyvsp[-1].re), (yyvsp[0].node),
			                              guess_boot_cpuid((yyvsp[0].node)));
		}
#line 1369 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"
    break;

  case 3: /* header: DT_V1 ';'  */
#line 118 "../scripts/dtc-aosp/dtc/dtc-parser.y"
                {
			(yyval.flags) = DTSF_V1;
		}
#line 1377 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"
    break;

  case 4: /* header: DT_V1 ';' DT_PLUGIN ';'  */
#line 122 "../scripts/dtc-aosp/dtc/dtc-parser.y"
                {
			(yyval.flags) = DTSF_V1 | DTSF_PLUGIN;
		}
#line 1385 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"
    break;

  case 6: /* headers: header headers  */
#line 130 "../scripts/dtc-aosp/dtc/dtc-parser.y"
                {
			if ((yyvsp[0].flags) != (yyvsp[-1].flags))
				ERROR(&(yylsp[0]), "Header flags don't match earlier ones");
			(yyval.flags) = (yyvsp[-1].flags);
		}
#line 1395 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"
    break;

  case 7: /* memreserves: %empty  */
#line 139 "../scripts/dtc-aosp/dtc/dtc-parser.y"
                {
			(yyval.re) = NULL;
		}
#line 1403 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"
    break;

  case 8: /* memreserves: memreserve memreserves  */
#line 143 "../scripts/dtc-aosp/dtc/dtc-parser.y"
                {
			(yyval.re) = chain_reserve_entry((yyvsp[-1].re), (yyvsp[0].re));
		}
#line 1411 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"
    break;

  case 9: /* memreserve: DT_MEMRESERVE integer_prim integer_prim ';'  */
#line 150 "../scripts/dtc-aosp/dtc/dtc-parser.y"
                {
			(yyval.re) = build_reserve_entry((yyvsp[-2].integer), (yyvsp[-1].integer));
		}
#line 1419 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"
    break;

  case 10: /* memreserve: DT_LABEL memreserve  */
#line 154 "../scripts/dtc-aosp/dtc/dtc-parser.y"
                {
			add_label(&(yyvsp[0].re)->labels, (yyvsp[-1].labelref));
			(yyval.re) = (yyvsp[0].re);
		}
#line 1428 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"
    break;

  case 11: /* devicetree: '/' nodedef  */
#line 162 "../scripts/dtc-aosp/dtc/dtc-parser.y"
                {
			(yyval.node) = name_node((yyvsp[0].node), "");
		}
#line 1436 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"
    break;

  case 12: /* devicetree: devicetree '/' nodedef  */
#line 166 "../scripts/dtc-aosp/dtc/dtc-parser.y"
                {
			(yyval.node) = merge_nodes((yyvsp[-2].node), (yyvsp[0].node));
		}
#line 1444 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"
    break;

  case 13: /* devicetree: devicetree DT_LABEL DT_REF nodedef  */
#line 171 "../scripts/dtc-aosp/dtc/dtc-parser.y"
                {
			struct node *target = get_node_by_ref((yyvsp[-3].node), (yyvsp[-1].labelref));

			if (target) {
				add_label(&target->labels, (yyvsp[-2].labelref));
				merge_nodes(target, (yyvsp[0].node));
			} else
				ERROR(&(yylsp[-1]), "Label or path %s not found", (yyvsp[-1].labelref));
			(yyval.node) = (yyvsp[-3].node);
		}
#line 1459 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"
    break;

  case 14: /* devicetree: devicetree DT_REF nodedef  */
#line 182 "../scripts/dtc-aosp/dtc/dtc-parser.y"
                {
			struct node *target = get_node_by_ref((yyvsp[-2].node), (yyvsp[-1].labelref));

			if (target) {
				merge_nodes(target, (yyvsp[0].node));
			} else {
				/*
				 * We rely on the rule being always:
				 *   versioninfo plugindecl memreserves devicetree
				 * so $-1 is what we want (plugindecl)
				 */
				if ((yyvsp[(-1) - (3)].flags) & DTSF_PLUGIN)
					add_orphan_node((yyvsp[-2].node), (yyvsp[0].node), (yyvsp[-1].labelref));
				else
					ERROR(&(yylsp[-1]), "Label or path %s not found", (yyvsp[-1].labelref));
			}
			(yyval.node) = (yyvsp[-2].node);
		}
#line 1482 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"
    break;

  case 15: /* devicetree: devicetree DT_DEL_NODE DT_REF ';'  */
#line 201 "../scripts/dtc-aosp/dtc/dtc-parser.y"
                {
			struct node *target = get_node_by_ref((yyvsp[-3].node), (yyvsp[-1].labelref));

			if (target)
				delete_node(target);
			else
				ERROR(&(yylsp[-1]), "Label or path %s not found", (yyvsp[-1].labelref));


			(yyval.node) = (yyvsp[-3].node);
		}
#line 1498 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"
    break;

  case 16: /* devicetree: %empty  */
#line 213 "../scripts/dtc-aosp/dtc/dtc-parser.y"
                {
			/* build empty node */
			(yyval.node) = name_node(build_node(NULL, NULL), "");
		}
#line 1507 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"
    break;

  case 17: /* nodedef: '{' proplist subnodes '}' ';'  */
#line 221 "../scripts/dtc-aosp/dtc/dtc-parser.y"
                {
			(yyval.node) = build_node((yyvsp[-3].proplist), (yyvsp[-2].nodelist));
		}
#line 1515 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"
    break;

  case 18: /* proplist: %empty  */
#line 228 "../scripts/dtc-aosp/dtc/dtc-parser.y"
                {
			(yyval.proplist) = NULL;
		}
#line 1523 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"
    break;

  case 19: /* proplist: proplist propdef  */
#line 232 "../scripts/dtc-aosp/dtc/dtc-parser.y"
                {
			(yyval.proplist) = chain_property((yyvsp[0].prop), (yyvsp[-1].proplist));
		}
#line 1531 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"
    break;

  case 20: /* propdef: DT_PROPNODENAME '=' propdata ';'  */
#line 239 "../scripts/dtc-aosp/dtc/dtc-parser.y"
                {
			(yyval.prop) = build_property((yyvsp[-3].propnodename), (yyvsp[-1].data));
		}
#line 1539 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"
    break;

  case 21: /* propdef: DT_PROPNODENAME ';'  */
#line 243 "../scripts/dtc-aosp/dtc/dtc-parser.y"
                {
			(yyval.prop) = build_property((yyvsp[-1].propnodename), empty_data);
		}
#line 1547 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"
    break;

  case 22: /* propdef: DT_DEL_PROP DT_PROPNODENAME ';'  */
#line 247 "../scripts/dtc-aosp/dtc/dtc-parser.y"
                {
			(yyval.prop) = build_property_delete((yyvsp[-1].propnodename));
		}
#line 1555 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"
    break;

  case 23: /* propdef: DT_LABEL propdef  */
#line 251 "../scripts/dtc-aosp/dtc/dtc-parser.y"
                {
			add_label(&(yyvsp[0].prop)->labels, (yyvsp[-1].labelref));
			(yyval.prop) = (yyvsp[0].prop);
		}
#line 1564 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"
    break;

  case 24: /* propdata: propdataprefix DT_STRING  */
#line 259 "../scripts/dtc-aosp/dtc/dtc-parser.y"
                {
			(yyval.data) = data_merge((yyvsp[-1].data), (yyvsp[0].data));
		}
#line 1572 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"
    break;

  case 25: /* propdata: propdataprefix arrayprefix '>'  */
#line 263 "../scripts/dtc-aosp/dtc/dtc-parser.y"
                {
			(yyval.data) = data_merge((yyvsp[-2].data), (yyvsp[-1].array).data);
		}
#line 1580 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"
    break;

  case 26: /* propdata: propdataprefix '[' bytestring ']'  */
#line 267 "../scripts/dtc-aosp/dtc/dtc-parser.y"
                {
			(yyval.data) = data_merge((yyvsp[-3].data), (yyvsp[-1].data));
		}
#line 1588 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"
    break;

  case 27: /* propdata: propdataprefix DT_REF  */
#line 271 "../scripts/dtc-aosp/dtc/dtc-parser.y"
                {
			(yyval.data) = data_add_marker((yyvsp[-1].data), REF_PATH, (yyvsp[0].labelref));
		}
#line 1596 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"
    break;

  case 28: /* propdata: propdataprefix DT_INCBIN '(' DT_STRING ',' integer_prim ',' integer_prim ')'  */
#line 275 "../scripts/dtc-aosp/dtc/dtc-parser.y"
                {
			FILE *f = srcfile_relative_open((yyvsp[-5].data).val, NULL);
			struct data d;

			if ((yyvsp[-3].integer) != 0)
				if (fseek(f, (yyvsp[-3].integer), SEEK_SET) != 0)
					die("Couldn't seek to offset %llu in \"%s\": %s",
					    (unsigned long long)(yyvsp[-3].integer), (yyvsp[-5].data).val,
					    strerror(errno));

			d = data_copy_file(f, (yyvsp[-1].integer));

			(yyval.data) = data_merge((yyvsp[-8].data), d);
			fclose(f);
		}
#line 1616 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"
    break;

  case 29: /* propdata: propdataprefix DT_INCBIN '(' DT_STRING ')'  */
#line 291 "../scripts/dtc-aosp/dtc/dtc-parser.y"
                {
			FILE *f = srcfile_relative_open((yyvsp[-1].data).val, NULL);
			struct data d = empty_data;

			d = data_copy_file(f, -1);

			(yyval.data) = data_merge((yyvsp[-4].data), d);
			fclose(f);
		}
#line 1630 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"
    break;

  case 30: /* propdata: propdata DT_LABEL  */
#line 301 "../scripts/dtc-aosp/dtc/dtc-parser.y"
                {
			(yyval.data) = data_add_marker((yyvsp[-1].data), LABEL, (yyvsp[0].labelref));
		}
#line 1638 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"
    break;

  case 31: /* propdataprefix: %empty  */
#line 308 "../scripts/dtc-aosp/dtc/dtc-parser.y"
                {
			(yyval.data) = empty_data;
		}
#line 1646 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"
    break;

  case 32: /* propdataprefix: propdata ','  */
#line 312 "../scripts/dtc-aosp/dtc/dtc-parser.y"
                {
			(yyval.data) = (yyvsp[-1].data);
		}
#line 1654 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"
    break;

  case 33: /* propdataprefix: propdataprefix DT_LABEL  */
#line 316 "../scripts/dtc-aosp/dtc/dtc-parser.y"
                {
			(yyval.data) = data_add_marker((yyvsp[-1].data), LABEL, (yyvsp[0].labelref));
		}
#line 1662 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"
    break;

  case 34: /* arrayprefix: DT_BITS DT_LITERAL '<'  */
#line 323 "../scripts/dtc-aosp/dtc/dtc-parser.y"
                {
			unsigned long long bits;

			bits = (yyvsp[-1].integer);

			if ((bits !=  8) && (bits != 16) &&
			    (bits != 32) && (bits != 64)) {
				ERROR(&(yylsp[-1]), "Array elements must be"
				      " 8, 16, 32 or 64-bits");
				bits = 32;
			}

			(yyval.array).data = empty_data;
			(yyval.array).bits = bits;
		}
#line 1682 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"
    break;

  case 35: /* arrayprefix: '<'  */
#line 339 "../scripts/dtc-aosp/dtc/dtc-parser.y"
                {
			(yyval.array).data = empty_data;
			(yyval.array).bits = 32;
		}
#line 1691 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"
    break;

  case 36: /* arrayprefix: arrayprefix integer_prim  */
#line 344 "../scripts/dtc-aosp/dtc/dtc-parser.y"
                {
			if ((yyvsp[-1].array).bits < 64) {
				uint64_t mask = (1ULL << (yyvsp[-1].array).bits) - 1;
				/*
				 * Bits above mask must either be all zero
				 * (positive within range of mask) or all one
				 * (negative and sign-extended). The second
				 * condition is true if when we set all bits
				 * within the mask to one (i.e. | in the
				 * mask), all bits are one.
				 */
				if (((yyvsp[0].integer) > mask) && (((yyvsp[0].integer) | mask) != -1ULL))
					ERROR(&(yylsp[0]), "Value out of range for"
					      " %d-bit array element", (yyvsp[-1].array).bits);
			}

			(yyval.array).data = data_append_integer((yyvsp[-1].array).data, (yyvsp[0].integer), (yyvsp[-1].array).bits);
		}
#line 1714 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"
    break;

  case 37: /* arrayprefix: arrayprefix DT_REF  */
#line 363 "../scripts/dtc-aosp/dtc/dtc-parser.y"
                {
			uint64_t val = ~0ULL >> (64 - (yyvsp[-1].array).bits);

			if ((yyvsp[-1].array).bits == 32)
				(yyvsp[-1].array).data = data_add_marker((yyvsp[-1].array).data,
							  REF_PHANDLE,
							  (yyvsp[0].labelref));
			else
				ERROR(&(yylsp[0]), "References are only allowed in "
					    "arrays with 32-bit elements.");

			(yyval.array).data = data_append_integer((yyvsp[-1].array).data, val, (yyvsp[-1].array).bits);
		}
#line 1732 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"
    break;

  case 38: /* arrayprefix: arrayprefix DT_LABEL  */
#line 377 "../scripts/dtc-aosp/dtc/dtc-parser.y"
                {
			(yyval.array).data = data_add_marker((yyvsp[-1].array).data, LABEL, (yyvsp[0].labelref));
		}
#line 1740 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"
    break;

  case 41: /* integer_prim: '(' integer_expr ')'  */
#line 386 "../scripts/dtc-aosp/dtc/dtc-parser.y"
                {
			(yyval.integer) = (yyvsp[-1].integer);
		}
#line 1748 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"
    break;

  case 44: /* integer_trinary: integer_or '?' integer_expr ':' integer_trinary  */
#line 397 "../scripts/dtc-aosp/dtc/dtc-parser.y"
                                                          { (yyval.integer) = (yyvsp[-4].integer) ? (yyvsp[-2].integer) : (yyvsp[0].integer); }
#line 1754 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"
    break;

  case 46: /* integer_or: integer_or DT_OR integer_and  */
#line 402 "../scripts/dtc-aosp/dtc/dtc-parser.y"
                                       { (yyval.integer) = (yyvsp[-2].integer) || (yyvsp[0].integer); }
#line 1760 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"
    break;

  case 48: /* integer_and: integer_and DT_AND integer_bitor  */
#line 407 "../scripts/dtc-aosp/dtc/dtc-parser.y"
                                           { (yyval.integer) = (yyvsp[-2].integer) && (yyvsp[0].integer); }
#line 1766 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"
    break;

  case 50: /* integer_bitor: integer_bitor '|' integer_bitxor  */
#line 412 "../scripts/dtc-aosp/dtc/dtc-parser.y"
                                           { (yyval.integer) = (yyvsp[-2].integer) | (yyvsp[0].integer); }
#line 1772 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"
    break;

  case 52: /* integer_bitxor: integer_bitxor '^' integer_bitand  */
#line 417 "../scripts/dtc-aosp/dtc/dtc-parser.y"
                                            { (yyval.integer) = (yyvsp[-2].integer) ^ (yyvsp[0].integer); }
#line 1778 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"
    break;

  case 54: /* integer_bitand: integer_bitand '&' integer_eq  */
#line 422 "../scripts/dtc-aosp/dtc/dtc-parser.y"
                                        { (yyval.integer) = (yyvsp[-2].integer) & (yyvsp[0].integer); }
#line 1784 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"
    break;

  case 56: /* integer_eq: integer_eq DT_EQ integer_rela  */
#line 427 "../scripts/dtc-aosp/dtc/dtc-parser.y"
                                        { (yyval.integer) = (yyvsp[-2].integer) == (yyvsp[0].integer); }
#line 1790 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"
    break;

  case 57: /* integer_eq: integer_eq DT_NE integer_rela  */
#line 428 "../scripts/dtc-aosp/dtc/dtc-parser.y"
                                        { (yyval.integer) = (yyvsp[-2].integer) != (yyvsp[0].integer); }
#line 1796 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"
    break;

  case 59: /* integer_rela: integer_rela '<' integer_shift  */
#line 433 "../scripts/dtc-aosp/dtc/dtc-parser.y"
                                         { (yyval.integer) = (yyvsp[-2].integer) < (yyvsp[0].integer); }
#line 1802 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"
    break;

  case 60: /* integer_rela: integer_rela '>' integer_shift  */
#line 434 "../scripts/dtc-aosp/dtc/dtc-parser.y"
                                         { (yyval.integer) = (yyvsp[-2].integer) > (yyvsp[0].integer); }
#line 1808 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"
    break;

  case 61: /* integer_rela: integer_rela DT_LE integer_shift  */
#line 435 "../scripts/dtc-aosp/dtc/dtc-parser.y"
                                           { (yyval.integer) = (yyvsp[-2].integer) <= (yyvsp[0].integer); }
#line 1814 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"
    break;

  case 62: /* integer_rela: integer_rela DT_GE integer_shift  */
#line 436 "../scripts/dtc-aosp/dtc/dtc-parser.y"
                                           { (yyval.integer) = (yyvsp[-2].integer) >= (yyvsp[0].integer); }
#line 1820 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"
    break;

  case 63: /* integer_shift: integer_shift DT_LSHIFT integer_add  */
#line 440 "../scripts/dtc-aosp/dtc/dtc-parser.y"
                                              { (yyval.integer) = (yyvsp[-2].integer) << (yyvsp[0].integer); }
#line 1826 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"
    break;

  case 64: /* integer_shift: integer_shift DT_RSHIFT integer_add  */
#line 441 "../scripts/dtc-aosp/dtc/dtc-parser.y"
                                              { (yyval.integer) = (yyvsp[-2].integer) >> (yyvsp[0].integer); }
#line 1832 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"
    break;

  case 66: /* integer_add: integer_add '+' integer_mul  */
#line 446 "../scripts/dtc-aosp/dtc/dtc-parser.y"
                                      { (yyval.integer) = (yyvsp[-2].integer) + (yyvsp[0].integer); }
#line 1838 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"
    break;

  case 67: /* integer_add: integer_add '-' integer_mul  */
#line 447 "../scripts/dtc-aosp/dtc/dtc-parser.y"
                                      { (yyval.integer) = (yyvsp[-2].integer) - (yyvsp[0].integer); }
#line 1844 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"
    break;

  case 69: /* integer_mul: integer_mul '*' integer_unary  */
#line 452 "../scripts/dtc-aosp/dtc/dtc-parser.y"
                                        { (yyval.integer) = (yyvsp[-2].integer) * (yyvsp[0].integer); }
#line 1850 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"
    break;

  case 70: /* integer_mul: integer_mul '/' integer_unary  */
#line 454 "../scripts/dtc-aosp/dtc/dtc-parser.y"
                {
			if ((yyvsp[0].integer) != 0) {
				(yyval.integer) = (yyvsp[-2].integer) / (yyvsp[0].integer);
			} else {
				ERROR(&(yyloc), "Division by zero");
				(yyval.integer) = 0;
			}
		}
#line 1863 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"
    break;

  case 71: /* integer_mul: integer_mul '%' integer_unary  */
#line 463 "../scripts/dtc-aosp/dtc/dtc-parser.y"
                {
			if ((yyvsp[0].integer) != 0) {
				(yyval.integer) = (yyvsp[-2].integer) % (yyvsp[0].integer);
			} else {
				ERROR(&(yyloc), "Division by zero");
				(yyval.integer) = 0;
			}
		}
#line 1876 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"
    break;

  case 74: /* integer_unary: '-' integer_unary  */
#line 476 "../scripts/dtc-aosp/dtc/dtc-parser.y"
                            { (yyval.integer) = -(yyvsp[0].integer); }
#line 1882 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"
    break;

  case 75: /* integer_unary: '~' integer_unary  */
#line 477 "../scripts/dtc-aosp/dtc/dtc-parser.y"
                            { (yyval.integer) = ~(yyvsp[0].integer); }
#line 1888 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"
    break;

  case 76: /* integer_unary: '!' integer_unary  */
#line 478 "../scripts/dtc-aosp/dtc/dtc-parser.y"
                            { (yyval.integer) = !(yyvsp[0].integer); }
#line 1894 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"
    break;

  case 77: /* bytestring: %empty  */
#line 483 "../scripts/dtc-aosp/dtc/dtc-parser.y"
                {
			(yyval.data) = empty_data;
		}
#line 1902 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"
    break;

  case 78: /* bytestring: bytestring DT_BYTE  */
#line 487 "../scripts/dtc-aosp/dtc/dtc-parser.y"
                {
			(yyval.data) = data_append_byte((yyvsp[-1].data), (yyvsp[0].byte));
		}
#line 1910 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"
    break;

  case 79: /* bytestring: bytestring DT_LABEL  */
#line 491 "../scripts/dtc-aosp/dtc/dtc-parser.y"
                {
			(yyval.data) = data_add_marker((yyvsp[-1].data), LABEL, (yyvsp[0].labelref));
		}
#line 1918 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"
    break;

  case 80: /* subnodes: %empty  */
#line 498 "../scripts/dtc-aosp/dtc/dtc-parser.y"
                {
			(yyval.nodelist) = NULL;
		}
#line 1926 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"
    break;

  case 81: /* subnodes: subnode subnodes  */
#line 502 "../scripts/dtc-aosp/dtc/dtc-parser.y"
                {
			(yyval.nodelist) = chain_node((yyvsp[-1].node), (yyvsp[0].nodelist));
		}
#line 1934 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"
    break;

  case 82: /* subnodes: subnode propdef  */
#line 506 "../scripts/dtc-aosp/dtc/dtc-parser.y"
                {
			ERROR(&(yylsp[0]), "Properties must precede subnodes");
			YYERROR;
		}
#line 1943 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"
    break;

  case 83: /* subnode: DT_PROPNODENAME nodedef  */
#line 514 "../scripts/dtc-aosp/dtc/dtc-parser.y"
                {
			(yyval.node) = name_node((yyvsp[0].node), (yyvsp[-1].propnodename));
		}
#line 1951 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"
    break;

  case 84: /* subnode: DT_DEL_NODE DT_PROPNODENAME ';'  */
#line 518 "../scripts/dtc-aosp/dtc/dtc-parser.y"
                {
			(yyval.node) = name_node(build_node_delete(), (yyvsp[-1].propnodename));
		}
#line 1959 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"
    break;

  case 85: /* subnode: DT_LABEL subnode  */
#line 522 "../scripts/dtc-aosp/dtc/dtc-parser.y"
                {
			add_label(&(yyvsp[0].node)->labels, (yyvsp[-1].labelref));
			(yyval.node) = (yyvsp[0].node);
		}
#line 1968 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"
    break;


#line 1972 "scripts/dtc-aosp/dtc/dtc-parser.tab.c"

      default: break;
    }
  /* User semantic actions sometimes alter yychar, and that requires
     that yytoken be updated with the new translation.  We take the
     approach of translating immediately before every use of yytoken.
     One alternative is translating here after every semantic action,
     but that translation would be missed if the semantic action invokes
     YYABORT, YYACCEPT, or YYERROR immediately after altering yychar or
     if it invokes YYBACKUP.  In the case of YYABORT or YYACCEPT, an
     incorrect destructor might then be invoked immediately.  In the
     case of YYERROR or YYBACKUP, subsequent parser actions might lead
     to an incorrect destructor call or verbose syntax error message
     before the lookahead is translated.  */
  YY_SYMBOL_PRINT ("-> $$ =", YY_CAST (yysymbol_kind_t, yyr1[yyn]), &yyval, &yyloc);

  YYPOPSTACK (yylen);
  yylen = 0;

  *++yyvsp = yyval;
  *++yylsp = yyloc;

  /* Now 'shift' the result of the reduction.  Determine what state
     that goes to, based on the state we popped back to and the rule
     number reduced by.  */
  {
    const int yylhs = yyr1[yyn] - YYNTOKENS;
    const int yyi = yypgoto[yylhs] + *yyssp;
    yystate = (0 <= yyi && yyi <= YYLAST && yycheck[yyi] == *yyssp
               ? yytable[yyi]
               : yydefgoto[yylhs]);
  }

  goto yynewstate;


/*--------------------------------------.
| yyerrlab -- here on detecting error.  |
`--------------------------------------*/
yyerrlab:
  /* Make sure we have latest lookahead translation.  See comments at
     user semantic actions for why this is necessary.  */
  yytoken = yychar == YYEMPTY ? YYSYMBOL_YYEMPTY : YYTRANSLATE (yychar);
  /* If not already recovering from an error, report this error.  */
  if (!yyerrstatus)
    {
      ++yynerrs;
      yyerror (YY_("syntax error"));
    }

  yyerror_range[1] = yylloc;
  if (yyerrstatus == 3)
    {
      /* If just tried and failed to reuse lookahead token after an
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
                      yytoken, &yylval, &yylloc);
          yychar = YYEMPTY;
        }
    }

  /* Else will try to reuse lookahead token after shifting the error
     token.  */
  goto yyerrlab1;


/*---------------------------------------------------.
| yyerrorlab -- error raised explicitly by YYERROR.  |
`---------------------------------------------------*/
yyerrorlab:
  /* Pacify compilers when the user code never invokes YYERROR and the
     label yyerrorlab therefore never appears in user code.  */
  if (0)
    YYERROR;
  ++yynerrs;

  /* Do not reclaim the symbols of the rule whose action triggered
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
  yyerrstatus = 3;      /* Each real token shifted decrements this.  */

  /* Pop stack until we find a state that shifts the error token.  */
  for (;;)
    {
      yyn = yypact[yystate];
      if (!yypact_value_is_default (yyn))
        {
          yyn += YYSYMBOL_YYerror;
          if (0 <= yyn && yyn <= YYLAST && yycheck[yyn] == YYSYMBOL_YYerror)
            {
              yyn = yytable[yyn];
              if (0 < yyn)
                break;
            }
        }

      /* Pop the current state because it cannot handle the error token.  */
      if (yyssp == yyss)
        YYABORT;

      yyerror_range[1] = *yylsp;
      yydestruct ("Error: popping",
                  YY_ACCESSING_SYMBOL (yystate), yyvsp, yylsp);
      YYPOPSTACK (1);
      yystate = *yyssp;
      YY_STACK_PRINT (yyss, yyssp);
    }

  YY_IGNORE_MAYBE_UNINITIALIZED_BEGIN
  *++yyvsp = yylval;
  YY_IGNORE_MAYBE_UNINITIALIZED_END

  yyerror_range[2] = yylloc;
  ++yylsp;
  YYLLOC_DEFAULT (*yylsp, yyerror_range, 2);

  /* Shift the error token.  */
  YY_SYMBOL_PRINT ("Shifting", YY_ACCESSING_SYMBOL (yyn), yyvsp, yylsp);

  yystate = yyn;
  goto yynewstate;


/*-------------------------------------.
| yyacceptlab -- YYACCEPT comes here.  |
`-------------------------------------*/
yyacceptlab:
  yyresult = 0;
  goto yyreturnlab;


/*-----------------------------------.
| yyabortlab -- YYABORT comes here.  |
`-----------------------------------*/
yyabortlab:
  yyresult = 1;
  goto yyreturnlab;


/*-----------------------------------------------------------.
| yyexhaustedlab -- YYNOMEM (memory exhaustion) comes here.  |
`-----------------------------------------------------------*/
yyexhaustedlab:
  yyerror (YY_("memory exhausted"));
  yyresult = 2;
  goto yyreturnlab;


/*----------------------------------------------------------.
| yyreturnlab -- parsing is finished, clean up and return.  |
`----------------------------------------------------------*/
yyreturnlab:
  if (yychar != YYEMPTY)
    {
      /* Make sure we have latest lookahead translation.  See comments at
         user semantic actions for why this is necessary.  */
      yytoken = YYTRANSLATE (yychar);
      yydestruct ("Cleanup: discarding lookahead",
                  yytoken, &yylval, &yylloc);
    }
  /* Do not reclaim the symbols of the rule whose action triggered
     this YYABORT or YYACCEPT.  */
  YYPOPSTACK (yylen);
  YY_STACK_PRINT (yyss, yyssp);
  while (yyssp != yyss)
    {
      yydestruct ("Cleanup: popping",
                  YY_ACCESSING_SYMBOL (+*yyssp), yyvsp, yylsp);
      YYPOPSTACK (1);
    }
#ifndef yyoverflow
  if (yyss != yyssa)
    YYSTACK_FREE (yyss);
#endif

  return yyresult;
}

#line 528 "../scripts/dtc-aosp/dtc/dtc-parser.y"


void yyerror(char const *s)
{
	ERROR(&yylloc, "%s", s);
}

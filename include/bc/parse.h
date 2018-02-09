#ifndef BC_PARSE_H
#define BC_PARSE_H

#include <stdbool.h>
#include <stdint.h>

#include <bc/vector.h>
#include <bc/program.h>
#include <bc/lex.h>

#define BC_PARSE_TOP_FLAG_PTR(parse)  \
  ((uint8_t*) bc_vec_top(&(parse)->flags))

#define BC_PARSE_TOP_FLAG(parse)  \
  (*(BC_PARSE_TOP_FLAG_PTR(parse)))

#define BC_PARSE_FLAG_FUNC_INNER (0x01)

#define BC_PARSE_FUNC_INNER(parse)  \
  (BC_PARSE_TOP_FLAG(parse) & BC_PARSE_FLAG_FUNC_INNER)

#define BC_PARSE_FLAG_FUNC (0x02)

#define BC_PARSE_FUNC(parse)  \
  (BC_PARSE_TOP_FLAG(parse) & BC_PARSE_FLAG_FUNC)

#define BC_PARSE_FLAG_HEADER (0x04)

#define BC_PARSE_HEADER(parse)  \
  (BC_PARSE_TOP_FLAG(parse) & BC_PARSE_FLAG_HEADER)

#define BC_PARSE_FLAG_LOOP (0x08)

#define BC_PARSE_LOOP(parse)  \
  (BC_PARSE_TOP_FLAG(parse) & BC_PARSE_FLAG_LOOP)

#define BC_PARSE_FLAG_LOOP_INNER (0x10)

#define BC_PARSE_LOOP_INNER(parse) \
  (BC_PARSE_TOP_FLAG(parse) & BC_PARSE_FLAG_LOOP_INNER)

#define BC_PARSE_FLAG_IF (0x20)

#define BC_PARSE_IF(parse)  \
  (BC_PARSE_TOP_FLAG(parse) & BC_PARSE_FLAG_IF)

#define BC_PARSE_FLAG_ELSE (0x40)

#define BC_PARSE_ELSE(parse)  \
  (BC_PARSE_TOP_FLAG(parse) & BC_PARSE_FLAG_ELSE)

#define BC_PARSE_FLAG_IF_END (0x80)

#define BC_PARSE_IF_END(parse)  \
  (BC_PARSE_TOP_FLAG(parse) & BC_PARSE_FLAG_IF_END)

#define BC_PARSE_CAN_EXEC(parse)  \
  (!(BC_PARSE_TOP_FLAG(parse) & (BC_PARSE_FLAG_FUNC_INNER |  \
                                 BC_PARSE_FLAG_FUNC |        \
                                 BC_PARSE_FLAG_HEADER |      \
                                 BC_PARSE_FLAG_LOOP |        \
                                 BC_PARSE_FLAG_LOOP_INNER |  \
                                 BC_PARSE_FLAG_IF |          \
                                 BC_PARSE_FLAG_ELSE |        \
                                 BC_PARSE_FLAG_IF_END)))

// We can calculate the conversion between tokens and exprs
// by subtracting the position of the first operator in the
// lex enum and adding the position of the first in the expr
// enum. WARNING: This only works for binary operators.
#define BC_PARSE_TOKEN_TO_EXPR(type) ((type) - BC_LEX_OP_POWER + BC_EXPR_POWER)

typedef struct BcOp {

  uint8_t prec;
  bool left;

} BcOp;

typedef struct BcParse {

  BcLex lex;
  BcLexToken token;

  BcVec flags;

  BcVec exit_labels;

  BcVec cond_labels;

  BcVec ops;

  BcProgram* program;
  size_t func;

  uint32_t num_braces;

  bool auto_part;

} BcParse;

BcStatus bc_parse_init(BcParse* parse, BcProgram* program);
BcStatus bc_parse_file(BcParse* parse, const char* file);
BcStatus bc_parse_text(BcParse* parse, const char* text);

BcStatus bc_parse_parse(BcParse* parse);

BcStatus bc_parse_expr(BcParse* parse, BcVec* code, bool posix_rel, bool print);

void bc_parse_free(BcParse* parse);

#endif // BC_PARSE_H

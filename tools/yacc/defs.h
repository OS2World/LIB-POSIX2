/*	$Id: defs.h,v 1.2 2000/08/14 16:26:17 amai Exp $	*/
/*	$NetBSD: defs.h,v 1.6 1996/03/19 03:21:30 jtc Exp $	*/

/*
 * Copyright (c) 1989 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Robert Paul Corbett.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *	This product includes software developed by the University of
 *	California, Berkeley and its contributors.
 * 4. Neither the name of the University nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE REGENTS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 *
 *	@(#)defs.h	5.6 (Berkeley) 5/24/93
 */

#include <assert.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*  machine-dependent definitions			*/
/*  the following definitions are for the Tahoe		*/
/*  they might have to be changed for other machines	*/

/*  MAXCHAR is the largest unsigned character value	*/
/*  MAXSHORT is the largest value of a C short		*/
/*  MINSHORT is the most negative value of a C short	*/
/*  MAXTABLE is the maximum table size			*/
/*  BITS_PER_WORD is the number of bits in a C unsigned	*/
/*  WORDSIZE computes the number of words needed to	*/
/*	store n bits					*/
/*  BIT returns the value of the n-th bit starting	*/
/*	from r (0-indexed)				*/
/*  SETBIT sets the n-th bit starting from r		*/

#define	MAXCHAR		255
#define	MAXSHORT	32767
#define MINSHORT	-32768
#define MAXTABLE	32500
#define BITS_PER_WORD	32
#define	WORDSIZE(n)	(((n)+(BITS_PER_WORD-1))/BITS_PER_WORD)
#define	BIT(r, n)	((((r)[(n)>>5])>>((n)&31))&1)
#define	SETBIT(r, n)	((r)[(n)>>5]|=((unsigned)1<<((n)&31)))


/*  character names  */

#define	NUL		'\0'    /*  the null character  */
#define	NEWLINE		'\n'    /*  line feed  */
#define	SP		' '     /*  space  */
#define	BS		'\b'    /*  backspace  */
#define	HT		'\t'    /*  horizontal tab  */
#define	VT		'\013'  /*  vertical tab  */
#define	CR		'\r'    /*  carriage return  */
#define	FF		'\f'    /*  form feed  */
#define	QUOTE		'\''    /*  single quote  */
#define	DOUBLE_QUOTE	'\"'    /*  double quote  */
#define	BACKSLASH	'\\'    /*  backslash  */


/* defines for constructing filenames */

#define CODE_SUFFIX	".code.c"
#define	DEFINES_SUFFIX	".tab.h"
#define	OUTPUT_SUFFIX	".tab.c"
#define	VERBOSE_SUFFIX	".output"


/* keyword codes */

#define TOKEN 0
#define LEFT 1
#define RIGHT 2
#define NONASSOC 3
#define MARK 4
#define TEXT 5
#define TYPE 6
#define START 7
#define UNION 8
#define IDENT 9
#define EXPECT 10


/*  symbol classes  */

#define UNKNOWN 0
#define TERM 1
#define NONTERM 2


/*  the undefined value  */

#define UNDEFINED (-1)


/*  action codes  */

#define SHIFT 1
#define REDUCE 2


/*  character macros  */

#define IS_IDENT(c)	(isalnum(c) || (c) == '_' || (c) == '.' || (c) == '$')
#define	IS_OCTAL(c)	((c) >= '0' && (c) <= '7')
#define	NUMERIC_VALUE(c)	((c) - '0')


/*  symbol macros  */

#define ISTOKEN(s)	((s) < start_symbol)
#define ISVAR(s)	((s) >= start_symbol)


/*  storage allocation macros  */

#define CALLOC(k,n)	(calloc((unsigned)(k),(unsigned)(n)))
#define	FREE(x)		(free((char*)(x)))
#define MALLOC(n)	(malloc((unsigned)(n)))
#define	NEW(t)		((t*)allocate(sizeof(t)))
#define	NEW2(n,t)	((t*)allocate((unsigned)((n)*sizeof(t))))
#define REALLOC(p,n)	(realloc((char*)(p),(unsigned)(n)))


/*  the structure of a symbol table entry  */

typedef struct bucket bucket;
struct bucket
{
    struct bucket *link;
    struct bucket *next;
    char *name;
    char *tag;
    short value;
    short index;
    short prec;
    char class;
    char assoc;
};


/*  the structure of the LR(0) state machine  */

typedef struct core core;
struct core
{
    struct core *next;
    struct core *link;
    short number;
    short accessing_symbol;
    short nitems;
    short items[1];
};


/*  the structure used to record shifts  */

typedef struct shifts shifts;
struct shifts
{
    struct shifts *next;
    short number;
    short nshifts;
    short shift[1];
};


/*  the structure used to store reductions  */

typedef struct reductions reductions;
struct reductions
{
    struct reductions *next;
    short number;
    short nreds;
    short rules[1];
};


/*  the structure used to represent parser actions  */

typedef struct action action;
struct action
{
    struct action *next;
    short symbol;
    short number;
    short prec;
    char action_code;
    char assoc;
    char suppressed;
};


/* global variables */

extern char dflag;
extern char lflag;
extern char rflag;
extern char tflag;
extern char vflag;
extern char *symbol_prefix;

extern char *cptr;
extern char *line;
extern int lineno;
extern int outline;

extern char *banner[];
extern char *tables[];
extern char *header[];
extern char *body[];
extern char *trailer[];

extern char *action_file_name;
extern char *code_file_name;
extern char *defines_file_name;
extern char *input_file_name;
extern char *output_file_name;
extern char *text_file_name;
extern char *union_file_name;
extern char *verbose_file_name;

extern FILE *action_file;
extern FILE *code_file;
extern FILE *defines_file;
extern FILE *input_file;
extern FILE *output_file;
extern FILE *text_file;
extern FILE *union_file;
extern FILE *verbose_file;

extern int nitems;
extern int nrules;
extern int nsyms;
extern int ntokens;
extern int nvars;
extern int ntags;

extern char unionized;
extern char line_format[];

extern int   start_symbol;
extern char  **symbol_name;
extern short *symbol_value;
extern short *symbol_prec;
extern char  *symbol_assoc;

extern short *ritem;
extern short *rlhs;
extern short *rrhs;
extern short *rprec;
extern char  *rassoc;

extern short **derives;
extern char *nullable;

extern bucket *first_symbol;
extern bucket *last_symbol;

extern int nstates;
extern core *first_state;
extern shifts *first_shift;
extern reductions *first_reduction;
extern short *accessing_symbol;
extern core **state_table;
extern shifts **shift_table;
extern reductions **reduction_table;
extern unsigned *LA;
extern short *LAruleno;
extern short *lookaheads;
extern short *goto_map;
extern short *from_state;
extern short *to_state;

extern action **parser;
extern int SRtotal;
extern int SRexpect;
extern int RRtotal;
extern short *SRconflicts;
extern short *RRconflicts;
extern short *defred;
extern short *rules_used;
extern short nunused;
extern short final_state;

/* global functions */

extern char *allocate(unsigned);
extern bucket *lookup(char*);
extern bucket *make_bucket(char*);
extern void reflexive_transitive_closure(unsigned *,int);
extern void unterminated_comment(int, char *, char *);
extern void unterminated_string(int, char *, char *);
extern void unterminated_union(int, char *, char *);
extern void unterminated_text(int, char *, char *);
extern void syntax_error(int, char *, char *);
extern void over_unionized(char *);
extern void illegal_character(char *);
extern void illegal_tag(int, char *, char *);
extern void set_first_derives(void);
extern void closure(short *nucleus, int n);
extern void finalize_closure(void);
extern void onintr(int signo);
extern void done(int);
extern void set_state_table(void);
extern void set_accessing_symbol(void);
extern void set_shift_table(void);
extern void set_reduction_table(void);
extern void set_maxrhs(void );
extern void initialize_LA(void );
extern void set_goto_map(void );
extern void initialize_F(void );
extern void build_relations(void );
extern void compute_FOLLOWS(void );
extern void compute_lookaheads(void );
extern void fatal(const char*);
extern void digraph(short **);
extern void open_files(void );
void reader(void );
void lr0(void );
void lalr(void );
void make_parser(void );
void verbose(void );
void output(void );
void no_space(void );
void save_reductions(void );
void save_shifts(void );
void no_space(void );
void find_final_state(void );
void remove_conflicts(void );
void unused_rules(void );
void defreds(void );
void total_conflicts(void );
void free_itemsets(void );
void free_shifts(void );
void free_reductions(void );
void output_prefix(void );
void output_stored_text(void );
void output_defines(void );
void output_rule_data(void );
void output_yydefred(void );
void output_actions(void );
void free_parser(void );
void output_debug(void );
void output_stype(void );
void write_section(char **);
void output_trailing_text(void );
void output_semantic_actions(void );
void find_final_state(void );
void remove_conflicts(void );
void unused_rules(void );
void total_conflicts(void );
void find_final_state(void );
void open_error(const char*);
int default_goto(int);
void save_column(int,int);
void sort_actions(void );
void pack_table(void );
void output_base(void );
void output_table(void );
void output_check(void );
void save_column(int,int);
int matching_vector(int);
int pack_vector(int);
void create_symbol_table(void );
void read_declarations(void );
void read_grammar(void );
void free_symbol_table(void );
void free_tags(void );
void pack_names(void );
void check_symbols(void );
void pack_symbols(void );
void pack_grammar(void );
void free_symbols(void );
void print_grammar(void );
void unexpected_EOF(void );
void used_reserved(const char*);
void tokenized_start(const char*);
void retyped_warning(const char*);
void reprec_warning(const char*);
void restarted_warning(void );
void expand_rules(void );
void no_grammar(void );
void default_action_warning(void );
void dollar_warning(int, int);
void dollar_error(int, const char*, const char*);
void untyped_lhs(void );
void unknown_rhs(int);
void revalued_warning(const char*);
void terminal_start(const char*);
void terminal_lhs(int);
void untyped_rhs(int, const char*);
void prec_redeclared(void );
void undefined_goal(const char*);
void undefined_symbol_warning(const char*);
void print_state(int);
void print_gotos(int);
void print_conflicts(int);
void print_core(int);
void print_nulls(int);
void print_actions(int);
void print_shifts(action *);
void print_reductions(action *, int);
void transitive_closure(unsigned *R,int n);
void unterminated_action(int, const char*, const char*);

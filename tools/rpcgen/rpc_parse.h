/*	$Id: rpc_parse.h,v 1.2 2000/11/17 19:17:34 arnd Exp $	*/
/*
 * Sun RPC is a product of Sun Microsystems, Inc. and is provided for
 * unrestricted use provided that this legend is included on all tape
 * media and as a part of the software program in whole or part.  Users
 * may copy or modify Sun RPC without charge, but are not authorized
 * to license or distribute it to anyone else except as part of a product or
 * program developed by the user or with the express written consent of
 * Sun Microsystems, Inc.
 *
 * SUN RPC IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND INCLUDING THE
 * WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR TRADE PRACTICE.
 *
 * Sun RPC is provided with no support and without any obligation on the
 * part of Sun Microsystems, Inc. to assist in its use, correction,
 * modification or enhancement.
 *
 * SUN MICROSYSTEMS, INC. SHALL HAVE NO LIABILITY WITH RESPECT TO THE
 * INFRINGEMENT OF COPYRIGHTS, TRADE SECRETS OR ANY PATENTS BY SUN RPC
 * OR ANY PART THEREOF.
 *
 * In no event will Sun Microsystems, Inc. be liable for any lost revenue
 * or profits or other special, indirect and consequential damages, even if
 * Sun has been advised of the possibility of such damages.
 *
 * Sun Microsystems, Inc.
 * 2550 Garcia Avenue
 * Mountain View, California  94043
 */

/*      @(#)rpc_parse.h  1.3  90/08/29  (C) 1987 SMI   */

#include <sys/cdefs.h>

/*
 * rpc_parse.h, Definitions for the RPCL parser 
 */

enum defkind__ {
	DEF_CONST,
	DEF_STRUCT,
	DEF_UNION,
	DEF_ENUM,
	DEF_TYPEDEF,
	DEF_PROGRAM
};
typedef enum defkind__ defkind;

typedef const char *const_def;

enum relation__ {
	REL_VECTOR,	/* fixed length array */
	REL_ARRAY,	/* variable length array */
	REL_POINTER,	/* pointer */
	REL_ALIAS,	/* simple */
};
typedef enum relation__ relation;

struct typedef_def__ {
	char *old_prefix;
	char *old_type;
	relation rel;
	char *array_max;
};
typedef struct typedef_def__ typedef_def;

struct enumval_list__ {
	char *name;
	char *assignment;
	struct enumval_list__ *next;
};
typedef struct enumval_list__ enumval_list;

struct enum_def__ {
	enumval_list *vals;
};
typedef struct enum_def__ enum_def;

struct declaration__ {
	char *prefix;
	char *type;
	char *name;
	relation rel;
	char *array_max;
};
typedef struct declaration__ declaration;

struct decl_list__ {
	declaration decl;
	struct decl_list__ *next;
};
typedef struct decl_list__ decl_list;

struct struct_def__ {
	decl_list *decls;
};
typedef struct struct_def__ struct_def;

struct case_list__ {
	char *case_name;
	int contflag;
	declaration case_decl;
	struct case_list__ *next;
};
typedef struct case_list__ case_list;

struct union_def__ {
	declaration enum_decl;
	case_list *cases;
	declaration *default_decl;
};
typedef struct union_def__ union_def;

struct arg_list__ {
	char *argname; /* name of struct for arg*/
	decl_list *decls;
};
	
typedef struct arg_list__ arg_list;

struct proc_list__ {
	char *proc_name;
	char *proc_num;
	arg_list args;
	int arg_num;
	char *res_type;
	char *res_prefix;
	struct proc_list__ *next;
};
typedef struct proc_list__ proc_list;

struct version_list__ {
	char *vers_name;
	char *vers_num;
	proc_list *procs;
	struct version_list__ *next;
};
typedef struct version_list__ version_list;

struct program_def__ {
	char *prog_num;
	version_list *versions;
};
typedef struct program_def__ program_def;

struct definition__ {
	char *def_name;
	defkind def_kind;
	union {
		const_def co;
		struct_def st;
		union_def un;
		enum_def en;
		typedef_def ty;
		program_def pr;
	} def;
};
typedef struct definition__ definition;

definition *get_definition(void);


struct bas_type__
{
  char *name;
  int length;
  struct bas_type__ *next;
};

typedef struct bas_type__ bas_type;

void pdeclaration(char *name,declaration *dec,int tab, char *separator);
void emit_inline(declaration *, int);
void emit_single_in_line(declaration *,int, relation);
void pxdrfuncdecl(char*,int );
void pprocdef(proc_list *,version_list *,char*,int,int);
void reinitialize(void);
void c_initialize(void);
int nullproc(proc_list *);
void add_sample_msg(void);
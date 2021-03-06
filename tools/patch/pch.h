/*	$Id: pch.h,v 1.2 2000/08/21 13:37:23 amai Exp $ */

EXT FILE *pfp INIT(Nullfp);		/* patch file pointer */

void re_patch(void);
void open_patch_file(const char *);
void set_hunkmax(void);
void grow_hunkmax(void);
bool there_is_another_patch(void);
int intuit_diff_type(void);
void next_intuit_at(long,long);
void skip_to(long,long);
bool another_hunk(void);
bool pch_swap(void);
short pch_line_len(LINENUM);
LINENUM pch_first(void);
LINENUM pch_ptrn_lines(void);
LINENUM pch_newfirst(void);
LINENUM pch_repl_lines(void);
LINENUM pch_end(void);
LINENUM pch_context(void);
LINENUM pch_hunk_beg(void);
char pch_char(LINENUM);
char *pfetch(LINENUM);
char *pgets(char *,int,FILE*);
void do_ed_script(void);

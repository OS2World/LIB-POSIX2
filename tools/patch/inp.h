/*	$Id: inp.h,v 1.2 2000/08/21 13:37:23 amai Exp $ */

EXT LINENUM input_lines INIT(0);	/* how long is input file in lines */
EXT LINENUM last_frozen_line INIT(0);	/* how many input lines have been */
					/* irretractibly output */

bool rev_in_string(const char *);
void scan_input(const char*);
bool plan_a(const char*);			/* returns false if insufficient memory */
void plan_b(const char*);
char *ifetch(LINENUM,int);
void re_input(void);

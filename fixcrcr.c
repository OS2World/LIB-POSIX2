/* public domain stuff, hacked in a minute by holger */
/* $Header: /cvsroot/posix2/posix2/fixcrcr.c,v 1.8 2000/08/14 16:17:00 amai Exp $ */

#define INCL_DOSFILEMGR
#include <os2.h>
#include <stdio.h>
#include <string.h>

	int
main(int argc, const char*argv[]) 
{
 FILE *fin,*fout;

	if (argc != 2 || (fin = fopen(argv[1],"rb")) == NULL) {

		fprintf(stderr,"\
%s: Fix dangerous DOS control characters in ASCII file: \n\
 CR (\'\\r\'), <CTRL Z> (0x1A).  - Public domain by holger, for posix/2.\n\
 WARNING:\n\
         Use only for plain ASCII files, will damage binaries! \n\n\
 Usage:\n\
   %s FILENAME (must not be read-only or open)\n",
		argv[0], argv[0]);
		exit(1);
	}
	{
	char tmppath[FILENAME_MAX];

	strcpy(tmppath,argv[1]);
	strcat(tmppath,".tmp");
	if ((fout = fopen(tmppath,"wb")) == NULL) {
		fprintf(stderr,"%s: Error open file %s\n",argv[0],tmppath);
		return(3);
	}{
	 register int c;
	while ( EOF != (c = getc(fin)) ) {
		if (c == '\0') { 		/* In this case there is nothing to 'fix'... */
			fprintf(stderr, 
"%s: Error!\nBinary zero detected in file <%s>.\n\
Exiting to prevent data corruption...\n", 
				argv[0], argv[1]);
			fclose(fin); fclose(fout); 
			DosDelete(tmppath);
			exit(99);
		}
		if (c == '\r' ||			/* CR detected, just ignore and skip over it */
			  c == 0x1a) 	/* sub/<CTRL Z>, ignore and skip DOS end marker */
			continue;
		if (EOF == 			/* error indicator; evaluate later */
				putc(c, fout))	/* Poof, we met neither CR nor CTRL Z: Write! */
			break;
	}}
	if ( ferror(fin) || 	/* this is severe, exit to prevent data loss! */
		   ferror(fout) ) { 
		fprintf(stderr,"\
%s: I/O-error while reading file %s\n. Exiting to prevent data corruption...",
			argv[0],tmppath);
		fclose(fin); fclose(fout); 
		DosDelete(tmppath);
		return(4);
	}
	/* cleanup */
	if ( fclose(fin) || fclose(fout) ) {
		fprintf(stderr,"\
%s: Error closing file %s or file %s\n",
			argv[0], argv[1], tmppath);
		return(5);
	}
	if (DosDelete(argv[1]) != 0) {
		fprintf(stderr,"%s: Error delete file %s\n",argv[0],argv[1]);
		return(7);
	}
	if (DosMove(tmppath,argv[1]) != 0) {
		fprintf(stderr,"%s: Error updating file %s\n",argv[0],argv[1]);
		return(8);
	}}
	return(0);
}

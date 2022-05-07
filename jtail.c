/*
 * Copyright (c) 2007 ... 2021 2022
 *     John McCue <jmccue@jmcunx.com>
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
 * ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
 * OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#ifndef _MSDOS
#include <sys/param.h>
#endif
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

#ifdef OpenBSD
#include <err.h>
#endif
#ifdef __FreeBSD_version
#include <err.h>
#endif
#ifdef __NetBSD_Version__
#include <err.h>
#endif

#include <j_lib2.h>
#include <j_lib2m.h>

#include "jtail.h"

  FILE *DEBUG_fp;

/*
 * show_file_heading() -- Show run stats
 */
void show_file_heading(FILE *fp, char *fname)
{

  fprintf(fp, "%s\n", LIT_C80);

  if (fname == (char *) NULL)
    fprintf(fp, "%s\n", LIT_STDIN);
  else
    {
      if (strcmp(fname, FILE_NAME_STDIN) == 0)
	fprintf(fp, "%s\n", LIT_STDIN);
      else
	fprintf(fp, "%s\n", fname);
    }

  fprintf(fp, "%s\n", LIT_C80);

} /* show_file_heading() */

/*
 * show_reverse() -- show lines in 'reverse' order
 */
void show_reverse(work_area *w, f_line *l)

{
  long i;
  long j;

  j = l->last_line - 1;

  for (i = w->show_lines; i > -1; i--)
    {
      if (l->line_data[i] != (char *) NULL)
	{
	  if ( w->show_ln )
	    {
	      fprintf(w->out.fp, "%ld: %s", j, l->line_data[i]); 
	      j--;
	    }
	  else
	    fprintf(w->out.fp, "%s", l->line_data[i]); 
	}
    }

} /* show_reverse() */

/*
 * show_normal() -- show lines in 'normal' order
 */
void show_normal(work_area *w, f_line *l)

{
  long i;
  long j;

  if (l->last_line < w->show_lines)
    j = 1;
  else
    j = l->last_line - w->show_lines;

  for (i = 0; i < w->show_lines; i++) 
    { 
      if (l->line_data[i] != (char *) NULL)
	{
	  if ( w->show_ln )
	    {
	      fprintf(w->out.fp, "%ld: %s", j, l->line_data[i]); 
	      j++;
	    }
	  else
	    fprintf(w->out.fp, "%s", l->line_data[i]); 
	}
    }


} /* show_normal() */

/*
 * add_new_line() -- Add a new line to table
 */
void add_new_line(long int max, f_line *l, char *buf)

{
  long i, j;

  for (i = 0; i < max; i++)
    {
      j = i + 1;
      free(l->line_data[i]);
      l->line_data[i] = (char *) NULL;
      if (l->line_data[j] != (char *) NULL)
	l->line_data[i] = strdup(l->line_data[j]);
    }

  j = max - 1;
  if (l->line_data[j] != (char *) NULL)
    free(l->line_data[j]);
  l->line_data[j] = strdup(buf);

} /* add_new_line() */

/*
 * open_in() -- open in file
 */
int open_in(FILE **in, char *fname, FILE *fp_err)

{

  int errsave;

  if (fname == (char *) NULL)
    {
      (*in) = stdin;
      return((int) TRUE);
    }
  if (strcmp(fname, FILE_NAME_STDIN) == 0)
    {
      (*in) = stdin;
      return((int) TRUE);
    }

  (*in) = fopen(fname, "r");
  errsave = errno;

  if ((*in) == (FILE *) NULL)
    {
      fprintf(fp_err, MSG_WARN_W002, fname, strerror(errsave));
      return((int) FALSE);
    }

  return((int) TRUE);

} /* open_in() */

/*
 * process_a_file()
 */
void process_a_file(work_area *w, char *fname,
                    char **buf,   size_t *bsiz)
{

  int move_up = (int) FALSE;
  long i;
  FILE *fp;
  f_line l;

  l.line_data = (char **) calloc((size_t) (w->show_lines + 1), (size_t) sizeof(char *));
  l.last_line = (long int) 0;

  if (w->verbose == (int) TRUE)
    show_file_heading(w->out.fp, fname);

  if ( ! open_in(&(fp), fname, (w->err.fp)) )
    return;

  /*** read and save the last n lines ***/
  i = 0L;
  while (j2_getline(buf, bsiz, fp) > (ssize_t) -1)
    {
      (l.last_line)++;  /* Line # of last line saved */
      if ( move_up )
	add_new_line(w->show_lines, &l, (*buf));
      else
	{
	  l.line_data[i] = strdup((*buf)); 
	  i++; 
	  if (i > (w->show_lines - 1)) 
	    move_up = (int) TRUE; 
	} 
    } 

  if (fname != (char *) NULL)
    {
      if (strcmp(fname, FILE_NAME_STDIN) != 0)
	{
	  if (fp != (FILE *) NULL)
	    fclose(fp);
	}
    }

  /*** show data ***/
  if (w->reverse)
    show_reverse(w, &l);
  else
    show_normal(w, &l);

  /*** release memory ***/ 
  if (l.line_data != (char **) NULL) 
    { 
      for (i = 0; l.line_data[i]  != (char *) NULL; i++)
	{
	  if (l.line_data[i]  != (char *) NULL) 
	    {
	      free(l.line_data[i]); 
	      l.line_data[i] = (char *) NULL;
	    }
	}
      free(l.line_data);
    } 

} /* process_a_file() */

/*
 * process_all()
 */
void process_all(int argc, char **argv, work_area *w)

{
  char *buf = (char *) NULL;
  size_t bsiz = (size_t) 0;
  int i;

  for (i = optind; i < argc; i++)
    process_a_file(w, argv[i], &buf, &bsiz);

  if (i == optind)
    process_a_file(w, FILE_NAME_STDIN, &buf, &bsiz);

  if (buf != (char *) NULL)
    free(buf);

} /* process_all() */

/*
 * main()
 */
int main(int argc, char **argv)
{
  work_area w;

#ifdef OpenBSD
  if(pledge("stdio rpath wpath cpath",NULL) == -1)
    err(1,"pledge\n");
#endif

  init(&w, argc, argv);

  DEBUG_fp = w.err.fp;

  process_all(argc, argv, &w);

  close_out(&(w.out));
  close_out(&(w.err));

  if (w.prog_name != (char *) NULL)
    free(w.prog_name);

  exit(w.err_code);

}  /* main() */

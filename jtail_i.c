/*
 * Copyright (c) 2007 ... 2023 2024
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
#include <ctype.h>
#include <errno.h>

#ifdef HAVE_JLIB
#include <j_lib2.h>
#include <j_lib2m.h>
#endif

#include "jtail.h"

/*
 * init_finfo() -- initialize out file structure
 */
void init_finfo(struct s_file_info *f)

{

  f->fp         = (FILE *) NULL;
  f->fname      = (char *) NULL;

} /* init_finfo() */

/*
 * clear_work()
 */
void clear_work(work_area *w, char *a)

{

  char *e = (char *) NULL;

  w->prog_name   = j2_get_prgname(a, PROG_NAME);
  w->arg_switch  = SWITCH_CHAR;

  w->err_code    = (int) EXIT_SUCCESS;
  w->num_files   = (int) 0;
  w->force       = (int) FALSE;
  w->verbose     = (int) TRUE;
  w->show_ln     = (int) FALSE;
  w->reverse     = (int) FALSE;
  w->show_lines  = TAIL_LINES_DEFAULT;

  init_finfo(&(w->out));
  init_finfo(&(w->err));
  w->err.fp = stderr;
  w->out.fp = stdout;

  /*** default args based upon environment ***/
  e = getenv(ARG_JTAIL_N);
  if (e != (char *) NULL)
    {
      if (j2_is_numr(e))
	w->show_lines  = atol(e);
    }
  if (w->show_lines < 1)
    w->show_lines  = TAIL_LINES_DEFAULT;

} /* clear_work() */

/*
 * init() -- setup for Run
 */
void init(work_area *w, int argc, char **argv)

{

  clear_work(w, argv[0]);

  /* process options */
  process_arg(argc, argv, w);

  if (w->show_lines < 1L) /* if < 1, changed to default */
    w->show_lines = TAIL_LINES_DEFAULT;

} /* init() */

/*
 * open_out() -- save the file anem and check status
 */
int open_out(FILE *wfp, struct s_file_info *f, char *fname, int force)

{

  int errsave;

  if (fname == (char *) NULL)
    return((int) TRUE);

  if (force == (int) FALSE)
    {
      if ( j2_f_exist(fname) )
	{
	  fprintf(wfp, MSG_ERR_E025, fname);
	  return((int) FALSE);
	}
    }


  f->fp = fopen(fname, "w");
  errsave = errno;

  if (f->fp == (FILE *) NULL)
    {
      f->fp = stderr;  /* needs to be something */
      fprintf(wfp, MSG_ERR_E002, fname);
      fprintf(wfp, "\t%s\n", strerror(errsave));
      return((int) FALSE);
    }

  /*** success, save file name ***/
  f->fname = strdup(fname);
  return((int) TRUE);

} /* open_out() */

/*
 * close_out() -- close output
 */
void close_out(struct s_file_info *f)
{

  if (f->fname != (char *) NULL)
    {
      fclose(f->fp);
      free(f->fname);
      f->fname = (char *) NULL;
    }

} /* close_out() */

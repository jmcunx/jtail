/*
 * Copyright (c) 2007 ... 2020 2021
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

#include <sys/param.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>
#include <j_lib2.h>
#include <j_lib2m.h>

#include "jtail.h"

#define MAXARG 80

char *jtail_a_rev = "$Id: jtail_a.c,v 3.4 2021/02/21 20:57:38 jmccue Exp $";

/*
 * process_arg() -- process arguments
 */
void process_arg(int argc, char **argv, work_area *w)

{

  char ckarg[MAXARG];
  int ok, i, opt;
  int found_verbose = (int) FALSE;
  int found_quiet = (int) FALSE;
  char *fout = (char *) NULL; 
  char *ferr = (char *) NULL; 
  int display_help = (int) FALSE;
  int display_rev  = (int) FALSE;

  snprintf(ckarg, MAXARG, "%c%c%c%c%c%c%c%c:%c:%c:",
	   ARG_COUNT, ARG_FORCE, ARG_HELP, ARG_QUIET, ARG_VERBOSE, 
           ARG_REVERSE, ARG_VERSION, ARG_NUMB, ARG_ERR, ARG_OUT);

  while ((opt = getopt(argc, argv, ckarg)) != -1)
    {
      switch (opt)
	{
	  case ARG_COUNT:
	    w->show_ln = (int) TRUE;
	    break;
	  case ARG_ERR:
	    ferr = optarg;
	    break;
	  case ARG_FORCE:
	    w->force = (int) TRUE;
	    break;
	  case ARG_HELP:
	    display_help = (int) TRUE;
	    break;
	  case ARG_NUMB:
	    w->show_lines = atol(optarg);
	    if (w->show_lines < 1)
	      {
		fprintf(stderr, MSG_ERR_E006, optarg, SWITCH_CHAR, ARG_NUMB);
		fprintf(stderr, MSG_ERR_E000, w->prog_name, SWITCH_CHAR, ARG_HELP);
		exit(EXIT_FAILURE);
	      }
	    break;
	  case ARG_OUT:
	    fout = optarg;
	    break;
	  case ARG_QUIET:
	    found_quiet = (int) TRUE;
	    w->verbose  = (int) FALSE;
	    break;
	  case ARG_VERBOSE:
	    found_verbose = (int) TRUE;
	    w->verbose    = (int) TRUE;
	    break;
	  case ARG_REVERSE:
	    w->reverse  = (int) TRUE;
	    break;
	  case ARG_VERSION:
	    display_rev = (int) TRUE;
	    break;
	  default:
	    fprintf(stderr, MSG_ERR_E000, w->prog_name, SWITCH_CHAR, ARG_HELP);
	    exit(EXIT_FAILURE);
	    break;
	}
    }

  /*** if necessary - save stdout/err files and open ***/
  ok = open_out(stderr, &(w->err), ferr, w->force);
  if ( ok )
    ok = open_out(w->err.fp, &(w->out), fout, w->force);
  if ( ! ok )
    {
      fprintf(w->err.fp, MSG_ERR_E000, w->prog_name, SWITCH_CHAR, ARG_HELP);
      exit(EXIT_FAILURE);
    }

  /*** see if rev/help display needed ***/
  ok = EXIT_SUCCESS;
  if (display_help)
    ok = show_brief_help(w);
  if (display_rev)
    ok = show_rev(w);
  if (ok != EXIT_SUCCESS)
    {
      close_out(&(w->out));
      close_out(&(w->err));
      exit(ok);
    }

  /*** Count number of files to process */
  for (i = optind; i < argc; i++)
    (w->num_files)++;
  if (w->num_files == 0)
    (w->num_files)++;  /* stdin when no files */

  /*** default verbose value ***/
  if ((found_quiet == (int) FALSE) && (found_verbose == (int) FALSE) )
    {
      if (w->num_files > 1)
	w->verbose = (int) TRUE;
      else
	w->verbose = (int) FALSE;
    }

} /* process_arg() */

/* END: jtail_a.c */

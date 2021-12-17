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

#ifndef JTAIL_H_REV

#define JTAIL_H_REV "INCLUDED"

#define PROG_NAME      "jtail"
#define JTAIL_MSG_SIZE 256
#define ARG_JTAIL_N    "ARG_JTAIL_N"

#define TAIL_LINES_DEFAULT 10L

/*** Structures ***/
typedef struct f_line_info
  {
    long int last_line;
    char **line_data;
  } f_line;

struct s_file_info
  {
    FILE *fp;
    char *fname;
  } ;

typedef struct
{
  char *prog_name;
  char arg_switch;
  int err_code;
  int  num_files;
  int  force;                  /* TRUE or FALSE, defaults to FALSE */
  int  verbose;                /* TRUE or FALSE, defaults to TRUE  */
  int  show_ln;                /* TRUE or FALSE, defaults to FALSE */
  int  reverse;                /* TRUE or FALSE, defaults to FALSE */
  long int  show_lines;        /* Lines to display                 */
  struct s_file_info out;
  struct s_file_info err;
}  work_area;

/*** prototypes ***/
int  show_brief_help(work_area *);
int  show_rev(work_area *);
void process_arg(int, char **, work_area *);
void get_prog_name(char **, char *);
void init(work_area *, int, char **);
int  open_out(FILE *, struct s_file_info *, char *, int);
void close_out(struct s_file_info *);

#endif /* JTAIL_H_REV */

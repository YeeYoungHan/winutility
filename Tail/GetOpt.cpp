/* 
 * Copyright (C) 2012 Yee Young Han <websearch@naver.com> (http://blog.naver.com/websearch)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
 */

#ifdef WIN32

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdlib.h>

#define OPTERRCOLON (1)
#define OPTERRNF (2)
#define OPTERRARG (3)

char *optarg;
int optreset = 0;
int optind = 1;
int opterr = 1;
int optopt;

static int optiserr(int argc, char * const *argv, int oint, const char *optstr, int optchr, int err)
{
  if(opterr)
  {
    fprintf(stderr, "Error in argument %d, char %d: ", oint, optchr+1);
    switch(err)
    {
    case OPTERRCOLON:
      fprintf(stderr, ": in flags\n");
      break;
    case OPTERRNF:
      fprintf(stderr, "option not found %c\n", argv[oint][optchr]);
      break;
    case OPTERRARG:
      fprintf(stderr, "no argument for option %c\n", argv[oint][optchr]);
      break;
    default:
      fprintf(stderr, "unknown\n");
      break;
    }
  }
  optopt = argv[oint][optchr];
  return('?');
}

int getopt( int argc, char* const *argv, const char *optstr )
{
  static int optchr = 0;
  static int dash = 0; /* have already seen the - */

  const char *cp;

  if (optreset)
    optreset = optchr = dash = 0;
  if(optind >= argc)
    return(EOF);
  if(!dash && (argv[optind][0] !=  '-'))
    return(EOF);
  if(!dash && (argv[optind][0] ==  '-') && !argv[optind][1])
  {
    /*
     * use to specify stdin. Need to let pgm process this and
     * the following args
     */
    return(EOF);
  }
  if( (argv[optind][0] == '-') && (argv[optind][1] == '-') )
  {
    /* -- indicates end of args */
    optind++;
    return(EOF);
  }

  if( !dash )
  {
    assert((argv[optind][0] == '-') && argv[optind][1]);
    dash = 1;
    optchr = 1;
  }

  // Check if the guy tries to do a -: kind of flag
  assert(dash);
  if( argv[optind][optchr] == ':' )
  {
    dash = 0;
    optind++;
    return( optiserr(argc, argv, optind-1, optstr, optchr, OPTERRCOLON) );
  }

  if( !(cp = strchr( optstr, argv[optind][optchr] ) ) )
  {
    int errind = optind;
    int errchr = optchr;

    if( !argv[optind][optchr+1] )
    {
      dash = 0;
      optind++;
    }
    else
      optchr++;
    return( optiserr( argc, argv, errind, optstr, errchr, OPTERRNF ) );
  }

  if( cp[1] == ':' )
  {
    dash = 0;
    optind++;
    if( optind == argc )
      return(optiserr(argc, argv, optind-1, optstr, optchr, OPTERRARG));
    optarg = argv[optind++];
    return(*cp);
  }
  else
  {
    if( !argv[optind][optchr+1] )
    {
      dash = 0;
      optind++;
    }
    else
      optchr++;
    return(*cp);
  }

  assert(0);
  return(0);
}

#endif

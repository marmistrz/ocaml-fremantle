/***********************************************************************/
/*                                                                     */
/*                 MLTk, Tcl/Tk interface of OCaml                     */
/*                                                                     */
/*    Francois Rouaix, Francois Pessaux, Jun Furuse and Pierre Weis    */
/*               projet Cristal, INRIA Rocquencourt                    */
/*            Jacques Garrigue, Kyoto University RIMS                  */
/*                                                                     */
/*  Copyright 2002 Institut National de Recherche en Informatique et   */
/*  en Automatique and Kyoto University.  All rights reserved.         */
/*  This file is distributed under the terms of the GNU Library        */
/*  General Public License, with the special exception on linking      */
/*  described in file LICENSE found in the OCaml source tree.          */
/*                                                                     */
/***********************************************************************/

/* $Id$ */

#include <stdlib.h>
#include <string.h>

#include <tcl.h>
#include <tk.h>
#include <mlvalues.h>
#include <alloc.h>
#include <memory.h>
#ifdef HAS_UNISTD
#include <unistd.h>
#endif
#include "camltk.h"

#if (TCL_MAJOR_VERSION > 8 || \
    (TCL_MAJOR_VERSION == 8 && TCL_MINOR_VERSION >= 1)) /* 8.1 */
# define UTFCONVERSION
#endif

#ifdef UTFCONVERSION

char *external_to_utf( CONST84 char *str ){
  char *res;
  Tcl_DString dstr;
  int length;

  Tcl_ExternalToUtfDString(NULL, str, strlen(str), &dstr);
  length = Tcl_DStringLength(&dstr);
  res = caml_stat_alloc(length + 1);
  memmove( res, Tcl_DStringValue(&dstr), length+1);
  Tcl_DStringFree(&dstr);

  return res;
}

char *utf_to_external( CONST84 char *str ){
  char *res;
  Tcl_DString dstr;
  int length;

  Tcl_UtfToExternalDString(NULL, str, strlen(str), &dstr);
  length = Tcl_DStringLength(&dstr);
  res = caml_stat_alloc(length + 1);
  memmove( res, Tcl_DStringValue(&dstr), length+1);
  Tcl_DStringFree(&dstr);

  return res;
}

char *caml_string_to_tcl( value s )
{
  return external_to_utf( String_val(s) );
}

value tcl_string_to_caml( CONST84 char *s )
{
  CAMLparam0();
  CAMLlocal1(res);
  char *str;

  str = utf_to_external( s );
  res = copy_string(str);
  stat_free(str);
  CAMLreturn(res);
}

#else

char *caml_string_to_tcl(value s){ return string_to_c(s); }
value tcl_string_to_caml(CONST84 char *s){ return copy_string(s); }

#endif

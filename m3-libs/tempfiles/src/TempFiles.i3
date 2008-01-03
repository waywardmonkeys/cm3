(* Copyright (C) 1994, Digital Equipment Corporation          *)
(* All rights reserved.                                       *)
(* See the file COPYRIGHT for a full description.             *)
(*                                                            *)
(* Last modified on Tue Nov  1 10:04:41 PST 1994 by kalsow    *)
(*      modified on Wed Sep  7 14:47:21 PDT 1994 by birrell   *)

(* Lectern: a user interface for viewing documents stored as images *)
(* Temp-file management *)

INTERFACE TempFiles;

IMPORT Pathname;

(* *)
(* This module keeps track of a list of temp files, and uses the Process
   cleanup mechanism to ensure that they get deleted on program exit,
   including termination by control-C.  It also will choose a temp file
   name for you, if you want. *)
(* *)

(* *)
(* temp file names *)
(* *)

PROCEDURE DefaultPrefix(prefix: Pathname.T);
  (* Use the given prefix as default for future temp file names generated by
     calls of Get.  Before the first call of DefaultPrefix,
     a call of Get with defaulted prefix would use a default prefix
     (/tmp on U*ix systems).  The notion of "prefix" is as specified in the
     Pathname interface.  *)

PROCEDURE Get(prefix: TEXT := NIL; part: TEXT := ","; ext: TEXT := NIL): TEXT;
  (* Returns an almost-certainly-unique file name, using prefix provided,
     (or by default the prefix as specified by the Init procedure), and
     a suffix composed of "part" concatenated
     with a manufactured string, and the given extension.  The notions
     of "prefix" and "extension" are as specified for the Pathname.Join
     procedure.  Does not call Note - that's up to you to do.
     For example, on U*x with the default prefix, Get(",x-", "y")
     returns a string of the form "/tmp/,x-*.y" *)


(* *)
(* Temp file list *)
(* *)

PROCEDURE Note(t: TEXT);
  (* Adds "t" to the list of files to be deleted on program exit.  There
     is no attempt to detect duplicates.  It's probably best to call this
     after creating the file, to avoid spurious warnings about failure to
     delete it (but at the risk of not deleting it, of course). *)

PROCEDURE Forget(t: TEXT);
  (* Removes "t" from the list of files to be delete on program exit.  Fails
     silently if "t" isn't on the list.  It's probably best to call this
     before deleting or renaming the file, to avoid spurious warnings about
     failure to delete it (but at the risk of not deleting it, of course). *)

END TempFiles.

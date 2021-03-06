(* Copyright (c) 2000 California Institute of Technology *)
(* All rights reserved. See the file COPYRIGHT for a full description. *)
(* $Id: ExtHeader.i3,v 1.2 2001-09-19 15:14:22 wagner Exp $ *)

INTERFACE ExtHeader;
IMPORT TextList;
IMPORT Rd;
TYPE
  T = RECORD
    sources, imports: TextList.T;
  END;
PROCEDURE Parse(from: Rd.T): T;
END ExtHeader.


  

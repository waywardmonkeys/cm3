(*                            -*- Mode: Modula-3 -*- 
 * 
 * For information about this program, contact Blair MacIntyre            
 * (bm@cs.columbia.edu) or Steven Feiner (feiner@cs.columbia.edu)         
 * at the Computer Science Dept., Columbia University,                    
 * 1214 Amsterdam Ave. Mailstop 0401, New York, NY, 10027.                
 *                                                                        
 * Copyright (C) 1995, 1996 by The Trustees of Columbia University in the 
 * City of New York.  Blair MacIntyre, Computer Science Department.       
 * 
 * Author          : Blair MacIntyre
 * Created On      : Fri Oct  6 12:27:53 1995
 * Last Modified By: Blair MacIntyre
 * Last Modified On: Wed Mar 12 12:15:19 1997
 * Update Count    : 5
 * 
 * $Source$
 * $Date$
 * $Author$
 * $Revision$
 * 
 * $Log$
 * Revision 1.2  1997/03/12 21:34:56  bm
 * Moved sharedobj from coterie
 *
 * 
 * HISTORY
 *)

MODULE LibEmbDirsWin32 EXPORTS LibEmbDirs;

IMPORT TextSeq;

BEGIN
  dirs := NEW(TextSeq.T).init();
  dirs.addhi("g:\\arm3\\obliqlibemb\\src");
END LibEmbDirsWin32.

(* Copyright (C) 1994, Digital Equipment Corporation. *)
(* All rights reserved.                               *)
(* See the file COPYRIGHT for a full description.     *)
 
(* Last modified on Wed Dec 16 08:50:01 PST 1992 by kalsow *)
(*      modified on Mon Aug  6 20:17:15 PDT 1990 by ellis  *)
(*      modified on Wed May  2 18:06:31 1990 by saxe       *)

UNSAFE MODULE Main;
IMPORT Test;

(* BITS FOR data types *)

TYPE
  SixBit = BITS 6 FOR [0 .. 63];
  TwelveBit = BITS 16 FOR ARRAY [0 .. 1] OF SixBit; (* Modula-2+: 12 *)
  TwoSixBits = ARRAY [0 .. 1] OF SixBit;
  PairSixBits = RECORD x: SixBit; y: SixBit; END;
  TwoPairSixBits = ARRAY [0..1] OF RECORD x: SixBit; y: SixBit; END;
  PairPairSixBits = RECORD x: PairSixBits; y: PairSixBits; END;
  ThirtyOneBit = (* BITS 64 FOR *) RECORD (* Modula-2+: 31 *)
    t: BITS 32 FOR RECORD u: TwelveBit; v: TwelveBit; END; (* Modula-2+: 25 *)
    s: SixBit;
  END;
  Woid = RECORD a: BITS 1 FOR BOOLEAN; c: ThirtyOneBit; END;
  Number = BITS 24 FOR [0 .. 16777215];

VAR
  p, q: SixBit;
  w, x, y: Woid;
  (*
  pp, qq: PairPairSixBits;
  *)
  ii: Number := 0;

PROCEDURE Plus(x: SixBit; y: SixBit): SixBit =
  BEGIN RETURN x + y; END Plus;

PROCEDURE TwelveSum(thirtyOne: ThirtyOneBit): SixBit =
  BEGIN
    RETURN
      (thirtyOne.t.u[0] + thirtyOne.t.u[1] + thirtyOne.t.v[0] +
         thirtyOne.t.v[1]) MOD 64;
  END TwelveSum;

BEGIN
  Test.checkI (BITSIZE(SixBit), 6);
  Test.checkI (BITSIZE(TwelveBit), 16);
  Test.checkI (BITSIZE(TwoSixBits), 16);  (* !! *)
  Test.checkI (BITSIZE(PairSixBits), 16);  (* !! *)
  Test.checkI (BITSIZE(TwoPairSixBits), 32);  (* !! *)
  Test.checkI (BITSIZE(PairPairSixBits), 32);  (* !! *)
  Test.checkI (BITSIZE(ThirtyOneBit), 48);
  Test.checkI (BITSIZE(Woid), 64);
  Test.checkI (BYTESIZE(SixBit), 1);
  Test.checkI (BYTESIZE(TwelveBit), 2);
  Test.checkI (BYTESIZE(TwoSixBits), 2);
  Test.checkI (BYTESIZE(PairSixBits), 2);
  Test.checkI (BYTESIZE(TwoPairSixBits), 4);
  Test.checkI (BYTESIZE(PairPairSixBits), 4);
  Test.checkI (BYTESIZE(ThirtyOneBit), 6);
  Test.checkI (BYTESIZE(Woid), 8);
  Test.checkI (BITSIZE(p), 6);
  Test.checkI (BITSIZE(w.a), 1);
  Test.checkI (BYTESIZE(w.c.t), 4);
  Test.checkI (BITSIZE(w.c.t), 32);
  Test.checkI (BITSIZE(w.c.t.u), 16);
  Test.checkI (BITSIZE(w.c.t.u[0]), 6);
  Test.checkI (BYTESIZE(w.c.t.u[1]), 1);
  Test.checkI (BITSIZE(ii), 24);

  p := 40;
  q := 27;
  Test.checkI (p + q, 67);
  Test.checkI (Plus (20, 27), 47);
  w.a := TRUE;
  FOR i := 0 TO 1 DO w.c.t.u[i] := q + i; END;
  w.c.t.v := w.c.t.u;
  x.c.t := w.c.t;
  y.c := x.c;
  y.a := w.a;
  <*ASSERT y.a *>
  y.c.s := TwelveSum(y.c);
  Test.checkI (y.c.s, 54);
  p := y.c.s;
  Test.checkI (p, 54);

  (* not possible in CM3
  pp.x.x := 20;
  pp.x.y := 30;
  pp.y.x := 40;
  pp.y.y := 50;
  ii := LOOPHOLE(pp, Number);
  qq := LOOPHOLE(ii, PairPairSixBits);
  Test.checkI (qq.x.x, 20);
  Test.checkI (qq.x.y, 30);
  Test.checkI (qq.y.x, 40);
  Test.checkI (qq.y.y, 50);
  Test.checkI (ii, 20 + 64 * (30 + 64 * (40 + 64 * 50)));
  *)

  Test.done ();
END Main.

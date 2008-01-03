(* Copyright (C) 1992, Digital Equipment Corporation           *)
(* All rights reserved.                                        *)
(* See the file COPYRIGHT for a full description.              *)

(* File: WordLE.m3                                             *)
(* Last Modified On Mon Dec  5 15:30:50 PST 1994 By kalsow     *)
(*      Modified On Tue Apr 10 11:13:09 1990 By muller         *)

MODULE WordLE;

IMPORT CG, CallExpr, Expr, ExprRep, Procedure, Target, TWord;
IMPORT Bool, Value, Formal, ProcType;
FROM Int IMPORT T;
IMPORT WordPlus AS Plus;

VAR Z: CallExpr.MethodList;
VAR formals: Value.T;

PROCEDURE Check (ce: CallExpr.T;  VAR cs: Expr.CheckState) =
  BEGIN
    EVAL Formal.CheckArgs (cs, ce.args, formals, ce.proc);
    ce.type := Bool.T;
  END Check;

PROCEDURE Compile (ce: CallExpr.T) =
  BEGIN
    Expr.Compile (ce.args[0]);
    Expr.Compile (ce.args[1]);
    CG.Compare (Target.Word.cg_type, CG.Cmp.LE);
  END Compile;

PROCEDURE PrepBR (ce: CallExpr.T;  true, false: CG.Label;  freq: CG.Frequency)=
  BEGIN
    Expr.Prep (ce.args[0]);
    Expr.Prep (ce.args[1]);
    Expr.Compile (ce.args[0]);
    Expr.Compile (ce.args[1]);
    CG.If_then (Target.Word.cg_type, CG.Cmp.LE, true, false, freq);
  END PrepBR;

PROCEDURE Fold (ce: CallExpr.T): Expr.T =
  VAR w0, w1: Target.Int;
  BEGIN
    IF Plus.GetArgs (ce.args, w0, w1)
      THEN RETURN Bool.Map [TWord.LE (w0, w1)];
      ELSE RETURN NIL;
    END;
  END Fold;

PROCEDURE Initialize () =
  VAR
    x1 := Formal.NewBuiltin ("x", 0, T);
    y1 := Formal.NewBuiltin ("y", 1, T);
    t1 := ProcType.New (Bool.T, x1, y1);
  BEGIN
    Z := CallExpr.NewMethodList (2, 2, TRUE, TRUE, TRUE, Bool.T,
                                 NIL,
                                 CallExpr.NotAddressable,
                                 Check,
                                 CallExpr.PrepArgs,
                                 Compile,
                                 CallExpr.NoLValue,
                                 CallExpr.NoLValue,
                                 PrepBR,
                                 CallExpr.NoBranch,
                                 Fold,
                                 CallExpr.NoBounds,
                                 CallExpr.IsNever, (* writable *)
                                 CallExpr.IsNever, (* designator *)
                                 CallExpr.NotWritable (* noteWriter *));
    Procedure.Define ("LE", Z, FALSE, t1);
    formals := ProcType.Formals (t1);
  END Initialize;

BEGIN
END WordLE.

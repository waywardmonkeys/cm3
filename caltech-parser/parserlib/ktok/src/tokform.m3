MODULE %tok;
%gen
IMPORT Rd, Thread;
IMPORT Wr;
IMPORT Fmt;
IMPORT RTAllocator;
FROM Stdio IMPORT stdout;
<* FATAL Wr.Failure, Thread.Alerted *>

REVEAL
  ParseType = ParseTypePublic BRANDED "%tok.ParseType" OBJECT
    x: REFANY := NIL;
    (* if allocated, a is an allocator.
       if freed into an allocator, a is "tail".
       else, a is NIL *)
  OVERRIDES
    discard := Discard;
    detach := Detach;
  END;
  Allocator = BRANDED "%tok.PrivAlloc" OBJECT
    m3type: INTEGER;
    free: ParseType := NIL;
    numAlloc: INTEGER := 0;
    valid: BOOLEAN := TRUE;
  END;

PROCEDURE NewPT(VAR a: Allocator; m3type: INTEGER): ParseType =
  VAR
    result: ParseType;
  BEGIN
    IF a = NIL THEN
      a := NEW(Allocator, m3type := m3type);
    END;
    <* ASSERT a.m3type = m3type *>
    IF a.free = NIL THEN
      result := RTAllocator.NewTraced(m3type);
    ELSE
      result := a.free;
      a.free := NARROW(a.free.x, ParseType); (* free := free.tail *)
    END;
    INC(a.numAlloc);
    result.x := a;
    RETURN result;
  END NewPT;

PROCEDURE Discard(self: ParseType) =
  VAR
    a: Allocator;
  BEGIN
    IF self.x # NIL THEN
      a := self.x;  (* this fails if self not allocated using New *)
      IF a.valid THEN
        self.x := a.free; (* self.tail = a.free *)
        a.free := self;
        DEC(a.numAlloc);
      END;
    END;
  END Discard;

PROCEDURE Detach(self: ParseType): ParseType = BEGIN
  self.x := NIL; RETURN self; END Detach;

PROCEDURE Purge(VAR a: Allocator): INTEGER =
  VAR
    result: INTEGER;
  BEGIN
    IF a = NIL THEN RETURN 0;END;
    a.valid := FALSE;
    result := a.numAlloc;
    a := NIL;
    RETURN result;
  END Purge;

VAR
  ConstTokens: ARRAY ConstTokenCode OF ConstToken;
PROCEDURE NewConstToken(val: ConstTokenCode): ConstToken =
  BEGIN
    <* ASSERT val IN LegalConstTokenCodes *>
    RETURN ConstTokens[val];
  END NewConstToken; 

PROCEDURE Test(lex: Lexer) =
  VAR
    typeName: TEXT;
  BEGIN
    TRY
      LOOP
        TYPECASE lex.get() OF
        | ConstToken(t) => typeName := "<CONST " & Fmt.Int(t.val) & ">";
        | NULL => typeName := "<NULL>";
%case\
        ELSE
          typeName := "<UNKNOWN>";
        END;
        TYPECASE lex OF RdLexer(l) => 
          Wr.PutText(stdout, typeName & ": \"" & l.getText() & "\"\n");
        ELSE
          Wr.PutText(stdout, typeName & "\n");
        END;
      END;
    EXCEPT
      Rd.EndOfFile =>
    END;
  END Test;

BEGIN
  FOR i := FIRST(ConstTokens) TO LAST(ConstTokens) DO
    IF i IN LegalConstTokenCodes THEN
      ConstTokens[i] := NEW(ConstToken, val := i);
    END;      
  END;
END %tok.

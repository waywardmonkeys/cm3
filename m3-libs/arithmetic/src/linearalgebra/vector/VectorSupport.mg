GENERIC MODULE VectorSupport(R);
(*Copyright (c) 1996, m3na project *)
FROM NADefinitions IMPORT Error, Err;

<*UNUSED*>
CONST Module = "VectorSupport.";


<*INLINE*>
PROCEDURE AssertEqualSize (READONLY x, y: T) RAISES {Error} =
  BEGIN
    IF NUMBER(x) # NUMBER(y) THEN RAISE Error(Err.bad_size); END;
  END AssertEqualSize;

PROCEDURE Clear (VAR z: T) =
  BEGIN
    FOR i := 0 TO LAST(z) DO z[i] := R.Zero; END;
  END Clear;

PROCEDURE Add (VAR z: T; READONLY x, y: T) RAISES {Error} =
  BEGIN
    AssertEqualSize(z, x);
    AssertEqualSize(z, y);
    FOR i := FIRST(z) TO LAST(z) DO z[i] := R.Add(x[i], y[i]); END;
  END Add;

(*-----------------*)
PROCEDURE Sub (VAR z: T; READONLY x, y: T) RAISES {Error} =
  BEGIN
    AssertEqualSize(z, x);
    AssertEqualSize(z, y);
    FOR i := FIRST(z) TO LAST(z) DO z[i] := R.Sub(x[i], y[i]); END;
  END Sub;

(*---------------------*)
PROCEDURE Neg (VAR z: T; READONLY x: T) RAISES {Error} =
  BEGIN
    AssertEqualSize(z, x);
    FOR i := FIRST(z) TO LAST(z) DO z[i] := R.Neg(x[i]); END;
  END Neg;


(*-----------------*)
PROCEDURE Scale (VAR z: T; READONLY x: T; y: R.T) RAISES {Error} =
  BEGIN
    AssertEqualSize(z, x);
    FOR i := FIRST(z) TO LAST(z) DO z[i] := R.Mul(x[i], y); END;
  END Scale;


(*-----------------*)
PROCEDURE Inner (READONLY x, y: T): R.T RAISES {Error} =
  VAR sum: R.T;
  BEGIN
    AssertEqualSize(x, y);
    sum := R.Zero;
    FOR i := FIRST(x) TO LAST(x) DO
      sum := R.Add(sum, R.Mul(R.Conj(x[i]), y[i]));
    END;
    RETURN sum;
  END Inner;



PROCEDURE Sum (READONLY x: T): R.T =
  VAR sum := R.Zero;
  BEGIN
    FOR i := FIRST(x) TO LAST(x) DO sum := R.Add(sum, x[i]); END;
    RETURN sum;
  END Sum;

(*-----------------*)
BEGIN
END VectorSupport.

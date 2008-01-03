(*******************************************************************************
 * This file was automatically generated by SWIG (http://www.swig.org/).
 * Version: 1.3.22
 *
 * Do not make changes to this file unless you know what you are doing --
 * modify the SWIG interface file instead.
 *******************************************************************************)

INTERFACE LongRealFFTW;
IMPORT Cstdio;


IMPORT LongReal AS R;


TYPE
  Plan <: REFANY;
  Complex =
    RECORD re, im: R.T;  END;    (* compliant to 'arithmetic' library *)
  Dir = {Forward, Backward};

  FlagSet = SET OF Flag;
  Flag = {
         (* documented flags *)
         DestroyInput, Unaligned, ConserveMemory,
         Exhaustive,             (* NO_EXHAUSTIVE is default *)
         PreserveInput,          (* cancels FFTW_DESTROY_INPUT *)
         Patient,                (* IMPATIENT is default *)
         Estimate,

         (* undocumented beyond-guru flags *)
         EstimatePatient, BelievePCost, DFT_R2HC_Icky, NonThreaded_Icky,
         NoBuffering, NoIndirectOp,
         AllowLargeGeneric,      (* NO_LARGE_GENERIC is default *)
         NoRankSplits, NoVRankSplits, NoVRecurse,

         NoSIMD};

CONST Measure = FlagSet{};

TYPE
  R2RKind = {R2HC, HC2R, DHT, REDFT00, REDFT01, REDFT10, REDFT11, RODFT00,
             RODFT01, RODFT10, RODFT11};

EXCEPTION SizeMismatch;

(* maximum comfort routines :-) *)
PROCEDURE DFTR2C1D (READONLY x: ARRAY OF R.T;
                    flags := FlagSet{Flag.Estimate}; ):
  REF ARRAY OF Complex;

PROCEDURE DFTC2R1D (READONLY x: ARRAY OF Complex;
                    parity: [0 .. 1];  (* even or odd output size *)
                    flags              := FlagSet{Flag.Estimate};   ):
  REF ARRAY OF R.T;

PROCEDURE DFTC2C1D (READONLY x   : ARRAY OF Complex;
                             sign: Dir                := Dir.Backward;
                    flags := FlagSet{Flag.Estimate}; ):
  REF ARRAY OF Complex;



(* medium comfort routines *)

PROCEDURE Execute (p: Plan; );

PROCEDURE PlanDFT1D (in, out: REF ARRAY OF Complex;
                     sign   : Dir                    := Dir.Backward;
                     flags: FlagSet := FlagSet{Flag.Estimate}; ): Plan
  RAISES {SizeMismatch};

PROCEDURE PlanDFT2D (in, out: REF ARRAY OF ARRAY OF Complex;
                     sign : Dir     := Dir.Backward;
                     flags: FlagSet := FlagSet{Flag.Estimate}; ): Plan
  RAISES {SizeMismatch};

PROCEDURE PlanDFT3D (in, out: REF ARRAY OF ARRAY OF ARRAY OF Complex;
                     sign : Dir     := Dir.Backward;
                     flags: FlagSet := FlagSet{Flag.Estimate}; ): Plan
  RAISES {SizeMismatch};

PROCEDURE PlanDFTR2C1D (in : REF ARRAY OF R.T;
                        out: REF ARRAY OF Complex;
                        flags: FlagSet := FlagSet{Flag.Estimate}; ): Plan
  RAISES {SizeMismatch};

PROCEDURE PlanDFTR2C2D (in : REF ARRAY OF ARRAY OF R.T;
                        out: REF ARRAY OF ARRAY OF Complex;
                        flags: FlagSet := FlagSet{Flag.Estimate}; ): Plan
  RAISES {SizeMismatch};

PROCEDURE PlanDFTR2C3D (in : REF ARRAY OF ARRAY OF ARRAY OF R.T;
                        out: REF ARRAY OF ARRAY OF ARRAY OF Complex;
                        flags: FlagSet := FlagSet{Flag.Estimate}; ): Plan
  RAISES {SizeMismatch};

PROCEDURE PlanDFTC2R1D (in : REF ARRAY OF Complex;
                        out: REF ARRAY OF R.T;
                        flags: FlagSet := FlagSet{Flag.Estimate}; ): Plan
  RAISES {SizeMismatch};

PROCEDURE PlanDFTC2R2D (in : REF ARRAY OF ARRAY OF Complex;
                        out: REF ARRAY OF ARRAY OF R.T;
                        flags: FlagSet := FlagSet{Flag.Estimate}; ): Plan
  RAISES {SizeMismatch};

PROCEDURE PlanDFTC2R3D (in : REF ARRAY OF ARRAY OF ARRAY OF Complex;
                        out: REF ARRAY OF ARRAY OF ARRAY OF R.T;
                        flags: FlagSet := FlagSet{Flag.Estimate}; ): Plan
  RAISES {SizeMismatch};

PROCEDURE PlanR2R1D (in, out: REF ARRAY OF R.T;
                     kind   : R2RKind;
                     flags: FlagSet := FlagSet{Flag.Estimate}; ): Plan
  RAISES {SizeMismatch};

PROCEDURE PlanR2R2D (in, out     : REF ARRAY OF ARRAY OF R.T;
                     kindx, kindy: R2RKind;
                     flags: FlagSet := FlagSet{Flag.Estimate}; ): Plan
  RAISES {SizeMismatch};

PROCEDURE PlanR2R3D (in, out: REF ARRAY OF ARRAY OF ARRAY OF R.T;
                     kindx, kindy, kindz: R2RKind;
                     flags: FlagSet := FlagSet{Flag.Estimate}; ): Plan
  RAISES {SizeMismatch};

PROCEDURE DestroyPlan (p: Plan; );

PROCEDURE ForgetWisdom ();

PROCEDURE Cleanup ();

PROCEDURE ExportWisdomToFile (output_file: Cstdio.FILE_star; );

PROCEDURE ExportWisdomToString (): TEXT;

PROCEDURE ExportWisdom (VAR write_char: PROCEDURE (c: CHAR; buf: ADDRESS; );
                        VAR data: REFANY; );

PROCEDURE ImportSystemWisdom (): INTEGER;

PROCEDURE ImportWisdomFromFile (input_file: Cstdio.FILE_star; ): INTEGER;

PROCEDURE ImportWisdomFromString (input_string: TEXT; ): INTEGER;

PROCEDURE ImportWisdom (VAR read_char: PROCEDURE (buf: ADDRESS; ): CARDINAL;
                        VAR data: REFANY; ): INTEGER;

PROCEDURE FPrintPlan (p: Plan; output_file: Cstdio.FILE_star; );

PROCEDURE PrintPlan (p: Plan; );

PROCEDURE Flops (p: Plan; VAR add, mul, fma: LONGREAL; );


END LongRealFFTW.

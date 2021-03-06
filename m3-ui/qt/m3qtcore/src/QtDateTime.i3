(*******************************************************************************
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.11
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
*******************************************************************************)

INTERFACE QtDateTime;

FROM QtNamespace IMPORT TimeSpec, DateFormat;





CONST                            (* Enum MonthNameType *)
  ADateFormat      = 0;
  StandaloneFormat = 1;

TYPE                             (* Enum MonthNameType *)
  MonthNameType = [0 .. 1];
PROCEDURE ShortMonthName (month: INTEGER; ): TEXT;

PROCEDURE ShortMonthName1 (month: INTEGER; type: MonthNameType; ): TEXT;

PROCEDURE ShortDayName (weekday: INTEGER; ): TEXT;

PROCEDURE ShortDayName1 (weekday: INTEGER; type: MonthNameType; ): TEXT;

PROCEDURE LongMonthName (month: INTEGER; ): TEXT;

PROCEDURE LongMonthName1 (month: INTEGER; type: MonthNameType; ): TEXT;

PROCEDURE LongDayName (weekday: INTEGER; ): TEXT;

PROCEDURE LongDayName1 (weekday: INTEGER; type: MonthNameType; ): TEXT;

PROCEDURE CurrentDate (): QDate;

PROCEDURE QDate_FromString (s: TEXT; f: DateFormat; ): QDate;

PROCEDURE QDate_FromString1 (s: TEXT; ): QDate;

PROCEDURE QDate_FromString2 (s, format: TEXT; ): QDate;

PROCEDURE QDate_IsValid1 (y, m, d: INTEGER; ): BOOLEAN;

PROCEDURE IsLeapYear (year: INTEGER; ): BOOLEAN;

PROCEDURE FromJulianDay (jd: INTEGER; ): QDate;


TYPE
  QDate <: QDatePublic;
  QDatePublic = BRANDED OBJECT
                  cxxObj: ADDRESS;
                METHODS
                  init_0            (): QDate;
                  init_1            (y, m, d: INTEGER; ): QDate;
                  isNull            (): BOOLEAN;
                  QDate_IsValid     (): BOOLEAN;
                  year              (): INTEGER;
                  month             (): INTEGER;
                  day               (): INTEGER;
                  dayOfWeek         (): INTEGER;
                  dayOfYear         (): INTEGER;
                  daysInMonth       (): INTEGER;
                  daysInYear        (): INTEGER;
                  weekNumber        (VAR yearNum: INTEGER; ): INTEGER;
                  weekNumber1       (): INTEGER;
                  toString          (f: DateFormat; ): TEXT;
                  toString1         (): TEXT;
                  toString2         (format: TEXT; ): TEXT;
                  setYMD            (y, m, d: INTEGER; ): BOOLEAN;
                  setDate           (year, month, day: INTEGER; ): BOOLEAN;
                  getDate           (VAR year, month, day: INTEGER; );
                  addDays           (days: INTEGER; ): QDate;
                  addMonths         (months: INTEGER; ): QDate;
                  addYears          (years: INTEGER; ): QDate;
                  daysTo            (arg1: QDate; ): INTEGER;
                  Equals            (other: QDate; ): BOOLEAN;
                  NotEquals         (other: QDate; ): BOOLEAN;
                  LessThan          (other: QDate; ): BOOLEAN;
                  LessThanEquals    (other: QDate; ): BOOLEAN;
                  GreaterThan       (other: QDate; ): BOOLEAN;
                  GreaterThanEquals (other: QDate; ): BOOLEAN;
                  toJulianDay       (): INTEGER;
                  destroyCxx        ();
                END;

PROCEDURE CurrentTime (): QTime;

PROCEDURE QTime_FromString (s: TEXT; f: DateFormat; ): QTime;

PROCEDURE QTime_FromString1 (s: TEXT; ): QTime;

PROCEDURE QTime_FromString2 (s, format: TEXT; ): QTime;

PROCEDURE IsValid1 (h, m, s, ms: INTEGER; ): BOOLEAN;

PROCEDURE IsValid2 (h, m, s: INTEGER; ): BOOLEAN;


TYPE
  QTime <: QTimePublic;
  QTimePublic = BRANDED OBJECT
                  cxxObj: ADDRESS;
                METHODS
                  init_0            (): QTime;
                  init_1            (h, m, s, ms: INTEGER; ): QTime;
                  init_2            (h, m, s: INTEGER; ): QTime;
                  init_3            (h, m: INTEGER; ): QTime;
                  isNull            (): BOOLEAN;
                  isValid           (): BOOLEAN;
                  hour              (): INTEGER;
                  minute            (): INTEGER;
                  second            (): INTEGER;
                  msec              (): INTEGER;
                  toString          (f: DateFormat; ): TEXT;
                  toString1         (): TEXT;
                  toString2         (format: TEXT; ): TEXT;
                  setHMS            (h, m, s, ms: INTEGER; ): BOOLEAN;
                  setHMS1           (h, m, s: INTEGER; ): BOOLEAN;
                  addSecs           (secs: INTEGER; ): QTime;
                  secsTo            (arg1: QTime; ): INTEGER;
                  addMSecs          (ms: INTEGER; ): QTime;
                  msecsTo           (arg1: QTime; ): INTEGER;
                  Equals            (other: QTime; ): BOOLEAN;
                  NotEquals         (other: QTime; ): BOOLEAN;
                  LessThan          (other: QTime; ): BOOLEAN;
                  LessThanEquals    (other: QTime; ): BOOLEAN;
                  GreaterThan       (other: QTime; ): BOOLEAN;
                  GreaterThanEquals (other: QTime; ): BOOLEAN;
                  start             ();
                  restart           (): INTEGER;
                  elapsed           (): INTEGER;
                  destroyCxx        ();
                END;

PROCEDURE CurrentDateTime (): QDateTime;

PROCEDURE CurrentDateTimeUtc (): QDateTime;

PROCEDURE FromString (s: TEXT; f: DateFormat; ): QDateTime;

PROCEDURE FromString1 (s: TEXT; ): QDateTime;

PROCEDURE FromString2 (s, format: TEXT; ): QDateTime;

PROCEDURE FromTime_t (secsSince1Jan1970UTC: CARDINAL; ): QDateTime;

PROCEDURE FromMSecsSinceEpoch (msecs: CARDINAL; ): QDateTime;

PROCEDURE CurrentMSecsSinceEpoch (): CARDINAL;


TYPE
  QDateTime <: QDateTimePublic;
  QDateTimePublic =
    BRANDED OBJECT
      cxxObj: ADDRESS;
    METHODS
      init_0   (): QDateTime;
      init_1   (arg1: QDate; ): QDateTime;
      init_2   (arg1: QDate; arg2: QTime; spec: TimeSpec; ): QDateTime;
      init_3   (arg1: QDate; arg2: QTime; ): QDateTime;
      init_4   (other: QDateTime; ): QDateTime;
      Assign   (other: QDateTime; ): QDateTime;
      isNull   (): BOOLEAN;
      isValid  (): BOOLEAN;
      date     (): QDate;
      time     (): QTime;
      timeSpec (): TimeSpec;
      toMSecsSinceEpoch  (): CARDINAL;
      toTime_t           (): CARDINAL;
      setDate            (date: QDate; );
      setTime            (time: QTime; );
      setTimeSpec        (spec: TimeSpec; );
      setMSecsSinceEpoch (msecs: CARDINAL; );
      setTime_t          (secsSince1Jan1970UTC: CARDINAL; );
      toString           (f: DateFormat; ): TEXT;
      toString1          (): TEXT;
      toString2          (format: TEXT; ): TEXT;
      addDays            (days: INTEGER; ): QDateTime;
      addMonths          (months: INTEGER; ): QDateTime;
      addYears           (years: INTEGER; ): QDateTime;
      addSecs            (secs: INTEGER; ): QDateTime;
      addMSecs           (msecs: CARDINAL; ): QDateTime;
      toTimeSpec         (spec: TimeSpec; ): QDateTime;
      toLocalTime        (): QDateTime;
      toUTC              (): QDateTime;
      daysTo             (arg1: QDateTime; ): INTEGER;
      secsTo             (arg1: QDateTime; ): INTEGER;
      msecsTo            (arg1: QDateTime; ): CARDINAL;
      Equals             (other: QDateTime; ): BOOLEAN;
      NotEquals          (other: QDateTime; ): BOOLEAN;
      LessThan           (other: QDateTime; ): BOOLEAN;
      LessThanEquals     (other: QDateTime; ): BOOLEAN;
      GreaterThan        (other: QDateTime; ): BOOLEAN;
      GreaterThanEquals  (other: QDateTime; ): BOOLEAN;
      setUtcOffset       (seconds: INTEGER; );
      utcOffset          (): INTEGER;
      destroyCxx         ();
    END;


END QtDateTime.

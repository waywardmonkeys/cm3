/*
Jay Krell
jay.krell@cornell.edu
January 2009

A sort of autoconf replacement, though far less general.
And maybe faster and simpler.

Bundled cc on HP_UX is K&R, so this is K&R,
at least for function prototypes.
We will still try to get away with such ANSI features as
    void*, unsigned/signed char, memset, etc.

On some systems this file cannot run from a.out because
that conflicts with its own output, so, for example:
  cc config.c -o config
  ./config
*/

#define _FILE_OFFSET_BITS 64
#ifdef __STDC__
#include <stdarg.h>
#else
#include <varargs.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include <stddef.h>
#include <pthread.h>
#ifndef _WIN32
#ifndef __STDC__
#define const /* nothing, for bundled HP-UX cc */
#endif
#include <netdb.h>
#include <netinet/in.h>
#include <sys/stat.h>
#endif /* WIN32 */
#include <setjmp.h>
typedef int BOOL;
#define TRUE 1
#define FALSE 0

#define IS_TYPE_SIGNED(x)  (((x)-1) < (x)0)
#define IS_FIELD_SIGNED(x) (((x) = -1) < 0)

/* If (x)-1 generates a warning, try ~(x)0.
If (x) = -1 generates a warning, try (memset(&x, -1, sizeof(x)), x) */

#ifdef __STDC__
#define CHECK(x) ((x) || (CheckFailed(#x), 0))
#else
/* bundled HP-UX cc */
#define CHECK(x) ((x) || (CheckFailed("x"), 0))
#endif

/* Define a 64bit integer type, and verify that it exists. */
#ifndef _MSC_VER
#define __int64 long long
#endif

#define ALIGN_OF(x) (sizeof(struct {char a; x b;}) - sizeof(x))

typedef unsigned U;

FILE* LogFile;

/* Bundled HP-UX does understand stdarg.h */

#if __STDC__
void Print(char* Format, ...)
#else
void Print(Format, va_alist)
    char* Format;
    va_dcl
#endif
{
    va_list Args;

#if __STDC__
    va_start(Args, Format);
#else
    va_start(Args);
#endif
    vprintf(Format, Args);
    vfprintf(LogFile, Format, Args);
    va_end(Args);
}

char* ConcatN(a, n)
    char** a;
    size_t n;
{
    size_t i = 0;
    size_t TotalLength = 1;
    size_t NextLength = 1;
    size_t Length;
    char* Result = 0;
    char* Cursor = 0;

    for (i = 0 ; i != n ; ++i)
    {
        Length = strlen(a[i]);
        NextLength += Length;
        if (NextLength < TotalLength)
        {
            Print("integer overflow\n");
            exit(1);
        }
        TotalLength = NextLength;
    }
    Result = (char*) malloc(TotalLength);
    if (Result == NULL)
    {
        Print("out of memory\n");
        exit(1);
    }
    Cursor = Result;
    for (i = 0 ; i != n ; ++i)
    {
        Length = strlen(a[i]);
        memcpy(Cursor, a[i], Length + 1);
        Cursor += Length;
    }
    return Result;
}

char* Concat3(a, b, c)
    char* a;
    char* b;
    char* c;
{
    char* d[3];
    d[0] = a;
    d[1] = b;
    d[2] = c;
    return ConcatN(d, 3);
}

char* Concat2(a, b)
    char* a;
    char* b;
{
    return Concat3(a, b, "");
}

void CheckFailed(x)
    char* x;
{
    Print("%s is not true; giving up\n", x);
    exit(1);
}

void SanityCheck()
{
    CHECK(CHAR_BIT == 8);

    /* Check that all the types exist and are reasonably sized.
    NOTE that these assumptions are beyond what the standard guarantees.
    */
    CHECK(sizeof(char) == 1);
    CHECK(sizeof(short) == 2);
    CHECK(sizeof(int) == 4);
    CHECK(sizeof(long) >= 4);
    CHECK((sizeof(long) == sizeof(int)) || (sizeof(long) == sizeof(int64_t)));
    CHECK(sizeof(int64_t) == 8);

    CHECK(sizeof(char) == sizeof(unsigned char));
    /*CHECK(sizeof(char) == sizeof(signed char));*/
    CHECK(sizeof(short) == sizeof(unsigned short));
    CHECK(sizeof(int) == sizeof(unsigned int));
    CHECK(sizeof(long) == sizeof(unsigned long));
    CHECK(sizeof(__int64) == sizeof(unsigned __int64));

    CHECK(sizeof(float) == 4);
    CHECK(sizeof(double) == 8);

#ifndef _WIN64
    CHECK(sizeof(long) == sizeof(void*));
#endif
    CHECK(sizeof(char*) == sizeof(void*));
    CHECK(sizeof(void*) == sizeof(void (*)()));

    /* Verify 2's compliment. */

#define UMAX(x) ((x)~(x)0)
#define SMAX(x) (((x)~(x)0) ^ (x)((~(x)0) << (sizeof(x) * 8 - 1)))
#define SMIN(x) ((x)(-SMAX(x) - 1))

    CHECK(SCHAR_MIN == -128);
    CHECK(SCHAR_MAX == 127);
    CHECK(UCHAR_MAX == 255);
    CHECK((CHAR_MIN == 0) || (CHAR_MIN == -128));
    CHECK((CHAR_MAX == 127) || (CHAR_MAX == 255));

    /*CHECK(SCHAR_MIN == SMIN(signed char));*/
    /*CHECK(SCHAR_MAX == SMAX(signed char));*/
    CHECK(UCHAR_MAX == UMAX(unsigned char));

    CHECK(SHRT_MIN == -32768);
    CHECK(SHRT_MAX == 32767);
    CHECK(USHRT_MAX == 65535);

    CHECK(SHRT_MIN == SMIN(short));
    CHECK(SHRT_MAX == SMAX(short));
    CHECK(USHRT_MAX == UMAX(unsigned short));

    CHECK(INT_MIN == (-2147483647 - 1));
    CHECK(INT_MAX == 2147483647);
#ifdef __STDC__
    /* bundled HP-UX cc */
    CHECK(UINT_MAX == 4294967295U);
#endif

    CHECK(INT_MIN == SMIN(int));
    CHECK(INT_MAX == SMAX(int));
    CHECK(UINT_MAX == UMAX(unsigned int));

    if (sizeof(long) == 4)
    {
        CHECK(LONG_MIN == INT_MIN);
        CHECK(LONG_MAX == INT_MAX);
    }
}

void DefineOpaqueType(Name, Size, Align)
    char* Name;
    size_t Size;
    size_t Align;
{
    /* TODO:
    pthread initializers -- should be doable by instantiating them here
    and examining their bits.
    */
    char* Element;

    if ((Size % Align) != 0)
    {
        Print("Size must be multiple of Align (%s, %u, %u)\n", Name, (U)Size, (U)Align);
        exit(1);
    }

    if (Size == sizeof(void*) && Align == ALIGN_OF(void*))
    {
        Print("%s = INTEGER; (* opaque *)\n", Name);
        return;
    }
    if (Size == sizeof(__int64) && Align == ALIGN_OF(__int64))
    {
        Print("%s = LONGINT; (* opaque *)\n", Name);
        return;
    }
    if (Size == sizeof(int) && Align == ALIGN_OF(int))
    {
        Print("%s = int32_t; (* opaque *)\n", Name);
        return;
    }
    if (Align == ALIGN_OF(void*))
        Element = "INTEGER";
    else if (Align == ALIGN_OF(__int64))
        Element = "LONGINT";
    else if (Align > ALIGN_OF(__int64))
    {
        Print("WARNING: %s alignment lowered from %u to LONGINT\n", Name, (U)Align);
        Element = "LONGINT";
    }
    else if (Align == ALIGN_OF(int))
        Element = "uint32_t";
    else if (Align == ALIGN_OF(short))
        Element = "uint16_t";
    else if (Align == ALIGN_OF(char))
        Element = "uint8_t";
    else
    {
        Print("ERROR: unable to represent alignment %u for type %s\n", (U)Align, Name);
        exit(1);
    }
    Print("%s = RECORD opaque: ARRAY [0..%u] OF int; END\n", Name, (U)(Size / Align - 1), Element);
}

#ifdef __STDC__
#define DEFINE_OPAQUE_TYPE(name) DefineOpaqueType(#name, sizeof(name), ALIGN_OF(name))
#else
#define DEFINE_OPAQUE_TYPE(name) DefineOpaqueType("name", sizeof(name), ALIGN_OF(name))
#endif

void DefineIntegerType(Name, Size, Signed, Align)
    char* Name;
    size_t Size;
    BOOL Signed;
    size_t Align;
{
    char* u = 0;

    u = (Signed ? "" : "u");
    Size *= 8;
    Align *= 8;

    Print("%s = %sint%u_t; (* align = %u *)\n", Name, u, (U)Size, (U)Align);
}

#ifdef __STDC__
#define DEFINE_INTEGER_TYPE(x) DefineIntegerType(#x, sizeof(x), IS_TYPE_SIGNED(x), ALIGN_OF(x))
#else
#define DEFINE_INTEGER_TYPE(x) DefineIntegerType("x", sizeof(x), IS_TYPE_SIGNED(x), ALIGN_OF(x))
#endif

void DefineIntegerFieldType(struc, field, myname, Size, Signed)
    char* struc;
    char* field;
    char* myname;
    size_t Size;
    BOOL Signed;
{
    char* u = 0;

    u = (Signed ? "" : "u");
    Size *= 8;

    Print("%s = %sint%u_t; (* %s.%s *)\n", myname, u, (U)Size, struc, field);
}

#ifdef __STDC__
#define DEFINE_INTEGER_FIELD_TYPE(struc, field, myinstance, myname) \
    DefineIntegerFieldType(#struc, #field, #myname, sizeof(myinstance), IS_FIELD_SIGNED(myinstance))
#else
#define DEFINE_INTEGER_FIELD_TYPE(struc, field, myinstance, myname) \
    DefineIntegerFieldType("struc", "field", "myname", sizeof(myinstance), IS_FIELD_SIGNED(myinstance))
#endif

char* Compiler;

char* PossibleCompilers[] = {
    "gcc -lpthread -Werror",
    "gcc -Werror",
    "cc -lpthread -Werror",
    "gcc -lpthread",
    "gcc",
    "cc",

#if 0
    /* MPW compilers */
    "C", /* original 68K */
    "ppcc", /* original PowerPC */
    "MrC", /* later PowerPC?68K */
    "SC", /* later PowerPC?68K */

    /* Windows compilers */
    "cl", /* Microsoft Visual C++ */
    "wcl386", /* Watcom */
    "dmc", /* Digtal Mars */
    "mwcc", /* Metrowerks */
    "sc", /* Symentic, but also the name of another command, that pauses to prompt, beware */
    NULL,
#endif
};

char* PossibleLinkOutput[] = {
    "conf1.exe",
    "a.exe",
    "a.out",
    NULL
};

char* PossibleCompileAndLinkOutput[] = {
    "conf1.exe",
    "conf1.o",
    "conf1.obj",
    "a.exe",
    "a.out",
    /* Check which one of these MPW produces */
    "conf1.c.o",
    "conf1.c.obj",
    NULL
};

char* PossibleCompileOutput[] = {
    "conf1.exe",
    "conf1.o",
    "conf1.obj",
    /* Check which one of these MPW produces */
    "conf1.c.o",
    "conf1.c.obj",
    NULL
};

char* WhichOutputExists(s)
    char** s;
{
    FILE* File = 0;
    size_t i = 0;
    for (i = 0 ; s[i] ; ++i)
    {
        File = fopen(s[i], "rb");
        if (File != NULL)
        {
            fclose(File);
            break;
        }
    }
    return s[i];
}

BOOL DoesOutputExist(s)
    char** s;
{
    return (WhichOutputExists(s) != NULL);
}

void DeleteOutput(s)
    char** s;
{
    size_t i = 0;
    for (i = 0 ; s[i] ; ++i)
    {
        unlink(s[i]);
    }
}

void DeleteCompileOutput()
{
    DeleteOutput(PossibleCompileOutput);
}

void DeleteCompileAndLinkOutput()
{
    DeleteOutput(PossibleCompileAndLinkOutput);
}

BOOL DoesCompileOutputExist()
{
    return DoesOutputExist(PossibleCompileOutput);
}

BOOL DoesCompileAndLinkOutputExist()
{
    return DoesOutputExist(PossibleCompileAndLinkOutput);
}

void CreateSourceFile(Snippet)
    char* Snippet;
{
    size_t i = 0;
    FILE* FileHandle = 0;

    FileHandle = fopen("conf1.c", "w");
    if (FileHandle == NULL)
    {
        Print("fopen(conf1.c, w) failed\n");
        exit(1);
    }
    fprintf(LogFile, "compiling: %s", Snippet);
    fprintf(FileHandle, "%s", Snippet);
    fclose(FileHandle);
}

BOOL TryCompile(Snippet)
    char* Snippet;
{
    char* CommandLine = 0;
    int ExitCode = 0;
    BOOL Exists = 0;

    CommandLine = Concat2(Compiler, " -c conf1.c");
    CreateSourceFile(Snippet);
    fprintf(LogFile, "running: %s\n", CommandLine);
    ExitCode = system(CommandLine);
    fprintf(LogFile, "=> %d\n", ExitCode);
    free(CommandLine);
    Exists = DoesCompileOutputExist();
    DeleteCompileOutput();
    return ((ExitCode == 0) && Exists);
}

BOOL TryCompileAndLink(Snippet)
    char* Snippet;
{
    char* CommandLine = 0;
    int ExitCode = 0;
    BOOL Exists = 0;

    CommandLine = Concat2(Compiler, " conf1.c");
    CreateSourceFile(Snippet);
    fprintf(LogFile, "running: %s\n", CommandLine);
    ExitCode = system(CommandLine);
    fprintf(LogFile, "=> %d\n", ExitCode);
    free(CommandLine);
    Exists = DoesCompileAndLinkOutputExist();
    DeleteCompileAndLinkOutput();
    return ((ExitCode == 0) && Exists);
}

BOOL TryCompileAndLinkAndRun(Snippet)
    char* Snippet;
{
    char* CommandLine = 0;
    int ExitCode = 0;
    char* Exists = 0;

    CommandLine = Concat2(Compiler, " conf1.c");
    CreateSourceFile(Snippet);
    fprintf(LogFile, "running: %s\n", CommandLine);
    ExitCode = system(CommandLine);
    free(CommandLine);
    Exists = WhichOutputExists(PossibleLinkOutput);
    if (Exists == FALSE)
        return FALSE;
    CommandLine = Concat2("./", Exists);
    ExitCode = system(CommandLine);
    DeleteCompileAndLinkOutput();
    return (ExitCode == 0);
}

char* DevNull;
char* PossibleDevNull[] = { "nul:", "/dev/null", NULL };

void FindDevNull()
{
    size_t i = 0;
    FILE* File = 0;

    if (DevNull)
        return;

    Print("looking for /dev/null..");
    for (i = 0 ; DevNull = PossibleDevNull[i] ; ++i)
    {
        File = fopen(DevNull, "r");
        if (File != NULL)
        {
            fclose(File);
            Print("%s\n", DevNull);
            return;
        }
    }
    Print("no /dev/null found\n");
    exit(1);
}

void FindCompiler()
{
    size_t i = 0;

    if (Compiler)
        return;

    Print("looking for C compiler..");
    for (i = 0 ; Compiler = PossibleCompilers[i] ; ++i)
    {
        if (TryCompile("int main() { return 0; }\n"))
        {
            Print("%s\n", Compiler);
            return;
        }
    }
    Print("no C compiler found\n");
    exit(1);
}

BOOL CheckHeader(Header)
    char* Header;
{
    char* Source;
    BOOL Result;

    Source = Concat3("#include ", Header, "\n");
    Result = TryCompile(Source);
    free(Source);
    return Result;
}

BOOL CheckField(Prefix, Struct, Type, Field)
    char* Prefix;
    char* Struct;
    char* Type;
    char* Field;
{
    char* Source = 0;
    char* a[30];
    BOOL Result = 0;
    size_t i = 0;

    if (Type[0])
    {
        if (CheckField(Prefix, Struct, "", Field) == FALSE)
        {
            return FALSE;
        }
    }

    Print("checking for %s%s%s.%s\n", Type, Type[0] ? " " : "", Struct, Field);

    a[i++] = Prefix;
    a[i++] = Struct;
    a[i++] = " a;\n";
    a[i++] = Type[0] ? Type : "void";
    a[i++] = "* b = ";
    a[i++] = "&a.";
    a[i++] = Field;
    a[i++] = ";\n";
    Source = ConcatN(a, i);
    Result = TryCompile(Source);
    free(Source);

    do
    {
        if (Type[0] && Result)
        {
            /* gcc is lenient here and only warns; try another test? */
            /* unable to come up with one, will try -Werror */

            /* HP-UX bundled cc also accepts such code without warning,
            and I'm not able to find a way to make it an error.
            We can do some other things though.
            We can assert that the size matches.
            We can make another variable of the type, assign a non-zero value,
            and memcmp the two. This at least discerns float from int.
            It does not discern various pointer types and integer types.
            */
            i = 0;
            a[i++] = Prefix;
            a[i++] = Struct;
            a[i++] = " a;\n";
            /* char a[(sizeof(b) == sizeof(c)) ? 1 : -1];
               is legal if the sizes are the same, else illegal.
            Remember to test this on SGI. History says it won't compile either way. */
            a[i++] = "char b[(sizeof(a.";
            a[i++] = Field;
            a[i++] = ") == sizeof(";
            a[i++] = Type;
            a[i++] = ")) ? 1 : -1];\n";
            Source = ConcatN(a, i);
            Result = TryCompile(Source);
            free(Source);
            if (Result == FALSE)
                break;

            i = 0;
            a[i++] = Prefix;
            a[i++] = Struct;
            a[i++] = " a;\n";
            a[i++] = "int main() {";
            a[i++] = Type;
            a[i++] = " b;\n";
            a[i++] = "b = (";
            a[i++] = Type;
            a[i++] = ")1;\n";

            a[i++] = "a.";
            a[i++] = Field;
            a[i++] = " = (";
            a[i++] = Type;
            a[i++] = ")1;\n";
            a[i++] = "return (memcmp(&a, &b, sizeof(b) != 0));}\n";
            Source = ConcatN(a, i);
            Result = TryCompileAndLinkAndRun(Source);
            free(Source);
        }
    } while(0);

    if (Type[0])
    {
        if (Result)
            Print("%s.%s DOES exist, with type %s\n", Struct, Field, Type);
        else
            Print("%s.%s DOES exist, but is NOT of type %s\n", Struct, Field, Type);
    }
    else
    {
        if (Result)
            Print("%s.%s DOES exist, of unspecified type\n", Struct, Field);
        else
            Print("%s.%s does NOT exist\n", Struct, Field);
    }
    return Result;
}

BOOL CheckGlobalVariable(Prefix, Type, Name)
    char* Prefix;
    char* Type;
    char* Name;
{
    char* Source = 0;
    char* a[20];
    BOOL Result = 0;
    size_t i = 0;

    if (Type[0])
    {
        if (CheckGlobalVariable(Prefix, "", Name) == FALSE)
        {
            return FALSE;
        }
    }

    Print("checking for %s%s%s\n", Type, Type[0] ? " " : "", Name);

    a[i++] = Prefix;
    a[i++] = "int main() { ";
    a[i++] = Type[0] ? Type : "void";
    a[i++] = "* a = &";
    a[i++] = Name;
    a[i++] = "; return 0;}\n";
    Source = ConcatN(a, i);
    Result = TryCompileAndLink(Source);
    free(Source);

    /* Again, compilers are very lenient and allow mixing pointer types without error,
    so try to check a little better. Even these checks can fail. */

    do
    {
        if (Type[0] && Result)
        {
            i = 0;
            a[i++] = Prefix;
            /* char a[(sizeof(b) == sizeof(c)) ? 1 : -1];
               is legal if the sizes are the same, else illegal.
            Remember to test this on SGI. History says it won't compile either way. */
            a[i++] = "char b[(sizeof(";
            a[i++] = Name;
            a[i++] = ") == sizeof(";
            a[i++] = Type;
            a[i++] = ")) ? 1 : -1];\n";
            Source = ConcatN(a, i);
            Result = TryCompile(Source);
            free(Source);
            if (Result == FALSE)
                break;

            i = 0;
            a[i++] = Prefix;
            a[i++] = "int main() {";
            a[i++] = Type;
            a[i++] = " a;\n";

            a[i++] = "a = (";
            a[i++] = Type;
            a[i++] = ")1;\n";

            a[i++] = Name;
            a[i++] = " = (";
            a[i++] = Type;
            a[i++] = ")1;\n";
            a[i++] = "return (memcmp(&";
            a[i++] = Name;
            a[i++] = ", &a, sizeof(a) != 0));}\n";
            Source = ConcatN(a, i);
            Result = TryCompileAndLinkAndRun(Source);
            free(Source);
        }
    } while(0);

    if (Type[0])
    {
        if (Result)
            Print("%s DOES exist, of type %s\n", Name, Type);
        else
            Print("%s does NOT exist or is not of type %s\n", Name, Type);
    }
    else
    {
        if (Result)
            Print("%s DOES exist, of unspecified type\n", Name);
        else
            Print("%s does NOT exist, of any type\n", Name);
    }
    return Result;
}

void StackDirection(a)
    char* a;
{
    char b = 0;

    if (a > &b)
        Print("stack grows down\n");
    else
        Print("stack grows up\n");
}

void Config()
{
    union {
        char bytes[sizeof(int)];
        int value;
    } endian;

    memset(&endian, 0, sizeof(endian));
    endian.bytes[0] = 1;

    LogFile = fopen("config.log", "w");
    if (LogFile == NULL)
    {
        Print("unable to open config.log\n");
        exit(1);
    }

    endian.value = (endian.value == 1);
    Print(endian.value ? "little endian\n" : "big endian\n");

    StackDirection(&endian.bytes[0]);

    SanityCheck();

    /* get the alignments and check that infrastructure works */

    DEFINE_INTEGER_TYPE(short);
    DEFINE_INTEGER_TYPE(int);
    DEFINE_INTEGER_TYPE(long); /* WordSize, except on Win64 */
    DEFINE_INTEGER_TYPE(__int64);

    DEFINE_INTEGER_TYPE(unsigned short);
    DEFINE_INTEGER_TYPE(unsigned int);
    DEFINE_INTEGER_TYPE(unsigned long); /* WordSize, except on Win64 */
    DEFINE_INTEGER_TYPE(unsigned __int64);

    DEFINE_INTEGER_TYPE(size_t);
    DEFINE_INTEGER_TYPE(ptrdiff_t);

    DEFINE_INTEGER_TYPE(pid_t);
    DEFINE_INTEGER_TYPE(gid_t);
    DEFINE_INTEGER_TYPE(clock_t);
    DEFINE_INTEGER_TYPE(uid_t);
    DEFINE_INTEGER_TYPE(time_t);
    DEFINE_INTEGER_TYPE(off_t);

    FindDevNull();
    FindCompiler();

    CheckHeader("<time.h>");

    CheckField("#include <time.h>\n", "struct tm", "int", "tm_sec");
    CheckField("#include <time.h>\n", "struct tm", "int", "tm_min");
    CheckField("#include <time.h>\n", "struct tm", "int", "tm_hour");
    CheckField("#include <time.h>\n", "struct tm", "int", "tm_mday");
    CheckField("#include <time.h>\n", "struct tm", "int", "tm_mon");
    CheckField("#include <time.h>\n", "struct tm", "int", "tm_year");
    CheckField("#include <time.h>\n", "struct tm", "int", "tm_wday");
    CheckField("#include <time.h>\n", "struct tm", "int", "tm_yday");
    CheckField("#include <time.h>\n", "struct tm", "int", "tm_isdst");
    CheckField("#include <time.h>\n", "struct tm", "int", "tm_isdst");
    CheckField("#include <time.h>\n", "struct tm", "int", "tm_gmtoff");
    CheckField("#include <time.h>\n", "struct tm", "int", "tm_zone");

    /* one we know which fields exist, the next thing is to construct a program
    that checks that we have all the fields, sort them by offset, and declare it;
    or write a little more Modula-3 code in C */

    /* test code */
    CheckField("#include <time.h>\n", "struct tm", "float", "tm_sec");
    CheckField("#include <time.h>\n", "struct tm", "float", "tm_zone");

    /* The underscore names are favored on Cygwin, since they are macros for functions. */

    if (CheckGlobalVariable("#include <time.h>\n", "int", "_daylight") == FALSE)
    {
        CheckGlobalVariable("#include <time.h>\n", "int", "daylight");
    }

    if (CheckGlobalVariable("#include <time.h>\n", "long", "_timezone") == FALSE)
    {
        if (CheckGlobalVariable("#include <time.h>\n", "int", "_timezone") == FALSE)
        {
            CheckGlobalVariable("#include <time.h>\n", "long", "timezone");
            CheckGlobalVariable("#include <time.h>\n", "int", "timezone");
        }
    }

    {
        /* test code */
        CheckGlobalVariable("int i;\n", "int", "i");
        CheckGlobalVariable("int i;\n", "int", "j");
        CheckGlobalVariable("int i;\n", "float", "i");
        CheckGlobalVariable("int i;\n", "float", "j");
        CheckGlobalVariable("int i;\n", "double", "i");

        CheckField("typedef struct { int i;} T;\n", "T", "int ", "j");
        CheckField("typedef struct { int i;} T;\n", "T", "int", "j");
        CheckField("typedef struct { int i;} T;\n", "T", "float", "j");
        CheckField("typedef struct { int i;} T;\n", "T", "float", "j");
    }

    /* bundled HP-UX cc */
    {
        /*
        We should probably just write C code for this.

          hostent_addrtype_t = int16_t;
          hostent_length_t = int16_t;

          struct_hostent = RECORD
            h_name:       char_star;
            h_aliases:    char_star_star;
            h_addrtype:   hostent_addrtype_t;
            h_length:     hostent_length_t;
            h_addr_list:  char_star_star;
          END;
        */
        typedef struct hostent hostent_t;
        hostent_t hostent;
        DEFINE_INTEGER_FIELD_TYPE(hostent_t, h_addrtype, hostent.h_addrtype, hostent_addrtype_t);
        DEFINE_INTEGER_FIELD_TYPE(hostent_t, h_length, hostent.h_length, hostent_length_t);
    }

    {
        /* test code */
        typedef struct { unsigned char a; /*signed*/ char b; unsigned short c; short d; unsigned int e;
        int f; unsigned __int64 g; __int64 h; } T;
        T t;

        DEFINE_INTEGER_FIELD_TYPE(T, a, t.a, a_t);
        DEFINE_INTEGER_FIELD_TYPE(T, b, t.b, b_t);
        DEFINE_INTEGER_FIELD_TYPE(T, c, t.c, c_t);
        DEFINE_INTEGER_FIELD_TYPE(T, d, t.d, d_t);
        DEFINE_INTEGER_FIELD_TYPE(T, e, t.e, e_t);
        DEFINE_INTEGER_FIELD_TYPE(T, f, t.f, f_t);
        DEFINE_INTEGER_FIELD_TYPE(T, g, t.g, g_t);
        DEFINE_INTEGER_FIELD_TYPE(T, h, t.h, h_t);
    }

    DEFINE_OPAQUE_TYPE(pthread_t);
    DEFINE_OPAQUE_TYPE(pthread_attr_t);
    DEFINE_OPAQUE_TYPE(pthread_mutex_t);
    DEFINE_OPAQUE_TYPE(pthread_cond_t);
    DEFINE_OPAQUE_TYPE(pthread_key_t);

    DEFINE_OPAQUE_TYPE(jmp_buf);

#ifndef _WIN32
    /* test code */
    {
        typedef struct stat stat_t;
        DEFINE_OPAQUE_TYPE(stat_t);
    }
#endif

    /* There are two possible definitions of uin.h. Check for each. */
/*
  struct_in_addr = RECORD
    s_addr: unsigned;
  END;

  struct_sockaddr_in = RECORD
    sin_len: unsigned_char; (* This is absent on most platforms. *)
    sin_family: unsigned_char; (* This is 16 bits on most platforms. *)
    sin_port: unsigned_short;
    sin_addr: struct_in_addr;
    sin_zero: ARRAY [0..7] OF char;
  END;

  struct_sockaddr_in = RECORD
    sin_family: unsigned_short; (* this is signed on some platforms; it does not matter *)
    sin_port: unsigned_short;
    sin_addr: struct_in_addr;
    sin_zero: ARRAY [0..7] OF char;
  END;
*/
    {
        typedef struct in_addr in_addr_t;
        typedef struct { unsigned addr; } in_addr2_t;
        in_addr2_t a;
        in_addr_t b;
        CHECK(sizeof(a) == sizeof(b));
        CHECK(ALIGN_OF(in_addr2_t) == ALIGN_OF(in_addr_t));
        a.addr = 1234;
        b.s_addr = 1234;
        CHECK(memcmp(&a, &b, sizeof(a)) == 0);
    }
    {
        typedef struct in_addr in_addr_t;
        typedef struct sockaddr_in sockaddr_in_t;
        typedef struct { unsigned short family, port; in_addr_t addr; char zero[8]; } sockaddr_in_nolen_t;
        typedef struct { unsigned char len, family; unsigned short port; in_addr_t addr; char zero[8]; } sockaddr_in_len_t;
        sockaddr_in_t a;
        sockaddr_in_nolen_t nolen;
        sockaddr_in_len_t len;
        const static char Prefix[] = "#include <netinet/in.h>\n";

        /* This could be relaxed but our memcmps below are sloppy. */
        CHECK(sizeof(len) == sizeof(nolen));
        CHECK(sizeof(len) == sizeof(a));
        CHECK(ALIGN_OF(sockaddr_in_len_t) == ALIGN_OF(sockaddr_in_nolen_t));
        CHECK(ALIGN_OF(sockaddr_in_len_t) == ALIGN_OF(sockaddr_in_t));

        /* A more correct check. */
        CHECK((sizeof(a) == sizeof(len)) || (sizeof(a) == sizeof(nolen)));
        CHECK((ALIGN_OF(sockaddr_in_t) == ALIGN_OF(sockaddr_in_len_t)) || (ALIGN_OF(sockaddr_in_t) == ALIGN_OF(sockaddr_in_nolen_t)));

        CHECK((sizeof(a.sin_family) == 2) || (sizeof(a.sin_family) == 1));
        CHECK(sizeof(a.sin_port) == 2);
        CHECK(sizeof(a.sin_addr) == 4);
        CHECK(sizeof(a.sin_zero) == 8);

        memset(&a, 0, sizeof(a));
        memset(&len, 0, sizeof(len));
        memset(&nolen, 0, sizeof(nolen));

        nolen.family = len.family = a.sin_family = 1;
        nolen.port = len.port = a.sin_port = 2;
        nolen.addr.s_addr = len.addr.s_addr = a.sin_addr.s_addr = 3;
        nolen.zero[0] = len.zero[0] = a.sin_zero[0] = 4;
        nolen.zero[1] = len.zero[1] = a.sin_zero[1] = 5;
        nolen.zero[2] = len.zero[2] = a.sin_zero[2] = 6;
        nolen.zero[3] = len.zero[3] = a.sin_zero[3] = 7;
        nolen.zero[4] = len.zero[4] = a.sin_zero[4] = 8;
        nolen.zero[5] = len.zero[5] = a.sin_zero[5] = 9;
        nolen.zero[6] = len.zero[6] = a.sin_zero[6] = 10;
        nolen.zero[7] = len.zero[7] = a.sin_zero[7] = 11;

        CHECK((memcmp(&a, &len, sizeof(len)) == 0) || (memcmp(&a, &nolen, sizeof(nolen)) == 0));

        /* This check is confused on a big endian system, so.. */
#if 0
        if (memcmp(&a, &len, sizeof(len)) == 0)
        {
            CheckField(Prefix, "struct sockaddr_in", "unsigned char", "len");
            CHECK(sizeof(a.sin_family) == 1);
            printf("sockaddr_in_t => has len field\n");
        }
        else
        {
            CHECK(memcmp(&a, &nolen, sizeof(nolen)) == 0);
            CHECK(sizeof(a.sin_family) == 2);
            printf("sockaddr_in_t => no len field\n");
        }
#else
        if (sizeof(a.sin_family) == 2)
        {
            CHECK(memcmp(&a, &nolen, sizeof(nolen)) == 0);
            if (CheckField(Prefix, "struct sockaddr_in", "unsigned char", "len") == TRUE)
            {
                printf("ERROR: confused about sockaddr_in_t.len\n");
                exit(1);
            }
            printf("sockaddr_in_t => no len field\n");
        }
        else if (sizeof(a.sin_family) == 1)
        {
            CHECK(memcmp(&a, &len, sizeof(len)) == 0);
            if (CheckField(Prefix, "struct sockaddr_in", "unsigned char", "len") == FALSE)
            {
                printf("ERROR: confused about sockaddr_in_t.len\n");
                exit(1);
            }
            printf("sockaddr_in_t => len field\n");
        }
        else
        {
            printf("ERROR: confused about sockaddr_in_t\n");
            exit(1);
        }
#endif
    }

    Print("done\n");

    fclose(LogFile);
}

int main()
{
    Config();
    return 0;
}

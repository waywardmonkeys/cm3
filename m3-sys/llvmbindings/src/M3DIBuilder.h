// File: m3-sys/llvmbindings/src/M3DIBUILDER.h 
// Implementation of a C binding to DIBuilder.  
// Derived From llvm 3.6.1, file: 

//===--- llvm/IR/DIBuilder.h - Debug Information Builder --------*- C++ -*-===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is distributed under the University of Illinois Open Source
// License. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//
//
// This file defines a DIBuilder that is useful for creating debugging
// information entries in LLVM IR form.
//
//===----------------------------------------------------------------------===//

#ifndef M3DIBUILDER_C_H
#define M3DIBUILDER_C_H

#include <sys/types.h>
#include "llvm/ADT/ArrayRef.h"
#include "llvm/ADT/StringRef.h"
#include "llvm/IR/DIBuilder.h"
#include "llvm-c/Support.h"

//#include "llvm/IR/DebugInfo.h"
//#include "llvm/IR/TrackingMDRef.h"
//#include "llvm/IR/ValueHandle.h"
//#include "llvm/Support/DataTypes.h" //Included by Core.h

#include "llvm-c/Core.h"

#ifdef __cplusplus
extern "C" {
#endif

/// Types that are omitted from Core.h:

typedef struct LLVMOpaqueConstant * LLVMConstantRef; 

/* The debug info nodes themselves: */ 
/// NOTE: Unlike regular llvm IR nodes, these are very small classes that 
/// contain only a pointer to an MDNode.  They are passed in and returned 
/// by value, as structs, with no pointer thereto. 

/// FIXME: The members of the type hierarchy rooted at DIDescriptor will 
/// probably have to be flattened into DIDescriptor to allow widening of 
/// values returned from these functions, when passed back in to other 
/// functions.   
typedef struct OpaqueDIBuilder * LLVMDIBuilderRef;
typedef struct OpaqueMDNode * LLVMMDNodePtr; 
typedef struct OpaqueMetadata * LLVMMetadataPtr; 

typedef struct DITypeRef * LLVMDITypeRef;

// We can't access the single data member MDNode of any subclass of 
// llvm::DIDescriptor because it's protected, has no appropriate constructor, 
// is not our friend, etc.  So we unsafely cast between it and structurally
// identical struct CDebugInfo, with data member DbgPtr, in order to get
// the MDNode in and out:   
typedef struct CDebugInfo { OpaqueMetadata * DbgPtr; } * LLVMCDebugInfo;

// These are all distinct opaque pointers, so we can have lots of overloaded
// unwrap functions on them, so, with inexcusable laziness, we avoid a massive 
// and error-prone job of typing lots of distinctly-named unwrap functions.
// Someday, fix this.

// Also, these actually hold the value of the MDNode pointer, because passing
// a struct, even with only a single, pointer-sized data member suffers ABI 
// mismatches between C and Modula-3 compilers.  We put the MDNode pointer inside
// an llvm class and remove it, when unwrapping/wrapping.    
typedef struct DIBasicType * LLVMDIBasicType;
typedef struct DICompileUnit * LLVMDICompileUnit;
typedef struct DICompositeType * LLVMDICompositeType;
typedef struct DIDerivedType * LLVMDIDerivedType;
typedef struct DIDescriptor * LLVMDIDescriptor;
typedef struct DIFile * LLVMDIFile;
typedef struct DIEnumerator * LLVMDIEnumerator;
typedef struct DIType * LLVMDIType;
typedef struct DITypeArray * LLVMDITypeArray;
typedef struct DISubroutineType * LLVMDISubroutineType;
typedef struct DIArray * LLVMDIArray;
typedef struct DIGlobalVariable * LLVMDIGlobalVariable;
typedef struct DIExpression * LLVMDIExpression;
typedef struct DILocation * LLVMDILocation;
typedef struct DIImportedEntity * LLVMDIImportedEntity;
typedef struct DINameSpace * LLVMDINameSpace;
typedef struct DIVariable * LLVMDIVariable;
typedef struct DISubrange * LLVMDISubrange;
typedef struct DILexicalBlockFile * LLVMDILexicalBlockFile;
typedef struct DILexicalBlock * LLVMDILexicalBlock;
typedef struct DIScope * LLVMDIScope;
typedef struct DISubprogram * LLVMDISubprogram;
typedef struct DITemplateTypeParameter * LLVMDITemplateTypeParameter;
typedef struct DITemplateValueParameter * LLVMDITemplateValueParameter;
typedef struct DIObjCProperty * LLVMDIObjCProperty;

/* Other llvm types needed as parameters to functions prototyped here. */ 

// llvm::StringRef, is not legal C, replace it by: 
// It would have been cleaner to just pass the struct by value, but this fails
// due to ABI mismatches on how a 2-word struct is passed, at least for AMD_64,
// between gcc 4.8.1 and Modula-3's code generator, a derivative of gcc 4.7. 
// So we pass the struct by reference.  
typedef struct StringRefStruct { char *Data; size_t Length; } 
  const * LLVMStringRef;

// llvm::ArrayRef<Metadata *> is not legal C, replace it by: 
struct LLVMArrayRefOfMetadataPtr { LLVMMetadataPtr *Data; size_t Length; };

// llvm::ArrayRef<int64_t> is not legal C, replace it by: 
struct LLVMArrayRefOfint64_t { int64_t *Data; size_t Length; };

//struct Function;
//struct Module;
//struct Value;
//enum ComplexAddrKind { OpPlus=1, OpDeref };

/* The debug info builder class, whose member functions are bound here as
   ordinary C functions. */ 

/// Constructor of a DIBuilder.  
LLVMDIBuilderRef 
DIBBuilderCreate(LLVMModuleRef Mod, LLVMBool AllowUnresolved) ;

/// finalize - Construct any deferred debug info descriptors.
void DIBfinalize(LLVMDIBuilderRef Builder) ;

void DIBFree(LLVMDIBuilderRef Builder) ;

/* The debug info node constructors: */ 

/// createCompileUnit - A CompileUnit provides an anchor for all debugging
/// information generated during this instance of compilation.
/// @param Lang     Source programming language, eg. dwarf::DW_LANG_C99
/// @param File     File name
/// @param Dir      Directory
/// @param Producer Identify producer of debugging information and code.
///                 Usually this is a compiler version string.
/// @param isOptimized A boolean flag which indicates whether optimization
///                    is ON or not.
/// @param Flags    This string lists command line options. This string is
///                 directly embedded in debug info output which may be used
///                 by a tool analyzing generated debugging information.
/// @param RV       This indicates runtime version for languages like
///                 Objective-C.
/// @param SplitName The name of the file that we'll split debug info out
///                  into.
/// @param Kind     The kind of debug information to generate.
/// @param EmitDebugInfo   A boolean flag which indicates whether debug
///                        information should be written to the final
///                        output or not. When this is false, debug
///                        information annotations will be present in
///                        the IL but they are not written to the final
///                        assembly or object file. This supports tracking
///                        source location information in the back end
///                        without actually changing the output (e.g.,
///                        when using optimization remarks).
LLVMDICompileUnit DIBcreateCompileUnit(LLVMDIBuilderRef Builder,  
                                unsigned Lang,  
                                LLVMStringRef File,
                                LLVMStringRef Dir,  
                                LLVMStringRef Producer,
                                LLVMBool isOptimized,  
                                LLVMStringRef Flags,
                                unsigned RV,
                                LLVMStringRef SplitName,
                                llvm::DIBuilder::DebugEmissionKind Kind,
                                LLVMBool EmitDebugInfo) ;

/// createFile - Create a file descriptor to hold debugging information
/// for a file.
LLVMDIFile DIBcreateFile(LLVMDIBuilderRef Builder,  
                                LLVMStringRef Filename,  
                                LLVMStringRef Directory) ;

/// createEnumerator - Create a single enumerator value.
LLVMDIEnumerator DIBcreateEnumerator(LLVMDIBuilderRef Builder,
                                LLVMStringRef Name,
                                int64_t Val) ;

/// \brief Create a DWARF unspecified type.
LLVMDIBasicType DIBcreateUnspecifiedType(LLVMDIBuilderRef Builder,
                                LLVMStringRef Name) ;

/// \brief Create C++11 nullptr type.
LLVMDIBasicType DIBcreateNullPtrType(LLVMDIBuilderRef Builder) ;

/// createBasicType - Create debugging information entry for a basic
/// type.
/// @param Name        Type name.
/// @param SizeInBits  Size of the type.
/// @param AlignInBits Type alignment.
/// @param Encoding    DWARF encoding code, e.g. dwarf::DW_ATE_float.
LLVMDIBasicType DIBcreateBasicType(LLVMDIBuilderRef Builder,
                           LLVMStringRef Name,
                           uint64_t SizeInBits,
                           uint64_t AlignInBits,
                           unsigned Encoding) ;

/// createQualifiedType - Create debugging information entry for a qualified
/// type, e.g. 'const int'.
/// @param Tag         Tag identifing type, e.g. dwarf::TAG_volatile_type
/// @param FromTy      Base Type.
LLVMDIDerivedType DIBcreateQualifiedType(LLVMDIBuilderRef Builder,
                                unsigned Tag,
                                LLVMDIType FromTy) ;

/// createPointerType - Create debugging information entry for a pointer.
/// @param PointeeTy   Type pointed by this pointer.
/// @param SizeInBits  Size.
/// @param AlignInBits Alignment. (optional)
/// @param Name        Pointer type name. (optional)
LLVMDIDerivedType DIBcreatePointerType(LLVMDIBuilderRef Builder,
                                LLVMDIType PointeeTy,
                                uint64_t SizeInBits,
                                uint64_t AlignInBits,
                                LLVMStringRef Name) ;

/// \brief Create debugging information entry for a pointer to member.
/// @param PointeeTy Type pointed to by this pointer.
/// @param Class Type for which this pointer points to members of.
/// @param SizeInBits  Size.
/// @param AlignInBits Alignment. (optional)
LLVMDIDerivedType DIBcreateMemberPointerType(LLVMDIBuilderRef Builder,
                                LLVMDIType PointeeTy,
                                LLVMDIType Class,
                                uint64_t SizeInBits,
                                uint64_t AlignInBits) ;

/// createReferenceType - Create debugging information entry for a c++
/// style reference or rvalue reference type.
LLVMDIDerivedType DIBcreateReferenceType(LLVMDIBuilderRef Builder,
                                unsigned Tag,
                                LLVMDIType RTy) ;

/// createTypedef - Create debugging information entry for a typedef.
/// @param Ty          Original type.
/// @param Name        Typedef name.
/// @param File        File where this type is defined.
/// @param LineNo      Line number.
/// @param Context     The surrounding context for the typedef.
LLVMDIDerivedType DIBcreateTypedef( LLVMDIBuilderRef Builder,
                                LLVMDIType Ty,
                                LLVMStringRef Name,
                                LLVMDIFile File,
                                unsigned LineNo,
                                LLVMDIDescriptor Context) ;

/// createFriend - Create debugging information entry for a 'friend'.
LLVMDIDerivedType DIBcreateFriend(  LLVMDIBuilderRef Builder,
                                LLVMDIType Ty,
                                LLVMDIType FriendTy) ;

/// createInheritance - Create debugging information entry to establish
/// inheritance relationship between two types.
/// @param Ty           Original type.
/// @param BaseTy       Base type. Ty is inherits from base.
/// @param BaseOffset   Base offset.
/// @param Flags        Flags to describe inheritance attribute,
///                     e.g. private
LLVMDIDerivedType DIBcreateInheritance(LLVMDIBuilderRef Builder,
                                LLVMDIType Ty,
                                LLVMDIType BaseTy,
                                uint64_t BaseOffset,
                                unsigned Flags) ;

/// createMemberType - Create debugging information entry for a member.
/// @param Scope        Member scope.
/// @param Name         Member name.
/// @param File         File where this member is defined.
/// @param LineNo       Line number.
/// @param SizeInBits   Member size.
/// @param AlignInBits  Member alignment.
/// @param OffsetInBits Member offset.
/// @param Flags        Flags to encode member attribute, e.g. private
/// @param Ty           Parent type.
LLVMDIDerivedType DIBcreateMemberType(LLVMDIBuilderRef Builder,
                                LLVMDIDescriptor Scope,
                                LLVMStringRef Name,
                                LLVMDIFile File,
                                unsigned LineNo,
                                uint64_t SizeInBits,
                                uint64_t AlignInBits,
                                uint64_t OffsetInBits,
                                unsigned Flags,
                                LLVMDIType Ty) ;


/// createStaticMemberType - Create debugging information entry for a
/// C++ static data member.
/// @param Scope      Member scope.
/// @param Name       Member name.
/// @param File       File where this member is declared.
/// @param LineNo     Line number.
/// @param Ty         Type of the static member.
/// @param Flags      Flags to encode member attribute, e.g. private.
/// @param Val        Const initializer of the member.
LLVMDIDerivedType DIBcreateStaticMemberType(LLVMDIBuilderRef Builder,
                                LLVMDIDescriptor Scope,
                                LLVMStringRef Name,
                                LLVMDIFile File,
                                unsigned LineNo,
                                LLVMDIType Ty,
                                unsigned Flags,
                                LLVMConstantRef Val) ;

/// createObjCIVar - Create debugging information entry for Objective-C
/// instance variable.
/// @param Name         Member name.
/// @param File         File where this member is defined.
/// @param LineNo       Line number.
/// @param SizeInBits   Member size.
/// @param AlignInBits  Member alignment.
/// @param OffsetInBits Member offset.
/// @param Flags        Flags to encode member attribute, e.g. private
/// @param Ty           Parent type.
/// @param PropertyNode Property associated with this ivar.
LLVMDIDerivedType DIBcreateObjCIVar(LLVMDIBuilderRef Builder,
                                LLVMStringRef Name,
                                LLVMDIFile File,
                                unsigned LineNo,
                                uint64_t SizeInBits,
                                uint64_t AlignInBits,
                                uint64_t OffsetInBits,
                                unsigned Flags,
                                LLVMDIType Ty,
                                LLVMMDNodePtr PropertyNode) ;

/// createObjCProperty - Create debugging information entry for Objective-C
/// property.
/// @param Name         Property name.
/// @param File         File where this property is defined.
/// @param LineNumber   Line number.
/// @param GetterName   Name of the Objective C property getter selector.
/// @param SetterName   Name of the Objective C property setter selector.
/// @param PropertyAttributes Objective C property attributes.
/// @param Ty           Type.
LLVMDIObjCProperty DIBcreateObjCProperty(LLVMDIBuilderRef Builder,
                                LLVMStringRef Name,
                                LLVMDIFile File,
                                unsigned LineNumber,
                                LLVMStringRef GetterName,
                                LLVMStringRef SetterName,
                                unsigned PropertyAttributes,
                                LLVMDIType Ty) ;

/// createClassType - Create debugging information entry for a class.
/// @param Scope        Scope in which this class is defined.
/// @param Name         class name.
/// @param File         File where this member is defined.
/// @param LineNumber   Line number.
/// @param SizeInBits   Member size.
/// @param AlignInBits  Member alignment.
/// @param OffsetInBits Member offset.
/// @param Flags        Flags to encode member attribute, e.g. private
/// @param Elements     class members.
/// @param VTableHolder Debug info of the base class that contains vtable
///                     for this type. This is used in
///                     DW_AT_containing_type. See DWARF documentation
///                     for more info.
/// @param TemplateParms Template type parameters.
/// @param UniqueIdentifier A unique identifier for the class.
LLVMDICompositeType DIBcreateClassType(LLVMDIBuilderRef Builder,
                                LLVMDIDescriptor Scope,
                                LLVMStringRef Name,
                                LLVMDIFile File,
                                unsigned LineNumber,
                                uint64_t SizeInBits,
                                uint64_t AlignInBits,
                                uint64_t OffsetInBits,
                                unsigned Flags,
                                LLVMDIType DerivedFrom,
                                LLVMDIArray Elements,
                                LLVMDIType VTableHolder,
                                LLVMMDNodePtr TemplateParms,
                                LLVMStringRef UniqueIdentifier);

/// createStructType - Create debugging information entry for a struct.
/// @param Scope        Scope in which this struct is defined.
/// @param Name         Struct name.
/// @param File         File where this member is defined.
/// @param LineNumber   Line number.
/// @param SizeInBits   Member size.
/// @param AlignInBits  Member alignment.
/// @param Flags        Flags to encode member attribute, e.g. private
/// @param Elements     Struct elements.
/// @param RunTimeLang  Optional parameter, Objective-C runtime version.
/// @param UniqueIdentifier A unique identifier for the struct.
LLVMDICompositeType DIBcreateStructType(LLVMDIBuilderRef Builder,
                                LLVMDIDescriptor Scope,
                                LLVMStringRef Name,
                                LLVMDIFile File,
                                unsigned LineNumber,
                                uint64_t SizeInBits,
                                uint64_t AlignInBits,
                                unsigned Flags,
                                LLVMDIType DerivedFrom,
                                LLVMDIArray Elements,
                                unsigned RunTimeLang,
                                LLVMDIType VTableHolder,
                                LLVMStringRef UniqueIdentifier);

/// createUnionType - Create debugging information entry for an union.
/// @param Scope        Scope in which this union is defined.
/// @param Name         Union name.
/// @param File         File where this member is defined.
/// @param LineNumber   Line number.
/// @param SizeInBits   Member size.
/// @param AlignInBits  Member alignment.
/// @param Flags        Flags to encode member attribute, e.g. private
/// @param Elements     Union elements.
/// @param RunTimeLang  Optional parameter, Objective-C runtime version.
/// @param UniqueIdentifier A unique identifier for the union.
LLVMDICompositeType DIBcreateUnionType(LLVMDIBuilderRef Builder,
                                LLVMDIDescriptor Scope,
                                LLVMStringRef Name,
                                LLVMDIFile File,
                                unsigned LineNumber,
                                uint64_t SizeInBits,
                                uint64_t AlignInBits,
                                unsigned Flags,
                                LLVMDIArray Elements,
                                unsigned RunTimeLang,
                                LLVMStringRef UniqueIdentifier);

/// createTemplateTypeParameter - Create debugging information for template
/// type parameter.
/// @param Scope        Scope in which this type is defined.
/// @param Name         Type parameter name.
/// @param Ty           Parameter type.
/// @param File         File where this type parameter is defined.
/// @param LineNo       Line number.
/// @param ColumnNo     Column Number.
LLVMDITemplateTypeParameter DIBcreateTemplateTypeParameter(LLVMDIBuilderRef Builder,
                                LLVMDIDescriptor Scope,
                                LLVMStringRef Name,
                                LLVMDIType Ty,
                                LLVMMDNodePtr File,
                                unsigned LineNo,
                                unsigned ColumnNo) ;

/// createTemplateValueParameter - Create debugging information for template
/// value parameter.
/// @param Scope        Scope in which this type is defined.
/// @param Name         Value parameter name.
/// @param Ty           Parameter type.
/// @param Val          Constant parameter value.
/// @param File         File where this type parameter is defined.
/// @param LineNo       Line number.
/// @param ColumnNo     Column Number.
LLVMDITemplateValueParameter 
DIBcreateTemplateValueParameter(LLVMDIBuilderRef Builder,
                                LLVMDIDescriptor Scope,
                                LLVMStringRef Name,
                                LLVMDIType Ty,
                                LLVMConstantRef Val,
                                LLVMMDNodePtr File,
                                unsigned LineNo,
                                unsigned ColumnNo) ;

/// \brief Create debugging information for a template template parameter.
/// @param Scope        Scope in which this type is defined.
/// @param Name         Value parameter name.
/// @param Ty           Parameter type.
/// @param Val          The fully qualified name of the template.
/// @param File         File where this type parameter is defined.
/// @param LineNo       Line number.
/// @param ColumnNo     Column Number.
LLVMDITemplateValueParameter DIBcreateTemplateTemplateParameter(LLVMDIBuilderRef Builder,
                                LLVMDIDescriptor Scope,
                                LLVMStringRef Name,
                                LLVMDIType Ty,
                                LLVMStringRef Val,
                                LLVMMDNodePtr File,
                                unsigned LineNo,
                                unsigned ColumnNo) ;

/// \brief Create debugging information for a template parameter pack.
/// @param Scope        Scope in which this type is defined.
/// @param Name         Value parameter name.
/// @param Ty           Parameter type.
/// @param Val          An array of types in the pack.
/// @param File         File where this type parameter is defined.
/// @param LineNo       Line number.
/// @param ColumnNo     Column Number.
LLVMDITemplateValueParameter DIBcreateTemplateParameterPack(LLVMDIBuilderRef Builder,
                                LLVMDIDescriptor Scope,
                                LLVMStringRef Name,
                                LLVMDIType Ty,
                                LLVMDIArray Val,
                                LLVMMDNodePtr File,
                                unsigned LineNo,
                                unsigned ColumnNo) ;

/// createArrayType - Create debugging information entry for an array.
/// @param Size         Array size.
/// @param AlignInBits  Alignment.
/// @param Ty           Element type.
/// @param Subscripts   Subscripts.
LLVMDICompositeType DIBcreateArrayType(LLVMDIBuilderRef Builder,
                                uint64_t Size,
                                uint64_t AlignInBits,
                                LLVMDIType Ty,
                                LLVMDIArray Subscripts) ;

/// createVectorType - Create debugging information entry for a vector type.
/// @param Size         Array size.
/// @param AlignInBits  Alignment.
/// @param Ty           Element type.
/// @param Subscripts   Subscripts.
LLVMDICompositeType DIBcreateVectorType(LLVMDIBuilderRef Builder,
                                uint64_t Size,
                                uint64_t AlignInBits,
                                LLVMDIType Ty,
                                LLVMDIArray Subscripts) ;

/// createEnumerationType - Create debugging information entry for an
/// enumeration.
/// @param Scope          Scope in which this enumeration is defined.
/// @param Name           Union name.
/// @param File           File where this member is defined.
/// @param LineNumber     Line number.
/// @param SizeInBits     Member size.
/// @param AlignInBits    Member alignment.
/// @param Elements       Enumeration elements.
/// @param UnderlyingType Underlying type of a C++11/ObjC fixed enum.
/// @param UniqueIdentifier A unique identifier for the enum.
LLVMDICompositeType DIBcreateEnumerationType(LLVMDIBuilderRef Builder,
                                LLVMDIDescriptor Scope,
                                LLVMStringRef Name,
                                LLVMDIFile File,
                                unsigned LineNumber,
                                uint64_t SizeInBits,
                                uint64_t AlignInBits,
                                LLVMDIArray Elements,
                                LLVMDIType UnderlyingType,
                                LLVMStringRef UniqueIdentifier);

/// createSubroutineType - Create subroutine type.
/// @param File           File in which this subroutine is defined.
/// @param ParameterTypes An array of subroutine parameter types. This
///                       includes return type at 0th index.
/// @param Flags           E.g.: LValueReference.
///                        These flags are used to emit dwarf attributes.
LLVMDICompositeType DIBcreateSubroutineType(LLVMDIBuilderRef Builder,
                                LLVMDIFile File,
                                LLVMDITypeArray ParameterTypes,
                                unsigned Flags) ;

/// createArtificialType - Create a new LLVMDIType with "artificial" flag set.
LLVMDIType DIBcreateArtificialType( LLVMDIBuilderRef Builder,
                                LLVMDIType Ty) ;

/// createObjectPointerType - Create a new LLVMDIType with the "object pointer"
/// flag set.
LLVMDIType DIBcreateObjectPointerType(LLVMDIBuilderRef Builder,
                                LLVMDIType Ty) ;

/// \brief Create a permanent forward-declared type.
LLVMDICompositeType DIBcreateForwardDecl(LLVMDIBuilderRef Builder,
                                unsigned Tag,
                                LLVMStringRef Name,
                                LLVMDIDescriptor Scope,
                                LLVMDIFile F,
                                unsigned Line,
                                unsigned RuntimeLang,
                                uint64_t SizeInBits,
                                uint64_t AlignInBits,
                                LLVMStringRef UniqueIdentifier);

/// \brief Create a temporary forward-declared type.
LLVMDICompositeType DIBcreateReplaceableForwardDecl(LLVMDIBuilderRef Builder,
                                unsigned Tag,
                                LLVMStringRef Name,
                                LLVMDIDescriptor Scope,
                                LLVMDIFile F,
                                unsigned Line,
                                unsigned RuntimeLang,
                                uint64_t SizeInBits,
                                uint64_t AlignInBits,
                                LLVMStringRef UniqueIdentifier);

/// retainType - Retain DIType in a module even if it is not referenced
/// through debug info anchors.
void DIBretainType(LLVMDIBuilderRef Builder, LLVMDIType T) ;

/// createUnspecifiedParameter - Create unspecified type descriptor
/// for a subroutine type.
LLVMDIBasicType DIBcreateUnspecifiedParameter(LLVMDIBuilderRef Builder) ;

/// getOrCreateArray - Get a DIArray, create one if required.
LLVMDIArray DIBgetOrCreateArray(LLVMDIBuilderRef Builder, 
                                LLVMArrayRefOfMetadataPtr *Elements) ;

/// getOrCreateSubrange - Create a descriptor for a value range.  This
/// implicitly uniques the values returned.
LLVMDISubrange DIBgetOrCreateSubrange(LLVMDIBuilderRef Builder,
                                int64_t Lo,
                                int64_t Count) ;

/// \brief Create a new descriptor for the specified global.
/// @param Context     Variable scope.
/// @param Name        Name of the variable.
/// @param LinkageName Mangled variable name.
/// @param File        File where this variable is defined.
/// @param LineNo      Line number.
/// @param Ty          Variable Type.
/// @param isLocalToUnit Boolean flag indicate whether this variable is
///                      externally visible or not.
/// @param Val         llvm::Value of the variable.
///                          ^What? 
/// @param Decl        Reference to the corresponding declaration.
LLVMDIGlobalVariable DIBcreateGlobalVariable
                               (LLVMDIBuilderRef Builder,
                                LLVMDIDescriptor Context,
                                LLVMStringRef Name,
                                LLVMStringRef LinkageName,
                                LLVMDIFile File,
                                unsigned LineNo,
                                LLVMDITypeRef Ty,
                                LLVMBool isLocalToUnit,
                                LLVMConstantRef Val,
                                LLVMMDNodePtr Decl) ;

/// createTempGlobalVariableFwdDecl - Identical to createGlobalVariable
/// except that the resulting DbgNode is temporary and meant to be RAUWed.
LLVMDIGlobalVariable DIBcreateTempGlobalVariableFwdDecl
                               (LLVMDIBuilderRef Builder,
                                LLVMDIDescriptor Context,
                                LLVMStringRef Name,
                                LLVMStringRef LinkageName,
                                LLVMDIFile File,
                                unsigned LineNo,
                                LLVMDITypeRef Ty,
                                LLVMBool isLocalToUnit,
                                LLVMConstantRef Val,
                                LLVMMDNodePtr Decl) ;


/// createLocalVariable - Create a new descriptor for the specified
/// local variable.
/// @param Tag         Dwarf TAG. Usually DW_TAG_auto_variable or
///                    DW_TAG_arg_variable.
/// @param Scope       Variable scope.
/// @param Name        Variable name.
/// @param File        File where this variable is defined.
/// @param LineNo      Line number.
/// @param Ty          Variable Type
/// @param AlwaysPreserve Boolean. Set to true if debug info for this
///                       variable should be preserved in optimized build.
/// @param Flags       Flags, e.g. artificial variable.
/// @param ArgNo       If this variable is an argument then this argument's
///                    number. 1 indicates 1st argument.
LLVMDIVariable DIBcreateLocalVariable(LLVMDIBuilderRef Builder,
                                unsigned Tag,
                                LLVMDIDescriptor Scope,
                                LLVMStringRef Name,
                                LLVMDIFile File,
                                unsigned LineNo,
                                LLVMDITypeRef Ty,
                                LLVMBool AlwaysPreserve,
                                unsigned Flags,
                                unsigned ArgNo) ;

/// createExpression - Create a new descriptor for the specified
/// variable which has a complex address expression for its address.
/// @param Addr        An array of complex address operations.
LLVMDIExpression DIBcreateExpression(LLVMDIBuilderRef Builder,
                                     LLVMArrayRefOfint64_t *Addr);

/// createPieceExpression - Create a descriptor to describe one part
/// of aggregate variable that is fragmented across multiple Values.
///
/// @param OffsetInBytes Offset of the piece in bytes.
/// @param SizeInBytes   Size of the piece in bytes.
LLVMDIExpression DIBcreatePieceExpression(LLVMDIBuilderRef Builder,
                                unsigned OffsetInBytes,
                                unsigned SizeInBytes);

/// createFunction - Create a new descriptor for the specified subprogram.
/// See comments in DISubprogram for descriptions of these fields.
/// @param Scope         Function scope.
/// @param Name          Function name.
/// @param LinkageName   Mangled function name.
/// @param File          File where this variable is defined.
/// @param LineNo        Line number.
/// @param Ty            Function type.
/// @param isLocalToUnit True if this function is not externally visible.
/// @param isDefinition  True if this is a function definition.
/// @param ScopeLine     Set to the beginning of the scope this starts
/// @param Flags         e.g. is this function prototyped or not.
///                      These flags are used to emit dwarf attributes.
/// @param isOptimized   True if optimization is ON.
/// @param Fn            llvm::Function pointer.
/// @param TParam        Function template parameters.
LLVMDISubprogram DIBcreateFunction( LLVMDIBuilderRef Builder,
/* ^'Tho overloaded, not renamed, because its overload 
   (DIBcreateFunctionFromScope is to be removed. */
                                LLVMDIDescriptor Scope,
                                LLVMStringRef Name,
                                LLVMStringRef LinkageName,
                                LLVMDIFile File,
                                unsigned LineNo,
                                LLVMDICompositeType Ty,
                                LLVMBool isLocalToUnit,
                                LLVMBool isDefinition,
                                unsigned ScopeLine,
                                unsigned Flags,
                                LLVMBool isOptimized,
                                LLVMValueRef Fn,
                                LLVMMDNodePtr TParam,
                                LLVMMDNodePtr Decl) ;

/// createTempFunctionFwdDecl - Identical to createFunction,
/// except that the resulting DbgNode is meant to be RAUWed.
LLVMDISubprogram DIBcreateTempFunctionFwdDecl( LLVMDIBuilderRef Builder,
                                LLVMDIDescriptor Scope,
                                LLVMStringRef Name,
                                LLVMStringRef LinkageName,
                                LLVMDIFile File,
                                unsigned LineNo,
                                LLVMDICompositeType Ty,
                                LLVMBool isLocalToUnit,
                                LLVMBool isDefinition,
                                unsigned ScopeLine,
                                unsigned Flags,
                                LLVMBool isOptimized,
                                LLVMValueRef Fn,
                                LLVMMDNodePtr TParam,
                                LLVMMDNodePtr Decl) ;

/// FIXME: this is added for dragonegg. Once we update dragonegg
/// to call resolve function, this will be removed.
LLVMDISubprogram DIBcreateFunctionFromScope(LLVMDIBuilderRef Builder,
             /* ^Renamed to eliminate overload. */
                            LLVMDIScope Scope,
                            LLVMStringRef Name,
                            LLVMStringRef LinkageName,
                            LLVMDIFile File, 
                            unsigned LineNo,
                            LLVMDICompositeType Ty, 
                            LLVMBool isLocalToUnit,
                            LLVMBool isDefinition,
                            unsigned ScopeLine,
                            unsigned Flags,
                            LLVMBool isOptimized,
                            LLVMValueRef Fn,
                            LLVMMDNodePtr TParam,
                            LLVMMDNodePtr Decl) ;

/// createMethod - Create a new descriptor for the specified C++ method.
/// See comments in LLVMDISubprogram for descriptions of these fields.
/// @param Scope         Function scope.
/// @param Name          Function name.
/// @param LinkageName   Mangled function name.
/// @param File          File where this variable is defined.
/// @param LineNo        Line number.
/// @param Ty            Function type.
/// @param isLocalToUnit True if this function is not externally visible..
/// @param isDefinition  True if this is a function definition.
/// @param Virtuality    Attributes describing virtualness. e.g. pure
///                      virtual function.
/// @param VTableIndex   Index no of this method in virtual table.
/// @param VTableHolder  Type that holds vtable.
/// @param Flags         e.g. is this function prototyped or not.
///                      This flags are used to emit dwarf attributes.
/// @param isOptimized   True if optimization is ON.
/// @param Fn            llvm::Function pointer.
/// @param TParam        Function template parameters.
LLVMDISubprogram DIBcreateMethod(LLVMDIBuilderRef Builder, 
                                LLVMDIDescriptor Scope,
                                LLVMStringRef Name,
                                LLVMStringRef LinkageName,
                                LLVMDIFile File,
                                unsigned LineNo,
                                LLVMDICompositeType Ty,
                                LLVMBool isLocalToUnit,
                                LLVMBool isDefinition,
                                unsigned Virtuality,
                                unsigned VTableIndex,
                                LLVMDIType VTableHolder,
                                unsigned Flags,
                                LLVMBool isOptimized,
                                LLVMValueRef Fn,
                                LLVMMDNodePtr TParam) ;

/// createNameSpace - This creates new descriptor for a namespace
/// with the specified parent scope.
/// @param Scope       Namespace scope
/// @param Name        Name of this namespace
/// @param File        Source file
/// @param LineNo      Line number
LLVMDINameSpace DIBcreateNameSpace(LLVMDIBuilderRef Builder,
                                LLVMDIDescriptor Scope,
                                LLVMStringRef Name,
                                LLVMDIFile File,
                                unsigned LineNo) ;

/// createLexicalBlockFile - This creates a descriptor for a lexical
/// block with a new file attached. This merely extends the existing
/// lexical block as it crosses a file.
/// @param Scope         Lexical block.
/// @param File          Source file.
/// @param Discriminator DWARF path discriminator value.
LLVMDILexicalBlockFile DIBcreateLexicalBlockFile(LLVMDIBuilderRef Builder,
                                LLVMDIDescriptor Scope,
                                LLVMDIFile File,
                                unsigned Discriminator) ;

/// createLexicalBlock - This creates a descriptor for a lexical block
/// with the specified parent context.
/// @param Scope         Parent lexical scope.
/// @param File          Source file
/// @param Line          Line number
/// @param Col           Column number
LLVMDILexicalBlock DIBcreateLexicalBlock(LLVMDIBuilderRef Builder,
                                LLVMDIDescriptor Scope,
                                LLVMDIFile File,
                                unsigned Line,
                                unsigned Col) ;

/// \brief Create a descriptor for an imported module.
/// @param Context The scope this module is imported into
/// @param NS The namespace being imported here

LLVMDIImportedEntity 
DIBcreateImportedModuleFromNamespace(LLVMDIBuilderRef Builder,
                 /* ^Renamed to eliminate overload. */
                                LLVMDIScope Context,
                                LLVMDINameSpace NS,
                                unsigned Line);

/// \brief Create a descriptor for an imported module.
/// @param Context The scope this module is imported into
/// @param NS An aliased namespace
/// @param Line Line number
LLVMDIImportedEntity 
DIBcreateImportedModuleFromImportedEntity(LLVMDIBuilderRef Builder,
                 /* ^Renamed to eliminate overload. */
                                LLVMDIScope Context,
                                LLVMDIImportedEntity NS,
                                unsigned Line);

/// \brief Create a descriptor for an imported function.
/// @param Context The scope this module is imported into
/// @param Decl The declaration (or definition) of a function, type, or
///             variable
/// @param Line Line number
/// @param Name Decl name 
LLVMDIImportedEntity 
DIBcreateImportedDeclarationFromDecl(LLVMDIBuilderRef Builder,
                                LLVMDIScope Context,
                                LLVMDIDescriptor Decl,
                                unsigned Line,
                                LLVMStringRef Name) ;

/// \brief Create a descriptor for an imported function.
/// @param Context The scope this module is imported into
/// @param Decl The declaration (or definition) of a function, type, or
///             variable
/// @param Line Line number
/// @param Name Decl name 
LLVMDIImportedEntity 
DIBcreateImportedDeclarationFromImportedEntity(LLVMDIBuilderRef Builder,
                                LLVMDIScope Context,
                                LLVMDIImportedEntity NS,
                                unsigned Line,
                                LLVMStringRef Name) ;

/// insertDeclare - Insert a new llvm.dbg.declare intrinsic call.
/// @param Storage     llvm::Value of the variable
/// @param VarInfo     Variable's debug info descriptor.
/// @param Expr         A complex location expression.
/// @param InsertAtEnd Location for the new intrinsic.
LLVMValueRef /*Instruction*/ DIBinsertDeclareAtEnd(LLVMDIBuilderRef Builder,
            /* ^Renamed to eliminate overload. */
                                LLVMValueRef Storage,
                                LLVMDIVariable VarInfo,
                                LLVMDIExpression Expr, 
                                LLVMBasicBlockRef InsertAtEnd) ;

/// insertDeclare - Insert a new llvm.dbg.declare intrinsic call.
/// @param Storage      llvm::Value of the variable
/// @param VarInfo      Variable's debug info descriptor.
/// @param Expr         A complex location expression.
/// @param InsertBefore Location for the new intrinsic.
LLVMValueRef /*Instruction*/ DIBinsertDeclareBefore(LLVMDIBuilderRef Builder,
            /* ^Renamed to eliminate overload. */
                                LLVMValueRef Storage,
                                LLVMDIVariable VarInfo,
                                LLVMDIExpression Expr, 
                                LLVMValueRef InsertBefore) ;

/// insertDbgValueIntrinsic - Insert a new llvm.dbg.value intrinsic call.
/// @param Val          llvm::Value of the variable
/// @param Offset       Offset
/// @param VarInfo      Variable's debug info descriptor.
/// @param Expr         A complex location expression.
/// @param InsertAtEnd Location for the new intrinsic.
LLVMValueRef /*Instruction*/ DIBinsertDbgValueIntrinsicAtEnd(LLVMDIBuilderRef Builder,
            /* ^Renamed to eliminate overload. */
                                LLVMValueRef Val,
                                uint64_t Offset,
                                LLVMDIVariable VarInfo,
                                LLVMDIExpression Expr, 
                                LLVMBasicBlockRef InsertAtEnd) ;

/// insertDbgValueIntrinsic - Insert a new llvm.dbg.value intrinsic call.
/// @param Val          llvm::Value of the variable
/// @param Offset       Offset
/// @param VarInfo      Variable's debug info descriptor.
/// @param Expr         A complex location expression.
/// @param InsertBefore Location for the new intrinsic.
LLVMValueRef /*Instruction*/ DIBinsertDbgValueIntrinsicBefore(LLVMDIBuilderRef Builder,
            /* ^Renamed to eliminate overload. */
                                LLVMValueRef Val,
                                uint64_t Offset,
                                LLVMDIVariable VarInfo,
                                LLVMDIExpression Expr, 
                                LLVMValueRef InsertBefore) ;

/// \brief Replace the vtable holder in the given composite type.
///
/// If this creates a self reference, it may orphan some unresolved cycles
/// in the operands of \c T, so \a DIBuilder needs to track that.
void DIBreplaceVTableHolder(LLVMDIBuilderRef Builder,
                         LLVMDICompositeType *T, 
                         LLVMDICompositeType VTableHolder);

/// \brief Replace arrays on a composite type.
///
/// If \c T is resolved, but the arrays aren't -- which can happen if \c T
/// has a self-reference -- \a DIBuilder needs to track the array to
/// resolve cycles.
void DIBreplaceArrays(LLVMDIBuilderRef Builder,
                   LLVMDICompositeType *T, 
                   LLVMDIArray Elements,
                   LLVMDIArray TParems);


// This apparently was in bindings/go/llvm/DIBuilderBindings.h of an earlier
// llvm than 3.6.1  It is in DIBuilder because that is where the stuff needed
// by its implementation is found.  
//changed scope was *
LLVMValueRef DIBgetDebugLoc(unsigned Line, 
                            unsigned Col, 
                            LLVMDIDescriptor Scope);

/// From DebugInfo.h: 
/// Replace all uses of debug info referenced by this descriptor.
void replaceAllUsesWith(LLVMDICompositeType Temp, LLVMDIDescriptor Final);

#ifdef __cplusplus
}
#endif /* !defined(__cplusplus) */

#endif /* !defined(M3DIBUILDER_H) */

//End M3DIBuilder.h 

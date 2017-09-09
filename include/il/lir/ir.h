/*
 * MIT License
 *
 * Copyright (c) 2016-2017 Abel Lucas <www.github.com/uael>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

/*!@file il/lir/ir.h
 * @author uael
 */
#ifndef __IL_LIR_IR_H
# define __IL_LIR_IR_H

#define vecof(T) struct { T *buf; unsigned long len; }
#define setof(T) struct { T *buf; unsigned long len; }
#define mapof(K, T) struct { T *buf; unsigned long len; }

enum il_lir_select {
  IL_LIR_SELECT_ANY = 0,
  IL_LIR_SELECT_EXACTMATCH,
  IL_LIR_SELECT_LARGEST,
  IL_LIR_SELECT_NODUPLICATES,
  IL_LIR_SELECT_SAMESIZE
};

enum il_lir_linkage {
  IL_LIR_LINKAGE_APPENDING = 0,
  IL_LIR_LINKAGE_AVAILABLE_EXTERNALLY,
  IL_LIR_LINKAGE_COMMON,
  IL_LIR_LINKAGE_INTERNAL,
  IL_LIR_LINKAGE_LINKONCE,
  IL_LIR_LINKAGE_LINKONCE_ODR,
  IL_LIR_LINKAGE_PRIVATE,
  IL_LIR_LINKAGE_WEAK,
  IL_LIR_LINKAGE_WEAK_ODR
};

enum il_lir_extlinkage {
  IL_LIR_EXTLINKAGE_EXTERN_WEAK = 0,
  IL_LIR_EXTLINKAGE_EXTERNAL
};

enum il_lir_visibility {
  IL_LIR_VISIBILITY_DEFAULT = 0,
  IL_LIR_VISIBILITY_HIDDEN,
  IL_LIR_VISIBILITY_PROTECTED
};

enum il_lir_dllstorage {
  IL_LIR_DLLSTORAGE_DLLIMPORT = 0,
  IL_LIR_DLLSTORAGE_DLLEXPORT
};

enum il_lir_tlsmodel {
  IL_LIR_LOCALDYNAMIC = 0,
  IL_LIR_INITIALEXEC,
  IL_LIR_LOCALEXEC
};

enum il_lir_paramattr {
  IL_LIR_PARAMATTR_STRING = 0,
  IL_LIR_PARAMATTR_ASSIGN,
  IL_LIR_PARAMATTR_ALIGN,
  IL_LIR_PARAMATTR_BYVAL,
  IL_LIR_PARAMATTR_DEREFERENCEABLE,
  IL_LIR_PARAMATTR_DEREFERENCEABLE_OR_NULL,
  IL_LIR_PARAMATTR_INALLOCA,
  IL_LIR_PARAMATTR_INREG,
  IL_LIR_PARAMATTR_NEST,
  IL_LIR_PARAMATTR_NOALIAS,
  IL_LIR_PARAMATTR_NOCAPTURE,
  IL_LIR_PARAMATTR_NONNULL,
  IL_LIR_PARAMATTR_READNONE,
  IL_LIR_PARAMATTR_READONLY,
  IL_LIR_PARAMATTR_RETURNED,
  IL_LIR_PARAMATTR_SIGNEXT,
  IL_LIR_PARAMATTR_SRET,
  IL_LIR_PARAMATTR_SWIFTERROR,
  IL_LIR_PARAMATTR_SWIFTSELF,
  IL_LIR_PARAMATTR_WRITEONLY,
  IL_LIR_PARAMATTR_ZEROEXT
};

enum il_lir_funcattr {
  IL_LIR_FUNCATTR_STRING = 0,
  IL_LIR_FUNCATTR_ASSIGN,
  IL_LIR_FUNCATTR_ID,
  IL_LIR_FUNCATTR_ALIGNSTACK,
  IL_LIR_FUNCATTR_ALLOCSIZE,
  IL_LIR_FUNCATTR_ALWAYSINLINE,
  IL_LIR_FUNCATTR_ARGMEMONLY,
  IL_LIR_FUNCATTR_BUILTIN,
  IL_LIR_FUNCATTR_COLD,
  IL_LIR_FUNCATTR_CONVERGENT,
  IL_LIR_FUNCATTR_INACCESSIBLEMEM_OR_ARGMEMONLY,
  IL_LIR_FUNCATTR_INACCESSIBLEMEMONLY,
  IL_LIR_FUNCATTR_INLINEHINT,
  IL_LIR_FUNCATTR_JUMPTABLE,
  IL_LIR_FUNCATTR_MINSIZE,
  IL_LIR_FUNCATTR_NAKED,
  IL_LIR_FUNCATTR_NOBUILTIN,
  IL_LIR_FUNCATTR_NODUPLICATE,
  IL_LIR_FUNCATTR_NOIMPLICITFLOAT,
  IL_LIR_FUNCATTR_NOINLINE,
  IL_LIR_FUNCATTR_NONLAZYBIND,
  IL_LIR_FUNCATTR_NORECURSE,
  IL_LIR_FUNCATTR_NOREDZONE,
  IL_LIR_FUNCATTR_NORETURN,
  IL_LIR_FUNCATTR_NOUNWIND,
  IL_LIR_FUNCATTR_OPTNONE,
  IL_LIR_FUNCATTR_OPTSIZE,
  IL_LIR_FUNCATTR_READNONE,
  IL_LIR_FUNCATTR_READONLY,
  IL_LIR_FUNCATTR_RETURNS_TWICE,
  IL_LIR_FUNCATTR_SAFESTACK,
  IL_LIR_FUNCATTR_SANITIZE_ADDRESS,
  IL_LIR_FUNCATTR_SANITIZE_MEMORY,
  IL_LIR_FUNCATTR_SANITIZE_THREAD,
  IL_LIR_FUNCATTR_SSP,
  IL_LIR_FUNCATTR_SSPREQ,
  IL_LIR_FUNCATTR_SSPSTRONG,
  IL_LIR_FUNCATTR_UWTABLE,
  IL_LIR_FUNCATTR_WRITEONLY
};

enum il_lir_callconv {
  IL_LIR_CALLCONV_AMDGPU_CS,
  IL_LIR_CALLCONV_AMDGPU_GS,
  IL_LIR_CALLCONV_AMDGPU_KERNEL,
  IL_LIR_CALLCONV_AMDGPU_PS,
  IL_LIR_CALLCONV_AMDGPU_VS,
  IL_LIR_CALLCONV_ANYREGCC,
  IL_LIR_CALLCONV_ARM_AAPCS_VFPCC,
  IL_LIR_CALLCONV_ARM_AAPCSCC,
  IL_LIR_CALLCONV_ARM_APCSCC,
  IL_LIR_CALLCONV_AVR_INTRCC,
  IL_LIR_CALLCONV_AVR_SIGNALCC,
  IL_LIR_CALLCONV_CC,
  IL_LIR_CALLCONV_CCC,
  IL_LIR_CALLCONV_COLDCC,
  IL_LIR_CALLCONV_CXX_FAST_TLSCC,
  IL_LIR_CALLCONV_FASTCC,
  IL_LIR_CALLCONV_GHCCC,
  IL_LIR_CALLCONV_HHVM_CCC,
  IL_LIR_CALLCONV_HHVMCC,
  IL_LIR_CALLCONV_INTEL_OCL_BICC,
  IL_LIR_CALLCONV_MSP430_INTRCC,
  IL_LIR_CALLCONV_PRESERVE_ALLCC,
  IL_LIR_CALLCONV_PRESERVE_MOSTCC,
  IL_LIR_CALLCONV_PTX_DEVICE,
  IL_LIR_CALLCONV_PTX_KERNEL,
  IL_LIR_CALLCONV_SPIR_FUNC,
  IL_LIR_CALLCONV_SPIR_KERNEL,
  IL_LIR_CALLCONV_SWIFTCC,
  IL_LIR_CALLCONV_WEBKIT_JSCC,
  IL_LIR_CALLCONV_X86_64_SYSVCC,
  IL_LIR_CALLCONV_X86_64_WIN64CC,
  IL_LIR_CALLCONV_X86_FASTCALLCC,
  IL_LIR_CALLCONV_X86_INTRCC,
  IL_LIR_CALLCONV_X86_REGCALLCC,
  IL_LIR_CALLCONV_X86_STDCALLCC,
  IL_LIR_CALLCONV_X86_THISCALLCC,
  IL_LIR_CALLCONV_X86_VECTORCALLCC,
};
/*
struct il_lir_id {

};

struct il_lir_lit {

};

struct il_lir_type {

};

struct il_lir_module {

};

struct il_lir_source {

};

struct il_lir_target {

};

struct il_lir_asmmodule {

};

struct il_lir_typedef {

};

struct il_lir_comdat {

};

struct il_lir_global {

};

struct il_lir_globaldef {

};

struct il_lir_func {

};

struct il_lir_param {

};

struct il_lir_funcdef {

};

struct il_lir_attrgroup {

};

struct il_lir_metadata {

};

struct il_lir_block {

};

struct il_lir_instr {

};

struct il_lir_storeinstr {

};

struct il_lir_fenceinstr {

};*/

#endif /* !__IL_LIR_IR_H */
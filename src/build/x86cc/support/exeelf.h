/****************************************************************************
*
*                            Open Watcom Project
*
* Copyright (c) 2002-2024 The Open Watcom Contributors. All Rights Reserved.
*    Portions Copyright (c) 1983-2002 Sybase, Inc. All Rights Reserved.
*
*  ========================================================================
*
*    This file contains Original Code and/or Modifications of Original
*    Code as defined in and that are subject to the Sybase Open Watcom
*    Public License version 1.0 (the 'License'). You may not use this file
*    except in compliance with the License. BY USING THIS FILE YOU AGREE TO
*    ALL TERMS AND CONDITIONS OF THE LICENSE. A copy of the License is
*    provided with the Original Code and Modifications, and is also
*    available at www.sybase.com/developer/opensource.
*
*    The Original Code and all software distributed under the License are
*    distributed on an 'AS IS' basis, WITHOUT WARRANTY OF ANY KIND, EITHER
*    EXPRESS OR IMPLIED, AND SYBASE AND ALL CONTRIBUTORS HEREBY DISCLAIM
*    ALL SUCH WARRANTIES, INCLUDING WITHOUT LIMITATION, ANY WARRANTIES OF
*    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE, QUIET ENJOYMENT OR
*    NON-INFRINGEMENT. Please see the License for the specific language
*    governing rights and limitations under the License.
*
*  ========================================================================
*
* Description:  Executable and Linkable Format (ELF) definitions.
*
****************************************************************************/


#ifndef _EXEELF_H_INCLUDED_
#define _EXEELF_H_INCLUDED_

#include <stdint.h>
#include "watcom.h"

// ELF scalar data types

typedef unsigned_32     Elf32_Addr;
typedef unsigned_16     Elf32_Half;
typedef unsigned_32     Elf32_Off;
typedef signed_32       Elf32_Sword;
typedef unsigned_32     Elf32_Word;

typedef unsigned_64     Elf64_Addr;
typedef uint16_t        Elf64_Half;
typedef unsigned_64     Elf64_Off;
typedef int32_t         Elf64_Sword;
typedef signed_64       Elf64_Sxword;
typedef uint32_t        Elf64_Word;
typedef unsigned_64     Elf64_Xword;

// the main header

#define EI_NIDENT       16

typedef struct {
    unsigned_8  e_ident[EI_NIDENT];     // signature & ID info
    Elf32_Half  e_type;                 // file type (i.e. obj file, exe file)
    Elf32_Half  e_machine;              // required architecture
    Elf32_Word  e_version;              // version of the file
    Elf32_Addr  e_entry;                // program entry point
    Elf32_Off   e_phoff;                // program header offset
    Elf32_Off   e_shoff;                // section header offset
    Elf32_Word  e_flags;                // processor specific flags
    Elf32_Half  e_ehsize;               // elf header size
    Elf32_Half  e_phentsize;            // program header entry size
    Elf32_Half  e_phnum;                // number of program header entries
    Elf32_Half  e_shentsize;            // section header entry size
    Elf32_Half  e_shnum;                // number of section header entries
    Elf32_Half  e_shstrndx;             // section name string table index.
// Obselete
/*    Elf32_Half        e_shosndx;              // OS info section index
    Elf32_Half  e_pad;                  // to keep header size a word multiple*/
} Elf32_Ehdr;

// ELF-64 header: identical to Elf32_Ehdr up to and including e_version
typedef struct {
    unsigned_8  e_ident[EI_NIDENT];     // signature & ID info
    Elf64_Half  e_type;                 // file type (i.e. obj file, exe file)
    Elf64_Half  e_machine;              // required architecture
    Elf64_Word  e_version;              // version of the file
    Elf64_Addr  e_entry;                // program entry point
    Elf64_Off   e_phoff;                // program header offset
    Elf64_Off   e_shoff;                // section header offset
    Elf64_Word  e_flags;                // processor specific flags
    Elf64_Half  e_ehsize;               // elf header size
    Elf64_Half  e_phentsize;            // program header entry size
    Elf64_Half  e_phnum;                // number of program header entries
    Elf64_Half  e_shentsize;            // section header entry size
    Elf64_Half  e_shnum;                // number of section header entries
    Elf64_Half  e_shstrndx;             // section name string table index.
} Elf64_Ehdr;

// e_ident field indicies

#define ELF_SIGNATURE_LEN 4

#define ELFMAG0         '\177'  // bytes of signature
#define ELFMAG1         'E'     // bytes of signature
#define ELFMAG2         'L'     // bytes of signature
#define ELFMAG3         'F'     // bytes of signature
#define EI_MAG0         0       // signature bytes
#define EI_MAG1         1       // signature bytes
#define EI_MAG2         2       // signature bytes
#define EI_MAG3         3       // signature bytes
#define EI_CLASS        4       // "file class", i.e. 32-bit vs. 64-bit
#define EI_DATA         5       // data encoding (big-endian vs. little-endian)
#define EI_VERSION      6       // header version #
#define EI_OSABI        7       // OS/ABI identification
#define EI_ABIVERSION   8       // ABI version

// contents of the EI_CLASS field index

#define ELFCLASS32      1       // 32-bit objects

// contents of the EI_DATA field index

#define ELFDATA2LSB     1       // "little-endian"

// the current elf version number (EI_VERSION)

#define EV_CURRENT      1


// contents of the EI_OSABI field index

#define ELFOSABI_86OPEN     5   // 86Open common IA32 ABI


// elf object file types

#define ET_REL          1       // relocatable file
#define ET_DYN          3       // shared object file

// elf machine types

#define EM_M32             1    // AT&T WE 32100
#define EM_386             3    // Intel 80386
// was  EM_486             6    // Reserved
#define EM_MIPS            8    // MIPS I Architecture
//                        11-14 // Reserved
//                        16    // Reserved
#define EM_PPC            20    // PowerPC
//                        23-35 // Reserved
#define EM_ARM            40    // Advanced RISC Machines ARM
#define EM_ALPHA          41    // Digital Alpha
#define EM_NCPU           56    // Sony nCPU embedded RISC processor
#define EM_X86_64         62    // AMD x86-64 architecture
#define EM_JAVELIN        77    // Infineon Technologies 32-bit embedded processor
#define EM_BLACKFIN      106    // Analog Devices Blackfin (DSP) processor
#define EM_TI_PRU        144    // Texas Instruments Programmable Realtime Unit
#define EM_ETPU          178    // Freescale Extended Time Processing Unit
#define EM_AARCH64       183    // ARM 64-bit architecture (AARCH64)
//                       205-209// Reserved by Intel
#define EM_AMDGPU        224    // AMD GPU architecture


// version number info

#define EV_CURRENT      1

// flags (machine specific)
//EM_M32
#define EF_M32_MAU      0x1

// special section indicies

#define SHN_UNDEF       0
#define SHN_COMMON      0xfff2  // references to this section are common

// section header

typedef struct {
    Elf32_Word  sh_name;        // name of the section
    Elf32_Word  sh_type;        // section type
    Elf32_Word  sh_flags;
    Elf32_Addr  sh_addr;        // starting address of section in image
    Elf32_Off   sh_offset;      // start of section in file
    Elf32_Word  sh_size;        // size of section in file.
    Elf32_Word  sh_link;        // multipurpose field   (based on type)
    Elf32_Word  sh_info;        // another multipurpose field (based on type)
    Elf32_Word  sh_addralign;   // address alignment
    Elf32_Word  sh_entsize;     // entry size for sects with fixed sized entries
} Elf32_Shdr;

typedef struct {
    Elf64_Word  sh_name;        // name of the section
    Elf64_Word  sh_type;        // section type
    Elf64_Xword sh_flags;
    Elf64_Addr  sh_addr;        // starting address of section in image
    Elf64_Off   sh_offset;      // start of section in file
    Elf64_Xword sh_size;        // size of section in file.
    Elf64_Word  sh_link;        // multipurpose field   (based on type)
    Elf64_Word  sh_info;        // another multipurpose field (based on type)
    Elf64_Xword sh_addralign;   // address alignment
    Elf64_Xword sh_entsize;     // entry size for sects with fixed sized entries
} Elf64_Shdr;

// section types

#define SHT_NULL                     0  // inactive
#define SHT_PROGBITS                 1  // meaning defined by program
#define SHT_SYMTAB                   2  // symbol table
#define SHT_STRTAB                   3  // string table
#define SHT_RELA                     4  // reloc entries with explicit addends
#define SHT_NOTE                     7  // comment information
#define SHT_NOBITS                   8  // like PROGBITS but no space in file.
#define SHT_REL                      9  // as RELA but no explicit addends
#define SHT_SUNW_symsort    0x6ffffff1
#define SHT_SUNW_tlssort    0x6ffffff2
#define SHT_SUNW_move       0x6ffffffa

// Old section types.  Readers should handle these, writers must use the above


// sh_flags values

#define SHF_WRITE            0x00000001 // section writable during execution
#define SHF_ALLOC            0x00000002 // section occupies space during exec
#define SHF_EXECINSTR        0x00000004 // section contains code

#define SHF_BEGIN            0x01000000 // section to be placed at the beginning
                                        // of like-named sections by static link
#define SHF_END              0x02000000 // same, end.

                                        // referenced or allocated

// symbol table entry

typedef struct {
    Elf32_Word  st_name;        // symbol name index into string table
    Elf32_Addr  st_value;       // symbol "value"
    Elf32_Word  st_size;        // symbol size
    unsigned_8  st_info;        // symbol's type and binding attribs.
    unsigned_8  st_other;       // no meaning yet.
    Elf32_Half  st_shndx;       // section index
//  ToDo: Field is only available for arm and arm64 targets
//  uint32_t    st_arch_subinfo;// Needed for arm function info (mach-o)
} Elf32_Sym;

typedef struct {
    Elf64_Word  st_name;        // symbol name index into string table
    unsigned_8  st_info;        // symbol's type and binding attribs.
    unsigned_8  st_other;       // no meaning yet.
    Elf64_Half  st_shndx;       // section index
    Elf64_Addr  st_value;       // symbol "value"
    Elf64_Xword st_size;        // symbol size
//  ToDo: Field is only available for arm and arm64 targets
//  uint32_t    st_arch_subinfo;// Needed for arm function info (mach-o)
} Elf64_Sym;

// symbol info field contents

inline int Elf32StInfo( unsigned short b, unsigned short t ) { return (((b)<<4)+((t)&0x0f)); }   // make a new st_info


// bind subfield contents

#define STB_LOCAL       0       // symbol has local binding
#define STB_GLOBAL      1       // symbol has global binding
#define STB_WEAK        2       // symbol has weak binding

// type subfield contents

#define STT_OBJECT      1       // symbol is a data object
#define STT_FUNC        2       // symbol is a code symbol
#define STT_SECTION     3       // symbol associated with a section
#define STT_FILE        4       // symbol gives name of the source file.
#define STT_COMMON      5       // symbol labels an uninitialized common block

// relocation entries

typedef struct {
    Elf32_Addr  r_offset;       // place to apply reloc (from begin of section)
    Elf32_Word  r_info;         // symbol idx, and type of reloc
} Elf32_Rel;

typedef struct {
    Elf32_Addr  r_offset;       // place to apply reloc (from begin of section)
    Elf32_Word  r_info;         // symbol idx, and type of reloc
    Elf32_Sword r_addend;       // value used as a basis for the reloc.
} Elf32_Rela;

typedef struct {
    Elf64_Addr  r_offset;       // place to apply reloc (from begin of section)
    Elf64_Xword r_info;         // symbol idx, and type of reloc
} Elf64_Rel;

typedef struct {
    Elf64_Addr  r_offset;       // place to apply reloc (from begin of section)
    Elf64_Xword r_info;         // symbol idx, and type of reloc
    Elf64_Sxword r_addend;      // value used as a basis for the reloc.
} Elf64_Rela;

// r_info field contents

inline unsigned int Elf32RInfo( unsigned int s, unsigned int t ) { return (((s)<<8)+(unsigned_8)(t)); }   // make a new r_info


// relocation types.
//386
#define R_386_NONE              0
#define R_386_32                1
#define R_386_PC32              2
#define R_386_GOT32             3

//X86_64

//PPC
#define R_PPC_NONE              0
#define R_PPC_ADDR32            1
#define R_PPC_ADDR24            2
#define R_PPC_ADDR16_LO         4
#define R_PPC_ADDR16_HI         5
#define R_PPC_ADDR16_HA         6
#define R_PPC_ADDR14            7
#define R_PPC_ADDR14_BRTAKEN    8
#define R_PPC_ADDR14_BRNTAKEN   9
#define R_PPC_REL24             10
#define R_PPC_REL14             11
#define R_PPC_REL14_BRTAKEN     12
#define R_PPC_REL14_BRNTAKEN    13
#define R_PPC_GOT16             14

//M32

//SPARC
#define R_SPARC_7               43
#define R_SPARC_5               44
#define R_SPARC_6               45

// MIPS
#define R_MIPS_NONE             0
#define R_MIPS_32               2
#define R_MIPS_26               4
#define R_MIPS_HI16             5
#define R_MIPS_LO16             6
#define R_MIPS_GOT16            9
#define R_MIPS_PC16             10

// Alpha
#define R_ALPHA_NONE            0
#define R_ALPHA_REFLONG         1
#define R_ALPHA_BRADDR          7
#define R_ALPHA_SREL16          9
#define R_ALPHA_GPRELHIGH       17
#define R_ALPHA_GPRELLOW        18
#define R_ALPHA_GPREL16         19

// program header

typedef struct {
    Elf32_Word  p_type;         // type of segment
    Elf32_Off   p_offset;       // offset of segment from beginnning of file
    Elf32_Addr  p_vaddr;        // segment virtual address
    Elf32_Addr  p_paddr;        // segment physical address
    Elf32_Word  p_filesz;       // size of segment in file
    Elf32_Word  p_memsz;        // size of segment in memory
    Elf32_Word  p_flags;        // segment flags
    Elf32_Word  p_align;        // segment align value (in mem & file)
} Elf32_Phdr;

typedef struct {
    Elf64_Word  p_type;         // type of segment
    Elf64_Word  p_flags;        // segment flags
    Elf64_Off   p_offset;       // offset of segment from beginnning of file
    Elf64_Addr  p_vaddr;        // segment virtual address
    Elf64_Addr  p_paddr;        // segment physical address
    Elf64_Xword p_filesz;       // size of segment in file
    Elf64_Xword p_memsz;        // size of segment in memory
    Elf64_Xword p_align;        // segment align value (in mem & file)
} Elf64_Phdr;

// segment types


// Old segment types.  Readers should handle these, writers must use the above


// elf segment flag bits


// note entry format

typedef struct {
    unsigned_32     n_namesz;   // length of name
    unsigned_32     n_descsz;   // length of descriptor
    unsigned_32     n_type;     // user defined "type" of the note
    //char            name[];   // variable length name
    //unsigned_32     desc[];   // descriptors go here
} Elf_Note;

// note types (used in core files)


// dynamic segment entry information.

typedef struct {
    Elf32_Sword         d_tag;
    union {
        Elf32_Word      d_val;
        Elf32_Addr      d_ptr;
    } d_un;
} Elf32_Dyn;

// dynamic array tags

#define DT_NEEDED       1               // name of a needed library
#define DT_IT           0x60000007      // init and term types for a DLL.
#define DT_INITTERM     0x60000009      // address of init and term function

// Old dynamic tags.  Readers should handle these, writers must use the above

                                        // Ha Ha Ha!

// description of DT_IT tag:
// Describe type for initalization and termination of DLL
// Required if DT_INIT and DT_FINI also specified



// DT_INITTERM function prototype

typedef unsigned long INITTERM( unsigned long modhandle, unsigned long flag );

// operating system information

typedef struct {
    Elf32_Word  os_type;
    Elf32_Word  os_size;
} Elf32_Os;

#define EOS_PN          1       // IBM Microkernel personality neutral

// OS/2-specific information

typedef struct {
    unsigned char       os2_sessiontype;
    unsigned char       os2_sessionflags;
    unsigned char       os2_reserved[14];
} Elf32_OS2Info;

// os2_sessiontype values


// import table entry

typedef struct {
    Elf32_Word  imp_ordinal;
    Elf32_Word  imp_name;
    Elf32_Word  imp_info;
    Elf32_Word  imp_reserved;
} Elf32_Import;


#define IMP_STR_IDX     1       // Value is string table index to load module
#define IMP_DT_IDX      2       // Value is ref to DT_NEEDED in Dynamic Segment

// export table entry

typedef struct {
    Elf32_Word  exp_ordinal;
    Elf32_Word  exp_symbol;
    Elf32_Word  exp_name;
    Elf32_Word  exp_reserved;
} Elf32_Export;

// Resource header

#define RH_NIDENT       16

typedef struct {
    unsigned char       rh_ident[RH_NIDENT];
    Elf32_Off           rh_name;
    Elf32_Word          rh_itnum;
    Elf32_Word          rh_rhsize;
    Elf32_Word          rh_size;
    Elf32_Off           rh_strtab;
} Elf32_Rhdr;

// rh_ident field indices

#define RH_CLASS        4       // class of resource collection
#define RH_DATA         5       // data encoding of resource collection
#define RH_VERSION      6       // version

// contents of RH_CLASS field


// contents of RH_DATA field


// contents of RH_VERSION field


// resource item

#define RI_NIDENT       4

typedef struct {
    unsigned char       ri_ident[RI_NIDENT];
    Elf32_Word          ri_type;
    Elf32_Off           ri_typename;
    Elf32_Word          ri_ordinal;
    Elf32_Off           ri_ordname;
    Elf32_Off           ri_data;
    Elf32_Word          ri_flags;
    Elf32_Word          ri_size;
    Elf32_Word          ri_reserved;
} Elf32_Ritem;

// ri_ident field indices

#define RI_VERSION      0       // version

// contents of RI_VERSION field


// demangle information structure

typedef struct {
    Elf32_Word  idm_dllname;
    Elf32_Word  idm_initparms;
} Elf32_Demangle;

// default library structure

typedef struct {
    Elf32_Word  lib_name;
} Elf32_Library;


// typedefs/structs/unions and macros to better manage Elf32 / Elf64 differences

typedef union {
    unsigned_8  e_ident[EI_NIDENT];
    Elf32_Ehdr  elf32;
    Elf64_Ehdr  elf64;
} Elfxx_Ehdr;



#endif // _EXEELF_H_INCLUDED_

// java/decompiler/code_constants.hpp -- the decompiler's constants: class-file versions, the value types and their families, access flags, constant
// pool tags, method-handle kinds and the JVM opcodes, under the names the decompiler's passes use.
//
// Ported from Fernflower (de.fernflower.code.CodeConstants), Copyright 2000-2015 JetBrains s.r.o., Apache License 2.0 (see NOTICE in this folder).
#pragma once

#include <string_view>

namespace cb::java::decompiler::cc {

// BYTECODE VERSIONS

inline constexpr int BYTECODE_JAVA_LE_4 = 1;
inline constexpr int BYTECODE_JAVA_5 = 2;
inline constexpr int BYTECODE_JAVA_6 = 3;
inline constexpr int BYTECODE_JAVA_7 = 4;
inline constexpr int BYTECODE_JAVA_8 = 5;

// VARIABLE TYPES

inline constexpr int TYPE_BYTE = 0;
inline constexpr int TYPE_CHAR = 1;
inline constexpr int TYPE_DOUBLE = 2;
inline constexpr int TYPE_FLOAT = 3;
inline constexpr int TYPE_INT = 4;
inline constexpr int TYPE_LONG = 5;
inline constexpr int TYPE_SHORT = 6;
inline constexpr int TYPE_BOOLEAN = 7;
inline constexpr int TYPE_OBJECT = 8;
inline constexpr int TYPE_ADDRESS = 9;
inline constexpr int TYPE_VOID = 10;
inline constexpr int TYPE_ANY = 11;
inline constexpr int TYPE_GROUP2EMPTY = 12;
inline constexpr int TYPE_NULL = 13;
inline constexpr int TYPE_NOTINITIALIZED = 14;
inline constexpr int TYPE_BYTECHAR = 15;
inline constexpr int TYPE_SHORTCHAR = 16;
inline constexpr int TYPE_UNKNOWN = 17;
inline constexpr int TYPE_GENVAR = 18;

// VARIABLE TYPE FAMILIES

inline constexpr int TYPE_FAMILY_UNKNOWN = 0;
inline constexpr int TYPE_FAMILY_BOOLEAN = 1;
inline constexpr int TYPE_FAMILY_INTEGER = 2;
inline constexpr int TYPE_FAMILY_FLOAT = 3;
inline constexpr int TYPE_FAMILY_LONG = 4;
inline constexpr int TYPE_FAMILY_DOUBLE = 5;
inline constexpr int TYPE_FAMILY_OBJECT = 6;

// MODULE CONSTANTS

inline constexpr int STACKSIZE_SIMPLE = 1;
inline constexpr int STACKSIZE_DOUBLE = 2;

inline constexpr int VAR_LOCAL = 0;
inline constexpr int VAR_STACK = 1;

inline constexpr int VAR_WRITE = 0;
inline constexpr int VAR_READ = 1;

// ACCESS FLAGS

inline constexpr int ACC_PUBLIC = 0x0001;
inline constexpr int ACC_PRIVATE = 0x0002;
inline constexpr int ACC_PROTECTED = 0x0004;
inline constexpr int ACC_STATIC = 0x0008;
inline constexpr int ACC_FINAL = 0x0010;
inline constexpr int ACC_SYNCHRONIZED = 0x0020;
inline constexpr int ACC_NATIVE = 0x0100;
inline constexpr int ACC_ABSTRACT = 0x0400;
inline constexpr int ACC_STRICT = 0x0800;
inline constexpr int ACC_VOLATILE = 0x0040;
inline constexpr int ACC_BRIDGE = 0x0040;
inline constexpr int ACC_TRANSIENT = 0x0080;
inline constexpr int ACC_VARARGS = 0x0080;
inline constexpr int ACC_SYNTHETIC = 0x1000;
inline constexpr int ACC_ANNOTATION = 0x2000;
inline constexpr int ACC_ENUM = 0x4000;

// CLASS FLAGS

inline constexpr int ACC_SUPER = 0x0020;
inline constexpr int ACC_INTERFACE = 0x0200;

// DEPENDENCY CONSTANTS

inline constexpr int DEP_CONSTANT = 0;
inline constexpr int DEP_UNKNOWN = 1;
inline constexpr int DEP_GENERAL = 2;
inline constexpr int DEP_PARAMS = 4;
inline constexpr int DEP_STATIC = 8;

// INSTRUCTION GROUPS

inline constexpr int GROUP_GENERAL = 1;
inline constexpr int GROUP_JUMP = 2;
inline constexpr int GROUP_SWITCH = 3;
inline constexpr int GROUP_INVOCATION = 4;
inline constexpr int GROUP_FIELDACCESS = 5;
inline constexpr int GROUP_RETURN = 6;

// POOL CONSTANTS

inline constexpr int CONSTANT_Utf8 = 1;
inline constexpr int CONSTANT_Integer = 3;
inline constexpr int CONSTANT_Float = 4;
inline constexpr int CONSTANT_Long = 5;
inline constexpr int CONSTANT_Double = 6;
inline constexpr int CONSTANT_Class = 7;
inline constexpr int CONSTANT_String = 8;
inline constexpr int CONSTANT_Fieldref = 9;
inline constexpr int CONSTANT_Methodref = 10;
inline constexpr int CONSTANT_InterfaceMethodref = 11;
inline constexpr int CONSTANT_NameAndType = 12;
inline constexpr int CONSTANT_MethodHandle = 15;
inline constexpr int CONSTANT_MethodType = 16;
inline constexpr int CONSTANT_InvokeDynamic = 18;

// MethodHandle reference_kind values

inline constexpr int CONSTANT_MethodHandle_REF_getField = 1;
inline constexpr int CONSTANT_MethodHandle_REF_getStatic = 2;
inline constexpr int CONSTANT_MethodHandle_REF_putField = 3;
inline constexpr int CONSTANT_MethodHandle_REF_putStatic = 4;
inline constexpr int CONSTANT_MethodHandle_REF_invokeVirtual = 5;
inline constexpr int CONSTANT_MethodHandle_REF_invokeStatic = 6;
inline constexpr int CONSTANT_MethodHandle_REF_invokeSpecial = 7;
inline constexpr int CONSTANT_MethodHandle_REF_newInvokeSpecial = 8;
inline constexpr int CONSTANT_MethodHandle_REF_invokeInterface = 9;

// VM OPCODES

inline constexpr int opc_nop = 0;
inline constexpr int opc_aconst_null = 1;
inline constexpr int opc_iconst_m1 = 2;
inline constexpr int opc_iconst_0 = 3;
inline constexpr int opc_iconst_1 = 4;
inline constexpr int opc_iconst_2 = 5;
inline constexpr int opc_iconst_3 = 6;
inline constexpr int opc_iconst_4 = 7;
inline constexpr int opc_iconst_5 = 8;
inline constexpr int opc_lconst_0 = 9;
inline constexpr int opc_lconst_1 = 10;
inline constexpr int opc_fconst_0 = 11;
inline constexpr int opc_fconst_1 = 12;
inline constexpr int opc_fconst_2 = 13;
inline constexpr int opc_dconst_0 = 14;
inline constexpr int opc_dconst_1 = 15;
inline constexpr int opc_bipush = 16;
inline constexpr int opc_sipush = 17;
inline constexpr int opc_ldc = 18;
inline constexpr int opc_ldc_w = 19;
inline constexpr int opc_ldc2_w = 20;
inline constexpr int opc_iload = 21;
inline constexpr int opc_lload = 22;
inline constexpr int opc_fload = 23;
inline constexpr int opc_dload = 24;
inline constexpr int opc_aload = 25;
inline constexpr int opc_iload_0 = 26;
inline constexpr int opc_iload_1 = 27;
inline constexpr int opc_iload_2 = 28;
inline constexpr int opc_iload_3 = 29;
inline constexpr int opc_lload_0 = 30;
inline constexpr int opc_lload_1 = 31;
inline constexpr int opc_lload_2 = 32;
inline constexpr int opc_lload_3 = 33;
inline constexpr int opc_fload_0 = 34;
inline constexpr int opc_fload_1 = 35;
inline constexpr int opc_fload_2 = 36;
inline constexpr int opc_fload_3 = 37;
inline constexpr int opc_dload_0 = 38;
inline constexpr int opc_dload_1 = 39;
inline constexpr int opc_dload_2 = 40;
inline constexpr int opc_dload_3 = 41;
inline constexpr int opc_aload_0 = 42;
inline constexpr int opc_aload_1 = 43;
inline constexpr int opc_aload_2 = 44;
inline constexpr int opc_aload_3 = 45;
inline constexpr int opc_iaload = 46;
inline constexpr int opc_laload = 47;
inline constexpr int opc_faload = 48;
inline constexpr int opc_daload = 49;
inline constexpr int opc_aaload = 50;
inline constexpr int opc_baload = 51;
inline constexpr int opc_caload = 52;
inline constexpr int opc_saload = 53;
inline constexpr int opc_istore = 54;
inline constexpr int opc_lstore = 55;
inline constexpr int opc_fstore = 56;
inline constexpr int opc_dstore = 57;
inline constexpr int opc_astore = 58;
inline constexpr int opc_istore_0 = 59;
inline constexpr int opc_istore_1 = 60;
inline constexpr int opc_istore_2 = 61;
inline constexpr int opc_istore_3 = 62;
inline constexpr int opc_lstore_0 = 63;
inline constexpr int opc_lstore_1 = 64;
inline constexpr int opc_lstore_2 = 65;
inline constexpr int opc_lstore_3 = 66;
inline constexpr int opc_fstore_0 = 67;
inline constexpr int opc_fstore_1 = 68;
inline constexpr int opc_fstore_2 = 69;
inline constexpr int opc_fstore_3 = 70;
inline constexpr int opc_dstore_0 = 71;
inline constexpr int opc_dstore_1 = 72;
inline constexpr int opc_dstore_2 = 73;
inline constexpr int opc_dstore_3 = 74;
inline constexpr int opc_astore_0 = 75;
inline constexpr int opc_astore_1 = 76;
inline constexpr int opc_astore_2 = 77;
inline constexpr int opc_astore_3 = 78;
inline constexpr int opc_iastore = 79;
inline constexpr int opc_lastore = 80;
inline constexpr int opc_fastore = 81;
inline constexpr int opc_dastore = 82;
inline constexpr int opc_aastore = 83;
inline constexpr int opc_bastore = 84;
inline constexpr int opc_castore = 85;
inline constexpr int opc_sastore = 86;
inline constexpr int opc_pop = 87;
inline constexpr int opc_pop2 = 88;
inline constexpr int opc_dup = 89;
inline constexpr int opc_dup_x1 = 90;
inline constexpr int opc_dup_x2 = 91;
inline constexpr int opc_dup2 = 92;
inline constexpr int opc_dup2_x1 = 93;
inline constexpr int opc_dup2_x2 = 94;
inline constexpr int opc_swap = 95;
inline constexpr int opc_iadd = 96;
inline constexpr int opc_ladd = 97;
inline constexpr int opc_fadd = 98;
inline constexpr int opc_dadd = 99;
inline constexpr int opc_isub = 100;
inline constexpr int opc_lsub = 101;
inline constexpr int opc_fsub = 102;
inline constexpr int opc_dsub = 103;
inline constexpr int opc_imul = 104;
inline constexpr int opc_lmul = 105;
inline constexpr int opc_fmul = 106;
inline constexpr int opc_dmul = 107;
inline constexpr int opc_idiv = 108;
inline constexpr int opc_ldiv = 109;
inline constexpr int opc_fdiv = 110;
inline constexpr int opc_ddiv = 111;
inline constexpr int opc_irem = 112;
inline constexpr int opc_lrem = 113;
inline constexpr int opc_frem = 114;
inline constexpr int opc_drem = 115;
inline constexpr int opc_ineg = 116;
inline constexpr int opc_lneg = 117;
inline constexpr int opc_fneg = 118;
inline constexpr int opc_dneg = 119;
inline constexpr int opc_ishl = 120;
inline constexpr int opc_lshl = 121;
inline constexpr int opc_ishr = 122;
inline constexpr int opc_lshr = 123;
inline constexpr int opc_iushr = 124;
inline constexpr int opc_lushr = 125;
inline constexpr int opc_iand = 126;
inline constexpr int opc_land = 127;
inline constexpr int opc_ior = 128;
inline constexpr int opc_lor = 129;
inline constexpr int opc_ixor = 130;
inline constexpr int opc_lxor = 131;
inline constexpr int opc_iinc = 132;
inline constexpr int opc_i2l = 133;
inline constexpr int opc_i2f = 134;
inline constexpr int opc_i2d = 135;
inline constexpr int opc_l2i = 136;
inline constexpr int opc_l2f = 137;
inline constexpr int opc_l2d = 138;
inline constexpr int opc_f2i = 139;
inline constexpr int opc_f2l = 140;
inline constexpr int opc_f2d = 141;
inline constexpr int opc_d2i = 142;
inline constexpr int opc_d2l = 143;
inline constexpr int opc_d2f = 144;
inline constexpr int opc_i2b = 145;
inline constexpr int opc_i2c = 146;
inline constexpr int opc_i2s = 147;
inline constexpr int opc_lcmp = 148;
inline constexpr int opc_fcmpl = 149;
inline constexpr int opc_fcmpg = 150;
inline constexpr int opc_dcmpl = 151;
inline constexpr int opc_dcmpg = 152;
inline constexpr int opc_ifeq = 153;
inline constexpr int opc_ifne = 154;
inline constexpr int opc_iflt = 155;
inline constexpr int opc_ifge = 156;
inline constexpr int opc_ifgt = 157;
inline constexpr int opc_ifle = 158;
inline constexpr int opc_if_icmpeq = 159;
inline constexpr int opc_if_icmpne = 160;
inline constexpr int opc_if_icmplt = 161;
inline constexpr int opc_if_icmpge = 162;
inline constexpr int opc_if_icmpgt = 163;
inline constexpr int opc_if_icmple = 164;
inline constexpr int opc_if_acmpeq = 165;
inline constexpr int opc_if_acmpne = 166;
inline constexpr int opc_goto = 167;
inline constexpr int opc_jsr = 168;
inline constexpr int opc_ret = 169;
inline constexpr int opc_tableswitch = 170;
inline constexpr int opc_lookupswitch = 171;
inline constexpr int opc_ireturn = 172;
inline constexpr int opc_lreturn = 173;
inline constexpr int opc_freturn = 174;
inline constexpr int opc_dreturn = 175;
inline constexpr int opc_areturn = 176;
inline constexpr int opc_return = 177;
inline constexpr int opc_getstatic = 178;
inline constexpr int opc_putstatic = 179;
inline constexpr int opc_getfield = 180;
inline constexpr int opc_putfield = 181;
inline constexpr int opc_invokevirtual = 182;
inline constexpr int opc_invokespecial = 183;
inline constexpr int opc_invokestatic = 184;
inline constexpr int opc_invokeinterface = 185;
inline constexpr int opc_invokedynamic = 186;
inline constexpr int opc_xxxunusedxxx = 186;
inline constexpr int opc_new = 187;
inline constexpr int opc_newarray = 188;
inline constexpr int opc_anewarray = 189;
inline constexpr int opc_arraylength = 190;
inline constexpr int opc_athrow = 191;
inline constexpr int opc_checkcast = 192;
inline constexpr int opc_instanceof = 193;
inline constexpr int opc_monitorenter = 194;
inline constexpr int opc_monitorexit = 195;
inline constexpr int opc_wide = 196;
inline constexpr int opc_multianewarray = 197;
inline constexpr int opc_ifnull = 198;
inline constexpr int opc_ifnonnull = 199;
inline constexpr int opc_goto_w = 200;
inline constexpr int opc_jsr_w = 201;

inline constexpr std::string_view CLINIT_NAME = "<clinit>";
inline constexpr std::string_view INIT_NAME = "<init>";

}  // namespace cb::java::decompiler::cc

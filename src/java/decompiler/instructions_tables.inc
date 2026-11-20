// java/decompiler/instructions_tables.inc -- the stack effect of each opcode and its name, generated from Fernflower (InstructionImpact.stack_impact,
// ConstantsUtil.opcodeNames), Copyright 2000-2015 JetBrains s.r.o., Apache License 2.0.
constexpr ItImpact kItImpact[202] = {
    {false, false, 0, {}, false, 0, {}},   // 0
    {true, false, 0, {}, false, 0, {}},   // 1
    {true, false, 0, {}, false, 0, {}},   // 2
    {true, false, 0, {}, false, 0, {}},   // 3
    {true, false, 0, {}, false, 0, {}},   // 4
    {true, false, 0, {}, false, 0, {}},   // 5
    {true, false, 0, {}, false, 0, {}},   // 6
    {true, false, 0, {}, false, 0, {}},   // 7
    {true, false, 0, {}, false, 0, {}},   // 8
    {false, false, 0, {}, true, 1, {cc::TYPE_LONG}},   // 9
    {false, false, 0, {}, true, 1, {cc::TYPE_LONG}},   // 10
    {false, false, 0, {}, true, 1, {cc::TYPE_FLOAT}},   // 11
    {false, false, 0, {}, true, 1, {cc::TYPE_FLOAT}},   // 12
    {false, false, 0, {}, true, 1, {cc::TYPE_FLOAT}},   // 13
    {false, false, 0, {}, true, 1, {cc::TYPE_DOUBLE}},   // 14
    {false, false, 0, {}, true, 1, {cc::TYPE_DOUBLE}},   // 15
    {false, false, 0, {}, true, 1, {cc::TYPE_INT}},   // 16
    {false, false, 0, {}, true, 1, {cc::TYPE_INT}},   // 17
    {true, false, 0, {}, false, 0, {}},   // 18
    {true, false, 0, {}, false, 0, {}},   // 19
    {true, false, 0, {}, false, 0, {}},   // 20
    {false, false, 0, {}, true, 1, {cc::TYPE_INT}},   // 21
    {false, false, 0, {}, true, 1, {cc::TYPE_LONG}},   // 22
    {false, false, 0, {}, true, 1, {cc::TYPE_FLOAT}},   // 23
    {false, false, 0, {}, true, 1, {cc::TYPE_DOUBLE}},   // 24
    {true, false, 0, {}, false, 0, {}},   // 25
    {true, false, 0, {}, false, 0, {}},   // 26
    {true, false, 0, {}, false, 0, {}},   // 27
    {true, false, 0, {}, false, 0, {}},   // 28
    {true, false, 0, {}, false, 0, {}},   // 29
    {true, false, 0, {}, false, 0, {}},   // 30
    {true, false, 0, {}, false, 0, {}},   // 31
    {true, false, 0, {}, false, 0, {}},   // 32
    {true, false, 0, {}, false, 0, {}},   // 33
    {true, false, 0, {}, false, 0, {}},   // 34
    {true, false, 0, {}, false, 0, {}},   // 35
    {true, false, 0, {}, false, 0, {}},   // 36
    {true, false, 0, {}, false, 0, {}},   // 37
    {true, false, 0, {}, false, 0, {}},   // 38
    {true, false, 0, {}, false, 0, {}},   // 39
    {true, false, 0, {}, false, 0, {}},   // 40
    {true, false, 0, {}, false, 0, {}},   // 41
    {true, false, 0, {}, false, 0, {}},   // 42
    {true, false, 0, {}, false, 0, {}},   // 43
    {true, false, 0, {}, false, 0, {}},   // 44
    {true, false, 0, {}, false, 0, {}},   // 45
    {false, true, 2, {cc::TYPE_OBJECT, cc::TYPE_INT}, true, 1, {cc::TYPE_INT}},   // 46
    {false, true, 2, {cc::TYPE_OBJECT, cc::TYPE_INT}, true, 1, {cc::TYPE_LONG}},   // 47
    {false, true, 2, {cc::TYPE_OBJECT, cc::TYPE_INT}, true, 1, {cc::TYPE_FLOAT}},   // 48
    {false, true, 2, {cc::TYPE_OBJECT, cc::TYPE_INT}, true, 1, {cc::TYPE_DOUBLE}},   // 49
    {true, false, 0, {}, false, 0, {}},   // 50
    {false, true, 2, {cc::TYPE_OBJECT, cc::TYPE_INT}, true, 1, {cc::TYPE_INT}},   // 51
    {false, true, 2, {cc::TYPE_OBJECT, cc::TYPE_INT}, true, 1, {cc::TYPE_INT}},   // 52
    {false, true, 2, {cc::TYPE_OBJECT, cc::TYPE_INT}, true, 1, {cc::TYPE_INT}},   // 53
    {false, true, 1, {cc::TYPE_INT}, false, 0, {}},   // 54
    {false, true, 1, {cc::TYPE_LONG}, false, 0, {}},   // 55
    {false, true, 1, {cc::TYPE_FLOAT}, false, 0, {}},   // 56
    {false, true, 1, {cc::TYPE_DOUBLE}, false, 0, {}},   // 57
    {true, false, 0, {}, false, 0, {}},   // 58
    {true, false, 0, {}, false, 0, {}},   // 59
    {true, false, 0, {}, false, 0, {}},   // 60
    {true, false, 0, {}, false, 0, {}},   // 61
    {true, false, 0, {}, false, 0, {}},   // 62
    {true, false, 0, {}, false, 0, {}},   // 63
    {true, false, 0, {}, false, 0, {}},   // 64
    {true, false, 0, {}, false, 0, {}},   // 65
    {true, false, 0, {}, false, 0, {}},   // 66
    {true, false, 0, {}, false, 0, {}},   // 67
    {true, false, 0, {}, false, 0, {}},   // 68
    {true, false, 0, {}, false, 0, {}},   // 69
    {true, false, 0, {}, false, 0, {}},   // 70
    {true, false, 0, {}, false, 0, {}},   // 71
    {true, false, 0, {}, false, 0, {}},   // 72
    {true, false, 0, {}, false, 0, {}},   // 73
    {true, false, 0, {}, false, 0, {}},   // 74
    {true, false, 0, {}, false, 0, {}},   // 75
    {true, false, 0, {}, false, 0, {}},   // 76
    {true, false, 0, {}, false, 0, {}},   // 77
    {true, false, 0, {}, false, 0, {}},   // 78
    {false, true, 3, {cc::TYPE_OBJECT, cc::TYPE_INT, cc::TYPE_INT}, false, 0, {}},   // 79
    {false, true, 3, {cc::TYPE_OBJECT, cc::TYPE_INT, cc::TYPE_LONG}, false, 0, {}},   // 80
    {false, true, 3, {cc::TYPE_OBJECT, cc::TYPE_INT, cc::TYPE_FLOAT}, false, 0, {}},   // 81
    {false, true, 3, {cc::TYPE_OBJECT, cc::TYPE_INT, cc::TYPE_DOUBLE}, false, 0, {}},   // 82
    {false, true, 3, {cc::TYPE_OBJECT, cc::TYPE_INT, cc::TYPE_OBJECT}, false, 0, {}},   // 83
    {false, true, 3, {cc::TYPE_OBJECT, cc::TYPE_INT, cc::TYPE_INT}, false, 0, {}},   // 84
    {false, true, 3, {cc::TYPE_OBJECT, cc::TYPE_INT, cc::TYPE_INT}, false, 0, {}},   // 85
    {false, true, 3, {cc::TYPE_OBJECT, cc::TYPE_INT, cc::TYPE_INT}, false, 0, {}},   // 86
    {false, true, 1, {cc::TYPE_ANY}, false, 0, {}},   // 87
    {false, true, 2, {cc::TYPE_ANY, cc::TYPE_ANY}, false, 0, {}},   // 88
    {true, false, 0, {}, false, 0, {}},   // 89
    {true, false, 0, {}, false, 0, {}},   // 90
    {true, false, 0, {}, false, 0, {}},   // 91
    {true, false, 0, {}, false, 0, {}},   // 92
    {true, false, 0, {}, false, 0, {}},   // 93
    {true, false, 0, {}, false, 0, {}},   // 94
    {true, false, 0, {}, false, 0, {}},   // 95
    {false, true, 2, {cc::TYPE_INT, cc::TYPE_INT}, true, 1, {cc::TYPE_INT}},   // 96
    {false, true, 2, {cc::TYPE_LONG, cc::TYPE_LONG}, true, 1, {cc::TYPE_LONG}},   // 97
    {false, true, 2, {cc::TYPE_FLOAT, cc::TYPE_FLOAT}, true, 1, {cc::TYPE_FLOAT}},   // 98
    {false, true, 2, {cc::TYPE_DOUBLE, cc::TYPE_DOUBLE}, true, 1, {cc::TYPE_DOUBLE}},   // 99
    {false, true, 2, {cc::TYPE_INT, cc::TYPE_INT}, true, 1, {cc::TYPE_INT}},   // 100
    {false, true, 2, {cc::TYPE_LONG, cc::TYPE_LONG}, true, 1, {cc::TYPE_LONG}},   // 101
    {false, true, 2, {cc::TYPE_FLOAT, cc::TYPE_FLOAT}, true, 1, {cc::TYPE_FLOAT}},   // 102
    {false, true, 2, {cc::TYPE_DOUBLE, cc::TYPE_DOUBLE}, true, 1, {cc::TYPE_DOUBLE}},   // 103
    {false, true, 2, {cc::TYPE_INT, cc::TYPE_INT}, true, 1, {cc::TYPE_INT}},   // 104
    {false, true, 2, {cc::TYPE_LONG, cc::TYPE_LONG}, true, 1, {cc::TYPE_LONG}},   // 105
    {false, true, 2, {cc::TYPE_FLOAT, cc::TYPE_FLOAT}, true, 1, {cc::TYPE_FLOAT}},   // 106
    {false, true, 2, {cc::TYPE_DOUBLE, cc::TYPE_DOUBLE}, true, 1, {cc::TYPE_DOUBLE}},   // 107
    {false, true, 2, {cc::TYPE_INT, cc::TYPE_INT}, true, 1, {cc::TYPE_INT}},   // 108
    {false, true, 2, {cc::TYPE_LONG, cc::TYPE_LONG}, true, 1, {cc::TYPE_LONG}},   // 109
    {false, true, 2, {cc::TYPE_FLOAT, cc::TYPE_FLOAT}, true, 1, {cc::TYPE_FLOAT}},   // 110
    {false, true, 2, {cc::TYPE_DOUBLE, cc::TYPE_DOUBLE}, true, 1, {cc::TYPE_DOUBLE}},   // 111
    {false, true, 2, {cc::TYPE_INT, cc::TYPE_INT}, true, 1, {cc::TYPE_INT}},   // 112
    {false, true, 2, {cc::TYPE_LONG, cc::TYPE_LONG}, true, 1, {cc::TYPE_LONG}},   // 113
    {false, true, 2, {cc::TYPE_FLOAT, cc::TYPE_FLOAT}, true, 1, {cc::TYPE_FLOAT}},   // 114
    {false, true, 2, {cc::TYPE_DOUBLE, cc::TYPE_DOUBLE}, true, 1, {cc::TYPE_DOUBLE}},   // 115
    {false, true, 1, {cc::TYPE_INT}, true, 1, {cc::TYPE_INT}},   // 116
    {false, true, 1, {cc::TYPE_LONG}, true, 1, {cc::TYPE_LONG}},   // 117
    {false, true, 1, {cc::TYPE_FLOAT}, true, 1, {cc::TYPE_FLOAT}},   // 118
    {false, true, 1, {cc::TYPE_DOUBLE}, true, 1, {cc::TYPE_DOUBLE}},   // 119
    {false, true, 2, {cc::TYPE_INT, cc::TYPE_INT}, true, 1, {cc::TYPE_INT}},   // 120
    {false, true, 2, {cc::TYPE_LONG, cc::TYPE_INT}, true, 1, {cc::TYPE_LONG}},   // 121
    {false, true, 2, {cc::TYPE_INT, cc::TYPE_INT}, true, 1, {cc::TYPE_INT}},   // 122
    {false, true, 2, {cc::TYPE_LONG, cc::TYPE_INT}, true, 1, {cc::TYPE_LONG}},   // 123
    {false, true, 2, {cc::TYPE_INT, cc::TYPE_INT}, true, 1, {cc::TYPE_INT}},   // 124
    {false, true, 2, {cc::TYPE_LONG, cc::TYPE_INT}, true, 1, {cc::TYPE_LONG}},   // 125
    {false, true, 2, {cc::TYPE_INT, cc::TYPE_INT}, true, 1, {cc::TYPE_INT}},   // 126
    {false, true, 2, {cc::TYPE_LONG, cc::TYPE_LONG}, true, 1, {cc::TYPE_LONG}},   // 127
    {false, true, 2, {cc::TYPE_INT, cc::TYPE_INT}, true, 1, {cc::TYPE_INT}},   // 128
    {false, true, 2, {cc::TYPE_LONG, cc::TYPE_LONG}, true, 1, {cc::TYPE_LONG}},   // 129
    {false, true, 2, {cc::TYPE_INT, cc::TYPE_INT}, true, 1, {cc::TYPE_INT}},   // 130
    {false, true, 2, {cc::TYPE_LONG, cc::TYPE_LONG}, true, 1, {cc::TYPE_LONG}},   // 131
    {false, false, 0, {}, false, 0, {}},   // 132
    {false, true, 1, {cc::TYPE_INT}, true, 1, {cc::TYPE_LONG}},   // 133
    {false, true, 1, {cc::TYPE_INT}, true, 1, {cc::TYPE_FLOAT}},   // 134
    {false, true, 1, {cc::TYPE_INT}, true, 1, {cc::TYPE_DOUBLE}},   // 135
    {false, true, 1, {cc::TYPE_LONG}, true, 1, {cc::TYPE_INT}},   // 136
    {false, true, 1, {cc::TYPE_LONG}, true, 1, {cc::TYPE_FLOAT}},   // 137
    {false, true, 1, {cc::TYPE_LONG}, true, 1, {cc::TYPE_DOUBLE}},   // 138
    {false, true, 1, {cc::TYPE_FLOAT}, true, 1, {cc::TYPE_INT}},   // 139
    {false, true, 1, {cc::TYPE_FLOAT}, true, 1, {cc::TYPE_LONG}},   // 140
    {false, true, 1, {cc::TYPE_FLOAT}, true, 1, {cc::TYPE_DOUBLE}},   // 141
    {false, true, 1, {cc::TYPE_DOUBLE}, true, 1, {cc::TYPE_INT}},   // 142
    {false, true, 1, {cc::TYPE_DOUBLE}, true, 1, {cc::TYPE_LONG}},   // 143
    {false, true, 1, {cc::TYPE_DOUBLE}, true, 1, {cc::TYPE_FLOAT}},   // 144
    {false, true, 1, {cc::TYPE_INT}, true, 1, {cc::TYPE_INT}},   // 145
    {false, true, 1, {cc::TYPE_INT}, true, 1, {cc::TYPE_INT}},   // 146
    {false, true, 1, {cc::TYPE_INT}, true, 1, {cc::TYPE_INT}},   // 147
    {false, true, 2, {cc::TYPE_LONG, cc::TYPE_LONG}, true, 1, {cc::TYPE_INT}},   // 148
    {false, true, 2, {cc::TYPE_FLOAT, cc::TYPE_FLOAT}, true, 1, {cc::TYPE_INT}},   // 149
    {false, true, 2, {cc::TYPE_FLOAT, cc::TYPE_FLOAT}, true, 1, {cc::TYPE_INT}},   // 150
    {false, true, 2, {cc::TYPE_DOUBLE, cc::TYPE_DOUBLE}, true, 1, {cc::TYPE_INT}},   // 151
    {false, true, 2, {cc::TYPE_DOUBLE, cc::TYPE_DOUBLE}, true, 1, {cc::TYPE_INT}},   // 152
    {false, true, 1, {cc::TYPE_INT}, false, 0, {}},   // 153
    {false, true, 1, {cc::TYPE_INT}, false, 0, {}},   // 154
    {false, true, 1, {cc::TYPE_INT}, false, 0, {}},   // 155
    {false, true, 1, {cc::TYPE_INT}, false, 0, {}},   // 156
    {false, true, 1, {cc::TYPE_INT}, false, 0, {}},   // 157
    {false, true, 1, {cc::TYPE_INT}, false, 0, {}},   // 158
    {false, true, 2, {cc::TYPE_INT, cc::TYPE_INT}, false, 0, {}},   // 159
    {false, true, 2, {cc::TYPE_INT, cc::TYPE_INT}, false, 0, {}},   // 160
    {false, true, 2, {cc::TYPE_INT, cc::TYPE_INT}, false, 0, {}},   // 161
    {false, true, 2, {cc::TYPE_INT, cc::TYPE_INT}, false, 0, {}},   // 162
    {false, true, 2, {cc::TYPE_INT, cc::TYPE_INT}, false, 0, {}},   // 163
    {false, true, 2, {cc::TYPE_INT, cc::TYPE_INT}, false, 0, {}},   // 164
    {false, true, 2, {cc::TYPE_OBJECT, cc::TYPE_OBJECT}, false, 0, {}},   // 165
    {false, true, 2, {cc::TYPE_OBJECT, cc::TYPE_OBJECT}, false, 0, {}},   // 166
    {false, false, 0, {}, false, 0, {}},   // 167
    {false, false, 0, {}, true, 1, {cc::TYPE_ADDRESS}},   // 168
    {false, false, 0, {}, false, 0, {}},   // 169
    {false, true, 1, {cc::TYPE_INT}, false, 0, {}},   // 170
    {false, true, 1, {cc::TYPE_INT}, false, 0, {}},   // 171
    {false, true, 1, {cc::TYPE_INT}, false, 0, {}},   // 172
    {false, true, 1, {cc::TYPE_LONG}, false, 0, {}},   // 173
    {false, true, 1, {cc::TYPE_FLOAT}, false, 0, {}},   // 174
    {false, true, 1, {cc::TYPE_DOUBLE}, false, 0, {}},   // 175
    {false, true, 1, {cc::TYPE_OBJECT}, false, 0, {}},   // 176
    {false, false, 0, {}, false, 0, {}},   // 177
    {true, false, 0, {}, false, 0, {}},   // 178
    {true, false, 0, {}, false, 0, {}},   // 179
    {true, false, 0, {}, false, 0, {}},   // 180
    {true, false, 0, {}, false, 0, {}},   // 181
    {true, false, 0, {}, false, 0, {}},   // 182
    {true, false, 0, {}, false, 0, {}},   // 183
    {true, false, 0, {}, false, 0, {}},   // 184
    {true, false, 0, {}, false, 0, {}},   // 185
    {true, false, 0, {}, false, 0, {}},   // 186
    {true, false, 0, {}, false, 0, {}},   // 187
    {true, false, 0, {}, false, 0, {}},   // 188
    {true, false, 0, {}, false, 0, {}},   // 189
    {false, true, 1, {cc::TYPE_OBJECT}, true, 1, {cc::TYPE_INT}},   // 190
    {true, false, 0, {}, false, 0, {}},   // 191
    {true, false, 0, {}, false, 0, {}},   // 192
    {true, false, 0, {}, false, 0, {}},   // 193
    {false, true, 1, {cc::TYPE_OBJECT}, false, 0, {}},   // 194
    {false, true, 1, {cc::TYPE_OBJECT}, false, 0, {}},   // 195
    {true, false, 0, {}, false, 0, {}},   // 196
    {true, false, 0, {}, false, 0, {}},   // 197
    {false, true, 1, {cc::TYPE_OBJECT}, false, 0, {}},   // 198
    {false, true, 1, {cc::TYPE_OBJECT}, false, 0, {}},   // 199
    {false, false, 0, {}, false, 0, {}},   // 200
    {false, false, 0, {}, true, 1, {cc::TYPE_ADDRESS}},   // 201
};

constexpr const char* kItNames[202] = {
    "nop",
    "aconst_null",
    "iconst_m1",
    "iconst_0",
    "iconst_1",
    "iconst_2",
    "iconst_3",
    "iconst_4",
    "iconst_5",
    "lconst_0",
    "lconst_1",
    "fconst_0",
    "fconst_1",
    "fconst_2",
    "dconst_0",
    "dconst_1",
    "bipush",
    "sipush",
    "ldc",
    "ldc_w",
    "ldc2_w",
    "iload",
    "lload",
    "fload",
    "dload",
    "aload",
    "iload_0",
    "iload_1",
    "iload_2",
    "iload_3",
    "lload_0",
    "lload_1",
    "lload_2",
    "lload_3",
    "fload_0",
    "fload_1",
    "fload_2",
    "fload_3",
    "dload_0",
    "dload_1",
    "dload_2",
    "dload_3",
    "aload_0",
    "aload_1",
    "aload_2",
    "aload_3",
    "iaload",
    "laload",
    "faload",
    "daload",
    "aaload",
    "baload",
    "caload",
    "saload",
    "istore",
    "lstore",
    "fstore",
    "dstore",
    "astore",
    "istore_0",
    "istore_1",
    "istore_2",
    "istore_3",
    "lstore_0",
    "lstore_1",
    "lstore_2",
    "lstore_3",
    "fstore_0",
    "fstore_1",
    "fstore_2",
    "fstore_3",
    "dstore_0",
    "dstore_1",
    "dstore_2",
    "dstore_3",
    "astore_0",
    "astore_1",
    "astore_2",
    "astore_3",
    "iastore",
    "lastore",
    "fastore",
    "dastore",
    "aastore",
    "bastore",
    "castore",
    "sastore",
    "pop",
    "pop2",
    "dup",
    "dup_x1",
    "dup_x2",
    "dup2",
    "dup2_x1",
    "dup2_x2",
    "swap",
    "iadd",
    "ladd",
    "fadd",
    "dadd",
    "isub",
    "lsub",
    "fsub",
    "dsub",
    "imul",
    "lmul",
    "fmul",
    "dmul",
    "idiv",
    "ldiv",
    "fdiv",
    "ddiv",
    "irem",
    "lrem",
    "frem",
    "drem",
    "ineg",
    "lneg",
    "fneg",
    "dneg",
    "ishl",
    "lshl",
    "ishr",
    "lshr",
    "iushr",
    "lushr",
    "iand",
    "land",
    "ior",
    "lor",
    "ixor",
    "lxor",
    "iinc",
    "i2l",
    "i2f",
    "i2d",
    "l2i",
    "l2f",
    "l2d",
    "f2i",
    "f2l",
    "f2d",
    "d2i",
    "d2l",
    "d2f",
    "i2b",
    "i2c",
    "i2s",
    "lcmp",
    "fcmpl",
    "fcmpg",
    "dcmpl",
    "dcmpg",
    "ifeq",
    "ifne",
    "iflt",
    "ifge",
    "ifgt",
    "ifle",
    "if_icmpeq",
    "if_icmpne",
    "if_icmplt",
    "if_icmpge",
    "if_icmpgt",
    "if_icmple",
    "if_acmpeq",
    "if_acmpne",
    "goto",
    "jsr",
    "ret",
    "tableswitch",
    "lookupswitch",
    "ireturn",
    "lreturn",
    "freturn",
    "dreturn",
    "areturn",
    "return",
    "getstatic",
    "putstatic",
    "getfield",
    "putfield",
    "invokevirtual",
    "invokespecial",
    "invokestatic",
    "invokeinterface",
    "invokedynamic",
    "new",
    "newarray",
    "anewarray",
    "arraylength",
    "athrow",
    "checkcast",
    "instanceof",
    "monitorenter",
    "monitorexit",
    "wide",
    "multianewarray",
    "ifnull",
    "ifnonnull",
    "goto_w",
    "jsr_w"
};

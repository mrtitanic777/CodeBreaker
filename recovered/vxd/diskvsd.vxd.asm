; DiskVSD -- reconstructed 32-bit disassembly (VxD / LE format)
; 4 objects, 978 instructions recovered by recursive descent

Declare_Virtual_Device DiskVSD, 3, 10, DiskVSD_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 080000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0xb84  R-X
DiskVSD_Control:
        cmp    eax, 0x1b
        je     loc_2_0000
        clc    
        ret    
        db  000h
loc_1_000C:
        mov    ebx, dword ptr [esp + 4]
        mov    word ptr [ebx + 2], 0
        mov    ax, word ptr [ebx]
        cmp    ax, 3
        je     loc_3_0000
        cmp    ax, 4
        je     loc_3_0181
        cmp    ax, 2
        je     loc_3_0197
        mov    word ptr [ebx + 2], 0xffff
        ret    
        db  000h, 000h
loc_1_0040:
        push   edi
        push   esi
        push   ebx
        mov    esi, dword ptr [esp + 0x10]
        mov    edi, dword ptr [esi + 0x20]
        cmp    byte ptr [edi + 3], 1
        jne    loc_1_0069
        mov    word ptr [esi + 0x6a], 0
loc_1_0056:
        mov    edi, dword ptr [esi + 0x14]
        sub    edi, 8
        mov    dword ptr [esi + 0x14], edi
        push   esi
        call   dword ptr [edi]
        add    esp, 4
loc_1_0065:
        pop    ebx
        pop    esi
        pop    edi
        ret    
loc_1_0069:
        call   sub_1_0298
        or     al, al
        je     loc_1_0056
        cmp    al, 2
        je     loc_1_0065
        add    dword ptr [esi + 0x14], 8
        mov    ecx, dword ptr [esi + 0x10]
        mov    ecx, dword ptr [ecx + 0xc]
        mov    dword ptr [esi + 0x10], ecx
        mov    eax, dword ptr [esi + 0x7c]
        test   dword ptr [esi + 0x6c], 0x200
        jne    loc_1_0098
        mov    edx, dword ptr [esi + 4]
        mul    dword ptr [edx + 0xdb]
loc_1_0098:
        mov    dword ptr [edi + 0x10], eax
        push   esi
        call   dword ptr [ecx]
        add    esp, 4
        jmp    loc_1_0065
loc_1_00A3:
        push   edi
        push   esi
        push   ebx
        mov    esi, dword ptr [esp + 0x10]
        mov    edi, dword ptr [esi + 0x20]
        mov    ebx, dword ptr [esi + 4]
        cmp    byte ptr [edi + 3], 1
        jne    loc_1_011E
        mov    eax, dword ptr [ebx + 0xd7]
        xchg   ah, al
        ror    eax, 0x10
        xchg   ah, al
        mov    dword ptr [ebx + 0xd3], eax
        xor    ecx, ecx
        mov    dword ptr [ebx + 0xd7], ecx
        mov    ecx, dword ptr [ebx + 0xdb]
        xor    cl, cl
        xchg   cl, ch
        ror    ecx, 0x10
        xchg   cl, ch
        mov    dword ptr [ebx + 0xdb], ecx
        bsf    ecx, ecx
        mov    byte ptr [ebx + 0x3c], cl
        mov    cx, 0x200
        cmp    eax, 0x600
        jbe    loc_1_0100
        cmp    eax, 0xd20
        ja     loc_1_011E
        mov    ch, 7
loc_1_0100:
        mov    eax, dword ptr [esi + 0x98]
        mov    word ptr [eax], cx
        mov    word ptr [esi + 0x6a], 0
        mov    dword ptr [esi + 0xa0], 0
        jmp    loc_1_0056
loc_1_011E:
        mov    word ptr [esi + 0x6a], 0x10
        mov    dword ptr [esi + 0xa0], 5
        jmp    loc_1_0056
loc_1_0133:
        mov    edx, dword ptr [esp + 4]
        mov    esi, 0x142                    ; -> loc_1_0142
        VMMCall  VMM.Schedule_Global_Event       ; cd200e800100
loc_1_0142:
        push   edi
        push   esi
        push   ebx
        mov    esi, edx
        mov    ebx, dword ptr [esi + 4]
        mov    eax, dword ptr [esi + 0x80]
        push   0
        push   eax
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
        mov    edi, dword ptr [esi + 0x20]
loc_1_015F:
        mov    eax, esi
        mov    esi, dword ptr [esi + 0x90]
        push   word ptr [edi + 3]
        cmp    byte ptr [edi + 3], 1
        push   esi
        push   edi
        mov    ecx, dword ptr [esi + 0x20]
        mov    esi, dword ptr [edi + 0x1c]
        mov    edi, dword ptr [ecx + 0x1c]
        movzx  ecx, byte ptr [ecx + 0xb]
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    edi
        pop    esi
        call   sub_1_0AE4
        mov    edi, dword ptr [esi + 0x20]
        pop    word ptr [edi + 3]
        cmp    byte ptr [edi + 3], 1
        jne    loc_1_0069
        mov    word ptr [esi + 0x6a], 0
        jmp    loc_1_0056
loc_1_01A3:
        push   edi
        push   esi
        push   ebx
        mov    esi, dword ptr [esp + 0x10]
        mov    edi, dword ptr [esi + 0x20]
        mov    ebx, dword ptr [esi + 4]
        cmp    byte ptr [edi + 3], 1
        jne    loc_1_0234
        test   byte ptr [ebx + 0x24], 0x40
        jne    loc_1_020F
        test   byte ptr [ebx + 0x17], 0x10
        jne    loc_1_01EF
        movzx  eax, word ptr [ebx + 0x55]
        xchg   ah, al
        shr    eax, 3
        dec    eax
        mov    dword ptr [ebx + 0xe3], eax
        mov    dword ptr [ebx + 0xdf], 0x40
        mov    dword ptr [ebx + 0xe7], 0x20
        shl    eax, 0xb
        mov    dword ptr [ebx + 0xd3], eax
loc_1_01EF:
        xor    eax, eax
        mov    dword ptr [ebx + 0x54], eax
        xchg   dword ptr [ebx + 0x58], eax
loc_1_01F7:
        xchg   ah, al
        rol    eax, 0x10
        xchg   ah, al
        mov    dword ptr [ebx + 0xdb], eax
        bsf    eax, eax
        mov    byte ptr [ebx + 0x3c], al
        jmp    loc_1_0056
loc_1_020F:
        mov    eax, dword ptr [ebx + 0xd7]
        xchg   ah, al
        ror    eax, 0x10
        xchg   ah, al
        mov    dword ptr [ebx + 0xd3], eax
        xor    eax, eax
        mov    dword ptr [ebx + 0xd7], eax
        mov    eax, dword ptr [ebx + 0xdb]
        xor    al, al
        jmp    loc_1_01F7
loc_1_0234:
        test   byte ptr [ebx + 0x24], 0x40
        jne    loc_1_0069
        mov    dword ptr [ebx + 0x54], 0
        mov    dword ptr [ebx + 0x58], 0
        jmp    loc_1_0069
loc_1_0251:
        push   edi
        push   esi
        push   ebx
        mov    esi, dword ptr [esp + 0x10]
        mov    edi, dword ptr [esi + 0x20]
        cmp    byte ptr [edi + 3], 1
        jne    loc_1_0069
        mov    byte ptr [edi + 0x30], 0x2f
        mov    eax, dword ptr [esi + 0x14]
        mov    dword ptr [eax], 0x40         ; -> loc_1_0040
        mov    dword ptr [edi + 0xc], 0
        mov    dword ptr [edi + 0x10], 0
        mov    byte ptr [edi + 0x39], 0
        mov    eax, dword ptr [esi + 0x10]
        add    eax, 0x14
        mov    dword ptr [esi + 0x10], eax
        push   esi
        call   dword ptr [eax]
        add    esp, 4
        pop    ebx
        pop    esi
        pop    edi
        ret    
        db  000h
sub_1_0298:
        mov    ebx, dword ptr [esi + 0x14]
        mov    ebx, dword ptr [ebx + 4]
        mov    dword ptr [esi + 0x10], ebx
        mov    ebx, dword ptr [esi + 4]
        test   byte ptr [edi + 3], 0x80
        jne    loc_1_02EB
        cmp    byte ptr [edi + 3], 4
        jne    loc_1_02BC
        cmp    byte ptr [edi + 4], 8
        je     loc_1_02D9
        cmp    byte ptr [edi + 4], 0x18
        je     loc_1_02D9
loc_1_02BC:
        cmp    byte ptr [edi + 0x4c], 5
        jb     loc_1_02D9
        mov    word ptr [esi + 0x6a], 0x1a
loc_1_02C8:
        xor    al, al
        ret    
loc_1_02CB:
        cmp    byte ptr [edi + 0x4c], 5
        jb     loc_1_02D9
        mov    al, byte ptr [ecx + 2]
        jmp    loc_1_0409
loc_1_02D9:
        inc    byte ptr [edi + 0x4c]
        mov    byte ptr [edi + 3], 0
        mov    ax, word ptr [esi + 0xe]
        mov    word ptr [esi + 0xc], ax
        mov    al, 1
        ret    
loc_1_02EB:
        mov    ecx, dword ptr [edi + 0x1c]
        mov    al, byte ptr [ecx + 2]
        and    eax, 0xf
        cmp    al, 0
        je     loc_1_02CB
        cmp    al, 5
        jne    loc_1_0336
        cmp    byte ptr [edi + 0x30], 0x1b
        jne    loc_1_0318
        test   byte ptr [edi + 0x34], 2
        je     loc_1_030F
        and    byte ptr [edi + 0x34], 0xfd
        mov    al, 1
        ret    
loc_1_030F:
        and    byte ptr [ebx + 0x23], 0x7f
        jmp    loc_1_0409
loc_1_0318:
        cmp    byte ptr [edi + 0x30], 0x2e
        jne    loc_1_0409
        mov    byte ptr [edi + 0x30], 0x2a
        mov    edx, dword ptr [esi + 0x14]
        mov    dword ptr [edx], 0x251        ; -> loc_1_0251
        and    word ptr [ebx + 0x7a], 0xfffe
        jmp    loc_1_02CB
loc_1_0336:
        cmp    al, 1
        jne    loc_1_0343
        mov    word ptr [esi + 0x6a], 2
        xor    al, al
        ret    
loc_1_0343:
        cmp    al, 3
        je     loc_1_02CB
        cmp    al, 4
        je     loc_1_02CB
        cmp    al, 6
        jne    loc_1_0359
        test   byte ptr [ebx + 0x20], 4
        je     loc_1_02CB
loc_1_0359:
        cmp    al, 0xb
        je     loc_1_02CB
        cmp    al, 2
        jne    loc_1_0409
        cmp    byte ptr [edi + 0x30], 0x1b
        je     loc_1_0409
        cmp    byte ptr [ecx + 0xc], 4
        jne    loc_1_0409
        cmp    byte ptr [ecx + 0xd], 3
        je     loc_1_0409
        cmp    byte ptr [ecx + 0xd], 4
        je     loc_1_0409
        bts    word ptr [ebx + 0x7a], 1
        jb     loc_1_0409
        mov    edx, edi
        mov    esi, 0x3a5                    ; -> loc_1_03A5
        VMMCall  VMM.Schedule_Global_Event       ; cd200e000100
        mov    al, 2
        ret    
loc_1_03A5:
        mov    edi, edx
        mov    esi, dword ptr [edi + 0x40]
        mov    ebx, dword ptr [esi + 4]
        test   byte ptr [ebx + 0x24], 0x20
        jne    loc_1_03D0
loc_1_03B3:
        mov    ax, 7
        xor    ecx, ecx
        call   sub_1_0936
loc_1_03BE:
        and    byte ptr [ebx + 0x7a], 0xfd
        or     eax, eax
        jne    loc_1_03F5
        push   esi
        call   sub_1_0440
        add    esp, 4
        ret    
loc_1_03D0:
        cmp    word ptr [esi + 0x68], 1
        jne    loc_1_03B3
        sub    esp, 0x34
        mov    ecx, esp
        add    ecx, 3
        and    ecx, 0xfffffffc
        mov    edx, 0x2a
        mov    ax, 0x2a
        call   sub_1_0936
        add    esp, 0x34
        jmp    loc_1_03BE
loc_1_03F5:
        mov    word ptr [esi + 0x6a], ax
        mov    eax, dword ptr [esi + 0x14]
        sub    eax, 8
        mov    dword ptr [esi + 0x14], eax
        push   esi
        call   dword ptr [eax]
        add    esp, 4
        ret    
loc_1_0409:
        mov    ebx, dword ptr [edi + 0x1c]
        cmp    byte ptr [ebx + 0xc], 0x30
        je     loc_1_0422
        movzx  eax, byte ptr [eax + 0x42d]   ; -> obj1:0x042D
        mov    word ptr [esi + 0x6a], ax
        jmp    loc_1_02C8
loc_1_0422:
        mov    word ptr [esi + 0x6a], 0x1c
        jmp    loc_1_02C8
        db  000h, 000h, 020h, 014h, 01Bh, 01Ah, 019h, 01Fh, 01Ah, 01Ah, 01Ah, 01Ah, 01Ah, 01Ah, 01Ah, 01Ah
        db  000h, 000h, 000h
sub_1_0440:
        mov    ebx, dword ptr [esp + 4]
        mov    esi, dword ptr [ebx + 4]
        test   byte ptr [esi + 0x24], 0x60
        jne    loc_1_04AA
loc_1_044D:
        cmp    word ptr [ebx + 0x68], 0xf
        je     loc_1_049A
        call   sub_1_0A4B
        mov    eax, dword ptr [ebx + 0x7c]
        test   dword ptr [ebx + 0x6c], 0x200
        jne    loc_1_046B
        mul    dword ptr [esi + 0xdb]
loc_1_046B:
        mov    dword ptr [edi + 0x10], eax
        mov    edx, dword ptr [ebx + 0x10]
        mov    eax, dword ptr [ebx + 0x14]
        mov    dword ptr [eax], 0x40         ; -> loc_1_0040
        mov    dword ptr [eax + 4], edx
        add    dword ptr [ebx + 0x14], 8
        movzx  eax, word ptr [ebx + 0x68]
        cmp    word ptr [ebx + 0x68], 0xc
        ja     loc_1_04B8
        call   sub_1_055A
loc_1_0493:
        or     dword ptr [ebx + 0x6c], 0x8000
loc_1_049A:
        mov    eax, dword ptr [ebx + 0x10]
        mov    eax, dword ptr [eax + 0xc]
        mov    dword ptr [ebx + 0x10], eax
        push   ebx
        call   dword ptr [eax]
        add    esp, 4
        ret    
loc_1_04AA:
        mov    word ptr [ebx + 0xc], 0x32
        mov    word ptr [ebx + 0xe], 0x32
        jmp    loc_1_044D
loc_1_04B8:
        cmp    eax, 0xd
        jne    loc_1_0515
        test   byte ptr [esi + 0x24], 0x40
        je     loc_1_0515
        cmp    word ptr [ebx + 0x92], 0x4411
        je     loc_1_04D9
        cmp    word ptr [ebx + 0x92], 0x440d
        jne    loc_1_0515
loc_1_04D9:
        cmp    dword ptr [ebx + 0x94], 0x4868
        je     loc_1_04F1
        cmp    dword ptr [ebx + 0x94], 0x868
        jne    loc_1_0515
loc_1_04F1:
        cmp    word ptr [ebx + 0x92], 0x4411
        je     loc_1_0503
        call   sub_1_05A4
        jmp    loc_1_0493
loc_1_0503:
        mov    dword ptr [ebx + 0xa0], 0
        mov    word ptr [ebx + 0x6a], 0
        jmp    loc_1_0594
loc_1_0515:
        cmp    eax, 0x2c
        jne    loc_1_0524
        call   sub_1_0A22
        jmp    loc_1_0493
loc_1_0524:
        cmp    eax, 0xe
        je     loc_1_05B4
        cmp    eax, 0x2a
        jne    loc_1_053C
        call   sub_1_09F8
        jmp    loc_1_0493
loc_1_053C:
        cmp    eax, 0x26
        jne    loc_1_054B
        call   sub_1_07D3
        jmp    loc_1_0493
loc_1_054B:
        sub    dword ptr [ebx + 0x14], 8
        jmp    loc_1_049A
loc_1_0554:
        call   sub_1_058E
        ret    
sub_1_055A:
        imul   eax, dword ptr [esi], 0x6a00000 ; -> loc_1_06A0
        add    byte ptr [eax], al
        and    al, 7
        add    byte ptr [eax], al
        mov    es, word ptr [0x6d70000]      ; -> loc_1_06D7
        add    byte ptr [eax], al
        bound  eax, qword ptr [edi]
        add    byte ptr [eax], al
        bound  eax, qword ptr [edi]
        add    byte ptr [eax], al
        xlatb  
        pop    es
        add    byte ptr [eax], al
        fild   dword ptr [edi]
        add    byte ptr [eax], al
        stosb  byte ptr es:[edi], al
        pop    es
        add    byte ptr [eax], al
        scasb  al, byte ptr es:[edi]
        pop    es
        add    byte ptr [eax], al
        add    byte ptr [edi], 0
        add    byte ptr [eax + 8], al        ; -> sub_1_0840
        add    byte ptr [eax], al
sub_1_058E:
        mov    word ptr [ebx + 0x6a], 0x16
loc_1_0594:
        mov    eax, dword ptr [ebx + 0x14]
        sub    eax, 0x10
        mov    dword ptr [ebx + 0x14], eax
        push   ebx
        call   dword ptr [eax]
        add    esp, 4
        ret    
sub_1_05A4:
        call   sub_1_0840
        mov    eax, dword ptr [ebx + 0x14]
        mov    dword ptr [eax - 8], 0xa3     ; -> loc_1_00A3
        ret    
loc_1_05B4:
        mov    esi, 0x5c1                    ; -> loc_1_05C1
        mov    edx, ebx
        VMMCall  VMM.Schedule_Global_Event       ; cd200e800100
loc_1_05C1:
        mov    ebx, edx
        mov    esi, dword ptr [ebx + 4]
        call   sub_1_08CC
        mov    dword ptr [edi + 0x30], 0x1704
        xor    eax, eax
        mov    dword ptr [edi + 0x34], eax
        mov    dword ptr [edi + 0x38], eax
        mov    dword ptr [edi + 0xc], 0x280
        mov    byte ptr [edi + 0xa], 0xa
        push   1
        push   0xc
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        or     eax, eax
        je     loc_1_0653
        mov    dword ptr [ebx + 0x80], eax
        mov    dword ptr [edi + 0x18], eax
        mov    dword ptr [edi + 0x10], 0xc
        mov    dword ptr [ebx + 0x7c], 0xc
        mov    edx, dword ptr [ebx + 0x90]
        movzx  ecx, word ptr [edx + 0x5e]
        mov    byte ptr [edi + 0x32], cl
        mov    ecx, 0x8000000
        mov    ch, byte ptr [edx + 0x5c]
        or     ch, 0xb0
        mov    dword ptr [eax], ecx
        mov    ecx, dword ptr [esi + 0xd3]
        xchg   cl, ch
        ror    ecx, 0x10
        xchg   cl, ch
        mov    dword ptr [eax + 4], ecx
        mov    ecx, dword ptr [esi + 0xdb]
        xchg   cl, ch
        ror    ecx, 0x10
        xchg   cl, ch
        mov    dword ptr [eax + 8], ecx
        mov    dword ptr [ebx + 0x70], 0x133 ; -> loc_1_0133
        jmp    loc_1_0AA7
loc_1_0653:
        push   edi
        push   esi
        push   ebx
        mov    byte ptr [edi + 3], 4
        mov    byte ptr [edi + 4], 0
        mov    byte ptr [edi + 0x4c], 5
        xchg   ebx, esi
        jmp    loc_1_015F
loc_1_0669:
        mov    dword ptr [edi + 0xc], 0x240
        mov    byte ptr [edi + 0xa], 0xa
        mov    byte ptr [edi + 0x30], 0x28
        mov    al, byte ptr [esi + 0x7d]
        shl    al, 5
        mov    byte ptr [edi + 0x31], al
        mov    eax, dword ptr [ebx + 0x74]
        xchg   al, ah
        ror    eax, 0x10
        xchg   al, ah
        mov    dword ptr [edi + 0x32], eax
        mov    byte ptr [edi + 0x36], 0
        mov    eax, dword ptr [ebx + 0x7c]
        xchg   al, ah
        mov    word ptr [edi + 0x37], ax
        mov    byte ptr [edi + 0x39], 0
        ret    
loc_1_06A0:
        mov    dword ptr [edi + 0xc], 0x280
        mov    byte ptr [edi + 0xa], 0xa
        mov    byte ptr [edi + 0x30], 0x2a
        mov    al, byte ptr [esi + 0x7d]
        shl    al, 5
        mov    byte ptr [edi + 0x31], al
        mov    eax, dword ptr [ebx + 0x74]
        xchg   al, ah
        ror    eax, 0x10
        xchg   al, ah
        mov    dword ptr [edi + 0x32], eax
        mov    byte ptr [edi + 0x36], 0
        mov    eax, dword ptr [ebx + 0x7c]
        xchg   al, ah
        mov    word ptr [edi + 0x37], ax
        mov    byte ptr [edi + 0x39], 0
        ret    
loc_1_06D7:
        mov    dword ptr [edi + 0xc], 0x280
        mov    byte ptr [edi + 0xa], 0xa
        mov    byte ptr [edi + 0x30], 0x2e
        mov    al, byte ptr [esi + 0x7d]
        test   word ptr [esi + 0x7a], 1
        jne    loc_1_06FF
        mov    byte ptr [edi + 0x30], 0x2a
        mov    eax, dword ptr [ebx + 0x14]
        mov    dword ptr [eax - 8], 0x251    ; -> loc_1_0251
loc_1_06FF:
        shl    al, 5
        mov    byte ptr [edi + 0x31], al
        mov    eax, dword ptr [ebx + 0x74]
        xchg   al, ah
        ror    eax, 0x10
        xchg   al, ah
        mov    dword ptr [edi + 0x32], eax
        mov    byte ptr [edi + 0x36], 0
        mov    eax, dword ptr [ebx + 0x7c]
        xchg   al, ah
        mov    word ptr [edi + 0x37], ax
        mov    byte ptr [edi + 0x39], 0
        ret    
loc_1_0724:
        mov    dword ptr [edi + 0x10], 0
        mov    dword ptr [edi + 0xc], 0
        mov    byte ptr [edi + 0xa], 0xa
        mov    byte ptr [edi + 0x30], 0x2f
        mov    al, byte ptr [esi + 0x7d]
        shl    al, 5
        mov    byte ptr [edi + 0x31], al
        mov    eax, dword ptr [ebx + 0x74]
        xchg   al, ah
        ror    eax, 0x10
        xchg   al, ah
        mov    dword ptr [edi + 0x32], eax
        mov    byte ptr [edi + 0x36], 0
        mov    eax, dword ptr [ebx + 0x7c]
        xchg   al, ah
        mov    word ptr [edi + 0x37], ax
        mov    byte ptr [edi + 0x39], 0
        ret    
loc_1_0762:
        mov    dword ptr [edi + 0xc], 0
        mov    byte ptr [edi + 0xa], 6
        mov    byte ptr [edi + 0x30], 0
        mov    al, byte ptr [esi + 0x7d]
        shl    al, 5
        mov    byte ptr [edi + 0x31], al
        mov    dword ptr [edi + 0x32], 0
        ret    
loc_1_0782:
        mov    dword ptr [edi + 0xc], 0x40
        mov    byte ptr [edi + 0xa], 6
        mov    byte ptr [edi + 0x30], 3
        mov    al, byte ptr [esi + 0x7d]
        shl    al, 5
        mov    byte ptr [edi + 0x31], al
        mov    eax, dword ptr [ebx + 0x7c]
        mov    byte ptr [edi + 0x34], al
        xor    eax, eax
        mov    word ptr [edi + 0x32], ax
        mov    byte ptr [edi + 0x35], al
        ret    
loc_1_07AA:
        mov    al, 1
        jmp    loc_1_07B0
loc_1_07AE:
        xor    al, al
loc_1_07B0:
        mov    dword ptr [edi + 0xc], 0
        mov    byte ptr [edi + 0xa], 6
        mov    byte ptr [edi + 0x30], 0x1e
        mov    dword ptr [edi + 0x32], 0
        mov    byte ptr [edi + 0x34], al
        mov    al, byte ptr [esi + 0x7d]
        shl    al, 5
        mov    byte ptr [edi + 0x31], al
        ret    
sub_1_07D3:
        mov    al, 1
        jmp    loc_1_080A
loc_1_07D7:
        mov    al, 3
        jmp    loc_1_080A
loc_1_07DB:
        cmp    dword ptr [esi + 0x8a], 0x75517953
        jne    loc_1_0808
        push   ebx
        mov    ebx, esi
        xor    ecx, ecx
        mov    ax, 6
        call   sub_1_0936
        pop    ebx
        cmp    ax, 0x20
        jne    loc_1_0808
        add    esp, 4
        jmp    loc_1_0554
        db  032h, 0C0h, 0EBh, 002h
loc_1_0808:
        mov    al, 2
loc_1_080A:
        mov    dword ptr [edi + 0x10], 0
        mov    dword ptr [edi + 0xc], 0
        mov    byte ptr [edi + 0xa], 6
        mov    byte ptr [edi + 0x30], 0x1b
        mov    dword ptr [edi + 0x32], 0
        mov    byte ptr [edi + 0x34], al
        mov    al, byte ptr [esi + 0x7d]
        shl    al, 5
        mov    byte ptr [edi + 0x31], al
        mov    word ptr [ebx + 0xc], 0x3c
        mov    word ptr [ebx + 0xe], 0x3c
        ret    
sub_1_0840:
        mov    dword ptr [ebx + 0x80], esi
        test   byte ptr [esi + 0x24], 0x40
        jne    loc_1_08A4
        add    dword ptr [ebx + 0x80], 0x54
        mov    byte ptr [edi + 0x30], 0x25
        mov    dword ptr [edi + 0x32], 0
        mov    dword ptr [edi + 0x36], 0
        mov    eax, 8
loc_1_086A:
        mov    byte ptr [edi + 0xa], 0xa
        mov    dword ptr [ebx + 0x7c], eax
        mov    dword ptr [edi + 0x10], eax
        or     dword ptr [ebx + 0x6c], 0x200
        and    byte ptr [ebx + 0x6c], 0xfd
        mov    eax, dword ptr [ebx + 0x14]
        mov    dword ptr [eax - 8], 0x1a3    ; -> loc_1_01A3
        push   ebx
        call   sub_1_0B14
        add    esp, 4
        mov    dword ptr [edi + 0xc], 0x40
        mov    al, byte ptr [esi + 0x7d]
        shl    al, 5
        mov    byte ptr [edi + 0x31], al
        ret    
loc_1_08A4:
        add    dword ptr [ebx + 0x80], 0xd3
        mov    byte ptr [edi + 0x30], 0x23
        mov    dword ptr [edi + 0x32], 0
        mov    dword ptr [edi + 0x36], 0xc0000
        mov    eax, 0xc
        jmp    loc_1_086A
        db  0C6h, 047h, 002h, 002h, 0C3h
sub_1_08CC:
        push   ebx
        push   dword ptr [ebx + 0x10]
        mov    ecx, dword ptr [esi + 4]
        add    ecx, 0x64
        sub    esp, 0x10
        mov    edi, esp
        mov    dword ptr [edi + 6], ecx
        add    cx, 0x68
        mov    word ptr [edi + 4], cx
        mov    byte ptr [edi + 0xe], 0
        mov    word ptr [edi], 2
        push   edi
        call   sub_1_0B00
        mov    edi, dword ptr [edi + 0xa]
        add    esp, 0x14
        pop    eax
        or     edi, edi
        stc    
        je     loc_1_0935
        add    edi, dword ptr [esi + 4]
        mov    dword ptr [edi + 0x10], eax
        mov    dword ptr [edi + 0xa4], edi
        mov    dword ptr [edi + 0x6c], 0x8409
        mov    word ptr [edi + 0x68], 0x12
        mov    dword ptr [edi + 8], esi
        mov    ebx, edi
        call   sub_1_0A4B
        pop    dword ptr [ebx + 0x90]
        mov    al, byte ptr [esi + 0x7d]
        shl    al, 5
        mov    byte ptr [edi + 0x31], al
        clc    
loc_1_0935:
        ret    
sub_1_0936:
        pushal 
        push   ecx
        mov    ecx, dword ptr [ebx + 4]
        add    ecx, 0x64
        sub    esp, 0x10
        mov    edi, esp
        mov    dword ptr [edi + 6], ecx
        add    cx, 0x68
        mov    word ptr [edi + 4], cx
        mov    byte ptr [edi + 0xe], 0
        mov    word ptr [edi], 2
        push   edi
        call   sub_1_0B00
        mov    edi, dword ptr [edi + 0xa]
        add    esp, 0x14
        or     edi, edi
        je     loc_1_09EF
        add    edi, dword ptr [ebx + 4]
        mov    word ptr [edi + 0x68], ax
        xor    eax, eax
        mov    dword ptr [edi + 0xa8], eax
        mov    dword ptr [edi + 0x7c], eax
        mov    dword ptr [edi + 0x6c], 0x8f09
        mov    al, byte ptr [ebx + 0x15]
        mov    byte ptr [edi + 0xb1], al
        pop    ecx
        or     ecx, ecx
        je     loc_1_09C1
        mov    dword ptr [edi + 0xac], edi
        add    dword ptr [edi + 0xac], 0xbc
        mov    dword ptr [edi + 0x80], ecx
        mov    dword ptr [edi + 0x7c], edx
        mov    dword ptr [edi + 8], ebx
        push   edi
        call   sub_1_0B14
        add    esp, 4
        or     eax, eax
        je     loc_1_09C1
        or     dword ptr [edi + 0x6c], 0x40
loc_1_09C1:
        mov    esi, edi
        xor    edx, edx
        call   sub_1_0B0C
        movzx  eax, word ptr [edi + 0x6a]
        sub    edi, dword ptr [ebx + 4]
        sub    esp, 8
        mov    esi, esp
        mov    dword ptr [esi + 4], edi
        mov    word ptr [esi], 4
        push   esi
        call   sub_1_0B00
        add    esp, 0xc
loc_1_09E9:
        mov    dword ptr [esp + 0x1c], eax
        popal  
        ret    
loc_1_09EF:
        mov    ax, 0x1d
        add    esp, 4
        jmp    loc_1_09E9
sub_1_09F8:
        mov    dword ptr [edi + 0xc], 0x40
        mov    byte ptr [edi + 0xa], 6
        mov    byte ptr [edi + 0x30], 0x1a
        mov    dword ptr [edi + 0x32], 0
        mov    eax, dword ptr [edi + 0x10]
        mov    byte ptr [edi + 0x34], al
        mov    byte ptr [edi + 0x32], 0x2e
        mov    al, byte ptr [esi + 0x7d]
        shl    al, 5
        mov    byte ptr [edi + 0x31], al
        ret    
sub_1_0A22:
        mov    dword ptr [edi + 0xc], 0x40
        mov    byte ptr [edi + 0xa], 0xc
        mov    dword ptr [edi + 0x30], 0x12
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [edi + 0x34], eax
        mov    al, byte ptr [esi + 0x7d]
        shl    al, 5
        mov    byte ptr [edi + 0x31], al
        mov    dword ptr [edi + 0x38], 0
        ret    
sub_1_0A4B:
        mov    eax, dword ptr [ebx + 0x10]
        movsx  edi, word ptr [eax + 0x10]
        add    edi, ebx
        mov    dword ptr [edi], 0x40
        mov    dword ptr [edi + 0x40], ebx
        mov    dword ptr [ebx + 0x20], edi
        mov    byte ptr [edi + 2], 0
        mov    byte ptr [edi + 3], 0
        xor    eax, eax
        mov    dword ptr [edi + 0x24], eax
        mov    dword ptr [edi + 0x44], eax
        mov    dword ptr [edi + 0x20], eax
        mov    byte ptr [edi + 0x4c], al
        mov    byte ptr [edi + 8], al
        mov    byte ptr [edi + 9], al
        mov    al, byte ptr [esi + 0x75]
        mov    byte ptr [edi + 5], al
        mov    al, byte ptr [esi + 0x7c]
        mov    byte ptr [edi + 6], al
        mov    al, byte ptr [esi + 0x7d]
        mov    byte ptr [edi + 7], al
        mov    edx, edi
        add    edx, 0x50
        mov    dword ptr [edi + 0x28], edx
        movzx  eax, word ptr [esi + 0x80]
        add    edx, eax
        mov    dword ptr [edi + 0x1c], edx
        mov    byte ptr [edi + 0xb], 0xe
        ret    
loc_1_0AA7:
        push   ebx
        call   sub_1_0B14
        add    esp, 4
        or     al, al
        je     loc_1_0AB9
        or     dword ptr [ebx + 0x6c], 0x40
loc_1_0AB9:
        mov    byte ptr [edi + 3], 0
        mov    dword ptr [edi + 0x44], 0
        mov    cx, word ptr [ebx + 0xe]
        mov    word ptr [ebx + 0xc], cx
        xor    edx, edx
        xchg   esi, edi
        xchg   edi, ebx
        mov    dword ptr [edi + 0xa4], edi
        mov    edx, 0x440                    ; -> sub_1_0440
        call   sub_1_0B0C
        ret    
sub_1_0AE4:
        sub    eax, dword ptr [ebx + 4]
        sub    esp, 8
        mov    ecx, esp
        mov    dword ptr [ecx + 4], eax
        mov    word ptr [ecx], 4
        push   ecx
        call   sub_1_0B00
        add    esp, 0xc
        ret    
        db  000h
sub_1_0B00:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_0B0C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_0B14:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  3, 10                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'DiskVSD '                  ; DDB_Name
        dd  080000000h                  ; DDB_Init_Order
        dd  DiskVSD_Control          ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x78  R-X
loc_2_0000:
        mov    dword ptr [0x4c], 0xc         ; -> loc_2_004C
        mov    dword ptr [0x50], 0xb00       ; -> loc_2_0050
        push   0x40                          ; -> loc_2_0040
        VxDCall  BLOCKDEV.Service_0007h          ; cd2007001000
        add    esp, 4
        cmp    word ptr [0x6c], 1            ; -> obj2:0x006C
        je     loc_2_003A
        cmp    word ptr [0x6c], 2            ; -> obj2:0x006C
        jne    loc_2_0038
        jmp    loc_2_003A
loc_2_0038:
        jmp    loc_2_003D
loc_2_003A:
        xor    eax, eax
        ret    
loc_2_003D:
        stc    
        ret    
        db  000h
loc_2_0040:
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
loc_2_004C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_0050:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   ebx
        inc    ebx
        push   ebx
        dec    ecx
        and    byte ptr [ecx + ebp*2 + 0x73], al
        imul   esp, dword ptr [eax], 0x56
        push   ebx
        inc    esp
        and    byte ptr [eax], ah
        and    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x1a8  R-X
loc_3_0000:
        mov    esi, dword ptr [ebx + 0xc]
        test   dword ptr [esi + 0x20], 0x6000
        jne    loc_3_0180
        cmp    byte ptr [esi + 0x74], 1
        jne    loc_3_0180
        cmp    byte ptr [esi + 0x40], 0
        jne    loc_3_0180
        mov    eax, dword ptr [esi + 0x1c]
        or     eax, 0x40000
        test   byte ptr [esi + 0x83], 0x80
        je     loc_3_00A4
        cmp    dword ptr [esi + 0x8a], 0x49534e49
        jne    loc_3_005B
        cmp    dword ptr [esi + 0x8e], 0x20204554
        jne    loc_3_008E
        cmp    dword ptr [esi + 0x92], 0x35323349
        jne    loc_3_008E
        jmp    loc_3_008A
loc_3_005B:
        cmp    word ptr [esi + 0xa0], 0x462a
        jne    loc_3_008E
        cmp    dword ptr [esi + 0x8a], 0x454d4f49
        jne    loc_3_008E
        cmp    dword ptr [esi + 0x8e], 0x20204147
        jne    loc_3_008E
        cmp    dword ptr [esi + 0x92], 0x30326f49
        jne    loc_3_008E
loc_3_008A:
        or     byte ptr [esi + 0x24], 0x20
loc_3_008E:
        test   byte ptr [esi + 0x24], 0x40
        je     loc_3_0098
        mov    byte ptr [esi + 0x40], 0xa
loc_3_0098:
        or     dword ptr [esi + 0x20], 0x10004
        or     eax, 0x20000
loc_3_00A4:
        inc    byte ptr [0]                  ; -> obj4:0x0000
        sub    esp, 0x18
        mov    ecx, esp
        mov    word ptr [ecx], 5
        or     eax, 8
        and    eax, 0xfffffffe
        mov    dword ptr [ecx + 0x12], eax
        mov    dword ptr [ecx + 4], esi
        mov    dword ptr [ecx + 8], 0x440    ; -> sub_1_0440
        mov    al, byte ptr [ebx + 8]
        mov    byte ptr [ecx + 0x16], al
        mov    dword ptr [ecx + 0xc], 0
        movzx  eax, word ptr [esi + 0x80]
        add    eax, 0x5e
        mov    word ptr [ecx + 0x10], ax
        push   ecx
        call   sub_1_0B00
        add    esp, 0x1c
        cmp    byte ptr [esi + 0x40], 0xa
        jne    loc_3_0141
        sub    esp, 0x2c
        mov    ecx, esp
        mov    eax, 0x2c
        mov    edx, 0x2c
        push   ecx
        mov    ebx, dword ptr [esi]
        call   sub_1_0936
        pop    ecx
        cmp    al, 0
        jne    loc_3_013E
        cmp    byte ptr [ecx + 4], 0x27
        jb     loc_3_013E
        mov    ecx, dword ptr [ecx + 0x27]
        xor    cl, cl
        xchg   ch, cl
        ror    ecx, 0x10
        xchg   ch, cl
        mov    dword ptr [esi + 0x54], ecx
        mov    dword ptr [esi + 0x60], 8
        mov    dword ptr [esi + 0x68], 0x20
        mov    dword ptr [esi + 0x5c], 0x200
        shr    ecx, 8
        mov    dword ptr [esi + 0x64], ecx
loc_3_013E:
        add    esp, 0x2c
loc_3_0141:
        mov    word ptr [esi + 0x7a], 1
        test   byte ptr [esi + 0x83], 0x80
        je     loc_3_0180
        mov    ebx, esi
        mov    ax, 6
        xor    ecx, ecx
        call   sub_1_0936
        mov    ax, 9
        xor    ecx, ecx
        call   sub_1_0936
        cmp    ax, 0x10
        jae    loc_3_0180
        mov    ax, 0xa
        xor    ecx, ecx
        call   sub_1_0936
        or     dword ptr [ebx + 0x1c], 0x10000
loc_3_0180:
        ret    
loc_3_0181:
        mov    esi, dword ptr [ebx + 0xc]
        cmp    byte ptr [esi + 0x74], 1
        jne    loc_3_0196
        cmp    byte ptr [esi + 0x40], 0
        jne    loc_3_0196
        dec    byte ptr [0]                  ; -> obj4:0x0000
loc_3_0196:
        ret    
loc_3_0197:
        cmp    byte ptr [0], 0               ; -> obj4:0x0000
        jne    loc_3_01A6
        mov    word ptr [ebx + 2], 0xffff
loc_3_01A6:
        ret    
        db  025h
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 4  vsize=0x4  RW-
VxD__LDATA ENDS

; PCCARD -- reconstructed 32-bit disassembly (VxD / LE format)
; 6 objects, 20950 instructions recovered by recursive descent

Declare_Virtual_Device PCCARD, 4, 0, PCCARD_Control, 0097Ch, Undefined_Init_Order
;   DDK version 0400h   init_order 016800000h   flags 00000h
;   service table: obj1:0x189c, 2 services

VxD__LTEXT SEGMENT   ; object 1  vsize=0x9890  R-X
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_000C:
        add    dword ptr [ecx], esp
        and    byte ptr [edx], bl
loc_1_0010:
        add    byte ptr [ecx], al
        add    byte ptr [ecx], al
        add    byte ptr [edx], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [ecx], al
        add    byte ptr [ecx], al
        add    byte ptr [ebx], al
        add    byte ptr [ecx], al
        add    byte ptr [ecx], al
        add    byte ptr [ecx], al
        add    byte ptr [ecx], al
        add    byte ptr [ecx], al
        add    byte ptr [ecx], al
        add    byte ptr [ecx], al
loc_1_0030:
        add    dword ptr [0xff201b1a], edx
        add    byte ptr [eax], al
loc_1_0038:
        dec    ebp
loc_1_0039:
        dec    edi
        inc    esp
        inc    ebp
        dec    ebp
        add    byte ptr [eax], al
        add    byte ptr [esi + 0x41], al
        pop    eax
        add    byte ptr [eax + 0x43], dl
        dec    ebp
        inc    ebx
        dec    ecx
        inc    ecx
        pop    esp
        add    byte ptr [ebp + 0x4e], dl
        dec    ebx
        dec    esi
        dec    edi
        push   edi
        dec    esi
        pop    edi
        dec    ebp
        inc    ecx
        dec    esi
        push   ebp
        inc    esi
        inc    ecx
        inc    ebx
        push   esp
        push   ebp
        push   edx
        inc    ebp
        push   edx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0064:
        sub    eax, 0x564544
        add    byte ptr [eax], al
        add    byte ptr [ebp + 0x4e], dl
        dec    ebx
        dec    esi
        dec    edi
        push   edi
        dec    esi
        pop    edi
        dec    ebp
        inc    ecx
        dec    esi
        push   ebp
        inc    esi
        inc    ecx
        inc    ebx
        push   esp
        push   ebp
        push   edx
        inc    ebp
        push   edx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0084:
        inc    esp
        jbe    0xf1
        arpl   word ptr [ebp + 0x44], sp
        jae    0xf1
        add    byte ptr [eax], al
loc_1_0090:
        sub    dl, byte ptr [eax + 0x43]
        dec    ebp
        inc    ebx
        dec    ecx
        inc    ecx
        pop    esp
        dec    ebp
        inc    esi
        inc    ebx
        add    byte ptr [ebx + 0x6c], al
        popal  
        jae    loc_1_0114
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x6f], al
        insd   dword ptr es:[edi], dx
        jo     loc_1_010A
        je     loc_1_0114
        bound  ebp, qword ptr [ebp + 0x49]
        inc    esp
        jae    loc_1_00B2
loc_1_00B2:
        add    byte ptr [eax], al
loc_1_00B4:
        add    dword ptr [edx], eax
        add    al, byte ptr [edx]
        or     byte ptr [eax], cl
        or     byte ptr [eax], cl
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
        add    al, byte ptr [edx]
loc_1_00C4:
        xor    byte ptr [ecx], dh
        xor    dh, byte ptr [ebx]
        xor    al, 0x35
        aaa    
        cmp    byte ptr [ecx], bh
        inc    ecx
        inc    edx
        inc    ebx
        inc    esp
        inc    ebp
        inc    esi
loc_1_00D4:
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [0xa]          ; -> loc_1_199E
        mov    ecx, dword ptr [esp + 0x14]
        push   ebp
        lea    esi, [ebx + 2]
        test   byte ptr [esi], 1
        je     loc_1_00EF
        mov    edi, dword ptr [ecx + 0x28]
        jmp    loc_1_00F2
loc_1_00EF:
        mov    edi, dword ptr [ecx + 0x24]
loc_1_00F2:
        test   edi, edi
        je     loc_1_0118
        mov    eax, dword ptr [0xa]          ; -> loc_1_199E
        mov    eax, dword ptr [eax + 4]
loc_1_00FE:
        mov    ecx, dword ptr [edi + 0xc]
        cmp    eax, ecx
        jb     loc_1_010E
        mov    edx, dword ptr [edi + 0x10]
        add    edx, ecx
loc_1_010A:
        cmp    edx, eax
        ja     loc_1_0114
loc_1_010E:
        mov    edi, dword ptr [edi]
        test   edi, edi
        jne    loc_1_00FE
loc_1_0114:
        test   edi, edi
        jne    loc_1_012D
loc_1_0118:
        mov    eax, dword ptr [0xa]          ; -> loc_1_199E
        cmp    dword ptr [eax + 4], 0
        je     loc_1_013A
        mov    eax, 7
        jmp    loc_1_01B7
loc_1_012D:
        test   byte ptr [edi + 0xb], 3
        je     loc_1_013A
        mov    eax, 7
        jmp    loc_1_01B7
loc_1_013A:
        push   0x19
        call   sub_1_2540
        add    esp, 4
        test   eax, eax
        jne    loc_1_014F
        mov    eax, 0x20
        jmp    loc_1_01B7
loc_1_014F:
        mov    word ptr [eax + 4], 1
        xor    edx, edx
        mov    ecx, dword ptr [0]            ; -> obj1:0x1924
        cmp    ecx, edx
        je     loc_1_017B
loc_1_0161:
        mov    bp, word ptr [eax + 4]
        cmp    word ptr [ecx + 4], bp
        ja     loc_1_017B
        jne    loc_1_0173
        inc    word ptr [eax + 4]
        jmp    loc_1_0177
loc_1_0173:
        mov    edx, ecx
        mov    ecx, dword ptr [ecx]
loc_1_0177:
        test   ecx, ecx
        jne    loc_1_0161
loc_1_017B:
        mov    cx, word ptr [eax + 4]
        mov    ebp, dword ptr [esp + 0x14]
        mov    word ptr [2], cx              ; -> loc_1_1996
        mov    ecx, dword ptr [esp + 0x18]
        mov    dword ptr [eax + 0x10], ebp
        push   0
        push   eax
        mov    dword ptr [eax + 0xc], ecx
        mov    si, word ptr [esi]
        push   edx
        mov    word ptr [eax + 6], si
        push   0                             ; -> obj1:0x1924
        mov    ebx, dword ptr [ebx + 4]
        mov    dword ptr [eax + 8], ebx
        mov    dword ptr [eax + 0x14], edi
        call   sub_1_2DB5
        add    esp, 0x10
        xor    eax, eax
loc_1_01B7:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_01BC:
        push   0
        push   dword ptr [esp + 8]
        push   0                             ; -> obj1:0x1924
        call   sub_1_2E0B
        add    esp, 0xc
        push   dword ptr [esp + 4]
        call   sub_1_254F
        add    esp, 4
        xor    eax, eax
        ret    
sub_1_01DE:
        mov    ecx, dword ptr [esp + 4]
        push   esi
        push   edi
        mov    eax, dword ptr [ecx + 0x14]
        mov    edi, dword ptr [esp + 0x14]
        mov    edx, dword ptr [eax + 0x10]
        mov    esi, dword ptr [eax + 0xc]
        add    esi, edx
        cmp    edx, edi
        jae    loc_1_01FE
        mov    eax, 0xa
        jmp    loc_1_021E
loc_1_01FE:
        mov    eax, dword ptr [ecx + 8]
        mov    edx, dword ptr [esp + 0x10]
        lea    ecx, [edx + eax]
        cmp    ecx, esi
        jbe    loc_1_0213
        mov    eax, 7
        jmp    loc_1_021E
loc_1_0213:
        add    eax, edi
        add    eax, edx
        cmp    esi, eax
        sbb    eax, eax
        and    eax, 0xa
loc_1_021E:
        pop    edi
        pop    esi
        ret    
sub_1_0221:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [esi]
        cmp    ecx, 0x4000000
        jb     loc_1_0241
        mov    eax, 7
        jmp    loc_1_038B
loc_1_0241:
        mov    eax, dword ptr [esi + 4]
        mov    edx, 0x4000000
        cmp    eax, edx
        ja     loc_1_0386
        add    eax, ecx
        cmp    eax, edx
        ja     loc_1_0386
        cmp    dword ptr [ebp + 8], 1
        mov    edi, dword ptr [ebp + 0xc]
        jne    loc_1_028A
        push   dword ptr [edi + 0xc]
        call   sub_1_27C1
        add    esp, 4
        test   eax, eax
        jne    loc_1_038B
        test   byte ptr [3], 1               ; -> obj1:0x196F
        je     loc_1_028A
        mov    eax, 0x1a
        jmp    loc_1_038B
loc_1_028A:
        lea    eax, [ebp - 4]
        push   eax
        push   dword ptr [edi + 0xc]
        call   sub_1_4432
        add    esp, 8
        test   eax, eax
        je     loc_1_02A7
        mov    eax, 0x19
        jmp    loc_1_038B
loc_1_02A7:
        mov    eax, dword ptr [edi + 0x14]
        test   eax, eax
        jne    loc_1_02B8
        test   byte ptr [edi + 6], 1
        jne    loc_1_02F9
        test   eax, eax
        je     loc_1_02C1
loc_1_02B8:
        mov    cl, byte ptr [eax + 9]
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 0x11], cl
loc_1_02C1:
        test   byte ptr [edi + 6], 1
        jne    loc_1_02D7
        mov    eax, dword ptr [ebp - 4]
        or     byte ptr [eax + 0x14], 8
        mov    eax, dword ptr [ebp - 4]
        and    word ptr [eax + 0x14], 0xfffd
loc_1_02D7:
        push   dword ptr [ebp - 4]
        call   sub_1_4053
        add    esp, 4
        test   eax, eax
        jne    loc_1_0372
        push   dword ptr [ebp - 4]
        call   sub_1_4154
        add    esp, 4
        test   eax, eax
        jne    loc_1_0372
loc_1_02F9:
        mov    eax, dword ptr [esi]
        mov    ebx, dword ptr [esi + 4]
        mov    ecx, dword ptr [4]            ; -> loc_1_1998
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], ecx
        test   ebx, ebx
        mov    edx, dword ptr [eax + 0x1a]
        mov    ecx, dword ptr [edx + 0xc]
        mov    dword ptr [ebp - 0x14], ecx
        je     loc_1_036E
loc_1_031A:
        lea    eax, [ebp - 0x10]
        push   eax
        push   dword ptr [ebp - 0xc]
        push   dword ptr [ebp - 4]
        call   sub_1_450F
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_0372
        mov    edx, dword ptr [ebp - 0x14]
        mov    eax, dword ptr [ebp - 4]
        sub    edx, dword ptr [eax + 0x16]
        sub    edx, dword ptr [ebp - 0xc]
        cmp    edx, ebx
        jbe    loc_1_0342
        mov    edx, ebx
loc_1_0342:
        cmp    dword ptr [ebp + 8], 0
        jne    loc_1_0350
        mov    edi, dword ptr [ebp - 8]
        mov    esi, dword ptr [ebp - 0x10]
        jmp    loc_1_0356
loc_1_0350:
        mov    edi, dword ptr [ebp - 0x10]
        mov    esi, dword ptr [ebp - 8]
loc_1_0356:
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        add    dword ptr [ebp - 0xc], edx
        add    dword ptr [ebp - 8], edx
        sub    ebx, edx
        jne    loc_1_031A
loc_1_036E:
        xor    esi, esi
        jmp    loc_1_0377
loc_1_0372:
        mov    esi, 0x19
loc_1_0377:
        push   dword ptr [ebp - 4]
        call   sub_1_44FF
        add    esp, 4
        mov    eax, esi
        jmp    loc_1_038B
loc_1_0386:
        mov    eax, 0xa
loc_1_038B:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_0392:
        push   esi
        push   edi
        mov    edi, dword ptr [0xa]          ; -> loc_1_199E
        mov    esi, dword ptr [esp + 0xc]
        cmp    dword ptr [esi + 0x14], 0
        jne    loc_1_03B2
        push   edi
        push   esi
        push   0
        call   sub_1_0221
        add    esp, 0xc
        jmp    loc_1_03E3
loc_1_03B2:
        push   dword ptr [edi + 4]
        push   dword ptr [edi]
        push   esi
        call   sub_1_01DE
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_03E3
        mov    eax, dword ptr [esi + 0x14]
        push   edi
        push   esi
        cmp    dword ptr [eax + 0x20], 0
        jne    loc_1_03DB
        push   0
        call   sub_1_0221
        add    esp, 0xc
        jmp    loc_1_03E3
loc_1_03DB:
        call   sub_1_0B47
        add    esp, 8
loc_1_03E3:
        pop    edi
        pop    esi
        ret    
loc_1_03E6:
        push   esi
        push   edi
        mov    edi, dword ptr [0xa]          ; -> loc_1_199E
        mov    esi, dword ptr [esp + 0xc]
        mov    eax, dword ptr [esi + 0x14]
        test   eax, eax
        jne    loc_1_0407
        push   edi
        push   esi
        push   1
        call   sub_1_0221
        add    esp, 0xc
        jmp    loc_1_0444
loc_1_0407:
        cmp    dword ptr [eax + 0x20], 0
        jne    loc_1_0428
        cmp    byte ptr [eax + 0xa], 6
        jne    loc_1_0421
        push   edi
        push   esi
        push   1
        call   sub_1_0221
        add    esp, 0xc
        jmp    loc_1_0444
loc_1_0421:
        mov    eax, 0x1a
        jmp    loc_1_0444
loc_1_0428:
        push   dword ptr [edi + 4]
        push   dword ptr [edi]
        push   esi
        call   sub_1_01DE
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_0444
        push   edi
        push   esi
        call   sub_1_0B76
        add    esp, 8
loc_1_0444:
        pop    edi
        pop    esi
        ret    
loc_1_0447:
        push   esi
        push   edi
        mov    edi, dword ptr [0xa]          ; -> loc_1_199E
        mov    esi, dword ptr [esp + 0xc]
        mov    eax, dword ptr [esi + 0x14]
        test   eax, eax
        je     loc_1_0491
        cmp    dword ptr [eax + 0x20], 0
        je     loc_1_0491
        push   dword ptr [edi + 8]
        push   dword ptr [edi]
        push   esi
        call   sub_1_01DE
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_0496
        push   dword ptr [edi + 8]
        push   dword ptr [edi + 4]
        push   esi
        call   sub_1_01DE
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_0496
        push   edi
        push   esi
        call   sub_1_0BF3
        add    esp, 8
        jmp    loc_1_0496
loc_1_0491:
        mov    eax, 0x1a
loc_1_0496:
        pop    edi
        pop    esi
        ret    
sub_1_0499:
        mov    eax, dword ptr [0]            ; -> obj1:0x1924
        test   eax, eax
        je     loc_1_04B3
        mov    cx, word ptr [esp + 4]
loc_1_04A7:
        cmp    word ptr [eax + 4], cx
        je     loc_1_04B3
        mov    eax, dword ptr [eax]
        test   eax, eax
        jne    loc_1_04A7
loc_1_04B3:
        ret    
sub_1_04B4:
        mov    al, byte ptr [esp + 0xc]
        push   esi
        mov    ecx, dword ptr [esp + 0xc]
        mov    byte ptr [0x10], al           ; -> loc_1_1E24
        mov    edx, dword ptr [ecx + 0xc]
        add    ecx, 0x14
        mov    ax, word ptr [edx + 0x10]
        mov    word ptr [2], ax              ; -> loc_1_1E16
        mov    edx, dword ptr [ecx]
        mov    al, byte ptr [edx + 9]
        mov    byte ptr [0x11], al           ; -> obj1:0x1E25
        mov    edx, dword ptr [ecx]
        mov    si, word ptr [edx + 0x24]
        mov    word ptr [0x12], si           ; -> loc_1_1E26
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [4], eax            ; -> loc_1_1E18
        mov    dword ptr [8], eax            ; -> loc_1_1E1C
        movzx  edx, byte ptr [esi + 3]
        mov    dword ptr [0xc], edx          ; -> loc_1_1E20
        mov    edx, dword ptr [ecx]
        mov    eax, dword ptr [edx + 0x20]
        push   dword ptr [eax + 4]
        mov    dx, word ptr [2]              ; -> loc_1_1E16
        add    eax, 0xc
        push   eax
        push   0                             ; -> obj1:0x1E14
        push   0
        push   0
        push   edx
        push   0x12
        call   sub_1_24A2
        add    esp, 0x1c
        cmp    eax, 0x18
        movzx  eax, word ptr [0x14]          ; -> loc_1_1E28
        jne    loc_1_0569
        test   eax, eax
        je     loc_1_054C
        cmp    eax, 1
        je     loc_1_0552
        cmp    eax, 2
        je     loc_1_0558
        cmp    eax, 3
        je     loc_1_055E
        jmp    loc_1_0562
loc_1_054C:
        mov    byte ptr [esi + 2], 4
        jmp    loc_1_0562
loc_1_0552:
        mov    byte ptr [esi + 2], 2
        jmp    loc_1_0562
loc_1_0558:
        mov    byte ptr [esi + 2], 1
        jmp    loc_1_0562
loc_1_055E:
        mov    byte ptr [esi + 2], 3
loc_1_0562:
        mov    eax, 0x18
        jmp    loc_1_0589
loc_1_0569:
        test   eax, eax
        je     loc_1_0578
        cmp    eax, 0x1a
        je     loc_1_057E
        mov    byte ptr [esi + 2], 0xe1
        jmp    loc_1_0582
loc_1_0578:
        mov    byte ptr [esi + 2], 0xe0
        jmp    loc_1_0582
loc_1_057E:
        mov    byte ptr [esi + 2], 0x84
loc_1_0582:
        movzx  eax, word ptr [0x14]          ; -> loc_1_1E28
loc_1_0589:
        pop    esi
        ret    
sub_1_058B:
        mov    eax, dword ptr [0]            ; -> obj1:0x1928
        test   eax, eax
        je     loc_1_05A0
loc_1_0594:
        cmp    dword ptr [esp + 4], eax
        je     loc_1_05A6
        mov    eax, dword ptr [eax]
        test   eax, eax
        jne    loc_1_0594
loc_1_05A0:
        mov    eax, 0x21
loc_1_05A5:
        ret    
loc_1_05A6:
        xor    eax, eax
        jmp    loc_1_05A5
sub_1_05AA:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    ax, word ptr [esi]
        push   eax
        call   sub_1_0499
        add    esp, 4
        test   eax, eax
        jne    loc_1_05C5
        mov    byte ptr [esi + 2], 0xc3
        jmp    loc_1_05D2
loc_1_05C5:
        mov    ecx, dword ptr [eax + 0x14]
        test   byte ptr [ecx + 7], 0x10
        jne    loc_1_05D4
        mov    byte ptr [esi + 2], 0x86
loc_1_05D2:
        xor    eax, eax
loc_1_05D4:
        pop    esi
        ret    
loc_1_05D6:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        push   dword ptr [ebp + 8]
        call   sub_1_058B
        add    esp, 4
        test   eax, eax
        jne    loc_1_06D5
        xor    ebx, ebx
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [ebp - 4], bx
        mov    dword ptr [ebp - 0xc], ebx
        mov    esi, dword ptr [eax + 0x12]
        mov    dword ptr [eax + 0xa], ebx
        cmp    word ptr [esi + 2], bx
        lea    edi, [esi + 4]
        jbe    loc_1_06C4
loc_1_0611:
        mov    al, byte ptr [edi + 2]
        test   al, 0x80
        jne    loc_1_06AD
        mov    byte ptr [ebp - 1], 0
        movzx  eax, al
        cmp    eax, 1
        je     loc_1_0639
        cmp    eax, 2
        je     loc_1_063F
        cmp    eax, 3
        jl     loc_1_0649
        cmp    eax, 4
        jle    loc_1_0645
        jmp    loc_1_0649
loc_1_0639:
        mov    byte ptr [ebp - 1], 0x10
        jmp    loc_1_0649
loc_1_063F:
        mov    byte ptr [ebp - 1], 0x20
        jmp    loc_1_0649
loc_1_0645:
        mov    byte ptr [ebp - 1], 0x30
loc_1_0649:
        push   edi
        call   sub_1_05AA
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        je     loc_1_06AD
        push   dword ptr [ebp - 1]
        push   eax
        push   edi
        call   sub_1_04B4
        add    esp, 0xc
        cmp    eax, 0x18
        jne    loc_1_0674
        mov    dword ptr [ebp - 0xc], 1
        jmp    loc_1_06AD
loc_1_0674:
        mov    ecx, dword ptr [ebp + 8]
        cmp    eax, 1
        sbb    al, al
        add    al, 0xe1
        mov    byte ptr [edi + 2], al
        mov    eax, dword ptr [ecx + 0xe]
        movzx  edx, word ptr [ecx + 4]
        push   dword ptr [eax + 4]
        add    eax, 0xc
        push   eax
        push   0
        push   edx
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [eax + 0xc]
        push   dword ptr [ebp - 4]
        mov    dx, word ptr [ecx + 0x10]
        push   edx
        push   0x81
        call   sub_1_24A2
        add    esp, 0x1c
loc_1_06AD:
        movzx  eax, word ptr [esi]
        inc    word ptr [ebp - 4]
        add    edi, eax
        mov    cx, word ptr [ebp - 4]
        cmp    word ptr [esi + 2], cx
        ja     loc_1_0611
loc_1_06C4:
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_1_06D5
        push   dword ptr [ebp + 8]
        call   sub_1_06DC
        add    esp, 4
loc_1_06D5:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_06DC:
        movzx  eax, word ptr [0x14]          ; -> loc_1_1E28
        push   ebx
        cmp    eax, 1
        push   esi
        push   edi
        jl     loc_1_06F0
        cmp    eax, 2
        jle    loc_1_06F6
loc_1_06F0:
        mov    bx, 0x1f4
        jmp    loc_1_06FD
loc_1_06F6:
        mov    bx, word ptr [0x16]           ; -> loc_1_1E2A
loc_1_06FD:
        mov    esi, dword ptr [esp + 0x10]
        cmp    dword ptr [esi + 0xa], 0
        jne    loc_1_0745
        push   0xe
        call   sub_1_2540
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        je     loc_1_0745
        push   edi
        mov    dword ptr [edi], 0            ; -> loc_1_05D6
        movzx  eax, bx
        push   eax
        mov    dword ptr [edi + 4], esi
        mov    word ptr [edi + 0xc], 2
        push   0                             ; -> loc_1_23D7
        call   sub_3_53B0
loc_1_0735:
        mov    dword ptr [edi + 8], eax
        test   eax, eax
        jne    loc_1_0745
        push   edi
        call   sub_1_254F
        add    esp, 4
loc_1_0745:
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_0749:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [4]            ; -> loc_1_1998
        cmp    word ptr [edi], 8
        jb     loc_1_07DD
        cmp    word ptr [edi + 2], 1
        jb     loc_1_07DD
        push   0x16
        call   sub_1_2540
        add    esp, 4
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_1_077B
        mov    eax, 0x20
        jmp    loc_1_07E2
loc_1_077B:
        lea    esi, [ebx + 4]
        xor    ebp, ebp
        mov    word ptr [esi], 1
        mov    edx, dword ptr [0]            ; -> obj1:0x1928
        cmp    edx, ebp
        je     loc_1_07AC
loc_1_078F:
        mov    cx, word ptr [edx + 4]
        mov    ax, word ptr [esi]
        cmp    cx, ax
        ja     loc_1_07AC
        jne    loc_1_07A4
        inc    ax
        mov    word ptr [esi], ax
        jmp    loc_1_07A8
loc_1_07A4:
        mov    ebp, edx
        mov    edx, dword ptr [edx]
loc_1_07A8:
        test   edx, edx
        jne    loc_1_078F
loc_1_07AC:
        mov    eax, dword ptr [esp + 0x14]
        push   ebx
        mov    dword ptr [ebx + 0xe], eax
        mov    dword ptr [ebx + 0x12], edi
        call   sub_1_07E7
        add    esp, 4
        push   0
        push   ebx
        push   ebp
        push   0                             ; -> obj1:0x1928
        call   sub_1_2DB5
        mov    ax, word ptr [esi]
        add    esp, 0x10
        mov    word ptr [2], ax              ; -> loc_1_1996
        xor    eax, eax
        jmp    loc_1_07E2
loc_1_07DD:
        mov    eax, 0x1c
loc_1_07E2:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_07E7:
        mov    eax, dword ptr [esp + 4]
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [eax + 0x12]
        mov    bx, word ptr [esi + 2]
        xor    ebp, ebp
        test   bx, bx
        lea    edi, [esi + 4]
        je     loc_1_0835
loc_1_0800:
        cmp    byte ptr [edi + 2], 0
        jne    loc_1_0829
        push   edi
        call   sub_1_05AA
        add    esp, 4
        test   eax, eax
        je     loc_1_0829
        push   0
        push   eax
        push   edi
        call   sub_1_04B4
        add    esp, 0xc
        cmp    eax, 0x18
        jne    loc_1_0829
        mov    ebp, 1
loc_1_0829:
        movzx  eax, word ptr [esi]
        dec    bx
        add    edi, eax
        test   bx, bx
        jne    loc_1_0800
loc_1_0835:
        test   ebp, ebp
        je     loc_1_0845
        push   dword ptr [esp + 0x14]
        call   sub_1_06DC
        add    esp, 4
loc_1_0845:
        xor    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_084C:
        push   0
        push   dword ptr [esp + 8]
        push   0                             ; -> obj1:0x1928
        call   sub_1_2E0B
        add    esp, 0xc
        push   dword ptr [esp + 4]
        call   sub_1_254F
        add    esp, 4
        xor    eax, eax
        ret    
sub_1_086E:
        mov    eax, dword ptr [0]            ; -> obj1:0x1928
        test   eax, eax
        je     loc_1_0888
        mov    cx, word ptr [esp + 4]
loc_1_087C:
        cmp    word ptr [eax + 4], cx
        je     loc_1_0888
        mov    eax, dword ptr [eax]
        test   eax, eax
        jne    loc_1_087C
loc_1_0888:
        ret    
sub_1_0889:
        push   0
        push   dword ptr [esp + 8]
        call   sub_1_2DDA
        add    esp, 8
        test   eax, eax
        je     loc_1_08A6
        push   eax
        call   sub_1_254F
        add    esp, 4
        jmp    sub_1_0889
loc_1_08A6:
        ret    
loc_1_08A7:
        push   esi
        mov    edx, dword ptr [0xa]          ; -> loc_1_199E
        mov    eax, dword ptr [esp + 0xc]
        lea    esi, [edx + 2]
        test   byte ptr [esi], 1
        je     loc_1_08BF
        mov    ecx, dword ptr [eax + 0x28]
        jmp    loc_1_08C2
loc_1_08BF:
        mov    ecx, dword ptr [eax + 0x24]
loc_1_08C2:
        test   ecx, ecx
        je     loc_1_08DD
        mov    eax, dword ptr [0xa]          ; -> loc_1_199E
        mov    eax, dword ptr [eax + 4]
loc_1_08CE:
        cmp    dword ptr [ecx + 0xc], eax
        je     loc_1_08D9
        mov    ecx, dword ptr [ecx]
        test   ecx, ecx
        jne    loc_1_08CE
loc_1_08D9:
        test   ecx, ecx
        jne    loc_1_08E4
loc_1_08DD:
        mov    eax, 7
        jmp    loc_1_08FC
loc_1_08E4:
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [ecx + 0x20], eax
        xor    eax, eax
        mov    si, word ptr [esi]
        mov    word ptr [ecx + 6], si
        mov    dx, word ptr [edx + 8]
        mov    word ptr [ecx + 0x24], dx
loc_1_08FC:
        pop    esi
        ret    
sub_1_08FE:
        mov    ecx, dword ptr [esp + 8]
        test   ecx, ecx
        je     loc_1_091A
        mov    eax, dword ptr [esp + 4]
        xor    edx, edx
loc_1_090C:
        cmp    dword ptr [ecx + 0x20], eax
        jne    loc_1_0914
        mov    dword ptr [ecx + 0x20], edx
loc_1_0914:
        mov    ecx, dword ptr [ecx]
        test   ecx, ecx
        jne    loc_1_090C
loc_1_091A:
        ret    
sub_1_091B:
        push   esi
        mov    esi, dword ptr [0]            ; -> obj1:0x191C
        test   esi, esi
        je     loc_1_094B
loc_1_0926:
        push   dword ptr [esi + 0x28]
        push   dword ptr [esp + 0xc]
        call   sub_1_08FE
        add    esp, 8
        push   dword ptr [esi + 0x24]
        push   dword ptr [esp + 0xc]
        call   sub_1_08FE
        add    esp, 8
        mov    esi, dword ptr [esi + 4]
        test   esi, esi
        jne    loc_1_0926
loc_1_094B:
        pop    esi
        ret    
sub_1_094D:
        mov    ecx, dword ptr [esp + 4]
        push   ebx
        push   esi
        add    ecx, 0x14
        push   edi
        mov    eax, dword ptr [ecx - 8]
        mov    ax, word ptr [eax + 0x10]
        mov    word ptr [2], ax              ; -> loc_1_1E16
        mov    edx, dword ptr [ecx]
        mov    al, byte ptr [edx + 9]
        mov    byte ptr [0x11], al           ; -> obj1:0x1E25
        mov    esi, dword ptr [esp + 0x14]
        mov    edx, dword ptr [ecx]
        mov    bx, word ptr [edx + 0x24]
        mov    word ptr [0x12], bx           ; -> loc_1_1E26
        mov    edx, dword ptr [ecx]
        mov    ebx, dword ptr [edx + 0x20]
        lea    edi, [ebx + 0xc]
loc_1_0987:
        mov    eax, dword ptr [ebx + 4]
        push   eax
        push   edi
        mov    ax, word ptr [2]              ; -> loc_1_1E16
        push   0                             ; -> obj1:0x1E14
        push   esi
        push   0
        push   eax
        push   0x12
        call   sub_1_24A2
        add    esp, 0x1c
        cmp    eax, 0x18
        jne    loc_1_09E9
        and    byte ptr [0x10], 0xcf         ; -> loc_1_1E24
        movzx  ecx, word ptr [0x14]          ; -> loc_1_1E28
        test   ecx, ecx
        je     loc_1_09CB
        cmp    ecx, 1
        je     loc_1_09D4
        cmp    ecx, 2
        je     loc_1_09DD
        cmp    ecx, 3
        jne    loc_1_09E4
loc_1_09CB:
        or     byte ptr [0x10], 0x30         ; -> loc_1_1E24
        jmp    loc_1_09E4
loc_1_09D4:
        or     byte ptr [0x10], 0x20         ; -> loc_1_1E24
        jmp    loc_1_09E4
loc_1_09DD:
        or     byte ptr [0x10], 0x10         ; -> loc_1_1E24
loc_1_09E4:
        cmp    eax, 0x18
        je     loc_1_0987
loc_1_09E9:
        movzx  eax, word ptr [0x14]          ; -> loc_1_1E28
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_09F4:
        mov    eax, dword ptr [esp + 8]
        sub    esp, 4
        mov    dword ptr [esp], 0
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [eax + 4]
        push   ebp
        test   ebx, ebx
        je     loc_1_0A75
loc_1_0A0E:
        mov    ebp, 0x200
        cmp    ebx, ebp
        jae    loc_1_0A19
        mov    ebp, ebx
loc_1_0A19:
        mov    eax, dword ptr [esp + 0x1c]
        push   0                             ; -> obj1:0x1C14
        push   dword ptr [esp + 0x1c]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [4], ecx            ; -> loc_1_1E18
        mov    dword ptr [0xc], ebp          ; -> loc_1_1E20
        mov    byte ptr [0x10], 0xc1         ; -> loc_1_1E24
        call   sub_1_094D
        add    esp, 8
        test   eax, eax
        jne    loc_1_0A7F
        mov    esi, 0                        ; -> obj1:0x1C14
        mov    edi, dword ptr [4]            ; -> loc_1_1998
        mov    ecx, ebp
        mov    eax, dword ptr [esp + 0x10]
        shr    ecx, 2
        lea    edi, [edi + eax]
        repe cmpsd dword ptr [esi], dword ptr es:[edi]
        jne    loc_1_0A7F
        mov    ecx, ebp
        and    ecx, 3
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        jne    loc_1_0A7F
        sub    ebx, ebp
        add    dword ptr [esp + 0x10], ebp
        test   ebx, ebx
        jne    loc_1_0A0E
loc_1_0A75:
        xor    eax, eax
loc_1_0A77:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_1_0A7F:
        mov    eax, 0x12
        jmp    loc_1_0A77
sub_1_0A86:
        mov    eax, dword ptr [esp + 8]
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [eax + 8]
        push   ebp
        test   ebx, ebx
        je     loc_1_0B28
loc_1_0A9C:
        mov    ebp, 0x100
        cmp    ebx, ebp
        jae    loc_1_0AA7
        mov    ebp, ebx
loc_1_0AA7:
        mov    eax, dword ptr [esp + 0x1c]
        push   0                             ; -> obj1:0x1C14
        push   dword ptr [esp + 0x1c]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [4], ecx            ; -> loc_1_1E18
        mov    dword ptr [0xc], ebp          ; -> loc_1_1E20
        mov    byte ptr [0x10], 0xc1         ; -> loc_1_1E24
        call   sub_1_094D
        add    esp, 8
        test   eax, eax
        jne    loc_1_0B32
        mov    eax, dword ptr [esp + 0x1c]
        push   0x100                         ; -> obj1:0x1D14
        push   dword ptr [esp + 0x1c]
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [4], ecx            ; -> loc_1_1E18
        mov    dword ptr [0xc], ebp          ; -> loc_1_1E20
        mov    byte ptr [0x10], 0xc1         ; -> loc_1_1E24
        call   sub_1_094D
        add    esp, 8
        test   eax, eax
        jne    loc_1_0B39
        mov    esi, 0                        ; -> obj1:0x1C14
        mov    edi, 0x100                    ; -> obj1:0x1D14
        mov    ecx, ebp
        shr    ecx, 2
        repe cmpsd dword ptr [esi], dword ptr es:[edi]
        jne    loc_1_0B40
        mov    ecx, ebp
        and    ecx, 3
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        jne    loc_1_0B40
        sub    ebx, ebp
        jne    loc_1_0A9C
loc_1_0B28:
        xor    eax, eax
loc_1_0B2A:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_1_0B32:
        mov    eax, 9
        jmp    loc_1_0B2A
loc_1_0B39:
        mov    eax, 9
        jmp    loc_1_0B2A
loc_1_0B40:
        mov    eax, 0x12
        jmp    loc_1_0B2A
sub_1_0B47:
        mov    eax, dword ptr [esp + 8]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [4], ecx            ; -> loc_1_1E18
        mov    edx, dword ptr [eax + 4]
        mov    eax, dword ptr [4]            ; -> loc_1_1998
        push   eax
        mov    dword ptr [0xc], edx          ; -> loc_1_1E20
        push   dword ptr [esp + 8]
        mov    byte ptr [0x10], 0xc1         ; -> loc_1_1E24
        call   sub_1_094D
        add    esp, 8
        ret    
sub_1_0B76:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        xor    edi, edi
        mov    esi, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [esi]
        mov    dword ptr [8], eax            ; -> loc_1_1E1C
        mov    ecx, dword ptr [esi + 4]
        mov    byte ptr [0x10], 0xc2         ; -> loc_1_1E24
        mov    dword ptr [0xc], ecx          ; -> loc_1_1E20
        test   byte ptr [esi + 8], 4
        je     loc_1_0BA4
        mov    byte ptr [0x10], 0xc6         ; -> loc_1_1E24
loc_1_0BA4:
        test   byte ptr [esi + 8], 8
        je     loc_1_0BC4
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax + 0x14]
        test   byte ptr [ecx + 7], 8
        je     loc_1_0BBF
        or     byte ptr [0x10], 8            ; -> loc_1_1E24
        jmp    loc_1_0BC4
loc_1_0BBF:
        mov    edi, 1
loc_1_0BC4:
        mov    eax, dword ptr [4]            ; -> loc_1_1998
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_094D
        add    esp, 8
        test   eax, eax
        jne    loc_1_0BE9
        test   edi, edi
        je     loc_1_0BE9
        push   esi
        push   dword ptr [ebp + 8]
        call   sub_1_09F4
        add    esp, 8
loc_1_0BE9:
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_1_0BED:
        mov    eax, 0x15
        ret    
sub_1_0BF3:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0x10]
        push   edi
        xor    edi, edi
        mov    eax, dword ptr [esi]
        mov    dword ptr [4], eax            ; -> loc_1_1E18
        mov    ecx, dword ptr [esi + 4]
        mov    dword ptr [8], ecx            ; -> loc_1_1E1C
        mov    edx, dword ptr [esi + 8]
        mov    byte ptr [0x10], 0xc3         ; -> loc_1_1E24
        mov    dword ptr [0xc], edx          ; -> loc_1_1E20
        test   byte ptr [esi + 0xc], 4
        je     loc_1_0C29
        mov    byte ptr [0x10], 0xc7         ; -> loc_1_1E24
loc_1_0C29:
        test   byte ptr [esi + 0xc], 8
        mov    ebx, dword ptr [esp + 0x10]
        je     loc_1_0C4A
        mov    eax, dword ptr [ebx + 0x14]
        test   byte ptr [eax + 7], 8
        je     loc_1_0C45
        or     byte ptr [0x10], 8            ; -> loc_1_1E24
        jmp    loc_1_0C4A
loc_1_0C45:
        mov    edi, 1
loc_1_0C4A:
        push   0
        push   ebx
        call   sub_1_094D
        add    esp, 8
        cmp    eax, 0x15
        jne    loc_1_0C64
        push   esi
        push   ebx
        call   sub_1_0BED
        add    esp, 8
loc_1_0C64:
        test   eax, eax
        jne    loc_1_0C76
        test   edi, edi
        je     loc_1_0C76
        push   esi
        push   ebx
        call   sub_1_0A86
        add    esp, 8
loc_1_0C76:
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_0C7A:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        mov    ax, word ptr [6]              ; -> loc_1_198A
        push   ebx
        mov    word ptr [ebp - 0x14], ax
        push   esi
        mov    si, word ptr [2]              ; -> loc_1_1996
        push   edi
        xor    ebx, ebx
        mov    edi, dword ptr [0xa]          ; -> loc_1_199E
        mov    al, byte ptr [3]              ; -> obj1:0x1987
        mov    dword ptr [ebp - 0x10], ebx
        and    al, 2
        mov    cl, byte ptr [2]              ; -> loc_1_1986
        or     al, 4
        mov    byte ptr [ebp - 8], cl
        movzx  ax, al
        mov    word ptr [ebp - 0x12], ax
        lea    edx, [ebp - 0x18]
        movzx  eax, word ptr [4]             ; -> loc_1_1988
        shl    eax, 0xc
        push   edx
        push   dword ptr [ebp - 0x14]
        mov    dword ptr [ebp - 0xc], eax
        call   sub_1_9645
        add    esp, 8
        test   eax, eax
        jne    loc_1_0D4D
        push   dword ptr [ebp - 0x18]
        push   0                             ; -> obj1:0x192C
        lea    eax, [ebp - 0x14]
        mov    dword ptr [0xa], eax          ; -> loc_1_199E
        call   sub_1_48FD
        add    esp, 8
        test   eax, eax
        jne    loc_1_0D4D
        mov    cx, word ptr [2]              ; -> loc_1_1996
        lea    eax, [ebp - 4]
        push   eax
        push   ecx
        call   sub_1_3AE5
        add    esp, 8
        test   eax, eax
        jne    loc_1_0D4D
        mov    ax, word ptr [2]              ; -> loc_1_1996
        mov    ecx, dword ptr [ebp - 4]
        mov    word ptr [6], ax              ; -> loc_1_198A
        add    ecx, 0x1a
        mov    eax, dword ptr [ecx]
        mov    eax, dword ptr [eax + 0xc]
        shr    eax, 0xc
        mov    word ptr [4], ax              ; -> loc_1_1988
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx]
        mov    edx, dword ptr [ecx + 8]
        mov    dword ptr [0xc], edx          ; -> loc_1_1990
        test   byte ptr [eax + 0x15], 1
        je     loc_1_0D48
        or     byte ptr [3], 2               ; -> obj1:0x1987
loc_1_0D48:
        mov    ebx, 1
loc_1_0D4D:
        mov    word ptr [2], si              ; -> loc_1_1996
        mov    eax, ebx
        mov    dword ptr [0xa], edi          ; -> loc_1_199E
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_0D63:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        mov    cx, word ptr [6]              ; -> loc_1_198A
        push   ebx
        push   esi
        lea    eax, [ebp - 8]
        push   edi
        push   eax
        push   ecx
        call   sub_1_3AE5
        add    esp, 8
        test   eax, eax
        jne    loc_1_0E60
        mov    eax, dword ptr [ebp - 8]
        mov    edi, dword ptr [ebp - 8]
        add    edi, 0x16
        mov    cl, byte ptr [eax + 0x11]
        mov    ax, word ptr [eax + 0x14]
        mov    byte ptr [ebp - 1], cl
        mov    word ptr [ebp - 4], ax
        mov    esi, dword ptr [edi]
        mov    eax, dword ptr [8]            ; -> loc_1_198C
        push   eax
        call   sub_1_2664
        add    esp, 4
        mov    dword ptr [edi], eax
        mov    edi, dword ptr [ebp - 8]
        add    edi, 0x16
        mov    eax, dword ptr [edi]
        cmp    eax, -1
        je     loc_1_0E5E
        mov    ecx, dword ptr [ebp - 8]
        add    ecx, 0x14
        mov    dx, word ptr [ecx]
        test   dh, 1
        mov    word ptr [ebp - 0xc], dx
        je     loc_1_0DE3
        mov    edx, dword ptr [ebp - 8]
        mov    ebx, dword ptr [edx + 0x1a]
        sub    edx, edx
        div    dword ptr [ebx + 0xc]
        test   edx, edx
        jne    loc_1_0E5E
loc_1_0DE3:
        test   byte ptr [3], 2               ; -> obj1:0x1987
        mov    ax, word ptr [ebp - 0xc]
        je     loc_1_0DF4
        or     al, 2
        jmp    loc_1_0DF8
loc_1_0DF4:
        and    ax, 0xfffd
loc_1_0DF8:
        mov    word ptr [ecx], ax
        mov    cl, byte ptr [2]              ; -> loc_1_1986
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0x11], cl
        push   dword ptr [ebp - 8]
        call   sub_1_4053
        add    esp, 4
        test   eax, eax
        jne    loc_1_0E2C
        push   dword ptr [ebp - 8]
        call   sub_1_4154
        add    esp, 4
        test   eax, eax
        jne    loc_1_0E2C
        mov    eax, 1
        jmp    loc_1_0E62
loc_1_0E2C:
        mov    ax, word ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [ecx + 0x14], ax
        mov    ecx, dword ptr [ebp - 8]
        mov    al, byte ptr [ebp - 1]
        mov    dword ptr [ecx + 0x16], esi
        mov    edx, dword ptr [ebp - 8]
        mov    byte ptr [edx + 0x11], al
        push   dword ptr [ebp - 8]
        call   sub_1_4053
        add    esp, 4
        push   dword ptr [ebp - 8]
        call   sub_1_4154
        add    esp, 4
        jmp    loc_1_0E60
loc_1_0E5E:
        mov    dword ptr [edi], esi
loc_1_0E60:
        xor    eax, eax
loc_1_0E62:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_0E69:
        mov    cx, word ptr [6]              ; -> loc_1_198A
        sub    esp, 4
        lea    eax, [esp]
        push   eax
        push   ecx
        call   sub_1_3AE5
        add    esp, 8
        test   eax, eax
        mov    eax, 0
        jne    loc_1_0E9D
        push   dword ptr [esp]
        call   sub_1_4B4F
        add    esp, 4
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
loc_1_0E9D:
        add    esp, 4
        ret    
loc_1_0EA1:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        mov    cx, word ptr [6]              ; -> loc_1_198A
        push   ebx
        push   esi
        xor    ebx, ebx
        mov    dword ptr [ebp - 8], 0
        lea    eax, [ebp - 4]
        push   eax
        push   ecx
        call   sub_1_9645
        add    esp, 8
        test   eax, eax
        je     loc_1_0ED1
        xor    eax, eax
        jmp    loc_1_0F9E
loc_1_0ED1:
        mov    ecx, dword ptr [ebp - 4]
        add    ecx, 0xc
        mov    eax, dword ptr [ecx]
        mov    edx, dword ptr [eax + 0x18]
        mov    dword ptr [0x14], edx         ; -> sub_1_1980
        mov    eax, dword ptr [ecx]
        mov    esi, dword ptr [eax + 0x1c]
        mov    byte ptr [1], 0x84            ; -> obj1:0x196D
        mov    dword ptr [0xa], esi          ; -> obj1:0x1976
        mov    eax, dword ptr [ecx]
        mov    al, byte ptr [eax + 0xc]
        mov    dword ptr [0xe], 0            ; -> obj1:0x197A
        mov    byte ptr [0], al              ; -> obj1:0x196C
        call   sub_1_2438
        test   eax, eax
        je     loc_1_0F18
        xor    eax, eax
        jmp    loc_1_0F9E
loc_1_0F18:
        xor    ax, ax
        mov    ecx, 0x10                     ; -> loc_1_19B4
        cmp    word ptr [0xe], ax            ; -> loc_1_19B2
        jbe    loc_1_0F52
loc_1_0F29:
        mov    dl, byte ptr [ecx]
        cmp    byte ptr [2], dl              ; -> loc_1_1986
        jne    loc_1_0F39
        movzx  esi, ax
        mov    dword ptr [ebp - 8], esi
loc_1_0F39:
        cmp    byte ptr [3], dl              ; -> obj1:0x1987
        jne    loc_1_0F44
        movzx  ebx, ax
loc_1_0F44:
        inc    ax
        add    ecx, 2
        cmp    ax, word ptr [0xe]            ; -> loc_1_19B2
        jb     loc_1_0F29
loc_1_0F52:
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [1], 0x8d            ; -> obj1:0x196D
        mov    ecx, dword ptr [eax + 0xc]
        mov    dl, byte ptr [ecx + 0xc]
        mov    byte ptr [0], dl              ; -> obj1:0x196C
        mov    al, byte ptr [eax + 0x12]
        mov    byte ptr [2], al              ; -> obj1:0x196E
        call   sub_1_2438
        test   eax, eax
        mov    eax, 0
        jne    loc_1_0F9E
        mov    byte ptr [1], 0x8e            ; -> obj1:0x196D
        mov    eax, dword ptr [ebp - 8]
        shl    al, 4
        add    al, bl
        mov    byte ptr [4], al              ; -> obj1:0x1970
        call   sub_1_2438
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
loc_1_0F9E:
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_0FA4:
        mov    eax, 1
        ret    
sub_1_0FAA:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ax, word ptr [esi + 0x10]
        mov    edi, dword ptr [ebp + 0xc]
        mov    word ptr [edi], ax
        mov    cl, byte ptr [ebp + 0x10]
        push   edi
        mov    byte ptr [edi + 4], cl
        mov    byte ptr [edi + 5], 0
        mov    word ptr [edi + 2], 0
        push   esi
        call   sub_1_384D
        add    esp, 8
        test   eax, eax
        jne    loc_1_1009
        mov    eax, dword ptr [ebp + 0x14]
        test   eax, eax
        je     loc_1_0FE6
        mov    cl, byte ptr [edi + 0x11]
        mov    byte ptr [eax], cl
loc_1_0FE6:
        mov    byte ptr [edi + 5], 0
        mov    word ptr [edi + 0x10], 0x20
        push   edi
        push   esi
        call   sub_1_3967
        add    esp, 8
        mov    ecx, eax
        mov    eax, 0
        cmp    ecx, 1
        adc    eax, -1
        and    eax, ecx
loc_1_1009:
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_1_100D:
        push   ebp
        mov    al, byte ptr [esp + 0x10]
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0xc]
        push   esi
        mov    byte ptr [esi + 4], al
        push   dword ptr [ebp + 8]
        call   sub_1_3906
        add    esp, 8
        test   eax, eax
        jne    loc_1_105C
        mov    eax, dword ptr [ebp + 0x14]
        test   eax, eax
        je     loc_1_1037
        mov    cl, byte ptr [esi + 0x11]
        mov    byte ptr [eax], cl
loc_1_1037:
        mov    byte ptr [esi + 5], 0
        mov    word ptr [esi + 0x10], 0x20
        push   esi
        push   dword ptr [ebp + 8]
        call   sub_1_3967
        add    esp, 8
        mov    ecx, eax
        mov    eax, 0
        cmp    ecx, 1
        adc    eax, -1
        and    eax, ecx
loc_1_105C:
        pop    esi
        pop    ebp
        ret    
sub_1_105F:
        mov    eax, dword ptr [esp + 4]
loc_1_1063:
        mov    ecx, eax
        inc    eax
        test   byte ptr [ecx], 0x80
        jne    loc_1_1063
        ret    
sub_1_106C:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0x10
        cmp    word ptr [ebp + 0x10], 0
        push   ebx
        push   esi
        push   edi
        jne    loc_1_1085
        add    eax, 0x24
        jmp    loc_1_1088
loc_1_1085:
        add    eax, 0x28
loc_1_1088:
        mov    dword ptr [ebp - 0xc], 0
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp + 0xc]
        movzx  ecx, byte ptr [ebp + 0x14]
        lea    esi, [eax + 0x14]
        lea    eax, [ecx + eax + 0x14]
        mov    dword ptr [ebp - 8], eax
        cmp    esi, eax
        jae    loc_1_1187
loc_1_10AB:
        mov    bl, byte ptr [esi]
        cmp    bl, 0xff
        je     loc_1_1187
        inc    esi
        mov    al, bl
        and    al, 7
        mov    byte ptr [ebp - 1], al
        cmp    al, 7
        jne    loc_1_10D2
        mov    al, byte ptr [esi]
        push   esi
        mov    byte ptr [ebp - 1], al
        call   sub_1_105F
        add    esp, 4
        mov    esi, eax
loc_1_10D2:
        shr    bl, 4
        cmp    bl, 0xe
        jne    loc_1_10E5
        push   esi
        call   sub_1_105F
        add    esp, 4
        mov    esi, eax
loc_1_10E5:
        mov    al, byte ptr [esi]
        cmp    al, 0xff
        je     loc_1_1187
        inc    esi
        push   0x37
        mov    byte ptr [ebp - 2], al
        call   sub_1_2540
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        je     loc_1_1187
        push   0
        push   edi
        push   dword ptr [ebp - 0x10]
        call   sub_1_2D7C
        mov    dx, word ptr [ebp + 0x10]
        add    esp, 0xc
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x10]
        mov    word ptr [edi + 4], ax
        mov    al, byte ptr [ebp - 1]
        mov    word ptr [edi + 6], dx
        mov    edx, dword ptr [ebp - 0xc]
        mov    byte ptr [edi + 8], 7
        mov    byte ptr [edi + 9], al
        mov    al, byte ptr [ebp - 2]
        shr    al, 3
        mov    dword ptr [edi + 0xc], edx
        movzx  edx, al
        inc    edx
        mov    al, byte ptr [ebp - 2]
        and    al, 7
        add    al, al
        add    al, 9
        mov    cl, al
        shl    edx, cl
        cmp    word ptr [ebp + 0x10], 0
        mov    dword ptr [edi + 0x10], edx
        je     loc_1_115E
        add    edx, edx
        mov    dword ptr [edi + 0x10], edx
loc_1_115E:
        lea    eax, [edi + 0xb]
        test   bl, bl
        mov    byte ptr [eax], 0
        mov    byte ptr [edi + 0xa], bl
        jne    loc_1_1170
        or     byte ptr [eax], 1
        jmp    loc_1_1178
loc_1_1170:
        cmp    bl, 0xd
        jne    loc_1_1178
        or     byte ptr [eax], 2
loc_1_1178:
        mov    eax, dword ptr [edi + 0x10]
        add    dword ptr [ebp - 0xc], eax
        cmp    esi, dword ptr [ebp - 8]
        jb     loc_1_10AB
loc_1_1187:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_118E:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        cmp    word ptr [esp + 0x10], 0
        mov    ebp, esp
        push   esi
        jne    loc_1_11A3
        mov    esi, dword ptr [eax + 0x24]
        jmp    loc_1_11A6
loc_1_11A3:
        mov    esi, dword ptr [eax + 0x28]
loc_1_11A6:
        mov    eax, dword ptr [ebp + 0xc]
        test   esi, esi
        lea    edx, [eax + 0x14]
        je     loc_1_11D0
        movzx  ecx, byte ptr [ebp + 0x14]
        lea    eax, [ecx + eax + 0x14]
loc_1_11B8:
        cmp    edx, eax
        jae    loc_1_11D0
        mov    cx, word ptr [edx]
        add    edx, 2
        mov    word ptr [esi + 0x1a], cx
        or     byte ptr [esi + 8], 0x20
        mov    esi, dword ptr [esi]
        test   esi, esi
        jne    loc_1_11B8
loc_1_11D0:
        pop    esi
        pop    ebp
        ret    
sub_1_11D3:
        push   ebp
        mov    edx, dword ptr [esp + 0xc]
        cmp    word ptr [esp + 0x10], 0
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   ebx
        lea    eax, [edx + 0x14]
        push   esi
        push   edi
        jne    loc_1_11F1
        mov    esi, dword ptr [ecx + 0x24]
        jmp    loc_1_11F4
loc_1_11F1:
        mov    esi, dword ptr [ecx + 0x28]
loc_1_11F4:
        test   esi, esi
        je     loc_1_1245
        movzx  ecx, byte ptr [ebp + 0x14]
        lea    edi, [ecx + edx + 0x14]
loc_1_1200:
        cmp    eax, edi
        jae    loc_1_1245
        lea    edx, [eax + 5]
        mov    ebx, 1
        mov    cl, byte ptr [eax]
        push   edx
        dec    cl
        shl    ebx, cl
        mov    cl, byte ptr [eax + 1]
        dec    cl
        shl    ebx, cl
        mov    cl, byte ptr [edx]
        dec    cl
        shl    ebx, cl
        mov    dword ptr [esi + 0x14], ebx
        mov    bx, 1
        mov    cl, byte ptr [eax + 4]
        dec    cl
        shl    bx, cl
        mov    word ptr [esi + 0x18], bx
        or     byte ptr [esi + 8], 0x18
        call   sub_1_105F
        add    esp, 4
        mov    esi, dword ptr [esi]
        test   esi, esi
        jne    loc_1_1200
loc_1_1245:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
sub_1_124A:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    eax, [ebp - 1]
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   eax
        mov    edi, dword ptr [ebp + 0xc]
        push   1
        push   edi
        push   esi
        call   sub_1_0FAA
        add    esp, 0x10
        test   eax, eax
        jne    loc_1_127B
        push   dword ptr [ebp - 1]
        push   0
        push   edi
        push   esi
        call   sub_1_106C
        add    esp, 0x10
loc_1_127B:
        lea    eax, [ebp - 1]
        push   eax
        push   0x17
        push   edi
        push   esi
        call   sub_1_0FAA
        add    esp, 0x10
        test   eax, eax
        jne    loc_1_129E
        push   dword ptr [ebp - 1]
        push   1
        push   edi
        push   esi
        call   sub_1_106C
        add    esp, 0x10
loc_1_129E:
        lea    eax, [ebp - 1]
        push   eax
        push   0x18
        push   edi
        push   esi
        call   sub_1_0FAA
        add    esp, 0x10
        test   eax, eax
        jne    loc_1_12C1
        push   dword ptr [ebp - 1]
        push   0
        push   edi
        push   esi
        call   sub_1_118E
        add    esp, 0x10
loc_1_12C1:
        lea    eax, [ebp - 1]
        push   eax
        push   0x19
        push   edi
        push   esi
        call   sub_1_0FAA
        add    esp, 0x10
        test   eax, eax
        jne    loc_1_12E4
        push   dword ptr [ebp - 1]
        push   1
        push   edi
        push   esi
        call   sub_1_118E
        add    esp, 0x10
loc_1_12E4:
        lea    eax, [ebp - 1]
        push   eax
        push   0x1e
        push   edi
        push   esi
        call   sub_1_0FAA
        add    esp, 0x10
        test   eax, eax
        jne    loc_1_1307
        push   dword ptr [ebp - 1]
        push   0
        push   edi
        push   esi
        call   sub_1_11D3
        add    esp, 0x10
loc_1_1307:
        lea    eax, [ebp - 1]
        push   eax
        push   0x1f
        push   edi
        push   esi
        call   sub_1_0FAA
        add    esp, 0x10
        test   eax, eax
        jne    loc_1_132A
        push   dword ptr [ebp - 1]
        push   1
        push   edi
        push   esi
        call   sub_1_11D3
        add    esp, 0x10
loc_1_132A:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_1330:
        mov    ecx, dword ptr [esp + 4]
        push   ebx
        mov    edx, dword ptr [esp + 0xc]
        push   esi
        or     byte ptr [ecx + 8], 0x40
        cmp    byte ptr [edx + 0x14], 0
        jne    loc_1_13EB
        add    edx, 0x15
        mov    esi, 0                        ; -> obj1:0x1C00
        mov    eax, edx
loc_1_1352:
        mov    bl, byte ptr [eax]
        cmp    bl, byte ptr [esi]
        jne    loc_1_1372
        or     bl, bl
        je     loc_1_136E
        mov    bl, byte ptr [eax + 1]
        cmp    bl, byte ptr [esi + 1]
        jne    loc_1_1372
        add    eax, 2
        add    esi, 2
        or     bl, bl
        jne    loc_1_1352
loc_1_136E:
        xor    eax, eax
        jmp    loc_1_1377
loc_1_1372:
        sbb    eax, eax
        sbb    eax, -1
loc_1_1377:
        test   eax, eax
        jne    loc_1_1381
        or     byte ptr [ecx + 0x26], 1
        jmp    loc_1_13F1
loc_1_1381:
        mov    eax, edx
        mov    esi, 0                        ; -> obj1:0x1C04
loc_1_1388:
        mov    bl, byte ptr [eax]
        cmp    bl, byte ptr [esi]
        jne    loc_1_13A8
        or     bl, bl
        je     loc_1_13A4
        mov    bl, byte ptr [eax + 1]
        cmp    bl, byte ptr [esi + 1]
        jne    loc_1_13A8
        add    eax, 2
        add    esi, 2
        or     bl, bl
        jne    loc_1_1388
loc_1_13A4:
        xor    eax, eax
        jmp    loc_1_13AD
loc_1_13A8:
        sbb    eax, eax
        sbb    eax, -1
loc_1_13AD:
        test   eax, eax
        jne    loc_1_13B7
        or     byte ptr [ecx + 0x26], 2
        jmp    loc_1_13F1
loc_1_13B7:
        mov    eax, 0                        ; -> obj1:0x1C0C
loc_1_13BC:
        mov    bl, byte ptr [edx]
        cmp    bl, byte ptr [eax]
        jne    loc_1_13DC
        or     bl, bl
        je     loc_1_13D8
        mov    bl, byte ptr [edx + 1]
        cmp    bl, byte ptr [eax + 1]
        jne    loc_1_13DC
        add    edx, 2
        add    eax, 2
        or     bl, bl
        jne    loc_1_13BC
loc_1_13D8:
        xor    eax, eax
        jmp    loc_1_13E1
loc_1_13DC:
        sbb    eax, eax
        sbb    eax, -1
loc_1_13E1:
        test   eax, eax
        jne    loc_1_13EB
        or     byte ptr [ecx + 0x26], 3
        jmp    loc_1_13F1
loc_1_13EB:
        or     word ptr [ecx + 0x26], 0x7fff
loc_1_13F1:
        pop    esi
        pop    ebx
        ret    
sub_1_13F4:
        sub    esp, 4
        lea    eax, [esp + 3]
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x18]
        push   ebp
        mov    esi, dword ptr [esp + 0x18]
        push   eax
        push   0x41
        push   edi
        push   esi
        call   sub_1_0FAA
        add    esp, 0x10
        test   eax, eax
        jne    loc_1_1528
loc_1_141C:
        push   0x37
        call   sub_1_2540
        add    esp, 4
        mov    ebx, eax
        test   ebx, ebx
        je     loc_1_1528
        push   0
        lea    eax, [esi + 0x2c]
        push   ebx
        push   eax
        call   sub_1_2D7C
        mov    ax, word ptr [esi + 0x10]
        add    esp, 0xc
        mov    word ptr [ebx + 4], ax
        mov    word ptr [ebx + 6], 0
        mov    byte ptr [ebx + 8], 6
        mov    ecx, dword ptr [edi + 0x16]
        mov    eax, 0
        mov    dword ptr [ebx + 0xc], ecx
        mov    edx, dword ptr [edi + 0x1a]
        mov    dword ptr [ebx + 0x10], edx
        cmp    byte ptr [edi + 0x15], 1
        adc    eax, -1
        and    ax, 0x8000
        mov    word ptr [ebx + 0x26], ax
        mov    ecx, dword ptr [esi + 0x24]
        test   ecx, ecx
        je     loc_1_14B6
loc_1_1478:
        mov    eax, dword ptr [ecx + 0xc]
        mov    edx, dword ptr [ebx + 0xc]
        cmp    eax, edx
        ja     loc_1_14B0
        mov    ebp, dword ptr [ecx + 0x10]
        add    ebp, eax
        cmp    ebp, edx
        jbe    loc_1_14B0
        mov    al, byte ptr [ecx + 9]
        mov    byte ptr [ebx + 9], al
        mov    edx, dword ptr [ecx + 0x14]
        mov    dword ptr [ebx + 0x14], edx
        mov    bp, word ptr [ecx + 0x18]
        mov    word ptr [ebx + 0x18], bp
        mov    ax, word ptr [ecx + 0x1a]
        mov    word ptr [ebx + 0x1a], ax
        mov    dl, byte ptr [ecx + 8]
        or     byte ptr [ebx + 8], dl
        mov    dword ptr [ebx + 0x28], ecx
loc_1_14B0:
        mov    ecx, dword ptr [ecx]
        test   ecx, ecx
        jne    loc_1_1478
loc_1_14B6:
        mov    byte ptr [edi + 4], 0xff
        push   edi
        push   esi
        call   sub_1_3906
        add    esp, 8
        test   eax, eax
        jne    loc_1_14FD
loc_1_14C8:
        mov    al, byte ptr [edi + 0x10]
        cmp    al, 0x41
        je     loc_1_14FD
        cmp    al, 0x46
        jne    loc_1_14EB
        push   edi
        push   esi
        call   sub_1_3967
        add    esp, 8
        test   eax, eax
        jne    loc_1_14FD
        push   edi
        push   ebx
        call   sub_1_1330
        add    esp, 8
loc_1_14EB:
        mov    byte ptr [edi + 4], 0xff
        push   edi
        push   esi
        call   sub_1_3906
        add    esp, 8
        test   eax, eax
        je     loc_1_14C8
loc_1_14FD:
        cmp    byte ptr [edi + 0x10], 0x41
        jne    loc_1_150F
        push   edi
        push   esi
        call   sub_1_3967
        add    esp, 8
        jmp    loc_1_1520
loc_1_150F:
        lea    eax, [esp + 0x13]
        push   eax
        push   0x41
        push   edi
        push   esi
        call   sub_1_100D
        add    esp, 0x10
loc_1_1520:
        test   eax, eax
        je     loc_1_141C
loc_1_1528:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_1_1530:
        mov    edx, dword ptr [esp + 8]
        push   esi
        mov    ecx, dword ptr [esp + 8]
        mov    ax, word ptr [ecx + 6]
        mov    word ptr [edx + 2], ax
        movzx  si, byte ptr [ecx + 0xa]
        shl    si, 4
        add    si, ax
        mov    word ptr [edx + 2], si
        mov    al, byte ptr [ecx + 8]
        mov    byte ptr [edx + 4], al
        mov    al, byte ptr [ecx + 9]
        mov    byte ptr [edx + 5], al
        mov    esi, dword ptr [ecx + 0xc]
        mov    dword ptr [edx + 0x10], esi
        mov    eax, dword ptr [ecx + 0x10]
        mov    dword ptr [edx + 0x14], eax
        mov    esi, dword ptr [ecx + 0x14]
        mov    dword ptr [edx + 0x18], esi
        mov    ax, word ptr [ecx + 0x18]
        mov    word ptr [edx + 0x1c], ax
        mov    si, word ptr [ecx + 0x1a]
        mov    word ptr [edx + 0x1e], si
        mov    dword ptr [edx + 8], ecx
        pop    esi
        ret    
sub_1_1584:
        mov    eax, dword ptr [esp + 4]
        test   eax, eax
        je     loc_1_1598
loc_1_158C:
        test   byte ptr [eax + 0xb], 3
        je     loc_1_159A
        mov    eax, dword ptr [eax]
        test   eax, eax
        jne    loc_1_158C
loc_1_1598:
        xor    eax, eax
loc_1_159A:
        ret    
loc_1_159B:
        mov    eax, dword ptr [esp + 8]
        push   esi
        mov    esi, dword ptr [0xa]          ; -> loc_1_199E
        test   byte ptr [esi + 2], 1
        je     loc_1_15B1
        mov    eax, dword ptr [eax + 0x28]
        jmp    loc_1_15B4
loc_1_15B1:
        mov    eax, dword ptr [eax + 0x24]
loc_1_15B4:
        push   eax
        call   sub_1_1584
        add    esp, 4
        test   eax, eax
        jne    loc_1_15C8
        mov    eax, 0x1f
        jmp    loc_1_15D4
loc_1_15C8:
        push   esi
        push   eax
        call   sub_1_1530
        add    esp, 8
        xor    eax, eax
loc_1_15D4:
        pop    esi
        ret    
loc_1_15D6:
        mov    eax, dword ptr [esp + 8]
        push   esi
        mov    esi, dword ptr [0xa]          ; -> loc_1_199E
        test   byte ptr [esi + 2], 1
        je     loc_1_15EC
        mov    ecx, dword ptr [eax + 0x28]
        jmp    loc_1_15EF
loc_1_15EC:
        mov    ecx, dword ptr [eax + 0x24]
loc_1_15EF:
        test   ecx, ecx
        je     loc_1_1609
        mov    eax, dword ptr [0xa]          ; -> loc_1_199E
        mov    eax, dword ptr [eax + 8]
loc_1_15FB:
        cmp    ecx, eax
        je     loc_1_1605
        mov    ecx, dword ptr [ecx]
        test   ecx, ecx
        jne    loc_1_15FB
loc_1_1605:
        test   ecx, ecx
        jne    loc_1_1610
loc_1_1609:
        mov    eax, 0x1c
        jmp    loc_1_1636
loc_1_1610:
        mov    eax, dword ptr [ecx]
        test   eax, eax
        je     loc_1_1631
        push   eax
        call   sub_1_1584
        add    esp, 4
        test   eax, eax
        je     loc_1_1631
        push   esi
        push   eax
        call   sub_1_1530
        add    esp, 8
        xor    eax, eax
        jmp    loc_1_1636
loc_1_1631:
        mov    eax, 0x1f
loc_1_1636:
        pop    esi
        ret    
sub_1_1638:
        mov    ecx, dword ptr [esp + 4]
        push   esi
        mov    edx, dword ptr [esp + 0xc]
        test   byte ptr [ecx + 2], 1
        je     loc_1_164C
        add    edx, 0x28
        jmp    loc_1_164F
loc_1_164C:
        add    edx, 0x24
loc_1_164F:
        mov    esi, dword ptr [edx]
        test   esi, esi
        je     loc_1_1695
        mov    eax, dword ptr [ecx + 4]
loc_1_1658:
        cmp    dword ptr [esi + 0xc], eax
        je     loc_1_1663
        mov    esi, dword ptr [esi]
        test   esi, esi
        jne    loc_1_1658
loc_1_1663:
        test   esi, esi
        je     loc_1_1695
        cmp    dword ptr [esi + 0x20], 0
        jne    loc_1_1695
        mov    eax, dword ptr [esi + 0x10]
        cmp    dword ptr [ecx + 8], eax
        je     loc_1_167C
        mov    eax, 0xa
        jmp    loc_1_169A
loc_1_167C:
        push   0
        push   esi
        push   edx
        call   sub_1_2E0B
        add    esp, 0xc
        push   esi
        call   sub_1_254F
        add    esp, 4
        xor    eax, eax
        jmp    loc_1_169A
loc_1_1695:
        mov    eax, 7
loc_1_169A:
        pop    esi
        ret    
sub_1_169C:
        mov    edx, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 8]
        mov    ax, word ptr [edx]
        mov    word ptr [ecx + 4], ax
        mov    ax, word ptr [edx + 2]
        and    ax, 0xff0f
        mov    word ptr [ecx + 6], ax
        mov    ax, word ptr [edx + 2]
        shr    ax, 4
        and    al, 0xf
        mov    byte ptr [ecx + 0xa], al
        mov    al, byte ptr [edx + 0x18]
        mov    byte ptr [ecx + 9], al
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [ecx + 0xc], eax
        mov    eax, dword ptr [edx + 8]
        mov    dword ptr [ecx + 0x10], eax
        mov    eax, dword ptr [edx + 0xc]
        mov    dword ptr [ecx + 0x14], eax
        mov    ax, word ptr [edx + 0x10]
        mov    word ptr [ecx + 0x18], ax
        mov    ax, word ptr [edx + 0x12]
        mov    word ptr [ecx + 0x1a], ax
        mov    edx, dword ptr [edx + 0x14]
        mov    dword ptr [ecx + 0x1c], edx
        ret    
loc_1_16F3:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [0xa]          ; -> loc_1_199E
        mov    dx, word ptr [esi + 2]
        push   ebp
        test   dl, 2
        je     loc_1_1718
        push   dword ptr [esp + 0x18]
        push   esi
        call   sub_1_1638
        add    esp, 8
        jmp    loc_1_1818
loc_1_1718:
        test   dh, 1
        jne    loc_1_174E
        mov    ecx, dword ptr [esi + 4]
        cmp    ecx, 0x4000000
        jb     loc_1_1732
        mov    eax, 7
        jmp    loc_1_1818
loc_1_1732:
        mov    eax, dword ptr [esi + 8]
        mov    edi, 0x4000000
        cmp    eax, edi
        jae    loc_1_1744
        add    eax, ecx
        cmp    eax, edi
        jbe    loc_1_174E
loc_1_1744:
        mov    eax, 0xa
        jmp    loc_1_1818
loc_1_174E:
        mov    ebp, dword ptr [esi + 4]
        mov    ecx, dword ptr [esi + 8]
        add    ecx, ebp
        mov    edi, dword ptr [esp + 0x18]
        test   dl, 1
        je     loc_1_1764
        add    edi, 0x28
        jmp    loc_1_1767
loc_1_1764:
        add    edi, 0x24
loc_1_1767:
        xor    ebx, ebx
        mov    edx, dword ptr [edi]
        cmp    edx, ebx
        je     loc_1_1787
loc_1_176F:
        mov    eax, dword ptr [edx + 0xc]
        cmp    ebp, eax
        jbe    loc_1_1787
        mov    ebx, dword ptr [edx + 0x10]
        add    ebx, eax
        cmp    ebx, ebp
        ja     loc_1_17A2
        mov    ebx, edx
        mov    edx, dword ptr [edx]
        test   edx, edx
        jne    loc_1_176F
loc_1_1787:
        test   edx, edx
        jne    loc_1_17C1
        push   0x37
        call   sub_1_2540
        add    esp, 4
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_1_17A9
        mov    eax, 0x20
        jmp    loc_1_1818
loc_1_17A2:
        mov    eax, 7
        jmp    loc_1_1818
loc_1_17A9:
        push   ebx
        push   esi
        call   sub_1_169C
        add    esp, 8
        push   0
        push   ebx
        push   edi
        call   sub_1_2D7C
        add    esp, 0xc
        jmp    loc_1_1816
loc_1_17C1:
        mov    ebp, dword ptr [edx + 0xc]
        cmp    ecx, ebp
        jbe    loc_1_17EA
        mov    eax, dword ptr [edx + 0x10]
        add    eax, ebp
        cmp    eax, ecx
        jne    loc_1_17E3
        cmp    dword ptr [edx + 0x20], 0
        jne    loc_1_17E3
        push   edx
        push   esi
        call   sub_1_169C
        add    esp, 8
        jmp    loc_1_1816
loc_1_17E3:
        mov    eax, 7
        jmp    loc_1_1818
loc_1_17EA:
        push   0x37
        call   sub_1_2540
        add    esp, 4
        mov    ebp, eax
        mov    eax, 0x20
        test   ebp, ebp
        je     loc_1_1818
        push   ebp
        push   esi
        call   sub_1_169C
        add    esp, 8
        push   0
        push   ebp
        push   ebx
        push   edi
        call   sub_1_2DB5
        add    esp, 0x10
loc_1_1816:
        xor    eax, eax
loc_1_1818:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_181D:
        mov    eax, dword ptr [esp + 8]
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 0x2c]
        mov    eax, 0x1f
        test   esi, esi
        je     loc_1_1849
        mov    edi, dword ptr [0xa]          ; -> loc_1_199E
        push   edi
        push   esi
        call   sub_1_1530
        mov    ax, word ptr [esi + 0x26]
        add    esp, 8
        mov    word ptr [edi + 0x20], ax
        xor    eax, eax
loc_1_1849:
        pop    edi
        pop    esi
        ret    
loc_1_184C:
        mov    eax, dword ptr [esp + 8]
        push   esi
        push   edi
        mov    esi, dword ptr [0xa]          ; -> loc_1_199E
        mov    ecx, dword ptr [eax + 0x2c]
        test   ecx, ecx
        je     loc_1_1870
        mov    eax, dword ptr [esi + 8]
loc_1_1862:
        cmp    eax, ecx
        je     loc_1_186C
        mov    ecx, dword ptr [ecx]
        test   ecx, ecx
        jne    loc_1_1862
loc_1_186C:
        test   ecx, ecx
        jne    loc_1_1877
loc_1_1870:
        mov    eax, 0x1c
        jmp    loc_1_1896
loc_1_1877:
        mov    edi, dword ptr [ecx]
        mov    eax, 0x1f
        test   edi, edi
        je     loc_1_1896
        push   esi
        push   edi
        call   sub_1_1530
        mov    ax, word ptr [edi + 0x26]
        add    esp, 8
        mov    word ptr [esi + 0x20], ax
        xor    eax, eax
loc_1_1896:
        pop    edi
        pop    esi
        ret    
        db  0CCh, 0CCh, 0CCh
        dd  PCCARD_Get_Version       ; fixup
        dd  PCCARD_Card_Services     ; fixup
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  0097Ch                      ; DDB_Req_Device_Number
        db  4, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'PCCARD  '                  ; DDB_Name
        dd  016800000h                  ; DDB_Init_Order
        dd  PCCARD_Control           ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  PCCARD_PM_API            ; DDB_PM_API_Proc
        dd  obj1:0x189C              ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        dd  loc_1_0D63               ; fixup
        dd  loc_1_0E69               ; fixup
        dd  loc_1_0C7A               ; fixup
        dd  loc_1_0EA1               ; fixup
        dd  loc_1_0FA4               ; fixup
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_1980:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1984:
        add    byte ptr [eax], al
loc_1_1986:
        add    byte ptr [eax], al
loc_1_1988:
        add    byte ptr [eax], al
loc_1_198A:
        add    byte ptr [eax], al
loc_1_198C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1990:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1994:
        add    byte ptr [eax], al
loc_1_1996:
        add    byte ptr [eax], al
loc_1_1998:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_199C:
        add    byte ptr [eax], al
loc_1_199E:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_19A4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_19A8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_19B2:
        add    byte ptr [eax], al
loc_1_19B4:
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
loc_1_19D4:
        add    byte ptr [eax], al
loc_1_19D6:
        add    byte ptr [eax], al
loc_1_19D8:
        add    byte ptr [eax], al
loc_1_19DA:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_19DE:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_19E4:
        add    byte ptr [eax], al
loc_1_19E6:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_19EC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_19F0:
        add    byte ptr [eax], al
loc_1_19F2:
        add    byte ptr [eax], al
loc_1_19F4:
        add    byte ptr [eax], al
loc_1_19F6:
        add    byte ptr [eax], al
loc_1_19F8:
        add    byte ptr [eax], al
loc_1_19FA:
        add    byte ptr [eax], al
loc_1_19FC:
        add    byte ptr [eax], al
loc_1_19FE:
        add    byte ptr [eax], al
loc_1_1A00:
        add    byte ptr [eax], al
loc_1_1A02:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1A14:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1A18:
        add    byte ptr [eax], al
loc_1_1A1A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1A1E:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1A22:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1A26:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1A2A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1A2E:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1A32:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1A36:
        add    byte ptr [eax], al
loc_1_1A38:
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
loc_1_1A58:
        add    byte ptr [eax], al
loc_1_1A5A:
        add    byte ptr [eax], al
loc_1_1A5C:
        add    byte ptr [eax], al
loc_1_1A5E:
        add    byte ptr [eax], al
loc_1_1A60:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1A64:
        add    byte ptr [eax], al
loc_1_1A66:
        add    byte ptr [eax], al
loc_1_1A68:
        add    byte ptr [eax], al
loc_1_1A6A:
        add    byte ptr [eax], al
loc_1_1A6C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1A70:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1A74:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1A7C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1A80:
        add    byte ptr [eax], dl
        add    byte ptr [eax], al
loc_1_1A84:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1A88:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1A8C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1A90:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1A94:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1A98:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1A9C:
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
loc_1_1B9C:
        dec    ebp
        imul   esp, dword ptr [ebx + 0x72], 0x666f736f
        je     loc_1_1BC6
        inc    ebx
        outsd  dx, dword ptr [esi]
        jb     0x1c1a
        outsd  dx, dword ptr [esi]
        jb     loc_1_1C0E
        je     0x1c18
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        or     cl, byte ptr [0x79706f43]
        jb     0x1c22
        push   0x39312074
        cmp    dword ptr [0x6c410d0a], esi
        insb   byte ptr es:[edi], dx
loc_1_1BC6:
        and    byte ptr [edx + 0x69], dl
        push   0x52207374
        jae    loc_1_1C37
        jb     0x1c4a
        and    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1BDC:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_1_1BE0:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_1_1BE4:
        sub    al, 1
        add    byte ptr [eax], al
loc_1_1BE8:
        sub    al, 1
        add    byte ptr [eax], al
loc_1_1BEC:
        inc    dword ptr [eax]
        add    byte ptr [eax], al
loc_1_1BF0:
        add    byte ptr [eax], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edi + ecx*2 + 0x53], al
        add    byte ptr [esi + 0x6c], al
        popal  
        jae    loc_1_1C71
        add    byte ptr [eax], al
        add    byte ptr [esi + 0x46], al
loc_1_1C0E:
        push   ebx
        xor    dh, byte ptr [eax]
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
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1C37:
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
loc_1_1C71:
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
        add    byte ptr [eax], al
        add    byte ptr [eax], ch
loc_1_1E16:
        add    byte ptr [eax], al
loc_1_1E18:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1E1C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1E20:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1E24:
        add    byte ptr [eax], al
loc_1_1E26:
        add    byte ptr [eax], al
loc_1_1E28:
        add    byte ptr [eax], al
loc_1_1E2A:
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
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1E7C:
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        adc    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   cs
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_52E0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_53EB
        add    byte ptr [eax], al
        add    byte ptr [esi], al
        add    byte ptr [eax], al
        add    byte ptr [eax], bl
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_78BD
        add    byte ptr [eax], al
        add    byte ptr [0x18000000], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_181D
        add    byte ptr [eax], al
        add    byte ptr [edx], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], bl
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_159B
        add    byte ptr [eax], al
        add    byte ptr [eax], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], bl
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_3A86
        add    byte ptr [eax], al
        add    byte ptr [edx], dl
        add    byte ptr [eax], al
        add    byte ptr [eax], bl
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_184C
        add    byte ptr [eax], al
        add    byte ptr [edx], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], bl
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_15D6
        add    byte ptr [eax], al
        add    byte ptr [eax], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], bl
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_3A86
        add    byte ptr [eax], al
        add    byte ptr [edx], dl
        add    byte ptr [eax], al
        add    byte ptr [eax], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_4EBB
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], al
        add    byte ptr [eax], al
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   es
        add    byte ptr [eax], al
        add    byte ptr [eax], bh
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_3A86
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], dl
        add    byte ptr [eax], al
        pop    eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_537C
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], al
        add    al, byte ptr [eax]
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   cs
        add    byte ptr [eax], al
        add    byte ptr [ecx], bl            ; -> loc_1_980F
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], al
        add    byte ptr [eax], al
        or     byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        adc    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    eax, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_97D8
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        xchg   edx, eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_45C7
        add    byte ptr [eax], al
        add    byte ptr [ebx], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], bl
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_00D4
        add    byte ptr [eax], al
        add    byte ptr [eax], cl
        add    byte ptr [eax], al
        add    byte ptr [ebx], dl
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_0392
        add    byte ptr [eax], al
        add    byte ptr [eax], cl
        add    byte ptr [eax], al
        add    byte ptr [ecx], bl
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_08A7
        add    byte ptr [eax], al
        add    byte ptr [edx], cl
        add    byte ptr [eax], al
        add    byte ptr [ecx], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_93CA
        add    byte ptr [eax], al
        add    byte ptr [ebx], cl
        add    byte ptr [eax], al
        add    byte ptr [ecx], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_8886
        add    byte ptr [eax], al
        add    byte ptr [0x2000000], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> sub_1_4B4F
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_75EF
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], bl            ; -> loc_1_90D5
        add    byte ptr [eax], al
        add    byte ptr [ebx], cl
        add    byte ptr [eax], al
        add    byte ptr [ecx], bl            ; -> loc_1_86C7
        add    byte ptr [eax], al
        add    byte ptr [eax], cl
        add    byte ptr [eax], al
        add    byte ptr [ecx], bl            ; -> sub_1_48FD
        add    byte ptr [eax], al
        add    byte ptr [0x19000000], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_96BC
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        adc    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        sbb    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        pop    es
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_4C41
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], bl
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_16F3
        add    byte ptr [eax], al
        add    byte ptr [ecx], bl
        add    byte ptr [eax], al
        add    byte ptr [ecx], bl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, 0
        add    al, byte ptr [eax]
        sbb    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [eax], al
        sbb    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [eax], al
        or     dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [eax], al
        pop    ecx
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_82F4
        add    byte ptr [eax], al
        add    byte ptr [esi], al
        add    byte ptr [eax], al
        add    al, 9
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_96BC
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], bl            ; -> loc_1_73DB
        add    byte ptr [eax], al
        add    byte ptr [ecx], dl
        add    byte ptr [eax], al
        add    byte ptr [ecx], bl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   es
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_8AE1
        add    byte ptr [eax], al
        add    byte ptr [esi], al
        add    byte ptr [eax], al
        add    byte ptr [eax], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_8C09
        add    byte ptr [eax], al
        add    byte ptr [edx], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_9876
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], al
        add    byte ptr [eax], al
        and    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [eax], al
        cwde   
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> loc_1_7ACA
        add    byte ptr [eax], al
        add    byte ptr [0x80000000], al
        cmp    eax, 0                        ; -> obj5:0x0000
        add    byte ptr [ebp + 0x25], dh
        call   sub_6_0000
loc_1_211E:
        call   sub_3_5272
loc_1_2123:
        or     eax, eax
        je     loc_1_2129
        stc    
        ret    
loc_1_2129:
        mov    eax, 0                        ; -> obj1:0x18A4
        push   eax
        call   sub_3_0000
loc_1_2134:
        add    esp, 4
        mov    byte ptr [0], 1               ; -> obj5:0x0000
        clc    
        ret    
PCCARD_Get_Version:
        mov    eax, 0x400
        clc    
        ret    
PCCARD_Card_Services:
        xor    ah, ah
        mov    word ptr [0], ax              ; -> loc_1_1994
        mov    word ptr [2], dx              ; -> loc_1_1996
        mov    dword ptr [4], esi            ; -> loc_1_1998
        mov    word ptr [8], cx              ; -> loc_1_199C
        mov    dword ptr [0xa], ebx          ; -> loc_1_199E
        call   sub_1_4C9B
        mov    dx, word ptr [2]              ; -> loc_1_1996
        mov    esi, dword ptr [4]            ; -> loc_1_1998
        mov    cx, word ptr [8]              ; -> loc_1_199C
        mov    ebx, dword ptr [0xa]          ; -> loc_1_199E
        or     eax, eax
        je     loc_1_218D
        stc    
loc_1_218D:
        ret    
PCCARD_Control:
        cmp    eax, 1
        je     0x2110
        cmp    eax, 0x1b
        je     0x2110
        cmp    eax, 0x22
        jne    loc_1_21AE
        push   edx
        push   ebx
        call   sub_3_0019
loc_1_21AC:
        stc    
        ret    
loc_1_21AE:
        cmp    eax, 6
        jne    loc_1_21BC
        call   sub_3_02AC
loc_1_21B8:
        cmp    eax, 1
        ret    
loc_1_21BC:
        cmp    eax, 0x23
        je     loc_1_222C
        clc    
        ret    
PCCARD_PM_API:
        mov    ax, word ptr [ebp + 0x1c]
        test   ax, ax
        je     loc_1_220A
        cmp    ax, 0x100
        jne    loc_1_2205
        mov    ax, 0x3810
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        cmp    eax, -1
        je     loc_1_21FF
        movzx  ecx, word ptr [ebp + 0x18]
        push   ecx
        push   eax
        call   sub_3_204D
loc_1_21EC:
        add    esp, 8
        mov    word ptr [ebp + 0x18], ax
        shr    eax, 0x10
        and    byte ptr [ebp + 0x2c], 0xfe
        or     word ptr [ebp + 0x2c], ax
        ret    
loc_1_21FF:
        mov    word ptr [ebp + 0x18], 0
loc_1_2205:
        or     byte ptr [ebp + 0x2c], 1
        ret    
loc_1_220A:
        mov    word ptr [ebp + 0x1c], 0x400
        and    byte ptr [ebp + 0x2c], 0xfe
        ret    
        ; ---- dispatch table (3 entries) ----
        dd  loc_1_224B
        dd  loc_1_2265
        dd  loc_1_2286
loc_1_2221:
        inc    esp
        jbe    0x228e
        arpl   word ptr [ebp + 0x44], sp
        jae    0x228e
        db  000h
loc_1_222C:
        mov    eax, dword ptr [esi + 0xc]
        cmp    eax, 3
        jae    loc_1_223D
        jmp    dword ptr [eax*4]             ; -> obj1:0x2215
loc_1_223D:
        xor    eax, eax
        inc    eax
        clc    
        ret    
loc_1_2242:
        xor    eax, eax
        ret    
loc_1_2245:
        xor    eax, eax
        mov    al, 0x57
        clc    
        ret    
loc_1_224B:
        cmp    dword ptr [esi + 0x1c], 4
        jb     loc_1_2242
        mov    edi, dword ptr [esi + 0x18]
        mov    eax, dword ptr [esi + 0x20]
        mov    dword ptr [edi], 0x400
        mov    dword ptr [eax], 4
        jmp    loc_1_2242
loc_1_2265:
        push   dword ptr [esi + 0x1c]
        push   dword ptr [esi + 0x18]
        call   sub_3_204D
loc_1_2270:
        pop    ecx
        pop    ecx
        movzx  ecx, ax
        mov    edi, dword ptr [esi + 0x20]
        shl    ecx, 2
        mov    dword ptr [edi], ecx
        shr    eax, 0x10
        test   eax, eax
        je     loc_1_2242
        jmp    loc_1_2245
loc_1_2286:
        cmp    dword ptr [esi + 0x14], 4
        jne    loc_1_2245
        push   dword ptr [esi + 0x1c]
        mov    ebx, dword ptr [esi + 0x10]
        mov    eax, esp
        mov    ebx, dword ptr [ebx]
        push   0
        push   eax
        push   dword ptr [esi + 0x18]
        push   1
        push   0                             ; -> loc_1_2221
        push   0
        push   ebx
        VxDCall  CONFIGMG._CONFIGMG_Read_Registry_Value ; cd203e003300
        add    esp, 0x1c
        mov    edi, dword ptr [esi + 0x20]
        pop    dword ptr [edi]
        test   eax, eax
        je     loc_1_2242
        jmp    loc_1_2245
        db  0CDh, 020h, 001h, 000h, 00Bh, 000h, 0C3h, 003h, 0B6h, 000h, 000h, 000h
sub_1_22C6:
        push   ecx
        push   esi
        sub    esp, 4
        mov    esi, esp
        pushfd 
        cli    
        in     al, 0x61
        mov    byte ptr [esi], al
        and    al, 0xfc
        out    0x61, al
        mov    al, 0xe8
        out    0x43, al
        in     al, 0x42
        or     al, 0x80
        and    al, 0xbf
        mov    byte ptr [esi + 1], al
        or     al, 0x30
        out    0x43, al
        in     al, 0x42
        mov    byte ptr [esi + 2], al
        mov    al, byte ptr [edi + 2]
        out    0x42, al
        in     al, 0x42
        mov    byte ptr [esi + 3], al
        mov    al, byte ptr [edi + 3]
        out    0x42, al
        mov    al, byte ptr [edi + 1]
        out    0x43, al
        mov    ah, al
        test   ah, 0x10
        je     loc_1_230D
        mov    al, byte ptr [edi + 2]
        out    0x42, al
loc_1_230D:
        test   ah, 0x20
        je     loc_1_2317
        mov    al, byte ptr [edi + 3]
        out    0x42, al
loc_1_2317:
        mov    ah, byte ptr [edi]
        and    ah, 3
        in     al, 0x61
        and    al, 0xfc
        or     al, ah
        out    0x61, al
        popfd  
        mov    ecx, 4
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        add    esp, 4
        pop    esi
        pop    ecx
        sub    eax, eax
        ret    
sub_1_2335:
        push   eax
        push   ecx
        push   edx
        push   edi
        mov    edi, 0                        ; -> obj1:0x22C1
        call   sub_1_22C6
        mov    byte ptr [0], 0               ; -> obj1:0x22C5
        push   0                             ; -> obj1:0x22C1
        VMMCall  VMM._SignalID                   ; cd2027010100
        add    esp, 4
        pop    edi
        pop    edx
        pop    ecx
        pop    eax
        ret    
sub_1_235D:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edi
        push   esi
        push   es
        mov    edi, 0                        ; -> obj1:0x22C1
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [edi + 2], al
        mov    byte ptr [edi + 3], ah
        call   sub_1_22C6
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, 0
        lea    esi, [0]                      ; -> sub_1_2335
        VMMCall  VMM.Set_Async_Time_Out          ; cd20fe000100
        or     esi, esi
        jne    loc_1_2396
        call   sub_1_2335
        jmp    loc_1_23B5
loc_1_2396:
        mov    byte ptr [0], 1               ; -> obj1:0x22C5
        push   0
        push   0                             ; -> obj1:0x22C1
        VMMCall  VMM._BlockOnID                  ; cd20fa000100
        pop    ecx
        pop    ecx
        cmp    byte ptr [0], 0               ; -> obj1:0x22C5
        jne    loc_1_2396
loc_1_23B5:
        pop    es
        pop    esi
        pop    edi
        pop    ebx
        leave  
        ret    
        db  0CCh, 011h, 004h
loc_1_23BE:
        adc    byte ptr [edx], al
        db  001h, 005h
loc_1_23C2:
        push   eax
        call   sub_1_8129
        or     eax, eax
        pop    eax
        je     loc_1_23D5
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
        clc    
        ret    
loc_1_23D5:
        stc    
        ret    
loc_1_23D7:
        pushal 
        push   edx
        call   sub_1_25E9
        add    esp, 4
        clc    
        popal  
        ret    
loc_1_23E4:
        mov    word ptr [0], ax              ; -> obj1:0x196C
        mov    word ptr [2], bx              ; -> obj1:0x196E
        mov    dword ptr [4], ecx            ; -> obj1:0x1970
        mov    word ptr [8], dx              ; -> obj1:0x1974
        mov    dword ptr [0xe], edi          ; -> obj1:0x197A
        pushal 
        call   sub_1_5507
        popal  
        mov    ax, word ptr [0]              ; -> obj1:0x196C
        mov    bx, word ptr [2]              ; -> obj1:0x196E
        mov    ecx, dword ptr [4]            ; -> obj1:0x1970
        mov    dx, word ptr [8]              ; -> obj1:0x1974
        mov    esi, dword ptr [0xa]          ; -> obj1:0x1976
        mov    edi, dword ptr [0xe]          ; -> obj1:0x197A
        clc    
        or     ah, ah
        je     loc_1_2437
        stc    
loc_1_2437:
        ret    
sub_1_2438:
        pushal 
        mov    ax, word ptr [0]              ; -> obj1:0x196C
        mov    bx, word ptr [2]              ; -> obj1:0x196E
        mov    ecx, dword ptr [4]            ; -> obj1:0x1970
        mov    dx, word ptr [8]              ; -> obj1:0x1974
        mov    esi, dword ptr [0xa]          ; -> obj1:0x1976
        mov    edi, dword ptr [0xe]          ; -> obj1:0x197A
        mov    bp, word ptr [0x12]           ; -> obj1:0x197E
        call   sub_1_1980
        mov    word ptr [0], ax              ; -> obj1:0x196C
        mov    word ptr [2], bx              ; -> obj1:0x196E
        mov    dword ptr [4], ecx            ; -> obj1:0x1970
        mov    word ptr [8], dx              ; -> obj1:0x1974
        mov    dword ptr [0xa], esi          ; -> obj1:0x1976
        mov    dword ptr [0xe], edi          ; -> obj1:0x197A
        mov    word ptr [0x12], bp           ; -> obj1:0x197E
        movzx  eax, ah
        mov    dword ptr [esp + 0x1c], eax
        popal  
        ret    
sub_1_24A2:
        push   ebp
        mov    ebp, esp
        pushal 
        mov    ax, word ptr [ebp + 8]
        mov    cx, word ptr [ebp + 0xc]
        mov    dx, word ptr [ebp + 0x10]
        mov    ebx, dword ptr [ebp + 0x1c]
        mov    di, word ptr [ebx]
        mov    esi, dword ptr [ebx + 4]
        mov    ebx, dword ptr [ebp + 0x14]
        push   0                             ; -> loc_1_24CA
        push   dword ptr [ebp + 0x20]
        mov    ebp, dword ptr [ebp + 0x18]
        ret    
loc_1_24CA:
        movzx  eax, ax
        mov    dword ptr [esp + 0x1c], eax
        popal  
        leave  
        ret    
loc_1_24D4:
        ret    
loc_1_24D5:
        pushal 
        mov    word ptr [0], ax              ; -> loc_1_1984
        mov    word ptr [2], bx              ; -> loc_1_1986
        mov    word ptr [4], cx              ; -> loc_1_1988
        mov    word ptr [6], dx              ; -> loc_1_198A
        mov    dword ptr [8], esi            ; -> loc_1_198C
        mov    dword ptr [0xc], edi          ; -> loc_1_1990
        movzx  ebx, ax
        shl    ebx, 2
        cmp    ebx, 0x14
        jae    loc_1_2516
        call   dword ptr [ebx]               ; -> obj1:0x18F4
        or     ax, ax
        jne    loc_1_2517
loc_1_2516:
        stc    
loc_1_2517:
        popal  
        mov    ax, word ptr [0]              ; -> loc_1_1984
        mov    bx, word ptr [2]              ; -> loc_1_1986
        mov    cx, word ptr [4]              ; -> loc_1_1988
        mov    dx, word ptr [6]              ; -> loc_1_198A
        mov    esi, dword ptr [8]            ; -> loc_1_198C
        mov    edi, dword ptr [0xc]          ; -> loc_1_1990
        ret    
sub_1_2540:
        push   1
        push   dword ptr [esp + 8]
        call   sub_3_53A0
loc_1_254B:
        add    esp, 8
        ret    
sub_1_254F:
        push   0
        push   dword ptr [esp + 8]
        call   sub_3_53A8
loc_1_255A:
        add    esp, 8
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 0x19
        ret    
sub_1_2566:
        push   esi
        push   0xe
        call   sub_1_2540
        add    esp, 4
        mov    esi, eax
        test   esi, esi
        je     loc_1_25AB
        mov    eax, dword ptr [esp + 8]
        mov    ecx, dword ptr [esp + 0x10]
        push   esi
        mov    dword ptr [esi], eax
        mov    dword ptr [esi + 4], ecx
        push   dword ptr [esp + 0x10]
        push   0                             ; -> loc_1_23D7
        mov    word ptr [esi + 0xc], 1
        call   sub_3_53B0
loc_1_2599:
        mov    dword ptr [esi + 8], eax
        test   eax, eax
        mov    eax, esi
        jne    loc_1_25AD
        push   esi
        call   sub_1_254F
        add    esp, 4
loc_1_25AB:
        xor    eax, eax
loc_1_25AD:
        pop    esi
        ret    
sub_1_25AF:
        push   esi
        mov    esi, dword ptr [esp + 8]
        movzx  eax, word ptr [esi + 0xc]
        cmp    eax, 1
        jl     loc_1_25DE
        cmp    eax, 2
        jle    loc_1_25C9
        cmp    eax, 3
        je     loc_1_25D3
        jmp    loc_1_25DE
loc_1_25C9:
        push   dword ptr [esi + 8]
        call   sub_3_53CC
loc_1_25D1:
        jmp    loc_1_25DE
loc_1_25D3:
        push   dword ptr [esi + 8]
        call   sub_1_9880
        add    esp, 4
loc_1_25DE:
        push   esi
        call   sub_1_254F
        add    esp, 4
        pop    esi
        ret    
sub_1_25E9:
        push   esi
        mov    esi, dword ptr [esp + 8]
        movzx  eax, word ptr [esi + 0xc]
        cmp    eax, 1
        je     loc_1_2603
        cmp    eax, 2
        jl     loc_1_2633
        cmp    eax, 3
        jle    loc_1_2622
        jmp    loc_1_2633
loc_1_2603:
        mov    word ptr [esi + 0xc], 3
        push   1
        push   esi
        push   0                             ; -> sub_1_25E9
        call   sub_1_9888
        add    esp, 0xc
        mov    dword ptr [esi + 8], eax
        test   eax, eax
        jne    loc_1_2633
        jmp    loc_1_262A
loc_1_2622:
        push   dword ptr [esi + 4]
        call   dword ptr [esi]
        add    esp, 4
loc_1_262A:
        push   esi
        call   sub_1_254F
        add    esp, 4
loc_1_2633:
        pop    esi
        ret    
sub_1_2635:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 0x18]
        test   eax, eax
        je     loc_1_264F
        mov    ecx, dword ptr [esp + 8]
loc_1_2644:
        cmp    dword ptr [eax + 0xc], ecx
        je     loc_1_264F
        mov    eax, dword ptr [eax]
        test   eax, eax
        jne    loc_1_2644
loc_1_264F:
        ret    
sub_1_2650:
        test   dword ptr [esp + 4], 0xfff
        mov    eax, 0xffffffff
        jne    loc_1_2663
        mov    eax, dword ptr [esp + 4]
loc_1_2663:
        ret    
sub_1_2664:
        test   dword ptr [esp + 4], 0xfc000fff
        mov    eax, 0xffffffff
        jne    loc_1_2677
        mov    eax, dword ptr [esp + 4]
loc_1_2677:
        ret    
sub_1_2678:
        mov    edx, dword ptr [esp + 8]
        test   edx, 0xfff
        je     loc_1_268B
        mov    eax, 0xffffffff
        jmp    loc_1_269D
loc_1_268B:
        mov    eax, dword ptr [esp + 4]
        lea    ecx, [eax + edx]
        cmp    ecx, eax
        mov    eax, 0xffffffff
        jb     loc_1_269D
        mov    eax, edx
loc_1_269D:
        ret    
sub_1_269E:
        mov    ecx, dword ptr [0]            ; -> obj1:0x1918
        test   ecx, ecx
        je     loc_1_26BB
        mov    al, byte ptr [esp + 4]
loc_1_26AC:
        cmp    byte ptr [ecx + 0xf], al
        je     loc_1_26B7
        mov    ecx, dword ptr [ecx]
        test   ecx, ecx
        jne    loc_1_26AC
loc_1_26B7:
        test   ecx, ecx
        jne    loc_1_26C2
loc_1_26BB:
        mov    eax, 1
        jmp    loc_1_26CA
loc_1_26C2:
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax], ecx
        xor    eax, eax
loc_1_26CA:
        ret    
sub_1_26CB:
        mov    al, byte ptr [esp + 4]
        mov    cl, al
        and    cl, 7
        test   al, 0x78
        jne    loc_1_270B
        movzx  ecx, cl
        cmp    ecx, 1
        je     loc_1_26F1
        cmp    ecx, 2
        je     loc_1_26F5
        cmp    ecx, 3
        je     loc_1_26F9
        cmp    ecx, 4
        je     loc_1_26FD
        jmp    loc_1_270B
loc_1_26F1:
        mov    cl, 6
        jmp    loc_1_26FF
loc_1_26F5:
        mov    cl, 5
        jmp    loc_1_26FF
loc_1_26F9:
        mov    cl, 4
        jmp    loc_1_26FF
loc_1_26FD:
        mov    cl, 1
loc_1_26FF:
        and    al, 0x80
        and    cl, 0xf
        shl    cl, 3
        or     al, cl
        or     al, 2
loc_1_270B:
        ret    
sub_1_270C:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        cmp    byte ptr [ebp + 0xc], 0x80
        push   ebx
        jne    loc_1_2724
        cmp    dword ptr [ebp + 8], 0
        je     loc_1_2724
        mov    al, byte ptr [ebp + 0x10]
        jmp    loc_1_27A0
loc_1_2724:
        push   dword ptr [ebp + 0xc]
        call   sub_1_26CB
        add    esp, 4
        mov    bl, al
        push   dword ptr [ebp + 0x10]
        call   sub_1_26CB
        add    esp, 4
        mov    byte ptr [ebp - 1], al
        push   dword ptr [ebp + 0x14]
        call   sub_1_26CB
        add    esp, 4
        mov    cl, bl
        and    cl, 7
        and    bl, 0x78
        shr    bl, 3
        mov    dl, byte ptr [ebp - 1]
        and    dl, 7
        mov    byte ptr [ebp - 2], dl
        mov    dl, byte ptr [ebp - 1]
        and    dl, 0x78
        shr    dl, 3
        mov    byte ptr [ebp - 3], dl
        mov    dl, al
        and    dl, 7
        and    al, 0x78
        shr    al, 3
        mov    byte ptr [ebp - 1], dl
        test   bl, bl
        je     loc_1_279E
        cmp    byte ptr [ebp - 2], cl
        jb     loc_1_279E
        cmp    dl, cl
        ja     loc_1_279E
        cmp    byte ptr [ebp - 2], cl
        jne    loc_1_278E
        cmp    byte ptr [ebp - 3], bl
        jb     loc_1_279E
loc_1_278E:
        cmp    byte ptr [ebp - 1], cl
        jne    loc_1_2797
        cmp    al, bl
        ja     loc_1_279E
loc_1_2797:
        mov    al, byte ptr [ebp + 0xc]
        and    al, 0x7f
        jmp    loc_1_27A0
loc_1_279E:
        xor    al, al
loc_1_27A0:
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_27A5:
        mov    ecx, dword ptr [esp + 4]
        test   ecx, ecx
        jne    loc_1_27B1
        xor    eax, eax
        jmp    loc_1_27C0
loc_1_27B1:
        mov    eax, 1
loc_1_27B6:
        cmp    eax, ecx
        jae    loc_1_27C0
        add    eax, eax
        test   eax, eax
        jne    loc_1_27B6
loc_1_27C0:
        ret    
sub_1_27C1:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        lea    edi, [esi + 0xc]
        mov    eax, dword ptr [edi]
        mov    ecx, dword ptr [eax + 0x18]
        mov    dword ptr [0x14], ecx         ; -> sub_1_1980
        mov    eax, dword ptr [edi]
        mov    edx, dword ptr [eax + 0x1c]
        mov    byte ptr [1], 0x8f            ; -> obj1:0x196D
        mov    dword ptr [0xa], edx          ; -> obj1:0x1976
        mov    eax, dword ptr [edi]
        mov    al, byte ptr [eax + 0xc]
        mov    byte ptr [0], al              ; -> obj1:0x196C
        mov    cl, byte ptr [esi + 0x12]
        mov    byte ptr [2], cl              ; -> obj1:0x196E
        call   sub_1_2438
        test   eax, eax
        jne    loc_1_2960
        movzx  ax, byte ptr [3]              ; -> obj1:0x196F
        mov    word ptr [ebp - 2], ax
        movzx  cx, byte ptr [9]              ; -> obj1:0x1975
        mov    word ptr [ebp - 4], cx
        mov    cx, ax
        and    cx, 0x80
        je     loc_1_284C
        test   byte ptr [0xf], 3             ; -> obj1:0x197B
        jne    loc_1_2848
        mov    eax, dword ptr [edi]
        cmp    word ptr [eax + 0x26], 0x500
        jb     loc_1_2848
        or     byte ptr [esi + 0x17], 0x10
        jmp    loc_1_284C
loc_1_2848:
        and    byte ptr [esi + 0x17], 0xef
loc_1_284C:
        mov    ax, word ptr [0xe]            ; -> obj1:0x197A
        and    ah, 3
        cmp    ah, 2
        jne    loc_1_295E
        test   cx, cx
        je     loc_1_295E
        and    word ptr [ebp - 2], 0xffce
        and    word ptr [ebp - 4], 0xff8e
        or     byte ptr [ebp - 2], 0x40
        test   byte ptr [esi + 0x17], 2
        mov    edi, dword ptr [esi + 0x18]
        je     loc_1_294D
        test   byte ptr [edi + 8], 4
        je     loc_1_294D
        lea    eax, [ebp - 8]
        push   eax
        push   esi
        call   sub_1_4432
        add    esp, 8
        test   eax, eax
        je     loc_1_28A9
        mov    eax, 0x19
        jmp    loc_1_2960
loc_1_28A9:
        lea    eax, [ebp - 0xc]
        push   eax
        push   dword ptr [edi + 0xc]
        push   dword ptr [ebp - 8]
        call   sub_1_450F
        add    esp, 0xc
        test   eax, eax
        je     loc_1_28D4
        push   dword ptr [ebp - 8]
        call   sub_1_44FF
        add    esp, 4
        mov    eax, 0x19
        jmp    loc_1_2960
loc_1_28D4:
        mov    eax, dword ptr [ebp - 0xc]
        push   dword ptr [ebp - 8]
        mov    bl, byte ptr [eax + 4]
        call   sub_1_44FF
        add    esp, 4
        test   eax, eax
        je     loc_1_28F0
        mov    eax, 0x19
        jmp    loc_1_2960
loc_1_28F0:
        mov    al, byte ptr [edi + 0x11]
        test   al, 0x11
        je     loc_1_2909
        test   bl, 1
        je     loc_1_2900
        or     byte ptr [ebp - 2], 1
loc_1_2900:
        test   bl, 0x10
        je     loc_1_2909
        or     byte ptr [ebp - 4], 1
loc_1_2909:
        test   al, 0x22
        je     loc_1_2921
        test   bl, 2
        jne    loc_1_2918
        and    word ptr [ebp - 2], 0xffbf
loc_1_2918:
        test   bl, 0x20
        je     loc_1_2921
        or     byte ptr [ebp - 4], 0x40
loc_1_2921:
        test   al, 0x44
        je     loc_1_2937
        test   bl, 4
        je     loc_1_292E
        or     byte ptr [ebp - 2], 0x20
loc_1_292E:
        test   bl, 0x40
        je     loc_1_2937
        or     byte ptr [ebp - 4], 0x20
loc_1_2937:
        test   al, 0x88
        je     loc_1_294D
        test   bl, 8
        je     loc_1_2944
        or     byte ptr [ebp - 2], 0x10
loc_1_2944:
        test   bl, 0x80
        je     loc_1_294D
        or     byte ptr [ebp - 4], 0x10
loc_1_294D:
        mov    al, byte ptr [ebp - 2]
        mov    cl, byte ptr [ebp - 4]
        mov    byte ptr [3], al              ; -> obj1:0x196F
        mov    byte ptr [9], cl              ; -> obj1:0x1975
loc_1_295E:
        xor    eax, eax
loc_1_2960:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_2967:
        push   ecx
        call   sub_1_27C1
        add    esp, 4
        test   eax, eax
        mov    ax, 0
        jne    loc_1_2980
        movzx  ax, byte ptr [3]              ; -> obj1:0x196F
loc_1_2980:
        ret    
sub_1_2981:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        lea    ecx, [esi + 0xc]
        mov    eax, dword ptr [ecx]
        mov    edx, dword ptr [eax + 0x18]
        mov    dword ptr [0x14], edx         ; -> sub_1_1980
        mov    eax, dword ptr [ecx]
        mov    edi, dword ptr [eax + 0x1c]
        mov    dword ptr [0xa], edi          ; -> obj1:0x1976
        mov    eax, dword ptr [ecx]
        mov    al, byte ptr [eax + 0xc]
        mov    byte ptr [0], al              ; -> obj1:0x196C
        mov    cl, byte ptr [esi + 0x12]
        mov    byte ptr [1], 0x8d            ; -> obj1:0x196D
        mov    byte ptr [2], cl              ; -> obj1:0x196E
        call   sub_1_2438
        test   eax, eax
        jne    loc_1_2A69
        mov    byte ptr [1], 0x8e            ; -> obj1:0x196D
        mov    byte ptr [9], 0xff            ; -> obj1:0x1975
        call   sub_1_2438
        test   eax, eax
        jne    loc_1_2A69
        cmp    dword ptr [ebp + 0xc], 0
        mov    edi, dword ptr [esi + 0x18]
        je     loc_1_2A67
        mov    ax, word ptr [0xe]            ; -> obj1:0x197A
        and    ah, 3
        cmp    ah, 1
        je     loc_1_2A67
        test   byte ptr [esi + 0x17], 2
        je     loc_1_2A67
        test   byte ptr [edi + 8], 4
        je     loc_1_2A67
        lea    eax, [ebp - 4]
        push   eax
        push   esi
        call   sub_1_4432
        add    esp, 8
        test   eax, eax
        je     loc_1_2A22
        mov    eax, 0x19
        jmp    loc_1_2A69
loc_1_2A22:
        lea    eax, [ebp - 8]
        push   eax
        push   dword ptr [edi + 0xc]
        push   dword ptr [ebp - 4]
        call   sub_1_450F
        add    esp, 0xc
        test   eax, eax
        je     loc_1_2A4A
        push   dword ptr [ebp - 4]
        call   sub_1_44FF
        add    esp, 4
        mov    eax, 0x19
        jmp    loc_1_2A69
loc_1_2A4A:
        mov    cl, byte ptr [edi + 0x11]
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 4], cl
        push   dword ptr [ebp - 4]
        call   sub_1_44FF
        add    esp, 4
        test   eax, eax
        mov    eax, 0x19
        jne    loc_1_2A69
loc_1_2A67:
        xor    eax, eax
loc_1_2A69:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_2A6F:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        pushfd 
        pop    dword ptr [ebp - 4]
        cli    
        mov    byte ptr [0], 0               ; -> loc_1_1BF0
        xor    eax, eax
loc_1_2A83:
        push   eax
        call   sub_1_4FF6
        add    esp, 4
        test   eax, eax
        je     loc_1_2AA1
        cmp    byte ptr [eax + 0x20], 2
        je     loc_1_2A83
        mov    cl, byte ptr [eax + 0xa]
        or     byte ptr [0], cl              ; -> loc_1_1BF0
        jmp    loc_1_2A83
loc_1_2AA1:
        push   dword ptr [ebp - 4]
        popfd  
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_2AA9:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0x10]
        mov    edi, dword ptr [ebp + 8]
        lea    ebx, [edi + 8]
        mov    dword ptr [esi], 0
        mov    eax, dword ptr [ebx]
        mov    ecx, dword ptr [eax + 0x18]
        mov    dword ptr [0x14], ecx         ; -> sub_1_1980
        mov    eax, dword ptr [ebx]
        mov    edx, dword ptr [eax + 0x1c]
        mov    byte ptr [1], 0x87            ; -> obj1:0x196D
        mov    dword ptr [0xa], edx          ; -> obj1:0x1976
        mov    eax, dword ptr [ebx]
        mov    al, byte ptr [eax + 0xc]
        mov    byte ptr [3], 0xff            ; -> obj1:0x196F
        mov    byte ptr [0], al              ; -> obj1:0x196C
        mov    cl, byte ptr [edi + 0x10]
        mov    dword ptr [0xe], 0            ; -> obj1:0x197A
        mov    byte ptr [9], cl              ; -> obj1:0x1975
        call   sub_1_2438
        test   eax, eax
        jne    loc_1_2B4A
        movzx  ecx, byte ptr [2]             ; -> obj1:0x196E
        mov    edi, dword ptr [ebp + 0xc]
        test   edi, ecx
        je     loc_1_2CBF
        test   edi, 3
        jne    loc_1_2CB9
        test   edi, 4
        je     loc_1_2CBF
        test   byte ptr [2], 3               ; -> obj1:0x196E
        je     loc_1_2CB9
        mov    dword ptr [esi], 0x24         ; -> loc_1_1A38
        jmp    loc_1_2CBF
loc_1_2B4A:
        cmp    eax, 0x11
        jne    loc_1_2CBF
        mov    eax, dword ptr [ebx]
        mov    ecx, dword ptr [eax + 0x18]
        mov    dword ptr [0x14], ecx         ; -> sub_1_1980
        mov    eax, dword ptr [ebx]
        mov    edx, dword ptr [eax + 0x1c]
        mov    byte ptr [1], 0x87            ; -> obj1:0x196D
        mov    dword ptr [0xa], edx          ; -> obj1:0x1976
        mov    eax, dword ptr [ebx]
        mov    al, byte ptr [eax + 0xc]
        mov    byte ptr [0], al              ; -> obj1:0x196C
        mov    cl, byte ptr [edi + 0x10]
        mov    dword ptr [0xe], 0            ; -> obj1:0x197A
        mov    byte ptr [3], cl              ; -> obj1:0x196F
        call   sub_1_2438
        test   eax, eax
        jne    loc_1_2CBF
        mov    edx, 4                        ; -> loc_1_19F0
        mov    edi, dword ptr [ebp + 0xc]
        movzx  ecx, byte ptr [2]             ; -> obj1:0x196E
        test   edi, ecx
        je     loc_1_2CBF
        test   edi, 3
        je     loc_1_2C55
        mov    cx, word ptr [4]              ; -> loc_1_19F0
        mov    word ptr [4], cx              ; -> loc_1_1A18
        movzx  ecx, word ptr [6]             ; -> loc_1_19F2
        shl    ecx, 0xc
        mov    dword ptr [6], ecx            ; -> loc_1_1A1A
        movzx  ecx, word ptr [8]             ; -> loc_1_19F4
        shl    ecx, 0xc
        mov    dword ptr [0xa], ecx          ; -> loc_1_1A1E
        movzx  ecx, word ptr [0xa]           ; -> loc_1_19F6
        shl    ecx, 0xc
        mov    dword ptr [0xe], ecx          ; -> loc_1_1A22
        movzx  ecx, word ptr [0xc]           ; -> loc_1_19F8
        shl    ecx, 0xc
        mov    dword ptr [0x12], ecx         ; -> loc_1_1A26
        movzx  ecx, word ptr [0xe]           ; -> loc_1_19FA
        shl    ecx, 0xc
        mov    dword ptr [0x16], ecx         ; -> loc_1_1A2A
        movzx  ecx, word ptr [0x10]          ; -> loc_1_19FC
        shl    ecx, 0xc
        mov    dword ptr [0x1a], ecx         ; -> loc_1_1A2E
        movzx  ecx, word ptr [0x12]          ; -> loc_1_19FE
        shl    ecx, 0xc
        mov    dword ptr [0x1e], ecx         ; -> loc_1_1A32
        mov    dl, byte ptr [0x14]           ; -> loc_1_1A00
        mov    byte ptr [0x22], dl           ; -> loc_1_1A36
        mov    bl, byte ptr [0x15]           ; -> obj1:0x1A01
        mov    byte ptr [0x23], bl           ; -> obj1:0x1A37
        jmp    loc_1_2CB9
loc_1_2C55:
        test   edi, 4
        je     loc_1_2CBF
        test   byte ptr [2], 3               ; -> obj1:0x196E
        je     loc_1_2C6B
        mov    edx, 0x16                     ; -> loc_1_1A02
loc_1_2C6B:
        mov    cx, word ptr [edx]
        mov    word ptr [4], cx              ; -> loc_1_1A18
        movzx  ebx, word ptr [edx + 2]
        mov    dword ptr [6], ebx            ; -> loc_1_1A1A
        movzx  edi, word ptr [edx + 4]
        mov    dword ptr [0xa], edi          ; -> loc_1_1A1E
        movzx  ecx, word ptr [edx + 6]
        mov    dword ptr [0xe], ecx          ; -> loc_1_1A22
        movzx  ebx, word ptr [edx + 8]
        mov    dword ptr [0x12], ebx         ; -> loc_1_1A26
        movzx  edi, word ptr [edx + 0xa]
        mov    dword ptr [0x16], edi         ; -> loc_1_1A2A
        mov    cl, byte ptr [edx + 0xc]
        mov    byte ptr [0x1a], cl           ; -> loc_1_1A2E
        mov    dl, byte ptr [edx + 0xd]
        mov    byte ptr [0x1b], dl           ; -> obj1:0x1A2F
loc_1_2CB9:
        mov    dword ptr [esi], 4            ; -> loc_1_1A18
loc_1_2CBF:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
sub_1_2CC4:
        push   esi
        xor    esi, esi
loc_1_2CC7:
        push   0
        push   0x186a0
        call   sub_3_5308
loc_1_2CD3:
        mov    ecx, dword ptr [esp + 0x10]
        add    esp, 8
        call   sub_1_2967
        test   al, 0x80
        je     loc_1_2CED
        test   al, 0x40
        jne    loc_1_2CF4
        inc    esi
        cmp    esi, 0x1e
        jb     loc_1_2CC7
loc_1_2CED:
        mov    eax, 0x19
loc_1_2CF2:
        pop    esi
        ret    
loc_1_2CF4:
        xor    eax, eax
        jmp    loc_1_2CF2
sub_1_2CF8:
        mov    ecx, dword ptr [esp + 4]
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        push   edi
        lea    edx, [ecx + 0xc]
        mov    dword ptr [esi], 0
        mov    eax, dword ptr [edx]
        mov    eax, dword ptr [eax + 0x18]
        mov    dword ptr [0x14], eax         ; -> sub_1_1980
        mov    eax, dword ptr [edx]
        mov    eax, dword ptr [eax + 0x1c]
        mov    byte ptr [1], 0x8c            ; -> obj1:0x196D
        mov    dword ptr [0xa], eax          ; -> obj1:0x1976
        mov    edx, dword ptr [edx]
        mov    al, byte ptr [edx + 0xc]
        mov    byte ptr [0], al              ; -> obj1:0x196C
        mov    cl, byte ptr [ecx + 0x12]
        mov    dword ptr [0xe], 0            ; -> obj1:0x197A
        mov    byte ptr [2], cl              ; -> obj1:0x196E
        call   sub_1_2438
        test   eax, eax
        jne    loc_1_2D54
        mov    ecx, dword ptr [0xa]          ; -> loc_1_19DE
        mov    dword ptr [esi], ecx
loc_1_2D54:
        pop    edi
        pop    esi
        ret    
sub_1_2D57:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        pushfd 
        pop    dword ptr [ebp - 4]
        cli    
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    esi, dword ptr [eax]
        mov    dword ptr [ecx + edx], esi
        mov    dword ptr [eax], ecx
        push   dword ptr [ebp - 4]
        popfd  
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_2D7C:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        pushfd 
        pop    dword ptr [ebp - 4]
        cli    
        mov    ecx, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [ebp + 0x10]
        xor    eax, eax
        mov    esi, dword ptr [ebp + 8]
        sub    esi, edx
        mov    dword ptr [edx + ecx], eax
        je     loc_1_2DAC
loc_1_2D9A:
        add    esi, edx
        mov    eax, dword ptr [esi]
        test   eax, eax
        je     loc_1_2DAA
        mov    esi, eax
        test   esi, esi
        jne    loc_1_2D9A
        jmp    loc_1_2DAC
loc_1_2DAA:
        mov    dword ptr [esi], ecx
loc_1_2DAC:
        push   dword ptr [ebp - 4]
        popfd  
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_2DB5:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        test   eax, eax
        je     loc_1_2DC7
        mov    ecx, dword ptr [ebp + 0x14]
        add    ecx, eax
        jmp    loc_1_2DCA
loc_1_2DC7:
        mov    ecx, dword ptr [ebp + 8]
loc_1_2DCA:
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   ecx
        call   sub_1_2D57
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_2DDA:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        pushfd 
        pop    dword ptr [ebp - 4]
        cli    
        mov    esi, dword ptr [ebp + 8]
        xor    edi, edi
        xor    eax, eax
        mov    ecx, dword ptr [esi]
        test   ecx, ecx
        je     loc_1_2E01
        mov    eax, ecx
        mov    ecx, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [eax + ecx]
        mov    dword ptr [esi], edx
        mov    dword ptr [eax + ecx], edi
loc_1_2E01:
        push   dword ptr [ebp - 4]
        popfd  
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_2E0B:
        push   ebp
        mov    ecx, dword ptr [esp + 0x10]
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    eax, dword ptr [ebp + 0xc]
        push   edi
        lea    esi, [eax + ecx]
        pushfd 
        pop    dword ptr [ebp - 4]
        cli    
        mov    edi, dword ptr [ebp + 8]
        sub    edi, ecx
        je     loc_1_2E3D
loc_1_2E29:
        add    edi, ecx
        mov    edx, dword ptr [edi]
        cmp    eax, edx
        je     loc_1_2E39
        mov    edi, edx
        test   edi, edi
        jne    loc_1_2E29
        jmp    loc_1_2E3D
loc_1_2E39:
        mov    eax, dword ptr [esi]
        mov    dword ptr [edi], eax
loc_1_2E3D:
        push   dword ptr [ebp - 4]
        popfd  
        pop    edi
        mov    dword ptr [esi], 0
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_2E4D:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        mov    ecx, dword ptr [4]            ; -> loc_1_1A88
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [edi + 0xc]
        add    esi, dword ptr [ebp + 0xc]
        test   byte ptr [ecx + 0x17], 0x10
        jne    loc_1_2F86
        cmp    dword ptr [8], 0              ; -> loc_1_1A8C
        jne    loc_1_2E8E
        push   8                             ; -> loc_1_1A8C
        push   ecx
        call   sub_1_4432
        add    esp, 8
        test   eax, eax
        jne    loc_1_3108
loc_1_2E8E:
        xor    edx, edx
        add    edi, 6
        mov    ebx, dword ptr [8]            ; -> loc_1_1A8C
        test   byte ptr [edi], 5
        je     loc_1_2EC0
        add    ebx, 0x14
        mov    cx, word ptr [ebx]
        mov    ax, cx
        and    al, 0xa
        cmp    al, 8
        je     loc_1_2EE6
        and    cx, 0xfffd
        mov    word ptr [ebx], cx
        mov    eax, dword ptr [8]            ; -> loc_1_1A8C
        or     byte ptr [eax + 0x14], 8
        jmp    loc_1_2EE1
loc_1_2EC0:
        add    ebx, 0x14
        mov    ax, word ptr [ebx]
        mov    cx, ax
        and    cl, 0xa
        cmp    cl, 2
        je     loc_1_2EE6
        or     al, 2
        mov    word ptr [ebx], ax
        mov    eax, dword ptr [8]            ; -> loc_1_1A8C
        and    word ptr [eax + 0x14], 0xfff7
loc_1_2EE1:
        mov    edx, 1
loc_1_2EE6:
        test   edx, edx
        je     loc_1_2EFA
        mov    eax, esi
        mov    ecx, dword ptr [8]            ; -> loc_1_1A8C
        and    eax, 0xfffff000
        mov    dword ptr [ecx + 0x16], eax
loc_1_2EFA:
        mov    eax, dword ptr [8]            ; -> loc_1_1A8C
        push   eax
        call   sub_1_4053
        add    esp, 4
        test   eax, eax
        jne    loc_1_3108
        mov    eax, dword ptr [8]            ; -> loc_1_1A8C
        push   eax
        call   sub_1_4154
        add    esp, 4
        test   eax, eax
        jne    loc_1_3108
        test   byte ptr [edi], 4
        lea    eax, [ebp - 4]
        push   eax
        je     loc_1_2F65
        push   2
        push   dword ptr [8]                 ; -> loc_1_1A8C
        call   sub_1_450F
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_3108
        xor    ecx, ecx
        mov    eax, dword ptr [ebp - 4]
        mov    cl, byte ptr [edi]
        and    ecx, 1
        mov    word ptr [eax], cx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 2], esi
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 6]
        jmp    loc_1_310A
loc_1_2F65:
        push   esi
        push   dword ptr [8]                 ; -> loc_1_1A8C
        call   sub_1_450F
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_3108
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax]
        jmp    loc_1_310A
loc_1_2F86:
        cmp    dword ptr [0x10], esi         ; -> loc_1_1A94
        ja     loc_1_2FBE
        mov    eax, dword ptr [0x14]         ; -> loc_1_1A98
        add    eax, dword ptr [0x10]         ; -> loc_1_1A94
        cmp    eax, esi
        jbe    loc_1_2FBE
        mov    ax, word ptr [edi + 6]
        xor    ax, word ptr [0]              ; -> loc_1_1A84
        test   ah, 7
        jne    loc_1_2FBE
        sub    esi, dword ptr [0x10]         ; -> loc_1_1A94
        mov    al, byte ptr [esi + 0x18]     ; -> loc_1_1A9C
        jmp    loc_1_310A
loc_1_2FBE:
        movzx  ecx, byte ptr [edi + 1]
        mov    eax, dword ptr [4]            ; -> loc_1_1A88
        imul   ecx, ecx, 0x28
        mov    edx, dword ptr [eax + 0x18]
        xor    eax, eax
        mov    ebx, dword ptr [ecx + edx + 4]
        xor    ecx, ecx
        mov    edx, 0x700
        mov    dword ptr [0x14], ecx         ; -> loc_1_1A98
        and    byte ptr [1], 0xf8            ; -> obj1:0x1A85
        mov    ax, word ptr [edi + 6]
        and    eax, edx
        mov    dword ptr [0x10], esi         ; -> loc_1_1A94
        or     dword ptr [0], eax            ; -> loc_1_1A84
        mov    ax, word ptr [edi + 6]
        and    ax, dx
        jne    loc_1_3035
        mov    eax, esi
        push   ecx
        and    al, 0xfc
        push   4
        push   0x18                          ; -> loc_1_1A9C
        mov    dword ptr [0x10], eax         ; -> loc_1_1A94
        push   eax
        push   ecx
        push   ebx
        call   sub_3_52A0
loc_1_301B:
        add    esp, 0x18
        test   eax, eax
        jne    loc_1_3108
        mov    dword ptr [0x14], 4           ; -> loc_1_1A98
        jmp    loc_1_30E3
loc_1_3035:
        cmp    ax, 0x600
        push   0
        push   4
        ja     loc_1_3097
        movzx  eax, ax
        lea    ecx, [ebp - 0xc]
        dec    eax
        sar    eax, 6
        push   ecx
        and    al, 0xfc
        add    eax, 0x10
        push   eax
        push   0
        push   ebx
        call   sub_3_52A0
loc_1_3058:
        add    esp, 0x18
        test   eax, eax
        jne    loc_1_3108
        push   0
        mov    eax, dword ptr [ebp - 0xc]
        push   0x100
        and    al, 0xf0
        add    eax, esi
        push   eax
        call   sub_3_5340
loc_1_3077:
        add    esp, 0xc
        mov    dword ptr [ebp - 4], eax
        xor    eax, eax
loc_1_307F:
        mov    ecx, dword ptr [ebp - 4]
        inc    eax
        mov    cl, byte ptr [ecx]
        mov    byte ptr [eax + 0x17], cl     ; -> obj1:0x1A9B
        inc    dword ptr [ebp - 4]
        cmp    eax, 0x100
        jb     loc_1_307F
        jmp    loc_1_30DE
loc_1_3097:
        lea    ecx, [ebp - 8]
        push   ecx
        push   0
        push   3
        push   ebx
        call   sub_3_52A0
loc_1_30A5:
        add    esp, 0x18
        test   eax, eax
        jne    loc_1_3108
        cmp    dword ptr [ebp - 8], esi
        jbe    loc_1_3108
        mov    eax, 0x100
        sub    dword ptr [ebp - 8], esi
        cmp    dword ptr [ebp - 8], eax
        jbe    loc_1_30C1
        mov    dword ptr [ebp - 8], eax
loc_1_30C1:
        push   0
        push   dword ptr [ebp - 8]
        push   0x18                          ; -> loc_1_1A9C
        push   esi
        push   4
        push   ebx
        call   sub_3_52A0
loc_1_30D4:
        add    esp, 0x18
        test   eax, eax
        jne    loc_1_3108
        mov    eax, dword ptr [ebp - 8]
loc_1_30DE:
        mov    dword ptr [0x14], eax         ; -> loc_1_1A98
loc_1_30E3:
        cmp    dword ptr [0x10], esi         ; -> loc_1_1A94
        ja     loc_1_3108
        mov    eax, dword ptr [0x14]         ; -> loc_1_1A98
        add    eax, dword ptr [0x10]         ; -> loc_1_1A94
        cmp    eax, esi
        jbe    loc_1_3108
        sub    esi, dword ptr [0x10]         ; -> loc_1_1A94
        mov    al, byte ptr [esi + 0x18]     ; -> loc_1_1A9C
        jmp    loc_1_310A
loc_1_3108:
        mov    al, 0xff
loc_1_310A:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_3111:
        push   esi
        mov    eax, dword ptr [4]            ; -> loc_1_1A88
        mov    esi, dword ptr [esp + 8]
        push   edi
        test   byte ptr [eax + 0x17], 0x10
        jne    loc_1_3132
        mov    word ptr [esi + 6], 0x10
        xor    eax, eax
        mov    dword ptr [esi + 8], eax
        mov    dword ptr [esi + 0xc], eax
        jmp    loc_1_3181
loc_1_3132:
        mov    eax, dword ptr [4]            ; -> loc_1_1A88
        lea    edi, [esi + 0xc]
        movzx  edx, byte ptr [esi + 1]
        mov    ecx, dword ptr [eax + 0x18]
        push   0
        imul   edx, edx, 0x28
        push   4
        push   edi
        push   0x28
        push   0
        push   dword ptr [edx + ecx + 4]
        call   sub_3_52A0
loc_1_3156:
        add    esp, 0x18
        test   eax, eax
        je     loc_1_3164
        mov    eax, 0x1f
        jmp    loc_1_3183
loc_1_3164:
        mov    word ptr [esi + 6], 0x11
        mov    eax, dword ptr [edi]
        mov    ecx, eax
        and    al, 0xf8
        and    cx, 7
        mov    dword ptr [edi], eax
        mov    ch, cl
        mov    dword ptr [esi + 8], eax
        mov    cl, 0x11
        mov    word ptr [esi + 6], cx
loc_1_3181:
        xor    eax, eax
loc_1_3183:
        pop    edi
        pop    esi
        ret    
sub_1_3186:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        mov    ebx, 2
        mov    ax, word ptr [esi + 6]
        push   0
        and    ax, 1
        push   esi
        cmp    ax, 1
        adc    ebx, -1
        call   sub_1_2E4D
        add    esp, 8
        cmp    al, 0x13
        jne    loc_1_31FB
        push   ebx
        push   esi
        call   sub_1_2E4D
        add    esp, 8
        cmp    al, 3
        jb     loc_1_31FB
        lea    eax, [ebx*2]
        push   eax
        push   esi
        call   sub_1_2E4D
        add    esp, 8
        cmp    al, 0x43
        jne    loc_1_31FB
        mov    eax, ebx
        imul   eax, eax, 3
        push   eax
        push   esi
        call   sub_1_2E4D
        add    esp, 8
        cmp    al, 0x49
        jne    loc_1_31FB
        shl    ebx, 2
        push   ebx
        push   esi
        call   sub_1_2E4D
        add    esp, 8
        cmp    al, 0x53
        mov    eax, 1
        je     loc_1_31FD
loc_1_31FB:
        xor    eax, eax
loc_1_31FD:
        pop    esi
        pop    ebx
        ret    
sub_1_3200:
        push   ebx
        xor    eax, eax
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        lea    edi, [esi + 6]
        mov    dx, word ptr [edi]
        mov    ax, dx
        and    eax, 0x70
        cmp    eax, 0x10
        je     loc_1_3230
        cmp    eax, 0x30
        je     loc_1_3240
        cmp    eax, 0x40
        je     loc_1_3230
        cmp    eax, 0x50
        je     loc_1_3252
        mov    eax, 0x1f
        jmp    loc_1_32AB
loc_1_3230:
        or     dl, 1
        lea    ebx, [esi + 0xc]
        mov    word ptr [edi], dx
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [ebx], eax
        jmp    loc_1_3275
loc_1_3240:
        and    dx, 0xfffe
        lea    ebx, [esi + 0xc]
        mov    word ptr [edi], dx
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [ebx], eax
        jmp    loc_1_3275
loc_1_3252:
        and    dx, 0xf8ff
        mov    ecx, dword ptr [esi + 8]
        mov    word ptr [edi], dx
        mov    eax, ecx
        and    ax, 7
        lea    ebx, [esi + 0xc]
        shl    ax, 8
        and    cl, 0xf8
        or     ax, dx
        mov    dword ptr [ebx], ecx
        mov    word ptr [edi], ax
loc_1_3275:
        push   esi
        call   sub_1_3186
        add    esp, 4
        test   eax, eax
        jne    loc_1_32A4
        test   byte ptr [edi], 5
        je     loc_1_328E
        mov    eax, 0x1f
        jmp    loc_1_32AB
loc_1_328E:
        shr    dword ptr [ebx], 1
        push   esi
        call   sub_1_3186
        add    esp, 4
        test   eax, eax
        jne    loc_1_32A4
        mov    eax, 0x1f
        jmp    loc_1_32AB
loc_1_32A4:
        and    word ptr [edi], 0xff8f
        xor    eax, eax
loc_1_32AB:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_32AF:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        call   sub_1_3200
        add    esp, 4
        test   eax, eax
        jne    loc_1_32C5
        xor    eax, eax
        jmp    loc_1_3307
loc_1_32C5:
        lea    ecx, [esi + 6]
        mov    ax, word ptr [ecx]
        test   al, 8
        je     loc_1_3302
        test   al, 4
        jne    loc_1_3302
        and    ax, 0xff86
        push   esi
        mov    dword ptr [esi + 0xc], 0
        mov    word ptr [ecx], ax
        or     al, 4
        mov    word ptr [ecx], ax
        call   sub_1_3186
        add    esp, 4
        test   eax, eax
        je     loc_1_32F7
        xor    eax, eax
        jmp    loc_1_3307
loc_1_32F7:
        push   esi
        call   sub_1_3200
        add    esp, 4
        jmp    loc_1_3307
loc_1_3302:
        mov    eax, 0x1f
loc_1_3307:
        pop    esi
        ret    
sub_1_3309:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        push   edi
        mov    dx, word ptr [esi + 6]
        push   ebp
        mov    ax, dx
        mov    edi, 2
        and    ax, 1
        cmp    ax, 1
        mov    al, byte ptr [esi + 0x10]
        movzx  ecx, al
        adc    edi, -1
        cmp    ecx, 2
        je     loc_1_3365
        cmp    ecx, 3
        je     loc_1_33D0
        cmp    ecx, 6
        je     loc_1_33D8
        cmp    ecx, 0x11
        jl     loc_1_3546
        cmp    ecx, 0x12
        jle    loc_1_34AB
        cmp    ecx, 0x14
        je     loc_1_353A
        jmp    loc_1_3546
loc_1_3365:
        cmp    byte ptr [esi + 0x11], 4
        jae    loc_1_3375
        mov    eax, 0x1f
        jmp    loc_1_3548
loc_1_3375:
        and    dx, 0xffdf
        push   5
        or     dl, 0x50
        push   esi
        mov    word ptr [esi + 6], dx
        call   sub_1_2E4D
        movzx  edi, al
        shl    edi, 8
        add    esp, 8
        push   4
        push   esi
        call   sub_1_2E4D
        movzx  eax, al
        add    esp, 8
        add    edi, eax
        shl    edi, 8
        push   3
        push   esi
        call   sub_1_2E4D
        movzx  eax, al
        add    esp, 8
        add    edi, eax
        shl    edi, 8
        push   2
        push   esi
        call   sub_1_2E4D
        movzx  eax, al
        add    esp, 8
        add    edi, eax
        mov    dword ptr [esi + 8], edi
        jmp    loc_1_3546
loc_1_33D0:
        or     dl, 8
        jmp    loc_1_3542
loc_1_33D8:
        mov    bl, byte ptr [esi + 1]
        movzx  ecx, byte ptr [esi + 0x11]
        movzx  eax, bl
        imul   eax, eax, 5
        add    eax, 6
        cmp    eax, ecx
        jg     loc_1_34A1
        lea    eax, [edi*2]
        push   eax
        push   esi
        call   sub_1_2E4D
        add    esp, 8
        cmp    al, bl
        jbe    loc_1_34A1
        movzx  eax, byte ptr [esi + 1]
        imul   eax, eax, 5
        inc    eax
        imul   eax, edi
        lea    ebp, [eax + edi*2]
        push   ebp
        push   esi
        call   sub_1_2E4D
        add    esp, 8
        cmp    al, 1
        sbb    ecx, ecx
        add    ebp, edi
        mov    ax, word ptr [esi + 6]
        and    cl, 0xf0
        and    ax, 0xff8f
        add    ecx, 0x40
        or     cx, ax
        mov    eax, edi
        imul   eax, eax, 3
        mov    word ptr [esi + 6], cx
        add    eax, ebp
        push   eax
        push   esi
        call   sub_1_2E4D
        movzx  ebx, al
        shl    ebx, 8
        add    esp, 8
        lea    eax, [ebp + edi*2]
        push   eax
        push   esi
        add    edi, ebp
        call   sub_1_2E4D
        movzx  ecx, al
        add    esp, 8
        add    ebx, ecx
        shl    ebx, 8
        push   edi
        push   esi
        call   sub_1_2E4D
        movzx  eax, al
        add    esp, 8
        add    ebx, eax
        shl    ebx, 8
        push   ebp
        push   esi
        call   sub_1_2E4D
        movzx  eax, al
        add    esp, 8
        add    ebx, eax
        test   byte ptr [esi + 6], 0x30
        mov    dword ptr [esi + 8], ebx
        je     loc_1_3546
        shl    dword ptr [esi + 8], 1
        jmp    loc_1_3546
loc_1_34A1:
        mov    eax, 0x1f
        jmp    loc_1_3548
loc_1_34AB:
        cmp    byte ptr [esi + 0x11], 4
        jae    loc_1_34BB
        mov    eax, 0x1f
        jmp    loc_1_3548
loc_1_34BB:
        lea    ebp, [edi*2]
        sub    al, 0x11
        cmp    al, 1
        sbb    eax, eax
        and    dx, 0xff8f
        and    al, 0xf0
        add    eax, 0x40
        or     ax, dx
        mov    word ptr [esi + 6], ax
        mov    eax, edi
        imul   eax, eax, 3
        add    eax, ebp
        push   eax
        push   esi
        call   sub_1_2E4D
        movzx  ebx, al
        shl    ebx, 8
        add    esp, 8
        lea    eax, [ebp*2]
        push   eax
        push   esi
        add    edi, ebp
        call   sub_1_2E4D
        movzx  ecx, al
        add    esp, 8
        add    ebx, ecx
        shl    ebx, 8
        push   edi
        push   esi
        call   sub_1_2E4D
        movzx  eax, al
        add    esp, 8
        add    ebx, eax
        shl    ebx, 8
        push   ebp
        push   esi
        call   sub_1_2E4D
        movzx  eax, al
        add    esp, 8
        add    ebx, eax
        test   byte ptr [esi + 6], 0x30
        mov    dword ptr [esi + 8], ebx
        je     loc_1_3546
        shl    dword ptr [esi + 8], 1
        jmp    loc_1_3546
loc_1_353A:
        and    dx, 0xffaf
        or     dl, 0x20
loc_1_3542:
        mov    word ptr [esi + 6], dx
loc_1_3546:
        xor    eax, eax
loc_1_3548:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_354D:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        mov    edi, 2
        mov    ax, word ptr [esi + 6]
        push   0
        and    ax, 1
        push   esi
        cmp    ax, 1
        adc    edi, -1
        call   sub_1_2E4D
        movzx  eax, al
        add    esp, 8
        test   eax, eax
        je     loc_1_359C
        cmp    eax, 0xff
        je     loc_1_35A6
        push   edi
        push   esi
        call   sub_1_2E4D
        add    esp, 8
        cmp    al, 0xff
        je     loc_1_35A6
        movzx  eax, al
        add    eax, 2
        imul   eax, edi
        add    dword ptr [esi + 0xc], eax
        jmp    loc_1_359F
loc_1_359C:
        add    dword ptr [esi + 0xc], edi
loc_1_359F:
        mov    eax, 1
        jmp    loc_1_35A8
loc_1_35A6:
        xor    eax, eax
loc_1_35A8:
        pop    edi
        pop    esi
        ret    
sub_1_35AB:
        sub    esp, 4
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0x10]
        push   edi
        mov    ax, word ptr [esi + 6]
        push   ebp
        and    ax, 1
        mov    ebp, 2
        cmp    ax, 1
        push   0
        adc    ebp, -1
        push   esi
        call   sub_1_2E4D
        add    esp, 8
        mov    bl, al
        mov    eax, dword ptr [4]            ; -> loc_1_1A88
        test   byte ptr [eax + 0x17], 0x10
        je     loc_1_35EB
        cmp    bl, 0x13
        jne    loc_1_3715
loc_1_35EB:
        cmp    bl, 0xff
        je     loc_1_35F5
        cmp    bl, 0x13
        jne    loc_1_362A
loc_1_35F5:
        push   esi
        call   sub_1_32AF
        add    esp, 4
        cmp    eax, 0x1f
        je     loc_1_3715
        push   esi
        call   sub_1_354D
        add    esp, 4
        test   eax, eax
        je     loc_1_3715
        mov    ebp, 1
        push   0
        push   esi
        call   sub_1_2E4D
        add    esp, 8
        mov    bl, al
loc_1_362A:
        mov    eax, dword ptr [4]            ; -> loc_1_1A88
        test   byte ptr [eax + 0x17], 0x10
        jne    loc_1_363E
        cmp    bl, 1
        jne    loc_1_3715
loc_1_363E:
        xor    edi, edi
        mov    dword ptr [esp + 0x10], edi
loc_1_3644:
        push   ebp
        push   esi
        call   sub_1_2E4D
        movzx  ecx, bl
        add    esp, 8
        cmp    ecx, 6
        jg     loc_1_366D
        je     loc_1_36A1
        test   ecx, ecx
        je     loc_1_36C0
        cmp    ecx, 1
        je     loc_1_3693
        cmp    ecx, 2
        jl     loc_1_3684
        cmp    ecx, 3
        jle    loc_1_36A1
        jmp    loc_1_3684
loc_1_366D:
        cmp    ecx, 0x11
        jl     loc_1_3684
        cmp    ecx, 0x12
        jle    loc_1_36A1
        cmp    ecx, 0x14
        je     loc_1_36A1
        cmp    ecx, 0xff
        je     loc_1_36C0
loc_1_3684:
        test   al, al
        jne    loc_1_36C0
        inc    edi
        cmp    edi, 8
        jne    loc_1_36C0
        jmp    loc_1_3715
loc_1_3693:
        test   al, al
        je     loc_1_3715
        cmp    al, 0x20
        jbe    loc_1_36C0
        cmp    al, 0xff
        jae    loc_1_36C0
        jmp    loc_1_3715
loc_1_36A1:
        mov    cx, word ptr [esi + 6]
        and    cl, 0x70
        cmp    cl, 0x10
        ja     loc_1_3715
        push   esi
        mov    byte ptr [esi + 0x10], bl
        mov    byte ptr [esi + 0x11], al
        call   sub_1_3309
        add    esp, 4
        test   eax, eax
        jne    loc_1_3715
loc_1_36C0:
        push   esi
        call   sub_1_354D
        add    esp, 4
        test   eax, eax
        jne    loc_1_36F6
        inc    word ptr [0]                  ; -> loc_1_1A7C
        push   esi
        call   sub_1_32AF
        add    esp, 4
        cmp    eax, 0x1f
        je     loc_1_3722
        mov    ax, word ptr [esi + 6]
        mov    ebp, 2
        and    ax, 1
        cmp    ax, 1
        adc    ebp, -1
loc_1_36F6:
        push   0
        push   esi
        call   sub_1_2E4D
        inc    dword ptr [esp + 0x18]
        add    esp, 8
        mov    bl, al
        cmp    dword ptr [esp + 0x10], 0x100
        jl     loc_1_3644
loc_1_3715:
        mov    eax, 0x1f
loc_1_371A:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_1_3722:
        xor    eax, eax
        jmp    loc_1_371A
sub_1_3726:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0xc]
        mov    esi, 2
        mov    ax, word ptr [edi + 6]
        push   edi
        and    ax, 1
        cmp    ax, 1
        adc    esi, -1
        call   sub_1_354D
        add    esp, 4
        test   eax, eax
        jne    loc_1_376E
        push   edi
        call   sub_1_32AF
        add    esp, 4
        test   eax, eax
        jne    loc_1_3792
        mov    ax, word ptr [edi + 6]
        mov    esi, 2
        and    ax, 1
        cmp    ax, 1
        adc    esi, -1
loc_1_376E:
        push   0
        push   edi
        call   sub_1_2E4D
        add    esp, 8
        mov    byte ptr [edi + 0x10], al
        push   esi
        push   edi
        call   sub_1_2E4D
        add    esp, 8
        mov    byte ptr [edi + 0x11], al
        push   edi
        call   sub_1_3309
        add    esp, 4
loc_1_3792:
        pop    edi
        pop    esi
        ret    
sub_1_3795:
        mov    eax, dword ptr [esp + 4]
        mov    dl, byte ptr [eax + 4]
        mov    cl, byte ptr [eax + 0x10]
        cmp    dl, cl
        jne    loc_1_37AA
        mov    eax, 1
        jmp    loc_1_37E6
loc_1_37AA:
        cmp    dl, 0xff
        jne    loc_1_37E4
        test   byte ptr [eax + 2], 1
        je     loc_1_37BC
        mov    eax, 1
        jmp    loc_1_37E6
loc_1_37BC:
        cmp    cl, 2
        je     loc_1_37E4
        cmp    cl, 3
        je     loc_1_37E4
        cmp    cl, 6
        je     loc_1_37E4
        cmp    cl, 0x11
        je     loc_1_37E4
        cmp    cl, 0x12
        je     loc_1_37E4
        cmp    cl, 0x14
        je     loc_1_37E4
        mov    eax, 1
        cmp    cl, 0x13
        jne    loc_1_37E6
loc_1_37E4:
        xor    eax, eax
loc_1_37E6:
        ret    
sub_1_37E7:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [4], esi            ; -> loc_1_1A88
        je     loc_1_3804
        push   0
        call   sub_1_3806
        add    esp, 4
        mov    dword ptr [4], esi            ; -> loc_1_1A88
loc_1_3804:
        pop    esi
        ret    
sub_1_3806:
        push   esi
        xor    esi, esi
        cmp    dword ptr [8], esi            ; -> loc_1_1A8C
        je     loc_1_3825
        mov    eax, dword ptr [8]            ; -> loc_1_1A8C
        push   eax
        call   sub_1_44FF
        add    esp, 4
        mov    dword ptr [8], esi            ; -> loc_1_1A8C
loc_1_3825:
        mov    eax, dword ptr [esp + 8]
        test   eax, eax
        je     loc_1_3835
        cmp    eax, dword ptr [4]            ; -> loc_1_1A88
        jne    loc_1_384B
loc_1_3835:
        xor    eax, eax
        mov    dword ptr [4], eax            ; -> loc_1_1A88
        mov    dword ptr [0x10], eax         ; -> loc_1_1A94
        mov    dword ptr [0x14], eax         ; -> loc_1_1A98
        mov    dword ptr [0xc], eax          ; -> loc_1_1A90
loc_1_384B:
        pop    esi
        ret    
sub_1_384D:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 0x18]
        test   word ptr [edi + 2], 0xfffe
        je     loc_1_3867
        mov    eax, 2
        jmp    loc_1_3901
loc_1_3867:
        mov    esi, dword ptr [esp + 0x14]
        push   esi
        call   sub_1_37E7
        add    esp, 4
        push   edi
        call   sub_1_3111
        add    esp, 4
        test   eax, eax
        je     loc_1_3888
        mov    eax, 0x1f
        jmp    loc_1_3901
loc_1_3888:
        mov    ax, word ptr [edi + 6]
        xor    ebx, ebx
        and    ax, 1
        mov    ebp, 2
        cmp    ax, 1
        push   ebx
        adc    ebp, -1
        push   edi
        call   sub_1_2E4D
        add    esp, 8
        mov    byte ptr [edi + 0x10], al
        push   ebp
        push   edi
        call   sub_1_2E4D
        add    esp, 8
        mov    byte ptr [edi + 0x11], al
        cmp    byte ptr [edi + 0x10], 0x13
        jne    loc_1_38CE
        push   edi
        call   sub_1_32AF
        add    esp, 4
        cmp    eax, 0x1f
        jne    loc_1_38D9
        jmp    loc_1_38D4
loc_1_38CE:
        test   byte ptr [esi + 0x17], 0x10
        je     loc_1_38D9
loc_1_38D4:
        mov    ebx, 0x1f
loc_1_38D9:
        test   ebx, ebx
        jne    loc_1_38F6
        push   edi
        call   sub_1_3795
        add    esp, 4
        test   eax, eax
        jne    loc_1_38F6
        push   edi
        push   esi
        call   sub_1_3906
        add    esp, 8
        jmp    loc_1_3901
loc_1_38F6:
        push   esi
        call   sub_1_3806
        add    esp, 4
        mov    eax, ebx
loc_1_3901:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_3906:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x14]
        test   word ptr [edi + 2], 0xfffe
        je     loc_1_391C
        mov    eax, 2
        jmp    loc_1_3963
loc_1_391C:
        push   dword ptr [esp + 0x10]
        call   sub_1_37E7
        xor    si, si
        add    esp, 4
loc_1_392B:
        push   edi
        call   sub_1_3726
        add    esp, 4
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_1_3955
        push   edi
        call   sub_1_3795
        add    esp, 4
        test   eax, eax
        jne    loc_1_3955
        inc    si
        mov    ebx, 0x1f
        cmp    si, 0x100
        jb     loc_1_392B
loc_1_3955:
        push   dword ptr [esp + 0x10]
        call   sub_1_3806
        add    esp, 4
        mov    eax, ebx
loc_1_3963:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_3967:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        test   word ptr [esi + 2], 0xfffe
        je     loc_1_3985
        mov    eax, 2
        jmp    loc_1_3A15
loc_1_3985:
        push   dword ptr [ebp + 8]
        call   sub_1_37E7
        mov    ax, word ptr [esi + 6]
        add    esp, 4
        and    ax, 1
        cmp    ax, 1
        mov    eax, 2
        adc    eax, -1
        push   eax
        mov    dword ptr [ebp - 8], eax
        push   esi
        call   sub_1_2E4D
        movzx  di, al
        mov    word ptr [esi + 0x12], di
        add    esp, 8
        movzx  ax, byte ptr [esi + 5]
        mov    word ptr [ebp - 2], ax
        cmp    di, ax
        jae    loc_1_39CE
        mov    esi, 0x1f
        jmp    loc_1_3A08
loc_1_39CE:
        mov    ax, word ptr [esi + 0x10]
        sub    di, word ptr [ebp - 2]
        cmp    di, ax
        jbe    loc_1_39DE
        mov    di, ax
loc_1_39DE:
        test   di, di
        lea    ebx, [esi + 0x14]
        je     loc_1_3A06
loc_1_39E6:
        movzx  eax, word ptr [ebp - 2]
        add    eax, 2
        imul   eax, dword ptr [ebp - 8]
        push   eax
        push   esi
        call   sub_1_2E4D
        mov    byte ptr [ebx], al
        inc    ebx
        inc    word ptr [ebp - 2]
        add    esp, 8
        dec    di
        jne    loc_1_39E6
loc_1_3A06:
        xor    esi, esi
loc_1_3A08:
        push   dword ptr [ebp + 8]
        call   sub_1_3806
        add    esp, 4
        mov    eax, esi
loc_1_3A15:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_3A1C:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   esi
        push   dword ptr [ebp + 8]
        call   sub_1_37E7
        mov    word ptr [0], 0               ; -> loc_1_1A7C
        add    esp, 4
        mov    esi, dword ptr [ebp + 0xc]
        mov    ax, word ptr [esi]
        mov    word ptr [ebp - 0x14], ax
        lea    eax, [ebp - 0x14]
        push   eax
        call   sub_1_3111
        add    esp, 4
        test   eax, eax
        jne    loc_1_3A7F
        lea    eax, [ebp - 0x14]
        push   eax
        call   sub_1_35AB
        add    esp, 4
        cmp    eax, 0x1f
        jne    loc_1_3A6A
        mov    word ptr [esi + 2], 0
        jmp    loc_1_3A74
loc_1_3A6A:
        mov    ax, word ptr [0]              ; -> loc_1_1A7C
        mov    word ptr [esi + 2], ax
loc_1_3A74:
        push   dword ptr [ebp + 8]
        call   sub_1_3806
        add    esp, 4
loc_1_3A7F:
        xor    eax, eax
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_3A86:
        mov    ecx, dword ptr [esp + 8]
        test   byte ptr [ecx + 0x17], 0x10
        je     loc_1_3A97
        mov    eax, 0x1f
        jmp    loc_1_3AE4
loc_1_3A97:
        movzx  eax, word ptr [0]             ; -> loc_1_1994
        mov    edx, dword ptr [0xa]          ; -> loc_1_199E
        cmp    eax, 7
        je     loc_1_3ABF
        cmp    eax, 0xa
        je     loc_1_3AC8
        cmp    eax, 0xd
        je     loc_1_3AD1
        cmp    eax, 0x2b
        je     loc_1_3ADA
        mov    eax, 0x1c
        jmp    loc_1_3AE4
loc_1_3ABF:
        push   edx
        push   ecx
        call   sub_1_384D
        jmp    loc_1_3AE1
loc_1_3AC8:
        push   edx
        push   ecx
        call   sub_1_3906
        jmp    loc_1_3AE1
loc_1_3AD1:
        push   edx
        push   ecx
        call   sub_1_3967
        jmp    loc_1_3AE1
loc_1_3ADA:
        push   edx
        push   ecx
        call   sub_1_3A1C
loc_1_3AE1:
        add    esp, 8
loc_1_3AE4:
        ret    
sub_1_3AE5:
        mov    ecx, dword ptr [0]            ; -> obj1:0x1920
        test   ecx, ecx
        je     loc_1_3B05
        mov    ax, word ptr [esp + 4]
loc_1_3AF4:
        cmp    word ptr [ecx + 0xe], ax
        je     loc_1_3B01
        mov    ecx, dword ptr [ecx + 4]
        test   ecx, ecx
        jne    loc_1_3AF4
loc_1_3B01:
        test   ecx, ecx
        jne    loc_1_3B0C
loc_1_3B05:
        mov    eax, 0x21
        jmp    loc_1_3B14
loc_1_3B0C:
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax], ecx
        xor    eax, eax
loc_1_3B14:
        ret    
sub_1_3B15:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        lea    eax, [ebp - 4]
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_269E
        add    esp, 8
        test   eax, eax
        jne    loc_1_3B58
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 0x14]
        test   ecx, ecx
        je     loc_1_3B4A
        mov    al, byte ptr [ebp + 0xc]
loc_1_3B3B:
        cmp    byte ptr [ecx + 0x10], al
        je     loc_1_3B46
        mov    ecx, dword ptr [ecx]
        test   ecx, ecx
        jne    loc_1_3B3B
loc_1_3B46:
        test   ecx, ecx
        jne    loc_1_3B51
loc_1_3B4A:
        mov    eax, 0x21
        jmp    loc_1_3B58
loc_1_3B51:
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [eax], ecx
        xor    eax, eax
loc_1_3B58:
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_3B5C:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 0x14]
        mov    edx, dword ptr [eax]
        mov    eax, dword ptr [edx + 0xc]
        mov    eax, dword ptr [eax + 0x14]
        test   eax, eax
        je     loc_1_3B7A
loc_1_3B6F:
        cmp    dword ptr [eax + 0x1a], ecx
        je     loc_1_3B7A
        mov    eax, dword ptr [eax]
        test   eax, eax
        jne    loc_1_3B6F
loc_1_3B7A:
        ret    
sub_1_3B7B:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        sub    esp, 0x20
        mov    dword ptr [eax], 0
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    dx, 1
        mov    ecx, dword ptr [esi + 4]
        mov    dword ptr [ebp - 0x14], ecx
        add    ecx, 0x14
        mov    eax, dword ptr [ecx]
        mov    dword ptr [ebp - 0x18], ecx
        mov    dword ptr [ebp - 0xc], 0x20
        mov    eax, dword ptr [eax]
        mov    cl, byte ptr [eax + 0x12]
        mov    eax, dword ptr [eax + 0xc]
        shl    dx, cl
        mov    word ptr [ebp - 4], dx
        mov    ebx, dword ptr [eax + 0x14]
        test   ebx, ebx
        je     loc_1_4044
loc_1_3BC4:
        mov    ax, word ptr [ebx + 0xc]
        test   al, 6
        jne    loc_1_3FC5
        test   al, 1
        je     loc_1_3BEB
        mov    cx, word ptr [0x16]           ; -> obj1:0x1942
        mov    eax, dword ptr [ebp - 0x14]
        mov    edx, dword ptr [eax + 0x10]
        cmp    word ptr [edx + 0x16], cx
        jne    loc_1_3FC5
loc_1_3BEB:
        mov    eax, dword ptr [ebp - 0x14]
        add    eax, 0x1a
        mov    dword ptr [ebp - 0x1c], eax
        cmp    byte ptr [eax], 0
        jne    loc_1_3C0C
        mov    ax, word ptr [esi + 8]
        and    ax, 2
        cmp    ax, 1
        sbb    eax, eax
        add    eax, 2
        jmp    loc_1_3C11
loc_1_3C0C:
        mov    eax, 4
loc_1_3C11:
        lea    ecx, [ebp - 8]
        push   ecx
        push   eax
        push   ebx
        call   sub_1_2AA9
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_404C
        mov    ax, word ptr [ebp - 4]
        mov    cl, byte ptr [2]              ; -> obj1:0x196E
        test   word ptr [4], ax              ; -> obj1:0x1970
        je     loc_1_3FC5
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_3FC5
        mov    eax, dword ptr [ebp - 0x1c]
        cmp    byte ptr [eax], 0
        jne    loc_1_3CD9
        test   byte ptr [esi + 8], 0x10
        je     loc_1_3C76
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax]
        test   ah, 1
        je     loc_1_3C6A
        test   ah, 2
        je     loc_1_3C76
loc_1_3C6A:
        mov    dword ptr [ebp - 0xc], 2
        jmp    loc_1_3FC5
loc_1_3C76:
        mov    eax, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 8]
        and    cl, 0x80
        mov    al, byte ptr [eax + 0x1f]
        push   eax
        cmp    cl, 1
        mov    al, byte ptr [edx + 0x1e]
        mov    dl, byte ptr [esi + 0x22]
        push   eax
        push   edx
        sbb    eax, eax
        inc    eax
        push   eax
        call   sub_1_270C
        add    esp, 0x10
        mov    byte ptr [ebp - 1], al
        test   al, al
        jne    loc_1_3CAD
        mov    dword ptr [ebp - 0xc], 0x17
        jmp    loc_1_3FC5
loc_1_3CAD:
        mov    eax, dword ptr [ebp - 8]
        test   byte ptr [eax], 0x20
        je     loc_1_3CBD
        mov    eax, dword ptr [esi + 0xe]
        mov    dword ptr [ebp - 0x10], eax
        jmp    loc_1_3CC6
loc_1_3CBD:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [eax + 0x16]
        mov    dword ptr [ebp - 0x10], ecx
loc_1_3CC6:
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x12
        cmp    dword ptr [eax], 0
        jne    loc_1_3D1B
        mov    dword ptr [eax], 0x1000
        jmp    loc_1_3D1B
loc_1_3CD9:
        cmp    byte ptr [esi + 0x22], 0x80
        je     loc_1_3CEB
        mov    dword ptr [ebp - 0xc], 0x17
        jmp    loc_1_3FC5
loc_1_3CEB:
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [ebp - 1], 0
        test   byte ptr [eax], 0x20
        je     loc_1_3CFC
        mov    eax, dword ptr [esi + 0xe]
        jmp    loc_1_3D07
loc_1_3CFC:
        push   dword ptr [esi + 0xe]
        call   sub_1_27A5
        add    esp, 4
loc_1_3D07:
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x12
        cmp    dword ptr [eax], 0
        jne    loc_1_3D1B
        mov    dword ptr [eax], 1
loc_1_3D1B:
        mov    cx, word ptr [esi + 8]
        test   cl, 0x20
        je     loc_1_3D30
        mov    eax, dword ptr [ebp - 8]
        test   byte ptr [eax], 4
        je     loc_1_3FC5
loc_1_3D30:
        and    cx, 8
        jne    loc_1_3D3E
        mov    eax, dword ptr [ebp - 8]
        test   byte ptr [eax], 8
        je     loc_1_3D4B
loc_1_3D3E:
        test   cx, cx
        je     loc_1_3D57
        mov    eax, dword ptr [ebp - 8]
        test   byte ptr [eax], 0x10
        jne    loc_1_3D57
loc_1_3D4B:
        mov    dword ptr [ebp - 0xc], 2
        jmp    loc_1_3FC5
loc_1_3D57:
        test   byte ptr [esi + 3], 0x40
        mov    eax, dword ptr [ebp - 8]
        je     loc_1_3DC3
        mov    eax, dword ptr [eax + 0xa]
        cmp    dword ptr [esi + 0xe], eax
        jae    loc_1_3D6B
        mov    dword ptr [esi + 0xe], eax
loc_1_3D6B:
        mov    eax, dword ptr [ebp - 8]
        test   byte ptr [eax], 2
        je     loc_1_3DA1
        mov    edi, dword ptr [eax + 0x12]
        mov    ecx, dword ptr [esi + 0xe]
        sub    edx, edx
        lea    eax, [ecx + edi - 1]
        div    edi
        imul   eax, edi
        cmp    ecx, eax
        jae    loc_1_3D8B
        mov    dword ptr [esi + 0xe], eax
loc_1_3D8B:
        mov    eax, dword ptr [ebp - 8]
        test   byte ptr [eax], 0x40
        je     loc_1_3DA1
        push   dword ptr [esi + 0xe]
        call   sub_1_27A5
        add    esp, 4
        mov    dword ptr [esi + 0xe], eax
loc_1_3DA1:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 0xe]
        test   eax, eax
        je     loc_1_3E30
        cmp    dword ptr [esi + 0xe], eax
        jbe    loc_1_3E30
        mov    dword ptr [ebp - 0xc], 0xa
        mov    dword ptr [esi + 0x1a], eax
        jmp    loc_1_3FC5
loc_1_3DC3:
        test   byte ptr [eax], 2
        je     loc_1_3E13
        mov    edi, dword ptr [esi + 0xe]
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [ecx + 0xa], edi
        ja     loc_1_3DFE
        mov    eax, dword ptr [ecx + 0xe]
        test   eax, eax
        je     loc_1_3DDE
        cmp    eax, edi
        jb     loc_1_3DFE
loc_1_3DDE:
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, edi
        sub    edx, edx
        div    dword ptr [ecx + 0x12]
        test   edx, edx
        jne    loc_1_3DFE
        test   byte ptr [ecx], 0x40
        je     loc_1_3E30
        push   edi
        call   sub_1_27A5
        add    esp, 4
        cmp    eax, edi
        je     loc_1_3E30
loc_1_3DFE:
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0xc], 0xa
        mov    ecx, dword ptr [eax + 0xe]
        mov    dword ptr [esi + 0x1a], ecx
        jmp    loc_1_3FC5
loc_1_3E13:
        mov    edx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [esi + 0xe]
        cmp    dword ptr [edx + 0xa], ecx
        je     loc_1_3E30
        mov    dword ptr [ebp - 0xc], 0xa
        mov    ecx, dword ptr [edx + 0xe]
        mov    dword ptr [esi + 0x1a], ecx
        jmp    loc_1_3FC5
loc_1_3E30:
        test   byte ptr [esi + 3], 0x40
        je     loc_1_3E66
        mov    eax, dword ptr [ebp - 0x10]
        cmp    dword ptr [esi + 0x1e], eax
        jae    loc_1_3E41
        mov    dword ptr [esi + 0x1e], eax
loc_1_3E41:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 2]
        cmp    dword ptr [esi + 0x12], eax
        jae    loc_1_3E4F
        mov    dword ptr [esi + 0x12], eax
loc_1_3E4F:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax + 6]
        cmp    dword ptr [esi + 0x16], eax
        jbe    loc_1_3EEA
        mov    dword ptr [esi + 0x16], eax
        jmp    loc_1_3EEA
loc_1_3E66:
        mov    ecx, dword ptr [esi + 0xa]
        test   ecx, ecx
        je     loc_1_3ED8
        mov    eax, dword ptr [ebp - 8]
        test   byte ptr [eax], 1
        je     loc_1_3E96
        lea    edi, [eax + 2]
        cmp    dword ptr [edi], ecx
        ja     loc_1_3E8A
        mov    eax, dword ptr [esi + 0xe]
        mov    edx, dword ptr [ebp - 8]
        add    eax, ecx
        dec    eax
        cmp    eax, dword ptr [edx + 6]
        jbe    loc_1_3EAC
loc_1_3E8A:
        mov    dword ptr [ebp - 0xc], 3
        jmp    loc_1_3FC5
loc_1_3E96:
        mov    edi, dword ptr [ebp - 8]
        add    edi, 2
        cmp    dword ptr [edi], ecx
        je     loc_1_3EAC
        mov    dword ptr [ebp - 0xc], 3
        jmp    loc_1_3FC5
loc_1_3EAC:
        cmp    dword ptr [ebp - 0x10], 0
        je     loc_1_3EC9
        mov    eax, ecx
        sub    edx, edx
        div    dword ptr [ebp - 0x10]
        test   edx, edx
        je     loc_1_3EC9
        mov    dword ptr [ebp - 0xc], 3
        jmp    loc_1_3FC5
loc_1_3EC9:
        mov    dword ptr [edi], ecx
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [esi + 0xa]
        add    eax, dword ptr [esi + 0xe]
        dec    eax
        mov    dword ptr [ecx + 6], eax
loc_1_3ED8:
        mov    eax, dword ptr [ebp - 8]
        test   byte ptr [eax], 1
        jne    loc_1_3EEA
        mov    ecx, dword ptr [eax + 2]
        add    ecx, dword ptr [esi + 0xe]
        dec    ecx
        mov    dword ptr [eax + 6], ecx
loc_1_3EEA:
        test   byte ptr [esi + 3], 0x10
        je     loc_1_3FE2
        mov    edx, 0                        ; -> loc_1_1A58
        xor    eax, eax
        mov    edi, edx
        mov    ecx, 9
        rep stosd dword ptr es:[edi], eax
        mov    eax, dword ptr [ebp - 0x1c]
        cmp    byte ptr [eax], 0
        mov    word ptr [0], 1               ; -> loc_1_1A58
        jne    loc_1_3F5C
        mov    eax, dword ptr [ebp - 0x10]
        mov    edi, dword ptr [ebp - 8]
        dec    eax
        push   edx
        not    eax
        mov    word ptr [2], 0x14            ; -> loc_1_1A5A
        mov    dword ptr [0x10], eax         ; -> loc_1_1A68
        mov    ecx, dword ptr [esi + 0xe]
        mov    dword ptr [0x14], ecx         ; -> loc_1_1A6C
        mov    eax, dword ptr [edi + 2]
        mov    dword ptr [0x18], eax         ; -> loc_1_1A70
        mov    eax, dword ptr [ebp - 0x18]
        mov    ecx, dword ptr [edi + 6]
        mov    dword ptr [0x1c], ecx         ; -> loc_1_1A74
        mov    edx, dword ptr [eax]
        push   dword ptr [edx]
        call   sub_3_23DB
loc_1_3F53:
        add    esp, 8
        test   eax, eax
        je     loc_1_3FD1
        jmp    loc_1_3FB6
loc_1_3F5C:
        mov    eax, dword ptr [ebp - 0x10]
        mov    edx, dword ptr [ebp - 8]
        dec    ax
        push   0                             ; -> loc_1_1A58
        not    ax
        mov    word ptr [2], 0xc             ; -> loc_1_1A5A
        mov    word ptr [0xc], ax            ; -> loc_1_1A64
        mov    ecx, dword ptr [esi + 0xe]
        mov    word ptr [0xe], cx            ; -> loc_1_1A66
        mov    eax, dword ptr [edx + 2]
        mov    word ptr [0x10], ax           ; -> loc_1_1A68
        mov    eax, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [edx + 6]
        mov    word ptr [0x12], cx           ; -> loc_1_1A6A
        mov    ecx, dword ptr [ebp - 0x18]
        mov    al, byte ptr [eax + 0x18]
        mov    byte ptr [0x17], al           ; -> obj1:0x1A6F
        mov    edx, dword ptr [ecx]
        push   dword ptr [edx]
        call   sub_3_23B1
loc_1_3FAF:
        add    esp, 8
        test   eax, eax
        je     loc_1_3FD8
loc_1_3FB6:
        cmp    dword ptr [esi + 0xa], 1
        sbb    eax, eax
        and    eax, 0x1d
        add    eax, 3
        mov    dword ptr [ebp - 0xc], eax
loc_1_3FC5:
        mov    ebx, dword ptr [ebx]
        test   ebx, ebx
        jne    loc_1_3BC4
        jmp    loc_1_4044
loc_1_3FD1:
        mov    eax, dword ptr [4]            ; -> loc_1_1A5C
        jmp    loc_1_3FDF
loc_1_3FD8:
        movzx  eax, word ptr [4]             ; -> loc_1_1A5C
loc_1_3FDF:
        mov    dword ptr [esi + 0xa], eax
loc_1_3FE2:
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [eax], ebx
        mov    eax, dword ptr [esi]
        and    eax, 0x40000000
        mov    cl, byte ptr [ebp - 1]
        cmp    eax, 1
        sbb    eax, eax
        and    al, 0xfe
        add    eax, 4
        or     word ptr [ebx + 0xc], ax
        mov    byte ptr [ebx + 0x11], cl
        xor    ecx, ecx
        mov    edi, dword ptr [ebp - 0x14]
        mov    byte ptr [ebx + 0x12], cl
        add    esi, 8
        mov    dword ptr [ebx + 0x16], ecx
        mov    eax, dword ptr [esi + 2]
        mov    dword ptr [ebx + 0x1e], eax
        mov    edx, dword ptr [esi + 6]
        mov    dword ptr [ebx + 0x22], edx
        mov    dword ptr [ebx + 0x1a], edi
        mov    dx, word ptr [esi]
        and    dx, 0xfeff
        mov    word ptr [esi], dx
        cmp    byte ptr [edi + 0x1a], cl
        jne    loc_1_403D
        mov    eax, dword ptr [ebp - 8]
        test   byte ptr [eax], 0x80
        je     loc_1_403D
        or     dh, 1
        mov    word ptr [esi], dx
loc_1_403D:
        mov    ax, word ptr [esi]
        mov    word ptr [ebx + 0x14], ax
loc_1_4044:
        cmp    ebx, 1
        sbb    eax, eax
        and    eax, dword ptr [ebp - 0xc]
loc_1_404C:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_4053:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x10]
        test   byte ptr [edi + 0xc], 2
        mov    esi, dword ptr [edi + 0x1a]
        jne    loc_1_406D
        mov    eax, 0x21
        jmp    loc_1_4150
loc_1_406D:
        xor    bl, bl
        cmp    byte ptr [esi + 0x1a], bl
        jne    loc_1_4080
        lea    eax, [edi + 0x14]
        test   byte ptr [eax], 0x10
        je     loc_1_408C
        mov    bl, 8
        jmp    loc_1_408C
loc_1_4080:
        mov    bl, 1
        lea    eax, [edi + 0x14]
        test   byte ptr [eax], 2
        je     loc_1_408C
        mov    bl, 9
loc_1_408C:
        mov    ax, word ptr [eax]
        test   al, 4
        je     loc_1_4096
        or     bl, 2
loc_1_4096:
        test   al, 8
        je     loc_1_409D
        or     bl, 4
loc_1_409D:
        lea    ecx, [edi + 8]
        mov    eax, dword ptr [ecx]
        mov    edx, dword ptr [eax + 0x18]
        mov    dword ptr [0x14], edx         ; -> sub_1_1980
        mov    eax, dword ptr [ecx]
        mov    edx, dword ptr [eax + 0x1c]
        mov    byte ptr [1], 0x88            ; -> obj1:0x196D
        mov    dword ptr [0xa], edx          ; -> obj1:0x1976
        mov    eax, dword ptr [ecx]
        mov    al, byte ptr [eax + 0xc]
        mov    byte ptr [0], al              ; -> obj1:0x196C
        mov    cl, byte ptr [edi + 0x10]
        mov    byte ptr [3], cl              ; -> obj1:0x196F
        call   sub_1_2438
        test   eax, eax
        jne    loc_1_4150
        mov    byte ptr [1], 0x89            ; -> obj1:0x196D
        mov    eax, dword ptr [esi + 0x14]
        mov    ecx, dword ptr [eax]
        mov    al, byte ptr [ecx + 0x12]
        mov    cl, byte ptr [2]              ; -> obj1:0x196E
        and    cl, 0xf0
        mov    byte ptr [9], bl              ; -> obj1:0x1975
        or     al, cl
        mov    byte ptr [2], al              ; -> obj1:0x196E
        mov    dl, byte ptr [edi + 0x11]
        mov    byte ptr [8], dl              ; -> obj1:0x1974
        cmp    byte ptr [esi + 0x1a], 0
        jne    loc_1_412B
        test   al, 0x10
        jne    loc_1_412B
        mov    eax, dword ptr [edi + 0x22]
        shr    eax, 0xc
        mov    word ptr [4], ax              ; -> obj1:0x1970
        mov    eax, dword ptr [edi + 0x1e]
        shr    eax, 0xc
        mov    word ptr [0xe], ax            ; -> obj1:0x197A
        jmp    loc_1_413C
loc_1_412B:
        mov    eax, dword ptr [edi + 0x22]
        mov    dword ptr [4], eax            ; -> obj1:0x1970
        mov    ecx, dword ptr [edi + 0x1e]
        mov    dword ptr [0xe], ecx          ; -> obj1:0x197A
loc_1_413C:
        call   sub_1_2438
        cmp    eax, 1
        mov    ecx, eax
        mov    eax, 0
        adc    eax, -1
        and    eax, ecx
loc_1_4150:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_4154:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    eax, [ebp - 4]
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   eax
        mov    ax, word ptr [esi + 0x14]
        and    ax, 2
        cmp    ax, 1
        sbb    eax, eax
        add    eax, 2
        push   eax
        push   esi
        call   sub_1_2AA9
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_423F
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_4196
        mov    eax, 0x11
        jmp    loc_1_423F
loc_1_4196:
        mov    eax, dword ptr [ebp - 4]
        lea    ecx, [esi + 0x16]
        test   byte ptr [eax], 0x80
        je     loc_1_41B6
        mov    eax, dword ptr [ecx]
        sub    edx, edx
        div    dword ptr [esi + 0x22]
        test   edx, edx
        je     loc_1_41CB
        mov    eax, 7
        jmp    loc_1_423F
loc_1_41B6:
        mov    edi, dword ptr [ebp - 4]
        mov    eax, dword ptr [ecx]
        sub    edx, edx
        div    dword ptr [edi + 0x1a]
        test   edx, edx
        je     loc_1_41CB
        mov    eax, 7
        jmp    loc_1_423F
loc_1_41CB:
        xor    dx, dx
        mov    ax, word ptr [esi + 0x14]
        test   al, 4
        je     loc_1_41DA
        mov    dx, 2
loc_1_41DA:
        test   al, 2
        je     loc_1_41E1
        or     dl, 1
loc_1_41E1:
        lea    edi, [esi + 8]
        mov    eax, dword ptr [edi]
        mov    eax, dword ptr [eax + 0x18]
        mov    dword ptr [0x14], eax         ; -> sub_1_1980
        mov    eax, dword ptr [edi]
        mov    eax, dword ptr [eax + 0x1c]
        mov    byte ptr [1], 0x8b            ; -> obj1:0x196D
        mov    dword ptr [0xa], eax          ; -> obj1:0x1976
        mov    edi, dword ptr [edi]
        mov    al, byte ptr [edi + 0xc]
        mov    byte ptr [0], al              ; -> obj1:0x196C
        mov    al, byte ptr [esi + 0x10]
        mov    byte ptr [3], al              ; -> obj1:0x196F
        mov    al, byte ptr [esi + 0x12]
        mov    word ptr [8], dx              ; -> obj1:0x1974
        mov    byte ptr [2], al              ; -> obj1:0x196E
        mov    eax, dword ptr [ecx]
        shr    eax, 0xc
        mov    word ptr [0xe], ax            ; -> obj1:0x197A
        call   sub_1_2438
        cmp    eax, 1
        mov    ecx, eax
        mov    eax, 0
        adc    eax, -1
        and    eax, ecx
loc_1_423F:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_4245:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        push   edi
        push   ebp
        test   esi, esi
        je     loc_1_434B
        mov    eax, dword ptr [esi + 0x14]
        mov    ecx, dword ptr [eax]
        mov    edx, dword ptr [ecx + 0xc]
        mov    ebx, dword ptr [edx + 0x14]
        test   ebx, ebx
        je     loc_1_434B
        mov    ebp, 0                        ; -> loc_1_1A58
loc_1_426D:
        test   byte ptr [ebx + 0xc], 2
        je     loc_1_4341
        cmp    dword ptr [ebx + 0x1a], esi
        jne    loc_1_4341
        mov    eax, dword ptr [esi + 0x14]
        cmp    esi, 0                        ; -> obj1:0x1950
        je     loc_1_4321
        test   byte ptr [eax + 0xb], 0x18
        jne    loc_1_4321
        mov    edi, ebp
        xor    eax, eax
        mov    ecx, 9
        rep stosd dword ptr es:[edi], eax
        cmp    byte ptr [esi + 0x1a], al
        mov    word ptr [0], 1               ; -> loc_1_1A58
        jne    loc_1_42EB
        mov    word ptr [2], 0x14            ; -> loc_1_1A5A
        mov    eax, dword ptr [esi + 8]
        push   ebp
        mov    dword ptr [4], eax            ; -> loc_1_1A5C
        mov    eax, dword ptr [esi + 0xc]
        add    eax, dword ptr [4]            ; -> loc_1_1A5C
        dec    eax
        mov    dword ptr [8], eax            ; -> loc_1_1A60
        mov    ecx, dword ptr [esi + 0x14]
        push   dword ptr [ecx]
        call   sub_3_25B0
loc_1_42DD:
        add    esp, 8
        test   eax, eax
        je     loc_1_4321
        mov    eax, 0x19
        jmp    loc_1_434D
loc_1_42EB:
        mov    word ptr [2], 0xc             ; -> loc_1_1A5A
        mov    eax, dword ptr [esi + 8]
        push   ebp
        mov    word ptr [4], ax              ; -> loc_1_1A5C
        mov    eax, dword ptr [esi + 0xc]
        add    ax, word ptr [4]              ; -> loc_1_1A5C
        dec    ax
        mov    word ptr [6], ax              ; -> loc_1_1A5E
        mov    ecx, dword ptr [esi + 0x14]
        push   dword ptr [ecx]
        call   sub_3_2586
loc_1_431A:
        add    esp, 8
        test   eax, eax
        jne    loc_1_4352
loc_1_4321:
        and    word ptr [ebx + 0x14], 0xfffb
        push   ebx
        call   sub_1_4053
        add    esp, 4
        test   eax, eax
        jne    loc_1_434D
        and    word ptr [ebx + 0xc], 0xfffd
        mov    dword ptr [ebx + 0x1a], 0
loc_1_4341:
        mov    ebx, dword ptr [ebx]
        test   ebx, ebx
        jne    loc_1_426D
loc_1_434B:
        xor    eax, eax
loc_1_434D:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_4352:
        mov    eax, 0x19
        jmp    loc_1_434D
sub_1_4359:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        push   0
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0x10]
        mov    dword ptr [ecx + 0x30], eax
        mov    dword ptr [ecx + 0x34], edx
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [ecx + 0x38], eax
        call   sub_1_3806
        add    esp, 4
        xor    eax, eax
        pop    ebp
        ret    
sub_1_4380:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x28
        push   ebx
        mov    edx, dword ptr [ebp + 0x10]
        push   esi
        xor    ebx, ebx
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        lea    edi, [ebp - 0x28]
        xor    eax, eax
        mov    ecx, 8
        mov    dword ptr [esi], ebx
        mov    dword ptr [edx], ebx
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        stosb  byte ptr es:[edi], al
        mov    word ptr [ebp - 0x20], 6
        mov    byte ptr [ebp - 6], 0x3a
        mov    dword ptr [ebp - 0x24], 0     ; -> obj1:0x1950
        mov    dword ptr [ebp - 0x28], 0x40000000
        mov    dword ptr [ebp - 0x12], 0xffffffff
        mov    eax, 0x1000
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0x1a], eax
        mov    dword ptr [ebp - 0x16], eax
        mov    dword ptr [ebp - 0xa], eax
        mov    dword ptr [8], eax            ; -> obj1:0x1958
        mov    dword ptr [0xc], eax          ; -> obj1:0x195C
        lea    eax, [ebp - 4]
        mov    edx, dword ptr [ecx + 0x18]
        push   eax
        lea    ecx, [ebp - 0x28]
        mov    dword ptr [0x14], edx         ; -> obj1:0x1964
        push   ecx
        call   sub_1_3B7B
        add    esp, 8
        test   eax, eax
        jne    loc_1_4408
        mov    ecx, dword ptr [ebp - 0x1a]
        mov    ebx, dword ptr [ebp - 0x12]
        mov    edx, dword ptr [ebp + 0x10]
        mov    dword ptr [esi], ecx
        mov    dword ptr [edx], ebx
loc_1_4408:
        mov    ecx, dword ptr [0]            ; -> obj1:0x1920
        mov    dword ptr [ebp - 4], ecx
        test   ecx, ecx
        je     loc_1_442B
loc_1_4415:
        mov    ecx, dword ptr [ebp - 4]
        and    word ptr [ecx + 0xc], 0xfffb
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ecx + 4]
        mov    dword ptr [ebp - 4], edx
        test   edx, edx
        jne    loc_1_4415
loc_1_442B:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_4432:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0x24
        lea    edx, [ecx + 0xc]
        push   esi
        push   edi
        mov    eax, dword ptr [edx]
        cmp    dword ptr [eax + 0x30], 0
        jne    loc_1_4453
        mov    edi, 0x19
        jmp    loc_1_44F7
loc_1_4453:
        mov    eax, dword ptr [eax + 0x38]
        mov    dword ptr [0], eax            ; -> obj1:0x0008
        mov    esi, dword ptr [edx]
        mov    edi, dword ptr [esi + 0x30]
        mov    dword ptr [8], edi            ; -> obj1:0x1958
        lea    edi, [ebp - 0x24]
        mov    esi, dword ptr [edx]
        mov    eax, dword ptr [esi + 0x34]
        mov    dword ptr [0xc], eax          ; -> obj1:0x195C
        xor    eax, eax
        mov    ecx, dword ptr [ecx + 0x18]
        mov    dword ptr [0x14], ecx         ; -> obj1:0x1964
        mov    ecx, 8
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        stosb  byte ptr es:[edi], al
        mov    word ptr [ebp - 0x1c], 6
        mov    edi, dword ptr [8]            ; -> obj1:0x1958
        mov    eax, dword ptr [0xc]          ; -> obj1:0x195C
        mov    esi, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 0x1a], edi
        push   esi
        mov    dword ptr [ebp - 0x16], eax
        mov    byte ptr [ebp - 2], 0x3a
        mov    dword ptr [ebp - 0x20], 0     ; -> obj1:0x1950
        lea    eax, [ebp - 0x24]
        push   eax
        call   sub_1_3B7B
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        jne    loc_1_44F7
        push   dword ptr [esi]
        call   sub_1_4053
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        jne    loc_1_44E4
        push   dword ptr [esi]
        call   sub_1_4154
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        jne    loc_1_44E4
        xor    eax, eax
        jmp    loc_1_44F9
loc_1_44E4:
        push   0                             ; -> obj1:0x1950
        call   sub_1_4245
        add    esp, 4
        mov    dword ptr [esi], 0
loc_1_44F7:
        mov    eax, edi
loc_1_44F9:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_44FF:
        mov    eax, dword ptr [esp + 4]
        push   dword ptr [eax + 0x1a]
        call   sub_1_4245
        add    esp, 4
        ret    
sub_1_450F:
        push   ebx
        push   esi
        mov    ebx, dword ptr [esp + 0xc]
        push   edi
        mov    esi, dword ptr [esp + 0x14]
        mov    eax, dword ptr [ebx + 0x16]
        cmp    eax, esi
        ja     loc_1_452C
        lea    edi, [ebx + 0x22]
        mov    ecx, dword ptr [edi]
        add    ecx, eax
        cmp    ecx, esi
        ja     loc_1_454A
loc_1_452C:
        lea    edi, [ebx + 0x22]
        mov    eax, esi
        sub    edx, edx
        push   ebx
        mov    ecx, dword ptr [edi]
        div    ecx
        imul   eax, ecx
        mov    dword ptr [ebx + 0x16], eax
        call   sub_1_4154
        add    esp, 4
        test   eax, eax
        jne    loc_1_4560
loc_1_454A:
        mov    eax, esi
        sub    edx, edx
        div    dword ptr [edi]
        mov    ecx, dword ptr [0]            ; -> obj1:0x0008
        mov    eax, dword ptr [esp + 0x18]
        add    ecx, edx
        mov    dword ptr [eax], ecx
        xor    eax, eax
loc_1_4560:
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_4564:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 8]
        mov    dl, byte ptr [eax + 0xf]
        mov    eax, dword ptr [0xa]          ; -> loc_1_199E
        mov    byte ptr [eax], dl
        mov    eax, dword ptr [0xa]          ; -> loc_1_199E
        mov    cl, byte ptr [ecx + 0x10]
        mov    byte ptr [eax + 1], cl
        xor    eax, eax
        ret    
loc_1_4583:
        sub    esp, 4
        mov    ecx, dword ptr [0xa]          ; -> loc_1_199E
        lea    eax, [esp]
        mov    dl, byte ptr [ecx + 1]
        push   eax
        push   edx
        mov    al, byte ptr [ecx]
        push   eax
        call   sub_1_3B15
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_45C3
        mov    eax, dword ptr [esp]
        test   byte ptr [eax + 0xc], 2
        mov    eax, 0x21
        je     loc_1_45C3
        mov    eax, dword ptr [esp]
        mov    ax, word ptr [eax + 0xe]
        mov    word ptr [2], ax              ; -> loc_1_1996
        xor    eax, eax
loc_1_45C3:
        add    esp, 4
        ret    
loc_1_45C7:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [0xa]          ; -> loc_1_199E
        mov    eax, ebp
        mov    ax, word ptr [eax]
        test   ax, 0xfff1
        je     loc_1_45E9
        mov    eax, 2
        jmp    loc_1_46DB
loc_1_45E9:
        and    ax, 2
        lea    ecx, [esp + 0x10]
        cmp    ax, 1
        push   ecx
        sbb    eax, eax
        mov    edi, dword ptr [esp + 0x1c]
        add    eax, 2
        push   eax
        push   edi
        call   sub_1_2AA9
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_46DB
        cmp    dword ptr [esp + 0x10], 0
        jne    loc_1_4622
        mov    eax, 2
        jmp    loc_1_46DB
loc_1_4622:
        mov    dx, word ptr [ebp]
        mov    cl, byte ptr [2]              ; -> obj1:0x196E
        test   dl, 4
        jne    loc_1_4644
        mov    eax, dword ptr [esp + 0x10]
        test   byte ptr [eax], 4
        jne    loc_1_4644
        mov    eax, 2
        jmp    loc_1_46DB
loc_1_4644:
        mov    si, word ptr [edi + 0x14]
        mov    bl, byte ptr [edi + 0x11]
        test   dl, 8
        je     loc_1_4684
        mov    eax, dword ptr [esp + 0x10]
        mov    edx, dword ptr [esp + 0x10]
        and    cl, 0x80
        mov    al, byte ptr [eax + 0x1f]
        push   eax
        cmp    cl, 1
        mov    al, byte ptr [edx + 0x1e]
        mov    dl, byte ptr [ebp + 2]
        push   eax
        push   edx
        sbb    eax, eax
        inc    eax
        push   eax
        call   sub_1_270C
        add    esp, 0x10
        test   al, al
        jne    loc_1_4681
        mov    eax, 0x17
        jmp    loc_1_46DB
loc_1_4681:
        mov    byte ptr [edi + 0x11], al
loc_1_4684:
        mov    cx, word ptr [edi + 0x14]
        push   edi
        and    cx, 0xfff9
        mov    word ptr [edi + 0x14], cx
        mov    ax, word ptr [ebp]
        and    ax, 6
        or     ax, cx
        mov    word ptr [edi + 0x14], ax
        call   sub_1_4053
        add    esp, 4
        test   eax, eax
        je     loc_1_46B6
        mov    word ptr [edi + 0x14], si
        mov    byte ptr [edi + 0x11], bl
        jmp    loc_1_46DB
loc_1_46B6:
        push   edi
        call   sub_1_4154
        add    esp, 4
        mov    ebp, eax
        test   ebp, ebp
        je     loc_1_46D9
        mov    word ptr [edi + 0x14], si
        push   edi
        mov    byte ptr [edi + 0x11], bl
        call   sub_1_4053
        add    esp, 4
        mov    eax, ebp
        jmp    loc_1_46DB
loc_1_46D9:
        xor    eax, eax
loc_1_46DB:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_1_46E3:
        push   ebp
        mov    ax, word ptr [esp + 0x14]
        mov    ebp, esp
        sub    esp, 0x34
        mov    word ptr [ebp - 2], ax
        push   ebx
        push   esi
        xor    eax, eax
        push   edi
        mov    esi, dword ptr [ebp + 0x10]
        lea    edi, [ebp - 0x30]
        mov    ecx, 8
        lea    ebx, [esi + 0x10]
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        stosb  byte ptr es:[edi], al
        test   word ptr [ebp - 2], 0xfe81
        je     loc_1_471C
        mov    eax, 2
        jmp    loc_1_48F6
loc_1_471C:
        push   0x1b
        call   sub_1_2540
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        jne    loc_1_4736
        mov    eax, 0x20
        jmp    loc_1_48F6
loc_1_4736:
        mov    eax, dword ptr [ebp + 0x14]
        and    eax, 0x40000000
        mov    dword ptr [ebp - 0xc], eax
        je     loc_1_478E
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [ebp - 0x1e], eax
        mov    ecx, dword ptr [ebx + 0xc]
        mov    dword ptr [ebp - 0x1a], ecx
        cmp    eax, ecx
        jbe    loc_1_475F
        mov    dword ptr [ebp - 4], 3
        jmp    loc_1_48EA
loc_1_475F:
        mov    eax, dword ptr [ebx]
        mov    ecx, 0xfffff000
        and    eax, ecx
        mov    dword ptr [ebx], eax
        not    eax
        inc    eax
        mov    dword ptr [ebp - 0x12], eax
        mov    eax, dword ptr [ebx + 4]
        add    eax, 0xfff
        and    eax, ecx
        mov    dword ptr [ebx + 4], eax
        mov    ecx, dword ptr [ebp - 0x1e]
        mov    dword ptr [edi + 8], ecx
loc_1_4783:
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_1_47AD
        mov    eax, dword ptr [ebx + 4]
        jmp    loc_1_47B4
loc_1_478E:
        push   dword ptr [esi + 4]
        call   sub_1_2650
        add    esp, 4
        mov    dword ptr [edi + 8], eax
        cmp    eax, -1
        jne    loc_1_4783
        mov    dword ptr [ebp - 4], 3
        jmp    loc_1_48EA
loc_1_47AD:
        mov    eax, dword ptr [esi + 8]
        sub    eax, dword ptr [esi + 4]
        inc    eax
loc_1_47B4:
        push   eax
        push   dword ptr [edi + 8]
        call   sub_1_2678
        add    esp, 8
        mov    dword ptr [edi + 0xc], eax
        cmp    eax, -1
        je     loc_1_48E3
        test   byte ptr [ebp - 2], 0x10
        je     loc_1_47DD
        test   eax, 0x3fff
        jne    loc_1_48E3
loc_1_47DD:
        test   eax, eax
        jne    loc_1_47E9
        mov    eax, dword ptr [0]            ; -> loc_1_1A80
        mov    dword ptr [edi + 0xc], eax
loc_1_47E9:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    byte ptr [edi + 0x1a], 0
        mov    dword ptr [edi + 0x10], eax
        mov    dword ptr [edi + 0x14], ecx
        mov    cl, byte ptr [ebp + 0x18]
        mov    edx, dword ptr [edi + 8]
        mov    dword ptr [ebp - 0x26], edx
        mov    dx, word ptr [ebp - 2]
        mov    eax, dword ptr [edi + 0xc]
        mov    word ptr [ebp - 0x28], dx
        mov    dword ptr [ebp - 0x22], eax
        mov    eax, dword ptr [ebp + 0x14]
        mov    byte ptr [ebp - 0xe], cl
        lea    ecx, [ebp - 8]
        mov    dword ptr [ebp - 0x2c], edi
        push   ecx
        mov    dword ptr [ebp - 0x30], eax
        lea    eax, [ebp - 0x30]
        push   eax
        call   sub_1_3B7B
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    loc_1_48EA
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_1_485D
        mov    eax, dword ptr [ebp - 0x1e]
        push   edi
        mov    dword ptr [ebx + 8], eax
        mov    ecx, dword ptr [ebp - 0x1a]
        mov    dword ptr [ebx + 0xc], ecx
        mov    edx, dword ptr [ebp - 0x22]
        mov    dword ptr [ebx + 4], edx
        call   sub_1_254F
        add    esp, 4
        jmp    loc_1_48DF
loc_1_485D:
        test   byte ptr [ebp + 0x17], 0x10
        je     loc_1_4869
        mov    eax, dword ptr [ebp - 0x26]
        mov    dword ptr [edi + 8], eax
loc_1_4869:
        cmp    dword ptr [ebp + 8], 0        ; -> obj1:0x192C
        jne    loc_1_48BB
        test   byte ptr [ebp + 0x17], 0x80
        jne    loc_1_48BB
        mov    eax, dword ptr [esi + 0x28]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0x16], eax
        push   dword ptr [ebp - 8]
        call   sub_1_4053
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    loc_1_48A5
        push   dword ptr [ebp - 8]
        call   sub_1_4154
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        je     loc_1_48BB
loc_1_48A5:
        push   edi
        call   sub_1_4245
        add    esp, 4
        test   eax, eax
        je     loc_1_48EA
        mov    dword ptr [ebp - 4], 0x19
        jmp    loc_1_48EA
loc_1_48BB:
        push   4
        mov    eax, dword ptr [ebp + 0xc]
        push   edi
        add    eax, 0x24
        push   eax
        call   sub_1_2D57
        add    esp, 0xc
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x1c
        push   0
        push   edi
        push   eax
        call   sub_1_2D57
        add    esp, 0xc
loc_1_48DF:
        xor    eax, eax
        jmp    loc_1_48F6
loc_1_48E3:
        mov    dword ptr [ebp - 4], 0xa
loc_1_48EA:
        push   edi
        call   sub_1_254F
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
loc_1_48F6:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_48FD:
        mov    eax, dword ptr [esp + 8]
        sub    esp, 0xc
        mov    word ptr [esp + 2], 0
        push   ebx
        mov    ecx, dword ptr [eax + 0x18]
        push   esi
        mov    ebx, dword ptr [0xa]          ; -> loc_1_199E
        mov    dword ptr [esp + 0x10], ecx
        push   edi
        push   ebp
        lea    esi, [ebx + 2]
        test   word ptr [esi], 0xfe81
        je     loc_1_4930
        mov    eax, 2
        jmp    loc_1_4B47
loc_1_4930:
        push   dword ptr [ebx + 4]
        call   sub_1_2650
        add    esp, 4
        mov    edi, eax
        cmp    edi, -1
        jne    loc_1_494C
        mov    eax, 3
        jmp    loc_1_4B47
loc_1_494C:
        push   dword ptr [ebx + 8]
        push   edi
        call   sub_1_2678
        add    esp, 8
        cmp    eax, -1
        je     loc_1_4B42
        test   byte ptr [esi], 0x10
        je     loc_1_4971
        test   eax, 0x3fff
        jne    loc_1_4B42
loc_1_4971:
        mov    ecx, dword ptr [esp + 0x18]
        mov    ecx, dword ptr [ecx + 8]
        test   ecx, 0x10000000
        je     loc_1_498A
        mov    eax, 0x1e
        jmp    loc_1_4B47
loc_1_498A:
        test   ecx, 0x8000000
        je     loc_1_4A7E
        mov    edx, 1
        mov    ecx, dword ptr [esp + 0x18]
        mov    ebp, dword ptr [ecx + 0x24]
        test   ebp, ebp
        je     loc_1_49D8
loc_1_49A6:
        cmp    byte ptr [ebp + 0x1a], 0
        jne    loc_1_49D1
        cmp    dword ptr [ebp + 0x10], 0     ; -> obj1:0x192C
        jne    loc_1_49D1
        xor    edx, edx
        test   edi, edi
        je     loc_1_49C0
        cmp    dword ptr [ebp + 8], edi
        jne    loc_1_49D1
loc_1_49C0:
        test   eax, eax
        je     loc_1_49D8
        cmp    dword ptr [ebp + 0xc], eax
        je     loc_1_49D8
        test   edi, edi
        jne    loc_1_4B42
loc_1_49D1:
        mov    ebp, dword ptr [ebp + 4]
        test   ebp, ebp
        jne    loc_1_49A6
loc_1_49D8:
        test   edx, edx
        je     loc_1_49E6
        mov    eax, 0x20
        jmp    loc_1_4B47
loc_1_49E6:
        test   ebp, ebp
        jne    loc_1_49FA
        cmp    edi, 1
        sbb    eax, eax
        and    eax, 0x1d
        add    eax, 3
        jmp    loc_1_4B47
loc_1_49FA:
        push   ebp
        push   dword ptr [esp + 0x24]
        call   sub_1_6288
        add    esp, 8
        push   ebp
        call   sub_1_3B5C
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        jne    loc_1_4A20
        mov    eax, 0x20
        jmp    loc_1_4B47
loc_1_4A20:
        mov    ax, word ptr [edi + 0x14]
        mov    cx, word ptr [esi]
        mov    word ptr [esp + 0x12], ax
        mov    word ptr [edi + 0x14], cx
loc_1_4A30:
        push   edi
        call   sub_1_4053
        mov    dword ptr [esp + 0x18], eax
        add    esp, 4
        test   eax, eax
        jne    loc_1_4AF7
        push   edi
        call   sub_1_4154
        mov    dword ptr [esp + 0x18], eax
        add    esp, 4
        test   eax, eax
        jne    loc_1_4AF7
        mov    ax, word ptr [edi + 0xe]
        mov    word ptr [2], ax              ; -> loc_1_1996
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ebx + 4], ecx
        mov    edx, dword ptr [ebp + 0xc]
        mov    dword ptr [ebx + 8], edx
        mov    ax, word ptr [edi + 0x14]
        mov    word ptr [esi], ax
        xor    eax, eax
        jmp    loc_1_4B47
loc_1_4A7E:
        mov    edx, 0                        ; -> loc_1_1A58
        xor    eax, eax
        mov    edi, edx
        mov    ecx, 9
        rep stosd dword ptr es:[edi], eax
        mov    word ptr [0], 1               ; -> loc_1_1A58
        mov    word ptr [2], 0x14            ; -> loc_1_1A5A
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [4], eax            ; -> loc_1_1A5C
        mov    eax, dword ptr [ebx + 4]
        add    eax, dword ptr [ebx + 8]
        dec    eax
        mov    dword ptr [8], eax            ; -> loc_1_1A60
        movzx  eax, word ptr [esi]
        mov    cl, byte ptr [ebx + 0xc]
        or     eax, 0x10000000
        push   ecx
        push   eax
        push   edx
        push   dword ptr [esp + 0x24]
        push   dword ptr [esp + 0x30]
        call   sub_1_46E3
        add    esp, 0x14
        test   eax, eax
        jne    loc_1_4B47
        mov    eax, dword ptr [esp + 0x20]
        mov    ebp, dword ptr [eax + 0x1c]
        push   ebp
        call   sub_1_3B5C
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        jne    loc_1_4A30
        mov    eax, 0x20
        jmp    loc_1_4B47
loc_1_4AF7:
        mov    ecx, dword ptr [esp + 0x18]
        push   ebp
        test   byte ptr [ecx + 0xb], 8
        je     loc_1_4B2C
        push   0                             ; -> obj1:0x192C
        call   sub_1_6288
        mov    ax, word ptr [esp + 0x1a]
        add    esp, 8
        mov    word ptr [edi + 0x14], ax
        push   edi
        call   sub_1_4053
        add    esp, 4
        push   edi
        call   sub_1_4154
        add    esp, 4
        jmp    loc_1_4B3C
loc_1_4B2C:
        call   sub_1_62B7
        mov    dword ptr [esp + 0x18], eax
        add    esp, 4
        test   eax, eax
        jne    loc_1_4B47
loc_1_4B3C:
        mov    eax, dword ptr [esp + 0x14]
        jmp    loc_1_4B47
loc_1_4B42:
        mov    eax, 0xa
loc_1_4B47:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
sub_1_4B4F:
        mov    eax, dword ptr [esp + 4]
        push   esi
        mov    esi, dword ptr [eax + 0x1a]
        mov    ecx, dword ptr [esi + 0x14]
        test   byte ptr [ecx + 0xb], 8
        je     loc_1_4B83
        and    word ptr [eax + 0x14], 0xfffb
        push   eax
        call   sub_1_4053
        add    esp, 4
        test   eax, eax
        jne    loc_1_4B92
        push   esi
        push   0                             ; -> obj1:0x192C
        call   sub_1_6288
        add    esp, 8
        jmp    loc_1_4B90
loc_1_4B83:
        push   esi
        call   sub_1_62B7
        add    esp, 4
        test   eax, eax
        jne    loc_1_4B92
loc_1_4B90:
        xor    eax, eax
loc_1_4B92:
        pop    esi
        ret    
loc_1_4B94:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [0xa]          ; -> loc_1_199E
        push   dword ptr [edi]
        call   sub_1_2664
        add    esp, 4
        mov    ecx, eax
        cmp    ecx, -1
        je     loc_1_4C0A
        mov    esi, dword ptr [esp + 0x14]
        mov    bx, word ptr [esi + 0x14]
        test   bh, 1
        je     loc_1_4BC5
        sub    edx, edx
        div    dword ptr [esi + 0x22]
        test   edx, edx
        jne    loc_1_4C0A
loc_1_4BC5:
        mov    al, byte ptr [edi + 4]
        test   al, al
        je     loc_1_4BE2
        test   bl, 0x10
        je     loc_1_4BDB
        mov    edx, dword ptr [esi + 0x22]
        shr    edx, 0xe
        cmp    dl, al
        ja     loc_1_4BE2
loc_1_4BDB:
        mov    eax, 8
        jmp    loc_1_4C0F
loc_1_4BE2:
        mov    bl, byte ptr [esi + 0x12]
        lea    ebp, [esi + 0x16]
        push   esi
        mov    byte ptr [esi + 0x12], al
        mov    edi, dword ptr [ebp]
        mov    dword ptr [ebp], ecx
        call   sub_1_4154
        add    esp, 4
        test   eax, eax
        je     loc_1_4C06
        mov    byte ptr [esi + 0x12], bl
        mov    dword ptr [ebp], edi
        jmp    loc_1_4C0F
loc_1_4C06:
        xor    eax, eax
        jmp    loc_1_4C0F
loc_1_4C0A:
        mov    eax, 7
loc_1_4C0F:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_4C14:
        push   dword ptr [esp + 4]
        call   sub_1_4F83
        add    esp, 4
        xor    edx, edx
        test   eax, eax
        je     loc_1_4C40
        push   dword ptr [eax + 4]
        movzx  ecx, word ptr [esp + 0xa]
        add    eax, 0xc
        push   eax
        push   edx
        push   ecx
        push   edx
        push   edx
        push   0x15
        call   sub_1_24A2
        add    esp, 0x1c
loc_1_4C40:
        ret    
loc_1_4C41:
        push   ebp
        mov    ecx, dword ptr [0xa]          ; -> loc_1_199E
        mov    dx, word ptr [0]              ; -> loc_1_1BE0
        mov    ebp, esp
        sub    esp, 4
        mov    eax, dword ptr [ebp + 8]
        movzx  ecx, word ptr [ecx]
        mov    ax, word ptr [eax + 0x16]
        mov    word ptr [ebp - 4], ax
        mov    word ptr [ebp - 2], dx
        push   dword ptr [ebp - 4]
        push   ecx
        push   0                             ; -> loc_1_4C14
        call   sub_1_2566
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_4C82
        mov    eax, 0x20
        jmp    loc_1_4C97
loc_1_4C82:
        mov    ax, word ptr [0]              ; -> loc_1_1BE0
        mov    word ptr [2], ax              ; -> loc_1_1996
        xor    eax, eax
        inc    word ptr [0]                  ; -> loc_1_1BE0
loc_1_4C97:
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_4C9B:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        cmp    word ptr [0], 0x37            ; -> loc_1_1994
        push   esi
        push   edi
        jae    loc_1_4EB0
        movzx  eax, word ptr [0]             ; -> loc_1_1994
        imul   eax, eax, 0xc
        lea    esi, [eax]                    ; -> loc_1_1E7C
        cmp    dword ptr [esi + 4], 0
        je     loc_1_4EB0
        test   byte ptr [esi], 0x20
        mov    ax, word ptr [esi + 8]
        je     loc_1_4CE5
        cmp    word ptr [8], ax              ; -> loc_1_199C
        mov    eax, 1
        adc    eax, -1
        jmp    loc_1_4CF3
loc_1_4CE5:
        sub    ax, word ptr [8]              ; -> loc_1_199C
        cmp    ax, 1
        sbb    eax, eax
        inc    eax
loc_1_4CF3:
        test   eax, eax
        je     loc_1_4D01
        mov    eax, 0x1b
        jmp    loc_1_4EB5
loc_1_4D01:
        xor    edi, edi
        mov    dword ptr [ebp - 8], edi
        mov    eax, dword ptr [esi]
        and    eax, 7
        je     loc_1_4D3B
        cmp    eax, 1
        je     loc_1_4DA1
        cmp    eax, 2
        je     loc_1_4DC8
        cmp    eax, 3
        je     loc_1_4E0D
        cmp    eax, 4
        je     loc_1_4E3B
        mov    eax, 0x19
        jmp    loc_1_4EB5
loc_1_4D3B:
        mov    dword ptr [ebp - 4], 0
loc_1_4D42:
        mov    eax, dword ptr [esi]
        test   al, 0x40
        je     loc_1_4D5C
        mov    ecx, dword ptr [0xa]          ; -> loc_1_199E
        movzx  edx, byte ptr [esi + 0xa]
        test   byte ptr [edx + ecx], 1
        je     loc_1_4EA2
loc_1_4D5C:
        test   al, 8
        je     loc_1_4D84
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [0xa]          ; -> loc_1_199E
        push   eax
        movzx  edx, byte ptr [esi + 0xb]
        movzx  ax, byte ptr [edx + ecx]
        push   eax
        call   sub_1_9645
        add    esp, 8
        test   eax, eax
        jne    loc_1_4EB5
loc_1_4D84:
        test   byte ptr [esi], 0x10
        je     loc_1_4E7B
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_4E5C
        mov    eax, 0x19
        jmp    loc_1_4EB5
loc_1_4DA1:
        mov    ax, word ptr [2]              ; -> loc_1_1996
        push   eax
        call   sub_1_4F83
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        jne    loc_1_4DC0
        mov    eax, 0x21
        jmp    loc_1_4EB5
loc_1_4DC0:
        mov    dword ptr [ebp - 4], edi
        jmp    loc_1_4D42
loc_1_4DC8:
        mov    cx, word ptr [2]              ; -> loc_1_1996
        lea    eax, [ebp - 4]
        push   eax
        push   ecx
        call   sub_1_3AE5
        add    esp, 8
        test   eax, eax
        jne    loc_1_4EB5
        mov    eax, dword ptr [ebp - 4]
        test   byte ptr [eax + 0xc], 2
        jne    loc_1_4DF7
        mov    eax, 0x21
        jmp    loc_1_4EB5
loc_1_4DF7:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 0x1a]
        mov    edx, dword ptr [ecx + 0x14]
        mov    eax, dword ptr [edx]
        mov    dword ptr [ebp - 8], eax
        mov    edi, dword ptr [ecx + 0x10]
        jmp    loc_1_4D42
loc_1_4E0D:
        mov    ax, word ptr [2]              ; -> loc_1_1996
        push   eax
        call   sub_1_0499
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    loc_1_4E2D
        mov    eax, 0x21
        jmp    loc_1_4EB5
loc_1_4E2D:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 0xc]
        mov    dword ptr [ebp - 8], ecx
        jmp    loc_1_4D42
loc_1_4E3B:
        mov    ax, word ptr [2]              ; -> loc_1_1996
        push   eax
        call   sub_1_086E
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    loc_1_4D42
        mov    eax, 0x21
        jmp    loc_1_4EB5
loc_1_4E5C:
        push   dword ptr [ebp - 8]
        call   sub_1_27C1
        add    esp, 4
        test   eax, eax
        jne    loc_1_4EB5
        test   byte ptr [3], 0x80            ; -> obj1:0x196F
        jne    loc_1_4E7B
        mov    eax, 0x14
        jmp    loc_1_4EB5
loc_1_4E7B:
        test   byte ptr [esi], 0x80
        je     loc_1_4EA2
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_4E8D
        mov    eax, 0x19
        jmp    loc_1_4EB5
loc_1_4E8D:
        mov    eax, dword ptr [ebp - 8]
        test   byte ptr [eax + 0x17], 0x40
        je     loc_1_4EA2
        cmp    dword ptr [eax + 0x1c], edi
        je     loc_1_4EA2
        mov    eax, 0x14
        jmp    loc_1_4EB5
loc_1_4EA2:
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 4]
        call   dword ptr [esi + 4]
        add    esp, 8
        jmp    loc_1_4EB5
loc_1_4EB0:
        mov    eax, 0x15
loc_1_4EB5:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_4EBB:
        push   esi
        mov    edx, dword ptr [0xa]          ; -> loc_1_199E
        movzx  eax, word ptr [8]             ; -> loc_1_199C
        push   edi
        sub    eax, 4
        cmp    eax, 9
        ja     loc_1_4EE0
        movzx  ecx, byte ptr [eax]           ; -> obj1:0x4F79
        jmp    dword ptr [ecx*4]             ; -> obj1:0x4F61
loc_1_4EE0:
        cmp    word ptr [8], 0x4b            ; -> loc_1_199C
        mov    ax, 0x3d
        jae    loc_1_4EF8
        mov    ax, word ptr [8]              ; -> loc_1_199C
        sub    ax, 0xe
loc_1_4EF8:
        mov    esi, 0                        ; -> loc_1_1B9C
        mov    edi, dword ptr [0xa]          ; -> loc_1_199E
        movzx  eax, ax
        add    edi, 0xe
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    word ptr [edx + 0xc], 0x3d
loc_1_4F1D:
        mov    word ptr [edx + 0xa], 0xe
loc_1_4F23:
        mov    ax, word ptr [0]              ; -> loc_1_23BE
        mov    word ptr [edx + 8], ax
loc_1_4F2D:
        mov    ax, word ptr [0]              ; -> obj1:0x23BC
        mov    word ptr [edx + 6], ax
loc_1_4F37:
        xor    ecx, ecx
        mov    word ptr [edx + 4], cx
        mov    eax, dword ptr [0]            ; -> obj1:0x191C
        cmp    eax, ecx
        je     loc_1_4F51
loc_1_4F46:
        inc    word ptr [edx + 4]
        mov    eax, dword ptr [eax + 4]
        test   eax, eax
        jne    loc_1_4F46
loc_1_4F51:
        mov    word ptr [edx], 0x4b
        xor    eax, eax
        mov    word ptr [edx + 2], 0x5343
        pop    edi
        pop    esi
        ret    
        ; ---- dispatch table (6 entries) ----
        dd  loc_1_4F51
        dd  loc_1_4F37
        dd  loc_1_4F2D
        dd  loc_1_4F23
        dd  loc_1_4F1D
        dd  loc_1_4EE0
        db  000h, 000h, 001h, 001h, 002h, 002h, 003h, 003h, 004h, 004h
sub_1_4F83:
        mov    cx, word ptr [esp + 4]
        push   esi
        cmp    word ptr [0x16], cx           ; -> obj1:0x1942
        jne    loc_1_4F99
        mov    eax, 0                        ; -> obj1:0x192C
        jmp    loc_1_4FF4
loc_1_4F99:
        xor    dx, dx
        xor    esi, esi
loc_1_4F9E:
        movzx  eax, dx
        test   eax, eax
        je     loc_1_4FB8
        cmp    eax, 1
        je     loc_1_4FBF
        cmp    eax, 2
        je     loc_1_4FC6
        xor    eax, eax
        mov    esi, 1
        jmp    loc_1_4FCB
loc_1_4FB8:
        mov    eax, dword ptr [0]            ; -> obj1:0x1908
        jmp    loc_1_4FCB
loc_1_4FBF:
        mov    eax, dword ptr [0]            ; -> obj1:0x1910
        jmp    loc_1_4FCB
loc_1_4FC6:
        mov    eax, dword ptr [0]            ; -> obj1:0x190C
loc_1_4FCB:
        test   eax, eax
        je     loc_1_4FEE
loc_1_4FCF:
        test   esi, esi
        jne    loc_1_4FEE
        cmp    word ptr [eax + 0x16], cx
        jne    loc_1_4FE8
        cmp    byte ptr [eax + 0x20], 2
        jne    loc_1_4FE1
        xor    eax, eax
loc_1_4FE1:
        mov    esi, 1
        jmp    loc_1_4FEA
loc_1_4FE8:
        mov    eax, dword ptr [eax]
loc_1_4FEA:
        test   eax, eax
        jne    loc_1_4FCF
loc_1_4FEE:
        inc    dx
        test   esi, esi
        je     loc_1_4F9E
loc_1_4FF4:
        pop    esi
        ret    
sub_1_4FF6:
        xor    dx, dx
        mov    eax, dword ptr [esp + 4]
        test   eax, eax
        je     loc_1_5031
        xor    ecx, ecx
        mov    cx, word ptr [eax + 8]
        and    ecx, 7
        cmp    ecx, 1
        je     loc_1_501B
        cmp    ecx, 2
        je     loc_1_5021
        cmp    ecx, 4
        je     loc_1_5027
        jmp    loc_1_502B
loc_1_501B:
        mov    dx, 3
        jmp    loc_1_502B
loc_1_5021:
        mov    dx, 2
        jmp    loc_1_502B
loc_1_5027:
        mov    dx, 1
loc_1_502B:
        mov    eax, dword ptr [eax]
        test   eax, eax
        jne    loc_1_5044
loc_1_5031:
        movzx  eax, dx
        test   eax, eax
        je     loc_1_5045
        cmp    eax, 1
        je     loc_1_504C
        cmp    eax, 2
        je     loc_1_5053
        xor    eax, eax
loc_1_5044:
        ret    
loc_1_5045:
        mov    eax, dword ptr [0]            ; -> obj1:0x1908
        jmp    loc_1_5058
loc_1_504C:
        mov    eax, dword ptr [0]            ; -> obj1:0x1910
        jmp    loc_1_5058
loc_1_5053:
        mov    eax, dword ptr [0]            ; -> obj1:0x190C
loc_1_5058:
        test   eax, eax
        jne    loc_1_5044
        inc    dx
        jmp    loc_1_5031
sub_1_5060:
        push   esi
        push   edi
        mov    edi, dword ptr [0]            ; -> obj1:0x191C
        test   edi, edi
        je     loc_1_50B8
        mov    esi, dword ptr [esp + 0xc]
loc_1_5070:
        mov    ecx, edi
        call   sub_1_2967
        test   al, 0x80
        je     loc_1_50B1
        mov    eax, dword ptr [edi + 0x14]
        and    eax, 0x40000000
        je     loc_1_508B
        test   byte ptr [esi + 8], 0x10
        jne    loc_1_5095
loc_1_508B:
        test   eax, eax
        jne    loc_1_50B1
        test   byte ptr [esi + 8], 8
        je     loc_1_50B1
loc_1_5095:
        push   dword ptr [esi + 4]
        mov    dx, word ptr [edi + 0x10]
        lea    ecx, [esi + 0xc]
        push   ecx
        push   0
        push   0
        push   0
        push   edx
        push   0x40
        call   sub_1_24A2
        add    esp, 0x1c
loc_1_50B1:
        mov    edi, dword ptr [edi + 4]
        test   edi, edi
        jne    loc_1_5070
loc_1_50B8:
        pop    edi
        pop    esi
        ret    
loc_1_50BB:
        push   esi
        mov    esi, dword ptr [esp + 8]
        movzx  eax, byte ptr [esi + 0x20]
        cmp    eax, 1
        je     loc_1_50D3
        cmp    eax, 2
        je     loc_1_5124
        jmp    loc_1_5168
loc_1_50D3:
        mov    eax, dword ptr [0]            ; -> obj1:0x191C
        test   eax, eax
        je     loc_1_50EA
loc_1_50DC:
        cmp    word ptr [eax + 0x32], 0
        jne    loc_1_5112
        mov    eax, dword ptr [eax + 4]
        test   eax, eax
        jne    loc_1_50DC
loc_1_50EA:
        push   esi
        call   sub_1_5060
        add    esp, 4
        xor    ecx, ecx
        push   dword ptr [esi + 4]
        mov    byte ptr [esi + 0x20], cl
        add    esi, 0xc
        push   esi
        push   ecx
        push   ecx
        push   ecx
        push   ecx
        push   0x82
        call   sub_1_24A2
        add    esp, 0x1c
        jmp    loc_1_5168
loc_1_5112:
        push   esi
        push   0x32
        push   0                             ; -> loc_1_50BB
        call   sub_1_2566
        add    esp, 0xc
        jmp    loc_1_5168
loc_1_5124:
        xor    eax, eax
        mov    ax, word ptr [esi + 8]
        and    eax, 7
        cmp    eax, 1
        je     loc_1_5140
        cmp    eax, 2
        je     loc_1_5147
        cmp    eax, 4
        je     loc_1_514E
        xor    eax, eax
        jmp    loc_1_5153
loc_1_5140:
        mov    eax, 0                        ; -> obj1:0x190C
        jmp    loc_1_5153
loc_1_5147:
        mov    eax, 0                        ; -> obj1:0x1910
        jmp    loc_1_5153
loc_1_514E:
        mov    eax, 0                        ; -> obj1:0x1908
loc_1_5153:
        push   0
        push   esi
        push   eax
        call   sub_1_2E0B
        add    esp, 0xc
        push   esi
        call   sub_1_254F
        add    esp, 4
loc_1_5168:
        pop    esi
        ret    
loc_1_516A:
        sub    esp, 4
        mov    eax, dword ptr [0xa]          ; -> loc_1_199E
        mov    dword ptr [esp], eax
        push   ebx
        push   esi
        xor    ebx, ebx
        test   word ptr [eax], 0xffe0
        push   edi
        push   ebp
        je     loc_1_518D
        mov    eax, 2
        jmp    loc_1_52D8
loc_1_518D:
        mov    eax, dword ptr [0xa]          ; -> loc_1_199E
        test   byte ptr [eax + 3], 0xf8
        jne    loc_1_52D3
        mov    eax, dword ptr [0xa]          ; -> loc_1_199E
        mov    ax, word ptr [eax + 0xc]
        cmp    word ptr [0], ax              ; -> loc_1_23BE
        jb     loc_1_52D3
        cmp    ax, 0x201
        jb     loc_1_52D3
        push   0x21
        call   sub_1_2540
        add    esp, 4
        mov    ebp, eax
        test   ebp, ebp
        jne    loc_1_51D6
        mov    eax, 0x20
        jmp    loc_1_52D8
loc_1_51D6:
        mov    ax, word ptr [0]              ; -> loc_1_1BDC
        lea    edi, [ebp + 8]
        mov    word ptr [ebp + 0x16], ax
        mov    esi, dword ptr [esp + 0x10]
        inc    word ptr [0]                  ; -> loc_1_1BDC
        mov    byte ptr [ebp + 0x20], 1
        mov    ecx, dword ptr [4]            ; -> loc_1_1998
        mov    dword ptr [ebp + 4], ecx
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        xor    esi, esi
        mov    dword ptr [ebp + 0x18], esi
        mov    dword ptr [ebp + 0x1c], esi
        movzx  ecx, byte ptr [0]             ; -> loc_1_1BF0
        not    ecx
        mov    al, byte ptr [ebp + 0xa]
        movzx  edx, al
        test   edx, ecx
        je     loc_1_522C
        or     byte ptr [0], al              ; -> loc_1_1BF0
        call   sub_1_9619
        cmp    eax, esi
        mov    edi, eax
        jne    loc_1_5273
loc_1_522C:
        test   byte ptr [ebp + 8], 0x18
        je     loc_1_524F
        push   ebp
        push   0x32
        push   0                             ; -> loc_1_50BB
        call   sub_1_2566
        add    esp, 0xc
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_1_5253
        mov    edi, 0x19
        jmp    loc_1_5273
loc_1_524F:
        mov    byte ptr [ebp + 0x20], 0
loc_1_5253:
        xor    ecx, ecx
        mov    eax, dword ptr [esp + 0x10]
        mov    cx, word ptr [eax]
        and    ecx, 7
        cmp    ecx, 1
        je     loc_1_5297
        cmp    ecx, 2
        je     loc_1_52A6
        cmp    ecx, 4
        je     loc_1_52B5
        mov    edi, 2
loc_1_5273:
        call   sub_1_2A6F
        call   sub_1_9619
        test   ebx, ebx
        je     loc_1_528A
        push   ebx
        call   sub_1_25AF
        add    esp, 4
loc_1_528A:
        push   ebp
        call   sub_1_254F
        add    esp, 4
        mov    eax, edi
        jmp    loc_1_52D8
loc_1_5297:
        push   0
        push   ebp
        push   0                             ; -> obj1:0x190C
        call   sub_1_2D7C
        jmp    loc_1_52C2
loc_1_52A6:
        push   0
        push   ebp
        push   0                             ; -> obj1:0x1910
        call   sub_1_2D7C
        jmp    loc_1_52C2
loc_1_52B5:
        push   0
        push   ebp
        push   0                             ; -> obj1:0x1908
        call   sub_1_2D57
loc_1_52C2:
        mov    ax, word ptr [ebp + 0x16]
        add    esp, 0xc
        mov    word ptr [2], ax              ; -> loc_1_1996
        xor    eax, eax
        jmp    loc_1_52D8
loc_1_52D3:
        mov    eax, 0x1c
loc_1_52D8:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_1_52E0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    byte ptr [esi + 0x20], 1
        jne    loc_1_52F5
        mov    eax, 0x21
        jmp    loc_1_537A
loc_1_52F5:
        xor    ecx, ecx
        cmp    dword ptr [esi + 0x1c], ecx
        jne    loc_1_5375
        cmp    dword ptr [esi + 0x18], ecx
        jne    loc_1_5375
        mov    eax, dword ptr [0]            ; -> obj1:0x191C
        cmp    eax, ecx
        je     loc_1_531C
loc_1_530A:
        test   byte ptr [eax + 0x17], 0x40
        je     loc_1_5315
        cmp    dword ptr [eax + 0x1c], esi
        je     loc_1_5375
loc_1_5315:
        mov    eax, dword ptr [eax + 4]
        test   eax, eax
        jne    loc_1_530A
loc_1_531C:
        mov    eax, dword ptr [0]            ; -> obj1:0x1924
        test   eax, eax
        je     loc_1_5330
loc_1_5325:
        cmp    dword ptr [eax + 0x10], esi
        je     loc_1_5375
        mov    eax, dword ptr [eax]
        test   eax, eax
        jne    loc_1_5325
loc_1_5330:
        test   byte ptr [esi + 8], 2
        je     loc_1_533F
        push   esi
        call   sub_1_091B
        add    esp, 4
loc_1_533F:
        mov    byte ptr [esi + 0x20], 2
        push   esi
        push   0x32
        push   0                             ; -> loc_1_50BB
        call   sub_1_2566
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_535E
        mov    eax, 0x19
        jmp    loc_1_537A
loc_1_535E:
        call   sub_1_2A6F
        call   sub_1_9619
        mov    word ptr [2], 0               ; -> loc_1_1996
        xor    eax, eax
        jmp    loc_1_537A
loc_1_5375:
        mov    eax, 0x1e
loc_1_537A:
        pop    esi
        ret    
loc_1_537C:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [0xa]          ; -> loc_1_199E
        push   ebp
        add    esi, 2
        test   word ptr [esi], 0xfffe
        je     loc_1_5397
        mov    eax, 2
        jmp    loc_1_53DF
loc_1_5397:
        xor    ebx, ebx
        mov    edi, dword ptr [esp + 0x14]
        mov    ebp, 1
loc_1_53A2:
        push   edi
        call   sub_1_4FF6
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        je     loc_1_53E4
        cmp    byte ptr [edi + 0x20], 2
        je     loc_1_53CF
        test   word ptr [esi], bp
        je     loc_1_53CD
        push   dword ptr [esp + 0x18]
        push   edi
        call   sub_1_2635
        add    esp, 8
        test   eax, eax
        je     loc_1_53CF
loc_1_53CD:
        mov    ebx, ebp
loc_1_53CF:
        test   ebx, ebx
        je     loc_1_53A2
        mov    ax, word ptr [edi + 0x16]
        mov    word ptr [2], ax              ; -> loc_1_1996
        xor    eax, eax
loc_1_53DF:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_53E4:
        mov    eax, 0x1f
        jmp    loc_1_53DF
loc_1_53EB:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [0xa]          ; -> loc_1_199E
        mov    edi, dword ptr [esp + 0x10]
        xor    ebx, ebx
        push   dword ptr [edi + 4]
        lea    ecx, [edi + 0xc]
        push   ecx
        push   ebx
        push   esi
        push   ebx
        push   ebx
        push   0x14
        call   sub_1_24A2
        add    esp, 0x1c
        cmp    byte ptr [esi + 5], bl
        jne    loc_1_5430
        mov    ax, word ptr [edi + 8]
        mov    word ptr [esi + 4], ax
        cmp    word ptr [8], 0xa             ; -> loc_1_199C
        jb     loc_1_5430
        mov    ax, word ptr [0]              ; -> loc_1_23BE
        mov    word ptr [esi + 8], ax
loc_1_5430:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_5436:
        push   ebx
        mov    eax, dword ptr [0xa]          ; -> loc_1_199E
        cmp    word ptr [eax + 2], 0
        push   esi
        push   edi
        push   ebp
        je     loc_1_5450
        mov    eax, 2
        jmp    loc_1_54E5
loc_1_5450:
        cmp    word ptr [0], 0x2d            ; -> loc_1_1994
        jne    loc_1_5474
        mov    edi, dword ptr [esp + 0x18]
        mov    esi, dword ptr [esp + 0x14]
        cmp    dword ptr [edi + 0x1c], esi
        je     loc_1_546E
        mov    eax, 0x21
        jmp    loc_1_54E5
loc_1_546E:
        mov    bx, 0xfe
        jmp    loc_1_5499
loc_1_5474:
        mov    bx, 0xe
        mov    edi, dword ptr [esp + 0x18]
        mov    eax, dword ptr [edi + 0x14]
        test   eax, 0x60000000
        je     loc_1_548D
        mov    eax, 0x1e
        jmp    loc_1_54E5
loc_1_548D:
        or     eax, 0x20000000
        mov    esi, dword ptr [esp + 0x14]
        mov    dword ptr [edi + 0x14], eax
loc_1_5499:
        push   0xa
        call   sub_1_2540
        add    esp, 4
        mov    ebp, eax
        test   ebp, ebp
        jne    loc_1_54B4
        and    byte ptr [edi + 0x17], 0xdf
        mov    eax, 0x20
        jmp    loc_1_54E5
loc_1_54B4:
        mov    word ptr [ebp + 8], bx
        push   ebp
        push   0x32
        mov    dword ptr [ebp], edi
        push   0                             ; -> loc_1_5F1F
        mov    dword ptr [ebp + 4], esi
        call   sub_1_2566
        add    esp, 0xc
        test   eax, eax
        mov    eax, 0
        jne    loc_1_54E5
        push   ebp
        call   sub_1_254F
        add    esp, 4
        mov    eax, 0x19
loc_1_54E5:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_54EA:
        mov    eax, dword ptr [0xa]          ; -> loc_1_199E
        cmp    word ptr [eax], 3
        mov    eax, 0x16
        jne    loc_1_5506
        mov    dword ptr [4], 0              ; -> loc_1_1998
        xor    eax, eax
loc_1_5506:
        ret    
sub_1_5507:
        push   ebp
        mov    cl, byte ptr [0]              ; -> obj1:0x196C
        mov    ebp, esp
        sub    esp, 4
        lea    eax, [ebp - 4]
        push   eax
        push   ecx
        call   sub_1_269E
        add    esp, 8
        test   eax, eax
        je     loc_1_552B
        mov    byte ptr [1], al              ; -> obj1:0x196D
        jmp    loc_1_5559
loc_1_552B:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 0x18]
        mov    dword ptr [0x14], ecx         ; -> sub_1_1980
        mov    edx, dword ptr [eax + 0x1c]
        mov    dword ptr [0xa], edx          ; -> obj1:0x1976
        mov    al, byte ptr [eax + 0xc]
        mov    byte ptr [0], al              ; -> obj1:0x196C
        call   sub_1_2438
        mov    ecx, dword ptr [ebp - 4]
        mov    dl, byte ptr [ecx + 0xf]
        mov    byte ptr [0], dl              ; -> obj1:0x196C
loc_1_5559:
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_555D:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        lea    esi, [ecx + 0xc]
        mov    edi, ecx
        and    byte ptr [ecx + 0x14], 0xdf
        mov    byte ptr [1], 0x90            ; -> obj1:0x196D
        mov    eax, dword ptr [esi]
        mov    al, byte ptr [eax + 0xc]
        mov    byte ptr [0], al              ; -> obj1:0x196C
        mov    cl, byte ptr [edi + 0x12]
        mov    byte ptr [2], cl              ; -> obj1:0x196E
        mov    edx, dword ptr [esi]
        mov    eax, dword ptr [edx + 0x18]
        mov    dword ptr [0x14], eax         ; -> sub_1_1980
        mov    edx, dword ptr [esi]
        mov    ecx, dword ptr [edx + 0x1c]
        mov    dword ptr [0xa], ecx          ; -> obj1:0x1976
        call   sub_1_2438
        test   eax, eax
        jne    loc_1_560E
        test   byte ptr [edi + 0x17], 0x10
        je     loc_1_560C
        push   0
        lea    ecx, [ebp - 2]
        push   2
        mov    eax, dword ptr [esi]
        push   ecx
        push   0x3e
        push   0
        push   dword ptr [eax + 4]
        call   sub_3_52A0
loc_1_55C2:
        add    esp, 0x18
        test   eax, eax
        je     loc_1_55D0
        mov    eax, 0x19
        jmp    loc_1_560E
loc_1_55D0:
        push   0
        lea    eax, [ebp - 2]
        push   2
        mov    ecx, dword ptr [esi]
        or     byte ptr [ebp - 2], 0x40
        push   eax
        push   0x3e
        push   1
        push   dword ptr [ecx + 4]
        call   sub_3_52A0
loc_1_55EA:
        and    word ptr [ebp - 2], 0xffbf
        add    esp, 0x18
        lea    ecx, [ebp - 2]
        mov    edx, dword ptr [esi]
        push   0
        push   2
        push   ecx
        push   0x3e
        push   1
        push   dword ptr [edx + 4]
        call   sub_3_52A0
loc_1_5609:
        add    esp, 0x18
loc_1_560C:
        xor    eax, eax
loc_1_560E:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_5614:
        push   esi
        mov    eax, 0
        test   byte ptr [ecx + 0x14], 0x20
        mov    esi, ecx
        je     loc_1_5632
        mov    ecx, esi
        call   sub_1_555D
        push   esi
        call   sub_1_2CC4
        add    esp, 4
loc_1_5632:
        pop    esi
        ret    
sub_1_5634:
        push   esi
        mov    esi, ecx
        lea    ecx, [ecx + 0xc]
        mov    eax, dword ptr [ecx]
        mov    edx, dword ptr [eax + 0x18]
        mov    dword ptr [0x14], edx         ; -> sub_1_1980
        mov    eax, dword ptr [ecx]
        mov    edx, dword ptr [eax + 0x1c]
        mov    byte ptr [1], 0x8d            ; -> obj1:0x196D
        mov    dword ptr [0xa], edx          ; -> obj1:0x1976
        mov    eax, dword ptr [ecx]
        mov    al, byte ptr [eax + 0xc]
        mov    byte ptr [0], al              ; -> obj1:0x196C
        mov    cl, byte ptr [esi + 0x12]
        mov    byte ptr [2], cl              ; -> obj1:0x196E
        call   sub_1_2438
        test   eax, eax
        jne    loc_1_569F
        mov    byte ptr [1], 0x8e            ; -> obj1:0x196D
        push   esi
        call   sub_1_954E
        add    esp, 4
        mov    byte ptr [3], al              ; -> obj1:0x196F
        mov    word ptr [8], 0               ; -> obj1:0x1974
        mov    word ptr [0xe], 0x100         ; -> obj1:0x197A
        call   sub_1_2438
loc_1_569F:
        pop    esi
        ret    
sub_1_56A1:
        sub    esp, 4
        push   esi
        mov    eax, dword ptr [ecx + 0x18]
        mov    esi, ecx
        xor    cl, cl
        cmp    byte ptr [esi + 0x30], cl
        jbe    loc_1_56C6
        xor    edx, edx
loc_1_56B3:
        mov    byte ptr [eax + 0x14], dl
        inc    cl
        mov    byte ptr [eax + 0x15], dl
        mov    byte ptr [eax + 0x16], dl
        add    eax, 0x28
        cmp    byte ptr [esi + 0x30], cl
        ja     loc_1_56B3
loc_1_56C6:
        lea    eax, [esp + 6]
        push   eax
        xor    eax, eax
        push   eax
        push   eax
        push   eax
        push   eax
        push   dword ptr [esi + 0x18]
        call   sub_1_6B27
        add    esp, 0x18
        test   eax, eax
        je     loc_1_56E7
        mov    eax, 0x19
        jmp    loc_1_574A
loc_1_56E7:
        lea    ecx, [esi + 0xc]
        mov    eax, dword ptr [ecx]
        mov    edx, dword ptr [eax + 0x18]
        mov    dword ptr [0x14], edx         ; -> sub_1_1980
        mov    eax, dword ptr [ecx]
        mov    edx, dword ptr [eax + 0x1c]
        mov    byte ptr [1], 0x8d            ; -> obj1:0x196D
        mov    dword ptr [0xa], edx          ; -> obj1:0x1976
        mov    eax, dword ptr [ecx]
        mov    al, byte ptr [eax + 0xc]
        mov    byte ptr [0], al              ; -> obj1:0x196C
        mov    cl, byte ptr [esi + 0x12]
        mov    byte ptr [2], cl              ; -> obj1:0x196E
        call   sub_1_2438
        test   eax, eax
        jne    loc_1_574A
        mov    byte ptr [1], 0x8e            ; -> obj1:0x196D
        mov    byte ptr [3], 0x80            ; -> obj1:0x196F
        mov    word ptr [8], 0               ; -> obj1:0x1974
        mov    ax, word ptr [esp + 6]
        mov    word ptr [4], ax              ; -> obj1:0x1970
        call   sub_1_2438
loc_1_574A:
        pop    esi
        add    esp, 4
        ret    
sub_1_574F:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        push   edi
        push   ebp
        xor    edi, edi
        mov    bl, byte ptr [esp + 0x18]
        cmp    byte ptr [esi + 0x30], bl
        je     loc_1_5796
        movzx  eax, bl
        imul   eax, eax, 0x28
        push   eax
        call   sub_1_2540
        add    esp, 4
        mov    ebp, eax
        cmp    ebp, edi
        je     loc_1_5787
        push   dword ptr [esi + 0x18]
        call   sub_1_254F
        add    esp, 4
        mov    dword ptr [esi + 0x18], ebp
        jmp    loc_1_5793
loc_1_5787:
        cmp    byte ptr [esi + 0x30], bl
        jae    loc_1_5793
        mov    bl, 1
        mov    edi, 0x19
loc_1_5793:
        mov    byte ptr [esi + 0x30], bl
loc_1_5796:
        xor    cl, cl
        mov    eax, dword ptr [esi + 0x18]
        cmp    byte ptr [esi + 0x30], cl
        jbe    loc_1_57CE
loc_1_57A0:
        mov    dword ptr [eax], esi
        mov    byte ptr [eax + 0x1f], cl
        add    eax, 0x28
        inc    cl
        mov    dword ptr [eax - 0x1c], 0xffffffff
        mov    byte ptr [eax - 0x11], 0xff
        mov    byte ptr [eax - 0x10], 0xff
        mov    byte ptr [eax - 0xf], 0xff
        mov    word ptr [eax - 0xe], 0xffff
        mov    word ptr [eax - 0xc], 0xffff
        cmp    byte ptr [esi + 0x30], cl
        ja     loc_1_57A0
loc_1_57CE:
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_57D5:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        push   0x34
        call   sub_1_2540
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        jne    loc_1_57F6
        mov    eax, 0x19
        jmp    loc_1_5970
loc_1_57F6:
        xor    ebx, ebx
        mov    esi, dword ptr [esp + 0x1c]
        mov    ax, word ptr [esi + 0x10]
        push   edi
        mov    word ptr [edi], ax
        push   esi
        mov    word ptr [edi + 2], bx
        mov    dword ptr [esp + 0x1c], ebx
        mov    byte ptr [edi + 5], bl
        mov    byte ptr [edi + 4], 6
        call   sub_1_384D
        add    esp, 8
        test   eax, eax
        jne    loc_1_583A
        push   edi
        mov    byte ptr [edi + 5], bl
        mov    word ptr [edi + 0x10], 0x20
        push   esi
        call   sub_1_3967
        add    esp, 8
        test   eax, eax
        jne    loc_1_583A
        mov    bl, byte ptr [edi + 0x14]
loc_1_583A:
        cmp    bl, 1
        jae    loc_1_5843
        mov    bl, 1
        jmp    loc_1_584A
loc_1_5843:
        cmp    bl, 8
        jbe    loc_1_584A
        mov    bl, 8
loc_1_584A:
        push   ebx
        push   esi
        call   sub_1_574F
        add    esp, 8
        test   eax, eax
        je     loc_1_5865
        mov    dword ptr [esp + 0x14], 0x19
        jmp    loc_1_5963
loc_1_5865:
        mov    byte ptr [esp + 0x11], 0
        cmp    byte ptr [esi + 0x30], 0
        mov    ebx, dword ptr [esi + 0x18]
        jbe    loc_1_5963
loc_1_5877:
        mov    word ptr [esp + 0x12], 0
loc_1_587E:
        mov    ax, word ptr [esi + 0x10]
        mov    cl, byte ptr [esp + 0x11]
        movzx  ebp, word ptr [esp + 0x12]
        mov    word ptr [edi], ax
        push   edi
        push   esi
        mov    byte ptr [edi + 1], cl
        mov    word ptr [edi + 2], 0
        mov    byte ptr [edi + 5], 0
        mov    al, byte ptr [ebp]            ; -> loc_1_000C
        mov    byte ptr [edi + 4], al
        call   sub_1_384D
        add    esp, 8
        test   eax, eax
        jne    loc_1_593E
        mov    byte ptr [edi + 5], 0
        mov    word ptr [edi + 0x10], 0x20
        push   edi
        push   esi
        call   sub_1_3967
        add    esp, 8
        test   eax, eax
        jne    loc_1_593E
        test   ebp, ebp
        je     loc_1_58E3
        cmp    ebp, 1
        je     loc_1_58EE
        cmp    ebp, 2
        je     loc_1_58FC
        cmp    ebp, 3
        je     loc_1_590E
        jmp    loc_1_593E
loc_1_58E3:
        mov    al, byte ptr [edi + 0x14]
        shr    al, 4
        mov    byte ptr [ebx + 0x17], al
        jmp    loc_1_593E
loc_1_58EE:
        mov    al, byte ptr [edi + 0x14]
        mov    byte ptr [ebx + 0x18], al
        mov    cl, byte ptr [edi + 0x15]
        mov    byte ptr [ebx + 0x19], cl
        jmp    loc_1_593E
loc_1_58FC:
        mov    ax, word ptr [edi + 0x14]
        mov    word ptr [ebx + 0x1a], ax
        mov    cx, word ptr [edi + 0x16]
        mov    word ptr [ebx + 0x1c], cx
        jmp    loc_1_593E
loc_1_590E:
        mov    dword ptr [ebx + 0xc], 0
        xor    ax, ax
        mov    al, byte ptr [edi + 0x14]
        and    ax, 3
        inc    ax
        je     loc_1_593E
loc_1_5923:
        dec    ax
        movzx  ecx, ax
        movzx  edx, byte ptr [ecx + edi + 0x16]
        lea    ecx, [eax*8]
        shl    edx, cl
        or     dword ptr [ebx + 0xc], edx
        test   ax, ax
        jne    loc_1_5923
loc_1_593E:
        inc    word ptr [esp + 0x12]
        cmp    word ptr [esp + 0x12], 4
        jb     loc_1_587E
        add    ebx, 0x28
        inc    byte ptr [esp + 0x11]
        mov    al, byte ptr [esp + 0x11]
        cmp    byte ptr [esi + 0x30], al
        ja     loc_1_5877
loc_1_5963:
        push   edi
        call   sub_1_254F
        mov    eax, dword ptr [esp + 0x18]
        add    esp, 4
loc_1_5970:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
sub_1_5978:
        push   esi
        push   0x34
        call   sub_1_2540
        add    esp, 4
        mov    esi, eax
        mov    eax, 0x19
        test   esi, esi
        je     loc_1_59B3
        push   esi
        push   dword ptr [esp + 0xc]
        call   sub_1_124A
        add    esp, 8
        push   esi
        push   dword ptr [esp + 0xc]
        call   sub_1_13F4
        add    esp, 8
        push   esi
        call   sub_1_254F
        add    esp, 4
        xor    eax, eax
loc_1_59B3:
        pop    esi
        ret    
sub_1_59B5:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        mov    si, word ptr [eax + 0xa]
        mov    ecx, dword ptr [ebp + 0xc]
        test   ecx, ecx
        je     loc_1_59F0
        test   byte ptr [ecx + 0x17], 0x40
        je     loc_1_59DE
        cmp    dword ptr [ecx + 0x1c], eax
        je     loc_1_59DE
        cmp    word ptr [ebp + 0x10], 0x16
        je     loc_1_59DE
        xor    eax, eax
        jmp    loc_1_5A58
loc_1_59DE:
        push   ecx
        push   eax
        call   sub_1_2635
        add    esp, 8
        test   eax, eax
        je     loc_1_59F0
        or     si, word ptr [eax + 0x10]
loc_1_59F0:
        movzx  eax, word ptr [ebp + 0x10]
        cmp    eax, 0x16
        jg     loc_1_5A08
        je     loc_1_5A4A
        dec    eax
        cmp    eax, 0x10
        ja     loc_1_5A0D
        jmp    dword ptr [eax*4]             ; -> obj1:0x5A5B
loc_1_5A08:
        cmp    eax, 0x40
        je     loc_1_5A2C
loc_1_5A0D:
        mov    eax, 1
        jmp    loc_1_5A58
loc_1_5A14:
        mov    ax, 0x10
        jmp    loc_1_5A4E
loc_1_5A1A:
        mov    ax, 0x20
        jmp    loc_1_5A4E
loc_1_5A20:
        mov    ax, 2
        jmp    loc_1_5A4E
loc_1_5A26:
        mov    ax, 0x40
        jmp    loc_1_5A4E
loc_1_5A2C:
        mov    ax, 0x80
        jmp    loc_1_5A4E
loc_1_5A32:
        mov    ax, 4
        jmp    loc_1_5A4E
loc_1_5A38:
        mov    ax, 8
        jmp    loc_1_5A4E
loc_1_5A3E:
        mov    ax, 0x100
        jmp    loc_1_5A4E
loc_1_5A44:
        mov    ax, 0x200
        jmp    loc_1_5A4E
loc_1_5A4A:
        mov    ax, 0x400
loc_1_5A4E:
        and    ax, si
        cmp    ax, 1
        sbb    eax, eax
        inc    eax
loc_1_5A58:
        pop    esi
        pop    ebp
        ret    
        ; ---- dispatch table (17 entries) ----
        dd  loc_1_5A14
        dd  loc_1_5A1A
        dd  loc_1_5A20
        dd  loc_1_5A26
        dd  loc_1_5A2C
        dd  loc_1_5A20
        dd  loc_1_5A0D
        dd  loc_1_5A32
        dd  loc_1_5A0D
        dd  loc_1_5A38
        dd  loc_1_5A3E
        dd  loc_1_5A3E
        dd  loc_1_5A0D
        dd  loc_1_5A2C
        dd  loc_1_5A44
        dd  loc_1_5A44
        dd  loc_1_5A44
sub_1_5A9F:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [0]            ; -> obj1:0x1918
        test   esi, esi
        je     loc_1_5B98
loc_1_5AB6:
        and    byte ptr [esi + 8], 0xfd
        cmp    word ptr [esi + 0x24], 0
        je     loc_1_5B8E
        mov    edi, dword ptr [esi + 0x10]
        test   edi, edi
        je     loc_1_5B8E
loc_1_5AD0:
        mov    ax, 1
        mov    cl, byte ptr [edi + 0x12]
        shl    ax, cl
        test   word ptr [esi + 0x24], ax
        je     loc_1_5B84
        pushfd 
        pop    dword ptr [ebp - 8]
        cli    
        not    ax
        and    word ptr [esi + 0x24], ax
        push   dword ptr [ebp - 8]
        popfd  
        mov    eax, dword ptr [edi + 0x38]
        xor    ebx, ebx
        test   eax, eax
        je     loc_1_5B09
        push   eax
        call   sub_1_25AF
        add    esp, 4
        mov    dword ptr [edi + 0x38], ebx
loc_1_5B09:
        push   edi
        call   sub_1_27C1
        add    esp, 4
        test   eax, eax
        je     loc_1_5B1E
        xor    ebx, ebx
        mov    word ptr [ebp - 2], bx
        jmp    loc_1_5B32
loc_1_5B1E:
        movzx  bx, byte ptr [9]              ; -> obj1:0x1975
        movzx  ax, byte ptr [3]              ; -> obj1:0x196F
        mov    word ptr [ebp - 2], ax
loc_1_5B32:
        mov    ax, word ptr [ebp - 2]
        and    ax, 0x80
        cmp    ax, 1
        sbb    eax, eax
        inc    eax
        push   eax
        push   edi
        call   sub_1_2981
        mov    ax, word ptr [edi + 0x36]
        add    esp, 8
        xor    ax, word ptr [ebp - 2]
        test   al, 0x80
        je     loc_1_5B5C
        or     bl, 0x80
        jmp    loc_1_5B61
loc_1_5B5C:
        and    bx, 0xff7f
loc_1_5B61:
        mov    ax, word ptr [ebp - 2]
        push   edi
        or     word ptr [edi + 0x34], bx
        mov    word ptr [edi + 0x36], ax
        push   dword ptr [0]                 ; -> loc_1_1BE8
        push   0                             ; -> sub_1_5C2D
        call   sub_1_2566
        add    esp, 0xc
        mov    dword ptr [edi + 0x38], eax
loc_1_5B84:
        mov    edi, dword ptr [edi]
        test   edi, edi
        jne    loc_1_5AD0
loc_1_5B8E:
        mov    esi, dword ptr [esi]
        test   esi, esi
        jne    loc_1_5AB6
loc_1_5B98:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_5B9F:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        xor    ebx, ebx
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        xor    edi, edi
loc_1_5BAC:
        push   edi
        call   sub_1_4FF6
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        je     loc_1_5C06
        cmp    byte ptr [edi + 0x20], bl
        jne    loc_1_5BAC
        test   byte ptr [ebp + 0x10], 2
        je     loc_1_5BD7
        push   dword ptr [ebp + 8]
        push   esi
        push   edi
        call   sub_1_59B5
        add    esp, 0xc
        test   eax, eax
        je     loc_1_5BAC
loc_1_5BD7:
        mov    ecx, 0
        test   esi, esi
        je     loc_1_5BE4
        movzx  ecx, word ptr [esi + 0x10]
loc_1_5BE4:
        push   dword ptr [edi + 4]
        lea    edx, [edi + 0xc]
        push   edx
        push   ebx
        push   ebx
        push   ebx
        push   ecx
        push   dword ptr [ebp + 8]
        call   sub_1_24A2
        add    esp, 0x1c
        test   byte ptr [ebp + 0x10], 1
        je     loc_1_5BAC
        test   eax, eax
        je     loc_1_5BAC
        jmp    loc_1_5C08
loc_1_5C06:
        xor    eax, eax
loc_1_5C08:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
sub_1_5C0D:
        mov    eax, dword ptr [0]            ; -> obj1:0x191C
        test   eax, eax
        je     loc_1_5C23
loc_1_5C16:
        cmp    dword ptr [esp + 4], eax
        je     loc_1_5C29
        mov    eax, dword ptr [eax + 4]
        test   eax, eax
        jne    loc_1_5C16
loc_1_5C23:
        mov    eax, 0x21
loc_1_5C28:
        ret    
loc_1_5C29:
        xor    eax, eax
        jmp    loc_1_5C28
sub_1_5C2D:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 0x14]
        push   edi
        call   sub_1_5C0D
        add    esp, 4
        test   eax, eax
        jne    loc_1_5DB7
        mov    ax, word ptr [edi + 0x32]
        xor    esi, esi
        cmp    ax, 0xff
        mov    dword ptr [edi + 0x38], esi
        jne    loc_1_5C70
        test   byte ptr [edi + 0x34], 0x80
        je     loc_1_5C70
        test   byte ptr [edi + 0x36], 0x80
        jne    loc_1_5C70
        push   edi
        call   sub_3_1879
loc_1_5C67:
        mov    word ptr [edi + 0x32], si
        add    esp, 4
        jmp    loc_1_5C79
loc_1_5C70:
        test   ax, ax
        jne    loc_1_5DB7
loc_1_5C79:
        mov    si, 1
loc_1_5C7D:
        mov    bx, word ptr [edi + 0x36]
        mov    ax, word ptr [edi + 0x34]
        and    ax, si
        movzx  eax, ax
        cmp    eax, 1
        je     loc_1_5CB3
        cmp    eax, 2
        je     loc_1_5CCC
        cmp    eax, 0x10
        je     loc_1_5CE2
        cmp    eax, 0x20
        je     loc_1_5CF3
        cmp    eax, 0x40
        je     loc_1_5D0C
        cmp    eax, 0x80
        je     loc_1_5D22
        xor    bp, bp
        jmp    loc_1_5D94
loc_1_5CB3:
        and    bx, 1
        mov    ebp, 0
        cmp    bx, 1
        adc    ebp, -1
        and    bp, 0x17
        jmp    loc_1_5D94
loc_1_5CCC:
        and    bx, 2
        cmp    bx, 1
        sbb    ebp, ebp
        and    ebp, 3
        add    bp, 3
        jmp    loc_1_5D94
loc_1_5CE2:
        and    bx, 0x10
        cmp    bx, 1
        sbb    ebp, ebp
        inc    bp
        jmp    loc_1_5D94
loc_1_5CF3:
        and    bx, 0x20
        mov    ebp, 0
        cmp    bx, 1
        adc    ebp, -1
        and    bp, 2
        jmp    loc_1_5D94
loc_1_5D0C:
        and    bx, 0x40
        mov    ebp, 0
        cmp    bx, 1
        adc    ebp, -1
        and    bp, 4
        jmp    loc_1_5D94
loc_1_5D22:
        xor    bp, bp
        test   byte ptr [edi + 0x17], 0x80
        je     loc_1_5D4A
        push   2
        push   edi
        push   5
        call   sub_1_5B9F
        and    byte ptr [edi + 0x17], 0x7f
        add    esp, 0xc
        test   bl, 0x80
        jne    loc_1_5D4F
        push   edi
        call   sub_3_1879
loc_1_5D47:
        add    esp, 4
loc_1_5D4A:
        test   bl, 0x80
        je     loc_1_5D72
loc_1_5D4F:
        mov    ecx, edi
        call   sub_1_555D
        mov    word ptr [edi + 0x32], 0x40
        push   edi
        mov    eax, dword ptr [0]            ; -> loc_1_1BE4
        push   eax
        push   0                             ; -> loc_1_5DBC
        call   sub_1_2566
        add    esp, 0xc
        jmp    loc_1_5D94
loc_1_5D72:
        and    byte ptr [edi + 0x14], 0xf7
        push   edi
        call   sub_1_3806
        add    esp, 4
        mov    ecx, edi
        call   sub_1_56A1
        and    byte ptr [edi + 0x14], 0xef
        push   4
        call   sub_3_031E
loc_1_5D91:
        add    esp, 4
loc_1_5D94:
        test   bp, bp
        je     loc_1_5DA5
        push   2
        push   edi
        push   ebp
        call   sub_1_5B9F
        add    esp, 0xc
loc_1_5DA5:
        add    si, si
        test   si, si
        jne    loc_1_5C7D
        mov    word ptr [edi + 0x34], 0
loc_1_5DB7:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_5DBC:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_5C0D
        add    esp, 4
        test   eax, eax
        jne    loc_1_5EC2
        mov    ecx, esi
        call   sub_1_2967
        test   al, 0x80
        je     loc_1_5EA8
        test   al, 0x40
        je     loc_1_5E92
        lea    eax, [esi + 0x24]
        push   eax
        call   sub_1_0889
        add    esp, 4
        lea    eax, [esi + 0x28]
        push   eax
        call   sub_1_0889
        add    esp, 4
        lea    eax, [esi + 0x2c]
        push   eax
        call   sub_1_0889
        add    esp, 4
        mov    ecx, esi
        call   sub_1_5634
        test   byte ptr [esi + 0x17], 0x10
        jne    loc_1_5E80
        mov    eax, dword ptr [esi + 0xc]
        cmp    dword ptr [eax + 0x30], 0
        jne    loc_1_5E31
        push   esi
        call   sub_3_12D7
loc_1_5E2E:
        add    esp, 4
loc_1_5E31:
        mov    word ptr [ebp - 2], 0
        mov    ax, word ptr [esi + 0x10]
        mov    word ptr [ebp - 4], ax
        lea    eax, [ebp - 4]
        push   eax
        push   esi
        call   sub_1_3A1C
        add    esp, 8
        test   eax, eax
        jne    loc_1_5E6C
        cmp    word ptr [ebp - 2], 0
        je     loc_1_5E6C
        push   esi
        call   sub_1_57D5
        add    esp, 4
        test   eax, eax
        jne    loc_1_5E6C
        push   esi
        call   sub_1_5978
        jmp    loc_1_5E7D
loc_1_5E6C:
        push   1
        push   esi
        call   sub_1_574F
        add    esp, 8
        push   esi
        call   sub_3_2C2C
loc_1_5E7D:
        add    esp, 4
loc_1_5E80:
        or     byte ptr [esi + 0x14], 8
        mov    word ptr [esi + 0x32], 0xff
        push   esi
        call   sub_3_1879
loc_1_5E90:
        jmp    loc_1_5EBF
loc_1_5E92:
        push   esi
        mov    eax, dword ptr [0]            ; -> loc_1_1BE4
        push   eax
        push   0                             ; -> loc_1_5DBC
        call   sub_1_2566
        add    esp, 0xc
        jmp    loc_1_5EC2
loc_1_5EA8:
        xor    eax, eax
        mov    word ptr [esi + 0x32], ax
        cmp    word ptr [esi + 0x34], ax
        je     loc_1_5EC2
        cmp    dword ptr [esi + 0x38], eax
        jne    loc_1_5EC2
        push   esi
        call   sub_1_5C2D
loc_1_5EBF:
        add    esp, 4
loc_1_5EC2:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_5EC7:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    word ptr [esi + 0x32], 0xff
        jne    loc_1_5EFF
        push   2
        push   esi
        push   0x40
        call   sub_1_5B9F
        add    esp, 0xc
        xor    eax, eax
        mov    word ptr [esi + 0x32], ax
        or     byte ptr [esi + 0x17], 0x80
        cmp    word ptr [esi + 0x34], ax
        je     loc_1_5EFF
        cmp    dword ptr [esi + 0x38], eax
        jne    loc_1_5EFF
        push   esi
        call   sub_1_5C2D
        add    esp, 4
loc_1_5EFF:
        pop    esi
        ret    
sub_1_5F01:
        push   esi
        mov    esi, dword ptr [esp + 8]
        test   byte ptr [esi + 0x17], 0x80
        je     loc_1_5F1D
        push   2
        push   esi
        push   5
        call   sub_1_5B9F
        and    byte ptr [esi + 0x17], 0x7f
        add    esp, 0xc
loc_1_5F1D:
        pop    esi
        ret    
loc_1_5F1F:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 8
        mov    dword ptr [ebp - 8], 0x32
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [eax]
        mov    edi, dword ptr [eax + 4]
        push   esi
        call   sub_1_5C0D
        add    esp, 4
        test   eax, eax
        jne    loc_1_5F82
        mov    ecx, esi
        call   sub_1_2967
        mov    word ptr [ebp - 2], ax
        mov    eax, dword ptr [ebp + 8]
        movzx  eax, word ptr [eax + 8]
        cmp    eax, 0xe
        je     loc_1_5F94
        cmp    eax, 0x10
        je     loc_1_6008
        cmp    eax, 0x11
        je     loc_1_6088
        cmp    eax, 0x16
        je     loc_1_60EE
        cmp    eax, 0xfe
        je     loc_1_610B
loc_1_5F82:
        push   dword ptr [ebp + 8]
        call   sub_1_254F
        add    esp, 4
loc_1_5F8D:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_5F94:
        xor    ebx, ebx
        cmp    word ptr [esi + 0x32], bx
        jne    loc_1_604C
        push   3
        push   esi
        push   0xe
        call   sub_1_5B9F
        add    esp, 0xc
        mov    dword ptr [ebp - 4], eax
        cmp    eax, ebx
        jne    loc_1_5FE1
        push   2
        push   esi
        push   5
        call   sub_1_5B9F
        add    esp, 0xc
        lea    ecx, [edi + 0xc]
        or     byte ptr [esi + 0x17], 0x40
        mov    dword ptr [esi + 0x1c], edi
        push   dword ptr [edi + 4]
        push   ecx
        mov    dx, word ptr [esi + 0x10]
        push   ebx
        push   ebx
        push   ebx
        push   edx
        push   0x40
        call   sub_1_24A2
        add    esp, 0x1c
loc_1_5FE1:
        and    byte ptr [esi + 0x17], 0xdf
        mov    eax, dword ptr [edi + 4]
        add    edi, 0xc
        push   eax
        push   edi
        mov    ax, word ptr [esi + 0x10]
        push   0
        push   0
        push   dword ptr [ebp - 4]
        push   eax
        push   0xd
        call   sub_1_24A2
        add    esp, 0x1c
        jmp    loc_1_5F82
loc_1_6008:
        cmp    word ptr [esi + 0x32], 0
        jne    loc_1_604C
        push   3
        push   esi
        push   0x10
        call   sub_1_5B9F
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_6064
        push   2
        push   esi
        push   0xf
        call   sub_1_5B9F
        add    esp, 0xc
        mov    ecx, esi
        call   sub_1_555D
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 8], 0x11
        mov    ecx, dword ptr [0]            ; -> loc_1_1BE4
        mov    word ptr [esi + 0x32], 0x11
        mov    dword ptr [ebp - 8], ecx
loc_1_604C:
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp - 8]
        push   0                             ; -> loc_1_5F1F
        call   sub_1_2566
        add    esp, 0xc
        jmp    loc_1_5F8D
loc_1_6064:
        mov    ecx, dword ptr [edi + 4]
        add    edi, 0xc
        push   ecx
        push   edi
        mov    cx, word ptr [esi + 0x10]
        push   0
        push   0
        push   eax
        push   ecx
        push   0x80
        call   sub_1_24A2
        add    esp, 0x1c
        jmp    loc_1_5F82
loc_1_6088:
        cmp    word ptr [esi + 0x32], 0x11
        jne    loc_1_604C
        mov    ebx, 0x80
        test   word ptr [ebp - 2], bx
        je     loc_1_60C7
        test   byte ptr [ebp - 2], 0x40
        je     loc_1_604C
        push   2
        add    edi, 0xc
        push   esi
        push   0x11
        call   sub_1_5B9F
        add    esp, 0xc
        xor    eax, eax
        push   dword ptr [edi - 8]
        push   edi
        mov    cx, word ptr [esi + 0x10]
        push   eax
        push   eax
        push   eax
        push   ecx
        push   ebx
        call   sub_1_24A2
        add    esp, 0x1c
loc_1_60C7:
        xor    eax, eax
        mov    word ptr [esi + 0x32], ax
        cmp    word ptr [esi + 0x34], ax
        je     loc_1_5F82
        cmp    dword ptr [esi + 0x38], eax
        jne    loc_1_5F82
        push   esi
        call   sub_1_5C2D
        add    esp, 4
        jmp    loc_1_5F82
loc_1_60EE:
        cmp    word ptr [esi + 0x32], 0
        jne    loc_1_604C
        push   2
        push   esi
        push   0x16
        call   sub_1_5B9F
        add    esp, 0xc
        jmp    loc_1_5F82
loc_1_610B:
        xor    ebx, ebx
        cmp    word ptr [esi + 0x32], bx
        jne    loc_1_604C
        push   dword ptr [edi + 4]
        mov    cx, word ptr [esi + 0x10]
        add    edi, 0xc
        push   edi
        push   ebx
        push   ebx
        push   ebx
        push   ecx
        push   5
        call   sub_1_24A2
        and    byte ptr [esi + 0x17], 0xbf
        add    esp, 0x1c
        mov    dword ptr [esi + 0x1c], ebx
        test   byte ptr [ebp - 2], 0x80
        je     loc_1_5F82
        push   2
        push   esi
        push   0x40
        call   sub_1_5B9F
        add    esp, 0xc
        jmp    loc_1_5F82
sub_1_6153:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0xc]
        mov    esi, dword ptr [edi + 0x10]
        test   esi, esi
        je     loc_1_616F
loc_1_6160:
        push   esi
        call   sub_1_5F01
        add    esp, 4
        mov    esi, dword ptr [esi]
        test   esi, esi
        jne    loc_1_6160
loc_1_616F:
        mov    eax, dword ptr [0]            ; -> obj1:0x1914
        test   eax, eax
        je     loc_1_6186
loc_1_6178:
        cmp    eax, edi
        je     loc_1_6182
        mov    eax, dword ptr [eax]
        test   eax, eax
        jne    loc_1_6178
loc_1_6182:
        test   eax, eax
        jne    loc_1_6198
loc_1_6186:
        push   edi
        call   sub_1_8CBE
        add    esp, 4
        push   edi
        call   sub_1_8008
        add    esp, 4
loc_1_6198:
        push   0
        push   edi
        push   0                             ; -> obj1:0x1914
        call   sub_1_2E0B
        add    esp, 0xc
        push   edi
        call   sub_1_254F
        add    esp, 4
        pop    edi
        pop    esi
        ret    
sub_1_61B4:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0x14]
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, edi
        and    ebx, 0x80000000
        je     loc_1_61D0
        test   byte ptr [esi + 0xb], 0x10
        jne    loc_1_61DA
loc_1_61D0:
        test   ebx, ebx
        jne    loc_1_61E4
        test   byte ptr [esi + 0xb], 8
        je     loc_1_61E4
loc_1_61DA:
        mov    eax, 0x19
        jmp    loc_1_6283
loc_1_61E4:
        test   edi, 0x20000000
        je     loc_1_6208
        mov    eax, dword ptr [0]            ; -> obj1:0x1920
        test   eax, eax
        je     loc_1_6281
loc_1_61F9:
        and    word ptr [eax + 0xc], 0xfffb
        mov    eax, dword ptr [eax + 4]
        test   eax, eax
        jne    loc_1_61F9
        jmp    loc_1_6281
loc_1_6208:
        mov    eax, dword ptr [ebp + 0x10]
        cmp    eax, 1
        je     loc_1_6221
        cmp    eax, 2
        je     loc_1_623A
        cmp    eax, 4
        je     loc_1_6252
        mov    eax, 0xd
        jmp    loc_1_6283
loc_1_6221:
        push   0x80
        push   edi
        push   dword ptr [ebp + 0xc]
        push   esi
        push   0                             ; -> obj1:0x192C
        call   sub_1_46E3
        add    esp, 0x14
        jmp    loc_1_6264
loc_1_623A:
        push   edi
        push   dword ptr [ebp + 0xc]
        push   esi
        push   0                             ; -> obj1:0x192C
        call   sub_1_8E41
        add    esp, 0x10
        test   eax, eax
        je     loc_1_6268
        jmp    loc_1_6283
loc_1_6252:
        push   edi
        push   dword ptr [ebp + 0xc]
        push   esi
        push   0                             ; -> obj1:0x192C
        call   sub_1_83E3
        add    esp, 0x10
loc_1_6264:
        test   eax, eax
        jne    loc_1_6283
loc_1_6268:
        test   edi, 0x40000000
        jne    loc_1_6281
        mov    eax, 0x8000000
        cmp    ebx, 1
        sbb    ecx, ecx
        and    ecx, eax
        add    ecx, eax
        or     dword ptr [esi + 8], ecx
loc_1_6281:
        xor    eax, eax
loc_1_6283:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
sub_1_6288:
        push   esi
        push   0
        mov    esi, dword ptr [esp + 0x10]
        push   esi
        mov    eax, dword ptr [esi + 0x10]
        add    eax, 0x1c
        push   eax
        call   sub_1_2E0B
        mov    eax, dword ptr [esp + 0x14]
        add    esp, 0xc
        mov    dword ptr [esi + 0x10], eax
        push   0
        push   esi
        add    eax, 0x1c
        push   eax
        call   sub_1_2D57
        add    esp, 0xc
        pop    esi
        ret    
sub_1_62B7:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        push   edi
        push   ebp
        push   esi
        mov    ebx, dword ptr [esi + 0x14]
        call   sub_1_4245
        add    esp, 4
        test   eax, eax
        jne    loc_1_6412
        cmp    dword ptr [esi + 0x10], 0     ; -> obj1:0x192C
        jne    loc_1_63AA
        movzx  eax, byte ptr [esi + 0x1a]
        cmp    eax, 1
        je     loc_1_62F3
        cmp    eax, 2
        je     loc_1_6316
        jmp    loc_1_63AA
loc_1_62F3:
        mov    ecx, dword ptr [ebx + 8]
        mov    eax, ecx
        and    eax, 0x40000000
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 0x20200000
        sub    eax, 0x40400001
        and    eax, ecx
        mov    dword ptr [ebx + 8], eax
        jmp    loc_1_63AA
loc_1_6316:
        mov    ebp, dword ptr [ebx]
        mov    edi, 1
        xor    al, al
        mov    edx, dword ptr [ebp + 0x18]
        mov    cl, byte ptr [ebp + 0x30]
        cmp    cl, al
        je     loc_1_6340
loc_1_6329:
        cmp    edx, ebx
        je     loc_1_6333
        test   byte ptr [edx + 0xa], 0x80
        jne    loc_1_633E
loc_1_6333:
        inc    al
        add    edx, 0x28
        cmp    al, cl
        jb     loc_1_6329
        jmp    loc_1_6340
loc_1_633E:
        xor    edi, edi
loc_1_6340:
        test   edi, edi
        je     loc_1_63A3
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [eax + 0x18]
        mov    dword ptr [0x14], ecx         ; -> sub_1_1980
        mov    edx, dword ptr [ebx]
        mov    eax, dword ptr [edx + 0xc]
        mov    ecx, dword ptr [eax + 0x1c]
        mov    byte ptr [1], 0x8d            ; -> obj1:0x196D
        mov    dword ptr [0xa], ecx          ; -> obj1:0x1976
        mov    edx, dword ptr [ebx]
        mov    eax, dword ptr [edx + 0xc]
        mov    al, byte ptr [eax + 0xc]
        mov    byte ptr [0], al              ; -> obj1:0x196C
        mov    edx, dword ptr [ebx]
        mov    cl, byte ptr [edx + 0x12]
        mov    byte ptr [2], cl              ; -> obj1:0x196E
        call   sub_1_2438
        test   eax, eax
        jne    loc_1_6412
        mov    byte ptr [1], 0x8e            ; -> obj1:0x196D
        and    word ptr [0xe], 0xff7f        ; -> obj1:0x197A
        call   sub_1_2438
        test   eax, eax
        jne    loc_1_6412
loc_1_63A3:
        and    dword ptr [ebx + 8], 0x7f7bffff
loc_1_63AA:
        cmp    byte ptr [esi + 0x1a], 2
        jne    loc_1_63E6
        test   byte ptr [ebx + 0xb], 0x18
        jne    loc_1_63E6
        mov    edx, 0                        ; -> loc_1_1A58
        xor    eax, eax
        mov    edi, edx
        mov    ecx, 9
        rep stosd dword ptr es:[edi], eax
        push   edx
        mov    eax, dword ptr [esi + 8]
        mov    word ptr [2], ax              ; -> loc_1_1A5A
        mov    ecx, dword ptr [esi + 0x14]
        push   dword ptr [ecx]
        call   sub_3_259B
loc_1_63DA:
        add    esp, 8
        test   eax, eax
        mov    eax, 0x19
        jne    loc_1_6412
loc_1_63E6:
        push   4
        add    ebx, 0x24
        push   esi
        push   ebx
        call   sub_1_2E0B
        add    esp, 0xc
        mov    eax, dword ptr [esi + 0x10]
        add    eax, 0x1c
        push   0
        push   esi
        push   eax
        call   sub_1_2E0B
        add    esp, 0xc
        push   esi
        call   sub_1_254F
        add    esp, 4
        xor    eax, eax
loc_1_6412:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_6417:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        push   edi
        test   byte ptr [esi + 0xb], 1
        je     loc_1_642E
        mov    eax, 0x1d
        jmp    loc_1_64FF
loc_1_642E:
        mov    eax, dword ptr [esp + 0x18]
        cmp    eax, 1
        je     loc_1_644B
        cmp    eax, 2
        je     loc_1_648C
        cmp    eax, 4
        je     loc_1_649F
        mov    eax, 0xd
        jmp    loc_1_64FF
loc_1_644B:
        mov    edi, dword ptr [esp + 0x14]
        push   dword ptr [edi + 4]
        call   sub_1_2650
        add    esp, 4
        mov    ebx, eax
        cmp    ebx, -1
        jne    loc_1_646B
        mov    eax, 3
        jmp    loc_1_64FF
loc_1_646B:
        mov    eax, dword ptr [edi + 8]
        sub    eax, dword ptr [edi + 4]
        inc    eax
        push   eax
        push   ebx
        call   sub_1_2678
        add    esp, 8
        cmp    eax, -1
        jne    loc_1_6488
        mov    eax, 0xa
        jmp    loc_1_64FF
loc_1_6488:
        xor    cl, cl
        jmp    loc_1_64AE
loc_1_648C:
        mov    edi, dword ptr [esp + 0x14]
        mov    cl, 1
        movzx  ebx, word ptr [edi + 4]
        movzx  eax, word ptr [edi + 6]
        sub    eax, ebx
        inc    eax
        jmp    loc_1_64AE
loc_1_649F:
        mov    edi, dword ptr [esp + 0x14]
        mov    eax, 1
        movzx  ebx, word ptr [edi + 2]
        mov    cl, 2
loc_1_64AE:
        mov    edx, dword ptr [esi + 0x24]
        test   edx, edx
        je     loc_1_64CF
loc_1_64B5:
        cmp    byte ptr [edx + 0x1a], cl
        jne    loc_1_64C4
        cmp    dword ptr [edx + 8], ebx
        jne    loc_1_64C4
        cmp    dword ptr [edx + 0xc], eax
        je     loc_1_64CB
loc_1_64C4:
        mov    edx, dword ptr [edx + 4]
        test   edx, edx
        jne    loc_1_64B5
loc_1_64CB:
        test   edx, edx
        jne    loc_1_64D6
loc_1_64CF:
        mov    eax, 0x1c
        jmp    loc_1_64FF
loc_1_64D6:
        cmp    dword ptr [edx + 0x10], 0     ; -> obj1:0x192C
        je     loc_1_64E6
        mov    eax, 0x19
        jmp    loc_1_64FF
loc_1_64E6:
        push   edx
        call   sub_1_62B7
        add    esp, 4
        test   eax, eax
        jne    loc_1_64FF
        cmp    dword ptr [esi + 0x24], 0
        jne    loc_1_64FD
        and    byte ptr [esi + 0xb], 0xe7
loc_1_64FD:
        xor    eax, eax
loc_1_64FF:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_6503:
        mov    edx, dword ptr [esp + 4]
        push   ebx
        push   esi
        xor    bl, bl
        lea    ecx, [edx + 0x14]
        mov    esi, dword ptr [edx + 0x18]
        mov    dl, byte ptr [edx + 0x30]
        mov    eax, dword ptr [ecx]
        and    eax, 0xfdffffff
        cmp    dl, bl
        mov    dword ptr [ecx], eax
        je     loc_1_6539
loc_1_6521:
        test   byte ptr [esi + 0xb], 2
        jne    loc_1_6532
        inc    bl
        add    esi, 0x28
        cmp    bl, dl
        jb     loc_1_6521
        jmp    loc_1_6539
loc_1_6532:
        or     eax, 0x2000000
        mov    dword ptr [ecx], eax
loc_1_6539:
        pop    esi
        pop    ebx
        ret    
sub_1_653C:
        mov    ecx, dword ptr [esp + 4]
        push   ebx
        push   esi
        add    ecx, 0xc
        push   edi
        push   ebp
        mov    eax, dword ptr [ecx]
        mov    edx, dword ptr [eax + 0x18]
        mov    dword ptr [0x14], edx         ; -> sub_1_1980
        mov    eax, dword ptr [ecx]
        mov    ebx, dword ptr [eax + 0x1c]
        mov    dword ptr [0xa], ebx          ; -> obj1:0x1976
        mov    eax, dword ptr [ecx]
        mov    al, byte ptr [eax + 0xc]
        mov    byte ptr [1], 0x84            ; -> obj1:0x196D
        mov    dword ptr [0xe], 0            ; -> obj1:0x197A
        mov    byte ptr [0], al              ; -> obj1:0x196C
        call   sub_1_2438
        test   eax, eax
        jne    loc_1_6638
        xor    di, di
        mov    edx, 0x10                     ; -> loc_1_19B4
        xor    bp, bp
        xor    si, si
        cmp    word ptr [0xe], bp            ; -> loc_1_19B2
        jbe    loc_1_660C
        xor    cx, cx
        xor    ax, ax
loc_1_65A2:
        test   bp, 8
        mov    bx, word ptr [edx]
        jne    loc_1_65BE
        cmp    byte ptr [esp + 0x18], bl
        jne    loc_1_65BE
        test   bh, 0x80
        je     loc_1_65BE
        or     di, ax
        or     bp, 8
loc_1_65BE:
        test   bp, 0x10
        jne    loc_1_65D7
        cmp    byte ptr [esp + 0x1c], bl
        jne    loc_1_65D7
        test   bh, 0x40
        je     loc_1_65D7
        or     di, cx
        or     bp, 0x10
loc_1_65D7:
        test   bp, 0x20
        jne    loc_1_65F0
        cmp    byte ptr [esp + 0x20], bl
        jne    loc_1_65F0
        test   bh, 0x20
        je     loc_1_65F0
        or     di, si
        or     bp, 0x20
loc_1_65F0:
        inc    si
        add    edx, 2
        movzx  ecx, cx
        add    cx, 0x10
        movzx  eax, ax
        add    ax, 0x100
        cmp    word ptr [0xe], si            ; -> loc_1_19B2
        ja     loc_1_65A2
loc_1_660C:
        test   bp, 8
        jne    loc_1_661A
        mov    eax, 0xe
        jmp    loc_1_6638
loc_1_661A:
        test   bp, 0x10
        je     loc_1_6633
        test   bp, 0x20
        je     loc_1_6633
        mov    eax, dword ptr [esp + 0x24]
        mov    word ptr [eax], di
        xor    eax, eax
        jmp    loc_1_6638
loc_1_6633:
        mov    eax, 0xf
loc_1_6638:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_663D:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    eax, [ebp - 2]
        push   eax
        mov    esi, dword ptr [ebp + 8]
        xor    eax, eax
        push   eax
        push   eax
        push   eax
        push   dword ptr [esi]
        call   sub_1_653C
        add    esp, 0x14
        test   eax, eax
        je     loc_1_6668
        mov    eax, 0x19
        jmp    loc_1_67D0
loc_1_6668:
        mov    eax, dword ptr [esi]
        mov    ecx, dword ptr [eax + 0xc]
        mov    edx, dword ptr [ecx + 0x18]
        mov    dword ptr [0x14], edx         ; -> sub_1_1980
        mov    eax, dword ptr [esi]
        mov    ecx, dword ptr [eax + 0xc]
        mov    edx, dword ptr [ecx + 0x1c]
        mov    byte ptr [1], 0x8d            ; -> obj1:0x196D
        mov    dword ptr [0xa], edx          ; -> obj1:0x1976
        mov    eax, dword ptr [esi]
        mov    ecx, dword ptr [eax + 0xc]
        mov    dl, byte ptr [ecx + 0xc]
        mov    byte ptr [0], dl              ; -> obj1:0x196C
        mov    eax, dword ptr [esi]
        mov    al, byte ptr [eax + 0x12]
        mov    byte ptr [2], al              ; -> obj1:0x196E
        call   sub_1_2438
        test   eax, eax
        jne    loc_1_67D0
        mov    eax, dword ptr [esi]
        mov    ecx, dword ptr [eax + 0xc]
        cmp    word ptr [ecx + 0x26], 0x500
        jb     loc_1_671C
        mov    ax, word ptr [4]              ; -> obj1:0x1970
        xor    ax, word ptr [ebp - 2]
        test   ah, 0xf
        je     loc_1_671C
        mov    ax, word ptr [ebp - 2]
        xor    ax, word ptr [ebp + 0x18]
        test   ah, 0xf
        je     loc_1_671C
        mov    eax, 0x1000
        test   dword ptr [4], eax            ; -> obj1:0x1970
        je     loc_1_66EC
        test   word ptr [ebp + 0x18], ax
        jne    loc_1_671C
loc_1_66EC:
        mov    ax, word ptr [4]              ; -> obj1:0x1970
        xor    ax, word ptr [ebp + 0x18]
        test   ah, 0xf
        je     loc_1_671C
        mov    byte ptr [1], 0x8e            ; -> obj1:0x196D
        mov    ax, word ptr [ebp - 2]
        or     ah, 0x20
        mov    word ptr [4], ax              ; -> obj1:0x1970
        call   sub_1_2438
        test   eax, eax
        jne    loc_1_67D0
loc_1_671C:
        mov    al, byte ptr [ebp + 0x14]
        mov    byte ptr [3], al              ; -> obj1:0x196F
        and    word ptr [0xe], 0xfcff        ; -> obj1:0x197A
        movzx  eax, word ptr [ebp + 0x10]
        test   eax, eax
        je     loc_1_673C
        cmp    eax, 1
        je     loc_1_6747
        jmp    loc_1_674E
loc_1_673C:
        and    word ptr [0xe], 0xff7f        ; -> obj1:0x197A
        jmp    loc_1_674E
loc_1_6747:
        or     byte ptr [0xe], 0x80          ; -> obj1:0x197A
loc_1_674E:
        mov    ax, word ptr [ebp + 0xc]
        test   ax, ax
        je     loc_1_676B
        and    ax, 0x1f
        and    word ptr [0xe], 0xffe0        ; -> obj1:0x197A
        or     word ptr [0xe], ax            ; -> obj1:0x197A
loc_1_676B:
        mov    eax, dword ptr [esi]
        mov    ecx, 0x100
        test   byte ptr [eax + 0x17], 2
        je     loc_1_6784
        movzx  edx, byte ptr [eax + 0x31]
        movzx  ecx, word ptr [edx*2]         ; -> loc_1_0010
loc_1_6784:
        mov    eax, dword ptr [esi + 8]
        and    eax, 0x40000
        cmp    eax, 1
        sbb    eax, eax
        and    ax, 0x40
        mov    byte ptr [1], 0x8e            ; -> obj1:0x196D
        or     ax, cx
        mov    cx, word ptr [ebp + 0x18]
        mov    dx, word ptr [ebp + 0x1c]
        mov    word ptr [4], cx              ; -> obj1:0x1970
        or     word ptr [0xe], ax            ; -> obj1:0x197A
        mov    word ptr [0x12], dx           ; -> obj1:0x197E
        call   sub_1_2438
        cmp    eax, 1
        mov    ecx, eax
        mov    eax, 0
        adc    eax, -1
        and    eax, ecx
loc_1_67D0:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_67D5:
        push   ebx
        xor    eax, eax
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x18]
        push   ebp
        test   esi, esi
        je     loc_1_67E5
        mov    dword ptr [esi], eax
loc_1_67E5:
        mov    edi, dword ptr [esp + 0x18]
        test   edi, edi
        je     loc_1_6800
        xor    ax, ax
        xor    ecx, ecx
loc_1_67F2:
        movzx  edx, ax
        inc    ax
        mov    dword ptr [edi + edx*4], ecx
        cmp    ax, 2
        jb     loc_1_67F2
loc_1_6800:
        mov    ecx, dword ptr [esp + 0x14]
        lea    edx, [ecx + 8]
        test   byte ptr [edx + 3], 2
        je     loc_1_685E
        xor    bp, bp
        mov    ebx, dword ptr [ecx + 0x24]
        test   ebx, ebx
        je     loc_1_685E
loc_1_6817:
        cmp    dword ptr [ebx + 0x14], ecx
        jne    loc_1_6857
        movzx  eax, byte ptr [ebx + 0x1a]
        cmp    eax, 1
        je     loc_1_682C
        cmp    eax, 2
        je     loc_1_6846
        jmp    loc_1_6857
loc_1_682C:
        test   edi, edi
        je     loc_1_6857
        test   byte ptr [edx + 2], 0x20
        je     loc_1_6865
        cmp    bp, 2
        jae    loc_1_6865
        movzx  eax, bp
        inc    bp
        mov    dword ptr [edi + eax*4], ebx
        jmp    loc_1_6857
loc_1_6846:
        test   esi, esi
        je     loc_1_6857
        test   byte ptr [edx + 2], 0x80
        je     loc_1_686C
        cmp    dword ptr [esi], 0
        jne    loc_1_686C
        mov    dword ptr [esi], ebx
loc_1_6857:
        mov    ebx, dword ptr [ebx + 4]
        test   ebx, ebx
        jne    loc_1_6817
loc_1_685E:
        xor    eax, eax
loc_1_6860:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_6865:
        mov    eax, 0x19
        jmp    loc_1_6860
loc_1_686C:
        mov    eax, 0x19
        jmp    loc_1_6860
sub_1_6873:
        push   ebp
        mov    ecx, dword ptr [esp + 0x10]
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        mov    word ptr [ecx], si
        cmp    dword ptr [ebp + 0xc], esi
        je     loc_1_68C5
        lea    eax, [ebp - 4]
        mov    edi, dword ptr [ebp + 8]
        push   eax
        push   esi
        push   edi
        call   sub_1_67D5
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_68BE
        cmp    dword ptr [ebp - 4], esi
        je     loc_1_68BE
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 8], esi
        je     loc_1_68BE
        or     byte ptr [edi + 0xb], 4
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0x10]
        mov    edx, dword ptr [eax + 8]
        mov    word ptr [ecx], dx
        jmp    loc_1_690A
loc_1_68BE:
        mov    eax, 0x19
        jmp    loc_1_690C
loc_1_68C5:
        mov    edi, dword ptr [ebp + 8]
        xor    bl, bl
        and    byte ptr [edi + 0xb], 0xfb
        mov    edi, dword ptr [edi]
        mov    esi, dword ptr [edi + 0x18]
        cmp    byte ptr [edi + 0x30], bl
        jbe    loc_1_690A
loc_1_68D8:
        test   byte ptr [esi + 0xb], 4
        je     loc_1_6900
        lea    eax, [ebp - 4]
        push   eax
        push   0
        push   esi
        call   sub_1_67D5
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_6900
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_6900
        mov    eax, dword ptr [ebp - 4]
        cmp    dword ptr [eax + 8], 0
        jne    loc_1_6913
loc_1_6900:
        inc    bl
        add    esi, 0x28
        cmp    byte ptr [edi + 0x30], bl
        ja     loc_1_68D8
loc_1_690A:
        xor    eax, eax
loc_1_690C:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_6913:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp + 0x10]
        mov    edx, dword ptr [eax + 8]
        mov    word ptr [ecx], dx
        jmp    loc_1_690A
sub_1_6921:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        xor    di, di
loc_1_692C:
        mov    ecx, dword ptr [ebp + 8]
        movzx  edx, di
        lea    ebx, [ecx + edx*4]
        mov    ecx, dword ptr [ebx]
        test   ecx, ecx
        je     loc_1_698A
        mov    ecx, dword ptr [ecx + 0x14]
        mov    edx, dword ptr [ecx]
        mov    ecx, dword ptr [edx + 0xc]
        mov    esi, dword ptr [ecx + 0x14]
        test   esi, esi
        je     loc_1_6982
loc_1_694A:
        mov    ecx, dword ptr [ebx]
        cmp    dword ptr [esi + 0x1a], ecx
        jne    loc_1_697C
        lea    ecx, [esi + 0x14]
        mov    ax, word ptr [ecx]
        and    ax, 0xfffb
        cmp    dword ptr [ebp + 0xc], 0
        mov    word ptr [ecx], ax
        je     loc_1_6969
        or     al, 4
        mov    word ptr [ecx], ax
loc_1_6969:
        push   esi
        call   sub_1_4053
        add    esp, 4
        test   eax, eax
        je     loc_1_697C
        cmp    dword ptr [ebp + 0x10], 0
        je     loc_1_698A
loc_1_697C:
        mov    esi, dword ptr [esi]
        test   esi, esi
        jne    loc_1_694A
loc_1_6982:
        inc    di
        cmp    di, 2
        jb     loc_1_692C
loc_1_698A:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
sub_1_698F:
        push   ebx
        xor    edx, edx
        mov    ecx, dword ptr [esp + 0xc]
        push   esi
        mov    eax, dword ptr [esp + 0xc]
        push   edi
        mov    dword ptr [ecx], 0xffffffff
        mov    edi, dword ptr [eax + 0x24]
        cmp    edi, edx
        je     loc_1_69CF
loc_1_69A9:
        cmp    byte ptr [edi + 0x1a], 1
        jne    loc_1_69C8
        mov    eax, dword ptr [edi + 8]
        cmp    dword ptr [ecx], eax
        jbe    loc_1_69B8
        mov    dword ptr [ecx], eax
loc_1_69B8:
        mov    eax, dword ptr [edi + 0xc]
        mov    esi, dword ptr [edi + 8]
        lea    eax, [eax + esi - 1]
        cmp    edx, eax
        jae    loc_1_69C8
        mov    edx, eax
loc_1_69C8:
        mov    edi, dword ptr [edi + 4]
        test   edi, edi
        jne    loc_1_69A9
loc_1_69CF:
        mov    eax, dword ptr [ecx]
        cmp    eax, -1
        jne    loc_1_69E2
        xor    edx, edx
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [ecx], edx
        mov    byte ptr [eax], dl
        jmp    loc_1_6A02
loc_1_69E2:
        xor    edx, eax
        mov    ebx, 1
        cmp    edx, ebx
        jbe    loc_1_69F7
loc_1_69ED:
        test   ebx, ebx
        je     loc_1_69F7
        add    ebx, ebx
        cmp    ebx, edx
        jb     loc_1_69ED
loc_1_69F7:
        dec    ebx
        mov    eax, dword ptr [esp + 0x18]
        mov    byte ptr [eax], bl
        not    ebx
        and    dword ptr [ecx], ebx
loc_1_6A02:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_6A06:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        lea    edi, [esi + 8]
        test   byte ptr [edi], 0xf
        jne    loc_1_6A20
        xor    eax, eax
        jmp    loc_1_6B21
loc_1_6A20:
        lea    eax, [ebp - 0x10]
        push   eax
        push   dword ptr [esi]
        call   sub_1_4432
        add    esp, 8
        test   eax, eax
        je     loc_1_6A3C
        mov    eax, 0x19
        jmp    loc_1_6B21
loc_1_6A3C:
        lea    eax, [ebp - 8]
        push   eax
        push   dword ptr [esi + 0xc]
        push   dword ptr [ebp - 0x10]
        call   sub_1_450F
        add    esp, 0xc
        test   eax, eax
        je     loc_1_6A67
        push   dword ptr [ebp - 0x10]
        call   sub_1_44FF
        add    esp, 4
        mov    eax, 0x19
        jmp    loc_1_6B21
loc_1_6A67:
        test   byte ptr [edi], 2
        je     loc_1_6A75
        mov    cl, byte ptr [esi + 0x10]
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 2], cl
loc_1_6A75:
        test   byte ptr [edi], 4
        je     loc_1_6A83
        mov    cl, byte ptr [esi + 0x11]
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 4], cl
loc_1_6A83:
        test   byte ptr [edi], 8
        je     loc_1_6A91
        mov    cl, byte ptr [esi + 0x12]
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 6], cl
loc_1_6A91:
        test   byte ptr [edi], 1
        je     loc_1_6B08
        mov    eax, dword ptr [esi]
        cmp    byte ptr [eax + 0x30], 2
        jb     loc_1_6B00
        mov    al, byte ptr [esi + 0x13]
        and    al, 3
        cmp    al, 3
        jne    loc_1_6B00
        lea    eax, [ebp - 1]
        lea    ecx, [ebp - 0xc]
        push   eax
        push   ecx
        push   esi
        call   sub_1_698F
        add    esp, 0xc
        test   byte ptr [edi], 0x20
        je     loc_1_6AC6
        mov    cl, byte ptr [ebp - 0xc]
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0xa], cl
loc_1_6AC6:
        test   byte ptr [edi], 0x40
        je     loc_1_6AD4
        mov    cl, byte ptr [ebp - 0xb]
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0xc], cl
loc_1_6AD4:
        test   byte ptr [edi], 0x80
        je     loc_1_6AE2
        mov    cl, byte ptr [ebp - 0xa]
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0xe], cl
loc_1_6AE2:
        test   byte ptr [edi + 1], 1
        je     loc_1_6AF1
        mov    cl, byte ptr [ebp - 9]
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0x10], cl
loc_1_6AF1:
        test   byte ptr [edi + 1], 2
        je     loc_1_6B00
        mov    cl, byte ptr [ebp - 1]
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0x12], cl
loc_1_6B00:
        mov    cl, byte ptr [esi + 0x13]
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax], cl
loc_1_6B08:
        push   dword ptr [ebp - 0x10]
        call   sub_1_44FF
        add    esp, 4
        cmp    eax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 0x19
loc_1_6B21:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_6B27:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        lea    eax, [ebp - 2]
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        push   eax
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        mov    esi, dword ptr [edi]
        push   esi
        call   sub_1_653C
        add    esp, 0x14
        test   eax, eax
        jne    loc_1_6D22
        mov    eax, dword ptr [esi + 0xc]
        mov    ecx, dword ptr [eax + 0x18]
        mov    dword ptr [0x14], ecx         ; -> sub_1_1980
        mov    eax, dword ptr [esi + 0xc]
        mov    edx, dword ptr [eax + 0x1c]
        mov    dword ptr [0xa], edx          ; -> obj1:0x1976
        mov    eax, dword ptr [esi + 0xc]
        mov    al, byte ptr [eax + 0xc]
        mov    byte ptr [1], 0x84            ; -> obj1:0x196D
        mov    dword ptr [0xe], 0            ; -> obj1:0x197A
        mov    byte ptr [0], al              ; -> obj1:0x196C
        call   sub_1_2438
        test   eax, eax
        jne    loc_1_6D22
        test   byte ptr [ebp + 0x19], 0x10
        je     loc_1_6BA9
        xor    eax, eax
        lea    ebx, [edi + 0x16]
        mov    byte ptr [edi + 0x14], al
        mov    byte ptr [edi + 0x15], al
        mov    byte ptr [ebx], al
        jmp    loc_1_6BC0
loc_1_6BA9:
        lea    eax, [edi + 0x14]
        mov    cl, byte ptr [ebp + 0xc]
        mov    al, byte ptr [ebp + 0x10]
        lea    ebx, [edi + 0x16]
        mov    byte ptr [edi + 0x14], cl
        mov    byte ptr [edi + 0x15], al
        mov    cl, byte ptr [ebp + 0x14]
        mov    byte ptr [ebx], cl
loc_1_6BC0:
        xor    dl, dl
        mov    eax, dword ptr [esi + 0x18]
        cmp    byte ptr [esi + 0x30], dl
        je     loc_1_6C32
loc_1_6BCA:
        cmp    byte ptr [ebp + 0xc], 0
        je     loc_1_6BF6
        test   byte ptr [ebp + 0x19], 0x10
        jne    loc_1_6BF6
        mov    cl, byte ptr [eax + 0x14]
        test   cl, cl
        je     loc_1_6C12
        cmp    byte ptr [ebp + 0xc], cl
        je     loc_1_6C12
        xor    eax, eax
        mov    byte ptr [edi + 0x14], al
        mov    byte ptr [edi + 0x15], al
        mov    byte ptr [ebx], al
        mov    eax, 0xe
        jmp    loc_1_6D22
loc_1_6BF6:
        mov    cl, byte ptr [eax + 0x14]
        test   cl, cl
        je     loc_1_6C12
        and    word ptr [ebp + 0x18], 0xefff
        mov    byte ptr [ebp + 0xc], cl
        mov    cl, byte ptr [eax + 0x15]
        mov    byte ptr [ebp + 0x10], cl
        mov    cl, byte ptr [eax + 0x16]
        mov    byte ptr [ebp + 0x14], cl
loc_1_6C12:
        mov    cl, byte ptr [eax + 0x15]
        cmp    byte ptr [ebp + 0x10], cl
        jae    loc_1_6C1D
        mov    byte ptr [ebp + 0x10], cl
loc_1_6C1D:
        mov    cl, byte ptr [eax + 0x16]
        cmp    byte ptr [ebp + 0x14], cl
        jae    loc_1_6C28
        mov    byte ptr [ebp + 0x14], cl
loc_1_6C28:
        inc    dl
        add    eax, 0x28
        cmp    dl, byte ptr [esi + 0x30]
        jb     loc_1_6BCA
loc_1_6C32:
        xor    ax, ax
        mov    edi, 0x10                     ; -> loc_1_19B4
        mov    word ptr [ebp - 2], 0
        xor    bx, bx
        cmp    word ptr [0xe], ax            ; -> loc_1_19B2
        jbe    loc_1_6D16
        xor    cx, cx
        mov    word ptr [ebp - 0xc], ax
loc_1_6C57:
        mov    dl, byte ptr [edi]
        test   al, 8
        mov    byte ptr [ebp - 5], dl
        jne    loc_1_6CA7
        cmp    byte ptr [ebp + 0xc], dl
        jne    loc_1_6CA7
        test   byte ptr [edi + 1], 0x80
        je     loc_1_6CA7
        mov    edx, dword ptr [esi + 0xc]
        cmp    word ptr [edx + 0x26], 0x500
        jb     loc_1_6C93
        mov    dx, word ptr [ebp + 0x18]
        and    dx, 0x1000
        cmp    dx, 1
        sbb    edx, edx
        and    edx, 0x1000
        add    edx, 0x1000
        jmp    loc_1_6C95
loc_1_6C93:
        xor    edx, edx
loc_1_6C95:
        mov    word ptr [ebp - 0x10], cx
        or     al, 8
        or     word ptr [ebp - 0x10], dx
        mov    dx, word ptr [ebp - 0x10]
        or     word ptr [ebp - 2], dx
loc_1_6CA7:
        test   al, 0x10
        jne    loc_1_6CCC
        mov    dl, byte ptr [ebp - 5]
        cmp    byte ptr [ebp + 0x10], dl
        jne    loc_1_6CCC
        test   byte ptr [edi + 1], 0x40
        je     loc_1_6CCC
        mov    dx, word ptr [ebp - 2]
        or     al, 0x10
        and    dx, 0xff0f
        or     dx, word ptr [ebp - 0xc]
        mov    word ptr [ebp - 2], dx
loc_1_6CCC:
        test   al, 0x20
        jne    loc_1_6CF0
        mov    dl, byte ptr [ebp - 5]
        cmp    byte ptr [ebp + 0x14], dl
        jne    loc_1_6CF0
        test   byte ptr [edi + 1], 0x20
        je     loc_1_6CF0
        mov    dx, word ptr [ebp - 2]
        or     al, 0x20
        and    dx, 0xfff0
        or     dx, bx
        mov    word ptr [ebp - 2], dx
loc_1_6CF0:
        inc    bx
        add    edi, 2
        movzx  edx, word ptr [ebp - 0xc]
        add    dx, 0x10
        movzx  ecx, cx
        add    cx, 0x100
        mov    word ptr [ebp - 0xc], dx
        cmp    bx, word ptr [0xe]            ; -> loc_1_19B2
        jb     loc_1_6C57
loc_1_6D16:
        mov    cx, word ptr [ebp - 2]
        mov    eax, dword ptr [ebp + 0x1c]
        mov    word ptr [eax], cx
        xor    eax, eax
loc_1_6D22:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_6D29:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x14]
        mov    edi, dword ptr [esp + 0x10]
        mov    word ptr [esi], 0
        mov    al, byte ptr [edi + 7]
        and    al, 0xf
        cmp    al, 8
        je     loc_1_6D48
        xor    eax, eax
        jmp    loc_1_6DD6
loc_1_6D48:
        mov    ecx, dword ptr [esp + 0xc]
        mov    eax, dword ptr [ecx]
        mov    eax, dword ptr [eax + 0xc]
        cmp    word ptr [eax + 0x26], 0x501
        jb     loc_1_6DD1
        mov    eax, dword ptr [eax + 0x18]
        mov    dword ptr [0x14], eax         ; -> sub_1_1980
        mov    edx, dword ptr [ecx]
        mov    eax, dword ptr [edx + 0xc]
        mov    edx, dword ptr [eax + 0x1c]
        mov    byte ptr [1], 0x8c            ; -> obj1:0x196D
        mov    dword ptr [0xa], edx          ; -> obj1:0x1976
        mov    eax, dword ptr [ecx]
        mov    edx, dword ptr [eax + 0xc]
        mov    al, byte ptr [edx + 0xc]
        mov    byte ptr [0], al              ; -> obj1:0x196C
        mov    ecx, dword ptr [ecx]
        mov    dl, byte ptr [ecx + 0x12]
        mov    dword ptr [0xe], 0            ; -> obj1:0x197A
        mov    byte ptr [2], dl              ; -> obj1:0x196E
        call   sub_1_2438
        test   eax, eax
        jne    loc_1_6DD6
        cmp    word ptr [2], 0x12            ; -> loc_1_19D6
        jb     loc_1_6DD1
        xor    dx, dx
        cmp    word ptr [0x10], dx           ; -> loc_1_19E4
        jbe    loc_1_6DD1
        mov    eax, dword ptr [edi + 0x12]
loc_1_6DBA:
        movzx  ecx, dx
        cmp    dword ptr [ecx*4 + 0x12], eax ; -> loc_1_19E6
        je     loc_1_6DD9
        inc    dx
        cmp    dx, word ptr [0x10]           ; -> loc_1_19E4
        jb     loc_1_6DBA
loc_1_6DD1:
        mov    eax, 0xb
loc_1_6DD6:
        pop    edi
        pop    esi
        ret    
loc_1_6DD9:
        mov    word ptr [esi], dx
        xor    eax, eax
        jmp    loc_1_6DD6
sub_1_6DE0:
        xor    dl, dl
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 0x18]
        mov    al, byte ptr [eax + 0x30]
        cmp    al, dl
        je     loc_1_6DFF
loc_1_6DF0:
        cmp    byte ptr [ecx + 0x14], 0
        jne    loc_1_6E02
        inc    dl
        add    ecx, 0x28
        cmp    dl, al
        jb     loc_1_6DF0
loc_1_6DFF:
        xor    eax, eax
loc_1_6E01:
        ret    
loc_1_6E02:
        mov    eax, 1
        jmp    loc_1_6E01
sub_1_6E09:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        push   esi
        push   edi
        lea    edi, [ecx + 0xc]
        mov    eax, dword ptr [edi]
        mov    edx, dword ptr [eax + 0x18]
        mov    dword ptr [0x14], edx         ; -> sub_1_1980
        mov    eax, dword ptr [edi]
        mov    esi, dword ptr [eax + 0x1c]
        mov    byte ptr [1], 0x8d            ; -> obj1:0x196D
        mov    dword ptr [0xa], esi          ; -> obj1:0x1976
        mov    eax, dword ptr [edi]
        mov    al, byte ptr [eax + 0xc]
        mov    byte ptr [0], al              ; -> obj1:0x196C
        mov    cl, byte ptr [ecx + 0x12]
        mov    byte ptr [2], cl              ; -> obj1:0x196E
        call   sub_1_2438
        test   eax, eax
        jne    loc_1_6F22
        mov    si, word ptr [4]              ; -> obj1:0x1970
        mov    eax, dword ptr [edi]
        mov    ecx, dword ptr [eax + 0x18]
        mov    dword ptr [0x14], ecx         ; -> sub_1_1980
        mov    eax, dword ptr [edi]
        mov    edx, dword ptr [eax + 0x1c]
        mov    dword ptr [0xa], edx          ; -> obj1:0x1976
        mov    eax, dword ptr [edi]
        mov    al, byte ptr [eax + 0xc]
        mov    byte ptr [1], 0x84            ; -> obj1:0x196D
        mov    dword ptr [0xe], 0            ; -> obj1:0x197A
        mov    byte ptr [0], al              ; -> obj1:0x196C
        call   sub_1_2438
        test   eax, eax
        jne    loc_1_6F22
        mov    ax, si
        and    ax, 0xf00
        shr    ax, 8
        cmp    word ptr [0xe], ax            ; -> loc_1_19B2
        jbe    loc_1_6F1D
        movzx  eax, ax
        mov    cx, word ptr [eax*2 + 0x10]   ; -> loc_1_19B4
        test   ch, 0x80
        je     loc_1_6F1D
        mov    eax, dword ptr [ebp + 0xc]
        mov    byte ptr [eax], cl
        mov    ax, si
        and    ax, 0xf0
        shr    ax, 4
        cmp    word ptr [0xe], ax            ; -> loc_1_19B2
        jbe    loc_1_6F16
        movzx  eax, ax
        mov    cx, word ptr [eax*2 + 0x10]   ; -> loc_1_19B4
        test   ch, 0x40
        je     loc_1_6F16
        and    si, 0xf
        mov    eax, dword ptr [ebp + 0x10]
        mov    byte ptr [eax], cl
        cmp    word ptr [0xe], si            ; -> loc_1_19B2
        jbe    loc_1_6F0F
        movzx  eax, si
        mov    cx, word ptr [eax*2 + 0x10]   ; -> loc_1_19B4
        test   ch, 0x20
        je     loc_1_6F0F
        mov    eax, dword ptr [ebp + 0x14]
        mov    byte ptr [eax], cl
        xor    eax, eax
        jmp    loc_1_6F22
loc_1_6F0F:
        mov    eax, 0xf
        jmp    loc_1_6F22
loc_1_6F16:
        mov    eax, 0xf
        jmp    loc_1_6F22
loc_1_6F1D:
        mov    eax, 0xe
loc_1_6F22:
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_1_6F26:
        sub    esp, 4
        lea    eax, [esp + 2]
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        push   eax
        push   0x1000
        push   0x32
        push   0x32
        push   0x32
        push   esi
        call   sub_1_6B27
        add    esp, 0x18
        test   eax, eax
        mov    eax, 0x19
        jne    loc_1_6F70
        push   0
        push   dword ptr [esp + 0xa]
        push   dword ptr [esi]
        call   sub_1_954E
        add    esp, 4
        push   eax
        push   2
        push   0
        push   esi
        call   sub_1_663D
        add    esp, 0x18
        xor    eax, eax
loc_1_6F70:
        pop    esi
        add    esp, 4
        ret    
sub_1_6F75:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0xc]
        test   word ptr [edi + 2], 0xfff8
        je     loc_1_6F93
        mov    eax, 2
        jmp    loc_1_70B3
loc_1_6F93:
        mov    cl, byte ptr [edi + 7]
        mov    al, cl
        and    al, 0xf
        test   cl, 0xf8
        jne    loc_1_70AE
        cmp    al, 1
        je     loc_1_6FB3
        cmp    al, 2
        je     loc_1_6FB3
        cmp    al, 8
        jne    loc_1_70AE
loc_1_6FB3:
        lea    eax, [ebp - 8]
        mov    esi, dword ptr [ebp + 8]
        push   eax
        push   edi
        push   esi
        call   sub_1_6D29
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_70B3
        lea    eax, [ebp - 6]
        mov    cl, byte ptr [edi + 6]
        push   eax
        mov    dl, byte ptr [edi + 5]
        push   ecx
        mov    al, byte ptr [edi + 4]
        push   edx
        push   eax
        push   dword ptr [esi]
        call   sub_1_653C
        add    esp, 0x14
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_1_70B3
        push   dword ptr [esi]
        call   sub_1_6DE0
        add    esp, 4
        test   eax, eax
        jne    loc_1_70AA
        mov    al, byte ptr [edi + 4]
        test   al, al
        je     loc_1_70AA
        cmp    al, 0x32
        je     loc_1_70AA
        mov    cx, word ptr [ebp - 6]
        mov    eax, 1
        shr    cx, 8
        and    cl, 0xf
        shl    eax, cl
        mov    ecx, dword ptr [esi]
        mov    dword ptr [ebp - 0xc], eax
        mov    ebx, dword ptr [ecx + 0xc]
        test   dword ptr [ebx + 0x3c], eax
        je     loc_1_703B
        mov    eax, 0x19
        jmp    loc_1_70B3
loc_1_703B:
        lea    eax, [ebp - 2]
        lea    edx, [ebp - 3]
        push   eax
        lea    ebx, [ebp - 1]
        push   edx
        push   ebx
        push   ecx
        call   sub_1_6E09
        add    esp, 0x10
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_1_70B3
        mov    al, byte ptr [ebp - 1]
        cmp    byte ptr [edi + 4], al
        je     loc_1_70AA
        mov    ax, word ptr [ebp - 6]
        push   0
        or     ah, 0x20
        push   eax
        push   dword ptr [esi]
        call   sub_1_954E
        add    esp, 4
        push   eax
        push   2
        push   0
        push   esi
        call   sub_1_663D
        add    esp, 0x18
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_1_70A1
        push   dword ptr [esi]
        call   sub_1_2CC4
        add    esp, 4
        mov    ebx, eax
        test   ebx, ebx
        je     loc_1_70A1
        mov    eax, dword ptr [esi]
        mov    edx, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 0xc]
        or     dword ptr [eax + 0x3c], edx
loc_1_70A1:
        push   esi
        call   sub_1_6F26
        add    esp, 4
loc_1_70AA:
        mov    eax, ebx
        jmp    loc_1_70B3
loc_1_70AE:
        mov    eax, 0xd
loc_1_70B3:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_70BA:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0xc]
        test   word ptr [edi + 2], 0xfff8
        je     loc_1_70D8
        mov    eax, 2
        jmp    loc_1_7288
loc_1_70D8:
        mov    al, byte ptr [edi + 7]
        mov    bl, al
        and    bl, 0xf
        test   al, 0xf8
        jne    loc_1_7283
        test   bl, bl
        je     loc_1_7283
        movzx  eax, bl
        lea    ecx, [eax - 1]
        test   eax, ecx
        jne    loc_1_7283
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0xb], 1
        je     loc_1_7111
        mov    eax, 0x1d
        jmp    loc_1_7288
loc_1_7111:
        lea    eax, [ebp - 4]
        mov    cl, byte ptr [edi + 6]
        push   eax
        mov    dl, byte ptr [edi + 5]
        push   0
        mov    al, byte ptr [edi + 4]
        push   ecx
        push   edx
        push   eax
        push   esi
        call   sub_1_6B27
        add    esp, 0x18
        test   eax, eax
        jne    loc_1_7288
        lea    eax, [ebp - 6]
        push   eax
        push   edi
        push   esi
        call   sub_1_6D29
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_7288
        mov    eax, dword ptr [esi]
        cmp    bl, 2
        mov    byte ptr [eax + 0x31], bl
        je     loc_1_7159
        cmp    bl, 8
        jne    loc_1_7166
loc_1_7159:
        mov    ecx, 0x2000000
        mov    eax, dword ptr [esi]
        or     dword ptr [esi + 8], ecx
        or     dword ptr [eax + 0x14], ecx
loc_1_7166:
        lea    eax, [ebp - 2]
        push   eax
        xor    eax, eax
        mov    ax, word ptr [edi + 2]
        and    eax, 2
        push   eax
        push   esi
        call   sub_1_6873
        add    esp, 0xc
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_1_725C
        push   dword ptr [ebp - 6]
        push   dword ptr [ebp - 4]
        push   dword ptr [esi]
        call   sub_1_954E
        add    esp, 4
        cmp    word ptr [ebp - 2], 1
        push   eax
        sbb    eax, eax
        inc    eax
        push   eax
        push   dword ptr [ebp - 2]
        push   esi
        call   sub_1_663D
        add    esp, 0x18
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_1_725C
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [esi + 0xc], eax
        mov    eax, dword ptr [esi + 8]
        movzx  ecx, byte ptr [edi + 0x10]
        xor    ecx, eax
        and    ecx, 0xf
        xor    ecx, eax
        mov    dword ptr [esi + 8], ecx
        mov    al, byte ptr [edi + 0xc]
        mov    byte ptr [esi + 0x10], al
        mov    dl, byte ptr [edi + 0xd]
        mov    byte ptr [esi + 0x11], dl
        push   esi
        mov    cl, byte ptr [edi + 0xe]
        mov    byte ptr [esi + 0x12], cl
        mov    al, byte ptr [edi + 0xf]
        and    al, 0x3f
        or     al, 0x40
        mov    byte ptr [esi + 0x13], al
        call   sub_1_6A06
        add    esp, 4
        mov    ebx, eax
        test   ebx, ebx
        je     loc_1_726E
        push   dword ptr [esi]
        and    byte ptr [esi + 0xb], 0xfc
        call   sub_1_6503
        add    esp, 4
        lea    eax, [ebp - 4]
        push   eax
        push   0x1000
        push   0x32
        push   0x32
        push   0x32
        push   esi
        call   sub_1_6B27
        add    esp, 0x18
        test   eax, eax
        je     loc_1_7228
        mov    ebx, 0x19
loc_1_7228:
        lea    eax, [ebp - 2]
        push   eax
        push   0
        push   esi
        call   sub_1_6873
        add    esp, 0xc
        push   0
        push   dword ptr [ebp - 4]
        push   dword ptr [esi]
        call   sub_1_954E
        add    esp, 4
        cmp    word ptr [ebp - 2], 1
        push   eax
        sbb    eax, eax
        inc    eax
        push   eax
        push   dword ptr [ebp - 2]
        push   esi
        call   sub_1_663D
        add    esp, 0x18
loc_1_725C:
        push   dword ptr [esi]
        and    byte ptr [esi + 0xb], 0xfc
        call   sub_1_6503
        add    esp, 4
        mov    eax, ebx
        jmp    loc_1_7288
loc_1_726E:
        mov    eax, dword ptr [esi]
        or     byte ptr [esi + 0xb], 1
        and    byte ptr [eax + 0x14], 0xdf
        mov    dword ptr [esi + 0x20], 0     ; -> obj1:0x192C
        xor    eax, eax
        jmp    loc_1_7288
loc_1_7283:
        mov    eax, 0xd
loc_1_7288:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_728F:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [esi]
        call   sub_1_2967
        and    ax, 0x80
        push   0
        mov    bx, ax
        lea    ecx, [ebp - 8]
        push   ecx
        call   sub_3_1180
loc_1_72B4:
        xor    ax, ax
        add    esp, 8
        xor    cl, cl
        test   bx, bx
        je     loc_1_72CD
        test   byte ptr [ebp - 8], 0x40
        je     loc_1_72CD
        mov    ax, 0x1000
        mov    cl, 0x32
loc_1_72CD:
        lea    edx, [ebp - 4]
        push   edx
        push   eax
        push   ecx
        push   ecx
        push   ecx
        push   esi
        call   sub_1_6B27
        add    esp, 0x18
        test   eax, eax
        je     loc_1_72EC
        mov    eax, 0x19
        jmp    loc_1_73D1
loc_1_72EC:
        lea    edi, [esi + 8]
        mov    eax, dword ptr [edi]
        test   eax, 0x2000000
        je     loc_1_737F
        test   bx, bx
        je     loc_1_731E
        test   al, 1
        je     loc_1_731E
        and    al, 0xf1
        push   esi
        mov    byte ptr [esi + 0x13], 0
        mov    dword ptr [edi], eax
        call   sub_1_6A06
        add    esp, 4
        test   eax, eax
        jne    loc_1_73D1
loc_1_731E:
        push   dword ptr [esi]
        and    byte ptr [edi + 3], 0xfd
        call   sub_1_6503
        add    esp, 4
        lea    eax, [ebp - 2]
        push   eax
        push   0
        push   esi
        call   sub_1_6873
        add    esp, 0xc
        mov    eax, 0x80
        test   bx, bx
        je     loc_1_7352
        push   dword ptr [esi]
        call   sub_1_954E
        movzx  eax, al
        add    esp, 4
loc_1_7352:
        cmp    word ptr [ebp - 2], 1
        push   0
        push   dword ptr [ebp - 4]
        push   eax
        sbb    eax, eax
        inc    eax
        push   eax
        push   dword ptr [ebp - 2]
        push   esi
        call   sub_1_663D
        add    esp, 0x18
        test   eax, eax
        je     loc_1_73C4
        mov    edx, 0x2000000
        mov    ecx, dword ptr [esi]
        or     dword ptr [edi], edx
        or     dword ptr [ecx + 0x14], edx
        jmp    loc_1_73D1
loc_1_737F:
        lea    eax, [ebp - 2]
        push   eax
        push   0
        push   esi
        call   sub_1_6873
        add    esp, 0xc
        mov    eax, 0x80
        test   bx, bx
        je     loc_1_73A5
        push   dword ptr [esi]
        call   sub_1_954E
        movzx  eax, al
        add    esp, 4
loc_1_73A5:
        cmp    word ptr [ebp - 2], 1
        push   0
        push   dword ptr [ebp - 4]
        push   eax
        sbb    eax, eax
        inc    eax
        push   eax
        push   dword ptr [ebp - 2]
        push   esi
        call   sub_1_663D
        add    esp, 0x18
        test   eax, eax
        jne    loc_1_73D1
loc_1_73C4:
        and    byte ptr [edi + 3], 0xfe
        mov    dword ptr [esi + 0x20], 0
        xor    eax, eax
loc_1_73D1:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_73D8:
        xor    eax, eax
        ret    
loc_1_73DB:
        push   ebp
        mov    ecx, dword ptr [esp + 0xc]
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ecx + 0x18]
        mov    edi, dword ptr [0xa]          ; -> loc_1_199E
        lea    eax, [edi + 2]
        test   word ptr [eax], 0xfff8
        mov    dword ptr [ebp - 8], eax
        je     loc_1_7408
        mov    eax, 2
        jmp    loc_1_75E8
loc_1_7408:
        mov    eax, dword ptr [0xa]          ; -> loc_1_199E
        mov    al, byte ptr [eax + 7]
        mov    bl, al
        and    bl, 0xf
        test   al, 0xf8
        jne    loc_1_75E3
        cmp    bl, 1
        je     loc_1_742B
        cmp    bl, 2
        jne    loc_1_75E3
loc_1_742B:
        test   byte ptr [esi + 0xb], 0x11
        je     loc_1_743B
        mov    eax, 0x1d
        jmp    loc_1_75E8
loc_1_743B:
        lea    eax, [ebp - 4]
        mov    cl, byte ptr [edi + 6]
        push   eax
        mov    dl, byte ptr [edi + 5]
        push   0
        mov    al, byte ptr [edi + 4]
        push   ecx
        push   edx
        push   eax
        push   esi
        call   sub_1_6B27
        add    esp, 0x18
        test   eax, eax
        jne    loc_1_75E8
        mov    ecx, dword ptr [ebp + 0xc]
        cmp    bl, 2
        mov    byte ptr [ecx + 0x31], bl
        jne    loc_1_7474
        mov    eax, 0x2000000
        or     dword ptr [esi + 8], eax
        or     dword ptr [ecx + 0x14], eax
loc_1_7474:
        push   0
        lea    eax, [ebp - 0x10]
        push   eax
        push   esi
        call   sub_1_67D5
        add    esp, 0xc
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_1_75D0
        lea    eax, [ebp - 2]
        xor    ecx, ecx
        push   eax
        mov    edx, dword ptr [ebp - 8]
        mov    cx, word ptr [edx]
        and    ecx, 2
        push   ecx
        push   esi
        call   sub_1_6873
        add    esp, 0xc
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_1_75D0
        push   0
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp + 0xc]
        call   sub_1_954E
        add    esp, 4
        cmp    word ptr [ebp - 2], 1
        push   eax
        sbb    eax, eax
        inc    eax
        push   eax
        push   dword ptr [ebp - 2]
        push   esi
        call   sub_1_663D
        add    esp, 0x18
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_1_75D0
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [esi + 0xc], eax
        mov    eax, dword ptr [esi + 8]
        movzx  ecx, byte ptr [edi + 0x10]
        xor    ecx, eax
        and    ecx, 0xf
        xor    ecx, eax
        mov    dword ptr [esi + 8], ecx
        mov    al, byte ptr [edi + 0xc]
        mov    byte ptr [esi + 0x10], al
        mov    dl, byte ptr [edi + 0xd]
        mov    byte ptr [esi + 0x11], dl
        push   esi
        mov    cl, byte ptr [edi + 0xe]
        mov    byte ptr [esi + 0x12], cl
        mov    al, byte ptr [edi + 0xf]
        and    al, 0x3f
        or     al, 0x40
        mov    byte ptr [esi + 0x13], al
        call   sub_1_6A06
        add    esp, 4
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_1_754F
        test   byte ptr [esi + 0xb], 2
        je     loc_1_753E
        push   0
        lea    eax, [ebp - 0x10]
        push   1
        push   eax
        call   sub_1_6921
        add    esp, 0xc
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_1_754F
loc_1_753E:
        or     byte ptr [esi + 0xb], 1
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [esi + 0x20], eax
        xor    eax, eax
        jmp    loc_1_75E8
loc_1_754F:
        test   byte ptr [esi + 0xb], 2
        je     loc_1_756B
        push   1
        lea    eax, [ebp - 0x10]
        push   0
        push   eax
        call   sub_1_6921
        add    esp, 0xc
        test   eax, eax
        je     loc_1_756B
        mov    ebx, eax
loc_1_756B:
        push   dword ptr [ebp + 0xc]
        and    byte ptr [esi + 0xb], 0xfc
        call   sub_1_6503
        add    esp, 4
        lea    eax, [ebp - 4]
        push   eax
        push   0x1000
        push   0x32
        push   0x32
        push   0x32
        push   esi
        call   sub_1_6B27
        add    esp, 0x18
        test   eax, eax
        je     loc_1_759B
        mov    ebx, 0x19
loc_1_759B:
        lea    eax, [ebp - 2]
        push   eax
        push   0
        push   esi
        call   sub_1_6873
        add    esp, 0xc
        push   0
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp + 0xc]
        call   sub_1_954E
        add    esp, 4
        cmp    word ptr [ebp - 2], 1
        push   eax
        sbb    eax, eax
        inc    eax
        push   eax
        push   dword ptr [ebp - 2]
        push   esi
        call   sub_1_663D
        add    esp, 0x18
loc_1_75D0:
        push   dword ptr [ebp + 0xc]
        and    byte ptr [esi + 0xb], 0xfc
        call   sub_1_6503
        add    esp, 4
        mov    eax, ebx
        jmp    loc_1_75E8
loc_1_75E3:
        mov    eax, 0xd
loc_1_75E8:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_75EF:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0xc]
        mov    esi, dword ptr [edi + 0x18]
        mov    ecx, dword ptr [esi + 8]
        test   ecx, 0x1000000
        je     loc_1_7784
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 0x20], eax
        jne    loc_1_7784
        test   ecx, 0x10000000
        jne    loc_1_7784
        push   0
        lea    eax, [ebp - 0x10]
        push   eax
        push   esi
        call   sub_1_67D5
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_7789
        mov    ecx, edi
        call   sub_1_2967
        and    ax, 0x80
        lea    ecx, [ebp - 4]
        mov    bx, ax
        push   ecx
        push   0x1000
        push   0x32
        push   0x32
        push   0x32
        push   esi
        call   sub_1_6B27
        add    esp, 0x18
        test   eax, eax
        je     loc_1_7670
        mov    eax, 0x19
        jmp    loc_1_7789
loc_1_7670:
        test   byte ptr [esi + 0xb], 2
        je     loc_1_7731
        push   0
        lea    eax, [ebp - 0x10]
        push   0
        push   eax
        call   sub_1_6921
        add    esp, 0xc
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        jne    loc_1_770A
        test   bx, bx
        je     loc_1_76B6
        mov    eax, dword ptr [esi + 8]
        test   al, 1
        je     loc_1_76B6
        and    al, 0xf1
        push   esi
        mov    dword ptr [esi + 8], eax
        mov    byte ptr [esi + 0x13], 0
        call   sub_1_6A06
        add    esp, 4
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        jne    loc_1_770A
loc_1_76B6:
        and    byte ptr [esi + 0xb], 0xfd
        push   edi
        call   sub_1_6503
        add    esp, 4
        lea    eax, [ebp - 2]
        push   eax
        push   0
        push   esi
        call   sub_1_6873
        add    esp, 0xc
        mov    eax, 0x80
        test   bx, bx
        je     loc_1_76E8
        push   edi
        call   sub_1_954E
        movzx  eax, al
        add    esp, 4
loc_1_76E8:
        cmp    word ptr [ebp - 2], 1
        push   0
        push   dword ptr [ebp - 4]
        push   eax
        sbb    eax, eax
        inc    eax
        push   eax
        push   dword ptr [ebp - 2]
        push   esi
        call   sub_1_663D
        add    esp, 0x18
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        je     loc_1_7775
loc_1_770A:
        mov    eax, 0x2000000
        push   1
        push   1
        lea    ecx, [ebp - 0x10]
        push   ecx
        or     dword ptr [esi + 8], eax
        or     dword ptr [edi + 0x14], eax
        call   sub_1_6921
        add    esp, 0xc
        test   eax, eax
        je     loc_1_772C
        mov    dword ptr [ebp - 8], eax
loc_1_772C:
        mov    eax, dword ptr [ebp - 8]
        jmp    loc_1_7789
loc_1_7731:
        lea    eax, [ebp - 2]
        push   eax
        push   0
        push   esi
        call   sub_1_6873
        add    esp, 0xc
        mov    eax, 0x80
        test   bx, bx
        je     loc_1_7756
        push   edi
        call   sub_1_954E
        movzx  eax, al
        add    esp, 4
loc_1_7756:
        cmp    word ptr [ebp - 2], 1
        push   0
        push   dword ptr [ebp - 4]
        push   eax
        sbb    eax, eax
        inc    eax
        push   eax
        push   dword ptr [ebp - 2]
        push   esi
        call   sub_1_663D
        add    esp, 0x18
        test   eax, eax
        jne    loc_1_7789
loc_1_7775:
        and    byte ptr [esi + 0xb], 0xfe
        mov    dword ptr [esi + 0x20], 0
        xor    eax, eax
        jmp    loc_1_7789
loc_1_7784:
        mov    eax, 0x21
loc_1_7789:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_7790:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 0x1c]
        mov    ecx, dword ptr [edi + 0x18]
        mov    edx, dword ptr [ecx + 8]
        test   edx, 0x1000000
        je     loc_1_78B0
        mov    eax, dword ptr [esp + 0x18]
        cmp    dword ptr [ecx + 0x20], eax
        jne    loc_1_78B0
        mov    esi, dword ptr [0xa]          ; -> loc_1_199E
        add    esi, 2
        mov    bx, word ptr [esi]
        test   bx, 0xffc1
        jne    loc_1_78A9
        not    edx
        test   edx, 0x2800000
        je     loc_1_77E4
        test   bl, 6
        jne    loc_1_78A9
loc_1_77E4:
        test   bl, 0x20
        je     loc_1_77F4
        mov    eax, dword ptr [0xa]          ; -> loc_1_199E
        movzx  edx, byte ptr [eax + 6]
        jmp    loc_1_77F8
loc_1_77F4:
        movzx  edx, byte ptr [ecx + 0x16]
loc_1_77F8:
        test   bl, 0x10
        je     loc_1_7808
        mov    eax, dword ptr [0xa]          ; -> loc_1_199E
        movzx  ebp, byte ptr [eax + 5]
        jmp    loc_1_780C
loc_1_7808:
        movzx  ebp, byte ptr [ecx + 0x15]
loc_1_780C:
        test   bl, 8
        je     loc_1_781D
        mov    ebx, dword ptr [0xa]          ; -> loc_1_199E
        movzx  eax, byte ptr [ebx + 4]
        jmp    loc_1_7821
loc_1_781D:
        movzx  eax, byte ptr [ecx + 0x14]
loc_1_7821:
        lea    ebx, [esp + 0x12]
        push   ebx
        push   0
        push   edx
        push   ebp
        push   eax
        push   ecx
        call   sub_1_6B27
        add    esp, 0x18
        test   eax, eax
        jne    loc_1_78B5
        lea    ecx, [edi + 0xc]
        mov    byte ptr [1], 0x8d            ; -> obj1:0x196D
        mov    eax, dword ptr [ecx]
        mov    al, byte ptr [eax + 0xc]
        mov    byte ptr [0], al              ; -> obj1:0x196C
        mov    dl, byte ptr [edi + 0x12]
        mov    byte ptr [2], dl              ; -> obj1:0x196E
        mov    ebx, dword ptr [ecx]
        mov    eax, dword ptr [ebx + 0x18]
        mov    dword ptr [0x14], eax         ; -> sub_1_1980
        mov    ebx, dword ptr [ecx]
        mov    ecx, dword ptr [ebx + 0x1c]
        mov    dword ptr [0xa], ecx          ; -> obj1:0x1976
        call   sub_1_2438
        test   eax, eax
        jne    loc_1_78B5
        mov    ax, word ptr [esi]
        test   al, 4
        je     loc_1_7890
        test   al, 2
        je     loc_1_7887
        or     byte ptr [0xe], 0x80          ; -> obj1:0x197A
        jmp    loc_1_7890
loc_1_7887:
        and    word ptr [0xe], 0xff7f        ; -> obj1:0x197A
loc_1_7890:
        mov    byte ptr [1], 0x8e            ; -> obj1:0x196D
        mov    ax, word ptr [esp + 0x12]
        mov    word ptr [4], ax              ; -> obj1:0x1970
        call   sub_1_2438
        jmp    loc_1_78B5
loc_1_78A9:
        mov    eax, 2
        jmp    loc_1_78B5
loc_1_78B0:
        mov    eax, 0x21
loc_1_78B5:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_1_78BD:
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [0xa]          ; -> loc_1_199E
        push   ebp
        mov    edi, esi
        mov    ebp, dword ptr [esp + 0x24]
        xor    eax, eax
        mov    ecx, 9
        mov    ebx, dword ptr [ebp + 0x18]
        rep stosd dword ptr es:[edi], eax
        stosb  byte ptr es:[edi], al
        mov    ax, word ptr [ebp + 0x10]
        mov    word ptr [esi], ax
        test   byte ptr [ebx + 0xb], 1
        je     loc_1_78FB
        or     byte ptr [esi + 3], 1
        mov    eax, dword ptr [ebx + 0x20]
        mov    ax, word ptr [eax + 0x16]
        mov    word ptr [2], ax              ; -> loc_1_1996
loc_1_78FB:
        cmp    dword ptr [ebx + 0xc], -1
        jne    loc_1_790B
        mov    eax, 0x19
        jmp    loc_1_7AC2
loc_1_790B:
        lea    ecx, [ebp + 0xc]
        mov    eax, dword ptr [ecx]
        mov    edx, dword ptr [eax + 0x18]
        mov    dword ptr [0x14], edx         ; -> sub_1_1980
        mov    eax, dword ptr [ecx]
        mov    edi, dword ptr [eax + 0x1c]
        mov    byte ptr [1], 0x84            ; -> obj1:0x196D
        mov    dword ptr [0xa], edi          ; -> obj1:0x1976
        mov    eax, dword ptr [ecx]
        mov    al, byte ptr [eax + 0xc]
        mov    dword ptr [0xe], 0            ; -> obj1:0x197A
        mov    byte ptr [0], al              ; -> obj1:0x196C
        call   sub_1_2438
        test   eax, eax
        jne    loc_1_7AC2
        mov    byte ptr [1], 0x8d            ; -> obj1:0x196D
        mov    al, byte ptr [ebp + 0x12]
        mov    byte ptr [2], al              ; -> obj1:0x196E
        call   sub_1_2438
        test   eax, eax
        jne    loc_1_7AC2
        mov    ax, word ptr [0xe]            ; -> obj1:0x197A
        and    ah, 3
        cmp    ah, 1
        jne    loc_1_797C
        or     byte ptr [esi + 7], 1
        jmp    loc_1_7980
loc_1_797C:
        or     byte ptr [esi + 7], 2
loc_1_7980:
        test   byte ptr [ebx + 0xa], 0x80
        lea    edi, [esi + 0x19]
        je     loc_1_7995
        mov    ax, word ptr [0xe]            ; -> obj1:0x197A
        and    al, 0x1f
        mov    byte ptr [edi], al
        jmp    loc_1_7998
loc_1_7995:
        mov    byte ptr [edi], 0xff
loc_1_7998:
        test   byte ptr [0xe], 0x80          ; -> obj1:0x197A
        je     loc_1_79A5
        or     byte ptr [esi + 2], 2
loc_1_79A5:
        test   byte ptr [ebp + 0x17], 0x40
        je     loc_1_79AF
        or     byte ptr [esi + 2], 1
loc_1_79AF:
        mov    eax, dword ptr [4]            ; -> obj1:0x1970
        mov    edx, dword ptr [4]            ; -> obj1:0x1970
        shr    eax, 4
        and    dl, 0xf
        and    al, 0xf
        mov    ecx, dword ptr [4]            ; -> obj1:0x1970
        shr    ecx, 8
        and    cl, 0xf
        movzx  eax, al
        movzx  ecx, cl
        mov    cl, byte ptr [ecx*2 + 0x10]   ; -> loc_1_19B4
        mov    byte ptr [esi + 4], cl
        movzx  ecx, dl
        mov    al, byte ptr [eax*2 + 0x10]   ; -> loc_1_19B4
        mov    byte ptr [esi + 5], al
        lea    eax, [esp + 0x10]
        mov    dl, byte ptr [ecx*2 + 0x10]   ; -> loc_1_19B4
        push   eax
        lea    ecx, [esp + 0x18]
        mov    byte ptr [esi + 6], dl
        push   ecx
        push   ebx
        call   sub_1_67D5
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_7AC2
        cmp    dword ptr [esp + 0x10], 0
        je     loc_1_7A38
        mov    eax, dword ptr [esp + 0x10]
        mov    cl, byte ptr [edi]
        cmp    byte ptr [eax + 8], cl
        je     loc_1_7A2C
        mov    eax, 0x19
        jmp    loc_1_7AC2
loc_1_7A2C:
        mov    eax, dword ptr [esp + 0x10]
        mov    ax, word ptr [eax + 0x18]
        mov    word ptr [esi + 0x1a], ax
loc_1_7A38:
        xor    cx, cx
loc_1_7A3B:
        movzx  eax, cx
        shl    eax, 2
        lea    edx, [esp + eax + 0x14]
        mov    ebp, dword ptr [edx]
        test   ebp, ebp
        je     loc_1_7A75
        inc    cx
        lea    edi, [esi + eax]
        cmp    cx, 2
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [edi + 0x1c], ax
        mov    ebp, dword ptr [edx]
        mov    al, byte ptr [ebp + 0xc]
        mov    byte ptr [edi + 0x1e], al
        mov    ebp, dword ptr [edx]
        mov    al, byte ptr [ebp + 0x19]
        mov    byte ptr [edi + 0x1f], al
        mov    ebp, dword ptr [edx]
        mov    dl, byte ptr [ebp + 0x18]
        mov    byte ptr [esi + 0x24], dl
        jb     loc_1_7A3B
loc_1_7A75:
        mov    eax, dword ptr [ebx + 0xc]
        mov    dword ptr [esi + 8], eax
        mov    cl, byte ptr [ebx + 0x13]
        mov    byte ptr [esi + 0xf], cl
        mov    dl, byte ptr [ebx + 0x10]
        mov    byte ptr [esi + 0xc], dl
        mov    al, byte ptr [ebx + 0x11]
        mov    byte ptr [esi + 0xd], al
        mov    cl, byte ptr [ebx + 0x12]
        mov    byte ptr [esi + 0xe], cl
        mov    eax, dword ptr [ebx + 8]
        and    al, 0xf
        mov    byte ptr [esi + 0x18], al
        mov    byte ptr [esi + 0x10], al
        mov    cl, byte ptr [ebx + 0x17]
        mov    byte ptr [esi + 0x11], cl
        mov    dl, byte ptr [ebx + 0x18]
        mov    byte ptr [esi + 0x12], dl
        mov    al, byte ptr [ebx + 0x19]
        mov    byte ptr [esi + 0x13], al
        xor    eax, eax
        mov    cx, word ptr [ebx + 0x1a]
        mov    word ptr [esi + 0x14], cx
        mov    dx, word ptr [ebx + 0x1c]
        mov    word ptr [esi + 0x16], dx
loc_1_7AC2:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
loc_1_7ACA:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        push   edi
        mov    esi, dword ptr [0xa]          ; -> loc_1_199E
        mov    edi, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [edi + 0x18]
        cmp    dword ptr [eax + 0xc], -1
        jne    loc_1_7AEB
        mov    eax, 0x19
        jmp    loc_1_7B64
loc_1_7AEB:
        lea    eax, [ebp - 4]
        push   eax
        push   edi
        call   sub_1_4432
        add    esp, 8
        test   eax, eax
        je     loc_1_7B03
        mov    eax, 0x19
        jmp    loc_1_7B64
loc_1_7B03:
        lea    eax, [ebp - 8]
        mov    edx, dword ptr [edi + 0x18]
        movzx  ecx, byte ptr [esi + 3]
        push   eax
        add    ecx, dword ptr [edx + 0xc]
        push   ecx
        push   dword ptr [ebp - 4]
        call   sub_1_450F
        add    esp, 0xc
        test   eax, eax
        je     loc_1_7B33
        push   dword ptr [ebp - 4]
        call   sub_1_44FF
        add    esp, 4
        mov    eax, 0x19
        jmp    loc_1_7B64
loc_1_7B33:
        cmp    byte ptr [esi + 2], 0
        jne    loc_1_7B43
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax]
        mov    byte ptr [esi + 4], al
        jmp    loc_1_7B4B
loc_1_7B43:
        mov    cl, byte ptr [esi + 4]
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax], cl
loc_1_7B4B:
        push   dword ptr [ebp - 4]
        call   sub_1_44FF
        add    esp, 4
        cmp    eax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 0x19
loc_1_7B64:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_7B6A:
        push   esi
        push   0x40
        call   sub_1_2540
        add    esp, 4
        mov    esi, eax
        test   esi, esi
        je     loc_1_7B92
        mov    eax, dword ptr [esp + 8]
        push   0
        push   esi
        mov    dword ptr [esi + 4], eax
        push   0                             ; -> obj1:0x1914
        call   sub_1_2D7C
        add    esp, 0xc
loc_1_7B92:
        mov    eax, esi
        pop    esi
        ret    
        db  056h, 057h, 033h, 0FFh, 08Bh, 074h, 024h, 00Ch, 039h, 07Eh, 010h, 075h, 005h, 039h, 07Eh, 014h
        db  074h, 00Fh, 056h, 0E8h, 05Ah, 004h, 000h, 000h, 083h, 0C4h, 004h, 085h, 0C0h, 074h, 002h, 08Bh
        db  0F8h, 06Ah, 000h, 056h, 068h
        dd  obj1:0x1914              ; fixup
        db  0E8h, 047h, 0B2h, 0FFh, 0FFh, 083h, 0C4h, 00Ch, 056h, 0E8h, 082h, 0A9h, 0FFh, 0FFh, 083h, 0C4h
        db  004h, 085h, 0C0h, 074h, 002h, 08Bh, 0F8h, 08Bh, 0C7h, 05Fh, 05Eh, 0C3h
sub_1_7BDB:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x1c
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0xc]
        mov    dword ptr [0xa], ebx          ; -> obj1:0x1976
        mov    byte ptr [1], 0x84            ; -> obj1:0x196D
        mov    byte ptr [0], 0               ; -> obj1:0x196C
        mov    dword ptr [0xe], 0            ; -> obj1:0x197A
        mov    dword ptr [0x14], edi         ; -> sub_1_1980
        call   sub_1_2438
        test   eax, eax
        jne    loc_1_7FCB
        mov    esi, dword ptr [0]            ; -> obj1:0x1914
        test   esi, esi
        je     loc_1_7C36
        mov    eax, dword ptr [ebx]
loc_1_7C27:
        cmp    dword ptr [esi + 4], eax
        je     loc_1_7C32
        mov    esi, dword ptr [esi]
        test   esi, esi
        jne    loc_1_7C27
loc_1_7C32:
        test   esi, esi
        jne    loc_1_7C40
loc_1_7C36:
        mov    eax, 0x19
        jmp    loc_1_7FCB
loc_1_7C40:
        mov    al, byte ptr [2]              ; -> obj1:0x196E
        mov    byte ptr [esi + 0xd], al
        mov    cl, byte ptr [3]              ; -> obj1:0x196F
        mov    byte ptr [esi + 0xe], cl
        mov    dword ptr [esi + 0x18], edi
        mov    dword ptr [esi + 0x1c], ebx
        test   byte ptr [4], 8               ; -> loc_1_19A8
        je     loc_1_7C64
        or     byte ptr [esi + 8], 0x20
loc_1_7C64:
        xor    edi, edi
        mov    ecx, dword ptr [0]            ; -> obj1:0x1918
        cmp    ecx, edi
        je     loc_1_7D04
loc_1_7C74:
        mov    edx, dword ptr [ecx + 0x10]
        mov    ebx, dword ptr [ecx + 0x14]
        mov    dword ptr [ebp - 0x10], ebx
        test   edi, edi
        jne    loc_1_7C9F
        cmp    byte ptr [ecx + 0xf], 0
        je     loc_1_7CF8
        movzx  ax, byte ptr [esi + 0xd]
        cmp    word ptr [edx + 0x10], ax
        jb     loc_1_7CF8
        movzx  ax, byte ptr [esi + 0xe]
        cmp    ax, word ptr [ebx + 0xe]
        jae    loc_1_7CF8
        jmp    loc_1_7D04
loc_1_7C9F:
        mov    ebx, dword ptr [edi + 0x10]
        movzx  ax, byte ptr [edi + 0xd]
        add    ax, word ptr [ebx + 0x10]
        mov    ebx, dword ptr [edi + 0x14]
        mov    word ptr [ebp - 2], ax
        mov    ax, word ptr [ebx + 0xe]
        movzx  bx, byte ptr [edi + 0xe]
        add    ax, bx
        mov    word ptr [ebp - 4], ax
        movzx  ebx, byte ptr [ecx + 0xf]
        movzx  eax, byte ptr [edi + 0xf]
        sub    eax, ebx
        cmp    eax, -1
        je     loc_1_7CF8
        movzx  edx, word ptr [edx + 0x10]
        movzx  eax, word ptr [ebp - 2]
        movzx  ebx, byte ptr [esi + 0xd]
        sub    edx, eax
        cmp    edx, ebx
        jl     loc_1_7CF8
        mov    ebx, dword ptr [ebp - 0x10]
        movzx  eax, word ptr [ebp - 4]
        movzx  edx, word ptr [ebx + 0xe]
        movzx  ebx, byte ptr [esi + 0xe]
        sub    edx, eax
        cmp    edx, ebx
        jge    loc_1_7D04
loc_1_7CF8:
        mov    edi, ecx
        mov    ecx, dword ptr [ecx]
        test   ecx, ecx
        jne    loc_1_7C74
loc_1_7D04:
        xor    ebx, ebx
        test   edi, edi
        jne    loc_1_7D1C
        mov    word ptr [ebp - 2], bx
        mov    byte ptr [esi + 0xf], bl
        mov    word ptr [ebp - 4], 1
        mov    dword ptr [ebp - 0xc], ebx
        jmp    loc_1_7D62
loc_1_7D1C:
        mov    al, byte ptr [edi + 0xf]
        inc    al
        mov    byte ptr [esi + 0xf], al
        mov    ecx, dword ptr [edi + 0x10]
        mov    dword ptr [ebp - 0xc], ecx
        cmp    dword ptr [ecx], 0
        je     loc_1_7D3C
loc_1_7D2F:
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [ebp - 0xc], ecx
        cmp    dword ptr [ecx], 0
        jne    loc_1_7D2F
loc_1_7D3C:
        mov    ebx, dword ptr [edi + 0x14]
        cmp    dword ptr [ebx], 0
        je     loc_1_7D4B
loc_1_7D44:
        mov    ebx, dword ptr [ebx]
        cmp    dword ptr [ebx], 0
        jne    loc_1_7D44
loc_1_7D4B:
        mov    eax, dword ptr [ebp - 0xc]
        mov    ax, word ptr [eax + 0x10]
        inc    ax
        mov    word ptr [ebp - 2], ax
        mov    ax, word ptr [ebx + 0xe]
        inc    ax
        mov    word ptr [ebp - 4], ax
loc_1_7D62:
        push   0
        push   esi
        push   0                             ; -> obj1:0x1914
        call   sub_1_2E0B
        add    esp, 0xc
        push   0
        push   esi
        push   edi
        push   0                             ; -> obj1:0x1918
        call   sub_1_2DB5
        mov    byte ptr [ebp - 0xd], 0
        mov    dword ptr [ebp - 0x18], 0
        mov    byte ptr [ebp - 0x11], 0
        add    esp, 0x10
        cmp    byte ptr [esi + 0xe], 0
        jbe    loc_1_7EAC
loc_1_7D9C:
        push   0x26
        call   sub_1_2540
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        je     loc_1_7FE8
        mov    cx, word ptr [ebp - 4]
        mov    al, byte ptr [ebp - 0x11]
        inc    word ptr [ebp - 4]
        push   0
        push   edi
        mov    dword ptr [edi + 8], esi
        mov    byte ptr [edi + 0x10], al
        lea    eax, [esi + 0x14]
        mov    word ptr [edi + 0xe], cx
        push   eax
        call   sub_1_2D7C
        add    esp, 0xc
        push   4
        push   edi
        push   ebx
        push   0                             ; -> obj1:0x1920
        mov    ebx, edi
        call   sub_1_2DB5
        add    esp, 0x10
        lea    eax, [ebp - 0x1c]
        push   eax
        push   3
        push   edi
        call   sub_1_2AA9
        add    esp, 0xc
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        jne    loc_1_7FEF
        movzx  ecx, word ptr [4]             ; -> obj1:0x1970
        mov    al, byte ptr [2]              ; -> obj1:0x196E
        mov    dl, al
        mov    dword ptr [edi + 0x16], ecx
        xor    ecx, ecx
        and    dl, 4
        mov    cl, al
        and    ecx, 1
        cmp    dl, 1
        sbb    edx, edx
        and    al, 2
        inc    edx
        cmp    al, 1
        sbb    eax, eax
        inc    eax
        cmp    eax, ecx
        jne    loc_1_7EF0
        cmp    edx, ecx
        je     loc_1_7EF0
        mov    byte ptr [edi + 0x11], cl
        test   cl, cl
        je     loc_1_7E9D
        cmp    dword ptr [ebp - 0x1c], 0
        je     loc_1_7E9D
        cmp    byte ptr [esi + 0xe], 1
        jbe    loc_1_7E5F
        mov    eax, dword ptr [ebp - 0x1c]
        mov    eax, dword ptr [eax + 0xa]
        cmp    dword ptr [0], eax            ; -> loc_1_1A80
        jae    loc_1_7E5F
        mov    dword ptr [0], eax            ; -> loc_1_1A80
loc_1_7E5F:
        mov    eax, dword ptr [edi + 0x16]
        test   dword ptr [ebp - 0x18], eax
        je     loc_1_7E91
        mov    ecx, dword ptr [esi + 0x14]
        test   ecx, ecx
        je     loc_1_7E85
loc_1_7E6E:
        cmp    dword ptr [ecx + 0x16], eax
        je     loc_1_7E7B
        mov    ecx, dword ptr [ecx]
        test   ecx, ecx
        jne    loc_1_7E6E
        jmp    loc_1_7E81
loc_1_7E7B:
        mov    al, byte ptr [ecx + 0x12]
        mov    byte ptr [edi + 0x12], al
loc_1_7E81:
        test   ecx, ecx
        jne    loc_1_7E9D
loc_1_7E85:
        mov    dword ptr [ebp - 8], 0x19
        jmp    loc_1_7FEF
loc_1_7E91:
        or     dword ptr [ebp - 0x18], eax
        mov    al, byte ptr [ebp - 0xd]
        mov    byte ptr [edi + 0x12], al
        inc    byte ptr [ebp - 0xd]
loc_1_7E9D:
        inc    byte ptr [ebp - 0x11]
        mov    al, byte ptr [ebp - 0x11]
        cmp    byte ptr [esi + 0xe], al
        ja     loc_1_7D9C
loc_1_7EAC:
        xor    cl, cl
        mov    eax, 1
        mov    dl, byte ptr [esi + 0xd]
        cmp    dl, cl
        je     loc_1_7EC7
loc_1_7EBA:
        test   dword ptr [ebp - 0x18], eax
        je     loc_1_7EFC
        inc    cl
        add    eax, eax
        cmp    cl, dl
        jb     loc_1_7EBA
loc_1_7EC7:
        xor    dl, dl
        cmp    byte ptr [ebp - 0xd], dl
        je     loc_1_7F1B
loc_1_7ECE:
        xor    ecx, ecx
        mov    eax, dword ptr [esi + 0x14]
        cmp    eax, ecx
        je     loc_1_7F0C
loc_1_7ED7:
        cmp    byte ptr [eax + 0x11], 0
        je     loc_1_7EE8
        cmp    byte ptr [eax + 0x12], dl
        jne    loc_1_7EE8
        inc    ecx
        cmp    ecx, 1
        ja     loc_1_7F08
loc_1_7EE8:
        mov    eax, dword ptr [eax]
        test   eax, eax
        jne    loc_1_7ED7
        jmp    loc_1_7F0C
loc_1_7EF0:
        mov    dword ptr [ebp - 8], 0x19
        jmp    loc_1_7FEF
loc_1_7EFC:
        mov    dword ptr [ebp - 8], 0x19
        jmp    loc_1_7FEF
loc_1_7F08:
        or     byte ptr [eax + 0xc], 1
loc_1_7F0C:
        test   ecx, ecx
        je     loc_1_7FD2
        inc    dl
        cmp    byte ptr [ebp - 0xd], dl
        ja     loc_1_7ECE
loc_1_7F1B:
        xor    bl, bl
        cmp    byte ptr [esi + 0xd], bl
        jbe    loc_1_7FC4
loc_1_7F26:
        push   0xc8
        call   sub_1_2540
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        je     loc_1_7FE8
        push   0x28
        call   sub_1_2540
        add    esp, 4
        test   eax, eax
        je     loc_1_7FDB
        push   0
        lea    ecx, [esi + 0x10]
        push   edi
        mov    dword ptr [eax], edi
        mov    dword ptr [eax + 0xc], 0xffffffff
        mov    byte ptr [eax + 0x17], 0xff
        mov    byte ptr [eax + 0x18], 0xff
        mov    byte ptr [eax + 0x19], 0xff
        mov    word ptr [eax + 0x1a], 0xffff
        mov    word ptr [eax + 0x1c], 0xffff
        push   ecx
        mov    dword ptr [edi + 0x18], eax
        mov    byte ptr [edi + 0x30], 1
        mov    ax, word ptr [ebp - 2]
        mov    dword ptr [edi + 0xc], esi
        inc    word ptr [ebp - 2]
        mov    byte ptr [edi + 0x12], bl
        mov    byte ptr [edi + 0x13], 0x80
        mov    dword ptr [edi + 0x20], 0
        mov    word ptr [edi + 0x10], ax
        inc    bl
        call   sub_1_2D7C
        add    esp, 0xc
        push   4
        push   edi
        push   dword ptr [ebp - 0xc]
        push   0                             ; -> obj1:0x191C
        call   sub_1_2DB5
        add    esp, 0x10
        mov    dword ptr [ebp - 0xc], edi
        cmp    byte ptr [esi + 0xd], bl
        ja     loc_1_7F26
loc_1_7FC4:
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [eax], esi
        xor    eax, eax
loc_1_7FCB:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_7FD2:
        mov    dword ptr [ebp - 8], 0x19
        jmp    loc_1_7FEF
loc_1_7FDB:
        test   edi, edi
        je     loc_1_7FE8
        push   edi
        call   sub_1_254F
        add    esp, 4
loc_1_7FE8:
        mov    dword ptr [ebp - 8], 0x20
loc_1_7FEF:
        push   esi
        call   sub_1_8008
        add    esp, 4
        test   eax, eax
        je     loc_1_8003
        mov    dword ptr [ebp - 8], 0x19
loc_1_8003:
        mov    eax, dword ptr [ebp - 8]
        jmp    loc_1_7FCB
sub_1_8008:
        push   ebx
        push   esi
        push   edi
        push   ebp
        xor    ebp, ebp
        mov    edi, dword ptr [esp + 0x14]
        push   ebp
        lea    esi, [edi + 0x10]
        push   esi
        call   sub_1_2DDA
        add    esp, 8
        mov    ebx, eax
        cmp    ebx, ebp
        je     loc_1_8063
loc_1_8025:
        push   4
        push   ebx
        push   0                             ; -> obj1:0x191C
        call   sub_1_2E0B
        add    esp, 0xc
        push   dword ptr [ebx + 0x18]
        call   sub_1_254F
        add    esp, 4
        push   ebx
        call   sub_1_254F
        add    esp, 4
        test   eax, eax
        je     loc_1_8052
        mov    ebp, 0x19
loc_1_8052:
        push   0
        push   esi
        call   sub_1_2DDA
        add    esp, 8
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_1_8025
loc_1_8063:
        lea    esi, [edi + 0x14]
        push   0
        push   esi
        call   sub_1_2DDA
        add    esp, 8
        mov    ebx, eax
        test   ebx, ebx
        je     loc_1_80AA
loc_1_8077:
        push   4
        push   ebx
        push   0                             ; -> obj1:0x1920
        call   sub_1_2E0B
        add    esp, 0xc
        push   ebx
        call   sub_1_254F
        add    esp, 4
        test   eax, eax
        je     loc_1_8099
        mov    ebp, 0x19
loc_1_8099:
        push   0
        push   esi
        call   sub_1_2DDA
        add    esp, 8
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_1_8077
loc_1_80AA:
        push   0
        push   edi
        push   0                             ; -> obj1:0x1918
        call   sub_1_2E0B
        add    esp, 0xc
        push   0
        push   edi
        push   0                             ; -> obj1:0x1914
        call   sub_1_2D7C
        add    esp, 0xc
        mov    eax, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_80D1:
        mov    eax, dword ptr [esp + 4]
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 0x10]
        test   esi, esi
        je     loc_1_8124
        mov    edi, 0x80
loc_1_80E3:
        mov    ecx, esi
        call   sub_1_56A1
        mov    ecx, esi
        call   sub_1_2967
        test   di, ax
        je     loc_1_811E
        cmp    dword ptr [esp + 0x10], 1
        mov    word ptr [esi + 0x34], di
        mov    eax, 0
        adc    eax, -1
        and    ax, di
        mov    word ptr [esi + 0x36], ax
        cmp    word ptr [esi + 0x32], 0
        jne    loc_1_811E
        push   esi
        call   sub_1_5C2D
        add    esp, 4
loc_1_811E:
        mov    esi, dword ptr [esi]
        test   esi, esi
        jne    loc_1_80E3
loc_1_8124:
        xor    eax, eax
        pop    edi
        pop    esi
        ret    
sub_1_8129:
        push   ebp
        mov    eax, dword ptr [0x14]         ; -> sub_1_1980
        mov    ebp, esp
        mov    ecx, dword ptr [0xa]          ; -> obj1:0x1976
        mov    dx, word ptr [0]              ; -> obj1:0x196C
        sub    esp, 0x1c
        mov    word ptr [ebp - 2], dx
        mov    dword ptr [ebp - 0x1c], eax
        push   ebx
        mov    dword ptr [ebp - 0x18], ecx
        push   esi
        push   edi
        mov    eax, dword ptr [4]            ; -> obj1:0x1970
        mov    ecx, dword ptr [0]            ; -> obj1:0x1918
        mov    dword ptr [ebp - 0x14], eax
        mov    dword ptr [ebp - 8], ecx
        xor    eax, eax
        mov    dword ptr [ebp - 0xc], eax
        cmp    ecx, eax
        je     loc_1_81E7
loc_1_8168:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [ecx + 0x20], eax
        jne    loc_1_81DB
        add    ecx, 8
        mov    dword ptr [ebp - 0x10], ecx
        test   byte ptr [ecx], 8
        jne    loc_1_8182
        or     byte ptr [ebp - 0xc], 2
loc_1_8182:
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ecx + 0x18]
        mov    dword ptr [0x14], eax         ; -> sub_1_1980
        mov    edx, dword ptr [ecx + 0x1c]
        mov    byte ptr [1], 0x9e            ; -> obj1:0x196D
        mov    dword ptr [0xa], edx          ; -> obj1:0x1976
        mov    al, byte ptr [ecx + 0xc]
        mov    byte ptr [0], al              ; -> obj1:0x196C
        call   sub_1_2438
        test   eax, eax
        je     loc_1_81AF
        int3   
loc_1_81AF:
        cmp    word ptr [4], 0               ; -> obj1:0x1970
        je     loc_1_81DB
        mov    ecx, dword ptr [ebp - 0x10]
        mov    eax, dword ptr [ecx]
        test   al, 2
        jne    loc_1_81CA
        or     byte ptr [ebp - 0xc], 1
        or     al, 2
        mov    dword ptr [ecx], eax
loc_1_81CA:
        mov    ax, word ptr [4]              ; -> obj1:0x1970
        mov    ecx, dword ptr [ebp - 8]
        or     byte ptr [ebp - 0xc], 2
        or     word ptr [ecx + 0x24], ax
loc_1_81DB:
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        jne    loc_1_8168
loc_1_81E7:
        mov    dx, word ptr [ebp - 2]
        mov    eax, dword ptr [ebp - 0x1c]
        test   byte ptr [ebp - 0xc], 1
        mov    word ptr [0], dx              ; -> obj1:0x196C
        mov    ecx, dword ptr [ebp - 0x18]
        mov    dword ptr [0x14], eax         ; -> sub_1_1980
        mov    dword ptr [0xa], ecx          ; -> obj1:0x1976
        mov    eax, dword ptr [ebp - 0x14]
        mov    dword ptr [4], eax            ; -> obj1:0x1970
        je     loc_1_8222
        push   1
        push   0
        push   0                             ; -> sub_1_5A9F
        call   sub_1_9888
        add    esp, 0xc
loc_1_8222:
        mov    eax, dword ptr [ebp - 0xc]
        pop    edi
        and    eax, 2
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_822F:
        push   esi
        xor    eax, eax
        push   edi
        mov    esi, dword ptr [0]            ; -> obj1:0x1918
        mov    dword ptr [0], eax            ; -> obj1:0x1BFC
        cmp    esi, eax
        je     loc_1_82AD
loc_1_8242:
        test   byte ptr [esi + 8], 1
        je     loc_1_82A7
        mov    edi, dword ptr [esi + 0x10]
        test   edi, edi
        je     loc_1_82A7
loc_1_824F:
        mov    eax, dword ptr [esi + 0x18]
        mov    dword ptr [0x14], eax         ; -> sub_1_1980
        mov    ecx, dword ptr [esi + 0x1c]
        mov    byte ptr [1], 0x8f            ; -> obj1:0x196D
        mov    dword ptr [0xa], ecx          ; -> obj1:0x1976
        mov    al, byte ptr [esi + 0xc]
        mov    byte ptr [0], al              ; -> obj1:0x196C
        mov    al, byte ptr [edi + 0x12]
        mov    byte ptr [2], al              ; -> obj1:0x196E
        call   sub_1_2438
        test   eax, eax
        jne    loc_1_82A1
        mov    ax, word ptr [edi + 0x36]
        movzx  cx, byte ptr [3]              ; -> obj1:0x196F
        xor    ax, cx
        test   al, 0x80
        je     loc_1_82A1
        mov    ax, 1
        mov    cl, byte ptr [edi + 0x12]
        shl    ax, cl
        or     word ptr [esi + 0x24], ax
loc_1_82A1:
        mov    edi, dword ptr [edi]
        test   edi, edi
        jne    loc_1_824F
loc_1_82A7:
        mov    esi, dword ptr [esi]
        test   esi, esi
        jne    loc_1_8242
loc_1_82AD:
        call   sub_1_5A9F
        push   0
        push   0x7d0
        push   0                             ; -> sub_1_8928
        call   sub_3_53B0
loc_1_82C3:
        pop    edi
        mov    dword ptr [0], eax            ; -> obj1:0x1BF8
        pop    esi
        ret    
sub_1_82CB:
        mov    eax, dword ptr [esp + 4]
        push   esi
        test   byte ptr [eax + 0x36], 0x80
        je     loc_1_82DC
        mov    word ptr [eax + 0x34], 0x80
loc_1_82DC:
        mov    si, 1
        mov    edx, dword ptr [eax + 0xc]
        mov    cl, byte ptr [eax + 0x12]
        shl    si, cl
        or     word ptr [edx + 0x24], si
        call   sub_1_5A9F
        pop    esi
        ret    
loc_1_82F4:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    esi, dword ptr [0xa]          ; -> loc_1_199E
        mov    eax, esi
        mov    ax, word ptr [eax]
        test   ah, 0xf8
        je     loc_1_8313
        mov    eax, 2
        jmp    loc_1_839D
loc_1_8313:
        test   al, 1
        jne    loc_1_834F
        cmp    word ptr [0], 0x2e            ; -> loc_1_1994
        jne    loc_1_8334
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [0xa]          ; -> loc_1_199E
        mov    ax, word ptr [eax + 0xa]
        mov    word ptr [ecx + 2], ax
        jmp    loc_1_839B
loc_1_8334:
        mov    cx, word ptr [esi + 2]
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [eax + 0xa], cx
        call   sub_1_2A6F
        call   sub_1_9619
        test   eax, eax
        je     loc_1_839B
        jmp    loc_1_839D
loc_1_834F:
        mov    edi, dword ptr [ebp + 0xc]
        push   edi
        push   dword ptr [ebp + 8]
        call   sub_1_2635
        add    esp, 8
        test   eax, eax
        jne    loc_1_8369
        mov    eax, 0xb
        jmp    loc_1_839D
loc_1_8369:
        cmp    word ptr [0], 0x2e            ; -> loc_1_1994
        jne    loc_1_837D
        mov    ax, word ptr [eax + 0x10]
        mov    word ptr [esi + 2], ax
        jmp    loc_1_839B
loc_1_837D:
        mov    cx, word ptr [esi + 2]
        push   edi
        mov    word ptr [eax + 0x10], cx
        call   sub_1_9572
        add    esp, 4
        push   edi
        call   sub_1_95A0
        add    esp, 4
        test   eax, eax
        jne    loc_1_839D
loc_1_839B:
        xor    eax, eax
loc_1_839D:
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_1_83A1:
        push   dword ptr [esp + 4]
        call   sub_1_27C1
        add    esp, 4
        test   eax, eax
        jne    loc_1_83CF
        mov    ecx, dword ptr [esp + 8]
        movzx  ax, byte ptr [3]              ; -> obj1:0x196F
        mov    word ptr [ecx + 2], ax
        xor    eax, eax
        movzx  dx, byte ptr [9]              ; -> obj1:0x1975
        mov    word ptr [ecx + 4], dx
loc_1_83CF:
        ret    
loc_1_83D0:
        mov    eax, dword ptr [0xa]          ; -> loc_1_199E
        push   eax
        push   dword ptr [esp + 0xc]
        call   sub_1_83A1
        add    esp, 8
        ret    
sub_1_83E3:
        push   ebp
        mov    al, byte ptr [esp + 0x14]
        mov    ebp, esp
        sub    esp, 8
        test   al, 0x20
        push   ebx
        push   esi
        push   edi
        je     loc_1_86BB
        test   al, 0xc0
        jne    loc_1_86BB
        mov    esi, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [esi + 8]
        test   eax, 0x80000000
        je     loc_1_8417
        mov    eax, 0x1e
        jmp    loc_1_86C0
loc_1_8417:
        test   eax, 0x1800000
        je     loc_1_8428
        mov    eax, 0x19
        jmp    loc_1_86C0
loc_1_8428:
        mov    eax, dword ptr [esi]
        mov    ecx, dword ptr [eax + 0xc]
        mov    edx, dword ptr [ecx + 0x18]
        mov    dword ptr [0x14], edx         ; -> sub_1_1980
        mov    eax, dword ptr [esi]
        mov    ecx, dword ptr [eax + 0xc]
        mov    edx, dword ptr [ecx + 0x1c]
        mov    byte ptr [1], 0x8c            ; -> obj1:0x196D
        mov    dword ptr [0xa], edx          ; -> obj1:0x1976
        mov    eax, dword ptr [esi]
        mov    ecx, dword ptr [eax + 0xc]
        mov    dl, byte ptr [ecx + 0xc]
        mov    byte ptr [0], dl              ; -> obj1:0x196C
        mov    eax, dword ptr [esi]
        mov    al, byte ptr [eax + 0x12]
        mov    dword ptr [0xe], 0            ; -> obj1:0x197A
        mov    byte ptr [2], al              ; -> obj1:0x196E
        call   sub_1_2438
        test   eax, eax
        jne    loc_1_86C0
        test   byte ptr [4], 2               ; -> loc_1_19D8
        jne    loc_1_848D
        mov    eax, 0xb
        jmp    loc_1_86C0
loc_1_848D:
        mov    ecx, dword ptr [ebp + 0x10]
        test   byte ptr [ecx], 2
        je     loc_1_849D
        mov    ax, word ptr [0xa]            ; -> loc_1_19DE
        jmp    loc_1_84A3
loc_1_849D:
        mov    ax, word ptr [6]              ; -> loc_1_19DA
loc_1_84A3:
        test   byte ptr [ebp + 0x17], 0x40
        mov    word ptr [ebp - 4], ax
        mov    ecx, dword ptr [ebp + 0x10]
        je     loc_1_84CA
        mov    ax, word ptr [ecx + 4]
        and    ax, word ptr [ebp - 4]
        mov    word ptr [ecx + 4], ax
        cmp    ax, 1
        sbb    eax, eax
        and    eax, 0x19
        jmp    loc_1_86C0
loc_1_84CA:
        mov    ax, word ptr [ecx + 2]
        mov    dx, word ptr [ecx + 4]
        mov    word ptr [ebp - 2], ax
        mov    word ptr [ebp - 6], dx
        test   ax, ax
        je     loc_1_84F0
        mov    edx, 1
        mov    cl, al
        shl    edx, cl
        movzx  eax, word ptr [ebp - 4]
        test   eax, edx
        je     loc_1_84FE
loc_1_84F0:
        cmp    word ptr [ebp - 2], 0
        jne    loc_1_8508
        cmp    word ptr [ebp - 6], 0
        jne    loc_1_8508
loc_1_84FE:
        mov    eax, 6
        jmp    loc_1_86C0
loc_1_8508:
        xor    cl, cl
        mov    eax, dword ptr [esi]
        mov    edi, dword ptr [eax + 0x18]
        mov    dl, byte ptr [eax + 0x30]
        cmp    dl, cl
        je     loc_1_8540
loc_1_8516:
        cmp    esi, edi
        je     loc_1_8537
        mov    eax, dword ptr [edi + 0x24]
        test   eax, eax
        je     loc_1_8537
loc_1_8521:
        cmp    byte ptr [eax + 0x1a], 2
        jne    loc_1_8530
        movzx  ebx, word ptr [ebp - 2]
        cmp    dword ptr [eax + 8], ebx
        jne    loc_1_855A
loc_1_8530:
        mov    eax, dword ptr [eax + 4]
        test   eax, eax
        jne    loc_1_8521
loc_1_8537:
        inc    cl
        add    edi, 0x28
        cmp    cl, dl
        jb     loc_1_8516
loc_1_8540:
        push   0x1b
        call   sub_1_2540
        add    esp, 4
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_1_8564
        mov    eax, 0x20
        jmp    loc_1_86C0
loc_1_855A:
        mov    eax, 6
        jmp    loc_1_86C0
loc_1_8564:
        cmp    dword ptr [ebp + 8], 0        ; -> obj1:0x192C
        jne    loc_1_8612
        test   byte ptr [ebp + 0x17], 0x80
        jne    loc_1_860C
        mov    eax, dword ptr [esi]
        mov    ecx, dword ptr [eax + 0xc]
        mov    edx, dword ptr [ecx + 0x18]
        mov    dword ptr [0x14], edx         ; -> sub_1_1980
        mov    eax, dword ptr [esi]
        mov    ecx, dword ptr [eax + 0xc]
        mov    edx, dword ptr [ecx + 0x1c]
        mov    byte ptr [1], 0x8d            ; -> obj1:0x196D
        mov    dword ptr [0xa], edx          ; -> obj1:0x1976
        mov    eax, dword ptr [esi]
        mov    ecx, dword ptr [eax + 0xc]
        mov    dl, byte ptr [ecx + 0xc]
        mov    byte ptr [0], dl              ; -> obj1:0x196C
        mov    eax, dword ptr [esi]
        mov    al, byte ptr [eax + 0x12]
        mov    byte ptr [2], al              ; -> obj1:0x196E
        call   sub_1_2438
        mov    edi, eax
        test   edi, edi
        jne    loc_1_8652
        mov    ecx, dword ptr [ebp + 0x10]
        and    word ptr [0xe], 0xffe0        ; -> obj1:0x197A
        mov    ax, word ptr [ecx]
        and    ax, 2
        mov    cx, word ptr [ebp - 2]
        cmp    ax, 1
        mov    byte ptr [1], 0x8e            ; -> obj1:0x196D
        sbb    eax, eax
        and    ax, 0x40
        and    cx, 0x1f
        or     ax, cx
        or     al, 0x80
        or     word ptr [0xe], ax            ; -> obj1:0x197A
        call   sub_1_2438
        mov    edi, eax
        test   edi, edi
        jne    loc_1_8652
        or     byte ptr [esi + 0xa], 0x80
loc_1_860C:
        or     byte ptr [esi + 0xb], 0x80
        jmp    loc_1_8669
loc_1_8612:
        test   byte ptr [ebp + 0x17], 0x10
        je     loc_1_8669
        mov    edx, 0                        ; -> loc_1_1A58
        xor    eax, eax
        mov    edi, edx
        mov    ecx, 9
        rep stosd dword ptr es:[edi], eax
        mov    ax, word ptr [ebp - 4]
        push   edx
        and    ax, word ptr [ebp - 6]
        mov    word ptr [4], ax              ; -> loc_1_1A5C
        push   dword ptr [esi]
        call   sub_3_23C6
loc_1_863E:
        add    esp, 8
        test   eax, eax
        je     loc_1_865F
        cmp    word ptr [ebp - 2], 1
        sbb    edi, edi
        and    edi, 0x1a
        add    edi, 6
loc_1_8652:
        push   ebx
        call   sub_1_254F
        add    esp, 4
        mov    eax, edi
        jmp    loc_1_86C0
loc_1_865F:
        mov    ax, word ptr [2]              ; -> loc_1_1A5A
        mov    word ptr [ebp - 2], ax
loc_1_8669:
        movzx  eax, word ptr [ebp - 2]
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0x10]
        mov    dword ptr [ebx + 8], eax
        mov    dword ptr [ebx + 0xc], 1
        mov    byte ptr [ebx + 0x1a], 2
        mov    dword ptr [ebx + 0x10], ecx
        mov    dword ptr [ebx + 0x14], esi
        test   byte ptr [edx], 2
        je     loc_1_8692
        or     byte ptr [esi + 0xa], 4
        jmp    loc_1_8696
loc_1_8692:
        and    byte ptr [esi + 0xa], 0xfb
loc_1_8696:
        push   4
        add    esi, 0x24
        push   ebx
        push   esi
        call   sub_1_2D57
        add    esp, 0xc
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x1c
        push   0
        push   ebx
        push   eax
        call   sub_1_2D57
        add    esp, 0xc
        xor    eax, eax
        jmp    loc_1_86C0
loc_1_86BB:
        mov    eax, 0x1c
loc_1_86C0:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_86C7:
        mov    ecx, dword ptr [esp + 8]
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [ecx + 0x18]
        mov    esi, dword ptr [0xa]          ; -> loc_1_199E
        lea    ebp, [esi + 5]
        mov    al, byte ptr [ebp]
        test   al, 0x20
        je     loc_1_887C
        test   al, 0xc0
        jne    loc_1_887C
        mov    eax, dword ptr [ebx + 8]
        test   eax, 0x11000000
        je     loc_1_8702
        mov    eax, 0x1d
        jmp    loc_1_8881
loc_1_8702:
        test   eax, 0x800000
        je     loc_1_8713
        mov    eax, 0x1e
        jmp    loc_1_8881
loc_1_8713:
        lea    edx, [ecx + 0xc]
        mov    eax, dword ptr [edx]
        mov    edi, dword ptr [eax + 0x18]
        mov    dword ptr [0x14], edi         ; -> sub_1_1980
        mov    eax, dword ptr [edx]
        mov    edi, dword ptr [eax + 0x1c]
        mov    byte ptr [1], 0x8c            ; -> obj1:0x196D
        mov    dword ptr [0xa], edi          ; -> obj1:0x1976
        mov    eax, dword ptr [edx]
        mov    al, byte ptr [eax + 0xc]
        mov    byte ptr [0], al              ; -> obj1:0x196C
        mov    cl, byte ptr [ecx + 0x12]
        mov    dword ptr [0xe], 0            ; -> obj1:0x197A
        mov    byte ptr [2], cl              ; -> obj1:0x196E
        call   sub_1_2438
        test   eax, eax
        jne    loc_1_8881
        test   byte ptr [4], 2               ; -> loc_1_19D8
        jne    loc_1_8770
        mov    eax, 0xb
        jmp    loc_1_8881
loc_1_8770:
        test   byte ptr [esi + 2], 7
        je     loc_1_8780
        mov    eax, 2
        jmp    loc_1_8881
loc_1_8780:
        mov    eax, dword ptr [ebx + 8]
        test   eax, 0x8000000
        je     loc_1_8815
        test   eax, 0x80000000
        jne    loc_1_879F
        mov    eax, 0x20
        jmp    loc_1_8881
loc_1_879F:
        mov    edi, dword ptr [ebx + 0x24]
        test   edi, edi
        je     loc_1_880E
loc_1_87A6:
        cmp    byte ptr [edi + 0x1a], 2
        je     loc_1_87B3
        mov    edi, dword ptr [edi + 4]
        test   edi, edi
        jne    loc_1_87A6
loc_1_87B3:
        test   edi, edi
        je     loc_1_880E
        cmp    dword ptr [edi + 0x10], 0     ; -> obj1:0x192C
        jne    loc_1_880E
        mov    al, byte ptr [ebp]
        test   al, 0x10
        je     loc_1_87E3
        mov    edx, 1
        mov    cl, byte ptr [edi + 8]
        shl    edx, cl
        movzx  eax, word ptr [esi + 6]
        test   eax, edx
        jne    loc_1_87FF
        mov    eax, 6
        jmp    loc_1_8881
loc_1_87E3:
        movzx  eax, al
        mov    ecx, dword ptr [edi + 8]
        sub    ecx, eax
        cmp    ecx, 1
        sbb    eax, eax
        inc    eax
        test   al, 0xf
        je     loc_1_87FF
        mov    eax, 6
        jmp    loc_1_8881
loc_1_87FF:
        push   edi
        push   dword ptr [esp + 0x18]
        call   sub_1_6288
        add    esp, 8
        jmp    loc_1_8868
loc_1_880E:
        mov    eax, 0x19
        jmp    loc_1_8881
loc_1_8815:
        mov    edi, 0                        ; -> loc_1_1A58
        xor    eax, eax
        mov    ecx, 9
        rep stosd dword ptr es:[edi], eax
        mov    cl, byte ptr [ebp]
        test   cl, 0x10
        je     loc_1_8831
        mov    ax, word ptr [esi + 6]
        jmp    loc_1_883B
loc_1_8831:
        mov    ax, 1
        and    cl, 0xf
        shl    ax, cl
loc_1_883B:
        mov    word ptr [4], ax              ; -> loc_1_1A5C
        movzx  eax, byte ptr [ebp]
        or     eax, 0x10000000
        push   eax
        push   0                             ; -> loc_1_1A58
        push   ebx
        push   dword ptr [esp + 0x20]
        call   sub_1_83E3
        add    esp, 0x10
        test   eax, eax
        jne    loc_1_8881
        mov    eax, dword ptr [esp + 0x14]
        mov    edi, dword ptr [eax + 0x1c]
loc_1_8868:
        mov    al, byte ptr [edi + 8]
        mov    byte ptr [esi + 4], al
        xor    eax, eax
        and    word ptr [esi + 2], 0xfeff
        or     byte ptr [ebx + 0xa], 0x80
        jmp    loc_1_8881
loc_1_887C:
        mov    eax, 0x1c
loc_1_8881:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_8886:
        mov    eax, dword ptr [esp + 8]
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 0x18]
        mov    ecx, dword ptr [esi + 8]
        test   ecx, 0x11000000
        je     loc_1_88A4
        mov    eax, 0x1d
        jmp    loc_1_8925
loc_1_88A4:
        mov    edi, dword ptr [esp + 0xc]
        mov    edx, dword ptr [edi + 0x1c]
        test   edx, edx
        je     loc_1_88D2
loc_1_88AF:
        cmp    byte ptr [edx + 0x1a], 2
        jne    loc_1_88C8
        mov    eax, dword ptr [0xa]          ; -> loc_1_199E
        movzx  eax, byte ptr [eax + 4]
        cmp    eax, dword ptr [edx + 8]
        jne    loc_1_88C8
        cmp    dword ptr [edx + 0x14], esi
        je     loc_1_88CE
loc_1_88C8:
        mov    edx, dword ptr [edx]
        test   edx, edx
        jne    loc_1_88AF
loc_1_88CE:
        test   edx, edx
        jne    loc_1_88D9
loc_1_88D2:
        mov    eax, 6
        jmp    loc_1_8925
loc_1_88D9:
        cmp    dword ptr [edx + 0x10], edi
        je     loc_1_88E5
        mov    eax, 0x19
        jmp    loc_1_8925
loc_1_88E5:
        mov    eax, dword ptr [0xa]          ; -> loc_1_199E
        mov    ax, word ptr [eax + 2]
        cmp    word ptr [edx + 0x18], ax
        je     loc_1_88FB
        mov    eax, 2
        jmp    loc_1_8925
loc_1_88FB:
        test   ecx, 0x8000000
        push   edx
        je     loc_1_8913
        push   0                             ; -> obj1:0x192C
        call   sub_1_6288
        add    esp, 8
        jmp    loc_1_891F
loc_1_8913:
        call   sub_1_62B7
        add    esp, 4
        test   eax, eax
        jne    loc_1_8925
loc_1_891F:
        and    byte ptr [esi + 0xa], 0x7f
        xor    eax, eax
loc_1_8925:
        pop    edi
        pop    esi
        ret    
sub_1_8928:
        push   esi
        push   1
        xor    esi, esi
        push   esi
        mov    dword ptr [0], esi            ; -> obj1:0x1BF8
        push   0                             ; -> loc_1_822F
        call   sub_1_9888
        add    esp, 0xc
        mov    dword ptr [0], eax            ; -> obj1:0x1BFC
        cmp    eax, esi
        jne    loc_1_895F
        push   esi
        push   0x7d0
        push   0                             ; -> sub_1_8928
        call   sub_3_53B0
loc_1_895A:
        mov    dword ptr [0], eax            ; -> obj1:0x1BF8
loc_1_895F:
        pop    esi
        ret    
sub_1_8961:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        mov    byte ptr [1], 0x85            ; -> obj1:0x196D
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    al, byte ptr [esi + 0xc]
        mov    byte ptr [0], al              ; -> obj1:0x196C
        mov    ecx, dword ptr [esi + 0x18]
        mov    dword ptr [0x14], ecx         ; -> sub_1_1980
        mov    edx, dword ptr [esi + 0x1c]
        mov    dword ptr [0xa], edx          ; -> obj1:0x1976
        call   sub_1_2438
        test   eax, eax
        jne    loc_1_8ADA
        test   byte ptr [0xe], 0x1f          ; -> obj1:0x197A
        jne    loc_1_89DF
        mov    byte ptr [1], 0x86            ; -> obj1:0x196D
        and    word ptr [0xe], 0xff7f        ; -> obj1:0x197A
        or     byte ptr [0xe], 0x40          ; -> obj1:0x197A
        call   sub_1_2438
        or     byte ptr [esi + 8], 1
        mov    eax, dword ptr [0]            ; -> obj1:0x1BF4
        inc    dword ptr [0]                 ; -> obj1:0x1BF4
        test   eax, eax
        jne    loc_1_89D8
        call   sub_1_8928
loc_1_89D8:
        xor    eax, eax
        jmp    loc_1_8ADA
loc_1_89DF:
        push   2
        lea    ecx, [ebp - 8]
        push   dword ptr [esi + 4]
        push   ecx
        call   sub_3_5300
loc_1_89ED:
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_8AD5
        push   0
        lea    eax, [ebp - 0xc]
        push   eax
        lea    edx, [ebp - 4]
        push   4
        push   dword ptr [ebp - 8]
        push   edx
        call   sub_3_52F8
loc_1_8A0C:
        add    esp, 0x14
        test   eax, eax
        jne    loc_1_8AD5
        mov    cx, word ptr [0xe]            ; -> obj1:0x197A
        mov    eax, dword ptr [ebp - 4]
        and    cx, 0x1f
        cmp    word ptr [eax + 2], cx
        jne    loc_1_8AD5
        push   0x20
        call   sub_1_2540
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        jne    loc_1_8A49
        mov    eax, 0x20
        jmp    loc_1_8ADA
loc_1_8A49:
        mov    ax, word ptr [0xe]            ; -> obj1:0x197A
        lea    ebx, [esi + 8]
        and    ax, 0x1f
        mov    word ptr [edi], ax
        mov    word ptr [edi + 2], 0
        mov    ecx, dword ptr [ebp - 4]
        and    byte ptr [ebx], 0xf7
        test   byte ptr [ecx], 1
        je     loc_1_8A73
        mov    word ptr [edi + 2], 2
        or     byte ptr [ebx], 8
loc_1_8A73:
        add    esi, 0x20
        push   edi
        mov    dword ptr [edi + 4], 0        ; -> loc_1_23C2
        call   sub_3_540C
loc_1_8A83:
        push   edi
        mov    dword ptr [esi], eax
        test   eax, eax
        jne    loc_1_8A94
        call   sub_1_254F
        add    esp, 4
        jmp    loc_1_8AD5
loc_1_8A94:
        call   sub_1_254F
        add    esp, 4
        push   dword ptr [esi]
        call   sub_3_53FC
loc_1_8AA3:
        and    byte ptr [ebx], 0xfe
        xor    ebx, ebx
        mov    byte ptr [1], 0x86            ; -> obj1:0x196D
        or     byte ptr [0xe], 0xc0          ; -> obj1:0x197A
        call   sub_1_2438
        mov    edi, eax
        test   edi, edi
        je     loc_1_8AD1
        push   dword ptr [esi]
        call   sub_3_53EC
loc_1_8AC8:
        push   dword ptr [esi]
        call   sub_3_53DC
loc_1_8ACF:
        mov    dword ptr [esi], ebx
loc_1_8AD1:
        mov    eax, edi
        jmp    loc_1_8ADA
loc_1_8AD5:
        mov    eax, 0x19
loc_1_8ADA:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_8AE1:
        push   ebp
        mov    eax, dword ptr [0xa]          ; -> loc_1_199E
        mov    ebp, esp
        sub    esp, 4
        cmp    word ptr [eax], 3
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [4]            ; -> loc_1_1998
        je     loc_1_8B05
        mov    eax, 0x16
        jmp    loc_1_8C02
loc_1_8B05:
        mov    eax, dword ptr [4]            ; -> loc_1_1998
        mov    ebx, dword ptr [0xa]          ; -> loc_1_199E
        add    ebx, 2
        mov    dword ptr [0x14], eax         ; -> sub_1_1980
        mov    eax, dword ptr [ebx]
        mov    byte ptr [1], 0x83            ; -> obj1:0x196D
        mov    byte ptr [0], 0               ; -> obj1:0x196C
        mov    dword ptr [0xa], eax          ; -> obj1:0x1976
        call   sub_1_2438
        test   eax, eax
        jne    loc_1_8C02
        mov    di, word ptr [2]              ; -> obj1:0x196E
        cmp    di, word ptr [0]              ; -> obj1:0x23C0
        ja     loc_1_8BFD
        cmp    di, 0x201
        jb     loc_1_8BFD
        lea    eax, [ebp - 4]
        push   eax
        push   esi
        push   dword ptr [ebx]
        call   sub_1_7BDB
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_8C02
        mov    eax, dword ptr [ebp - 4]
        push   1
        mov    word ptr [eax + 0x26], di
        xor    esi, esi
        push   dword ptr [ebp - 4]
        call   sub_1_80D1
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        jne    loc_1_8BE5
        push   dword ptr [ebp - 4]
        call   sub_1_8961
        add    esp, 4
        mov    edi, eax
        cmp    edi, esi
        jne    loc_1_8BE5
        push   0xa
        call   sub_1_2540
        add    esp, 4
        mov    edi, eax
        cmp    edi, esi
        je     loc_1_8BE1
        mov    dword ptr [edi], esi
        mov    dword ptr [edi + 4], esi
        mov    word ptr [edi + 8], 0x16
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x10]
        mov    dword ptr [edi], eax
        test   eax, eax
        je     loc_1_8BD8
        push   edi
        push   0x32
        push   0                             ; -> loc_1_5F1F
        call   sub_1_2566
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_8BE1
loc_1_8BD8:
        push   edi
        call   sub_1_254F
        add    esp, 4
loc_1_8BE1:
        xor    eax, eax
        jmp    loc_1_8C02
loc_1_8BE5:
        push   dword ptr [ebp - 4]
        call   sub_1_8008
        add    esp, 4
        test   eax, eax
        je     loc_1_8BF9
        mov    edi, 0x19
loc_1_8BF9:
        mov    eax, edi
        jmp    loc_1_8C02
loc_1_8BFD:
        mov    eax, 0x19
loc_1_8C02:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_8C09:
        push   esi
        mov    eax, dword ptr [0xa]          ; -> loc_1_199E
        cmp    word ptr [eax + 4], 3
        je     loc_1_8C1D
        mov    eax, 0x16
        jmp    loc_1_8C65
loc_1_8C1D:
        mov    esi, dword ptr [0]            ; -> obj1:0x1918
        test   esi, esi
        je     loc_1_8C3E
        mov    eax, dword ptr [0xa]          ; -> loc_1_199E
        mov    eax, dword ptr [eax + 6]
loc_1_8C2F:
        cmp    dword ptr [esi + 0x1c], eax
        je     loc_1_8C3A
        mov    esi, dword ptr [esi]
        test   esi, esi
        jne    loc_1_8C2F
loc_1_8C3A:
        test   esi, esi
        jne    loc_1_8C45
loc_1_8C3E:
        mov    eax, 0x19
        jmp    loc_1_8C65
loc_1_8C45:
        push   esi
        call   sub_1_8CBE
        add    esp, 4
        push   esi
        call   sub_1_8961
        add    esp, 4
        cmp    eax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 0x19
loc_1_8C65:
        pop    esi
        ret    
sub_1_8C67:
        push   esi
        mov    eax, dword ptr [0]            ; -> obj1:0x1918
        push   edi
        test   eax, eax
        mov    edi, dword ptr [esp + 0xc]
        je     loc_1_8C80
loc_1_8C76:
        cmp    edi, eax
        je     loc_1_8C8B
        mov    eax, dword ptr [eax]
        test   eax, eax
        jne    loc_1_8C76
loc_1_8C80:
        cmp    edi, eax
        je     loc_1_8C8B
        mov    eax, 0x19
        jmp    loc_1_8CBB
loc_1_8C8B:
        mov    esi, dword ptr [edi + 0x10]
        test   esi, esi
        je     loc_1_8CAA
loc_1_8C92:
        mov    ecx, esi
        call   sub_1_2967
        test   al, 0x80
        je     loc_1_8CA4
        mov    ecx, esi
        call   sub_1_5614
loc_1_8CA4:
        mov    esi, dword ptr [esi]
        test   esi, esi
        jne    loc_1_8C92
loc_1_8CAA:
        push   edi
        call   sub_1_8961
        add    esp, 4
        test   eax, eax
        jne    loc_1_8CBB
        or     byte ptr [edi + 8], 0x10
loc_1_8CBB:
        pop    edi
        pop    esi
        ret    
sub_1_8CBE:
        push   ebx
        mov    eax, dword ptr [0]            ; -> obj1:0x1918
        push   esi
        test   eax, eax
        mov    esi, dword ptr [esp + 0xc]
        push   edi
        je     loc_1_8CD8
loc_1_8CCE:
        cmp    eax, esi
        je     loc_1_8CE3
        mov    eax, dword ptr [eax]
        test   eax, eax
        jne    loc_1_8CCE
loc_1_8CD8:
        cmp    eax, esi
        je     loc_1_8CE3
        mov    eax, 0x19
        jmp    loc_1_8D61
loc_1_8CE3:
        lea    edi, [esi + 8]
        mov    eax, dword ptr [esi + 0x10]
        and    byte ptr [edi], 0xef
        test   eax, eax
        je     loc_1_8CFA
loc_1_8CF0:
        or     byte ptr [eax + 0x14], 0x20
        mov    eax, dword ptr [eax]
        test   eax, eax
        jne    loc_1_8CF0
loc_1_8CFA:
        add    esi, 0x20
        xor    ebx, ebx
        mov    eax, dword ptr [esi]
        test   eax, eax
        je     loc_1_8D14
        push   eax
        call   sub_3_53EC
loc_1_8D0B:
        push   dword ptr [esi]
        call   sub_3_53DC
loc_1_8D12:
        mov    dword ptr [esi], ebx
loc_1_8D14:
        mov    eax, dword ptr [edi]
        test   al, 1
        je     loc_1_8D5F
        and    al, 0xfe
        mov    dword ptr [edi], eax
        dec    dword ptr [0]                 ; -> obj1:0x1BF4
        jne    loc_1_8D5F
        xor    esi, esi
        cmp    dword ptr [0], esi            ; -> obj1:0x1BF8
        je     loc_1_8D41
        mov    eax, dword ptr [0]            ; -> obj1:0x1BF8
        push   eax
        call   sub_3_53CC
loc_1_8D3B:
        mov    dword ptr [0], esi            ; -> obj1:0x1BF8
loc_1_8D41:
        xor    esi, esi
        cmp    dword ptr [0], esi            ; -> obj1:0x1BFC
        je     loc_1_8D5F
        mov    eax, dword ptr [0]            ; -> obj1:0x1BFC
        push   eax
        call   sub_1_9880
        add    esp, 4
        mov    dword ptr [0], esi            ; -> obj1:0x1BFC
loc_1_8D5F:
        xor    eax, eax
loc_1_8D61:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_8D65:
        push   ebp
        mov    edx, dword ptr [esp + 0xc]
        mov    ebp, esp
        sub    esp, 0x28
        xor    eax, eax
        mov    ecx, 8
        push   ebx
        push   esi
        push   edi
        lea    esi, [edx + 0xc]
        lea    edi, [ebp - 0x28]
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        stosb  byte ptr es:[edi], al
        mov    edi, dword ptr [ebp + 0x10]
        and    edi, 0x40000000
        je     loc_1_8DA8
        movzx  eax, word ptr [esi + 4]
        mov    dword ptr [ebp - 0x16], eax
        movzx  ecx, word ptr [esi + 6]
        mov    dword ptr [ebp - 0x12], ecx
        movzx  eax, word ptr [esi]
        not    eax
        inc    eax
        mov    dword ptr [ebp - 0xa], eax
        jmp    loc_1_8DAF
loc_1_8DA8:
        movzx  eax, word ptr [edx + 4]
        mov    dword ptr [ebp - 0x1e], eax
loc_1_8DAF:
        mov    word ptr [ebp - 0x20], 4
        mov    byte ptr [ebp - 6], 0x80
        mov    ebx, dword ptr [ebp + 0x14]
        test   byte ptr [ebp + 0x10], 8
        movzx  eax, word ptr [ebx]
        mov    dword ptr [ebp - 0x1a], eax
        je     loc_1_8DCE
        mov    word ptr [ebp - 0x20], 0xc
loc_1_8DCE:
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0x10]
        lea    edx, [ebp - 4]
        mov    dword ptr [ebp - 0x24], eax
        push   edx
        mov    dword ptr [ebp - 0x28], ecx
        lea    ecx, [ebp - 0x28]
        push   ecx
        call   sub_1_3B7B
        add    esp, 8
        test   eax, eax
        je     loc_1_8E07
        cmp    eax, 0xa
        jne    loc_1_8E1B
        test   ebx, ebx
        je     loc_1_8E1B
        movzx  ecx, word ptr [ebx]
        cmp    ecx, dword ptr [ebp - 0xe]
        jbe    loc_1_8E1B
        mov    ecx, dword ptr [ebp - 0xe]
        mov    word ptr [ebx], cx
        jmp    loc_1_8E1B
loc_1_8E07:
        test   edi, edi
        je     loc_1_8E22
        mov    eax, dword ptr [ebp - 0x16]
        mov    word ptr [esi + 4], ax
        mov    ecx, dword ptr [ebp - 0x12]
        mov    word ptr [esi + 6], cx
loc_1_8E19:
        xor    eax, eax
loc_1_8E1B:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_8E22:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x10], 0     ; -> obj1:0x192C
        jne    loc_1_8E19
        test   byte ptr [ebp + 0x13], 0x80
        jne    loc_1_8E19
        push   dword ptr [ebp - 4]
        call   sub_1_4053
        add    esp, 4
        jmp    loc_1_8E1B
sub_1_8E41:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        add    eax, 8
        sub    esp, 0x18
        mov    dword ptr [ebp - 0x14], eax
        mov    eax, dword ptr [eax]
        mov    ecx, 0x60000000
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0x10]
        mov    edx, eax
        and    edx, ecx
        cmp    edx, ecx
        jne    loc_1_8E70
        mov    eax, 0x1e
        jmp    loc_1_90CE
loc_1_8E70:
        test   eax, 0x1000000
        jne    loc_1_90C9
        test   eax, 0x200000
        je     loc_1_8E8D
        test   eax, 0x20000000
        je     loc_1_90C9
loc_1_8E8D:
        test   eax, 0x400000
        je     loc_1_8E9F
        test   eax, 0x40000000
        je     loc_1_90C9
loc_1_8E9F:
        push   0x1b
        call   sub_1_2540
        add    esp, 4
        mov    esi, eax
        test   esi, esi
        jne    loc_1_8EB9
        mov    eax, 0x20
        jmp    loc_1_90CE
loc_1_8EB9:
        mov    eax, dword ptr [ebp + 0x14]
        and    eax, 0x40000000
        mov    dword ptr [ebp - 0x18], eax
        je     loc_1_8ED7
        lea    ecx, [esi + 0xc]
        movzx  eax, word ptr [edi + 0xe]
        mov    dword ptr [ecx], eax
        mov    dl, byte ptr [edi + 0x17]
        mov    byte ptr [esi + 0x18], dl
        jmp    loc_1_8F04
loc_1_8ED7:
        movzx  eax, word ptr [edi + 4]
        mov    dword ptr [esi + 8], eax
        test   eax, eax
        jne    loc_1_8EEE
        mov    dword ptr [ebp - 0x10], 3
        jmp    loc_1_902C
loc_1_8EEE:
        movzx  edx, word ptr [edi + 6]
        movzx  eax, word ptr [edi + 4]
        lea    ecx, [esi + 0xc]
        sub    edx, eax
        inc    edx
        mov    dword ptr [ecx], edx
        mov    al, byte ptr [edi + 0xb]
        mov    byte ptr [esi + 0x18], al
loc_1_8F04:
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0xc]
        mov    byte ptr [esi + 0x1a], 1
        mov    bl, byte ptr [ebp + 0x14]
        mov    dword ptr [esi + 0x10], eax
        mov    dword ptr [esi + 0x14], edx
        mov    byte ptr [esi + 0x19], bl
        mov    ax, word ptr [ecx]
        mov    word ptr [ebp - 4], ax
        mov    word ptr [ebp - 2], ax
        lea    eax, [ebp - 2]
        push   eax
        push   dword ptr [ebp + 0x14]
        push   edi
        push   esi
        call   sub_1_8D65
        add    esp, 0x10
        mov    dword ptr [ebp - 0x10], eax
        test   eax, eax
        je     loc_1_903D
        mov    ax, word ptr [ebp - 4]
        cmp    word ptr [ebp - 2], ax
        jae    loc_1_9018
        cmp    word ptr [ebp - 2], 0
        je     loc_1_9018
        mov    ax, word ptr [edi + 0xc]
        mov    cx, word ptr [edi + 4]
        mov    dx, word ptr [edi + 6]
        mov    word ptr [ebp - 0xc], ax
        mov    ax, 0x8000
        mov    word ptr [ebp - 0xa], cx
        test   word ptr [ebp - 2], ax
        mov    word ptr [ebp - 8], dx
        jne    loc_1_8F85
loc_1_8F7C:
        shr    ax, 1
        test   word ptr [ebp - 2], ax
        je     loc_1_8F7C
loc_1_8F85:
        xor    bx, bx
        mov    word ptr [ebp - 2], ax
        cmp    word ptr [ebp - 4], bx
        je     loc_1_8FFA
loc_1_8F92:
        mov    ax, word ptr [ebp - 2]
        movzx  ecx, bx
        movzx  edx, ax
        mov    word ptr [ebp - 6], ax
        add    ecx, edx
        movzx  eax, word ptr [ebp - 4]
        cmp    ecx, eax
        jle    loc_1_8FB5
        mov    ax, word ptr [ebp - 4]
        sub    ax, bx
        mov    word ptr [ebp - 6], ax
loc_1_8FB5:
        mov    ax, word ptr [edi + 4]
        lea    ecx, [ebp - 6]
        add    ax, word ptr [ebp - 6]
        push   ecx
        dec    ax
        push   dword ptr [ebp + 0x14]
        push   edi
        mov    word ptr [edi + 6], ax
        push   esi
        call   sub_1_8D65
        add    esp, 0x10
        mov    dword ptr [ebp - 0x10], eax
        test   eax, eax
        jne    loc_1_8FFA
        mov    ax, word ptr [ebp - 6]
        add    word ptr [edi + 4], ax
        mov    ax, word ptr [ebp - 2]
        add    bx, word ptr [ebp - 6]
        dec    ax
        not    ax
        mov    word ptr [edi + 0xc], ax
        cmp    bx, word ptr [ebp - 4]
        jb     loc_1_8F92
loc_1_8FFA:
        mov    ax, word ptr [ebp - 0xc]
        cmp    dword ptr [ebp - 0x10], 0
        mov    cx, word ptr [ebp - 0xa]
        mov    dx, word ptr [ebp - 8]
        mov    word ptr [edi + 0xc], ax
        mov    word ptr [edi + 4], cx
        mov    word ptr [edi + 6], dx
        je     loc_1_903D
loc_1_9018:
        push   esi
        call   sub_1_4245
        add    esp, 4
        test   eax, eax
        je     loc_1_902C
        mov    dword ptr [ebp - 0x10], 0x19
loc_1_902C:
        push   esi
        call   sub_1_254F
        add    esp, 4
        mov    eax, dword ptr [ebp - 0x10]
        jmp    loc_1_90CE
loc_1_903D:
        cmp    dword ptr [ebp - 0x18], 0
        je     loc_1_904E
        push   esi
        call   sub_1_254F
        add    esp, 4
        jmp    loc_1_90C5
loc_1_904E:
        cmp    dword ptr [ebp + 8], 0        ; -> obj1:0x192C
        jne    loc_1_90A1
        test   byte ptr [ebp + 0x17], 0x80
        jne    loc_1_907F
        mov    ecx, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ecx]
        mov    eax, ecx
        and    eax, 0x200000
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 0xffe00000
        add    eax, 0x400000
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 0x14]
        mov    dword ptr [ecx], eax
loc_1_907F:
        mov    ecx, dword ptr [ebp - 0x14]
        mov    ecx, dword ptr [ecx]
        mov    eax, ecx
        and    eax, 0x20000000
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 0xe0000000
        add    eax, 0x40000000
        or     eax, ecx
        mov    ecx, dword ptr [ebp - 0x14]
        mov    dword ptr [ecx], eax
loc_1_90A1:
        push   4
        mov    eax, dword ptr [ebp + 0xc]
        push   esi
        add    eax, 0x24
        push   eax
        call   sub_1_2D57
        add    esp, 0xc
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x1c
        push   0
        push   esi
        push   eax
        call   sub_1_2D57
        add    esp, 0xc
loc_1_90C5:
        xor    eax, eax
        jmp    loc_1_90CE
loc_1_90C9:
        mov    eax, 0x19
loc_1_90CE:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_90D5:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        sub    esp, 0x1c
        mov    ecx, dword ptr [eax + 0x18]
        push   ebx
        push   esi
        mov    dword ptr [ebp - 0xc], ecx
        push   edi
        mov    esi, dword ptr [0xa]          ; -> loc_1_199E
        add    ecx, 8
        mov    dword ptr [ebp - 8], ecx
        mov    edx, dword ptr [ecx]
        test   edx, 0x11000000
        je     loc_1_9108
        mov    eax, 0x1d
        jmp    loc_1_93C3
loc_1_9108:
        test   edx, 0x600000
        je     loc_1_911A
        mov    eax, 0x1e
        jmp    loc_1_93C3
loc_1_911A:
        mov    eax, dword ptr [0xa]          ; -> loc_1_199E
        cmp    byte ptr [eax + 4], 0
        je     loc_1_93BE
        mov    cl, byte ptr [eax + 8]
        test   cl, cl
        je     loc_1_9153
        mov    di, word ptr [eax + 6]
        mov    ax, word ptr [eax + 2]
        cmp    di, ax
        je     loc_1_93BE
        test   ax, ax
        je     loc_1_93BE
        test   di, di
        je     loc_1_93BE
loc_1_9153:
        xor    di, di
        xor    eax, eax
loc_1_9158:
        movzx  ebx, di
        inc    di
        mov    dword ptr [ebp + ebx*4 - 0x14], eax
        cmp    di, 2
        jb     loc_1_9158
        test   edx, 0x8000000
        je     loc_1_9302
        test   edx, 0x20000000
        je     loc_1_92F8
        test   cl, cl
        je     loc_1_918F
        test   edx, 0x40000000
        je     loc_1_92F8
loc_1_918F:
        xor    bx, bx
loc_1_9192:
        movzx  eax, bx
        lea    edi, [eax*4]
        mov    al, byte ptr [esi + eax*4 + 4]
        test   al, al
        je     loc_1_925F
        movzx  eax, al
        push   eax
        call   sub_1_27A5
        mov    word ptr [ebp - 2], ax
        add    esp, 4
        lea    ecx, [ebp + edi - 0x14]
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [ebp - 0x18], 1
        mov    eax, dword ptr [eax + 0x24]
        mov    dword ptr [ecx], eax
        test   eax, eax
        je     loc_1_9233
loc_1_91CF:
        mov    eax, dword ptr [ecx]
        cmp    byte ptr [eax + 0x1a], 1
        jne    loc_1_9228
        cmp    dword ptr [eax + 0x10], 0     ; -> obj1:0x192C
        jne    loc_1_9228
        mov    dword ptr [ebp - 0x18], 0
        cmp    word ptr [edi + esi + 2], 0
        je     loc_1_91FB
        movzx  edx, word ptr [edi + esi + 2]
        cmp    dword ptr [eax + 8], edx
        je     loc_1_9210
        jmp    loc_1_9228
loc_1_91FB:
        movzx  eax, word ptr [ebp - 2]
        mov    edx, dword ptr [ecx]
        mov    dword ptr [ebp - 0x1c], eax
        mov    eax, dword ptr [edx + 8]
        sub    edx, edx
        div    dword ptr [ebp - 0x1c]
        test   edx, edx
        jne    loc_1_9228
loc_1_9210:
        mov    eax, dword ptr [ecx]
        movzx  edx, byte ptr [edi + esi + 4]
        cmp    edx, dword ptr [eax + 0xc]
        je     loc_1_9233
        cmp    word ptr [edi + esi + 2], 0
        jne    loc_1_92B3
loc_1_9228:
        mov    eax, dword ptr [ecx]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [ecx], eax
        test   eax, eax
        jne    loc_1_91CF
loc_1_9233:
        cmp    dword ptr [ebp - 0x18], 0
        jne    loc_1_92EA
        mov    eax, dword ptr [ecx]
        test   eax, eax
        je     loc_1_92F1
        inc    bx
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_6288
        add    esp, 8
        cmp    bx, 2
        jb     loc_1_9192
loc_1_925F:
        xor    cx, cx
loc_1_9262:
        movzx  eax, cx
        lea    edx, [eax*4]
        cmp    byte ptr [edx + esi + 4], 0
        je     loc_1_92AC
        mov    eax, dword ptr [ebp - 8]
        mov    ebx, dword ptr [ebp + edx - 0x14]
        mov    eax, dword ptr [eax]
        mov    edi, eax
        and    edi, 0x200000
        cmp    edi, 1
        sbb    edi, edi
        inc    cx
        and    edi, 0xffe00000
        add    edi, 0x400000
        or     edi, eax
        mov    eax, dword ptr [ebp - 8]
        cmp    cx, 2
        mov    dword ptr [eax], edi
        mov    edi, dword ptr [ebx + 8]
        mov    word ptr [edx + esi + 2], di
        jb     loc_1_9262
loc_1_92AC:
        xor    eax, eax
        jmp    loc_1_93C3
loc_1_92B3:
        mov    edi, 0xa
loc_1_92B8:
        xor    si, si
        test   bx, bx
        je     loc_1_93BA
loc_1_92C4:
        mov    eax, dword ptr [ebp - 8]
        test   byte ptr [eax + 3], 8
        movzx  eax, si
        push   dword ptr [ebp + eax*4 - 0x14]
        je     loc_1_939E
        push   0                             ; -> obj1:0x192C
        call   sub_1_6288
        add    esp, 8
        jmp    loc_1_93AF
loc_1_92EA:
        mov    edi, 0x19
        jmp    loc_1_92B8
loc_1_92F1:
        mov    edi, 3
        jmp    loc_1_92B8
loc_1_92F8:
        mov    eax, 0x20
        jmp    loc_1_93C3
loc_1_9302:
        xor    bx, bx
loc_1_9305:
        movzx  eax, bx
        lea    edx, [esi + eax*4]
        cmp    byte ptr [edx + 4], 0
        je     loc_1_925F
        mov    edi, 0                        ; -> loc_1_1A58
        xor    eax, eax
        mov    ecx, 9
        rep stosd dword ptr es:[edi], eax
        mov    word ptr [0], 1               ; -> loc_1_1A58
        mov    word ptr [2], 0xc             ; -> loc_1_1A5A
        mov    ax, word ptr [edx + 2]
        mov    word ptr [4], ax              ; -> loc_1_1A5C
        movzx  ax, byte ptr [edx + 4]
        add    ax, word ptr [edx + 2]
        dec    ax
        mov    word ptr [6], ax              ; -> loc_1_1A5E
        mov    cl, byte ptr [esi + 0xa]
        mov    byte ptr [0xb], cl            ; -> obj1:0x1A63
        movzx  eax, byte ptr [edx + 5]
        or     eax, 0x10000000
        push   eax
        push   0                             ; -> loc_1_1A58
        push   dword ptr [ebp - 0xc]
        push   dword ptr [ebp + 8]
        call   sub_1_8E41
        add    esp, 0x10
        mov    edi, eax
        test   edi, edi
        jne    loc_1_92B8
        movzx  edx, bx
        inc    bx
        mov    eax, dword ptr [ebp + 8]
        cmp    bx, 2
        mov    ecx, dword ptr [eax + 0x1c]
        mov    dword ptr [ebp + edx*4 - 0x14], ecx
        jb     loc_1_9305
        jmp    loc_1_925F
loc_1_939E:
        call   sub_1_62B7
        add    esp, 4
        test   eax, eax
        je     loc_1_93AF
        mov    edi, 0x19
loc_1_93AF:
        inc    si
        cmp    si, bx
        jb     loc_1_92C4
loc_1_93BA:
        mov    eax, edi
        jmp    loc_1_93C3
loc_1_93BE:
        mov    eax, 0x1c
loc_1_93C3:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_93CA:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        mov    ecx, dword ptr [0xa]          ; -> loc_1_199E
        sub    esp, 8
        cmp    byte ptr [ecx + 4], 0
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 0x18]
        jne    loc_1_93F0
        mov    eax, 0x1c
        jmp    loc_1_9547
loc_1_93F0:
        test   byte ptr [esi + 0xb], 0x11
        je     loc_1_9400
        mov    eax, 0x1d
        jmp    loc_1_9547
loc_1_9400:
        xor    ax, ax
        xor    ecx, ecx
loc_1_9405:
        movzx  edx, ax
        inc    ax
        mov    dword ptr [ebp + edx*4 - 8], ecx
        cmp    ax, 2
        jb     loc_1_9405
        xor    dx, dx
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0x1c]
loc_1_941D:
        movzx  eax, dx
        shl    eax, 2
        mov    edi, dword ptr [0xa]          ; -> loc_1_199E
        add    edi, eax
        cmp    byte ptr [edi + 4], 0
        jne    loc_1_9448
        test   ecx, ecx
        je     loc_1_947A
loc_1_9435:
        cmp    dword ptr [ecx + 0x14], esi
        jne    loc_1_9440
        cmp    byte ptr [ecx + 0x1a], 1
        je     loc_1_94AD
loc_1_9440:
        mov    ecx, dword ptr [ecx]
        test   ecx, ecx
        jne    loc_1_9435
        jmp    loc_1_947A
loc_1_9448:
        test   ecx, ecx
        je     loc_1_947A
        lea    eax, [ebp + eax - 8]
loc_1_9450:
        cmp    dword ptr [eax], 0
        jne    loc_1_947A
        cmp    dword ptr [ecx + 0x14], esi
        jne    loc_1_9474
        cmp    byte ptr [ecx + 0x1a], 1
        jne    loc_1_9474
        movzx  ebx, word ptr [edi + 2]
        cmp    ebx, dword ptr [ecx + 8]
        jne    loc_1_9474
        movzx  ebx, byte ptr [edi + 4]
        cmp    ebx, dword ptr [ecx + 0xc]
        jne    loc_1_9474
        mov    dword ptr [eax], ecx
loc_1_9474:
        mov    ecx, dword ptr [ecx]
        test   ecx, ecx
        jne    loc_1_9450
loc_1_947A:
        inc    dx
        cmp    dx, 2
        jb     loc_1_941D
        xor    dx, dx
loc_1_9485:
        movzx  eax, dx
        lea    edi, [eax*4]
        mov    eax, dword ptr [ebp + eax*4 - 8]
        test   eax, eax
        jne    loc_1_94B7
        mov    eax, dword ptr [0xa]          ; -> loc_1_199E
        cmp    byte ptr [edi + eax + 4], 0
        je     loc_1_94CE
        mov    eax, 0x1c
        jmp    loc_1_9547
loc_1_94AD:
        mov    eax, 0x1c
        jmp    loc_1_9547
loc_1_94B7:
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x10], ecx
        jne    loc_1_94FC
        mov    ecx, dword ptr [0xa]          ; -> loc_1_199E
        mov    al, byte ptr [eax + 0x19]
        cmp    byte ptr [edi + ecx + 5], al
        jne    loc_1_9503
loc_1_94CE:
        inc    dx
        cmp    dx, 2
        jb     loc_1_9485
        xor    di, di
        xor    ebx, ebx
loc_1_94DB:
        movzx  eax, di
        mov    eax, dword ptr [ebp + eax*4 - 8]
        test   eax, eax
        je     loc_1_9545
        test   byte ptr [esi + 0xb], 8
        push   eax
        je     loc_1_950A
        push   0                             ; -> obj1:0x192C
        call   sub_1_6288
        add    esp, 8
        jmp    loc_1_951B
loc_1_94FC:
        mov    eax, 0x19
        jmp    loc_1_9547
loc_1_9503:
        mov    eax, 2
        jmp    loc_1_9547
loc_1_950A:
        call   sub_1_62B7
        add    esp, 4
        test   eax, eax
        je     loc_1_951B
        mov    ebx, 0x19
loc_1_951B:
        test   ebx, ebx
        jne    loc_1_953D
        mov    ecx, dword ptr [esi + 8]
        mov    eax, ecx
        and    eax, 0x400000
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 0x200000
        sub    eax, 0x400001
        and    eax, ecx
        mov    dword ptr [esi + 8], eax
loc_1_953D:
        inc    di
        cmp    di, 2
        jb     loc_1_94DB
loc_1_9545:
        mov    eax, ebx
loc_1_9547:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_954E:
        mov    ecx, dword ptr [esp + 4]
        mov    al, byte ptr [ecx + 0x13]
        or     al, byte ptr [0]              ; -> loc_1_1BF0
        test   byte ptr [ecx + 0x17], 2
        je     loc_1_9569
        test   al, 0x71
        je     loc_1_9567
        or     al, 0x10
loc_1_9567:
        and    al, 0x9e
loc_1_9569:
        and    al, byte ptr [0]              ; -> loc_1_1BEC
        or     al, 0x80
        ret    
sub_1_9572:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        pushfd 
        pop    dword ptr [ebp - 4]
        cli    
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 0x13], 0
        mov    ecx, dword ptr [eax + 0x20]
        test   ecx, ecx
        je     loc_1_9598
loc_1_958B:
        mov    dl, byte ptr [ecx + 0x10]
        or     byte ptr [eax + 0x13], dl
        mov    ecx, dword ptr [ecx + 4]
        test   ecx, ecx
        jne    loc_1_958B
loc_1_9598:
        push   dword ptr [ebp - 4]
        popfd  
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_95A0:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        mov    ecx, esi
        call   sub_1_2967
        mov    di, ax
        lea    ecx, [esi + 0xc]
        mov    eax, dword ptr [ecx]
        mov    edx, dword ptr [eax + 0x18]
        mov    dword ptr [0x14], edx         ; -> sub_1_1980
        mov    eax, dword ptr [ecx]
        mov    edx, dword ptr [eax + 0x1c]
        mov    byte ptr [1], 0x8d            ; -> obj1:0x196D
        mov    dword ptr [0xa], edx          ; -> obj1:0x1976
        mov    eax, dword ptr [ecx]
        mov    al, byte ptr [eax + 0xc]
        mov    byte ptr [0], al              ; -> obj1:0x196C
        mov    cl, byte ptr [esi + 0x12]
        mov    byte ptr [2], cl              ; -> obj1:0x196E
        call   sub_1_2438
        test   eax, eax
        jne    loc_1_9616
        mov    byte ptr [1], 0x8e            ; -> obj1:0x196D
        test   di, 0x80
        je     loc_1_960A
        push   esi
        call   sub_1_954E
        add    esp, 4
        mov    byte ptr [3], al              ; -> obj1:0x196F
        jmp    loc_1_9611
loc_1_960A:
        mov    byte ptr [3], 0x80            ; -> obj1:0x196F
loc_1_9611:
        call   sub_1_2438
loc_1_9616:
        pop    edi
        pop    esi
        ret    
sub_1_9619:
        push   esi
        mov    esi, dword ptr [0]            ; -> obj1:0x191C
        test   esi, esi
        je     loc_1_9641
loc_1_9624:
        push   esi
        call   sub_1_9572
        add    esp, 4
        push   esi
        call   sub_1_95A0
        add    esp, 4
        test   eax, eax
        jne    loc_1_9643
        mov    esi, dword ptr [esi + 4]
        test   esi, esi
        jne    loc_1_9624
loc_1_9641:
        xor    eax, eax
loc_1_9643:
        pop    esi
        ret    
sub_1_9645:
        mov    ecx, dword ptr [0]            ; -> obj1:0x191C
        test   ecx, ecx
        je     loc_1_9665
        mov    ax, word ptr [esp + 4]
loc_1_9654:
        cmp    word ptr [ecx + 0x10], ax
        je     loc_1_9661
        mov    ecx, dword ptr [ecx + 4]
        test   ecx, ecx
        jne    loc_1_9654
loc_1_9661:
        test   ecx, ecx
        jne    loc_1_966C
loc_1_9665:
        mov    eax, 0xb
        jmp    loc_1_9674
loc_1_966C:
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax], ecx
        xor    eax, eax
loc_1_9674:
        ret    
sub_1_9675:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        lea    eax, [ebp - 4]
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_269E
        add    esp, 8
        test   eax, eax
        jne    loc_1_96B8
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 0x10]
        test   ecx, ecx
        je     loc_1_96AA
        mov    al, byte ptr [ebp + 0xc]
loc_1_969B:
        cmp    byte ptr [ecx + 0x12], al
        je     loc_1_96A6
        mov    ecx, dword ptr [ecx]
        test   ecx, ecx
        jne    loc_1_969B
loc_1_96A6:
        test   ecx, ecx
        jne    loc_1_96B1
loc_1_96AA:
        mov    eax, 0xb
        jmp    loc_1_96B8
loc_1_96B1:
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [eax], ecx
        xor    eax, eax
loc_1_96B8:
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_96BC:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        push   edi
        mov    edi, dword ptr [esp + 0x14]
        push   ebp
        push   edi
        push   esi
        call   sub_1_2635
        add    esp, 8
        mov    ebx, eax
        cmp    word ptr [0], 0x22            ; -> loc_1_1994
        jne    loc_1_976D
        test   ebx, ebx
        je     loc_1_96F0
        mov    eax, 0xb
        jmp    loc_1_97B3
loc_1_96F0:
        push   0x12
        call   sub_1_2540
        add    esp, 4
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_1_970A
        mov    eax, 0x20
        jmp    loc_1_97B3
loc_1_970A:
        mov    dword ptr [ebx + 8], esi
        mov    dword ptr [ebx + 0xc], edi
        mov    eax, dword ptr [0xa]          ; -> loc_1_199E
        mov    ax, word ptr [eax + 2]
        mov    word ptr [ebx + 0x10], ax
        mov    cl, byte ptr [edi + 0x13]
        movzx  ebp, al
        movzx  edx, cl
        not    edx
        test   ebp, edx
        je     loc_1_974D
        or     cl, al
        push   edi
        mov    byte ptr [edi + 0x13], cl
        call   sub_1_95A0
        add    esp, 4
        mov    ebp, eax
        test   ebp, ebp
        je     loc_1_974D
        push   ebx
        call   sub_1_254F
        add    esp, 4
        mov    eax, ebp
        jmp    loc_1_97B3
loc_1_974D:
        push   0
        add    esi, 0x18
        push   ebx
        add    edi, 0x20
        push   esi
        call   sub_1_2D7C
        add    esp, 0xc
        push   4
        push   ebx
        push   edi
        call   sub_1_2D7C
        add    esp, 0xc
        jmp    loc_1_97B1
loc_1_976D:
        test   ebx, ebx
        jne    loc_1_9778
        mov    eax, 0xb
        jmp    loc_1_97B3
loc_1_9778:
        push   0
        add    esi, 0x18
        push   ebx
        push   esi
        call   sub_1_2E0B
        add    esp, 0xc
        lea    eax, [edi + 0x20]
        push   4
        push   ebx
        push   eax
        call   sub_1_2E0B
        add    esp, 0xc
        push   ebx
        call   sub_1_254F
        add    esp, 4
        push   edi
        call   sub_1_9572
        add    esp, 4
        push   edi
        call   sub_1_95A0
        add    esp, 4
loc_1_97B1:
        xor    eax, eax
loc_1_97B3:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_97B8:
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [ecx + 0xc]
        mov    dl, byte ptr [eax + 0xf]
        mov    eax, dword ptr [0xa]          ; -> loc_1_199E
        mov    byte ptr [eax + 2], dl
        mov    eax, dword ptr [0xa]          ; -> loc_1_199E
        mov    cl, byte ptr [ecx + 0x12]
        mov    byte ptr [eax + 3], cl
        xor    eax, eax
        ret    
loc_1_97D8:
        sub    esp, 4
        mov    ecx, dword ptr [0xa]          ; -> loc_1_199E
        lea    eax, [esp]
        mov    dl, byte ptr [ecx + 3]
        push   eax
        push   edx
        mov    al, byte ptr [ecx + 2]
        push   eax
        call   sub_1_9675
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_980B
        mov    ecx, dword ptr [esp]
        mov    edx, dword ptr [0xa]          ; -> loc_1_199E
        mov    cx, word ptr [ecx + 0x10]
        mov    word ptr [edx], cx
loc_1_980B:
        add    esp, 4
        ret    
loc_1_980F:
        push   esi
        mov    eax, dword ptr [0xa]          ; -> loc_1_199E
        cmp    word ptr [eax + 2], 0
        je     loc_1_9823
        mov    eax, 2
        jmp    loc_1_9874
loc_1_9823:
        push   0xa
        call   sub_1_2540
        add    esp, 4
        mov    esi, eax
        test   esi, esi
        jne    loc_1_983A
        mov    eax, 0x20
        jmp    loc_1_9874
loc_1_983A:
        mov    eax, dword ptr [esp + 0xc]
        mov    ecx, dword ptr [esp + 8]
        mov    word ptr [esi + 8], 0x10
        push   esi
        mov    dword ptr [esi], eax
        push   0x32
        mov    dword ptr [esi + 4], ecx
        push   0                             ; -> loc_1_5F1F
        call   sub_1_2566
        add    esp, 0xc
        test   eax, eax
        mov    eax, 0
        jne    loc_1_9874
        push   esi
        call   sub_1_254F
        add    esp, 4
        mov    eax, 0x19
loc_1_9874:
        pop    esi
        ret    
loc_1_9876:
        mov    eax, 0x15
        ret    
loc_1_987C:
        ret    
        db  0CCh, 0CCh, 0CCh
sub_1_9880:
        VxDCall  SHELL._SHELL_CancelAppyTimeEvent ; cd200f801700
        int3   
        int3   
sub_1_9888:
        VxDCall  SHELL._SHELL_CallAtAppyTime     ; cd200e801700
        int3   
        int3   
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x25  R-X
        db  001h, 000h, 000h, 000h
        dd  loc_2_0010               ; fixup
        db  000h, 001h, 000h, 000h, 000h, 000h, 015h, 000h
loc_2_0010:
        push   eax
        inc    ebx
        dec    ebp
        inc    ebx
        dec    ecx
        inc    ecx
        and    byte ptr [ebx + 0x61], al
        jb     0x7f
        and    byte ptr [ebx + 0x65], dl
        jb     0x96
        db  069h, 063h, 065h, 073h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x5446  R-X
sub_3_0000:
        mov    dword ptr [0], 1              ; -> obj5:0x0004
        push   0
        push   dword ptr [esp + 8]
        call   sub_3_5278
        add    esp, 8
        ret    
sub_3_0019:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        VxDCall  CONFIGMG._CONFIGMG_Get_Version  ; cd2000003300
        mov    word ptr [ebp - 2], ax
        cmp    word ptr [ebp - 2], 0x400
        jb     loc_3_00C4
        mov    eax, dword ptr [ebp + 0xc]
        test   eax, eax
        je     loc_3_00C4
        cmp    eax, 1
        je     loc_3_0052
        cmp    eax, 2
        je     loc_3_00C4
        mov    eax, 1
        jmp    loc_3_00C9
loc_3_0052:
        cmp    dword ptr [0], 0              ; -> obj5:0x0004
        jne    loc_3_0062
        mov    eax, 0x21
        jmp    loc_3_00C9
loc_3_0062:
        mov    ecx, dword ptr [0]            ; -> obj1:0x1918
        mov    eax, dword ptr [ebp + 8]
        test   ecx, ecx
        je     loc_3_00B9
loc_3_006F:
        mov    edx, dword ptr [ecx + 0x10]
        test   edx, edx
        je     loc_3_009A
loc_3_0076:
        cmp    dword ptr [edx + 8], eax
        je     loc_3_00A2
        mov    esi, dword ptr [edx + 0x2c]
        test   esi, esi
        jne    loc_3_0089
        mov    esi, dword ptr [edx + 0x24]
        test   esi, esi
        je     loc_3_0094
loc_3_0089:
        cmp    dword ptr [esi + 0x2c], eax
        je     loc_3_00AD
        mov    esi, dword ptr [esi]
        test   esi, esi
        jne    loc_3_0089
loc_3_0094:
        mov    edx, dword ptr [edx]
        test   edx, edx
        jne    loc_3_0076
loc_3_009A:
        mov    ecx, dword ptr [ecx]
        test   ecx, ecx
        jne    loc_3_006F
        jmp    loc_3_00B9
loc_3_00A2:
        push   edx
        call   sub_3_00FD
        add    esp, 4
        jmp    loc_3_00C9
loc_3_00AD:
        push   eax
        push   edx
        call   sub_3_0139
        add    esp, 8
        jmp    loc_3_00C9
loc_3_00B9:
        push   eax
        call   sub_3_01A2
        add    esp, 4
        jmp    loc_3_00C9
loc_3_00C4:
        mov    eax, 0x13
loc_3_00C9:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
sub_3_00D0:
        mov    eax, dword ptr [esp + 4]
        push   esi
        push   0
        push   0
        mov    esi, dword ptr [eax + 8]
        push   esi
        call   sub_3_5288
        add    esp, 0xc
        test   eax, eax
        jne    loc_3_00FB
        push   3
        push   0
        push   0                             ; -> loc_3_1873
        push   esi
        call   sub_3_5280
        add    esp, 0x10
loc_3_00FB:
        pop    esi
        ret    
sub_3_00FD:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    byte ptr [esi + 0x30], 2
        jb     loc_3_011B
        push   3
        push   0
        push   0
        push   dword ptr [esi + 8]
        call   sub_3_5280
        add    esp, 0x10
        jmp    loc_3_0137
loc_3_011B:
        push   esi
        call   sub_3_00D0
        add    esp, 4
        test   eax, eax
        jne    loc_3_0137
        add    esi, 0x14
        mov    ecx, dword ptr [esi]
        or     cl, 2
        mov    dword ptr [esi], ecx
        and    cl, 0xfe
        mov    dword ptr [esi], ecx
loc_3_0137:
        pop    esi
        ret    
sub_3_0139:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x54
        push   esi
        lea    eax, [ebp - 4]
        push   edi
        lea    ecx, [ebp - 0x54]
        push   1
        xor    edi, edi
        push   eax
        mov    esi, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 4], 0x50
        push   ecx
        push   1
        push   0                             ; -> obj5:0x00C8
        push   edi
        push   esi
        call   sub_3_5298
        add    esp, 0x1c
        test   eax, eax
        jne    loc_3_019C
        push   edi
        lea    eax, [ebp - 0x54]
        push   2
        push   eax
        push   esi
        call   sub_3_5290
        add    esp, 0x10
        test   eax, eax
        jne    loc_3_019C
        push   edi
        push   edi
        push   esi
        call   sub_3_5288
        add    esp, 0xc
        test   eax, eax
        jne    loc_3_019C
        push   3
        push   edi
        push   edi
        push   esi
        call   sub_3_5280
        add    esp, 0x10
loc_3_019C:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_01A2:
        push   ebp
        mov    ecx, dword ptr [0]            ; -> obj1:0x1918
        mov    ebp, esp
        sub    esp, 0x58
        test   ecx, ecx
        push   ebx
        push   esi
        push   edi
        je     loc_3_01C3
loc_3_01B5:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [ecx + 4], eax
        je     loc_3_01DE
        mov    ecx, dword ptr [ecx]
        test   ecx, ecx
        jne    loc_3_01B5
loc_3_01C3:
        push   dword ptr [ebp + 8]
        call   sub_1_7B6A
loc_3_01CB:
        add    esp, 4
        mov    esi, eax
        test   esi, esi
        jne    loc_3_01E8
        mov    eax, 2
        jmp    loc_3_02A5
loc_3_01DE:
        mov    eax, 0x10
        jmp    loc_3_02A5
loc_3_01E8:
        mov    dword ptr [ebp - 8], 0x50
        push   1
        xor    edi, edi
        push   0                             ; -> loc_3_060B
        push   dword ptr [ebp + 8]
        call   sub_3_5288
        add    esp, 0xc
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_3_029A
        push   edi
        push   0                             ; -> loc_3_05C7
        push   dword ptr [ebp + 8]
        call   sub_3_52B0
        add    esp, 0xc
        mov    ebx, eax
        cmp    ebx, edi
        jne    loc_3_029A
        push   1
        lea    eax, [ebp - 8]
        push   eax
        lea    ecx, [ebp - 0x58]
        push   ecx
        push   1
        push   0                             ; -> obj5:0x00CC
        push   edi
        push   dword ptr [ebp + 8]
        call   sub_3_5298
        add    esp, 0x1c
        mov    ebx, eax
        cmp    ebx, edi
        jne    loc_3_029A
        push   edi
        lea    eax, [ebp - 0x58]
        push   2
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_3_5290
        add    esp, 0x10
        mov    ebx, eax
        cmp    ebx, edi
        jne    loc_3_029A
        push   edi
        lea    eax, [ebp - 4]
        push   edi
        push   edi
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_3_52A8
        add    esp, 0x14
        test   eax, eax
        jne    loc_3_0296
        cmp    dword ptr [ebp - 4], 4
        jne    loc_3_0296
        push   edi
        lea    eax, [esi + 0x28]
        push   8
        push   eax
        push   edi
        push   2
        push   dword ptr [ebp + 8]
        call   sub_3_52A0
        add    esp, 0x18
        mov    ebx, eax
        cmp    ebx, edi
        jne    loc_3_029A
loc_3_0296:
        xor    eax, eax
        jmp    loc_3_02A5
loc_3_029A:
        push   esi
        call   sub_1_6153
loc_3_02A0:
        add    esp, 4
        mov    eax, ebx
loc_3_02A5:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_02AC:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [0]            ; -> obj1:0x1918
        push   ebp
        test   esi, esi
        je     loc_3_02FC
loc_3_02BA:
        mov    edi, dword ptr [esi + 0x10]
        test   edi, edi
        je     loc_3_02E9
loc_3_02C1:
        xor    bl, bl
        mov    ebp, dword ptr [edi + 0x18]
        test   ebp, ebp
        je     loc_3_02DF
loc_3_02CA:
        cmp    bl, byte ptr [edi + 0x30]
        jae    loc_3_02DF
        push   ebp
        inc    bl
        call   sub_1_728F
loc_3_02D7:
        add    esp, 4
        add    ebp, 0x28
        jne    loc_3_02CA
loc_3_02DF:
        or     byte ptr [edi + 0x14], 1
        mov    edi, dword ptr [edi]
        test   edi, edi
        jne    loc_3_02C1
loc_3_02E9:
        push   esi
        call   sub_1_6153
loc_3_02EF:
        add    esp, 4
        mov    esi, dword ptr [0]            ; -> obj1:0x1918
        test   esi, esi
        jne    loc_3_02BA
loc_3_02FC:
        mov    eax, dword ptr [0]            ; -> obj1:0x1914
        test   eax, eax
        je     loc_3_0317
loc_3_0305:
        push   eax
        call   sub_1_6153
loc_3_030B:
        add    esp, 4
        mov    eax, dword ptr [0]            ; -> obj1:0x1914
        test   eax, eax
        jne    loc_3_0305
loc_3_0317:
        xor    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_3_031E:
        push   esi
        push   0
        push   0                             ; -> obj5:0x0008
        call   sub_3_1180
        add    esp, 8
        test   byte ptr [0], 8               ; -> obj5:0x0008
        jne    loc_3_03A5
        mov    eax, dword ptr [esp + 8]
        cmp    eax, 1
        je     loc_3_0351
        cmp    eax, 2
        je     loc_3_035D
        cmp    eax, 3
        je     loc_3_0378
        cmp    eax, 4
        je     loc_3_0384
        jmp    loc_3_03A5
loc_3_0351:
        push   0x12c
        push   0x1521
        jmp    loc_3_039D
loc_3_035D:
        mov    esi, 0x96
        push   esi
        push   0x70b
        call   sub_1_235D
loc_3_036D:
        add    esp, 8
        push   esi
        push   0x548
        jmp    loc_3_039D
loc_3_0378:
        push   0x12c
        push   0x1521
        jmp    loc_3_039D
loc_3_0384:
        mov    esi, 0x96
        push   esi
        push   0x548
        call   sub_1_235D
loc_3_0394:
        add    esp, 8
        push   esi
        push   0x70b
loc_3_039D:
        call   sub_1_235D
loc_3_03A2:
        add    esp, 8
loc_3_03A5:
        pop    esi
        ret    
sub_3_03A7:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        xor    ebx, ebx
        push   esi
        call   sub_1_5F01
loc_3_03B5:
        add    esp, 4
        mov    eax, dword ptr [esi + 8]
        test   eax, eax
        je     loc_3_03CF
        push   ebx
        push   eax
        call   sub_3_52B8
        add    esp, 8
        mov    byte ptr [esi + 0x48], bl
        mov    dword ptr [esi + 8], ebx
loc_3_03CF:
        push   esi
        call   sub_3_50F5
        add    esp, 4
        pop    esi
        pop    ebx
        ret    
sub_3_03DB:
        push   ebp
        and    byte ptr [esp + 0x13], 0x7f
        mov    ebp, esp
        sub    esp, 0x90
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [esi]
        mov    ax, word ptr [ebx + 0x10]
        mov    word ptr [ebp - 0xc], ax
        lea    eax, [ebp - 0xc]
        push   eax
        push   ebx
        call   sub_1_83A1
loc_3_0403:
        add    esp, 8
        test   eax, eax
        jne    loc_3_0561
        test   byte ptr [ebp - 0xa], 0x80
        je     loc_3_0561
        mov    eax, dword ptr [esi + 8]
        test   eax, 0x100000
        je     loc_3_0438
        cmp    dword ptr [ebp + 0x10], 0x11
        je     loc_3_0438
        push   0
        push   esi
        call   sub_1_73D8
loc_3_0430:
        add    esp, 8
        jmp    loc_3_0577
loc_3_0438:
        test   eax, 0x80000
        je     loc_3_044B
        push   dword ptr [ebp + 0xc]
        push   esi
        call   sub_3_10AE
        add    esp, 8
loc_3_044B:
        mov    ax, word ptr [ebx + 0x10]
        mov    ecx, ebx
        mov    word ptr [ebp - 2], 0
        mov    word ptr [ebp - 4], ax
        call   sub_1_5614
loc_3_0460:
        test   eax, eax
        jne    loc_3_0561
        cmp    byte ptr [ebx + 0x48], 0
        je     loc_3_0561
        lea    eax, [ebp - 4]
        push   eax
        push   ebx
        call   sub_1_3A1C
loc_3_047C:
        add    esp, 8
        test   eax, eax
        jne    loc_3_0561
        cmp    word ptr [ebp - 2], 0
        je     loc_3_0561
        push   0
        lea    eax, [ebp - 0x90]
        push   0
        push   eax
        push   ebx
        call   sub_3_394C
        add    esp, 0x10
        test   eax, eax
        jne    loc_3_0561
        lea    edi, [ebx + 0x48]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        lea    edi, [ecx + ebx + 0x45]
        cmp    byte ptr [edi], 0x5c
        jne    loc_3_0561
        lea    eax, [ebp - 0x90]
        lea    ecx, [ebx + 0x48]
        mov    edx, ecx
        mov    byte ptr [edi], 0
loc_3_04D7:
        mov    cl, byte ptr [eax]
        cmp    cl, byte ptr [edx]
        jne    loc_3_04F7
        or     cl, cl
        je     loc_3_04F3
        mov    cl, byte ptr [eax + 1]
        cmp    cl, byte ptr [edx + 1]
        jne    loc_3_04F7
        add    eax, 2
        add    edx, 2
        or     cl, cl
        jne    loc_3_04D7
loc_3_04F3:
        xor    eax, eax
        jmp    loc_3_04FC
loc_3_04F7:
        sbb    eax, eax
        sbb    eax, -1
loc_3_04FC:
        mov    byte ptr [edi], 0x5c
        test   eax, eax
        jne    loc_3_0561
        cmp    dword ptr [ebp + 0x10], 7
        je     loc_3_0515
        cmp    dword ptr [ebp + 0x10], 8
        je     loc_3_0515
        cmp    dword ptr [ebp + 0x10], 0x11
        jne    loc_3_054C
loc_3_0515:
        push   0
        lea    eax, [ebp - 0x10]
        push   eax
        push   dword ptr [ebp + 0xc]
        call   sub_3_52C0
        add    esp, 0xc
        test   eax, eax
        jne    loc_3_054C
        test   byte ptr [ebp - 0x10], 8
        je     loc_3_054C
        push   esi
        call   sub_1_728F
loc_3_0536:
        add    esp, 4
        push   ebx
        call   sub_1_6DE0
loc_3_053F:
        add    esp, 4
        test   eax, eax
        jne    loc_3_0577
        or     byte ptr [ebx + 0x14], 0x20
        jmp    loc_3_0577
loc_3_054C:
        push   dword ptr [ebp + 0xc]
        push   esi
        call   sub_3_0E3D
        add    esp, 8
        test   eax, eax
        mov    eax, 0
        je     loc_3_0579
loc_3_0561:
        push   ebx
        call   sub_3_03A7
        add    esp, 4
        and    byte ptr [ebx + 0x14], 0xfe
        push   ebx
        call   sub_1_82CB
loc_3_0574:
        add    esp, 4
loc_3_0577:
        xor    eax, eax
loc_3_0579:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_0580:
        mov    eax, dword ptr [esp + 4]
        push   ebx
        test   byte ptr [eax + 8], 4
        je     loc_3_0592
        mov    eax, 1
        jmp    loc_3_05BE
loc_3_0592:
        mov    eax, dword ptr [eax + 0x10]
        test   eax, eax
        je     loc_3_05BC
loc_3_0599:
        xor    dl, dl
        mov    ecx, dword ptr [eax + 0x18]
        test   ecx, ecx
        je     loc_3_05B6
        mov    bl, byte ptr [eax + 0x30]
loc_3_05A5:
        cmp    dl, bl
        jae    loc_3_05B6
        test   byte ptr [ecx + 0xa], 0x10
        jne    loc_3_05C0
        inc    dl
        add    ecx, 0x28
        jne    loc_3_05A5
loc_3_05B6:
        mov    eax, dword ptr [eax]
        test   eax, eax
        jne    loc_3_0599
loc_3_05BC:
        xor    eax, eax
loc_3_05BE:
        pop    ebx
        ret    
loc_3_05C0:
        mov    eax, 1
        jmp    loc_3_05BE
loc_3_05C7:
        push   ebp
        mov    ebp, esp
        push   dword ptr [esp + 0x10]
        push   0
        call   sub_3_1FF8
        add    esp, 8
        test   eax, eax
        je     loc_3_0604
        mov    eax, dword ptr [eax]
        test   byte ptr [eax + 0x17], 0x10
        je     loc_3_0604
        mov    eax, dword ptr [eax + 0xc]
        mov    ecx, dword ptr [eax + 0x2c]
        test   ecx, ecx
        je     loc_3_0604
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   ecx
        add    esp, 0x14
        jmp    loc_3_0609
loc_3_0604:
        mov    eax, 0x20
loc_3_0609:
        pop    ebp
        ret    
loc_3_060B:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0x10]
        push   edi
        call   sub_3_1F16
        add    esp, 4
        mov    ebx, eax
        test   ebx, ebx
        je     loc_3_0C8F
        cmp    dword ptr [ebp + 0x14], edi
        jne    loc_3_0997
        mov    eax, dword ptr [ebp + 8]
        cmp    eax, 1
        je     loc_3_0674
        cmp    eax, 2
        je     loc_3_06AC
        cmp    eax, 4
        je     loc_3_06D6
        cmp    eax, 5
        je     loc_3_06FC
        cmp    eax, 8
        je     loc_3_0892
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        push   edi
        push   dword ptr [ebp + 0xc]
        push   eax
        push   ebx
        call   sub_3_1C3C
        add    esp, 0x18
        jmp    loc_3_0C94
loc_3_0674:
        cmp    dword ptr [ebp + 0xc], 1
        je     loc_3_0691
        push   ebx
        call   sub_1_8C67
loc_3_0680:
        add    esp, 4
        test   eax, eax
        je     loc_3_0691
        mov    eax, 0x13
        jmp    loc_3_0C94
loc_3_0691:
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        push   edi
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        push   ebx
        call   sub_3_1C3C
        add    esp, 0x18
        jmp    loc_3_0C94
loc_3_06AC:
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        push   edi
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        push   ebx
        call   sub_3_1C3C
        add    esp, 0x18
        push   ebx
        call   sub_1_8CBE
loc_3_06C8:
        add    esp, 4
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        jmp    loc_3_0C94
loc_3_06D6:
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        push   edi
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        push   ebx
        call   sub_3_1C3C
        add    esp, 0x18
        push   ebx
        call   sub_1_6153
loc_3_06F2:
        add    esp, 4
        xor    eax, eax
        jmp    loc_3_0C94
loc_3_06FC:
        mov    esi, dword ptr [ebx + 0x10]
        test   esi, esi
        je     loc_3_0843
        xor    ebx, ebx
loc_3_0709:
        mov    eax, dword ptr [esi + 0x14]
        test   eax, 0x10000000
        jne    loc_3_084A
        test   al, 1
        je     loc_3_07F4
        and    al, 0xb9
        push   esi
        mov    dword ptr [esi + 0x14], eax
        mov    dword ptr [ebp - 4], ebx
        call   sub_3_03A7
        mov    cx, word ptr [esi + 0x10]
        add    esp, 4
        mov    word ptr [ebp - 0xc], cx
        lea    ecx, [ebp - 0xc]
        push   ecx
        push   esi
        call   sub_1_83A1
loc_3_0742:
        add    esp, 8
        test   eax, eax
        jne    loc_3_0839
        test   byte ptr [ebp - 0xa], 0x80
        je     loc_3_0839
        test   byte ptr [esi + 0x14], 8
        je     loc_3_0839
        mov    dword ptr [ebp - 0x10], 0x13
        mov    ecx, esi
        call   sub_1_5614
loc_3_076F:
        test   eax, eax
        jne    loc_3_086B
        push   dword ptr [esi + 0x18]
        call   sub_1_6F26
loc_3_077F:
        add    esp, 4
        test   eax, eax
        jne    loc_3_086B
        lea    eax, [ebp - 4]
        push   eax
        push   edi
        push   esi
        call   sub_3_3F1C
        add    esp, 0xc
        mov    dword ptr [ebp - 0x10], eax
        cmp    eax, ebx
        jne    loc_3_086B
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [esi + 0x18]
        mov    dword ptr [esi + 8], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [esi + 0x14]
        and    al, 0xfe
        mov    dword ptr [esi + 0x14], eax
        cmp    dword ptr [ebp - 4], ebx
        je     loc_3_07C3
        test   al, 2
        je     loc_3_07CD
loc_3_07C3:
        push   edi
        push   esi
        call   sub_3_4F85
        add    esp, 8
loc_3_07CD:
        cmp    dword ptr [ebp - 4], ebx
        je     loc_3_0839
        test   byte ptr [esi + 0x14], 2
        je     loc_3_0839
        push   esi
        call   sub_3_00D0
        add    esp, 4
        test   eax, eax
        jne    loc_3_0862
        and    byte ptr [esi + 0x14], 0xfe
        push   esi
        call   sub_1_5EC7
loc_3_07EF:
        add    esp, 4
        jmp    loc_3_0839
loc_3_07F4:
        mov    ax, word ptr [esi + 0x10]
        mov    word ptr [ebp - 0xc], ax
        lea    eax, [ebp - 0xc]
        push   eax
        push   esi
        call   sub_1_83A1
loc_3_0806:
        add    esp, 8
        test   eax, eax
        jne    loc_3_0839
        test   byte ptr [ebp - 0xa], 0x80
        je     loc_3_0839
        lea    eax, [esi + 0x48]
        cmp    byte ptr [eax], bl
        je     loc_3_0839
        push   ebx
        lea    ecx, [ebp - 4]
        push   edi
        push   eax
        push   ecx
        call   sub_3_52E0
        add    esp, 0x10
        test   eax, eax
        jne    loc_3_0839
        push   ebx
        push   dword ptr [ebp - 4]
        call   sub_3_52B8
        add    esp, 8
loc_3_0839:
        mov    esi, dword ptr [esi]
        test   esi, esi
        jne    loc_3_0709
loc_3_0843:
        xor    eax, eax
        jmp    loc_3_0C94
loc_3_084A:
        push   dword ptr [ebp + 0x18]
        push   edi
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        push   esi
        call   sub_3_1C6D
        add    esp, 0x14
        jmp    loc_3_0C94
loc_3_0862:
        and    byte ptr [esi + 0x14], 0xfd
        jmp    loc_3_0C94
loc_3_086B:
        xor    edi, edi
        cmp    dword ptr [ebp - 4], edi
        je     loc_3_0881
        push   edi
        push   dword ptr [ebp - 4]
        call   sub_3_52B8
        add    esp, 8
        mov    dword ptr [ebp - 4], edi
loc_3_0881:
        push   esi
        call   sub_1_5EC7
loc_3_0887:
        add    esp, 4
        mov    eax, dword ptr [ebp - 0x10]
        jmp    loc_3_0C94
loc_3_0892:
        push   0
        push   0                             ; -> obj5:0x0008
        call   sub_3_1180
        add    esp, 8
        test   byte ptr [0], 0x20            ; -> obj5:0x0008
        jne    loc_3_0C8F
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0x7fffffff
        cmp    eax, 5
        je     loc_3_08E5
        cmp    eax, 7
        jb     loc_3_08CA
        cmp    eax, 8
        jbe    loc_3_0918
        cmp    eax, 0x11
        je     loc_3_0918
loc_3_08CA:
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        push   edi
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        push   ebx
        call   sub_3_1C3C
        add    esp, 0x18
        jmp    loc_3_0C94
loc_3_08E5:
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        push   edi
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        push   ebx
        call   sub_3_1C3C
        add    esp, 0x18
        push   ebx
        call   sub_3_0580
        add    esp, 4
        test   eax, eax
        jne    loc_3_0911
        push   ebx
        call   sub_1_8CBE
loc_3_090E:
        add    esp, 4
loc_3_0911:
        xor    eax, eax
        jmp    loc_3_0C94
loc_3_0918:
        push   ebx
        call   sub_3_0580
        add    esp, 4
        test   eax, eax
        je     loc_3_0932
        test   byte ptr [ebp + 0xc], 1
        je     loc_3_0932
        xor    eax, eax
        jmp    loc_3_0C94
loc_3_0932:
        test   byte ptr [ebx + 8], 0x10
        je     loc_3_0941
        push   ebx
        call   sub_1_8CBE
loc_3_093E:
        add    esp, 4
loc_3_0941:
        push   ebx
        call   sub_1_8C67
loc_3_0947:
        add    esp, 4
        mov    esi, dword ptr [ebx + 0x10]
        test   esi, esi
        je     loc_3_097C
loc_3_0951:
        cmp    dword ptr [esi + 8], 0
        jne    loc_3_0976
        mov    eax, dword ptr [esi + 0x18]
        cmp    dword ptr [eax + 4], 0
        jne    loc_3_0976
        push   esi
        call   sub_3_03A7
        add    esp, 4
        and    byte ptr [esi + 0x14], 0xfe
        push   esi
        call   sub_1_82CB
loc_3_0973:
        add    esp, 4
loc_3_0976:
        mov    esi, dword ptr [esi]
        test   esi, esi
        jne    loc_3_0951
loc_3_097C:
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        push   edi
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        push   ebx
        call   sub_3_1C3C
        add    esp, 0x18
        jmp    loc_3_0C8B
loc_3_0997:
        push   0
        lea    ecx, [ebp - 0x14]
        push   dword ptr [ebp + 0x14]
        push   ecx
        call   sub_3_52D8
        add    esp, 0xc
        cmp    dword ptr [ebp - 0x14], edi
        je     loc_3_09C8
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        push   edi
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        push   ebx
        call   sub_3_1C3C
        add    esp, 0x18
        jmp    loc_3_0C94
loc_3_09C8:
        push   dword ptr [ebp + 0x14]
        push   ebx
        call   sub_3_1F31
        add    esp, 8
        mov    esi, eax
        test   esi, esi
        jne    loc_3_0A0D
        push   dword ptr [ebp + 0x14]
        push   ebx
        call   sub_3_1FF8
        add    esp, 8
        test   eax, eax
        je     loc_3_0A0D
        mov    ecx, dword ptr [eax]
        test   byte ptr [ecx + 0x17], 0x10
        je     loc_3_0A0D
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        push   edi
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        push   eax
        call   sub_3_1D6C
        add    esp, 0x18
        jmp    loc_3_0C94
loc_3_0A0D:
        xor    edi, edi
        test   esi, esi
        je     loc_3_0A16
        mov    edi, dword ptr [esi + 0x18]
loc_3_0A16:
        mov    eax, dword ptr [ebp + 8]
        test   eax, eax
        je     loc_3_0A47
        cmp    eax, 1
        je     loc_3_0A58
        cmp    eax, 2
        je     loc_3_0AAE
        cmp    eax, 4
        je     loc_3_0AAE
        cmp    eax, 6
        je     loc_3_0B12
        cmp    eax, 8
        je     loc_3_0B39
        jmp    loc_3_0C8F
loc_3_0A47:
        push   dword ptr [ebp + 0x14]
        push   edi
        call   sub_3_0CDF
        add    esp, 8
        jmp    loc_3_0C94
loc_3_0A58:
        test   edi, edi
        je     loc_3_0A6D
        push   dword ptr [ebp + 0x14]
        push   edi
        call   sub_3_0E3D
        add    esp, 8
        jmp    loc_3_0C94
loc_3_0A6D:
        push   0
        push   dword ptr [ebp + 0x14]
        push   ebx
        call   sub_3_5124
        add    esp, 0xc
        test   eax, eax
        je     loc_3_0C8F
        cmp    byte ptr [eax + 0x48], 0
        jne    loc_3_0C8F
        mov    ecx, dword ptr [eax + 0x14]
        test   cl, 0x10
        jne    loc_3_0C8F
        or     cl, 0x10
        push   2
        mov    dword ptr [eax + 0x14], ecx
        call   sub_3_031E
        add    esp, 4
        jmp    loc_3_0C8B
loc_3_0AAE:
        test   edi, edi
        je     loc_3_0AC6
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp + 0x14]
        push   edi
        call   sub_3_104E
        add    esp, 0xc
        jmp    loc_3_0C94
loc_3_0AC6:
        push   0
        push   dword ptr [ebp + 0x14]
        push   ebx
        call   sub_3_5124
        add    esp, 0xc
        mov    esi, eax
        test   esi, esi
        je     loc_3_0B0B
        cmp    dword ptr [ebp + 8], 4
        jne    loc_3_0B0B
        cmp    dword ptr [esi + 8], 0
        je     loc_3_0AFB
        cmp    byte ptr [esi + 0x48], 0
        jne    loc_3_0AFB
        test   byte ptr [esi + 0x14], 2
        je     loc_3_0AFB
        push   esi
        call   sub_3_03A7
        add    esp, 4
loc_3_0AFB:
        push   dword ptr [ebp + 0x14]
        push   ebx
        call   sub_3_518B
        and    byte ptr [esi + 0x14], 0xef
        add    esp, 8
loc_3_0B0B:
        xor    eax, eax
        jmp    loc_3_0C94
loc_3_0B12:
        test   edi, edi
        je     loc_3_0B26
        push   edi
        call   sub_3_4106
        add    esp, 4
        xor    eax, eax
        jmp    loc_3_0C94
loc_3_0B26:
        push   dword ptr [ebp + 0x14]
        push   ebx
        call   sub_3_51B2
        add    esp, 8
        xor    eax, eax
        jmp    loc_3_0C94
loc_3_0B39:
        test   esi, esi
        je     loc_3_0C8F
        push   0
        push   0                             ; -> obj5:0x0008
        call   sub_3_1180
        add    esp, 8
        test   byte ptr [0], 0x20            ; -> obj5:0x0008
        jne    loc_3_0C8F
        mov    ecx, dword ptr [esi + 8]
        cmp    dword ptr [edi + 4], ecx
        jne    loc_3_0C8F
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0x7fffffff
        dec    eax
        cmp    eax, 0x10
        ja     loc_3_0C8F
        jmp    dword ptr [eax*4]             ; -> obj3:0x0C9B
loc_3_0B82:
        test   byte ptr [edi + 0xa], 2
        jne    loc_3_0C8F
        test   esi, esi
        je     loc_3_0C8B
        push   0
        push   ecx
        call   sub_3_52D0
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        je     loc_3_0BB9
        push   0
        push   dword ptr [ebx + 4]
        call   sub_3_52C8
        add    esp, 8
        mov    eax, edi
        jmp    loc_3_0C94
loc_3_0BB9:
        push   esi
        call   sub_3_03A7
        add    esp, 4
        jmp    loc_3_0C8B
loc_3_0BC7:
        test   byte ptr [edi + 0xa], 0x10
        je     loc_3_0BD7
        push   1
        push   edi
        call   sub_1_73D8
loc_3_0BD5:
        jmp    loc_3_0BE0
loc_3_0BD7:
        push   dword ptr [ebp + 0x14]
        push   edi
        call   sub_3_10AE
loc_3_0BE0:
        add    esp, 8
        xor    eax, eax
        jmp    loc_3_0C94
loc_3_0BEA:
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 0x14]
        push   edi
        call   sub_3_03DB
        add    esp, 0xc
        jmp    loc_3_0C94
loc_3_0BFE:
        lea    ecx, [edi + 8]
        mov    eax, dword ptr [ecx]
        and    eax, 0xffefffff
        mov    dword ptr [ecx], eax
        or     eax, 0x20000
        mov    dword ptr [ecx], eax
        test   eax, 0x80000
        jne    loc_3_0C29
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 0x14]
        push   edi
        call   sub_3_03DB
        add    esp, 0xc
        jmp    loc_3_0C94
loc_3_0C29:
        xor    eax, eax
        jmp    loc_3_0C94
loc_3_0C2D:
        xor    eax, eax
        lea    esi, [edi + 8]
        mov    ecx, dword ptr [esi]
        or     ecx, 0x20000
        test   ecx, 0x80000
        mov    dword ptr [esi], ecx
        jne    loc_3_0C5B
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 0x14]
        push   edi
        and    ecx, 0xffefffff
        mov    dword ptr [esi], ecx
        call   sub_3_03DB
        add    esp, 0xc
loc_3_0C5B:
        test   eax, eax
        jne    loc_3_0C94
        or     byte ptr [esi + 2], 0x10
        jmp    loc_3_0C94
loc_3_0C65:
        lea    ecx, [edi + 8]
        mov    eax, dword ptr [ecx]
        and    eax, 0xffefffff
        mov    dword ptr [ecx], eax
        or     eax, 0x20000
        mov    dword ptr [ecx], eax
        test   eax, 0x80000
        je     loc_3_0C8B
        push   dword ptr [ebp + 0x14]
        push   edi
        call   sub_3_10AE
        add    esp, 8
loc_3_0C8B:
        xor    eax, eax
        jmp    loc_3_0C94
loc_3_0C8F:
        mov    eax, 1
loc_3_0C94:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        ; ---- dispatch table (17 entries) ----
        dd  loc_3_0B82
        dd  loc_3_0C8F
        dd  loc_3_0C8F
        dd  loc_3_0C8F
        dd  loc_3_0BC7
        dd  loc_3_0C8F
        dd  loc_3_0BEA
        dd  loc_3_0BEA
        dd  loc_3_0BFE
        dd  loc_3_0C2D
        dd  loc_3_0BFE
        dd  loc_3_0C2D
        dd  loc_3_0BFE
        dd  loc_3_0C2D
        dd  loc_3_0C65
        dd  loc_3_0C65
        dd  loc_3_0BEA
sub_3_0CDF:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        test   esi, esi
        je     loc_3_0E34
        push   1
        lea    ecx, [ebp - 0xc]
        push   dword ptr [ebp + 0xc]
        push   ecx
        call   sub_3_5300
        add    esp, 0xc
        test   eax, eax
        jne    loc_3_0E34
        xor    edi, edi
loc_3_0D0E:
        mov    eax, dword ptr [ebp - 0xc]
        xor    ebx, ebx
        push   edi
        lea    ecx, [ebp - 8]
        push   ecx
        lea    edx, [ebp - 0x14]
        push   0x897c
        mov    dword ptr [ebp - 4], eax
        push   eax
        push   edx
        call   sub_3_52F8
        add    esp, 0x14
        test   eax, eax
        jne    loc_3_0E04
        push   edi
        lea    eax, [ebp - 8]
        push   eax
        lea    edx, [ebp - 4]
        push   edi
        push   dword ptr [ebp - 4]
        push   edx
        call   sub_3_52F8
        add    esp, 0x14
        test   eax, eax
        jne    loc_3_0DF4
loc_3_0D52:
        test   ebx, ebx
        jne    loc_3_0DF4
        mov    ecx, 0x40000000
        mov    eax, dword ptr [ebp - 8]
        cmp    eax, 1
        je     loc_3_0D7A
        cmp    eax, 2
        je     loc_3_0D95
        cmp    eax, 4
        je     loc_3_0DB3
        cmp    eax, 0x897c
        je     loc_3_0DC9
        jmp    loc_3_0DD7
loc_3_0D7A:
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ecx + 0x24]
        or     eax, 0x40000004
        push   eax
        push   dword ptr [ebp - 8]
        push   ecx
        push   esi
        call   sub_1_61B4
loc_3_0D90:
        add    esp, 0x10
        jmp    loc_3_0DD5
loc_3_0D95:
        mov    eax, dword ptr [ebp - 4]
        test   byte ptr [eax + 0x18], 0x40
        je     loc_3_0DA3
        mov    ecx, 0x40000008
loc_3_0DA3:
        push   ecx
        push   dword ptr [ebp - 8]
        push   eax
        push   esi
        call   sub_1_61B4
loc_3_0DAE:
        add    esp, 0x10
        jmp    loc_3_0DD5
loc_3_0DB3:
        push   0x40000020
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 4]
        push   esi
        call   sub_1_61B4
loc_3_0DC4:
        add    esp, 0x10
        jmp    loc_3_0DD5
loc_3_0DC9:
        push   dword ptr [ebp - 4]
        push   esi
        call   sub_1_6F75
loc_3_0DD2:
        add    esp, 8
loc_3_0DD5:
        mov    ebx, eax
loc_3_0DD7:
        push   edi
        lea    eax, [ebp - 8]
        push   eax
        lea    edx, [ebp - 4]
        push   edi
        push   dword ptr [ebp - 4]
        push   edx
        call   sub_3_52F8
        add    esp, 0x14
        test   eax, eax
        je     loc_3_0D52
loc_3_0DF4:
        push   0x20000000
        push   edi
        push   edi
        push   esi
        call   sub_1_61B4
loc_3_0E01:
        add    esp, 0x10
loc_3_0E04:
        mov    eax, dword ptr [ebp - 0xc]
        push   edi
        push   eax
        lea    ecx, [ebp - 0xc]
        push   ecx
        mov    dword ptr [ebp - 0x10], eax
        call   sub_3_52F0
        add    esp, 0xc
        mov    dword ptr [ebp - 4], eax
        test   ebx, ebx
        je     loc_3_0E2B
        push   edi
        push   dword ptr [ebp - 0x10]
        call   sub_3_52E8
        add    esp, 8
loc_3_0E2B:
        cmp    dword ptr [ebp - 4], edi
        je     loc_3_0D0E
loc_3_0E34:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_0E3D:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        test   edi, edi
        jne    loc_3_0E57
        mov    eax, 1
        jmp    loc_3_1047
loc_3_0E57:
        mov    esi, dword ptr [edi]
        mov    eax, dword ptr [ebp + 0xc]
        cmp    dword ptr [esi + 8], eax
        jne    loc_3_0E80
        mov    ecx, esi
        call   sub_1_5614
loc_3_0E68:
        test   eax, eax
        je     loc_3_0E80
        push   3
        call   sub_3_031E
        add    esp, 4
        mov    eax, 0x13
        jmp    loc_3_1047
loc_3_0E80:
        cmp    byte ptr [esi + 0x30], 2
        jb     loc_3_0E9E
        mov    eax, dword ptr [ebp + 0xc]
        cmp    dword ptr [esi + 8], eax
        jne    loc_3_0E9E
        push   esi
        call   sub_1_5EC7
loc_3_0E94:
        add    esp, 4
        xor    eax, eax
        jmp    loc_3_1047
loc_3_0E9E:
        push   2
        lea    ecx, [ebp - 0x14]
        push   dword ptr [ebp + 0xc]
        push   ecx
        call   sub_3_5300
        add    esp, 0xc
        test   eax, eax
        jne    loc_3_1022
        push   0
        lea    eax, [ebp - 8]
        push   eax
        lea    edx, [ebp - 0x10]
        push   0x897c
        push   dword ptr [ebp - 0x14]
        push   edx
        call   sub_3_52F8
        add    esp, 0x14
        test   eax, eax
        jne    loc_3_1022
        mov    eax, dword ptr [ebp - 0x14]
        xor    ebx, ebx
        push   ebx
        lea    ecx, [ebp - 8]
        push   ecx
        lea    edx, [ebp - 4]
        push   ebx
        mov    dword ptr [ebp - 4], eax
        push   eax
        push   edx
        call   sub_3_52F8
        add    esp, 0x14
        test   eax, eax
        jne    loc_3_0F71
loc_3_0EF8:
        test   ebx, ebx
        jne    loc_3_0F71
        xor    ecx, ecx
        test   byte ptr [esi + 0x14], 2
        je     loc_3_0F09
        mov    ecx, 0x80000000
loc_3_0F09:
        mov    eax, dword ptr [ebp - 8]
        cmp    eax, 1
        je     loc_3_0F1D
        cmp    eax, 2
        je     loc_3_0F2E
        cmp    eax, 4
        je     loc_3_0F41
        jmp    loc_3_0F56
loc_3_0F1D:
        mov    edx, dword ptr [ebp - 4]
        mov    eax, dword ptr [edx + 0x24]
        or     al, 4
        or     ecx, eax
        push   ecx
        push   dword ptr [ebp - 8]
        push   edx
        jmp    loc_3_0F4B
loc_3_0F2E:
        mov    eax, dword ptr [ebp - 4]
        test   byte ptr [eax + 0x18], 0x40
        je     loc_3_0F3A
        or     cl, 8
loc_3_0F3A:
        push   ecx
        push   dword ptr [ebp - 8]
        push   eax
        jmp    loc_3_0F4B
loc_3_0F41:
        or     cl, 0x20
        push   ecx
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 4]
loc_3_0F4B:
        push   edi
        call   sub_1_61B4
loc_3_0F51:
        add    esp, 0x10
        mov    ebx, eax
loc_3_0F56:
        push   0
        lea    eax, [ebp - 8]
        push   eax
        lea    edx, [ebp - 4]
        push   0
        push   dword ptr [ebp - 4]
        push   edx
        call   sub_3_52F8
        add    esp, 0x14
        test   eax, eax
        je     loc_3_0EF8
loc_3_0F71:
        test   byte ptr [esi + 0x14], 2
        jne    loc_3_0F9F
        test   ebx, ebx
        jne    loc_3_0F9F
        mov    ax, word ptr [esi + 0x10]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    word ptr [ecx], ax
        push   dword ptr [ebp - 0x10]
        push   edi
        call   sub_1_70BA
loc_3_0F8E:
        add    esp, 8
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_3_0F9F
        mov    eax, dword ptr [esi + 0x14]
        and    al, 0xfe
        mov    dword ptr [esi + 0x14], eax
loc_3_0F9F:
        mov    dword ptr [ebp - 0xc], 0
        test   ebx, ebx
        jne    loc_3_100F
        mov    eax, dword ptr [esi + 0x14]
        test   al, 0x10
        jne    loc_3_0FC0
        or     al, 0x10
        push   2
        mov    dword ptr [esi + 0x14], eax
        call   sub_3_031E
        add    esp, 4
loc_3_0FC0:
        push   dword ptr [ebp + 0xc]
        call   sub_3_1281
        add    esp, 4
        cmp    eax, -1
        je     loc_3_0FE1
        test   eax, eax
        je     loc_3_1029
        push   0
        push   eax
        call   sub_3_5308
        add    esp, 8
        jmp    loc_3_1029
loc_3_0FE1:
        mov    al, byte ptr [edi + 0x1e]
        cmp    al, 9
        jne    loc_3_0FF9
        push   0
        push   0x1e8480
        call   sub_3_5308
        add    esp, 8
        jmp    loc_3_1029
loc_3_0FF9:
        cmp    al, 4
        jne    loc_3_1029
        push   0
        call   sub_3_1212
        push   eax
        call   sub_3_5308
        add    esp, 8
        jmp    loc_3_1029
loc_3_100F:
        push   3
        call   sub_3_031E
        mov    dword ptr [ebp - 0xc], 0x13
        add    esp, 4
        jmp    loc_3_1029
loc_3_1022:
        mov    dword ptr [ebp - 0xc], 1
loc_3_1029:
        mov    eax, dword ptr [ebp + 0xc]
        cmp    dword ptr [esi + 8], eax
        jne    loc_3_103A
        push   esi
        call   sub_1_5EC7
loc_3_1037:
        add    esp, 4
loc_3_103A:
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_3_1044
        or     byte ptr [edi + 0xa], 8
loc_3_1044:
        mov    eax, dword ptr [ebp - 0xc]
loc_3_1047:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_104E:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        test   edi, edi
        je     loc_3_10A7
        mov    ebx, dword ptr [edi]
        push   1
        push   0x100000
        mov    esi, dword ptr [ebp + 0xc]
        push   esi
        call   sub_3_5310
        add    esp, 0xc
        push   esi
        push   edi
        call   sub_3_10AE
        add    esp, 8
        cmp    dword ptr [ebx + 8], esi
        jne    loc_3_1091
        or     byte ptr [ebx + 0x14], 1
        cmp    dword ptr [ebp + 0x10], 4
        jne    loc_3_1091
        xor    eax, eax
        mov    byte ptr [ebx + 0x48], al
        mov    dword ptr [ebx + 8], eax
loc_3_1091:
        cmp    dword ptr [edi + 4], esi
        jne    loc_3_10A7
        cmp    dword ptr [ebp + 0x10], 4
        jne    loc_3_10A7
        mov    dword ptr [edi + 4], 0
        and    byte ptr [edi + 0xa], 0xfd
loc_3_10A7:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
sub_3_10AE:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0xc]
        mov    ebx, dword ptr [esi]
        cmp    dword ptr [ebx + 8], edi
        jne    loc_3_10CE
        cmp    byte ptr [ebx + 0x30], 2
        jae    loc_3_1177
loc_3_10CE:
        and    byte ptr [esi + 0xa], 0xe7
        push   esi
        call   sub_1_728F
loc_3_10D8:
        add    esp, 4
        push   ebx
        call   sub_1_6DE0
loc_3_10E1:
        add    esp, 4
        test   eax, eax
        jne    loc_3_10EC
        or     byte ptr [ebx + 0x14], 0x20
loc_3_10EC:
        mov    eax, dword ptr [ebx + 0x40]
        test   eax, eax
        je     loc_3_110A
        cmp    dword ptr [ebx + 8], edi
        jne    loc_3_110A
        push   0
        push   eax
        call   sub_3_52B8
        mov    dword ptr [ebx + 0x40], 0
        add    esp, 8
loc_3_110A:
        push   2
        lea    eax, [ebp - 0xc]
        push   edi
        push   eax
        call   sub_3_5300
        add    esp, 0xc
        test   eax, eax
        jne    loc_3_1177
        mov    eax, dword ptr [ebp - 0xc]
        xor    edi, edi
        push   edi
        lea    ecx, [ebp - 8]
        push   ecx
        lea    edx, [ebp - 4]
        push   edi
        mov    dword ptr [ebp - 4], eax
        push   eax
        push   edx
        call   sub_3_52F8
        add    esp, 0x14
        test   eax, eax
        jne    loc_3_1177
loc_3_113C:
        mov    eax, dword ptr [ebp - 8]
        cmp    eax, 1
        jb     loc_3_115E
        cmp    eax, 2
        jbe    loc_3_114E
        cmp    eax, 4
        jne    loc_3_115E
loc_3_114E:
        push   edi
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 4]
        push   esi
        call   sub_1_6417
loc_3_115B:
        add    esp, 0x10
loc_3_115E:
        push   edi
        lea    eax, [ebp - 8]
        push   eax
        lea    edx, [ebp - 4]
        push   edi
        push   dword ptr [ebp - 4]
        push   edx
        call   sub_3_52F8
        add    esp, 0x14
        test   eax, eax
        je     loc_3_113C
loc_3_1177:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_1180:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        sub    esp, 8
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        test   edi, edi
        je     loc_3_1193
        mov    dword ptr [edi], eax
loc_3_1193:
        mov    esi, dword ptr [ebp + 0xc]
        xor    eax, eax
        test   esi, esi
        je     loc_3_119F
        mov    dword ptr [esi + 8], eax
loc_3_119F:
        lea    eax, [ebp - 4]
        push   eax
        push   0                             ; -> obj5:0x000C
        push   0x80000002
        call   sub_3_5328
        add    esp, 0xc
        test   eax, eax
        jne    loc_3_120C
        test   edi, edi
        je     loc_3_11DD
        mov    dword ptr [ebp - 8], 4
        lea    ecx, [ebp - 8]
        push   ecx
        push   edi
        push   0
        push   0
        push   0                             ; -> obj5:0x003C
        push   dword ptr [ebp - 4]
        call   sub_3_5320
        add    esp, 0x18
loc_3_11DD:
        test   esi, esi
        je     loc_3_1201
        mov    dword ptr [ebp - 8], 0xc
        lea    ecx, [ebp - 8]
        push   ecx
        push   esi
        push   0
        push   0
        push   0                             ; -> obj5:0x0044
        push   dword ptr [ebp - 4]
        call   sub_3_5320
        add    esp, 0x18
loc_3_1201:
        push   dword ptr [ebp - 4]
        call   sub_3_5318
        add    esp, 4
loc_3_120C:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_1212:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        mov    dword ptr [ebp - 4], 0x5dc
        lea    eax, [ebp - 8]
        push   eax
        push   0                             ; -> obj5:0x000C
        push   0x80000002
        call   sub_3_5328
        add    esp, 0xc
        test   eax, eax
        jne    loc_3_1267
        lea    eax, [ebp - 0xc]
        lea    ecx, [ebp - 4]
        mov    dword ptr [ebp - 0xc], 4
        push   eax
        push   ecx
        push   0
        push   0
        push   0                             ; -> obj5:0x0058
        push   dword ptr [ebp - 8]
        call   sub_3_5320
        add    esp, 0x18
        push   dword ptr [ebp - 8]
        call   sub_3_5318
        add    esp, 4
loc_3_1267:
        mov    eax, 0x2710
        cmp    dword ptr [ebp - 4], eax
        jbe    loc_3_1274
        mov    dword ptr [ebp - 4], eax
loc_3_1274:
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        imul   eax, eax, 0x3e8
        pop    ebp
        ret    
sub_3_1281:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   1
        lea    eax, [ebp - 8]
        push   eax
        lea    ecx, [ebp - 4]
        mov    dword ptr [ebp - 8], 4
        push   ecx
        push   0
        push   0                             ; -> obj5:0x0064
        push   0
        push   dword ptr [ebp + 8]
        call   sub_3_5298
        add    esp, 0x1c
        test   eax, eax
        jne    loc_3_12CE
        cmp    dword ptr [ebp - 8], 4
        jne    loc_3_12CE
        mov    eax, 0x2710
        cmp    dword ptr [ebp - 4], eax
        jbe    loc_3_12C3
        mov    dword ptr [ebp - 4], eax
loc_3_12C3:
        mov    eax, dword ptr [ebp - 4]
        imul   eax, eax, 0x3e8
        jmp    loc_3_12D3
loc_3_12CE:
        mov    eax, 0xffffffff
loc_3_12D3:
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_12D7:
        push   ebp
        xor    edx, edx
        mov    ebp, esp
        sub    esp, 0x58
        mov    word ptr [ebp - 2], dx
        xor    eax, eax
        push   ebx
        mov    ecx, 9
        push   esi
        push   edi
        lea    edi, [ebp - 0x48]
        rep stosd dword ptr es:[edi], eax
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [ebp - 0x46], 0x14
        mov    esi, dword ptr [eax + 0xc]
        mov    word ptr [ebp - 0x48], 1
        cmp    dword ptr [esi + 0x30], edx
        jne    loc_3_163D
        lea    eax, [ebp - 0x18]
        lea    ecx, [ebp - 8]
        push   eax
        push   ecx
        push   dword ptr [ebp + 8]
        call   sub_1_4380
loc_3_131D:
        add    esp, 0xc
        test   eax, eax
        je     loc_3_1332
        mov    dword ptr [ebp - 8], 0x1000
        mov    dword ptr [ebp - 0x18], 0xffffff
loc_3_1332:
        xor    edi, edi
        lea    ecx, [ebp - 0xc]
        push   edi
        push   dword ptr [esi + 4]
        push   ecx
        call   sub_3_52D8
        add    esp, 0xc
        lea    ecx, [ebp - 0x4c]
        push   edi
        push   edi
        push   edi
        push   ecx
        push   dword ptr [ebp - 0xc]
        call   sub_3_52A8
        add    esp, 0x14
        test   eax, eax
        jne    loc_3_1360
        cmp    dword ptr [ebp - 0x4c], 4
        je     loc_3_13AF
loc_3_1360:
        xor    edi, edi
        mov    eax, dword ptr [0]            ; -> obj5:0x00BC
        mov    dword ptr [ebp - 0xc], edi
        cmp    eax, edi
        je     loc_3_13AF
        cmp    eax, dword ptr [ebp - 0x18]
        jae    loc_3_13AF
        mov    ecx, dword ptr [0]            ; -> obj5:0x00C0
        cmp    dword ptr [ebp - 8], ecx
        jne    loc_3_13AF
        push   dword ptr [0]                 ; -> obj5:0x00C4
        push   ecx
        push   eax
        push   esi
        call   sub_1_4359
loc_3_138C:
        add    esp, 0x10
        lea    eax, [esi + 0x30]
        push   1
        push   4
        push   eax
        push   3
        push   0                             ; -> obj5:0x004C
        push   edi
        push   dword ptr [esi + 4]
        call   sub_3_5330
        add    esp, 0x1c
        jmp    loc_3_163D
loc_3_13AF:
        lea    eax, [ebp - 0x58]
        push   eax
        push   0                             ; -> obj5:0x0008
        call   sub_3_1180
        add    esp, 8
        test   byte ptr [0], 4               ; -> obj5:0x0008
        jne    loc_3_13F6
        cmp    dword ptr [ebp - 0x50], 0
        je     loc_3_13F6
        mov    eax, dword ptr [ebp - 0x50]
        mov    ecx, dword ptr [ebp - 0x58]
        mov    edx, dword ptr [ebp - 0x54]
        mov    dword ptr [ebp - 0x34], eax
        lea    eax, [ebp - 0x48]
        mov    dword ptr [ebp - 0x30], ecx
        push   eax
        mov    dword ptr [ebp - 0x2c], edx
        push   dword ptr [ebp - 0xc]
        push   esi
        call   sub_3_1662
        add    esp, 0xc
        jmp    loc_3_163D
loc_3_13F6:
        push   1
        lea    eax, [ebp - 0x24]
        push   eax
        lea    ecx, [ebp - 0x10]
        mov    dword ptr [ebp - 0x24], 4
        push   ecx
        push   3
        push   0                             ; -> obj5:0x004C
        push   0
        push   dword ptr [esi + 4]
        call   sub_3_5298
        add    esp, 0x1c
        test   eax, eax
        jne    loc_3_1446
        cmp    dword ptr [ebp - 0x24], 4
        jne    loc_3_1446
        cmp    dword ptr [ebp - 0x10], 0
        je     loc_3_1446
        mov    edi, 0                        ; -> obj5:0x0080
        mov    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [0], eax            ; -> obj5:0x0080
        mov    eax, dword ptr [ebp - 8]
        add    eax, dword ptr [ebp - 0x10]
        dec    eax
        mov    dword ptr [4], eax            ; -> obj5:0x0084
        jmp    loc_3_1452
loc_3_1446:
        mov    dword ptr [ebp - 0x10], 0
        mov    edi, 0xc                      ; -> obj5:0x008C
loc_3_1452:
        cmp    dword ptr [ebp - 0xc], 0
        jne    loc_3_15CA
        mov    word ptr [ebp - 0x1a], 0
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x10]
        mov    word ptr [ebp - 0x1c], ax
        cmp    dword ptr [edi], 0
        je     loc_3_15CA
loc_3_1476:
        mov    ebx, dword ptr [edi]
loc_3_1478:
        cmp    dword ptr [edi + 4], ebx
        jbe    loc_3_15BE
        cmp    ebx, dword ptr [ebp - 0x18]
        jae    loc_3_15BE
        test   ebx, ebx
        je     loc_3_15BE
        push   dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 8]
        add    eax, ebx
        dec    eax
        push   eax
        push   ebx
        push   1
        call   sub_3_210B
        add    esp, 0x10
        test   eax, eax
        jne    loc_3_15A8
        push   0
        push   dword ptr [ebp - 8]
        push   ebx
        call   sub_3_5340
        add    esp, 0xc
        cmp    eax, -1
        je     loc_3_15A0
        push   eax
        push   dword ptr [ebp - 8]
        push   ebx
        push   esi
        call   sub_1_4359
loc_3_14D1:
        add    esp, 0x10
        lea    eax, [ebp - 0x1c]
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_1_3A1C
loc_3_14E0:
        add    esp, 8
        mov    dword ptr [ebp - 0x14], eax
        test   eax, eax
        jne    loc_3_152E
        cmp    word ptr [ebp - 0x1a], 0
        je     loc_3_152E
        push   dword ptr [ebp + 8]
        call   sub_3_37AC
        mov    word ptr [ebp - 2], ax
        add    esp, 4
        mov    dword ptr [ebp - 0x20], 0
loc_3_1507:
        push   dword ptr [ebp + 8]
        call   sub_1_3806
loc_3_150F:
        add    esp, 4
        push   dword ptr [ebp + 8]
        call   sub_3_37AC
        add    esp, 4
        cmp    ax, word ptr [ebp - 2]
        jne    loc_3_152E
        inc    dword ptr [ebp - 0x20]
        cmp    dword ptr [ebp - 0x20], 8
        jl     loc_3_1507
        jmp    loc_3_1535
loc_3_152E:
        mov    dword ptr [ebp - 0x14], 0x19
loc_3_1535:
        push   0
        push   0
        push   0
        push   esi
        call   sub_1_4359
loc_3_1541:
        add    esp, 0x10
        cmp    dword ptr [ebp - 0x14], 0
        jne    loc_3_15A0
        mov    eax, dword ptr [ebp - 8]
        lea    ecx, [ebp - 0x48]
        push   ecx
        mov    dword ptr [ebp - 0x34], eax
        add    eax, ebx
        mov    dword ptr [ebp - 0x30], ebx
        push   dword ptr [ebp - 0xc]
        push   esi
        dec    eax
        mov    dword ptr [ebp - 0x2c], eax
        call   sub_3_1662
        add    esp, 0xc
        mov    dword ptr [ebp - 0x14], eax
        cmp    dword ptr [esi + 0x30], 0
        je     loc_3_15A0
        push   dword ptr [ebp + 8]
        call   sub_3_37AC
        add    esp, 4
        cmp    ax, word ptr [ebp - 2]
        je     loc_3_1644
        cmp    dword ptr [ebp - 0x14], 0
        je     loc_3_159A
        push   0
        push   dword ptr [ebp - 0x14]
        call   sub_3_52B8
        add    esp, 8
loc_3_159A:
        mov    ebx, dword ptr [edi + 4]
        sub    ebx, dword ptr [ebp - 8]
loc_3_15A0:
        add    ebx, dword ptr [ebp - 8]
        jmp    loc_3_1478
loc_3_15A8:
        mov    ebx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 8]
        add    ebx, eax
        dec    ecx
        not    ecx
        dec    ebx
        and    ebx, ecx
        cmp    eax, ebx
        jbe    loc_3_1478
loc_3_15BE:
        add    edi, 0xc
        cmp    dword ptr [edi], 0
        jne    loc_3_1476
loc_3_15CA:
        cmp    dword ptr [ebp - 0x10], 0
        je     loc_3_15F2
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    dword ptr [ebp - 0x34], eax
        mov    dword ptr [ebp - 0x30], ecx
        add    eax, ecx
        lea    ecx, [ebp - 0x48]
        dec    eax
        push   ecx
        push   dword ptr [ebp - 0xc]
        push   esi
        mov    dword ptr [ebp - 0x2c], eax
        call   sub_3_1662
        add    esp, 0xc
loc_3_15F2:
        mov    eax, dword ptr [ebp + 8]
        mov    edi, dword ptr [eax + 0xc]
        add    edi, 0x30
        cmp    dword ptr [edi], 0
        jne    loc_3_1623
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 0x18]
        lea    edx, [ebp - 0x48]
        mov    dword ptr [ebp - 0x34], eax
        push   edx
        mov    dword ptr [ebp - 0x2c], ecx
        push   dword ptr [ebp - 0xc]
        push   esi
        mov    dword ptr [ebp - 0x30], 0xc0000
        call   sub_3_1662
        add    esp, 0xc
loc_3_1623:
        xor    eax, eax
        cmp    dword ptr [edi], eax
        jne    loc_3_163D
        push   eax
        push   eax
        push   0                             ; -> obj5:0x00D4
        push   0                             ; -> obj5:0x00E0
        call   sub_3_5338
        add    esp, 0x10
loc_3_163D:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_3_1644:
        push   1
        lea    eax, [esi + 0x30]
        push   4
        push   eax
        push   3
        push   0                             ; -> obj5:0x004C
        push   0
        push   dword ptr [esi + 4]
        call   sub_3_5330
        add    esp, 0x1c
        jmp    loc_3_163D
sub_3_1662:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0x10]
        mov    dword ptr [ebp - 8], 1
        mov    dword ptr [esi + 0x10], 0xffffffff
        mov    eax, dword ptr [esi + 0x14]
        cmp    eax, 1
        jbe    loc_3_1695
loc_3_1684:
        cmp    dword ptr [ebp - 8], 0
        je     loc_3_1695
        shl    dword ptr [ebp - 8], 1
        shl    dword ptr [esi + 0x10], 1
        cmp    dword ptr [ebp - 8], eax
        jb     loc_3_1684
loc_3_1695:
        mov    edi, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 0xc], edi
        test   edi, edi
        jne    loc_3_16B7
        push   0
        lea    eax, [ebp - 0xc]
        push   0
        push   eax
        call   sub_3_5358
        add    esp, 0xc
        test   eax, eax
        jne    loc_3_17BE
loc_3_16B7:
        push   1
        lea    ecx, [ebp - 4]
        push   dword ptr [ebp - 0xc]
        push   0                             ; -> obj5:0x0070
        push   ecx
        call   sub_3_52E0
        add    esp, 0x10
        test   eax, eax
        jne    loc_3_17BE
        xor    ebx, ebx
        push   ebx
        mov    dword ptr [ebp - 8], ebx
        push   ebx
        push   1
        call   sub_4_0000
loc_3_16E3:
        add    esp, 4
        push   eax
        push   1
        push   0                             ; -> obj5:0x00EC
        push   ebx
        push   dword ptr [ebp - 4]
        call   sub_3_5330
        add    esp, 0x1c
        test   eax, eax
        jne    loc_3_171C
        push   ebx
        lea    eax, [ebp - 8]
        push   4
        push   eax
        push   3
        push   0                             ; -> obj5:0x00F8
        push   ebx
        push   dword ptr [ebp - 4]
        call   sub_3_5330
        add    esp, 0x1c
        test   eax, eax
        je     loc_3_1740
loc_3_171C:
        push   0
        push   dword ptr [ebp - 4]
        call   sub_3_52B8
        add    esp, 8
        lea    ecx, [ebp - 4]
        push   1
        push   dword ptr [ebp - 0xc]
        push   0
        push   ecx
        call   sub_3_52E0
        add    esp, 0x10
        test   eax, eax
        jne    loc_3_17BE
loc_3_1740:
        xor    ebx, ebx
        lea    ecx, [ebp - 0x10]
        push   ebx
        push   ebx
        push   dword ptr [ebp - 4]
        push   ecx
        call   sub_3_5350
        add    esp, 0x10
        test   eax, eax
        jne    loc_3_1787
        push   ebx
        lea    ecx, [ebp - 0x14]
        push   0x24
        push   esi
        push   1
        push   dword ptr [ebp - 0x10]
        push   ecx
        call   sub_3_5348
        add    esp, 0x18
        test   eax, eax
        jne    loc_3_1787
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        push   esi
        push   0                             ; -> loc_3_17C7
        push   dword ptr [ebp - 4]
        call   sub_3_5280
        add    esp, 0x10
        jmp    loc_3_178A
loc_3_1787:
        mov    esi, dword ptr [ebp + 8]
loc_3_178A:
        mov    eax, dword ptr [esi + 0x30]
        test   eax, eax
        jne    loc_3_17A0
        push   0
        push   dword ptr [ebp - 4]
        call   sub_3_52B8
        add    esp, 8
        jmp    loc_3_17BE
loc_3_17A0:
        test   edi, edi
        jne    loc_3_17B9
        mov    dword ptr [0], eax            ; -> obj5:0x00BC
        mov    eax, dword ptr [esi + 0x34]
        mov    dword ptr [0], eax            ; -> obj5:0x00C0
        mov    eax, dword ptr [esi + 0x38]
        mov    dword ptr [0], eax            ; -> obj5:0x00C4
loc_3_17B9:
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_3_17C0
loc_3_17BE:
        xor    eax, eax
loc_3_17C0:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_3_17C7:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0xc
        cmp    eax, 1
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0x14]
        je     loc_3_17F6
        cmp    eax, 2
        je     loc_3_185C
        cmp    eax, 3
        je     loc_3_186A
        cmp    eax, 4
        je     loc_3_185C
        mov    eax, 1
        jmp    loc_3_186C
loc_3_17F6:
        push   2
        lea    ecx, [ebp - 8]
        push   dword ptr [ebp + 0x10]
        push   ecx
        call   sub_3_5300
        add    esp, 0xc
        test   eax, eax
        jne    loc_3_186C
        mov    eax, dword ptr [ebp - 8]
        push   0
        lea    ecx, [ebp - 0xc]
        lea    edx, [ebp - 4]
        push   ecx
        mov    dword ptr [ebp - 4], eax
        push   1
        push   eax
        push   edx
        call   sub_3_52F8
        add    esp, 0x14
        test   eax, eax
        jne    loc_3_186A
        mov    eax, dword ptr [ebp - 4]
        push   0
        mov    edi, dword ptr [eax + 4]
        mov    ebx, dword ptr [eax + 8]
        sub    ebx, edi
        inc    ebx
        push   ebx
        push   edi
        call   sub_3_5340
        add    esp, 0xc
        cmp    eax, -1
        jne    loc_3_184E
        mov    eax, 0x13
        jmp    loc_3_186C
loc_3_184E:
        push   eax
        push   ebx
        push   edi
        push   esi
        call   sub_1_4359
loc_3_1857:
        add    esp, 0x10
        jmp    loc_3_186A
loc_3_185C:
        xor    eax, eax
        push   eax
        push   eax
        push   eax
        push   esi
        call   sub_1_4359
loc_3_1867:
        add    esp, 0x10
loc_3_186A:
        xor    eax, eax
loc_3_186C:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_3_1873:
        mov    eax, 1
        ret    
sub_3_1879:
        push   esi
        push   0
        mov    esi, dword ptr [esp + 0xc]
        mov    eax, dword ptr [esi + 0xc]
        or     byte ptr [esi + 0x14], 1
        push   dword ptr [eax + 4]
        call   sub_3_52C8
        add    esp, 8
        test   eax, eax
        je     loc_3_189F
        push   esi
        call   sub_1_5EC7
loc_3_189C:
        add    esp, 4
loc_3_189F:
        pop    esi
        ret    
loc_3_18A1:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        push   dword ptr [ebp + 0x10]
        call   sub_3_1F79
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        je     loc_3_1AE4
        mov    esi, dword ptr [ebp + 0x14]
        cmp    esi, dword ptr [ebp + 0x10]
        jne    loc_3_1974
        cmp    dword ptr [ebp + 8], 5
        jne    loc_3_1AE4
        xor    bl, bl
        mov    esi, dword ptr [edi + 0x18]
        test   esi, esi
        je     loc_3_1952
loc_3_18DE:
        cmp    byte ptr [edi + 0x30], bl
        jbe    loc_3_1952
        mov    dword ptr [ebp - 8], 0
        mov    ax, word ptr [edi + 0x10]
        mov    word ptr [ebp - 0x10], ax
        lea    eax, [ebp - 0x10]
        push   eax
        push   edi
        call   sub_1_83A1
loc_3_18FC:
        add    esp, 8
        test   eax, eax
        jne    loc_3_193E
        test   byte ptr [ebp - 0xe], 0x80
        je     loc_3_193E
        test   byte ptr [edi + 0x14], 8
        je     loc_3_193E
        push   esi
        call   sub_1_6F26
loc_3_1915:
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    loc_3_1959
        lea    eax, [ebp - 8]
        push   eax
        push   dword ptr [ebp + 0x10]
        push   esi
        call   sub_3_4058
        add    esp, 0xc
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        jne    loc_3_1959
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [esi + 4], eax
        jmp    loc_3_194B
loc_3_193E:
        push   0
        push   dword ptr [esi + 4]
        call   sub_3_52B8
        add    esp, 8
loc_3_194B:
        inc    bl
        add    esi, 0x28
        jne    loc_3_18DE
loc_3_1952:
        xor    eax, eax
        jmp    loc_3_1AE9
loc_3_1959:
        cmp    dword ptr [ebp - 8], 0
        je     loc_3_196C
        push   0
        push   dword ptr [ebp - 8]
        call   sub_3_52B8
        add    esp, 8
loc_3_196C:
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_3_1AE9
loc_3_1974:
        push   0
        lea    eax, [ebp - 0x14]
        push   esi
        push   eax
        call   sub_3_52D8
        add    esp, 0xc
        mov    eax, dword ptr [ebp + 0x10]
        cmp    eax, dword ptr [ebp - 0x14]
        jne    loc_3_1AE4
        push   esi
        push   edi
        call   sub_3_1FCF
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        jne    loc_3_19A9
        mov    eax, 0x13
        jmp    loc_3_1AE9
loc_3_19A9:
        mov    eax, dword ptr [ebp + 8]
        test   eax, eax
        je     loc_3_19CE
        cmp    eax, 1
        je     loc_3_19DD
        cmp    eax, 2
        je     loc_3_19EC
        cmp    eax, 4
        je     loc_3_19EC
        cmp    eax, 6
        je     loc_3_19FE
        cmp    eax, 8
        je     loc_3_1A0E
        jmp    loc_3_1AE4
loc_3_19CE:
        push   esi
        push   edi
        call   sub_3_0CDF
        add    esp, 8
        jmp    loc_3_1AE9
loc_3_19DD:
        push   esi
        push   edi
        call   sub_3_0E3D
        add    esp, 8
        jmp    loc_3_1AE9
loc_3_19EC:
        push   dword ptr [ebp + 8]
        push   esi
        push   edi
        call   sub_3_104E
        add    esp, 0xc
        jmp    loc_3_1AE9
loc_3_19FE:
        push   edi
        call   sub_3_4106
        add    esp, 4
        xor    eax, eax
        jmp    loc_3_1AE9
loc_3_1A0E:
        push   0
        push   0                             ; -> obj5:0x0008
        call   sub_3_1180
        add    esp, 8
        test   byte ptr [0], 0x20            ; -> obj5:0x0008
        jne    loc_3_1AE4
        mov    ecx, dword ptr [ebp + 0xc]
        mov    eax, ecx
        and    eax, 0x7fffffff
        sub    eax, 5
        cmp    eax, 0xc
        ja     loc_3_1AE4
        jmp    dword ptr [eax*4]             ; -> obj3:0x1AF0
loc_3_1A47:
        test   byte ptr [edi + 0xa], 0x10
        je     loc_3_1A57
        push   1
        push   edi
        call   sub_1_73D8
loc_3_1A55:
        jmp    loc_3_1A5E
loc_3_1A57:
        push   esi
        push   edi
        call   sub_3_10AE
loc_3_1A5E:
        add    esp, 8
        xor    eax, eax
        jmp    loc_3_1AE9
loc_3_1A68:
        push   ecx
        push   esi
        push   edi
        call   sub_3_03DB
        add    esp, 0xc
        jmp    loc_3_1AE9
loc_3_1A75:
        mov    eax, dword ptr [edi + 8]
        and    eax, 0xffefffff
        mov    dword ptr [edi + 8], eax
        test   eax, 0x80000
        jne    loc_3_1A94
        push   ecx
        push   esi
        push   edi
        call   sub_3_03DB
        add    esp, 0xc
        jmp    loc_3_1AE9
loc_3_1A94:
        xor    eax, eax
        jmp    loc_3_1AE9
loc_3_1A98:
        xor    eax, eax
        lea    ebx, [edi + 8]
        mov    edx, dword ptr [ebx]
        test   edx, 0x80000
        jne    loc_3_1ABA
        and    edx, 0xffefffff
        push   ecx
        push   esi
        mov    dword ptr [ebx], edx
        push   edi
        call   sub_3_03DB
        add    esp, 0xc
loc_3_1ABA:
        test   eax, eax
        jne    loc_3_1AE9
        or     byte ptr [ebx + 2], 0x10
        jmp    loc_3_1AE9
loc_3_1AC4:
        mov    eax, dword ptr [edi + 8]
        and    eax, 0xffefffff
        mov    dword ptr [edi + 8], eax
        test   eax, 0x80000
        je     loc_3_1AE0
        push   esi
        push   edi
        call   sub_3_10AE
        add    esp, 8
loc_3_1AE0:
        xor    eax, eax
        jmp    loc_3_1AE9
loc_3_1AE4:
        mov    eax, 1
loc_3_1AE9:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        ; ---- dispatch table (13 entries) ----
        dd  loc_3_1A47
        dd  loc_3_1AE4
        dd  loc_3_1A68
        dd  loc_3_1A68
        dd  loc_3_1A75
        dd  loc_3_1A98
        dd  loc_3_1A75
        dd  loc_3_1A98
        dd  loc_3_1AC4
        dd  loc_3_1AC4
        dd  loc_3_1AC4
        dd  loc_3_1AC4
        dd  loc_3_1A68
loc_3_1B24:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0x14
        cmp    eax, 1
        push   ebx
        push   esi
        push   edi
        jbe    loc_3_1B65
        cmp    eax, 2
        jb     loc_3_1B5B
        cmp    eax, 3
        jbe    loc_3_1C2A
        cmp    eax, 4
        je     loc_3_1C2E
        cmp    eax, 5
        je     loc_3_1C2A
        cmp    eax, 6
        je     loc_3_1B65
loc_3_1B5B:
        mov    esi, 1
        jmp    loc_3_1C33
loc_3_1B65:
        xor    esi, esi
        push   6
        push   dword ptr [ebp + 0x10]
        lea    ecx, [ebp - 8]
        push   ecx
        call   sub_3_5300
        add    esp, 0xc
        test   eax, eax
        jne    loc_3_1B9B
        push   esi
        lea    eax, [ebp - 0x14]
        push   eax
        lea    edx, [ebp - 0x10]
        push   4
        push   dword ptr [ebp - 8]
        push   edx
        call   sub_3_52F8
        add    esp, 0x14
        test   eax, eax
        je     loc_3_1BCB
        jmp    loc_3_1C2E
loc_3_1B9B:
        push   2
        lea    ecx, [ebp - 8]
        push   dword ptr [ebp + 0x10]
        push   ecx
        call   sub_3_5300
        add    esp, 0xc
        test   eax, eax
        jne    loc_3_1C2E
        push   0
        lea    eax, [ebp - 0x14]
        push   eax
        lea    edx, [ebp - 0x10]
        push   4
        push   dword ptr [ebp - 8]
        push   edx
        call   sub_3_52F8
        add    esp, 0x14
        test   eax, eax
        jne    loc_3_1C2E
loc_3_1BCB:
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp + 0x14]
        mov    bx, word ptr [eax + 2]
        mov    edi, dword ptr [ecx]
        test   edi, edi
        je     loc_3_1C33
loc_3_1BDB:
        mov    eax, dword ptr [edi + 0xc]
        push   0
        push   0
        lea    ecx, [ebp - 4]
        push   4
        mov    dword ptr [ebp - 4], eax
        push   eax
        push   ecx
        call   sub_3_52F8
        add    esp, 0x14
        test   eax, eax
        jne    loc_3_1C22
        cmp    dword ptr [ebp + 8], 0
        mov    eax, dword ptr [ebp - 4]
        jne    loc_3_1C1C
        mov    dword ptr [ebp - 0xc], 1
        movzx  edx, word ptr [eax + 4]
        mov    cl, bl
        shl    dword ptr [ebp - 0xc], cl
        test   dword ptr [ebp - 0xc], edx
        je     loc_3_1C2E
        mov    word ptr [eax + 2], bx
        jmp    loc_3_1C22
loc_3_1C1C:
        cmp    word ptr [eax + 2], bx
        jne    loc_3_1C2E
loc_3_1C22:
        mov    edi, dword ptr [edi]
        test   edi, edi
        jne    loc_3_1BDB
        jmp    loc_3_1C33
loc_3_1C2A:
        xor    esi, esi
        jmp    loc_3_1C33
loc_3_1C2E:
        mov    esi, 0x13
loc_3_1C33:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_1C3C:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        mov    ecx, dword ptr [eax + 0x28]
        test   ecx, ecx
        je     loc_3_1C66
        test   byte ptr [eax + 8], 0x20
        je     loc_3_1C66
        push   dword ptr [ebp + 0x1c]
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        call   ecx
        add    esp, 0x14
        jmp    loc_3_1C6B
loc_3_1C66:
        mov    eax, 1
loc_3_1C6B:
        pop    ebp
        ret    
sub_3_1C6D:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x28
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        lea    edi, [esi + 0x14]
        mov    eax, dword ptr [edi]
        test   al, 1
        je     loc_3_1CCE
        mov    cx, word ptr [esi + 0x10]
        and    al, 0xf9
        mov    word ptr [ebp - 8], cx
        mov    dword ptr [edi], eax
        lea    ecx, [ebp - 8]
        push   ecx
        push   esi
        call   sub_1_83A1
loc_3_1C98:
        add    esp, 8
        test   eax, eax
        jne    loc_3_1CCE
        test   byte ptr [ebp - 6], 0x80
        je     loc_3_1CCE
        test   byte ptr [edi], 8
        je     loc_3_1CCE
        mov    ecx, esi
        call   sub_1_5614
loc_3_1CB1:
        test   eax, eax
        jne    loc_3_1CC4
        push   dword ptr [esi + 0x18]
        call   sub_1_6F26
loc_3_1CBD:
        add    esp, 4
        test   eax, eax
        je     loc_3_1CCE
loc_3_1CC4:
        mov    eax, 0x13
        jmp    loc_3_1D65
loc_3_1CCE:
        push   dword ptr [ebp + 0x18]
        mov    edi, dword ptr [ebp + 0x14]
        push   edi
        push   edi
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [esi + 0xc]
        call   sub_3_1C3C
        add    esp, 0x18
        test   eax, eax
        jne    loc_3_1D65
        mov    bl, 1
        push   0
        push   edi
        lea    eax, [ebp - 0x28]
        push   eax
        call   sub_3_5368
        add    esp, 0xc
        test   eax, eax
        jne    loc_3_1D26
loc_3_1D00:
        movzx  eax, bl
        shl    eax, 2
        push   0
        push   dword ptr [ebp + eax - 0x2c]
        lea    edx, [ebp + eax - 0x28]
        push   edx
        call   sub_3_5360
        add    esp, 0xc
        test   eax, eax
        jne    loc_3_1D2D
        inc    bl
        cmp    bl, 8
        jb     loc_3_1D00
        jmp    loc_3_1D2D
loc_3_1D26:
        mov    dword ptr [ebp - 0x28], 0
loc_3_1D2D:
        push   ebx
        push   esi
        call   sub_1_574F
loc_3_1D34:
        add    esp, 8
        test   eax, eax
        je     loc_3_1D42
        mov    eax, 0x13
        jmp    loc_3_1D65
loc_3_1D42:
        xor    cl, cl
        mov    edi, dword ptr [esi + 0x18]
        test   edi, edi
        je     loc_3_1D63
        mov    dl, byte ptr [esi + 0x30]
loc_3_1D4E:
        cmp    cl, dl
        jae    loc_3_1D63
        movzx  eax, cl
        inc    cl
        mov    ebx, dword ptr [ebp + eax*4 - 0x28]
        mov    dword ptr [edi + 4], ebx
        add    edi, 0x28
        jne    loc_3_1D4E
loc_3_1D63:
        xor    eax, eax
loc_3_1D65:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_1D6C:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x20
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        push   dword ptr [ebp + 0x1c]
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        mov    esi, dword ptr [edi]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [esi + 0xc]
        call   sub_3_1C3C
        add    esp, 0x18
        test   eax, eax
        je     loc_3_1DA1
        cmp    eax, 1
        jne    loc_3_1F0F
loc_3_1DA1:
        mov    eax, dword ptr [ebp + 0xc]
        cmp    eax, 1
        je     loc_3_1DCE
        cmp    eax, 2
        je     loc_3_1ECB
        cmp    eax, 4
        je     loc_3_1EC4
        cmp    eax, 6
        je     loc_3_1F0B
        mov    edi, 1
        jmp    loc_3_1F0D
loc_3_1DCE:
        lea    eax, [ebp - 0x1a]
        lea    ecx, [ebp - 0x1b]
        push   eax
        lea    edx, [ebp - 0x1c]
        push   ecx
        push   edx
        push   edi
        call   sub_3_4C20
        add    esp, 0x10
        lea    ecx, [ebp - 2]
        lea    eax, [ebp - 3]
        lea    edx, [ebp - 1]
        push   ecx
        push   eax
        push   edx
        push   dword ptr [edi]
        call   sub_1_6E09
loc_3_1DF6:
        add    esp, 0x10
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        jne    loc_3_1E94
        cmp    byte ptr [ebp - 0x1c], 0
        jne    loc_3_1E10
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [ebp - 0x1c], al
loc_3_1E10:
        cmp    byte ptr [ebp - 0x1b], 0
        jne    loc_3_1E1C
        mov    al, byte ptr [ebp - 3]
        mov    byte ptr [ebp - 0x1b], al
loc_3_1E1C:
        cmp    byte ptr [ebp - 0x1a], 0
        jne    loc_3_1E28
        mov    al, byte ptr [ebp - 2]
        mov    byte ptr [ebp - 0x1a], al
loc_3_1E28:
        mov    ax, word ptr [esi + 0x10]
        xor    ebx, ebx
        mov    word ptr [ebp - 0x20], ax
        mov    byte ptr [ebp - 0x14], bl
        mov    word ptr [ebp - 0x1e], bx
        mov    byte ptr [ebp - 0x13], bl
        mov    byte ptr [ebp - 0x12], bl
        mov    byte ptr [ebp - 0x11], bl
        mov    byte ptr [ebp - 0x10], bl
        mov    byte ptr [ebp - 0xf], bl
        lea    eax, [ebp - 0x20]
        mov    dword ptr [ebp - 0xe], ebx
        mov    byte ptr [ebp - 0x19], 4
        mov    dword ptr [ebp - 0x18], 0xffffffff
        push   eax
        push   edi
        call   sub_1_70BA
loc_3_1E60:
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        cmp    eax, ebx
        jne    loc_3_1E6E
        and    byte ptr [esi + 0x14], 0xfe
loc_3_1E6E:
        mov    al, byte ptr [ebp - 0x1c]
        cmp    byte ptr [ebp - 1], al
        je     loc_3_1E94
        push   dword ptr [ebp + 0x1c]
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [esi + 0xc]
        call   sub_3_1C3C
        add    esp, 0x18
        test   eax, eax
        jne    loc_3_1F0F
loc_3_1E94:
        xor    edi, edi
        cmp    dword ptr [ebp - 8], edi
        jne    loc_3_1EB3
        mov    eax, dword ptr [esi + 0x14]
        test   al, 0x10
        jne    loc_3_1F0D
        or     al, 0x10
        push   2
        mov    dword ptr [esi + 0x14], eax
        call   sub_3_031E
        add    esp, 4
        jmp    loc_3_1F0D
loc_3_1EB3:
        push   3
        mov    edi, 0x13
        call   sub_3_031E
        add    esp, 4
        jmp    loc_3_1F0D
loc_3_1EC4:
        mov    dword ptr [edi + 4], 0
loc_3_1ECB:
        push   edi
        call   sub_1_728F
loc_3_1ED1:
        add    esp, 4
        push   esi
        call   sub_1_6DE0
loc_3_1EDA:
        add    esp, 4
        test   eax, eax
        jne    loc_3_1EE5
        or     byte ptr [esi + 0x14], 0x20
loc_3_1EE5:
        xor    cl, cl
        mov    eax, dword ptr [esi + 0x18]
        test   eax, eax
        je     loc_3_1F02
        mov    dl, byte ptr [esi + 0x30]
loc_3_1EF1:
        cmp    cl, dl
        jae    loc_3_1F02
        cmp    dword ptr [eax + 4], 0
        jne    loc_3_1F02
        inc    cl
        add    eax, 0x28
        jne    loc_3_1EF1
loc_3_1F02:
        cmp    byte ptr [esi + 0x30], cl
        ja     loc_3_1F0B
        or     byte ptr [esi + 0x14], 1
loc_3_1F0B:
        xor    edi, edi
loc_3_1F0D:
        mov    eax, edi
loc_3_1F0F:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_1F16:
        mov    eax, dword ptr [0]            ; -> obj1:0x1918
        test   eax, eax
        je     loc_3_1F2E
        mov    ecx, dword ptr [esp + 4]
loc_3_1F23:
        cmp    dword ptr [eax + 4], ecx
        je     loc_3_1F30
        mov    eax, dword ptr [eax]
        test   eax, eax
        jne    loc_3_1F23
loc_3_1F2E:
        xor    eax, eax
loc_3_1F30:
        ret    
sub_3_1F31:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        push   0
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0xc]
        lea    eax, [ebp - 4]
        push   dword ptr [esi + 4]
        push   edi
        push   eax
        call   sub_3_5370
        add    esp, 0x10
        test   eax, eax
        jne    loc_3_1F6C
        mov    eax, dword ptr [esi + 0x10]
        test   eax, eax
        je     loc_3_1F6C
loc_3_1F5C:
        cmp    dword ptr [eax + 8], edi
        jne    loc_3_1F66
        cmp    eax, dword ptr [ebp - 4]
        je     loc_3_1F74
loc_3_1F66:
        mov    eax, dword ptr [eax]
        test   eax, eax
        jne    loc_3_1F5C
loc_3_1F6C:
        xor    eax, eax
loc_3_1F6E:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
loc_3_1F74:
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_3_1F6E
sub_3_1F79:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    eax, [ebp - 4]
        push   0
        mov    esi, dword ptr [ebp + 8]
        push   esi
        push   esi
        push   eax
        call   sub_3_5370
        add    esp, 0x10
        test   eax, eax
        jne    loc_3_1FC3
        mov    eax, dword ptr [0]            ; -> obj1:0x1918
        test   eax, eax
        je     loc_3_1FC3
loc_3_1FA0:
        mov    ecx, dword ptr [eax + 0x10]
        test   ecx, ecx
        je     loc_3_1FBD
loc_3_1FA7:
        cmp    ecx, dword ptr [ebp - 4]
        jne    loc_3_1FB7
        cmp    dword ptr [ecx + 8], esi
        jne    loc_3_1FB7
        cmp    byte ptr [ecx + 0x30], 2
        jae    loc_3_1FCA
loc_3_1FB7:
        mov    ecx, dword ptr [ecx]
        test   ecx, ecx
        jne    loc_3_1FA7
loc_3_1FBD:
        mov    eax, dword ptr [eax]
        test   eax, eax
        jne    loc_3_1FA0
loc_3_1FC3:
        xor    eax, eax
loc_3_1FC5:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
loc_3_1FCA:
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_3_1FC5
sub_3_1FCF:
        push   ebx
        xor    dl, dl
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [ecx + 0x18]
        test   eax, eax
        je     loc_3_1FF4
        mov    bl, byte ptr [ecx + 0x30]
        mov    ecx, dword ptr [esp + 0xc]
loc_3_1FE4:
        cmp    dl, bl
        jae    loc_3_1FF4
        cmp    dword ptr [eax + 4], ecx
        je     loc_3_1FF6
        inc    dl
        add    eax, 0x28
        jne    loc_3_1FE4
loc_3_1FF4:
        xor    eax, eax
loc_3_1FF6:
        pop    ebx
        ret    
sub_3_1FF8:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [0]            ; -> obj1:0x1918
        test   edi, edi
        je     loc_3_2046
        mov    ecx, dword ptr [esp + 0x14]
        mov    esi, dword ptr [esp + 0x18]
loc_3_200E:
        test   ecx, ecx
        je     loc_3_2016
        cmp    edi, ecx
        jne    loc_3_2040
loc_3_2016:
        mov    ebp, dword ptr [edi + 0x10]
        test   ebp, ebp
        je     loc_3_2040
loc_3_201D:
        xor    bl, bl
        mov    eax, dword ptr [ebp + 0x18]
        test   eax, eax
        je     loc_3_2039
        mov    dl, byte ptr [ebp + 0x30]
loc_3_2029:
        cmp    bl, dl
        jae    loc_3_2039
        cmp    dword ptr [eax + 4], esi
        je     loc_3_2048
        inc    bl
        add    eax, 0x28
        jne    loc_3_2029
loc_3_2039:
        mov    ebp, dword ptr [ebp]
        test   ebp, ebp
        jne    loc_3_201D
loc_3_2040:
        mov    edi, dword ptr [edi]
        test   edi, edi
        jne    loc_3_200E
loc_3_2046:
        xor    eax, eax
loc_3_2048:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_3_204D:
        push   ebx
        mov    edx, dword ptr [0]            ; -> obj1:0x1918
        push   esi
        mov    ecx, edx
        push   edi
        mov    esi, 0xffffffff
        test   edx, edx
        je     loc_3_207E
loc_3_2061:
        mov    edi, dword ptr [ecx + 0x10]
        test   edi, edi
        je     loc_3_2078
loc_3_2068:
        movzx  eax, word ptr [edi + 0x10]
        cmp    eax, esi
        jle    loc_3_2072
        mov    esi, eax
loc_3_2072:
        mov    edi, dword ptr [edi]
        test   edi, edi
        jne    loc_3_2068
loc_3_2078:
        mov    ecx, dword ptr [ecx]
        test   ecx, ecx
        jne    loc_3_2061
loc_3_207E:
        inc    esi
        lea    eax, [esi*4]
        cmp    eax, dword ptr [esp + 0x14]
        jbe    loc_3_2094
        or     esi, 0x10000
        jmp    loc_3_2105
loc_3_2094:
        mov    ebx, dword ptr [esp + 0x10]
        test   esi, esi
        jle    loc_3_20A4
        mov    edi, ebx
        xor    eax, eax
        mov    ecx, esi
        rep stosd dword ptr es:[edi], eax
loc_3_20A4:
        test   edx, edx
        je     loc_3_2105
loc_3_20A8:
        mov    ecx, dword ptr [edx + 0x10]
        test   ecx, ecx
        je     loc_3_20FF
loc_3_20AF:
        mov    eax, dword ptr [ecx + 8]
        test   eax, eax
        jne    loc_3_20C0
        mov    eax, dword ptr [ecx + 0x18]
        mov    eax, dword ptr [eax + 4]
        test   eax, eax
        je     loc_3_20C7
loc_3_20C0:
        movzx  edi, word ptr [ecx + 0x10]
        mov    dword ptr [ebx + edi*4], eax
loc_3_20C7:
        cmp    byte ptr [ecx + 0x48], 0
        jne    loc_3_20F9
        test   byte ptr [ecx + 0x14], 2
        je     loc_3_20F9
        mov    eax, dword ptr [ecx + 0x2c]
        test   eax, eax
        jne    loc_3_20E1
        mov    eax, dword ptr [ecx + 0x24]
        test   eax, eax
        je     loc_3_20F9
loc_3_20E1:
        cmp    dword ptr [eax + 0x2c], 0
        jne    loc_3_20EF
        mov    eax, dword ptr [eax]
        test   eax, eax
        jne    loc_3_20E1
        jmp    loc_3_20F9
loc_3_20EF:
        movzx  edi, word ptr [ecx + 0x10]
        mov    eax, dword ptr [eax + 0x2c]
        mov    dword ptr [ebx + edi*4], eax
loc_3_20F9:
        mov    ecx, dword ptr [ecx]
        test   ecx, ecx
        jne    loc_3_20AF
loc_3_20FF:
        mov    edx, dword ptr [edx]
        test   edx, edx
        jne    loc_3_20A8
loc_3_2105:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_3_210B:
        push   ebp
        mov    eax, dword ptr [esp + 0x14]
        mov    ebp, esp
        sub    esp, 0x10
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        lea    edi, [esi + eax - 1]
        cmp    dword ptr [ebp + 0x10], edi
        jb     loc_3_217D
        push   0
        lea    ecx, [ebp - 0x10]
        push   dword ptr [ebp + 8]
        push   0
        push   4
        push   ecx
        call   sub_3_5388
        add    esp, 0x14
        test   eax, eax
        jne    loc_3_217D
        push   0
        lea    eax, [ebp - 0xc]
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        lea    edx, [ebp - 8]
        push   edx
        push   dword ptr [ebp - 0x10]
        call   sub_3_5380
        add    esp, 0x14
        test   eax, eax
        jne    loc_3_217D
loc_3_2159:
        cmp    dword ptr [ebp - 8], edi
        ja     loc_3_2188
        cmp    dword ptr [ebp - 4], esi
        jae    loc_3_218C
        push   0
        lea    eax, [ebp - 4]
        push   eax
        lea    ecx, [ebp - 8]
        push   ecx
        lea    edx, [ebp - 0xc]
        push   edx
        call   sub_3_5378
        add    esp, 0x10
        test   eax, eax
        je     loc_3_2159
loc_3_217D:
        mov    eax, 0xffffffff
loc_3_2182:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
loc_3_2188:
        xor    eax, eax
        jmp    loc_3_2182
loc_3_218C:
        cmp    dword ptr [ebp - 4], -1
        je     loc_3_217D
        mov    eax, dword ptr [ebp - 4]
        inc    eax
        jmp    loc_3_2182
sub_3_2198:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        sub    esp, 0x24
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        cmp    dword ptr [edi + 0x40], ecx
        jne    loc_3_221B
        push   ecx
        mov    eax, dword ptr [edi + 0xc]
        push   dword ptr [eax + 4]
        push   ecx
        lea    ecx, [ebp - 4]
        push   ecx
        call   sub_3_52E0
        add    esp, 0x10
        test   eax, eax
        je     loc_3_21CD
        mov    eax, 0x13
        jmp    loc_3_23AA
loc_3_21CD:
        push   2
        xor    esi, esi
        push   esi
        lea    ecx, [ebp - 0x18]
        push   dword ptr [ebp - 4]
        push   ecx
        call   sub_3_5350
        add    esp, 0x10
        test   eax, eax
        jne    loc_3_223C
        push   esi
        push   esi
        push   esi
        push   dword ptr [ebp - 4]
        call   sub_3_5280
        add    esp, 0x10
        test   eax, eax
        jne    loc_3_223C
        push   esi
        lea    ecx, [ebp - 0x1c]
        push   dword ptr [ebp - 4]
        push   ecx
        lea    edx, [ebp - 0x10]
        push   edx
        call   sub_3_5398
        add    esp, 0x10
        test   eax, eax
        jne    loc_3_223C
        test   byte ptr [ebp - 0x10], 8
        je     loc_3_223C
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [edi + 0x40], eax
loc_3_221B:
        push   0
        lea    eax, [ebp - 4]
        push   dword ptr [edi + 0x40]
        push   0
        push   eax
        call   sub_3_52E0
        add    esp, 0x10
        test   eax, eax
        je     loc_3_2253
        mov    eax, 0x13
        jmp    loc_3_23AA
loc_3_223C:
        push   0
        push   dword ptr [ebp - 4]
        call   sub_3_52B8
        add    esp, 8
        mov    eax, 0x13
        jmp    loc_3_23AA
loc_3_2253:
        xor    ebx, ebx
        lea    ecx, [ebp - 0xc]
        mov    dword ptr [ebp - 0x14], 0x13
        push   ebx
        push   ebx
        push   dword ptr [ebp - 4]
        push   ecx
        call   sub_3_5350
        add    esp, 0x10
        test   eax, eax
        jne    loc_3_239A
        push   ebx
        mov    esi, dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0x14]
        push   esi
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp - 0xc]
        lea    edx, [ebp - 8]
        push   edx
        call   sub_3_5348
        add    esp, 0x18
        test   eax, eax
        jne    loc_3_239A
        push   ebx
        push   ebx
        push   ebx
        push   dword ptr [ebp - 4]
        call   sub_3_5280
        add    esp, 0x10
        test   eax, eax
        jne    loc_3_239A
        push   ebx
        lea    ecx, [ebp - 0x1c]
        push   dword ptr [ebp - 4]
        push   ecx
        lea    edx, [ebp - 0x10]
        push   edx
        call   sub_3_5398
        add    esp, 0x10
        test   eax, eax
        jne    loc_3_239A
        test   byte ptr [ebp - 0x10], 8
        je     loc_3_239A
        push   2
        lea    ecx, [ebp - 0x18]
        push   dword ptr [edi + 0x40]
        push   ecx
        call   sub_3_5300
        add    esp, 0xc
        test   eax, eax
        jne    loc_3_239A
        push   2
        lea    ecx, [ebp - 0xc]
        push   dword ptr [ebp - 4]
        push   ecx
        call   sub_3_5300
        add    esp, 0xc
        test   eax, eax
        jne    loc_3_239A
        push   ebx
        lea    eax, [ebp - 0x20]
        push   eax
        lea    eax, [ebp - 8]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp - 0xc]
        push   eax
        call   sub_3_52F8
        add    esp, 0x14
        test   eax, eax
        jne    loc_3_239A
        push   ebx
        lea    ecx, [ebp - 0x24]
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp - 0x18]
        push   ecx
        call   sub_3_5348
        add    esp, 0x18
        test   eax, eax
        jne    loc_3_239A
        mov    eax, dword ptr [ebp + 0xc]
        cmp    eax, 1
        je     loc_3_2351
        cmp    eax, 2
        je     loc_3_2362
        cmp    eax, 4
        je     loc_3_2377
        jmp    loc_3_2382
loc_3_2351:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [eax + 4]
        mov    edx, dword ptr [eax + 8]
        mov    dword ptr [esi + 4], ecx
        mov    dword ptr [esi + 8], edx
        jmp    loc_3_2382
loc_3_2362:
        mov    eax, dword ptr [ebp - 8]
        mov    cx, word ptr [eax + 4]
        mov    ax, word ptr [eax + 6]
        mov    word ptr [esi + 4], cx
        mov    word ptr [esi + 6], ax
        jmp    loc_3_2382
loc_3_2377:
        mov    eax, dword ptr [ebp - 8]
        mov    ax, word ptr [eax + 2]
        mov    word ptr [esi + 2], ax
loc_3_2382:
        push   0
        lea    ecx, [ebp - 8]
        push   dword ptr [ebp - 8]
        push   ecx
        call   sub_3_5390
        mov    dword ptr [ebp - 0x14], 0
        add    esp, 0xc
loc_3_239A:
        push   0
        push   dword ptr [ebp - 4]
        call   sub_3_52B8
        add    esp, 8
        mov    eax, dword ptr [ebp - 0x14]
loc_3_23AA:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_23B1:
        push   0x18
        push   dword ptr [esp + 0xc]
        push   2
        push   dword ptr [esp + 0x10]
        call   sub_3_2198
        add    esp, 0x10
        ret    
sub_3_23C6:
        push   8
        push   dword ptr [esp + 0xc]
        push   4
        push   dword ptr [esp + 0x10]
        call   sub_3_2198
        add    esp, 0x10
        ret    
sub_3_23DB:
        push   0x24
        push   dword ptr [esp + 0xc]
        push   1
        push   dword ptr [esp + 0x10]
        call   sub_3_2198
        add    esp, 0x10
        ret    
sub_3_23F0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x24
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0xc], 0x13
        push   esi
        push   edi
        xor    esi, esi
        cmp    dword ptr [eax + 0x40], esi
        je     loc_3_257C
        push   2
        lea    edx, [ebp - 0x1c]
        push   dword ptr [eax + 0x40]
        push   edx
        call   sub_3_5300
        add    esp, 0xc
        test   eax, eax
        jne    loc_3_257C
        mov    eax, dword ptr [ebp - 0x1c]
        push   esi
        lea    ecx, [ebp - 0x10]
        mov    dword ptr [ebp - 4], eax
        push   ecx
        lea    ecx, [ebp - 4]
        push   dword ptr [ebp + 0xc]
        push   eax
        push   ecx
        call   sub_3_52F8
        add    esp, 0x14
        test   eax, eax
        jne    loc_3_257C
        mov    edi, dword ptr [ebp + 0x10]
        xor    ebx, ebx
loc_3_244F:
        mov    eax, dword ptr [ebp + 0xc]
        cmp    eax, 1
        je     loc_3_2463
        cmp    eax, 2
        je     loc_3_2476
        cmp    eax, 4
        je     loc_3_248D
        jmp    loc_3_249F
loc_3_2463:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 4]
        cmp    dword ptr [edi + 4], ecx
        jne    loc_3_249F
        mov    eax, dword ptr [eax + 8]
        cmp    dword ptr [edi + 8], eax
        jmp    loc_3_2498
loc_3_2476:
        mov    eax, dword ptr [ebp - 4]
        mov    cx, word ptr [eax + 4]
        cmp    word ptr [edi + 4], cx
        jne    loc_3_249F
        mov    ax, word ptr [eax + 6]
        cmp    word ptr [edi + 6], ax
        jmp    loc_3_2498
loc_3_248D:
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 2]
        cmp    word ptr [edi + 2], ax
loc_3_2498:
        jne    loc_3_249F
        mov    esi, 1
loc_3_249F:
        mov    dword ptr [ebp - 8], ebx
        test   esi, esi
        je     loc_3_2535
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        lea    edx, [ebp - 8]
        push   dword ptr [eax + 0x40]
        push   ebx
        push   edx
        call   sub_3_52E0
        add    esp, 0x10
        test   eax, eax
        jne    loc_3_2535
        push   2
        lea    ecx, [ebp - 0x14]
        push   ebx
        push   dword ptr [ebp - 8]
        push   ecx
        call   sub_3_5350
        add    esp, 0x10
        test   eax, eax
        jne    loc_3_2535
        push   ebx
        push   ebx
        push   ebx
        push   dword ptr [ebp - 8]
        call   sub_3_5280
        add    esp, 0x10
        test   eax, eax
        jne    loc_3_2535
        push   ebx
        lea    ecx, [ebp - 0x20]
        push   dword ptr [ebp - 8]
        push   ecx
        lea    edx, [ebp - 0x18]
        push   edx
        call   sub_3_5398
        add    esp, 0x10
        test   eax, eax
        jne    loc_3_2535
        test   byte ptr [ebp - 0x18], 8
        je     loc_3_2535
        push   ebx
        lea    ecx, [ebp - 0x24]
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp - 0x14]
        push   ecx
        call   sub_3_5348
        add    esp, 0x18
        test   eax, eax
        jne    loc_3_2535
        push   ebx
        push   dword ptr [ebp - 8]
        call   sub_3_52B8
        add    esp, 8
        test   eax, eax
        je     loc_3_2567
loc_3_2535:
        cmp    dword ptr [ebp - 8], ebx
        je     loc_3_2546
        push   ebx
        push   dword ptr [ebp - 8]
        call   sub_3_52B8
        add    esp, 8
loc_3_2546:
        push   ebx
        lea    eax, [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 4]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp - 4]
        push   eax
        call   sub_3_52F8
        add    esp, 0x14
        test   eax, eax
        je     loc_3_244F
        jmp    loc_3_257C
loc_3_2567:
        xor    esi, esi
        lea    ecx, [ebp - 4]
        push   esi
        push   dword ptr [ebp - 4]
        push   ecx
        call   sub_3_5390
        add    esp, 0xc
        mov    dword ptr [ebp - 0xc], esi
loc_3_257C:
        mov    eax, dword ptr [ebp - 0xc]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_2586:
        push   0x18
        push   dword ptr [esp + 0xc]
        push   2
        push   dword ptr [esp + 0x10]
        call   sub_3_23F0
        add    esp, 0x10
        ret    
sub_3_259B:
        push   8
        push   dword ptr [esp + 0xc]
        push   4
        push   dword ptr [esp + 0x10]
        call   sub_3_23F0
        add    esp, 0x10
        ret    
sub_3_25B0:
        push   0x24
        push   dword ptr [esp + 0xc]
        push   1
        push   dword ptr [esp + 0x10]
        call   sub_3_23F0
        add    esp, 0x10
        ret    
sub_3_25C5:
        push   ebx
        push   esi
        push   edi
        push   0x37
        call   sub_1_2540
loc_3_25CF:
        add    esp, 4
        mov    esi, eax
        mov    eax, 0x19
        test   esi, esi
        je     loc_3_262F
        xor    ebx, ebx
        mov    edi, dword ptr [esp + 0x10]
        push   ebx
        push   esi
        lea    eax, [edi + 0x24]
        push   eax
        call   sub_1_2D7C
loc_3_25EE:
        mov    ax, word ptr [edi + 0x10]
        mov    ecx, dword ptr [esp + 0x24]
        mov    word ptr [esi + 4], ax
        add    esp, 0xc
        mov    word ptr [esi + 6], bx
        mov    eax, dword ptr [esp + 0x14]
        mov    byte ptr [esi + 8], bl
        mov    byte ptr [esi + 9], 1
        mov    dl, byte ptr [ecx + 0xe]
        mov    byte ptr [esi + 0xa], dl
        mov    dword ptr [esi + 0xc], ebx
        mov    dword ptr [esi + 0x10], eax
        mov    ebx, dword ptr [ecx + 4]
        mov    dword ptr [esi + 0x14], ebx
        mov    dx, word ptr [ecx + 2]
        mov    word ptr [esi + 0x18], dx
        mov    ax, word ptr [ecx]
        mov    word ptr [esi + 0x1a], ax
        xor    eax, eax
loc_3_262F:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_3_2633:
        mov    ecx, dword ptr [esp + 4]
        lea    edx, [ecx + 0xc]
        mov    eax, dword ptr [edx]
        mov    eax, dword ptr [eax + 0x18]
        mov    dword ptr [0x14], eax         ; -> sub_1_1980
        mov    eax, dword ptr [edx]
        mov    eax, dword ptr [eax + 0x1c]
        mov    byte ptr [1], 0x8d            ; -> obj1:0x196D
        mov    dword ptr [0xa], eax          ; -> obj1:0x1976
        mov    edx, dword ptr [edx]
        mov    al, byte ptr [edx + 0xc]
        mov    byte ptr [0], al              ; -> obj1:0x196C
        mov    cl, byte ptr [ecx + 0x12]
        mov    byte ptr [2], cl              ; -> obj1:0x196E
        call   sub_1_2438
loc_3_266D:
        test   eax, eax
        mov    al, 0
        jne    loc_3_2678
        mov    al, byte ptr [4]              ; -> obj1:0x1970
loc_3_2678:
        ret    
sub_3_2679:
        mov    ecx, dword ptr [esp + 4]
        lea    edx, [ecx + 0xc]
        mov    eax, dword ptr [edx]
        mov    eax, dword ptr [eax + 0x18]
        mov    dword ptr [0x14], eax         ; -> sub_1_1980
        mov    eax, dword ptr [edx]
        mov    eax, dword ptr [eax + 0x1c]
        mov    byte ptr [1], 0x8d            ; -> obj1:0x196D
        mov    dword ptr [0xa], eax          ; -> obj1:0x1976
        mov    edx, dword ptr [edx]
        mov    al, byte ptr [edx + 0xc]
        mov    byte ptr [0], al              ; -> obj1:0x196C
        mov    cl, byte ptr [ecx + 0x12]
        mov    byte ptr [2], cl              ; -> obj1:0x196E
        call   sub_1_2438
loc_3_26B3:
        test   eax, eax
        mov    al, 0x19
        jne    loc_3_26DB
        mov    byte ptr [1], 0x8e            ; -> obj1:0x196D
        mov    al, byte ptr [esp + 8]
        mov    byte ptr [4], al              ; -> obj1:0x1970
        call   sub_1_2438
loc_3_26CE:
        mov    cl, 0
        cmp    eax, 1
        adc    cl, 0xff
        and    cl, 0x19
        mov    al, cl
loc_3_26DB:
        ret    
sub_3_26DC:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        lea    ecx, [esi + 0xc]
        mov    eax, dword ptr [ecx]
        mov    edx, dword ptr [eax + 0x18]
        mov    dword ptr [0x14], edx         ; -> sub_1_1980
        mov    eax, dword ptr [ecx]
        mov    edi, dword ptr [eax + 0x1c]
        mov    byte ptr [1], 0x84            ; -> obj1:0x196D
        mov    dword ptr [0xa], edi          ; -> obj1:0x1976
        mov    eax, dword ptr [ecx]
        mov    al, byte ptr [eax + 0xc]
        mov    dword ptr [0xe], 0            ; -> obj1:0x197A
        mov    byte ptr [0], al              ; -> obj1:0x196C
        call   sub_1_2438
loc_3_271B:
        test   eax, eax
        jne    loc_3_274B
        xor    al, al
        cmp    word ptr [0xe], 0             ; -> loc_1_19B2
        jbe    loc_3_274B
        mov    dl, byte ptr [esp + 0x10]
loc_3_272F:
        movzx  ecx, al
        cmp    byte ptr [ecx*2 + 0x10], dl   ; -> loc_1_19B4
        je     loc_3_2750
        inc    al
        movzx  edi, word ptr [0xe]           ; -> loc_1_19B2
        movzx  ecx, al
        cmp    ecx, edi
        jb     loc_3_272F
loc_3_274B:
        mov    al, 0x19
loc_3_274D:
        pop    edi
        pop    esi
        ret    
loc_3_2750:
        mov    cl, 0x11
        imul   cl
        push   eax
        push   esi
        call   sub_3_2679
        add    esp, 8
        jmp    loc_3_274D
sub_3_2760:
        push   ebp
        mov    ecx, dword ptr [0]            ; -> obj1:0x0000
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        call   sub_1_2967
loc_3_2773:
        test   al, 0x80
        mov    eax, 0x19
        je     loc_3_27BA
        lea    eax, [ebp - 4]
        mov    esi, dword ptr [ebp + 8]
        push   eax
        push   esi
        mov    eax, dword ptr [0]            ; -> obj1:0x0004
        push   eax
        call   sub_1_450F
loc_3_278F:
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 4]
        mov    edi, dword ptr [ebp + 0xc]
        lea    ecx, [ebp - 4]
        mov    al, byte ptr [eax]
        push   ecx
        inc    esi
        mov    byte ptr [edi], al
        push   esi
        mov    eax, dword ptr [0]            ; -> obj1:0x0004
        push   eax
        call   sub_1_450F
loc_3_27AD:
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax]
        mov    byte ptr [edi + 1], al
        xor    eax, eax
loc_3_27BA:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_27C0:
        push   ebp
        mov    ecx, dword ptr [0]            ; -> obj1:0x0000
        mov    ebp, esp
        sub    esp, 4
        push   esi
        call   sub_1_2967
loc_3_27D2:
        test   al, 0x80
        mov    eax, 0x19
        je     loc_3_2817
        lea    eax, [ebp - 4]
        mov    esi, dword ptr [ebp + 8]
        push   eax
        push   esi
        mov    eax, dword ptr [0]            ; -> obj1:0x0004
        push   eax
        call   sub_1_450F
loc_3_27EE:
        add    esp, 0xc
        mov    cl, byte ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp - 4]
        lea    edx, [ebp - 4]
        push   edx
        inc    esi
        push   esi
        mov    byte ptr [eax], cl
        mov    eax, dword ptr [0]            ; -> obj1:0x0004
        push   eax
        call   sub_1_450F
loc_3_280A:
        mov    eax, dword ptr [ebp - 4]
        add    esp, 0xc
        mov    cl, byte ptr [ebp + 0xd]
        mov    byte ptr [eax], cl
        xor    eax, eax
loc_3_2817:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_281C:
        mov    al, byte ptr [esp + 4]
        or     al, al
        lahf   
        and    ah, 4
        mov    byte ptr [esp + 4], ah
        cmp    byte ptr [esp + 4], 1
        sbb    eax, eax
        neg    eax
        ret    
sub_3_2834:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    eax, [ebp - 2]
        push   eax
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_3_2760
        add    esp, 8
        test   eax, eax
        jne    loc_3_28A3
        mov    al, byte ptr [ebp - 2]
        cmp    byte ptr [ebp - 1], al
        jne    loc_3_28A3
        push   dword ptr [ebp - 2]
        call   sub_3_281C
        add    esp, 4
        test   eax, eax
        je     loc_3_28A3
        lea    eax, [ebp - 4]
        add    esi, 2
        push   eax
        push   esi
        call   sub_3_2760
        add    esp, 8
        test   eax, eax
        jne    loc_3_28A3
        mov    al, byte ptr [ebp - 4]
        cmp    byte ptr [ebp - 3], al
        jne    loc_3_28A3
        mov    cx, word ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 0xc]
        and    cx, 0xff00
        mov    ax, word ptr [ebp - 2]
        and    ax, 0xff
        add    cx, ax
        mov    eax, 1
        mov    word ptr [edx], cx
        jmp    loc_3_28A5
loc_3_28A3:
        xor    eax, eax
loc_3_28A5:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_28AA:
        push   0x9090
        push   dword ptr [esp + 8]
        call   sub_3_27C0
        add    esp, 8
        test   eax, eax
        mov    eax, 0
        jne    loc_3_28D4
        push   dword ptr [esp + 8]
        push   dword ptr [esp + 8]
        call   sub_3_2834
        add    esp, 8
loc_3_28D4:
        ret    
sub_3_28D5:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x20
        push   esi
        push   edi
        lea    esi, [ebp - 0x1c]
        mov    edi, 0xb
loc_3_28E5:
        lea    eax, [ebp - 0x20]
        push   eax
        push   edi
        push   dword ptr [0]                 ; -> obj1:0x0004
        call   sub_1_450F
loc_3_28F5:
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 0x20]
        inc    edi
        mov    al, byte ptr [eax]
        mov    byte ptr [esi], al
        inc    esi
        cmp    edi, 0x24
        jb     loc_3_28E5
        cmp    word ptr [ebp - 0x1c], 0x200
        jne    loc_3_2963
        cmp    byte ptr [ebp - 0x1a], 0
        je     loc_3_2963
        cmp    word ptr [ebp - 0x19], 1
        jne    loc_3_2963
        cmp    byte ptr [ebp - 0x17], 1
        je     loc_3_2927
        cmp    byte ptr [ebp - 0x17], 2
        jne    loc_3_2963
loc_3_2927:
        xor    eax, eax
        cmp    word ptr [ebp - 0x16], ax
        je     loc_3_2963
        test   byte ptr [ebp - 0x16], 0xf
        jne    loc_3_2963
        cmp    word ptr [ebp - 0x14], ax
        jne    loc_3_2940
        cmp    dword ptr [ebp - 7], eax
        je     loc_3_2963
loc_3_2940:
        xor    eax, eax
        cmp    word ptr [ebp - 0x11], ax
        je     loc_3_2963
        cmp    word ptr [ebp - 0xf], ax
        je     loc_3_2963
        cmp    word ptr [ebp - 0xd], ax
        je     loc_3_2963
        cmp    byte ptr [ebp - 0x12], 0xf0
        jb     loc_3_2963
        movzx  eax, word ptr [ebp - 0x14]
        shl    eax, 9
        jmp    loc_3_2965
loc_3_2963:
        xor    eax, eax
loc_3_2965:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_296B:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        lea    eax, [ebp - 2]
        push   edi
        mov    esi, 0x4000
        push   eax
        push   esi
        call   sub_3_2760
        add    esp, 8
        test   eax, eax
        jne    loc_3_29EE
        xor    edi, edi
loc_3_298B:
        push   edi
        push   esi
        call   sub_3_27C0
        add    esp, 8
        test   eax, eax
        jne    loc_3_29EE
        lea    eax, [ebp - 6]
        push   eax
        push   esi
        call   sub_3_2760
        add    esp, 8
        test   eax, eax
        jne    loc_3_29EE
        lea    eax, [ebp - 4]
        push   eax
        push   edi
        call   sub_3_2760
        add    esp, 8
        test   eax, eax
        jne    loc_3_29EE
        cmp    word ptr [ebp - 6], di
        jne    loc_3_29F6
        cmp    word ptr [ebp - 4], di
        je     loc_3_29F6
        push   dword ptr [ebp - 2]
        push   esi
        call   sub_3_27C0
        add    esp, 8
        test   eax, eax
        jne    loc_3_29EE
        add    esi, 0x4000
        lea    eax, [ebp - 2]
        push   eax
        push   esi
        call   sub_3_2760
        add    esp, 8
        test   eax, eax
        je     loc_3_298B
loc_3_29EE:
        xor    eax, eax
loc_3_29F0:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
loc_3_29F6:
        mov    eax, esi
        jmp    loc_3_29F0
sub_3_29FA:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        lea    eax, [ebp - 2]
        push   esi
        push   edi
        push   eax
        push   0
        call   sub_3_28AA
        add    esp, 8
        test   eax, eax
        jne    loc_3_2A3B
        push   0x78
        push   dword ptr [ebp + 8]
        call   sub_3_26DC
        add    esp, 8
        lea    eax, [ebp - 2]
        push   eax
        push   0
        call   sub_3_28AA
        add    esp, 8
        test   eax, eax
        jne    loc_3_2A3B
        xor    eax, eax
        jmp    loc_3_2AD1
loc_3_2A3B:
        mov    ax, word ptr [0]              ; -> obj5:0x0104
        mov    esi, 0                        ; -> obj5:0x0104
        cmp    word ptr [ebp - 2], ax
        je     loc_3_2A5E
loc_3_2A4C:
        cmp    word ptr [esi], 0
        je     loc_3_2AB3
        mov    ax, word ptr [ebp - 2]
        add    esi, 0xf
        cmp    word ptr [esi], ax
        jne    loc_3_2A4C
loc_3_2A5E:
        mov    bx, word ptr [esi + 0xc]
        mov    eax, dword ptr [esi + 8]
        mov    edi, eax
        mov    dword ptr [ebp - 8], eax
loc_3_2A6A:
        lea    eax, [ebp - 4]
        push   eax
        push   edi
        call   sub_3_2834
        add    esp, 8
        test   eax, eax
        je     loc_3_2A85
        mov    ax, word ptr [ebp - 2]
        cmp    word ptr [ebp - 4], ax
        je     loc_3_2AB9
loc_3_2A85:
        lea    eax, [ebp - 4]
        push   eax
        push   edi
        call   sub_3_28AA
        add    esp, 8
        test   eax, eax
        je     loc_3_2AB9
        mov    ax, word ptr [ebp - 2]
        cmp    word ptr [ebp - 4], ax
        jne    loc_3_2AB9
        push   ebx
        push   edi
        call   sub_3_27C0
        add    esp, 8
        test   eax, eax
        jne    loc_3_2AB7
        add    edi, dword ptr [ebp - 8]
        jmp    loc_3_2A6A
loc_3_2AB3:
        xor    eax, eax
        jmp    loc_3_2AD1
loc_3_2AB7:
        xor    edi, edi
loc_3_2AB9:
        push   ebx
        push   0
        call   sub_3_27C0
        add    esp, 8
        test   eax, eax
        je     loc_3_2ACA
        xor    edi, edi
loc_3_2ACA:
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [eax], esi
        mov    eax, edi
loc_3_2AD1:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_2AD8:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        lea    eax, [esi + 0x24]
        push   eax
        call   sub_1_0889
loc_3_2AED:
        add    esp, 4
        lea    eax, [esi + 0x28]
        push   eax
        call   sub_1_0889
loc_3_2AF9:
        add    esp, 4
        lea    eax, [esi + 0x2c]
        xor    edi, edi
        push   eax
        call   sub_1_0889
loc_3_2B07:
        add    esp, 4
        lea    eax, [ebp - 2]
        push   eax
        push   edi
        call   sub_3_2760
        add    esp, 8
        test   eax, eax
        jne    loc_3_2C25
        push   0x9090
        push   edi
        call   sub_3_27C0
        add    esp, 8
        test   eax, eax
        jne    loc_3_2C25
        lea    eax, [ebp - 4]
        push   eax
        push   edi
        call   sub_3_2760
        add    esp, 8
        test   eax, eax
        jne    loc_3_2C25
        mov    ecx, esi
        call   sub_1_2967
loc_3_2B51:
        cmp    byte ptr [ebp - 2], 0xe9
        mov    di, ax
        je     loc_3_2B60
        cmp    byte ptr [ebp - 2], 0xeb
        jne    loc_3_2BA6
loc_3_2B60:
        mov    bx, di
        and    bx, 1
        jne    loc_3_2B71
        cmp    word ptr [ebp - 4], 0x9090
        jne    loc_3_2BA6
loc_3_2B71:
        push   dword ptr [ebp - 2]
        push   0
        call   sub_3_27C0
        add    esp, 8
        test   eax, eax
        jne    loc_3_2C25
        push   esi
        call   sub_3_28D5
        add    esp, 4
        test   eax, eax
        je     loc_3_2BA6
        test   bx, bx
        mov    ecx, 0xb4                     ; -> obj5:0x01B8
        jne    loc_3_2BA2
        mov    ecx, 0xa5                     ; -> obj5:0x01A9
loc_3_2BA2:
        push   ecx
        push   eax
        jmp    loc_3_2C1A
loc_3_2BA6:
        test   di, 1
        je     loc_3_2BB4
        mov    eax, 0x1a
        jmp    loc_3_2C25
loc_3_2BB4:
        cmp    word ptr [ebp - 4], 0x9090
        push   esi
        jne    loc_3_2BEA
        call   sub_3_296B
        add    esp, 4
        mov    edi, eax
        push   dword ptr [ebp - 2]
        push   0
        call   sub_3_27C0
        add    esp, 8
        test   eax, eax
        jne    loc_3_2C25
        test   edi, edi
        jne    loc_3_2BE3
        mov    eax, 0x19
        jmp    loc_3_2C25
loc_3_2BE3:
        push   0xa5                          ; -> obj5:0x01A9
        jmp    loc_3_2C19
loc_3_2BEA:
        call   sub_3_2633
        add    esp, 4
        mov    bl, al
        lea    eax, [ebp - 8]
        push   eax
        push   esi
        call   sub_3_29FA
        add    esp, 8
        mov    edi, eax
        push   ebx
        push   esi
        call   sub_3_2679
        add    esp, 8
        mov    eax, 0x19
        test   edi, edi
        je     loc_3_2C25
        push   dword ptr [ebp - 8]
loc_3_2C19:
        push   edi
loc_3_2C1A:
        push   esi
        call   sub_3_25C5
        add    esp, 0xc
        xor    eax, eax
loc_3_2C25:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_2C2C:
        push   esi
        push   edi
        mov    esi, 0x19
        push   0                             ; -> obj1:0x0004
        mov    edi, dword ptr [esp + 0x10]
        push   edi
        call   sub_1_4432
loc_3_2C42:
        add    esp, 8
        test   eax, eax
        jne    loc_3_2CAD
        mov    eax, dword ptr [0]            ; -> obj1:0x0004
        and    word ptr [eax + 0x14], 0xfffd
        mov    eax, dword ptr [0]            ; -> obj1:0x0004
        or     byte ptr [eax + 0x14], 8
        mov    ecx, dword ptr [0]            ; -> obj1:0x0004
        mov    dword ptr [ecx + 0x16], 0
        push   dword ptr [0]                 ; -> obj1:0x0004
        call   sub_1_4053
loc_3_2C75:
        add    esp, 4
        test   eax, eax
        jne    loc_3_2C9F
        mov    eax, dword ptr [0]            ; -> obj1:0x0004
        push   eax
        call   sub_1_4154
loc_3_2C87:
        add    esp, 4
        test   eax, eax
        jne    loc_3_2C9F
        push   edi
        mov    dword ptr [0], edi            ; -> obj1:0x0000
        call   sub_3_2AD8
        add    esp, 4
        mov    esi, eax
loc_3_2C9F:
        mov    eax, dword ptr [0]            ; -> obj1:0x0004
        push   eax
        call   sub_1_44FF
loc_3_2CAA:
        add    esp, 4
loc_3_2CAD:
        mov    eax, esi
        pop    edi
        pop    esi
        ret    
loc_3_2CB2:
        mov    al, byte ptr [0xa9]           ; -> obj5:0x0341
        and    al, 0xf
        cmp    al, 1
        jne    loc_3_2D20
        cmp    dword ptr [6], 0              ; -> obj5:0x029E
        je     loc_3_2D20
        test   byte ptr [0xe], 1             ; -> obj5:0x02A6
        je     loc_3_2D20
        cmp    byte ptr [0xd], 0             ; -> obj5:0x02A5
        je     loc_3_2D20
        mov    ax, word ptr [0xc]            ; -> obj5:0x01D4
        cmp    word ptr [0x11], ax           ; -> obj5:0x02A9
        jne    loc_3_2D20
        cmp    word ptr [0xf], 8             ; -> obj5:0x02A7
        jne    loc_3_2D20
        mov    ax, word ptr [0xe]            ; -> obj5:0x01D6
        cmp    word ptr [0x15], ax           ; -> obj5:0x02AD
        jne    loc_3_2D20
        cmp    word ptr [0x13], 2            ; -> obj5:0x02AB
        jne    loc_3_2D20
        mov    eax, dword ptr [esp + 8]
        mov    ecx, dword ptr [esp + 4]
        mov    byte ptr [ecx + 0x1e], 4
        mov    byte ptr [eax], 1
        mov    eax, 0x13
        jmp    loc_3_2D22
loc_3_2D20:
        xor    eax, eax
loc_3_2D22:
        ret    
sub_3_2D23:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    byte ptr [ebp - 1], 0
        lea    eax, [ebp - 1]
        push   eax
        push   0                             ; -> loc_3_2CB2
        push   dword ptr [ebp + 8]
        call   sub_3_4320
        movzx  eax, byte ptr [ebp - 1]
        add    esp, 0xc
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_2D49:
        push   esi
        mov    al, byte ptr [0xa9]           ; -> obj5:0x0341
        push   edi
        and    al, 0xf
        cmp    al, 1
        jne    loc_3_2DBB
        cmp    dword ptr [6], 0              ; -> obj5:0x029E
        je     loc_3_2DBB
        test   byte ptr [0xe], 1             ; -> obj5:0x02A6
        je     loc_3_2DBB
        cmp    byte ptr [0xd], 0             ; -> obj5:0x02A5
        je     loc_3_2DBB
        mov    eax, 0                        ; -> obj5:0x01C8
loc_3_2D76:
        mov    dx, word ptr [eax]
        mov    esi, 0x11                     ; -> obj5:0x02A9
loc_3_2D7E:
        cmp    word ptr [esi], dx
        jne    loc_3_2D96
        mov    di, 1
        mov    cl, byte ptr [eax + 2]
        shl    di, cl
        test   word ptr [0x23], di           ; -> obj5:0x02BB
        jne    loc_3_2DAD
loc_3_2D96:
        add    esi, 4
        cmp    esi, 0x21                     ; -> obj5:0x02B9
        jb     loc_3_2D7E
        add    eax, 3
        cmp    eax, 0xc                      ; -> obj5:0x01D4
        jb     loc_3_2D76
        jmp    loc_3_2DBB
loc_3_2DAD:
        mov    eax, dword ptr [esp + 0x10]
        mov    byte ptr [eax], 1
        mov    eax, 0x13
        jmp    loc_3_2DBD
loc_3_2DBB:
        xor    eax, eax
loc_3_2DBD:
        pop    edi
        pop    esi
        ret    
sub_3_2DC0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    eax, [ebp - 1]
        push   eax
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 1], 0
        push   0                             ; -> sub_3_2D49
        push   esi
        call   sub_3_4320
        add    esp, 0xc
        cmp    byte ptr [ebp - 1], 0
        je     loc_3_2DEA
        mov    byte ptr [esi + 0x1e], 2
loc_3_2DEA:
        movzx  eax, byte ptr [ebp - 1]
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
loc_3_2DF3:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        xor    edi, edi
        mov    dword ptr [ebp - 4], esi
loc_3_2E03:
        test   edi, edi
        jne    loc_3_2E4F
        mov    edx, 0x11                     ; -> obj5:0x02A9
        mov    ebx, dword ptr [ebp - 4]
        imul   ebx, ebx, 3
        mov    ax, word ptr [ebx]            ; -> obj5:0x01C8
loc_3_2E19:
        cmp    word ptr [edx], ax
        jne    loc_3_2E34
        mov    si, 1
        mov    cl, byte ptr [ebx + 2]        ; -> obj5:0x01CA
        shl    si, cl
        test   word ptr [0x23], si           ; -> obj5:0x02BB
        jne    loc_3_2E41
loc_3_2E34:
        add    edx, 4
        cmp    edx, 0x21                     ; -> obj5:0x02B9
        jb     loc_3_2E19
        jmp    loc_3_2E46
loc_3_2E41:
        mov    edi, 1
loc_3_2E46:
        inc    dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 4], 4
        jb     loc_3_2E03
loc_3_2E4F:
        mov    al, byte ptr [0xa9]           ; -> obj5:0x0341
        and    al, 0xf
        cmp    al, 1
        jne    loc_3_2FAF
        xor    eax, eax
        cmp    dword ptr [6], eax            ; -> obj5:0x029E
        je     loc_3_2FAF
        test   byte ptr [0xe], 1             ; -> obj5:0x02A6
        je     loc_3_2FAF
        cmp    byte ptr [0xd], al            ; -> obj5:0x02A5
        je     loc_3_2FAF
        mov    word ptr [0], ax              ; -> obj5:0x0298
        cmp    word ptr [0x23], ax           ; -> obj5:0x02BB
        je     loc_3_2E9D
        mov    word ptr [0], 2               ; -> obj5:0x0298
loc_3_2E9D:
        mov    byte ptr [5], 2               ; -> obj5:0x029D
        mov    byte ptr [0xa], 0             ; -> obj5:0x02A2
        test   byte ptr [0xe], 2             ; -> obj5:0x02A6
        je     loc_3_2ED8
        push   0
        lea    eax, [ebp - 4]
        push   eax
        call   sub_3_1180
        add    esp, 8
        test   byte ptr [ebp - 4], 0x10
        jne    loc_3_2ED8
        test   byte ptr [0xaa], 8            ; -> obj5:0x0342
        je     loc_3_2ED8
        or     byte ptr [0xa], 8             ; -> obj5:0x02A2
loc_3_2ED8:
        and    byte ptr [0xe], 0xfb          ; -> obj5:0x02A6
        mov    byte ptr [0xb], 0             ; -> obj5:0x02A3
        mov    byte ptr [0xc], 0             ; -> obj5:0x02A4
        and    byte ptr [0x25], 0x7f         ; -> obj5:0x02BD
        test   edi, edi
        je     loc_3_2F22
        mov    word ptr [0x23], si           ; -> obj5:0x02BB
        push   0x2000
        push   0                             ; -> obj5:0x0298
        mov    esi, dword ptr [ebp + 0xc]
        push   dword ptr [esi]
        push   dword ptr [ebp + 8]
        call   sub_3_44F4
        add    esp, 0x10
        test   eax, eax
        je     loc_3_2F25
        jmp    loc_3_2FB1
loc_3_2F22:
        mov    esi, dword ptr [ebp + 0xc]
loc_3_2F25:
        cmp    word ptr [0x23], 0            ; -> obj5:0x02BB
        je     loc_3_2F38
        mov    word ptr [0x23], 0xffff       ; -> obj5:0x02BB
loc_3_2F38:
        cmp    edi, 1
        sbb    edi, edi
        and    edi, 0x2000
        add    edi, 0x3000
        push   edi
        push   0                             ; -> obj5:0x0298
        push   dword ptr [esi]
        push   dword ptr [ebp + 8]
        call   sub_3_44F4
        add    esp, 0x10
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_3_2FB1
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        push   dword ptr [ecx]
        call   sub_1_2CF8
loc_3_2F70:
        movzx  ecx, word ptr [0x23]          ; -> obj5:0x02BB
        add    esp, 8
        test   dword ptr [ebp - 8], ecx
        je     loc_3_2FAB
        mov    eax, dword ptr [ebp - 8]
        add    edi, 0x800
        or     byte ptr [0x25], 0x80         ; -> obj5:0x02BD
        and    word ptr [0x23], ax           ; -> obj5:0x02BB
        push   edi
        push   0                             ; -> obj5:0x0298
        push   dword ptr [esi]
        push   dword ptr [ebp + 8]
        call   sub_3_44F4
        add    esp, 0x10
        mov    ebx, eax
loc_3_2FAB:
        mov    eax, ebx
        jmp    loc_3_2FB1
loc_3_2FAF:
        xor    eax, eax
loc_3_2FB1:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_2FB8:
        lea    eax, [esp + 8]
        push   eax
        push   0                             ; -> loc_3_2DF3
        push   dword ptr [esp + 0xc]
        call   sub_3_4320
        add    esp, 0xc
        ret    
sub_3_2FCF:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        push   edi
        mov    edi, 1
        mov    al, byte ptr [esi]
        and    al, 0x78
        shr    al, 2
        movzx  edx, al
        test   byte ptr [esi], 0x80
        mov    cx, word ptr [edx]            ; -> obj5:0x0348
        je     loc_3_3024
        mov    bx, 0xa
        mov    ax, cx
        sub    dx, dx
        div    bx
        movzx  eax, ax
        movzx  edx, cx
        imul   eax, eax, 0xa
        cmp    eax, edx
        jne    loc_3_3024
        mov    al, byte ptr [esi + 1]
        and    al, 0x7f
        cmp    al, 0x64
        jae    loc_3_3024
        imul   cx, bx
        movzx  ax, al
        add    cx, ax
        mov    edi, 2
loc_3_3024:
        xor    ebx, ebx
        mov    bl, byte ptr [esi]
        and    ebx, 7
        sub    ebx, edi
        sub    ebx, 4
        test   ebx, ebx
        jle    loc_3_303D
loc_3_3034:
        imul   cx, cx, 0xa
        dec    ebx
        jne    loc_3_3034
        test   ebx, ebx
loc_3_303D:
        jge    loc_3_3054
        neg    ebx
loc_3_3041:
        mov    si, 0xa
        mov    ax, cx
        sub    dx, dx
        div    si
        mov    cx, ax
        dec    ebx
        jne    loc_3_3041
loc_3_3054:
        mov    al, cl
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_3_305A:
        push   ebp
        mov    al, byte ptr [esp + 0x10]
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        xor    edi, edi
        and    al, 3
        mov    byte ptr [ebp - 8], al
        je     loc_3_3103
loc_3_3077:
        mov    al, byte ptr [esi]
        xor    ebx, ebx
        inc    esi
        mov    byte ptr [ebp - 1], al
loc_3_307F:
        mov    edx, 1
        mov    cl, bl
        shl    edx, cl
        movzx  eax, byte ptr [ebp - 1]
        test   eax, edx
        je     loc_3_30F0
        mov    al, byte ptr [esi]
        mov    byte ptr [ebp - 4], al
        test   byte ptr [esi], 0x80
        je     loc_3_30A2
        inc    esi
        mov    al, byte ptr [esi]
        mov    byte ptr [ebp - 3], al
        jmp    loc_3_30A6
loc_3_30A2:
        mov    byte ptr [ebp - 3], 0
loc_3_30A6:
        test   byte ptr [esi], 0x80
        je     loc_3_30B1
loc_3_30AB:
        inc    esi
        test   byte ptr [esi], 0x80
        jne    loc_3_30AB
loc_3_30B1:
        inc    esi
        test   ebx, ebx
        jne    loc_3_30F0
        lea    eax, [ebp - 4]
        push   eax
        call   sub_3_2FCF
        add    esp, 4
        mov    byte ptr [ebp - 4], al
        test   edi, edi
        je     loc_3_30D5
        cmp    edi, 1
        je     loc_3_30DE
        cmp    edi, 2
        je     loc_3_30E7
        jmp    loc_3_30F0
loc_3_30D5:
        mov    cl, byte ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0xc]
        mov    byte ptr [eax + 2], cl
loc_3_30DE:
        mov    cl, byte ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0xc]
        mov    byte ptr [eax + 3], cl
loc_3_30E7:
        mov    cl, byte ptr [ebp - 4]
        mov    eax, dword ptr [ebp + 0xc]
        mov    byte ptr [eax + 4], cl
loc_3_30F0:
        inc    ebx
        cmp    ebx, 7
        jl     loc_3_307F
        movzx  eax, byte ptr [ebp - 8]
        inc    edi
        cmp    eax, edi
        jg     loc_3_3077
loc_3_3103:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_310C:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        mov    al, byte ptr [esp + 0xc]
        and    eax, 0x30
        je     loc_3_314D
        cmp    eax, 0x10
        je     loc_3_3158
        cmp    eax, 0x20
        je     loc_3_3166
        cmp    eax, 0x30
        je     loc_3_3176
loc_3_3129:
        mov    eax, dword ptr [ebp + 8]
loc_3_312C:
        xor    edx, edx
        mov    ecx, 0xc0
        mov    dl, byte ptr [ebp + 0xc]
        and    edx, ecx
        je     loc_3_3185
        cmp    edx, 0x40
        je     loc_3_3190
        cmp    edx, 0x80
        je     loc_3_319C
        cmp    edx, ecx
        je     loc_3_31AA
        jmp    loc_3_31B5
loc_3_314D:
        mov    edx, dword ptr [ebp + 0x10]
        mov    dword ptr [edx], 0
        jmp    loc_3_3129
loc_3_3158:
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0x10]
        movzx  ecx, byte ptr [eax]
        inc    eax
        mov    dword ptr [edx], ecx
        jmp    loc_3_312C
loc_3_3166:
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0x10]
        movzx  ecx, word ptr [eax]
        add    eax, 2
        mov    dword ptr [edx], ecx
        jmp    loc_3_312C
loc_3_3176:
        mov    eax, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [eax]
        add    eax, 4
        mov    dword ptr [edx], ecx
        jmp    loc_3_312C
loc_3_3185:
        mov    edx, dword ptr [ebp + 0x14]
        mov    dword ptr [edx], 0
        jmp    loc_3_31B5
loc_3_3190:
        movzx  ecx, byte ptr [eax]
        inc    ecx
        mov    edx, dword ptr [ebp + 0x14]
        inc    eax
        mov    dword ptr [edx], ecx
        jmp    loc_3_31B5
loc_3_319C:
        movzx  ecx, word ptr [eax]
        inc    ecx
        mov    edx, dword ptr [ebp + 0x14]
        add    eax, 2
        mov    dword ptr [edx], ecx
        jmp    loc_3_31B5
loc_3_31AA:
        mov    ecx, dword ptr [eax]
        mov    edx, dword ptr [ebp + 0x14]
        inc    ecx
        add    eax, 4
        mov    dword ptr [edx], ecx
loc_3_31B5:
        pop    ebp
        ret    
sub_3_31B7:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    dl, byte ptr [ebp + 0xc]
        mov    al, dl
        and    eax, 0x18
        je     loc_3_3256
        cmp    eax, 8
        je     loc_3_3264
        cmp    eax, 0x10
        je     loc_3_3275
        cmp    eax, 0x18
        je     loc_3_3288
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0x10]
loc_3_31EE:
        shl    dword ptr [esi], 8
        xor    ebx, ebx
        mov    bl, dl
        and    ebx, 0x60
        je     loc_3_32A4
        cmp    ebx, 0x20
        je     loc_3_32B2
        cmp    ebx, 0x40
        je     loc_3_32C0
        cmp    ebx, 0x60
        je     loc_3_32D0
        mov    esi, dword ptr [ebp + 0x18]
loc_3_321C:
        shl    dword ptr [esi], 8
        test   dl, 0x80
        je     loc_3_332E
        test   ebx, ebx
        je     loc_3_32E9
        cmp    ebx, 0x20
        je     loc_3_32F7
        cmp    ebx, 0x40
        je     loc_3_3305
        cmp    ebx, 0x60
        je     loc_3_3315
        mov    edx, dword ptr [ebp + 0x14]
loc_3_324E:
        shl    dword ptr [edx], 8
        jmp    loc_3_3337
loc_3_3256:
        mov    esi, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [esi], 0
        jmp    loc_3_31EE
loc_3_3264:
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0x10]
        movzx  ecx, byte ptr [eax]
        inc    eax
        mov    dword ptr [esi], ecx
        jmp    loc_3_31EE
loc_3_3275:
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0x10]
        movzx  ecx, word ptr [eax]
        add    eax, 2
        mov    dword ptr [esi], ecx
        jmp    loc_3_31EE
loc_3_3288:
        mov    eax, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0x10]
        movzx  edi, byte ptr [eax + 2]
        movzx  ecx, word ptr [eax]
        shl    edi, 0x10
        add    eax, 3
        add    edi, ecx
        mov    dword ptr [esi], edi
        jmp    loc_3_31EE
loc_3_32A4:
        mov    esi, dword ptr [ebp + 0x18]
        mov    dword ptr [esi], 0
        jmp    loc_3_321C
loc_3_32B2:
        movzx  ecx, byte ptr [eax]
        mov    esi, dword ptr [ebp + 0x18]
        inc    eax
        mov    dword ptr [esi], ecx
        jmp    loc_3_321C
loc_3_32C0:
        movzx  ecx, word ptr [eax]
        mov    esi, dword ptr [ebp + 0x18]
        add    eax, 2
        mov    dword ptr [esi], ecx
        jmp    loc_3_321C
loc_3_32D0:
        movzx  edi, byte ptr [eax + 2]
        movzx  ecx, word ptr [eax]
        shl    edi, 0x10
        mov    esi, dword ptr [ebp + 0x18]
        add    edi, ecx
        add    eax, 3
        mov    dword ptr [esi], edi
        jmp    loc_3_321C
loc_3_32E9:
        mov    edx, dword ptr [ebp + 0x14]
        mov    dword ptr [edx], 0
        jmp    loc_3_324E
loc_3_32F7:
        movzx  ecx, byte ptr [eax]
        mov    edx, dword ptr [ebp + 0x14]
        inc    eax
        mov    dword ptr [edx], ecx
        jmp    loc_3_324E
loc_3_3305:
        movzx  ecx, word ptr [eax]
        mov    edx, dword ptr [ebp + 0x14]
        add    eax, 2
        mov    dword ptr [edx], ecx
        jmp    loc_3_324E
loc_3_3315:
        movzx  ecx, byte ptr [eax + 2]
        movzx  edx, word ptr [eax]
        shl    ecx, 0x10
        add    eax, 3
        add    ecx, edx
        mov    edx, dword ptr [ebp + 0x14]
        mov    dword ptr [edx], ecx
        jmp    loc_3_324E
loc_3_332E:
        mov    edx, dword ptr [ebp + 0x14]
        mov    dword ptr [edx], 0xffffffff
loc_3_3337:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
sub_3_333C:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0x18
        test   byte ptr [eax], 0x80
        push   ebx
        push   esi
        push   edi
        je     loc_3_335C
        inc    eax
        mov    esi, dword ptr [ebp + 0xc]
        mov    cl, byte ptr [eax]
        mov    byte ptr [esi + 0xa9], cl
        jmp    loc_3_335F
loc_3_335C:
        mov    esi, dword ptr [ebp + 0xc]
loc_3_335F:
        inc    eax
        mov    cl, byte ptr [eax]
        inc    eax
        mov    byte ptr [ebp - 1], cl
        push   dword ptr [ebp - 1]
        push   esi
        push   eax
        call   sub_3_305A
        add    esp, 0xc
        test   byte ptr [ebp - 1], 4
        je     loc_3_33A6
        mov    cl, byte ptr [eax]
        inc    eax
        mov    dl, cl
        and    dl, 3
        cmp    dl, 3
        je     loc_3_3392
        test   byte ptr [eax], 0x80
        je     loc_3_3391
loc_3_338B:
        inc    eax
        test   byte ptr [eax], 0x80
        jne    loc_3_338B
loc_3_3391:
        inc    eax
loc_3_3392:
        and    cl, 0x1c
        cmp    cl, 0x1c
        je     loc_3_33A6
        test   byte ptr [eax], 0x80
        je     loc_3_33A5
loc_3_339F:
        inc    eax
        test   byte ptr [eax], 0x80
        jne    loc_3_339F
loc_3_33A5:
        inc    eax
loc_3_33A6:
        test   byte ptr [ebp - 1], 8
        je     loc_3_3482
        lea    edi, [esi + 0xf]
        mov    ecx, 4
        mov    edx, edi
        xor    ebx, ebx
loc_3_33BC:
        mov    dword ptr [esi + 0x1f], ebx
        mov    word ptr [edx], bx
        add    edx, 4
        mov    word ptr [edx - 2], bx
        dec    ecx
        jne    loc_3_33BC
        mov    bl, byte ptr [eax]
        inc    eax
        test   bl, 0x80
        movzx  ecx, bl
        mov    dword ptr [esi + 0x1f], ecx
        je     loc_3_3460
        mov    dword ptr [ebp - 0x10], 0
        mov    cl, byte ptr [eax]
        inc    eax
        mov    byte ptr [ebp - 2], cl
        and    cl, 0xf
        movzx  ecx, cl
        mov    dword ptr [ebp - 0x18], ecx
loc_3_33F4:
        lea    ecx, [ebp - 8]
        lea    edx, [ebp - 0xc]
        push   ecx
        push   edx
        push   dword ptr [ebp - 2]
        push   eax
        call   sub_3_310C
        add    esp, 0x10
        cmp    dword ptr [ebp - 0x10], 4
        jge    loc_3_3450
        cmp    dword ptr [ebp - 8], 0
        jne    loc_3_3423
        mov    cl, bl
        mov    edx, 1
        and    cl, 0x1f
        shl    edx, cl
        mov    dword ptr [ebp - 8], edx
loc_3_3423:
        cmp    dword ptr [ebp - 8], 0xffff
        ja     loc_3_3450
        cmp    dword ptr [ebp - 0xc], 0xffff
        ja     loc_3_3450
        mov    ecx, dword ptr [ebp - 8]
        add    ecx, dword ptr [ebp - 0xc]
        cmp    ecx, 0x10000
        ja     loc_3_3450
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp - 0xc]
        mov    word ptr [edi], cx
        mov    word ptr [edi + 2], dx
loc_3_3450:
        add    edi, 4
        inc    dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp - 0x10]
        cmp    ecx, dword ptr [ebp - 0x18]
        jle    loc_3_33F4
        jmp    loc_3_3482
loc_3_3460:
        mov    cl, bl
        and    cl, 0x1f
        je     loc_3_3482
        mov    edx, 1
        shl    edx, cl
        mov    dword ptr [ebp - 8], edx
        cmp    edx, 0xffff
        ja     loc_3_347C
        mov    word ptr [edi], dx
loc_3_347C:
        mov    word ptr [esi + 0x11], 0
loc_3_3482:
        test   byte ptr [ebp - 1], 0x10
        je     loc_3_34B0
        mov    cl, byte ptr [eax]
        inc    eax
        test   cl, 0x10
        movzx  edx, cl
        mov    dword ptr [esi + 0x25], edx
        je     loc_3_34A2
        mov    cx, word ptr [eax]
        add    eax, 2
        mov    word ptr [esi + 0x23], cx
        jmp    loc_3_34B0
loc_3_34A2:
        mov    dx, 1
        and    cl, 0xf
        shl    dx, cl
        mov    word ptr [esi + 0x23], dx
loc_3_34B0:
        mov    dl, byte ptr [ebp - 1]
        and    dl, 0x60
        je     loc_3_34CA
        lea    ecx, [esi + 0x29]
        mov    ebx, 8
        xor    edi, edi
loc_3_34C2:
        mov    dword ptr [ecx], edi
        add    ecx, 0x10
        dec    ebx
        jne    loc_3_34C2
loc_3_34CA:
        movzx  ecx, dl
        cmp    ecx, 0x20
        je     loc_3_34E1
        cmp    ecx, 0x40
        je     loc_3_3501
        cmp    ecx, 0x60
        je     loc_3_3523
        jmp    loc_3_358C
loc_3_34E1:
        mov    dword ptr [esi + 0x31], 0xffffffff
        movzx  ecx, word ptr [eax]
        shl    ecx, 8
        add    eax, 2
        mov    dword ptr [esi + 0x29], ecx
        xor    ecx, ecx
        mov    dword ptr [esi + 0x2d], ecx
        mov    dword ptr [esi + 0x35], ecx
        jmp    loc_3_358C
loc_3_3501:
        movzx  ecx, word ptr [eax]
        shl    ecx, 8
        add    eax, 2
        movzx  edx, word ptr [eax]
        shl    edx, 8
        add    eax, 2
        mov    dword ptr [esi + 0x29], ecx
        mov    dword ptr [esi + 0x2d], edx
        add    ecx, edx
        mov    dword ptr [esi + 0x35], edx
        mov    dword ptr [esi + 0x31], ecx
        jmp    loc_3_358C
loc_3_3523:
        mov    cl, byte ptr [eax]
        xor    edi, edi
        inc    eax
        lea    ebx, [esi + 0x2d]
        mov    byte ptr [ebp - 2], cl
        and    cl, 7
        movzx  ecx, cl
        mov    dword ptr [ebp - 0x10], ecx
loc_3_3537:
        lea    ecx, [ebp - 0x14]
        lea    edx, [ebp - 0xc]
        push   ecx
        push   edx
        lea    ecx, [ebp - 8]
        push   ecx
        push   dword ptr [ebp - 2]
        push   eax
        call   sub_3_31B7
        add    esp, 0x14
        cmp    edi, 8
        jge    loc_3_3583
        cmp    dword ptr [ebp - 0xc], -1
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ebx - 4], ecx
        jne    loc_3_356F
        mov    dword ptr [ebx], 0
        mov    dword ptr [ebx + 4], 0xffffffff
        jmp    loc_3_357D
loc_3_356F:
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [ebx], ecx
        mov    ecx, dword ptr [ebp - 8]
        add    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [ebx + 4], ecx
loc_3_357D:
        mov    ecx, dword ptr [ebp - 0x14]
        mov    dword ptr [ebx + 8], ecx
loc_3_3583:
        add    ebx, 0x10
        inc    edi
        cmp    edi, dword ptr [ebp - 0x10]
        jle    loc_3_3537
loc_3_358C:
        test   byte ptr [ebp - 1], 0x80
        je     loc_3_359A
        mov    al, byte ptr [eax]
        mov    byte ptr [esi + 0xaa], al
loc_3_359A:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_35A1:
        mov    eax, dword ptr [esp + 4]
        push   ebx
        push   esi
        inc    eax
        mov    esi, dword ptr [esp + 0x10]
        mov    bl, byte ptr [eax]
        push   ebx
        inc    eax
        push   esi
        push   eax
        call   sub_3_305A
        add    esp, 0xc
        test   bl, 8
        je     loc_3_35C0
        inc    eax
loc_3_35C0:
        test   bl, 0x10
        je     loc_3_35ED
        mov    cl, byte ptr [eax]
        inc    eax
        test   cl, 0x10
        movzx  edx, cl
        mov    dword ptr [esi + 0x25], edx
        je     loc_3_35DF
        mov    cx, word ptr [eax]
        add    eax, 2
        mov    word ptr [esi + 0x23], cx
        jmp    loc_3_35ED
loc_3_35DF:
        mov    dx, 1
        and    cl, 0xf
        shl    dx, cl
        mov    word ptr [esi + 0x23], dx
loc_3_35ED:
        test   bl, 0x20
        je     loc_3_35F3
        inc    eax
loc_3_35F3:
        test   bl, 0x80
        je     loc_3_3600
        mov    al, byte ptr [eax]
        mov    byte ptr [esi + 0xaa], al
loc_3_3600:
        pop    esi
        pop    ebx
        ret    
sub_3_3603:
        mov    edx, dword ptr [esp + 4]
        push   ebx
        push   esi
        add    edx, 0x14
        push   edi
        xor    ebx, ebx
        push   ebp
        mov    al, byte ptr [edx]
        mov    bl, al
        and    ebx, 0x40
        jne    loc_3_3630
        mov    esi, 0                        ; -> obj5:0x01E8
        mov    ebp, dword ptr [esp + 0x18]
        mov    edi, ebp
        mov    ecx, 0x2b
        rep movsd dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
        jmp    loc_3_3634
loc_3_3630:
        mov    ebp, dword ptr [esp + 0x18]
loc_3_3634:
        and    al, 0x3f
        push   ebp
        push   edx
        mov    byte ptr [ebp + 0xd], al
        cmp    dword ptr [esp + 0x24], 0
        je     loc_3_3649
        call   sub_3_35A1
        jmp    loc_3_364E
loc_3_3649:
        call   sub_3_333C
loc_3_364E:
        add    esp, 8
        test   ebx, ebx
        je     loc_3_3666
        mov    edi, 0                        ; -> obj5:0x01E8
        mov    esi, ebp
        mov    ecx, 0x2b
        rep movsd dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
loc_3_3666:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_3_366B:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        add    ecx, 0x14
        movzx  edx, word ptr [ecx - 4]
        push   ebx
        cmp    edx, 4
        push   esi
        push   edi
        jl     loc_3_377F
        mov    bl, byte ptr [ecx]
        xor    eax, eax
        mov    al, bl
        sub    edx, 2
        and    eax, 3
        and    bl, 0x3c
        shr    bl, 2
        add    ecx, 2
        movzx  esi, bl
        lea    edi, [eax + 1]
        cmp    edi, edx
        jg     loc_3_377F
        test   eax, eax
        je     loc_3_36BD
        cmp    eax, 1
        je     loc_3_36C2
        cmp    eax, 2
        je     loc_3_36C7
        cmp    eax, 3
        je     loc_3_36D5
        jmp    loc_3_36DC
loc_3_36BD:
        movzx  ebx, byte ptr [ecx]
        jmp    loc_3_36D7
loc_3_36C2:
        movzx  ebx, word ptr [ecx]
        jmp    loc_3_36D7
loc_3_36C7:
        movzx  ebx, byte ptr [ecx + 2]
        movzx  edi, word ptr [ecx]
        shl    ebx, 0x10
        add    ebx, edi
        jmp    loc_3_36D7
loc_3_36D5:
        mov    ebx, dword ptr [ecx]
loc_3_36D7:
        mov    edi, dword ptr [ebp + 0xc]
        mov    dword ptr [edi], ebx
loc_3_36DC:
        lea    edi, [eax + 1]
        lea    ecx, [eax + ecx + 1]
        sub    edx, edi
        lea    eax, [esi + 1]
        cmp    eax, edx
        jg     loc_3_377F
        test   esi, esi
        jne    loc_3_36FA
        movzx  ax, byte ptr [ecx]
        jmp    loc_3_36FD
loc_3_36FA:
        mov    ax, word ptr [ecx]
loc_3_36FD:
        mov    edi, dword ptr [ebp + 0x10]
        lea    ecx, [esi + ecx + 1]
        mov    word ptr [edi], ax
        lea    eax, [esi + 1]
        sub    edx, eax
        xor    esi, esi
        mov    eax, dword ptr [ebp + 0x14]
loc_3_3711:
        cmp    edx, 3
        jl     loc_3_377F
        mov    bl, byte ptr [ecx]
        cmp    bl, 0xff
        je     loc_3_377F
        cmp    bl, 0xc0
        jne    loc_3_3769
        mov    bl, byte ptr [ecx + 2]
        shr    bl, 6
        movzx  edi, bl
        lea    ebx, [edi + 3]
        cmp    ebx, edx
        jg     loc_3_377F
        test   edi, edi
        je     loc_3_3747
        cmp    edi, 1
        je     loc_3_374F
        cmp    edi, 2
        je     loc_3_3755
        cmp    edi, 3
        je     loc_3_3764
        jmp    loc_3_3769
loc_3_3747:
        movzx  ebx, byte ptr [ecx + 2]
        mov    dword ptr [eax], ebx
        jmp    loc_3_3769
loc_3_374F:
        movzx  edi, word ptr [ecx + 2]
        jmp    loc_3_3767
loc_3_3755:
        movzx  edi, byte ptr [ecx + 4]
        movzx  ebx, word ptr [ecx + 2]
        shl    edi, 0x10
        add    edi, ebx
        jmp    loc_3_3767
loc_3_3764:
        mov    edi, dword ptr [ecx + 2]
loc_3_3767:
        mov    dword ptr [eax], edi
loc_3_3769:
        movzx  edi, byte ptr [ecx + 1]
        add    eax, 4
        inc    esi
        lea    ebx, [edi + 2]
        lea    ecx, [edi + ecx + 2]
        sub    edx, ebx
        cmp    esi, 4
        jl     loc_3_3711
loc_3_377F:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
sub_3_3784:
        push   ebp
        mov    edx, dword ptr [esp + 0x10]
        mov    ebp, esp
        test   edx, edx
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        jle    loc_3_37A5
        mov    ecx, dword ptr [ebp + 0xc]
loc_3_3796:
        mov    bl, byte ptr [ecx]
        mov    byte ptr [eax], bl
        inc    eax
        test   bl, bl
        je     loc_3_37A5
        dec    edx
        inc    ecx
        test   edx, edx
        jg     loc_3_3796
loc_3_37A5:
        mov    byte ptr [eax - 1], 0
        pop    ebx
        pop    ebp
        ret    
sub_3_37AC:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0x1c
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [eax + 0x18]
        xor    eax, eax
        push   0x1a
        push   esi
        mov    dword ptr [ebp - 0xc], eax
        mov    dword ptr [ebp - 8], eax
        call   sub_3_4C91
        add    esp, 8
        test   eax, eax
        jne    loc_3_37F9
        push   esi
        call   sub_3_4CF1
        add    esp, 4
        test   eax, eax
        jne    loc_3_37F9
        lea    eax, [ebp - 0x1c]
        lea    ecx, [ebp - 2]
        push   eax
        lea    edx, [ebp - 8]
        push   ecx
        push   edx
        push   0                             ; -> obj5:0x03D4
        call   sub_3_366B
        add    esp, 0x10
loc_3_37F9:
        push   0xff
        push   esi
        call   sub_3_4C91
        add    esp, 8
        test   eax, eax
        jne    loc_3_38DD
        mov    edi, 0x14                     ; -> obj5:0x03E8
loc_3_3814:
        mov    eax, 0                        ; -> loc_1_0030
        mov    cl, byte ptr [0]              ; -> loc_1_0030
        cmp    byte ptr [0x10], cl           ; -> obj5:0x03D0
        je     loc_3_3837
loc_3_3827:
        cmp    byte ptr [eax], 0xff
        je     loc_3_3837
        inc    eax
        mov    cl, byte ptr [0x10]           ; -> obj5:0x03D0
        cmp    byte ptr [eax], cl
        jne    loc_3_3827
loc_3_3837:
        mov    bl, byte ptr [eax]
        cmp    bl, 0xff
        je     loc_3_38CC
        push   esi
        call   sub_3_4CF1
        add    esp, 4
        test   eax, eax
        jne    loc_3_38CC
        test   byte ptr [6], 1               ; -> obj5:0x03DA
        movzx  ecx, word ptr [0x10]          ; -> obj5:0x03E4
        jne    loc_3_388C
        mov    eax, dword ptr [0xc]          ; -> obj5:0x03E0
        add    eax, 4
        cmp    eax, dword ptr [ebp - 8]
        ja     loc_3_388C
        lea    eax, [ecx*2 + 4]
        add    eax, dword ptr [0xc]          ; -> obj5:0x03E0
        cmp    eax, dword ptr [ebp - 8]
        jbe    loc_3_388C
        mov    ecx, dword ptr [ebp - 8]
        sub    ecx, dword ptr [0xc]          ; -> obj5:0x03E0
        sub    ecx, 4
        shr    ecx, 1
loc_3_388C:
        cmp    bl, 0x15
        jne    loc_3_38BC
        mov    eax, 0x16                     ; -> obj5:0x03EA
        lea    ebx, [ecx + 0x14]             ; -> obj5:0x03E8
loc_3_389C:
        cmp    eax, ebx
        jae    loc_3_38B4
        mov    edx, eax
        inc    eax
        cmp    byte ptr [edx], 0
        jne    loc_3_389C
loc_3_38A8:
        cmp    eax, ebx
        jae    loc_3_38B4
        mov    edx, eax
        inc    eax
        cmp    byte ptr [edx], 0
        jne    loc_3_38A8
loc_3_38B4:
        sub    eax, edi
        cmp    ecx, eax
        jbe    loc_3_38BC
        mov    ecx, eax
loc_3_38BC:
        push   0
        lea    eax, [ebp - 0xc]
        push   eax
        push   ecx
        push   edi
        call   sub_3_5420
        add    esp, 0x10
loc_3_38CC:
        push   esi
        call   sub_3_4CDD
        add    esp, 4
        test   eax, eax
        je     loc_3_3814
loc_3_38DD:
        mov    ax, word ptr [ebp - 0xc]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_38E8:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [0]            ; -> obj1:0x1918
        test   esi, esi
        je     loc_3_393F
        mov    edx, dword ptr [esp + 0x10]
loc_3_38F9:
        mov    ecx, dword ptr [esi + 0x10]
        test   ecx, ecx
        je     loc_3_3939
loc_3_3900:
        cmp    edx, ecx
        je     loc_3_3933
        lea    eax, [ecx + 0x48]
        lea    edi, [edx + 0x48]
loc_3_390A:
        mov    bl, byte ptr [eax]
        cmp    bl, byte ptr [edi]
        jne    loc_3_392A
        or     bl, bl
        je     loc_3_3926
        mov    bl, byte ptr [eax + 1]
        cmp    bl, byte ptr [edi + 1]
        jne    loc_3_392A
        add    eax, 2
        add    edi, 2
        or     bl, bl
        jne    loc_3_390A
loc_3_3926:
        xor    eax, eax
        jmp    loc_3_392F
loc_3_392A:
        sbb    eax, eax
        sbb    eax, -1
loc_3_392F:
        test   eax, eax
        je     loc_3_3948
loc_3_3933:
        mov    ecx, dword ptr [ecx]
        test   ecx, ecx
        jne    loc_3_3900
loc_3_3939:
        mov    esi, dword ptr [esi]
        test   esi, esi
        jne    loc_3_38F9
loc_3_393F:
        mov    eax, 1
loc_3_3944:
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_3_3948:
        xor    eax, eax
        jmp    loc_3_3944
sub_3_394C:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        sub    esp, 0x44
        mov    eax, 0                        ; -> obj1:0x0044
        push   ebx
        push   esi
        mov    byte ptr [ebp - 0x44], cl
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 4], ecx
        cmp    byte ptr [0], cl              ; -> obj1:0x0044
        je     loc_3_397B
loc_3_396D:
        mov    dl, byte ptr [eax]
        inc    dword ptr [ebp - 4]
        inc    eax
        mov    byte ptr [esi], dl
        inc    esi
        cmp    byte ptr [eax], 0
        jne    loc_3_396D
loc_3_397B:
        mov    ebx, 0                        ; -> obj1:0x004C
        mov    eax, dword ptr [ebp + 8]
        push   0x15
        mov    edi, dword ptr [eax + 0x18]
        push   edi
        call   sub_3_4C91
        add    esp, 8
        test   eax, eax
        jne    loc_3_3A0B
        push   edi
        call   sub_3_4CF1
        add    esp, 4
        test   eax, eax
        jne    loc_3_3A0B
        cmp    word ptr [0x10], 2            ; -> obj5:0x03E4
        jbe    loc_3_3A0B
        cmp    byte ptr [0x16], 0            ; -> obj5:0x03EA
        je     loc_3_3A0B
        mov    ebx, 0x16                     ; -> obj5:0x03EA
        mov    edx, 0xffffffff
        movzx  eax, word ptr [0x10]          ; -> obj5:0x03E4
        mov    edi, ebx
        mov    ecx, edx
        mov    byte ptr [eax + 0x14], 0      ; -> obj5:0x03E8
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        add    ecx, edx
        movzx  eax, cx
        movzx  ecx, word ptr [0x10]          ; -> obj5:0x03E4
        sub    ecx, 2
        cmp    eax, ecx
        jge    loc_3_3A07
        push   0x40
        mov    edi, ebx
        mov    ecx, edx
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        add    ecx, ebx
        lea    eax, [ebp - 0x44]
        push   ecx
        push   eax
        call   sub_3_3784
        add    esp, 0xc
        jmp    loc_3_3A0B
loc_3_3A07:
        mov    byte ptr [ebp - 0x44], 0
loc_3_3A0B:
        cmp    dword ptr [ebp - 4], 0x74
        jge    loc_3_3A2F
loc_3_3A11:
        mov    al, byte ptr [ebx]
        test   al, al
        je     loc_3_3A2F
        cmp    al, 0x20
        jl     loc_3_3A25
        cmp    al, 0x7f
        jge    loc_3_3A25
        inc    ebx
        mov    byte ptr [esi], al
        inc    esi
        jmp    loc_3_3A26
loc_3_3A25:
        inc    ebx
loc_3_3A26:
        inc    dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 4], 0x74
        jl     loc_3_3A11
loc_3_3A2F:
        mov    byte ptr [esi], 0x2d
        inc    esi
        cmp    byte ptr [ebp - 0x44], 0
        je     loc_3_3A66
        cmp    dword ptr [ebp - 4], 0x78
        lea    eax, [ebp - 0x44]
        jge    loc_3_3A62
loc_3_3A42:
        mov    cl, byte ptr [eax]
        test   cl, cl
        je     loc_3_3A62
        cmp    cl, 0x20
        jl     loc_3_3A58
        cmp    cl, 0x7f
        jge    loc_3_3A58
        inc    eax
        mov    byte ptr [esi], cl
        inc    esi
        jmp    loc_3_3A59
loc_3_3A58:
        inc    eax
loc_3_3A59:
        inc    dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 4], 0x78
        jl     loc_3_3A42
loc_3_3A62:
        mov    byte ptr [esi], 0x2d
        inc    esi
loc_3_3A66:
        mov    eax, dword ptr [ebp + 0x10]
        test   eax, eax
        je     loc_3_3A72
        lea    ecx, [esi - 1]
        mov    dword ptr [eax], ecx
loc_3_3A72:
        push   dword ptr [ebp + 8]
        call   sub_3_37AC
        add    esp, 4
        push   eax
        push   esi
        call   sub_3_4D76
        add    esp, 8
        mov    esi, eax
        cmp    dword ptr [ebp + 0x14], 0
        jne    loc_3_3A96
        xor    eax, eax
        mov    byte ptr [esi], 0
        jmp    loc_3_3ABF
loc_3_3A96:
        xor    edi, edi
        mov    byte ptr [esi], 0x5c
        mov    byte ptr [esi + 2], 0
        inc    esi
loc_3_3AA0:
        push   dword ptr [ebp + 8]
        lea    eax, [edi + 0x30]
        mov    byte ptr [esi], al
        call   sub_3_38E8
        add    esp, 4
        test   eax, eax
        jne    loc_3_3AC6
        inc    edi
        cmp    edi, 0xa
        jl     loc_3_3AA0
        mov    eax, 0x13
loc_3_3ABF:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_3_3AC6:
        xor    eax, eax
        jmp    loc_3_3ABF
sub_3_3ACA:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        movzx  eax, byte ptr [esi + 0x1e]
        test   eax, eax
        je     loc_3_3AFC
        cmp    eax, 2
        je     loc_3_3B2A
        cmp    eax, 4
        je     loc_3_3C47
        cmp    eax, 6
        je     loc_3_3C4E
        xor    eax, eax
        jmp    loc_3_3C59
loc_3_3AFC:
        movzx  eax, byte ptr [ebp + 0xc]
        cmp    eax, 2
        je     loc_3_3B11
        cmp    eax, 4
        je     loc_3_3B1C
        xor    eax, eax
        jmp    loc_3_3C59
loc_3_3B11:
        push   esi
        call   sub_3_2DC0
        add    esp, 4
        jmp    loc_3_3B35
loc_3_3B1C:
        push   esi
        call   sub_3_2D23
        add    esp, 4
        jmp    loc_3_3C59
loc_3_3B2A:
        mov    al, byte ptr [ebp + 0xc]
        sub    al, 2
        cmp    al, 1
        sbb    eax, eax
        neg    eax
loc_3_3B35:
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     loc_3_3C14
        cmp    byte ptr [esi + 0x1e], 2
        jne    loc_3_3C14
        push   0x22
        push   esi
        call   sub_3_4C91
        add    esp, 8
        test   eax, eax
        jne    loc_3_3B94
        push   esi
        call   sub_3_4CF1
        add    esp, 4
        test   eax, eax
        jne    loc_3_3B94
loc_3_3B68:
        cmp    byte ptr [0x14], 1            ; -> obj5:0x03E8
        jb     loc_3_3B7A
        cmp    byte ptr [0x14], 3            ; -> obj5:0x03E8
        jbe    loc_3_3BE1
loc_3_3B7A:
        push   esi
        call   sub_3_4CDD
        add    esp, 4
        test   eax, eax
        jne    loc_3_3B94
        push   esi
        call   sub_3_4CF1
        add    esp, 4
        test   eax, eax
        je     loc_3_3B68
loc_3_3B94:
        push   0x15
        push   esi
        call   sub_3_4C91
        add    esp, 8
        test   eax, eax
        jne    loc_3_3C40
        push   esi
        call   sub_3_4CF1
        add    esp, 4
        test   eax, eax
        jne    loc_3_3C40
        xor    ebx, ebx
        xor    edi, edi
        movzx  eax, word ptr [0x10]          ; -> obj5:0x03E4
        mov    ecx, 0x16                     ; -> obj5:0x03EA
        add    eax, 0x14                     ; -> obj5:0x03E8
        cmp    eax, ecx
        jbe    loc_3_3C14
loc_3_3BD1:
        mov    dl, byte ptr [ecx]
        test   dl, dl
        jne    loc_3_3BEC
        inc    ebx
        cmp    ebx, 4
        jge    loc_3_3C14
        xor    edi, edi
        jmp    loc_3_3C39
loc_3_3BE1:
        mov    byte ptr [esi + 0x1e], 9
        mov    eax, 1
        jmp    loc_3_3C59
loc_3_3BEC:
        cmp    dl, 0xff
        je     loc_3_3C14
        cmp    dl, 0x61
        jb     loc_3_3C00
        cmp    dl, 0x7a
        ja     loc_3_3C00
        sub    dl, 0x20
        mov    byte ptr [ecx], dl
loc_3_3C00:
        test   edi, edi
        je     loc_3_3C1B
        mov    dl, byte ptr [ecx]
        cmp    byte ptr [edi], dl
        jne    loc_3_3C19
        inc    edi
        cmp    byte ptr [edi], 0
        jne    loc_3_3C39
        mov    byte ptr [esi + 0x1e], 9
loc_3_3C14:
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_3_3C59
loc_3_3C19:
        xor    edi, edi
loc_3_3C1B:
        mov    dl, byte ptr [ecx]
        cmp    byte ptr [0], dl              ; -> loc_1_0038
        jne    loc_3_3C2C
        mov    edi, 1                        ; -> loc_1_0039
        jmp    loc_3_3C39
loc_3_3C2C:
        cmp    byte ptr [0], dl              ; -> obj1:0x0040
        jne    loc_3_3C39
        mov    edi, 1                        ; -> obj1:0x0041
loc_3_3C39:
        inc    ecx
        cmp    eax, ecx
        ja     loc_3_3BD1
        jmp    loc_3_3C14
loc_3_3C40:
        mov    eax, 1
        jmp    loc_3_3C59
loc_3_3C47:
        mov    al, byte ptr [ebp + 0xc]
        sub    al, 4
        jmp    loc_3_3C53
loc_3_3C4E:
        mov    al, byte ptr [ebp + 0xc]
        sub    al, 6
loc_3_3C53:
        cmp    al, 1
        sbb    eax, eax
        neg    eax
loc_3_3C59:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_3_3C60:
        test   byte ptr [0xa9], 0xf          ; -> obj5:0x0341
        je     loc_3_3C84
        cmp    dword ptr [6], 0              ; -> obj5:0x029E
        je     loc_3_3CAD
        test   byte ptr [0xe], 1             ; -> obj5:0x02A6
        je     loc_3_3CAD
        cmp    byte ptr [0xd], 0             ; -> obj5:0x02A5
        je     loc_3_3CAD
loc_3_3C84:
        mov    eax, 0xf                      ; -> obj5:0x02A7
loc_3_3C89:
        cmp    word ptr [eax], 0
        jne    loc_3_3CB0
        add    eax, 4
        cmp    eax, 0x1f                     ; -> obj5:0x02B7
        jb     loc_3_3C89
        mov    eax, 0x29                     ; -> obj5:0x02C1
loc_3_3C9E:
        cmp    dword ptr [eax], 0
        jne    loc_3_3CBE
        add    eax, 0x10
        cmp    eax, 0xa9                     ; -> obj5:0x0341
        jb     loc_3_3C9E
loc_3_3CAD:
        xor    eax, eax
loc_3_3CAF:
        ret    
loc_3_3CB0:
        mov    eax, dword ptr [esp + 8]
        mov    byte ptr [eax], 1
        mov    eax, 0x13
        jmp    loc_3_3CAF
loc_3_3CBE:
        mov    eax, dword ptr [esp + 8]
        mov    byte ptr [eax], 1
        mov    eax, 0x13
        jmp    loc_3_3CAF
sub_3_3CCC:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        push   0x15
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_3_4C91
        add    esp, 8
        test   eax, eax
        jne    loc_3_3D37
        push   esi
        call   sub_3_4CF1
        add    esp, 4
        test   eax, eax
        jne    loc_3_3D37
        mov    edi, 0x16                     ; -> obj5:0x03EA
        mov    edx, 0xffffffff
        mov    ecx, edx
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        add    ecx, edx
        je     loc_3_3D37
        lea    edi, [ecx + 0x17]             ; -> obj5:0x03EB
        sub    eax, eax
        mov    ecx, edx
        repne scasb al, byte ptr es:[edi]
        not    ecx
        add    ecx, edx
        je     loc_3_3D37
        mov    byte ptr [ebp - 1], 0
        lea    eax, [ebp - 1]
        push   eax
        push   0                             ; -> loc_3_3C60
        push   esi
        call   sub_3_4320
        movzx  eax, byte ptr [ebp - 1]
        add    esp, 0xc
        jmp    loc_3_3D39
loc_3_3D37:
        xor    eax, eax
loc_3_3D39:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_3D3F:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        add    eax, 0x30
        sub    esp, 8
        mov    dword ptr [ebp - 8], eax
        cmp    byte ptr [eax], 1
        push   ebx
        push   esi
        push   edi
        jbe    loc_3_3E4E
        mov    eax, dword ptr [ebp + 8]
        push   0x15
        mov    esi, dword ptr [eax + 0x18]
        push   esi
        call   sub_3_4C91
        add    esp, 8
        test   eax, eax
        jne    loc_3_3E4E
        push   esi
        call   sub_3_4CF1
        add    esp, 4
        test   eax, eax
        jne    loc_3_3E4E
        mov    edi, 0x16                     ; -> obj5:0x03EA
        mov    edx, 0xffffffff
        mov    ecx, edx
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        add    ecx, edx
        je     loc_3_3E4E
        lea    edi, [ecx + 0x17]             ; -> obj5:0x03EB
        sub    eax, eax
        mov    ecx, edx
        repne scasb al, byte ptr es:[edi]
        not    ecx
        add    ecx, edx
        je     loc_3_3E4E
        push   6
        push   esi
        call   sub_3_4C91
        add    esp, 8
        test   eax, eax
        jne    loc_3_3E4E
        push   esi
        call   sub_3_4CF1
        add    esp, 4
        test   eax, eax
        jne    loc_3_3E4E
        cmp    byte ptr [0x14], 1            ; -> obj5:0x03E8
        jae    loc_3_3DE7
        mov    byte ptr [0x14], 1            ; -> obj5:0x03E8
        jmp    loc_3_3DF7
loc_3_3DE7:
        cmp    byte ptr [0x14], 8            ; -> obj5:0x03E8
        jbe    loc_3_3DF7
        mov    byte ptr [0x14], 8            ; -> obj5:0x03E8
loc_3_3DF7:
        mov    eax, dword ptr [ebp - 8]
        mov    bl, byte ptr [eax]
        cmp    byte ptr [0x14], bl           ; -> obj5:0x03E8
        je     loc_3_3E09
        mov    byte ptr [eax], 1
        jmp    loc_3_3E4E
loc_3_3E09:
        mov    byte ptr [ebp - 1], 0
        mov    byte ptr [ebp - 5], 0
        test   bl, bl
        je     loc_3_3E48
loc_3_3E15:
        lea    eax, [ebp - 1]
        push   eax
        push   0                             ; -> loc_3_3C60
        push   esi
        call   sub_3_4320
        add    esp, 0xc
        cmp    byte ptr [ebp - 1], 0
        jne    loc_3_3E3A
        add    esi, 0x28
        inc    byte ptr [ebp - 5]
        cmp    byte ptr [ebp - 5], bl
        jb     loc_3_3E15
        jmp    loc_3_3E48
loc_3_3E3A:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [esi + 0x1e], 0xa
        mov    dword ptr [eax + 0x3c], 0     ; -> obj5:0x01D8
loc_3_3E48:
        movzx  eax, byte ptr [ebp - 1]
        jmp    loc_3_3E50
loc_3_3E4E:
        xor    eax, eax
loc_3_3E50:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_3E57:
        mov    eax, dword ptr [esp + 8]
        movzx  ecx, byte ptr [eax]
        cmp    ecx, 1
        je     loc_3_3E6B
        cmp    ecx, 2
        je     loc_3_3E86
loc_3_3E68:
        xor    eax, eax
loc_3_3E6A:
        ret    
loc_3_3E6B:
        mov    al, byte ptr [eax + 1]
        push   eax
        push   dword ptr [esp + 8]
        call   sub_3_3ACA
        add    esp, 8
        test   eax, eax
        je     loc_3_3E68
        mov    eax, 1
        jmp    loc_3_3E6A
loc_3_3E86:
        push   dword ptr [esp + 4]
        call   sub_3_3CCC
        add    esp, 4
        test   eax, eax
        je     loc_3_3E68
        mov    eax, 1
        jmp    loc_3_3E6A
sub_3_3E9D:
        push   ebx
        xor    ecx, ecx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x10]
        push   0x21
        push   edi
        mov    eax, dword ptr [edi]
        lea    esi, [edi + 0x1e]
        mov    dword ptr [eax + 0x3c], ecx
        mov    byte ptr [esi], cl
        call   sub_3_4C91
        add    esp, 8
        test   eax, eax
        jne    loc_3_3EDC
        push   edi
        call   sub_3_4CF1
        add    esp, 4
        test   eax, eax
        jne    loc_3_3EDC
        cmp    byte ptr [0x14], 9            ; -> obj5:0x03E8
        jae    loc_3_3EDC
        mov    al, byte ptr [0x14]           ; -> obj5:0x03E8
        mov    byte ptr [esi], al
loc_3_3EDC:
        cmp    byte ptr [0], 0               ; -> obj5:0x01DC
        mov    ebx, 0                        ; -> obj5:0x01DC
        je     loc_3_3F00
loc_3_3EEA:
        push   ebx
        push   edi
        call   sub_3_3E57
        add    esp, 8
        test   eax, eax
        jne    loc_3_3F09
        add    ebx, 3
        cmp    byte ptr [ebx], 0
        jne    loc_3_3EEA
loc_3_3F00:
        mov    eax, 0x13
loc_3_3F05:
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_3_3F09:
        mov    eax, dword ptr [edi]
        cmp    byte ptr [esi], 0
        mov    dword ptr [eax + 0x3c], ebx
        jne    loc_3_3F18
        mov    al, byte ptr [ebx + 1]
        mov    byte ptr [esi], al
loc_3_3F18:
        xor    eax, eax
        jmp    loc_3_3F05
sub_3_3F1C:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 0x30], 1
        jb     loc_3_3F67
        push   esi
        call   sub_3_3D3F
        add    esp, 4
        test   eax, eax
        jne    loc_3_3F4A
        push   dword ptr [esi + 0x18]
        call   sub_3_3E9D
        add    esp, 4
        test   eax, eax
        jne    loc_3_3F67
loc_3_3F4A:
        lea    edi, [esi + 0x48]
        push   1
        lea    eax, [esi + 0x44]
        push   eax
        push   edi
        push   esi
        call   sub_3_394C
        add    esp, 0x10
        test   eax, eax
        jne    loc_3_3F67
        or     byte ptr [esi + 0x14], 4
        jmp    loc_3_3F71
loc_3_3F67:
        lea    edi, [esi + 0x48]
        or     byte ptr [esi + 0x14], 2
        mov    byte ptr [edi], 0
loc_3_3F71:
        push   0
        mov    ebx, dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   edi
        push   ebx
        call   sub_3_52E0
        add    esp, 0x10
        cmp    eax, 0x10
        jne    loc_3_3F91
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [ebx], eax
        xor    eax, eax
        jmp    loc_3_3FE3
loc_3_3F91:
        test   eax, eax
        jne    loc_3_3FE0
        mov    ebx, dword ptr [ebx]
        push   0
        push   esi
        push   dword ptr [ebp + 0xc]
        push   ebx
        call   sub_3_5430
        mov    cx, word ptr [esi + 0x10]
        add    esp, 0x10
        mov    word ptr [ebp - 4], cx
        push   0
        mov    word ptr [ebp - 2], 0
        lea    ecx, [ebp - 4]
        push   ecx
        push   4
        push   8
        push   ebx
        call   sub_3_5428
        add    esp, 0x14
        cmp    byte ptr [edi], 0
        je     loc_3_3FDC
        push   ebx
        push   dword ptr [esi + 0x18]
        call   sub_3_4AF9
        add    esp, 8
        test   eax, eax
        jne    loc_3_3FE0
loc_3_3FDC:
        xor    eax, eax
        jmp    loc_3_3FE3
loc_3_3FE0:
        mov    byte ptr [edi], 0
loc_3_3FE3:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_3FEA:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        push   edi
        mov    ecx, dword ptr [esi]
        xor    edi, edi
        mov    edx, dword ptr [esp + 0x14]
        lea    eax, [ecx + 0x48]
loc_3_3FFC:
        mov    bl, byte ptr [eax]
        test   bl, bl
        je     loc_3_4011
        cmp    dword ptr [ecx + 0x44], eax
        jbe    loc_3_4011
        inc    eax
        inc    edi
        mov    byte ptr [edx], bl
        inc    edx
        cmp    edi, 0x6f
        jl     loc_3_3FFC
loc_3_4011:
        cmp    byte ptr [0], 0               ; -> loc_1_0064
        mov    eax, 0                        ; -> loc_1_0064
        je     loc_3_402A
loc_3_401F:
        mov    bl, byte ptr [eax]
        inc    eax
        mov    byte ptr [edx], bl
        inc    edx
        cmp    byte ptr [eax], 0
        jne    loc_3_401F
loc_3_402A:
        mov    al, byte ptr [esi + 0x1f]
        add    al, 0x30
        mov    byte ptr [edx], al
        inc    edx
        mov    eax, dword ptr [esp + 0x18]
        test   eax, eax
        je     loc_3_403C
        mov    dword ptr [eax], edx
loc_3_403C:
        mov    eax, dword ptr [ecx + 0x44]
        cmp    byte ptr [eax], 0
        je     loc_3_404F
loc_3_4044:
        mov    cl, byte ptr [eax]
        inc    eax
        mov    byte ptr [edx], cl
        inc    edx
        cmp    byte ptr [eax], 0
        jne    loc_3_4044
loc_3_404F:
        xor    eax, eax
        pop    edi
        pop    esi
        mov    byte ptr [edx], 0
        pop    ebx
        ret    
sub_3_4058:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x84
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [esi]
        cmp    byte ptr [ebx + 0x48], 0
        je     loc_3_40FA
        push   esi
        call   sub_3_3E9D
        add    esp, 4
        test   eax, eax
        jne    loc_3_40FA
        push   0
        lea    eax, [ebp - 0x84]
        push   eax
        push   esi
        call   sub_3_3FEA
        add    esp, 0xc
        lea    eax, [ebp - 0x84]
        mov    edi, dword ptr [ebp + 0x10]
        push   0
        push   dword ptr [ebp + 0xc]
        push   eax
        push   edi
        call   sub_3_52E0
        add    esp, 0x10
        cmp    eax, 0x10
        jne    loc_3_40B8
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [edi], eax
        xor    eax, eax
        jmp    loc_3_40FF
loc_3_40B8:
        test   eax, eax
        jne    loc_3_40FF
        mov    edi, dword ptr [edi]
        push   0
        push   ebx
        push   dword ptr [ebp + 0xc]
        push   edi
        call   sub_3_5430
        mov    cx, word ptr [ebx + 0x10]
        add    esp, 0x10
        mov    word ptr [ebp - 4], cx
        push   0
        mov    word ptr [ebp - 2], 0
        lea    ecx, [ebp - 4]
        push   ecx
        push   4
        push   8
        push   edi
        call   sub_3_5428
        add    esp, 0x14
        push   edi
        push   esi
        call   sub_3_4AF9
        add    esp, 8
        jmp    loc_3_40FF
loc_3_40FA:
        mov    eax, 0x13
loc_3_40FF:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_4106:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x8c
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [esi]
        lea    edx, [ebx + 0x48]
        mov    al, byte ptr [edx]
        mov    byte ptr [ebp - 8], al
        test   al, al
        je     loc_3_42E5
        lea    edi, [edx + 7]
        mov    ecx, 0                        ; -> obj1:0x006C
        cmp    byte ptr [0], 0               ; -> obj1:0x006C
        je     loc_3_4145
loc_3_4138:
        mov    al, byte ptr [ecx]
        cmp    byte ptr [edi], al
        jne    loc_3_4145
        inc    ecx
        inc    edi
        cmp    byte ptr [ecx], 0
        jne    loc_3_4138
loc_3_4145:
        mov    eax, dword ptr [ebx + 0x44]
        test   eax, eax
        je     loc_3_425D
        cmp    byte ptr [ecx], 0
        je     loc_3_425D
        mov    ecx, dword ptr [esi + 4]
        cmp    dword ptr [ebx + 8], ecx
        jne    loc_3_4189
        cmp    byte ptr [ebp - 8], 0
        lea    edi, [ebp - 0x8c]
        je     loc_3_4178
loc_3_416D:
        mov    cl, byte ptr [edx]
        inc    edx
        mov    byte ptr [edi], cl
        inc    edi
        cmp    byte ptr [edx], 0
        jne    loc_3_416D
loc_3_4178:
        sub    eax, ebx
        mov    byte ptr [edi], 0
        lea    ecx, [ebp + eax - 0xd4]
        mov    dword ptr [ebp - 4], ecx
        jmp    loc_3_419D
loc_3_4189:
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0x8c]
        push   eax
        push   ecx
        push   esi
        call   sub_3_3FEA
        add    esp, 0xc
loc_3_419D:
        mov    eax, dword ptr [ebp - 4]
        cmp    byte ptr [eax], 0x2d
        jne    loc_3_41DD
        xor    edx, edx
        lea    edi, [ebp - 0x85]
        push   edx
        mov    ecx, 0xffffffff
        mov    byte ptr [eax], dl
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        dec    ecx
        lea    eax, [ebp - 0x85]
        push   ecx
        push   eax
        push   1
        push   0                             ; -> loc_1_0084
        push   edx
        push   dword ptr [esi + 4]
        call   sub_3_5330
        add    esp, 0x1c
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx], 0x2d
loc_3_41DD:
        mov    edi, dword ptr [ebp - 4]
        add    edi, 5
        cmp    byte ptr [edi], 0x5c
        jne    loc_3_4202
        push   0
        lea    eax, [ebp - 0x8c]
        push   eax
        mov    byte ptr [edi], 0
        push   dword ptr [esi + 4]
        call   sub_3_5438
        add    esp, 0xc
        mov    byte ptr [edi], 0x5c
loc_3_4202:
        mov    dx, word ptr [esi + 0x1a]
        cmp    dx, 0xffff
        je     loc_3_425D
        xor    ecx, ecx
        lea    eax, [ebp - 0x8c]
loc_3_4215:
        cmp    byte ptr [eax], 0
        je     loc_3_4226
        cmp    dword ptr [ebp - 4], eax
        jbe    loc_3_4226
        inc    eax
        inc    ecx
        cmp    ecx, 0x6a
        jl     loc_3_4215
loc_3_4226:
        push   edx
        mov    byte ptr [eax], 0x2d
        inc    eax
        push   eax
        call   sub_3_4D76
        mov    cx, word ptr [esi + 0x1c]
        add    esp, 8
        mov    byte ptr [eax], 0x2d
        push   ecx
        inc    eax
        push   eax
        call   sub_3_4D76
        add    esp, 8
        lea    ecx, [ebp - 0x8c]
        mov    byte ptr [eax], 0
        push   0
        push   ecx
        push   dword ptr [esi + 4]
        call   sub_3_5438
        add    esp, 0xc
loc_3_425D:
        mov    eax, dword ptr [esi + 4]
        cmp    dword ptr [ebx + 8], eax
        jne    loc_3_427B
        cmp    byte ptr [ebx + 0x30], 2
        jb     loc_3_427B
        push   0
        push   0                             ; -> loc_1_0090
        push   eax
        call   sub_3_5438
        add    esp, 0xc
loc_3_427B:
        movzx  eax, byte ptr [esi + 0x1e]
        xor    edx, edx
        mov    ebx, dword ptr [eax*4]        ; -> obj5:0x0368
        cmp    byte ptr [ebx], dl
        je     loc_3_42B0
        push   edx
        mov    edi, ebx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        dec    ecx
        push   ecx
        push   ebx
        push   1
        push   0                             ; -> obj1:0x009C
        push   edx
        push   dword ptr [esi + 4]
        call   sub_3_5330
        add    esp, 0x1c
loc_3_42B0:
        movzx  eax, byte ptr [esi + 0x1e]
        xor    edx, edx
        mov    ebx, dword ptr [eax*4]        ; -> obj5:0x0394
        cmp    byte ptr [ebx], dl
        je     loc_3_42E5
        push   edx
        mov    edi, ebx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        dec    ecx
        push   ecx
        push   ebx
        push   1
        push   0                             ; -> obj1:0x00A4
        push   edx
        push   dword ptr [esi + 4]
        call   sub_3_5330
        add    esp, 0x1c
loc_3_42E5:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_42EC:
        mov    edx, dword ptr [esp + 8]
        push   edi
        mov    edi, edx
        xor    eax, eax
        mov    ecx, 0x2b
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        stosb  byte ptr es:[edi], al
        mov    eax, dword ptr [esp + 8]
        mov    ecx, dword ptr [eax]
        test   byte ptr [ecx + 0x17], 0x10
        jne    loc_3_431E
        mov    byte ptr [edx + 2], 0x32
        mov    byte ptr [edx + 3], 0x32
        mov    byte ptr [edx + 4], 0x32
        mov    byte ptr [edx + 0xa9], 0x70
loc_3_431E:
        pop    edi
        ret    
sub_3_4320:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    word ptr [ebp - 2], bx
        lea    edi, [ebp - 0x18]
        xor    eax, eax
        push   0                             ; -> obj5:0x01E8
        mov    dword ptr [ebp - 8], ebx
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_3_42EC
        add    esp, 8
        mov    eax, dword ptr [esi]
        mov    edi, dword ptr [eax + 0x14]
        and    edi, 0x10000000
        jne    loc_3_43F7
        cmp    byte ptr [eax + 0x30], 1
        jne    loc_3_437C
        push   0x1a
        push   esi
        call   sub_3_4C91
        add    esp, 8
        test   eax, eax
        je     loc_3_43B3
        mov    eax, 0x13
        jmp    loc_3_44ED
loc_3_437C:
        push   6
        push   esi
        call   sub_3_4C91
        add    esp, 8
        test   eax, eax
        je     loc_3_4395
        mov    eax, 0x13
        jmp    loc_3_44ED
loc_3_4395:
        mov    byte ptr [4], 0x1a            ; -> obj5:0x03C4
        push   esi
        call   sub_3_4CDD
        add    esp, 4
        test   eax, eax
        je     loc_3_43B3
        mov    eax, 0x13
        jmp    loc_3_44ED
loc_3_43B3:
        push   esi
        call   sub_3_4CF1
        add    esp, 4
        test   eax, eax
        je     loc_3_43CA
        mov    eax, 0x13
        jmp    loc_3_44ED
loc_3_43CA:
        lea    eax, [ebp - 0x18]
        lea    ecx, [ebp - 2]
        push   eax
        lea    edx, [ebp - 8]
        push   ecx
        push   edx
        push   0                             ; -> obj5:0x03D4
        call   sub_3_366B
        add    esp, 0x10
        mov    eax, dword ptr [esi + 8]
        movzx  ecx, word ptr [ebp - 2]
        xor    ecx, eax
        and    ecx, 0x3ff
        xor    ecx, eax
        mov    dword ptr [esi + 8], ecx
loc_3_43F7:
        test   edi, edi
        je     loc_3_4414
        push   5
        push   esi
        call   sub_3_4C91
        add    esp, 8
        test   eax, eax
        je     loc_3_446C
        mov    eax, 0x13
        jmp    loc_3_44ED
loc_3_4414:
        mov    eax, dword ptr [esi]
        cmp    byte ptr [eax + 0x30], 1
        jne    loc_3_4435
        push   0x1b
        push   esi
        call   sub_3_4C91
        add    esp, 8
        test   eax, eax
        je     loc_3_446C
        mov    eax, 0x13
        jmp    loc_3_44ED
loc_3_4435:
        push   6
        push   esi
        call   sub_3_4C91
        add    esp, 8
        test   eax, eax
        je     loc_3_444E
        mov    eax, 0x13
        jmp    loc_3_44ED
loc_3_444E:
        mov    byte ptr [4], 0x1b            ; -> obj5:0x03C4
        push   esi
        call   sub_3_4CDD
        add    esp, 4
        test   eax, eax
        je     loc_3_446C
        mov    eax, 0x13
        jmp    loc_3_44ED
loc_3_446C:
        push   esi
        call   sub_3_4CF1
        add    esp, 4
        test   eax, eax
        je     loc_3_4480
        mov    eax, 0x13
        jmp    loc_3_44ED
loc_3_4480:
        mov    ebx, 0                        ; -> obj5:0x0298
loc_3_4485:
        push   ebx
        push   esi
        call   sub_3_42EC
        add    esp, 8
        push   edi
        push   ebx
        push   0                             ; -> obj5:0x03D4
        call   sub_3_3603
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 8]
        mov    cl, byte ptr [ebp - 2]
        mov    dword ptr [6], eax            ; -> obj5:0x029E
        xor    eax, eax
        mov    byte ptr [0xe], cl            ; -> obj5:0x02A6
        mov    al, byte ptr [0xa9]           ; -> obj5:0x0341
        and    eax, 3
        push   dword ptr [ebp + 0x10]
        push   esi
        mov    ecx, dword ptr [ebp + eax*4 - 0x18]
        mov    dword ptr [0xab], ecx         ; -> obj5:0x0343
        call   dword ptr [ebp + 0xc]
        add    esp, 8
        test   eax, eax
        jne    loc_3_44ED
        push   esi
        call   sub_3_4CDD
        add    esp, 4
        test   eax, eax
        jne    loc_3_44EB
        push   esi
        call   sub_3_4CF1
        add    esp, 4
        test   eax, eax
        je     loc_3_4485
loc_3_44EB:
        xor    eax, eax
loc_3_44ED:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_44F4:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x78
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0x10]
        cmp    byte ptr [esi + 2], 0x21
        jbe    loc_3_450F
        add    dword ptr [ebp + 0x14], 0x200
        jmp    loc_3_4518
loc_3_450F:
        jne    loc_3_4518
        add    dword ptr [ebp + 0x14], 0x100
loc_3_4518:
        push   0
        lea    edx, [ebp - 8]
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0xc]
        push   edx
        call   sub_3_5350
        add    esp, 0x10
        test   eax, eax
        jne    loc_3_4775
        lea    edi, [esi + 0xf]
        xor    ebx, ebx
        mov    dword ptr [ebp - 4], 0
loc_3_4540:
        mov    ax, word ptr [edi]
        test   ax, ax
        je     loc_3_45E0
        mov    word ptr [ebp - 0x54], ax
        mov    ecx, dword ptr [esi + 0x1f]
        mov    ax, word ptr [edi + 2]
        mov    dword ptr [ebp - 0x4a], ecx
        mov    word ptr [ebp - 0x62], 1
        mov    word ptr [ebp - 0x60], 0xc
        mov    word ptr [ebp - 0x52], ax
        mov    byte ptr [ebp - 0x58], bl
        mov    word ptr [ebp - 0x5e], bx
        mov    ecx, 1
        mov    word ptr [ebp - 0x5c], bx
        mov    eax, 0xffff
        mov    word ptr [ebp - 0x5a], bx
        mov    byte ptr [ebp - 0x57], bl
        cmp    word ptr [ebp - 0x54], cx
        jbe    loc_3_4598
        movzx  edx, word ptr [ebp - 0x54]
loc_3_4590:
        add    ecx, ecx
        add    eax, eax
        cmp    ecx, edx
        jb     loc_3_4590
loc_3_4598:
        mov    word ptr [ebp - 0x50], 0xffff
        mov    word ptr [ebp - 0x56], ax
        cmp    word ptr [ebp - 0x52], bx
        je     loc_3_45B6
        mov    ax, word ptr [ebp - 0x54]
        add    ax, word ptr [ebp - 0x52]
        dec    ax
        mov    word ptr [ebp - 0x50], ax
loc_3_45B6:
        mov    word ptr [ebp - 0x4e], bx
        push   ebx
        push   0x1c
        lea    eax, [ebp - 0x62]
        push   eax
        lea    edx, [ebp - 0xc]
        push   2
        mov    byte ptr [ebp - 0x4c], bl
        push   dword ptr [ebp - 8]
        push   edx
        mov    byte ptr [ebp - 0x4b], bl
        call   sub_3_5348
        add    esp, 0x18
        test   eax, eax
        jne    loc_3_4775
loc_3_45E0:
        add    edi, 4
        inc    dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 4], 4
        jb     loc_3_4540
        mov    ax, word ptr [esi + 0x23]
        test   ax, ax
        je     loc_3_465D
        mov    word ptr [ebp - 0x42], ax
        mov    edx, dword ptr [esi + 0x25]
        mov    eax, edx
        mov    ecx, dword ptr [ebp + 8]
        mov    word ptr [ebp - 0x46], 0
        and    al, 0x7f
        mov    edi, dword ptr [ecx]
        mov    dword ptr [ebp - 0x3e], eax
        mov    eax, dword ptr [edi + 8]
        test   eax, eax
        je     loc_3_462A
        cmp    dword ptr [ebp + 0xc], eax
        je     loc_3_462A
        test   byte ptr [edi + 0x14], 0x40
        je     loc_3_462A
        mov    word ptr [ebp - 0x46], 1
loc_3_462A:
        test   dl, 0x80
        je     loc_3_4633
        or     byte ptr [ebp - 0x46], 2
loc_3_4633:
        xor    edi, edi
        lea    eax, [ebp - 0x46]
        mov    word ptr [ebp - 0x44], di
        push   edi
        mov    word ptr [ebp - 0x40], di
        push   0xc
        push   eax
        lea    edx, [ebp - 0xc]
        push   4
        push   dword ptr [ebp - 8]
        push   edx
        call   sub_3_5348
        add    esp, 0x18
        test   eax, eax
        jne    loc_3_4775
loc_3_465D:
        lea    edi, [esi + 0x29]
        xor    ebx, ebx
        mov    dword ptr [ebp - 4], 0
loc_3_4669:
        mov    eax, dword ptr [edi]
        test   eax, eax
        je     loc_3_46EC
        mov    ecx, dword ptr [edi + 8]
        mov    edx, dword ptr [edi + 0xc]
        mov    dword ptr [ebp - 0x26], eax
        mov    dword ptr [ebp - 0x1e], ecx
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [ebp - 0x12], edx
        mov    dword ptr [ebp - 0x22], eax
        mov    dword ptr [ebp - 0x36], ebx
        mov    word ptr [ebp - 0x2e], bx
        mov    dword ptr [ebp - 0x32], ebx
        mov    word ptr [ebp - 0x2c], bx
        mov    ecx, 1
        mov    word ptr [ebp - 0x3a], 1
        mov    dword ptr [ebp - 0x16], 8
        mov    eax, 0xffffffff
        cmp    dword ptr [ebp - 0x26], ecx
        mov    word ptr [ebp - 0x38], 0x14
        jbe    loc_3_46C1
loc_3_46B4:
        test   ecx, ecx
        je     loc_3_46C1
        add    ecx, ecx
        add    eax, eax
        cmp    ecx, dword ptr [ebp - 0x26]
        jb     loc_3_46B4
loc_3_46C1:
        mov    word ptr [ebp - 0x1a], bx
        push   ebx
        mov    word ptr [ebp - 0x18], bx
        push   0x2c
        lea    edx, [ebp - 0xc]
        mov    dword ptr [ebp - 0x2a], eax
        lea    eax, [ebp - 0x3a]
        push   eax
        push   1
        push   dword ptr [ebp - 8]
        push   edx
        call   sub_3_5348
        add    esp, 0x18
        test   eax, eax
        jne    loc_3_4775
loc_3_46EC:
        add    edi, 0x10
        inc    dword ptr [ebp - 4]
        cmp    dword ptr [ebp - 4], 8
        jb     loc_3_4669
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [eax]
        mov    dx, word ptr [ecx + 0x10]
        mov    cl, byte ptr [esi + 2]
        mov    ax, word ptr [esi]
        mov    byte ptr [ebp - 0x74], cl
        mov    word ptr [ebp - 0x78], dx
        mov    cl, byte ptr [esi + 5]
        mov    word ptr [ebp - 0x76], ax
        mov    dl, byte ptr [esi + 3]
        mov    al, byte ptr [esi + 4]
        mov    byte ptr [ebp - 0x73], dl
        mov    edx, dword ptr [esi + 6]
        mov    byte ptr [ebp - 0x72], al
        mov    al, byte ptr [esi + 0xa]
        mov    byte ptr [ebp - 0x71], cl
        mov    cl, byte ptr [esi + 0xb]
        mov    dword ptr [ebp - 0x70], edx
        mov    dl, byte ptr [esi + 0xc]
        mov    byte ptr [ebp - 0x6c], al
        mov    al, byte ptr [esi + 0xd]
        mov    byte ptr [ebp - 0x6b], cl
        mov    cl, byte ptr [esi + 0xe]
        mov    byte ptr [ebp - 0x6a], dl
        mov    edx, dword ptr [esi + 0xab]
        push   0
        push   0x16
        mov    byte ptr [ebp - 0x69], al
        lea    eax, [ebp - 0x78]
        mov    byte ptr [ebp - 0x68], cl
        push   eax
        mov    dword ptr [ebp - 0x66], edx
        push   0x897c
        lea    edx, [ebp - 0xc]
        push   dword ptr [ebp - 8]
        push   edx
        mov    byte ptr [ebp - 0x67], 0
        call   sub_3_5348
        add    esp, 0x18
loc_3_4775:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_477C:
        sub    esp, 4
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0x10]
        mov    cl, byte ptr [esi + 0xa9]
        and    cl, 0xf
        je     loc_3_47A9
        cmp    dword ptr [esi + 6], 0
        je     loc_3_47A2
        test   byte ptr [esi + 0xe], 1
        je     loc_3_47A2
        cmp    byte ptr [esi + 0xd], 0
        jne    loc_3_47A9
loc_3_47A2:
        mov    eax, 0x13
        jmp    loc_3_480F
loc_3_47A9:
        xor    eax, eax
        mov    word ptr [esi], ax
        cmp    word ptr [esi + 0x23], ax
        je     loc_3_47BF
        mov    word ptr [esi + 0x23], 0xffff
        mov    word ptr [esi], 2
loc_3_47BF:
        mov    byte ptr [esi + 0xa], 0
        movzx  eax, cl
        mov    bl, byte ptr [esi + 0xe]
        mov    al, byte ptr [eax]            ; -> loc_1_00B4
        test   bl, 2
        mov    byte ptr [esi + 5], al
        je     loc_3_47FA
        lea    eax, [esp + 8]
        push   0
        push   eax
        call   sub_3_1180
        add    esp, 8
        test   byte ptr [esp + 8], 0x10
        jne    loc_3_47FA
        test   byte ptr [esi + 0xaa], 8
        je     loc_3_47FA
        mov    byte ptr [esi + 0xa], 8
loc_3_47FA:
        mov    byte ptr [esi + 0xb], 0
        and    bl, 0xfb
        test   bl, 8
        mov    byte ptr [esi + 0xe], bl
        je     loc_3_480D
        mov    byte ptr [esi + 0xc], 0
loc_3_480D:
        xor    eax, eax
loc_3_480F:
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_3_4815:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        push   0                             ; -> obj5:0x0298
        call   sub_3_477C
        add    esp, 4
        test   eax, eax
        je     loc_3_4834
        xor    eax, eax
        jmp    loc_3_48E5
loc_3_4834:
        xor    ecx, ecx
        xor    edx, edx
        mov    eax, 0xf                      ; -> obj5:0x02A7
loc_3_483D:
        cmp    word ptr [eax], 0
        je     loc_3_4852
        mov    ecx, 1
        cmp    word ptr [eax + 2], cx
        sbb    esi, esi
        add    esi, ecx
        or     edx, esi
loc_3_4852:
        add    eax, 4
        cmp    eax, 0x1f                     ; -> obj5:0x02B7
        jb     loc_3_483D
        mov    ebx, 0x2000
        test   edx, edx
        jne    loc_3_4876
        cmp    ecx, 1
        sbb    ebx, ebx
        and    ebx, 0x2000
        add    ebx, 0x3000
loc_3_4876:
        mov    edi, dword ptr [esp + 0x1c]
        push   ebx
        mov    esi, dword ptr [esp + 0x1c]
        push   0                             ; -> obj5:0x0298
        and    byte ptr [0x25], 0x7f         ; -> obj5:0x02BD
        push   dword ptr [edi]
        push   esi
        call   sub_3_44F4
        add    esp, 0x10
        mov    ebp, eax
        test   ebp, ebp
        jne    loc_3_48E5
        lea    eax, [esp + 0x10]
        push   eax
        push   dword ptr [esi]
        call   sub_1_2CF8
loc_3_48A8:
        add    esp, 8
        movzx  ecx, word ptr [0x23]          ; -> obj5:0x02BB
        test   dword ptr [esp + 0x10], ecx
        je     loc_3_48E3
        mov    eax, dword ptr [esp + 0x10]
        add    ebx, 0x800
        or     byte ptr [0x25], 0x80         ; -> obj5:0x02BD
        and    word ptr [0x23], ax           ; -> obj5:0x02BB
        push   ebx
        push   0                             ; -> obj5:0x0298
        push   dword ptr [edi]
        push   esi
        call   sub_3_44F4
        add    esp, 0x10
        mov    ebp, eax
loc_3_48E3:
        mov    eax, ebp
loc_3_48E5:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_3_48ED:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        cmp    word ptr [0x23], 0            ; -> obj5:0x02BB
        push   ebx
        push   esi
        je     loc_3_492E
        mov    esi, dword ptr [ebp + 0xc]
        lea    eax, [ebp - 1]
        mov    byte ptr [ebp - 1], 0
        mov    bl, byte ptr [esi]
        push   eax
        push   dword ptr [ebp + 8]
        or     bl, 1
        mov    byte ptr [esi], bl
        call   sub_3_2D49
        add    esp, 8
        cmp    byte ptr [ebp - 1], 0
        je     loc_3_492E
        or     bl, 2
        mov    eax, 0x13
        mov    byte ptr [esi], bl
        jmp    loc_3_4930
loc_3_492E:
        xor    eax, eax
loc_3_4930:
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_4936:
        mov    ecx, dword ptr [esp + 4]
        movzx  eax, byte ptr [ecx + 0x1e]
        cmp    eax, 2
        je     loc_3_494D
        cmp    eax, 9
        mov    eax, 0x13
        jne    loc_3_495A
loc_3_494D:
        push   dword ptr [esp + 8]
        push   ecx
        call   sub_3_2FB8
        add    esp, 8
loc_3_495A:
        ret    
sub_3_495B:
        lea    eax, [esp + 8]
        push   eax
        push   0                             ; -> loc_3_4815
        push   dword ptr [esp + 0xc]
        call   sub_3_4320
        add    esp, 0xc
        ret    
sub_3_4972:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [eax]
        mov    al, byte ptr [esi + 0x30]
        cmp    al, 1
        ja     loc_3_4992
        mov    eax, 0x13
        jmp    loc_3_4AF2
loc_3_4992:
        xor    edi, edi
        mov    ebx, dword ptr [esi + 0x18]
        mov    byte ptr [ebp - 2], 0
        mov    byte ptr [ebp - 3], 0
        test   al, al
        je     loc_3_49D6
loc_3_49A3:
        mov    byte ptr [ebp - 1], 0
        lea    eax, [ebp - 1]
        push   eax
        push   0                             ; -> loc_3_48ED
        push   ebx
        call   sub_3_4320
        add    esp, 0xc
        test   byte ptr [ebp - 1], 1
        je     loc_3_49C0
        inc    edi
loc_3_49C0:
        mov    al, byte ptr [ebp - 1]
        add    ebx, 0x28
        and    al, 2
        inc    byte ptr [ebp - 3]
        mov    cl, byte ptr [ebp - 3]
        or     byte ptr [ebp - 2], al
        cmp    byte ptr [esi + 0x30], cl
        ja     loc_3_49A3
loc_3_49D6:
        cmp    edi, 2
        jb     loc_3_4ACF
        or     byte ptr [esi + 0x14], 0x40
        mov    edi, 0                        ; -> obj5:0x0298
        xor    ebx, ebx
        push   edi
        push   dword ptr [ebp + 8]
        call   sub_3_42EC
        mov    word ptr [0], 2               ; -> obj5:0x0298
        add    esp, 8
        mov    byte ptr [2], bl              ; -> obj5:0x029A
        mov    byte ptr [3], bl              ; -> obj5:0x029B
        mov    byte ptr [4], bl              ; -> obj5:0x029C
        mov    byte ptr [5], 2               ; -> obj5:0x029D
        test   byte ptr [ebp - 2], 2
        mov    dword ptr [0x25], ebx         ; -> obj5:0x02BD
        je     loc_3_4A49
        mov    word ptr [0x23], 0x18         ; -> obj5:0x02BB
        push   0x2000
        push   edi
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_3_44F4
        add    esp, 0x10
        test   eax, eax
        jne    loc_3_4AF2
loc_3_4A49:
        mov    word ptr [0x23], 0xffff       ; -> obj5:0x02BB
        push   0x3000
        push   0                             ; -> obj5:0x0298
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_3_44F4
        add    esp, 0x10
        test   eax, eax
        jne    loc_3_4AF2
        lea    eax, [ebp - 8]
        mov    ecx, dword ptr [ebp + 8]
        push   eax
        push   dword ptr [ecx]
        call   sub_1_2CF8
loc_3_4A80:
        movzx  ecx, word ptr [0x23]          ; -> obj5:0x02BB
        add    esp, 8
        test   dword ptr [ebp - 8], ecx
        je     loc_3_4AB8
        mov    eax, dword ptr [ebp - 8]
        push   0x3800
        or     byte ptr [0x25], 0x80         ; -> obj5:0x02BD
        and    word ptr [0x23], ax           ; -> obj5:0x02BB
        push   0                             ; -> obj5:0x0298
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_3_44F4
        add    esp, 0x10
loc_3_4AB8:
        push   0
        push   dword ptr [ebp + 0xc]
        push   esi
        push   0                             ; -> loc_3_1B24
        push   4
        push   0
        call   sub_3_5440
        add    esp, 0x18
loc_3_4ACF:
        push   0
        push   esi
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 0xc]
        call   sub_3_5430
        add    esp, 0x10
        push   1
        push   0                             ; -> loc_3_18A1
        push   dword ptr [ebp + 0xc]
        call   sub_3_5288
        add    esp, 0xc
loc_3_4AF2:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_4AF9:
        push   ebp
        mov    ecx, dword ptr [esp + 8]
        mov    ebp, esp
        mov    eax, dword ptr [ecx]
        mov    edx, dword ptr [eax + 0x3c]
        movzx  eax, byte ptr [edx + 2]
        cmp    eax, 1
        je     loc_3_4B1F
        cmp    eax, 2
        je     loc_3_4B2A
        cmp    eax, 3
        je     loc_3_4B35
        mov    eax, 0x13
        jmp    loc_3_4B3E
loc_3_4B1F:
        push   dword ptr [ebp + 0xc]
        push   ecx
        call   sub_3_4936
        jmp    loc_3_4B3E
loc_3_4B2A:
        push   dword ptr [ebp + 0xc]
        push   ecx
        call   sub_3_495B
        jmp    loc_3_4B3E
loc_3_4B35:
        push   dword ptr [ebp + 0xc]
        push   ecx
        call   sub_3_4972
loc_3_4B3E:
        mov    esp, ebp
        pop    ebp
        ret    
loc_3_4B42:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        cmp    byte ptr [esi + 3], 0
        jne    loc_3_4BA9
        mov    al, byte ptr [esi]
        test   al, al
        je     loc_3_4B61
        cmp    byte ptr [2], al              ; -> obj5:0x029A
        jae    loc_3_4BA9
loc_3_4B61:
        lea    eax, [ebp - 2]
        mov    cl, byte ptr [4]              ; -> obj5:0x029C
        push   eax
        mov    dl, byte ptr [3]              ; -> obj5:0x029B
        push   ecx
        mov    al, byte ptr [2]              ; -> obj5:0x029A
        push   edx
        mov    edi, dword ptr [ebp + 8]
        push   eax
        push   dword ptr [edi]
        call   sub_1_653C
loc_3_4B83:
        add    esp, 0x14
        test   eax, eax
        jne    loc_3_4C18
        mov    al, byte ptr [2]              ; -> obj5:0x029A
        mov    byte ptr [esi], al
        mov    cl, byte ptr [3]              ; -> obj5:0x029B
        mov    byte ptr [esi + 1], cl
        mov    dl, byte ptr [4]              ; -> obj5:0x029C
        mov    byte ptr [esi + 2], dl
        jmp    loc_3_4C18
loc_3_4BA9:
        mov    al, byte ptr [2]              ; -> obj5:0x029A
        cmp    byte ptr [esi], al
        jne    loc_3_4C18
        mov    al, byte ptr [3]              ; -> obj5:0x029B
        cmp    byte ptr [esi + 1], al
        jae    loc_3_4BE6
        lea    eax, [ebp - 2]
        mov    cl, byte ptr [3]              ; -> obj5:0x029B
        push   eax
        mov    edi, dword ptr [ebp + 8]
        push   0
        push   ecx
        push   0
        push   dword ptr [edi]
        call   sub_1_653C
loc_3_4BD5:
        add    esp, 0x14
        test   eax, eax
        jne    loc_3_4BE9
        mov    al, byte ptr [3]              ; -> obj5:0x029B
        mov    byte ptr [esi + 1], al
        jmp    loc_3_4BE9
loc_3_4BE6:
        mov    edi, dword ptr [ebp + 8]
loc_3_4BE9:
        mov    al, byte ptr [4]              ; -> obj5:0x029C
        cmp    byte ptr [esi + 2], al
        jae    loc_3_4C18
        lea    eax, [ebp - 2]
        mov    cl, byte ptr [4]              ; -> obj5:0x029C
        push   eax
        push   ecx
        push   0
        push   0
        push   dword ptr [edi]
        call   sub_1_653C
loc_3_4C09:
        add    esp, 0x14
        test   eax, eax
        jne    loc_3_4C18
        mov    al, byte ptr [4]              ; -> obj5:0x029C
        mov    byte ptr [esi + 2], al
loc_3_4C18:
        xor    eax, eax
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_4C20:
        push   ebp
        mov    edx, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        mov    ecx, dword ptr [edx]
        push   ebx
        mov    byte ptr [ebp - 1], 0
        xor    bl, bl
        mov    eax, dword ptr [ecx + 0x18]
        mov    cl, byte ptr [ecx + 0x30]
        cmp    cl, bl
        je     loc_3_4C54
loc_3_4C3D:
        cmp    byte ptr [eax + 0x14], 0
        jne    loc_3_4C4E
        inc    bl
        add    eax, 0x28
        cmp    cl, bl
        ja     loc_3_4C3D
        jmp    loc_3_4C54
loc_3_4C4E:
        mov    al, byte ptr [eax + 0x14]
        mov    byte ptr [ebp - 1], al
loc_3_4C54:
        mov    al, byte ptr [ebp - 1]
        lea    ecx, [ebp - 4]
        push   ecx
        mov    byte ptr [ebp - 4], al
        push   0                             ; -> loc_3_4B42
        mov    byte ptr [ebp - 3], al
        push   edx
        mov    byte ptr [ebp - 2], al
        call   sub_3_4320
        add    esp, 0xc
        mov    cl, byte ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 0xc]
        mov    bl, byte ptr [ebp - 3]
        mov    eax, dword ptr [ebp + 0x10]
        mov    byte ptr [edx], cl
        mov    cl, byte ptr [ebp - 2]
        mov    edx, dword ptr [ebp + 0x14]
        mov    byte ptr [eax], bl
        xor    eax, eax
        pop    ebx
        mov    byte ptr [edx], cl
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_4C91:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx]
        mov    ax, word ptr [eax + 0x10]
        mov    word ptr [0], ax              ; -> obj5:0x03C0
        mov    dl, byte ptr [ecx + 0x1f]
        mov    byte ptr [1], dl              ; -> obj5:0x03C1
        mov    dl, byte ptr [esp + 8]
        mov    byte ptr [5], 0               ; -> obj5:0x03C5
        lea    eax, [edx - 0xff]
        push   0                             ; -> obj5:0x03C0
        cmp    al, 1
        mov    byte ptr [4], dl              ; -> obj5:0x03C4
        sbb    eax, eax
        neg    eax
        mov    word ptr [2], ax              ; -> obj5:0x03C2
        push   dword ptr [ecx]
        call   sub_1_384D
loc_3_4CD9:
        add    esp, 8
        ret    
sub_3_4CDD:
        mov    eax, dword ptr [esp + 4]
        push   0                             ; -> obj5:0x03C0
        push   dword ptr [eax]
        call   sub_1_3906
loc_3_4CED:
        add    esp, 8
        ret    
sub_3_4CF1:
        mov    ax, word ptr [0]              ; -> obj5:0x03C0
        mov    dl, byte ptr [4]              ; -> obj5:0x03C4
        mov    cx, word ptr [2]              ; -> obj5:0x03C2
        mov    byte ptr [4], dl              ; -> obj5:0x03D8
        mov    word ptr [0], ax              ; -> obj5:0x03D4
        mov    edx, dword ptr [0xc]          ; -> obj5:0x03CC
        mov    ax, word ptr [6]              ; -> obj5:0x03C6
        mov    dword ptr [0xc], edx          ; -> obj5:0x03E0
        mov    word ptr [2], cx              ; -> obj5:0x03D6
        push   0                             ; -> obj5:0x03D4
        mov    word ptr [6], ax              ; -> obj5:0x03DA
        mov    ecx, dword ptr [8]            ; -> obj5:0x03C8
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [8], ecx            ; -> obj5:0x03DC
        mov    byte ptr [5], 0               ; -> obj5:0x03D9
        mov    word ptr [0x10], 0x80         ; -> obj5:0x03E4
        push   dword ptr [eax]
        call   sub_1_3967
loc_3_4D5B:
        mov    cx, word ptr [0x12]           ; -> obj5:0x03E6
        add    esp, 8
        cmp    word ptr [0x10], cx           ; -> obj5:0x03E4
        jbe    loc_3_4D75
        mov    word ptr [0x10], cx           ; -> obj5:0x03E4
loc_3_4D75:
        ret    
sub_3_4D76:
        mov    edx, 4
        mov    eax, dword ptr [esp + 4]
loc_3_4D7F:
        mov    cx, word ptr [esp + 8]
        shr    cx, 0xc
        shl    word ptr [esp + 8], 4
        movzx  ecx, cx
        mov    cl, byte ptr [ecx]            ; -> loc_1_00C4
        mov    byte ptr [eax], cl
        inc    eax
        dec    edx
        jne    loc_3_4D7F
        ret    
sub_3_4D9E:
        push   ebx
        mov    edx, dword ptr [0]            ; -> obj1:0x1918
        push   esi
        test   edx, edx
        push   edi
        je     loc_3_4DF8
        mov    ecx, dword ptr [esp + 0x10]
loc_3_4DAF:
        mov    edi, dword ptr [edx + 0x10]
        test   edi, edi
        je     loc_3_4DF2
loc_3_4DB6:
        mov    eax, dword ptr [edi + 0x2c]
        test   eax, eax
        jne    loc_3_4DC4
        mov    eax, dword ptr [edi + 0x24]
        test   eax, eax
        je     loc_3_4DEC
loc_3_4DC4:
        test   byte ptr [eax + 0xb], 3
        jne    loc_3_4DE6
        cmp    dword ptr [eax + 0x2c], 0
        je     loc_3_4DE6
        cmp    ecx, eax
        je     loc_3_4DE6
        mov    si, word ptr [eax + 0x1a]
        cmp    word ptr [ecx + 0x1a], si
        jne    loc_3_4DE6
        mov    bl, byte ptr [eax + 0x30]
        cmp    byte ptr [ecx + 0x30], bl
        je     loc_3_4DFA
loc_3_4DE6:
        mov    eax, dword ptr [eax]
        test   eax, eax
        jne    loc_3_4DC4
loc_3_4DEC:
        mov    edi, dword ptr [edi]
        test   edi, edi
        jne    loc_3_4DB6
loc_3_4DF2:
        mov    edx, dword ptr [edx]
        test   edx, edx
        jne    loc_3_4DAF
loc_3_4DF8:
        xor    eax, eax
loc_3_4DFA:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_3_4DFE:
        mov    ecx, dword ptr [esp + 8]
        push   ebx
        push   esi
        mov    eax, 0                        ; -> obj5:0x04E4
        cmp    byte ptr [0], 0               ; -> obj5:0x04E4
        push   edi
        push   ebp
        je     loc_3_4E1F
loc_3_4E14:
        mov    dl, byte ptr [eax]
        inc    eax
        mov    byte ptr [ecx], dl
        inc    ecx
        cmp    byte ptr [eax], 0
        jne    loc_3_4E14
loc_3_4E1F:
        mov    esi, dword ptr [esp + 0x14]
        mov    ax, word ptr [esi + 0x1a]
        push   eax
        push   ecx
        xor    ebx, ebx
        call   sub_3_4D76
        mov    edi, dword ptr [esp + 0x24]
        mov    byte ptr [eax], 0x5c
        mov    byte ptr [eax + 2], bl
        add    esp, 8
        mov    ebp, eax
        mov    dword ptr [edi], ebx
        inc    ebp
loc_3_4E42:
        lea    eax, [ebx + 0x30]
        push   esi
        mov    byte ptr [ebp], al
        mov    byte ptr [esi + 0x30], bl
        call   sub_3_4D9E
        add    esp, 4
        test   eax, eax
        je     loc_3_4E6F
        test   bl, bl
        jne    loc_3_4E5E
        mov    dword ptr [edi], eax
loc_3_4E5E:
        inc    bl
        cmp    bl, 0xa
        jl     loc_3_4E42
        mov    eax, 0x13
loc_3_4E6A:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_3_4E6F:
        xor    eax, eax
        jmp    loc_3_4E6A
sub_3_4E73:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        mov    edi, dword ptr [ebp + 8]
        cmp    dword ptr [edi + 0x2c], esi
        je     loc_3_4F7E
        push   esi
        mov    ebx, 0x80
        push   ebx
        call   sub_3_53A0
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        cmp    eax, esi
        je     loc_3_4F7E
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        mov    dword ptr [ebp - 8], ebx
        push   dword ptr [ebp - 4]
        push   1
        push   0                             ; -> obj5:0x04F0
        push   esi
        push   dword ptr [edi + 0x2c]
        call   sub_3_5298
        add    esp, 0x1c
        test   eax, eax
        jne    loc_3_4F71
        push   esi
        mov    edx, dword ptr [ebp + 0xc]
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 4]
        push   1
        push   0                             ; -> obj5:0x04FC
        push   esi
        push   dword ptr [edx + 0x2c]
        call   sub_3_5330
        add    esp, 0x1c
        test   eax, eax
        jne    loc_3_4F71
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        mov    dword ptr [ebp - 8], ebx
        push   dword ptr [ebp - 4]
        push   1
        push   0                             ; -> obj5:0x0508
        push   esi
        push   dword ptr [edi + 0x2c]
        call   sub_3_5298
        add    esp, 0x1c
        test   eax, eax
        jne    loc_3_4F71
        push   esi
        mov    edx, dword ptr [ebp + 0xc]
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 4]
        push   1
        push   0                             ; -> obj5:0x0510
        push   esi
        push   dword ptr [edx + 0x2c]
        call   sub_3_5330
        add    esp, 0x1c
        test   eax, eax
        jne    loc_3_4F71
        push   esi
        mov    dword ptr [ebp - 8], ebx
        lea    ebx, [ebp - 8]
        push   ebx
        push   dword ptr [ebp - 4]
        push   1
        push   0                             ; -> obj5:0x0518
        push   esi
        push   dword ptr [edi + 0x2c]
        call   sub_3_5298
        add    esp, 0x1c
        test   eax, eax
        jne    loc_3_4F71
        push   esi
        mov    edx, dword ptr [ebp + 0xc]
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 4]
        push   1
        push   0                             ; -> obj5:0x0524
        push   esi
        push   dword ptr [edx + 0x2c]
        call   sub_3_5330
        add    esp, 0x1c
loc_3_4F71:
        push   0
        push   dword ptr [ebp - 4]
        call   sub_3_53A8
        add    esp, 8
loc_3_4F7E:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_4F85:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x2c
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [esi + 0x2c]
        test   edi, edi
        jne    loc_3_4FA3
        mov    edi, dword ptr [esi + 0x24]
        test   edi, edi
        je     loc_3_50E9
loc_3_4FA3:
        mov    dword ptr [ebp - 0x10], 0x10000
        lea    eax, [ebp - 0x14]
        push   eax
        push   0                             ; -> obj5:0x000C
        push   0x80000002
        call   sub_3_5328
        add    esp, 0xc
        test   eax, eax
        jne    loc_3_4FF2
        lea    eax, [ebp - 0x18]
        lea    ecx, [ebp - 0x10]
        mov    dword ptr [ebp - 0x18], 4
        push   eax
        push   ecx
        push   0
        push   0
        push   0                             ; -> obj5:0x0530
        push   dword ptr [ebp - 0x14]
        call   sub_3_5320
        add    esp, 0x18
        push   dword ptr [ebp - 0x14]
        call   sub_3_5318
        add    esp, 4
loc_3_4FF2:
        test   edi, edi
        je     loc_3_50EE
        xor    ebx, ebx
loc_3_4FFC:
        test   byte ptr [edi + 0xb], 3
        jne    loc_3_50DD
        mov    eax, dword ptr [ebp - 0x10]
        cmp    dword ptr [edi + 0x10], eax
        jb     loc_3_50DD
        cmp    word ptr [edi + 0x1a], bx
        jne    loc_3_5022
        cmp    byte ptr [edi + 0xa], 6
        jne    loc_3_50DD
loc_3_5022:
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0x2c]
        push   eax
        push   ecx
        push   edi
        call   sub_3_4DFE
        add    esp, 0xc
        test   eax, eax
        jne    loc_3_50E9
        push   ebx
        lea    ecx, [ebp - 0x2c]
        push   dword ptr [ebp + 0xc]
        push   ecx
        or     byte ptr [esi + 0x14], 4
        lea    edx, [ebp - 4]
        push   edx
        call   sub_3_52E0
        add    esp, 0x10
        test   eax, eax
        je     loc_3_506A
        cmp    eax, 0x10
        jne    loc_3_50EE
        cmp    dword ptr [edi + 0x2c], ebx
        jne    loc_3_50DD
        jmp    loc_3_50EE
loc_3_506A:
        mov    eax, dword ptr [ebp - 4]
        push   ebx
        push   edi
        mov    dword ptr [edi + 0x2c], eax
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp - 4]
        call   sub_3_5430
        mov    dx, word ptr [edi + 4]
        add    esp, 0x10
        lea    ecx, [edi + 0x31]
        mov    eax, dword ptr [edi + 0x28]
        mov    word ptr [ecx], dx
        test   eax, eax
        je     loc_3_5096
        mov    eax, dword ptr [eax + 0xc]
        jmp    loc_3_5099
loc_3_5096:
        mov    eax, dword ptr [edi + 0xc]
loc_3_5099:
        push   ebx
        mov    dword ptr [edi + 0x33], eax
        push   ecx
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp - 4]
        call   sub_3_5430
        mov    cx, word ptr [esi + 0x10]
        add    esp, 0x10
        mov    word ptr [ebp - 0xc], cx
        push   ebx
        mov    word ptr [ebp - 0xa], bx
        lea    ecx, [ebp - 0xc]
        push   ecx
        push   4
        push   8
        push   dword ptr [ebp - 4]
        call   sub_3_5428
        add    esp, 0x14
        cmp    dword ptr [ebp - 8], ebx
        je     loc_3_50DD
        push   edi
        push   dword ptr [ebp - 8]
        call   sub_3_4E73
        add    esp, 8
loc_3_50DD:
        mov    edi, dword ptr [edi]
        test   edi, edi
        jne    loc_3_4FFC
        jmp    loc_3_50EE
loc_3_50E9:
        mov    eax, 0x13
loc_3_50EE:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_50F5:
        mov    eax, dword ptr [esp + 4]
        push   esi
        mov    esi, dword ptr [eax + 0x2c]
        test   esi, esi
        jne    loc_3_5108
        mov    esi, dword ptr [eax + 0x24]
        test   esi, esi
        je     loc_3_5120
loc_3_5108:
        mov    eax, dword ptr [esi + 0x2c]
        test   eax, eax
        je     loc_3_511A
        push   0
        push   eax
        call   sub_3_52B8
        add    esp, 8
loc_3_511A:
        mov    esi, dword ptr [esi]
        test   esi, esi
        jne    loc_3_5108
loc_3_5120:
        xor    eax, eax
        pop    esi
        ret    
sub_3_5124:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        push   0
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0xc]
        lea    eax, [ebp - 4]
        push   dword ptr [esi + 4]
        push   edi
        push   eax
        call   sub_3_5370
        add    esp, 0x10
        test   eax, eax
        jne    loc_3_5179
        mov    eax, dword ptr [esi + 0x10]
        test   eax, eax
        je     loc_3_5179
loc_3_514F:
        mov    ecx, dword ptr [eax + 0x2c]
        test   ecx, ecx
        jne    loc_3_515D
        mov    ecx, dword ptr [eax + 0x24]
        test   ecx, ecx
        je     loc_3_5173
loc_3_515D:
        test   byte ptr [ecx + 0xb], 3
        jne    loc_3_516D
        cmp    dword ptr [ecx + 0x2c], edi
        jne    loc_3_516D
        cmp    dword ptr [ebp - 4], ecx
        je     loc_3_5181
loc_3_516D:
        mov    ecx, dword ptr [ecx]
        test   ecx, ecx
        jne    loc_3_515D
loc_3_5173:
        mov    eax, dword ptr [eax]
        test   eax, eax
        jne    loc_3_514F
loc_3_5179:
        xor    eax, eax
loc_3_517B:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
loc_3_5181:
        cmp    dword ptr [ebp + 0x10], 0
        je     loc_3_517B
        mov    eax, ecx
        jmp    loc_3_517B
sub_3_518B:
        push   1
        push   dword ptr [esp + 0xc]
        push   dword ptr [esp + 0xc]
        call   sub_3_5124
        add    esp, 0xc
        test   eax, eax
        jne    loc_3_51A8
        mov    eax, 0x13
        jmp    loc_3_51B1
loc_3_51A8:
        mov    dword ptr [eax + 0x2c], 0
        xor    eax, eax
loc_3_51B1:
        ret    
sub_3_51B2:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x80
        push   ebx
        push   esi
        push   edi
        push   1
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_3_5124
        add    esp, 0xc
        test   eax, eax
        je     loc_3_526B
        mov    ecx, 0                        ; -> obj5:0x0540
        lea    edx, [ebp - 0x80]
        cmp    byte ptr [0], 0               ; -> obj5:0x0540
        je     loc_3_51F9
loc_3_51E7:
        lea    esi, [ebp - 1]
        cmp    edx, esi
        jae    loc_3_51F9
        mov    bl, byte ptr [ecx]
        inc    ecx
        mov    byte ptr [edx], bl
        inc    edx
        cmp    byte ptr [ecx], 0
        jne    loc_3_51E7
loc_3_51F9:
        lea    ecx, [ebp - 0x80]
        sub    ecx, edx
        add    ecx, 0x7f
        cmp    ecx, 0xd
        jae    loc_3_520B
        mov    byte ptr [edx], 0
        jmp    loc_3_5241
loc_3_520B:
        mov    ecx, 0                        ; -> obj5:0x0554
        mov    byte ptr [edx], 0x20
        inc    edx
        mov    byte ptr [edx], 0x28
        inc    edx
        cmp    byte ptr [0], 0               ; -> obj5:0x0554
        je     loc_3_522C
loc_3_5221:
        mov    bl, byte ptr [ecx]
        inc    ecx
        mov    byte ptr [edx], bl
        inc    edx
        cmp    byte ptr [ecx], 0
        jne    loc_3_5221
loc_3_522C:
        mov    ax, word ptr [eax + 0x1a]
        push   eax
        push   edx
        call   sub_3_4D76
        add    esp, 8
        mov    byte ptr [eax], 0x29
        mov    byte ptr [eax + 1], 0
loc_3_5241:
        push   0
        lea    edi, [ebp - 0x80]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        dec    ecx
        lea    eax, [ebp - 0x80]
        push   ecx
        push   eax
        push   1
        push   0                             ; -> obj5:0x055C
        push   0
        push   dword ptr [ebp + 0xc]
        call   sub_3_5330
        add    esp, 0x1c
loc_3_526B:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_3_5272:
        xor    ax, ax
        ret    
        db  0CCh, 0CCh
sub_3_5278:
        VxDCall  CONFIGMG._CONFIGMG_Register_DevLoader ; cd2032803300
        int3   
        int3   
sub_3_5280:
        VxDCall  CONFIGMG._CONFIGMG_Register_Device_Driver ; cd200e803300
        int3   
        int3   
sub_3_5288:
        VxDCall  CONFIGMG._CONFIGMG_Register_Enumerator ; cd200f803300
        int3   
        int3   
sub_3_5290:
        VxDCall  CONFIGMG._CONFIGMG_Load_DLVxDs  ; cd202f803300
        int3   
        int3   
sub_3_5298:
        VxDCall  CONFIGMG._CONFIGMG_Read_Registry_Value ; cd203e803300
        int3   
        int3   
sub_3_52A0:
        VxDCall  CONFIGMG._CONFIGMG_Call_Enumerator_Function ; cd2054803300
        int3   
        int3   
sub_3_52A8:
        VxDCall  CONFIGMG._CONFIGMG_Get_Bus_Info ; cd2044803300
        int3   
        int3   
sub_3_52B0:
        VxDCall  CONFIGMG._CONFIGMG_Register_Enumerator_Function ; cd2053803300
        int3   
        int3   
sub_3_52B8:
        VxDCall  CONFIGMG._CONFIGMG_Remove_SubTree ; cd200d803300
        int3   
        int3   
sub_3_52C0:
        VxDCall  CONFIGMG.Service_8065h          ; cd2065803300
        int3   
        int3   
sub_3_52C8:
        VxDCall  CONFIGMG._CONFIGMG_Reenumerate_DevNode ; cd2033803300
        int3   
        int3   
sub_3_52D0:
        VxDCall  CONFIGMG._CONFIGMG_Query_Remove_SubTree ; cd200c803300
        int3   
        int3   
sub_3_52D8:
        VxDCall  CONFIGMG._CONFIGMG_Get_Parent   ; cd2003803300
        int3   
        int3   
sub_3_52E0:
        VxDCall  CONFIGMG._CONFIGMG_Create_DevNode ; cd200b803300
        int3   
        int3   
sub_3_52E8:
        VxDCall  CONFIGMG._CONFIGMG_Free_Log_Conf ; cd2019803300
        int3   
        int3   
sub_3_52F0:
        VxDCall  CONFIGMG._CONFIGMG_Get_Next_Log_Conf ; cd201b803300
        int3   
        int3   
sub_3_52F8:
        VxDCall  CONFIGMG._CONFIGMG_Get_Next_Res_Des ; cd201f803300
        int3   
        int3   
sub_3_5300:
        VxDCall  CONFIGMG._CONFIGMG_Get_First_Log_Conf ; cd201a803300
        int3   
        int3   
sub_3_5308:
        VxDCall  CONFIGMG._CONFIGMG_Yield        ; cd2015803300
        int3   
        int3   
sub_3_5310:
        VxDCall  CONFIGMG.Service_805Bh          ; cd205b803300
        int3   
        int3   
sub_3_5318:
        VMMCall  VMM._RegCloseKey                ; cd2049810100
        int3   
        int3   
sub_3_5320:
        VMMCall  VMM._RegQueryValueEx            ; cd2051810100
        int3   
        int3   
sub_3_5328:
        VMMCall  VMM._RegOpenKey                 ; cd2048810100
        int3   
        int3   
sub_3_5330:
        VxDCall  CONFIGMG._CONFIGMG_Write_Registry_Value ; cd203f803300
        int3   
        int3   
sub_3_5338:
        VxDCall  SHELL._SHELL_CallDll            ; cd2018801700
        int3   
        int3   
sub_3_5340:
        VMMCall  VMM._MapPhysToLinear            ; cd206c800100
        int3   
        int3   
sub_3_5348:
        VxDCall  CONFIGMG._CONFIGMG_Add_Res_Des  ; cd201c803300
        int3   
        int3   
sub_3_5350:
        VxDCall  CONFIGMG._CONFIGMG_Add_Empty_Log_Conf ; cd2018803300
        int3   
        int3   
sub_3_5358:
        VxDCall  CONFIGMG._CONFIGMG_Locate_DevNode ; cd2002803300
        int3   
        int3   
sub_3_5360:
        VxDCall  CONFIGMG._CONFIGMG_Get_Sibling  ; cd2005803300
        int3   
        int3   
sub_3_5368:
        VxDCall  CONFIGMG._CONFIGMG_Get_Child    ; cd2004803300
        int3   
        int3   
sub_3_5370:
        VxDCall  CONFIGMG._CONFIGMG_Get_Private_DWord ; cd2009803300
        int3   
        int3   
sub_3_5378:
        VxDCall  CONFIGMG._CONFIGMG_Next_Range   ; cd202d803300
        int3   
        int3   
sub_3_5380:
        VxDCall  CONFIGMG._CONFIGMG_First_Range  ; cd202c803300
        int3   
        int3   
sub_3_5388:
        VxDCall  CONFIGMG._CONFIGMG_Query_Arbitrator_Free_Data ; cd2013803300
        int3   
        int3   
sub_3_5390:
        VxDCall  CONFIGMG._CONFIGMG_Free_Res_Des ; cd201e803300
        int3   
        int3   
sub_3_5398:
        VxDCall  CONFIGMG._CONFIGMG_Get_DevNode_Status ; cd2036803300
        int3   
        int3   
sub_3_53A0:
        VMMCall  VMM._HeapAllocate               ; cd204f800100
        int3   
        int3   
sub_3_53A8:
        VMMCall  VMM._HeapFree                   ; cd2051800100
        int3   
        int3   
sub_3_53B0:
        push   esi
        mov    edx, dword ptr [esp + 0x10]
        mov    eax, dword ptr [esp + 0xc]
        mov    esi, dword ptr [esp + 8]
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    eax, esi
        pop    esi
        ret    0xc
        db  0CCh, 0CCh, 0CCh
sub_3_53CC:
        push   esi
        mov    esi, dword ptr [esp + 8]
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
        pop    esi
        ret    4
        db  0CCh
sub_3_53DC:
        mov    eax, dword ptr [esp + 4]
        VxDCall  VPICD.VPICD_Force_Default_Behavior ; cd2011000300
        ret    4
        db  0CCh, 0CCh, 0CCh
sub_3_53EC:
        mov    eax, dword ptr [esp + 4]
        VxDCall  VPICD.VPICD_Physically_Mask     ; cd2008000300
        ret    4
        db  0CCh, 0CCh, 0CCh
sub_3_53FC:
        mov    eax, dword ptr [esp + 4]
        VxDCall  VPICD.VPICD_Physically_Unmask   ; cd2009000300
        ret    4
        db  0CCh, 0CCh, 0CCh
sub_3_540C:
        push   edi
        mov    edi, dword ptr [esp + 8]
        VxDCall  VPICD.VPICD_Virtualize_IRQ      ; cd2001000300
        cmc    
        sbb    ecx, ecx
        pop    edi
        and    eax, ecx
        ret    4
sub_3_5420:
        VxDCall  CONFIGMG._CONFIGMG_Get_CRC_CheckSum ; cd2031803300
        int3   
        int3   
sub_3_5428:
        VxDCall  CONFIGMG._CONFIGMG_Set_Bus_Info ; cd2043803300
        int3   
        int3   
sub_3_5430:
        VxDCall  CONFIGMG._CONFIGMG_Set_Private_DWord ; cd200a803300
        int3   
        int3   
sub_3_5438:
        VxDCall  CONFIGMG._CONFIGMG_Add_ID       ; cd2055803300
        int3   
        int3   
sub_3_5440:
        VxDCall  CONFIGMG._CONFIGMG_Register_Arbitrator ; cd2010803300
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0x13  R-X
sub_4_0000:
        mov    ecx, dword ptr [esp + 4]
        movzx  eax, word ptr [ecx*2 + 0xa]   ; -> obj2:0x000A
        add    eax, dword ptr [4]            ; -> obj2:0x0004
        ret    
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 5  vsize=0x567  RW-
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 053h, 079h, 073h, 074h
        db  065h, 06Dh, 05Ch, 043h, 075h, 072h, 072h, 065h, 06Eh, 074h, 043h, 06Fh, 06Eh, 074h, 072h, 06Fh
        db  06Ch, 053h, 065h, 074h, 05Ch, 053h, 065h, 072h, 076h, 069h, 063h, 065h, 073h, 05Ch, 043h, 06Ch
        db  061h, 073h, 073h, 05Ch, 050h, 043h, 04Dh, 043h, 049h, 041h, 000h, 000h, 04Fh, 070h, 074h, 069h
        db  06Fh, 06Eh, 073h, 000h, 04Dh, 065h, 06Dh, 06Fh, 072h, 079h, 000h, 000h, 041h, 06Ch, 06Ch, 06Fh
        db  063h, 04Dh, 065h, 06Dh, 057h, 069h, 06Eh, 000h, 041h, 054h, 041h, 044h, 065h, 06Ch, 061h, 079h
        db  000h, 000h, 000h, 000h, 043h, 06Fh, 06Eh, 066h, 069h, 067h, 044h, 065h, 06Ch, 061h, 079h, 000h
        db  050h, 043h, 04Dh, 043h, 049h, 041h, 05Ch, 050h, 043h, 043h, 041h, 052h, 044h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 010h, 000h, 000h, 000h, 000h, 010h, 000h
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h, 010h, 000h, 000h, 000h, 000h, 00Dh, 000h, 0FFh, 0FFh, 00Fh, 000h
        db  000h, 010h, 000h, 000h, 000h, 000h, 00Ch, 000h, 0FFh, 0FFh, 00Ch, 000h, 000h, 010h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 04Dh, 054h, 044h, 000h, 044h, 072h, 069h, 076h
        db  065h, 072h, 000h, 000h, 050h, 043h, 043h, 061h, 072h, 064h, 041h, 067h, 065h, 06Eh, 074h, 000h
        db  068h, 077h, 061h, 067h, 065h, 06Eh, 074h, 02Eh, 064h, 06Ch, 06Ch, 000h, 044h, 065h, 076h, 069h
        db  063h, 065h, 044h, 065h, 073h, 063h, 000h, 000h, 043h, 06Fh, 06Eh, 066h, 069h, 067h, 046h, 06Ch
        db  061h, 067h, 073h, 000h, 001h, 0A7h, 001h, 000h, 000h, 000h, 004h, 000h, 000h, 000h, 004h, 000h
        db  000h, 000h, 005h, 001h, 02Ah, 001h, 000h, 000h, 000h, 008h, 000h, 000h, 000h, 008h, 000h, 000h
        db  000h, 005h, 001h, 0A2h, 001h, 000h, 000h, 000h, 004h, 000h, 000h, 000h, 004h, 000h, 000h, 000h
        db  005h, 001h, 029h, 001h, 000h, 000h, 000h, 008h, 000h, 000h, 000h, 008h, 000h, 000h, 000h, 005h
        db  089h, 0B4h, 001h, 000h, 000h, 000h, 004h, 000h, 000h, 000h, 004h, 000h, 000h, 000h, 005h, 089h
        db  0BDh, 001h, 000h, 000h, 000h, 008h, 000h, 000h, 000h, 008h, 000h, 000h, 000h, 005h, 089h, 0A0h
        db  004h, 000h, 000h, 000h, 002h, 000h, 000h, 000h, 040h, 000h, 0FFh, 0FFh, 005h, 089h, 0A2h, 004h
        db  000h, 000h, 000h, 002h, 000h, 000h, 000h, 020h, 000h, 0FFh, 0FFh, 005h, 089h, 0A6h, 004h, 000h
        db  000h, 000h, 002h, 000h, 000h, 000h, 020h, 000h, 0FFh, 0FFh, 005h, 089h, 0A7h, 004h, 000h, 000h
        db  000h, 002h, 000h, 000h, 000h, 010h, 000h, 0FFh, 0FFh, 005h, 089h, 0AAh, 004h, 000h, 000h, 000h
        db  002h, 000h, 000h, 000h, 040h, 000h, 0FFh, 0FFh, 005h, 000h, 000h, 001h, 000h, 001h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 006h, 002h, 000h, 001h, 000h, 001h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 0F8h, 003h, 004h, 0F8h, 002h, 003h, 0E8h, 003h
        db  004h, 0E8h, 002h, 003h, 070h, 001h, 076h, 003h, 003h, 000h, 003h, 000h, 001h, 002h, 001h, 001h
        db  004h, 002h, 002h, 000h, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 00Ah, 000h, 00Ch, 000h, 00Dh, 000h, 00Fh, 000h
        db  014h, 000h, 019h, 000h, 01Eh, 000h, 023h, 000h, 028h, 000h, 02Dh, 000h, 032h, 000h, 037h, 000h
        db  03Ch, 000h, 046h, 000h, 050h, 000h, 05Ah, 000h
        dd  obj5:0x046C              ; fixup
        dd  obj5:0x0470              ; fixup
        dd  obj5:0x0474              ; fixup
        dd  obj5:0x0478              ; fixup
        dd  obj5:0x047C              ; fixup
        dd  obj5:0x0480              ; fixup
        dd  obj5:0x0484              ; fixup
        dd  obj5:0x0488              ; fixup
        dd  obj5:0x048C              ; fixup
        dd  obj5:0x0490              ; fixup
        dd  obj5:0x0498              ; fixup
        dd  obj5:0x04A8              ; fixup
        dd  obj5:0x04AC              ; fixup
        dd  obj5:0x04B0              ; fixup
        dd  obj5:0x04B4              ; fixup
        dd  obj5:0x04B8              ; fixup
        dd  obj5:0x04C4              ; fixup
        dd  obj5:0x04C8              ; fixup
        dd  obj5:0x04CC              ; fixup
        dd  obj5:0x04D0              ; fixup
        dd  obj5:0x04D4              ; fixup
        dd  obj5:0x04E0              ; fixup
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 068h, 064h, 063h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  04Dh, 06Fh, 064h, 065h, 06Dh, 000h, 000h, 000h, 04Dh, 075h, 06Ch, 074h, 069h, 046h, 075h, 06Eh
        db  063h, 074h, 069h, 06Fh, 06Eh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 02Ah, 050h, 04Eh, 050h, 030h, 036h, 030h, 030h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 02Ah, 050h, 04Eh, 050h, 043h, 032h, 030h, 030h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 050h, 043h, 04Dh, 043h, 049h, 041h, 05Ch, 04Dh, 054h, 044h, 02Dh, 000h
        db  044h, 065h, 076h, 069h, 063h, 065h, 044h, 065h, 073h, 063h, 000h, 000h, 044h, 065h, 076h, 069h
        db  063h, 065h, 044h, 065h, 073h, 063h, 000h, 000h, 044h, 072h, 069h, 076h, 065h, 072h, 000h, 000h
        db  044h, 072h, 069h, 076h, 065h, 072h, 000h, 000h, 043h, 06Fh, 06Eh, 066h, 069h, 067h, 046h, 06Ch
        db  061h, 067h, 073h, 000h, 043h, 06Fh, 06Eh, 066h, 069h, 067h, 046h, 06Ch, 061h, 067h, 073h, 000h
        db  04Dh, 069h, 06Eh, 052h, 065h, 067h, 069h, 06Fh, 06Eh, 053h, 069h, 07Ah, 065h, 000h, 000h, 000h
        db  04Dh, 065h, 06Dh, 06Fh, 072h, 079h, 020h, 043h, 061h, 072h, 064h, 020h, 044h, 065h, 076h, 069h
        db  063h, 065h, 000h, 000h, 04Dh, 054h, 044h, 02Dh, 000h, 000h, 000h, 000h, 044h, 065h, 076h, 069h
        db  063h, 065h, 044h, 065h, 073h, 063h, 000h
VxD__LDATA ENDS

VxD__LTEXT SEGMENT   ; object 6  vsize=0x9a  R-X
sub_6_0000:
        mov    word ptr [0], 0x2c            ; -> loc_1_19A4
        push   esi
        xor    edx, edx
        push   edi
        mov    dword ptr [0], edx            ; -> obj1:0x1908
        mov    dword ptr [0], edx            ; -> obj1:0x190C
        mov    dword ptr [0], edx            ; -> obj1:0x1910
        mov    dword ptr [0], edx            ; -> obj1:0x1918
        mov    dword ptr [0], edx            ; -> obj1:0x191C
        mov    esi, 0                        ; -> obj1:0x192C
        mov    dword ptr [0], edx            ; -> obj1:0x1920
        mov    edi, esi
        xor    eax, eax
        mov    word ptr [0], 0x12            ; -> loc_1_19D4
        mov    word ptr [0], 0x24            ; -> loc_1_19EC
        mov    word ptr [0], 0x40            ; -> loc_1_1A14
        mov    ecx, 8
        rep stosd dword ptr es:[edi], eax
        stosb  byte ptr es:[edi], al
        mov    ax, word ptr [0]              ; -> loc_1_1BDC
        mov    edi, 0                        ; -> obj1:0x1950
        mov    word ptr [0x16], ax           ; -> obj1:0x1942
        mov    ecx, 6
        inc    word ptr [0]                  ; -> loc_1_1BDC
        xor    eax, eax
        mov    dword ptr [4], 0              ; -> obj1:0x1930
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        stosb  byte ptr es:[edi], al
        pop    edi
        mov    byte ptr [0x1a], dl           ; -> obj1:0x196A
        mov    dword ptr [0x10], esi         ; -> obj1:0x1960
        pop    esi
        ret    
VxD__LTEXT ENDS

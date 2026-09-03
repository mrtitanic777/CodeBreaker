; NWLINK -- reconstructed 32-bit disassembly (VxD / LE format)
; 4 objects, 11725 instructions recovered by recursive descent

Declare_Virtual_Device NWLINK, 1, 0, NWLINK_Control, 00487h, Undefined_Init_Order
;   DDK version 0400h   init_order 054005FF6h   flags 00000h
;   service table: obj1:0x57a8, 7 services

VxD__LTEXT SEGMENT   ; object 1  vsize=0x7d28  R-X
NWLINK_Service_06:
        dec    ebx
        js     loc_1_0029
        cmp    ebx, 0x10
        jae    loc_1_0029
        cmp    eax, 1
        je     loc_1_0015
        cmp    eax, 2
        je     loc_1_001F
        mov    al, 0xff
        ret    
loc_1_0015:
        mov    dword ptr [ebx*4 + 0x54c8], esi ; -> obj1:0x54C8
        xor    eax, eax
        ret    
loc_1_001F:
        xor    eax, eax
        mov    dword ptr [ebx*4 + 0x54c8], eax ; -> obj1:0x54C8
        ret    
loc_1_0029:
        mov    al, 0xfe
        ret    
loc_1_002C:
        push   ebx
        push   esi
        push   edi
        push   eax
        mov    ebx, dword ptr [eax + 0x24]
        movzx  ecx, byte ptr [ebx + 0x1e]
        jecxz  loc_1_0050
        mov    eax, dword ptr [esi + 0x1c]
        mov    dx, word ptr [esi + 0x20]
        lea    edi, [ebx + 0x1f]
loc_1_0043:
        scasd  eax, dword ptr es:[edi]
        jne    loc_1_004B
        cmp    word ptr [edi], dx
        je     loc_1_005A
loc_1_004B:
        add    edi, 2
        loop   loc_1_0043
loc_1_0050:
        push   ebx
        push   esi
        call   sub_1_5BD3
        add    esp, 8
loc_1_005A:
        pop    esi
        and    dword ptr [esi + 0xc], 0xffffdfff
        mov    dword ptr [esi + 0x14], 0
        call   sub_1_3FF7
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_0072:
        push   esi
        push   eax
        call   sub_1_5CD9
        add    esp, 8
        ret    
loc_1_007D:
        push   esi
        push   edi
        mov    edi, dword ptr [0x7c3c]       ; -> obj1:0x7C3C
        cmp    byte ptr [eax + 9], 0
        jne    loc_1_0117
        cmp    byte ptr [edi + 0x283], 0xfe
        je     loc_1_0117
        lea    esi, [edi + 0x28e]
        movzx  edx, byte ptr [esi + 1]
        movzx  eax, byte ptr [esi]
        dec    eax
        jns    loc_1_0125
        test   dword ptr [edi], 4
        je     loc_1_00B5
        cmp    dl, 4
        jne    loc_1_00FB
loc_1_00B5:
        sub    edx, 7
        sbb    ecx, ecx
        and    edx, ecx
        add    edx, 7
        push   esi
        lea    esi, [edi + 0x64]
        push   esi
        push   edi
        call   sub_1_5508
        add    esp, 0xc
loc_1_00CF:
        or     eax, eax
        je     loc_1_00FB
        lea    esi, [edi + 0x10]
        add    eax, 0x2a
        mov    word ptr [esi + 0x28], ax
        call   sub_1_518D
        mov    dword ptr [edi + 0x65], eax
        mov    dword ptr [esi + 0xc], 0x80
        mov    dword ptr [esi + 4], 0
        mov    edx, dword ptr [edi + 8]
        call   sub_1_391B
loc_1_00FB:
        mov    esi, dword ptr [0x7c40]       ; -> obj1:0x7C40
        and    dword ptr [esi + 0xc], 0xffffdfff
        mov    dword ptr [esi + 0x14], 0
        call   sub_1_3FF7
        pop    edi
        pop    esi
        ret    
loc_1_0117:
        mov    esi, 0x5dc7                   ; -> sub_1_5DC7
        VMMCall  VMM.Schedule_Global_Event       ; cd200e000100
        pop    edi
        pop    esi
        ret    
loc_1_0125:
        mov    edx, eax
        xor    eax, eax
        push   edi
        mov    edi, 0x54c8                   ; -> obj1:0x54C8
        mov    ecx, 0x10
loc_1_0134:
        repe scasd eax, dword ptr es:[edi]
        je     loc_1_014B
        dec    edx
        jns    loc_1_0134
        mov    ecx, dword ptr [edi - 4]
        pop    edi
        lea    eax, [edi + 0x64]
        push   eax
        push   esi
        call   ecx
        add    esp, 8
        jmp    loc_1_00CF
loc_1_014B:
        pop    edi
        jmp    loc_1_00FB
sub_1_014E:
        push   ebx
        push   esi
        push   edi
        movzx  edx, word ptr [esp + 0x10]
        call   sub_1_5346
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_015F:
        test   dword ptr [edi], 4
        je     loc_1_0181
        or     dword ptr [edi], 0x20
        mov    esi, dword ptr [edi + 0x2f4]
        or     esi, esi
        je     loc_1_0181
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
        mov    edx, edi
        jmp    loc_1_027B
loc_1_0181:
        xor    eax, eax
        ret    
loc_1_0184:
        push   esi
        push   edi
        mov    edi, dword ptr [eax - 4]
        cmp    byte ptr [eax + 9], 0
        je     loc_1_0195
        inc    dword ptr [edi + 0x2f0]
loc_1_0195:
        test   dword ptr [edi], 0x20
        jne    loc_1_0246
        dec    dword ptr [edi + 0x2d8]
        je     loc_1_020E
        mov    ecx, dword ptr [edi + 0x2e8]
        jecxz  loc_1_01D2
        movzx  edx, word ptr [eax + 0x28]
        add    ecx, edx
        cmp    ecx, 0x2a
        jae    loc_1_01C1
        mov    ecx, 0x2a
loc_1_01C1:
        cmp    ecx, 0x200
        jb     loc_1_01CE
        mov    ecx, 0x200
loc_1_01CE:
        mov    word ptr [eax + 0x28], cx
loc_1_01D2:
        dec    dword ptr [edi + 0x2e0]
        je     loc_1_01E6
        mov    edi, esi
        mov    esi, eax
        call   sub_1_410D
        pop    edi
        pop    esi
        ret    
loc_1_01E6:
        mov    eax, dword ptr [edi + 0x2dc]
        mov    dword ptr [edi + 0x2e0], eax
        mov    eax, dword ptr [edi + 0x2ec]
        mov    edx, edi
        mov    esi, 0x27b                    ; -> loc_1_027B
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    dword ptr [edi + 0x2f4], esi
        pop    edi
        pop    esi
        ret    
loc_1_020E:
        push   edi
        lea    esi, [edi + 0x64]
        call   sub_1_518D
        mov    dword ptr [esi + 1], eax
        mov    eax, dword ptr [edi + 0x2f0]
        mov    word ptr [esi + 5], ax
        mov    edx, dword ptr [edi + 8]
        add    edi, 0x10
        mov    word ptr [edi + 0x28], 0x31
        mov    dword ptr [edi + 0xc], 0x80
        mov    dword ptr [edi + 4], 0
        mov    esi, edi
        call   sub_1_391B
        pop    edi
loc_1_0246:
        push   0
        push   0x22e
        push   dword ptr [edi + 0x2d4]
        VxDCall  VPOWERD._VPOWERD_Deregister_Power_Handler ; cd200b002800
        mov    dword ptr [edi + 0x2d4], 0
        and    dword ptr [edi], 0xffffffdb
        test   dword ptr [edi], 0x10
        jne    loc_1_0273
        pop    edi
        pop    esi
        xor    eax, eax
        ret    
loc_1_0273:
        call   sub_1_5DC7
        pop    edi
        pop    esi
        ret    
loc_1_027B:
        push   esi
        push   edi
        mov    dword ptr [edx + 0x2f4], 0
        mov    edi, edx
        test   dword ptr [edx], 4
        je     loc_1_02AA
        test   dword ptr [edx], 0x20
        jne    loc_1_0246
        mov    esi, dword ptr [edx + 0x2d4]
        add    esi, 4
        mov    edi, dword ptr [edx + 4]
        call   sub_1_410D
loc_1_02AA:
        pop    edi
        pop    esi
        xor    eax, eax
        ret    
loc_1_02AF:
        inc    dword ptr [0x54c4]            ; -> obj1:0x54C4
        xor    esi, esi
        ret    
loc_1_02B8:
        push   0
        push   0x58
        push   eax
        VxDCall  VPOWERD._VPOWERD_Deregister_Power_Handler ; cd200b002800
        ret    
sub_1_02C4:
        push   ebx
        push   esi
        mov    eax, 1
        mov    ebx, 2
        mov    esi, 0x6026                   ; -> loc_1_6026
        call   0
        pop    esi
        pop    ebx
        ret    
        db  000h, 000h, 000h
sub_1_02E0:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        mov    ax, word ptr [esi]
        or     ax, ax
        je     loc_1_030C
        push   eax
        call   sub_1_42A4
        add    esp, 4
        or     eax, eax
        je     loc_1_030C
        inc    word ptr [0x5934]             ; -> loc_1_5934
        mov    eax, 0xff
        jmp    loc_1_0418
loc_1_030C:
        cmp    dword ptr [0x5844], 0         ; -> loc_1_5844
        jne    loc_1_0326
        inc    word ptr [0x5934]             ; -> loc_1_5934
        mov    eax, 0xfe
        jmp    loc_1_0418
loc_1_0326:
        test   byte ptr [0x58f8], 8          ; -> loc_1_58F8
        je     loc_1_034A
        cmp    dword ptr [esp + 0x18], 0
        jne    loc_1_034A
        push   esi
        push   0
        call   sub_1_544C
        add    esp, 8
        or     eax, eax
        je     loc_1_0358
        jmp    loc_1_0418
loc_1_034A:
        cmp    word ptr [esi], 0
        jne    loc_1_0358
        call   sub_1_439A
        mov    word ptr [esi], ax
loc_1_0358:
        mov    eax, dword ptr [0x5840]       ; -> loc_1_5840
        mov    ecx, dword ptr [0x5840]       ; -> loc_1_5840
        mov    edx, dword ptr [eax]
        lea    eax, [edx + 1]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [0x5840]       ; -> loc_1_5840
        movzx  ecx, word ptr [0x5932]        ; -> loc_1_5932
        mov    eax, dword ptr [eax]
        cmp    ecx, eax
        jae    loc_1_0382
        mov    word ptr [0x5932], ax         ; -> loc_1_5932
loc_1_0382:
        mov    ecx, dword ptr [0x5870]       ; -> loc_1_5870
        dec    dword ptr [0x5844]            ; -> loc_1_5844
        mov    eax, dword ptr [ecx + 0x18]
        mov    dword ptr [0x5870], eax       ; -> loc_1_5870
        mov    eax, dword ptr [0x5860]       ; -> loc_1_5860
        mov    dword ptr [eax + edx*4], ecx
        mov    eax, dword ptr [0x585c]       ; -> loc_1_585C
        mov    di, word ptr [esi]
        mov    word ptr [eax + edx*2], di
        mov    eax, dword ptr [0x5864]       ; -> loc_1_5864
        mov    byte ptr [edx + eax], 0
        mov    ax, word ptr [esi]
        mov    word ptr [ecx + 0x14], ax
        mov    dword ptr [ecx + 8], edx
        mov    eax, dword ptr [esp + 0x10]
        or     eax, eax
        mov    dword ptr [ecx], eax
        mov    dword ptr [ecx + 0x18], 0
        je     loc_1_03D5
        mov    dword ptr [ecx + 0x18], 1
loc_1_03D5:
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [ecx + 0x1c], 0
        mov    dword ptr [ecx + 0x20], 0
        lea    eax, [ecx + 0xc]
        mov    dword ptr [ecx + 0x28], 0
        mov    dword ptr [ecx + 0x24], 0
        mov    byte ptr [ecx + 0x16], 0
        mov    byte ptr [ecx + 0x17], 0
        mov    dword ptr [ecx + 0x2c], 0
        mov    dword ptr [eax], eax
        mov    dword ptr [ecx + 0x10], eax
        sub    eax, eax
        or     dword ptr [0x58f8], 2         ; -> loc_1_58F8
loc_1_0418:
        pop    edi
        pop    esi
        ret    
sub_1_041B:
        push   ebx
        push   esi
        push   edi
        push   ebp
        push   dword ptr [esp + 0x14]
        call   sub_1_4300
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    loc_1_0436
        jmp    loc_1_055B
loc_1_0436:
        test   byte ptr [0x58f8], 8          ; -> loc_1_58F8
        je     loc_1_0455
        cmp    dword ptr [esp + 0x18], 0
        jne    loc_1_0455
        lea    eax, [esp + 0x14]
        push   eax
        push   1
        call   sub_1_544C
        add    esp, 8
loc_1_0455:
        mov    eax, dword ptr [esi + 0x1c]
        or     eax, eax
        je     loc_1_0476
loc_1_045C:
        mov    ecx, dword ptr [eax + 0x10]
        mov    dword ptr [esi + 0x1c], ecx
        push   1
        push   0
        push   eax
        call   sub_1_0562
        add    esp, 0xc
        mov    eax, dword ptr [esi + 0x1c]
        or     eax, eax
        jne    loc_1_045C
loc_1_0476:
        mov    eax, dword ptr [esi + 0x20]
        or     eax, eax
        je     loc_1_0490
loc_1_047D:
        push   1
        push   esi
        push   eax
        call   sub_1_0562
        add    esp, 0xc
        mov    eax, dword ptr [esi + 0x20]
        or     eax, eax
        jne    loc_1_047D
loc_1_0490:
        call   sub_1_2196
        mov    ecx, dword ptr [esi + 0xc]
        mov    eax, esi
        sub    eax, ecx
        cmp    eax, -0xc
        je     loc_1_04B6
loc_1_04A1:
        push   ecx
        call   sub_1_5130
        add    esp, 4
        mov    ecx, dword ptr [esi + 0xc]
        mov    eax, esi
        sub    eax, ecx
        cmp    eax, -0xc
        jne    loc_1_04A1
loc_1_04B6:
        mov    eax, dword ptr [esi + 0x28]
        or     eax, eax
        je     loc_1_04D0
loc_1_04BD:
        push   1
        push   esi
        push   eax
        call   sub_1_0562
        add    esp, 0xc
        mov    eax, dword ptr [esi + 0x28]
        or     eax, eax
        jne    loc_1_04BD
loc_1_04D0:
        mov    edi, dword ptr [esi + 0x2c]
        or     edi, edi
        je     loc_1_0530
loc_1_04D7:
        mov    eax, dword ptr [edi + 4]
        test   eax, 0x10000
        jne    loc_1_0516
        and    eax, 0xfffffcff
        mov    dword ptr [edi + 4], eax
        mov    ebp, dword ptr [edi + 0x24]
        or     ebp, ebp
        je     loc_1_051E
        mov    ebx, 0xfc
loc_1_04F5:
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [edi + 0x24], eax
        push   dword ptr [ebp]
        call   sub_1_4608
        add    esp, 4
        mov    byte ptr [ebp + 9], bl
        mov    byte ptr [ebp + 8], 0
        mov    ebp, dword ptr [edi + 0x24]
        or     ebp, ebp
        jne    loc_1_04F5
        jmp    loc_1_051E
loc_1_0516:
        or     eax, 0x800000
        mov    dword ptr [edi + 4], eax
loc_1_051E:
        push   0
        push   edi
        call   sub_1_0BD0
        add    esp, 8
        mov    edi, dword ptr [esi + 0x2c]
        or     edi, edi
        jne    loc_1_04D7
loc_1_0530:
        mov    word ptr [esi + 0x14], 0
        mov    eax, dword ptr [0x5870]       ; -> loc_1_5870
        mov    dword ptr [esi + 0x18], eax
        mov    dword ptr [0x5870], esi       ; -> loc_1_5870
        mov    eax, dword ptr [0x5840]       ; -> loc_1_5840
        inc    dword ptr [0x5844]            ; -> loc_1_5844
        cmp    dword ptr [eax], 0
        jne    loc_1_055B
        and    dword ptr [0x58f8], 0xfffffffd ; -> loc_1_58F8
loc_1_055B:
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_0562:
        push   esi
        push   edi
        cmp    dword ptr [esp + 0x14], 0
        mov    esi, dword ptr [esp + 0xc]
        jne    loc_1_057F
        cmp    byte ptr [esi + 8], 0
        jne    loc_1_057F
        mov    eax, 0xff
        jmp    loc_1_064F
loc_1_057F:
        mov    eax, dword ptr [esi + 0xc]
        test   al, 0x10
        je     loc_1_05A4
        test   al, 0x80
        je     loc_1_058F
        jmp    loc_1_064A
loc_1_058F:
        test   ah, 0x10
        jne    loc_1_0631
        inc    word ptr [0x593a]             ; -> loc_1_593A
        jmp    loc_1_064A
loc_1_05A4:
        test   al, 0x20
        je     loc_1_0603
        test   ah, 0x20
        je     loc_1_05BA
        or     eax, 0x4000
        mov    dword ptr [esi + 0xc], eax
        jmp    loc_1_064A
loc_1_05BA:
        mov    edi, dword ptr [esp + 0x10]
        or     edi, edi
        jne    loc_1_05D1
        mov    ax, word ptr [esi + 0xa]
        push   eax
        call   sub_1_42A4
        add    esp, 4
        mov    edi, eax
loc_1_05D1:
        test   byte ptr [esi + 0xc], 0x80
        push   0x10
        push   esi
        je     loc_1_05EB
        lea    eax, [edi + 0x28]
        push   eax
        call   sub_1_46EA
        add    esp, 0xc
        dec    dword ptr [edi + 0x24]
        jmp    loc_1_05F7
loc_1_05EB:
        add    edi, 0x20
        push   edi
        call   sub_1_46EA
        add    esp, 0xc
loc_1_05F7:
        push   dword ptr [esi]
        call   sub_1_4608
        add    esp, 4
        jmp    loc_1_0631
loc_1_0603:
        test   al, 0x40
        je     loc_1_0617
        mov    eax, dword ptr [esi]
        add    eax, 0x58
        push   eax
        call   sub_1_5130
        add    esp, 4
        jmp    loc_1_0631
loc_1_0617:
        test   ah, 1
        je     loc_1_064A
        mov    eax, dword ptr [esi + 0x10]
        push   0
        or     dword ptr [eax + 4], 0x800000
        push   eax
        call   sub_1_0BD0
        add    esp, 8
loc_1_0631:
        mov    dword ptr [esi], 0
        mov    dword ptr [esi + 0xc], 0x8000
        mov    byte ptr [esi + 9], 0xfc
        mov    byte ptr [esi + 8], 0
        sub    eax, eax
        jmp    loc_1_064F
loc_1_064A:
        mov    eax, 0xf9
loc_1_064F:
        pop    edi
        pop    esi
        ret    
sub_1_0652:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x18]
        inc    word ptr [0x5942]             ; -> loc_1_5942
        mov    ax, word ptr [esi + 0xa]
        push   eax
        call   sub_1_42A4
        add    esp, 4
        mov    ebp, eax
        or     ebp, ebp
        jne    loc_1_0689
        mov    byte ptr [esi + 9], 0xff
        mov    byte ptr [esi + 8], 0
        mov    eax, 0xff
        jmp    loc_1_0914
loc_1_0689:
        cmp    dword ptr [0x5858], 0         ; -> loc_1_5858
        jne    loc_1_06AB
        mov    byte ptr [esi + 9], 0xef
        mov    byte ptr [esi + 8], 0
        inc    word ptr [0x5944]             ; -> loc_1_5944
        mov    eax, 0xef
        jmp    loc_1_0914
loc_1_06AB:
        cmp    dword ptr [0x5848], 0         ; -> loc_1_5848
        je     loc_1_0900
        cmp    dword ptr [0x584c], 2         ; -> loc_1_584C
        jb     loc_1_0900
        cmp    word ptr [esi + 0x22], 1
        jne    loc_1_08EA
        cmp    word ptr [esi + 0x28], 0x2a
        jne    loc_1_08EA
        or     dword ptr [esi + 0xc], 0x180
        mov    edi, dword ptr [0x5874]       ; -> loc_1_5874
        dec    dword ptr [0x5858]            ; -> loc_1_5858
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [0x5874], eax       ; -> loc_1_5874
        call   sub_1_43D4
        mov    word ptr [edi + 2], ax
        movzx  ecx, ax
        mov    eax, dword ptr [esp + 0x24]
        mov    dword ptr [eax], ecx
        mov    dword ptr [edi + 4], 0x10001
        mov    eax, dword ptr [esp + 0x20]
        or     eax, eax
        je     loc_1_071E
        mov    dword ptr [edi + 4], 0x10401
loc_1_071E:
        add    al, al
        mov    byte ptr [edi + 1], al
        mov    byte ptr [edi], 2
        mov    dword ptr [edi + 0xc], ebp
        mov    dword ptr [edi + 0x1c], 5
        lea    ecx, [edi + 0x30]
        mov    eax, dword ptr [esp + 0x1c]
        or     eax, eax
        jne    loc_1_0743
        mov    dword ptr [ecx], 0xa
        jmp    loc_1_0745
loc_1_0743:
        mov    dword ptr [ecx], eax
loc_1_0745:
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edi + 0x34], eax
        mov    dword ptr [edi + 0x24], esi
        mov    dword ptr [edi + 0x14], 0
        mov    dword ptr [edi + 0x18], 0
        mov    eax, dword ptr [esi + 0x10]
        mov    dword ptr [edi + 8], eax
        lea    eax, [edi + 8]
        mov    ebx, dword ptr [esp + 0x28]
        push   eax
        lea    eax, [edi + 0x2c]
        push   eax
        lea    eax, [ebx + 6]
        push   eax
        lea    eax, [edi + 0x38]
        push   eax
        call   sub_1_621B
        add    esp, 0x10
        or     eax, eax
        je     loc_1_07A7
        mov    byte ptr [esi + 9], 0xfa
        mov    byte ptr [esi + 8], 0
        mov    eax, dword ptr [0x5874]       ; -> loc_1_5874
        mov    dword ptr [edi + 0x10], eax
        mov    eax, 0xfa
        mov    dword ptr [0x5874], edi       ; -> loc_1_5874
        inc    dword ptr [0x5858]            ; -> loc_1_5858
        jmp    loc_1_0914
loc_1_07A7:
        mov    eax, dword ptr [edi + 0x2c]
        mov    ecx, eax
        shl    ecx, 8
        shr    eax, 8
        or     ecx, eax
        and    ecx, 0xffff
        lea    eax, [edi + 0x3e]
        mov    dword ptr [esp + 0x10], eax
        push   0xc
        lea    eax, [ebx + 6]
        push   eax
        mov    dword ptr [edi + 0x2c], ecx
        mov    eax, dword ptr [esp + 0x18]
        add    eax, 6
        push   eax
        call   sub_1_4AF3
        add    esp, 0xc
        push   6
        lea    eax, [edi + 0x38]
        push   eax
        lea    eax, [esi + 0x1c]
        push   eax
        call   sub_1_4AF3
        mov    eax, dword ptr [esp + 0x1c]
        add    esp, 0xc
        mov    word ptr [eax + 0x24], 0
        mov    word ptr [eax + 0x26], 0
        mov    eax, dword ptr [ebp + 0x24]
        or     eax, eax
        je     loc_1_081C
        lea    ecx, [eax - 1]
        shr    ecx, 8
        dec    ax
        shl    ax, 8
        or     cx, ax
        mov    eax, dword ptr [esp + 0x10]
        mov    word ptr [eax + 0x28], cx
        jmp    loc_1_0826
loc_1_081C:
        mov    eax, dword ptr [esp + 0x10]
        mov    word ptr [eax + 0x28], 0
loc_1_0826:
        or     dword ptr [ebp + 0x18], 2
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [0x5864]       ; -> loc_1_5864
        or     byte ptr [eax + ecx], 4
        mov    eax, dword ptr [ebp + 0x2c]
        mov    dword ptr [edi + 0x10], eax
        mov    dword ptr [ebp + 0x2c], edi
        inc    byte ptr [ebp + 0x16]
        mov    byte ptr [ebx + 5], 5
        mov    byte ptr [ebx + 0x1e], 0xc0
        mov    byte ptr [ebx + 0x1f], 0
        mov    ax, word ptr [edi + 2]
        mov    word ptr [ebx + 0x20], ax
        mov    word ptr [ebx + 0x22], 0xffff
        mov    word ptr [ebx + 0x24], 0
        mov    word ptr [ebx + 0x26], 0
        mov    eax, dword ptr [ebp + 0x24]
        or     eax, eax
        je     loc_1_0885
        lea    ecx, [eax - 1]
        shr    ecx, 8
        dec    ax
        shl    ax, 8
        or     cx, ax
        mov    word ptr [ebx + 0x28], cx
        jmp    loc_1_088B
loc_1_0885:
        mov    word ptr [ebx + 0x28], 0
loc_1_088B:
        mov    dword ptr [edi + 0x24], esi
        mov    edx, dword ptr [0x5854]       ; -> loc_1_5854
        inc    dword ptr [0x5854]            ; -> loc_1_5854
        mov    eax, dword ptr [0x5868]       ; -> loc_1_5868
        mov    cx, word ptr [edi + 2]
        mov    word ptr [eax + edx*2], cx
        mov    eax, dword ptr [0x586c]       ; -> loc_1_586C
        mov    dword ptr [eax + edx*4], edi
        mov    eax, dword ptr [edi + 0x1c]
        mov    dword ptr [edi + 0x20], eax
        mov    byte ptr [esi + 8], 0xff
        push   dword ptr [edi + 8]
        push   esi
        call   sub_1_4092
        add    esp, 8
        test   byte ptr [0x58f8], 4          ; -> loc_1_58F8
        jne    loc_1_08E6
        push   0
        push   0x3e33                        ; -> loc_1_3E33
        push   0x64
        call   sub_1_4AAB
        add    esp, 0xc
        or     dword ptr [0x58f8], 4         ; -> loc_1_58F8
loc_1_08E6:
        sub    eax, eax
        jmp    loc_1_0914
loc_1_08EA:
        mov    byte ptr [esi + 9], 0xfd
        mov    byte ptr [esi + 8], 0
        inc    word ptr [0x5944]             ; -> loc_1_5944
        mov    eax, 0xfd
        jmp    loc_1_0914
loc_1_0900:
        mov    byte ptr [esi + 9], 1
        mov    byte ptr [esi + 8], 0
        inc    word ptr [0x5944]             ; -> loc_1_5944
        mov    eax, 1
loc_1_0914:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_1_091C:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x10]
        inc    word ptr [0x5946]             ; -> loc_1_5946
        mov    ax, word ptr [edi + 0xa]
        push   eax
        call   sub_1_42A4
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    loc_1_0951
        mov    byte ptr [edi + 9], 0xff
        mov    byte ptr [edi + 8], 0
        inc    word ptr [0x5948]             ; -> loc_1_5948
        jmp    loc_1_0A30
loc_1_0951:
        push   edi
        call   sub_1_4488
        add    esp, 4
        or     eax, eax
        jne    loc_1_0972
        mov    byte ptr [edi + 9], 1
        mov    byte ptr [edi + 8], 0
        inc    word ptr [0x5948]             ; -> loc_1_5948
        jmp    loc_1_0A30
loc_1_0972:
        cmp    dword ptr [0x5858], 0         ; -> loc_1_5858
        jne    loc_1_098F
        mov    byte ptr [edi + 9], 0xef
        mov    byte ptr [edi + 8], 0
        inc    word ptr [0x5948]             ; -> loc_1_5948
        jmp    loc_1_0A30
loc_1_098F:
        mov    ebx, dword ptr [0x5874]       ; -> loc_1_5874
        dec    dword ptr [0x5858]            ; -> loc_1_5858
        mov    eax, dword ptr [ebx + 0x10]
        mov    dword ptr [0x5874], eax       ; -> loc_1_5874
        call   sub_1_43D4
        mov    word ptr [ebx + 2], ax
        mov    dword ptr [edi + 0x10], ebx
        mov    word ptr [edi + 0x22], 0
        mov    dword ptr [ebx + 4], 0x30000
        mov    eax, dword ptr [esp + 0x18]
        or     eax, eax
        je     loc_1_09CB
        mov    dword ptr [ebx + 4], 0x30400
loc_1_09CB:
        add    al, al
        mov    byte ptr [ebx + 1], al
        mov    byte ptr [ebx], 1
        mov    dword ptr [ebx + 0xc], esi
        mov    dword ptr [ebx + 0x20], 0xffffffff
        lea    eax, [ebx + 0x30]
        mov    dword ptr [ebx + 0x1c], 5
        mov    ecx, dword ptr [esp + 0x14]
        or     ecx, ecx
        jne    loc_1_09F7
        mov    dword ptr [eax], 0xa
        jmp    loc_1_09F9
loc_1_09F7:
        mov    dword ptr [eax], ecx
loc_1_09F9:
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebx + 0x34], eax
        mov    dword ptr [ebx + 0x14], 0
        mov    dword ptr [ebx + 0x18], 0
        or     dword ptr [esi + 0x18], 2
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [0x5864]       ; -> loc_1_5864
        or     byte ptr [eax + ecx], 4
        mov    eax, dword ptr [esi + 0x2c]
        mov    dword ptr [ebx + 0x10], eax
        mov    dword ptr [esi + 0x2c], ebx
        inc    byte ptr [esi + 0x17]
        mov    dword ptr [ebx + 0x24], edi
        mov    byte ptr [edi + 8], 0xfe
loc_1_0A30:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_0A34:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        mov    ecx, 0x64
        mov    byte ptr [esi], 3
        mov    dword ptr [esi + 0x20], 0xffffffff
        mov    dword ptr [esi + 0x68], 0
        mov    eax, dword ptr [esi + 0x2c]
        and    eax, 0xffff
        imul   eax, eax, 0x37
        add    eax, 0x1c2
        sub    edx, edx
        div    ecx
        lea    edi, [esi + 0x3e]
        mov    dword ptr [esi + 0x1c], eax
        mov    word ptr [edi], 0xffff
        mov    word ptr [edi + 2], 0x2a00
        mov    byte ptr [edi + 4], 0
        mov    byte ptr [edi + 5], 5
        push   0xa
        mov    eax, dword ptr [esi + 8]
        add    eax, 0x18
        push   eax
        lea    eax, [edi + 0x12]
        push   eax
        call   sub_1_4AF3
        add    esp, 0xc
        mov    ecx, dword ptr [esi + 0xc]
        mov    ax, word ptr [ecx + 0x14]
        mov    word ptr [edi + 0x1c], ax
        mov    byte ptr [edi + 0x1e], 0x80
        mov    byte ptr [edi + 0x1f], 0
        mov    ax, word ptr [esi + 2]
        mov    word ptr [edi + 0x20], ax
        mov    word ptr [edi + 0x24], 0
        mov    word ptr [edi + 0x26], 0
        mov    eax, dword ptr [ecx + 0x24]
        or     eax, eax
        je     loc_1_0AD5
        lea    ecx, [eax - 1]
        shr    ecx, 8
        dec    ax
        shl    ax, 8
        or     cx, ax
        mov    word ptr [edi + 0x28], cx
        jmp    loc_1_0ADB
loc_1_0AD5:
        mov    word ptr [edi + 0x28], 0
loc_1_0ADB:
        push   dword ptr [esi + 8]
        lea    eax, [esi + 0x38]
        push   eax
        push   edi
        call   sub_1_0AFF
        add    esp, 0xc
        mov    dword ptr [esi + 0x18], eax
        mov    eax, dword ptr [esi + 0x24]
        or     dword ptr [esi + 4], 0x300
        mov    byte ptr [eax + 9], 0
        pop    edi
        pop    esi
        ret    
sub_1_0AFF:
        push   ebx
        push   esi
        push   edi
        push   ebp
        cmp    dword ptr [0x5848], 3         ; -> loc_1_5848
        jae    loc_1_0B21
        call   sub_1_1C7D
        cmp    dword ptr [0x5848], 0         ; -> loc_1_5848
        jne    loc_1_0B21
        sub    eax, eax
        jmp    loc_1_0BCB
loc_1_0B21:
        cmp    dword ptr [0x584c], 2         ; -> loc_1_584C
        jae    loc_1_0B3F
        call   sub_1_1CDF
        cmp    dword ptr [0x584c], 2         ; -> loc_1_584C
        jae    loc_1_0B3F
        sub    eax, eax
        jmp    loc_1_0BCB
loc_1_0B3F:
        cmp    dword ptr [0x5848], 0         ; -> loc_1_5848
        mov    eax, 0
        jbe    loc_1_0BCB
        mov    edi, dword ptr [0x58b4]       ; -> loc_1_58B4
        dec    dword ptr [0x5848]            ; -> loc_1_5848
        mov    eax, dword ptr [edi + 0x38]
        lea    esi, [edi + 0x38]
        mov    dword ptr [0x58b4], eax       ; -> loc_1_58B4
        mov    ebx, dword ptr [0x58b8]       ; -> loc_1_58B8
        sub    dword ptr [0x584c], 2         ; -> loc_1_584C
        mov    ecx, dword ptr [ebx]
        mov    ebp, dword ptr [esp + 0x1c]
        mov    eax, dword ptr [ecx]
        mov    dword ptr [0x58b8], eax       ; -> loc_1_58B8
        mov    dword ptr [ecx], 0
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [ecx + 4], eax
        push   ebp
        push   0x2a
        push   esi
        mov    dword ptr [ecx + 0xc], 0x2a
        push   dword ptr [esp + 0x24]
        call   dword ptr [ebp + 0x28]
        add    esp, 0x10
        lea    eax, [esi + 0x20]
        mov    dword ptr [ebx + 4], eax
        mov    eax, dword ptr [esi + 0x1c]
        mov    dword ptr [ebx + 0xc], eax
        push   ebx
        push   edi
        call   sub_1_2498
        mov    dword ptr [esi + 0xc], 0
        mov    byte ptr [esi + 4], 0x80
        lea    eax, [esi + 0x14]
        mov    dword ptr [esi + 8], ebp
        mov    dword ptr [eax], eax
        mov    dword ptr [esi + 0x18], eax
        mov    eax, edi
loc_1_0BCB:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_0BD0:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        mov    edi, dword ptr [esi + 0xc]
        mov    ax, word ptr [esi + 2]
        push   eax
        call   sub_1_434F
        add    esp, 4
        test   byte ptr [esi + 4], 8
        je     loc_1_0BFD
        push   0x14
        push   esi
        push   0x5890                        ; -> loc_1_5890
        call   sub_1_46EA
        add    esp, 0xc
loc_1_0BFD:
        push   0x10
        push   esi
        lea    eax, [edi + 0x2c]
        push   eax
        call   sub_1_46EA
        add    esp, 0xc
        mov    eax, dword ptr [esi + 4]
        test   eax, 0x10000
        je     loc_1_0C6C
        mov    ecx, dword ptr [esi + 0x24]
        test   eax, 0x800000
        je     loc_1_0C47
        mov    byte ptr [ecx + 9], 0xfc
        mov    byte ptr [ecx + 8], 0
        test   byte ptr [esi + 6], 2
        je     loc_1_0C33
        dec    byte ptr [edi + 0x17]
        jmp    loc_1_0C36
loc_1_0C33:
        dec    byte ptr [edi + 0x16]
loc_1_0C36:
        mov    eax, dword ptr [ecx]
        or     eax, eax
        je     loc_1_0C9A
        push   eax
        call   sub_1_4608
        add    esp, 4
        jmp    loc_1_0C9A
loc_1_0C47:
        test   eax, 0x400000
        mov    byte ptr [ecx + 9], 0xed
        jne    loc_1_0C56
        mov    byte ptr [ecx + 9], 0xed
loc_1_0C56:
        inc    word ptr [0x5944]             ; -> loc_1_5944
        push   dword ptr [ecx]
        call   sub_1_4615
        add    esp, 4
        dec    byte ptr [edi + 0x16]
        jmp    loc_1_0C9A
loc_1_0C6C:
        dec    byte ptr [edi + 0x16]
        mov    ecx, dword ptr [esi + 0x24]
        or     ecx, ecx
        je     loc_1_0C9A
        cmp    dword ptr [esp + 0x14], 1
        sbb    bl, bl
        add    bl, 0xed
loc_1_0C80:
        mov    eax, dword ptr [ecx + 0x10]
        mov    dword ptr [esi + 0x24], eax
        mov    byte ptr [ecx + 9], bl
        push   dword ptr [ecx]
        call   sub_1_4615
        add    esp, 4
        mov    ecx, dword ptr [esi + 0x24]
        or     ecx, ecx
        jne    loc_1_0C80
loc_1_0C9A:
        cmp    byte ptr [edi + 0x16], 0
        jne    loc_1_0CB7
        cmp    byte ptr [edi + 0x17], 0
        jne    loc_1_0CB7
        and    dword ptr [edi + 0x18], 0xfffffffd
        mov    eax, dword ptr [edi + 8]
        mov    ecx, dword ptr [0x5864]       ; -> loc_1_5864
        and    byte ptr [eax + ecx], 0xfb
loc_1_0CB7:
        mov    ecx, dword ptr [esi + 0x18]
        or     ecx, ecx
        je     loc_1_0CE1
        mov    dl, byte ptr [ecx + 0x3c]
        lea    eax, [ecx + 0x38]
        test   dl, 4
        je     loc_1_0CD8
        and    dl, 0x7f
        mov    byte ptr [eax + 4], dl
        mov    dword ptr [eax + 0xc], 0
        jmp    loc_1_0CE1
loc_1_0CD8:
        push   ecx
        call   sub_1_4608
        add    esp, 4
loc_1_0CE1:
        mov    eax, dword ptr [0x5874]       ; -> loc_1_5874
        mov    dword ptr [esi + 0x10], eax
        mov    dword ptr [0x5874], esi       ; -> loc_1_5874
        inc    dword ptr [0x5858]            ; -> loc_1_5858
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_0CF9:
        mov    eax, dword ptr [esp + 4]
        cmp    dword ptr [eax + 0x24], 0
        jbe    loc_1_0D3C
        mov    edx, dword ptr [eax + 0x28]
        mov    ecx, dword ptr [edx + 0x10]
        mov    dword ptr [eax + 0x28], ecx
        dec    dword ptr [eax + 0x24]
        mov    ecx, dword ptr [edx]
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [ecx + 0x40], eax
        mov    byte ptr [edx + 9], 0xed
        movzx  eax, word ptr [esp + 8]
        shl    eax, 0x10
        mov    ecx, dword ptr [edx + 0xc]
        and    ecx, 0xffff
        or     eax, ecx
        mov    dword ptr [edx + 0xc], eax
        push   dword ptr [edx]
        call   sub_1_4615
        add    esp, 4
loc_1_0D3C:
        ret    
        db  000h, 000h, 000h
sub_1_0D40:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    ebx, dword ptr [ebx + 8]
        mov    ax, 0xfdff
        or     ax, word ptr [ebx + 0x2c]
        VMMCall  VMM.Enable_VM_Ints              ; cd201a000100
loc_1_0D72:
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        VMMCall  VMM.Release_Time_Slice          ; cd2038000100
        cmp    byte ptr [esi + 8], 0
        jne    loc_1_0D72
        cmp    ax, 0xffff
        jne    loc_1_0D90
        VMMCall  VMM.Disable_VM_Ints             ; cd201b000100
loc_1_0D90:
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        pop    esi
        pop    ebx
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_0DB0:
        mov    ecx, dword ptr [esp + 4]
        cmp    ecx, 0x2710
        jbe    loc_1_0DBE
        jmp    loc_1_0DCC
loc_1_0DBE:
        mov    eax, dword ptr [esp + 8]
        add    eax, eax
        sub    edx, edx
        div    ecx
        or     eax, eax
        jne    loc_1_0DD0
loc_1_0DCC:
        mov    ax, 1
loc_1_0DD0:
        ret    
loc_1_0DD1:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 0x14]
        mov    esi, dword ptr [ebx + 0x3c]
        or     esi, esi
        je     loc_1_0F27
        mov    eax, dword ptr [esp + 0x18]
        cmp    eax, 0x40010008
        je     loc_1_0DFF
        cmp    eax, 0x40010009
        je     loc_1_0EDD
        jmp    loc_1_0F27
loc_1_0DFF:
        mov    edi, dword ptr [esp + 0x1c]
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [ebx + 0x1c], eax
        mov    dword ptr [esi + 0xc], eax
        add    eax, 0xe
        mov    dword ptr [esi + 0x10], eax
        mov    eax, dword ptr [edi]
        mov    dword ptr [ebx + 0x20], eax
        push   dword ptr [ebx + 0x1c]
        push   eax
        call   sub_1_0DB0
        add    esp, 8
        movzx  eax, ax
        mov    dword ptr [ebx + 0x24], eax
        mov    word ptr [esi + 0x24], ax
        push   6
        lea    eax, [edi + 0x14]
        push   eax
        lea    eax, [esi + 0x1c]
        push   eax
        call   sub_1_4AF3
        add    esp, 0xc
        push   6
        add    edi, 0xe
        push   edi
        push   0x57a0                        ; -> obj1:0x57A0
        call   sub_1_4AF3
        add    esp, 0xc
        and    dword ptr [esi + 8], 0xffffffef
        test   byte ptr [0x58f8], 0xc0       ; -> loc_1_58F8
        jne    loc_1_0EB6
        sub    ebp, ebp
        cmp    dword ptr [0x58c8], ebp       ; -> loc_1_58C8
        jbe    loc_1_0E9D
        mov    edi, 0x58dc                   ; -> loc_1_58DC
        sub    ebx, ebx
loc_1_0E70:
        test   byte ptr [0x58f8], 0x10       ; -> loc_1_58F8
        je     loc_1_0E83
        push   dword ptr [edi]
        call   sub_1_6CBB
        add    esp, 4
loc_1_0E83:
        mov    eax, dword ptr [edi]
        cmp    esi, eax
        je     loc_1_0E91
        mov    dword ptr [eax + 0x18], ebx
        mov    eax, dword ptr [edi]
        mov    byte ptr [eax + 0x23], bl
loc_1_0E91:
        add    edi, 4
        inc    ebp
        cmp    dword ptr [0x58c8], ebp       ; -> loc_1_58C8
        ja     loc_1_0E70
loc_1_0E9D:
        mov    dword ptr [0x58ec], esi       ; -> loc_1_58EC
        mov    dword ptr [0x5694], 0         ; -> loc_1_5694
        push   esi
        call   sub_1_2FF9
        add    esp, 4
loc_1_0EB6:
        test   byte ptr [0x58f8], 0x10       ; -> loc_1_58F8
        je     loc_1_0ED1
        cmp    dword ptr [0x7d08], 0         ; -> loc_1_7D08
        jbe    loc_1_0ED1
        push   esi
        call   sub_1_7221
        add    esp, 4
loc_1_0ED1:
        and    dword ptr [0x58f8], 0xffff7fff ; -> loc_1_58F8
        jmp    loc_1_0F27
loc_1_0EDD:
        mov    eax, dword ptr [esi + 0x48]
        or     dword ptr [esi + 8], 0x10
        mov    dword ptr [eax + 0x20], 0
        test   byte ptr [0x58f8], 0x10       ; -> loc_1_58F8
        je     loc_1_0F07
        or     dword ptr [0x58f8], 0x8000    ; -> loc_1_58F8
        push   esi
        call   sub_1_6CBB
        add    esp, 4
loc_1_0F07:
        cmp    dword ptr [esi + 0x18], 0
        je     loc_1_0F18
        mov    dword ptr [esi + 0x18], 0
        mov    byte ptr [esi + 0x23], 0
loc_1_0F18:
        call   sub_1_1B15
        and    dword ptr [0x58f8], 0xffffff3f ; -> loc_1_58F8
loc_1_0F27:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    0x10
sub_1_0F2E:
        sub    esp, 0xc
        push   esi
        cmp    dword ptr [0x7c60], 1         ; -> loc_1_7C60
        jne    loc_1_0F45
        mov    eax, 1
        jmp    loc_1_104B
loc_1_0F45:
        mov    dword ptr [0x7c60], 1         ; -> loc_1_7C60
        push   -1
        push   0
        lea    eax, [esp + 0x10]
        push   0xc
        push   eax
        call   sub_1_2468
        cmp    dword ptr [esp + 8], 0
        jne    loc_1_0F7A
        mov    dword ptr [0x7c60], 0         ; -> loc_1_7C60
        mov    eax, 1
        jmp    loc_1_104B
loc_1_0F7A:
        cmp    dword ptr [0x5878], 1         ; -> loc_1_5878
        sbb    eax, eax
        and    eax, 0xfffffff0
        add    eax, 0x48
        push   eax
        mov    eax, dword ptr [esp + 0xc]
        push   dword ptr [0x582c]            ; -> loc_1_582C
        add    eax, 8
        push   eax
        lea    eax, [esp + 0x18]
        push   eax
        call   sub_1_2474
        cmp    dword ptr [esp + 0xc], 0
        je     loc_1_0FCA
        push   0
        push   0xc
        push   dword ptr [esp + 0x10]
        call   sub_1_246E
        mov    dword ptr [0x7c60], 0         ; -> loc_1_7C60
        mov    eax, 1
        jmp    loc_1_104B
loc_1_0FCA:
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax], 0x58a4       ; -> loc_1_58A4
        mov    ecx, dword ptr [0x58a8]       ; -> loc_1_58A8
        mov    eax, dword ptr [esp + 8]
        sub    esi, esi
        mov    dword ptr [eax + 4], ecx
        mov    ecx, dword ptr [0x58a8]       ; -> loc_1_58A8
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [0x58a8], eax       ; -> loc_1_58A8
        cmp    dword ptr [0x582c], esi       ; -> loc_1_582C
        jbe    loc_1_1034
loc_1_1000:
        mov    eax, dword ptr [esp + 8]
        push   dword ptr [eax + 8]
        lea    eax, [esp + 8]
        push   eax
        lea    eax, [esp + 0x14]
        push   eax
        call   sub_1_2480
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [0x58b4]       ; -> loc_1_58B4
        mov    dword ptr [ecx + 0x38], eax
        inc    esi
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [0x58b4], eax       ; -> loc_1_58B4
        cmp    dword ptr [0x582c], esi       ; -> loc_1_582C
        ja     loc_1_1000
loc_1_1034:
        mov    eax, dword ptr [0x582c]       ; -> loc_1_582C
        add    dword ptr [0x5848], eax       ; -> loc_1_5848
        mov    dword ptr [0x7c60], 0         ; -> loc_1_7C60
        sub    eax, eax
loc_1_104B:
        pop    esi
        add    esp, 0xc
        ret    
sub_1_1050:
        sub    esp, 0xc
        push   esi
        cmp    dword ptr [0x7c64], 1         ; -> loc_1_7C64
        jne    loc_1_1067
        mov    eax, 1
        jmp    loc_1_115E
loc_1_1067:
        mov    dword ptr [0x7c64], 1         ; -> loc_1_7C64
        push   -1
        push   0
        lea    eax, [esp + 0x10]
        push   0xc
        push   eax
        call   sub_1_2468
        cmp    dword ptr [esp + 8], 0
        jne    loc_1_109C
        mov    dword ptr [0x7c64], 0         ; -> loc_1_7C64
        mov    eax, 1
        jmp    loc_1_115E
loc_1_109C:
        mov    eax, dword ptr [esp + 8]
        push   dword ptr [0x5830]            ; -> loc_1_5830
        add    eax, 8
        push   eax
        lea    eax, [esp + 0x14]
        push   eax
        call   sub_1_2486
        cmp    dword ptr [esp + 0xc], 0
        je     loc_1_10DC
        push   0
        push   0xc
        push   dword ptr [esp + 0x10]
        call   sub_1_246E
        mov    dword ptr [0x7c64], 0         ; -> loc_1_7C64
        mov    eax, 1
        jmp    loc_1_115E
loc_1_10DC:
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax], 0x58ac       ; -> loc_1_58AC
        mov    eax, dword ptr [0x58b0]       ; -> loc_1_58B0
        mov    ecx, dword ptr [esp + 8]
        sub    esi, esi
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [0x58b0]       ; -> loc_1_58B0
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [0x58b0], eax       ; -> loc_1_58B0
        cmp    dword ptr [0x5830], esi       ; -> loc_1_5830
        jbe    loc_1_1147
loc_1_1110:
        push   0
        mov    eax, dword ptr [esp + 0xc]
        push   0
        push   dword ptr [eax + 8]
        lea    eax, [esp + 0x10]
        push   eax
        lea    eax, [esp + 0x1c]
        push   eax
        call   sub_1_2492
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [0x58b8]       ; -> loc_1_58B8
        mov    dword ptr [ecx], eax
        inc    esi
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [0x58b8], eax       ; -> loc_1_58B8
        cmp    esi, dword ptr [0x5830]       ; -> loc_1_5830
        jb     loc_1_1110
loc_1_1147:
        mov    eax, dword ptr [0x5830]       ; -> loc_1_5830
        add    dword ptr [0x584c], eax       ; -> loc_1_584C
        mov    dword ptr [0x7c64], 0         ; -> loc_1_7C64
        sub    eax, eax
loc_1_115E:
        pop    esi
        add    esp, 0xc
        ret    
sub_1_1163:
        sub    esp, 4
        mov    eax, dword ptr [0x5814]       ; -> obj1:0x5814
        or     eax, eax
        je     loc_1_117A
        push   eax
        lea    eax, [esp + 4]
        push   eax
        call   sub_1_24B0
loc_1_117A:
        add    esp, 4
        ret    
loc_1_117E:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 4]
        or     eax, 0x10000000
        mov    dword ptr [esi + 4], eax
        test   eax, 0x1000000
        je     loc_1_11A7
        push   dword ptr [esi + 0x14]
        push   0xc0000001
        push   dword ptr [esi + 0x10]
        call   sub_1_24CE
        jmp    loc_1_11B8
loc_1_11A7:
        test   eax, 0x2000000
        je     loc_1_11BE
        push   0
        push   dword ptr [esi + 0x10]
        call   sub_1_24D4
loc_1_11B8:
        dec    dword ptr [0x58c4]            ; -> loc_1_58C4
loc_1_11BE:
        and    dword ptr [esi + 4], 0xfcffffff
        mov    dword ptr [esi + 0x10], 0
        pop    esi
        ret    8
sub_1_11D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   esi
        push   edi
        mov    dword ptr [ebp - 0xc], 0
        mov    esi, dword ptr [ebp + 0xc]
        movzx  eax, word ptr [esi]
        mov    dword ptr [ebp - 8], eax
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 0xc]
        push   dword ptr [ebp - 8]
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ecx + 4], eax
        or     eax, eax
        jne    loc_1_1211
        sub    al, al
        jmp    loc_1_1234
loc_1_1211:
        mov    ax, word ptr [esi]
        mov    word ptr [ecx + 2], ax
        mov    word ptr [ecx], ax
        movzx  eax, word ptr [esi]
        mov    edi, dword ptr [ecx + 4]
        mov    ecx, eax
        mov    esi, dword ptr [esi + 4]
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    al, 1
loc_1_1234:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_123A:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   esi
        mov    esi, dword ptr [ebp + 8]
        cmp    word ptr [esi + 2], 0
        je     loc_1_1282
        mov    dword ptr [ebp - 8], 0
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ebp - 4], eax
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 4]
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
        mov    dword ptr [ebp - 0xc], eax
        mov    dword ptr [esi + 4], 0
        mov    word ptr [esi + 2], 0
        mov    word ptr [esi], 0
loc_1_1282:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_1287:
        push   esi
        push   edi
        mov    edx, dword ptr [esp + 0xc]
        mov    edi, dword ptr [esp + 0x10]
        mov    ax, word ptr [edx]
        cmp    word ptr [edi], ax
        jne    loc_1_12B7
        movzx  eax, ax
        mov    ecx, eax
        shr    ecx, 2
        mov    esi, dword ptr [edx + 4]
        mov    edi, dword ptr [edi + 4]
        repe cmpsd dword ptr [esi], dword ptr es:[edi]
        jne    loc_1_12B2
        mov    ecx, eax
        and    ecx, 3
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
loc_1_12B2:
        sete   al
        jmp    loc_1_12B9
loc_1_12B7:
        sub    al, al
loc_1_12B9:
        pop    edi
        pop    esi
        ret    
sub_1_12BC:
        sub    esp, 0xc
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x1c]
        mov    eax, dword ptr [0x58c4]       ; -> loc_1_58C4
        mov    dword ptr [esi], eax
        push   0
        mov    edi, dword ptr [esp + 0x1c]
        push   0x7c78                        ; -> loc_1_7C78
        lea    eax, [esp + 0x14]
        push   edi
        push   eax
        lea    eax, [esp + 0x20]
        push   eax
        call   sub_1_249E
        cmp    dword ptr [esp + 0x10], 0
        jne    loc_1_133B
        mov    eax, dword ptr [esp + 0xc]
        push   1
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [esi + 4], eax
        push   0x7c8c                        ; -> loc_1_7C8C
        lea    eax, [esp + 0x14]
        push   edi
        push   eax
        lea    eax, [esp + 0x20]
        push   eax
        call   sub_1_249E
        cmp    dword ptr [esp + 0x10], 0
        jne    loc_1_133B
        mov    eax, dword ptr [esp + 0xc]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [esp + 8], eax
        mov    eax, dword ptr [esp + 8]
        xchg   ah, al
        ror    eax, 0x10
        xchg   ah, al
        mov    dword ptr [esp + 8], eax
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [esi + 8], eax
        sub    eax, eax
        jmp    loc_1_1340
loc_1_133B:
        mov    eax, 1
loc_1_1340:
        pop    edi
        pop    esi
        add    esp, 0xc
        ret    
sub_1_1346:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x1c]
        or     esi, esi
        je     loc_1_136E
        mov    ebx, dword ptr [esi + 0x3c]
        or     ebx, ebx
        mov    edi, dword ptr [esp + 0x20]
        je     loc_1_13A1
loc_1_1360:
        cmp    dword ptr [ebx + 0x30], edi
        je     loc_1_13A1
        mov    ebx, dword ptr [ebx + 0x4c]
        or     ebx, ebx
        jne    loc_1_1360
        jmp    loc_1_13A1
loc_1_136E:
        sub    ecx, ecx
        cmp    dword ptr [0x5838], ecx       ; -> loc_1_5838
        jbe    loc_1_1396
        mov    eax, 0x58dc                   ; -> loc_1_58DC
        mov    edi, dword ptr [esp + 0x20]
loc_1_1381:
        mov    ebx, dword ptr [eax]
        cmp    dword ptr [ebx + 0x30], edi
        je     loc_1_139E
        add    eax, 4
        inc    ecx
        cmp    dword ptr [0x5838], ecx       ; -> loc_1_5838
        ja     loc_1_1381
        jmp    loc_1_139E
loc_1_1396:
        mov    edi, dword ptr [esp + 0x20]
        mov    ebx, dword ptr [esp + 0x10]
loc_1_139E:
        mov    esi, dword ptr [ebx + 0x48]
loc_1_13A1:
        or     ebx, ebx
        mov    eax, dword ptr [esp + 0x18]
        je     loc_1_153B
        mov    dword ptr [eax], 0
        sub    ebp, ebp
        cmp    dword ptr [0x5854], ebp       ; -> loc_1_5854
        jbe    loc_1_13DF
loc_1_13BD:
        mov    eax, dword ptr [0x586c]       ; -> loc_1_586C
        mov    eax, dword ptr [eax + ebp*4]
        cmp    dword ptr [eax + 8], ebx
        jne    loc_1_13D6
        push   1
        push   eax
        call   sub_1_0BD0
        add    esp, 8
        dec    ebp
loc_1_13D6:
        inc    ebp
        cmp    ebp, dword ptr [0x5854]       ; -> loc_1_5854
        jb     loc_1_13BD
loc_1_13DF:
        test   byte ptr [0x58f8], 0x10       ; -> loc_1_58F8
        je     loc_1_140C
        cmp    dword ptr [0x58c8], 2         ; -> loc_1_58C8
        ja     loc_1_1403
        test   byte ptr [0x58f9], 2          ; -> obj1:0x58F9
        jne    loc_1_1403
        push   0
        call   sub_1_6BC0
        jmp    loc_1_1409
loc_1_1403:
        push   ebx
        call   sub_1_6CBB
loc_1_1409:
        add    esp, 4
loc_1_140C:
        mov    dword ptr [ebx + 0x18], 0
        mov    dword ptr [ebx + 8], 0
        dec    dword ptr [0x58c8]            ; -> loc_1_58C8
        mov    ecx, dword ptr [ebx]
        cmp    dword ptr [0x58c8], ecx       ; -> loc_1_58C8
        je     loc_1_147C
        lea    edx, [ecx*4 + 0x58dc]         ; -> loc_1_58DC
        mov    eax, dword ptr [0x58c8]       ; -> loc_1_58C8
        mov    eax, dword ptr [eax*4 + 0x58dc] ; -> loc_1_58DC
        mov    dword ptr [edx], eax
        mov    dword ptr [eax], ecx
        mov    ebp, 1
        mov    eax, dword ptr [edx]
        shl    ebp, cl
        mov    dword ptr [eax + 4], ebp
        mov    eax, dword ptr [0x58c8]       ; -> loc_1_58C8
        mov    dword ptr [eax*4 + 0x58dc], ebx ; -> loc_1_58DC
        mov    eax, dword ptr [0x58c8]       ; -> loc_1_58C8
        mov    edx, 1
        mov    dword ptr [ebx], eax
        mov    cl, byte ptr [0x58c8]         ; -> loc_1_58C8
        mov    eax, dword ptr [0x58c8]       ; -> loc_1_58C8
        mov    eax, dword ptr [eax*4 + 0x58dc] ; -> loc_1_58DC
        shl    edx, cl
        mov    dword ptr [eax + 4], edx
loc_1_147C:
        cmp    dword ptr [0x58ec], ebx       ; -> loc_1_58EC
        jne    loc_1_1489
        call   sub_1_1B15
loc_1_1489:
        mov    ecx, dword ptr [esi + 0x3c]
        cmp    ebx, ecx
        jne    loc_1_1498
        mov    eax, dword ptr [ebx + 0x4c]
        mov    dword ptr [esi + 0x3c], eax
        jmp    loc_1_14B1
loc_1_1498:
        or     ecx, ecx
        je     loc_1_14B1
loc_1_149C:
        mov    eax, dword ptr [ecx + 0x4c]
        cmp    ebx, eax
        je     loc_1_14AB
        mov    ecx, eax
        or     ecx, ecx
        jne    loc_1_149C
        jmp    loc_1_14B1
loc_1_14AB:
        mov    eax, dword ptr [ebx + 0x4c]
        mov    dword ptr [ecx + 0x4c], eax
loc_1_14B1:
        dec    dword ptr [esi + 8]
        cmp    dword ptr [esi + 0x3c], 0
        jne    loc_1_1541
        and    dword ptr [esi + 4], 0xfffffffe
        call   sub_1_2196
        mov    eax, dword ptr [esi + 4]
        test   eax, 0x1000000
        jne    loc_1_14D9
        or     eax, 0x2000000
        mov    dword ptr [esi + 4], eax
loc_1_14D9:
        push   dword ptr [esi + 0xc]
        mov    dword ptr [esi + 0x10], edi
        push   dword ptr [esp + 0x1c]
        call   sub_1_24BC
        mov    eax, dword ptr [esp + 0x18]
        cmp    dword ptr [eax], 0
        jne    loc_1_1541
        dec    dword ptr [0x58c4]            ; -> loc_1_58C4
        and    dword ptr [esi + 4], 0xfdffffff
        mov    ecx, dword ptr [esi]
        cmp    dword ptr [0x58c4], 0         ; -> loc_1_58C4
        jbe    loc_1_1541
        cmp    dword ptr [0x58c4], ecx       ; -> loc_1_58C4
        je     loc_1_1541
        mov    eax, dword ptr [0x58c4]       ; -> loc_1_58C4
        mov    eax, dword ptr [eax*4 + 0x58cc] ; -> loc_1_58CC
        mov    dword ptr [ecx*4 + 0x58cc], eax ; -> loc_1_58CC
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [0x58c4]       ; -> loc_1_58C4
        mov    dword ptr [eax*4 + 0x58cc], esi ; -> loc_1_58CC
        mov    eax, dword ptr [0x58c4]       ; -> loc_1_58C4
        mov    dword ptr [esi], eax
        jmp    loc_1_1541
loc_1_153B:
        mov    dword ptr [eax], 0xc0000001
loc_1_1541:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    0xc
sub_1_154B:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ecx, dword ptr [0x7cf0]       ; -> loc_1_7CF0
        mov    edx, 1
        mov    eax, 6
        sub    ebx, ebx
loc_1_1561:
        cmp    byte ptr [ecx + 8], bl
        je     loc_1_1568
        mov    edx, ebx
loc_1_1568:
        add    ecx, 0x2a
        dec    eax
        jne    loc_1_1561
        or     edx, edx
        je     loc_1_158E
        push   0
        push   0x1ec
        push   dword ptr [0x7cf0]            ; -> loc_1_7CF0
        call   sub_1_246E
        mov    dword ptr [0x7cf0], 0         ; -> loc_1_7CF0
loc_1_158E:
        mov    esi, dword ptr [esp + 0x14]
        mov    ebp, 1
        mov    edi, dword ptr [esi + 0x48]
        sub    ecx, ecx
        mov    dl, byte ptr [edi + 0x31]
        lea    eax, [edi + 0x30]
loc_1_15A2:
        mov    bl, byte ptr [eax + ecx]
        cmp    dl, bl
        jae    loc_1_15AD
        mov    dl, bl
        mov    ebp, ecx
loc_1_15AD:
        inc    ecx
        cmp    ecx, 3
        jl     loc_1_15A2
        or     ebp, ebp
        je     loc_1_15C3
        cmp    ebp, 1
        je     loc_1_15D3
        cmp    ebp, 2
        je     loc_1_15EA
        jmp    loc_1_15FF
loc_1_15C3:
        mov    dword ptr [esi + 0x28], 0x4763 ; -> loc_1_4763
        mov    dword ptr [esi + 0x2c], 0x1f87 ; -> loc_1_1F87
        jmp    loc_1_15F8
loc_1_15D3:
        mov    dword ptr [esi + 0x28], 0x47c2 ; -> loc_1_47C2
        mov    dword ptr [esi + 0x2c], 0x1e60 ; -> loc_1_1E60
        mov    dword ptr [esi + 0x14], 3
        jmp    loc_1_15FF
loc_1_15EA:
        mov    dword ptr [esi + 0x28], 0x4712 ; -> loc_1_4712
        mov    dword ptr [esi + 0x2c], 0x1f31 ; -> loc_1_1F31
loc_1_15F8:
        mov    dword ptr [esi + 0x14], 0
loc_1_15FF:
        mov    eax, dword ptr [edi + 0x1c]
        sub    eax, dword ptr [esi + 0x14]
        mov    dword ptr [esi + 0xc], eax
        mov    eax, dword ptr [edi + 0x1c]
        add    dword ptr [esi + 0x10], eax
        or     dword ptr [0x58f8], 1         ; -> loc_1_58F8
        and    dword ptr [0x58f8], 0xfffff7ff ; -> loc_1_58F8
        test   byte ptr [0x58f8], 0x10       ; -> loc_1_58F8
        je     loc_1_1637
        cmp    dword ptr [esi + 0x18], 0
        je     loc_1_1637
        push   esi
        call   sub_1_7221
        add    esp, 4
loc_1_1637:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_163C:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x1c]
        mov    eax, dword ptr [esi + 0x48]
        cmp    dword ptr [eax + 0x18], 0
        je     loc_1_1655
        jmp    loc_1_17C2
loc_1_1655:
        cmp    word ptr [0x7cf4], 0          ; -> loc_1_7CF4
        jne    loc_1_1683
        push   1
        push   0
        push   0x24df                        ; -> loc_1_24DF
        push   0x7cf4                        ; -> loc_1_7CF4
        call   sub_1_5206
        add    esp, 0x10
        or     eax, eax
        je     loc_1_1683
        mov    eax, 1
        jmp    loc_1_17C4
loc_1_1683:
        push   -1
        push   0
        push   0x1ec
        push   0x7cf0                        ; -> loc_1_7CF0
        call   sub_1_2468
        cmp    dword ptr [0x7cf0], 0         ; -> loc_1_7CF0
        jne    loc_1_16A9
        mov    eax, 1
        jmp    loc_1_17C4
loc_1_16A9:
        mov    eax, dword ptr [esi + 0x48]
        mov    byte ptr [eax + 0x2f], 0
        mov    eax, dword ptr [esi + 0x48]
        mov    dword ptr [eax + 0x30], 0
        and    dword ptr [0x58f8], 0xfffffffe ; -> loc_1_58F8
        mov    ebx, dword ptr [0x7cf0]       ; -> loc_1_7CF0
        or     dword ptr [0x58f8], 0x800     ; -> loc_1_58F8
        lea    ebp, [ebx + 0xfc]
        mov    eax, dword ptr [0x58ec]       ; -> loc_1_58EC
        mov    dword ptr [esp + 0x14], eax
        mov    dword ptr [0x58ec], esi       ; -> loc_1_58EC
        mov    dword ptr [esp + 0x10], 0
loc_1_16EE:
        mov    edi, ebx
        sub    eax, eax
        mov    ecx, 0xa
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    dword ptr [ebx + 0x14], esi
        mov    ax, word ptr [0x7cf4]         ; -> loc_1_7CF4
        mov    word ptr [ebx + 0xa], ax
        lea    edi, [ebx + 0x1c]
        mov    eax, 0xffffffff
        stosd  dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    word ptr [ebx + 0x22], 1
        mov    dword ptr [ebx + 0x24], ebp
        lea    edi, [ebp + 0xa]
        mov    word ptr [ebx + 0x28], 0x28
        mov    byte ptr [ebp + 5], 1
        mov    dword ptr [ebp + 6], 0
        stosd  dword ptr es:[edi], eax
        mov    ecx, 3
        stosw  word ptr es:[edi], ax
        lea    eax, [ebp + 0x1e]
        mov    word ptr [ebp + 0x10], 0x5304
        mov    word ptr [eax], 0x100
        mov    dword ptr [eax + 2], 0xffffffff
        mov    eax, dword ptr [esp + 0x10]
        cdq    
        idiv   ecx
        or     edx, edx
        je     loc_1_1764
        cmp    edx, 1
        je     loc_1_176D
        cmp    edx, 2
        je     loc_1_1776
        jmp    loc_1_177D
loc_1_1764:
        mov    dword ptr [esi + 0x28], 0x4763 ; -> loc_1_4763
        jmp    loc_1_177D
loc_1_176D:
        mov    dword ptr [esi + 0x28], 0x47c2 ; -> loc_1_47C2
        jmp    loc_1_177D
loc_1_1776:
        mov    dword ptr [esi + 0x28], 0x4712 ; -> loc_1_4712
loc_1_177D:
        mov    edi, dword ptr [esi + 0x18]
        mov    dword ptr [esi + 0x18], 0
        push   esi
        push   ebx
        call   sub_1_4092
        add    esp, 8
        mov    dword ptr [esi + 0x18], edi
        add    ebx, 0x2a
        add    ebp, 0x28
        inc    dword ptr [esp + 0x10]
        cmp    dword ptr [esp + 0x10], 6
        jl     loc_1_16EE
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [0x58ec], eax       ; -> loc_1_58EC
        mov    dword ptr [esi + 0x28], 0x47c2 ; -> loc_1_47C2
        push   esi
        call   sub_1_40AC
        add    esp, 4
loc_1_17C2:
        sub    eax, eax
loc_1_17C4:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
loc_1_17CC:
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x2c]
        mov    dword ptr [esi], 0xc0000001
        mov    eax, dword ptr [0x5838]       ; -> loc_1_5838
        cmp    dword ptr [0x58c8], eax       ; -> loc_1_58C8
        jb     loc_1_17F5
        mov    dword ptr [esi], 0xc000009a
        jmp    loc_1_1ACD
loc_1_17F5:
        lea    eax, [esp + 0x10]
        push   dword ptr [esp + 0x38]
        push   eax
        push   esi
        call   sub_1_24C8
        cmp    dword ptr [esi], 0
        je     loc_1_1814
        mov    dword ptr [esi], 0xc0010015
        jmp    loc_1_1ACD
loc_1_1814:
        lea    eax, [esp + 0x18]
        push   eax
        push   dword ptr [esp + 0x14]
        call   sub_1_12BC
        add    esp, 8
        mov    edi, eax
        push   dword ptr [esp + 0x10]
        call   sub_1_24A4
        cmp    edi, 1
        jne    loc_1_1840
        mov    dword ptr [esi], 0xc0010015
        jmp    loc_1_1ACD
loc_1_1840:
        sub    edi, edi
        cmp    dword ptr [0x5834], edi       ; -> loc_1_5834
        jbe    loc_1_187B
        mov    ebp, 0x58cc                   ; -> loc_1_58CC
        mov    ebx, dword ptr [esp + 0x34]
loc_1_1853:
        mov    eax, dword ptr [ebp]
        test   byte ptr [eax + 4], 1
        je     loc_1_186D
        push   ebx
        add    eax, 0x34
        push   eax
        call   sub_1_1287
        add    esp, 8
        cmp    al, 1
        je     loc_1_187F
loc_1_186D:
        add    ebp, 4
        inc    edi
        cmp    dword ptr [0x5834], edi       ; -> loc_1_5834
        ja     loc_1_1853
        jmp    loc_1_187F
loc_1_187B:
        mov    ebx, dword ptr [esp + 0x34]
loc_1_187F:
        cmp    dword ptr [0x5834], edi       ; -> loc_1_5834
        je     loc_1_1898
        mov    eax, dword ptr [edi*4 + 0x58cc] ; -> loc_1_58CC
        test   byte ptr [eax + 4], 1
        jne    loc_1_192E
loc_1_1898:
        sub    edi, edi
        cmp    dword ptr [0x5834], edi       ; -> loc_1_5834
        jbe    loc_1_18C5
        mov    eax, 0x58cc                   ; -> loc_1_58CC
        mov    ecx, 1
loc_1_18AC:
        mov    edx, dword ptr [eax]
        test   dword ptr [edx + 4], ecx
        je     loc_1_18BF
        add    eax, 4
        inc    edi
        cmp    dword ptr [0x5834], edi       ; -> loc_1_5834
        ja     loc_1_18AC
loc_1_18BF:
        cmp    dword ptr [0x5834], edi       ; -> loc_1_5834
loc_1_18C5:
        jne    loc_1_18D2
        mov    dword ptr [esi], 0xc000009a
        jmp    loc_1_1ACD
loc_1_18D2:
        lea    ebp, [edi*4 + 0x58cc]         ; -> loc_1_58CC
        push   ebx
        mov    eax, dword ptr [ebp]
        add    eax, 0x34
        push   eax
        call   sub_1_11D0
        add    esp, 8
        or     al, al
        jne    loc_1_18F8
        mov    dword ptr [esi], 0xc000009a
        jmp    loc_1_1ACD
loc_1_18F8:
        push   dword ptr [0x58c4]            ; -> loc_1_58C4
        push   dword ptr [ebp]
        push   ebx
        call   sub_4_02A7
        add    esp, 0xc
        or     eax, eax
        je     loc_1_1928
        mov    eax, dword ptr [ebp]
        add    eax, 0x34
        push   eax
        call   sub_1_123A
        add    esp, 4
        mov    dword ptr [esi], 0xc0010007
        jmp    loc_1_1ACD
loc_1_1928:
        inc    dword ptr [0x58c4]            ; -> loc_1_58C4
loc_1_192E:
        sub    ecx, ecx
        cmp    dword ptr [0x5838], ecx       ; -> loc_1_5838
        jbe    loc_1_1955
        mov    eax, 0x58dc                   ; -> loc_1_58DC
        mov    edx, 1
loc_1_1942:
        mov    ebx, dword ptr [eax]
        test   dword ptr [ebx + 8], edx
        je     loc_1_1955
        add    eax, 4
        inc    ecx
        cmp    dword ptr [0x5838], ecx       ; -> loc_1_5838
        ja     loc_1_1942
loc_1_1955:
        lea    ebx, [ecx*4 + 0x58dc]         ; -> loc_1_58DC
        mov    eax, dword ptr [ebx]
        mov    ebp, dword ptr [esp + 0x30]
        mov    dword ptr [eax + 0x30], ebp
        mov    edx, dword ptr [ebx]
        lea    edi, [edi*4 + 0x58cc]         ; -> loc_1_58CC
        mov    eax, dword ptr [esp + 0x3c]
        mov    dword ptr [edx + 0x34], eax
        inc    dword ptr [0x58c8]            ; -> loc_1_58C8
        push   ecx
        push   dword ptr [edi]
        lea    eax, [esp + 0x20]
        push   dword ptr [ebx]
        push   eax
        call   sub_4_077F
        add    esp, 0x10
        or     eax, eax
        je     loc_1_199F
        mov    eax, dword ptr [ebx]
        and    dword ptr [eax + 8], 0xfffffffe
        mov    dword ptr [esi], 0xc0010015
        jmp    loc_1_19E9
loc_1_199F:
        cmp    word ptr [0x7c68], 0          ; -> loc_1_7C68
        je     loc_1_1A4D
        test   byte ptr [0x58f9], 2          ; -> obj1:0x58F9
        jne    loc_1_19BF
        cmp    dword ptr [0x58c8], 1         ; -> loc_1_58C8
        jbe    loc_1_1A3D
loc_1_19BF:
        push   dword ptr [0x55e8]            ; -> obj1:0x55E8
        push   dword ptr [0x55e4]            ; -> obj1:0x55E4
        push   dword ptr [0x55e0]            ; -> obj1:0x55E0
        push   dword ptr [0x55dc]            ; -> loc_1_55DC
        call   sub_1_6F66
        add    esp, 0x10
        or     eax, eax
        je     loc_1_1A2A
        mov    dword ptr [esi], 0xc0000001
loc_1_19E9:
        dec    dword ptr [0x58c8]            ; -> loc_1_58C8
        mov    eax, dword ptr [edi]
        push   ebp
        or     dword ptr [eax + 4], 0x1000000
        mov    ecx, dword ptr [edi]
        mov    eax, dword ptr [esi]
        mov    dword ptr [ecx + 0x14], eax
        lea    eax, [esp + 0x18]
        push   dword ptr [edi]
        push   eax
        call   sub_1_1346
        cmp    dword ptr [esp + 0x14], 0x103
        jne    loc_1_1A1C
        mov    dword ptr [esi], 0x103
loc_1_1A1C:
        mov    eax, dword ptr [edi]
        and    dword ptr [eax + 4], 0xfeffffff
        jmp    loc_1_1ACD
loc_1_1A2A:
        cmp    dword ptr [0x55ec], 0         ; -> obj1:0x55EC
        je     loc_1_1A3D
        or     dword ptr [0x58f8], 0x400     ; -> loc_1_58F8
loc_1_1A3D:
        call   sub_1_60B0
        mov    word ptr [0x7c68], 0          ; -> loc_1_7C68
        jmp    loc_1_1AC7
loc_1_1A4D:
        cmp    dword ptr [0x58c8], 1         ; -> loc_1_58C8
        jbe    loc_1_1AC7
        test   byte ptr [0x58f8], 0x10       ; -> loc_1_58F8
        jne    loc_1_1A94
        push   dword ptr [0x55e8]            ; -> obj1:0x55E8
        push   dword ptr [0x55e4]            ; -> obj1:0x55E4
        push   dword ptr [0x55e0]            ; -> obj1:0x55E0
        push   dword ptr [0x55dc]            ; -> loc_1_55DC
        call   sub_1_6F66
        add    esp, 0x10
        cmp    dword ptr [0x55ec], 0         ; -> obj1:0x55EC
        je     loc_1_1AB2
        or     dword ptr [0x58f8], 0x400     ; -> loc_1_58F8
        jmp    loc_1_1AB2
loc_1_1A94:
        push   0x5204
        push   dword ptr [ebx]
        call   sub_1_6CEF
        add    esp, 8
        push   0x5304
        push   dword ptr [ebx]
        call   sub_1_6CEF
        add    esp, 8
loc_1_1AB2:
        mov    eax, dword ptr [0x58ec]       ; -> loc_1_58EC
        mov    eax, dword ptr [eax + 8]
        test   al, 8
        je     loc_1_1AC7
        test   al, 0x10
        je     loc_1_1AC7
        call   sub_1_1B15
loc_1_1AC7:
        mov    dword ptr [esi], 0
loc_1_1ACD:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x18
        ret    0x14
loc_1_1AD7:
        jmp    loc_1_4CB6
sub_1_1ADC:
        mov    eax, dword ptr [0x58f0]       ; -> loc_1_58F0
        imul   eax, eax, 0x41c64e6d
        add    eax, 0x3039
        mov    dword ptr [0x58f0], eax       ; -> loc_1_58F0
        je     loc_1_1AF8
        cmp    eax, -1
        jne    loc_1_1AFD
loc_1_1AF8:
        mov    eax, 1
loc_1_1AFD:
        mov    dword ptr [0x58f0], eax       ; -> loc_1_58F0
        push   eax
        call   sub_1_2D02
        add    esp, 4
        or     eax, eax
        jne    sub_1_1ADC
        mov    eax, dword ptr [0x58f0]       ; -> loc_1_58F0
        ret    
sub_1_1B15:
        push   esi
        push   edi
        mov    ecx, dword ptr [0x58dc]       ; -> loc_1_58DC
        sub    edx, edx
        cmp    dword ptr [0x58c8], edx       ; -> loc_1_58C8
        jbe    loc_1_1B4D
        mov    eax, 0x58dc                   ; -> loc_1_58DC
        mov    esi, 8
loc_1_1B31:
        mov    edi, dword ptr [eax]
        test   dword ptr [edi + 8], esi
        je     loc_1_1B46
        add    eax, 4
        inc    edx
        cmp    dword ptr [0x58c8], edx       ; -> loc_1_58C8
        ja     loc_1_1B31
        jmp    loc_1_1B4D
loc_1_1B46:
        mov    ecx, dword ptr [edx*4 + 0x58dc] ; -> loc_1_58DC
loc_1_1B4D:
        cmp    dword ptr [0x58ec], ecx       ; -> loc_1_58EC
        je     loc_1_1B64
        mov    dword ptr [0x58ec], ecx       ; -> loc_1_58EC
        push   ecx
        call   sub_1_2FF9
        add    esp, 4
loc_1_1B64:
        pop    edi
        pop    esi
        ret    
        db  000h
loc_1_1B68:
        ret    8
loc_1_1B6B:
        test   dword ptr [0x58f8], 0x920     ; -> loc_1_58F8
        jne    loc_1_1B94
        mov    eax, dword ptr [esp + 4]
        cld    
        sub    esp, 0x14
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        call   sub_1_1C36
        pop    ebx
        pop    edi
        pop    esi
        pop    ebp
        add    esp, 0x14
loc_1_1B91:
        ret    0x1c
loc_1_1B94:
        test   dword ptr [0x58f8], 0x800     ; -> loc_1_58F8
        je     loc_1_1B91
        mov    eax, dword ptr [esp + 4]
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ecx
        cmp    dword ptr [ebp + 0x14], 0xe
        jb     loc_1_1BD9
        mov    esi, dword ptr [ebp + 0x10]
        movzx  ecx, word ptr [esi + 0xc]
        cmp    ecx, 0x3781
        je     loc_1_1BF0
        xchg   cl, ch
        cmp    ecx, 0x600
        jae    loc_1_1BD9
        mov    esi, dword ptr [ebp + 0x18]
        cmp    word ptr [esi], -1
        je     loc_1_1BE9
        cmp    word ptr [esi], 0xe0e0
        je     loc_1_1BDF
loc_1_1BD9:
        pop    ecx
        pop    edi
        pop    esi
        pop    ebp
        jmp    loc_1_1B91
loc_1_1BDF:
        mov    eax, 1
        add    esi, 3
        jmp    loc_1_1BF8
loc_1_1BE9:
        mov    eax, 0
        jmp    loc_1_1BF8
loc_1_1BF0:
        mov    eax, 2
        mov    esi, dword ptr [ebp + 0x18]
loc_1_1BF8:
        cmp    word ptr [esi], -1
        jne    loc_1_1BD9
        movzx  ecx, word ptr [esi + 2]
        xchg   cl, ch
        cmp    ecx, 0x28
        jb     loc_1_1BD9
        cmp    word ptr [esi + 0x1c], 0x5304
        jne    loc_1_1BD9
        cmp    word ptr [esi + 0x1e], 0x200
        jne    loc_1_1BD9
        mov    ecx, esi
        mov    esi, dword ptr [ebp + 8]
        xchg   ecx, eax
        push   ecx
        mov    edi, dword ptr [esi + 0x3c]
        call   sub_1_211D
        pop    ecx
        cmp    byte ptr [ecx + esi + 0x30], 0xff
        je     loc_1_1BD9
        inc    byte ptr [ecx + esi + 0x30]
        jmp    loc_1_1BD9
sub_1_1C36:
        mov    ebx, eax
        mov    edi, dword ptr [ebx + 0x3c]
        or     edi, edi
        je     loc_1_1C4F
loc_1_1C3F:
        mov    eax, dword ptr [edi + 0x14]
        mov    dword ptr [ebp + 0x14], eax
        jmp    dword ptr [edi + 0x2c]
loc_1_1C48:
        mov    edi, dword ptr [edi + 0x4c]
        or     edi, edi
        jne    loc_1_1C3F
loc_1_1C4F:
        mov    eax, 0x10001
        ret    
loc_1_1C55:
        call   sub_1_0F2E
        mov    dword ptr [0x5670], 0         ; -> obj1:0x5670
        ret    
sub_1_1C65:
        push   esi
        mov    esi, dword ptr [0x5670]       ; -> obj1:0x5670
        VMMCall  VMM.Cancel_Restricted_Event     ; cd205b010100
        mov    dword ptr [0x5670], 0         ; -> obj1:0x5670
        pop    esi
sub_1_1C7D:
        push   ebx
        VxDCall  PAGESWAP.PageSwap_Get_Version   ; cd2000000700
        cmp    bl, 2
        jne    loc_1_1CB0
        cmp    dword ptr [0x5670], 0         ; -> obj1:0x5670
        jne    loc_1_1CB5
        push   esi
        mov    esi, 0x1c55                   ; -> loc_1_1C55
        xor    eax, eax
        xor    ebx, ebx
        mov    ecx, 2
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
        mov    dword ptr [0x5670], esi       ; -> obj1:0x5670
        pop    esi
        pop    ebx
        ret    
loc_1_1CB0:
        call   sub_1_0F2E
loc_1_1CB5:
        pop    ebx
        ret    
loc_1_1CB7:
        call   sub_1_1050
        mov    dword ptr [0x5674], 0         ; -> obj1:0x5674
        ret    
sub_1_1CC7:
        push   esi
        mov    esi, dword ptr [0x5674]       ; -> obj1:0x5674
        VMMCall  VMM.Cancel_Restricted_Event     ; cd205b010100
        mov    dword ptr [0x5674], 0         ; -> obj1:0x5674
        pop    esi
sub_1_1CDF:
        push   ebx
        VxDCall  PAGESWAP.PageSwap_Get_Version   ; cd2000000700
        cmp    bl, 2
        jne    loc_1_1D12
        cmp    dword ptr [0x5674], 0         ; -> obj1:0x5674
        jne    loc_1_1D17
        push   esi
        mov    esi, 0x1cb7                   ; -> loc_1_1CB7
        xor    eax, eax
        xor    ebx, ebx
        mov    ecx, 2
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
        mov    dword ptr [0x5674], esi       ; -> obj1:0x5674
        pop    esi
        pop    ebx
        ret    
loc_1_1D12:
        call   sub_1_1050
loc_1_1D17:
        pop    ebx
        ret    
loc_1_1D19:
        push   eax
        push   edx
        cmp    dword ptr [0x5848], 3         ; -> loc_1_5848
        jl     loc_1_1DD7
loc_1_1D28:
        cmp    dword ptr [0x584c], 0x14      ; -> loc_1_584C
        jl     loc_1_1DEE
loc_1_1D35:
        push   esi
        push   edi
        mov    ebx, edi
        push   dword ptr [esi + 4]
        mov    ecx, dword ptr [esi]
        and    cl, byte ptr [edi + 0x22]
        push   ecx
        mov    esi, esp
        mov    ebx, edi
        mov    edi, dword ptr [ebp + 0x2c]
        mov    ecx, dword ptr [ebp + 0x30]
        mov    edx, dword ptr [eax + 0x34]
        push   dword ptr [eax + 0x30]
        movzx  eax, word ptr [eax + 0x14]
        dec    dword ptr [0x5848]            ; -> loc_1_5848
        sub    dword ptr [0x584c], 0x14      ; -> loc_1_584C
        call   dword ptr [esp]
        inc    dword ptr [0x5848]            ; -> loc_1_5848
        add    dword ptr [0x584c], 0x14      ; -> loc_1_584C
        add    esp, 0xc
        mov    ecx, esi
        pop    edi
        pop    esi
        jecxz  loc_1_1DD4
        cmp    word ptr [ecx + 0x22], 0x14
        ja     loc_1_1DD4
        mov    dword ptr [ecx + 0xc], 0
        mov    dword ptr [ecx + 0x18], 0x4923 ; -> loc_1_4923
        mov    byte ptr [ecx + 9], 0
        push   ecx
        call   sub_1_4488
        cmp    ebx, -1
        jne    loc_1_1DA6
        xor    ebx, ebx
        or     byte ptr [eax + 0x3c], 8
loc_1_1DA6:
        pop    ecx
        pop    edx
        pop    eax
        sub    dword ptr [ebp + 0x34], ebx
        jb     loc_1_1DC0
        sub    dword ptr [ebp + 0x30], ebx
        jb     loc_1_1DCB
        add    dword ptr [ebp + 0x2c], ebx
loc_1_1DB6:
        add    dword ptr [ebp + 0x14], ebx
        sub    edx, ebx
        jmp    loc_1_2045
loc_1_1DC0:
        mov    byte ptr [ecx + 9], 1
        push   dword ptr [ecx]
        jmp    loc_1_2095
loc_1_1DCB:
        mov    dword ptr [ebp + 0x30], 0
        jmp    loc_1_1DB6
loc_1_1DD4:
        pop    edx
        pop    eax
        ret    
loc_1_1DD7:
        push   eax
        push   edx
        call   sub_1_1C7D
        pop    edx
        pop    eax
        cmp    dword ptr [0x5848], 1         ; -> loc_1_5848
        jl     loc_1_1E0D
        jmp    loc_1_1D28
loc_1_1DEE:
        push   eax
        push   edx
        call   sub_1_1CDF
        pop    edx
        pop    eax
        cmp    dword ptr [0x584c], 0x14      ; -> loc_1_584C
        jl     loc_1_1E0D
        cmp    dword ptr [0x5848], 1         ; -> loc_1_5848
        jge    loc_1_1D35
loc_1_1E0D:
        jmp    loc_1_1DD4
        db  083h, 07Dh, 028h, 00Eh, 072h, 04Ah, 08Bh, 075h, 024h, 00Fh, 0B7h, 046h, 00Ch, 086h, 0C4h, 03Dh
        db  000h, 006h, 000h, 000h, 00Fh, 083h, 01Fh, 0FEh, 0FFh, 0FFh, 08Bh, 045h, 02Ch, 066h, 081h, 038h
        db  0AAh, 0AAh, 00Fh, 085h, 011h, 0FEh, 0FFh, 0FFh, 083h, 0C0h, 003h, 066h, 081h, 078h, 003h, 081h
        db  037h, 00Fh, 085h, 002h, 0FEh, 0FFh, 0FFh, 089h, 075h, 00Ch, 083h, 0C6h, 006h, 0BAh, 008h, 000h
        db  000h, 000h, 001h, 055h, 02Ch, 029h, 055h, 030h, 029h, 055h, 034h, 0E9h, 05Bh, 001h, 000h, 000h
        db  0C3h
loc_1_1E60:
        cmp    dword ptr [ebp + 0x28], 0xe
        jb     loc_1_1EA1
        mov    esi, dword ptr [ebp + 0x24]
        movzx  eax, word ptr [esi + 0xc]
        xchg   ah, al
        cmp    eax, 0x600
        jae    loc_1_1C48
        mov    eax, dword ptr [ebp + 0x2c]
        cmp    word ptr [eax], 0xe0e0
        jne    loc_1_1C48
        mov    dword ptr [ebp + 0xc], esi
        add    esi, 6
        mov    edx, 3
        add    dword ptr [ebp + 0x2c], edx
        sub    dword ptr [ebp + 0x30], edx
        sub    dword ptr [ebp + 0x34], edx
        jmp    sub_1_1FBA
loc_1_1EA1:
        ret    
loc_1_1EA2:
        cmp    dword ptr [ebp + 0x28], 0xe
        jb     loc_1_1F81
        mov    eax, dword ptr [ebp + 0x2c]
        cmp    word ptr [eax], 0xaaaa
        jne    loc_1_1F81
        cmp    word ptr [eax + 6], 0x3781
        jne    loc_1_1F81
        call   sub_1_1F0E
        mov    edx, 8
        add    dword ptr [ebp + 0x2c], edx
        sub    dword ptr [ebp + 0x30], edx
        sub    dword ptr [ebp + 0x34], edx
        jmp    sub_1_1FBA
loc_1_1EDE:
        cmp    dword ptr [ebp + 0x28], 0xe
        jb     loc_1_1F81
        mov    eax, dword ptr [ebp + 0x2c]
        cmp    word ptr [eax], 0xe0e0
        jne    loc_1_1F81
        call   sub_1_1F0E
        mov    edx, 3
        add    dword ptr [ebp + 0x2c], edx
        sub    dword ptr [ebp + 0x30], edx
        sub    dword ptr [ebp + 0x34], edx
        jmp    sub_1_1FBA
sub_1_1F0E:
        mov    esi, dword ptr [ebp + 0x24]
        add    esi, 2
        mov    dword ptr [ebp + 0xc], esi
        add    esi, 6
        test   byte ptr [esi - 6], 0x80
        jne    loc_1_1F30
        test   dword ptr [edi + 8], 2
        je     loc_1_1F30
        push   eax
        call   sub_1_2292
        pop    eax
loc_1_1F30:
        ret    
loc_1_1F31:
        cmp    dword ptr [ebp + 0x28], 0xe
        jne    loc_1_1C48
        mov    esi, dword ptr [ebp + 0x24]
        mov    ax, word ptr [esi + 0xc]
        cmp    ax, 0x3781
        jne    loc_1_1C48
        mov    dword ptr [ebp + 0xc], esi
        add    esi, 6
        jmp    sub_1_1FBA
loc_1_1F54:
        cmp    dword ptr [ebp + 0x28], 3
        jne    loc_1_1F81
        mov    eax, dword ptr [ebp + 0x24]
        cmp    byte ptr [eax + 2], 0xfa
        lea    esi, [ebp + 4]
        xor    edx, edx
        mov    dword ptr [esi], edx
        mov    byte ptr [esi + 4], dl
        mov    cl, byte ptr [eax]
        mov    byte ptr [esi + 5], cl
        push   edx
        mov    dh, byte ptr [eax + 1]
        push   edx
        mov    dword ptr [ebp + 0xc], esp
        call   sub_1_1FBA
        add    esp, 8
        ret    
loc_1_1F81:
        mov    eax, 0
        ret    
loc_1_1F87:
        cmp    dword ptr [ebp + 0x28], 0xe
        jne    loc_1_1F81
        mov    esi, dword ptr [ebp + 0x24]
        movzx  eax, word ptr [esi + 0xc]
        xchg   ah, al
        cmp    eax, 0x600
        jae    loc_1_1C48
        cmp    ax, 0x1e
        jb     loc_1_1F81
        mov    eax, dword ptr [ebp + 0x2c]
        cmp    word ptr [eax], -1
        jne    loc_1_1C48
        mov    dword ptr [ebp + 0xc], esi
        add    esi, 6
sub_1_1FBA:
        mov    eax, dword ptr [ebp + 0x2c]
        movzx  edx, word ptr [eax + 2]
        xchg   dh, dl
        cmp    dword ptr [ebp + 0x34], edx
        jb     loc_1_1F81
        inc    dword ptr [0x5922]            ; -> loc_1_5922
        test   dword ptr [0x58f8], 0x10      ; -> loc_1_58F8
        je     sub_1_1FFC
        test   dword ptr [0x58f8], 0x40      ; -> loc_1_58F8
        je     sub_1_1FFC
        cmp    byte ptr [eax + 5], 0x14
        je     loc_1_2809
        mov    ecx, dword ptr [edi + 0x18]
        cmp    dword ptr [eax + 6], ecx
        jne    loc_1_26E0
sub_1_1FFC:
        cmp    word ptr [eax + 0x1c], 0x5304
        je     loc_1_2113
loc_1_2008:
        push   dword ptr [eax + 0x10]
        call   sub_1_42A4
        pop    ecx
        jb     loc_1_1F81
        test   dword ptr [eax + 0x18], 8
        jne    loc_1_1D19
        mov    ecx, dword ptr [eax + 0x20]
        or     ecx, ecx
        je     loc_1_20B9
        mov    ebx, dword ptr [ecx + 0x10]
        mov    dword ptr [eax + 0x20], ebx
        or     ebx, ebx
        jne    loc_1_2045
        mov    ebx, dword ptr [0x5864]       ; -> loc_1_5864
        add    ebx, dword ptr [eax + 8]
        and    byte ptr [ebx], 0xfe
loc_1_2045:
        lodsd  eax, dword ptr [esi]
        and    al, byte ptr [edi + 0x22]
        mov    dword ptr [ecx + 0x1c], eax
        lodsw  ax, word ptr [esi]
        mov    word ptr [ecx + 0x20], ax
        or     dword ptr [ecx + 0xc], 0x2000
        mov    ebx, dword ptr [ecx]
        mov    dword ptr [ebx + 0x40], edi
        cmp    dword ptr [ebx + 4], edx
        jb     loc_1_20D7
loc_1_2063:
        push   ebx
        or     edx, edx
        je     loc_1_2095
        cmp    dword ptr [ebp + 0x30], edx
        jb     loc_1_20E0
        mov    ebx, dword ptr [ebx + 8]
        mov    esi, dword ptr [ebp + 0x2c]
loc_1_2073:
        mov    ecx, dword ptr [ebx + 0xc]
        sub    ecx, edx
        sbb    eax, eax
        and    ecx, eax
        add    ecx, edx
        mov    edi, dword ptr [ebx + 4]
        mov    eax, ecx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    cl, al
        and    cl, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    ebx, dword ptr [ebx]
        sub    edx, eax
        jne    loc_1_2073
loc_1_2095:
        pop    edx
        test   byte ptr [edx + 0x3c], 8
        jne    loc_1_20B3
        add    edx, 0x58
        mov    ebx, edx
        xchg   dword ptr [0x588c], ebx       ; -> loc_1_588C
        mov    dword ptr [ebx], edx
        mov    dword ptr [edx + 4], ebx
        mov    dword ptr [edx], 0x5888       ; -> loc_1_5888
loc_1_20B2:
        ret    
loc_1_20B3:
        call   sub_1_4619
        ret    
loc_1_20B9:
        test   dword ptr [eax + 0x18], 2
        je     loc_1_20B2
        mov    ebx, dword ptr [ebp + 0x2c]
        call   sub_1_3A69
        jae    loc_1_2045
        inc    dword ptr [0x5926]            ; -> loc_1_5926
        ret    
loc_1_20D7:
        mov    byte ptr [ecx + 9], 0xfd
        mov    edx, dword ptr [ebx + 4]
        jmp    loc_1_2063
loc_1_20E0:
        lea    ecx, [ebp + 0x10]
        push   ecx
        push   ebx
        push   edx
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x20]
        mov    eax, dword ptr [edi + 0x48]
        push   dword ptr [eax + 0xc]
        lea    eax, [ebp + 0xc]
        push   eax
        VxDCall  VPOWERD.Service_002Eh           ; cd202e002800
        cmp    dword ptr [ebp + 0xc], 0
        je     loc_1_2095
        int3   
        cmp    dword ptr [ebp + 0xc], 0x103
        je     loc_1_20B2
        pop    edx
        call   sub_1_2236
        ret    
loc_1_2113:
        call   sub_1_211D
        jmp    loc_1_2008
sub_1_211D:
        cmp    word ptr [eax + 0x1e], 0x200
        jne    loc_1_217A
        cmp    dword ptr [eax + 6], 0
        je     loc_1_217A
        mov    ecx, dword ptr [eax + 6]
        cmp    dword ptr [edi + 0x18], ecx
        jne    loc_1_213E
        cmp    byte ptr [edi + 0x23], 0xa
        jb     loc_1_213A
        ret    
loc_1_213A:
        inc    byte ptr [edi + 0x23]
        ret    
loc_1_213E:
        dec    byte ptr [edi + 0x23]
        js     loc_1_2144
        ret    
loc_1_2144:
        mov    dword ptr [edi + 0x18], ecx
        mov    byte ptr [edi + 0x23], 1
        test   dword ptr [0x58f8], 0x800     ; -> loc_1_58F8
        jne    loc_1_217A
        push   edx
        push   eax
        test   dword ptr [0x58f8], 0x10      ; -> loc_1_58F8
        je     loc_1_2171
        push   edi
        call   sub_1_6CBB
        call   sub_1_7221
        pop    edi
loc_1_2171:
        push   edi
        call   sub_1_2FF9
        pop    edi
        pop    eax
        pop    edx
loc_1_217A:
        ret    
loc_1_217B:
        cmp    dword ptr [0x5890], 0         ; -> loc_1_5890
        je     loc_1_2189
        call   sub_1_3D82
loc_1_2189:
        call   sub_1_2196
        call   sub_1_3D82
        ret    4
sub_1_2196:
        mov    eax, 0x5888                   ; -> loc_1_5888
        cmp    dword ptr [eax], eax
        je     loc_1_21B8
        push   ebx
        mov    ebx, eax
loc_1_21A2:
        mov    edx, dword ptr [ebx]
        mov    eax, dword ptr [edx]
        mov    dword ptr [eax + 4], ebx
        mov    dword ptr [ebx], eax
        sub    edx, 0x58
        call   sub_1_4619
        cmp    dword ptr [ebx], ebx
        jne    loc_1_21A2
        pop    ebx
loc_1_21B8:
        ret    
sub_1_21B9:
        mov    eax, dword ptr [esp + 4]
        mov    edx, dword ptr [esp + 8]
        test   byte ptr [edx + 0x3c], 0xd0
        jne    loc_1_21DA
        and    byte ptr [edx + 0x3c], 0xfb
        cmp    dword ptr [esp + 0xc], 0
        jne    loc_1_2210
loc_1_21D2:
        call   sub_1_4619
        ret    0xc
loc_1_21DA:
        test   byte ptr [edx + 0x3c], 0x40
        jne    loc_1_21F2
loc_1_21E0:
        and    byte ptr [edx + 0x3c], 0xfb
        test   byte ptr [edx + 0x3c], 0x10
        je     loc_1_21EF
        call   sub_1_27D4
loc_1_21EF:
        ret    0xc
loc_1_21F2:
        and    byte ptr [edx + 0x3c], 0xbf
        push   0
        mov    ecx, esp
        push   edx
        push   dword ptr [eax + 0xc]
        push   ecx
        VxDCall  VPOWERD.Service_002Dh           ; cd202d002800
        pop    eax
        cmp    eax, 0x103
        jne    loc_1_21E0
        ret    0xc
loc_1_2210:
        mov    ecx, dword ptr [edx + 0x44]
        jecxz  loc_1_21D2
        mov    byte ptr [ecx + 9], 0xff
        jmp    loc_1_21D2
loc_1_221B:
        mov    edx, dword ptr [esp + 8]
        cmp    dword ptr [esp + 0xc], 0
        jne    loc_1_222E
        call   sub_1_4619
        ret    0x10
loc_1_222E:
        call   sub_1_2236
        ret    0x10
sub_1_2236:
        mov    edx, dword ptr [edx + 0x44]
        test   dword ptr [edx + 0xc], 0x4000
        jne    loc_1_227D
        push   dword ptr [edx + 0xa]
        call   sub_1_42A4
        pop    ecx
        jb     loc_1_227D
        test   dword ptr [eax + 0x18], 8
        jne    loc_1_2272
        mov    ecx, edx
        test   dword ptr [edx + 0xc], 0x80
        jne    loc_1_2268
        xchg   dword ptr [eax + 0x20], ecx
        mov    dword ptr [edx + 0x10], ecx
        ret    
loc_1_2268:
        xchg   dword ptr [eax + 0x28], ecx
        mov    dword ptr [edx + 0x10], ecx
        inc    dword ptr [eax + 0x24]
        ret    
loc_1_2272:
        mov    byte ptr [edx + 9], 0xff
        mov    edx, dword ptr [edx]
        jmp    sub_1_4619
loc_1_227D:
        mov    word ptr [edx + 8], 0xfc00
        push   esi
        push   ebx
        mov    ebx, dword ptr [edx]
        jmp    loc_1_4677
        db  0C2h, 010h, 000h
loc_1_228F:
        ret    4
sub_1_2292:
        mov    ebx, edi
        call   sub_1_232F
        jb     loc_1_22A7
        mov    edi, ebx
        ret    
loc_1_229E:
        mov    dword ptr [edi], 0
        mov    edi, ebx
        ret    
loc_1_22A7:
        mov    ecx, dword ptr [ebx + 0x38]
        cmp    dword ptr [0x5878], ecx       ; -> loc_1_5878
        je     loc_1_230E
        inc    dword ptr [ebx + 0x38]
loc_1_22B5:
        mov    edi, dword ptr [ebx + 0x3c]
        mov    eax, dword ptr [esi + 2]
        mov    dword ptr [edi + ecx*4], eax
        mov    edi, dword ptr [ebx + 0x40]
        mov    ax, word ptr [esi]
        mov    dl, al
        and    al, 0x7f
        mov    word ptr [edi + ecx*2], ax
        mov    edi, ecx
        lea    edi, [ecx + ecx*2]
        shl    ecx, 4
        lea    edi, [ecx + edi*2]
        add    edi, dword ptr [ebx + 0x44]
        inc    dword ptr [0x587c]            ; -> loc_1_587C
        mov    eax, dword ptr [0x587c]       ; -> loc_1_587C
        stosd  dword ptr es:[edi], eax
        test   dl, 0x80
        je     loc_1_229E
        mov    ax, word ptr [esi + 6]
        movzx  ecx, al
        and    cl, 0x1f
        cmp    cl, 2
        je     loc_1_229E
        and    al, 0x3f
        xor    ah, 0x80
        stosw  word ptr es:[edi], ax
        push   esi
        add    esi, 8
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    esi
        mov    edi, ebx
        ret    
loc_1_230E:
        mov    edi, dword ptr [ebx + 0x44]
        xor    eax, eax
        lea    edx, [eax - 1]
loc_1_2316:
        cmp    edx, dword ptr [edi]
        jb     loc_1_231E
        mov    edx, dword ptr [edi]
        mov    eax, ecx
loc_1_231E:
        add    edi, 0x16
        loop   loc_1_2316
        neg    eax
        add    eax, dword ptr [0x5878]       ; -> loc_1_5878
        mov    ecx, eax
        jmp    loc_1_22B5
sub_1_232F:
        mov    edi, dword ptr [ebx + 0x3c]
        mov    ecx, dword ptr [ebx + 0x38]
loc_1_2335:
        mov    eax, dword ptr [esi + 2]
        repne scasd eax, dword ptr es:[edi]
        jne    loc_1_2363
        push   ecx
        mov    edx, dword ptr [ebx + 0x38]
        sub    edx, ecx
        mov    eax, dword ptr [ebx + 0x40]
        mov    cx, word ptr [esi]
        and    cl, 0x7f
        cmp    word ptr [eax + edx*2 - 2], cx
        pop    ecx
        jne    loc_1_2335
        dec    edx
        mov    ecx, edx
        lea    edi, [edx + edx*2]
        shl    edx, 4
        lea    edi, [edx + edi*2]
        add    edi, dword ptr [ebx + 0x44]
        ret    
loc_1_2363:
        stc    
        ret    
sub_1_2365:
        call   sub_1_232F
        jae    loc_1_236D
        ret    
loc_1_236D:
        push   esi
        cli    
        dec    dword ptr [ebx + 0x38]
        mov    eax, dword ptr [ebx + 0x38]
        cmp    ecx, eax
        je     loc_1_23A6
        mov    esi, eax
        lea    edx, [esi + esi*2]
        shl    esi, 4
        lea    esi, [esi + edx*2]
        add    esi, dword ptr [ebx + 0x44]
        mov    edx, ecx
        mov    ecx, 5
        rep movsd dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        mov    edi, dword ptr [ebx + 0x3c]
        mov    ecx, dword ptr [edi + eax*4]
        mov    dword ptr [edi + edx*4], ecx
        mov    edi, dword ptr [ebx + 0x40]
        mov    cx, word ptr [edi + eax*2]
        mov    word ptr [edi + edx*2], cx
loc_1_23A6:
        sti    
        pop    esi
        ret    
        db  000h, 000h, 000h, 000h
sub_1_23AD:
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        VMMCall  VMM.Enable_VM_Ints              ; cd201a000100
loc_1_23CD:
        VMMCall  VMM.Release_Time_Slice          ; cd2038000100
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        test   dword ptr [esi + 4], 0x20000000
        je     loc_1_23CD
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        and    dword ptr [esi + 4], 0xdfffffff
        pop    esi
        xor    eax, eax
        xchg   dword ptr [0x23a9], eax       ; -> obj1:0x23A9
        ret    
        db  08Bh, 074h, 024h, 004h, 083h, 0ECh, 06Ch, 057h, 08Dh, 07Ch, 024h, 004h, 0CDh, 020h, 08Dh, 000h
        db  001h, 000h, 05Fh, 0CDh, 020h, 083h, 000h, 001h, 000h, 0CDh, 020h, 01Ah, 000h, 001h, 000h, 033h
        db  0C0h, 0CDh, 020h, 061h, 001h, 001h, 000h, 0CDh, 020h, 085h, 000h, 001h, 000h, 080h, 03Eh, 000h
        db  075h, 0EDh, 0CDh, 020h, 086h, 000h, 001h, 000h, 056h, 08Dh, 074h, 024h, 004h, 0CDh, 020h, 08Eh
        db  000h, 001h, 000h, 05Eh, 083h, 0C4h, 06Ch, 0C3h
loc_1_2450:
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [0x23a9], eax       ; -> obj1:0x23A9
        mov    eax, dword ptr [esp + 4]
        or     dword ptr [eax + 4], 0x20000000
        ret    0xc
        db  000h
sub_1_2468:
        VxDCall  VPOWERD._VPOWERD_Register_Power_Handler ; cd200a802800
sub_1_246E:
        VxDCall  VPOWERD._VPOWERD_Deregister_Power_Handler ; cd200b802800
sub_1_2474:
        VxDCall  VPOWERD.Service_8016h           ; cd2016802800
sub_1_247A:
        VxDCall  VPOWERD.Service_8017h           ; cd2017802800
sub_1_2480:
        VxDCall  VPOWERD.Service_8018h           ; cd2018802800
sub_1_2486:
        VxDCall  VPOWERD.Service_801Ch           ; cd201c802800
sub_1_248C:
        VxDCall  VPOWERD.Service_801Dh           ; cd201d802800
sub_1_2492:
        VxDCall  VPOWERD.Service_801Eh           ; cd201e802800
sub_1_2498:
        VxDCall  VPOWERD.Service_8023h           ; cd2023802800
sub_1_249E:
        VxDCall  VPOWERD._VPOWERD_Restore_Power_On_Defaults ; cd2006802800
sub_1_24A4:
        VxDCall  VPOWERD._VPOWERD_Get_Power_Status ; cd2007802800
loc_1_24AA:
        VxDCall  VPOWERD.Service_8029h           ; cd2029802800
sub_1_24B0:
        VxDCall  VPOWERD.Service_802Ah           ; cd202a802800
sub_1_24B6:
        VxDCall  VPOWERD.Service_802Bh           ; cd202b802800
sub_1_24BC:
        VxDCall  VPOWERD.Service_802Ch           ; cd202c802800
sub_1_24C2:
        VxDCall  VPOWERD.Service_8030h           ; cd2030802800
sub_1_24C8:
        VxDCall  VPOWERD.Service_8051h           ; cd2051802800
sub_1_24CE:
        VxDCall  VPOWERD.Service_8052h           ; cd2052802800
sub_1_24D4:
        VxDCall  VPOWERD.Service_8053h           ; cd2053802800
        add    byte ptr [eax], al
loc_1_24DC:
        xor    esi, esi
        ret    
loc_1_24DF:
        cmp    ecx, 0x28
        jl     loc_1_24DC
        cmp    word ptr [edi + 0x1e], 0x200
        jne    loc_1_24DC
        mov    ecx, dword ptr [edi + 0x20]
        mov    esi, ebx
        mov    edx, edi
        lea    ebx, [0x565c]                 ; -> obj1:0x565C
loc_1_24F9:
        mov    edi, ebx
        mov    ebx, dword ptr [edi + 0x24]
        or     ebx, ebx
        je     loc_1_25AC
        cmp    dword ptr [ebx + 4], esi
        jne    loc_1_24F9
        mov    eax, dword ptr [ebx + 0xc]
        cmp    dword ptr [eax], ecx
        jne    loc_1_24F9
        push   edi
        push   esi
        mov    edi, dword ptr [ebx + 0x10]
        lea    esi, [edx + 0x16]
        cmp    dword ptr [edx + 6], ecx
        jne    loc_1_2525
        mov    esi, dword ptr [ebx + 0xc]
        add    esi, 4
loc_1_2525:
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        pop    esi
        pop    edi
        movzx  eax, word ptr [edx + 0x26]
        mov    dword ptr [ebx + 0x14], eax
        or     byte ptr [ebx], 0x80
        mov    byte ptr [ebx + 1], 0
        mov    eax, dword ptr [ebx + 0x24]
        mov    dword ptr [edi + 0x24], eax
        push   esi
        xor    esi, esi
        xchg   dword ptr [ebx + 0x1c], esi
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
        pop    esi
        test   byte ptr [ebx], 2
        jne    loc_1_2559
        test   byte ptr [ebx], 4
        jne    loc_1_256E
        mov    ebx, edi
        jmp    loc_1_24F9
loc_1_2559:
        push   edx
        push   esi
        mov    esi, 0x2659                   ; -> sub_1_2659
        mov    edx, ebx
        VMMCall  VMM.Schedule_Global_Event       ; cd200e000100
        pop    esi
        pop    edx
        mov    ebx, edi
        jmp    loc_1_24F9
loc_1_256E:
        push   esi
        mov    esi, dword ptr [ebx + 8]
        mov    eax, dword ptr [ebx + 0x14]
        xchg   al, ah
        mov    word ptr [esi + 0xa], ax
        push   edx
        mov    edx, dword ptr [ebx + 0x10]
        mov    eax, dword ptr [edx]
        mov    dword ptr [esi + 0x1c], eax
        mov    ax, word ptr [edx + 4]
        mov    word ptr [esi + 0x20], ax
        pop    edx
        mov    byte ptr [esi + 9], 0
        push   esi
        call   sub_1_26BF
        pop    esi
        push   0
        push   0x8e
        push   esi
        VxDCall  VPOWERD._VPOWERD_Deregister_Power_Handler ; cd200b002800
        pop    esi
        jmp    loc_1_24F9
loc_1_25AC:
        xor    esi, esi
        ret    
loc_1_25AF:
        cmp    dword ptr [edx + 0x1c], 0
        je     loc_1_25E5
        dec    dword ptr [edx + 0x20]
        je     loc_1_260A
        mov    ebx, edx
        mov    esi, dword ptr [ebx + 0x18]
        cmp    byte ptr [esi + 8], 0xff
        je     loc_1_25FC
        mov    eax, 0x3e8
        mov    esi, 0x25af                   ; -> loc_1_25AF
        mov    edx, ebx
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    dword ptr [ebx + 0x1c], esi
        mov    esi, dword ptr [ebx + 0x18]
        mov    edi, dword ptr [ebx + 4]
        call   sub_1_410D
loc_1_25E5:
        ret    
loc_1_25E6:
        mov    eax, 0x3e8
        mov    esi, 0x25af                   ; -> loc_1_25AF
        mov    edx, ebx
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    dword ptr [ebx + 0x1c], esi
        ret    
loc_1_25FC:
        dec    dword ptr [ebx + 0x14]
        jne    loc_1_25E6
        push   esi
        call   sub_1_0562
        pop    esi
        mov    edx, ebx
loc_1_260A:
        mov    dword ptr [edx + 0x1c], 0
        or     byte ptr [edx], 0x80
        mov    byte ptr [edx + 1], 0xfa
        mov    esi, edx
        push   0x24
        push   edx
        push   0x5680                        ; -> loc_1_5680
        call   sub_1_46EA
        add    esp, 0xc
        test   byte ptr [esi], 2
        je     loc_1_263B
        test   byte ptr [esi], 4
        jne    loc_1_263C
        mov    edx, esi
        call   sub_1_2659
loc_1_263B:
        ret    
loc_1_263C:
        mov    esi, dword ptr [esi + 8]
        mov    byte ptr [esi + 9], 0xfa
        push   esi
        call   sub_1_26BF
        pop    esi
        push   0
        push   0x8e
        push   esi
        VxDCall  VPOWERD._VPOWERD_Deregister_Power_Handler ; cd200b002800
        ret    
sub_1_2659:
        mov    esi, dword ptr [edx + 8]
        cmp    byte ptr [edx + 1], 0
        jne    loc_1_269E
        or     dword ptr [esi + 4], 0x200000
        mov    eax, dword ptr [edx + 0x14]
        xchg   ah, al
        mov    dword ptr [esi + 0x2c], eax
        push   0x3c8b                        ; -> loc_1_3C8B
loc_1_2676:
        cmp    byte ptr [edx + 0x44], 0
        jne    loc_1_268D
        push   0
        push   0x8e
        push   edx
        VxDCall  VPOWERD._VPOWERD_Deregister_Power_Handler ; cd200b002800
        mov    edx, esi
        ret    
loc_1_268D:
        mov    dword ptr [edx + 0x40], 1
        mov    dword ptr [edx + 0x54], 0x26ad ; -> loc_1_26AD
        mov    edx, esi
        ret    
loc_1_269E:
        mov    eax, dword ptr [esi + 0xc]
        or     dword ptr [esi + 4], 0x20000
        inc    byte ptr [eax + 0x17]
        jmp    loc_1_2676
loc_1_26AD:
        sub    eax, 0x3c
        push   0
        push   0x8e
        push   eax
        VxDCall  VPOWERD._VPOWERD_Deregister_Power_Handler ; cd200b002800
        ret    
sub_1_26BF:
        mov    esi, dword ptr [esp]
        mov    byte ptr [esi + 8], 0
        mov    eax, dword ptr [esi + 4]
        or     eax, eax
        je     loc_1_26CF
        call   dword ptr [eax]
loc_1_26CF:
        ret    
loc_1_26D0:
        pop    ecx
        ret    
loc_1_26D2:
        inc    dword ptr [0x56ac]            ; -> loc_1_56AC
        ret    
loc_1_26D9:
        inc    dword ptr [0x56a8]            ; -> loc_1_56A8
        ret    
loc_1_26E0:
        mov    ecx, dword ptr [eax + 6]
        or     ecx, ecx
        je     sub_1_1FFC
        push   esi
        mov    esi, dword ptr [ebp + 0xc]
        mov    ebx, dword ptr [edi + 0x1c]
        cmp    dword ptr [esi], ebx
        jne    loc_1_26D0
        mov    bx, word ptr [edi + 0x20]
        cmp    word ptr [esi + 4], bx
        jne    loc_1_26D0
        mov    ebx, edi
        mov    edi, eax
        mov    eax, ecx
        push   edx
        call   sub_1_2D10
        pop    edx
        jb     loc_1_26D0
        cmp    word ptr [esi + 0x14], 0
        jne    loc_1_2735
        mov    eax, dword ptr [esi + 0x20]
        mov    ecx, dword ptr [eax + 0x1c]
        cmp    dword ptr [edi + 0xa], ecx
        jne    loc_1_2735
        mov    cx, word ptr [eax + 0x20]
        cmp    word ptr [edi + 0xe], cx
        jne    loc_1_2735
        mov    eax, edi
        mov    edi, ebx
        pop    esi
        jmp    sub_1_1FFC
loc_1_2735:
        pop    ecx
        cmp    dword ptr [0x7cac], edx       ; -> loc_1_7CAC
        jb     loc_1_26D2
        call   sub_1_291A
        jb     loc_1_26D9
        inc    dword ptr [0x56a4]            ; -> loc_1_56A4
        mov    edi, dword ptr [eax + 0x48]
        inc    byte ptr [edi + 4]
        cmp    dword ptr [edi + 0x12], 0
        jne    loc_1_275D
        mov    ecx, dword ptr [ebx + 0x18]
        mov    dword ptr [edi + 0x12], ecx
loc_1_275D:
        mov    ebx, dword ptr [esi + 0x20]
        push   ebx
        push   edx
        lea    edx, [eax + 0x38]
        push   edx
        lea    edx, [esi + 0xc]
        mov    ecx, dword ptr [edi + 6]
        cmp    dword ptr [ebx + 0x18], ecx
        jne    loc_1_2774
        lea    edx, [edi + 0xa]
loc_1_2774:
        mov    esi, eax
        push   edx
        call   dword ptr [ebx + 0x28]
        add    esp, 0x10
loc_1_277D:
        mov    ecx, dword ptr [esi + 0x44]
        push   ecx
        mov    eax, dword ptr [esi + 0x54]
        mov    dword ptr [ecx + 0xc], eax
        lea    eax, [esi + 0x58]
        mov    dword ptr [ecx + 4], eax
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [ecx + 4], edi
        push   esi
        VxDCall  VPOWERD.Service_0023h           ; cd2023002800
        push   esi
        or     byte ptr [esi + 0x3c], 4
        mov    dword ptr [esi + 0x40], ebx
        mov    ebx, dword ptr [ebx + 0x48]
        push   dword ptr [ebx + 0xc]
        lea    eax, [ebp + 0xc]
        push   eax
        or     dword ptr [0x58f8], 0x20      ; -> loc_1_58F8
        VxDCall  VPOWERD.Service_002Dh           ; cd202d002800
        and    dword ptr [0x58f8], 0xffffffdf ; -> loc_1_58F8
        cmp    dword ptr [ebp + 0xc], 0x103
        jne    loc_1_27C9
        ret    
loc_1_27C9:
        push   dword ptr [ebp + 0xc]
        push   esi
        push   ebx
        call   sub_1_21B9
        ret    
sub_1_27D4:
        push   edx
        push   edx
        VxDCall  VPOWERD.Service_0019h           ; cd2019002800
        pop    edx
        test   byte ptr [edx + 0x3c], 0x20
        jne    loc_1_27F5
        mov    eax, edx
        xchg   dword ptr [0x7d1c], eax       ; -> loc_1_7D1C
        mov    dword ptr [edx + 0x38], eax
        inc    dword ptr [0x7cb4]            ; -> obj1:0x7CB4
        ret    
loc_1_27F5:
        push   ebx
        push   esi
        push   edi
        mov    ebx, edx
        call   sub_1_2884
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_2803:
        jmp    sub_1_1FFC
loc_1_2808:
        ret    
loc_1_2809:
        test   dword ptr [edi + 8], 8
        jne    loc_1_281E
        test   dword ptr [0x58f8], 0x400     ; -> loc_1_58F8
        je     loc_1_2803
loc_1_281E:
        mov    ecx, 0xffffffff
        cmp    dword ptr [eax + 0xa], ecx
        jne    loc_1_2803
        cmp    word ptr [eax + 0xe], cx
        jne    loc_1_2803
        movzx  ecx, byte ptr [eax + 4]
        cmp    cl, 8
        jae    loc_1_2803
        mov    ebx, edi
        mov    edi, eax
        jecxz  loc_1_2849
        push   eax
        lea    edi, [eax + 0x1e]
        mov    eax, dword ptr [ebx + 0x18]
        repne scasd eax, dword ptr es:[edi]
        pop    edi
        je     loc_1_2808
loc_1_2849:
        call   sub_1_291A
        jb     loc_1_2808
        push   eax
        mov    eax, dword ptr [eax + 0x48]
        mov    dword ptr [ebp + 0x2c], eax
        mov    dword ptr [ebp + 0x30], edx
        mov    edi, ebx
        push   ebx
        call   sub_1_1FFC
        pop    eax
        pop    ebx
        mov    esi, dword ptr [ebx + 0x48]
        mov    edx, dword ptr [eax + 0x18]
        cmp    dword ptr [esi + 0x12], 0
        jne    loc_1_2873
        mov    dword ptr [esi + 0x12], edx
loc_1_2873:
        movzx  ecx, byte ptr [esi + 4]
        inc    byte ptr [esi + 4]
        mov    dword ptr [esi + ecx*4 + 0x1e], edx
        mov    word ptr [ebx + 0x3e], 0xffff
sub_1_2884:
        mov    esi, dword ptr [ebx + 0x48]
        movsx  edx, word ptr [ebx + 0x3e]
        or     byte ptr [ebx + 0x3c], 0x20
        push   ebx
loc_1_2890:
        inc    edx
        cmp    dword ptr [0x58c8], edx       ; -> loc_1_58C8
        je     loc_1_28E0
        mov    ebx, dword ptr [edx*4 + 0x58dc] ; -> loc_1_58DC
        mov    eax, dword ptr [ebx + 0x18]
        or     eax, eax
        je     loc_1_2890
        movzx  ecx, byte ptr [esi + 4]
        lea    edi, [esi + 0x1e]
        repne scasd eax, dword ptr es:[edi]
        je     loc_1_2890
        test   dword ptr [ebx + 8], 8
        jne    loc_1_2890
        pop    edi
        mov    dword ptr [esi + 6], eax
        mov    word ptr [edi + 0x3e], dx
        movzx  ecx, word ptr [esi + 2]
        xchg   ch, cl
        push   ebx
        push   ecx
        lea    edx, [edi + 0x38]
        push   edx
        lea    edx, [esi + 0xa]
        push   edx
        call   dword ptr [ebx + 0x28]
        add    esp, 0x10
        xchg   edi, esi
        jmp    loc_1_277D
loc_1_28E0:
        pop    ebx
        and    byte ptr [ebx + 0x3c], 0xdf
        mov    eax, ebx
        xchg   dword ptr [0x7d1c], eax       ; -> loc_1_7D1C
        mov    dword ptr [ebx + 0x38], eax
        inc    dword ptr [0x7cb4]            ; -> obj1:0x7CB4
        ret    
loc_1_28F7:
        mov    eax, dword ptr [eax - 4]
        inc    dword ptr [0x7cb4]            ; -> obj1:0x7CB4
        mov    ecx, eax
        xchg   dword ptr [0x7d1c], ecx       ; -> loc_1_7D1C
        mov    dword ptr [eax + 0x38], ecx
        inc    dword ptr [0x569c]            ; -> loc_1_569C
        ret    
loc_1_2912:
        inc    dword ptr [0x7cb4]            ; -> obj1:0x7CB4
        stc    
        ret    
sub_1_291A:
        dec    dword ptr [0x7cb4]            ; -> obj1:0x7CB4
        js     loc_1_2912
        push   esi
        push   ebx
        mov    ebx, dword ptr [0x7d1c]       ; -> loc_1_7D1C
        mov    ecx, dword ptr [ebx + 0x38]
        mov    dword ptr [0x7d1c], ecx       ; -> loc_1_7D1C
        mov    esi, dword ptr [ebx + 0x48]
        cmp    dword ptr [ebp + 0x30], edx
        jb     loc_1_2959
        xchg   esi, edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    cl, dl
        and    cl, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        clc    
        mov    eax, dword ptr [ebx + 0x44]
        mov    eax, dword ptr [eax]
        mov    dword ptr [eax + 0xc], edx
        mov    eax, ebx
        pop    ebx
        pop    esi
        ret    
loc_1_2959:
        mov    ecx, dword ptr [ebx + 0x44]
        mov    dword ptr [ecx + 4], esi
        mov    dword ptr [ecx + 0xc], edx
        mov    esi, edx
        mov    edi, dword ptr [ecx]
        mov    dword ptr [ecx], 0
        push   ecx
        push   ebx
        VxDCall  VPOWERD.Service_0023h           ; cd2023002800
        lea    ecx, [ebx + 0x54]
        mov    eax, dword ptr [esp]
        push   ecx
        push   ebx
        push   esi
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x20]
        mov    eax, dword ptr [eax + 0x48]
        push   dword ptr [eax + 0xc]
        lea    eax, [ebp + 0xc]
        push   eax
        VxDCall  VPOWERD.Service_002Eh           ; cd202e002800
        mov    ecx, dword ptr [ebx + 0x44]
        mov    dword ptr [ecx], edi
        push   ebx
        VxDCall  VPOWERD.Service_0019h           ; cd2019002800
        cmp    dword ptr [ebp + 0xc], 0
        jne    loc_1_29B1
        mov    edx, dword ptr [ebx + 0x54]
        mov    dword ptr [edi + 0xc], edx
        clc    
        mov    eax, ebx
loc_1_29AE:
        pop    ebx
        pop    esi
        ret    
loc_1_29B1:
        cmp    dword ptr [ebp + 0xc], 0x103
        je     loc_1_29CE
        mov    eax, ebx
        xchg   dword ptr [0x7d1c], eax       ; -> loc_1_7D1C
        mov    dword ptr [ebx + 0x38], eax
        inc    dword ptr [0x7cb4]            ; -> obj1:0x7CB4
        stc    
        jmp    loc_1_29AE
loc_1_29CE:
        jmp    loc_1_29CE
loc_1_29D0:
        test   dword ptr [0x58f8], 0x40      ; -> loc_1_58F8
        je     loc_1_2AEA
        mov    eax, dword ptr [ebx + 0x18]
        mov    ecx, dword ptr [edi + 0x12]
        jecxz  loc_1_29ED
        cmp    dword ptr [ebx + 0x18], ecx
        jne    loc_1_2A11
loc_1_29ED:
        mov    eax, dword ptr [edi + 0x20]
        cmp    eax, -1
        je     loc_1_2A70
        call   sub_1_2D10
        jb     loc_1_2A11
        cmp    word ptr [esi + 0x14], 0xe00
        je     loc_1_2A11
        mov    ecx, dword ptr [ebx + 4]
        test   dword ptr [esi + 4], ecx
        je     loc_1_2A25
        cmp    dword ptr [ebx + 0x18], eax
        je     loc_1_2A25
loc_1_2A11:
        xor    esi, esi
        ret    
loc_1_2A14:
        call   sub_1_2D4D
        xor    esi, esi
        ret    
loc_1_2A1C:
        inc    dword ptr [0x56a0]            ; -> loc_1_56A0
        xor    esi, esi
        ret    
loc_1_2A25:
        mov    ecx, dword ptr [0x5684]       ; -> loc_1_5684
        cmp    dword ptr [esi + 0x18], ecx
        jb     loc_1_2A14
        call   sub_1_2A8C
        jb     loc_1_2A1C
        mov    dword ptr [ecx + 0x24], edx
        mov    word ptr [ecx + 0x28], 0x28
        mov    byte ptr [edx + 5], 1
        mov    word ptr [edx + 0x1e], 0x200
        mov    dword ptr [edx + 0x20], eax
        mov    eax, dword ptr [esi + 0x14]
        mov    si, word ptr [ebx + 0x24]
        shl    esi, 0x18
        add    eax, esi
        inc    ah
        mov    dword ptr [edx + 0x24], eax
        mov    esi, ecx
        mov    edi, ebx
        inc    dword ptr [0x5698]            ; -> loc_1_5698
        call   sub_1_410D
        xor    esi, esi
        ret    
loc_1_2A70:
        lea    eax, [edi + 0x16]
        push   0
        push   0
        push   eax
        push   ebx
        call   sub_1_6AB9
        add    esp, 0x10
        xor    esi, esi
        ret    
loc_1_2A84:
        inc    dword ptr [0x7cb4]            ; -> obj1:0x7CB4
        stc    
        ret    
sub_1_2A8C:
        dec    dword ptr [0x7cb4]            ; -> obj1:0x7CB4
        js     loc_1_2A84
        mov    edx, dword ptr [0x7d1c]       ; -> loc_1_7D1C
        mov    ecx, dword ptr [edx + 0x38]
        mov    dword ptr [0x7d1c], ecx       ; -> loc_1_7D1C
        mov    ecx, dword ptr [edx + 0x48]
        mov    dword ptr [ecx], edx
        add    ecx, 4
        mov    dword ptr [ecx + 0x18], 0x28f7 ; -> loc_1_28F7
        mov    dword ptr [ecx + 4], 1
        mov    word ptr [ecx + 0xa], 0x5304
        mov    dword ptr [ecx + 0xc], 0
        mov    edx, dword ptr [edi + 0x16]
        mov    dword ptr [ecx + 0x1c], edx
        mov    dword ptr [ecx + 0x34], edx
        mov    edx, dword ptr [edi + 0x1a]
        mov    word ptr [ecx + 0x20], dx
        mov    dword ptr [ecx + 0x38], edx
        mov    edx, dword ptr [ebx + 0x18]
        mov    dword ptr [ecx + 0x30], edx
        mov    word ptr [ecx + 0x22], 1
        lea    edx, [ecx + 0x2a]
        ret    
loc_1_2AE9:
        pop    eax
loc_1_2AEA:
        xor    esi, esi
        ret    
loc_1_2AED:
        mov    eax, dword ptr [ebx + 0x1c]
        cmp    dword ptr [esi], eax
        jne    loc_1_2AFE
        mov    ax, word ptr [ebx + 0x20]
        cmp    word ptr [esi + 4], ax
        je     loc_1_2AEA
loc_1_2AFE:
        cmp    word ptr [edi + 0x1e], 0x100
        je     loc_1_29D0
        mov    dword ptr [0x5694], 6         ; -> loc_1_5694
        mov    eax, dword ptr [ebx + 0x18]
        cmp    dword ptr [edi + 0x12], eax
        jne    loc_1_2AEA
        push   0
        movzx  edx, word ptr [edi + 2]
        xchg   dh, dl
        sub    ecx, edx
        sbb    esi, esi
        and    ecx, esi
        add    ecx, edx
        cmp    ecx, edx
        je     loc_1_2B3F
        call   sub_1_291A
        jb     loc_1_2AE9
        mov    ecx, edx
        mov    edi, dword ptr [eax + 0x48]
        mov    dword ptr [esp], eax
loc_1_2B3F:
        sub    ecx, 0x20
        lea    edx, [edi + 0x16]
        add    edi, 0x20
        shr    ecx, 3
        jecxz  loc_1_2B59
        push   edx
loc_1_2B4E:
        call   sub_1_2BD2
        add    edi, 8
        loop   loc_1_2B4E
        pop    ecx
loc_1_2B59:
        pop    ecx
        jecxz  loc_1_2B6D
        mov    eax, ecx
        xchg   dword ptr [0x7d1c], eax       ; -> loc_1_7D1C
        mov    dword ptr [ecx + 0x38], eax
        inc    dword ptr [0x7cb4]            ; -> obj1:0x7CB4
loc_1_2B6D:
        xor    esi, esi
        ret    
loc_1_2B70:
        call   sub_1_6640
        mov    dword ptr [0x56bc], 0         ; -> loc_1_56BC
        ret    
sub_1_2B80:
        push   esi
        mov    esi, dword ptr [0x56bc]       ; -> loc_1_56BC
        VMMCall  VMM.Cancel_Restricted_Event     ; cd205b010100
        mov    dword ptr [0x56bc], 0         ; -> loc_1_56BC
        pop    esi
sub_1_2B98:
        push   ebx
        VxDCall  PAGESWAP.PageSwap_Get_Version   ; cd2000000700
        cmp    bl, 2
        jne    loc_1_2BCB
        cmp    dword ptr [0x56bc], 0         ; -> loc_1_56BC
        jne    loc_1_2BD0
        push   esi
        mov    esi, 0x2b70                   ; -> loc_1_2B70
        xor    eax, eax
        xor    ebx, ebx
        mov    ecx, 2
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
        mov    dword ptr [0x56bc], esi       ; -> loc_1_56BC
        pop    esi
        jmp    loc_1_2BD0
loc_1_2BCB:
        call   sub_1_6640
loc_1_2BD0:
        pop    ebx
        ret    
sub_1_2BD2:
        push   ecx
        mov    eax, dword ptr [edi]
        call   sub_1_2D10
        jb     loc_1_2C92
        cmp    dword ptr [esi + 0x20], ebx
        jne    loc_1_2C24
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [ecx + 2]
        cmp    dword ptr [esi + 0xe], eax
        jne    loc_1_2C24
        mov    ax, word ptr [ecx]
        cmp    word ptr [esi + 0xc], ax
        jne    loc_1_2C24
        mov    eax, dword ptr [edi + 4]
        cmp    ah, 0x10
        jae    loc_1_2C1C
        mov    dword ptr [esi + 0x14], eax
        mov    eax, dword ptr [ebx + 4]
        or     dword ptr [esi + 8], eax
        mov    ecx, dword ptr [0x5684]       ; -> loc_1_5684
        add    ecx, 0x2bf20
        mov    dword ptr [esi + 0x18], ecx
        pop    ecx
        ret    
loc_1_2C1C:
        dec    edx
        call   sub_1_2D4D
        pop    ecx
        ret    
loc_1_2C24:
        mov    ecx, dword ptr [0x5684]       ; -> loc_1_5684
        cmp    dword ptr [esi + 0x18], ecx
        jb     loc_1_2C5E
        mov    ax, word ptr [edi + 6]
        mov    dx, word ptr [esi + 0x16]
        cmp    ah, dh
        jb     loc_1_2C5E
        je     loc_1_2C52
        mov    cx, word ptr [ebx + 0x24]
        add    dh, cl
        cmp    ah, dh
        jae    loc_1_2C50
loc_1_2C47:
        mov    eax, dword ptr [ebx + 4]
        or     dword ptr [esi + 4], eax
        or     dword ptr [esi + 8], eax
loc_1_2C50:
        pop    ecx
        ret    
loc_1_2C52:
        mov    ax, word ptr [edi + 4]
        mov    dx, word ptr [esi + 0x14]
        cmp    ah, dh
        jae    loc_1_2C47
loc_1_2C5E:
        mov    edx, dword ptr [esp + 8]
        mov    eax, dword ptr [edx]
        mov    dword ptr [esi + 0xc], eax
        mov    ax, word ptr [edx + 4]
        mov    word ptr [esi + 0x10], ax
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [esi + 0x14], eax
        mov    dword ptr [esi + 0x20], ebx
        mov    eax, dword ptr [ebx + 4]
        or     dword ptr [esi + 4], eax
        or     dword ptr [esi + 8], eax
        mov    ecx, dword ptr [0x5684]       ; -> loc_1_5684
        add    ecx, 0x2bf20
        mov    dword ptr [esi + 0x18], ecx
loc_1_2C90:
        pop    ecx
        ret    
loc_1_2C92:
        cmp    word ptr [edi + 4], 0x1000
        je     loc_1_2C90
        dec    dword ptr [0x7d08]            ; -> loc_1_7D08
        js     loc_1_2CE4
loc_1_2CA2:
        mov    esi, dword ptr [0x7d20]       ; -> loc_1_7D20
        mov    ecx, dword ptr [esi + 0x24]
        mov    dword ptr [0x7d20], ecx       ; -> loc_1_7D20
        mov    dword ptr [esi], eax
        mov    ecx, dword ptr [0x56b8]       ; -> loc_1_56B8
        mov    edx, dword ptr [0x56b0]       ; -> loc_1_56B0
        mov    dword ptr [edx + ecx*4], eax
        mov    edx, dword ptr [0x56b4]       ; -> loc_1_56B4
        mov    dword ptr [edx + ecx*4], esi
        inc    dword ptr [0x56b8]            ; -> loc_1_56B8
        xor    ecx, ecx
        mov    dword ptr [esi + 4], ecx
        mov    dword ptr [esi + 8], ecx
        mov    dword ptr [esi + 0x24], ecx
        mov    dword ptr [esi + 0x1c], ecx
        jmp    loc_1_2C5E
loc_1_2CE4:
        inc    dword ptr [0x7d08]            ; -> loc_1_7D08
        call   sub_1_2B98
        cmp    dword ptr [0x7d08], 0         ; -> loc_1_7D08
        je     loc_1_2D00
        dec    dword ptr [0x7d08]            ; -> loc_1_7D08
        jmp    loc_1_2CA2
loc_1_2D00:
        pop    ecx
        ret    
sub_1_2D02:
        push   esi
        mov    eax, dword ptr [esp + 8]
        call   sub_1_2D10
        mov    eax, esi
        pop    esi
        ret    
sub_1_2D10:
        cld    
        mov    ecx, dword ptr [0x56b8]       ; -> loc_1_56B8
        jecxz  loc_1_2D37
        push   edi
        mov    edi, dword ptr [0x56b0]       ; -> loc_1_56B0
        mov    edx, ecx
        or     esi, esi
        repne scasd eax, dword ptr es:[edi]
        pop    edi
        jne    loc_1_2D37
        sub    edx, ecx
        mov    esi, dword ptr [0x56b4]       ; -> loc_1_56B4
        mov    esi, dword ptr [esi + edx*4 - 4]
        clc    
        ret    
loc_1_2D37:
        xor    esi, esi
        stc    
        ret    
sub_1_2D3B:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    edx, dword ptr [esp + 0xc]
        call   sub_1_2D4D
        xor    eax, eax
        pop    esi
        ret    
sub_1_2D4D:
        push   edi
        push   edx
        mov    ecx, dword ptr [0x58f4]       ; -> loc_1_58F4
        dec    ecx
        cmp    edx, ecx
        jg     loc_1_2D7A
        mov    dword ptr [0x58f4], ecx       ; -> loc_1_58F4
        mov    edi, dword ptr [0x56b0]       ; -> loc_1_56B0
        mov    eax, dword ptr [edi + ecx*4]
        mov    dword ptr [edi + edx*4], eax
        mov    edi, dword ptr [0x56b4]       ; -> loc_1_56B4
        mov    eax, dword ptr [edi + ecx*4]
        mov    dword ptr [edi + edx*4], eax
        mov    edx, ecx
loc_1_2D7A:
        mov    ecx, dword ptr [0x56b8]       ; -> loc_1_56B8
        dec    ecx
        mov    edi, dword ptr [0x56b0]       ; -> loc_1_56B0
        mov    eax, dword ptr [edi + ecx*4]
        mov    dword ptr [edi + edx*4], eax
        mov    edi, dword ptr [0x56b4]       ; -> loc_1_56B4
        mov    eax, dword ptr [edi + ecx*4]
        mov    dword ptr [edi + edx*4], eax
        dec    dword ptr [0x56b8]            ; -> loc_1_56B8
        pop    edx
        mov    edi, esi
        xor    esi, esi
        xchg   dword ptr [edi + 0x1c], esi
        push   ebx
        push   edx
loc_1_2DA9:
        or     esi, esi
        je     loc_1_2DC0
        mov    dword ptr [esi + 0xc], 0
        mov    ebx, dword ptr [esi + 8]
        call   sub_1_34DE
        mov    esi, ebx
        jmp    loc_1_2DA9
loc_1_2DC0:
        pop    edx
        pop    ebx
        mov    eax, edi
        xchg   dword ptr [0x7d20], eax       ; -> loc_1_7D20
        mov    dword ptr [edi + 0x24], eax
        inc    dword ptr [0x7d08]            ; -> loc_1_7D08
        pop    edi
        ret    
loc_1_2DD5:
        mov    ebx, 0xffffffff
        jmp    loc_1_2E66
loc_1_2DDF:
        mov    dword ptr [0x5690], 0         ; -> loc_1_5690
        add    dword ptr [0x5684], 0x6e      ; -> loc_1_5684
        jb     loc_1_2DD5
        dec    dword ptr [0x5688]            ; -> loc_1_5688
        je     loc_1_2E11
loc_1_2DFA:
        mov    esi, 0x2ddf                   ; -> loc_1_2DDF
        mov    eax, 0x6e
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    dword ptr [0x5690], esi       ; -> loc_1_5690
        ret    
loc_1_2E11:
        mov    dword ptr [0x5688], 0x110     ; -> loc_1_5688
        test   dword ptr [0x58f8], 0x1000    ; -> loc_1_58F8
        je     loc_1_2E2C
        call   sub_1_79E7
loc_1_2E2C:
        test   dword ptr [0x58f8], 0x10      ; -> loc_1_58F8
        je     loc_1_2DFA
        test   dword ptr [0x58f8], 0x40      ; -> loc_1_58F8
        je     loc_1_2E49
        call   sub_1_2EBE
loc_1_2E49:
        cmp    dword ptr [0x5694], 0         ; -> loc_1_5694
        je     loc_1_2E58
        dec    dword ptr [0x5694]            ; -> loc_1_5694
loc_1_2E58:
        dec    dword ptr [0x568c]            ; -> loc_1_568C
        jne    loc_1_2DFA
        mov    ebx, dword ptr [0x5684]       ; -> loc_1_5684
loc_1_2E66:
        mov    esi, dword ptr [0x56b4]       ; -> loc_1_56B4
        mov    ecx, dword ptr [0x56b8]       ; -> loc_1_56B8
        mov    edx, dword ptr [0x58f4]       ; -> loc_1_58F4
        sub    ecx, edx
        jecxz  loc_1_2E95
        lea    esi, [esi + edx*4]
        cld    
loc_1_2E80:
        lodsd  eax, dword ptr [esi]
        mov    edx, dword ptr [eax + 8]
        mov    dword ptr [eax + 4], edx
        mov    dword ptr [eax + 8], 0
        cmp    dword ptr [eax + 0x18], ebx
        jb     loc_1_2EA4
        loop   loc_1_2E80
loc_1_2E95:
        mov    dword ptr [0x568c], 6         ; -> loc_1_568C
        jmp    loc_1_2DFA
loc_1_2EA4:
        mov    edx, dword ptr [0x56b8]       ; -> loc_1_56B8
        sub    edx, ecx
        push   ecx
        push   esi
        mov    esi, eax
        call   sub_1_2D4D
        pop    esi
        pop    ecx
        sub    esi, 4
        loop   loc_1_2E80
        jmp    loc_1_2E95
sub_1_2EBE:
        call   sub_1_6B69
        ret    
sub_1_2EC4:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0x10]
        mov    edx, dword ptr [eax]
        mov    edi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0xc]
        push   0x32
loc_1_2ED7:
        mov    esi, dword ptr [0x56b4]       ; -> loc_1_56B4
        mov    esi, dword ptr [esi + edx*4]
        mov    eax, dword ptr [0x5684]       ; -> loc_1_5684
        cmp    dword ptr [esi + 0x18], eax
        jb     loc_1_2F2A
        mov    eax, dword ptr [ebx + 4]
        test   dword ptr [esi + 4], eax
        jne    loc_1_2F0F
        mov    eax, dword ptr [esi]
        mov    dword ptr [edi], eax
        mov    eax, dword ptr [esi + 0x14]
        inc    ah
        mov    cx, word ptr [ebx + 0x24]
        shl    ecx, 0x18
        add    eax, ecx
        mov    dword ptr [edi + 4], eax
        add    edi, 8
        dec    dword ptr [esp]
        je     loc_1_2F31
loc_1_2F0F:
        inc    edx
loc_1_2F10:
        cmp    dword ptr [0x56b8], edx       ; -> loc_1_56B8
        jne    loc_1_2ED7
        xor    edx, edx
loc_1_2F1A:
        pop    ecx
        mov    ebx, dword ptr [ebp + 0x10]
        mov    dword ptr [ebx], edx
        neg    ecx
        lea    eax, [ecx + 0x32]
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
loc_1_2F2A:
        call   sub_1_2D4D
        jmp    loc_1_2F10
loc_1_2F31:
        inc    edx
        cmp    dword ptr [0x56b8], edx       ; -> loc_1_56B8
        jne    loc_1_2F1A
        xor    edx, edx
        jmp    loc_1_2F1A
loc_1_2F3E:
        movzx  ecx, byte ptr [eax + 9]
        sub    eax, 0x18
        jecxz  loc_1_2F51
        test   byte ptr [eax + 0x15], 1
        je     loc_1_2F51
        and    dword ptr [esi + 8], 0xffffffdf
loc_1_2F51:
        cmp    dword ptr [eax + 8], 0
        jne    loc_1_2F6B
loc_1_2F57:
        mov    edx, dword ptr [eax]
        inc    dword ptr [0x7cb4]            ; -> obj1:0x7CB4
        mov    eax, edx
        xchg   dword ptr [0x7d1c], eax       ; -> loc_1_7D1C
        mov    dword ptr [edx + 0x38], eax
        ret    
loc_1_2F6B:
        mov    edx, eax
        mov    esi, 0x2f7e                   ; -> loc_1_2F7E
        mov    eax, 0x37
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        ret    
loc_1_2F7E:
        mov    edi, edx
        push   edx
        call   sub_1_69D1
        pop    edx
        or     eax, eax
        mov    eax, edi
        jne    loc_1_2F57
        ret    
sub_1_2F8E:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        mov    ebx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebx + 0xc]
        mov    eax, dword ptr [ecx]
        call   sub_1_2D10
        jb     loc_1_2FEC
        mov    edi, dword ptr [ebx + 0x10]
        cmp    word ptr [esi + 0x14], 0
        je     loc_1_2FCE
        mov    eax, dword ptr [esi + 0xc]
        mov    dword ptr [edi], eax
        mov    ax, word ptr [esi + 0x10]
        mov    word ptr [edi + 4], ax
        movzx  eax, word ptr [esi + 0x16]
        mov    dword ptr [ebx + 0x14], eax
        mov    ecx, dword ptr [esi + 0x20]
loc_1_2FC3:
        mov    dword ptr [ebx + 0x28], ecx
        mov    byte ptr [ebx + 1], 0
loc_1_2FCA:
        pop    esi
        pop    ebx
        pop    ebp
        ret    
loc_1_2FCE:
        mov    ecx, dword ptr [ebx + 0xc]
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [edi], eax
        mov    ax, word ptr [ecx + 8]
        mov    word ptr [edi + 4], ax
        mov    ecx, dword ptr [esi + 0x20]
        movzx  eax, word ptr [ecx + 0x24]
        xchg   ah, al
        mov    dword ptr [ebx + 0x14], eax
        jmp    loc_1_2FC3
loc_1_2FEC:
        mov    byte ptr [ebx + 1], 0xfa
        mov    dword ptr [ebx + 0x14], 0
        jmp    loc_1_2FCA
sub_1_2FF9:
        mov    eax, dword ptr [esp + 4]
        cmp    dword ptr [0x58ec], eax       ; -> loc_1_58EC
        jne    loc_1_302B
        mov    eax, dword ptr [eax + 0x18]
        push   eax
        VxDCall  Dev_0492.Service_0000h          ; cd2000009204
        jb     loc_1_3017
        VxDCall  Dev_0492.Service_0003h          ; cd2003009204
loc_1_3017:
        VxDCall  Dev_0494.Service_0000h          ; cd2000009404
        jb     loc_1_3025
        VxDCall  Dev_0494.Service_0002h          ; cd2002009404
loc_1_3025:
        pop    eax
        call   sub_1_76E9
loc_1_302B:
        ret    
sub_1_302C:
        push   esi
        mov    eax, dword ptr [esp + 8]
        push   0
        mov    edx, esp
        mov    esi, 0x3051                   ; -> loc_1_3051
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    esi, edx
loc_1_3042:
        cmp    byte ptr [esi], 0
        jne    loc_1_304E
        call   sub_1_4A98
        jmp    loc_1_3042
loc_1_304E:
        pop    edx
        pop    esi
        ret    
loc_1_3051:
        mov    byte ptr [edx], 0xff
        ret    
        db  000h, 000h, 000h
NWLINK_Service_02:
        test   dword ptr [0x58f8], 0x10      ; -> loc_1_58F8
        je     loc_1_3070
        mov    eax, 0x56d0                   ; -> loc_1_56D0
        mov    esi, 0x343a                   ; -> sub_1_343A
        clc    
        ret    
loc_1_3070:
        xor    eax, eax
        stc    
        ret    
NWLINK_Service_05:
        cmp    eax, 0x101
        je     loc_1_3088
        cmp    eax, 0x102
        je     loc_1_3090
        mov    eax, 0xffffffff
        ret    
loc_1_3088:
        push   esi
        call   sub_1_75B3
        pop    esi
        ret    
loc_1_3090:
        push   esi
        call   sub_1_770D
        pop    esi
        ret    
        ; ---- dispatch table (5 entries) ----
        dd  loc_1_30AC
        dd  loc_1_30C1
        dd  loc_1_30ED
        dd  loc_1_3174
        dd  loc_1_3277
loc_1_30AC:
        xor    esi, esi
        ret    
loc_1_30AF:
        movzx  eax, word ptr [edi + 0x1e]
        xchg   ah, al
        cmp    eax, 5
        jae    loc_1_30AC
        jmp    dword ptr [eax*4 + 0x3098]    ; -> obj1:0x3098
loc_1_30C1:
        test   dword ptr [0x58f8], 0x10      ; -> loc_1_58F8
        je     loc_1_30DF
        lea    eax, [edi + 0x16]
        push   dword ptr [edi + 0x20]
        push   1
        push   eax
        push   ebx
        call   sub_1_6AB9
        add    esp, 0x10
loc_1_30DF:
        xor    esi, esi
        ret    
loc_1_30E2:
        pop    eax
        xor    esi, esi
        ret    
loc_1_30E6:
        and    dword ptr [ebx + 8], 0xffffffdf
loc_1_30EA:
        xor    esi, esi
        ret    
loc_1_30ED:
        test   dword ptr [0x58f8], 0x10      ; -> loc_1_58F8
        je     loc_1_30EA
        xor    edx, edx
        mov    eax, dword ptr [ebx + 0x1c]
        cmp    dword ptr [esi], eax
        jne    loc_1_3116
        mov    ax, word ptr [ebx + 0x20]
        cmp    word ptr [esi + 4], ax
        jne    loc_1_3116
        test   dword ptr [ebx + 8], 0x20
        jne    loc_1_30E6
        inc    edx
loc_1_3116:
        push   0
        mov    esi, edx
        movzx  edx, word ptr [edi + 2]
        xchg   dh, dl
        sub    ecx, edx
        sbb    eax, eax
        and    ecx, eax
        add    ecx, edx
        cmp    ecx, edx
        je     loc_1_313B
        call   sub_1_291A
        jb     loc_1_30E2
        mov    ecx, edx
        mov    edi, dword ptr [eax + 0x48]
        mov    dword ptr [esp], eax
loc_1_313B:
        sub    ecx, 0x20
        lea    edx, [edi + 0x16]
        add    edi, 0x20
        shr    ecx, 6
        jecxz  loc_1_3155
        push   edx
loc_1_314A:
        call   sub_1_3311
        add    edi, 0x40
        loop   loc_1_314A
        pop    ecx
loc_1_3155:
        pop    ecx
        jecxz  loc_1_3169
        mov    eax, ecx
        xchg   dword ptr [0x7d1c], eax       ; -> loc_1_7D1C
        mov    dword ptr [ecx + 0x38], eax
        inc    dword ptr [0x7cb4]            ; -> obj1:0x7CB4
loc_1_3169:
        xor    esi, esi
        ret    
        db  0EBh, 0E7h
loc_1_316E:
        add    esp, 0xc
loc_1_3171:
        xor    esi, esi
        ret    
loc_1_3174:
        test   dword ptr [0x58f8], 0x1000    ; -> loc_1_58F8
        je     loc_1_318A
        push   ebx
        push   edi
        call   sub_1_78F3
        add    esp, 8
loc_1_318A:
        test   dword ptr [0x58f8], 0x40      ; -> loc_1_58F8
        je     loc_1_3171
        mov    ax, word ptr [edi + 0x20]
        push   edi
        mov    edi, dword ptr [0x56d0]       ; -> loc_1_56D0
        mov    ecx, dword ptr [0x56d8]       ; -> loc_1_56D8
        xor    esi, esi
loc_1_31A9:
        jecxz  loc_1_3229
        repne scasw ax, word ptr es:[edi]
        jne    loc_1_3229
        push   ecx
        push   edi
        neg    ecx
        add    ecx, dword ptr [0x56d8]       ; -> loc_1_56D8
        mov    edi, dword ptr [0x56d4]       ; -> loc_1_56D4
        mov    ecx, dword ptr [edi + ecx*4 - 4]
        mov    edi, dword ptr [0x5684]       ; -> loc_1_5684
        cmp    dword ptr [ecx + 0x58], edi
        jb     loc_1_31FA
        mov    edi, dword ptr [ecx + 0x46]
        cmp    dword ptr [ebx + 0x18], edi
        je     loc_1_316E
        or     esi, esi
        je     loc_1_320E
        push   eax
        mov    edi, dword ptr [ecx + 0xc]
        mov    dh, byte ptr [edi + 0x17]
        mov    eax, dword ptr [ebx + 4]
        test   dword ptr [edi + 4], eax
        jne    loc_1_31ED
        add    dh, byte ptr [ebx + 0x24]
loc_1_31ED:
        pop    eax
        cmp    dh, dl
        jae    loc_1_31F6
        mov    dl, dh
        mov    esi, ecx
loc_1_31F6:
        pop    edi
        pop    ecx
        jmp    loc_1_31A9
loc_1_31FA:
        push   eax
        push   esi
        push   edx
        mov    esi, ecx
        call   sub_1_34DE
        pop    edx
        pop    esi
        pop    eax
        pop    edi
        sub    edi, 2
        pop    ecx
        jmp    loc_1_31A9
loc_1_320E:
        mov    esi, ecx
        mov    edi, dword ptr [esi + 0xc]
        mov    dl, byte ptr [edi + 0x17]
        mov    ecx, dword ptr [ebx + 4]
        test   dword ptr [edi + 4], ecx
        jne    loc_1_3221
        add    dl, byte ptr [ebx + 0x24]
loc_1_3221:
        pop    edi
        pop    ecx
        jmp    loc_1_31A9
loc_1_3225:
        pop    edi
        xor    esi, esi
        ret    
loc_1_3229:
        or     esi, esi
        je     loc_1_3225
        mov    edx, dword ptr [esi + 0xc]
        mov    eax, dword ptr [ebx + 4]
        test   dword ptr [edx + 4], eax
        jne    loc_1_3225
        pop    edi
        call   sub_1_2A8C
        jb     loc_1_3274
        mov    word ptr [ecx + 0xa], 0x5204
        mov    dword ptr [ecx + 0x24], edx
        mov    word ptr [ecx + 0x28], 0x60
        mov    byte ptr [edx + 5], 4
        mov    word ptr [edx + 0x1e], 0x400
        mov    eax, ecx
        add    esi, 0x14
        lea    edi, [edx + 0x20]
        mov    ecx, 0x10
        rep movsd dword ptr es:[edi], dword ptr [esi]
        inc    byte ptr [edx + 0x5f]
        mov    esi, eax
        mov    edi, ebx
        call   sub_1_410D
loc_1_3274:
        xor    esi, esi
        ret    
loc_1_3277:
        xor    esi, esi
        ret    
loc_1_327A:
        call   sub_1_67CD
        mov    dword ptr [0x56cc], 0         ; -> loc_1_56CC
        ret    
sub_1_328A:
        push   esi
        mov    esi, dword ptr [0x56cc]       ; -> loc_1_56CC
        VMMCall  VMM.Cancel_Restricted_Event     ; cd205b010100
        mov    dword ptr [0x56cc], 0         ; -> loc_1_56CC
        pop    esi
sub_1_32A2:
        push   ebx
        VxDCall  PAGESWAP.PageSwap_Get_Version   ; cd2000000700
        cmp    bl, 2
        jne    loc_1_32D5
        cmp    dword ptr [0x56cc], 0         ; -> loc_1_56CC
        jne    loc_1_32DA
        push   esi
        mov    esi, 0x327a                   ; -> loc_1_327A
        xor    eax, eax
        xor    ebx, ebx
        mov    ecx, 2
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
        mov    dword ptr [0x56cc], esi       ; -> loc_1_56CC
        pop    esi
        jmp    loc_1_32DA
loc_1_32D5:
        call   sub_1_67CD
loc_1_32DA:
        pop    ebx
        ret    
loc_1_32DC:
        push   edi
        mov    ax, word ptr [edi]
        cmp    dword ptr [0x56e0], 0         ; -> loc_1_56E0
        je     loc_1_32FD
        mov    edi, dword ptr [0x56e8]       ; -> loc_1_56E8
        mov    ecx, dword ptr [0x56e0]       ; -> loc_1_56E0
        repne scasw ax, word ptr es:[edi]
        pop    edi
        je     loc_1_331E
        pop    ecx
        ret    
loc_1_32FD:
        mov    edi, dword ptr [0x56ec]       ; -> loc_1_56EC
        mov    ecx, dword ptr [0x56e4]       ; -> loc_1_56E4
        repne scasw ax, word ptr es:[edi]
        pop    edi
        jne    loc_1_331E
        pop    ecx
        ret    
sub_1_3311:
        push   ecx
        test   dword ptr [0x58f8], 0x10000   ; -> loc_1_58F8
        jne    loc_1_32DC
loc_1_331E:
        mov    esi, edi
        call   sub_1_343A
        jb     loc_1_3384
        cmp    word ptr [edi + 0x3e], 0x1000
        je     loc_1_334E
        mov    ecx, dword ptr [0x5684]       ; -> loc_1_5684
        add    ecx, 0x2bf20
        mov    dword ptr [esi + 0x58], ecx
        push   edi
        xchg   edi, esi
        add    edi, 0x14
        mov    ecx, 0x10
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    edi
        pop    ecx
        ret    
loc_1_334E:
        call   sub_1_34DE
        pop    ecx
        ret    
loc_1_3355:
        mov    eax, edx
        xchg   dword ptr [0x7d24], eax       ; -> loc_1_7D24
        mov    dword ptr [edx], eax
        pop    esi
        pop    edi
        inc    dword ptr [0x7d0c]            ; -> loc_1_7D0C
        pop    ecx
        ret    
loc_1_3369:
        inc    dword ptr [0x7d0c]            ; -> loc_1_7D0C
        call   sub_1_32A2
        dec    dword ptr [0x7d0c]            ; -> loc_1_7D0C
        jns    loc_1_3394
        inc    dword ptr [0x7d0c]            ; -> loc_1_7D0C
loc_1_3382:
        pop    ecx
        ret    
loc_1_3384:
        cmp    word ptr [edi + 0x3e], 0x1000
        je     loc_1_3382
        dec    dword ptr [0x7d0c]            ; -> loc_1_7D0C
        js     loc_1_3369
loc_1_3394:
        push   edi
        push   esi
        mov    edx, dword ptr [0x7d24]       ; -> loc_1_7D24
        mov    esi, dword ptr [edx]
        mov    dword ptr [0x7d24], esi       ; -> loc_1_7D24
        mov    dword ptr [edx + 0x54], ecx
        push   eax
        push   edx
        mov    eax, dword ptr [edi + 0x32]
        call   sub_1_2D10
        pop    edx
        pop    eax
        jb     loc_1_3355
        mov    dword ptr [edx + 0xc], esi
        mov    ecx, edx
        xchg   dword ptr [esi + 0x1c], ecx
        mov    dword ptr [edx + 8], ecx
        mov    ecx, 0x10
        lea    esi, [edx + 0x14]
        xchg   esi, edi
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, dword ptr [0x5684]       ; -> loc_1_5684
        add    ecx, 0x2bf20
        mov    dword ptr [edx + 0x58], ecx
        mov    dword ptr [edx + 0x64], ebx
        mov    ecx, dword ptr [0x56d8]       ; -> loc_1_56D8
        mov    esi, dword ptr [0x56d4]       ; -> loc_1_56D4
        mov    dword ptr [esi + ecx*4], edx
        mov    esi, dword ptr [0x56d0]       ; -> loc_1_56D0
        mov    di, word ptr [edx + 0x14]
        mov    word ptr [esi + ecx*2], di
        inc    dword ptr [0x56d8]            ; -> loc_1_56D8
        pop    ecx
        jecxz  loc_1_3412
        mov    dword ptr [edx], 0
        mov    dword ptr [ecx], edx
        mov    dword ptr [edx + 4], ecx
        pop    edi
        pop    ecx
        ret    
loc_1_3412:
        mov    edi, dword ptr [0x56c8]       ; -> loc_1_56C8
        inc    dword ptr [0x56c8]            ; -> loc_1_56C8
        mov    esi, dword ptr [0x56c0]       ; -> loc_1_56C0
        mov    dword ptr [esi + edi*4], eax
        mov    esi, dword ptr [0x56c4]       ; -> loc_1_56C4
        lea    eax, [esi + edi*4]
        mov    dword ptr [eax], edx
        mov    dword ptr [edx], ecx
        mov    dword ptr [edx + 4], eax
        pop    edi
        pop    ecx
        ret    
sub_1_343A:
        push   ebx
        push   edi
        mov    edi, esi
        call   sub_1_34AF
        mov    eax, edx
        mov    ebx, ecx
        call   sub_1_3488
        jb     loc_1_3484
        mov    eax, esi
        mov    edx, edi
loc_1_3452:
        cmp    dword ptr [eax + 0x54], ebx
        jne    loc_1_3476
        lea    esi, [eax + 0x14]
        mov    edi, edx
        mov    ecx, ebx
        shr    ecx, 2
        repe cmpsd dword ptr [esi], dword ptr es:[edi]
        jne    loc_1_3476
        mov    ecx, ebx
        and    cl, 3
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        jne    loc_1_3476
        mov    esi, eax
        clc    
        mov    ecx, ebx
        pop    edi
        pop    ebx
        ret    
loc_1_3476:
        mov    esi, eax
        mov    eax, dword ptr [esi]
        or     eax, eax
        jne    loc_1_3452
loc_1_347E:
        stc    
        mov    ecx, ebx
        pop    edi
        pop    ebx
        ret    
loc_1_3484:
        xor    esi, esi
        jmp    loc_1_347E
sub_1_3488:
        cld    
        push   edi
        mov    edi, dword ptr [0x56c0]       ; -> loc_1_56C0
        mov    ecx, dword ptr [0x56c8]       ; -> loc_1_56C8
        mov    edx, ecx
        or     esi, esi
        repne scasd eax, dword ptr es:[edi]
        pop    edi
        jne    loc_1_34AD
        sub    edx, ecx
        mov    esi, dword ptr [0x56c4]       ; -> loc_1_56C4
        mov    esi, dword ptr [esi + edx*4 - 4]
        clc    
        ret    
loc_1_34AD:
        stc    
        ret    
sub_1_34AF:
        push   edi
        mov    ecx, 0x30
        xor    eax, eax
        lea    edi, [esi + 2]
        repne scasb al, byte ptr es:[edi]
        neg    ecx
        lea    ecx, [ecx + 0x32]
        push   ecx
        shr    ecx, 2
        xor    edx, edx
        jecxz  loc_1_34CE
loc_1_34C9:
        lodsd  eax, dword ptr [esi]
        xor    edx, eax
        loop   loc_1_34C9
loc_1_34CE:
        mov    ecx, dword ptr [esp]
        and    cl, 3
        je     loc_1_34DB
loc_1_34D6:
        lodsb  al, byte ptr [esi]
        xor    dl, al
        loop   loc_1_34D6
loc_1_34DB:
        pop    ecx
        pop    edi
        ret    
sub_1_34DE:
        push   edi
        mov    ecx, dword ptr [esi + 0xc]
        jecxz  loc_1_3504
        mov    eax, dword ptr [ecx + 0x1c]
        cmp    eax, esi
        jne    loc_1_34F3
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [ecx + 0x1c], eax
        jmp    loc_1_3504
loc_1_34F3:
        mov    ecx, dword ptr [eax + 8]
        cmp    ecx, esi
        je     loc_1_34FE
        mov    eax, ecx
        jmp    loc_1_34F3
loc_1_34FE:
        mov    ecx, dword ptr [ecx + 8]
        mov    dword ptr [eax + 8], ecx
loc_1_3504:
        mov    eax, esi
        mov    ecx, dword ptr [0x56d8]       ; -> loc_1_56D8
        mov    edx, ecx
        mov    edi, dword ptr [0x56d4]       ; -> loc_1_56D4
        push   edi
        repne scasd eax, dword ptr es:[edi]
        neg    ecx
        lea    ecx, [edx + ecx - 1]
        pop    edi
        mov    eax, dword ptr [edi + edx*4 - 4]
        mov    dword ptr [edi + ecx*4], eax
        mov    edi, dword ptr [0x56d0]       ; -> loc_1_56D0
        mov    ax, word ptr [edi + edx*2 - 2]
        mov    word ptr [edi + ecx*2], ax
        dec    dword ptr [0x56d8]            ; -> loc_1_56D8
        mov    eax, dword ptr [esi + 4]
        mov    ecx, dword ptr [esi]
        mov    dword ptr [eax], ecx
        mov    edi, esi
        xchg   dword ptr [0x7d24], edi       ; -> loc_1_7D24
        mov    dword ptr [esi], edi
        inc    dword ptr [0x7d0c]            ; -> loc_1_7D0C
        jecxz  loc_1_3558
        mov    dword ptr [ecx + 4], eax
        pop    edi
        ret    
loc_1_3558:
        mov    edx, eax
        sub    edx, dword ptr [0x56c4]       ; -> loc_1_56C4
        mov    ecx, dword ptr [0x56c8]       ; -> loc_1_56C8
        shl    ecx, 2
        cmp    edx, ecx
        jb     loc_1_356F
        pop    edi
        ret    
loc_1_356F:
        lea    edi, [ecx - 4]
        cmp    edi, edx
        je     loc_1_3595
        mov    esi, dword ptr [0x56c4]       ; -> loc_1_56C4
        mov    edi, dword ptr [ecx + esi - 4]
        lea    esi, [edx + esi]
        mov    dword ptr [esi], edi
        mov    dword ptr [edi + 4], esi
        mov    esi, dword ptr [0x56c0]       ; -> loc_1_56C0
        mov    edi, dword ptr [ecx + esi - 4]
        mov    dword ptr [edx + esi], edi
loc_1_3595:
        dec    dword ptr [0x56c8]            ; -> loc_1_56C8
        pop    edi
        ret    
loc_1_359D:
        mov    ax, word ptr [esp + 4]
        push   edi
        mov    edi, dword ptr [0x56d0]       ; -> loc_1_56D0
        mov    ecx, dword ptr [0x56d8]       ; -> loc_1_56D8
        sub    ecx, edx
        push   ecx
        jecxz  loc_1_35C6
        lea    edi, [edi + edx*2]
        repne scasw ax, word ptr es:[edi]
        jne    loc_1_35C6
        pop    eax
        sub    eax, ecx
        lea    edx, [eax + edx - 1]
        pop    edi
        jmp    loc_1_35F1
loc_1_35C6:
        pop    ecx
        pop    edi
        jmp    loc_1_3638
sub_1_35CA:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [eax + 8]
        mov    edi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [eax + 4]
        push   dword ptr [eax + 0x16]
        push   7
        cmp    edx, dword ptr [0x56d8]       ; -> loc_1_56D8
        jae    loc_1_3638
loc_1_35E9:
        cmp    word ptr [esp + 4], -1
        jne    loc_1_359D
loc_1_35F1:
        mov    esi, dword ptr [0x56d4]       ; -> loc_1_56D4
        mov    esi, dword ptr [esi + edx*4]
        mov    eax, dword ptr [0x5684]       ; -> loc_1_5684
        cmp    dword ptr [esi + 0x58], eax
        jb     loc_1_364A
        mov    ecx, dword ptr [esi + 0xc]
        mov    eax, dword ptr [ebx + 4]
        test   dword ptr [ecx + 4], eax
        jne    loc_1_362D
        mov    ax, word ptr [esi + 0x52]
        cmp    ah, 0xf
        je     loc_1_362D
        inc    ah
        mov    ecx, 0x10
        add    esi, 0x14
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    word ptr [edi - 2], ax
        dec    dword ptr [esp]
        je     loc_1_3653
loc_1_362D:
        inc    edx
loc_1_362E:
        cmp    dword ptr [0x56d8], edx       ; -> loc_1_56D8
        jne    loc_1_35E9
        xor    edx, edx
loc_1_3638:
        pop    ecx
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [eax + 8], edx
        neg    ecx
        lea    eax, [ecx + 7]
        pop    edi
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
loc_1_364A:
        push   edx
        call   sub_1_34DE
        pop    edx
        jmp    loc_1_362E
loc_1_3653:
        inc    edx
        cmp    dword ptr [0x56d8], edx       ; -> loc_1_56D8
        jne    loc_1_3638
        xor    edx, edx
        jmp    loc_1_3638
loc_1_3660:
        push   0
        push   0x8a
        push   eax
        VxDCall  VPOWERD._VPOWERD_Deregister_Power_Handler ; cd200b002800
        ret    
        db  000h
loc_1_3670:
        cmp    dword ptr [0x58c8], 0         ; -> loc_1_58C8
        je     loc_1_368A
        mov    edi, dword ptr [0x58ec]       ; -> loc_1_58EC
        mov    eax, dword ptr [edi + 0xc]
        and    al, 0xfe
        mov    ecx, 0xa
        ret    
loc_1_368A:
        xor    eax, eax
        ret    
loc_1_368D:
        VMMCall  VMM.Simulate_Far_Ret            ; cd204a000100
loc_1_3693:
        mov    eax, ebx
        movzx  ebx, word ptr [ebp + 0x10]
        cmp    ebx, 0x1b
        jae    loc_1_3750
        test   byte ptr [ebx + 0x577c], 1    ; -> obj1:0x577C
        je     loc_1_36F9
        push   eax
        movzx  ecx, word ptr [ebp + 0x38]
        push   0
        push   ecx
        push   eax
        VMMCall  VMM._SelectorMapFlat            ; cd206e000100
        add    esp, 0xc
        movzx  edx, word ptr [ebp + 4]
        lea    esi, [edx + eax]
        movzx  edi, word ptr [ebp]
        add    edi, eax
        pop    eax
        test   byte ptr [ebx + 0x577c], 2    ; -> obj1:0x577C
        je     loc_1_36F9
        shl    dword ptr [esi + 0xc], 0x10
        cmp    ebx, 7
        je     loc_1_36E7
        mov    dword ptr [esi + 0x18], 0x4971 ; -> sub_1_4971
        mov    dword ptr [esi + 0x14], eax
loc_1_36E7:
        mov    word ptr [esi + 0xc], 1
        mov    cx, word ptr [ebp + 0x38]
        shl    ecx, 0x10
        mov    cx, dx
        mov    dword ptr [esi], ecx
loc_1_36F9:
        movzx  edx, word ptr [ebp + 0x14]
        test   dword ptr [0x58f8], 1         ; -> loc_1_58F8
        je     loc_1_3756
        test   dword ptr [0x58f8], 0x20000   ; -> loc_1_58F8
        je     loc_1_371E
        push   dword ptr [ebp + 0x2c]
        VMMCall  VMM.Enable_VM_Ints              ; cd201a000100
loc_1_371E:
        xor    ecx, ecx
        call   sub_1_56F0
loc_1_3727:
        mov    word ptr [ebp + 0x1c], ax
        mov    word ptr [ebp + 0x18], cx
        mov    word ptr [ebp + 0x14], dx
        test   dword ptr [0x58f8], 0x20000   ; -> loc_1_58F8
        jne    loc_1_3740
        ret    
loc_1_3740:
        pop    eax
        test   eax, 0x200
        je     loc_1_3749
        ret    
loc_1_3749:
        VMMCall  VMM.Disable_VM_Ints             ; cd201b000100
        ret    
loc_1_3750:
        mov    ax, 1
        jmp    loc_1_3727
loc_1_3756:
        mov    ax, 2
        jmp    loc_1_3727
loc_1_375C:
        cmp    word ptr [ebp + 0x1c], 0x7a00
        je     loc_1_3788
        cmp    word ptr [ebp + 0x1c], 0x1684
        je     loc_1_377E
loc_1_376C:
        mov    ecx, dword ptr [0x5904]       ; -> loc_1_5904
        mov    edx, dword ptr [0x5908]       ; -> loc_1_5908
        VMMCall  VMM.Simulate_Far_Jmp            ; cd2049800100
loc_1_377E:
        cmp    word ptr [ebp + 0x10], 0x200
        jne    loc_1_376C
        jmp    loc_1_378C
loc_1_3788:
        mov    byte ptr [ebp + 0x1c], 0xff
loc_1_378C:
        mov    eax, dword ptr [0x5900]       ; -> loc_1_5900
        mov    word ptr [ebp], ax
        shr    eax, 0x10
        mov    word ptr [ebp + 0x38], ax
        VMMCall  VMM.Simulate_Iret               ; cd2047800100
loc_1_37A2:
        VMMCall  VMM.Simulate_Far_Ret            ; cd204a000100
loc_1_37A8:
        mov    eax, ebx
        movzx  ebx, word ptr [ebp + 0x10]
        cmp    ebx, 0x1b
        jae    loc_1_3869
        test   dword ptr [0x58f8], 1         ; -> loc_1_58F8
        je     loc_1_3863
        test   byte ptr [ebx + 0x577c], 1    ; -> obj1:0x577C
        je     loc_1_3815
        movzx  edx, word ptr [ebp + 0x38]
        shl    edx, 4
        add    edx, dword ptr [eax + 4]
        movzx  esi, word ptr [ebp + 4]
        add    esi, edx
        movzx  edi, word ptr [ebp]
        add    edi, edx
        test   byte ptr [ebx + 0x577c], 2    ; -> obj1:0x577C
        je     loc_1_3815
        shl    dword ptr [esi + 0xc], 0x10
        cmp    ebx, 7
        je     loc_1_3802
        mov    dword ptr [esi + 0x18], 0x4971 ; -> sub_1_4971
        mov    dword ptr [esi + 0x14], eax
loc_1_3802:
        mov    word ptr [esi + 0xc], 2
        mov    cx, word ptr [ebp + 0x38]
        shl    ecx, 0x10
        mov    cx, word ptr [ebp + 4]
        mov    dword ptr [esi], ecx
loc_1_3815:
        movzx  edx, word ptr [ebp + 0x14]
        test   dword ptr [0x58f8], 0x20000   ; -> loc_1_58F8
        je     loc_1_382E
        push   dword ptr [ebp + 0x2c]
        VMMCall  VMM.Enable_VM_Ints              ; cd201a000100
loc_1_382E:
        xor    ecx, ecx
        call   sub_1_56F0
loc_1_3837:
        mov    word ptr [ebp + 0x1c], ax
        mov    word ptr [ebp + 0x18], cx
        mov    word ptr [ebp + 0x14], dx
        test   dword ptr [0x58f8], 0x20000   ; -> loc_1_58F8
        jne    loc_1_3851
        clc    
        ret    
loc_1_3851:
        pop    eax
        test   eax, 0x200
        je     loc_1_385B
        clc    
        ret    
loc_1_385B:
        VMMCall  VMM.Disable_VM_Ints             ; cd201b000100
        clc    
        ret    
loc_1_3863:
        mov    ax, 2
        jmp    loc_1_3837
loc_1_3869:
        mov    ax, 1
        stc    
        ret    
loc_1_386F:
        cmp    word ptr [ebp + 0x1c], 0x7a00
        je     loc_1_3879
        stc    
        ret    
loc_1_3879:
        mov    byte ptr [ebp + 0x1c], 0xff
        mov    eax, dword ptr [0x58fc]       ; -> loc_1_58FC
        mov    word ptr [ebp], ax
        shr    eax, 0x10
        mov    word ptr [ebp + 0x38], ax
        mov    eax, dword ptr [0x5530]       ; -> obj1:0x5530
        mov    word ptr [ebp + 0x10], ax
        clc    
        ret    
loc_1_3898:
        mov    byte ptr [esi + 9], 1
        mov    byte ptr [esi + 8], 0
        ret    
loc_1_38A1:
        mov    byte ptr [esi + 9], 0xfd
        mov    byte ptr [esi + 8], 0
        ret    
sub_1_38AA:
        and    dword ptr [eax + 4], 0xfffffdff
        or     dword ptr [eax + 4], 0x40000
        cmp    word ptr [esi + 0x22], 1
        jb     loc_1_38A1
        cmp    word ptr [esi + 0x28], 0x2a
        jb     loc_1_38A1
        mov    byte ptr [esi + 9], 0
        mov    byte ptr [esi + 8], 0xff
        mov    ebx, eax
        push   esi
        call   sub_1_4488
        or     eax, eax
        je     loc_1_3898
        pop    edx
        mov    edi, dword ptr [eax + 8]
        mov    edi, dword ptr [edi + 4]
        mov    byte ptr [edi + 0x1f], 0xfe
        jmp    loc_1_396A
loc_1_38EA:
        mov    byte ptr [esi + 9], 0xfd
        mov    edx, eax
        call   sub_1_4619
        mov    al, 0xfd
        inc    word ptr [0x5952]             ; -> loc_1_5952
        ret    
loc_1_38FF:
        mov    byte ptr [esi + 9], 1
        jmp    loc_1_390F
loc_1_3905:
        mov    byte ptr [esi + 9], 0xee
        jmp    loc_1_390F
loc_1_390B:
        mov    byte ptr [esi + 9], 0xfd
loc_1_390F:
        mov    byte ptr [esi + 8], 0
        inc    word ptr [0x5952]             ; -> loc_1_5952
        ret    
sub_1_391B:
        or     dword ptr [esi + 0xc], 0x80
        inc    dword ptr [0x594a]            ; -> loc_1_594A
        inc    dword ptr [0x591c]            ; -> loc_1_591C
        push   edx
        call   sub_1_42D3
        pop    edx
        jb     loc_1_3905
        test   dword ptr [eax + 4], 0x200
        je     loc_1_3905
        cmp    word ptr [esi + 0x28], 0x2a
        jb     loc_1_390B
        mov    byte ptr [esi + 9], 0
        mov    ebx, eax
        push   esi
        call   sub_1_4488
        or     eax, eax
        je     loc_1_38FF
        pop    edx
        jb     loc_1_390B
        push   eax
        mov    eax, dword ptr [eax + 4]
        mov    edx, dword ptr [0x58ec]       ; -> loc_1_58EC
        cmp    dword ptr [edx + 0x10], eax
        pop    eax
        jb     loc_1_38EA
loc_1_396A:
        mov    byte ptr [esi + 8], 0xfa
        mov    edx, esi
        lea    esi, [ebx + 0x38]
        lea    edi, [edx + 0x1c]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        mov    edi, dword ptr [eax + 8]
        mov    edi, dword ptr [edi + 4]
        push   edi
        and    byte ptr [edi + 0x1e], 0x1f
        or     byte ptr [edi + 0x1e], 0x40
        mov    ecx, dword ptr [ebx + 0x5e]
        mov    dword ptr [edi + 0x20], ecx
        mov    byte ptr [edi + 5], 5
        add    edi, 6
        lea    esi, [ebx + 0x44]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        pop    edi
        xor    ecx, ecx
        mov    dword ptr [edx + 0x10], ecx
        mov    esi, edx
        cmp    dword ptr [ebx + 0x24], ecx
        jne    loc_1_39F9
        test   dword ptr [ebx + 4], 0x80
        jne    loc_1_39F9
        test   dword ptr [ebx + 4], 0x20
        jne    loc_1_3A1C
        mov    ecx, dword ptr [ebx + 0x30]
        mov    dword ptr [ebx + 0x34], ecx
        mov    ecx, dword ptr [ebx + 0x1c]
        mov    dword ptr [ebx + 0x20], ecx
        or     dword ptr [ebx + 4], 1
        push   eax
        mov    eax, ebx
        mov    dx, word ptr [eax + 0x62]
        mov    word ptr [edi + 0x24], dx
        mov    dword ptr [ebx + 0x24], esi
        mov    dword ptr [ebx + 0x28], esi
        add    dh, 1
        adc    dl, 0
        mov    word ptr [eax + 0x62], dx
        call   sub_1_441F
        mov    ebx, eax
        pop    eax
        mov    edi, dword ptr [ebx + 8]
        mov    dx, word ptr [ebx + 0x5a]
        jmp    sub_1_41D1
loc_1_39F9:
        mov    edi, dword ptr [ebx + 8]
        mov    dx, word ptr [ebx + 0x5a]
        call   sub_1_41D1
        cmp    dword ptr [ebx + 0x24], 0
        jne    loc_1_3A12
        mov    dword ptr [ebx + 0x24], esi
        mov    dword ptr [ebx + 0x28], esi
        ret    
loc_1_3A12:
        mov    ecx, dword ptr [ebx + 0x28]
        mov    dword ptr [ecx + 0x10], esi
        mov    dword ptr [ebx + 0x28], esi
        ret    
loc_1_3A1C:
        inc    dword ptr [0x594e]            ; -> loc_1_594E
        jmp    loc_1_39F9
loc_1_3A24:
        test   dword ptr [eax + 4], 0x10000
        je     loc_1_3A64
        test   dword ptr [eax + 4], 0x100000
        jne    loc_1_3A64
        or     dword ptr [eax + 4], 0x100000
        mov    esi, 0x3c8b                   ; -> loc_1_3C8B
        mov    edx, eax
        VMMCall  VMM.Schedule_Global_Event       ; cd200e000100
        mov    cx, word ptr [ebx + 0x20]
        mov    word ptr [edx + 0x60], cx
        mov    cx, word ptr [ebx + 0x1c]
        mov    word ptr [edx + 0x4e], cx
        jmp    loc_1_3A64
loc_1_3A5C:
        mov    dword ptr [eax + 0x68], ecx
        jmp    loc_1_3B26
loc_1_3A64:
        pop    edx
        pop    esi
        pop    edi
loc_1_3A67:
        stc    
        ret    
sub_1_3A69:
        cmp    edx, 0x2a
        jb     loc_1_3A67
        cmp    byte ptr [ebx + 5], 5
        jne    loc_1_3A67
        inc    dword ptr [0x595e]            ; -> loc_1_595E
        push   eax
        push   dword ptr [ebx + 0x22]
        call   sub_1_42D3
        pop    ecx
        jb     loc_1_3BE0
        xchg   dword ptr [esp], edi
        push   esi
        push   edx
        cmp    dword ptr [eax + 0xc], edi
        jne    loc_1_3A64
        test   dword ptr [eax + 4], 0x100
        je     loc_1_3A24
        or     dword ptr [eax + 4], 2
        and    dword ptr [eax + 4], 0xffffff7f
        mov    edx, dword ptr [ebx + 0x26]
        mov    ecx, edx
        xchg   dword ptr [eax + 0x68], ecx
        cmp    cx, dx
        je     loc_1_3AF4
        cmp    word ptr [eax + 0x62], dx
        jne    loc_1_3A5C
        test   dword ptr [eax + 4], 1
        je     loc_1_3AF4
        and    dword ptr [eax + 4], 0xfffffffe
        mov    dword ptr [eax + 0x20], 0xffffffff
        mov    edx, dword ptr [eax + 0x24]
        mov    esi, dword ptr [edx + 0x10]
        mov    dword ptr [eax + 0x24], esi
        mov    edx, dword ptr [edx]
        add    edx, 0x58
        mov    esi, dword ptr [0x588c]       ; -> loc_1_588C
        mov    dword ptr [0x588c], edx       ; -> loc_1_588C
        mov    dword ptr [esi], edx
        mov    dword ptr [edx + 4], esi
        mov    dword ptr [edx], 0x5888       ; -> loc_1_5888
loc_1_3AF4:
        mov    dx, word ptr [ebx + 0x28]
        add    dh, 1
        adc    dl, 0
        cmp    word ptr [eax + 0x62], dx
        je     loc_1_3B7E
        and    dword ptr [eax + 4], 0xffffffdf
        test   dword ptr [eax + 4], 9
        jne    loc_1_3B26
        cmp    dword ptr [eax + 0x24], 0
        je     loc_1_3B26
        mov    edx, eax
        xchg   dword ptr [0x5890], edx       ; -> loc_1_5890
        mov    dword ptr [eax + 0x14], edx
        or     dword ptr [eax + 4], 8
loc_1_3B26:
        xor    ecx, ecx
        test   byte ptr [ebx + 0x1e], 0x80
        jne    loc_1_3B8F
        mov    dx, word ptr [ebx + 0x24]
        cmp    word ptr [eax + 0x64], dx
        jne    loc_1_3B84
        mov    dl, byte ptr [ebx + 0x1f]
        cmp    dl, 0xff
        je     loc_1_3BA9
        dec    dword ptr [edi + 0x24]
        js     loc_1_3B9E
        add    byte ptr [eax + 0x65], 1
        adc    byte ptr [eax + 0x64], 0
        mov    ecx, dword ptr [edi + 0x28]
        mov    esi, dword ptr [ecx + 0x10]
        mov    dword ptr [edi + 0x28], esi
        cmp    dl, 0xfe
        je     loc_1_3BBB
loc_1_3B5B:
        or     dword ptr [eax + 4], 0x40
        test   dword ptr [eax + 4], 8
        jne    loc_1_3B77
        mov    edx, eax
        xchg   dword ptr [0x5890], edx       ; -> loc_1_5890
        mov    dword ptr [eax + 0x14], edx
        or     dword ptr [eax + 4], 8
loc_1_3B77:
        cmp    ecx, 1
loc_1_3B7A:
        pop    edx
        pop    esi
        pop    edi
        ret    
loc_1_3B7E:
        or     dword ptr [eax + 4], 0x20
        jmp    loc_1_3B26
loc_1_3B84:
        inc    word ptr [eax + 0x6e]
        inc    word ptr [0x5964]             ; -> loc_1_5964
loc_1_3B8F:
        cmp    byte ptr [ebx + 0x1f], 0xff
        je     loc_1_3BA9
loc_1_3B95:
        test   byte ptr [ebx + 0x1e], 0x40
        jne    loc_1_3B5B
        stc    
        jmp    loc_1_3B7A
loc_1_3B9E:
        or     dword ptr [eax + 4], 0x10
        inc    dword ptr [edi + 0x24]
        xor    ecx, ecx
        jmp    loc_1_3B5B
loc_1_3BA9:
        test   dword ptr [eax + 4], 0x40000
        je     loc_1_3B95
        or     dword ptr [eax + 4], 0x800
        jmp    loc_1_3B5B
loc_1_3BBB:
        or     dword ptr [eax + 4], 0x80000
        and    dword ptr [eax + 4], 0xfffffdff
        mov    byte ptr [eax], 4
        jmp    loc_1_3B5B
loc_1_3BCE:
        inc    word ptr [0x5962]             ; -> loc_1_5962
        stc    
        ret    
loc_1_3BD7:
        inc    word ptr [0x5966]             ; -> loc_1_5966
        stc    
        ret    
loc_1_3BE0:
        pop    eax
        cmp    word ptr [ebx + 0x22], -1
        jne    loc_1_3BCE
        cmp    byte ptr [eax + 0x17], 0
        je     loc_1_3BD7
        push   edx
        push   edi
        push   esi
        push   eax
        xor    edx, edx
        mov    eax, dword ptr [eax + 0x2c]
loc_1_3BF7:
        or     eax, eax
        je     loc_1_3C49
        test   dword ptr [eax + 4], 0x20000
        je     loc_1_3C0B
        mov    edx, eax
loc_1_3C06:
        mov    eax, dword ptr [eax + 0x10]
        jmp    loc_1_3BF7
loc_1_3C0B:
        mov    cx, word ptr [ebx + 0x20]
        cmp    word ptr [eax + 0x60], cx
        jne    loc_1_3C06
        lea    esi, [ebx + 0x12]
        lea    edi, [eax + 0x44]
        mov    ecx, 3
        repe cmpsd dword ptr [esi], dword ptr es:[edi]
        jne    loc_1_3C06
        or     dword ptr [eax + 4], 0x40
        test   dword ptr [eax + 4], 8
        jne    loc_1_3C43
        mov    edx, dword ptr [0x5890]       ; -> loc_1_5890
        mov    dword ptr [eax + 0x14], edx
        mov    dword ptr [0x5890], eax       ; -> loc_1_5890
        or     dword ptr [eax + 4], 8
loc_1_3C43:
        pop    eax
        pop    edi
        pop    esi
        pop    edx
        stc    
        ret    
loc_1_3C49:
        pop    eax
        dec    byte ptr [eax + 0x17]
        and    dword ptr [edx + 4], 0xfffdffff
        mov    cx, word ptr [ebx + 0x20]
        mov    word ptr [edx + 0x60], cx
        lea    esi, [ebx + 0x12]
        lea    edi, [edx + 0x44]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        pop    esi
        lea    edi, [edx + 0x38]
        lodsd  eax, dword ptr [esi]
        pop    ecx
        and    al, byte ptr [ecx + 0x22]
        stosd  dword ptr es:[edi], eax
        movsw  word ptr es:[edi], word ptr [esi]
        mov    edi, ecx
        mov    dword ptr [edx + 8], edi
        mov    esi, 0x3c8b                   ; -> loc_1_3C8B
        VMMCall  VMM.Schedule_Global_Event       ; cd200e000100
        pop    edx
        stc    
        ret    
loc_1_3C84:
        cmp    al, 0xff
        jne    loc_1_3C89
        ret    
loc_1_3C89:
        int3   
        ret    
loc_1_3C8B:
        mov    edi, edx
        test   dword ptr [edi + 4], 1
        jne    loc_1_3CB5
        test   dword ptr [edi + 4], 0x200000
        jne    loc_1_3CB5
        push   0
        mov    edx, esp
        push   edi
        push   edx
        call   sub_1_6360
        add    esp, 8
        pop    edx
        cmp    al, 0
        jne    loc_1_3C84
        mov    dword ptr [edi + 0x2c], edx
loc_1_3CB5:
        push   edi
        call   sub_1_0A34
        add    esp, 4
        mov    eax, dword ptr [edi + 4]
        and    dword ptr [edi + 4], 0xfffefffe
        mov    ebx, dword ptr [edi + 0x24]
        mov    dword ptr [edi + 0x24], 0
        mov    cx, word ptr [edi + 0x5e]
        mov    word ptr [ebx + 0xe], cx
        test   eax, 1
        jne    loc_1_3D28
        mov    eax, edi
        mov    edx, dword ptr [eax + 0x18]
        call   sub_1_3F7A
        mov    ecx, dword ptr [0x5854]       ; -> loc_1_5854
        or     ecx, ecx
        jne    loc_1_3D05
        mov    esi, 0x3e33                   ; -> loc_1_3E33
        mov    eax, 0x64
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
loc_1_3D05:
        inc    dword ptr [0x5854]            ; -> loc_1_5854
        mov    dx, word ptr [edi + 2]
        mov    esi, dword ptr [0x5868]       ; -> loc_1_5868
        mov    word ptr [esi + ecx*2], dx
        mov    esi, dword ptr [0x586c]       ; -> loc_1_586C
        mov    dword ptr [esi + ecx*4], edi
        mov    eax, dword ptr [edi + 0xc]
        inc    byte ptr [eax + 0x16]
loc_1_3D28:
        mov    eax, dword ptr [0x5854]       ; -> loc_1_5854
        cmp    word ptr [0x5940], ax         ; -> loc_1_5940
        jae    loc_1_3D3C
        mov    word ptr [0x5940], ax         ; -> loc_1_5940
loc_1_3D3C:
        mov    edx, dword ptr [ebx]
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [edx + 0x40], eax
        call   sub_1_4619
        ret    
sub_1_3D4A:
        test   dword ptr [edx + 4], 0x80000000
        jne    loc_1_3D81
        push   0
        push   edx
        test   dword ptr [edx + 4], 0x80000
        je     loc_1_3D79
        lea    edi, [edx + 0x3e]
        mov    eax, edx
        mov    byte ptr [eax + 0x5d], 0xff
        and    byte ptr [eax + 0x5c], 0x7f
        call   sub_1_441F
        mov    edx, dword ptr [eax + 0x18]
        call   sub_1_3F7A
loc_1_3D79:
        call   sub_1_0BD0
        add    esp, 8
loc_1_3D81:
        ret    
sub_1_3D82:
        push   ebx
        push   edi
loc_1_3D84:
        mov    eax, dword ptr [0x5890]       ; -> loc_1_5890
        or     eax, eax
        je     loc_1_3E13
        mov    edx, dword ptr [eax + 0x14]
        mov    dword ptr [0x5890], edx       ; -> loc_1_5890
        and    dword ptr [eax + 4], 0xfffffff7
        cmp    dword ptr [eax + 0x24], 0
        je     loc_1_3DE4
        test   dword ptr [eax + 4], 0xa1
        jne    loc_1_3DED
        or     dword ptr [eax + 4], 1
        and    dword ptr [eax + 4], 0xffffffbf
        mov    edx, dword ptr [eax + 0x30]
        mov    dword ptr [eax + 0x34], edx
        mov    edx, dword ptr [eax + 0x1c]
        mov    dword ptr [eax + 0x20], edx
        mov    edx, dword ptr [eax + 0x24]
        mov    edx, dword ptr [edx]
        call   sub_1_4417
        mov    cx, word ptr [eax + 0x62]
        mov    word ptr [edi + 0x24], cx
        add    ch, 1
        adc    cl, 0
        mov    word ptr [eax + 0x62], cx
        call   sub_1_3F7A
        jmp    loc_1_3D84
loc_1_3DE4:
        test   dword ptr [eax + 4], 0x80800
        jne    loc_1_3E16
loc_1_3DED:
        test   dword ptr [eax + 4], 0x800
        jne    loc_1_3E16
        and    dword ptr [eax + 4], 0xffffffbf
        lea    edi, [eax + 0x3e]
        mov    byte ptr [edi + 0x1e], 0x80
        call   sub_1_441F
        mov    edx, dword ptr [eax + 0x18]
        call   sub_1_3F7A
        jmp    loc_1_3D84
loc_1_3E13:
        pop    edi
        pop    ebx
        ret    
loc_1_3E16:
        push   esi
        mov    edx, eax
        call   sub_1_3D4A
        pop    esi
        jmp    loc_1_3D84
loc_1_3E24:
        and    dword ptr [0x58f8], 0xfffffffb ; -> loc_1_58F8
        ret    
loc_1_3E2C:
        call   sub_1_3D82
        jmp    loc_1_3E3D
loc_1_3E33:
        cld    
        cmp    dword ptr [0x5890], 0         ; -> loc_1_5890
        jne    loc_1_3E2C
loc_1_3E3D:
        mov    ecx, dword ptr [0x5854]       ; -> loc_1_5854
        jecxz  loc_1_3E24
        mov    esi, dword ptr [0x586c]       ; -> loc_1_586C
loc_1_3E4B:
        lodsd  eax, dword ptr [esi]
        dec    dword ptr [eax + 0x20]
        je     loc_1_3E70
        loop   loc_1_3E4B
loc_1_3E53:
        dec    dword ptr [0x58c0]            ; -> loc_1_58C0
        je     loc_1_3EFC
loc_1_3E5F:
        mov    esi, 0x3e33                   ; -> loc_1_3E33
        mov    eax, 0x64
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        ret    
loc_1_3E70:
        push   ecx
        dec    dword ptr [eax + 0x34]
        js     loc_1_3EB9
        test   dword ptr [eax + 4], 1
        jne    loc_1_3E99
        test   dword ptr [eax + 4], 0x80
        je     loc_1_3EB4
        inc    word ptr [eax + 0x6c]
        mov    edx, dword ptr [eax + 0x18]
        lea    edi, [eax + 0x3e]
        call   sub_1_441F
        jmp    loc_1_3EA7
loc_1_3E99:
        mov    edx, dword ptr [eax + 0x24]
        mov    edx, dword ptr [edx]
        call   sub_1_4417
        or     byte ptr [edi + 0x1e], 0x40
loc_1_3EA7:
        mov    ecx, dword ptr [eax + 0x1c]
        add    ecx, ecx
        mov    dword ptr [eax + 0x20], ecx
        call   sub_1_3F7A
loc_1_3EB4:
        pop    ecx
        loop   loc_1_3E4B
        jmp    loc_1_3E53
loc_1_3EB9:
        test   dword ptr [eax + 4], 1
        je     loc_1_3ECB
        inc    word ptr [0x5954]             ; -> loc_1_5954
        jmp    loc_1_3ED2
loc_1_3ECB:
        inc    word ptr [0x5968]             ; -> loc_1_5968
loc_1_3ED2:
        push   dword ptr [eax + 8]
        push   dword ptr [eax + 2]
        push   dword ptr [eax + 0xc]
        push   eax
        call   sub_1_0BD0
        pop    eax
        call   sub_1_0CF9
        add    esp, 0xc
        pop    ecx
        dec    ecx
        jne    loc_1_3E4B
        jmp    loc_1_3E53
loc_1_3EF7:
        jmp    loc_1_3E5F
loc_1_3EFC:
        mov    dword ptr [0x58c0], 0x37      ; -> loc_1_58C0
        mov    ecx, dword ptr [0x5854]       ; -> loc_1_5854
        jecxz  loc_1_3EF7
        mov    esi, dword ptr [0x586c]       ; -> loc_1_586C
loc_1_3F14:
        lodsd  eax, dword ptr [esi]
        test   dword ptr [eax + 4], 0x400
        je     loc_1_3F6F
        test   dword ptr [eax + 4], 0x81
        jne    loc_1_3F6F
        test   dword ptr [eax + 4], 2
        jne    loc_1_3F4A
        or     dword ptr [eax + 4], 0x80
        mov    dword ptr [eax + 0x34], 0xa
        mov    edx, dword ptr [eax + 0x1c]
        mov    dword ptr [eax + 0x20], edx
        mov    byte ptr [eax + 0x5c], 0xc0
        jmp    loc_1_3F57
loc_1_3F4A:
        test   dword ptr [eax + 4], 4
        jne    loc_1_3F6F
        mov    byte ptr [eax + 0x5c], 0x80
loc_1_3F57:
        push   ecx
        lea    edi, [eax + 0x3e]
        call   sub_1_441F
        and    dword ptr [eax + 4], 0xffffffbf
        mov    edx, dword ptr [eax + 0x18]
        push   eax
        call   sub_1_3F7A
        pop    eax
        pop    ecx
loc_1_3F6F:
        and    dword ptr [eax + 4], 0xfffffff9
        loop   loc_1_3F14
        jmp    loc_1_3E5F
sub_1_3F7A:
        or     dword ptr [eax + 4], 4
        test   byte ptr [edx + 0x3c], 4
        je     loc_1_3F89
        or     byte ptr [edx + 0x3c], 0x40
        ret    
loc_1_3F89:
        mov    eax, dword ptr [eax + 8]
        push   edx
        push   0
        mov    ecx, esp
        push   edx
        mov    eax, dword ptr [eax + 0x48]
        push   dword ptr [eax + 0xc]
        push   ecx
        or     byte ptr [edx + 0x3c], 4
        VxDCall  VPOWERD.Service_002Dh           ; cd202d002800
        pop    eax
        pop    edx
        cmp    eax, 0x103
        jne    loc_1_3FAD
        ret    
loc_1_3FAD:
        and    byte ptr [edx + 0x3c], 0xfb
        ret    
        db  000h, 000h
sub_1_3FB4:
        push   esi
        mov    esi, dword ptr [esp + 8]
        call   sub_1_3FF7
        pop    esi
        ret    
loc_1_3FC0:
        mov    eax, 1
        mov    byte ptr [esi + 9], al
        mov    byte ptr [esi + 8], 0
        ret    
loc_1_3FCD:
        mov    eax, 0xff
        mov    byte ptr [esi + 9], al
        mov    byte ptr [esi + 8], 0
        ret    
loc_1_3FDA:
        mov    eax, 0xfd
        mov    byte ptr [esi + 9], al
        mov    byte ptr [esi + 8], 0
        test   dword ptr [esi + 0xc], 0x80
        je     loc_1_3FF6
        inc    word ptr [0x595c]             ; -> loc_1_595C
loc_1_3FF6:
        ret    
sub_1_3FF7:
        push   dword ptr [esi + 0xa]
        call   sub_1_42A4
        pop    edx
        jb     loc_1_3FCD
        mov    edx, dword ptr [esi + 0x14]
        cmp    dword ptr [eax], edx
        jne    loc_1_3FCD
        inc    dword ptr [0x5936]            ; -> loc_1_5936
        push   eax
        or     dword ptr [esi + 0xc], 0x20
        push   esi
        call   sub_1_4488
        pop    edx
        pop    edx
        jb     loc_1_3FDA
        or     eax, eax
        je     loc_1_3FC0
        test   dword ptr [esi + 0xc], 0x80
        jne    loc_1_404B
        mov    ecx, dword ptr [edx + 0x20]
        mov    dword ptr [esi + 0x10], ecx
        mov    dword ptr [edx + 0x20], esi
        mov    eax, dword ptr [edx + 8]
        add    eax, dword ptr [0x5864]       ; -> loc_1_5864
        or     byte ptr [eax], 1
loc_1_4040:
        mov    byte ptr [esi + 9], 0
        mov    byte ptr [esi + 8], 0xfe
        xor    eax, eax
        ret    
loc_1_404B:
        inc    dword ptr [0x5958]            ; -> loc_1_5958
        inc    dword ptr [edx + 0x24]
        mov    ecx, dword ptr [edx + 0x28]
        mov    dword ptr [esi + 0x10], ecx
        mov    dword ptr [edx + 0x28], esi
        cmp    dword ptr [edx + 0x24], 1
        jne    loc_1_4040
        mov    ecx, dword ptr [edx + 0x2c]
loc_1_4066:
        jecxz  loc_1_4040
        test   dword ptr [ecx + 4], 0x10
        je     loc_1_408D
        and    dword ptr [ecx + 4], 0xffffffef
        test   dword ptr [ecx + 4], 8
        jne    loc_1_4089
        mov    edi, ecx
        xchg   dword ptr [0x5890], edi       ; -> loc_1_5890
        mov    dword ptr [ecx + 0x14], edi
loc_1_4089:
        or     dword ptr [ecx + 4], 0x48
loc_1_408D:
        mov    ecx, dword ptr [ecx + 0x10]
        jmp    loc_1_4066
sub_1_4092:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        mov    edi, dword ptr [esp + 0x10]
        call   sub_1_410D
        pop    edi
        pop    esi
        ret    
loc_1_40A4:
        push   edx
        call   sub_1_154B
        pop    edx
        ret    
sub_1_40AC:
        push   esi
        mov    eax, dword ptr [0x583c]       ; -> loc_1_583C
        mov    edx, dword ptr [esp + 8]
        mov    esi, 0x40a4                   ; -> loc_1_40A4
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        pop    esi
        ret    
loc_1_40C3:
        mov    byte ptr [esi + 9], 1
        mov    byte ptr [esi + 8], 0
        mov    al, 1
        ret    
loc_1_40CE:
        mov    byte ptr [esi + 9], 0xfd
        mov    byte ptr [esi + 8], 0
        mov    edx, eax
        call   sub_1_4619
        inc    word ptr [0x5920]             ; -> loc_1_5920
        mov    al, 0xfd
        ret    
loc_1_40E7:
        mov    edx, dword ptr [edi + 0x1c]
        mov    dword ptr [esi + 0x1c], edx
        mov    dx, word ptr [edi + 0x20]
        mov    word ptr [esi + 0x20], dx
        jmp    loc_1_41B1
loc_1_40FA:
        mov    byte ptr [esi + 9], 4
        mov    edx, eax
        mov    dword ptr [eax + 0x40], edi
        call   sub_1_4619
        mov    al, 4
        ret    
sub_1_410B:
        xor    edi, edi
sub_1_410D:
        or     dword ptr [esi + 0xc], 0x10
        mov    byte ptr [esi + 9], 0
        push   esi
        call   sub_1_4488
        pop    ecx
        jb     loc_1_40CE
        or     eax, eax
        je     loc_1_40C3
        cmp    word ptr [esi + 0x28], 0x1e
        jb     loc_1_40CE
        test   dword ptr [0x58f8], 0x8000    ; -> loc_1_58F8
        je     loc_1_414D
        mov    edx, dword ptr [0x57a0]       ; -> obj1:0x57A0
        cmp    edx, dword ptr [esi + 0x1c]
        jne    loc_1_414D
        mov    edx, 0x57a0                   ; -> obj1:0x57A0
        mov    edx, dword ptr [edx + 2]
        cmp    edx, dword ptr [esi + 0x1e]
        je     loc_1_40FA
loc_1_414D:
        or     edi, edi
        jne    loc_1_41A2
        mov    edi, dword ptr [0x58ec]       ; -> loc_1_58EC
        mov    ecx, dword ptr [0x58c8]       ; -> loc_1_58C8
        jecxz  loc_1_40FA
        cmp    cl, 1
        je     loc_1_41A2
        mov    edx, dword ptr [eax + 8]
        mov    edx, dword ptr [edx + 4]
        mov    edx, dword ptr [edx + 6]
        or     edx, edx
        je     loc_1_41A2
        inc    ecx
loc_1_4172:
        cmp    dword ptr [edi + 0x18], edx
        je     loc_1_41A2
        mov    edi, dword ptr [ecx*4 + 0x58d4] ; -> loc_1_58D4
        loop   loc_1_4172
        mov    edi, dword ptr [0x58ec]       ; -> loc_1_58EC
        test   dword ptr [0x58f8], 0x10      ; -> loc_1_58F8
        je     loc_1_41A2
        push   eax
        push   esi
        mov    eax, edx
        call   sub_1_2D10
        jb     loc_1_41A0
        mov    edi, dword ptr [esi + 0x20]
loc_1_41A0:
        pop    esi
        pop    eax
loc_1_41A2:
        mov    ecx, dword ptr [esi + 0x1c]
        movzx  edx, word ptr [esi + 0x20]
        or     edx, ecx
        je     loc_1_40E7
loc_1_41B1:
        mov    dx, word ptr [esi + 0xa]
        test   dword ptr [edi + 8], 1
        je     loc_1_40FA
        test   dword ptr [edi + 8], 0x10
        jne    loc_1_40FA
        mov    cl, 1
sub_1_41D1:
        mov    byte ptr [esi + 8], 0xff
        mov    dword ptr [eax + 0x40], edi
        push   ebx
        push   ecx
        push   edi
        mov    ebx, eax
        push   dword ptr [ebx + 4]
        lea    ecx, [ebx + 0x38]
        push   ecx
        lea    eax, [esi + 0x1c]
        push   eax
        call   dword ptr [edi + 0x28]
        add    esp, 8
        pop    edi
        push   edx
        mov    edx, dword ptr [0x58b8]       ; -> loc_1_58B8
        mov    ecx, dword ptr [edx]
        mov    dword ptr [0x58b8], ecx       ; -> loc_1_58B8
        mov    dword ptr [edx], 0
        dec    dword ptr [0x584c]            ; -> loc_1_584C
        lea    ecx, [ebx + 0x58]
        mov    dword ptr [edx + 4], ecx
        mov    ecx, dword ptr [ebx + 0x54]
        mov    dword ptr [edx + 0xc], ecx
        push   edi
        mov    edi, dword ptr [ebx + 8]
        push   edx
        push   ebx
        VxDCall  VPOWERD.Service_0023h           ; cd2023002800
        pop    ecx
        pop    edx
        xchg   dword ptr [esp], esi
        mov    eax, dword ptr [ebx + 4]
        cmp    dword ptr [esi + 0x10], eax
        jb     loc_1_4287
        mov    eax, dword ptr [edi + 4]
        xchg   cl, ch
        mov    word ptr [eax + 2], cx
        mov    byte ptr [eax + 4], 0
        mov    word ptr [eax + 0x1c], dx
        mov    esi, dword ptr [0x58ec]       ; -> loc_1_58EC
        add    esi, 0x18
        lea    edi, [eax + 0x12]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        pop    esi
        mov    word ptr [eax], 0xffff
        pop    ecx
        jecxz  loc_1_4283
        push   eax
        mov    edx, esp
        push   ebx
        mov    edi, dword ptr [ebx + 0x40]
        mov    edi, dword ptr [edi + 0x48]
        push   dword ptr [edi + 0xc]
        push   edx
        or     byte ptr [ebx + 0x3c], 4
        VxDCall  VPOWERD.Service_002Dh           ; cd202d002800
        inc    dword ptr [0x591c]            ; -> loc_1_591C
        pop    eax
        cmp    eax, 0x103
        jne    loc_1_4298
        pop    ebx
        xor    eax, eax
        ret    
loc_1_4283:
        pop    ebx
        xor    eax, eax
        ret    
loc_1_4287:
        pop    esi
        mov    byte ptr [esi + 9], 0xfd
        mov    edx, ebx
        call   sub_1_4619
        pop    ecx
        pop    ebx
        mov    al, 0xfd
        ret    
loc_1_4298:
        push   eax
        push   ebx
        push   edi
        call   sub_1_21B9
        pop    ebx
        xor    eax, eax
        ret    
sub_1_42A4:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [0x5840]       ; -> loc_1_5840
        mov    ecx, dword ptr [ecx]
        jecxz  loc_1_42CF
        push   edi
        mov    edi, dword ptr [0x585c]       ; -> loc_1_585C
        lea    edi, [edi + ecx*2 - 2]
        std    
        repne scasw ax, word ptr es:[edi]
        cld    
        pop    edi
        jne    loc_1_42CF
        mov    eax, dword ptr [0x5860]       ; -> loc_1_5860
        mov    eax, dword ptr [eax + ecx*4]
        clc    
        ret    
loc_1_42CF:
        xor    eax, eax
        stc    
        ret    
sub_1_42D3:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [0x5854]       ; -> loc_1_5854
        jecxz  loc_1_42FC
        push   edi
        mov    edi, dword ptr [0x5868]       ; -> loc_1_5868
        lea    edi, [edi + ecx*2 - 2]
        std    
        repne scasw ax, word ptr es:[edi]
        cld    
        pop    edi
        jne    loc_1_42FC
        mov    eax, dword ptr [0x586c]       ; -> loc_1_586C
        mov    eax, dword ptr [eax + ecx*4]
        clc    
        ret    
loc_1_42FC:
        xor    eax, eax
        stc    
        ret    
sub_1_4300:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [0x5840]       ; -> loc_1_5840
        mov    ecx, dword ptr [ecx]
        jecxz  loc_1_434C
        lea    edx, [ecx - 1]
        push   edi
        mov    edi, dword ptr [0x585c]       ; -> loc_1_585C
        lea    edi, [edi + ecx*2 - 2]
        std    
        repne scasw ax, word ptr es:[edi]
        cld    
        jne    loc_1_434B
        mov    edi, dword ptr [0x5860]       ; -> loc_1_5860
        push   dword ptr [edi + ecx*4]
        mov    eax, dword ptr [edi + edx*4]
        mov    dword ptr [edi + ecx*4], eax
        mov    edi, dword ptr [0x585c]       ; -> loc_1_585C
        mov    ax, word ptr [edi + edx*2]
        mov    word ptr [edi + ecx*2], ax
        mov    ecx, dword ptr [0x5840]       ; -> loc_1_5840
        dec    dword ptr [ecx]
        pop    eax
        pop    edi
        ret    
loc_1_434B:
        pop    edi
loc_1_434C:
        xor    eax, eax
        ret    
sub_1_434F:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [0x5854]       ; -> loc_1_5854
        jecxz  loc_1_4397
        lea    edx, [ecx - 1]
        push   edi
        mov    edi, dword ptr [0x5868]       ; -> loc_1_5868
        lea    edi, [edi + ecx*2 - 2]
        std    
        repne scasw ax, word ptr es:[edi]
        cld    
        jne    loc_1_4396
        mov    edi, dword ptr [0x586c]       ; -> loc_1_586C
        push   dword ptr [edi + ecx*4]
        mov    eax, dword ptr [edi + edx*4]
        mov    dword ptr [edi + ecx*4], eax
        mov    edi, dword ptr [0x5868]       ; -> loc_1_5868
        mov    ax, word ptr [edi + edx*2]
        mov    word ptr [edi + ecx*2], ax
        dec    dword ptr [0x5854]            ; -> loc_1_5854
        pop    eax
        pop    edi
        ret    
loc_1_4396:
        pop    edi
loc_1_4397:
        xor    eax, eax
        ret    
sub_1_439A:
        cld    
        push   edi
        mov    edx, dword ptr [0x5840]       ; -> loc_1_5840
        mov    edx, dword ptr [edx]
        xor    eax, eax
loc_1_43A6:
        mov    ax, word ptr [0x57a6]         ; -> obj1:0x57A6
        xchg   ah, al
        mov    ecx, edx
        add    word ptr [0x57a6], 2          ; -> obj1:0x57A6
        jb     loc_1_43C9
loc_1_43BA:
        jecxz  loc_1_43C7
        mov    edi, dword ptr [0x585c]       ; -> loc_1_585C
        repne scasw ax, word ptr es:[edi]
        je     loc_1_43A6
loc_1_43C7:
        pop    edi
        ret    
loc_1_43C9:
        mov    word ptr [0x57a6], 0x4000     ; -> obj1:0x57A6
        jmp    loc_1_43BA
sub_1_43D4:
        push   edi
loc_1_43D5:
        VMMCall  VMM.Get_System_Time             ; cd203f000100
        movzx  eax, al
        add    eax, dword ptr [0x58bc]       ; -> loc_1_58BC
        inc    eax
        inc    eax
        mov    dword ptr [0x58bc], eax       ; -> loc_1_58BC
        and    ax, 0xfffe
        mov    ecx, dword ptr [0x5840]       ; -> loc_1_5840
        mov    ecx, dword ptr [ecx]
loc_1_43F7:
        dec    ecx
        js     loc_1_4415
        mov    edi, dword ptr [0x5860]       ; -> loc_1_5860
        mov    edi, dword ptr [edi + ecx*4]
        mov    edi, dword ptr [edi + 0x2c]
loc_1_4406:
        or     edi, edi
        je     loc_1_43F7
        cmp    ax, word ptr [edi + 2]
        je     loc_1_43D5
        mov    edi, dword ptr [edi + 0x10]
        jmp    loc_1_4406
loc_1_4415:
        pop    edi
        ret    
sub_1_4417:
        mov    edi, dword ptr [edx + 8]
        mov    edi, dword ptr [edi]
        mov    edi, dword ptr [edi + 4]
sub_1_441F:
        mov    ebx, dword ptr [eax + 0xc]
        mov    ecx, dword ptr [ebx + 0x24]
        jecxz  loc_1_443B
loc_1_4427:
        mov    bx, word ptr [eax + 0x64]
        mov    word ptr [edi + 0x26], bx
        xchg   bh, bl
        add    ecx, ebx
        dec    ecx
        xchg   ch, cl
        mov    word ptr [edi + 0x28], cx
        ret    
loc_1_443B:
        or     dword ptr [eax + 4], 0x10
        jmp    loc_1_4427
loc_1_4441:
        push   ecx
        call   sub_1_1CDF
        pop    ecx
        cmp    dword ptr [0x584c], ecx       ; -> loc_1_584C
        jg     loc_1_44F5
        inc    dword ptr [0x5848]            ; -> loc_1_5848
        mov    esi, dword ptr [0x58b4]       ; -> loc_1_58B4
        mov    dword ptr [edi + 0x38], esi
        mov    dword ptr [0x58b4], edi       ; -> loc_1_58B4
loc_1_4469:
        pop    esi
        pop    edi
        pop    ebx
        xor    eax, eax
        ret    
loc_1_446F:
        push   ecx
        call   sub_1_1C7D
        pop    ecx
        cmp    dword ptr [0x5848], 0         ; -> loc_1_5848
        jg     loc_1_4498
        jmp    loc_1_4469
loc_1_4481:
        mov    eax, edi
        pop    esi
        pop    edi
        pop    ebx
        stc    
        ret    
sub_1_4488:
        push   ebx
        push   edi
        push   esi
        mov    ebx, dword ptr [esp + 0x10]
        cmp    dword ptr [0x5848], 3         ; -> loc_1_5848
        jl     loc_1_446F
loc_1_4498:
        dec    dword ptr [0x5848]            ; -> loc_1_5848
        mov    edi, dword ptr [0x58b4]       ; -> loc_1_58B4
        mov    esi, dword ptr [edi + 0x38]
        mov    dword ptr [0x58b4], esi       ; -> loc_1_58B4
        lea    esi, [edi + 0x4c]
        mov    eax, esi
        xchg   dword ptr [0x5894], eax       ; -> loc_1_5894
        mov    dword ptr [eax + 4], esi
        mov    dword ptr [esi], eax
        mov    dword ptr [esi + 4], 0x5894   ; -> loc_1_5894
        mov    edx, dword ptr [ebx]
        mov    dword ptr [edi + 0x48], edx
        mov    dword ptr [edi + 0x44], ebx
        mov    dword ptr [ebx], edi
        mov    dx, word ptr [ebx + 0xc]
        and    dl, 0x83
        mov    byte ptr [edi + 0x3c], dl
        test   word ptr [ebx + 0xc], 0x840
        jne    loc_1_4481
        movzx  ecx, word ptr [ebx + 0x22]
        jecxz  loc_1_4481
        lea    eax, [ecx + 0x14]
        cmp    dword ptr [0x584c], eax       ; -> loc_1_584C
        jbe    loc_1_4441
loc_1_44F5:
        push   edi
        sub    dword ptr [0x584c], ecx       ; -> loc_1_584C
        lea    esi, [ebx + 0x24]
        mov    eax, dword ptr [0x58b8]       ; -> loc_1_58B8
        push   ecx
        push   eax
loc_1_4506:
        mov    eax, dword ptr [eax]
        loop   loc_1_4506
        mov    dword ptr [0x58b8], eax       ; -> loc_1_58B8
        pop    eax
        pop    ecx
        test   dword ptr [ebx + 0xc], 3
        je     loc_1_45A6
        push   eax
        push   ecx
        mov    edi, eax
        mov    eax, ebx
        mov    ecx, 0x24
        call   sub_1_45CE
        mov    ecx, dword ptr [ebx + 0x14]
        mov    eax, edi
        test   dword ptr [ebx + 0xc], 1
        jne    loc_1_456C
        mov    ebx, dword ptr [ecx + 4]
loc_1_453F:
        mov    edi, eax
        mov    edx, dword ptr [esi]
        movzx  eax, dx
        xor    dh, dh
        shr    edx, 0xc
        add    eax, edx
        add    eax, ebx
        mov    dword ptr [edi + 4], eax
        movzx  ecx, word ptr [esi + 4]
        mov    dword ptr [edi + 0xc], ecx
        call   sub_1_45CE
        add    esi, 6
        mov    eax, dword ptr [edi]
        dec    dword ptr [esp]
        jne    loc_1_453F
        pop    ecx
        pop    edx
        jmp    loc_1_45BD
loc_1_456C:
        mov    ebx, ecx
loc_1_456E:
        mov    edi, eax
        mov    edx, dword ptr [esi]
        movzx  ecx, dx
        push   ecx
        shr    edx, 0x10
        push   0
        push   edx
        push   ebx
        VMMCall  VMM._SelectorMapFlat            ; cd206e000100
        add    esp, 0xc
        pop    ecx
        add    eax, ecx
        mov    dword ptr [edi + 4], eax
        mov    cx, word ptr [esi + 4]
        mov    dword ptr [edi + 0xc], ecx
        call   sub_1_45CE
        add    esi, 6
        mov    eax, dword ptr [edi]
        dec    dword ptr [esp]
        jne    loc_1_456E
        pop    ecx
        pop    edx
        jmp    loc_1_45BD
loc_1_45A6:
        mov    edx, eax
loc_1_45A8:
        mov    edi, eax
        mov    eax, dword ptr [esi]
        mov    dword ptr [edi + 4], eax
        movzx  eax, word ptr [esi + 4]
        mov    dword ptr [edi + 0xc], eax
        add    esi, 6
        mov    eax, dword ptr [edi]
        loop   loc_1_45A8
loc_1_45BD:
        mov    dword ptr [edi], ecx
        pop    edi
        push   edx
        push   edi
        VxDCall  VPOWERD.Service_0023h           ; cd2023002800
        mov    eax, edi
        pop    esi
        pop    edi
        pop    ebx
        ret    
sub_1_45CE:
        lea    ecx, [ecx + eax]
        shr    eax, 0xc
        shr    ecx, 0xc
        sub    ecx, eax
        inc    ecx
        push   0x100
        push   ecx
        push   eax
        VMMCall  VMM._LinPageLock                ; cd2063000100
        add    esp, 0xc
        ret    
sub_1_45EB:
        lea    ecx, [ecx + eax]
        shr    eax, 0xc
        shr    ecx, 0xc
        sub    ecx, eax
        inc    ecx
        push   0x100
        push   ecx
        push   eax
        VMMCall  VMM._LinPageUnLock              ; cd2064000100
        add    esp, 0xc
        ret    
sub_1_4608:
        push   esi
        push   ebx
        mov    ebx, dword ptr [esp + 0xc]
        jmp    loc_1_4677
loc_1_4610:
        and    byte ptr [edx + 0x3c], 0x7f
        ret    
sub_1_4615:
        mov    edx, dword ptr [esp + 4]
sub_1_4619:
        test   byte ptr [edx + 0x3c], 4
        jne    loc_1_4610
        push   esi
        push   ebx
        mov    eax, dword ptr [edx + 0x44]
        mov    ecx, dword ptr [edx + 0x48]
        push   edx
        or     eax, eax
        je     loc_1_4676
        mov    byte ptr [eax + 8], 0
        cmp    dword ptr [eax + 4], 0
        je     loc_1_466E
        mov    esi, dword ptr [edx + 0x40]
        mov    dl, 0xff
        mov    ebx, dword ptr [eax + 0x18]
        test   dword ptr [eax + 0xc], 0x400
        je     loc_1_464A
        mov    dword ptr [eax + 0x18], edi
loc_1_464A:
        call   ebx
        jmp    loc_1_4676
loc_1_464E:
        ror    dword ptr [eax + 0xc], 0x10
        test   word ptr [eax + 0xe], 0x400
        je     loc_1_4676
        mov    ecx, dword ptr [edi + 0x44]
        mov    dword ptr [eax + 0x10], ecx
        mov    ecx, dword ptr [edi + 0x48]
        mov    dword ptr [eax + 0x14], ecx
        mov    ecx, dword ptr [edi + 0x4c]
        mov    dword ptr [eax + 0x18], ecx
        jmp    loc_1_4676
loc_1_466E:
        test   word ptr [eax + 0xc], 0x500
        jne    loc_1_464E
loc_1_4676:
        pop    ebx
loc_1_4677:
        mov    eax, dword ptr [ebx + 0x4c]
        mov    ecx, dword ptr [ebx + 0x50]
        mov    dword ptr [ecx], eax
        mov    dword ptr [eax + 4], ecx
        mov    ecx, dword ptr [ebx + 0x14]
        jecxz  loc_1_46CF
        test   byte ptr [ebx + 0x3c], 3
        je     loc_1_46B2
        mov    eax, dword ptr [ebx + 0x44]
        mov    ecx, 0x24
        call   sub_1_45EB
        mov    esi, dword ptr [ebx + 8]
loc_1_469D:
        mov    eax, dword ptr [esi + 4]
        cmp    eax, esi
        je     loc_1_46AC
        mov    ecx, dword ptr [esi + 0xc]
        call   sub_1_45EB
loc_1_46AC:
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    loc_1_469D
loc_1_46B2:
        mov    edx, dword ptr [ebx + 8]
        mov    ecx, dword ptr [0x58b8]       ; -> loc_1_58B8
        mov    dword ptr [0x58b8], edx       ; -> loc_1_58B8
        mov    edx, dword ptr [ebx + 0xc]
        mov    dword ptr [edx], ecx
        mov    ecx, dword ptr [ebx + 0x14]
        add    dword ptr [0x584c], ecx       ; -> loc_1_584C
loc_1_46CF:
        push   ebx
        VxDCall  VPOWERD.Service_0019h           ; cd2019002800
        mov    eax, ebx
        xchg   dword ptr [0x58b4], eax       ; -> loc_1_58B4
        mov    dword ptr [ebx + 0x38], eax
        inc    dword ptr [0x5848]            ; -> loc_1_5848
        pop    ebx
        pop    esi
        ret    
sub_1_46EA:
        push   ebx
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 0xc]
        mov    ebx, dword ptr [esp + 0x10]
        mov    edx, ecx
        sub    edx, ebx
        mov    ecx, dword ptr [ecx]
loc_1_46FD:
        jecxz  loc_1_4710
        cmp    eax, ecx
        je     loc_1_470A
        mov    edx, ecx
        mov    ecx, dword ptr [ebx + ecx]
        jmp    loc_1_46FD
loc_1_470A:
        mov    ecx, dword ptr [ebx + ecx]
        mov    dword ptr [ebx + edx], ecx
loc_1_4710:
        pop    ebx
        ret    
loc_1_4712:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        mov    eax, 0x8137
        jmp    loc_1_476B
loc_1_471E:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        mov    eax, dword ptr [ebp + 0x10]
        test   al, 1
        je     loc_1_476B
        push   edx
        inc    eax
        push   eax
        dec    dword ptr [0x584c]            ; -> loc_1_584C
        mov    edi, dword ptr [0x58b8]       ; -> loc_1_58B8
        mov    esi, dword ptr [edi]
        mov    dword ptr [0x58b8], esi       ; -> loc_1_58B8
        mov    dword ptr [edi + 4], edi
        mov    dword ptr [edi + 0xc], 1
        mov    dword ptr [edi], 0
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        sub    esi, 0x38
        push   esi
        VxDCall  VPOWERD.Service_0024h           ; cd2024002800
        pop    eax
        pop    edx
        jmp    loc_1_476B
loc_1_4763:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        mov    eax, dword ptr [ebp + 0x10]
loc_1_476B:
        cld    
        mov    edi, dword ptr [ebp + 0xc]
        mov    dword ptr [edi + 0x1c], 0xe
        add    edi, 0x20
loc_1_4779:
        mov    esi, dword ptr [ebp + 8]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        mov    esi, dword ptr [ebp + 0x14]
        add    esi, 0x1c
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        xchg   ah, al
        mov    word ptr [edi], ax
        pop    esi
        pop    edi
        pop    ebp
        ret    
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        mov    eax, dword ptr [ebp + 0x10]
        add    eax, 8
        mov    edi, dword ptr [ebp + 0xc]
        mov    dword ptr [edi + 0x1c], 0x16
        add    edi, 0x20
        mov    word ptr [edi + 0xe], 0xe0e0
        mov    byte ptr [edi + 0x10], 3
        mov    dword ptr [edi + 0x11], 0
        mov    word ptr [edi + 0x14], 0x3781
        jmp    loc_1_4779
loc_1_47C2:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        mov    eax, dword ptr [ebp + 0x10]
        add    eax, 3
        mov    edi, dword ptr [ebp + 0xc]
        mov    dword ptr [edi + 0x1c], 0x11
        add    edi, 0x20
        mov    word ptr [edi + 0xe], 0xe0e0
        mov    byte ptr [edi + 0x10], 3
        jmp    loc_1_4779
loc_1_47E6:
        push   ebp
        mov    ebp, esp
        push   edx
        mov    edx, dword ptr [ebp + 0xc]
        mov    dword ptr [edx + 0x1c], 3
        mov    eax, dword ptr [ebp + 0x14]
        mov    al, byte ptr [ebx + 0x21]
        mov    byte ptr [edx + 0x20], al
        mov    eax, dword ptr [ebp + 8]
        mov    cl, byte ptr [eax + 5]
        cmp    dword ptr [eax], -1
        jne    loc_1_4811
        cmp    word ptr [eax + 4], -1
        jne    loc_1_4811
        xor    ecx, ecx
loc_1_4811:
        mov    byte ptr [edx + 0x21], cl
        mov    byte ptr [edx + 0x22], 0xfa
        pop    edx
        pop    ebp
        ret    
loc_1_481B:
        push   ebp
        mov    ebp, esp
        push   edx
        push   edi
        mov    edx, dword ptr [ebp + 0xc]
        mov    dword ptr [edx + 0x1c], 0x11
        call   sub_1_486C
        mov    word ptr [edi], 0xe0e0
        mov    byte ptr [edi + 2], 3
        pop    edi
        pop    edx
        pop    ebp
        ret    
loc_1_483C:
        push   ebp
        mov    ebp, esp
        push   edx
        push   edi
        mov    edx, dword ptr [ebp + 0xc]
        mov    dword ptr [edx + 0x1c], 0x16
        call   sub_1_486C
        mov    word ptr [edi], 0xaaaa
        mov    byte ptr [edi + 2], 3
        add    edi, 3
        mov    dword ptr [edi], 0
        mov    word ptr [edi + 3], 0x3781
        pop    edi
        pop    edx
        pop    ebp
        ret    
sub_1_486C:
        push   ebx
        push   esi
        cld    
        mov    edx, dword ptr [ebp + 0xc]
        lea    edi, [edx + 0x20]
        mov    esi, dword ptr [ebp + 8]
        mov    eax, 0x4010
        stosw  word ptr es:[edi], ax
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        mov    ebx, dword ptr [ebp + 0x14]
        lea    esi, [ebx + 0x1c]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        test   dword ptr [ebx + 8], 2
        je     loc_1_48D6
        mov    esi, dword ptr [ebp + 8]
        push   edx
        push   edi
        call   sub_1_232F
        jb     loc_1_48C6
        inc    dword ptr [0x587c]            ; -> loc_1_587C
        mov    eax, dword ptr [0x587c]       ; -> loc_1_587C
        stosd  dword ptr es:[edi], eax
        mov    esi, edi
        pop    edi
        pop    edx
        movzx  ecx, byte ptr [esi]
        and    cl, 0x1f
        jecxz  loc_1_48D6
        add    dword ptr [edx + 0x1c], ecx
        or     byte ptr [edi - 6], 0x80
        shr    ecx, 1
        rep movsw word ptr es:[edi], word ptr [esi]
        jmp    loc_1_48D6
loc_1_48C6:
        pop    edi
        pop    edx
        add    dword ptr [edx + 0x1c], 2
        or     byte ptr [edi - 6], 0x80
        mov    ax, 0x70c2
        stosw  word ptr es:[edi], ax
loc_1_48D6:
        pop    esi
        pop    ebx
        ret    
sub_1_48D9:
        mov    ebx, dword ptr [edx + 0x4c]
        mov    ecx, dword ptr [edx + 0x50]
        mov    dword ptr [ecx], ebx
        mov    dword ptr [ebx + 4], ecx
        xor    ecx, ecx
        xchg   dword ptr [edx + 0x54], ecx
        jecxz  loc_1_4911
        mov    eax, dword ptr [edx + 0x58]
        mov    ecx, dword ptr [edx + 0x5c]
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [edx + 0x44]
        mov    ecx, dword ptr [edx + 0x48]
        mov    byte ptr [eax + 9], 0
        mov    byte ptr [eax + 8], 0
        cmp    dword ptr [eax + 4], 0
        je     loc_1_4911
        push   edx
        call   sub_1_4971
        pop    edx
loc_1_4911:
        mov    ebx, edx
        xchg   dword ptr [0x58b4], ebx       ; -> loc_1_58B4
        mov    dword ptr [edx + 0x38], ebx
        inc    dword ptr [0x5848]            ; -> loc_1_5848
        ret    
loc_1_4923:
        push   ebp
        push   ebx
        push   edi
        push   esi
        test   dword ptr [eax + 0xc], 0x400
        jne    loc_1_4944
loc_1_4930:
        ror    dword ptr [eax + 0xc], 0x10
        push   ebx
        push   edx
        push   eax
        mov    esi, eax
        call   dword ptr [eax + 4]
        add    esp, 0xc
        pop    esi
        pop    edi
        pop    ebx
        pop    ebp
        ret    
loc_1_4944:
        mov    esi, dword ptr [eax + 0x18]
        mov    edi, dword ptr [esi + 0x44]
        mov    dword ptr [eax + 0x10], edi
        mov    edi, dword ptr [esi + 0x48]
        mov    dword ptr [eax + 0x14], edi
        mov    edi, dword ptr [esi + 0x4c]
        mov    dword ptr [eax + 0x18], edi
        jmp    loc_1_4930
        dd  loc_1_49DE               ; fixup
        dd  loc_1_49F5               ; fixup
loc_1_4963:
        mov    ebx, dword ptr [eax]
        mov    ebx, dword ptr [ebx + 0x40]
        mov    dword ptr [eax + 0xa], ecx
        mov    byte ptr [eax + 8], 0xaa
        jmp    loc_1_4993
sub_1_4971:
        push   ebx
        push   edi
        push   esi
        movzx  edi, word ptr [eax + 0xc]
        and    edi, 3
        mov    esi, dword ptr [edi*4 + 0x4957] ; -> obj1:0x4957
        test   word ptr [eax + 0xc], 0x800
        jne    loc_1_4963
        mov    ebx, ecx
        xchg   dword ptr [eax + 0x14], ebx
        mov    byte ptr [eax + 8], 0xfb
loc_1_4993:
        mov    edx, ebx
        add    edx, dword ptr [0x5820]       ; -> obj1:0x5820
        xor    ecx, ecx
        cmp    dword ptr [edx], ecx
        jne    loc_1_49C1
        cmp    dword ptr [edx + 4], ecx
        mov    dword ptr [edx], eax
        mov    dword ptr [edx + 4], eax
        mov    dword ptr [eax], ecx
        jne    loc_1_49BD
        VMMCall  VMM.Wake_Up_VM                  ; cd2039000100
        VMMCall  VMM.Test_Cur_VM_Handle          ; cd2002000100
        jne    loc_1_49CC
        call   esi
loc_1_49BD:
        pop    esi
        pop    edi
        pop    ebx
        ret    
loc_1_49C1:
        mov    ebx, eax
        xchg   dword ptr [edx + 4], ebx
        mov    dword ptr [ebx], eax
        mov    dword ptr [eax], ecx
        jmp    loc_1_49BD
loc_1_49CC:
        mov    ecx, 1
        mov    eax, 0x1000
        VMMCall  VMM.Call_Priority_VM_Event      ; cd2014000100
        jmp    loc_1_49BD
loc_1_49DE:
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        jmp    loc_1_4A0A
loc_1_49F5:
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
loc_1_4A0A:
        mov    ecx, dword ptr [edx]
        jecxz  loc_1_4A5F
        mov    eax, dword ptr [ecx]
        mov    dword ptr [edx], eax
        push   edx
        mov    eax, dword ptr [ebx + 8]
        cmp    byte ptr [ecx + 8], 0xaa
        je     loc_1_4A78
        mov    edx, dword ptr [ecx + 0x14]
        test   dword ptr [ecx + 0xc], 0x40
        jne    loc_1_4A7B
        mov    byte ptr [eax + 0x1c], 0xff
        test   word ptr [ecx + 0xc], 0x400
        jne    loc_1_4A81
loc_1_4A34:
        ror    dword ptr [ecx + 0xc], 0x10
        mov    byte ptr [ecx + 8], 0
        mov    word ptr [eax + 4], dx
        shr    edx, 0x10
        mov    word ptr [eax + 0x38], dx
        mov    ecx, dword ptr [ecx + 4]
        movzx  edx, cx
        shr    ecx, 0x10
        VMMCall  VMM.Simulate_Far_Call           ; cd2048000100
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        pop    edx
        jmp    loc_1_4A0A
loc_1_4A5F:
        mov    dword ptr [edx + 4], ecx
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        ret    
loc_1_4A78:
        mov    edx, dword ptr [ecx + 0xa]
loc_1_4A7B:
        mov    byte ptr [eax + 0x1c], 0
        jmp    loc_1_4A34
loc_1_4A81:
        mov    esi, dword ptr [ecx + 0x18]
        mov    edi, dword ptr [esi + 0x44]
        mov    dword ptr [ecx + 0x10], edi
        mov    edi, dword ptr [esi + 0x48]
        mov    dword ptr [ecx + 0x14], edi
        mov    edi, dword ptr [esi + 0x4c]
        mov    dword ptr [ecx + 0x18], edi
        jmp    loc_1_4A34
sub_1_4A98:
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        ret    
sub_1_4AAB:
        push   esi
        mov    eax, dword ptr [esp + 8]
        mov    esi, dword ptr [esp + 0xc]
        mov    edx, dword ptr [esp + 0x10]
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    eax, esi
        pop    esi
        ret    
sub_1_4AC2:
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    edx, dword ptr [esp + 0xc]
        VMMCall  VMM.Schedule_Global_Event       ; cd200e000100
        mov    eax, esi
        pop    esi
        ret    
sub_1_4AD5:
        push   esi
        mov    esi, dword ptr [esp + 8]
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
        pop    esi
        ret    
sub_1_4AE2:
        push   edi
        mov    edi, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 0xc]
        mov    ecx, dword ptr [esp + 0x10]
        rep stosb byte ptr es:[edi], al
        pop    edi
        ret    
sub_1_4AF3:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0xc]
        mov    esi, dword ptr [esp + 0x10]
        mov    ecx, dword ptr [esp + 0x14]
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    edi
        pop    esi
        ret    
        db  000h, 000h
NWLINK_Control:
        cmp    eax, 1
        je     loc_3_0371
        cmp    eax, 0x1b
        je     loc_3_0371
        cmp    eax, 7
        je     loc_1_4C5D
        cmp    eax, 0x24
        je     loc_1_4CB6
        cmp    eax, 0x23
        je     loc_1_4D5C
        cmp    eax, 0xb
        je     loc_1_4B42
        clc    
        ret    
NWLINK_Service_00:
        mov    ax, 0x1e03
        clc    
        ret    
loc_1_4B42:
        push   ebx
        mov    edx, ebx
        mov    ebx, 0
        call   sub_1_53F6
        pop    ebx
        clc    
        ret    
NWLINK_Service_03:
        mov    edi, dword ptr [esi + 0x3c]
        cmp    ebx, 3
        jne    loc_1_4B5F
        mov    edx, dword ptr [edi + 0x18]
        jmp    loc_1_4B8A
loc_1_4B5F:
        cmp    ebx, 5
        jne    loc_1_4B8D
        mov    edx, dword ptr [edi + 0x18]
        or     edx, edx
        jne    loc_1_4B8A
        mov    edx, dword ptr [0x58f0]       ; -> loc_1_58F0
        or     edx, edx
        jne    loc_1_4B80
        VMMCall  VMM.Get_System_Time             ; cd203f000100
        mov    dword ptr [0x58f0], eax       ; -> loc_1_58F0
loc_1_4B80:
        call   sub_1_1ADC
        mov    edx, eax
        mov    dword ptr [edi + 0x18], edx
loc_1_4B8A:
        xor    eax, eax
        ret    
loc_1_4B8D:
        cmp    ebx, 4
        jne    loc_1_4B9E
        or     edx, edx
        je     loc_1_4B9C
        mov    dword ptr [edi + 0x18], edx
        xor    eax, eax
        ret    
loc_1_4B9C:
        jmp    loc_1_4BC6
loc_1_4B9E:
        cmp    ebx, 2
        jne    loc_1_4BB7
        and    dword ptr [0x58f8], 0xffffffbf ; -> loc_1_58F8
        or     dword ptr [0x58f8], 0x80      ; -> loc_1_58F8
        xor    eax, eax
        ret    
loc_1_4BB7:
        cmp    ebx, 1
        jne    loc_1_4BC6
        or     dword ptr [0x58f8], 0x40      ; -> loc_1_58F8
        xor    eax, eax
        ret    
loc_1_4BC6:
        xor    eax, eax
        dec    eax
        ret    
NWLINK_Service_04:
        or     ebx, ebx
        je     loc_1_4C1B
        cmp    ebx, 2
        ja     loc_1_4C1B
        test   dword ptr [0x58f8], 0x10      ; -> loc_1_58F8
        jne    loc_1_4BE9
        mov    eax, dword ptr [0x58ec]       ; -> loc_1_58EC
        mov    eax, dword ptr [eax + 0x34]
        clc    
        ret    
loc_1_4BE9:
        push   ebx
        push   esi
        push   edi
        cmp    ebx, 1
        jne    loc_1_4C03
        mov    eax, edx
        call   sub_1_2D10
        jb     loc_1_4C59
        mov    eax, dword ptr [esi + 0x20]
        mov    eax, dword ptr [eax + 0x34]
        clc    
        jmp    loc_1_4C59
loc_1_4C03:
        push   edx
        and    edx, 0xffff
        call   sub_1_42D3
        pop    edx
        jb     loc_1_4C59
        mov    eax, dword ptr [eax + 8]
        mov    eax, dword ptr [eax + 0x34]
        clc    
        jmp    loc_1_4C59
loc_1_4C1B:
        push   ebx
        push   esi
        push   edi
        cmp    ebx, 3
        jne    loc_1_4C58
        mov    ecx, dword ptr [0x5834]       ; -> loc_1_5834
        mov    esi, 0x58cc                   ; -> loc_1_58CC
        sub    esi, 4
loc_1_4C31:
        dec    ecx
        js     loc_1_4C53
        add    esi, 4
        mov    edi, dword ptr [esi]
        mov    eax, dword ptr [edi + 4]
        and    eax, 1
        je     loc_1_4C31
        mov    eax, dword ptr [edi + 0x3c]
        or     eax, eax
        je     loc_1_4C31
        mov    eax, dword ptr [eax + 0x34]
        sub    eax, edx
        jne    loc_1_4C31
        not    eax
        jmp    loc_1_4C59
loc_1_4C53:
        clc    
        xor    eax, eax
        jmp    loc_1_4C59
loc_1_4C58:
        stc    
loc_1_4C59:
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_4C5D:
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        mov    byte ptr [ebp + 0x1d], 0x34
        mov    eax, 0x21
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        movzx  edx, word ptr [ebp + 0x38]
        shl    edx, 4
        movzx  ecx, word ptr [ebp + 0x10]
        add    edx, ecx
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        add    ebx, dword ptr [0x5820]       ; -> obj1:0x5820
        mov    dword ptr [ebx], 0
        add    edx, 0xf
        mov    dword ptr [ebx + 8], edx
        ret    
loc_1_4CB6:
        test   dword ptr [0x58f8], 1         ; -> loc_1_58F8
        je     loc_1_4D5B
        and    dword ptr [0x58f8], 0xfffffffe ; -> loc_1_58F8
        and    dword ptr [0x58f8], 0xffffdfff ; -> loc_1_58F8
        cmp    dword ptr [0x5670], 0         ; -> obj1:0x5670
        je     loc_1_4CE5
        call   sub_1_1C65
loc_1_4CE5:
        cmp    dword ptr [0x5674], 0         ; -> obj1:0x5674
        je     loc_1_4CF3
        call   sub_1_1CC7
loc_1_4CF3:
        mov    ecx, dword ptr [0x5a4a]       ; -> loc_1_5A4A
        jecxz  loc_1_4D05
        push   ecx
        VxDCall  Dev_0048.Service_0002h          ; cd2002004800
        add    esp, 4
loc_1_4D05:
        xor    edx, edx
        call   sub_1_53F6
        push   ebp
        push   eax
        mov    ebp, esp
        mov    ebx, dword ptr [0x58c4]       ; -> loc_1_58C4
        or     ebx, ebx
        je     loc_1_4D54
loc_1_4D1A:
        mov    edi, dword ptr [ebx*4 + 0x58c8] ; -> loc_1_58C8
        test   dword ptr [edi + 4], 1
        je     loc_1_4D51
        and    dword ptr [edi + 4], 0xfffffffe
        push   dword ptr [edi + 0xc]
        push   ebp
        VxDCall  VPOWERD.Service_002Ch           ; cd202c002800
        cmp    dword ptr [ebp], 0x103
        jne    loc_1_4D51
loc_1_4D41:
        test   dword ptr [edi + 4], 0x10000000
        jne    loc_1_4D51
        call   sub_1_4A98
        jmp    loc_1_4D41
loc_1_4D51:
        dec    ebx
        jne    loc_1_4D1A
loc_1_4D54:
        call   sub_1_1163
        pop    eax
        pop    ebp
loc_1_4D5B:
        ret    
loc_1_4D5C:
        mov    ebx, dword ptr [esi + 0xc]
        or     ebx, ebx
        je     loc_1_4DD1
        cmp    ebx, -1
        je     loc_1_4DD6
        test   dword ptr [0x58f8], 1         ; -> loc_1_58F8
        je     loc_1_4DD9
        cmp    ebx, 0x100
        je     loc_1_4DB6
        cmp    ebx, 0x101
        je     loc_1_4D92
        cmp    ebx, 0x102
        je     loc_1_4DA4
        mov    eax, 1
        ret    
loc_1_4D92:
        cmp    dword ptr [esi + 0x14], 0x40
        jb     loc_1_4DCB
        push   dword ptr [esi + 0x10]
        call   sub_1_75B3
        add    esp, 4
        ret    
loc_1_4DA4:
        cmp    dword ptr [esi + 0x14], 0x40
        jb     loc_1_4DCB
        push   dword ptr [esi + 0x10]
        call   sub_1_770D
        add    esp, 4
        ret    
loc_1_4DB6:
        mov    ebx, dword ptr [esi + 0x14]
        cmp    ebx, 4
        jb     loc_1_4DCB
        mov    ebx, dword ptr [esi + 0x10]
        mov    ebx, dword ptr [ebx]
        jmp    dword ptr [ebx*4 + 0x5adc]    ; -> obj1:0x5ADC
        db  0C3h
loc_1_4DCB:
        mov    eax, 1
        ret    
loc_1_4DD1:
        mov    ecx, 0x1e03
loc_1_4DD6:
        xor    eax, eax
        ret    
loc_1_4DD9:
        mov    eax, 2
        ret    
loc_1_4DDF:
        cmp    dword ptr [esi + 0x1c], 0xa
        jb     loc_1_4DFF
        mov    edi, dword ptr [esi + 0x18]
        xchg   esi, edi
        call   sub_1_519F
        xchg   esi, edi
        mov    ebx, dword ptr [esi + 0x20]
        or     eax, eax
        jne    loc_1_4E04
        mov    dword ptr [ebx], 0xa
        ret    
loc_1_4DFF:
        mov    eax, 1
loc_1_4E04:
        mov    dword ptr [ebx], 0
        ret    
NWLINK_Service_01:
        cmp    ebx, 0x1b
        jae    loc_1_4E97
        test   dword ptr [0x58f8], 1         ; -> loc_1_58F8
        je     loc_1_4E45
loc_1_4E20:
        test   byte ptr [ebx + 0x577c], 2    ; -> obj1:0x577C
        je     loc_1_4E3E
        mov    dword ptr [esi + 0xc], 4
        mov    dword ptr [esi + 0x18], 0x4923 ; -> loc_1_4923
        mov    dword ptr [esi + 0x14], 0
loc_1_4E3E:
        jmp    dword ptr [ebx*4 + 0x5a50]    ; -> obj1:0x5A50
loc_1_4E45:
        test   dword ptr [0x58f8], 0x2000    ; -> loc_1_58F8
        je     loc_1_4E83
        test   byte ptr [ebx + 0x577c], 4    ; -> obj1:0x577C
        jne    loc_1_4E20
        test   dword ptr [0x58f8], 0x800     ; -> loc_1_58F8
        je     loc_1_4E83
loc_1_4E66:
        call   sub_1_4A98
        test   dword ptr [0x58f8], 0x800     ; -> loc_1_58F8
        jne    loc_1_4E66
        test   dword ptr [0x58f8], 1         ; -> loc_1_58F8
        jne    loc_1_4E20
loc_1_4E83:
        test   byte ptr [ebx + 0x577c], 2    ; -> obj1:0x577C
        je     loc_1_4E94
        mov    byte ptr [esi + 9], 4
        mov    byte ptr [esi + 8], 0
loc_1_4E94:
        mov    al, 2
        ret    
loc_1_4E97:
        cmp    ebx, 0x101
        je     loc_1_4EA2
        mov    al, 1
        ret    
loc_1_4EA2:
        test   dword ptr [0x58f8], 1         ; -> loc_1_58F8
        je     loc_1_4EB1
        mov    al, 2
        ret    
loc_1_4EB1:
        push   ebx
        call   sub_1_4F84
        jb     loc_1_4EC2
        push   edi
        push   esi
        call   sub_1_644C
        pop    esi
        pop    edi
loc_1_4EC2:
        pop    ebx
        ret    
sub_1_4EC4:
        movzx  ecx, word ptr [0xc]           ; -> obj2:0x000C
        add    ecx, dword ptr [4]            ; -> loc_2_0004
        movzx  ebx, word ptr [0xe]           ; -> loc_2_000E
        add    ebx, dword ptr [4]            ; -> loc_2_0004
        VxDCall  Dev_0048.Service_0000h          ; cd2000004800
        or     eax, eax
        je     loc_1_4EFD
        push   0
        push   ebx
        push   ecx
        push   0
        push   0
        push   esp
        VxDCall  Dev_0048.Service_0001h          ; cd2001004800
        add    esp, 0x18
loc_1_4EFD:
        mov    dword ptr [0x5a4a], eax       ; -> loc_1_5A4A
        or     eax, eax
        jne    loc_1_4F07
        ret    
loc_1_4F07:
        mov    ebx, 0x596a                   ; -> loc_1_596A
        mov    ecx, 8
loc_1_4F11:
        push   ecx
        push   ebx
        push   dword ptr [ebx + 0x18]
        mov    edx, dword ptr [ebx + 8]
        movzx  eax, word ptr [edx*2 + 0xa]   ; -> obj2:0x000A
        add    eax, dword ptr [4]            ; -> loc_2_0004
        push   eax
        mov    edx, dword ptr [ebx + 0xc]
        movzx  eax, word ptr [edx*2 + 0xa]   ; -> obj2:0x000A
        add    eax, dword ptr [4]            ; -> loc_2_0004
        push   eax
        mov    edx, dword ptr [ebx]
        movzx  eax, word ptr [edx*2 + 0xa]   ; -> obj2:0x000A
        add    eax, dword ptr [4]            ; -> loc_2_0004
        push   eax
        mov    edx, dword ptr [ebx + 4]
        movzx  eax, word ptr [edx*2 + 0xa]   ; -> obj2:0x000A
        add    eax, dword ptr [4]            ; -> loc_2_0004
        push   eax
        push   dword ptr [ebx + 0x14]
        push   0
        push   esp
        push   dword ptr [0x5a4a]            ; -> loc_1_5A4A
        VxDCall  Dev_0048.Service_0003h          ; cd2003004800
        add    esp, 0x24
        pop    ebx
        pop    ecx
        mov    dword ptr [ebx + 0x10], eax
        add    ebx, 0x1c
        dec    cx
        jecxz  loc_1_4F80
        jmp    loc_1_4F11
loc_1_4F80:
        ret    
loc_1_4F81:
        xor    eax, eax
        ret    
sub_1_4F84:
        cmp    dword ptr [0x58c8], 0         ; -> loc_1_58C8
        je     loc_1_4F94
        mov    edi, dword ptr [0x58ec]       ; -> loc_1_58EC
        ret    
loc_1_4F94:
        mov    eax, 4
        stc    
        ret    
sub_1_4F9B:
        push   ebp
        push   edx
        mov    ebp, esp
        push   0
        push   0
        push   0
        push   ebp
        call   sub_1_02E0
        add    esp, 0x10
        pop    edx
        pop    ebp
        ret    
loc_1_4FB1:
        push   edx
        mov    ebx, eax
        add    eax, dword ptr [0x5820]       ; -> obj1:0x5820
        mov    eax, dword ptr [eax + 8]
        movzx  eax, word ptr [eax]
        mov    ecx, dword ptr [ebx + 0xc]
        add    eax, ecx
        shl    ecx, 0x10
        add    eax, ecx
        mov    ecx, esp
        push   0
        push   eax
        push   ebx
        push   ecx
        call   sub_1_02E0
        add    esp, 0x10
        pop    edx
        ret    
loc_1_4FDB:
        push   0
        push   edx
        call   sub_1_041B
        add    esp, 8
        ret    
loc_1_4FE7:
        push   esi
        push   edi
        mov    ebx, edi
        call   sub_1_4F84
        jb     loc_1_5028
        push   ebp
        push   0
        mov    ebp, esp
        push   edi
        push   ebp
        push   esi
        push   ebx
        call   sub_1_61D0
        add    esp, 0x10
        or     eax, eax
        jne    loc_1_501D
        test   dword ptr [edi + 8], 2
        je     loc_1_5024
        mov    esi, ebx
        mov    ebx, edi
        call   sub_1_2365
        xor    eax, eax
        jmp    loc_1_5024
loc_1_501D:
        inc    word ptr [0x593c]             ; -> loc_1_593C
loc_1_5024:
        pop    ecx
        xchg   cl, ch
        pop    ebp
loc_1_5028:
        pop    edi
        pop    esi
        ret    
loc_1_502B:
        push   edi
        call   sub_1_410B
        pop    edi
        ret    
loc_1_5033:
        push   esi
        call   sub_1_3FF7
        pop    esi
        ret    
loc_1_503B:
        mov    byte ptr [esi + 9], 0xf0
        mov    byte ptr [esi + 8], 0
        ret    
loc_1_5044:
        mov    byte ptr [esi + 9], 1
        mov    byte ptr [esi + 8], 0
        ret    
loc_1_504D:
        mov    byte ptr [esi + 9], 0xff
        mov    byte ptr [esi + 8], 0
        ret    
loc_1_5056:
        or     dword ptr [esi + 0xc], 0x40
        cmp    byte ptr [esi + 8], 0
        jne    loc_1_50BB
loc_1_5060:
        mov    ebx, eax
        push   dword ptr [esi + 0xa]
        call   sub_1_42A4
        pop    edx
        jb     loc_1_504D
        cmp    dword ptr [eax], ebx
        jne    loc_1_504D
        mov    edi, eax
        push   esi
        call   sub_1_4488
        pop    edx
        or     eax, eax
        je     loc_1_5044
        lea    edi, [edi + 0xc]
        lea    edx, [eax + 0x58]
loc_1_5084:
        mov    ecx, edx
        xchg   dword ptr [edi], ecx
        mov    dword ptr [ecx + 4], edx
        mov    dword ptr [edx], ecx
        mov    dword ptr [edx + 4], edi
        mov    byte ptr [esi + 8], 0xfd
        mov    edi, eax
loc_1_5096:
        movzx  eax, word ptr [ebp + 0x1c]
        or     eax, eax
        je     loc_1_50B8
        imul   eax, eax, 0x37
loc_1_50A1:
        mov    edx, edi
        mov    esi, 0x48d9                   ; -> sub_1_48D9
        VMMCall  VMM.Set_VM_Time_Out             ; cd203d000100
        mov    dword ptr [edi + 0x54], esi
        inc    dword ptr [0x592a]            ; -> loc_1_592A
        ret    
loc_1_50B8:
        inc    eax
        jmp    loc_1_50A1
loc_1_50BB:
        push   eax
        push   ecx
        push   dword ptr [esi + 0xa]
        call   sub_1_42A4
        pop    edi
        mov    edi, eax
        pop    ecx
        pop    eax
        jb     loc_1_504D
        lea    edx, [edi + 0xc]
        mov    ebx, edx
loc_1_50D1:
        mov    ebx, dword ptr [ebx]
        cmp    edx, ebx
        je     loc_1_5060
        cmp    dword ptr [ebx - 0x14], esi
        je     loc_1_50DE
        jmp    loc_1_50D1
loc_1_50DE:
        lea    edi, [ebx - 0x58]
        mov    esi, dword ptr [edi + 0x54]
        mov    ebx, eax
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
        jmp    loc_1_5096
loc_1_50EE:
        or     dword ptr [esi + 0xc], 0x840
        cmp    byte ptr [esi + 8], 0
        jne    loc_1_511C
loc_1_50FB:
        mov    ebx, eax
        push   esi
        call   sub_1_4488
        pop    edx
        or     eax, eax
        je     loc_1_5044
        mov    dword ptr [eax + 0x40], ebx
        mov    edi, 0x589c                   ; -> loc_1_589C
        lea    edx, [eax + 0x58]
        jmp    loc_1_5084
loc_1_511C:
        mov    edx, 0x589c                   ; -> loc_1_589C
        mov    ebx, edx
loc_1_5123:
        mov    ebx, dword ptr [ebx]
        cmp    edx, ebx
        je     loc_1_50FB
        cmp    dword ptr [ebx - 0x14], esi
        je     loc_1_50DE
        jmp    loc_1_5123
sub_1_5130:
        push   ebx
        push   esi
        mov    ebx, dword ptr [esp + 0xc]
        mov    ecx, dword ptr [ebx]
        mov    edx, dword ptr [ebx + 4]
        mov    dword ptr [ecx + 4], edx
        mov    dword ptr [edx], ecx
        sub    ebx, 0x20
        xor    esi, esi
        xchg   dword ptr [ebx + 0x1c], esi
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
        xor    esi, esi
        xchg   dword ptr [ebx + 0xc], esi
        mov    byte ptr [esi + 9], 0xfc
        mov    byte ptr [esi + 8], 0
        mov    edx, dword ptr [esi]
        call   sub_1_48D9
        pop    esi
        pop    ebx
        ret    
loc_1_5165:
        mov    eax, 0x5894                   ; -> loc_1_5894
        mov    ecx, eax
loc_1_516C:
        mov    ecx, dword ptr [ecx]
        cmp    ecx, eax
        je     loc_1_5187
        cmp    dword ptr [ecx - 8], esi
        je     loc_1_5179
        jmp    loc_1_516C
loc_1_5179:
        push   0
        push   0
        push   esi
        call   sub_1_0562
        add    esp, 0xc
        ret    
loc_1_5187:
        mov    eax, 0xff
        ret    
sub_1_518D:
        push   ebx
        VMMCall  VMM.Get_System_Time             ; cd203f000100
        xor    edx, edx
        mov    ebx, 0x37
        div    ebx
        pop    ebx
        ret    
sub_1_519F:
        push   esi
        push   edi
        call   sub_1_4F84
        jb     loc_1_51BC
        mov    edx, edi
        mov    edi, esi
        lea    esi, [edx + 0x18]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        movzx  ecx, word ptr [edx + 0x24]
        pop    edi
        pop    esi
        xor    eax, eax
        ret    
loc_1_51BC:
        xor    eax, eax
        mov    edi, esi
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        pop    edi
        pop    esi
        inc    eax
        ret    
loc_1_51C8:
        xor    eax, eax
        ret    
sub_1_51CB:
        push   edi
        call   sub_1_4F84
        jb     loc_1_51E6
        mov    edi, dword ptr [edi + 0x48]
        mov    eax, dword ptr [edi + 0x1c]
        sub    eax, 0x40
        mov    edx, 0xa
        mov    ecx, dword ptr [edi + 0x20]
        pop    edi
        ret    
loc_1_51E6:
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        pop    edi
        ret    
loc_1_51EE:
        push   edi
        call   sub_1_4F84
        jb     loc_1_51E6
        mov    eax, dword ptr [edi + 0xc]
        mov    ecx, 0xa
        pop    edi
        ret    
loc_1_5200:
        call   sub_1_51CB
        ret    
sub_1_5206:
        push   esi
        mov    edx, dword ptr [esp + 8]
        movzx  edx, word ptr [edx]
        mov    esi, dword ptr [esp + 0xc]
        mov    eax, dword ptr [esp + 0x10]
        call   sub_1_5224
        mov    esi, dword ptr [esp + 8]
        mov    word ptr [esi], dx
        pop    esi
        ret    
sub_1_5224:
        push   eax
        call   sub_1_4F9B
        or     eax, eax
        jne    loc_1_5250
        push   edx
        call   sub_1_42A4
        add    esp, 4
        or     dword ptr [eax + 0x18], 8
        mov    dword ptr [eax + 0x30], esi
        pop    dword ptr [eax + 0x34]
        mov    eax, dword ptr [eax + 8]
        add    eax, dword ptr [0x5864]       ; -> loc_1_5864
        or     byte ptr [eax], 2
        xor    eax, eax
        ret    
loc_1_5250:
        pop    edx
        ret    
loc_1_5252:
        xor    eax, eax
        cmp    dword ptr [0x58c8], eax       ; -> loc_1_58C8
        je     loc_1_5282
        mov    al, 0xff
        mov    word ptr [ebp + 0x10], 0x30a
        jmp    loc_1_5276
loc_1_5266:
        xor    eax, eax
        cmp    dword ptr [0x58c8], eax       ; -> loc_1_58C8
        je     loc_1_5282
        mov    al, 0xff
        mov    bx, 0x30a
loc_1_5276:
        mov    ecx, dword ptr [0x5828]       ; -> loc_1_5828
        mov    edx, dword ptr [0x5858]       ; -> loc_1_5858
loc_1_5282:
        ret    
loc_1_5283:
        mov    byte ptr [esi + 9], al
        mov    byte ptr [esi + 8], 0
        ret    
loc_1_528B:
        call   sub_1_4F84
        jb     loc_1_5283
        mov    dword ptr [esi + 0x10], edi
        mov    edx, dword ptr [esi + 0x24]
        movzx  edi, dx
        test   dword ptr [esi + 0xc], 2
        je     loc_1_52B0
        xor    dh, dh
        shr    edx, 0xc
        add    edx, dword ptr [eax + 4]
        add    edi, edx
        jmp    loc_1_52C2
loc_1_52B0:
        shr    edx, 0x10
        push   0
        push   edx
        push   eax
        VMMCall  VMM._SelectorMapFlat            ; cd206e000100
        add    esp, 0xc
        add    edi, eax
loc_1_52C2:
        mov    eax, dword ptr [ebp + 0x1c]
loc_1_52C5:
        push   0
        mov    edx, esp
        push   edi
        movzx  ecx, ah
        movzx  eax, al
        push   edx
        push   ecx
        push   eax
        push   esi
        call   sub_1_0652
        add    esp, 0x14
        pop    edx
        ret    
loc_1_52DE:
        push   eax
        call   sub_1_4F84
        pop    eax
        jb     loc_1_5283
        mov    dword ptr [esi + 0x10], edi
        mov    edi, dword ptr [esi + 0x24]
        jmp    loc_1_52C5
loc_1_52EF:
        mov    eax, dword ptr [ebp + 0x1c]
loc_1_52F2:
        or     dword ptr [esi + 0xc], 0x500
        movzx  ecx, ah
        movzx  eax, al
        push   ecx
        push   eax
        push   esi
        call   sub_1_091C
        add    esp, 0xc
        ret    
loc_1_530B:
        or     dword ptr [esi + 0xc], 0x80
        push   edx
        call   sub_1_42D3
        pop    edx
        jb     loc_1_532C
        test   dword ptr [eax + 4], 0x40000
        jne    loc_1_532C
        call   sub_1_38AA
        xor    eax, eax
        ret    
loc_1_532C:
        mov    byte ptr [esi + 9], 0xee
        mov    byte ptr [esi + 8], 0
        cmp    dword ptr [esi + 4], 0
        je     loc_1_5343
        mov    eax, esi
        mov    ecx, dword ptr [eax]
        mov    dl, 0xff
        call   dword ptr [eax + 0x18]
loc_1_5343:
        xor    eax, eax
        ret    
sub_1_5346:
        inc    word ptr [0x5956]             ; -> loc_1_5956
        push   edx
        call   sub_1_42D3
        pop    edx
        jb     loc_1_5369
        or     dword ptr [eax + 4], 0x400000
        push   1
        push   eax
        call   sub_1_0BD0
        add    esp, 8
        ret    
loc_1_5369:
        push   edx
        call   sub_1_42D3
        pop    edx
        jb     loc_1_53B7
        mov    ebx, eax
        mov    edi, esi
        mov    edx, esi
        lea    esi, [ebx]
        movsd  dword ptr es:[edi], dword ptr [esi]
        lea    esi, [ebx + 0x60]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    eax, dword ptr [ebx + 0x68]
        stosd  dword ptr es:[edi], eax
        mov    ax, word ptr [ebx + 0x5a]
        stosw  word ptr es:[edi], ax
        lea    esi, [ebx + 0x38]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        lea    esi, [ebx + 0x44]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    eax, dword ptr [ebx + 0x30]
        xchg   ah, al
        stosw  word ptr es:[edi], ax
        mov    eax, dword ptr [ebx + 0x1c]
        xchg   ah, al
        stosw  word ptr es:[edi], ax
        mov    ax, word ptr [ebx + 0x6e]
        xchg   ah, al
        shl    eax, 0x10
        mov    ax, word ptr [ebx + 0x6c]
        xchg   ah, al
        stosd  dword ptr es:[edi], eax
        xor    eax, eax
        ret    
loc_1_53B7:
        mov    eax, 0xee
        ret    
loc_1_53BD:
        or     dword ptr [esi + 0xc], 0x80
        call   sub_1_3FF7
        ret    
loc_1_53CA:
        cmp    dword ptr [esp + 0xc], -1
        jne    loc_1_53F0
        mov    eax, dword ptr [esp + 8]
        cmp    word ptr [eax + 0x1c], 0x111d
        jne    loc_1_53F0
        mov    edx, dword ptr [esp + 4]
        mov    edx, dword ptr [edx + 8]
        push   ebx
        mov    ebx, 4
        call   sub_1_53F6
        pop    ebx
loc_1_53F0:
        jmp    loc_1_5884
sub_1_53F6:
        push   esi
        push   edi
        mov    esi, dword ptr [0x5860]       ; -> loc_1_5860
        mov    edi, dword ptr [0x5840]       ; -> loc_1_5840
        mov    edi, dword ptr [edi]
        inc    edi
loc_1_5407:
        dec    edi
        je     loc_1_5449
        lodsd  eax, dword ptr [esi]
        or     edx, edx
        jne    loc_1_541A
        test   dword ptr [eax + 0x18], 1
        jne    loc_1_5435
        jmp    loc_1_541F
loc_1_541A:
        cmp    dword ptr [ebx + eax], edx
        jne    loc_1_5407
loc_1_541F:
        mov    cx, word ptr [eax + 0x14]
        push   edx
        push   0
        push   ecx
        call   sub_1_041B
        add    esp, 8
        pop    edx
        sub    esi, 4
        jmp    loc_1_5407
loc_1_5435:
        push   edx
        mov    cx, word ptr [eax + 0x14]
        push   ecx
        call   sub_1_4300
        add    esp, 4
        pop    edx
        sub    esi, 4
        jmp    loc_1_5407
loc_1_5449:
        pop    edi
        pop    esi
        ret    
sub_1_544C:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    ebx, dword ptr [ebx + 8]
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebx + 0x10], eax
        mov    esi, dword ptr [ebp + 0xc]
        mov    ax, word ptr [esi]
        mov    dword ptr [ebx + 0x14], eax
        mov    ecx, dword ptr [0x5914]       ; -> loc_1_5914
        movzx  edx, cx
        shr    ecx, 0x10
        VMMCall  VMM.Simulate_Far_Call           ; cd2048000100
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        mov    eax, dword ptr [ebx + 0x14]
        mov    word ptr [esi], ax
        movzx  eax, word ptr [ebx + 0x1c]
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        pop    esi
        pop    ebx
        pop    ebp
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_5508:
        xor    al, 0x5e
        add    byte ptr [eax], al
        push   ecx
        pop    esi
        add    byte ptr [eax], al
        jno    loc_1_5570
        add    byte ptr [eax], al
        xchg   ecx, eax
        pop    esi
        add    byte ptr [eax], al
        pop    edi
        add    dword ptr [eax], eax
        add    byte ptr [ecx + 0x200005f], bh ; -> loc_1_5FB9
        pushal 
        add    byte ptr [eax], al
        sbb    dword ptr [eax], esp
        add    byte ptr [edi], cl
        add    byte ptr [edi], cl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebp + edx*2], dl    ; -> loc_1_5554
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebp + 0x55], dh     ; -> obj1:0x5575
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        xchg   esi, eax
        push   ebp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        mov    bh, 0x55
        add    byte ptr [eax], al
loc_1_5554:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5570:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5596:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_55D8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_55DC:
        add    byte ptr [eax], 0
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x56], dh     ; -> obj1:0x5670
        add    byte ptr [eax], al
        je     loc_1_56D4
        add    byte ptr [eax], al
loc_1_5680:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5684:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5688:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_568C:
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
loc_1_5690:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5694:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5698:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_569C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_56A0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_56A4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_56A8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_56AC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_56B0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_56B4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_56B8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_56BC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_56C0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_56C4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_56C8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_56CC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_56D0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_56D4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_56D8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_56E0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_56E4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_56E8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_56EC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_56F0:
        mov    cl, 0x4f
        add    byte ptr [eax], al
        fisttp dword ptr [edi]
        add    bh, ah                        ; -> loc_1_4FE7
        dec    edi
        add    byte ptr [eax], al
        or     eax, dword ptr [ecx]
        add    bh, dh                        ; -> sub_1_3FF7
        aas    
        add    byte ptr [eax], al
        push   esi
        push   eax
        add    byte ptr [eax], al
        push   ecx
        add    byte ptr [eax], al
        out    dx, al
        push   eax
        add    byte ptr [eax], al
        lea    edx, [ecx]
        add    byte ptr [edi - 0x67ffffaf], bl ; -> sub_1_519F
        dec    edx
        add    byte ptr [eax], al
        enter  0x51, 0
        or     dword ptr [edi], 0x520000     ; -> loc_1_5200
        add    byte ptr [ecx + 0xb00004f], al ; -> loc_1_4F81
        inc    ecx
        add    byte ptr [eax], al
        push   edx
        push   edx
        add    byte ptr [eax], al
        mov    edx, dword ptr [edx]
        add    bh, ch                        ; -> loc_1_52EF
        push   edx
        add    byte ptr [eax], al
        or     edx, dword ptr [ebx]
        add    byte ptr [esi + 0x53], al     ; -> sub_1_5346
        add    byte ptr [eax], al
        imul   edx, dword ptr [ebx], 0x391b00 ; -> sub_1_391B
        add    byte ptr [ebp - 0x7effffad], bh ; -> loc_1_53BD
        dec    edi
        add    byte ptr [eax], al
        or     dword ptr [edi], 0x367000     ; -> loc_1_3670
        add    byte ptr [ecx - 0x7effffb1], al ; -> loc_1_4F81
        dec    edi
        add    byte ptr [eax], al
        or     dword ptr [edi], 0x4f8100     ; -> loc_1_4F81
        add    byte ptr [ecx - 0x7effffb1], al ; -> loc_1_4F81
        dec    edi
        add    byte ptr [eax], al
        or     dword ptr [edi], 0x4f8100     ; -> loc_1_4F81
        add    byte ptr [esp + eax], al
        add    dword ptr [ebx], eax
        pop    es
        add    eax, dword ptr [0x4010403]
        add    dword ptr [eax], eax
        add    al, 4
        add    eax, dword ptr [eax]
        add    eax, dword ptr [ebx]
        add    eax, dword ptr [eax]
        add    dword ptr [ebx], eax
        add    eax, dword ptr [eax]
        add    byte ptr [eax + eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 040h
        dd  NWLINK_Service_00        ; fixup
        dd  NWLINK_Service_01        ; fixup
        dd  NWLINK_Service_02        ; fixup
        dd  NWLINK_Service_03        ; fixup
        dd  NWLINK_Service_04        ; fixup
        dd  NWLINK_Service_05        ; fixup
        dd  NWLINK_Service_06        ; fixup
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00487h                      ; DDB_Req_Device_Number
        db  1, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'NWLINK  '                  ; DDB_Name
        dd  054005FF6h                  ; DDB_Init_Order
        dd  NWLINK_Control           ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  obj1:0x57A8              ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_5824:
        and    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5828:
        adc    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_582C:
        adc    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5830:
        inc    eax
        add    byte ptr [eax], al
        db  000h
loc_1_5834:
        add    al, 0
        add    byte ptr [eax], al
loc_1_5838:
        add    al, 0
        add    byte ptr [eax], al
loc_1_583C:
        fadd   qword ptr [eax]
        add    byte ptr [eax], al
loc_1_5840:
        push   eax
        pop    eax
        add    byte ptr [eax], al
loc_1_5844:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5848:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_584C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5850:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5854:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5858:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_585C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5860:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5864:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5868:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_586C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5870:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5874:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5878:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_587C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5884:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5888:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_588C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5890:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5894:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_589C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_58A4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_58A8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_58AC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_58B0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_58B4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_58B8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_58BC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_58C0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_58C4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_58C8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_58CC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_58D4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_58DC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_58EC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_58F0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_58F4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_58F8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_58FC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5900:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5904:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5908:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_590C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5910:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5914:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5918:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_591C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5920:
        add    byte ptr [eax], al
loc_1_5922:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5926:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_592A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5930:
        add    byte ptr [eax], al
loc_1_5932:
        add    byte ptr [eax], al
loc_1_5934:
        add    byte ptr [eax], al
loc_1_5936:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_593A:
        add    byte ptr [eax], al
loc_1_593C:
        add    byte ptr [eax], al
loc_1_593E:
        add    byte ptr [eax], al
loc_1_5940:
        add    byte ptr [eax], al
loc_1_5942:
        add    byte ptr [eax], al
loc_1_5944:
        add    byte ptr [eax], al
loc_1_5946:
        add    byte ptr [eax], al
loc_1_5948:
        add    byte ptr [eax], al
loc_1_594A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_594E:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5952:
        add    byte ptr [eax], al
loc_1_5954:
        add    byte ptr [eax], al
loc_1_5956:
        add    byte ptr [eax], al
loc_1_5958:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_595C:
        add    byte ptr [eax], al
loc_1_595E:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_5962:
        add    byte ptr [eax], al
loc_1_5964:
        add    byte ptr [eax], al
loc_1_5966:
        add    byte ptr [eax], al
loc_1_5968:
        add    byte ptr [eax], al
loc_1_596A:
        push   es
        add    byte ptr [eax], al
        add    byte ptr [edi], al
        add    byte ptr [eax], al
        add    byte ptr [eax], cl
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        sbb    al, 0x59
        add    byte ptr [eax], al
        or     dword ptr [eax], eax
        add    byte ptr [eax], al
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
        or     eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        and    bl, byte ptr [ecx]
        add    byte ptr [eax + eax], cl
        add    byte ptr [eax], al
        or     eax, 0xe000000
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        pop    ecx
        add    byte ptr [eax], al
        sldt   word ptr [eax]
        add    byte ptr [eax], dl
        add    byte ptr [eax], al
        add    byte ptr [ecx], dl
        add    byte ptr [eax], al
        add    byte ptr [ebx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x58], dl     ; -> loc_1_5850
        add    byte ptr [eax], al
        adc    al, byte ptr [eax]
        add    byte ptr [eax], al
        adc    eax, dword ptr [eax]
        add    byte ptr [eax], al
        adc    al, 0
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        dec    edx
        pop    ecx
        add    byte ptr [eax], al
        adc    eax, 0x16000000
        add    byte ptr [eax], al
        add    byte ptr [edi], dl
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        pop    esi
        pop    ecx
        add    byte ptr [eax], al
        sbb    byte ptr [eax], al
        add    byte ptr [eax], al
        sbb    dword ptr [eax], eax
        add    byte ptr [eax], al
        sbb    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        mov    eax, 0x1b000056
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], bl
        add    byte ptr [eax], al
        sbb    eax, 0x3000000
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, cl                        ; -> loc_1_56C8
        push   esi
        add    byte ptr [eax], al
loc_1_5A4A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        ; ---- dispatch table (70 entries) ----
        dd  sub_1_4F9B
        dd  loc_1_4FDB
        dd  loc_1_4FE7
        dd  loc_1_502B
        dd  loc_1_5033
        dd  loc_1_503B
        dd  loc_1_5165
        dd  loc_1_503B
        dd  sub_1_518D
        dd  sub_1_519F
        dd  sub_1_4A98
        dd  loc_1_51C8
        dd  loc_1_4F81
        dd  sub_1_51CB
        dd  sub_1_5224
        dd  loc_1_4F81
        dd  loc_1_5266
        dd  loc_1_52DE
        dd  loc_1_52F2
        dd  loc_1_530B
        dd  sub_1_5346
        dd  loc_1_5369
        dd  sub_1_391B
        dd  loc_1_53BD
        dd  loc_1_4F81
        dd  loc_1_4F81
        dd  loc_1_51EE
        dd  loc_1_4F81
        dd  loc_1_4F81
        dd  loc_1_4F81
        dd  loc_1_4F81
        dd  loc_1_4F81
        dd  loc_1_4F81
        dd  loc_1_4F81
        dd  loc_1_4F81
        dd  loc_1_4F81
        dd  loc_1_4F81
        dd  loc_1_4F81
        dd  loc_1_4F81
        dd  loc_1_4F81
        dd  loc_1_4F81
        dd  loc_1_4F81
        dd  loc_1_4F81
        dd  loc_1_4F81
        dd  loc_1_4DDF
        dd  loc_1_4F81
        dd  loc_1_4F81
        dd  loc_1_4F81
        dd  loc_1_4F81
        dd  loc_1_4F81
        dd  loc_1_4F81
        dd  loc_1_4F81
        dd  loc_1_4F81
        dd  loc_1_4F81
        dd  loc_1_4F81
        dd  loc_1_4F81
        dd  loc_1_4F81
        dd  loc_1_4F81
        dd  loc_1_4F81
        dd  loc_1_4F81
        dd  loc_1_4F81
        dd  loc_1_4F81
        dd  loc_1_4F81
        dd  loc_1_4F81
        dd  loc_1_4F81
        dd  loc_1_4F81
        dd  loc_1_4F81
        dd  loc_1_4F81
        dd  loc_1_4F81
        dd  loc_1_4F81
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_5B70:
        mov    ecx, dword ptr [0x7c40]       ; -> obj1:0x7C40
        mov    eax, dword ptr [0x7c44]       ; -> obj1:0x7C44
        mov    word ptr [ecx + 0xa], ax
        mov    eax, dword ptr [0x7c40]       ; -> obj1:0x7C40
        mov    dword ptr [eax + 0xc], 0x100
        mov    eax, dword ptr [0x7c40]       ; -> obj1:0x7C40
        push   1
        mov    dword ptr [eax + 4], 1
        mov    eax, dword ptr [0x7c40]       ; -> obj1:0x7C40
        push   0
        mov    dword ptr [eax + 0x18], 0x72  ; -> loc_1_0072
        mov    eax, dword ptr [0x7c40]       ; -> obj1:0x7C40
        mov    word ptr [eax + 0x22], 0
        push   dword ptr [0x7c40]            ; -> obj1:0x7C40
        call   sub_1_091C
        mov    eax, dword ptr [0x7c40]       ; -> obj1:0x7C40
        add    esp, 0xc
        cmp    byte ptr [eax + 8], 0
        je     loc_1_5BD2
        or     dword ptr [0x7c48], 2         ; -> obj1:0x7C48
loc_1_5BD2:
        ret    
sub_1_5BD3:
        sub    esp, 4
        push   ebx
        push   esi
        mov    ebx, 1
        test   byte ptr [0x7c48], 6          ; -> obj1:0x7C48
        jne    loc_1_5BEB
        call   sub_1_5B70
loc_1_5BEB:
        push   -1
        push   0
        lea    eax, [esp + 0x10]
        push   0x58
        push   eax
        call   sub_1_2468
        cmp    dword ptr [esp + 8], 0
        je     loc_1_5CD3
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 8]
        add    esi, 0x2a
        mov    word ptr [eax + 0xa], 0x5604
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax + 0xc], 0
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax + 4], 1
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax + 0x18], 0x2b8 ; -> loc_1_02B8
        mov    eax, dword ptr [esp + 8]
        push   6
        mov    word ptr [eax + 0x22], 1
        mov    eax, dword ptr [esp + 0xc]
        mov    word ptr [eax + 0x28], 0x24
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [eax + 0x24], esi
        mov    eax, dword ptr [esp + 0x1c]
        add    eax, 0x1c
        push   eax
        mov    eax, dword ptr [esp + 0x10]
        add    eax, 0x1c
        push   eax
        call   sub_1_4AF3
        add    esp, 0xc
        mov    eax, dword ptr [esp + 0x14]
        push   0xc
        add    eax, 0x12
        push   eax
        lea    eax, [esi + 6]
        push   eax
        call   sub_1_4AF3
        add    esp, 0xc
        mov    byte ptr [esi + 5], 0
        mov    byte ptr [esi + 0x1e], 1
        mov    byte ptr [esi + 0x1f], 1
        mov    eax, dword ptr [0x7c44]       ; -> obj1:0x7C44
        sub    ecx, ecx
        mov    word ptr [esi + 0x20], ax
        mov    byte ptr [esi + 0x23], 0
        sub    edx, edx
loc_1_5CA0:
        mov    eax, ecx
        inc    ecx
        cmp    dword ptr [eax*4 + 0x54c8], edx ; -> obj1:0x54C8
        je     loc_1_5CB1
        mov    byte ptr [esi + ebx + 0x23], cl
        inc    ebx
loc_1_5CB1:
        cmp    ecx, 0x10
        jb     loc_1_5CA0
        mov    byte ptr [esi + 0x22], bl
        mov    eax, dword ptr [esp + 8]
        push   dword ptr [esp + 0x10]
        dec    bx
        add    word ptr [eax + 0x28], bx
        push   dword ptr [esp + 0xc]
        call   sub_1_4092
        add    esp, 8
loc_1_5CD3:
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_1_5CD9:
        or     dword ptr [0x7c48], 4         ; -> obj1:0x7C48
        and    dword ptr [0x7c48], 0xfffffffd ; -> obj1:0x7C48
        push   -1
        push   0
        push   0x2f8
        push   0x7c3c                        ; -> obj1:0x7C3C
        call   sub_1_2468
        cmp    dword ptr [0x7c3c], 0         ; -> obj1:0x7C3C
        jne    loc_1_5D0F
        and    dword ptr [0x7c48], 0xfffffffb ; -> obj1:0x7C48
        jmp    loc_1_5DC6
loc_1_5D0F:
        mov    eax, dword ptr [0x7c3c]       ; -> obj1:0x7C3C
        mov    dword ptr [eax], 2
        mov    ecx, dword ptr [0x7c3c]       ; -> obj1:0x7C3C
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [0x7c3c]       ; -> obj1:0x7C3C
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [eax + 0x2d4], 0
        mov    edx, dword ptr [0x7c3c]       ; -> obj1:0x7C3C
        mov    eax, dword ptr [ecx + 0xc]
        mov    dword ptr [edx + 8], eax
        mov    eax, dword ptr [0x7c3c]       ; -> obj1:0x7C3C
        shr    dword ptr [eax + 8], 0x10
        mov    edx, dword ptr [0x7c3c]       ; -> obj1:0x7C3C
        mov    eax, dword ptr [0x7c44]       ; -> obj1:0x7C44
        mov    word ptr [edx + 0x1a], ax
        mov    eax, dword ptr [0x7c3c]       ; -> obj1:0x7C3C
        mov    byte ptr [eax + 0x18], 0
        mov    eax, dword ptr [0x7c3c]       ; -> obj1:0x7C3C
        mov    dword ptr [eax + 0x14], 0
        mov    eax, dword ptr [0x7c3c]       ; -> obj1:0x7C3C
        mov    word ptr [eax + 0x32], 1
        mov    eax, dword ptr [0x7c3c]       ; -> obj1:0x7C3C
        mov    edx, dword ptr [0x7c3c]       ; -> obj1:0x7C3C
        add    eax, 0x3a
        mov    dword ptr [edx + 0x34], eax
        mov    dword ptr [ecx + 0x18], 0x7d  ; -> loc_1_007D
        mov    word ptr [ecx + 0x22], 1
        mov    word ptr [ecx + 0x28], 0x70
        mov    eax, dword ptr [0x7c3c]       ; -> obj1:0x7C3C
        add    eax, 0x264
        mov    dword ptr [ecx + 0x24], eax
        mov    dword ptr [ecx + 0xc], 0x80
        mov    dword ptr [ecx + 0x14], 0
        push   ecx
        call   sub_1_3FB4
        add    esp, 4
loc_1_5DC6:
        ret    
sub_1_5DC7:
        push   esi
        mov    esi, dword ptr [0x7c3c]       ; -> obj1:0x7C3C
        cmp    byte ptr [esi + 0x18], 0
        je     loc_1_5DDF
        push   dword ptr [esi + 8]
        call   sub_1_014E
        add    esp, 4
loc_1_5DDF:
        mov    eax, dword ptr [esi]
        test   al, 4
        je     loc_1_5DEC
        or     eax, 0x30
        mov    dword ptr [esi], eax
        jmp    loc_1_5E32
loc_1_5DEC:
        test   al, 8
        je     loc_1_5E0A
        push   0
        mov    ax, word ptr [0x54c0]         ; -> obj1:0x54C0
        push   eax
        call   sub_1_041B
        add    esp, 8
        mov    word ptr [0x54c0], 0          ; -> obj1:0x54C0
loc_1_5E0A:
        push   0
        push   0x2f8
        push   dword ptr [0x7c3c]            ; -> obj1:0x7C3C
        call   sub_1_246E
        mov    dword ptr [0x7c3c], 0         ; -> obj1:0x7C3C
        and    dword ptr [0x7c48], 0xfffffffb ; -> obj1:0x7C48
        call   sub_1_5B70
loc_1_5E32:
        pop    esi
        ret    
loc_1_5E34:
        mov    eax, dword ptr [esp + 8]
        mov    byte ptr [eax], 0
        mov    byte ptr [eax + 5], 3
        mov    byte ptr [eax + 6], 0xa
        mov    byte ptr [eax + 7], 3
        mov    byte ptr [eax + 8], 0xa
        mov    eax, 9
        ret    
loc_1_5E51:
        mov    eax, dword ptr [esp + 8]
        push   0x22
        mov    byte ptr [eax], 0
        push   0x591c                        ; -> loc_1_591C
        add    eax, 5
        push   eax
        call   sub_1_4AF3
        add    esp, 0xc
        mov    eax, 0x27
        ret    
loc_1_5E71:
        mov    eax, dword ptr [esp + 8]
        push   0x2c
        mov    byte ptr [eax], 0
        push   0x593e                        ; -> loc_1_593E
        add    eax, 5
        push   eax
        call   sub_1_4AF3
        add    esp, 0xc
        mov    eax, 0x31
        ret    
loc_1_5E91:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 0x20]
        mov    esi, dword ptr [esp + 0x18]
        movzx  eax, word ptr [edi + 0x14]
        or     eax, eax
        mov    dword ptr [esi + 0x2d8], eax
        jne    loc_1_5EC5
        mov    eax, dword ptr [esp + 0x1c]
        mov    byte ptr [eax], 0
        mov    word ptr [eax + 5], 0
        mov    eax, 7
        jmp    loc_1_5FB1
loc_1_5EC5:
        push   -1
        push   0
        lea    eax, [esp + 0x18]
        push   0x22e
        push   eax
        call   sub_1_2468
        cmp    dword ptr [esp + 0x10], 0
        jne    loc_1_5EF0
        mov    eax, dword ptr [esp + 0x1c]
        mov    byte ptr [eax], 0xff
        mov    eax, 7
        jmp    loc_1_5FB1
loc_1_5EF0:
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax], esi
        mov    ebx, dword ptr [esp + 0x10]
        mov    dword ptr [ebx + 8], 1
        mov    dword ptr [ebx + 0x10], 0
        mov    dword ptr [ebx + 0x1c], 0x184 ; -> loc_1_0184
        add    ebx, 4
        mov    eax, dword ptr [0x7c44]       ; -> obj1:0x7C44
        mov    word ptr [ebx + 0xa], ax
        mov    word ptr [ebx + 0x22], 1
        lea    ebp, [ebx + 0x2a]
        mov    dword ptr [ebx + 0x24], ebp
        mov    ax, word ptr [edi + 0x18]
        mov    word ptr [ebx + 0x28], ax
        push   0xc
        lea    eax, [edi + 2]
        push   eax
        lea    eax, [ebp + 6]
        push   eax
        call   sub_1_4AF3
        add    esp, 0xc
        push   6
        lea    eax, [edi + 0xe]
        push   eax
        lea    eax, [ebx + 0x1c]
        push   eax
        call   sub_1_4AF3
        add    esp, 0xc
        mov    byte ptr [ebp + 5], 0
        or     dword ptr [esi], 4
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [esi + 0x2d4], eax
        movzx  eax, byte ptr [edi + 0x17]
        mov    dword ptr [esi + 0x2e0], eax
        movzx  eax, byte ptr [edi + 0x17]
        mov    dword ptr [esi + 0x2dc], eax
        movzx  eax, word ptr [edi + 0x18]
        mov    dword ptr [esi + 0x2e4], eax
        movsx  eax, word ptr [edi + 0x1a]
        mov    dword ptr [esi + 0x2e8], eax
        push   dword ptr [esi + 4]
        movzx  eax, byte ptr [edi + 0x16]
        imul   eax, eax, 0x37
        mov    dword ptr [esi + 0x2ec], eax
        mov    dword ptr [esi + 0x2f0], 0
        push   ebx
        call   sub_1_4092
        add    esp, 8
        sub    eax, eax
loc_1_5FB1:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_1_5FB9:
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        mov    byte ptr [esi], 0
        mov    dword ptr [0x54c4], 0         ; -> obj1:0x54C4
        test   byte ptr [0x7c48], 8          ; -> obj1:0x7C48
        jne    loc_1_5FF1
        push   1
        push   0
        push   0x2af                         ; -> loc_1_02AF
        push   0x54c0                        ; -> obj1:0x54C0
        call   sub_1_5206
        add    esp, 0x10
        or     dword ptr [0x7c48], 8         ; -> obj1:0x7C48
loc_1_5FF1:
        mov    ax, word ptr [0x54c0]         ; -> obj1:0x54C0
        mov    word ptr [esi + 5], ax
        mov    eax, 7
        pop    esi
        ret    
loc_1_6002:
        mov    eax, dword ptr [esp + 8]
        mov    byte ptr [eax], 0
        mov    ecx, dword ptr [0x54c4]       ; -> obj1:0x54C4
        mov    word ptr [eax + 5], cx
        mov    eax, 7
        ret    
loc_1_6019:
        mov    eax, dword ptr [esp + 8]
        mov    byte ptr [eax], 0xff
        mov    eax, 5
        ret    
loc_1_6026:
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0xc]
        movzx  eax, byte ptr [eax + 1]
        or     eax, eax
        je     loc_1_6047
        cmp    eax, 1
        je     loc_1_6098
        mov    edx, dword ptr [esp + 0x10]
        mov    eax, 5
        mov    byte ptr [edx], 0xff
        jmp    loc_1_60AD
loc_1_6047:
        mov    edx, dword ptr [esp + 0x10]
        mov    edi, edx
        sub    eax, eax
        mov    ecx, 0x38
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    esi, dword ptr [0x58ec]       ; -> loc_1_58EC
        lea    edi, [edx + 9]
        add    esi, 0x1c
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        mov    eax, dword ptr [0x58ec]       ; -> loc_1_58EC
        mov    esi, 0x7c4c                   ; -> obj1:0x7C4C
        mov    eax, dword ptr [eax + 0x10]
        sub    ax, 0x40
        mov    word ptr [edx + 0x11], ax
        mov    eax, dword ptr [0x58ec]       ; -> loc_1_58EC
        lea    edi, [edx + 0x26]
        mov    ax, word ptr [eax + 0x24]
        mov    word ptr [edx + 0x15], ax
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    eax, 0xe2
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
        jmp    loc_1_60AD
loc_1_6098:
        mov    edi, dword ptr [esp + 0x10]
        sub    eax, eax
        mov    ecx, 0xa
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        stosb  byte ptr es:[edi], al
        mov    eax, 0x2b
loc_1_60AD:
        pop    edi
        pop    esi
        ret    
sub_1_60B0:
        sub    esp, 4
        push   esi
        mov    word ptr [esp + 6], 0x5604
        push   -1
        push   0
        push   0x133
        push   0x7c40                        ; -> obj1:0x7C40
        call   sub_1_2468
        cmp    dword ptr [0x7c40], 0         ; -> obj1:0x7C40
        je     loc_1_61C0
        mov    esi, dword ptr [0x7c40]       ; -> obj1:0x7C40
        mov    dword ptr [esi + 0x2e], 1
        mov    dword ptr [esi + 0x42], 0x2c  ; -> loc_1_002C
        mov    word ptr [esi + 0x34], 0x5604
        mov    word ptr [esi + 0x4c], 1
        mov    word ptr [esi + 0x52], 0xdf
        lea    eax, [esi + 0x54]
        mov    dword ptr [esi + 0x4e], eax
        add    esi, 0x2a
        push   0x22
        push   0
        push   0x591c                        ; -> loc_1_591C
        call   sub_1_4AE2
        add    esp, 0xc
        push   0x2c
        push   0
        push   0x593e                        ; -> loc_1_593E
        call   sub_1_4AE2
        mov    eax, dword ptr [0x5824]       ; -> loc_1_5824
        mov    word ptr [0x5930], ax         ; -> loc_1_5930
        mov    eax, dword ptr [0x5828]       ; -> loc_1_5828
        mov    word ptr [0x593e], ax         ; -> loc_1_593E
        add    esp, 0xc
        push   1
        push   0
        lea    eax, [esp + 0xe]
        push   0
        push   eax
        call   sub_1_02E0
        add    esp, 0x10
        or     eax, eax
        push   0
        je     loc_1_6168
        push   0x109
        push   esi
        call   sub_1_246E
        jmp    loc_1_61C0
loc_1_6168:
        push   0
        push   0
        push   0x7c44                        ; -> obj1:0x7C44
        call   sub_1_02E0
        add    esp, 0x10
        or     eax, eax
        je     loc_1_619A
        push   0
        push   0x109
        push   esi
        call   sub_1_246E
        push   0
        push   dword ptr [esp + 0xa]
        call   sub_1_041B
        add    esp, 8
        jmp    loc_1_61C0
loc_1_619A:
        mov    dword ptr [0x7c48], 1         ; -> obj1:0x7C48
        mov    dword ptr [esi + 0xc], 0
        mov    dword ptr [esi + 0x14], 0
        push   esi
        call   sub_1_3FB4
        add    esp, 4
        call   sub_1_02C4
loc_1_61C0:
        pop    esi
        add    esp, 4
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_61D0:
        sub    esp, 0x2c
        mov    ecx, dword ptr [esp + 0x3c]
        mov    eax, 0xfa
        test   byte ptr [ecx + 8], 0x10
        jne    loc_1_6217
        mov    eax, dword ptr [esp + 0x34]
        mov    dword ptr [esp + 0xc], eax
        mov    eax, dword ptr [esp + 0x30]
        mov    dword ptr [esp + 0x10], eax
        lea    eax, [esp]
        mov    dword ptr [esp + 4], ecx
        mov    byte ptr [esp], 1
        push   eax
        call   sub_1_6265
        mov    ecx, dword ptr [esp + 0x18]
        mov    eax, dword ptr [esp + 0x3c]
        add    esp, 4
        mov    dword ptr [eax], ecx
        movzx  eax, byte ptr [esp + 1]
loc_1_6217:
        add    esp, 0x2c
        ret    
sub_1_621B:
        sub    esp, 0x2c
        push   esi
        mov    eax, dword ptr [esp + 0x38]
        mov    dword ptr [esp + 0x10], eax
        mov    eax, dword ptr [esp + 0x34]
        mov    esi, dword ptr [esp + 0x40]
        mov    dword ptr [esp + 0x14], eax
        mov    eax, dword ptr [esi]
        mov    dword ptr [esp + 8], eax
        lea    eax, [esp + 4]
        mov    byte ptr [esp + 4], 1
        push   eax
        call   sub_1_6265
        mov    ecx, dword ptr [esp + 0x1c]
        mov    eax, dword ptr [esp + 0x40]
        add    esp, 4
        mov    dword ptr [eax], ecx
        mov    eax, dword ptr [esp + 0x2c]
        mov    dword ptr [esi], eax
        movzx  eax, byte ptr [esp + 5]
        pop    esi
        add    esp, 0x2c
        ret    
sub_1_6265:
        sub    esp, 0x54
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x64]
        mov    edi, dword ptr [esi + 4]
        mov    ecx, dword ptr [esi + 0xc]
        mov    eax, dword ptr [ecx]
        cmp    dword ptr [edi + 0x18], eax
        je     loc_1_632B
        or     eax, eax
        je     loc_1_632B
        test   byte ptr [0x58f8], 0x10       ; -> loc_1_58F8
        je     loc_1_62AD
        cmp    dword ptr [0x5694], 0         ; -> loc_1_5694
        je     loc_1_62AD
        push   esi
        call   sub_1_2F8E
        add    esp, 4
        cmp    byte ptr [esi + 1], 0xfa
        jne    loc_1_6359
loc_1_62AD:
        lea    eax, [esp + 0xc]
        mov    dword ptr [esi + 0x18], eax
        push   edi
        push   esi
        call   sub_1_6549
        add    esp, 8
        or     eax, eax
        je     loc_1_62CB
        mov    byte ptr [esi + 1], 1
        jmp    loc_1_6359
loc_1_62CB:
        mov    ebx, dword ptr [esi + 0x18]
        test   byte ptr [esi], 0x80
        jne    loc_1_62DD
loc_1_62D3:
        call   sub_1_4A98
        test   byte ptr [esi], 0x80
        je     loc_1_62D3
loc_1_62DD:
        cmp    byte ptr [ebx + 8], 0
        je     loc_1_62EC
        mov    eax, dword ptr [ebx]
        mov    dword ptr [eax + 0x44], 0
loc_1_62EC:
        mov    dword ptr [esi + 0x28], edi
        cmp    byte ptr [esi + 1], 0
        je     loc_1_6359
        cmp    dword ptr [edi + 0x18], 0
        jne    loc_1_6359
        push   6
        mov    eax, dword ptr [esi + 0xc]
        add    eax, 4
        push   eax
        push   dword ptr [esi + 0x10]
        call   sub_1_4AF3
        add    esp, 0xc
        mov    ax, word ptr [edi + 0x24]
        mov    byte ptr [esi + 1], 0
        movzx  ecx, ax
        shr    ax, 8
        shl    ecx, 8
        movzx  eax, ax
        or     ecx, eax
        mov    dword ptr [esi + 0x14], ecx
        jmp    loc_1_6359
loc_1_632B:
        push   6
        add    ecx, 4
        push   ecx
        push   dword ptr [esi + 0x10]
        call   sub_1_4AF3
        add    esp, 0xc
        mov    ax, word ptr [edi + 0x24]
        mov    byte ptr [esi + 1], 0
        movzx  ecx, ax
        shr    ax, 8
        shl    ecx, 8
        movzx  eax, ax
        or     ecx, eax
        mov    dword ptr [esi + 0x14], ecx
        mov    dword ptr [esi + 0x28], edi
loc_1_6359:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x54
        ret    
sub_1_6360:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 0x1c]
        mov    esi, dword ptr [edi + 8]
        mov    eax, dword ptr [edi + 0x44]
        lea    ebx, [edi + 0x44]
        cmp    dword ptr [esi + 0x18], eax
        jne    loc_1_638A
        mov    eax, dword ptr [esp + 0x18]
        movzx  ecx, word ptr [esi + 0x24]
        mov    dword ptr [eax], ecx
        sub    eax, eax
        jmp    loc_1_6444
loc_1_638A:
        test   byte ptr [0x58f8], 0x10       ; -> loc_1_58F8
        je     loc_1_63D0
        cmp    dword ptr [0x5694], 0         ; -> loc_1_5694
        je     loc_1_63D0
        push   eax
        call   sub_1_2D02
        add    esp, 4
        or     eax, eax
        jne    loc_1_63B3
        mov    eax, 1
        jmp    loc_1_6444
loc_1_63B3:
        mov    ax, word ptr [eax + 0x16]
        movzx  ecx, ax
        shr    ax, 8
        shl    ecx, 8
        movzx  eax, ax
        or     ecx, eax
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [eax], ecx
        sub    eax, eax
        jmp    loc_1_6444
loc_1_63D0:
        push   -1
        push   0
        lea    eax, [esp + 0x18]
        push   0x8e
        push   eax
        call   sub_1_2468
        cmp    dword ptr [esp + 0x10], 0
        mov    eax, 1
        je     loc_1_6444
        mov    ebp, dword ptr [esp + 0x10]
        lea    eax, [ebp + 0x3c]
        add    ebp, 0x2c
        mov    ecx, dword ptr [esp + 0x10]
        push   0xa
        mov    dword ptr [ecx + 0x18], eax
        push   ebx
        push   ebp
        call   sub_1_4AF3
        mov    eax, dword ptr [esp + 0x1c]
        add    esp, 0xc
        mov    dword ptr [eax + 0xc], ebp
        mov    eax, dword ptr [esp + 0x10]
        add    ebp, 0xa
        mov    dword ptr [eax + 0x10], ebp
        mov    eax, dword ptr [esp + 0x10]
        push   esi
        mov    dword ptr [eax + 4], esi
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [eax + 8], edi
        mov    eax, dword ptr [esp + 0x14]
        mov    byte ptr [eax], 2
        push   dword ptr [esp + 0x14]
        call   sub_1_6549
        add    esp, 8
        mov    eax, 0xff
loc_1_6444:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_1_644C:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 0x18]
        mov    esi, dword ptr [esp + 0x1c]
        mov    eax, dword ptr [ebx + 0x24]
        lea    edi, [ebx + 0x24]
        cmp    dword ptr [esi + 0x18], eax
        jne    loc_1_648B
        mov    ax, word ptr [esi + 0x24]
        mov    word ptr [ebx + 0xa], ax
        add    esi, 0x1c
        lea    edi, [ebx + 0x1c]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        mov    byte ptr [ebx + 9], 0
        push   ebx
        call   sub_1_26BF
        add    esp, 4
        sub    eax, eax
        jmp    loc_1_6541
loc_1_648B:
        test   byte ptr [0x58f8], 0x10       ; -> loc_1_58F8
        je     loc_1_64CC
        cmp    dword ptr [0x5694], 0         ; -> loc_1_5694
        je     loc_1_64CC
        push   eax
        call   sub_1_2D02
        add    esp, 4
        or     eax, eax
        je     loc_1_64CC
        mov    cx, word ptr [eax + 0x16]
        mov    word ptr [ebx + 0xa], cx
        lea    esi, [eax + 0xc]
        lea    edi, [ebx + 0x1c]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        mov    byte ptr [ebx + 9], 0
        push   ebx
        call   sub_1_26BF
        add    esp, 4
        sub    eax, eax
        jmp    loc_1_6541
loc_1_64CC:
        push   -1
        push   0
        lea    eax, [esp + 0x18]
        push   0x8e
        push   eax
        call   sub_1_2468
        cmp    dword ptr [esp + 0x10], 0
        mov    eax, 1
        je     loc_1_6541
        mov    ebp, dword ptr [esp + 0x10]
        lea    eax, [ebp + 0x3c]
        add    ebp, 0x2c
        mov    ecx, dword ptr [esp + 0x10]
        push   0xa
        mov    dword ptr [ecx + 0x18], eax
        push   dword ptr [edi]
        push   ebp
        call   sub_1_4AF3
        mov    eax, dword ptr [esp + 0x1c]
        add    esp, 0xc
        mov    dword ptr [eax + 0xc], ebp
        mov    eax, dword ptr [esp + 0x10]
        add    ebp, 0xa
        mov    dword ptr [eax + 0x10], ebp
        mov    eax, dword ptr [esp + 0x10]
        push   esi
        mov    dword ptr [eax + 4], esi
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [eax + 8], ebx
        mov    eax, dword ptr [esp + 0x14]
        mov    byte ptr [eax], 4
        push   dword ptr [esp + 0x14]
        call   sub_1_6549
        add    esp, 8
        mov    eax, 0xff
loc_1_6541:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_1_6549:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x10]
        mov    dword ptr [edi + 0x20], 0x14
        mov    esi, dword ptr [edi + 0x18]
        cmp    word ptr [0x7cf4], 0          ; -> loc_1_7CF4
        lea    ebx, [esi + 0x2a]
        jne    loc_1_658B
        push   1
        push   0
        push   0x24df                        ; -> loc_1_24DF
        push   0x7cf4                        ; -> loc_1_7CF4
        call   sub_1_5206
        add    esp, 0x10
        or     eax, eax
        je     loc_1_658B
        mov    eax, 1
        jmp    loc_1_6639
loc_1_658B:
        mov    dword ptr [esi + 4], 0
        mov    ax, word ptr [0x7cf4]         ; -> loc_1_7CF4
        mov    word ptr [esi + 0xa], ax
        mov    dword ptr [esi + 0xc], 0
        lea    eax, [esi + 0x1c]
        push   6
        push   -1
        push   eax
        call   sub_1_4AE2
        add    esp, 0xc
        mov    word ptr [esi + 0x22], 1
        mov    dword ptr [esi + 0x24], ebx
        mov    word ptr [esi + 0x28], 0x28
        mov    byte ptr [ebx + 5], 1
        lea    eax, [ebx + 0xa]
        mov    dword ptr [ebx + 6], 0
        push   6
        push   -1
        push   eax
        call   sub_1_4AE2
        add    esp, 0xc
        mov    word ptr [ebx + 0x10], 0x5304
        add    ebx, 0x1e
        mov    word ptr [ebx], 0x100
        mov    eax, dword ptr [edi + 0xc]
        mov    eax, dword ptr [eax]
        mov    dword ptr [ebx + 2], eax
        mov    word ptr [ebx + 6], 0xffff
        mov    word ptr [ebx + 8], 0xffff
        mov    eax, dword ptr [0x5680]       ; -> loc_1_5680
        mov    dword ptr [edi + 0x24], eax
        mov    dword ptr [0x5680], edi       ; -> loc_1_5680
        push   edi
        push   0x25af                        ; -> loc_1_25AF
        push   0x3e8
        call   sub_1_4AAB
        add    esp, 0xc
        mov    dword ptr [edi + 0x1c], eax
        mov    dword ptr [edi + 0x14], 5
        push   dword ptr [esp + 0x14]
        push   esi
        call   sub_1_4092
        add    esp, 8
        sub    eax, eax
loc_1_6639:
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  000h, 000h, 000h
sub_1_6640:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [0x7cfc]       ; -> loc_1_7CFC
        cmp    dword ptr [0x7d10], eax       ; -> loc_1_7D10
        ja     loc_1_665E
        mov    eax, 1
        jmp    loc_1_67C5
loc_1_665E:
        cmp    dword ptr [0x7cb8], 1         ; -> loc_1_7CB8
        jne    loc_1_6671
        mov    eax, 1
        jmp    loc_1_67C5
loc_1_6671:
        mov    dword ptr [0x7cb8], 1         ; -> loc_1_7CB8
        mov    eax, dword ptr [0x7ca8]       ; -> loc_1_7CA8
        lea    ebp, [eax*2]
        mov    ebx, dword ptr [0x7cfc]       ; -> loc_1_7CFC
        imul   ebx, ebx, 0x28
        push   -1
        push   0
        lea    eax, [esp + 0x18]
        push   ebp
        add    ebx, 8
        push   eax
        call   sub_1_2468
        cmp    dword ptr [esp + 0x10], 0
        jne    loc_1_66BD
        mov    dword ptr [0x7cb8], 0         ; -> loc_1_7CB8
        mov    eax, 1
        jmp    loc_1_67C5
loc_1_66BD:
        push   -1
        push   0
        lea    eax, [esp + 0x1c]
        push   ebx
        push   eax
        call   sub_1_2468
        cmp    dword ptr [esp + 0x14], 0
        jne    loc_1_66F3
        push   0
        push   ebx
        push   dword ptr [esp + 0x18]
        call   sub_1_246E
        mov    dword ptr [0x7cb8], 0         ; -> loc_1_7CB8
        mov    eax, 1
        jmp    loc_1_67C5
loc_1_66F3:
        mov    eax, dword ptr [0x7cfc]       ; -> loc_1_7CFC
        lea    ecx, [eax*4]
        mov    edi, dword ptr [esp + 0x10]
        mov    esi, dword ptr [0x56b0]       ; -> loc_1_56B0
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [0x56b0], eax       ; -> loc_1_56B0
        mov    edx, dword ptr [0x7cfc]       ; -> loc_1_7CFC
        shl    edx, 3
        add    edx, dword ptr [esp + 0x10]
        mov    eax, dword ptr [0x7cfc]       ; -> loc_1_7CFC
        lea    ecx, [eax*4]
        mov    edi, edx
        mov    esi, dword ptr [0x56b4]       ; -> loc_1_56B4
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    dword ptr [0x56b4], edx       ; -> loc_1_56B4
        push   0
        push   dword ptr [0x7ca8]            ; -> loc_1_7CA8
        push   dword ptr [0x7d18]            ; -> loc_1_7D18
        call   sub_1_246E
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [0x7d18], eax       ; -> loc_1_7D18
        mov    dword ptr [0x7ca8], ebp       ; -> loc_1_7CA8
        mov    ecx, dword ptr [esp + 0x14]
        mov    eax, dword ptr [0x7cf8]       ; -> loc_1_7CF8
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [0x7cf8], eax       ; -> loc_1_7CF8
        mov    dword ptr [eax + 4], ebx
        sub    eax, eax
        mov    ecx, dword ptr [esp + 0x14]
        add    ecx, 8
        cmp    dword ptr [0x7cfc], eax       ; -> loc_1_7CFC
        jbe    loc_1_67A8
loc_1_678D:
        mov    edx, dword ptr [0x7d20]       ; -> loc_1_7D20
        mov    dword ptr [ecx + 0x24], edx
        mov    dword ptr [0x7d20], ecx       ; -> loc_1_7D20
        add    ecx, 0x28
        inc    eax
        cmp    eax, dword ptr [0x7cfc]       ; -> loc_1_7CFC
        jb     loc_1_678D
loc_1_67A8:
        mov    eax, dword ptr [0x7cfc]       ; -> loc_1_7CFC
        add    dword ptr [0x7d08], eax       ; -> loc_1_7D08
        shl    dword ptr [0x7cfc], 1         ; -> loc_1_7CFC
        mov    dword ptr [0x7cb8], 0         ; -> loc_1_7CB8
        sub    eax, eax
loc_1_67C5:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
sub_1_67CD:
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [0x7d14]       ; -> loc_1_7D14
        cmp    dword ptr [0x7d00], eax       ; -> loc_1_7D00
        jb     loc_1_67EB
        mov    eax, 1
        jmp    loc_1_69C9
loc_1_67EB:
        cmp    dword ptr [0x7cbc], 1         ; -> loc_1_7CBC
        jne    loc_1_67FE
        mov    eax, 1
        jmp    loc_1_69C9
loc_1_67FE:
        mov    dword ptr [0x7cbc], 1         ; -> loc_1_7CBC
        mov    eax, dword ptr [0x7cb0]       ; -> obj1:0x7CB0
        add    eax, eax
        mov    dword ptr [esp + 0x18], eax
        mov    ebx, dword ptr [0x7d00]       ; -> loc_1_7D00
        imul   ebx, ebx, 0x6c
        push   -1
        push   0
        add    ebx, 8
        lea    eax, [esp + 0x18]
        push   dword ptr [esp + 0x20]
        push   eax
        call   sub_1_2468
        cmp    dword ptr [esp + 0x10], 0
        jne    loc_1_684C
        mov    dword ptr [0x7cbc], 0         ; -> loc_1_7CBC
        mov    eax, 1
        jmp    loc_1_69C9
loc_1_684C:
        push   -1
        push   0
        lea    eax, [esp + 0x1c]
        push   ebx
        push   eax
        call   sub_1_2468
        cmp    dword ptr [esp + 0x14], 0
        jne    loc_1_6882
        push   0
        push   ebx
        push   dword ptr [esp + 0x18]
        call   sub_1_246E
        mov    dword ptr [0x7cbc], 0         ; -> loc_1_7CBC
        mov    eax, 1
        jmp    loc_1_69C9
loc_1_6882:
        mov    eax, dword ptr [0x7d00]       ; -> loc_1_7D00
        lea    ecx, [eax*4]
        mov    edi, dword ptr [esp + 0x10]
        mov    esi, dword ptr [0x56c0]       ; -> loc_1_56C0
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [0x56c0], eax       ; -> loc_1_56C0
        mov    edx, dword ptr [0x7d00]       ; -> loc_1_7D00
        shl    edx, 3
        add    edx, dword ptr [esp + 0x10]
        mov    eax, dword ptr [0x7d00]       ; -> loc_1_7D00
        lea    ecx, [eax*4]
        mov    edi, edx
        mov    esi, dword ptr [0x56c4]       ; -> loc_1_56C4
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    dword ptr [0x56c4], edx       ; -> loc_1_56C4
        mov    eax, dword ptr [0x7d00]       ; -> loc_1_7D00
        lea    ebp, [edx + eax*8]
        add    eax, eax
        mov    edi, ebp
        mov    esi, dword ptr [0x56d0]       ; -> loc_1_56D0
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    dword ptr [0x56d0], ebp       ; -> loc_1_56D0
        mov    eax, dword ptr [0x7d00]       ; -> loc_1_7D00
        lea    ebp, [ebp + eax*4]
        lea    ecx, [eax*4]
        mov    edi, ebp
        mov    esi, dword ptr [0x56d4]       ; -> loc_1_56D4
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    dword ptr [0x56d4], ebp       ; -> loc_1_56D4
        sub    eax, eax
        cmp    dword ptr [0x56c8], eax       ; -> loc_1_56C8
        jbe    loc_1_6936
loc_1_6925:
        mov    ecx, dword ptr [edx]
        inc    eax
        mov    dword ptr [ecx + 4], edx
        add    edx, 4
        cmp    dword ptr [0x56c8], eax       ; -> loc_1_56C8
        ja     loc_1_6925
loc_1_6936:
        push   0
        push   dword ptr [0x7cb0]            ; -> obj1:0x7CB0
        push   dword ptr [0x7d04]            ; -> loc_1_7D04
        call   sub_1_246E
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [0x7d04], eax       ; -> loc_1_7D04
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [0x7cb0], eax       ; -> obj1:0x7CB0
        mov    eax, dword ptr [0x7cec]       ; -> loc_1_7CEC
        mov    ecx, dword ptr [esp + 0x14]
        sub    esi, esi
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [0x7cec], eax       ; -> loc_1_7CEC
        mov    dword ptr [eax + 4], ebx
        mov    edi, dword ptr [esp + 0x14]
        add    edi, 8
        cmp    dword ptr [0x7d00], esi       ; -> loc_1_7D00
        jbe    loc_1_69AC
loc_1_6983:
        push   0x30
        push   0
        lea    eax, [edi + 0x16]
        push   eax
        call   sub_1_4AE2
        add    esp, 0xc
        mov    eax, dword ptr [0x7d24]       ; -> loc_1_7D24
        mov    dword ptr [edi], eax
        mov    dword ptr [0x7d24], edi       ; -> loc_1_7D24
        add    edi, 0x6c
        inc    esi
        cmp    dword ptr [0x7d00], esi       ; -> loc_1_7D00
        ja     loc_1_6983
loc_1_69AC:
        mov    eax, dword ptr [0x7d00]       ; -> loc_1_7D00
        add    dword ptr [0x7d0c], eax       ; -> loc_1_7D0C
        shl    dword ptr [0x7d00], 1         ; -> loc_1_7D00
        mov    dword ptr [0x7cbc], 0         ; -> loc_1_7CBC
        sub    eax, eax
loc_1_69C9:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
sub_1_69D1:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x14]
        push   6
        lea    ebx, [esi + 0x42]
        lea    edi, [esi + 0x18]
        lea    ebp, [ebx + 0x1e]
        mov    dword ptr [edi + 4], 1
        mov    dword ptr [edi + 0x18], 0x2f3e ; -> loc_1_2F3E
        mov    dword ptr [edi + 0xc], 0
        lea    eax, [esi + 0xc]
        push   eax
        lea    eax, [edi + 0x1c]
        push   eax
        call   sub_1_4AF3
        add    esp, 0xc
        mov    word ptr [edi + 0x22], 1
        mov    dword ptr [edi + 0x24], ebx
        mov    word ptr [ebp], 0x200
        cmp    byte ptr [esi + 0x14], 0
        jne    loc_1_6A48
        mov    word ptr [edi + 0xa], 0x5304
        lea    eax, [esi + 8]
        push   eax
        push   dword ptr [esi + 4]
        add    ebp, 2
        push   ebp
        call   sub_1_2EC4
        add    esp, 0xc
        lea    ecx, [eax*8 + 0x20]
        mov    word ptr [edi + 0x28], cx
        mov    byte ptr [ebx + 5], 1
        jmp    loc_1_6A6D
loc_1_6A48:
        mov    word ptr [edi + 0xa], 0x5204
        push   esi
        add    ebp, 2
        push   ebp
        call   sub_1_35CA
        add    esp, 8
        mov    ecx, eax
        shl    cx, 6
        add    cx, 0x20
        mov    word ptr [edi + 0x28], cx
        mov    byte ptr [ebx + 5], 4
loc_1_6A6D:
        or     eax, eax
        je     loc_1_6AAF
        mov    eax, dword ptr [esi + 4]
        push   8
        mov    eax, dword ptr [eax + 0x18]
        mov    dword ptr [ebx + 6], eax
        lea    eax, [esi + 0xc]
        push   eax
        add    ebx, 0xa
        push   ebx
        call   sub_1_4AF3
        add    esp, 0xc
        cmp    byte ptr [esi + 0x14], 0
        je     loc_1_6A9F
        test   byte ptr [esi + 0x15], 1
        je     loc_1_6A9F
        mov    eax, dword ptr [esi + 4]
        or     dword ptr [eax + 8], 0x20
loc_1_6A9F:
        push   dword ptr [esi + 4]
        push   edi
        call   sub_1_4092
        add    esp, 8
        sub    eax, eax
        jmp    loc_1_6AB4
loc_1_6AAF:
        mov    eax, 1
loc_1_6AB4:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_6AB9:
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [0x7cb4], 0         ; -> obj1:0x7CB4
        je     loc_1_6B65
        mov    esi, dword ptr [0x7d1c]       ; -> loc_1_7D1C
        dec    dword ptr [0x7cb4]            ; -> obj1:0x7CB4
        mov    eax, dword ptr [esi + 0x38]
        mov    dword ptr [0x7d1c], eax       ; -> loc_1_7D1C
        mov    edi, dword ptr [esi + 0x48]
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [edi], esi
        mov    dword ptr [edi + 4], eax
        mov    dword ptr [edi + 8], 0
        mov    bl, byte ptr [esp + 0x18]
        mov    byte ptr [edi + 0x14], bl
        mov    ax, word ptr [esp + 0x1c]
        mov    word ptr [edi + 0x16], ax
        mov    eax, dword ptr [esp + 0x14]
        or     eax, eax
        jne    loc_1_6B30
        push   6
        push   -1
        lea    eax, [edi + 0xc]
        push   eax
        call   sub_1_4AE2
        add    esp, 0xc
        mov    byte ptr [edi + 0x15], 1
        or     bl, bl
        jne    loc_1_6B28
        mov    word ptr [edi + 0x12], 0x5304
        jmp    loc_1_6B43
loc_1_6B28:
        mov    word ptr [edi + 0x12], 0x5204
        jmp    loc_1_6B43
loc_1_6B30:
        mov    byte ptr [edi + 0x15], 0
        push   8
        push   eax
        lea    eax, [edi + 0xc]
        push   eax
        call   sub_1_4AF3
        add    esp, 0xc
loc_1_6B43:
        push   edi
        call   sub_1_69D1
        add    esp, 4
        cmp    eax, 1
        jne    loc_1_6B65
        mov    eax, dword ptr [0x7d1c]       ; -> loc_1_7D1C
        mov    dword ptr [esi + 0x38], eax
        mov    dword ptr [0x7d1c], esi       ; -> loc_1_7D1C
        inc    dword ptr [0x7cb4]            ; -> obj1:0x7CB4
loc_1_6B65:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_6B69:
        push   ebx
        push   esi
        push   edi
        sub    esi, esi
        cmp    dword ptr [0x58c8], esi       ; -> loc_1_58C8
        jbe    loc_1_6BAC
        mov    edi, 0x58dc                   ; -> loc_1_58DC
        mov    ebx, 2
loc_1_6B80:
        mov    ecx, dword ptr [edi]
        mov    eax, dword ptr [ecx + 0x48]
        test   dword ptr [eax + 4], ebx
        jne    loc_1_6BA0
        push   0xffff
        mov    al, byte ptr [0x7cc0]         ; -> loc_1_7CC0
        push   eax
        push   0
        push   ecx
        call   sub_1_6AB9
        add    esp, 0x10
loc_1_6BA0:
        add    edi, 4
        inc    esi
        cmp    dword ptr [0x58c8], esi       ; -> loc_1_58C8
        ja     loc_1_6B80
loc_1_6BAC:
        cmp    byte ptr [0x7cc0], 1          ; -> loc_1_7CC0
        sbb    al, al
        neg    al
        mov    byte ptr [0x7cc0], al         ; -> loc_1_7CC0
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_6BC0:
        push   esi
        and    dword ptr [0x58f8], 0xffffffaf ; -> loc_1_58F8
        push   dword ptr [0x5690]            ; -> loc_1_5690
        call   sub_1_4AD5
        add    esp, 4
        mov    dword ptr [0x5690], 0         ; -> loc_1_5690
        push   0
        push   0x5304
        call   sub_1_041B
        add    esp, 8
        test   byte ptr [0x58f9], 0x10       ; -> obj1:0x58F9
        jne    loc_1_6C07
        push   0
        push   0x5204
        call   sub_1_041B
        add    esp, 8
loc_1_6C07:
        cmp    dword ptr [0x7ca8], 0         ; -> loc_1_7CA8
        je     loc_1_6C23
        push   0
        push   dword ptr [0x7ca8]            ; -> loc_1_7CA8
        push   dword ptr [0x7d18]            ; -> loc_1_7D18
        call   sub_1_246E
loc_1_6C23:
        mov    dword ptr [0x7ca8], 0         ; -> loc_1_7CA8
        cmp    dword ptr [0x7cf8], 0         ; -> loc_1_7CF8
        je     loc_1_6C52
loc_1_6C36:
        mov    eax, dword ptr [0x7cf8]       ; -> loc_1_7CF8
        push   0
        mov    esi, dword ptr [eax]
        push   dword ptr [eax + 4]
        push   eax
        call   sub_1_246E
        mov    dword ptr [0x7cf8], esi       ; -> loc_1_7CF8
        or     esi, esi
        jne    loc_1_6C36
loc_1_6C52:
        cmp    dword ptr [0x7cb0], 0         ; -> obj1:0x7CB0
        je     loc_1_6C6E
        push   0
        push   dword ptr [0x7cb0]            ; -> obj1:0x7CB0
        push   dword ptr [0x7d04]            ; -> loc_1_7D04
        call   sub_1_246E
loc_1_6C6E:
        mov    dword ptr [0x7cb0], 0         ; -> obj1:0x7CB0
        cmp    dword ptr [0x7cec], 0         ; -> loc_1_7CEC
        je     loc_1_6C9D
loc_1_6C81:
        mov    eax, dword ptr [0x7cec]       ; -> loc_1_7CEC
        push   0
        mov    esi, dword ptr [eax]
        push   dword ptr [eax + 4]
        push   eax
        call   sub_1_246E
        mov    dword ptr [0x7cec], esi       ; -> loc_1_7CEC
        or     esi, esi
        jne    loc_1_6C81
loc_1_6C9D:
        cmp    dword ptr [0x56bc], 0         ; -> loc_1_56BC
        je     loc_1_6CAB
        call   sub_1_2B80
loc_1_6CAB:
        cmp    dword ptr [0x56cc], 0         ; -> loc_1_56CC
        je     loc_1_6CB9
        call   sub_1_328A
loc_1_6CB9:
        pop    esi
        ret    
sub_1_6CBB:
        push   esi
        push   edi
        sub    edi, edi
        cmp    dword ptr [0x56b8], edi       ; -> loc_1_56B8
        jbe    loc_1_6CEC
        mov    esi, dword ptr [esp + 0xc]
loc_1_6CCB:
        mov    eax, dword ptr [0x56b4]       ; -> loc_1_56B4
        mov    eax, dword ptr [eax + edi*4]
        cmp    dword ptr [eax + 0x20], esi
        jne    loc_1_6CE3
        push   edi
        push   eax
        call   sub_1_2D3B
        add    esp, 8
        dec    edi
loc_1_6CE3:
        inc    edi
        cmp    edi, dword ptr [0x56b8]       ; -> loc_1_56B8
        jb     loc_1_6CCB
loc_1_6CEC:
        pop    edi
        pop    esi
        ret    
sub_1_6CEF:
        sub    esp, 0x58
        push   esi
        push   edi
        lea    eax, [esp + 0x2a]
        mov    dword ptr [esp + 8], eax
        mov    dword ptr [esp + 0x38], 0
        mov    di, word ptr [esp + 0x68]
        mov    word ptr [esp + 0x3e], di
        mov    dword ptr [esp + 0x40], 0
        push   6
        lea    eax, [esp + 0x54]
        push   -1
        push   eax
        call   sub_1_4AE2
        mov    word ptr [esp + 0x62], 1
        lea    eax, [esp + 0x18]
        mov    dword ptr [esp + 0x64], eax
        mov    word ptr [esp + 0x68], 0x28
        mov    esi, dword ptr [esp + 0x70]
        add    esp, 0xc
        mov    eax, dword ptr [esi + 0x18]
        mov    dword ptr [esp + 0x12], eax
        push   6
        lea    eax, [esp + 0x1a]
        push   -1
        push   eax
        call   sub_1_4AE2
        mov    word ptr [esp + 0x28], di
        add    esp, 0xc
        cmp    di, 0x5304
        jne    loc_1_6D94
        mov    eax, dword ptr [esp + 8]
        mov    byte ptr [esp + 0x11], 1
        mov    word ptr [eax], 0x100
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [eax + 2], 0xffffffff
        mov    eax, dword ptr [esp + 8]
        mov    word ptr [eax + 6], 0
        mov    eax, dword ptr [esp + 8]
        mov    word ptr [eax + 8], 0
        jmp    loc_1_6DAC
loc_1_6D94:
        mov    eax, dword ptr [esp + 8]
        mov    byte ptr [esp + 0x11], 4
        mov    word ptr [eax], 0x100
        mov    eax, dword ptr [esp + 8]
        mov    word ptr [eax + 2], 0xffff
loc_1_6DAC:
        mov    edi, dword ptr [0x58ec]       ; -> loc_1_58EC
        mov    dword ptr [0x58ec], esi       ; -> loc_1_58EC
        lea    eax, [esp + 0x34]
        push   esi
        push   eax
        call   sub_1_4092
        add    esp, 8
        mov    dword ptr [0x58ec], edi       ; -> loc_1_58EC
        lea    eax, [esp + 0x34]
        push   eax
        call   sub_1_0D40
        add    esp, 4
        pop    edi
        pop    esi
        add    esp, 0x58
        ret    
sub_1_6DDF:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    word ptr [esp + 0xe], 0x5204
        mov    esi, dword ptr [esp + 0x14]
        mov    eax, esi
        imul   eax, eax, 0xe
        mov    dword ptr [0x7cb0], eax       ; -> obj1:0x7CB0
        push   -1
        push   0
        push   eax
        push   0x7d04                        ; -> loc_1_7D04
        call   sub_1_2468
        cmp    dword ptr [0x7d04], 0         ; -> loc_1_7D04
        jne    loc_1_6E26
        mov    dword ptr [0x7cb0], 0         ; -> obj1:0x7CB0
        mov    eax, 1
        jmp    loc_1_6F5F
loc_1_6E26:
        mov    edi, esi
        imul   edi, edi, 0x6c
        add    edi, 8
        push   -1
        push   0
        push   edi
        push   0x7cec                        ; -> loc_1_7CEC
        call   sub_1_2468
        cmp    dword ptr [0x7cec], 0         ; -> loc_1_7CEC
        jne    loc_1_6E6D
        push   0
        push   dword ptr [0x7cb0]            ; -> obj1:0x7CB0
        push   dword ptr [0x7d04]            ; -> loc_1_7D04
        call   sub_1_246E
        mov    dword ptr [0x7cb0], 0         ; -> obj1:0x7CB0
        mov    eax, 1
        jmp    loc_1_6F5F
loc_1_6E6D:
        mov    dword ptr [0x7d00], esi       ; -> loc_1_7D00
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [0x7d14], eax       ; -> loc_1_7D14
        mov    eax, dword ptr [0x7d04]       ; -> loc_1_7D04
        mov    dword ptr [0x56c0], eax       ; -> loc_1_56C0
        lea    eax, [esi*4]
        mov    ecx, dword ptr [0x56c0]       ; -> loc_1_56C0
        add    ecx, eax
        mov    dword ptr [0x56c4], ecx       ; -> loc_1_56C4
        add    eax, ecx
        mov    dword ptr [0x56d0], eax       ; -> loc_1_56D0
        lea    eax, [esi*2]
        add    eax, dword ptr [0x56d0]       ; -> loc_1_56D0
        mov    dword ptr [0x56d4], eax       ; -> loc_1_56D4
        mov    ebx, dword ptr [0x7cec]       ; -> loc_1_7CEC
        mov    eax, dword ptr [0x7cec]       ; -> loc_1_7CEC
        add    ebx, 8
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [0x7cec]       ; -> loc_1_7CEC
        or     esi, esi
        mov    dword ptr [eax + 4], edi
        mov    dword ptr [0x7d24], 0         ; -> loc_1_7D24
        je     loc_1_6F03
        mov    edi, esi
loc_1_6EE0:
        push   0x30
        push   0
        lea    eax, [ebx + 0x16]
        push   eax
        call   sub_1_4AE2
        add    esp, 0xc
        mov    eax, dword ptr [0x7d24]       ; -> loc_1_7D24
        mov    dword ptr [ebx], eax
        mov    dword ptr [0x7d24], ebx       ; -> loc_1_7D24
        add    ebx, 0x6c
        dec    edi
        jne    loc_1_6EE0
loc_1_6F03:
        mov    dword ptr [0x7d0c], esi       ; -> loc_1_7D0C
        mov    dword ptr [0x56c8], 0         ; -> loc_1_56C8
        mov    dword ptr [0x56d8], 0         ; -> loc_1_56D8
        push   1
        sub    edi, edi
        push   0
        lea    eax, [esp + 0x16]
        push   0x30af                        ; -> loc_1_30AF
        push   eax
        call   sub_1_5206
        add    esp, 0x10
        cmp    dword ptr [0x58c8], edi       ; -> loc_1_58C8
        jbe    loc_1_6F5D
        mov    esi, 0x58dc                   ; -> loc_1_58DC
loc_1_6F42:
        push   0x5204
        push   dword ptr [esi]
        call   sub_1_6CEF
        add    esp, 8
        add    esi, 4
        inc    edi
        cmp    dword ptr [0x58c8], edi       ; -> loc_1_58C8
        ja     loc_1_6F42
loc_1_6F5D:
        sub    eax, eax
loc_1_6F5F:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_1_6F66:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    word ptr [esp + 0x12], 0x5304
        mov    esi, dword ptr [0x7cac]       ; -> loc_1_7CAC
        add    esi, 0x2a
        mov    ebx, dword ptr [esp + 0x18]
        lea    eax, [ebx*8]
        mov    dword ptr [0x7ca8], eax       ; -> loc_1_7CA8
        mov    eax, esi
        imul   eax, dword ptr [0x7cb4]       ; -> obj1:0x7CB4
        mov    ecx, ebx
        imul   ecx, ecx, 0x28
        lea    ebp, [eax + ecx + 8]
        push   -1
        push   0
        push   dword ptr [0x7ca8]            ; -> loc_1_7CA8
        push   0x7d18                        ; -> loc_1_7D18
        call   sub_1_2468
        cmp    dword ptr [0x7d18], 0         ; -> loc_1_7D18
        jne    loc_1_6FC1
        jmp    loc_1_7214
loc_1_6FC1:
        push   -1
        push   0
        push   ebp
        push   0x7cf8                        ; -> loc_1_7CF8
        call   sub_1_2468
        cmp    dword ptr [0x7cf8], 0         ; -> loc_1_7CF8
        jne    loc_1_6FF1
        push   0
        push   dword ptr [0x7ca8]            ; -> loc_1_7CA8
        push   dword ptr [0x7d18]            ; -> loc_1_7D18
        call   sub_1_246E
        jmp    loc_1_7214
loc_1_6FF1:
        mov    dword ptr [0x7cfc], ebx       ; -> loc_1_7CFC
        mov    eax, dword ptr [esp + 0x1c]
        mov    dword ptr [0x7d10], eax       ; -> loc_1_7D10
        mov    eax, dword ptr [0x7d18]       ; -> loc_1_7D18
        mov    dword ptr [0x56b0], eax       ; -> loc_1_56B0
        lea    eax, [ebx*4]
        add    eax, dword ptr [0x56b0]       ; -> loc_1_56B0
        mov    dword ptr [0x56b4], eax       ; -> loc_1_56B4
        mov    edi, dword ptr [0x7cf8]       ; -> loc_1_7CF8
        mov    eax, dword ptr [0x7cf8]       ; -> loc_1_7CF8
        add    edi, 8
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [0x7cf8]       ; -> loc_1_7CF8
        or     ebx, ebx
        mov    dword ptr [eax + 4], ebp
        mov    dword ptr [0x7d20], 0         ; -> loc_1_7D20
        je     loc_1_705D
        mov    eax, ebx
loc_1_7048:
        mov    ecx, dword ptr [0x7d20]       ; -> loc_1_7D20
        mov    dword ptr [edi + 0x24], ecx
        mov    dword ptr [0x7d20], edi       ; -> loc_1_7D20
        add    edi, 0x28
        dec    eax
        jne    loc_1_7048
loc_1_705D:
        mov    dword ptr [0x7d08], ebx       ; -> loc_1_7D08
        mov    dword ptr [0x58f4], 0         ; -> loc_1_58F4
        mov    dword ptr [0x56b8], 0         ; -> loc_1_56B8
        mov    eax, dword ptr [0x582c]       ; -> loc_1_582C
        cmp    dword ptr [0x7cb4], eax       ; -> obj1:0x7CB4
        jbe    loc_1_708E
        mov    eax, dword ptr [0x7cb4]       ; -> obj1:0x7CB4
        mov    dword ptr [0x582c], eax       ; -> loc_1_582C
loc_1_708E:
        mov    eax, dword ptr [0x7cb4]       ; -> obj1:0x7CB4
        add    eax, eax
        cmp    eax, dword ptr [0x5830]       ; -> loc_1_5830
        jbe    loc_1_70A2
        mov    dword ptr [0x5830], eax       ; -> loc_1_5830
loc_1_70A2:
        call   sub_1_0F2E
        or     eax, eax
        jne    loc_1_7214
        call   sub_1_1050
        or     eax, eax
        jne    loc_1_7214
        sub    ecx, ecx
        cmp    dword ptr [0x7cb4], ecx       ; -> obj1:0x7CB4
        jbe    loc_1_7121
        mov    ebx, 0xfffffffe
loc_1_70CB:
        mov    edx, dword ptr [0x58b4]       ; -> loc_1_58B4
        dec    dword ptr [0x5848]            ; -> loc_1_5848
        mov    eax, dword ptr [edx + 0x38]
        mov    dword ptr [0x58b4], eax       ; -> loc_1_58B4
        mov    dword ptr [edx + 0x48], edi
        mov    eax, dword ptr [0x58b8]       ; -> loc_1_58B8
        mov    ebp, dword ptr [eax]
        mov    ebp, dword ptr [ebp]
        mov    dword ptr [0x58b8], ebp       ; -> loc_1_58B8
        mov    ebp, dword ptr [eax]
        mov    dword ptr [ebp], 0
        add    edi, esi
        add    dword ptr [0x584c], ebx       ; -> loc_1_584C
        mov    dword ptr [edx + 0x44], eax
        mov    byte ptr [edx + 0x3c], 0x10
        mov    eax, dword ptr [0x7d1c]       ; -> loc_1_7D1C
        inc    ecx
        mov    dword ptr [edx + 0x38], eax
        mov    dword ptr [0x7d1c], edx       ; -> loc_1_7D1C
        cmp    ecx, dword ptr [0x7cb4]       ; -> obj1:0x7CB4
        jb     loc_1_70CB
loc_1_7121:
        or     dword ptr [0x58f8], 0x10      ; -> loc_1_58F8
        sub    esi, esi
        cmp    dword ptr [0x58c8], esi       ; -> loc_1_58C8
        jbe    loc_1_715D
        mov    edi, 0x58dc                   ; -> loc_1_58DC
        sub    ebx, ebx
loc_1_7139:
        mov    eax, dword ptr [edi]
        cmp    dword ptr [eax + 0x18], ebx
        je     loc_1_7151
        cmp    dword ptr [0x7d08], ebx       ; -> loc_1_7D08
        jbe    loc_1_715D
        push   eax
        call   sub_1_7221
        add    esp, 4
loc_1_7151:
        add    edi, 4
        inc    esi
        cmp    dword ptr [0x58c8], esi       ; -> loc_1_58C8
        ja     loc_1_7139
loc_1_715D:
        mov    eax, dword ptr [esp + 0x20]
        or     eax, eax
        je     loc_1_7185
        push   dword ptr [esp + 0x24]
        push   eax
        call   sub_1_6DDF
        add    esp, 8
        or     eax, eax
        je     loc_1_7185
        push   0
        call   sub_1_6BC0
        add    esp, 4
        jmp    loc_1_7214
loc_1_7185:
        push   1
        push   0
        lea    eax, [esp + 0x1a]
        push   0x2aed                        ; -> loc_1_2AED
        sub    esi, esi
        push   eax
        call   sub_1_5206
        add    esp, 0x10
        cmp    dword ptr [0x58c8], esi       ; -> loc_1_58C8
        jbe    loc_1_71C5
        mov    edi, 0x58dc                   ; -> loc_1_58DC
loc_1_71AA:
        push   0x5304
        push   dword ptr [edi]
        call   sub_1_6CEF
        add    esp, 8
        add    edi, 4
        inc    esi
        cmp    dword ptr [0x58c8], esi       ; -> loc_1_58C8
        ja     loc_1_71AA
loc_1_71C5:
        mov    eax, dword ptr [0x58ec]       ; -> loc_1_58EC
        cmp    dword ptr [eax + 0x18], 0
        jne    loc_1_71DD
        push   0xbb8
        call   sub_1_302C
        add    esp, 4
loc_1_71DD:
        mov    dword ptr [0x568c], 6         ; -> loc_1_568C
        test   byte ptr [0x58f9], 0x10       ; -> obj1:0x58F9
        jne    loc_1_7210
        push   0
        push   0x2ddf                        ; -> loc_1_2DDF
        push   0x6e
        call   sub_1_4AAB
        add    esp, 0xc
        mov    dword ptr [0x5690], eax       ; -> loc_1_5690
        mov    dword ptr [0x5688], 0x110     ; -> loc_1_5688
loc_1_7210:
        sub    eax, eax
        jmp    loc_1_7219
loc_1_7214:
        mov    eax, 1
loc_1_7219:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_1_7221:
        push   esi
        push   edi
        mov    esi, dword ptr [0x7d20]       ; -> loc_1_7D20
        mov    edi, dword ptr [esp + 0xc]
        mov    eax, dword ptr [esi + 0x24]
        mov    dword ptr [0x7d20], eax       ; -> loc_1_7D20
        dec    dword ptr [0x7d08]            ; -> loc_1_7D08
        mov    eax, dword ptr [edi + 0x18]
        mov    dword ptr [esi], eax
        lea    eax, [esi + 0xc]
        push   6
        push   0
        push   eax
        call   sub_1_4AE2
        add    esp, 0xc
        mov    word ptr [esi + 0x14], 0
        mov    ax, word ptr [edi + 0x24]
        mov    ch, al
        mov    cl, ah
        mov    word ptr [esi + 0x16], cx
        mov    dword ptr [esi + 0x18], 0xffffffff
        mov    dword ptr [esi + 0x20], edi
        mov    eax, dword ptr [edi + 4]
        mov    dword ptr [esi + 4], eax
        mov    dword ptr [esi + 0x1c], 0
        mov    eax, dword ptr [0x56b8]       ; -> loc_1_56B8
        cmp    dword ptr [0x58f4], eax       ; -> loc_1_58F4
        jae    loc_1_72C0
        mov    ecx, dword ptr [0x56b4]       ; -> loc_1_56B4
        mov    eax, dword ptr [0x58f4]       ; -> loc_1_58F4
        mov    eax, dword ptr [ecx + eax*4]
        mov    edx, dword ptr [0x56b4]       ; -> loc_1_56B4
        mov    ecx, dword ptr [0x56b8]       ; -> loc_1_56B8
        mov    dword ptr [edx + ecx*4], eax
        mov    ecx, dword ptr [0x56b0]       ; -> loc_1_56B0
        mov    eax, dword ptr [0x58f4]       ; -> loc_1_58F4
        mov    eax, dword ptr [ecx + eax*4]
        mov    edx, dword ptr [0x56b0]       ; -> loc_1_56B0
        mov    ecx, dword ptr [0x56b8]       ; -> loc_1_56B8
        mov    dword ptr [edx + ecx*4], eax
loc_1_72C0:
        mov    eax, dword ptr [0x56b4]       ; -> loc_1_56B4
        mov    ecx, dword ptr [0x58f4]       ; -> loc_1_58F4
        mov    dword ptr [eax + ecx*4], esi
        mov    eax, dword ptr [esi]
        mov    edx, dword ptr [0x56b0]       ; -> loc_1_56B0
        mov    ecx, dword ptr [0x58f4]       ; -> loc_1_58F4
        mov    dword ptr [edx + ecx*4], eax
        inc    dword ptr [0x56b8]            ; -> loc_1_56B8
        inc    dword ptr [0x58f4]            ; -> loc_1_58F4
        pop    edi
        pop    esi
        ret    
        db  000h, 000h
sub_1_72F0:
        sub    esp, 4
        mov    word ptr [esp + 2], 0x5204
        mov    eax, dword ptr [0x58f8]       ; -> loc_1_58F8
        test   al, 0x10
        jne    loc_1_733E
        push   1
        push   0
        lea    eax, [esp + 0xa]
        push   0x30af                        ; -> loc_1_30AF
        push   eax
        call   sub_1_5206
        add    esp, 0x10
        push   0
        push   0x2ddf                        ; -> loc_1_2DDF
        push   0x6e
        call   sub_1_4AAB
        add    esp, 0xc
        mov    dword ptr [0x5690], eax       ; -> loc_1_5690
        mov    dword ptr [0x5688], 0x110     ; -> loc_1_5688
        mov    eax, dword ptr [0x58f8]       ; -> loc_1_58F8
loc_1_733E:
        or     eax, 0x1000
        mov    dword ptr [0x58f8], eax       ; -> loc_1_58F8
        add    esp, 4
        ret    
sub_1_734C:
        mov    ecx, dword ptr [0x58f8]       ; -> loc_1_58F8
        test   cl, 0x10
        jne    loc_1_7384
        push   0
        push   0x5204
        call   sub_1_041B
        add    esp, 8
        push   dword ptr [0x5690]            ; -> loc_1_5690
        call   sub_1_4AD5
        add    esp, 4
        mov    dword ptr [0x5690], 0         ; -> loc_1_5690
        mov    ecx, dword ptr [0x58f8]       ; -> loc_1_58F8
loc_1_7384:
        mov    eax, dword ptr [0x7cd8]       ; -> loc_1_7CD8
        or     eax, eax
        je     loc_1_73BF
        push   0
        push   eax
        push   dword ptr [0x7cdc]            ; -> loc_1_7CDC
        call   sub_1_246E
        mov    dword ptr [0x7cdc], 0         ; -> loc_1_7CDC
        mov    dword ptr [0x7cd8], 0         ; -> loc_1_7CD8
        mov    dword ptr [0x7cd4], 0         ; -> loc_1_7CD4
        mov    ecx, dword ptr [0x58f8]       ; -> loc_1_58F8
loc_1_73BF:
        and    ecx, 0xffffefff
        mov    dword ptr [0x58f8], ecx       ; -> loc_1_58F8
        ret    
sub_1_73CC:
        push   ebx
        push   esi
        mov    eax, dword ptr [0x7cc8]       ; -> loc_1_7CC8
        or     eax, eax
        je     loc_1_7419
        mov    edx, dword ptr [esp + 0xc]
loc_1_73DB:
        mov    cx, word ptr [edx]
        cmp    word ptr [eax + 4], cx
        jne    loc_1_7413
        lea    ecx, [eax + 6]
        lea    esi, [edx + 2]
loc_1_73EA:
        mov    bl, byte ptr [ecx]
        cmp    bl, byte ptr [esi]
        jne    loc_1_740A
        or     bl, bl
        je     loc_1_7406
        mov    bl, byte ptr [ecx + 1]
        cmp    bl, byte ptr [esi + 1]
        jne    loc_1_740A
        add    ecx, 2
        add    esi, 2
        or     bl, bl
        jne    loc_1_73EA
loc_1_7406:
        sub    ecx, ecx
        jmp    loc_1_740F
loc_1_740A:
        sbb    ecx, ecx
        sbb    ecx, -1
loc_1_740F:
        or     ecx, ecx
        je     loc_1_741B
loc_1_7413:
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    loc_1_73DB
loc_1_7419:
        sub    eax, eax
loc_1_741B:
        pop    esi
        pop    ebx
        ret    
sub_1_741E:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edx, dword ptr [esp + 0x14]
        mov    ecx, dword ptr [edx + 0x24]
        mov    dword ptr [edx + 4], 1
        lea    ebx, [ecx + 0x1e]
        mov    dword ptr [edx + 0x18], 0x79a5 ; -> loc_1_79A5
        mov    word ptr [edx + 0xa], 0x5204
        mov    dword ptr [edx + 0xc], 0
        mov    word ptr [edx + 0x22], 1
        lea    edi, [edx + 0x1c]
        mov    eax, 0xffffffff
        stosd  dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    byte ptr [ecx + 5], 4
        mov    dword ptr [ecx + 6], 0
        lea    edi, [ecx + 0xa]
        stosd  dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        cmp    dword ptr [esp + 0x1c], 0
        mov    word ptr [ecx + 0x10], 0x5204
        mov    word ptr [ebx], 0x200
        je     loc_1_74A8
        add    ebx, 2
        mov    eax, dword ptr [esp + 0x18]
        sub    ebp, ebp
        cmp    eax, ebp
        je     loc_1_74C0
loc_1_7488:
        cmp    dword ptr [0x7cd4], ebp       ; -> loc_1_7CD4
        jbe    loc_1_74C0
        lea    esi, [eax + 4]
        mov    edi, ebx
        mov    ecx, 0x10
        rep movsd dword ptr es:[edi], dword ptr [esi]
        add    ebx, 0x40
        mov    eax, dword ptr [eax]
        inc    ebp
        or     eax, eax
        jne    loc_1_7488
        jmp    loc_1_74C0
loc_1_74A8:
        mov    ebp, 1
        mov    esi, dword ptr [esp + 0x18]
        add    esi, 4
        lea    edi, [ebx + 2]
        mov    ecx, 0x10
        rep movsd dword ptr es:[edi], dword ptr [esi]
        sub    eax, eax
loc_1_74C0:
        shl    bp, 6
        add    bp, 0x20
        mov    word ptr [edx + 0x28], bp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_74D1:
        sub    esp, 4
        push   esi
        and    dword ptr [0x58f8], 0xffffbfff ; -> loc_1_58F8
        mov    eax, dword ptr [0x7cc4]       ; -> loc_1_7CC4
        cmp    dword ptr [0x7cd4], eax       ; -> loc_1_7CD4
        jae    loc_1_75AE
        cmp    dword ptr [0x7cd4], 7         ; -> loc_1_7CD4
        jae    loc_1_75AE
        cmp    dword ptr [0x7cdc], 0         ; -> loc_1_7CDC
        je     loc_1_750F
        mov    eax, dword ptr [0x7cdc]       ; -> loc_1_7CDC
        mov    byte ptr [eax + 8], 1
loc_1_750F:
        mov    esi, dword ptr [0x7cc4]       ; -> loc_1_7CC4
        shl    esi, 6
        add    esi, 0x4a
        push   -1
        push   0
        lea    eax, [esp + 0xc]
        push   esi
        push   eax
        call   sub_1_2468
        cmp    dword ptr [esp + 4], 0
        jne    loc_1_754F
        or     dword ptr [0x58f8], 0x4000    ; -> loc_1_58F8
        cmp    dword ptr [0x7cdc], 0         ; -> loc_1_7CDC
        je     loc_1_75AE
        mov    eax, dword ptr [0x7cdc]       ; -> loc_1_7CDC
        mov    byte ptr [eax + 8], 0
        jmp    loc_1_75AE
loc_1_754F:
        mov    eax, dword ptr [0x7cc4]       ; -> loc_1_7CC4
        cmp    eax, 7
        jb     loc_1_755E
        mov    eax, 7
loc_1_755E:
        mov    dword ptr [0x7cd4], eax       ; -> loc_1_7CD4
        cmp    dword ptr [0x7cd8], 0         ; -> loc_1_7CD8
        je     loc_1_757F
        push   0
        push   dword ptr [0x7cd8]            ; -> loc_1_7CD8
        push   dword ptr [0x7cdc]            ; -> loc_1_7CDC
        call   sub_1_246E
loc_1_757F:
        mov    dword ptr [0x7cd8], esi       ; -> loc_1_7CD8
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [0x7cdc], eax       ; -> loc_1_7CDC
        mov    byte ptr [eax + 8], 0
        mov    eax, dword ptr [0x7cdc]       ; -> loc_1_7CDC
        mov    word ptr [eax + 0x22], 1
        mov    eax, dword ptr [0x7cdc]       ; -> loc_1_7CDC
        mov    ecx, dword ptr [0x7cdc]       ; -> loc_1_7CDC
        add    eax, 0x2a
        mov    dword ptr [ecx + 0x24], eax
loc_1_75AE:
        pop    esi
        add    esp, 4
        ret    
sub_1_75B3:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        test   byte ptr [0x58f9], 0x10       ; -> obj1:0x58F9
        jne    loc_1_75C7
        call   sub_1_72F0
loc_1_75C7:
        mov    ebx, dword ptr [esp + 0x18]
        push   ebx
        call   sub_1_73CC
        add    esp, 4
        or     eax, eax
        je     loc_1_75E2
        mov    eax, 0x4da
        jmp    loc_1_76E2
loc_1_75E2:
        inc    dword ptr [0x7cc4]            ; -> loc_1_7CC4
        cmp    dword ptr [0x7cdc], 0         ; -> loc_1_7CDC
        je     loc_1_7608
        mov    eax, dword ptr [0x7cdc]       ; -> loc_1_7CDC
        cmp    byte ptr [eax + 8], 0
        je     loc_1_7608
        or     dword ptr [0x58f8], 0x4000    ; -> loc_1_58F8
        jmp    loc_1_760D
loc_1_7608:
        call   sub_1_74D1
loc_1_760D:
        push   -1
        push   0
        lea    eax, [esp + 0x14]
        push   0x44
        push   eax
        call   sub_1_2468
        cmp    dword ptr [esp + 0xc], 0
        jne    loc_1_762E
        mov    eax, 8
        jmp    loc_1_76E2
loc_1_762E:
        mov    edi, dword ptr [esp + 0xc]
        add    edi, 4
        mov    esi, ebx
        mov    ecx, 0xc
        rep movsd dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        mov    esi, dword ptr [0x58ec]       ; -> loc_1_58EC
        mov    edi, dword ptr [esp + 0xc]
        add    edi, 0x36
        add    esi, 0x18
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        mov    ecx, dword ptr [esp + 0xc]
        mov    ax, word ptr [ebx + 0x3c]
        mov    word ptr [ecx + 0x40], ax
        mov    eax, dword ptr [esp + 0xc]
        push   -1
        mov    word ptr [eax + 0x42], 0x100
        mov    ecx, dword ptr [esp + 0x10]
        mov    eax, dword ptr [0x7cc8]       ; -> loc_1_7CC8
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [0x7cc8], eax       ; -> loc_1_7CC8
        push   0
        lea    eax, [esp + 0x18]
        push   0x8a
        push   eax
        call   sub_1_2468
        cmp    dword ptr [esp + 0x10], 0
        je     loc_1_76E0
        mov    eax, dword ptr [esp + 0x10]
        mov    ecx, dword ptr [esp + 0x10]
        add    eax, 0x2a
        push   0
        mov    dword ptr [ecx + 0x24], eax
        push   dword ptr [esp + 0x10]
        push   dword ptr [esp + 0x18]
        call   sub_1_741E
        mov    eax, dword ptr [esp + 0x1c]
        add    esp, 0xc
        mov    dword ptr [eax + 4], 1
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x18], 0x3660 ; -> loc_1_3660
        push   dword ptr [0x58ec]            ; -> loc_1_58EC
        push   dword ptr [esp + 0x14]
        call   sub_1_4092
        add    esp, 8
loc_1_76E0:
        sub    eax, eax
loc_1_76E2:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
sub_1_76E9:
        push   esi
        push   edi
        mov    eax, dword ptr [0x7cc8]       ; -> loc_1_7CC8
        or     eax, eax
        je     loc_1_770A
loc_1_76F4:
        mov    esi, dword ptr [0x58ec]       ; -> loc_1_58EC
        lea    edi, [eax + 0x36]
        add    esi, 0x18
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    loc_1_76F4
loc_1_770A:
        pop    edi
        pop    esi
        ret    
sub_1_770D:
        sub    esp, 4
        push   esi
        test   byte ptr [0x58f9], 0x10       ; -> obj1:0x58F9
        jne    loc_1_7724
        mov    eax, 0x4db
        jmp    loc_1_77E9
loc_1_7724:
        push   dword ptr [esp + 0xc]
        call   sub_1_73CC
        add    esp, 4
        mov    esi, eax
        or     esi, esi
        jne    loc_1_7740
        mov    eax, 0x4db
        jmp    loc_1_77E9
loc_1_7740:
        mov    eax, dword ptr [0x7ccc]       ; -> loc_1_7CCC
        or     eax, eax
        je     loc_1_775B
loc_1_7749:
        cmp    dword ptr [eax + 0x10], esi
        jne    loc_1_7755
        mov    dword ptr [eax + 0x10], 0
loc_1_7755:
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    loc_1_7749
loc_1_775B:
        push   -1
        push   0
        lea    eax, [esp + 0xc]
        push   0x8a
        push   eax
        call   sub_1_2468
        cmp    dword ptr [esp + 4], 0
        je     loc_1_77C0
        mov    word ptr [esi + 0x42], 0x1000
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 4]
        add    eax, 0x2a
        push   0
        push   esi
        mov    dword ptr [ecx + 0x24], eax
        push   dword ptr [esp + 0xc]
        call   sub_1_741E
        mov    eax, dword ptr [esp + 0x10]
        add    esp, 0xc
        mov    dword ptr [eax + 4], 1
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax + 0x18], 0x3660 ; -> loc_1_3660
        push   dword ptr [0x58ec]            ; -> loc_1_58EC
        push   dword ptr [esp + 8]
        call   sub_1_4092
        add    esp, 8
loc_1_77C0:
        push   0
        push   esi
        push   0x7cc8                        ; -> loc_1_7CC8
        call   sub_1_46EA
        add    esp, 0xc
        push   0
        push   0x44
        push   esi
        call   sub_1_246E
        dec    dword ptr [0x7cc4]            ; -> loc_1_7CC4
        jne    loc_1_77E7
        call   sub_1_734C
loc_1_77E7:
        sub    eax, eax
loc_1_77E9:
        pop    esi
        add    esp, 4
        ret    
loc_1_77EE:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [0x7cdc]       ; -> loc_1_7CDC
        cmp    byte ptr [eax + 8], 0
        je     loc_1_7818
        push   0
        push   0x77ee                        ; -> loc_1_77EE
        push   0x37
        call   sub_1_4AAB
        add    esp, 0xc
        mov    dword ptr [0x7cd0], eax       ; -> loc_1_7CD0
        jmp    loc_1_78EE
loc_1_7818:
        sub    ebp, ebp
loc_1_781A:
        mov    ebx, dword ptr [0x7ccc]       ; -> loc_1_7CCC
        cmp    ebx, ebp
        je     loc_1_78E4
        mov    eax, dword ptr [ebx]
        mov    dword ptr [0x7ccc], eax       ; -> loc_1_7CCC
        cmp    dword ptr [ebx + 0x10], ebp
        je     loc_1_78C4
        mov    eax, dword ptr [0x7cdc]       ; -> loc_1_7CDC
        mov    dword ptr [eax + 0xc], ebp
        mov    eax, dword ptr [0x7cdc]       ; -> loc_1_7CDC
        mov    dword ptr [eax + 4], 1
        mov    eax, dword ptr [0x7cdc]       ; -> loc_1_7CDC
        mov    dword ptr [eax + 0x18], 0x77ee ; -> loc_1_77EE
        mov    eax, dword ptr [0x7cdc]       ; -> loc_1_7CDC
        mov    word ptr [eax + 0xa], 0x5204
        mov    eax, dword ptr [0x7cdc]       ; -> loc_1_7CDC
        lea    esi, [ebx + 8]
        mov    word ptr [eax + 0x22], 1
        mov    eax, dword ptr [0x7cdc]       ; -> loc_1_7CDC
        mov    word ptr [eax + 0x28], 0x60
        mov    eax, dword ptr [0x7cdc]       ; -> loc_1_7CDC
        mov    eax, dword ptr [eax + 0x24]
        mov    byte ptr [eax + 5], 4
        mov    edi, dword ptr [0x7cdc]       ; -> loc_1_7CDC
        add    edi, 0x1c
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        lea    esi, [ebx + 4]
        lea    edi, [eax + 6]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        add    eax, 0x1e
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    word ptr [eax], 0x400
        mov    esi, dword ptr [ebx + 0x10]
        lea    edi, [eax + 2]
        add    esi, 4
        mov    ecx, 0x10
        rep movsd dword ptr es:[edi], dword ptr [esi]
        push   ebp
        push   dword ptr [0x7cdc]            ; -> loc_1_7CDC
        call   sub_1_4092
        add    esp, 8
loc_1_78C4:
        mov    eax, dword ptr [0x58b4]       ; -> loc_1_58B4
        mov    dword ptr [ebx], eax
        lea    eax, [ebx - 0x38]
        mov    dword ptr [0x58b4], eax       ; -> loc_1_58B4
        inc    dword ptr [0x5848]            ; -> loc_1_5848
        cmp    dword ptr [ebx + 0x10], ebp
        je     loc_1_781A
        jmp    loc_1_78EE
loc_1_78E4:
        mov    dword ptr [0x7cd0], 0         ; -> loc_1_7CD0
loc_1_78EE:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_78F3:
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0x14]
        test   byte ptr [eax + 8], 8
        jne    loc_1_7992
        mov    ebx, dword ptr [0x7cc8]       ; -> loc_1_7CC8
        or     ebx, ebx
        je     loc_1_7992
        mov    edx, dword ptr [esp + 0x10]
        mov    ax, word ptr [edx + 0x20]
loc_1_791A:
        cmp    word ptr [ebx + 4], ax
        je     loc_1_7928
        mov    ebx, dword ptr [ebx]
        or     ebx, ebx
        jne    loc_1_791A
        jmp    loc_1_7992
loc_1_7928:
        cmp    dword ptr [0x5848], 0         ; -> loc_1_5848
        je     loc_1_7992
        dec    dword ptr [0x5848]            ; -> loc_1_5848
        mov    ecx, dword ptr [0x58b4]       ; -> loc_1_58B4
        mov    eax, dword ptr [ecx + 0x38]
        add    ecx, 0x38
        mov    dword ptr [0x58b4], eax       ; -> loc_1_58B4
        lea    esi, [edx + 0x12]
        lea    edi, [ecx + 4]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    dword ptr [ecx + 0x10], ebx
        mov    eax, dword ptr [0x7ccc]       ; -> loc_1_7CCC
        mov    dword ptr [ecx], eax
        mov    dword ptr [0x7ccc], ecx       ; -> loc_1_7CCC
        cmp    dword ptr [0x7cd0], 0         ; -> loc_1_7CD0
        jne    loc_1_7992
        cmp    word ptr [edx + 0x20], 0x400
        push   0
        push   0x77ee                        ; -> loc_1_77EE
        je     loc_1_7983
        call   sub_1_4AC2
        add    esp, 8
        jmp    loc_1_798D
loc_1_7983:
        push   0xa
        call   sub_1_4AAB
        add    esp, 0xc
loc_1_798D:
        mov    dword ptr [0x7cd0], eax       ; -> loc_1_7CD0
loc_1_7992:
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_7996:
        mov    dword ptr [0x7ce4], 0         ; -> loc_1_7CE4
        jmp    sub_1_79E7
loc_1_79A5:
        test   byte ptr [0x58f9], 0x40       ; -> obj1:0x58F9
        je     loc_1_79B3
        call   sub_1_74D1
loc_1_79B3:
        mov    eax, dword ptr [0x7ce0]       ; -> loc_1_7CE0
        or     eax, eax
        je     loc_1_79E6
        push   1
        push   eax
        push   dword ptr [0x7cdc]            ; -> loc_1_7CDC
        call   sub_1_741E
        add    esp, 0xc
        mov    dword ptr [0x7ce0], eax       ; -> loc_1_7CE0
        push   dword ptr [0x58ec]            ; -> loc_1_58EC
        push   dword ptr [0x7cdc]            ; -> loc_1_7CDC
        call   sub_1_4092
        add    esp, 8
loc_1_79E6:
        ret    
sub_1_79E7:
        cmp    dword ptr [0x7ce4], 0         ; -> loc_1_7CE4
        je     loc_1_7A08
        push   dword ptr [0x7ce4]            ; -> loc_1_7CE4
        call   sub_1_4AD5
        add    esp, 4
        mov    dword ptr [0x7ce4], 0         ; -> loc_1_7CE4
loc_1_7A08:
        cmp    dword ptr [0x7cd4], 0         ; -> loc_1_7CD4
        je     loc_1_7A75
        mov    eax, dword ptr [0x7cdc]       ; -> loc_1_7CDC
        cmp    byte ptr [eax + 8], 0
        je     loc_1_7A37
        push   0
        push   0x7996                        ; -> loc_1_7996
        push   0x96
        call   sub_1_4AAB
        add    esp, 0xc
        mov    dword ptr [0x7ce4], eax       ; -> loc_1_7CE4
        jmp    loc_1_7A75
loc_1_7A37:
        mov    eax, dword ptr [0x7ce8]       ; -> loc_1_7CE8
        inc    dword ptr [0x7ce8]            ; -> loc_1_7CE8
        test   al, 1
        jne    loc_1_7A75
        push   1
        push   dword ptr [0x7cc8]            ; -> loc_1_7CC8
        push   dword ptr [0x7cdc]            ; -> loc_1_7CDC
        call   sub_1_741E
        add    esp, 0xc
        mov    dword ptr [0x7ce0], eax       ; -> loc_1_7CE0
        push   dword ptr [0x58ec]            ; -> loc_1_58EC
        push   dword ptr [0x7cdc]            ; -> loc_1_7CDC
        call   sub_1_4092
        add    esp, 8
loc_1_7A75:
        ret    
        db  000h, 000h
loc_1_7A78:
        dec    ebp
        inc    ecx
        pop    eax
        push   ebx
        dec    edi
        inc    ebx
        dec    ebx
        inc    ebp
        push   esp
        push   ebx
        add    byte ptr [eax], al
loc_1_7A84:
        dec    ebp
        inc    ecx
        pop    eax
        inc    ebx
        dec    edi
        dec    esi
        dec    esi
        inc    ebp
        inc    ebx
        push   esp
        add    byte ptr [eax], al
loc_1_7A90:
        inc    ebx
        inc    ecx
        inc    ebx
        dec    eax
        inc    ebp
        push   ebx
        dec    ecx
        pop    edx
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [edx + 0x49], dl
        push   eax
        pop    edi
        push   esp
        inc    ecx
        inc    edx
        dec    esp
        inc    ebp
        pop    edi
        push   ebx
        dec    ecx
        pop    edx
        inc    ebp
        add    byte ptr [eax], al
loc_1_7AAC:
        push   edx
        dec    edi
        push   ebp
        push   esp
        inc    ebp
        push   edx
        pop    edi
        dec    ebp
        inc    ecx
        pop    eax
        add    byte ptr [eax], al
loc_1_7AB8:
        push   ebx
        inc    ecx
        push   eax
        pop    edi
        push   esp
        inc    ecx
        inc    edx
        dec    esp
        inc    ebp
        pop    edi
        push   ebx
        dec    ecx
        pop    edx
        inc    ebp
        add    byte ptr [eax], al
loc_1_7AC8:
        push   ebx
        inc    ecx
        push   eax
        pop    edi
        dec    ebp
        inc    ecx
        pop    eax
        db  000h
sub_1_7AD0:
        push   edx
        dec    ecx
        push   eax
        pop    edi
        inc    edx
        push   ebp
        inc    esi
        inc    esi
        inc    ebp
        push   edx
        pop    edi
        push   ebx
        dec    ecx
        pop    edx
        inc    ebp
        add    byte ptr [edx + 0x49], dl
        push   eax
        pop    edi
        inc    edx
        push   ebp
        inc    esi
        inc    esi
        inc    ebp
        push   edx
        push   ebx
        add    byte ptr [ebp + 0x4e], al
        inc    ecx
        inc    edx
        dec    esp
        inc    ebp
        pop    edi
        push   esp
        pop    ecx
        push   eax
        inc    ebp
        pop    edi
        xor    dh, byte ptr [eax]
        add    byte ptr [eax], al
loc_1_7AFC:
        inc    esi
        push   edx
        inc    ecx
        dec    ebp
        inc    ebp
        pop    edi
        inc    esp
        inc    ebp
        push   esp
        inc    ebp
        inc    ebx
        push   esp
        dec    ecx
        dec    edi
        dec    esi
        pop    edi
        push   eax
        inc    ebp
        push   edx
        dec    ecx
        dec    edi
        inc    esp
        add    byte ptr [eax], al
loc_1_7B14:
        js     loc_1_7B90
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        inc    dword ptr [eax]
        add    byte ptr [eax], al
        and    byte ptr [eax], al
        add    byte ptr [eax], al
        and    al, 0x58
        add    byte ptr [eax], al
        test   byte ptr [edx], bh
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x10000000], al
        add    byte ptr [eax], al
        add    byte ptr [eax], ch            ; -> loc_1_5828
        pop    eax
        add    byte ptr [eax], al
        nop    
        jp     loc_1_7B3F
loc_1_7B3F:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x58], bh     ; -> loc_1_5878
        add    byte ptr [eax], al
        pushfd 
        jp     loc_1_7B53
loc_1_7B53:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        fcom   qword ptr [ebp]
        add    byte ptr [edx + edi*2 + 0x100000], ch ; -> loc_1_7AAC
        add    byte ptr [eax], al
        add    byte ptr [eax], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], ah
        add    byte ptr [eax], al
        loopne loc_1_7BCB
        add    byte ptr [eax], al
        mov    eax, 0x7a
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    ah, ah                        ; -> obj1:0x55E4
        push   ebp
        add    byte ptr [eax], al
        enter  0x7a, 0
loc_1_7B90:
        adc    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], ah
        add    byte ptr [eax], al
        call   sub_1_7AD0
        jp     loc_1_7BA3
loc_1_7BA3:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        adc    al, byte ptr [eax]
        add    byte ptr [eax - 0x53fffffc], cl ; -> loc_1_7CAC
        jl     loc_1_7BB3
loc_1_7BB3:
        add    al, ah                        ; -> obj1:0x7AE0
        jp     loc_1_7BB7
loc_1_7BB7:
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], dl
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], al
        add    byte ptr [eax], al
        mov    ah, 0x7c
        add    byte ptr [eax], al
        in     al, dx
        jp     loc_1_7BCB
loc_1_7BCB:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    ah, ch                        ; -> obj1:0x55EC
        push   ebp
        add    byte ptr [eax], al
        cld    
        jp     loc_1_7BDF
loc_1_7BDF:
        add    byte ptr [eax + eax], ah
        add    byte ptr [eax - 0x23ffffed], cl
        add    byte ptr [eax], al
        add    byte ptr [eax + ebx*2], bh    ; -> loc_1_583C
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7C04:
        dec    esi
        push   edi
        dec    esp
        dec    ecx
        dec    esi
        dec    ebx
        add    byte ptr [eax], al
loc_1_7C0C:
        pop    es
        add    byte ptr [edi], al
        add    byte ptr [esp + edi*2], al    ; -> loc_1_7C04
        add    byte ptr [eax], al
loc_1_7C14:
        push   ebx
        inc    ecx
        push   eax
        pop    edi
        push   esp
        pop    ecx
        push   eax
        inc    ebp
        push   ebx
        pop    edi
        dec    ecx
        dec    esi
        inc    ebx
        dec    esp
        push   ebp
        inc    esp
        inc    ebp
        add    byte ptr [eax], al
        db  000h
loc_1_7C28:
        push   ebx
        inc    ecx
        push   eax
        pop    edi
        push   esp
        pop    ecx
        push   eax
        inc    ebp
        push   ebx
        pop    edi
        inc    ebp
        pop    eax
        inc    ebx
        dec    esp
        push   ebp
        inc    esp
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], ah
        dec    esi
        inc    esp
        dec    ecx
        push   ebx
        and    byte ptr [ebx], dh
        xor    dword ptr cs:[eax], esp
        inc    esp
        push   edx
        dec    ecx
        push   esi
        inc    ebp
        push   edx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7C60:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7C64:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7C68:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_1_7C6C:
        inc    esi
        push   edx
        inc    ecx
        dec    ebp
        inc    ebp
        pop    edi
        push   esp
        pop    ecx
        push   eax
        inc    ebp
        add    byte ptr [eax], al
loc_1_7C78:
        or     eax, dword ptr [eax]
        or     eax, dword ptr [eax]
        insb   byte ptr es:[edi], dx
        jl     loc_1_7C7F
loc_1_7C7F:
        add    byte ptr [esi + 0x45], cl
        push   esp
        push   edi
        dec    edi
        push   edx
        dec    ebx
        pop    edi
        dec    ecx
        inc    esp
        add    byte ptr [eax], al
loc_1_7C8C:
        or     eax, dword ptr [eax]
        or     eax, dword ptr [eax]
        cmp    byte ptr [eax + eax], 0x46
        dec    esp
        inc    ecx
        inc    edi
        push   ebx
        add    byte ptr [eax], al
        add    byte ptr [esi], al
        add    byte ptr [esi], al
        add    byte ptr [esp + edi*2], dl    ; -> obj1:0x7C94
        add    byte ptr [eax], al
loc_1_7CA8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7CAC:
        mov    byte ptr [eax + eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], al
        add    byte ptr [eax], al
loc_1_7CB8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7CBC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7CC0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7CC4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7CC8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7CCC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7CD0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7CD4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7CD8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7CDC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7CE0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7CE4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7CE8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7CEC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7CF0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7CF4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7CF8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7CFC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7D00:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7D04:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7D08:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7D0C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7D10:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7D14:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7D18:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7D1C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7D20:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_7D24:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x2fc  R-X
        db  01Dh, 000h, 000h, 000h
loc_2_0004:
        dec    eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        db  000h
loc_2_000E:
        sbb    al, 0
        and    eax, dword ptr [eax]
        and    eax, 0x32003000
        add    byte ptr [eax + eax], bh
        push   esp
        add    byte ptr [edi], bh
        lea    eax, [eax]
        test   eax, 0xea00da00
        add    byte ptr [edx], al
        add    dword ptr [eax + 1], ebp
        js     0x2d
        xchg   byte ptr [ecx], al
        popfd  
        add    dword ptr [ebp - 0x22fe3aff], ebp
        add    ebx, ebp
        add    dword ptr [edi], eax
        add    ah, byte ptr [ebx]
        add    ch, byte ptr [edi]
        add    al, byte ptr [ebp + 2]
        jo     loc_2_0044
        jnp    loc_2_0046
loc_2_0044:
        lea    eax, [edx]
loc_2_0046:
        mov    ah, 2
loc_2_0048:
        dec    ecx
        push   eax
        pop    eax
        das    
        push   ebx
        push   eax
        pop    eax
        and    byte ptr [ebx + 0x6f], ah
        insd   dword ptr es:[edi], dx
        jo     loc_2_00B6
        je     loc_2_00C0
        bound  ebp, qword ptr [ebp + 0x20]
        jo     0xcf
        outsd  dx, dword ptr [esi]
        je     0xcf
        arpl   word ptr [edi + 0x6c], bp
        add    byte ptr [esi + 0x77], cl
        insb   byte ptr es:[edi], dx
        imul   ebp, dword ptr [esi + 0x6b], 0x70002000
        jb     0x91
        jae    loc_2_00D8
        arpl   word ptr [edi + 0x6e], bp
        add    byte ptr fs:[eax], ah
        add    byte ptr [ecx + 0x50], cl
        pop    eax
        pop    edi
        push   ebx
        outsb  dx, byte ptr gs:[esi]
        jae    loc_2_0084
loc_2_0084:
        dec    ecx
        push   eax
        pop    eax
        and    byte ptr [eax + 0x61], dh
        arpl   word ptr [ebx + 0x65], bp
        je     loc_2_0102
        and    byte ptr [ebx + 0x65], dh
        outsb  dx, byte ptr [esi]
        je     0xc4
        jae    loc_2_00FC
        arpl   word ptr [edi + 0x6e], bp
        add    byte ptr fs:[ecx + 0x50], cl
        pop    eax
        and    byte ptr [eax + 0x61], dh
        arpl   word ptr [ebx + 0x65], bp
        je     0x11a
        and    byte ptr [ebx + 0x65], dh
        outsb  dx, byte ptr [esi]
        je     loc_2_00CD
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        and    byte ptr [eax + ebp*2 + 0x65], dh
        and    byte ptr [esi + 0x65], ch
loc_2_00B6:
        je     loc_2_012F
        outsd  dx, dword ptr [esi]
        jb     loc_2_0126
        and    byte ptr [eax + 0x65], dh
        jb     loc_2_00E0
loc_2_00C0:
        jae    loc_2_0127
        arpl   word ptr [edi + 0x6e], bp
        add    byte ptr fs:[ecx + 0x50], cl
        pop    eax
        pop    edi
        push   eax
        popal  
loc_2_00CD:
        arpl   word ptr [ebx + 0x65], bp
        je     loc_2_0145
        dec    ecx
        outsb  dx, byte ptr [esi]
        add    byte ptr [ecx + 0x50], cl
        pop    eax
loc_2_00D8:
        and    byte ptr [eax + 0x61], dh
        arpl   word ptr [ebx + 0x65], bp
        je     loc_2_0153
loc_2_00E0:
        and    byte ptr [edx + 0x65], dh
        arpl   word ptr [ebp + 0x69], sp
        jbe    loc_2_014D
        das    
        jae    0x151
        arpl   word ptr [edi + 0x6e], bp
        add    byte ptr fs:[ecx + 0x50], cl
        pop    eax
        and    byte ptr [eax + 0x61], dh
        arpl   word ptr [ebx + 0x65], bp
        je     loc_2_016F
loc_2_00FC:
        and    byte ptr [edx + 0x65], dh
        arpl   word ptr [ebp + 0x69], sp
loc_2_0102:
        jbe    0x169
        and    byte ptr fs:[esi + 0x72], ah
        outsd  dx, dword ptr [esi]
        insd   dword ptr es:[edi], dx
        and    byte ptr [eax + ebp*2 + 0x65], dh
        and    byte ptr [esi + 0x65], ch
        je     0x18a
        outsd  dx, dword ptr [esi]
        jb     0x181
        and    byte ptr [eax + 0x65], dh
        jb     loc_2_013B
        jae    0x182
        arpl   word ptr [edi + 0x6e], bp
        add    byte ptr fs:[ecx + 0x50], cl
        pop    eax
        pop    edi
loc_2_0126:
        push   eax
loc_2_0127:
        popal  
        arpl   word ptr [ebx + 0x65], bp
        je     loc_2_01A0
        dec    esp
        outsd  dx, dword ptr [esi]
loc_2_012F:
        jae    0x1a5
        add    byte ptr [ecx + 0x50], cl
        pop    eax
        and    byte ptr [eax + 0x61], dh
        arpl   word ptr [ebx + 0x65], bp
loc_2_013B:
        je     0x1b0
        and    byte ptr [edi + ebp*2 + 0x73], ch
        je     0x172
        jae    loc_2_01AA
loc_2_0145:
        arpl   word ptr [edi + 0x6e], bp
        add    byte ptr fs:[ecx + 0x50], cl
        pop    eax
loc_2_014D:
        and    byte ptr [eax + 0x61], dh
        arpl   word ptr [ebx + 0x65], bp
loc_2_0153:
        je     0x1c8
        and    byte ptr [edx + 0x65], dh
        arpl   word ptr [ebp + 0x69], sp
        jbe    loc_2_01C2
        and    byte ptr fs:[esi + 0x72], ah
        outsd  dx, dword ptr [esi]
        insd   dword ptr es:[edi], dx
        and    byte ptr [eax + ebp*2 + 0x65], dh
        and    byte ptr [esi + 0x65], ch
        je     0x1e3
        outsd  dx, dword ptr [esi]
        jb     loc_2_01DA
loc_2_016F:
        and    byte ptr [eax + ebp*2 + 0x61], dh
        je     0x195
        ja     loc_2_01DC
        jb     0x1de
        and    byte ptr [edx + esi*2 + 0x6f], ah
        jo     loc_2_01EF
        and    byte ptr fs:[edx + 0x65], ah
        arpl   word ptr [ecx + 0x75], sp
        jae    loc_2_01EE
        and    byte ptr [esi + 0x6f], ch
        bound  ebp, qword ptr [edi + 0x64]
        jns    0x1b1
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        and    byte ptr [eax + ebp*2 + 0x65], dh
        and    byte ptr [edi + ebp*2 + 0x63], ch
        popal  
        insb   byte ptr es:[edi], dx
        and    byte ptr [ebx + 0x79], dh
loc_2_01A0:
        jae    loc_2_0216
        insd   dword ptr es:[edi], dx
        and    byte ptr [edi + 0x61], dh
        outsb  dx, byte ptr [esi]
        je     0x21d
loc_2_01AA:
        and    byte ptr [eax + ebp*2 + 0x65], dh
        insd   dword ptr es:[edi], dx
        add    byte ptr [ecx + 0x50], cl
        pop    eax
        pop    edi
        push   ebx
        outsd  dx, dword ptr [esi]
        arpl   word ptr [ebx + 0x65], bp
        je     loc_2_022E
        dec    edi
        jo     loc_2_0223
        outsb  dx, byte ptr [esi]
        add    byte ptr [edi + 0x70], cl
loc_2_01C2:
        outsb  dx, byte ptr gs:[esi]
        and    byte ptr [ebx + 0x6f], dh
        arpl   word ptr [ebx + 0x65], bp
        je     0x23f
        and    byte ptr [eax], al
        dec    esi
        jne    loc_2_023E
        bound  esp, qword ptr [ebp + 0x72]
        and    byte ptr [edi + 0x66], ch
        and    byte ptr [edi + 0x70], ch
loc_2_01DA:
        outsb  dx, byte ptr gs:[esi]
loc_2_01DC:
        and    byte ptr [ebx + 0x6f], dh
        arpl   word ptr [ebx + 0x65], bp
        je     0x257
        add    byte ptr [ebx + 0x50], dl
        pop    eax
        pop    edi
        push   ebx
        outsb  dx, byte ptr gs:[esi]
        push   eax
loc_2_01EE:
        popal  
loc_2_01EF:
        arpl   word ptr [ebx + 0x65], bp
        je     0x267
        add    byte ptr [ebx + 0x50], dl
        pop    eax
        and    byte ptr [eax + 0x61], dh
        arpl   word ptr [ebx + 0x65], bp
        je     loc_2_0273
        and    byte ptr [ebx + 0x65], dh
        outsb  dx, byte ptr [esi]
        je     loc_2_0235
        jae    loc_2_026D
        arpl   word ptr [edi + 0x6e], bp
        add    byte ptr fs:[ebx + 0x50], dl
        pop    eax
        and    byte ptr [eax + 0x61], dh
        arpl   word ptr [ebx + 0x65], bp
loc_2_0216:
        je     0x28b
        and    byte ptr [ebx + 0x65], dh
        outsb  dx, byte ptr [esi]
        je     loc_2_024D
        jae    loc_2_0285
        arpl   word ptr [edi + 0x6e], bp
loc_2_0223:
        add    byte ptr fs:[ebx + 0x50], dl
        pop    eax
        pop    edi
        push   eax
        popal  
        arpl   word ptr [ebx + 0x65], bp
loc_2_022E:
        je     0x2a3
        dec    ecx
        outsb  dx, byte ptr [esi]
        add    byte ptr [ebx + 0x50], dl
loc_2_0235:
        pop    eax
        and    byte ptr [eax + 0x61], dh
        arpl   word ptr [ebx + 0x65], bp
        je     0x2b1
loc_2_023E:
        and    byte ptr [edx + 0x65], dh
        arpl   word ptr [ebp + 0x69], sp
        jbe    0x2ab
        das    
        jae    0x2af
        arpl   word ptr [edi + 0x6e], bp
loc_2_024D:
        add    byte ptr fs:[ebx + 0x50], dl
        pop    eax
        and    byte ptr [eax + 0x61], dh
        arpl   word ptr [ebx + 0x65], bp
        je     0x2cd
        and    byte ptr [edx + 0x65], dh
        arpl   word ptr [ebp + 0x69], sp
        jbe    0x2c7
        das    
        jae    loc_2_02CB
        arpl   word ptr [edi + 0x6e], bp
        add    byte ptr fs:[edx + 0x49], dl
loc_2_026D:
        push   esp
        pop    edi
        inc    ebp
        outsb  dx, byte ptr [esi]
        je     0x2e5
loc_2_0273:
        imul   esp, dword ptr [ebp + 0x73], 0x756f5200
        je     0x2e5
        outsb  dx, byte ptr [esi]
        and    byte ptr [si + 0x61], dl
        bound  ebp, qword ptr [ebp + 0x20]
loc_2_0285:
        outsb  dx, byte ptr gs:[esi]
        je     0x2fb
        imul   esp, dword ptr [ebp + 0x73], 0x6d754e00
        bound  esp, qword ptr [ebp + 0x72]
        and    byte ptr [edi + 0x66], ch
        and    byte ptr [ecx + 0x50], cl
        pop    eax
        and    byte ptr [ecx + 0x6e], cl
        je     0x304
        jb     0x30f
        je     0x31b
        outsd  dx, dword ptr [esi]
        jb     0x312
        imul   ebp, dword ptr [esi + 0x67], 0x756f7220
        je     0x315
        jae    0x2d2
        imul   ebp, dword ptr [esi + 0x6f], 0x77
        outsb  dx, byte ptr [esi]
        add    byte ptr [ebx + 0x49], dl
        push   esp
        inc    ebp
        outsb  dx, byte ptr [esi]
        je     0x331
        imul   esp, dword ptr [ebp + 0x73], 0x50415300
        and    byte ptr [ecx + 0x62], dl
        insb   byte ptr es:[edi], dx
loc_2_02CB:
        and    byte ptr gs:[ebp + 0x6e], ah
        je     0x343
        imul   esp, dword ptr [ebp + 0x73], 0x6d754e00
        bound  esp, qword ptr [ebp + 0x72]
        and    byte ptr [edi + 0x66], ch
        and    byte ptr [ebx + 0x65], dh
        jb     0x359
        imul   esp, dword ptr [ebx + 0x65], 0x76646120
        jb     0x361
        imul   esi, dword ptr [ebx + 0x65], 0x746e656d
        jae    0x316
        imul   ebp, dword ptr [esi + 0x6f], 0x77
        outsb  dx, byte ptr [esi]
        db  000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x8cc  R-X
        db  083h, 0ECh, 004h, 06Ah, 042h, 0FFh, 074h, 024h, 00Ch, 08Dh, 044h, 024h, 008h, 068h
        dd  obj1:0x5814              ; fixup
        db  050h, 0E8h
        dd  loc_1_24AA               ; fixup
        db  083h, 07Ch, 024h, 000h, 001h, 01Bh, 0C0h, 040h, 083h, 0C4h, 004h, 0C3h
sub_3_0024:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [0x58a4], 0x58a4    ; -> loc_1_58A4
        je     loc_3_0060
        mov    esi, dword ptr [0x58a4]       ; -> loc_1_58A4
        mov    eax, esi
        mov    ecx, dword ptr [eax]
        mov    dword ptr [ecx + 4], 0x58a4   ; -> loc_1_58A4
        mov    eax, dword ptr [eax]
        mov    dword ptr [0x58a4], eax       ; -> loc_1_58A4
        push   dword ptr [esi + 8]
        call   sub_1_247A
        push   0
        push   0xc
        push   esi
        call   sub_1_246E
loc_3_0060:
        cmp    dword ptr [0x58ac], 0x58ac    ; -> loc_1_58AC
        je     loc_3_0096
        mov    esi, dword ptr [0x58ac]       ; -> loc_1_58AC
        mov    eax, esi
        mov    ecx, dword ptr [eax]
        mov    dword ptr [ecx + 4], 0x58ac   ; -> loc_1_58AC
        mov    eax, dword ptr [eax]
        mov    dword ptr [0x58ac], eax       ; -> loc_1_58AC
        push   dword ptr [esi + 8]
        call   sub_1_248C
        push   0
        push   0xc
        push   esi
        call   sub_1_246E
loc_3_0096:
        mov    esi, dword ptr [esp + 0x14]
        or     esi, esi
        je     loc_3_00C2
        mov    edi, 0x58cc                   ; -> loc_1_58CC
        mov    ebx, 1
loc_3_00A8:
        mov    eax, dword ptr [edi]
        test   dword ptr [eax + 4], ebx
        je     loc_3_00BC
        push   dword ptr [eax + 0xc]
        lea    eax, [esp + 0x10]
        push   eax
        call   sub_1_24BC
loc_3_00BC:
        add    edi, 4
        dec    esi
        jne    loc_3_00A8
loc_3_00C2:
        cmp    dword ptr [0x5818], 0         ; -> obj1:0x5818
        je     loc_3_00DE
        push   0
        push   dword ptr [0x581c]            ; -> obj1:0x581C
        push   dword ptr [0x5818]            ; -> obj1:0x5818
        call   sub_1_246E
loc_3_00DE:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_3_00E5:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        sub    ecx, ecx
        mov    word ptr [esi], 0
        mov    edx, dword ptr [esp + 0xc]
loc_3_00F6:
        mov    al, byte ptr [edx]
        inc    edx
        cmp    al, 0x30
        jl     loc_3_0105
        cmp    al, 0x39
        jg     loc_3_0105
        sub    al, 0x30
        jmp    loc_3_011B
loc_3_0105:
        cmp    al, 0x41
        jl     loc_3_0111
        cmp    al, 0x46
        jg     loc_3_0111
        sub    al, 0x37
        jmp    loc_3_011B
loc_3_0111:
        cmp    al, 0x61
        jl     loc_3_0132
        cmp    al, 0x66
        jg     loc_3_0132
        sub    al, 0x57
loc_3_011B:
        mov    di, word ptr [esi]
        shl    di, 4
        movsx  ax, al
        add    di, ax
        mov    word ptr [esi], di
        inc    ecx
        cmp    ecx, 4
        jl     loc_3_00F6
loc_3_0132:
        or     ecx, ecx
        mov    eax, 0xffffffff
        je     loc_3_013D
        sub    eax, eax
loc_3_013D:
        pop    edi
        pop    esi
        ret    
sub_3_0140:
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edx, dword ptr [esp + 0x2c]
        mov    edi, edx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        mov    word ptr [esp + 0x1e], cx
        mov    ax, cx
        mov    word ptr [esp + 0x1c], ax
        mov    dword ptr [esp + 0x20], edx
        lea    eax, [esp + 0x1c]
        push   2
        push   eax
        lea    eax, [esp + 0x1c]
        push   dword ptr [esp + 0x30]
        push   eax
        lea    eax, [esp + 0x28]
        push   eax
        call   sub_1_249E
        cmp    dword ptr [esp + 0x18], 0
        jne    loc_3_0229
        mov    eax, dword ptr [esp + 0x14]
        mov    edi, dword ptr [eax + 8]
        movzx  eax, word ptr [eax + 4]
        or     edi, edi
        je     loc_3_0229
        sub    ebx, ebx
        sub    ecx, ecx
        cmp    eax, ecx
        jle    loc_3_01B5
loc_3_01A9:
        cmp    byte ptr [edi + ecx], 0x2c
        jne    loc_3_01B0
        inc    ebx
loc_3_01B0:
        inc    ecx
        cmp    ecx, eax
        jl     loc_3_01A9
loc_3_01B5:
        push   -1
        push   0
        lea    eax, [ebx*2 + 2]
        push   eax
        inc    ebx
        push   dword ptr [esp + 0x40]
        call   sub_1_2468
        mov    eax, dword ptr [esp + 0x34]
        cmp    dword ptr [eax], 0
        je     loc_3_0229
        sub    ebp, ebp
        cmp    ebx, ebp
        jle    loc_3_0229
        mov    esi, dword ptr [esp + 0x30]
loc_3_01DE:
        cmp    byte ptr [edi], 0x2c
        je     loc_3_0223
        lea    eax, [esp + 0x12]
        push   eax
        push   edi
        call   sub_3_00E5
        add    esp, 8
        or     eax, eax
        jne    loc_3_0229
        mov    ah, byte ptr [esp + 0x12]
        mov    al, byte ptr [esp + 0x13]
        mov    word ptr [esp + 0x12], ax
        mov    cx, ax
        mov    eax, dword ptr [esp + 0x34]
        mov    edx, dword ptr [esi]
        mov    eax, dword ptr [eax]
        mov    word ptr [eax + edx*2], cx
        inc    dword ptr [esi]
        cmp    byte ptr [edi], 0x2c
        je     loc_3_0223
loc_3_0218:
        cmp    byte ptr [edi], 0
        je     loc_3_0223
        inc    edi
        cmp    byte ptr [edi], 0x2c
        jne    loc_3_0218
loc_3_0223:
        inc    ebp
        inc    edi
        cmp    ebp, ebx
        jl     loc_3_01DE
loc_3_0229:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
sub_3_0231:
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        lea    eax, [esp + 0x14]
        push   0x7c0c                        ; -> loc_1_7C0C
        push   eax
        lea    eax, [esp + 0x14]
        push   eax
        call   sub_1_24C8
        cmp    dword ptr [esp + 0xc], 0
        je     loc_3_025C
        mov    eax, 1
        jmp    loc_3_0342
loc_3_025C:
        mov    esi, 0x7b14                   ; -> loc_1_7B14
        cmp    dword ptr [0x7b14], 0         ; -> loc_1_7B14
        je     loc_3_02DE
        sub    ebx, ebx
loc_3_026C:
        mov    edi, dword ptr [esi]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        mov    word ptr [esp + 0x1a], cx
        mov    ax, cx
        mov    word ptr [esp + 0x18], ax
        mov    eax, dword ptr [esi]
        mov    dword ptr [esp + 0x1c], eax
        mov    ecx, dword ptr [esi + 0x10]
        mov    eax, dword ptr [esi + 0xc]
        mov    dword ptr [ecx], eax
        lea    eax, [esp + 0x18]
        push   ebx
        push   eax
        lea    eax, [esp + 0x18]
        push   dword ptr [esp + 0x1c]
        push   eax
        lea    eax, [esp + 0x1c]
        push   eax
        call   sub_1_249E
        cmp    dword ptr [esp + 0xc], ebx
        jne    loc_3_02D7
        mov    eax, dword ptr [esp + 0x10]
        cmp    esi, 0x7b14                   ; -> loc_1_7B14
        mov    eax, dword ptr [eax + 4]
        jne    loc_3_02C8
        add    eax, 3
        and    eax, 0xfffffffc
loc_3_02C8:
        cmp    dword ptr [esi + 4], eax
        ja     loc_3_02D7
        cmp    dword ptr [esi + 8], eax
        jb     loc_3_02D7
        mov    ecx, dword ptr [esi + 0x10]
        mov    dword ptr [ecx], eax
loc_3_02D7:
        add    esi, 0x14
        cmp    dword ptr [esi], ebx
        jne    loc_3_026C
loc_3_02DE:
        push   0x56e8                        ; -> loc_1_56E8
        push   0x56e0                        ; -> loc_1_56E0
        push   0x7c14                        ; -> loc_1_7C14
        push   dword ptr [esp + 0x20]
        call   sub_3_0140
        add    esp, 0x10
        cmp    dword ptr [0x56e0], 0         ; -> loc_1_56E0
        jne    loc_3_032F
        push   0x56ec                        ; -> loc_1_56EC
        push   0x56e4                        ; -> loc_1_56E4
        push   0x7c28                        ; -> loc_1_7C28
        push   dword ptr [esp + 0x20]
        call   sub_3_0140
        add    esp, 0x10
        cmp    dword ptr [0x56e0], 0         ; -> loc_1_56E0
        jne    loc_3_032F
        cmp    dword ptr [0x56e4], 0         ; -> loc_1_56E4
        je     loc_3_0339
loc_3_032F:
        or     dword ptr [0x58f8], 0x10000   ; -> loc_1_58F8
loc_3_0339:
        push   dword ptr [esp + 0x14]
        call   sub_1_24A4
loc_3_0342:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
        db  000h, 000h, 000h, 00Bh, 0D2h, 074h, 00Ah, 00Fh, 0B7h, 0C2h, 032h, 0F6h, 0C1h, 0EAh, 00Ch, 003h
        db  0D0h, 089h, 015h
        dd  loc_1_5918               ; fixup
        db  081h, 00Dh
        dd  loc_1_58F8               ; fixup
        db  000h, 001h, 000h, 000h, 0CDh, 020h, 000h, 000h, 028h, 000h, 0C3h
loc_3_0371:
        test   dword ptr [0x58f8], 0x101     ; -> loc_1_58F8
        jne    loc_3_0487
        or     dword ptr [0x58f8], 0x100     ; -> loc_1_58F8
        call   sub_3_07DB
        rol    eax, 0x11
        or     dword ptr [0x58f8], eax       ; -> loc_1_58F8
        call   sub_3_06F9
        rol    eax, 0x11
        or     dword ptr [0x58f8], eax       ; -> loc_1_58F8
        call   sub_3_0698
        or     eax, eax
        je     loc_3_03C0
        test   dword ptr [0x58f8], 0x20000   ; -> loc_1_58F8
        je     loc_3_049A
loc_3_03C0:
        VxDCall  VPOWERD._VPOWERD_Get_Version    ; cd2000002800
        jb     loc_3_049A
        VxDCall  Dev_0492.Service_0000h          ; cd2000009204
        jb     loc_3_03DE
        or     dword ptr [0x58f8], 0x200     ; -> loc_1_58F8
loc_3_03DE:
        push   0x887                         ; -> loc_3_0887
        call   0
        pop    edx
        or     eax, eax
        jne    loc_3_049A
        call   sub_3_0231
        or     eax, eax
        jne    loc_3_0495
        call   sub_4_0000
        or     eax, eax
        jne    loc_3_0495
        call   sub_3_0586
        push   0
        push   0xc
        VMMCall  VMM._Allocate_Device_CB_Area    ; cd20a7000100
        add    esp, 8
        or     eax, eax
        je     loc_3_0489
        mov    dword ptr [0x5820], eax       ; -> obj1:0x5820
        call   sub_3_04C2
        push   4
        push   8
        VMMCall  VMM._Allocate_Global_V86_Data_Area ; cd20a8000100
        add    esp, 8
        or     eax, eax
        je     loc_3_0495
        mov    dword ptr [eax + 1], 0xea
        mov    edi, dword ptr [0x58fc]       ; -> loc_1_58FC
        mov    dword ptr [eax + 2], edi
        mov    word ptr [eax + 6], 0x1e03
        shl    eax, 0xc
        inc    eax
        mov    dword ptr [0x58fc], eax       ; -> loc_1_58FC
        add    eax, 5
        mov    dword ptr [0x5530], eax       ; -> obj1:0x5530
        call   sub_3_049C
        cmp    dword ptr [0x5918], 0         ; -> loc_1_5918
        je     loc_3_0478
        call   sub_3_063B
        jb     loc_3_0489
loc_3_0478:
        call   sub_1_4EC4
        or     dword ptr [0x58f8], 0x2000    ; -> loc_1_58F8
loc_3_0487:
        clc    
        ret    
loc_3_0489:
        push   dword ptr [0x5834]            ; -> loc_1_5834
        call   sub_3_0024
        pop    eax
loc_3_0495:
        call   sub_1_1163
loc_3_049A:
        stc    
        ret    
sub_3_049C:
        xor    eax, eax
        mov    dword ptr [0x58c0], 0x37      ; -> loc_1_58C0
        mov    dword ptr [0x5890], eax       ; -> loc_1_5890
        mov    eax, 0x5894                   ; -> loc_1_5894
        mov    dword ptr [eax], eax
        mov    dword ptr [eax + 4], eax
        mov    eax, 0x589c                   ; -> loc_1_589C
        mov    dword ptr [eax], eax
        mov    dword ptr [eax + 4], eax
        ret    
sub_3_04C2:
        mov    eax, 0x2f
        mov    esi, 0x386f                   ; -> loc_1_386F
        VMMCall  VMM.Hook_V86_Int_Chain          ; cd2041000100
        mov    eax, 0x7a
        mov    esi, 0x37a8                   ; -> loc_1_37A8
        VMMCall  VMM.Hook_V86_Int_Chain          ; cd2041000100
        mov    esi, 0x37a2                   ; -> loc_1_37A2
        VMMCall  VMM.Allocate_V86_Call_Back      ; cd200b000100
        mov    dword ptr [0x58fc], eax       ; -> loc_1_58FC
        mov    eax, 0x2f
        VMMCall  VMM.Get_PM_Int_Vector           ; cd2044000100
        mov    dword ptr [0x5904], ecx       ; -> loc_1_5904
        mov    dword ptr [0x5908], edx       ; -> loc_1_5908
        mov    esi, 0x375c                   ; -> loc_1_375C
        VMMCall  VMM.Allocate_PM_Call_Back       ; cd200c000100
        mov    ecx, eax
        movzx  edx, cx
        shr    ecx, 0x10
        mov    eax, 0x2f
        VMMCall  VMM.Set_PM_Int_Vector           ; cd2045000100
        mov    eax, 0x7a
        VMMCall  VMM.Get_PM_Int_Vector           ; cd2044000100
        mov    dword ptr [0x590c], ecx       ; -> loc_1_590C
        mov    dword ptr [0x5910], edx       ; -> loc_1_5910
        mov    esi, 0x3693                   ; -> loc_1_3693
        VMMCall  VMM.Allocate_PM_Call_Back       ; cd200c000100
        mov    ecx, eax
        movzx  edx, cx
        shr    ecx, 0x10
        mov    eax, 0x7a
        VMMCall  VMM.Set_PM_Int_Vector           ; cd2045000100
        mov    esi, 0x368d                   ; -> loc_1_368D
        VMMCall  VMM.Allocate_PM_Call_Back       ; cd200c000100
        mov    dword ptr [0x5900], eax       ; -> loc_1_5900
        mov    eax, 0x400024
        mov    esi, 0x53ca                   ; -> loc_1_53CA
        VMMCall  VMM.Hook_Device_Service         ; cd2090000100
        jb     loc_3_0584
        mov    dword ptr [0x5884], esi       ; -> loc_1_5884
loc_3_0584:
        clc    
        ret    
sub_3_0586:
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        mov    word ptr [ebp + 0x1c], 0x7a00
        mov    eax, 0x2f
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        mov    al, byte ptr [ebp + 0x1c]
        inc    al
        jne    loc_3_062F
        mov    ax, word ptr [ebp + 0x38]
        shl    eax, 0x10
        mov    ax, word ptr [ebp]
        mov    dword ptr [0x5914], eax       ; -> loc_1_5914
        or     dword ptr [0x58f8], 8         ; -> loc_1_58F8
        push   0
        push   0xa
        VMMCall  VMM._Allocate_Temp_V86_Data_Area ; cd20a9000100
        add    esp, 8
        or     eax, eax
        je     loc_3_062F
        push   eax
        shr    eax, 4
        mov    word ptr [ebp + 0x38], ax
        mov    word ptr [ebp + 4], 0
        mov    word ptr [ebp + 0x10], 9
        mov    ecx, dword ptr [0x5914]       ; -> loc_1_5914
        movzx  edx, cx
        shr    ecx, 0x10
        VMMCall  VMM.Simulate_Far_Call           ; cd2048000100
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        pop    eax
        mov    ecx, dword ptr [eax]
        mov    esi, dword ptr [0x58dc]       ; -> loc_1_58DC
        mov    dword ptr [esi + 0x18], ecx
        VMMCall  VMM._Free_Temp_V86_Data_Area    ; cd20aa000100
loc_3_0619:
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        ret    
loc_3_062F:
        mov    dword ptr [0x5918], 0         ; -> loc_1_5918
        jmp    loc_3_0619
sub_3_063B:
        mov    edi, dword ptr [0x5824]       ; -> loc_1_5824
        lea    ecx, [edi + edi*2]
        push   2
        push   ecx
        VMMCall  VMM._Allocate_Global_V86_Data_Area ; cd20a8000100
        add    esp, 8
        or     eax, eax
        je     loc_3_0696
        mov    esi, dword ptr [0x5918]       ; -> loc_1_5918
        lea    edi, [eax + edi*2]
        mov    dword ptr [0x585c], eax       ; -> loc_1_585C
        mov    dword ptr [0x5864], edi       ; -> loc_1_5864
        shl    eax, 0xc
        shr    ax, 0xc
        shl    edi, 0xc
        shr    di, 0xc
        mov    dword ptr [esi + 0x32], eax
        mov    dword ptr [esi + 0x36], edi
        mov    dword ptr [esi + 0x3a], 0
        lea    eax, [esi + 0x3a]
        mov    dword ptr [0x5840], eax       ; -> loc_1_5840
        mov    eax, dword ptr [0x55d8]       ; -> loc_1_55D8
        mov    word ptr [esi + 0x42], ax
        clc    
        ret    
loc_3_0696:
        stc    
        ret    
sub_3_0698:
        push   ebp
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    ebp, dword ptr [ebx + 8]
        mov    word ptr [ebp + 0x14], 0
        mov    word ptr [ebp + 0x1c], 0x7a00
        mov    eax, 0x2f
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        mov    cl, byte ptr [ebp + 0x1c]
        xor    eax, eax
        inc    cl
        jne    loc_3_06D9
        inc    al
loc_3_06D9:
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        pop    ebp
        ret    
loc_3_06F0:
        dec    esi
        inc    esp
        dec    ecx
        push   ebx
        dec    eax
        dec    esp
        push   eax
        and    al, 0
sub_3_06F9:
        push   ebp
        push   esi
        push   edi
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    ebp, dword ptr [ebx + 8]
        push   0
        push   0x80
        VMMCall  VMM._Allocate_Temp_V86_Data_Area ; cd20a9000100
        add    esp, 8
        or     eax, eax
        je     loc_3_07B4
        mov    edi, eax
        mov    edx, eax
        mov    esi, 0x6f0                    ; -> loc_3_06F0
        mov    ecx, 9
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    eax, edx
        shr    eax, 4
        mov    word ptr [ebp + 0x3c], ax
        mov    ax, dx
        and    ax, 0xf
        mov    word ptr [ebp + 0x14], ax
        mov    word ptr [ebp + 0x1c], 0x3d40
        mov    ax, 0x21
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        mov    eax, 0
        test   dword ptr [ebp + 0x2c], 1
        jne    loc_3_07AE
        mov    ax, word ptr [ebp + 0x1c]
        mov    word ptr [ebp + 0x10], ax
        mov    byte ptr [ebp + 0x1d], 0x3e
        mov    ax, 0x21
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        VMMCall  VMM._Free_Temp_V86_Data_Area    ; cd20aa000100
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        pop    edi
        pop    esi
        pop    ebp
        mov    eax, 1
        ret    
loc_3_07AE:
        VMMCall  VMM._Free_Temp_V86_Data_Area    ; cd20aa000100
loc_3_07B4:
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        pop    edi
        pop    esi
        pop    ebp
        xor    eax, eax
        ret    
loc_3_07CF:
        dec    edi
        inc    esp
        dec    ecx
        and    al, 0x48
        dec    esp
        push   eax
        and    al, 0
loc_3_07D8:
        xor    eax, eax
        ret    
sub_3_07DB:
        VxDCall  Dev_0497.Service_0000h          ; cd2000009704
        jb     loc_3_07D8
        push   ebp
        push   edi
        push   esi
        push   edx
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    ebp, dword ptr [ebx + 8]
        mov    ebx, dword ptr [ebx + 4]
        mov    dx, 0x5100
        mov    word ptr [ebp + 0x1c], 0x5100
loc_3_0812:
        mov    word ptr [ebp + 0x1c], dx
        mov    eax, 0x2f
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        cmp    byte ptr [ebp + 0x1c], 0xff
        je     loc_3_082F
loc_3_0827:
        inc    dh
        jne    loc_3_0812
        xor    eax, eax
        jmp    loc_3_0863
loc_3_082F:
        movzx  eax, word ptr [ebp + 0x38]
        shl    eax, 4
        movzx  ecx, word ptr [ebp + 4]
        add    eax, ecx
        add    eax, ebx
        mov    edi, eax
        mov    esi, 0x7cf                    ; -> loc_3_07CF
        cld    
        cmpsd  dword ptr [esi], dword ptr es:[edi]
        jne    loc_3_0827
        cmpsd  dword ptr [esi], dword ptr es:[edi]
        jne    loc_3_0827
        movzx  eax, word ptr [ebp + 0x14]
        shl    eax, 4
        movzx  ecx, word ptr [ebp + 0x10]
        add    eax, ecx
        mov    dword ptr [0x5918], eax       ; -> loc_1_5918
        mov    eax, 1
loc_3_0863:
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        pop    edx
        pop    esi
        pop    edi
        pop    ebp
        ret    
        db  000h, 000h, 000h
loc_3_0880:
        dec    esi
        push   edi
        dec    esp
        dec    ecx
        dec    esi
        dec    ebx
        db  000h
loc_3_0887:
        add    ecx, dword ptr [edx]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        or     al, byte ptr [eax]
        add    byte ptr [esi + 0x11], bh     ; -> loc_1_117E
        add    byte ptr [eax], al
        mov    ecx, 0x1b000021               ; -> loc_1_221B
        and    al, byte ptr [eax]
        add    byte ptr [eax + 0x1b], ch     ; -> loc_1_1B68
        add    byte ptr [eax], al
        push   eax
        and    al, 0
        add    byte ptr [ebx + 0x1b], ch     ; -> loc_1_1B6B
        add    byte ptr [eax], al
        jnp    0x8cc
        add    byte ptr [eax], al
        ror    dword ptr [0x228f0000], 1     ; -> loc_1_228F
        add    byte ptr [eax], al
        int3   
        pop    ss
        add    byte ptr [eax], al
        inc    esi
        adc    eax, dword ptr [eax]
        add    bh, dl                        ; -> loc_1_1AD7
        sbb    al, byte ptr [eax]
        add    byte ptr [esi], al
        add    byte ptr [esi], al
        add    byte ptr [eax + 8], al        ; -> loc_3_0880
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0xb10  R-X
sub_4_0000:
        push   esi
        mov    dword ptr [0x588c], 0x5888    ; -> loc_1_588C
        mov    dword ptr [0x5888], 0x5888    ; -> loc_1_5888
        mov    dword ptr [0x58a8], 0x58a4    ; -> loc_1_58A8
        mov    dword ptr [0x58a4], 0x58a4    ; -> loc_1_58A4
        mov    dword ptr [0x58b0], 0x58ac    ; -> loc_1_58B0
        mov    dword ptr [0x58ac], 0x58ac    ; -> loc_1_58AC
        mov    dword ptr [0x58b4], 0         ; -> loc_1_58B4
        mov    dword ptr [0x58b8], 0         ; -> loc_1_58B8
        mov    dword ptr [0x584c], 0         ; -> loc_1_584C
        mov    dword ptr [0x5848], 0         ; -> loc_1_5848
        call   sub_1_0F2E
        cmp    eax, 1
        jne    loc_4_0079
        mov    eax, 1
        jmp    loc_4_0225
loc_4_0079:
        call   sub_1_1050
        cmp    eax, 1
        jne    loc_4_0097
        push   0
        call   sub_3_0024
        add    esp, 4
        mov    eax, 1
        jmp    loc_4_0225
loc_4_0097:
        mov    eax, dword ptr [0x5824]       ; -> loc_1_5824
        imul   eax, eax, 0x3f
        mov    dword ptr [0x581c], eax       ; -> obj1:0x581C
        mov    eax, dword ptr [0x5828]       ; -> loc_1_5828
        imul   eax, eax, 0x76
        add    dword ptr [0x581c], eax       ; -> obj1:0x581C
        mov    eax, dword ptr [0x5834]       ; -> loc_1_5834
        shl    eax, 6
        add    dword ptr [0x581c], eax       ; -> obj1:0x581C
        mov    eax, dword ptr [0x5838]       ; -> loc_1_5838
        imul   eax, eax, 0x50
        add    dword ptr [0x581c], eax       ; -> obj1:0x581C
        push   -1
        push   0
        push   dword ptr [0x581c]            ; -> obj1:0x581C
        push   0x5818                        ; -> obj1:0x5818
        call   sub_1_2468
        cmp    dword ptr [0x5818], 0         ; -> obj1:0x5818
        jne    loc_4_00FF
        push   0
        call   sub_3_0024
        add    esp, 4
        mov    eax, 1
        jmp    loc_4_0225
loc_4_00FF:
        mov    ecx, dword ptr [0x5818]       ; -> obj1:0x5818
        sub    edx, edx
        cmp    dword ptr [0x5838], edx       ; -> loc_1_5838
        jbe    loc_4_012A
        mov    eax, 0x58dc                   ; -> loc_1_58DC
        sub    esi, esi
loc_4_0116:
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 8], esi
        add    ecx, 0x50
        add    eax, 4
        inc    edx
        cmp    dword ptr [0x5838], edx       ; -> loc_1_5838
        ja     loc_4_0116
loc_4_012A:
        mov    eax, dword ptr [0x58dc]       ; -> loc_1_58DC
        mov    dword ptr [0x58ec], eax       ; -> loc_1_58EC
        sub    edx, edx
        cmp    dword ptr [0x5834], edx       ; -> loc_1_5834
        jbe    loc_4_0159
        mov    eax, 0x58cc                   ; -> loc_1_58CC
        sub    esi, esi
loc_4_0145:
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], esi
        add    ecx, 0x40
        add    eax, 4
        inc    edx
        cmp    dword ptr [0x5834], edx       ; -> loc_1_5834
        ja     loc_4_0145
loc_4_0159:
        mov    eax, dword ptr [0x5840]       ; -> loc_1_5840
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [0x5824]       ; -> loc_1_5824
        mov    dword ptr [0x5844], eax       ; -> loc_1_5844
        sub    eax, eax
        mov    dword ptr [0x5870], 0         ; -> loc_1_5870
        cmp    dword ptr [0x5824], eax       ; -> loc_1_5824
        jbe    loc_4_019D
loc_4_0182:
        mov    edx, dword ptr [0x5870]       ; -> loc_1_5870
        mov    dword ptr [ecx + 0x18], edx
        mov    dword ptr [0x5870], ecx       ; -> loc_1_5870
        inc    eax
        add    ecx, 0x38
        cmp    eax, dword ptr [0x5824]       ; -> loc_1_5824
        jb     loc_4_0182
loc_4_019D:
        mov    dword ptr [0x5854], 0         ; -> loc_1_5854
        mov    eax, dword ptr [0x5828]       ; -> loc_1_5828
        mov    dword ptr [0x5858], eax       ; -> loc_1_5858
        mov    dword ptr [0x5874], 0         ; -> loc_1_5874
        sub    eax, eax
        cmp    dword ptr [0x5828], eax       ; -> loc_1_5828
        jbe    loc_4_01E0
loc_4_01C5:
        mov    edx, dword ptr [0x5874]       ; -> loc_1_5874
        mov    dword ptr [ecx + 0x10], edx
        mov    dword ptr [0x5874], ecx       ; -> loc_1_5874
        inc    eax
        add    ecx, 0x70
        cmp    eax, dword ptr [0x5828]       ; -> loc_1_5828
        jb     loc_4_01C5
loc_4_01E0:
        mov    dword ptr [0x585c], ecx       ; -> loc_1_585C
        mov    eax, dword ptr [0x5824]       ; -> loc_1_5824
        lea    eax, [ecx + eax*2]
        mov    dword ptr [0x5860], eax       ; -> loc_1_5860
        mov    eax, dword ptr [0x5824]       ; -> loc_1_5824
        shl    eax, 2
        add    eax, dword ptr [0x5860]       ; -> loc_1_5860
        mov    dword ptr [0x5864], eax       ; -> loc_1_5864
        add    eax, dword ptr [0x5824]       ; -> loc_1_5824
        mov    dword ptr [0x5868], eax       ; -> loc_1_5868
        mov    eax, dword ptr [0x5828]       ; -> loc_1_5828
        add    eax, eax
        add    eax, dword ptr [0x5868]       ; -> loc_1_5868
        mov    dword ptr [0x586c], eax       ; -> loc_1_586C
        sub    eax, eax
loc_4_0225:
        pop    esi
        ret    
sub_4_0227:
        push   esi
        push   edi
        mov    eax, dword ptr [0x5878]       ; -> loc_1_5878
        mov    esi, dword ptr [esp + 0xc]
        imul   eax, eax, 0x1c
        lea    edi, [esi + 0x3c]
        push   -1
        push   0
        push   eax
        push   edi
        call   sub_1_2468
        mov    eax, dword ptr [0x5878]       ; -> loc_1_5878
        shl    eax, 2
        add    eax, dword ptr [edi]
        mov    dword ptr [esi + 0x40], eax
        mov    ecx, dword ptr [0x5878]       ; -> loc_1_5878
        lea    ecx, [eax + ecx*2]
        mov    dword ptr [esi + 0x44], ecx
        mov    dword ptr [esi + 0x38], 0
        or     dword ptr [esi + 8], 2
        pop    edi
        pop    esi
        ret    
        db  053h, 056h, 057h, 055h, 02Bh, 0DBh, 08Bh, 07Ch, 024h, 018h, 03Bh, 0FBh, 074h, 024h, 08Bh, 06Ch
        db  024h, 014h, 0BEh
        dd  loc_1_58CC               ; fixup
        db  053h, 0FFh, 036h, 055h, 0E8h, 01Dh, 000h, 000h, 000h, 083h, 0C4h, 00Ch, 00Bh, 0C0h, 075h, 012h
        db  083h, 0C5h, 008h, 083h, 0C6h, 004h, 043h, 03Bh, 0FBh, 077h, 0E5h, 08Bh, 0C3h, 05Dh, 05Fh, 05Eh
        db  05Bh, 0C3h, 02Bh, 0C0h, 0EBh, 0F7h
sub_4_02A7:
        sub    esp, 0x38
        mov    ecx, dword ptr [esp + 0x40]
        mov    eax, dword ptr [esp + 0x44]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esp + 0x40]
        mov    byte ptr [eax + 0x2e], 0
        mov    eax, dword ptr [esp + 0x40]
        mov    dword ptr [eax + 8], 0
        mov    eax, dword ptr [esp + 0x40]
        mov    dword ptr [eax + 0x3c], 0
        push   dword ptr [esp + 0x40]
        push   dword ptr [esp + 0x40]
        call   sub_4_0A60
        add    esp, 8
        or     eax, eax
        mov    eax, dword ptr [esp + 0x40]
        je     loc_4_02F6
        mov    dword ptr [eax + 4], 0x80000000
        jmp    loc_4_0776
loc_4_02F6:
        or     dword ptr [eax + 4], 1
        mov    eax, dword ptr [esp + 0x40]
        mov    eax, dword ptr [eax + 0x18]
        or     eax, eax
        jne    loc_4_0312
        mov    dword ptr [esp + 0x24], 0x1010102
        jmp    loc_4_0472
loc_4_0312:
        cmp    eax, 1
        jne    loc_4_0324
        mov    dword ptr [esp + 0x24], 0x2010102
        jmp    loc_4_0472
loc_4_0324:
        cmp    eax, 2
        jne    loc_4_035D
        mov    dword ptr [esp + 0x24], 0x6010102
        mov    eax, dword ptr [esp + 0x40]
        add    eax, 0x2d
        mov    dword ptr [esp + 0x28], eax
        push   6
        mov    eax, dword ptr [esp + 0x44]
        push   0
        add    eax, 0x28
        push   eax
        call   sub_1_4AE2
        mov    dword ptr [esp + 0x38], 1
        add    esp, 0xc
        jmp    loc_4_0472
loc_4_035D:
        cmp    eax, 3
        jne    loc_4_0776
        mov    word ptr [esp + 8], 0x80
        mov    word ptr [esp + 0xa], 0
        mov    word ptr [esp + 0xc], 0x3781
        mov    dword ptr [esp + 0x20], 1
        lea    eax, [esp + 8]
        mov    dword ptr [esp + 0x24], 0x4010104
        mov    dword ptr [esp + 0x28], eax
        mov    dword ptr [esp + 0x2c], 6
        lea    eax, [esp + 0x10]
        push   eax
        mov    eax, dword ptr [esp + 0x44]
        push   dword ptr [eax + 0xc]
        lea    eax, [esp + 8]
        push   eax
        call   sub_1_24C2
        cmp    dword ptr [esp], 0x103
        jne    loc_4_03D0
        lea    eax, [esp + 0x10]
        push   dword ptr [esp + 0x40]
        push   eax
        call   sub_1_23AD
        mov    dword ptr [esp + 8], eax
        add    esp, 8
loc_4_03D0:
        mov    dword ptr [esp + 4], 1
        mov    dword ptr [esp + 0x24], 0x4010106
        lea    eax, [esp + 4]
        mov    dword ptr [esp + 0x28], eax
        mov    dword ptr [esp + 0x2c], 4
        lea    eax, [esp + 0x10]
        push   eax
        mov    eax, dword ptr [esp + 0x44]
        push   dword ptr [eax + 0xc]
        lea    eax, [esp + 8]
        push   eax
        call   sub_1_24C2
        cmp    dword ptr [esp], 0x103
        jne    loc_4_0425
        lea    eax, [esp + 0x10]
        push   dword ptr [esp + 0x40]
        push   eax
        call   sub_1_23AD
        mov    dword ptr [esp + 8], eax
        add    esp, 8
loc_4_0425:
        mov    dword ptr [esp + 0x24], 0xff010207
        lea    eax, [esp + 0x40]
        mov    dword ptr [esp + 0x28], eax
        mov    dword ptr [esp + 0x2c], 4
        lea    eax, [esp + 0x10]
        push   eax
        mov    eax, dword ptr [esp + 0x44]
        push   dword ptr [eax + 0xc]
        lea    eax, [esp + 8]
        push   eax
        call   sub_1_24C2
        cmp    dword ptr [esp], 0x103
        jne    loc_4_0472
        lea    eax, [esp + 0x10]
        push   dword ptr [esp + 0x40]
        push   eax
        call   sub_1_23AD
        mov    dword ptr [esp + 8], eax
        add    esp, 8
loc_4_0472:
        mov    eax, dword ptr [esp + 0x40]
        mov    dword ptr [esp + 0x20], 0
        cmp    dword ptr [eax + 0x18], 2
        jne    loc_4_0495
        add    eax, 0x2d
        mov    dword ptr [esp + 0x28], eax
        mov    dword ptr [esp + 0x2c], 1
        jmp    loc_4_04A8
loc_4_0495:
        mov    eax, dword ptr [esp + 0x40]
        add    eax, 0x28
        mov    dword ptr [esp + 0x28], eax
        mov    dword ptr [esp + 0x2c], 6
loc_4_04A8:
        lea    eax, [esp + 0x10]
        push   eax
        mov    eax, dword ptr [esp + 0x44]
        push   dword ptr [eax + 0xc]
        lea    eax, [esp + 8]
        push   eax
        call   sub_1_24C2
        cmp    dword ptr [esp], 0x103
        jne    loc_4_04DD
        lea    eax, [esp + 0x10]
        push   dword ptr [esp + 0x40]
        push   eax
        call   sub_1_23AD
        mov    dword ptr [esp + 8], eax
        add    esp, 8
loc_4_04DD:
        mov    dword ptr [esp + 0x24], 0x10106
        mov    eax, dword ptr [esp + 0x40]
        add    eax, 0x1c
        mov    dword ptr [esp + 0x28], eax
        mov    dword ptr [esp + 0x2c], 4
        lea    eax, [esp + 0x10]
        push   eax
        mov    eax, dword ptr [esp + 0x44]
        push   dword ptr [eax + 0xc]
        lea    eax, [esp + 8]
        push   eax
        call   sub_1_24C2
        cmp    dword ptr [esp], 0x103
        jne    loc_4_052D
        lea    eax, [esp + 0x10]
        push   dword ptr [esp + 0x40]
        push   eax
        call   sub_1_23AD
        mov    dword ptr [esp + 8], eax
        add    esp, 8
loc_4_052D:
        mov    eax, dword ptr [esp + 0x40]
        cmp    dword ptr [eax + 0x18], 2
        jne    loc_4_054C
        mov    ecx, dword ptr [eax + 0x1c]
        add    eax, 0x1c
        cmp    ecx, 0x240
        jb     loc_4_054A
        mov    ecx, 0x240
loc_4_054A:
        mov    dword ptr [eax], ecx
loc_4_054C:
        mov    eax, dword ptr [esp + 0x40]
        cmp    dword ptr [eax + 0x18], 3
        jne    loc_4_0562
        mov    dword ptr [eax + 0x20], 0
        jmp    loc_4_05E4
loc_4_0562:
        mov    dword ptr [esp + 0x24], 0x10107
        mov    eax, dword ptr [esp + 0x40]
        add    eax, 0x20
        mov    dword ptr [esp + 0x28], eax
        mov    dword ptr [esp + 0x2c], 4
        lea    eax, [esp + 0x10]
        push   eax
        mov    eax, dword ptr [esp + 0x44]
        push   dword ptr [eax + 0xc]
        lea    eax, [esp + 8]
        push   eax
        call   sub_1_24C2
        cmp    dword ptr [esp], 0x103
        jne    loc_4_05B2
        lea    eax, [esp + 0x10]
        push   dword ptr [esp + 0x40]
        push   eax
        call   sub_1_23AD
        mov    dword ptr [esp + 8], eax
        add    esp, 8
loc_4_05B2:
        mov    eax, dword ptr [esp + 0x40]
        mov    eax, dword ptr [eax + 0x20]
        cmp    eax, 0x2710
        jb     loc_4_05CD
        mov    eax, dword ptr [esp + 0x40]
        mov    dword ptr [eax + 0x24], 1
        jmp    loc_4_05EC
loc_4_05CD:
        mov    ecx, dword ptr [esp + 0x40]
        push   dword ptr [ecx + 0x1c]
        push   eax
        call   sub_1_0DB0
        mov    ecx, dword ptr [esp + 0x48]
        add    esp, 8
        mov    dword ptr [ecx + 0x24], eax
loc_4_05E4:
        mov    eax, dword ptr [esp + 0x40]
        or     dword ptr [eax + 4], 2
loc_4_05EC:
        mov    dword ptr [esp + 0x24], 0x10105
        lea    eax, [esp + 4]
        mov    dword ptr [esp + 0x28], eax
        mov    dword ptr [esp + 0x2c], 4
        lea    eax, [esp + 0x10]
        push   eax
        mov    eax, dword ptr [esp + 0x44]
        push   dword ptr [eax + 0xc]
        lea    eax, [esp + 8]
        push   eax
        call   sub_1_24C2
        cmp    dword ptr [esp], 0x103
        jne    loc_4_0639
        lea    eax, [esp + 0x10]
        push   dword ptr [esp + 0x40]
        push   eax
        call   sub_1_23AD
        mov    dword ptr [esp + 8], eax
        add    esp, 8
loc_4_0639:
        mov    dword ptr [esp + 0x20], 1
        mov    eax, dword ptr [esp + 4]
        cmp    eax, 0x100
        jb     loc_4_0651
        mov    eax, 0x100
loc_4_0651:
        mov    dword ptr [esp + 4], eax
        mov    eax, dword ptr [esp + 0x40]
        cmp    dword ptr [eax + 0x18], 1
        jne    loc_4_0667
        mov    dword ptr [esp + 4], 0xc0
loc_4_0667:
        mov    dword ptr [esp + 0x24], 0x1010f
        lea    eax, [esp + 4]
        mov    dword ptr [esp + 0x28], eax
        mov    dword ptr [esp + 0x2c], 4
        lea    eax, [esp + 0x10]
        push   eax
        mov    eax, dword ptr [esp + 0x44]
        push   dword ptr [eax + 0xc]
        lea    eax, [esp + 8]
        push   eax
        call   sub_1_24C2
        cmp    dword ptr [esp], 0x103
        jne    loc_4_06B4
        lea    eax, [esp + 0x10]
        push   dword ptr [esp + 0x40]
        push   eax
        call   sub_1_23AD
        mov    dword ptr [esp + 8], eax
        add    esp, 8
loc_4_06B4:
        mov    eax, dword ptr [esp + 0x40]
        cmp    dword ptr [eax + 0x18], 1
        jne    loc_4_071D
        mov    dword ptr [esp + 0x24], 0x2010103
        mov    dword ptr [esp + 4], 0x8000
        lea    eax, [esp + 4]
        mov    dword ptr [esp + 0x28], eax
        mov    dword ptr [esp + 0x2c], 4
        lea    eax, [esp + 0x10]
        push   eax
        mov    eax, dword ptr [esp + 0x44]
        push   dword ptr [eax + 0xc]
        lea    eax, [esp + 8]
        push   eax
        call   sub_1_24C2
        cmp    dword ptr [esp], 0x103
        jne    loc_4_0713
        lea    eax, [esp + 0x10]
        push   dword ptr [esp + 0x40]
        push   eax
        call   sub_1_23AD
        mov    dword ptr [esp + 8], eax
        add    esp, 8
loc_4_0713:
        mov    dword ptr [esp + 4], 0x4009
        jmp    loc_4_0725
loc_4_071D:
        mov    dword ptr [esp + 4], 9
loc_4_0725:
        mov    dword ptr [esp + 0x24], 0x1010e
        lea    eax, [esp + 4]
        mov    dword ptr [esp + 0x28], eax
        mov    dword ptr [esp + 0x2c], 4
        lea    eax, [esp + 0x10]
        push   eax
        mov    eax, dword ptr [esp + 0x44]
        push   dword ptr [eax + 0xc]
        lea    eax, [esp + 8]
        push   eax
        call   sub_1_24C2
        cmp    dword ptr [esp], 0x103
        jne    loc_4_0772
        lea    eax, [esp + 0x10]
        push   dword ptr [esp + 0x40]
        push   eax
        call   sub_1_23AD
        mov    dword ptr [esp + 8], eax
        add    esp, 8
loc_4_0772:
        sub    eax, eax
        jmp    loc_4_077B
loc_4_0776:
        mov    eax, 1
loc_4_077B:
        add    esp, 0x38
        ret    
sub_4_077F:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x18]
        mov    ebx, dword ptr [esp + 0x1c]
        mov    dword ptr [esi + 8], 1
        mov    eax, dword ptr [ebx + 0x18]
        or     eax, eax
        je     loc_4_07BE
        cmp    eax, 1
        je     loc_4_0867
        cmp    eax, 2
        je     loc_4_08E6
        cmp    eax, 3
        je     loc_4_0922
        mov    eax, 1
        jmp    loc_4_0A04
loc_4_07BE:
        mov    ebp, dword ptr [esp + 0x14]
        movzx  eax, byte ptr [ebx + 0x2e]
        mov    ecx, dword ptr [ebp + 4]
        mov    edx, 1
        shl    edx, cl
        test   edx, eax
        je     loc_4_07DE
        mov    eax, 1
        jmp    loc_4_0A04
loc_4_07DE:
        or     ecx, ecx
        jne    loc_4_0808
        test   byte ptr [ebp + 0xc], 4
        mov    dword ptr [esi + 0x28], 0x471e ; -> loc_1_471E
        jne    loc_4_07F6
        mov    dword ptr [esi + 0x28], 0x4763 ; -> loc_1_4763
loc_4_07F6:
        lea    edi, [esi + 0x14]
        mov    dword ptr [esi + 0x2c], 0x1f87 ; -> loc_1_1F87
        mov    dword ptr [edi], 0
        jmp    loc_4_084B
loc_4_0808:
        cmp    ecx, 1
        jne    loc_4_080F
        jmp    loc_4_0834
loc_4_080F:
        cmp    ecx, 2
        jne    loc_4_082D
        mov    dword ptr [esi + 0x28], 0x4712 ; -> loc_1_4712
        lea    edi, [esi + 0x14]
        mov    dword ptr [esi + 0x2c], 0x1f31 ; -> loc_1_1F31
        mov    dword ptr [edi], 0
        jmp    loc_4_084B
loc_4_082D:
        mov    dword ptr [ebp + 4], 4
loc_4_0834:
        mov    dword ptr [esi + 0x28], 0x47c2 ; -> loc_1_47C2
        lea    edi, [esi + 0x14]
        mov    dword ptr [esi + 0x2c], 0x1e60 ; -> loc_1_1E60
        mov    dword ptr [edi], 3
loc_4_084B:
        lea    edx, [esi + 0x10]
        mov    al, 1
        mov    dword ptr [edx], 0xe
        mov    cl, byte ptr [ebp + 4]
        shl    al, cl
        or     byte ptr [ebx + 0x2e], al
        mov    byte ptr [esi + 0x22], 0xff
        jmp    loc_4_095A
loc_4_0867:
        cmp    dword ptr [ebx + 8], 0
        je     loc_4_0877
        mov    eax, 1
        jmp    loc_4_0A04
loc_4_0877:
        mov    ebp, dword ptr [esp + 0x14]
        cmp    dword ptr [ebp + 4], 6
        jne    loc_4_08A4
        mov    dword ptr [esi + 0x28], 0x483c ; -> loc_1_483C
        lea    edi, [esi + 0x14]
        mov    dword ptr [esi + 0x2c], 0x1ea2 ; -> loc_1_1EA2
        mov    dword ptr [edi], 8
        mov    dword ptr [0x55d8], 0xaa00    ; -> loc_1_55D8
        jmp    loc_4_08C5
loc_4_08A4:
        mov    dword ptr [esi + 0x28], 0x481b ; -> loc_1_481B
        lea    edi, [esi + 0x14]
        mov    dword ptr [esi + 0x2c], 0x1ede ; -> loc_1_1EDE
        mov    dword ptr [edi], 3
        mov    dword ptr [0x55d8], 0xe000    ; -> loc_1_55D8
loc_4_08C5:
        cmp    dword ptr [0x5878], 0         ; -> loc_1_5878
        je     loc_4_08D7
        push   esi
        call   sub_4_0227
        add    esp, 4
loc_4_08D7:
        lea    edx, [esi + 0x10]
        mov    dword ptr [edx], 0xe
        mov    byte ptr [esi + 0x22], 0x7f
        jmp    loc_4_095A
loc_4_08E6:
        cmp    dword ptr [ebx + 8], 0
        je     loc_4_08F6
        mov    eax, 1
        jmp    loc_4_0A04
loc_4_08F6:
        mov    dword ptr [esi + 0x28], 0x47e6 ; -> loc_1_47E6
        lea    edi, [esi + 0x14]
        mov    dword ptr [esi + 0x2c], 0x1f54 ; -> loc_1_1F54
        mov    dword ptr [edi], 0
        lea    edx, [esi + 0x10]
        mov    dword ptr [0x55d8], 0xfa      ; -> loc_1_55D8
        mov    dword ptr [edx], 3
        jmp    loc_4_0956
loc_4_0922:
        cmp    dword ptr [ebx + 8], 0
        je     loc_4_0932
        mov    eax, 1
        jmp    loc_4_0A04
loc_4_0932:
        mov    dword ptr [esi + 0x28], 0x4763 ; -> loc_1_4763
        lea    edi, [esi + 0x14]
        mov    dword ptr [esi + 0x2c], 0x1f87 ; -> loc_1_1F87
        mov    dword ptr [edi], 0
        lea    edx, [esi + 0x10]
        or     dword ptr [esi + 8], 0x10
        mov    dword ptr [edx], 0xe
loc_4_0956:
        mov    ebp, dword ptr [esp + 0x14]
loc_4_095A:
        inc    dword ptr [ebx + 8]
        mov    eax, dword ptr [ebx + 0x3c]
        or     eax, eax
        jne    loc_4_0969
        mov    dword ptr [ebx + 0x3c], esi
        jmp    loc_4_097C
loc_4_0969:
        cmp    dword ptr [eax + 0x4c], 0
        je     loc_4_0979
        sub    ecx, ecx
loc_4_0971:
        mov    eax, dword ptr [eax + 0x4c]
        cmp    dword ptr [eax + 0x4c], ecx
        jne    loc_4_0971
loc_4_0979:
        mov    dword ptr [eax + 0x4c], esi
loc_4_097C:
        mov    dword ptr [esi + 0x4c], 0
        mov    dword ptr [esi + 0x48], ebx
        mov    ecx, dword ptr [esp + 0x20]
        mov    dword ptr [esi], ecx
        mov    eax, 1
        shl    eax, cl
        mov    dword ptr [esi + 4], eax
        test   byte ptr [ebx + 4], 2
        je     loc_4_09A0
        or     dword ptr [esi + 8], 8
loc_4_09A0:
        mov    eax, dword ptr [ebx + 0x24]
        mov    word ptr [esi + 0x24], ax
        mov    eax, dword ptr [ebx + 0x1c]
        sub    eax, dword ptr [edi]
        mov    dword ptr [esi + 0xc], eax
        mov    eax, dword ptr [ebx + 0x1c]
        add    dword ptr [edx], eax
        test   byte ptr [esi + 8], 2
        je     loc_4_09BE
        sub    dword ptr [esi + 0xc], 0x12
loc_4_09BE:
        push   6
        lea    eax, [ebx + 0x28]
        push   eax
        lea    eax, [esi + 0x1c]
        push   eax
        call   sub_1_4AF3
        add    esp, 0xc
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [esi + 0x18], eax
        mov    byte ptr [esi + 0x23], 0
        cmp    dword ptr [ebx + 0x18], 0
        jne    loc_4_09F1
        cmp    dword ptr [ebp + 4], 4
        jne    loc_4_09F1
        push   esi
        call   sub_1_163C
        add    esp, 4
        jmp    loc_4_09F8
loc_4_09F1:
        or     dword ptr [0x58f8], 1         ; -> loc_1_58F8
loc_4_09F8:
        and    dword ptr [0x58f8], 0xfffffeff ; -> loc_1_58F8
        sub    eax, eax
loc_4_0A04:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  053h, 056h, 057h, 055h, 02Bh, 0DBh, 08Bh, 074h, 024h, 018h, 03Bh, 0F3h, 074h, 034h, 0BDh
        dd  loc_1_58DC               ; fixup
        db  08Bh, 07Ch, 024h, 01Ch, 08Bh, 007h, 039h, 044h, 024h, 014h, 076h, 02Ah, 053h, 0FFh, 034h, 085h
        dd  loc_1_58CC               ; fixup
        db  0FFh, 075h, 000h, 057h, 0E8h, 046h, 0FDh, 0FFh, 0FFh, 083h, 0C4h, 010h, 00Bh, 0C0h, 075h, 019h
        db  083h, 0C5h, 004h, 083h, 0C7h, 010h, 043h, 03Bh, 0DEh, 072h, 0D5h, 02Bh, 0C0h, 05Dh, 05Fh, 05Eh
        db  05Bh, 0C3h, 0B8h, 001h, 000h, 000h, 000h, 0EBh, 0F4h, 0B8h, 001h, 000h, 000h, 000h, 0EBh, 0EDh
sub_4_0A60:
        sub    esp, 0x18
        push   esi
        mov    dword ptr [esp + 0xc], 0
        mov    dword ptr [esp + 0x10], 1
        mov    dword ptr [esp + 0x14], 7
        mov    dword ptr [esp + 0x18], 3
        push   0
        push   0
        mov    esi, dword ptr [esp + 0x2c]
        push   dword ptr [esp + 0x28]
        push   esi
        push   dword ptr [0x5814]            ; -> obj1:0x5814
        lea    eax, [esp + 0x20]
        push   4
        push   eax
        lea    eax, [esi + 0x18]
        push   eax
        lea    eax, [esi + 0xc]
        push   eax
        lea    eax, [esp + 0x2c]
        push   eax
        lea    eax, [esp + 0x2c]
        push   eax
        call   sub_1_24B6
        cmp    dword ptr [esp + 4], 0x103
        je     loc_4_0AC6
        cmp    dword ptr [esp + 4], 1
        jmp    loc_4_0AEA
loc_4_0AC6:
        test   byte ptr [esi + 7], 0x20
        jne    loc_4_0AD7
loc_4_0ACC:
        call   sub_1_4A98
        test   byte ptr [esi + 7], 0x20
        je     loc_4_0ACC
loc_4_0AD7:
        mov    eax, dword ptr [esi + 4]
        and    eax, 0xdfffffff
        mov    dword ptr [esi + 4], eax
        and    eax, 0x40000000
        cmp    eax, 1
loc_4_0AEA:
        sbb    eax, eax
        inc    eax
        pop    esi
        add    esp, 0x18
        ret    
loc_4_0AF2:
        cmp    dword ptr [esp + 8], 0
        mov    eax, dword ptr [esp + 4]
        je     loc_4_0B04
        or     dword ptr [eax + 4], 0x40000000
loc_4_0B04:
        or     dword ptr [eax + 4], 0x20000000
        ret    0x240c
        db  04Eh, 057h
VxD__LTEXT ENDS

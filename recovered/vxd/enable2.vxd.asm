; ENABLE2 -- reconstructed 32-bit disassembly (VxD / LE format)
; 5 objects, 2679 instructions recovered by recursive descent

Declare_Virtual_Device ENABLE2, 0, 128, ENABLE2_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 034000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x1a48  R-X
        db  0EBh, 006h, 0FFh, 025h
        dd  sub_1_16FC               ; fixup
loc_1_0008:
        pushal 
        mov    dword ptr [0x176d], eax       ; -> loc_1_176D
        VxDCall  ENABLE.VMINI_Status             ; cd2002003700
        mov    dword ptr [0x1704], eax       ; -> obj1:0x1704
        call   sub_1_1769
        cmp    dword ptr [0x176d], 1         ; -> loc_1_176D
        jne    loc_1_0058
        lea    edi, [0x17bc]                 ; -> obj1:0x17BC
        mov    ebx, 1
        VxDCall  ENABLE.VMINI_GetData            ; cd2007003700
        pushal 
        VxDCall  VKD.VKD_Get_Kbd_Owner           ; cd2008000d00
        VxDCall  VKD.VKD_Get_Shift_State         ; cd2011000d00
        and    al, 0xf
        mov    byte ptr [0x1737], al         ; -> obj1:0x1737
        mov    byte ptr [0x1a24], al         ; -> loc_1_1A24
        popal  
        jmp    loc_1_0136
loc_1_0058:
        cmp    dword ptr [0x176d], 0x100     ; -> loc_1_176D
        jne    loc_1_007A
        lea    edi, [0x1772]                 ; -> obj1:0x1772
        mov    ebx, 0x100
        VxDCall  ENABLE.VMINI_GetData            ; cd2007003700
        jmp    loc_1_0136
loc_1_007A:
        cmp    dword ptr [0x176d], 0x40      ; -> loc_1_176D
        jne    loc_1_00A0
        mov    ebx, 0x40
        lea    edi, [0x1792]                 ; -> obj1:0x1792
        VxDCall  ENABLE.VMINI_GetData            ; cd2007003700
        pushal 
        call   sub_1_0ACD
        popal  
        jmp    loc_1_0136
loc_1_00A0:
        cmp    dword ptr [0x176d], 0x200     ; -> loc_1_176D
        jne    loc_1_00BF
        mov    ebx, 0x200
        lea    edi, [0x178a]                 ; -> obj1:0x178A
        VxDCall  ENABLE.VMINI_GetData            ; cd2007003700
        jmp    loc_1_0136
loc_1_00BF:
        cmp    dword ptr [0x176d], 0x400     ; -> loc_1_176D
        jne    loc_1_00DE
        mov    ebx, 0x400
        lea    edi, [0x17ae]                 ; -> obj1:0x17AE
        VxDCall  ENABLE.VMINI_GetData            ; cd2007003700
        jmp    loc_1_0136
loc_1_00DE:
        cmp    dword ptr [0x176d], 0x80      ; -> loc_1_176D
        jne    loc_1_00FD
        mov    ebx, 0x80
        lea    edi, [0x17d0]                 ; -> obj1:0x17D0
        VxDCall  ENABLE.VMINI_GetData            ; cd2007003700
        jmp    loc_1_0136
loc_1_00FD:
        cmp    dword ptr [0x176d], 8         ; -> loc_1_176D
        jne    loc_1_0119
        mov    ebx, 8
        lea    edi, [0x17e8]                 ; -> obj1:0x17E8
        VxDCall  ENABLE.VMINI_GetData            ; cd2007003700
        jmp    loc_1_0136
loc_1_0119:
        cmp    dword ptr [0x176d], 0x800     ; -> loc_1_176D
        jne    loc_1_0136
        mov    ebx, 0x800
        lea    edi, [0x1818]                 ; -> obj1:0x1818
        VxDCall  ENABLE.VMINI_GetData            ; cd2007003700
loc_1_0136:
        VxDCall  VKD.VKD_Get_Kbd_Owner           ; cd2008000d00
        VxDCall  VKD.VKD_Get_Shift_State         ; cd2011000d00
        and    al, 0xf
        mov    byte ptr [0x1738], al         ; -> loc_1_1738
        popal  
        clc    
        call   sub_1_16FC
        ret    
loc_1_0152:
        mov    esi, dword ptr [0x19f1]       ; -> obj1:0x19F1
        VMMCall  VMM.Cancel_Global_Event         ; cd2012000100
        mov    esi, 0xffffffff
        xchg   dword ptr [0x1a40], esi       ; -> obj1:0x1A40
        cmp    esi, -1
        je     loc_1_0179
        cmp    esi, 0
        je     loc_1_0179
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
loc_1_0179:
        lea    edi, [0x16bc]                 ; -> obj1:0x16BC
        xor    eax, eax
        mov    dword ptr [edi], eax
        mov    dword ptr [edi + 0x18], eax
        mov    dword ptr [edi + 8], eax
        mov    dword ptr [edi + 0xc], eax
        mov    dword ptr [edi + 0x10], eax
        mov    dword ptr [edi + 0x14], eax
        mov    dword ptr [edi + 0x1c], eax
        VxDCall  ENABLE.VMINI_RegisterAccess     ; cd2006003700
        call   sub_1_0B13
        mov    eax, 0x370001
        mov    esi, 8                        ; -> loc_1_0008
        VMMCall  VMM.Unhook_Device_Service       ; cd201c010100
        clc    
        ret    
        db  0B8h, 000h, 004h, 000h, 000h, 0F8h, 0C3h
ENABLE2_Control:
        cmp    eax, 0x1b
        je     loc_3_0000
        cmp    eax, 0x1c
        je     loc_1_0152
        cmp    eax, 1
        je     loc_3_0000
        cmp    eax, 5
        je     loc_1_0152
        clc    
        ret    
sub_1_01DB:
        mov    word ptr [0x1724], 0          ; -> obj1:0x1724
        ret    
loc_1_01E5:
        mov    word ptr [0x1720], cx         ; -> obj1:0x1720
        cmp    al, 0xe0
        je     loc_1_01FD
        cmp    al, 0xe1
        je     loc_1_01FD
        test   al, 0x80
        je     loc_1_01FD
        call   sub_1_01DB
loc_1_01FD:
        push   eax
        cmp    dword ptr [0x1a40], -1        ; -> obj1:0x1A40
        jne    loc_1_0224
        pushal 
        mov    eax, 0x37
        mov    edx, 0
        mov    esi, 0xb98                    ; -> loc_4_0B98
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    dword ptr [0x1a40], esi       ; -> obj1:0x1A40
        popal  
loc_1_0224:
        cmp    al, 0xe0
        je     loc_1_0236
        cmp    al, 0xe1
        jne    loc_1_0236
        mov    byte ptr [0x1748], al         ; -> loc_1_1748
        jmp    loc_1_0397
loc_1_0236:
        mov    ah, byte ptr [0x1748]         ; -> loc_1_1748
        mov    byte ptr [0x1748], 0          ; -> loc_1_1748
        cmp    al, 0x54
        jne    loc_1_024C
        jmp    loc_1_03A4
loc_1_024C:
        cmp    al, 0xd5
        jne    loc_1_0255
        jmp    loc_1_03A4
loc_1_0255:
        test   word ptr [0x1767], 0x20       ; -> loc_1_1767
        je     loc_1_0265
        jmp    loc_1_0372
loc_1_0265:
        test   dword ptr [0x17c0], 1         ; -> obj1:0x17C0
        jne    loc_1_0276
        jmp    loc_1_0372
loc_1_0276:
        cmp    byte ptr [0x173a], 0          ; -> loc_1_173A
        je     loc_1_0284
        jmp    loc_1_0340
loc_1_0284:
        cmp    ah, 0xe0
        jne    loc_1_0316
        cmp    al, 0x2a
        jne    loc_1_02B6
        test   byte ptr [0x1762], 0x28       ; -> obj1:0x1762
        jne    loc_1_029F
        jmp    loc_1_038A
loc_1_029F:
        xchg   ah, al
        call   sub_1_1824
        jb     loc_1_02AF
        VxDCall  VKD.VKD_Put_Byte                ; cd2013000d00
loc_1_02AF:
        xchg   ah, al
        jmp    loc_1_03A4
loc_1_02B6:
        cmp    al, 0x37
        jne    loc_1_02DE
        test   byte ptr [0x1762], 0x28       ; -> obj1:0x1762
        jne    loc_1_02F0
        jmp    loc_1_02DE
        db  0C6h, 005h
        dd  obj1:0x1739              ; fixup
        db  0FFh, 0B4h, 000h, 0B0h, 054h, 0F6h, 005h
        dd  obj1:0x1762              ; fixup
        db  014h, 074h, 03Bh, 0E9h, 0C6h, 000h, 000h, 000h
loc_1_02DE:
        cmp    al, 0xb7
        jne    loc_1_02F0
        cmp    byte ptr [0x1739], 0          ; -> obj1:0x1739
        je     loc_1_030D
        jmp    loc_1_0397
loc_1_02F0:
        cmp    al, 0xaa
        jne    loc_1_030D
        cmp    byte ptr [0x1739], 0          ; -> obj1:0x1739
        je     loc_1_030D
        mov    byte ptr [0x1739], 0          ; -> obj1:0x1739
        mov    ah, 0
        mov    al, 0xd4
        jmp    loc_1_03A4
loc_1_030D:
        mov    byte ptr [0x1739], 0          ; -> obj1:0x1739
        jmp    loc_1_038A
loc_1_0316:
        cmp    ah, 0xe1
        jne    loc_1_038A
        jmp    loc_1_0372
        db  0F6h, 005h
        dd  obj1:0x1762              ; fixup
        db  014h, 074h, 002h, 0EBh, 00Bh, 0F6h, 005h
        dd  loc_1_1763               ; fixup
        db  014h, 075h, 002h, 0EBh, 03Fh, 0C6h, 005h
        dd  loc_1_173A               ; fixup
        db  0FFh, 0B4h, 0E0h, 0B0h, 046h, 0EBh, 04Ah
loc_1_0340:
        cmp    al, 0xc5
        jne    loc_1_0358
        mov    ah, 0xe0
        mov    al, 0xc6
        mov    byte ptr [0x173a], 0          ; -> loc_1_173A
        mov    byte ptr [0x1746], 0          ; -> loc_1_1746
        jmp    loc_1_038A
loc_1_0358:
        inc    byte ptr [0x1746]             ; -> loc_1_1746
        cmp    byte ptr [0x1746], 3          ; -> loc_1_1746
        jle    loc_1_0370
        mov    byte ptr [0x173a], 0          ; -> loc_1_173A
        jmp    loc_1_0372
loc_1_0370:
        jmp    loc_1_03A4
loc_1_0372:
        test   dword ptr [0x1796], 1         ; -> obj1:0x1796
        je     loc_1_038A
        cmp    ah, 0xe1
        jne    loc_1_038A
        mov    byte ptr [0x1747], 0xff       ; -> obj1:0x1747
loc_1_038A:
        clc    
        call   sub_1_03F0
        jb     loc_1_0397
        call   sub_1_15E8
loc_1_0397:
        jb     loc_1_03A2
        test   al, 0x80
        je     loc_1_03A2
        call   sub_1_01DB
loc_1_03A2:
        pop    eax
        ret    
loc_1_03A4:
        pop    eax
        jb     loc_1_03B6
        test   al, 0x80
        je     loc_1_03B0
        call   sub_1_01DB
loc_1_03B0:
        call   sub_1_1824
loc_1_03B6:
        ret    
        db  000h
loc_1_03B8:
        push   0
        push   edx
        push   0                             ; -> loc_4_0000
        VxDCall  SHELL._SHELL_CallAtAppyTime     ; cd200e001700
        add    esp, 0xc
        or     eax, eax
        je     loc_1_03CF
        clc    
        ret    
loc_1_03CF:
        stc    
        ret    
loc_1_03D1:
        xor    edx, edx
        mov    dx, cx
        push   0
        push   edx
        push   0x5e                          ; -> loc_4_005E
        VxDCall  SHELL._SHELL_CallAtAppyTime     ; cd200e001700
        add    esp, 0xc
        or     eax, eax
        je     loc_1_03ED
        clc    
        ret    
loc_1_03ED:
        stc    
        ret    
        db  000h
sub_1_03F0:
        push   ebx
        push   ecx
        push   edx
        push   edi
        push   esi
        jae    loc_1_03FE
        mov    byte ptr [0x1749], 0xff       ; -> obj1:0x1749
loc_1_03FE:
        cmp    al, 0xe0
        jne    loc_1_040E
        mov    byte ptr [0x1869], 0xe0       ; -> obj1:0x1869
        jmp    loc_1_066A
loc_1_040E:
        mov    ah, byte ptr [0x1869]         ; -> obj1:0x1869
        mov    byte ptr [0x1869], 0          ; -> obj1:0x1869
        mov    bx, ax
        mov    byte ptr [0x1738], 0          ; -> loc_1_1738
        and    al, 0x7f
        mov    ecx, 6
        mov    edi, 0x172b                   ; -> loc_1_172B
        cld    
        repne scasw ax, word ptr es:[edi]
        jne    loc_1_047A
        mov    al, 1
        shl    al, cl
        mov    byte ptr [0x1771], al         ; -> loc_1_1771
        mov    byte ptr [0x1738], al         ; -> loc_1_1738
        mov    ah, byte ptr [0x1737]         ; -> obj1:0x1737
        test   bl, 0x80
        je     loc_1_0456
        not    al
        and    ah, al
        jmp    loc_1_0458
loc_1_0456:
        or     ah, al
loc_1_0458:
        mov    byte ptr [0x1737], ah         ; -> obj1:0x1737
        cmp    byte ptr [0x1749], 0          ; -> obj1:0x1749
        jne    loc_1_047A
        cmp    bl, 0x36
        jne    loc_1_047A
        test   ah, 0x3c
        jne    loc_1_047A
        mov    byte ptr [0x1729], 0xff       ; -> obj1:0x1729
        jmp    loc_1_0481
loc_1_047A:
        mov    byte ptr [0x1729], 0          ; -> obj1:0x1729
loc_1_0481:
        mov    ax, bx
        and    bl, 0x7f
        test   dword ptr [0x1776], 1         ; -> obj1:0x1776
        jne    loc_1_0498
        jmp    loc_1_066A
loc_1_0498:
        test   al, 0x80
        je     loc_1_04E1
        cmp    word ptr [0x1722], bx         ; -> obj1:0x1722
        je     loc_1_04AA
        jmp    loc_1_066A
loc_1_04AA:
        cmp    dword ptr [0x1786], 0x7ff8    ; -> obj1:0x1786
        je     loc_1_04CC
        mov    ebx, dword ptr [0x1786]       ; -> obj1:0x1786
        mov    word ptr [0x1726], bx         ; -> obj1:0x1726
        mov    byte ptr [0x172a], 0xff       ; -> obj1:0x172A
        jmp    loc_1_04D5
loc_1_04CC:
        mov    word ptr [0x1722], 0          ; -> obj1:0x1722
loc_1_04D5:
        mov    byte ptr [0x1728], 0          ; -> obj1:0x1728
        jmp    loc_1_066A
loc_1_04E1:
        cmp    ax, 0xe02a
        je     loc_1_0508
        cmp    ax, 0xe036
        je     loc_1_0508
        cmp    ax, 0xe11d
        je     loc_1_0508
        cmp    ax, 0xe046
        je     loc_1_0508
        cmp    word ptr [0x1724], 0xe11d     ; -> obj1:0x1724
        jne    loc_1_050D
        cmp    al, 0x45
        jne    loc_1_050D
loc_1_0508:
        jmp    loc_1_066A
loc_1_050D:
        cmp    ax, word ptr [0x1724]         ; -> obj1:0x1724
        jne    loc_1_051B
        jmp    loc_1_0687
loc_1_051B:
        cmp    dword ptr [0x1786], 0x7ff8    ; -> obj1:0x1786
        je     loc_1_059B
        mov    bx, word ptr [0x1722]         ; -> obj1:0x1722
        cmp    ax, bx
        je     loc_1_055C
        mov    word ptr [0x1722], ax         ; -> obj1:0x1722
        test   dword ptr [0x1776], 0x40      ; -> obj1:0x1776
        je     loc_1_054A
        call   sub_1_1478
loc_1_054A:
        mov    ebx, dword ptr [0x1786]       ; -> obj1:0x1786
        mov    word ptr [0x1726], bx         ; -> obj1:0x1726
        jmp    loc_1_066A
loc_1_055C:
        cmp    word ptr [0x1726], 0          ; -> obj1:0x1726
        jg     loc_1_0590
        mov    ebx, dword ptr [0x1786]       ; -> obj1:0x1786
        mov    word ptr [0x1726], bx         ; -> obj1:0x1726
        clc    
        mov    word ptr [0x1722], ax         ; -> obj1:0x1722
        test   dword ptr [0x1776], 0x40      ; -> obj1:0x1776
        je     loc_1_058B
        call   sub_1_1478
loc_1_058B:
        jmp    loc_1_066A
loc_1_0590:
        mov    word ptr [0x1722], ax         ; -> obj1:0x1722
        jmp    loc_1_0687
loc_1_059B:
        mov    ebx, dword ptr [0x177a]       ; -> obj1:0x177A
        cmp    bx, 0x7ff8
        jne    loc_1_05AC
        mov    bx, 0
loc_1_05AC:
        or     bx, bx
        je     loc_1_05DB
        test   dword ptr [0x1776], 0x40      ; -> obj1:0x1776
        je     loc_1_05C2
        call   sub_1_1478
loc_1_05C2:
        mov    word ptr [0x1726], bx         ; -> obj1:0x1726
        mov    byte ptr [0x1728], 0          ; -> obj1:0x1728
        mov    word ptr [0x1724], ax         ; -> obj1:0x1724
        jmp    loc_1_0687
loc_1_05DB:
        cmp    dword ptr [0x1786], 0x7ff8    ; -> obj1:0x1786
        je     loc_1_0639
        cmp    byte ptr [0x172a], 0          ; -> obj1:0x172A
        je     loc_1_060B
        mov    ebx, dword ptr [0x1786]       ; -> obj1:0x1786
        mov    word ptr [0x1726], bx         ; -> obj1:0x1726
        mov    word ptr [0x1724], ax         ; -> obj1:0x1724
        mov    word ptr [0x1722], ax         ; -> obj1:0x1722
        jmp    loc_1_0687
loc_1_060B:
        mov    ebx, dword ptr [0x1786]       ; -> obj1:0x1786
        mov    word ptr [0x1726], bx         ; -> obj1:0x1726
        mov    byte ptr [0x1728], 0          ; -> obj1:0x1728
        mov    byte ptr [0x172a], 0xff       ; -> obj1:0x172A
        test   dword ptr [0x1776], 0x40      ; -> obj1:0x1776
        je     loc_1_0637
        call   sub_1_1478
loc_1_0637:
        jmp    loc_1_065E
loc_1_0639:
        test   dword ptr [0x1776], 0x40      ; -> obj1:0x1776
        je     loc_1_064A
        call   sub_1_1478
loc_1_064A:
        mov    ebx, dword ptr [0x177e]       ; -> obj1:0x177E
        mov    word ptr [0x1726], bx         ; -> obj1:0x1726
        mov    byte ptr [0x1728], 0xff       ; -> obj1:0x1728
loc_1_065E:
        mov    word ptr [0x1724], ax         ; -> obj1:0x1724
        mov    word ptr [0x1722], ax         ; -> obj1:0x1722
loc_1_066A:
        cmp    byte ptr [0x1725], 0xe0       ; -> obj1:0x1725
        jne    loc_1_067C
        push   eax
        mov    al, 0xe0
        call   sub_1_0C34
        pop    eax
loc_1_067C:
        call   sub_1_0C34
loc_1_0681:
        pop    esi
        pop    edi
        pop    edx
        pop    ecx
        pop    ebx
        ret    
loc_1_0687:
        stc    
        jmp    loc_1_0681
        db  000h, 000h
sub_1_068C:
        test   cl, 0x80
        je     loc_1_06AC
        movzx  ebx, byte ptr [0x19db]        ; -> obj1:0x19DB
        cmp    ebx, 0xc
        jne    loc_1_069F
        xor    ebx, ebx
loc_1_069F:
        mov    byte ptr [ebx + 0x19dd], al   ; -> obj1:0x19DD
        inc    ebx
        mov    byte ptr [0x19db], bl         ; -> obj1:0x19DB
loc_1_06AC:
        clc    
        ret    
sub_1_06AE:
        movzx  ebx, byte ptr [0x19dc]        ; -> loc_1_19DC
        cmp    bl, byte ptr [0x19db]         ; -> obj1:0x19DB
        je     loc_1_06D3
        cmp    ebx, 0xc
        jne    loc_1_06C4
        xor    ebx, ebx
loc_1_06C4:
        mov    al, byte ptr [ebx + 0x19dd]   ; -> obj1:0x19DD
        inc    ebx
        mov    byte ptr [0x19dc], bl         ; -> loc_1_19DC
        clc    
        ret    
loc_1_06D3:
        stc    
        ret    
sub_1_06D5:
        pushal 
        mov    dl, al
        mov    esi, 0x2d3                    ; -> loc_4_02D3
        VMMCall  VMM.Schedule_Global_Event       ; cd200e000100
        mov    dword ptr [0x19f1], esi       ; -> obj1:0x19F1
        popal  
        ret    
sub_1_06EB:
        call   sub_1_06AE
        jb     loc_1_0708
        call   sub_1_08F0
        xor    esi, esi
        xor    edi, edi
        call   sub_1_06D5
        VxDCall  VMD.Service_0003h               ; cd2003000c00
        stc    
        ret    
loc_1_0708:
        clc    
        ret    
sub_1_070A:
        mov    dh, byte ptr [0x1762]         ; -> obj1:0x1762
        or     dh, byte ptr [0x1737]         ; -> obj1:0x1737
        or     dh, byte ptr [0x1763]         ; -> loc_1_1763
        cmp    byte ptr [0x186c], 0          ; -> loc_1_186C
        jne    loc_1_0731
        push   eax
        mov    eax, dword ptr [0x1940]       ; -> loc_1_1940
        mov    dword ptr [0x193c], eax       ; -> loc_1_193C
        pop    eax
loc_1_0731:
        test   dword ptr [0x1796], 1         ; -> obj1:0x1796
        jne    loc_1_0742
        jmp    loc_1_07E3
loc_1_0742:
        mov    ah, byte ptr [0x1882]         ; -> loc_1_1882
        mov    byte ptr [0x1882], 0          ; -> loc_1_1882
        cmp    al, 0xe0
        jne    loc_1_0758
        mov    byte ptr [0x1882], al         ; -> loc_1_1882
loc_1_0758:
        mov    si, ax
        and    al, 0x7f
        mov    ecx, 0x11
        mov    edi, 0x1883                   ; -> obj1:0x1883
        cld    
        repne scasw ax, word ptr es:[edi]
        je     loc_1_0777
loc_1_076D:
        call   sub_1_0A9C
        mov    ax, si
loc_1_0775:
        jmp    loc_1_07E3
loc_1_0777:
        xor    ebx, ebx
        mov    bx, cx
        mov    cx, si
        cmp    byte ptr [0x1747], 0          ; -> obj1:0x1747
        je     loc_1_0798
        cmp    cx, 0xc5
        jne    loc_1_076D
        mov    byte ptr [0x1747], 0          ; -> obj1:0x1747
        jmp    loc_1_076D
loc_1_0798:
        cmp    cx, 0x53
        jne    loc_1_07A8
        cmp    dh, 0x3c
        jne    loc_1_07A8
        mov    ax, cx
        jmp    loc_1_0775
loc_1_07A8:
        cmp    byte ptr [0x186f], 0          ; -> obj1:0x186F
        jne    loc_1_07C3
        cmp    cx, 0x45
        je     loc_1_07C3
        cmp    cx, 0xc5
        je     loc_1_07C3
        mov    ax, cx
        jmp    loc_1_0775
loc_1_07C3:
        mov    ax, word ptr [ebx*2 + 0x18e9] ; -> loc_1_18E9
        call   sub_1_18A5
        and    cl, 0x7f
        cmp    cx, 0xe01c
        je     loc_1_07E3
        cmp    cl, 0x45
        je     loc_1_07E3
        stc    
        ret    
loc_1_07E3:
        clc    
        ret    
loc_1_07E5:
        mov    ax, cx
        ret    
loc_1_07E9:
        pushal 
        test   cl, 0x80
        je     loc_1_0855
        mov    ax, word ptr [0x1720]         ; -> obj1:0x1720
        and    ax, 0x20
        shl    ax, 2
        mov    ebx, dword ptr [0x1796]       ; -> obj1:0x1796
        and    bx, 0x80
        xor    ax, bx
        test   ax, 0x80
        je     loc_1_082A
        mov    byte ptr [0x186f], 0xff       ; -> obj1:0x186F
        push   ax
        mov    al, 0xfe
        call   sub_1_06D5
        pop    ax
        call   sub_1_1457
        jmp    loc_1_0855
loc_1_082A:
        call   sub_1_0A9C
        mov    byte ptr [0x186f], 0          ; -> obj1:0x186F
        push   ax
        mov    al, 0xff
        call   sub_1_06D5
        pop    ax
        call   sub_1_144F
        push   eax
        push   ecx
        xor    ecx, ecx
        mov    cl, 0x53
        xor    eax, eax
        call   sub_1_087B
        pop    ecx
        pop    eax
loc_1_0855:
        mov    ax, cx
        popal  
        ret    
loc_1_085A:
        cmp    ax, 3
        jne    loc_1_0862
        push   eax
        pop    eax
loc_1_0862:
        mov    byte ptr [0x1874], al         ; -> loc_1_1874
        push   eax
        movzx  eax, word ptr [0x1880]        ; -> loc_1_1880
        call   sub_1_08F0
        call   sub_1_06D5
        pop    eax
        ret    
sub_1_087B:
        push   eax
        push   ecx
        test   cl, 0x80
        jne    loc_1_08ED
        call   sub_1_0A9C
        mov    bl, byte ptr [0x1875]         ; -> obj1:0x1875
        mov    cl, byte ptr [0x1874]         ; -> loc_1_1874
        mov    dl, bl
        or     ax, ax
        je     loc_1_089E
        or     dl, cl
        jmp    loc_1_08A2
loc_1_089E:
        not    cl
        and    dl, cl
loc_1_08A2:
        mov    byte ptr [0x1875], dl         ; -> obj1:0x1875
        shl    bl, 1
        shl    bl, 1
        or     bl, dl
        and    ebx, 0xf
        mov    bl, byte ptr [ebx + 0x190b]   ; -> obj1:0x190B
        or     bx, bx
        je     loc_1_08ED
        mov    word ptr [0x1880], bx         ; -> loc_1_1880
        mov    ebx, 0x187a                   ; -> loc_1_187A
        movsx  esi, word ptr [0x187e]        ; -> loc_1_187E
        movsx  edi, word ptr [0x187c]        ; -> loc_1_187C
        movzx  eax, word ptr [0x1880]        ; -> loc_1_1880
        call   sub_1_08F0
        call   sub_1_06D5
        VxDCall  VMD.Service_0003h               ; cd2003000c00
loc_1_08ED:
        pop    ecx
        pop    eax
        ret    
sub_1_08F0:
        and    eax, 0xffff
        cmp    byte ptr [0x1874], 2          ; -> loc_1_1874
        je     loc_1_0910
        test   al, 2
        je     loc_1_0907
        or     eax, 0x200000
loc_1_0907:
        cmp    byte ptr [0x1874], 1          ; -> loc_1_1874
        je     loc_1_0919
loc_1_0910:
        test   al, 8
        je     loc_1_0919
        or     eax, 0x100000
loc_1_0919:
        shr    eax, 0x10
        ret    
sub_1_091D:
        xor    ax, ax
        not    ax
        call   sub_1_068C
        xor    ax, ax
        jmp    sub_1_068C
loc_1_0930:
        call   sub_1_091D
        jmp    sub_1_091D
loc_1_0937:
        test   cl, 0x80
        je     loc_1_0953
        cmp    word ptr [0x1872], ax         ; -> loc_1_1872
        jne    loc_1_0A3C
        call   sub_1_0A9C
        jmp    loc_1_0A3C
loc_1_0953:
        cmp    word ptr [0x1872], ax         ; -> loc_1_1872
        jne    loc_1_0968
        mov    byte ptr [0x186c], 0xff       ; -> loc_1_186C
        jmp    loc_1_0A3C
loc_1_0968:
        call   sub_1_0A9C
        mov    word ptr [0x1872], ax         ; -> loc_1_1872
        cmp    dword ptr [0x1786], 0x7ff8    ; -> obj1:0x1786
        je     loc_1_0996
        cmp    dword ptr [0x1782], 0x7ff8    ; -> obj1:0x1782
        je     loc_1_0996
        cmp    byte ptr [0x1749], 0          ; -> obj1:0x1749
        jne    loc_1_0996
        jne    loc_1_099D
loc_1_0996:
        mov    byte ptr [0x186c], 0xff       ; -> loc_1_186C
loc_1_099D:
        mov    cl, ah
        cbw    
        mov    word ptr [0x187c], ax         ; -> loc_1_187C
        mov    al, cl
        cbw    
        mov    word ptr [0x187e], ax         ; -> loc_1_187E
        mov    ah, byte ptr [0x1762]         ; -> obj1:0x1762
        or     ah, byte ptr [0x1737]         ; -> obj1:0x1737
        or     ah, byte ptr [0x1763]         ; -> loc_1_1763
        test   ah, 0x14
        je     loc_1_0A0B
        mov    eax, dword ptr [0x17a2]       ; -> obj1:0x17A2
        mov    bx, ax
        cmp    word ptr [0x187e], 0          ; -> loc_1_187E
        je     loc_1_09ED
        cmp    word ptr [0x187e], -1         ; -> loc_1_187E
        jne    loc_1_09E7
        neg    ax
loc_1_09E7:
        mov    word ptr [0x187e], ax         ; -> loc_1_187E
loc_1_09ED:
        cmp    word ptr [0x187c], 0          ; -> loc_1_187C
        je     loc_1_0A0B
        cmp    word ptr [0x187c], -1         ; -> loc_1_187C
        jne    loc_1_0A04
        neg    bx
loc_1_0A04:
        mov    word ptr [0x187c], bx         ; -> loc_1_187C
loc_1_0A0B:
        or     word ptr [0x1880], 1          ; -> loc_1_1880
        mov    eax, 0x187a                   ; -> loc_1_187A
        movsx  esi, word ptr [0x187e]        ; -> loc_1_187E
        movsx  edi, word ptr [0x187c]        ; -> loc_1_187C
        mov    ax, word ptr [0x1880]         ; -> loc_1_1880
        call   sub_1_08F0
        call   sub_1_06D5
        VxDCall  VMD.Service_0003h               ; cd2003000c00
loc_1_0A3C:
        ret    
        db  0F7h, 005h
        dd  obj1:0x1796              ; fixup
        db  010h, 000h, 000h, 000h, 074h, 000h, 0E8h, 04Eh, 000h, 000h, 000h, 0C6h, 005h
        dd  loc_1_186E               ; fixup
        db  0FFh, 083h, 00Dh
        dd  obj1:0x1796              ; fixup
        db  001h, 0C6h, 005h
        dd  obj1:0x186F              ; fixup
        db  0FFh, 0C3h, 0F7h, 005h
        dd  obj1:0x1776              ; fixup
        db  010h, 000h, 000h, 000h, 074h, 000h, 083h, 025h
        dd  obj1:0x1796              ; fixup
        db  0FEh, 0C6h, 005h
        dd  obj1:0x186F              ; fixup
        db  000h, 0E8h, 019h, 000h, 000h, 000h, 080h, 03Dh
        dd  obj1:0x1875              ; fixup
        db  000h, 074h, 00Fh, 050h, 051h, 033h, 0C9h, 0B1h, 053h, 033h, 0C0h, 0E8h, 0E2h, 0FDh, 0FFh, 0FFh
        db  059h, 058h, 0C3h
sub_1_0A9C:
        mov    word ptr [0x1872], 0          ; -> loc_1_1872
        mov    byte ptr [0x186c], 0          ; -> loc_1_186C
        mov    byte ptr [0x186d], 0          ; -> obj1:0x186D
        mov    byte ptr [0x1870], 0          ; -> loc_1_1870
        mov    word ptr [0x187e], 0          ; -> loc_1_187E
        mov    word ptr [0x187c], 0          ; -> loc_1_187C
        ret    
sub_1_0ACD:
        mov    al, byte ptr [0x1875]         ; -> obj1:0x1875
        call   sub_1_08F0
        call   sub_1_06D5
        call   sub_1_0A9C
        mov    esi, 0x367                    ; -> loc_4_0367
        mov    edx, ebx
        VMMCall  VMM.Schedule_Global_Event       ; cd200e000100
        ret    
        db  000h
sub_1_0AF0:
        cmp    dword ptr [0x1a18], 0         ; -> loc_1_1A18
        jne    loc_1_0B11
        mov    eax, 0xb0002
        mov    esi, 0xb44                    ; -> loc_1_0B44
        VMMCall  VMM.Hook_Device_Service         ; cd2090000100
        jb     loc_1_0B12
        mov    dword ptr [0x1a18], esi       ; -> loc_1_1A18
loc_1_0B11:
        clc    
loc_1_0B12:
        ret    
sub_1_0B13:
        cmp    dword ptr [0x1a18], 0         ; -> loc_1_1A18
        je     loc_1_0B38
        mov    eax, 0xb0002
        mov    esi, 0xb44                    ; -> loc_1_0B44
        VMMCall  VMM.Unhook_Device_Service       ; cd201c010100
        jb     loc_1_0B39
        mov    dword ptr [0x1a18], 0         ; -> loc_1_1A18
loc_1_0B38:
        clc    
loc_1_0B39:
        ret    
        db  08Bh, 0C0h, 0EBh, 006h, 0FFh, 025h
        dd  loc_1_1A18               ; fixup
loc_1_0B44:
        pushal 
        pushfd 
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        xor    edx, edx
        mov    esi, 0xb61                    ; -> loc_1_0B61
        VMMCall  VMM.Schedule_VM_Event           ; cd200f000100
        popfd  
        popal  
        jmp    loc_1_1A18
loc_1_0B61:
        pushal 
        pushfd 
        test   dword ptr [0x17ec], 1         ; -> obj1:0x17EC
        je     loc_1_0C0D
        test   dword ptr [0x1704], 0x77b     ; -> obj1:0x1704
        je     loc_1_0C0D
        cmp    dword ptr [0x1808], 0         ; -> obj1:0x1808
        jne    loc_1_0B9E
        cmp    dword ptr [0x17f0], 0         ; -> obj1:0x17F0
        jne    loc_1_0B9E
        cmp    dword ptr [0x17fc], 0         ; -> obj1:0x17FC
        je     loc_1_0C0D
loc_1_0B9E:
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf000100
        xchg   dword ptr [0x1a14], eax       ; -> obj1:0x1A14
        sub    eax, dword ptr [0x1a14]       ; -> obj1:0x1A14
        neg    eax
        cmp    eax, 0x3e8
        jl     loc_1_0C0D
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    eax, ebx
        VxDCall  VMD.VMD_Get_Mouse_Owner         ; cd2002000c00
        cmp    eax, ebx
        jne    loc_1_0BE2
        mov    eax, dword ptr [0x1808]       ; -> obj1:0x1808
        cmp    eax, 0
        je     loc_1_0C0D
        mov    ecx, dword ptr [0x180c]       ; -> obj1:0x180C
        call   sub_1_0C10
        jmp    loc_1_0C0D
loc_1_0BE2:
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    dword ptr [0x19f8], ebx       ; -> loc_1_19F8
        cmp    dword ptr [0x17f0], 0         ; -> obj1:0x17F0
        jne    loc_1_0C00
        cmp    dword ptr [0x17fc], 0         ; -> obj1:0x17FC
        je     loc_1_0C0D
loc_1_0C00:
        xor    edx, edx
        mov    esi, 0x4d4                    ; -> loc_4_04D4
        VMMCall  VMM.Schedule_VM_Event           ; cd200f000100
loc_1_0C0D:
        popfd  
        popal  
        ret    
sub_1_0C10:
        cmp    ax, 4
        je     loc_1_0C2D
        mov    word ptr [0x1828], ax         ; -> loc_1_1828
        mov    word ptr [0x182a], cx         ; -> loc_1_182A
        mov    edx, 0
        jmp    loc_1_03B8
loc_1_0C2D:
        jmp    loc_1_03D1
        db  000h, 000h
sub_1_0C34:
        mov    dl, byte ptr [0x1a2e]         ; -> loc_1_1A2E
        mov    byte ptr [0x1a34], dl         ; -> loc_1_1A34
        mov    word ptr [0x1a2e], ax         ; -> loc_1_1A2E
        cmp    al, 0xe0
        jne    loc_1_0C57
        mov    byte ptr [0x1765], 1          ; -> loc_1_1765
        stc    
        jmp    loc_1_0E21
loc_1_0C57:
        cmp    byte ptr [0x1a34], 0xe0       ; -> loc_1_1A34
        jne    loc_1_0C82
        mov    dl, al
        and    dl, 0x7f
        cmp    dl, 0x2a
        jne    loc_1_0C82
        test   al, 0x80
        je     loc_1_0C7C
        cmp    byte ptr [0x1a30], 0          ; -> loc_1_1A30
        je     loc_1_0C7C
        call   sub_1_129E
loc_1_0C7C:
        clc    
        jmp    loc_1_0E21
loc_1_0C82:
        mov    dh, byte ptr [0x1737]         ; -> obj1:0x1737
        mov    dl, byte ptr [0x1a24]         ; -> loc_1_1A24
        mov    byte ptr [0x1a24], dh         ; -> loc_1_1A24
        mov    ah, byte ptr [0x1738]         ; -> loc_1_1738
        cmp    byte ptr [0x1a30], 0          ; -> loc_1_1A30
        je     loc_1_0CA8
        call   sub_1_129E
loc_1_0CA8:
        or     ah, ah
        jne    loc_1_0CB1
        jmp    loc_1_0DB3
loc_1_0CB1:
        test   al, 0x80
        jne    loc_1_0CC5
        mov    byte ptr [0x1a25], 0          ; -> obj1:0x1A25
        test   ah, dl
        je     loc_1_0CC5
        jmp    loc_1_0E06
loc_1_0CC5:
        mov    cl, byte ptr [0x1a26]         ; -> loc_1_1A26
        test   ah, 3
        je     loc_1_0CD4
        inc    cl
        jmp    loc_1_0CD7
loc_1_0CD4:
        xor    cx, cx
loc_1_0CD7:
        mov    byte ptr [0x1a26], cl         ; -> loc_1_1A26
        test   dword ptr [0x17c0], 1         ; -> obj1:0x17C0
        jne    loc_1_0CEE
        jmp    loc_1_0E06
loc_1_0CEE:
        mov    cx, word ptr [0x1762]         ; -> obj1:0x1762
        test   al, 0x80
        je     loc_1_0D0E
        or     ch, cl
        test   ah, ch
        jne    loc_1_0D09
        call   sub_1_1244
        jmp    loc_1_0E06
loc_1_0D09:
        jmp    loc_1_0DFF
loc_1_0D0E:
        cmp    dh, ah
        je     loc_1_0D2F
        call   sub_1_142E
        test   dword ptr [0x17c0], 1         ; -> obj1:0x17C0
        jne    loc_1_0D28
        jmp    loc_1_0E06
loc_1_0D28:
        xor    cx, cx
        mov    cl, dh
        jmp    loc_1_0D5C
loc_1_0D2F:
        mov    bx, 0x303
        test   bl, ah
        jne    loc_1_0D43
        mov    bx, 0x1414
        test   bl, ah
        jne    loc_1_0D43
        mov    bx, 0x2828
loc_1_0D43:
        test   bl, ch
        je     loc_1_0D5C
        not    bx
        and    cx, bx
        push   edx
        mov    dh, ch
        or     dh, cl
        or     dh, ah
        call   sub_1_13EC
        pop    edx
        jmp    loc_1_0D79
loc_1_0D5C:
        mov    bx, cx
        or     bl, bh
        test   ah, ch
        jne    loc_1_0D67
        xor    cl, ah
loc_1_0D67:
        test   dword ptr [0x17c0], 0x80      ; -> obj1:0x17C0
        je     loc_1_0D79
        test   ah, bl
        je     loc_1_0D79
        xor    ch, ah
loc_1_0D79:
        mov    word ptr [0x1762], cx         ; -> obj1:0x1762
        test   dword ptr [0x17c0], 0x40      ; -> obj1:0x17C0
        je     loc_1_0DA6
        test   ah, ch
        jne    loc_1_0D97
        mov    byte ptr [0x1a3b], 1          ; -> obj1:0x1A3B
loc_1_0D97:
        test   ah, ch
        jne    loc_1_0D9F
        test   ah, cl
        je     loc_1_0DA6
loc_1_0D9F:
        mov    byte ptr [0x1a3c], 1          ; -> loc_1_1A3C
loc_1_0DA6:
        test   ah, cl
        jne    loc_1_0DAC
        jmp    loc_1_0DFF
loc_1_0DAC:
        call   sub_1_0E75
        jmp    loc_1_0E06
loc_1_0DB3:
        mov    byte ptr [0x1a26], 0          ; -> loc_1_1A26
        test   dword ptr [0x17c0], 1         ; -> obj1:0x17C0
        je     loc_1_0E06
        or     dh, dh
        je     loc_1_0DD5
        test   al, 0x80
        jne    loc_1_0DD5
        call   sub_1_142E
        jmp    loc_1_0E06
loc_1_0DD5:
        cmp    al, 0xe0
        je     loc_1_0DE0
        mov    byte ptr [0x1a30], 1          ; -> loc_1_1A30
loc_1_0DE0:
        mov    dh, byte ptr [0x1763]         ; -> loc_1_1763
        call   sub_1_13EC
        mov    byte ptr [0x1762], 0          ; -> obj1:0x1762
        call   sub_1_0E75
        mov    ax, word ptr [0x1a2e]         ; -> loc_1_1A2E
        jmp    loc_1_0E21
loc_1_0DFF:
        call   sub_1_0F10
        jmp    loc_1_0E21
loc_1_0E06:
        cmp    byte ptr [0x1765], 0          ; -> loc_1_1765
        je     loc_1_0E21
        call   sub_1_1497
        mov    byte ptr [0x1765], 0          ; -> loc_1_1765
        mov    ax, word ptr [0x1a2e]         ; -> loc_1_1A2E
loc_1_0E21:
        jb     loc_1_0E26
        cmp    al, 0xff
        cmc    
loc_1_0E26:
        pushfd 
        jb     loc_1_0E3E
        cmp    byte ptr [0x1765], 0          ; -> loc_1_1765
        je     loc_1_0E3E
        call   sub_1_1497
        mov    byte ptr [0x1765], 0          ; -> loc_1_1765
loc_1_0E3E:
        cmp    byte ptr [0x1a3b], 0          ; -> obj1:0x1A3B
        je     loc_1_0E53
        mov    byte ptr [0x1a3b], 0          ; -> obj1:0x1A3B
        call   sub_1_144F
loc_1_0E53:
        cmp    byte ptr [0x1a3c], 0          ; -> loc_1_1A3C
        je     loc_1_0E68
        mov    byte ptr [0x1a3c], 0          ; -> loc_1_1A3C
        call   sub_1_1457
loc_1_0E68:
        popfd  
        jb     loc_1_0E74
        cmp    al, 0
        je     loc_1_0E74
        call   sub_1_070A
loc_1_0E74:
        ret    
sub_1_0E75:
        push   eax
        push   ebx
        push   ecx
        xor    eax, eax
        mov    bx, word ptr [0x1762]         ; -> obj1:0x1762
        and    bx, 0x303
        je     loc_1_0EA7
        mov    cx, 8
        or     bl, bh
        cmp    bl, 2
        je     loc_1_0E97
        shl    cx, 3
loc_1_0E97:
        cmp    bh, 0
        je     loc_1_0EA0
        shl    cx, 6
loc_1_0EA0:
        or     ax, cx
        or     ax, 1
loc_1_0EA7:
        mov    bx, word ptr [0x1762]         ; -> obj1:0x1762
        and    bx, 0x1414
        je     loc_1_0ED7
        mov    cx, 0x10
        cmp    byte ptr [0x1a32], 0          ; -> loc_1_1A32
        je     loc_1_0EC6
        shl    cx, 3
loc_1_0EC6:
        cmp    bx, 4
        je     loc_1_0ED0
        shl    cx, 6
loc_1_0ED0:
        or     ax, cx
        or     ax, 2
loc_1_0ED7:
        mov    bx, word ptr [0x1762]         ; -> obj1:0x1762
        and    bx, 0x2828
        je     loc_1_0F07
        mov    cx, 0x20
        cmp    byte ptr [0x1a33], 0          ; -> obj1:0x1A33
        je     loc_1_0EF6
        shl    cx, 3
loc_1_0EF6:
        cmp    bx, 8
        je     loc_1_0F00
        shl    cx, 6
loc_1_0F00:
        or     ax, cx
        or     ax, 4
loc_1_0F07:
        mov    dword ptr [0x171c], eax       ; -> obj1:0x171C
        pop    ecx
        pop    ebx
        pop    eax
        ret    
sub_1_0F10:
        push   ebx
        mov    bl, byte ptr [0x1a27]         ; -> obj1:0x1A27
        xor    bl, byte ptr [0x1762]         ; -> obj1:0x1762
        test   bl, 3
        jne    loc_1_0FA6
        push   eax
        and    al, 0x7f
        cmp    al, 0x2a
        je     loc_1_0F37
        cmp    al, 0x36
        je     loc_1_0F37
        pop    eax
        jmp    loc_1_0FF9
loc_1_0F37:
        pop    eax
        test   dword ptr [0x171c], 0x1249    ; -> obj1:0x171C
        je     loc_1_0FF9
        push   eax
        and    al, 0x7f
        cmp    al, 0x36
        pop    eax
        je     loc_1_0F7D
        test   dword ptr [0x171c], 0x208     ; -> obj1:0x171C
        jne    loc_1_0FF9
        mov    byte ptr [0x1a3a], 1          ; -> loc_1_1A3A
        or     byte ptr [0x1a28], 0x80       ; -> loc_1_1A28
        call   sub_1_132C
        call   sub_1_1244
        jmp    loc_1_11FB
loc_1_0F7D:
        test   dword ptr [0x171c], 0x1040    ; -> obj1:0x171C
        jne    loc_1_0FF9
        mov    byte ptr [0x1a3a], 1          ; -> loc_1_1A3A
        or     byte ptr [0x1a28], 0x80       ; -> loc_1_1A28
        call   sub_1_132C
        call   sub_1_1244
        jmp    loc_1_11FB
loc_1_0FA6:
        cmp    byte ptr [0x1a29], 0          ; -> obj1:0x1A29
        je     loc_1_0FE9
        mov    bh, byte ptr [0x1a28]         ; -> loc_1_1A28
        xor    bh, al
        and    bh, 0x7f
        je     loc_1_0FE9
        xor    bh, bh
        mov    byte ptr [0x1a3c], bh         ; -> loc_1_1A3C
        inc    bh
        mov    byte ptr [0x1a3b], bh         ; -> obj1:0x1A3B
        mov    byte ptr [0x1a3a], bh         ; -> loc_1_1A3A
        or     byte ptr [0x1a2a], 0x80       ; -> loc_1_1A2A
        call   sub_1_132C
        call   sub_1_1244
        dec    byte ptr [0x1a29]             ; -> obj1:0x1A29
loc_1_0FE9:
        inc    byte ptr [0x1a29]             ; -> obj1:0x1A29
        mov    byte ptr [0x1a28], al         ; -> loc_1_1A28
        jmp    loc_1_11FB
loc_1_0FF9:
        test   bl, 0x14
        jne    loc_1_1083
        push   eax
        and    al, 0x7f
        cmp    al, 0x1d
        je     loc_1_100F
        pop    eax
        jmp    loc_1_1100
loc_1_100F:
        pop    eax
        test   dword ptr [0x171c], 0x2492    ; -> obj1:0x171C
        je     loc_1_1100
        cmp    byte ptr [0x1a34], 0xe0       ; -> loc_1_1A34
        je     loc_1_1056
        test   dword ptr [0x171c], 0x410     ; -> obj1:0x171C
        jne    loc_1_1100
        mov    byte ptr [0x1a3a], 1          ; -> loc_1_1A3A
        or     byte ptr [0x1a2a], 0x80       ; -> loc_1_1A2A
        call   sub_1_135E
        call   sub_1_1244
        jmp    loc_1_11FB
loc_1_1056:
        test   dword ptr [0x171c], 0x2080    ; -> obj1:0x171C
        jne    loc_1_1100
        mov    byte ptr [0x1a3a], 1          ; -> loc_1_1A3A
        or     byte ptr [0x1a2a], 0x80       ; -> loc_1_1A2A
        call   sub_1_135E
        call   sub_1_1244
        jmp    loc_1_11FB
loc_1_1083:
        cmp    byte ptr [0x1a2b], 0          ; -> obj1:0x1A2B
        je     loc_1_10D2
        test   dword ptr [0x171c], 0x2080    ; -> obj1:0x171C
        je     loc_1_10A3
        cmp    byte ptr [0x1a34], 0xe0       ; -> loc_1_1A34
        je     loc_1_10D2
        jmp    loc_1_10AC
loc_1_10A3:
        cmp    byte ptr [0x1a34], 0xe0       ; -> loc_1_1A34
        jne    loc_1_10D2
loc_1_10AC:
        call   sub_1_135E
        call   sub_1_1244
        xor    bh, bh
        mov    byte ptr [0x1a3c], bh         ; -> loc_1_1A3C
        inc    bh
        mov    byte ptr [0x1a3b], bh         ; -> obj1:0x1A3B
        mov    byte ptr [0x1a3a], bh         ; -> loc_1_1A3A
        dec    byte ptr [0x1a2b]             ; -> obj1:0x1A2B
loc_1_10D2:
        mov    byte ptr [0x1a32], 0          ; -> loc_1_1A32
        cmp    byte ptr [0x1a34], 0xe0       ; -> loc_1_1A34
        jne    loc_1_10F0
        or     byte ptr [0x1764], 4          ; -> obj1:0x1764
        mov    byte ptr [0x1a32], 1          ; -> loc_1_1A32
loc_1_10F0:
        inc    byte ptr [0x1a2b]             ; -> obj1:0x1A2B
        mov    byte ptr [0x1a2a], al         ; -> loc_1_1A2A
        jmp    loc_1_11FB
loc_1_1100:
        test   bl, 0x28
        jne    loc_1_1183
        push   eax
        and    al, 0x7f
        cmp    al, 0x38
        je     loc_1_1112
        pop    eax
        jmp    loc_1_11FB
loc_1_1112:
        pop    eax
        test   dword ptr [0x171c], 0x4924    ; -> obj1:0x171C
        je     loc_1_11FB
        cmp    byte ptr [0x1a34], 0xe0       ; -> loc_1_1A34
        je     loc_1_1159
        test   dword ptr [0x171c], 0x820     ; -> obj1:0x171C
        jne    loc_1_11FB
        mov    byte ptr [0x1a3a], 1          ; -> loc_1_1A3A
        or     byte ptr [0x1a2c], 0x80       ; -> loc_1_1A2C
        call   sub_1_13A3
        call   sub_1_1244
        jmp    loc_1_11FB
loc_1_1159:
        test   dword ptr [0x171c], 0x4100    ; -> obj1:0x171C
        jne    loc_1_11FB
        mov    byte ptr [0x1a3a], 1          ; -> loc_1_1A3A
        or     byte ptr [0x1a2c], 0x80       ; -> loc_1_1A2C
        call   sub_1_13A3
        call   sub_1_1244
        jmp    loc_1_11FB
loc_1_1183:
        cmp    byte ptr [0x1a2d], 0          ; -> obj1:0x1A2D
        je     loc_1_11D2
        test   dword ptr [0x171c], 0x4100    ; -> obj1:0x171C
        je     loc_1_11A3
        cmp    byte ptr [0x1a34], 0xe0       ; -> loc_1_1A34
        je     loc_1_11D2
        jmp    loc_1_11AC
loc_1_11A3:
        cmp    byte ptr [0x1a34], 0xe0       ; -> loc_1_1A34
        jne    loc_1_11D2
loc_1_11AC:
        call   sub_1_13A3
        call   sub_1_1244
        xor    bh, bh
        mov    byte ptr [0x1a3c], bh         ; -> loc_1_1A3C
        inc    bh
        mov    byte ptr [0x1a3b], bh         ; -> obj1:0x1A3B
        mov    byte ptr [0x1a3a], bh         ; -> loc_1_1A3A
        dec    byte ptr [0x1a2d]             ; -> obj1:0x1A2D
loc_1_11D2:
        mov    byte ptr [0x1a33], 0          ; -> obj1:0x1A33
        cmp    byte ptr [0x1a34], 0xe0       ; -> loc_1_1A34
        jne    loc_1_11F0
        or     byte ptr [0x1764], 8          ; -> obj1:0x1764
        mov    byte ptr [0x1a33], 1          ; -> obj1:0x1A33
loc_1_11F0:
        inc    byte ptr [0x1a2d]             ; -> obj1:0x1A2D
        mov    byte ptr [0x1a2c], al         ; -> loc_1_1A2C
loc_1_11FB:
        mov    bl, byte ptr [0x1762]         ; -> obj1:0x1762
        mov    byte ptr [0x1a27], bl         ; -> obj1:0x1A27
        pop    ebx
        cmp    byte ptr [0x1a3a], 0          ; -> loc_1_1A3A
        jne    loc_1_1218
        mov    byte ptr [0x1765], 0          ; -> loc_1_1765
loc_1_1218:
        call   sub_1_0E75
        pushal 
        mov    edx, dword ptr [0x171c]       ; -> obj1:0x171C
        mov    esi, 0xb3b                    ; -> loc_4_0B3B
        VMMCall  VMM.Schedule_Global_Event       ; cd200e000100
        popal  
        cmp    byte ptr [0x1a3a], 0          ; -> loc_1_1A3A
        jne    loc_1_123B
        stc    
        ret    
loc_1_123B:
        mov    byte ptr [0x1a3a], 0          ; -> loc_1_1A3A
        clc    
        ret    
sub_1_1244:
        push   eax
        xor    ah, ah
        and    al, 0x7f
        cmp    al, 0x2a
        je     loc_1_1251
        cmp    al, 0x36
        jne    loc_1_1268
loc_1_1251:
        mov    byte ptr [0x1a29], ah         ; -> obj1:0x1A29
        mov    byte ptr [0x1a28], ah         ; -> loc_1_1A28
        and    word ptr [0x1762], 0xfcfc     ; -> obj1:0x1762
        jmp    loc_1_129C
loc_1_1268:
        cmp    al, 0x1d
        jne    loc_1_1283
        mov    byte ptr [0x1a2b], ah         ; -> obj1:0x1A2B
        mov    byte ptr [0x1a2a], ah         ; -> loc_1_1A2A
        and    word ptr [0x1762], 0xfbfb     ; -> obj1:0x1762
        jmp    loc_1_129C
loc_1_1283:
        cmp    al, 0x38
        jne    loc_1_129C
        mov    byte ptr [0x1a2d], ah         ; -> obj1:0x1A2D
        mov    byte ptr [0x1a2c], ah         ; -> loc_1_1A2C
        and    word ptr [0x1762], 0xf7f7     ; -> obj1:0x1762
loc_1_129C:
        pop    eax
        ret    
sub_1_129E:
        pushal 
        cmp    byte ptr [0x1a28], 0          ; -> loc_1_1A28
        je     loc_1_12BF
        test   byte ptr [0x1a28], 0x80       ; -> loc_1_1A28
        je     loc_1_12BF
        cmp    byte ptr [0x1a29], 3          ; -> obj1:0x1A29
        jge    loc_1_12BF
        call   sub_1_132C
loc_1_12BF:
        cmp    byte ptr [0x1a2a], 0          ; -> loc_1_1A2A
        je     loc_1_12DF
        test   byte ptr [0x1a2a], 0x80       ; -> loc_1_1A2A
        je     loc_1_12DF
        cmp    byte ptr [0x1a2b], 3          ; -> obj1:0x1A2B
        jge    loc_1_12DF
        call   sub_1_135E
loc_1_12DF:
        cmp    byte ptr [0x1a2c], 0          ; -> loc_1_1A2C
        je     loc_1_12FF
        test   byte ptr [0x1a2c], 0x80       ; -> loc_1_1A2C
        je     loc_1_12FF
        cmp    byte ptr [0x1a2d], 3          ; -> obj1:0x1A2D
        jge    loc_1_12FF
        call   sub_1_13A3
loc_1_12FF:
        mov    bl, byte ptr [0x1762]         ; -> obj1:0x1762
        mov    byte ptr [0x1a27], bl         ; -> obj1:0x1A27
        mov    byte ptr [0x1a30], 0          ; -> loc_1_1A30
        popal  
        jmp    loc_1_1218
        db  08Ah, 01Dh
        dd  obj1:0x1762              ; fixup
        db  088h, 01Dh
        dd  obj1:0x1A27              ; fixup
        db  061h, 0E8h, 0EEh, 0FEh, 0FFh, 0FFh, 0F9h, 0C3h
sub_1_132C:
        push   ebx
        VxDCall  VKD.VKD_Get_Kbd_Owner           ; cd2008000d00
        mov    al, byte ptr [0x1a28]         ; -> loc_1_1A28
        VxDCall  VKD.VKD_Put_Byte                ; cd2013000d00
        jae    loc_1_134E
loc_1_1340:
        mov    bl, byte ptr [0x1762]         ; -> obj1:0x1762
        mov    byte ptr [0x1a27], bl         ; -> obj1:0x1A27
        pop    ebx
        ret    
loc_1_134E:
        mov    byte ptr [0x1a28], 0          ; -> loc_1_1A28
        mov    byte ptr [0x1a29], 0          ; -> obj1:0x1A29
        pop    ebx
        ret    
sub_1_135E:
        push   ebx
        cmp    byte ptr [0x1a32], 1          ; -> loc_1_1A32
        jne    loc_1_1380
        mov    al, 0xe0
        VxDCall  VKD.VKD_Put_Byte                ; cd2013000d00
        jb     loc_1_1340
        mov    byte ptr [0x1a32], 0          ; -> loc_1_1A32
        and    byte ptr [0x1764], 0xeb       ; -> obj1:0x1764
loc_1_1380:
        VxDCall  VKD.VKD_Get_Kbd_Owner           ; cd2008000d00
        mov    al, byte ptr [0x1a2a]         ; -> loc_1_1A2A
        VxDCall  VKD.VKD_Put_Byte                ; cd2013000d00
        jb     loc_1_1340
        mov    byte ptr [0x1a2a], 0          ; -> loc_1_1A2A
        mov    byte ptr [0x1a2b], 0          ; -> obj1:0x1A2B
        pop    ebx
        ret    
sub_1_13A3:
        push   ebx
        cmp    byte ptr [0x1a33], 1          ; -> obj1:0x1A33
        jne    loc_1_13C5
        mov    al, 0xe0
        VxDCall  VKD.VKD_Put_Byte                ; cd2013000d00
        jb     loc_1_1340
        mov    byte ptr [0x1a33], 0          ; -> obj1:0x1A33
        and    byte ptr [0x1764], 0xd7       ; -> obj1:0x1764
loc_1_13C5:
        VxDCall  VKD.VKD_Get_Kbd_Owner           ; cd2008000d00
        mov    al, byte ptr [0x1a2c]         ; -> loc_1_1A2C
        VxDCall  VKD.VKD_Put_Byte                ; cd2013000d00
        jb     loc_1_1340
        mov    byte ptr [0x1a2c], 0          ; -> loc_1_1A2C
        mov    byte ptr [0x1a2d], 0          ; -> obj1:0x1A2D
        pop    ebx
        ret    
sub_1_13EC:
        push   edx
        push   ecx
        push   eax
        mov    dl, byte ptr [0x1762]         ; -> obj1:0x1762
        or     dl, byte ptr [0x1763]         ; -> loc_1_1763
        mov    edi, edx
        mov    esi, 0x174a                   ; -> loc_1_174A
        mov    ecx, 6
loc_1_1407:
        mov    edx, edi
        lodsw  ax, word ptr [esi]
        mov    bx, ax
        and    dx, bx
        lodsw  ax, word ptr [esi]
        xor    dh, dl
        je     loc_1_1428
        test   bl, dl
        je     loc_1_141D
        or     al, 0x80
loc_1_141D:
        push   edi
        push   esi
        push   ecx
        call   sub_1_070A
        pop    ecx
        pop    esi
        pop    edi
loc_1_1428:
        loop   loc_1_1407
        pop    eax
        pop    ecx
        pop    edx
        ret    
sub_1_142E:
        test   dword ptr [0x17c0], 0x100     ; -> obj1:0x17C0
        je     loc_1_144E
        and    dword ptr [0x17c0], 0xfffffffe ; -> obj1:0x17C0
        mov    esi, 0xb0c                    ; -> loc_4_0B0C
        mov    edx, ebx
        VMMCall  VMM.Schedule_Global_Event       ; cd200e000100
loc_1_144E:
        ret    
sub_1_144F:
        pushal 
        mov    eax, 4
        jmp    loc_1_145F
sub_1_1457:
        pushal 
        mov    eax, 3
        jmp    loc_1_145F
loc_1_145F:
        test   dword ptr [0x17c0], 0x40      ; -> obj1:0x17C0
        je     loc_1_1476
        mov    ecx, 1
        VxDCall  ENABLE.VMINI_Siren              ; cd2005003700
loc_1_1476:
        popal  
        ret    
sub_1_1478:
        pushal 
        test   dword ptr [0x17c0], 0x40      ; -> obj1:0x17C0
        je     loc_1_1495
        mov    eax, 5
        mov    ecx, 1
        VxDCall  ENABLE.VMINI_Siren              ; cd2005003700
loc_1_1495:
        popal  
        ret    
sub_1_1497:
        pushal 
        VxDCall  VKD.VKD_Get_Kbd_Owner           ; cd2008000d00
        mov    al, 0xe0
        call   sub_1_070A
        jb     loc_1_14AF
        mov    al, 0xe0
        VxDCall  VKD.VKD_Put_Byte                ; cd2013000d00
loc_1_14AF:
        popal  
        ret    
loc_1_14B1:
        push   eax
        VxDCall  VKD.VKD_Get_Kbd_Owner           ; cd2008000d00
        cmp    byte ptr [0x1a28], 0          ; -> loc_1_1A28
        je     loc_1_14EE
        or     byte ptr [0x1a28], 0x80       ; -> loc_1_1A28
        push   ebx
        VxDCall  VKD.VKD_Get_Kbd_Owner           ; cd2008000d00
        mov    esi, 0x1a28                   ; -> loc_1_1A28
        mov    ecx, 1
        VxDCall  VKD.VKD_Force_Keys              ; cd2007000d00
        mov    byte ptr [0x1a28], 0          ; -> loc_1_1A28
        mov    byte ptr [0x1a29], 0          ; -> obj1:0x1A29
        pop    ebx
loc_1_14EE:
        cmp    byte ptr [0x1a2a], 0          ; -> loc_1_1A2A
        je     loc_1_1539
        or     byte ptr [0x1a2a], 0x80       ; -> loc_1_1A2A
        push   ebx
        test   byte ptr [0x1764], 4          ; -> obj1:0x1764
        je     loc_1_1514
        call   sub_1_1497
        and    byte ptr [0x1764], 0xeb       ; -> obj1:0x1764
loc_1_1514:
        VxDCall  VKD.VKD_Get_Kbd_Owner           ; cd2008000d00
        mov    esi, 0x1a2a                   ; -> loc_1_1A2A
        mov    ecx, 1
        VxDCall  VKD.VKD_Force_Keys              ; cd2007000d00
        mov    byte ptr [0x1a2a], 0          ; -> loc_1_1A2A
        mov    byte ptr [0x1a2b], 0          ; -> obj1:0x1A2B
        pop    ebx
loc_1_1539:
        cmp    byte ptr [0x1a2c], 0          ; -> loc_1_1A2C
        je     loc_1_1582
        or     byte ptr [0x1a2c], 0x80       ; -> loc_1_1A2C
        test   byte ptr [0x1764], 8          ; -> obj1:0x1764
        je     loc_1_155E
        call   sub_1_1497
        and    byte ptr [0x1764], 0xd7       ; -> obj1:0x1764
loc_1_155E:
        VxDCall  VKD.VKD_Get_Kbd_Owner           ; cd2008000d00
        mov    esi, 0x1a2c                   ; -> loc_1_1A2C
        mov    ecx, 1
        VxDCall  VKD.VKD_Force_Keys              ; cd2007000d00
        mov    byte ptr [0x1a2c], 0          ; -> loc_1_1A2C
        mov    byte ptr [0x1a2d], 0          ; -> obj1:0x1A2D
loc_1_1582:
        pop    eax
        cmp    eax, 1
        jne    loc_1_15AC
        pushal 
        mov    byte ptr [0x1a26], 0          ; -> loc_1_1A26
        mov    byte ptr [0x1a25], 0          ; -> obj1:0x1A25
        mov    dh, byte ptr [0x1737]         ; -> obj1:0x1737
        call   sub_1_13EC
        mov    word ptr [0x1762], 0          ; -> obj1:0x1762
        popal  
loc_1_15AC:
        jmp    loc_1_1218
        db  000h, 000h, 000h, 0C7h, 045h, 01Ch, 000h, 004h, 000h, 000h, 066h, 083h, 065h, 02Ch, 0FEh, 0C3h
        db  0B0h, 001h, 0C3h, 08Ah, 0E8h, 0B1h, 001h, 0BAh, 0FFh, 0FFh, 0FFh, 0FFh, 0CDh, 020h, 007h, 000h
        db  00Dh, 000h, 072h, 003h, 0B0h, 001h, 0C3h, 0B0h, 000h, 0C3h, 03Ch, 000h, 075h, 002h, 0EBh, 000h
        db  0F8h, 0C3h, 0F9h, 0C3h, 0C3h, 0C3h, 000h
sub_1_15E8:
        push   eax
        test   dword ptr [0x178e], 1         ; -> obj1:0x178E
        je     loc_1_1642
        cmp    byte ptr [0x1a44], 0xff       ; -> loc_1_1A44
        je     loc_1_1642
        VxDCall  VKD.VKD_Get_Kbd_Owner           ; cd2008000d00
        VxDCall  VKD.VKD_Get_Shift_State         ; cd2011000d00
        mov    bl, al
        mov    cl, al
        mov    bh, byte ptr [0x1a44]         ; -> loc_1_1A44
        test   dword ptr [0x1796], 1         ; -> obj1:0x1796
        je     loc_1_1628
        and    bl, 0x50
        and    bh, 0x50
        jmp    loc_1_162E
loc_1_1628:
        and    bl, 0x70
        and    bh, 0x70
loc_1_162E:
        xor    bh, bl
        je     loc_1_1642
        and    bh, bl
        je     loc_1_163D
        call   sub_1_1653
        jmp    loc_1_1642
loc_1_163D:
        call   sub_1_164B
loc_1_1642:
        mov    byte ptr [0x1a44], cl         ; -> loc_1_1A44
        pop    eax
        clc    
        ret    
sub_1_164B:
        pushal 
        mov    eax, 4
        jmp    loc_1_165B
sub_1_1653:
        pushal 
        mov    eax, 3
        jmp    loc_1_165B
loc_1_165B:
        mov    ecx, 1
        VxDCall  ENABLE.VMINI_Siren              ; cd2005003700
        popal  
        ret    
        db  0CBh, 000h, 000h, 000h
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  0, 128                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'ENABLE2 '                  ; DDB_Name
        dd  034000000h                  ; DDB_Init_Order
        dd  ENABLE2_Control          ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_16FC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_172B:
        cmp    al, ah
        sbb    eax, 0x1d0038e0
        add    byte ptr [edx], ch
        add    byte ptr [esi], dh
        add    byte ptr [eax], al
loc_1_1738:
        add    byte ptr [eax], al
loc_1_173A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [esi], al
loc_1_1746:
        add    byte ptr [eax], al
loc_1_1748:
        add    byte ptr [eax], al
loc_1_174A:
        add    al, byte ptr [edx]
        sub    al, byte ptr [eax]
        add    dword ptr [ecx], eax
        add    byte ptr ss:[esp + eax], al
        sbb    eax, 0x1d101000
        loopne loc_1_1763
        or     byte ptr [eax], bh
        add    byte ptr [eax], ah
        and    byte ptr [eax], bh
        loopne loc_1_1763
loc_1_1763:
        add    byte ptr [eax], al
loc_1_1765:
        add    al, ah
loc_1_1767:
        add    byte ptr [eax], al
sub_1_1769:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_176D:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1771:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
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
sub_1_1824:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1828:
        add    byte ptr [eax], al
loc_1_182A:
        add    byte ptr [eax], al
loc_1_182C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1830:
        add    al, 0
        add    byte ptr [eax], al
loc_1_1834:
        sub    byte ptr [eax], bl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [eax], al
        sub    al, 0x18
        add    byte ptr [eax], al
loc_1_1850:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1854:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1858:
        push   ebx
        outsd  dx, dword ptr [esi]
        jne    loc_1_18CA
        push   ebx
        outsb  dx, byte ptr gs:[esi]
        je     0x18d4
        jns    loc_1_18B4
        jb     loc_1_18D5
        arpl   word ptr [eax], ax
loc_1_1868:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_186C:
        add    byte ptr [eax], al
loc_1_186E:
        add    bh, bh
loc_1_1870:
        add    byte ptr [eax], al
loc_1_1872:
        add    byte ptr [eax], al
loc_1_1874:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_187A:
        add    al, byte ptr [eax]
loc_1_187C:
        add    byte ptr [eax], al
loc_1_187E:
        add    byte ptr [eax], al
loc_1_1880:
        add    byte ptr [eax], al
loc_1_1882:
        add    byte ptr [ebp], al
        xor    eax, 0x4a0037e0
        add    byte ptr [esi], cl
        sbb    al, 0xe0
        push   ebx
        add    byte ptr [ecx], cl
        dec    eax
        add    byte ptr [edi], al
        dec    ebp
        add    byte ptr [eax + eax + 0x4b], cl
        add    byte ptr [ecx], dl
        push   eax
        add    byte ptr [edi], cl
        push   edx
        db  000h
sub_1_18A5:
        jnp    0x18af
        add    byte ptr [eax], al
        aaa    
        or     dword ptr [eax], eax
        add    byte ptr [edi], dh            ; -> loc_1_0937
        or     dword ptr [eax], eax
        add    byte ptr [edi], dh            ; -> loc_1_0937
        or     dword ptr [eax], eax
loc_1_18B4:
        add    byte ptr [edi], dh            ; -> loc_1_0937
        or     dword ptr [eax], eax
        add    byte ptr [0x37000009], bl     ; -> sub_1_091D
        or     dword ptr [eax], eax
        add    byte ptr [edi], dh            ; -> loc_1_0937
        or     dword ptr [eax], eax
        add    byte ptr [edi], dh            ; -> loc_1_0937
        or     dword ptr [eax], eax
        add    byte ptr [edi], dh            ; -> loc_1_0937
loc_1_18CA:
        or     dword ptr [eax], eax
        add    byte ptr [ebx + 8], bh        ; -> sub_1_087B
        add    byte ptr [eax], al
        in     eax, 7
        add    byte ptr [eax], al
loc_1_18D5:
        xor    byte ptr [ecx], cl
        add    byte ptr [eax], al
        pop    edx
        or     byte ptr [eax], al
        add    byte ptr [edx + 8], bl        ; -> loc_1_085A
        add    byte ptr [eax], al
        pop    edx
        or     byte ptr [eax], al
        add    cl, ch                        ; -> loc_1_07E9
        pop    es
        add    byte ptr [eax], al
loc_1_18E9:
        add    dword ptr [eax], eax
        add    edi, edi
        add    dword ptr [eax], eax
        add    dword ptr [ecx], eax
        add    bh, bh
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        db  0FFh, 0FFh, 0FFh, 000h, 0FFh, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 002h, 000h, 003h, 000h
        db  001h, 000h, 000h, 000h, 000h, 002h, 008h, 00Ah, 004h, 000h, 00Ch, 008h, 010h, 012h, 000h, 002h
        db  014h, 010h, 004h, 000h, 0FFh, 000h, 0FFh, 001h, 0FEh, 000h, 0FFh, 000h, 0FFh, 002h, 0FFh, 003h
        db  0FEh, 002h, 0FFh, 002h, 0FDh, 000h, 0FDh, 001h, 0FCh, 000h, 0FDh, 000h, 0FFh, 000h, 0FFh, 001h
        db  0FEh, 000h, 0FFh, 000h
loc_1_193B:
        pop    edx
loc_1_193C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1940:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1944:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_19C4:
        adc    al, byte ptr [edx]
        add    al, byte ptr [ebx]
        add    al, byte ptr [edx]
        add    al, byte ptr [ebx]
        add    al, byte ptr [edx]
        add    al, byte ptr [ebx]
        add    al, byte ptr [edx]
        add    al, byte ptr [ebx]
        add    al, byte ptr [edx]
        add    al, byte ptr [ebx]
        add    al, byte ptr [edx]
        add    byte ptr [eax], al
loc_1_19DC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_19F8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_19FC:
        add    byte ptr [eax], al
loc_1_19FE:
        add    byte ptr [eax], al
loc_1_1A00:
        add    byte ptr [eax], al
loc_1_1A02:
        add    byte ptr [eax], al
loc_1_1A04:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1A08:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1A0C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1A10:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh
loc_1_1A18:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1A20:
        add    byte ptr [eax], al
loc_1_1A22:
        add    byte ptr [eax], al
loc_1_1A24:
        add    byte ptr [eax], al
loc_1_1A26:
        add    byte ptr [eax], al
loc_1_1A28:
        add    byte ptr [eax], al
loc_1_1A2A:
        add    byte ptr [eax], al
loc_1_1A2C:
        add    byte ptr [eax], al
loc_1_1A2E:
        add    byte ptr [eax], al
loc_1_1A30:
        add    byte ptr [eax], al
loc_1_1A32:
        add    byte ptr [eax], al
loc_1_1A34:
        add    byte ptr [eax], al
        loopne 0x1a55
        loopne 0x1a72
loc_1_1A3A:
        add    byte ptr [eax], al
loc_1_1A3C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh
loc_1_1A44:
        inc    dword ptr [eax]
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x83  R-X
        db  005h, 000h, 000h, 000h
loc_2_0004:
        sbb    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
loc_2_000C:
        add    byte ptr [eax], al
loc_2_000E:
        or     al, 0
        sbb    eax, 0x4d003600
        add    byte ptr [ebx], ch
loc_2_0018:
        inc    ebp
        dec    esi
        inc    ecx
        inc    edx
        dec    esp
        inc    ebp
        xor    ebp, dword ptr [esi]
        inc    esp
        dec    esp
        dec    esp
        add    byte ptr [eax + 0x45], cl
        dec    esp
        push   eax
        inc    ebp
        push   edx
        pop    edi
        inc    esi
        dec    esp
        inc    ecx
        push   ebx
        dec    eax
        dec    ecx
        push   esp
        inc    ebp
        dec    ebp
        add    byte ptr [eax + 0x45], cl
        dec    esp
        push   eax
        inc    ebp
        push   edx
        pop    edi
        inc    ebx
        dec    esp
        inc    ebp
        inc    ecx
        push   edx
        dec    eax
        dec    ecx
        inc    edi
        dec    eax
        inc    ebx
        dec    edi
        dec    esi
        push   esp
        push   edx
        inc    ecx
        push   ebx
        push   esp
        add    byte ptr [eax + 0x45], cl
        dec    esp
        push   eax
        inc    ebp
        push   edx
        pop    edi
        push   ebx
        inc    ebp
        push   esp
        dec    eax
        dec    ecx
        inc    edi
        dec    eax
        inc    ebx
        dec    edi
        dec    esi
        push   esp
        push   edx
        inc    ecx
        push   ebx
        push   esp
        add    byte ptr [eax + 0x45], cl
        dec    esp
        push   eax
        inc    ebp
        push   edx
        pop    edi
        inc    ebx
        push   edx
        inc    ebp
        inc    ecx
        push   esp
        inc    ebp
        push   esp
        push   edx
        inc    ecx
        dec    esi
        push   ebx
        dec    esp
        inc    ecx
        push   esp
        dec    ecx
        dec    edi
        dec    esi
        push   esp
        inc    ecx
        inc    edx
        dec    esp
        inc    ebp
        db  000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x224  R-X
loc_3_0000:
        call   sub_3_00AB
        jb     loc_3_0098
        lea    edi, [0x16bc]                 ; -> obj1:0x16BC
        mov    dword ptr [0x16e0], 0x171c    ; -> obj1:0x16E0
        mov    eax, 0x1db                    ; -> sub_1_01DB
        mov    dword ptr [0x16e4], eax       ; -> obj1:0x16E4
        VxDCall  ENABLE.VMINI_RegisterAccess     ; cd2006003700
        mov    eax, dword ptr [0x16dc]       ; -> obj1:0x16DC
        mov    dword ptr [0x1700], eax       ; -> obj1:0x1700
        mov    eax, dword ptr [0x16e8]       ; -> obj1:0x16E8
        mov    dword ptr [0x170c], eax       ; -> obj1:0x170C
        mov    eax, dword ptr [0x16ec]       ; -> obj1:0x16EC
        mov    dword ptr [0x1714], eax       ; -> obj1:0x1714
        mov    eax, dword ptr [0x16f0]       ; -> obj1:0x16F0
        mov    dword ptr [0x1718], eax       ; -> obj1:0x1718
        mov    eax, dword ptr [0x16f4]       ; -> obj1:0x16F4
        mov    dword ptr [0x1710], eax       ; -> obj1:0x1710
        mov    eax, dword ptr [0x16f8]       ; -> obj1:0x16F8
        mov    dword ptr [0x1850], eax       ; -> loc_1_1850
        mov    eax, dword ptr [0x16d4]       ; -> obj1:0x16D4
        mov    dword ptr [0x1824], eax       ; -> sub_1_1824
        mov    eax, 0x370001
        mov    esi, 8                        ; -> loc_1_0008
        VMMCall  VMM.Hook_Device_Service         ; cd2090000100
        mov    dword ptr [0x16fc], esi       ; -> sub_1_16FC
        jb     loc_3_0098
        call   sub_1_0AF0
        jb     loc_3_0098
        mov    dword ptr [0x1769], esi       ; -> sub_1_1769
        clc    
        ret    
loc_3_0098:
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    eax, 5
        VxDCall  ENABLE.VMINI_DisplayError       ; cd2003003700
        stc    
        ret    
sub_3_00AB:
        mov    dword ptr [0x1772], 0x18      ; -> obj1:0x1772
        mov    dword ptr [0x178a], 8         ; -> obj1:0x178A
        mov    dword ptr [0x1792], 0x1c      ; -> obj1:0x1792
        mov    dword ptr [0x17ae], 0xe       ; -> obj1:0x17AE
        mov    dword ptr [0x17bc], 8         ; -> obj1:0x17BC
        mov    dword ptr [0x17c4], 0xc       ; -> obj1:0x17C4
        mov    dword ptr [0x17d0], 0x18      ; -> obj1:0x17D0
        mov    dword ptr [0x17e8], 0x30      ; -> obj1:0x17E8
        mov    dword ptr [0x1818], 0xc       ; -> obj1:0x1818
        mov    ebx, 0x100
        mov    edi, 0x1772                   ; -> obj1:0x1772
        VxDCall  ENABLE.VMINI_GetData            ; cd2007003700
        mov    ebx, 0x200
        mov    edi, 0x178a                   ; -> obj1:0x178A
        VxDCall  ENABLE.VMINI_GetData            ; cd2007003700
        mov    ebx, 0x40
        mov    edi, 0x1792                   ; -> obj1:0x1792
        VxDCall  ENABLE.VMINI_GetData            ; cd2007003700
        pushal 
        call   sub_1_0ACD
        popal  
        mov    ebx, 0x400
        mov    edi, 0x17ae                   ; -> obj1:0x17AE
        VxDCall  ENABLE.VMINI_GetData            ; cd2007003700
        mov    ebx, 1
        mov    edi, 0x17bc                   ; -> obj1:0x17BC
        VxDCall  ENABLE.VMINI_GetData            ; cd2007003700
        mov    ebx, 0x1000
        mov    edi, 0x17c4                   ; -> obj1:0x17C4
        VxDCall  ENABLE.VMINI_GetData            ; cd2007003700
        mov    ebx, 0x80
        mov    edi, 0x17d0                   ; -> obj1:0x17D0
        VxDCall  ENABLE.VMINI_GetData            ; cd2007003700
        mov    ebx, 8
        mov    edi, 0x17e8                   ; -> obj1:0x17E8
        VxDCall  ENABLE.VMINI_GetData            ; cd2007003700
        mov    ebx, 0x800
        mov    edi, 0x1818                   ; -> obj1:0x1818
        VxDCall  ENABLE.VMINI_GetData            ; cd2007003700
        VxDCall  ENABLE.VMINI_Status             ; cd2002003700
        mov    dword ptr [0x1704], eax       ; -> obj1:0x1704
        mov    dword ptr [0x16bc], 0x1e5     ; -> obj1:0x16BC
        mov    dword ptr [0x16d4], 0         ; -> obj1:0x16D4
        mov    dword ptr [0x16d8], 0x14b1    ; -> obj1:0x16D8
        VxDCall  VKD.VKD_Get_Kbd_Owner           ; cd2008000d00
        VxDCall  VKD.VKD_Get_Shift_State         ; cd2011000d00
        and    al, 0xf
        mov    byte ptr [0x1738], al         ; -> loc_1_1738
        mov    byte ptr [0x1737], al         ; -> obj1:0x1737
        mov    byte ptr [0x1a24], al         ; -> loc_1_1A24
        clc    
        ret    
        db  043h, 06Fh, 070h, 079h, 072h, 069h, 067h, 068h, 074h, 020h, 028h, 043h, 029h, 020h, 04Dh, 069h
        db  063h, 072h, 06Fh, 073h, 06Fh, 066h, 074h, 020h, 043h, 06Fh, 072h, 070h, 02Eh, 020h, 031h, 039h
        db  039h, 033h, 02Ch, 020h, 031h, 039h, 039h, 034h, 02Eh, 020h, 020h, 041h, 06Ch, 06Ch, 020h, 072h
        db  069h, 067h, 068h, 074h, 073h, 020h, 072h, 065h, 073h, 065h, 072h, 076h, 065h, 064h, 02Eh, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0xbd8  R-X
loc_4_0000:
        mov    edx, dword ptr [esp + 4]
        cmp    edx, 4
        jae    loc_4_005D
        pushal 
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        movzx  eax, word ptr [edx*2 + 0xe]   ; -> loc_2_000E
        add    eax, dword ptr [4]            ; -> loc_2_0004
        movzx  ebx, word ptr [0xc]           ; -> loc_2_000C
        add    ebx, dword ptr [4]            ; -> loc_2_0004
        push   dword ptr [edx*4 + 0x1834]    ; -> loc_1_1834
        push   dword ptr [edx*4 + 0x1830]    ; -> loc_1_1830
        push   eax
        push   ebx
        VxDCall  SHELL._SHELL_CallDll            ; cd2018001700
        add    esp, 0x10
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        popal  
loc_4_005D:
        ret    
loc_4_005E:
        mov    edx, dword ptr [esp + 4]
        mov    dword ptr [0x1854], edx       ; -> loc_1_1854
        pushal 
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        mov    eax, dword ptr [0x1850]       ; -> loc_1_1850
        or     eax, eax
        je     loc_4_009C
        cmp    byte ptr [eax], 0
        je     loc_4_009C
        push   0x1854                        ; -> loc_1_1854
        push   4
        push   0x1858                        ; -> loc_1_1858
        push   eax
        VxDCall  SHELL._SHELL_CallDll            ; cd2018001700
        add    esp, 0x10
loc_4_009C:
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        popal  
        ret    
        db  000h, 000h, 000h
sub_4_00B0:
        test   dword ptr [0x1776], 1         ; -> obj1:0x1776
        jne    loc_4_00C1
        jmp    loc_4_0184
loc_4_00C1:
        mov    ax, word ptr [0x1726]         ; -> obj1:0x1726
        sub    ax, 0x37
        je     loc_4_00D4
        jb     loc_4_00D4
        jmp    loc_4_017E
loc_4_00D4:
        mov    word ptr [0x1726], 0          ; -> obj1:0x1726
        cmp    dword ptr [0x1786], 0x7ff8    ; -> obj1:0x1786
        je     loc_4_00EE
        jmp    loc_4_0184
loc_4_00EE:
        mov    ax, word ptr [0x1724]         ; -> obj1:0x1724
        or     ax, ax
        jne    loc_4_00FE
        jmp    loc_4_0184
loc_4_00FE:
        test   dword ptr [0x1776], 0x40      ; -> obj1:0x1776
        je     loc_4_0121
        and    al, 0x7f
        cmp    al, 0x36
        je     loc_4_0121
        cmp    al, 0x2a
        je     loc_4_0121
        cmp    al, 0x1d
        je     loc_4_0121
        cmp    al, 0x38
        je     loc_4_0121
        call   sub_1_1478
loc_4_0121:
        push   eax
        cmp    byte ptr [0x1725], 0xe0       ; -> obj1:0x1725
        jne    loc_4_0134
        push   eax
        mov    al, 0xe0
        call   sub_1_0C34
        pop    eax
loc_4_0134:
        call   sub_1_0C34
        pop    eax
        jb     loc_4_0151
        mov    byte ptr [0x1868], al         ; -> loc_1_1868
        mov    ecx, 1
        mov    esi, 0x1868                   ; -> loc_1_1868
        VxDCall  VKD.VKD_Force_Keys              ; cd2007000d00
loc_4_0151:
        mov    eax, dword ptr [0x1782]       ; -> obj1:0x1782
        cmp    byte ptr [0x1728], 0          ; -> obj1:0x1728
        jne    loc_4_017E
        mov    byte ptr [0x1728], 0xff       ; -> obj1:0x1728
        cmp    dword ptr [0x1786], 0x7ff8    ; -> obj1:0x1786
        je     loc_4_0179
        mov    eax, dword ptr [0x177e]       ; -> obj1:0x177E
        jmp    loc_4_017E
loc_4_0179:
        mov    eax, dword ptr [0x177e]       ; -> obj1:0x177E
loc_4_017E:
        mov    word ptr [0x1726], ax         ; -> obj1:0x1726
loc_4_0184:
        ret    
        db  000h, 000h, 000h
sub_4_0188:
        test   dword ptr [0x1796], 1         ; -> obj1:0x1796
        je     loc_4_02D2
        call   sub_1_06EB
        jae    loc_4_01A4
        jmp    loc_4_02D2
loc_4_01A4:
        cmp    byte ptr [0x186c], 0          ; -> loc_1_186C
        je     loc_4_02D2
        test   dword ptr [0x1796], 0x40      ; -> obj1:0x1796
        jne    loc_4_01F0
        mov    ch, byte ptr [0x1762]         ; -> obj1:0x1762
        or     ch, byte ptr [0x1737]         ; -> obj1:0x1737
        or     ch, byte ptr [0x1763]         ; -> loc_1_1763
        test   ch, 0x14
        je     loc_4_01F0
        mov    eax, dword ptr [0x193c]       ; -> loc_1_193C
        or     eax, eax
        je     loc_4_01E9
        dec    eax
        mov    dword ptr [0x193c], eax       ; -> loc_1_193C
        jne    loc_4_02D2
loc_4_01E9:
        mov    eax, dword ptr [0x17a2]       ; -> obj1:0x17A2
        jmp    loc_4_026B
loc_4_01F0:
        test   ch, 3
        jne    loc_4_01F7
        jmp    loc_4_0213
loc_4_01F7:
        mov    eax, dword ptr [0x193c]       ; -> loc_1_193C
        or     eax, eax
        je     loc_4_020C
        dec    eax
        mov    dword ptr [0x193c], eax       ; -> loc_1_193C
        jne    loc_4_02D2
loc_4_020C:
        mov    eax, 1
        jmp    loc_4_026B
loc_4_0213:
        xor    eax, eax
        xor    ebx, ebx
        cmp    byte ptr [0x186d], 0          ; -> obj1:0x186D
        jne    loc_4_024B
        mov    bl, byte ptr [0x1870]         ; -> loc_1_1870
        cmp    bl, byte ptr [0x193b]         ; -> loc_1_193B
        jb     loc_4_0239
        mov    byte ptr [0x186d], 0xff       ; -> obj1:0x186D
        xor    ebx, ebx
        jmp    loc_4_025B
loc_4_0239:
        mov    al, byte ptr [ebx + 0x1944]   ; -> loc_1_1944
        cbw    
        inc    bx
        mov    byte ptr [0x1870], bl         ; -> loc_1_1870
        jmp    loc_4_026B
loc_4_024B:
        mov    bl, byte ptr [0x1871]         ; -> obj1:0x1871
        cmp    bl, byte ptr [0x19c4]         ; -> loc_1_19C4
        jb     loc_4_025B
        xor    ebx, ebx
loc_4_025B:
        mov    al, byte ptr [ebx + 0x19c5]   ; -> obj1:0x19C5
        cbw    
        inc    bx
        mov    byte ptr [0x1871], bl         ; -> obj1:0x1871
loc_4_026B:
        or     ax, ax
        je     loc_4_02D2
        xor    bx, bx
        cmp    byte ptr [0x1873], bl         ; -> obj1:0x1873
        je     loc_4_0283
        mov    bx, ax
        jg     loc_4_0283
        neg    bx
loc_4_0283:
        xor    cx, cx
        cmp    byte ptr [0x1872], cl         ; -> loc_1_1872
        je     loc_4_0296
        mov    cx, ax
        jg     loc_4_0296
        neg    cx
loc_4_0296:
        mov    word ptr [0x187c], cx         ; -> loc_1_187C
        mov    word ptr [0x187e], bx         ; -> loc_1_187E
        or     word ptr [0x1880], 1          ; -> loc_1_1880
        movsx  esi, word ptr [0x187e]        ; -> loc_1_187E
        movsx  edi, word ptr [0x187c]        ; -> loc_1_187C
        mov    ax, word ptr [0x1880]         ; -> loc_1_1880
        call   sub_1_08F0
        call   sub_1_06D5
        VxDCall  VMD.Service_0003h               ; cd2003000c00
        jmp    loc_4_02D2
loc_4_02D2:
        ret    
loc_4_02D3:
        mov    dword ptr [0x19f1], 0         ; -> obj1:0x19F1
        mov    al, dl
        cmp    al, 0xfe
        jne    loc_4_02EE
        and    byte ptr [0x19e9], 0xef       ; -> obj1:0x19E9
        xor    ebx, ebx
        jmp    loc_4_032C
loc_4_02EE:
        cmp    al, 0xff
        jne    loc_4_02FD
        or     byte ptr [0x19e9], 0x10       ; -> obj1:0x19E9
        xor    ebx, ebx
        jmp    loc_4_032C
loc_4_02FD:
        shr    al, 2
        and    al, 0xc
        mov    ah, byte ptr [0x1874]         ; -> loc_1_1874
        and    ah, 3
        or     al, ah
        cmp    byte ptr [0x186f], 0          ; -> obj1:0x186F
        jne    loc_4_0318
        or     al, 0x10
loc_4_0318:
        mov    byte ptr [0x19e9], al         ; -> obj1:0x19E9
        cmp    al, byte ptr [0x19ed]         ; -> obj1:0x19ED
        je     loc_4_0366
        mov    byte ptr [0x19ed], al         ; -> obj1:0x19ED
        xor    ebx, ebx
loc_4_032C:
        mov    ebp, dword ptr [0x1710]       ; -> obj1:0x1710
        cmp    dword ptr [ebp + ebx*8], 0xffff
        je     loc_4_0360
        mov    eax, dword ptr [ebp + ebx*8]
        mov    ecx, dword ptr [ebp + ebx*8 + 4]
        push   ebx
        push   edx
        mov    esi, dword ptr [0x19e9]       ; -> obj1:0x19E9
        push   0
        push   0
        push   esi
        push   2
        push   ecx
        push   eax
        VxDCall  SHELL._SHELL_PostMessage        ; cd2006001700
        add    esp, 0x18
        pop    edx
        pop    ebx
loc_4_0360:
        inc    ebx
        cmp    ebx, 8
        jl     loc_4_032C
loc_4_0366:
        ret    
loc_4_0367:
        xor    edx, edx
        mov    eax, dword ptr [0x179a]       ; -> obj1:0x179A
        mov    bx, 0x12
        cmp    ax, bx
        jae    loc_4_037A
        mov    ax, bx
loc_4_037A:
        div    bx
        mov    ecx, 0x12
        mov    edi, 0x19c5                   ; -> obj1:0x19C5
        rep stosb byte ptr es:[edi], al
        mov    eax, 5
        cmp    dword ptr [0x179e], 0x1388    ; -> obj1:0x179E
        jae    loc_4_03B4
        mov    eax, dword ptr [0x179e]       ; -> obj1:0x179E
        shl    eax, 1
        or     eax, eax
        je     loc_4_03B4
        mov    ebx, 0x3e8
        cdq    
        div    ebx
        sub    eax, 2
        jae    loc_4_03B4
        xor    eax, eax
loc_4_03B4:
        cld    
        xor    ecx, ecx
        mov    edi, 0x1944                   ; -> loc_1_1944
        mov    esi, dword ptr [eax*4]        ; -> obj5:0x0000
        mov    cl, byte ptr [esi]
        mov    byte ptr [0x193b], cl         ; -> loc_1_193B
        inc    esi
        rep movsb byte ptr es:[edi], byte ptr [esi]
        xor    eax, eax
        mov    al, 9
        test   dword ptr [0x1776], 1         ; -> obj1:0x1776
        je     loc_4_0409
        cmp    dword ptr [0x1786], 0x7ff8    ; -> obj1:0x1786
        je     loc_4_0409
        mov    eax, dword ptr [0x1786]       ; -> obj1:0x1786
        je     loc_4_0409
        push   edx
        and    edx, 0
        mov    dx, word ptr [0x1788]         ; -> obj1:0x1788
        mov    bx, 0x37
        div    bx
        pop    edx
        and    eax, 0xffff
loc_4_0409:
        mov    dword ptr [0x1940], eax       ; -> loc_1_1940
        xor    ax, ax
        mov    bl, byte ptr [0x19c5]         ; -> obj1:0x19C5
        mov    al, byte ptr [0x193b]         ; -> loc_1_193B
        div    bl
        cmp    al, 0
        jne    loc_4_0424
        mov    al, 1
loc_4_0424:
        xor    edx, edx
        mov    dl, al
        mov    cl, dl
        xor    ebx, ebx
        mov    bl, byte ptr [0x193b]         ; -> loc_1_193B
        mov    edi, 0x1944                   ; -> loc_1_1944
        add    edi, edx
        sub    bl, cl
        mov    al, 1
loc_4_043D:
        cmp    bl, 0
        je     loc_4_045C
        add    byte ptr [edi], al
        inc    edi
        dec    cl
        dec    bl
        cmp    cl, 0
        jne    loc_4_043D
        mov    cl, dl
        cmp    al, byte ptr [0x19c5]         ; -> obj1:0x19C5
        je     loc_4_043D
        inc    al
        jmp    loc_4_043D
loc_4_045C:
        ret    
        db  000h, 000h, 000h
sub_4_0460:
        cmp    word ptr [0x19fc], 0          ; -> loc_1_19FC
        je     loc_4_049F
        cmp    word ptr [0x19fe], 0          ; -> loc_1_19FE
        jne    loc_4_049F
        xor    ebx, ebx
        mov    ebx, dword ptr [0x17f4]       ; -> obj1:0x17F4
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf000100
        sub    eax, dword ptr [0x1a08]       ; -> loc_1_1A08
        cmp    eax, ebx
        jb     loc_4_049F
        mov    ebx, dword ptr [0x1a10]       ; -> loc_1_1A10
        mov    esi, 0x832                    ; -> loc_4_0832
        xor    edx, edx
        VMMCall  VMM.Schedule_VM_Event           ; cd200f000100
loc_4_049F:
        cmp    byte ptr [0x1a23], 0          ; -> obj1:0x1A23
        je     loc_4_04D3
        xor    ebx, ebx
        mov    ebx, dword ptr [0x17f4]       ; -> obj1:0x17F4
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf000100
        sub    eax, dword ptr [0x1a0c]       ; -> loc_1_1A0C
        cmp    eax, ebx
        jb     loc_4_04D3
        mov    ebx, dword ptr [0x1a10]       ; -> loc_1_1A10
        mov    esi, 0xa1e                    ; -> loc_4_0A1E
        xor    edx, edx
        VMMCall  VMM.Schedule_VM_Event           ; cd200f000100
loc_4_04D3:
        ret    
loc_4_04D4:
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        mov    dword ptr [ebp + 0x1c], 0xf00
        mov    eax, 0x10
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        mov    eax, dword ptr [ebp + 0x1c]
        mov    word ptr [0x1a20], ax         ; -> loc_1_1A20
        mov    ebx, dword ptr [ebp + 0x10]
        mov    byte ptr [0x1a22], bh         ; -> loc_1_1A22
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        mov    ebx, dword ptr [0x19f8]       ; -> loc_1_19F8
        mov    ax, word ptr [0x1a20]         ; -> loc_1_1A20
        cmp    al, 7
        je     loc_4_054B
        cmp    al, 4
        jb     loc_4_054B
        cmp    dword ptr [0x17fc], 3         ; -> obj1:0x17FC
        jne    loc_4_058B
        mov    eax, dword ptr [0x1800]       ; -> obj1:0x1800
        call   sub_4_0AD7
        jmp    loc_4_058B
loc_4_054B:
        cmp    dword ptr [0x17f0], 3         ; -> obj1:0x17F0
        jne    loc_4_0560
        mov    eax, dword ptr [0x17f4]       ; -> obj1:0x17F4
        call   sub_4_0AD7
        jmp    loc_4_058B
loc_4_0560:
        cmp    dword ptr [0x17f0], 1         ; -> obj1:0x17F0
        jne    loc_4_0575
        mov    edx, 0x42656570
        call   sub_4_058C
        jmp    loc_4_058B
loc_4_0575:
        cmp    dword ptr [0x17f0], 2         ; -> obj1:0x17F0
        jne    loc_4_058B
        mov    edx, dword ptr [0x17f4]       ; -> obj1:0x17F4
        call   sub_4_0A69
        jmp    loc_4_058B
loc_4_058B:
        ret    
sub_4_058C:
        cmp    word ptr [0x19fe], 0          ; -> loc_1_19FE
        jne    loc_4_0830
        mov    word ptr [0x19fc], 0          ; -> loc_1_19FC
        cmp    edx, 0
        je     loc_4_0830
        push   eax
        push   ecx
        mov    dword ptr [0x1a04], edx       ; -> loc_1_1A04
        mov    ecx, edx
        mov    dword ptr [0x1a10], ebx       ; -> loc_1_1A10
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        mov    dword ptr [ebp + 0x1c], 0x300
        xor    ebx, ebx
        mov    bh, byte ptr [0x1a22]         ; -> loc_1_1A22
        mov    dword ptr [ebp + 0x10], ebx
        mov    eax, 0x10
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        push   dword ptr [ebp + 0x14]
        cmp    cl, 0
        je     loc_4_066E
        mov    dword ptr [ebp + 0x1c], 0x200
        xor    ebx, ebx
        mov    bh, byte ptr [0x1a22]         ; -> loc_1_1A22
        mov    dword ptr [ebp + 0x10], ebx
        mov    dword ptr [ebp + 0x14], 0x4f
        mov    eax, 0x10
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        mov    dword ptr [ebp + 0x1c], 0x800
        xor    ebx, ebx
        mov    bh, byte ptr [0x1a22]         ; -> loc_1_1A22
        mov    dword ptr [ebp + 0x10], ebx
        mov    eax, 0x10
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        mov    eax, dword ptr [ebp + 0x1c]
        mov    byte ptr [0x1a03], al         ; -> obj1:0x1A03
        mov    eax, 0xa00
        mov    al, cl
        mov    dword ptr [ebp + 0x1c], eax
        xor    ebx, ebx
        mov    bh, byte ptr [0x1a22]         ; -> loc_1_1A22
        mov    dword ptr [ebp + 0x10], ebx
        mov    dword ptr [ebp + 0x18], 1
        mov    eax, 0x10
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        or     word ptr [0x19fc], 1          ; -> loc_1_19FC
loc_4_066E:
        shr    ecx, 8
        cmp    cl, 0
        je     loc_4_06EE
        mov    dword ptr [ebp + 0x1c], 0x200
        xor    ebx, ebx
        mov    bh, byte ptr [0x1a22]         ; -> loc_1_1A22
        mov    dword ptr [ebp + 0x10], ebx
        mov    dword ptr [ebp + 0x14], 0x4e
        mov    eax, 0x10
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        mov    dword ptr [ebp + 0x1c], 0x800
        xor    ebx, ebx
        mov    bh, byte ptr [0x1a22]         ; -> loc_1_1A22
        mov    dword ptr [ebp + 0x10], ebx
        mov    eax, 0x10
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        mov    eax, dword ptr [ebp + 0x1c]
        mov    byte ptr [0x1a02], al         ; -> loc_1_1A02
        mov    eax, 0xa00
        mov    al, cl
        mov    dword ptr [ebp + 0x1c], eax
        xor    ebx, ebx
        mov    bh, byte ptr [0x1a22]         ; -> loc_1_1A22
        mov    dword ptr [ebp + 0x10], ebx
        mov    dword ptr [ebp + 0x18], 1
        mov    eax, 0x10
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        or     word ptr [0x19fc], 2          ; -> loc_1_19FC
loc_4_06EE:
        shr    ecx, 8
        cmp    cl, 0
        je     loc_4_076E
        mov    dword ptr [ebp + 0x1c], 0x200
        xor    ebx, ebx
        mov    bh, byte ptr [0x1a22]         ; -> loc_1_1A22
        mov    dword ptr [ebp + 0x10], ebx
        mov    dword ptr [ebp + 0x14], 0x4d
        mov    eax, 0x10
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        mov    dword ptr [ebp + 0x1c], 0x800
        xor    ebx, ebx
        mov    bh, byte ptr [0x1a22]         ; -> loc_1_1A22
        mov    dword ptr [ebp + 0x10], ebx
        mov    eax, 0x10
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        mov    eax, dword ptr [ebp + 0x1c]
        mov    byte ptr [0x1a01], al         ; -> obj1:0x1A01
        mov    eax, 0xa00
        mov    al, cl
        mov    dword ptr [ebp + 0x1c], eax
        xor    ebx, ebx
        mov    bh, byte ptr [0x1a22]         ; -> loc_1_1A22
        mov    dword ptr [ebp + 0x10], ebx
        mov    dword ptr [ebp + 0x18], 1
        mov    eax, 0x10
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        or     word ptr [0x19fc], 4          ; -> loc_1_19FC
loc_4_076E:
        shr    ecx, 8
        cmp    cl, 0
        je     loc_4_07EE
        mov    dword ptr [ebp + 0x1c], 0x200
        xor    ebx, ebx
        mov    bh, byte ptr [0x1a22]         ; -> loc_1_1A22
        mov    dword ptr [ebp + 0x10], ebx
        mov    dword ptr [ebp + 0x14], 0x4c
        mov    eax, 0x10
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        mov    dword ptr [ebp + 0x1c], 0x800
        xor    ebx, ebx
        mov    bh, byte ptr [0x1a22]         ; -> loc_1_1A22
        mov    dword ptr [ebp + 0x10], ebx
        mov    eax, 0x10
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        mov    eax, dword ptr [ebp + 0x1c]
        mov    byte ptr [0x1a00], al         ; -> loc_1_1A00
        mov    eax, 0xa00
        mov    al, cl
        mov    dword ptr [ebp + 0x1c], eax
        xor    ebx, ebx
        mov    bh, byte ptr [0x1a22]         ; -> loc_1_1A22
        mov    dword ptr [ebp + 0x10], ebx
        mov    dword ptr [ebp + 0x18], 1
        mov    eax, 0x10
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        or     word ptr [0x19fc], 8          ; -> loc_1_19FC
loc_4_07EE:
        mov    dword ptr [ebp + 0x1c], 0x200
        xor    ebx, ebx
        mov    bh, byte ptr [0x1a22]         ; -> loc_1_1A22
        mov    dword ptr [ebp + 0x10], ebx
        pop    dword ptr [ebp + 0x14]
        mov    eax, 0x10
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf000100
        mov    dword ptr [0x1a08], eax       ; -> loc_1_1A08
        pop    ecx
        pop    eax
loc_4_0830:
        clc    
        ret    
loc_4_0832:
        mov    word ptr [0x19fe], 1          ; -> loc_1_19FE
        pushfd 
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        mov    dword ptr [ebp + 0x1c], 0x300
        xor    ebx, ebx
        mov    bh, byte ptr [0x1a22]         ; -> loc_1_1A22
        mov    dword ptr [ebp + 0x10], ebx
        mov    eax, 0x10
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        push   dword ptr [ebp + 0x14]
        test   word ptr [0x19fc], 1          ; -> loc_1_19FC
        je     loc_4_08CA
        mov    dword ptr [ebp + 0x1c], 0x200
        xor    ebx, ebx
        mov    bh, byte ptr [0x1a22]         ; -> loc_1_1A22
        mov    dword ptr [ebp + 0x10], ebx
        mov    dword ptr [ebp + 0x14], 0x4f
        mov    eax, 0x10
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        mov    eax, 0xa00
        mov    al, byte ptr [0x1a03]         ; -> obj1:0x1A03
        mov    dword ptr [ebp + 0x1c], eax
        xor    ebx, ebx
        mov    bh, byte ptr [0x1a22]         ; -> loc_1_1A22
        mov    dword ptr [ebp + 0x10], ebx
        mov    dword ptr [ebp + 0x18], 1
        mov    eax, 0x10
        VMMCall  VMM.Exec_Int                    ; cd2084000100
loc_4_08CA:
        test   word ptr [0x19fc], 2          ; -> loc_1_19FC
        je     loc_4_0923
        mov    dword ptr [ebp + 0x1c], 0x200
        xor    ebx, ebx
        mov    bh, byte ptr [0x1a22]         ; -> loc_1_1A22
        mov    dword ptr [ebp + 0x10], ebx
        mov    dword ptr [ebp + 0x14], 0x4e
        mov    eax, 0x10
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        mov    eax, 0xa00
        mov    al, byte ptr [0x1a02]         ; -> loc_1_1A02
        mov    dword ptr [ebp + 0x1c], eax
        xor    ebx, ebx
        mov    bh, byte ptr [0x1a22]         ; -> loc_1_1A22
        mov    dword ptr [ebp + 0x10], ebx
        mov    dword ptr [ebp + 0x18], 1
        mov    eax, 0x10
        VMMCall  VMM.Exec_Int                    ; cd2084000100
loc_4_0923:
        test   word ptr [0x19fc], 4          ; -> loc_1_19FC
        je     loc_4_097C
        mov    dword ptr [ebp + 0x1c], 0x200
        xor    ebx, ebx
        mov    bh, byte ptr [0x1a22]         ; -> loc_1_1A22
        mov    dword ptr [ebp + 0x10], ebx
        mov    dword ptr [ebp + 0x14], 0x4d
        mov    eax, 0x10
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        mov    eax, 0xa00
        mov    al, byte ptr [0x1a01]         ; -> obj1:0x1A01
        mov    dword ptr [ebp + 0x1c], eax
        xor    ebx, ebx
        mov    bh, byte ptr [0x1a22]         ; -> loc_1_1A22
        mov    dword ptr [ebp + 0x10], ebx
        mov    dword ptr [ebp + 0x18], 1
        mov    eax, 0x10
        VMMCall  VMM.Exec_Int                    ; cd2084000100
loc_4_097C:
        test   word ptr [0x19fc], 8          ; -> loc_1_19FC
        je     loc_4_09D5
        mov    dword ptr [ebp + 0x1c], 0x200
        xor    ebx, ebx
        mov    bh, byte ptr [0x1a22]         ; -> loc_1_1A22
        mov    dword ptr [ebp + 0x10], ebx
        mov    dword ptr [ebp + 0x14], 0x4c
        mov    eax, 0x10
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        mov    eax, 0xa00
        mov    al, byte ptr [0x1a00]         ; -> loc_1_1A00
        mov    dword ptr [ebp + 0x1c], eax
        xor    ebx, ebx
        mov    bh, byte ptr [0x1a22]         ; -> loc_1_1A22
        mov    dword ptr [ebp + 0x10], ebx
        mov    dword ptr [ebp + 0x18], 1
        mov    eax, 0x10
        VMMCall  VMM.Exec_Int                    ; cd2084000100
loc_4_09D5:
        mov    dword ptr [ebp + 0x1c], 0x200
        xor    ebx, ebx
        mov    bh, byte ptr [0x1a22]         ; -> loc_1_1A22
        mov    dword ptr [ebp + 0x10], ebx
        pop    dword ptr [ebp + 0x14]
        mov    eax, 0x10
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        popfd  
        mov    word ptr [0x19fc], 0          ; -> loc_1_19FC
        mov    word ptr [0x19fe], 0          ; -> loc_1_19FE
        ret    
loc_4_0A1E:
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        mov    dword ptr [ebp + 0x1c], 0xb00
        mov    dword ptr [ebp + 0x10], 0
        mov    eax, 0x10
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        mov    byte ptr [0x1a23], 0          ; -> obj1:0x1A23
        ret    
sub_4_0A69:
        mov    dword ptr [0x1a10], ebx       ; -> loc_1_1A10
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        mov    dword ptr [ebp + 0x1c], 0xb00
        mov    dword ptr [ebp + 0x10], 0x1f
        mov    eax, 0x10
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf000100
        mov    dword ptr [0x1a0c], eax       ; -> loc_1_1A0C
        mov    byte ptr [0x1a23], 1          ; -> obj1:0x1A23
        ret    
loc_4_0AC5:
        mov    eax, 0x11
        mov    ebx, dword ptr [0x19f8]       ; -> loc_1_19F8
        VMMCall  VMM.System_Control              ; cd2093000100
        ret    
sub_4_0AD7:
        push   eax
        mov    eax, 0x10
        mov    ebx, dword ptr [0x19f8]       ; -> loc_1_19F8
        VMMCall  VMM.System_Control              ; cd2093000100
        mov    eax, 0
        mov    ebx, dword ptr [0x19f8]       ; -> loc_1_19F8
        VxDCall  VDD.VDD_Msg_ClrScrn             ; cd2007000a00
        pop    eax
        xor    edx, edx
        mov    esi, 0xac5                    ; -> loc_4_0AC5
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        ret    
        db  000h, 000h, 000h
loc_4_0B0C:
        push   eax
        test   dword ptr [0x17c0], 0x10      ; -> obj1:0x17C0
        je     loc_4_0B29
        mov    eax, 2
        mov    ecx, 1
        VxDCall  ENABLE.VMINI_Siren              ; cd2005003700
loc_4_0B29:
        mov    ebx, 0x8001
        mov    edi, 0x17bc                   ; -> obj1:0x17BC
        VxDCall  ENABLE.VMINI_GetData            ; cd2007003700
        pop    eax
        ret    
loc_4_0B3B:
        pushal 
        mov    esi, edx
        xor    ebx, ebx
        mov    ebp, dword ptr [0x170c]       ; -> obj1:0x170C
        cmp    byte ptr [ebp], bl
        je     loc_4_0B95
        mov    ebp, dword ptr [0x1714]       ; -> obj1:0x1714
        cmp    esi, dword ptr [ebp]
        je     loc_4_0B95
        mov    ebp, dword ptr [0x1714]       ; -> obj1:0x1714
        mov    dword ptr [ebp], esi
loc_4_0B5F:
        mov    ebp, dword ptr [0x1710]       ; -> obj1:0x1710
        cmp    dword ptr [ebp + ebx*8], 0xffff
        je     loc_4_0B8F
        mov    eax, dword ptr [ebp + ebx*8]
        mov    ecx, dword ptr [ebp + ebx*8 + 4]
        push   ebx
        push   edx
        push   esi
        push   0
        push   0
        push   esi
        push   1
        push   ecx
        push   eax
        VxDCall  SHELL._SHELL_PostMessage        ; cd2006001700
        add    esp, 0x18
        pop    esi
        pop    edx
        pop    ebx
loc_4_0B8F:
        inc    ebx
        cmp    ebx, 8
        jl     loc_4_0B5F
loc_4_0B95:
        popal  
        ret    
        db  000h
loc_4_0B98:
        pushal 
        mov    dword ptr [0x1a40], 0         ; -> obj1:0x1A40
        call   sub_4_00B0
        call   sub_4_0188
        call   sub_4_0460
        cmp    dword ptr [0x1a40], -1        ; -> obj1:0x1A40
        je     loc_4_0BD3
        mov    eax, 0x37
        xor    edx, edx
        mov    esi, 0xb98                    ; -> loc_4_0B98
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    dword ptr [0x1a40], esi       ; -> obj1:0x1A40
loc_4_0BD3:
        popal  
        ret    
        db  000h, 000h, 000h
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 5  vsize=0x21c  RW-
        dd  obj5:0x0024              ; fixup
        dd  obj5:0x0039              ; fixup
        dd  obj5:0x0056              ; fixup
        dd  obj5:0x007C              ; fixup
        dd  obj5:0x00AB              ; fixup
        dd  obj5:0x00E3              ; fixup
        dd  obj5:0x0124              ; fixup
        dd  obj5:0x016D              ; fixup
        dd  obj5:0x01C0              ; fixup
        db  012h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 001h, 000h, 001h, 000h, 001h, 000h, 001h
        db  000h, 001h, 000h, 001h, 001h, 01Bh, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 001h, 000h
        db  000h, 000h, 001h, 000h, 000h, 001h, 001h, 000h, 000h, 001h, 000h, 001h, 000h, 001h, 001h, 000h
        db  001h, 001h, 024h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        db  001h, 000h, 000h, 000h, 001h, 000h, 000h, 001h, 000h, 000h, 001h, 000h, 000h, 001h, 000h, 001h
        db  000h, 001h, 000h, 001h, 000h, 001h, 000h, 001h, 02Dh, 000h, 000h, 000h, 000h, 001h, 000h, 000h
        db  000h, 000h, 001h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 001h
        db  000h, 000h, 000h, 001h, 000h, 000h, 001h, 000h, 000h, 001h, 000h, 000h, 001h, 000h, 000h, 001h
        db  000h, 001h, 000h, 001h, 000h, 001h, 001h, 036h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        db  000h, 001h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 001h, 000h
        db  000h, 000h, 001h, 000h, 000h, 000h, 001h, 000h, 000h, 001h, 000h, 000h, 001h, 000h, 000h, 001h
        db  001h, 000h, 001h, 000h, 001h, 000h, 001h, 000h, 001h, 000h, 001h, 000h, 001h, 001h, 001h, 03Fh
        db  000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 001h, 000h, 000h
        db  000h, 001h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 001h, 000h, 000h, 001h, 000h, 000h, 001h
        db  000h, 000h, 001h, 000h, 000h, 001h, 000h, 000h, 001h, 000h, 000h, 001h, 000h, 000h, 001h, 000h
        db  001h, 000h, 001h, 000h, 001h, 000h, 001h, 000h, 001h, 000h, 001h, 000h, 001h, 000h, 001h, 001h
        db  048h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 000h, 001h
        db  000h, 000h, 000h, 001h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 001h
        db  000h, 000h, 001h, 000h, 000h, 001h, 000h, 000h, 001h, 000h, 000h, 001h, 000h, 000h, 001h, 000h
        db  000h, 001h, 000h, 001h, 000h, 001h, 000h, 001h, 000h, 000h, 001h, 000h, 001h, 000h, 001h, 000h
        db  001h, 000h, 001h, 000h, 001h, 000h, 001h, 000h, 001h, 051h, 000h, 000h, 000h, 000h, 001h, 000h
        db  000h, 000h, 001h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 001h
        db  000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 001h, 000h, 000h, 001h
        db  000h, 000h, 000h, 001h, 000h, 000h, 000h, 001h, 000h, 000h, 001h, 000h, 000h, 001h, 000h, 000h
        db  001h, 000h, 000h, 001h, 000h, 001h, 000h, 001h, 000h, 001h, 001h, 000h, 001h, 000h, 001h, 000h
        db  001h, 000h, 001h, 000h, 001h, 000h, 001h, 000h, 001h, 000h, 001h, 001h, 05Ah, 000h, 000h, 000h
        db  000h, 001h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 000h, 001h
        db  000h, 000h, 000h, 001h, 000h, 000h, 000h, 001h, 000h, 000h, 001h, 000h, 000h, 000h, 001h, 000h
        db  000h, 000h, 001h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 001h, 000h
        db  000h, 001h, 000h, 000h, 001h, 000h, 000h, 001h, 000h, 000h, 001h, 000h, 000h, 001h, 000h, 000h
        db  001h, 000h, 000h, 001h, 000h, 001h, 000h, 001h, 000h, 001h, 000h, 001h, 000h, 001h, 000h, 001h
        db  000h, 001h, 000h, 001h, 000h, 001h, 000h, 001h
VxD__LDATA ENDS

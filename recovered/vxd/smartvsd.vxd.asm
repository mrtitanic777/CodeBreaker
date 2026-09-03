; SMARTVSD -- reconstructed 32-bit disassembly (VxD / LE format)
; 5 objects, 1676 instructions recovered by recursive descent

Declare_Virtual_Device SMARTVSD, 4, 0, SMARTVSD_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 080000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x1310  R-X
SMARTVSD_Control:
        cmp    eax, 0x1b
        je     loc_2_0000
        cmp    eax, 0x1c
        jne    loc_1_0017
        call   sub_1_0851
        cmp    eax, 1
        ret    
loc_1_0017:
        cmp    eax, 0x23
        jne    loc_1_0029
        push   esi
        push   edx
        push   ebx
        push   ecx
        call   sub_1_0150
        cmp    eax, 1
        ret    
loc_1_0029:
        clc    
        ret    
        db  000h
loc_1_002C:
        mov    ebx, dword ptr [esp + 4]
        mov    word ptr [ebx + 2], 0
        movzx  esi, word ptr [ebx]
        cmp    esi, 3
        jne    loc_1_0049
        push   ebx
        call   sub_3_0030
        add    esp, 4
        jmp    loc_1_005B
loc_1_0049:
        cmp    esi, 2
        jne    loc_1_0055
        call   sub_3_0000
        jmp    loc_1_005B
loc_1_0055:
        mov    word ptr [ebx + 2], 0xffff
loc_1_005B:
        ret    
sub_1_005C:
        VMMCall  VMM._PageCheckLinRange          ; cd2067800100
        add    byte ptr [eax], al
sub_1_0064:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   edx
        push   ebx
        push   ecx
        sub    esp, 0x10
        mov    esi, esp
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [edi + 4]
        add    ecx, 0x64
        mov    dword ptr [esi + 6], ecx
        add    ecx, 0x68
        mov    word ptr [esi + 4], cx
        mov    byte ptr [esi + 0xe], 2
        mov    word ptr [esi], 2
        push   esi
        call   sub_1_1178
        mov    esi, dword ptr [esi + 0xa]
        add    esp, 0x14
        or     esi, esi
        je     loc_1_011C
        add    esi, dword ptr [edi + 4]
        movzx  eax, word ptr [ebp + 0x14]
        mov    dword ptr [esi + 0x7c], eax
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [esi + 0x80], eax
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [esi + 0x90], eax
        mov    ax, 0x2b
        mov    word ptr [esi + 0x68], ax
        mov    eax, dword ptr [ebp + 0x18]
        mov    dword ptr [esi + 0x6c], eax
        or     dword ptr [esi + 0x6c], 0x501
        xor    eax, eax
        mov    dword ptr [esi + 0x64], eax
        mov    dword ptr [esi + 0x78], eax
        mov    dword ptr [esi + 0xa8], eax
        mov    al, byte ptr [edi + 0x15]
        mov    byte ptr [esi + 0xb1], al
        lea    ebx, [edi + 0xc3]
        push   esi
        lea    esi, [esi + 0x64]
        VxDCall  BLOCKDEV.BlockDev_Send_Command  ; cd2004001000
        pop    esi
        movzx  eax, word ptr [esi + 0x6a]
        sub    esi, dword ptr [edi + 4]
        sub    esp, 8
        mov    edi, esp
        mov    dword ptr [edi + 4], esi
        mov    word ptr [edi], 4
        push   edi
        call   sub_1_1178
        add    esp, 0xc
loc_1_0115:
        pop    ecx
        pop    ebx
        pop    edx
        pop    edi
        pop    esi
        leave  
        ret    
loc_1_011C:
        mov    ax, 0x1d
        jmp    loc_1_0115
sub_1_0122:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ecx
        cld    
        movzx  ecx, word ptr [ebp + 0x10]
        mov    esi, dword ptr [ebp + 0xc]
        mov    edi, dword ptr [ebp + 8]
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        movzx  ecx, byte ptr [ebp + 0x10]
        and    cx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    eax, dword ptr [ebp + 8]
        pop    ecx
        pop    edi
        pop    esi
        leave  
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h
sub_1_0150:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], eax
        jmp    loc_1_01E0
loc_1_016B:
        mov    dword ptr [ebp - 4], 0
        jmp    loc_1_0220
loc_1_0177:
        call   sub_1_0866
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_0220
loc_1_0184:
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_022F
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_0220
loc_1_019D:
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_02BA
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_0220
loc_1_01B6:
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_04F9
        mov    dword ptr [ebp - 4], eax
        jmp    loc_1_0220
loc_1_01CF:
        mov    dword ptr [ebp - 4], 1
        jmp    loc_1_0220
        db  0E9h, 040h, 000h, 000h, 000h
loc_1_01E0:
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_016B
        cmp    dword ptr [ebp - 8], 0x74080
        je     loc_1_0184
        cmp    dword ptr [ebp - 8], 0x7c084
        je     loc_1_019D
        cmp    dword ptr [ebp - 8], 0x7c088
        je     loc_1_01B6
        cmp    dword ptr [ebp - 8], -1
        je     loc_1_0177
        jmp    loc_1_01CF
loc_1_0220:
        mov    eax, dword ptr [ebp - 4]
        jmp    loc_1_0228
loc_1_0228:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0x10
sub_1_022F:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        call   sub_1_08C0
        and    eax, 0xff
        or     eax, eax
        je     loc_1_0258
        mov    eax, 1
        jmp    loc_1_02B3
loc_1_0258:
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax + 0x10]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 0x10]
        cmp    dword ptr [eax + 0x1c], 0x18
        jne    loc_1_0277
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax + 0x18]
        mov    dword ptr [ebp - 4], eax
loc_1_0277:
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax], 1
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 1], 2
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 4], 7
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [eax + 2], 0
        mov    al, byte ptr [0x11ac]         ; -> loc_1_11AC
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 3], al
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax + 0x20]
        mov    dword ptr [eax], 0x18
        sub    eax, eax
        jmp    loc_1_02B3
loc_1_02B3:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0xc
sub_1_02BA:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x2c
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 0x14], 0
        mov    dword ptr [ebp - 0x2c], 0
        mov    dword ptr [ebp - 0x10], 0
        mov    dword ptr [ebp - 8], 0
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        call   sub_1_0AC1
        and    eax, 0xff
        or     eax, eax
        je     loc_1_0305
        mov    eax, 1
        jmp    loc_1_04F2
loc_1_0305:
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax + 0x10]
        mov    dword ptr [ebp - 0x24], eax
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax + 0x18]
        add    eax, 4
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    byte ptr [eax], 0
        mov    eax, dword ptr [ebp - 0xc]
        mov    byte ptr [eax + 1], 0
        mov    eax, dword ptr [ebp - 0xc]
        mov    byte ptr [eax + 3], 0
        mov    eax, dword ptr [ebp - 0xc]
        mov    al, byte ptr [eax + 3]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    byte ptr [ecx + 2], al
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [eax + 8], 0
        mov    eax, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [eax + 8]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [ebp - 0x24]
        mov    al, byte ptr [eax + 0xc]
        push   eax
        call   sub_1_087B
        add    esp, 4
        mov    dword ptr [ebp - 0x28], eax
        cmp    dword ptr [ebp - 0x28], 0
        jne    loc_1_0383
        mov    eax, dword ptr [ebp - 0xc]
        mov    byte ptr [eax], 5
        mov    eax, dword ptr [ebp - 0xc]
        mov    byte ptr [eax + 1], 0
        mov    eax, 1
        jmp    loc_1_04F2
loc_1_0383:
        lea    eax, [ebp - 0x18]
        push   eax
        mov    eax, dword ptr [ebp - 0x24]
        mov    eax, dword ptr [eax]
        add    eax, 0x20
        push   eax
        mov    eax, dword ptr [ebp - 0x24]
        push   eax
        call   sub_1_06FA
        mov    word ptr [ebp - 0x20], ax
        mov    eax, dword ptr [ebp - 0x20]
        and    eax, 0xffff
        or     eax, eax
        je     loc_1_03C6
        mov    al, byte ptr [ebp - 0x20]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp - 0xc]
        mov    byte ptr [eax + 1], 0
        mov    eax, 1
        jmp    loc_1_04F2
loc_1_03C6:
        mov    eax, dword ptr [ebp - 0x24]
        cmp    dword ptr [eax], 0
        je     loc_1_03DF
        mov    dword ptr [ebp - 8], 0x400000
        mov    word ptr [ebp - 0x14], 1
loc_1_03DF:
        mov    eax, dword ptr [ebp + 0x10]
        cmp    dword ptr [eax + 0x1c], 0x18
        jne    loc_1_0409
        mov    eax, dword ptr [ebp - 0x24]
        mov    al, byte ptr [eax + 4]
        and    eax, 0xff
        cmp    eax, 0xda
        jne    loc_1_0409
        mov    dword ptr [ebp - 4], 0xffffffff
loc_1_0409:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    ax, word ptr [ebp - 0x14]
        push   eax
        mov    eax, dword ptr [ebp - 0x18]
        add    eax, 0x20
        push   eax
        mov    eax, dword ptr [ebp - 0x18]
        add    eax, 4
        push   eax
        mov    eax, dword ptr [ebp - 0x28]
        push   eax
        call   sub_1_0064
        add    esp, 0x14
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_0457
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax + 0x18]
        mov    dword ptr [ebp - 0x1c], eax
        push   8
        mov    eax, dword ptr [ebp - 0x24]
        add    eax, 4
        push   eax
        mov    eax, dword ptr [ebp - 0x1c]
        add    eax, 0x10
        push   eax
        call   sub_1_0122
        add    esp, 0xc
loc_1_0457:
        mov    eax, dword ptr [ebp - 0x14]
        and    eax, 0xffff
        or     eax, eax
        je     loc_1_0487
        mov    eax, dword ptr [ebp - 0x24]
        mov    al, byte ptr [eax + 0xa]
        and    eax, 1
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 0x200
        mov    ecx, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [ecx + 0x20]
        mov    dword ptr [ecx], eax
        jmp    loc_1_0493
loc_1_0487:
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax + 0x20]
        mov    dword ptr [eax], 0xc
loc_1_0493:
        mov    eax, dword ptr [ebp - 0x24]
        mov    al, byte ptr [eax + 0xa]
        and    eax, 0xff
        test   al, 1
        je     loc_1_04B8
        mov    eax, dword ptr [ebp - 0xc]
        mov    byte ptr [eax], 1
        mov    eax, dword ptr [ebp - 0x24]
        mov    al, byte ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 0xc]
        mov    byte ptr [ecx + 1], al
loc_1_04B8:
        mov    eax, dword ptr [ebp - 0x24]
        mov    eax, dword ptr [eax]
        add    eax, 0x20
        push   eax
        mov    eax, dword ptr [ebp - 0x18]
        push   eax
        call   sub_1_07C0
        mov    eax, dword ptr [ebp - 0xc]
        mov    al, byte ptr [eax]
        and    eax, 0xff
        or     eax, eax
        je     loc_1_04EB
        mov    eax, 1
        jmp    loc_1_04F2
        db  0E9h, 007h, 000h, 000h, 000h
loc_1_04EB:
        sub    eax, eax
        jmp    loc_1_04F2
loc_1_04F2:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0xc
sub_1_04F9:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x2c
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 0x10], 0
        mov    dword ptr [ebp - 0x2c], 0
        mov    dword ptr [ebp - 0xc], 0
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        call   sub_1_0CD7
        and    eax, 0xff
        or     eax, eax
        je     loc_1_053D
        mov    eax, 1
        jmp    loc_1_06F3
loc_1_053D:
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax + 0x10]
        mov    dword ptr [ebp - 0x1c], eax
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax + 0x18]
        mov    dword ptr [ebp - 0x28], eax
        mov    eax, dword ptr [ebp - 0x28]
        add    eax, 4
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax], 0
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 1], 0
        mov    eax, dword ptr [ebp - 0x1c]
        mov    al, byte ptr [eax + 0xc]
        push   eax
        call   sub_1_087B
        add    esp, 4
        mov    dword ptr [ebp - 0x20], eax
        cmp    dword ptr [ebp - 0x20], 0
        jne    loc_1_0598
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax], 5
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 1], 0
        mov    eax, 1
        jmp    loc_1_06F3
loc_1_0598:
        lea    eax, [ebp - 0x14]
        push   eax
        push   0x20
        mov    eax, dword ptr [ebp - 0x1c]
        push   eax
        call   sub_1_06FA
        mov    word ptr [ebp - 0x18], ax
        mov    eax, dword ptr [ebp - 0x18]
        and    eax, 0xffff
        or     eax, eax
        je     loc_1_05D4
        mov    al, byte ptr [ebp - 0x18]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 1], 0
        mov    eax, 1
        jmp    loc_1_06F3
loc_1_05D4:
        mov    eax, dword ptr [ebp - 0x1c]
        cmp    dword ptr [eax], 0
        je     loc_1_0630
        lea    eax, [ebp - 0x24]
        push   eax
        mov    eax, dword ptr [ebp - 0x1c]
        mov    eax, dword ptr [eax]
        push   eax
        mov    eax, dword ptr [ebp - 0x28]
        add    eax, 0x10
        push   eax
        call   sub_1_06FA
        mov    word ptr [ebp - 0x18], ax
        mov    eax, dword ptr [ebp - 0x18]
        and    eax, 0xffff
        or     eax, eax
        je     loc_1_0623
        mov    al, byte ptr [ebp - 0x18]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx], al
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 1], 0
        mov    eax, 1
        jmp    loc_1_06F3
loc_1_0623:
        mov    dword ptr [ebp - 4], 0x200000
        mov    word ptr [ebp - 0x10], 1
loc_1_0630:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    ax, word ptr [ebp - 0x10]
        push   eax
        mov    eax, dword ptr [ebp - 0x24]
        push   eax
        mov    eax, dword ptr [ebp - 0x14]
        add    eax, 4
        push   eax
        mov    eax, dword ptr [ebp - 0x20]
        push   eax
        call   sub_1_0064
        add    esp, 0x14
        mov    eax, dword ptr [ebp - 0x10]
        and    eax, 0xffff
        or     eax, eax
        je     loc_1_068F
        mov    eax, dword ptr [ebp - 0x1c]
        mov    eax, dword ptr [eax]
        push   eax
        mov    eax, dword ptr [ebp - 0x24]
        push   eax
        call   sub_1_07C0
        mov    eax, dword ptr [ebp - 0x1c]
        mov    al, byte ptr [eax + 0xa]
        and    eax, 1
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 0x200
        mov    ecx, dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [ecx + 0x20]
        mov    dword ptr [ecx], eax
        jmp    loc_1_069B
loc_1_068F:
        mov    eax, dword ptr [ebp + 0x10]
        mov    eax, dword ptr [eax + 0x20]
        mov    dword ptr [eax], 0xc
loc_1_069B:
        mov    eax, dword ptr [ebp - 0x1c]
        mov    al, byte ptr [eax + 0xa]
        and    eax, 0xff
        test   al, 1
        je     loc_1_06C0
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax], 1
        mov    eax, dword ptr [ebp - 0x1c]
        mov    al, byte ptr [eax + 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx + 1], al
loc_1_06C0:
        push   0x20
        mov    eax, dword ptr [ebp - 0x14]
        push   eax
        call   sub_1_07C0
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax]
        and    eax, 0xff
        or     eax, eax
        je     loc_1_06EC
        mov    eax, 1
        jmp    loc_1_06F3
        db  0E9h, 007h, 000h, 000h, 000h
loc_1_06EC:
        sub    eax, eax
        jmp    loc_1_06F3
loc_1_06F3:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0xc
sub_1_06FA:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 8], 0
        jne    loc_1_0723
        push   0x1250                        ; -> loc_1_1250
        call   sub_1_1230
        add    esp, 4
        mov    ax, 4
        jmp    loc_1_07B9
loc_1_0723:
        mov    eax, dword ptr [ebp + 8]
        shr    eax, 0xc
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp + 8]
        and    eax, 0xfff
        add    eax, dword ptr [ebp + 0xc]
        dec    eax
        shr    eax, 0xc
        inc    eax
        mov    dword ptr [ebp - 8], eax
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_005C
        add    esp, 0xc
        cmp    eax, dword ptr [ebp - 8]
        je     loc_1_0763
        mov    ax, 4
        jmp    loc_1_07B9
loc_1_0763:
        push   0xc0000000
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0xc]
        push   eax
        call   sub_1_1168
        add    esp, 0xc
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_07A3
        mov    ecx, dword ptr [ebp + 8]
        and    ecx, 0xfff
        add    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp + 0x10]
        mov    dword ptr [edx], ecx
        sub    ax, ax
        jmp    loc_1_07B9
        db  0E9h, 016h, 000h, 000h, 000h
loc_1_07A3:
        push   0x127c                        ; -> loc_1_127C
        call   sub_1_1230
        add    esp, 4
        mov    ax, 7
        jmp    loc_1_07B9
loc_1_07B9:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    0xc
sub_1_07C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        cmp    dword ptr [ebp + 8], 0
        jne    loc_1_07E9
        push   0x12a4                        ; -> loc_1_12A4
        call   sub_1_1230
        add    esp, 4
        mov    ax, 4
        jmp    loc_1_084A
loc_1_07E9:
        mov    eax, dword ptr [ebp + 8]
        shr    eax, 0xc
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 8]
        and    eax, 0xfff
        add    eax, dword ptr [ebp + 0xc]
        dec    eax
        shr    eax, 0xc
        inc    eax
        mov    dword ptr [ebp - 4], eax
        push   0x40000000
        mov    eax, dword ptr [ebp - 4]
        push   eax
        mov    eax, dword ptr [ebp - 8]
        push   eax
        call   sub_1_1170
        add    esp, 0xc
        mov    dword ptr [ebp - 0xc], eax
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_1_0834
        sub    ax, ax
        jmp    loc_1_084A
        db  0E9h, 016h, 000h, 000h, 000h
loc_1_0834:
        push   0x12d0                        ; -> obj1:0x12D0
        call   sub_1_1230
        add    esp, 4
        mov    ax, 0x57
        jmp    loc_1_084A
loc_1_084A:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
sub_1_0851:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, 1
        jmp    loc_1_0861
loc_1_0861:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_0866:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, 1
        jmp    loc_1_0876
loc_1_0876:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_087B:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        xor    eax, eax
        mov    al, byte ptr [ebp + 8]
        cdq    
        xor    eax, edx
        sub    eax, edx
        and    eax, 1
        xor    eax, edx
        sub    eax, edx
        xor    ecx, ecx
        mov    cl, byte ptr [ebp + 8]
        sar    ecx, 1
        lea    ecx, [ecx + ecx*8]
        mov    eax, dword ptr [ecx + eax*4 + 0x1201] ; -> obj1:0x1201
        jmp    loc_1_08B2
loc_1_08B2:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_08C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x18]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        add    eax, 4
        mov    dword ptr [ebp - 0x10], eax
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_08F3
        mov    al, 4
        jmp    loc_1_0ABA
loc_1_08F3:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x1c], 0x18
        jne    loc_1_0907
        mov    dword ptr [ebp - 4], 0xffffffff
loc_1_0907:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x1c], 0x10
        je     loc_1_0925
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_0925
        mov    al, 4
        jmp    loc_1_0ABA
loc_1_0925:
        mov    eax, dword ptr [ebp - 0x10]
        mov    byte ptr [eax + 1], 0
        mov    word ptr [ebp - 0xc], 0
        jmp    loc_1_093B
loc_1_0937:
        inc    word ptr [ebp - 0xc]
loc_1_093B:
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, 0xffff
        cmp    eax, 2
        jae    loc_1_0977
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 0x10]
        cmp    dword ptr [ecx + eax*4 + 4], 0
        je     loc_1_0972
        mov    eax, dword ptr [ebp - 0x10]
        mov    byte ptr [eax], 9
        mov    eax, dword ptr [ebp - 0x10]
        mov    al, byte ptr [eax]
        jmp    loc_1_0ABA
loc_1_0972:
        jmp    loc_1_0937
loc_1_0977:
        mov    word ptr [ebp - 0xc], 0
        jmp    loc_1_0986
loc_1_0982:
        inc    word ptr [ebp - 0xc]
loc_1_0986:
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, 0xffff
        cmp    eax, 2
        jae    loc_1_09C8
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 0x10]
        mov    al, byte ptr [eax + ecx + 2]
        and    eax, 0xff
        or     eax, eax
        je     loc_1_09C3
        mov    eax, dword ptr [ebp - 0x10]
        mov    byte ptr [eax], 9
        mov    eax, dword ptr [ebp - 0x10]
        mov    al, byte ptr [eax]
        jmp    loc_1_0ABA
loc_1_09C3:
        jmp    loc_1_0982
loc_1_09C8:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x10]
        mov    dword ptr [ebp - 0x14], eax
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_09E4
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x18]
        mov    dword ptr [ebp - 0x14], eax
loc_1_09E4:
        cmp    dword ptr [ebp - 0x14], 0
        jne    loc_1_09FE
        mov    eax, dword ptr [ebp - 0x10]
        mov    byte ptr [eax], 4
        mov    eax, dword ptr [ebp - 0x10]
        mov    al, byte ptr [eax]
        jmp    loc_1_0ABA
loc_1_09FE:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x14], 0x18
        je     loc_1_0A15
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_0A2C
loc_1_0A15:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x1c], 0x18
        je     loc_1_0A3C
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_0A3C
loc_1_0A2C:
        mov    eax, dword ptr [ebp - 0x10]
        mov    byte ptr [eax], 4
        mov    eax, dword ptr [ebp - 0x10]
        mov    al, byte ptr [eax]
        jmp    loc_1_0ABA
loc_1_0A3C:
        mov    word ptr [ebp - 0xc], 0
        jmp    loc_1_0A4B
loc_1_0A47:
        inc    word ptr [ebp - 0xc]
loc_1_0A4B:
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, 0xffff
        cmp    eax, 4
        jae    loc_1_0A87
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 0x14]
        cmp    dword ptr [ecx + eax*4 + 8], 0
        je     loc_1_0A82
        mov    eax, dword ptr [ebp - 0x10]
        mov    byte ptr [eax], 9
        mov    eax, dword ptr [ebp - 0x10]
        mov    al, byte ptr [eax]
        jmp    loc_1_0ABA
loc_1_0A82:
        jmp    loc_1_0A47
loc_1_0A87:
        mov    eax, dword ptr [ebp - 0x14]
        mov    al, byte ptr [eax + 2]
        and    eax, 0xff
        or     eax, eax
        je     loc_1_0AAA
        mov    eax, dword ptr [ebp - 0x10]
        mov    byte ptr [eax], 9
        mov    eax, dword ptr [ebp - 0x10]
        mov    al, byte ptr [eax]
        jmp    loc_1_0ABA
loc_1_0AAA:
        mov    eax, dword ptr [ebp - 0x10]
        mov    byte ptr [eax], 0
        mov    eax, dword ptr [ebp - 0x10]
        mov    al, byte ptr [eax]
        jmp    loc_1_0ABA
loc_1_0ABA:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_0AC1:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x18]
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_0AE4
        mov    al, 4
        jmp    loc_1_0CD0
loc_1_0AE4:
        mov    eax, dword ptr [ebp - 4]
        add    eax, 4
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x1c], 0x10
        je     loc_1_0B0E
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x1c], 0x18
        je     loc_1_0B0E
        mov    al, 4
        jmp    loc_1_0CD0
loc_1_0B0E:
        mov    eax, dword ptr [ebp - 0x10]
        mov    byte ptr [eax + 1], 0
        mov    byte ptr [ebp - 0xc], 0
        jmp    loc_1_0B21
loc_1_0B1E:
        inc    byte ptr [ebp - 0xc]
loc_1_0B21:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0xc]
        cmp    eax, 2
        jae    loc_1_0B57
        xor    eax, eax
        mov    al, byte ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 0x10]
        cmp    dword ptr [ecx + eax*4 + 4], 0
        je     loc_1_0B52
        mov    eax, dword ptr [ebp - 0x10]
        mov    byte ptr [eax], 9
        mov    eax, dword ptr [ebp - 0x10]
        mov    al, byte ptr [eax]
        jmp    loc_1_0CD0
loc_1_0B52:
        jmp    loc_1_0B1E
loc_1_0B57:
        mov    byte ptr [ebp - 0xc], 0
        jmp    loc_1_0B63
loc_1_0B60:
        inc    byte ptr [ebp - 0xc]
loc_1_0B63:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0xc]
        cmp    eax, 2
        jae    loc_1_0B9F
        xor    eax, eax
        mov    al, byte ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    al, byte ptr [eax + ecx + 2]
        and    eax, 0xff
        or     eax, eax
        je     loc_1_0B9A
        mov    eax, dword ptr [ebp - 0x10]
        mov    byte ptr [eax], 9
        mov    eax, dword ptr [ebp - 0x10]
        mov    al, byte ptr [eax]
        jmp    loc_1_0CD0
loc_1_0B9A:
        jmp    loc_1_0B60
loc_1_0B9F:
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x10]
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_0BC2
        mov    eax, dword ptr [ebp - 0x10]
        mov    byte ptr [eax], 4
        mov    eax, dword ptr [ebp - 0x10]
        mov    al, byte ptr [eax]
        jmp    loc_1_0CD0
loc_1_0BC2:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        add    eax, 0x20
        mov    ecx, dword ptr [ebp + 8]
        cmp    eax, dword ptr [ecx + 0x14]
        je     loc_1_0BE6
        mov    eax, dword ptr [ebp - 0x10]
        mov    byte ptr [eax], 4
        mov    eax, dword ptr [ebp - 0x10]
        mov    al, byte ptr [eax]
        jmp    loc_1_0CD0
loc_1_0BE6:
        mov    byte ptr [ebp - 0xc], 0
        jmp    loc_1_0BF2
loc_1_0BEF:
        inc    byte ptr [ebp - 0xc]
loc_1_0BF2:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0xc]
        cmp    eax, 4
        jae    loc_1_0C28
        xor    eax, eax
        mov    al, byte ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [ecx + eax*4 + 0x10], 0
        je     loc_1_0C23
        mov    eax, dword ptr [ebp - 0x10]
        mov    byte ptr [eax], 9
        mov    eax, dword ptr [ebp - 0x10]
        mov    al, byte ptr [eax]
        jmp    loc_1_0CD0
loc_1_0C23:
        jmp    loc_1_0BEF
loc_1_0C28:
        mov    byte ptr [ebp - 0xc], 0
        jmp    loc_1_0C34
loc_1_0C31:
        inc    byte ptr [ebp - 0xc]
loc_1_0C34:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0xc]
        cmp    eax, 3
        jae    loc_1_0C70
        xor    eax, eax
        mov    al, byte ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + ecx + 0xd]
        and    eax, 0xff
        or     eax, eax
        je     loc_1_0C6B
        mov    eax, dword ptr [ebp - 0x10]
        mov    byte ptr [eax], 9
        mov    eax, dword ptr [ebp - 0x10]
        mov    al, byte ptr [eax]
        jmp    loc_1_0CD0
loc_1_0C6B:
        jmp    loc_1_0C31
loc_1_0C70:
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0xb]
        and    eax, 0xff
        or     eax, eax
        je     loc_1_0C93
        mov    eax, dword ptr [ebp - 0x10]
        mov    byte ptr [eax], 9
        mov    eax, dword ptr [ebp - 0x10]
        mov    al, byte ptr [eax]
        jmp    loc_1_0CD0
loc_1_0C93:
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax], 0
        je     loc_1_0CBE
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax], 0x200
        je     loc_1_0CBE
        mov    eax, dword ptr [ebp - 0x10]
        mov    byte ptr [eax], 4
        mov    eax, dword ptr [ebp - 0x10]
        mov    al, byte ptr [eax]
        jmp    loc_1_0CD0
loc_1_0CBE:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_0F3F
        jmp    loc_1_0CD0
loc_1_0CD0:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_0CD7:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x18]
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_0CFA
        mov    al, 4
        jmp    loc_1_0F38
loc_1_0CFA:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x1c], 0x10
        jae    loc_1_0D0E
        mov    al, 4
        jmp    loc_1_0F38
loc_1_0D0E:
        mov    eax, dword ptr [ebp - 4]
        add    eax, 4
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 0x10]
        mov    byte ptr [eax + 1], 0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x10]
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], 0
        jne    loc_1_0D41
        mov    eax, dword ptr [ebp - 0x10]
        mov    byte ptr [eax], 4
        mov    eax, dword ptr [ebp - 0x10]
        mov    al, byte ptr [eax]
        jmp    loc_1_0F38
loc_1_0D41:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x14], 0x20
        je     loc_1_0D5E
        mov    eax, dword ptr [ebp - 0x10]
        mov    byte ptr [eax], 4
        mov    eax, dword ptr [ebp - 0x10]
        mov    al, byte ptr [eax]
        jmp    loc_1_0F38
loc_1_0D5E:
        mov    byte ptr [ebp - 0xc], 0
        jmp    loc_1_0D6A
loc_1_0D67:
        inc    byte ptr [ebp - 0xc]
loc_1_0D6A:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0xc]
        cmp    eax, 2
        jae    loc_1_0DA0
        xor    eax, eax
        mov    al, byte ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 0x10]
        cmp    dword ptr [ecx + eax*4 + 4], 0
        je     loc_1_0D9B
        mov    eax, dword ptr [ebp - 0x10]
        mov    byte ptr [eax], 9
        mov    eax, dword ptr [ebp - 0x10]
        mov    al, byte ptr [eax]
        jmp    loc_1_0F38
loc_1_0D9B:
        jmp    loc_1_0D67
loc_1_0DA0:
        mov    byte ptr [ebp - 0xc], 0
        jmp    loc_1_0DAC
loc_1_0DA9:
        inc    byte ptr [ebp - 0xc]
loc_1_0DAC:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0xc]
        cmp    eax, 2
        jae    loc_1_0DE8
        xor    eax, eax
        mov    al, byte ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 0x10]
        mov    al, byte ptr [eax + ecx + 2]
        and    eax, 0xff
        or     eax, eax
        je     loc_1_0DE3
        mov    eax, dword ptr [ebp - 0x10]
        mov    byte ptr [eax], 9
        mov    eax, dword ptr [ebp - 0x10]
        mov    al, byte ptr [eax]
        jmp    loc_1_0F38
loc_1_0DE3:
        jmp    loc_1_0DA9
loc_1_0DE8:
        mov    byte ptr [ebp - 0xc], 0
        jmp    loc_1_0DF4
loc_1_0DF1:
        inc    byte ptr [ebp - 0xc]
loc_1_0DF4:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0xc]
        cmp    eax, 4
        jae    loc_1_0E2A
        xor    eax, eax
        mov    al, byte ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [ecx + eax*4 + 0x10], 0
        je     loc_1_0E25
        mov    eax, dword ptr [ebp - 0x10]
        mov    byte ptr [eax], 9
        mov    eax, dword ptr [ebp - 0x10]
        mov    al, byte ptr [eax]
        jmp    loc_1_0F38
loc_1_0E25:
        jmp    loc_1_0DF1
loc_1_0E2A:
        mov    byte ptr [ebp - 0xc], 0
        jmp    loc_1_0E36
loc_1_0E33:
        inc    byte ptr [ebp - 0xc]
loc_1_0E36:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0xc]
        cmp    eax, 3
        jae    loc_1_0E72
        xor    eax, eax
        mov    al, byte ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + ecx + 0xd]
        and    eax, 0xff
        or     eax, eax
        je     loc_1_0E6D
        mov    eax, dword ptr [ebp - 0x10]
        mov    byte ptr [eax], 9
        mov    eax, dword ptr [ebp - 0x10]
        mov    al, byte ptr [eax]
        jmp    loc_1_0F38
loc_1_0E6D:
        jmp    loc_1_0E33
loc_1_0E72:
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax + 0xb]
        and    eax, 0xff
        or     eax, eax
        je     loc_1_0E95
        mov    eax, dword ptr [ebp - 0x10]
        mov    byte ptr [eax], 9
        mov    eax, dword ptr [ebp - 0x10]
        mov    al, byte ptr [eax]
        jmp    loc_1_0F38
loc_1_0E95:
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax], 0
        je     loc_1_0EC0
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax], 0x200
        je     loc_1_0EC0
        mov    eax, dword ptr [ebp - 0x10]
        mov    byte ptr [eax], 4
        mov    eax, dword ptr [ebp - 0x10]
        mov    al, byte ptr [eax]
        jmp    loc_1_0F38
loc_1_0EC0:
        mov    byte ptr [ebp - 0xc], 0
        jmp    loc_1_0ECC
loc_1_0EC9:
        inc    byte ptr [ebp - 0xc]
loc_1_0ECC:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0xc]
        cmp    eax, 2
        jae    loc_1_0F02
        xor    eax, eax
        mov    al, byte ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 0x10]
        cmp    dword ptr [ecx + eax*4 + 4], 0
        je     loc_1_0EFD
        mov    eax, dword ptr [ebp - 0x10]
        mov    byte ptr [eax], 9
        mov    eax, dword ptr [ebp - 0x10]
        mov    al, byte ptr [eax]
        jmp    loc_1_0F38
loc_1_0EFD:
        jmp    loc_1_0EC9
loc_1_0F02:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        add    eax, 0x10
        mov    ecx, dword ptr [ebp + 8]
        cmp    eax, dword ptr [ecx + 0x1c]
        je     loc_1_0F26
        mov    eax, dword ptr [ebp - 0x10]
        mov    byte ptr [eax], 4
        mov    eax, dword ptr [ebp - 0x10]
        mov    al, byte ptr [eax]
        jmp    loc_1_0F38
loc_1_0F26:
        mov    eax, dword ptr [ebp - 8]
        push   eax
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_1_0F3F
        jmp    loc_1_0F38
loc_1_0F38:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
sub_1_0F3F:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0xc]
        and    eax, 0xff
        sar    eax, 1
        and    eax, 0xffff
        cmp    eax, 1
        jle    loc_1_0F73
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax], 5
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        jmp    loc_1_1160
loc_1_0F73:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax], 3
        mov    byte ptr [ebp - 8], 0
        jmp    loc_1_0F85
loc_1_0F82:
        inc    byte ptr [ebp - 8]
loc_1_0F85:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        cmp    eax, 3
        jae    loc_1_0FC7
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        mov    al, byte ptr [eax + 0x12fc]   ; -> loc_1_12FC
        and    eax, 0xff
        mov    ecx, dword ptr [ebp + 0xc]
        mov    cl, byte ptr [ecx + 0xa]
        and    ecx, 0xff
        cmp    eax, ecx
        jne    loc_1_0FC2
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax], 0
        jmp    loc_1_0FC7
loc_1_0FC2:
        jmp    loc_1_0F82
loc_1_0FC7:
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        and    eax, 0xff
        or     eax, eax
        je     loc_1_0FE3
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        jmp    loc_1_1160
loc_1_0FE3:
        mov    eax, 1
        mov    ecx, dword ptr [ebp + 0xc]
        mov    cl, byte ptr [ecx + 0xc]
        shl    eax, cl
        xor    ecx, ecx
        mov    cl, byte ptr [0x11ac]         ; -> loc_1_11AC
        test   ecx, eax
        jne    loc_1_1010
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax], 0xa
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        jmp    loc_1_1160
loc_1_1010:
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0xa]
        and    eax, 0xff
        cmp    eax, 0xb0
        jne    loc_1_1026
loc_1_1026:
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        mov    al, byte ptr [eax + 0x1300]   ; -> loc_1_1300
        mov    byte ptr [ebp - 4], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   al, 1
        je     loc_1_107D
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 4]
        and    eax, 0xff
        cmp    eax, 0xd0
        jl     loc_1_106D
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 4]
        and    eax, 0xff
        cmp    eax, 0xef
        jle    loc_1_107D
loc_1_106D:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax], 8
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        jmp    loc_1_1160
loc_1_107D:
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 4]
        and    eax, 0xff
        cmp    eax, 0xd7
        jne    loc_1_10AA
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax + 1], 4
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax], 1
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        jmp    loc_1_1160
loc_1_10AA:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   al, 8
        je     loc_1_10DB
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 7]
        and    eax, 0xff
        cmp    eax, 0x4f
        je     loc_1_10DB
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax], 8
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        jmp    loc_1_1160
loc_1_10DB:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   al, 0x10
        je     loc_1_110E
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 8]
        and    eax, 0xff
        cmp    eax, 0xc2
        je     loc_1_110E
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax], 8
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        jmp    loc_1_1160
loc_1_110E:
        xor    eax, eax
        mov    al, byte ptr [ebp - 4]
        test   al, 0x20
        je     loc_1_1150
        mov    eax, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + 0xc]
        and    eax, 1
        shl    eax, 4
        or     eax, 0xa0
        mov    ecx, dword ptr [ebp + 0xc]
        mov    cl, byte ptr [ecx + 9]
        and    ecx, 0xf0
        cmp    eax, ecx
        je     loc_1_1150
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax], 5
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        jmp    loc_1_1160
loc_1_1150:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [eax], 0
        mov    eax, dword ptr [ebp + 8]
        mov    al, byte ptr [eax]
        jmp    loc_1_1160
loc_1_1160:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    8
        db  000h
sub_1_1168:
        VMMCall  VMM._LinPageLock                ; cd2063800100
        add    byte ptr [eax], al
sub_1_1170:
        VMMCall  VMM._LinPageUnLock              ; cd2064800100
        add    byte ptr [eax], al
sub_1_1178:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_11AC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  4, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'SMARTVSD'                  ; DDB_Name
        dd  080000000h                  ; DDB_Init_Order
        dd  SMARTVSD_Control         ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_1230:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        pushal 
        mov    esi, dword ptr [ebp + 8]
        VMMCall  VMM.Out_Debug_String            ; cd20c2000100
        popal  
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_1250:
        push   ebx
        dec    ebp
        inc    ecx
        push   edx
        push   esp
        push   esi
        push   ebx
        inc    esp
        cmp    ah, byte ptr [eax]
        dec    esp
        outsd  dx, dword ptr [esi]
        arpl   word ptr [ebx + 0x42], bp
        jne    0x12c7
        jb     loc_1_128D
        sub    dword ptr [eax], esp
        dec    esi
        push   ebp
        dec    esp
        dec    esp
        and    byte ptr [edx + 0x69], dl
        outsb  dx, byte ptr [esi]
        xor    esp, dword ptr [bx + si]
        jo     loc_1_12E3
        imul   ebp, dword ptr [esi + 0x74], 0xd0a7265
        db  000h
loc_1_127C:
        push   ebx
        dec    ebp
        inc    ecx
        push   edx
        push   esp
        push   esi
        push   ebx
        inc    esp
        cmp    ah, byte ptr [eax]
        dec    esp
        outsd  dx, dword ptr [esi]
        arpl   word ptr [ebx + 0x42], bp
        jne    loc_1_12F3
loc_1_128D:
        jb     0x12b9
        sub    dword ptr [eax], esp
        inc    ebx
        popal  
        outsb  dx, byte ptr [esi]
        daa    
        je     0x12b9
        insb   byte ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        arpl   word ptr [ebx + 0x21], bp
        or     cl, byte ptr [0]
loc_1_12A4:
        push   ebx
        dec    ebp
        inc    ecx
        push   edx
        push   esp
        push   esi
        push   ebx
        inc    esp
        cmp    ah, byte ptr [eax]
        and    byte ptr [ebp + 0x6e], dl
        dec    esp
        outsd  dx, dword ptr [esi]
        arpl   word ptr [ebx + 0x42], bp
        jne    0x131e
        jb     0x12e4
        sub    dword ptr [eax], esp
        dec    esi
        push   ebp
        dec    esp
        dec    esp
        and    byte ptr [eax + 0x6f], dh
        imul   ebp, dword ptr [esi + 0x74], 0xa207265
        or     eax, 0x53000000
        dec    ebp
        inc    ecx
        push   edx
        push   esp
        push   esi
        push   ebx
        inc    esp
        cmp    ah, byte ptr [eax]
        push   ebp
        outsb  dx, byte ptr [esi]
        dec    esp
        outsd  dx, dword ptr [esi]
        arpl   word ptr [ebx + 0x42], bp
        jne    0x1349
loc_1_12E3:
        jb     0x130f
        sub    dword ptr [eax], esp
        inc    ebx
        popal  
        outsb  dx, byte ptr [esi]
        daa    
        je     0x130f
        push   ebp
        outsb  dx, byte ptr [esi]
        dec    esp
        outsd  dx, dword ptr [esi]
loc_1_12F3:
        arpl   word ptr [ebx + 0x21], bp
        or     cl, byte ptr [0]
loc_1_12FC:
        mov    al, 0xec
        db  0A1h, 000h
loc_1_1300:
        cmp    dword ptr [eax], esp
        and    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x5c  R-X
loc_2_0000:
        push   0x24                          ; -> loc_2_0024
        VxDCall  BLOCKDEV.Service_0007h          ; cd2007001000
        add    esp, 4
        cmp    word ptr [0x50], 1            ; -> obj2:0x0050
        je     loc_2_0023
        cmp    word ptr [0x50], 2            ; -> obj2:0x0050
        je     loc_2_0023
        stc    
loc_2_0023:
        ret    
loc_2_0024:
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        sub    al, 0
        add    byte ptr [eax], al
        js     loc_2_0047
        add    byte ptr [eax], al
        push   ebx
        dec    ebp
        inc    ecx
        push   edx
        push   esp
        and    byte ptr [esi + 0x53], dl
        inc    esp
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_2_0047:
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

VxD__LTEXT SEGMENT   ; object 3  vsize=0x204  R-X
sub_3_0000:
        mov    al, byte ptr [0x1209]         ; -> obj1:0x1209
        shl    al, 2
        or     al, byte ptr [0x1200]         ; -> obj1:0x1200
        mov    byte ptr [0x11ac], al         ; -> loc_1_11AC
        cmp    byte ptr [0x11ac], 0          ; -> loc_1_11AC
        jne    loc_3_0022
        mov    word ptr [ebx + 2], 0xffff
loc_3_0022:
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_3_0030:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0xc]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        test   byte ptr [eax + 0x21], 0x60
        jne    loc_3_008B
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x74]
        and    eax, 0xff
        or     eax, eax
        jne    loc_3_0075
        mov    eax, dword ptr [ebp - 4]
        mov    al, byte ptr [eax + 0x40]
        and    eax, 0xff
        or     eax, eax
        je     loc_3_0082
loc_3_0075:
        mov    eax, dword ptr [ebp - 4]
        test   byte ptr [eax + 0x24], 2
        je     loc_3_008B
loc_3_0082:
        mov    eax, dword ptr [ebp - 4]
        push   eax
        call   sub_3_0090
loc_3_008B:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_3_0090:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xec
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ebp - 4], 0
        mov    byte ptr [ebp - 0x14], 0
        push   0
        mov    eax, dword ptr [ebp + 8]
        mov    eax, dword ptr [eax + 0x70]
        push   eax
        lea    eax, [ebp - 0x10]
        push   eax
        call   sub_5_0000
        add    esp, 0xc
        mov    byte ptr [ebp - 0xc], 0
        jmp    loc_3_00C8
loc_3_00C5:
        inc    byte ptr [ebp - 0xc]
loc_3_00C8:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0xc]
        cmp    eax, 2
        jge    loc_3_0153
        xor    eax, eax
        mov    al, byte ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 0x10]
        cmp    dword ptr [eax*4], ecx        ; -> obj4:0x0000
        jne    loc_3_014E
        xor    eax, eax
        mov    al, byte ptr [ebp - 0xc]
        mov    al, byte ptr [eax + eax*8 + 0x1200] ; -> obj1:0x1200
        and    eax, 0xff
        or     al, 2
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 0xc]
        mov    byte ptr [ecx + ecx*8 + 0x1200], al ; -> obj1:0x1200
        mov    eax, dword ptr [ebp + 8]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 0xc]
        mov    dword ptr [ecx + ecx*8 + 0x1205], eax ; -> obj1:0x1205
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax + 0x24], 2
        je     loc_3_0145
        xor    eax, eax
        mov    al, byte ptr [ebp - 0xc]
        mov    al, byte ptr [eax + eax*8 + 0x1200] ; -> obj1:0x1200
        and    eax, 0xff
        or     al, 0x20
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 0xc]
        mov    byte ptr [ecx + ecx*8 + 0x1200], al ; -> obj1:0x1200
loc_3_0145:
        mov    byte ptr [ebp - 0x14], 0xff
        jmp    loc_3_0153
loc_3_014E:
        jmp    loc_3_00C5
loc_3_0153:
        xor    eax, eax
        mov    al, byte ptr [ebp - 0x14]
        or     eax, eax
        jne    loc_3_01FD
        push   0
        mov    eax, dword ptr [ebp - 0x10]
        push   eax
        lea    eax, [ebp - 0xec]
        push   eax
        call   sub_5_0008
        add    esp, 0xc
        or     eax, eax
        je     loc_3_017D
loc_3_017D:
        xor    eax, eax
        mov    ax, word ptr [ebp - 0x8e]
        sub    eax, 0x1f0
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        mov    byte ptr [ebp - 8], al
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        mov    al, byte ptr [eax + eax*8 + 0x1200] ; -> obj1:0x1200
        and    eax, 0xff
        or     al, 1
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 8]
        mov    byte ptr [ecx + ecx*8 + 0x1200], al ; -> obj1:0x1200
        mov    eax, dword ptr [ebp + 8]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 8]
        mov    dword ptr [ecx + ecx*8 + 0x1201], eax ; -> obj1:0x1201
        mov    eax, dword ptr [ebp - 0x10]
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 8]
        mov    dword ptr [ecx*4], eax        ; -> obj4:0x0000
        mov    eax, dword ptr [ebp + 8]
        test   byte ptr [eax + 0x24], 2
        je     loc_3_01FD
        xor    eax, eax
        mov    al, byte ptr [ebp - 8]
        mov    al, byte ptr [eax + eax*8 + 0x1200] ; -> obj1:0x1200
        and    eax, 0xff
        or     al, 0x10
        xor    ecx, ecx
        mov    cl, byte ptr [ebp - 8]
        mov    byte ptr [ecx + ecx*8 + 0x1200], al ; -> obj1:0x1200
loc_3_01FD:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    4
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 4  vsize=0x8  RW-
        db  0CDh, 020h, 003h, 080h, 033h, 000h, 000h, 000h
VxD__LDATA ENDS

VxD__LTEXT SEGMENT   ; object 5  vsize=0xe  R-X
sub_5_0000:
        VxDCall  CONFIGMG._CONFIGMG_Get_Parent   ; cd2003803300
        add    byte ptr [eax], al
sub_5_0008:
        VxDCall  Dev_2633.Service_803Bh          ; cd203b803326
VxD__LTEXT ENDS

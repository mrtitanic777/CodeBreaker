; VFAT -- reconstructed 32-bit disassembly (VxD / LE format)
; 5 objects, 18963 instructions recovered by recursive descent

Declare_Virtual_Device VFAT, 3, 0, VFAT_Control, 00486h, Undefined_Init_Order
;   DDK version 0400h   init_order 0A0010100h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0xbf6c  R-X
loc_1_0000:
        cmp    dword ptr [0xb6a0], esi       ; -> obj1:0xB6A0
        je     loc_1_0019
        cmp    dword ptr [0xb690], esi       ; -> obj1:0xB690
        je     loc_1_0022
loc_1_0010:
        cmp    dword ptr [0xb1b8], esi       ; -> obj1:0xB1B8
        je     loc_1_002B
        ret    
loc_1_0019:
        and    dword ptr [0xb6a0], 0         ; -> obj1:0xB6A0
        jmp    8
loc_1_0022:
        and    dword ptr [0xb690], 0         ; -> obj1:0xB690
        jmp    loc_1_0010
loc_1_002B:
        mov    dword ptr [0xb1b4], 0xf0000000 ; -> obj1:0xB1B4
        ret    
sub_1_0036:
        and    byte ptr [esi + 0x28], al
        jne    loc_1_0048
        lea    eax, [esi + 0x2c]
        cmp    dword ptr [eax], 0
        je     loc_1_0048
        jmp    sub_1_A258
loc_1_0048:
        ret    
loc_1_0049:
        call   sub_1_025A
        mov    eax, eax
sub_1_0050:
        call   sub_1_00B0
        jae    loc_1_0060
        cmp    dword ptr [0xb1cc], 1         ; -> obj1:0xB1CC
        jae    loc_1_0049
loc_1_0060:
        ret    
        db  02Eh, 08Bh, 0C0h
sub_1_0064:
        cmp    ebx, dword ptr [0xb1b4]       ; -> obj1:0xB1B4
        jne    loc_1_0099
        test   al, 0xf8
        jne    loc_1_0099
        mov    esi, dword ptr [0xb1b8]       ; -> obj1:0xB1B8
        cmp    dword ptr [esi + 0xc], edi
        jne    loc_1_0099
        test   al, 4
        je     loc_1_008B
        push   eax
        push   ecx
        push   edx
        VxDCall  Dev_048B.Service_0006h          ; cd2006008b04
        pop    edx
        pop    ecx
        pop    eax
loc_1_008B:
        test   al, 2
        mov    eax, dword ptr [esi + 0x10]
        jne    loc_1_0093
        ret    
loc_1_0093:
        VxDCall  Dev_048B.Service_8007h          ; cd2007808b04
loc_1_0099:
        VxDCall  Dev_048B.Service_0004h          ; cd2004008b04
        jb     loc_1_00AD
        mov    dword ptr [0xb1b4], ebx       ; -> obj1:0xB1B4
        mov    dword ptr [0xb1b8], esi       ; -> obj1:0xB1B8
loc_1_00AD:
        ret    
        db  08Bh, 0C0h
sub_1_00B0:
        mov    ah, byte ptr [0xb6f8]         ; -> obj1:0xB6F8
        mov    al, 0
        call   sub_1_0064
        jb     loc_1_00C4
        cmp    byte ptr [esi + 0x28], 0
        ret    
loc_1_00C4:
        ret    
loc_1_00C5:
        call   sub_1_0272
        mov    ebx, edx
        mov    al, 0x10
        jmp    loc_1_00D5
sub_1_00D0:
        mov    edi, dword ptr [ebp + 0x1c]
        sub    al, al
loc_1_00D5:
        push   ebx
        sub    ebx, dword ptr [edi + 0xc]
        mov    cl, byte ptr [edi + 0x4e]
        not    cl
        and    bl, cl
        add    ebx, dword ptr [edi + 0xc]
        mov    ah, byte ptr [0xb6f8]         ; -> obj1:0xB6F8
        or     al, 5
        call   sub_1_0064
        pop    edx
        jae    loc_1_0108
        test   esi, esi
        je     loc_1_00C5
        push   edi
        push   edx
        call   sub_1_1628
        pop    edx
        pop    edi
        mov    ebx, edx
        jb     sub_1_00D0
        mov    ebx, dword ptr [esi + 8]
        nop    
loc_1_0108:
        cmp    byte ptr [esi + 0x28], 0
        jne    loc_1_01C3
        sub    edx, ebx
        bt     dword ptr [esi + 0x24], edx
        jb     loc_1_0125
        mov    cl, byte ptr [edi + 0x4c]
        shl    edx, cl
        add    edx, dword ptr [esi + 0x10]
        mov    ebx, edx
        ret    
loc_1_0125:
        VxDCall  Dev_048B.Service_0007h          ; cd2007008b04
loc_1_012B:
        bts    dword ptr [0xb1c0], 0         ; -> obj1:0xB1C0
        jae    loc_1_0141
        mov    eax, 0xb1c8                   ; -> obj1:0xB1C8
        call   sub_1_A220
        jmp    loc_1_012B
loc_1_0141:
        mov    dword ptr [0xb1c4], esi       ; -> obj1:0xB1C4
        push   edx
        push   esi
        xchg   edi, ebx
        add    edi, edx
        mov    cl, byte ptr [ebx + 0x4c]
        shl    edx, cl
        add    edx, dword ptr [esi + 0x10]
        mov    esi, edx
        mov    ecx, 1
        push   esi
        call   sub_1_1458
        pop    ebx
        pop    esi
        pop    ecx
        jb     loc_1_019A
        btr    dword ptr [esi + 0x24], ecx
        btr    dword ptr [esi + 0x25], ecx
        mov    dword ptr [0xb1c4], 0         ; -> obj1:0xB1C4
        mov    byte ptr [0xb1c0], 0          ; -> obj1:0xB1C0
        mov    eax, 0xb1c8                   ; -> obj1:0xB1C8
        cmp    dword ptr [eax], 0
        je     loc_1_018F
        call   sub_1_A258
loc_1_018F:
        mov    edi, dword ptr [esi + 0xc]
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
        clc    
        ret    
loc_1_019A:
        bts    dword ptr [esi + 0x24], ecx
        bts    dword ptr [esi + 0x25], ecx
        mov    byte ptr [0xb1c0], 0          ; -> obj1:0xB1C0
        mov    eax, 0xb1c8                   ; -> obj1:0xB1C8
        cmp    dword ptr [eax], 0
        je     loc_1_01B8
        call   sub_1_A258
loc_1_01B8:
        mov    edi, dword ptr [esi + 0xc]
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
        stc    
        ret    
loc_1_01C3:
        call   sub_1_059A
        mov    ebx, edx
        jmp    sub_1_00D0
        db  090h
sub_1_01D0:
        call   sub_1_00D0
        jb     loc_1_01D8
        ret    
loc_1_01D8:
        mov    edx, dword ptr [esi + 0xc]
        cmp    byte ptr [edx + 0x46], 2
        jne    loc_1_024E
        test   byte ptr [edx + 0x52], 4
        jne    loc_1_024E
        test   byte ptr [ebp + 0x39], 0x40
        jne    loc_1_024E
        VxDCall  Dev_048B.Service_0007h          ; cd2007008b04
        mov    byte ptr [ebp + 0x34], 0
        push   ebx
        push   esi
        sub    ebx, dword ptr [esi + 0x10]
        mov    cl, byte ptr [edx + 0x4c]
        shr    ebx, cl
        push   ebx
        add    ebx, dword ptr [esi + 8]
        mov    ecx, dword ptr [edx + 0x30]
        cmp    byte ptr [edx + 0x48], 0
        je     loc_1_0210
        neg    ecx
loc_1_0210:
        add    ebx, ecx
        call   sub_1_00D0
        jb     loc_1_0245
        mov    esi, ebx
        pop    edx
        mov    eax, dword ptr [esp]
        btr    dword ptr [eax + 0x24], edx
        jae    loc_1_0250
        btr    dword ptr [eax + 0x25], edx
        mov    ecx, dword ptr [eax + 0xc]
        movzx  ecx, word ptr [ecx + 0x20]
        shr    ecx, 2
        push   edi
        mov    edi, dword ptr [esp + 8]
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    edi
        pop    esi
        pop    ebx
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
        jmp    loc_1_0258
loc_1_0245:
        pop    eax
        pop    esi
        pop    ebx
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
loc_1_024E:
        stc    
        ret    
loc_1_0250:
        pop    esi
        pop    ebx
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
loc_1_0258:
        clc    
        ret    
sub_1_025A:
        cmp    dword ptr [0xb1cc], 0         ; -> obj1:0xB1CC
        je     loc_1_0271
        push   eax
        mov    eax, 0xb1d0                   ; -> obj1:0xB1D0
        call   sub_1_A220
        pop    eax
        jmp    sub_1_025A
loc_1_0271:
        ret    
sub_1_0272:
        pushal 
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        movzx  edi, word ptr [edi + 0x18]
        bts    dword ptr [0xb1cc], 0         ; -> obj1:0xB1CC
        jb     loc_1_02CC
        mov    dword ptr [0xb1dc], edi       ; -> obj1:0xB1DC
        sub    eax, eax
        mov    dword ptr [0xb1d8], eax       ; -> obj1:0xB1D8
        call   sub_1_05C6
        cmp    dword ptr [0xb1d8], 0         ; -> obj1:0xB1D8
        jne    loc_1_02AC
loc_1_02A2:
        mov    eax, 0xb1d4                   ; -> obj1:0xB1D4
        call   sub_1_A220
loc_1_02AC:
        popal  
        sub    eax, eax
        mov    dword ptr [0xb1cc], eax       ; -> obj1:0xB1CC
        mov    dword ptr [0xb1dc], eax       ; -> obj1:0xB1DC
        cmp    dword ptr [0xb1d0], eax       ; -> obj1:0xB1D0
        jne    loc_1_02C2
        ret    
loc_1_02C2:
        mov    eax, 0xb1d0                   ; -> obj1:0xB1D0
        jmp    sub_1_A258
loc_1_02CC:
        cmp    edi, dword ptr [0xb1dc]       ; -> obj1:0xB1DC
        je     loc_1_02D7
        popal  
        jmp    sub_1_025A
loc_1_02D7:
        cmp    dword ptr [0xb1d4], 0         ; -> obj1:0xB1D4
        je     loc_1_02AC
        jmp    loc_1_02A2
        db  001h, 003h, 007h, 00Fh, 01Fh, 03Fh, 07Fh, 0FFh
loc_1_02EA:
        call   sub_1_0272
        mov    ebx, edx
        mov    al, 0x10
        jmp    loc_1_02FD
        db  02Eh, 08Bh, 0C0h
sub_1_02F8:
        mov    edi, dword ptr [ebp + 0x1c]
        sub    al, al
loc_1_02FD:
        push   ebx
        sub    ebx, dword ptr [edi + 0xc]
        mov    cl, byte ptr [edi + 0x4e]
        not    cl
        and    bl, cl
        add    ebx, dword ptr [edi + 0xc]
        mov    ah, byte ptr [0xb6f8]         ; -> obj1:0xB6F8
        or     al, 5
        call   sub_1_0064
        pop    edx
        jae    loc_1_0330
        test   esi, esi
        je     loc_1_02EA
        movzx  ecx, byte ptr [edi + 0x4d]
        mov    cl, byte ptr [ecx + 0x2e1]    ; -> obj1:0x02E1
        mov    byte ptr [esi + 0x24], cl
        mov    ebx, dword ptr [esi + 8]
        nop    
loc_1_0330:
        cmp    byte ptr [esi + 0x28], 0
        jne    loc_1_035B
        sub    edx, ebx
        btr    dword ptr [esi + 0x24], edx
        btr    dword ptr [esi + 0x25], edx
        mov    cl, byte ptr [edi + 0x4c]
        shl    edx, cl
        add    edx, dword ptr [esi + 0x10]
        mov    ebx, edx
        movzx  ecx, word ptr [edi + 0x20]
        push   edi
        mov    edi, edx
        xor    eax, eax
        shr    ecx, 2
        cld    
        rep stosd dword ptr es:[edi], eax
        pop    edi
        ret    
loc_1_035B:
        call   sub_1_059A
        mov    ebx, edx
        jmp    sub_1_02F8
sub_1_0364:
        test   byte ptr [ebp + 0x39], 8
        setne  dh
        or     byte ptr [esi + 0x2b], dh
        movzx  eax, byte ptr [ebp + 0x35]
        bts    dword ptr [esi + 0x26], eax
        mov    ebx, dword ptr [esi + 0xc]
sub_1_0379:
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf000100
        cld    
        mov    dword ptr [esi + 0x14], eax
        cmp    dword ptr [esi + 0x1c], 0
        jne    loc_1_03EB
        mov    dword ptr [esi + 0x18], eax
        push   edi
        mov    edi, ebx
        call   sub_1_897F
        pop    edi
        pushfd 
        cli    
        mov    eax, 0xb6fc                   ; -> obj1:0xB6FC
        mov    ebx, dword ptr [eax + 0x20]
        mov    dword ptr [ebx + 0x1c], esi
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x20], ebx
        mov    dword ptr [eax + 0x20], esi
        or     byte ptr [esi + 0x2b], 8
        inc    dword ptr [0xb750]            ; -> loc_1_B750
        popfd  
        push   ecx
        push   edx
        mov    ah, byte ptr [0xb6f8]         ; -> obj1:0xB6F8
        VxDCall  Dev_048B.Service_0002h          ; cd2002008b04
        xchg   ecx, eax
        mov    eax, dword ptr [0xb750]       ; -> loc_1_B750
        sub    edx, edx
        shl    eax, 3
        div    ecx
        pop    edx
        pop    ecx
        mov    al, byte ptr [eax + 0xb1e0]   ; -> obj1:0xB1E0
        cmp    al, byte ptr [0xb1e9]         ; -> obj1:0xB1E9
        mov    byte ptr [0xb1e9], al         ; -> obj1:0xB1E9
        jbe    loc_1_03EB
        call   sub_1_8C47
loc_1_03EB:
        ret    
sub_1_03EC:
        test   byte ptr [ebp + 0x38], 4
        setne  dl
sub_1_03F3:
        mov    eax, dword ptr [esi + 0xc]
        sub    ebx, dword ptr [esi + 0x10]
        push   ecx
        mov    cl, byte ptr [eax + 0x4c]
        shr    ebx, cl
        pop    ecx
        sub    eax, eax
        bts    eax, ebx
        or     byte ptr [esi + 0x32], al
        or     dl, dl
        je     loc_1_0417
        push   ecx
        push   edi
        call   sub_1_0464
        pop    edi
        pop    ecx
        jmp    loc_1_0422
loc_1_0417:
        call   sub_1_0423
        call   sub_1_0364
        clc    
loc_1_0422:
        ret    
sub_1_0423:
        or     al, byte ptr [esi + 0x2a]
        cmp    al, byte ptr [esi + 0x32]
        jne    loc_1_0436
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf000100
        mov    dword ptr [esi + 0x18], eax
        sub    al, al
loc_1_0436:
        mov    byte ptr [esi + 0x2a], al
        ret    
sub_1_043A:
        mov    al, 1
        shl    al, cl
        dec    al
        xchg   ebx, ecx
        shl    al, cl
        xchg   ecx, ebx
        or     byte ptr [esi + 0x32], al
        or     dl, dl
        je     loc_1_0458
        push   ecx
        push   edi
        call   sub_1_04B3
        pop    edi
        pop    ecx
        jmp    loc_1_0463
loc_1_0458:
        call   sub_1_0423
        call   sub_1_0364
        clc    
loc_1_0463:
        ret    
sub_1_0464:
        mov    edi, dword ptr [esi + 0xc]
        mov    eax, ebx
        add    eax, dword ptr [esi + 8]
        cmp    dword ptr [edi + 0x28], eax
        jbe    loc_1_0478
        mov    ecx, 1
        jmp    sub_1_04B3
loc_1_0478:
        mov    al, bl
        mov    ah, byte ptr [edi + 0x24]
        and    ah, byte ptr [edi + 0x4e]
        and    al, ah
        mov    bh, al
        neg    al
        add    al, ah
        inc    al
        mov    cl, bl
        mov    dh, 1
        shl    dh, cl
        mov    ah, dh
        xor    ecx, ecx
loc_1_0494:
        test   byte ptr [esi + 0x32], dh
        je     loc_1_04A0
        inc    ecx
        add    dh, dh
        dec    al
        jne    loc_1_0494
loc_1_04A0:
        dec    bh
        jl     loc_1_04B0
        shr    ah, 1
        test   byte ptr [esi + 0x32], ah
        je     loc_1_04B0
        inc    ecx
        dec    bl
        jmp    loc_1_04A0
loc_1_04B0:
        movzx  ebx, bl
sub_1_04B3:
        VxDCall  Dev_048B.Service_0007h          ; cd2007008b04
        mov    edi, dword ptr [esi + 0xc]
        mov    al, 1
        shl    al, cl
        dec    al
        xchg   bl, cl
        shl    al, cl
        xchg   bl, cl
        xor    byte ptr [esi + 0x32], al
        mov    byte ptr [esi + 0x2a], 0
        push   eax
        push   ebx
        push   ecx
        push   edx
        push   esi
        mov    eax, ebx
        add    ebx, dword ptr [esi + 8]
        mov    esi, dword ptr [esi + 0x10]
        push   ecx
        mov    cl, byte ptr [edi + 0x4c]
        shl    eax, cl
        pop    ecx
        add    esi, eax
        call   sub_1_1574
        pop    esi
        pop    edx
        pop    ecx
        pop    ebx
        pop    eax
        pushfd 
        cmp    dl, 2
        je     loc_1_054B
        mov    edi, dword ptr [esi + 0xc]
        cmp    byte ptr [edi + 0x46], 1
        jbe    loc_1_054B
        test   byte ptr [edi + 0x52], 4
        jne    loc_1_054B
        push   eax
        mov    edx, ebx
        add    edx, dword ptr [esi + 8]
        add    ecx, edx
        movzx  eax, word ptr [edi + 0x26]
        cmp    ecx, eax
        jbe    loc_1_054A
        mov    eax, dword ptr [edi + 0x18]
        cmp    edx, eax
        jae    loc_1_054A
        cmp    ecx, eax
        jbe    loc_1_0521
        mov    ecx, eax
loc_1_0521:
        movzx  eax, word ptr [edi + 0x26]
        cmp    edx, eax
        jae    loc_1_052B
        mov    edx, eax
loc_1_052B:
        sub    ecx, edx
        mov    ebx, edx
        sub    ebx, eax
        add    ebx, dword ptr [edi + 0x18]
        sub    edx, dword ptr [esi + 8]
        push   ecx
        mov    cl, byte ptr [edi + 0x4c]
        shl    edx, cl
        pop    ecx
        add    edx, dword ptr [esi + 0x10]
        push   esi
        mov    esi, edx
        call   sub_1_1574
        pop    esi
loc_1_054A:
        pop    eax
loc_1_054B:
        popfd  
        pushfd 
        jb     loc_1_0592
loc_1_054F:
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
        cli    
        cmp    byte ptr [esi + 0x32], 0
        je     loc_1_0565
        popfd  
        pushfd 
        call   sub_1_0379
        jmp    loc_1_0590
loc_1_0565:
        mov    word ptr [esi + 0x26], 0
        test   byte ptr [esi + 0x2b], 0x18
        je     loc_1_0581
        push   esi
        mov    ebx, esi
        cli    
        call   sub_1_899F
        cli    
        call   sub_1_893E
        pop    esi
loc_1_0581:
        mov    eax, 0xb1d4                   ; -> obj1:0xB1D4
        cmp    dword ptr [eax], 0
        je     loc_1_0590
        call   sub_1_A258
loc_1_0590:
        popfd  
        ret    
loc_1_0592:
        or     byte ptr [esi + 0x24], al
        or     byte ptr [esi + 0x25], al
        jmp    loc_1_054F
sub_1_059A:
        cmp    byte ptr [esi + 0x28], 0
        je     loc_1_05C3
        push   eax
        push   ecx
        lea    eax, [esi + 0x28]
        push   dword ptr [esi + 8]
        push   dword ptr [esi + 0xc]
        mov    ecx, 0xff
        call   sub_1_A20C
        pop    eax
        cmp    eax, dword ptr [esi + 0xc]
        pop    eax
        jne    loc_1_05BF
        cmp    eax, dword ptr [esi + 8]
loc_1_05BF:
        pop    ecx
        pop    eax
        jne    loc_1_05C4
loc_1_05C3:
        ret    
loc_1_05C4:
        stc    
        ret    
sub_1_05C6:
        mov    edx, 0xffffffff
sub_1_05CB:
        pushfd 
        cli    
        mov    esi, 0xb6fc                   ; -> obj1:0xB6FC
loc_1_05D2:
        mov    esi, dword ptr [esi + 0x1c]
        cmp    esi, 0xb6fc                   ; -> obj1:0xB6FC
        je     loc_1_05F2
        and    eax, eax
        je     loc_1_05EC
        cmp    dword ptr [esi + 0xc], eax
        jne    loc_1_05D2
        test   word ptr [esi + 0x26], dx
        je     loc_1_05D2
loc_1_05EC:
        or     byte ptr [esi + 0x2b], 4
        jmp    loc_1_05D2
loc_1_05F2:
        popfd  
        mov    bl, 0
        jmp    sub_1_8279
sub_1_05FA:
        mov    edx, 0xffffffff
sub_1_05FF:
        push   eax
        call   sub_1_05CB
        pop    ecx
        mov    edi, 0xb748                   ; -> loc_1_B748
loc_1_060B:
        mov    edi, dword ptr [edi]
        cli    
        cmp    edi, 0xb748                   ; -> loc_1_B748
        je     loc_1_0634
        jecxz  loc_1_061C
        cmp    ecx, edi
        jne    loc_1_060B
loc_1_061C:
        mov    eax, dword ptr [edi + 0x68]
        or     eax, dword ptr [edi + 0x80]
        je     loc_1_060B
        lea    eax, [edi + 0x88]
        call   sub_1_A220
        jmp    loc_1_060B
loc_1_0634:
        sti    
        ret    
sub_1_0636:
        mov    eax, dword ptr [ebp + 0x1c]
        movzx  ecx, byte ptr [ebp + 0x35]
        xor    edx, edx
        bts    edx, ecx
        call   sub_1_05FF
        mov    ebx, dword ptr [ebp + 0x1c]
        movzx  eax, byte ptr [ebp + 0x35]
        xchg   byte ptr [eax + ebx + 0xbc], ah
        shr    eax, 8
        cmp    al, 0x10
        cmc    
        jb     loc_1_0668
        mov    eax, 0x27
        call   sub_1_44AF
        clc    
loc_1_0668:
        ret    
loc_1_0669:
        sub    eax, eax
        mov    edx, eax
        mov    ecx, 0xb748                   ; -> loc_1_B748
loc_1_0672:
        mov    ecx, dword ptr [ecx]
        cmp    ecx, 0xb748                   ; -> loc_1_B748
        je     loc_1_0681
        add    eax, dword ptr [ecx + 0x6c]
        jmp    loc_1_0672
loc_1_0681:
        shl    eax, 0xc
        ret    
sub_1_0685:
        cmp    word ptr [0xbd78], 0          ; -> obj1:0xBD78
        je     loc_1_07A9
        mov    esi, dword ptr [ebp + 0x1c]
        mov    esi, dword ptr [esi + 0x14]
        test   byte ptr [esi + 4], 0x80
        jne    loc_1_07A9
        cmp    edx, dword ptr [0xb890]       ; -> obj1:0xB890
        jae    loc_1_07A9
        mov    esi, dword ptr [ebp + 0x20]
        cmp    eax, dword ptr [esi + 0x18]
        lea    eax, [edx + eax]
        mov    dword ptr [esi + 0x18], eax
        jne    loc_1_07C1
        lea    edx, [eax + 0xfff]
        and    edx, 0xfffff000
        cmp    edx, dword ptr [esi + 0x28]
        jae    loc_1_06EA
        cmp    eax, dword ptr [esi + 0x24]
        jae    loc_1_07A9
        mov    dword ptr [esi + 0x24], edx
        mov    dword ptr [esi + 0x28], edx
        mov    dword ptr [esi + 0x20], 1
        jmp    loc_1_06ED
loc_1_06EA:
        mov    edx, dword ptr [esi + 0x28]
loc_1_06ED:
        sub    esp, 0xc
        mov    edi, dword ptr [ebp + 0x1c]
        mov    eax, dword ptr [esi + 0x20]
        mov    dword ptr [esp + 8], eax
        movsx  ecx, word ptr [edi + 0x50]
        and    edx, ecx
        mov    dword ptr [esp], edx
        neg    ecx
        mov    dword ptr [esp + 4], ecx
loc_1_0709:
        mov    edi, dword ptr [ebp + 0x1c]
        mov    ebx, dword ptr [esi + 8]
        mov    eax, dword ptr [esp]
        cmp    eax, dword ptr [ebx + 0x30]
        jae    loc_1_07A5
        mov    cl, byte ptr [edi + 0x4c]
        shr    eax, cl
        mov    ebx, eax
        and    bl, byte ptr [edi + 0x24]
        mov    cl, byte ptr [edi + 0x25]
        shr    eax, cl
        mov    dword ptr [ebp + 0x40], eax
        xchg   ebx, eax
        call   sub_1_7F50
        jb     loc_1_077F
        xchg   edx, eax
        call   sub_1_2102
        jae    loc_1_077F
        call   sub_1_7E5E
        jb     loc_1_077F
        sub    ebx, dword ptr [edi + 0xc]
        jb     loc_1_077F
        mov    cl, byte ptr [edi + 0x4e]
        not    cl
        and    bl, cl
        add    ebx, dword ptr [edi + 0xc]
        mov    ah, byte ptr [0xb6f8]         ; -> obj1:0xB6F8
        mov    al, 1
        call   sub_1_0064
        jae    loc_1_077F
        test   esi, esi
        je     loc_1_07A5
        cli    
        cmp    byte ptr [esi + 0x28], 0
        jne    loc_1_077F
        VxDCall  Dev_048B.Service_000Dh          ; cd200d008b04
        jne    loc_1_077F
        sti    
        push   esi
        call   sub_1_17B4
        pop    esi
        jb     loc_1_07AA
loc_1_077F:
        sti    
        mov    esi, dword ptr [ebp + 0x20]
        mov    eax, dword ptr [esp + 4]
        add    dword ptr [esp], eax
        add    dword ptr [esi + 0x28], eax
        dec    dword ptr [esp + 8]
        jne    loc_1_0709
        mov    esi, dword ptr [ebp + 0x20]
        sub    cl, cl
        cmp    dword ptr [esi + 0x20], 0x10
        adc    cl, cl
        shl    dword ptr [esi + 0x20], cl
loc_1_07A5:
        sti    
        add    esp, 0xc
loc_1_07A9:
        ret    
loc_1_07AA:
        cli    
        cmp    byte ptr [esi + 0x28], 0
        jne    loc_1_07A5
        VxDCall  Dev_048B.Service_000Dh          ; cd200d008b04
        jne    loc_1_07A5
        VxDCall  Dev_048B.Service_0005h          ; cd2005008b04
        jmp    loc_1_07A5
loc_1_07C1:
        mov    dword ptr [esi + 0x20], 1
        and    eax, 0xfffff000
        mov    dword ptr [esi + 0x24], eax
        mov    dword ptr [esi + 0x28], eax
        ret    
sub_1_07D4:
        test   word ptr [ebp + 0x42], 2
        jne    loc_1_0888
        mov    eax, dword ptr [ebp + 0x1c]
        test   word ptr [eax + 0x52], 0x2000
        jne    loc_1_0888
        inc    dword ptr [0xb274]            ; -> loc_1_B274
        sub    ebx, ebx
        cmp    word ptr [esi], 4
        jbe    loc_1_088D
        cmp    word ptr [esi + 2], 4
        jbe    loc_1_0885
        movzx  ecx, word ptr [esi + 2]
        sub    ecx, 4
        lea    esi, [esi + 4]
        sub    eax, eax
        xchg   word ptr [ecx + esi], ax
        push   eax
        push   ecx
        push   esi
        sub    ecx, ecx
        call   sub_1_0B81
        mov    eax, ecx
        mov    edx, dword ptr [0xb278]       ; -> loc_1_B278
        sub    edx, 0x18
        mov    ecx, dword ptr [0xb880]       ; -> loc_1_B880
loc_1_0833:
        add    edx, 0x18
        cmp    dword ptr [edx], eax
        loopne loc_1_0833
        mov    ebx, ecx
        pop    esi
        pop    ecx
        pop    eax
        mov    word ptr [ecx + esi], ax
        jne    loc_1_0888
        cmp    word ptr [edx + 0x14], cx
        jne    loc_1_0866
        mov    edi, dword ptr [edx + 0x10]
        cmp    dword ptr [ebp + 0x1c], edi
        jne    loc_1_0866
        mov    edi, dword ptr [edx + 8]
        test   edi, edi
        je     loc_1_0866
        mov    eax, esi
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        mov    esi, eax
        je     loc_1_0879
        movzx  ecx, word ptr [edx + 0x14]
loc_1_0866:
        or     ebx, ebx
        je     loc_1_0888
        sub    eax, eax
        xchg   word ptr [ecx + esi], ax
        push   eax
        push   ecx
        push   esi
        mov    ecx, ebx
        mov    eax, dword ptr [edx]
        jmp    loc_1_0833
loc_1_0879:
        mov    ebx, dword ptr [0xb274]       ; -> loc_1_B274
        mov    dword ptr [edx + 0xc], ebx
        mov    ebx, dword ptr [edx + 4]
loc_1_0885:
        cmp    ebx, ebx
        ret    
loc_1_0888:
        sub    eax, eax
        or     esp, esp
        ret    
loc_1_088D:
        sub    eax, eax
        stc    
        ret    
sub_1_0891:
        test   word ptr [ebp + 0x42], 2
        jne    loc_1_0976
        mov    eax, dword ptr [ebp + 0x1c]
        test   word ptr [eax + 0x52], 0x2000
        jne    loc_1_0976
        movzx  ecx, word ptr [esi + 2]
        sub    ecx, 4
        jbe    loc_1_0976
        lea    esi, [esi + 4]
        push   ecx
        push   esi
        mov    eax, ecx
        mov    esi, dword ptr [ebp + 0x64]
        VxDCall  Dev_048B.Service_0007h          ; cd2007008b04
        call   sub_1_A1CE
        push   eax
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
        jb     loc_1_0972
        mov    esi, dword ptr [esp + 4]
        mov    edi, dword ptr [esp]
        mov    ecx, dword ptr [esp + 8]
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, dword ptr [esp + 8]
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    esi, dword ptr [0xb278]       ; -> loc_1_B278
        mov    edi, esi
        mov    ecx, dword ptr [0xb880]       ; -> loc_1_B880
        mov    eax, 0xffffffff
        nop    
loc_1_0908:
        cmp    eax, dword ptr [esi + 0xc]
        jbe    loc_1_0912
        mov    edi, esi
        mov    eax, dword ptr [esi + 0xc]
loc_1_0912:
        add    esi, 0x18
        loop   loc_1_0908
        mov    ecx, dword ptr [esp]
        xchg   dword ptr [edi + 8], ecx
        jecxz  loc_1_0926
        xchg   ecx, eax
        call   sub_1_A1F8
        xchg   ecx, eax
loc_1_0926:
        mov    esi, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 8]
        sub    eax, eax
        xchg   word ptr [ecx + esi], ax
        push   eax
        sub    ecx, ecx
        call   sub_1_0B81
        pop    eax
        mov    word ptr [esi], ax
        mov    eax, ecx
        mov    dword ptr [edi], eax
        mov    edx, dword ptr [esp + 8]
        mov    word ptr [edi + 0x14], dx
        mov    edx, dword ptr [ebp + 0x1c]
        mov    dword ptr [edi + 0x10], edx
        test   byte ptr [edx + 0x52], 3
        mov    edx, dword ptr [ebx + 0x12]
        je     loc_1_095D
        xor    edx, edx
loc_1_095D:
        mov    dx, word ptr [ebx + 0x1a]
        mov    dword ptr [edi + 4], edx
        mov    edx, dword ptr [0xb274]       ; -> loc_1_B274
        mov    dword ptr [edi + 0xc], edx
        lea    esp, [esp + 0xc]
        ret    
loc_1_0972:
        lea    esp, [esp + 0xc]
loc_1_0976:
        sub    eax, eax
        ret    
sub_1_0979:
        mov    edi, dword ptr [0xb278]       ; -> loc_1_B278
        mov    ecx, dword ptr [0xb880]       ; -> loc_1_B880
loc_1_0985:
        or     eax, eax
        je     loc_1_098E
        cmp    dword ptr [edi + 4], eax
        jne    loc_1_09A9
loc_1_098E:
        cmp    dword ptr [edi + 0x10], ebx
        jne    loc_1_09A9
        sub    edx, edx
        mov    dword ptr [edi + 0x10], edx
        mov    dword ptr [edi + 0xc], edx
        xchg   dword ptr [edi + 8], edx
        and    edx, edx
        je     loc_1_09A9
        xchg   edx, eax
        call   sub_1_A1F8
        xchg   edx, eax
loc_1_09A9:
        add    edi, 0x18
        loop   loc_1_0985
        ret    
sub_1_09AF:
        test   word ptr [ebp + 0x42], 2
        jne    loc_1_09F8
        mov    eax, dword ptr [ebp + 0x1c]
        test   word ptr [eax + 0x52], 0x2000
        jne    loc_1_09F8
        push   8
        push   0
        call   sub_1_0B81
        mov    dword ptr [esp], ecx
        mov    esi, dword ptr [ebp + 0x1c]
        mov    esi, dword ptr [esi + 0x90]
loc_1_09D7:
        mov    eax, dword ptr [esp]
        sub    edx, edx
        div    dword ptr [0xb884]            ; -> loc_1_B884
        shl    edx, 4
        cmp    dword ptr [edx + esi + 8], ecx
        je     loc_1_09FB
loc_1_09EB:
        shr    dword ptr [esp], 3
        dec    dword ptr [esp + 4]
        jne    loc_1_09D7
        add    esp, 8
loc_1_09F8:
        or     esp, esp
        ret    
loc_1_09FB:
        cmp    dword ptr [edx + esi + 4], ebx
        jne    loc_1_09EB
        inc    dword ptr [0xb280]            ; -> loc_1_B280
        mov    eax, dword ptr [0xb280]       ; -> loc_1_B280
        mov    dword ptr [edx + esi + 0xc], eax
        mov    eax, dword ptr [edx + esi]
        add    esp, 8
        cmp    eax, eax
        ret    
sub_1_0A19:
        test   word ptr [ebp + 0x42], 2
        jne    loc_1_0AE1
        mov    eax, dword ptr [ebp + 0x1c]
        test   byte ptr [eax + 0x53], 0x20
        jne    loc_1_0AE1
        push   ebx
        push   edx
        push   esi
        push   edi
        push   0
        push   8
        push   0
        push   -1
        push   0
        mov    eax, esi
        lea    esi, [esi + 0xb2]
        test   byte ptr [eax + 7], 2
        jne    loc_1_0A54
        lea    esi, [eax + 0x96]
loc_1_0A54:
        call   sub_1_0B9C
        mov    edi, dword ptr [ebp + 0x1c]
        mov    edi, dword ptr [edi + 0x90]
        mov    esi, dword ptr [ebp + 0x6c]
        mov    ebx, dword ptr [esi]
        mov    eax, dword ptr [esi + 0xc]
        test   byte ptr [esi + 7], 2
        jne    loc_1_0A73
        mov    eax, dword ptr [esi + 0x10]
loc_1_0A73:
        mov    dword ptr [esp + 8], ecx
        mov    dword ptr [esp + 0x10], eax
loc_1_0A7B:
        mov    eax, dword ptr [esp + 8]
        sub    edx, edx
        div    dword ptr [0xb884]            ; -> loc_1_B884
        shl    edx, 4
        mov    eax, dword ptr [edx + edi + 0xc]
        cmp    eax, dword ptr [esp + 4]
        jae    loc_1_0A9B
        mov    dword ptr [esp + 4], eax
        mov    dword ptr [esp], edx
loc_1_0A9B:
        cmp    dword ptr [edx + edi + 4], 0
        jne    loc_1_0AA8
        cmp    dword ptr [edx + edi], 0
        je     loc_1_0ABC
loc_1_0AA8:
        cmp    dword ptr [edx + edi + 8], ecx
        je     loc_1_0AE2
loc_1_0AAE:
        shr    dword ptr [esp + 8], 3
        dec    dword ptr [esp + 0xc]
        jne    loc_1_0A7B
        mov    edx, dword ptr [esp]
loc_1_0ABC:
        mov    dword ptr [edx + edi + 8], ecx
        mov    dword ptr [edx + edi + 4], ebx
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [edx + edi], eax
loc_1_0ACB:
        inc    dword ptr [0xb280]            ; -> loc_1_B280
        mov    eax, dword ptr [0xb280]       ; -> loc_1_B280
        mov    dword ptr [edx + edi + 0xc], eax
        add    esp, 0x14
        pop    edi
        pop    esi
        pop    edx
        pop    ebx
loc_1_0AE1:
        ret    
loc_1_0AE2:
        cmp    dword ptr [edx + edi + 4], ebx
        jne    loc_1_0AAE
        mov    eax, dword ptr [esp + 0x10]
        cmp    dword ptr [edx + edi], eax
        jne    loc_1_0AAE
        jmp    loc_1_0ACB
sub_1_0AF3:
        push   ebp
        mov    ebp, esp
        push   edi
        mov    edi, dword ptr [ebx + 0x90]
        or     edi, edi
        je     loc_1_0B7C
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0xc]
        or     ecx, eax
        je     loc_1_0B11
        or     eax, eax
        je     loc_1_0B1E
        jmp    loc_1_0B3D
loc_1_0B11:
        mov    ecx, dword ptr [0xb27c]       ; -> loc_1_B27C
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        jmp    loc_1_0B7C
loc_1_0B1E:
        mov    ecx, dword ptr [0xb884]       ; -> loc_1_B884
        mov    edx, dword ptr [ebp + 8]
        sub    eax, eax
loc_1_0B29:
        cmp    dword ptr [edi + 4], edx
        jne    loc_1_0B36
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        loop   loc_1_0B29
        jmp    loc_1_0B7C
loc_1_0B36:
        add    edi, 0x10
        loop   loc_1_0B29
        jmp    loc_1_0B7C
loc_1_0B3D:
        mov    ecx, dword ptr [0xb884]       ; -> loc_1_B884
        push   ebx
        push   esi
        mov    ebx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp + 0x14]
        mov    esi, dword ptr [ebp + 0x10]
loc_1_0B4E:
        cmp    dword ptr [edi], eax
        je     loc_1_0B5D
        cmp    dword ptr [edi], edx
        je     loc_1_0B73
loc_1_0B56:
        add    edi, 0x10
        loop   loc_1_0B4E
        jmp    loc_1_0B7A
loc_1_0B5D:
        cmp    dword ptr [edi + 4], ebx
        jne    loc_1_0B56
loc_1_0B62:
        and    dword ptr [edi + 4], 0
        and    dword ptr [edi], 0
        and    dword ptr [edi + 8], 0
        and    dword ptr [edi + 0xc], 0
        jmp    loc_1_0B56
loc_1_0B73:
        cmp    dword ptr [edi + 4], esi
        jne    loc_1_0B56
        jmp    loc_1_0B62
loc_1_0B7A:
        pop    esi
        pop    ebx
loc_1_0B7C:
        pop    edi
        leave  
        ret    0x10
sub_1_0B81:
        mov    eax, eax
loc_1_0B84:
        lodsw  ax, word ptr [esi]
        xor    al, cl
        movzx  eax, al
        shr    ecx, 8
        xor    ecx, dword ptr [eax*4 + 0xb28c] ; -> loc_1_B28C
        cmp    word ptr [esi], 0
        jne    loc_1_0B84
        ret    
sub_1_0B9C:
        mov    edx, dword ptr [0xb288]       ; -> loc_1_B288
        mov    edi, dword ptr [0xb284]       ; -> loc_1_B284
        sub    eax, eax
        mov    eax, eax
loc_1_0BAC:
        lodsw  ax, word ptr [esi]
        cmp    ax, 0x586
        ja     loc_1_0BBC
        movzx  ebx, byte ptr [eax + edx]
        sub    ax, word ptr [ebx + edi]
loc_1_0BBC:
        xor    al, cl
        movzx  eax, al
        shr    ecx, 8
        xor    ecx, dword ptr [eax*4 + 0xb28c] ; -> loc_1_B28C
        cmp    word ptr [esi], 0
        jne    loc_1_0BAC
        ret    
        db  000h, 000h
sub_1_0BD4:
        mov    eax, dword ptr [ebp + 0x4c]
sub_1_0BD7:
        mov    edi, dword ptr [ebp + 0x1c]
        or     eax, eax
        je     loc_1_0BE7
        or     ax, ax
        je     loc_1_0C77
loc_1_0BE7:
        mov    dword ptr [ebp + 0x4c], eax
        mov    cl, byte ptr [edi + 0x4c]
        sub    cl, 5
        shr    eax, cl
        sub    edx, edx
        cmp    dword ptr [ebp + 0x50], edx
        jne    loc_1_0C03
        mov    ebx, dword ptr [edi + 0x34]
        add    ebx, eax
        mov    byte ptr [ebp + 0x60], al
        jmp    sub_1_0C48
loc_1_0C03:
        mov    dl, al
        and    dl, byte ptr [edi + 0x24]
        mov    cl, byte ptr [edi + 0x25]
        shr    eax, cl
loc_1_0C0D:
        mov    byte ptr [ebp + 0x60], dl
        mov    ebx, dword ptr [ebp + 0x54]
        cmp    eax, dword ptr [ebp + 0x58]
        je     loc_1_0C41
        ja     loc_1_0C29
        mov    ebx, dword ptr [ebp + 0x50]
        mov    dword ptr [ebp + 0x58], 0
        mov    dword ptr [ebp + 0x54], ebx
        jmp    loc_1_0C0D
loc_1_0C29:
        sub    eax, dword ptr [ebp + 0x58]
        add    dword ptr [ebp + 0x58], eax
        xchg   ecx, eax
loc_1_0C30:
        push   ecx
        call   sub_1_1F2C
        pop    ecx
        jb     loc_1_0C6D
        loop   loc_1_0C30
        mov    dword ptr [ebp + 0x54], ebx
        mov    dl, byte ptr [ebp + 0x60]
loc_1_0C41:
        call   sub_1_7E5E
        jb     loc_1_0C75
sub_1_0C48:
        mov    dword ptr [ebp + 0x5c], ebx
        call   sub_1_00D0
        jb     loc_1_0C75
        mov    dword ptr [ebp + 0x64], esi
        movzx  edx, word ptr [edi + 0x20]
        add    edx, ebx
        mov    eax, dword ptr [ebp + 0x4c]
        shl    eax, 5
        and    ax, word ptr [edi + 0x22]
        and    eax, 0xffff
        add    ebx, eax
        ret    
loc_1_0C6D:
        mov    ebx, dword ptr [ebp + 0x50]
        call   sub_1_1201
loc_1_0C75:
        stc    
        ret    
loc_1_0C77:
        pushal 
        mov    ebx, dword ptr [ebp + 0x50]
        or     ebx, ebx
        je     loc_1_0C84
        call   sub_1_2129
loc_1_0C84:
        popal  
        jae    loc_1_0BE7
        jmp    loc_1_0C6D
sub_1_0C8D:
        mov    esi, dword ptr [ebp + 0x64]
        xor    ecx, ecx
        test   byte ptr [edi + 0x52], 3
        jne    loc_1_0C9B
        mov    ecx, dword ptr [ebx + 0x12]
loc_1_0C9B:
        mov    cx, word ptr [ebx + 0x1a]
        cmp    ecx, 2
        jb     loc_1_0CDC
        cmp    dword ptr [edi + 0x2c], ecx
        jb     loc_1_0CDC
        VxDCall  Dev_048B.Service_0007h          ; cd2007008b04
        mov    edx, dword ptr [ebp + 0x20]
        mov    edx, dword ptr [edx + 8]
        sub    eax, eax
        mov    dword ptr [edx + 0x10], eax
        mov    dword ptr [edx + 0x30], eax
        mov    dword ptr [edx + 0x50], eax
        mov    eax, dword ptr [edx + 0x54]
        sub    eax, 8
        mov    dword ptr [edx + 0x5c], eax
        push   ebx
        mov    ebx, ecx
        call   sub_1_7D50
        pop    ebx
        mov    esi, dword ptr [ebp + 0x64]
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
        ret    
loc_1_0CDC:
        clc    
        ret    
sub_1_0CDE:
        inc    dword ptr [ebp + 0x4c]
        cmp    word ptr [ebp + 0x4c], 0
        je     loc_1_0D3C
loc_1_0CE8:
        add    ebx, 0x20
        cmp    ebx, edx
        jb     loc_1_0D58
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0x1c]
        inc    byte ptr [ebp + 0x60]
        mov    al, byte ptr [ebp + 0x60]
        sub    al, byte ptr [ebp + 0x61]
        je     loc_1_0D0C
        mov    ebx, dword ptr [ebp + 0x5c]
        inc    ebx
loc_1_0D03:
        call   sub_1_0C48
        jae    loc_1_0D56
        jmp    loc_1_0D39
loc_1_0D0C:
        mov    byte ptr [ebp + 0x60], al
        mov    ebx, dword ptr [ebp + 0x54]
        test   ebx, ebx
        je     loc_1_0D39
        dec    dword ptr [ebp + 0x4c]
        call   sub_1_1F2C
        jb     loc_1_0D39
        call   sub_1_2102
        jae    loc_1_0D51
        inc    dword ptr [ebp + 0x4c]
        mov    dword ptr [ebp + 0x54], ebx
        inc    dword ptr [ebp + 0x58]
        sub    edx, edx
        call   sub_1_7E5E
        jae    loc_1_0D03
loc_1_0D39:
        pop    edi
        pop    esi
        ret    
loc_1_0D3C:
        pushal 
        mov    ebx, dword ptr [ebp + 0x54]
        or     ebx, ebx
        je     loc_1_0D4C
        mov    edi, dword ptr [ebp + 0x1c]
        call   sub_1_2129
loc_1_0D4C:
        popal  
        jae    loc_1_0CE8
        jmp    loc_1_0D53
loc_1_0D51:
        pop    edi
        pop    esi
loc_1_0D53:
        cmp    eax, eax
        ret    
loc_1_0D56:
        pop    edi
        pop    esi
loc_1_0D58:
        test   esp, esp
        ret    
sub_1_0D5B:
        jb     loc_1_0E01
        test   byte ptr [ebp + 0x44], 8
        je     loc_1_0D71
        test   byte ptr [ebp + 0x42], 0x40
        jne    loc_1_10B5
loc_1_0D71:
        mov    edi, dword ptr [ebp + 0x1c]
        pushfd 
        cmp    dword ptr [ebp + 0x64], 0
        je     loc_1_0D86
        push   esi
        mov    esi, dword ptr [ebp + 0x64]
        VxDCall  Dev_048B.Service_0007h          ; cd2007008b04
        pop    esi
loc_1_0D86:
        popfd  
        push   ebx
        push   edx
        call   sub_1_0C8D
        pop    edx
        pop    ebx
        pushfd 
        push   esi
        mov    esi, dword ptr [ebp + 0x64]
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
        pop    esi
        popfd  
        jb     loc_1_10B5
        mov    al, byte ptr [ebp + 0x44]
        mov    byte ptr [ebx + 0xb], al
        push   ebx
        push   edx
        lea    edi, [ebx + 0xd]
        pushfd 
        cmp    dword ptr [ebp + 0x64], 0
        je     loc_1_0DC0
        push   esi
        mov    esi, dword ptr [ebp + 0x64]
        VxDCall  Dev_048B.Service_0007h          ; cd2007008b04
        pop    esi
loc_1_0DC0:
        popfd  
        stc    
        call   sub_1_12EA
        pushfd 
        push   esi
        mov    esi, dword ptr [ebp + 0x64]
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
        pop    esi
        popfd  
        pop    edx
        pop    ebx
        sub    eax, eax
        mov    dword ptr [ebx + 0x1c], eax
        bt     dword ptr [ebp + 0x44], 4
        sbb    eax, eax
        mov    word ptr [ebx + 0x1a], ax
        mov    word ptr [ebx + 0x14], ax
        mov    esi, dword ptr [ebp + 0x64]
        push   ebx
        push   edx
        call   sub_1_03EC
        pop    edx
        pop    ebx
        mov    eax, 0
        mov    ecx, 0
        ret    
loc_1_0E01:
        sub    esp, 0x1c
        sub    eax, eax
        mov    dword ptr [esp + 0x18], eax
        mov    dword ptr [esp + 4], eax
        mov    dword ptr [esp + 0x10], eax
        mov    dword ptr [esp + 0x14], eax
        mov    dword ptr [esp], ecx
        mov    byte ptr [esp + 0x1b], al
        mov    esi, dword ptr [ebp + 0x6c]
        mov    al, 1
        test   byte ptr [ebp + 0x42], 0x20
        je     loc_1_0E37
        mov    al, byte ptr [esi + 9]
        cmp    al, 2
        ja     loc_1_0E37
        test   byte ptr [esi + 8], 0x3f
        jne    loc_1_0E37
        mov    al, 1
loc_1_0E37:
        mov    byte ptr [esp + 0x18], al
        mov    dword ptr [esp + 0x14], eax
        mov    byte ptr [esp + 0x19], al
        mov    byte ptr [esi + 9], al
        movzx  ecx, al
        mov    byte ptr [esp + 0x1a], 0
        cmp    al, 1
        jne    loc_1_0E61
        mov    eax, dword ptr [esi + 0x18]
        cmp    eax, -1
        je     loc_1_0E75
        mov    byte ptr [esp + 0x1a], 1
        jmp    loc_1_0EB7
loc_1_0E61:
        mov    eax, dword ptr [esi + 0x14]
        cmp    eax, -1
        je     loc_1_0E75
        cmp    cl, byte ptr [esi + 0xb]
        ja     loc_1_0E75
        mov    byte ptr [esp + 0x1a], 1
        jmp    loc_1_0EB7
loc_1_0E75:
        cmp    dword ptr [ebp + 0x50], 0
        jne    loc_1_0E9D
        mov    eax, dword ptr [esi + 0x1c]
        cmp    eax, -1
        je     loc_1_10B2
        add    ecx, eax
        mov    edi, dword ptr [ebp + 0x1c]
        cmp    cx, word ptr [edi + 0x44]
        ja     loc_1_10B2
        mov    byte ptr [esp + 0x1a], 1
        jmp    loc_1_0EB7
loc_1_0E9D:
        inc    byte ptr [esp + 0x18]
        mov    eax, dword ptr [esi + 0x1c]
        cmp    eax, -1
        jne    loc_1_0EB7
        mov    edi, dword ptr [ebp + 0x1c]
        call   sub_1_10C1
        jb     loc_1_10A5
loc_1_0EB7:
        mov    dword ptr [esp + 0x10], eax
        mov    dword ptr [ebp + 0x4c], eax
        movzx  ecx, byte ptr [esp + 0x19]
        lea    ecx, [ecx + eax - 1]
        mov    dword ptr [esp + 0xc], ecx
        call   sub_1_0BD7
        jb     loc_1_10A5
        mov    eax, edx
        sub    eax, ebx
        shr    eax, 5
        cmp    byte ptr [esp + 0x18], al
        ja     loc_1_0F5C
        mov    ecx, dword ptr [esp]
        pushfd 
        cmp    dword ptr [ebp + 0x64], 0
        je     loc_1_0EFC
        push   esi
        mov    esi, dword ptr [ebp + 0x64]
        VxDCall  Dev_048B.Service_0007h          ; cd2007008b04
        pop    esi
        inc    byte ptr [esp + 0x1f]
loc_1_0EFC:
        popfd  
        push   ebx
        call   sub_1_8F7F
        pop    ebx
        pushfd 
        cmp    byte ptr [esp + 0x1f], 0
        je     loc_1_0F1B
        push   esi
        mov    esi, dword ptr [ebp + 0x64]
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
        pop    esi
        dec    byte ptr [esp + 0x1f]
loc_1_0F1B:
        popfd  
        cmp    byte ptr [esp + 0x1a], 0
        jne    loc_1_0F47
        movzx  ecx, byte ptr [esp + 0x18]
        dec    ecx
        shl    ecx, 5
        add    ecx, ebx
        xor    eax, eax
        mov    dword ptr [ecx], eax
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [ecx + 8], eax
        mov    dword ptr [ecx + 0xc], eax
        mov    dword ptr [ecx + 0x10], eax
        mov    dword ptr [ecx + 0x14], eax
        mov    dword ptr [ecx + 0x18], eax
        mov    dword ptr [ecx + 0x1c], eax
loc_1_0F47:
        mov    esi, dword ptr [ebp + 0x64]
        push   ebx
        call   sub_1_03EC
        pop    ebx
        jb     loc_1_10A5
        jmp    loc_1_1072
loc_1_0F5C:
        movzx  eax, byte ptr [esp + 0x18]
        shl    eax, 5
        pushfd 
        cmp    dword ptr [ebp + 0x64], 0
        je     loc_1_0F76
        push   esi
        mov    esi, dword ptr [ebp + 0x64]
        VxDCall  Dev_048B.Service_0007h          ; cd2007008b04
        pop    esi
loc_1_0F76:
        popfd  
        push   eax
        push   ebx
        push   edx
        call   sub_1_A1CE
        pop    edx
        pop    ebx
        pop    ecx
        pushfd 
        push   esi
        mov    esi, dword ptr [ebp + 0x64]
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
        pop    esi
        popfd  
        jb     loc_1_10A5
        mov    dword ptr [esp + 8], eax
        mov    dword ptr [esp + 4], eax
        cmp    byte ptr [esp + 0x1a], 0
        jne    loc_1_0FC1
        lea    ecx, [ecx + eax - 0x20]
        xor    eax, eax
        mov    dword ptr [ecx], eax
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [ecx + 8], eax
        mov    dword ptr [ecx + 0xc], eax
        mov    dword ptr [ecx + 0x10], eax
        mov    dword ptr [ecx + 0x14], eax
        mov    dword ptr [ecx + 0x18], eax
        mov    dword ptr [ecx + 0x1c], eax
loc_1_0FC1:
        mov    ecx, dword ptr [esp]
        pushfd 
        cmp    dword ptr [ebp + 0x64], 0
        je     loc_1_0FDA
        push   esi
        mov    esi, dword ptr [ebp + 0x64]
        VxDCall  Dev_048B.Service_0007h          ; cd2007008b04
        pop    esi
        inc    byte ptr [esp + 0x1f]
loc_1_0FDA:
        popfd  
        push   ebx
        push   edx
        mov    ebx, dword ptr [esp + 0xc]
        call   sub_1_8F7F
        pop    edx
        pop    ebx
        pushfd 
        cmp    byte ptr [esp + 0x1f], 0
        je     loc_1_0FFF
        push   esi
        mov    esi, dword ptr [ebp + 0x64]
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
        pop    esi
        dec    byte ptr [esp + 0x1f]
loc_1_0FFF:
        popfd  
loc_1_1000:
        movzx  ecx, byte ptr [esp + 0x18]
        sub    edx, ebx
        shr    edx, 5
        sub    ecx, edx
        sbb    eax, eax
        and    ecx, eax
        add    ecx, edx
        mov    byte ptr [esp + 0x19], cl
        sub    byte ptr [esp + 0x18], cl
        mov    esi, dword ptr [esp + 8]
        mov    edi, ebx
        shl    ecx, 3
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    dword ptr [esp + 8], esi
        mov    esi, dword ptr [ebp + 0x64]
        push   ebx
        call   sub_1_03EC
        pop    ebx
        cmp    byte ptr [esp + 0x18], 0
        je     loc_1_1072
        movzx  eax, byte ptr [esp + 0x19]
        add    eax, dword ptr [ebp + 0x4c]
        cmp    byte ptr [esp + 0x1a], 0
        jne    loc_1_1069
        mov    edi, dword ptr [ebp + 0x1c]
        mov    ecx, eax
        shl    ecx, 5
        test   word ptr [edi + 0x4a], cx
        jne    loc_1_1069
        cmp    byte ptr [esp + 0x18], 1
        je     loc_1_1072
        dec    eax
        mov    dword ptr [ebp + 0x4c], eax
        call   sub_1_10C1
        jb     loc_1_10A5
loc_1_1069:
        call   sub_1_0BD7
        jae    loc_1_1000
        jmp    loc_1_10A5
loc_1_1072:
        mov    eax, dword ptr [esp + 0xc]
        mov    edx, eax
        xchg   dword ptr [ebp + 0x4c], edx
        sub    eax, edx
        jb     loc_1_109E
        shl    eax, 5
        add    ebx, eax
loc_1_1084:
        mov    ecx, dword ptr [esp + 4]
        jecxz  loc_1_1091
        xchg   ecx, eax
        call   sub_1_A1F8
        xchg   ecx, eax
loc_1_1091:
        mov    eax, dword ptr [esp + 0x10]
        mov    ecx, dword ptr [esp + 0x14]
        add    esp, 0x1c
        clc    
        ret    
loc_1_109E:
        call   sub_1_0BD4
        jae    loc_1_1084
loc_1_10A5:
        mov    ecx, dword ptr [esp + 4]
        jecxz  loc_1_10B2
        xchg   ecx, eax
        call   sub_1_A1F8
        xchg   ecx, eax
loc_1_10B2:
        add    esp, 0x1c
loc_1_10B5:
        mov    eax, 0
        mov    ecx, 0
        stc    
        ret    
sub_1_10C1:
        mov    ebx, dword ptr [ebp + 0x50]
        and    ebx, ebx
        je     loc_1_10F8
        xor    eax, eax
loc_1_10CA:
        inc    eax
        push   ebx
        push   eax
        call   sub_1_1F2C
        pop    eax
        pop    ecx
        jbe    loc_1_1187
        call   sub_1_2102
        jb     loc_1_10CA
        mov    ebx, ecx
        mov    cl, byte ptr [edi + 0x47]
        mov    edx, 0x10000
        sub    cl, 5
        shr    edx, cl
        cmp    eax, edx
        jae    loc_1_1187
loc_1_10F8:
        mov    ecx, 1
        push   ebx
        xor    esi, esi
        call   sub_1_7816
        jb     loc_1_1166
        cmp    dword ptr [ebp + 0x50], 0
        jne    loc_1_111B
        push   ebx
        call   sub_1_1201
        pop    ebx
        mov    dword ptr [ebp + 0x4c], 0xffffffff
loc_1_111B:
        xor    dl, dl
        push   ebx
        call   sub_1_7E5E
        pop    edx
        jb     loc_1_116B
        movzx  ecx, byte ptr [edi + 0x24]
        test   byte ptr [edi + 0x52], 0x10
        je     loc_1_1149
        movzx  eax, byte ptr [edi + 0xac]
        bt     dword ptr [0xb868], eax       ; -> obj1:0xB868
        jb     loc_1_1149
        call   sub_1_118F
        jb     loc_1_1149
        xor    ecx, ecx
loc_1_1149:
        inc    ecx
loc_1_114A:
        push   ecx
        push   edx
        push   ebx
        call   sub_1_02F8
        jb     loc_1_1168
        call   sub_1_03EC
        jb     loc_1_1168
        pop    ebx
        pop    edx
        pop    ecx
        inc    ebx
        loop   loc_1_114A
        mov    eax, dword ptr [ebp + 0x4c]
        inc    eax
        clc    
loc_1_1166:
        pop    ecx
        ret    
loc_1_1168:
        pop    eax
        pop    edx
        pop    ecx
loc_1_116B:
        mov    edi, dword ptr [ebp + 0x1c]
        cmp    byte ptr [ebp + 0x34], 0x2b
        jne    loc_1_1178
        mov    byte ptr [ebp + 0x34], 0
loc_1_1178:
        pop    ebx
        or     ebx, ebx
        je     loc_1_1189
        mov    edx, 0xffffffff
loc_1_1182:
        call   sub_1_7D52
loc_1_1187:
        stc    
        ret    
loc_1_1189:
        mov    ebx, edx
        xor    edx, edx
        jmp    loc_1_1182
sub_1_118F:
        pushal 
        mov    cx, 0xdb08
        push   ebx
        mov    ebx, edi
        call   sub_1_449E
        pop    ebx
        jb     loc_1_11BB
        movzx  edx, byte ptr [edi + 0x4d]
        movzx  ecx, word ptr [edi + 0x4a]
        inc    ecx
        shr    ecx, 0xc
        stc    
        je     loc_1_11BB
loc_1_11AE:
        call   sub_1_00B0
        cmc    
        jb     loc_1_11BB
        add    ebx, edx
        loop   loc_1_11AE
        clc    
loc_1_11BB:
        popal  
        ret    
sub_1_11BD:
        mov    edi, dword ptr [ebp + 0x1c]
        test   byte ptr [edi + 0x52], 3
        je     loc_1_11DD
        xor    eax, eax
        mov    dword ptr [ebp + 0x50], eax
        mov    byte ptr [ebp + 0x60], al
        mov    eax, dword ptr [edi + 0x28]
        mov    edx, dword ptr [edi + 0x34]
        sub    eax, edx
        mov    byte ptr [ebp + 0x61], al
        mov    dword ptr [ebp + 0x5c], edx
loc_1_11DC:
        ret    
loc_1_11DD:
        push   ebx
        push   ecx
        push   esi
        mov    ebx, dword ptr [edi + 0x3c]
        call   sub_1_1201
        pop    esi
        pop    ecx
        pop    ebx
        jmp    loc_1_11DC
sub_1_11ED:
        mov    edi, dword ptr [ebp + 0x1c]
        mov    edx, dword ptr [ebx + 0x12]
        test   byte ptr [edi + 0x52], 3
        je     loc_1_11FB
        xor    edx, edx
loc_1_11FB:
        mov    dx, word ptr [ebx + 0x1a]
        mov    ebx, edx
sub_1_1201:
        or     ebx, ebx
        je     sub_1_11BD
        mov    dword ptr [ebp + 0x50], ebx
        mov    dword ptr [ebp + 0x54], ebx
        sub    edx, edx
        mov    byte ptr [ebp + 0x60], dl
        mov    dword ptr [ebp + 0x58], edx
        mov    edi, dword ptr [ebp + 0x1c]
        mov    al, byte ptr [edi + 0x24]
        inc    eax
        mov    byte ptr [ebp + 0x61], al
        call   sub_1_7E5E
        mov    dword ptr [ebp + 0x5c], ebx
        ret    
sub_1_1226:
        call   sub_1_9F05
        movzx  eax, word ptr [edx + 0xe]
        and    eax, 0x204
        cmp    eax, 0x204
        je     loc_1_12BF
        push   edx
        call   sub_1_12CB
        pop    edx
        mov    al, 5
        jb     loc_1_12C9
        add    esi, 0x45
        mov    ecx, 0xb
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        jne    loc_1_12C0
        sub    esi, 0x50
        sub    edi, 0xb
        test   byte ptr [edx + 0xe], 8
        jne    loc_1_1266
        or     byte ptr [esi + 0x43], 0x20
loc_1_1266:
        mov    al, byte ptr [esi + 0x43]
        mov    byte ptr [edi + 0xb], al
        mov    eax, dword ptr [esi + 0x38]
        mov    dword ptr [edi + 0x16], eax
        mov    ecx, dword ptr [ebp + 0x1c]
        bt     word ptr [ecx + 0x52], 8
        cmc    
        sbb    ecx, ecx
        mov    eax, ecx
        and    ax, word ptr [esi + 0x3c]
        mov    word ptr [edi + 0x12], ax
        mov    eax, ecx
        and    eax, dword ptr [esi + 0x3e]
        mov    dword ptr [edi + 0xe], eax
        mov    eax, ecx
        and    al, byte ptr [esi + 0x42]
        mov    byte ptr [edi + 0xd], al
        mov    eax, dword ptr [esi + 0x10]
        mov    word ptr [edi + 0x1a], ax
        shr    eax, 0x10
        mov    word ptr [edi + 0x14], ax
        mov    eax, dword ptr [esi + 0x30]
        mov    dword ptr [edi + 0x1c], eax
        push   edx
        mov    esi, ebx
        mov    ebx, edi
        call   sub_1_03EC
        pop    edx
        jb     loc_1_12BF
        or     word ptr [edx + 0xe], 0x204
loc_1_12BF:
        ret    
loc_1_12C0:
        sub    eax, eax
        call   sub_1_05C6
        mov    al, 2
loc_1_12C9:
        stc    
        ret    
sub_1_12CB:
        mov    ebx, dword ptr [esi + 0x1c]
        call   sub_1_00D0
        jb     loc_1_12E9
        mov    ebx, esi
        mov    esi, dword ptr [ebp + 0x20]
        mov    esi, dword ptr [esi + 8]
        mov    edi, dword ptr [ebx + 0x10]
        movzx  eax, byte ptr [esi + 0x25]
        shl    eax, 5
        add    edi, eax
loc_1_12E9:
        ret    
sub_1_12EA:
        lea    esp, [esp - 0xc]
        mov    eax, esp
        pushfd 
        VxDCall  IFSMgr.Service_0077h            ; cd2077004000
        sub    eax, eax
        popfd  
        rcl    eax, 1
        mov    edx, dword ptr [ebp + 0x1c]
        bt     word ptr [edx + 0x52], 8
        rcl    eax, 1
        bt     word ptr [ebp + 0x42], 0xc
        rcl    eax, 1
        lea    edi, [edi - 0xd]
        jmp    dword ptr [eax*4 + 0x1319]    ; -> obj1:0x1319
        ; ---- dispatch table (8 entries) ----
        dd  loc_1_1339
        dd  loc_1_134E
        dd  loc_1_136A
        dd  loc_1_136A
        dd  loc_1_138E
        dd  loc_1_138E
        dd  loc_1_1378
        dd  loc_1_1378
loc_1_1339:
        mov    al, byte ptr [esp + 8]
        mov    byte ptr [edi + 0xd], al
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [edi + 0xe], eax
        mov    edx, dword ptr [esp]
        mov    eax, edx
        jmp    loc_1_13A4
loc_1_134E:
        mov    eax, dword ptr [ebp + 0x14]
        mov    edx, dword ptr [eax + 0xc]
        mov    eax, dword ptr [eax + 8]
        call   sub_1_141D
        mov    byte ptr [edi + 0xd], al
        mov    dword ptr [edi + 0xe], edx
        mov    edx, dword ptr [esp + 4]
        mov    eax, edx
        jmp    loc_1_13A4
loc_1_136A:
        sub    eax, eax
        mov    byte ptr [edi + 0xd], al
        mov    dword ptr [edi + 0xe], eax
        mov    edx, dword ptr [esp + 4]
        jmp    loc_1_13A4
loc_1_1378:
        sub    eax, eax
        mov    edx, dword ptr [esp + 4]
        cmp    edx, dword ptr [edi + 0xe]
        jne    loc_1_13A4
        cmp    byte ptr [edi + 0xd], 0
        je     loc_1_13A4
        mov    edx, dword ptr [esp]
        jmp    loc_1_13A4
loc_1_138E:
        mov    edx, dword ptr [esp + 4]
        mov    eax, edx
        cmp    edx, dword ptr [edi + 0xe]
        jne    loc_1_13A4
        cmp    byte ptr [edi + 0xd], 0
        je     loc_1_13A4
        mov    edx, dword ptr [esp]
        mov    eax, edx
loc_1_13A4:
        mov    dword ptr [edi + 0x16], edx
        shr    eax, 0x10
        mov    dword ptr [edi + 0x12], eax
        lea    edi, [edi + 0x1a]
        lea    esp, [esp + 0xc]
        ret    
sub_1_13B5:
        test   al, 0x1f
        je     loc_1_13CD
        mov    ecx, eax
        and    ecx, 0x1e0
        je     loc_1_13CD
        cmp    ecx, 0x180
        ja     loc_1_13CD
        clc    
        ret    
loc_1_13CD:
        stc    
        mov    eax, edx
        ret    
sub_1_13D1:
        mov    ecx, eax
        and    ecx, 0x1f
        cmp    ecx, 0x1d
        ja     loc_1_1419
        mov    ecx, eax
        and    ecx, 0x7e0
        cmp    ecx, 0x760
        ja     loc_1_1419
        mov    ecx, eax
        and    ecx, 0xf800
        cmp    ecx, 0xb800
        ja     loc_1_1419
        mov    ecx, eax
        and    ecx, 0x1f0000
        je     loc_1_1419
        mov    ecx, eax
        and    ecx, 0x1e00000
        je     loc_1_1419
        cmp    ecx, 0x1800000
        ja     loc_1_1419
        clc    
        ret    
loc_1_1419:
        stc    
        mov    eax, edx
        ret    
sub_1_141D:
        push   0x1312d00
        push   edx
        push   eax
        mov    eax, edx
        sub    edx, edx
        div    dword ptr [esp + 8]
        mov    eax, dword ptr [esp]
        div    dword ptr [esp + 8]
        mov    dword ptr [esp + 8], 0x186a0
        mov    eax, edx
        sub    edx, edx
        div    dword ptr [esp + 8]
        mov    dword ptr [esp + 8], eax
        VxDCall  IFSMgr.IFSMgr_Win32ToDosTime    ; cd203a004000
        mov    edx, eax
        mov    eax, dword ptr [esp + 8]
        add    esp, 0xc
        ret    
        db  000h
sub_1_1458:
        mov    edx, edi
        mov    edi, ebx
        call   sub_1_A43B
        push   eax
        mov    word ptr [ebx + 4], 0
        add    edx, dword ptr [edi + 0x10]
        mov    dword ptr [ebx + 0x10], edx
        mov    dword ptr [ebx + 0x18], ecx
        mov    dword ptr [ebx + 0x1c], esi
        mov    dword ptr [ebx + 0x2c], 0
        mov    dword ptr [ebx + 0x30], 0
        mov    dword ptr [ebx + 0xc], 0x154f ; -> loc_1_154F
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf000100
        cld    
        mov    dword ptr [edi + 0x78], eax
        mov    eax, ebx
        push   ebx
        call   sub_1_A558
        inc    dword ptr [0xb68c]            ; -> obj1:0xB68C
        or     dword ptr [eax + 8], 0x20
        test   byte ptr [ebp + 0x37], 0x90
        jne    loc_1_14AF
        and    dword ptr [eax + 8], 0xffffffdf
loc_1_14AF:
        call   sub_1_199E
        pop    ebx
loc_1_14B5:
        cli    
        bt     dword ptr [ebx + 0x2c], 0
        jb     loc_1_14C7
        lea    eax, [ebx + 0x30]
        call   sub_1_A220
        jmp    loc_1_14B5
loc_1_14C7:
        sti    
        cmp    word ptr [ebx + 6], 0x10
        cmc    
        jae    loc_1_14D6
        mov    al, byte ptr [ebx + 6]
        mov    byte ptr [ebp + 0x34], al
loc_1_14D6:
        pop    ecx
        pushfd 
        call   sub_1_A4AE
        jecxz  loc_1_14E7
        btr    dword ptr [0xb8f4], 0         ; -> loc_1_B8F4
loc_1_14E7:
        popfd  
        ret    
        db  02Eh, 08Bh, 0C0h
sub_1_14EC:
        bts    word ptr [ebx + 0x1f], 3
        jb     loc_1_1540
        push   edx
        mov    dx, 9
        jmp    loc_1_1508
sub_1_14FB:
        btr    word ptr [ebx + 0x1f], 3
        jae    loc_1_1540
        push   edx
        mov    dx, 0xa
loc_1_1508:
        mov    eax, dword ptr [ebx + 0x14]
        test   byte ptr [eax + 2], 0x80
        jne    loc_1_153F
        push   ecx
        push   esi
        push   edi
        mov    edi, ebx
        xor    ah, ah
        call   sub_1_A3AF
        jb     loc_1_1541
        or     dword ptr [ebx + 8], 8
        mov    word ptr [ebx + 4], dx
        mov    dword ptr [ebx + 0x18], 0
        mov    dword ptr [ebx + 0xc], 0x1548 ; -> loc_1_1548
        mov    eax, ebx
        call   sub_1_199E
loc_1_153C:
        pop    edi
        pop    esi
        pop    ecx
loc_1_153F:
        pop    edx
loc_1_1540:
        ret    
loc_1_1541:
        xor    word ptr [ebx + 0x1f], 3
        jmp    loc_1_153C
loc_1_1548:
        mov    ebx, esi
        jmp    sub_1_A4AE
loc_1_154F:
        cld    
        dec    dword ptr [0xb68c]            ; -> obj1:0xB68C
        xchg   esi, ebx
        cmp    word ptr [ebx + 6], 0x10
        jae    loc_1_156B
loc_1_155F:
        or     dword ptr [ebx + 0x2c], 1
        lea    eax, [ebx + 0x30]
        jmp    sub_1_A258
loc_1_156B:
        or     dword ptr [ebx + 0x2c], 3
        jmp    loc_1_155F
        db  02Eh, 08Bh, 0C0h
sub_1_1574:
        test   byte ptr [ebp + 0x37], 0x10
        jne    loc_1_157F
        call   sub_1_897F
loc_1_157F:
        mov    edx, ebx
        call   sub_1_A43B
        push   eax
        mov    word ptr [ebx + 4], 1
        add    edx, dword ptr [edi + 0x10]
        mov    dword ptr [ebx + 0x10], edx
        mov    dword ptr [ebx + 0x18], ecx
        mov    dword ptr [ebx + 0x1c], esi
        mov    dword ptr [ebx + 0x2c], 0
        mov    dword ptr [ebx + 0x30], 0
        mov    dword ptr [ebx + 0xc], 0x154f ; -> loc_1_154F
        bt     dword ptr [ebp + 0x38], 0xb
        sbb    eax, eax
        and    eax, 0x1000000
        or     dword ptr [ebx + 8], eax
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf000100
        cld    
        mov    dword ptr [edi + 0x78], eax
        mov    eax, ebx
        push   ebx
        push   edi
        call   sub_1_A558
        inc    dword ptr [0xb68c]            ; -> obj1:0xB68C
        or     dword ptr [eax + 8], 0x20
        test   byte ptr [ebp + 0x37], 0x90
        jne    loc_1_15E4
        and    dword ptr [eax + 8], 0xffffffdf
loc_1_15E4:
        call   sub_1_199E
        pop    edi
        pop    ebx
        cli    
        test   byte ptr [ebx + 0x2c], 1
        jne    loc_1_15FA
        lea    eax, [ebx + 0x30]
        call   sub_1_A220
loc_1_15FA:
        sti    
        bt     dword ptr [ebx + 0x2c], 1
        jae    loc_1_1608
        mov    al, byte ptr [ebx + 6]
        mov    byte ptr [ebp + 0x34], al
loc_1_1608:
        pop    ecx
        pushfd 
        call   sub_1_A4AE
        jecxz  loc_1_1619
        btr    dword ptr [0xb8f4], 0         ; -> loc_1_B8F4
loc_1_1619:
        test   byte ptr [ebp + 0x37], 0x10
        jne    loc_1_1624
        call   sub_1_899F
loc_1_1624:
        popfd  
        ret    
        db  08Bh, 0C0h
sub_1_1628:
        mov    byte ptr [esi + 0x24], 0xff
        push   esi
        VxDCall  Dev_048B.Service_0007h          ; cd2007008b04
        call   sub_1_A43B
        pop    esi
        jae    loc_1_165E
        cmp    byte ptr [esi + 0x28], 0
        jne    loc_1_1676
        cmp    byte ptr [esi + 0x24], 0xff
        jne    loc_1_1676
        cmp    byte ptr [esi + 0x32], 0
        jne    loc_1_1676
        cmp    byte ptr [0xb1c0], 0          ; -> obj1:0xB1C0
        je     loc_1_165E
        cmp    dword ptr [0xb1c4], esi       ; -> obj1:0xB1C4
        je     loc_1_1676
loc_1_165E:
        push   ebx
        call   sub_1_1682
        pop    ebx
        clc    
loc_1_1666:
        pushfd 
        push   esi
        call   sub_1_A4AE
        pop    esi
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
        popfd  
        ret    
loc_1_1676:
        stc    
        jmp    loc_1_1666
        db  000h, 001h, 003h, 007h, 00Fh, 01Fh, 03Fh, 07Fh, 0FFh
sub_1_1682:
        mov    byte ptr [esi + 0x28], 1
        sub    eax, eax
        mov    byte ptr [esi + 0x25], al
        mov    byte ptr [esi + 0x24], al
        mov    edx, dword ptr [esi + 8]
        mov    edi, dword ptr [esi + 0xc]
        mov    ecx, esi
        mov    dword ptr [ebx + 0x2c], ecx
        mov    dword ptr [ebx + 0x10], edx
        mov    eax, dword ptr [edi + 0x10]
        add    dword ptr [ebx + 0x10], eax
        mov    dword ptr [ebx + 0xc], 0x178a ; -> loc_1_178A
        movzx  eax, byte ptr [edi + 0x4d]
        add    edx, eax
        sub    edx, dword ptr [edi + 8]
        jbe    loc_1_16B6
        sub    eax, edx
loc_1_16B6:
        mov    edx, dword ptr [ecx + 8]
        mov    dword ptr [ebx + 0x18], eax
        mov    eax, dword ptr [ecx + 0x10]
        mov    dword ptr [ebx + 0x1c], eax
        test   edx, edx
        jns    loc_1_16ED
        mov    edx, dword ptr [edi + 0xc]
        movzx  eax, byte ptr [edi + 0x4d]
        sub    eax, edx
        add    dword ptr [ebx + 0x10], eax
        sub    dword ptr [ebx + 0x18], eax
        push   ecx
        mov    ah, byte ptr [eax + 0x1679]   ; -> obj1:0x1679
        or     byte ptr [ecx + 0x24], ah
        or     byte ptr [ecx + 0x25], ah
        sub    ah, ah
        mov    cl, byte ptr [edi + 0x4c]
        shl    eax, cl
        pop    ecx
        add    dword ptr [ebx + 0x1c], eax
loc_1_16ED:
        mov    byte ptr [ecx + 0x29], 0
        or     byte ptr [ecx + 0x28], 0x80
        push   ecx
        push   ebx
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf000100
        cld    
        mov    dword ptr [edi + 0x78], eax
        xchg   ebx, eax
        call   sub_1_A558
        inc    dword ptr [0xb68c]            ; -> obj1:0xB68C
        or     dword ptr [eax + 8], 0x20
        test   byte ptr [ebp + 0x37], 0x90
        jne    loc_1_171B
        and    dword ptr [eax + 8], 0xffffffdf
loc_1_171B:
        call   sub_1_199E
        mov    ebx, dword ptr [esp + 4]
        lea    eax, [ebx + 0x28]
        mov    ecx, 0x80
        call   sub_1_A20C
        pop    ebx
        cmp    word ptr [ebx + 6], 0x10
        jae    loc_1_173B
        pop    esi
        ret    
loc_1_173B:
        pop    edx
        mov    esi, dword ptr [edx + 0x10]
        mov    ebx, dword ptr [edx + 0xc]
        movzx  ecx, byte ptr [ebx + 0x4d]
        mov    edi, dword ptr [edx + 8]
        sub    eax, eax
loc_1_174B:
        bt     dword ptr [edx + 0x25], eax
        jb     loc_1_176E
        pushal 
        push   dword ptr [ebp + 0x34]
        mov    ecx, 1
        call   sub_1_1458
        pop    eax
        mov    byte ptr [ebp + 0x34], al
        popal  
        jae    loc_1_176E
        bts    dword ptr [edx + 0x25], eax
        bts    dword ptr [edx + 0x24], eax
loc_1_176E:
        movzx  ebx, word ptr [ebx + 0x20]
        add    esi, ebx
        mov    ebx, dword ptr [edx + 0xc]
        inc    eax
        inc    edi
        loop   loc_1_174B
        mov    esi, edx
        mov    byte ptr [esi + 0x28], 0
        lea    eax, [esi + 0x2c]
        call   sub_1_A258
        ret    
loc_1_178A:
        cld    
        dec    dword ptr [0xb68c]            ; -> obj1:0xB68C
        mov    ebx, dword ptr [esi + 0x2c]
        mov    cl, byte ptr [ebx + 0x29]
        cmp    word ptr [esi + 6], 0x10
        jae    loc_1_17AB
loc_1_179E:
        and    byte ptr [ebx + 0x28], cl
        xchg   esi, ecx
        lea    eax, [ebx + 0x2c]
        jmp    sub_1_A258
loc_1_17AB:
        mov    cl, 0x7f
        mov    byte ptr [ebx + 0x29], cl
        jmp    loc_1_179E
        db  08Bh, 0C0h
sub_1_17B4:
        sub    ebx, ebx
        mov    byte ptr [esi + 0x25], bl
        mov    byte ptr [esi + 0x24], bl
        mov    edx, dword ptr [esi + 8]
        mov    edi, dword ptr [esi + 0xc]
        mov    ah, 0x41
        mov    ecx, esi
        push   esi
        call   sub_1_A3AF
        pop    esi
        jb     loc_1_181D
        mov    byte ptr [esi + 0x28], 1
        VxDCall  Dev_048B.Service_0007h          ; cd2007008b04
        mov    dword ptr [ebx + 0x2c], esi
        mov    dword ptr [ebx + 0x10], edx
        mov    dword ptr [ebx + 0xc], 0x181f ; -> loc_1_181F
        movzx  eax, byte ptr [edi + 0x4d]
        add    edx, eax
        sub    edx, dword ptr [edi + 8]
        jbe    loc_1_17F3
        sub    eax, edx
loc_1_17F3:
        mov    dword ptr [ebx + 0x18], eax
        mov    eax, dword ptr [esi + 0x10]
        mov    dword ptr [ebx + 0x1c], eax
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf000100
        cld    
        mov    dword ptr [edi + 0x78], eax
        inc    dword ptr [edi + 0x64]
        mov    eax, ebx
        call   sub_1_A558
        inc    dword ptr [0xb68c]            ; -> obj1:0xB68C
        call   sub_1_199E
        clc    
        ret    
loc_1_181D:
        stc    
        ret    
loc_1_181F:
        cld    
        dec    dword ptr [0xb68c]            ; -> obj1:0xB68C
        mov    edx, esi
        mov    esi, dword ptr [edx + 0x2c]
        cmp    word ptr [edx + 6], 0x10
        jae    loc_1_186A
loc_1_1832:
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
        mov    byte ptr [esi + 0x28], 0
        lea    ebx, [edx]
        push   esi
        call   sub_1_A4AE
        pop    esi
        mov    eax, dword ptr [esi + 0xc]
        dec    dword ptr [eax + 0x64]
        jne    loc_1_185C
        add    eax, 0x8c
        cmp    dword ptr [eax], 0
        je     loc_1_185C
        call   sub_1_A258
loc_1_185C:
        lea    eax, [esi + 0x2c]
        cmp    dword ptr [eax], 0
        je     loc_1_1869
        call   sub_1_A258
loc_1_1869:
        ret    
loc_1_186A:
        mov    eax, dword ptr [esi + 0xc]
        test   byte ptr [eax + 0x1f], 0x20
        je     loc_1_188D
        mov    edi, 0x22f0                   ; -> loc_1_22F0
        mov    ecx, 4
        movzx  eax, word ptr [edx + 6]
        cld    
        repne scasb al, byte ptr es:[edi]
        mov    eax, dword ptr [esi + 0xc]
        jne    loc_1_188D
        or     byte ptr [eax + 0x1f], 0x40
loc_1_188D:
        movzx  eax, byte ptr [eax + 0x4d]
        mov    al, byte ptr [eax + 0x1679]   ; -> obj1:0x1679
        mov    byte ptr [esi + 0x24], al
        jmp    loc_1_1832
        db  0F9h, 0C3h, 0E3h, 0FCh, 050h, 051h, 052h, 056h, 057h, 08Dh, 01Ch, 0CDh, 000h, 000h, 000h, 000h
        db  08Dh, 05Ch, 08Bh, 004h, 08Bh, 07Dh, 01Ch, 0B4h, 041h, 0E8h, 0E2h, 08Ah, 000h, 000h, 05Fh, 05Eh
        db  05Ah, 059h, 058h, 072h, 0DBh, 0C7h, 043h, 00Ch
        dd  loc_1_1931               ; fixup
        db  083h, 04Bh, 008h, 002h, 066h, 0C7h, 043h, 004h, 000h, 000h, 089h, 043h, 018h, 08Bh, 045h, 01Ch
        db  003h, 050h, 010h, 089h, 053h, 010h, 08Dh, 043h, 050h, 089h, 043h, 01Ch, 089h, 04Bh, 02Ch, 08Dh
        db  044h, 0C8h, 004h, 089h, 043h, 030h, 0FCh, 02Bh, 0C0h, 051h, 057h, 08Dh, 07Bh, 050h, 0D1h, 0E1h
        db  0F3h, 0A5h, 0ABh, 05Eh, 059h, 0F3h, 0A5h, 0CDh, 020h, 0CFh, 000h, 001h, 000h, 08Bh, 07Dh, 01Ch
        db  089h, 047h, 078h, 0FFh, 047h, 064h, 0FCh, 08Bh, 0C3h, 0E8h, 042h, 08Ch, 000h, 000h, 083h, 048h
        db  008h, 020h, 0F6h, 045h, 037h, 090h, 075h, 004h, 083h, 060h, 008h, 0DFh, 0FFh, 005h
        dd  obj1:0xB68C              ; fixup
        db  0E8h, 06Fh, 000h, 000h, 000h, 0F8h, 0C3h
loc_1_1931:
        dec    dword ptr [0xb68c]            ; -> obj1:0xB68C
        cld    
        mov    ebx, esi
        mov    esi, dword ptr [ebx + 0x30]
        mov    ecx, dword ptr [ebx + 0x2c]
loc_1_1940:
        lodsd  eax, dword ptr [esi]
        cmp    word ptr [ebx + 6], 0x10
        jb     loc_1_1958
        mov    edx, dword ptr [eax + 0xc]
        movzx  edx, byte ptr [edx + 0x4d]
        mov    dl, byte ptr [edx + 0x1679]   ; -> obj1:0x1679
        mov    byte ptr [eax + 0x24], dl
loc_1_1958:
        xchg   esi, eax
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
        xchg   esi, eax
        mov    byte ptr [eax + 0x28], 0
        loop   loc_1_1940
        mov    esi, dword ptr [ebx + 0x30]
        mov    ecx, dword ptr [ebx + 0x2c]
        mov    eax, dword ptr [esi]
        mov    eax, dword ptr [eax + 0xc]
        dec    dword ptr [eax + 0x64]
        jne    loc_1_1986
        lea    eax, [eax + 0x8c]
        cmp    dword ptr [eax], 0
        je     loc_1_1986
        call   sub_1_A258
loc_1_1986:
        lodsd  eax, dword ptr [esi]
        lea    eax, [eax + 0x2c]
        cmp    dword ptr [eax], 0
        je     loc_1_1994
        call   sub_1_A258
loc_1_1994:
        loop   loc_1_1986
        lea    ebx, [ebx]
        call   sub_1_A4AE
        ret    
sub_1_199E:
        push   ebp
        mov    esi, eax
        mov    ebx, dword ptr [edi + 0x14]
        mov    al, byte ptr [edi + 0xac]
        mov    byte ptr [esi + 0x4d], al
        test   byte ptr [esi + 0xa], 8
        jne    loc_1_19B9
        mov    eax, dword ptr [ebx + 0x20]
        add    dword ptr [esi + 0x10], eax
loc_1_19B9:
        cmp    word ptr [esi + 4], 0xd
        je     loc_1_19C3
        mov    dword ptr [esi + 0x44], ebx
loc_1_19C3:
        mov    edi, dword ptr [ebx + 0x1c]
        VxDCall  BLOCKDEV.BlockDev_Send_Command  ; cd2004001000
        cld    
        pop    ebp
        ret    
        db  066h, 0C7h, 043h, 004h, 000h, 000h, 003h, 057h, 010h, 089h, 053h, 010h, 089h, 04Bh, 018h, 083h
        db  04Bh, 008h, 002h, 089h, 073h, 01Ch, 0C7h, 043h, 02Ch, 000h, 000h, 000h, 000h, 0C7h, 043h, 030h
        db  000h, 000h, 000h, 000h, 0CDh, 020h, 0CFh, 000h, 001h, 000h, 0FCh, 089h, 047h, 078h, 08Bh, 0C3h
        db  053h, 0E8h, 053h, 08Bh, 000h, 000h, 0FFh, 005h
        dd  obj1:0xB68C              ; fixup
        db  083h, 048h, 008h, 020h, 0F6h, 045h, 037h, 090h, 075h, 004h, 083h, 060h, 008h, 0DFh, 0E8h, 080h
        db  0FFh, 0FFh, 0FFh, 05Bh, 0C3h, 050h, 08Bh, 0D3h, 0E8h, 013h, 08Ah, 000h, 000h, 087h, 004h, 024h
        db  0C7h, 043h, 00Ch
        dd  loc_1_154F               ; fixup
        db  0E8h, 098h, 0FFh, 0FFh, 0FFh, 0FAh, 00Fh, 0BAh, 063h, 02Ch, 000h, 072h, 00Ah, 08Dh, 043h, 030h
        db  0E8h, 0D9h, 087h, 000h, 000h, 0EBh, 0EEh, 0FBh, 066h, 083h, 07Bh, 006h, 010h, 0F5h, 073h, 006h
        db  08Ah, 043h, 006h, 088h, 045h, 034h, 059h, 09Ch, 0E8h, 04Fh, 08Ah, 000h, 000h, 0E3h, 008h, 00Fh
        db  0BAh, 035h
        dd  loc_1_B8F4               ; fixup
        db  000h, 09Dh, 0C3h, 000h
sub_1_1A6C:
        sub    esp, 0x28
        mov    dword ptr [esp + 0xc], eax
        mov    dword ptr [esp + 8], edx
        mov    dword ptr [esp + 4], 0
        mov    dword ptr [esp], edi
        mov    word ptr [edi], 4
        mov    word ptr [edi + 2], 4
        mov    word ptr [edi + 4], 0
        mov    eax, 0x584
        call   sub_1_A1CE
        jb     loc_1_1B6C
        mov    dword ptr [esp + 4], eax
        mov    edi, eax
        mov    edx, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 0xc]
        call   sub_1_1B7E
        jb     loc_1_1B6C
loc_1_1ABC:
        mov    esi, dword ptr [esp + 4]
        mov    eax, dword ptr [esi]
        dec    eax
        mov    edx, 0xb
        mul    edx
        lea    esi, [eax + esi + 4]
        cmp    byte ptr [esi], 5
        jne    loc_1_1AD6
        mov    byte ptr [esi], 0xe5
loc_1_1AD6:
        mov    eax, 0x200020
        lea    edi, [esp + 0x10]
        mov    ecx, 0xb
        rep stosw word ptr es:[edi], ax
        lea    edi, [esp + 0x10]
        push   1
        push   8
        push   esi
        push   edi
        VxDCall  IFSMgr.BCSToUni                 ; cd2042004000
        add    esp, 0x10
        lea    esi, [esi + 8]
        lea    edi, [esp + 0x20]
        push   1
        push   3
        push   esi
        push   edi
        VxDCall  IFSMgr.BCSToUni                 ; cd2042004000
        add    esp, 0x10
        mov    edi, dword ptr [esp]
        movzx  edx, word ptr [edi]
        mov    word ptr [edx + edi], 0
        lea    edi, [edx + edi + 2]
        lea    esi, [esp + 0x10]
        push   0
        push   esi
        push   edi
        VxDCall  IFSMgr.FcbToShort               ; cd2048004000
        add    esp, 0xc
        mov    edi, dword ptr [esp]
        movzx  edx, word ptr [edi]
        mov    word ptr [edi + 2], dx
        add    eax, 2
        mov    word ptr [edx + edi], ax
        add    ax, word ptr [edi]
        mov    word ptr [edi], ax
        mov    word ptr [eax + edi], 0
        mov    edi, dword ptr [esp + 4]
        dec    dword ptr [edi]
        jne    loc_1_1ABC
        mov    ecx, dword ptr [esp + 4]
        jecxz  loc_1_1B67
        xchg   ecx, eax
        call   sub_1_A1F8
        xchg   ecx, eax
loc_1_1B67:
        add    esp, 0x28
        clc    
        ret    
loc_1_1B6C:
        mov    ecx, dword ptr [esp + 4]
        jecxz  loc_1_1B79
        xchg   ecx, eax
        call   sub_1_A1F8
        xchg   ecx, eax
loc_1_1B79:
        add    esp, 0x28
        stc    
        ret    
sub_1_1B7E:
        sub    esp, 0xc
        mov    dword ptr [esp], edi
        mov    dword ptr [esp + 4], edx
        mov    dword ptr [esp + 8], eax
        mov    dword ptr [edi], 0
        mov    ebx, edx
        call   sub_1_1201
        jb     loc_1_1BED
        mov    eax, dword ptr [esp + 8]
        call   sub_1_0BD7
        jb     loc_1_1BED
        mov    al, byte ptr [ebx]
        cmp    al, 0xe5
        je     loc_1_1BED
loc_1_1BAC:
        mov    edi, dword ptr [esp]
        cmp    dword ptr [edi], 0x80
        jae    loc_1_1BED
        mov    eax, 0xb
        mul    dword ptr [edi]
        inc    dword ptr [edi]
        lea    edi, [eax + edi + 4]
        lea    esi, [ebx]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
        mov    edx, dword ptr [esp + 4]
        or     edx, edx
        je     loc_1_1BE8
        mov    eax, dword ptr [ebp + 0x1c]
        cmp    dword ptr [eax + 0x3c], edx
        je     loc_1_1BE8
        call   sub_1_1BF2
        jb     loc_1_1BED
        mov    dword ptr [esp + 4], edx
        jmp    loc_1_1BAC
loc_1_1BE8:
        add    esp, 0xc
        clc    
        ret    
loc_1_1BED:
        add    esp, 0xc
        stc    
        ret    
sub_1_1BF2:
        sub    esp, 8
        mov    ebx, dword ptr [ebp + 0x1c]
        or     edx, edx
        je     loc_1_1CA9
        cmp    dword ptr [ebx + 0x3c], edx
        je     loc_1_1CA9
        mov    ebx, edx
        call   sub_1_1201
        jb     loc_1_1CA9
        sub    eax, eax
        call   sub_1_0BD7
        jb     loc_1_1CA9
        cmp    dword ptr [ebx], 0x2020202e
        jne    loc_1_1CA9
        cmp    dword ptr [ebx + 0x20], 0x20202e2e
        jne    loc_1_1CA9
        mov    eax, dword ptr [ebx + 0x12]
        mov    ax, word ptr [ebx + 0x1a]
        mov    ecx, dword ptr [ebx + 0x32]
        mov    cx, word ptr [ebx + 0x3a]
        mov    edx, dword ptr [ebp + 0x1c]
        test   byte ptr [edx + 0x52], 3
        je     loc_1_1C51
        movzx  eax, ax
        movzx  ecx, cx
loc_1_1C51:
        mov    dword ptr [esp + 4], eax
        mov    dword ptr [esp], ecx
        mov    ebx, ecx
        call   sub_1_1201
        jb     loc_1_1CA9
        sub    eax, eax
        call   sub_1_0BD7
        jb     loc_1_1CA9
loc_1_1C6A:
        mov    al, byte ptr [ebx]
        cmp    al, 0xe5
        je     loc_1_1C97
        cmp    al, 0x2e
        je     loc_1_1C97
        cmp    al, 0
        je     loc_1_1CA9
        test   byte ptr [ebx + 0xb], 0x10
        je     loc_1_1C97
        mov    eax, dword ptr [ebx + 0x12]
        mov    ax, word ptr [ebx + 0x1a]
        mov    ecx, dword ptr [ebp + 0x1c]
        test   byte ptr [ecx + 0x52], 3
        je     loc_1_1C91
        movzx  eax, ax
loc_1_1C91:
        cmp    dword ptr [esp + 4], eax
        je     loc_1_1CA0
loc_1_1C97:
        call   sub_1_0CDE
        ja     loc_1_1C6A
        jmp    loc_1_1CA9
loc_1_1CA0:
        mov    edx, dword ptr [esp]
        clc    
loc_1_1CA4:
        lea    esp, [esp + 8]
        ret    
loc_1_1CA9:
        stc    
        jmp    loc_1_1CA4
sub_1_1CAC:
        cmp    dword ptr [0xb8a8], 0         ; -> obj1:0xB8A8
        je     loc_1_1D0D
        mov    edi, dword ptr [ebp + 0x1c]
        test   word ptr [edi + 0x52], 0x2001
        jne    loc_1_1D0D
        mov    ebx, dword ptr [edi + 0x14]
        test   byte ptr [ebx + 4], 0x40
        jne    loc_1_1D0D
        test   byte ptr [ebx + 1], 0x40
        jne    loc_1_1D0D
        movzx  ebx, word ptr [edi + 0x26]
        push   eax
        push   dword ptr [ebp + 0x34]
        call   sub_1_00D0
        pop    eax
        mov    byte ptr [ebp + 0x34], al
        pop    eax
        jb     loc_1_1D0D
        add    ebx, 7
        shl    eax, 2
        test   byte ptr [edi + 0x52], 3
        je     loc_1_1CF5
        sub    ebx, 4
        shl    eax, 4
loc_1_1CF5:
        not    ah
        mov    dl, byte ptr [ebx]
        and    dl, ah
        or     dl, al
        mov    byte ptr [ebx], dl
        mov    dl, 1
        push   dword ptr [ebp + 0x34]
        call   sub_1_03F3
        pop    eax
        mov    byte ptr [ebp + 0x34], al
loc_1_1D0D:
        ret    
sub_1_1D0E:
        mov    edi, dword ptr [ebp + 0x1c]
        btr    dword ptr [edi + 0x1f], 4
        jae    loc_1_1D21
        mov    ax, 0x200
        call   sub_1_1CAC
loc_1_1D21:
        ret    
sub_1_1D22:
        mov    edi, dword ptr [ebp + 0x1c]
        bts    dword ptr [edi + 0x1f], 4
        jb     loc_1_1D36
        mov    ax, 2
        call   sub_1_1CAC
        cmc    
loc_1_1D36:
        cmc    
        ret    
sub_1_1D38:
        mov    edi, dword ptr [ebp + 0x1c]
        btr    dword ptr [edi + 0x1f], 5
        jae    loc_1_1D4B
        mov    ax, 0x100
        call   sub_1_1CAC
loc_1_1D4B:
        ret    
sub_1_1D4C:
        cmp    dword ptr [edi + 0x2c], ebx
        jb     loc_1_1D96
        cmp    ebx, 2
        jb     loc_1_1D96
        push   ebx
        push   edx
        push   edi
        call   sub_1_1D0E
        pop    edi
        pop    edx
        pop    ebx
        cmp    word ptr [edi + 0x20], 0x200
        jne    loc_1_1DA0
        test   byte ptr [edi + 0x52], 2
        je     loc_1_1DA0
        cmp    bx, -8
        jae    loc_1_1D96
        push   ebx
        push   edx
        shr    ebx, 8
        add    bx, word ptr [edi + 0x26]
        call   sub_1_01D0
        pop    edx
        pop    eax
        jb     loc_1_1D96
        movzx  eax, al
        mov    word ptr [ebx + eax*2], dx
        xor    edx, edx
        jmp    sub_1_03F3
loc_1_1D95:
        pop    eax
loc_1_1D96:
        or     dword ptr [edi + 0x40], 0xffffffff
        mov    byte ptr [ebp + 0x34], 0x14
        stc    
        ret    
loc_1_1DA0:
        call   sub_1_2102
        jae    loc_1_1D96
        push   edx
        push   ebx
        mov    eax, ebx
        test   byte ptr [edi + 0x52], 1
        je     loc_1_1DB3
        shr    eax, 1
loc_1_1DB3:
        add    eax, ebx
        test   byte ptr [edi + 0x52], 3
        jne    loc_1_1DBD
        add    eax, eax
loc_1_1DBD:
        movzx  edx, word ptr [edi + 0x22]
        mov    cl, byte ptr [edi + 0x4c]
        and    edx, eax
        shr    eax, cl
        movzx  ecx, word ptr [edi + 0x26]
        add    eax, ecx
        cmp    byte ptr [edi + 0x48], 0
        je     loc_1_1DD7
        add    eax, dword ptr [edi + 0x30]
loc_1_1DD7:
        push   eax
        push   edx
        cmp    dword ptr [0xb1c0], 0         ; -> obj1:0xB1C0
        jne    loc_1_1E50
        cmp    dword ptr [0xb694], eax       ; -> obj1:0xB694
        jne    loc_1_1E19
        mov    esi, dword ptr [0xb690]       ; -> obj1:0xB690
        or     esi, esi
        je     loc_1_1E19
        cmp    dword ptr [esi + 0xc], edi
        jne    loc_1_1E19
        mov    ecx, dword ptr [0xb698]       ; -> obj1:0xB698
        cmp    dword ptr [esi + 8], ecx
        jne    loc_1_1E19
        cmp    byte ptr [esi + 0x24], 0
        jne    loc_1_1E19
        mov    ebx, dword ptr [0xb69c]       ; -> obj1:0xB69C
        test   dword ptr [esi + 0x25], 0xff0000ff
        je     loc_1_1E57
loc_1_1E19:
        cmp    dword ptr [0xb6a4], eax       ; -> obj1:0xB6A4
        jne    loc_1_1E50
        mov    esi, dword ptr [0xb6a0]       ; -> obj1:0xB6A0
        or     esi, esi
        je     loc_1_1E50
        cmp    dword ptr [esi + 0xc], edi
        jne    loc_1_1E50
        mov    ecx, dword ptr [0xb6a8]       ; -> obj1:0xB6A8
        cmp    dword ptr [esi + 8], ecx
        jne    loc_1_1E50
        cmp    byte ptr [esi + 0x24], 0
        jne    loc_1_1E50
        mov    ebx, dword ptr [0xb6ac]       ; -> obj1:0xB6AC
        test   dword ptr [esi + 0x25], 0xff0000ff
        je     loc_1_1E57
loc_1_1E50:
        mov    ebx, eax
        call   sub_1_01D0
loc_1_1E57:
        pop    ecx
        pop    edx
        pop    eax
        jb     loc_1_1D95
        mov    dword ptr [0xb690], esi       ; -> obj1:0xB690
        mov    dword ptr [0xb694], edx       ; -> obj1:0xB694
        mov    esi, dword ptr [esi + 8]
        mov    dword ptr [0xb698], esi       ; -> obj1:0xB698
        mov    dword ptr [0xb69c], ebx       ; -> obj1:0xB69C
        mov    esi, dword ptr [0xb690]       ; -> obj1:0xB690
        test   byte ptr [edi + 0x52], 3
        je     loc_1_1E9A
        test   byte ptr [edi + 0x52], 1
        jne    loc_1_1EB1
        pop    edx
        add    ebx, ecx
        mov    word ptr [ebx], dx
loc_1_1E93:
        xor    edx, edx
        jmp    sub_1_03F3
loc_1_1E9A:
        add    ebx, ecx
        pop    edx
        mov    ecx, dword ptr [ebx]
        and    edx, 0xfffffff
        and    ecx, 0xf0000000
        or     ecx, edx
        mov    dword ptr [ebx], ecx
        jmp    loc_1_1E93
loc_1_1EB1:
        cmp    word ptr [edi + 0x22], cx
        je     loc_1_1EDA
        xchg   dword ptr [esp], esi
        and    si, 0xfff
        mov    edx, 0xf000
        add    ebx, ecx
        test   al, 1
        je     loc_1_1ED1
        rol    dx, 4
        shl    esi, 4
loc_1_1ED1:
        and    word ptr [ebx], dx
        or     word ptr [ebx], si
        pop    esi
        jmp    loc_1_1E93
loc_1_1EDA:
        add    ebx, ecx
        pop    ecx
        test   al, 1
        jne    loc_1_1F04
        mov    byte ptr [ebx], cl
        push   edx
        xor    edx, edx
        call   sub_1_03F3
        pop    ebx
        inc    ebx
        push   ecx
        call   sub_1_01D0
        pop    ecx
        jb     loc_1_1D96
        and    ch, 0xf
        and    byte ptr [ebx], 0xf0
        or     byte ptr [ebx], ch
        jmp    loc_1_1E93
loc_1_1F04:
        shl    ecx, 4
        and    byte ptr [ebx], 0xf
        or     byte ptr [ebx], cl
        push   edx
        xor    edx, edx
        call   sub_1_03F3
        pop    ebx
        inc    ebx
        push   ecx
        call   sub_1_01D0
        pop    ecx
        jb     loc_1_1D96
        mov    byte ptr [ebx], ch
        jmp    loc_1_1E93
        db  08Bh, 0C0h
sub_1_1F2C:
        cmp    ebx, 2
        jb     loc_1_1FC9
        cmp    dword ptr [edi + 0x2c], ebx
        jb     loc_1_1FC4
        cmp    word ptr [edi + 0x20], 0x200
        jne    loc_1_1FDC
        test   byte ptr [edi + 0x52], 2
        je     loc_1_1FDC
        push   ebx
        shr    ebx, 8
        add    bx, word ptr [edi + 0x26]
        cmp    dword ptr [0xb6a4], ebx       ; -> obj1:0xB6A4
        jne    loc_1_1F95
        cmp    dword ptr [0xb1c0], 0         ; -> obj1:0xB1C0
        jne    loc_1_1F95
        mov    esi, dword ptr [0xb6a0]       ; -> obj1:0xB6A0
        or     esi, esi
        je     loc_1_1F95
        cmp    edi, dword ptr [esi + 0xc]
        jne    loc_1_1F95
        mov    eax, dword ptr [0xb6a8]       ; -> obj1:0xB6A8
        cmp    eax, dword ptr [esi + 8]
        jne    loc_1_1F95
        cmp    byte ptr [esi + 0x24], 0
        jne    loc_1_1F95
        test   dword ptr [esi + 0x25], 0xff0000ff
        je     loc_1_1FCC
loc_1_1F95:
        push   ebx
        call   sub_1_01D0
        pop    ecx
        pop    eax
        jb     loc_1_1FC4
        mov    dword ptr [0xb6a0], esi       ; -> obj1:0xB6A0
        mov    dword ptr [0xb6a4], ecx       ; -> obj1:0xB6A4
        mov    dword ptr [0xb6ac], ebx       ; -> obj1:0xB6AC
        mov    ecx, dword ptr [esi + 8]
        mov    dword ptr [0xb6a8], ecx       ; -> obj1:0xB6A8
        sub    ah, ah
        movzx  ebx, word ptr [ebx + eax*2]
        and    ebx, ebx
        ret    
loc_1_1FC3:
        pop    eax
loc_1_1FC4:
        or     dword ptr [edi + 0x40], 0xffffffff
        stc    
loc_1_1FC9:
        ret    
        db  08Bh, 0C0h
loc_1_1FCC:
        mov    ebx, dword ptr [0xb6ac]       ; -> obj1:0xB6AC
        pop    eax
        sub    ah, ah
        movzx  ebx, word ptr [ebx + eax*2]
        and    ebx, ebx
        ret    
loc_1_1FDC:
        call   sub_1_2102
        jae    loc_1_1FC4
        push   ebx
        mov    eax, ebx
        test   byte ptr [edi + 0x52], 1
        je     loc_1_1FEE
        shr    eax, 1
loc_1_1FEE:
        add    eax, ebx
        test   byte ptr [edi + 0x52], 3
        jne    loc_1_1FF8
        add    eax, eax
loc_1_1FF8:
        movzx  edx, word ptr [edi + 0x22]
        mov    cl, byte ptr [edi + 0x4c]
        and    edx, eax
        shr    eax, cl
        movzx  ecx, word ptr [edi + 0x26]
        add    eax, ecx
        cmp    byte ptr [edi + 0x48], 0
        je     loc_1_2012
        add    eax, dword ptr [edi + 0x30]
loc_1_2012:
        push   eax
        push   edx
        cmp    dword ptr [0xb1c0], 0         ; -> obj1:0xB1C0
        jne    loc_1_208B
        cmp    dword ptr [0xb6a4], eax       ; -> obj1:0xB6A4
        jne    loc_1_2054
        mov    esi, dword ptr [0xb6a0]       ; -> obj1:0xB6A0
        or     esi, esi
        je     loc_1_2054
        cmp    dword ptr [esi + 0xc], edi
        jne    loc_1_2054
        mov    ecx, dword ptr [0xb6a8]       ; -> obj1:0xB6A8
        cmp    dword ptr [esi + 8], ecx
        jne    loc_1_2054
        cmp    byte ptr [esi + 0x24], 0
        jne    loc_1_2054
        mov    ebx, dword ptr [0xb6ac]       ; -> obj1:0xB6AC
        test   dword ptr [esi + 0x25], 0xff0000ff
        je     loc_1_2092
loc_1_2054:
        cmp    dword ptr [0xb694], eax       ; -> obj1:0xB694
        jne    loc_1_208B
        mov    esi, dword ptr [0xb690]       ; -> obj1:0xB690
        or     esi, esi
        je     loc_1_208B
        cmp    dword ptr [esi + 0xc], edi
        jne    loc_1_208B
        mov    ecx, dword ptr [0xb698]       ; -> obj1:0xB698
        cmp    dword ptr [esi + 8], ecx
        jne    loc_1_208B
        cmp    byte ptr [esi + 0x24], 0
        jne    loc_1_208B
        mov    ebx, dword ptr [0xb69c]       ; -> obj1:0xB69C
        test   dword ptr [esi + 0x25], 0xff0000ff
        je     loc_1_2092
loc_1_208B:
        mov    ebx, eax
        call   sub_1_01D0
loc_1_2092:
        pop    ecx
        pop    edx
        pop    eax
        jb     loc_1_1FC3
        mov    dword ptr [0xb6a0], esi       ; -> obj1:0xB6A0
        mov    dword ptr [0xb6a4], edx       ; -> obj1:0xB6A4
        mov    dword ptr [0xb6ac], ebx       ; -> obj1:0xB6AC
        mov    esi, dword ptr [esi + 8]
        mov    dword ptr [0xb6a8], esi       ; -> obj1:0xB6A8
        test   byte ptr [edi + 0x52], 3
        je     loc_1_20DD
        cmp    word ptr [edi + 0x22], cx
        je     loc_1_20E7
        mov    bx, word ptr [ecx + ebx]
loc_1_20C6:
        test   byte ptr [edi + 0x52], 2
        jne    loc_1_20D6
        test   al, 1
        je     loc_1_20D3
        shr    ebx, 4
loc_1_20D3:
        and    bh, 0xf
loc_1_20D6:
        and    ebx, 0xffff
        ret    
loc_1_20DD:
        mov    ebx, dword ptr [ecx + ebx]
        and    ebx, 0xfffffff
        ret    
loc_1_20E7:
        mov    bl, byte ptr [ecx + ebx]
        push   eax
        push   ebx
        lea    ebx, [edx + 1]
        call   sub_1_01D0
        mov    ecx, ebx
        pop    ebx
        pop    eax
        jb     loc_1_1FC3
        mov    bh, byte ptr [ecx]
        jmp    loc_1_20C6
sub_1_2102:
        test   byte ptr [edi + 0x52], 2
        je     loc_1_210D
        cmp    bx, -8
        ret    
loc_1_210D:
        test   byte ptr [edi + 0x52], 3
        jne    loc_1_211C
        push   ebx
        shl    ebx, 4
        cmp    ebx, -0x80
        pop    ebx
        ret    
loc_1_211C:
        cmp    bx, 0xff0
        je     loc_1_2128
        cmp    bx, 0xff8
loc_1_2128:
        ret    
sub_1_2129:
        mov    esi, ebx
loc_1_212B:
        push   esi
        call   sub_1_1F2C
        pop    esi
        jbe    loc_1_2163
        call   sub_1_2102
        jae    loc_1_2164
        xchg   esi, ebx
        push   esi
        call   sub_1_1F2C
        pop    esi
        jbe    loc_1_2163
        call   sub_1_2102
        jae    loc_1_2164
        push   esi
        call   sub_1_1F2C
        pop    esi
        jbe    loc_1_2163
        call   sub_1_2102
        jae    loc_1_2164
        xchg   esi, ebx
        cmp    ebx, esi
        jne    loc_1_212B
loc_1_2163:
        stc    
loc_1_2164:
        ret    
sub_1_2165:
        push   esi
        mov    esi, dword ptr [esi + 8]
loc_1_2169:
        bts    dword ptr [esi + 0x24], 4
        jb     loc_1_21C8
        mov    ecx, eax
        mov    eax, dword ptr [esi + 0x50]
        sub    ecx, eax
        jb     loc_1_21AD
        inc    ecx
        mov    ebx, dword ptr [esi + 0x10]
        mov    edx, dword ptr [esi + 0x5c]
        or     eax, eax
        je     loc_1_21D6
        mov    ebx, dword ptr [edx + 4]
        add    ebx, dword ptr [edx]
        dec    ebx
loc_1_218A:
        push   ebx
        push   ecx
        push   esi
        call   sub_1_1F2C
        pop    esi
        pop    ecx
        pop    eax
        jbe    loc_1_21C5
        call   sub_1_2102
        jae    loc_1_21C5
        inc    eax
        mov    edx, dword ptr [esi + 0x5c]
        cmp    ebx, eax
        jne    loc_1_21D6
loc_1_21A6:
        inc    dword ptr [esi + 0x50]
        inc    dword ptr [edx]
        loop   loc_1_218A
loc_1_21AD:
        clc    
loc_1_21AE:
        pushfd 
        and    byte ptr [esi + 0x24], 0xef
        mov    eax, 0xb6b0                   ; -> obj1:0xB6B0
        cmp    dword ptr [eax], 0
        je     loc_1_21C2
        call   sub_1_A258
loc_1_21C2:
        popfd  
        pop    esi
        ret    
loc_1_21C5:
        stc    
        jmp    loc_1_21AE
loc_1_21C8:
        push   eax
        mov    eax, 0xb6b0                   ; -> obj1:0xB6B0
        call   sub_1_A220
        pop    eax
        jmp    loc_1_2169
loc_1_21D6:
        add    edx, 8
        cmp    dword ptr [esi + 0x58], edx
        jbe    loc_1_21E9
        mov    dword ptr [esi + 0x5c], edx
loc_1_21E1:
        mov    dword ptr [edx + 4], ebx
        and    dword ptr [edx], 0
        jmp    loc_1_21A6
loc_1_21E9:
        push   ebx
        push   ecx
        call   sub_1_21F9
        pop    ecx
        pop    ebx
        jb     loc_1_21AE
        mov    edx, dword ptr [esi + 0x5c]
        jmp    loc_1_21E1
sub_1_21F9:
        push   eax
        push   esi
        push   edi
        mov    eax, dword ptr [esi + 0x58]
        sub    eax, dword ptr [esi + 0x54]
        add    eax, 0x40
        test   byte ptr [ebp + 0x37], 0x10
        je     loc_1_2235
        lea    ebx, [eax + 0xfff]
        shr    ebx, 0xc
        push   8
        push   ebx
        push   dword ptr [esi + 0x54]
        VMMCall  VMM._PageReAllocate             ; cd2054000100
        add    esp, 0xc
        cmp    edx, 1
        jb     loc_1_2272
        shl    ebx, 0xc
        add    ebx, eax
        xchg   dword ptr [esi + 0x58], ebx
        mov    dword ptr [esi + 0x5c], ebx
        jmp    loc_1_2271
loc_1_2235:
        push   eax
        test   byte ptr [esi + 0x24], 2
        jne    loc_1_224A
        cmp    eax, 0x800
        jb     loc_1_224A
        call   sub_1_A1E0
        jmp    loc_1_224F
loc_1_224A:
        call   sub_1_A1CE
loc_1_224F:
        pop    ecx
        jb     loc_1_2272
        add    ecx, eax
        xchg   dword ptr [esi + 0x58], ecx
        sub    ecx, dword ptr [esi + 0x54]
        mov    edi, eax
        lea    edx, [ecx + edi]
        mov    dword ptr [esi + 0x5c], edx
        xchg   dword ptr [esi + 0x54], eax
        shr    ecx, 2
        mov    esi, eax
        rep movsd dword ptr es:[edi], dword ptr [esi]
        call   sub_1_A1F8
loc_1_2271:
        clc    
loc_1_2272:
        pop    edi
        pop    esi
        pop    eax
        ret    
sub_1_2276:
        mov    eax, dword ptr [ecx + 0x54]
        sub    eax, 8
loc_1_227C:
        add    eax, 8
        sub    ebx, dword ptr [eax]
        jae    loc_1_227C
        add    ebx, dword ptr [eax]
        add    ebx, dword ptr [eax + 4]
        ret    
        db  000h, 000h, 000h
loc_1_228C:
        or     al, byte ptr [ebx]
        adc    byte ptr [edi], cl
        sub    al, 0
        add    ah, dh                        ; -> loc_1_22F4
        and    al, byte ptr [eax]
        add    byte ptr [ecx + 0x4100002e], cl ; -> loc_1_2E89
        inc    ebx
        add    byte ptr [eax], al
        nop    
        inc    ecx
        add    byte ptr [eax], al
        dd  loc_1_26FE               ; fixup
        dd  loc_1_3530               ; fixup
        dd  loc_1_32B0               ; fixup
        dd  loc_1_AB85               ; fixup
        dd  loc_1_3F50               ; fixup
        dd  loc_1_4668               ; fixup
        dd  loc_1_5F4C               ; fixup
        dd  loc_1_59F2               ; fixup
        dd  loc_1_A808               ; fixup
        dd  loc_1_51F6               ; fixup
loc_1_22CC:
        or     al, byte ptr [ebx]
        adc    byte ptr [eax], cl
        xchg   edx, eax
        push   ecx
        add    byte ptr [eax], al
        mov    byte ptr [edi], cl
        add    byte ptr [ecx], dl            ; -> loc_1_5111
        push   ecx
        add    byte ptr [eax], al
        jo     0x2324
        add    byte ptr [eax], al
        loop   0x2328
        add    byte ptr [eax], al
        push   0x68000046                    ; -> loc_1_4668
        inc    esi
        add    byte ptr [eax], al
        and    al, 0xac
        add    byte ptr [eax], al
loc_1_22F0:
        adc    al, 0x15
        sbb    bl, byte ptr [ebx]
loc_1_22F4:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 0x14]
        cld    
        sub    eax, eax
        mov    byte ptr [ebp + 0x37], al
        mov    word ptr [ebp + 0x38], 0x2000
        mov    byte ptr [ebp + 0x34], al
        mov    byte ptr [ebp + 0x35], al
        mov    dword ptr [ebp + 0x6c], eax
        mov    dword ptr [ebp + 0x64], eax
        mov    byte ptr [ebp + 0x45], 0x16
        mov    byte ptr [ebp + 0x46], 0
        cmp    byte ptr [ebp + 4], 2
        jb     loc_1_2329
        call   sub_1_A318
        jmp    loc_1_2340
loc_1_2329:
        call   sub_1_A26B
        or     word ptr [ebp + 0x38], 0x800
        call   sub_1_5B85
        jb     loc_1_2340
        call   sub_1_5CFF
loc_1_2340:
        mov    al, byte ptr [ebp + 4]
        cmp    al, 2
        je     loc_1_2384
        cmp    al, 0
        je     loc_1_23AA
        cmp    al, 1
        je     loc_1_2549
        cmp    al, 3
        je     loc_1_2362
        cmp    al, 4
        je     loc_1_2362
        mov    al, 3
        jmp    loc_1_4576
loc_1_2362:
        mov    eax, 0x820
        mov    edx, dword ptr [ebp]
        call   sub_1_43C9
        mov    eax, dword ptr [ebp + 0xc]
        call   sub_1_91B8
        jae    loc_1_4548
        mov    al, 3
        jmp    loc_1_454A
loc_1_2384:
        mov    eax, 0
        mov    edx, dword ptr [ebp]
        call   sub_1_43C9
        mov    eax, dword ptr [ebp + 0xc]
        call   sub_1_91B8
        mov    al, 3
        jb     loc_1_23A5
        je     loc_1_4548
        mov    al, 5
loc_1_23A5:
        jmp    loc_1_454A
loc_1_23AA:
        or     word ptr [ebp + 0x38], 0x8000
        sub    eax, eax
        push   eax
        push   eax
        push   eax
        push   eax
        push   eax
        mov    eax, 0x400
        mov    edx, dword ptr [ebp]
        call   sub_1_4418
        call   sub_1_43C9
        test   byte ptr [ebp + 0x42], 0x20
        je     loc_1_23DC
        mov    eax, dword ptr [ebp + 0x28]
        cmp    word ptr [eax], 0
        je     loc_1_253B
loc_1_23DC:
        mov    eax, dword ptr [ebp + 0xc]
        call   sub_1_91B8
        jae    loc_1_2533
        jne    loc_1_2537
        mov    eax, dword ptr [ebp + 0x1c]
        mov    eax, dword ptr [eax + 0x14]
        test   byte ptr [eax + 4], 0x40
        jne    loc_1_253F
        mov    byte ptr [ebp + 0x44], 0x10
        sub    ecx, ecx
        stc    
        call   sub_1_0D5B
        jb     loc_1_253B
        mov    dword ptr [esp + 8], eax
        mov    dword ptr [esp + 0x10], ecx
        mov    eax, dword ptr [ebp + 0x4c]
        mov    dword ptr [esp + 0xc], eax
        sub    eax, eax
        xchg   dword ptr [ebp + 0x50], eax
        mov    dword ptr [esp + 4], eax
        mov    edi, dword ptr [ebp + 0x1c]
        call   sub_1_10C1
        jb     loc_1_250A
        mov    ecx, dword ptr [ebp + 0x50]
        mov    dword ptr [esp], ecx
        call   sub_1_0BD7
        jb     loc_1_24FF
        mov    edi, ebx
        mov    al, 0x2e
        stosb  byte ptr es:[edi], al
        mov    al, 0x20
        mov    ecx, 0xa
        rep stosb byte ptr es:[edi], al
        mov    al, 0x10
        stosb  byte ptr es:[edi], al
        sub    eax, eax
        stosb  byte ptr es:[edi], al
        push   ebx
        pushfd 
        cmp    dword ptr [ebp + 0x64], 0
        je     loc_1_2470
        push   esi
        mov    esi, dword ptr [ebp + 0x64]
        VxDCall  Dev_048B.Service_0007h          ; cd2007008b04
        pop    esi
loc_1_2470:
        popfd  
        clc    
        call   sub_1_12EA
        pushfd 
        push   esi
        mov    esi, dword ptr [ebp + 0x64]
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
        pop    esi
        popfd  
        pop    ebx
        mov    eax, dword ptr [esp]
        mov    word ptr [ebx + 0x1a], ax
        shr    eax, 0x10
        mov    word ptr [ebx + 0x14], ax
        mov    esi, ebx
        lea    edi, [esi + 0x20]
        mov    ecx, 8
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    byte ptr [esi + 1], 0x2e
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [ebp + 0x1c]
        cmp    dword ptr [ecx + 0x3c], eax
        jne    loc_1_24B1
        xor    eax, eax
loc_1_24B1:
        mov    word ptr [esi + 0x1a], ax
        shr    eax, 0x10
        mov    word ptr [esi + 0x14], ax
        mov    esi, dword ptr [ebp + 0x64]
        call   sub_1_03EC
        jb     loc_1_24FF
        mov    eax, dword ptr [esp + 4]
        and    eax, eax
        xchg   dword ptr [ebp + 0x50], eax
        jne    loc_1_24D5
        mov    byte ptr [ebp + 0x61], 0xff
loc_1_24D5:
        mov    dword ptr [ebp + 0x58], 0xffffffff
        mov    eax, dword ptr [esp + 0xc]
        call   sub_1_0BD7
        jb     loc_1_24FF
        mov    eax, dword ptr [esp]
        mov    word ptr [ebx + 0x1a], ax
        shr    eax, 0x10
        mov    word ptr [ebx + 0x14], ax
        mov    esi, dword ptr [ebp + 0x64]
        call   sub_1_03EC
        jae    loc_1_252F
loc_1_24FF:
        mov    edi, dword ptr [ebp + 0x1c]
        mov    ebx, dword ptr [esp]
        call   sub_1_7D50
loc_1_250A:
        mov    eax, dword ptr [esp + 4]
        or     eax, eax
        jne    loc_1_2516
        mov    byte ptr [ebp + 0x61], 0xff
loc_1_2516:
        mov    dword ptr [ebp + 0x58], 0xffffffff
        mov    dword ptr [ebp + 0x50], eax
        mov    eax, dword ptr [esp + 8]
        mov    ecx, dword ptr [esp + 0x10]
        call   sub_1_9A90
        jmp    loc_1_253B
loc_1_252F:
        sub    eax, eax
        jmp    loc_1_2541
loc_1_2533:
        mov    al, 5
        jmp    loc_1_2541
loc_1_2537:
        mov    al, 3
        jmp    loc_1_2541
loc_1_253B:
        mov    al, 0x52
        jmp    loc_1_2541
loc_1_253F:
        mov    al, 0x13
loc_1_2541:
        add    esp, 0x14
        jmp    loc_1_454A
loc_1_2549:
        or     word ptr [ebp + 0x38], 0x8000
        call   sub_1_1D0E
        mov    eax, 0
        mov    edx, dword ptr [ebp]
        call   sub_1_43C9
        sub    esp, 0x20
        mov    eax, dword ptr [ebp + 0xc]
        call   sub_1_91B8
        jae    loc_1_2575
loc_1_256E:
        mov    al, 3
        jmp    loc_1_26F6
loc_1_2575:
        jne    loc_1_26F4
        inc    ebx
        je     loc_1_26F4
        dec    ebx
        call   sub_1_11ED
        jb     loc_1_261F
        sub    eax, eax
        call   sub_1_0BD7
        jb     loc_1_26F4
        cmp    dword ptr [ebx], 0x2020202e
        jne    loc_1_26F4
        inc    dword ptr [ebp + 0x4c]
        call   sub_1_0BD4
        jb     loc_1_26F4
        cmp    dword ptr [ebx], 0x20202e2e
        jne    loc_1_26F4
        lea    edi, [esp]
        test   byte ptr [ebp + 0x42], 0x20
        je     loc_1_25D8
        mov    dword ptr [edi], 0x2a0004
        mov    word ptr [edi + 4], 0
        jmp    loc_1_25F5
loc_1_25D8:
        mov    eax, 0x1a
        stosw  word ptr es:[edi], ax
        mov    eax, 0x3f
        mov    ecx, 0xc
        rep stosw word ptr es:[edi], ax
        mov    word ptr [edi], 0
        mov    byte ptr [edi - 8], 0x2e
loc_1_25F5:
        mov    eax, 0x11
        push   edx
        mov    edx, dword ptr [ebp]
        call   sub_1_43C9
        pop    edx
        lea    eax, [esp]
        mov    ecx, 1
        call   sub_1_9366
        jae    loc_1_26F4
        or     eax, eax
        jne    loc_1_26F6
loc_1_261F:
        and    word ptr [ebp + 0x42], 0xfffe
        mov    eax, dword ptr [ebp + 0x1c]
        mov    eax, dword ptr [eax + 0x14]
        test   byte ptr [eax + 4], 0x40
        mov    al, 0x13
        jne    loc_1_26F6
        mov    eax, dword ptr [ebp + 0xc]
        call   sub_1_91B8
        jb     loc_1_256E
        pushfd 
        cmp    dword ptr [ebp + 0x64], 0
        je     loc_1_2656
        push   esi
        mov    esi, dword ptr [ebp + 0x64]
        VxDCall  Dev_048B.Service_0007h          ; cd2007008b04
        pop    esi
loc_1_2656:
        popfd  
        mov    eax, dword ptr [ebx + 0x12]
        mov    ax, word ptr [ebx + 0x1a]
        push   ebx
        mov    ebx, dword ptr [ebp + 0x1c]
        test   byte ptr [ebx + 0x52], 3
        je     loc_1_266B
        movzx  eax, ax
loc_1_266B:
        push   eax
        push   eax
        call   sub_1_0979
        pop    eax
        push   0
        push   eax
        push   0
        push   eax
        call   sub_1_0AF3
        pop    eax
        call   sub_1_AB5D
        pop    ebx
        pushfd 
        push   esi
        mov    esi, dword ptr [ebp + 0x64]
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
        pop    esi
        popfd  
        mov    esi, dword ptr [ebp + 0x6c]
        test   byte ptr [esi + 7], 2
        je     loc_1_26BB
        cmp    byte ptr [esi + 4], 0
        je     loc_1_26BB
        mov    eax, dword ptr [ebx + 0x12]
        mov    ax, word ptr [ebx + 0x1a]
        push   eax
        mov    eax, dword ptr [esi + 0xc]
        movzx  ecx, byte ptr [esi + 4]
        inc    ecx
        call   sub_1_9A90
        pop    ebx
        jae    loc_1_26D1
        jmp    loc_1_26F4
loc_1_26BB:
        mov    esi, dword ptr [ebp + 0x64]
        mov    byte ptr [ebx], 0xe5
        mov    eax, dword ptr [ebx + 0x12]
        mov    ax, word ptr [ebx + 0x1a]
        push   eax
        call   sub_1_03EC
        pop    ebx
        jb     loc_1_26F4
loc_1_26D1:
        mov    edi, dword ptr [ebp + 0x1c]
        test   byte ptr [edi + 0x52], 3
        je     loc_1_26DD
        movzx  ebx, bx
loc_1_26DD:
        call   sub_1_7D50
        jb     loc_1_26F4
        mov    eax, dword ptr [ebp + 0x1c]
        call   sub_1_05C6
        add    esp, 0x20
        jmp    loc_1_4548
loc_1_26F4:
        mov    al, 5
loc_1_26F6:
        add    esp, 0x20
        jmp    loc_1_454A
loc_1_26FE:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 0x14]
        cld    
        mov    ax, word ptr [ebp + 0x18]
        and    ah, 0x91
        mov    byte ptr [ebp + 0x37], ah
        sub    eax, eax
        mov    word ptr [ebp + 0x38], 0x2000
        mov    byte ptr [ebp + 0x34], al
        mov    byte ptr [ebp + 0x35], al
        mov    dword ptr [ebp + 0x6c], eax
        mov    dword ptr [ebp + 0x64], eax
        sub    esp, 8
        mov    dword ptr [esp], 0
        mov    eax, dword ptr [ebp + 0x20]
        mov    dword ptr [esp + 4], eax
        test   byte ptr [ebp + 0x18], 0x12
        jne    loc_1_2743
        call   sub_1_A318
        jmp    loc_1_275A
loc_1_2743:
        call   sub_1_A26B
        or     word ptr [ebp + 0x38], 0x800
        call   sub_1_5B85
        jb     loc_1_275A
        call   sub_1_5CFF
loc_1_275A:
        mov    dword ptr [ebp + 0x20], 0
        mov    al, byte ptr [ebp]
        mov    byte ptr [ebp + 0x44], al
        mov    byte ptr [ebp + 0x45], 0x16
        mov    byte ptr [ebp + 0x46], 0
        movzx  eax, byte ptr [ebp + 4]
        and    al, 0x7f
        cmp    al, 0x7f
        jne    loc_1_277F
        mov    al, 0x82
        mov    byte ptr [ebp + 4], 2
loc_1_277F:
        mov    word ptr [ebp + 0x62], ax
        mov    al, byte ptr [ebp + 0x18]
        mov    byte ptr [ebp + 0x47], al
        and    al, al
        je     loc_1_291A
        mov    bl, al
        and    bl, 0xf
        cmp    bl, 2
        ja     loc_1_291A
        test   al, 0xe0
        jne    loc_1_291A
        call   sub_1_9DD4
        jb     loc_1_2906
        mov    dword ptr [ebp + 0x20], eax
        mov    esi, dword ptr [ebp + 0x10]
        mov    dword ptr [esi], 0x483a       ; -> loc_1_483A
        mov    dword ptr [esi + 4], 0x49e8   ; -> loc_1_49E8
        mov    dword ptr [esi + 8], 0x22cc   ; -> loc_1_22CC
        mov    esi, dword ptr [ebp + 0x20]
        mov    ax, word ptr [ebp + 0x62]
        mov    word ptr [esi + 0xc], ax
        test   byte ptr [ebp + 0x47], 1
        je     loc_1_27F9
        call   sub_1_291E
        mov    dword ptr [esp], eax
        jae    loc_1_2823
        cmp    al, 2
        jne    loc_1_2906
        test   byte ptr [ebp + 0x47], 0x10
        je     loc_1_2906
loc_1_27F9:
        mov    eax, dword ptr [ebp + 0x1c]
        mov    eax, dword ptr [eax + 0x14]
        test   byte ptr [eax + 4], 0x40
        mov    al, 0x13
        jne    loc_1_2906
        test   byte ptr [ebp + 0x44], 8
        je     loc_1_2815
        mov    byte ptr [ebp + 0x45], 8
loc_1_2815:
        call   sub_1_2A47
        mov    dword ptr [esp], eax
        jb     loc_1_2906
loc_1_2823:
        mov    esi, dword ptr [ebp + 0x20]
        test   byte ptr [ebp + 0x19], 0x40
        je     loc_1_2832
        or     word ptr [esi + 0xe], 0x83
loc_1_2832:
        bt     dword ptr [ebp], 0x10
        sbb    eax, eax
        and    eax, 0x100
        or     word ptr [esi + 0xe], ax
        mov    esi, dword ptr [esi + 8]
        cmp    dword ptr [esi + 0x30], 0
        jns    loc_1_2852
        test   eax, 0x100
        je     loc_1_28D0
loc_1_2852:
        test   byte ptr [ebp + 0x37], 0x10
        je     loc_1_2880
        or     byte ptr [esi + 0x24], 0x80
        mov    eax, dword ptr [ebp + 0x20]
        mov    dword ptr [0xb758], eax       ; -> loc_1_B758
        call   0xffff071a                    ; -> obj3:0x071A
        mov    ebx, dword ptr [ebp + 0x1c]
        or     word ptr [ebx + 0x52], 0x4000
        mov    eax, dword ptr [ebx + 0x14]
        test   byte ptr [eax + 2], 1
        je     loc_1_2880
        call   sub_1_14EC
loc_1_2880:
        test   byte ptr [ebp + 0x19], 4
        jne    loc_1_28A2
        mov    eax, dword ptr [ebp + 0x24]
        or     eax, eax
        je     loc_1_28A2
        mov    dword ptr [esi + 0x64], eax
        mov    ebx, dword ptr [esp + 4]
        push   dword ptr [ebp + 0x20]
        push   ebx
        push   eax
        VxDCall  IFSMgr.IFSMgr_ReassignLockFileInst ; cd2058004000
        add    esp, 0xc
loc_1_28A2:
        mov    ebx, dword ptr [esi + 0x30]
        mov    dword ptr [ebp + 0x24], ebx
        mov    ebx, dword ptr [esi + 0x38]
        mov    dword ptr [ebp + 0x28], ebx
        movzx  ebx, byte ptr [esi + 0x43]
        mov    dword ptr [ebp], ebx
        dec    dword ptr [esi + 0x20]
        jns    loc_1_28C1
        mov    edi, esi
        call   sub_1_A046
loc_1_28C1:
        mov    eax, dword ptr [esp]
        mov    word ptr [ebp + 0x18], ax
        add    esp, 8
        jmp    loc_1_4548
loc_1_28D0:
        mov    edi, esi
        mov    ebx, dword ptr [ebp + 0x20]
        movzx  eax, word ptr [ebx + 0xc]
        and    eax, 7
        mov    eax, dword ptr [eax*4 + 0xb7d0] ; -> obj1:0xB7D0
        sub    dword ptr [edi + 0x2c], eax
        mov    ecx, dword ptr [ebx]
        mov    edx, dword ptr [ebx + 4]
        mov    dword ptr [ecx + 4], edx
        mov    dword ptr [edx], ecx
        call   sub_1_A040
        cmp    dword ptr [edi + 0x20], 0
        jne    loc_1_2904
        cmp    dword ptr [edi], edi
        jne    loc_1_2904
        call   sub_1_9FA9
loc_1_2904:
        mov    al, 5
loc_1_2906:
        mov    ecx, dword ptr [ebp + 0x20]
        jecxz  loc_1_2912
        xchg   ecx, eax
        call   sub_1_A1F8
        xchg   ecx, eax
loc_1_2912:
        add    esp, 8
        jmp    loc_1_454A
loc_1_291A:
        mov    al, 1
        jmp    loc_1_2906
sub_1_291E:
        mov    eax, 0xc000
        mov    edx, dword ptr [ebp]
        call   sub_1_43C9
        mov    bl, byte ptr [ebp + 0x62]
        and    bl, 0x77
        mov    al, bl
        and    bl, 0x70
        cmp    bl, 0x40
        ja     loc_1_2A3F
        xor    bl, al
        cmp    bl, 4
        ja     loc_1_2A3F
        mov    eax, dword ptr [ebp + 0xc]
        or     eax, eax
        jne    loc_1_2960
        mov    esi, dword ptr [ebp + 0x28]
        call   sub_1_AFEF
        jae    loc_1_2971
        jmp    loc_1_2A43
loc_1_2960:
        call   sub_1_91B8
        jb     loc_1_2A34
        je     loc_1_2A3B
loc_1_2971:
        mov    al, byte ptr [ebx + 0xb]
        mov    esi, dword ptr [ebp + 0x20]
        test   al, 1
        je     loc_1_298A
        sub    ecx, ecx
        mov    dx, word ptr [ebp + 0x62]
        test   dx, 0x80
        jne    loc_1_29A5
        jmp    loc_1_29B5
loc_1_298A:
        sub    edx, edx
        mov    cx, word ptr [ebp + 0x62]
        test   cx, 0x80
        jne    loc_1_29CC
        inc    edx
        and    cl, 3
        je     loc_1_29CC
        cmp    cl, 3
        je     loc_1_29CC
        dec    edx
        jmp    loc_1_29CC
loc_1_29A5:
        mov    cx, 0x80
        or     cx, 0x40
        mov    byte ptr [esi + 0xc], 0x40
        mov    word ptr [ebp + 0x62], cx
loc_1_29B5:
        sub    edx, edx
        inc    edx
        mov    cl, byte ptr [esi + 0xc]
        and    cl, 3
        je     loc_1_29CC
        cmp    cl, 3
        je     loc_1_29CC
        test   byte ptr [ebp + 0x19], 8
        je     loc_1_2A3B
        dec    edx
loc_1_29CC:
        pushfd 
        cmp    dword ptr [ebp + 0x64], 0
        je     loc_1_29DE
        push   esi
        mov    esi, dword ptr [ebp + 0x64]
        VxDCall  Dev_048B.Service_0007h          ; cd2007008b04
        pop    esi
loc_1_29DE:
        popfd  
        push   ebx
        mov    ecx, edx
        call   sub_1_7E79
        pop    ebx
        pushfd 
        push   esi
        mov    esi, dword ptr [ebp + 0x64]
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
        pop    esi
        popfd  
        jb     loc_1_2A39
        test   byte ptr [ebp + 0x19], 2
        je     loc_1_2A07
        mov    eax, dword ptr [ebp + 0x20]
        mov    eax, dword ptr [eax + 8]
        or     byte ptr [eax + 0x24], 4
loc_1_2A07:
        test   byte ptr [ebp + 0x37], 0x11
        jne    loc_1_2A2E
        mov    eax, dword ptr [ebp + 0x1c]
        test   byte ptr [eax + 0x52], 3
        jne    loc_1_2A1D
        cmp    word ptr [ebx + 0x14], 0
        jne    loc_1_2A24
loc_1_2A1D:
        cmp    word ptr [ebx + 0x1a], 0
        je     loc_1_2A2E
loc_1_2A24:
        sub    eax, eax
        sub    edx, edx
        call   sub_1_0685
        clc    
loc_1_2A2E:
        mov    eax, 1
        ret    
loc_1_2A34:
        mov    al, 3
        jne    loc_1_2A39
        dec    eax
loc_1_2A39:
        stc    
        ret    
loc_1_2A3B:
        mov    al, 5
        stc    
        ret    
loc_1_2A3F:
        mov    al, 0xc
        stc    
        ret    
loc_1_2A43:
        mov    al, 2
        stc    
        ret    
sub_1_2A47:
        sub    esp, 4
        mov    dword ptr [esp], 0
        mov    eax, 0x8400
        mov    edx, dword ptr [ebp]
        call   sub_1_4418
        call   sub_1_43C9
        test   byte ptr [ebp + 0x42], 0x20
        je     loc_1_2A76
        mov    eax, dword ptr [ebp + 0x28]
        cmp    word ptr [eax], 0
        je     loc_1_2C5F
loc_1_2A76:
        test   byte ptr [ebp + 0x19], 4
        je     loc_1_2A8A
        mov    eax, dword ptr [ebp + 0x24]
        mov    dword ptr [ebp + 0x48], eax
        or     word ptr [ebp + 0x42], 0x200
        jmp    loc_1_2AC8
loc_1_2A8A:
        mov    edi, dword ptr [ebp + 0x14]
        or     edi, edi
        je     loc_1_2AC8
        cmp    dword ptr [edi], 0xf
        jne    loc_1_2AC8
        or     word ptr [ebp + 0x42], 0x1000
        mov    edi, dword ptr [edi + 4]
        call   sub_1_9171
        or     edx, edx
        je     loc_1_2AC8
        push   edx
        mov    edi, dword ptr [ebp + 0xc]
        movzx  edi, word ptr [edi + 2]
        add    edi, dword ptr [ebp + 0xc]
        call   sub_1_9171
        mov    eax, edx
        pop    edx
        cmp    eax, edx
        je     loc_1_2AC8
        mov    dword ptr [ebp + 0x48], edx
        or     word ptr [ebp + 0x42], 0x200
loc_1_2AC8:
        mov    al, byte ptr [ebp + 0x44]
        test   al, 0xd0
        jne    loc_1_2C5F
        test   al, 8
        je     loc_1_2AD9
        mov    al, 8
loc_1_2AD9:
        or     al, 0x20
        mov    byte ptr [ebp + 0x44], al
        mov    esi, dword ptr [ebp + 0x20]
        mov    ax, word ptr [ebp + 0x62]
        or     word ptr [esi + 0xc], ax
        mov    eax, dword ptr [ebp + 0xc]
        call   sub_1_91B8
        jae    loc_1_2B1A
        jne    loc_1_2C61
        test   byte ptr [ebp + 0x44], 8
        je     loc_1_2B09
        test   byte ptr [ebp + 0x42], 0x40
        jne    loc_1_2C5F
loc_1_2B09:
        test   byte ptr [ebp + 0x47], 0xf0
        mov    al, 2
        jne    loc_1_2BED
        jmp    loc_1_2C61
loc_1_2B1A:
        mov    dword ptr [esp], 3
        je     loc_1_2C5F
        test   byte ptr [ebp + 0x44], 8
        jne    loc_1_2C5F
        test   byte ptr [ebp + 0x47], 2
        mov    al, 0x50
        je     loc_1_2C61
        mov    ah, byte ptr [ebx + 0xb]
        test   ah, 1
        jne    loc_1_2C5F
        mov    al, byte ptr [ebp + 0x45]
        pushfd 
        cmp    dword ptr [ebp + 0x64], 0
        je     loc_1_2B5E
        push   esi
        mov    esi, dword ptr [ebp + 0x64]
        VxDCall  Dev_048B.Service_0007h          ; cd2007008b04
        pop    esi
loc_1_2B5E:
        popfd  
        push   ebx
        mov    eax, 2
        call   sub_1_7FED
        pop    ebx
        pushfd 
        push   esi
        mov    esi, dword ptr [ebp + 0x64]
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
        pop    esi
        popfd  
        jb     loc_1_2C5B
        mov    esi, dword ptr [ebp + 0x20]
        mov    dword ptr [esi + 8], eax
        mov    edi, dword ptr [eax]
        mov    dword ptr [esi], edi
        mov    dword ptr [esi + 4], eax
        mov    dword ptr [edi + 4], esi
        mov    dword ptr [eax], esi
        sub    ecx, ecx
        clc    
        call   sub_1_0D5B
        jb     loc_1_2BAF
        xor    ecx, ecx
        call   sub_1_7E79
        mov    eax, dword ptr [ebp + 0x20]
        and    word ptr [eax + 0xe], 0xfdbb
        jmp    loc_1_2C34
loc_1_2BAF:
        mov    esi, dword ptr [ebp + 0x20]
        mov    edi, dword ptr [esi + 8]
        dec    dword ptr [edi + 0x20]
        jns    loc_1_2BBF
        call   sub_1_A046
loc_1_2BBF:
        mov    ax, word ptr [esi + 0xc]
        and    eax, 7
        mov    eax, dword ptr [eax*4 + 0xb7d0] ; -> obj1:0xB7D0
        sub    dword ptr [edi + 0x2c], eax
        je     loc_1_2BE1
        mov    ebx, dword ptr [ebp + 0x20]
        mov    eax, dword ptr [ebx]
        mov    ecx, dword ptr [ebx + 4]
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [ecx], eax
        jmp    loc_1_2BE9
loc_1_2BE1:
        xor    ebx, ebx
        inc    ebx
        call   sub_1_9F5C
loc_1_2BE9:
        mov    al, 0x52
        jmp    loc_1_2C61
loc_1_2BED:
        mov    dword ptr [esp], 2
        sub    ecx, ecx
        stc    
        call   sub_1_0D5B
        mov    al, 0x52
        jb     loc_1_2C61
        push   ebx
        sub    ecx, ecx
        call   sub_1_7E79
        pop    ebx
        jb     loc_1_2C4C
        test   byte ptr [ebp + 0x44], 8
        je     loc_1_2C34
        mov    edx, dword ptr [ebp + 0x6c]
        lea    eax, [edx + 0x66]
        lea    edx, [edx + 0x24]
        push   eax
        push   1
        push   0xb
        push   0x16
        push   edx
        push   eax
        VxDCall  IFSMgr.UniToBCS                 ; cd2040004000
        add    esp, 0x14
        pop    eax
        call   sub_1_6E3C
        jb     loc_1_2C61
loc_1_2C34:
        test   byte ptr [ebp + 0x19], 2
        je     loc_1_2C44
        mov    eax, dword ptr [ebp + 0x20]
        mov    eax, dword ptr [eax + 8]
        or     byte ptr [eax + 0x24], 4
loc_1_2C44:
        mov    eax, dword ptr [esp]
        add    esp, 4
        clc    
        ret    
loc_1_2C4C:
        mov    byte ptr [ebx], 0xe5
        mov    esi, dword ptr [ebp + 0x64]
        call   sub_1_03EC
        mov    al, 8
        jmp    loc_1_2C61
loc_1_2C5B:
        mov    al, 0x20
        jmp    loc_1_2C61
loc_1_2C5F:
        mov    al, 5
loc_1_2C61:
        add    esp, 4
        stc    
        ret    
loc_1_2C66:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 0x14]
        cld    
        sub    eax, eax
        mov    byte ptr [ebp + 0x37], al
        mov    word ptr [ebp + 0x38], 0xa800
        mov    byte ptr [ebp + 0x34], al
        mov    byte ptr [ebp + 0x35], al
        mov    dword ptr [ebp + 0x6c], eax
        mov    dword ptr [ebp + 0x64], eax
        call   sub_1_A26B
        call   sub_1_1D0E
        call   sub_1_5CFF
        push   0
        push   0
        mov    eax, 0x8000
        mov    edx, dword ptr [ebp]
        call   sub_1_43C9
        mov    ax, word ptr [ebp]
        and    al, 0x3f
        mov    word ptr [ebp + 0x45], ax
        test   al, 0x50
        jne    loc_1_2E79
        mov    eax, dword ptr [ebp + 0xc]
        call   sub_1_91B8
        jb     loc_1_2E81
        je     loc_1_2E17
loc_1_2CCC:
        test   byte ptr [ebp + 0x38], 4
        jne    loc_1_2CE6
        mov    esi, dword ptr [ebp + 0x64]
        VxDCall  Dev_048B.Service_0007h          ; cd2007008b04
        call   sub_1_5B85
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
loc_1_2CE6:
        or     dword ptr [esp + 4], 2
        mov    eax, dword ptr [ebp + 0x1c]
        mov    eax, dword ptr [eax + 0x14]
        test   byte ptr [eax + 4], 0x40
        jne    loc_1_2E7D
        mov    word ptr [ebp + 0x62], 2
        push   ebx
        push   edx
        mov    eax, 2
        call   sub_1_7FED
        pop    edx
        pop    ebx
        jb     loc_1_2E17
        mov    dword ptr [esp], eax
        mov    esi, dword ptr [ebp + 0x64]
        VxDCall  Dev_048B.Service_0007h          ; cd2007008b04
        mov    edi, dword ptr [esp]
        call   sub_1_A040
        cmp    dword ptr [edi + 0x2c], 0x10001
        je     loc_1_2D58
        push   ebx
        push   edx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        test   word ptr [ebx], 0x180
        pop    edx
        pop    ebx
        je     loc_1_2D58
        sub    dword ptr [edi + 0x2c], 0x10001
        mov    esi, dword ptr [ebp + 0x64]
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
        jmp    loc_1_2E17
loc_1_2D58:
        mov    eax, dword ptr [edi]
loc_1_2D5A:
        cmp    eax, edi
        je     loc_1_2D67
        or     word ptr [eax + 0xe], 0x10
        mov    eax, dword ptr [eax]
        jmp    loc_1_2D5A
loc_1_2D67:
        lea    eax, [edi + 0x64]
        cmp    dword ptr [eax], 0
        je     loc_1_2D7F
        push   edx
        push   0
        push   0
        push   eax
        VxDCall  IFSMgr.IFSMgr_RemoveLocks       ; cd2055004000
        add    esp, 0xc
        pop    edx
loc_1_2D7F:
        test   byte ptr [ebp + 0x45], 1
        jne    loc_1_2D8B
        test   byte ptr [ebx + 0xb], 1
        jne    loc_1_2DF9
loc_1_2D8B:
        mov    edi, dword ptr [ebp + 0x6c]
        mov    al, byte ptr [ebx + 0xb]
        and    al, 0x1f
        cmp    al, 8
        jne    loc_1_2D9C
        or     dword ptr [esp + 4], 4
loc_1_2D9C:
        test   byte ptr [edi + 7], 2
        je     loc_1_2DB9
        cmp    byte ptr [edi + 4], 0
        je     loc_1_2DB9
        mov    eax, dword ptr [edi + 0xc]
        movzx  ecx, byte ptr [edi + 4]
        inc    ecx
        call   sub_1_9A90
        jae    loc_1_2DC6
        jmp    loc_1_2DF9
loc_1_2DB9:
        mov    esi, dword ptr [ebp + 0x64]
        mov    byte ptr [ebx], 0xe5
        call   sub_1_03EC
        jb     loc_1_2DF9
loc_1_2DC6:
        or     dword ptr [esp + 4], 1
        mov    ebx, dword ptr [esp]
        mov    ebx, dword ptr [ebx + 0x10]
        mov    edi, dword ptr [ebp + 0x1c]
        call   sub_1_7D50
        pushfd 
        mov    edi, dword ptr [ebp + 0x6c]
        mov    eax, dword ptr [edi]
        xor    edx, edx
        cmp    byte ptr [edi + 4], 0
        je     loc_1_2DEA
        mov    edx, dword ptr [edi + 0xc]
loc_1_2DEA:
        mov    ebx, dword ptr [ebp + 0x1c]
        push   edx
        push   eax
        push   dword ptr [edi + 0x10]
        push   eax
        call   sub_1_0AF3
        popfd  
loc_1_2DF9:
        pushfd 
        mov    esi, dword ptr [ebp + 0x64]
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
        popfd  
        mov    edi, dword ptr [esp]
        pushfd 
        sub    dword ptr [edi + 0x2c], 0x10001
        call   sub_1_9FAF
        popfd  
        jb     loc_1_2E79
loc_1_2E17:
        test   byte ptr [ebp + 0x42], 1
        je     loc_1_2E51
        call   sub_1_0BD4
        jb     loc_1_2E79
        push   edx
        mov    eax, 0x8010
        mov    edx, dword ptr [ebp]
        call   sub_1_43C9
        pop    edx
        mov    eax, dword ptr [ebp + 0xc]
        movzx  ecx, word ptr [eax + 2]
        lea    eax, [ecx + eax]
        mov    ecx, 1
        call   sub_1_9366
        jae    loc_1_2CCC
        or     eax, eax
        jne    loc_1_2E81
loc_1_2E51:
        test   byte ptr [esp + 4], 4
        je     loc_1_2E61
        sub    eax, eax
        call   sub_1_6E3C
        jb     loc_1_2E7D
loc_1_2E61:
        test   byte ptr [esp + 4], 1
        je     loc_1_2E70
        add    esp, 8
        jmp    loc_1_4548
loc_1_2E70:
        mov    al, 5
        test   byte ptr [esp + 4], 2
        jne    loc_1_2E81
loc_1_2E79:
        mov    al, 2
        jmp    loc_1_2E81
loc_1_2E7D:
        mov    al, 0x13
        jmp    loc_1_2E81
loc_1_2E81:
        add    esp, 8
        jmp    loc_1_454A
loc_1_2E89:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 0x14]
        cld    
        sub    eax, eax
        mov    byte ptr [ebp + 0x37], al
        mov    word ptr [ebp + 0x38], 0x2000
        mov    byte ptr [ebp + 0x34], al
        mov    byte ptr [ebp + 0x35], al
        mov    dword ptr [ebp + 0x6c], eax
        mov    dword ptr [ebp + 0x64], eax
        call   sub_1_A318
        cmp    byte ptr [ebp + 4], 3
        jae    loc_1_30CD
        mov    eax, 0xc000
        mov    edx, dword ptr [ebp]
        call   sub_1_43C9
        cmp    byte ptr [ebp + 4], 1
        jne    loc_1_2EDD
        or     word ptr [ebp + 0x38], 0x800
        call   sub_1_5B85
        jb     loc_1_2EDD
        call   sub_1_5CFF
loc_1_2EDD:
        mov    al, byte ptr [ebp]
        mov    byte ptr [ebp + 0x44], al
        mov    byte ptr [ebp + 0x45], 0x16
        mov    byte ptr [ebp + 0x46], 0
        mov    eax, dword ptr [ebp + 0xc]
        call   sub_1_91B8
        jb     loc_1_454A
        cmp    byte ptr [ebp + 4], 1
        jne    loc_1_2F7B
        cmp    ebx, -1
        je     loc_1_2F74
        push   ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        test   word ptr [ebx], 0x180
        pop    ebx
        mov    eax, 0
        je     loc_1_2F1D
        mov    eax, 1
loc_1_2F1D:
        mov    word ptr [ebp + 0x62], 2
        push   eax
        push   ebx
        call   sub_1_7FED
        mov    edi, eax
        pop    ebx
        pop    eax
        jb     loc_1_2F69
        push   edi
        mov    al, byte ptr [ebp + 0x44]
        and    al, 0x27
        and    byte ptr [ebx + 0xb], 0xd8
        or     byte ptr [ebx + 0xb], al
        mov    al, byte ptr [ebx + 0xb]
        mov    byte ptr [edi + 0x43], al
        mov    esi, dword ptr [ebp + 0x64]
        call   sub_1_03EC
        pop    edi
        pushfd 
        call   sub_1_A040
        sub    dword ptr [edi + 0x2c], 0x10001
        jne    loc_1_2F60
        call   sub_1_9FA9
loc_1_2F60:
        popfd  
        jae    loc_1_4548
        jmp    loc_1_2F74
loc_1_2F69:
        cmp    eax, 0
        mov    al, 0x20
        je     loc_1_454A
loc_1_2F74:
        mov    al, 5
        jmp    loc_1_454A
loc_1_2F7B:
        cmp    byte ptr [ebp + 4], 2
        je     loc_1_2FA4
        cmp    ebx, -1
        je     loc_1_2F98
        mov    eax, dword ptr [ebx + 0x1c]
        mov    dword ptr [ebp + 0x24], eax
        movzx  eax, byte ptr [ebx + 0xb]
        mov    dword ptr [ebp], eax
        jmp    loc_1_4548
loc_1_2F98:
        mov    dword ptr [ebp], 0x10
        jmp    loc_1_4548
loc_1_2FA4:
        cmp    ebx, -1
        je     loc_1_3059
        mov    edx, dword ptr [ebx + 0x12]
        mov    ecx, dword ptr [ebp + 0x1c]
        test   byte ptr [ecx + 0x52], 3
        je     loc_1_2FBB
        xor    edx, edx
loc_1_2FBB:
        mov    dx, word ptr [ebx + 0x1a]
        test   byte ptr [ebx + 0xb], 0x10
        jne    loc_1_2FFD
        push   dword ptr [ebx + 0x1c]
        push   edx
        mov    esi, dword ptr [ebp + 0x64]
        call   sub_1_7F9B
        pop    edx
        pop    ecx
        jb     loc_1_2FDB
        mov    ecx, dword ptr [eax + 0x30]
        mov    edx, dword ptr [eax + 0x10]
loc_1_2FDB:
        mov    edi, dword ptr [ebp + 0x1c]
        movzx  eax, word ptr [edi + 0x4a]
        add    ecx, eax
        not    eax
        and    ecx, eax
        mov    dword ptr [ebp + 0x24], ecx
        or     ecx, ecx
        je     loc_1_4548
        test   byte ptr [edi + 0x52], 0x10
        je     loc_1_4548
loc_1_2FFD:
        mov    edi, dword ptr [ebp + 0x1c]
        and    dword ptr [ebp + 0x24], 0
        or     edx, edx
        je     loc_1_4548
        mov    ebx, edx
loc_1_300E:
        xor    esi, esi
loc_1_3010:
        cmp    ebx, 2
        jb     loc_1_306B
        cmp    dword ptr [edi + 0x2c], ebx
        jb     loc_1_306B
        inc    esi
        push   ebx
        push   esi
        call   sub_1_1F2C
        pop    esi
        pop    eax
        jb     loc_1_306B
        inc    eax
        call   sub_1_2102
        jae    loc_1_3032
        cmp    eax, ebx
        je     loc_1_3010
loc_1_3032:
        test   byte ptr [edi + 0x52], 0x10
        jne    loc_1_3041
        mov    cl, byte ptr [edi + 0x47]
        shl    esi, cl
        mov    eax, esi
        jmp    loc_1_304A
loc_1_3041:
        sub    eax, esi
        call   sub_1_3072
        jb     loc_1_306B
loc_1_304A:
        add    dword ptr [ebp + 0x24], eax
        call   sub_1_2102
        jb     loc_1_300E
        jmp    loc_1_4548
loc_1_3059:
        mov    edi, dword ptr [ebp + 0x1c]
        movzx  eax, word ptr [edi + 0x44]
        shl    eax, 5
        mov    dword ptr [ebp + 0x24], eax
        jmp    loc_1_4548
loc_1_306B:
        mov    al, 5
        jmp    loc_1_454A
sub_1_3072:
        push   ebx
        push   edi
        push   eax
        push   esi
        mov    edx, dword ptr [ebp + 0x1c]
        mov    eax, dword ptr [edx + 0x14]
        movzx  ebx, word ptr [eax + 0xa]
        mov    esi, dword ptr [eax + 0xc]
        push   eax
        mov    ah, 2
        call   sub_1_A4E1
        mov    esi, ebx
        pop    dword ptr [ebx + 0x44]
        mov    al, byte ptr [edx + 0xac]
        mov    byte ptr [esi + 0x4d], al
        mov    dword ptr [esi + 8], 0x500
        mov    word ptr [esi + 4], 0x29
        mov    word ptr [esi + 0x2e], 0xdb09
        pop    dword ptr [esi + 0x34]
        pop    dword ptr [esi + 0x30]
        VxDCall  BLOCKDEV.BlockDev_Send_Command  ; cd2004001000
        mov    eax, dword ptr [esi + 0x3c]
        movzx  edx, word ptr [esi + 6]
        cmp    edx, 0x10
        cmc    
        mov    ebx, esi
        call   sub_1_A4AE
        pop    edi
        pop    ebx
        ret    
loc_1_30CD:
        cmp    byte ptr [ebp + 4], 3
        jb     loc_1_329D
        cmp    byte ptr [ebp + 4], 9
        ja     loc_1_329D
        je     loc_1_3155
        test   byte ptr [ebp + 4], 1
        je     loc_1_3155
        mov    eax, dword ptr [ebp + 0x1c]
        mov    eax, dword ptr [eax + 0x14]
        test   byte ptr [eax + 4], 0x40
        jne    loc_1_32A5
        cmp    byte ptr [ebp + 4], 5
        je     loc_1_3105
        cmp    byte ptr [ebp + 4], 7
        jne    loc_1_310F
loc_1_3105:
        test   byte ptr [eax + 0x53], 1
        jne    loc_1_32A9
loc_1_310F:
        mov    ax, word ptr [ebp + 0x2a]
        shl    eax, 0x10
        cmp    byte ptr [ebp + 4], 5
        je     loc_1_3120
        mov    ax, word ptr [ebp + 0x28]
loc_1_3120:
        or     eax, eax
        je     loc_1_3143
        sub    edx, edx
        call   sub_1_13D1
        jb     loc_1_32A1
        cmp    byte ptr [ebp + 4], 7
        jne    loc_1_3143
        cmp    word ptr [ebp + 0x18], 0xc8
        jae    loc_1_32A1
loc_1_3143:
        or     word ptr [ebp + 0x38], 0x800
        call   sub_1_5B85
        jb     loc_1_3155
        call   sub_1_5CFF
loc_1_3155:
        mov    word ptr [ebp + 0x45], 0x16
        mov    eax, 0xc000
        mov    edx, dword ptr [ebp]
        call   sub_1_43C9
        mov    eax, dword ptr [ebp + 0xc]
        call   sub_1_91B8
        jb     loc_1_32AB
        cmp    ebx, -1
        jne    loc_1_318A
        cmp    byte ptr [ebp + 4], 9
        je     loc_1_3256
        jmp    loc_1_32A9
loc_1_318A:
        mov    word ptr [ebp + 0x62], 2
        push   ebx
        mov    eax, 1
        call   sub_1_7FED
        pop    ebx
        jb     loc_1_32A9
        mov    edi, eax
        mov    esi, dword ptr [ebp + 0x64]
        VxDCall  Dev_048B.Service_0007h          ; cd2007008b04
        sub    edx, edx
        movzx  eax, byte ptr [ebp + 4]
        sub    eax, 3
        jmp    loc_1_31BD
loc_1_31BD:
        lock xor dword ptr [eax], eax
        add    cl, bl                        ; -> obj1:0x31D9
        xor    dword ptr [eax], eax
        add    byte ptr [ebx], al            ; -> loc_1_3203
        xor    al, byte ptr [eax]
        add    dh, bl                        ; -> loc_1_31DE
        xor    dword ptr [eax], eax
        add    byte ptr [ecx], dl            ; -> loc_1_3211
        xor    al, byte ptr [eax]
        add    bh, ah                        ; -> loc_1_31E7
        xor    dword ptr [eax], eax
        add    byte ptr [edi], bh            ; -> loc_1_323F
        xor    al, byte ptr [eax]
        add    byte ptr [ebx + 0x50eb3847], cl
loc_1_31DE:
        movzx  eax, word ptr [edi + 0x3c]
        shl    eax, 0x10
        jmp    loc_1_322E
loc_1_31E7:
        movzx  edx, byte ptr [edi + 0x42]
        mov    eax, dword ptr [edi + 0x3e]
        jmp    loc_1_322E
loc_1_31F0:
        mov    ax, word ptr [ebp + 0x2a]
        shl    eax, 0x10
        mov    ax, word ptr [ebp + 0x28]
        mov    dword ptr [edi + 0x38], eax
        mov    dword ptr [ebx + 0x16], eax
        jmp    loc_1_3277
loc_1_3203:
        mov    ax, word ptr [ebp + 0x2a]
        mov    word ptr [edi + 0x3c], ax
        mov    word ptr [ebx + 0x12], ax
        jmp    loc_1_3277
loc_1_3211:
        movzx  edx, word ptr [ebp + 0x18]
        mov    ax, word ptr [ebp + 0x2a]
        shl    eax, 0x10
        mov    ax, word ptr [ebp + 0x28]
        mov    byte ptr [edi + 0x42], dl
        mov    dword ptr [edi + 0x3e], eax
        mov    byte ptr [ebx + 0xd], dl
        mov    dword ptr [ebx + 0xe], eax
        jmp    loc_1_3277
loc_1_322E:
        mov    word ptr [ebp + 0x28], ax
        shr    eax, 0x10
        mov    word ptr [ebp + 0x2a], ax
        mov    word ptr [ebp + 0x18], dx
        jmp    loc_1_327C
loc_1_323F:
        mov    eax, dword ptr [edi + 0x10]
        mov    word ptr [ebp + 0x18], ax
        test   byte ptr [edi + 0x24], 8
        je     loc_1_327C
        or     word ptr [ebp + 0x18], 0xffff
        mov    dword ptr [ebp + 0x24], eax
        jmp    loc_1_327C
loc_1_3256:
        mov    ebx, dword ptr [ebp + 0x1c]
        mov    eax, dword ptr [ebx + 0x3c]
        mov    word ptr [ebp + 0x18], ax
        test   byte ptr [ebx + 0x52], 3
        jne    loc_1_4548
        or     word ptr [ebp + 0x18], 0xffff
        mov    dword ptr [ebp + 0x24], eax
        jmp    loc_1_4548
loc_1_3277:
        call   sub_1_03EC
loc_1_327C:
        call   sub_1_A040
        sub    dword ptr [edi + 0x2c], 0x10001
        jne    loc_1_328F
        call   sub_1_9FA9
loc_1_328F:
        mov    esi, dword ptr [ebp + 0x64]
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
        jmp    loc_1_4548
loc_1_329D:
        mov    al, 1
        jmp    loc_1_32AB
loc_1_32A1:
        mov    al, 0xd
        jmp    loc_1_32AB
loc_1_32A5:
        mov    al, 0x13
        jmp    loc_1_32AB
loc_1_32A9:
        mov    al, 5
loc_1_32AB:
        jmp    loc_1_454A
loc_1_32B0:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 0x14]
        cld    
        sub    eax, eax
        mov    byte ptr [ebp + 0x37], al
        mov    word ptr [ebp + 0x38], 0x2000
        mov    byte ptr [ebp + 0x34], al
        mov    byte ptr [ebp + 0x35], al
        mov    dword ptr [ebp + 0x6c], eax
        mov    dword ptr [ebp + 0x64], eax
        call   sub_1_A318
        cmp    byte ptr [ebp + 4], 0
        jne    loc_1_32FC
        mov    eax, dword ptr [ebp]
        and    al, 0x1e
        test   ah, ah
        je     loc_1_32E8
        mov    ax, 0x1016
loc_1_32E8:
        mov    word ptr [ebp + 0x45], ax
        call   sub_1_330C
        jae    loc_1_4548
        jmp    loc_1_454A
loc_1_32FC:
        call   sub_1_3435
        jae    loc_1_4548
        jmp    loc_1_454A
sub_1_330C:
        mov    eax, 0x8000
        mov    edx, dword ptr [ebp]
        call   sub_1_43C9
        mov    eax, dword ptr [ebp + 0xc]
        call   sub_1_91B8
loc_1_3321:
        mov    edi, dword ptr [ebp + 0x14]
        jb     loc_1_3429
        cmp    ebx, -1
        je     loc_1_342B
        pushfd 
        cmp    dword ptr [ebp + 0x64], 0
        je     loc_1_3345
        push   esi
        mov    esi, dword ptr [ebp + 0x64]
        VxDCall  Dev_048B.Service_0007h          ; cd2007008b04
        pop    esi
loc_1_3345:
        popfd  
        mov    esi, dword ptr [ebp + 0x6c]
        lea    esi, [esi + 0x24]
        lea    eax, [edi + 1]
        push   1
        push   8
        push   0x10
        push   esi
        push   eax
        VxDCall  IFSMgr.UniToBCS                 ; cd2040004000
        add    esp, 0x14
        mov    esi, dword ptr [ebp + 0x6c]
        lea    esi, [esi + 0x34]
        lea    eax, [edi + 9]
        push   1
        push   3
        push   6
        push   esi
        push   eax
        VxDCall  IFSMgr.UniToBCS                 ; cd2040004000
        add    esp, 0x14
        lea    edi, [edi + 0xc]
        mov    al, byte ptr [ebp + 0x45]
        cmp    byte ptr [ebp + 0x46], 0x10
        jne    loc_1_3388
        mov    al, 0xd7
loc_1_3388:
        stosb  byte ptr es:[edi], al
loc_1_3389:
        mov    eax, dword ptr [ebp + 0x4c]
        mov    esi, dword ptr [ebp + 0x1c]
        mov    cl, byte ptr [esi + 0x47]
        sub    cl, 5
        shr    eax, cl
        mov    eax, dword ptr [ebp + 0x4c]
        je     loc_1_33E1
        cmp    dword ptr [ebp + 0x50], 0
        je     loc_1_33E1
        mov    eax, 1
        shl    eax, cl
        sub    dword ptr [ebp + 0x4c], eax
        push   ebx
        push   edi
        mov    edi, esi
        mov    ebx, dword ptr [ebp + 0x50]
        call   sub_1_1F2C
        pop    esi
        pop    eax
        jb     loc_1_33CB
        call   sub_1_2102
        jae    loc_1_33CB
        mov    dword ptr [ebp + 0x50], ebx
        mov    edi, esi
        xchg   ebx, eax
        jmp    loc_1_3389
loc_1_33CB:
        mov    word ptr [esi], 0xffff
        pushfd 
        push   esi
        mov    esi, dword ptr [ebp + 0x64]
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
        pop    esi
        popfd  
        mov    al, 0x12
        stc    
        ret    
loc_1_33E1:
        stosw  word ptr es:[edi], ax
        mov    eax, dword ptr [ebp + 0x50]
        stosd  dword ptr es:[edi], eax
        lea    esi, [ebx + 0x16]
        mov    al, byte ptr [ebx + 0xb]
        mov    edi, dword ptr [ebp + 0x14]
        lea    edi, [edi + 0x15]
        stosb  byte ptr es:[edi], al
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    eax, dword ptr [ebp + 0x1c]
        test   byte ptr [eax + 0x52], 3
        mov    eax, dword ptr [esi - 8]
        je     loc_1_3403
        xor    eax, eax
loc_1_3403:
        lodsw  ax, word ptr [esi]
        mov    dword ptr [ebp + 0x28], eax
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    esi, ebx
        push   edi
        call   sub_1_9ED8
        pop    edi
        cmp    byte ptr [edi], 5
        jne    loc_1_341A
        mov    byte ptr [edi], 0xe5
loc_1_341A:
        pushfd 
        push   esi
        mov    esi, dword ptr [ebp + 0x64]
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
        pop    esi
        popfd  
        clc    
        ret    
loc_1_3429:
        jne    loc_1_342D
loc_1_342B:
        mov    al, 0x12
loc_1_342D:
        mov    word ptr [edi + 0xd], 0xffff
        stc    
        ret    
sub_1_3435:
        sub    esp, 0x34
        mov    esi, dword ptr [ebp + 0x14]
        cmp    word ptr [esi + 0xd], -1
        je     loc_1_3529
        inc    esi
        mov    eax, 0x200020
        lea    edi, [esp + 0x1c]
        mov    ecx, 0xb
        rep stosw word ptr es:[edi], ax
        lea    eax, [esp + 0x1c]
        push   1
        push   8
        push   esi
        push   eax
        VxDCall  IFSMgr.BCSToUni                 ; cd2042004000
        add    esp, 0x10
        lea    esi, [esi + 8]
        lea    eax, [esp + 0x2c]
        push   1
        push   3
        push   esi
        push   eax
        VxDCall  IFSMgr.BCSToUni                 ; cd2042004000
        add    esp, 0x10
        lea    eax, [esp + 0x1c]
        lea    edx, [esp + 2]
        push   0
        push   eax
        push   edx
        VxDCall  IFSMgr.FcbToShort               ; cd2048004000
        add    esp, 0xc
        mov    word ptr [esp + eax + 2], 0
        add    eax, 2
        mov    word ptr [esp], ax
        lea    edi, [esp + 0x1c]
        mov    ecx, 0xb
        mov    ax, 0x3f
        repne scasw ax, word ptr es:[edi]
        mov    eax, 0x18
        jne    loc_1_3520
        or     eax, 1
        mov    word ptr [ebp + 0x42], ax
        mov    esi, dword ptr [ebp + 0x14]
        lea    esi, [esi + 0xc]
        lodsb  al, byte ptr [esi]
        sub    ah, ah
        cmp    al, 0xd7
        jne    loc_1_34D3
        mov    ax, 0x1016
loc_1_34D3:
        mov    word ptr [ebp + 0x45], ax
        lea    edi, [ebp + 0x4c]
        sub    eax, eax
        lodsw  ax, word ptr [esi]
        stosd  dword ptr es:[edi], eax
        lodsd  eax, dword ptr [esi]
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        xor    eax, eax
        stosd  dword ptr es:[edi], eax
        mov    edi, dword ptr [ebp + 0x1c]
        cmp    dword ptr [ebp + 0x50], 0
        je     loc_1_34F4
        mov    al, byte ptr [edi + 0x24]
        inc    eax
        jmp    loc_1_34FE
loc_1_34F4:
        mov    byte ptr [ebp + 0x60], 0
        mov    eax, dword ptr [edi + 0x28]
        sub    eax, dword ptr [edi + 0x34]
loc_1_34FE:
        mov    byte ptr [ebp + 0x61], al
        call   sub_1_0BD4
        jb     loc_1_3520
        lea    eax, [esp]
        mov    ecx, 1
        call   sub_1_9366
        jb     loc_1_3520
        add    esp, 0x34
        clc    
        jmp    loc_1_3321
loc_1_3520:
        mov    esi, dword ptr [ebp + 0x14]
        mov    word ptr [esi + 0xd], 0xffff
loc_1_3529:
        add    esp, 0x34
        mov    al, 0x12
        stc    
        ret    
loc_1_3530:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 0x14]
        cld    
        sub    eax, eax
        mov    byte ptr [ebp + 0x37], al
        mov    word ptr [ebp + 0x38], 0xa800
        mov    byte ptr [ebp + 0x34], al
        mov    byte ptr [ebp + 0x35], al
        mov    dword ptr [ebp + 0x6c], eax
        mov    dword ptr [ebp + 0x64], eax
        call   sub_1_A26B
        call   sub_1_1D0E
        call   sub_1_5B85
        jb     loc_1_3566
        call   sub_1_5CFF
loc_1_3566:
        and    word ptr [ebp + 0x38], 0x2804
        mov    eax, dword ptr [ebp]
        or     eax, dword ptr [ebp + 0x24]
        test   eax, 0x80000000
        je     loc_1_357E
        jmp    loc_1_3A2C
loc_1_357E:
        mov    eax, dword ptr [ebp + 0x1c]
        mov    eax, dword ptr [eax + 0x14]
        test   byte ptr [eax + 4], 0x40
        mov    al, 0x13
        jne    loc_1_4576
        mov    eax, 0xc6
        call   sub_1_A1CE
        mov    dword ptr [ebp + 0x68], eax
        mov    edi, eax
        mov    al, 8
        jb     loc_1_4576
        and    dword ptr [edi], 0
        mov    byte ptr [edi + 0xba], 0
        mov    eax, dword ptr [ebp + 0x1c]
        test   byte ptr [eax + 0x52], 3
        jne    loc_1_35C1
        or     byte ptr [edi + 0xba], 1
loc_1_35C1:
        mov    al, byte ptr [ebp]
        mov    byte ptr [ebp + 0x45], al
        mov    byte ptr [ebp + 0x46], 0
        mov    eax, 0
        mov    edx, dword ptr [ebp]
        call   sub_1_43C9
        test   eax, 0x20
        je     loc_1_35E5
        or     word ptr [ebp + 0x42], 0x2000
loc_1_35E5:
        mov    eax, dword ptr [ebp + 0xc]
        call   sub_1_91B8
        jb     loc_1_39D5
        cmp    ebx, -1
        je     loc_1_3A28
        mov    esi, dword ptr [ebp + 0x6c]
        mov    edi, dword ptr [ebp + 0x68]
        test   byte ptr [ebx + 0xb], 0x10
        jne    loc_1_3642
        mov    ax, 0x12
        mov    word ptr [ebp + 0x62], ax
        push   ebx
        push   esi
        push   edi
        bt     dword ptr [ebp + 0x18], 4
        sbb    eax, eax
        and    eax, 1
        call   sub_1_7FED
        pop    edi
        pop    esi
        pop    ebx
        jb     loc_1_3A28
        mov    dword ptr [edi], eax
        cmp    dword ptr [eax], eax
        je     loc_1_3642
        test   byte ptr [eax + 0x24], 0x80
        jne    loc_1_3A28
        or     byte ptr [edi + 0xba], 4
loc_1_3642:
        mov    eax, dword ptr [ebp + 0x50]
        mov    dword ptr [edi + 4], eax
        mov    eax, dword ptr [esi + 0x10]
        mov    dword ptr [edi + 8], eax
        mov    eax, dword ptr [esi + 0xc]
        mov    dword ptr [edi + 0xc], eax
        mov    al, byte ptr [esi + 4]
        mov    byte ptr [edi + 0x10], al
        mov    al, byte ptr [esi + 7]
        mov    byte ptr [edi + 0x11], al
        mov    esi, ebx
        lea    edi, [edi + 0x12]
        mov    ecx, 8
        rep movsd dword ptr es:[edi], dword ptr [esi]
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0x68]
        mov    esi, dword ptr [ebp + 0x6c]
        movzx  ecx, word ptr [esi + 0x2b2]
        mov    word ptr [edi + 0x32], cx
        jecxz  loc_1_3693
        cmp    ecx, 0x10
        jae    loc_1_3693
        lea    esi, [esi + 0x2b6]
        lea    edi, [edi + 0x36]
        add    ecx, ecx
        rep movsd dword ptr es:[edi], dword ptr [esi]
loc_1_3693:
        pop    edi
        pop    esi
        sub    eax, eax
        xchg   dword ptr [ebp + 0x6c], eax
        call   sub_1_A1F8
        xor    eax, eax
        mov    edx, dword ptr [ebp + 0x24]
        call   sub_1_4418
        call   sub_1_43C9
        mov    edi, dword ptr [ebp + 0x14]
        or     edi, edi
        je     loc_1_36EC
        cmp    dword ptr [edi], 0xf
        jne    loc_1_36EC
        or     word ptr [ebp + 0x42], 0x1000
        mov    edi, dword ptr [edi + 4]
        call   sub_1_9171
        or     edx, edx
        je     loc_1_36EC
        push   edx
        mov    edi, dword ptr [ebp + 0xc]
        movzx  edi, word ptr [edi + 2]
        add    edi, dword ptr [ebp + 0x10]
        call   sub_1_9171
        mov    eax, edx
        pop    edx
        cmp    eax, edx
        je     loc_1_36EC
        mov    dword ptr [ebp + 0x48], edx
        or     word ptr [ebp + 0x42], 0x200
loc_1_36EC:
        test   byte ptr [ebp + 0x42], 0x20
        je     loc_1_36F8
        or     word ptr [ebp + 0x42], 0x2400
loc_1_36F8:
        mov    byte ptr [ebp + 0x45], 0x16
        mov    eax, dword ptr [ebp + 0x10]
        call   sub_1_91B8
        jae    loc_1_3711
        je     loc_1_3820
        jmp    loc_1_39D5
loc_1_3711:
        cmp    ebx, -1
        je     loc_1_3A28
        mov    esi, dword ptr [ebp + 0x68]
        mov    eax, dword ptr [ebp + 0x50]
        cmp    dword ptr [esi + 4], eax
        jne    loc_1_3A28
        mov    eax, dword ptr [ebp + 0x4c]
        cmp    eax, dword ptr [esi + 8]
        jne    loc_1_3A28
        test   byte ptr [ebp + 0x42], 0x20
        je     loc_1_3A28
        mov    esi, dword ptr [ebp + 0x28]
        cmp    word ptr [esi], 0
        je     loc_1_3A1A
        mov    edi, dword ptr [ebp + 0x6c]
        test   byte ptr [edi + 7], 2
        je     loc_1_37E2
        sub    eax, eax
        lea    edi, [edi + 0xb2]
        mov    ecx, 0x100
        repne scasw ax, word ptr es:[edi]
        jne    loc_1_3A28
        lea    eax, [edi - 2]
        mov    edi, dword ptr [ebp + 0x6c]
        movzx  ecx, word ptr [edi + 0xb0]
        lea    edi, [edi + 0xb2]
        sub    eax, edi
        cmp    ecx, eax
        jne    loc_1_37FE
        shr    ecx, 1
        repe cmpsw word ptr [esi], word ptr es:[edi]
        je     loc_1_3A1A
        lea    esi, [ebx]
        mov    edi, dword ptr [ebp + 0x68]
        lea    edi, [edi + 0xbb]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
        mov    esi, dword ptr [ebp + 0x68]
        mov    edi, dword ptr [ebp + 0x6c]
        lea    esi, [esi + 4]
        sub    eax, eax
        mov    edx, dword ptr [ebp + 0x28]
        movzx  ecx, word ptr [edi + 0xb0]
        pushfd 
        cmp    dword ptr [ebp + 0x64], 0
        je     loc_1_37CA
        push   esi
        mov    esi, dword ptr [ebp + 0x64]
        VxDCall  Dev_048B.Service_0007h          ; cd2007008b04
        pop    esi
loc_1_37CA:
        popfd  
        call   sub_1_3DC3
        pushfd 
        push   esi
        mov    esi, dword ptr [ebp + 0x64]
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
        pop    esi
        popfd  
        jmp    loc_1_397B
loc_1_37E2:
        movzx  ecx, word ptr [edi + 0x94]
        lea    edi, [edi + 0x96]
        sub    ecx, 2
        shr    ecx, 1
        repe cmpsw word ptr [esi], word ptr es:[edi]
        jne    loc_1_37FE
        jmp    loc_1_3A1A
loc_1_37FE:
        call   sub_1_0CDE
        jb     loc_1_39D5
        je     loc_1_38AB
        mov    esi, dword ptr [ebp + 0x6c]
        mov    ah, byte ptr [ebx]
        call   sub_1_9A38
        jae    loc_1_37FE
        jmp    loc_1_38AB
loc_1_3820:
        mov    esi, dword ptr [ebp + 0x68]
        mov    eax, dword ptr [ebp + 0x50]
        cmp    dword ptr [esi + 4], eax
        jne    loc_1_3845
        mov    edi, dword ptr [ebp + 0x6c]
        mov    eax, dword ptr [edi + 0x20]
        cmp    eax, dword ptr [esi + 8]
        jne    loc_1_38AB
        inc    eax
        je     loc_1_38AB
        test   byte ptr [edi + 8], 1
        jne    loc_1_38AB
        and    byte ptr [edi + 8], 0xf7
        jmp    loc_1_38AB
loc_1_3845:
        test   byte ptr [esi + 0x1d], 0x10
        je     loc_1_38AB
        test   byte ptr [ebp + 0x42], 0x20
        je     loc_1_3A28
        test   byte ptr [esi + 0xba], 1
        je     loc_1_3865
        cmp    word ptr [esi + 0x26], 0
        jne    loc_1_3870
loc_1_3865:
        cmp    word ptr [esi + 0x2c], 0
        je     loc_1_3A28
loc_1_3870:
        or     byte ptr [esi + 0xba], 2
        mov    edi, dword ptr [ebp + 0x6c]
        movzx  ecx, word ptr [esi + 0x32]
        cmp    word ptr [edi + 0x2b2], cx
        jb     loc_1_38AB
        cmp    ecx, 0x10
        jae    loc_1_3A28
        or     ecx, ecx
        je     loc_1_3A28
        lea    esi, [esi + 0x36]
        lea    edi, [edi + 0x2b6]
        add    ecx, ecx
        repe cmpsd dword ptr [esi], dword ptr es:[edi]
        je     loc_1_3A28
loc_1_38AB:
        test   byte ptr [ebp + 0x42], 0x20
        je     loc_1_38BE
        mov    esi, dword ptr [ebp + 0x28]
        cmp    word ptr [esi], 0
        je     loc_1_3A28
loc_1_38BE:
        mov    esi, dword ptr [ebp + 0x68]
        lea    ecx, [esi + 0x12]
        stc    
        call   sub_1_0D5B
        jb     loc_1_3A28
        mov    eax, dword ptr [ebp + 0x68]
        test   byte ptr [eax + 0xba], 4
        je     loc_1_38E3
        mov    edi, dword ptr [eax]
        call   sub_1_444E
loc_1_38E3:
        mov    edi, dword ptr [ebp + 0x68]
        mov    ecx, dword ptr [ebp + 0x50]
        mov    dword ptr [edi + 0xb6], ecx
        lea    esi, [ebx]
        lea    edi, [edi + 0xbb]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
        mov    esi, dword ptr [ebp + 0x68]
        mov    ebx, dword ptr [esi + 4]
        call   sub_1_1201
        mov    esi, dword ptr [ebp + 0x68]
        mov    eax, dword ptr [esi + 8]
        movzx  ecx, byte ptr [esi + 0x10]
        jecxz  loc_1_3916
        mov    eax, dword ptr [esi + 0xc]
loc_1_3916:
        inc    ecx
        call   sub_1_9A90
        mov    esi, dword ptr [ebp + 0x68]
        test   byte ptr [esi + 0xba], 2
        je     loc_1_397B
        xor    ebx, ebx
        test   byte ptr [esi + 0xba], 1
        je     loc_1_3936
        mov    ebx, dword ptr [esi + 0x24]
loc_1_3936:
        mov    bx, word ptr [esi + 0x2c]
        call   sub_1_1201
        mov    eax, 1
        call   sub_1_0BD7
        jb     loc_1_3A28
        mov    esi, dword ptr [ebp + 0x68]
        mov    eax, dword ptr [esi + 0xb6]
        mov    edi, dword ptr [ebp + 0x1c]
        test   byte ptr [edi + 0x52], 3
        jne    loc_1_3968
        cmp    dword ptr [edi + 0x3c], eax
        jne    loc_1_3968
        xor    eax, eax
loc_1_3968:
        mov    word ptr [ebx + 0x1a], ax
        shr    eax, 0x10
        mov    word ptr [ebx + 0x14], ax
        mov    esi, dword ptr [ebp + 0x64]
        call   sub_1_03EC
loc_1_397B:
        mov    esi, dword ptr [ebp + 0x68]
        test   byte ptr [esi + 0x1d], 0x10
        je     loc_1_399B
        sub    eax, eax
        mov    ebx, dword ptr [ebp + 0x1c]
        call   sub_1_0979
        sub    eax, eax
        push   eax
        push   eax
        push   eax
        push   eax
        call   sub_1_0AF3
        jmp    loc_1_39D3
loc_1_399B:
        test   byte ptr [esi + 0x1d], 8
        jne    loc_1_39BF
        mov    eax, dword ptr [esi + 4]
        xor    edx, edx
        cmp    byte ptr [esi + 0x10], 0
        je     loc_1_39AF
        mov    edx, dword ptr [esi + 0xc]
loc_1_39AF:
        mov    ebx, dword ptr [ebp + 0x1c]
        push   edx
        push   eax
        push   dword ptr [esi + 8]
        push   eax
        call   sub_1_0AF3
        jmp    loc_1_39D3
loc_1_39BF:
        mov    esi, dword ptr [ebp + 0x68]
        test   byte ptr [esi + 0x1d], 8
        je     loc_1_39D3
        lea    eax, [esi + 0xbb]
        call   sub_1_6E3C
loc_1_39D3:
        sub    eax, eax
loc_1_39D5:
        push   eax
        mov    edi, dword ptr [ebp + 0x68]
        mov    edi, dword ptr [edi]
        or     edi, edi
        je     loc_1_3A0C
        test   al, al
        jne    loc_1_39F4
        lea    edi, [edi + 0x45]
        mov    esi, dword ptr [ebp + 0x68]
        lea    esi, [esi + 0xbb]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
loc_1_39F4:
        mov    edi, dword ptr [ebp + 0x68]
        mov    edi, dword ptr [edi]
        call   sub_1_A040
        sub    dword ptr [edi + 0x2c], 0x10001
        jne    loc_1_3A0C
        call   sub_1_9FA9
loc_1_3A0C:
        mov    eax, dword ptr [ebp + 0x68]
        call   sub_1_A1F8
        pop    eax
        jmp    loc_1_454A
loc_1_3A1A:
        sub    eax, eax
        push   eax
        mov    edi, dword ptr [ebp + 0x68]
        mov    edi, dword ptr [edi]
        or     edi, edi
        jne    loc_1_39F4
        jmp    loc_1_3A0C
loc_1_3A28:
        mov    al, 5
        jmp    loc_1_39D5
loc_1_3A2C:
        mov    eax, 0x98
        call   sub_1_A1CE
        mov    dword ptr [ebp + 0x68], eax
        mov    edi, eax
        mov    al, 8
        jb     loc_1_4576
        mov    dword ptr [edi], 0
        mov    al, byte ptr [ebp]
        mov    byte ptr [ebp + 0x45], al
        mov    byte ptr [ebp + 0x46], 0
        mov    eax, 0
        mov    edx, dword ptr [ebp]
        call   sub_1_43C9
        mov    word ptr [edi + 4], ax
        mov    eax, dword ptr [ebp + 0xc]
        call   sub_1_91B8
        jb     loc_1_3D95
        cmp    ebx, -1
        je     loc_1_3D8F
        mov    edi, dword ptr [ebp + 0x68]
        mov    esi, dword ptr [ebp + 0x6c]
        mov    byte ptr [edi + 0x8d], 0
        mov    al, byte ptr [ebx + 0xb]
        mov    byte ptr [edi + 0x81], al
        mov    eax, dword ptr [ebp + 0x50]
        mov    dword ptr [edi + 0x74], eax
        mov    eax, dword ptr [esi + 0x10]
        mov    dword ptr [edi + 0x78], eax
        mov    eax, dword ptr [esi + 0xc]
        mov    dword ptr [edi + 0x7c], eax
        mov    al, byte ptr [esi + 4]
        mov    byte ptr [edi + 0x80], al
        mov    esi, dword ptr [ebp + 0x3e]
        movzx  ecx, word ptr [esi]
        shr    ecx, 1
        inc    ecx
        lea    edi, [edi + 8]
        rep movsw word ptr es:[edi], word ptr [esi]
        mov    esi, dword ptr [ebp + 0x6c]
        lea    esi, [esi + 0x66]
        mov    edi, dword ptr [ebp + 0x68]
        lea    edi, [edi + 0x40]
        mov    ecx, 0xb
        rep movsw word ptr es:[edi], word ptr [esi]
        mov    eax, 0
        mov    edx, dword ptr [ebp + 0x24]
        call   sub_1_43C9
        mov    edi, dword ptr [ebp + 0x68]
        mov    word ptr [edi + 6], ax
        mov    eax, dword ptr [ebp + 0x10]
        call   sub_1_91B8
        jae    loc_1_3AF3
        jne    loc_1_3D95
loc_1_3AF3:
        mov    eax, dword ptr [ebp + 0x50]
        mov    edi, dword ptr [ebp + 0x68]
        cmp    eax, dword ptr [edi + 0x74]
        jne    loc_1_3D8F
        mov    esi, dword ptr [ebp + 0x6c]
        lea    esi, [esi + 0x24]
        lea    edi, [edi + 0x56]
        mov    ecx, 0xb
        rep movsw word ptr es:[edi], word ptr [esi]
loc_1_3B13:
        mov    edx, dword ptr [ebp + 0x68]
        lea    esi, [edx + 0x56]
        lea    edi, [edx + 0x40]
        mov    ecx, 0xb
loc_1_3B21:
        lodsw  ax, word ptr [esi]
        cmp    ax, 0x3f
        jne    loc_1_3B2C
        mov    ax, word ptr [edi]
loc_1_3B2C:
        stosw  word ptr es:[edi], ax
        loop   loc_1_3B21
        lea    esi, [edx + 0x40]
        lea    edi, [edx + 0x26]
        push   edx
        push   0
        push   esi
        push   edi
        VxDCall  IFSMgr.FcbToShort               ; cd2048004000
        add    esp, 0xc
        pop    edx
        mov    word ptr [eax + edi], 0
        add    eax, 2
        mov    word ptr [edx + 0x24], ax
        mov    ebx, dword ptr [edx + 0x74]
        push   edx
        call   sub_1_1201
        pop    edx
        mov    byte ptr [ebp + 0x45], 0x16
        mov    ax, word ptr [edx + 6]
        or     ax, 0x10
        mov    word ptr [ebp + 0x42], ax
        lea    eax, [edx + 0x24]
        mov    ecx, 0
        push   edx
        call   sub_1_9366
        pop    edx
        jae    loc_1_3D8F
        or     eax, eax
        jne    loc_1_3D95
        mov    eax, dword ptr [edx + 0x78]
        call   sub_1_0BD7
        jb     loc_1_3D8F
        mov    edi, dword ptr [ebp + 0x68]
        mov    dword ptr [edi + 0x6c], ebx
        mov    dword ptr [edi + 0x70], edx
        mov    al, byte ptr [ebx + 0xb]
        mov    byte ptr [edi + 0x81], al
        lea    esi, [edi + 0x40]
        lea    edi, [edi + 0x82]
        pushfd 
        cmp    dword ptr [ebp + 0x64], 0
        je     loc_1_3BC4
        push   esi
        mov    esi, dword ptr [ebp + 0x64]
        VxDCall  Dev_048B.Service_0007h          ; cd2007008b04
        pop    esi
loc_1_3BC4:
        popfd  
        push   1
        push   0xb
        push   0x16
        push   esi
        push   edi
        VxDCall  IFSMgr.UniToBCS                 ; cd2040004000
        add    esp, 0x14
        pushfd 
        push   esi
        mov    esi, dword ptr [ebp + 0x64]
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
        pop    esi
        popfd  
        cmp    byte ptr [edi], 0xe5
        jne    loc_1_3BEB
        mov    byte ptr [edi], 5
loc_1_3BEB:
        mov    edx, dword ptr [ebp + 0x68]
        test   byte ptr [edx + 0x81], 8
        je     loc_1_3C0A
        lea    esi, [edx + 0x82]
        lea    edi, [edx + 0x8d]
        mov    ecx, 0xb
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_1_3C0A:
        test   byte ptr [edx + 0x81], 0x10
        jne    loc_1_3C4B
        mov    word ptr [ebp + 0x62], 0x12
        sub    eax, eax
        pushfd 
        cmp    dword ptr [ebp + 0x64], 0
        je     loc_1_3C2D
        push   esi
        mov    esi, dword ptr [ebp + 0x64]
        VxDCall  Dev_048B.Service_0007h          ; cd2007008b04
        pop    esi
loc_1_3C2D:
        popfd  
        call   sub_1_7FED
        pushfd 
        push   esi
        mov    esi, dword ptr [ebp + 0x64]
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
        pop    esi
        popfd  
        jb     loc_1_3D8F
        mov    edx, dword ptr [ebp + 0x68]
        mov    dword ptr [edx], eax
loc_1_3C4B:
        lea    esi, [edx + 0x74]
        lea    eax, [edx + 0x40]
        sub    edx, edx
        sub    ecx, ecx
        pushfd 
        cmp    dword ptr [ebp + 0x64], 0
        je     loc_1_3C67
        push   esi
        mov    esi, dword ptr [ebp + 0x64]
        VxDCall  Dev_048B.Service_0007h          ; cd2007008b04
        pop    esi
loc_1_3C67:
        popfd  
        call   sub_1_3DC3
        pushfd 
        push   esi
        mov    esi, dword ptr [ebp + 0x64]
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
        pop    esi
        popfd  
        jb     loc_1_3D95
        sub    eax, eax
        mov    ebx, dword ptr [ebp + 0x1c]
        push   eax
        push   eax
        push   eax
        push   eax
        call   sub_1_0AF3
        mov    edx, dword ptr [ebp + 0x68]
        test   byte ptr [edx + 0x81], 0x10
        je     loc_1_3CA6
        sub    eax, eax
        mov    ebx, dword ptr [ebp + 0x1c]
        call   sub_1_0979
        jmp    loc_1_3D10
loc_1_3CA6:
        mov    edx, dword ptr [ebp + 0x68]
        mov    edi, dword ptr [edx]
        lea    esi, [edx + 0x82]
        lea    edi, [edi + 0x45]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
        sub    edi, edi
        xchg   dword ptr [edx], edi
        pushfd 
        cmp    dword ptr [ebp + 0x64], 0
        je     loc_1_3CCF
        push   esi
        mov    esi, dword ptr [ebp + 0x64]
        VxDCall  Dev_048B.Service_0007h          ; cd2007008b04
        pop    esi
loc_1_3CCF:
        popfd  
        call   sub_1_A040
        pushfd 
        push   esi
        mov    esi, dword ptr [ebp + 0x64]
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
        pop    esi
        popfd  
        sub    dword ptr [edi + 0x2c], 0x10001
        jne    loc_1_3D10
        pushfd 
        cmp    dword ptr [ebp + 0x64], 0
        je     loc_1_3CFD
        push   esi
        mov    esi, dword ptr [ebp + 0x64]
        VxDCall  Dev_048B.Service_0007h          ; cd2007008b04
        pop    esi
loc_1_3CFD:
        popfd  
        call   sub_1_9FA9
        pushfd 
        push   esi
        mov    esi, dword ptr [ebp + 0x64]
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
        pop    esi
        popfd  
loc_1_3D10:
        mov    edx, dword ptr [ebp + 0x68]
        test   byte ptr [edx + 4], 1
        je     loc_1_3D74
        mov    al, byte ptr [ebp]
        mov    byte ptr [ebp + 0x45], al
        mov    ax, word ptr [edx + 4]
        or     ax, 0x10
        mov    word ptr [ebp + 0x42], ax
        lea    eax, [edx + 8]
        mov    ebx, dword ptr [edx + 0x6c]
        mov    edx, dword ptr [edx + 0x70]
        mov    ecx, 1
        call   sub_1_9366
        jae    loc_1_3D46
        or     eax, eax
        je     loc_1_3D74
        jmp    loc_1_3D95
loc_1_3D46:
        mov    esi, dword ptr [ebp + 0x6c]
        mov    edx, dword ptr [ebp + 0x68]
        mov    eax, dword ptr [esi + 0x10]
        mov    dword ptr [edx + 0x78], eax
        mov    eax, dword ptr [esi + 0xc]
        mov    dword ptr [edx + 0x7c], eax
        mov    al, byte ptr [esi + 4]
        mov    byte ptr [edx + 0x80], al
        lea    esi, [esi + 0x66]
        lea    edi, [edx + 0x40]
        mov    ecx, 0xb
        rep movsw word ptr es:[edi], word ptr [esi]
        jmp    loc_1_3B13
loc_1_3D74:
        mov    edx, dword ptr [ebp + 0x68]
        cmp    byte ptr [edx + 0x8d], 0
        je     loc_1_3D93
        lea    eax, [edx + 0x8d]
        call   sub_1_6E3C
        jae    loc_1_3D93
        jmp    loc_1_3D95
loc_1_3D8F:
        mov    al, 5
        jmp    loc_1_3D95
loc_1_3D93:
        sub    eax, eax
loc_1_3D95:
        push   eax
        mov    edx, dword ptr [ebp + 0x68]
        mov    ecx, dword ptr [edx]
        jecxz  loc_1_3DB2
        mov    edi, ecx
        call   sub_1_A040
        sub    dword ptr [edi + 0x2c], 0x10001
        jne    loc_1_3DB2
        call   sub_1_9FA9
loc_1_3DB2:
        xchg   dword ptr [ebp + 0x68], eax
        call   sub_1_A1F8
        xchg   dword ptr [ebp + 0x68], eax
        pop    eax
        jmp    loc_1_454A
sub_1_3DC3:
        sub    esp, 0x2c
        mov    dword ptr [esp + 0x1c], eax
        mov    dword ptr [esp + 0x20], edx
        mov    word ptr [esp + 0x1a], cx
        lea    edi, [esp]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
        mov    ebx, dword ptr [esp]
        call   sub_1_1201
        jb     loc_1_3F49
        mov    ecx, dword ptr [esp + 0x1c]
        jecxz  loc_1_3E1A
        lea    edx, [esp + 0xe]
        push   1
        push   0xb
        push   0x16
        push   ecx
        push   edx
        VxDCall  IFSMgr.UniToBCS                 ; cd2040004000
        add    esp, 0x14
        lea    esi, [esp + 0xe]
        cmp    byte ptr [esi], 0xe5
        jne    loc_1_3E0F
        mov    byte ptr [esi], 5
loc_1_3E0F:
        sub    al, al
        call   sub_1_8D88
        mov    byte ptr [esp + 0xd], al
loc_1_3E1A:
        mov    eax, dword ptr [esp + 8]
        cmp    eax, -1
        jne    loc_1_3E27
        mov    eax, dword ptr [esp + 4]
loc_1_3E27:
        call   sub_1_0BD7
        jb     loc_1_3F49
        cmp    dword ptr [esp + 0x1c], 0
        jne    loc_1_3E49
        mov    al, byte ptr [ebx + 0xb]
        and    al, 0x1f
        cmp    al, 0xf
        jne    loc_1_3E49
        mov    al, byte ptr [ebx + 0xd]
        mov    byte ptr [esp + 0xd], al
loc_1_3E49:
        cmp    dword ptr [esp + 0x20], 0
        je     loc_1_3ED9
        mov    dword ptr [esp + 0x24], ebx
        mov    dword ptr [esp + 0x28], edx
        sub    edx, edx
        movzx  eax, word ptr [esp + 0x1a]
        mov    ecx, 0x1a
        div    ecx
        mov    ecx, edx
        mov    edx, 0x1a
        mul    edx
        add    dword ptr [esp + 0x20], eax
        jecxz  loc_1_3ECC
        mov    eax, 0xffffffff
        lea    edi, [ebx + 1]
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        lea    edi, [edi + 3]
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        lea    edi, [edi + 2]
        stosd  dword ptr es:[edi], eax
        mov    esi, dword ptr [esp + 0x20]
        lea    edi, [ebx + 1]
        mov    eax, 0x20605
        mov    edx, 0x1c0e
        shr    ecx, 1
        inc    ecx
loc_1_3EA3:
        movsw  word ptr es:[edi], word ptr [esi]
        dec    al
        loopne loc_1_3EA3
        jecxz  loc_1_3EB9
        movzx  edi, dl
        lea    edi, [edi + ebx]
        shr    edx, 8
        shr    eax, 8
        jne    loc_1_3EA3
loc_1_3EB9:
        dec    byte ptr [esp + 0xc]
        sub    dword ptr [esp + 0x20], 0x1a
        mov    ebx, dword ptr [esp + 0x24]
        mov    edx, dword ptr [esp + 0x28]
        jmp    loc_1_3F07
loc_1_3ECC:
        sub    dword ptr [esp + 0x20], 0x1a
        mov    ebx, dword ptr [esp + 0x24]
        mov    edx, dword ptr [esp + 0x28]
loc_1_3ED9:
        cmp    byte ptr [esp + 0xc], 0
        je     loc_1_3F23
        dec    byte ptr [esp + 0xc]
        mov    esi, dword ptr [esp + 0x20]
        test   esi, esi
        jne    loc_1_3EF1
        mov    byte ptr [ebx], 0xe5
        jmp    loc_1_3F07
loc_1_3EF1:
        lea    edi, [ebx + 1]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        lea    edi, [edi + 3]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        lea    edi, [edi + 2]
        movsd  dword ptr es:[edi], dword ptr [esi]
        sub    dword ptr [esp + 0x20], 0x1a
loc_1_3F07:
        mov    al, byte ptr [esp + 0xd]
        mov    byte ptr [ebx + 0xd], al
        mov    esi, dword ptr [ebp + 0x64]
        push   ebx
        push   edx
        call   sub_1_03EC
        pop    edx
        pop    ebx
        call   sub_1_0CDE
        ja     loc_1_3ED9
        jmp    loc_1_3F49
loc_1_3F23:
        cmp    dword ptr [esp + 0x1c], 0
        je     loc_1_3F35
        lea    esi, [esp + 0xe]
        lea    edi, [ebx]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
loc_1_3F35:
        mov    esi, dword ptr [ebp + 0x64]
        call   sub_1_03EC
        jb     loc_1_3F49
        add    esp, 0x2c
        clc    
        ret    
        db  083h, 0C4h, 02Ch, 0F9h, 0C3h
loc_1_3F49:
        add    esp, 0x2c
        mov    al, 5
        stc    
        ret    
loc_1_3F50:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 0x14]
        cld    
        sub    eax, eax
        mov    byte ptr [ebp + 0x37], al
        mov    word ptr [ebp + 0x38], ax
        mov    byte ptr [ebp + 0x34], al
        mov    byte ptr [ebp + 0x35], al
        cmp    byte ptr [0xb786], 0          ; -> obj1:0xB786
        jne    loc_1_3F76
        call   sub_1_A26B
loc_1_3F76:
        mov    edi, dword ptr [ebp + 0x1c]
        cmp    byte ptr [ebp + 4], 2
        je     loc_1_40CD
        mov    esi, dword ptr [edi + 0x14]
        mov    edi, 0xb748                   ; -> loc_1_B748
loc_1_3F8B:
        mov    edi, dword ptr [edi]
        cmp    edi, 0xb748                   ; -> loc_1_B748
        je     loc_1_4089
        cmp    dword ptr [edi + 0x14], esi
        jne    loc_1_3F8B
        mov    eax, 0xb720                   ; -> obj1:0xB720
loc_1_3FA3:
        mov    eax, dword ptr [eax + 8]
        cmp    eax, 0xb720                   ; -> obj1:0xB720
        je     loc_1_3FBC
        cmp    dword ptr [eax + 0x28], edi
        jne    loc_1_3FA3
loc_1_3FB2:
        mov    eax, 4
        jmp    loc_1_4177
loc_1_3FBC:
        cmp    byte ptr [ebp + 4], 1
        jne    loc_1_3FDD
        pushfd 
        cli    
        mov    ecx, 0xb6fc                   ; -> obj1:0xB6FC
loc_1_3FC9:
        mov    ecx, dword ptr [ecx + 0x1c]
        cmp    ecx, 0xb6fc                   ; -> obj1:0xB6FC
        je     loc_1_3FDC
        cmp    dword ptr [ecx + 0xc], edi
        jne    loc_1_3FC9
        popfd  
        jmp    loc_1_3FB2
loc_1_3FDC:
        popfd  
loc_1_3FDD:
        push   esi
        push   edi
        xor    ecx, ecx
        xchg   dword ptr [edi + 0x9c], ecx
        jecxz  loc_1_3FFB
        cmp    dword ptr [0xbd70], 0         ; -> obj1:0xBD70
        jne    loc_1_3FF9
        xchg   ecx, eax
        call   sub_1_A1F8
        xchg   ecx, eax
loc_1_3FF9:
        sub    ecx, ecx
loc_1_3FFB:
        xchg   dword ptr [edi + 0x90], ecx
        jecxz  loc_1_4013
        cmp    dword ptr [0xbd70], 0         ; -> obj1:0xBD70
        jne    loc_1_4013
        xchg   ecx, eax
        call   sub_1_A1F8
        xchg   ecx, eax
loc_1_4013:
        cmp    byte ptr [ebp + 4], 1
        je     loc_1_403B
        mov    eax, dword ptr [esp]
        push   dword ptr [ebp + 0x1c]
        mov    dword ptr [ebp + 0x1c], eax
        bt     dword ptr [eax + 0x1f], 4
        jb     loc_1_402E
        call   sub_1_42C0
loc_1_402E:
        call   sub_1_05FA
        call   sub_1_1D22
        pop    dword ptr [ebp + 0x1c]
loc_1_403B:
        mov    ebx, dword ptr [esp]
        call   sub_1_A72F
        pop    edi
        pop    esi
        mov    edx, edi
        call   sub_1_58BA
        xchg   edi, edx
        jb     loc_1_3F8B
        test   byte ptr [edi + 0x52], 3
        je     loc_1_406D
        mov    eax, dword ptr [edi + 0x40]
        mov    word ptr [edx + 0x1f], ax
        mov    eax, dword ptr [edi + 0x38]
        mov    word ptr [edx + 0x1d], ax
        jmp    loc_1_3F8B
loc_1_406D:
        cmp    word ptr [edx + 0xf], 0
        jne    loc_1_3F8B
        mov    eax, dword ptr [edi + 0x40]
        mov    dword ptr [edx + 0x1f], eax
        mov    eax, dword ptr [edi + 0x38]
        mov    dword ptr [edx + 0x39], eax
        jmp    loc_1_3F8B
loc_1_4089:
        mov    edi, 0xb748                   ; -> loc_1_B748
loc_1_408E:
        mov    edi, dword ptr [edi]
        cmp    edi, 0xb748                   ; -> loc_1_B748
        je     loc_1_4175
        cmp    dword ptr [edi + 0x14], esi
        jne    loc_1_408E
        mov    ebx, dword ptr [edi]
        mov    ecx, dword ptr [edi + 4]
        mov    dword ptr [ebx + 4], ecx
        mov    dword ptr [ecx], ebx
        cmp    dword ptr [0xbd70], 0         ; -> obj1:0xBD70
        je     loc_1_40BB
        mov    ebx, edi
        call   sub_1_14FB
loc_1_40BB:
        cmp    dword ptr [0xbd70], 0         ; -> obj1:0xBD70
        jne    loc_1_40CB
        xchg   edi, eax
        call   sub_1_A1F8
        xchg   edi, eax
loc_1_40CB:
        jmp    loc_1_4089
loc_1_40CD:
        mov    eax, 0xb720                   ; -> obj1:0xB720
loc_1_40D2:
        mov    eax, dword ptr [eax + 8]
        cmp    eax, 0xb720                   ; -> obj1:0xB720
        je     loc_1_40EB
        cmp    dword ptr [eax + 0x28], edi
        jne    loc_1_40D2
        mov    eax, 4
        jmp    loc_1_4177
loc_1_40EB:
        push   edi
        xor    ecx, ecx
        xchg   dword ptr [edi + 0x9c], ecx
        jecxz  loc_1_4108
        cmp    dword ptr [0xbd70], 0         ; -> obj1:0xBD70
        jne    loc_1_4106
        xchg   ecx, eax
        call   sub_1_A1F8
        xchg   ecx, eax
loc_1_4106:
        sub    ecx, ecx
loc_1_4108:
        xchg   dword ptr [edi + 0x90], ecx
        jecxz  loc_1_4120
        cmp    dword ptr [0xbd70], 0         ; -> obj1:0xBD70
        jne    loc_1_4120
        xchg   ecx, eax
        call   sub_1_A1F8
        xchg   ecx, eax
loc_1_4120:
        mov    eax, dword ptr [esp]
        push   dword ptr [ebp + 0x1c]
        mov    dword ptr [ebp + 0x1c], eax
        bt     dword ptr [eax + 0x1f], 4
        jb     loc_1_4135
        call   sub_1_42C0
loc_1_4135:
        call   sub_1_05FA
        call   sub_1_1D22
        pop    dword ptr [ebp + 0x1c]
        mov    ebx, dword ptr [esp]
        call   sub_1_A72F
        pop    edi
        mov    ebx, dword ptr [edi]
        mov    ecx, dword ptr [edi + 4]
        mov    dword ptr [ebx + 4], ecx
        mov    dword ptr [ecx], ebx
        cmp    dword ptr [0xbd70], 0         ; -> obj1:0xBD70
        je     loc_1_4165
        mov    ebx, edi
        call   sub_1_14FB
loc_1_4165:
        cmp    dword ptr [0xbd70], 0         ; -> obj1:0xBD70
        jne    loc_1_4175
        xchg   edi, eax
        call   sub_1_A1F8
        xchg   edi, eax
loc_1_4175:
        sub    eax, eax
loc_1_4177:
        mov    word ptr [ebp + 0x1a], ax
        push   eax
        cmp    byte ptr [0xb786], 0          ; -> obj1:0xB786
        jne    loc_1_418A
        call   sub_1_A2CE
loc_1_418A:
        pop    eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_4190:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 0x14]
        cld    
        mov    ax, word ptr [ebp + 0x18]
        and    ah, 0x91
        mov    byte ptr [ebp + 0x37], ah
        sub    eax, eax
        mov    word ptr [ebp + 0x38], ax
        mov    byte ptr [ebp + 0x34], al
        mov    byte ptr [ebp + 0x35], al
        call   sub_1_A318
        mov    edi, dword ptr [ebp + 0x1c]
        cmp    byte ptr [ebp + 4], 1
        jne    loc_1_41D2
        mov    ebx, dword ptr [edi + 0x2c]
        dec    ebx
        mov    edx, dword ptr [edi + 0x40]
        cmp    edx, -1
        je     loc_1_41F4
        cmp    ebx, edx
        jae    loc_1_41F4
        or     edx, 0xffffffff
        jmp    loc_1_41F4
loc_1_41D2:
        call   sub_1_5BEF
        jb     loc_1_42B9
        test   byte ptr [edi + 0x52], 0x10
        je     loc_1_41F4
        call   sub_1_5B05
        cmp    eax, edx
        jae    loc_1_41F4
        mov    ecx, ebx
        sub    ebx, edx
        add    ebx, eax
        mov    edx, eax
loc_1_41F4:
        movzx  ecx, word ptr [edi + 0x20]
        movzx  eax, byte ptr [edi + 0x24]
        inc    eax
        test   byte ptr [ebp + 0x18], 1
        jne    loc_1_4253
        mov    dword ptr [ebp], ecx
        bsr    ecx, ebx
        sub    ecx, 0xf
        jbe    loc_1_423B
        mov    ch, 6
        sub    ch, byte ptr [edi + 0x25]
        cmp    cl, ch
        jbe    loc_1_4219
        mov    cl, ch
loc_1_4219:
        shl    eax, cl
        shr    ebx, cl
        shr    edx, cl
        cmp    ebx, 0x10000
        jb     loc_1_422D
        mov    ebx, dword ptr [0xb8a0]       ; -> obj1:0xB8A0
loc_1_422D:
        cmp    edx, 0x10000
        jb     loc_1_423B
        mov    edx, dword ptr [0xb8a4]       ; -> obj1:0xB8A4
loc_1_423B:
        mov    dword ptr [ebp + 0x24], ebx
        mov    dword ptr [ebp + 0x28], edx
loc_1_4241:
        mov    word ptr [ebp + 0x18], ax
        mov    cl, byte ptr [edi + 0xba]
        mov    byte ptr [ebp + 4], cl
        jmp    loc_1_4574
loc_1_4253:
        mov    esi, dword ptr [ebp + 0x14]
        cmp    dword ptr [ebp], 0x2c
        jb     loc_1_42B9
        mov    word ptr [esi], 0x2c
        mov    word ptr [esi + 2], 0
        mov    dword ptr [esi + 4], eax
        mov    dword ptr [esi + 8], ecx
        mov    dword ptr [esi + 0xc], edx
        mov    dword ptr [esi + 0x10], ebx
        mov    eax, dword ptr [edi + 0x2c]
        dec    eax
        mov    dword ptr [esi + 0x20], eax
        mov    cl, byte ptr [edi + 0x25]
        shl    eax, cl
        mov    dword ptr [esi + 0x18], eax
        mov    eax, dword ptr [edi + 0x40]
        mov    dword ptr [esi + 0x1c], eax
        shl    eax, cl
        mov    dword ptr [esi + 0x14], eax
        test   byte ptr [edi + 0x52], 0x10
        je     loc_1_42AE
        mov    eax, 0x29
        mov    ebx, edi
        mov    ecx, 0xdb0b
        push   esi
        call   sub_1_44AF
        pop    esi
        jb     loc_1_42AE
        mov    dword ptr [esi + 0x18], edx
        mov    dword ptr [esi + 0x14], ecx
loc_1_42AE:
        xor    eax, eax
        mov    dword ptr [esi + 0x24], eax
        mov    dword ptr [esi + 0x28], eax
        dec    eax
        jmp    loc_1_4241
loc_1_42B9:
        mov    al, 5
        jmp    loc_1_4576
sub_1_42C0:
        test   byte ptr [eax + 0x52], 3
        jne    loc_1_4315
        mov    ebx, dword ptr [eax + 0x14]
        test   byte ptr [ebx + 4], 0x40
        jne    loc_1_4315
        mov    ebx, dword ptr [eax + 0xa0]
        cmp    ebx, -1
        je     loc_1_4315
        call   sub_1_00D0
        jb     loc_1_4312
        add    ebx, 0x1e4
        cmp    dword ptr [ebx], 0x61417272
        jne    loc_1_4312
        xor    ecx, ecx
        mov    eax, dword ptr [edi + 0x40]
        cmp    dword ptr [ebx + 4], eax
        je     loc_1_42FD
        mov    dword ptr [ebx + 4], eax
        inc    ecx
loc_1_42FD:
        mov    eax, dword ptr [edi + 0x38]
        cmp    dword ptr [ebx + 8], eax
        je     loc_1_4309
        mov    dword ptr [ebx + 8], eax
        inc    ecx
loc_1_4309:
        jecxz  loc_1_4312
        xor    edx, edx
        call   sub_1_03F3
loc_1_4312:
        mov    eax, dword ptr [ebp + 0x1c]
loc_1_4315:
        ret    
sub_1_4316:
        mov    eax, dword ptr [ebp + 0x1c]
        mov    edx, 0xb720                   ; -> obj1:0xB720
loc_1_431E:
        mov    edx, dword ptr [edx + 8]
        cmp    edx, 0xb720                   ; -> obj1:0xB720
        je     loc_1_4340
        cmp    dword ptr [edx + 0x28], eax
        jne    loc_1_431E
        mov    ecx, dword ptr [edx]
        mov    ecx, dword ptr [ecx + 4]
loc_1_4333:
        mov    ecx, dword ptr [ecx]
        cmp    ecx, edx
        je     loc_1_431E
        test   byte ptr [ecx + 0xf], 2
        jne    loc_1_4333
        stc    
loc_1_4340:
        ret    
loc_1_4341:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 0x14]
        cld    
        sub    eax, eax
        mov    byte ptr [ebp + 0x37], al
        mov    word ptr [ebp + 0x38], ax
        mov    byte ptr [ebp + 0x34], al
        mov    byte ptr [ebp + 0x35], al
        test   byte ptr [ebp + 0x18], 2
        je     loc_1_436F
        call   sub_1_B02E
        jae    loc_1_4574
        jmp    loc_1_4576
loc_1_436F:
        call   sub_1_A26B
        mov    eax, dword ptr [ebp + 0x1c]
        call   sub_1_42C0
        call   sub_1_05FA
        call   sub_1_4316
        jb     loc_1_438D
        call   sub_1_1D22
loc_1_438D:
        mov    ebx, dword ptr [ebp + 0x1c]
        test   byte ptr [ebx + 0x52], 3
        je     loc_1_43AA
        mov    dword ptr [ebx + 0x38], 2
        mov    eax, dword ptr [ebx + 0x60]
        or     eax, eax
        je     loc_1_43AA
        mov    word ptr [eax + 0x1d], 2
loc_1_43AA:
        mov    eax, 0x27
        test   byte ptr [ebp + 0x18], 1
        je     loc_1_43BF
        call   sub_1_A72F
        mov    eax, 0x28
loc_1_43BF:
        call   sub_1_44AF
        jmp    loc_1_4574
sub_1_43C9:
        test   edx, 0xc0000000
        je     loc_1_43D6
        or     al, 1
        and    ah, 0x3f
loc_1_43D6:
        test   edx, 0x20000000
        je     loc_1_43E0
        or     al, 4
loc_1_43E0:
        test   edx, 0x14000000
        je     loc_1_43EA
        or     al, 0x20
loc_1_43EA:
        test   edx, 0x14000000
        jne    loc_1_43F5
        and    ah, 0xfb
loc_1_43F5:
        test   edx, 0x4000000
        jne    loc_1_43FF
        or     al, 8
loc_1_43FF:
        mov    edx, dword ptr [ebp + 0x1c]
        cmp    dword ptr [edx + 0x90], 0
        je     loc_1_4410
        test   ah, 0x28
        je     loc_1_4413
loc_1_4410:
        and    ah, 0x3f
loc_1_4413:
        mov    word ptr [ebp + 0x42], ax
        ret    
sub_1_4418:
        test   edx, 0x14000000
        jne    loc_1_444D
        push   ecx
        push   edi
        mov    edi, dword ptr [ebp + 0xc]
        movzx  ecx, word ptr [edi + 2]
        movzx  ecx, word ptr [ecx + edi]
        shr    ecx, 1
        je     loc_1_444B
        dec    ecx
        je     loc_1_444B
        mov    edi, dword ptr [ebp + 0x28]
loc_1_4437:
        cmp    word ptr [edi], 0x80
        jae    loc_1_4445
        lea    edi, [edi + 2]
        loop   loc_1_4437
        jmp    loc_1_444B
loc_1_4445:
        or     edx, 0x10000000
loc_1_444B:
        pop    edi
        pop    ecx
loc_1_444D:
        ret    
sub_1_444E:
        mov    edx, dword ptr [ebp + 0x1c]
        mov    esi, dword ptr [ebp + 0x64]
        cmp    edx, dword ptr [edi + 0x28]
        jne    loc_1_449C
        cmp    edx, dword ptr [esi + 0xc]
        jne    loc_1_449C
        bt     dword ptr [edi + 0x24], 3
        sbb    eax, eax
        and    eax, dword ptr [ebx + 0x12]
        mov    ax, word ptr [ebx + 0x1a]
        or     eax, eax
        je     loc_1_4475
        cmp    eax, dword ptr [edi + 0x10]
        jne    loc_1_449C
loc_1_4475:
        mov    eax, dword ptr [ebp + 0x50]
        mov    dword ptr [edi + 0x18], eax
        mov    eax, dword ptr [ebp + 0x4c]
        mov    dword ptr [edi + 0x14], eax
        mov    eax, ebx
        sub    eax, dword ptr [esi + 0x10]
        shr    eax, 5
        mov    byte ptr [edi + 0x25], al
        mov    cl, byte ptr [edx + 0x4c]
        sub    cl, 5
        shr    eax, cl
        add    eax, dword ptr [esi + 8]
        mov    dword ptr [edi + 0x1c], eax
        clc    
        ret    
loc_1_449C:
        stc    
        ret    
sub_1_449E:
        mov    eax, 0x29
        call   sub_1_44AF
        jb     loc_1_44AE
        cmp    edx, 1
        cmc    
loc_1_44AE:
        ret    
sub_1_44AF:
        push   eax
        push   edx
        mov    edx, ebx
        mov    eax, dword ptr [ebx + 0x14]
        movzx  ebx, word ptr [eax + 0xa]
        mov    esi, dword ptr [eax + 0xc]
        push   eax
        mov    ah, 1
        test   byte ptr [ebp + 0x37], 0x90
        jne    loc_1_44C8
        mov    ah, 2
loc_1_44C8:
        call   sub_1_A4E1
        jb     loc_1_452D
        mov    esi, ebx
        pop    dword ptr [ebx + 0x44]
        mov    al, byte ptr [edx + 0xac]
        mov    byte ptr [esi + 0x4d], al
        mov    dword ptr [esi + 8], 0x400
        mov    dword ptr [esi + 0xc], 0x4538 ; -> loc_1_4538
        pop    edx
        pop    eax
        mov    word ptr [esi + 4], ax
        cmp    eax, 0x29
        jne    loc_1_44FD
        mov    word ptr [esi + 0x2e], cx
        mov    dword ptr [esi + 0x30], edx
loc_1_44FD:
        VxDCall  BLOCKDEV.BlockDev_Send_Command  ; cd2004001000
loc_1_4503:
        cli    
        cmp    dword ptr [esi + 0xc], 0
        je     loc_1_4516
        mov    eax, 0xb6b4                   ; -> obj1:0xB6B4
        call   sub_1_A220
        jmp    loc_1_4503
loc_1_4516:
        sti    
        mov    ecx, dword ptr [esi + 0x30]
        mov    edx, dword ptr [esi + 0x3c]
        movzx  eax, word ptr [esi + 6]
        cmp    eax, 0x10
        cmc    
        mov    ebx, esi
        call   sub_1_A4AE
loc_1_452C:
        ret    
loc_1_452D:
        add    esp, 0xc
        mov    eax, 0x1d
        stc    
        jmp    loc_1_452C
loc_1_4538:
        and    dword ptr [esi + 0xc], 0
        mov    eax, 0xb6b4                   ; -> obj1:0xB6B4
        jmp    sub_1_A258
        db  08Bh, 0C0h
loc_1_4548:
        sub    eax, eax
loc_1_454A:
        test   byte ptr [ebp + 0x39], 0x20
        je     loc_1_4576
        sub    ecx, ecx
        xchg   dword ptr [ebp + 0x6c], ecx
        jecxz  loc_1_4576
        xchg   dword ptr [0xb844], ecx       ; -> obj1:0xB844
        jecxz  loc_1_4576
        xchg   ecx, eax
        call   sub_1_A1F8
        xchg   ecx, eax
        jmp    loc_1_4576
loc_1_4568:
        call   sub_1_9EB2
loc_1_456D:
        mov    al, 6
        jmp    loc_1_4576
        db  02Eh, 08Bh, 0C0h
loc_1_4574:
        sub    eax, eax
loc_1_4576:
        movzx  eax, al
loc_1_4579:
        mov    word ptr [ebp + 0x1a], ax
        test   byte ptr [ebp + 0x37], 0x11
        jne    loc_1_459D
        test   byte ptr [ebp + 0x38], 4
        je     loc_1_459D
        xor    ecx, ecx
        xchg   byte ptr [ebp + 0x34], cl
        push   ecx
        call   sub_1_0636
        pop    ecx
        mov    byte ptr [ebp + 0x34], cl
        jae    loc_1_459D
        mov    byte ptr [ebp + 0x34], al
loc_1_459D:
        mov    ebx, dword ptr [ebp + 0x1c]
        btr    dword ptr [ebx + 0x1f], 6
        jb     loc_1_45BE
        movzx  eax, byte ptr [ebp + 0x34]
        or     eax, eax
        je     loc_1_45CF
        mov    edi, 0x22f0                   ; -> loc_1_22F0
        mov    ecx, 4
        cld    
        repne scasb al, byte ptr es:[edi]
        jne    loc_1_45CF
loc_1_45BE:
        test   byte ptr [ebp + 0x37], 0x11
        je     loc_1_45CA
        or     byte ptr [ebx + 0x1f], 0x40
        jmp    loc_1_45CF
loc_1_45CA:
        call   sub_1_1D38
loc_1_45CF:
        movzx  eax, word ptr [ebp + 0x1a]
        bt     dword ptr [ebp + 0x38], 0xf
        jae    loc_1_4604
        or     eax, eax
        jne    loc_1_4604
        mov    edx, dword ptr [ebp + 0x1c]
        test   byte ptr [edx + 0x52], 3
        jne    loc_1_4604
        mov    ecx, dword ptr [edx + 0x14]
        test   byte ptr [ecx + 1], 0x40
        je     loc_1_4604
        test   byte ptr [ecx + 2], 0x80
        jne    loc_1_4604
        push   eax
        mov    eax, edx
        call   sub_1_42C0
        call   sub_1_1D22
        pop    eax
loc_1_4604:
        cmp    dword ptr [0xb8b0], 0         ; -> obj1:0xB8B0
        je     loc_1_4628
        test   byte ptr [ebp + 0x37], 0x90
        je     loc_1_461F
        test   dword ptr [0xb8ac], 0xffffff  ; -> obj1:0xB8AC
        jne    loc_1_4628
loc_1_461F:
        call   sub_1_A2CE
        jmp    loc_1_462D
        db  08Bh, 0C0h
loc_1_4628:
        call   sub_1_A361
loc_1_462D:
        movzx  ecx, byte ptr [ebp + 0x34]
        jecxz  loc_1_463D
        VxDCall  BLOCKDEV.Service_000Fh          ; cd200f001000
loc_1_4639:
        mov    word ptr [ebp + 0x1a], ax
loc_1_463D:
        movzx  ebx, byte ptr [ebp + 0x35]
        or     bl, bl
        je     loc_1_4655
        mov    edi, dword ptr [ebp + 0x1c]
        btr    dword ptr [edi + 0x1c], ebx
        cmp    dword ptr [0xb6f4], 0         ; -> obj1:0xB6F4
        jne    loc_1_465A
loc_1_4655:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_465A:
        push   eax
        mov    eax, 0xb6f4                   ; -> obj1:0xB6F4
        call   sub_1_A258
        pop    eax
        jmp    loc_1_4655
loc_1_4668:
        mov    al, 0x32
        jmp    loc_1_4576
        db  000h
loc_1_4670:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 0x14]
        cld    
        sub    eax, eax
        mov    byte ptr [ebp + 0x37], al
        mov    word ptr [ebp + 0x38], ax
        mov    byte ptr [ebp + 0x34], al
        mov    byte ptr [ebp + 0x35], al
        call   sub_1_A318
        mov    edx, dword ptr [ebp + 0x20]
        test   byte ptr [edx + 0xe], 0x10
        jne    loc_1_456D
        mov    edx, dword ptr [edx + 8]
        lea    edx, [edx + 0x64]
        movzx  eax, word ptr [ebp + 0x18]
        test   byte ptr [ebp + 4], 1
        jne    loc_1_46C6
        push   eax
        push   dword ptr [ebp + 0x20]
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp + 0x28]
        push   dword ptr [ebp + 0x24]
        push   edx
        VxDCall  IFSMgr.IFSMgr_LockFile          ; cd2053004000
        add    esp, 0x18
        jmp    loc_1_454A
loc_1_46C6:
        push   eax
        push   dword ptr [ebp + 0x20]
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp + 0x28]
        push   dword ptr [ebp + 0x24]
        push   edx
        VxDCall  IFSMgr.IFSMgr_UnlockFile        ; cd2054004000
        add    esp, 0x18
        jmp    loc_1_454A
loc_1_46E2:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 0x14]
        cld    
        sub    eax, eax
        mov    byte ptr [ebp + 0x37], al
        mov    word ptr [ebp + 0x38], ax
        mov    byte ptr [ebp + 0x34], al
        mov    byte ptr [ebp + 0x35], al
        call   sub_1_A318
        mov    edi, dword ptr [ebp + 0x20]
        test   byte ptr [edi + 0xe], 0x10
        jne    loc_1_456D
        mov    esi, dword ptr [edi + 8]
        cmp    byte ptr [ebp + 4], 7
        je     loc_1_47E3
        cmp    byte ptr [ebp + 4], 6
        je     loc_1_476C
        cmp    byte ptr [ebp + 4], 5
        je     loc_1_47B4
        cmp    byte ptr [ebp + 4], 4
        je     loc_1_4752
        cmp    byte ptr [ebp + 4], 1
        je     loc_1_4797
        cmp    byte ptr [ebp + 4], 0
        jne    loc_1_4833
        mov    eax, dword ptr [esi + 0x38]
        mov    word ptr [ebp + 0x28], ax
        shr    eax, 0x10
        mov    word ptr [ebp + 0x2a], ax
        jmp    loc_1_4574
loc_1_4752:
        sub    edx, edx
        movzx  eax, word ptr [esi + 0x3c]
        call   sub_1_13B5
        mov    word ptr [ebp + 0x28], 0
        mov    word ptr [ebp + 0x2a], ax
        jmp    loc_1_4574
loc_1_476C:
        sub    edx, edx
        mov    eax, dword ptr [esi + 0x3e]
        call   sub_1_13D1
        mov    word ptr [ebp + 0x28], ax
        shr    eax, 0x10
        mov    word ptr [ebp + 0x2a], ax
        movzx  eax, byte ptr [esi + 0x42]
        cmp    eax, 0xc8
        sbb    edx, edx
        and    eax, edx
        mov    word ptr [ebp + 0x18], ax
        jmp    loc_1_4574
loc_1_4797:
        mov    ax, word ptr [ebp + 0x2a]
        shl    eax, 0x10
        mov    ax, word ptr [ebp + 0x28]
        mov    dword ptr [esi + 0x38], eax
        and    word ptr [edi + 0xe], 0xfff3
        or     word ptr [edi + 0xe], 0x40
        jmp    loc_1_4574
loc_1_47B4:
        mov    eax, dword ptr [esi + 0x28]
        test   word ptr [eax + 0x52], 0x100
        jne    loc_1_482C
        sub    edx, edx
        movzx  eax, word ptr [ebp + 0x2a]
        or     eax, eax
        je     loc_1_47D0
        call   sub_1_13B5
        jb     loc_1_4825
loc_1_47D0:
        mov    word ptr [esi + 0x3c], ax
        and    word ptr [edi + 0xe], 0xfffb
        or     word ptr [edi + 0xe], 0x20
        jmp    loc_1_4574
loc_1_47E3:
        mov    eax, dword ptr [esi + 0x28]
        test   word ptr [eax + 0x52], 0x100
        jne    loc_1_482C
        mov    ax, word ptr [ebp + 0x2a]
        shl    eax, 0x10
        mov    ax, word ptr [ebp + 0x28]
        sub    edx, edx
        or     eax, eax
        je     loc_1_4815
        mov    edx, dword ptr [esi + 0x3e]
        call   sub_1_13D1
        jb     loc_1_4825
        movzx  edx, word ptr [ebp + 0x18]
        cmp    edx, 0xc8
        jae    loc_1_4825
loc_1_4815:
        mov    byte ptr [esi + 0x42], dl
        mov    dword ptr [esi + 0x3e], eax
        and    word ptr [edi + 0xe], 0xfffb
        jmp    loc_1_4574
loc_1_4825:
        mov    al, 0xd
        jmp    loc_1_4576
loc_1_482C:
        mov    al, 5
        jmp    loc_1_4576
loc_1_4833:
        mov    al, 1
        jmp    loc_1_4576
loc_1_483A:
        pushfd 
        inc    dword ptr [0xb768]            ; -> loc_1_B768
        popfd  
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 0x14]
        cld    
        mov    ax, word ptr [ebp + 0x18]
        and    ah, 0x91
        mov    byte ptr [ebp + 0x37], ah
        sub    eax, eax
        mov    word ptr [ebp + 0x38], ax
        mov    byte ptr [ebp + 0x34], al
        mov    byte ptr [ebp + 0x35], al
        mov    esi, dword ptr [ebp + 0x20]
        mov    esi, dword ptr [esi + 8]
        test   byte ptr [esi + 0x43], 8
        jne    loc_1_49D7
        mov    byte ptr [ebp + 0x3a], 0
        clc    
        call   sub_1_9DFC
        call   sub_1_A318
        mov    esi, dword ptr [ebp + 0x20]
        test   byte ptr [esi + 0xe], 0x10
        jne    loc_1_4568
        mov    edx, dword ptr [esi + 8]
        test   byte ptr [ebp + 0x37], 0x90
        jne    loc_1_4969
        test   byte ptr [edx + 0x23], 0x81
        je     loc_1_48A7
        mov    edi, edx
        call   sub_1_A02F
loc_1_48A7:
        inc    dword ptr [edx + 0x20]
loc_1_48AA:
        mov    al, byte ptr [esi + 0xc]
        and    al, 3
        cmp    al, 1
        je     loc_1_498C
        mov    ebx, dword ptr [ebp + 0x24]
        mov    eax, dword ptr [edx + 0x30]
        sub    eax, ebx
        ja     loc_1_48C6
        mov    ebx, dword ptr [edx + 0x30]
        sub    eax, eax
loc_1_48C6:
        mov    dword ptr [ebp + 0x3c], ebx
        mov    ecx, dword ptr [ebp]
        jecxz  loc_1_4933
        cmp    eax, ecx
        jae    loc_1_48D9
        mov    dword ptr [ebp], eax
        or     eax, eax
        je     loc_1_4933
loc_1_48D9:
        cmp    dword ptr [edx + 0x64], 0
        je     loc_1_490D
        mov    eax, 1
        mov    ecx, dword ptr [ebp]
        mov    edx, ebx
        add    ecx, edx
        mov    edi, esi
        call   sub_1_9D98
        mov    al, 0x21
        jb     loc_1_498E
        test   byte ptr [ebp + 0x37], 0x10
        je     loc_1_490D
        cmp    ebx, dword ptr [0xb6bc]       ; -> obj1:0xB6BC
        jne    loc_1_490D
        jmp    loc_1_49B7
loc_1_490D:
        call   sub_1_6FE4
        jb     loc_1_498C
        test   byte ptr [ebp + 0x37], 0x10
        jne    loc_1_4949
        test   byte ptr [ebp + 0x37], 1
        jne    loc_1_4933
        test   word ptr [ebp + 0x18], 0x2000
        jne    loc_1_4933
        mov    eax, dword ptr [ebp + 0x24]
        mov    edx, dword ptr [ebp]
        call   sub_1_0685
loc_1_4933:
        mov    esi, dword ptr [ebp + 0x20]
        mov    edi, dword ptr [esi + 8]
        test   byte ptr [ebp + 0x37], 0x90
        jne    loc_1_4949
        dec    dword ptr [edi + 0x20]
        jns    loc_1_4949
        call   sub_1_A046
loc_1_4949:
        mov    ecx, dword ptr [ebp]
        jecxz  loc_1_495F
        pushfd 
        add    dword ptr [0xb760], ecx       ; -> loc_1_B760
        popfd  
        add    ecx, dword ptr [ebp + 0x3c]
        mov    dword ptr [ebp + 0x24], ecx
        mov    dword ptr [ebp + 0x3c], ecx
loc_1_495F:
        call   sub_1_9EB2
        jmp    loc_1_4574
loc_1_4969:
        test   byte ptr [ebp + 0x37], 0x10
        jne    loc_1_48AA
        test   byte ptr [edx + 0x24], 2
        jne    loc_1_48AA
        call   sub_1_7EEA
        jae    loc_1_48AA
        mov    al, 0x1f
        jmp    loc_1_498E
loc_1_498C:
        mov    al, 5
loc_1_498E:
        mov    dword ptr [ebp], 0
        mov    esi, dword ptr [ebp + 0x20]
        mov    edi, dword ptr [esi + 8]
        push   eax
        test   byte ptr [ebp + 0x37], 0x90
        jne    loc_1_49AC
        dec    dword ptr [edi + 0x20]
        jns    loc_1_49AC
        call   sub_1_A046
loc_1_49AC:
        call   sub_1_9EB2
        pop    eax
        jmp    loc_1_4576
loc_1_49B7:
        push   edi
        push   ecx
        mov    ecx, dword ptr [0xb6c0]       ; -> obj1:0xB6C0
        mov    edi, dword ptr [ebp + 0x14]
        mov    esi, dword ptr [ecx + 0x10]
        mov    ecx, 0x1000
        call   sub_1_823A
        pop    ecx
        pop    edi
        pop    esi
        jmp    loc_1_4949
loc_1_49D7:
        mov    dword ptr [ebp], 0
        mov    eax, 5
        jmp    loc_1_4639
loc_1_49E8:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 0x14]
        cld    
        mov    ax, word ptr [ebp + 0x18]
        and    ah, 0x91
        mov    byte ptr [ebp + 0x37], ah
        sub    eax, eax
        mov    word ptr [ebp + 0x38], ax
        mov    byte ptr [ebp + 0x34], al
        mov    byte ptr [ebp + 0x35], al
        mov    esi, dword ptr [ebp + 0x20]
        mov    esi, dword ptr [esi + 8]
        test   byte ptr [esi + 0x43], 8
        jne    loc_1_4F7A
        mov    byte ptr [ebp + 0x3a], 0
        stc    
        call   sub_1_9DFC
        call   sub_1_A318
        pushfd 
        inc    dword ptr [0xb76c]            ; -> loc_1_B76C
        popfd  
        mov    esi, dword ptr [ebp + 0x20]
        test   byte ptr [esi + 0xe], 0x10
        jne    loc_1_4568
        mov    edx, dword ptr [ebp]
        mov    eax, dword ptr [ebp + 0x24]
        add    edx, eax
        jb     loc_1_4F1F
        jns    loc_1_4A55
        test   byte ptr [esi + 0xf], 1
        je     loc_1_4F1F
loc_1_4A55:
        mov    dword ptr [ebp + 0x3c], eax
        mov    edx, dword ptr [esi + 8]
        test   byte ptr [edx + 0x24], 4
        je     loc_1_4A67
        or     word ptr [ebp + 0x38], 0x800
loc_1_4A67:
        mov    eax, dword ptr [edx + 0x28]
        cmp    dword ptr [ebp + 0x1c], eax
        test   byte ptr [ebp + 0x37], 1
        je     loc_1_4A89
        movzx  ecx, word ptr [eax + 0x22]
        test   dword ptr [ebp + 0x24], ecx
        jne    loc_1_4F1F
        test   dword ptr [ebp], ecx
        jne    loc_1_4F1F
loc_1_4A89:
        mov    edi, edx
        test   byte ptr [ebp + 0x37], 0x90
        jne    loc_1_4A9A
        call   sub_1_9FEC
        or     byte ptr [edi + 0x23], 1
loc_1_4A9A:
        mov    al, byte ptr [esi + 0xc]
        and    al, 3
        cmp    al, 0
        je     loc_1_4F1F
        cmp    dword ptr [edx + 0x64], 0
        je     loc_1_4ACC
        mov    eax, 0
        mov    ecx, dword ptr [ebp]
        mov    edx, dword ptr [ebp + 0x24]
        add    ecx, edx
        mov    edi, esi
        call   sub_1_9D98
        mov    edx, dword ptr [esi + 8]
        mov    al, 0x21
        jb     loc_1_4F21
loc_1_4ACC:
        mov    edi, dword ptr [ebp + 0x1c]
        mov    ecx, dword ptr [edi + 0x14]
        test   dword ptr [ecx + 4], 0x40
        mov    al, 0x13
        jne    loc_1_4F21
        test   byte ptr [ebp + 0x37], 0x10
        jne    loc_1_4B2D
        test   dword ptr [ecx + 4], 0x8000
        jne    loc_1_4B00
        test   word ptr [edi + 0x52], 0x8000
        jne    loc_1_4B00
        test   word ptr [esi + 0xe], 0x81
        je     loc_1_4B09
loc_1_4B00:
        push   edx
        call   sub_1_5CB0
        pop    edx
        jmp    loc_1_4B2D
loc_1_4B09:
        test   byte ptr [edi + 0x52], 0x10
        je     loc_1_4B2D
        movzx  ecx, word ptr [edi + 0x4a]
        mov    eax, dword ptr [ebp + 0x24]
        and    eax, ecx
        add    eax, dword ptr [ebp]
        add    eax, ecx
        mov    cl, byte ptr [edi + 0x47]
        shr    eax, cl
        push   dword ptr [ebp + 0x3c]
        call   sub_1_5B8D
        pop    dword ptr [ebp + 0x3c]
loc_1_4B2D:
        mov    eax, dword ptr [ebp + 0x3c]
        cmp    eax, dword ptr [edx + 0x30]
        setae  bl
        or     byte ptr [ebp + 0x3a], bl
        add    eax, dword ptr [ebp]
        jb     loc_1_4F1F
        je     loc_1_4BC8
        movzx  ebx, word ptr [edi + 0x4a]
        mov    cl, byte ptr [edi + 0x47]
        dec    cl
        add    eax, ebx
        rcr    eax, 1
        shr    eax, cl
        add    ebx, dword ptr [edx + 0x30]
        rcr    ebx, 1
        shr    ebx, cl
        mov    ecx, eax
        sub    ecx, ebx
        jbe    loc_1_4BC8
        test   byte ptr [ebp + 0x37], 0x10
        je     loc_1_4B77
        xchg   edx, edi
        call   sub_1_9FEC
        or     byte ptr [edi + 0x23], 1
        xchg   edx, edi
loc_1_4B77:
        dec    ebx
        mov    dword ptr [ebp + 0x40], ebx
        mov    ebx, dword ptr [edx + 0x10]
        and    ebx, ebx
        je     loc_1_4BA5
        mov    ebx, dword ptr [ebp + 0x40]
        cmp    dword ptr [edx + 0x50], ebx
        ja     loc_1_4B98
        xchg   ebx, eax
        push   ecx
        call   sub_1_2165
        pop    ecx
        jb     loc_1_4D73
loc_1_4B98:
        push   ecx
        call   sub_1_7F50
        pop    ecx
        jb     loc_1_4D73
loc_1_4BA5:
        mov    esi, dword ptr [esi + 8]
        call   sub_1_7816
        jb     loc_1_4F21
        mov    esi, dword ptr [ebp + 0x20]
        and    word ptr [esi + 0xe], 0xfdff
        mov    edx, dword ptr [esi + 8]
        cmp    dword ptr [edx + 0x10], 0
        jne    loc_1_4BC8
        mov    dword ptr [edx + 0x10], ebx
loc_1_4BC8:
        and    dword ptr [esi + 0xc], 0xffb3fffb
        test   byte ptr [ebp + 0x37], 0x10
        je     loc_1_4BDB
        mov    eax, dword ptr [ebp + 0x24]
        mov    dword ptr [ebp + 0x3c], eax
loc_1_4BDB:
        cmp    dword ptr [ebp], 0
        je     loc_1_4DB5
        mov    eax, dword ptr [ebp + 0x3c]
        add    eax, dword ptr [ebp]
        cmp    eax, dword ptr [edx + 0x30]
        jbe    loc_1_4BF6
        xchg   dword ptr [edx + 0x30], eax
        mov    dword ptr [ebp + 0x70], eax
loc_1_4BF6:
        test   byte ptr [ebp + 0x37], 0x10
        je     loc_1_4C5C
        cmp    dword ptr [0xb6bc], -1        ; -> obj1:0xB6BC
        je     loc_1_4C4C
        mov    edx, dword ptr [ebp + 0x24]
        cmp    edx, dword ptr [0xb6bc]       ; -> obj1:0xB6BC
        ja     loc_1_4C4C
        add    edx, dword ptr [ebp]
        cmp    edx, dword ptr [0xb6bc]       ; -> obj1:0xB6BC
        jbe    loc_1_4C4C
        mov    dword ptr [0xb6bc], 0xffffffff ; -> obj1:0xB6BC
        cmp    dword ptr [0xb6b8], 2         ; -> obj1:0xB6B8
        jne    loc_1_4C4C
        pushal 
        mov    esi, dword ptr [0xb6c0]       ; -> obj1:0xB6C0
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
        VxDCall  Dev_048B.Service_0005h          ; cd2005008b04
        mov    dword ptr [0xb6b8], 0         ; -> obj1:0xB6B8
        popal  
loc_1_4C4C:
        call   sub_1_70A6
        jb     loc_1_4F1F
        jmp    loc_1_4CF7
loc_1_4C5C:
        test   byte ptr [ebp + 0x37], 1
        jne    loc_1_4C83
        mov    ecx, dword ptr [ebp]
        cmp    ecx, 0x1000
        jbe    loc_1_4C7F
        shr    ecx, 0xc
        inc    ecx
        mov    ah, byte ptr [0xb6f8]         ; -> obj1:0xB6F8
        VxDCall  Dev_048B.Service_0003h          ; cd2003008b04
        jb     loc_1_4C83
loc_1_4C7F:
        or     byte ptr [ebp + 0x3a], 2
loc_1_4C83:
        mov    esi, dword ptr [ebp + 0x20]
        call   sub_1_774E
        jb     loc_1_4F1F
        mov    esi, dword ptr [ebp + 0x20]
        call   sub_1_7704
        mov    esi, dword ptr [ebp + 0x20]
        cmp    dword ptr [ebp + 0x44], 0
        je     loc_1_4CB4
        movzx  ebx, word ptr [ebp + 0x3c]
        call   sub_1_762A
        jb     loc_1_4D7A
        mov    esi, dword ptr [ebp + 0x20]
loc_1_4CB4:
        cmp    dword ptr [ebp + 0x48], 0
        je     loc_1_4CE2
loc_1_4CBA:
        mov    esi, dword ptr [ebp + 0x20]
        mov    ecx, dword ptr [ebp + 0x48]
        call   sub_1_76B7
        jb     loc_1_4F1F
        sub    dword ptr [ebp + 0x48], ecx
        call   sub_1_57B5
        jb     loc_1_4D7A
        mov    esi, dword ptr [ebp + 0x20]
        cmp    dword ptr [ebp + 0x48], 0
        jne    loc_1_4CBA
loc_1_4CE2:
        mov    ecx, dword ptr [ebp + 0x4c]
        jecxz  loc_1_4CF7
        mov    dword ptr [ebp + 0x44], ecx
        sub    ebx, ebx
        call   sub_1_762A
        jb     loc_1_4D7A
loc_1_4CF7:
        mov    esi, dword ptr [ebp + 0x20]
        test   byte ptr [ebp + 0x37], 0x10
        je     loc_1_4D0B
        test   byte ptr [ebp + 0x3a], 1
        je     loc_1_4D4D
        jmp    loc_1_4F3C
loc_1_4D0B:
        test   byte ptr [ebp + 0x37], 1
        jne    loc_1_4D4D
        test   word ptr [ebp + 0x38], 4
        je     loc_1_4D4D
        push   esi
        push   edi
        call   sub_1_0636
        pop    edi
        pop    esi
        jae    loc_1_4D29
        mov    byte ptr [ebp + 0x34], al
        jmp    loc_1_4D7A
loc_1_4D29:
        test   word ptr [esi + 0xe], 1
        je     loc_1_4D4D
        push   dword ptr [ebp + 0x38]
        or     word ptr [ebp + 0x38], 0x800
        call   sub_1_1226
        pop    esi
        mov    word ptr [ebp + 0x38], si
        jb     loc_1_4F21
        mov    esi, dword ptr [ebp + 0x20]
loc_1_4D4D:
        mov    edi, dword ptr [esi + 8]
        call   sub_1_A01A
        mov    ecx, dword ptr [ebp]
        pushfd 
        add    dword ptr [0xb764], ecx       ; -> loc_1_B764
        popfd  
        add    ecx, dword ptr [ebp + 0x3c]
        mov    dword ptr [ebp + 0x24], ecx
        mov    dword ptr [ebp + 0x3c], ecx
        call   sub_1_9EB2
        jmp    loc_1_4574
loc_1_4D73:
        mov    al, 8
        jmp    loc_1_4F21
loc_1_4D7A:
        mov    esi, dword ptr [ebp + 0x20]
        test   byte ptr [ebp + 0x3a], 1
        je     loc_1_4F1F
        mov    eax, dword ptr [edi + 0x14]
        test   dword ptr [eax + 4], 0x80
        je     loc_1_4D9B
        test   word ptr [edi + 0x52], 0x10
        jne    loc_1_4DA5
loc_1_4D9B:
        cmp    byte ptr [ebp + 0x34], 0x2b
        jne    loc_1_4F1F
loc_1_4DA5:
        xor    eax, eax
        mov    dword ptr [ebp], eax
        mov    byte ptr [ebp + 0x34], al
        xchg   dword ptr [ebp + 0x70], eax
        mov    dword ptr [ebp + 0x3c], eax
        jmp    loc_1_4DB5
loc_1_4DB5:
        mov    edi, dword ptr [esi + 8]
        cmp    dword ptr [edi + 0x64], 0
        je     loc_1_4DDE
        mov    eax, 0
        mov    ecx, dword ptr [edi + 0x30]
        mov    edx, dword ptr [ebp + 0x3c]
        cmp    ecx, edx
        jbe    loc_1_4DDE
        push   edi
        mov    edi, esi
        call   sub_1_9D98
        pop    edi
        mov    al, 0x21
        jb     loc_1_4F21
loc_1_4DDE:
        and    word ptr [esi + 0xe], 0xfdff
        mov    edx, dword ptr [esi + 8]
        mov    edi, dword ptr [ebp + 0x1c]
        mov    eax, dword ptr [ebp + 0x3c]
        sub    eax, 1
        jb     loc_1_4EC8
        mov    dword ptr [ebp + 0x3c], eax
        mov    cl, byte ptr [edi + 0x47]
        shr    eax, cl
        inc    eax
        cmp    dword ptr [edx + 0x50], eax
        jbe    loc_1_4E1C
        mov    ecx, edx
        mov    edx, eax
        lea    ebx, [eax - 1]
        call   sub_1_2276
        mov    dword ptr [ecx + 0x5c], eax
        sub    ebx, dword ptr [eax + 4]
        inc    ebx
        mov    dword ptr [eax], ebx
        mov    dword ptr [ecx + 0x50], edx
loc_1_4E1C:
        mov    esi, dword ptr [ebp + 0x20]
        call   sub_1_774E
        jb     loc_1_4E3A
        mov    esi, dword ptr [ebp + 0x20]
        call   sub_1_7F50
        jb     loc_1_4E3A
        mov    edx, 0xffffffff
        call   sub_1_7D52
loc_1_4E3A:
        mov    al, 5
        jb     loc_1_4F21
        sub    eax, eax
        mov    esi, dword ptr [ebp + 0x20]
        test   byte ptr [ebp + 0x37], 0x10
        je     loc_1_4E52
        mov    ebx, dword ptr [ebp + 0x24]
        jmp    loc_1_4E56
loc_1_4E52:
        mov    ebx, dword ptr [ebp + 0x3c]
        inc    ebx
loc_1_4E56:
        mov    esi, dword ptr [esi + 8]
        mov    dword ptr [esi + 0x30], ebx
        test   byte ptr [ebp + 0x37], 0x10
        je     loc_1_4EC6
        cmp    dword ptr [0xb6bc], -1        ; -> obj1:0xB6BC
        je     loc_1_4EA4
        cmp    dword ptr [0xb6bc], ebx       ; -> obj1:0xB6BC
        jb     loc_1_4EA4
        mov    dword ptr [0xb6bc], 0xffffffff ; -> obj1:0xB6BC
        cmp    dword ptr [0xb6b8], 2         ; -> obj1:0xB6B8
        jne    loc_1_4EA4
        pushal 
        mov    esi, dword ptr [0xb6c0]       ; -> obj1:0xB6C0
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
        VxDCall  Dev_048B.Service_0005h          ; cd2005008b04
        mov    dword ptr [0xb6b8], 0         ; -> obj1:0xB6B8
        popal  
loc_1_4EA4:
        mov    esi, dword ptr [ebp + 0x20]
        push   esi
        and    word ptr [esi + 0xe], 0xfffb
        mov    byte ptr [ebp + 0x36], 0x41
        call   sub_1_1226
        pop    esi
        jb     loc_1_4F21
        mov    esi, dword ptr [esi + 8]
        mov    eax, dword ptr [esi + 0x28]
        call   sub_1_05FA
        sub    eax, eax
loc_1_4EC6:
        jmp    loc_1_4F21
loc_1_4EC8:
        mov    esi, dword ptr [esi + 8]
        mov    ebx, dword ptr [esi + 0x10]
        and    dword ptr [esi + 0x50], 0
        mov    eax, dword ptr [esi + 0x54]
        sub    eax, 8
        mov    dword ptr [esi + 0x5c], eax
        push   esi
        call   sub_1_7D50
        mov    al, 5
        jb     loc_1_4EE7
        sub    eax, eax
loc_1_4EE7:
        pop    esi
        sub    ebx, ebx
        mov    dword ptr [esi + 0x10], ebx
        mov    dword ptr [esi + 0x30], ebx
        test   byte ptr [ebp + 0x37], 0x10
        je     loc_1_4F21
        or     eax, eax
        jne    loc_1_4F21
        mov    byte ptr [ebp + 0x36], 0x41
        mov    esi, dword ptr [ebp + 0x20]
        and    word ptr [esi + 0xe], 0xfffb
        call   sub_1_1226
        jb     loc_1_4F21
        mov    esi, dword ptr [ebp + 0x20]
        mov    esi, dword ptr [esi + 8]
        mov    eax, dword ptr [esi + 0x28]
        call   sub_1_05FA
        sub    eax, eax
        jmp    loc_1_4F21
loc_1_4F1F:
        mov    al, 5
loc_1_4F21:
        and    dword ptr [ebp], 0
        mov    esi, dword ptr [ebp + 0x20]
        mov    edi, dword ptr [esi + 8]
        push   eax
        call   sub_1_A01A
        call   sub_1_9EB2
        pop    eax
        jmp    loc_1_4576
loc_1_4F3C:
        mov    esi, dword ptr [ebp + 0x20]
        and    word ptr [esi + 0xe], 0xfffb
        mov    byte ptr [ebp + 0x36], 0x41
        push   esi
        call   sub_1_1226
        pop    esi
        jb     loc_1_4F74
        sub    eax, eax
loc_1_4F53:
        push   eax
        mov    edi, dword ptr [esi + 8]
        mov    eax, dword ptr [edi + 0x28]
        push   esi
        call   sub_1_05FA
        pop    esi
        mov    edi, dword ptr [esi + 8]
        call   sub_1_A01A
        call   sub_1_9EB2
        pop    eax
        jmp    loc_1_4576
loc_1_4F74:
        and    dword ptr [ebp], 0
        jmp    loc_1_4F53
loc_1_4F7A:
        and    dword ptr [ebp], 0
        mov    eax, 5
        jmp    loc_1_4639
loc_1_4F88:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 0x14]
        cld    
        mov    ax, word ptr [ebp + 0x18]
        and    ah, 0x91
        mov    byte ptr [ebp + 0x37], ah
        sub    eax, eax
        mov    word ptr [ebp + 0x38], 0x800
        mov    byte ptr [ebp + 0x34], al
        mov    byte ptr [ebp + 0x35], al
        call   sub_1_A318
        mov    esi, dword ptr [ebp + 0x20]
        movzx  eax, word ptr [esi + 0xe]
        and    eax, 0x20c
        cmp    eax, 0x20c
        sete   al
        dec    al
        movsx  eax, al
        and    eax, 0x8000
        or     word ptr [ebp + 0x38], ax
        test   word ptr [ebp + 0x18], 1
        jne    loc_1_4FF3
        test   word ptr [esi + 0xe], 0x81
        je     loc_1_4FE7
        call   sub_1_5CB0
        jmp    loc_1_4FF3
loc_1_4FE7:
        call   sub_1_5B85
        jb     loc_1_4FF3
        call   sub_1_5CFF
loc_1_4FF3:
        test   byte ptr [esi + 0xe], 0x10
        jne    loc_1_5107
        mov    edx, dword ptr [esi + 8]
        mov    eax, dword ptr [edx + 0x28]
        cmp    dword ptr [ebp + 0x1c], eax
        test   byte ptr [edx + 0x23], 0x81
        je     loc_1_5013
        mov    edi, edx
        call   sub_1_A02F
loc_1_5013:
        inc    dword ptr [edx + 0x20]
        test   word ptr [ebp + 0x18], 1
        jne    loc_1_5042
        call   sub_1_1226
        jb     loc_1_5044
        mov    eax, dword ptr [ebp + 0x1c]
        test   word ptr [eax + 0x52], 0x400
        je     loc_1_5042
        call   sub_1_05FA
        mov    ebx, dword ptr [ebp + 0x1c]
        mov    eax, 0x27
        call   sub_1_44AF
loc_1_5042:
        xor    eax, eax
loc_1_5044:
        mov    ebx, dword ptr [ebp + 0x20]
        mov    edi, dword ptr [ebx + 8]
        push   eax
        cmp    byte ptr [ebp + 4], 0
        je     loc_1_5078
        test   word ptr [ebp + 0x18], 2
        jne    loc_1_5078
        lea    eax, [edi + 0x64]
        cmp    dword ptr [eax], 0
        je     loc_1_5078
        sub    edx, edx
        cmp    byte ptr [ebp + 4], 2
        je     loc_1_506C
        mov    edx, dword ptr [ebp + 8]
loc_1_506C:
        push   ebx
        push   edx
        push   eax
        VxDCall  IFSMgr.IFSMgr_RemoveLocks       ; cd2055004000
        add    esp, 0xc
loc_1_5078:
        dec    dword ptr [edi + 0x20]
        jns    loc_1_5082
        call   sub_1_A046
loc_1_5082:
        cmp    byte ptr [ebp + 4], 2
        jne    loc_1_5101
        mov    ax, word ptr [ebx + 0xc]
        and    eax, 7
        mov    eax, dword ptr [eax*4 + 0xb7d0] ; -> obj1:0xB7D0
        sub    dword ptr [edi + 0x2c], eax
        je     loc_1_50B8
        mov    ebx, dword ptr [ebp + 0x20]
        mov    eax, dword ptr [ebx]
        mov    ecx, dword ptr [ebx + 4]
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [ecx], eax
        test   word ptr [ebp + 0x18], 2
        je     loc_1_50DB
        mov    eax, dword ptr [edi + 0x64]
        mov    dword ptr [ebp + 0x24], eax
        jmp    loc_1_50DB
loc_1_50B8:
        test   word ptr [ebp + 0x18], 2
        je     loc_1_50D4
        mov    eax, dword ptr [edi + 0x64]
        mov    dword ptr [ebp + 0x24], eax
        or     eax, eax
        je     loc_1_50D4
        push   eax
        VxDCall  IFSMgr.IFSMgr_UnassignLockList  ; cd2059004000
        add    esp, 4
loc_1_50D4:
        xor    ebx, ebx
        call   sub_1_9F5C
loc_1_50DB:
        mov    ebx, dword ptr [ebp + 0x20]
        cmp    ebx, dword ptr [0xb758]       ; -> loc_1_B758
        jne    loc_1_50FA
        mov    dword ptr [0xb758], 0         ; -> loc_1_B758
        mov    dword ptr [0xb6bc], 0xffffffff ; -> obj1:0xB6BC
loc_1_50FA:
        xchg   ebx, eax
        call   sub_1_A1F8
        xchg   ebx, eax
loc_1_5101:
        pop    eax
        jmp    loc_1_4576
loc_1_5107:
        push   0
        cmp    byte ptr [ebp + 4], 2
        jne    loc_1_5101
        jmp    loc_1_50DB
loc_1_5111:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 0x14]
        cld    
        sub    eax, eax
        mov    byte ptr [ebp + 0x37], al
        mov    word ptr [ebp + 0x38], 0x800
        mov    byte ptr [ebp + 0x34], al
        mov    byte ptr [ebp + 0x35], al
        call   sub_1_A318
        mov    esi, dword ptr [ebp + 0x20]
        test   byte ptr [esi + 0xe], 0x10
        jne    loc_1_456D
        test   word ptr [ebp + 0x18], 4
        jne    loc_1_5168
        call   sub_1_5CB0
        mov    esi, dword ptr [ebp + 0x20]
        mov    edx, dword ptr [esi + 8]
        mov    eax, dword ptr [edx + 0x28]
        call   sub_1_1226
        jb     loc_1_4576
        call   sub_1_A060
        jmp    loc_1_4574
loc_1_5168:
        call   sub_1_5B85
        jb     loc_1_5174
        call   sub_1_5CFF
loc_1_5174:
        mov    edx, dword ptr [esi + 8]
        mov    eax, dword ptr [edx + 0x28]
        call   sub_1_1226
        jb     loc_1_4576
        mov    eax, dword ptr [ebp + 0x1c]
        call   sub_1_05C6
        jmp    loc_1_4574
loc_1_5192:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 0x14]
        cld    
        mov    ax, word ptr [ebp + 0x18]
        and    ah, 0x91
        mov    byte ptr [ebp + 0x37], ah
        sub    eax, eax
        mov    word ptr [ebp + 0x38], ax
        mov    byte ptr [ebp + 0x34], al
        mov    byte ptr [ebp + 0x35], al
        call   sub_1_A318
        mov    edx, dword ptr [ebp + 0x20]
        test   byte ptr [edx + 0xe], 0x10
        jne    loc_1_456D
        mov    ecx, dword ptr [ebp + 0x24]
        cmp    byte ptr [ebp + 4], 0
        je     loc_1_51EB
        cmp    byte ptr [ebp + 4], 2
        mov    al, 1
        jne    loc_1_4576
        mov    esi, dword ptr [edx + 8]
        test   byte ptr [esi + 0x23], 0x81
        je     loc_1_51E8
        mov    edi, esi
        call   sub_1_A02F
loc_1_51E8:
        add    ecx, dword ptr [esi + 0x30]
loc_1_51EB:
        mov    dword ptr [ebp + 0x3c], ecx
        mov    dword ptr [ebp + 0x24], ecx
        jmp    loc_1_4574
loc_1_51F6:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 0x14]
        cmp    byte ptr [ebp + 4], 2
        je     loc_1_5268
        cmp    byte ptr [ebp + 4], 3
        je     loc_1_5336
        test   byte ptr [ebp + 0x18], 1
        jne    loc_1_521D
        mov    eax, dword ptr [ebp + 0x1c]
        test   byte ptr [eax + 0x52], 3
        je     loc_1_5229
loc_1_521D:
        cmp    byte ptr [ebp + 4], 0
        je     loc_1_5239
        cmp    byte ptr [ebp + 4], 1
        je     loc_1_5232
loc_1_5229:
        mov    ax, 1
        jmp    loc_1_4639
loc_1_5232:
        call   sub_1_569A
        jmp    loc_1_523E
loc_1_5239:
        call   sub_1_560D
loc_1_523E:
        or     al, al
        je     loc_1_5247
        call   sub_1_5AEA
loc_1_5247:
        movzx  ecx, byte ptr [ebp + 0x34]
        or     ecx, ecx
        je     loc_1_4579
        mov    eax, ecx
        mov    byte ptr [ebp + 0x34], 0
        movzx  ecx, byte ptr [ebp + 4]
        VxDCall  BLOCKDEV.Service_000Eh          ; cd200e001000
        jmp    loc_1_4579
loc_1_5268:
        cld    
        sub    eax, eax
        mov    byte ptr [ebp + 0x37], al
        mov    word ptr [ebp + 0x38], ax
        mov    byte ptr [ebp + 0x34], al
        mov    byte ptr [ebp + 0x35], al
        call   sub_1_A318
        mov    ebx, dword ptr [ebp + 0x1c]
        mov    edx, dword ptr [ebx + 0x14]
        mov    ax, word ptr [ebx + 0x52]
        movzx  ecx, byte ptr [ebx + 0xac]
        test   word ptr [ebp + 0x18], 5
        jne    loc_1_52A5
        test   word ptr [ebp + 0x18], 0xa
        jne    loc_1_52C2
        mov    al, 1
        jmp    loc_1_4576
loc_1_52A5:
        push   eax
        call   sub_1_825B
        pop    eax
        or     ax, 0x2500
        test   dword ptr [edx + 4], 0x80
        je     loc_1_531B
        or     ax, word ptr [0xbd78]         ; -> obj1:0xBD78
        jmp    loc_1_531B
loc_1_52C2:
        or     byte ptr [ebx + 0x1f], 0x30
        and    ax, 0x5aff
        test   dword ptr [edx + 4], 0x80
        je     loc_1_52DA
        or     ax, word ptr [0xbd78]         ; -> obj1:0xBD78
loc_1_52DA:
        test   dword ptr [0xb86c], 0x80000000 ; -> obj1:0xB86C
        jne    loc_1_52F1
        bt     dword ptr [0xb86c], ecx       ; -> obj1:0xB86C
        jae    loc_1_5301
        jmp    loc_1_5305
loc_1_52F1:
        test   dword ptr [edx], 0x4000
        je     loc_1_5305
        test   dword ptr [edx], 0x10000
        jne    loc_1_5305
loc_1_5301:
        or     ax, 0x400
loc_1_5305:
        test   dword ptr [edx + 4], 0x40
        jne    loc_1_5317
        bt     dword ptr [0xb868], ecx       ; -> obj1:0xB868
        jae    loc_1_531B
loc_1_5317:
        or     ax, 0x100
loc_1_531B:
        mov    word ptr [ebx + 0x52], ax
        sub    eax, eax
        call   sub_1_0979
        sub    eax, eax
        push   eax
        push   eax
        push   eax
        push   eax
        call   sub_1_0AF3
        jmp    loc_1_4548
loc_1_5336:
        mov    ebx, dword ptr [ebp + 0x14]
        cmp    word ptr [ebx + 2], 0
        ja     loc_1_5354
        mov    edx, dword ptr [ebx + 4]
        cmp    edx, 4
        ja     loc_1_5354
        mov    word ptr [ebx], 0x18
        jmp    loc_1_5360
loc_1_5354:
        mov    ax, 1
        jmp    loc_1_4639
        db  02Eh, 08Bh, 0C0h
loc_1_5360:
        je     0x53b5
        add    byte ptr [eax], al
        int3   
        push   ebx
        add    byte ptr [eax], al
        int3   
        push   ebx
        add    byte ptr [eax], al
        rcl    dword ptr [ebx], cl
        add    ah, dl                        ; -> loc_1_54D4
        push   esp
        add    byte ptr [eax], al
loc_1_5374:
        cld    
        sub    eax, eax
        mov    byte ptr [ebp + 0x37], al
        mov    word ptr [ebp + 0x38], ax
        mov    byte ptr [ebp + 0x34], al
        mov    byte ptr [ebp + 0x35], al
        call   sub_1_A318
        mov    edi, dword ptr [ebp + 0x1c]
        mov    eax, dword ptr [ebx + 0xc]
        cmp    eax, -1
        jne    loc_1_5399
        add    eax, 3
        jmp    loc_1_53A3
loc_1_5399:
        cmp    eax, 2
        jb     loc_1_53A6
        cmp    dword ptr [edi + 0x2c], eax
        jb     loc_1_53A6
loc_1_53A3:
        mov    dword ptr [edi + 0x38], eax
loc_1_53A6:
        mov    eax, dword ptr [ebx + 8]
        or     eax, eax
        je     loc_1_4574
        cmp    eax, -1
        je     loc_1_53BF
        cmp    dword ptr [edi + 0x2c], eax
        jbe    loc_1_4574
loc_1_53BF:
        mov    dword ptr [edi + 0x40], eax
        call   sub_1_5BCE
        jmp    loc_1_4574
loc_1_53CC:
        xor    eax, eax
        jmp    loc_1_4639
loc_1_53D3:
        cld    
        sub    eax, eax
        mov    byte ptr [ebp + 0x37], al
        mov    word ptr [ebp + 0x38], ax
        mov    byte ptr [ebp + 0x34], al
        mov    byte ptr [ebp + 0x35], al
        mov    edi, dword ptr [ebp + 0x1c]
        mov    ax, 5
        test   byte ptr [edi + 0x52], 3
        jne    loc_1_4639
        call   sub_1_A26B
        movzx  eax, byte ptr [edi + 0x48]
        test   byte ptr [edi + 0x52], 4
        je     loc_1_5408
        or     eax, 0x80
loc_1_5408:
        mov    dword ptr [ebx + 0xc], eax
        mov    eax, dword ptr [ebx + 8]
        cmp    eax, -1
        je     loc_1_4574
        test   eax, 0xffffff70
        jne    loc_1_54CD
        mov    ah, al
        and    al, 0xf
        cmp    byte ptr [edi + 0x46], al
        jbe    loc_1_54CD
        test   ah, 0x80
        jne    loc_1_543C
        or     al, al
        jne    loc_1_54CD
loc_1_543C:
        bt     dword ptr [edi + 0x52], 2
        sbb    dl, dl
        xor    dl, ah
        cmp    byte ptr [edi + 0x48], al
        jne    loc_1_5455
        test   dl, 0x80
        je     loc_1_4574
        jmp    loc_1_5480
loc_1_5455:
        mov    dword ptr [edi + 0x38], 2
        or     dword ptr [edi + 0x40], 0xffffffff
        push   eax
        push   edx
        call   sub_1_5BCE
        mov    eax, edi
        call   sub_1_05FA
        mov    ebx, dword ptr [ebp + 0x1c]
        call   sub_1_A72F
        pop    edx
        pop    eax
        mov    edi, dword ptr [ebp + 0x1c]
        test   dl, 0x80
        je     loc_1_5490
loc_1_5480:
        btc    dword ptr [edi + 0x52], 2
        jae    loc_1_5490
        push   eax
        call   sub_1_5574
        pop    eax
        jb     loc_1_54C6
loc_1_5490:
        mov    byte ptr [edi + 0x48], al
        xor    ebx, ebx
        call   sub_1_00D0
        jb     loc_1_54C6
        cmp    byte ptr [ebx + 0x42], 0x29
        jne    loc_1_54C6
        movzx  eax, byte ptr [edi + 0x48]
        test   byte ptr [edi + 0x52], 4
        je     loc_1_54B1
        or     eax, 0x80
loc_1_54B1:
        and    word ptr [ebx + 0x28], 0xff70
        or     word ptr [ebx + 0x28], ax
        call   sub_1_55D2
        jae    loc_1_4574
loc_1_54C6:
        mov    al, 5
        jmp    loc_1_4576
loc_1_54CD:
        mov    al, 0x57
        jmp    loc_1_4576
loc_1_54D4:
        cld    
        sub    eax, eax
        mov    byte ptr [ebp + 0x37], al
        mov    word ptr [ebp + 0x38], ax
        mov    byte ptr [ebp + 0x34], al
        mov    byte ptr [ebp + 0x35], al
        mov    edi, dword ptr [ebp + 0x1c]
        mov    ax, 5
        test   byte ptr [edi + 0x52], 3
        jne    loc_1_4639
        call   sub_1_A26B
        mov    eax, dword ptr [edi + 0x3c]
        mov    dword ptr [ebx + 0xc], eax
        mov    ebx, dword ptr [ebx + 8]
        cmp    ebx, -1
        je     loc_1_4574
        mov    al, 0xd
        cmp    dword ptr [edi + 0x2c], ebx
        jb     loc_1_4576
        cmp    ebx, 2
        jb     loc_1_4576
        push   ebx
        call   sub_1_1F2C
        pop    eax
        jbe    loc_1_4576
        xchg   dword ptr [edi + 0x3c], eax
        mov    ebx, edi
        push   eax
        call   sub_1_0979
        pop    eax
        xor    edx, edx
        push   edx
        push   edx
        push   edx
        push   eax
        call   sub_1_0AF3
        xor    ebx, ebx
        call   sub_1_00D0
        jb     loc_1_556D
        cmp    byte ptr [ebx + 0x42], 0x29
        jne    loc_1_556D
        mov    eax, dword ptr [edi + 0x3c]
        cmp    dword ptr [ebx + 0x2c], eax
        je     loc_1_4574
        mov    dword ptr [ebx + 0x2c], eax
        call   sub_1_55D2
        jae    loc_1_4574
loc_1_556D:
        mov    al, 5
        jmp    loc_1_4576
sub_1_5574:
        mov    eax, dword ptr [ebp + 0x1c]
        call   sub_1_05FA
        mov    edi, dword ptr [ebp + 0x1c]
        mov    byte ptr [edi + 0xbc], 0
        or     word ptr [ebp + 0x38], 0x4000
        mov    ebx, 2
loc_1_5591:
        push   ebx
        call   sub_1_1F2C
        mov    edx, ebx
        pop    ebx
        jb     loc_1_55C9
        push   ebx
        call   sub_1_1D4C
        pop    ebx
        jb     loc_1_55C9
        movzx  eax, word ptr [edi + 0x20]
        shr    eax, 2
        add    ebx, eax
        neg    eax
        and    ebx, eax
        cmp    dword ptr [edi + 0x2c], ebx
        jae    loc_1_5591
        mov    eax, edi
        call   sub_1_05FA
        mov    edi, dword ptr [ebp + 0x1c]
        cmp    byte ptr [edi + 0xbc], 0x10
        cmc    
loc_1_55C9:
        pushfd 
        and    word ptr [ebp + 0x38], 0xbfff
        popfd  
        ret    
sub_1_55D2:
        mov    edi, dword ptr [esi + 0xc]
        test   byte ptr [edi + 0x52], 3
        jne    loc_1_5606
        cmp    dword ptr [edi + 0xa4], -1
        je     loc_1_5606
        VxDCall  Dev_048B.Service_0007h          ; cd2007008b04
        push   ebx
        push   esi
        mov    esi, ebx
        mov    ebx, dword ptr [edi + 0xa4]
        mov    ecx, 1
        call   sub_1_1574
        pop    esi
        pop    ebx
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
loc_1_5606:
        mov    dl, 1
        jmp    sub_1_03F3
sub_1_560D:
        cld    
        sub    eax, eax
        mov    byte ptr [ebp + 0x37], al
        mov    word ptr [ebp + 0x38], 0x1400
        mov    byte ptr [ebp + 0x34], al
        mov    byte ptr [ebp + 0x35], al
        call   sub_1_A318
        mov    edi, dword ptr [ebp + 0x1c]
        movzx  ecx, byte ptr [edi + 0x4c]
        movzx  edx, word ptr [edi + 0x22]
        ror    edx, cl
        mov    eax, dword ptr [ebp]
        test   edx, eax
        jne    loc_1_5695
        or     eax, eax
        je     loc_1_5694
        mov    edx, dword ptr [ebp + 0x24]
        add    edx, eax
        jb     loc_1_5695
        cmp    dword ptr [edi + 8], edx
        jb     loc_1_5695
        shl    eax, cl
        mov    dword ptr [ebp], eax
        clc    
        call   sub_1_9DFC
        call   sub_1_5E7C
        movzx  ecx, byte ptr [edi + 0x4c]
        mov    eax, dword ptr [ebp]
        shr    eax, cl
        mov    ecx, eax
        xor    eax, eax
        mov    edx, eax
        mov    dword ptr [ebp + 0x6c], eax
        mov    ebx, dword ptr [ebp + 0x24]
        call   sub_1_70F6
        pushfd 
        push   eax
        jb     loc_1_5689
        mov    eax, dword ptr [ebp]
        pushfd 
        add    dword ptr [0xb760], eax       ; -> loc_1_B760
        popfd  
        pushfd 
        inc    dword ptr [0xb768]            ; -> loc_1_B768
        popfd  
loc_1_5689:
        call   sub_1_9EB2
        pop    eax
        popfd  
        sbb    edx, edx
        and    eax, edx
loc_1_5694:
        ret    
loc_1_5695:
        mov    ax, 0x1b
        ret    
sub_1_569A:
        cld    
        sub    eax, eax
        mov    byte ptr [ebp + 0x37], al
        mov    word ptr [ebp + 0x38], 0x1400
        mov    byte ptr [ebp + 0x34], al
        mov    byte ptr [ebp + 0x35], al
        call   sub_1_A26B
        push   0
        push   0
        call   sub_1_5CFF
        mov    byte ptr [ebp + 0x3a], 8
        movzx  eax, word ptr [ebp + 0x18]
        test   al, 1
        je     loc_1_56D4
        and    ah, 0xe0
        cmp    ah, 0x40
        jne    loc_1_56D4
        or     word ptr [ebp + 0x38], 0x800
loc_1_56D4:
        mov    edi, dword ptr [ebp + 0x1c]
        mov    ecx, dword ptr [edi + 0x14]
        mov    eax, 0x1d
        test   dword ptr [ecx + 4], 0x40
        jne    loc_1_57AF
        movzx  ecx, byte ptr [edi + 0x4c]
        movzx  edx, word ptr [edi + 0x22]
        ror    edx, cl
        mov    eax, dword ptr [ebp]
        or     eax, eax
        je     loc_1_57A4
        test   edx, eax
        jne    loc_1_57A9
        mov    edx, dword ptr [ebp + 0x24]
        add    edx, eax
        jb     loc_1_57A9
        cmp    dword ptr [edi + 8], edx
        jb     loc_1_57A9
        shl    eax, cl
        mov    dword ptr [ebp], eax
        stc    
        call   sub_1_9DFC
        movzx  ecx, byte ptr [edi + 0x4c]
        mov    eax, dword ptr [ebp]
        shr    eax, cl
        mov    ecx, eax
        mov    ebx, dword ptr [ebp + 0x24]
        mov    edx, dword ptr [edi + 0x30]
        movzx  eax, byte ptr [edi + 0x46]
        imul   edx, eax
        movzx  eax, word ptr [edi + 0x26]
        add    edx, eax
        cmp    ebx, edx
        jae    loc_1_5764
        inc    dword ptr [esp]
        or     dword ptr [edi + 0x40], 0xffffffff
        push   ebx
        push   ecx
        push   edi
        call   sub_1_5BCE
        mov    eax, dword ptr [ebp + 0x1c]
        call   sub_1_05FA
        pop    edi
        pop    ecx
        pop    ebx
loc_1_5764:
        call   sub_1_57B5
        mov    eax, 0x1d
        sbb    edx, edx
        and    eax, edx
        mov    dword ptr [esp + 4], eax
        or     eax, eax
        jne    loc_1_578D
        mov    eax, dword ptr [ebp]
        pushfd 
        add    dword ptr [0xb764], eax       ; -> loc_1_B764
        popfd  
        pushfd 
        inc    dword ptr [0xb76c]            ; -> loc_1_B76C
        popfd  
loc_1_578D:
        call   sub_1_9EB2
        cmp    dword ptr [esp], 0
        je     loc_1_57A0
        mov    eax, dword ptr [ebp + 0x1c]
        call   sub_1_05FA
loc_1_57A0:
        mov    eax, dword ptr [esp + 4]
loc_1_57A4:
        lea    esp, [esp + 8]
        ret    
loc_1_57A9:
        mov    ax, 0x1b
        jmp    loc_1_57A4
loc_1_57AF:
        mov    byte ptr [ebp + 0x34], 0x1f
        jmp    loc_1_57A4
sub_1_57B5:
        test   byte ptr [ebp + 0x3a], 2
        jne    loc_1_5833
        push   ecx
        push   ebx
        sub    ebx, dword ptr [edi + 0xc]
        mov    al, byte ptr [edi + 0x4e]
        not    al
        and    bl, al
        add    ebx, dword ptr [edi + 0xc]
        call   sub_1_0050
        mov    eax, ebx
        pop    ebx
        jae    loc_1_5801
        sub    eax, ebx
        add    al, byte ptr [edi + 0x4d]
        movzx  eax, al
loc_1_57DC:
        cmp    eax, ecx
        jbe    loc_1_57EE
        test   byte ptr [ebp + 0x37], 1
        jne    loc_1_57EC
        cmp    dword ptr [ebp + 0x48], 0
        je     loc_1_5801
loc_1_57EC:
        mov    eax, ecx
loc_1_57EE:
        add    ebx, eax
        sub    ecx, eax
        je     loc_1_5801
        call   sub_1_0050
        jae    loc_1_5801
        movzx  eax, byte ptr [edi + 0x4d]
        jmp    loc_1_57DC
loc_1_5801:
        pop    esi
        sub    esi, ecx
        je     loc_1_5833
        push   esi
        push   ebx
        push   ecx
        sub    ebx, esi
        mov    ecx, esi
        mov    esi, dword ptr [ebp + 0x14]
        call   sub_1_1574
        pop    ecx
        pop    ebx
        pop    esi
        jb     loc_1_58B4
        mov    edi, dword ptr [ebp + 0x1c]
        xchg   ecx, eax
        mov    cl, byte ptr [edi + 0x4c]
        shl    esi, cl
        add    dword ptr [ebp + 0x14], esi
        xchg   ecx, eax
        or     ecx, ecx
        je     loc_1_58B4
loc_1_5833:
        push   ebx
        push   ecx
        call   sub_1_02F8
        mov    edx, ebx
        pop    ecx
        pop    ebx
        jb     loc_1_58B4
        mov    ax, word ptr [edi + 0x4d]
        sub    ebx, dword ptr [edi + 0xc]
        and    ah, bl
        add    ebx, dword ptr [edi + 0xc]
        sub    al, ah
        movzx  eax, al
        cmp    eax, ecx
        jbe    loc_1_5857
        mov    eax, ecx
loc_1_5857:
        add    ebx, eax
        sub    ecx, eax
        VxDCall  Dev_048B.Service_0007h          ; cd2007008b04
        push   esi
        push   eax
        push   edx
        push   ecx
        mov    cl, byte ptr [edi + 0x4c]
        shl    eax, cl
        mov    ecx, eax
        mov    edi, edx
        mov    esi, dword ptr [ebp + 0x14]
        call   sub_1_823A
        mov    dword ptr [ebp + 0x14], esi
        pop    eax
        pop    edx
        pop    ecx
        pop    esi
        mov    edi, dword ptr [ebp + 0x1c]
        jb     loc_1_58A4
        push   eax
        push   ebx
        sub    ebx, ecx
        sub    ebx, dword ptr [esi + 8]
        mov    al, 1
        shl    al, cl
        dec    al
        xchg   ecx, ebx
        shl    al, cl
        xchg   ecx, ebx
        not    al
        and    byte ptr [esi + 0x24], al
        and    byte ptr [esi + 0x25], al
        call   sub_1_5C8F
        pop    ebx
        pop    ecx
loc_1_58A4:
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
        jb     loc_1_58B4
        or     ecx, ecx
        jne    sub_1_57B5
loc_1_58B4:
        ret    
        db  08Dh, 064h, 024h, 008h, 0C3h
sub_1_58BA:
        mov    edi, dword ptr [edx + 0x60]
        or     edi, edi
        jne    loc_1_58ED
        mov    al, byte ptr [edx + 0xac]
        mov    edi, dword ptr [0xb754]       ; -> loc_1_B754
        mov    ecx, edi
loc_1_58CF:
        cmp    byte ptr [edi], al
        je     loc_1_58EA
        mov    edi, dword ptr [edi + 0x19]
        cmp    di, -1
        stc    
        je     loc_1_58ED
        movzx  ecx, di
        shr    edi, 0x10
        shl    edi, 4
        add    edi, ecx
        jmp    loc_1_58CF
loc_1_58EA:
        mov    dword ptr [edx + 0x60], edi
loc_1_58ED:
        ret    
sub_1_58EE:
        mov    edi, edx
        mov    edx, dword ptr [edi + 0x60]
        mov    al, byte ptr [edi + 0xac]
        mov    byte ptr [esi], al
        mov    byte ptr [esi + 1], 0xff
        mov    byte ptr [esi + 0x18], 0
        or     edx, edx
        je     loc_1_5915
        mov    al, byte ptr [edx + 1]
        mov    byte ptr [esi + 1], al
        mov    al, byte ptr [edx + 0x18]
        and    al, 0x80
        mov    byte ptr [esi + 0x18], al
loc_1_5915:
        movzx  eax, word ptr [edi + 0x20]
        mov    word ptr [esi + 2], ax
        mov    al, byte ptr [edi + 0x24]
        mov    byte ptr [esi + 4], al
        mov    al, byte ptr [edi + 0x25]
        mov    byte ptr [esi + 5], al
        movzx  eax, word ptr [edi + 0x26]
        mov    word ptr [esi + 6], ax
        mov    al, byte ptr [edi + 0x46]
        mov    byte ptr [esi + 8], al
        movzx  eax, word ptr [edi + 0x44]
        mov    word ptr [esi + 9], ax
        mov    al, byte ptr [edi + 0xba]
        mov    byte ptr [esi + 0x17], al
        mov    eax, dword ptr [edi + 0x28]
        mov    word ptr [esi + 0xb], ax
        mov    dword ptr [esi + 0x29], eax
        mov    eax, dword ptr [edi + 0x30]
        mov    word ptr [esi + 0xf], ax
        mov    dword ptr [esi + 0x31], eax
        mov    eax, dword ptr [edi + 0x34]
        mov    word ptr [esi + 0x11], ax
        mov    eax, dword ptr [edi + 0x38]
        mov    word ptr [esi + 0x1d], ax
        mov    dword ptr [esi + 0x39], eax
        push   esi
        call   sub_1_5BEF
        pop    esi
        jb     loc_1_59F1
        mov    dword ptr [esi + 0x1f], edx
        inc    ebx
        mov    word ptr [esi + 0xd], bx
        mov    dword ptr [esi + 0x2d], ebx
        xor    eax, eax
        mov    word ptr [esi + 0x23], ax
        mov    word ptr [esi + 0x25], ax
        mov    word ptr [esi + 0x27], ax
        mov    dword ptr [esi + 0x35], eax
        test   byte ptr [edi + 0x52], 3
        jne    loc_1_59EA
        mov    word ptr [esi + 0xb], ax
        mov    word ptr [esi + 0xd], ax
        mov    word ptr [esi + 0xf], ax
        mov    word ptr [esi + 0x11], ax
        mov    word ptr [esi + 0x1d], ax
        mov    word ptr [esi + 0x1f], ax
        movzx  edx, byte ptr [edi + 0x48]
        mov    word ptr [esi + 0x23], dx
        test   byte ptr [edi + 0x52], 0x10
        je     loc_1_59C4
        or     word ptr [esi + 0x23], 0x100
loc_1_59C4:
        test   byte ptr [edi + 0x52], 4
        je     loc_1_59D0
        or     word ptr [esi + 0x23], 0x80
loc_1_59D0:
        mov    edx, dword ptr [edi + 0xa0]
        mov    word ptr [esi + 0x25], dx
        mov    edx, dword ptr [edi + 0xa4]
        mov    word ptr [esi + 0x27], dx
        mov    edx, dword ptr [edi + 0x3c]
        mov    dword ptr [esi + 0x35], edx
loc_1_59EA:
        dec    eax
        mov    dword ptr [esi + 0x13], eax
        mov    dword ptr [esi + 0x19], eax
loc_1_59F1:
        ret    
loc_1_59F2:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 0x14]
        cld    
        sub    eax, eax
        mov    byte ptr [ebp + 0x37], al
        mov    word ptr [ebp + 0x38], ax
        mov    byte ptr [ebp + 0x34], al
        mov    byte ptr [ebp + 0x35], al
        call   sub_1_A318
        mov    edx, dword ptr [ebp + 0x1c]
        test   byte ptr [ebp + 0x18], 1
        jne    loc_1_5AA1
        test   byte ptr [edx + 0x52], 3
        je     loc_1_5A98
        call   sub_1_58BA
        jb     loc_1_5A98
        mov    ebx, edi
        movzx  eax, word ptr [edx + 0x20]
        mov    word ptr [edi + 2], ax
        mov    al, byte ptr [edx + 0x24]
        mov    byte ptr [edi + 4], al
        mov    al, byte ptr [edx + 0x25]
        mov    byte ptr [edi + 5], al
        movzx  eax, word ptr [edx + 0x26]
        mov    word ptr [edi + 6], ax
        mov    al, byte ptr [edx + 0x46]
        mov    byte ptr [edi + 8], al
        movzx  eax, word ptr [edx + 0x44]
        mov    word ptr [edi + 9], ax
        mov    eax, dword ptr [edx + 0x28]
        mov    word ptr [edi + 0xb], ax
        mov    eax, dword ptr [edx + 0x2c]
        mov    word ptr [edi + 0xd], ax
        mov    eax, dword ptr [edx + 0x30]
        mov    word ptr [edi + 0xf], ax
        mov    eax, dword ptr [edx + 0x34]
        mov    word ptr [edi + 0x11], ax
        mov    al, byte ptr [edx + 0xba]
        mov    byte ptr [edi + 0x17], al
        mov    edi, edx
        mov    eax, ebx
        call   sub_1_5BEF
        jb     loc_1_5A98
        mov    word ptr [eax + 0x1f], dx
        shl    eax, 0xc
        shr    ax, 0xc
        mov    dword ptr [ebp + 0x10], eax
        jmp    loc_1_4574
loc_1_5A98:
        mov    ax, 0xffff
        jmp    loc_1_4576
loc_1_5AA1:
        call   sub_1_58BA
        mov    eax, dword ptr [ebp]
        cmp    eax, 0x3f
        jb     loc_1_5ACB
        mov    esi, dword ptr [ebp + 0x14]
        mov    word ptr [esi], 0x3d
        add    esi, 2
        call   sub_1_58EE
        jae    loc_1_4574
        mov    al, 5
        jmp    loc_1_4576
loc_1_5ACB:
        mov    al, 0x18
        jmp    loc_1_4576
loc_1_5AD2:
        pop    ds
loc_1_5AD3:
        or     al, 1
        sbb    ecx, dword ptr [eax]
        add    al, 0x1e
        or     edx, dword ptr [eax]
        push   ss
        or     al, 1
        adc    al, 1
        add    dl, byte ptr [0xfa30202]
        add    dl, byte ptr [ebx]
        add    byte ptr [ebx], al
sub_1_5AEA:
        mov    edi, 0x5ad2                   ; -> loc_1_5AD2
        mov    ecx, 8
loc_1_5AF4:
        scasb  al, byte ptr es:[edi]
        je     loc_1_5B01
        add    edi, 2
        loop   loc_1_5AF4
        mov    edi, 0x5ad3                   ; -> loc_1_5AD3
loc_1_5B01:
        mov    ax, word ptr [edi]
        ret    
sub_1_5B05:
        push   ebx
        push   ecx
        push   edx
        push   esi
        mov    ecx, dword ptr [edi + 0x14]
        movzx  ebx, word ptr [ecx + 0xa]
        mov    esi, dword ptr [ecx + 0xc]
        mov    ah, 2
        call   sub_1_A4E1
        mov    esi, ebx
        mov    dword ptr [esi + 0x44], ecx
        mov    al, byte ptr [edi + 0xac]
        mov    byte ptr [esi + 0x4d], al
        mov    dword ptr [esi + 8], 0x500
        mov    word ptr [esi + 4], 0x29
        mov    word ptr [esi + 0x2e], 0xdb02
        movzx  eax, byte ptr [edi + 0x24]
        inc    eax
        mov    dword ptr [esi + 0x34], eax
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [esi + 0x38], eax
        VxDCall  BLOCKDEV.BlockDev_Send_Command  ; cd2004001000
        cmp    word ptr [esi + 6], 0x10
        cmc    
        pushfd 
        sbb    ecx, ecx
        mov    eax, dword ptr [esi + 0x3c]
        or     eax, ecx
        push   eax
        call   sub_1_A4AE
        pop    eax
        mov    edx, dword ptr [edi + 0x6c]
        movzx  ecx, byte ptr [edi + 0x4d]
        bsf    ecx, ecx
        add    cl, byte ptr [edi + 0x4c]
        shl    edx, cl
        mov    cl, byte ptr [edi + 0x47]
        shr    edx, cl
        sub    eax, edx
        cmc    
        sbb    edx, edx
        and    eax, edx
        popfd  
        pop    esi
        pop    edx
        pop    ecx
        pop    ebx
        ret    
sub_1_5B85:
        mov    eax, 1
        mov    edi, dword ptr [ebp + 0x1c]
sub_1_5B8D:
        test   word ptr [edi + 0x52], 0x10
        je     loc_1_5BCD
        push   ebx
        push   edx
        push   esi
        push   eax
        mov    ecx, 0xdb03
        mov    ebx, edi
        call   sub_1_449E
        cmp    eax, 0x10
        sbb    eax, eax
        and    eax, edx
        pop    ecx
        sub    eax, dword ptr [edi + 0x6c]
        jb     loc_1_5BBB
        sub    eax, dword ptr [edi + 0x68]
        jb     loc_1_5BBB
        sub    eax, ecx
        jae    loc_1_5BCA
loc_1_5BBB:
        push   edi
        mov    eax, edi
        call   sub_1_05FA
        pop    edi
        call   sub_1_5CB0
        stc    
loc_1_5BCA:
        pop    esi
        pop    edx
        pop    ebx
loc_1_5BCD:
        ret    
sub_1_5BCE:
        or     eax, 0xffffffff
        jmp    loc_1_5BD5
sub_1_5BD3:
        xor    eax, eax
loc_1_5BD5:
        mov    esi, dword ptr [edi + 0x9c]
        or     esi, esi
        je     loc_1_5BEE
        mov    ecx, dword ptr [edi + 0x2c]
        shr    ecx, 0xa
        inc    ecx
        push   edi
        cld    
        mov    edi, esi
        rep stosw word ptr es:[edi], ax
        pop    edi
loc_1_5BEE:
        ret    
sub_1_5BEF:
        push   eax
loc_1_5BF0:
        mov    ebx, dword ptr [edi + 0x2c]
        dec    ebx
        mov    edx, dword ptr [edi + 0x40]
        cmp    edx, -1
        je     loc_1_5C04
        cmp    ebx, edx
        jae    loc_1_5C3E
        or     dword ptr [edi + 0x40], 0xffffffff
loc_1_5C04:
        cmp    dword ptr [edi + 0x9c], 0
        jne    loc_1_5C40
        mov    esi, dword ptr [edi + 0xa8]
        xor    edx, edx
        mov    ecx, ebx
        jecxz  loc_1_5C39
        lea    ebx, [edx + 1]
loc_1_5C1C:
        inc    ebx
        push   ebx
        push   ecx
        push   edx
        push   esi
        call   sub_1_1F2C
        pop    esi
        pop    edx
        pop    ecx
        pop    ebx
        jb     loc_1_5C3E
        jne    loc_1_5C2F
        inc    edx
loc_1_5C2F:
        loop   loc_1_5C1C
        cmp    dword ptr [edi + 0xa8], esi
        jne    loc_1_5BF0
loc_1_5C39:
        mov    dword ptr [edi + 0x40], edx
        dec    ebx
        clc    
loc_1_5C3E:
        pop    eax
        ret    
loc_1_5C40:
        xor    ebx, ebx
loc_1_5C42:
        call   sub_1_7788
        jb     loc_1_5C4F
        add    ebx, 0x400
loc_1_5C4F:
        cmp    dword ptr [edi + 0x2c], ebx
        jae    loc_1_5C42
        mov    esi, dword ptr [edi + 0x9c]
        xor    edx, edx
loc_1_5C5C:
        movsx  eax, word ptr [esi]
        add    esi, 2
        cmp    eax, -2
        je     loc_1_5C73
        cmp    eax, 0x401
        cmc    
        jb     loc_1_5C3E
        add    edx, eax
        jmp    loc_1_5C5C
loc_1_5C73:
        mov    ebx, dword ptr [edi + 0x2c]
        jmp    loc_1_5C39
        db  033h, 0D2h, 066h, 0F7h, 045h, 038h, 004h, 000h, 074h, 008h, 042h, 0F6h, 045h, 03Ah, 008h, 074h
        db  001h, 042h, 0E9h, 064h, 0A7h, 0FFh, 0FFh
sub_1_5C8F:
        push   edx
        xor    edx, edx
        test   word ptr [ebp + 0x38], 4
        je     loc_1_5CA9
        mov    dl, 2
        test   byte ptr [ebp + 0x3a], 8
        jne    loc_1_5CA9
        dec    edx
        cmp    byte ptr [edi + 0x4d], cl
        ja     loc_1_5CA9
        dec    edx
loc_1_5CA9:
        call   sub_1_043A
        pop    edx
        ret    
sub_1_5CB0:
        cmp    byte ptr [ebp + 0x35], 0
        jne    loc_1_5CDF
        or     word ptr [ebp + 0x38], 4
loc_1_5CBB:
        mov    eax, dword ptr [ebp + 0x1c]
        movzx  edx, byte ptr [eax + 0x1e]
loc_1_5CC2:
        bts    dword ptr [eax + 0x1c], edx
        jb     loc_1_5CE0
        mov    byte ptr [edx + eax + 0xbc], dh
        mov    byte ptr [ebp + 0x35], dl
        inc    edx
        and    dl, 0xf
        cmp    dl, 1
        adc    dl, 0
        mov    byte ptr [eax + 0x1e], dl
loc_1_5CDF:
        ret    
loc_1_5CE0:
        cmp    word ptr [eax + 0x1c], -2
        jae    loc_1_5CF3
        inc    edx
        and    dl, 0xf
        cmp    dl, 1
        adc    dl, 0
        jmp    loc_1_5CC2
loc_1_5CF3:
        mov    eax, 0xb6f4                   ; -> obj1:0xB6F4
        call   sub_1_A220
        jmp    loc_1_5CBB
sub_1_5CFF:
        mov    eax, dword ptr [ebp + 0x1c]
        test   word ptr [eax + 0x52], 0xa400
        jne    loc_1_5D16
        mov    eax, dword ptr [eax + 0x14]
        test   dword ptr [eax + 4], 0x8000
        je     loc_1_5D1B
loc_1_5D16:
        call   sub_1_5CB0
loc_1_5D1B:
        ret    
loc_1_5D1C:
        pushal 
        mov    ebx, dword ptr [esp + 0x24]
        mov    ecx, dword ptr [esp + 0x28]
        push   ebp
        sub    esp, 0x74
        mov    ebp, esp
        cld    
        sub    eax, eax
        mov    byte ptr [ebp + 0x37], al
        mov    word ptr [ebp + 0x38], ax
        mov    byte ptr [ebp + 0x34], al
        mov    byte ptr [ebp + 0x35], al
        mov    eax, ebx
        mov    edx, ecx
        cmp    dword ptr [0xb6b8], 0         ; -> obj1:0xB6B8
        jne    loc_1_5E67
        mov    dword ptr [0xb6b8], 1         ; -> obj1:0xB6B8
        mov    esi, dword ptr [0xb758]       ; -> loc_1_B758
        cmp    esi, 0
        je     loc_1_5E51
        mov    dword ptr [ebp + 0x20], esi
        mov    dword ptr [ebp + 0x14], eax
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [ebp + 0x24], edx
        mov    edi, dword ptr [eax + 0x28]
        cmp    dword ptr [edi + 0x68], 8
        jae    loc_1_5E51
        mov    dword ptr [ebp], 0x1000
        mov    dword ptr [ebp + 0x1c], edi
        mov    eax, dword ptr [edi + 0x14]
        test   dword ptr [eax + 4], 0x40
        jne    loc_1_5E51
        mov    dword ptr [ebp + 0x3c], edx
        mov    ebx, dword ptr [esi + 8]
        add    edx, 0x1000
        cmp    edx, dword ptr [ebx + 0x30]
        ja     loc_1_5E51
        push   esi
        call   sub_1_774E
        pop    esi
        jb     loc_1_5E51
        push   esi
        call   sub_1_7704
        pop    esi
        mov    eax, dword ptr [0xb6f0]       ; -> loc_1_B6F0
        lea    edx, [eax + 0x10]
        mov    dword ptr [eax + 0xc], edx
loc_1_5DCC:
        push   edx
        mov    ecx, dword ptr [ebp + 0x48]
        call   sub_1_76B7
        pop    edx
        jb     loc_1_5E51
        mov    dword ptr [edx], ebx
        mov    dword ptr [edx + 4], ecx
        add    edx, 8
        sub    dword ptr [ebp + 0x48], ecx
        jne    loc_1_5DCC
        mov    eax, dword ptr [0xb6f0]       ; -> loc_1_B6F0
        mov    dword ptr [edx], 0
        mov    ebx, dword ptr [eax + 0x10]
        mov    edx, eax
        mov    eax, dword ptr [ebp + 0x3c]
        mov    dword ptr [edx], eax
        push   edi
        push   esi
        push   edx
        mov    ah, byte ptr [0xb6f8]         ; -> obj1:0xB6F8
        mov    al, 0x13
        VxDCall  Dev_048B.Service_0004h          ; cd2004008b04
        pop    edx
        cmp    esi, 0
        je     loc_1_5E4F
        mov    ebx, esi
        or     byte ptr [esi + 0x2b], 2
        mov    dword ptr [edx + 4], esi
        mov    edi, dword ptr [esi + 0x10]
        mov    dword ptr [edx + 8], edi
        mov    esi, dword ptr [ebp + 0x14]
        mov    ecx, 0x1000
        call   sub_1_823A
        pop    esi
        pop    edi
        pushal 
        mov    esi, dword ptr [0xb6f0]       ; -> loc_1_B6F0
        call   sub_1_8C70
        popal  
        jb     loc_1_5E5B
        add    dword ptr [ebp + 0x3c], 0x1000
        clc    
        add    esp, 0x74
        pop    ebp
        popal  
        xor    eax, eax
        clc    
        ret    
loc_1_5E4F:
        pop    esi
        pop    edi
loc_1_5E51:
        mov    dword ptr [0xb6b8], 0         ; -> obj1:0xB6B8
loc_1_5E5B:
        add    esp, 0x74
        pop    ebp
        popal  
        mov    eax, 1
        stc    
        ret    
loc_1_5E67:
        cmp    dword ptr [0xb6b8], 2         ; -> obj1:0xB6B8
        jb     loc_1_5E5B
        add    esp, 0x74
        pop    ebp
        popal  
        mov    eax, 2
        stc    
        ret    
sub_1_5E7C:
        cmp    byte ptr [edi + 0x46], 1
        jbe    loc_1_5EF2
        mov    eax, dword ptr [ebp + 0x24]
        movzx  ecx, byte ptr [edi + 0x4c]
        mov    edx, dword ptr [ebp]
        shr    edx, cl
        lea    ecx, [eax + edx]
        mov    ebx, dword ptr [edi + 0x18]
        mov    edx, dword ptr [edi + 0x30]
        add    edx, ebx
        cmp    eax, edx
        jae    loc_1_5EF2
        cmp    ecx, ebx
        jbe    loc_1_5EF2
        cmp    ebx, eax
        jae    loc_1_5EA7
        mov    ebx, eax
loc_1_5EA7:
        cmp    edx, ecx
        jbe    loc_1_5EAD
        mov    edx, ecx
loc_1_5EAD:
        push   ebx
        movzx  eax, byte ptr [edi + 0x4e]
        not    eax
        sub    ebx, dword ptr [edi + 0xc]
        and    ebx, eax
        add    ebx, dword ptr [edi + 0xc]
        call   sub_1_0050
        jb     loc_1_5EF3
        pop    ebx
loc_1_5EC4:
        mov    eax, ebx
        sub    eax, dword ptr [esi + 8]
        sub    ecx, ecx
        bts    ecx, eax
        or     byte ptr [esi + 0x24], cl
        inc    ebx
        cmp    ebx, edx
        jae    loc_1_5EF2
        movzx  eax, byte ptr [edi + 0x4d]
        add    eax, dword ptr [esi + 8]
        cmp    ebx, eax
        jb     loc_1_5EC4
loc_1_5EE1:
        call   sub_1_0050
        jae    loc_1_5EC4
loc_1_5EE8:
        movzx  eax, byte ptr [edi + 0x4d]
        add    ebx, eax
        cmp    ebx, edx
        jb     loc_1_5EE1
loc_1_5EF2:
        ret    
loc_1_5EF3:
        pop    eax
        jmp    loc_1_5EE8
        db  000h, 000h, 040h, 000h
        dd  loc_1_5F8A               ; fixup
        db  041h, 000h
        dd  loc_1_617B               ; fixup
        db  042h, 000h
        dd  loc_1_5F8A               ; fixup
        db  046h, 000h
        dd  loc_1_6059               ; fixup
        db  047h, 000h
        dd  loc_1_5F8A               ; fixup
        db  048h, 001h
        dd  loc_1_5F8A               ; fixup
        db  049h, 001h
        dd  loc_1_5F8A               ; fixup
        db  060h, 001h
        dd  loc_1_5F8A               ; fixup
        db  061h, 000h
        dd  loc_1_617B               ; fixup
        db  062h, 000h
        dd  loc_1_5F8A               ; fixup
        db  066h, 001h
        dd  loc_1_6109               ; fixup
        db  067h, 001h
        dd  loc_1_5F8A               ; fixup
        db  068h, 001h
        dd  loc_1_5F8A               ; fixup
        db  0FFh, 001h
        dd  loc_1_5F8A               ; fixup
loc_1_5F4C:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 0x14]
        cld    
        sub    eax, eax
        mov    byte ptr [ebp + 0x37], al
        mov    word ptr [ebp + 0x38], ax
        mov    byte ptr [ebp + 0x34], al
        mov    byte ptr [ebp + 0x35], al
        mov    edi, dword ptr [ebp + 0x1c]
        mov    ecx, dword ptr [ebp + 0x28]
        movzx  eax, byte ptr [ecx + 0x1c]
        movzx  edx, word ptr [ecx + 0x18]
        cmp    al, 0x12
        je     loc_1_619C
        cmp    al, 0xd
        je     loc_1_6016
        cmp    al, 0x11
        je     loc_1_6043
loc_1_5F8A:
        call   sub_1_A3B9
        mov    word ptr [ebx + 4], 0xd
        mov    dword ptr [ebx + 8], 0x510
        mov    esi, ebx
        mov    dword ptr [esi + 0x38], ecx
        mov    ax, word ptr [ecx + 0x10]
        mov    word ptr [esi + 0x2c], ax
        dec    al
        mov    byte ptr [esi + 0x4d], al
        movzx  eax, word ptr [ecx + 0x18]
        mov    dword ptr [esi + 0x30], eax
        test   byte ptr [ebp + 0x18], 1
        je     loc_1_5FC7
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [esi + 0x34], eax
        and    byte ptr [esi + 8], 0xef
        jmp    loc_1_5FD4
loc_1_5FC7:
        mov    ax, 0x3c14
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        mov    dword ptr [esi + 0x34], eax
loc_1_5FD4:
        mov    ax, word ptr [ecx + 0x1c]
        mov    word ptr [esi + 0x2e], ax
        xor    eax, eax
        mov    dword ptr [esi + 0x44], eax
        VMMCall  VMM.Get_System_Time             ; cd203f000100
        cld    
        mov    dword ptr [edi + 0x78], eax
        mov    eax, esi
        push   ecx
        push   eax
        call   sub_1_199E
        pop    edi
        pop    ebx
        mov    eax, dword ptr [edi + 0x3c]
        mov    word ptr [ebx + 0x1c], ax
        cmp    word ptr [edi + 6], 0x10
        jae    loc_1_6006
        xor    eax, eax
loc_1_6006:
        mov    word ptr [ebp + 0x1a], ax
        mov    ebx, edi
        call   sub_1_A4AE
loc_1_6011:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_6016:
        mov    ebx, 0x5ef2                   ; -> loc_1_5EF2
loc_1_601B:
        add    ebx, 6
        cmp    byte ptr [ebx], dl
        jb     loc_1_601B
        ja     loc_1_5F8A
        cmp    byte ptr [ebx + 1], 0
        jne    loc_1_6040
        mov    eax, dword ptr [ebp + 0x1c]
        test   byte ptr [eax + 0x52], 3
        jne    loc_1_6040
        mov    ax, 5
        cmp    dh, 0x48
        jne    loc_1_6053
loc_1_6040:
        jmp    dword ptr [ebx + 2]
loc_1_6043:
        cmp    dl, 0x66
        je     loc_1_6051
        cmp    dl, 0x46
        jne    loc_1_5F8A
loc_1_6051:
        xor    eax, eax
loc_1_6053:
        mov    word ptr [ebp + 0x1a], ax
        jmp    loc_1_6011
loc_1_6059:
        call   sub_1_A318
        xor    ebx, ebx
        call   sub_1_00D0
        mov    al, 0x1e
        mov    byte ptr [ebp + 0x34], 0
        jb     loc_1_4576
        mov    al, 0x1a
        cmp    byte ptr [ebx + 0x15], 0xf0
        jb     loc_1_4576
        xor    edx, edx
        cmp    word ptr [ebx + 0x16], dx
        jne    loc_1_6096
        cmp    byte ptr [ebx + 0x42], 0x29
        jne    loc_1_4576
        mov    edx, 0x1c
        jmp    loc_1_60A0
loc_1_6096:
        cmp    byte ptr [ebx + 0x26], 0x29
        jne    loc_1_4576
loc_1_60A0:
        mov    ax, 0x3c14
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        VxDCall  Dev_048B.Service_0007h          ; cd2007008b04
        push   esi
        lea    edi, [edx + ebx + 0x27]
        lea    esi, [eax + 2]
        mov    ecx, 0x17
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    esi
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
        push   eax
        call   sub_1_55D2
        pop    ebx
        mov    al, 0x1d
        mov    byte ptr [ebp + 0x34], 0
        jb     loc_1_4576
        lea    esi, [ebx + 2]
        mov    eax, dword ptr [ebp + 0x1c]
        lea    edi, [eax + 0x54]
        movsd  dword ptr es:[edi], dword ptr [esi]
        lea    edi, [eax + 0xae]
        mov    ecx, 0xb
        rep movsb byte ptr es:[edi], byte ptr [esi]
        lea    edi, [eax + 0xae]
        call   sub_1_6EC3
        mov    edi, dword ptr [ebp + 0x1c]
        mov    byte ptr [edi + 0xb9], al
        jmp    loc_1_4574
loc_1_6109:
        call   sub_1_A318
        xor    ebx, ebx
        call   sub_1_00D0
        mov    al, 0x1e
        mov    byte ptr [ebp + 0x34], 0
        jb     loc_1_4576
        mov    al, 0x1a
        cmp    byte ptr [ebx + 0x15], 0xf0
        jb     loc_1_4576
        xor    edx, edx
        cmp    word ptr [ebx + 0x16], dx
        jne    loc_1_6146
        cmp    byte ptr [ebx + 0x42], 0x29
        jne    loc_1_4576
        mov    edx, 0x1c
        jmp    loc_1_6150
loc_1_6146:
        cmp    byte ptr [ebx + 0x26], 0x29
        jne    loc_1_4576
loc_1_6150:
        mov    ax, 0x3c14
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        VxDCall  Dev_048B.Service_0007h          ; cd2007008b04
        push   esi
        lea    edi, [eax + 2]
        lea    esi, [edx + ebx + 0x27]
        mov    ecx, 0x17
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    esi
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
        jmp    loc_1_4574
loc_1_617B:
        push   ecx
        push   edi
        push   eax
        mov    eax, edi
        call   sub_1_05FA
        pop    eax
        pop    edi
        pop    ecx
        or     eax, eax
        je     loc_1_5F8A
        mov    ebx, edi
        call   sub_1_A72F
        jmp    loc_1_5F8A
loc_1_619C:
        mov    esi, dword ptr [ebp + 0x14]
        cmp    dword ptr [esi], 1
        ja     loc_1_622F
        call   sub_1_A318
        cmp    dword ptr [esi], 1
        jne    loc_1_61B7
        mov    esi, dword ptr [esi + 8]
        jmp    loc_1_61B9
loc_1_61B7:
        xor    esi, esi
loc_1_61B9:
        mov    ebx, dword ptr [ebp + 0x20]
        mov    ebx, dword ptr [ebx + 8]
        mov    ebx, dword ptr [ebx + 0x10]
        mov    edi, dword ptr [ebp + 0x1c]
        xor    ecx, ecx
        or     ebx, ebx
        je     loc_1_6224
        cmp    dword ptr [edi + 0x2c], ebx
        jb     loc_1_622F
        or     esi, esi
        je     loc_1_61FB
loc_1_61D4:
        mov    eax, ecx
        mov    edx, ebx
        mov    cl, byte ptr [edi + 0x25]
        sub    edx, 2
        jb     loc_1_622F
        shl    edx, cl
        add    edx, dword ptr [edi + 0x28]
        mov    ecx, eax
        mov    dword ptr [esi + 4], edx
        mov    dword ptr [esi], 0
loc_1_61F0:
        or     esi, esi
        je     loc_1_61FB
        movzx  eax, byte ptr [edi + 0x24]
        inc    eax
        add    dword ptr [esi], eax
loc_1_61FB:
        push   ebx
        push   ecx
        push   esi
        call   sub_1_1F2C
        pop    esi
        pop    ecx
        pop    eax
        jb     loc_1_622F
        call   sub_1_2102
        jae    loc_1_6223
        cmp    dword ptr [edi + 0x2c], ebx
        jb     loc_1_622F
        inc    eax
        cmp    eax, ebx
        je     loc_1_61F0
        inc    ecx
        or     esi, esi
        je     loc_1_61FB
        add    esi, 8
        jmp    loc_1_61D4
loc_1_6223:
        inc    ecx
loc_1_6224:
        mov    esi, dword ptr [ebp + 0x14]
        mov    dword ptr [esi + 4], ecx
        jmp    loc_1_4574
loc_1_622F:
        mov    eax, 5
        jmp    loc_1_4576
        db  000h, 000h, 000h
        ; ---- dispatch table (7 entries) ----
        dd  loc_1_628A
        dd  loc_1_6283
        dd  loc_1_62D0
        dd  loc_1_62D7
        dd  loc_1_62EF
        dd  loc_1_630B
        dd  loc_1_6325
loc_1_6258:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 0x14]
        cld    
        sub    eax, eax
        mov    byte ptr [ebp + 0x37], al
        mov    word ptr [ebp + 0x38], ax
        mov    byte ptr [ebp + 0x34], al
        mov    byte ptr [ebp + 0x35], al
        movzx  esi, byte ptr [ebp + 4]
        cmp    esi, 7
        jae    loc_1_6283
        jmp    dword ptr [esi*4 + 0x623c]    ; -> obj1:0x623C
loc_1_6283:
        mov    eax, 1
        jmp    loc_1_62C7
loc_1_628A:
        call   sub_1_A26B
        bts    dword ptr [0xb786], 0         ; -> obj1:0xB786
        call   sub_1_6338
        mov    dword ptr [ebp + 0x10], 0x228c ; -> loc_1_228C
        push   eax
        call   sub_1_A2CE
        btr    dword ptr [0xb786], 0         ; -> obj1:0xB786
        pop    eax
        or     eax, eax
        jne    loc_1_62C7
        mov    eax, 1
        and    eax, dword ptr [0xb870]       ; -> obj1:0xB870
        mov    word ptr [ebp + 6], ax
        sub    eax, eax
loc_1_62C7:
        mov    word ptr [ebp + 0x1a], ax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_62D0:
        call   sub_1_6593
        jmp    loc_1_62C7
loc_1_62D7:
        mov    edi, dword ptr [ebp + 0x28]
        mov    eax, edi
        call   sub_1_05FA
        mov    ebx, dword ptr [ebp + 0x28]
        call   sub_1_A72F
        mov    edi, ebx
        sub    eax, eax
        jmp    loc_1_62C7
loc_1_62EF:
        call   sub_1_66AD
        or     eax, eax
        jne    loc_1_62C7
        mov    eax, 1
        and    eax, dword ptr [0xb870]       ; -> obj1:0xB870
        mov    word ptr [ebp + 6], ax
        sub    eax, eax
        jmp    loc_1_62C7
loc_1_630B:
        mov    edx, dword ptr [ebp + 0x1c]
        movzx  eax, word ptr [ebp + 0x18]
        mov    byte ptr [edx + 0xac], al
        mov    eax, dword ptr [ebp + 0x24]
        mov    byte ptr [edx + 0xad], al
        xor    eax, eax
        jmp    loc_1_62C7
loc_1_6325:
        mov    edx, dword ptr [ebp + 0x1c]
        mov    al, byte ptr [edx + 0xad]
        mov    byte ptr [edx + 0xac], al
        xor    eax, eax
        jmp    loc_1_62C7
sub_1_6338:
        mov    ebx, dword ptr [ebp + 0x10]
        cmp    dword ptr [ebx + 0x10], 0x800
        ja     loc_1_6523
        mov    edx, dword ptr [ebp + 0x28]
        mov    ebx, dword ptr [ebp + 0x24]
        mov    eax, dword ptr [ebp + 0x1c]
        mov    dword ptr [0xb754], eax       ; -> loc_1_B754
        xor    eax, eax
        mov    byte ptr [ebp + 0x34], al
        mov    eax, 0xcc
        call   sub_1_A1CE
        jb     loc_1_6523
        mov    edi, eax
        sub    eax, eax
        mov    ecx, 0x33
        rep stosd dword ptr es:[edi], eax
        lea    eax, [edi - 0xcc]
        mov    dword ptr [ebp + 0x1c], eax
        mov    dword ptr [eax + 0x10], ebx
        mov    byte ptr [eax + 0xac], dl
        mov    byte ptr [eax + 0xad], dl
        mov    ebx, dword ptr [ebp + 0x10]
        mov    dword ptr [ebx + 0x18], 0x6258 ; -> loc_1_6258
        mov    dword ptr [eax + 0x14], ebx
        or     word ptr [ebp + 0x38], 0x400
        mov    ebx, dword ptr [0xb87c]       ; -> loc_1_B87C
        mov    dword ptr [eax + 0x74], ebx
        mov    ebx, dword ptr [0xb874]       ; -> loc_1_B874
        mov    dword ptr [eax + 0x7c], ebx
        mov    ebx, dword ptr [0xb878]       ; -> obj1:0xB878
        mov    dword ptr [eax + 0x70], ebx
        mov    byte ptr [eax + 0x1f], 0x30
        xchg   esi, eax
        push   0
        push   0x800
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        or     eax, eax
        je     loc_1_650F
        xchg   esi, eax
        push   esi
        mov    ebx, dword ptr [ebp + 0x1c]
        mov    ecx, 1
        sub    edi, edi
        push   esi
        call   sub_1_1458
        pop    esi
        jb     loc_1_6502
        push   esi
        call   sub_1_68DF
        pop    esi
        jb     loc_1_6502
        mov    ebx, dword ptr [ebp + 0x1c]
        mov    eax, dword ptr [ebx + 0x14]
        test   byte ptr [eax + 1], 0x40
        je     loc_1_6410
        call   sub_1_A5B7
loc_1_6410:
        pop    esi
        mov    ebx, dword ptr [ebp + 0x1c]
        test   byte ptr [ebx + 0x52], 3
        jne    loc_1_6463
        mov    ecx, 1
        mov    edi, dword ptr [ebx + 0xa0]
        cmp    edi, -1
        je     loc_1_6463
        push   ebx
        push   esi
        call   sub_1_1458
        pop    esi
        pop    ebx
        jb     loc_1_6503
        add    esi, 0x1e4
        cmp    dword ptr [esi], 0x61417272
        jne    loc_1_645D
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ebx + 0x40], eax
        mov    eax, dword ptr [esi + 8]
        cmp    eax, 2
        jb     loc_1_645D
        cmp    dword ptr [ebx + 0x2c], eax
        jb     loc_1_645D
        mov    dword ptr [ebx + 0x38], eax
loc_1_645D:
        sub    esi, 0x1e4
loc_1_6463:
        push   0
        push   esi
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
        mov    eax, 0xb748                   ; -> loc_1_B748
loc_1_6474:
        mov    eax, dword ptr [eax]
        cmp    eax, 0xb748                   ; -> loc_1_B748
        je     loc_1_64CE
        call   sub_1_684B
        jne    loc_1_6474
        mov    edx, dword ptr [eax + 0x14]
        test   byte ptr [edx + 1], 0x40
        jne    loc_1_64BB
loc_1_648D:
        mov    dword ptr [ebp + 0x1c], edx
        test   byte ptr [ebx + 0x52], 0x80
        je     loc_1_649B
        call   sub_1_A72F
loc_1_649B:
        and    word ptr [eax + 0x52], 0xfeff
        mov    dx, word ptr [ebx + 0x52]
        and    dx, 0x100
        or     word ptr [eax + 0x52], dx
        xchg   ebx, eax
        call   sub_1_A1F8
        xchg   ebx, eax
        mov    eax, 0x11c
        ret    
loc_1_64BB:
        or     dword ptr [eax + 0x40], 0xffffffff
        cmp    dword ptr [eax + 0x68], 0
        jne    loc_1_648D
        xchg   ebx, eax
        call   sub_1_A72F
        xchg   ebx, eax
        jmp    loc_1_648D
loc_1_64CE:
        mov    eax, dword ptr [ebx + 0x14]
        mov    dword ptr [eax + 0x14], ebx
        pushfd 
        cli    
        mov    eax, 0xb748                   ; -> loc_1_B748
        mov    ecx, dword ptr [eax]
        mov    dword ptr [ebx], ecx
        mov    dword ptr [ebx + 4], eax
        mov    dword ptr [ecx + 4], ebx
        mov    dword ptr [eax], ebx
        popfd  
        mov    edx, dword ptr [ebp + 0x1c]
        call   sub_1_6DDA
        call   sub_1_6529
        call   sub_1_6556
        call   sub_1_6EDA
        sub    eax, eax
        ret    
loc_1_6502:
        pop    esi
loc_1_6503:
        push   0
        push   esi
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
loc_1_650F:
        mov    eax, dword ptr [ebp + 0x1c]
        call   sub_1_A1F8
        movzx  ecx, byte ptr [ebp + 0x34]
        jecxz  loc_1_6523
        VxDCall  BLOCKDEV.Service_800Fh          ; cd200f801000
loc_1_6523:
        mov    eax, 5
        ret    
sub_1_6529:
        mov    edx, dword ptr [ebp + 0x1c]
        mov    dword ptr [edx + 0x90], 0
        mov    eax, dword ptr [edx + 0x14]
        test   byte ptr [eax + 1], 0x40
        jne    loc_1_6555
        mov    eax, dword ptr [0xb27c]       ; -> loc_1_B27C
        or     eax, eax
        je     loc_1_6555
        call   sub_1_A1E0
        jb     loc_1_6555
        mov    dword ptr [edx + 0x90], eax
loc_1_6555:
        ret    
sub_1_6556:
        mov    edx, dword ptr [ebp + 0x1c]
        mov    eax, dword ptr [edx + 0x2c]
        shr    eax, 0xa
        cmp    eax, 0x1e
        jb     loc_1_6592
        lea    eax, [eax + eax + 4]
        push   eax
        call   sub_1_A1CE
        pop    ecx
        jb     loc_1_6592
        mov    dword ptr [edx + 0x9c], eax
        push   edi
        mov    edi, eax
        or     eax, 0xffffffff
        shrd   edx, ecx, 2
        cld    
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        shld   ecx, edx, 2
        rep stosb byte ptr es:[edi], al
        dec    word ptr [edi - 2]
        pop    edi
loc_1_6592:
        ret    
sub_1_6593:
        mov    ebx, dword ptr [ebp + 0x28]
        mov    eax, 0xcc
        call   sub_1_A516
        jb     loc_1_66A7
        mov    edi, eax
        sub    eax, eax
        mov    ecx, 0x33
        rep stosd dword ptr es:[edi], eax
        lea    eax, [edi - 0xcc]
        mov    dword ptr [eax + 0x10], 0
        mov    dword ptr [ebp + 0x1c], eax
        mov    dl, byte ptr [ebx + 0xac]
        mov    byte ptr [eax + 0xac], dl
        mov    dl, byte ptr [ebx + 0xad]
        mov    byte ptr [eax + 0xad], dl
        or     word ptr [ebp + 0x38], 0x500
        mov    edx, dword ptr [0xb87c]       ; -> loc_1_B87C
        mov    dword ptr [eax + 0x74], edx
        mov    edx, dword ptr [0xb874]       ; -> loc_1_B874
        mov    dword ptr [eax + 0x7c], edx
        mov    edx, dword ptr [0xb878]       ; -> obj1:0xB878
        mov    dword ptr [eax + 0x70], edx
        mov    byte ptr [eax + 0x1f], 0x30
        mov    edx, dword ptr [ebx + 0x14]
        mov    dword ptr [eax + 0x14], edx
        mov    eax, 0x800
        call   sub_1_A516
        or     eax, eax
        je     loc_1_669F
        mov    esi, eax
        mov    ebx, dword ptr [ebp + 0x1c]
        mov    ecx, 1
        sub    edi, edi
        push   esi
        call   sub_1_1458
        pop    esi
        jb     loc_1_668B
        push   esi
        call   sub_1_68DF
        pop    esi
        jb     loc_1_668B
        mov    ebx, dword ptr [ebp + 0x1c]
        mov    eax, dword ptr [ebx + 0x14]
        test   byte ptr [eax + 1], 0x40
        je     loc_1_6645
        call   sub_1_A5B7
loc_1_6645:
        mov    eax, esi
        call   sub_1_A53E
        mov    ebx, dword ptr [ebp + 0x1c]
        mov    eax, dword ptr [ebp + 0x28]
        call   sub_1_684B
        pushf  
        mov    eax, ebx
        call   sub_1_A53E
        popf   
        jne    loc_1_6667
        xor    eax, eax
        ret    
loc_1_6667:
        mov    eax, dword ptr [ebp + 0x28]
        mov    ebx, dword ptr [eax + 0x54]
        mov    dword ptr [ebp + 0x28], ebx
        movzx  ecx, byte ptr [eax + 0xb9]
        lea    esi, [eax + 0xae]
        mov    edi, dword ptr [ebp + 0x14]
        rep movsb byte ptr es:[edi], byte ptr [esi]
        xor    al, al
        stosb  byte ptr es:[edi], al
        mov    eax, 0xffffffff
        ret    
loc_1_668B:
        mov    eax, esi
        call   sub_1_A53E
        mov    eax, dword ptr [ebp + 0x1c]
        call   sub_1_A53E
        mov    eax, dword ptr [ebp + 0x28]
        jmp    loc_1_6667
loc_1_669F:
        mov    eax, dword ptr [ebp + 0x1c]
        call   sub_1_A53E
loc_1_66A7:
        mov    eax, 5
        ret    
sub_1_66AD:
        mov    dword ptr [ebp + 0x10], 0x228c ; -> loc_1_228C
        mov    ebx, dword ptr [ebp + 0x1c]
        mov    eax, 0xcc
        call   sub_1_A1CE
        jb     loc_1_681E
        mov    edi, eax
        mov    ecx, 0x33
        mov    esi, ebx
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    edi, eax
        mov    dword ptr [ebp + 0x1c], edi
        mov    edx, dword ptr [ebp + 0x28]
        mov    byte ptr [edi + 0xac], dl
        mov    byte ptr [edi + 0xad], dl
        xor    eax, eax
        mov    dword ptr [edi + 0x9c], eax
        mov    dword ptr [edi], eax
        mov    dword ptr [edi + 4], eax
        mov    byte ptr [edi + 0xbb], al
        mov    dword ptr [edi + 0x64], eax
        mov    dword ptr [edi + 0x68], eax
        mov    dword ptr [edi + 0x6c], eax
        mov    dword ptr [edi + 0x78], eax
        mov    dword ptr [edi + 0x80], eax
        mov    dword ptr [edi + 0x84], eax
        mov    dword ptr [edi + 0x88], eax
        mov    dword ptr [edi + 0x8c], eax
        mov    dword ptr [edi + 0xbc], eax
        mov    dword ptr [edi + 0xc0], eax
        mov    dword ptr [edi + 0xc4], eax
        mov    dword ptr [edi + 0xc8], eax
        mov    word ptr [edi + 0x52], ax
        mov    dword ptr [edi + 0x90], eax
        mov    byte ptr [edi + 0x1f], 0x30
        mov    eax, dword ptr [0xb87c]       ; -> loc_1_B87C
        mov    dword ptr [edi + 0x74], eax
        mov    eax, dword ptr [0xb874]       ; -> loc_1_B874
        mov    dword ptr [edi + 0x7c], eax
        mov    eax, dword ptr [0xb878]       ; -> obj1:0xB878
        mov    dword ptr [edi + 0x70], eax
        push   0x800
        VxDCall  IFSMgr.IFSMgr_GetHeap           ; cd200d004000
        add    esp, 4
        or     eax, eax
        je     loc_1_67FD
        mov    esi, eax
        push   esi
        mov    ebx, edi
        mov    ecx, 1
        sub    edi, edi
        call   sub_1_1458
        pop    esi
        jb     loc_1_67F3
        push   esi
        call   sub_1_68DF
        pop    esi
        jb     loc_1_67F3
        push   esi
        VxDCall  IFSMgr.IFSMgr_RetHeap           ; cd200e004000
        add    esp, 4
        mov    ebx, dword ptr [ebp + 0x1c]
        mov    eax, 0xb748                   ; -> loc_1_B748
loc_1_67A0:
        mov    eax, dword ptr [eax]
        cmp    eax, 0xb748                   ; -> loc_1_B748
        je     loc_1_67C5
        call   sub_1_684B
        jne    loc_1_67A0
        mov    ecx, dword ptr [eax + 0x14]
        cmp    dword ptr [ebx + 0x14], ecx
        jne    loc_1_67A0
        xchg   ebx, eax
        call   sub_1_A1F8
        xchg   ebx, eax
        mov    eax, 0x55
        ret    
loc_1_67C5:
        pushfd 
        cli    
        mov    eax, 0xb748                   ; -> loc_1_B748
        mov    ecx, dword ptr [eax]
        mov    dword ptr [ebx], ecx
        mov    dword ptr [ebx + 4], eax
        mov    dword ptr [ecx + 4], ebx
        mov    dword ptr [eax], ebx
        popfd  
        or     word ptr [ebx + 0x52], 0x800
        mov    edx, ebx
        call   sub_1_6DDA
        call   sub_1_6529
        call   sub_1_6556
        xor    eax, eax
        ret    
loc_1_67F3:
        push   esi
        VxDCall  IFSMgr.IFSMgr_RetHeap           ; cd200e004000
        add    esp, 4
loc_1_67FD:
        mov    eax, dword ptr [ebp + 0x1c]
        mov    esi, dword ptr [eax + 0x14]
        call   sub_1_A1F8
        mov    eax, 0xb748                   ; -> loc_1_B748
loc_1_680D:
        mov    eax, dword ptr [eax]
        cmp    eax, 0xb748                   ; -> loc_1_B748
        je     loc_1_681E
        cmp    dword ptr [eax + 0x14], esi
        jne    loc_1_680D
        mov    dword ptr [ebp + 0x1c], eax
loc_1_681E:
        mov    dword ptr [ebp + 0x10], 0xb790 ; -> obj1:0xB790
        xor    eax, eax
        ret    
loc_1_6828:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, 5
        mov    word ptr [ecx + 0x1a], ax
        ret    
loc_1_6836:
        mov    ecx, dword ptr [esp + 4]
        cmp    byte ptr [ecx + 4], 2
        jne    loc_1_3F50
        xor    eax, eax
        mov    word ptr [ecx + 0x1a], ax
        ret    
sub_1_684B:
        mov    edx, dword ptr [ebx + 0x54]
        cmp    dword ptr [eax + 0x54], edx
        jne    loc_1_68DE
        lea    edi, [eax + 0x20]
        lea    esi, [ebx + 0x20]
        mov    ecx, 6
        repe cmpsd dword ptr [esi], dword ptr es:[edi]
        jne    loc_1_68DE
        movzx  edx, word ptr [eax + 0x44]
        cmp    word ptr [ebx + 0x44], dx
        jne    loc_1_68DE
        lea    edi, [eax + 0xae]
        lea    esi, [ebx + 0xae]
        mov    ecx, 0xd
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        jne    loc_1_68DE
        mov    dl, byte ptr [ebx + 0xac]
        mov    esi, dword ptr [eax + 0x14]
        mov    ecx, 0xb748                   ; -> loc_1_B748
loc_1_6893:
        mov    ecx, dword ptr [ecx]
        cmp    ecx, 0xb748                   ; -> loc_1_B748
        je     loc_1_68B6
        cmp    dword ptr [ecx + 0x14], esi
        jne    loc_1_6893
        cmp    byte ptr [ecx + 0xac], dl
        jne    loc_1_6893
        test   byte ptr [eax + 0x53], 2
        je     loc_1_68C2
        test   byte ptr [ebx + 0x53], 2
        jne    loc_1_68BA
loc_1_68B6:
        or     ebx, ebx
        jmp    loc_1_68DE
loc_1_68BA:
        mov    edx, dword ptr [eax + 0x5c]
        cmp    dword ptr [ebx + 0x5c], edx
        jmp    loc_1_68DE
loc_1_68C2:
        mov    edx, dword ptr [eax + 0x58]
        cmp    dword ptr [ebx + 0x58], edx
        jne    loc_1_68DE
        test   byte ptr [ebx + 0x53], 2
        je     loc_1_68DE
        mov    edx, dword ptr [ebx + 0x5c]
        mov    dword ptr [eax + 0x5c], edx
        or     word ptr [eax + 0x52], 0x200
        cmp    eax, eax
loc_1_68DE:
        ret    
sub_1_68DF:
        mov    edi, dword ptr [ebp + 0x1c]
        and    word ptr [edi + 0x52], 0xfeff
        mov    eax, dword ptr [edi + 0x14]
        test   byte ptr [eax + 4], 0x40
        jne    loc_1_6901
        movzx  eax, byte ptr [edi + 0xac]
        bt     dword ptr [0xb868], eax       ; -> obj1:0xB868
        jae    loc_1_6907
loc_1_6901:
        or     word ptr [edi + 0x52], 0x100
loc_1_6907:
        mov    byte ptr [edi + 0x1e], 1
        or     dword ptr [edi + 0xa0], 0xffffffff
        or     dword ptr [edi + 0xa4], 0xffffffff
        cmp    word ptr [esi + 0x16], 0
        je     loc_1_6A7F
        mov    byte ptr [edi + 0x48], 0
        cmp    byte ptr [esi + 0x26], 0x29
        je     loc_1_6936
        or     word ptr [edi + 0x52], 0x80
        jmp    loc_1_695E
loc_1_6936:
        mov    eax, dword ptr [esi + 0x27]
        mov    dword ptr [edi + 0x54], eax
        push   edi
        push   esi
        lea    edi, [edi + 0xae]
        lea    esi, [esi + 0x2b]
        mov    ecx, 0xb
        rep movsb byte ptr es:[edi], byte ptr [esi]
        lea    edi, [edi - 0xb]
        call   sub_1_6EC3
        pop    esi
        pop    edi
        mov    byte ptr [edi + 0xb9], al
loc_1_695E:
        lea    esi, [esi + 0xb]
        lodsw  ax, word ptr [esi]
        mov    word ptr [edi + 0x20], ax
        and    al, al
        jne    loc_1_6CA6
        cmp    ah, 2
        je     loc_1_6987
        cmp    ah, 4
        je     loc_1_6987
        cmp    ah, 8
        je     loc_1_6987
        cmp    ah, 0x10
        jne    loc_1_6CA6
loc_1_6987:
        mov    edx, eax
        dec    eax
        mov    word ptr [edi + 0x22], ax
        lodsb  al, byte ptr [esi]
        sub    al, 1
        jb     loc_1_6CA6
        mov    byte ptr [edi + 0x24], al
        inc    al
        movzx  eax, al
        bsf    ebx, eax
        mov    byte ptr [edi + 0x25], bl
        btr    eax, ebx
        or     eax, eax
        jne    loc_1_6CA6
        lodsw  ax, word ptr [esi]
        mov    word ptr [edi + 0x26], ax
        lodsb  al, byte ptr [esi]
        mov    byte ptr [edi + 0x46], al
        and    al, al
        je     loc_1_6CA6
        cmp    al, 3
        jae    loc_1_6CA6
        mov    bh, al
        lodsw  ax, word ptr [esi]
        mov    word ptr [edi + 0x44], ax
        movzx  edx, dx
        shr    edx, 5
        lea    eax, [edx + eax - 1]
        mov    ecx, edx
        xor    edx, edx
        div    ecx
        mov    ecx, eax
        add    esi, 2
        lodsb  al, byte ptr [esi]
        mov    byte ptr [edi + 0xba], al
        lodsw  ax, word ptr [esi]
        movzx  eax, ax
        mov    dword ptr [edi + 0x30], eax
        movzx  edx, bh
        mul    edx
        movzx  edx, word ptr [edi + 0x26]
        add    eax, edx
        mov    dword ptr [edi + 0x34], eax
        add    eax, ecx
        mov    dword ptr [edi + 0x28], eax
        mov    cl, bl
        cmp    word ptr [esi - 5], 0
        jne    loc_1_6A17
        mov    ebx, dword ptr [esi + 8]
        jmp    loc_1_6A1B
loc_1_6A17:
        movzx  ebx, word ptr [esi - 5]
loc_1_6A1B:
        mov    dword ptr [edi + 8], ebx
        sub    ebx, eax
        jb     loc_1_6CA6
        shr    ebx, cl
        inc    ebx
        mov    dword ptr [edi + 0x2c], ebx
        mov    ebx, dword ptr [edi + 0x30]
        movzx  eax, word ptr [edi + 0x20]
        imul   eax, ebx
        cmp    dword ptr [edi + 0x2c], 0xff6
        jae    loc_1_6A5E
        add    eax, eax
        sub    edx, edx
        mov    ebx, 3
        div    ebx
        dec    eax
        mov    cl, 1
        cmp    dword ptr [edi + 0x2c], eax
        jbe    loc_1_6BF8
        mov    dword ptr [edi + 0x2c], eax
        jmp    loc_1_6BF8
loc_1_6A5E:
        shr    eax, 1
        mov    cl, 2
        cmp    eax, 0xff7
        jb     loc_1_6BF8
        dec    eax
        cmp    dword ptr [edi + 0x2c], eax
        jbe    loc_1_6BF8
        mov    dword ptr [edi + 0x2c], eax
        jmp    loc_1_6BF8
loc_1_6A7F:
        cmp    byte ptr [esi + 0x42], 0x29
        jne    loc_1_6CA6
        cmp    dword ptr [esi + 0x52], 0x33544146
        jne    loc_1_6CA6
        cmp    dword ptr [esi + 0x56], 0x20202032
        jne    loc_1_6CA6
        cmp    word ptr [esi + 0x2a], 0
        ja     loc_1_6CA6
        mov    eax, dword ptr [esi + 0x43]
        mov    dword ptr [edi + 0x54], eax
        push   edi
        push   esi
        lea    edi, [edi + 0xae]
        lea    esi, [esi + 0x47]
        mov    ecx, 0xb
        rep movsb byte ptr es:[edi], byte ptr [esi]
        sub    edi, 0xb
        call   sub_1_6EC3
        pop    esi
        pop    edi
        mov    byte ptr [edi + 0xb9], al
        movzx  eax, word ptr [esi + 0xb]
        mov    word ptr [edi + 0x20], ax
        and    al, al
        jne    loc_1_6CA6
        cmp    ah, 2
        je     loc_1_6AFE
        cmp    ah, 4
        je     loc_1_6AFE
        cmp    ah, 8
        je     loc_1_6AFE
        cmp    ah, 0x10
        jne    loc_1_6CA6
loc_1_6AFE:
        mov    edx, eax
        dec    eax
        mov    word ptr [edi + 0x22], ax
        movzx  eax, byte ptr [esi + 0xd]
        sub    eax, 1
        jb     loc_1_6CA6
        mov    byte ptr [edi + 0x24], al
        inc    eax
        bsf    ebx, eax
        mov    byte ptr [edi + 0x25], bl
        btr    eax, ebx
        or     eax, eax
        jne    loc_1_6CA6
        movzx  ecx, word ptr [esi + 0xe]
        mov    word ptr [edi + 0x26], cx
        or     ecx, ecx
        je     loc_1_6CA6
        movzx  eax, word ptr [esi + 0x30]
        or     eax, eax
        je     loc_1_6B49
        cmp    eax, ecx
        jae    loc_1_6B49
        mov    dword ptr [edi + 0xa0], eax
loc_1_6B49:
        movzx  eax, word ptr [esi + 0x32]
        or     eax, eax
        je     loc_1_6B5B
        cmp    eax, ecx
        jae    loc_1_6B5B
        mov    dword ptr [edi + 0xa4], eax
loc_1_6B5B:
        mov    al, byte ptr [esi + 0x10]
        mov    byte ptr [edi + 0x46], al
        or     al, al
        je     loc_1_6CA6
        cmp    al, 2
        ja     loc_1_6CA6
        mov    al, byte ptr [esi + 0x15]
        mov    byte ptr [edi + 0xba], al
        mov    eax, dword ptr [esi + 0x24]
        mov    dword ptr [edi + 0x30], eax
        cmp    byte ptr [edi + 0x46], 2
        jb     loc_1_6B88
        add    eax, eax
loc_1_6B88:
        add    eax, ecx
        mov    dword ptr [edi + 0x28], eax
        mov    cl, bl
        mov    ebx, dword ptr [esi + 0x20]
        mov    dword ptr [edi + 8], ebx
        sub    ebx, eax
        jb     loc_1_6CA6
        shr    ebx, cl
        inc    ebx
        mov    dword ptr [edi + 0x2c], ebx
        mov    eax, dword ptr [edi + 0x30]
        shr    edx, 2
        mul    edx
        or     edx, edx
        jne    loc_1_6BB8
        dec    eax
        cmp    dword ptr [edi + 0x2c], eax
        jbe    loc_1_6BB8
        mov    dword ptr [edi + 0x2c], eax
loc_1_6BB8:
        cmp    dword ptr [edi + 0x2c], 0xfff6
        jb     loc_1_6CA6
        mov    eax, dword ptr [esi + 0x2c]
        mov    dword ptr [edi + 0x3c], eax
        cmp    eax, 2
        jb     loc_1_6CA6
        cmp    dword ptr [edi + 0x2c], eax
        jb     loc_1_6CA6
        movzx  eax, word ptr [esi + 0x28]
        xor    ecx, ecx
        test   al, 0x80
        je     loc_1_6BF8
        or     ecx, 4
        and    al, 0xf
        mov    byte ptr [edi + 0x48], al
        cmp    byte ptr [edi + 0x46], al
        jbe    loc_1_6CA6
loc_1_6BF8:
        or     cl, 0x20
        mov    eax, dword ptr [edi + 0x14]
        cmp    word ptr [eax + 8], 0x10
        jb     loc_1_6C08
        and    cl, 0xdf
loc_1_6C08:
        or     byte ptr [edi + 0x52], cl
        and    word ptr [edi + 0x52], 0xfbff
        test   byte ptr [0xb86f], 0x80       ; -> loc_1_B86F
        jne    loc_1_6C2C
        movzx  ecx, byte ptr [edi + 0xac]
        bt     dword ptr [0xb86c], ecx       ; -> obj1:0xB86C
        jae    loc_1_6C38
        jmp    loc_1_6C3E
loc_1_6C2C:
        test   byte ptr [eax + 1], 0x40
        je     loc_1_6C3E
        test   byte ptr [eax + 2], 1
        jne    loc_1_6C3E
loc_1_6C38:
        or     word ptr [edi + 0x52], 0x400
loc_1_6C3E:
        movzx  eax, word ptr [edi + 0x20]
        bsf    ecx, eax
        mov    byte ptr [edi + 0x4c], cl
        add    cl, byte ptr [edi + 0x25]
        mov    byte ptr [edi + 0x47], cl
        or     eax, 0xffffffff
        mov    dword ptr [edi + 0x40], eax
        shl    eax, cl
        not    eax
        mov    word ptr [edi + 0x4a], ax
        mov    dword ptr [edi + 0x38], 2
        mov    eax, 0x1000
        mov    cl, byte ptr [edi + 0x4c]
        shr    eax, cl
        mov    byte ptr [edi + 0x4d], al
        dec    eax
        mov    byte ptr [edi + 0x4e], al
loc_1_6C74:
        and    eax, dword ptr [edi + 0x28]
        mov    dword ptr [edi + 0xc], eax
        movzx  ebx, word ptr [edi + 0x4a]
        inc    ebx
        cmp    ebx, 0x1000
        jb     loc_1_6C8C
        mov    ebx, 0x1000
loc_1_6C8C:
        setb   al
        inc    eax
        mov    byte ptr [edi + 0x4f], al
        neg    ebx
        mov    word ptr [edi + 0x50], bx
        movzx  eax, word ptr [edi + 0x26]
        add    eax, dword ptr [edi + 0x30]
        mov    dword ptr [edi + 0x18], eax
        mov    edx, edi
        ret    
loc_1_6CA6:
        mov    edi, dword ptr [ebp + 0x1c]
        mov    ebx, dword ptr [edi + 0x14]
        test   byte ptr [ebx + 1], 0x40
        je     loc_1_6DD8
        mov    ecx, dword ptr [ebx + 0x10]
        cmp    ecx, 0x200
        jne    loc_1_6DD8
        push   edi
        push   esi
        mov    ebx, edi
        mov    ecx, 1
        mov    edi, ecx
        call   sub_1_1458
        pop    esi
        pop    edi
        jb     loc_1_6DD8
        mov    ah, byte ptr [esi]
        cmp    ah, 0xf8
        jb     loc_1_6DD8
        mov    al, 1
        mov    ebx, 0x4008
        mov    ecx, 0x140
        mov    edx, 0x101
        test   ah, 2
        jne    loc_1_6D05
        inc    al
        inc    bl
        add    ecx, 0x28
loc_1_6D05:
        test   ah, 1
        je     loc_1_6D12
        add    ecx, ecx
        mov    bh, 0x70
        inc    dh
        inc    dl
loc_1_6D12:
        mov    byte ptr [edi + 0xba], ah
        mov    dword ptr [edi + 8], ecx
        movzx  ecx, al
        mov    dword ptr [edi + 0x30], ecx
        inc    ecx
        mov    dword ptr [edi + 0x18], ecx
        add    ecx, ecx
        dec    ecx
        mov    dword ptr [edi + 0x34], ecx
        movzx  ecx, dh
        bsf    ecx, ecx
        mov    byte ptr [edi + 0x25], cl
        dec    dh
        mov    byte ptr [edi + 0x24], dh
        mov    eax, 0x200
        mov    word ptr [edi + 0x20], ax
        shl    eax, cl
        bsf    ecx, eax
        mov    byte ptr [edi + 0x47], cl
        dec    eax
        mov    word ptr [edi + 0x4a], ax
        movzx  ecx, bh
        mov    word ptr [edi + 0x44], cx
        shr    ecx, 4
        add    ecx, dword ptr [edi + 0x18]
        add    ecx, dword ptr [edi + 0x30]
        mov    dword ptr [edi + 0x28], ecx
        mov    eax, dword ptr [edi + 8]
        sub    eax, ecx
        mov    cl, byte ptr [edi + 0x25]
        shr    eax, cl
        inc    eax
        mov    dword ptr [edi + 0x2c], eax
        mov    eax, dword ptr [edi + 8]
        mov    word ptr [edi + 0x22], 0x1ff
        mov    byte ptr [edi + 0x4c], 9
        mov    byte ptr [edi + 0x46], 2
        mov    word ptr [edi + 0x26], 1
        mov    byte ptr [edi + 0x4d], 8
        mov    byte ptr [edi + 0x4e], 7
        mov    word ptr [edi + 0x52], 0x1181
        mov    dword ptr [edi + 0xae], 0
        mov    dword ptr [edi + 0xb2], 0
        mov    word ptr [edi + 0xb6], 0
        mov    byte ptr [edi + 0xb8], 0
        mov    byte ptr [edi + 0xb9], 0
        mov    byte ptr [edi + 0x1e], 1
        or     dword ptr [edi + 0x40], 0xffffffff
        mov    dword ptr [edi + 0x38], 2
        movzx  eax, byte ptr [edi + 0x4d]
        jmp    loc_1_6C74
loc_1_6DD8:
        stc    
        ret    
sub_1_6DDA:
        mov    ecx, dword ptr [edx + 0x14]
        movzx  ebx, word ptr [ecx + 0xa]
        mov    esi, dword ptr [ecx + 0xc]
        mov    ah, 2
        call   sub_1_A4E1
        mov    esi, ebx
        mov    dword ptr [esi + 0x44], ecx
        mov    al, byte ptr [edx + 0xac]
        mov    byte ptr [esi + 0x4d], al
        mov    dword ptr [esi + 8], 0x500
        bt     dword ptr [ebp + 0x38], 0xa
        sbb    eax, eax
        and    eax, 0x808
        or     dword ptr [esi + 8], eax
        mov    word ptr [esi + 4], 0x29
        mov    word ptr [esi + 0x2e], 0xdb00
        push   edx
        VxDCall  BLOCKDEV.BlockDev_Send_Command  ; cd2004001000
        pop    edx
        and    word ptr [edx + 0x52], 0xffef
        cmp    word ptr [esi + 6], 0x10
        jae    loc_1_6E34
        or     word ptr [edx + 0x52], 0x10
loc_1_6E34:
        mov    ebx, esi
        call   sub_1_A4AE
        ret    
sub_1_6E3C:
        sub    esp, 4
        mov    dword ptr [esp], eax
        sub    ebx, ebx
        call   sub_1_00D0
        jb     loc_1_6EBE
        lea    edi, [ebx + 0x47]
        cmp    word ptr [ebx + 0x16], 0
        je     loc_1_6E5E
        lea    edi, [ebx + 0x2b]
        cmp    byte ptr [ebx + 0x26], 0x29
        jne    loc_1_6E7E
loc_1_6E5E:
        mov    eax, esi
        mov    esi, dword ptr [esp]
        test   esi, esi
        jne    loc_1_6E6C
        mov    esi, 0xb770                   ; -> loc_1_B770
loc_1_6E6C:
        mov    ecx, 0xb
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    esi, eax
        push   ebx
        call   sub_1_55D2
        pop    ebx
        jb     loc_1_6EBE
loc_1_6E7E:
        mov    edi, dword ptr [ebp + 0x1c]
        mov    esi, dword ptr [esp]
        test   esi, esi
        jne    loc_1_6E9F
        mov    esi, 0xb770                   ; -> loc_1_B770
        cmp    word ptr [ebx + 0x16], 0
        je     loc_1_6E9F
        cmp    byte ptr [ebx + 0x26], 0x29
        je     loc_1_6E9F
        mov    esi, 0xb77b                   ; -> loc_1_B77B
loc_1_6E9F:
        lea    edi, [edi + 0xae]
        mov    ecx, 0xb
        rep movsb byte ptr es:[edi], byte ptr [esi]
        lea    edi, [edi - 0xb]
        call   sub_1_6EC3
        mov    edi, dword ptr [ebp + 0x1c]
        mov    byte ptr [edi + 0xb9], al
        clc    
loc_1_6EBE:
        lea    esp, [esp + 4]
        ret    
sub_1_6EC3:
        mov    eax, 0xb
loc_1_6EC8:
        cmp    byte ptr [eax + edi - 1], 0x20
        je     loc_1_6ED6
        cmp    byte ptr [eax + edi - 1], 0
        jne    loc_1_6ED9
loc_1_6ED6:
        dec    eax
        jne    loc_1_6EC8
loc_1_6ED9:
        ret    
sub_1_6EDA:
        mov    edx, dword ptr [ebp + 0x1c]
        mov    eax, dword ptr [edx + 0x14]
        test   byte ptr [eax + 1], 0x40
        jne    loc_1_6FE3
        test   byte ptr [edx + 0x52], 0x10
        jne    loc_1_6FE3
        test   byte ptr [edx + 0x53], 8
        jne    loc_1_6FE3
        test   byte ptr [edx + 0x52], 3
        je     loc_1_6FE3
        cmp    byte ptr [edx + 0xac], 2
        ja     loc_1_6FE3
        mov    edi, dword ptr [0xb754]       ; -> loc_1_B754
loc_1_6F1B:
        movzx  eax, byte ptr [edx + 0xac]
        cmp    byte ptr [edi], al
        je     loc_1_6F40
        mov    edi, dword ptr [edi + 0x19]
        cmp    di, -1
        je     loc_1_6FE3
        movzx  ecx, di
        shr    edi, 0x10
        shl    edi, 4
        add    edi, ecx
        jmp    loc_1_6F1B
loc_1_6F40:
        cmp    byte ptr [edi + 0x17], 0xff
        je     loc_1_6FE3
        mov    al, byte ptr [edx + 0xba]
        cmp    byte ptr [edi + 0x17], al
        jne    loc_1_6FB1
        mov    ax, word ptr [edx + 0x20]
        cmp    word ptr [edi + 2], ax
        jne    loc_1_6FB1
        mov    al, byte ptr [edx + 0x24]
        cmp    byte ptr [edi + 4], al
        jne    loc_1_6FB1
        mov    al, byte ptr [edx + 0x25]
        cmp    byte ptr [edi + 5], al
        jne    loc_1_6FB1
        mov    eax, dword ptr [edx + 0x2c]
        cmp    word ptr [edi + 0xd], ax
        jne    loc_1_6FB1
        mov    ax, word ptr [edx + 0x44]
        cmp    word ptr [edi + 9], ax
        jne    loc_1_6FB1
        mov    ax, word ptr [edx + 0x26]
        cmp    word ptr [edi + 6], ax
        jne    loc_1_6FB1
        mov    al, byte ptr [edx + 0x46]
        cmp    byte ptr [edi + 8], al
        jne    loc_1_6FB1
        mov    eax, dword ptr [edx + 0x30]
        cmp    word ptr [edi + 0xf], ax
        jne    loc_1_6FB1
        mov    eax, dword ptr [edx + 0x28]
        cmp    word ptr [edi + 0xb], ax
        jne    loc_1_6FB1
        mov    eax, dword ptr [edx + 0x34]
        cmp    word ptr [edi + 0x11], ax
        jne    loc_1_6FB1
        jmp    loc_1_6FE3
loc_1_6FB1:
        mov    al, byte ptr [edx + 0xac]
        or     al, 0x41
        mov    byte ptr [0xb914], al         ; -> loc_1_B914
        lea    ecx, [0xb914]                 ; -> loc_1_B914
        push   ecx
        push   0xc0003
        push   0                             ; -> loc_2_0000
        VMMCall  VMM._LocalizeStackSprintf       ; cd2059010100
        pushal 
        mov    esi, eax
        mov    eax, 1
        VMMCall  VMM.Fatal_Error_Handler         ; cd20be000100
loc_1_6FE3:
        ret    
sub_1_6FE4:
        mov    edi, dword ptr [ebp + 0x1c]
        mov    ecx, dword ptr [ebp]
        shr    ecx, 0xc
        je     loc_1_6FFE
        inc    ecx
        mov    ah, byte ptr [0xb6f8]         ; -> obj1:0xB6F8
        VxDCall  Dev_048B.Service_0003h          ; cd2003008b04
        jb     loc_1_7008
loc_1_6FFE:
        test   byte ptr [ebp + 0x37], 1
        jne    loc_1_7008
        or     byte ptr [ebp + 0x3a], 4
loc_1_7008:
        mov    esi, dword ptr [ebp + 0x20]
        call   sub_1_774E
        jb     loc_1_707E
        mov    esi, dword ptr [ebp + 0x20]
        call   sub_1_7704
        mov    esi, dword ptr [ebp + 0x20]
        movzx  eax, word ptr [ebp + 0x3c]
        and    ax, word ptr [edi + 0x22]
        mov    dword ptr [ebp + 0x6c], eax
        cmp    dword ptr [ebp + 0x4c], 0
        je     loc_1_7031
        inc    dword ptr [ebp + 0x48]
loc_1_7031:
        cmp    dword ptr [ebp + 0x44], 0
        je     loc_1_703A
        inc    dword ptr [ebp + 0x48]
loc_1_703A:
        mov    esi, dword ptr [ebp + 0x20]
        mov    ecx, dword ptr [ebp + 0x48]
        call   sub_1_76B7
        jb     loc_1_707E
        mov    eax, dword ptr [ebp + 0x44]
        mov    edx, dword ptr [ebp + 0x4c]
        cmp    ecx, dword ptr [ebp + 0x48]
        je     loc_1_7054
        sub    edx, edx
loc_1_7054:
        push   ecx
        test   byte ptr [ebp + 0x37], 0x10
        jne    loc_1_7071
        call   sub_1_70F6
        jb     loc_1_707D
loc_1_7062:
        sub    ecx, ecx
        mov    dword ptr [ebp + 0x44], ecx
        mov    dword ptr [ebp + 0x6c], ecx
        pop    ecx
        sub    dword ptr [ebp + 0x48], ecx
        jne    loc_1_703A
        ret    
loc_1_7071:
        mov    esi, dword ptr [ebp + 0x20]
        call   sub_1_7080
        jb     loc_1_707D
        jmp    loc_1_7062
loc_1_707D:
        pop    eax
loc_1_707E:
        stc    
        ret    
sub_1_7080:
        push   edi
        xchg   edi, ebx
        or     eax, edx
        jne    loc_1_70A3
        add    edi, dword ptr [ebx + 0x10]
        mov    esi, dword ptr [ebp + 0x14]
        push   ebx
        push   ecx
        call   sub_1_1458
        pop    eax
        pop    ebx
        jb     loc_1_70A1
        mov    cl, byte ptr [ebx + 0x4c]
        shl    eax, cl
        add    dword ptr [ebp + 0x14], eax
        clc    
loc_1_70A1:
        pop    edi
        ret    
loc_1_70A3:
        stc    
        jmp    loc_1_70A1
sub_1_70A6:
        push   esi
        call   sub_1_774E
        pop    esi
        jb     loc_1_70D8
        push   esi
        call   sub_1_7704
        pop    esi
        cmp    dword ptr [ebp + 0x48], 0
        je     loc_1_70D8
loc_1_70BC:
        mov    ecx, dword ptr [ebp + 0x48]
        push   esi
        call   sub_1_76B7
        pop    esi
        jb     loc_1_70D8
        sub    dword ptr [ebp + 0x48], ecx
        call   sub_1_70D9
        jb     loc_1_70D8
        cmp    dword ptr [ebp + 0x48], 0
        jne    loc_1_70BC
loc_1_70D8:
        ret    
sub_1_70D9:
        push   esi
        add    ebx, dword ptr [edi + 0x10]
        mov    esi, dword ptr [ebp + 0x14]
        push   edi
        push   ecx
        call   sub_1_1574
        pop    eax
        pop    edi
        jb     loc_1_70F4
        mov    cl, byte ptr [edi + 0x4c]
        shl    eax, cl
        add    dword ptr [ebp + 0x14], eax
        clc    
loc_1_70F4:
        pop    esi
        ret    
sub_1_70F6:
        mov    dword ptr [ebp + 0x58], eax
        mov    dword ptr [ebp + 0x5c], edx
        mov    dword ptr [ebp + 0x60], ebx
        mov    dword ptr [ebp + 0x68], ecx
        mov    dword ptr [ebp + 0x70], 0
        test   byte ptr [ebp + 0x37], 1
        jne    loc_1_748A
loc_1_7113:
        and    eax, eax
        je     loc_1_7118
        dec    ecx
loc_1_7118:
        and    edx, edx
        je     loc_1_711D
        dec    ecx
loc_1_711D:
        mov    eax, ecx
        mov    cl, byte ptr [edi + 0x4c]
        shl    eax, cl
        add    eax, dword ptr [ebp + 0x58]
        add    eax, edx
        mov    edx, ebx
        sub    edx, dword ptr [edi + 0xc]
        movzx  esi, byte ptr [edi + 0x4e]
        not    esi
        and    edx, esi
        add    edx, dword ptr [edi + 0xc]
        mov    dword ptr [ebp + 0x64], edx
        sub    ebx, edx
        add    dword ptr [ebp + 0x68], ebx
        shl    ebx, cl
        mov    esi, dword ptr [ebp + 0x6c]
        add    esi, ebx
        lea    edx, [eax + esi]
        mov    ebx, 0x1000
        cmp    edx, ebx
        ja     loc_1_7164
        mov    dword ptr [ebp + 0x58], eax
        sub    ebx, ebx
        mov    dword ptr [ebp + 0x5c], ebx
        mov    bl, byte ptr [edi + 0x4d]
        mov    dword ptr [ebp + 0x68], ebx
        jmp    sub_1_7184
loc_1_7164:
        sub    ebx, esi
        and    esi, esi
        jne    loc_1_716C
        mov    ebx, esi
loc_1_716C:
        mov    dword ptr [ebp + 0x58], ebx
        and    edx, 0xfff
        mov    dword ptr [ebp + 0x5c], edx
        movzx  eax, byte ptr [edi + 0x4e]
        add    dword ptr [ebp + 0x68], eax
        not    eax
        and    dword ptr [ebp + 0x68], eax
sub_1_7184:
        mov    ebx, dword ptr [ebp + 0x64]
        call   sub_1_00B0
        jb     loc_1_71A3
        clc    
        call   sub_1_7502
        jb     loc_1_71A2
        cmp    dword ptr [ebp + 0x68], 0
        jne    sub_1_7184
        mov    ebx, dword ptr [ebp + 0x70]
        add    dword ptr [ebp + 0x14], ebx
loc_1_71A2:
        ret    
loc_1_71A3:
        movzx  ecx, byte ptr [edi + 0x4d]
        mov    eax, dword ptr [ebp + 0x68]
        cmp    ecx, eax
        je     loc_1_71F6
        sub    eax, ecx
        mov    ebx, dword ptr [ebp + 0x64]
        add    ebx, eax
        mov    edx, eax
        call   sub_1_00B0
        jb     loc_1_71F6
        push   dword ptr [ebp + 0x64]
        push   dword ptr [ebp + 0x14]
        add    dword ptr [ebp + 0x64], edx
        mov    cl, byte ptr [edi + 0x4c]
        shl    edx, cl
        mov    ecx, dword ptr [ebp + 0x58]
        jecxz  loc_1_71D8
        lea    edx, [ecx + edx - 0x1000]
loc_1_71D8:
        add    dword ptr [ebp + 0x14], edx
        stc    
        push   dword ptr [ebp + 0x14]
        call   sub_1_7502
        lahf   
        pop    ebx
        sub    ebx, dword ptr [ebp + 0x14]
        sub    dword ptr [ebp + 0x70], ebx
        sahf   
        pop    dword ptr [ebp + 0x14]
        pop    dword ptr [ebp + 0x64]
        jae    loc_1_71A3
        ret    
loc_1_71F6:
        test   byte ptr [ebp + 0x3a], 2
        jne    loc_1_746E
        mov    ecx, dword ptr [ebp + 0x68]
        mov    ebx, dword ptr [ebp + 0x64]
        cmp    dword ptr [edi + 8], ebx
        jbe    loc_1_746E
        movzx  edx, byte ptr [edi + 0x4d]
loc_1_7213:
        call   sub_1_00B0
        jae    loc_1_7222
        add    ebx, edx
        sub    ecx, edx
        jne    loc_1_7213
        jmp    loc_1_725B
loc_1_7222:
        sub    ecx, edx
        je     sub_1_7184
        push   dword ptr [ebp + 0x5c]
        push   dword ptr [ebp + 0x70]
        push   ecx
        sub    dword ptr [ebp + 0x68], ecx
        sub    eax, eax
        mov    dword ptr [ebp + 0x5c], eax
        mov    dword ptr [ebp + 0x70], eax
        mov    eax, dword ptr [ebp + 0x64]
        add    eax, dword ptr [ebp + 0x68]
        push   eax
        call   sub_1_7184
        pop    dword ptr [ebp + 0x64]
        pop    dword ptr [ebp + 0x68]
        pop    dword ptr [ebp + 0x70]
        pop    dword ptr [ebp + 0x5c]
        jae    sub_1_7184
        ret    
loc_1_725B:
        sub    esi, esi
        cmp    dword ptr [ebp + 0x5c], esi
        je     loc_1_7273
        movzx  eax, byte ptr [edi + 0x4d]
        mov    ebx, dword ptr [ebp + 0x68]
        sub    ebx, eax
        add    ebx, dword ptr [ebp + 0x64]
        call   sub_1_7608
loc_1_7273:
        push   esi
        sub    esi, esi
        cmp    dword ptr [ebp + 0x58], esi
        je     loc_1_7283
        mov    ebx, dword ptr [ebp + 0x64]
        call   sub_1_7608
loc_1_7283:
        push   esi
        call   sub_1_A3B9
        mov    edx, dword ptr [ebp + 0x64]
        add    edx, dword ptr [edi + 0x10]
        mov    dword ptr [ebx + 0x10], edx
        mov    ecx, dword ptr [ebp + 0x68]
        mov    dword ptr [ebx + 0x2c], 0
        mov    dword ptr [ebx + 0x30], 0
        mov    dword ptr [ebx + 0xc], 0x154f ; -> loc_1_154F
        or     dword ptr [ebx + 8], 2
        lea    esi, [ebx + 0x50]
        mov    dword ptr [ebx + 0x1c], esi
        mov    edx, dword ptr [ebp + 0x14]
        mov    eax, dword ptr [ebp + 0x58]
        test   eax, eax
        je     loc_1_72DF
        add    edx, eax
        movzx  eax, byte ptr [edi + 0x4d]
        sub    ecx, eax
        cmp    dword ptr [esp], 0
        je     loc_1_72DF
        add    dword ptr [ebx + 0x18], eax
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [esp]
        mov    eax, dword ptr [eax + 0x10]
        mov    dword ptr [esi + 4], eax
        add    esi, 8
        xor    eax, eax
loc_1_72DF:
        add    dword ptr [ebx + 0x10], eax
        jecxz  loc_1_7311
        mov    dword ptr [esi], ecx
        mov    dword ptr [esi + 4], edx
        cmp    dword ptr [ebp + 0x5c], 0
        je     loc_1_730E
        movzx  eax, byte ptr [edi + 0x4d]
        sub    ecx, eax
        je     loc_1_72FC
        mov    dword ptr [esi], ecx
        add    esi, 8
loc_1_72FC:
        mov    edx, dword ptr [esp + 4]
        test   edx, edx
        je     loc_1_7311
        add    ecx, eax
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [edx + 0x10]
        mov    dword ptr [esi + 4], eax
loc_1_730E:
        add    esi, 8
loc_1_7311:
        mov    dword ptr [esi], 0
        add    dword ptr [ebx + 0x18], ecx
        je     loc_1_736E
        mov    eax, dword ptr [ebx + 0x18]
        add    eax, dword ptr [ebx + 0x10]
        sub    eax, dword ptr [edi + 8]
        cmc    
        sbb    edx, edx
        and    eax, edx
        sub    dword ptr [ebx + 0x18], eax
        sub    dword ptr [esi - 8], eax
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf000100
        cld    
        mov    dword ptr [edi + 0x78], eax
        mov    eax, ebx
        push   ebx
        push   edi
        call   sub_1_A558
        inc    dword ptr [0xb68c]            ; -> obj1:0xB68C
        call   sub_1_199E
        pop    edi
        pop    ebx
loc_1_7350:
        cli    
        bt     dword ptr [ebx + 0x2c], 0
        jb     loc_1_7362
        lea    eax, [ebx + 0x30]
        call   sub_1_A220
        jmp    loc_1_7350
loc_1_7362:
        sti    
        cmp    word ptr [ebx + 6], 0x10
        jae    loc_1_743D
loc_1_736E:
        call   sub_1_A4AE
        pop    esi
        test   esi, esi
        je     loc_1_7385
        mov    al, 0
        call   sub_1_0036
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
loc_1_7385:
        pop    esi
        test   esi, esi
        je     loc_1_7397
        mov    al, 0
        call   sub_1_0036
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
loc_1_7397:
        cmp    dword ptr [ebp + 0x58], 0
        je     loc_1_73A9
        clc    
        call   sub_1_7502
        jb     loc_1_7434
loc_1_73A9:
        test   byte ptr [ebp + 0x3a], 4
        je     loc_1_740C
        mov    ecx, dword ptr [ebp + 0x68]
        jecxz  loc_1_740C
        cmp    dword ptr [ebp + 0x5c], 0
        je     loc_1_73C2
        movzx  eax, byte ptr [edi + 0x4d]
        sub    ecx, eax
        je     loc_1_740C
loc_1_73C2:
        push   ecx
        mov    ebx, dword ptr [ebp + 0x64]
        call   sub_1_7608
        je     loc_1_73F6
        mov    edi, dword ptr [esi + 0x10]
        mov    edx, esi
        mov    esi, dword ptr [ebp + 0x14]
        mov    ecx, 0x400
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    byte ptr [edx + 0x25], 0
        mov    byte ptr [edx + 0x24], 0
        mov    esi, edx
        mov    edi, dword ptr [ebp + 0x1c]
        mov    al, 0
        call   sub_1_0036
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
loc_1_73F6:
        movzx  eax, byte ptr [edi + 0x4d]
        add    dword ptr [ebp + 0x64], eax
        add    dword ptr [ebp + 0x14], 0x1000
        sub    dword ptr [ebp + 0x68], eax
        pop    ecx
        sub    ecx, eax
        jne    loc_1_73C2
loc_1_740C:
        mov    ebx, dword ptr [ebp + 0x68]
        add    dword ptr [ebp + 0x64], ebx
        mov    cl, byte ptr [edi + 0x4c]
        shl    ebx, cl
        add    dword ptr [ebp + 0x14], ebx
        cmp    dword ptr [ebp + 0x5c], 0
        je     loc_1_7434
        sub    dword ptr [ebp + 0x14], 0x1000
        movzx  ecx, byte ptr [edi + 0x4d]
        sub    dword ptr [ebp + 0x64], ecx
        stc    
        call   sub_1_7502
loc_1_7434:
        lahf   
        mov    ebx, dword ptr [ebp + 0x70]
        add    dword ptr [ebp + 0x14], ebx
        sahf   
        ret    
loc_1_743D:
        call   sub_1_A4AE
        pop    esi
        test   esi, esi
        je     loc_1_7458
        mov    byte ptr [esi + 0x24], 0xff
        mov    al, 0
        call   sub_1_0036
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
loc_1_7458:
        pop    esi
        test   esi, esi
        je     loc_1_746E
        mov    byte ptr [esi + 0x24], 0xff
        mov    al, 0
        call   sub_1_0036
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
loc_1_746E:
        cmp    dword ptr [ebp + 0x58], 0
        je     loc_1_747B
        call   sub_1_7502
        jb     loc_1_7434
loc_1_747B:
        cmp    dword ptr [ebp + 0x68], 0
        je     loc_1_7434
        call   sub_1_7502
        jb     loc_1_7434
        jmp    loc_1_747B
loc_1_748A:
        test   byte ptr [ebp + 0x3a], 2
        jne    loc_1_7113
        mov    esi, eax
        or     esi, edx
        jne    loc_1_7113
        call   sub_1_74DA
        jb     loc_1_74BF
        mov    ecx, dword ptr [ebp + 0x68]
        mov    ebx, edi
        mov    edi, dword ptr [ebp + 0x60]
        mov    esi, dword ptr [ebp + 0x14]
        call   sub_1_1458
        mov    byte ptr [ebp + 0x34], 0
        mov    edi, dword ptr [ebp + 0x1c]
        mov    edx, dword ptr [ebp + 0x5c]
loc_1_74BF:
        mov    ecx, dword ptr [ebp + 0x68]
        mov    ebx, dword ptr [ebp + 0x60]
        mov    eax, dword ptr [ebp + 0x58]
        jb     loc_1_7113
        mov    eax, ecx
        mov    cl, byte ptr [edi + 0x4c]
        shl    eax, cl
        add    dword ptr [ebp + 0x14], eax
        clc    
        ret    
sub_1_74DA:
        mov    esi, ebx
        sub    ebx, dword ptr [edi + 0xc]
        movzx  eax, byte ptr [edi + 0x4e]
        not    eax
        and    ebx, eax
        add    ebx, dword ptr [edi + 0xc]
        sub    esi, ebx
        add    ecx, esi
loc_1_74EE:
        call   sub_1_00B0
        jae    loc_1_7500
        movzx  eax, byte ptr [edi + 0x4d]
        add    ebx, eax
        sub    ecx, eax
        ja     loc_1_74EE
        stc    
loc_1_7500:
        cmc    
        ret    
sub_1_7502:
        movzx  ecx, byte ptr [edi + 0x4d]
        jb     loc_1_75B0
        cmp    dword ptr [ebp + 0x58], 0
        jne    loc_1_7559
        cmp    ecx, dword ptr [ebp + 0x68]
        jae    loc_1_75B0
loc_1_751B:
        mov    ebx, dword ptr [ebp + 0x64]
        push   ecx
        call   sub_1_00D0
        pop    edx
        jb     loc_1_7558
        VxDCall  Dev_048B.Service_0007h          ; cd2007008b04
        push   edi
        xchg   ebx, esi
        movzx  ecx, word ptr [edi + 0x20]
        mov    edi, dword ptr [ebp + 0x14]
        call   sub_1_823A
        mov    dword ptr [ebp + 0x14], edi
        pop    edi
        mov    esi, ebx
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
        jb     loc_1_7558
        mov    ecx, edx
        inc    dword ptr [ebp + 0x64]
        loop   loc_1_751B
loc_1_7551:
        movzx  eax, byte ptr [edi + 0x4d]
        sub    dword ptr [ebp + 0x68], eax
loc_1_7558:
        ret    
loc_1_7559:
        mov    edx, dword ptr [ebp + 0x6c]
loc_1_755C:
        push   edx
        mov    ebx, dword ptr [ebp + 0x60]
        inc    dword ptr [ebp + 0x60]
        call   sub_1_00D0
        pop    edx
        jb     loc_1_7558
        movzx  ecx, word ptr [edi + 0x20]
        sub    ecx, edx
        sub    dword ptr [ebp + 0x58], ecx
        jae    loc_1_7580
        add    ecx, dword ptr [ebp + 0x58]
        mov    dword ptr [ebp + 0x58], 0
loc_1_7580:
        VxDCall  Dev_048B.Service_0007h          ; cd2007008b04
        push   esi
        push   edi
        lea    esi, [edx + ebx]
        mov    edi, dword ptr [ebp + 0x14]
        call   sub_1_823A
        mov    dword ptr [ebp + 0x14], edi
        pop    edi
        pop    esi
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
        jb     loc_1_7558
        sub    edx, edx
        cmp    dword ptr [ebp + 0x58], edx
        jne    loc_1_755C
        movzx  eax, byte ptr [edi + 0x4d]
        add    dword ptr [ebp + 0x64], eax
        jmp    loc_1_7551
loc_1_75B0:
        cmp    dword ptr [ebp + 0x5c], 0
        je     loc_1_751B
        mov    ebx, dword ptr [ebp + 0x64]
        call   sub_1_00D0
        jb     loc_1_7558
        VxDCall  Dev_048B.Service_0007h          ; cd2007008b04
        xchg   ebx, esi
        mov    ecx, dword ptr [ebp + 0x5c]
        cmp    cx, word ptr [edi + 0x20]
        jb     loc_1_75D9
        mov    cx, word ptr [edi + 0x20]
loc_1_75D9:
        sub    dword ptr [ebp + 0x5c], ecx
        push   edi
        mov    edi, dword ptr [ebp + 0x14]
        call   sub_1_823A
        mov    esi, ebx
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
        jb     loc_1_7558
        mov    dword ptr [ebp + 0x14], edi
        pop    edi
        inc    dword ptr [ebp + 0x64]
        cmp    dword ptr [ebp + 0x5c], 0
        jne    loc_1_75B0
        movzx  eax, byte ptr [edi + 0x4d]
        sub    dword ptr [ebp + 0x68], eax
        ret    
sub_1_7608:
        mov    ah, byte ptr [0xb6f8]         ; -> obj1:0xB6F8
        mov    al, 7
        call   sub_1_0064
        jae    loc_1_7620
        test   esi, esi
        je     loc_1_7626
        mov    byte ptr [esi + 0x28], 1
        ret    
loc_1_7620:
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
loc_1_7626:
        xor    esi, esi
        ret    
loc_1_7629:
        ret    
sub_1_762A:
        push   ebx
        call   sub_1_7F50
        jb     loc_1_7659
        mov    dl, byte ptr [ebp + 0x3b]
        call   sub_1_7E5E
        jb     loc_1_7659
        mov    eax, dword ptr [esp]
        and    ax, word ptr [edi + 0x22]
        jne    loc_1_7654
        test   byte ptr [ebp + 0x3a], 1
        je     loc_1_7654
        call   sub_1_02F8
        jmp    loc_1_7659
        db  08Bh, 0C0h
loc_1_7654:
        call   sub_1_00D0
loc_1_7659:
        pop    edx
        jb     loc_1_7629
        mov    ecx, dword ptr [ebp + 0x44]
        push   ebx
        xchg   esi, ebx
        movzx  eax, word ptr [edi + 0x22]
        and    edx, eax
        add    esi, edx
        add    edx, ecx
        and    edx, eax
        mov    edi, esi
        mov    esi, ebx
        VxDCall  Dev_048B.Service_0007h          ; cd2007008b04
        mov    esi, dword ptr [ebp + 0x14]
        call   sub_1_823A
        mov    dword ptr [ebp + 0x14], esi
        mov    esi, ebx
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
        pop    ebx
        mov    edi, dword ptr [ebp + 0x1c]
        jb     loc_1_76B6
        push   edx
        call   sub_1_03EC
        pop    edx
        mov    edi, dword ptr [ebp + 0x1c]
        jb     loc_1_76B6
        test   edx, edx
        jne    loc_1_76B5
        mov    ecx, dword ptr [ebp + 0x20]
        mov    al, byte ptr [ebp + 0x3b]
        inc    eax
        cmp    al, byte ptr [edi + 0x24]
        jbe    loc_1_76B2
        inc    dword ptr [ebp + 0x40]
        mov    al, 0
loc_1_76B2:
        mov    byte ptr [ebp + 0x3b], al
loc_1_76B5:
        clc    
loc_1_76B6:
        ret    
sub_1_76B7:
        mov    eax, dword ptr [esi + 8]
        mov    edx, dword ptr [eax + 0x54]
        sub    edx, 8
        mov    ebx, dword ptr [ebp + 0x40]
loc_1_76C3:
        add    edx, 8
        sub    ebx, dword ptr [edx]
        jae    loc_1_76C3
        mov    eax, ebx
        add    ebx, dword ptr [edx]
        neg    eax
        add    ebx, dword ptr [edx + 4]
        mov    edx, ecx
        mov    cl, byte ptr [edi + 0x25]
        shl    eax, cl
        mov    ecx, edx
        movzx  edx, byte ptr [ebp + 0x3b]
        sub    eax, edx
        cmp    eax, ecx
        jae    loc_1_76E8
        mov    ecx, eax
loc_1_76E8:
        push   ecx
        push   edx
        add    edx, ecx
        mov    eax, edx
        and    dl, byte ptr [edi + 0x24]
        mov    cl, byte ptr [edi + 0x25]
        shr    eax, cl
        mov    byte ptr [ebp + 0x3b], dl
        add    dword ptr [ebp + 0x40], eax
        pop    edx
        call   sub_1_7E5E
        pop    ecx
        ret    
sub_1_7704:
        movzx  eax, word ptr [ebp + 0x3c]
        movzx  edx, word ptr [edi + 0x20]
        and    ax, word ptr [edi + 0x22]
        mov    ebx, dword ptr [ebp]
        je     loc_1_7732
        sub    eax, edx
        neg    eax
        sub    ebx, eax
        jae    loc_1_7721
        add    eax, ebx
        xor    ebx, ebx
loc_1_7721:
        mov    dword ptr [ebp + 0x44], eax
        xchg   ebx, eax
        mov    ebx, edx
        xor    edx, edx
        div    ebx
        mov    dword ptr [ebp + 0x48], eax
        mov    dword ptr [ebp + 0x4c], edx
        ret    
loc_1_7732:
        test   byte ptr [ebp + 0x37], 1
        je     loc_1_7721
        cmp    dword ptr [ebp], 0x1000
        jne    loc_1_7721
        test   dword ptr [ebp + 0x3c], 0xfff
        jne    loc_1_7721
        sub    eax, eax
        jmp    loc_1_7721
sub_1_774E:
        mov    eax, dword ptr [ebp + 0x3c]
        mov    cl, byte ptr [edi + 0x4c]
        shr    eax, cl
        mov    ebx, eax
        and    bl, byte ptr [edi + 0x24]
        mov    byte ptr [ebp + 0x3b], bl
        mov    cl, byte ptr [edi + 0x25]
        shr    eax, cl
        mov    dword ptr [ebp + 0x40], eax
        mov    eax, dword ptr [ebp + 0x3c]
        add    eax, dword ptr [ebp]
        movzx  ecx, word ptr [edi + 0x4a]
        add    eax, ecx
        mov    cl, byte ptr [edi + 0x47]
        shr    eax, cl
        mov    edx, dword ptr [esi + 8]
        je     loc_1_777D
        dec    eax
loc_1_777D:
        cmp    dword ptr [edx + 0x50], eax
        ja     loc_1_7787
        call   sub_1_2165
loc_1_7787:
        ret    
sub_1_7788:
        or     eax, 0xffffffff
        cmp    dword ptr [edi + 0x9c], 0
        je     loc_1_77BD
loc_1_7794:
        mov    ecx, ebx
        mov    eax, dword ptr [edi + 0x9c]
        shr    ecx, 0xa
        movsx  eax, word ptr [eax + ecx*2]
        or     eax, eax
        jg     loc_1_77BD
        mov    eax, dword ptr [edi + 0x9c]
        jl     loc_1_77BE
loc_1_77AF:
        inc    ecx
        cmp    word ptr [eax + ecx*2], 0
        je     loc_1_77AF
        mov    ebx, ecx
        shl    ebx, 0xa
        stc    
loc_1_77BD:
        ret    
loc_1_77BE:
        push   edx
        push   ebx
        push   dword ptr [edi + 0xa8]
        xor    edx, edx
        lea    eax, [eax + ecx*2]
        mov    ebx, ecx
        shl    ebx, 0xa
        mov    ecx, dword ptr [edi + 0x2c]
        dec    ebx
        sub    ecx, ebx
        cmp    ecx, 0x400
        jb     loc_1_77E3
        mov    ecx, 0x400
loc_1_77E3:
        inc    ebx
        push   eax
        push   ebx
        push   ecx
        push   edx
        push   esi
        call   sub_1_1F2C
        pop    esi
        pop    edx
        pop    ecx
        pop    ebx
        pop    eax
        jb     loc_1_780A
        jne    loc_1_77F8
        inc    edx
loc_1_77F8:
        loop   loc_1_77E3
loc_1_77FA:
        pop    ecx
        pop    ebx
        cmp    dword ptr [edi + 0xa8], ecx
        jne    loc_1_7807
        mov    word ptr [eax], dx
loc_1_7807:
        pop    edx
        jmp    loc_1_7794
loc_1_780A:
        cmp    ebx, 2
        jb     loc_1_77F8
        mov    edx, 0x401
        jmp    loc_1_77FA
sub_1_7816:
        or     esi, esi
        je     loc_1_783C
        and    byte ptr [esi + 0x24], 0xbf
        movzx  eax, word ptr [edi + 0x4a]
        add    eax, dword ptr [esi + 0x30]
        rcr    eax, 1
        push   ecx
        mov    cl, byte ptr [edi + 0x47]
        dec    cl
        shr    eax, cl
        pop    ecx
        cmp    dword ptr [esi + 0x50], eax
        je     loc_1_783C
        or     byte ptr [esi + 0x24], 0x40
        mov    eax, dword ptr [esi + 0x50]
loc_1_783C:
        push   eax
        push   ebx
        push   ecx
        push   edi
        test   byte ptr [edi + 0x53], 0x40
        je     loc_1_7860
        cmp    dword ptr [0xb78a], 0         ; -> obj1:0xB78A
        je     loc_1_7860
        push   5
        push   dword ptr [0xb78a]            ; -> obj1:0xB78A
        VMMCall  VMM._EnterMutex                 ; cd202e010100
        add    esp, 8
loc_1_7860:
        pop    edi
        pop    ecx
        pop    ebx
        pop    edx
loc_1_7864:
        bts    dword ptr [edi + 0x94], 0
        jae    loc_1_787B
        lea    eax, [edi + 0x98]
        call   sub_1_A220
        jmp    loc_1_7864
loc_1_787B:
        sub    esp, 0x28
        mov    eax, 0x27
        cmp    dword ptr [edi + 0x2c], ecx
        jbe    loc_1_7A4E
        cmp    dword ptr [edi + 0x40], ecx
        jb     loc_1_79CB
        mov    dword ptr [esp + 4], esi
        mov    dword ptr [esp], edx
        test   byte ptr [edi + 0x52], 0x10
        je     loc_1_78C5
        test   byte ptr [ebp + 0x37], 0x10
        jne    loc_1_78BC
        call   sub_1_5B05
        cmp    eax, 1
        jae    loc_1_78C5
        mov    eax, 0x27
        jmp    loc_1_79CB
loc_1_78BC:
        push   ecx
        mov    eax, ecx
        call   sub_1_5B8D
        pop    ecx
loc_1_78C5:
        mov    dword ptr [esp + 0x20], ebx
        mov    dword ptr [esp + 0x18], ecx
        mov    dword ptr [esp + 0x10], ecx
        mov    eax, dword ptr [edi + 0xa8]
        mov    dword ptr [esp + 8], eax
        mov    dword ptr [esp + 0x14], ebx
        and    dword ptr [esp + 0x24], 0
        test   byte ptr [ebp + 0x37], 0x10
        jne    loc_1_7BBD
        mov    eax, ecx
        mov    edx, ecx
        mov    cl, byte ptr [edi + 0x47]
        shl    eax, cl
        mov    ecx, edx
        cmp    dword ptr [0xb888], eax       ; -> obj1:0xB888
        jbe    loc_1_7BBD
loc_1_7905:
        and    ebx, ebx
        je     loc_1_7A54
        call   sub_1_2102
        jae    loc_1_7A49
loc_1_7918:
        inc    ebx
        cmp    dword ptr [edi + 0x2c], ebx
        jb     loc_1_7A54
        push   ebx
        call   sub_1_1F2C
        pop    edx
        jb     loc_1_7A49
        jne    loc_1_7A54
loc_1_7935:
        cmp    dword ptr [esp + 0x24], 0
        jne    loc_1_7940
        mov    dword ptr [esp + 0x24], edx
loc_1_7940:
        mov    ebx, dword ptr [esp + 0x14]
        and    ebx, ebx
        je     loc_1_7955
        push   edx
        call   sub_1_1D4C
        pop    edx
        jb     loc_1_7A49
loc_1_7955:
        mov    ebx, edx
        dec    edx
        cmp    dword ptr [edi + 0x38], edx
        jne    loc_1_7960
        mov    dword ptr [edi + 0x38], ebx
loc_1_7960:
        mov    eax, dword ptr [edi + 0x38]
        cmp    dword ptr [edi + 0x2c], eax
        ja     loc_1_796F
        mov    dword ptr [edi + 0x38], 2
loc_1_796F:
        or     edx, 0xffffffff
        push   ebx
        call   sub_1_1D4C
        pop    ebx
        jb     loc_1_7A49
        inc    dword ptr [edi + 0xa8]
        inc    dword ptr [esp + 8]
        mov    esi, dword ptr [esp + 4]
        call   sub_1_7C8D
        mov    ecx, dword ptr [edi + 0x9c]
        jecxz  loc_1_79A9
        mov    eax, ebx
        shr    eax, 0xa
        cmp    word ptr [ecx + eax*2], -1
        sbb    word ptr [ecx + eax*2], 0
loc_1_79A9:
        test   byte ptr [ebp + 0x37], 0x10
        jne    loc_1_7A25
loc_1_79AF:
        mov    dword ptr [esp + 0x14], ebx
        dec    dword ptr [esp + 0x10]
        jne    loc_1_7918
        cmp    dword ptr [edi + 0x40], -1
        je     loc_1_79CB
        mov    eax, dword ptr [esp + 0x18]
        sub    dword ptr [edi + 0x40], eax
        clc    
loc_1_79CB:
        pushfd 
        push   eax
        mov    eax, dword ptr [edi + 0x60]
        or     eax, eax
        je     loc_1_79E1
        mov    edx, dword ptr [edi + 0x38]
        test   byte ptr [edi + 0x52], 3
        je     loc_1_79E1
        mov    word ptr [eax + 0x1d], dx
loc_1_79E1:
        and    dword ptr [edi + 0x94], 0
        cmp    dword ptr [edi + 0x98], 0
        je     loc_1_79FC
        lea    eax, [edi + 0x98]
        call   sub_1_A258
loc_1_79FC:
        test   byte ptr [edi + 0x53], 0x40
        je     loc_1_7A1A
        cmp    dword ptr [0xb78a], 0         ; -> obj1:0xB78A
        je     loc_1_7A1A
        push   dword ptr [0xb78a]            ; -> obj1:0xB78A
        VMMCall  VMM._LeaveMutex                 ; cd202f010100
        add    esp, 4
loc_1_7A1A:
        pop    eax
        popfd  
        mov    ebx, dword ptr [esp + 0x24]
        lea    esp, [esp + 0x28]
        ret    
loc_1_7A25:
        test   byte ptr [edi + 0x52], 0x10
        je     loc_1_79AF
        pushal 
        mov    edx, ebx
        mov    ebx, edi
        mov    cx, 0xdb0a
        call   sub_1_449E
        popal  
        jae    loc_1_79AF
        dec    dword ptr [esp + 0x10]
        jmp    loc_1_7B55
loc_1_7A49:
        mov    eax, 5
loc_1_7A4E:
        stc    
        jmp    loc_1_79CB
loc_1_7A54:
        mov    ebx, dword ptr [edi + 0x38]
        mov    esi, dword ptr [esp + 4]
        call   sub_1_7CFF
        jae    loc_1_7A7A
        mov    eax, 1
        mov    cl, 0x11
        sub    cl, byte ptr [edi + 0x47]
        shl    eax, cl
        add    ebx, eax
        cmp    dword ptr [edi + 0x2c], ebx
        jae    loc_1_7A7A
        mov    ebx, 2
loc_1_7A7A:
        test   byte ptr [edi + 0x52], 2
        jne    loc_1_7AB4
loc_1_7A80:
        call   sub_1_7788
        jb     loc_1_7A95
        push   ebx
        call   sub_1_1F2C
        pop    edx
        jb     loc_1_7A49
        je     loc_1_7AAC
        lea    ebx, [edx + 1]
loc_1_7A95:
        cmp    dword ptr [edi + 0x2c], ebx
        jae    loc_1_7A80
        mov    ebx, 2
        cmp    dword ptr [edi + 0x38], ebx
        mov    dword ptr [edi + 0x38], ebx
        jne    loc_1_7A80
        jmp    loc_1_7B33
loc_1_7AAC:
        mov    dword ptr [edi + 0x38], edx
        jmp    loc_1_7935
loc_1_7AB4:
        mov    eax, dword ptr [edi + 0x2c]
        movzx  ecx, word ptr [edi + 0x22]
        lea    eax, [ecx + eax*2 + 2]
        mov    cl, byte ptr [edi + 0x4c]
        shr    eax, cl
        inc    eax
        mov    dword ptr [esp + 0xc], eax
loc_1_7AC9:
        dec    dword ptr [esp + 0xc]
        js     loc_1_7B33
        mov    dword ptr [esp + 0x1c], ebx
        add    ebx, ebx
        mov    eax, ebx
        mov    cl, byte ptr [edi + 0x4c]
        shr    ebx, cl
        movzx  ecx, word ptr [edi + 0x26]
        cmp    byte ptr [edi + 0x48], 0
        je     loc_1_7AE9
        add    ecx, dword ptr [edi + 0x30]
loc_1_7AE9:
        add    ebx, ecx
        push   eax
        call   sub_1_01D0
        pop    eax
        jb     loc_1_7A49
        movzx  ecx, word ptr [edi + 0x22]
        and    eax, ecx
        lea    esi, [eax + ebx]
        inc    ecx
        sub    ecx, eax
        shr    ecx, 1
        push   edi
        mov    edi, esi
        sub    eax, eax
        mov    edx, ecx
        repne scasw ax, word ptr es:[edi]
        pop    edi
        jne    loc_1_7B26
        sub    edx, ecx
        dec    edx
        add    edx, dword ptr [esp + 0x1c]
        cmp    dword ptr [edi + 0x2c], edx
        jae    loc_1_7AAC
loc_1_7B1F:
        mov    ebx, 2
        jmp    loc_1_7AC9
loc_1_7B26:
        mov    ebx, dword ptr [esp + 0x1c]
        add    ebx, edx
        cmp    dword ptr [edi + 0x2c], ebx
        jae    loc_1_7AC9
        jmp    loc_1_7B1F
loc_1_7B33:
        mov    eax, dword ptr [esp + 8]
        cmp    dword ptr [edi + 0xa8], eax
        jne    loc_1_7B55
        cmp    dword ptr [edi + 0x40], -1
        jne    loc_1_7B55
        call   sub_1_5BD3
        mov    ebx, dword ptr [esp + 0x18]
        sub    ebx, dword ptr [esp + 0x10]
        mov    dword ptr [edi + 0x40], ebx
loc_1_7B55:
        mov    ebx, dword ptr [esp + 0x18]
        mov    ecx, dword ptr [esp + 0x20]
        mov    edx, dword ptr [esp + 0x24]
        sub    ebx, dword ptr [esp + 0x10]
        je     loc_1_7B9A
        mov    esi, dword ptr [esp + 4]
        mov    eax, dword ptr [esp]
        call   sub_1_7CC8
        add    dword ptr [esp + 8], ebx
        push   dword ptr [edi + 0x40]
        or     dword ptr [edi + 0x40], 0xffffffff
        jecxz  loc_1_7B8E
        mov    ebx, ecx
        mov    edx, 0xffffffff
        call   sub_1_7D52
        jmp    loc_1_7B95
loc_1_7B8E:
        mov    ebx, edx
        call   sub_1_7D50
loc_1_7B95:
        pop    dword ptr [edi + 0x40]
        jb     loc_1_7BB3
loc_1_7B9A:
        mov    eax, dword ptr [edi + 0xa8]
        mov    ebx, dword ptr [esp + 0x20]
        mov    ecx, dword ptr [esp + 0x18]
        cmp    dword ptr [esp + 8], eax
        jne    loc_1_78C5
        stc    
loc_1_7BB3:
        mov    eax, 0x27
        jmp    loc_1_79CB
loc_1_7BBD:
        or     ebx, ebx
        jne    loc_1_7BC4
        mov    ebx, dword ptr [edi + 0x38]
loc_1_7BC4:
        inc    ebx
        call   sub_1_7BE5
        mov    edx, ebx
        jae    loc_1_7AAC
        cmp    byte ptr [ebp + 0x34], 0x10
        jae    loc_1_7A49
        mov    ebx, dword ptr [esp + 0x20]
        jmp    loc_1_7905
sub_1_7BE5:
        sub    esp, 0x14
        mov    dword ptr [esp + 0x10], ecx
        mov    eax, dword ptr [edi + 0x2c]
        mov    dword ptr [esp + 4], eax
loc_1_7BF3:
        lea    esi, [ebx - 1]
        lea    ebx, [ecx + esi]
        cmp    eax, ebx
        jae    loc_1_7C04
        mov    ebx, 2
        jmp    loc_1_7BF3
loc_1_7C04:
        lea    eax, [ebx - 1]
        mov    dword ptr [esp], eax
        mov    dword ptr [esp + 0xc], ebx
        and    dword ptr [esp + 8], 0
        jmp    loc_1_7C38
loc_1_7C15:
        cmp    eax, 0x400
        je     loc_1_7C79
loc_1_7C1C:
        push   ebx
        push   esi
        call   sub_1_1F2C
        pop    esi
        pop    ebx
        jb     loc_1_7C88
        jne    loc_1_7C40
loc_1_7C29:
        mov    ecx, ebx
        dec    ebx
        cmp    ebx, esi
        jbe    loc_1_7C81
        test   ecx, 0x3ff
        jne    loc_1_7C1C
loc_1_7C38:
        call   sub_1_7788
        jae    loc_1_7C15
        dec    ebx
loc_1_7C40:
        mov    eax, dword ptr [esp + 0xc]
        sub    eax, ebx
        cmc    
        sbb    edx, edx
        and    eax, edx
        mov    dword ptr [esp + 8], eax
        lea    esi, [ebx + eax]
        add    ebx, dword ptr [esp + 0x10]
        mov    dword ptr [esp + 0xc], ebx
        cmp    dword ptr [esp + 4], ebx
        jae    loc_1_7C38
        xor    eax, eax
        xchg   dword ptr [esp], eax
        mov    dword ptr [esp + 4], eax
        mov    ebx, 1
        mov    dword ptr [esp + 0xc], ebx
        or     eax, eax
        jne    loc_1_7C40
        stc    
        jmp    loc_1_7C88
loc_1_7C79:
        and    ebx, 0xfffffc00
        jmp    loc_1_7C29
loc_1_7C81:
        lea    ebx, [esi + 1]
        sub    ebx, dword ptr [esp + 8]
loc_1_7C88:
        lea    esp, [esp + 0x14]
        ret    
sub_1_7C8D:
        or     esi, esi
        je     loc_1_7CAE
        test   byte ptr [esi + 0x24], 0x40
        jne    loc_1_7CAE
        mov    edx, dword ptr [esi + 0x5c]
        cmp    dword ptr [esi + 0x50], 0
        je     loc_1_7CAF
        mov    ecx, dword ptr [edx]
        add    ecx, dword ptr [edx + 4]
        cmp    ecx, ebx
        jne    loc_1_7CAF
loc_1_7CA9:
        inc    dword ptr [edx]
        inc    dword ptr [esi + 0x50]
loc_1_7CAE:
        ret    
loc_1_7CAF:
        add    edx, 8
        cmp    dword ptr [esi + 0x58], edx
        jbe    loc_1_7CC2
        mov    dword ptr [esi + 0x5c], edx
        mov    dword ptr [edx + 4], ebx
        and    dword ptr [edx], 0
        jmp    loc_1_7CA9
loc_1_7CC2:
        or     byte ptr [esi + 0x24], 0x40
        jmp    loc_1_7CAE
sub_1_7CC8:
        or     esi, esi
        je     loc_1_7CF0
        cmp    dword ptr [esi + 0x50], eax
        je     loc_1_7CF0
        or     eax, eax
        je     loc_1_7CF1
        push   ebx
        push   ecx
        push   eax
        mov    ecx, esi
        lea    ebx, [eax - 1]
        call   sub_1_2276
        mov    dword ptr [esi + 0x5c], eax
        sub    ebx, dword ptr [eax + 4]
        inc    ebx
        mov    dword ptr [eax], ebx
        pop    dword ptr [esi + 0x50]
        pop    ecx
        pop    ebx
loc_1_7CF0:
        ret    
loc_1_7CF1:
        mov    dword ptr [esi + 0x50], eax
        mov    eax, dword ptr [esi + 0x54]
        sub    eax, 8
        mov    dword ptr [esi + 0x5c], eax
        jmp    loc_1_7CF0
sub_1_7CFF:
        mov    edx, 0xb720                   ; -> obj1:0xB720
loc_1_7D04:
        mov    edx, dword ptr [edx + 8]
        cmp    edx, 0xb720                   ; -> obj1:0xB720
        je     loc_1_7D4F
        cmp    word ptr [edx + 0x2e], 0
        je     loc_1_7D04
        cmp    edx, esi
        je     loc_1_7D04
        cmp    dword ptr [edx + 0x28], edi
        jne    loc_1_7D04
        test   byte ptr [edx + 0x23], 0x81
        jne    loc_1_7D04
        cmp    dword ptr [edx + 0x50], 0
        je     loc_1_7D04
        bt     dword ptr [edx + 0x24], 4
        jb     loc_1_7D04
        inc    word ptr [edx + 0x20]
        mov    ecx, dword ptr [edx + 0x5c]
        mov    eax, dword ptr [ecx + 4]
        add    eax, dword ptr [ecx]
        push   eax
        push   edi
        mov    edi, edx
        call   sub_1_A040
        pop    edi
        pop    eax
        dec    eax
        cmp    eax, ebx
        jne    loc_1_7D04
        stc    
loc_1_7D4F:
        ret    
sub_1_7D50:
        xor    edx, edx
sub_1_7D52:
        cmp    ebx, 2
        jb     loc_1_7E5A
        cmp    dword ptr [edi + 0x2c], ebx
        jb     loc_1_7E5A
        or     edx, edx
        jne    loc_1_7E03
        push   ebx
        call   sub_1_7E5E
        jb     loc_1_7E00
        mov    esi, ebx
        sub    ebx, dword ptr [edi + 0xc]
        mov    al, byte ptr [edi + 0x4e]
        not    al
        and    bl, al
        add    ebx, dword ptr [edi + 0xc]
        mov    cl, byte ptr [edi + 0x24]
        mov    dl, 2
        shl    dl, cl
        dec    dl
        mov    ecx, esi
        sub    ecx, ebx
        shl    dl, cl
        movzx  ecx, word ptr [edi + 0x4a]
        add    ecx, 0x1000
        shr    ecx, 0xc
loc_1_7DA3:
        call   sub_1_00B0
        jb     loc_1_7DF8
        cli    
        VxDCall  Dev_048B.Service_000Dh          ; cd200d008b04
        or     eax, eax
        jne    loc_1_7DF7
        mov    al, byte ptr [esi + 0x2b]
        test   al, 0x10
        jne    loc_1_7DF7
        push   ecx
        push   edx
        not    dl
        and    byte ptr [esi + 0x2a], dl
        and    byte ptr [esi + 0x32], dl
        jne    loc_1_7DDD
        test   al, 8
        je     loc_1_7DDD
        push   ebx
        mov    ebx, esi
        cli    
        call   sub_1_893E
        mov    esi, ebx
        pop    ebx
        call   sub_1_899F
loc_1_7DDD:
        mov    edx, dword ptr [esp]
        or     dl, byte ptr [esi + 0x24]
        mov    byte ptr [esi + 0x24], dl
        inc    dl
        jne    loc_1_7DF5
        call   0
        VxDCall  Dev_048B.Service_0005h          ; cd2005008b04
loc_1_7DF5:
        pop    edx
        pop    ecx
loc_1_7DF7:
        sti    
loc_1_7DF8:
        movzx  eax, byte ptr [edi + 0x4d]
        add    ebx, eax
        loop   loc_1_7DA3
loc_1_7E00:
        pop    ebx
        sub    edx, edx
loc_1_7E03:
        push   ebx
        push   edx
        call   sub_1_1F2C
        mov    eax, ebx
        pop    edx
        pop    ebx
        jb     loc_1_7E5C
        push   eax
        push   edx
        push   ebx
        call   sub_1_1D4C
        pop    eax
        pop    edx
        pop    ebx
        jb     loc_1_7E5C
        and    ebx, ebx
        je     loc_1_7E5A
        or     edx, edx
        jne    loc_1_7E4F
        inc    dword ptr [edi + 0xa8]
        cmp    dword ptr [edi + 0x9c], 0
        je     loc_1_7E47
        shr    eax, 0xa
        add    eax, eax
        add    eax, dword ptr [edi + 0x9c]
        cmp    word ptr [eax], -1
        adc    word ptr [eax], 0
loc_1_7E47:
        cmp    dword ptr [edi + 0x40], -1
        adc    dword ptr [edi + 0x40], 0
loc_1_7E4F:
        call   sub_1_2102
        jb     sub_1_7D50
loc_1_7E5A:
        clc    
        ret    
loc_1_7E5C:
        stc    
        ret    
sub_1_7E5E:
        cmp    dword ptr [edi + 0x2c], ebx
        jb     loc_1_7E77
        sub    ebx, 2
        jb     loc_1_7E77
        mov    cl, byte ptr [edi + 0x25]
        mov    al, dl
        shl    ebx, cl
        or     bl, al
        mov    ecx, dword ptr [edi + 0x28]
        add    ebx, ecx
        ret    
loc_1_7E77:
        stc    
        ret    
sub_1_7E79:
        mov    edi, dword ptr [ebp + 0x20]
        cmp    dword ptr [edi + 8], 0
        jne    loc_1_7E9F
        push   ebx
        sub    eax, eax
        call   sub_1_7FED
        pop    ebx
        jb     loc_1_7EE9
        mov    edi, dword ptr [ebp + 0x20]
        mov    dword ptr [edi + 8], eax
        mov    esi, dword ptr [eax]
        mov    dword ptr [edi], esi
        mov    dword ptr [edi + 4], eax
        mov    dword ptr [esi + 4], edi
        mov    dword ptr [eax], edi
loc_1_7E9F:
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [edi + 0x14], eax
        movzx  eax, byte ptr [ebp + 5]
        mov    dword ptr [edi + 0x10], eax
        mov    word ptr [edi + 0xe], 0x24c
        mov    dword ptr [edi + 0x20], 1
        sub    eax, eax
        mov    dword ptr [edi + 0x18], eax
        mov    dword ptr [edi + 0x24], eax
        mov    dword ptr [edi + 0x28], eax
        mov    eax, dword ptr [ebp + 0x1c]
        test   byte ptr [eax + 0x53], 1
        je     loc_1_7ED2
        or     word ptr [edi + 0xe], 0x20
loc_1_7ED2:
        test   byte ptr [eax + 0x53], 4
        je     loc_1_7EDD
        or     word ptr [edi + 0xe], 2
loc_1_7EDD:
        xor    eax, eax
        mov    dword ptr [ebp + 0x3c], eax
        mov    dword ptr [ebp + 0x40], eax
        mov    byte ptr [ebp + 0x3b], al
        clc    
loc_1_7EE9:
        ret    
sub_1_7EEA:
        push   ecx
        push   esi
        push   edi
        mov    edi, edx
        call   sub_1_9FEC
        or     byte ptr [edi + 0x23], 1
        mov    ecx, dword ptr [edi + 0x58]
        sub    ecx, dword ptr [edi + 0x54]
        cmp    ecx, 0x800
        jb     loc_1_7F34
        mov    ecx, dword ptr [edi + 0x5c]
        add    ecx, 8
        sub    ecx, dword ptr [edi + 0x54]
        push   ecx
        mov    eax, ecx
        call   sub_1_A1CE
        pop    ecx
        jb     loc_1_7F38
        shr    ecx, 2
        mov    edi, eax
        xchg   dword ptr [edx + 0x54], eax
        mov    esi, eax
        rep movsd dword ptr es:[edi], dword ptr [esi]
        call   sub_1_A1F8
        mov    dword ptr [edx + 0x58], edi
        sub    edi, 8
        mov    dword ptr [edx + 0x5c], edi
loc_1_7F34:
        or     byte ptr [edx + 0x24], 2
loc_1_7F38:
        pushfd 
        mov    edi, edx
        call   sub_1_A01A
        popfd  
        pop    edi
        pop    esi
        pop    ecx
        ret    
loc_1_7F45:
        push   eax
        mov    eax, edx
        call   sub_1_2165
        pop    eax
        jb     loc_1_7F64
sub_1_7F50:
        mov    edx, dword ptr [ebp + 0x40]
        mov    ecx, dword ptr [esi + 8]
        cmp    edx, dword ptr [ecx + 0x50]
        jae    loc_1_7F45
        mov    ebx, edx
        push   eax
        call   sub_1_2276
        pop    eax
loc_1_7F64:
        ret    
loc_1_7F65:
        mov    ecx, dword ptr [esp + 8]
        mov    edx, dword ptr [ecx + 8]
        cmp    edx, dword ptr [ecx + 4]
        jne    loc_1_7F79
        mov    eax, dword ptr [edx]
        mov    dword ptr [ecx + 8], eax
        xor    eax, eax
        ret    
loc_1_7F79:
        mov    eax, dword ptr [edx]
        mov    dword ptr [ecx + 8], eax
        mov    ecx, dword ptr [esp + 4]
        mov    al, byte ptr [edx + 0xc]
        mov    ah, al
        and    ax, 0x807f
        mov    word ptr [ecx + 0xc], ax
        movzx  eax, word ptr [edx + 0x16]
        mov    dword ptr [ecx], eax
        mov    eax, 1
        ret    
sub_1_7F9B:
        mov    ecx, ebx
        sub    ecx, dword ptr [esi + 0x10]
        shr    ecx, 5
        mov    dl, cl
        mov    edi, dword ptr [esi + 0xc]
        mov    eax, ecx
        mov    cl, byte ptr [edi + 0x4c]
        sub    cl, 5
        shr    eax, cl
        mov    ecx, eax
        add    ecx, dword ptr [esi + 8]
        mov    eax, 0xb720                   ; -> obj1:0xB720
loc_1_7FBC:
        mov    eax, dword ptr [eax + 8]
        cmp    eax, 0xb720                   ; -> obj1:0xB720
        je     loc_1_7FEB
        cmp    dl, byte ptr [eax + 0x25]
        jne    loc_1_7FBC
        cmp    ecx, dword ptr [eax + 0x1c]
        jne    loc_1_7FBC
        cmp    edi, dword ptr [eax + 0x28]
        jne    loc_1_7FBC
        push   ecx
        push   edi
        push   esi
        lea    esi, [ebx]
        lea    edi, [eax + 0x45]
        mov    ecx, 0xb
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        pop    esi
        pop    edi
        pop    ecx
        jne    loc_1_7FBC
        clc    
        ret    
loc_1_7FEB:
        stc    
        ret    
sub_1_7FED:
        push   eax
loc_1_7FEE:
        mov    esi, dword ptr [ebp + 0x64]
        VxDCall  Dev_048B.Service_0007h          ; cd2007008b04
        call   sub_1_7F9B
        jb     loc_1_80C9
        xchg   edi, eax
        mov    ecx, dword ptr [esp]
        test   ecx, 1
        jne    loc_1_8052
        and    ecx, 2
        sub    esp, 0x10
        mov    ebx, esp
        mov    al, byte ptr [ebp + 0x62]
        mov    ah, al
        and    ax, 0x807f
        mov    word ptr [ebx + 0xc], ax
        movzx  ax, byte ptr [edi + 0x43]
        mov    word ptr [ebx + 0xe], ax
        movzx  eax, word ptr [ebp + 0xa]
        mov    dword ptr [ebx], eax
        mov    dword ptr [ebx + 4], edi
        mov    eax, dword ptr [edi]
        mov    dword ptr [ebx + 8], eax
        push   ebx
        push   0x7f65                        ; -> loc_1_7F65
        push   ebx
        push   ecx
        VxDCall  IFSMgr.IFSMgr_CheckAccessConflict ; cd2052004000
        add    esp, 0x10
        add    esp, 0x10
        or     eax, eax
        jne    loc_1_8082
loc_1_8052:
        mov    bl, byte ptr [ebp + 0x62]
        and    ebx, 7
        mov    ecx, dword ptr [ebx*4 + 0xb7d0] ; -> obj1:0xB7D0
        add    dword ptr [edi + 0x2c], ecx
loc_1_8062:
        test   byte ptr [edi + 0x23], 0x81
        jne    loc_1_807B
        inc    dword ptr [edi + 0x20]
        mov    esi, dword ptr [ebp + 0x64]
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
        mov    eax, edi
        add    esp, 4
        clc    
        ret    
loc_1_807B:
        call   sub_1_A02F
        jmp    loc_1_8062
loc_1_8082:
        mov    esi, dword ptr [ebp + 0x64]
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
        add    esp, 4
        stc    
        ret    
loc_1_8090:
        mov    eax, 0xb7f4                   ; -> obj1:0xB7F4
        call   sub_1_A220
        mov    esi, dword ptr [ebp + 0x64]
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
        jmp    loc_1_7FEE
loc_1_80A8:
        mov    byte ptr [0xb7f0], 0          ; -> obj1:0xB7F0
        mov    eax, 0xb7f4                   ; -> obj1:0xB7F4
        call   sub_1_A258
        mov    esi, dword ptr [ebp + 0x64]
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
        mov    al, 8
        add    esp, 4
        stc    
        ret    
loc_1_80C9:
        bts    dword ptr [0xb7f0], 0         ; -> obj1:0xB7F0
        jb     loc_1_8090
        mov    eax, 0x68
        push   ecx
        call   sub_1_A1CE
        pop    ecx
        jb     loc_1_80A8
        push   edx
        mov    edx, dword ptr [ebp + 0x50]
        mov    dword ptr [eax + 0x18], edx
        mov    edx, dword ptr [ebp + 0x4c]
        mov    dword ptr [eax + 0x14], edx
        pop    edx
        mov    byte ptr [eax + 0x25], dl
        mov    dword ptr [eax + 0x1c], ecx
        mov    ecx, 0xb720                   ; -> obj1:0xB720
        mov    edi, dword ptr [ecx + 8]
        mov    dword ptr [eax + 8], edi
        mov    dword ptr [eax + 0xc], ecx
        mov    dword ptr [edi + 0xc], eax
        mov    dword ptr [ecx + 8], eax
        xchg   edi, eax
        sub    eax, eax
        mov    dword ptr [edi + 0x20], eax
        mov    dword ptr [edi + 0x60], eax
        mov    byte ptr [edi + 0x23], 1
        mov    byte ptr [0xb7f0], al         ; -> obj1:0xB7F0
        mov    eax, 0xb7f4                   ; -> obj1:0xB7F4
        cmp    dword ptr [eax], 0
        je     loc_1_812A
        call   sub_1_A258
loc_1_812A:
        xor    ecx, ecx
        mov    dword ptr [edi + 0x64], ecx
        mov    dword ptr [edi], edi
        mov    dword ptr [edi + 4], edi
        mov    al, byte ptr [ebx + 0xb]
        mov    byte ptr [edi + 0x43], al
        mov    byte ptr [edi + 0x24], cl
        mov    ecx, dword ptr [ebp + 0x1c]
        mov    dword ptr [edi + 0x28], ecx
        xor    eax, eax
        test   byte ptr [ecx + 0x52], 3
        jne    loc_1_8152
        mov    eax, dword ptr [ebx + 0x12]
        or     byte ptr [edi + 0x24], 8
loc_1_8152:
        mov    ax, word ptr [ebx + 0x1a]
        mov    dword ptr [edi + 0x10], eax
        mov    al, byte ptr [ebx + 0xd]
        mov    byte ptr [edi + 0x42], al
        mov    eax, dword ptr [ebx + 0xe]
        mov    dword ptr [edi + 0x3e], eax
        mov    ax, word ptr [ebx + 0x12]
        mov    word ptr [edi + 0x3c], ax
        mov    eax, dword ptr [ebx + 0x16]
        mov    dword ptr [edi + 0x38], eax
        mov    eax, dword ptr [ebx + 0x1c]
        mov    dword ptr [edi + 0x30], eax
        mov    esi, ebx
        add    edi, 0x45
        mov    ecx, 0xb
        rep movsb byte ptr es:[edi], byte ptr [esi]
        sub    edi, 0x50
        mov    esi, dword ptr [edi + 0x28]
        test   byte ptr [ebp + 0x37], 0x10
        je     loc_1_81D6
        mov    ecx, dword ptr [esi + 0x2c]
        lea    ecx, [ecx*8 + 0xff7]
        shr    ecx, 0xc
        push   8
        push   ecx
        push   0x80080000
        VMMCall  VMM._PageReserve                ; cd201d010100
        add    esp, 0xc
        or     eax, eax
        je     loc_1_821F
        push   8
        push   1
        push   eax
        VMMCall  VMM._PageReAllocate             ; cd2054000100
        add    esp, 0xc
        or     eax, eax
        je     loc_1_821F
        or     byte ptr [edi + 0x24], 1
        mov    dword ptr [edi + 0x54], eax
        add    eax, 0x1000
        mov    dword ptr [edi + 0x58], eax
        jmp    loc_1_81EB
loc_1_81D6:
        mov    eax, 0x40
        call   sub_1_A1CE
        jb     loc_1_821F
        mov    dword ptr [edi + 0x54], eax
        add    eax, 0x40
        mov    dword ptr [edi + 0x58], eax
loc_1_81EB:
        and    dword ptr [edi + 0x50], 0
        mov    eax, dword ptr [edi + 0x54]
        sub    eax, 8
        mov    dword ptr [edi + 0x5c], eax
        mov    bl, byte ptr [ebp + 0x62]
        and    ebx, 7
        mov    ecx, dword ptr [ebx*4 + 0xb7d0] ; -> obj1:0xB7D0
        mov    dword ptr [edi + 0x2c], ecx
        call   sub_1_A01A
        xchg   edi, eax
        inc    dword ptr [eax + 0x20]
        mov    esi, dword ptr [ebp + 0x64]
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
        add    esp, 4
        clc    
        ret    
loc_1_821F:
        mov    eax, dword ptr [edi + 8]
        mov    ebx, dword ptr [edi + 0xc]
        mov    dword ptr [eax + 0xc], ebx
        mov    dword ptr [ebx + 8], eax
        xchg   edi, eax
        call   sub_1_A1F8
        xchg   edi, eax
        jmp    loc_1_80A8
        db  090h
loc_1_8238:
        movsb  byte ptr es:[edi], byte ptr [esi]
        dec    ecx
sub_1_823A:
        jecxz  loc_1_8253
        test   esi, 3
        jne    loc_1_8238
        shrd   eax, ecx, 2
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        shld   ecx, eax, 2
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_1_8253:
        clc    
        ret    
loc_1_8255:
        mov    byte ptr [ebp + 0x34], 0xf
        stc    
        ret    
sub_1_825B:
        mov    eax, 0xb720                   ; -> obj1:0xB720
loc_1_8260:
        mov    eax, dword ptr [eax + 8]
        cmp    eax, 0xb720                   ; -> obj1:0xB720
        je     loc_1_8275
        cmp    ebx, dword ptr [eax + 0x28]
        jne    loc_1_8260
        or     byte ptr [eax + 0x44], 1
        jmp    loc_1_8260
loc_1_8275:
        ret    
        db  000h, 000h
loc_1_8278:
        ret    
sub_1_8279:
        mov    eax, 0xb6fc                   ; -> obj1:0xB6FC
        cmp    dword ptr [eax + 0x1c], eax
        je     loc_1_8278
        mov    edi, 0xb748                   ; -> loc_1_B748
loc_1_8288:
        mov    edi, dword ptr [edi]
        cmp    edi, 0xb748                   ; -> loc_1_B748
        je     loc_1_829E
        mov    dword ptr [edi + 0x84], 0
        jmp    loc_1_8288
loc_1_829E:
        mov    bh, 1
        mov    word ptr [0xb834], bx         ; -> obj1:0xB834
        call   sub_1_89CF
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf000100
        cld    
        mov    edx, eax
        mov    cl, byte ptr [0xb81d]         ; -> loc_1_B81D
        mov    esi, 0xb6fc                   ; -> obj1:0xB6FC
        mov    ebx, 0x1000000
        mov    eax, eax
loc_1_82C8:
        mov    esi, dword ptr [esi + 0x20]
loc_1_82CB:
        cmp    esi, 0xb6fc                   ; -> obj1:0xB6FC
        je     loc_1_838E
        mov    edi, dword ptr [esi + 0xc]
        btr    dword ptr [esi + 0x2b], 2
        jb     loc_1_82F4
        mov    eax, dword ptr [edi + 0x74]
        shr    eax, cl
        add    eax, dword ptr [esi + 0x18]
        sub    eax, edx
        jbe    loc_1_82F4
        cmp    eax, ebx
        jae    loc_1_82C8
        xchg   ebx, eax
        jmp    loc_1_82C8
loc_1_82F4:
        push   ebx
        push   ecx
        push   edx
        push   dword ptr [esi + 0x20]
        mov    eax, dword ptr [esi + 0x1c]
        mov    ebx, dword ptr [esi + 0x20]
        mov    dword ptr [eax + 0x20], ebx
        mov    dword ptr [ebx + 0x1c], eax
        dec    dword ptr [0xb750]            ; -> loc_1_B750
        add    byte ptr [esi + 0x2b], 8
        mov    eax, dword ptr [esi + 8]
        mov    ecx, dword ptr [edi + 0x84]
        jecxz  loc_1_8330
        cmp    eax, dword ptr [ecx + 8]
        jbe    loc_1_8330
        mov    ebx, dword ptr [ecx + 0x1c]
        cmp    ebx, dword ptr [edi + 0x80]
        je     loc_1_8370
        cmp    eax, dword ptr [ebx + 8]
        jb     loc_1_8370
loc_1_8330:
        mov    ecx, dword ptr [edi + 0x80]
        jecxz  loc_1_835F
loc_1_8338:
        mov    ecx, dword ptr [ecx + 0x20]
        cmp    eax, dword ptr [ecx + 8]
        ja     loc_1_8370
        cmp    ecx, dword ptr [edi + 0x80]
        jne    loc_1_8338
        mov    eax, dword ptr [ecx + 0x20]
        mov    dword ptr [eax + 0x1c], esi
        mov    dword ptr [esi + 0x1c], ecx
        mov    dword ptr [esi + 0x20], eax
        mov    dword ptr [ecx + 0x20], esi
        mov    dword ptr [edi + 0x80], esi
        jmp    loc_1_837F
loc_1_835F:
        mov    dword ptr [edi + 0x80], esi
        mov    dword ptr [esi + 0x1c], esi
        mov    dword ptr [esi + 0x20], esi
        jmp    loc_1_837F
        db  02Eh, 08Bh, 0C0h
loc_1_8370:
        mov    eax, dword ptr [ecx + 0x1c]
        mov    dword ptr [esi + 0x1c], eax
        mov    dword ptr [esi + 0x20], ecx
        mov    dword ptr [eax + 0x20], esi
        mov    dword ptr [ecx + 0x1c], esi
loc_1_837F:
        mov    dword ptr [edi + 0x84], esi
        pop    esi
        pop    edx
        pop    ecx
        pop    ebx
        jmp    loc_1_82CB
loc_1_838E:
        mov    byte ptr [0xb835], 0          ; -> loc_1_B835
        push   ebx
        mov    edi, 0xb748                   ; -> loc_1_B748
loc_1_839B:
        mov    edi, dword ptr [edi]
        cmp    edi, 0xb748                   ; -> loc_1_B748
        je     loc_1_83C6
        sub    eax, eax
        cmp    dword ptr [edi + 0x80], eax
        je     loc_1_839B
        push   dword ptr [edi + 0x78]
        call   sub_1_83D0
        pop    eax
        cmp    byte ptr [0xb834], 0          ; -> obj1:0xB834
        je     loc_1_839B
        mov    dword ptr [edi + 0x78], eax
        jmp    loc_1_839B
loc_1_83C6:
        pop    ebx
loc_1_83C7:
        ret    
        db  001h, 002h, 004h, 008h, 010h, 020h, 040h, 080h
sub_1_83D0:
        call   sub_1_83F7
        jb     loc_1_83D8
        ret    
loc_1_83D8:
        mov    esi, 0x892b                   ; -> loc_1_892B
        mov    eax, 0x64
        mov    edx, edi
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        cld    
        ret    
loc_1_83EC:
        stc    
        ret    
loc_1_83EE:
        btr    dword ptr [0xb83c], 0         ; -> obj1:0xB83C
        ret    
sub_1_83F7:
        bts    dword ptr [0xb83c], 0         ; -> obj1:0xB83C
        jb     loc_1_83EC
        push   eax
        mov    ah, 1
        call   sub_1_A3AF
        pop    eax
        jb     loc_1_83EE
        mov    dword ptr [0xb840], ebx       ; -> obj1:0xB840
        mov    byte ptr [edi + 0xbb], al
        cmp    dword ptr [edi + 0x68], 8
        jae    loc_1_8624
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf000100
        cld    
        mov    dword ptr [0xb838], eax       ; -> obj1:0xB838
loc_1_842E:
        mov    esi, dword ptr [edi + 0x80]
        and    esi, esi
        je     loc_1_8624
        cmp    byte ptr [esi + 0x32], 0
        jne    loc_1_844E
        mov    ebx, esi
        pushfd 
        cli    
        call   sub_1_893E
        popfd  
        jmp    loc_1_842E
loc_1_844E:
        test   byte ptr [esi + 0x32], 1
        je     loc_1_8487
loc_1_8454:
        mov    ecx, esi
        mov    ebx, dword ptr [esi + 8]
        movzx  eax, byte ptr [edi + 0x4d]
        sub    ebx, eax
        call   sub_1_00B0
        xchg   ecx, esi
        jb     loc_1_8487
        jne    loc_1_8487
        test   byte ptr [ecx + 0x32], 0x80
        je     loc_1_8487
        cmp    dword ptr [ecx + 0x1c], 0
        je     loc_1_8487
        mov    esi, ecx
        movzx  eax, byte ptr [edi + 0x4d]
        mov    al, byte ptr [eax + 0x2e1]    ; -> obj1:0x02E1
        cmp    byte ptr [esi + 0x32], al
        je     loc_1_8454
loc_1_8487:
        bsf    ecx, dword ptr [esi + 0x32]
        mov    ax, word ptr [edi + 0x20]
        mul    cx
        movzx  ebx, ax
        mov    edx, dword ptr [esi + 8]
        add    edx, ecx
        add    ebx, dword ptr [esi + 0x10]
        sub    eax, eax
        inc    eax
        shl    eax, cl
        sub    ecx, ecx
        mov    ah, al
        mov    eax, eax
loc_1_84A8:
        inc    ecx
        or     ah, al
        shl    al, 1
        jb     loc_1_84B4
        test   byte ptr [esi + 0x32], al
        jne    loc_1_84A8
loc_1_84B4:
        VxDCall  Dev_048B.Service_0007h          ; cd2007008b04
        push   eax
        push   esi
        push   ebx
        mov    ah, 1
        call   sub_1_A3AF
        jb     loc_1_864D
        mov    word ptr [ebx + 4], 1
        add    edx, dword ptr [edi + 0x10]
        mov    dword ptr [ebx + 0x10], edx
        mov    edx, ebx
        mov    esi, dword ptr [esp + 4]
        mov    dword ptr [edx + 0x2c], esi
        mov    dword ptr [edx + 0xc], 0x8769 ; -> loc_1_8769
        or     dword ptr [edx + 8], 2
        mov    al, byte ptr [edi + 0xbb]
        xor    byte ptr [edx + 8], al
        bt     dword ptr [esi + 0x2b], 0
        sbb    eax, eax
        and    eax, 0x1000000
        or     dword ptr [edx + 8], eax
        lea    esi, [edx + 0x50]
        mov    dword ptr [edx + 0x1c], esi
loc_1_8508:
        mov    dword ptr [esi], ecx
        add    dword ptr [edx + 0x18], ecx
        pop    dword ptr [esi + 4]
        add    esi, 8
        pop    ebx
        pop    eax
        inc    byte ptr [edx + 0x4c]
        test   byte ptr [edx + 8], 1
        je     loc_1_8527
        mov    ecx, dword ptr [0xb838]       ; -> obj1:0xB838
        mov    dword ptr [edi + 0x78], ecx
loc_1_8527:
        not    ah
        and    byte ptr [ebx + 0x2a], ah
        and    byte ptr [ebx + 0x32], ah
        je     loc_1_8658
loc_1_8535:
        mov    dword ptr [esi], 0
        test   byte ptr [edi + 0x52], 4
        jne    loc_1_85E7
        cmp    byte ptr [edi + 0x46], 1
        je     loc_1_85E7
        mov    eax, dword ptr [edi + 0x18]
        cmp    byte ptr [edi + 0x48], 0
        je     loc_1_855B
        add    eax, dword ptr [edi + 0x30]
loc_1_855B:
        mov    ebx, dword ptr [edx + 0x10]
        cmp    ebx, eax
        jae    loc_1_85E7
        mov    ecx, dword ptr [edx + 0x18]
        add    ecx, ebx
        movzx  eax, word ptr [edi + 0x26]
        cmp    ecx, eax
        jbe    loc_1_85E7
        push   edx
        push   edi
        mov    esi, edx
        sub    edi, edi
        xchg   dword ptr [0xb840], edi       ; -> obj1:0xB840
        mov    eax, dword ptr [edi + 0x48]
        push   dword ptr [edi + 0x24]
        mov    ecx, 0x26
        mov    edx, edi
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    dword ptr [edx + 0x24]
        pop    edi
        mov    dword ptr [edx + 0x48], eax
        lea    eax, [edx + 0x50]
        mov    dword ptr [edx + 0x1c], eax
        mov    eax, dword ptr [edi + 0x30]
        cmp    byte ptr [edi + 0x48], 0
        je     loc_1_85A6
        neg    eax
loc_1_85A6:
        add    dword ptr [edx + 0x10], eax
        mov    esi, dword ptr [edx + 0x2c]
        lea    eax, [edx + 0x50]
loc_1_85AF:
        VxDCall  Dev_048B.Service_0007h          ; cd2007008b04
        add    eax, 8
        cmp    dword ptr [eax], 0
        je     loc_1_85CD
        movzx  ebx, byte ptr [edi + 0x4d]
        add    ebx, dword ptr [esi + 8]
        push   eax
        call   sub_1_00B0
        pop    eax
        jmp    loc_1_85AF
loc_1_85CD:
        xchg   dword ptr [esp], edx
        inc    dword ptr [edi + 0x68]
        push   edi
        xchg   edx, eax
        call   sub_1_A558
        inc    dword ptr [0xb68c]            ; -> obj1:0xB68C
        call   sub_1_199E
        pop    edi
        pop    edx
loc_1_85E7:
        xchg   edx, eax
        call   sub_1_A558
        inc    dword ptr [0xb68c]            ; -> obj1:0xB68C
        inc    dword ptr [edi + 0x68]
        push   edi
        call   sub_1_199E
        pop    edi
        cmp    dword ptr [edi + 0x68], 8
        jae    loc_1_8624
        cmp    dword ptr [0xb840], 0         ; -> obj1:0xB840
        jne    loc_1_842E
        mov    ah, 1
        call   sub_1_A3AF
        jb     loc_1_8635
        mov    dword ptr [0xb840], ebx       ; -> obj1:0xB840
        jmp    loc_1_842E
loc_1_8624:
        xor    ebx, ebx
        xchg   dword ptr [0xb840], ebx       ; -> obj1:0xB840
        or     ebx, ebx
        je     loc_1_8635
        call   sub_1_A4AE
loc_1_8635:
        mov    byte ptr [0xb83c], 0          ; -> obj1:0xB83C
        cmp    dword ptr [edi + 0x68], 0
        jne    loc_1_864C
        cmp    dword ptr [edi + 0x80], 0
        je     loc_1_864C
        stc    
loc_1_864C:
        ret    
loc_1_864D:
        pop    eax
        pop    esi
        pop    eax
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
        jmp    loc_1_8624
loc_1_8658:
        mov    al, byte ptr [ebx + 0x2b]
        push   eax
        push   edx
        push   esi
        push   dword ptr [ebx + 8]
        pushfd 
        cli    
        call   sub_1_893E
        popfd  
        call   sub_1_899F
        pop    ebx
        pop    esi
        pop    edx
        pop    eax
        cmp    byte ptr [edx + 0x4c], 8
        je     loc_1_8535
        movzx  ecx, byte ptr [edi + 0x4d]
        test   byte ptr [ecx + 0x83c7], ah   ; -> loc_1_83C7
        jne    loc_1_8535
        mov    ecx, dword ptr [edi + 0x14]
        mov    cx, word ptr [ecx + 8]
        cmp    cl, byte ptr [edx + 0x4c]
        je     loc_1_8535
        movzx  ecx, byte ptr [edi + 0x4d]
        add    ebx, ecx
        push   eax
        push   esi
        call   sub_1_00B0
        mov    ebx, esi
        pop    esi
        pop    eax
        jb     loc_1_8535
        jne    loc_1_8535
        test   byte ptr [ebx + 0x32], 1
        je     loc_1_8535
        xor    al, byte ptr [ebx + 0x2b]
        test   al, 1
        jne    loc_1_8535
        cmp    dword ptr [ebx + 0x1c], 0
        je     loc_1_8535
        xchg   ebx, esi
        VxDCall  Dev_048B.Service_0007h          ; cd2007008b04
        xchg   ebx, esi
        sub    eax, eax
        sub    ecx, ecx
        inc    eax
loc_1_86E7:
        inc    ecx
        or     ah, al
        shl    al, 1
        jb     loc_1_86F3
        test   byte ptr [ebx + 0x32], al
        jne    loc_1_86E7
loc_1_86F3:
        push   eax
        push   ebx
        push   dword ptr [ebx + 0x10]
        jmp    loc_1_8508
sub_1_86FD:
        mov    esi, dword ptr [edx + 0x2c]
        mov    edi, dword ptr [esi + 0xc]
        mov    ebx, dword ptr [edx + 0x1c]
loc_1_8706:
        cmp    word ptr [esi + 0x26], 0
        je     loc_1_8714
        cmp    word ptr [esi + 0x26], 2
        jb     loc_1_872C
loc_1_8714:
        add    ebx, 8
        cmp    dword ptr [ebx], 0
        je     loc_1_872C
        push   ebx
        movzx  ebx, byte ptr [edi + 0x4d]
        add    ebx, dword ptr [esi + 8]
        call   sub_1_00B0
        pop    ebx
        jmp    loc_1_8706
loc_1_872C:
        ret    
loc_1_872D:
        call   sub_1_86FD
        jae    loc_1_878F
        mov    esi, dword ptr [edx + 0x2c]
        mov    ebx, dword ptr [esi + 0xc]
        test   byte ptr [ebx + 0x1f], 0x20
        je     loc_1_8757
        mov    edi, 0x22f0                   ; -> loc_1_22F0
        mov    ecx, 4
        movzx  eax, word ptr [edx + 6]
        cld    
        repne scasb al, byte ptr es:[edi]
        jne    loc_1_8757
        or     byte ptr [ebx + 0x1f], 0x40
loc_1_8757:
        push   edx
        mov    edx, ebx
        mov    esi, 0x8874                   ; -> loc_1_8874
        VMMCall  VMM.Schedule_Global_Event       ; cd200e000100
        cld    
        pop    edx
        jmp    loc_1_878F
loc_1_8769:
        cld    
        dec    dword ptr [0xb68c]            ; -> obj1:0xB68C
        mov    edx, esi
        sub    eax, eax
        mov    ebx, eax
        mov    ecx, 0x408
        mov    esi, 0x8787                   ; -> loc_1_8787
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
        ret    
loc_1_8787:
        cld    
        cmp    word ptr [edx + 6], 0x10
        jae    loc_1_872D
loc_1_878F:
        mov    esi, dword ptr [edx + 0x2c]
        mov    ebx, dword ptr [edx + 0x1c]
        mov    edi, dword ptr [esi + 0xc]
loc_1_8798:
        cmp    word ptr [edx + 6], 0x10
        jae    loc_1_87CF
loc_1_879F:
        cmp    byte ptr [esi + 0x32], 1
        sbb    eax, eax
        sub    dword ptr [0xb1d8], eax       ; -> obj1:0xB1D8
        not    eax
        and    word ptr [esi + 0x26], ax
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
        add    ebx, 8
        cmp    dword ptr [ebx], 0
        je     loc_1_87EA
        push   ebx
        movzx  ebx, byte ptr [edi + 0x4d]
        add    ebx, dword ptr [esi + 8]
        call   sub_1_00B0
        pop    ebx
        jmp    loc_1_8798
loc_1_87CF:
        movzx  eax, word ptr [edx + 6]
        mov    ecx, 0x10
loc_1_87D8:
        dec    ecx
        je     loc_1_879F
        bt     dword ptr [esi + 0x26], ecx
        jae    loc_1_87D8
        mov    byte ptr [ecx + edi + 0xbc], al
        jmp    loc_1_87D8
loc_1_87EA:
        mov    eax, 0xb1d4                   ; -> obj1:0xB1D4
        cmp    dword ptr [eax], 0
        je     loc_1_87F9
        call   sub_1_A258
loc_1_87F9:
        lea    ebx, [edx]
        mov    edx, dword ptr [edx + 8]
        call   sub_1_A4AE
        dec    dword ptr [edi + 0x68]
        test   dl, 1
        je     loc_1_8831
        call   sub_1_88F1
loc_1_8810:
        mov    eax, dword ptr [edi + 0x68]
        or     eax, dword ptr [edi + 0x80]
        jne    loc_1_8830
        call   sub_1_89A5
        lea    eax, [edi + 0x88]
        cmp    dword ptr [eax], 0
        je     loc_1_8830
        call   sub_1_A258
loc_1_8830:
        ret    
loc_1_8831:
        cmp    dword ptr [edi + 0x80], 0
        je     loc_1_8841
        call   sub_1_88F1
        jmp    loc_1_885C
loc_1_8841:
        push   edi
        call   sub_1_8AF8
        pop    edi
        jb     loc_1_8810
        cmp    dword ptr [0xb82a], 1         ; -> obj1:0xB82A
        jne    loc_1_885C
        cmp    dword ptr [0xb75c], 0         ; -> sub_1_B75C
        jne    loc_1_88CE
loc_1_885C:
        bts    dword ptr [0xb808], 0         ; -> obj1:0xB808
        jb     loc_1_8810
        mov    esi, 0x8c27                   ; -> sub_1_8C27
        VMMCall  VMM.Schedule_Global_Event       ; cd200e000100
        cld    
        jmp    loc_1_8810
loc_1_8874:
        bts    dword ptr [0xb808], 1         ; -> obj1:0xB808
        jb     loc_1_88CD
        mov    al, byte ptr [edx + 0xac]
        add    al, 0x41
        mov    byte ptr [0xb914], al         ; -> loc_1_B914
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        lea    ecx, [0xb914]                 ; -> loc_1_B914
        push   ecx
        push   0xc0002
        push   0                             ; -> loc_2_0000
        VMMCall  VMM._LocalizeStackSprintf       ; cd2059010100
        mov    ecx, eax
        movzx  edi, word ptr [0xc]           ; -> obj2:0x000C
        add    edi, dword ptr [4]            ; -> loc_2_0004
        mov    eax, 0xc0001000
        VxDCall  SHELL.SHELL_SYSMODAL_Message    ; cd2003001700
        add    esp, dword ptr [esp]
        btr    dword ptr [0xb808], 1         ; -> obj1:0xB808
loc_1_88CD:
        ret    
loc_1_88CE:
        cmp    dword ptr [0xb6b8], 0         ; -> obj1:0xB6B8
        jne    loc_1_885C
        cmp    dword ptr [0xb68c], 0         ; -> obj1:0xB68C
        jne    loc_1_885C
        pushal 
        call   sub_1_B75C
        popal  
        jmp    loc_1_885C
sub_1_88F1:
        cmp    dword ptr [edi + 0x68], 6
        jae    loc_1_8925
        sub    eax, eax
        cmp    dword ptr [edi + 0x80], eax
        je     loc_1_8925
        cmp    byte ptr [0xb835], al         ; -> loc_1_B835
        jne    loc_1_8925
        call   sub_1_83F7
        jae    loc_1_8925
        bts    dword ptr [edi + 0x1f], 0
        jb     loc_1_8925
        mov    esi, 0x8926                   ; -> loc_1_8926
        mov    edx, edi
        VMMCall  VMM.Schedule_Global_Event       ; cd200e000100
        cld    
loc_1_8925:
        ret    
loc_1_8926:
        btr    dword ptr [edx + 0x1f], 0
loc_1_892B:
        cld    
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, edx
        sub    eax, eax
        call   sub_1_83D0
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_893E:
        mov    esi, dword ptr [ebx + 0x1c]
        mov    eax, dword ptr [ebx + 0x1c]
        mov    edx, dword ptr [ebx + 0x20]
        mov    dword ptr [eax + 0x20], edx
        mov    dword ptr [edx + 0x1c], eax
        test   byte ptr [ebx + 0x2b], 8
        jne    loc_1_8977
        cmp    dword ptr [edi + 0x80], ebx
        jne    loc_1_8961
        mov    dword ptr [edi + 0x80], esi
loc_1_8961:
        cmp    ebx, esi
        jne    loc_1_896D
        sub    esi, esi
        mov    dword ptr [edi + 0x80], esi
loc_1_896D:
        sub    eax, eax
        mov    dword ptr [ebx + 0x1c], eax
        and    byte ptr [ebx + 0x2b], 0xe6
        ret    
loc_1_8977:
        dec    dword ptr [0xb750]            ; -> loc_1_B750
        jmp    loc_1_896D
sub_1_897F:
        inc    dword ptr [edi + 0x6c]
        cmp    dword ptr [edi + 0x6c], 1
        je     loc_1_8989
loc_1_8988:
        ret    
loc_1_8989:
        mov    eax, dword ptr [edi + 0x14]
        test   dword ptr [eax], 0x10000
        je     loc_1_8988
        push   ebx
        mov    ebx, edi
        call   sub_1_14EC
        pop    ebx
        jmp    loc_1_8988
sub_1_899F:
        dec    dword ptr [edi + 0x6c]
        je     loc_1_89AB
loc_1_89A4:
        ret    
sub_1_89A5:
        cmp    dword ptr [edi + 0x6c], 0
        jne    loc_1_89A4
loc_1_89AB:
        cmp    dword ptr [edi + 0x68], 0
        jne    loc_1_89A4
        mov    eax, dword ptr [edi + 0x14]
        test   dword ptr [eax], 0x10000
        je     loc_1_89A4
        test   word ptr [edi + 0x52], 0x4000
        jne    loc_1_89A4
        push   ebx
        mov    ebx, edi
        call   sub_1_14FB
        pop    ebx
        jmp    loc_1_89A4
sub_1_89CF:
        push   ebx
        push   ecx
        push   edx
        mov    eax, dword ptr [0xb750]       ; -> loc_1_B750
        mov    ebx, dword ptr [0xb804]       ; -> obj1:0xB804
        mov    dword ptr [0xb804], eax       ; -> obj1:0xB804
        sub    ebx, eax
        sbb    edx, edx
        and    eax, edx
        add    eax, dword ptr [0xb750]       ; -> loc_1_B750
        shl    eax, 3
        push   eax
        mov    ah, byte ptr [0xb6f8]         ; -> obj1:0xB6F8
        VxDCall  Dev_048B.Service_0002h          ; cd2002008b04
        mov    ebx, eax
        pop    eax
        sub    edx, edx
        div    ebx
        mov    al, byte ptr [eax + 0xb80c]   ; -> obj1:0xB80C
        mov    byte ptr [0xb81d], al         ; -> loc_1_B81D
        pop    edx
        pop    ecx
        pop    ebx
        ret    
loc_1_8A14:
        cld    
        mov    dword ptr [0xb81e], 0         ; -> obj1:0xB81E
        cmp    edx, dword ptr [0xb822]       ; -> obj1:0xB822
        jne    loc_1_8A93
        push   ebx
        push   esi
        push   edi
        push   ebp
        inc    dword ptr [0xb7f8]            ; -> obj1:0xB7F8
        cmp    dword ptr [0xb718], 0xb6fc    ; -> obj1:0xB718
        je     loc_1_8A94
        push   edx
        mov    bl, 1
        call   sub_1_8279
        push   ebx
        call   sub_1_8AF8
        pop    eax
        pop    edx
        jb     loc_1_8A80
        cmp    eax, ebx
        jb     loc_1_8A54
        xchg   ebx, eax
loc_1_8A54:
        cmp    eax, 0x7d0
        jb     loc_1_8A60
loc_1_8A5B:
        mov    eax, 0x7d0
loc_1_8A60:
        cmp    dword ptr [0xb822], edx       ; -> obj1:0xB822
        jne    loc_1_8A80
        cmp    eax, 1
        adc    eax, 0
        mov    esi, 0x8a14                   ; -> loc_1_8A14
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        cld    
        mov    dword ptr [0xb81e], esi       ; -> obj1:0xB81E
loc_1_8A80:
        mov    eax, 0xb1d4                   ; -> obj1:0xB1D4
        cmp    dword ptr [eax], 0
        je     loc_1_8A8F
        call   sub_1_A258
loc_1_8A8F:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
loc_1_8A93:
        ret    
loc_1_8A94:
        cmp    dword ptr [0xb75c], 0         ; -> sub_1_B75C
        je     loc_1_8A5B
        cmp    dword ptr [0xb6b8], 0         ; -> obj1:0xB6B8
        jne    loc_1_8A5B
        cmp    dword ptr [0xb68c], 0         ; -> obj1:0xB68C
        jne    loc_1_8A5B
        pushal 
        call   sub_1_B75C
        cmp    eax, 0
        popal  
        je     loc_1_8A5B
        jmp    loc_1_8A80
loc_1_8ABE:
        cmp    edx, dword ptr [0xb7f8]       ; -> obj1:0xB7F8
        je     loc_1_8AE3
loc_1_8AC6:
        mov    edx, dword ptr [0xb7f8]       ; -> obj1:0xB7F8
        mov    dword ptr [0xb7fc], edx       ; -> obj1:0xB7FC
        mov    eax, 0x1388
        mov    esi, 0x8abe                   ; -> loc_1_8ABE
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        ret    
loc_1_8AE3:
        inc    dword ptr [0xb800]            ; -> obj1:0xB800
        pushal 
        call   sub_1_8C27
        popal  
        jmp    loc_1_8AC6
loc_1_8AF2:
        mov    ebx, 0x1f4
        ret    
sub_1_8AF8:
        cmp    byte ptr [0xb835], 0          ; -> loc_1_B835
        jne    loc_1_8AF2
        mov    dword ptr [0xb82a], 0         ; -> obj1:0xB82A
        mov    dword ptr [0xb82e], 1         ; -> obj1:0xB82E
        call   sub_1_89CF
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf000100
        cld    
        xchg   edx, eax
        mov    cl, byte ptr [0xb81d]         ; -> loc_1_B81D
        mov    ebx, 0xf4240
        sub    esi, esi
        mov    edi, 0xb748                   ; -> loc_1_B748
loc_1_8B34:
        mov    edi, dword ptr [edi]
        cmp    edi, 0xb748                   ; -> loc_1_B748
        je     loc_1_8B7C
        cmp    dword ptr [edi + 0x68], 8
        jae    loc_1_8B6C
        or     byte ptr [edi + 0x1f], 2
        inc    esi
        mov    eax, dword ptr [edi + 0x7c]
        shr    eax, cl
        add    eax, dword ptr [edi + 0x78]
        sub    eax, edx
        jbe    loc_1_8B34
        dec    esi
        mov    dword ptr [0xb82e], 0         ; -> obj1:0xB82E
        btr    dword ptr [edi + 0x1f], 1
        cmp    eax, ebx
        jae    loc_1_8B34
        xchg   ebx, eax
        jmp    loc_1_8B34
loc_1_8B6C:
        and    byte ptr [edi + 0x1f], 0xfd
        mov    dword ptr [0xb82e], 0         ; -> obj1:0xB82E
        jmp    loc_1_8B34
loc_1_8B7C:
        and    esi, esi
        je     loc_1_8C1B
        push   ebx
        mov    ecx, esi
        pushfd 
        cli    
        mov    esi, 0xb6fc                   ; -> obj1:0xB6FC
        mov    eax, eax
loc_1_8B90:
        mov    esi, dword ptr [esi + 0x20]
        cmp    esi, 0xb6fc                   ; -> obj1:0xB6FC
        je     loc_1_8C19
        mov    dword ptr [0xb82e], 0         ; -> obj1:0xB82E
        mov    edi, dword ptr [esi + 0xc]
        test   byte ptr [edi + 0x1f], 2
        je     loc_1_8B90
        cmp    dword ptr [edi + 0x80], 0
        jne    loc_1_8B90
        mov    eax, dword ptr [esi + 0x14]
        add    eax, dword ptr [edi + 0x70]
        cmp    eax, edx
        ja     loc_1_8B90
        mov    eax, dword ptr [esi + 0x1c]
        mov    ebx, dword ptr [esi + 0x20]
        mov    dword ptr [eax + 0x20], ebx
        mov    dword ptr [ebx + 0x1c], eax
        dec    dword ptr [0xb750]            ; -> loc_1_B750
        add    byte ptr [esi + 0x2b], 8
        mov    dword ptr [edi + 0x80], esi
        mov    dword ptr [esi + 0x1c], esi
        mov    dword ptr [esi + 0x20], esi
        popfd  
        mov    al, 1
        call   sub_1_83F7
        jae    loc_1_8C16
        cmp    dword ptr [edi + 0x80], 1
        jb     loc_1_8C16
        cmp    dword ptr [edi + 0x68], 0
        jne    loc_1_8C15
        bts    dword ptr [edi + 0x1f], 0
        jb     loc_1_8C16
        mov    esi, 0x8926                   ; -> loc_1_8926
        mov    eax, 0x64
        mov    edx, edi
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
loc_1_8C15:
        stc    
loc_1_8C16:
        pop    eax
        cmc    
        ret    
loc_1_8C19:
        popfd  
        pop    ebx
loc_1_8C1B:
        mov    eax, dword ptr [0xb82e]       ; -> obj1:0xB82E
        mov    dword ptr [0xb82a], eax       ; -> obj1:0xB82A
        clc    
        ret    
sub_1_8C27:
        cld    
        btr    dword ptr [0xb808], 0         ; -> obj1:0xB808
        inc    dword ptr [0xb826]            ; -> obj1:0xB826
        mov    edx, dword ptr [0xb826]       ; -> obj1:0xB826
        mov    dword ptr [0xb822], edx       ; -> obj1:0xB822
        jmp    loc_1_8A14
sub_1_8C47:
        bts    dword ptr [0xb808], 0         ; -> obj1:0xB808
        jb     loc_1_8C6D
        push   esi
        sub    esi, esi
        xchg   dword ptr [0xb81e], esi       ; -> obj1:0xB81E
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
        mov    esi, 0x8c27                   ; -> sub_1_8C27
        VMMCall  VMM.Schedule_Global_Event       ; cd200e000100
        cld    
        pop    esi
loc_1_8C6D:
        ret    
        db  08Bh, 0C0h
sub_1_8C70:
        pushal 
        mov    ah, 1
        push   esi
        call   sub_1_A3AF
        pop    esi
        jb     loc_1_8CF0
        call   sub_1_897F
        lea    edx, [esi + 0x10]
loc_1_8C84:
        mov    word ptr [ebx + 4], 1
        mov    dword ptr [ebx + 0x2c], esi
        mov    dword ptr [ebx + 0xc], 0x8cf2 ; -> loc_1_8CF2
        or     dword ptr [ebx + 8], 0x1000000
        or     dword ptr [ebx + 8], 2
        mov    al, byte ptr [edi + 0xbb]
        xor    byte ptr [ebx + 8], al
        mov    eax, dword ptr [edx]
        add    eax, dword ptr [edi + 0x10]
        mov    dword ptr [ebx + 0x10], eax
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [ebx + 0x18], eax
        add    edx, 8
        mov    dword ptr [esi + 0xc], edx
        lea    ecx, [ebx + 0x50]
        mov    byte ptr [ebx + 0x4c], 1
        mov    dword ptr [ebx + 0x1c], ecx
        mov    dword ptr [ecx], eax
        mov    edx, dword ptr [esi + 8]
        mov    dword ptr [ecx + 4], edx
        mov    edx, ecx
        mov    cl, byte ptr [edi + 0x4c]
        shl    eax, cl
        add    dword ptr [esi + 8], eax
        add    edx, 8
        mov    dword ptr [edx], 0
        mov    eax, ebx
        call   sub_1_A558
        push   edi
        call   sub_1_199E
        pop    edi
        clc    
loc_1_8CF0:
        popal  
        ret    
loc_1_8CF2:
        pushal 
        mov    edx, dword ptr [esi + 0x2c]
        cmp    word ptr [esi + 6], 0x10
        jae    loc_1_8D0A
        mov    edi, dword ptr [edx + 0xc]
        cmp    dword ptr [esi], -1
        je     loc_1_8D0A
        cmp    dword ptr [edi], 0
        jne    loc_1_8D19
loc_1_8D0A:
        mov    edx, esi
        mov    esi, 0x8d2b                   ; -> loc_1_8D2B
        VMMCall  VMM.Schedule_Global_Event       ; cd200e000100
        popal  
        ret    
loc_1_8D19:
        mov    ebx, esi
        mov    edi, dword ptr [edx + 4]
        mov    esi, edx
        mov    edi, dword ptr [edi + 0xc]
        mov    edx, dword ptr [esi + 0xc]
        jmp    loc_1_8C84
loc_1_8D2B:
        pushal 
        mov    eax, dword ptr [edx + 0x2c]
        mov    ebx, edx
        mov    esi, dword ptr [eax + 4]
        cmp    word ptr [ebx + 6], 0x10
        mov    edi, dword ptr [esi + 0xc]
        jb     loc_1_8D4F
        cmp    dword ptr [eax], -1
        je     loc_1_8D4F
        mov    dword ptr [0xb6b8], 2         ; -> obj1:0xB6B8
        jmp    loc_1_8D6B
loc_1_8D4F:
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
        VxDCall  Dev_048B.Service_0005h          ; cd2005008b04
        mov    dword ptr [eax], 0xffffffff
        mov    dword ptr [0xb6b8], 0         ; -> obj1:0xB6B8
loc_1_8D6B:
        call   sub_1_899F
        call   sub_1_A4AE
        bts    dword ptr [0xb808], 0         ; -> obj1:0xB808
        jb     loc_1_8D85
        popal  
        jmp    sub_1_8C27
loc_1_8D85:
        popal  
        clc    
        ret    
sub_1_8D88:
        mov    ah, 0xb
loc_1_8D8A:
        ror    al, 1
        add    al, byte ptr [esi]
        inc    esi
        dec    ah
        jne    loc_1_8D8A
        ret    
sub_1_8D94:
        mov    esi, dword ptr [ebp + 0x6c]
        test   esi, esi
        je     loc_1_8DA5
        cmp    cl, 0
        je     loc_1_8DD9
        jmp    loc_1_8E30
loc_1_8DA5:
        push   ecx
        sub    eax, eax
        mov    esi, eax
        xchg   dword ptr [0xb844], esi       ; -> obj1:0xB844
        cmp    esi, 0
        je     loc_1_8DC3
        push   edi
        mov    edi, esi
        mov    ecx, 0xf5
        rep stosd dword ptr es:[edi], eax
        pop    edi
        pop    ecx
        jmp    loc_1_8DD6
loc_1_8DC3:
        mov    eax, 0x3d4
        call   sub_1_A1CE
        mov    esi, eax
        pop    ecx
        jb     loc_1_8EA0
loc_1_8DD6:
        mov    dword ptr [ebp + 0x6c], esi
loc_1_8DD9:
        mov    byte ptr [esi + 9], 1
        mov    byte ptr [esi + 0xa], 0
        mov    word ptr [esi + 0xb0], 0
        cmp    cl, 0
        jne    loc_1_8E30
        test   word ptr [ebp + 0x42], 0x24
        je     loc_1_8E30
        mov    eax, dword ptr [ebp + 0x3a]
        movzx  ecx, word ptr [eax]
        sub    cx, word ptr [eax + 2]
        sub    ecx, 2
        mov    eax, ecx
        mov    dx, 0x1a
        div    dl
        cmp    dh, ah
        adc    al, dh
        inc    al
        mov    byte ptr [esi + 9], al
        mov    byte ptr [esi + 0xa], ah
        mov    word ptr [esi + 0xb0], cx
        bt     word ptr [ebp + 0x42], 9
        sbb    eax, eax
        and    eax, dword ptr [ebp + 0x48]
        mov    dword ptr [esi + 0x338], eax
loc_1_8E30:
        mov    byte ptr [esi + 4], 0
        mov    byte ptr [esi + 5], 0
        mov    byte ptr [esi + 6], 0
        mov    byte ptr [esi + 7], 0
        mov    byte ptr [esi + 8], 0
        mov    dword ptr [esi + 0xc], 0xffffffff
        mov    dword ptr [esi + 0x10], 0xffffffff
        mov    dword ptr [esi + 0x20], 0xffffffff
        mov    dword ptr [esi + 0x18], 0xffffffff
        mov    dword ptr [esi + 0x1c], 0xffffffff
        mov    dword ptr [esi + 0x14], 0xffffffff
        mov    byte ptr [esi + 0xb], 0
        mov    ax, word ptr [ebp + 0x42]
        and    ax, 0x410
        cmp    ax, 0x410
        jne    loc_1_8E9E
        sub    eax, eax
        lea    edi, [esi + 0x33c]
        mov    ecx, 0x26
        rep stosd dword ptr es:[edi], eax
        bts    dword ptr [esi + 0x354], 0
        inc    word ptr [esi + 0x340]
loc_1_8E9E:
        clc    
        ret    
loc_1_8EA0:
        stc    
        mov    eax, 8
        ret    
sub_1_8EA7:
        cmp    eax, dword ptr [esi + 0x338]
        jne    loc_1_8EB9
        mov    dword ptr [esi + 0x338], 0
loc_1_8EB9:
        cmp    eax, 0x98967f
        ja     loc_1_8EEC
        sub    eax, dword ptr [esi + 0x33c]
        jb     loc_1_8EEC
        cmp    eax, 0x2800
        jae    loc_1_8EEC
        mov    edx, eax
        and    eax, 0x3ff
        shr    edx, 0xa
        jne    loc_1_8EE4
        bts    dword ptr [esi + 0x354], eax
        jb     loc_1_8EEC
loc_1_8EE4:
        inc    word ptr [esi + edx*2 + 0x340]
loc_1_8EEC:
        ret    
sub_1_8EED:
        mov    eax, dword ptr [esi + 0x338]
        or     eax, eax
        jne    loc_1_8F2E
        cmp    word ptr [esi + 0x340], 0x400
        jae    loc_1_8F31
        lea    edi, [esi + 0x354]
        mov    eax, 0xffffffff
        mov    ecx, 0x20
        repe scasd eax, dword ptr es:[edi]
        je     loc_1_8F31
        mov    eax, dword ptr [edi - 4]
        not    eax
        bsf    eax, eax
        sub    ecx, 0x1f
        neg    ecx
        shl    ecx, 5
        add    eax, ecx
        add    eax, dword ptr [esi + 0x33c]
loc_1_8F2E:
        cmp    ecx, ecx
        ret    
loc_1_8F31:
        mov    eax, 0x400
        lea    edi, [esi + 0x342]
        mov    ecx, 9
        repe scasw ax, word ptr es:[edi]
        jbe    loc_1_8F55
        lea    eax, [esi + 0x342]
        sub    eax, edi
        neg    eax
        shl    eax, 9
        jmp    loc_1_8F5A
loc_1_8F55:
        mov    eax, 0x2800
loc_1_8F5A:
        add    dword ptr [esi + 0x33c], eax
        cmp    dword ptr [esi + 0x33c], 0x989400
        jae    loc_1_8F7D
        lea    edi, [esi + 0x340]
        sub    eax, eax
        mov    ecx, 0x25
        rep stosd dword ptr es:[edi], eax
        inc    ecx
        ret    
loc_1_8F7D:
        stc    
        ret    
sub_1_8F7F:
        sub    esp, 0x14
        mov    dword ptr [esp + 4], ebx
        mov    dword ptr [esp], ecx
        call   sub_1_910A
        lea    edi, [esp + 8]
        push   1
        push   8
        push   0x10
        push   esi
        push   edi
        VxDCall  IFSMgr.UniToBCS                 ; cd2040004000
        add    esp, 0x14
        lea    esi, [esi + 0x10]
        lea    edi, [edi + 8]
        push   1
        push   3
        push   6
        push   esi
        push   edi
        VxDCall  IFSMgr.UniToBCS                 ; cd2040004000
        add    esp, 0x14
        cmp    byte ptr [esp + 8], 0xe5
        jne    loc_1_8FC6
        mov    byte ptr [esp + 8], 5
loc_1_8FC6:
        mov    esi, dword ptr [ebp + 0x6c]
        mov    ah, byte ptr [esi + 0xa]
        mov    al, byte ptr [esi + 9]
        cmp    al, 1
        jbe    loc_1_9051
        movzx  ecx, word ptr [esi + 0xb0]
        test   ecx, ecx
        je     loc_1_9051
        dec    al
        test   ah, ah
        je     loc_1_8FE7
        add    ecx, 2
loc_1_8FE7:
        movzx  eax, al
        shl    eax, 5
        add    dword ptr [esp + 4], eax
        lea    esi, [esp + 8]
        sub    al, al
        call   sub_1_8D88
        mov    edx, ecx
        mov    esi, dword ptr [ebp + 0x28]
        mov    edi, dword ptr [esp + 4]
        mov    ah, 1
        sub    edi, 0x20
loc_1_900A:
        mov    ebx, 0xb848                   ; -> obj1:0xB848
        mov    byte ptr [edi], ah
        inc    ah
        mov    byte ptr [edi + 0xb], 0xf
        mov    byte ptr [edi + 0xc], 0
        mov    byte ptr [edi + 0xd], al
        mov    word ptr [edi + 0x1a], 0
loc_1_9024:
        mov    ecx, dword ptr [ebx]
        jecxz  loc_1_9046
        add    edi, dword ptr [ebx + 4]
        sub    edx, ecx
        jb     loc_1_9036
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_1_9031:
        add    ebx, 8
        jmp    loc_1_9024
loc_1_9036:
        add    ecx, edx
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    ecx, edx
        neg    ecx
        mov    al, 0xff
        rep stosb byte ptr es:[edi], al
        sub    edx, edx
        jmp    loc_1_9031
loc_1_9046:
        sub    edi, 0x40
        test   edx, edx
        jne    loc_1_900A
        or     byte ptr [edi + 0x20], 0x40
loc_1_9051:
        lea    esi, [esp + 8]
        mov    edi, dword ptr [esp + 4]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
        mov    ecx, dword ptr [esp]
        jecxz  loc_1_90BC
        lea    esi, [ecx + 0xb]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
        mov    edi, dword ptr [esp + 4]
        mov    eax, dword ptr [ebp + 0x1c]
        test   byte ptr [edi + 0xb], 0x10
        je     loc_1_9099
        cmp    word ptr [edi + 0x1a], 0
        jne    loc_1_9099
        test   byte ptr [eax + 0x52], 3
        jne    loc_1_9093
        cmp    word ptr [edi + 0x14], 0
        jne    loc_1_9099
        mov    word ptr [edi + 0x14], 0xffff
loc_1_9093:
        mov    word ptr [edi + 0x1a], 0xffff
loc_1_9099:
        test   byte ptr [eax + 0x53], 1
        je     loc_1_90B5
        mov    word ptr [edi + 0x12], 0
        mov    word ptr [edi + 0x10], 0
        mov    word ptr [edi + 0xe], 0
        mov    byte ptr [edi + 0xd], 0
loc_1_90B5:
        call   sub_1_90E6
        jmp    loc_1_90E2
loc_1_90BC:
        mov    al, byte ptr [ebp + 0x44]
        stosb  byte ptr es:[edi], al
        sub    eax, eax
        stosb  byte ptr es:[edi], al
        clc    
        call   sub_1_12EA
        bt     dword ptr [ebp + 0x44], 4
        sbb    eax, eax
        mov    word ptr [edi - 6], ax
        stosw  word ptr es:[edi], ax
        sub    eax, eax
        stosd  dword ptr es:[edi], eax
        mov    edi, dword ptr [esp + 4]
        call   sub_1_90E6
loc_1_90E2:
        add    esp, 0x14
        ret    
sub_1_90E6:
        test   byte ptr [ebp + 0x43], 0x10
        je     loc_1_9109
        mov    eax, dword ptr [ebp + 0x1c]
        test   byte ptr [eax + 0x53], 1
        jne    loc_1_9109
        mov    eax, dword ptr [ebp + 0x14]
        mov    edx, dword ptr [eax + 0xc]
        mov    eax, dword ptr [eax + 8]
        call   sub_1_141D
        mov    dword ptr [edi + 0xe], edx
        mov    byte ptr [edi + 0xd], al
loc_1_9109:
        ret    
sub_1_910A:
        sub    esp, 0x18
        mov    ebx, dword ptr [ebp + 0x6c]
        test   byte ptr [ebp + 0x43], 0x10
        je     loc_1_914F
        mov    eax, dword ptr [ebp + 0x14]
        mov    eax, dword ptr [eax + 4]
        movzx  ecx, word ptr [eax]
        sub    ecx, 2
        lea    eax, [eax + 2]
        lea    edx, [esp]
        push   ecx
        push   eax
        push   edx
        VxDCall  IFSMgr.ShortToLossyFcb          ; cd206e004000
        add    esp, 0xc
        mov    ecx, 0xb
        lea    edi, [esp]
        lea    esi, [ebx + 0x50]
        test   byte ptr [ebx + 8], 8
        je     loc_1_9148
        lea    esi, [ebx + 0x3a]
loc_1_9148:
        push   esi
        repe cmpsw word ptr [esi], word ptr es:[edi]
        pop    esi
        je     loc_1_916D
loc_1_914F:
        lea    esi, [ebx + 0x24]
        test   byte ptr [ebp + 0x42], 0x20
        je     loc_1_916D
        lea    esi, [ebx + 0x3a]
        test   byte ptr [ebx + 8], 8
        jne    loc_1_916D
        cmp    dword ptr [ebx + 0x338], 0
        jne    loc_1_916D
        lea    esi, [ebx + 0x50]
loc_1_916D:
        add    esp, 0x18
        ret    
sub_1_9171:
        movzx  ecx, word ptr [edi]
        shr    ecx, 1
        sub    ecx, 1
        jbe    loc_1_91B4
        lea    edi, [edi + 2]
loc_1_917E:
        sub    edx, edx
        mov    eax, 0x7e
        repne scasw ax, word ptr es:[edi]
        jne    loc_1_91B6
        jecxz  loc_1_91B6
loc_1_918C:
        movzx  eax, word ptr [edi]
        cmp    eax, 0x2e
        je     loc_1_91B6
        or     eax, eax
        je     loc_1_91B6
        sub    eax, 0x30
        cmp    eax, 9
        ja     loc_1_917E
        lea    edx, [edx + edx*4]
        lea    edx, [eax + edx*2]
        cmp    edx, 0xf423f
        ja     loc_1_91B4
        lea    edi, [edi + 2]
        loop   loc_1_918C
        ret    
loc_1_91B4:
        sub    edx, edx
loc_1_91B6:
        ret    
        db  000h
sub_1_91B8:
        sub    esp, 0x10
        mov    dword ptr [ebp + 0x3a], eax
        movzx  ecx, word ptr [eax]
        sub    cx, word ptr [eax + 2]
        je     loc_1_9346
        jb     loc_1_9358
        sub    ecx, 2
        jbe    loc_1_9358
        and    dword ptr [ebp + 0x70], 0
        and    word ptr [ebp + 0x42], 0xffef
        and    dword ptr [esp + 4], 0
        test   byte ptr [ebp + 0x43], 8
        je     loc_1_9202
        mov    eax, dword ptr [ebp + 0x10]
        mov    word ptr [eax], 4
        mov    word ptr [eax + 2], 4
        mov    word ptr [eax + 4], 0
loc_1_9202:
        test   byte ptr [ebp + 0x43], 0x28
        jne    loc_1_924F
        mov    esi, dword ptr [ebp + 0x3a]
        call   sub_1_07D4
        jb     loc_1_9346
        jne    loc_1_924F
        mov    dword ptr [ebp + 0x70], eax
        bt     word ptr [ebp + 0x42], 0xf
        jae    loc_1_923F
        mov    ecx, eax
        mov    esi, dword ptr [ebp + 0x3a]
        movzx  eax, word ptr [esi + 2]
        lea    esi, [eax + esi + 2]
        call   sub_1_09AF
        jne    loc_1_923F
        mov    dword ptr [esp + 8], eax
        mov    dword ptr [esp + 4], ebx
loc_1_923F:
        call   sub_1_1201
        jae    loc_1_92E9
        jmp    loc_1_9358
loc_1_924F:
        call   sub_1_11BD
        mov    esi, dword ptr [ebp + 0x3a]
        lea    esi, [esi + 4]
        movzx  eax, word ptr [esi]
        cmp    word ptr [eax + esi], 0
        je     loc_1_92E9
loc_1_9268:
        mov    dword ptr [esp], esi
        mov    ax, 0x16
        xchg   word ptr [ebp + 0x45], ax
        mov    word ptr [esp + 0xc], ax
        mov    eax, dword ptr [esp]
        mov    ecx, 0
        call   sub_1_9366
        jae    loc_1_9294
        or     eax, eax
        jne    loc_1_9361
        jmp    loc_1_9358
loc_1_9294:
        test   byte ptr [ebx + 0xb], 0x10
        je     loc_1_9358
        call   sub_1_99B8
        jb     loc_1_935F
        mov    ax, word ptr [esp + 0xc]
        mov    word ptr [ebp + 0x45], ax
        mov    esi, dword ptr [esp]
        movzx  eax, word ptr [esi]
        lea    esi, [eax + esi]
        movzx  eax, word ptr [esi]
        cmp    word ptr [eax + esi], 0
        je     loc_1_92D1
        call   sub_1_11ED
        jae    loc_1_9268
        jmp    loc_1_9358
loc_1_92D1:
        test   byte ptr [ebp + 0x43], 0x28
        jne    loc_1_92E2
        mov    esi, dword ptr [ebp + 0x3a]
        call   sub_1_0891
        mov    dword ptr [ebp + 0x70], eax
loc_1_92E2:
        call   sub_1_11ED
        jb     loc_1_9358
loc_1_92E9:
        or     word ptr [ebp + 0x42], 0x10
        mov    esi, dword ptr [ebp + 0x3a]
        movzx  eax, word ptr [esi + 2]
        lea    esi, [eax + esi]
        mov    eax, esi
        cmp    dword ptr [esp + 4], 0
        je     loc_1_9320
        mov    ebx, dword ptr [esp + 4]
        mov    edx, dword ptr [esp + 8]
        mov    ecx, 3
        call   sub_1_9366
        jae    loc_1_9332
        or     eax, eax
        jne    loc_1_9361
        and    dword ptr [esp + 4], 0
        jmp    loc_1_92E9
loc_1_9320:
        mov    ecx, 0
        call   sub_1_9366
        jae    loc_1_9332
        or     eax, eax
        jne    loc_1_9361
        jmp    loc_1_934F
loc_1_9332:
        call   sub_1_99B8
        jb     loc_1_935F
        add    esp, 0x10
        test   byte ptr [ebx + 0xb], 0x10
        sete   al
        and    al, al
        ret    
loc_1_9346:
        add    esp, 0x10
        or     ebx, 0xffffffff
        xor    ah, ah
        ret    
loc_1_934F:
        mov    al, 2
        add    esp, 0x10
        cmp    eax, eax
        stc    
        ret    
loc_1_9358:
        mov    al, 3
        add    esp, 0x10
        stc    
        ret    
loc_1_935F:
        mov    al, 5
loc_1_9361:
        add    esp, 0x10
        stc    
        ret    
sub_1_9366:
        mov    dword ptr [ebp + 0x3e], eax
        sub    esp, 0x10
        mov    byte ptr [esp + 6], cl
        mov    byte ptr [esp + 7], 0
        mov    dword ptr [esp + 8], ebx
        mov    dword ptr [esp + 0xc], edx
        sub    eax, eax
        bt     word ptr [ebp + 0x42], 0xd
        adc    eax, eax
        mov    dword ptr [esp], eax
        pushfd 
        cmp    dword ptr [ebp + 0x64], 0
        je     loc_1_93A0
        push   esi
        mov    esi, dword ptr [ebp + 0x64]
        VxDCall  Dev_048B.Service_0007h          ; cd2007008b04
        pop    esi
        inc    byte ptr [esp + 0xb]
loc_1_93A0:
        popfd  
        call   sub_1_8D94
        jb     loc_1_9944
        mov    edi, dword ptr [ebp + 0x3e]
        movzx  ecx, word ptr [edi]
        sub    eax, eax
        xchg   word ptr [ecx + edi], ax
        mov    word ptr [esp + 4], ax
        mov    al, byte ptr [ebp + 0x42]
        and    al, 0x30
        cmp    al, 0x10
        jne    loc_1_93E1
        mov    eax, dword ptr [ebp + 0x3e]
        movzx  ecx, word ptr [eax]
        sub    ecx, 2
        lea    eax, [eax + 2]
        lea    edx, [esi + 0x24]
        push   ecx
        push   eax
        push   edx
        VxDCall  IFSMgr.ShortToLossyFcb          ; cd206e004000
        add    esp, 0xc
loc_1_93E1:
        mov    ax, word ptr [ebp + 0x42]
        and    ax, 0x410
        cmp    ax, 0x410
        jne    loc_1_941C
        mov    eax, dword ptr [ebp + 0x3e]
        movzx  ecx, word ptr [eax]
        sub    ecx, 2
        mov    eax, dword ptr [ebp + 0x28]
        lea    edx, [esi + 0x3a]
        push   ecx
        push   eax
        push   edx
        VxDCall  IFSMgr.CreateBasis              ; cd2045004000
        add    esp, 0xc
        or     byte ptr [esi + 8], al
        push   esi
        lea    edi, [esi + 0x50]
        lea    esi, [esi + 0x3a]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        pop    esi
loc_1_941C:
        mov    byte ptr [esi + 0x7c], 0
        test   word ptr [ebp + 0x42], 0x810
        jne    loc_1_9445
        mov    eax, dword ptr [ebp + 0x3e]
        movzx  ecx, word ptr [eax]
        sub    ecx, 2
        jbe    loc_1_9445
        lea    ebx, [eax + 2]
        lea    eax, [esi + 0x7c]
        push   ecx
        push   ebx
        push   eax
        VxDCall  IFSMgr.Service_0078h            ; cd2078004000
        add    esp, 0xc
loc_1_9445:
        mov    ebx, dword ptr [esp + 8]
        mov    edx, dword ptr [esp + 0xc]
        cmp    byte ptr [esp + 6], 0
        je     loc_1_94BD
        cmp    byte ptr [esp + 6], 1
        je     loc_1_9842
        or     ebx, ebx
        jne    loc_1_9472
        mov    eax, dword ptr [ebp + 0x1c]
        movzx  eax, word ptr [eax + 0x44]
        cmp    edx, eax
        jae    loc_1_991B
loc_1_9472:
        call   sub_1_1201
        jb     loc_1_991B
        pushfd 
        cmp    byte ptr [esp + 0xb], 0
        je     loc_1_9494
        push   esi
        mov    esi, dword ptr [ebp + 0x64]
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
        pop    esi
        dec    byte ptr [esp + 0xb]
loc_1_9494:
        popfd  
        mov    eax, dword ptr [esp + 0xc]
        call   sub_1_0BD7
        jb     loc_1_991B
        pushfd 
        cmp    dword ptr [ebp + 0x64], 0
        je     loc_1_94BA
        push   esi
        mov    esi, dword ptr [ebp + 0x64]
        VxDCall  Dev_048B.Service_0007h          ; cd2007008b04
        pop    esi
        inc    byte ptr [esp + 0xb]
loc_1_94BA:
        popfd  
        jmp    loc_1_950C
loc_1_94BD:
        mov    dword ptr [ebp + 0x4c], 0
        mov    al, byte ptr [ebp + 0x45]
        and    al, 0x9e
        cmp    al, 8
        jne    loc_1_94D2
        call   sub_1_11BD
loc_1_94D2:
        pushfd 
        cmp    byte ptr [esp + 0xb], 0
        je     loc_1_94E9
        push   esi
        mov    esi, dword ptr [ebp + 0x64]
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
        pop    esi
        dec    byte ptr [esp + 0xb]
loc_1_94E9:
        popfd  
        call   sub_1_0BD4
        jb     loc_1_991B
        pushfd 
        cmp    dword ptr [ebp + 0x64], 0
        je     loc_1_950B
        push   esi
        mov    esi, dword ptr [ebp + 0x64]
        VxDCall  Dev_048B.Service_0007h          ; cd2007008b04
        pop    esi
        inc    byte ptr [esp + 0xb]
loc_1_950B:
        popfd  
loc_1_950C:
        mov    esi, dword ptr [ebp + 0x6c]
        cmp    dword ptr [esp], 1
        jne    loc_1_9536
        movzx  eax, word ptr [esi + 0x2b2]
        cmp    eax, 0x10
        jae    loc_1_9536
        mov    ecx, dword ptr [ebp + 0x50]
        mov    dword ptr [esi + eax*8 + 0x2b6], ecx
        and    dword ptr [esi + eax*8 + 0x2ba], 0
        inc    dword ptr [esp]
loc_1_9536:
        mov    ah, byte ptr [ebx]
        call   sub_1_9A38
        jb     loc_1_98D4
        jne    loc_1_9564
        mov    byte ptr [esi + 4], 0
        mov    byte ptr [esi + 7], 0
        mov    byte ptr [esi + 5], 0
        mov    dword ptr [esi + 0xc], 0xffffffff
        mov    dword ptr [esi + 0x10], 0xffffffff
        jmp    loc_1_9842
loc_1_9564:
        mov    al, byte ptr [ebx + 0xb]
        and    al, 0x1f
        cmp    al, 0xf
        jne    loc_1_9600
        mov    ah, byte ptr [ebx]
        mov    al, byte ptr [ebx + 0xd]
        and    ah, 0xbf
        cmp    ah, 0x14
        ja     loc_1_9825
        cmp    ah, 1
        jb     loc_1_9825
        test   byte ptr [ebx], 0x40
        je     loc_1_95B7
        mov    byte ptr [esi + 4], 0
        mov    byte ptr [esi + 5], ah
        mov    byte ptr [esi + 6], al
        mov    byte ptr [esi + 7], 0
        mov    al, 0x1a
        mul    ah
        movzx  eax, ax
        mov    word ptr [eax + esi + 0xb2], 0
        mov    eax, dword ptr [ebp + 0x4c]
        mov    dword ptr [esi + 0xc], eax
        jmp    loc_1_95D3
loc_1_95B7:
        cmp    byte ptr [esi + 4], 0
        je     loc_1_9825
        cmp    byte ptr [esi + 5], ah
        jne    loc_1_9825
        cmp    byte ptr [esi + 6], al
        jne    loc_1_9825
loc_1_95D3:
        dec    byte ptr [esi + 5]
        inc    byte ptr [esi + 4]
        movzx  eax, byte ptr [esi + 5]
        mov    ah, 0x1a
        mul    ah
        lea    edi, [esi + eax + 0xb2]
        push   esi
        lea    esi, [ebx + 1]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        add    esi, 3
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        add    esi, 2
        movsd  dword ptr es:[edi], dword ptr [esi]
        pop    esi
        jmp    loc_1_9842
loc_1_9600:
        bt     dword ptr [ebx + 0xb], 3
        sbb    eax, eax
        and    eax, 0x40
        or     word ptr [ebp + 0x42], ax
        or     byte ptr [esi + 7], 1
        mov    dword ptr [esi + 0x94], 0
        cmp    byte ptr [esi + 0x7c], 0
        jne    loc_1_9627
        call   sub_1_9CEF
loc_1_9627:
        mov    eax, dword ptr [ebp + 0x50]
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [ebp + 0x4c]
        mov    dword ptr [esi + 0x10], eax
        cmp    byte ptr [esi + 4], 0
        je     loc_1_9652
        cmp    byte ptr [esi + 5], 0
        jne    loc_1_9652
        push   esi
        lea    esi, [ebx]
        sub    al, al
        call   sub_1_8D88
        pop    esi
        cmp    byte ptr [esi + 6], al
        jne    loc_1_9652
        or     byte ptr [esi + 7], 2
loc_1_9652:
        test   byte ptr [ebx + 0xb], 0x18
        jne    loc_1_9670
        mov    ax, word ptr [ebp + 0x42]
        and    ax, 0x4010
        cmp    ax, 0x4010
        jne    loc_1_9670
        mov    ecx, dword ptr [ebp + 0x70]
        jecxz  loc_1_9670
        call   sub_1_0A19
loc_1_9670:
        mov    ax, word ptr [ebp + 0x42]
        and    ax, 0x410
        cmp    ax, 0x410
        jne    loc_1_96C5
        test   byte ptr [ebx + 0xb], 8
        jne    loc_1_96C5
        lea    edi, [esi + 0x3a]
        lea    eax, [esi + 0x66]
        push   edx
        push   eax
        push   edi
        VxDCall  IFSMgr.MatchBasisName           ; cd2046004000
        add    esp, 8
        pop    edx
        test   eax, eax
        je     loc_1_96C5
        js     loc_1_96BB
        or     byte ptr [esi + 8], 0x10
        push   edx
        call   sub_1_8EA7
        pop    edx
        lea    edi, [esi + 0x3a]
        lea    esi, [esi + 0x66]
        mov    ecx, 0xb
        repe cmpsw word ptr [esi], word ptr es:[edi]
        mov    esi, dword ptr [ebp + 0x6c]
        jne    loc_1_96C5
loc_1_96BB:
        or     byte ptr [esi + 8], 8
        mov    eax, dword ptr [ebp + 0x4c]
        mov    dword ptr [esi + 0x20], eax
loc_1_96C5:
        mov    al, byte ptr [ebp + 0x42]
        and    al, 0x30
        cmp    al, 0x10
        je     loc_1_976B
        movzx  eax, byte ptr [ebp + 0x42]
        and    al, 0x11
        cmp    al, 0x11
        setne  al
        dec    eax
        and    eax, 1
        mov    edi, dword ptr [ebp + 0x3e]
        lea    edi, [edi + 2]
        test   byte ptr [esi + 7], 2
        je     loc_1_9728
        lea    ecx, [esi + 0xb2]
        push   edx
        push   eax
        or     eax, 4
        push   eax
        push   ecx
        push   edi
        VxDCall  IFSMgr.IFSMgr_MetaMatch         ; cd203d004000
        add    esp, 0xc
        test   eax, eax
        pop    eax
        pop    edx
        je     loc_1_9728
        cmp    word ptr [esi + 0x94], 0
        jne    loc_1_9719
        call   sub_1_9CEF
loc_1_9719:
        call   sub_1_9C63
        je     loc_1_9884
        and    byte ptr [esi + 7], 0xfd
loc_1_9728:
        cmp    byte ptr [esi + 0x7c], 0
        je     loc_1_974A
        mov    ecx, 0xb
        push   esi
        push   edi
        lea    esi, [esi + 0x7c]
        lea    edi, [ebx]
        cld    
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        pop    edi
        pop    esi
        jne    loc_1_97FB
        jmp    loc_1_97CA
loc_1_974A:
        lea    ecx, [esi + 0x96]
        push   edx
        or     eax, 4
        push   eax
        push   ecx
        push   edi
        VxDCall  IFSMgr.IFSMgr_MetaMatch         ; cd203d004000
        add    esp, 0xc
        pop    edx
        test   eax, eax
        je     loc_1_97FB
        jmp    loc_1_97CA
loc_1_976B:
        lea    eax, [esi + 0x24]
        lea    ecx, [esi + 0x66]
        push   edx
        push   1
        push   ecx
        push   eax
        VxDCall  IFSMgr.IFSMgr_MetaMatch         ; cd203d004000
        add    esp, 0xc
        pop    edx
        test   eax, eax
        jne    loc_1_97CA
        test   byte ptr [ebp + 0x42], 1
        jne    loc_1_97FB
        test   byte ptr [esi + 7], 2
        je     loc_1_97FB
        mov    eax, dword ptr [ebp + 0x3e]
        movzx  ecx, word ptr [eax]
        cmp    word ptr [ecx + esi + 0xb0], 0
        jne    loc_1_97FB
        lea    eax, [eax + 2]
        lea    ecx, [esi + 0xb2]
        push   edx
        push   4
        push   ecx
        push   eax
        VxDCall  IFSMgr.IFSMgr_MetaMatch         ; cd203d004000
        add    esp, 0xc
        pop    edx
        test   eax, eax
        je     loc_1_97FB
        call   sub_1_9C63
        je     loc_1_9884
        jmp    loc_1_97FB
loc_1_97CA:
        or     byte ptr [esi + 7], 4
        cmp    word ptr [esi + 0x94], 0
        jne    loc_1_97DD
        call   sub_1_9CEF
loc_1_97DD:
        test   byte ptr [esi + 7], 2
        je     loc_1_9884
        call   sub_1_9C63
        je     loc_1_9884
        and    byte ptr [esi + 7], 0xfd
        jmp    loc_1_9884
loc_1_97FB:
        mov    esi, dword ptr [ebp + 0x6c]
        mov    byte ptr [esi + 4], 0
        mov    byte ptr [esi + 7], 0
        mov    byte ptr [esi + 5], 0
        mov    dword ptr [esi + 0xc], 0xffffffff
        mov    dword ptr [esi + 0x10], 0xffffffff
        cmp    byte ptr [esp + 6], 3
        je     loc_1_991B
        jmp    loc_1_9842
loc_1_9825:
        mov    esi, dword ptr [ebp + 0x6c]
        mov    byte ptr [esi + 4], 0
        mov    byte ptr [esi + 7], 0
        mov    byte ptr [esi + 5], 0
        mov    dword ptr [esi + 0xc], 0xffffffff
        mov    dword ptr [esi + 0x10], 0xffffffff
loc_1_9842:
        pushfd 
        cmp    byte ptr [esp + 0xb], 0
        je     loc_1_9859
        push   esi
        mov    esi, dword ptr [ebp + 0x64]
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
        pop    esi
        dec    byte ptr [esp + 0xb]
loc_1_9859:
        popfd  
        call   sub_1_0CDE
        jb     loc_1_991B
        pushfd 
        cmp    dword ptr [ebp + 0x64], 0
        je     loc_1_987B
        push   esi
        mov    esi, dword ptr [ebp + 0x64]
        VxDCall  Dev_048B.Service_0007h          ; cd2007008b04
        pop    esi
        inc    byte ptr [esp + 0xb]
loc_1_987B:
        popfd  
        jne    loc_1_950C
        jmp    loc_1_98D4
loc_1_9884:
        mov    al, byte ptr [ebx + 0xb]
        mov    ah, byte ptr [ebp + 0x46]
        and    ah, al
        xor    ah, byte ptr [ebp + 0x46]
        and    ah, 0x3f
        jne    loc_1_98C2
        mov    ah, byte ptr [ebp + 0x45]
        and    ah, 0x9e
        test   al, 8
        je     loc_1_98AC
        test   ah, 8
        jne    loc_1_9963
        jmp    loc_1_9825
loc_1_98AC:
        cmp    ah, 8
        je     loc_1_9825
        not    ah
        and    ah, al
        and    ah, 0x16
        je     loc_1_9963
loc_1_98C2:
        test   byte ptr [ebp + 0x42], 1
        je     loc_1_991B
        cmp    byte ptr [esp + 6], 3
        je     loc_1_991B
        jmp    loc_1_9825
loc_1_98D4:
        mov    esi, dword ptr [ebp + 0x6c]
        mov    ax, word ptr [ebp + 0x42]
        and    ax, 0x410
        cmp    ax, 0x410
        jne    loc_1_991B
        cmp    dword ptr [esi + 0x338], 0
        jne    loc_1_9912
        cmp    word ptr [esi + 0x3a], 0x20
        je     loc_1_9912
        test   byte ptr [esi + 8], 0xb
        je     loc_1_991B
        test   byte ptr [ebp + 0x43], 0x10
        jne    loc_1_990A
        cmp    dword ptr [0xb88c], 0         ; -> obj1:0xB88C
        jne    loc_1_9912
loc_1_990A:
        test   byte ptr [esi + 8], 0x18
        jne    loc_1_9916
        jmp    loc_1_991B
loc_1_9912:
        or     byte ptr [esi + 8], 8
loc_1_9916:
        call   sub_1_9B41
loc_1_991B:
        mov    esi, dword ptr [ebp + 0x6c]
        mov    eax, dword ptr [ebp + 0x50]
        mov    dword ptr [esi], eax
        mov    dword ptr [esi + 0xc], 0xffffffff
        mov    dword ptr [esi + 0x10], 0xffffffff
        mov    eax, dword ptr [ebp + 0x3e]
        movzx  ecx, word ptr [eax]
        lea    ecx, [eax + ecx]
        mov    ax, word ptr [esp + 4]
        mov    word ptr [ecx], ax
        sub    eax, eax
loc_1_9944:
        pushfd 
        cmp    byte ptr [esp + 0xb], 0
        je     loc_1_995B
        push   esi
        mov    esi, dword ptr [ebp + 0x64]
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
        pop    esi
        dec    byte ptr [esp + 0xb]
loc_1_995B:
        popfd  
        add    esp, 0x10
        sub    ah, ah
        stc    
        ret    
loc_1_9963:
        mov    esi, dword ptr [ebp + 0x6c]
        mov    eax, dword ptr [ebp + 0x3e]
        movzx  ecx, word ptr [eax]
        lea    ecx, [eax + ecx]
        mov    ax, word ptr [esp + 4]
        mov    word ptr [ecx], ax
        cmp    dword ptr [esp], 2
        jne    loc_1_999A
        movzx  eax, word ptr [esi + 0x2b2]
        cmp    eax, 0x10
        jae    loc_1_999A
        mov    ecx, dword ptr [ebp + 0x4c]
        mov    dword ptr [esi + eax*8 + 0x2ba], ecx
        inc    word ptr [esi + 0x2b2]
loc_1_999A:
        pushfd 
        cmp    byte ptr [esp + 0xb], 0
        je     loc_1_99B1
        push   esi
        mov    esi, dword ptr [ebp + 0x64]
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
        pop    esi
        dec    byte ptr [esp + 0xb]
loc_1_99B1:
        popfd  
        add    esp, 0x10
        sub    eax, eax
        ret    
sub_1_99B8:
        test   byte ptr [ebp + 0x43], 8
        je     loc_1_99FA
        mov    esi, dword ptr [ebp + 0x6c]
        mov    edi, dword ptr [ebp + 0x10]
        cmp    byte ptr [ebp + 4], 4
        je     loc_1_99FC
loc_1_99CA:
        movzx  ecx, word ptr [esi + 0x94]
        movzx  eax, word ptr [edi]
        mov    word ptr [edi + 2], ax
        add    word ptr [edi], cx
        cmp    word ptr [edi], 0x1fe
        jae    loc_1_9A36
        mov    word ptr [eax + edi], cx
        lea    edi, [eax + edi + 2]
        lea    esi, [esi + 0x96]
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        adc    ecx, ecx
        rep movsw word ptr es:[edi], word ptr [esi]
loc_1_99FA:
        clc    
        ret    
loc_1_99FC:
        test   byte ptr [esi + 7], 2
        je     loc_1_99CA
        mov    eax, edi
        movzx  ecx, word ptr [edi]
        mov    word ptr [edi + 2], cx
        lea    esi, [esi + 0xb2]
        lea    edi, [ecx + edi + 2]
loc_1_9A15:
        cmp    ecx, 0x1fe
        jae    loc_1_9A36
        movsw  word ptr es:[edi], word ptr [esi]
        add    ecx, 2
        cmp    word ptr [edi - 2], 0
        jne    loc_1_9A15
        mov    edi, ecx
        xchg   word ptr [eax], cx
        sub    edi, ecx
        mov    word ptr [ecx + eax], di
        clc    
        ret    
loc_1_9A36:
        stc    
        ret    
sub_1_9A38:
        or     ah, ah
        je     loc_1_9A88
        cmp    ah, 0xe5
        je     loc_1_9A56
        mov    al, byte ptr [esi + 0xb]
        cmp    al, byte ptr [esi + 9]
        jae    loc_1_9A85
        mov    dword ptr [esi + 0x14], 0xffffffff
        mov    byte ptr [esi + 0xb], 0
        jmp    loc_1_9A85
loc_1_9A56:
        movzx  eax, byte ptr [esi + 0xb]
        cmp    al, byte ptr [esi + 9]
        jae    loc_1_9A76
        add    eax, dword ptr [esi + 0x14]
        cmp    eax, dword ptr [ebp + 0x4c]
        jne    loc_1_9A6C
        inc    byte ptr [esi + 0xb]
        jmp    loc_1_9A76
loc_1_9A6C:
        mov    eax, dword ptr [ebp + 0x4c]
        mov    dword ptr [esi + 0x14], eax
        mov    byte ptr [esi + 0xb], 1
loc_1_9A76:
        cmp    dword ptr [esi + 0x18], -1
        jne    loc_1_9A82
        mov    eax, dword ptr [ebp + 0x4c]
        mov    dword ptr [esi + 0x18], eax
loc_1_9A82:
        cmp    eax, eax
        ret    
loc_1_9A85:
        or     esp, esp
        ret    
loc_1_9A88:
        mov    eax, dword ptr [ebp + 0x4c]
        mov    dword ptr [esi + 0x1c], eax
        stc    
        ret    
sub_1_9A90:
        push   ecx
        push   eax
        call   sub_1_0BD7
        jb     loc_1_9B32
loc_1_9A9D:
        mov    dword ptr [esp], ebx
loc_1_9AA0:
        mov    al, byte ptr [ebx]
        test   al, al
        je     loc_1_9B3C
        cmp    al, 0xe5
        je     loc_1_9B3C
        mov    ah, 0xf
        mov    al, byte ptr [ebx + 0xb]
        and    al, ah
        cmp    al, ah
        je     loc_1_9AC4
        cmp    dword ptr [esp + 4], 1
        jne    loc_1_9B3C
loc_1_9AC4:
        mov    byte ptr [ebx], 0xe5
        dec    dword ptr [esp + 4]
        je     loc_1_9AD7
        inc    dword ptr [ebp + 0x4c]
        add    ebx, 0x20
        cmp    ebx, edx
        jb     loc_1_9AA0
loc_1_9AD7:
        mov    esi, dword ptr [ebp + 0x64]
        mov    ebx, dword ptr [esp]
        call   sub_1_03EC
        jb     loc_1_9B32
        mov    ecx, dword ptr [esp + 4]
        jecxz  loc_1_9B37
        mov    edi, dword ptr [ebp + 0x1c]
        mov    al, byte ptr [ebp + 0x60]
        inc    eax
        mov    byte ptr [ebp + 0x60], al
        sub    al, byte ptr [ebp + 0x61]
        je     loc_1_9AFF
        mov    ebx, dword ptr [ebp + 0x5c]
        inc    ebx
        jmp    loc_1_9B28
loc_1_9AFF:
        mov    byte ptr [ebp + 0x60], al
        mov    ebx, dword ptr [ebp + 0x54]
        dec    dword ptr [ebp + 0x4c]
        call   sub_1_1F2C
        jb     loc_1_9B32
        call   sub_1_2102
        jae    loc_1_9B32
        inc    dword ptr [ebp + 0x4c]
        mov    dword ptr [ebp + 0x54], ebx
        inc    dword ptr [ebp + 0x58]
        sub    edx, edx
        call   sub_1_7E5E
        jb     loc_1_9B32
loc_1_9B28:
        call   sub_1_0C48
        jmp    loc_1_9A9D
loc_1_9B32:
        add    esp, 8
        stc    
        ret    
loc_1_9B37:
        add    esp, 8
        clc    
        ret    
loc_1_9B3C:
        add    esp, 8
        stc    
        ret    
sub_1_9B41:
        sub    esp, 0x2c
        jmp    loc_1_9C3B
loc_1_9B49:
        mov    dword ptr [ebp + 0x4c], 0
        mov    al, byte ptr [ebp + 0x45]
        and    al, 0x9e
        cmp    al, 8
        jne    loc_1_9B5E
        call   sub_1_11BD
loc_1_9B5E:
        call   sub_1_0BD4
        jb     loc_1_9C5E
loc_1_9B69:
        mov    dword ptr [esp], edx
        cmp    byte ptr [ebx], 0
        je     loc_1_9C3B
        cmp    byte ptr [ebx], 0xe5
        je     loc_1_9C2B
        test   byte ptr [ebx + 0xb], 8
        jne    loc_1_9C2B
        pushfd 
        cmp    dword ptr [ebp + 0x64], 0
        je     loc_1_9B9E
        push   esi
        mov    esi, dword ptr [ebp + 0x64]
        VxDCall  Dev_048B.Service_0007h          ; cd2007008b04
        pop    esi
        inc    byte ptr [esp + 0x2c]
loc_1_9B9E:
        popfd  
        lea    esi, [ebx]
        lea    edi, [esp + 4]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
        cmp    byte ptr [esp + 4], 5
        jne    loc_1_9BB6
        mov    byte ptr [esp + 4], 0xe5
loc_1_9BB6:
        mov    eax, 0x200020
        lea    edi, [esp + 0x10]
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        lea    eax, [esp + 4]
        lea    edi, [esp + 0x10]
        push   1
        push   8
        push   eax
        push   edi
        VxDCall  IFSMgr.BCSToUni                 ; cd2042004000
        add    esp, 0x10
        lea    eax, [esp + 0xc]
        lea    edi, [esp + 0x20]
        push   1
        push   3
        push   eax
        push   edi
        VxDCall  IFSMgr.BCSToUni                 ; cd2042004000
        add    esp, 0x10
        lea    eax, [esp + 0x10]
        mov    esi, dword ptr [ebp + 0x6c]
        lea    edi, [esi + 0x3a]
        push   eax
        push   edi
        VxDCall  IFSMgr.MatchBasisName           ; cd2046004000
        add    esp, 8
        cmp    eax, 0
        jle    loc_1_9C13
        call   sub_1_8EA7
loc_1_9C13:
        pushfd 
        cmp    byte ptr [esp + 0x2c], 0
        je     loc_1_9C2A
        push   esi
        mov    esi, dword ptr [ebp + 0x64]
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
        pop    esi
        dec    byte ptr [esp + 0x2c]
loc_1_9C2A:
        popfd  
loc_1_9C2B:
        mov    edx, dword ptr [esp]
        call   sub_1_0CDE
        ja     loc_1_9B69
        jb     loc_1_9C5E
loc_1_9C3B:
        mov    esi, dword ptr [ebp + 0x6c]
        call   sub_1_8EED
        jb     loc_1_9C5E
        jne    loc_1_9B49
        lea    edi, [esi + 0x3a]
        push   eax
        push   edi
        VxDCall  IFSMgr.AppendBasisTail          ; cd2047004000
        add    esp, 8
        add    esp, 0x2c
        clc    
        ret    
loc_1_9C5E:
        add    esp, 0x2c
        stc    
        ret    
sub_1_9C63:
        push   eax
        push   ebx
        push   edx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0x6c]
        lea    edi, [esi + 0x96]
        lea    esi, [esi + 0xb2]
        mov    ebx, dword ptr [0xb288]       ; -> loc_1_B288
        mov    ecx, dword ptr [0xb284]       ; -> loc_1_B284
        sub    edi, 2
loc_1_9C86:
        add    edi, 2
        movzx  eax, word ptr [edi]
        cmp    eax, 0x2e
        je     loc_1_9CE9
        cmp    eax, 0x7e
        je     loc_1_9CE9
        or     eax, eax
        je     loc_1_9CE9
        cmp    eax, 0x20
        je     loc_1_9CE6
loc_1_9C9F:
        lodsw  ax, word ptr [esi]
        cmp    eax, 0x586
        ja     loc_1_9CB0
        movzx  edx, byte ptr [eax + ebx]
        sub    ax, word ptr [edx + ecx]
loc_1_9CB0:
        cmp    ax, word ptr [edi]
        je     loc_1_9C86
        cmp    eax, 0x20
        je     loc_1_9C9F
        cmp    eax, 0x2e
        je     loc_1_9C9F
        or     eax, eax
        je     loc_1_9CD0
        cmp    word ptr [edi], 0x5f
        je     loc_1_9C86
        cmp    word ptr [edi], 0x80
        jae    loc_1_9C86
loc_1_9CD0:
        movzx  eax, word ptr [edi]
        cmp    eax, 0x7e
        je     loc_1_9CE9
        cmp    eax, 0x2e
        je     loc_1_9CE6
        or     eax, eax
        je     loc_1_9CE6
        add    edi, 2
        jmp    loc_1_9CD0
loc_1_9CE6:
        or     eax, 1
loc_1_9CE9:
        pop    edi
        pop    esi
        pop    edx
        pop    ebx
        pop    eax
        ret    
sub_1_9CEF:
        push   edx
        mov    eax, dword ptr [ebx]
        mov    dword ptr [esi + 0x96], eax
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [esi + 0x9a], eax
        mov    eax, dword ptr [ebx + 8]
        mov    byte ptr [esi + 0x9e], al
        shr    eax, 8
        mov    word ptr [esi + 0x9f], ax
        cmp    byte ptr [esi + 0x96], 5
        jne    loc_1_9D24
        mov    byte ptr [esi + 0x96], 0xe5
loc_1_9D24:
        mov    eax, 0x200020
        mov    dword ptr [esi + 0x66], eax
        mov    dword ptr [esi + 0x6a], eax
        mov    dword ptr [esi + 0x6e], eax
        mov    dword ptr [esi + 0x72], eax
        mov    dword ptr [esi + 0x76], eax
        mov    word ptr [esi + 0x7a], ax
        lea    eax, [esi + 0x96]
        lea    edx, [esi + 0x66]
        push   1
        push   8
        push   eax
        push   edx
        VxDCall  IFSMgr.BCSToUni                 ; cd2042004000
        add    esp, 0x10
        lea    eax, [esi + 0x9e]
        lea    edx, [esi + 0x76]
        push   1
        push   3
        push   eax
        push   edx
        VxDCall  IFSMgr.BCSToUni                 ; cd2042004000
        add    esp, 0x10
        lea    eax, [esi + 0x66]
        lea    edx, [esi + 0x96]
        push   0
        push   eax
        push   edx
        VxDCall  IFSMgr.FcbToShort               ; cd2048004000
        add    esp, 0xc
        mov    word ptr [eax + esi + 0x96], 0
        add    eax, 2
        mov    word ptr [esi + 0x94], ax
        pop    edx
        ret    
sub_1_9D98:
        push   ecx
        push   edx
        mov    edi, dword ptr [ebp + 0x20]
        mov    edi, dword ptr [edi + 8]
        sub    ecx, edx
        je     loc_1_9DC0
loc_1_9DA4:
        push   eax
        push   dword ptr [ebp + 0x20]
        push   dword ptr [ebp + 8]
        push   ecx
        push   edx
        push   dword ptr [edi + 0x64]
        VxDCall  IFSMgr.IFSMgr_CheckLocks        ; cd2056004000
        add    esp, 0x18
loc_1_9DB9:
        cmp    eax, 1
        cmc    
        pop    edx
        pop    ecx
        ret    
loc_1_9DC0:
        mov    ecx, dword ptr [edi + 0x30]
        sub    ecx, edx
        ja     loc_1_9DA4
        jne    loc_1_9DCD
        sub    eax, eax
        jmp    loc_1_9DB9
loc_1_9DCD:
        mov    edx, dword ptr [edi + 0x30]
        neg    ecx
        jmp    loc_1_9DA4
sub_1_9DD4:
        mov    eax, 0x2c
        call   sub_1_A1CE
        xchg   edi, eax
        mov    al, 8
        jb     loc_1_9DEF
        xor    eax, eax
        mov    ecx, 0xb
        rep stosd dword ptr es:[edi], eax
        lea    eax, [edi - 0x2c]
loc_1_9DEF:
        ret    
sub_1_9DF0:
        push   ecx
        push   edx
        VxDCall  IFSMgr.IFSMgr_Get_DOSTime       ; cd2007004000
        cld    
        pop    edx
        pop    ecx
        ret    
sub_1_9DFC:
        pushfd 
        test   byte ptr [ebp + 0x37], 0x90
        jne    loc_1_9E8D
        test   byte ptr [ebp + 0x37], 1
        jne    loc_1_9E9F
        cmp    dword ptr [ebp], 0
        je     loc_1_9E8D
        popfd  
        mov    eax, dword ptr [ebp]
        mov    esi, 0x200
        mov    edx, 0x40000000
        jb     loc_1_9E32
        mov    esi, 0x1000
        or     edx, 0x80000000
loc_1_9E32:
        cmp    eax, esi
        jb     loc_1_9E73
loc_1_9E36:
        mov    esi, dword ptr [ebp + 0x14]
        add    eax, esi
        dec    eax
        shr    esi, 0xc
        shr    eax, 0xc
        sub    eax, esi
        inc    eax
        mov    dword ptr [ebp + 0x54], eax
        push   edx
        push   eax
        push   esi
        VMMCall  VMM._LinPageLock                ; cd2063000100
        add    esp, 0xc
        cld    
        and    eax, eax
        je     loc_1_9E84
        mov    edx, dword ptr [ebp + 0x14]
        and    edx, 0xfff
        or     eax, edx
        mov    dword ptr [ebp + 0x14], eax
        shr    eax, 0xc
        mov    dword ptr [ebp + 0x50], eax
        or     word ptr [ebp + 0x38], 0x80
loc_1_9E73:
        mov    eax, dword ptr [ebp + 0x1c]
        test   word ptr [eax + 0x52], 0x20
        jne    loc_1_9E7F
        ret    
loc_1_9E7F:
        or     byte ptr [ebp + 0x3a], 2
        ret    
loc_1_9E84:
        or     byte ptr [ebp + 0x3a], 2
        and    byte ptr [ebp + 0x37], 0xfe
        ret    
loc_1_9E8D:
        popfd  
        mov    eax, dword ptr [ebp + 0x1c]
        test   word ptr [eax + 0x52], 0x20
        jne    loc_1_9E9A
        ret    
loc_1_9E9A:
        or     byte ptr [ebp + 0x3a], 2
        ret    
loc_1_9E9F:
        popfd  
        mov    eax, dword ptr [ebp]
        mov    edx, 0x40000000
        jb     loc_1_9E36
        or     edx, 0x80000000
        jmp    loc_1_9E36
sub_1_9EB2:
        test   word ptr [ebp + 0x38], 0x80
        je     loc_1_9ED7
        mov    esi, dword ptr [ebp + 0x50]
        mov    eax, dword ptr [ebp + 0x54]
        push   0x40000000
        push   eax
        push   esi
        VMMCall  VMM._LinPageUnLock              ; cd2064000100
        add    esp, 0xc
        cld    
        xor    word ptr [ebp + 0x38], 0x80
loc_1_9ED7:
        ret    
sub_1_9ED8:
        mov    ax, 0x2020
        mov    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    cl, 8
loc_1_9EE5:
        dec    edi
        cmp    byte ptr [edi], al
        loope  loc_1_9EE5
        inc    edi
        cmp    word ptr [esi], ax
        jne    loc_1_9EF5
        cmp    byte ptr [esi + 2], al
        je     loc_1_9F01
loc_1_9EF5:
        mov    al, 0x2e
        stosb  byte ptr es:[edi], al
        movsb  byte ptr es:[edi], byte ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
loc_1_9EFB:
        dec    edi
        cmp    byte ptr [edi], ah
        je     loc_1_9EFB
        inc    edi
loc_1_9F01:
        xor    eax, eax
        stosb  byte ptr es:[edi], al
        ret    
sub_1_9F05:
        call   sub_1_9DF0
        ror    eax, 0x10
        mov    edx, esi
        mov    esi, dword ptr [esi + 8]
        mov    ecx, dword ptr [esi + 0x28]
        test   word ptr [ecx + 0x52], 0x100
        jne    loc_1_9F3D
        test   word ptr [ebp + 0x18], 1
        jne    loc_1_9F3D
        test   dword ptr [edx + 0xc], 0x200004
        jne    loc_1_9F3D
        cmp    ax, word ptr [esi + 0x3c]
        je     loc_1_9F3D
        mov    word ptr [esi + 0x3c], ax
        and    word ptr [edx + 0xe], 0xfffb
loc_1_9F3D:
        test   word ptr [edx + 0xe], 0x40
        jne    loc_1_9F5B
        ror    eax, 0x10
        cmp    eax, dword ptr [esi + 0x3e]
        jne    loc_1_9F53
        cmp    byte ptr [esi + 0x42], 0
        jne    loc_1_9F5B
loc_1_9F53:
        mov    dword ptr [esi + 0x38], eax
        and    word ptr [edx + 0xe], 0xfffb
loc_1_9F5B:
        ret    
sub_1_9F5C:
        cmp    dword ptr [edi + 0x2c], 0
        jne    loc_1_9F84
        mov    eax, dword ptr [edi + 8]
        mov    ebx, dword ptr [edi + 0xc]
        mov    dword ptr [eax + 0xc], ebx
        mov    dword ptr [ebx + 8], eax
        cmp    dword ptr [0xbd70], 0         ; -> obj1:0xBD70
        jne    loc_1_9F83
        call   sub_1_9F85
        xchg   edi, eax
        call   sub_1_A1F8
        xchg   edi, eax
loc_1_9F83:
        ret    
loc_1_9F84:
        ret    
sub_1_9F85:
        mov    eax, dword ptr [edi + 0x54]
        or     eax, eax
        je     loc_1_9FA8
        test   byte ptr [edi + 0x24], 1
        jne    loc_1_9F98
        call   sub_1_A1F8
        ret    
loc_1_9F98:
        push   ecx
        push   edx
        push   0
        push   eax
        VMMCall  VMM._PageFree                   ; cd2055000100
        add    esp, 8
        pop    edx
        pop    ecx
loc_1_9FA8:
        ret    
sub_1_9FA9:
        cmp    dword ptr [edi + 0x2c], 0
        jne    loc_1_9FD1
sub_1_9FAF:
        mov    eax, dword ptr [edi + 8]
        mov    ebx, dword ptr [edi + 0xc]
        mov    dword ptr [eax + 0xc], ebx
        mov    dword ptr [ebx + 8], eax
        cmp    dword ptr [0xbd70], 0         ; -> obj1:0xBD70
        jne    loc_1_9FD0
        call   sub_1_9F85
        xchg   edi, eax
        call   sub_1_A1F8
        xchg   edi, eax
loc_1_9FD0:
        ret    
loc_1_9FD1:
        ret    
loc_1_9FD2:
        push   eax
        lea    eax, [edi + 0x60]
        test   dword ptr [edi + 0x20], 0x100ffff
        jne    loc_1_9FE6
        push   eax
        call   sub_1_A258
        pop    eax
loc_1_9FE6:
        call   sub_1_A220
        pop    eax
sub_1_9FEC:
        test   byte ptr [edi + 0x23], 0x81
        jne    loc_1_9FD2
        cmp    word ptr [edi + 0x20], 0
        jne    loc_1_9FFA
        ret    
loc_1_9FFA:
        or     byte ptr [edi + 0x23], 0x80
        push   eax
        lea    eax, [edi + 0x60]
        call   sub_1_A220
        pop    eax
        cmp    word ptr [edi + 0x20], 0
        jne    loc_1_9FFA
        test   byte ptr [edi + 0x23], 1
        jne    loc_1_9FFA
        and    byte ptr [edi + 0x23], 0x7f
        ret    
sub_1_A01A:
        and    byte ptr [edi + 0x23], 0xfe
        cmp    dword ptr [edi + 0x60], 0
        jne    loc_1_A025
        ret    
loc_1_A025:
        lea    eax, [edi + 0x60]
        call   sub_1_A258
        clc    
        ret    
sub_1_A02F:
        test   byte ptr [edi + 0x23], 0x81
        jne    loc_1_A036
        ret    
loc_1_A036:
        lea    eax, [edi + 0x60]
        call   sub_1_A220
        jmp    sub_1_A02F
sub_1_A040:
        dec    dword ptr [edi + 0x20]
        js     sub_1_A046
        ret    
sub_1_A046:
        test   byte ptr [edi + 0x23], 0x80
        je     loc_1_A05C
        cmp    word ptr [edi + 0x20], 0
        jne    loc_1_A05B
        lea    eax, [edi + 0x60]
        call   sub_1_A258
loc_1_A05B:
        clc    
loc_1_A05C:
        ret    
        db  000h, 000h, 000h
sub_1_A060:
        mov    esi, dword ptr [ebp + 0x20]
        mov    esi, dword ptr [esi + 8]
        cmp    dword ptr [esi + 0x50], 0
        je     loc_1_A1A5
        mov    edi, dword ptr [esi]
        mov    eax, 8
loc_1_A077:
        cmp    edi, esi
        je     loc_1_A083
        and    ax, word ptr [edi + 0xe]
        mov    edi, dword ptr [edi]
        jmp    loc_1_A077
loc_1_A083:
        cmp    eax, 8
        je     loc_1_A1A5
        sub    esp, 0x1c
        mov    edi, dword ptr [ebp + 0x1c]
        mov    eax, dword ptr [esi + 0x50]
        mov    dword ptr [esp + 4], eax
        mov    eax, dword ptr [esi + 0x54]
        mov    dword ptr [esp + 0xc], eax
        sub    eax, eax
        mov    dword ptr [esp + 0x10], eax
        mov    dword ptr [esp], eax
        mov    dword ptr [esp + 8], eax
        movzx  eax, byte ptr [ebp + 0x35]
        sub    edx, edx
        bts    edx, eax
loc_1_A0B6:
        mov    eax, dword ptr [esp + 0xc]
        mov    ebx, dword ptr [eax + 4]
        add    ebx, dword ptr [esp + 0x10]
        sub    ebx, 2
        mov    cl, byte ptr [edi + 0x25]
        shl    ebx, cl
        add    ebx, dword ptr [edi + 0x28]
        sub    ebx, dword ptr [edi + 0xc]
        movzx  eax, byte ptr [edi + 0x4e]
        not    eax
        and    ebx, eax
        add    ebx, dword ptr [edi + 0xc]
loc_1_A0DA:
        mov    ah, byte ptr [0xb6f8]         ; -> obj1:0xB6F8
        sub    al, al
        VxDCall  Dev_048B.Service_0004h          ; cd2004008b04
        jb     loc_1_A0FA
        sub    eax, eax
        cmp    al, byte ptr [esi + 0x32]
        sbb    eax, eax
        or     dword ptr [esp], eax
        and    eax, edx
        or     word ptr [esi + 0x26], ax
loc_1_A0FA:
        movzx  eax, byte ptr [edi + 0x4d]
        add    ebx, eax
        add    dword ptr [esp + 8], eax
        movzx  eax, byte ptr [edi + 0x24]
        cmp    dword ptr [esp + 8], eax
        jb     loc_1_A0DA
        and    dword ptr [esp + 8], 0
        mov    eax, dword ptr [esp + 0xc]
        inc    dword ptr [esp + 0x10]
        mov    ecx, dword ptr [eax]
        cmp    dword ptr [esp + 0x10], ecx
        jb     loc_1_A12F
        add    eax, 8
        mov    dword ptr [esp + 0xc], eax
        and    dword ptr [esp + 0x10], 0
loc_1_A12F:
        dec    dword ptr [esp + 4]
        jne    loc_1_A0B6
        movzx  ecx, byte ptr [edi + 0x4e]
        not    ecx
        movzx  eax, word ptr [edi + 0x26]
        sub    eax, dword ptr [edi + 0xc]
        and    eax, ecx
        add    eax, dword ptr [edi + 0xc]
        mov    dword ptr [esp + 0x14], eax
        mov    eax, dword ptr [edi + 0x18]
        cmp    byte ptr [edi + 0x46], 1
        je     loc_1_A157
        add    eax, dword ptr [edi + 0x30]
loc_1_A157:
        sub    eax, dword ptr [edi + 0xc]
        and    eax, ecx
        add    eax, dword ptr [edi + 0xc]
        mov    dword ptr [esp + 0x18], eax
        mov    esi, 0xb6fc                   ; -> obj1:0xB6FC
loc_1_A168:
        mov    esi, dword ptr [esi + 0x1c]
        cmp    esi, 0xb6fc                   ; -> obj1:0xB6FC
        je     loc_1_A1A2
        cmp    dword ptr [esi + 0xc], edi
        jne    loc_1_A168
        mov    eax, dword ptr [esi + 8]
        cmp    dword ptr [esp + 0x14], 0
        js     loc_1_A194
        cmp    eax, dword ptr [esp + 0x14]
        jb     loc_1_A168
        cmp    eax, dword ptr [esp + 0x18]
        jae    loc_1_A168
loc_1_A18E:
        or     word ptr [esi + 0x26], dx
        jmp    loc_1_A168
loc_1_A194:
        cmp    eax, dword ptr [esp + 0x14]
        je     loc_1_A18E
        cmp    eax, dword ptr [esp + 0x18]
        jae    loc_1_A168
        jmp    loc_1_A18E
loc_1_A1A2:
        add    esp, 0x1c
loc_1_A1A5:
        ret    
        db  000h, 000h, 051h, 0C1h, 0E9h, 002h, 033h, 0DBh, 0E3h, 007h, 0ADh, 0D1h, 0C3h, 033h, 0D8h, 0E2h
        db  0F9h, 059h, 0F6h, 0C1h, 002h, 074h, 008h, 066h, 0ADh, 066h, 003h, 0D8h, 0C1h, 0C3h, 003h, 0D0h
        db  0E9h, 073h, 003h, 0ACh, 002h, 0D8h, 093h, 0C3h
sub_1_A1CE:
        push   edx
        push   1
        push   eax
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        pop    edx
        cmp    eax, 1
        ret    
sub_1_A1E0:
        push   edx
        push   0x101
        push   eax
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        pop    edx
        cmp    eax, 1
        ret    
        db  02Eh, 08Bh, 0C0h
sub_1_A1F8:
        push   ecx
        push   edx
        push   0
        push   eax
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
        pop    edx
        pop    ecx
        ret    
        db  000h, 000h, 000h
sub_1_A20C:
        cli    
        test   dword ptr [eax], ecx
        jne    loc_1_A213
        sti    
        ret    
loc_1_A213:
        add    eax, 4
        call   sub_1_A220
        sub    eax, 4
        jmp    sub_1_A20C
sub_1_A220:
        cli    
        inc    word ptr [eax]
        push   edx
        push   ecx
        push   eax
        test   byte ptr [ebp + 0x37], 0x90
        je     loc_1_A24C
        mov    ecx, dword ptr [ebp + 0x1c]
        mov    ecx, dword ptr [ecx + 0x14]
        test   dword ptr [ecx + 4], 0x80
        jne    loc_1_A24C
        test   dword ptr [ecx], 0x400000
        jne    loc_1_A24C
        VxDCall  IFSMgr.IFSMgr_BlockNoEvents     ; cd2036004000
        jmp    loc_1_A252
loc_1_A24C:
        VxDCall  IFSMgr.IFSMgr_Block             ; cd2012004000
loc_1_A252:
        pop    eax
        pop    ecx
        pop    edx
        ret    
        db  08Bh, 0C0h
sub_1_A258:
        push   ecx
        push   edx
        mov    dword ptr [eax], 0
        push   eax
        VxDCall  IFSMgr.IFSMgr_Wakeup            ; cd2013004000
        pop    eax
        pop    edx
        pop    ecx
        ret    
sub_1_A26B:
        push   ebx
loc_1_A26C:
        push   edi
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        mov    ebx, edi
        pop    edi
        cld    
        cmp    dword ptr [0xb8ac], 0         ; -> obj1:0xB8AC
        jne    loc_1_A2A9
        or     byte ptr [0xb8af], 0x80       ; -> obj1:0xB8AF
        movzx  ebx, word ptr [ebx + 0x18]
        mov    dword ptr [0xb8b4], ebx       ; -> obj1:0xB8B4
        push   edi
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        add    edi, dword ptr [0xbd8c]       ; -> obj1:0xBD8C
        inc    dword ptr [edi]
        pop    edi
        pop    ebx
        inc    dword ptr [0xb8b0]            ; -> obj1:0xB8B0
        ret    
loc_1_A2A9:
        or     byte ptr [0xb8af], 0x80       ; -> obj1:0xB8AF
        push   eax
        mov    eax, 0xb8b8                   ; -> obj1:0xB8B8
        call   sub_1_A220
        pop    eax
        cmp    dword ptr [0xb8b0], 0         ; -> obj1:0xB8B0
        jne    loc_1_A2A9
        mov    byte ptr [0xb8af], 0          ; -> obj1:0xB8AF
        jmp    loc_1_A26C
sub_1_A2CE:
        push   edi
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        add    edi, dword ptr [0xbd8c]       ; -> obj1:0xBD8C
        dec    dword ptr [edi]
        pop    edi
        dec    dword ptr [0xb8b0]            ; -> obj1:0xB8B0
        cmp    dword ptr [0xb8b8], 0         ; -> obj1:0xB8B8
        je     loc_1_A2FA
        push   eax
        mov    eax, 0xb8b8                   ; -> obj1:0xB8B8
        call   sub_1_A258
        pop    eax
        ret    
loc_1_A2FA:
        mov    byte ptr [0xb8af], 0          ; -> obj1:0xB8AF
        cmp    dword ptr [0xb8bc], 0         ; -> obj1:0xB8BC
        je     loc_1_A316
        push   eax
        mov    eax, 0xb8bc                   ; -> obj1:0xB8BC
        call   sub_1_A258
        pop    eax
loc_1_A316:
        ret    
        db  090h
sub_1_A318:
        inc    dword ptr [0xb8ac]            ; -> obj1:0xB8AC
        js     loc_1_A334
loc_1_A320:
        push   edi
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        add    edi, dword ptr [0xbd8c]       ; -> obj1:0xBD8C
loc_1_A32D:
        inc    dword ptr [edi]
        pop    edi
        ret    
        db  02Eh, 08Bh, 0C0h
loc_1_A334:
        test   byte ptr [ebp + 0x37], 0x90
        jne    loc_1_A320
        push   edi
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        add    edi, dword ptr [0xbd8c]       ; -> obj1:0xBD8C
        cmp    dword ptr [edi], 0
        jne    loc_1_A32D
        pop    edi
        dec    dword ptr [0xb8ac]            ; -> obj1:0xB8AC
        push   eax
        mov    eax, 0xb8bc                   ; -> obj1:0xB8BC
        call   sub_1_A220
        pop    eax
        jmp    sub_1_A318
sub_1_A361:
        push   edi
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        add    edi, dword ptr [0xbd8c]       ; -> obj1:0xBD8C
        dec    dword ptr [edi]
        pop    edi
        dec    dword ptr [0xb8ac]            ; -> obj1:0xB8AC
        jns    loc_1_A38F
        cmp    word ptr [0xb8ac], 0          ; -> obj1:0xB8AC
        jne    loc_1_A38F
        push   eax
        mov    eax, 0xb8b8                   ; -> obj1:0xB8B8
        call   sub_1_A258
        pop    eax
loc_1_A38F:
        ret    
loc_1_A390:
        mov    ebx, dword ptr [esp + 4]
        mov    word ptr [ebx + 2], 0
        ret    
        db  000h, 052h, 025h, 000h, 0FFh, 000h, 000h, 08Bh, 077h, 014h, 00Fh, 0B7h, 056h, 00Ah, 003h, 0DAh
        db  02Bh, 0D2h, 0EBh, 02Fh
sub_1_A3AF:
        push   edx
        sub    edx, edx
        and    eax, 0xff00
        jmp    loc_1_A3D4
sub_1_A3B9:
        push   edx
        sub    eax, eax
        mov    esi, dword ptr [edi + 0x14]
        bt     dword ptr [ebp + 0x38], 0xc
        sbb    edx, edx
        and    edx, 0x2000000
        test   byte ptr [ebp + 0x39], 5
        jne    loc_1_A41B
        mov    ah, 2
loc_1_A3D4:
        mov    esi, dword ptr [edi + 0x14]
loc_1_A3D7:
        movzx  ebx, word ptr [esi + 0xa]
        add    ebx, 0x40
        test   byte ptr [esi + 1], 8
        je     loc_1_A3ED
        push   eax
        movzx  eax, word ptr [esi + 8]
        lea    ebx, [ebx + eax*8]
        pop    eax
loc_1_A3ED:
        mov    esi, dword ptr [esi + 0xc]
        call   sub_1_A4E1
        jb     loc_1_A419
        lea    eax, [ebx + 0x94]
        mov    dword ptr [ebx + 0x48], eax
        sub    eax, eax
        mov    dword ptr [ebx], eax
        mov    dword ptr [ebx + 4], eax
        mov    dword ptr [ebx + 0x14], eax
        mov    dword ptr [ebx + 0x18], eax
        mov    byte ptr [ebx + 0x4c], al
        or     dx, 0x401
        mov    dword ptr [ebx + 8], edx
        clc    
loc_1_A419:
        pop    edx
        ret    
loc_1_A41B:
        or     edx, 0x80808
        test   byte ptr [ebp + 0x39], 1
        jne    loc_1_A42D
        and    edx, 0xfffff7ff
loc_1_A42D:
        test   byte ptr [esi + 1], 0x40
        jne    loc_1_A3D7
        and    edx, 0xfff7ffff
        jmp    loc_1_A3D7
sub_1_A43B:
        push   ecx
        push   edx
        push   esi
        push   edi
        push   0
        test   byte ptr [ebp + 0x37], 0x90
        jne    loc_1_A45E
        call   sub_1_A3B9
        bts    dword ptr [esp], 0
loc_1_A451:
        sub    eax, eax
loc_1_A453:
        bt     dword ptr [esp], 0
        pop    ecx
        pop    edi
        pop    esi
        pop    edx
        pop    ecx
        ret    
loc_1_A45E:
        mov    ah, 1
        call   sub_1_A3AF
        jae    loc_1_A451
        bts    dword ptr [0xb8f4], 0         ; -> loc_1_B8F4
        jb     loc_1_A495
        mov    ebx, dword ptr [0xb8f8]       ; -> loc_1_B8F8
        mov    eax, dword ptr [0xb8fc]       ; -> loc_1_B8FC
        mov    dword ptr [ebx], eax
        add    eax, 0x14
        mov    dword ptr [ebx + 0x14], eax
        mov    ah, 4
        lea    ecx, [ebx + 0x14]
        call   sub_1_A3AF
        mov    eax, 1
        jmp    loc_1_A453
loc_1_A495:
        bts    dword ptr [esp], 0
        bts    dword ptr [0xb8f4], 0x1f      ; -> loc_1_B8F4
        mov    eax, 0xb900                   ; -> loc_1_B900
        call   sub_1_A220
        jmp    loc_1_A45E
sub_1_A4AE:
        pushfd 
        sub    ebx, dword ptr [ebx + 0x20]
        sub    esp, 8
        mov    esi, esp
        mov    dword ptr [esi + 4], ebx
        mov    word ptr [esi], 4
        push   esi
        call   sub_1_B8C0
        add    esp, 0xc
        btr    dword ptr [0xb8f4], 0x1f      ; -> loc_1_B8F4
        jae    loc_1_A4DF
        push   eax
        mov    eax, 0xb900                   ; -> loc_1_B900
        call   sub_1_A258
        pop    eax
loc_1_A4DF:
        popfd  
        ret    
sub_1_A4E1:
        sub    esp, 0x10
        mov    word ptr [esp + 4], bx
        mov    dword ptr [esp + 6], esi
        mov    byte ptr [esp + 0xe], ah
        mov    word ptr [esp], 2
        mov    dword ptr [esp + 0xa], ecx
        mov    ebx, esp
        push   ebx
        call   sub_1_B8C0
        mov    ebx, dword ptr [ebx + 0xa]
        add    esp, 0x14
        cmp    ebx, 1
        jb     loc_1_A515
        lea    ebx, [esi + ebx]
        mov    dword ptr [ebx + 0x20], esi
loc_1_A515:
        ret    
sub_1_A516:
        push   edx
        sub    esp, 0x10
        mov    edx, esp
        mov    word ptr [edx + 4], ax
        mov    word ptr [edx + 0xa], 9
        mov    word ptr [edx], 3
        mov    byte ptr [edx + 0xc], 8
        push   edx
        call   sub_1_B8C0
        mov    eax, dword ptr [edx + 6]
        add    esp, 0x14
        pop    edx
        ret    
sub_1_A53E:
        push   edx
        sub    esp, 8
        mov    edx, esp
        mov    dword ptr [edx + 4], eax
        mov    word ptr [edx], 4
        push   edx
        call   sub_1_B8C0
        add    esp, 0xc
        pop    edx
        ret    
sub_1_A558:
        push   edi
        push   ebx
        mov    ebx, dword ptr [edi + 0x14]
        mov    dword ptr [eax + 0x44], ebx
        push   eax
        call   sub_1_B8D0
        or     eax, eax
        pop    eax
        pop    ebx
        pop    edi
        jne    loc_1_A56F
        ret    
loc_1_A56F:
        or     dword ptr [eax + 8], 0x40
        ret    
sub_1_A574:
        push   ebx
        push   ecx
        push   edx
        push   esi
        xor    eax, eax
        mov    ebx, 0x5e
        mov    ecx, 5
        add    esi, 3
loc_1_A587:
        mul    ebx
        movzx  edx, byte ptr [esi]
        inc    esi
        sub    dl, 0x21
        add    eax, edx
        loop   loc_1_A587
        pop    esi
        pop    edx
        pop    ecx
        pop    ebx
        ret    
sub_1_A599:
        push   ebx
        push   ecx
        push   edx
        mov    ebx, 0x5e
        mov    ecx, 5
loc_1_A5A6:
        xor    edx, edx
        div    ebx
        add    dl, 0x21
        mov    byte ptr [ecx + esi + 2], dl
        loop   loc_1_A5A6
        pop    edx
        pop    ecx
        pop    ebx
        ret    
sub_1_A5B7:
        push   0
        push   0
        push   0
        or     byte ptr [esp], 2
        mov    ebx, dword ptr [ebp + 0x1c]
        test   byte ptr [ebx + 0x53], 0x10
        jne    loc_1_A629
        mov    eax, dword ptr [esi + 8]
        and    eax, 0xffffff
        cmp    eax, 0x434849
        je     loc_1_A629
        call   sub_1_A7BE
        je     loc_1_A629
        and    byte ptr [esp], 0xfd
        mov    eax, dword ptr [ebx + 0x14]
        test   byte ptr [eax + 4], 0x40
        jne    loc_1_A632
        call   sub_1_9DF0
        mov    edx, eax
        VMMCall  VMM.Get_System_Time             ; cd203f000100
        xor    eax, edx
        call   sub_1_A599
        and    dword ptr [esi + 8], 0xff000000
        or     dword ptr [esi + 8], 0x434849
        push   esi
        mov    edi, ebx
        xor    ebx, ebx
        mov    ecx, 1
        call   sub_1_1574
        pop    esi
        jb     loc_1_A707
        or     byte ptr [esp], 4
loc_1_A629:
        call   sub_1_A574
        mov    dword ptr [esp + 8], eax
loc_1_A632:
        mov    ebx, dword ptr [ebp + 0x1c]
        movzx  ecx, word ptr [ebx + 0x44]
        mov    edi, dword ptr [ebx + 0x34]
        test   byte ptr [ebx + 0x52], 3
        jne    loc_1_A65E
        mov    edi, dword ptr [ebx + 0x3c]
        sub    edi, 2
        jb     loc_1_A707
        mov    cl, byte ptr [ebx + 0x25]
        shl    edi, cl
        add    edi, dword ptr [ebx + 0x28]
        movzx  ecx, word ptr [ebx + 0x4a]
        inc    ecx
        shr    ecx, 5
loc_1_A65E:
        mov    ebx, dword ptr [ebp + 0x1c]
        push   ecx
        push   edi
        push   esi
        mov    ecx, 1
        call   sub_1_1458
        pop    esi
        pop    edi
        pop    ecx
        jb     loc_1_A707
        inc    edi
        mov    edx, dword ptr [esp + 4]
        mov    ebx, dword ptr [ebp + 0x1c]
        push   ecx
        push   esi
        movzx  ecx, word ptr [ebx + 0x20]
        shr    ecx, 2
loc_1_A688:
        ror    edx, 5
        lodsd  eax, dword ptr [esi]
        xor    edx, eax
        loop   loc_1_A688
        pop    esi
        pop    ecx
        mov    dword ptr [esp + 4], edx
        movzx  edx, word ptr [ebx + 0x20]
        mov    eax, esi
loc_1_A69C:
        mov    bl, byte ptr [eax]
        cmp    bl, 0xe5
        je     loc_1_A6F7
        or     bl, bl
        je     loc_1_A707
        mov    bh, byte ptr [eax + 0xb]
        test   bh, 8
        je     loc_1_A6F7
        and    bh, 7
        cmp    bh, 7
        je     loc_1_A6F7
        bts    dword ptr [esp], 0
        jb     loc_1_A6F7
        push   ecx
        push   edi
        push   eax
        mov    ecx, dword ptr [ebp + 0x1c]
        lea    edi, [ecx + 0xae]
        xchg   esi, eax
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
        push   eax
        lea    edi, [edi - 0xb]
        call   sub_1_6EC3
        mov    byte ptr [ecx + 0xb9], al
        pop    eax
        cmp    bl, 5
        jne    loc_1_A6EC
        mov    byte ptr [ecx + 0xae], 0xe5
loc_1_A6EC:
        mov    esi, eax
        pop    eax
        pop    edi
        pop    ecx
        test   byte ptr [esp], 2
        jne    loc_1_A707
loc_1_A6F7:
        add    eax, 0x20
        sub    edx, 0x20
        loopne loc_1_A69C
        or     ecx, ecx
        jne    loc_1_A65E
loc_1_A707:
        mov    ebx, dword ptr [ebp + 0x1c]
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [ebx + 0x58], eax
        test   byte ptr [esp], 6
        je     loc_1_A724
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [ebx + 0x5c], eax
        or     word ptr [ebx + 0x52], 0x200
loc_1_A724:
        bt     dword ptr [esp], 0
        cmc    
        lea    esp, [esp + 0xc]
        ret    
sub_1_A72F:
        pushal 
        mov    ecx, dword ptr [ebx + 0x14]
loc_1_A733:
        mov    ebx, 0xb748                   ; -> loc_1_B748
loc_1_A738:
        sti    
        mov    ebx, dword ptr [ebx]
        cmp    ebx, 0xb748                   ; -> loc_1_B748
        je     loc_1_A77B
        cmp    dword ptr [ebx + 0x14], ecx
        jne    loc_1_A738
        cmp    dword ptr [0xbd70], 0         ; -> obj1:0xBD70
        jne    loc_1_A767
        push   ebx
        push   ecx
        xor    eax, eax
        call   sub_1_0979
        xor    eax, eax
        push   eax
        push   eax
        push   eax
        push   eax
        call   sub_1_0AF3
        pop    ecx
        pop    ebx
loc_1_A767:
        cli    
        cmp    dword ptr [ebx + 0x64], 0
        je     loc_1_A738
        lea    eax, [ebx + 0x8c]
        call   sub_1_A220
        jmp    loc_1_A733
loc_1_A77B:
        mov    edx, 0xa78e                   ; -> loc_1_A78E
        mov    ah, byte ptr [0xb6f8]         ; -> obj1:0xB6F8
        VxDCall  Dev_048B.Service_0009h          ; cd2009008b04
        popal  
        ret    
loc_1_A78E:
        mov    edx, dword ptr [esi + 0xc]
        cmp    dword ptr [edx + 0x14], ecx
        je     loc_1_A797
loc_1_A796:
        ret    
loc_1_A797:
        cmp    byte ptr [esi + 0x32], 0
        jne    loc_1_A796
        test   byte ptr [esi + 0x2b], 2
        jne    loc_1_A796
        VxDCall  Dev_048B.Service_000Dh          ; cd200d008b04
        or     eax, eax
        jne    loc_1_A796
        call   0
        push   ebx
        push   ecx
        VxDCall  Dev_048B.Service_0005h          ; cd2005008b04
        pop    ecx
        pop    ebx
        jmp    loc_1_A796
sub_1_A7BE:
        cld    
        push   ebx
        push   ecx
        push   edx
        push   esi
        push   edi
        mov    ebx, dword ptr [0xb894]       ; -> obj1:0xB894
        or     ebx, ebx
        je     loc_1_A7EF
loc_1_A7CE:
        mov    ecx, dword ptr [ebx]
        jecxz  loc_1_A7EF
        lea    edx, [ebx + 4]
loc_1_A7D5:
        push   ecx
        push   esi
        movzx  eax, word ptr [edx + 2]
        add    esi, eax
        lea    edi, [edx + 4]
        movzx  ecx, word ptr [edx]
        lea    edx, [ecx + edx + 4]
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        pop    esi
        pop    ecx
        je     loc_1_A7FF
        loop   loc_1_A7D5
loc_1_A7EF:
        cmp    ebx, 0xb904                   ; -> loc_1_B904
        mov    ebx, 0xb904                   ; -> loc_1_B904
        jne    loc_1_A7CE
        or     eax, 1
loc_1_A7FF:
        pop    edi
        pop    esi
        pop    edx
        pop    ecx
        pop    ebx
        ret    
        db  000h, 000h, 000h
loc_1_A808:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 0x14]
        cld    
        sub    eax, eax
        mov    byte ptr [ebp + 0x37], al
        mov    word ptr [ebp + 0x38], 0x2000
        mov    byte ptr [ebp + 0x34], al
        mov    byte ptr [ebp + 0x35], al
        mov    dword ptr [ebp + 0x6c], eax
        mov    dword ptr [ebp + 0x64], eax
        call   sub_1_A318
        mov    eax, 0xc000
        mov    edx, dword ptr [ebp]
        call   sub_1_43C9
        mov    ax, word ptr [ebp]
        and    al, 0x1e
        mov    word ptr [ebp + 0x45], ax
        mov    eax, dword ptr [ebp + 0xc]
        call   sub_1_91B8
        jb     loc_1_A936
        cmp    ebx, -1
        je     loc_1_A93C
        mov    esi, dword ptr [ebp + 0x64]
        VxDCall  Dev_048B.Service_0007h          ; cd2007008b04
        mov    esi, dword ptr [ebp + 0x10]
        mov    dword ptr [esi], 0xa943       ; -> loc_1_A943
        mov    dword ptr [esi + 4], 0x4668   ; -> loc_1_4668
        mov    dword ptr [esi + 8], 0xb920   ; -> loc_1_B920
        mov    esi, dword ptr [ebp + 0xc]
        movzx  ecx, word ptr [esi]
        sub    cx, word ptr [esi + 2]
        lea    eax, [ecx + 0x1b]
        push   ecx
        push   esi
        call   sub_1_A1CE
        pop    esi
        pop    ecx
        mov    edi, eax
        mov    al, 8
        or     edi, edi
        je     loc_1_A936
        movzx  eax, word ptr [esi + 2]
        lea    esi, [eax + esi]
        mov    edx, edi
        lea    edi, [edi + 0x17]
        rep movsb byte ptr es:[edi], byte ptr [esi]
        sub    eax, eax
        stosw  word ptr es:[edi], ax
        mov    eax, dword ptr [ebp + 0x1c]
        mov    dword ptr [edx + 8], eax
        mov    eax, dword ptr [ebp + 0x50]
        mov    dword ptr [edx + 0xc], eax
        mov    eax, dword ptr [ebp + 0x4c]
        mov    dword ptr [edx + 0x10], eax
        mov    ax, word ptr [ebp + 0x45]
        mov    word ptr [edx + 0x14], ax
        sub    eax, eax
        test   word ptr [ebp + 0x42], 8
        setne  al
        shl    al, 3
        or     ah, al
        test   word ptr [ebp + 0x42], 0x20
        setne  al
        shl    al, 2
        or     ah, al
        test   word ptr [ebp + 0x42], 1
        setne  al
        shl    al, 0
        or     ah, al
        mov    byte ptr [edx + 0x16], ah
        mov    dword ptr [ebp + 0x20], edx
        mov    eax, dword ptr [ebp + 0x50]
        mov    dword ptr [edx + 0xc], eax
        call   sub_1_AA38
        mov    edx, dword ptr [ebp + 0x20]
        jb     loc_1_A921
        mov    ecx, 0xb918                   ; -> loc_1_B918
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [eax], edx
        mov    dword ptr [edx], ecx
        mov    dword ptr [edx + 4], eax
        mov    dword ptr [ecx + 4], edx
        xor    eax, eax
        jmp    loc_1_A928
loc_1_A921:
        xchg   edx, eax
        call   sub_1_A1F8
        xchg   edx, eax
loc_1_A928:
        mov    esi, dword ptr [ebp + 0x64]
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
        jmp    loc_1_454A
loc_1_A936:
        jne    loc_1_454A
loc_1_A93C:
        mov    al, 2
        jmp    loc_1_454A
loc_1_A943:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 0x14]
        cld    
        sub    eax, eax
        mov    byte ptr [ebp + 0x37], al
        mov    word ptr [ebp + 0x38], 0x2000
        mov    byte ptr [ebp + 0x34], al
        mov    byte ptr [ebp + 0x35], al
        mov    dword ptr [ebp + 0x6c], eax
        mov    dword ptr [ebp + 0x64], eax
        call   sub_1_A318
        mov    ebx, dword ptr [ebp + 0x20]
        cmp    dword ptr [ebx + 8], 0
        je     loc_1_A9FB
        test   word ptr [ebp + 0x18], 0x40
        jne    loc_1_A983
        test   byte ptr [ebx + 0x16], 1
        je     loc_1_A9FB
loc_1_A983:
        mov    eax, 0x10
        test   byte ptr [ebx + 0x16], 1
        je     loc_1_A991
        or     eax, 1
loc_1_A991:
        test   byte ptr [ebx + 0x16], 4
        je     loc_1_A99A
        or     eax, 0x20
loc_1_A99A:
        test   byte ptr [ebx + 0x16], 8
        je     loc_1_A9A3
        or     eax, 8
loc_1_A9A3:
        mov    word ptr [ebp + 0x42], ax
        mov    ax, word ptr [ebx + 0x14]
        mov    word ptr [ebp + 0x45], ax
        mov    edx, dword ptr [ebx + 0x10]
        inc    edx
        test   word ptr [ebp + 0x18], 0x40
        je     loc_1_A9C3
        mov    edx, dword ptr [ebp + 0x24]
        cmp    edx, dword ptr [ebx + 0x10]
        ja     loc_1_A9FB
loc_1_A9C3:
        lea    eax, [ebx + 0x17]
        mov    ebx, dword ptr [ebx + 0xc]
        mov    ecx, 2
        call   sub_1_9366
        jb     loc_1_A9F3
        mov    esi, dword ptr [ebp + 0x64]
        VxDCall  Dev_048B.Service_0007h          ; cd2007008b04
        call   sub_1_AA38
        push   eax
        mov    esi, dword ptr [ebp + 0x64]
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
        pop    eax
        jmp    loc_1_454A
loc_1_A9F3:
        or     eax, eax
        jne    loc_1_454A
loc_1_A9FB:
        mov    al, 0x12
        jmp    loc_1_454A
loc_1_AA02:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 0x14]
        cld    
        sub    eax, eax
        mov    byte ptr [ebp + 0x37], al
        mov    word ptr [ebp + 0x38], ax
        mov    byte ptr [ebp + 0x34], al
        mov    byte ptr [ebp + 0x35], al
        call   sub_1_A318
        mov    ebx, dword ptr [ebp + 0x20]
        mov    eax, dword ptr [ebx]
        mov    edx, dword ptr [ebx + 4]
        mov    dword ptr [eax + 4], edx
        mov    dword ptr [edx], eax
        xchg   ebx, eax
        call   sub_1_A1F8
        xchg   ebx, eax
        jmp    loc_1_4574
sub_1_AA38:
        mov    edx, dword ptr [ebp + 0x20]
        mov    eax, dword ptr [ebp + 0x50]
        cmp    dword ptr [edx + 0xc], eax
        jne    loc_1_AB56
        mov    eax, dword ptr [ebp + 0x4c]
        cmp    eax, dword ptr [edx + 0x10]
        jb     loc_1_AB56
        mov    dword ptr [edx + 0x10], eax
        mov    edx, dword ptr [ebp + 0x6c]
        test   byte ptr [edx + 7], 2
        je     loc_1_AA62
        mov    eax, dword ptr [edx + 0xc]
loc_1_AA62:
        mov    dword ptr [ebp + 0x24], eax
        mov    edx, dword ptr [ebp + 0x14]
        movzx  eax, byte ptr [ebx + 0xb]
        and    al, 0x3f
        mov    dword ptr [edx], eax
        mov    dword ptr [edx + 0x1c], 0
        mov    eax, dword ptr [ebx + 0x1c]
        mov    dword ptr [edx + 0x20], eax
        mov    eax, dword ptr [ebx + 0xe]
        push   edx
        push   eax
        VxDCall  IFSMgr.IFSMgr_DosToWin32Time    ; cd2039004000
        add    esp, 4
        mov    ecx, edx
        pop    edx
        mov    dword ptr [edx + 4], eax
        mov    dword ptr [edx + 8], ecx
        push   edx
        movzx  eax, byte ptr [ebx + 0xd]
        mov    edx, 0x186a0
        mul    edx
        pop    edx
        add    dword ptr [edx + 4], eax
        adc    dword ptr [edx + 8], 0
        mov    ax, word ptr [ebx + 0x12]
        shl    eax, 0x10
        push   edx
        push   eax
        VxDCall  IFSMgr.IFSMgr_DosToWin32Time    ; cd2039004000
        add    esp, 4
        mov    ecx, edx
        pop    edx
        mov    dword ptr [edx + 0xc], eax
        mov    dword ptr [edx + 0x10], ecx
        mov    eax, dword ptr [ebx + 0x16]
        push   edx
        push   eax
        VxDCall  IFSMgr.IFSMgr_DosToWin32Time    ; cd2039004000
        add    esp, 4
        mov    ecx, edx
        pop    edx
        mov    dword ptr [edx + 0x14], eax
        mov    dword ptr [edx + 0x18], ecx
        sub    eax, eax
        mov    dword ptr [edx + 0x28], eax
        mov    dword ptr [edx + 0x24], eax
        mov    esi, dword ptr [ebp + 0x6c]
        mov    word ptr [edx + 0x2c], 0
        mov    word ptr [edx + 0x234], 0
        lea    edi, [edx + 0x2c]
        test   byte ptr [esi + 7], 2
        je     loc_1_AB24
        push   esi
        lea    esi, [esi + 0xb2]
        mov    ecx, 0x103
        sub    eax, eax
loc_1_AB0B:
        movsw  word ptr es:[edi], word ptr [esi]
        cmp    word ptr [edi - 2], ax
        loopne loc_1_AB0B
        pop    esi
        je     loc_1_AB18
        stosw  word ptr es:[edi], ax
loc_1_AB18:
        lea    edi, [edx + 0x234]
        test   byte ptr [esi + 7], 1
        je     loc_1_AB53
loc_1_AB24:
        movzx  ecx, word ptr [esi + 0x94]
        sub    ecx, 2
        jbe    loc_1_AB4F
        cmp    ecx, 0x18
        ja     loc_1_AB4F
        lea    eax, [esi + 0x96]
        mov    esi, eax
        rep movsb byte ptr es:[edi], byte ptr [esi]
        cmp    word ptr [edi - 2], 0x2e
        jne    loc_1_AB4F
        cmp    word ptr [eax], 0x2e
        je     loc_1_AB4F
        sub    edi, 2
loc_1_AB4F:
        sub    eax, eax
        stosw  word ptr es:[edi], ax
loc_1_AB53:
        xor    eax, eax
        ret    
loc_1_AB56:
        mov    edx, dword ptr [ebp + 0x20]
        mov    al, 0x12
        stc    
        ret    
sub_1_AB5D:
        mov    edx, 0xb918                   ; -> loc_1_B918
loc_1_AB62:
        mov    edx, dword ptr [edx]
        cmp    edx, 0xb918                   ; -> loc_1_B918
        je     loc_1_AB84
        cmp    dword ptr [edx + 8], ebx
        jne    loc_1_AB62
        cmp    eax, -1
        je     loc_1_AB7B
        cmp    dword ptr [edx + 0xc], eax
        jne    loc_1_AB62
loc_1_AB7B:
        mov    dword ptr [edx + 8], 0
        jmp    loc_1_AB62
loc_1_AB84:
        ret    
loc_1_AB85:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 0x14]
        cld    
        sub    eax, eax
        mov    byte ptr [ebp + 0x37], al
        mov    word ptr [ebp + 0x38], ax
        mov    byte ptr [ebp + 0x34], al
        mov    byte ptr [ebp + 0x35], al
        call   sub_1_A318
        cmp    word ptr [ebp + 0x18], 0
        je     loc_1_4574
        mov    al, 1
        cmp    word ptr [ebp + 0x18], 2
        jne    loc_1_4576
        mov    edi, dword ptr [ebp + 0x1c]
        mov    eax, dword ptr [ebp + 0x14]
        test   byte ptr [edi + 0x52], 3
        je     loc_1_ABD4
        cmp    dword ptr [ebp], 4
        jb     loc_1_ABE6
        mov    dword ptr [eax], 0x544146
        jmp    loc_1_ABE6
loc_1_ABD4:
        cmp    dword ptr [ebp], 6
        jb     loc_1_ABE6
        mov    dword ptr [eax], 0x33544146
        mov    word ptr [eax + 4], 0x32
loc_1_ABE6:
        mov    dword ptr [ebp], 0x10400ff
        mov    dx, 0x4006
        test   word ptr [edi + 0x52], 0x10
        je     loc_1_ABFE
        or     dx, 0x8000
loc_1_ABFE:
        mov    word ptr [ebp + 0x18], dx
        movzx  eax, word ptr [edi + 0x20]
        mov    dword ptr [ebp + 0x24], eax
        jmp    loc_1_4574
        db  08Bh, 0C0h
        ; ---- dispatch table (5 entries) ----
        dd  loc_1_AC61
        dd  loc_1_ACF7
        dd  loc_1_AD22
        dd  loc_1_AD5B
        dd  loc_1_ADE0
loc_1_AC24:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 0x14]
        cld    
        mov    ax, word ptr [ebp + 0x18]
        and    ah, 0x91
        mov    byte ptr [ebp + 0x37], ah
        sub    eax, eax
        mov    word ptr [ebp + 0x38], ax
        mov    byte ptr [ebp + 0x34], al
        mov    byte ptr [ebp + 0x35], al
        call   sub_1_A318
        movzx  eax, byte ptr [ebp + 4]
        cmp    eax, 5
        jae    loc_1_AC5A
        jmp    dword ptr [eax*4 + 0xac10]    ; -> obj1:0xAC10
loc_1_AC5A:
        mov    al, 1
        jmp    loc_1_4576
loc_1_AC61:
        mov    esi, dword ptr [ebp + 0x20]
        test   byte ptr [esi + 0xe], 0x10
        jne    loc_1_456D
        mov    esi, dword ptr [esi + 8]
        mov    edi, dword ptr [ebp + 0x14]
        movzx  eax, byte ptr [esi + 0x43]
        mov    dword ptr [edi], eax
        mov    eax, dword ptr [esi + 0x38]
        push   eax
        VxDCall  IFSMgr.IFSMgr_DosToWin32Time    ; cd2039004000
        add    esp, 4
        mov    dword ptr [edi + 0x14], eax
        mov    dword ptr [edi + 0x18], edx
        mov    eax, dword ptr [esi + 0x3e]
        push   eax
        VxDCall  IFSMgr.IFSMgr_DosToWin32Time    ; cd2039004000
        add    esp, 4
        mov    dword ptr [edi + 4], eax
        mov    dword ptr [edi + 8], edx
        movzx  eax, byte ptr [esi + 0x42]
        mov    edx, 0x186a0
        mul    edx
        add    dword ptr [edi + 4], eax
        adc    dword ptr [edi + 8], edx
        movzx  eax, word ptr [esi + 0x3c]
        shl    eax, 0x10
        push   eax
        VxDCall  IFSMgr.IFSMgr_DosToWin32Time    ; cd2039004000
        add    esp, 4
        mov    dword ptr [edi + 0xc], eax
        mov    dword ptr [edi + 0x10], edx
        mov    edx, dword ptr [esi + 0x28]
        mov    eax, dword ptr [edx + 0x54]
        mov    dword ptr [edi + 0x1c], eax
        mov    eax, dword ptr [esi + 0x30]
        mov    dword ptr [edi + 0x24], eax
        mov    dword ptr [edi + 0x20], 0
        mov    dword ptr [edi + 0x28], 1
        movzx  eax, byte ptr [esi + 0x25]
        mov    dword ptr [edi + 0x30], eax
        mov    eax, dword ptr [esi + 0x1c]
        mov    dword ptr [edi + 0x2c], eax
        jmp    loc_1_4574
loc_1_ACF7:
        mov    esi, dword ptr [ebp + 0x20]
        test   byte ptr [esi + 0xe], 0x10
        jne    loc_1_456D
        mov    esi, dword ptr [esi + 8]
        mov    edx, dword ptr [esi + 0x18]
        mov    eax, dword ptr [esi + 0x14]
        mov    edi, dword ptr [ebp + 0xc]
        call   sub_1_1A6C
        mov    al, 8
        jb     loc_1_454A
        jmp    loc_1_4548
loc_1_AD22:
        mov    esi, dword ptr [ebp + 0xc]
        mov    word ptr [esi], 0
        mov    esi, dword ptr [ebp + 0x20]
        mov    eax, dword ptr [esi + 8]
        test   eax, eax
        je     loc_1_AD54
        cmp    dword ptr [ebp + 0x1c], eax
        jne    loc_1_AD54
        mov    edx, dword ptr [esi + 0xc]
        mov    eax, dword ptr [esi + 0x10]
        mov    edi, dword ptr [ebp + 0xc]
        call   sub_1_1A6C
        mov    al, 8
        jb     loc_1_454A
        jmp    loc_1_4548
loc_1_AD54:
        mov    al, 0x12
        jmp    loc_1_454A
loc_1_AD5B:
        or     word ptr [ebp + 0x38], 0x2000
        mov    dword ptr [ebp + 0x6c], 0
        mov    dword ptr [ebp + 0x64], 0
        mov    esi, dword ptr [ebp + 0xc]
        cmp    word ptr [esi], 0
        je     loc_1_4548
        mov    esi, dword ptr [ebp + 0x20]
        mov    eax, dword ptr [esi + 8]
        test   eax, eax
        je     loc_1_ADD9
        cmp    dword ptr [ebp + 0x1c], eax
        jne    loc_1_ADD9
        mov    ax, word ptr [esi + 0x14]
        mov    word ptr [ebp + 0x45], ax
        mov    word ptr [ebp + 0x42], 8
        mov    eax, dword ptr [ebp + 0xc]
        movzx  edx, word ptr [eax + 2]
        lea    edx, [edx + eax + 2]
        cmp    word ptr [edx], 0x2e
        jne    loc_1_ADB4
        mov    word ptr [edx], 0x3f
        cmp    word ptr [edx + 2], 0x2e
loc_1_ADB4:
        sete   dl
        movzx  edx, dl
        push   edx
        call   sub_1_91B8
        pop    edx
        jb     loc_1_ADD9
        mov    esi, dword ptr [ebp + 0x20]
        mov    eax, dword ptr [ebp + 0x50]
        mov    dword ptr [esi + 0xc], eax
        mov    eax, dword ptr [ebp + 0x4c]
        add    eax, edx
        mov    dword ptr [esi + 0x10], eax
        jmp    loc_1_4548
loc_1_ADD9:
        mov    al, 0x12
        jmp    loc_1_454A
loc_1_ADE0:
        or     word ptr [ebp + 0x38], 0x2000
        mov    dword ptr [ebp + 0x6c], 0
        mov    dword ptr [ebp + 0x64], 0
        call   sub_1_AF87
        mov    eax, 2
        xor    edx, edx
        call   sub_1_43C9
        mov    byte ptr [ebp + 0x45], 0x16
        mov    byte ptr [ebp + 0x46], 0
        mov    eax, dword ptr [ebp + 0xc]
        call   sub_1_91B8
        jb     loc_1_AF3D
        je     loc_1_AF44
        mov    esi, dword ptr [ebp + 0x64]
        VxDCall  Dev_048B.Service_0007h          ; cd2007008b04
        mov    eax, dword ptr [ebp + 0x20]
        mov    eax, dword ptr [eax + 8]
        mov    ecx, dword ptr [ebp + 0x1c]
        cmp    ecx, dword ptr [eax + 0x28]
        jne    loc_1_AF4B
        mov    cl, byte ptr [ebx + 0xb]
        test   cl, 0x18
        jne    loc_1_AF4B
        cmp    cl, byte ptr [eax + 0x43]
        jne    loc_1_AF4B
        lea    esi, [ebx]
        lea    edi, [eax + 0x45]
        mov    ecx, 0xb
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        jne    loc_1_AF4B
        test   dword ptr [ebp], 1
        je     loc_1_AE77
loc_1_AE6C:
        bts    dword ptr [eax + 0x24], 4
        jb     loc_1_AF70
loc_1_AE77:
        cli    
        mov    dl, byte ptr [ebx + 0xd]
        mov    byte ptr [eax + 0x42], dl
        mov    dx, word ptr [ebx + 0xe]
        mov    word ptr [eax + 0x3e], dx
        mov    dx, word ptr [ebx + 0x10]
        mov    word ptr [eax + 0x40], dx
        mov    dx, word ptr [ebx + 0x12]
        mov    word ptr [eax + 0x3c], dx
        mov    dx, word ptr [ebx + 0x16]
        mov    word ptr [eax + 0x38], dx
        mov    dx, word ptr [ebx + 0x18]
        mov    word ptr [eax + 0x3a], dx
        mov    edx, dword ptr [ebx + 0x1c]
        mov    dword ptr [eax + 0x30], edx
        mov    esi, dword ptr [ebp + 0x64]
        mov    edx, ebx
        sub    edx, dword ptr [esi + 0x10]
        shr    edx, 5
        mov    byte ptr [eax + 0x25], dl
        mov    edi, dword ptr [ebp + 0x1c]
        mov    cl, byte ptr [edi + 0x4c]
        sub    cl, 5
        shr    edx, cl
        add    edx, dword ptr [esi + 8]
        mov    dword ptr [eax + 0x1c], edx
        mov    edx, dword ptr [ebp + 0x50]
        mov    dword ptr [eax + 0x18], edx
        mov    edx, dword ptr [ebp + 0x4c]
        mov    dword ptr [eax + 0x14], edx
        xor    edx, edx
        test   byte ptr [eax + 0x24], 8
        je     loc_1_AEE2
        mov    edx, dword ptr [ebx + 0x12]
loc_1_AEE2:
        mov    dx, word ptr [ebx + 0x1a]
        mov    dword ptr [eax + 0x10], edx
        test   dword ptr [ebp], 1
        je     loc_1_AEFF
        and    dword ptr [eax + 0x50], 0
        mov    esi, dword ptr [eax + 0x54]
        sub    esi, 8
        mov    dword ptr [eax + 0x5c], esi
loc_1_AEFF:
        and    byte ptr [eax + 0x44], 0xfe
        sti    
        mov    esi, dword ptr [ebp + 0x64]
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
        sub    eax, eax
loc_1_AF0F:
        push   eax
        call   sub_1_AFC5
        test   dword ptr [ebp], 1
        je     loc_1_AF37
        mov    eax, dword ptr [ebp + 0x20]
        mov    eax, dword ptr [eax + 8]
        and    byte ptr [eax + 0x24], 0xef
        mov    eax, 0xb6b0                   ; -> obj1:0xB6B0
        cmp    dword ptr [eax], 0
        je     loc_1_AF37
        call   sub_1_A258
loc_1_AF37:
        pop    eax
loc_1_AF38:
        jmp    loc_1_454A
loc_1_AF3D:
        mov    eax, 3
        jmp    loc_1_AF0F
loc_1_AF44:
        mov    eax, 2
        jmp    loc_1_AF0F
loc_1_AF4B:
        mov    esi, dword ptr [ebp + 0x64]
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
        mov    eax, 2
        jmp    loc_1_AF0F
loc_1_AF5B:
        mov    esi, dword ptr [ebp + 0x64]
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
        call   sub_1_AFC5
        mov    eax, 5
        jmp    loc_1_AF38
loc_1_AF70:
        test   byte ptr [eax + 0x24], 0x80
        jne    loc_1_AF5B
        push   eax
        mov    eax, 0xb6b0                   ; -> obj1:0xB6B0
        call   sub_1_A220
        pop    eax
        jmp    loc_1_AE6C
sub_1_AF87:
        test   byte ptr [ebp + 0x37], 0x90
        je     loc_1_AFC4
loc_1_AF8D:
        bts    dword ptr [0xb944], 0         ; -> loc_1_B944
        jae    loc_1_AFAB
        bts    dword ptr [0xb944], 0x1f      ; -> loc_1_B944
        mov    eax, 0xb948                   ; -> loc_1_B948
        call   sub_1_A220
        jmp    loc_1_AF8D
loc_1_AFAB:
        sub    eax, eax
        lea    edi, [0xb94c]                 ; -> loc_1_B94C
        mov    dword ptr [ebp + 0x6c], edi
        mov    ecx, 0xf5
        rep stosd dword ptr es:[edi], eax
        mov    cl, 0
        call   sub_1_8D94
loc_1_AFC4:
        ret    
sub_1_AFC5:
        test   byte ptr [ebp + 0x37], 0x90
        je     loc_1_AFEE
        btr    dword ptr [0xb944], 0         ; -> loc_1_B944
        mov    dword ptr [ebp + 0x6c], 0
        btr    dword ptr [0xb944], 0x1f      ; -> loc_1_B944
        jae    loc_1_AFEE
        mov    eax, 0xb948                   ; -> loc_1_B948
        call   sub_1_A258
loc_1_AFEE:
        ret    
sub_1_AFEF:
        mov    ebx, dword ptr [esi]
        or     ebx, ebx
        jne    loc_1_B002
        mov    eax, dword ptr [ebp + 0x1c]
        movzx  eax, word ptr [eax + 0x44]
        cmp    word ptr [esi + 4], ax
        jae    loc_1_B02C
loc_1_B002:
        call   sub_1_1201
        jb     loc_1_B02C
        movzx  eax, word ptr [esi + 4]
        push   esi
        call   sub_1_0BD7
        pop    esi
        jb     loc_1_B02C
        test   byte ptr [ebx + 0xb], 0xd8
        jne    loc_1_B02C
        lea    esi, [esi + 6]
        lea    edi, [ebx]
        mov    ecx, 0xb
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        jne    loc_1_B02C
        clc    
        ret    
loc_1_B02C:
        stc    
        ret    
sub_1_B02E:
        call   sub_1_A26B
        sub    esi, esi
        mov    edi, dword ptr [ebp + 0x1c]
        cli    
        mov    eax, dword ptr [edi + 0x64]
        or     eax, dword ptr [edi + 0x68]
        or     eax, dword ptr [edi + 0x6c]
        or     eax, dword ptr [edi + 0x80]
        or     eax, dword ptr [edi + 0x88]
        or     eax, dword ptr [edi + 0x8c]
        jne    loc_1_B15D
        mov    word ptr [edi + 0x1c], ax
        mov    dword ptr [edi + 0x78], eax
        mov    byte ptr [edi + 0xbb], al
        mov    dword ptr [edi + 0x84], eax
        mov    dword ptr [edi + 0xbc], eax
        mov    dword ptr [edi + 0xc0], eax
        mov    dword ptr [edi + 0xc4], eax
        mov    dword ptr [edi + 0xc8], eax
        sti    
        test   byte ptr [edi + 0x52], 0x10
        je     loc_1_B0A1
        mov    cx, 0xdb01
        mov    ebx, edi
        push   esi
        push   edi
        call   sub_1_449E
        pop    edi
        pop    esi
        jb     loc_1_B15D
loc_1_B0A1:
        push   edi
        push   0x800
        VxDCall  IFSMgr.IFSMgr_GetHeap           ; cd200d004000
        add    esp, 4
        pop    edi
        mov    esi, eax
        or     esi, esi
        je     loc_1_B15D
        bt     word ptr [edi + 0x52], 4
        cmc    
        sbb    ebx, ebx
        and    ebx, 0x100
        or     word ptr [ebp + 0x38], bx
        push   esi
        push   edi
        mov    ebx, edi
        mov    ecx, 1
        sub    edi, edi
        call   sub_1_1458
        pop    edi
        pop    esi
        jb     loc_1_B15D
        push   esi
        push   edi
        call   sub_1_68DF
        pop    edi
        pop    esi
        jb     loc_1_B15D
        xor    ecx, ecx
        xchg   dword ptr [edi + 0x9c], ecx
        jecxz  loc_1_B103
        xchg   ecx, eax
        call   sub_1_A1F8
        xchg   ecx, eax
        call   sub_1_6556
loc_1_B103:
        test   byte ptr [edi + 0x52], 0x10
        jne    loc_1_B126
        push   esi
        push   edi
        mov    ebx, edi
        mov    ecx, 1
        sub    edi, edi
        call   sub_1_1458
        pop    edi
        pop    esi
        jb     loc_1_B15D
        push   esi
        push   edi
        call   sub_1_A5B7
        pop    edi
        pop    esi
loc_1_B126:
        push   esi
        push   edi
        sub    eax, eax
        mov    ebx, edi
        call   sub_1_0979
        pop    edi
        pop    esi
        push   esi
        push   edi
        sub    eax, eax
        mov    ebx, edi
        push   eax
        push   eax
        push   eax
        push   eax
        call   sub_1_0AF3
        pop    edi
        pop    esi
        sub    eax, eax
loc_1_B146:
        push   eax
        or     esi, esi
        je     loc_1_B155
        push   esi
        VxDCall  IFSMgr.IFSMgr_RetHeap           ; cd200e004000
        add    esp, 4
loc_1_B155:
        pop    eax
        or     eax, eax
        je     loc_1_B15B
        stc    
loc_1_B15B:
        sti    
        ret    
loc_1_B15D:
        mov    eax, 5
        jmp    loc_1_B146
VFAT_Control:
        cmp    eax, 0
        je     0xffff02c4                    ; -> obj3:0x02C4
        cmp    eax, 1
        je     loc_3_02C6
        cmp    eax, 2
        je     loc_3_0494
        cmp    eax, 4
        je     loc_1_B195
        cmp    eax, 0x14
        je     loc_1_B19D
        cmp    eax, 6
        je     loc_1_B195
        cmp    eax, 5
        je     loc_1_B195
        clc    
        ret    
loc_1_B195:
        inc    dword ptr [0xbd70]            ; -> obj1:0xBD70
        clc    
        ret    
loc_1_B19D:
        test   dword ptr [0xb1a3], 0x400000  ; -> obj1:0xB1A3
        jne    loc_4_0257
        ret    
        db  0F8h, 0C3h, 0F8h, 0C3h, 000h, 000h, 000h, 000h, 000h, 0F0h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 001h, 001h, 002h, 003h, 004h, 005h, 000h, 000h, 000h
loc_1_B1EC:
        pop    esp
        outsb  dx, byte ptr [esi]
        cmp    eax, 0x4f203d3d
        inc    esi
        push   esp
        and    byte ptr [ebp + edx*2 + 0x4d], al
        push   eax
        and    byte ptr [0x6e5c3d3d], bh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_B204:
        dec    edi
        inc    esi
        push   esp
        and    byte ptr [eax], ch
        and    eax, 0x2029786c
        and    eax, 0x2e253a63
        xor    dword ptr [ecx], esi
        jae    0xb273
        outsb  dx, byte ptr [esi]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_B21C:
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        push   0x2f746e63
        insb   byte ptr es:[edi], dx
        popal  
        cmp    eax, 0x20786c25
        and    byte ptr [eax], al
loc_1_B230:
        and    byte ptr [eax], ah
        push   edx
        das    
        push   edi
        je     0xb258
        cmp    eax, 0x786c2520
        pop    esp
        outsb  dx, byte ptr [esi]
        add    byte ptr [eax], ah
        and    byte ptr [eax], ah
        and    byte ptr [ebx + 0x46], dl
        push   esp
        and    byte ptr [ebx + 0x68], al
        popal  
        imul   ebp, dword ptr [esi + 0x3a], 0x6e5c
        add    byte ptr [eax], al
loc_1_B254:
        or     dword ptr [ebp + 0x6d], eax
        jo     0xb2cd
        jns    0xb289
        pop    esp
        outsb  dx, byte ptr [esi]
        add    byte ptr [eax], al
        add    byte ptr [ecx], cl
        push   ebp
        dec    ecx
        inc    esp
        cmp    eax, 0x20786c25
        push   eax
        dec    ecx
        inc    esp
        cmp    eax, 0x5c786c25
        outsb  dx, byte ptr [esi]
        add    byte ptr [eax], al
loc_1_B274:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_B278:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_B27C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_B280:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_B284:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_B288:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_B28C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_B290:
        xchg   esi, eax
        xor    byte ptr [edi], al
        ja     loc_1_B2C1
        popal  
        push   cs
        out    dx, al
        mov    edx, 0x19990951
        les    ebp, ptr [ebp + 7]
        db  08Fh, 0F4h, 06Ah, 070h, 035h, 0A5h, 063h, 0E9h, 0A3h, 095h, 064h, 09Eh, 032h, 088h, 0DBh, 00Eh
        db  0A4h, 0B8h, 0DCh, 079h, 01Eh, 0E9h, 0D5h, 0E0h, 088h, 0D9h, 0D2h, 097h, 02Bh, 04Ch, 0B6h, 009h
        db  0BDh
loc_1_B2C1:
        jl     loc_1_B274
        jle    0xb2cc
        sub    eax, 0x1d91e7b8
        mov    edi, 0xb7106490
        sbb    eax, 0x6ab020f2
        dec    eax
        jno    loc_1_B290
        fiadd  word ptr [ecx - 0x42]
        test   byte ptr [ebp - 0x2c], bh
        ficomp dword ptr [edx]
        jmp    0xb2c6
        db  0DDh, 06Dh, 051h, 0B5h, 0D4h, 0F4h, 0C7h, 085h, 0D3h, 083h, 056h, 098h, 06Ch, 013h, 0C0h, 0A8h
        db  06Bh, 064h, 07Ah, 0F9h, 062h, 0FDh, 0ECh, 0C9h, 065h, 08Ah, 04Fh, 05Ch, 001h, 014h, 0D9h, 06Ch
        db  006h, 063h, 063h, 03Dh, 00Fh, 0FAh, 0F5h, 00Dh, 008h, 08Dh, 0C8h, 020h, 06Eh, 03Bh, 05Eh, 010h
        db  069h, 04Ch, 0E4h, 041h, 060h, 0D5h, 072h, 071h, 067h, 0A2h, 0D1h, 0E4h, 003h, 03Ch, 047h, 0D4h
        db  004h, 04Bh, 0FDh, 085h, 00Dh, 0D2h, 06Bh, 0B5h, 00Ah, 0A5h, 0FAh, 0A8h, 0B5h, 035h, 06Ch, 098h
        db  0B2h, 042h, 0D6h, 0C9h, 0BBh, 0DBh, 040h, 0F9h, 0BCh, 0ACh, 0E3h, 06Ch, 0D8h, 032h, 075h, 05Ch
        db  0DFh, 045h, 0CFh, 00Dh, 0D6h, 0DCh, 059h, 03Dh, 0D1h, 0ABh, 0ACh, 030h, 0D9h, 026h, 03Ah, 000h
        db  0DEh, 051h, 080h, 051h, 0D7h, 0C8h, 016h, 061h, 0D0h, 0BFh, 0B5h, 0F4h, 0B4h, 021h, 023h, 0C4h
        db  0B3h, 056h, 099h, 095h, 0BAh, 0CFh, 00Fh, 0A5h, 0BDh, 0B8h, 09Eh, 0B8h, 002h, 028h, 008h, 088h
        db  005h, 05Fh, 0B2h, 0D9h, 00Ch, 0C6h, 024h, 0E9h, 00Bh, 0B1h, 087h, 07Ch, 06Fh, 02Fh, 011h, 04Ch
        db  068h, 058h, 0ABh, 01Dh, 061h, 0C1h, 03Dh, 02Dh, 066h, 0B6h, 090h, 041h, 0DCh, 076h, 006h, 071h
        db  0DBh, 001h, 0BCh, 020h, 0D2h, 098h, 02Ah, 010h, 0D5h, 0EFh, 089h, 085h, 0B1h, 071h, 01Fh, 0B5h
        db  0B6h, 006h, 0A5h, 0E4h, 0BFh, 09Fh, 033h, 0D4h, 0B8h, 0E8h, 0A2h, 0C9h, 007h, 078h, 034h, 0F9h
        db  000h, 00Fh, 08Eh, 0A8h, 009h, 096h, 018h, 098h, 00Eh, 0E1h, 0BBh, 00Dh, 06Ah, 07Fh, 02Dh, 03Dh
        db  06Dh, 008h, 097h, 06Ch, 064h, 091h, 001h, 05Ch, 063h, 0E6h, 0F4h, 051h, 06Bh, 06Bh, 062h, 061h
        db  06Ch, 01Ch, 0D8h, 030h, 065h, 085h, 04Eh, 000h, 062h, 0F2h, 0EDh, 095h, 006h, 06Ch, 07Bh, 0A5h
        db  001h, 01Bh, 0C1h, 0F4h, 008h, 082h, 057h, 0C4h, 00Fh, 0F5h, 0C6h, 0D9h, 0B0h, 065h, 050h, 0E9h
        db  0B7h, 012h, 0EAh, 0B8h, 0BEh, 08Bh, 07Ch, 088h, 0B9h, 0FCh, 0DFh, 01Dh, 0DDh, 062h, 049h, 02Dh
        db  0DAh, 015h, 0F3h, 07Ch, 0D3h, 08Ch, 065h, 04Ch, 0D4h, 0FBh, 058h, 061h, 0B2h, 04Dh, 0CEh, 051h
        db  0B5h, 03Ah, 074h, 000h, 0BCh, 0A3h, 0E2h, 030h, 0BBh, 0D4h, 041h, 0A5h, 0DFh, 04Ah, 0D7h, 095h
        db  0D8h, 03Dh, 06Dh, 0C4h, 0D1h, 0A4h, 0FBh, 0F4h, 0D6h, 0D3h, 06Ah, 0E9h, 069h, 043h, 0FCh, 0D9h
        db  06Eh, 034h, 046h, 088h, 067h, 0ADh, 0D0h, 0B8h, 060h, 0DAh, 073h, 02Dh, 004h, 044h, 0E5h, 01Dh
        db  003h, 033h, 05Fh, 04Ch, 00Ah, 0AAh, 0C9h, 07Ch, 00Dh, 0DDh, 03Ch, 071h, 005h, 050h, 0AAh, 041h
        db  002h, 027h, 010h, 010h, 00Bh, 0BEh, 086h, 020h, 00Ch, 0C9h, 025h, 0B5h, 068h, 057h, 0B3h, 085h
        db  06Fh, 020h, 009h, 0D4h, 066h, 0B9h, 09Fh, 0E4h, 061h, 0CEh, 00Eh, 0F9h, 0DEh, 05Eh, 098h, 0C9h
        db  0D9h, 029h, 022h, 098h, 0D0h, 0B0h, 0B4h, 0A8h, 0D7h, 0C7h, 017h, 03Dh, 0B3h, 059h, 081h, 00Dh
        db  0B4h, 02Eh, 03Bh, 05Ch, 0BDh, 0B7h, 0ADh, 06Ch, 0BAh, 0C0h, 020h, 083h, 0B8h, 0EDh, 0B6h, 0B3h
        db  0BFh, 09Ah, 00Ch, 0E2h, 0B6h, 003h, 09Ah, 0D2h, 0B1h, 074h, 039h, 047h, 0D5h, 0EAh, 0AFh, 077h
        db  0D2h, 09Dh, 015h, 026h, 0DBh, 004h, 083h, 016h, 0DCh, 073h, 012h, 00Bh, 063h, 0E3h, 084h, 03Bh
        db  064h, 094h, 03Eh, 06Ah, 06Dh, 00Dh, 0A8h, 05Ah, 06Ah, 07Ah, 00Bh, 0CFh, 00Eh, 0E4h, 09Dh, 0FFh
        db  009h, 093h, 027h, 0AEh, 000h, 00Ah, 0B1h, 09Eh, 007h, 07Dh, 044h, 093h, 00Fh, 0F0h, 0D2h, 0A3h
        db  008h, 087h, 068h, 0F2h, 001h, 01Eh, 0FEh, 0C2h, 006h, 069h, 05Dh, 057h, 062h, 0F7h, 0CBh, 067h
        db  065h, 080h, 071h, 036h, 06Ch, 019h, 0E7h, 006h, 06Bh, 06Eh, 076h, 01Bh, 0D4h, 0FEh, 0E0h, 02Bh
        db  0D3h, 089h, 05Ah, 07Ah, 0DAh, 010h, 0CCh, 04Ah, 0DDh, 067h, 06Fh, 0DFh, 0B9h, 0F9h, 0F9h, 0EFh
        db  0BEh, 08Eh, 043h, 0BEh, 0B7h, 017h, 0D5h, 08Eh, 0B0h, 060h, 0E8h, 0A3h, 0D6h, 0D6h, 07Eh, 093h
        db  0D1h, 0A1h, 0C4h, 0C2h, 0D8h, 038h, 052h, 0F2h, 0DFh, 04Fh, 0F1h, 067h, 0BBh, 0D1h, 067h, 057h
        db  0BCh, 0A6h, 0DDh, 006h, 0B5h, 03Fh, 04Bh, 036h, 0B2h, 048h, 0DAh, 02Bh, 00Dh, 0D8h, 04Ch, 01Bh
        db  00Ah, 0AFh, 0F6h, 04Ah, 003h, 036h, 060h, 07Ah, 004h, 041h, 0C3h, 0EFh, 060h, 0DFh, 055h, 0DFh
        db  067h, 0A8h, 0EFh, 08Eh, 06Eh, 031h, 079h, 0BEh, 069h, 046h, 08Ch, 0B3h, 061h, 0CBh, 01Ah, 083h
        db  066h, 0BCh, 0A0h, 0D2h, 06Fh, 025h, 036h, 0E2h, 068h, 052h, 095h, 077h, 00Ch, 0CCh, 003h, 047h
        db  00Bh, 0BBh, 0B9h, 016h, 002h, 022h, 02Fh, 026h, 005h, 055h, 0BEh, 03Bh, 0BAh, 0C5h, 028h, 00Bh
        db  0BDh, 0B2h, 092h, 05Ah, 0B4h, 02Bh, 004h, 06Ah, 0B3h, 05Ch, 0A7h, 0FFh, 0D7h, 0C2h, 031h, 0CFh
        db  0D0h, 0B5h, 08Bh, 09Eh, 0D9h, 02Ch, 01Dh, 0AEh, 0DEh, 05Bh, 0B0h, 0C2h, 064h, 09Bh, 026h, 0F2h
        db  063h, 0ECh, 09Ch, 0A3h, 06Ah, 075h, 00Ah, 093h, 06Dh, 002h, 0A9h, 006h, 009h, 09Ch, 03Fh, 036h
        db  00Eh, 0EBh, 085h, 067h, 007h, 072h, 013h, 057h, 000h, 005h, 082h, 04Ah, 0BFh, 095h, 014h, 07Ah
        db  0B8h, 0E2h, 0AEh, 02Bh, 0B1h, 07Bh, 038h, 01Bh, 0B6h, 00Ch, 09Bh, 08Eh, 0D2h, 092h, 00Dh, 0BEh
        db  0D5h, 0E5h, 0B7h, 0EFh, 0DCh, 07Ch, 021h, 0DFh, 0DBh, 00Bh, 0D4h, 0D2h, 0D3h, 086h, 042h, 0E2h
        db  0D4h, 0F1h, 0F8h, 0B3h, 0DDh, 068h, 06Eh, 083h, 0DAh, 01Fh, 0CDh, 016h, 0BEh, 081h, 05Bh, 026h
        db  0B9h, 0F6h, 0E1h, 077h, 0B0h, 06Fh, 077h, 047h, 0B7h, 018h, 0E6h, 05Ah, 008h, 088h, 070h, 06Ah
        db  00Fh, 0FFh, 0CAh, 03Bh, 006h, 066h, 05Ch, 00Bh, 001h, 011h, 0FFh, 09Eh, 065h, 08Fh, 069h, 0AEh
        db  062h, 0F8h, 0D3h, 0FFh, 06Bh, 061h, 045h, 0CFh, 06Ch, 016h, 078h, 0E2h, 00Ah, 0A0h, 0EEh, 0D2h
        db  00Dh, 0D7h, 054h, 083h, 004h, 04Eh, 0C2h, 0B3h, 003h, 039h, 061h, 026h, 067h, 0A7h, 0F7h, 016h
        db  060h, 0D0h, 04Dh, 047h, 069h, 049h, 0DBh, 077h, 06Eh, 03Eh, 04Ah, 06Ah, 0D1h, 0AEh, 0DCh, 05Ah
        db  0D6h, 0D9h, 066h, 00Bh, 0DFh, 040h, 0F0h, 03Bh, 0D8h, 037h, 053h, 0AEh, 0BCh, 0A9h, 0C5h, 09Eh
        db  0BBh, 0DEh, 07Fh, 0CFh, 0B2h, 047h, 0E9h, 0FFh, 0B5h, 030h, 01Ch, 0F2h, 0BDh, 0BDh, 08Ah, 0C2h
        db  0BAh, 0CAh, 030h, 093h, 0B3h, 053h, 0A6h, 0A3h, 0B4h, 024h, 005h, 036h, 0D0h, 0BAh, 093h, 006h
        db  0D7h, 0CDh, 029h, 057h, 0DEh, 054h, 0BFh, 067h, 0D9h, 023h, 02Eh, 07Ah, 066h, 0B3h, 0B8h, 04Ah
        db  061h, 0C4h, 002h, 01Bh, 068h, 05Dh, 094h, 02Bh, 06Fh, 02Ah, 037h, 0BEh, 00Bh, 0B4h, 0A1h, 08Eh
        db  00Ch, 0C3h, 01Bh, 0DFh, 005h, 05Ah, 08Dh, 0EFh, 002h, 02Dh, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_B6F0:
        mov    esp, 0xb6
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edi], cl
        push   cs
        or     al, 8
        push   ecx
        test   dword ptr [ecx], 0xf0703
        add    al, byte ptr [esi]
        push   cs
        add    byte ptr [eax], al
        add    al, 0xc
        add    byte ptr [eax], al
        add    byte ptr [eax], cl
loc_1_B748:
        dec    eax
        mov    bh, 0
        add    byte ptr [eax - 0x49], cl     ; -> loc_1_B748
        add    byte ptr [eax], al
loc_1_B750:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_B754:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_B758:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_B75C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_B760:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_B764:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_B768:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_B76C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_B770:
        dec    esi
        dec    edi
        and    byte ptr [esi + 0x41], cl
        dec    ebp
        inc    ebp
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
loc_1_B77B:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx], cl
        add    edx, dword ptr [eax]
        movaps xmm5, xmmword ptr [eax]       ; -> loc_1_6828
        add    byte ptr [eax], ch            ; -> loc_1_6828
        push   0x68280000                    ; -> loc_1_6828
        add    byte ptr [eax], al
        sub    byte ptr [eax], ch
        add    byte ptr [eax], ch            ; -> loc_1_6828
        push   0x68280000                    ; -> loc_1_6828
        add    byte ptr [eax], al
        sub    byte ptr [eax], ch
        add    byte ptr [eax], ch            ; -> loc_1_6828
        push   0x68280000                    ; -> loc_1_6828
        add    byte ptr [eax], al
        push   0x68280000                    ; -> loc_1_6828
        add    byte ptr [eax], al
        sub    byte ptr [eax], ch
        add    byte ptr [eax], ch            ; -> loc_1_6828
        push   0x68280000                    ; -> loc_1_6828
        add    byte ptr [eax], al
        sub    byte ptr [eax], ch
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [ecx], al
        add    byte ptr [ecx], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [ecx], al
        add    byte ptr [ecx], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [ecx], eax
        add    dword ptr [edx], eax
        add    al, byte ptr [edx]
        add    al, byte ptr [ebx]
        add    eax, dword ptr [esi + eax]
        pop    ds
loc_1_B81D:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_B835:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx], cl
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], cl
        add    byte ptr [eax], al
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
loc_1_B86F:
        cmp    edi, -1
        db  0FFh, 0FFh
loc_1_B874:
        hlt    
        add    dword ptr [eax], eax
        add    al, ch
        add    eax, dword ptr [eax]
        db  000h
loc_1_B87C:
        rol    byte ptr [edi], 1
        add    byte ptr [eax], al
loc_1_B880:
        and    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_B884:
        std    
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], cl
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_B897:
        add    byte ptr [eax], al
        add    bl, bh
        jg     loc_1_B89D
loc_1_B89D:
        add    bl, bh
        jg     loc_1_B897
        inc    dword ptr [eax]
        add    dh, dh
        inc    dword ptr [eax]
        add    bh, bh
        db  0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_B8C0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_B8D0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_B8F4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_B8F8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_B8FC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_B900:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_B904:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        or     byte ptr [eax], al
        db  0F0h, 001h, 0CDh, 013h, 0CDh, 014h, 0CDh, 001h, 0CDh, 003h
loc_1_B914:
        inc    ecx
        cmp    al, byte ptr [eax]
        db  000h
loc_1_B918:
        sbb    byte ptr [ecx - 0x46e80000], bh ; -> loc_1_B918
        add    byte ptr [eax], al
loc_1_B920:
        or     al, byte ptr [ebx]
        adc    byte ptr [eax], cl
        push   0x2000046                     ; -> loc_1_AA02
        stosb  byte ptr es:[edi], al
        add    byte ptr [eax], al
        push   0x68000046                    ; -> loc_1_4668
        inc    esi
        add    byte ptr [eax], al
        push   0x68000046                    ; -> loc_1_4668
        inc    esi
        add    byte ptr [eax], al
        push   0x24000046                    ; -> loc_1_AC24
        lodsb  al, byte ptr [esi]
        add    byte ptr [eax], al
loc_1_B944:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_B948:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_B94C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
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
        dw  00486h                      ; DDB_Req_Device_Number
        db  3, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'VFAT    '                  ; DDB_Name
        dd  0A0010100h                  ; DDB_Init_Order
        dd  VFAT_Control             ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h
        dd  loc_1_8238               ; fixup
        dd  loc_1_8253               ; fixup
        dd  loc_1_8255               ; fixup
        db  000h, 000h, 000h, 000h, 000h, 000h
loc_1_BD90:
        pop    ebx
        and    eax, 0x2d205d63
        and    byte ptr [0x6e5c73], ah
loc_1_BD9C:
        and    eax, 0x3f6e5c73
        and    byte ptr [eax], al
        db  000h
loc_1_BDA4:
        and    eax, 0x6e5c63
        add    byte ptr [eax], al
        db  000h
loc_1_BDAC:
        pop    edx
        jb     0xbe1f
        and    byte ptr [esi + 0x69], ah
        insb   byte ptr es:[edi], dx
        jae    0xbdd8
        ja     0xbe23
        insb   byte ptr es:[edi], dx
        insb   byte ptr es:[edi], dx
        and    byte ptr [0x20656273], ah
        imul   esi, dword ptr fs:[ebx + 0x70], 0x6579616c
        pop    esp
        outsb  dx, byte ptr [esi]
        add    byte ptr [eax], al
loc_1_BDD0:
        push   ebx
        outsd  dx, dword ptr [esi]
        jb     0xbe46
        jns    0xbe02
        and    byte ptr [ecx + 0x6f], bh
        jne    0xbdfb
        outsb  dx, byte ptr [esi]
        and    byte ptr fs:[eax + ebp*2 + 0x65], dh
        and    byte ptr [ebp + 0x62], ah
        jne    0xbe50
        and    byte ptr [esi + 0x65], dh
        jb     0xbe61
        db  069h, 06Fh, 06Eh, 02Eh, 000h, 000h
loc_1_BDF4:
        and    eax, 0x42435463
        cmp    eax, 0x6c383025
        js     0xbe20
        and    byte ptr [eax + ecx*2 + 0x49], dl
        inc    esp
        cmp    eax, 0x6c343025
        js     loc_1_BE2C
        and    byte ptr [ebx + 0x6f], al
        jne    0xbe7f
        je     0xbe50
        and    eax, 0x786c3830
        pop    esp
        outsb  dx, byte ptr [esi]
        add    byte ptr [eax], al
loc_1_BE1C:
        and    eax, 0x42435463
        cmp    eax, 0x6c383025
        js     loc_1_BE48
        and    byte ptr [eax + ecx*2 + 0x49], dl
loc_1_BE2C:
        inc    esp
        cmp    eax, 0x6c343025
        js     0xbe54
        and    byte ptr [ebx + 0x6f], al
        jne    0xbea7
        je     loc_1_BE78
        and    eax, 0x786c3830
        pop    esp
        outsb  dx, byte ptr [esi]
        add    byte ptr [eax], al
loc_1_BE44:
        inc    ebp
        js     loc_1_BEAA
        insb   byte ptr es:[edi], dx
loc_1_BE48:
        and    byte ptr [eax + ebp*2 + 0x72], dh
        popal  
        jae    loc_1_BE71
        cmp    eax, 0x38302520
        insb   byte ptr es:[edi], dx
        js     loc_1_BE79
        and    byte ptr [eax], ah
        dec    esi
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        sub    eax, 0x6c637865
        and    byte ptr [eax + ebp*2 + 0x72], dh
        popal  
        jae    0xbe8c
        cmp    eax, 0x38302520
loc_1_BE71:
        insb   byte ptr es:[edi], dx
        js     0xbed0
        outsb  dx, byte ptr [esi]
        add    byte ptr [eax], al
        db  000h
loc_1_BE78:
        dec    esi
loc_1_BE79:
        outsd  dx, dword ptr [esi]
        and    byte ptr [eax + ebp*2 + 0x72], dh
        popal  
        jae    loc_1_BEA3
        arpl   word ptr [ebp + 0x72], si
        jb     loc_1_BEED
        outsb  dx, byte ptr [esi]
        je     0xbef7
        jns    0xbead
        imul   ebp, dword ptr [esi + 0x20], 0x54414656
        pop    esp
        outsb  dx, byte ptr [esi]
        add    byte ptr [eax], al
loc_1_BE98:
        push   esp
        dec    ecx
        inc    esp
        cmp    eax, 0x78343025
        and    byte ptr [ebx + 0x42], al
loc_1_BEA3:
        dec    esp
        dec    ebx
        cmp    eax, 0x78343025
loc_1_BEAA:
        and    byte ptr [0x6e5c73], ah
loc_1_BEB0:
        pop    esp
        outsb  dx, byte ptr [esi]
        and    eax, 0x64412073
        jb     loc_1_BF1F
        jae    0xbf2f
        and    byte ptr [0x38302520], bh
        insb   byte ptr es:[edi], dx
        js     0xbf21
        outsb  dx, byte ptr [esi]
        add    byte ptr [eax], al
loc_1_BEC8:
        and    eax, 0x203d2073
        and    eax, 0x786c3430
        add    byte ptr [eax], al
loc_1_BED4:
        and    eax, 0x203d2073
        and    eax, 0x786c3830
        add    byte ptr [eax], al
loc_1_BEE0:
        and    eax, 0x203d2073
        add    byte ptr [eax], al
        add    byte ptr [0x786c3230], ah
loc_1_BEED:
        add    byte ptr [eax], al
        db  000h
loc_1_BEF0:
        and    eax, 0x203d2073
        daa    
        add    byte ptr [eax], al
        db  025h, 063h, 000h, 000h
loc_1_BEFC:
        daa    
        add    byte ptr [eax], al
        add    byte ptr [eax], ah
        add    byte ptr [eax], al
        add    byte ptr [esi + ebp*2], bl
        add    byte ptr [esi + ebp*2], bl
        add    byte ptr [eax + eax], bl
        add    byte ptr [edx], ch
        add    byte ptr [eax], al
        add    byte ptr [0x73312e], ah
        add    byte ptr [eax], al
        add    byte ptr [esi + ebp*2], bl
loc_1_BF1F:
        add    byte ptr [ecx + 0x72], ch
        pop    edi
        jo     0xbf95
        popal  
        je     0xbf90
        db  020h, 03Dh, 020h, 000h
loc_1_BF2C:
        pop    esp
        outsb  dx, byte ptr [esi]
        add    byte ptr [eax], al
loc_1_BF30:
        dec    edi
        je     0xbf93
        dec    esi
        popal  
        insd   dword ptr es:[edi], dx
        and    byte ptr gs:[0x73252720], bh
        daa    
        pop    esp
        outsb  dx, byte ptr [esi]
        add    byte ptr [eax], al
        add    byte ptr [edi + 0x66], cl
        je     0xbfa7
        inc    esp
        jb     0xbfb4
        jbe    0xbfb2
        and    byte ptr [0x3a632520], bh
        pop    esp
        outsb  dx, byte ptr [esi]
        add    byte ptr [eax], al
        add    byte ptr [edi + 0x66], cl
        je     0xbfbb
        dec    esi
        popal  
        insd   dword ptr es:[edi], dx
        and    byte ptr gs:[0x73252720], bh
        daa    
        pop    esp
        outsb  dx, byte ptr [esi]
        add    byte ptr [eax], al
        db  000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x63c  R-X
loc_2_0000:
        sub    eax, dword ptr [eax]
        add    byte ptr [eax], al
loc_2_0004:
        add    byte ptr fs:[eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [edx], dl
        add    byte ptr [ebx], dl
loc_2_0012:
        out    dx, eax
        db  000h
loc_2_0014:
        adc    al, byte ptr [ecx]
loc_2_0016:
        jnp    0x19
loc_2_0018:
        mov    byte ptr [ecx], al
loc_2_001A:
        mov    es, word ptr [ecx]
loc_2_001C:
        popfd  
        db  001h
loc_2_001E:
        test   al, 1
loc_2_0020:
        in     eax, 1
        db  0F3h, 001h
loc_2_0024:
        std    
        db  001h
loc_2_0026:
        cmp    dword ptr [edx], eax
loc_2_0028:
        dec    esp
        db  002h
loc_2_002A:
        push   edi
        db  002h, 083h, 002h
loc_2_002E:
        cdq    
        db  002h
loc_2_0030:
        movsd  dword ptr es:[edi], dword ptr [esi]
        db  002h
loc_2_0032:
        rol    byte ptr [edx], cl
loc_2_0034:
        fiadd  word ptr [edx]
loc_2_0036:
        jmp    0xc203af3d
        db  003h, 0D1h, 003h, 0F0h, 003h, 006h, 004h, 013h, 004h, 035h, 004h, 046h, 004h, 055h, 004h, 074h
        db  004h, 092h, 004h, 0A1h, 004h, 0CAh, 004h, 0EAh, 004h, 0F8h, 004h, 021h, 005h, 046h, 005h, 053h
        db  005h, 093h, 005h, 0A4h, 005h, 0BAh, 005h, 0D8h, 005h
loc_2_0064:
        inc    esp
        imul   esi, dword ptr [ebx + 0x6b], 0x69725720
        je     0xd3
        and    byte ptr [ebp + 0x72], al
        jb     0xe2
        jb     loc_2_0075
loc_2_0075:
        add    byte ptr [ebp + 0x6e], dl
        popal  
        bound  ebp, qword ptr [ebp + 0x20]
        je     0xee
        and    byte ptr [edi + 0x72], dh
        imul   esi, dword ptr [ebp + 0x20], 0x64206f74
        imul   esi, dword ptr [ebx + 0x6b], 0x206e6920
        jb     0xfd
        jbe    loc_2_00FB
        and    byte ptr [0x440a0d73], ah
        popal  
        je     0x100
        and    byte ptr [edi + 0x72], ch
        and    byte ptr [esi + 0x69], ah
        insb   byte ptr es:[edi], dx
        jae    0xc9
        insd   dword ptr es:[edi], dx
        popal  
        jns    0xcd
        bound  esp, qword ptr [ebp + 0x20]
        insb   byte ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        jae    0x128
        add    byte ptr cs:[eax], al
        push   esi
        inc    esi
        inc    ecx
        push   esp
        and    byte ptr [esi], ah
        and    byte ptr [edi + ecx*2 + 0x53], al
        and    byte ptr [esi + 0x6f], dh
        insb   byte ptr es:[edi], dx
        jne    0x134
        and    byte ptr gs:[ecx + 0x6e], ch
        and    byte ptr [edx + esi*2 + 0x69], ah
        jbe    0x136
        and    byte ptr [0x616c2073], ah
        jns    loc_2_0148
        jne    0x14f
        jae    0xfd
        popal  
        jb     0x145
        and    byte ptr [ebp + 0x49], cl
        push   ebx
        dec    ebp
        inc    ecx
        push   esp
        inc    ebx
        dec    eax
        inc    ebp
        inc    esp
        or     eax, 0x6e61430a
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr [esi]
        je     0x115
        jo     0x169
        outsd  dx, dword ptr [esi]
        arpl   word ptr [ebp + 0x65], sp
loc_2_00FB:
        and    byte ptr fs:[esi + 0x75], ah
        jb     0x175
        push   0x202e7265
        and    byte ptr [edi + ebp*2 + 0x69], al
        outsb  dx, byte ptr [esi]
        and    byte ptr [bp + di + 0x6f], dh
        and    byte ptr [ebp + 0x61], ch
        jns    0x134
        jae    0x18c
        jb     0x189
        jns    0x13c
        imul   esi, dword ptr fs:[ebx + 0x6b], 0x74616420
        popal  
        or     eax, 0x656c500a
        popal  
        jae    0x193
        and    byte ptr [edx + 0x65], dh
        jo     0x1a2
        jb     loc_2_01A9
        and    byte ptr [eax + ebp*2 + 0x69], dh
        jae    loc_2_015B
        arpl   word ptr [edi + 0x6e], bp
        imul   esi, dword ptr fs:[ecx + ebp*2 + 0x6f], 0x6d69206e
        insd   dword ptr es:[edi], dx
loc_2_0148:
        imul   esp, dword ptr fs:[ecx + 0x74], 0x2e796c65
        add    byte ptr [eax], al
        push   esi
        inc    esi
        inc    ecx
        push   esp
        and    byte ptr [ebp + 0x76], al
loc_2_015B:
        imul   esp, dword ptr [ebx + 0x65], 0x696e4920
        je     0x1cd
        popal  
        insb   byte ptr es:[edi], dx
        imul   edi, dword ptr [edx + 0x61], 0x6e6f6974
        and    byte ptr [esi + 0x61], al
        imul   ebp, dword ptr [ebp + 0x64], 0x20410000
        jbe    0x1e5
        arpl   word ptr [ebp + 0x20], sp
        outsd  dx, dword ptr [esi]
        jb     0x1a2
        jb     0x1e9
        jae    loc_2_01F5
        jne    0x1fa
        arpl   word ptr [ebp + 0x20], sp
        jb     loc_2_01F2
        jno    loc_2_0204
        imul   esi, dword ptr [edx + 0x65], 0x79622064
        and    byte ptr [esi + 0x46], dl
        inc    ecx
        push   esp
        and    byte ptr [ecx + 0x73], ch
        and    byte ptr [esi + 0x6f], ch
        je     loc_2_01C3
        jo     0x217
        jae    0x20d
        outsb  dx, byte ptr [esi]
loc_2_01A9:
        je     0x1b8
        or     ch, byte ptr [edi + 0x72]
        and    byte ptr [ecx + 0x73], ch
        and    byte ptr [ebp + 0x6e], dh
        popal  
        jbe    loc_2_0218
        imul   ebp, dword ptr [ecx + 0x62], 0x202e656c
        and    byte ptr [esi + 0x46], dl
        inc    ecx
loc_2_01C3:
        push   esp
        and    byte ptr [ebx + 0x61], ah
        outsb  dx, byte ptr [esi]
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr [esi]
        je     loc_2_01EC
        arpl   word ptr [edi + 0x6e], bp
        je     loc_2_023A
        outsb  dx, byte ptr [esi]
        jne    0x239
        and    byte ptr [edi + ebp*2 + 0x61], ch
        imul   ebp, dword ptr fs:[esi + 0x67], 0x4600002e
        imul   ebp, dword ptr [ebp + 0x20], 0x74737953
        insd   dword ptr es:[edi], dx
        add    byte ptr [eax], al
loc_2_01EC:
        push   esi
        inc    esi
        inc    ecx
        push   esp
        add    byte ptr [eax], al
loc_2_01F2:
        push   edi
        jb     0x25e
loc_2_01F5:
        je     0x25c
        jae    0x228
        jae    loc_2_0260
        arpl   word ptr [edi + 0x6e], bp
        add    byte ptr fs:[eax], al
        push   edi
        jb     loc_2_026D
loc_2_0204:
        je     0x26b
        jae    0x25b
        arpl   word ptr gs:[eax], ax
        add    byte ptr [esi + 0x75], cl
        insd   dword ptr es:[edi], dx
        bound  esp, qword ptr [ebp + 0x72]
        and    byte ptr [edi + 0x66], ch
        and    byte ptr [edi + 0x72], dh
loc_2_0218:
        imul   esi, dword ptr [ebp + 0x20], 0x75716572
        jae    loc_2_0297
        jae    0x245
        insd   dword ptr es:[edi], dx
        popal  
        and    byte ptr gs:[edi + ebp*2 + 0x20], dh
        je     loc_2_0297
        and    byte ptr gs:[esi + 0x69], ah
        insb   byte ptr es:[edi], dx
        and    byte ptr gs:[ebx + 0x79], dh
        jae    0x2ae
loc_2_023A:
        insd   dword ptr es:[edi], dx
        and    byte ptr [eax + 0x65], dh
        jb     0x261
        jae    loc_2_02A8
        arpl   word ptr [edi + 0x6e], bp
        add    byte ptr fs:[eax], al
        push   edx
        popal  
        jae    0x27e
        jae    0x2b6
        arpl   word ptr [edi + 0x6e], bp
        add    byte ptr fs:[eax], al
        push   edx
        popal  
        jae    loc_2_02B0
        arpl   word ptr gs:[eax], ax
loc_2_0260:
        add    byte ptr [esi + 0x75], cl
        insd   dword ptr es:[edi], dx
        bound  esp, qword ptr [ebp + 0x72]
        and    byte ptr [edi + 0x66], ch
        and    byte ptr [edx + 0x65], dh
loc_2_026D:
        popal  
        and    byte ptr fs:[edx + 0x65], dh
        jno    0x2e9
        jae    0x2eb
        jae    0x299
        insd   dword ptr es:[edi], dx
        popal  
        and    byte ptr gs:[edi + ebp*2 + 0x20], dh
        je     0x2eb
        and    byte ptr gs:[esi + 0x69], ah
        insb   byte ptr es:[edi], dx
        and    byte ptr gs:[ebx + 0x79], dh
        jae    0x302
        insd   dword ptr es:[edi], dx
        and    byte ptr [eax + 0x65], dh
        jb     0x2b5
        jae    0x2fc
loc_2_0297:
        arpl   word ptr [edi + 0x6e], bp
        add    byte ptr fs:[eax], al
        inc    edx
        jns    0x314
        jae    loc_2_02C3
        jb     0x30a
        popal  
        das    
loc_2_02A8:
        jae    loc_2_030F
        arpl   word ptr [edi + 0x6e], bp
        add    byte ptr fs:[eax], al
loc_2_02B0:
        inc    edx
        push   edx
        popal  
        jae    0x30a
        arpl   word ptr gs:[eax], ax
        add    byte ptr [edx + 0x79], al
        je     0x324
        jae    loc_2_02E1
        jb     0x328
loc_2_02C3:
        popal  
        and    byte ptr fs:[esi + 0x72], ah
        outsd  dx, dword ptr [esi]
        insd   dword ptr es:[edi], dx
        and    byte ptr [eax + ebp*2 + 0x65], dh
        and    byte ptr [esi + 0x69], ah
        insb   byte ptr es:[edi], dx
        and    byte ptr gs:[ebx + 0x79], dh
        jae    0x34c
        insd   dword ptr es:[edi], dx
        and    byte ptr [eax + 0x65], dh
        jb     loc_2_02FF
        jae    0x346
loc_2_02E1:
        arpl   word ptr [edi + 0x6e], bp
        add    byte ptr fs:[eax], al
        inc    edx
        jns    0x35e
        jae    loc_2_030D
        ja     loc_2_0361
        imul   esi, dword ptr [esp + esi*2 + 0x65], 0x65732f6e
        arpl   word ptr [edi + 0x6e], bp
        add    byte ptr fs:[eax], al
        inc    edx
        push   edi
loc_2_02FF:
        jb     0x36a
        je     0x368
        jae    0x358
        arpl   word ptr gs:[eax], ax
        add    byte ptr [edx + 0x79], al
        je     0x372
loc_2_030D:
        jae    0x32f
loc_2_030F:
        ja     0x383
        imul   esi, dword ptr [esp + esi*2 + 0x65], 0x6f74206e
        and    byte ptr [eax + ebp*2 + 0x65], dh
        and    byte ptr [esi + 0x69], ah
        insb   byte ptr es:[edi], dx
        and    byte ptr gs:[ebx + 0x79], dh
        jae    loc_2_039B
        insd   dword ptr es:[edi], dx
        and    byte ptr [eax + 0x65], dh
        jb     loc_2_034E
        jae    0x395
        arpl   word ptr [edi + 0x6e], bp
        add    byte ptr fs:[eax], al
        inc    esp
        imul   esi, dword ptr [edx + 0x74], 0x61642079
        je     loc_2_03A1
        add    byte ptr [eax], al
        inc    esp
        imul   esi, dword ptr [edx + 0x74], 0x74614479
        popal  
        add    byte ptr [eax], al
        push   esp
loc_2_034E:
        push   0x756e2065
        insd   dword ptr es:[edi], dx
        bound  esp, qword ptr [ebp + 0x72]
        and    byte ptr [edi + 0x66], ch
        and    byte ptr [edx + 0x79], ah
        je     0x3c4
        jae    loc_2_0381
loc_2_0361:
        ja     0x3c4
        imul   esi, dword ptr [ecx + ebp*2 + 0x6e], 0x6f742067
        and    byte ptr [edx + 0x65], ah
        and    byte ptr [edi + 0x72], dh
        imul   esi, dword ptr [esp + esi*2 + 0x65], 0x6f74206e
        and    byte ptr [eax + ebp*2 + 0x65], dh
        and    byte ptr [ecx + ebp*2 + 0x73], ah
loc_2_0381:
        imul   ebp, dword ptr [esi], 0x20
        and    byte ptr [esi + 0x6f], cl
        je     0x3ee
        and    byte ptr [eax + ebp*2 + 0x61], dh
        je     0x3af
        or     eax, 0x7269640a
        je     0x40f
        and    byte ptr [ecx + 0x74], ah
        popal  
loc_2_039B:
        and    byte ptr [ecx + 0x73], ch
        and    byte ptr [ebx + 0x65], ch
loc_2_03A1:
        jo     loc_2_0417
        and    byte ptr [edi + 0x6e], ch
        and    byte ptr [ecx + 0x20], ah
        jo     0x410
        jb     0x3cd
        arpl   word ptr [ecx + 0x63], sp
        push   0x6c622065
        outsd  dx, dword ptr [esi]
        arpl   word ptr [ebx + 0x20], bp
        bound  esp, qword ptr [ecx + 0x73]
        imul   esi, dword ptr [ebx + 0x2c], 0x646e6120
        and    byte ptr [ebx + 0x6f], dh
        and    byte ptr [ecx + 0x20], ah
        outsb  dx, byte ptr [esi]
        jne    0x439
        bound  esp, qword ptr [ebp + 0x72]
        and    byte ptr [0x6769680a], cl
        push   0x74207265
        push   0x74206e61
        push   0x63612065
        je     loc_2_045B
        popal  
        insb   byte ptr es:[edi], dx
        and    byte ptr [esi + 0x75], ch
        insd   dword ptr es:[edi], dx
        bound  esp, qword ptr [ebp + 0x72]
        and    byte ptr [edi + 0x66], ch
        and    byte ptr [edx + 0x79], ah
        je     0x45c
        jae    0x419
        ja     loc_2_046D
        imul   esi, dword ptr [esp + esi*2 + 0x65], 0x616d206e
        jns    0x425
        bound  esp, qword ptr [ebp + 0x20]
        jb     loc_2_046F
        jo     0x47b
        jb     0x482
        add    byte ptr cs:[eax], al
        and    esp, dword ptr [eax]
        dec    esp
        outsd  dx, dword ptr [esi]
loc_2_0417:
        popal  
        inc    ebx
        popal  
        arpl   word ptr [eax + 0x65], bp
        inc    edx
        insb   byte ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        arpl   word ptr [ebx + 0x73], bp
        add    byte ptr [eax], al
        dec    esp
        outsd  dx, dword ptr [esi]
        popal  
        inc    ebx
        popal  
        arpl   word ptr [eax + 0x65], bp
        push   ebx
        imul   edi, dword ptr [edx + 0x65], 0x754e0000
        insd   dword ptr es:[edi], dx
        bound  esp, qword ptr [ebp + 0x72]
        and    byte ptr [edi + 0x66], ch
        and    byte ptr [edx + 0x6c], ah
        outsd  dx, dword ptr [esi]
        arpl   word ptr [ebx + 0x73], bp
        and    byte ptr [ecx + 0x6e], ch
        and    byte ptr [edi + ebp*2 + 0x61], ch
        and    byte ptr fs:[ebx + 0x61], ah
        arpl   word ptr [eax + 0x65], bp
        add    byte ptr [ebx], ah
        and    byte ptr [ebp + 0x6d], al
        popal  
        outsb  dx, byte ptr [esi]
loc_2_045B:
        and    byte ptr fs:[edi + ebp*2 + 0x61], cl
        and    byte ptr fs:[esi + 0x61], al
        jne    0x4d2
        je     0x4db
        add    byte ptr [eax], al
        inc    esp
        insd   dword ptr es:[edi], dx
loc_2_046D:
        popal  
        outsb  dx, byte ptr [esi]
loc_2_046F:
        dec    esp
        outsd  dx, dword ptr [esi]
        popal  
        jae    loc_2_0476
loc_2_0476:
        add    byte ptr [esi + 0x75], cl
        insd   dword ptr es:[edi], dx
        bound  esp, qword ptr [ebp + 0x72]
        and    byte ptr [edi + 0x66], ch
        and    byte ptr [ebp + 0x6d], al
        popal  
        outsb  dx, byte ptr [esi]
        and    byte ptr fs:[edi + ebp*2 + 0x61], cl
        and    byte ptr fs:[eax + 0x61], dl
        and    byte ptr gs:[bp + 0x61], al
        jne    loc_2_0502
        je     loc_2_050B
        add    byte ptr [ebx], ah
        and    byte ptr [edi + ebp*2 + 0x61], cl
        inc    ebx
        popal  
        arpl   word ptr [eax + 0x65], bp
        dec    eax
        imul   esi, dword ptr [ebx + esi*2], 0x616f4c00
        inc    ebx
        popal  
        arpl   word ptr [eax + 0x65], bp
        dec    eax
        imul   esi, dword ptr [ebx + esi*2], 0x6d754e00
        bound  esp, qword ptr [ebp + 0x72]
        and    byte ptr [edi + 0x66], ch
        and    byte ptr [edx + 0x6c], ah
        outsd  dx, dword ptr [esi]
        arpl   word ptr [ebx + 0x73], bp
        and    byte ptr [ecx + 0x6e], ch
        and    byte ptr [edi + ebp*2 + 0x61], ch
        and    byte ptr fs:[ebx + 0x61], ah
        arpl   word ptr [eax + 0x65], bp
        add    byte ptr [ebx], ah
        and    byte ptr [ebp + 0x6d], al
        popal  
        outsb  dx, byte ptr [esi]
        dec    esp
        outsd  dx, dword ptr [esi]
        popal  
        push   edx
        popal  
        jae    0x50a
        sub    byte ptr [ecx + 0x6e], ch
        and    byte ptr [eax + 0x61], dl
        jae    0x51d
        add    byte ptr [eax], al
        dec    esp
        outsd  dx, dword ptr [esi]
        popal  
        inc    ebx
        popal  
        arpl   word ptr [eax + 0x65], bp
        push   edx
        popal  
loc_2_0502:
        add    byte ptr fs:[eax], al
        dec    esi
        jne    0x575
        bound  esp, qword ptr [ebp + 0x72]
loc_2_050B:
        and    byte ptr [edi + 0x66], ch
        and    byte ptr [eax + 0x61], dl
        jae    loc_2_0535
        push   edx
        popal  
        and    byte ptr fs:[ecx + 0x6e], ch
        and    byte ptr [edx + 0x79], ah
        and    byte ptr [ebp + 0x6d], al
        popal  
        outsb  dx, byte ptr [esi]
        and    byte ptr fs:[edi + ebp*2 + 0x61], cl
        jb     loc_2_052E
loc_2_052E:
        and    esp, dword ptr [eax]
        dec    esp
        outsd  dx, dword ptr [esi]
        popal  
        inc    ebx
loc_2_0535:
        popal  
        arpl   word ptr [eax + 0x65], bp
        inc    esp
        imul   esi, dword ptr [ebx + 0x63], 0x73647261
        and    byte ptr [eax], ch
        imul   ebp, dword ptr [esi + 0x20], 0x65676150
        jae    0x575
        add    byte ptr [eax], al
        dec    esp
        outsd  dx, dword ptr [esi]
        popal  
        inc    ebx
        popal  
        arpl   word ptr [eax + 0x65], bp
        inc    esp
        jae    loc_2_05BD
        add    byte ptr [eax], al
        dec    esi
        jne    0x5cc
        bound  esp, qword ptr [ebp + 0x72]
        and    byte ptr [edi + 0x66], ch
        and    byte ptr [eax + 0x61], dl
        jae    loc_2_058C
        inc    esp
        imul   esi, dword ptr [ebx + 0x63], 0x65647261
        and    byte ptr fs:[esi + 0x72], ah
        outsd  dx, dword ptr [esi]
        insd   dword ptr es:[edi], dx
        and    byte ptr [edi + ebp*2 + 0x61], cl
        inc    ebx
        popal  
        arpl   word ptr [eax + 0x65], bp
        add    byte ptr [ebx], ah
        and    byte ptr [edi + ebp*2 + 0x61], cl
        inc    ebx
loc_2_058C:
        popal  
        arpl   word ptr [eax + 0x65], bp
        inc    ebx
        insb   byte ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        jae    0x5fa
        inc    esp
        imul   esi, dword ptr [ebx + 0x63], 0x73647261
        and    byte ptr [eax], ch
        imul   ebp, dword ptr [esi + 0x20], 0x65676150
        jae    0x5d1
        add    byte ptr [eax], al
        dec    esp
        inc    ebx
        sub    eax, 0x736f6c43
        inc    esp
        jae    0x618
        add    byte ptr [eax], al
        dec    esi
        jne    0x627
        bound  esp, qword ptr [ebp + 0x72]
loc_2_05BD:
        and    byte ptr [edi + 0x66], ch
        and    byte ptr [eax + 0x61], dl
        jae    0x5e7
        inc    esp
        imul   esi, dword ptr [ebx + 0x63], 0x65647261
        and    byte ptr fs:[esi + 0x72], ah
        outsd  dx, dword ptr [esi]
        insd   dword ptr es:[edi], dx
        and    byte ptr [edi + ebp*2 + 0x61], cl
        inc    ebx
        popal  
        arpl   word ptr [eax + 0x65], bp
        or     eax, 0x2074610a
        arpl   word ptr [edi + ebp*2 + 0x73], bp
        and    byte ptr gs:[esi + 0x69], ah
        insb   byte ptr es:[edi], dx
        and    byte ptr gs:[ecx + ebp*2 + 0x6d], dh
        and    byte ptr gs:[0x614d000a], cl
        js     0x61b
        dec    esp
        outsd  dx, dword ptr [esi]
        popal  
        jb     loc_2_0622
        push   edx
        popal  
        add    byte ptr fs:[eax], al
        dec    ebp
        popal  
        js     0x658
        outsd  dx, dword ptr [esi]
        popal  
        inc    ebx
        popal  
        arpl   word ptr [eax + 0x65], bp
        push   edx
        popal  
        push   ebx
        imul   edi, dword ptr [edx + 0x65], 0x614d0000
        js     0x68f
loc_2_0622:
        imul   ebp, dword ptr [ebp + 0x61], 0x6f4c206c
        popal  
        inc    ebx
        insb   byte ptr es:[edi], dx
        jne    0x6a2
        je     0x696
        jb     0x653
        imul   ebp, dword ptr [esi + 0x20], 0x65676150
        jae    0x63c
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0xa34  R-X
        db  060h, 068h
        dd  loc_1_B1EC               ; fixup
        db  0E8h, 02Ch, 002h, 000h, 000h, 083h, 0C4h, 004h, 0B8h
        dd  obj1:0xB720              ; fixup
        db  08Bh, 040h, 008h, 03Dh
        dd  obj1:0xB720              ; fixup
        db  00Fh, 084h, 080h, 000h, 000h, 000h, 08Dh, 058h, 045h, 08Bh, 050h, 028h, 00Fh, 0B6h, 092h, 0ACh
        db  000h, 000h, 000h, 080h, 0C2h, 041h, 053h, 052h, 050h, 068h
        dd  loc_1_B204               ; fixup
        db  0E8h, 0F9h, 001h, 000h, 000h, 083h, 0C4h, 010h, 0FFh, 070h, 020h, 068h
        dd  loc_1_B21C               ; fixup
        db  0E8h, 0E9h, 001h, 000h, 000h, 083h, 0C4h, 008h, 0FFh, 070h, 02Ch, 068h
        dd  loc_1_B230               ; fixup
        db  0E8h, 0D9h, 001h, 000h, 000h, 083h, 0C4h, 008h, 068h
        dd  obj1:0xB240              ; fixup
        db  0E8h, 0CCh, 001h, 000h, 000h, 083h, 0C4h, 004h, 08Bh, 018h, 03Bh, 0D8h, 075h, 00Fh, 068h
        dd  loc_1_B254               ; fixup
        db  0E8h, 0B9h, 001h, 000h, 000h, 083h, 0C4h, 004h, 0EBh, 090h, 0FFh, 073h, 014h, 0FFh, 073h, 010h
        db  068h
        dd  obj1:0xB260              ; fixup
        db  0E8h, 0A4h, 001h, 000h, 000h, 083h, 0C4h, 00Ch, 08Bh, 01Bh, 03Bh, 0D8h, 075h, 0E7h, 0E9h, 072h
        db  0FFh, 0FFh, 0FFh, 061h, 0C3h, 000h
        dd  obj4:0x0114              ; fixup
        dd  loc_4_0308               ; fixup
        dd  loc_4_0131               ; fixup
        dd  loc_4_12AA               ; fixup
        dd  loc_4_013F               ; fixup
        dd  obj4:0x0328              ; fixup
        dd  obj4:0x014D              ; fixup
        dd  loc_4_0337               ; fixup
        dd  obj4:0x0162              ; fixup
        dd  loc_4_10BE               ; fixup
        dd  loc_4_0178               ; fixup
        dd  loc_4_1172               ; fixup
        dd  obj4:0x0192              ; fixup
        dd  obj4:0x11B0              ; fixup
        dd  loc_4_01A6               ; fixup
        dd  obj4:0x11C2              ; fixup
        dd  loc_4_01B9               ; fixup
        dd  obj4:0x11E3              ; fixup
        dd  loc_4_01C5               ; fixup
        dd  loc_4_12E7               ; fixup
        dd  loc_4_01D4               ; fixup
        dd  loc_4_0336               ; fixup
        dd  loc_4_01E3               ; fixup
        dd  obj4:0x0338              ; fixup
        dd  loc_4_01F9               ; fixup
        dd  obj4:0x1341              ; fixup
        dd  loc_4_020A               ; fixup
        dd  obj4:0x1351              ; fixup
        db  054h, 06Fh, 067h, 067h, 06Ch, 065h, 020h, 065h, 06Dh, 070h, 074h, 079h, 020h, 066h, 069h, 065h
        db  06Ch, 064h, 020h, 070h, 072h, 069h, 06Eh, 074h, 069h, 06Eh, 067h, 02Eh, 000h, 044h, 075h, 06Dh
        db  070h, 020h, 074h, 068h, 065h, 020h, 04Fh, 046h, 054h, 02Eh, 000h, 044h, 075h, 06Dh, 070h, 020h
        db  074h, 068h, 065h, 020h, 06Ch, 06Fh, 067h, 02Eh, 000h, 043h, 068h, 061h, 06Eh, 067h, 065h, 020h
        db  074h, 068h, 065h, 020h, 06Ch, 06Fh, 067h, 020h, 06Dh, 061h, 073h, 06Bh, 02Eh, 000h, 044h, 075h
        db  06Dh, 070h, 020h, 074h, 068h, 065h, 020h, 076h, 06Fh, 06Ch, 075h, 06Dh, 065h, 020h, 06Ch, 069h
        db  073h, 074h, 02Eh, 000h, 044h, 075h, 06Dh, 070h, 020h, 074h, 068h, 065h, 020h, 064h, 069h, 072h
        db  065h, 063h, 074h, 06Fh, 072h, 079h, 020h, 063h, 061h, 063h, 068h, 065h, 02Eh, 000h, 044h, 075h
        db  06Dh, 070h, 020h, 062h, 075h, 066h, 066h, 065h, 072h, 020h, 068h, 065h, 061h, 064h, 065h, 072h
        db  02Eh, 000h, 044h, 075h, 06Dh, 070h, 020h, 062h, 075h, 066h, 066h, 065h, 072h, 020h, 063h, 068h
        db  061h, 069h, 06Eh, 02Eh, 000h, 044h, 075h, 06Dh, 070h, 020h, 049h, 04Fh, 052h, 045h, 051h, 02Eh
        db  000h, 044h, 075h, 06Dh, 070h, 020h, 056h, 046h, 041h, 054h, 020h, 053h, 046h, 054h, 02Eh, 000h
        db  046h, 06Ch, 075h, 073h, 068h, 020h, 074h, 068h, 065h, 020h, 06Ch, 06Fh, 067h, 02Eh, 000h, 044h
        db  075h, 06Dh, 070h, 020h, 074h, 068h, 072h, 065h, 061h, 064h, 073h, 020h, 069h, 06Eh, 020h, 056h
        db  046h, 041h, 054h, 02Eh, 000h, 044h, 075h, 06Dh, 070h, 020h, 073h, 074h, 061h, 074h, 069h, 073h
        db  074h, 069h, 063h, 073h, 02Eh, 000h, 043h, 06Ch, 065h, 061h, 072h, 020h, 073h, 074h, 061h, 074h
        db  069h, 073h, 074h, 069h, 063h, 073h, 02Eh, 000h, 05Bh, 058h, 05Dh, 020h, 02Dh, 020h, 045h, 078h
        db  069h, 074h, 020h, 06Dh, 065h, 06Eh, 075h, 02Eh, 000h, 032h, 0D2h, 0B8h, 001h, 000h, 000h, 000h
        db  0CDh, 041h, 0C3h, 060h, 08Dh, 06Ch, 024h, 01Ch, 006h, 08Ch, 0DFh, 08Eh, 0C7h, 08Bh, 075h, 008h
        db  08Dh, 07Dh, 00Ch, 0B8h, 073h, 000h, 000h, 000h, 0CDh, 041h, 001h, 005h
        dd  obj4:0x13BC              ; fixup
        db  007h, 061h, 0C3h, 0B8h, 077h, 000h, 000h, 000h, 0B3h, 001h, 0CDh, 041h, 00Ah, 0E4h, 03Ch, 056h
        db  0B8h, 077h, 000h, 000h, 000h, 0B3h, 001h, 0CDh, 041h, 00Ah, 0E4h, 03Ch, 046h, 0B8h, 077h, 000h
        db  000h, 000h, 0B3h, 001h, 0CDh, 041h, 00Ah, 0E4h, 03Ch, 041h, 0B8h, 077h, 000h, 000h, 000h, 0B3h
        db  001h, 0CDh, 041h, 00Ah, 0E4h, 03Ch, 054h, 0B8h, 077h, 000h, 000h, 000h, 0B3h, 001h, 0CDh, 041h
        db  00Ah, 0E4h, 075h, 05Ah, 0B9h, 00Eh, 000h, 000h, 000h, 0BBh
        dd  loc_4_00A4               ; fixup
        db  08Bh, 003h, 0BAh, 04Fh, 000h, 000h, 000h, 02Bh, 0D1h, 050h, 052h, 068h
        dd  loc_1_BD90               ; fixup
        db  0E8h, 080h, 0FFh, 0FFh, 0FFh, 083h, 0C4h, 00Ch, 083h, 0C3h, 008h, 0E2h, 0E3h, 068h
        dd  obj4:0x021C              ; fixup
        db  068h
        dd  loc_1_BD9C               ; fixup
loc_3_02C6:
        mov    ebp, 0x69e80000
        db  0FFh, 0FFh, 0FFh, 083h, 0C4h, 008h, 0E8h, 057h, 0FFh, 0FFh, 0FFh, 03Ch, 01Bh, 075h, 002h, 0B0h
        db  058h, 050h, 068h
        dd  loc_1_BDA4               ; fixup
        db  0E8h, 050h, 0FFh, 0FFh, 0FFh, 083h, 0C4h, 008h, 03Ch, 061h, 072h, 004h, 02Ch, 061h, 0EBh, 002h
        db  02Ch, 041h, 03Ch, 017h, 074h, 00Eh, 03Ch, 00Eh, 073h, 09Ch, 00Fh, 0B6h, 0C0h, 0FFh, 014h, 0C5h
        dd  loc_4_00A8               ; fixup
        db  0F8h, 0C3h, 0B8h
        dd  obj4:0x13C1              ; fixup
        db  080h, 035h
        dd  loc_4_13C0               ; fixup
        db  001h, 075h, 003h, 083h, 0C0h, 004h, 050h, 068h
        dd  loc_1_BDAC               ; fixup
        db  0E8h, 013h, 0FFh, 0FFh, 0FFh, 083h, 0C4h, 008h, 0C3h, 068h
        dd  loc_1_BDD0               ; fixup
        db  0E8h, 005h, 0FFh, 0FFh, 0FFh, 083h, 0C4h, 004h, 0C3h, 0C3h, 0C3h, 0CDh, 020h, 001h, 000h, 001h
        db  000h, 0CDh, 020h, 00Dh, 001h, 001h, 000h, 08Bh, 0D7h, 08Bh, 035h
        dd  obj1:0xBD8C              ; fixup
        db  02Bh, 0C9h, 08Bh, 004h, 03Eh, 00Bh, 0C0h, 074h, 035h, 051h, 00Fh, 0B7h, 04Fh, 018h, 03Bh, 00Dh
        dd  obj1:0xB8B4              ; fixup
        db  074h, 014h, 050h, 051h, 057h, 06Ah, 020h, 068h
        dd  loc_1_BDF4               ; fixup
        db  0E8h, 0C6h, 0FEh, 0FFh, 0FFh, 083h, 0C4h, 014h, 0EBh, 012h, 050h, 051h, 057h, 06Ah, 02Ah, 068h
        dd  loc_1_BE1C               ; fixup
        db  0E8h, 0B2h, 0FEh, 0FFh, 0FFh, 083h, 0C4h, 014h, 059h, 041h, 0CDh, 020h, 013h, 001h, 001h, 000h
        db  03Bh, 0FAh, 075h, 0BAh, 0A1h
        dd  obj1:0xB8B0              ; fixup
        db  08Bh, 015h
        dd  obj1:0xB8AC              ; fixup
        db  052h, 050h, 068h
        dd  loc_1_BE44               ; fixup
        db  0E8h, 08Ch, 0FEh, 0FFh, 0FFh, 083h, 0C4h, 00Ch, 00Bh, 0C9h, 075h, 00Dh, 068h
        dd  loc_1_BE78               ; fixup
        db  0E8h, 07Bh, 0FEh, 0FFh, 0FFh, 083h, 0C4h, 004h, 0B8h
        dd  loc_4_13C7               ; fixup
        db  08Bh, 008h, 0E3h, 028h, 083h, 0C0h, 004h, 083h, 039h, 000h, 074h, 017h, 00Fh, 0B7h, 051h, 002h
        db  00Fh, 0B7h, 009h, 050h, 051h, 052h, 068h
        dd  loc_1_BE98               ; fixup
        db  0E8h, 053h, 0FEh, 0FFh, 0FFh, 083h, 0C4h, 010h, 040h, 080h, 078h, 0FFh, 000h, 075h, 0F9h, 0EBh
        db  0D4h, 0C3h, 039h, 000h, 000h, 000h, 007h, 056h, 06Fh, 06Ch, 054h, 061h, 062h, 000h, 09Ch, 000h
        db  004h, 000h, 00Fh, 056h, 06Fh, 06Ch, 05Fh, 061h, 063h, 063h, 06Ch, 075h, 073h, 046h, 072h, 065h
        db  065h, 000h, 048h, 000h, 001h, 000h, 00Eh, 056h, 06Fh, 06Ch, 05Fh, 041h, 063h, 074h, 069h, 076h
        db  065h, 046h, 041h, 054h, 000h, 0AEh, 000h, 00Bh, 040h, 00Ah, 056h, 06Fh, 06Ch, 05Fh, 04Ch, 061h
        db  062h, 065h, 06Ch, 000h, 0B9h, 000h, 001h, 000h, 00Bh, 056h, 06Fh, 06Ch, 05Fh, 042h, 064h, 04Ch
        db  06Ch, 065h, 06Eh, 000h, 054h, 000h, 004h, 000h, 00Bh, 056h, 06Fh, 06Ch, 05Fh, 042h, 06Fh, 06Fh
        db  074h, 049h, 044h, 000h, 00Ch, 000h, 004h, 000h, 00Ch, 056h, 06Fh, 06Ch, 05Fh, 042h, 075h, 066h
        db  053h, 06Bh, 065h, 077h, 000h, 040h, 000h, 004h, 000h, 00Eh, 056h, 06Fh, 06Ch, 05Fh, 063h, 063h
        db  06Ch, 075h, 073h, 046h, 072h, 065h, 065h, 000h, 05Ch, 000h, 004h, 000h, 00Eh, 056h, 06Fh, 06Ch
        db  05Fh, 043h, 068h, 069h, 053h, 065h, 072h, 069h, 061h, 06Ch, 000h, 038h, 000h, 004h, 000h, 00Eh
        db  056h, 06Fh, 06Ch, 05Fh, 063h
loc_3_0494:
        insb   byte ptr es:[edi], dx
        jne    0x50a
        inc    ecx
        insb   byte ptr es:[edi], dx
        insb   byte ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        arpl   word ptr [eax], ax
        dec    edx
        add    byte ptr [edx], al
        add    byte ptr [esi], cl
        push   esi
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        pop    edi
        inc    ebx
        insb   byte ptr es:[edi], dx
        jne    0x51d
        inc    edx
        insd   dword ptr es:[edi], dx
        popal  
        jae    loc_3_051A
        add    byte ptr [edi], al
        add    dword ptr [eax], eax
        orps   xmm5, xmmword ptr [edi + 0x6c]
        pop    edi
        inc    ebx
        insb   byte ptr es:[edi], dx
        jne    0x530
        inc    edx
        jae    0x528
        imul   esp, dword ptr [esi + 0x74], 0x4002c00
        add    byte ptr [esi + edx*2], cl
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        pop    edi
        arpl   word ptr [ebp + esi*2 + 0x73], bp
        dec    ebp
        popal  
        js     loc_3_04D5
loc_3_04D5:
        cmp    al, 0
        add    al, 0
        or     eax, 0x5f6c6f56
        arpl   word ptr [ebp + esi*2 + 0x73], bp
        push   edx
        outsd  dx, dword ptr [esi]
        outsd  dx, dword ptr [esi]
        je     loc_3_04E7
loc_3_04E7:
        and    al, 0
        add    dword ptr [eax], eax
        adc    byte ptr [esi + 0x6f], dl
        insb   byte ptr es:[edi], dx
        pop    edi
        inc    ebx
        insb   byte ptr es:[edi], dx
        jne    0x567
        je     loc_3_055B
        jb     0x545
        popal  
        jae    0x566
        add    byte ptr [0x11000100], ah
        push   esi
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        pop    edi
        inc    ebx
        insb   byte ptr es:[edi], dx
        jne    loc_3_057C
        je     0x570
        jb     loc_3_0560
        push   0x746669
        xor    byte ptr [eax], al
        add    al, 0
        or     al, 0x56
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
loc_3_051A:
        pop    edi
        arpl   word ptr [ebx + 0x65], si
        arpl   word ptr [esi + 0x41], ax
        push   esp
        add    byte ptr [eax + eax + 4], ch
        add    byte ptr [esi + edx*2], cl
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        pop    edi
        inc    esp
        imul   esi, dword ptr [edx + 0x74], 0x746e43
        lodsb  al, byte ptr [esi]
        add    byte ptr [ecx], al
        add    byte ptr [edx], cl
        push   esi
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        pop    edi
        inc    esp
        jb     0x5a9
        jbe    loc_3_05A7
        add    byte ptr [ebp + 0xe000100], ch
        push   esi
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        pop    edi
        inc    esp
        jb     loc_3_05B8
        jbe    loc_3_05B6
        dec    edi
        jb     loc_3_05BD
        add    byte ptr [bx + si + 0x400], ch
        add    byte ptr [edi], cl
loc_3_055B:
        push   esi
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        pop    edi
        inc    esi
loc_3_0560:
        inc    ecx
        push   esp
        inc    ebx
        push   0x65676e61
        jae    loc_3_056A
loc_3_056A:
        inc    esi
        add    byte ptr [ecx], al
        add    byte ptr [0x5f6c6f56], cl
        inc    esi
        popal  
        je     loc_3_05BA
        outsd  dx, dword ptr [esi]
        jne    loc_3_05E8
        je     loc_3_057C
loc_3_057C:
        add    byte ptr es:[edx], al
        add    byte ptr [0x5f6c6f56], cl
        inc    esi
        imul   esi, dword ptr [edx + 0x73], 0x74614674
        add    byte ptr [edx], dl
        add    al, byte ptr [eax]
        or     dl, byte ptr [esi + 0x6f]
        insb   byte ptr es:[edi], dx
        pop    edi
        inc    esi
        insb   byte ptr es:[edi], dx
        popal  
        jae    loc_3_059D
loc_3_059D:
        mov    byte ptr [eax], al
        add    al, 0
        push   cs
        push   esi
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        pop    edi
        inc    esi
loc_3_05A7:
        insb   byte ptr es:[edi], dx
        jne    0x61d
        push   0x64616548
        add    byte ptr [eax], bh
        add    al, 0
        push   cs
        push   esi
loc_3_05B6:
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
loc_3_05B8:
        pop    edi
        dec    esp
loc_3_05BA:
        popal  
        jae    loc_3_0631
loc_3_05BD:
        inc    ecx
        push   esp
        imul   ebp, dword ptr [ebp + 0x65], 0x4000800
        add    byte ptr [ebx], cl
        push   esi
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        pop    edi
        dec    ebp
        popal  
        js     loc_3_0623
        arpl   word ptr gs:[eax], ax
        mov    edx, 0xa000100
        push   esi
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        pop    edi
        dec    ebp
        imul   esp, dword ptr fs:[ecx], 0x40074
        push   cs
        push   esi
loc_3_05E8:
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        pop    edi
        dec    ebp
        jae    loc_3_0632
        popal  
        je     loc_3_0652
        inc    ecx
        add    byte ptr gs:[bx + si], dh
        add    al, 0
        push   cs
        push   esi
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        pop    edi
        dec    ebp
        jae    loc_3_064A
        insb   byte ptr es:[edi], dx
        inc    edx
        jne    loc_3_066D
        add    byte ptr [eax + 0xd000400], dl
        push   esi
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        pop    edi
        dec    esi
        popal  
        insd   dword ptr es:[edi], dx
        dec    eax
        popal  
        jae    loc_3_0681
        add    byte ptr [eax], dl
        add    byte ptr [eax + eax], al
        or     edx, dword ptr [esi + 0x6f]
        insb   byte ptr es:[edi], dx
        pop    edi
loc_3_0623:
        dec    edi
        jae    0x68d
        je     loc_3_062A
loc_3_062A:
        pushal 
        add    byte ptr [eax + eax], al
        or     dword ptr [esi + 0x6f], edx
loc_3_0631:
        insb   byte ptr es:[edi], dx
loc_3_0632:
        pop    edi
        jo     0x679
        push   eax
        inc    edx
        add    byte ptr [eax], dl
        add    al, byte ptr [eax]
        or     edx, dword ptr [esi + 0x6f]
        insb   byte ptr es:[edi], dx
        pop    edi
        push   edx
        popal  
        dec    ebp
        popal  
        jae    loc_3_06B2
        add    byte ptr [edi], cl
loc_3_064A:
        add    dword ptr [eax], eax
        or     edx, dword ptr [esi + 0x6f]
        insb   byte ptr es:[edi], dx
        pop    edi
        push   edx
loc_3_0652:
        popal  
        jae    loc_3_0698
        outsb  dx, byte ptr [esi]
        je     loc_3_0658
loc_3_0658:
        add    byte ptr fs:[eax + eax], al
        or     dl, byte ptr [esi + 0x6f]
        insb   byte ptr es:[edi], dx
        pop    edi
        push   edx
        inc    ebx
        outsb  dx, byte ptr [esi]
        je     loc_3_0667
loc_3_0667:
        mov    word ptr [eax], es
        add    al, 0
        or     al, 0x56
loc_3_066D:
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        pop    edi
        push   edx
        inc    esi
        insb   byte ptr es:[edi], dx
        jne    loc_3_06E9
        push   0x2004400
        add    byte ptr [eax], dl
        push   esi
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        pop    edi
loc_3_0681:
        push   edx
        outsd  dx, dword ptr [esi]
        outsd  dx, dword ptr [esi]
        je     0x6cb
        outsb  dx, byte ptr [esi]
        je     loc_3_06FB
        imul   esp, dword ptr [ebp + 0x73], 0x4005800
        add    byte ptr [esi + edx*2], cl
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        pop    edi
        push   edx
        outsd  dx, dword ptr [esi]
loc_3_0698:
        outsd  dx, dword ptr [esi]
        je     loc_3_06EE
        jne    loc_3_070A
        add    byte ptr [eax + eax + 0x560f0004], ah
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        pop    edi
        jae    loc_3_070E
        arpl   word ptr [edx + 0x6f], ax
        outsd  dx, dword ptr [esi]
        je     loc_3_06F0
        insb   byte ptr es:[edi], dx
        je     loc_3_06B2
loc_3_06B2:
        sbb    byte ptr [eax], al
        add    al, 0
        push   cs
        push   esi
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        pop    edi
        jae    0x722
        arpl   word ptr [esi + 0x61], ax
        je     0x707
        outsb  dx, byte ptr [esi]
        add    byte ptr fs:[eax], ch
        add    byte ptr [eax + eax], al
        or     eax, 0x5f6c6f56
        jae    0x735
        arpl   word ptr [esi + 0x69], ax
        jb     0x748
        je     loc_3_06D7
loc_3_06D7:
        mov    al, byte ptr [0xe000400]
        push   esi
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        pop    edi
        jae    0x747
        arpl   word ptr [esi + 0x53], ax
        dec    ecx
        outsb  dx, byte ptr [esi]
        outsw  dx, word ptr [esi]
loc_3_06E9:
        add    byte ptr [eax + eax], dh
        add    al, 0
loc_3_06EE:
        or     al, 0x56
loc_3_06F0:
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        pop    edi
        jae    loc_3_075A
        arpl   word ptr [edx + 0x6f], dx
        outsd  dx, dword ptr [esi]
        je     loc_3_06FB
loc_3_06FB:
        and    al, byte ptr [eax]
        add    al, byte ptr [eax]
        orps   xmm5, xmmword ptr [edi + 0x6c]
        pop    edi
        push   ebx
        arpl   word ptr gs:[edi + ebp*2 + 0x72], si
loc_3_070A:
        dec    ebp
        popal  
        jae    loc_3_0779
loc_3_070E:
        add    byte ptr [eax + eax + 1], cl
        add    byte ptr [eax], dl
        push   esi
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        pop    edi
        push   ebx
        arpl   word ptr gs:[edi + ebp*2 + 0x72], si
        push   ebx
        push   0x746669
        and    byte ptr [eax], al
        add    al, byte ptr [eax]
        orps   xmm5, xmmword ptr [edi + 0x6c]
        pop    edi
        push   ebx
        arpl   word ptr gs:[edi + ebp*2 + 0x72], si
        push   ebx
        imul   edi, dword ptr [edx + 0x65], 0x1001f00
        add    byte ptr [edx], cl
        push   esi
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        pop    edi
        push   ebx
        insb   byte ptr es:[edi], dx
        popal  
        add    byte ptr [di], cl
        add    dword ptr [eax], eax
        or     byte ptr [esi + 0x6f], dl
        insb   byte ptr es:[edi], dx
        pop    edi
        push   ebx
        push   eax
        inc    edx
        add    byte ptr [esi], cl
        add    dword ptr [eax], eax
        or     al, 0x56
loc_3_075A:
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        pop    edi
        push   ebx
        push   eax
        inc    edx
        dec    ebp
        popal  
        jae    loc_3_07CF
        add    byte ptr [eax + eax], bl
        add    al, byte ptr [eax]
        push   cs
        push   esi
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        pop    edi
        push   esp
        popal  
        inc    ecx
        arpl   word ptr [ecx + ebp*2 + 0x76], si
        add    byte ptr gs:[esi], bl
loc_3_0779:
        add    byte ptr [ecx], al
        add    byte ptr [edi], cl
        push   esi
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        pop    edi
        push   esp
        popal  
        inc    edx
        imul   esi, dword ptr [esi + ecx*2 + 0x65], 0x7c007478
        add    byte ptr [eax + eax], al
        or     al, 0x56
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        pop    edi
        push   esi
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        dec    ecx
        insb   byte ptr es:[edi], dx
        add    byte ptr gs:[eax + eax], dl
        add    al, 0
        or     byte ptr [esi + 0x6f], dl
        insb   byte ptr es:[edi], dx
        pop    edi
        push   esi
        jb     loc_3_0819
        add    byte ptr [eax], ch
        add    al, 0
        or     dl, byte ptr [esi + 0x6f]
        insb   byte ptr es:[edi], dx
        pop    edi
        push   edi
        jb     loc_3_07F9
        outsb  dx, byte ptr [esi]
        je     loc_3_07B9
loc_3_07B9:
        add    byte ptr [eax], 4
        add    byte ptr [ebx], cl
        push   esi
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
loc_3_07C1:
        pop    edi
        push   edi
        jb     0x80d
        popal  
        add    byte ptr fs:[eax + eax + 0x560b0004], al
loc_3_07CF:
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        pop    edi
loc_3_07D2:
        push   edi
        jb     loc_3_081D
        imul   ebp, dword ptr [esi + 0x74], 0x100bb00
        add    byte ptr [edx], cl
        push   esi
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        pop    edi
        push   edi
        jb     loc_3_0835
        jb     loc_3_0850
        add    byte ptr [ecx], dl
        add    byte ptr [eax], al
        add    byte ptr [eax], cl
        inc    edx
        push   ebp
        inc    esi
        dec    esi
        dec    edi
        inc    esp
        inc    ebp
        add    byte ptr [eax], cl
loc_3_07F6:
        add    byte ptr [eax + eax], al
loc_3_07F9:
        push   es
        inc    edx
        pop    edi
        push   ebx
        inc    ebp
        inc    ebx
        add    byte ptr [eax + eax], cl
        add    al, 0
        push   es
        inc    edx
        pop    edi
loc_3_0807:
        push   esi
        dec    edi
        dec    esp
        add    byte ptr [eax], dl
        add    byte ptr [eax + eax], al
        pop    es
        inc    edx
loc_3_0811:
        pop    edi
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        add    byte ptr [eax + eax], bl
loc_3_0819:
        add    al, 0
loc_3_081B:
        pop    es
        inc    edx
loc_3_081D:
        pop    edi
        dec    esi
        inc    ebp
        pop    eax
        push   esp
        add    byte ptr [eax], ah
        add    byte ptr [eax + eax], al
        pop    es
        inc    edx
        pop    edi
        push   eax
        push   edx
        inc    ebp
        push   esi
        add    byte ptr [edx], dh
        add    byte ptr [ecx], al
        add    byte ptr [edi], al
        inc    edx
loc_3_0835:
        pop    edi
        inc    esp
        dec    ecx
        push   edx
        push   esp
        add    byte ptr [ecx], ch
        add    byte ptr [ecx], al
        add    byte ptr [ebx], cl
        inc    edx
        pop    edi
        push   ebx
        inc    ebx
        push   edx
        pop    edi
        dec    ebp
        inc    ecx
        push   ebx
        dec    ebx
        add    byte ptr [0x9000100], ah
loc_3_0850:
        inc    edx
        pop    edi
loc_3_0852:
        inc    edx
        inc    ecx
        inc    esp
        push   ebx
        inc    ebp
        inc    ebx
        add    byte ptr [eax], ch
        add    byte ptr [ecx], al
        add    byte ptr [edi], al
        inc    edx
        pop    edi
        dec    esp
loc_3_0861:
        inc    ebx
        dec    esi
        push   esp
        add    byte ptr [ebx], ch
        add    byte ptr [ecx], al
        add    byte ptr [edi], al
        inc    edx
        pop    edi
        inc    esi
        dec    esp
        inc    ecx
        inc    edi
        add    byte ptr [eax + eax], ch
        add    al, 0
        pop    es
        inc    edx
        pop    edi
        push   edi
        inc    ecx
        dec    ecx
        push   esp
        add    byte ptr [eax + eax], dl
        add    al, 0
        or     dword ptr [edx + 0x5f], eax
        dec    esp
        inc    esp
loc_3_0886:
        push   esp
        dec    ecx
        dec    ebp
        inc    ebp
        add    byte ptr [eax], bl
        add    byte ptr [eax + eax], al
        or     dword ptr [edx + 0x5f], eax
        inc    esi
        inc    esp
        push   esp
        dec    ecx
        dec    ebp
        inc    ebp
        add    byte ptr [esi], ah
        add    byte ptr [edx], al
        add    byte ptr [esi], al
        inc    edx
        pop    edi
        push   esp
        inc    ecx
        inc    edi
        add    byte ptr [eax + eax], ah
        add    dword ptr [eax], eax
        or     al, byte ptr [edx + 0x5f]
        dec    ecx
        outsb  dx, byte ptr [esi]
        jbe    loc_3_0910
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [eax + eax + 0x30], 0x7000200
        inc    edx
        pop    edi
        dec    eax
        inc    ebx
        dec    esi
        push   esp
        add    byte ptr [ebx], dh
        add    byte ptr [ecx], al
        add    byte ptr [eax], cl
        inc    edx
        pop    edi
        inc    esi
        push   ebx
        inc    esp
        dec    ecx
        inc    esp
        add    byte ptr [edi], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], cl
        inc    edx
        push   ebp
        inc    esi
        push   ebx
        dec    eax
        dec    edi
        push   edx
        push   esp
        add    byte ptr [eax], cl
        add    byte ptr [eax + eax], al
        push   es
        inc    edx
        pop    edi
        push   ebx
        inc    ebp
        inc    ebx
        add    byte ptr [eax + eax], cl
        add    al, 0
        push   es
        inc    edx
        pop    edi
        push   esi
        dec    edi
        dec    esp
        add    byte ptr [eax], dl
        add    byte ptr [eax + eax], al
        pop    es
        inc    edx
        pop    edi
        inc    ecx
        inc    esp
        inc    esp
        push   edx
        add    byte ptr [edx], dh
        add    byte ptr [ecx], al
        add    byte ptr [edi], al
        inc    edx
        pop    edi
        inc    esp
        dec    ecx
        push   edx
        push   esp
        add    byte ptr [eax], ch
        add    byte ptr [ecx], al
        add    byte ptr [edi], al
        inc    edx
        pop    edi
        dec    esp
loc_3_0910:
        inc    ebx
        dec    esi
        push   esp
        add    byte ptr [ebx], ch
        add    byte ptr [ecx], al
        add    byte ptr [edi], al
        inc    edx
loc_3_091A:
        pop    edi
        inc    esi
        dec    esp
        inc    ecx
        inc    edi
        add    byte ptr [esi], ah
        add    byte ptr [edx], al
        add    byte ptr [esi], al
        inc    edx
        pop    edi
        push   esp
        inc    ecx
        inc    edi
        add    byte ptr [0xa000000], al
        inc    esp
        dec    ecx
        push   edx
        pop    edi
        inc    ebx
        inc    ecx
        inc    ebx
        dec    eax
        inc    ebp
        add    byte ptr [eax + eax], dl
        add    al, byte ptr [eax]
        pop    es
        inc    esp
        inc    ebx
        pop    edi
        dec    esp
        outsb  dx, byte ptr gs:[esi]
        add    byte ptr [eax + eax], al
        add    al, 0
        or     eax, dword ptr [ebx + eax*2 + 0x5f]
        arpl   word ptr [ebp + esi*2 + 0x73], bp
        inc    esp
        imul   esi, dword ptr [edx], 0x40008
        or     byte ptr [ebx + eax*2 + 0x5f], al
        push   eax
        popal  
        je     0x9ca
        add    byte ptr [eax + eax], cl
        add    al, 0
        or     byte ptr [ebx + eax*2 + 0x5f], al
        push   esp
        imul   ebp, dword ptr [ebp + 0x65], 0x4001000
        add    byte ptr [edx], cl
        inc    esp
        inc    ebx
        pop    edi
        push   esi
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        je     loc_3_09DE
        bound  eax, qword ptr [eax]
        sldt   word ptr [eax]
        add    byte ptr [esi], al
        dec    ecx
        dec    edi
        push   edx
        inc    ebp
        push   ecx
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], al
        or     ch, byte ptr [ecx + 0x72]
        pop    edi
        insb   byte ptr es:[edi], dx
        outsb  dx, byte ptr gs:[esi]
        je     loc_3_0A00
        add    byte ptr [eax + eax], al
        add    dword ptr [eax], eax
        or     dword ptr [ecx + 0x72], ebp
        pop    edi
        insb   byte ptr es:[edi], dx
        popal  
        jae    loc_3_09A7
loc_3_09A7:
        add    eax, 0x8000100
        imul   esi, dword ptr [edx + 0x5f], 0x72657375
        add    byte ptr [esi], al
        add    byte ptr [edx], al
        add    byte ptr [edi], al
        imul   esi, dword ptr [edx + 0x5f], 0x6e6673
        or     byte ptr [eax], al
        add    al, 0
        pop    es
        imul   esi, dword ptr [edx + 0x5f], 0x646970
        or     al, 0
        add    al, 0
        or     dword ptr [ecx + 0x72], ebp
        pop    edi
        jo     0xa46
        popal  
        je     0xa41
        add    byte ptr [eax], dl
        add    byte ptr [eax + eax], al
loc_3_09DE:
        or     byte ptr [ecx + 0x72], ch
        pop    edi
        popal  
        jne    0xa5d
        xor    dword ptr [eax], eax
        adc    al, 0
        add    al, 0
        or     byte ptr [ecx + 0x72], ch
        pop    edi
        popal  
        je     0xa54
        add    byte ptr [eax], bl
        add    byte ptr [edx], al
        add    byte ptr [ebx], cl
        imul   esi, dword ptr [edx + 0x5f], 0x6974706f
loc_3_0A00:
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        jae    loc_3_0A04
loc_3_0A04:
        sbb    al, byte ptr [eax]
        add    al, byte ptr [eax]
loc_3_0A08:
        or     dword ptr [ecx + 0x72], ebp
        pop    edi
loc_3_0A0C:
        jb     0xa81
        outsd  dx, dword ptr [esi]
        jb     loc_3_0A12
loc_3_0A12:
        sbb    al, 0
        add    al, 0
        push   es
        imul   esi, dword ptr [edx + 0x5f], 0x20006872
        add    byte ptr [eax + eax], al
        push   es
        imul   esi, dword ptr [edx + 0x5f], 0x24006866
        add    byte ptr [eax + eax], al
        pop    es
        imul   esi, dword ptr [edx + 0x5f], 0x736f70
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0x1420  R-X
        db  047h, 04Ah, 0EBh, 0E4h, 056h, 068h
        dd  loc_1_BEF0               ; fixup
        db  0E8h, 028h, 0F2h, 0FFh, 0FFh, 083h, 0C4h, 008h, 0F7h, 0C2h, 000h, 080h, 000h, 000h, 074h, 009h
        db  08Bh, 03Ch, 01Fh, 00Bh, 0FFh, 074h, 031h, 02Bh, 0FBh, 081h, 0E2h, 0FFh, 03Fh, 000h, 000h, 074h
        db  027h, 080h, 03Ch, 01Fh, 000h, 074h, 021h, 0FFh, 034h, 01Fh, 068h
        dd  obj1:0xBEF8              ; fixup
        db  0E8h, 0F9h, 0F1h, 0FFh, 0FFh, 083h, 0C4h, 008h, 047h, 04Ah, 0EBh, 0DEh, 00Fh, 0B6h, 0C0h, 003h
        db  0F0h, 049h, 074h, 063h, 0E9h, 007h, 0FFh, 0FFh, 0FFh, 068h
        dd  loc_1_BEFC               ; fixup
        db  0E8h, 0DBh, 0F1h, 0FFh, 0FFh, 083h, 0C4h, 004h, 00Fh, 0B6h, 0C0h, 003h, 0F0h, 049h, 074h, 049h
        db  0A1h
        dd  obj4:0x13BC              ; fixup
        db  083h, 0F8h, 034h, 073h, 023h, 033h, 0D2h, 0BFh, 01Ah, 000h, 000h, 000h, 0F7h, 0F7h, 0F7h, 0DAh
        db  083h, 0C2h, 01Ah, 068h
        dd  obj1:0xBF00              ; fixup
        db  0E8h, 0AEh, 0F1h, 0FFh, 0FFh, 083h, 0C4h, 004h, 04Ah, 075h, 0F0h, 0E9h, 0C5h, 0FEh, 0FFh, 0FFh
        db  068h
        dd  obj1:0xBF04              ; fixup
        db  0E8h, 099h, 0F1h, 0FFh, 0FFh, 083h, 0C4h, 004h, 0C7h, 005h
        dd  obj4:0x13BC              ; fixup
loc_4_00A4:
        adc    eax, dword ptr [eax]
        add    byte ptr [eax], al
loc_4_00A8:
        add    byte ptr [eax], al
        add    cl, ch
        test   eax, 0x68fffffe
        or     byte ptr [edi + 0x7de80000], bh
        int1   
        db  0FFh, 0FFh, 083h, 0C4h, 004h, 0C3h, 0B8h, 077h, 000h, 000h, 000h, 0B3h, 002h, 0CDh, 041h, 00Ah
        db  0E4h, 075h, 01Ch, 0BBh
        dd  loc_1_B748               ; fixup
        db  08Bh, 01Bh, 081h, 0FBh
        dd  loc_1_B748               ; fixup
        db  074h, 00Ch, 0BEh
        dd  loc_4_03F1               ; fixup
        db  0E8h, 052h, 0FEh, 0FFh, 0FFh, 0EBh, 0EAh, 0C3h, 0B8h, 078h, 000h, 000h, 000h, 0CDh, 041h, 083h
        db  0FBh, 019h, 076h, 00Bh, 0BEh
        dd  loc_4_03F1               ; fixup
        db  0E8h, 039h, 0FEh, 0FFh, 0FFh, 0C3h, 08Ah, 0D3h, 0BBh
        dd  loc_1_B748               ; fixup
        db  08Bh, 01Bh, 081h, 0FBh
        dd  loc_1_B748               ; fixup
        db  074h, 016h, 038h, 093h, 0ACh, 000h, 000h, 000h, 075h, 0EEh, 052h, 0BEh
        dd  loc_4_03F1               ; fixup
        db  0E8h, 014h, 0FEh, 0FFh, 0FFh, 05Ah, 0EBh, 0E0h, 0C3h
sub_4_0126:
        jecxz  loc_4_0171
loc_4_0128:
        movzx  eax, word ptr [esi]
        sub    ecx, eax
        push   ecx
        lea    ecx, [eax - 2]
loc_4_0131:
        add    esi, 2
        jecxz  loc_4_016C
        push   0xbf0c                        ; -> obj1:0xBF0C
        db  0E8h, 0F7h, 0F0h, 0FFh
loc_4_013F:
        inc    dword ptr [ebx + 0x7e8004c4]
        add    dword ptr [eax], eax
        je     loc_4_0156
        push   0xbf10                        ; -> obj1:0xBF10
        call   0xfffff237
        add    esp, 4
loc_4_0156:
        push   esi
        push   0xbf14                        ; -> obj1:0xBF14
        call   0xfffff237
        add    esp, 8
        add    esi, 2
        sub    ecx, 2
        ja     0x143
loc_4_016C:
        pop    ecx
        or     ecx, ecx
        jne    loc_4_0128
loc_4_0171:
        ret    
        db  08Bh, 01Dh
        dd  loc_1_B278               ; fixup
loc_4_0178:
        mov    ecx, dword ptr [0xb880]       ; -> loc_1_B880
loc_4_017E:
        cmp    word ptr [ebx + 0x14], 0
        je     loc_4_01AA
        push   ecx
        mov    esi, 0x92b                    ; -> loc_4_092B
        call   0xffffff36
        movzx  ecx, word ptr [ebx + 0x14]
        mov    esi, dword ptr [ebx + 8]
        call   sub_4_0126
        push   0xbf1c                        ; -> obj1:0xBF1C
        call   0xfffff237
loc_4_01A6:
        add    esp, 4
        pop    ecx
loc_4_01AA:
        add    ebx, 0x18
        loop   loc_4_017E
        ret    
        db  0B8h, 078h, 000h, 000h, 000h, 0CDh, 041h, 0BEh
        dd  obj4:0x07E8              ; fixup
loc_4_01B9:
        pop    es
        add    byte ptr [eax], al
        call   0xffffff36
        ret    
        db  08Ah, 025h
        dd  obj1:0xB6F8              ; fixup
loc_4_01C5:
        mov    dh, 0
        add    byte ptr [edx + 0x11d4], bh   ; -> loc_4_11D4
        VxDCall  Dev_048B.Service_0009h          ; cd2009008b04
        ret    
loc_4_01D4:
        pushal 
        mov    ebx, esi
        mov    esi, 0x8cc                    ; -> obj4:0x08CC
        call   0xffffff36
        popal  
        ret    
loc_4_01E3:
        mov    eax, 0x77
        mov    bl, 2
        int    0x41
        or     ah, ah
        jne    loc_4_020A
        sub    esp, 0x80
        mov    esi, esp
        db  0B8h
loc_4_01F9:
        jne    loc_4_01FB
loc_4_01FB:
        add    byte ptr [eax], al
        int    0x41
        mov    ebx, dword ptr [esi + 0x14]
        add    esp, 0x80
        jmp    loc_4_0211
loc_4_020A:
        mov    eax, 0x78
        int    0x41
loc_4_0211:
        mov    esi, 0x97f                    ; -> loc_4_097F
        call   0xffffff36
        add    ebx, 0x34
        mov    esi, 0xa4d                    ; -> loc_4_0A4D
        call   0xffffff36
        cmp    byte ptr [ebx + 2], 0x41
        jne    loc_4_0265
        test   word ptr [ebx + 4], 0x2000
        je     loc_4_0265
        mov    esi, dword ptr [ebx - 0x28]
        or     esi, esi
        je     loc_4_0265
        push   0xbf20                        ; -> obj1:0xBF20
        call   0xfffff237
        add    esp, 4
        movzx  ecx, word ptr [esi]
        sub    ecx, 4
        add    esi, 4
        db  0E8h, 0CEh, 0FEh, 0FFh
loc_4_0257:
        jmp    loc_1_BF2C
        db  0BFh, 000h, 000h, 0E8h, 0D5h, 0EFh, 0FFh, 0FFh, 083h, 0C4h, 004h
loc_4_0265:
        cmp    byte ptr [ebx + 2], 0x41
        jne    loc_4_0277
        mov    esi, 0xb9e                    ; -> obj4:0x0B9E
        call   0xffffff36
        jmp    loc_4_0287
loc_4_0277:
        cmp    byte ptr [ebx + 2], 0x59
        jne    loc_4_0287
        mov    esi, 0xa97                    ; -> loc_4_0A97
        call   0xffffff36
loc_4_0287:
        sub    esp, 0x10
        mov    edi, esp
        mov    esi, dword ptr [ebx - 0x14]
        call   sub_4_0361
        jb     loc_4_02A6
        mov    ecx, esp
        push   ecx
        push   0xbf30                        ; -> loc_1_BF30
        call   0xfffff237
        add    esp, 8
loc_4_02A6:
        add    esp, 0x10
        ret    
        db  0B8h, 077h, 000h, 000h, 000h, 0B3h, 002h, 0CDh, 041h, 00Ah, 0E4h, 00Fh, 084h, 045h, 0EDh, 0FFh
        db  0FFh, 0B8h, 078h, 000h, 000h, 000h, 0CDh, 041h, 0BEh
        dd  obj4:0x0C9A              ; fixup
        db  0E8h, 06Ah, 0FCh, 0FFh, 0FFh, 08Bh, 043h, 028h, 00Fh, 0B6h, 080h, 0ACh, 000h, 000h, 000h, 004h
        db  041h, 050h, 068h
        dd  obj1:0xBF44              ; fixup
        db  0E8h, 054h, 0EFh, 0FFh, 0FFh, 083h, 0C4h, 008h, 0C3h, 0B8h, 077h, 000h, 000h, 000h, 0B3h, 002h
        db  0CDh, 041h, 00Ah, 0E4h, 075h, 01Ah, 081h, 0ECh, 080h, 000h, 000h, 000h, 08Bh, 0F4h, 0B8h, 075h
        db  000h, 000h, 000h, 0CDh, 041h, 08Bh, 05Eh, 018h, 081h, 0C4h
loc_4_0308:
        add    byte ptr [eax], 0
        add    bl, ch
        pop    es
        mov    eax, 0x78
        int    0x41
        mov    esi, 0xe4c                    ; -> obj4:0x0E4C
        call   0xffffff36
        sub    esp, 0x10
        mov    edi, esp
        mov    esi, ebx
        call   sub_4_0361
        jb     loc_4_033D
        mov    eax, esp
        push   eax
        push   0xbf58                        ; -> obj1:0xBF58
        db  0E8h
loc_4_0336:
        std    
loc_4_0337:
        out    dx, al
        db  0FFh, 0FFh, 083h, 0C4h, 008h
loc_4_033D:
        add    esp, 0x10
        ret    
        db  0BBh
        dd  loc_1_B760               ; fixup
        db  0BEh
        dd  loc_4_0EF7               ; fixup
        db  0E8h, 0E6h, 0FBh, 0FFh, 0FFh, 0C3h, 0BFh
        dd  loc_1_B760               ; fixup
        db  0B9h, 004h, 000h, 000h, 000h, 033h, 0C0h, 0FCh, 0F3h, 0ABh, 0C3h
sub_4_0361:
        mov    ecx, 0x2c
        mov    eax, 0x79
        int    0x41
        or     ax, ax
        jne    loc_4_03B9
        mov    esi, dword ptr [esi + 8]
        mov    ecx, 0x68
        mov    eax, 0x79
        int    0x41
        or     ax, ax
        jne    loc_4_03B9
        cld    
        push   esi
        mov    esi, dword ptr [esi + 0x28]
        mov    ecx, 0xcc
        mov    eax, 0x79
        int    0x41
        pop    ecx
        or     ax, ax
        jne    loc_4_03B9
        movzx  eax, byte ptr [esi + 0xac]
        add    al, 0x41
        stosb  byte ptr es:[edi], al
        mov    al, 0x3a
        stosb  byte ptr es:[edi], al
        lea    esi, [ecx + 0x45]
        mov    ecx, 0xb
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    byte ptr [edi], 0
        clc    
loc_4_03B8:
        ret    
loc_4_03B9:
        stc    
        jmp    loc_4_03B8
        db  000h, 000h, 000h, 000h, 000h, 04Eh, 04Fh, 054h, 020h, 000h, 000h
        dd  obj1:0xB1D4              ; fixup
        db  046h, 06Ch, 077h, 05Fh, 044h, 065h, 073h, 063h, 000h
        dd  obj1:0xB8BC              ; fixup
        db  046h, 075h, 06Ch, 06Ch, 048h, 06Fh, 06Ch, 064h, 053h, 065h, 06Dh, 000h
        dd  obj1:0xB8B8              ; fixup
        db  046h, 075h, 06Ch, 06Ch, 04Ch, 06Fh, 063h, 06Bh, 053h
loc_4_03F1:
        insd   dword ptr es:[edi], dx
        add    ah, dh                        ; -> obj1:0xB7F4
        mov    bh, 0
        add    byte ptr [edi + 0x6f], al
        je     0x454
        popal  
        imul   esi, dword ptr [eax + eax - 0x30], 0x4e0000b1 ; -> obj1:0xB1D0
        inc    ecx
        inc    edx
        inc    esp
        jae    loc_4_046F
        add    al, cl                        ; -> obj1:0xB1C8
        mov    cl, 0
        add    byte ptr [edx + 0x64], dl
        bound  eax, qword ptr [esi + 0x44]
        jae    0x47c
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_046F:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_092B:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_097F:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_0A4D:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_0A97:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_0EF7:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx + 0xb6f825], cl ; -> obj1:0xB6F8
        add    ch, cl
        and    byte ptr [edi], cl
        add    byte ptr [ebx - 0x323c06fc], cl
        and    byte ptr [eax], al
        add    byte ptr [ebx + 0x3df77204], cl
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        jb     0x100d
        VxDCall  IFSMgr.IFSMgr_Get_Version       ; cd2000004000
        jb     0x100d
        cmp    eax, 0x22
        jb     0x100d
        mov    esi, 0                        ; -> loc_1_0000
        mov    ecx, 4
        VxDCall  Dev_048B.Service_0001h          ; cd2001008b04
        jb     0x100d
        mov    byte ptr [0xb6f8], al         ; -> obj1:0xB6F8
        push   0
        push   0
        sub    ecx, ecx
        VMMCall  VMM._GetVMPgCount               ; cd205c000100
        mov    dword ptr [0xb78a], ecx       ; -> obj1:0xB78A
        pop    eax
        pop    eax
        push   0x22
        push   1
        push   0
        push   0x40
        push   0x6258                        ; -> loc_1_6258
        push   0
        VxDCall  IFSMgr.Service_0076h            ; cd2076004000
        add    esp, 0x18
        cld    
        mov    eax, 0xb6fc                   ; -> obj1:0xB6FC
        mov    dword ptr [eax + 0x1c], eax
        mov    dword ptr [eax + 0x20], eax
        mov    dword ptr [0xb728], 0xb720    ; -> obj1:0xB728
        mov    dword ptr [0xb72c], 0xb720    ; -> obj1:0xB72C
        call   sub_4_12C0
        mov    eax, 0x7d0
        mov    esi, 0x8c27                   ; -> sub_1_8C27
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    eax, dword ptr [0xb6f0]       ; -> loc_1_B6F0
        mov    dword ptr [eax], 0xffffffff
        cld    
        mov    dword ptr [0xb81e], esi       ; -> obj1:0xB81E
        cmp    esi, 1
        ret    
        db  0E8h, 0A0h, 000h, 000h, 000h, 083h
loc_4_10BE:
        in     al, dx
        adc    byte ptr [ebp + 0x73c35], cl  ; -> obj3:0x073C
        add    byte ptr [ebp + 0x5657243c], cl
        push   0x80000002
        VMMCall  VMM._RegOpenKey                 ; cd2048010100
        add    esp, 0xc
        or     eax, eax
        jne    loc_4_1158
        lea    ebx, [0x91a]                  ; -> loc_3_091A
loc_4_10E3:
        mov    ecx, dword ptr [ebx + 4]
        or     ecx, ecx
        je     loc_4_1146
        mov    eax, dword ptr [ebx + 8]
        lea    edx, [esp + 4]
        lea    esi, [esp + 0xc]
        lea    edi, [esp + 8]
        mov    dword ptr [edi], ecx
        mov    ecx, dword ptr [esp]
        push   edi
        push   esi
        push   edx
        push   0
        push   eax
        push   ecx
        VMMCall  VMM._RegQueryValueEx            ; cd2051010100
        add    esp, 0x18
        or     eax, eax
        jne    loc_4_1141
        mov    eax, dword ptr [esp + 4]
        cmp    eax, dword ptr [ebx]
        je     loc_4_112A
        cmp    eax, 4
        jne    loc_4_1128
        cmp    dword ptr [ebx], 3
        jne    loc_4_1128
        cmp    dword ptr [ebx + 4], 4
loc_4_1128:
        jne    loc_4_1141
loc_4_112A:
        mov    edi, dword ptr [ebx + 0xc]
        mov    ecx, dword ptr [ebx + 4]
        sub    al, al
        rep stosb byte ptr es:[edi], al
        lea    esi, [esp + 0xc]
        mov    edi, dword ptr [ebx + 0xc]
        mov    ecx, dword ptr [esp + 8]
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_4_1141:
        add    ebx, 0x10
        jmp    loc_4_10E3
loc_4_1146:
        mov    eax, dword ptr [esp]
        push   eax
        VMMCall  VMM._RegCloseKey                ; cd2049010100
        add    esp, 4
        add    esp, 0x10
        clc    
        ret    
loc_4_1158:
        add    esp, 0x10
        stc    
        ret    
        db  083h, 0ECh, 038h, 08Dh, 005h
        dd  obj3:0x0768              ; fixup
        db  08Dh, 01Ch, 024h, 053h, 050h, 068h, 002h, 000h, 000h, 080h, 0CDh, 020h
loc_4_1172:
        dec    eax
        add    dword ptr [ecx], eax
        add    byte ptr [ebx - 0x3ff4f33c], al
        jne    loc_4_129D
        mov    dword ptr [esp + 0xc], 0
loc_4_1189:
        mov    dword ptr [esp + 4], 0
        mov    dword ptr [esp + 8], 0
        mov    dword ptr [esp + 0x10], 0
        mov    dword ptr [esp + 0x14], 0
loc_4_11A9:
        mov    dword ptr [esp + 0x1c], 0x12
        mov    esi, dword ptr [esp]
        mov    edi, dword ptr [esp + 0x10]
        lea    eax, [esp + 0x14]
        lea    ebx, [esp + 0x18]
        lea    ecx, [esp + 0x22]
        lea    edx, [esp + 0x1c]
        push   edx
        push   ecx
        push   ebx
        push   0
        push   eax
        push   0
        push   edi
        push   esi
        db  0CDh, 020h
loc_4_11D4:
        push   eax
        add    dword ptr [ecx], eax
        add    byte ptr [ebx - 0x77cdf3c], al
        add    byte ptr [eax + edx + 0x3d], dh
        ljmp   0x3d09:0x74000000
        add    eax, dword ptr [ecx]
        add    byte ptr [eax], al
        je     loc_4_1258
        jmp    loc_4_11A9
        db  0FFh, 044h, 024h, 010h, 083h, 07Ch, 024h, 004h, 020h, 073h, 0AEh, 083h, 07Ch, 024h, 018h, 003h
        db  075h, 0A7h, 08Bh, 04Ch, 024h, 01Ch, 083h, 0E9h, 002h, 076h, 09Eh, 083h, 0F9h, 010h, 077h, 099h
        db  00Fh, 0B7h, 044h, 024h, 022h, 08Dh, 004h, 001h, 03Dh, 000h, 002h, 000h, 000h, 077h, 08Ah, 066h
        db  089h, 04Ch, 024h, 020h, 001h, 04Ch, 024h, 008h, 0FFh, 044h, 024h, 004h, 083h, 03Dh
        dd  obj1:0xB894              ; fixup
        db  000h, 00Fh, 084h, 070h, 0FFh, 0FFh, 0FFh, 08Bh, 03Dh
        dd  obj1:0xB894              ; fixup
        db  0FFh, 007h, 08Bh, 07Ch, 024h, 00Ch, 083h, 0C1h, 004h, 08Dh, 074h, 024h, 020h, 0FCh, 0F3h, 0A4h
        db  089h, 07Ch, 024h, 00Ch, 0E9h, 051h, 0FFh, 0FFh, 0FFh
loc_4_1258:
        cmp    dword ptr [esp + 0xc], 0
        jne    loc_4_1290
        cmp    dword ptr [esp + 4], 0
        je     loc_4_1290
        imul   eax, dword ptr [esp + 4], 4
        add    eax, dword ptr [esp + 8]
        add    eax, 4
        call   sub_1_A1CE
        jb     loc_4_1290
        mov    dword ptr [0xb894], eax       ; -> obj1:0xB894
        mov    dword ptr [eax], 0
        lea    eax, [eax + 4]
        mov    dword ptr [esp + 0xc], eax
        jmp    loc_4_1189
loc_4_1290:
        mov    eax, dword ptr [esp]
        push   eax
        VMMCall  VMM._RegCloseKey                ; cd2049010100
        add    esp, 4
loc_4_129D:
        add    esp, 0x38
        ret    
        db  0BEh
        dd  loc_3_0886               ; fixup
loc_4_12A6:
        lodsd  eax, dword ptr [esi]
        or     eax, eax
        db  074h
loc_4_12AA:
        adc    al, 0x8b
        clc    
        lodsd  eax, dword ptr [esi]
        push   esi
        sub    esi, esi
        VMMCall  VMM.Get_Profile_Decimal_Int     ; cd20ab000100
        pop    esi
        mov    edx, eax
        lodsd  eax, dword ptr [esi]
        mov    dword ptr [eax], edx
        jmp    loc_4_12A6
        db  0C3h
sub_4_12C0:
        ret    
        db  000h, 000h, 000h, 0F8h, 0C3h, 0E8h, 0EDh, 0FDh, 0FFh, 0FFh, 073h, 005h, 0E8h, 0CFh, 0FFh, 0FFh
        db  0FFh, 0CDh, 020h, 051h, 000h, 040h, 000h, 083h, 038h, 006h, 072h, 079h, 08Bh, 050h, 014h, 089h
        db  015h
        dd  loc_1_B284               ; fixup
        db  08Bh
loc_4_12E7:
        push   eax
        sbb    byte ptr [ecx + 0xb28815], cl ; -> loc_1_B288
        add    byte ptr [ebx], ch
        test   byte ptr [edi + 0xb88435], 0  ; -> loc_1_B884
        or     esi, esi
        je     loc_4_1356
        cmp    dword ptr [0xb880], 0         ; -> loc_1_B880
        je     loc_4_1356
        or     esi, 1
        cmp    esi, dword ptr [0xb880]       ; -> loc_1_B880
        jb     loc_4_1356
        cmp    esi, 0x3ffd
        ja     loc_4_1356
        mov    edi, 0xffffffff
        mov    edx, edi
        mov    ecx, esi
loc_4_1320:
        inc    edi
        add    edx, 2
        sub    ecx, edx
        ja     loc_4_1320
        dec    edi
        sub    esi, 2
loc_4_132C:
        mov    ecx, 3
        inc    edi
        add    esi, 2
loc_4_1335:
        sub    edx, edx
        mov    eax, esi
        div    ecx
        or     edx, edx
        je     loc_4_132C
        inc    ecx
        cmp    ecx, edi
        jbe    loc_4_1335
        mov    eax, 0x10
        mul    esi
        mov    dword ptr [0xb27c], eax       ; -> loc_1_B27C
        mov    dword ptr [0xb884], esi       ; -> loc_1_B884
loc_4_1356:
        mov    eax, dword ptr [0xb880]       ; -> loc_1_B880
        cmp    eax, 0x10
        jae    loc_4_1367
        mov    al, 0x10
        mov    dword ptr [0xb880], eax       ; -> loc_1_B880
loc_4_1367:
        cmp    eax, 0x100
        jbe    loc_4_1378
        mov    eax, 0x100
        mov    dword ptr [0xb880], eax       ; -> loc_1_B880
loc_4_1378:
        mov    ecx, 0x18
        mul    ecx
        push   1
        push   eax
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        cmp    eax, 1
        jb     0x1467
        mov    dword ptr [0xb278], eax       ; -> loc_1_B278
        mov    eax, dword ptr [0xb898]       ; -> obj1:0xB898
        mov    edx, dword ptr [0xb89c]       ; -> obj1:0xB89C
        cmp    eax, 0x7ffb0000
        jne    loc_4_13B3
        cmp    edx, 0x7ffb0000
        je     loc_4_13E8
loc_4_13B3:
        shr    eax, 0xf
        shr    edx, 0xf
        cmp    eax, 0xfffe
        ja     loc_4_13E8
loc_4_13C0:
        cmp    eax, 0x8000
        jb     loc_4_13E8
loc_4_13C7:
        cmp    edx, 0xfffe
        ja     loc_4_13DB
        cmp    edx, 0x8000
        jb     loc_4_13DB
        cmp    edx, eax
        jbe    loc_4_13DD
loc_4_13DB:
        mov    edx, eax
loc_4_13DD:
        mov    dword ptr [0xb8a0], eax       ; -> obj1:0xB8A0
        mov    dword ptr [0xb8a4], edx       ; -> obj1:0xB8A4
loc_4_13E8:
        call   0x100e
        jb     0x1467
        VxDCall  BLOCKDEV.BlockDev_Get_Version   ; cd2000001000
        jb     0x1467
        call   0x16d0
        jb     0x1467
        mov    esi, 0xbd7a                   ; -> obj1:0xBD7A
        VMMCall  VMM.Install_Exception_Handler   ; cd20ef000100
        jb     0x1467
        VMMCall  VMM._AllocateThreadDataSlot     ; cd20ff000100
        or     eax, eax
        stc    
        je     0x1467
        mov    dword ptr [0xbd8c], eax       ; -> obj1:0xBD8C
        mov    esi, eax
        int    0x20
        db  001h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 5  vsize=0x12  R-X
        db  083h, 0FBh, 001h, 074h, 008h, 0B8h, 000h, 000h, 033h, 0DBh, 033h, 0F6h, 0C3h, 0B8h, 001h, 080h
        db  0EBh, 0F6h
VxD__LTEXT ENDS

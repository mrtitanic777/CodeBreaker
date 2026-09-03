; SERIAL -- reconstructed 32-bit disassembly (VxD / LE format)
; 4 objects, 2305 instructions recovered by recursive descent

Declare_Virtual_Device SERIAL, 1, 0, SERIAL_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 030000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x11ec  R-X
SERIAL_Control:
        cmp    eax, 0xb
        je     loc_1_0915
        cmp    eax, 0x1b
        je     loc_2_0000
        clc    
        ret    
sub_1_0014:
        push   esi
        lea    esi, [esi + 0x94]
        mov    dword ptr [esi], 0x30
        test   cl, 1
        je     loc_1_002C
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [esi + 4], eax
loc_1_002C:
        test   cl, 2
        je     loc_1_0037
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [esi + 8], eax
loc_1_0037:
        test   cl, 4
        je     loc_1_0042
        mov    eax, dword ptr [ebx + 0xc]
        mov    dword ptr [esi + 0xc], eax
loc_1_0042:
        test   cl, 8
        je     loc_1_004D
        mov    eax, dword ptr [ebx + 0x10]
        mov    dword ptr [esi + 0x10], eax
loc_1_004D:
        test   cl, 0x70
        je     loc_1_0064
        mov    al, byte ptr [ebx + 0x16]
        mov    byte ptr [esi + 0x16], al
        mov    al, byte ptr [ebx + 0x17]
        mov    byte ptr [esi + 0x17], al
        mov    al, byte ptr [ebx + 0x18]
        mov    byte ptr [esi + 0x18], al
loc_1_0064:
        test   cl, 0x80
        je     loc_1_006F
        mov    al, byte ptr [ebx + 0x19]
        mov    byte ptr [esi + 0x19], al
loc_1_006F:
        test   ch, 1
        je     loc_1_007A
        mov    al, byte ptr [ebx + 0x1a]
        mov    byte ptr [esi + 0x1a], al
loc_1_007A:
        test   ch, 2
        je     loc_1_0085
        mov    al, byte ptr [ebx + 0x1b]
        mov    byte ptr [esi + 0x1b], al
loc_1_0085:
        test   ch, 4
        je     loc_1_0090
        mov    al, byte ptr [ebx + 0x1c]
        mov    byte ptr [esi + 0x1c], al
loc_1_0090:
        test   ch, 8
        je     loc_1_009B
        mov    al, byte ptr [ebx + 0x1d]
        mov    byte ptr [esi + 0x1d], al
loc_1_009B:
        test   ch, 0x10
        je     loc_1_00A6
        mov    al, byte ptr [ebx + 0x1e]
        mov    byte ptr [esi + 0x1e], al
loc_1_00A6:
        test   ch, 0x20
        je     loc_1_00B1
        mov    eax, dword ptr [ebx + 0x20]
        mov    dword ptr [esi + 0x20], eax
loc_1_00B1:
        test   ch, 0x80
        je     loc_1_00BC
        mov    eax, dword ptr [ebx + 0x28]
        mov    dword ptr [esi + 0x28], eax
loc_1_00BC:
        test   ch, 0x40
        je     loc_1_00C7
        mov    eax, dword ptr [ebx + 0x24]
        mov    dword ptr [esi + 0x24], eax
loc_1_00C7:
        test   ecx, 0x10000
        je     loc_1_00D5
        mov    eax, dword ptr [ebx + 0x2c]
        mov    dword ptr [esi + 0x2c], eax
loc_1_00D5:
        mov    ax, word ptr [ebx + 0x14]
        mov    word ptr [esi + 0x14], ax
        pop    esi
        ret    
sub_1_00DF:
        xor    eax, eax
        xor    ecx, ecx
        call   sub_1_00EF
        not    al
        and    al, 0xb0
        xchg   al, ah
        dec    ecx
sub_1_00EF:
        cmp    dword ptr [ebx + 0x20], ecx
        jne    loc_1_00F6
        or     al, 0x80
loc_1_00F6:
        cmp    dword ptr [ebx + 0x24], ecx
        jne    loc_1_00FD
        or     al, 0x10
loc_1_00FD:
        cmp    dword ptr [ebx + 0x28], ecx
        jne    loc_1_0104
        or     al, 0x20
loc_1_0104:
        ret    
sub_1_0105:
        push   edx
        mov    ecx, dword ptr [ebx + 4]
        xor    eax, eax
        cmp    ecx, 0x1c200
        je     loc_1_0120
        cmp    ecx, 0xff10
        jae    loc_1_0132
        cmp    ecx, 2
        jb     loc_1_0129
loc_1_0120:
        xor    edx, edx
        mov    eax, 0x1c200
        div    ecx
loc_1_0129:
        mov    ecx, eax
        mov    eax, 0xfffffff4
        pop    edx
        ret    
loc_1_0132:
        cmp    ecx, 0xff1f
        ja     loc_1_0120
        push   ebx
        mov    ebx, ecx
        sub    ebx, 0xff10
        shl    ebx, 1
        movzx  eax, word ptr [ebx + 0x11a8]  ; -> obj1:0x11A8
        pop    ebx
        jmp    loc_1_0129
sub_1_014F:
        movzx  eax, word ptr [ebx + 0x16]
        cmp    ah, 4
        ja     loc_1_01A9
        mov    edi, 0xff1e
        or     ah, ah
        jne    loc_1_0167
        xor    edi, 0x404
loc_1_0167:
        cmp    al, 8
        ja     loc_1_01A2
        sub    al, 5
        jb     loc_1_01A2
        add    ah, ah
        je     loc_1_0175
        dec    ah
loc_1_0175:
        shl    ah, 3
        or     al, ah
        or     al, 4
        mov    ah, byte ptr [ebx + 0x18]
        or     ah, ah
        js     loc_1_01A9
        je     loc_1_0190
        sub    ah, 2
        je     loc_1_0192
        jns    loc_1_01A9
        test   al, 3
        jne    loc_1_01A9
loc_1_0190:
        and    al, 0xfb
loc_1_0192:
        push   edx
        mov    cl, byte ptr [ebx + 0x16]
        mov    edx, 0xff
        shl    edx, cl
        mov    ah, dh
        pop    edx
        clc    
        ret    
loc_1_01A2:
        mov    eax, 0xfffffff5
        stc    
        ret    
loc_1_01A9:
        mov    eax, 0xfffffffb
        stc    
        ret    
sub_1_01B0:
        or     byte ptr [esi + 0xf8], 0x40
        mov    byte ptr [esi + 0xfb], ah
        jmp    sub_1_099A
sub_1_01C2:
        or     ebx, ebx
        je     loc_1_0204
        mov    edx, dword ptr [esi + 0xf4]
        mov    al, byte ptr [edx]
        and    al, byte ptr [esi + 0xfe]
        xor    al, byte ptr [esi + 0xfe]
        shr    al, 4
        mov    ah, byte ptr [esi + 0xfc]
        and    ah, 0x18
        or     al, ah
        mov    ah, byte ptr [esi + 0xf8]
        and    ah, 0x60
        or     al, ah
        mov    ecx, dword ptr [esi + 0x4c]
        mov    edx, dword ptr [esi + 0x58]
        movzx  eax, al
        mov    dword ptr [ebx], eax
        mov    dword ptr [ebx + 4], ecx
        mov    dword ptr [ebx + 8], edx
loc_1_0204:
        ret    
sub_1_0205:
        push   edi
        mov    edx, dword ptr [esi + 0xc4]
        shl    ebx, 2
        cli    
        call   sub_1_1170
        sti    
        jb     loc_1_021C
        mov    eax, dword ptr [esi + 0x34]
loc_1_021C:
        pop    edi
        ret    
loc_1_021E:
        mov    ecx, 0xff40
        jmp    loc_1_022A
loc_1_0225:
        mov    ecx, 0xffffbf00
loc_1_022A:
        and    byte ptr [esi + 0xfc], ch
        or     byte ptr [esi + 0xfc], cl
        add    dl, 3
        in     al, dx
        and    al, ch
        or     al, cl
        jmp    loc_1_0240
loc_1_0240:
        out    dx, al
        clc    
        ret    
sub_1_0243:
        push   esi
        push   edi
        xor    eax, eax
        test   byte ptr [esi + 0x114], 0x40
        jne    loc_1_0259
        or     eax, dword ptr [esi + 0x34]
        jne    loc_1_033D
loc_1_0259:
        or     eax, dword ptr [esi + 0x4c]
        je     loc_1_0323
        cmp    ecx, eax
        jbe    loc_1_0268
        mov    ecx, eax
loc_1_0268:
        push   ecx
        mov    edx, dword ptr [esi + 0x40]
        mov    eax, dword ptr [esi + 0x50]
        sub    edx, eax
        cmp    edx, ecx
        jbe    loc_1_0277
        mov    edx, ecx
loc_1_0277:
        xchg   edx, ecx
        sub    edx, ecx
        push   esi
        mov    ebx, dword ptr [esi + 0x3c]
        mov    esi, ebx
        add    esi, eax
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    ecx, edx
        jecxz  loc_1_028E
        mov    esi, ebx
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_1_028E:
        sub    esi, ebx
        mov    ebx, esi
        pop    esi
        pop    ecx
        cli    
        mov    dword ptr [esi + 0x50], ebx
        sub    dword ptr [esi + 0x4c], ecx
        mov    eax, dword ptr [esi + 0x4c]
        and    byte ptr [esi + 0xcd], 0xfe
        sti    
        test   byte ptr [esi + 0x9c], 0x60
        je     loc_1_02BA
        test   byte ptr [esi + 0xfc], 0x12
        je     loc_1_031C
        jmp    loc_1_02C3
loc_1_02BA:
        test   byte ptr [esi + 0xfc], 0x12
        je     loc_1_031C
loc_1_02C3:
        cmp    eax, dword ptr [esi + 0xa0]
        ja     loc_1_031C
        mov    edx, dword ptr [esi + 0xc4]
        mov    ah, byte ptr [esi + 0xfd]
        push   ecx
        cli    
        mov    cl, byte ptr [esi + 0xfc]
        or     ah, ah
        je     loc_1_02F1
        add    dl, 4
        in     al, dx
        or     al, ah
        jmp    loc_1_02EB
loc_1_02EB:
        jmp    loc_1_02ED
loc_1_02ED:
        out    dx, al
        and    cl, 0xfd
loc_1_02F1:
        test   byte ptr [esi + 0x9c], 0x60
        je     loc_1_0304
        test   cl, 0x10
        je     loc_1_0314
        and    cl, 0xef
        jmp    loc_1_030C
loc_1_0304:
        test   cl, 0x10
        je     loc_1_0314
        and    cl, 0xef
loc_1_030C:
        or     cl, 4
        call   sub_1_099A
loc_1_0314:
        mov    byte ptr [esi + 0xfc], cl
        sti    
        pop    ecx
loc_1_031C:
        mov    eax, ecx
        or     esp, esp
        pop    edi
        pop    esi
        ret    
loc_1_0323:
        test   byte ptr [esi + 0x9c], 1
        jne    loc_1_033B
        mov    al, byte ptr [esi + 0xb0]
        test   byte ptr [esi + 0xf8], 0x20
        jne    loc_1_031C
loc_1_033B:
        xor    eax, eax
loc_1_033D:
        xor    ecx, ecx
        pop    edi
        pop    esi
        ret    
sub_1_0342:
        push   ecx
        call   sub_1_09CE
        pop    ecx
        push   ecx
        jne    loc_1_03C1
        mov    edx, dword ptr [esi + 0x48]
        sub    edx, dword ptr [esi + 0x58]
        jle    loc_1_03B1
        push   ecx
        cmp    ecx, edx
        jbe    loc_1_035B
        mov    ecx, edx
loc_1_035B:
        push   ecx
        push   esi
        push   edi
        mov    ebx, dword ptr [esi + 0x44]
        mov    edx, dword ptr [esi + 0x48]
        mov    edi, dword ptr [esi + 0x60]
        sub    edx, edi
        cmp    edx, ecx
        jbe    loc_1_036F
        mov    edx, ecx
loc_1_036F:
        xchg   ecx, edx
        sub    edx, ecx
        pop    esi
        add    edi, ebx
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    ecx, edx
        jecxz  loc_1_0381
        mov    edi, ebx
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_1_0381:
        sub    edi, ebx
        mov    edx, edi
        mov    edi, esi
        pop    esi
        pop    ebx
        cli    
        mov    dword ptr [esi + 0x60], edx
        add    dword ptr [esi + 0x58], ebx
        mov    edx, dword ptr [esi + 0x58]
        cmp    edx, dword ptr [esi + 0xe4]
        jb     loc_1_03A2
        and    byte ptr [esi + 0xcd], 0xfd
loc_1_03A2:
        call   sub_1_099A
        sti    
        pop    ecx
        sub    ecx, ebx
        jne    loc_1_03B8
loc_1_03AD:
        pop    eax
        sub    eax, ecx
        ret    
loc_1_03B1:
        cli    
        call   sub_1_099A
        sti    
loc_1_03B8:
        or     dword ptr [esi + 0x34], 0x100
        jmp    loc_1_03AD
loc_1_03C1:
        pop    eax
        sub    eax, ecx
        ret    
sub_1_03C5:
        push   edi
        mov    ecx, 0xc
        lea    edi, [esi + 0x4c]
        or     bl, bl
        jne    loc_1_03D7
        add    edi, ecx
        mov    edx, dword ptr [esi + 0x58]
loc_1_03D7:
        cld    
        shr    ecx, 2
        xor    eax, eax
        cli    
        rep stosd dword ptr es:[edi], eax
        sti    
        or     bl, bl
        je     loc_1_03F3
        call   sub_3_0000
        and    byte ptr [esi + 0xcd], 0xfe
        jmp    loc_1_040A
loc_1_03F3:
        or     edx, edx
        je     loc_1_040A
        cmp    dword ptr [esi + 0xe4], 0
        je     loc_1_040A
        mov    eax, 2
        call   sub_1_0A8C
loc_1_040A:
        pop    edi
        ret    
loc_1_040C:
        mov    esi, dword ptr [edx + 8]
        or     esi, esi
        je     loc_1_0431
        cmp    byte ptr [edx + 1], 1
        ja     loc_1_0438
        push   eax
        push   edx
        call   sub_1_0464
        pop    edx
        pop    eax
        jb     loc_1_0431
loc_1_0424:
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
        cmp    byte ptr [edx + 2], 0
        je     loc_1_0431
        ret    
loc_1_0431:
        stc    
        ret    
loc_1_0433:
        mov    esi, dword ptr [edx + 8]
        jmp    loc_1_043A
loc_1_0438:
        xor    ebx, ebx
loc_1_043A:
        push   eax
        push   edx
        inc    bh
        xor    bl, bl
loc_1_0440:
        push   ebx
        call   sub_1_0464
        pop    ebx
        jb     loc_1_044C
        or     bl, 1
loc_1_044C:
        mov    esi, dword ptr [esi + 0x130]
        or     esi, esi
        jne    loc_1_0440
        test   bl, bl
        pop    edx
        pop    eax
        jne    loc_1_0433
        dec    bh
        je     loc_1_0431
        jmp    loc_1_0424
        db  08Bh, 0C0h
sub_1_0464:
        test   byte ptr [esi + 0x3b], 1
        jne    loc_1_04F6
        mov    edx, dword ptr [esi + 0xc4]
        add    dl, 2
        in     al, dx
        test   al, 1
        jne    loc_1_04F6
        mov    ecx, dword ptr [esi + 0xf0]
        mov    ecx, dword ptr [ecx]
        push   ecx
        jmp    loc_1_04AC
loc_1_0487:
        cmp    dword ptr [esi + 0x58], 0
        je     loc_1_04A6
        pop    edx
        push   edx
        dec    edx
        in     al, dx
        and    al, 0xfd
        jmp    loc_1_0495
loc_1_0495:
        jmp    loc_1_0497
loc_1_0497:
        out    dx, al
        or     al, 2
        jmp    loc_1_049C
loc_1_049C:
        jmp    loc_1_049E
loc_1_049E:
        out    dx, al
        jmp    loc_1_04A1
loc_1_04A1:
        jmp    loc_1_04A3
loc_1_04A3:
        jmp    loc_1_04A5
loc_1_04A5:
        out    dx, al
loc_1_04A6:
        pop    edx
        in     al, dx
        test   al, 1
        jne    loc_1_04B7
loc_1_04AC:
        and    eax, 7
        push   edx
        jmp    loc_1_1160
loc_1_04B7:
        mov    eax, dword ptr [esi + 0xec]
        mov    ebx, dword ptr [esi + 0xf0]
        and    eax, dword ptr [ebx]
        mov    dword ptr [ebx], eax
        pop    ebx
        test   byte ptr [esi + 0xcd], 0x40
        je     loc_1_04E1
        not    ebx
        and    eax, ebx
        je     loc_1_04E1
        mov    eax, 4
        call   sub_1_0A8C
loc_1_04E1:
        mov    edx, dword ptr [esi + 0x107]
        or     dh, dh
        je     loc_1_04F4
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf000100
        mov    dword ptr [edx + 0x24], eax
loc_1_04F4:
        clc    
        ret    
loc_1_04F6:
        stc    
        ret    
loc_1_04F8:
        add    dl, 4
        in     al, dx
        mov    ebx, dword ptr [esi + 0xf4]
        mov    byte ptr [ebx], al
        mov    ch, al
        movzx  eax, al
        mov    ah, al
        shr    eax, 2
        shr    ah, 1
        shr    eax, 3
        and    eax, 0x138
        mov    ebx, dword ptr [esi + 0xf0]
        or     dword ptr [ebx], eax
        mov    ah, ch
        shr    ah, 2
        and    eax, 0xc00
        or     dword ptr [ebx], eax
        mov    ah, ch
        shr    ah, 3
        and    eax, 0x1000
        or     dword ptr [ebx], eax
        mov    ah, ch
        shl    ah, 3
        and    eax, 0x2000
        or     dword ptr [ebx], eax
        mov    al, byte ptr [esi + 0xfe]
        or     al, al
        je     loc_1_0560
        and    ch, al
        cmp    ch, al
        je     loc_1_0565
        or     byte ptr [esi + 0xfc], 0x20
loc_1_055B:
        jmp    loc_1_04A6
loc_1_0560:
        jmp    loc_1_0487
loc_1_0565:
        and    byte ptr [esi + 0xfc], 0x5f
        mov    ecx, dword ptr [esi + 0x58]
        jecxz  loc_1_055B
        jmp    loc_1_0774
loc_1_0576:
        mov    ebx, dword ptr [esi + 0xf0]
        or     byte ptr [ebx], 0x80
        add    dl, 3
        in     al, dx
        test   al, 0xe
        je     loc_1_058D
        mov    byte ptr [esi + 0x102], al
loc_1_058D:
        test   al, 0x10
        je     loc_1_0487
        or     byte ptr [ebx], 0x40
        jmp    loc_1_0487
loc_1_059D:
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf000100
        mov    ecx, dword ptr [esi + 0x12c]
        mov    dword ptr [ecx], eax
        sub    dl, 2
        in     al, dx
        and    byte ptr [esi + 0xcd], 0x7f
        mov    ah, byte ptr [esi + 0x102]
        mov    bh, ah
        or     ah, ah
        je     loc_1_05D2
        and    ah, byte ptr [esi + 0xf9]
        or     byte ptr [esi + 0x34], ah
        mov    byte ptr [esi + 0x102], 0
loc_1_05D2:
        mov    ecx, dword ptr [esi + 0xf0]
        or     byte ptr [ecx], 1
loc_1_05DB:
        mov    ecx, dword ptr [esi + 0x4c]
        cmp    ecx, dword ptr [esi + 0x40]
        jge    loc_1_0605
        test   byte ptr [esi + 0xf8], 0x20
        jne    loc_1_0605
        test   bh, 4
        je     loc_1_060B
        test   byte ptr [esi + 0x9d], 4
        je     loc_1_060B
        mov    al, byte ptr [esi + 0xaf]
        jmp    loc_1_06A1
loc_1_0605:
        or     dword ptr [esi + 0x34], 1
        jmp    loc_1_0634
loc_1_060B:
        and    al, byte ptr [esi + 0xfa]
        jne    loc_1_061C
        test   byte ptr [esi + 0x9d], 8
        jne    loc_1_0634
loc_1_061C:
        test   byte ptr [esi + 0x9c], 1
        jne    loc_1_0639
        cmp    al, byte ptr [esi + 0xb0]
        jne    loc_1_0639
        or     byte ptr [esi + 0xf8], 0x20
loc_1_0634:
        jmp    loc_1_0747
loc_1_0639:
        test   byte ptr [esi + 0x9d], 1
        je     loc_1_06A1
        cmp    al, byte ptr [esi + 0xae]
        jne    loc_1_067D
        or     byte ptr [esi + 0xfc], 8
        test   byte ptr [esi + 0x9c], 0x60
        je     loc_1_0634
        cmp    ecx, dword ptr [esi + 0xa0]
        ja     loc_1_0634
        and    byte ptr [esi + 0xfc], 0xf7
        and    byte ptr [esi + 0xfc], 0xeb
        mov    al, byte ptr [esi + 0xad]
        call   sub_1_0763
        jmp    loc_1_0634
loc_1_067D:
        cmp    al, byte ptr [esi + 0xad]
        jne    loc_1_06A1
        and    byte ptr [esi + 0xfc], 0xf7
        test   byte ptr [esi + 0x9c], 0x60
        je     loc_1_069C
        and    byte ptr [esi + 0xfc], 0xf7
loc_1_069C:
        jmp    loc_1_0774
loc_1_06A1:
        cmp    al, byte ptr [esi + 0xb1]
        jne    loc_1_06B2
        mov    ebx, dword ptr [esi + 0xf0]
        or     byte ptr [ebx], 2
loc_1_06B2:
        test   byte ptr [esi + 0x114], 1
        jne    loc_1_0634
        mov    edi, dword ptr [esi + 0x3c]
        mov    ebx, dword ptr [esi + 0x54]
        mov    byte ptr [edi + ebx], al
        inc    ebx
        cmp    ebx, dword ptr [esi + 0x40]
        jb     loc_1_06D0
        xor    ebx, ebx
loc_1_06D0:
        mov    dword ptr [esi + 0x54], ebx
        inc    ecx
        mov    dword ptr [esi + 0x4c], ecx
        cmp    ecx, dword ptr [esi + 0xe8]
        jb     loc_1_0720
        test   byte ptr [esi + 0xfc], 0x12
        jne    loc_1_0720
        mov    ah, byte ptr [esi + 0xfd]
        or     ah, ah
        je     loc_1_0705
        add    dl, 4
        in     al, dx
        not    ah
        and    al, ah
        or     byte ptr [esi + 0xfc], 2
        out    dx, al
        sub    dl, 4
loc_1_0705:
        test   byte ptr [esi + 0x9d], 2
        je     loc_1_0720
        or     byte ptr [esi + 0xfc], 0x10
        mov    al, byte ptr [esi + 0xae]
        call   sub_1_0763
loc_1_0720:
        cmp    ecx, dword ptr [esi + 0xe0]
        jb     loc_1_0740
        test   byte ptr [esi + 0xcd], 1
        jne    loc_1_0747
        push   0x747                         ; -> loc_1_0747
        mov    eax, 1
        jmp    sub_1_0A8C
loc_1_0740:
        and    byte ptr [esi + 0xcd], 0xfe
loc_1_0747:
        pop    edx
        push   edx
        add    dl, 3
        in     al, dx
        test   al, 1
        je     loc_1_075E
        sub    dl, 5
        jmp    loc_1_0756
loc_1_0756:
        jmp    loc_1_0758
loc_1_0758:
        in     al, dx
        jmp    loc_1_05DB
loc_1_075E:
        jmp    loc_1_0487
sub_1_0763:
        add    dl, 5
        mov    ah, al
loc_1_0768:
        in     al, dx
        test   al, 0x20
        je     loc_1_0768
        sub    dl, 5
        mov    al, ah
        out    dx, al
        ret    
loc_1_0774:
        pop    edx
        push   edx
        dec    dl
        in     al, dx
        test   al, 2
        jne    loc_1_0785
        or     al, 2
        out    dx, al
        jmp    loc_1_0782
loc_1_0782:
        jmp    loc_1_0784
loc_1_0784:
        out    dx, al
loc_1_0785:
        add    dl, 4
        jmp    loc_1_078A
loc_1_078A:
        jmp    loc_1_078C
loc_1_078C:
        in     al, dx
        sub    dl, 5
        test   al, 0x20
        jne    loc_1_07AA
        jmp    loc_1_04A6
loc_1_0799:
        add    dl, 3
        jmp    loc_1_079E
loc_1_079E:
        in     al, dx
        sub    dl, 5
        test   al, 0x20
        je     loc_1_083A
loc_1_07AA:
        mov    ah, byte ptr [esi + 0xfc]
        test   ah, 0x60
        jne    loc_1_0843
        test   byte ptr [esi + 0x9c], 0x60
        jne    loc_1_07D8
        test   ah, 5
        je     loc_1_07D8
        and    ah, 0xeb
        mov    al, byte ptr [esi + 0xad]
        mov    byte ptr [esi + 0xfc], ah
        jmp    loc_1_0847
loc_1_07D8:
        test   ah, 0x6d
        jne    loc_1_0843
        test   byte ptr [esi + 0xf8], 0x40
        je     loc_1_07F5
        and    byte ptr [esi + 0xf8], 0xbf
        mov    al, byte ptr [esi + 0xfb]
        jmp    loc_1_0847
loc_1_07F5:
        mov    ecx, dword ptr [esi + 0x58]
        jecxz  loc_1_083A
        test   byte ptr [esi + 0xf8], 0x92
        jne    loc_1_0810
        test   byte ptr [esi + 0x9c], 0x60
        je     loc_1_08E7
loc_1_0810:
        test   byte ptr [esi + 0x9c], 0x40
        je     loc_1_085A
        mov    ecx, dword ptr [esi + 0x103]
        cmp    ecx, dword ptr [esi + 0xa0]
        jne    loc_1_0851
        mov    dword ptr [esi + 0x103], 0
        or     byte ptr [esi + 0xfc], 8
        jmp    loc_1_087E
loc_1_083A:
        mov    ebx, dword ptr [esi + 0xf0]
        or     byte ptr [ebx], 4
loc_1_0843:
        inc    edx
        in     al, dx
        and    al, 0xfd
loc_1_0847:
        jmp    loc_1_0849
loc_1_0849:
        jmp    loc_1_084B
loc_1_084B:
        out    dx, al
        jmp    loc_1_04A6
loc_1_0851:
        inc    ecx
        mov    dword ptr [esi + 0x103], ecx
        jmp    loc_1_0897
loc_1_085A:
        test   byte ptr [esi + 0x9c], 0x20
        je     loc_1_0897
        mov    ecx, dword ptr [esi + 0x103]
        or     ecx, ecx
        jne    loc_1_0886
        mov    dword ptr [esi + 0x103], 1
        or     byte ptr [esi + 0xfc], 8
loc_1_087E:
        mov    al, byte ptr [esi + 0xae]
        jmp    loc_1_0847
loc_1_0886:
        inc    ecx
        cmp    ecx, dword ptr [esi + 0xa0]
        jne    loc_1_0891
        xor    ecx, ecx
loc_1_0891:
        mov    dword ptr [esi + 0x103], ecx
loc_1_0897:
        mov    edi, dword ptr [esi + 0x44]
        mov    ebx, dword ptr [esi + 0x5c]
        mov    al, byte ptr [edi + ebx]
        inc    ebx
        cmp    ebx, dword ptr [esi + 0x48]
        jb     loc_1_08A8
        xor    ebx, ebx
loc_1_08A8:
        mov    dword ptr [esi + 0x5c], ebx
        mov    ecx, dword ptr [esi + 0x58]
        dec    ecx
        mov    dword ptr [esi + 0x58], ecx
        out    dx, al
loc_1_08B3:
        mov    ebx, dword ptr [esi + 0xf0]
        or     dword ptr [ebx], 0x4000
        cmp    ecx, dword ptr [esi + 0xe4]
        jae    loc_1_04A6
        test   byte ptr [esi + 0xcd], 2
        jne    loc_1_04A6
        push   0x4a6                         ; -> loc_1_04A6
        mov    eax, 2
        jmp    sub_1_0A8C
loc_1_08E7:
        movzx  ecx, byte ptr [esi + 0x123]
        cmp    ecx, dword ptr [esi + 0x58]
        jbe    loc_1_08F6
        mov    ecx, dword ptr [esi + 0x58]
loc_1_08F6:
        sub    dword ptr [esi + 0x58], ecx
        mov    edi, dword ptr [esi + 0x44]
        mov    ebx, dword ptr [esi + 0x5c]
loc_1_08FF:
        mov    al, byte ptr [edi + ebx]
        out    dx, al
        inc    ebx
        cmp    ebx, dword ptr [esi + 0x48]
        jb     loc_1_090B
        xor    ebx, ebx
loc_1_090B:
        loop   loc_1_08FF
        mov    dword ptr [esi + 0x5c], ebx
        mov    ecx, dword ptr [esi + 0x58]
        jmp    loc_1_08B3
loc_1_0915:
        mov    esi, dword ptr [0x1158]       ; -> obj1:0x1158
        or     esi, esi
        je     loc_1_0941
        VMMCall  VMM.List_Get_First              ; cd20a3000100
loc_1_0925:
        je     loc_1_0941
        push   eax
        mov    eax, dword ptr [eax + 0x10c]
        or     eax, eax
        je     loc_1_0938
        VxDCall  VPICD.VPICD_Physically_Unmask   ; cd2009000300
loc_1_0938:
        pop    eax
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        jmp    loc_1_0925
loc_1_0941:
        clc    
        ret    
        db  000h
loc_1_0944:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 8]
        or     byte ptr [eax + 0x3b], 1
        jecxz  loc_1_0956
        and    byte ptr [eax + 0x3b], 0xfe
loc_1_0956:
        ret    
sub_1_0957:
        push   ebx
        push   ecx
        mov    eax, dword ptr [esi + 0x10c]
        or     eax, eax
        je     loc_1_0984
        mov    ebx, dword ptr [0x115c]       ; -> obj1:0x115C
        VxDCall  VPICD.VPICD_Get_Complete_Status ; cd2005000300
        mov    al, 0xff
        test   cl, 4
        jne    loc_1_0984
        mov    eax, dword ptr [esi + 0x10c]
        VxDCall  VPICD.VPICD_Physically_Mask     ; cd2008000300
        xor    al, al
loc_1_0984:
        pop    ecx
        pop    ebx
        ret    
sub_1_0987:
        push   eax
        mov    eax, dword ptr [esi + 0x10c]
        or     eax, eax
        je     loc_1_0998
        VxDCall  VPICD.VPICD_Physically_Unmask   ; cd2009000300
loc_1_0998:
        pop    eax
        ret    
sub_1_099A:
        test   byte ptr [esi + 0x3b], 1
        jne    loc_1_09B5
loc_1_09A0:
        mov    edx, dword ptr [esi + 0xc4]
        add    dl, 1
        in     al, dx
        test   al, 2
        jne    loc_1_09B4
        or     al, 2
        out    dx, al
        jmp    loc_1_09B3
loc_1_09B3:
        out    dx, al
loc_1_09B4:
        ret    
loc_1_09B5:
        call   sub_3_016A
        jne    loc_1_09A0
        xor    eax, eax
        mov    dword ptr [esi + 0x58], eax
        mov    dword ptr [esi + 0x103], eax
        mov    eax, dword ptr [esi + 0x5c]
        mov    dword ptr [esi + 0x60], eax
        ret    
sub_1_09CE:
        push   edi
        xor    edi, edi
loc_1_09D1:
        mov    ecx, 0xb
loc_1_09D6:
        xor    dh, dh
        mov    eax, dword ptr [esi + 0xf4]
        mov    al, byte ptr [eax]
        and    al, byte ptr [esi + 0xff]
        xor    al, byte ptr [esi + 0xff]
        je     loc_1_0A2F
        mov    ah, al
        shl    ah, 1
        jae    loc_1_09FF
        cmp    edi, dword ptr [esi + 0xb4]
        jb     loc_1_09FF
        or     dh, 0x80
loc_1_09FF:
        shl    ah, 1
        shl    ah, 1
        jae    loc_1_0A10
        cmp    edi, dword ptr [esi + 0xbc]
        jb     loc_1_0A10
        or     dh, 0x40
loc_1_0A10:
        shl    ah, 1
        jae    loc_1_0A1F
        cmp    edi, dword ptr [esi + 0xb8]
        jb     loc_1_0A1F
        or     dh, 0x20
loc_1_0A1F:
        or     dh, dh
        jne    loc_1_0A28
        loop   loc_1_09D6
        inc    edi
        jmp    loc_1_09D1
loc_1_0A28:
        xor    ah, ah
        mov    al, dh
        or     byte ptr [esi + 0x34], al
loc_1_0A2F:
        or     al, al
        pop    edi
        ret    
loc_1_0A33:
        or     byte ptr [esi + 0xfc], 8
loc_1_0A3A:
        clc    
        ret    
loc_1_0A3C:
        and    byte ptr [esi + 0xfc], 0xf7
        call   sub_1_099A
        clc    
        ret    
loc_1_0A4A:
        add    dl, 4
        in     al, dx
        or     al, 2
        jmp    loc_1_0A52
loc_1_0A52:
        out    dx, al
        clc    
        ret    
loc_1_0A55:
        add    dl, 4
        in     al, dx
        and    al, 0xfd
        jmp    loc_1_0A5D
loc_1_0A5D:
        out    dx, al
        clc    
        ret    
loc_1_0A60:
        add    dl, 4
        in     al, dx
        or     al, 1
        jmp    loc_1_0A68
loc_1_0A68:
        out    dx, al
        clc    
        ret    
loc_1_0A6B:
        add    dl, 4
        in     al, dx
        and    al, 0xfe
        jmp    loc_1_0A73
loc_1_0A73:
        out    dx, al
        clc    
        ret    
loc_1_0A76:
        sti    
        clc    
        ret    
loc_1_0A79:
        movzx  eax, byte ptr [esi + 0x115]
        shl    eax, 0x10
        mov    ax, word ptr [esi + 0xc4]
        stc    
        ret    
sub_1_0A8C:
        pushal 
        or     byte ptr [esi + 0xcd], al
        mov    ebx, dword ptr [esi + 0xf0]
        push   dword ptr [ebx]
        push   eax
        cmp    eax, 4
        jne    loc_1_0AB0
        push   dword ptr [esi + 0x110]
        push   esi
        call   dword ptr [esi + 0xc8]
        jmp    loc_1_0AD6
loc_1_0AB0:
        cmp    eax, 1
        jne    loc_1_0AC4
        push   dword ptr [esi + 0xd4]
        push   esi
        call   dword ptr [esi + 0xd0]
        jmp    loc_1_0AD6
loc_1_0AC4:
        cmp    eax, 2
        jne    loc_1_0AD6
        push   dword ptr [esi + 0xdc]
        push   esi
        call   dword ptr [esi + 0xd8]
loc_1_0AD6:
        add    esp, 0x10
        popal  
        ret    
loc_1_0ADB:
        mov    dword ptr [0x11e8], 0         ; -> obj1:0x11E8
        mov    esi, edx
        VMMCall  VMM.List_Get_First              ; cd20a3000100
loc_1_0AED:
        je     loc_1_0B31
        test   byte ptr [eax + 0x114], 0x20
        jne    loc_1_0B29
        cmp    dword ptr [eax + 0xe0], -1
        je     loc_1_0B29
        cmp    dword ptr [eax + 0x4c], 0
        je     loc_1_0B29
        test   byte ptr [eax + 0xcd], 1
        jne    loc_1_0B29
        xor    byte ptr [eax + 0xcd], 0x80
        js     loc_1_0B29
        push   eax
        push   esi
        mov    esi, eax
        mov    eax, 1
        call   sub_1_0A8C
        pop    esi
        pop    eax
loc_1_0B29:
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        jmp    loc_1_0AED
loc_1_0B31:
        mov    eax, 0x64
        mov    edx, esi
        mov    esi, 0xadb                    ; -> loc_1_0ADB
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    dword ptr [0x11e8], esi       ; -> obj1:0x11E8
        ret    
        db  000h, 000h
loc_1_0B4C:
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0xc]
        call   sub_1_01C2
        xor    eax, eax
        mov    dword ptr [esi + 0xc], eax
        inc    eax
        pop    ebx
        pop    esi
        leave  
        ret    
sub_1_0B66:
        enter  4, 0
        push   esi
        push   edi
        push   ebx
        cld    
        mov    esi, dword ptr [ebp + 8]
        test   byte ptr [esi + 0x3b], 1
        je     loc_1_0B87
        call   sub_3_016A
        mov    eax, 0xfffffffb
        je     loc_1_0DA1
loc_1_0B87:
        mov    ebx, dword ptr [ebp + 0xc]
        test   byte ptr [ebp + 0x10], 1
        je     loc_1_0B97
        call   sub_1_0105
        jecxz  loc_1_0BA4
loc_1_0B97:
        test   byte ptr [ebp + 0x10], 0x70
        je     loc_1_0BA9
        call   sub_1_014F
        jae    loc_1_0BA9
loc_1_0BA4:
        jmp    loc_1_0DA1
loc_1_0BA9:
        mov    edx, dword ptr [esi + 0xc4]
        inc    edx
        xor    eax, eax
        out    dx, al
        mov    ecx, dword ptr [ebx + 4]
        sub    ecx, dword ptr [esi + 0x98]
        mov    dword ptr [ebp - 4], ecx
        mov    ecx, dword ptr [ebp + 0x10]
        call   sub_1_0014
        lea    ebx, [esi + 0x94]
        test   byte ptr [ebp + 0x11], 0xe0
        je     loc_1_0BE1
        call   sub_1_00DF
        xchg   ah, al
        mov    word ptr [esi + 0xff], ax
loc_1_0BE1:
        add    dl, 2
        test   byte ptr [ebp + 0x10], 0x70
        je     loc_1_0C08
        call   sub_1_014F
        push   eax
        or     al, 0x80
        out    dx, al
        mov    byte ptr [esi + 0xfa], ah
        mov    eax, edi
        and    byte ptr [esi + 0x9c], ah
        mov    byte ptr [esi + 0xf9], al
        pop    eax
loc_1_0C08:
        test   byte ptr [ebp + 0x10], 1
        je     loc_1_0C25
        push   eax
        call   sub_1_0105
        sub    dl, 3
        mov    al, cl
        out    dx, al
        mov    al, ch
        inc    edx
        jmp    loc_1_0C1F
loc_1_0C1F:
        out    dx, al
        add    dl, 2
        pop    eax
        out    dx, al
loc_1_0C25:
        inc    dl
        test   byte ptr [ebp + 0x10], 2
        je     loc_1_0C68
        mov    al, byte ptr [esi + 0x9c]
        and    al, 0x82
        rol    al, 1
        shr    al, 1
        adc    al, 0
        mov    ah, al
        xor    al, 0xb
        out    dx, al
        mov    al, byte ptr [esi + 0x9d]
        rol    al, 1
        rol    al, 1
        rol    al, 1
        and    al, 3
        not    ah
        and    al, ah
        mov    byte ptr [esi + 0xfd], al
        mov    al, byte ptr [esi + 0x9c]
        shl    al, 1
        and    al, 0x30
        mov    byte ptr [esi + 0xfe], al
loc_1_0C68:
        mov    eax, dword ptr [esi + 0x40]
        sub    eax, dword ptr [esi + 0xa4]
        mov    dword ptr [esi + 0xe8], eax
        test   byte ptr [esi + 0x114], 0x80
        je     loc_1_0C8E
        cmp    dword ptr [ebp - 4], 0
        jne    loc_1_0C8E
        sub    dl, 4
        jmp    loc_1_0D32
loc_1_0C8E:
        sub    dl, 2
        xor    al, al
        out    dx, al
        mov    ah, 0x10
        add    dl, 3
loc_1_0C99:
        in     al, dx
        test   al, 1
        je     loc_1_0CA9
        sub    dl, 5
        in     al, dx
        add    dl, 5
        dec    ah
        jne    loc_1_0C99
loc_1_0CA9:
        and    byte ptr [esi + 0xf8], 0x7f
        sub    dl, 3
        test   byte ptr [esi + 0xf8], 2
        jne    loc_1_0D02
        mov    eax, dword ptr [esi + 0x98]
        cmp    eax, 0x12c0
        jb     loc_1_0D02
        cmp    ah, 0xff
        jne    loc_1_0CD5
        cmp    eax, 0xff15
        jb     loc_1_0D02
loc_1_0CD5:
        mov    al, 7
        or     al, byte ptr [esi + 0x122]
        out    dx, al
        test   byte ptr [esi + 0xf8], 4
        jne    loc_1_0D0C
        jmp    loc_1_0CE9
loc_1_0CE9:
        in     al, dx
        or     byte ptr [esi + 0xf8], 4
        test   al, 0x80
        je     loc_1_0CF9
        test   al, 0x40
        jne    loc_1_0D0C
loc_1_0CF9:
        jmp    loc_1_0CFB
loc_1_0CFB:
        or     byte ptr [esi + 0xf8], 2
loc_1_0D02:
        or     byte ptr [esi + 0xf8], 0x80
        xor    al, al
        out    dx, al
loc_1_0D0C:
        sub    dl, 2
        VMMCall  VMM.Get_System_Time             ; cd203f000100
        mov    ecx, eax
loc_1_0D17:
        in     al, dx
        mov    eax, 0x64
        VMMCall  VMM.Time_Slice_Sleep            ; cd2061010100
        VMMCall  VMM.Get_System_Time             ; cd203f000100
        sub    eax, ecx
        cmp    eax, 0xc8
        jb     loc_1_0D17
loc_1_0D32:
        add    dl, 6
        in     al, dx
        jmp    loc_1_0D38
loc_1_0D38:
        in     al, dx
        push   esi
        mov    esi, dword ptr [esi + 0xf4]
        mov    byte ptr [esi], al
        pop    esi
        mov    byte ptr [esi + 0xfc], 0
        test   byte ptr [esi + 0x9d], 0x80
        jne    loc_1_0D8B
        mov    eax, dword ptr [esi + 0xf4]
        mov    al, byte ptr [eax]
        mov    ah, byte ptr [esi + 0xfe]
        and    al, ah
        cmp    al, ah
        je     loc_1_0D8B
        cmp    ah, 0x30
        jne    loc_1_0D84
        test   byte ptr [esi + 0xf8], 1
        jne    loc_1_0D84
        cmp    al, 0x10
        jne    loc_1_0D84
        and    ah, 0xdf
        mov    byte ptr [esi + 0xfe], ah
        jmp    loc_1_0D8B
loc_1_0D84:
        or     byte ptr [esi + 0xfc], 0xa0
loc_1_0D8B:
        sub    dl, 5
        mov    al, 0xd
        cli    
        out    dx, al
        add    dl, 4
        jmp    loc_1_0D97
loc_1_0D97:
        in     al, dx
        sub    dl, 5
        jmp    loc_1_0D9D
loc_1_0D9D:
        in     al, dx
        sti    
        xor    eax, eax
loc_1_0DA1:
        mov    dword ptr [esi + 0xc], eax
        or     eax, eax
        jne    loc_1_0DB3
        or     byte ptr [esi + 0x114], 0x80
        or     al, 1
        jmp    loc_1_0DB5
loc_1_0DB3:
        xor    eax, eax
loc_1_0DB5:
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
loc_1_0DBA:
        push   edi
        mov    edx, dword ptr [esp + 8]
        lea    edi, [edx + 0x4c]
        mov    ecx, 6
        xor    eax, eax
        mov    dword ptr [edx + 0xc], eax
        rep stosd dword ptr es:[edi], eax
        mov    edi, edx
        mov    eax, dword ptr [esp + 0xc]
        or     eax, eax
        jne    loc_1_0E16
        test   byte ptr [edi + 0x114], 8
        je     loc_1_0DFD
        push   0
        push   dword ptr [esp + 0x14]
        push   dword ptr [edi + 0x3c]
        VMMCall  VMM._HeapReAllocate             ; cd2050000100
        add    esp, 0xc
        or     eax, eax
        je     loc_1_0E9D
        jne    loc_1_0E16
loc_1_0DFD:
        push   0
        push   dword ptr [esp + 0x14]
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        pop    ecx
        pop    ecx
        or     eax, eax
        je     loc_1_0E62
        or     byte ptr [edi + 0x114], 8
loc_1_0E16:
        mov    dword ptr [edi + 0x3c], eax
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [edi + 0x40], eax
        mov    eax, dword ptr [esp + 0x14]
        or     eax, eax
        jne    loc_1_0E8A
        cmp    dword ptr [esp + 0x18], 0
        je     loc_1_0E91
        test   byte ptr [edi + 0x114], 4
        je     loc_1_0E50
        push   0
        push   dword ptr [esp + 0x1c]
        push   dword ptr [edi + 0x44]
        VMMCall  VMM._HeapReAllocate             ; cd2050000100
        add    esp, 0xc
        or     eax, eax
        je     loc_1_0E9D
        jne    loc_1_0E8A
loc_1_0E50:
        push   0
        push   dword ptr [esp + 0x1c]
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        pop    ecx
        pop    ecx
        or     eax, eax
        jne    loc_1_0E83
loc_1_0E62:
        test   byte ptr [edi + 0x114], 8
        je     loc_1_0E7F
        and    byte ptr [edi + 0x114], 0xf7
        push   0
        push   dword ptr [edi + 0x3c]
        VMMCall  VMM._HeapFree                   ; cd2051000100
        pop    ecx
        pop    ecx
loc_1_0E7F:
        xor    eax, eax
        jmp    loc_1_0E9D
loc_1_0E83:
        or     byte ptr [edi + 0x114], 4
loc_1_0E8A:
        or     byte ptr [edi + 0x114], 0x10
loc_1_0E91:
        mov    dword ptr [edi + 0x44], eax
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [edi + 0x48], eax
        or     al, 1
loc_1_0E9D:
        pop    edi
        ret    
loc_1_0E9F:
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        mov    eax, 0x4000
        test   byte ptr [esi + 0xf8], 0x40
        jne    loc_1_0EC1
        mov    ah, byte ptr [ebp + 0xc]
        cli    
        call   sub_1_01B0
        sti    
        xor    eax, eax
loc_1_0EC1:
        mov    dword ptr [esi + 0xc], eax
        or     eax, eax
        mov    eax, 0xffffffff
        je     loc_1_0ECF
        xor    eax, eax
loc_1_0ECF:
        pop    ebx
        pop    esi
        leave  
        ret    
loc_1_0ED3:
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0xc]
        call   sub_1_01C2
        xor    eax, eax
        mov    dword ptr [esi + 0xc], eax
        xchg   dword ptr [esi + 0x34], eax
        mov    edx, dword ptr [ebp + 0x10]
        mov    dword ptr [edx], eax
        or     al, 1
        pop    ebx
        pop    esi
        leave  
        ret    
loc_1_0EF6:
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        mov    ebx, dword ptr [ebp + 0xc]
        mov    esi, dword ptr [ebp + 8]
        xor    eax, eax
        mov    dword ptr [esi + 0xc], 0xffffffec
        cmp    bl, 0xc8
        jne    loc_1_0F26
        mov    dword ptr [esi + 0xc], eax
        cmp    dword ptr [esi + 0x4c], eax
        je     loc_1_0F76
        mov    eax, dword ptr [esi + 0x3c]
        add    eax, dword ptr [esi + 0x50]
        mov    cl, byte ptr [eax]
        mov    eax, dword ptr [ebp + 0x14]
        mov    byte ptr [eax], cl
        jmp    loc_1_0F74
loc_1_0F26:
        cmp    bl, 0x15
        jne    loc_1_0F34
        and    byte ptr [esi + 0x114], 0xdf
        jmp    loc_1_0F74
loc_1_0F34:
        cmp    bl, 0x14
        jne    loc_1_0F42
        or     byte ptr [esi + 0x114], 0x40
        jmp    loc_1_0F74
loc_1_0F42:
        cmp    bl, 0x10
        jne    loc_1_0F50
        or     byte ptr [esi + 0x114], 0x20
        jmp    loc_1_0F74
loc_1_0F50:
        cmp    bl, 0x13
        jne    loc_1_0F60
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [esi + 0x12c], eax
        jmp    loc_1_0F74
loc_1_0F60:
        cmp    bl, 0xd
        ja     loc_1_0F76
        mov    dword ptr [esi + 0xc], eax
        call   sub_1_0205
        mov    ecx, dword ptr [ebp + 0x14]
        jecxz  loc_1_0F74
        mov    dword ptr [ecx], eax
loc_1_0F74:
        or     al, 1
loc_1_0F76:
        pop    ebx
        pop    esi
        leave  
        ret    
loc_1_0F7A:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        mov    dword ptr [edx + 0xc], 0
        mov    edx, dword ptr [edx + 0xf0]
        mov    ebx, dword ptr [ebp + 0xc]
        not    ebx
        cli    
        mov    eax, dword ptr [edx]
        and    ebx, eax
        mov    dword ptr [edx], ebx
        sti    
        mov    edx, dword ptr [ebp + 0x10]
        mov    dword ptr [edx], eax
        or     al, 1
        pop    ebx
        leave  
        ret    
loc_1_0FA5:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0x10]
        mov    edi, dword ptr [ebp + 0xc]
        test   byte ptr [esi + 0x114], 0x10
        jne    loc_1_0FEA
        cli    
        mov    dword ptr [esi + 0x44], edi
        mov    dword ptr [esi + 0x48], ecx
        mov    dword ptr [esi + 0x58], ecx
        xor    eax, eax
        mov    dword ptr [esi + 0x60], eax
        mov    dword ptr [esi + 0x5c], eax
        cmp    ecx, dword ptr [esi + 0xe4]
        jb     loc_1_0FDE
        and    byte ptr [esi + 0xcd], 0xfd
loc_1_0FDE:
        sti    
        push   ecx
        cli    
        call   sub_1_099A
        sti    
        pop    eax
        jmp    loc_1_0FF1
loc_1_0FEA:
        push   esi
        call   sub_1_0342
        pop    esi
loc_1_0FF1:
        mov    edx, dword ptr [ebp + 0x14]
        mov    dword ptr [edx], eax
        xor    eax, eax
        mov    dword ptr [esi + 0xc], eax
        inc    eax
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
loc_1_1001:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 0x10]
        call   sub_1_0243
        mov    ecx, 0
        jne    loc_1_1020
        mov    ecx, eax
        xor    eax, eax
loc_1_1020:
        mov    edx, dword ptr [ebp + 0x14]
        mov    dword ptr [edx], eax
        mov    dword ptr [esi + 0xc], ecx
        mov    eax, 0xffffffff
        jecxz  loc_1_1031
        xor    eax, eax
loc_1_1031:
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
sub_1_1036:
        mov    esi, dword ptr [0x1158]       ; -> obj1:0x1158
        VMMCall  VMM.List_Get_First              ; cd20a3000100
loc_1_1042:
        je     loc_1_105E
        test   byte ptr [eax + 0x114], 0x20
        jne    loc_1_1056
        cmp    dword ptr [eax + 0xe0], -1
        jne    loc_1_106E
loc_1_1056:
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        jne    loc_1_1042
loc_1_105E:
        xor    esi, esi
        xchg   dword ptr [0x11e8], esi       ; -> obj1:0x11E8
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
        jmp    loc_1_1091
loc_1_106E:
        cmp    dword ptr [0x11e8], 0         ; -> obj1:0x11E8
        jne    loc_1_1091
        mov    eax, 0x64
        mov    edx, esi
        mov    esi, 0xadb                    ; -> loc_1_0ADB
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    dword ptr [0x11e8], esi       ; -> obj1:0x11E8
        jmp    loc_1_1091
loc_1_1091:
        ret    
loc_1_1092:
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0xc]
        call   sub_1_03C5
        xor    eax, eax
        mov    dword ptr [esi + 0xc], eax
        inc    eax
        pop    ebx
        pop    esi
        leave  
        ret    
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  1, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'SERIAL  '                  ; DDB_Name
        dd  030000000h                  ; DDB_Init_Order
        dd  SERIAL_Control           ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        dd  sub_1_0B66               ; fixup
        dd  loc_3_04B8               ; fixup
        dd  loc_1_0DBA               ; fixup
        dd  loc_1_0E9F               ; fixup
        dd  loc_3_04DC               ; fixup
        dd  loc_1_0B4C               ; fixup
        dd  loc_1_0ED3               ; fixup
        dd  loc_3_0533               ; fixup
        dd  loc_3_0548               ; fixup
        dd  loc_1_0EF6               ; fixup
        dd  loc_1_1092               ; fixup
        dd  loc_3_0575               ; fixup
        dd  loc_1_0F7A               ; fixup
        dd  loc_1_0FA5               ; fixup
        dd  loc_1_1001               ; fixup
        dd  loc_3_0597               ; fixup
        dd  loc_3_05E8               ; fixup
        dd  loc_3_063E               ; fixup
        dd  loc_3_0690               ; fixup
        dd  loc_3_06AC               ; fixup
        dd  loc_3_06F2               ; fixup
        dd  loc_3_072C               ; fixup
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_1160:
        clc    
        add    al, 0
        add    byte ptr [ecx - 0x62fffff9], bl ; -> loc_1_0799
        add    eax, 0x5760000                ; -> loc_1_0576
        add    byte ptr [eax], al
sub_1_1170:
        cmp    cl, byte ptr [edx]
        add    byte ptr [eax], al
        xor    ecx, dword ptr [edx]
        add    byte ptr [eax], al
        cmp    al, 0xa
        add    byte ptr [eax], al
        dec    edx
        or     al, byte ptr [eax]
        add    byte ptr [ebp + 0xa], dl      ; -> loc_1_0A55
        add    byte ptr [eax], al
        pushal 
        or     al, byte ptr [eax]
        add    byte ptr [ebx + 0xa], ch      ; -> loc_1_0A6B
        add    byte ptr [eax], al
        jbe    loc_1_1198
        add    byte ptr [eax], al
        cmp    cl, byte ptr [edx]
        add    byte ptr [eax], al
        cmp    cl, byte ptr [edx]
        add    byte ptr [eax], al
loc_1_1198:
        jns    0x11a4
        add    byte ptr [eax], al
        jns    0x11a8
        add    byte ptr [eax], al
        push   ds
        add    al, byte ptr [eax]
        add    byte ptr [0x17000002], ah     ; -> loc_1_0225
        add    al, 0x80
        add    eax, eax
        add    byte ptr [eax], ah
        xor    byte ptr [eax], al
        sbb    byte ptr [eax], al
        or     al, 0
        or     byte ptr [eax], al
        push   es
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    ah, dh
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x58  R-X
loc_2_0000:
        cmp    dword ptr [0x115c], 0         ; -> obj1:0x115C
        jne    loc_2_0056
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    dword ptr [0x115c], ebx       ; -> obj1:0x115C
        VxDCall  BIOS.Service_0000h              ; cd2000002b00
        jb     loc_2_0057
        mov    eax, 4
        mov    ecx, 0x134
        VMMCall  VMM.List_Create                 ; cd209b000100
        jb     loc_2_0057
        mov    dword ptr [0x1158], esi       ; -> obj1:0x1158
        xor    eax, eax
        mov    ecx, 0xc
        VMMCall  VMM.List_Create                 ; cd209b000100
        jb     loc_2_0057
        mov    dword ptr [0x5c], esi         ; -> obj4:0x005C
        push   0x730                         ; -> loc_3_0730
        VxDCall  BIOS.Service_0001h              ; cd2001002b00
        pop    ecx
loc_2_0056:
        clc    
loc_2_0057:
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x945  R-X
sub_3_0000:
        push   esi
        push   edi
        test   byte ptr [esi + 0x9c], 0x60
        je     loc_3_0016
        test   byte ptr [esi + 0xfc], 0x12
        je     loc_3_0079
        jmp    loc_3_001F
loc_3_0016:
        test   byte ptr [esi + 0xfc], 0x12
        je     loc_3_0079
loc_3_001F:
        mov    eax, dword ptr [esi + 0x4c]
        cmp    eax, dword ptr [esi + 0xa0]
        ja     loc_3_0079
        mov    edx, dword ptr [esi + 0xc4]
        mov    ah, byte ptr [esi + 0xfd]
        cli    
        mov    cl, byte ptr [esi + 0xfc]
        or     ah, ah
        je     loc_3_004F
        add    dl, 4
        in     al, dx
        or     al, ah
        jmp    loc_3_0049
loc_3_0049:
        jmp    loc_3_004B
loc_3_004B:
        out    dx, al
        and    cl, 0xfd
loc_3_004F:
        test   byte ptr [esi + 0x9c], 0x60
        je     loc_3_0062
        test   cl, 0x10
        je     loc_3_0072
        and    cl, 0xef
        jmp    loc_3_006A
loc_3_0062:
        test   cl, 0x10
        je     loc_3_0072
        and    cl, 0xef
loc_3_006A:
        or     cl, 4
        call   sub_1_099A
loc_3_0072:
        mov    byte ptr [esi + 0xfc], cl
        sti    
loc_3_0079:
        xor    eax, eax
        or     eax, dword ptr [esi + 0x34]
        jne    loc_3_00C7
        or     eax, dword ptr [esi + 0x4c]
        je     loc_3_00AD
        mov    edi, dword ptr [esi + 0x3c]
        mov    ebx, dword ptr [esi + 0x50]
        mov    al, byte ptr [edi + ebx]
        inc    ebx
        cmp    ebx, dword ptr [esi + 0x40]
        jb     loc_3_0096
        xor    ebx, ebx
loc_3_0096:
        mov    dword ptr [esi + 0x50], ebx
        dec    dword ptr [esi + 0x4c]
        mov    ecx, dword ptr [esi + 0x4c]
        jae    loc_3_00A8
        and    byte ptr [esi + 0xcd], 0xfe
loc_3_00A8:
        or     esp, esp
        pop    edi
        pop    esi
        ret    
loc_3_00AD:
        test   byte ptr [esi + 0x9c], 1
        jne    loc_3_00C5
        mov    al, byte ptr [esi + 0xb0]
        test   byte ptr [esi + 0xf8], 0x20
        jne    loc_3_00A8
loc_3_00C5:
        xor    eax, eax
loc_3_00C7:
        xor    ecx, ecx
        pop    edi
        pop    esi
        ret    
sub_3_00CC:
        or     byte ptr [esi + 0x114], 1
        xor    ecx, ecx
        mov    dword ptr [esi + 0x34], ecx
        mov    dword ptr [esi + 0x4c], ecx
        call   0
        test   byte ptr [esi + 0xfc], 0x80
        jne    loc_3_010A
loc_3_00E9:
        mov    ecx, dword ptr [esi + 0x58]
        jecxz  loc_3_010F
        VMMCall  VMM.Get_System_Time             ; cd203f000100
        mov    edi, eax
loc_3_00F6:
        cmp    dword ptr [esi + 0x58], ecx
        jne    loc_3_00E9
        VMMCall  VMM.Get_System_Time             ; cd203f000100
        sub    eax, edi
        cmp    eax, 0xfa
        jb     loc_3_00F6
loc_3_010A:
        mov    ecx, 0xfffffffe
loc_3_010F:
        mov    edx, dword ptr [esi + 0xc4]
        call   sub_3_01B8
        mov    eax, ecx
        ret    
        db  000h, 000h, 000h
sub_3_0120:
        push   edx
        mov    ecx, dword ptr [esi + 0x124]
        test   ecx, ecx
        je     loc_3_014E
        mov    eax, dword ptr [esi + 0x128]
        test   eax, eax
        je     loc_3_014B
        push   1
        push   esi
        push   0x944                         ; -> loc_1_0944
        push   eax
        push   1
        call   ecx
        add    esp, 0x14
        mov    dword ptr [esi + 0x107], eax
loc_3_014B:
        cmp    eax, 1
loc_3_014E:
        pop    edx
        ret    
sub_3_0150:
        mov    ecx, dword ptr [esi + 0x107]
        jecxz  loc_3_0169
        push   0x944                         ; -> loc_1_0944
        push   ecx
        push   3
        call   dword ptr [esi + 0x124]
        add    esp, 0xc
loc_3_0169:
        ret    
sub_3_016A:
        push   ecx
        test   byte ptr [esi + 0x3b], 1
        je     loc_3_018F
        push   0x944                         ; -> loc_1_0944
        push   dword ptr [esi + 0x107]
        push   2
        call   dword ptr [esi + 0x124]
        add    esp, 0xc
        test   eax, eax
        je     loc_3_0191
        and    byte ptr [esi + 0x3b], 0xfe
loc_3_018F:
        test   esi, esi
loc_3_0191:
        pop    ecx
        ret    
sub_3_0193:
        push   ecx
        push   eax
        sti    
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf000100
        mov    ecx, eax
        add    ecx, 0xc8
loc_3_01A4:
        in     al, dx
        and    al, 0x60
        cmp    al, 0x60
        je     loc_3_01B5
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf000100
        cmp    eax, ecx
        jb     loc_3_01A4
loc_3_01B5:
        pop    eax
        pop    ecx
        ret    
sub_3_01B8:
        test   byte ptr [esi + 0x3b], 1
        je     loc_3_01C5
        call   sub_3_016A
        je     loc_3_0221
loc_3_01C5:
        inc    edx
        mov    al, 1
        out    dx, al
        add    dl, 4
        jmp    loc_3_01CE
loc_3_01CE:
        call   sub_3_0193
        xor    al, al
        test   byte ptr [esi + 0xf8], 8
        je     loc_3_01E0
        mov    al, 0x87
loc_3_01E0:
        sub    dl, 3
        out    dx, al
        jmp    loc_3_01E6
loc_3_01E6:
        call   sub_1_0957
        add    dl, 2
        in     al, dx
        jmp    loc_3_01F1
loc_3_01F1:
        mov    ah, al
        or     al, 0x10
        out    dx, al
        jmp    loc_3_01F8
loc_3_01F8:
        sub    dl, 4
        xor    al, al
        out    dx, al
        jmp    loc_3_0200
loc_3_0200:
        add    dl, 5
        call   sub_3_0193
        mov    al, ah
        dec    dl
        and    al, 3
        out    dx, al
        call   sub_1_0987
        sub    dl, 3
        xor    al, al
        out    dx, al
        dec    dl
        call   sub_1_0957
loc_3_0221:
        push   ecx
        push   ebx
        mov    edi, dword ptr [esi + 0x116]
        dec    byte ptr [edi + 1]
        jne    loc_3_025B
        movzx  eax, byte ptr [esi + 0x115]
        VxDCall  VPICD.VPICD_Get_Virtualization_Count ; cd2016000300
        cmp    eax, 1
        ja     loc_3_0246
        cmp    byte ptr [edi + 2], 0
        jne    loc_3_024B
loc_3_0246:
        call   sub_1_0987
loc_3_024B:
        xor    eax, eax
        xchg   dword ptr [esi + 0x10c], eax
        VxDCall  VPICD.VPICD_Force_Default_Behavior ; cd2011000300
        jmp    loc_3_0290
loc_3_025B:
        cli    
        mov    eax, dword ptr [edi + 8]
        cmp    eax, esi
        jne    loc_3_026E
        mov    eax, dword ptr [eax + 0x130]
        mov    dword ptr [edi + 8], eax
        jmp    loc_3_028A
loc_3_026E:
        cmp    dword ptr [eax + 0x130], esi
        je     loc_3_027E
        mov    eax, dword ptr [eax + 0x130]
        jmp    loc_3_026E
loc_3_027E:
        mov    ecx, dword ptr [esi + 0x130]
        mov    dword ptr [eax + 0x130], ecx
loc_3_028A:
        sti    
        call   sub_1_0987
loc_3_0290:
        pop    ebx
        call   sub_3_0150
        pop    ecx
        xor    eax, eax
        ret    
sub_3_029A:
        push   edi
        mov    edx, dword ptr [eax + 0x11e]
        xor    ecx, ecx
loc_3_02A3:
        mov    cl, byte ptr [edx]
        mov    ch, byte ptr [edi]
        or     ecx, ecx
        je     loc_3_02BF
        call   sub_3_02C1
        xchg   ch, cl
        call   sub_3_02C1
        inc    edx
        inc    edi
        cmp    cl, ch
        je     loc_3_02A3
        or     esi, esi
loc_3_02BF:
        pop    edi
        ret    
sub_3_02C1:
        cmp    cl, 0x41
        jb     loc_3_02CE
        cmp    cl, 0x5a
        ja     loc_3_02CE
        or     cl, 0x20
loc_3_02CE:
        ret    
        db  000h
loc_3_02D0:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    edi, dword ptr [ebp + 8]
        mov    esi, dword ptr [0x1158]       ; -> obj1:0x1158
        VMMCall  VMM.List_Get_First              ; cd20a3000100
loc_3_02E5:
        je     loc_3_0312
        call   sub_3_029A
        je     loc_3_02F6
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        jmp    loc_3_02E5
loc_3_02F6:
        mov    esi, eax
        cmp    dword ptr [esi + 0x64], 0x534d5446
        je     loc_3_031C
        mov    eax, dword ptr [ebp + 0xc]
        call   sub_3_0120
        jae    loc_3_0330
        jb     loc_3_031C
loc_3_030D:
        call   sub_3_0150
loc_3_0312:
        mov    eax, 0xfffffff6
        jmp    loc_3_0482
loc_3_031C:
        mov    eax, 0xfffffffe
        cmp    dword ptr [esi + 0xc4], -1
        jne    loc_3_0482
        jmp    loc_3_0312
loc_3_0330:
        mov    edx, dword ptr [esi + 0xc4]
        add    dl, 2
        in     al, dx
        test   al, 0x30
        jne    loc_3_030D
        mov    edx, dword ptr [esi + 0x116]
        cmp    byte ptr [edx + 1], 0
        je     loc_3_0375
        cli    
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [esi + 0x10c], eax
        inc    byte ptr [edx + 1]
        xor    ecx, ecx
        mov    eax, dword ptr [edx + 8]
loc_3_035C:
        cmp    dword ptr [eax + 0x130], ecx
        je     loc_3_036C
        mov    eax, dword ptr [eax + 0x130]
        jmp    loc_3_035C
loc_3_036C:
        mov    dword ptr [eax + 0x130], esi
        sti    
        jmp    loc_3_03C1
loc_3_0375:
        movzx  eax, byte ptr [esi + 0x115]
        mov    edi, 0x11c8                   ; -> obj1:0x11C8
        mov    word ptr [edi], ax
        mov    dword ptr [edi + 4], 0x40c    ; -> loc_1_040C
        mov    word ptr [edi + 2], 6
        mov    dword ptr [edi + 0x1c], edx
        VxDCall  VPICD.VPICD_Virtualize_IRQ      ; cd2001000300
        jb     loc_3_030D
        mov    dword ptr [edx + 8], esi
        mov    dword ptr [esi + 0x10c], eax
        mov    dword ptr [edx + 4], eax
        inc    byte ptr [edx + 1]
        mov    ebx, dword ptr [0x115c]       ; -> obj1:0x115C
        VxDCall  VPICD.VPICD_Get_Complete_Status ; cd2005000300
        and    cl, 4
        mov    byte ptr [edx + 2], cl
loc_3_03C1:
        lea    edi, [esi + 0x4c]
        xor    eax, eax
        mov    ecx, 6
        cld    
        rep stosd dword ptr es:[edi], eax
        mov    byte ptr [esi + 0xfc], al
        mov    byte ptr [esi + 0x114], al
        mov    byte ptr [esi + 0x3b], al
        mov    dword ptr [esi + 0xe4], eax
        mov    dword ptr [esi + 0xcc], eax
        dec    eax
        mov    dword ptr [esi + 0xe0], eax
        mov    dword ptr [esi + 0xa0], eax
        mov    dword ptr [esi + 0xa4], eax
        mov    dword ptr [esi + 0x9c], 1
        mov    edx, dword ptr [esi + 0xc4]
        add    dl, 3
        in     al, dx
        mov    ah, al
        or     al, 0x80
        out    dx, al
        sub    dl, 3
        in     al, dx
        movzx  ecx, al
        inc    dl
        in     al, dx
        mov    ch, al
        mov    al, ah
        add    dl, 2
        out    dx, al
        push   edx
        xor    edx, edx
        xor    eax, eax
        jecxz  loc_3_0435
        mov    eax, 0x1c200
        div    ecx
loc_3_0435:
        pop    edx
        mov    dword ptr [esi + 0x98], eax
        mov    byte ptr [esi + 0xaa], 8
        dec    dl
        in     al, dx
        test   al, 0x80
        je     loc_3_0451
        or     byte ptr [esi + 0xf8], 8
loc_3_0451:
        lea    edi, [esi + 0x88]
        mov    dword ptr [esi + 0x12c], edi
        lea    edi, [esi + 0x30]
        mov    dword ptr [esi + 0xf0], edi
        lea    edi, [esi + 0x38]
        mov    dword ptr [esi + 0xf4], edi
        and    byte ptr [esi + 0xf8], 0x1f
        or     byte ptr [esi + 0xf8], 0x80
        xor    eax, eax
        mov    dword ptr [esi + 0x34], eax
loc_3_0482:
        mov    ebx, dword ptr [ebp + 0x10]
        mov    dword ptr [ebx], eax
        or     eax, eax
        jne    loc_3_04B1
        mov    ebx, dword ptr [ebp + 0xc]
        mov    dword ptr [esi + 0x11a], ebx
        mov    dword ptr [esi + 0xc], eax
        mov    edx, dword ptr [esi + 0xc4]
        add    dl, 4
        in     al, dx
        or     al, 8
        out    dx, al
        call   sub_1_0987
        and    dword ptr [esi + 0x34], 0
        mov    eax, esi
        jmp    loc_3_04B3
loc_3_04B1:
        xor    eax, eax
loc_3_04B3:
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
loc_3_04B8:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        xor    eax, eax
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [esi + 0xc], eax
        lea    esi, [esi + 0x94]
        mov    edi, dword ptr [ebp + 0xc]
        mov    ecx, 0xc
        rep movsd dword ptr es:[edi], dword ptr [esi]
        inc    eax
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
loc_3_04DC:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    esi, dword ptr [ebp + 8]
        call   sub_3_00CC
        mov    dword ptr [esi + 0xc], eax
        test   byte ptr [esi + 0x114], 8
        je     loc_3_0503
        push   0
        push   dword ptr [esi + 0x3c]
        VMMCall  VMM._HeapFree                   ; cd2051000100
        pop    ecx
        pop    ecx
loc_3_0503:
        test   byte ptr [esi + 0x114], 4
        je     loc_3_0519
        push   0
        push   dword ptr [esi + 0x44]
        VMMCall  VMM._HeapFree                   ; cd2051000100
        pop    ecx
        pop    ecx
loc_3_0519:
        mov    ebx, esi
        or     byte ptr [esi + 0x114], 0x20
        call   sub_1_1036
        call   sub_3_08F3
        or     al, 1
        pop    ebx
        pop    edi
        pop    esi
        leave  
        ret    
loc_3_0533:
        mov    edx, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [edx + 0xf4], ecx
        xor    eax, eax
        mov    dword ptr [edx + 0xc], eax
        inc    eax
        ret    
loc_3_0548:
        push   esi
        push   edi
        mov    esi, 0x1c                     ; -> obj4:0x001C
        mov    edi, dword ptr [esp + 0x10]
        mov    eax, edi
        mov    ecx, 0x40
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    edx, dword ptr [esp + 0xc]
        mov    ecx, dword ptr [edx + 0x40]
        mov    dword ptr [eax + 0x30], ecx
        mov    ecx, dword ptr [edx + 0x48]
        mov    dword ptr [eax + 0x2c], ecx
        xor    eax, eax
        mov    dword ptr [edx + 0xc], eax
        inc    eax
        pop    edi
        pop    esi
        ret    
loc_3_0575:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [edx + 0xec], eax
        mov    ecx, dword ptr [ebp + 0x10]
        jecxz  loc_3_058F
        mov    dword ptr [edx + 0xf0], ecx
loc_3_058F:
        xor    eax, eax
        mov    dword ptr [edx + 0xc], eax
        inc    eax
        leave  
        ret    
loc_3_0597:
        mov    edx, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 8]
        mov    eax, dword ptr [esp + 0xc]
        and    byte ptr [edx + 0xcd], 0xbf
        mov    dword ptr [edx + 0x110], eax
        mov    dword ptr [edx + 0xc8], ecx
        jecxz  loc_3_05E1
        cli    
        or     byte ptr [edx + 0xcd], 0x40
        mov    ecx, dword ptr [edx + 0xf0]
        mov    eax, dword ptr [edx + 0xec]
        and    eax, dword ptr [ecx]
        sti    
        or     eax, eax
        je     loc_3_05E1
        push   esi
        mov    esi, edx
        mov    eax, 4
        call   sub_1_0A8C
        pop    esi
loc_3_05E1:
        xor    eax, eax
        mov    dword ptr [edx + 0xc], eax
        inc    eax
        ret    
loc_3_05E8:
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        mov    ecx, dword ptr [ebp + 0xc]
        mov    esi, dword ptr [ebp + 8]
        mov    eax, ecx
        inc    eax
        je     loc_3_0600
        cmp    ecx, dword ptr [esi + 0x40]
        jb     loc_3_0600
        mov    ecx, dword ptr [esi + 0x40]
loc_3_0600:
        mov    ebx, dword ptr [ebp + 0x10]
        or     ebx, ebx
        jne    loc_3_060A
        or     ecx, 0xffffffff
loc_3_060A:
        cli    
        mov    dword ptr [esi + 0xd0], ebx
        mov    edx, dword ptr [ebp + 0x14]
        mov    dword ptr [esi + 0xd4], edx
        mov    dword ptr [esi + 0xe0], ecx
        sti    
        cmp    dword ptr [esi + 0x4c], 0
        jne    loc_3_0633
        mov    esi, dword ptr [esi + 0x12c]
        mov    dword ptr [esi], 0
loc_3_0633:
        call   sub_1_1036
        or     al, 1
        pop    ebx
        pop    esi
        leave  
        ret    
loc_3_063E:
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        mov    edx, dword ptr [ebp + 0xc]
        mov    esi, dword ptr [ebp + 8]
        inc    edx
        je     loc_3_065E
        dec    edx
        test   byte ptr [esi + 0x114], 0x10
        je     loc_3_065E
        cmp    edx, dword ptr [esi + 0x48]
        jbe    loc_3_065E
        mov    edx, dword ptr [esi + 0x48]
loc_3_065E:
        mov    ebx, dword ptr [ebp + 0x10]
        or     ebx, ebx
        jne    loc_3_0667
        xor    edx, edx
loc_3_0667:
        cli    
        mov    dword ptr [esi + 0xe4], edx
        mov    dword ptr [esi + 0xd8], ebx
        mov    ebx, dword ptr [ebp + 0x14]
        mov    dword ptr [esi + 0xdc], ebx
        cmp    dword ptr [esi + 0x58], edx
        jae    loc_3_0689
        or     byte ptr [esi + 0xcd], 2
loc_3_0689:
        sti    
        or     al, 1
        pop    ebx
        pop    esi
        leave  
        ret    
loc_3_0690:
        mov    edx, dword ptr [esp + 4]
        mov    edx, dword ptr [edx + 0xc4]
        add    dl, 6
        in     al, dx
        and    eax, 0xf0
        mov    edx, dword ptr [esp + 8]
        mov    dword ptr [edx], eax
        or     al, 1
        ret    
loc_3_06AC:
        xor    eax, eax
        mov    edx, dword ptr [esp + 0xc]
        push   0x34
        pop    ecx
        cmp    dword ptr [edx], ecx
        mov    dword ptr [edx], ecx
        jb     loc_3_06EF
        mov    edx, dword ptr [esp + 8]
        or     edx, edx
        je     loc_3_06EF
        mov    dword ptr [edx + 0x28], eax
        mov    dword ptr [edx + 0x2c], eax
        mov    byte ptr [edx + 0x30], al
        mov    al, 0x34
        mov    dword ptr [edx], eax
        mov    al, 1
        mov    dword ptr [edx + 0x24], eax
        shl    eax, 8
        mov    word ptr [edx + 4], ax
        mov    eax, dword ptr [esp + 4]
        lea    eax, [eax + 0x94]
        lea    edx, [edx + 8]
        VxDCall  BIOS.Service_0020h              ; cd2020002b00
loc_3_06EF:
        or     al, 1
        ret    
loc_3_06F2:
        sub    esp, 0x30
        xor    eax, eax
        mov    edx, dword ptr [esp + 0x34]
        mov    dword ptr [edx + 0xc], 0xffffffeb
        cmp    dword ptr [esp + 0x3c], 0x34
        jb     loc_3_0728
        lea    edx, [esp]
        mov    eax, dword ptr [esp + 0x38]
        lea    eax, [eax + 8]
        VxDCall  BIOS.Service_001Fh              ; cd201f002b00
        push   -1
        push   edx
        push   dword ptr [esp + 0x3c]
        call   sub_1_0B66
        add    esp, 0xc
loc_3_0728:
        add    esp, 0x30
        ret    
loc_3_072C:
        ret    
        db  000h, 000h, 000h
loc_3_0730:
        mov    eax, dword ptr [esp + 4]
        or     eax, eax
        je     loc_3_0739
        ret    
loc_3_0739:
        push   esi
        push   edi
        push   ebx
        cmp    dword ptr [esp + 0x20], 0
        je     loc_3_08EF
        mov    esi, dword ptr [0x1158]       ; -> obj1:0x1158
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_3_076D
        mov    ecx, dword ptr [esp + 0x1c]
loc_3_0759:
        cmp    dword ptr [eax + 0xc4], ecx
        je     loc_3_08EF
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        jne    loc_3_0759
loc_3_076D:
        VMMCall  VMM.List_Allocate               ; cd209d000100
        jb     loc_3_08EF
        VMMCall  VMM.List_Attach                 ; cd209e000100
        mov    edi, eax
        mov    ebx, eax
        xor    eax, eax
        mov    ecx, 0x4d
        rep stosd dword ptr es:[edi], eax
        mov    dword ptr [ebx], 0x10a0094
        mov    dword ptr [ebx + 4], 0x10fc   ; -> obj1:0x10FC
        mov    dword ptr [ebx + 8], 0x17
        mov    word ptr [ebx + 0xad], 0x1311
        dec    dword ptr [ebx + 0xe0]
        mov    byte ptr [ebx + 0x122], 0x80
        mov    byte ptr [ebx + 0x123], 0x10
        lea    esi, [0x69]                   ; -> obj4:0x0069
        mov    dword ptr [esi], 0x80001002
        mov    dword ptr [0x6d], 4           ; -> obj4:0x006D
        push   0
        push   0x6d                          ; -> obj4:0x006D
        push   esi
        push   3
        push   0x60                          ; -> obj4:0x0060
        push   0
        push   dword ptr [esp + 0x2c]
        VxDCall  CONFIGMG._CONFIGMG_Read_Registry_Value ; cd203e003300
        add    esp, 0x1c
        test   eax, eax
        jne    loc_3_083A
        mov    cl, byte ptr [esi + 3]
        mov    byte ptr [ebx + 0x122], cl
        cmp    byte ptr [esi + 2], 0
        je     loc_3_080B
        or     byte ptr [ebx + 0xf8], 1
loc_3_080B:
        cmp    byte ptr [esi], 1
        ja     loc_3_0822
        jb     loc_3_081B
        or     byte ptr [ebx + 0xf8], 4
        jmp    loc_3_0822
loc_3_081B:
        or     byte ptr [ebx + 0xf8], 2
loc_3_0822:
        cmp    byte ptr [esi + 1], 1
        jbe    loc_3_0833
        mov    cl, byte ptr [esi + 1]
        mov    byte ptr [ebx + 0x123], cl
        jmp    loc_3_083A
loc_3_0833:
        or     byte ptr [ebx + 0xf8], 0x10
loc_3_083A:
        mov    ecx, dword ptr [esp + 0x1c]
        mov    dword ptr [ebx + 0xc4], ecx
        mov    ecx, dword ptr [esp + 0x20]
        mov    byte ptr [ebx + 0x115], cl
        mov    esi, dword ptr [0x5c]         ; -> obj4:0x005C
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_3_0868
loc_3_085C:
        cmp    byte ptr [eax], cl
        je     loc_3_087E
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        jne    loc_3_085C
loc_3_0868:
        VMMCall  VMM.List_Allocate               ; cd209d000100
        VMMCall  VMM.List_Attach                 ; cd209e000100
        mov    dword ptr [eax], ecx
        xor    ecx, ecx
        mov    dword ptr [eax + 8], ecx
        mov    dword ptr [eax + 4], ecx
loc_3_087E:
        mov    dword ptr [ebx + 0x116], eax
        mov    edi, dword ptr [esp + 0x24]
        mov    esi, edi
        xor    eax, eax
        xor    ecx, ecx
        dec    ecx
        repne scasb al, byte ptr es:[edi]
        neg    ecx
        dec    ecx
        push   ecx
        push   0
        push   ecx
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        pop    ecx
        pop    ecx
        mov    edi, eax
        pop    ecx
        or     eax, eax
        je     loc_3_08EA
        mov    dword ptr [ebx + 0x11e], eax
        rep movsb byte ptr es:[edi], byte ptr [esi]
        push   eax
        push   0x2d0                         ; -> loc_3_02D0
        push   dword ptr [esp + 0x20]
        VxDCall  BIOS.Service_0019h              ; cd2019002b00
        add    esp, 0xc
        push   dword ptr [esp + 0x24]
        VxDCall  BIOS.Service_0021h              ; cd2021002b00
        pop    ecx
        mov    dword ptr [ebx + 0x124], eax
        test   eax, eax
        je     loc_3_08EF
        push   dword ptr [esp + 0x24]
        VxDCall  BIOS.Service_0022h              ; cd2022002b00
        pop    ecx
        mov    dword ptr [ebx + 0x128], eax
        jmp    loc_3_08EF
loc_3_08EA:
        call   sub_3_08F3
loc_3_08EF:
        pop    ebx
        pop    edi
        pop    esi
        ret    
sub_3_08F3:
        push   esi
        mov    esi, dword ptr [0x5c]         ; -> obj4:0x005C
        mov    eax, dword ptr [ebx + 0x116]
        or     eax, eax
        je     loc_3_0916
        cmp    byte ptr [eax + 1], 0
        jne    loc_3_0916
        VMMCall  VMM.List_Remove                 ; cd20a1000100
        VMMCall  VMM.List_Deallocate             ; cd20a2000100
loc_3_0916:
        cmp    dword ptr [ebx + 0x11e], 0
        je     loc_3_092F
        push   0
        push   dword ptr [ebx + 0x11e]
        VMMCall  VMM._HeapFree                   ; cd2051000100
        pop    ecx
        pop    ecx
loc_3_092F:
        mov    esi, dword ptr [0x1158]       ; -> obj1:0x1158
        mov    eax, ebx
        VMMCall  VMM.List_Remove                 ; cd20a1000100
        VMMCall  VMM.List_Deallocate             ; cd20a2000100
        pop    esi
        ret    
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 4  vsize=0x71  RW-
        db  043h, 04Fh, 04Dh, 031h, 000h, 043h, 04Fh, 04Dh, 032h, 000h, 043h, 04Fh, 04Dh, 033h, 000h, 043h
        db  04Fh, 04Dh, 034h, 000h, 053h, 045h, 052h, 049h, 041h, 04Ch, 000h, 000h, 040h, 000h, 002h, 000h
        db  001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 010h, 001h, 000h, 000h, 000h, 0FFh, 000h, 000h, 000h, 07Fh, 000h, 000h, 000h
        db  0FFh, 0FFh, 000h, 010h, 00Fh, 000h, 007h, 01Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  053h, 065h, 074h, 074h, 069h, 06Eh, 067h, 073h, 023h, 053h, 045h, 052h, 049h, 041h, 04Ch, 020h
        db  056h
VxD__LDATA ENDS

; DRVSPACX -- reconstructed 32-bit disassembly (VxD / LE format)
; 7 objects, 10177 instructions recovered by recursive descent

Declare_Virtual_Device DRVSPACX, 3, 10, DRVSPACX_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 080000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0xe6c  R-X
DRVSPACX_Control:
        cmp    eax, 0x1b
        je     loc_3_0370
        cmp    eax, 4
        je     loc_1_098C
        cmp    eax, 0x17
        je     loc_1_098C
        cmp    eax, 0x25
        je     loc_1_0A7F
        cmp    eax, 0x14
        je     loc_1_0234
        clc    
        ret    
        db  000h
loc_1_0030:
        mov    ebx, dword ptr [esp + 4]
        mov    word ptr [ebx + 2], 0
        movzx  eax, word ptr [ebx]
        cmp    eax, 0x15
        jae    loc_1_0090
        mov    al, byte ptr [eax + 0x52]     ; -> obj1:0x0052
        and    eax, 0x7f
        jmp    dword ptr [eax*4 + 0x68]      ; -> obj1:0x0068
        db  000h, 000h, 001h, 002h, 003h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 004h, 000h, 000h
        db  005h, 006h, 007h, 008h, 009h, 000h
        ; ---- dispatch table (10 entries) ----
        dd  loc_1_0090
        dd  loc_4_0135
        dd  loc_4_0000
        dd  loc_4_00AB
        dd  loc_4_0112
        dd  loc_4_020D
        dd  loc_4_023B
        dd  obj4:0x0AB6
        dd  loc_4_0AB7
        dd  loc_4_0BF0
loc_1_0090:
        ret    
sub_1_0091:
        inc    dword ptr [0xe1c]             ; -> loc_1_0E1C
        cmp    dword ptr [0xe1c], 1          ; -> loc_1_0E1C
        jne    loc_1_00C0
        mov    ecx, dword ptr [0xe18]        ; -> loc_1_0E18
        jecxz  loc_1_00C0
        mov    eax, dword ptr [0xe24]        ; -> loc_1_0E24
        push   0
        push   ecx
        push   eax
        VMMCall  VMM._LinPageLock                ; cd2063000100
        add    esp, 0xc
        cmp    eax, 1
        jb     loc_1_00C0
        clc    
loc_1_00C0:
        ret    
loc_1_00C1:
        call   sub_1_0091
        jb     loc_1_00E2
        mov    eax, ebp
        xchg   dword ptr [0xe44], eax        ; -> obj1:0x0E44
        mov    dword ptr [ebp], eax
        movzx  eax, byte ptr [ebp + 0x88]
        mov    dword ptr [eax*4 + 0xdb0], ebp ; -> loc_1_0DB0
        clc    
loc_1_00E2:
        ret    
loc_1_00E3:
        movzx  ecx, byte ptr [ebp + 0x88]
        xor    esi, esi
        mov    edx, 0xe44                    ; -> obj1:0x0E44
loc_1_00F1:
        mov    eax, dword ptr [edx]
        or     eax, eax
        je     loc_1_0134
        cmp    eax, ebp
        je     loc_1_0109
        mov    edx, eax
        cmp    byte ptr [eax + 0x88], cl
        jne    loc_1_00F1
        mov    esi, eax
        jmp    loc_1_00F1
loc_1_0109:
        or     esi, esi
        jne    loc_1_011D
loc_1_010D:
        mov    eax, dword ptr [eax]
        or     eax, eax
        je     loc_1_011D
        cmp    byte ptr [eax + 0x88], cl
        jne    loc_1_010D
        mov    esi, eax
loc_1_011D:
        xor    eax, eax
        xchg   dword ptr [ebp], eax
        mov    dword ptr [edx], eax
        cmp    dword ptr [ecx*4 + 0xdb0], ebp ; -> loc_1_0DB0
        jne    loc_1_0134
        mov    dword ptr [ecx*4 + 0xdb0], esi ; -> loc_1_0DB0
loc_1_0134:
        ret    
sub_1_0135:
        dec    dword ptr [0xe1c]             ; -> loc_1_0E1C
        jne    loc_1_015A
        mov    ecx, dword ptr [0xe18]        ; -> loc_1_0E18
        jecxz  loc_1_015A
        mov    eax, dword ptr [0xe24]        ; -> loc_1_0E24
        push   0x2000
        push   ecx
        push   eax
        VMMCall  VMM._LinPageUnLock              ; cd2064000100
        add    esp, 0xc
loc_1_015A:
        ret    
loc_1_015B:
        push   edx
        mov    edi, dword ptr [0xe40]        ; -> obj1:0x0E40
loc_1_0162:
        cmp    byte ptr [edi], al
        je     loc_1_017D
        mov    edx, dword ptr [edi + 0x19]
        cmp    dx, -1
        je     loc_1_017C
        movzx  edi, dx
        shr    edx, 0xc
        and    dl, 0xf0
        add    edi, edx
        jmp    loc_1_0162
loc_1_017C:
        stc    
loc_1_017D:
        pop    edx
        ret    
        db  000h
loc_1_0180:
        bts    dword ptr [0xae0], 0          ; -> obj1:0x0AE0
        jb     loc_1_01CF
        mov    eax, 0x2f
        mov    esi, 0x1d0                    ; -> sub_1_01D0
        VMMCall  VMM.Hook_V86_Int_Chain          ; cd2041000100
        mov    eax, 0x2f
        VMMCall  VMM.Get_PM_Int_Vector           ; cd2044000100
        mov    dword ptr [0xadc], ecx        ; -> obj1:0x0ADC
        mov    dword ptr [0xad8], edx        ; -> obj1:0x0AD8
        mov    esi, 0x1ec                    ; -> loc_1_01EC
        VMMCall  VMM.Allocate_PM_Call_Back       ; cd200c000100
        movzx  edx, ax
        mov    ecx, eax
        shr    ecx, 0x10
        mov    eax, 0x2f
        VMMCall  VMM.Set_PM_Int_Vector           ; cd2045000100
loc_1_01CF:
        ret    
sub_1_01D0:
        cmp    word ptr [ebp + 0x1c], 0x4a11
        je     loc_1_01DA
loc_1_01D8:
        stc    
        ret    
loc_1_01DA:
        movzx  ebx, word ptr [ebp + 0x10]
        cmp    bx, 0x13
        jae    loc_1_01D8
        jmp    dword ptr [ebx*4 + 0xf0c]     ; -> obj4:0x0F0C
loc_1_01EC:
        cmp    word ptr [ebp + 0x1c], 0x4a11
        je     loc_1_0206
loc_1_01F4:
        mov    ecx, dword ptr [0xadc]        ; -> obj1:0x0ADC
        mov    edx, dword ptr [0xad8]        ; -> obj1:0x0AD8
        VMMCall  VMM.Simulate_Far_Jmp            ; cd2049800100
loc_1_0206:
        call   sub_1_01D0
        jb     loc_1_01F4
        VMMCall  VMM.Simulate_Iret               ; cd2047000100
        ret    
sub_1_0214:
        pushal 
        lea    ebp, [esp + 0x1c]
        push   es
        mov    edi, ds
        mov    es, edi
        mov    esi, dword ptr [ebp + 8]
        lea    edi, [ebp + 0xc]
        mov    eax, 0x73
        int    0x41
        add    dword ptr [0xae4], eax        ; -> obj1:0x0AE4
        pop    es
        popal  
        ret    
loc_1_0234:
        cld    
        test   dword ptr [0x23b], 0x400000   ; -> obj1:0x023B
        je     loc_1_0338
        mov    edi, 0x177e                   ; -> loc_6_177E
loc_1_024A:
        inc    edi
loc_1_024B:
        mov    eax, 0x77
        mov    bl, 1
        int    0x41
        or     ah, ah
        je     loc_1_0261
        cmp    byte ptr [edi], 0x3f
        je     loc_1_024A
        scasb  al, byte ptr es:[edi]
        je     loc_1_024B
        dec    edi
loc_1_0261:
        cmp    byte ptr [edi], 0
        or     ah, ah
        jne    loc_1_0324
        push   0xae8                         ; -> loc_1_0AE8
        call   sub_1_0214
        add    esp, 4
        mov    ecx, 0xe44                    ; -> obj1:0x0E44
loc_1_027E:
        mov    ecx, dword ptr [ecx]
        jecxz  loc_1_02BD
        movzx  edi, byte ptr [ecx + 0x88]
        movzx  eax, byte ptr [ecx + 0x89]
        movzx  edx, byte ptr [ecx + 0x8a]
        add    edi, 0x41
        add    eax, 0x41
        add    edx, 0x41
        mov    bl, 0x2a
        cmp    dword ptr [ecx + 0x48], 0
        jne    loc_1_02AA
        mov    bl, 0x20
loc_1_02AA:
        push   edx
        push   eax
        push   ebx
        push   edi
        push   0xb00                         ; -> loc_1_0B00
        call   sub_1_0214
        add    esp, 0x14
        jmp    loc_1_027E
loc_1_02BD:
        push   0xb14                         ; -> loc_1_0B14
        call   sub_1_0214
        add    esp, 4
loc_1_02CA:
        mov    ecx, 0xd
        mov    ebx, 0x1788                   ; -> loc_6_1788
loc_1_02D4:
        mov    eax, dword ptr [ebx]
        mov    edx, 0x4e
        sub    edx, ecx
        push   eax
        push   edx
        push   0xb18                         ; -> loc_1_0B18
        call   sub_1_0214
        add    esp, 0xc
        add    ebx, 8
        loop   loc_1_02D4
        push   0x18ee                        ; -> obj6:0x18EE
        push   0xb24                         ; -> loc_1_0B24
        call   sub_1_0214
        add    esp, 8
        call   sub_6_0000
        cmp    al, 0x1b
        jne    loc_1_030E
        mov    al, 0x58
loc_1_030E:
        push   eax
        push   0xb2c                         ; -> loc_1_0B2C
        call   sub_1_0214
        add    esp, 8
        cmp    al, 0x61
        jb     loc_1_0324
        sub    al, 0x61
        jmp    loc_1_0326
loc_1_0324:
        sub    al, 0x41
loc_1_0326:
        cmp    al, 0x17
        je     loc_1_0338
        cmp    al, 0xd
        jae    loc_1_02CA
        movzx  eax, al
        call   sub_6_178C
loc_1_0338:
        clc    
        ret    
        db  000h, 000h
sub_1_033C:
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf000100
        mov    ebx, 0xe44                    ; -> obj1:0x0E44
        xor    ebp, ebp
loc_1_0349:
        mov    ebx, dword ptr [ebx]
        or     ebx, ebx
        je     loc_1_0372
        test   byte ptr [ebx + 4], 0x80
        je     loc_1_0349
        cmp    dword ptr [ebx + 0x48], 0
        jne    loc_1_0349
        mov    edi, eax
        sub    edi, dword ptr [ebx + 0x28]
        jb     loc_1_036E
        cmp    edi, eax
        je     loc_1_036E
        cmp    edi, 0x7d0
        jb     loc_1_0349
loc_1_036E:
        mov    ebp, ebx
        jmp    loc_1_0349
loc_1_0372:
        or     ebp, ebp
        je     loc_1_03AD
        mov    dword ptr [ebp + 0x48], 0xffffff44
        mov    dword ptr [ebp + 0x23c], esp
        lea    eax, [ebp + 0xb6]
        mov    dword ptr [ebp + 0xe0], eax
        mov    dword ptr [ebp + 0xdc], 0x400
        mov    esi, dword ptr [ebp + 0x24]
        call   sub_7_1B8C
        mov    ebx, 0xffffff44
        call   sub_7_4344
        jmp    sub_1_033C
loc_1_03AD:
        btr    dword ptr [0xcfc], 0          ; -> obj1:0x0CFC
        ret    
loc_1_03B6:
        bts    dword ptr [0xcfc], 0          ; -> obj1:0x0CFC
        jb     loc_1_03C5
        call   sub_1_033C
loc_1_03C5:
        bts    dword ptr [0xcfc], 1          ; -> obj1:0x0CFC
        jb     loc_1_03D4
        call   sub_1_03ED
loc_1_03D4:
        mov    eax, 0x3e8
        xor    edx, edx
        mov    esi, 0x3b6                    ; -> loc_1_03B6
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    dword ptr [0xe28], esi        ; -> loc_1_0E28
        ret    
sub_1_03ED:
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf000100
        mov    esi, dword ptr [0xd94]        ; -> loc_1_0D94
loc_1_03F9:
        cmp    esi, 0xd7c                    ; -> loc_1_0D7C
        je     loc_1_0438
        mov    edi, eax
        sub    edi, dword ptr [esi + 0x24]
        jb     loc_1_0410
        cmp    edi, 0x7d0
        jb     loc_1_0438
loc_1_0410:
        mov    edi, dword ptr [esi + 0x14]
        mov    ebx, dword ptr [esi + 0x18]
        mov    dword ptr [edi + 0x18], ebx
        mov    dword ptr [ebx + 0x14], edi
        mov    ebp, dword ptr [esi + 8]
        mov    ebx, esi
        xchg   dword ptr [ebp + 0xe4], ebx
        mov    dword ptr [esi + 0x14], ebx
        mov    ebx, dword ptr [esi + 0x18]
        mov    dword ptr [esi + 0x18], 0xffffffcc
        mov    esi, ebx
        jmp    loc_1_03F9
loc_1_0438:
        mov    ebp, 0xe44                    ; -> obj1:0x0E44
loc_1_043D:
        mov    ebp, dword ptr [ebp]
        or     ebp, ebp
        je     loc_1_0485
        cmp    dword ptr [ebp + 0x48], 0
        jne    loc_1_043D
        cmp    dword ptr [ebp + 0xe4], 0
        je     loc_1_043D
        mov    dword ptr [ebp + 0x48], 0xffffff44
        mov    dword ptr [ebp + 0x23c], esp
        lea    eax, [ebp + 0xb6]
        mov    dword ptr [ebp + 0xe0], eax
        mov    dword ptr [ebp + 0xdc], 0x400
        mov    ebx, 0xffffff44
        call   sub_7_4344
        jmp    sub_1_03ED
loc_1_0485:
        btr    dword ptr [0xcfc], 1          ; -> obj1:0x0CFC
        ret    
        db  000h, 000h
loc_1_0490:
        mov    edx, dword ptr [esp + 4]
        push   ebx
        push   edi
        push   esi
        push   ebp
        push   0x75c                         ; -> loc_1_075C
loc_1_049D:
        movzx  eax, byte ptr [edx + 0xb1]
        cmp    al, 0x1a
        jae    loc_1_05B5
        mov    ebp, dword ptr [eax*4 + 0xdb0] ; -> loc_1_0DB0
        mov    ebx, edx
        cmp    word ptr [edx + 0x68], 0xc
        je     loc_1_0606
        or     ebp, ebp
        je     loc_1_064C
        mov    ecx, dword ptr [edx + 8]
        test   byte ptr [ecx + 0x20], 4
        je     loc_1_04F8
        test   byte ptr [edx + 0x6f], 2
        jne    loc_1_04F8
        test   byte ptr [edx + 0x6c], 8
        jne    loc_1_08D5
        mov    ecx, dword ptr [edx + 0xa8]
        or     ecx, ecx
        je     loc_1_05A5
        cmp    dword ptr [ebp + 0x3c], ecx
        jne    loc_1_05DD
loc_1_04F8:
        movzx  eax, word ptr [edx + 0x68]
        cmp    eax, 0x2a
        jae    loc_1_05B5
        cmp    byte ptr [eax + 0x710], 0     ; -> loc_1_0710
        je     loc_1_05B5
        jl     loc_1_0583
        cmp    eax, 0xd
        je     loc_1_0761
loc_1_051D:
        or     dword ptr [edx + 0x8c], 0xffffffff
        lea    eax, [ebp + 0x40]
        cmp    dword ptr [eax + 8], 0
        jne    loc_1_05C8
        mov    dword ptr [eax + 8], edx
loc_1_0534:
        mov    ebx, edx
        mov    dword ptr [ebp + 0x23c], esp
        lea    eax, [ebx + 0x6a]
        mov    dword ptr [ebp + 0xe0], eax
        xor    edx, edx
        xchg   word ptr [ebp + 0xb6], dx
        mov    word ptr [ebx + 0x6a], dx
        cmp    edx, 0x10
        jae    loc_1_08A6
        test   byte ptr [ebp + 4], 0x10
        jne    loc_1_059A
        mov    eax, dword ptr [ebx + 0x6c]
        mov    dword ptr [ebp + 0xdc], eax
        movzx  eax, word ptr [ebx + 0x68]
        push   0x8a6                         ; -> loc_1_08A6
        mov    al, byte ptr [eax + 0x710]    ; -> loc_1_0710
        and    eax, 0x7f
        jmp    dword ptr [eax*4 + 0x73c]     ; -> obj1:0x073C
loc_1_0583:
        mov    ebx, edx
        push   0x8d5                         ; -> loc_1_08D5
        mov    al, byte ptr [eax + 0x710]    ; -> loc_1_0710
        and    eax, 0x7f
        jmp    dword ptr [eax*4 + 0x73c]     ; -> obj1:0x073C
loc_1_059A:
        mov    word ptr [ebx + 0x6a], 0x14
        jmp    loc_1_08A6
loc_1_05A5:
        cmp    word ptr [edx + 0x68], 0xd
        je     loc_1_04F8
        jmp    loc_1_08D5
loc_1_05B5:
        mov    ebx, edx
        push   0x8d5                         ; -> loc_1_08D5
        cmp    word ptr [edx + 0x68], 0xd
        je     sub_1_0870
        ret    
loc_1_05C8:
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ecx], edx
        lea    ecx, [edx + 0x8c]
        mov    dword ptr [ecx], 0
        mov    dword ptr [eax + 4], ecx
        ret    
loc_1_05DD:
        mov    ebp, 0xe44                    ; -> obj1:0x0E44
loc_1_05E2:
        mov    ebp, dword ptr [ebp]
        or     ebp, ebp
        je     loc_1_08D5
        cmp    byte ptr [ebp + 0x88], al
        jne    loc_1_05E2
        cmp    dword ptr [ebp + 0x3c], ecx
        jne    loc_1_05E2
        mov    dword ptr [eax*4 + 0xdb0], ebp ; -> loc_1_0DB0
        jmp    loc_1_04F8
loc_1_0606:
        or     ebp, ebp
        je     loc_1_0610
        mov    al, byte ptr [ebp + 0x89]
loc_1_0610:
        mov    ebp, 0xe44                    ; -> obj1:0x0E44
loc_1_0615:
        mov    ebp, dword ptr [ebp]
        or     ebp, ebp
        je     loc_1_08D5
        cmp    byte ptr [ebp + 0x89], al
        jne    loc_1_0615
        test   byte ptr [ebp + 6], 2
        jne    loc_1_0615
        cmp    dword ptr [ebp + 0x48], 0
        jne    loc_1_0615
        test   byte ptr [ebp + 4], 0x80
        jne    loc_1_0643
        push   eax
        push   ebx
        call   sub_7_4687
        pop    ebx
        pop    eax
loc_1_0643:
        and    dword ptr [ebp + 4], 0xffffdfff
        jmp    loc_1_0615
loc_1_064C:
        cmp    word ptr [ebx + 0x68], 0xd
        jne    loc_1_08D5
        cmp    word ptr [ebx + 0x92], 0x440d
        jne    loc_1_08D5
        cmp    dword ptr [ebx + 0x94], 0x872
        je     loc_1_0682
        cmp    dword ptr [ebx + 0x94], 0x4872
        jne    loc_1_08D5
loc_1_0682:
        test   byte ptr [ebx + 0x6c], 0x10
        je     loc_1_08D5
        movzx  eax, byte ptr [ebx + 0xb1]
        mov    edx, dword ptr [ebx + 0x9c]
        mov    edx, dword ptr [edx + 0x14]
        cmp    dl, 1
        ja     loc_1_08D5
        cmp    dh, 1
        ja     loc_1_08D5
        mov    ecx, 0x1a
loc_1_06B3:
        mov    ebp, dword ptr [ecx*4 + 0xdac] ; -> loc_1_0DAC
        or     ebp, ebp
        je     loc_1_06D1
        cmp    byte ptr [ebp + 0x89], al
        jne    loc_1_06D1
        test   dl, 1
        jne    loc_1_06F9
        test   byte ptr [ebp + 6], 4
        jne    loc_1_06E1
loc_1_06D1:
        loop   loc_1_06B3
        test   dl, 2
        jne    loc_1_08AE
        jmp    loc_1_08D5
loc_1_06E1:
        test   byte ptr [ebp + 4], 1
        je     loc_1_06D1
        and    dword ptr [ebx + 0xa0], 0
        mov    word ptr [ebx + 0x6a], 0
        jmp    loc_1_08AE
loc_1_06F9:
        or     dl, 2
        and    dword ptr [ebp + 4], 0xfffbffff
        or     dh, dh
        je     loc_1_06D1
        or     dword ptr [ebp + 4], 0x40000
        jmp    loc_1_06D1
loc_1_0710:
        add    dword ptr [edx], eax
        add    dword ptr [eax], -0x7d
        add    byte ptr [eax + eax], al
        add    byte ptr [0], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [esi], al
        push   es
        pop    es
        add    byte ptr [eax], al
        ; ---- dispatch table (8 entries) ----
        dd  loc_1_0889
        dd  loc_7_3CFF
        dd  loc_7_3CE4
        dd  loc_1_094B
        dd  loc_1_0889
        dd  loc_7_3E86
        dd  loc_1_088A
        dd  loc_7_3F69
loc_1_075C:
        pop    ebp
        pop    esi
        pop    edi
        pop    ebx
        ret    
loc_1_0761:
        mov    ebx, edx
        movzx  eax, word ptr [ebx + 0x92]
        cmp    ax, 0x440d
        jne    loc_1_07C5
        cmp    dword ptr [ebx + 0x94], 0x4860
        je     loc_1_078C
        cmp    dword ptr [ebx + 0x94], 0x860
        jne    loc_1_08D5
loc_1_078C:
        mov    eax, dword ptr [ebp + 0x38]
        test   byte ptr [eax + 0x20], 4
        je     loc_1_07A4
        mov    eax, dword ptr [ebx + 0x98]
        test   byte ptr [eax], 1
        je     loc_1_08D5
loc_1_07A4:
        mov    eax, dword ptr [ebx + 0x14]
        mov    dword ptr [eax], 0x42b8       ; -> loc_7_42B8
        mov    dword ptr [eax + 4], ebp
        add    dword ptr [ebx + 0x14], 8
        mov    al, byte ptr [ebp + 0x89]
        mov    byte ptr [ebx + 0xb1], al
        jmp    loc_1_08D5
loc_1_07C5:
        cmp    ax, 0x4404
        jne    loc_1_08D5
        cmp    dword ptr [ebx + 0x94], 5
        jb     loc_1_08D5
        mov    esi, dword ptr [ebx + 0x98]
        cmp    word ptr [esi], 0x444d
        jne    loc_1_08D5
        mov    al, byte ptr [esi + 2]
        mov    ecx, 0x80000000
        cmp    al, 0x72
        je     loc_1_0803
        cmp    al, 0x77
        jne    loc_1_051D
        xor    ecx, ecx
loc_1_0803:
        test   byte ptr [esi + 5], 1
        jne    loc_1_051D
        mov    word ptr [esi + 3], 2
        mov    edx, dword ptr [ebx + 0x94]
        call   sub_7_12FC
        mov    edx, ebx
        mov    dword ptr [ebx + 0x98], esi
        jae    loc_1_049D
        mov    word ptr [ebx + 0x6a], 0x1d
        jmp    loc_1_08AE
sub_1_0834:
        cmp    word ptr [ebx + 0x68], 0xd
        jne    loc_1_086E
        cmp    word ptr [ebx + 0x92], 0x4404
        jne    loc_1_086E
        cmp    dword ptr [ebx + 0x94], 5
        jb     loc_1_086E
        mov    esi, dword ptr [ebx + 0x98]
        cmp    word ptr [esi], 0x444d
        jne    loc_1_086E
        mov    al, byte ptr [esi + 2]
        cmp    al, 0x72
        je     loc_1_0867
        cmp    al, 0x77
        jne    loc_1_086E
loc_1_0867:
        test   byte ptr [esi + 5], 1
        stc    
        jne    loc_1_086F
loc_1_086E:
        clc    
loc_1_086F:
        ret    
sub_1_0870:
        call   sub_1_0834
        jae    loc_1_0888
        mov    esi, dword ptr [ebx + 0x98]
        mov    edx, dword ptr [ebx + 0x94]
        call   sub_7_13B2
loc_1_0888:
        ret    
loc_1_0889:
        ret    
loc_1_088A:
        call   sub_1_093E
        jb     loc_1_08A5
        mov    ebx, dword ptr [ebp + 0x48]
        cmp    word ptr [ebx + 0x68], 0x28
        jne    loc_1_08CC
        call   sub_7_4687
        mov    ebx, dword ptr [ebp + 0x48]
        jmp    loc_1_08CC
loc_1_08A5:
        ret    
loc_1_08A6:
        mov    ebx, dword ptr [ebp + 0x48]
        call   sub_7_4344
loc_1_08AE:
        cmp    word ptr [ebx + 0x68], 0xd
        je     loc_1_08C5
loc_1_08B5:
        mov    eax, dword ptr [ebx + 0x14]
        sub    eax, 8
        mov    dword ptr [ebx + 0x14], eax
        push   ebx
        call   dword ptr [eax]
        add    esp, 4
        ret    
loc_1_08C5:
        call   sub_1_0870
        jmp    loc_1_08B5
loc_1_08CC:
        pop    eax
        mov    ebx, dword ptr [ebp + 0x48]
        call   sub_7_4344
loc_1_08D5:
        mov    eax, dword ptr [ebx + 8]
        test   byte ptr [eax + 0x1d], 0x20
        jne    loc_1_08EE
loc_1_08DE:
        mov    eax, dword ptr [ebx + 0x10]
        mov    eax, dword ptr [eax + 0xc]
        mov    dword ptr [ebx + 0x10], eax
        push   ebx
        call   dword ptr [eax]
        add    esp, 4
        ret    
loc_1_08EE:
        test   byte ptr [ebx + 0x6e], 1
        jne    loc_1_08DE
        cmp    byte ptr [ebx + 0xb1], 1
        jbe    loc_1_08DE
        movzx  ecx, byte ptr [ebx + 0xb1]
        mov    eax, dword ptr [ecx*4 + 0xdb0] ; -> loc_1_0DB0
        or     eax, eax
        je     loc_1_0918
        movzx  ecx, byte ptr [eax + 0x8a]
        jmp    loc_1_0936
loc_1_0918:
        mov    eax, 0xe44                    ; -> obj1:0x0E44
loc_1_091D:
        mov    eax, dword ptr [eax]
        or     eax, eax
        je     loc_1_08DE
        cmp    byte ptr [eax + 0x89], cl
        jne    loc_1_091D
        mov    cl, byte ptr [eax + 0x8a]
        cmp    cl, 1
        ja     loc_1_08DE
loc_1_0936:
        mov    byte ptr [ebx + 0xb1], cl
        jmp    loc_1_08DE
sub_1_093E:
        call   sub_7_1B8C
        jb     loc_1_094A
        call   sub_7_1CEA
loc_1_094A:
        ret    
loc_1_094B:
        mov    word ptr [ebx + 0x6a], 0
        pop    eax
        jmp    loc_1_08AE
        db  000h
sub_1_0958:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_096C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0974:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_098C:
        mov    esi, dword ptr [0xe28]        ; -> loc_1_0E28
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
        xor    edx, edx
        mov    byte ptr [0xe63], dl          ; -> loc_1_0E63
        mov    esi, 0x9ca                    ; -> loc_1_09CA
        VMMCall  VMM.Call_Global_Event           ; cd2010000100
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
loc_1_09B1:
        cmp    byte ptr [0xe63], 0           ; -> loc_1_0E63
        jne    loc_1_09C2
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        jmp    loc_1_09B1
loc_1_09C2:
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        clc    
        ret    
loc_1_09CA:
        push   ebp
loc_1_09CB:
        cmp    edx, 0x1a
        jae    loc_1_0A3C
        mov    ebp, dword ptr [edx*4 + 0xdb0] ; -> loc_1_0DB0
        inc    edx
        or     ebp, ebp
        je     loc_1_09CB
        push   edx
        mov    edx, 1
        call   sub_7_4AD7
        pop    ecx
        jb     loc_1_09CB
        mov    dword ptr [esi + 0x80], ecx
        dec    ecx
        xor    eax, eax
        mov    word ptr [esi + 0x68], 0x28
        mov    dword ptr [esi + 0xac], eax
        mov    byte ptr [esi + 0xb0], al
        mov    dword ptr [esi + 0x64], eax
        mov    dword ptr [esi + 0xa8], eax
        mov    word ptr [esi + 0x6a], ax
        mov    dword ptr [esi + 0x7c], eax
        mov    dword ptr [esi + 0x6c], 0x400
        mov    byte ptr [esi + 0xb1], cl
        mov    dword ptr [esi + 0x70], 0xa45 ; -> loc_1_0A45
        add    esi, 0x64
        neg    edx
        lea    edx, [edx + esi]
        mov    dword ptr [esi + 0x20], edx
        VxDCall  BLOCKDEV.BlockDev_Send_Command  ; cd2004001000
loc_1_0A3A:
        pop    ebp
        ret    
loc_1_0A3C:
        mov    byte ptr [0xe63], 1           ; -> loc_1_0E63
        jmp    loc_1_0A3A
loc_1_0A45:
        mov    edx, dword ptr [esi + 0x1c]
        mov    esi, dword ptr [esi + 0x20]
        sub    esp, 8
        mov    dword ptr [esp + 4], esi
        mov    word ptr [esp], 4
        mov    esi, esp
        push   esi
        call   sub_1_0958
        add    esp, 0xc
        mov    esi, 0x9ca                    ; -> loc_1_09CA
        push   eax
        push   ebx
        push   ecx
        xor    eax, eax
        mov    ebx, eax
        mov    ecx, 0x408
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
        pop    ecx
        pop    ebx
        pop    eax
        ret    
loc_1_0A7F:
        call   sub_4_1B80
        ret    
        db  000h, 000h, 000h
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  3, 10                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'DRVSPACX'                  ; DDB_Name
        dd  080000000h                  ; DDB_Init_Order
        dd  DRVSPACX_Control         ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_0AE8:
        push   ebx
        jne    0xb5b
        jo     loc_1_0B5C
        jb     loc_1_0B63
        jae    0xb11
        xor    ebp, dword ptr [esi]
        xor    byte ptr [eax], ah
        inc    ebx
        push   esi
        inc    esi
        jae    0xb28
        pop    esp
        outsb  dx, byte ptr [esi]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0B00:
        and    byte ptr [eax], ah
        and    eax, 0x3d632563
        and    byte ptr ds:[0x25282063], ah
        arpl   word ptr [ecx], bp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0B14:
        pop    esp
        outsb  dx, byte ptr [esi]
        add    byte ptr [eax], al
loc_1_0B18:
        pop    ebx
        and    eax, 0x2d205d63
        and    byte ptr [0x6e5c73], ah
loc_1_0B24:
        and    eax, 0x3f6e5c73
        and    byte ptr [eax], al
        db  000h
loc_1_0B2C:
        and    eax, 0x6e5c63
        add    byte ptr [eax], al
        db  000h
loc_1_0B34:
        pop    edx
        jb     0xba7
        and    byte ptr [esi + 0x69], ah
        insb   byte ptr es:[edi], dx
        jae    0xb60
        ja     0xbab
        insb   byte ptr es:[edi], dx
        insb   byte ptr es:[edi], dx
        and    byte ptr [0x20656273], ah
        imul   esi, dword ptr fs:[ebx + 0x70], 0x6579616c
        pop    esp
        outsb  dx, byte ptr [esi]
        add    byte ptr [eax], al
loc_1_0B58:
        push   ebx
        outsd  dx, dword ptr [esi]
        jb     loc_1_0BCE
loc_1_0B5C:
        jns    loc_1_0B8A
        and    byte ptr [ecx + 0x6f], bh
        jne    0xb83
loc_1_0B63:
        outsb  dx, byte ptr [esi]
        and    byte ptr fs:[eax + ebp*2 + 0x65], dh
        and    byte ptr [ebp + 0x62], ah
        jne    0xbd8
        and    byte ptr [edx + esi*2 + 0x69], ah
        jbe    loc_1_0BDC
        jb     0xb99
        outsw  dx, word ptr [esi]
        jb     loc_1_0B9D
        je     0xbe7
        imul   esi, dword ptr [ebx + 0x2e], 0x6e5c
        add    byte ptr [eax], al
loc_1_0B88:
        pop    esp
        outsb  dx, byte ptr [esi]
loc_1_0B8A:
        and    eax, 0x64412073
        jb     0xbf7
        jae    0xc07
        and    byte ptr [0x38302520], bh
        insb   byte ptr es:[edi], dx
        js     loc_1_0BF9
loc_1_0B9D:
        outsb  dx, byte ptr [esi]
        add    byte ptr [eax], al
loc_1_0BA0:
        and    eax, 0x203d2073
        and    eax, 0x786c3430
        add    byte ptr [eax], al
loc_1_0BAC:
        and    eax, 0x203d2073
        and    eax, 0x786c3830
        add    byte ptr [eax], al
loc_1_0BB8:
        and    eax, 0x203d2073
        add    byte ptr [eax], al
        add    byte ptr [0x786c3230], ah
        add    byte ptr [eax], al
        add    byte ptr [0x203d2073], ah
        daa    
loc_1_0BCE:
        add    byte ptr [eax], al
loc_1_0BD0:
        and    eax, 0x25000063
        jae    0xbf7
        cmp    eax, 0
loc_1_0BDC:
        and    byte ptr [0x270073], ah
        add    byte ptr [eax], al
loc_1_0BE4:
        and    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0BE8:
        pop    esp
        outsb  dx, byte ptr [esi]
        add    byte ptr [eax], al
loc_1_0BEC:
        pop    esp
        outsb  dx, byte ptr [esi]
        add    byte ptr [eax], al
loc_1_0BF0:
        sub    cl, byte ptr [eax + 0x45]
        dec    esp
        inc    esp
        sub    bl, byte ptr [esi + ebp*2]
loc_1_0BF9:
        add    byte ptr [eax], al
        db  000h
loc_1_0BFC:
        arpl   word ptr [ecx + 0x5f], sp
        dec    eax
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        and    byte ptr fs:[0x32302520], bh
        insb   byte ptr es:[edi], dx
        js     0xc68
        outsb  dx, byte ptr [esi]
        add    byte ptr [eax], al
        add    byte ptr [edx], ch
        dec    eax
        inc    ebp
        dec    esp
        inc    esp
        sub    bl, byte ptr [esi + ebp*2]
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x63], ah
        pop    edi
        dec    eax
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        and    byte ptr fs:[0x32302520], bh
        insb   byte ptr es:[edi], dx
        js     0xc88
        outsb  dx, byte ptr [esi]
        add    byte ptr [eax], al
        add    byte ptr [ebx + 0x6f], dl
        jb     0xca6
        jns    loc_1_0C62
        and    byte ptr [ecx + 0x6f], bh
        jne    0xc5b
        outsb  dx, byte ptr [esi]
        and    byte ptr fs:[eax + ebp*2 + 0x65], dh
        and    byte ptr [ebp + 0x62], ah
        jne    0xcb0
        and    byte ptr [edx + esi*2 + 0x69], ah
        jbe    0xcb4
        jb     0xc71
        outsw  dx, word ptr [esi]
        jb     0xc75
        je     0xcbf
        imul   esi, dword ptr [ebx + 0x2e], 0x6e5c
        add    byte ptr [eax], al
loc_1_0C60:
        push   ebx
        outsd  dx, dword ptr [esi]
loc_1_0C62:
        jb     0xcd6
        jns    loc_1_0C92
        and    byte ptr [ecx + 0x6f], bh
        jne    0xc8b
        outsb  dx, byte ptr [esi]
        and    byte ptr fs:[eax + ebp*2 + 0x65], dh
        and    byte ptr [ebp + 0x62], ah
        jne    0xce0
        and    byte ptr [edx + esi*2 + 0x69], ah
        jbe    0xce4
        jb     0xca1
        outsw  dx, word ptr [esi]
        jb     0xca5
        je     0xcef
        imul   esi, dword ptr [ebx + 0x2e], 0x6e5c
        add    byte ptr [eax], al
loc_1_0C90:
        push   ebx
        outsd  dx, dword ptr [esi]
loc_1_0C92:
        jb     0xd06
        jns    loc_1_0CC2
        and    byte ptr [ecx + 0x6f], bh
        jne    0xcbb
        outsb  dx, byte ptr [esi]
        and    byte ptr fs:[eax + ebp*2 + 0x65], dh
        and    byte ptr [ebp + 0x62], ah
        jne    loc_1_0D10
        and    byte ptr [edx + esi*2 + 0x69], ah
        jbe    loc_1_0D14
        jb     0xcd1
        outsw  dx, word ptr [esi]
        jb     0xcd5
        je     loc_1_0D1F
        imul   esi, dword ptr [ebx + 0x2e], 0x6e5c
        add    byte ptr [eax], al
loc_1_0CC0:
        push   ebx
        outsd  dx, dword ptr [esi]
loc_1_0CC2:
        jb     0xd36
        jns    loc_1_0CF2
        and    byte ptr [ecx + 0x6f], bh
        jne    0xceb
        outsb  dx, byte ptr [esi]
        and    byte ptr fs:[eax + ebp*2 + 0x65], dh
        and    byte ptr [ebp + 0x62], ah
        jne    loc_1_0D40
        and    byte ptr [edx + esi*2 + 0x69], ah
        jbe    0xd44
        jb     loc_1_0D01
        outsw  dx, word ptr [esi]
        jb     loc_1_0D05
        je     loc_1_0D4F
        imul   esi, dword ptr [ebx + 0x2e], 0x6e5c
        add    byte ptr [eax], al
loc_1_0CF0:
        add    byte ptr [eax], al
loc_1_0CF2:
        add    byte ptr [eax], al
        dd  loc_1_0CF0               ; fixup
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_0D01:
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> obj1:0x0D00
loc_1_0D05:
        or     eax, 0
        add    byte ptr [eax], al
loc_1_0D0C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0D10:
        or     al, 0xd
        add    byte ptr [eax], al
loc_1_0D14:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        inc    eax
        add    byte ptr [eax], al
loc_1_0D1F:
        add    byte ptr [eax], ch
        add    byte ptr [eax], al
        push   0
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0D40:
        inc    eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        db  000h
loc_1_0D4F:
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + 0xd], bl      ; -> obj1:0x0D58
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0D64:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0D68:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0D6C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0D70:
        add    byte ptr [eax], al
loc_1_0D72:
        add    byte ptr [eax], al
loc_1_0D74:
        add    dword ptr [ecx], eax
loc_1_0D76:
        add    byte ptr [eax], al
loc_1_0D78:
        add    byte ptr [eax], al
loc_1_0D7A:
        add    byte ptr [eax], al
loc_1_0D7C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0D90:
        jl     0xd9f
        add    byte ptr [eax], al
loc_1_0D94:
        jl     0xda3
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0DAC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0DB0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0E18:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0E1C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0E20:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0E24:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0E28:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0E2C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0E30:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0E34:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 00Fh, 000h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 0F0h, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        dd  obj7:0x4800              ; fixup
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_0E5F:
        add    dword ptr [ebx], eax
        add    al, byte ptr [esi]
loc_1_0E63:
        add    byte ptr [eax], al
loc_1_0E65:
        add    byte ptr [eax], al
loc_1_0E67:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x3f9  R-X
loc_2_0000:
        or     eax, dword ptr [eax]
        add    byte ptr [eax], al
loc_2_0004:
        and    al, 0
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        xor    al, byte ptr [eax]
        fld    qword ptr [eax]
        xchg   dword ptr [ecx], eax
        and    al, byte ptr [edx]
loc_2_0016:
        lds    eax, ptr [edx]
loc_2_0018:
        salc   
        add    bh, byte ptr [ebx]
        add    ecx, dword ptr [esi + 3]
loc_2_001E:
        mov    ecx, 0xd503c903
        add    eax, dword ptr [edx + esi*2 + 0x69]
        jbe    loc_2_008E
        push   ebx
        jo     0x8d
        arpl   word ptr [ebp + 0x20], sp
        inc    ebp
        jb     0xa4
        outsd  dx, dword ptr [esi]
        jb     0x6f
        and    byte ptr [edx + esi*2 + 0x69], al
        jbe    0xa0
        and    byte ptr [0x203a7331], ah
        sub    byte ptr [0x5c3a7332], ah
        inc    esp
        and    eax, 0x50537333
        inc    ecx
        inc    ebx
        inc    ebp
        and    eax, 0x297334
        inc    ecx
        outsb  dx, byte ptr [esi]
        and    byte ptr [ebp + 0x72], ah
        jb     0xcc
        jb     0x7f
        outsd  dx, dword ptr [esi]
        arpl   word ptr [ebx + 0x75], sp
        jb     loc_2_00D7
        and    byte ptr fs:[edi + 0x68], dh
        imul   ebp, dword ptr [ebp + 0x20], 0x64616572
        imul   ebp, dword ptr [esi + 0x67], 0x20726f20
        ja     loc_2_00ED
        imul   esi, dword ptr [ecx + ebp*2 + 0x6e], 0x68742067
        and    byte ptr gs:[edx + 0x69], al
        je     0xcf
        inc    ecx
        push   esp
        and    byte ptr [edi + 0x6e], ch
loc_2_008E:
        and    byte ptr [eax + ebp*2 + 0x69], dh
        jae    0xb4
        jb     0x100
        jbe    0xfe
        or     eax, 0x206f540a
        imul   di, word ptr [eax + 0x20], 0x6874
        and    byte ptr gs:[eax + 0x72], dh
        outsd  dx, dword ptr [esi]
        bound  ebp, qword ptr [ebp + 0x6d]
        sub    al, 0x20
        jb     0x127
        outsb  dx, byte ptr [esi]
        and    byte ptr [ebx + 0x63], dl
        popal  
        outsb  dx, byte ptr [esi]
        inc    esp
        imul   esi, dword ptr [ebx + 0x6b], 0x646e6120
        and    byte ptr [eax + 0x65], dh
        jb     0x12b
        outsd  dx, dword ptr [esi]
        jb     loc_2_0135
        and    byte ptr [ecx + 0x20], ah
        je     loc_2_0135
        outsd  dx, dword ptr [esi]
        jb     0x13f
        jne    0x139
        push   0x73657420
loc_2_00D7:
        je     loc_2_00F9
        outsd  dx, dword ptr [esi]
        or     ax, 0x640a
        jb     loc_2_0149
        jbe    0x147
        and    byte ptr [0x61207331], ah
        outsb  dx, byte ptr [esi]
        and    byte ptr fs:[ecx + 0x74], ch
loc_2_00ED:
        jae    0x10f
        push   0x2074736f
        jb     loc_2_0160
        jbe    0x15e
loc_2_00F9:
        and    byte ptr [0xd2e7332], ah
        or     al, byte ptr [eax]
        inc    ecx
        outsb  dx, byte ptr [esi]
        and    byte ptr [ebp + 0x72], ah
        jb     loc_2_0177
        jb     0x12a
        outsd  dx, dword ptr [esi]
        arpl   word ptr [ebx + 0x75], sp
        jb     0x182
        and    byte ptr fs:[edi + 0x68], dh
        imul   ebp, dword ptr [ebp + 0x20], 0x64616572
        imul   ebp, dword ptr [esi + 0x67], 0x20726f20
        ja     0x198
        imul   esi, dword ptr [ecx + ebp*2 + 0x6e], 0x68742067
        and    byte ptr gs:[ebp + 0x44], cl
        inc    esi
        inc    ecx
        push   esp
loc_2_0135:
        and    byte ptr [edi + 0x6e], ch
        and    byte ptr [eax + ebp*2 + 0x69], dh
        jae    0x15e
        jb     0x1aa
        jbe    0x1a8
        or     eax, 0x206f540a
loc_2_0149:
        imul   di, word ptr [eax + 0x20], 0x6874
        and    byte ptr gs:[eax + 0x72], dh
        outsd  dx, dword ptr [esi]
        bound  ebp, qword ptr [ebp + 0x6d]
        sub    al, 0x20
        jb     loc_2_01D1
        outsb  dx, byte ptr [esi]
        and    byte ptr [ebx + 0x63], dl
loc_2_0160:
        popal  
        outsb  dx, byte ptr [esi]
        inc    esp
        imul   esi, dword ptr [ebx + 0x6b], 0x646e6120
        and    byte ptr [eax + 0x65], dh
        jb     0x1d5
        outsd  dx, dword ptr [esi]
        jb     loc_2_01DF
        and    byte ptr [ecx + 0x20], ah
        je     loc_2_01DF
loc_2_0177:
        outsd  dx, dword ptr [esi]
        jb     loc_2_01E9
        jne    loc_2_01E3
        push   0x73657420
        je     loc_2_01A3
        outsd  dx, dword ptr [esi]
        or     ax, 0x640a
        jb     0x1f3
        jbe    loc_2_01F1
        and    byte ptr [0x61207331], ah
        outsb  dx, byte ptr [esi]
        and    byte ptr fs:[ecx + 0x74], ch
        jae    0x1b9
        push   0x2074736f
        jb     loc_2_020A
        jbe    0x208
loc_2_01A3:
        and    byte ptr [0xd2e7332], ah
        or     al, byte ptr [eax]
        push   esp
        push   0x20657265
        popal  
        jb     loc_2_0219
        and    byte ptr [ecx + 0x6e], ch
        arpl   word ptr [edi + 0x6e], bp
        jae    0x225
        jae    0x232
        outsb  dx, byte ptr gs:[esi]
        arpl   word ptr [ecx + 0x65], bp
        jae    0x1e5
        imul   ebp, dword ptr [esi + 0x20], 0x20656874
        dec    ebp
        inc    esp
        inc    esi
        inc    ecx
        push   esp
loc_2_01D1:
        and    byte ptr [edi + 0x6e], ch
        and    byte ptr [eax + ebp*2 + 0x69], dh
        jae    loc_2_01FA
        jb     loc_2_0246
        jbe    0x244
loc_2_01DF:
        and    byte ptr cs:[eax], ah
        push   esp
loc_2_01E3:
        outsd  dx, dword ptr [esi]
        and    byte ptr [esi + 0x69], ah
        js     0x209
loc_2_01E9:
        je     loc_2_0253
        or     eax, 0x6f72700a
loc_2_01F1:
        bound  ebp, qword ptr [ebp + 0x6d]
        sub    al, 0x20
        jb     0x26e
        outsb  dx, byte ptr [esi]
loc_2_01FA:
        and    byte ptr [ebx + 0x63], dl
        popal  
        outsb  dx, byte ptr [esi]
        inc    esp
        imul   esi, dword ptr [ebx + 0x6b], 0x646e6120
        and    byte ptr [eax + 0x65], dh
loc_2_020A:
        jb     loc_2_0272
        outsd  dx, dword ptr [esi]
        jb     0x27c
        and    byte ptr [ecx + 0x20], ah
        je     0x27c
        outsd  dx, dword ptr [esi]
        jb     0x286
        jne    0x280
loc_2_0219:
        push   0x73657420
        je     loc_2_0240
        outsd  dx, dword ptr [esi]
        and    byte ptr [edx + esi*2 + 0x69], ah
        jbe    0x28d
        and    byte ptr [0x61207331], ah
        outsb  dx, byte ptr [esi]
        or     eax, 0x7374690a
        and    byte ptr [eax + 0x6f], ch
        jae    loc_2_02AE
        and    byte ptr [edx + esi*2 + 0x69], ah
        jbe    0x2a5
loc_2_0240:
        and    byte ptr [0x2e7332], ah
loc_2_0246:
        inc    ecx
        outsb  dx, byte ptr [esi]
        and    byte ptr [ebp + 0x72], ah
        jb     0x2bc
        jb     0x26f
        outsd  dx, dword ptr [esi]
        arpl   word ptr [ebx + 0x75], sp
loc_2_0253:
        jb     0x2c7
        and    byte ptr fs:[edi + 0x68], dh
        imul   ebp, dword ptr [ebp + 0x20], 0x74697277
        imul   ebp, dword ptr [esi + 0x67], 0x6e6f6320
        je     0x2dd
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        and    byte ptr [ecx + 0x74], ah
        popal  
loc_2_0272:
        and    byte ptr [edi + ebp*2 + 0x20], dh
        je     0x2e0
        imul   esi, dword ptr [ebx + 0x20], 0x76697264
        and    byte ptr cs:[eax], ah
        push   esp
        outsd  dx, dword ptr [esi]
        or     eax, 0x7869660a
        and    byte ptr [eax + ebp*2 + 0x65], dh
        and    byte ptr [eax + 0x72], dh
        outsd  dx, dword ptr [esi]
        bound  ebp, qword ptr [ebp + 0x6d]
        sub    al, 0x20
        jb     loc_2_030F
        outsb  dx, byte ptr [esi]
        and    byte ptr [ebx + 0x63], dl
        popal  
        outsb  dx, byte ptr [esi]
        inc    esp
        imul   esi, dword ptr [ebx + 0x6b], 0x646e6120
        and    byte ptr [eax + 0x65], dh
        jb     0x313
        outsd  dx, dword ptr [esi]
loc_2_02AE:
        jb     0x31d
        and    byte ptr [ecx + 0x20], ah
        je     0x31d
        outsd  dx, dword ptr [esi]
        jb     loc_2_0327
        jne    0x321
        push   0x73657420
        je     loc_2_02E1
        outsd  dx, dword ptr [esi]
        or     ax, 0x640a
        jb     0x331
        jbe    loc_2_032F
        and    byte ptr [0x61207331], ah
        outsb  dx, byte ptr [esi]
        and    byte ptr fs:[ecx + 0x74], ch
        jae    loc_2_02F7
        push   0x2074736f
        jb     0x348
        jbe    0x346
loc_2_02E1:
        and    byte ptr [0xd2e7332], ah
        or     al, byte ptr [eax]
        inc    esp
        jb     0x355
        jbe    loc_2_0353
        push   ebx
        jo     loc_2_0352
        arpl   word ptr [ebp + 0x20], sp
        inc    ebp
        jb     0x369
loc_2_02F7:
        outsd  dx, dword ptr [esi]
        jb     loc_2_02FA
loc_2_02FA:
        push   esp
        push   0x72442065
        imul   esi, dword ptr [esi + 0x65], 0x63617053
        and    byte ptr gs:[esi + 0x78], dl
        inc    esp
        and    byte ptr [eax + 0x61], ch
loc_2_030F:
        jae    0x331
        outsd  dx, dword ptr [esi]
        jbe    loc_2_0379
        jb     loc_2_037C
        insb   byte ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        ja     0x37f
        and    byte ptr fs:[ecx + 0x74], ch
        jae    loc_2_0340
        imul   ebp, dword ptr [esi + 0x74], 0x616e7265
loc_2_0327:
        insb   byte ptr es:[edi], dx
        and    byte ptr [ebx + 0x74], dh
        popal  
        arpl   word ptr [ebx + 0x2e], bp
loc_2_032F:
        or     eax, 0x6265520a
        outsd  dx, dword ptr [esi]
        outsd  dx, dword ptr [esi]
        je     0x364
        and    byte ptr [edx + 0x75], dh
        outsb  dx, byte ptr [esi]
        and    byte ptr [ebx + 0x63], dl
        popal  
loc_2_0340:
        outsb  dx, byte ptr [esi]
        inc    esp
        imul   esi, dword ptr [ebx + 0x6b], 0x6e61202c
        and    byte ptr fs:[ebx + 0x68], ah
        arpl   word ptr gs:[ebx + 0x20], bp
        popal  
loc_2_0352:
        insb   byte ptr es:[edi], dx
loc_2_0353:
        insb   byte ptr es:[edi], dx
        and    byte ptr [edx + esi*2 + 0x69], ah
        jbe    loc_2_03BF
        jae    0x38a
        or     eax, 0x7244000a
        imul   esi, dword ptr [esi + 0x65], 0x63617053
        and    byte ptr gs:[edi + 0x61], dl
        jb     0x3dc
        imul   ebp, dword ptr [esi + 0x67], 0x65685400
        and    byte ptr [edx + esi*2 + 0x69], al
loc_2_0379:
        jbe    loc_2_03E0
        push   ebx
loc_2_037C:
        jo     loc_2_03DF
        arpl   word ptr [ebp + 0x20], sp
        push   esi
        js     0x3c8
        and    byte ptr [ecx + 0x6e], ah
        and    byte ptr fs:[eax + ebp*2 + 0x65], dh
        and    byte ptr [edx + esi*2 + 0x69], al
        jbe    loc_2_03F7
        push   ebx
        jo     loc_2_03F6
        arpl   word ptr [ebp + 0x20], sp
        jb     0x3ff
        popal  
        insb   byte ptr es:[edi], dx
        and    byte ptr [ebp + 0x6f], ch
        and    byte ptr gs:[edx + esi*2 + 0x69], ah
        jbe    0x40c
        jb     0x3b6
        or     ah, byte ptr [ecx + 0x72]
        and    byte ptr gs:[ebp + 0x69], ch
        jae    0x41f
        popal  
        je     0x418
        push   0x202e6465
        and    byte ptr [ecx + 0x6f], bl
        jne    loc_2_03DF
loc_2_03BF:
        insd   dword ptr es:[edi], dx
        popal  
        jns    loc_2_03E3
        outsb  dx, byte ptr [esi]
        and    byte ptr fs:[edi + ebp*2 + 0x20], dh
        jb     0x432
        imul   ebp, dword ptr [esi + 0x73], 0x6c6c6174
        and    byte ptr [eax + ebp*2 + 0x65], dh
        insd   dword ptr es:[edi], dx
        or     eax, 0x3a41000a
loc_2_03DF:
        pop    esp
loc_2_03E0:
        push   edx
        inc    ebp
        inc    ecx
loc_2_03E3:
        inc    esp
        push   esp
        dec    eax
        dec    ecx
        push   ebx
        push   esp
        pop    eax
        push   esp
        add    byte ptr [edx + 0x45], dl
        inc    ecx
        inc    esp
        push   esp
        dec    eax
        dec    ecx
        push   ebx
        push   esp
loc_2_03F6:
        pop    eax
loc_2_03F7:
        push   esp
        db  000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x924  R-X
loc_3_0000:
        xor    dl, dl
        mov    eax, 1
        int    0x41
        ret    
        db  0B8h
        dd  loc_6_177A               ; fixup
        db  080h, 035h
        dd  loc_6_1779               ; fixup
        db  001h, 075h, 003h, 083h, 0C0h, 004h, 050h, 068h
        dd  loc_1_0B34               ; fixup
        db  0E8h
        dd  sub_1_0214               ; fixup
        db  083h, 0C4h, 008h, 0C3h, 068h
        dd  loc_1_0B58               ; fixup
        db  0E8h
        dd  sub_1_0214               ; fixup
        db  083h, 0C4h, 004h, 0C3h, 083h, 0ECh, 00Ch, 0FCh, 0ADh, 089h, 004h, 024h, 0B8h, 04Fh, 000h, 000h
        db  000h, 033h, 0D2h, 0F7h, 034h, 024h, 089h, 044h, 024h, 004h, 0B8h, 04Fh, 000h, 000h, 000h, 02Bh
        db  0C2h, 02Bh, 044h, 024h, 004h, 089h, 044h, 024h, 008h, 0ADh, 08Bh, 0C8h, 0ACh, 053h, 056h, 068h
        dd  loc_1_0B88               ; fixup
        db  0E8h
        dd  sub_1_0214               ; fixup
        db  083h, 0C4h, 00Ch, 00Fh, 0B6h, 0C0h, 003h, 0F0h, 0C7h, 005h
        dd  obj1:0x0AE4              ; fixup
        db  000h, 000h, 000h, 000h
loc_3_007F:
        lodsd  eax, dword ptr [esi]
        movzx  edi, ax
        shr    eax, 0x10
        mov    edx, eax
        lodsb  al, byte ptr [esi]
        cmp    dx, -1
        je     loc_3_00BD
        cmp    edx, 2
        jne    loc_3_00C2
        movzx  edx, word ptr [edi + ebx]
        cmp    byte ptr [0x1779], 0          ; -> loc_6_1779
        je     loc_3_00A9
        or     edx, edx
        je     loc_3_0182
loc_3_00A9:
        push   edx
        push   esi
        push   0xba0                         ; -> loc_1_0BA0
        call   sub_1_0214
        add    esp, 0xc
        jmp    loc_3_01D9
loc_3_00BD:
        lea    edx, [edi + ebx]
        jmp    loc_3_00DB
loc_3_00C2:
        cmp    edx, 4
        jne    loc_3_00EF
        mov    edx, dword ptr [edi + ebx]
        cmp    byte ptr [0x1779], 0          ; -> loc_6_1779
        je     loc_3_00DB
        or     edx, edx
        je     loc_3_0182
loc_3_00DB:
        push   edx
        push   esi
        push   0xbac                         ; -> loc_1_0BAC
        call   sub_1_0214
        add    esp, 0xc
        jmp    loc_3_01D9
loc_3_00EF:
        test   edx, 0xc000
        jne    loc_3_0139
        cmp    byte ptr [0x1779], 0          ; -> loc_6_1779
        je     loc_3_010B
        cmp    edx, 1
        jne    loc_3_010B
        cmp    byte ptr [edi + ebx], 0
        je     loc_3_0182
loc_3_010B:
        push   esi
        push   0xbb8                         ; -> loc_1_0BB8
        call   sub_1_0214
        add    esp, 8
loc_3_0119:
        or     edx, edx
        je     loc_3_01D9
        push   eax
        movzx  eax, byte ptr [edi + ebx]
        push   eax
        push   0xbc0                         ; -> obj1:0x0BC0
        call   sub_1_0214
        db  083h, 0C4h
loc_3_0133:
        or     byte ptr [eax + 0x47], bl
        dec    edx
        jmp    loc_3_0119
loc_3_0139:
        cmp    edx, 0xc000
        jae    loc_3_0193
        push   esi
        push   0xbc8                         ; -> obj1:0x0BC8
        call   sub_1_0214
        add    esp, 8
        test   edx, 0x8000
        je     loc_3_0160
        mov    edi, dword ptr [edi + ebx]
        or     edi, edi
        je     loc_3_01CC
        sub    edi, ebx
loc_3_0160:
        and    edx, 0x3fff
        je     loc_3_01CC
        cmp    byte ptr [edi + ebx], 0
        je     loc_3_01CC
        push   dword ptr [edi + ebx]
        push   0xbd0                         ; -> loc_1_0BD0
        call   sub_1_0214
        add    esp, 8
        inc    edi
        dec    edx
        jmp    loc_3_0160
loc_3_0182:
        movzx  eax, al
        add    esi, eax
        dec    ecx
        je     loc_3_0229
        jmp    loc_3_007F
loc_3_0193:
        push   esi
        push   0xbd4                         ; -> obj1:0x0BD4
        call   sub_1_0214
        add    esp, 8
        movzx  eax, al
        add    esi, eax
        xor    edx, edx
loc_3_01A8:
        cmp    byte ptr [esi], 0xff
        je     loc_3_01C9
        bt     dword ptr [edi + ebx], edx
        jae    loc_3_01C1
        push   esi
        push   0xbdc                         ; -> loc_1_0BDC
        call   sub_1_0214
        add    esp, 8
loc_3_01C1:
        lodsb  al, byte ptr [esi]
        or     al, al
        jne    loc_3_01C1
        inc    edx
        jmp    loc_3_01A8
loc_3_01C9:
        inc    esi
        jmp    loc_3_01DE
loc_3_01CC:
        push   0xbe0                         ; -> obj1:0x0BE0
        call   sub_1_0214
        add    esp, 4
loc_3_01D9:
        movzx  eax, al
        add    esi, eax
loc_3_01DE:
        dec    ecx
        je     loc_3_0229
        mov    eax, dword ptr [0xae4]        ; -> obj1:0x0AE4
        cmp    eax, dword ptr [esp + 8]
        jae    loc_3_020D
        xor    edx, edx
        div    dword ptr [esp + 4]
        neg    edx
        add    edx, dword ptr [esp + 4]
loc_3_01F8:
        push   0xbe4                         ; -> loc_1_0BE4
        call   sub_1_0214
        add    esp, 4
        dec    edx
        jne    loc_3_01F8
        jmp    loc_3_007F
loc_3_020D:
        push   0xbe8                         ; -> loc_1_0BE8
        call   sub_1_0214
        add    esp, 4
        mov    dword ptr [0xae4], 0          ; -> obj1:0x0AE4
        jmp    loc_3_007F
loc_3_0229:
        push   0xbec                         ; -> loc_1_0BEC
        call   sub_1_0214
        add    esp, 4
        add    esp, 0xc
        ret    
        db  002h, 000h, 000h, 000h, 04Fh, 000h, 000h, 000h, 003h, 044h, 055h, 000h, 000h, 001h, 0FFh, 0FFh
        db  009h, 026h, 075h, 05Fh, 061h, 066h, 072h, 061h, 067h, 000h, 004h, 002h, 0FFh, 0FFh, 008h, 026h
        db  075h, 05Fh, 061h, 070h, 063h, 063h, 000h, 0D8h, 000h, 004h, 000h, 00Dh, 075h, 05Fh, 042h, 069h
        db  074h, 046h, 041h, 054h, 050h, 061h, 067h, 065h, 000h, 0AEh, 000h, 004h, 000h, 015h, 075h, 05Fh
        db  042h, 050h, 042h, 042h, 069h, 067h, 054h, 06Fh, 074h, 061h, 06Ch, 053h, 065h, 063h, 074h, 06Fh
        db  072h, 073h, 000h, 01Ch, 000h, 004h, 000h, 00Ah, 075h, 05Fh, 063h, 062h, 043h, 06Fh, 064h, 065h
        db  064h, 000h, 038h, 002h, 004h, 000h, 00Ah, 075h, 05Fh, 063h, 062h, 053h, 074h, 061h, 063h, 06Bh
        db  000h, 064h, 000h, 004h, 000h, 00Fh, 075h, 05Fh, 063h, 063h, 06Ch, 075h, 073h, 046h, 075h, 06Ch
        db  06Ch, 04Dh, 069h, 06Eh, 000h, 07Ch, 000h, 004h, 000h, 00Ch, 075h, 05Fh, 063h, 065h, 06Eh, 074h
        db  04Dh, 044h, 046h, 041h, 054h, 000h, 09Fh, 000h, 002h, 000h, 00Bh, 075h, 05Fh, 063h, 065h, 06Eh
        db  074h, 052h, 06Fh, 06Fh, 074h, 000h, 0FCh, 000h, 004h, 000h, 008h, 075h, 05Fh, 063h, 066h, 072h
        db  061h, 067h, 000h, 028h, 000h, 004h, 000h, 00Fh, 075h, 05Fh, 043h, 06Ch, 075h, 073h, 04Ch, 061h
        db  073h, 074h, 055h, 073h, 065h, 064h, 000h, 018h, 000h, 004h, 000h, 00Ch, 075h, 05Fh, 063h, 073h
        db  065h, 063h, 043h, 06Fh, 064h, 065h, 064h, 000h, 0F2h, 000h, 001h, 000h, 011h, 075h, 05Fh, 063h
        db  073h, 065h, 063h, 043h, 06Fh, 06Eh, 074h, 069h, 067h, 046h, 061h, 069h, 06Ch, 000h, 0A4h, 000h
        db  002h, 000h, 00Ah, 075h, 05Fh, 063h, 073h, 065h, 063h, 046h, 041h, 054h, 000h, 054h, 000h, 004h
        db  000h, 00Bh, 075h, 05Fh, 063h, 073h, 065h, 063h, 048h, 065h, 061h, 070h, 000h, 058h, 000h, 004h
        db  000h, 00Fh, 075h, 05Fh, 063h, 073h, 065h, 063h, 048h, 065h, 061h, 070h, 046h, 072h, 065h, 065h
        db  000h, 05Ch, 000h, 004h, 000h, 00Eh, 075h, 05Fh, 063h, 073h, 065h, 063h, 048h, 065h, 061h, 070h
        db  04Dh, 069h, 06Eh, 000h, 0CCh, 000h
loc_3_0370:
        add    al, 0
        or     esi, dword ptr [ebp + 0x5f]
        arpl   word ptr [ebx + 0x65], si
        arpl   word ptr [ebp + 0x66], cx
        je     loc_3_037E
loc_3_037E:
        adc    al, 0
        add    al, 0
        or     al, 0x75
        pop    edi
        arpl   word ptr [ebx + 0x65], si
        arpl   word ptr [eax + 0x6c], dx
        popal  
        imul   ebp, dword ptr [esi], 0x2009c
        pcmpeqw mm3, qword ptr [edi + 0x63]
        jae    loc_3_03FE
        arpl   word ptr [edx + 0x65], dx
        jae    loc_3_0403
        jb     loc_3_0416
        add    byte ptr fs:[eax], dl
        add    al, 0
        or     al, 0x75
        pop    edi
        arpl   word ptr [ebx + 0x65], si
        arpl   word ptr [edi + ebp*2 + 0x74], dx
        popal  
        insb   byte ptr es:[edi], dx
        add    al, cl
        add    byte ptr [eax + eax], al
        push   cs
        jne    loc_3_041A
        arpl   word ptr [ebx + 0x65], si
        arpl   word ptr [ebp + 0x73], dx
        push   ebx
        inc    edi
        inc    esp
        add    byte ptr [eax + eax + 0x75090002], dh
        pop    edi
        inc    ebx
        push   esi
        inc    esi
        push   esi
        jb     loc_3_03D5
loc_3_03D5:
        mov    dword ptr [eax], eax
        add    dword ptr [eax], eax
        or     dh, byte ptr [ebp + 0x5f]
        inc    esp
        jb     loc_3_0448
        jbe    0x446
        dec    esi
        outsd  dx, dword ptr [esi]
        add    al, bh
        add    byte ptr [eax + eax], al
        adc    dword ptr [ebp + 0x5f], esi
        inc    ebp
        inc    ebx
        push   edx
        inc    esp
        outsb  dx, byte ptr gs:[esi]
        outsd  dx, dword ptr [esi]
        insd   dword ptr es:[edi], dx
        imul   ebp, dword ptr [esi + 0x61], 0x726f74
        hlt    
        add    byte ptr [eax + eax], al
loc_3_03FE:
        pcmpeqw mm3, qword ptr [edi + 0x45]
        inc    ebx
loc_3_0403:
        push   edx
        dec    esi
        jne    0x474
        jb     loc_3_046B
        je     0x47b
        jb     loc_3_040E
loc_3_040E:
        js     loc_3_0410
loc_3_0410:
        add    al, 0
        adc    dh, byte ptr [ebp + 0x5f]
        inc    esi
loc_3_0416:
        inc    ecx
        push   esp
        je     loc_3_0489
loc_3_041A:
        dec    ebp
        inc    esp
        inc    esi
        inc    ecx
        push   esp
        inc    esp
        insb   byte ptr es:[edi], dx
        je     loc_3_0485
        add    byte ptr [eax + eax], al
        add    al, 0xc0
        or     byte ptr [ebp + 0x5f], dh
        inc    esi
        insb   byte ptr es:[edi], dx
        popal  
        jae    loc_3_0432
loc_3_0432:
        push   ebx
        push   edi
        push   eax
        add    byte ptr [esi + 0x31], al
        xor    al, byte ptr [eax]
        push   edx
        inc    ebp
        dec    ebp
        add    byte ptr [eax + 0x4d], dl
        dec    edi
        add    byte ptr [edx + 0x41], al
        inc    esp
        add    byte ptr [ebx + 0x48], al
loc_3_0448:
        inc    esp
        add    byte ptr [ebx + 0x4c], al
        inc    esp
        add    byte ptr [ebx + 0x44], al
        push   edx
        add    byte ptr [ebx + 0x44], al
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [edi + 0x52], dl
        inc    esp
        add    byte ptr [edi + 0x52], dl
        dec    ebp
        add    byte ptr [edx + 0x49], al
        push   esp
        add    byte ptr [eax], al
        inc    esi
        dec    ecx
        dec    esi
        add    byte ptr [eax], al
        inc    ebx
loc_3_046B:
        dec    esp
        dec    esi
        add    bh, bh
        add    al, 0
        add    al, 0
        or     byte ptr [ebp + 0x5f], dh
        inc    esi
        insb   byte ptr es:[edi], dx
        popal  
        jae    loc_3_047C
loc_3_047C:
        dec    esp
        add    byte ptr [eax + eax], al
        adc    dh, byte ptr [ebp + 0x5f]
        dec    ecx
        outsb  dx, byte ptr [esi]
loc_3_0485:
        je     loc_3_04EC
        jb     loc_3_04F7
loc_3_0489:
        popal  
        insb   byte ptr es:[edi], dx
        push   ebx
        arpl   word ptr gs:[edi + ebp*2 + 0x72], si
        jae    loc_3_0493
loc_3_0493:
        fadd   qword ptr [eax]
        add    al, 0
        or     esi, dword ptr [ebp + 0x5f]
        dec    ecx
        dec    edi
        push   eax
        inc    esi
        insb   byte ptr es:[edi], dx
        popal  
        jae    loc_3_04A3
loc_3_04A3:
        lock add byte ptr [edx], al
        add    byte ptr [ebx], cl
        jne    loc_3_0509
        dec    ecx
        dec    edi
        push   ebx
        je     loc_3_0510
        je     loc_3_0526
        jae    loc_3_04B3
loc_3_04B3:
        add    byte ptr [eax], 4
        add    byte ptr [edi], cl
        jne    0x519
        imul   esi, dword ptr [eax + 0x67], 0x42727543
        imul   esi, dword ptr [esi + eax*2 + 0x41], 0x840054
        add    al, 0
        adc    byte ptr [ebp + 0x5f], dh
        imul   esi, dword ptr [eax + 0x67], 0x7473614c
        inc    edx
        imul   esi, dword ptr [esi + eax*2 + 0x41], 0x600054
        add    al, 0
        or     esi, dword ptr [ebp + 0x5f]
        imul   esi, dword ptr [ebx + 0x65], 0x61654863
        jo     loc_3_04EC
loc_3_04EC:
        mov    word ptr [eax], es
        add    dword ptr [eax], eax
        adc    byte ptr [ebp + 0x5f], dh
        insb   byte ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        push   ebx
loc_3_04F7:
        arpl   word ptr gs:[eax + 0x65], dx
        jb     0x540
        insb   byte ptr es:[edi], dx
        jne    loc_3_0573
        add    byte ptr [ebx + 0x14000100], cl
        jne    0x567
        insb   byte ptr es:[edi], dx
loc_3_0509:
        outsd  dx, dword ptr [esi]
        dec    eax
        outsd  dx, dword ptr [esi]
        jae    0x583
        push   ebx
loc_3_0510:
        arpl   word ptr gs:[edi + ebp*2 + 0x72], si
        push   ebx
        imul   edi, dword ptr [edx + 0x65], 0x1001100
        add    byte ptr [esi], cl
        jne    0x580
        insd   dword ptr es:[edi], dx
        arpl   word ptr fs:[ebx + 0x65], si
loc_3_0526:
        arpl   word ptr [ebx + 0x6f], sp
        add    byte ptr fs:[eax], dl
        add    byte ptr [ecx], al
        add    byte ptr [esi], cl
        jne    0x593
        insd   dword ptr es:[edi], dx
        arpl   word ptr fs:[ebx + 0x65], si
        arpl   word ptr [eax + 0x6c], si
        popal  
        imul   ebp, dword ptr [esi], 0x40008
        or     eax, 0x444d5f75
        inc    esi
        inc    ecx
        push   esp
        dec    ecx
        outsb  dx, byte ptr [esi]
        js     loc_3_0552
loc_3_0552:
        sldt   word ptr [ecx]
        ror    byte ptr [edx], 0x75
        pop    edi
        insd   dword ptr es:[edi], dx
        insb   byte ptr es:[edi], dx
        popal  
        jae    loc_3_0561
loc_3_0561:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [esi + 0x52], al
        inc    edi
        add    byte ptr [ebp + 0x4e], dl
        inc    ebx
        add    byte ptr [ecx + 0x4c], al
        dec    esp
        add    bh, bh
loc_3_0573:
        or     al, 0
        add    al, 0
        or     eax, 0x646d5f75
        jae    loc_3_05E3
        arpl   word ptr [esi + 0x6c], sp
        popal  
        jae    loc_3_0585
loc_3_0585:
        lea    eax, [eax]
        add    dword ptr [eax], eax
        adc    byte ptr [ebp + 0x5f], dh
        dec    ebp
        imul   esp, dword ptr fs:[ecx + 0x44], 0x72637365
        imul   esi, dword ptr [eax + 0x74], 0x4002000
        add    byte ptr [edx], cl
        jne    0x600
        outsd  dx, dword ptr [esi]
        inc    sp
        popal  
        je     loc_3_0609
        add    byte ptr [eax + eax], ah
        add    al, 0
        or     al, 0x75
        pop    edi
        jo     0x614
        inc    ebx
        insb   byte ptr es:[edi], dx
        jne    0x629
        je     0x61d
        jb     loc_3_05BA
loc_3_05BA:
        cmp    al, 2
        add    al, 0
        or     dh, byte ptr [ebp + 0x5f]
        jo     0x625
        push   ebx
        je     loc_3_0627
        arpl   word ptr [ebx], bp
        sub    al, 0
        add    al, 0
        or     al, 0x75
        pop    edi
        jo     loc_3_0635
        popal  
        inc    edx
        imul   esi, dword ptr [esi + eax*2 + 0x41], 0xe40054
        add    al, 0
        or     esi, dword ptr [ebp + 0x5f]
        jo     0x646
loc_3_05E3:
        popal  
        inc    esi
        insb   byte ptr es:[edi], dx
        jne    loc_3_065B
        push   0x4003000
        add    byte ptr [ebx], cl
        jne    0x650
        jo     loc_3_0656
        popal  
        dec    ebp
        inc    esp
        inc    esi
        inc    ecx
        push   esp
        add    byte ptr [eax], bh
        add    byte ptr [eax + eax], al
        or     esi, dword ptr [ebp + 0x5f]
        jo     0x667
        arpl   word ptr [edx + 0x48], sp
        outsd  dx, dword ptr [esi]
        jae    loc_3_067D
loc_3_0609:
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], al
        or     dh, byte ptr [ebp + 0x5f]
        jo     loc_3_0677
        jne    0x663
        js     loc_3_068C
        add    byte ptr [edx + eax + 4], al
        add    byte ptr [edi], cl
        jne    0x67f
        jo     0x686
        jne    0x672
        js     0x69b
loc_3_0627:
        push   ecx
        jne    0x68f
        jne    loc_3_0691
        add    byte ptr [esi + 0xf000200], dh
        jne    loc_3_0693
        push   eax
loc_3_0635:
        outsb  dx, byte ptr gs:[esi]
        imul   ebp, dword ptr fs:[esi + 0x67], 0x6f727245
        jb     loc_3_0641
loc_3_0641:
        xor    al, 0
        add    al, 0
        or     al, 0x75
        pop    edi
        jo     loc_3_06B0
        outsb  dx, byte ptr [esi]
        inc    esp
        imul   esi, dword ptr [ebx + 0x6b], 0x48004f49
        add    byte ptr [eax + eax], al
loc_3_0656:
        or     esi, dword ptr [ebp + 0x5f]
        jo     loc_3_06C4
loc_3_065B:
        outsd  dx, dword ptr [esi]
        jo     loc_3_06AD
        jb     0x6c9
        add    byte ptr [bx + si + 2], al
        add    al, 0
        or     esi, dword ptr [ebp + 0x5f]
        jo     0x6d4
        outsd  dx, dword ptr [esi]
        jo     0x6c5
        outsd  dx, dword ptr [esi]
        jb     0x6dc
        add    byte ptr [esi + 0xcffff00], cl
loc_3_0677:
        jne    loc_3_06D9
        push   eax
        insb   byte ptr es:[edi], dx
        popal  
loc_3_067D:
        imul   ebp, dword ptr [esi + 0x42], 0xb8004250
        add    byte ptr [eax + eax], al
        or     dword ptr [ebp + 0x5f], esi
        jo     0x6fe
loc_3_068C:
        insd   dword ptr es:[edi], dx
        pop    edi
        jne    loc_3_0691
loc_3_0691:
        rol    byte ptr [eax], 1
loc_3_0693:
        add    al, 0
        or     dh, byte ptr [ebp + 0x5f]
        jo     loc_3_070D
        inc    ebx
        jne    0x711
        add    byte ptr [eax + eax], bh
        add    al, 0
        pop    es
        jne    0x706
        jo     0x71f
        jb     0x71b
        add    al, ah
loc_3_06AD:
        add    byte ptr [eax + eax], al
loc_3_06B0:
        or     esi, dword ptr [ebp + 0x5f]
        jo     loc_3_072C
        push   ebx
        je     0x719
        je     0x72f
        jae    loc_3_06BC
loc_3_06BC:
        mov    bl, 0
        add    dword ptr [eax], eax
        or     byte ptr [ebp + 0x5f], dh
        push   edx
loc_3_06C4:
        jno    0x710
        inc    esp
        add    byte ptr [eax + eax + 4], dh
        add    byte ptr [ebx], cl
        jne    0x72f
        jae    0x737
        arpl   word ptr [ebx + 0x6c], ax
        jne    loc_3_074A
        xor    al, byte ptr [eax]
loc_3_06D9:
        aam    0
        add    al, 0
        or     dword ptr [ebp + 0x5f], esi
        jae    0x747
        arpl   word ptr [ebx + 0x75], ax
        jb     loc_3_06E7
loc_3_06E7:
        insb   byte ptr es:[edi], dx
        add    byte ptr [eax + eax], al
        or     dh, byte ptr [ebp + 0x5f]
        jae    loc_3_0755
        arpl   word ptr [eax + 0x65], cx
        popal  
        jo     loc_3_06F6
loc_3_06F6:
        push   0xb000400
        jne    0x75c
        jae    0x764
        arpl   word ptr [ebp + 0x44], cx
        inc    esi
        inc    ecx
        push   esp
        add    byte ptr [ebx + 0xd000100], bl
        jne    0x76c
loc_3_070D:
        push   ebx
        arpl   word ptr gs:[eax + 0x65], dx
        jb     0x757
        insb   byte ptr es:[edi], dx
        jne    loc_3_078A
        add    byte ptr [eax], dh
        add    al, 0
        pcmpeqw mm3, qword ptr [edi + 0x73]
        arpl   word ptr gs:[edx + 0x6f], dx
        outsd  dx, dword ptr [esi]
        je     loc_3_0768
        jo     loc_3_0799
        popal  
        jb     loc_3_072C
loc_3_072C:
        mov    esp, 0xc000400
        jne    0x792
        push   ebx
        arpl   word ptr gs:[edi + ebp*2 + 0x72], si
        dec    ebp
        popal  
        jo     loc_3_073D
loc_3_073D:
        rol    byte ptr [eax], 4
        add    byte ptr [edi], cl
        jne    loc_3_07A3
        push   ebx
        arpl   word ptr gs:[edi + ebp*2 + 0x72], si
loc_3_074A:
        dec    ebp
        popal  
        jo     0x793
        outsb  dx, byte ptr [esi]
        add    ah, al
        add    byte ptr [eax + eax], al
loc_3_0755:
        adc    dh, byte ptr [ebp + 0x5f]
        push   ebx
        arpl   word ptr gs:[edi + ebp*2 + 0x72], si
        dec    ebp
        popal  
        jo     loc_3_07A7
        outsb  dx, byte ptr [esi]
        dec    ebp
        popal  
        js     loc_3_0768
loc_3_0768:
        cdq    
        add    byte ptr [edx], al
        add    byte ptr [0x65535f75], cl
        arpl   word ptr [edi + ebp*2 + 0x72], si
        push   ebx
        imul   edi, dword ptr [edx + 0x65], 0x100b200
        add    byte ptr [0x65535f75], cl
        jno    loc_3_07FA
        outsb  dx, byte ptr gs:[esi]
        arpl   word ptr [ebp + 0x4e], sp
loc_3_078A:
        outsd  dx, dword ptr [esi]
        add    byte ptr [eax + 2], cl
        add    al, 0x40
        or     eax, 0x74535f75
        popal  
        arpl   word ptr [ebx + 0x46], bp
loc_3_0799:
        outsb  dx, byte ptr gs:[esi]
        arpl   word ptr [ebp], sp
        dec    esp
        add    bh, bh
        dec    dword ptr [ecx]
loc_3_07A3:
        jne    0x805
        push   ebx
loc_3_07A7:
        je     loc_3_080A
        arpl   word ptr [ebx], bp
loc_3_07AC:
        mov    byte ptr [eax], al
        add    dword ptr [eax], eax
        or     al, 0x75
        pop    edi
        push   esp
        push   0x72447369
        imul   esi, dword ptr [esi + 0x65], 0x1008a00
        add    byte ptr [ebx], cl
        jne    loc_3_0823
        push   esp
        jb     0x83c
        dec    eax
        outsd  dx, dword ptr [esi]
        jae    loc_3_0840
        add    byte ptr [eax], cl
        add    al, 0
        adc    esi, dword ptr [ebp + 0x5f]
loc_3_07D4:
        push   ebp
        outsb  dx, byte ptr [esi]
        imul   esi, dword ptr [ebx + edx*2 + 0x65], 0x6c772e6d
loc_3_07DE:
        pop    edi
        outsd  dx, dword ptr [esi]
        ja     loc_3_0850
        jb     loc_3_07E5
loc_3_07E5:
        inc    eax
        add    byte ptr [eax + eax], al
        push   ss
        jne    0x84b
        push   ebp
        outsb  dx, byte ptr [esi]
        imul   esi, dword ptr [ebx + edx*2 + 0x65], 0x6c772e6d
        pop    edi
        jo     loc_3_0862
loc_3_07F9:
        outsd  dx, dword ptr [esi]
loc_3_07FA:
        jo     loc_3_0864
        popal  
        add    byte ptr fs:[edx], al
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], bh
        add    byte ptr [eax], al
        push   es
        push   ebx
loc_3_080A:
        push   esp
        inc    ecx
        push   esp
        push   ebx
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], al
        or     eax, 0x5f617473
        inc    edx
        imul   esp, dword ptr [edi + 0x52], 0x73646165
        add    byte ptr [eax + eax], al
loc_3_0823:
        add    al, 0
        adc    esi, dword ptr [ebx + 0x74]
        popal  
        pop    edi
        inc    edx
        imul   esp, dword ptr [edi + 0x52], 0x53646165
        arpl   word ptr gs:[edi + ebp*2 + 0x72], si
        jae    loc_3_0839
loc_3_0839:
        or     byte ptr [eax], al
        add    al, 0
        adc    byte ptr [ebx + 0x74], dh
loc_3_0840:
        popal  
        pop    edi
        inc    edx
        jne    loc_3_08AE
        insb   byte ptr es:[edi], dx
        inc    edx
        imul   esi, dword ptr [esi + eax*2 + 0x41], 0xc0054
loc_3_0850:
        add    al, 0
        db  00Fh, 073h, 074h, 061h, 05Fh, 043h, 061h, 063h
loc_3_085A:
        push   0x756f4665
        outsb  dx, byte ptr [esi]
        db  064h, 000h
loc_3_0862:
        adc    byte ptr [eax], al
loc_3_0864:
        add    al, 0
        adc    byte ptr [ebx + 0x74], dh
        popal  
loc_3_086A:
        pop    edi
        inc    ebx
        popal  
        arpl   word ptr [eax + 0x65], bp
        dec    eax
        imul   ebp, dword ptr [esi + 0x74], 0x14004b4f
        add    byte ptr [eax + eax], al
        adc    eax, 0x5f617473
        inc    ebx
        popal  
        arpl   word ptr [eax + 0x65], bp
        dec    eax
        imul   ebp, dword ptr [esi + 0x74], 0x63736944
        popal  
        jb     loc_3_08F4
        add    byte ptr [eax], bl
        add    byte ptr [eax + eax], al
        push   cs
        jae    loc_3_090C
        popal  
        pop    edi
        inc    ebx
        popal  
        arpl   word ptr [eax + 0x65], bp
        push   edx
        popal  
        add    byte ptr fs:[eax + eax], bl
        add    al, 0
        push   cs
        jae    loc_3_091F
        popal  
        pop    edi
        inc    ebx
loc_3_08AE:
        popal  
        arpl   word ptr [eax + 0x65], bp
        push   edi
        popal  
        imul   esi, dword ptr [eax + eax + 0x20], 0xf000400
        jae    0x932
        popal  
        pop    edi
        inc    ebx
        popal  
        arpl   word ptr [eax + 0x65], bp
        push   edi
        jb     0x931
        je     0x92f
        add    byte ptr [eax], ch
        add    byte ptr [eax + eax], al
        db  00Fh
loc_3_08D0:
        jae    0x946
        popal  
        pop    edi
        inc    ebx
        push   0x526b6365
        jne    0x950
        add    byte ptr gs:[eax + eax], ch
        add    al, 0
        db  00Fh, 073h, 074h, 061h, 05Fh, 043h, 06Fh, 06Dh, 070h, 044h, 069h, 072h, 065h, 063h, 074h, 000h
        db  030h
loc_3_08F4:
        add    byte ptr [eax + eax], al
        or     eax, 0x5f617473
        inc    ebx
        outsd  dx, dword ptr [esi]
        insd   dword ptr es:[edi], dx
        jo     0x973
        jae    0x977
        add    byte ptr [eax + eax], dh
        add    al, 0
        db  00Dh, 073h, 074h
loc_3_090C:
        popal  
        pop    edi
        inc    ebx
        outsd  dx, dword ptr [esi]
        insd   dword ptr es:[edi], dx
        jo     0x966
        popal  
        jbe    0x97b
        add    byte ptr [eax], bh
        add    byte ptr [eax + eax], al
        db  00Fh, 073h, 074h, 061h
loc_3_091F:
        pop    edi
        arpl   word ptr [ebx + 0x65], si
        db  063h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0x1bf2  R-X
loc_4_0000:
        insd   dword ptr es:[edi], dx
        pop    edi
        jne    loc_4_0063
        popaw  
        je     loc_4_0067
        xor    dword ptr [edx], esi
        bound  ebp, qword ptr [ecx + 0x74]
        add    byte ptr [eax + eax], ah
        add    al, byte ptr [eax]
        sbb    dh, byte ptr [edx + 0x6d]
        pop    edi
        jne    loc_4_0077
        imul   si, word ptr [edx + 0x73], 0x5f74
        arpl   word ptr [ebp + esi*2 + 0x73], bp
        je     0x89
        jb     loc_4_0085
        jae    loc_4_008D
        arpl   word ptr [edi + ebp*2 + 0x72], si
        add    byte ptr [esi], cl
        add    byte ptr [eax + eax], al
        db  00Fh, 072h, 06Dh, 05Fh, 075h, 05Fh, 068h, 06Fh, 073h, 074h, 05Fh, 069h, 072h, 070h, 074h, 000h
        db  04Ch, 000h, 001h, 000h, 01Bh, 072h, 06Dh, 05Fh, 075h, 05Fh, 068h, 06Fh, 073h, 074h, 05Fh, 073h
        db  065h, 063h, 074h, 06Fh, 072h, 05Fh, 073h, 069h, 07Ah, 065h, 05Fh, 070h, 06Fh, 077h, 032h, 000h
        db  00Ah, 000h
loc_4_0063:
        add    al, 0
        db  013h, 072h
loc_4_0067:
        insd   dword ptr es:[edi], dx
        pop    edi
        jne    0xca
        push   0x5f74736f
        jae    0xe6
        jb     loc_4_00D5
        je     loc_4_00DB
        db  067h
loc_4_0077:
        jns    loc_4_0079
loc_4_0079:
        pop    eax
        add    byte ptr [eax + eax], al
        adc    dword ptr [edx + 0x6d], esi
        pop    edi
        jne    0xe2
        db  068h, 06Fh
loc_4_0085:
        jae    0xfb
        pop    edi
        jbe    0xf9
        insb   byte ptr es:[edi], dx
        pop    edi
        db  069h
loc_4_008D:
        add    byte ptr fs:[eax], bl
        add    byte ptr [edx], al
        add    byte ptr [edx], dl
        jb     0x103
        pop    edi
        jne    loc_4_00F8
        imul   ebp, dword ptr [edi + 0x5f], 0x74726170
        pop    edi
        jae    loc_4_010C
        jp     0x10a
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], dl
loc_4_00AB:
        jb     0x11a
        pop    edi
        jne    0x10f
        insb   byte ptr es:[edi], dx
        popal  
        jae    0x128
        pop    edi
        arpl   word ptr [eax + 0x61], bp
        imul   ebp, dword ptr [esi], 0x4005c
        adc    dword ptr [edx + 0x6d], esi
        pop    edi
        jne    loc_4_0124
        insb   byte ptr es:[edi], dx
        jo     loc_4_0127
        imul   esi, dword ptr fs:[ebx + 0x6b], 0x70616d5f
        add    byte ptr [eax], dl
        add    dword ptr [eax], eax
loc_4_00D5:
        adc    al, 0x72
        insd   dword ptr es:[edi], dx
        pop    edi
        jne    0x13a
loc_4_00DB:
        insd   dword ptr es:[edi], dx
        imul   esp, dword ptr fs:[ecx + 0x5f], 0x63736564
        jb     loc_4_0150
        jo     0x15d
        add    byte ptr [edi], dl
        add    dword ptr [eax], eax
        adc    eax, 0x755f6d72
        pop    edi
        jb     0x15b
        insd   dword ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
loc_4_00F8:
        jbe    0x15b
        bound  ebp, qword ptr [ebp + 0x5f]
        insd   dword ptr es:[edi], dx
        imul   esp, dword ptr fs:[ecx], 0x10053
        adc    dword ptr [edx + 0x6d], esi
        pop    edi
loc_4_010C:
        jne    0x16d
        jae    loc_4_0175
        jno    loc_4_0187
loc_4_0112:
        outsb  dx, byte ptr gs:[esi]
        arpl   word ptr [ebp + 0x5f], sp
        outsb  dx, byte ptr [esi]
        outsd  dx, dword ptr [esi]
        add    byte ptr [esi], dl
        add    dword ptr [eax], eax
        adc    byte ptr [edx + 0x6d], dh
        pop    edi
        jne    loc_4_0183
loc_4_0124:
        jae    0x192
        outsd  dx, dword ptr [esi]
loc_4_0127:
        je     loc_4_0188
        je     loc_4_018C
        imul   esp, dword ptr [ebp + 0x6e], 0
        push   es
        add    byte ptr [eax + eax], al
        pop    ss
        db  072h
loc_4_0135:
        insd   dword ptr es:[edi], dx
        pop    edi
        jne    loc_4_0198
        jae    loc_4_01AC
        jp     0x1b1
        outsd  dx, dword ptr [esi]
        jb     loc_4_01B3
        pop    edi
        jb     loc_4_01A9
        pop    edi
        insb   byte ptr es:[edi], dx
        je     loc_4_014B
loc_4_014B:
        push   ss
        add    byte ptr [edx], al
        add    byte ptr [ebx], bl
loc_4_0150:
        jb     0x1bf
        pop    edi
        jne    0x1b4
        jae    0x1c8
        jp     loc_4_01CD
        outsd  dx, dword ptr [esi]
        jb     loc_4_01BB
        arpl   word ptr [ebp + esi*2 + 0x73], bp
        je     0x1c7
        jb     loc_4_01C3
        outsd  dx, dword ptr [esi]
        jae    0x1ce
        je     loc_4_016B
loc_4_016B:
        dec    esi
        add    byte ptr [ecx], al
        add    byte ptr [edi], cl
        jb     loc_4_01DF
        pop    edi
        jne    0x1d4
loc_4_0175:
        jne    0x1e5
        imul   esi, dword ptr [edi + ebx*2 + 0x63], 0x65646f
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
loc_4_0183:
        sbb    eax, dword ptr [eax]
        add    byte ptr [eax], al
loc_4_0187:
        push   es
loc_4_0188:
        dec    ebp
        inc    esp
        inc    edx
        push   eax
loc_4_018C:
        inc    edx
        add    byte ptr [eax], al
        add    byte ptr [ebx], al
        add    byte ptr [eax], cl
        bound  ebp, qword ptr [edi + 0x6f]
        je     loc_4_0202
loc_4_0198:
        insd   dword ptr es:[edi], dx
        jo     loc_4_019B
loc_4_019B:
        add    eax, dword ptr [eax]
        or     byte ptr [eax + 4], al
        outsd  dx, dword ptr [esi]
        insd   dword ptr es:[edi], dx
        add    byte ptr [ebx], cl
        add    byte ptr [edx], al
        add    byte ptr [ebx], dl
loc_4_01A9:
        inc    edx
        push   eax
        inc    edx
loc_4_01AC:
        pop    edi
        inc    edx
        jns    loc_4_0224
        jae    loc_4_0203
loc_4_01B3:
        jb     0x209
        arpl   word ptr gs:[edi + ebp*2 + 0x72], si
loc_4_01BB:
        add    byte ptr [0x16000100], cl
        inc    edx
        push   eax
loc_4_01C3:
        inc    edx
        pop    edi
        push   ebx
        arpl   word ptr gs:[edi + ebp*2 + 0x72], si
        jae    0x21d
loc_4_01CD:
        jb     0x213
        insb   byte ptr es:[edi], dx
        jne    0x246
        je     loc_4_023A
        jb     loc_4_01D7
loc_4_01D7:
        push   cs
        add    byte ptr [edx], al
        add    byte ptr [edx + eax*2], dl
        push   eax
        inc    edx
loc_4_01DF:
        pop    edi
        push   edx
        jae    0x249
        jb     0x25c
        push   ebx
        arpl   word ptr gs:[edi + ebp*2 + 0x72], si
        jae    loc_4_01F0
loc_4_01F0:
        adc    byte ptr [eax], al
        add    dword ptr [eax], eax
        adc    dword ptr [edx + 0x50], eax
        inc    edx
        pop    edi
        dec    esi
        jne    loc_4_0269
        bound  esp, qword ptr [ebp + 0x72]
        dec    edi
        inc    si
loc_4_0202:
        inc    ecx
loc_4_0203:
        push   esp
        jae    loc_4_0206
loc_4_0206:
        adc    dword ptr [eax], eax
        add    al, byte ptr [eax]
        adc    byte ptr [edx + 0x50], al
loc_4_020D:
        inc    edx
        pop    edi
        push   edx
        outsd  dx, dword ptr [esi]
        outsd  dx, dword ptr [esi]
        je     loc_4_0259
        outsb  dx, byte ptr [esi]
        je     loc_4_0289
        imul   esp, dword ptr [ebp + 0x73], 0x2001300
        add    byte ptr [ecx], dl
        inc    edx
        push   eax
        inc    edx
        pop    edi
loc_4_0224:
        push   esp
        outsd  dx, dword ptr [esi]
        je     loc_4_0289
        insb   byte ptr es:[edi], dx
        push   ebx
        arpl   word ptr gs:[edi + ebp*2 + 0x72], si
        jae    loc_4_0231
loc_4_0231:
        adc    eax, 0x14000100
        inc    edx
        push   eax
        inc    edx
        pop    edi
loc_4_023A:
        dec    ebp
loc_4_023B:
        imul   esp, dword ptr fs:[ecx + 0x44], 0x72637365
        imul   esi, dword ptr [eax + 0x74], 0x1600726f
        add    byte ptr [edx], al
        add    byte ptr [edx], dl
        inc    edx
        push   eax
        inc    edx
        pop    edi
        push   ebx
        arpl   word ptr gs:[edi + ebp*2 + 0x72], si
loc_4_0259:
        jae    0x2ab
        jb     loc_4_02A4
        inc    ecx
        push   esp
        add    byte ptr [eax], bl
        add    byte ptr [edx], al
        add    byte ptr [edx + eax*2], dl
        push   eax
        inc    edx
loc_4_0269:
        pop    edi
        push   ebx
        arpl   word ptr gs:[edi + ebp*2 + 0x72], si
        jae    loc_4_02C2
        jb     0x2c9
        jb     0x2d8
        arpl   word ptr [ebx], bp
        sbb    al, byte ptr [eax]
        add    al, byte ptr [eax]
        or     al, byte ptr [edx + 0x50]
        inc    edx
        pop    edi
        dec    eax
        popal  
        jae    loc_4_0289
loc_4_0289:
        sbb    al, 0
        add    al, 0
        adc    al, byte ptr [edx + 0x50]
        inc    edx
        pop    edi
        dec    eax
        imul   esp, dword ptr [esp + 0x65], 0x6365536e
        je     0x30c
        jb     loc_4_0312
        add    byte ptr [eax], ah
        add    byte ptr [eax + eax], al
loc_4_02A4:
        adc    al, 0x42
        push   eax
        inc    edx
        pop    edi
        inc    edx
        imul   esp, dword ptr [edi + 0x54], 0x6c61746f
        push   ebx
        arpl   word ptr gs:[edi + ebp*2 + 0x72], si
        jae    loc_4_02B9
loc_4_02B9:
        das    
        add    byte ptr [edx], al
        add    byte ptr [edx], cl
        pop    edi
        popaw  
loc_4_02C2:
        je     loc_4_0323
        jo     0x327
        jb     loc_4_033C
        jae    loc_4_02CA
loc_4_02CA:
        sub    eax, 0xe000200
        imul   si, word ptr [edx + 0x73], 0x5f74
        arpl   word ptr [ebp + esi*2 + 0x73], bp
        je     0x340
        jb     loc_4_02DD
loc_4_02DD:
        sub    eax, dword ptr [eax]
        add    al, byte ptr [eax]
        or     esp, dword ptr [esi + 0x69]
        jb     0x359
        je     loc_4_0347
        popal  
        je     0x34d
        add    byte ptr [ecx], ch
        add    byte ptr [edx], al
        add    byte ptr [ebx], cl
        imul   si, word ptr [edx + 0x73], 0x5f74
        jb     0x369
        outsd  dx, dword ptr [esi]
        je     loc_4_02FD
loc_4_02FD:
        daa    
        add    byte ptr [edx], al
        add    byte ptr [0x5f676f6c], cl
        imul   ebp, dword ptr [esi + 0x74], 0x616e7265
        insb   byte ptr es:[edi], dx
        add    byte ptr [esi], bh
        add    byte ptr [edx], al
loc_4_0312:
        add    byte ptr [eax], cl
        insd   dword ptr es:[edi], dx
        popal  
        js     loc_4_0377
        arpl   word ptr [ecx + 0x70], sp
        add    byte ptr [ecx], bh
        add    byte ptr [eax + eax], al
        or     dword ptr [ebp + 0x61], ebp
loc_4_0323:
        js     loc_4_0384
        imul   bp, word ptr [ebp], 0x35
        add    al, 0
        or     dword ptr [ebp + 0x69], ebp
        outsb  dx, byte ptr [esi]
        pop    edi
        imul   bp, word ptr [ebp], 0x33
        add    al, byte ptr [eax]
loc_4_033C:
        or     dword ptr [eax + 0x61], esi
        jae    loc_4_03B4
        pop    edi
        outsd  dx, dword ptr [esi]
        add    byte ptr [edx], dh
loc_4_0347:
        add    byte ptr [ecx], al
        add    byte ptr [ebx + esi*2], dl
        arpl   word ptr gs:[edi + ebp*2 + 0x72], si
        jae    loc_4_03B2
        jo     loc_4_03B4
        jne    0x3c5
        imul   esi, dword ptr [edi + ebx*2 + 0x70], 0x32776f
        add    byte ptr es:[ecx], al
        add    byte ptr [ebx + esi*2], cl
        arpl   word ptr gs:[edi + ebp*2 + 0x72], si
        pop    edi
        jo     0x3dc
        ja     loc_4_03A1
        add    byte ptr [eax + eax], ah
        add    al, byte ptr [eax]
        adc    dword ptr [ebx + 0x71], esi
loc_4_0377:
        jp     loc_4_03ED
        outsd  dx, dword ptr [esi]
        jb     0x3db
        arpl   word ptr [ebp + esi*2 + 0x73], bp
        je     0x3e7
        jb     0x3e3
loc_4_0384:
        outsd  dx, dword ptr [esi]
        add    byte ptr [0xb000100], bh
        js     0x3f3
        popal  
        je     0x3ef
        xor    dword ptr [edx], esi
        bound  ebp, qword ptr [ecx + 0x74]
        add    byte ptr [ebx], al
        add    byte ptr [eax], al
        add    byte ptr [edx], dl
        add    byte ptr [eax], al
        add    byte ptr [esp + eax*2], al
        push   eax
loc_4_03A1:
        inc    edx
        add    byte ptr [eax + eax], al
        add    dword ptr [eax], eax
        adc    dword ptr [eax + esi*2 + 0x62], esp
        pop    edi
        arpl   word ptr [ebp + esi*2 + 0x73], bp
        je     0x417
loc_4_03B2:
        jb     loc_4_0413
loc_4_03B4:
        insd   dword ptr es:[edi], dx
        popal  
        jae    0x423
        add    byte ptr [0x12000100], al
        jo     0x423
        pop    edi
        arpl   word ptr [ebp + esi*2 + 0x73], bp
        je     0x42d
        jb     0x429
        jae    0x434
        imul   esp, dword ptr [esi + 0x74], 0x2001100
        add    byte ptr [edi], cl
        jo     0x43a
        pop    edi
        imul   esi, dword ptr fs:[edx + 0x5f], 0x74636573
        outsd  dx, dword ptr [esi]
        jb     loc_4_03E4
loc_4_03E4:
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        or     ah, byte ptr [eax + esi*2 + 0x62]
        pop    edi
loc_4_03ED:
        jb     0x459
        jbe    loc_4_0457
        add    byte ptr [ebx], dl
        add    byte ptr [eax + eax], al
        adc    byte ptr [eax + esi*2 + 0x62], ah
        pop    edi
        jb     loc_4_0468
        jbe    loc_4_0466
        jb     0x462
        popal  
        jb     loc_4_0408
loc_4_0408:
        or     byte ptr [eax], al
        add    dword ptr [eax], eax
        push   cs
        jo     0x472
        pop    edi
        popaw  
loc_4_0413:
        je     0x474
        arpl   word ptr [edi + 0x75], bp
        outsb  dx, byte ptr [esi]
        je     loc_4_041B
loc_4_041B:
        sldt   word ptr [edx]
        add    byte ptr [0x5f627064], cl
        popaw  
        je     0x487
        jae    0x493
        jp     loc_4_0491
        add    byte ptr [eax], bl
        add    byte ptr [ecx], al
        add    byte ptr [edx], cl
        jo     0x497
        pop    edi
        insb   byte ptr es:[edi], dx
        popal  
        jae    loc_4_043C
loc_4_043C:
        push   es
        add    byte ptr [edx], al
        add    byte ptr [esi], cl
        jo     0x4a6
        pop    edi
        imul   si, word ptr [edx + 0x73], 0x5f74
        popaw  
        je     loc_4_044F
loc_4_044F:
        or     eax, dword ptr [eax]
        add    al, byte ptr [eax]
        adc    dword ptr [eax + esi*2 + 0x62], esp
loc_4_0457:
        pop    edi
        imul   si, word ptr [edx + 0x73], 0x5f74
        jae    loc_4_04C5
        arpl   word ptr [edi + ebp*2 + 0x72], si
        add    byte ptr [edi], bl
loc_4_0466:
        add    byte ptr [edx], al
loc_4_0468:
        add    byte ptr [0x5f627064], cl
        jb     0x4d6
        pop    edi
        arpl   word ptr [esi + 0x74], bp
        add    byte ptr [0x10000200], cl
        jo     loc_4_04E1
        pop    edi
        insd   dword ptr es:[edi], dx
        popal  
        js     0x4e3
        arpl   word ptr [ebp + esi*2 + 0x73], bp
        je     0x4ef
        jb     loc_4_048C
loc_4_048C:
        pop    ss
        add    byte ptr [ecx], al
        add    byte ptr [edx], cl
loc_4_0491:
        jo     loc_4_04F6
        pop    edi
        insd   dword ptr es:[edi], dx
        imul   esp, dword ptr fs:[ecx], 0x40019
        or     eax, 0x5f627064
        outsb  dx, byte ptr [esi]
        js     loc_4_051C
        pop    edi
        jo     loc_4_050E
        add    byte ptr [0xe000200], bl
        jo     loc_4_0517
        pop    edi
        outsb  dx, byte ptr [esi]
        js     0x52e
        pop    edi
        jb     loc_4_0523
        add    byte ptr gs:[ecx], cl
        add    byte ptr [edx], al
        add    byte ptr [ecx], dl
loc_4_04C5:
        jo     0x52a
        pop    edi
        jb     loc_4_053A
        outsd  dx, dword ptr [esi]
        je     loc_4_052D
        outsb  dx, byte ptr gs:[esi]
        je     0x544
        imul   esp, dword ptr [ebp + 0x73], 0x2000200
        add    byte ptr [eax], dl
        jo     loc_4_0540
        pop    edi
        jae    loc_4_0546
loc_4_04E1:
        arpl   word ptr [edi + ebp*2 + 0x72], si
        pop    edi
        jae    loc_4_0551
        jp     loc_4_054F
        add    byte ptr [ecx], al
        add    byte ptr [ecx], al
        add    byte ptr [ecx], cl
        jo     loc_4_0555
        pop    edi
        jne    loc_4_0564
loc_4_04F6:
        imul   esi, dword ptr [eax + eax - 0x48], 0x78
        int    0x41
        mov    esi, 0x117f                   ; -> obj6:0x117F
        call   0xfffff038
        ret    
        db  0B8h, 077h, 000h
loc_4_050E:
        add    byte ptr [eax], al
        mov    bl, 2
        int    0x41
        or     ah, ah
        db  075h
loc_4_0517:
        sub    eax, 0x80ec81
loc_4_051C:
        add    byte ptr [eax], al
        mov    esi, esp
        db  0B8h, 075h, 000h
loc_4_0523:
        add    byte ptr [eax], al
        int    0x41
        mov    esi, dword ptr [esi + 0x14]
        db  081h, 0C4h, 080h
loc_4_052D:
        add    byte ptr [eax], al
        add    byte ptr [ecx + 0x34c], bh
        mov    eax, 0x79
loc_4_053A:
        int    0x41
        or     ax, ax
        db  075h
loc_4_0540:
        sub    eax, 0x1febde8b
        db  0B8h
loc_4_0546:
        js     loc_4_0548
loc_4_0548:
        add    byte ptr [eax], al
        int    0x41
        cmp    ebx, 0x1a
loc_4_054F:
        jb     loc_4_056F
loc_4_0551:
        mov    esi, ebx
        db  0B9h, 04Ch
loc_4_0555:
        add    eax, dword ptr [eax]
        add    byte ptr [eax + 0x79], bh
        int    0x41
        or     ax, ax
        jne    loc_4_056E
loc_4_0564:
        mov    esi, 0x23a                    ; -> loc_6_023A
        call   0xfffff038
loc_4_056E:
        ret    
loc_4_056F:
        mov    eax, 0xe44                    ; -> obj1:0x0E44
loc_4_0574:
        mov    eax, dword ptr [eax]
        or     eax, eax
        je     loc_4_056E
        cmp    byte ptr [eax + 0x88], bl
        jne    loc_4_0574
        push   eax
        push   ebx
        mov    ebx, eax
        mov    esi, 0x23a                    ; -> loc_6_023A
        call   0xfffff038
        pop    ebx
        pop    eax
        jmp    loc_4_0574
        db  0B8h, 077h, 000h, 000h, 000h, 0B3h, 002h, 0CDh, 041h, 00Ah, 0E4h, 075h, 03Ch, 08Ah, 025h
        dd  loc_1_0E67               ; fixup
        db  0BAh
        dd  obj6:0x15B4              ; fixup
        db  0CDh, 020h, 009h, 000h, 08Bh, 004h, 0EBh, 054h, 09Ch, 0F7h, 046h, 00Ch, 000h, 000h, 000h, 0F0h
        db  075h, 049h, 08Bh, 0DEh, 0BEh
        dd  obj6:0x0CB1              ; fixup
        db  0E8h, 06Eh, 0EAh, 0FFh, 0FFh, 09Dh, 09Ch, 074h, 039h, 068h
        dd  loc_1_0BF0               ; fixup
        db  0E8h
        dd  sub_1_0214               ; fixup
        db  083h, 0C4h, 004h, 0EBh, 02Ah, 0B8h, 078h, 000h, 000h, 000h, 0CDh, 041h, 09Ch, 0BEh
        dd  obj6:0x0CB1              ; fixup
        db  0E8h, 049h, 0EAh, 0FFh, 0FFh, 08Bh, 0F3h, 0CDh, 020h, 00Dh, 000h, 08Bh, 004h, 074h, 00Eh, 050h
        db  068h
        dd  loc_1_0BFC               ; fixup
        db  0E8h
        dd  sub_1_0214               ; fixup
        db  083h, 0C4h, 008h, 09Dh, 0C3h, 0B8h, 077h, 000h, 000h, 000h, 0B3h, 002h, 0CDh, 041h, 00Ah, 0E4h
        db  075h, 04Fh, 08Ah, 025h
        dd  obj1:0x0E68              ; fixup
        db  0BAh
        dd  obj6:0x1629              ; fixup
        db  0CDh, 020h, 009h, 000h, 08Bh, 004h, 0EBh, 067h, 09Ch, 08Bh, 05Eh, 00Ch, 081h, 0E3h, 000h, 000h
        db  000h, 0F0h, 081h, 0FBh, 000h, 000h, 000h, 010h, 075h, 054h, 08Bh, 0DEh, 0BEh
        dd  obj6:0x0D4E              ; fixup
        db  0F6h, 043h, 00Ch, 0FFh, 074h, 005h, 0BEh
        dd  obj6:0x0E05              ; fixup
        db  0E8h, 0E6h, 0E9h, 0FFh, 0FFh, 09Dh, 09Ch, 074h, 039h, 068h
        dd  obj1:0x0C10              ; fixup
        db  0E8h
        dd  sub_1_0214               ; fixup
        db  083h, 0C4h, 004h, 0EBh, 02Ah, 0B8h, 078h, 000h, 000h, 000h, 0CDh, 041h, 09Ch, 0BEh
        dd  obj6:0x0D4E              ; fixup
        db  0E8h, 0C1h, 0E9h, 0FFh, 0FFh, 08Bh, 0F3h, 0CDh, 020h, 00Dh, 000h, 08Bh, 004h, 074h, 00Eh, 050h
        db  068h
        dd  obj1:0x0C1C              ; fixup
        db  0E8h
        dd  sub_1_0214               ; fixup
        db  083h, 0C4h, 008h, 09Dh, 0C3h, 068h
        dd  obj1:0x0C30              ; fixup
        db  0E8h
        dd  sub_1_0214               ; fixup
        db  083h, 0C4h, 004h, 0C3h, 068h
        dd  loc_1_0C60               ; fixup
        db  0E8h
        dd  sub_1_0214               ; fixup
        db  083h, 0C4h, 004h, 0C3h, 0C3h, 0B8h, 078h, 000h, 000h, 000h, 0CDh, 041h, 083h, 0FBh, 01Ah, 072h
        db  00Bh, 0BEh
        dd  obj6:0x0E4B              ; fixup
        db  0E8h, 074h, 0E9h, 0FFh, 0FFh, 0C3h, 0B8h
        dd  obj1:0x0E44              ; fixup
        db  08Bh, 000h, 00Bh, 0C0h, 074h, 0F4h, 038h, 098h, 088h, 000h, 000h, 000h, 075h, 0F2h, 053h, 08Bh
        db  098h, 0B8h, 000h, 000h, 000h, 00Bh, 0DBh, 074h, 00Ch, 0BEh
        dd  obj6:0x0E4B              ; fixup
        db  050h, 0E8h, 04Ah, 0E9h, 0FFh, 0FFh, 058h, 05Bh, 0EBh, 0D8h, 068h
        dd  loc_1_0C90               ; fixup
        db  0E8h
        dd  sub_1_0214               ; fixup
        db  083h, 0C4h, 004h, 0C3h, 0B8h, 077h, 000h, 000h, 000h, 0B3h, 002h, 0CDh, 041h, 00Ah, 0E4h, 075h
        db  02Dh, 08Bh, 01Dh
        dd  obj1:0x0E40              ; fixup
        db  066h, 083h, 0FBh, 0FFh, 074h, 020h, 0BEh
        dd  obj6:0x1396              ; fixup
        db  0E8h, 015h, 0E9h, 0FFh, 0FFh, 08Bh, 05Bh, 019h, 066h, 083h, 0FBh, 0FFh, 074h, 00Dh, 00Fh, 0B7h
        db  0C3h, 0C1h, 0EBh, 010h, 0C1h, 0E3h, 004h, 003h, 0D8h, 0EBh, 0E0h, 0C3h, 0B8h, 078h, 000h, 000h
        db  000h, 0CDh, 041h, 083h, 0FBh, 019h, 076h, 00Bh, 0BEh
        dd  obj6:0x1396              ; fixup
        db  0E8h, 0E8h, 0E8h, 0FFh, 0FFh, 0C3h, 00Fh, 0B6h, 0C3h, 0E8h
        dd  loc_1_015B               ; fixup
        db  072h, 00Ch, 0BEh
        dd  obj6:0x1396              ; fixup
        db  08Bh, 0DFh, 0E8h, 0D1h, 0E8h, 0FFh, 0FFh, 0C3h, 068h
        dd  loc_1_0CC0               ; fixup
        db  0E8h
        dd  sub_1_0214               ; fixup
        db  083h, 0C4h, 004h, 0C3h, 000h, 000h, 000h, 000h, 04Eh, 04Fh, 054h, 020h, 000h, 044h, 03Fh, 03Fh
        db  053h, 050h, 041h, 043h, 03Fh, 000h
        dd  loc_6_17F0               ; fixup
        dd  obj6:0x000A              ; fixup
        dd  loc_6_1829               ; fixup
        dd  obj6:0x1691              ; fixup
        dd  loc_6_1833               ; fixup
        dd  obj6:0x169F              ; fixup
        dd  obj6:0x1844              ; fixup
        dd  obj6:0x16AD              ; fixup
        dd  obj6:0x180D              ; fixup
        dd  obj6:0x14F9              ; fixup
        dd  loc_6_1819               ; fixup
        dd  obj6:0x150B              ; fixup
        dd  obj6:0x1856              ; fixup
        dd  obj6:0x16AE              ; fixup
        dd  obj6:0x1870              ; fixup
        dd  obj6:0x1594              ; fixup
        dd  loc_6_187F               ; fixup
        dd  obj6:0x1609              ; fixup
        dd  loc_6_188F               ; fixup
        dd  obj6:0x002A              ; fixup
        dd  obj6:0x18AC              ; fixup
        dd  obj6:0x16F2              ; fixup
        dd  loc_6_18C5               ; fixup
        dd  obj6:0x1768              ; fixup
        dd  obj6:0x18E0              ; fixup
        dd  obj6:0x1700              ; fixup
        db  054h, 06Fh, 067h, 067h, 06Ch, 065h, 020h, 065h, 06Dh, 070h, 074h, 079h, 020h, 066h, 069h, 065h
        db  06Ch, 064h, 020h, 070h, 072h, 069h, 06Eh, 074h, 069h, 06Eh, 067h, 02Eh, 000h, 044h, 075h, 06Dh
        db  070h, 020h, 04Dh, 044h, 042h, 050h, 042h, 02Eh, 000h, 044h, 075h, 06Dh, 070h, 020h, 044h, 049h
        db  053h, 04Bh, 05Fh, 055h, 04Eh, 049h, 054h, 02Eh, 000h, 044h, 075h, 06Dh, 070h, 020h, 06Ch, 06Fh
        db  067h, 02Eh, 000h, 044h, 075h, 06Dh, 070h, 020h, 073h, 074h, 061h, 074h, 069h, 073h, 074h, 069h
        db  063h, 073h, 02Eh, 000h, 043h, 06Ch, 065h, 061h, 072h, 020h, 073h, 074h, 061h, 074h, 069h, 073h
        db  074h, 069h, 063h, 073h, 02Eh, 000h, 044h, 075h, 06Dh, 070h, 020h, 072h, 065h, 061h, 06Ch, 02Dh
        db  06Dh, 06Fh, 064h, 065h, 020h, 044h, 049h, 053h, 04Bh, 05Fh, 055h, 04Eh, 049h, 054h, 02Eh, 000h
        db  044h, 075h, 06Dh, 070h, 020h, 043h, 041h, 043h, 048h, 045h, 048h, 044h, 052h, 02Eh, 000h, 044h
        db  075h, 06Dh, 070h, 020h, 043h, 041h, 043h, 048h, 045h, 043h, 04Ch, 055h, 053h, 02Eh, 000h, 054h
        db  06Fh, 067h, 067h, 06Ch, 065h, 020h, 063h, 06Ch, 075h, 073h, 074h, 065h, 072h, 020h, 063h, 068h
        db  065h, 063h, 06Bh, 073h, 075h, 06Dh, 06Dh, 069h, 06Eh, 067h, 02Eh, 000h, 044h, 075h, 06Dh, 070h
        db  020h, 070h, 061h, 074h, 068h, 020h, 063h, 06Fh, 076h, 065h, 072h, 061h, 067h, 065h, 020h, 064h
        db  061h, 074h, 061h, 02Eh, 000h, 050h, 075h, 074h, 020h, 072h, 065h, 066h, 065h, 072h, 065h, 06Eh
        db  063h, 065h, 020h, 06Dh, 061h, 072h, 06Bh, 020h, 069h, 06Eh, 020h, 06Ch, 06Fh, 067h, 02Eh, 000h
        db  044h, 075h, 06Dh, 070h, 020h, 044h, 04Fh, 053h, 020h, 044h, 050h, 042h, 02Eh, 000h, 05Bh, 058h
        db  05Dh, 020h, 02Dh, 020h, 045h, 078h, 069h, 074h, 020h, 06Dh, 065h, 06Eh, 075h, 02Eh, 000h, 000h
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
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_4_0AB7:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
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
loc_4_0BF0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
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
sub_4_0E69:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_0F85:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax - 0x76], ah
        db  0F0h, 08Ah, 0D3h, 0E8h
        dd  loc_4_1880               ; fixup
        db  00Bh, 0C0h, 00Fh, 085h, 017h, 001h, 000h, 000h, 052h, 06Ah, 000h, 068h, 000h, 002h, 000h, 000h
        db  0CDh, 020h, 0A9h, 000h, 001h, 000h, 083h, 0C4h, 008h, 05Ah, 00Bh, 0C0h, 00Fh, 084h, 0FDh, 000h
        db  000h, 000h, 08Bh, 0F8h, 08Ah, 056h, 053h, 0BEh
        dd  obj5:0x0776              ; fixup
        db  08Bh, 0DFh, 0FCh, 0ACh, 0AAh, 00Ah, 0C0h, 075h, 0FAh, 08Ah, 0C6h, 004h, 041h, 088h, 003h, 00Fh
        db  0B6h, 0C2h, 0B2h, 00Ah, 0F6h, 0F2h, 080h, 0C4h, 030h, 088h, 067h, 0FEh, 032h, 0E4h, 0F6h, 0F2h
        db  080h, 0C4h, 030h, 088h, 067h, 0FDh, 004h, 030h, 088h, 047h, 0FCh, 066h, 0C7h, 043h, 004h, 052h
        db  056h, 066h
loc_4_1068:
        mov    dword ptr [ebp + 0x1c], 0xcbc13d20
        add    al, 0x66
loc_4_1071:
        mov    dword ptr [ebp + 0x3c], ebx
        rol    ebx, 4
        mov    eax, ebx
        and    ax, 0xf
        mov    word ptr [ebp + 0x14], ax
        mov    eax, 0x21
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        test   byte ptr [ebp + 0x2c], 1
        je     loc_4_10C3
        mov    word ptr [ebx + 4], 0x4c42
        mov    word ptr [ebp + 0x1c], 0x3d20
        ror    ebx, 4
        mov    word ptr [ebp + 0x3c], bx
        rol    ebx, 4
        mov    eax, ebx
        and    ax, 0xf
        mov    word ptr [ebp + 0x14], ax
        mov    eax, 0x21
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        test   byte ptr [ebp + 0x2c], 1
        jne    loc_4_111E
loc_4_10C3:
        mov    ax, word ptr [ebp + 0x1c]
        mov    word ptr [ebp + 0x10], ax
        ror    ebx, 4
        mov    word ptr [ebp + 0x3c], bx
        rol    ebx, 4
        mov    eax, ebx
        and    ax, 0xf
        mov    word ptr [ebp + 0x14], ax
        mov    word ptr [ebp + 0x18], 0x200
        mov    byte ptr [ebp + 0x1d], 0x3f
        mov    eax, 0x21
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        test   byte ptr [ebp + 0x2c], 1
        jne    loc_4_110D
        cmp    word ptr [ebp + 0x1c], 0x200
        jne    loc_4_110D
        mov    esi, ebx
        call   sub_7_00F1
        mov    dword ptr [esp + 4], ecx
loc_4_110D:
        pushfd 
        mov    byte ptr [ebp + 0x1d], 0x3e
        mov    eax, 0x21
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        popfd  
loc_4_111E:
        clc    
        pushfd 
        VMMCall  VMM._Free_Temp_V86_Data_Area    ; cd20aa000100
        popfd  
        je     loc_4_1131
        stc    
        mov    byte ptr [0x82c], 1           ; -> obj5:0x082C
loc_4_1131:
        popal  
        ret    
        db  060h, 06Ah, 000h, 06Ah, 020h, 0CDh, 020h, 0A9h, 000h, 001h, 000h, 083h, 0C4h, 008h, 00Bh, 0C0h
        db  074h, 051h, 08Bh, 0F8h, 08Bh, 0D8h, 08Bh, 035h
        dd  obj1:0x0E48              ; fixup
        db  0FCh, 0ACh, 0AAh, 00Ah, 0C0h, 075h, 0FAh, 08Bh, 044h, 024h, 01Ch, 000h, 003h, 066h, 0C7h, 045h
        db  01Ch, 000h, 043h, 0C1h, 0CBh, 004h, 066h, 089h, 05Dh, 03Ch, 0C1h, 0C3h, 004h, 066h, 083h, 0E3h
        db  00Fh, 066h, 089h, 05Dh, 014h, 0B8h, 021h, 000h, 000h, 000h, 0CDh, 020h, 084h, 000h, 001h, 000h
        db  0F6h, 045h, 02Ch, 001h, 075h, 00Bh, 08Bh, 044h, 024h, 01Ch, 00Fh, 0ABh, 005h
        dd  obj5:0x07C0              ; fixup
        db  0CDh, 020h, 0AAh, 000h, 001h, 000h, 061h, 0C3h
sub_4_1198:
        pushal 
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    ebp, dword ptr [ebx + 8]
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        mov    byte ptr [ebp + 0x1d], 0x52
        mov    eax, 0x21
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        movzx  ebx, word ptr [ebp + 0x38]
        shl    ebx, 4
        movzx  ecx, word ptr [ebp + 0x10]
        add    ebx, ecx
        movzx  eax, word ptr [ebx]
        movzx  ebx, word ptr [ebx + 2]
        shl    ebx, 4
        add    ebx, eax
        mov    dword ptr [0xe40], ebx        ; -> obj1:0x0E40
        mov    word ptr [ebp + 0x1c], 0x4a11
        mov    word ptr [ebp + 0x10], 0
        mov    eax, 0x2f
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        cmp    word ptr [ebp + 0x1c], 0
        jne    loc_4_1358
        cmp    word ptr [ebp + 0x10], 0x444d
        jne    loc_4_1358
        mov    byte ptr [0xe66], 1           ; -> obj1:0x0E66
        movzx  ecx, word ptr [ebp + 0x14]
        mov    word ptr [0xd76], cx          ; -> loc_1_0D76
        cmp    word ptr [0xd74], cx          ; -> loc_1_0D74
        jb     loc_4_1235
        mov    word ptr [0xd74], cx          ; -> loc_1_0D74
loc_4_1235:
        mov    bl, byte ptr [ebp + 0x19]
        mov    byte ptr [0xd78], bl          ; -> loc_1_0D78
        movzx  ecx, byte ptr [ebp + 0x18]
        sub    cl, 0x41
        mov    byte ptr [0xd7a], cl          ; -> loc_1_0D7A
        cmp    word ptr [0xd74], 0xa         ; -> loc_1_0D74
        jb     loc_4_1283
        mov    word ptr [ebp + 0x1c], 0x4a11
        mov    word ptr [ebp + 0x10], 0xb
        mov    eax, 0x2f
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        cmp    word ptr [ebp + 0x1c], 0
        jne    loc_4_1283
        movzx  eax, word ptr [ebp + 0x18]
        shl    eax, 0x10
        mov    ax, word ptr [ebp + 0x14]
        mov    dword ptr [0xd64], eax        ; -> loc_1_0D64
loc_4_1283:
        mov    word ptr [ebp + 0x1c], 0x4a11
        mov    word ptr [ebp + 0x10], 8
        mov    eax, 0x2f
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        cmp    word ptr [ebp + 0x1c], 0
        jne    loc_4_12B5
        mov    ax, word ptr [ebp + 0x10]
        mov    word ptr [0xd72], ax          ; -> loc_1_0D72
        mov    ax, word ptr [ebp + 0x18]
        mov    word ptr [0xd70], ax          ; -> loc_1_0D70
loc_4_12B5:
        mov    word ptr [ebp + 0x1c], 0x4a11
        mov    word ptr [ebp + 0x10], 9
        mov    eax, 0x2f
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        cmp    word ptr [ebp + 0x1c], 0
        jne    loc_4_1358
        mov    al, byte ptr [ebp + 0x18]
        mov    byte ptr [0xd79], al          ; -> obj1:0x0D79
        mov    ax, word ptr [ebp + 0x14]
        shl    eax, 0x10
        mov    ax, word ptr [ebp]
        mov    dword ptr [0xd6c], eax        ; -> loc_1_0D6C
loc_4_12EF:
        mov    byte ptr [0x82c], 1           ; -> obj5:0x082C
        VMMCall  VMM.VMM_GetSystemInitState      ; cd2011010100
        cmp    eax, 0x20000000
        ja     loc_4_1358
        mov    byte ptr [0x82c], 0           ; -> obj5:0x082C
        call   0xfffd26cd                    ; -> obj4:0x16CD
        cmp    word ptr [0xd76], 0x101       ; -> loc_1_0D76
        je     loc_4_1358
        cmp    word ptr [0xd76], 0xd         ; -> loc_1_0D76
        je     loc_4_1358
        cmp    byte ptr [0xd7b], 0           ; -> obj1:0x0D7B
        jne    loc_4_1358
        movzx  edi, word ptr [0x1a]          ; -> obj2:0x001A
        add    edi, dword ptr [4]            ; -> loc_2_0004
        movzx  ecx, word ptr [0x1c]          ; -> obj2:0x001C
loc_4_1341:
        add    ecx, dword ptr [4]            ; -> loc_2_0004
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    eax, 0x80001000
        VxDCall  SHELL.SHELL_SYSMODAL_Message    ; cd2003001700
loc_4_1358:
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        popal  
        ret    
        db  000h, 033h, 0C0h, 0BFh
        dd  obj3:0x0826              ; fixup
loc_4_1377:
        mov    byte ptr [edi + 7], 0x33
        VMMCall  VMM.Get_DDB                     ; cd2046010100
        or     ecx, ecx
        jne    loc_4_1531
        movzx  eax, word ptr [0x1e]          ; -> loc_2_001E
        add    eax, dword ptr [4]            ; -> loc_2_0004
        mov    dword ptr [0xe48], eax        ; -> obj1:0x0E48
        movzx  esi, word ptr [0x20]          ; -> obj2:0x0020
        add    esi, dword ptr [4]            ; -> loc_2_0004
        mov    edi, 0x316a                   ; -> loc_7_316A
        mov    ecx, 0xb
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    eax, 0                        ; -> loc_7_0000
        and    eax, 0xfffff000
        mov    edx, 0x4c4c                   ; -> obj7:0x4C4C
        add    edx, 0xfff
        and    edx, 0xfffff000
        sub    edx, eax
        shr    eax, 0xc
        shr    edx, 0xc
        mov    dword ptr [0xe24], eax        ; -> loc_1_0E24
        mov    dword ptr [0xe18], edx        ; -> loc_1_0E18
        VMMCall  VMM._GetNulPageHandle           ; cd206a000100
        mov    dword ptr [0xe38], eax        ; -> obj1:0x0E38
        call   sub_4_16CF
        mov    edx, dword ptr [0xe34]        ; -> loc_1_0E34
        sub    edx, 0x64
        sbb    eax, eax
        and    edx, eax
        add    edx, 0x64
        mov    dword ptr [0xe34], edx        ; -> loc_1_0E34
        call   sub_4_1632
        mov    edx, dword ptr [0xe30]        ; -> loc_1_0E30
        sub    edx, 4
        sbb    eax, eax
        and    edx, eax
        add    edx, 4
        xor    eax, eax
        mov    edi, 0x812                    ; -> obj3:0x0812
        call   sub_4_15D0
        cmp    eax, 1
        sbb    eax, eax
        and    edx, eax
        mov    dword ptr [0xe2c], edx        ; -> loc_1_0E2C
        movzx  eax, byte ptr [0xe65]         ; -> loc_1_0E65
        mov    edi, 0x7e8                    ; -> obj3:0x07E8
        call   sub_4_15D0
        mov    byte ptr [0xe65], al          ; -> loc_1_0E65
        movzx  eax, byte ptr [0xe64]         ; -> obj1:0x0E64
        mov    edi, 0x7de                    ; -> loc_3_07DE
        call   sub_4_15D0
        mov    byte ptr [0xe64], al          ; -> obj1:0x0E64
        call   sub_4_1198
        call   sub_4_1541
        movzx  edx, byte ptr [0xe66]         ; -> obj1:0x0E66
        or     edx, dword ptr [0xd64]        ; -> loc_1_0D64
        je     loc_4_1531
        VxDCall  Dev_048B.Service_0000h          ; cd2000008b04
        jb     loc_4_1531
        cmp    eax, 0x100
        jne    loc_4_1531
        mov    ecx, 0x10
        mov    esi, 0x1c09                   ; -> sub_7_1C09
        VxDCall  Dev_048B.Service_0001h          ; cd2001008b04
        jb     loc_4_1531
        mov    byte ptr [0xe68], al          ; -> obj1:0x0E68
        mov    ecx, 2
        mov    esi, 0x2e3c                   ; -> sub_7_2E3C
        VxDCall  Dev_048B.Service_0001h          ; cd2001008b04
        jb     loc_4_1533
        mov    byte ptr [0xe67], al          ; -> loc_1_0E67
        call   sub_4_175C
        jb     loc_4_1531
        cmp    dword ptr [0xe3c], 0          ; -> obj1:0x0E3C
        jne    loc_4_14F8
        call   sub_4_1685
        jb     loc_4_14E7
        call   sub_4_1628
        jae    loc_4_14F8
loc_4_14E7:
        and    dword ptr [0xe2c], 0          ; -> loc_1_0E2C
        mov    dword ptr [0xe3c], 0xe4c      ; -> obj1:0x0E3C
loc_4_14F8:
        mov    dword ptr [0x7a8], 0x30       ; -> obj3:0x07A8
        mov    dword ptr [0x7ac], 0x958      ; -> loc_3_07AC
        push   0x79c                         ; -> obj3:0x079C
        VxDCall  BLOCKDEV.Service_0007h          ; cd2007001000
        add    esp, 4
        cmp    word ptr [0x7c8], 1           ; -> obj3:0x07C8
        je     loc_4_152E
        cmp    word ptr [0x7c8], 2           ; -> obj3:0x07C8
        jne    loc_4_1531
loc_4_152E:
        xor    eax, eax
        ret    
loc_4_1531:
        stc    
        ret    
loc_4_1533:
        mov    ah, byte ptr [0xe68]          ; -> obj1:0x0E68
        VxDCall  Dev_048B.Service_000Fh          ; cd200f008b04
        jmp    loc_4_1531
sub_4_1541:
        mov    edi, 0x7d4                    ; -> loc_3_07D4
        xor    edx, edx
        push   edx
        call   sub_4_160A
        mov    esi, edx
        pop    edx
        jb     loc_4_1558
        call   sub_4_156F
loc_4_1558:
        mov    esi, 0x894                    ; -> obj3:0x0894
        call   sub_4_156F
        or     edx, dword ptr [0xd64]        ; -> loc_1_0D64
        mov    dword ptr [0xd64], edx        ; -> loc_1_0D64
        ret    
sub_4_156F:
        xor    eax, eax
        cld    
loc_4_1572:
        lodsb  al, byte ptr [esi]
        or     al, al
        je     loc_4_15CF
        cmp    al, 0x20
        je     loc_4_1572
        cmp    al, 9
        je     loc_4_1572
        dec    esi
        cmp    word ptr [esi], 0x30
        je     loc_4_15CF
        cmp    word ptr [esi], 0x31
        jne    loc_4_1591
        or     edx, 0xffffffff
        jmp    loc_4_15CF
loc_4_1591:
        lodsb  al, byte ptr [esi]
        or     al, al
        je     loc_4_15CF
        sub    al, 0x61
        jae    loc_4_159E
        add    al, 0x20
        jae    loc_4_1591
loc_4_159E:
        cmp    al, 0x19
        ja     loc_4_1591
        cmp    byte ptr [esi], 0x2d
        jne    loc_4_15CA
        mov    cl, byte ptr [esi + 1]
        sub    cl, 0x61
        jae    loc_4_15B4
        add    cl, 0x20
        jae    loc_4_15CA
loc_4_15B4:
        cmp    cl, 0x19
        ja     loc_4_15CA
        cmp    al, cl
        ja     loc_4_1591
loc_4_15BD:
        bts    edx, eax
        inc    eax
        cmp    al, cl
        jbe    loc_4_15BD
        add    esi, 2
        jmp    loc_4_1591
loc_4_15CA:
        bts    edx, eax
        jmp    loc_4_1591
loc_4_15CF:
        ret    
sub_4_15D0:
        push   eax
        push   ecx
        VMMCall  VMM.VMM_GetSystemInitState      ; cd2011010100
        cmp    eax, 0x20000000
        pop    ecx
        pop    eax
        ja     loc_4_15EC
        mov    esi, 0x81d                    ; -> obj3:0x081D
        VMMCall  VMM.Get_Profile_Boolean         ; cd20b1000100
loc_4_15EC:
        ret    
        db  050h, 051h, 0CDh, 020h, 011h, 001h, 001h, 000h, 03Dh, 000h, 000h, 000h, 020h, 059h, 058h, 077h
        db  00Bh, 0BEh
        dd  obj3:0x081D              ; fixup
        db  0CDh, 020h, 0ABh, 000h, 001h, 000h, 0C3h
sub_4_160A:
        push   eax
        push   ecx
        VMMCall  VMM.VMM_GetSystemInitState      ; cd2011010100
        cmp    eax, 0x20000001
        pop    ecx
        pop    eax
        cmc    
        jb     loc_4_1627
        mov    esi, 0x81d                    ; -> obj3:0x081D
        VMMCall  VMM.Get_Profile_String          ; cd20b3000100
loc_4_1627:
        ret    
sub_4_1628:
        VxDCall  Dev_0042.Service_0000h          ; cd2000004200
        jae    loc_4_163A
        jmp    loc_4_164F
sub_4_1632:
        VxDCall  Dev_0042.Service_0000h          ; cd2000004200
        jb     loc_4_164F
loc_4_163A:
        cmp    eax, 0x40a
        jb     loc_4_164F
        sub    eax, eax
        VxDCall  Dev_0042.Service_0002h          ; cd2002004200
        inc    byte ptr [0xe5e]              ; -> obj1:0x0E5E
loc_4_164F:
        mov    eax, 0x4a12
        mov    edx, 0x4d524349
        push   0x2f
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        cmp    edx, 0x4943524d
        jne    loc_4_167C
        movzx  eax, word ptr [edi + 0xc]
        test   eax, 0x100
        je     loc_4_167C
        mov    dword ptr [0xe3c], edi        ; -> obj1:0x0E3C
        jmp    loc_4_1684
loc_4_167C:
        mov    byte ptr [0xe5e], 0           ; -> obj1:0x0E5E
        stc    
loc_4_1684:
        ret    
sub_4_1685:
        pushal 
        mov    edx, 0x7f9                    ; -> loc_3_07F9
        mov    eax, 1
        call   sub_4_16A8
        jae    loc_4_16A6
        mov    edx, 0x802                    ; -> obj3:0x0802
        mov    eax, 1
        call   sub_4_16A8
loc_4_16A6:
        popal  
        ret    
sub_4_16A8:
        sub    esp, 0x50
        mov    edi, esp
        push   eax
        push   edx
        VMMCall  VMM.Get_Exec_Path               ; cd20b6000100
        mov    esi, edx
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    esi
loc_4_16BB:
        lodsb  al, byte ptr [esi]
        stosb  byte ptr es:[edi], al
        test   al, al
        jne    loc_4_16BB
        pop    eax
        mov    edx, esp
        VxDCall  APM.Service_0001h               ; cd2001002700
        lea    esp, [esp + 0x50]
        ret    
sub_4_16CF:
        sub    esp, 0xc
        lea    esi, [0x82e]                  ; -> obj3:0x082E
        lea    edi, [esp + 8]
        push   edi
        push   esi
        push   0x80000002
        VMMCall  VMM._RegOpenKey                 ; cd2048010100
        add    esp, 0xc
        cmp    eax, 1
        cmc    
        jb     loc_4_1754
        mov    ebx, 0x8d0                    ; -> loc_3_08D0
loc_4_16F7:
        add    ebx, 0x14
        mov    ecx, dword ptr [ebx]
        or     ecx, ecx
        je     loc_4_1745
        mov    eax, dword ptr [ebx + 8]
        lea    edx, [esp + 4]
        mov    esi, dword ptr [ebx + 0xc]
        lea    edi, [esp]
        mov    dword ptr [edi], ecx
        mov    ecx, dword ptr [esp + 8]
        push   edi
        push   esi
        push   edx
        push   0
        push   eax
        push   ecx
        VMMCall  VMM._RegQueryValueEx            ; cd2051010100
        add    esp, 0x18
        or     eax, eax
        jne    loc_4_173B
        mov    eax, dword ptr [esp + 4]
        cmp    dword ptr [ebx + 4], eax
        je     loc_4_16F7
        cmp    eax, 4
        jne    loc_4_173B
        cmp    dword ptr [ebx + 4], 3
        je     loc_4_16F7
loc_4_173B:
        mov    eax, dword ptr [ebx + 0x10]
        mov    esi, dword ptr [ebx + 0xc]
        mov    dword ptr [esi], eax
        jmp    loc_4_16F7
loc_4_1745:
        mov    eax, dword ptr [esp + 8]
        push   eax
        VMMCall  VMM._RegCloseKey                ; cd2049010100
        add    esp, 4
        clc    
loc_4_1754:
        lea    esp, [esp + 0xc]
        ret    
        db  000h, 000h, 000h
sub_4_175C:
        call   sub_4_0E69
        jb     loc_4_1798
        mov    dword ptr [0x4828], eax       ; -> loc_7_4828
        mov    ecx, 8
        mov    ebx, 0x4828                   ; -> loc_7_4828
        mov    edi, 0x20000008
loc_4_1777:
        dec    edi
        mov    ah, byte ptr [0xe68]          ; -> obj1:0x0E68
        mov    al, 0x33
        push   ecx
        VxDCall  Dev_048B.Service_0004h          ; cd2004008b04
        pop    ecx
        jae    loc_4_1798
        or     esi, esi
        je     loc_4_1798
        mov    dword ptr [ecx*4 + 0x4804], esi ; -> loc_7_4804
        loop   loc_4_1777
        clc    
loc_4_1798:
        ret    
        db  000h, 000h, 000h, 058h, 058h, 058h, 058h, 058h, 058h, 058h, 058h, 040h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 044h, 052h, 056h, 053h, 050h, 041h, 043h, 045h, 020h
        db  020h, 020h, 020h, 020h, 020h, 020h, 020h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 041h, 075h, 074h, 06Fh, 04Dh
        db  06Fh, 075h, 06Eh, 074h, 000h, 046h, 041h, 054h, 033h, 032h, 048h, 06Fh, 073h, 074h, 000h, 049h
        db  06Eh, 076h, 061h, 06Ch, 069h, 064h, 061h, 074h, 065h, 042h, 069h, 074h, 046h, 041h, 054h, 000h
        db  04Dh, 052h, 043h, 049h, 02Eh, 056h, 058h, 044h, 000h, 056h, 04Dh, 04Dh, 033h, 032h, 05Ch, 04Dh
        db  052h, 043h, 049h, 032h, 02Eh, 056h, 058h, 044h, 000h, 04Eh, 06Fh, 043h, 06Fh, 06Dh, 070h, 072h
        db  065h, 073h, 073h, 000h, 044h, 052h, 056h, 053h, 050h, 041h, 043h, 045h, 000h, 044h, 052h, 056h
        db  053h, 050h, 041h, 043h, 058h, 053h, 079h, 073h, 074h, 065h, 06Dh, 05Ch, 043h, 075h, 072h, 072h
        db  065h, 06Eh, 074h, 043h, 06Fh, 06Eh, 074h, 072h, 06Fh, 06Ch, 053h, 065h, 074h, 05Ch, 043h, 06Fh
        db  06Eh, 074h, 072h, 06Fh, 06Ch, 05Ch, 046h, 069h, 06Ch, 065h, 053h, 079h, 073h, 074h, 065h, 06Dh
        db  000h, 041h, 075h, 074h, 06Fh, 04Dh, 06Fh, 075h, 06Eh, 074h, 044h, 072h, 069h, 076h, 065h, 073h
        db  000h, 043h, 06Fh, 06Dh, 070h, 072h, 065h, 073h, 073h, 069h, 06Fh, 06Eh, 041h, 06Ch, 067h, 06Fh
        db  072h, 069h, 074h, 068h, 06Dh, 000h, 043h
loc_4_1880:
        outsd  dx, dword ptr [esi]
        insd   dword ptr es:[edi], dx
        jo     loc_4_18F6
        jae    loc_4_18FA
        imul   ebp, dword ptr [edi + 0x6e], 0x65726854
        jae    loc_4_18F8
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        add    byte ptr fs:[eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], dl
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        pop    edx
        or     byte ptr [eax], al
        add    byte ptr [eax + ecx], dl      ; -> obj3:0x0894
loc_4_18F6:
        add    byte ptr [eax], al
loc_4_18F8:
        add    al, 0
loc_4_18FA:
        add    byte ptr [eax], al
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        push   8
        add    byte ptr [eax], al
        xor    byte ptr [esi], cl
        add    byte ptr [eax], al
        add    al, byte ptr [eax]
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [eax], al
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        jg     loc_4_191E
        add    byte ptr [eax], al
        xor    al, 0xe
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_191E:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_4_1B80:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 5  vsize=0x830  RW-
        db  08Bh, 07Bh, 00Ch, 0F7h, 047h, 020h, 000h, 040h, 000h, 000h, 00Fh, 084h, 080h, 000h, 000h, 000h
        db  0F6h, 047h, 022h, 040h, 074h, 00Ch, 08Bh, 035h
        dd  loc_1_0974               ; fixup
        db  0F6h, 046h, 028h, 008h, 075h, 06Eh, 00Fh, 0B6h, 057h, 014h, 0BEh
        dd  obj1:0x0E44              ; fixup
        db  08Bh, 036h, 00Bh, 0F6h, 074h, 05Fh, 038h, 096h, 08Ah, 000h, 000h, 000h, 075h, 0F2h, 052h, 056h
        db  08Bh, 0CFh, 0E8h, 085h, 00Eh, 000h, 000h, 05Eh, 05Ah, 072h, 04Bh, 089h, 07Eh, 038h, 089h, 086h
        db  040h, 002h, 000h, 000h, 052h, 08Bh, 0D7h, 0E8h, 022h, 009h, 000h, 000h, 05Ah, 00Fh, 0B6h, 08Eh
        db  088h, 000h, 000h, 000h, 03Bh, 0D1h, 075h, 006h, 08Ah, 08Eh, 089h, 000h, 000h, 000h, 08Bh, 0C7h
        db  0E8h, 040h, 00Bh, 000h, 000h, 0F6h, 047h, 020h, 004h, 074h, 0B5h, 0F6h, 046h, 004h, 004h, 075h
        db  0AFh, 080h, 04Eh, 004h, 004h, 00Fh, 0B6h, 086h, 08Ah, 000h, 000h, 000h, 00Fh, 0ABh, 005h
        dd  loc_1_0D64               ; fixup
        db  0EBh, 09Bh, 0C3h, 08Bh, 0EEh, 0E8h
        dd  loc_1_00E3               ; fixup
        db  08Bh, 075h, 000h, 0E8h, 076h, 009h, 000h, 000h, 0E8h
        dd  sub_1_0135               ; fixup
        db  00Fh, 0B6h, 057h, 014h, 0EBh, 082h, 08Bh, 043h, 00Ch, 0F6h, 040h, 021h, 040h, 074h, 05Dh, 0E8h
        db  006h, 009h, 000h, 000h, 08Bh, 0F0h, 0B8h
        dd  obj1:0x0E44              ; fixup
        db  08Bh, 000h, 00Bh, 0C0h, 074h, 04Bh, 039h, 070h, 038h, 075h, 0F5h, 050h, 00Fh, 0B6h, 090h, 088h
        db  000h, 000h, 000h, 0F6h, 040h, 004h, 001h, 074h, 007h, 00Fh, 0B6h, 090h, 089h, 000h, 000h, 000h
        db  08Bh, 0C2h, 0E8h, 0F0h, 00Ah, 000h, 000h, 058h, 050h, 00Fh, 0B6h, 098h, 088h, 000h, 000h, 000h
        db  0E8h, 04Ah, 008h, 000h, 000h, 058h, 033h, 0C9h, 056h, 089h, 048h, 03Ch, 089h, 048h, 038h, 087h
        db  088h, 040h, 002h, 000h, 000h, 087h, 0CEh, 050h, 0E8h
        dd  loc_7_4B38               ; fixup
        db  058h, 05Eh, 0EBh, 0AFh, 0C3h, 0E8h, 04Dh, 013h, 000h, 000h, 083h, 03Dh
        dd  obj5:0x013C              ; fixup
        db  000h, 075h, 015h, 053h, 057h, 08Ah, 025h
        dd  obj1:0x0E68              ; fixup
        db  0B9h, 00Ah, 000h, 000h, 000h, 0CDh, 020h, 010h, 000h, 08Bh, 004h, 05Fh, 05Bh, 08Bh, 015h
        dd  loc_1_0D64               ; fixup
        db  083h, 0FAh, 0FFh, 075h, 054h, 0B9h, 01Ah, 000h, 000h, 000h, 049h, 078h, 046h, 08Bh, 0C1h, 0E8h
        dd  sub_7_02C9               ; fixup
        db  072h, 0F4h, 0F6h, 040h, 020h, 004h, 075h, 0EEh, 052h, 08Bh, 0D1h, 053h, 051h, 0E8h, 029h, 012h
        db  000h, 000h, 08Bh, 0C3h, 059h, 05Bh, 05Ah, 072h, 0DDh, 0A8h, 080h, 074h, 00Dh, 08Bh, 004h, 08Dh
        dd  loc_1_0DB0               ; fixup
        db  08Ah, 080h, 08Ah, 000h, 000h, 000h, 051h, 08Ah, 0C8h, 0BEh, 012h, 000h, 000h, 000h, 0E8h, 0C7h
        db  001h, 000h, 000h, 072h, 003h, 00Fh, 0B3h, 0CAh, 059h, 0EBh, 0B7h, 089h, 015h
        dd  loc_1_0D64               ; fixup
        db  00Bh, 015h
        dd  obj5:0x0138              ; fixup
        db  00Bh, 015h
        dd  obj1:0x0AE0              ; fixup
        db  075h, 03Ch, 033h, 0F6h, 087h, 035h
        dd  loc_1_0E28               ; fixup
        db  0CDh, 020h, 03Eh, 000h, 001h, 000h, 053h, 057h, 0E8h, 001h, 01Ah, 000h, 000h, 032h, 0E4h, 086h
        db  025h
        dd  loc_1_0E67               ; fixup
        db  00Ah, 0E4h, 074h, 006h, 0CDh, 020h, 00Fh, 000h, 08Bh, 004h, 032h, 0E4h, 086h, 025h
        dd  obj1:0x0E68              ; fixup
        db  00Ah, 0E4h, 074h, 006h, 0CDh, 020h, 00Fh, 000h, 08Bh, 004h, 05Fh, 05Bh, 0F9h, 01Bh, 0C0h, 066h
        db  089h, 043h, 002h, 072h, 026h, 0E8h
        dd  loc_1_0180               ; fixup
        db  083h, 03Dh
        dd  loc_1_0E28               ; fixup
        db  000h, 075h, 018h, 0B8h, 0E8h, 003h, 000h, 000h, 033h, 0D2h, 0BEh
        dd  loc_1_03B6               ; fixup
        db  0CDh, 020h, 03Ch, 000h, 001h, 000h, 089h, 035h
        dd  loc_1_0E28               ; fixup
        db  0C3h, 08Ah, 053h, 014h, 0E8h, 004h, 000h, 000h, 000h, 009h, 07Bh, 010h, 0C3h, 033h, 0FFh, 0B8h
        dd  obj1:0x0E44              ; fixup
        db  08Bh, 000h, 00Bh, 0C0h, 074h, 014h, 038h, 090h, 089h, 000h, 000h, 000h, 075h, 0F2h, 00Fh, 0B6h
        db  088h, 088h, 000h, 000h, 000h, 00Fh, 0ABh, 0CFh, 0EBh, 0E6h, 0C3h, 083h, 0ECh, 004h, 083h, 07Bh
        db  010h, 000h, 00Fh, 085h, 001h, 001h, 000h, 000h, 083h, 024h, 024h, 000h, 08Bh, 073h, 00Ch, 08Bh
        db  043h, 014h, 089h, 043h, 018h, 089h, 043h, 01Ch, 08Bh, 02Ch, 085h
        dd  loc_1_0DB0               ; fixup
        db  00Bh, 0EDh, 074h, 013h, 08Ah, 085h, 089h, 000h, 000h, 000h, 0F6h, 045h, 004h, 00Ch, 075h, 007h
        db  00Fh, 0ABh, 043h, 010h, 089h, 02Ch, 024h, 0BDh
        dd  obj1:0x0E44              ; fixup
        db  08Bh, 06Dh, 000h, 00Bh, 0EDh, 00Fh, 084h, 09Ah, 000h, 000h, 000h, 038h, 085h, 089h, 000h, 000h
        db  000h, 075h, 0EDh, 0F6h, 045h, 004h, 00Ch, 075h, 009h, 083h, 0BDh, 0B8h, 000h, 000h, 000h, 000h
        db  074h, 0DEh, 083h, 07Dh, 03Ch, 000h, 075h, 0D8h, 089h, 075h, 03Ch, 0F6h, 045h, 004h, 00Ch, 074h
        db  048h, 00Fh, 0B6h, 095h, 089h, 000h, 000h, 000h, 089h, 053h, 018h, 08Ah, 095h, 08Ah, 000h, 000h
        db  000h, 089h, 053h, 01Ch, 0F6h, 045h, 004h, 004h, 074h, 02Fh, 050h, 053h, 056h, 08Bh, 043h, 014h
        db  088h, 085h, 089h, 000h, 000h, 000h, 033h, 0D2h, 087h, 014h, 085h
        dd  loc_1_0DB0               ; fixup
        db  052h, 050h, 0E8h, 065h, 009h, 000h, 000h, 058h, 072h, 020h, 08Fh, 004h, 085h
        dd  loc_1_0DB0               ; fixup
        db  05Eh, 05Bh, 058h, 088h, 085h, 089h, 000h, 000h, 000h, 00Fh, 0B6h, 095h, 088h, 000h, 000h, 000h
        db  00Fh, 0ABh, 053h, 010h, 0E9h, 077h, 0FFh, 0FFh, 0FFh, 058h, 05Eh, 05Bh, 058h, 088h, 085h, 089h
        db  000h, 000h, 000h, 053h, 056h, 0FFh, 075h, 000h, 0E8h, 0BCh, 007h, 000h, 000h, 05Dh, 05Eh, 05Bh
        db  0E9h, 05Eh, 0FFh, 0FFh, 0FFh, 083h, 03Ch, 024h, 000h, 074h, 007h, 08Bh, 043h, 014h, 00Fh, 0B3h
        db  043h, 010h, 083h, 07Bh, 010h, 000h, 075h, 016h, 08Bh, 053h, 014h, 080h, 0FAh, 01Ah, 073h, 00Eh
        db  00Fh, 0A3h, 015h
        dd  loc_1_0D64               ; fixup
        db  073h, 005h, 0E8h, 025h, 000h, 000h, 000h, 083h, 0C4h, 004h, 0C3h, 083h, 0ECh, 00Ch, 066h, 089h
        db  034h, 024h, 08Bh, 0F4h, 089h, 04Eh, 004h, 056h, 0FFh, 015h
        dd  sub_1_0958               ; fixup
        db  066h, 083h, 07Eh, 002h, 001h, 0F5h, 08Bh, 076h, 008h, 08Dh, 064h, 024h, 010h, 0C3h, 083h, 0ECh
        db  038h, 033h, 0C0h, 089h, 044h, 024h, 018h, 089h, 044h, 024h, 01Ch, 089h, 044h, 024h, 020h, 089h
        db  044h, 024h, 024h, 089h, 044h, 024h, 028h, 089h, 044h, 024h, 02Ch, 089h, 044h, 024h, 030h, 089h
        db  044h, 024h, 034h, 089h, 044h, 024h, 010h, 089h, 044h, 024h, 014h, 089h, 05Ch, 024h, 008h, 08Bh
        db  043h, 014h, 089h, 004h, 024h, 089h, 043h, 018h, 089h, 043h, 01Ch, 08Bh, 0D8h, 0E8h
        dd  obj7:0x0010              ; fixup
        db  089h, 044h, 024h, 004h, 08Dh, 044h, 024h, 018h, 089h, 044h, 024h, 00Ch, 0E8h, 05Fh, 001h, 000h
        db  000h, 00Fh, 082h, 0DDh, 000h, 000h, 000h, 08Bh, 074h, 024h, 00Ch, 00Fh, 0BCh, 00Eh, 075h, 00Ch
        db  083h, 0C6h, 004h, 080h, 044h, 024h, 010h, 020h, 075h, 0F1h, 0EBh, 079h, 00Fh, 0B3h, 00Eh, 089h
        db  074h, 024h, 00Ch, 003h, 04Ch, 024h, 010h, 053h, 051h, 0E8h, 0BAh, 000h, 000h, 000h, 059h, 05Bh
        db  072h, 0D5h, 00Fh, 0B6h, 015h
        dd  loc_1_0D7A               ; fixup
        db  0B6h, 019h, 00Fh, 0BCh, 054h, 024h, 004h, 074h, 009h, 00Fh, 0B3h, 054h, 024h, 004h, 00Fh, 0BAh
        db  0EAh, 01Fh, 053h, 051h, 08Bh, 0CAh, 0E8h, 080h, 002h, 000h, 000h, 059h, 05Bh, 072h, 029h, 088h
        db  09Dh, 08Ah, 000h, 000h, 000h, 08Bh, 074h, 024h, 008h, 00Fh, 0B6h, 095h, 088h, 000h, 000h, 000h
        db  00Fh, 0ABh, 056h, 010h, 08Bh, 056h, 00Ch, 089h, 055h, 03Ch, 088h, 08Dh, 0B2h, 000h, 000h, 000h
        db  00Ah, 0C9h, 075h, 004h, 089h, 06Ch, 024h, 014h, 053h, 08Bh, 0D8h, 0B8h, 000h, 0D7h, 000h, 000h
        db  0CDh, 020h, 032h, 000h, 040h, 000h, 05Bh, 0E9h, 072h, 0FFh, 0FFh, 0FFh, 08Bh, 06Ch, 024h, 014h
        db  00Bh, 0EDh, 074h, 022h, 0E8h, 0C0h, 001h, 000h, 000h, 08Bh, 044h, 024h, 008h, 00Fh, 0B6h, 095h
        db  089h, 000h, 000h, 000h, 089h, 050h, 018h, 00Fh, 0B3h, 050h, 010h, 00Fh, 0B6h, 095h, 088h, 000h
        db  000h, 000h, 00Fh, 0ABh, 050h, 010h, 08Bh, 01Ch, 024h, 0BDh
        dd  obj1:0x0E44              ; fixup
        db  08Bh, 06Dh, 000h, 00Bh, 0EDh, 074h, 015h, 038h, 09Dh, 08Ah, 000h, 000h, 000h, 075h, 0F1h, 083h
        db  07Dh, 03Ch, 000h, 075h, 0EBh, 0E8h, 031h, 006h, 000h, 000h, 0EBh, 0DCh, 0F8h, 083h, 0C4h, 038h
        db  0C3h, 081h, 0ECh, 010h, 000h, 000h, 000h, 08Bh, 0FCh, 0BEh
        dd  obj5:0x0776              ; fixup
        db  0FCh, 0A5h, 0A5h, 0A5h, 0A5h, 080h, 0C3h, 041h, 08Bh, 0F4h, 088h, 01Eh, 00Fh, 0B6h, 0C1h, 0B1h
        db  00Ah, 0F6h, 0F1h, 080h, 0C4h, 030h, 088h, 066h, 00Eh, 032h, 0E4h, 0F6h, 0F1h, 080h, 0C4h, 030h
        db  088h, 066h, 00Dh, 004h, 030h, 088h, 046h, 00Ch, 066h, 0C7h, 046h, 004h, 052h, 056h, 0B8h, 001h
        db  0D5h, 000h, 000h, 0BBh, 014h, 000h, 000h, 000h, 033h, 0C9h, 0BAh, 001h, 000h, 000h, 000h, 0CDh
        db  020h, 032h, 000h, 040h, 000h, 073h, 01Fh, 08Bh, 0F4h, 066h, 0C7h, 046h, 004h, 042h, 04Ch, 0B8h
        db  001h, 0D5h, 000h, 000h, 0BBh, 014h, 000h, 000h, 000h, 033h, 0C9h, 0BAh, 001h, 000h, 000h, 000h
        db  0CDh, 020h, 032h, 000h, 040h, 000h, 08Dh, 064h, 024h, 010h, 0C3h, 050h, 053h, 057h, 056h, 081h
        db  0ECh, 058h, 001h, 000h, 000h, 089h, 044h, 024h, 004h, 0C7h, 004h, 024h, 000h, 000h, 000h, 000h
        db  08Dh, 07Ch, 024h, 008h, 0BEh
        dd  obj5:0x0776              ; fixup
        db  0FCh, 0A5h, 0A5h, 0A5h, 0A5h, 080h, 0C3h, 041h, 08Dh, 074h, 024h, 008h, 088h, 01Eh, 0C7h, 046h
        db  00Ch, 03Fh, 03Fh, 03Fh, 000h, 08Dh, 054h, 024h, 018h, 0B9h, 007h, 000h, 000h, 000h, 0B8h, 000h
        db  04Eh, 000h, 000h, 0CDh, 020h, 032h, 000h, 040h, 000h, 072h, 07Bh, 08Bh, 0D8h, 0EBh, 011h, 08Dh
        db  054h, 024h, 018h, 0B8h, 000h, 04Fh, 000h, 000h, 0CDh, 020h, 032h, 000h, 040h, 000h, 072h, 057h
        db  08Dh, 0B4h, 024h, 051h, 001h, 000h, 000h, 080h, 0BCh, 024h, 048h, 001h, 000h, 000h, 000h, 075h
        db  004h, 08Dh, 074h, 024h, 04Dh, 033h, 0D2h, 08Bh, 0C2h, 0ACh, 02Ch, 030h, 072h, 0D1h, 03Ch, 009h
        db  077h, 0CDh, 06Bh, 0D2h, 00Ah, 003h, 0D0h, 0ACh, 02Ch, 030h, 072h, 0C3h, 03Ch, 009h, 077h, 0BFh
        db  06Bh, 0D2h, 00Ah, 003h, 0D0h, 0ACh, 02Ch, 030h, 072h, 0B5h, 03Ch, 009h, 077h, 0B1h, 06Bh, 0D2h
        db  00Ah, 003h, 0D0h, 081h, 0FAh, 0FFh, 000h, 000h, 000h, 077h, 0A4h, 08Bh, 044h, 024h, 004h, 00Fh
        db  0ABh, 010h, 0FFh, 004h, 024h, 0EBh, 098h, 0B8h, 000h, 0DCh, 000h, 000h, 0CDh, 020h, 032h, 000h
        db  040h, 000h, 083h, 03Ch, 024h, 001h, 08Dh, 0A4h, 024h, 058h, 001h, 000h, 000h, 05Eh, 05Fh, 05Bh
        db  058h, 0C3h, 060h, 081h, 0ECh, 010h, 000h, 000h, 000h, 08Dh, 03Ch, 024h, 08Bh, 0D7h, 08Bh, 035h
        dd  obj1:0x0E48              ; fixup
        db  0B9h, 010h, 000h, 000h, 000h, 0FCh, 0F3h, 0A4h, 000h, 002h, 0B8h, 000h, 043h, 000h, 000h, 033h
        db  0DBh, 08Bh, 0F2h, 0CDh, 020h, 032h, 000h, 040h, 000h, 0F5h, 08Dh, 064h, 024h, 010h, 061h, 0C3h
        db  08Bh, 05Dh, 03Ch, 00Fh, 0B6h, 095h, 088h, 000h, 000h, 000h, 0B8h
        dd  obj1:0x0E44              ; fixup
        db  08Bh, 000h, 00Bh, 0C0h, 074h, 011h, 03Bh, 0C5h, 074h, 0F6h, 039h, 058h, 03Ch, 075h, 0F1h, 088h
        db  090h, 089h, 000h, 000h, 000h, 0EBh, 0E9h, 039h, 02Ch, 095h
        dd  loc_1_0DB0               ; fixup
        db  075h, 00Bh, 0C7h, 004h, 095h
        dd  loc_1_0DB0               ; fixup
        db  000h, 000h, 000h, 000h, 086h, 095h, 089h, 000h, 000h, 000h, 088h, 095h, 088h, 000h, 000h, 000h
        db  089h, 02Ch, 095h
        dd  loc_1_0DB0               ; fixup
        db  00Fh, 0BAh, 07Dh, 004h, 000h, 00Fh, 0B6h, 095h, 089h, 000h, 000h, 000h, 0B9h
        dd  obj1:0x0E44              ; fixup
        db  08Bh, 009h, 0E3h, 00Fh, 038h, 091h, 088h, 000h, 000h, 000h, 075h, 0F4h, 089h, 00Ch, 095h
        dd  loc_1_0DB0               ; fixup
        db  0C3h, 083h, 0ECh, 004h, 089h, 00Ch, 024h, 08Bh, 0FBh, 08Bh, 0D8h, 06Ah, 001h, 068h, 000h, 002h
        db  000h, 000h, 0CDh, 020h, 04Fh, 000h, 001h, 000h, 083h, 0C4h, 008h, 083h, 0F8h, 001h, 00Fh, 082h
        db  09Bh, 000h, 000h, 000h, 08Bh, 0F0h, 0B8h, 002h, 0D6h, 000h, 000h, 0B9h, 048h, 000h, 000h, 000h
        db  033h, 0D2h, 0CDh, 020h, 032h, 000h, 040h, 000h, 0BAh, 00Dh, 000h, 000h, 000h, 072h, 070h, 03Bh
        db  0C1h, 0F9h, 075h, 06Bh, 0E8h
        dd  sub_7_0147               ; fixup
        db  072h, 064h, 057h, 0E8h, 0EAh, 000h, 000h, 000h, 05Fh, 072h, 05Bh, 080h, 04Dh, 004h, 008h, 0E8h
        db  075h, 000h, 000h, 000h, 0BAh, 004h, 000h, 000h, 000h, 072h, 064h, 00Fh, 0BAh, 024h, 024h, 01Dh
        db  073h, 009h, 056h, 0E8h
        dd  loc_7_0096               ; fixup
        db  05Eh, 072h, 054h, 08Bh, 00Ch, 024h, 053h, 057h, 056h, 0E8h, 04Bh, 001h, 000h, 000h, 05Eh, 05Fh
        db  05Bh, 0BAh, 002h, 000h, 000h, 000h, 072h, 03Fh, 08Bh, 0C7h, 0E8h, 0CDh, 0FEh, 0FFh, 0FFh, 01Bh
        db  0C0h, 025h, 000h, 000h, 004h, 000h, 009h, 045h, 004h, 083h, 065h, 004h, 0EFh, 00Fh, 0BAh, 024h
        db  024h, 01Dh, 073h, 00Ah, 0BAh, 00Ch, 000h, 000h, 000h, 00Fh, 0BAh, 065h, 004h, 00Fh, 09Ch, 052h
        db  06Ah, 000h, 056h, 0CDh, 020h, 051h, 000h, 001h, 000h, 083h, 0C4h, 008h, 05Ah, 09Dh, 08Bh, 0C3h
        db  08Bh, 0DFh, 08Dh, 064h, 024h, 004h, 0C3h, 052h, 0E8h, 0B3h, 002h, 000h, 000h, 05Ah, 0F9h, 0EBh
        db  0DDh, 08Bh, 055h, 04Ch, 042h, 0C1h, 0E2h, 009h, 0B8h, 002h, 0D6h, 000h, 000h, 0B9h, 000h, 002h
        db  000h, 000h, 0FFh, 076h, 033h, 0CDh, 020h, 032h, 000h, 040h, 000h, 05Ah, 072h, 04Bh, 03Bh, 0C1h
        db  072h, 047h, 08Bh, 006h, 0C1h, 0E0h, 008h, 066h, 081h, 0FAh, 000h, 001h, 073h, 007h, 03Dh, 000h
        db  0F8h, 044h, 052h, 0EBh, 005h, 03Dh, 000h, 0F8h, 008h, 022h, 0F9h, 075h, 02Ch, 08Bh, 055h, 050h
        db  04Ah, 0C1h, 0E2h, 009h, 0B8h, 002h, 0D6h, 000h, 000h, 0B9h, 000h, 002h, 000h, 000h, 0CDh, 020h
        db  032h, 000h, 040h, 000h, 072h, 013h, 03Bh, 0C1h, 072h, 00Fh, 08Bh, 006h, 0C1h, 0E0h, 008h, 03Dh
        db  000h, 04Dh, 044h, 052h, 00Fh, 095h, 0C0h, 0D0h, 0E8h, 0C3h, 056h, 083h, 0ECh, 008h, 08Bh, 0F4h
        db  0C7h, 006h, 000h, 000h, 000h, 000h, 0B8h, 012h, 0DFh, 000h, 000h, 0CDh, 020h, 032h, 000h, 040h
        db  000h, 08Bh, 04Eh, 004h, 08Dh, 064h, 024h, 008h, 05Eh, 0BAh, 008h, 000h, 000h, 000h, 072h, 05Eh
        db  08Bh, 0C7h, 0E8h
        dd  sub_7_02C9               ; fixup
        db  091h, 08Bh, 051h, 030h, 00Bh, 0D2h, 074h, 015h, 066h, 083h, 07Ah, 011h, 000h, 075h, 00Eh, 0BAh
        db  005h, 000h, 000h, 000h, 080h, 03Dh
        dd  obj1:0x0E64              ; fixup
        db  001h, 072h, 03Ah, 051h, 0E8h, 056h, 005h, 000h, 000h, 059h, 0BAh, 003h, 000h, 000h, 000h, 072h
        db  02Ch, 08Bh, 0E8h, 08Bh, 0C7h, 088h, 085h, 089h, 000h
VxD__LDATA ENDS

VxD__LTEXT SEGMENT   ; object 6  vsize=0x1900  R-X
sub_6_0000:
        inc    ebp
        sbb    al, 3
        add    dword ptr [eax], edi
        ror    dword ptr [edi], cl
        test   dword ptr [edx], eax
        add    al, byte ptr [eax]
        add    byte ptr [ebp - 0x75], dl
        sub    al, 0x95
        mov    al, 0xd
        add    byte ptr [eax], al
        movzx  ecx, byte ptr [ebp + 0x89]
        push   edx
        push   ecx
        push   dword ptr [ebp + 0x3c]
        VxDCall  IFSMgr.IFSMgr_SwapDrives        ; cd205c004000
        add    esp, 0xc
        or     eax, eax
        jne    loc_6_003E
        call   0xfffff624
        pop    ebp
        mov    word ptr [ebp + 0x1c], 0
        jmp    loc_6_020E
loc_6_003E:
        pop    ebp
        mov    word ptr [ebp + 0x1c], 0x101
        jmp    loc_6_020E
        db  066h, 0C7h, 045h, 01Ch, 002h, 001h, 080h, 03Dh
        dd  obj5:0x082C              ; fixup
        db  000h, 00Fh, 084h, 0B1h, 001h, 000h, 000h, 066h, 0C7h, 045h, 01Ch, 011h, 04Ah, 0E9h
        dd  loc_1_01D8               ; fixup
        db  0C6h, 045h, 018h, 0FFh, 0E9h, 09Dh, 001h, 000h, 000h, 096h, 066h, 0B8h, 004h, 038h, 0CDh, 020h
        db  01Ch, 000h, 001h, 000h, 096h, 0C6h, 046h, 003h, 00Bh, 080h, 07Eh, 002h, 061h, 074h, 00Ah, 080h
        db  07Eh, 002h, 041h, 00Fh, 085h, 07Dh, 001h, 000h, 000h, 0C6h, 046h, 003h, 001h, 00Fh, 0B6h, 055h
        db  014h, 080h, 0FAh, 01Ah, 00Fh, 083h, 06Ch, 001h, 000h, 000h, 00Fh, 0B6h, 046h, 057h, 03Ch, 01Ah
        db  00Fh, 083h, 060h, 001h, 000h, 000h, 0C6h, 046h, 003h, 002h, 083h, 03Ch, 095h
        dd  loc_1_0DB0               ; fixup
        db  000h, 00Fh, 085h, 04Eh, 001h, 000h, 000h, 083h, 03Ch, 085h
        dd  loc_1_0DB0               ; fixup
        db  000h, 00Fh, 085h, 040h, 001h, 000h, 000h, 08Bh, 0D8h, 08Ah, 04Eh, 058h, 053h, 052h, 056h, 0E8h
        db  0CFh, 0F3h, 0FFh, 0FFh, 05Eh, 059h, 05Bh, 0BAh, 005h, 000h, 000h, 000h, 00Fh, 082h, 0A8h, 000h
        db  000h, 000h, 08Ah, 0E9h, 080h, 07Eh, 002h, 061h, 075h, 004h, 00Fh, 0BAh, 0E9h, 01Eh, 00Fh, 0BAh
        db  0E9h, 01Dh, 056h, 0E8h, 097h, 0F5h, 0FFh, 0FFh, 05Eh, 09Ch, 052h, 08Bh, 0D8h, 0B8h, 000h, 0D7h
        db  000h, 000h, 0CDh, 020h, 032h, 000h, 040h, 000h, 05Ah, 09Dh, 072h, 074h, 08Ah, 046h, 058h, 088h
        db  085h, 0B2h, 000h, 000h, 000h, 08Bh, 045h, 038h, 08Bh, 040h, 018h, 089h, 045h, 03Ch, 08Ah, 095h
        db  089h, 000h, 000h, 000h, 0B9h
        dd  obj1:0x0E44              ; fixup
        db  08Bh, 009h, 0E3h, 015h, 03Bh, 0CDh, 074h, 0F8h, 039h, 041h, 03Ch, 075h, 0F3h, 0F6h, 041h, 004h
        db  001h, 074h, 0EDh, 08Ah, 091h, 088h, 000h, 000h, 000h, 088h, 095h, 08Ah, 000h, 000h, 000h, 00Fh
        db  0B6h, 095h, 088h, 000h, 000h, 000h, 052h, 0FFh, 075h, 03Ch, 0CDh, 020h, 05Ah, 000h, 040h, 000h
        db  083h, 0C4h, 008h, 00Bh, 0C0h, 075h, 012h, 080h, 07Eh, 003h, 00Ch, 00Fh, 084h, 09Dh, 000h, 000h
        db  000h, 088h, 046h, 003h, 0E9h, 095h, 000h, 000h, 000h, 0C6h, 046h, 003h, 002h, 0E8h, 052h, 0F9h
        db  0FFh, 0FFh, 0E9h, 087h, 000h, 000h, 000h, 083h, 0FAh, 00Ch, 075h, 005h, 088h, 056h, 003h, 0EBh
        db  082h, 080h, 03Dh
        dd  obj5:0x082C              ; fixup
        db  000h, 00Fh, 085h
        dd  loc_1_01D8               ; fixup
        db  088h, 056h, 003h, 0EBh, 06Bh, 066h, 0C7h, 045h, 01Ch, 001h, 001h, 00Fh, 0B6h, 055h, 014h, 080h
        db  0FAh, 01Ah, 073h, 05Ch, 08Bh, 004h, 095h
        dd  loc_1_0DB0               ; fixup
        db  00Bh, 0C0h, 074h, 053h, 0F6h, 040h, 004h, 001h, 075h, 04Bh, 083h, 0B8h, 0B8h, 000h, 000h, 000h
        db  000h, 074h, 019h, 08Bh, 050h, 038h, 08Bh, 052h, 030h, 0F6h, 042h, 03Fh, 001h, 074h, 00Dh, 0F6h
        db  040h, 004h, 004h, 075h, 007h, 050h, 0E8h, 051h, 002h, 000h, 000h, 058h, 08Bh, 0F0h, 00Fh, 0B6h
        db  090h, 088h, 000h, 000h, 000h, 052h, 0FFh, 070h, 03Ch, 0CDh, 020h, 05Bh, 000h, 040h, 000h, 083h
        db  0C4h, 008h, 00Bh, 0C0h, 075h, 00Fh, 055h, 08Bh, 0EEh, 0E8h, 0CDh, 0F8h, 0FFh, 0FFh, 05Dh, 033h
        db  0C0h
loc_6_020A:
        mov    word ptr [ebp + 0x1c], ax
loc_6_020E:
        clc    
        ret    
        db  066h, 0C7h, 045h, 01Ch, 002h, 001h, 080h, 03Dh
        dd  obj5:0x082C              ; fixup
        db  000h, 074h, 0EFh, 066h, 0C7h, 045h, 01Ch, 011h, 04Ah, 0E9h
        dd  loc_1_01D8               ; fixup
        db  080h, 03Dh
        dd  obj5:0x082C              ; fixup
        db  000h, 00Fh, 085h
        dd  loc_1_01D8               ; fixup
        db  066h, 0B8h, 000h
loc_6_023A:
        add    ebx, ebp
        int    0xf
        mov    bh, 5
        jb     loc_6_024F
        add    byte ptr [eax], al
        mov    word ptr [ebp + 0x10], ax
        mov    ecx, 0xe44                    ; -> obj1:0x0E44
loc_6_024D:
        mov    ecx, dword ptr [ecx]
loc_6_024F:
        jecxz  loc_6_0264
        mov    edx, dword ptr [ecx + 0xc0]
        sub    edx, dword ptr [ecx + 0xbc]
        shr    edx, 3
        sub    eax, edx
        jmp    loc_6_024D
loc_6_0264:
        or     eax, eax
        jge    loc_6_026E
        sub    word ptr [ebp + 0x10], ax
        xor    eax, eax
loc_6_026E:
        cmp    word ptr [0xd70], ax          ; -> loc_1_0D70
        jae    loc_6_027D
        mov    ax, word ptr [0xd70]          ; -> loc_1_0D70
loc_6_027D:
        mov    word ptr [ebp + 0x18], ax
        xor    eax, eax
        jmp    loc_6_020A
        db  066h, 0B8h, 0AAh, 0AAh, 066h, 089h, 045h, 010h, 066h, 089h, 045h, 000h, 066h, 089h, 045h, 004h
        db  0C6h, 045h, 019h, 000h, 0A0h
        dd  obj1:0x0D79              ; fixup
        db  088h, 045h, 018h, 0A1h
        dd  loc_1_0D6C               ; fixup
        db  066h, 089h, 045h, 000h, 0C1h, 0E8h, 010h, 066h, 089h, 045h, 014h, 033h, 0C0h, 0E9h, 052h, 0FFh
        db  0FFh, 0FFh, 066h, 08Bh, 045h, 018h, 0C1h, 0E0h, 010h, 066h, 08Bh, 045h, 014h, 087h, 005h
        dd  loc_1_0D64               ; fixup
        db  080h, 03Dh
        dd  obj5:0x082C              ; fixup
        db  000h, 00Fh, 085h
        dd  loc_1_01D8               ; fixup
        db  066h, 089h, 045h, 014h, 0C1h, 0E8h, 010h, 066h, 089h, 045h, 018h, 033h, 0C0h, 0E9h, 022h, 0FFh
        db  0FFh, 0FFh, 0A1h
        dd  loc_1_0D64               ; fixup
        db  0EBh, 0DAh
        dd  obj4:0x1333              ; fixup
        dd  obj4:0x1316              ; fixup
        dd  loc_4_1377               ; fixup
        dd  loc_4_1341               ; fixup
        db  00Fh, 0B6h, 055h, 014h, 081h, 0FAh, 004h, 000h, 000h, 000h, 00Fh, 083h, 0F9h, 0FEh, 0FFh, 0FFh
        db  0FFh, 024h, 095h
        dd  loc_4_12EF               ; fixup
        db  00Fh, 0B7h, 045h, 018h, 0A3h
        dd  loc_1_0E30               ; fixup
        db  083h, 0E8h, 004h, 01Bh, 0D2h, 023h, 0C2h, 083h, 0C0h, 004h, 0A3h
        dd  loc_1_0E2C               ; fixup
        db  0E9h
        dd  loc_1_01D8               ; fixup
        db  0A1h
        dd  loc_1_0E30               ; fixup
        db  066h, 089h, 045h, 018h, 0E9h, 0CDh, 0FEh, 0FFh, 0FFh, 00Fh, 0B7h, 045h, 018h, 083h, 0F8h, 064h
        db  00Fh, 087h, 0BAh, 0FEh, 0FFh, 0FFh, 0A3h
        dd  loc_1_0E34               ; fixup
        db  0B9h
        dd  obj1:0x0E44              ; fixup
        db  0BEh, 064h, 000h, 000h, 000h, 08Bh, 009h, 00Bh, 0C9h, 00Fh, 084h
        dd  loc_1_01D8               ; fixup
        db  08Bh, 041h, 054h, 0F7h, 025h
        dd  loc_1_0E34               ; fixup
        db  0F7h, 0F6h, 089h, 041h, 05Ch, 0EBh, 0E6h, 0A1h
        dd  loc_1_0E34               ; fixup
        db  066h, 089h, 045h, 018h, 0E9h, 089h, 0FEh, 0FFh, 0FFh, 080h, 04Dh, 02Ch, 001h, 0C3h, 080h, 0FAh
        db  01Ah, 0F5h, 072h, 050h, 033h, 0DBh, 08Bh, 004h, 095h
        dd  loc_1_0DB0               ; fixup
        db  00Bh, 0C0h, 074h, 01Ch, 0B3h, 080h, 08Ah, 0B8h, 0B2h, 000h, 000h, 000h, 08Ah, 090h, 089h, 000h
        db  000h, 000h, 0F6h, 040h, 004h, 001h, 075h, 02Dh, 08Ah, 090h, 08Ah, 000h, 000h, 000h, 0EBh, 025h
        db  0B9h, 01Ah, 000h, 000h, 000h, 08Bh, 004h, 08Dh
        dd  loc_1_0DAC               ; fixup
        db  00Bh, 0C0h, 074h, 00Eh, 038h, 090h, 089h, 000h, 000h, 000h, 075h, 006h, 0F6h, 040h, 004h, 001h
        db  075h, 004h, 0E2h, 0E5h, 0EBh, 003h, 049h, 08Bh, 0D1h, 00Ah, 0DAh, 0C3h, 083h, 0ECh, 004h, 08Dh
        db  00Ch, 024h, 050h, 051h, 052h, 051h, 052h, 050h, 0CDh, 020h, 048h, 001h, 001h, 000h, 083h, 0C4h
        db  00Ch, 083h, 0F8h, 001h, 05Ah, 059h, 058h, 072h, 00Fh, 051h, 052h, 050h, 0CDh, 020h, 04Ah, 001h
        db  001h, 000h, 083h, 0C4h, 00Ch, 083h, 0F8h, 001h, 0F5h, 08Bh, 004h, 024h, 08Dh, 064h, 024h, 004h
        db  0C3h, 08Bh, 0F2h, 0FCh, 050h, 0ACh, 00Ah, 0C0h, 075h, 0FBh, 058h, 02Bh, 0F2h, 04Eh, 056h, 052h
        db  06Ah, 001h, 06Ah, 000h, 051h, 050h, 0CDh, 020h, 052h, 001h, 001h, 000h, 083h, 0C4h, 018h, 0C3h
        db  0B8h, 002h, 000h, 000h, 080h, 0BAh
        dd  obj5:0x0788              ; fixup
        db  0E8h, 09Dh, 0FFh, 0FFh, 0FFh, 072h, 01Bh, 0B9h
        dd  obj5:0x07B2              ; fixup
        db  0BAh
        dd  obj5:0x07BE              ; fixup
        db  050h, 0E8h, 0C0h, 0FFh, 0FFh, 0FFh, 058h, 050h, 0CDh, 020h, 049h, 001h, 001h, 000h, 083h, 0C4h
        db  004h, 0C3h, 000h, 000h, 080h, 03Dh
        dd  obj1:0x0E66              ; fixup
        db  000h, 00Fh, 084h, 03Fh, 002h, 000h, 000h, 055h, 053h, 057h, 056h, 0E8h
        dd  sub_1_0091               ; fixup
        db  00Fh, 082h, 044h, 002h, 000h, 000h, 0CDh, 020h, 011h, 001h, 001h, 000h, 03Dh, 000h, 000h, 000h
        db  020h, 077h, 038h, 083h, 0ECh, 06Ch, 057h, 08Dh, 07Ch, 024h, 004h, 0CDh, 020h, 08Dh, 000h, 001h
        db  000h, 05Fh, 0CDh, 020h, 001h, 000h, 001h, 000h, 08Bh, 06Bh, 008h, 0CDh, 020h, 082h, 000h, 001h
        db  000h, 0E8h, 01Dh, 002h, 000h, 000h, 0CDh, 020h, 086h, 000h, 001h, 000h, 056h, 08Dh, 074h, 024h
        db  004h, 0CDh, 020h, 08Eh, 000h, 001h, 000h, 05Eh, 083h, 0C4h, 06Ch, 033h, 0EDh, 0BAh, 019h, 000h
        db  000h, 000h, 00Fh, 0B6h, 082h
        dd  obj5:0x07F8              ; fixup
        db  03Ch, 0FFh, 00Fh, 084h, 0ACh, 000h, 000h, 000h, 0B6h, 046h, 0E8h, 0E9h, 002h, 000h, 000h, 0B6h
        db  000h, 00Fh, 082h, 0C7h, 001h, 000h, 000h, 0E8h, 0DEh, 003h, 000h, 000h, 00Fh, 082h, 0BCh, 001h
        db  000h, 000h, 089h, 028h, 08Bh, 0E8h, 0B6h, 049h, 0E8h, 0CBh, 002h, 000h, 000h, 032h, 0F6h, 0F6h
        db  045h, 004h, 004h, 074h, 07Fh, 083h, 0A5h, 0B8h, 000h, 000h, 000h, 000h, 060h, 083h, 0ECh, 06Ch
        db  057h, 08Dh, 07Ch, 024h, 004h, 0CDh, 020h, 08Dh, 000h, 001h, 000h, 05Fh, 0CDh, 020h, 001h, 000h
        db  001h, 000h, 08Bh, 06Bh, 008h, 0CDh, 020h, 082h, 000h, 001h, 000h, 088h, 055h, 014h, 066h, 0C7h
        db  045h, 01Ch, 011h, 04Ah, 00Fh, 0A3h, 015h
        dd  loc_1_0D68               ; fixup
        db  072h, 020h, 066h, 0C7h, 045h, 010h, 002h, 000h, 0B8h, 02Fh, 000h, 000h, 000h, 0CDh, 020h, 084h
        db  000h, 001h, 000h, 08Ah, 08Ah
        dd  obj5:0x07F8              ; fixup
        db  088h, 04Dh, 014h, 066h, 0C7h, 045h, 01Ch, 011h, 04Ah, 066h, 0C7h, 045h, 010h, 006h, 000h, 0B8h
        db  02Fh, 000h, 000h, 000h, 0CDh, 020h, 084h, 000h, 001h, 000h, 0CDh, 020h, 086h, 000h, 001h, 000h
        db  056h, 08Dh, 074h, 024h, 004h, 0CDh, 020h, 08Eh, 000h, 001h, 000h, 05Eh, 083h, 0C4h, 06Ch, 061h
        db  04Ah, 00Fh, 08Dh, 03Eh, 0FFh, 0FFh, 0FFh, 08Bh, 0DDh, 00Bh, 0EDh, 00Fh, 084h, 099h, 000h, 000h
        db  000h, 00Fh, 0B6h, 085h, 088h, 000h, 000h, 000h, 0E8h
        dd  sub_7_02C9               ; fixup
        db  072h, 057h, 083h, 0ECh, 008h, 089h, 044h, 024h, 004h, 08Bh, 0C4h, 066h, 0C7h, 000h, 00Ah, 000h
        db  050h, 0FFh, 015h
        dd  sub_1_0958               ; fixup
        db  083h, 0C4h, 00Ch, 08Bh, 045h, 038h, 00Fh, 0B6h, 08Dh, 08Ah, 000h, 000h, 000h, 088h, 048h, 014h
        db  08Ah, 08Dh, 088h, 000h, 000h, 000h, 0E8h, 0DAh, 0F5h, 0FFh, 0FFh, 00Fh, 0A3h, 00Dh
        dd  loc_1_0D68               ; fixup
        db  072h, 01Eh, 066h, 0BEh, 013h, 000h, 0E8h, 065h, 0EDh, 0FFh, 0FFh, 051h, 08Ah, 08Dh, 089h, 000h
        db  000h, 000h, 066h, 0BEh, 012h, 000h, 0E8h, 055h, 0EDh, 0FFh, 0FFh, 059h, 072h, 002h, 088h, 00Eh
        db  08Ah, 08Dh, 088h, 000h, 000h, 000h, 08Ah, 0ADh, 089h, 000h, 000h, 000h, 08Bh, 055h, 038h, 0F6h
        db  045h, 004h, 004h, 074h, 012h, 08Ah, 0ADh, 08Ah, 000h, 000h, 000h, 0F6h, 045h, 004h, 001h, 074h
        db  006h, 08Ah, 08Dh, 089h, 000h, 000h, 000h, 0E8h, 0BBh, 0F2h, 0FFh, 0FFh, 08Bh, 06Dh, 000h, 0E9h
        db  05Fh, 0FFh, 0FFh, 0FFh, 00Bh, 0DBh, 074h, 013h, 08Bh, 0EBh, 08Bh, 01Bh, 08Bh, 055h, 038h, 0E8h
        db  038h, 0F3h, 0FFh, 0FFh, 0E8h
        dd  loc_1_00C1               ; fixup
        db  0EBh, 0E9h, 080h, 03Dh
        dd  obj5:0x082C              ; fixup
        db  000h, 075h, 056h, 066h, 083h, 03Dh
        dd  loc_1_0D74               ; fixup
        db  00Ch, 072h, 04Ch, 060h, 083h, 0ECh, 06Ch, 057h, 08Dh, 07Ch, 024h, 004h, 0CDh, 020h, 08Dh, 000h
        db  001h, 000h, 05Fh, 0CDh, 020h, 001h, 000h, 001h, 000h, 08Bh, 06Bh, 008h, 0CDh, 020h, 083h, 000h
        db  001h, 000h, 066h, 0C7h, 045h, 01Ch, 011h, 04Ah, 066h, 0C7h, 045h, 010h, 00Dh, 000h, 0B8h, 02Fh
        db  000h, 000h, 000h, 0CDh, 020h, 084h, 000h, 001h, 000h, 0CDh, 020h, 086h, 000h, 001h, 000h, 056h
        db  08Dh, 074h, 024h, 004h, 0CDh, 020h, 08Eh, 000h, 001h, 000h, 05Eh, 083h, 0C4h, 06Ch, 061h, 0E8h
        dd  sub_1_0135               ; fixup
        db  05Eh, 05Fh, 05Bh, 05Dh, 0C3h, 0C6h, 005h
        dd  obj5:0x082C              ; fixup
        db  001h, 0C6h, 082h
        dd  obj5:0x07F8              ; fixup
        db  0FFh, 0E9h, 0C3h, 0FEh, 0FFh, 0FFh, 0C6h, 005h
        dd  obj5:0x082C              ; fixup
        db  001h, 0EBh, 0DFh, 0B9h, 01Ah, 000h, 000h, 000h, 033h, 0DBh, 089h, 01Dh
        dd  loc_1_0E20               ; fixup
        db  089h, 01Dh
        dd  loc_1_0D68               ; fixup
        db  0C6h, 083h
        dd  obj5:0x07F8              ; fixup
        db  0FFh, 066h, 0C7h, 045h, 01Ch, 011h, 04Ah, 066h, 0C7h, 045h, 010h, 001h, 000h, 088h, 05Dh, 014h
        db  0B8h, 02Fh, 000h, 000h, 000h, 0CDh, 020h, 084h, 000h, 001h, 000h, 066h, 083h, 07Dh, 01Ch, 000h
        db  075h, 07Eh, 08Ah, 045h, 010h, 0A8h, 080h, 074h, 077h, 024h, 07Fh, 088h, 083h
        dd  obj5:0x07F8              ; fixup
        db  066h, 0C7h, 045h, 01Ch, 011h, 04Ah, 066h, 0C7h, 045h, 010h, 001h, 000h, 088h, 045h, 014h, 0B8h
        db  02Fh, 000h, 000h, 000h, 0CDh, 020h, 084h, 000h, 001h, 000h, 066h, 083h, 07Dh, 01Ch, 000h, 075h
        db  04Eh, 08Ah, 045h, 010h, 024h, 07Fh, 088h, 083h
        dd  obj5:0x0812              ; fixup
        db  038h, 0D8h, 074h, 013h, 086h, 083h
        dd  obj5:0x07F8              ; fixup
        db  088h, 083h
        dd  obj5:0x0812              ; fixup
        db  00Fh, 0ABh, 01Dh
        dd  loc_1_0D68               ; fixup
        db  0B0h, 0FFh, 086h, 083h
        dd  obj5:0x07F8              ; fixup
        db  0E8h
        dd  loc_3_0000               ; fixup
        db  072h, 01Dh, 088h, 083h
        dd  obj5:0x07F8              ; fixup
        db  066h, 089h, 034h, 05Dh
        dd  obj5:0x07C4              ; fixup
        db  00Fh, 0B6h, 0C0h, 00Fh, 0ABh, 005h
        dd  loc_1_0E20               ; fixup
        db  0E8h
        dd  loc_3_0133               ; fixup
        db  043h, 049h, 00Fh, 085h, 052h, 0FFh, 0FFh, 0FFh, 080h, 03Dh
        dd  obj5:0x082C              ; fixup
        db  000h, 074h, 033h, 0B9h, 01Ah, 000h, 000h, 000h, 08Ah, 081h
        dd  obj5:0x07F7              ; fixup
        db  03Ch, 0FFh, 0E1h, 0F6h, 074h, 022h, 00Fh, 0B6h, 099h
        dd  obj5:0x0812              ; fixup
        db  038h, 0D8h, 074h, 013h, 038h, 0D9h, 074h, 00Fh, 080h, 0BBh
        dd  obj5:0x07F8              ; fixup
        db  0FFh, 075h, 006h, 088h, 081h
        dd  obj5:0x0812              ; fixup
        db  00Bh, 0C9h, 075h, 0D2h, 0C3h, 060h, 083h, 0ECh, 06Ch, 057h, 08Dh, 07Ch, 024h, 004h, 0CDh, 020h
        db  08Dh, 000h, 001h, 000h, 05Fh, 0CDh, 020h, 001h, 000h, 001h, 000h, 08Bh, 06Bh, 008h, 0CDh, 020h
        db  083h, 000h, 001h, 000h, 052h, 06Ah, 000h, 06Ah, 011h, 0CDh, 020h, 0A9h, 000h, 001h, 000h, 083h
        db  0C4h, 008h, 05Ah, 00Bh, 0C0h, 074h, 06Bh, 08Bh, 0F8h, 066h, 0C7h, 007h, 04Dh, 044h, 088h, 077h
        db  002h, 066h, 0C7h, 047h, 003h, 03Fh, 03Fh, 066h, 0C7h, 045h, 01Ch, 004h, 044h, 0FEh, 0C2h, 088h
        db  055h, 010h, 066h, 0C7h, 045h, 018h, 011h, 000h, 0C1h, 0E8h, 004h, 066h, 089h, 045h, 03Ch, 066h
        db  0C7h, 045h, 014h, 000h, 000h, 0B8h, 021h, 000h, 000h, 000h, 0CDh, 020h, 084h, 000h, 001h, 000h
        db  0F6h, 045h, 02Ch, 001h, 075h, 026h, 066h, 081h, 07Fh, 003h, 04Bh, 04Fh, 075h, 01Eh, 0CDh, 020h
        db  0AAh, 000h, 001h, 000h, 0CDh, 020h, 086h, 000h, 001h, 000h, 056h, 08Dh, 074h, 024h, 004h, 0CDh
        db  020h, 08Eh, 000h, 001h, 000h, 05Eh, 083h, 0C4h, 06Ch, 061h, 0F8h, 0C3h, 0CDh, 020h, 0AAh, 000h
        db  001h, 000h, 0CDh, 020h, 086h, 000h, 001h, 000h, 056h, 08Dh, 074h, 024h, 004h, 0CDh, 020h, 08Eh
        db  000h, 001h, 000h, 05Eh, 083h, 0C4h, 06Ch, 061h, 0F9h, 0C3h, 066h, 0C7h, 045h, 01Ch, 011h, 04Ah
        db  066h, 0C7h, 045h, 010h, 007h, 000h, 088h, 055h, 014h, 0B8h, 02Fh, 000h, 000h, 000h, 0CDh, 020h
        db  084h, 000h, 001h, 000h, 00Fh, 0B7h, 045h, 01Ch, 00Bh, 0C0h, 075h, 02Ah, 096h, 066h, 0B8h, 004h
        db  03Ch, 0CDh, 020h, 01Ch, 000h, 001h, 000h, 096h, 083h, 0EEh, 002h, 080h, 07Eh, 026h, 0EBh, 074h
        db  015h, 080h, 07Eh, 026h, 0E9h, 074h, 00Fh, 083h, 0C8h, 0FFh, 0A2h
        dd  obj1:0x0D7B              ; fixup
        db  0C6h, 005h
        dd  obj1:0x0E66              ; fixup
        db  000h, 0C3h, 060h, 083h, 0ECh, 06Ch, 057h, 08Dh, 07Ch, 024h, 004h, 0CDh, 020h, 08Dh, 000h, 001h
        db  000h, 05Fh, 0CDh, 020h, 001h, 000h, 001h, 000h, 08Bh, 06Bh, 008h, 0CDh, 020h, 083h, 000h, 001h
        db  000h, 0E8h, 08Fh, 0FFh, 0FFh, 0FFh, 0CDh, 020h, 086h, 000h, 001h, 000h, 056h, 08Dh, 074h, 024h
        db  004h, 0CDh, 020h, 08Eh, 000h, 001h, 000h, 05Eh, 083h, 0C4h, 06Ch, 00Bh, 0C0h, 00Fh, 085h, 0C9h
        db  000h, 000h, 000h, 00Fh, 0B7h, 006h, 08Bh, 04Eh, 05Ch, 066h, 02Bh, 0C1h, 033h, 0D2h, 0BBh, 006h
        db  000h, 000h, 000h, 0F7h, 0F3h, 040h, 08Bh, 0D0h, 08Bh, 044h, 024h, 014h, 00Fh, 0B6h, 080h
        dd  obj5:0x07F8              ; fixup
        db  0E8h
        dd  sub_7_02C9               ; fixup
        db  08Bh, 0C8h, 08Bh, 0C2h, 050h, 0E8h, 039h, 0F4h, 0FFh, 0FFh, 08Bh, 0D8h, 059h, 00Fh, 082h, 091h
        db  000h, 000h, 000h, 089h, 05Ch, 024h, 01Ch, 08Bh, 054h, 024h, 014h, 088h, 093h, 088h, 000h, 000h
        db  000h, 00Fh, 0BFh, 004h, 055h
        dd  obj5:0x07C4              ; fixup
        db  066h, 089h, 083h, 0B4h, 000h, 000h, 000h, 080h, 03Dh
        dd  obj1:0x0E5E              ; fixup
        db  000h, 074h, 012h, 083h, 0F8h, 0FFh, 074h, 00Dh, 00Fh, 0B6h, 080h
        dd  loc_1_0E5F               ; fixup
        db  0CDh, 020h, 003h, 000h, 042h, 000h, 00Fh, 0B6h, 082h
        dd  obj5:0x07F8              ; fixup
        db  088h, 083h, 089h, 000h, 000h, 000h, 00Fh, 0A3h, 005h
        dd  obj5:0x07C0              ; fixup
        db  01Bh, 0C0h, 025h, 000h, 000h, 004h, 000h, 009h, 043h, 004h, 08Ah, 082h
        dd  obj5:0x0812              ; fixup
        db  088h, 083h, 08Ah, 000h, 000h, 000h, 0B0h, 001h, 00Fh, 0A3h, 015h
        dd  loc_1_0D68               ; fixup
        db  01Ch, 000h, 008h, 043h, 004h, 0E8h, 01Fh, 000h, 000h, 000h, 072h, 013h, 08Bh, 0EBh, 051h, 0E8h
        db  011h, 0F5h, 0FFh, 0FFh, 059h, 072h, 008h, 0E8h, 091h, 001h, 000h, 000h, 0F8h, 061h, 0C3h, 08Bh
        db  0EBh, 0E8h, 03Fh, 0F0h, 0FFh, 0FFh, 0F9h, 061h, 0C3h, 060h, 08Dh, 076h, 026h, 08Dh, 0BBh, 08Eh
        db  000h, 000h, 000h, 0B9h, 024h, 000h, 000h, 000h, 0FCh, 0F3h, 0A4h, 08Bh, 074h, 024h, 004h, 089h
        db  0B3h, 0B8h, 000h, 000h, 000h, 00Fh, 0B6h, 04Eh, 055h, 038h, 0CDh, 01Ah, 0C9h, 080h, 0E1h, 002h
        db  008h, 04Bh, 004h, 08Ah, 04Eh, 053h, 088h, 08Bh, 0B2h, 000h, 000h, 000h, 080h, 07Eh, 04Bh, 009h
        db  00Fh, 085h, 044h, 001h, 000h, 000h, 00Fh, 0B7h, 056h, 022h, 089h, 053h, 074h, 00Fh, 0B7h, 046h
        db  01Eh, 089h, 043h, 04Ch, 08Bh, 046h, 002h, 048h, 089h, 043h, 054h, 089h, 083h, 0F8h, 000h, 000h
        db  000h, 048h, 0C1h, 0E8h, 00Eh, 03Dh, 000h, 001h, 000h, 000h, 00Fh, 087h, 01Ah, 001h, 000h, 000h
        db  089h, 083h, 084h, 000h, 000h, 000h, 00Fh, 0B7h, 083h, 0A4h, 000h, 000h, 000h, 02Bh, 0D0h, 089h
        db  053h, 06Ch, 00Fh, 0B7h, 046h, 016h, 089h, 043h, 068h, 0C7h, 083h, 080h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 00Fh, 0B6h, 046h, 04Ah, 048h, 00Fh, 088h, 0ECh, 000h, 000h, 000h, 039h, 083h
        db  084h, 000h, 000h, 000h, 00Fh, 087h, 0E0h, 000h, 000h, 000h, 08Ah, 046h, 050h, 088h, 083h, 08Dh
        db  000h, 000h, 000h, 00Fh, 0B7h, 046h, 020h, 089h, 043h, 070h, 00Fh, 0BFh, 046h, 024h, 089h, 043h
        db  078h, 08Ah, 046h, 04Dh, 088h, 083h, 08Ch, 000h, 000h, 000h, 00Fh, 0B7h, 083h, 0A1h, 000h, 000h
        db  000h, 00Bh, 0C0h, 075h, 006h, 08Bh, 083h, 0AEh, 000h, 000h, 000h, 089h, 083h, 0AEh, 000h, 000h
        db  000h, 089h, 083h, 0F4h, 000h, 000h, 000h, 02Bh, 043h, 074h, 08Ah, 08Bh, 08Ch, 000h, 000h, 000h
        db  0D3h, 0E8h, 083h, 0C0h, 002h, 003h, 043h, 078h, 089h, 043h, 07Ch, 066h, 083h, 0BBh, 0B4h, 000h
        db  000h, 000h, 003h, 072h, 01Bh, 08Bh, 046h, 07Dh, 089h, 083h, 0F4h, 000h, 000h, 000h, 08Bh, 086h
        db  081h, 000h, 000h, 000h, 083h, 0F8h, 001h, 083h, 0D0h, 000h, 089h, 083h, 0F8h, 000h, 000h, 000h
        db  08Dh, 043h, 040h, 089h, 043h, 044h, 08Bh, 043h, 038h, 080h, 078h, 03Ch, 009h, 075h, 05Bh, 0C6h
        db  083h, 08Bh, 000h, 000h, 000h, 009h, 08Bh, 043h, 04Ch, 003h, 043h, 074h, 00Fh, 0B7h, 093h, 0A4h
        db  000h, 000h, 000h, 02Bh, 0C2h, 089h, 043h, 060h, 083h, 04Bh, 058h, 0FFh, 083h, 04Bh, 064h, 0FFh
        db  08Bh, 043h, 054h, 0F7h, 025h
        dd  loc_1_0E34               ; fixup
        db  0B9h, 064h, 000h, 000h, 000h, 0F7h, 0F1h, 089h, 043h, 05Ch, 066h, 083h, 03Dh
        dd  loc_1_0D74               ; fixup
        db  00Ah, 072h, 014h, 066h, 083h, 07Eh, 064h, 000h, 075h, 00Dh, 08Bh, 046h, 006h, 089h, 043h, 058h
        db  00Fh, 0B7h, 046h, 062h, 089h, 043h, 064h, 081h, 04Bh, 004h, 000h, 020h, 000h, 000h, 061h, 0C3h
        db  0F9h, 0EBh, 0FBh, 056h, 08Bh, 046h, 05Ch, 08Bh, 0D0h, 0C1h, 0E8h, 010h, 0C1h, 0E0h, 004h, 00Fh
        db  0B7h, 0F2h, 003h, 0F0h, 08Bh, 0BBh, 0BCh, 000h, 000h, 000h, 0FCh, 033h, 0C0h, 066h, 0ADh, 0ABh
        db  0A5h, 0E2h, 0FAh, 05Eh, 0C3h, 080h, 03Dh
        dd  loc_1_0E63               ; fixup
        db  000h, 074h, 02Eh, 060h, 0BEh
        dd  obj5:0x0068              ; fixup
        db  039h, 035h
        dd  obj5:0x0140              ; fixup
        db  076h, 012h, 08Bh, 03Eh, 08Bh, 046h, 004h, 083h, 0F8h, 0FFh, 074h, 003h, 089h, 047h, 006h, 083h
        db  0C6h, 008h, 0EBh, 0E6h, 0BBh, 019h, 000h, 000h, 000h, 0E8h, 08Ch, 0EDh, 0FFh, 0FFh, 04Bh, 079h
        db  0F8h, 061h, 0C3h, 033h, 0C0h, 087h, 005h
        dd  loc_7_4828               ; fixup
        db  00Bh, 0C0h, 074h, 02Dh, 02Dh, 000h, 020h, 000h, 000h, 06Ah, 000h, 050h, 0CDh, 020h, 055h, 000h
        db  001h, 000h, 083h, 0C4h, 008h, 0B9h, 008h, 000h, 000h, 000h, 08Bh, 034h, 08Dh
        dd  loc_7_4804               ; fixup
        db  0CDh, 020h, 008h, 000h, 08Bh, 004h, 051h, 0CDh, 020h, 005h, 000h, 08Bh, 004h, 059h, 0E2h, 0E9h
        db  0C3h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
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
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        dd  obj5:0x0068              ; fixup
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
        db  000h, 000h, 041h, 03Ah, 05Ch
loc_6_1779:
        inc    esp
loc_6_177A:
        aas    
        aas    
        push   ebx
        push   eax
loc_6_177E:
        inc    ecx
        inc    ebx
        inc    ebp
        xor    byte ptr cs:[eax], dh
        xor    byte ptr [eax], al
        add    byte ptr [eax], al
loc_6_1788:
        push   ebx
        jns    0x17fe
        db  074h
sub_6_178C:
        insd   dword ptr es:[edi], dx
        pop    esp
        inc    ebx
        jne    0x1804
        jb     0x17f9
        outsb  dx, byte ptr [esi]
        je     loc_6_17DA
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        je     0x180d
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        push   ebx
        je     0x17fd
        inc    ebx
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        je     0x1818
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        pop    esp
        push   ebx
        push   0x6f647475
        ja     loc_6_181F
        add    byte ptr [esi + 0x6f], al
        jb     loc_6_1819
        push   edx
        bound  ebp, qword ptr gs:[edi + 0x6f]
        je     loc_6_17BE
loc_6_17BE:
        xor    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_6_17DA:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_6_17F0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh
        db  0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h
loc_6_1819:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_6_181F:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_6_1829:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_6_1833:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_6_187F:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_6_188F:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_6_18C5:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
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

VxD__LTEXT SEGMENT   ; object 7  vsize=0x4c4c  R-X
loc_7_0000:
        dec    ebp
        push   ebx
        inc    esp
        inc    edx
        dec    esp
        db  036h, 02Eh, 030h
loc_7_0008:
        dec    ebp
        push   ebx
        inc    esp
        push   ebx
        push   eax
        xor    byte ptr cs:[ebx], dh
        sar    byte ptr [ecx + 0xe44], 0x8b  ; -> obj1:0x0E44
        or     ebx, esp
        pop    ds
        cmp    byte ptr [ecx + 0x8a], bl
        jne    0x17
        movzx  edx, byte ptr [ecx + 0x88]
        cmp    edx, ebx
        jne    loc_7_0035
        movzx  edx, byte ptr [ecx + 0x89]
loc_7_0035:
        bts    eax, edx
        jmp    0x17
        db  0C3h
loc_7_003B:
        mov    ebp, edx
        mov    dword ptr [ebp + 0x48], 0xffffff44
        mov    dword ptr [ebp + 0x23c], esp
        lea    eax, [ebp + 0xb6]
        mov    dword ptr [ebp + 0xe0], eax
        mov    dword ptr [ebp + 0xdc], 0x400
        btr    dword ptr [ebp + 4], 0xd
        bts    dword ptr [ebp + 4], 0xe
        or     dword ptr [ebp + 0x58], 0xffffffff
        or     dword ptr [ebp + 0x64], 0xffffffff
        call   sub_7_106E
        call   sub_7_1CEA
        btr    dword ptr [ebp + 4], 0xe
        mov    ebx, 0xffffff44
        call   sub_7_4344
        push   ebp
        VMMCall  VMM._SignalID                   ; cd2027010100
        add    esp, 4
        ret    
loc_7_0096:
        call   sub_1_0091
        jb     loc_7_00F0
        btr    dword ptr [ebp + 4], 0xd
        mov    esi, 0x3b                     ; -> loc_7_003B
        mov    edx, ebp
        push   eax
        push   ebx
        push   ecx
        xor    eax, eax
        mov    ebx, eax
        mov    ecx, 0x408
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
        pop    ecx
        pop    ebx
        pop    eax
loc_7_00BE:
        cli    
        bt     dword ptr [ebp + 4], 0xd
        jb     loc_7_00D9
        mov    eax, 5
        push   eax
        push   ebp
        VMMCall  VMM._BlockOnID                  ; cd20fa000100
        lea    esp, [esp + 8]
        jmp    loc_7_00BE
loc_7_00D9:
        call   sub_1_0135
        xor    eax, eax
        xchg   word ptr [ebp + 0xb6], ax
        mov    edx, 0xd
        cmp    eax, 0x10
        cmc    
loc_7_00F0:
        ret    
sub_7_00F1:
        push   edi
        push   esi
        lea    esi, [esi + 3]
        mov    edi, 0                        ; -> loc_7_0000
        mov    ecx, 8
        cld    
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        pop    esi
        pop    edi
        jne    loc_7_0123
        movzx  ecx, word ptr [esi + 0x33]
        cmp    ecx, 1
        je     loc_7_0122
        cmp    ecx, 2
        je     loc_7_0122
        cmp    ecx, 0x100
        jne    loc_7_0122
        mov    ecx, 3
loc_7_0122:
        ret    
loc_7_0123:
        push   edi
        push   esi
        lea    esi, [esi + 3]
        mov    edi, 8                        ; -> loc_7_0008
        mov    ecx, 8
        cld    
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        pop    esi
        pop    edi
        jne    loc_7_0122
        movzx  ecx, word ptr [esi + 0x33]
        cmp    ecx, 1
        je     loc_7_0122
        cmp    ecx, 0
        jmp    loc_7_0122
sub_7_0147:
        sub    esp, 0x28
        call   sub_7_00F1
        jne    loc_7_02AD
        mov    eax, edi
        call   sub_7_02C9
        jb     loc_7_02B4
        cmp    byte ptr [eax + 0x3c], 9
        jne    loc_7_02BB
        cmp    byte ptr [esi + 0xd], 0x10
        je     loc_7_0188
        cmp    word ptr [esi + 0x33], 0x100
        jb     loc_7_02C2
        cmp    byte ptr [esi + 0xd], 0x40
        jne    loc_7_02C2
loc_7_0188:
        cmp    word ptr [esi + 0xe], 0
        je     loc_7_02AD
        cmp    byte ptr [esi + 0x10], 1
        jne    loc_7_02AD
        cmp    byte ptr [esi + 0x15], 0xf8
        jne    loc_7_02AD
        cmp    word ptr [esi + 0x16], 0
        je     loc_7_02AD
        cmp    dword ptr [esi + 0x1c], 0
        jne    loc_7_02AD
        cmp    dword ptr [esi + 0x20], 1
        sbb    eax, eax
        cmp    word ptr [esi + 0x13], 1
        adc    eax, 0
        je     loc_7_02AD
        cmp    byte ptr [esi + 0x26], 9
        jne    loc_7_02BB
        movzx  eax, word ptr [esi + 0xb]
        bsf    edx, eax
        je     loc_7_02BB
        btr    eax, edx
        or     eax, eax
        jne    loc_7_02BB
        bts    eax, edx
        cmp    dl, 9
        jne    loc_7_02BB
        movzx  edx, word ptr [esi + 0x11]
        shl    edx, 5
        lea    edx, [eax + edx - 1]
        shr    edx, 9
        movzx  ecx, word ptr [esi + 0x16]
        movzx  eax, word ptr [esi + 0xe]
        add    ecx, eax
        cmp    word ptr [esi + 0x29], cx
        jne    loc_7_02AD
        add    edx, ecx
        cmp    word ptr [esi + 0x2b], dx
        jne    loc_7_02AD
        movzx  edx, word ptr [esi + 0x27]
        shl    edx, 9
        mov    eax, 0xd602
        mov    ecx, 0x24
        mov    dword ptr [esp + 0x24], esi
        lea    esi, [esp]
        cmp    ebx, -1
        jne    loc_7_0250
        call   sub_7_3692
        jmp    loc_7_0256
loc_7_0250:
        VxDCall  IFSMgr.IFSMgr_Ring0_FileIO      ; cd2032004000
loc_7_0256:
        mov    esi, dword ptr [esp + 0x24]
        jb     loc_7_02AD
        lea    edx, [esp]
        cmp    byte ptr [edx], 0xe9
        je     loc_7_026F
        cmp    byte ptr [edx], 0xeb
        jne    loc_7_02AD
        cmp    byte ptr [edx + 2], 0x90
        jne    loc_7_02AD
loc_7_026F:
        cmp    byte ptr [edx + 0x10], 2
        jne    loc_7_02AD
        movzx  eax, word ptr [edx + 0xb]
        cmp    word ptr [esi + 0xb], ax
        jne    loc_7_02BB
        mov    al, byte ptr [edx + 0xd]
        cmp    byte ptr [esi + 0xd], al
        jne    loc_7_02C2
        movzx  eax, word ptr [edx + 0xe]
        cmp    word ptr [esi + 0xe], ax
        jne    loc_7_02AD
        movzx  eax, word ptr [edx + 0x11]
        cmp    word ptr [esi + 0x11], ax
        jne    loc_7_02AD
        movzx  eax, word ptr [edx + 0x16]
        cmp    word ptr [esi + 0x16], ax
        jne    loc_7_02AD
loc_7_02A5:
        lea    esp, [esp + 0x28]
        ret    
loc_7_02AA:
        stc    
        jmp    loc_7_02A5
loc_7_02AD:
        mov    edx, 0xd
        jmp    loc_7_02AA
loc_7_02B4:
        mov    edx, 5
        jmp    loc_7_02AA
loc_7_02BB:
        mov    edx, 7
        jmp    loc_7_02AA
loc_7_02C2:
        mov    edx, 6
        jmp    loc_7_02AA
sub_7_02C9:
        sub    esp, 0xc
        mov    dword ptr [esp + 8], eax
        mov    eax, esp
        mov    word ptr [eax], 7
        push   eax
        call   sub_1_0958
        mov    eax, dword ptr [esp + 8]
        cmp    word ptr [esp + 6], 1
        cmc    
        lea    esp, [esp + 0x10]
        ret    
sub_7_02EE:
        cmp    ebx, -1
        je     loc_7_0312
        mov    eax, 0xd800
        VxDCall  IFSMgr.IFSMgr_Ring0_FileIO      ; cd2032004000
        jb     loc_7_04BF
        mov    edx, eax
        shr    edx, 9
        je     loc_7_04C0
        mov    dword ptr [ebp + 0x50], edx
loc_7_0312:
        movsx  eax, word ptr [esi + 0x33]
        cmp    eax, 0x100
        jne    loc_7_0322
        mov    eax, 3
loc_7_0322:
        mov    word ptr [ebp + 0xb4], ax
        cmp    byte ptr [0xe5e], 0           ; -> obj1:0x0E5E
        je     loc_7_0344
        cmp    eax, -1
        je     loc_7_0344
        movzx  eax, byte ptr [eax + 0xe5f]   ; -> loc_1_0E5F
        VxDCall  Dev_0042.Service_0003h          ; cd2003004200
loc_7_0344:
        lea    edi, [ebp + 0x8e]
        mov    ecx, 0x24
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        sub    esi, 0x24
        movsx  eax, word ptr [esi + 0x2d]
        mov    dword ptr [ebp + 0x78], eax
        movzx  edx, word ptr [esi + 0x2b]
        mov    dword ptr [ebp + 0x6c], edx
        movzx  eax, word ptr [esi + 0x27]
        mov    dword ptr [ebp + 0x4c], eax
        lea    eax, [eax + edx + 3]
        neg    eax
        add    eax, dword ptr [ebp + 0x50]
        jae    loc_7_04C0
        mov    dword ptr [ebp + 0x54], eax
        mov    dword ptr [ebp + 0xf8], eax
        movzx  ecx, word ptr [ebp + 0xa4]
        add    edx, ecx
        mov    dword ptr [ebp + 0x74], edx
        movzx  eax, word ptr [ebp + 0xa1]
        or     eax, eax
        jne    loc_7_039F
        mov    eax, dword ptr [ebp + 0xae]
loc_7_039F:
        add    eax, ecx
        mov    dword ptr [ebp + 0xae], eax
        mov    dword ptr [ebp + 0xf4], eax
        sub    eax, edx
        mov    cl, byte ptr [esi + 0x32]
        mov    byte ptr [ebp + 0x8c], cl
        shr    eax, cl
        cmp    eax, 0xff7
        sbb    cl, cl
        and    cl, 2
        or     byte ptr [ebp + 4], cl
        add    eax, 2
        add    eax, dword ptr [ebp + 0x78]
        mov    dword ptr [ebp + 0x7c], eax
        movzx  eax, word ptr [esi + 0x24]
        mov    dword ptr [ebp + 0x68], eax
        mov    eax, dword ptr [ebp + 0x54]
        dec    eax
        shr    eax, 0xe
        mov    dword ptr [ebp + 0x84], eax
        cmp    eax, 0x100
        jae    loc_7_04C0
        cmp    word ptr [ebp + 0xb4], 3
        jae    loc_7_040E
        cmp    eax, 0xff
        ja     loc_7_04C0
        cmp    byte ptr [esi + 0x2f], al
        ja     loc_7_044A
        jmp    loc_7_04C0
loc_7_040E:
        cmp    word ptr [esi + 0x2f], ax
        jbe    loc_7_04C0
        mov    edx, dword ptr [esi + 0x44]
        or     edx, edx
        je     loc_7_04C0
        mov    eax, dword ptr [ebp + 0x54]
        mov    cl, byte ptr [ebp + 0x8c]
        shr    eax, cl
        mov    ecx, edx
        mul    dword ptr [esi + 0x40]
        cmp    edx, ecx
        jae    loc_7_04C0
        mov    eax, dword ptr [esi + 0x40]
        mov    dword ptr [ebp + 0xf8], ecx
        mov    dword ptr [ebp + 0xf4], eax
loc_7_044A:
        movzx  eax, word ptr [esi + 0x29]
        movzx  edx, word ptr [ebp + 0xa4]
        add    eax, edx
        mov    dword ptr [ebp + 0x70], eax
        mov    al, byte ptr [esi + 0x15]
        mov    byte ptr [ebp + 0x8d], al
        or     dword ptr [ebp + 0x64], 0xffffffff
        or     dword ptr [ebp + 0x58], 0xffffffff
        mov    eax, dword ptr [ebp + 0x4c]
        add    eax, dword ptr [ebp + 0x74]
        movzx  edx, word ptr [ebp + 0xa4]
        sub    eax, edx
        mov    dword ptr [ebp + 0x60], eax
        mov    eax, dword ptr [ebp + 0x54]
        mul    dword ptr [0xe34]             ; -> loc_1_0E34
        mov    ecx, 0x64
        div    ecx
        mov    dword ptr [ebp + 0x5c], eax
        cmp    ebx, -1
        je     loc_7_04BF
        push   esi
        sub    esp, 0xc
        mov    esi, esp
        mov    dword ptr [esi], 1
        mov    eax, dword ptr [ebp + 0xbc]
        mov    dword ptr [esi + 8], eax
        mov    eax, 0xdf12
        VxDCall  IFSMgr.IFSMgr_Ring0_FileIO      ; cd2032004000
        lea    esp, [esp + 0xc]
        pop    esi
        mov    edx, 8
loc_7_04BF:
        ret    
loc_7_04C0:
        stc    
        mov    edx, 0xd
        jmp    loc_7_04BF
sub_7_04C8:
        xor    eax, eax
sub_7_04CA:
        push   ebx
        sub    esp, 0xc
        mov    dword ptr [esp + 8], eax
        xor    edx, edx
        mov    dword ptr [esp + 4], edx
        mov    dword ptr [esp], edx
        dec    eax
        js     loc_7_0505
        sub    eax, dword ptr [ebp + 0x60]
        jb     loc_7_0505
        add    eax, 0xf
        and    eax, 0xfffffff0
        cmp    dword ptr [ebp + 0x54], eax
        jbe    loc_7_05B0
        shld   edx, eax, 0x12
        and    eax, 0x3fff
        shr    eax, 3
        mov    dword ptr [esp + 4], edx
        mov    dword ptr [esp], eax
loc_7_0505:
        test   byte ptr [ebp + 5], 0x20
        je     loc_7_05B7
loc_7_050F:
        mov    ecx, dword ptr [ebp + 0x18]
        cmp    dword ptr [ebp + 0x58], -1
        je     loc_7_0521
        cmp    dword ptr [ebp + 0x58], ecx
        jb     loc_7_05B1
loc_7_0521:
        cmp    byte ptr [ebp + 0xf2], cl
        jbe    loc_7_059B
        mov    edx, dword ptr [ebp + 0x84]
        inc    edx
        mov    eax, dword ptr [ebp + 0x80]
loc_7_0536:
        cmp    dword ptr [esp + 4], eax
        ja     loc_7_0571
        push   ecx
        push   edx
        call   sub_7_101D
        pop    edx
        pop    ecx
        jb     loc_7_05B1
        mov    edi, dword ptr [ebp + 0xd8]
        mov    esi, dword ptr [ebp + 0x80]
        cmp    dword ptr [esp + 4], esi
        mov    esi, dword ptr [esp]
        je     loc_7_055E
        xor    esi, esi
loc_7_055E:
        add    esi, edi
        add    edi, 0x800
        push   ecx
        push   edx
        call   sub_7_079B
        pop    edx
        pop    ecx
        jae    loc_7_05B1
loc_7_0571:
        mov    eax, dword ptr [ebp + 0x80]
        cmp    eax, dword ptr [ebp + 0x84]
        cmc    
        sbb    edi, edi
        or     eax, edi
        inc    eax
        dec    edx
        jne    loc_7_0536
        cmp    byte ptr [ebp + 0xf2], cl
        jb     loc_7_059B
        cmp    dword ptr [esp + 8], 0
        jne    loc_7_059B
        mov    byte ptr [ebp + 0xf2], cl
loc_7_059B:
        cmp    ecx, 1
        je     loc_7_05B0
        test   byte ptr [ebp + 6], 1
        jne    loc_7_05B0
        cmp    word ptr [ebp + 0xb4], 3
        jae    loc_7_05C6
loc_7_05B0:
        stc    
loc_7_05B1:
        lea    esp, [esp + 0xc]
        pop    ebx
        ret    
loc_7_05B7:
        push   ebx
        call   sub_7_106E
        pop    ebx
        jae    loc_7_050F
        jmp    loc_7_05B0
loc_7_05C6:
        push   ecx
        mov    eax, dword ptr [ebp + 0x80]
        call   sub_7_101D
        pop    ecx
        jb     loc_7_05B1
        test   byte ptr [ebp + 5], 1
        jne    loc_7_05E9
        lea    ecx, [ecx*4 + 0x207]
        add    ecx, dword ptr [ebp + 0x1c]
        shr    ecx, 9
        dec    ecx
loc_7_05E9:
        inc    ecx
        cmp    dword ptr [ebp + 0x58], -1
        je     loc_7_05F5
        cmp    dword ptr [ebp + 0x58], ecx
        jb     loc_7_05B1
loc_7_05F5:
        lea    ebx, [ebp + 0xfc]
        and    dword ptr [ebx], 0
        mov    esi, dword ptr [ebp + 0xd8]
        call   sub_7_0A86
        or     ecx, ecx
        je     loc_7_0655
        mov    edx, dword ptr [ebp + 0x84]
        or     edx, edx
        je     loc_7_05B0
loc_7_0617:
        mov    eax, dword ptr [ebp + 0x80]
        or     eax, eax
        jne    loc_7_0628
        mov    eax, dword ptr [ebp + 0x84]
        inc    eax
loc_7_0628:
        dec    eax
        push   ecx
        push   edx
        call   sub_7_101D
        pop    edx
        pop    ecx
        jb     loc_7_05B1
        push   edx
        lea    ebx, [ebp + 0xfc]
        mov    esi, dword ptr [ebp + 0xd8]
        call   sub_7_0A86
        pop    edx
        jecxz  loc_7_0655
        dec    edx
        jne    loc_7_0617
        jmp    loc_7_05B0
loc_7_0655:
        mov    ecx, dword ptr [ebp + 0xfc]
        dec    ecx
        mov    edx, 1
loc_7_0661:
        mov    eax, dword ptr [ebp + edx*4 + 0x100]
        lea    esi, [edx - 1]
        rol    eax, 8
loc_7_066E:
        mov    ebx, dword ptr [ebp + esi*4 + 0x100]
        rol    ebx, 8
        cmp    al, bl
        jbe    loc_7_0689
        ror    ebx, 8
        mov    dword ptr [ebp + esi*4 + 0x104], ebx
        dec    esi
        jns    loc_7_066E
loc_7_0689:
        ror    eax, 8
        mov    dword ptr [ebp + esi*4 + 0x104], eax
        inc    edx
        loop   loc_7_0661
        mov    ecx, dword ptr [ebp + 0x18]
        test   byte ptr [ebp + 5], 1
        jne    loc_7_06AD
        lea    ecx, [ecx*4 + 0x207]
        add    ecx, dword ptr [ebp + 0x1c]
        shr    ecx, 9
        dec    ecx
loc_7_06AD:
        inc    ecx
        xor    edx, edx
loc_7_06B0:
        inc    edx
        sub    cl, byte ptr [ebp + edx*4 + 0xff]
        ja     loc_7_06B0
        je     loc_7_06F8
        cmp    dword ptr [ebp + 0xfc], edx
        jbe    loc_7_06F8
        add    cl, byte ptr [ebp + edx*4 + 0xff]
        mov    eax, edx
loc_7_06CD:
        cmp    byte ptr [ebp + eax*4 + 0x103], cl
        jb     loc_7_06F1
        je     loc_7_06E3
        inc    eax
        cmp    dword ptr [ebp + 0xfc], eax
        ja     loc_7_06CD
        jmp    loc_7_06F1
loc_7_06E3:
        mov    eax, dword ptr [ebp + eax*4 + 0x100]
        mov    dword ptr [ebp + edx*4 + 0xfc], eax
loc_7_06F1:
        sub    cl, byte ptr [ebp + edx*4 + 0xff]
loc_7_06F8:
        add    byte ptr [ebp + edx*4 + 0xff], cl
        mov    dword ptr [ebp + 0xfc], edx
        test   byte ptr [ebp + 5], 1
        jne    loc_7_0750
        lea    edx, [edx*4 + 4]
        mov    eax, dword ptr [ebp + 0x1c]
        lea    edx, [eax + edx + 0x1ff]
        mov    ecx, dword ptr [ebp + 0x18]
        lea    eax, [eax + ecx*4 + 0x207]
        shr    edx, 9
        shr    eax, 9
        sub    eax, edx
        jbe    loc_7_0750
        mov    ecx, dword ptr [ebp + 0xfc]
        cmp    byte ptr [ebp + ecx*4 + 0xff], 1
        jne    loc_7_0749
        dec    ecx
        mov    dword ptr [ebp + 0xfc], ecx
        jmp    loc_7_0750
loc_7_0749:
        dec    byte ptr [ebp + ecx*4 + 0xff]
loc_7_0750:
        mov    ecx, dword ptr [ebp + 0xfc]
        dec    ecx
        mov    edx, 1
loc_7_075C:
        mov    eax, dword ptr [ebp + edx*4 + 0x100]
        lea    esi, [edx - 1]
        rol    eax, 8
loc_7_0769:
        mov    ebx, dword ptr [ebp + esi*4 + 0x100]
        rol    ebx, 8
        cmp    eax, ebx
        jae    loc_7_0784
        ror    ebx, 8
        mov    dword ptr [ebp + esi*4 + 0x104], ebx
        dec    esi
        jns    loc_7_0769
loc_7_0784:
        ror    eax, 8
        mov    dword ptr [ebp + esi*4 + 0x104], eax
        inc    edx
        loop   loc_7_075C
        call   sub_7_0EFA
        jmp    loc_7_05B1
sub_7_079B:
        push   ebx
        call   sub_7_082F
        jb     loc_7_082D
        mov    edx, esi
        sub    edx, dword ptr [ebp + 0xd8]
        shl    edx, 3
        add    edx, eax
        mov    ebx, dword ptr [ebp + 0x80]
        cmp    dword ptr [ebp + 0x84], ebx
        jne    loc_7_07D7
        mov    ebx, dword ptr [ebp + 0x54]
        and    ebx, 0x3fff
        je     loc_7_07D7
        dec    ebx
        sub    ebx, edx
        jb     loc_7_082D
        inc    ebx
        cmp    ebx, ecx
        jb     loc_7_082D
loc_7_07D7:
        push   ecx
        push   edx
        call   sub_7_0A19
        cmp    word ptr [ebp + 0xb4], 3
        jae    loc_7_07F7
        cmp    dword ptr [ebp + 0x64], -1
        je     loc_7_07F7
        sub    dword ptr [ebp + 0x64], edi
        jae    loc_7_07F7
        and    dword ptr [ebp + 0x64], 0
loc_7_07F7:
        call   sub_7_0B80
        pop    edx
        pop    ecx
        cmp    dword ptr [ebp + 0x58], -1
        je     loc_7_0807
        sub    dword ptr [ebp + 0x58], ecx
loc_7_0807:
        mov    eax, dword ptr [ebp + 0x80]
        shl    eax, 0xe
        add    eax, edx
        add    eax, dword ptr [ebp + 0x60]
        inc    eax
        mov    dword ptr [ebp + 0xfc], 1
        mov    dword ptr [ebp + 0x100], eax
        mov    byte ptr [ebp + 0x103], cl
        clc    
loc_7_082D:
        pop    ebx
        ret    
sub_7_082F:
        cmp    ecx, 0x1f
        jb     sub_7_08A9
        sub    esp, 0xc
        mov    dword ptr [esp + 8], edi
        lea    ebx, [ecx - 0xf]
        shr    ebx, 4
        add    ebx, ebx
        mov    dword ptr [esp + 4], ebx
        add    ebx, esi
        lea    edi, [ebx + 4]
loc_7_084C:
        sub    ebx, 2
        cmp    word ptr [ebx], 0
        jne    loc_7_0863
        cmp    ebx, esi
        ja     loc_7_084C
        call   sub_7_08A9
        jae    loc_7_08A4
        lea    ebx, [edi - 4]
loc_7_0863:
        lea    esi, [ebx + 2]
        mov    ebx, dword ptr [esp + 4]
        add    ebx, esi
        cmp    dword ptr [esp + 8], ebx
        jbe    loc_7_08A3
        lea    edi, [ebx + 4]
loc_7_0875:
        sub    ebx, 2
        cmp    word ptr [ebx], 0
        jne    loc_7_0863
        cmp    ebx, esi
        ja     loc_7_0875
        sub    esi, 2
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [esp], edi
        sub    edi, eax
        sbb    edx, edx
        and    edi, edx
        add    edi, eax
        call   sub_7_08A9
        jae    loc_7_08A4
        mov    ebx, dword ptr [esp]
        sub    ebx, 4
        jmp    loc_7_0863
loc_7_08A3:
        stc    
loc_7_08A4:
        lea    esp, [esp + 0xc]
        ret    
sub_7_08A9:
        cmp    ecx, 0x11
        jb     loc_7_0912
        sub    esp, 8
        xor    eax, eax
        mov    ebx, eax
        sub    esi, 2
loc_7_08B8:
        add    esi, 2
        cmp    esi, edi
        jae    loc_7_090C
        mov    ax, word ptr [esi]
loc_7_08C2:
        mov    dword ptr [esp + 4], esi
        mov    bx, 0x10
        cmp    eax, 0
        je     loc_7_08D2
        bsf    ebx, eax
loc_7_08D2:
        or     ebx, ebx
        je     loc_7_08B8
        mov    dword ptr [esp], ebx
loc_7_08D9:
        add    esi, 2
        cmp    esi, edi
        jae    loc_7_090C
        mov    ax, word ptr [esi]
        or     eax, eax
        jne    loc_7_08F0
        add    ebx, 0x10
        cmp    ebx, ecx
        jb     loc_7_08D9
        jmp    loc_7_08FF
loc_7_08F0:
        bsr    dx, ax
        xor    dx, 0xf
        add    bx, dx
        cmp    ebx, ecx
        jb     loc_7_08C2
loc_7_08FF:
        mov    esi, dword ptr [esp + 4]
        mov    eax, 0x10
        sub    eax, dword ptr [esp]
        stc    
loc_7_090C:
        cmc    
        lea    esp, [esp + 8]
        ret    
loc_7_0912:
        sub    esp, 4
        mov    edx, 1
        ror    dx, cl
        mov    ebx, edi
        mov    dword ptr [esp], edx
        xchg   edi, ecx
        sub    ecx, esi
        cld    
        mov    eax, 0xffffffff
        shr    ecx, 2
        je     loc_7_093C
        xchg   esi, edi
        repe scasd eax, dword ptr es:[edi]
        xchg   esi, edi
        je     loc_7_093C
        sub    esi, 4
loc_7_093C:
        mov    ecx, edi
        inc    eax
loc_7_093F:
        cmp    ebx, esi
        jbe    loc_7_098C
        mov    ax, word ptr [esi]
        add    esi, 2
        cmp    ax, -1
        je     loc_7_093F
        mov    edx, dword ptr [esp]
loc_7_0952:
        mov    edi, edx
        test   edx, eax
        je     loc_7_0971
loc_7_0958:
        ror    dx, cl
        cmp    edx, edi
        jb     loc_7_0952
        cmp    ebx, esi
        jbe    loc_7_098C
        mov    ax, word ptr [esi]
        add    esi, 2
        cmp    ax, -1
        jne    loc_7_0952
        jmp    loc_7_093F
loc_7_0971:
        mov    edi, ecx
loc_7_0973:
        dec    edi
        je     loc_7_098F
        rol    dx, 1
        jb     loc_7_0983
loc_7_097B:
        test   edx, eax
        je     loc_7_0973
        mov    edi, edx
        jmp    loc_7_0958
loc_7_0983:
        sub    esi, 2
        mov    ax, word ptr [esi - 2]
        jmp    loc_7_097B
loc_7_098C:
        stc    
        jmp    loc_7_0998
loc_7_098F:
        bsf    eax, edx
        sub    esi, 2
        xor    eax, 0xf
loc_7_0998:
        mov    edi, ebx
        lea    esp, [esp + 4]
        ret    
sub_7_099F:
        xor    edi, edi
        mov    ebx, ecx
        sub    ecx, 0x10
        sbb    edx, edx
        and    ecx, edx
        add    ecx, 0x10
        mov    dx, 0x8000
        dec    ecx
        sar    dx, cl
        mov    ecx, eax
        shr    dx, cl
        mov    ax, word ptr [esi]
        cmp    eax, 1
        sbb    edi, 0
        not    edx
        and    ax, dx
        cmp    eax, 1
        adc    edi, 0
        mov    word ptr [esi], ax
        neg    ecx
        add    ecx, 0x10
        sub    ebx, ecx
        jbe    loc_7_0A18
        xor    eax, eax
loc_7_09DC:
        add    si, 2
        sub    ebx, 0x10
        jb     loc_7_09F2
        cmp    word ptr [esi], 1
        sbb    edi, 0
        inc    edi
        mov    word ptr [esi], ax
        jmp    loc_7_09DC
loc_7_09F2:
        add    ebx, 0x10
        je     loc_7_0A18
        mov    dx, 0x8000
        lea    ecx, [ebx - 1]
        sar    dx, cl
        mov    ax, word ptr [esi]
        cmp    eax, 1
        sbb    edi, 0
        not    edx
        and    ax, dx
        cmp    eax, 1
        adc    edi, 0
        mov    word ptr [esi], ax
loc_7_0A18:
        ret    
sub_7_0A19:
        xor    edi, edi
        mov    ebx, ecx
        sub    ecx, 0x10
        sbb    edx, edx
        and    ecx, edx
        add    ecx, 0x10
        mov    dx, 0x8000
        dec    ecx
        sar    dx, cl
        mov    ecx, eax
        shr    dx, cl
        mov    ax, word ptr [esi]
        cmp    eax, 1
        sbb    edi, 0
        or     ax, dx
        mov    word ptr [esi], ax
        neg    ecx
        add    ecx, 0x10
        sub    ebx, ecx
        jbe    loc_7_0A83
loc_7_0A4C:
        add    si, 2
        sub    ebx, 0x10
        jb     loc_7_0A65
        mov    ax, word ptr [esi]
        cmp    eax, 1
        sbb    edi, 0
        mov    word ptr [esi], 0xffff
        jmp    loc_7_0A4C
loc_7_0A65:
        add    ebx, 0x10
        je     loc_7_0A83
        mov    dx, 0x8000
        lea    ecx, [ebx - 1]
        sar    dx, cl
        mov    ax, word ptr [esi]
        cmp    eax, 1
        sbb    edi, 0
        or     ax, dx
        mov    word ptr [esi], ax
loc_7_0A83:
        neg    edi
        ret    
sub_7_0A86:
        sub    esp, 0x10
        mov    dword ptr [esp + 0xc], ebx
        lea    eax, [esi + 0x800]
        mov    edx, dword ptr [ebp + 0x80]
        cmp    dword ptr [ebp + 0x84], edx
        jne    loc_7_0AB5
        mov    edx, dword ptr [ebp + 0x54]
        and    edx, 0x3fff
        je     loc_7_0AB5
        add    edx, 0xf
        shr    edx, 4
        lea    eax, [esi + edx*2]
loc_7_0AB5:
        mov    dword ptr [esp + 8], eax
        mov    edx, ecx
        cld    
loc_7_0ABC:
        mov    ecx, dword ptr [esp + 8]
        sub    ecx, esi
        jbe    loc_7_0B75
        shr    ecx, 1
        mov    ax, 0xffff
        mov    edi, esi
        repe scasw ax, word ptr es:[edi]
        mov    esi, edi
        je     loc_7_0B75
        mov    ecx, 0x10
loc_7_0AE0:
        lea    eax, [esi - 2]
        sub    eax, dword ptr [ebp + 0xd8]
        shl    eax, 3
        mov    edi, dword ptr [ebp + 0x80]
        shl    edi, 0xe
        add    eax, edi
        mov    dword ptr [esp + 4], eax
        mov    ax, word ptr [esi - 2]
loc_7_0AFF:
        dec    ecx
        js     loc_7_0ABC
        bt     eax, ecx
        jb     loc_7_0AFF
        mov    dword ptr [esp], ecx
        xor    edi, edi
loc_7_0B0C:
        inc    edi
        dec    ecx
        js     loc_7_0B52
        bt     eax, ecx
        jae    loc_7_0B0C
loc_7_0B15:
        mov    ebx, dword ptr [esp]
        xor    ebx, 0xf
        add    ebx, dword ptr [esp + 4]
        lea    eax, [edi + ebx]
        sub    eax, dword ptr [ebp + 0x54]
        jb     loc_7_0B2D
        xor    ecx, ecx
        sub    edi, eax
        jbe    loc_7_0ABC
loc_7_0B2D:
        sub    edx, edi
        inc    ebx
        add    ebx, dword ptr [ebp + 0x60]
        shl    edi, 0x18
        or     edi, ebx
        mov    eax, dword ptr [esp + 0xc]
        inc    dword ptr [eax]
        mov    ebx, dword ptr [eax]
        lea    ebx, [eax + ebx*4]
        mov    dword ptr [ebx], edi
        or     edx, edx
        jle    loc_7_0B75
        or     ecx, ecx
        jne    loc_7_0AE0
        jmp    loc_7_0ABC
loc_7_0B52:
        cmp    dword ptr [esp + 8], esi
        jbe    loc_7_0B15
        mov    ax, word ptr [esi]
        add    esi, 2
        or     ax, ax
        jne    loc_7_0B6D
        add    edi, 0x10
        cmp    edi, 0x41
        jb     loc_7_0B52
        jmp    loc_7_0B15
loc_7_0B6D:
        dec    edi
        mov    ecx, 0x10
        jmp    loc_7_0B0C
loc_7_0B75:
        cmp    edx, 0x42
        sbb    ecx, ecx
        and    ecx, edx
        add    esp, 0x10
        ret    
sub_7_0B80:
        or     dword ptr [ebp + 4], 0x1000
        mov    edi, dword ptr [ebp + 0x2c]
        cmp    byte ptr [edi + 0x32], 0xff
        je     loc_7_0BA9
        mov    al, 0xf
        mov    ecx, dword ptr [ebp + 0x80]
        and    ecx, 1
        shl    ecx, 2
        shl    al, cl
        cmp    byte ptr [edi + 0x32], 0
        je     loc_7_0BAA
loc_7_0BA6:
        or     byte ptr [edi + 0x32], al
loc_7_0BA9:
        ret    
loc_7_0BAA:
        mov    ecx, eax
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf000100
        mov    dword ptr [edi + 0x24], eax
        mov    eax, dword ptr [0xd90]        ; -> loc_1_0D90
        mov    dword ptr [edi + 0x14], eax
        mov    dword ptr [edi + 0x18], 0xd7c ; -> loc_1_0D7C
        mov    dword ptr [eax + 0x18], edi
        mov    dword ptr [0xd90], edi        ; -> loc_1_0D90
        mov    eax, ecx
        jmp    loc_7_0BA6
sub_7_0BD1:
        or     edx, 0xffffffff
        jmp    loc_7_0BD8
sub_7_0BD6:
        xor    edx, edx
loc_7_0BD8:
        sub    esp, 4
        mov    dword ptr [esp], edx
        mov    ecx, eax
        and    eax, 0xffffff
        shr    ecx, 0x18
        sub    eax, dword ptr [ebp + 0x60]
        jbe    loc_7_0C30
        dec    eax
        js     loc_7_0C30
        lea    edx, [ecx + eax]
        cmp    dword ptr [ebp + 0x54], edx
        jb     loc_7_0C30
        push   eax
        push   ecx
        shr    eax, 0xe
        call   sub_7_101D
        pop    ecx
        pop    eax
        jb     loc_7_0C2B
        mov    esi, eax
        and    esi, 0x3fff
        mov    edx, esi
        shr    esi, 3
        and    esi, 0xfffffffe
        and    eax, 0xf
        add    esi, dword ptr [ebp + 0xd8]
        add    edx, ecx
        sub    edx, 0x4000
        ja     loc_7_0C5C
        xor    edx, edx
loc_7_0C2B:
        lea    esp, [esp + 4]
        ret    
loc_7_0C30:
        add    dword ptr [esp], 1
        jb     loc_7_0C2B
        mov    eax, dword ptr [ebp + 0xe0]
        mov    word ptr [eax], 0x14
        bts    dword ptr [ebp + 4], 0xf
        jb     loc_7_0C2B
        call   sub_7_43D3
        jb     loc_7_0C2B
        mov    esi, 4
        call   sub_7_4B5E
        stc    
        jmp    loc_7_0C2B
loc_7_0C5C:
        sub    ecx, edx
        mov    edi, dword ptr [ebp + 0x80]
        inc    edi
        shl    edi, 0xe
        inc    edi
        add    edi, dword ptr [ebp + 0x60]
        shl    edi, 8
        shld   edx, edi, 0x18
        clc    
        jmp    loc_7_0C2B
sub_7_0C76:
        test   byte ptr [ebp + 5], 0x80
        jne    loc_7_0CAF
loc_7_0C7C:
        call   sub_7_0BD6
        jb     loc_7_0CAF
        push   edx
        cmp    dword ptr [ebp + 0x58], -1
        je     loc_7_0C8D
        add    dword ptr [ebp + 0x58], ecx
loc_7_0C8D:
        call   sub_7_099F
        cmp    word ptr [ebp + 0xb4], 3
        jae    loc_7_0CA5
        cmp    dword ptr [ebp + 0x64], -1
        je     loc_7_0CA5
        add    dword ptr [ebp + 0x64], edi
loc_7_0CA5:
        call   sub_7_0B80
        pop    eax
        or     eax, eax
        jne    loc_7_0C7C
loc_7_0CAF:
        ret    
sub_7_0CB0:
        test   eax, 0x20000000
        jne    loc_7_0CD1
        shl    eax, 8
        mov    al, ch
        ror    eax, 8
        mov    dword ptr [ebp + 0x100], eax
        mov    dword ptr [ebp + 0xfc], 1
        clc    
loc_7_0CD0:
        ret    
loc_7_0CD1:
        or     ebx, ebx
        je     loc_7_0DBB
        test   eax, 0x40000000
        je     loc_7_0D5D
        push   dword ptr [ebp + 0xd0]
        push   dword ptr [ebp + 0xc8]
        push   dword ptr [ebp + 0xdc]
        lea    edx, [ebp + 0x224]
        mov    dword ptr [ebp + 0xd0], edx
        and    dword ptr [ebp + 0xc8], 0
        or     dword ptr [ebp + 0xdc], 2
        and    dword ptr [edx + 0x10], 0
        mov    dword ptr [edx], 1
        movzx  esi, byte ptr [ebp + 0x9b]
        dec    esi
        shl    esi, 9
        add    esi, ebx
        mov    dword ptr [edx + 4], esi
        movzx  ecx, ch
        dec    ecx
        mov    dword ptr [edx + 8], ecx
        mov    dword ptr [edx + 0xc], ebx
        push   eax
        push   ecx
        push   esi
        inc    ecx
        and    eax, 0xffffff
        xor    edi, edi
        call   sub_7_2E50
        pop    esi
        pop    ecx
        pop    eax
        pop    dword ptr [ebp + 0xdc]
        pop    dword ptr [ebp + 0xc8]
        pop    dword ptr [ebp + 0xd0]
        jb     loc_7_0CD0
        jmp    loc_7_0D78
loc_7_0D5D:
        push   eax
        push   ecx
        movzx  ecx, ch
        and    eax, 0xffffff
        mov    edi, ebx
        call   sub_7_2E50
        pop    ecx
        pop    eax
        jb     loc_7_0CD0
        mov    esi, ebx
loc_7_0D78:
        push   eax
        and    eax, 0xffffff
        call   sub_7_0DE5
        pop    eax
        jb     loc_7_0DB0
        bt     eax, 0x1e
        sbb    ch, 0
        cmp    byte ptr [ebp + 0x9b], ch
        jb     loc_7_0DB0
        or     ebx, ebx
        jne    loc_7_0CD0
loc_7_0D9D:
        pushfd 
        call   sub_7_489D
        and    dword ptr [0x482c], 0         ; -> obj7:0x482C
        popfd  
        jmp    loc_7_0CD0
loc_7_0DB0:
        cmp    ebx, 1
        jb     loc_7_0D9D
        stc    
        jmp    loc_7_0CD0
loc_7_0DBB:
        push   eax
        push   ebx
        push   ecx
        call   0x484c
        pop    ecx
        pop    ebx
        pop    eax
        jb     loc_7_0CD0
        push   eax
        push   ecx
        mov    ecx, 1
        and    eax, 0xffffff
        push   edi
        call   sub_7_2E50
        pop    esi
        pop    ecx
        pop    eax
        jb     loc_7_0D9D
        jmp    loc_7_0D78
sub_7_0DE5:
        lea    edi, [ebp + 0xfc]
        mov    edx, dword ptr [esi]
        add    esi, 4
        cmp    edx, 0x41
        ja     loc_7_0E8D
        dec    edx
        jle    loc_7_0E8D
        cmp    byte ptr [ebp + 0x9b], dl
        jb     loc_7_0E8C
        inc    edx
        mov    dword ptr [edi], edx
        add    edi, 4
        mov    edx, dword ptr [esi]
        add    esi, 4
        test   edx, 0x3800000
        jne    loc_7_0E8D
        rol    edx, 8
        shr    dl, 2
        inc    dl
        or     ch, ch
        je     loc_7_0E2F
        cmp    dl, ch
        jne    loc_7_0E8D
loc_7_0E2F:
        mov    ch, dl
        shr    edx, 8
        cmp    edx, eax
        jne    loc_7_0E8D
        shl    edx, 8
        mov    dl, ch
        ror    edx, 8
        mov    dword ptr [edi], edx
        add    edi, 4
        mov    edx, dword ptr [ebp + 0xfc]
        dec    edx
loc_7_0E4C:
        mov    eax, dword ptr [esi]
        add    esi, 4
        test   eax, 0x3800000
        jne    loc_7_0E8D
        rol    eax, 8
        shr    al, 2
        inc    al
        ror    eax, 8
        mov    dword ptr [edi], eax
        add    edi, 4
        rol    eax, 8
        cmp    byte ptr [ebp + 0x9b], al
        jb     loc_7_0E8C
        add    ch, al
        jb     loc_7_0E8C
        shr    eax, 8
        cmp    dword ptr [ebp + 0x60], eax
        ja     loc_7_0E8D
        dec    edx
        jne    loc_7_0E4C
        dec    ch
        cmp    byte ptr [ebp + 0x9b], ch
        inc    ch
loc_7_0E8C:
        ret    
loc_7_0E8D:
        stc    
        jmp    loc_7_0E8C
sub_7_0E90:
        test   byte ptr [ebp + 5], 0x80
        jne    loc_7_0EBF
        mov    byte ptr [ebp + 0xf2], 0xff
        xor    ebx, ebx
        call   sub_7_0CB0
        jb     loc_7_0EBF
        lea    ebx, [ebp + 0xfc]
        mov    ecx, dword ptr [ebx]
loc_7_0EAE:
        mov    eax, dword ptr [ebx + ecx*4]
        push   ebx
        push   ecx
        call   sub_7_0C76
        pop    ecx
        pop    ebx
        jb     loc_7_0EBF
        loop   loc_7_0EAE
        clc    
loc_7_0EBF:
        ret    
sub_7_0EC0:
        call   sub_7_0BD6
        jb     loc_7_0EF9
        push   edx
        cmp    dword ptr [ebp + 0x58], -1
        je     loc_7_0ED1
        sub    dword ptr [ebp + 0x58], ecx
loc_7_0ED1:
        call   sub_7_0A19
        cmp    word ptr [ebp + 0xb4], 3
        jae    loc_7_0EEF
        cmp    dword ptr [ebp + 0x64], -1
        je     loc_7_0EEF
        sub    dword ptr [ebp + 0x64], edi
        jae    loc_7_0EEF
        and    dword ptr [ebp + 0x64], 0
loc_7_0EEF:
        call   sub_7_0B80
        pop    eax
        or     eax, eax
        jne    sub_7_0EC0
loc_7_0EF9:
        ret    
sub_7_0EFA:
        push   ebx
        lea    ebx, [ebp + 0xfc]
        mov    ecx, dword ptr [ebx]
loc_7_0F03:
        mov    eax, dword ptr [ebx + ecx*4]
        push   ebx
        push   ecx
        call   sub_7_0EC0
        pop    ecx
        pop    ebx
        jb     loc_7_0F14
        loop   loc_7_0F03
        clc    
loc_7_0F14:
        pop    ebx
        ret    
sub_7_0F16:
        call   sub_7_0BD1
        jb     loc_7_0F2A
        push   edx
        call   sub_7_0F2B
        pop    eax
        jb     loc_7_0F2A
        or     eax, eax
        jne    sub_7_0F16
loc_7_0F2A:
        ret    
sub_7_0F2B:
        mov    edi, ecx
        sub    ecx, 0x10
        sbb    edx, edx
        and    ecx, edx
        add    ecx, 0x10
        mov    dx, 0x8000
        dec    ecx
        sar    dx, cl
        mov    ecx, eax
        shr    dx, cl
        test   word ptr [esi], dx
        jne    loc_7_0F79
        neg    ecx
        add    ecx, 0x10
        sub    edi, ecx
        jbe    loc_7_0F77
loc_7_0F52:
        add    si, 2
        sub    edi, 0x10
        jb     loc_7_0F63
        cmp    word ptr [esi], 0
        jne    loc_7_0F79
        jmp    loc_7_0F52
loc_7_0F63:
        add    edi, 0x10
        je     loc_7_0F77
        mov    dx, 0x8000
        lea    ecx, [edi - 1]
        sar    dx, cl
        test   word ptr [esi], dx
        jne    loc_7_0F79
loc_7_0F77:
        clc    
loc_7_0F78:
        ret    
loc_7_0F79:
        stc    
        jmp    loc_7_0F78
sub_7_0F7C:
        push   eax
        push   ecx
        shl    eax, 8
        mov    al, ch
        ror    eax, 8
        mov    dword ptr [ebp + 0x100], eax
        mov    dword ptr [ebp + 0xfc], 1
        call   sub_7_0F16
        pop    ecx
        pop    eax
        jb     loc_7_0FA6
        test   eax, 0x20000000
        jne    loc_7_0FA7
loc_7_0FA6:
        ret    
loc_7_0FA7:
        call   sub_7_0CB0
        jb     loc_7_0FA6
        push   ebx
        call   sub_7_0FB7
        pop    ebx
        jmp    loc_7_0FA6
sub_7_0FB7:
        lea    edx, [ebp + 0xfc]
        mov    ecx, dword ptr [edx]
loc_7_0FBF:
        mov    eax, dword ptr [edx + ecx*4]
        push   ecx
        push   edx
        call   sub_7_0F16
        pop    edx
        pop    ecx
        jb     loc_7_1007
        cmp    dword ptr [edx], ecx
        jbe    loc_7_1005
        push   ecx
        mov    eax, dword ptr [edx + ecx*4]
        xor    ebx, ebx
        shld   ebx, eax, 8
        and    eax, 0xffffff
        lea    ebx, [ebx + eax - 1]
loc_7_0FE4:
        inc    ecx
        mov    edi, dword ptr [edx + ecx*4]
        xor    esi, esi
        shld   esi, edi, 8
        and    edi, 0xffffff
        lea    esi, [esi + edi - 1]
        cmp    ebx, edi
        jb     loc_7_1000
        cmp    eax, esi
        jbe    loc_7_1008
loc_7_1000:
        cmp    dword ptr [edx], ecx
        ja     loc_7_0FE4
        pop    ecx
loc_7_1005:
        loop   loc_7_0FBF
loc_7_1007:
        ret    
loc_7_1008:
        pop    ecx
        stc    
        jmp    loc_7_1007
sub_7_100C:
        cmp    dword ptr [ebp + 0x58], -1
        jne    loc_7_101B
        push   ebx
        call   sub_7_106E
        pop    ebx
        jb     loc_7_101C
loc_7_101B:
        clc    
loc_7_101C:
        ret    
sub_7_101D:
        mov    edx, eax
        and    al, 0xfe
        mov    esi, dword ptr [ebp + 0x84]
        inc    esi
        mov    ecx, eax
        add    ecx, 2
        sub    ecx, esi
        sbb    edi, edi
        and    ecx, edi
        add    ecx, esi
        sub    ecx, eax
        shl    ecx, 2
        shl    eax, 2
        push   ebx
        push   edx
        lea    ebx, [ebp + 0x2c]
        call   sub_7_2C95
        pop    edx
        pop    ebx
        jb     loc_7_1061
        mov    dword ptr [ebp + 0x80], edx
        and    edx, 1
        shl    edx, 0xb
        add    eax, edx
        mov    dword ptr [ebp + 0xd8], eax
        clc    
loc_7_1060:
        ret    
loc_7_1061:
        mov    esi, 2
        call   sub_7_4B5E
        stc    
        jmp    loc_7_1060
sub_7_106E:
        mov    byte ptr [ebp + 0xf2], 0xff
        mov    ecx, dword ptr [ebp + 4]
        xor    ecx, 0x2000
        test   ecx, 0xb000
        setne  cl
        rol    ecx, 0xc
        and    ecx, 0x1000
        and    dword ptr [ebp + 4], 0xffff7fff
        mov    eax, dword ptr [ebp + 0x38]
        test   byte ptr [eax + 0x20], 2
        je     loc_7_1254
        sub    esp, 0x28
        mov    dword ptr [esp + 0x24], ecx
        or     eax, 0xffffffff
        mov    dword ptr [esp], eax
        mov    dword ptr [esp + 4], eax
        mov    dword ptr [esp + 8], eax
        mov    dword ptr [esp + 0xc], eax
        mov    dword ptr [esp + 0x10], eax
        mov    dword ptr [esp + 0x14], eax
        mov    dword ptr [esp + 0x18], eax
        mov    dword ptr [esp + 0x1c], eax
        mov    ecx, dword ptr [ebp + 0x54]
        shr    ecx, 4
        mov    dword ptr [ebp + 0x64], ecx
        mov    ecx, dword ptr [ebp + 0x54]
        mov    dword ptr [ebp + 0x58], ecx
        neg    ecx
        and    ecx, 0xf
        je     loc_7_1125
        mov    ebx, 1
        shl    ebx, cl
        dec    ebx
        mov    eax, dword ptr [ebp + 0x84]
        btr    dword ptr [esp], eax
        call   sub_7_101D
        jb     loc_7_126E
        mov    edi, eax
        mov    ecx, 0x200
        xor    eax, eax
        cld    
        rep stosd dword ptr es:[edi], eax
        mov    edx, dword ptr [ebp + 0x54]
        and    edx, 0x3fff
        shr    edx, 4
        or     word ptr [edi + edx*2 - 0x800], bx
        call   sub_7_0B80
loc_7_1125:
        mov    ebx, dword ptr [ebp + 0x78]
        add    ebx, 2
loc_7_112B:
        cmp    dword ptr [ebp + 0x7c], ebx
        jbe    loc_7_11FC
        mov    eax, ebx
        inc    ebx
        call   sub_7_2933
        jb     loc_7_126E
        test   eax, 0x80000000
        je     loc_7_112B
        push   ebx
        xor    ebx, ebx
        call   sub_7_0CB0
        pop    ebx
        jb     loc_7_125E
        mov    edx, dword ptr [ebp + 0xfc]
        mov    dword ptr [esp + 0x20], edx
loc_7_1162:
        lea    edi, [ebp + 0xfc]
        mov    edx, dword ptr [esp + 0x20]
        mov    eax, dword ptr [edi + edx*4]
loc_7_116F:
        call   sub_7_0BD6
        jb     loc_7_11D3
        sub    dword ptr [ebp + 0x58], ecx
        cmc    
        sbb    edi, edi
        and    dword ptr [ebp + 0x58], edi
        mov    edi, dword ptr [ebp + 0x80]
        btr    dword ptr [esp], edi
        jae    loc_7_119F
        mov    edi, dword ptr [ebp + 0xd8]
        push   eax
        push   ecx
        mov    ecx, 0x200
        xor    eax, eax
        cld    
        rep stosd dword ptr es:[edi], eax
        pop    ecx
        pop    eax
loc_7_119F:
        push   eax
        push   ecx
        push   edx
        push   esi
        call   sub_7_0F2B
        pop    esi
        pop    edx
        pop    ecx
        pop    eax
        jb     loc_7_11DE
loc_7_11AE:
        push   edx
        push   ebx
        call   sub_7_0A19
        pop    ebx
        cmp    word ptr [ebp + 0xb4], 3
        jae    loc_7_11C9
        sub    dword ptr [ebp + 0x64], edi
        jae    loc_7_11C9
        and    dword ptr [ebp + 0x64], 0
loc_7_11C9:
        call   sub_7_0B80
        pop    eax
        or     eax, eax
        jne    loc_7_116F
loc_7_11D3:
        dec    dword ptr [esp + 0x20]
        jne    loc_7_1162
        jmp    loc_7_112B
loc_7_11DE:
        bts    dword ptr [ebp + 4], 0xf
        jb     loc_7_11AE
        call   sub_7_43D3
        jb     loc_7_11AE
        push   esi
        push   edx
        mov    esi, 4
        call   sub_7_4B5E
        pop    edx
        pop    esi
        jmp    loc_7_11AE
loc_7_11FC:
        mov    edx, dword ptr [ebp + 0x84]
        inc    edx
loc_7_1203:
        dec    edx
        js     loc_7_122A
        btr    dword ptr [esp], edx
        jae    loc_7_1203
        mov    eax, edx
        push   edx
        call   sub_7_101D
        pop    edx
        jb     loc_7_126E
        mov    edi, eax
        mov    ecx, 0x200
        xor    eax, eax
        cld    
        rep stosd dword ptr es:[edi], eax
        call   sub_7_0B80
        jmp    loc_7_1203
loc_7_122A:
        mov    ecx, dword ptr [esp + 0x24]
        btr    dword ptr [ebp + 4], 0xc
        or     dword ptr [ebp + 4], ecx
        inc    edx
        mov    dword ptr [ebp + 0x80], edx
        test   byte ptr [ebp + 4], 4
        je     loc_7_1248
        call   sub_7_1CEA
loc_7_1248:
        lea    esp, [esp + 0x28]
loc_7_124C:
        pushfd 
        bts    dword ptr [ebp + 4], 0xd
        popfd  
        ret    
loc_7_1254:
        xor    eax, eax
        mov    dword ptr [ebp + 0x58], eax
        mov    dword ptr [ebp + 0x64], eax
        jmp    loc_7_124C
loc_7_125E:
        mov    eax, dword ptr [ebp + 0xe0]
        cmp    word ptr [eax], 0x10
        jb     loc_7_112B
loc_7_126E:
        bts    dword ptr [ebp + 4], 0xf
        xor    eax, eax
        stc    
        mov    dword ptr [ebp + 0x58], eax
        mov    dword ptr [ebp + 0x64], eax
        jmp    loc_7_1248
        db  000h, 000h
sub_7_1280:
        cmp    eax, 0xc0000000
        cmc    
        jb     loc_7_129C
        cmp    eax, 0x400000
        jae    loc_7_129C
        mov    ecx, ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        xchg   ebx, ecx
        add    eax, dword ptr [ecx + 4]
loc_7_129C:
        ret    
sub_7_129D:
        sub    esp, 4
        mov    dword ptr [esp], eax
        or     edx, 0x40000000
        lea    ecx, [ecx + eax - 1]
        shr    eax, 0xc
        shr    ecx, 0xc
        sub    ecx, eax
        jb     loc_7_12D4
        inc    ecx
        push   edx
        push   ecx
        push   eax
        VMMCall  VMM._LinPageLock                ; cd2063000100
        add    esp, 0xc
        cmp    eax, 1
        jb     loc_7_12D4
        mov    edx, dword ptr [esp]
        and    edx, 0xfff
        or     eax, edx
loc_7_12D4:
        lea    esp, [esp + 4]
        ret    
sub_7_12D9:
        lea    ecx, [ecx + eax - 1]
        shr    eax, 0xc
        shr    ecx, 0xc
        sub    ecx, eax
        jb     loc_7_12FB
        inc    ecx
        push   0x40000000
        push   ecx
        push   eax
        VMMCall  VMM._LinPageUnLock              ; cd2064000100
        add    esp, 0xc
        cmp    eax, 1
loc_7_12FB:
        ret    
sub_7_12FC:
        sub    esp, 0x10
        mov    dword ptr [esp], ecx
        mov    dword ptr [esp + 0xc], edx
        xor    edx, edx
        mov    dword ptr [esp + 8], edx
        mov    word ptr [esi + 3], 0xd
        mov    ecx, dword ptr [esp + 0xc]
        cmp    ecx, 0x10
        jb     loc_7_1395
        mov    word ptr [esi + 3], 2
        mov    eax, esi
        call   sub_7_129D
        jb     loc_7_1395
        mov    esi, eax
        movzx  ecx, word ptr [esi + 6]
        or     ecx, ecx
        je     loc_7_138A
        mov    dword ptr [esp + 4], ecx
        mov    word ptr [esi + 3], 0xd
        shl    ecx, 4
        add    ecx, 0x10
        cmp    dword ptr [esp + 0xc], ecx
        jb     loc_7_139A
        mov    word ptr [esi + 3], 2
        lea    edi, [esi + 0x10]
loc_7_1353:
        mov    eax, dword ptr [edi]
        or     eax, eax
        je     loc_7_1373
        call   sub_7_1280
        jb     loc_7_139A
        movzx  ecx, byte ptr [edi + 0xa]
        shl    ecx, 9
        je     loc_7_13AA
        mov    edx, dword ptr [esp]
        call   sub_7_129D
        jb     loc_7_139A
loc_7_1373:
        mov    dword ptr [edi + 0xc], eax
        inc    dword ptr [esp + 8]
        add    edi, 0x10
        dec    dword ptr [esp + 4]
        jne    loc_7_1353
        mov    eax, dword ptr [esp + 8]
        mov    dword ptr [esi + 0xc], eax
loc_7_138A:
        mov    word ptr [esi + 3], 0x4f4b
        or     byte ptr [esi + 5], 1
        clc    
loc_7_1395:
        lea    esp, [esp + 0x10]
        ret    
loc_7_139A:
        mov    ecx, dword ptr [esp + 8]
        mov    edx, dword ptr [esp + 0xc]
        call   sub_7_13B5
        stc    
        jmp    loc_7_1395
loc_7_13AA:
        mov    word ptr [esi + 3], 2
        jmp    loc_7_139A
sub_7_13B2:
        mov    ecx, dword ptr [esi + 0xc]
sub_7_13B5:
        sub    esp, 8
        mov    dword ptr [esp + 4], edx
        jecxz  loc_7_13DF
        mov    dword ptr [esp], ecx
        lea    edi, [esi + 0x10]
loc_7_13C4:
        mov    eax, dword ptr [edi + 0xc]
        or     eax, eax
        je     loc_7_13D7
        movzx  ecx, byte ptr [edi + 0xa]
        shl    ecx, 9
        call   sub_7_12D9
loc_7_13D7:
        add    edi, 0x10
        dec    dword ptr [esp]
        jne    loc_7_13C4
loc_7_13DF:
        and    byte ptr [esi + 5], 0xfe
        mov    eax, esi
        mov    ecx, dword ptr [esp + 4]
        call   sub_7_12D9
        add    esp, 8
        ret    
sub_7_13F2:
        sub    esp, 0xc
        mov    dword ptr [esp + 4], esi
        mov    dword ptr [ebp + 8], 0
        and    dword ptr [ebp + 4], 0xffffffbf
        movzx  ecx, word ptr [esi + 6]
        or     ecx, ecx
        je     loc_7_14BC
        mov    dword ptr [esp + 8], ecx
        lea    edi, [esi + 0x10]
        mov    dword ptr [esp], edi
loc_7_141A:
        mov    edi, dword ptr [esp]
        movzx  eax, word ptr [edi + 8]
        or     eax, eax
        je     loc_7_1516
        cmp    eax, 2
        jb     loc_7_1537
        add    eax, dword ptr [ebp + 0x78]
        cmp    dword ptr [ebp + 0x7c], eax
        jbe    loc_7_1537
        test   byte ptr [ebp + 5], 0x18
        jne    loc_7_1558
        call   sub_7_2933
        jb     loc_7_153B
        test   eax, 0x80000000
        je     loc_7_153F
        mov    dword ptr [ebp + 0xc], eax
        mov    word ptr [ebp + 0x10], cx
        test   eax, 0x20000000
        jne    loc_7_14C1
        mov    edi, dword ptr [esp]
        mov    eax, edi
        mov    edi, dword ptr [edi + 0xc]
        or     edi, edi
        je     loc_7_1490
        cmp    byte ptr [eax + 0xa], ch
        jne    loc_7_1554
        call   sub_7_2AA1
        jb     loc_7_153B
        and    dword ptr [ebp + 4], 0xffffffbf
loc_7_1490:
        mov    edi, dword ptr [esp]
        mov    eax, dword ptr [ebp + 0x18]
        mov    byte ptr [edi + 0xa], al
        mov    eax, dword ptr [ebp + 0xc]
        mov    edx, eax
        shr    eax, 0x1c
        mov    byte ptr [edi + 0xb], al
        and    edx, 0xffffff
        mov    dword ptr [edi + 4], edx
loc_7_14AD:
        add    dword ptr [esp], 0x10
        dec    dword ptr [esp + 8]
        jne    loc_7_141A
        clc    
loc_7_14BC:
        lea    esp, [esp + 0xc]
        ret    
loc_7_14C1:
        call   0x484c
        jb     loc_7_153B
        push   edi
        call   sub_7_2AA1
        pop    esi
        jb     loc_7_1500
        and    dword ptr [ebp + 4], 0xffffffbf
        add    esi, dword ptr [ebp + 0x20]
        mov    edi, dword ptr [esp]
        cmp    dword ptr [edi + 0xc], 0
        je     loc_7_14F5
        mov    ecx, dword ptr [ebp + 0x18]
        cmp    byte ptr [edi + 0xa], cl
        jne    loc_7_150B
        mov    edi, dword ptr [edi + 0xc]
        shl    ecx, 9
        sub    ecx, dword ptr [ebp + 0x20]
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_7_14F5:
        call   sub_7_489D
        and    dword ptr [ebp + 0x20], 0
        jmp    loc_7_1490
loc_7_1500:
        call   sub_7_489D
        and    dword ptr [ebp + 0x20], 0
        jmp    loc_7_153B
loc_7_150B:
        call   sub_7_489D
        and    dword ptr [ebp + 0x20], 0
        jmp    loc_7_1554
loc_7_1516:
        mov    eax, dword ptr [edi + 4]
        movzx  ecx, byte ptr [edi + 0xa]
        mov    edi, dword ptr [edi + 0xc]
        or     edi, edi
        je     loc_7_14AD
        cmp    eax, dword ptr [ebp + 0x60]
        sbb    eax, 0
        call   sub_7_2E50
        jae    loc_7_14AD
        jmp    loc_7_153B
loc_7_1537:
        mov    al, 4
        jmp    loc_7_1562
loc_7_153B:
        mov    al, 3
        jmp    loc_7_1562
loc_7_153F:
        mov    edi, dword ptr [esp]
        and    dword ptr [edi + 4], 0
        cmp    dword ptr [edi + 0xc], 0
        je     loc_7_14AD
        mov    al, 5
        jmp    loc_7_1562
loc_7_1554:
        mov    al, 0xc
        jmp    loc_7_1562
loc_7_1558:
        mov    al, 1
        test   byte ptr [ebp + 5], 0x10
        jne    loc_7_1562
        mov    al, 0xe
loc_7_1562:
        movzx  eax, al
        mov    esi, dword ptr [esp + 4]
        mov    word ptr [esi + 3], ax
        and    dword ptr [ebp + 4], 0xffffe7ff
        stc    
        jmp    loc_7_14BC
sub_7_157A:
        sub    esp, 0xc
        test   byte ptr [ebp + 5], 0x18
        jne    loc_7_17B1
        and    dword ptr [ebp + 8], 0
        and    dword ptr [ebp + 4], 0xffffffbf
        test   byte ptr [ebp + 5], 0x20
        je     loc_7_1756
        movzx  ecx, word ptr [esi + 6]
        or     ecx, ecx
        je     loc_7_1744
        mov    dword ptr [esp + 8], ecx
        mov    dword ptr [esp + 4], esi
        lea    edi, [esi + 0x10]
        mov    dword ptr [esp], edi
loc_7_15B3:
        mov    edi, dword ptr [esp]
        movzx  eax, word ptr [edi + 8]
        cmp    eax, 2
        jb     loc_7_17A5
        add    eax, dword ptr [ebp + 0x78]
        cmp    dword ptr [ebp + 0x7c], eax
        jbe    loc_7_17A5
        movzx  ecx, byte ptr [edi + 0xa]
        mov    eax, dword ptr [edi + 4]
        or     ecx, ecx
        je     loc_7_17AD
        test   byte ptr [edi + 0xb], 2
        jne    loc_7_180F
        cmp    byte ptr [ebp + 0x9b], cl
        jb     loc_7_17AD
        mov    edx, eax
        or     edx, edx
        js     loc_7_1763
        rol    eax, 8
        or     al, al
        jne    loc_7_17A9
        mov    al, cl
        ror    eax, 8
        mov    dword ptr [ebp + 0x100], eax
        mov    dword ptr [ebp + 0xfc], 1
        sub    edx, dword ptr [ebp + 0x60]
        jbe    loc_7_17A9
        dec    edx
        and    edx, 0x3fff
        add    edx, ecx
        cmp    edx, 0x4000
        ja     loc_7_17BD
        call   sub_7_0F16
        jb     loc_7_1793
loc_7_1647:
        call   sub_7_0EFA
        jb     loc_7_17A1
loc_7_1652:
        add    dword ptr [esp], 0x10
        dec    dword ptr [esp + 8]
        jne    loc_7_15B3
        mov    ecx, dword ptr [esp + 4]
        mov    edi, ecx
        movzx  ecx, word ptr [ecx + 6]
        mov    dword ptr [esp + 8], ecx
        add    edi, 0x10
        mov    dword ptr [esp], edi
loc_7_1674:
        mov    edi, dword ptr [esp]
        test   byte ptr [edi + 0xb], 2
        jne    loc_7_1885
        mov    eax, dword ptr [edi + 4]
        rol    eax, 8
        mov    al, byte ptr [edi + 0xa]
        ror    eax, 8
        mov    edi, dword ptr [edi + 0xc]
        or     edi, edi
        je     loc_7_17D6
        mov    dword ptr [ebp + 0x100], eax
        mov    dword ptr [ebp + 0xfc], 1
        call   sub_7_259A
        jb     loc_7_17DA
loc_7_16B3:
        mov    edi, dword ptr [esp]
        movzx  eax, word ptr [edi + 8]
        add    eax, dword ptr [ebp + 0x78]
        call   sub_7_2933
        jb     loc_7_17DA
        mov    dword ptr [ebp + 0xc], eax
        mov    word ptr [ebp + 0x10], cx
        test   eax, 0xffffff
        je     loc_7_17DE
        mov    edi, dword ptr [esp]
        mov    al, byte ptr [edi + 0xb]
        shl    eax, 0x1c
        or     eax, dword ptr [edi + 4]
        test   eax, 0x20000000
        jne    loc_7_1749
        mov    ch, byte ptr [edi + 0xa]
        test   eax, 0x40000000
        je     loc_7_16F9
        mov    cl, ch
loc_7_16F9:
        movzx  edx, word ptr [edi + 8]
        add    edx, dword ptr [ebp + 0x78]
        call   sub_7_29FE
        jb     loc_7_17DA
        mov    eax, dword ptr [ebp + 0xc]
        test   eax, 0x80000000
        je     loc_7_1724
        mov    cx, word ptr [ebp + 0x10]
        call   sub_7_0E90
        jb     loc_7_17DA
loc_7_1724:
        add    dword ptr [esp], 0x10
        dec    dword ptr [esp + 8]
        jne    loc_7_1674
        call   sub_7_1CEA
        jb     loc_7_17DA
        and    dword ptr [ebp + 4], 0xffffefff
loc_7_1744:
        lea    esp, [esp + 0xc]
        ret    
loc_7_1749:
        mov    esi, dword ptr [edi + 0xc]
        mov    ch, byte ptr [esi + 7]
        shr    ch, 2
        inc    ch
        jmp    loc_7_16F9
loc_7_1756:
        push   esi
        call   sub_7_106E
        pop    esi
        jae    loc_7_17B1
        mov    al, 3
        jmp    loc_7_17CE
loc_7_1763:
        btr    eax, 0x1f
        mov    dword ptr [ebp + 0x18], ecx
        bts    dword ptr [ebp + 4], 0x10
        call   sub_7_04CA
        mov    edi, dword ptr [esp]
        pushfd 
        btr    dword ptr [ebp + 4], 0x10
        popfd  
        jb     loc_7_1793
        mov    eax, dword ptr [ebp + 0x100]
        and    eax, 0xffffff
        mov    dword ptr [edi + 4], eax
        jmp    loc_7_1652
loc_7_1793:
        mov    eax, dword ptr [ebp + 0xe0]
        cmp    word ptr [eax], 0x10
        mov    al, 9
        jb     loc_7_17BF
loc_7_17A1:
        mov    al, 3
        jmp    loc_7_17BF
loc_7_17A5:
        mov    al, 4
        jmp    loc_7_17BF
loc_7_17A9:
        mov    al, 6
        jmp    loc_7_17BF
loc_7_17AD:
        mov    al, 7
        jmp    loc_7_17BF
loc_7_17B1:
        mov    al, 0xe
        test   byte ptr [ebp + 5], 0x10
        je     loc_7_17CE
        mov    al, 1
        jmp    loc_7_17CE
loc_7_17BD:
        mov    al, 0xa
loc_7_17BF:
        mov    esi, dword ptr [esp + 4]
        and    dword ptr [ebp + 4], 0xffffdfff
        or     dword ptr [ebp + 0x58], 0xffffffff
loc_7_17CE:
        mov    word ptr [esi + 6], 0
        jmp    loc_7_17F7
loc_7_17D6:
        mov    al, 0xb
        jmp    loc_7_17E0
loc_7_17DA:
        mov    al, 3
        jmp    loc_7_17E0
loc_7_17DE:
        mov    al, 8
loc_7_17E0:
        mov    esi, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp + 8]
        sub    word ptr [esi + 6], cx
        and    dword ptr [ebp + 4], 0xffffdfff
        or     dword ptr [ebp + 0x58], 0xffffffff
loc_7_17F7:
        movzx  eax, al
        mov    word ptr [esi + 3], ax
        and    dword ptr [ebp + 4], 0xffffe7ff
        stc    
        jmp    loc_7_1744
loc_7_180B:
        mov    al, 0xf
        jmp    loc_7_17BF
loc_7_180F:
        cmp    word ptr [ebp + 0xb4], 3
        jb     loc_7_180B
        xor    ecx, ecx
        push   edi
        mov    esi, dword ptr [edi + 0xc]
        call   sub_7_0DE5
        pop    edi
        jb     loc_7_180B
        cmp    byte ptr [edi + 0xa], ch
        jne    loc_7_180B
        bt     dword ptr [edi + 0xb], 2
        sbb    ch, 0
        cmp    byte ptr [ebp + 0x9b], ch
        jb     loc_7_180B
        mov    ecx, dword ptr [ebp + 0xfc]
        lea    esi, [ebp + 0x100]
loc_7_1848:
        mov    edx, dword ptr [esi]
        add    esi, 4
        xor    eax, eax
        shld   eax, edx, 8
        and    edx, 0xffffff
        sub    edx, dword ptr [ebp + 0x60]
        dec    edx
        and    edx, 0x3fff
        add    edx, eax
        cmp    edx, 0x4000
        ja     loc_7_17BD
        loop   loc_7_1848
        push   edi
        call   sub_7_0FB7
        pop    edi
        jae    loc_7_1647
        jmp    loc_7_1793
loc_7_1885:
        mov    esi, dword ptr [edi + 0xc]
        mov    edi, esi
        mov    ebx, dword ptr [esi]
        add    esi, 4
loc_7_188F:
        mov    eax, dword ptr [esi]
        add    esi, 4
        xor    ecx, ecx
        shld   ecx, eax, 6
        inc    ecx
        and    eax, 0x7fffff
        push   esi
        call   sub_7_2E49
        pop    esi
        jb     loc_7_17DA
        dec    ebx
        jne    loc_7_188F
        jmp    loc_7_16B3
        db  000h, 000h, 000h
sub_7_18B8:
        sub    esp, 8
        cmp    dword ptr [ebp + 8], edx
        jne    loc_7_18CC
        call   sub_7_19E9
        cmp    edx, edx
        jmp    loc_7_19E4
loc_7_18CC:
        test   byte ptr [ebp + 4], 0x80
        je     loc_7_18E5
        mov    dword ptr [esp + 4], edx
        call   sub_7_1B8C
        mov    edx, dword ptr [esp + 4]
        jb     loc_7_19E4
loc_7_18E5:
        movzx  ecx, byte ptr [ebp + 0x9b]
        shr    ecx, 3
        mov    edi, edx
        shl    edi, 8
        lea    edi, [ecx + edi + 0x10000000]
        mov    ebx, ebp
loc_7_18FD:
        mov    ah, byte ptr [0xe68]          ; -> obj1:0x0E68
        xor    al, al
        dec    edi
        VxDCall  Dev_048B.Service_0004h          ; cd2004008b04
        jb     loc_7_192F
        mov    dword ptr [ebp + ecx*4 + 0x200], esi
        loop   loc_7_18FD
        mov    dword ptr [ebp + 8], edx
        or     dword ptr [ebp + 4], 0x40
        call   sub_7_19E9
        call   sub_7_1A80
        cmp    eax, eax
        jmp    loc_7_19E4
loc_7_192F:
        and    dword ptr [ebp + 4], 0xffffffbf
        mov    dword ptr [esp + 4], edx
        mov    eax, 0xcf0                    ; -> loc_1_0CF0
        call   sub_7_490C
        jb     loc_7_19E4
        movzx  ecx, byte ptr [ebp + 0x9b]
        shr    ecx, 3
        mov    dword ptr [esp], ecx
loc_7_1954:
        mov    edi, dword ptr [esp + 4]
        shl    edi, 8
        add    edi, dword ptr [esp]
        or     edi, 0x10000000
        dec    edi
        mov    ebx, ebp
loc_7_1967:
        cmp    dword ptr [esp], 0
        je     loc_7_19B6
        mov    ah, byte ptr [0xe68]          ; -> obj1:0x0E68
        mov    al, 0x17
        VxDCall  Dev_048B.Service_0004h          ; cd2004008b04
        or     esi, esi
        je     loc_7_198F
        dec    dword ptr [esp]
        dec    edi
        mov    ecx, dword ptr [esp]
        mov    dword ptr [ebp + ecx*4 + 0x204], esi
        jmp    loc_7_1967
loc_7_198F:
        call   sub_7_1B06
        call   sub_7_1CEA
        jb     loc_7_19A9
        jne    loc_7_1954
        mov    eax, 0xd0c                    ; -> loc_1_0D0C
        call   sub_7_4974
        jmp    loc_7_1954
loc_7_19A9:
        mov    eax, 0xcf0                    ; -> loc_1_0CF0
        call   sub_7_493D
        stc    
        jmp    loc_7_19E4
loc_7_19B6:
        and    dword ptr [ebp + 4], 0xfffff87f
        or     dword ptr [ebp + 4], 0x20
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [ebp + 8], eax
        xor    eax, eax
        mov    dword ptr [ebp + 0x20], eax
        mov    dword ptr [ebp + 0xc], eax
        mov    dword ptr [ebp + 0x10], eax
        call   sub_7_1A80
        mov    eax, 0xcf0                    ; -> loc_1_0CF0
        call   sub_7_493D
        or     esp, esp
loc_7_19E4:
        lea    esp, [esp + 8]
        ret    
sub_7_19E9:
        lea    edx, [ebp + 0x204]
        movzx  ecx, byte ptr [ebp + 0x9b]
        shr    ecx, 3
loc_7_19F9:
        mov    esi, dword ptr [edx + ecx*4 - 4]
        VxDCall  Dev_048B.Service_0007h          ; cd2007008b04
        loop   loc_7_19F9
        mov    ecx, dword ptr [esi + 0x1c]
        mov    edx, dword ptr [esi + 0x20]
        mov    dword ptr [ebp + 0xc], ecx
        mov    dword ptr [ebp + 0x10], edx
        test   byte ptr [ebp + 4], 0x80
        jne    loc_7_1A1A
        mov    byte ptr [ebp + 0x14], dl
loc_7_1A1A:
        mov    ecx, dword ptr [esi + 0x18]
        and    ecx, 0x780
        mov    edx, dword ptr [ebp + 4]
        and    edx, 0xfffff87f
        lea    edx, [ecx + edx + 0x20]
        mov    dword ptr [ebp + 4], edx
        movzx  ecx, byte ptr [esi + 0x24]
        mov    dword ptr [ebp + 0x20], ecx
        ret    
sub_7_1A3B:
        lea    edx, [ebp + 0x204]
        movzx  ecx, byte ptr [ebp + 0x9b]
        shr    ecx, 3
        bt     dword ptr [ebp + 4], 7
        sbb    eax, eax
loc_7_1A52:
        mov    esi, dword ptr [edx + ecx*4 - 4]
        mov    byte ptr [esi + 0x32], al
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
        loop   loc_7_1A52
        mov    ecx, dword ptr [ebp + 0xc]
        mov    edx, dword ptr [ebp + 0x10]
        mov    dword ptr [esi + 0x1c], ecx
        mov    dword ptr [esi + 0x20], edx
        xor    ecx, ecx
        xchg   dword ptr [ebp + 0x20], ecx
        mov    byte ptr [esi + 0x24], cl
        mov    eax, dword ptr [ebp + 4]
        mov    dword ptr [esi + 0x18], eax
        and    dword ptr [ebp + 4], 0xffffffdf
        ret    
sub_7_1A80:
        lea    ebx, [ebp + 0x204]
        movzx  ecx, byte ptr [ebp + 0x9b]
        shr    ecx, 3
        mov    edi, dword ptr [ebp + 0x24]
        call   sub_7_1A99
        ret    
sub_7_1A99:
        sub    esp, 8
        cmp    ecx, 1
        jb     loc_7_1B01
        mov    dword ptr [esp + 4], ecx
loc_7_1AA5:
        mov    eax, dword ptr [ebx + ecx*4 - 4]
        mov    edx, dword ptr [eax + 0x10]
        shr    edx, 0xc
        lea    eax, [eax + 0x14]
        mov    dword ptr [esp], ecx
        push   0
        push   eax
        push   1
        push   edx
        VMMCall  VMM._CopyPageTable              ; cd2061000100
        add    esp, 0x10
        cmp    eax, 1
        jb     loc_7_1B01
        mov    ecx, dword ptr [esp]
        dec    ecx
        jne    loc_7_1AA5
        mov    ecx, dword ptr [esp + 4]
        shr    edi, 0xc
        add    edi, ecx
loc_7_1AD8:
        dec    edi
        mov    eax, dword ptr [ebx + ecx*4 - 4]
        mov    eax, dword ptr [eax + 0x14]
        shr    eax, 0xc
        mov    dword ptr [esp], ecx
        push   0x20080
        push   eax
        push   1
        push   edi
        VMMCall  VMM._PageCommitPhys             ; cd2028010100
        add    esp, 0x10
        mov    ecx, dword ptr [esp]
        loop   loc_7_1AD8
        shl    edi, 0xc
        clc    
loc_7_1B01:
        lea    esp, [esp + 8]
        ret    
sub_7_1B06:
        push   edx
        xor    ecx, ecx
        mov    edx, ecx
        mov    eax, 0xe44                    ; -> obj1:0x0E44
loc_7_1B10:
        mov    eax, dword ptr [eax]
        or     eax, eax
        je     loc_7_1B4D
loc_7_1B16:
        test   byte ptr [eax + 4], 0x80
        je     loc_7_1B10
        mov    dword ptr [eax + 0x28], edx
        or     ecx, eax
        mov    eax, dword ptr [eax]
        or     eax, eax
        jne    loc_7_1B16
        jecxz  loc_7_1B4D
        bts    dword ptr [0xcfc], 0          ; -> obj1:0x0CFC
        jb     loc_7_1B16
        mov    esi, 0x33c                    ; -> sub_1_033C
        push   eax
        push   ebx
        push   ecx
        xor    eax, eax
        mov    ebx, eax
        mov    ecx, 0x408
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
        pop    ecx
        pop    ebx
        pop    eax
loc_7_1B4D:
        pop    edx
        ret    
sub_7_1B4F:
        cmp    dword ptr [ebp + 8], 0
        je     loc_7_1B85
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf000100
        mov    dword ptr [ebp + 0x28], eax
        mov    eax, dword ptr [ebp + 0xe0]
        cmp    word ptr [eax], 0x10
        jae    loc_7_1B79
        cmp    dword ptr [0xd0c], 0          ; -> loc_1_0D0C
        jne    loc_7_1B85
        call   sub_7_1A3B
loc_7_1B78:
        ret    
loc_7_1B79:
        xor    eax, eax
        mov    dword ptr [ebp + 8], eax
        and    dword ptr [ebp + 4], 0xffffff7f
loc_7_1B85:
        call   sub_7_1B8C
        jmp    loc_7_1B78
sub_7_1B8C:
        test   byte ptr [ebp + 4], 0xa0
        je     loc_7_1BCA
        cmp    dword ptr [ebp + 8], 0
        je     loc_7_1BD6
        test   byte ptr [ebp + 4], 0x80
        je     loc_7_1BC5
        test   byte ptr [ebp + 4], 0x20
        jne    loc_7_1BA9
        call   sub_7_19E9
loc_7_1BA9:
        mov    eax, dword ptr [ebp + 0xe0]
        push   dword ptr [eax]
        mov    esi, dword ptr [ebp + 0x24]
        call   sub_7_238A
        pop    edx
        jb     loc_7_1BD6
        mov    eax, dword ptr [ebp + 0xe0]
        mov    word ptr [eax], dx
loc_7_1BC5:
        call   sub_7_1A3B
loc_7_1BCA:
        mov    eax, 0xd0c                    ; -> loc_1_0D0C
        call   sub_7_4996
        clc    
loc_7_1BD5:
        ret    
loc_7_1BD6:
        pushfd 
        call   sub_7_1A3B
        movzx  ecx, byte ptr [ebp + 0x9b]
        shr    ecx, 3
loc_7_1BE6:
        mov    esi, 0xffc00000
        xchg   dword ptr [ebp + ecx*4 + 0x200], esi
        push   ecx
        VxDCall  Dev_048B.Service_0005h          ; cd2005008b04
        pop    ecx
        loop   loc_7_1BE6
        mov    eax, 0xd0c                    ; -> loc_1_0D0C
        call   sub_7_4996
        popfd  
        jmp    loc_7_1BD5
sub_7_1C09:
        mov    edx, dword ptr [esi + 0xc]
        and    edx, 0xfffff00
        shr    edx, 8
        mov    ecx, dword ptr [esi + 8]
        cmp    dword ptr [ecx + 8], edx
        je     loc_7_1C1E
loc_7_1C1D:
        ret    
loc_7_1C1E:
        xor    dword ptr [ecx + 8], edx
        and    dword ptr [ecx + 4], 0xffffffbf
        jmp    loc_7_1C1D
sub_7_1C27:
        mov    esi, dword ptr [0xd94]        ; -> loc_1_0D94
loc_7_1C2D:
        cmp    esi, 0xd7c                    ; -> loc_1_0D7C
        je     loc_7_1C5D
        mov    eax, dword ptr [esi + 0x14]
        mov    edx, dword ptr [esi + 0x18]
        mov    dword ptr [eax + 0x18], edx
        mov    dword ptr [edx + 0x14], eax
        mov    edx, dword ptr [esi + 8]
        mov    eax, esi
        xchg   dword ptr [edx + 0xe4], eax
        mov    dword ptr [esi + 0x14], eax
        mov    eax, dword ptr [esi + 0x18]
        mov    dword ptr [esi + 0x18], 0xffffffcb
        mov    esi, eax
        jmp    loc_7_1C2D
loc_7_1C5D:
        bts    dword ptr [0xcfc], 1          ; -> obj1:0x0CFC
        jb     loc_7_1C83
        xor    edx, edx
        mov    esi, 0x3ed                    ; -> sub_1_03ED
        push   eax
        push   ebx
        push   ecx
        xor    eax, eax
        mov    ebx, eax
        mov    ecx, 0x408
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
        pop    ecx
        pop    ebx
        pop    eax
loc_7_1C83:
        ret    
sub_7_1C84:
        cmp    dword ptr [ebp + 0xe4], 0
        jne    loc_7_1C8E
loc_7_1C8D:
        ret    
loc_7_1C8E:
        sub    esp, 4
        mov    dword ptr [esp], 0
loc_7_1C98:
        mov    esi, dword ptr [ebp + 0xe4]
        or     esi, esi
        je     loc_7_1CC6
        mov    eax, dword ptr [esi + 0x14]
        mov    dword ptr [ebp + 0xe4], eax
        VxDCall  Dev_048B.Service_0007h          ; cd2007008b04
        call   sub_7_2DC6
        jae    loc_7_1C98
        mov    eax, dword ptr [ebp + 0xe0]
        movzx  eax, word ptr [eax]
        mov    dword ptr [esp], eax
        jmp    loc_7_1C98
loc_7_1CC6:
        mov    eax, 0xd00                    ; -> obj1:0x0D00
        call   sub_7_4A17
        mov    eax, dword ptr [esp]
        cmp    eax, 0x10
        cmc    
        jb     loc_7_1CDF
loc_7_1CD9:
        lea    esp, [esp + 4]
        jmp    loc_7_1C8D
loc_7_1CDF:
        mov    edx, dword ptr [ebp + 0xe0]
        mov    word ptr [edx], ax
        jmp    loc_7_1CD9
sub_7_1CEA:
        mov    eax, dword ptr [0xd94]        ; -> loc_1_0D94
loc_7_1CEF:
        mov    esi, eax
        cmp    esi, 0xd7c                    ; -> loc_1_0D7C
        je     loc_7_1D21
        mov    eax, dword ptr [esi + 0x18]
        cmp    dword ptr [esi + 8], ebp
        jne    loc_7_1CEF
        mov    ebx, dword ptr [esi + 0x14]
        mov    ecx, dword ptr [esi + 0x18]
        mov    dword ptr [ebx + 0x18], ecx
        mov    dword ptr [ecx + 0x14], ebx
        mov    ebx, esi
        xchg   dword ptr [ebp + 0xe4], ebx
        mov    dword ptr [esi + 0x14], ebx
        mov    dword ptr [esi + 0x18], 0xffffffca
        jmp    loc_7_1CEF
loc_7_1D21:
        cmp    dword ptr [ebp + 0xe4], 0
        je     loc_7_1D33
        call   sub_7_1C84
        jb     loc_7_1D33
        test   esp, esp
loc_7_1D33:
        ret    
loc_7_1D34:
        mov    eax, dword ptr [ebp + 0xd4]
        movzx  ecx, word ptr [ebp + 0xa4]
        sub    eax, ecx
        mov    dword ptr [ebp + 0xd4], eax
loc_7_1D49:
        cmp    dword ptr [ebp + 0xcc], 0
        je     loc_7_20A0
        mov    edx, dword ptr [ebp + 0xd4]
        mov    cl, byte ptr [ebp + 0x8c]
        shr    edx, cl
        push   edx
        call   sub_7_18B8
        pop    ebx
        jb     loc_7_20A0
        test   byte ptr [ebp + 4], 0x40
        jne    loc_7_1F9D
        mov    eax, ebx
        call   sub_7_2933
        jb     loc_7_2092
        mov    dword ptr [ebp + 0xc], eax
        mov    word ptr [ebp + 0x10], cx
        test   eax, 0x80000000
        jne    loc_7_1DC2
        test   eax, 0xffffff
        je     loc_7_1DB2
        xor    ecx, ecx
        mov    dword ptr [ebp + 8], ecx
        call   sub_7_2BAD
        mov    dword ptr [ebp + 8], ebx
        jne    loc_7_1DC2
        jb     loc_7_2092
loc_7_1DB2:
        or     dword ptr [ebp + 4], 0x140
        and    dword ptr [ebp + 0x14], 0
        and    word ptr [ebp + 0x10], 0
loc_7_1DC2:
        test   byte ptr [ebp + 4], 0x40
        jne    loc_7_1F9D
        mov    edi, dword ptr [ebp + 0x24]
        test   byte ptr [ebp + 0xf], 0x40
        je     loc_7_1F40
        test   byte ptr [ebp + 0xdc], 2
        jne    loc_7_1EB9
        movzx  ecx, byte ptr [ebp + 0x9b]
        cmp    dword ptr [ebp + 0xcc], ecx
        jb     loc_7_1EB9
        dec    ecx
        test   dword ptr [ebp + 0xd4], ecx
        jne    loc_7_1EB9
        xor    edx, edx
        xchg   dword ptr [ebp + 8], edx
        cmp    byte ptr [ebp + 0x10], cl
        jbe    loc_7_1F3A
        mov    edi, dword ptr [ebp + 0xc]
        and    edi, 0xffffff
        inc    ecx
        mov    ebx, dword ptr [ebp + 0xcc]
loc_7_1E24:
        add    edi, ecx
        sub    ebx, ecx
        cmp    ebx, ecx
        jb     loc_7_1E87
        inc    edx
        mov    eax, edx
        push   ecx
        push   edx
        push   edi
        call   sub_7_2933
        mov    esi, eax
        pop    edi
        mov    eax, ecx
        pop    edx
        pop    ecx
        jb     loc_7_2092
        push   esi
        and    esi, 0xe0000000
        cmp    esi, 0xc0000000
        pop    esi
        jne    loc_7_1E87
        cmp    al, cl
        jne    loc_7_1E87
        and    esi, 0xffffff
        cmp    esi, edi
        jne    loc_7_1E87
        push   ebx
        push   ecx
        push   edx
        push   edi
        mov    edi, edx
        shl    edi, 8
        add    edi, 0x10000000
        mov    ebx, ebp
        mov    ah, byte ptr [0xe68]          ; -> obj1:0x0E68
        xor    al, al
        VxDCall  Dev_048B.Service_0004h          ; cd2004008b04
        pop    edi
        pop    edx
        pop    ecx
        pop    ebx
        jb     loc_7_1E24
loc_7_1E87:
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0xffffff
        sub    edi, eax
        cmp    edi, ecx
        jbe    loc_7_1F3A
        mov    ecx, edi
        sub    dword ptr [ebp + 0xcc], edi
        add    dword ptr [ebp + 0xd4], edi
        xor    edi, edi
        call   sub_7_2E50
        jae    loc_7_1F8E
        jmp    loc_7_2092
loc_7_1EB9:
        mov    eax, dword ptr [ebp + 0xc]
        test   eax, 0x20000000
        jne    loc_7_1F40
        movzx  ecx, byte ptr [ebp + 0x9b]
        lea    ebx, [ecx - 1]
        and    ebx, dword ptr [ebp + 0xd4]
        jne    loc_7_1EDE
        test   byte ptr [ebp + 0xdc], 0x20
        je     loc_7_1F40
loc_7_1EDE:
        movzx  edx, byte ptr [ebp + 0x11]
        cmp    ecx, edx
        je     loc_7_1EF0
        add    ebx, dword ptr [ebp + 0xcc]
        cmp    ebx, edx
        ja     loc_7_1F40
loc_7_1EF0:
        mov    edx, dword ptr [ebp + 0xd4]
        dec    ecx
        and    eax, 0xffffff
        and    edx, ecx
        add    eax, edx
        inc    ecx
        sub    ecx, edx
        cmp    dword ptr [ebp + 0xcc], ecx
        ja     loc_7_1F11
        mov    ecx, dword ptr [ebp + 0xcc]
loc_7_1F11:
        xor    edi, edi
        push   ecx
        call   sub_7_2E50
        pop    ecx
        jb     loc_7_2092
        sub    dword ptr [ebp + 0xcc], ecx
        add    dword ptr [ebp + 0xd4], ecx
        and    dword ptr [ebp + 8], 0
        call   sub_7_1B4F
        jmp    loc_7_1D49
loc_7_1F3A:
        mov    edi, dword ptr [ebp + 0xd0]
loc_7_1F40:
        push   edi
        call   sub_7_2AA1
        pop    edi
        jb     loc_7_2092
        test   byte ptr [ebp + 0xf], 0x40
        je     loc_7_1F9D
        mov    edx, dword ptr [ebp + 0x14]
        shl    edx, 9
        add    edi, edx
        cmp    dword ptr [ebp + 8], 0
        jne    loc_7_1F9D
loc_7_1F61:
        movzx  ecx, byte ptr [ebp + 0x9b]
        mov    edx, dword ptr [ebp + 0x14]
        sub    ecx, edx
        xor    eax, eax
        shl    ecx, 7
        cld    
        rep stosd dword ptr es:[edi], eax
        mov    dword ptr [ebp + 0xd0], edi
        movzx  ecx, byte ptr [ebp + 0x9b]
        add    dword ptr [ebp + 0xd4], ecx
        sub    dword ptr [ebp + 0xcc], ecx
loc_7_1F8E:
        call   sub_7_1B4F
        call   sub_7_20A1
        jmp    loc_7_1D49
loc_7_1F9D:
        test   byte ptr [ebp + 5], 1
        jne    loc_7_2012
        mov    edi, dword ptr [ebp + 0xd0]
        test   byte ptr [ebp + 0xdc], 2
        jne    loc_7_1FCA
        movzx  ecx, byte ptr [ebp + 0x9b]
        cmp    dword ptr [ebp + 0xcc], ecx
        jb     loc_7_1FCA
        dec    ecx
        test   dword ptr [ebp + 0xd4], ecx
        je     loc_7_1FD5
loc_7_1FCA:
        call   0x484c
        jb     loc_7_2092
loc_7_1FD5:
        mov    ecx, dword ptr [ebp + 0x14]
        push   edi
        call   sub_7_281E
        pop    edi
        jb     loc_7_2087
        cmp    dword ptr [ebp + 0xd0], edi
        je     loc_7_2000
        call   sub_7_48CC
        and    dword ptr [ebp + 0x20], 0
        mov    edi, dword ptr [ebp + 0x24]
        or     dword ptr [ebp + 4], 0x100
loc_7_2000:
        mov    edx, dword ptr [ebp + 0x14]
        shl    edx, 9
        add    edi, edx
        test   byte ptr [ebp + 5], 1
        je     loc_7_1F61
loc_7_2012:
        movzx  ecx, byte ptr [ebp + 0x9b]
        mov    esi, dword ptr [ebp + 0xd4]
        dec    ecx
        and    esi, ecx
        inc    ecx
        sub    ecx, esi
        cmp    dword ptr [ebp + 0xcc], ecx
        ja     loc_7_2033
        mov    ecx, dword ptr [ebp + 0xcc]
loc_7_2033:
        mov    edx, ecx
        add    ecx, esi
        mov    edi, dword ptr [ebp + 0x14]
        sub    ecx, edi
        jbe    loc_7_204C
        shl    edi, 9
        shl    ecx, 7
        add    edi, dword ptr [ebp + 0x24]
        xor    eax, eax
        cld    
        rep stosd dword ptr es:[edi], eax
loc_7_204C:
        mov    ecx, edx
        mov    edi, dword ptr [ebp + 0x24]
        push   ecx
        call   sub_7_2859
        call   sub_7_1B4F
        pop    ecx
        add    dword ptr [ebp + 0xd4], ecx
        sub    dword ptr [ebp + 0xcc], ecx
        movzx  ecx, byte ptr [ebp + 0x9b]
        dec    ecx
        test   dword ptr [ebp + 0xd4], ecx
        jne    loc_7_1D49
        call   sub_7_20A1
        jmp    loc_7_1D49
loc_7_2087:
        mov    eax, dword ptr [ebp + 0xe0]
        mov    word ptr [eax], 0x14
loc_7_2092:
        and    dword ptr [ebp + 8], 0
        call   sub_7_1B4F
        call   sub_7_489D
loc_7_20A0:
        ret    
sub_7_20A1:
        test   byte ptr [ebp + 4], 0x80
        jne    loc_7_20D4
        xor    edx, edx
        xchg   dword ptr [ebp + 8], edx
        or     edx, edx
        je     loc_7_20D4
        and    dword ptr [ebp + 4], 0xffffffbf
        movzx  ecx, byte ptr [ebp + 0x9b]
        shr    ecx, 3
loc_7_20BE:
        mov    esi, dword ptr [ebp + ecx*4 + 0x200]
        push   ecx
        call   sub_7_1C09
        VxDCall  Dev_048B.Service_0005h          ; cd2005008b04
        pop    ecx
        loop   loc_7_20BE
loc_7_20D4:
        ret    
loc_7_20D5:
        mov    eax, dword ptr [ebp + 0xd4]
        movzx  ecx, word ptr [ebp + 0xa4]
        sub    eax, ecx
        mov    dword ptr [ebp + 0xd4], eax
        cmp    dword ptr [0xe2c], 0          ; -> loc_1_0E2C
        jne    loc_7_20FB
        bts    dword ptr [ebp + 0xdc], 0x18
loc_7_20FB:
        cmp    dword ptr [ebp + 0xcc], 0
        je     loc_7_2389
        mov    edx, dword ptr [ebp + 0xd4]
        mov    cl, byte ptr [ebp + 0x8c]
        shr    edx, cl
        push   edx
        call   sub_7_18B8
        pop    ebx
        jb     loc_7_2389
        btr    dword ptr [ebp + 4], 0xa
        bt     dword ptr [ebp + 0xdc], 0x18
        sbb    eax, eax
        and    eax, 0x400
        or     dword ptr [ebp + 4], eax
        test   byte ptr [ebp + 4], 0x40
        jne    loc_7_21E5
        mov    eax, ebx
        call   sub_7_2933
        jb     loc_7_237B
        mov    dword ptr [ebp + 0xc], eax
        mov    word ptr [ebp + 0x10], cx
        test   byte ptr [ebp + 0xdf], 1
        je     loc_7_21E5
        mov    edx, eax
        and    edx, 0xe0000000
        cmp    edx, 0xc0000000
        jne    loc_7_21E5
        movzx  edx, byte ptr [ebp + 0x9b]
        cmp    dl, ch
        je     loc_7_2194
        dec    edx
        and    edx, dword ptr [ebp + 0xd4]
        add    edx, dword ptr [ebp + 0xcc]
        movzx  edi, ch
        cmp    edx, edi
        ja     loc_7_21E5
loc_7_2194:
        movzx  ecx, byte ptr [ebp + 0x9b]
        mov    edx, dword ptr [ebp + 0xd4]
        dec    ecx
        and    eax, 0xffffff
        and    edx, ecx
        add    eax, edx
        inc    ecx
        sub    ecx, edx
        cmp    dword ptr [ebp + 0xcc], ecx
        ja     loc_7_21BC
        mov    ecx, dword ptr [ebp + 0xcc]
loc_7_21BC:
        xor    edi, edi
        push   ecx
        call   sub_7_2E49
        pop    ecx
        jb     loc_7_237B
        sub    dword ptr [ebp + 0xcc], ecx
        add    dword ptr [ebp + 0xd4], ecx
        and    dword ptr [ebp + 8], 0
        call   sub_7_1B4F
        jmp    loc_7_20FB
loc_7_21E5:
        test   byte ptr [ebp + 0xdc], 2
        jne    loc_7_2252
        movzx  ecx, byte ptr [ebp + 0x9b]
        cmp    dword ptr [ebp + 0xcc], ecx
        jb     loc_7_2252
        dec    ecx
        test   dword ptr [ebp + 0xd4], ecx
        jne    loc_7_2252
        inc    ecx
        mov    dword ptr [ebp + 0x14], ecx
        mov    esi, dword ptr [ebp + 0xd0]
        or     dword ptr [ebp + 4], 0x180
        call   sub_7_238A
        jb     loc_7_237B
        test   byte ptr [ebp + 5], 1
        je     loc_7_222C
        and    dword ptr [ebp + 8], 0
loc_7_222C:
        movzx  ecx, byte ptr [ebp + 0x9b]
        add    dword ptr [ebp + 0xd4], ecx
        sub    dword ptr [ebp + 0xcc], ecx
        shl    ecx, 9
        add    dword ptr [ebp + 0xd0], ecx
        call   sub_7_1B4F
        jmp    loc_7_20FB
loc_7_2252:
        test   byte ptr [ebp + 4], 0x40
        jne    loc_7_2288
        movzx  ecx, byte ptr [ebp + 0x9b]
        cmp    dword ptr [ebp + 0xcc], ecx
        jb     loc_7_2270
        dec    ecx
        test   dword ptr [ebp + 0xd4], ecx
        je     loc_7_2288
loc_7_2270:
        and    dword ptr [ebp + 0x14], 0
        test   byte ptr [ebp + 0xf], 0x80
        je     loc_7_2288
        mov    edi, dword ptr [ebp + 0x24]
        call   sub_7_2AA1
        jb     loc_7_237B
loc_7_2288:
        movzx  ecx, byte ptr [ebp + 0x9b]
        mov    esi, dword ptr [ebp + 0xd4]
        dec    ecx
        and    esi, ecx
        inc    ecx
        sub    ecx, esi
        cmp    dword ptr [ebp + 0xcc], ecx
        ja     loc_7_22A9
        mov    ecx, dword ptr [ebp + 0xcc]
loc_7_22A9:
        mov    edx, dword ptr [ebp + 0x14]
        cmp    edx, esi
        jb     loc_7_22BD
        lea    eax, [esi + ecx]
        cmp    byte ptr [ebp + 0x9b], al
        ja     loc_7_22BD
        mov    edx, esi
loc_7_22BD:
        or     edx, edx
        je     loc_7_22F0
        test   byte ptr [ebp + 5], 1
        jne    loc_7_22F0
        push   esi
        push   ecx
        push   edx
        push   edx
        call   0x484c
        pop    ecx
        jb     loc_7_22D8
        call   sub_7_281E
loc_7_22D8:
        pop    edx
        pop    ecx
        pop    esi
        jb     loc_7_236A
        push   ecx
        push   edx
        push   esi
        call   sub_7_48CC
        pop    esi
        pop    edx
        pop    ecx
        and    dword ptr [ebp + 0x20], 0
loc_7_22F0:
        cmp    edx, esi
        jae    loc_7_230C
        mov    edi, edx
        shl    edi, 9
        add    edi, dword ptr [ebp + 0x24]
        xchg   ecx, edx
        neg    ecx
        add    ecx, esi
        shl    ecx, 7
        xor    eax, eax
        cld    
        rep stosd dword ptr es:[edi], eax
        mov    ecx, edx
loc_7_230C:
        push   ecx
        push   esi
        mov    eax, esi
        call   sub_7_28C2
        pop    esi
        pop    ecx
        or     dword ptr [ebp + 4], 0x1c0
        add    dword ptr [ebp + 0xd4], ecx
        sub    dword ptr [ebp + 0xcc], ecx
        add    ecx, esi
        cmp    dword ptr [ebp + 0x14], ecx
        ja     loc_7_2334
        mov    dword ptr [ebp + 0x14], ecx
loc_7_2334:
        cmp    dword ptr [ebp + 0x58], -1
        je     loc_7_2356
        movzx  eax, byte ptr [ebp + 0x9b]
        cmp    dword ptr [ebp + 0x58], eax
        jbe    loc_7_2356
        cmp    word ptr [ebp + 0xb4], 3
        jae    loc_7_2360
        cmp    dword ptr [ebp + 0x64], 0
        ja     loc_7_2360
loc_7_2356:
        mov    esi, dword ptr [ebp + 0x24]
        call   sub_7_238A
        jb     loc_7_237B
loc_7_2360:
        call   sub_7_1B4F
        jmp    loc_7_20FB
loc_7_236A:
        mov    eax, dword ptr [ebp + 0xe0]
        cmp    word ptr [eax], 0x10
        jae    loc_7_237B
        mov    word ptr [eax], 0x14
loc_7_237B:
        and    dword ptr [ebp + 8], 0
        call   sub_7_1B4F
        call   sub_7_489D
loc_7_2389:
        ret    
sub_7_238A:
        mov    ebx, dword ptr [ebp + 0x24]
        xor    ebx, esi
        or     dword ptr [ebp + 4], 0x200
        and    dword ptr [ebp + 4], 0xffffff7f
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [ebp + 0x18], eax
        mov    edi, esi
        test   byte ptr [ebp + 5], 4
        jne    loc_7_23DC
        push   ebx
        call   0x484c
        pop    ebx
        jb     loc_7_2559
        push   esi
        call   sub_7_2722
        pop    edi
        jb     loc_7_23D7
        call   sub_7_48CC
        mov    edi, dword ptr [ebp + 0x24]
        and    dword ptr [ebp + 4], 0xfffffcff
        bts    dword ptr [ebp + 4], 6
        jmp    loc_7_23DC
loc_7_23D7:
        call   sub_7_489D
loc_7_23DC:
        test   byte ptr [ebp + 0xdf], 2
        je     loc_7_242F
        test   byte ptr [ebp + 0xf], 0x80
        jne    loc_7_242F
        test   byte ptr [ebp + 0xf], 0x20
        jne    loc_7_242F
        test   dword ptr [ebp + 0xc], 0xffffff
        je     loc_7_242F
        mov    eax, dword ptr [ebp + 0x18]
        cmp    byte ptr [ebp + 0x11], al
        jne    loc_7_242F
        mov    eax, dword ptr [ebp + 0xc]
        mov    cx, word ptr [ebp + 0x10]
        push   ebx
        push   edi
        xor    ebx, ebx
        call   sub_7_0F7C
        pop    edi
        pop    ebx
        jb     loc_7_242F
        push   edi
        call   sub_7_0EFA
        pop    edi
        jb     loc_7_2559
        or     dword ptr [ebp + 0xc], 0x80000000
        jmp    loc_7_24BF
loc_7_242F:
        mov    eax, dword ptr [ebp + 0x18]
        xor    ecx, ecx
        test   byte ptr [ebp + 0xf], 0x80
        je     loc_7_2449
        test   byte ptr [ebp + 0xf], 0x20
        jne    loc_7_245A
        cmp    byte ptr [ebp + 0x11], al
        je     loc_7_24BF
        movzx  ecx, byte ptr [ebp + 0x11]
loc_7_2449:
        cmp    dword ptr [ebp + 0x58], -1
        je     loc_7_245A
        add    ecx, dword ptr [ebp + 0x58]
        cmp    ecx, eax
        jbe    loc_7_254E
loc_7_245A:
        test   byte ptr [ebp + 0xf], 0x80
        je     loc_7_2476
        mov    eax, dword ptr [ebp + 0xc]
        mov    cx, word ptr [ebp + 0x10]
        push   ebx
        push   edi
        call   sub_7_0E90
        pop    edi
        pop    ebx
        jb     loc_7_2559
loc_7_2476:
        push   edi
        call   sub_7_04C8
        pop    edi
        jae    loc_7_24D7
        test   byte ptr [ebp + 0xf], 0x80
        je     loc_7_24AA
        mov    eax, dword ptr [ebp + 0xc]
        mov    cx, word ptr [ebp + 0x10]
        push   ebx
        push   edi
        xor    ebx, ebx
        call   sub_7_0CB0
        pop    edi
        pop    ebx
        jb     loc_7_2559
        push   edi
        call   sub_7_0EFA
        pop    edi
        jb     loc_7_2559
loc_7_24AA:
        mov    eax, dword ptr [ebp + 0xe0]
        cmp    word ptr [eax], 0x10
        jae    loc_7_2559
        jmp    loc_7_2554
loc_7_24BF:
        mov    eax, dword ptr [ebp + 0xc]
        mov    cx, word ptr [ebp + 0x10]
        push   ebx
        push   edi
        xor    ebx, ebx
        call   sub_7_0CB0
        pop    edi
        pop    ebx
        jb     loc_7_2559
loc_7_24D7:
        push   ebx
        push   edi
        call   sub_7_259A
        pop    edi
        pop    ebx
        jb     loc_7_2567
        mov    eax, dword ptr [ebp + 0x100]
        shld   ecx, eax, 0x10
        mov    cl, byte ptr [ebp + 0x14]
        mov    word ptr [ebp + 0x10], cx
        and    eax, 0xffffff
        bt     dword ptr [ebp + 4], 9
        sbb    edx, edx
        and    edx, 0x40000000
        or     eax, edx
        cmp    dword ptr [ebp + 0xfc], 2
        cmc    
        sbb    edx, edx
        and    edx, 0x20000000
        or     eax, edx
        or     eax, 0x80000000
        mov    dword ptr [ebp + 0xc], eax
        mov    edx, dword ptr [ebp + 8]
        call   sub_7_29FE
        jb     loc_7_2559
        or     ebx, ebx
        jne    loc_7_254C
        test   byte ptr [ebp + 5], 2
        jne    loc_7_254C
        call   sub_7_48BD
        jb     loc_7_254C
        call   sub_7_48CC
        or     dword ptr [ebp + 4], 0x100
loc_7_254C:
        clc    
loc_7_254D:
        ret    
loc_7_254E:
        mov    eax, dword ptr [ebp + 0xe0]
loc_7_2554:
        mov    word ptr [eax], 0x2b
loc_7_2559:
        and    dword ptr [ebp + 8], 0
        and    dword ptr [ebp + 4], 0xfffff87f
        stc    
        jmp    loc_7_254D
loc_7_2567:
        and    dword ptr [ebp + 0xc], 0
        mov    eax, dword ptr [ebp + 0xe0]
        movzx  eax, word ptr [eax]
        cmp    eax, 0xff
        ja     loc_7_2559
        push   edi
        mov    edi, 0x2591                   ; -> loc_7_2591
        mov    ecx, 9
        cld    
        repne scasb al, byte ptr es:[edi]
        pop    edi
        jne    loc_7_2559
        jmp    loc_7_23DC
loc_7_2591:
        adc    byte ptr [edx + 0x1e1d1b1a], dl
        and    byte ptr [ecx], ah
sub_7_259A:
        cmp    dword ptr [ebp + 0xfc], 1
        ja     loc_7_25BA
        mov    eax, dword ptr [ebp + 0x100]
        xor    ecx, ecx
        shld   ecx, eax, 8
        and    eax, 0xffffff
        call   sub_7_2E49
loc_7_25B9:
        ret    
loc_7_25BA:
        test   byte ptr [ebp + 4], 0x20
        je     loc_7_25D5
        movzx  esi, byte ptr [ebp + 0x9b]
        cmp    dword ptr [ebp + 0x18], esi
        jae    loc_7_25D5
        dec    esi
        shl    esi, 9
        add    esi, dword ptr [ebp + 0x24]
        jmp    loc_7_25E0
loc_7_25D5:
        push   edi
        call   0x484c
        mov    esi, edi
        pop    edi
        jb     loc_7_25B9
loc_7_25E0:
        call   sub_7_26EB
        test   byte ptr [ebp + 5], 1
        je     loc_7_25F9
        mov    edx, 0x45544550
loc_7_25F0:
        mov    dword ptr [eax], edx
        add    eax, 4
        loop   loc_7_25F0
        jmp    loc_7_2605
loc_7_25F9:
        mov    edx, dword ptr [edi]
        add    edi, 4
        mov    dword ptr [eax], edx
        add    eax, 4
        loop   loc_7_25F9
loc_7_2605:
        movzx  ecx, byte ptr [ebp + 0x103]
        dec    ecx
        je     loc_7_2684
        push   dword ptr [ebp + 0xd0]
        push   dword ptr [ebp + 0xc8]
        push   dword ptr [ebp + 0xdc]
        lea    edx, [ebp + 0x224]
        mov    dword ptr [ebp + 0xd0], edx
        xor    eax, eax
        mov    dword ptr [ebp + 0xc8], eax
        or     dword ptr [ebp + 0xdc], 2
        mov    dword ptr [edx + 0x10], eax
        inc    eax
        mov    dword ptr [edx], eax
        mov    dword ptr [edx + 4], esi
        mov    dword ptr [edx + 8], ecx
        mov    dword ptr [edx + 0xc], edi
        shl    ecx, 9
        add    edi, ecx
        mov    eax, dword ptr [ebp + 0x100]
        rol    eax, 8
        movzx  ecx, al
        shr    eax, 8
        push   edi
        push   esi
        xor    edi, edi
        call   sub_7_2E49
        pop    esi
        pop    edi
        pop    dword ptr [ebp + 0xdc]
        pop    dword ptr [ebp + 0xc8]
        pop    dword ptr [ebp + 0xd0]
        jb     loc_7_25B9
        jmp    loc_7_26A5
loc_7_2684:
        mov    eax, dword ptr [ebp + 0x100]
        and    eax, 0xffffff
        mov    ecx, 1
        push   edi
        mov    edi, esi
        push   esi
        call   sub_7_2E49
        pop    esi
        pop    edi
        jb     loc_7_25B9
loc_7_26A5:
        movzx  ecx, byte ptr [ebp + 0x9b]
        dec    ecx
        shl    ecx, 9
        add    ecx, dword ptr [ebp + 0x24]
        cmp    ecx, esi
        je     loc_7_26BC
        call   sub_7_489D
loc_7_26BC:
        mov    ecx, dword ptr [ebp + 0xfc]
        dec    ecx
        lea    ebx, [ebp + 0x104]
loc_7_26C9:
        push   ecx
        mov    eax, dword ptr [ebx]
        add    ebx, 4
        rol    eax, 8
        movzx  ecx, al
        shr    eax, 8
        call   sub_7_2E49
        pop    ecx
        jb     loc_7_25B9
        loop   loc_7_26C9
        jmp    loc_7_25B9
sub_7_26EB:
        lea    ebx, [ebp + 0xfc]
        mov    ecx, dword ptr [ebx]
        push   esi
        add    ebx, 4
        mov    dword ptr [esi], ecx
        add    esi, 4
loc_7_26FC:
        mov    eax, dword ptr [ebx]
        add    ebx, 4
        rol    eax, 8
        dec    al
        shl    al, 2
        ror    eax, 8
        mov    dword ptr [esi], eax
        add    esi, 4
        loop   loc_7_26FC
        mov    eax, esi
        pop    esi
        lea    ecx, [esi + 0x200]
        sub    ecx, eax
        shr    ecx, 2
        ret    
sub_7_2722:
        push   ebx
        sub    esp, 0x14
        mov    ecx, dword ptr [ebp + 0x14]
        mov    dword ptr [ebp + 0x18], ecx
        cmp    ecx, 2
        jb     loc_7_27B6
        mov    dword ptr [esp], esi
        mov    dword ptr [esp + 8], edi
        movzx  eax, byte ptr [ebp + 0x9b]
        shl    eax, 9
        mov    word ptr [esp + 0xc], ax
        shl    ecx, 9
        mov    word ptr [esp + 4], cx
        movzx  edx, word ptr [ebp + 0xb4]
        mov    esi, 0xd08                    ; -> obj1:0x0D08
loc_7_275F:
        add    esi, 0x10
        cmp    dword ptr [esi], edx
        jne    loc_7_275F
        mov    edx, dword ptr [0xe2c]        ; -> loc_1_0E2C
        dec    edx
        je     loc_7_27BC
loc_7_276F:
        mov    eax, 0xd58                    ; -> obj1:0x0D58
        call   sub_7_490C
        jb     loc_7_27B6
        mov    eax, dword ptr [esi + edx*4]
loc_7_277E:
        lea    esi, [esp]
        push   ebp
        mov    ebx, dword ptr [0xe3c]        ; -> obj1:0x0E3C
        call   dword ptr [ebx + 8]
        pop    ebp
        cmp    eax, 1
        cmc    
        jb     loc_7_27CC
loc_7_2792:
        movzx  edx, word ptr [esp + 0xc]
        mov    dword ptr [ebp + 0x1c], edx
        add    edx, 0x1ff
        shr    edx, 9
        mov    dword ptr [ebp + 0x18], edx
        inc    edx
        cmp    dword ptr [ebp + 0x14], edx
loc_7_27AA:
        pushfd 
        mov    eax, 0xd58                    ; -> obj1:0x0D58
        call   sub_7_493D
        popfd  
loc_7_27B6:
        lea    esp, [esp + 0x14]
        pop    ebx
        ret    
loc_7_27BC:
        mov    ecx, dword ptr [ebp + 0x58]
        cmp    ecx, -1
        je     loc_7_27C9
        cmp    dword ptr [ebp + 0x5c], ecx
        jb     loc_7_27B6
loc_7_27C9:
        inc    edx
        jmp    loc_7_276F
loc_7_27CC:
        cmp    eax, 9
        je     loc_7_27DE
        cmp    eax, 8
        mov    eax, 0x40
        je     loc_7_277E
        stc    
        jmp    loc_7_27AA
loc_7_27DE:
        push   edx
        mov    eax, 0x10
        mov    edx, ebp
        mov    esi, 0x2803                   ; -> loc_7_2803
        cli    
        VMMCall  VMM.Set_Async_Time_Out          ; cd20fe000100
        pop    edx
        call   sub_7_4A6C
        lea    esi, [esp]
        call   edx
        or     eax, eax
        je     loc_7_2792
        jmp    loc_7_27CC
loc_7_2803:
        mov    esi, 0x49ed                   ; -> sub_7_49ED
        push   eax
        push   ebx
        push   ecx
        xor    eax, eax
        mov    ebx, eax
        mov    ecx, 0x408
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
        pop    ecx
        pop    ebx
        pop    eax
        ret    
sub_7_281E:
        push   ebx
        sub    esp, 0x14
        mov    esi, dword ptr [ebp + 0x24]
        add    esi, dword ptr [ebp + 0x20]
        shl    ecx, 9
        mov    word ptr [esp + 0xc], cx
        mov    dword ptr [esp + 8], edi
        mov    dword ptr [esp], esi
        and    dword ptr [esp + 0x10], 0
        mov    eax, 0x20
        lea    esi, [esp]
        push   ebp
        mov    ebx, dword ptr [0xe3c]        ; -> obj1:0x0E3C
        call   dword ptr [ebx + 8]
        pop    ebp
        cmp    eax, 1
        cmc    
        lea    esp, [esp + 0x14]
        pop    ebx
        ret    
sub_7_2859:
        cld    
        shl    esi, 9
        add    esi, edi
        test   byte ptr [ebp + 0xdc], 2
        jne    loc_7_287A
        mov    edi, dword ptr [ebp + 0xd0]
        shl    ecx, 7
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    dword ptr [ebp + 0xd0], edi
        ret    
loc_7_287A:
        mov    eax, dword ptr [ebp + 0xd0]
        mov    edi, dword ptr [ebp + 0xc8]
        mov    edx, dword ptr [eax]
        sub    edx, edi
        push   ecx
        cmp    edx, ecx
        ja     loc_7_2891
        mov    ecx, edx
loc_7_2891:
        add    dword ptr [ebp + 0xc8], ecx
        shl    edi, 9
        add    edi, dword ptr [eax + 4]
        mov    eax, dword ptr [eax]
        cmp    dword ptr [ebp + 0xc8], eax
        jb     loc_7_28B5
        add    dword ptr [ebp + 0xd0], 8
        and    dword ptr [ebp + 0xc8], 0
loc_7_28B5:
        mov    eax, ecx
        shl    ecx, 7
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    ecx
        sub    ecx, eax
        ja     loc_7_287A
        ret    
sub_7_28C2:
        mov    esi, dword ptr [ebp + 0x24]
sub_7_28C5:
        cld    
        mov    edi, eax
        shl    edi, 9
        add    edi, esi
        test   dword ptr [ebp + 0xdc], 2
        jne    loc_7_28EB
        mov    esi, dword ptr [ebp + 0xd0]
        shl    ecx, 7
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    dword ptr [ebp + 0xd0], esi
        ret    
loc_7_28EB:
        mov    eax, dword ptr [ebp + 0xd0]
        mov    esi, dword ptr [ebp + 0xc8]
        mov    edx, dword ptr [eax]
        sub    edx, esi
        push   ecx
        cmp    edx, ecx
        ja     loc_7_2902
        mov    ecx, edx
loc_7_2902:
        add    dword ptr [ebp + 0xc8], ecx
        shl    esi, 9
        add    esi, dword ptr [eax + 4]
        mov    eax, dword ptr [eax]
        cmp    dword ptr [ebp + 0xc8], eax
        jb     loc_7_2926
        add    dword ptr [ebp + 0xd0], 8
        and    dword ptr [ebp + 0xc8], 0
loc_7_2926:
        mov    eax, ecx
        shl    ecx, 7
        rep movsd dword ptr es:[edi], dword ptr [esi]
        pop    ecx
        sub    ecx, eax
        ja     loc_7_28EB
        ret    
sub_7_2933:
        cmp    word ptr [ebp + 0xb4], 3
        jae    loc_7_2996
        push   eax
        push   ebx
        shr    eax, 0xa
        shl    eax, 3
        add    eax, dword ptr [ebp + 0x68]
        lea    ebx, [ebp + 0x30]
        mov    ecx, 8
        call   sub_7_2C95
        pop    ebx
        pop    edx
        jb     loc_7_2982
        and    edx, 0x3ff
        shl    edx, 2
        add    edx, eax
        mov    eax, dword ptr [edx]
        btr    eax, 0x15
        shld   ecx, eax, 0xa
        mov    ch, cl
        shr    cl, 4
        and    ch, 0xf
        add    cx, 0x101
        and    eax, 0xc01fffff
loc_7_2980:
        ret    
loc_7_2981:
        pop    ebx
loc_7_2982:
        call   sub_7_43D3
        jb     loc_7_2980
        mov    esi, 3
        call   sub_7_4B5E
        stc    
        jmp    loc_7_2980
loc_7_2996:
        push   ebx
        xor    edx, edx
        mov    ecx, 0x330
        div    ecx
        shl    eax, 3
        add    eax, dword ptr [ebp + 0x68]
        lea    ebx, [ebp + 0x30]
        mov    ecx, 8
        push   edx
        call   sub_7_2C95
        mov    ebx, eax
        pop    eax
        jb     loc_7_2981
        xor    edx, edx
        mov    ecx, 0x66
        div    ecx
        shl    eax, 9
        add    ebx, eax
        lea    edx, [edx + edx*4]
        add    edx, ebx
        mov    eax, dword ptr [edx]
        mov    cx, word ptr [edx + 3]
        xchg   ch, cl
        and    eax, 0xff7fffff
        shl    eax, 8
        mov    al, cl
        and    cl, 0x3f
        ror    eax, 8
        and    eax, 0xc07fffff
        shr    ch, 2
        sbb    ebx, ebx
        and    ebx, 0x20000000
        add    cx, 0x101
        or     eax, ebx
        pop    ebx
        jmp    loc_7_2980
sub_7_29FE:
        sub    cx, 0x101
        cmp    word ptr [ebp + 0xb4], 3
        jae    loc_7_2A68
        shl    cl, 4
        or     cl, ch
        rol    eax, 0xa
        mov    al, cl
        ror    eax, 0xa
        push   eax
        mov    eax, edx
        call   sub_7_2933
        pop    eax
        jb     loc_7_2A43
        cmp    dword ptr [edx], eax
        je     loc_7_2A43
loc_7_2A29:
        or     dword ptr [ebp + 4], 0x1000
        mov    dword ptr [edx], eax
        sub    edx, dword ptr [esi + 0x10]
        shr    edx, 9
        cmp    byte ptr [esi + 0x32], 0
        je     loc_7_2A44
loc_7_2A3E:
        bts    dword ptr [esi + 0x32], edx
        clc    
loc_7_2A43:
        ret    
loc_7_2A44:
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf000100
        mov    dword ptr [esi + 0x24], eax
        mov    ecx, dword ptr [0xd90]        ; -> loc_1_0D90
        mov    dword ptr [esi + 0x14], ecx
        mov    dword ptr [esi + 0x18], 0xd7c ; -> loc_1_0D7C
        mov    dword ptr [ecx + 0x18], esi
        mov    dword ptr [0xd90], esi        ; -> loc_1_0D90
        jmp    loc_7_2A3E
loc_7_2A68:
        xor    esi, esi
        btr    eax, 0x1d
        rcl    esi, 1
        btr    eax, 0x1c
        rcl    esi, 0x19
        rol    eax, 8
        or     cl, al
        shl    ch, 2
        mov    al, ch
        ror    eax, 8
        or     eax, esi
        push   eax
        push   ecx
        mov    eax, edx
        call   sub_7_2933
        pop    ecx
        pop    eax
        jb     loc_7_2A3E
        cmp    dword ptr [edx], eax
        jne    loc_7_2A9C
        cmp    byte ptr [edx + 4], cl
        je     loc_7_2A43
loc_7_2A9C:
        mov    byte ptr [edx + 4], cl
        jmp    loc_7_2A29
sub_7_2AA1:
        xor    edx, edx
        mov    dword ptr [ebp + 0x20], edx
        mov    cx, word ptr [ebp + 0x10]
        mov    dl, cl
        mov    dword ptr [ebp + 0x14], edx
        mov    eax, dword ptr [ebp + 0xc]
        bt     eax, 0x1e
        sbb    edx, edx
        and    dword ptr [ebp + 4], 0xfffffeff
        and    edx, 0x100
        or     dword ptr [ebp + 4], edx
        push   ebx
        push   edi
        mov    ebx, edi
        call   sub_7_0CB0
        pop    edi
        pop    ebx
        jb     loc_7_2B2F
        mov    eax, dword ptr [ebp + 0xfc]
        dec    eax
        jne    loc_7_2B4B
        mov    edx, dword ptr [ebp + 0x100]
        shld   eax, edx, 8
        mov    dword ptr [ebp + 0x18], eax
        mov    ecx, eax
        mov    eax, edx
        and    eax, 0xffffff
        push   edi
        call   sub_7_2E50
        pop    edi
        jb     loc_7_2B2E
        test   byte ptr [ebp + 5], 1
        jne    loc_7_2B18
        cmp    word ptr [edi], 0x5153
        je     loc_7_2B18
        cmp    word ptr [edi], 0x4d4a
        je     loc_7_2B18
        cmp    word ptr [edi], 0x5344
        jne    loc_7_2B2F
loc_7_2B18:
        or     dword ptr [ebp + 4], 0x40
        test   byte ptr [ebp + 5], 1
        je     loc_7_2B2E
        mov    eax, dword ptr [ebp + 0x18]
        cmp    eax, dword ptr [ebp + 0x14]
        ja     loc_7_2B2E
        mov    dword ptr [ebp + 0x14], eax
        clc    
loc_7_2B2E:
        ret    
loc_7_2B2F:
        test   byte ptr [ebp + 0xf], 0x80
        stc    
        je     loc_7_2B2E
        mov    edx, dword ptr [ebp + 0xe0]
        cmp    word ptr [edx], 0x10
        cmc    
        jb     loc_7_2B2E
        mov    word ptr [edx], 0x14
        stc    
        jmp    loc_7_2B2E
loc_7_2B4B:
        mov    ecx, eax
        movzx  edx, byte ptr [ebp + 0x103]
        dec    edx
        test   byte ptr [ebp + 0xf], 0x40
        jne    loc_7_2B7D
        lea    eax, [eax*4 + 8]
        mov    dword ptr [ebp + 0x20], eax
        inc    edx
        add    eax, edi
        cmp    word ptr [eax], 0x4d4a
        je     loc_7_2B7D
        cmp    word ptr [eax], 0x5344
        je     loc_7_2B7D
        cmp    word ptr [eax], 0x5153
        jne    loc_7_2B2F
loc_7_2B7D:
        mov    dword ptr [ebp + 0x18], edx
        shl    edx, 9
        add    edi, edx
        lea    esi, [ebp + 0x104]
loc_7_2B8B:
        mov    eax, dword ptr [esi]
        add    esi, 4
        rol    eax, 8
        push   ecx
        push   esi
        mov    cl, al
        add    dword ptr [ebp + 0x18], ecx
        shr    eax, 8
        call   sub_7_2E50
        pop    esi
        pop    ecx
        jb     loc_7_2B2E
        loop   loc_7_2B8B
        jmp    loc_7_2B18
sub_7_2BAD:
        push   ebx
        push   ecx
        mov    eax, dword ptr [ebp + 0xc]
        mov    ebx, dword ptr [ebp + 0x24]
        mov    cx, word ptr [ebp + 0x10]
        mov    word ptr [ebp + 0xf0], 0
        call   sub_7_0F7C
        pop    ecx
        pop    ebx
        jb     loc_7_2C09
        jecxz  loc_7_2BD4
        or     dword ptr [ebp + 0xc], 0x80000000
loc_7_2BD4:
        xor    eax, eax
        xchg   dword ptr [ebp + 8], eax
        test   byte ptr [ebp + 0xf], 0x40
        jne    loc_7_2BEB
        mov    dword ptr [ebp + 8], ebx
        push   ecx
        call   sub_7_2C3B
        pop    ecx
        jb     loc_7_2C09
loc_7_2BEB:
        or     esp, esp
        jecxz  loc_7_2C08
        call   sub_7_0EFA
        jb     loc_7_2C08
        mov    eax, dword ptr [ebp + 0xc]
        mov    cx, word ptr [ebp + 0x10]
        mov    edx, ebx
        call   sub_7_29FE
        jb     loc_7_2C08
        or     esp, esp
loc_7_2C08:
        ret    
loc_7_2C09:
        and    dword ptr [ebp + 8], 0
        cmp    word ptr [ebp + 0xf0], 0x10
        cmc    
        jb     loc_7_2C08
        xor    eax, eax
        mov    ecx, dword ptr [ebp + 0xe0]
        mov    word ptr [ecx], ax
        mov    dword ptr [ebp + 0xc], eax
        mov    dword ptr [ebp + 0x10], eax
        mov    ecx, 0x101
        mov    edx, ebx
        call   sub_7_29FE
        jb     loc_7_2C08
        cmp    eax, eax
        jmp    loc_7_2C08
sub_7_2C3B:
        mov    edi, dword ptr [ebp + 0x24]
        call   sub_7_2AA1
        jb     loc_7_2C8C
        test   byte ptr [ebp + 5], 1
        jne    loc_7_2C8C
        mov    esi, dword ptr [ebp + 0x24]
        mov    edi, dword ptr [ebp + 0x18]
        movzx  ecx, byte ptr [ebp + 0x9b]
        sub    ecx, edi
        je     loc_7_2C69
        shl    edi, 9
        add    edi, esi
        shl    ecx, 7
        xor    eax, eax
        cld    
        rep stosd dword ptr es:[edi], eax
loc_7_2C69:
        push   ebx
        call   0x484c
        pop    ebx
        jb     loc_7_2C8C
        mov    ecx, dword ptr [ebp + 0x14]
        call   sub_7_281E
        jb     loc_7_2C8D
        call   sub_7_48CC
        and    dword ptr [ebp + 0x20], 0
        or     dword ptr [ebp + 4], 0x140
loc_7_2C8C:
        ret    
loc_7_2C8D:
        call   sub_7_489D
        stc    
        jmp    loc_7_2C8C
sub_7_2C95:
        sub    esp, 0xc
        mov    esi, dword ptr [ebx]
        or     esi, esi
        je     loc_7_2CFF
        cmp    dword ptr [esi + 8], ebp
        jne    loc_7_2CF8
        cmp    dword ptr [esi + 0xc], eax
        jne    loc_7_2CF8
loc_7_2CA8:
        mov    eax, dword ptr [esi + 0x10]
        cmp    byte ptr [esi + 0x32], 0
        jne    loc_7_2CB6
loc_7_2CB1:
        lea    esp, [esp + 0xc]
        ret    
loc_7_2CB6:
        cmp    dword ptr [esi + 0x18], -0x36
        jae    loc_7_2CB1
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf000100
        mov    dword ptr [esi + 0x24], eax
        mov    eax, dword ptr [esi + 0x10]
        cmp    dword ptr [0xd90], esi        ; -> loc_1_0D90
        je     loc_7_2CB1
        mov    ecx, dword ptr [esi + 0x14]
        mov    edx, dword ptr [esi + 0x18]
        mov    dword ptr [ecx + 0x18], edx
        mov    dword ptr [edx + 0x14], ecx
        mov    ecx, dword ptr [0xd90]        ; -> loc_1_0D90
        mov    dword ptr [esi + 0x14], ecx
        mov    dword ptr [esi + 0x18], 0xd7c ; -> loc_1_0D7C
        mov    dword ptr [ecx + 0x18], esi
        mov    dword ptr [0xd90], esi        ; -> loc_1_0D90
        clc    
        jmp    loc_7_2CB1
loc_7_2CF8:
        and    dword ptr [esi + 0x1c], 0
        and    dword ptr [ebx], 0
loc_7_2CFF:
        mov    dword ptr [esp + 8], eax
        mov    dword ptr [esp], ebx
        mov    dword ptr [esp + 4], ecx
loc_7_2D0A:
        cmp    dword ptr [0xd00], 0          ; -> obj1:0x0D00
        jne    loc_7_2D99
loc_7_2D17:
        mov    ebx, ebp
        mov    edi, dword ptr [esp + 8]
        mov    ah, byte ptr [0xe67]          ; -> loc_1_0E67
        mov    al, 0x15
        VxDCall  Dev_048B.Service_0004h          ; cd2004008b04
        jb     loc_7_2D3A
loc_7_2D2D:
        mov    ebx, dword ptr [esp]
        mov    dword ptr [esi + 0x1c], ebx
        mov    dword ptr [ebx], esi
        jmp    loc_7_2CA8
loc_7_2D3A:
        or     esi, esi
        je     loc_7_2D99
        VxDCall  Dev_048B.Service_0007h          ; cd2007008b04
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [esi + 0x20], ecx
        mov    edi, dword ptr [esi + 0x10]
        mov    eax, dword ptr [esp + 8]
        push   esi
        call   sub_7_2E50
        pop    esi
        pushfd 
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
        cmp    byte ptr [esi + 0x32], 0
        jne    loc_7_2D7B
        cmp    dword ptr [0xd00], 0          ; -> obj1:0x0D00
        je     loc_7_2D7B
        push   eax
        mov    eax, 0xd00                    ; -> obj1:0x0D00
        call   sub_7_4A17
        pop    eax
loc_7_2D7B:
        popfd  
        jae    loc_7_2D2D
        call   sub_7_2E3C
        VxDCall  Dev_048B.Service_0005h          ; cd2005008b04
        mov    eax, 0xd00                    ; -> obj1:0x0D00
        call   sub_7_4A17
        stc    
        jmp    loc_7_2CB1
loc_7_2D99:
        call   sub_7_1C27
        cmp    dword ptr [ebp + 0xe4], 0
        je     loc_7_2DB7
        call   sub_7_1C84
        jae    loc_7_2D0A
        jmp    loc_7_2CB1
loc_7_2DB7:
        mov    eax, 0xd00                    ; -> obj1:0x0D00
        call   sub_7_4974
        jmp    loc_7_2D17
sub_7_2DC6:
        xor    edx, edx
        xchg   byte ptr [esi + 0x32], dl
        bsf    eax, edx
        bsr    ecx, edx
        sub    ecx, eax
        inc    ecx
        mov    edi, eax
        shl    edi, 9
        add    eax, dword ptr [esi + 0xc]
        add    edi, dword ptr [esi + 0x10]
        push   esi
        call   sub_7_2E49
        pop    esi
        pushfd 
        VxDCall  Dev_048B.Service_0008h          ; cd2008008b04
        cmp    byte ptr [esi + 0x32], 0
        jne    loc_7_2E08
        cmp    dword ptr [0xd00], 0          ; -> obj1:0x0D00
        je     loc_7_2E08
        push   eax
        mov    eax, 0xd00                    ; -> obj1:0x0D00
        call   sub_7_4A17
        pop    eax
loc_7_2E08:
        popfd  
        jb     loc_7_2E0C
loc_7_2E0B:
        ret    
loc_7_2E0C:
        call   sub_7_2E3C
        VxDCall  Dev_048B.Service_0005h          ; cd2005008b04
        bts    dword ptr [ebp + 4], 0xf
        jb     loc_7_2E2F
        call   sub_7_43D3
        jb     loc_7_2E2F
        mov    esi, 5
        call   sub_7_4B5E
loc_7_2E2F:
        mov    eax, 0xd00                    ; -> obj1:0x0D00
        call   sub_7_4A17
        stc    
        jmp    loc_7_2E0B
sub_7_2E3C:
        mov    ecx, dword ptr [esi + 0x1c]
        jecxz  loc_7_2E48
        cmp    dword ptr [ecx], esi
        jne    loc_7_2E48
        and    dword ptr [ecx], 0
loc_7_2E48:
        ret    
sub_7_2E49:
        mov    edx, 1
        jmp    sub_7_2E52
sub_7_2E50:
        xor    edx, edx
sub_7_2E52:
        push   edx
        inc    eax
        mov    edx, ecx
        mov    cl, 9
        sub    cl, byte ptr [ebp + 0x8b]
        shl    edx, cl
        shl    eax, cl
        mov    ecx, edx
        call   sub_7_2EB1
        jb     loc_7_2EA0
loc_7_2E6B:
        mov    eax, dword ptr [esi]
        sub    eax, edx
        cmp    ecx, eax
        ja     loc_7_2E7B
        add    edx, dword ptr [esi + 4]
        pop    eax
        call   dword ptr [ebp + 0x34]
        ret    
loc_7_2E7B:
        push   eax
        push   ecx
        mov    ecx, eax
        add    edx, dword ptr [esi + 4]
        mov    eax, dword ptr [esp + 8]
        call   dword ptr [ebp + 0x34]
        pop    edx
        pop    eax
        jb     loc_7_2EAE
        sub    edx, eax
        add    esi, 8
        cmp    dword ptr [ebp + 0xc0], esi
        jbe    loc_7_2EA0
        mov    ecx, edx
        xor    edx, edx
        jmp    loc_7_2E6B
loc_7_2EA0:
        mov    eax, 0x10
        mov    edx, dword ptr [ebp + 0xe0]
        mov    word ptr [edx], ax
loc_7_2EAE:
        stc    
        pop    edx
        ret    
sub_7_2EB1:
        mov    esi, dword ptr [ebp + 0xbc]
        mov    edx, eax
loc_7_2EB9:
        sub    edx, dword ptr [esi]
        jb     loc_7_2ECA
        add    esi, 8
        cmp    dword ptr [ebp + 0xc0], esi
        ja     loc_7_2EB9
        stc    
        ret    
loc_7_2ECA:
        add    edx, dword ptr [esi]
        clc    
        ret    
loc_7_2ECE:
        pushal 
        mov    esi, dword ptr [ebp + 0x240]
        mov    word ptr [esi + 0x68], ax
        mov    eax, dword ptr [ebp + 0x38]
        mov    dword ptr [esi + 8], eax
        add    edx, dword ptr [eax + 0x28]
        mov    eax, dword ptr [eax]
        movzx  eax, byte ptr [eax + 0x77]
        inc    eax
        shl    eax, 3
        lea    eax, [eax + esi + 0xbc]
        mov    dword ptr [esi + 0xac], eax
        xor    eax, eax
        mov    byte ptr [esi + 0xb0], al
        mov    dword ptr [esi + 0x64], eax
        mov    word ptr [esi + 0x6a], ax
        mov    dword ptr [esi + 0x78], eax
        mov    eax, dword ptr [ebp + 0xdc]
        and    eax, 0x829
        or     eax, 0x400
        mov    dword ptr [esi + 0x6c], eax
        mov    dword ptr [esi + 0x74], edx
        mov    dword ptr [esi + 0x7c], ecx
        cmp    word ptr [esi + 0x68], 0xd
        je     loc_7_2F82
        mov    eax, ecx
        mov    cl, byte ptr [ebp + 0x8b]
        shl    eax, cl
        mov    ecx, dword ptr [esi + 0x7c]
        or     edi, edi
        jne    loc_7_2F69
        test   dword ptr [ebp + 0xdc], 2
        jne    loc_7_2F58
        mov    edi, dword ptr [ebp + 0xd0]
        add    dword ptr [ebp + 0xd0], eax
        xor    eax, eax
        jmp    loc_7_2F69
loc_7_2F58:
        call   sub_7_3017
        or     dword ptr [esi + 0x6c], 2
        lea    edi, [esi + 0xbc]
        xor    eax, eax
loc_7_2F69:
        mov    dword ptr [esi + 0x80], edi
        add    dword ptr [esp], eax
        push   esi
        call   sub_1_096C
        pop    esi
        or     eax, eax
        je     loc_7_2F82
        or     dword ptr [esi + 0x6c], 0x40
loc_7_2F82:
        mov    al, byte ptr [ebp + 0x89]
        mov    byte ptr [esi + 0xb1], al
        mov    dword ptr [esi + 0x70], 0x2fe7 ; -> loc_7_2FE7
        xor    edx, edx
        test   byte ptr [esi + 0x6c], 8
        jne    loc_7_2FA0
        mov    edx, dword ptr [ebp + 0x3c]
loc_7_2FA0:
        mov    dword ptr [esi + 0xa8], edx
        mov    dword ptr [esi + 0x84], ebp
        lea    esi, [esi + 0x64]
        VxDCall  BLOCKDEV.BlockDev_Send_Command  ; cd2004001000
        call   sub_7_4A6C
        movzx  edx, word ptr [esi + 6]
        mov    dword ptr [esp + 0x1c], edx
        mov    word ptr [ebp + 0xf0], dx
        xor    ecx, ecx
        mov    eax, dword ptr [ebp + 0xe0]
        mov    ebx, dword ptr [ebp + 0x240]
        mov    word ptr [eax], dx
        mov    word ptr [ebx + 0xc], cx
        mov    dword ptr [ebx + 4], ecx
        cmp    edx, 0x10
        cmc    
        popal  
        ret    
loc_7_2FE7:
        lea    edx, [esi - 0x64]
        mov    esi, 0x3005                   ; -> loc_7_3005
        push   eax
        push   ebx
        push   ecx
        xor    eax, eax
        mov    ebx, eax
        mov    ecx, 0x408
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
        pop    ecx
        pop    ebx
        pop    eax
        ret    
loc_7_3005:
        mov    edx, dword ptr [edx + 0x84]
        and    dword ptr [edx + 4], 0xfffdffff
        jmp    sub_7_49ED
sub_7_3017:
        push   eax
        push   ebx
        push   ecx
        push   edx
        push   edi
        push   esi
        mov    ebx, dword ptr [ebp + 0x240]
        lea    edx, [ebx + 0xbc]
        mov    byte ptr [ebx + 0xb0], 0
        mov    esi, dword ptr [ebp + 0xd0]
loc_7_3036:
        or     ecx, ecx
        je     loc_7_308B
        mov    eax, dword ptr [esi]
        mov    edi, dword ptr [ebp + 0xc8]
        sub    eax, edi
        cmp    ecx, eax
        ja     loc_7_304A
        mov    eax, ecx
loc_7_304A:
        sub    ecx, eax
        mov    dword ptr [edx], eax
        add    dword ptr [ebp + 0xc8], eax
        mov    eax, ecx
        mov    cl, byte ptr [ebp + 0x8b]
        shl    edi, cl
        mov    ecx, eax
        add    edi, dword ptr [esi + 4]
        mov    dword ptr [edx + 4], edi
        inc    byte ptr [ebx + 0xb0]
        add    edx, 8
        mov    eax, dword ptr [ebp + 0xc8]
        cmp    dword ptr [esi], eax
        ja     loc_7_3036
        add    esi, 8
        mov    dword ptr [ebp + 0xd0], esi
        and    dword ptr [ebp + 0xc8], 0
        jmp    loc_7_3036
loc_7_308B:
        and    dword ptr [edx], 0
        pop    esi
        pop    edi
        pop    edx
        pop    ecx
        pop    ebx
        pop    eax
        ret    
loc_7_3095:
        pushal 
        push   ecx
        push   edx
        mov    edx, 2
        call   sub_7_4AD7
        pop    edx
        pop    ecx
        mov    word ptr [esi + 0x68], ax
        mov    eax, dword ptr [ebp + 0x38]
        mov    dword ptr [esi + 8], eax
        add    edx, dword ptr [eax + 0x28]
        mov    eax, dword ptr [eax]
        movzx  eax, byte ptr [eax + 0x77]
        inc    eax
        shl    eax, 3
        lea    eax, [eax + esi + 0xbc]
        mov    dword ptr [esi + 0xac], eax
        xor    eax, eax
        mov    byte ptr [esi + 0xb0], al
        mov    dword ptr [esi + 0x64], eax
        mov    word ptr [esi + 0x6a], ax
        mov    dword ptr [esi + 0x78], eax
        mov    dword ptr [esi + 0x6c], 0x500
        mov    dword ptr [esi + 0x74], edx
        mov    dword ptr [esi + 0x7c], ecx
        mov    eax, ecx
        mov    cl, byte ptr [ebp + 0x8b]
        shl    eax, cl
        mov    ecx, dword ptr [esi + 0x7c]
        cmp    word ptr [esi + 0x68], 0xd
        je     loc_7_3114
        mov    dword ptr [esi + 0x80], edi
        add    dword ptr [esp], eax
        push   esi
        call   sub_1_096C
        pop    esi
        or     eax, eax
        je     loc_7_3114
        or     dword ptr [esi + 0x6c], 0x40
loc_7_3114:
        mov    al, byte ptr [ebp + 0x89]
        mov    byte ptr [esi + 0xb1], al
        xor    eax, eax
        test   byte ptr [esi + 0x6c], 8
        jne    loc_7_312B
        mov    eax, dword ptr [ebp + 0x3c]
loc_7_312B:
        mov    dword ptr [esi + 0xa8], eax
        lea    esi, [esi + 0x64]
        VxDCall  BLOCKDEV.BlockDev_Send_Command  ; cd2004001000
        and    dword ptr [ebp + 4], 0xfffdffff
        movzx  edx, word ptr [esi + 6]
        mov    word ptr [ebp + 0xf0], dx
        lea    esi, [esi - 0x64]
        call   sub_7_4B35
        cmp    edx, 0x10
        cmc    
        mov    dword ptr [esp + 0x1c], edx
        popal  
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h
loc_7_316A:
        push   edx
        inc    ebp
        inc    ecx
        inc    esp
        push   esp
        dec    eax
        dec    ecx
        push   ebx
        push   esp
        pop    eax
        push   esp
loc_7_3175:
        inc    esp
        push   edx
        push   esi
        push   ebx
        push   eax
        inc    ecx
        inc    ebx
        inc    ebp
        xor    byte ptr [eax], dh
        xor    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_318D:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_3191:
        add    byte ptr [eax], al
loc_7_3193:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
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
loc_7_31C0:
        cmp    dword ptr [ebx + 0x8c], -1
        jne    loc_7_31F0
        test   byte ptr [ebp + 4], 0x80
        jne    loc_7_31F0
        and    dword ptr [ebp + 8], 0
        and    byte ptr [ebp + 4], 0xbf
        push   ebx
        call   sub_7_31F8
        pop    ebx
        sbb    edx, edx
        and    eax, edx
        mov    dword ptr [ebx + 0xa0], eax
        and    edx, 0x1a
        mov    word ptr [ebx + 0x6a], dx
loc_7_31EF:
        ret    
loc_7_31F0:
        mov    word ptr [ebx + 0x6a], 0x21
        jmp    loc_7_31EF
sub_7_31F8:
        and    dword ptr [ebp + 4], 0xfff95ffd
        or     dword ptr [ebp + 4], 0x1010
        mov    eax, 0x36d8                   ; -> obj7:0x36D8
        call   sub_7_490C
        jb     loc_7_33FF
        mov    esi, 0x36e4                   ; -> obj7:0x36E4
        xor    edx, edx
        mov    ecx, 1
        mov    edi, esi
        call   sub_7_3415
        jb     loc_7_3406
        cmp    byte ptr [esi], 0xe9
        je     loc_7_3247
        cmp    byte ptr [esi], 0xeb
        jne    loc_7_3406
        cmp    byte ptr [esi + 2], 0x90
        jne    loc_7_3406
loc_7_3247:
        cmp    word ptr [esi + 0xb], 0x200
        jne    loc_7_3406
        cmp    byte ptr [esi + 0xd], 0
        je     loc_7_3406
        mov    edi, 0x3180                   ; -> obj7:0x3180
        mov    ecx, 0x24
        cmp    word ptr [esi + 0x16], 0
        jne    loc_7_3271
        add    ecx, 0x1c
loc_7_3271:
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        movzx  ecx, byte ptr [0x318d]        ; -> loc_7_318D
        bsf    ecx, ecx
        mov    byte ptr [0x3169], cl         ; -> obj7:0x3169
        movzx  eax, word ptr [0x3196]        ; -> obj7:0x3196
        or     eax, eax
        je     loc_7_32FA
        movzx  ecx, byte ptr [0x3190]        ; -> obj7:0x3190
        mul    ecx
        movzx  edx, word ptr [0x318e]        ; -> obj7:0x318E
        add    eax, edx
        movzx  edx, word ptr [0x3191]        ; -> loc_7_3191
        shr    edx, 4
        add    eax, edx
        mov    dword ptr [0x3160], eax       ; -> obj7:0x3160
        mov    byte ptr [0x3168], 2          ; -> obj7:0x3168
        cmp    word ptr [0x3196], 0xc        ; -> obj7:0x3196
        jb     loc_7_331D
        movzx  eax, word ptr [0x3193]        ; -> loc_7_3193
        or     eax, eax
        jne    loc_7_32D3
        mov    eax, dword ptr [0x31a0]       ; -> obj7:0x31A0
loc_7_32D3:
        sub    eax, dword ptr [0x3160]       ; -> obj7:0x3160
        jb     loc_7_3406
        xor    edx, edx
        movzx  ecx, byte ptr [0x318d]        ; -> loc_7_318D
        div    ecx
        cmp    eax, 0xff6
        jb     loc_7_331D
        mov    byte ptr [0x3168], 0          ; -> obj7:0x3168
        jmp    loc_7_331D
loc_7_32FA:
        mov    byte ptr [0x3168], 1          ; -> obj7:0x3168
        movzx  ecx, byte ptr [0x3190]        ; -> obj7:0x3190
        mov    eax, dword ptr [0x31a4]       ; -> obj7:0x31A4
        mul    ecx
        movzx  edx, word ptr [0x318e]        ; -> obj7:0x318E
        add    eax, edx
        mov    dword ptr [0x3160], eax       ; -> obj7:0x3160
loc_7_331D:
        mov    esi, 0x3175                   ; -> loc_7_3175
        movzx  eax, byte ptr [ebp + 0xb2]
        mov    cl, 0xa
        div    cl
        add    ah, 0x30
        mov    byte ptr [esi + 0xa], ah
        xor    ah, ah
        div    cl
        add    ah, 0x30
        mov    byte ptr [esi + 9], ah
        add    al, 0x30
        mov    byte ptr [esi + 8], al
        mov    word ptr [esi + 1], 0x5652
        mov    edi, 0x36e4                   ; -> obj7:0x36E4
        call   sub_7_341F
        jae    loc_7_336A
        mov    word ptr [esi + 1], 0x4c42
        mov    edi, 0x36e4                   ; -> obj7:0x36E4
        call   sub_7_341F
        jb     loc_7_3406
loc_7_336A:
        test   byte ptr [edi + 0xb], 0x18
        jne    loc_7_3406
        mov    eax, dword ptr [edi + 0x1c]
        shr    eax, 9
        je     loc_7_3406
        mov    dword ptr [ebp + 0x50], eax
        xor    ebx, ebx
        cmp    word ptr [0x3196], 0          ; -> obj7:0x3196
        jne    loc_7_3392
        mov    ebx, dword ptr [edi + 0x12]
loc_7_3392:
        mov    bx, word ptr [edi + 0x1a]
        or     ebx, ebx
        je     loc_7_3406
        call   sub_7_34F6
        jb     loc_7_3406
        or     eax, 0xffffffff
        lea    ecx, [eax + 2]
        mov    edi, 0x36e4                   ; -> obj7:0x36E4
        call   sub_7_2E50
        jb     loc_7_33F0
        movzx  edi, byte ptr [ebp + 0x89]
        mov    esi, 0x36e4                   ; -> obj7:0x36E4
        or     ebx, 0xffffffff
        call   sub_7_0147
        jb     loc_7_340E
        call   sub_7_02EE
        jb     loc_7_340E
        mov    edi, 0x36e4                   ; -> obj7:0x36E4
        mov    esi, 0x316a                   ; -> loc_7_316A
        call   sub_7_341F
        cmc    
        sbb    eax, eax
        and    eax, 0x40000
        or     dword ptr [ebp + 4], eax
        and    dword ptr [ebp + 4], 0xffffffef
        xor    eax, eax
loc_7_33F0:
        pushfd 
        push   eax
        mov    eax, 0x36d8                   ; -> obj7:0x36D8
        call   sub_7_493D
        pop    eax
        popfd  
loc_7_33FE:
        ret    
loc_7_33FF:
        mov    eax, 8
        jmp    loc_7_33FE
loc_7_3406:
        stc    
        mov    eax, 2
        jmp    loc_7_33F0
loc_7_340E:
        mov    eax, 0xb
        jmp    loc_7_33F0
sub_7_3415:
        mov    eax, 0
        jmp    loc_7_2ECE
sub_7_341F:
        cmp    byte ptr [0x3168], 1          ; -> obj7:0x3168
        je     loc_7_347D
        movzx  ebx, word ptr [0x3191]        ; -> loc_7_3191
        add    ebx, 0xf
        shr    ebx, 4
        mov    edx, dword ptr [0x3160]       ; -> obj7:0x3160
        sub    edx, ebx
loc_7_343D:
        dec    ebx
        js     loc_7_347B
        push   edx
        push   edi
        mov    ecx, 1
        call   sub_7_3415
        pop    edi
        pop    edx
        jb     loc_7_347B
        inc    edx
        mov    ecx, 0x10
        push   edi
        cld    
loc_7_3458:
        cmp    byte ptr [edi], 0
        je     loc_7_347A
        push   ecx
        push   edi
        push   esi
        mov    ecx, 0xb
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        pop    esi
        pop    edi
        pop    ecx
        je     loc_7_3474
        add    edi, 0x20
        loop   loc_7_3458
        pop    edi
        jmp    loc_7_343D
loc_7_3474:
        lea    esp, [esp + 4]
        jmp    loc_7_347C
loc_7_347A:
        pop    edi
loc_7_347B:
        stc    
loc_7_347C:
        ret    
loc_7_347D:
        mov    ebx, dword ptr [0x31ac]       ; -> obj7:0x31AC
loc_7_3483:
        mov    edx, ebx
        sub    edx, 2
        mov    cl, byte ptr [0x3169]         ; -> obj7:0x3169
        shl    edx, cl
        add    edx, dword ptr [0x3160]       ; -> obj7:0x3160
        movzx  ecx, byte ptr [0x318d]        ; -> loc_7_318D
loc_7_349D:
        push   ecx
        push   edi
        mov    ecx, 1
        call   sub_7_3415
        pop    edi
        pop    ecx
        jb     loc_7_347C
        inc    edx
        push   ecx
        push   edi
        mov    ecx, 0x10
        cld    
loc_7_34B6:
        cmp    byte ptr [edi], 0
        je     loc_7_34F1
        push   ecx
        push   edi
        push   esi
        mov    ecx, 0xb
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        pop    esi
        pop    edi
        pop    ecx
        je     loc_7_34EB
        add    edi, 0x20
        loop   loc_7_34B6
        pop    edi
        pop    ecx
        loop   loc_7_349D
        push   edi
        call   sub_7_3564
        pop    edi
        jb     loc_7_347C
        cmp    ebx, 0xffffff8
        jae    loc_7_347B
        cmp    ebx, 1
        jb     loc_7_347C
        jmp    loc_7_3483
loc_7_34EB:
        lea    esp, [esp + 8]
        jmp    loc_7_347C
loc_7_34F1:
        add    esp, 8
        jmp    loc_7_347B
sub_7_34F6:
        or     dword ptr [0x3164], 0xffffffff ; -> obj7:0x3164
        mov    esi, dword ptr [ebp + 0xbc]
        or     ebx, ebx
        je     loc_7_3559
loc_7_3507:
        cmp    dword ptr [ebp + 0xc4], esi
        jbe    loc_7_3561
        cmp    ebx, 2
        jb     loc_7_3561
        mov    eax, ebx
        sub    eax, 2
        mov    cl, byte ptr [0x3169]         ; -> obj7:0x3169
        shl    eax, cl
        add    eax, dword ptr [0x3160]       ; -> obj7:0x3160
        mov    dword ptr [esi + 4], eax
        mov    dword ptr [esi], 0
loc_7_3530:
        movzx  eax, byte ptr [0x318d]        ; -> loc_7_318D
        add    dword ptr [esi], eax
        push   ebx
        push   esi
        call   sub_7_3564
        pop    esi
        pop    eax
        jb     loc_7_3561
        cmp    ebx, 0xffffff8
        jae    loc_7_3556
        inc    eax
        cmp    eax, ebx
        je     loc_7_3530
        add    esi, 8
        jmp    loc_7_3507
loc_7_3556:
        add    esi, 8
loc_7_3559:
        mov    dword ptr [ebp + 0xc0], esi
        clc    
loc_7_3560:
        ret    
loc_7_3561:
        stc    
        jmp    loc_7_3560
sub_7_3564:
        cmp    byte ptr [0x3168], 1          ; -> obj7:0x3168
        je     loc_7_35C1
        ja     loc_7_361D
        lea    edx, [ebx + ebx]
        shr    edx, 9
        movzx  eax, word ptr [0x318e]        ; -> obj7:0x318E
        add    edx, eax
        cmp    dword ptr [0x3164], edx       ; -> obj7:0x3164
        je     loc_7_35A1
        mov    dword ptr [0x3164], edx       ; -> obj7:0x3164
        mov    edi, 0x36e4                   ; -> obj7:0x36E4
        mov    ecx, 1
        call   sub_7_3415
        jb     loc_7_35C0
loc_7_35A1:
        lea    edx, [ebx + ebx]
        and    edx, 0x1ff
        movzx  ebx, word ptr [edx + 0x36e4]  ; -> obj7:0x36E4
        cmp    ebx, 0xfff8
        jb     loc_7_35BF
        or     ebx, 0xfff0000
loc_7_35BF:
        clc    
loc_7_35C0:
        ret    
loc_7_35C1:
        lea    edx, [ebx*4]
        shr    edx, 9
        movzx  eax, word ptr [0x318e]        ; -> obj7:0x318E
        test   byte ptr [0x31a8], 0xf        ; -> obj7:0x31A8
        je     loc_7_35E1
        add    eax, dword ptr [0x31a4]       ; -> obj7:0x31A4
loc_7_35E1:
        add    edx, eax
        cmp    dword ptr [0x3164], edx       ; -> obj7:0x3164
        je     loc_7_3602
        mov    dword ptr [0x3164], edx       ; -> obj7:0x3164
        mov    edi, 0x36e4                   ; -> obj7:0x36E4
        mov    ecx, 1
        call   sub_7_3415
        jb     loc_7_35C0
loc_7_3602:
        lea    edx, [ebx*4]
        and    edx, 0x1ff
        mov    ebx, dword ptr [edx + 0x36e4] ; -> obj7:0x36E4
        and    ebx, 0xfffffff
        jmp    loc_7_35C0
loc_7_361D:
        mov    eax, ebx
        xor    edx, edx
        mov    ecx, 0x400
        div    ecx
        push   edx
        lea    edx, [eax + eax*2]
        movzx  eax, word ptr [0x318e]        ; -> obj7:0x318E
        add    edx, eax
        cmp    dword ptr [0x3164], edx       ; -> obj7:0x3164
        je     loc_7_3652
        mov    dword ptr [0x3164], edx       ; -> obj7:0x3164
        mov    edi, 0x36e4                   ; -> obj7:0x36E4
        mov    ecx, 3
        call   sub_7_3415
loc_7_3652:
        pop    edx
        jb     loc_7_35C0
        lea    eax, [edx + edx*2]
        shr    eax, 1
        movzx  ebx, word ptr [eax + 0x36e4]  ; -> obj7:0x36E4
        test   dl, 1
        je     loc_7_366D
        shr    ebx, 4
loc_7_366D:
        and    ebx, 0xfff
        cmp    ebx, 0xff0
        je     loc_7_3687
        cmp    ebx, 0xff8
        jb     loc_7_35BF
loc_7_3687:
        or     ebx, 0xfffffff
        jmp    loc_7_35C0
sub_7_3692:
        push   edi
        sub    esi, esp
        push   esi
        push   ecx
        push   edx
        mov    eax, edx
        shr    eax, 9
        dec    eax
        mov    ecx, 1
        mov    edi, 0x38e4                   ; -> obj7:0x38E4
        call   sub_7_2E50
        jb     loc_7_36D3
        pop    edx
        pop    ecx
        pop    edi
        add    edi, esp
        push   edi
        push   ecx
        push   edx
        and    edx, 0x1ff
        lea    eax, [edx + ecx]
        cmp    eax, 0x201
        cmc    
        jb     loc_7_36D3
        mov    eax, ecx
        lea    esi, [edx + 0x38e4]           ; -> obj7:0x38E4
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_7_36D3:
        pop    edx
        pop    ecx
        pop    esi
        pop    edi
        ret    
        db  000h, 000h, 000h, 000h
        dd  obj7:0x36D8              ; fixup
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
        db  000h, 000h, 000h, 000h
loc_7_3CE4:
        mov    eax, dword ptr [ebp + 0x38]
        or     dword ptr [ebp + 4], 0x800
        test   byte ptr [ebp + 5], 0x20
        jne    loc_7_3CFF
        call   sub_7_100C
        jb     loc_7_3E85
loc_7_3CFF:
        cmp    dword ptr [ebx + 0x78], 0
        jne    loc_7_3E7F
        mov    eax, dword ptr [ebx + 0x7c]
        mov    dword ptr [ebp + 0xcc], eax
        mov    edx, dword ptr [ebx + 0x80]
        test   byte ptr [ebx + 0x6c], 2
        jne    loc_7_3D72
loc_7_3D1E:
        mov    dword ptr [ebp + 0xd0], edx
        mov    dword ptr [ebp + 0xc8], 0
        mov    edx, dword ptr [ebx + 0x74]
        test   byte ptr [ebx + 0x6e], 8
        jne    loc_7_3D43
        mov    ecx, dword ptr [ebp + 0x38]
        sub    edx, dword ptr [ecx + 0x28]
        jb     loc_7_3E7F
loc_7_3D43:
        mov    dword ptr [ebp + 0xd4], edx
        add    eax, edx
        jb     loc_7_3E7F
        cmp    dword ptr [ebp + 0xae], eax
        jb     loc_7_3E7F
        cmp    dword ptr [ebp + 0x74], edx
        ja     loc_7_3D82
loc_7_3D62:
        cmp    word ptr [ebx + 0x68], 0
        je     loc_7_1D34
        jmp    loc_7_20D5
loc_7_3D72:
        cmp    dword ptr [edx], eax
        jne    loc_7_3D1E
        mov    edx, dword ptr [edx + 4]
        and    dword ptr [ebp + 0xdc], 0xfffffffd
        jmp    loc_7_3D1E
loc_7_3D82:
        xor    ecx, ecx
        sub    eax, dword ptr [ebp + 0x74]
        jbe    loc_7_3D91
        mov    ecx, eax
        sub    dword ptr [ebp + 0xcc], eax
loc_7_3D91:
        push   ecx
        movzx  eax, word ptr [ebp + 0x9c]
        sub    edx, eax
        jae    loc_7_3DB4
        neg    edx
        cmp    dword ptr [ebx + 0x7c], edx
        ja     loc_7_3DA7
        mov    edx, dword ptr [ebx + 0x7c]
loc_7_3DA7:
        mov    ecx, edx
        call   sub_7_47D7
        jbe    loc_7_3E66
loc_7_3DB4:
        mov    eax, dword ptr [ebp + 0xd4]
        movzx  ecx, word ptr [ebp + 0x9c]
        sub    eax, ecx
        movzx  ecx, word ptr [ebp + 0xa4]
        sub    eax, ecx
        jae    loc_7_3DF5
        neg    eax
        mov    ecx, dword ptr [ebp + 0xcc]
        cmp    ecx, eax
        jb     loc_7_3DDC
        mov    ecx, eax
loc_7_3DDC:
        cmp    word ptr [ebx + 0x68], 0
        je     loc_7_3DEE
        push   ebx
        push   ecx
        call   sub_7_4407
        pop    ecx
        pop    ebx
        jb     loc_7_3E66
loc_7_3DEE:
        call   sub_7_47D7
        jbe    loc_7_3E66
loc_7_3DF5:
        mov    eax, dword ptr [ebp + 0xd4]
        movzx  ecx, word ptr [ebp + 0x9c]
        sub    eax, ecx
        movzx  ecx, word ptr [ebp + 0xa4]
        sub    eax, ecx
        sub    dword ptr [ebp + 0xd4], ecx
        sub    eax, ecx
        jae    loc_7_3E5B
        neg    eax
        mov    ecx, dword ptr [ebp + 0xcc]
        cmp    ecx, eax
        jb     loc_7_3E25
        mov    ecx, eax
loc_7_3E25:
        cmp    word ptr [ebx + 0x68], 0
        je     loc_7_3E54
        test   dword ptr [ebp + 0xdc], 2
        je     loc_7_3E3F
        call   sub_7_3017
        jmp    loc_7_3E4A
loc_7_3E3F:
        mov    edx, ecx
        shl    edx, 9
        add    dword ptr [ebp + 0xd0], edx
loc_7_3E4A:
        sub    dword ptr [ebp + 0xcc], ecx
        je     loc_7_3E66
        jmp    loc_7_3E5B
loc_7_3E54:
        call   sub_7_47D7
        jbe    loc_7_3E66
loc_7_3E5B:
        mov    ecx, dword ptr [ebp + 0xcc]
        call   sub_7_47D7
loc_7_3E66:
        pop    ecx
        jb     loc_7_3E85
        jecxz  loc_7_3E85
        mov    dword ptr [ebp + 0xcc], ecx
        mov    eax, dword ptr [ebp + 0x74]
        mov    dword ptr [ebp + 0xd4], eax
        jmp    loc_7_3D62
loc_7_3E7F:
        mov    word ptr [ebx + 0x6a], 0x10
loc_7_3E85:
        ret    
loc_7_3E86:
        movzx  eax, word ptr [ebx + 0x92]
        cmp    ax, 0x4404
        jne    loc_1_08CC
        cmp    dword ptr [ebx + 0x94], 5
        jb     loc_1_08CC
        mov    edi, dword ptr [ebx + 0x98]
        cmp    word ptr [edi], 0x444d
        jne    loc_1_08CC
        push   edi
        mov    al, byte ptr [edi + 2]
        cmp    al, 0x46
        je     loc_7_3ED3
        cmp    al, 0x49
        je     loc_7_3EDD
        cmp    al, 0x72
        je     loc_7_3EEC
        cmp    al, 0x73
        je     loc_7_3EFE
        cmp    al, 0x77
        je     loc_7_3F46
        stc    
        jmp    loc_7_3F5F
loc_7_3ED3:
        call   sub_1_093E
        jmp    loc_7_3F5F
loc_7_3EDD:
        call   sub_1_093E
        jb     loc_7_3F5F
        call   sub_7_4687
        clc    
        jmp    loc_7_3F5F
loc_7_3EEC:
        push   edi
        push   ebx
        call   sub_1_093E
        pop    ebx
        pop    esi
        jb     loc_7_3F5F
        call   sub_7_13F2
        jmp    loc_7_3F5F
loc_7_3EFE:
        cmp    dword ptr [ebx + 0x94], 0x11
        jb     loc_7_3F5F
        call   sub_1_093E
        jb     loc_7_3F5F
        call   sub_7_100C
        jb     loc_7_3F5F
        mov    edi, dword ptr [esp]
        mov    eax, dword ptr [ebp + 0x54]
        mov    dword ptr [edi + 5], eax
        mov    eax, dword ptr [ebp + 0x58]
        mov    dword ptr [edi + 9], eax
        mov    eax, dword ptr [ebp + 0x64]
        cmp    word ptr [ebp + 0xb4], 3
        jb     loc_7_3F40
        movzx  ecx, byte ptr [ebp + 0x9b]
        inc    ecx
        mov    eax, dword ptr [ebp + 0x58]
        xor    edx, edx
        div    ecx
loc_7_3F40:
        mov    dword ptr [edi + 0xd], eax
        clc    
        jmp    loc_7_3F5F
loc_7_3F46:
        push   edi
        push   ebx
        call   sub_1_093E
        pop    ebx
        pop    esi
        jb     loc_7_3F5F
        push   esi
        xor    edx, edx
        call   sub_7_470E
        pop    esi
        call   sub_7_157A
loc_7_3F5F:
        pop    edi
        jb     loc_7_3F68
        mov    word ptr [edi + 3], 0x4f4b
loc_7_3F68:
        ret    
loc_7_3F69:
        movzx  eax, word ptr [ebx + 0x92]
        cmp    ah, 0xdb
        jne    loc_7_3F83
        cmp    al, 0xc
        jae    loc_7_3F83
        movzx  eax, al
        jmp    dword ptr [eax*4 + 0x3f8c]    ; -> obj7:0x3F8C
loc_7_3F83:
        mov    word ptr [ebx + 0x6a], 0x16
        ret    
        db  08Bh, 0C0h
        ; ---- dispatch table (12 entries) ----
        dd  loc_7_3FBC
        dd  loc_7_31C0
        dd  loc_7_3FC4
        dd  loc_7_400E
        dd  loc_7_3F83
        dd  loc_7_3F83
        dd  loc_7_3F83
        dd  loc_7_3F83
        dd  loc_7_4046
        dd  loc_7_40CB
        dd  loc_7_415A
        dd  loc_7_4299
loc_7_3FBC:
        and    dword ptr [ebx + 0xa0], 0
        ret    
loc_7_3FC4:
        call   sub_7_100C
        push   ebx
        call   sub_7_1B8C
        pop    ebx
        mov    eax, dword ptr [ebp + 0x58]
        cmp    eax, 0x80
        cmc    
        sbb    ecx, ecx
        and    eax, ecx
        mov    cl, byte ptr [ebp + 0x8c]
        shr    eax, cl
        cmp    dword ptr [0xe2c], 1          ; -> loc_1_0E2C
        jbe    loc_7_3FFA
        mul    dword ptr [ebp + 0xf4]
        div    dword ptr [ebp + 0xf8]
loc_7_3FFA:
        cmp    eax, 0x10000
        cmc    
        sbb    ecx, ecx
        or     eax, ecx
        movzx  eax, ax
        mov    dword ptr [ebx + 0xa0], eax
        ret    
loc_7_400E:
        xor    eax, eax
        test   byte ptr [ebp + 5], 0x80
        jne    loc_7_403F
        cmp    dword ptr [ebp + 0x58], -1
        je     loc_7_403F
        push   ebx
        call   sub_7_1B8C
        pop    ebx
        mov    eax, dword ptr [ebp + 0x64]
        cmp    word ptr [ebp + 0xb4], 3
        jb     loc_7_403F
        movzx  ecx, byte ptr [ebp + 0x9b]
        inc    ecx
        mov    eax, dword ptr [ebp + 0x58]
        xor    edx, edx
        div    ecx
loc_7_403F:
        mov    dword ptr [ebx + 0xa0], eax
        ret    
loc_7_4046:
        and    dword ptr [ebx + 0xa0], 0
        cmp    word ptr [ebp + 0xb4], 3
        jb     loc_7_40AC
        mov    eax, dword ptr [ebx + 0x94]
        cmp    eax, 2
        jb     loc_7_40B2
        add    eax, dword ptr [ebp + 0x78]
        cmp    dword ptr [ebp + 0x7c], eax
        jbe    loc_7_40B2
        cmp    dword ptr [ebp + 8], eax
        je     loc_7_40B8
        mov    edi, eax
        shl    edi, 8
        add    edi, 0x10000000
        push   eax
        mov    ebx, ebp
        mov    ah, byte ptr [0xe68]          ; -> obj1:0x0E68
        xor    al, al
        VxDCall  Dev_048B.Service_0004h          ; cd2004008b04
        pop    eax
        mov    ebx, dword ptr [ebp + 0x48]
        jae    loc_7_40B8
        push   eax
        call   sub_7_2933
        pop    edx
        jb     loc_7_40AB
        test   eax, 0x80000000
        jne    loc_7_40B8
        and    eax, 0xff000000
        call   sub_7_29FE
loc_7_40AB:
        ret    
loc_7_40AC:
        inc    dword ptr [ebx + 0xa0]
loc_7_40B2:
        inc    dword ptr [ebx + 0xa0]
loc_7_40B8:
        inc    dword ptr [ebx + 0xa0]
        mov    eax, dword ptr [ebp + 0xe0]
        mov    word ptr [eax], 0x16
        jmp    loc_7_40AB
loc_7_40CB:
        and    dword ptr [ebx + 0xa0], 0
        mov    ecx, dword ptr [ebx + 0x98]
        or     ecx, ecx
        je     loc_7_4159
        mov    eax, dword ptr [ebp + 0xe0]
        mov    word ptr [eax], 0x10
        mov    edx, dword ptr [ebx + 0x94]
        add    edx, dword ptr [ebp + 0x78]
        lea    esi, [ecx + edx]
        cmp    dword ptr [ebp + 0x7c], esi
        jb     loc_7_4159
        mov    word ptr [eax], 0
        mov    ebx, ecx
        test   byte ptr [ebp + 4], 0x80
        je     loc_7_411A
        cmp    dword ptr [ebp + 8], edx
        jb     loc_7_411A
        cmp    dword ptr [ebp + 8], esi
        jae    loc_7_411A
        push   ebx
        push   edx
        call   sub_7_1B8C
        pop    edx
        pop    ebx
        jb     loc_7_4159
loc_7_411A:
        push   edx
        mov    eax, edx
        call   sub_7_2933
        pop    edx
        jb     loc_7_4159
        test   eax, 0x80000000
        je     loc_7_4155
        push   ebx
        push   edx
        xor    ebx, ebx
        call   sub_7_0CB0
        pop    edx
        pop    ebx
        jb     loc_7_4159
        mov    ecx, dword ptr [ebp + 0xfc]
        mov    esi, dword ptr [ebp + 0x48]
loc_7_4142:
        movzx  eax, byte ptr [ebp + ecx*4 + 0xff]
        shl    eax, 9
        add    dword ptr [esi + 0xa0], eax
        loop   loc_7_4142
loc_7_4155:
        inc    edx
        dec    ebx
        jne    loc_7_411A
loc_7_4159:
        ret    
loc_7_415A:
        sub    esp, 0x10
        and    dword ptr [ebx + 0xa0], 0
        mov    dword ptr [esp + 0xc], ebx
        call   sub_7_1B8C
        mov    ebx, dword ptr [esp + 0xc]
        jb     loc_7_4216
        and    dword ptr [ebp + 8], 0
        and    byte ptr [ebp + 4], 0xbf
        mov    eax, dword ptr [ebx + 0x94]
        cmp    eax, 2
        jb     loc_7_423C
        add    eax, dword ptr [ebp + 0x78]
        cmp    dword ptr [ebp + 0x7c], eax
        jbe    loc_7_423C
        mov    edx, eax
        call   sub_7_470E
        push   eax
        call   sub_7_2933
        pop    edx
        jb     loc_7_4216
        or     dword ptr [ebp + 4], 0x100
        movzx  esi, byte ptr [ebp + 0x9b]
        mov    dword ptr [ebp + 0x18], esi
        test   eax, 0x80000000
        je     loc_7_41E3
        cmp    byte ptr [ebp + 0x9b], ch
        ja     loc_7_41D8
        test   eax, 0x20000000
        jne    loc_7_41D8
        test   eax, 0x40000000
        jne    loc_7_4216
loc_7_41D8:
        mov    eax, edx
        xor    ecx, ecx
        call   sub_7_45E9
        jb     loc_7_4216
loc_7_41E3:
        push   edx
        call   sub_7_04C8
        pop    edx
        jb     loc_7_4232
        mov    eax, dword ptr [ebp + 0x100]
        shld   ecx, eax, 0x10
        mov    cl, byte ptr [ebp + 0x9b]
        and    eax, 0xffffff
        cmp    dword ptr [ebp + 0xfc], 2
        jae    loc_7_4248
loc_7_420A:
        or     eax, 0xc0000000
        call   sub_7_29FE
        jb     loc_7_421B
loc_7_4216:
        lea    esp, [esp + 0x10]
        ret    
loc_7_421B:
        or     dword ptr [ebp + 0x58], 0xffffffff
        or     dword ptr [ebp + 0x64], 0xffffffff
        btr    dword ptr [ebp + 4], 0xd
        mov    ebx, dword ptr [esp + 0xc]
        inc    dword ptr [ebx + 0xa0]
loc_7_4232:
        mov    ebx, dword ptr [esp + 0xc]
        inc    dword ptr [ebx + 0xa0]
loc_7_423C:
        mov    ebx, dword ptr [esp + 0xc]
        inc    dword ptr [ebx + 0xa0]
        jmp    loc_7_4216
loc_7_4248:
        mov    dword ptr [esp + 8], edx
        mov    dword ptr [esp + 4], eax
        mov    dword ptr [esp], ecx
        call   0x484c
        jb     loc_7_421B
        mov    esi, edi
        call   sub_7_26EB
        mov    edx, 0x45544550
loc_7_4266:
        mov    dword ptr [eax], edx
        add    eax, 4
        loop   loc_7_4266
        mov    eax, dword ptr [esp + 4]
        mov    ecx, 1
        mov    edi, esi
        call   sub_7_2E49
        jb     loc_7_421B
        call   sub_7_489D
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [esp]
        mov    edx, dword ptr [esp + 8]
        or     eax, 0x20000000
        jmp    loc_7_420A
loc_7_4299:
        call   sub_7_100C
        push   ebx
        call   sub_7_1B8C
        pop    ebx
        mov    eax, dword ptr [ebp + 0x58]
        mov    dword ptr [ebx + 0x94], eax
        mov    eax, dword ptr [ebp + 0x54]
        mov    dword ptr [ebx + 0xa0], eax
        ret    
loc_7_42B8:
        sub    esp, 8
        mov    edx, dword ptr [esp + 0xc]
        mov    eax, dword ptr [edx + 0x14]
        mov    eax, dword ptr [eax + 4]
        mov    ecx, 0xe44                    ; -> obj1:0x0E44
loc_7_42CA:
        mov    ecx, dword ptr [ecx]
        or     ecx, ecx
        je     loc_7_4331
        cmp    ecx, eax
        jne    loc_7_42CA
        mov    cl, byte ptr [eax + 0x89]
        cmp    byte ptr [edx + 0xb1], cl
        je     loc_7_42F9
        mov    ecx, dword ptr [edx + 8]
        test   byte ptr [ecx + 0x1d], 0x20
        je     loc_7_4331
        mov    cl, byte ptr [eax + 0x8a]
        cmp    byte ptr [edx + 0xb1], cl
        jne    loc_7_4331
loc_7_42F9:
        mov    cl, byte ptr [eax + 0x88]
        mov    byte ptr [edx + 0xb1], cl
        mov    dword ptr [esp + 4], edi
        mov    dword ptr [esp], esi
        mov    edi, dword ptr [edx + 0x98]
        lea    edi, [edi + 7]
        lea    esi, [eax + 0x99]
        mov    ecx, 0x19
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        sub    edi, 0x19
        mov    byte ptr [edi + 5], 2
        mov    edi, dword ptr [esp + 4]
        mov    esi, dword ptr [esp]
loc_7_4331:
        mov    eax, dword ptr [edx + 0x14]
        sub    eax, 8
        mov    dword ptr [edx + 0x14], eax
        push   edx
        call   dword ptr [eax]
        add    esp, 4
        add    esp, 8
        ret    
sub_7_4344:
        cmp    dword ptr [0xd0c], 0          ; -> loc_1_0D0C
        jne    loc_7_43A3
loc_7_434D:
        cmp    dword ptr [ebp + 0xe4], 0
        jne    loc_7_43AC
loc_7_4356:
        pop    edx
        lea    eax, [ebp + 0x40]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [eax + 8], ecx
        or     ecx, ecx
        jne    loc_7_4365
        jmp    edx
loc_7_4365:
        xor    esi, esi
        xchg   dword ptr [ecx + 0x8c], esi
        mov    dword ptr [eax], esi
        or     esi, esi
        jne    loc_7_4376
        mov    dword ptr [eax + 4], eax
loc_7_4376:
        cmp    ebx, 0xffffff44
        je     loc_7_4385
        push   0x43cb                        ; -> loc_7_43CB
        jmp    edx
loc_7_4385:
        push   edx
        mov    edx, ebp
        mov    esi, 0x43c9                   ; -> loc_7_43C9
        push   eax
        push   ebx
        push   ecx
        xor    eax, eax
        mov    ebx, eax
        mov    ecx, 0x408
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
        pop    ecx
        pop    ebx
        pop    eax
        ret    
loc_7_43A3:
        push   ebx
        call   sub_7_1B8C
        pop    ebx
        jmp    loc_7_434D
loc_7_43AC:
        push   ebx
        mov    eax, dword ptr [ebp + 0xe0]
        push   dword ptr [eax]
        call   sub_7_1C84
        pop    edx
        pop    ebx
        jb     loc_7_4356
        mov    eax, dword ptr [ebp + 0xe0]
        mov    word ptr [eax], dx
        jmp    loc_7_4356
loc_7_43C9:
        mov    ebp, edx
loc_7_43CB:
        mov    edx, dword ptr [ebp + 0x48]
        jmp    loc_1_0534
sub_7_43D3:
        push   eax
        push   ecx
        push   edx
        sub    esp, 4
        xor    eax, eax
        lea    ecx, [esp]
        movzx  edx, byte ptr [ebp + 0x88]
        cmp    dl, 0xff
        je     loc_7_43FF
        push   eax
        push   eax
        push   ecx
        push   edx
        VxDCall  IFSMgr.IFSMgr_GetLockState      ; cd206f004000
        add    esp, 0x10
        or     eax, eax
        jne    loc_7_43FF
        cmp    dword ptr [esp], -1
loc_7_43FF:
        lea    esp, [esp + 4]
        pop    edx
        pop    ecx
        pop    eax
        ret    
sub_7_4407:
        mov    eax, dword ptr [ebp + 0xd4]
        mov    esi, dword ptr [ebp + 0xd0]
        test   dword ptr [ebp + 0xdc], 2
        je     sub_7_4500
        push   esi
        push   dword ptr [ebp + 0xc8]
        test   byte ptr [ebp + 4], 2
        jne    loc_7_4488
        mov    edi, esi
loc_7_4432:
        or     ecx, ecx
        je     loc_7_44F3
        mov    edx, dword ptr [edi]
        mov    esi, dword ptr [ebp + 0xc8]
        sub    edx, esi
        shl    esi, 9
        add    esi, dword ptr [edi + 4]
        cmp    edx, ecx
        jb     loc_7_4450
        mov    edx, ecx
loc_7_4450:
        push   eax
        push   ecx
        push   edx
        push   edi
        mov    ecx, edx
        call   sub_7_4500
        pop    edi
        pop    edx
        pop    ecx
        pop    eax
        jb     loc_7_44F3
        add    eax, edx
        add    dword ptr [ebp + 0xc8], edx
        sub    ecx, edx
        mov    edx, dword ptr [ebp + 0xc8]
        cmp    dword ptr [edi], edx
        ja     loc_7_4432
        add    edi, 8
        mov    dword ptr [ebp + 0xc8], 0
        jmp    loc_7_4432
loc_7_4488:
        push   eax
        push   ecx
        mov    eax, 0x36d8                   ; -> obj7:0x36D8
        call   sub_7_490C
        pop    ecx
        pop    eax
        jb     loc_7_44F3
        push   eax
        movzx  edx, word ptr [ebp + 0x9c]
        sub    eax, edx
        xor    edx, edx
        mov    edi, 3
        div    edi
        pop    eax
        sub    edi, edx
        or     ecx, ecx
        je     loc_7_44E7
loc_7_44B2:
        cmp    edi, ecx
        jb     loc_7_44B8
        mov    edi, ecx
loc_7_44B8:
        push   ecx
        mov    ecx, edi
        push   eax
        push   ecx
        xor    eax, eax
        mov    esi, 0x36e4                   ; -> obj7:0x36E4
        call   sub_7_28C5
        pop    ecx
        pop    eax
        mov    esi, 0x36e4                   ; -> obj7:0x36E4
        push   eax
        push   ecx
        call   sub_7_4500
        pop    edx
        pop    eax
        pop    ecx
        jb     loc_7_44E7
        add    eax, edx
        mov    edi, 3
        sub    ecx, edx
        ja     loc_7_44B2
loc_7_44E7:
        pushfd 
        mov    eax, 0x36d8                   ; -> obj7:0x36D8
        call   sub_7_493D
        popfd  
loc_7_44F3:
        pop    dword ptr [ebp + 0xc8]
        pop    dword ptr [ebp + 0xd0]
        ret    
sub_7_4500:
        mov    edx, ecx
        test   byte ptr [ebp + 4], 2
        jne    loc_7_4556
        movzx  ecx, word ptr [ebp + 0x9c]
        sub    eax, ecx
        shl    eax, 9
        push   edx
        shl    edx, 9
        pop    ecx
        add    edx, eax
        shr    edx, 1
        add    edx, dword ptr [ebp + 0x78]
        cmp    dword ptr [ebp + 0x7c], edx
        ja     loc_7_4528
        mov    edx, dword ptr [ebp + 0x7c]
loc_7_4528:
        shr    eax, 1
        cmp    eax, 1
        sbb    ecx, ecx
        add    ecx, ecx
        sub    eax, ecx
        add    ecx, ecx
        sub    esi, ecx
        add    eax, dword ptr [ebp + 0x78]
loc_7_453A:
        cmp    eax, edx
        jae    loc_7_45E8
        movzx  ecx, word ptr [esi]
        call   sub_7_45E9
        jb     loc_7_45E8
        add    esi, 2
        inc    eax
        jmp    loc_7_453A
loc_7_4556:
        movzx  ecx, word ptr [ebp + 0x9c]
        sub    eax, ecx
        shl    eax, 9
        push   edx
        shl    edx, 9
        pop    ecx
        push   eax
        add    eax, edx
        add    eax, eax
        xor    edx, edx
        mov    edi, 3
        div    edi
        cmp    edx, 1
        cmc    
        sbb    eax, 0
        add    eax, dword ptr [ebp + 0x78]
        cmp    dword ptr [ebp + 0x7c], eax
        ja     loc_7_4587
        mov    eax, dword ptr [ebp + 0x7c]
loc_7_4587:
        xchg   dword ptr [esp], eax
        add    eax, eax
        xor    edx, edx
        mov    edi, 3
        div    edi
        cmp    edx, 1
        cmc    
        adc    eax, 0
        cmp    eax, 1
        sbb    ecx, ecx
        sub    eax, ecx
        sub    eax, ecx
        lea    ecx, [ecx + ecx*2]
        sub    esi, ecx
        add    eax, dword ptr [ebp + 0x78]
        bt     edx, 0
        adc    esi, 0
        bt     edx, 1
        pop    edx
        jb     loc_7_45D1
loc_7_45BB:
        cmp    eax, edx
        jae    loc_7_45E8
        movzx  ecx, word ptr [esi]
        and    ecx, 0xfff
        call   sub_7_45E9
        jb     loc_7_45E8
        inc    esi
        inc    eax
loc_7_45D1:
        cmp    eax, edx
        jae    loc_7_45E8
        movzx  ecx, word ptr [esi]
        shr    ecx, 4
        call   sub_7_45E9
        jb     loc_7_45E8
        add    esi, 2
        inc    eax
        jmp    loc_7_45BB
loc_7_45E8:
        ret    
sub_7_45E9:
        push   eax
        push   ecx
        push   edx
        push   esi
        jecxz  loc_7_464C
        test   dword ptr [ebp + 0xdc], 0x2000000
        je     loc_7_4682
        push   eax
        call   sub_7_2933
        pop    edx
        jb     loc_7_4682
        test   eax, 0x80000000
        jne    loc_7_4682
        test   eax, 0xffffff
        je     loc_7_4682
        cmp    dword ptr [ebp + 8], edx
        jne    loc_7_4621
        test   byte ptr [ebp + 4], 0x80
        jne    loc_7_4682
loc_7_4621:
        push   eax
        push   ecx
        push   edx
        call   sub_7_18B8
        pop    ebx
        pop    ecx
        pop    eax
        jb     loc_7_4682
        and    dword ptr [ebp + 4], 0xffffffbf
        mov    dword ptr [ebp + 0xc], eax
        mov    word ptr [ebp + 0x10], cx
        mov    ecx, 1
        call   sub_7_2BAD
        pushfd 
        call   sub_7_1B4F
        popfd  
        jmp    loc_7_4682
loc_7_464C:
        push   eax
        call   sub_7_2933
        pop    edx
        jb     loc_7_4682
        test   eax, 0x80000000
        je     loc_7_4682
        cmp    dword ptr [ebp + 8], edx
        jne    loc_7_4666
        btr    dword ptr [ebp + 4], 7
loc_7_4666:
        push   ecx
        call   sub_7_470E
        pop    ecx
        push   eax
        push   ecx
        push   edx
        call   sub_7_0E90
        pop    edx
        pop    ecx
        pop    eax
        and    eax, 0x7fffffff
        call   sub_7_29FE
loc_7_4682:
        pop    esi
        pop    edx
        pop    ecx
        pop    eax
        ret    
sub_7_4687:
        call   sub_7_46B8
        xor    edx, edx
        call   sub_7_470E
        or     dword ptr [ebp + 0x64], 0xffffffff
        or     dword ptr [ebp + 0x58], 0xffffffff
        mov    byte ptr [ebp + 0xf2], 0xff
        or     dword ptr [ebp + 4], 0x20000
        cmp    byte ptr [0xe65], 0           ; -> loc_1_0E65
        je     loc_7_46B7
        btr    dword ptr [ebp + 4], 0xd
loc_7_46B7:
        ret    
sub_7_46B8:
        mov    ah, byte ptr [0xe67]          ; -> loc_1_0E67
        mov    edx, 0x46d4                   ; -> loc_7_46D4
        VxDCall  Dev_048B.Service_0009h          ; cd2009008b04
        mov    eax, 0xd00                    ; -> obj1:0x0D00
        call   sub_7_4A17
        ret    
loc_7_46D4:
        jne    loc_7_470D
        cmp    dword ptr [esi + 8], ebp
        jne    loc_7_470D
        test   dword ptr [esi + 0xc], 0xf0000000
        jne    loc_7_470D
        cmp    byte ptr [esi + 0x32], 0
        je     loc_7_4702
        mov    eax, dword ptr [esi + 0xc]
        cmp    dword ptr [ebp + 0x68], eax
        jbe    loc_7_470D
        mov    eax, dword ptr [esi + 0x14]
        mov    edx, dword ptr [esi + 0x18]
        mov    dword ptr [eax + 0x18], edx
        mov    dword ptr [edx + 0x14], eax
        mov    byte ptr [esi + 0x32], 0
loc_7_4702:
        call   sub_7_2E3C
        VxDCall  Dev_048B.Service_0005h          ; cd2005008b04
loc_7_470D:
        ret    
sub_7_470E:
        test   byte ptr [ebp + 4], 0x20
        jne    loc_7_479F
        test   edx, edx
        je     loc_7_4721
        cmp    dword ptr [ebp + 8], edx
        jne    loc_7_4729
loc_7_4721:
        and    dword ptr [ebp + 8], 0
        and    byte ptr [ebp + 4], 0xbf
loc_7_4729:
        xor    ebx, ebx
        test   edx, edx
        jne    loc_7_4748
        mov    ecx, edx
        push   eax
        push   edx
        mov    ah, byte ptr [0xe68]          ; -> obj1:0x0E68
        mov    edx, 0x47a0                   ; -> loc_7_47A0
        VxDCall  Dev_048B.Service_0009h          ; cd2009008b04
        pop    edx
        pop    eax
        jmp    loc_7_478F
loc_7_4748:
        movzx  ecx, byte ptr [ebp + 0x9b]
        shr    ecx, 3
        push   eax
        mov    edi, edx
        shl    edi, 8
        lea    edi, [ecx + edi + 0x10000000]
loc_7_475F:
        push   ebx
        mov    ebx, ebp
        mov    ah, byte ptr [0xe68]          ; -> obj1:0x0E68
        xor    al, al
        dec    edi
        VxDCall  Dev_048B.Service_0004h          ; cd2004008b04
        pop    ebx
        jb     loc_7_478C
        VxDCall  Dev_048B.Service_000Dh          ; cd200d008b04
        jne    loc_7_478C
        push   ecx
        push   edx
        call   sub_7_1C09
        VxDCall  Dev_048B.Service_0005h          ; cd2005008b04
        pop    edx
        pop    ecx
        inc    ebx
loc_7_478C:
        loop   loc_7_475F
        pop    eax
loc_7_478F:
        test   ebx, ebx
        je     loc_7_479F
        push   eax
        mov    eax, 0xd0c                    ; -> loc_1_0D0C
        call   sub_7_4996
        pop    eax
loc_7_479F:
        ret    
loc_7_47A0:
        jne    loc_7_47D6
        mov    edx, dword ptr [esi + 0xc]
        mov    eax, edx
        and    edx, 0xf0000000
        cmp    edx, 0x10000000
        jne    loc_7_47D6
        cmp    dword ptr [esi + 8], ebp
        jne    loc_7_47D6
        jecxz  loc_7_47C8
        and    eax, 0xfffff00
        shr    eax, 8
        cmp    eax, ecx
        jne    loc_7_47D6
loc_7_47C8:
        push   ecx
        call   sub_7_1C09
        VxDCall  Dev_048B.Service_0005h          ; cd2005008b04
        pop    ecx
        inc    ebx
loc_7_47D6:
        ret    
sub_7_47D7:
        mov    eax, dword ptr [ebp + 0xd4]
        add    eax, dword ptr [ebp + 0x4c]
        dec    eax
        movzx  edx, word ptr [ebx + 0x68]
        xor    edi, edi
        push   ecx
        call   sub_7_2E52
        pop    ecx
        jb     loc_7_47FC
        add    dword ptr [ebp + 0xd4], ecx
        sub    dword ptr [ebp + 0xcc], ecx
loc_7_47FC:
        ret    
        db  000h, 000h, 000h, 0B8h, 0FFh, 0FFh, 0FFh
loc_7_4804:
        inc    ebx
        add    byte ptr [eax], al
loc_7_4808:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_7_4828:
        add    al, dh
        db  0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        dd  obj7:0x4830              ; fixup
        db  000h, 000h, 000h, 000h, 0FFh
loc_7_483D:
        inc    edx
        dec    esp
        add    byte ptr [edx + 0x56], dl
        add    byte ptr [ecx], al
loc_7_4845:
        inc    ecx
        add    byte ptr [eax], dh
        xor    byte ptr [eax], dh
        add    byte ptr [eax + 0x4830b8], dl ; -> obj7:0x4830
        add    al, ch
        mov    dh, 0
        add    byte ptr [eax], al
        jb     loc_7_4885
        mov    dword ptr [0x482c], ebp       ; -> obj7:0x482C
        mov    edi, dword ptr [0x4828]       ; -> loc_7_4828
        cmp    byte ptr [0x483c], 0          ; -> obj7:0x483C
        je     loc_7_4885
        mov    ebx, 0x4808                   ; -> loc_7_4808
        mov    ecx, 8
        call   sub_7_1A99
        jb     loc_7_4886
        mov    byte ptr [0x483c], 0          ; -> obj7:0x483C
loc_7_4885:
        ret    
loc_7_4886:
        mov    eax, 0x4830                   ; -> obj7:0x4830
        call   sub_7_493D
        mov    dword ptr [0x482c], 0         ; -> obj7:0x482C
        stc    
        jmp    loc_7_4885
sub_7_489D:
        cmp    dword ptr [0x4838], ebp       ; -> obj7:0x4838
        jne    loc_7_48BC
        mov    eax, 0x4830                   ; -> obj7:0x4830
        call   sub_7_493D
        cmp    dword ptr [eax + 8], 0
        je     loc_7_48BC
        and    dword ptr [0x482c], 0         ; -> obj7:0x482C
loc_7_48BC:
        ret    
sub_7_48BD:
        cmp    dword ptr [0x482c], ebp       ; -> obj7:0x482C
        stc    
        jne    loc_7_48CB
        call   0x484c
loc_7_48CB:
        ret    
sub_7_48CC:
        mov    eax, dword ptr [ebp + 0x24]
        xchg   dword ptr [0x4828], eax       ; -> loc_7_4828
        mov    dword ptr [ebp + 0x24], eax
        movzx  ecx, byte ptr [ebp + 0x9b]
        shr    ecx, 3
loc_7_48E2:
        mov    esi, dword ptr [ebp + ecx*4 + 0x200]
        mov    edi, dword ptr [ecx*4 + 0x4804] ; -> loc_7_4804
        VxDCall  Dev_048B.Service_0011h          ; cd2011008b04
        loop   loc_7_48E2
        cmp    byte ptr [ebp + 0x9b], 0x40
        setne  byte ptr [0x483c]             ; -> obj7:0x483C
        call   sub_7_489D
        ret    
sub_7_490C:
        cmp    dword ptr [eax + 8], 0
        jne    loc_7_4916
        mov    dword ptr [eax + 8], ebp
loc_7_4915:
        ret    
loc_7_4916:
        pushal 
        call   sub_7_1CEA
        popal  
        jb     loc_7_4915
        jne    sub_7_490C
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ecx], ebp
        lea    ecx, [ebp + 0x244]
        mov    dword ptr [ecx], 0
        mov    dword ptr [eax + 4], ecx
        call   sub_7_4A6C
        clc    
        jmp    loc_7_4915
sub_7_493D:
        push   edx
        push   esi
        mov    edx, dword ptr [eax]
        mov    dword ptr [eax + 8], edx
        or     edx, edx
        je     loc_7_4971
        mov    esi, dword ptr [edx + 0x244]
        mov    dword ptr [eax], esi
        or     esi, esi
        jne    loc_7_4957
        mov    dword ptr [eax + 4], eax
loc_7_4957:
        mov    esi, 0x49ed                   ; -> sub_7_49ED
        push   eax
        push   ebx
        push   ecx
        xor    eax, eax
        mov    ebx, eax
        mov    ecx, 0x408
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
        pop    ecx
        pop    ebx
        pop    eax
loc_7_4971:
        pop    esi
        pop    edx
        ret    
sub_7_4974:
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ecx], ebp
        lea    ecx, [ebp + 0x244]
        mov    dword ptr [ecx], 0
        mov    dword ptr [eax + 4], ecx
        cmp    dword ptr [eax], ebp
        sete   cl
        or     byte ptr [eax + 8], cl
        call   sub_7_4A6C
        ret    
sub_7_4996:
        btr    dword ptr [eax + 8], 0
        jb     loc_7_499E
        ret    
loc_7_499E:
        bts    dword ptr [eax + 8], 1
        jb     loc_7_49C5
        push   edx
        push   esi
        mov    edx, eax
        mov    esi, 0x49c6                   ; -> loc_7_49C6
        push   eax
        push   ebx
        push   ecx
        xor    eax, eax
        mov    ebx, eax
        mov    ecx, 0x408
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
        pop    ecx
        pop    ebx
        pop    eax
        pop    esi
        pop    edx
loc_7_49C5:
        ret    
loc_7_49C6:
        btr    dword ptr [edx + 8], 1
        bt     dword ptr [edx + 8], 2
        jb     loc_7_4A4C
        mov    eax, edx
        mov    edx, dword ptr [edx]
        xor    ecx, ecx
        xchg   dword ptr [edx + 0x244], ecx
        mov    dword ptr [eax], ecx
        or     ecx, ecx
        setne  cl
        jne    loc_7_49EA
        mov    dword ptr [eax + 4], eax
loc_7_49EA:
        or     byte ptr [eax + 8], cl
sub_7_49ED:
        mov    dword ptr [edx + 0x23c], esp
        mov    ecx, dword ptr [edx + 0x238]
        jecxz  loc_7_4A16
        sub    esp, ecx
        lea    esi, [edx + 0x24c]
        mov    edi, esp
        cld    
        shrd   eax, ecx, 2
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        shld   ecx, eax, 2
        rep movsb byte ptr es:[edi], byte ptr [esi]
        popal  
loc_7_4A16:
        ret    
sub_7_4A17:
        btr    dword ptr [eax + 8], 0
        jb     loc_7_4A1F
        ret    
loc_7_4A1F:
        bts    dword ptr [eax + 8], 2
        bts    dword ptr [eax + 8], 1
        jb     loc_7_4A4B
        push   edx
        push   esi
        mov    edx, eax
        mov    esi, 0x4a4c                   ; -> loc_7_4A4C
        push   eax
        push   ebx
        push   ecx
        xor    eax, eax
        mov    ebx, eax
        mov    ecx, 0x408
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
        pop    ecx
        pop    ebx
        pop    eax
        pop    esi
        pop    edx
loc_7_4A4B:
        ret    
loc_7_4A4C:
        mov    eax, edx
        xor    edx, edx
        mov    dword ptr [eax + 8], edx
        xchg   dword ptr [eax], edx
        mov    dword ptr [eax + 4], eax
loc_7_4A58:
        xor    eax, eax
        xchg   dword ptr [edx + 0x244], eax
        push   eax
        call   sub_7_49ED
        pop    edx
        or     edx, edx
        jne    loc_7_4A58
        ret    
sub_7_4A6C:
        pushal 
        mov    ecx, dword ptr [ebp + 0x23c]
        sub    ecx, esp
        cmp    ecx, 0x100
        ja     loc_7_4AA3
        mov    dword ptr [ebp + 0x238], ecx
        mov    esi, esp
        lea    edi, [ebp + 0x24c]
        cld    
        shrd   eax, ecx, 2
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        shld   ecx, eax, 2
        rep movsb byte ptr es:[edi], byte ptr [esi]
        add    esp, dword ptr [ebp + 0x238]
        sti    
        ret    
loc_7_4AA3:
        and    dword ptr [ebp + 0x238], 0
        movzx  edi, word ptr [0x16]          ; -> loc_2_0016
        add    edi, dword ptr [4]            ; -> loc_2_0004
        movzx  ecx, word ptr [0x18]          ; -> loc_2_0018
        add    ecx, dword ptr [4]            ; -> loc_2_0004
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    eax, 0xe0000000
        VxDCall  SHELL.SHELL_SYSMODAL_Message    ; cd2003001700
loc_7_4AD5:
        jmp    loc_7_4AD5
sub_7_4AD7:
        mov    ecx, dword ptr [ebp + 0x38]
        cmp    ecx, 1
        jb     loc_7_4B10
loc_7_4ADF:
        sub    esp, 0x10
        mov    word ptr [esp], 2
        mov    byte ptr [esp + 0xe], dl
        call   sub_7_4B11
        mov    word ptr [esp + 4], si
        mov    dword ptr [esp + 6], edx
        mov    esi, esp
        push   esi
        call   sub_1_0958
        mov    esi, dword ptr [esi + 0xa]
        add    esp, 0x14
        cmp    esi, 1
        lea    esi, [edx + esi - 0x64]
loc_7_4B10:
        ret    
sub_7_4B11:
        mov    esi, dword ptr [ecx]
        movzx  esi, byte ptr [esi + 0x77]
        test   dword ptr [ecx + 0x1c], 0x800
        je     loc_7_4B22
        add    esi, esi
loc_7_4B22:
        inc    esi
        shl    esi, 3
        mov    edx, dword ptr [ecx + 4]
        add    esi, edx
        add    esi, 0xbc
        add    edx, 0x64
        ret    
sub_7_4B35:
        mov    ecx, dword ptr [ebp + 0x38]
loc_7_4B38:
        mov    ecx, dword ptr [ecx + 4]
        add    ecx, 0x64
        neg    ecx
        lea    esi, [ecx + esi + 0x64]
        sub    esp, 8
        mov    dword ptr [esp + 4], esi
        mov    word ptr [esp], 4
        mov    esi, esp
        push   esi
        call   sub_1_0958
        add    esp, 0xc
        ret    
sub_7_4B5E:
        bt     dword ptr [ebp + 4], 0xe
        jb     loc_7_4B8B
        cmp    dword ptr [ebp + 0xe8], 1
        cmc    
        jb     loc_7_4B8B
        mov    dword ptr [ebp + 0xec], esi
        mov    edx, ebp
        mov    esi, 0x4b8c                   ; -> loc_7_4B8C
        VMMCall  VMM.Schedule_Global_Event       ; cd200e000100
        mov    dword ptr [ebp + 0xe8], esi
        cmp    esi, 1
loc_7_4B8B:
        ret    
loc_7_4B8C:
        cld    
        movzx  eax, byte ptr [edx + 0x88]
        add    al, 0x41
        mov    byte ptr [0x4843], al         ; -> obj7:0x4843
        mov    eax, 0x4843                   ; -> obj7:0x4843
        movzx  ebx, byte ptr [edx + 0x89]
        add    bl, 0x41
        mov    byte ptr [0x4845], bl         ; -> loc_7_4845
        mov    ebx, 0x4845                   ; -> loc_7_4845
        mov    ecx, 0x483d                   ; -> loc_7_483D
        cmp    word ptr [edx + 0xb4], 0
        je     loc_7_4BC9
        mov    ecx, 0x4840                   ; -> obj7:0x4840
loc_7_4BC9:
        movzx  edi, byte ptr [edx + 0xb2]
        push   eax
        push   edx
        mov    eax, edi
        mov    edi, 0x4847                   ; -> obj7:0x4847
        mov    dl, 0xa
        div    dl
        add    ah, 0x30
        mov    byte ptr [edi + 2], ah
        xor    ah, ah
        div    dl
        add    ax, 0x3030
        mov    word ptr [edi], ax
        pop    edx
        pop    eax
        push   edi
        push   ecx
        push   ebx
        push   eax
        push   0x180001
        push   0                             ; -> loc_2_0000
        VMMCall  VMM._LocalizeStackSprintf       ; cd2059010100
        mov    edi, eax
        mov    eax, dword ptr [edx + 0xec]
        add    eax, 0x100000
        push   0x4845                        ; -> loc_7_4845
        push   0x4843                        ; -> obj7:0x4843
        push   eax
        push   0                             ; -> loc_2_0000
        VMMCall  VMM._LocalizeStackSprintf       ; cd2059010100
        mov    ecx, eax
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    eax, 0x80001000
        VxDCall  SHELL.SHELL_SYSMODAL_Message    ; cd2003001700
        add    esp, dword ptr [esp]
        add    esp, dword ptr [esp]
        xor    eax, eax
        mov    dword ptr [edx + 0xe8], eax
        ret    
        db  026h, 044h, 052h
VxD__LTEXT ENDS

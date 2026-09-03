; CDVSD -- reconstructed 32-bit disassembly (VxD / LE format)
; 4 objects, 2653 instructions recovered by recursive descent

Declare_Virtual_Device CDVSD, 3, 10, CDVSD_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 080000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x1048  R-X
CDVSD_Control:
        cmp    eax, 0x1b
        je     loc_2_0000
        cmp    eax, 0x32
        je     loc_1_0266
        clc    
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_0020:
        mov    ebx, dword ptr [esp + 4]
        mov    word ptr [ebx + 2], 0
        mov    si, word ptr [ebx]
        cmp    si, 3
        je     0xffff0060                    ; -> obj3:0x0060
        cmp    si, 4
        je     loc_3_0271
        cmp    si, 2
        je     loc_3_02C1
        cmp    si, 9
        je     loc_1_0079
        cmp    si, 0x15
        je     loc_1_005E
        mov    word ptr [ebx + 2], 0xffff
        ret    
loc_1_005E:
        mov    esi, dword ptr [ebx + 0xc]
        mov    ecx, dword ptr [0x14]         ; -> obj1:0x0014
loc_1_0067:
        or     ecx, ecx
        je     loc_1_0078
        cmp    dword ptr [ecx + 4], esi
        je     loc_1_0074
        mov    ecx, dword ptr [ecx]
        jmp    loc_1_0067
loc_1_0074:
        or     byte ptr [ecx + 8], 0x40
loc_1_0078:
        ret    
loc_1_0079:
        cli    
        mov    edx, dword ptr [0x14]         ; -> obj1:0x0014
        or     edx, edx
        je     loc_1_00B9
        test   byte ptr [0x18], 2            ; -> obj1:0x0018
        je     loc_1_00B9
        test   byte ptr [0x18], 1            ; -> obj1:0x0018
        jne    loc_1_00B9
        bts    dword ptr [edx + 8], 2
        jb     loc_1_00B9
        bts    dword ptr [edx + 8], 5
        push   esi
        mov    esi, 0x1c6                    ; -> loc_1_01C6
        and    byte ptr [edx + 8], 0xf7
        VMMCall  VMM.Schedule_Global_Event       ; cd200e000100
        mov    dword ptr [0x1c], esi         ; -> obj1:0x001C
        pop    esi
loc_1_00B9:
        sti    
        ret    
loc_1_00BB:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        push   ebx
        mov    edi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [edi + 8]
        test   byte ptr [0x18], 1            ; -> obj1:0x0018
        jne    loc_1_01AF
        mov    edx, dword ptr [edi + 0x90]
        cmp    word ptr [edi + 0x6a], 0x17
        je     loc_1_0123
        cmp    word ptr [edi + 0x6a], 0x10
        jae    loc_1_016E
        bts    dword ptr [edx + 8], 1
        jb     loc_1_016E
        cmp    dword ptr [ebx + 0xa6], 0x63656b6d
        jne    loc_1_015B
        cmp    dword ptr [ebx + 0xaa], 0x78783572
        jne    loc_1_015B
        push   esi
        push   edx
        mov    esi, 0x1b4                    ; -> loc_1_01B4
        mov    edx, ebx
        mov    eax, 0x1388
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        pop    edx
        pop    esi
        jmp    loc_1_016E
loc_1_0123:
        push   edx
        cmp    dword ptr [ebx + 0x70], 0
        je     loc_1_014E
        lea    edx, [ebp - 4]
        push   0
        push   dword ptr [ebx + 0x70]
        push   edx
        VxDCall  CONFIGMG._CONFIGMG_Get_Parent   ; cd2003003300
        add    esp, 0xc
        or     eax, eax
        jne    loc_1_014E
        push   0
        push   dword ptr [ebp - 4]
        VxDCall  CONFIGMG._CONFIGMG_Reenumerate_DevNode ; cd2033003300
        add    esp, 8
loc_1_014E:
        pop    edx
        or     byte ptr [edx + 8], 0x40
        mov    word ptr [edi + 0x6a], 0
        jmp    loc_1_016E
loc_1_015B:
        push   edx
        push   0
        push   ebx
        push   0x23                          ; -> loc_3_0023
        VxDCall  SHELL._SHELL_CallAtAppyTime     ; cd200e001700
        add    esp, 0xc
        pop    edx
loc_1_016E:
        push   word ptr [edi + 0x6a]
        push   edx
        call   sub_1_0CF0
        pop    edx
        pop    ax
        cmp    ax, 0x10
        jb     loc_1_0188
        bts    dword ptr [edx + 8], 3
        jae    loc_1_019D
loc_1_0188:
        cli    
        btr    dword ptr [edx + 8], 5
        mov    edx, dword ptr [edx]
        or     edx, edx
        je     loc_1_019D
        bts    dword ptr [edx + 8], 5
        and    byte ptr [edx + 8], 0xf7
loc_1_019D:
        sti    
        mov    esi, 0x1c6                    ; -> loc_1_01C6
        VMMCall  VMM.Schedule_Global_Event       ; cd200e000100
        mov    dword ptr [0x1c], esi         ; -> obj1:0x001C
loc_1_01AF:
        pop    ebx
        pop    esi
        pop    edi
        leave  
        ret    
loc_1_01B4:
        push   0
        push   edx
        push   0x23                          ; -> loc_3_0023
        VxDCall  SHELL._SHELL_CallAtAppyTime     ; cd200e001700
        add    esp, 0xc
        ret    
loc_1_01C6:
        cli    
        xor    esi, esi
        xchg   dword ptr [0x1c], esi         ; -> obj1:0x001C
        sti    
        or     esi, esi
        je     loc_1_0241
loc_1_01D4:
        or     edx, edx
        je     loc_1_0242
        test   byte ptr [0x18], 1            ; -> obj1:0x0018
        jne    loc_1_0242
        bts    dword ptr [edx + 8], 5
        test   byte ptr [edx + 8], 0x50
        jne    loc_1_0252
        test   byte ptr [edx + 8], 1
        je     loc_1_024E
        btr    dword ptr [edx + 8], 7
        jb     loc_1_024E
        mov    esi, dword ptr [edx + 4]
        cmp    byte ptr [esi + 0x76], 0
        jne    loc_1_024E
        push   edx
        call   sub_1_0D0B
        pop    edx
        or     edi, edi
        je     loc_1_024E
        mov    word ptr [edi + 0x68], 6
        mov    dword ptr [edi + 0x7c], 0
        and    word ptr [edi + 0x6c], 0x77f7
        or     byte ptr [edi + 0x6d], 0x40
        mov    dword ptr [edi + 0x70], 0xbb  ; -> loc_1_00BB
        mov    dword ptr [edi + 0xa4], edi
        mov    dword ptr [edi + 0x90], edx
        mov    ebx, esi
        xor    edx, edx
        call   sub_1_0F60
loc_1_0241:
        ret    
loc_1_0242:
        mov    edx, dword ptr [0x14]         ; -> obj1:0x0014
        btr    dword ptr [edx + 8], 2
        ret    
loc_1_024E:
        mov    edx, dword ptr [edx]
        jmp    loc_1_01D4
loc_1_0252:
        test   byte ptr [edx + 8], 0x10
        je     loc_1_0242
        push   0
        push   edx
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
        jmp    loc_1_0242
loc_1_0266:
        cli    
        or     byte ptr [0x18], 1            ; -> obj1:0x0018
        mov    edx, dword ptr [0x14]         ; -> obj1:0x0014
loc_1_0274:
        or     edx, edx
        je     loc_1_0283
        bt     dword ptr [edx + 8], 5
        jb     loc_1_0294
        mov    edx, dword ptr [edx]
        jmp    loc_1_0274
loc_1_0283:
        xor    esi, esi
        xchg   dword ptr [0x1c], esi         ; -> obj1:0x001C
        VMMCall  VMM.Cancel_Global_Event         ; cd2012000100
        sti    
        clc    
        ret    
loc_1_0294:
        sti    
        mov    esi, dword ptr [edx + 4]
        push   edx
        call   sub_1_0D0B
        pop    edx
        or     edi, edi
        je     loc_1_0283
        mov    word ptr [edi + 0x68], 0x10
        mov    dword ptr [edi + 0x7c], 0
        and    word ptr [edi + 0x6c], 0x77f7
        or     byte ptr [edi + 0x6d], 0x41
        mov    dword ptr [edi + 0xa4], edi
        mov    ebx, esi
        xor    edx, edx
        call   sub_1_0F60
        call   sub_1_0CF0
        jmp    loc_1_0283
        db  000h, 000h, 000h
sub_1_02D4:
        push   edi
        push   esi
        push   ebx
        mov    esi, dword ptr [esp + 0x10]
        mov    eax, dword ptr [esi + 0x14]
        mov    eax, dword ptr [eax + 4]
        mov    dword ptr [esi + 0x10], eax
        test   byte ptr [esi + 0x6d], 0x80
        je     loc_1_0308
        mov    edi, dword ptr [esi + 0x20]
        mov    dword ptr [edi + 0x44], 0
        cmp    byte ptr [edi + 3], 1
        jne    loc_1_0311
        mov    word ptr [esi + 0x6a], 0
        mov    edx, dword ptr [eax + 8]
        bts    dword ptr [edx + 8], 7
loc_1_0308:
        call   sub_1_038D
loc_1_030D:
        pop    ebx
        pop    esi
        pop    edi
        ret    
loc_1_0311:
        call   sub_1_0418
        or     al, al
        je     loc_1_0308
        cmp    al, 2
        je     loc_1_030D
        mov    edi, dword ptr [esi + 0x20]
        mov    ebx, dword ptr [esi + 8]
        mov    eax, dword ptr [esi + 0x7c]
        test   dword ptr [esi + 0x6c], 0x200
        jne    loc_1_0336
        mul    dword ptr [ebx + 0xdb]
loc_1_0336:
        mov    dword ptr [edi + 0x10], eax
        add    dword ptr [esi + 0x14], 8
        mov    eax, dword ptr [esi + 0x10]
        mov    eax, dword ptr [eax + 0xc]
        mov    dword ptr [esi + 0x10], eax
        push   esi
        call   dword ptr [eax]
        add    esp, 4
        jmp    loc_1_030D
loc_1_034E:
        push   edi
        push   esi
        push   ebx
        mov    esi, dword ptr [esp + 0x10]
        mov    edi, dword ptr [esi + 0x20]
        test   byte ptr [edi + 3], 0x80
        je     loc_1_0377
        mov    eax, dword ptr [edi + 0x1c]
        mov    al, byte ptr [eax + 0xc]
        cmp    al, 0x28
        jb     loc_1_0377
        cmp    al, 0x29
        jbe    loc_1_0370
        cmp    al, 0x3a
        jne    loc_1_0377
loc_1_0370:
        mov    edi, dword ptr [esi + 8]
        or     byte ptr [edi + 0x22], 8
loc_1_0377:
        mov    edi, dword ptr [esi + 0x14]
        sub    edi, 8
        mov    dword ptr [esi + 0x14], edi
        mov    ebx, dword ptr [esi + 4]
        push   esi
        call   dword ptr [edi]
        add    esp, 4
        pop    ebx
        pop    esi
        pop    edi
        ret    
sub_1_038D:
        cmp    word ptr [esi + 0x6a], 0x19
        je     loc_1_03E1
        cmp    word ptr [esi + 0x6a], 0x18
        je     loc_1_03E1
loc_1_039B:
        push   ebx
        cmp    word ptr [esi + 0x68], 0x12
        jne    loc_1_03A9
        mov    word ptr [esi + 0x6a], 0
loc_1_03A9:
        mov    edi, dword ptr [esi + 0x14]
        sub    edi, 8
        mov    dword ptr [esi + 0x14], edi
        mov    ebx, dword ptr [esi + 4]
        push   esi
        call   dword ptr [edi]
        add    esp, 4
        xchg   ebx, esi
        cli    
        cmp    dword ptr [esi + 0x49c], 0
        je     loc_1_03D9
        cmp    dword ptr [esi + 0x49c], ebx
        jne    loc_1_03DE
        mov    dword ptr [esi + 0x49c], 0
loc_1_03D9:
        call   sub_1_0EFF
loc_1_03DE:
        sti    
        pop    ebx
        ret    
loc_1_03E1:
        mov    edx, dword ptr [esi + 0x10]
        mov    edx, dword ptr [edx + 8]
        btr    dword ptr [edx + 8], 1
        jae    loc_1_039B
        mov    ebx, dword ptr [esi + 8]
        push   0
        push   ebx
        push   0                             ; -> loc_3_0000
        VxDCall  SHELL._SHELL_CallAtAppyTime     ; cd200e001700
        add    esp, 0xc
        jmp    loc_1_039B
sub_1_0404:
        mov    edx, dword ptr [esp + 4]
        push   esi
        mov    esi, 0x2e8                    ; -> loc_3_02E8
        VMMCall  VMM.Schedule_Global_Event       ; cd200e000100
        pop    esi
        ret    
        db  000h, 000h
sub_1_0418:
        mov    ebx, dword ptr [esi + 0x14]
        mov    ebx, dword ptr [ebx + 4]
        mov    dword ptr [esi + 0x10], ebx
        mov    ebx, dword ptr [esi + 4]
        test   byte ptr [edi + 3], 0x80
        jne    loc_1_04BD
        cmp    byte ptr [edi + 3], 0xa
        je     loc_1_04F8
        cmp    byte ptr [edi + 3], 4
        jne    loc_1_0450
        cmp    byte ptr [edi + 4], 2
        je     loc_1_045F
        cmp    byte ptr [edi + 4], 8
        je     loc_1_0450
        cmp    byte ptr [edi + 4], 0x18
        je     loc_1_04AB
loc_1_0450:
        cmp    byte ptr [edi + 0x4c], 5
        jb     loc_1_04AB
loc_1_0456:
        mov    word ptr [esi + 0x6a], 0x1a
loc_1_045C:
        xor    al, al
        ret    
loc_1_045F:
        cmp    byte ptr [edi + 0x30], 3
        je     loc_1_0456
        mov    edx, edi
        mov    esi, 0x475                    ; -> loc_1_0475
        VMMCall  VMM.Schedule_Global_Event       ; cd200e000100
        mov    al, 2
        ret    
loc_1_0475:
        mov    edi, edx
        mov    esi, dword ptr [edi + 0x40]
        mov    ebx, dword ptr [esi + 4]
        mov    ax, 0xb
        mov    ecx, dword ptr [edi + 0x1c]
        mov    edx, 0xe
        call   sub_1_0DDA
        or     eax, eax
        jne    loc_1_04EF
        or     byte ptr [edi + 3], 0x80
        push   esi
        call   sub_1_02D4
        add    esp, 4
        ret    
loc_1_04A0:
        cmp    byte ptr [edi + 0x4c], 5
        jb     loc_1_04AB
        mov    al, byte ptr [ecx + 2]
        jmp    loc_1_0501
loc_1_04AB:
        inc    byte ptr [edi + 0x4c]
        mov    byte ptr [edi + 3], 0
        mov    ax, word ptr [esi + 0xe]
        mov    word ptr [esi + 0xc], ax
        mov    al, 1
        ret    
loc_1_04BD:
        mov    ecx, dword ptr [edi + 0x1c]
        mov    al, byte ptr [ecx + 2]
        and    eax, 0xf
        cmp    al, 0
        je     loc_1_04A0
        cmp    al, 5
        je     loc_1_0501
        cmp    al, 1
        jne    loc_1_04DB
        mov    word ptr [esi + 0x6a], 2
        xor    al, al
        ret    
loc_1_04DB:
        cmp    byte ptr [ecx + 0xc], 0x45
        je     loc_1_04F8
        cmp    al, 3
        je     loc_1_04A0
        cmp    al, 4
        je     loc_1_04A0
        cmp    al, 0xb
        je     loc_1_04A0
        jmp    loc_1_0501
loc_1_04EF:
        mov    word ptr [esi + 0x6a], 0x20
        xor    al, al
        ret    
loc_1_04F8:
        mov    word ptr [esi + 0x6a], 0x17
        xor    al, al
        ret    
loc_1_0501:
        mov    cl, byte ptr [ecx + 0xc]
        cmp    cl, 0x64
        je     loc_1_0529
        cmp    cl, 0x3a
        je     loc_1_052F
        cmp    cl, 0x24
        jne    loc_1_0519
        cmp    byte ptr [edi + 0x30], 0x28
        je     loc_1_0529
loc_1_0519:
        movzx  eax, byte ptr [eax + 0x535]   ; -> obj1:0x0535
loc_1_0520:
        mov    word ptr [esi + 0x6a], ax
        jmp    loc_1_045C
loc_1_0529:
        mov    ax, 0x28
        jmp    loc_1_0520
loc_1_052F:
        mov    ax, 0x18
        jmp    loc_1_0520
        db  000h, 000h, 020h, 014h, 01Bh, 016h, 019h, 01Fh, 01Ah, 01Ah, 01Ah, 01Ah, 01Ah, 01Ah, 01Ah, 01Ah
        db  000h, 000h, 000h
loc_1_0548:
        push   ebp
        push   edi
        push   esi
        mov    ebx, dword ptr [esp + 0x10]
        mov    esi, dword ptr [ebx + 4]
        mov    edx, dword ptr [ebx + 0x10]
        mov    eax, dword ptr [ebx + 0x14]
        mov    dword ptr [eax], 0x2d4        ; -> sub_1_02D4
        mov    dword ptr [eax + 4], edx
        add    dword ptr [ebx + 0x14], 8
        cmp    word ptr [ebx + 0x68], 0xf
        je     loc_1_060E
        test   byte ptr [ebx + 0x6d], 8
        jne    loc_1_0596
        cli    
        lea    eax, [esi + 0x43c]
        xor    ecx, ecx
loc_1_057F:
        cmp    dword ptr [eax + 0x64], ecx
        je     loc_1_0589
        mov    eax, dword ptr [eax + 0x64]
        jmp    loc_1_057F
loc_1_0589:
        mov    dword ptr [eax + 0x64], ebx
        mov    dword ptr [ebx + 0x64], ecx
        call   sub_1_0EFF
        jmp    loc_1_060A
loc_1_0596:
        cmp    word ptr [ebx + 0x68], 0x12
        je     loc_1_0616
        call   sub_1_0E67
        movzx  eax, word ptr [ebx + 0x68]
        cmp    eax, 0xe
        jae    loc_1_0673
        call   sub_1_063B
loc_1_05B8:
        or     al, al
        jne    loc_1_0678
        mov    edi, dword ptr [ebx + 0x20]
        mov    eax, dword ptr [ebx + 0x80]
        mov    dword ptr [edi + 0x18], eax
        mov    eax, dword ptr [ebx + 0x7c]
        test   dword ptr [ebx + 0x6c], 0x200
        jne    loc_1_05DE
        mul    dword ptr [esi + 0xdb]
loc_1_05DE:
        mov    dword ptr [edi + 0x10], eax
loc_1_05E1:
        mov    edi, dword ptr [ebx + 0x20]
        or     dword ptr [ebx + 0x6c], 0x8000
        mov    dword ptr [edi + 0x44], 0
loc_1_05F2:
        mov    eax, dword ptr [ebx + 0x10]
        mov    edx, dword ptr [eax + 8]
        test   byte ptr [edx + 9], 2
        jne    loc_1_062D
loc_1_05FE:
        mov    eax, dword ptr [eax + 0xc]
        mov    dword ptr [ebx + 0x10], eax
        push   ebx
        call   dword ptr [eax]
        add    esp, 4
loc_1_060A:
        pop    esi
        pop    edi
        pop    ebp
        ret    
loc_1_060E:
        mov    dword ptr [eax], 0x34e        ; -> loc_1_034E
        jmp    loc_1_05E1
loc_1_0616:
        mov    edi, dword ptr [ebx + 0x20]
        cmp    byte ptr [edi + 0x30], 0x43
        jne    loc_1_05E1
        mov    word ptr [ebx + 0xe], 0x5a
        mov    word ptr [ebx + 0xc], 0x5a
        jmp    loc_1_05E1
loc_1_062D:
        mov    word ptr [ebx + 0xe], 0x3c
        mov    word ptr [ebx + 0xc], 0x3c
        jmp    loc_1_05FE
sub_1_063B:
        rol    dword ptr [esi], 0
        add    byte ptr [edi + 8], bl        ; -> loc_1_085F
        add    byte ptr [eax], al
        pop    edi
        or     byte ptr [eax], al
        add    byte ptr [edi + 8], bl        ; -> loc_1_085F
        add    byte ptr [eax], al
        pop    edi
        or     byte ptr [eax], al
        add    byte ptr [esi], al            ; -> sub_1_0806
        or     byte ptr [eax], al
        add    byte ptr [esi], al            ; -> sub_1_0806
        or     byte ptr [eax], al
        add    byte ptr [edx + 0xd], dh      ; -> loc_1_0D72
        add    byte ptr [eax], al
        jbe    0x66a
        add    byte ptr [eax], al
        xor    al, 8
        add    byte ptr [eax], al
        cmp    byte ptr [eax], cl
        add    byte ptr [eax], al
        mov    al, 0xd
        add    byte ptr [eax], al
        push   0x88000008                    ; -> loc_1_0F88
        sldt   word ptr [eax]
loc_1_0673:
        jmp    loc_1_05F2
loc_1_0678:
        dec    al
        jne    loc_1_0691
        mov    eax, dword ptr [ebx + 0x14]
        sub    eax, 8
        mov    dword ptr [ebx + 0x14], eax
        mov    esi, ebx
        call   sub_1_038D
        jmp    loc_1_060A
loc_1_0691:
        dec    al
        je     loc_1_060A
        cmp    al, 2
        je     loc_1_05F2
        mov    word ptr [ebx + 0x6a], 0x16
        cmp    word ptr [ebx + 0x68], 0xd
        jne    loc_1_060A
        mov    dword ptr [ebx + 0xa0], 0x1f
        jmp    loc_1_060A
loc_1_06C1:
        cmp    dword ptr [ebx + 0x74], 0x10
        jb     loc_1_0757
        mov    dword ptr [edi + 0xc], 0x248
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
        mov    eax, dword ptr [ebx + 0x7c]
        test   byte ptr [ebx + 0x6e], 4
        jne    0x72d
        mov    byte ptr [edi + 0x36], 0
        mov    byte ptr [edi + 0x39], 0
        test   byte ptr [ebx + 0x6d], 2
        jne    loc_1_071D
        xchg   al, ah
        mov    word ptr [edi + 0x37], ax
        mov    eax, 0x800
loc_1_0712:
        cmp    eax, dword ptr [esi + 0x490]
        jne    loc_1_0769
        xor    al, al
        ret    
loc_1_071D:
        mov    word ptr [edi + 0x37], 0x100
        jmp    loc_1_0712
loc_1_0725:
        pushal 
        add    byte ptr [eax + 0x30094009], dl
        or     dword ptr [edi - 0x73], edx
        cmp    eax, 0x725                    ; -> loc_1_0725
        mov    ecx, 4
        repne scasw ax, word ptr es:[edi]
        pop    edi
        jne    loc_1_0757
        mov    byte ptr [edi + 0x3a], cl
        mov    dword ptr [edi + 0x36], 0x1000000
        mov    byte ptr [edi + 0xa], 0xc
        mov    byte ptr [edi + 0x30], 0xd8
        xor    al, al
        mov    byte ptr [edi + 0x3b], al
        ret    
loc_1_0757:
        mov    word ptr [ebx + 0x6a], 0x10
        mov    al, 1
        ret    
loc_1_0760:
        mov    word ptr [ebx + 0x6a], 0x21
        mov    al, 1
        ret    
loc_1_0769:
        call   sub_1_07CD
        jb     loc_1_0760
        mov    edx, ebx
        push   esi
        mov    esi, 0x2fc                    ; -> loc_3_02FC
        VMMCall  VMM.Schedule_Global_Event       ; cd200e000100
        pop    esi
        mov    al, 2
        ret    
loc_1_0782:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        mov    edi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [edi + 4]
        mov    esi, dword ptr [edi + 0x20]
        cmp    byte ptr [esi + 3], 1
        pushfd 
        push   dword ptr [edi + 0x90]
        call   sub_1_0CF0
        pop    ebx
        mov    edi, dword ptr [ebx + 0x20]
        mov    esi, dword ptr [ebx + 8]
        popfd  
        jne    loc_1_07C0
        movzx  ecx, word ptr [esi + 0x466]
        xchg   ch, cl
        mov    dword ptr [esi + 0x490], ecx
        xor    al, al
        jmp    loc_1_05B8
loc_1_07C0:
        mov    word ptr [ebx + 0x6a], 0x28
        mov    al, 1
        jmp    loc_1_05B8
sub_1_07CD:
        push   ebx
        push   dword ptr [ebx + 0x10]
        call   sub_1_0D0B
        pop    eax
        or     edi, edi
        stc    
        je     loc_1_0805
        mov    dword ptr [edi + 0x10], eax
        mov    dword ptr [edi + 0xa4], edi
        mov    word ptr [edi + 0x68], 0x12
        mov    dword ptr [edi + 8], esi
        mov    ebx, edi
        call   sub_1_0E67
        pop    dword ptr [ebx + 0x90]
        mov    al, byte ptr [esi + 0x7d]
        shl    al, 5
        mov    byte ptr [edi + 0x31], al
        clc    
loc_1_0805:
        ret    
sub_1_0806:
        mov    dword ptr [edi + 0xc], 0
        mov    byte ptr [edi + 0xa], 6
        mov    byte ptr [edi + 0x30], 0
        mov    al, byte ptr [esi + 0x7d]
        shl    al, 5
        mov    byte ptr [edi + 0x31], al
        mov    dword ptr [edi + 0x32], 0
        mov    word ptr [ebx + 0xe], 0x3c
        mov    word ptr [ebx + 0xc], 0x3c
        xor    al, al
        ret    
loc_1_0834:
        mov    al, 1
        jmp    loc_1_083A
loc_1_0838:
        xor    al, al
loc_1_083A:
        mov    dword ptr [edi + 0xc], 0
        mov    byte ptr [edi + 0xa], 6
        mov    byte ptr [edi + 0x30], 0x1e
        mov    dword ptr [edi + 0x32], 0
        mov    byte ptr [edi + 0x34], al
        mov    al, byte ptr [esi + 0x7d]
        shl    al, 5
        mov    byte ptr [edi + 0x31], al
        xor    al, al
        ret    
loc_1_085F:
        mov    word ptr [ebx + 0x6a], 0x16
        mov    al, 1
        ret    
loc_1_0868:
        and    byte ptr [esi + 0x47e], 0xfb
        mov    edx, ebx
        mov    esi, 0x87f                    ; -> loc_1_087F
        VMMCall  VMM.Schedule_Global_Event       ; cd200e000100
        mov    al, 2
        ret    
loc_1_087F:
        mov    eax, dword ptr [edx + 0x14]
        mov    ecx, dword ptr [edx + 0x10]
        mov    dword ptr [eax + 4], ecx
        mov    ebx, edx
        mov    esi, dword ptr [edx + 8]
        call   sub_1_07CD
        jb     loc_1_0912
        call   sub_1_0806
        mov    dword ptr [edi + 0x10], 0
        mov    dword ptr [ebx + 0x70], 0x8ac ; -> loc_1_08AC
        jmp    sub_1_0ED9
loc_1_08AC:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        pushfd 
        sti    
        mov    edi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [edi + 8]
        mov    esi, dword ptr [edi + 0x20]
        cmp    byte ptr [esi + 3], 1
        jne    loc_1_0B8A
        xor    edx, edx
        mov    dword ptr [esi + 0x32], 0x96000000
        mov    word ptr [esi + 0x37], dx
        mov    byte ptr [esi + 0x39], dl
        mov    byte ptr [esi + 0x30], 0x45
        mov    byte ptr [esi + 3], 0
        mov    byte ptr [esi + 0xa], 0xa
        mov    dword ptr [esi + 0xc], 0
        mov    dword ptr [esi + 0x10], edx
        mov    dword ptr [esi + 0x36], edx
        or     dword ptr [edi + 0x6c], 0xe08
        and    dword ptr [edi + 0x6c], 0xfffffefd
        xchg   edi, ebx
        xchg   esi, edi
        mov    dword ptr [ebx + 0x70], 0x922 ; -> loc_1_0922
        call   sub_1_0ED9
        popfd  
        pop    esi
        pop    edi
        pop    ebp
        ret    
loc_1_0912:
        mov    ebx, edx
        mov    word ptr [ebx + 0x6a], 0x1d
        push   ebp
        push   edi
        push   esi
        jmp    loc_1_05B8
loc_1_0922:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        pushfd 
        sti    
        mov    edi, dword ptr [ebp + 8]
        mov    ebx, dword ptr [edi + 8]
        mov    esi, dword ptr [edi + 0x20]
        mov    eax, dword ptr [edi + 0x80]
        cmp    byte ptr [esi + 0x30], 0x45
        je     loc_1_0BCC
        cmp    byte ptr [esi + 0x30], 0x1a
        je     loc_1_0C2C
        cmp    byte ptr [esi + 3], 1
        je     loc_1_0971
        cmp    byte ptr [esi + 3], 0x12
        je     loc_1_0971
        test   byte ptr [esi + 0x39], 0x40
        jne    loc_1_0B6A
        test   byte ptr [esi + 0x39], 0x80
        jne    loc_1_0B07
        jmp    loc_1_0B8A
loc_1_0971:
        test   byte ptr [esi + 0x39], 0x40
        jne    loc_1_0B34
        test   byte ptr [esi + 0x39], 0x80
        jne    loc_1_0B73
        test   dword ptr [ebx + 0x482], 2
        je     loc_1_09C4
        mov    edx, dword ptr [ebx + 0x48c]
        lea    edx, [edx + 0x10]
        movzx  ecx, byte ptr [ebx + 0x137]
        sub    cl, byte ptr [ebx + 0x136]
        inc    cl
        inc    cl
loc_1_09AB:
        push   ecx
        mov    ecx, dword ptr [ebx + ecx*8 + 0x134]
        call   sub_1_0F9C
        cmp    ecx, edx
        pop    ecx
        jbe    loc_1_09CF
        loop   loc_1_09AB
        jmp    loc_1_0B8A
loc_1_09C4:
        xor    edx, edx
        mov    word ptr [ebx + 0x474], dx
        jmp    loc_1_09DD
loc_1_09CF:
        movzx  ecx, byte ptr [ebx + ecx*8 + 0x132]
        mov    byte ptr [ebx + 0x133], cl
loc_1_09DD:
        mov    eax, dword ptr [edi + 0x80]
        mov    cl, byte ptr [eax + 2]
        or     cl, cl
        je     loc_1_0B8A
        cmp    cl, 0x64
        jae    loc_1_0B8A
        movzx  ecx, byte ptr [eax + 3]
        or     cl, cl
        je     loc_1_0B8A
        cmp    cl, 0x64
        jae    loc_1_0A53
        jmp    loc_1_0A3F
loc_1_0A0A:
        sub    cl, byte ptr [eax + 2]
        jb     loc_1_0B8A
        inc    cl
        mov    byte ptr [eax + ecx*8 + 6], 0xaa
        movzx  ecx, word ptr [eax]
        xchg   cl, ch
        push   dword ptr [edi + 0x90]
        call   sub_1_0CF0
        pop    esi
        mov    word ptr [esi + 0x6a], 0
loc_1_0A31:
        sub    dword ptr [esi + 0x14], 8
        call   sub_1_038D
        popfd  
        pop    esi
        pop    edi
        pop    ebp
        ret    
loc_1_0A3F:
        mov    edx, ecx
        sub    dl, byte ptr [eax + 2]
        inc    edx
        cmp    byte ptr [eax + edx*8 + 6], 0xaa
        je     loc_1_0A8D
        cmp    byte ptr [eax + edx*8 + 6], 0xa2
        je     loc_1_0A8D
loc_1_0A53:
        push   eax
        mov    eax, ecx
        shl    ax, 4
        shr    al, 4
        aad    0xa
        mov    ecx, eax
        pop    eax
        cmp    ecx, 0x63
        jae    loc_1_0B8A
        mov    edx, ecx
        sub    dl, byte ptr [eax + 2]
        inc    edx
        cmp    byte ptr [eax + edx*8 + 6], 0xaa
        je     loc_1_0A8D
        cmp    byte ptr [eax + edx*8 + 6], 0xa2
        je     loc_1_0A8D
        movzx  ecx, word ptr [eax]
        xchg   cl, ch
        sub    ecx, 0xc
        shr    ecx, 3
        add    cl, byte ptr [eax + 2]
loc_1_0A8D:
        push   ecx
        push   esi
        mov    byte ptr [eax + 3], cl
        mov    ecx, edx
        mov    dl, byte ptr [eax + 2]
        lea    ebp, [eax + 4]
        xor    esi, esi
loc_1_0A9C:
        mov    byte ptr [ebp + 2], dl
        movzx  si, byte ptr [ebp + 1]
        lea    ebp, [ebp + 8]
        inc    edx
        test   esi, 0xffff0000
        jne    loc_1_0AFC
        test   esi, 4
        jne    loc_1_0AFC
        test   byte ptr [ebp + 1], 4
        je     loc_1_0AFC
        cmp    byte ptr [ebp + 2], 0xaa
        je     loc_1_0AFC
        cmp    byte ptr [ebp + 2], 0xa2
        je     loc_1_0AFC
        or     byte ptr [ebx + 0x482], 0x10
        mov    byte ptr [ebx + 0x487], dl
        push   ecx
        push   eax
        mov    ecx, dword ptr [ebp + 4]
        call   sub_1_0F9C
        cmp    ecx, 0x2bf2
        jb     loc_1_0AF2
        sub    ecx, 0x2bf2
        shl    esi, 0x10
loc_1_0AF2:
        call   sub_1_0FC9
        mov    dword ptr [ebp + 4], ecx
        pop    eax
        pop    ecx
loc_1_0AFC:
        loop   loc_1_0B05
        pop    esi
        pop    ecx
        jmp    loc_1_0A0A
loc_1_0B05:
        jmp    loc_1_0A9C
loc_1_0B07:
        mov    cl, byte ptr [ebx + 0x474]
        inc    cl
        cmp    cl, byte ptr [ebx + 0x475]
        jne    loc_1_0B8A
        mov    eax, dword ptr [ebx + 0x48c]
        sub    eax, 0x96
        mov    dword ptr [ebx + 0x488], eax
        mov    byte ptr [ebx + 0x486], 0xff
        jmp    loc_1_0BC6
loc_1_0B34:
        and    byte ptr [ebx + 0x482], 0xfd
        cmp    word ptr [eax], 0xa00
        jne    loc_1_0B6A
        movzx  edx, word ptr [eax + 2]
        mov    word ptr [ebx + 0x474], dx
        mov    ecx, dword ptr [eax + 8]
        call   sub_1_0F9C
        mov    dword ptr [ebx + 0x48c], ecx
        cmp    dh, dl
        je     loc_1_0B6A
        or     byte ptr [ebx + 0x482], 2
        or     byte ptr [esi + 0x39], 0x80
loc_1_0B6A:
        xchg   edi, esi
        xchg   esi, ebx
        jmp    loc_1_0C74
loc_1_0B73:
        lea    edx, [eax + 4]
        movzx  ecx, word ptr [eax]
        xchg   cl, ch
        add    eax, ecx
loc_1_0B7D:
        cmp    byte ptr [edx + 3], 0xa1
        je     loc_1_0BA1
        add    edx, 0xb
        cmp    edx, eax
        jl     loc_1_0B7D
loc_1_0B8A:
        push   dword ptr [edi + 0x90]
        call   sub_1_0CF0
        pop    esi
        mov    word ptr [esi + 0x6a], 0x18
        jmp    loc_1_0A31
loc_1_0BA1:
        movzx  eax, byte ptr [edx + 8]
        mov    byte ptr [ebx + 0x486], al
        add    edx, 0xb
        cmp    byte ptr [edx + 3], 0xa2
        jne    loc_1_0B8A
        mov    eax, dword ptr [edx + 7]
        movzx  ax, ah
        rol    eax, 0x10
        xchg   ah, al
        mov    dword ptr [ebx + 0x488], eax
loc_1_0BC6:
        mov    byte ptr [esi + 0x39], 0x40
        jmp    loc_1_0B6A
loc_1_0BCC:
        and    byte ptr [ebx + 0x482], 0xed
        or     byte ptr [ebx + 0x482], 1
        cmp    byte ptr [esi + 3], 1
        je     loc_1_0BFB
        mov    ecx, dword ptr [esi + 0x1c]
        cmp    byte ptr [ecx + 0xc], 0x64
        je     loc_1_0BFB
        mov    cl, byte ptr [ecx + 2]
        and    ecx, 0xf
        cmp    cl, 5
        jne    loc_1_0BFB
        and    byte ptr [ebx + 0x482], 0xfe
loc_1_0BFB:
        xchg   edi, esi
        xchg   esi, ebx
        mov    byte ptr [edi + 0xa], 6
        mov    byte ptr [edi + 0x30], 0x1a
        mov    al, byte ptr [esi + 0x7d]
        shl    al, 5
        mov    byte ptr [edi + 0x31], al
        mov    byte ptr [edi + 0x32], 0x41
        xor    ecx, ecx
        mov    byte ptr [edi + 0x35], cl
        mov    byte ptr [edi + 0x33], cl
loc_1_0C1C:
        mov    cl, 0x14
        mov    byte ptr [edi + 0x34], cl
        lea    eax, [esi + 0x45c]
        jmp    loc_1_0CC5
loc_1_0C2C:
        xchg   edi, esi
        xchg   esi, ebx
        xor    ecx, ecx
        mov    dword ptr [esi + 0x490], 0x800
        and    byte ptr [esi + 0x482], 0xfb
        cmp    byte ptr [edi + 3], 1
        jne    loc_1_0C74
        test   word ptr [esi + 0x466], 0xffff
        je     loc_1_0C74
        or     byte ptr [esi + 0x482], 4
        cmp    byte ptr [edi + 0x32], 0x41
        mov    byte ptr [edi + 0x32], 1
        je     loc_1_0C1C
        movzx  ecx, word ptr [esi + 0x466]
        xchg   cl, ch
        mov    dword ptr [esi + 0x490], ecx
loc_1_0C74:
        mov    byte ptr [edi + 0x30], 0x43
        push   edi
        lea    edi, [esi + 0x134]
        mov    ecx, 0xc9
        xor    eax, eax
        rep stosd dword ptr es:[edi], eax
        pop    edi
        mov    al, byte ptr [esi + 0x7d]
        shl    al, 5
        or     al, 2
        mov    byte ptr [edi + 0x31], al
        xor    eax, eax
        mov    dword ptr [edi + 0x32], eax
        mov    al, byte ptr [edi + 0x39]
        xor    al, 0x40
        ror    eax, 8
        mov    dword ptr [edi + 0x36], eax
        mov    word ptr [edi + 0x37], 0x2403
        mov    byte ptr [edi + 0xa], 0xa
        lea    eax, [esi + 0x134]
        mov    ecx, 0x324
        mov    word ptr [ebx + 0xc], 0x3c
        mov    word ptr [ebx + 0xe], 0x3c
loc_1_0CC5:
        mov    dword ptr [ebx + 0x80], eax
        mov    dword ptr [edi + 0xc], 0x248
        mov    dword ptr [ebx + 0x7c], ecx
        mov    dword ptr [edi + 0x10], ecx
        or     dword ptr [ebx + 0x6c], 0xe08
        and    dword ptr [ebx + 0x6c], 0xfffffefd
        call   sub_1_0EC7
        popfd  
        pop    esi
        pop    edi
        pop    ebp
        ret    
sub_1_0CF0:
        sub    edi, dword ptr [ebx + 4]
        sub    esp, 8
        mov    esi, esp
        mov    dword ptr [esi + 4], edi
        mov    word ptr [esi], 4
        push   esi
        call   sub_1_0F54
        add    esp, 0xc
        ret    
sub_1_0D0B:
        push   esi
        mov    esi, dword ptr [esi]
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
        call   sub_1_0F54
        mov    edi, dword ptr [edi + 0xa]
        add    esp, 0x14
        mov    eax, edi
        or     edi, edi
        je     loc_1_0D70
        add    edi, dword ptr [esi + 4]
        xor    eax, eax
        mov    dword ptr [edi + 0x64], eax
        mov    dword ptr [edi + 0xa8], eax
        mov    dword ptr [edi + 0x7c], eax
        mov    dword ptr [edi + 0x6c], 0x8c09
        pop    esi
        mov    al, byte ptr [esi + 0x14]
        mov    byte ptr [edi + 0xb1], al
        mov    eax, edi
        add    eax, 0xbc
        mov    dword ptr [edi + 0xac], eax
        ret    
loc_1_0D70:
        pop    esi
        ret    
loc_1_0D72:
        mov    al, 3
        jmp    loc_1_0D78
loc_1_0D76:
        mov    al, 2
loc_1_0D78:
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
        xor    al, al
        ret    
loc_1_0DB0:
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
        xor    al, al
        ret    
sub_1_0DDA:
        pushal 
        mov    esi, dword ptr [ebx]
        mov    ecx, dword ptr [esi + 4]
        add    ecx, 0x64
        sub    esp, 0x10
        mov    edi, esp
        mov    dword ptr [edi + 6], ecx
        add    cx, 0x58
        mov    word ptr [edi + 4], cx
        mov    byte ptr [edi + 0xe], 0
        mov    word ptr [edi], 2
        push   edi
        call   sub_1_0F54
        mov    edi, dword ptr [edi + 0xa]
        add    esp, 0x14
        mov    ecx, eax
        mov    eax, 0x1d
        or     edi, edi
        je     loc_1_0E61
        add    edi, dword ptr [esi + 4]
        xor    edx, edx
        mov    dword ptr [edi + 0xa8], edx
        mov    dword ptr [edi + 0x7c], edx
        mov    word ptr [edi + 0x68], cx
        mov    dword ptr [edi + 0x6c], 0x8d09
        mov    edx, 0x548                    ; -> loc_1_0548
        xor    edx, edx
        mov    al, byte ptr [esi + 0x14]
        mov    byte ptr [edi + 0xb1], al
        call   sub_1_0F60
        movzx  eax, word ptr [edi + 0x6a]
        sub    edi, dword ptr [ebx + 4]
        sub    esp, 8
        mov    esi, esp
        mov    dword ptr [esi + 4], edi
        mov    word ptr [esi], 4
        push   esi
        call   sub_1_0F54
        add    esp, 0xc
loc_1_0E61:
        mov    dword ptr [esp + 0x1c], eax
        popal  
        ret    
sub_1_0E67:
        push   edx
        mov    eax, dword ptr [ebx + 0x10]
        movsx  edi, word ptr [eax + 0x10]
        lea    edi, [edi + ebx + 4]
        mov    dword ptr [edi + 0x40], ebx
        mov    dword ptr [edi], 0x40
        mov    byte ptr [edi + 2], 0
        mov    byte ptr [edi + 3], 0
        mov    dword ptr [ebx + 0x20], edi
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
        pop    edx
        ret    
sub_1_0EC7:
        push   ebx
        call   sub_1_0F68
        add    esp, 4
        or     al, al
        je     sub_1_0ED9
        or     dword ptr [ebx + 0x6c], 0x40
sub_1_0ED9:
        mov    byte ptr [edi + 3], 0
        mov    dword ptr [edi + 0x44], 0
        mov    cx, word ptr [ebx + 0xe]
        mov    word ptr [ebx + 0xc], cx
        xor    edx, edx
        xchg   esi, edi
        xchg   edi, ebx
        mov    dword ptr [edi + 0xa4], edi
        call   sub_1_0F60
        ret    
sub_1_0EFF:
        cmp    dword ptr [esi + 0x49c], 0
        jne    loc_1_0F38
        mov    ebx, dword ptr [esi + 0x4a0]
        or     ebx, ebx
        je     loc_1_0F38
        mov    dword ptr [esi + 0x49c], ebx
        mov    ecx, dword ptr [ebx + 0x64]
        mov    dword ptr [esi + 0x4a0], ecx
        sti    
        mov    ax, word ptr [ebx + 0xe]
        mov    word ptr [ebx + 0xc], ax
        cmp    byte ptr [ebx + 0x68], 0xd
        je     loc_1_0F3A
        push   ebp
        push   edi
        push   esi
        jmp    loc_1_0596
loc_1_0F38:
        sti    
        ret    
loc_1_0F3A:
        mov    edx, ebx
        mov    esi, 0xf47                    ; -> loc_1_0F47
        VMMCall  VMM.Schedule_Global_Event       ; cd200e800100
loc_1_0F47:
        sub    esp, 0xc
        mov    ebx, edx
        mov    esi, dword ptr [ebx + 4]
        jmp    loc_1_0596
sub_1_0F54:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_0F60:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_0F68:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0F70:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0F88:
        mov    dword ptr [edi - 4], 0x3f5    ; -> loc_3_03F5
        push   ebx
        call   sub_1_0404
        lea    esp, [esp + 4]
        mov    al, 2
        ret    
sub_1_0F9C:
        push   edx
        mov    eax, 0x1194
        movzx  cx, ch
        mul    cx
        shl    edx, 0x10
        add    eax, edx
        ror    ecx, 0x10
        movzx  edx, ch
        add    edx, eax
        xchg   ecx, edx
        mov    eax, 0x4b
        mul    dl
        add    ecx, eax
        sub    ecx, 0x96
        pop    edx
        ret    
sub_1_0FC9:
        push   edx
        push   ebx
        add    ecx, 0x96
        mov    ebx, 0x1194
        mov    eax, ecx
        xor    edx, edx
        div    ebx
        movzx  ecx, al
        mov    eax, edx
        mov    ebx, 0x4b
        xor    edx, edx
        div    bx
        mov    ch, al
        shl    ecx, 8
        shl    edx, 0x18
        or     ecx, edx
        pop    ebx
        pop    edx
        ret    
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  3, 10                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'CDVSD   '                  ; DDB_Name
        dd  080000000h                  ; DDB_Init_Order
        dd  CDVSD_Control            ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x80  R-X
loc_2_0000:
        call   sub_2_0030
        push   0x48                          ; -> loc_2_0048
        VxDCall  BLOCKDEV.Service_0007h          ; cd2007001000
        add    esp, 4
        cmp    word ptr [0x74], 1            ; -> obj2:0x0074
        je     loc_2_002B
        cmp    word ptr [0x74], 2            ; -> obj2:0x0074
        jne    loc_2_0029
        jmp    loc_2_002B
loc_2_0029:
        jmp    loc_2_002E
loc_2_002B:
        xor    eax, eax
        ret    
loc_2_002E:
        stc    
        ret    
sub_2_0030:
        mov    dword ptr [0x54], 0x20        ; -> loc_2_0054
        mov    dword ptr [0x58], 0xf54       ; -> loc_2_0058
        ret    
        db  000h, 000h, 000h
loc_2_0048:
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        add    byte ptr [eax], ah
        add    byte ptr [eax], al
loc_2_0054:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_0058:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   ebx
        inc    ebx
        push   ebx
        dec    ecx
        xor    ah, byte ptr [eax]
        inc    ebx
        inc    esp
        sub    eax, 0x204d4f52
        push   esi
        push   ebx
        inc    esp
        add    byte ptr [eax], al
        inc    eax
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

VxD__LTEXT SEGMENT   ; object 3  vsize=0x1280  R-X
loc_3_0000:
        mov    edx, dword ptr [esp + 4]
        sub    esp, 0xc
        mov    ecx, esp
        mov    word ptr [ecx], 0xd
        mov    dword ptr [ecx + 4], edx
        mov    dword ptr [ecx + 8], 1
        push   ecx
        call   sub_1_0F54
        add    esp, 0x10
        ret    
loc_3_0023:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 8]
        sub    esp, 0xc
        mov    ecx, esp
        mov    word ptr [ecx], 0xe
        mov    dword ptr [ecx + 4], edx
        mov    dword ptr [ecx + 8], 1
        push   ecx
        call   sub_1_0F54
        add    esp, 0x10
        leave  
        ret    
loc_3_0049:
        inc    ecx
        jne    0xc0
        outsd  dx, dword ptr [esi]
        dec    ecx
        outsb  dx, byte ptr [esi]
        jae    loc_3_00B6
        jb     loc_3_00C7
        dec    esi
        outsd  dx, dword ptr [esi]
        je     0xc0
        imul   sp, word ptr [ebx + 0x61], 0x6974
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        add    byte ptr [esi - 0x75], dl
        jae    0x70
        test   dword ptr [esi + 0x20], 0x6000
        jne    loc_3_0227
        cmp    byte ptr [esi + 0x74], 1
        jne    loc_3_0227
        cmp    byte ptr [esi + 0x40], 5
        jne    loc_3_0227
        mov    edx, dword ptr [0xf70]        ; -> loc_1_0F70
        test   byte ptr [edx + 0x28], 0x10
        jne    loc_3_00CD
        mov    eax, dword ptr [esi + 0x1c]
        test   eax, 0x100000
        jne    loc_3_00CD
        test   byte ptr [edx + 0x29], 8
        jne    loc_3_0227
        test   byte ptr [esi + 0x482], 0x20
        jne    loc_3_00C3
        mov    al, byte ptr [esi + 0x84]
        and    al, 7
loc_3_00B6:
        cmp    al, 2
        jae    loc_3_00CD
        test   byte ptr [esi + 0x482], 8
        jne    loc_3_00CD
loc_3_00C3:
        test   byte ptr [edx + 0x29], 0x10
loc_3_00C7:
        jne    loc_3_0227
loc_3_00CD:
        inc    byte ptr [0]                  ; -> obj4:0x0000
        mov    eax, dword ptr [esi + 0x1c]
        or     eax, 0x40000
        test   byte ptr [esi + 0x83], 0x80
        je     loc_3_0133
        or     dword ptr [esi + 0x20], 0x10004
        mov    dword ptr [esi + 0x494], 0xff02ff01
        mov    dword ptr [esi + 0x498], 0
        or     eax, 0x30000
        and    eax, 0xffefffff
        push   eax
        push   1
        push   0xc
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        mov    edx, eax
        pop    eax
        or     edx, edx
        je     loc_3_0227
        mov    ecx, dword ptr [0x14]         ; -> obj1:0x0014
        mov    dword ptr [edx], ecx
        mov    dword ptr [edx + 4], esi
        mov    dword ptr [0x14], edx         ; -> obj1:0x0014
loc_3_0133:
        push   esi
        push   edi
        mov    esi, edi
        sub    esp, 0x18
        mov    edi, esp
        mov    word ptr [edi], 5
        or     eax, 8
        and    eax, 0xfffffffe
        mov    dword ptr [edi + 0x12], eax
        mov    dword ptr [edi + 4], esi
        mov    dword ptr [edi + 8], 0x548    ; -> loc_1_0548
        mov    al, byte ptr [ebx + 8]
        mov    byte ptr [edi + 0x16], al
        mov    dword ptr [edi + 0xc], edx
        movzx  eax, word ptr [esi + 0x80]
        add    eax, 0x62
        mov    word ptr [edi + 0x10], ax
        push   edi
        call   sub_1_0F54
        add    esp, 0x1c
        pop    edi
        pop    esi
        mov    byte ptr [esi + 0x7f], 0xe
        sub    esp, 0x1c
        mov    ecx, esp
        mov    word ptr [ecx], 0x11
        mov    dword ptr [ecx + 4], esi
        mov    dword ptr [ecx + 0xc], 3
        mov    dword ptr [ecx + 0x10], 1
        lea    edx, [ecx + 0x18]
        mov    dword ptr [ecx + 0x14], edx
        mov    dword ptr [ecx + 8], 0x49     ; -> loc_3_0049
        push   ecx
        call   sub_1_0F54
        movzx  ecx, word ptr [ecx + 2]
        movzx  edx, byte ptr [edx]
        add    esp, 0x20
        or     ecx, ecx
        mov    ecx, edx
        mov    edx, dword ptr [0x14]         ; -> obj1:0x0014
        jne    loc_3_01D7
        or     byte ptr [edx + 9], 4
        or     ecx, ecx
        je     loc_3_01D7
        test   byte ptr [esi + 0x22], 4
        jne    loc_3_01D7
        and    byte ptr [edx + 9], 0xfb
        or     byte ptr [edx + 9], 1
loc_3_01D7:
        cmp    byte ptr [esi + 0x7d], 1
        jb     loc_3_0210
        ja     loc_3_0202
        mov    ecx, dword ptr [edx]
        or     ecx, ecx
        je     loc_3_0202
        or     byte ptr [ecx + 9], 2
        mov    eax, dword ptr [ecx + 4]
        or     byte ptr [eax + 0x482], 0x80
        test   byte ptr [ecx + 9], 1
        jne    loc_3_0202
        and    byte ptr [ecx + 8], 0xfe
        call   sub_3_0229
loc_3_0202:
        or     byte ptr [edx + 9], 2
        mov    eax, dword ptr [edx + 4]
        or     byte ptr [eax + 0x482], 0x80
loc_3_0210:
        mov    ecx, edx
        call   sub_3_0229
        test   byte ptr [edx + 9], 1
        jne    loc_3_0223
        test   byte ptr [edx + 9], 6
        jne    loc_3_0227
loc_3_0223:
        or     byte ptr [edx + 8], 1
loc_3_0227:
        pop    esi
        ret    
sub_3_0229:
        enter  0xc, 0
        pushal 
        mov    dword ptr [ebp - 0xc], 1
        mov    esi, dword ptr [ecx + 4]
        mov    edx, dword ptr [esi + 0x70]
        lea    eax, [ebp - 8]
        mov    byte ptr [eax], 0
        test   byte ptr [ecx + 9], 4
        jne    loc_3_0256
        test   byte ptr [ecx + 9], 1
        jne    loc_3_0253
        test   byte ptr [ecx + 9], 2
        jne    loc_3_0256
loc_3_0253:
        mov    byte ptr [eax], 1
loc_3_0256:
        push   0
        push   1
        push   eax
        push   3
        push   0x49                          ; -> loc_3_0049
        push   0
        push   edx
        VxDCall  CONFIGMG._CONFIGMG_Write_Registry_Value ; cd203f003300
        add    esp, 0x1c
        popal  
        leave  
        ret    
loc_3_0271:
        mov    esi, dword ptr [ebx + 0xc]
        cmp    byte ptr [esi + 0x74], 1
        jne    loc_3_029A
        cmp    byte ptr [esi + 0x40], 5
        jne    loc_3_029A
        mov    eax, 0x14                     ; -> obj1:0x0014
        mov    edx, dword ptr [0x14]         ; -> obj1:0x0014
loc_3_028B:
        or     edx, edx
        je     loc_3_029A
        cmp    dword ptr [edx + 4], esi
        je     loc_3_029B
        mov    eax, edx
        mov    edx, dword ptr [edx]
        jmp    loc_3_028B
loc_3_029A:
        ret    
loc_3_029B:
        cli    
        mov    ecx, dword ptr [edx]
        mov    dword ptr [eax], ecx
        mov    dword ptr [edx], edx
        or     byte ptr [edx + 8], 0x10
        dec    byte ptr [0]                  ; -> obj4:0x0000
        bts    dword ptr [edx + 8], 5
        sti    
        jb     loc_3_02C0
        push   0
        push   edx
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
loc_3_02C0:
        ret    
loc_3_02C1:
        cmp    byte ptr [0], 0               ; -> obj4:0x0000
        jne    loc_3_02DE
        xor    esi, esi
        xchg   dword ptr [0x1c], esi         ; -> obj1:0x001C
        VMMCall  VMM.Cancel_Global_Event         ; cd2012000100
        mov    word ptr [ebx + 2], 0xffff
loc_3_02DE:
        or     byte ptr [0x18], 2            ; -> obj1:0x0018
        ret    
        db  000h, 000h
loc_3_02E8:
        push   ebp
        push   edi
        push   esi
        mov    ebx, edx
        mov    edi, dword ptr [ebx + 0x20]
        mov    esi, dword ptr [ebx + 4]
        call   dword ptr [edi - 4]
        jmp    loc_1_05B8
        db  000h
loc_3_02FC:
        mov    edi, edx
        mov    ebx, dword ptr [edi + 8]
        mov    esi, dword ptr [edi + 0x20]
        lea    eax, [ebx + 0x45c]
        mov    byte ptr [eax], 0
        mov    dword ptr [edi + 0x80], eax
        xor    ecx, ecx
        mov    cl, 0x14
        mov    dword ptr [esi + 0x10], ecx
        mov    dword ptr [edi + 0x7c], ecx
        or     byte ptr [edi + 0x6d], 2
        mov    dword ptr [esi + 0xc], 0x288
        mov    byte ptr [esi + 0x34], cl
        mov    cl, byte ptr [ebx + 0x7d]
        shl    cl, 5
        or     cl, 0x10
        mov    byte ptr [esi + 0x31], cl
        mov    ecx, dword ptr [edi + 0x90]
        mov    edx, dword ptr [ecx + 0x7c]
        test   byte ptr [ecx + 0x6d], 2
        jne    loc_3_034B
        mov    edx, 0x800
loc_3_034B:
        xchg   dl, dh
        mov    word ptr [eax + 0xa], dx
        mov    byte ptr [esi + 0x30], 0x15
        mov    byte ptr [esi + 0xa], 6
        xor    edx, edx
        mov    word ptr [esi + 0x32], dx
        mov    byte ptr [esi + 0x35], dl
        mov    byte ptr [eax + 3], 8
        mov    dword ptr [edi + 0x70], 0x782 ; -> loc_1_0782
        xchg   edi, ebx
        xchg   esi, edi
        jmp    sub_1_0EC7
        db  000h, 000h
sub_3_0378:
        push   es
        add    byte ptr [eax], al
        inc    esi
        push   es
        add    byte ptr [eax], al
        jns    loc_3_0388
        add    byte ptr [eax], al
        stosb  byte ptr es:[edi], al
        push   es
        add    byte ptr [eax], al
loc_3_0388:
        push   7
        add    byte ptr [eax], al
        jnp    loc_3_0395
        add    byte ptr [eax], al
        mov    al, byte ptr [edi]
        add    byte ptr [eax], al
        lahf   
loc_3_0395:
        pop    es
        add    byte ptr [eax], al
        test   eax, 0xb3000007               ; -> loc_3_07B3
        pop    es
        add    byte ptr [eax], al
        mov    bl, 7
        add    byte ptr [eax], al
        hlt    
        pop    es
        add    byte ptr [eax], al
        xchg   esi, eax
        or     dword ptr [eax], eax
        add    byte ptr [edi], ah            ; -> loc_3_0A27
        or     al, byte ptr [eax]
        add    cl, al                        ; -> loc_3_04C1
        add    al, 0
        add    byte ptr [esi], ch            ; -> loc_3_0A2E
        or     al, byte ptr [eax]
        add    byte ptr [edi + 0xa], dl      ; -> loc_3_0A57
        add    byte ptr [eax], al
        daa    
        or     al, byte ptr [eax]
        add    bl, ch                        ; -> loc_3_06EB
        push   es
        add    byte ptr [eax], al
        or     eax, dword ptr [edi]
        add    byte ptr [eax], al
        xor    al, byte ptr [edi]
        add    byte ptr [eax], al
        daa    
        or     al, byte ptr [eax]
        add    byte ptr [edi], ah            ; -> loc_3_0A27
        or     al, byte ptr [eax]
        add    byte ptr [edi], ah            ; -> loc_3_0A27
        or     al, byte ptr [eax]
        add    byte ptr [edi], ah            ; -> loc_3_0A27
        or     al, byte ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    dword ptr [ecx], eax
        add    dword ptr [ecx], eax
        add    byte ptr [eax], al
        add    dword ptr [ecx], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_03F5:
        test   byte ptr [ebx + 0x6d], 0x40
        jne    loc_3_0405
        test   byte ptr [esi + 0x22], 8
        jne    loc_3_0494
loc_3_0405:
        movzx  eax, word ptr [ebx + 0x92]
        cmp    al, 0x19
        jae    loc_3_046D
        cmp    ah, 0xcf
        je     loc_3_0470
        cmp    ah, 0xcd
        jne    loc_3_046D
        mov    dword ptr [edi + 0xc], 0
        mov    dword ptr [ebx + 0x7c], 0
        xor    ah, ah
        mov    byte ptr [edi + 0x31], ah
        cmp    byte ptr [eax + 0x3dc], 1     ; -> obj3:0x03DC
        je     loc_3_0466
        call   sub_3_0378
        or     al, al
        jne    loc_3_0465
loc_3_0441:
        mov    dl, byte ptr [esi + 0x7d]
        shl    dl, 5
        or     byte ptr [edi + 0x31], dl
        mov    edx, dword ptr [ebx + 0x10]
        mov    eax, dword ptr [ebx + 0x14]
        sub    eax, 8
        mov    dword ptr [eax], 0x404        ; -> sub_1_0404
        mov    dword ptr [eax + 4], edx
        mov    dword ptr [edi - 4], 0xaa8    ; -> loc_3_0AA8
        xor    al, al
loc_3_0465:
        ret    
loc_3_0466:
        call   sub_1_0806
        jmp    loc_3_0441
loc_3_046D:
        mov    al, 4
        ret    
loc_3_0470:
        xor    ah, ah
        mov    edx, dword ptr [eax*4 + 0x378] ; -> sub_3_0378
        cmp    edx, 0xa27                    ; -> loc_3_0A27
        je     loc_3_046D
        mov    dword ptr [ebx + 0xa0], 0
        mov    word ptr [ebx + 0x6a], 0
        mov    al, 1
        ret    
loc_3_0494:
        cmp    word ptr [ebx + 0x92], 0xcd02
        je     loc_3_0405
        mov    dword ptr [ebx + 0xa0], 0x1f
        cmp    word ptr [edi + 0x6a], 0x18
        jne    loc_3_04BE
        mov    dword ptr [ebx + 0xa0], 0x15
loc_3_04BE:
        mov    al, 1
        ret    
loc_3_04C1:
        test   byte ptr [esi + 0x47e], 4
        je     loc_3_07B3
        mov    ecx, dword ptr [ebx + 0x98]
        cmp    dword ptr [ecx + 4], 0
        je     loc_3_0518
        mov    eax, dword ptr [ecx + 8]
        mov    edx, dword ptr [ecx + 0xc]
        cmp    eax, edx
        ja     loc_3_054C
        xchg   al, ah
        rol    eax, 0x10
        xchg   al, ah
        xor    al, al
        xchg   dl, dh
        rol    edx, 0x10
        xchg   dl, dh
        mov    byte ptr [edi + 0x32], al
        mov    byte ptr [edi + 0x39], al
        mov    dword ptr [edi + 0x35], edx
        mov    dword ptr [edi + 0x32], eax
        mov    byte ptr [edi + 0x30], 0x47
        mov    byte ptr [edi + 0xa], 0xa
        mov    byte ptr [edi + 0x39], al
loc_3_050B:
        xor    eax, eax
        mov    dword ptr [ebx + 0x7c], eax
        mov    dword ptr [edi + 0xc], 0
        ret    
loc_3_0518:
        mov    eax, dword ptr [ecx + 8]
        cmp    eax, dword ptr [ecx + 0xc]
        ja     loc_3_054C
        mov    eax, dword ptr [ecx + 0xc]
        mov    edx, dword ptr [ecx + 8]
        sub    eax, edx
        xchg   al, ah
        rol    eax, 0x10
        xchg   al, ah
        mov    dword ptr [edi + 0x36], eax
        xchg   dl, dh
        rol    edx, 0x10
        xchg   dl, dh
        mov    dword ptr [edi + 0x32], edx
        mov    byte ptr [edi + 0x30], 0xa5
        mov    byte ptr [edi + 0xa], 0xc
        mov    word ptr [edi + 0x3a], 0
        jmp    loc_3_050B
loc_3_054C:
        mov    dword ptr [ebx + 0xa0], 0x16
        mov    word ptr [ebx + 0x6a], 0x10
        mov    al, 1
        ret    
loc_3_055F:
        adc    byte ptr cs:[edi], dl
        pop    ss
sub_3_0563:
        mov    byte ptr [edi + 0x30], 0x42
        mov    byte ptr [edi + 0xa], 0xa
        mov    word ptr [ebx + 0xe], 0x1e
        mov    word ptr [ebx + 0xc], 0x1e
        rol    al, 1
        mov    byte ptr [edi + 0x33], ah
        mov    byte ptr [edi + 0x31], al
        movzx  eax, ah
        movzx  eax, byte ptr [eax + 0x55f]   ; -> loc_3_055F
        xchg   al, ah
        mov    word ptr [edi + 0x37], ax
        xchg   al, ah
        mov    dword ptr [edi + 0xc], 0x248
        mov    cl, 1
        call   sub_3_05BC
        or     eax, eax
        mov    al, 1
        je     loc_3_05B7
        xor    eax, eax
        mov    word ptr [edi + 0x34], ax
        mov    byte ptr [edi + 0x39], al
        mov    byte ptr [edi + 0x36], al
        mov    byte ptr [edi + 0x32], 0x40
        xor    al, al
loc_3_05B7:
        ret    
sub_3_05B8:
        push   edx
        push   edi
        jmp    loc_3_05F3
sub_3_05BC:
        push   edx
        push   edi
        mov    dword ptr [ebx + 0x7c], eax
        cmp    eax, 8
        ja     loc_3_05CB
        mov    eax, 8
loc_3_05CB:
        sub    esp, 0x10
        mov    edi, esp
        mov    word ptr [edi], 3
        mov    word ptr [edi + 4], ax
        mov    word ptr [edi + 0xa], 9
        mov    byte ptr [edi + 0xc], cl
        push   edi
        call   sub_1_0F54
        mov    edi, dword ptr [edi + 6]
        add    esp, 0x14
        or     edi, edi
        je     loc_3_0624
loc_3_05F3:
        mov    eax, dword ptr [ebx + 0x20]
        mov    dword ptr [ebx + 0x80], edi
        mov    dword ptr [eax + 0x18], edi
        and    dword ptr [ebx + 0x6c], 0xfffffffd
        or     dword ptr [ebx + 0x6c], 0x200
        mov    byte ptr [ebx + 0xb0], 0
        push   edi
        push   ebx
        call   sub_1_0F68
        add    esp, 4
        pop    edi
        or     eax, eax
loc_3_061F:
        mov    eax, edi
        pop    edi
        pop    edx
        ret    
loc_3_0624:
        mov    word ptr [ebx + 0x6a], 0x1d
        mov    dword ptr [ebx + 0xa0], 8
        jmp    loc_3_061F
loc_3_0636:
        mov    byte ptr [edi + 0x30], 1
        mov    byte ptr [edi + 0xa], 6
        xor    eax, eax
        mov    dword ptr [edi + 0x32], eax
        xor    al, al
        ret    
loc_3_0646:
        mov    edx, dword ptr [ebx + 0x98]
        mov    ecx, dword ptr [edx + 8]
        cmp    dword ptr [edx + 4], 1
        jne    loc_3_0661
        xchg   ch, cl
        rol    ecx, 0x10
        xchg   ch, cl
        call   sub_1_0F9C
loc_3_0661:
        mov    byte ptr [edi + 0x30], 0x2b
        mov    byte ptr [edi + 0xa], 0xa
        xchg   cl, ch
        ror    ecx, 0x10
        xchg   cl, ch
        mov    dword ptr [edi + 0x32], ecx
        xor    eax, eax
        mov    dword ptr [edi + 0x36], eax
        ret    
loc_3_0679:
        test   byte ptr [esi + 0x22], 8
        jne    loc_3_0689
        mov    ax, 0x100
        call   sub_3_0563
        ret    
loc_3_0689:
        mov    edx, 0xf97
        mov    ecx, dword ptr [ebx + 0x98]
        mov    dword ptr [ecx + 4], edx
        mov    dword ptr [ebx + 0xa0], 0
        mov    word ptr [ebx + 0x6a], 0
        mov    al, 1
        ret    
loc_3_06AA:
        mov    edx, dword ptr [ebx + 0x98]
        cmp    dword ptr [edx + 4], 1
        ja     loc_3_06D9
        mov    dword ptr [edx + 8], 0x924
        je     loc_3_06C6
        mov    dword ptr [edx + 8], 0x800
loc_3_06C6:
        mov    dword ptr [ebx + 0xa0], 0
        mov    word ptr [ebx + 0x6a], 0
loc_3_06D6:
        mov    al, 1
        ret    
loc_3_06D9:
        mov    dword ptr [ebx + 0xa0], 0x16
        mov    word ptr [ebx + 0x6a], 0x10
        jmp    loc_3_06D6
loc_3_06EB:
        mov    edx, dword ptr [ebx + 0x98]
        mov    dword ptr [edx + 4], 0x96
        mov    dword ptr [ebx + 0xa0], 0
        mov    word ptr [ebx + 0x6a], 0
        mov    al, 1
        ret    
loc_3_070B:
        mov    edx, dword ptr [ebx + 0x98]
        mov    dword ptr [edx + 8], 0x96
        mov    dword ptr [edx + 0xc], 0x96
        mov    dword ptr [ebx + 0xa0], 0
        mov    word ptr [ebx + 0x6a], 0
        mov    al, 1
        ret    
loc_3_0732:
        mov    dword ptr [edx + 4], 3
        je     loc_3_074E
loc_3_073B:
        mov    dword ptr [ebx + 0xa0], 0
        mov    word ptr [ebx + 0x6a], 0
        mov    al, 1
        ret    
loc_3_074E:
        cmp    dword ptr [edx + 8], 0x96
        je     loc_3_073B
        mov    dword ptr [ebx + 0xa0], 0x57
        mov    word ptr [ebx + 0x6a], 0x10
        mov    al, 1
        ret    
loc_3_076A:
        mov    edx, dword ptr [ebx + 0x98]
        mov    eax, dword ptr [edx + 4]
        mov    ah, 1
        call   sub_3_0563
        ret    
loc_3_077B:
        mov    edx, dword ptr [ebx + 0x98]
        xor    ax, ax
        call   sub_3_0563
        ret    
loc_3_078A:
        mov    edx, dword ptr [ebx + 0x98]
        xor    ax, ax
        call   sub_3_0563
        mov    edx, dword ptr [edx + 4]
        mov    byte ptr [edi + 0x36], dl
        ret    
loc_3_079F:
        mov    ax, 0x101
        call   sub_3_0563
        ret    
loc_3_07A9:
        mov    ax, 0x101
        call   sub_3_0563
        ret    
loc_3_07B3:
        mov    al, 0x1c
        mov    byte ptr [edi + 0x34], al
        mov    cl, 1
        call   sub_3_05BC
        or     eax, eax
        mov    al, 1
        je     loc_3_07F3
        mov    byte ptr [edi + 0x31], 0
        mov    byte ptr [edi + 0x32], 0xe
        cmp    word ptr [ebx + 0x92], 0xcd09
        je     loc_3_07DC
        mov    byte ptr [edi + 0x32], 0x4e
loc_3_07DC:
        mov    byte ptr [edi + 0x30], 0x1a
        mov    byte ptr [edi + 0xa], 6
        xor    al, al
        mov    byte ptr [edi + 0x35], al
        mov    byte ptr [edi + 0x33], al
        mov    dword ptr [edi + 0xc], 0x248
loc_3_07F3:
        ret    
loc_3_07F4:
        mov    edx, dword ptr [ebx + 0x98]
        movzx  ecx, word ptr [esi + 0x474]
        mov    word ptr [edx + 0x10], cx
        movzx  ecx, byte ptr [edx + 0x12]
        cmp    ecx, 0xff
        jne    loc_3_0818
        movzx  ecx, byte ptr [esi + 0x475]
loc_3_0818:
        mov    byte ptr [edx + 0x13], cl
        cmp    cl, byte ptr [esi + 0x474]
        jb     loc_3_0894
        je     loc_3_08E5
        cmp    cl, byte ptr [esi + 0x475]
        ja     loc_3_0894
        je     loc_3_08CD
        jmp    loc_3_08CB
loc_3_083C:
        movzx  eax, byte ptr [esi + 0x136]
        mov    dword ptr [edx + 4], eax
        mov    dword ptr [edx + 0x14], 0
        mov    al, byte ptr [esi + 0x474]
        mov    byte ptr [edx + 0x13], al
loc_3_0856:
        movzx  eax, byte ptr [esi + 0x137]
        mov    dword ptr [edx + 8], eax
        mov    al, 0xaa
        test   byte ptr [esi + 0x482], 0x10
        je     loc_3_0877
        movzx  eax, byte ptr [esi + 0x487]
        dec    eax
        mov    dword ptr [edx + 8], eax
        inc    eax
loc_3_0877:
        movzx  ecx, byte ptr [esi + 0x137]
        inc    cl
        sub    cl, byte ptr [esi + 0x136]
loc_3_0886:
        cmp    byte ptr [esi + ecx*8 + 0x13a], al
        je     loc_3_08A7
        sub    cl, 1
        jae    loc_3_0886
loc_3_0894:
        mov    word ptr [ebx + 0x6a], 0x10
        mov    dword ptr [ebx + 0xa0], 0x57
        mov    al, 1
        ret    
loc_3_08A7:
        mov    eax, dword ptr [esi + ecx*8 + 0x13c]
        xchg   ah, al
        rol    eax, 0x10
        xchg   ah, al
loc_3_08B5:
        mov    dword ptr [edx + 0xc], eax
        mov    word ptr [ebx + 0x6a], 0
        mov    dword ptr [ebx + 0xa0], 0
        mov    al, 1
        ret    
loc_3_08CB:
        jmp    loc_3_0921
loc_3_08CD:
        movzx  ecx, byte ptr [esi + 0x133]
        mov    dword ptr [edx + 4], ecx
        mov    eax, dword ptr [esi + 0x48c]
        mov    dword ptr [edx + 0x14], eax
        jmp    loc_3_0856
loc_3_08E5:
        test   byte ptr [esi + 0x482], 2
        je     loc_3_083C
        movzx  eax, byte ptr [esi + 0x136]
        mov    dword ptr [edx + 4], eax
        xor    eax, eax
        mov    dword ptr [edx + 0x14], eax
        movzx  ecx, byte ptr [esi + 0x486]
        cmp    cl, 0xff
        jne    loc_3_0916
        movzx  ecx, byte ptr [esi + 0x133]
        dec    cl
loc_3_0916:
        mov    dword ptr [edx + 8], ecx
        mov    eax, dword ptr [esi + 0x488]
        jmp    loc_3_08B5
loc_3_0921:
        mov    edx, dword ptr [ebx + 0x98]
        movzx  ecx, byte ptr [edx + 0x13]
        mov    byte ptr [edi + 0x36], cl
        mov    eax, 8
        mov    word ptr [edi + 0x37], ax
        xchg   al, ah
        mov    dword ptr [ebx + 0x7c], eax
        sub    esp, 0x10
        mov    edx, esp
        mov    word ptr [edx + 4], ax
        mov    word ptr [edx + 0xa], 9
        mov    word ptr [edx], 3
        mov    byte ptr [edx + 0xc], 8
        push   edx
        call   sub_1_0F54
        mov    eax, dword ptr [edx + 6]
        add    esp, 0x14
        mov    edx, eax
        or     eax, eax
        mov    al, 1
        je     loc_3_0995
        push   edi
        mov    edi, edx
        call   sub_3_05B8
        pop    edi
        mov    byte ptr [edi + 0x30], 0x43
        mov    byte ptr [edi + 0xa], 0xa
        mov    al, byte ptr [esi + 0x7d]
        shl    al, 5
        or     al, 2
        mov    byte ptr [edi + 0x31], al
        xor    eax, eax
        mov    dword ptr [edi + 0x32], eax
        mov    byte ptr [edi + 0x39], 0x80
        mov    dword ptr [edi + 0xc], 0x248
loc_3_0995:
        ret    
loc_3_0996:
        mov    edx, dword ptr [ebx + 0x98]
        mov    eax, dword ptr [edx + 4]
        test   eax, 0xffffff00
        jne    loc_3_09E2
        cmp    al, byte ptr [esi + 0x137]
        ja     loc_3_09E2
        cmp    al, byte ptr [esi + 0x136]
        jb     loc_3_09E2
        test   byte ptr [esi + 0x482], 0x10
        je     loc_3_09C7
        cmp    al, byte ptr [esi + 0x487]
        jae    loc_3_09E2
loc_3_09C7:
        movzx  ecx, byte ptr [esi + 0x137]
        sub    cl, byte ptr [esi + 0x136]
loc_3_09D4:
        cmp    byte ptr [esi + ecx*8 + 0x13a], al
        je     loc_3_09F5
        sub    cl, 1
        jae    loc_3_09D4
loc_3_09E2:
        mov    word ptr [ebx + 0x6a], 0x10
        mov    dword ptr [ebx + 0xa0], 0x57
        mov    al, 1
        ret    
loc_3_09F5:
        mov    eax, dword ptr [esi + ecx*8 + 0x13c]
        xchg   ah, al
        rol    eax, 0x10
        xchg   ah, al
        mov    dword ptr [edx + 8], eax
        movzx  eax, byte ptr [esi + ecx*8 + 0x139]
        rol    al, 4
        mov    dword ptr [edx + 0xc], eax
        mov    word ptr [ebx + 0x6a], 0
        mov    dword ptr [ebx + 0xa0], 0
        mov    al, 1
        ret    
loc_3_0A27:
        and    byte ptr [ebx + 0x6d], 0x7f
        mov    al, 4
        ret    
loc_3_0A2E:
        test   byte ptr [esi + 0x47e], 1
        je     loc_3_0A41
        mov    ax, 0x100
        call   sub_3_0563
        ret    
loc_3_0A41:
        xor    eax, eax
        and    byte ptr [esi + 0x47e], 0xfd
        mov    word ptr [ebx + 0x6a], ax
        mov    dword ptr [ebx + 0xa0], eax
        mov    al, 1
        ret    
loc_3_0A57:
        test   byte ptr [esi + 0x47e], 2
        je     loc_3_0A95
        mov    ecx, dword ptr [esi + 0x47a]
        call   sub_1_0FC9
        mov    byte ptr [edi + 0x32], 0
        mov    byte ptr [edi + 0x39], 0
        mov    dword ptr [edi + 0x35], ecx
        mov    ecx, dword ptr [esi + 0x476]
        call   sub_1_0FC9
        mov    dword ptr [edi + 0x32], ecx
        mov    byte ptr [edi + 0x30], 0x47
        mov    byte ptr [edi + 0xa], 0xa
        mov    word ptr [edi + 0x3a], 0
        xor    al, al
        ret    
loc_3_0A95:
        mov    word ptr [ebx + 0x6a], 0x20
        mov    dword ptr [ebx + 0xa0], 0x15
        mov    al, 1
        ret    
loc_3_0AA8:
        mov    eax, dword ptr [ebx + 0x14]
        mov    eax, dword ptr [eax + 4]
        add    dword ptr [ebx + 0x14], 8
        mov    dword ptr [ebx + 0x10], eax
        mov    dword ptr [edi + 0x44], 0
        cmp    byte ptr [edi + 3], 1
        ja     loc_3_0B16
loc_3_0AC2:
        cmp    byte ptr [edi + 0x30], 0
        je     loc_3_0AE0
        movzx  eax, word ptr [ebx + 0x92]
        xor    ah, ah
        cmp    eax, 0x19
        ja     loc_3_0B40
        call   sub_3_0B8B
        ret    
loc_3_0AE0:
        cmp    byte ptr [edi + 3], 1
        jne    loc_3_0B0E
        mov    word ptr [ebx + 0xc], 0xf
        mov    word ptr [ebx + 0xe], 0xf
        mov    dword ptr [edi + 0xc], 0
        mov    byte ptr [edi + 0x31], 0
        movzx  eax, word ptr [ebx + 0x92]
        xor    ah, ah
        call   sub_3_0378
        ret    
loc_3_0B0E:
        call   sub_3_0BEF
        mov    al, 1
        ret    
loc_3_0B16:
        test   byte ptr [edi + 3], 0x80
        jne    loc_3_0B53
        cmp    byte ptr [edi + 3], 4
        jne    loc_3_0B32
        cmp    byte ptr [edi + 4], 2
        cmp    byte ptr [edi + 4], 8
        je     loc_3_0B32
        cmp    byte ptr [edi + 4], 0x18
        je     loc_3_0B3A
loc_3_0B32:
        cmp    byte ptr [edi + 0x4c], 5
        jb     loc_3_0B3A
        jmp    loc_3_0AC2
loc_3_0B3A:
        inc    byte ptr [edi + 0x4c]
        mov    al, 1
        ret    
loc_3_0B40:
        mov    word ptr [ebx + 0x6a], 0x16
        mov    dword ptr [ebx + 0xa0], 0x1f
        mov    al, 1
        ret    
loc_3_0B53:
        mov    ecx, dword ptr [edi + 0x1c]
        mov    al, byte ptr [ecx + 2]
        and    eax, 0xf
        cmp    al, 0
        je     loc_3_0B32
        cmp    al, 5
        jne    loc_3_0B69
        jmp    loc_3_0AC2
loc_3_0B69:
        cmp    al, 1
        jne    loc_3_0B76
        mov    byte ptr [edi + 3], 1
        jmp    loc_3_0AC2
loc_3_0B76:
        cmp    al, 3
        je     loc_3_0B32
        cmp    al, 4
        je     loc_3_0B32
        cmp    al, 0xb
        je     loc_3_0B32
        cmp    al, 2
        jne    loc_3_0B86
loc_3_0B86:
        jmp    loc_3_0AC2
sub_3_0B8B:
        pop    ebp
        or     al, 0
        add    byte ptr [ebp + 0xc], bl      ; -> loc_3_0C5D
        add    byte ptr [eax], al
        jo     loc_3_0BA5
        add    byte ptr [eax], al
        pop    ebp
        or     al, 0
        add    byte ptr [esi + 0xd], bh      ; -> loc_3_0D7E
        add    byte ptr [eax], al
        stosb  byte ptr es:[edi], al
        or     eax, 0xdd40000                ; -> loc_3_0DD4
loc_3_0BA5:
        add    byte ptr [eax], al
        dec    byte ptr [0xea20000]          ; -> loc_3_0EA2
        add    byte ptr [eax], al
        and    dword ptr [edi], ecx
        add    byte ptr [eax], al
        mov    cl, byte ptr [edi]
        add    byte ptr [eax], al
        push   es
        adc    dword ptr [eax], eax
        add    byte ptr [ebp + 0xc], bl      ; -> loc_3_0C5D
        add    byte ptr [eax], al
        pop    ebp
        or     al, 0
        add    byte ptr [ebp + 0xc], ah      ; -> loc_3_0C65
        add    byte ptr [eax], al
        test   byte ptr [eax + eax], 0
        or     al, 0
        add    byte ptr [ebp + 0xc], bl      ; -> loc_3_0C5D
        add    byte ptr [eax], al
        pop    ebp
        or     al, 0
        add    byte ptr [ebp + 0xc], bl      ; -> loc_3_0C5D
        add    byte ptr [eax], al
        pop    ebp
        or     al, 0
        add    byte ptr [ebp + 0xc], bl      ; -> loc_3_0C5D
        add    byte ptr [eax], al
        pop    ebp
        or     al, 0
        add    byte ptr [ebp + 0xc], bl      ; -> loc_3_0C5D
        add    byte ptr [eax], al
        pop    ebp
        or     al, 0
        db  000h
sub_3_0BEF:
        cmp    byte ptr [edi + 3], 1
        jne    loc_3_0C06
        mov    word ptr [ebx + 0x6a], 0
        mov    dword ptr [ebx + 0xa0], 0
        ret    
loc_3_0C06:
        cmp    byte ptr [edi + 4], 8
        je     loc_3_0C3B
        cmp    byte ptr [edi + 4], 0x18
        je     loc_3_0C4C
        mov    eax, dword ptr [edi + 0x1c]
        test   byte ptr [edi + 3], 0x80
        je     loc_3_0C3B
        movzx  edx, byte ptr [eax + 2]
        and    dl, 0xf
        movzx  eax, word ptr [edx*2 + 0x125e] ; -> obj3:0x125E
        movzx  ecx, ah
        movzx  eax, al
        mov    word ptr [ebx + 0x6a], ax
        mov    dword ptr [ebx + 0xa0], ecx
        ret    
loc_3_0C3B:
        mov    word ptr [ebx + 0x6a], 0x21
        mov    dword ptr [ebx + 0xa0], 0x15
        ret    
loc_3_0C4C:
        mov    word ptr [ebx + 0x6a], 0x1b
        mov    dword ptr [ebx + 0xa0], 0x1f
        ret    
loc_3_0C5D:
        call   sub_3_0BEF
        mov    al, 1
        ret    
loc_3_0C65:
        cmp    byte ptr [edi + 0x30], 0x1a
        je     loc_3_0CDC
        cmp    byte ptr [edi + 0x30], 0x15
        je     loc_3_0CE6
        cmp    byte ptr [edi + 3], 1
        ja     loc_3_0CD4
        and    byte ptr [esi + 0x47e], 0xfd
        or     byte ptr [esi + 0x47e], 1
        cmp    byte ptr [edi + 0x30], 0x47
        je     loc_3_0CC0
        mov    edx, dword ptr [edi + 0x32]
        xchg   dl, dh
        rol    edx, 0x10
        xchg   dl, dh
        mov    ecx, dword ptr [edi + 0x36]
        xchg   cl, ch
        rol    ecx, 0x10
        xchg   cl, ch
        add    ecx, edx
loc_3_0CA1:
        mov    dword ptr [esi + 0x476], edx
        mov    dword ptr [esi + 0x47a], ecx
        mov    dword ptr [ebx + 0xa0], 0
        mov    word ptr [ebx + 0x6a], 0
        mov    al, 1
        ret    
loc_3_0CC0:
        mov    ecx, dword ptr [edi + 0x32]
        call   sub_1_0F9C
        mov    edx, ecx
        mov    ecx, dword ptr [edi + 0x35]
        call   sub_1_0F9C
        jmp    loc_3_0CA1
loc_3_0CD4:
        call   sub_3_0BEF
        mov    al, 1
        ret    
loc_3_0CDC:
        cmp    byte ptr [edi + 3], 1
        je     loc_3_0FBA
loc_3_0CE6:
        or     byte ptr [esi + 0x47e], 4
        and    byte ptr [edi + 0x31], 0xe0
        jmp    loc_3_04C1
loc_3_0CF6:
        mov    eax, dword ptr [ebx + 0x80]
        and    byte ptr [esi + 0x47e], 0xfc
        cmp    byte ptr [edi + 3], 1
        jne    loc_3_0D71
        cmp    byte ptr [edi + 0x30], 0x42
        jne    loc_3_0D52
        mov    ecx, dword ptr [eax + 8]
        mov    dword ptr [edi + 0x32], ecx
        xchg   cl, ch
        rol    ecx, 0x10
        xchg   cl, ch
        mov    dword ptr [esi + 0x476], ecx
        cmp    byte ptr [eax + 1], 0x11
        jne    loc_3_0D52
        xor    ecx, ecx
        mov    dword ptr [edi + 0x36], ecx
        mov    byte ptr [edi + 0x30], 0x2b
        mov    byte ptr [edi + 0xa], 0xa
        mov    dword ptr [ebx + 0x7c], ecx
        mov    dword ptr [edi + 0xc], 0
        and    byte ptr [edi + 0x31], 0xe0
        mov    word ptr [ebx + 0xc], 0xf
        mov    word ptr [ebx + 0xe], 0xf
        xor    al, al
        ret    
loc_3_0D52:
        or     byte ptr [esi + 0x47e], 2
        call   sub_3_1244
        mov    word ptr [ebx + 0x6a], 0
        mov    dword ptr [ebx + 0xa0], 0
        mov    al, 1
        ret    
loc_3_0D71:
        call   sub_3_1244
        call   sub_3_0BEF
        mov    al, 1
        ret    
loc_3_0D7E:
        mov    eax, dword ptr [ebx + 0x80]
        cmp    byte ptr [edi + 3], 1
        jne    loc_3_0D9D
        mov    edx, dword ptr [eax + 8]
        mov    ecx, dword ptr [ebx + 0x98]
        xchg   dl, dh
        rol    edx, 0x10
        xchg   dl, dh
        mov    dword ptr [ecx + 8], edx
loc_3_0D9D:
        call   sub_3_1244
        call   sub_3_0BEF
        mov    al, 1
        ret    
loc_3_0DAA:
        mov    eax, dword ptr [ebx + 0x80]
        cmp    byte ptr [edi + 3], 1
        jne    loc_3_0E7D
        test   byte ptr [eax + 0x10], 0x80
        je     loc_3_0E7D
        push   edi
        push   esi
        mov    ecx, dword ptr [ebx + 0x98]
        lea    esi, [eax + 0x11]
        lea    edi, [ecx + 4]
        jmp    loc_3_0E53
loc_3_0DD4:
        mov    eax, dword ptr [ebx + 0x80]
        cmp    byte ptr [edi + 3], 1
        jne    loc_3_0E7D
        test   byte ptr [eax + 0x1f], 0x80
        je     loc_3_0E8A
        push   edi
        push   esi
        mov    ecx, dword ptr [ebx + 0x98]
        lea    esi, [eax + 0x20]
        lea    edi, [ecx + 8]
        jmp    loc_3_0E53
loc_3_0DFE:
        mov    eax, dword ptr [ebx + 0x80]
        cmp    byte ptr [edi + 3], 1
        jne    loc_3_0E7D
        movzx  edx, byte ptr [eax + 5]
        cmp    dl, 0x40
        jae    loc_3_0E8A
        and    dl, 0xf0
        or     dl, 0x10
        mov    byte ptr [eax + 5], dl
        cmp    byte ptr [eax + 1], 0x11
        je     loc_3_0E29
        and    byte ptr [esi + 0x47e], 0xfe
loc_3_0E29:
        push   edi
        push   esi
        push   eax
        mov    ecx, dword ptr [ebx + 0x98]
        lea    esi, [eax + 6]
        lea    edi, [ecx + 5]
        mov    dword ptr [edi + 7], 0
        movsw  word ptr es:[edi], word ptr [esi]
        lodsd  eax, dword ptr [esi]
        mov    edx, eax
        lodsd  eax, dword ptr [esi]
        ror    eax, 8
        stosd  dword ptr es:[edi], eax
        mov    eax, edx
        dec    edi
        stosd  dword ptr es:[edi], eax
        lea    edi, [ecx + 4]
        pop    eax
        jmp    loc_3_0E70
loc_3_0E53:
        push   edi
        push   eax
        inc    edi
        mov    dword ptr [esi + 8], 0
        mov    ecx, 7
        xor    eax, eax
loc_3_0E64:
        lodsb  al, byte ptr [esi]
        rol    eax, 0xc
        lodsb  al, byte ptr [esi]
        or     al, ah
        stosb  byte ptr es:[edi], al
        loop   loc_3_0E64
        pop    eax
        pop    edi
loc_3_0E70:
        movzx  ecx, byte ptr [eax + 5]
        rol    ecx, 4
        or     cl, ch
        mov    byte ptr [edi], cl
        pop    esi
        pop    edi
loc_3_0E7D:
        call   sub_3_1244
        call   sub_3_0BEF
        mov    al, 1
        ret    
loc_3_0E8A:
        call   sub_3_1244
        mov    word ptr [ebx + 0x6a], 0x10
        mov    dword ptr [ebx + 0xa0], 0xd
        mov    al, 1
        ret    
loc_3_0EA2:
        mov    eax, dword ptr [ebx + 0x80]
        cmp    byte ptr [edi + 3], 1
        jne    loc_3_0EC1
        mov    ecx, dword ptr [ebx + 0x98]
        cmp    byte ptr [eax + 1], 0x11
        jne    loc_3_0EFA
        mov    dword ptr [ecx + 4], 0
loc_3_0EC1:
        call   sub_3_1244
        mov    edx, ecx
        mov    ecx, dword ptr [esi + 0x476]
        call   sub_1_0FC9
        xchg   cl, ch
        rol    ecx, 0x10
        xchg   cl, ch
        mov    dword ptr [edx + 8], ecx
        mov    ecx, dword ptr [esi + 0x47a]
        call   sub_1_0FC9
        xchg   cl, ch
        rol    ecx, 0x10
        xchg   cl, ch
        mov    dword ptr [edx + 0xc], ecx
        call   sub_3_0BEF
        mov    al, 1
        ret    
loc_3_0EFA:
        and    byte ptr [esi + 0x47e], 0xfe
        test   dword ptr [esi + 0x47e], 2
        jne    loc_3_0F16
        mov    dword ptr [ecx + 4], 2
        jmp    loc_3_0EC1
loc_3_0F16:
        mov    dword ptr [ecx + 4], 1
        mov    edx, ecx
        jmp    loc_3_0EC1
loc_3_0F21:
        mov    eax, dword ptr [ebx + 0x80]
        cmp    byte ptr [edi + 3], 1
        jne    loc_3_0F5C
        mov    edx, dword ptr [ebx + 0x98]
        cmp    byte ptr [eax + 3], 8
        jne    loc_3_0F72
        cmp    byte ptr [eax + 0xc], 0xe
        jne    loc_3_0F72
        push   edi
        push   esi
        push   eax
        lea    esi, [eax + 0x14]
        lea    edi, [edx + 4]
        mov    ecx, 4
loc_3_0F4D:
        lodsw  ax, word ptr [esi]
        or     al, al
        je     loc_3_0F69
        dec    al
loc_3_0F55:
        stosw  word ptr es:[edi], ax
        loop   loc_3_0F4D
        pop    eax
        pop    esi
        pop    edi
loc_3_0F5C:
        call   sub_3_1244
        call   sub_3_0BEF
        mov    al, 1
        ret    
loc_3_0F69:
        mov    eax, 4
        sub    eax, ecx
        jmp    loc_3_0F55
loc_3_0F72:
        mov    word ptr [ebx + 0x6a], 0x10
        mov    dword ptr [ebx + 0xa0], 0x1f
        call   sub_3_1244
        mov    al, 1
        ret    
loc_3_0F8A:
        cmp    byte ptr [edi + 0x30], 0x15
        je     loc_3_1038
        mov    edx, dword ptr [ebx + 0x98]
        push   esi
        push   edi
        lea    edi, [esi + 0x494]
        lea    esi, [edx + 4]
        mov    ecx, 4
loc_3_0FAA:
        lodsw  ax, word ptr [esi]
        inc    al
        or     ah, ah
        jne    loc_3_0FB4
        xor    eax, eax
loc_3_0FB4:
        stosw  word ptr es:[edi], ax
        loop   loc_3_0FAA
        pop    edi
        pop    esi
loc_3_0FBA:
        mov    eax, dword ptr [ebx + 0x80]
        cmp    byte ptr [edi + 3], 1
        jne    loc_3_104B
        cmp    byte ptr [eax + 3], 8
        jne    loc_3_1058
        mov    byte ptr [eax + 0xc], 0xe
        mov    byte ptr [eax + 0xd], 0xe
        mov    byte ptr [eax + 0xe], 4
        mov    dword ptr [eax + 4], 0
        mov    ecx, dword ptr [esi + 0x490]
        xchg   cl, ch
        ror    ecx, 0x10
        xchg   cl, ch
        mov    dword ptr [eax + 8], ecx
        mov    ecx, dword ptr [esi + 0x494]
        and    dword ptr [eax + 0x14], ecx
        mov    ecx, dword ptr [esi + 0x498]
        and    dword ptr [eax + 0x18], ecx
        mov    dword ptr [edi + 0x32], 0
        mov    byte ptr [eax], 0
        mov    edx, 0x1c
        mov    byte ptr [edi + 0x34], dl
        mov    byte ptr [edi + 0x31], 0x10
        mov    byte ptr [edi + 0x30], 0x15
        mov    byte ptr [edi + 0xa], 6
        mov    dword ptr [edi + 0xc], 0x288
        mov    byte ptr [edi + 3], 0
        mov    dword ptr [ebx + 0x7c], edx
        xor    al, al
        ret    
loc_3_1038:
        mov    eax, dword ptr [ebx + 0x80]
        cmp    byte ptr [edi + 3], 1
        jne    loc_3_104B
        or     byte ptr [esi + 0x47e], 4
loc_3_104B:
        call   sub_3_1244
        call   sub_3_0BEF
        mov    al, 1
        ret    
loc_3_1058:
        mov    word ptr [ebx + 0x6a], 0x10
        mov    dword ptr [ebx + 0xa0], 0x1f
        call   sub_3_1244
        mov    al, 1
        ret    
loc_3_1070:
        mov    ecx, dword ptr [ebx + 0x98]
        mov    eax, dword ptr [ebx + 0x80]
        cmp    byte ptr [edi + 3], 1
        jne    loc_3_10CD
        mov    edx, 0x4390
        cmp    byte ptr [eax + 1], 0x11
        je     loc_3_10A6
loc_3_108D:
        and    edx, 0xffffbfff
        and    byte ptr [esi + 0x47e], 0xfe
        test   byte ptr [esi + 0x482], 1
        jne    loc_3_10A6
        and    edx, 0xffffffef
loc_3_10A6:
        or     edx, 2
        cmp    byte ptr [esi + 0x79], 0
        jne    loc_3_10B2
        and    edx, 0xfffffffd
loc_3_10B2:
        mov    dword ptr [ecx + 4], edx
        mov    dword ptr [ebx + 0xa0], 0
        mov    word ptr [ebx + 0x6a], 0
        call   sub_3_1244
        mov    al, 1
        ret    
loc_3_10CD:
        mov    edx, dword ptr [edi + 0x1c]
        movzx  edx, byte ptr [edx + 2]
        and    dl, 0xf
        cmp    dl, 5
        mov    edx, 0xb91
        jne    loc_3_108D
        call   sub_3_1244
        call   sub_3_0BEF
        mov    al, 1
        ret    
        db  0C7h, 083h, 0A0h, 000h, 000h, 000h, 01Fh, 000h, 000h, 000h, 066h, 0C7h, 043h, 06Ah, 016h, 000h
        db  0E8h, 041h, 001h, 000h, 000h, 0B0h, 001h, 0C3h
loc_3_1106:
        cmp    byte ptr [edi + 3], 1
        jne    loc_3_1224
        mov    eax, dword ptr [ebx + 0x80]
        lea    edx, [eax + 4]
        movzx  ecx, word ptr [eax]
        xchg   cl, ch
        add    eax, ecx
        movzx  ecx, byte ptr [edi + 0x36]
        mov    ch, byte ptr [esi + 0x475]
        push   ebx
        push   esi
        mov    esi, dword ptr [ebx + 0x98]
loc_3_1132:
        cmp    byte ptr [edx + 3], 0xa0
        je     loc_3_1167
loc_3_1138:
        add    edx, 0xb
        cmp    edx, eax
        jl     loc_3_1132
loc_3_113F:
        pop    esi
        pop    ebx
        mov    word ptr [ebx + 0x6a], 0x14
        mov    dword ptr [ebx + 0xa0], 0xb
loc_3_1151:
        mov    edx, dword ptr [ebx + 0x80]
        push   esi
        mov    esi, 0x122c                   ; -> loc_3_122C
        VMMCall  VMM.Schedule_Global_Event       ; cd200e000100
        pop    esi
        mov    al, 1
        ret    
loc_3_1167:
        and    byte ptr [edx + 1], 0xf0
        cmp    byte ptr [edx + 1], 0x10
        jne    loc_3_1138
        cmp    byte ptr [edx], cl
        jne    loc_3_113F
        movzx  ebx, byte ptr [edx + 8]
        mov    dword ptr [esi + 4], ebx
        add    edx, 0xb
        cmp    byte ptr [edx + 3], 0xa1
        jne    loc_3_113F
        and    byte ptr [edx + 1], 0xf0
        cmp    byte ptr [edx + 1], 0x10
        jne    loc_3_113F
        movzx  ebx, byte ptr [edx + 8]
        mov    dword ptr [esi + 8], ebx
        add    edx, 0xb
        cmp    byte ptr [edx + 3], 0xa2
        jne    loc_3_113F
        and    byte ptr [edx + 1], 0xf0
        cmp    byte ptr [edx + 1], 0x10
        jne    loc_3_113F
        mov    ebx, dword ptr [edx + 7]
        movzx  bx, bh
        rol    ebx, 0x10
        xchg   bh, bl
        mov    dword ptr [esi + 0xc], ebx
        mov    ebx, dword ptr [esi + 4]
loc_3_11BB:
        cmp    byte ptr [edx + 2], bl
        je     loc_3_11CC
loc_3_11C0:
        add    edx, 0xb
        cmp    edx, eax
        jle    loc_3_11BB
        jmp    loc_3_113F
loc_3_11CC:
        and    byte ptr [edx + 1], 0xf0
        cmp    byte ptr [edx + 1], 0x10
        jne    loc_3_11C0
        cmp    byte ptr [edx + 3], 0x99
        jae    loc_3_11C0
        cmp    byte ptr [edx], cl
        jne    loc_3_113F
        mov    byte ptr [esi + 0x13], cl
        mov    ecx, dword ptr [edx + 7]
        call   sub_1_0F9C
        mov    dword ptr [esi + 0x14], ecx
        pop    esi
        pop    ebx
        movzx  ecx, byte ptr [edi + 0x36]
        cmp    cl, byte ptr [esi + 0x475]
        jne    loc_3_120F
        mov    ecx, dword ptr [esi + 0x48c]
        mov    edx, dword ptr [ebx + 0x98]
        mov    dword ptr [edx + 0x14], ecx
loc_3_120F:
        mov    word ptr [ebx + 0x6a], 0
        mov    dword ptr [ebx + 0xa0], 0
        jmp    loc_3_1151
loc_3_1224:
        call   sub_3_0BEF
        mov    al, 1
        ret    
loc_3_122C:
        sub    esp, 8
        mov    eax, esp
        mov    dword ptr [eax + 4], edx
        mov    word ptr [eax], 4
        push   eax
        call   sub_1_0F54
        add    esp, 0xc
        ret    
sub_3_1244:
        push   esi
        sub    esp, 8
        mov    esi, esp
        mov    dword ptr [esi + 4], eax
        mov    word ptr [esi], 4
        push   esi
        call   sub_1_0F54
        add    esp, 0xc
        pop    esi
        ret    
        db  000h, 000h, 001h, 000h, 020h, 015h, 014h, 01Bh, 01Bh, 01Fh, 010h, 057h, 019h, 015h, 01Fh, 013h
        db  010h, 057h, 01Ah, 01Fh, 01Ah, 01Fh, 01Ah, 01Fh, 01Ah, 01Fh, 01Ah, 01Fh, 01Ah, 01Fh, 01Ah, 01Fh
        db  023h, 043h
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 4  vsize=0x4  RW-
VxD__LDATA ENDS

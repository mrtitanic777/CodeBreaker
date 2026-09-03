; VQIC117 -- reconstructed 32-bit disassembly (VxD / LE format)
; 2 objects, 16264 instructions recovered by recursive descent

Declare_Virtual_Device VQIC117, 2, 2, VQIC117_Control, 00000h, Undefined_Init_Order
;   DDK version 030Ah   init_order 080000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0xc064  R-X
VQIC117_Control:
        cmp    eax, 0x1b
        je     loc_1_0290
        cmp    eax, 0x1c
        je     loc_1_02BB
        cmp    eax, 0x23
        jne    loc_1_0024
        push   esi
loc_1_0018:
        push   edx
        push   ecx
        push   ebx
        call   sub_1_0026
loc_1_0020:
        cmp    eax, 1
        ret    
loc_1_0024:
        clc    
        ret    
sub_1_0026:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        sub    esp, 4
loc_1_0030:
        cmp    eax, 0x100
        push   esi
        push   edi
        ja     loc_1_0045
        je     loc_1_00CB
        test   eax, eax
        je     loc_1_00BF
        jmp    loc_1_00B5
loc_1_0045:
        cmp    eax, 0x20d
        ja     loc_1_008B
        je     loc_1_01C1
        cmp    eax, 0x101
        je     loc_1_00F5
        cmp    eax, 0x102
        je     loc_1_0114
        cmp    eax, 0x103
        je     loc_1_0155
        cmp    eax, 0x104
        je     loc_1_0187
        cmp    eax, 0x106
        je     loc_1_01A4
        jmp    loc_1_00B5
loc_1_008B:
        cmp    eax, 0x20e
        je     loc_1_0206
        cmp    eax, 0x20f
        je     loc_1_0254
        cmp    eax, 0x306
        je     loc_1_026D
        cmp    eax, -1
        je     loc_1_0280
loc_1_00B5:
        mov    eax, 0x75
        jmp    loc_1_0287
loc_1_00BF:
        mov    ecx, 0x400
        xor    eax, eax
        jmp    loc_1_0287
loc_1_00CB:
        mov    edi, dword ptr [ebp + 0x14]
        cmp    dword ptr [edi + 0x1c], 4
        jb     loc_1_00EB
        mov    eax, dword ptr [edi + 0x18]
        mov    dword ptr [eax], 0x10302
        mov    ecx, dword ptr [edi + 0x20]
        mov    dword ptr [ecx], 4
        jmp    loc_1_0285
loc_1_00EB:
        mov    eax, 0x1156
        jmp    loc_1_0287
loc_1_00F5:
        mov    edi, dword ptr [ebp + 0x14]
        push   dword ptr [edi + 4]
        push   0
        push   0x40ce78                      ; -> obj1:0xBE78
        push   0x40cf80                      ; -> obj1:0xBF80
        call   sub_1_1C52
        add    esp, 0x10
        jmp    loc_1_0287
loc_1_0114:
        cmp    dword ptr [0x40d060], 0       ; -> loc_1_C060
        je     loc_1_012A
        push   0x40cd48                      ; -> loc_1_BD48
        call   sub_1_110B
        add    esp, 4
loc_1_012A:
        mov    edi, dword ptr [ebp + 0x14]
        mov    dword ptr [0x40d060], 0       ; -> loc_1_C060
        mov    eax, dword ptr [edi + 0x10]
        mov    ax, word ptr [eax]
        push   eax
        push   0x40ce78                      ; -> obj1:0xBE78
        push   0x40cf80                      ; -> obj1:0xBF80
        call   sub_1_1BF3
        add    esp, 0xc
        jmp    loc_1_0287
loc_1_0155:
        mov    edi, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [edi + 0x10]
        push   dword ptr [edi + 0x24]
        push   dword ptr [ecx]
        push   dword ptr [edi + 0x18]
        push   0x40ce78                      ; -> obj1:0xBE78
        push   0x40cf80                      ; -> obj1:0xBF80
        call   sub_1_19B5
        add    esp, 0x14
        test   eax, eax
        jne    loc_1_0287
        mov    eax, 0x3e5
        jmp    loc_1_0287
loc_1_0187:
        mov    edi, dword ptr [ebp + 0x14]
        push   dword ptr [edi + 0x10]
        push   0x40ce78                      ; -> obj1:0xBE78
        push   0x40cf80                      ; -> obj1:0xBF80
        call   sub_1_1957
        add    esp, 0xc
        jmp    loc_1_0287
loc_1_01A4:
        mov    edi, dword ptr [ebp + 0x14]
        push   dword ptr [edi + 0x18]
        push   0x40ce78                      ; -> obj1:0xBE78
        push   0x40cf80                      ; -> obj1:0xBF80
        call   sub_1_181E
        add    esp, 0xc
        jmp    loc_1_0287
loc_1_01C1:
        lea    eax, [ebp - 4]
        mov    edi, dword ptr [ebp + 0x14]
        push   eax
        mov    ecx, dword ptr [edi + 0x10]
        mov    ax, word ptr [ecx]
        push   eax
        push   0x40ce78                      ; -> obj1:0xBE78
        call   sub_1_17D6
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_01F2
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        add    eax, 0x548
        mov    dword ptr [0x40d014], eax     ; -> obj1:0xC014
        jmp    loc_1_01FC
loc_1_01F2:
        mov    dword ptr [0x40d014], 0x40cfe0 ; -> obj1:0xC014
loc_1_01FC:
        mov    eax, dword ptr [0x40d014]     ; -> obj1:0xC014
        jmp    loc_1_0287
loc_1_0206:
        mov    edi, dword ptr [ebp + 0x14]
        mov    eax, dword ptr [edi + 0x18]
        mov    byte ptr [eax + 0x22], 0x22
        push   dword ptr [edi + 0x18]
        push   0
        mov    dword ptr [ebp - 4], 0
        call   sub_1_1179
        add    esp, 8
        test   eax, eax
        jne    loc_1_0287
        push   0x40cd48                      ; -> loc_1_BD48
        call   sub_1_1644
        add    esp, 4
        test   eax, eax
        jne    loc_1_0287
        mov    esi, 0x40cd60                 ; -> obj1:0xBD60
        mov    edi, dword ptr [edi + 0x18]
        mov    ecx, 0x45
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    dword ptr [0x40d060], 1       ; -> loc_1_C060
        jmp    loc_1_0287
loc_1_0254:
        mov    dword ptr [0x40d060], 0       ; -> loc_1_C060
        push   0x40cd48                      ; -> loc_1_BD48
        call   sub_1_110B
        add    esp, 4
        jmp    loc_1_0287
loc_1_026D:
        mov    edi, dword ptr [ebp + 0x14]
        push   dword ptr [edi + 4]
        call   sub_1_02C4
        movzx  eax, ax
        add    esp, 4
        jmp    loc_1_0287
loc_1_0280:
        call   sub_1_1090
loc_1_0285:
        xor    eax, eax
loc_1_0287:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x10
        db  0CCh
loc_1_0290:
        mov    byte ptr [0x401fff], 0        ; -> obj1:0x0FFF
        VxDCall  VFBACKUP.VFBACKUP_Get_Version   ; cd2000003600
        jb     loc_1_02A6
        mov    byte ptr [0x401fff], 1        ; -> obj1:0x0FFF
loc_1_02A6:
        VMMCall  VMM.Get_Execution_Focus         ; cd2030000100
        push   0
        push   0
        push   ebx
        call   sub_1_16F4
        add    esp, 0xc
        clc    
        ret    
loc_1_02BB:
        call   sub_1_1090
        clc    
        ret    
        db  0CCh, 0CCh
sub_1_02C4:
        enter  0, 0
        in     al, 0xa1
        mov    ah, al
        in     al, 0x21
        bts    ax, 6
        mov    bx, ax
        mov    ecx, 0xf
        mov    edx, 1
loc_1_02E0:
        bt     bx, dx
        jae    loc_1_02F9
        mov    eax, edx
        pushal 
        mov    ebx, dword ptr [ebp + 8]
        VxDCall  VPICD.VPICD_Get_IRQ_Complete_Status ; cd200b000300
        popal  
        jae    loc_1_02F9
        btr    bx, dx
loc_1_02F9:
        inc    dx
        loop   loc_1_02E0
        mov    ax, bx
        leave  
        ret    
        db  0CCh, 0CCh
sub_1_0304:
        enter  0, 0
        push   0
        push   dword ptr [ebp + 8]
        VMMCall  VMM._PageFree                   ; cd2055000100
        add    esp, 8
        or     eax, eax
        je     loc_1_031E
        xor    eax, eax
        jmp    loc_1_0321
loc_1_031E:
        mov    eax, dword ptr [ebp + 8]
loc_1_0321:
        leave  
        ret    
        db  0CCh
sub_1_0324:
        enter  0, 0
        push   ebx
        push   ecx
        mov    ebx, 0x406
        movzx  ecx, word ptr [ebp + 8]
        xor    eax, eax
        cmp    ax, cx
        je     loc_1_0342
loc_1_033A:
        add    ebx, 2
        loop   loc_1_033A
        movzx  eax, word ptr [ebx]
loc_1_0342:
        pop    ecx
        pop    ebx
        leave  
        ret    
        db  0CCh, 0CCh
sub_1_0348:
        enter  0, 0
        push   edx
        mov    edx, dword ptr [ebp + 8]
        or     dx, dx
        je     loc_1_037E
        add    dx, 0x402
        in     al, dx
        and    al, 0x1f
        out    dx, al
        or     al, 0x40
        out    dx, al
        in     al, dx
        mov    ah, al
        and    al, 0xc0
        cmp    al, 0x40
        jne    loc_1_037E
        mov    al, ah
        bt     ax, 0
        jae    loc_1_037E
        mov    al, 0x14
        out    dx, al
        mov    eax, 1
        pop    edx
        leave  
        ret    
loc_1_037E:
        mov    eax, 0
        pop    edx
        leave  
        ret    
        db  0CCh, 0CCh
sub_1_0388:
        pushfd 
        push   ebx
        push   ecx
        push   edx
        VMMCall  VMM.Get_Machine_Info            ; cd20bb000100
        bt     ebx, 0x13
        jb     loc_1_03A5
        bt     ebx, 0x12
        jb     loc_1_03AC
        mov    eax, 2
        jmp    loc_1_03B1
loc_1_03A5:
        mov    eax, 3
        jmp    loc_1_03B1
loc_1_03AC:
        mov    eax, 1
loc_1_03B1:
        bt     ebx, 0x10
        jae    loc_1_03BA
        or     eax, 0x10
loc_1_03BA:
        pop    edx
        pop    ecx
        pop    ebx
        popfd  
        ret    
        db  0CCh
sub_1_03C0:
        enter  0, 0
        push   edi
        push   esi
        push   ebx
        push   ecx
        push   edx
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0x1c]
        push   dword ptr [ebp + 0x14]
        push   esi
        push   0xfff
        push   0
        push   dword ptr [ebp + 0x10]
        push   ebx
        push   0
        push   dword ptr [ebp + 0xc]
        VMMCall  VMM._PageAllocate               ; cd2053000100
        add    esp, 0x20
        test   eax, eax
        je     loc_1_0412
        mov    ebx, dword ptr [ebp + 0x18]
        mov    dword ptr [ebx], edx
        push   eax
        shr    edx, 0xc
        push   0
        push   esi
        push   1
        push   edx
        VMMCall  VMM._CopyPageTable              ; cd2061000100
        add    esp, 0x10
        mov    eax, dword ptr [esi]
        and    eax, 0xfffff000
        mov    dword ptr [esi], eax
        pop    eax
loc_1_0412:
        pop    edx
        pop    ecx
        pop    ebx
        pop    esi
        pop    edi
        leave  
        ret    
        db  0CCh, 0CCh, 0CCh
sub_1_041C:
        enter  0, 0
        push   edi
        push   esi
        push   ebx
        push   ecx
        push   edx
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, eax
        shr    eax, 0xc
        shr    ecx, 0xc
        sub    ecx, eax
        inc    ecx
        push   0x40000000
        push   ecx
        push   eax
        VMMCall  VMM._LinPageUnLock              ; cd2064000100
        add    esp, 0xc
        pop    edx
        pop    ecx
        pop    ebx
        pop    esi
        pop    edi
        leave  
        ret    
        db  0CCh, 0CCh, 0CCh
sub_1_0450:
        enter  0, 0
        push   edi
        push   esi
        push   ebx
        push   ecx
        push   edx
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
        add    ecx, eax
        shr    eax, 0xc
        shr    ecx, 0xc
        sub    ecx, eax
        inc    ecx
        push   0x40000000
        push   ecx
        push   eax
        VMMCall  VMM._LinPageLock                ; cd2063000100
        add    esp, 0xc
        or     eax, eax
        je     loc_1_0489
        mov    ecx, dword ptr [ebp + 8]
        and    ecx, 0xfff
        add    eax, ecx
loc_1_0489:
        pop    edx
        pop    ecx
        pop    ebx
        pop    esi
        pop    edi
        leave  
        ret    
sub_1_0490:
        enter  0, 0
        pushal 
        mov    edi, dword ptr [0x40d014]     ; -> obj1:0xC014
        mov    ah, byte ptr [edi]
        mov    bl, byte ptr [edi + 2]
        mov    bh, byte ptr [edi + 7]
        xor    edx, edx
        mov    dx, word ptr [edi + 0x18]
        mov    ecx, edx
        add    edx, 2
        ror    ecx, 0x10
        or     edx, ecx
        xor    ecx, ecx
        test   bl, 8
        jne    loc_1_04C1
        inc    ecx
        or     bl, 8
        mov    al, bl
        out    dx, al
loc_1_04C1:
        mov    al, byte ptr [ebp + 8]
        and    al, 0x7f
        ror    edx, 0x10
        xor    bh, al
        test   byte ptr [edi + 3], 0x10
        je     loc_1_04D6
        cmp    ecx, 0
        jne    loc_1_04FA
loc_1_04D6:
        cmp    al, ah
        je     loc_1_04DF
        out    dx, al
        mov    ah, al
        jmp    loc_1_04EB
loc_1_04DF:
        xor    bl, 2
        mov    al, bl
        ror    edx, 0x10
        out    dx, al
        ror    edx, 0x10
loc_1_04EB:
        movzx  ecx, byte ptr [edi + 0xf]
        cmp    ecx, 0
        je     loc_1_04F8
loc_1_04F4:
        in     al, 0x21
        loop   loc_1_04F4
loc_1_04F8:
        jmp    loc_1_0522
loc_1_04FA:
        out    dx, al
        mov    ah, al
        ror    edx, 0x10
        test   byte ptr [edi + 3], 3
        je     loc_1_050C
        and    bl, 0xdf
        mov    al, bl
        out    dx, al
loc_1_050C:
        xor    bl, 2
        mov    al, bl
        out    dx, al
        ror    edx, 0x10
        movzx  ecx, byte ptr [edi + 0x11]
        cmp    ecx, 0
        je     loc_1_0522
loc_1_051E:
        in     al, 0x21
        loop   loc_1_051E
loc_1_0522:
        mov    al, byte ptr [ebp + 0xc]
        xor    bh, al
        cmp    al, ah
        je     loc_1_0530
        out    dx, al
        mov    ah, al
        jmp    loc_1_0539
loc_1_0530:
        xor    bl, 2
        mov    al, bl
        ror    edx, 0x10
        out    dx, al
loc_1_0539:
        mov    byte ptr [edi], ah
        mov    byte ptr [edi + 2], bl
        mov    byte ptr [edi + 7], bh
        popal  
        leave  
        ret    
sub_1_0544:
        enter  0, 0
        pushal 
        mov    edi, dword ptr [0x40d014]     ; -> obj1:0xC014
        mov    dx, word ptr [edi + 0x18]
        mov    ecx, edx
        add    edx, 2
        mov    bl, byte ptr [edi + 2]
        mov    bh, byte ptr [edi + 7]
        test   bl, 8
        jne    loc_1_0569
        or     bl, 8
        mov    al, bl
        out    dx, al
loc_1_0569:
        mov    al, byte ptr [ebp + 8]
        and    al, 0x7f
        xchg   ecx, edx
        out    dx, al
        xor    bh, al
        xor    bl, 2
        mov    al, bl
        xchg   ecx, edx
        out    dx, al
        push   ecx
        movzx  ecx, byte ptr [edi + 0x10]
        cmp    ecx, 0
        je     loc_1_0589
loc_1_0585:
        in     al, 0x21
        loop   loc_1_0585
loc_1_0589:
        pop    ecx
        mov    al, byte ptr [ebp + 0xc]
        xchg   ecx, edx
        out    dx, al
        xor    bh, al
        mov    byte ptr [edi], al
        xor    bl, 2
        mov    al, bl
        xchg   ecx, edx
        out    dx, al
        mov    byte ptr [edi + 7], bh
        mov    byte ptr [edi + 2], bl
        popal  
        leave  
        ret    
        db  0CCh, 0CCh, 0CCh
sub_1_05A8:
        push   ecx
        push   ebx
        mov    ebx, dword ptr [0x401ff1]     ; -> obj1:0x0FF1
        cmp    ebx, 0
        je     loc_1_05D1
        mov    eax, dword ptr [0x401fed]     ; -> obj1:0x0FED
        cmp    eax, 0
        je     loc_1_05D1
        VxDCall  VPICD.VPICD_Get_Complete_Status ; cd2005000300
        xor    eax, eax
        and    ecx, 4
        je     loc_1_05D1
        mov    eax, 1
loc_1_05D1:
        pop    ebx
        pop    ecx
        ret    
sub_1_05D4:
        enter  0, 0
        mov    eax, dword ptr [ebp + 0xc]
        leave  
        ret    
        db  0CCh, 0CCh, 0CCh
sub_1_05E0:
        enter  0, 0
        mov    eax, dword ptr [ebp + 8]
        or     eax, eax
        je     loc_1_05F7
        push   0
        push   eax
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
loc_1_05F7:
        leave  
        ret    
        db  0CCh, 0CCh, 0CCh
sub_1_05FC:
        enter  0, 0
        mov    eax, dword ptr [ebp + 8]
        or     eax, eax
        je     loc_1_0613
        push   1
        push   eax
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
loc_1_0613:
        leave  
        ret    
        db  0CCh, 0CCh, 0CCh
sub_1_0618:
        enter  0, 0
        push   edi
        push   esi
        push   ebx
        mov    ebx, dword ptr [ebp + 8]
        cmp    byte ptr [ebp + 0xc], 6
        je     loc_1_066D
        mov    edi, 0x402070                 ; -> obj1:0x1070
        cmp    dword ptr [0x401ff1], 0       ; -> obj1:0x0FF1
        jne    loc_1_068E
        cmp    dword ptr [0x401fed], 0       ; -> obj1:0x0FED
        jne    loc_1_0662
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [0x401fe8], eax     ; -> loc_1_0FE8
        mov    eax, dword ptr [ebp + 0xc]
        mov    word ptr [edi], ax
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [0x401fe4], eax     ; -> sub_1_0FE4
        VxDCall  VPICD.VPICD_Virtualize_IRQ      ; cd2001000300
        jb     loc_1_068E
        mov    dword ptr [0x401fed], eax     ; -> obj1:0x0FED
loc_1_0662:
        mov    dword ptr [0x401ff1], ebx     ; -> obj1:0x0FF1
        mov    ax, word ptr [edi]
        jmp    loc_1_0692
loc_1_066D:
        cmp    byte ptr [0x401fff], 0        ; -> obj1:0x0FFF
        je     loc_1_0692
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [0x401fe8], eax     ; -> loc_1_0FE8
        mov    eax, dword ptr [ebp + 0x10]
        mov    dword ptr [0x401fe4], eax     ; -> sub_1_0FE4
        mov    dword ptr [0x401ff1], ebx     ; -> obj1:0x0FF1
        jmp    loc_1_0692
loc_1_068E:
        mov    ax, 0
loc_1_0692:
        pop    ebx
        pop    esi
        pop    edi
        leave  
        ret    
        db  0CCh
sub_1_0698:
        enter  0, 0
        push   edi
        push   esi
        push   ebx
        cmp    byte ptr [0x401fec], 0        ; -> loc_1_0FEC
        je     loc_1_06AF
        mov    eax, 1
        jmp    loc_1_0701
loc_1_06AF:
        mov    ebx, dword ptr [ebp + 8]
        cmp    byte ptr [0x401fff], 0        ; -> obj1:0x0FFF
        je     loc_1_06EF
        xor    eax, eax
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        mov    ebx, edi
        mov    ecx, 0x401730                 ; -> loc_1_0730
        VxDCall  VFBACKUP.VFBACKUP_Lock_NEC      ; cd2001003600
        jae    loc_1_06D9
        mov    eax, 2
        jmp    loc_1_0701
loc_1_06D9:
        mov    dword ptr [0x401fed], eax     ; -> obj1:0x0FED
        push   ebx
        VxDCall  VFBACKUP.VFBACKUP_Lock_All_Ports ; cd2005003600
        pop    ebx
        jae    loc_1_06EF
        mov    eax, 4
        jmp    loc_1_0701
loc_1_06EF:
        mov    dword ptr [0x401fe0], ebx     ; -> obj1:0x0FE0
        mov    dx, 0x3f7
        in     al, dx
        mov    byte ptr [0x401ff6], al       ; -> loc_1_0FF6
        xor    eax, eax
loc_1_0701:
        pop    ebx
        pop    esi
        pop    edi
        leave  
        ret    
        db  0CCh, 0CCh
sub_1_0708:
        pushal 
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        popal  
        ret    
        db  0CCh, 0CCh, 0CCh
sub_1_0720:
        mov    ecx, dword ptr [esp + 4]
        VxDCall  VTD.VTD_Delay                   ; cd200a000500
        mov    eax, ecx
        ret    
        db  0CCh, 0CCh, 0CCh
loc_1_0730:
        cmp    dword ptr [0x401ff1], 0       ; -> obj1:0x0FF1
        je     loc_1_0749
        push   edx
        mov    edx, dword ptr [0x401fe8]     ; -> loc_1_0FE8
        call   sub_1_0FE4
        pop    edx
        clc    
        ret    
loc_1_0749:
        VxDCall  VPICD.VPICD_Set_Int_Request     ; cd2002000300
        clc    
        ret    
        db  0CCh, 0CCh, 0CCh
loc_1_0754:
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
        VxDCall  VPICD.VPICD_Clear_Int_Request   ; cd2003800300
loc_1_0760:
        cmp    dword ptr [0x401ff1], 0       ; -> obj1:0x0FF1
        je     loc_1_078E
        cmp    ebx, dword ptr [0x401ff1]     ; -> obj1:0x0FF1
        je     loc_1_0773
        jmp    loc_1_078E
loc_1_0773:
        cmp    eax, dword ptr [0x401fed]     ; -> obj1:0x0FED
        je     loc_1_077B
loc_1_077B:
        cmp    ecx, 0
        je     loc_1_0788
        VxDCall  VPICD.VPICD_Physically_Mask     ; cd2008000300
        jmp    loc_1_078E
loc_1_0788:
        VxDCall  VPICD.VPICD_Physically_Unmask   ; cd2009000300
loc_1_078E:
        ret    
        db  0CCh
sub_1_0790:
        mov    eax, dword ptr [0x401fed]     ; -> obj1:0x0FED
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
        ret    
sub_1_079C:
        enter  0, 0
        pushfd 
        push   edi
        push   esi
        push   ebx
        push   ecx
        push   edx
        mov    eax, dword ptr [ebp + 0xc]
        mov    ebx, dword ptr [ebp + 8]
        mov    ecx, 8
        mov    edx, dword ptr [ebp + 0x14]
        mov    esi, dword ptr [ebp + 0x10]
        mov    edi, 0
        VMMCall  VMM.Call_Priority_VM_Event      ; cd2014000100
        mov    eax, esi
        pop    edx
        pop    ecx
        pop    ebx
        pop    esi
        pop    edi
        popfd  
        leave  
        ret    
loc_1_07CC:
        xor    esi, esi
        cmp    dword ptr [0x401fe0], 0       ; -> obj1:0x0FE0
        je     loc_1_07F4
        mov    eax, dword ptr [0x401ff7]     ; -> obj1:0x0FF7
        mov    byte ptr [eax], 0xff
        mov    eax, 0x2710
        mov    edx, 0
        mov    esi, 0x4017cc                 ; -> loc_1_07CC
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
loc_1_07F4:
        mov    dword ptr [0x401ffb], esi     ; -> obj1:0x0FFB
        ret    
        db  0CCh
sub_1_07FC:
        enter  0, 0
        push   edi
        push   esi
        push   ebx
        mov    eax, dword ptr [0x401fed]     ; -> obj1:0x0FED
        cmp    eax, 0
        je     loc_1_0823
        cmp    byte ptr [ebp + 8], 6
        je     loc_1_0823
        VxDCall  VPICD.VPICD_Force_Default_Behavior ; cd2011000300
        mov    dword ptr [0x401fed], 0       ; -> obj1:0x0FED
loc_1_0823:
        mov    dword ptr [0x401ff1], 0       ; -> obj1:0x0FF1
        pop    ebx
        pop    esi
        pop    edi
        leave  
        ret    
        db  0CCh, 0CCh
sub_1_0834:
        enter  0, 0
        push   edi
        push   esi
        push   ebx
        mov    ebx, dword ptr [0x401fe0]     ; -> obj1:0x0FE0
        mov    dword ptr [0x401fe0], 0       ; -> obj1:0x0FE0
        mov    byte ptr [0x401ff5], 1        ; -> obj1:0x0FF5
        mov    al, byte ptr [0x401ff6]       ; -> loc_1_0FF6
        mov    dx, 0x3f7
        out    dx, al
        cmp    byte ptr [0x401fff], 0        ; -> obj1:0x0FFF
        je     loc_1_086B
        VxDCall  VFBACKUP.VFBACKUP_UnLock_NEC    ; cd2002003600
loc_1_086B:
        mov    esi, dword ptr [0x401ffb]     ; -> obj1:0x0FFB
        cmp    esi, 0
        je     loc_1_0886
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
        mov    dword ptr [0x401ffb], 0       ; -> obj1:0x0FFB
loc_1_0886:
        pop    ebx
        pop    esi
        pop    edi
        leave  
        ret    
        db  0CCh
sub_1_088C:
        enter  0, 0
        pushal 
        mov    eax, dword ptr [ebp + 8]
        VMMCall  VMM.Destroy_Semaphore           ; cd2026000100
        popal  
        leave  
        ret    
        db  0CCh, 0CCh, 0CCh
sub_1_08A0:
        enter  0, 0
        pushfd 
        push   ecx
        mov    eax, dword ptr [ebp + 8]
        mov    ecx, 1
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
        pop    ecx
        popfd  
        leave  
        ret    
sub_1_08B8:
        enter  0, 0
        pushfd 
        push   ecx
        mov    ecx, dword ptr [ebp + 8]
        VMMCall  VMM.Create_Semaphore            ; cd2025000100
        jae    loc_1_08CB
        xor    eax, eax
loc_1_08CB:
        pop    ecx
        popfd  
        leave  
        ret    
        db  0CCh
sub_1_08D0:
        enter  0, 0
        pushfd 
        pushal 
        cld    
        mov    esi, dword ptr [0x40d014]     ; -> obj1:0xC014
        mov    bh, byte ptr [esi + 6]
        mov    bl, byte ptr [esi]
        mov    al, byte ptr [esi + 2]
        xor    edx, edx
        mov    dx, word ptr [esi + 0x18]
        mov    ecx, edx
        add    ecx, 1
        ror    ecx, 0x10
        or     edx, ecx
        test   al, 8
        je     loc_1_0916
        and    al, 0xf7
        add    edx, 2
        out    dx, al
        sub    edx, 2
        mov    byte ptr [esi + 2], al
        movzx  ecx, byte ptr [esi + 0xb]
        cmp    ecx, 0
        je     loc_1_0912
loc_1_090E:
        in     al, 0x21
        loop   loc_1_090E
loc_1_0912:
        and    byte ptr [esi + 3], 0xf7
loc_1_0916:
        movzx  ecx, word ptr [ebp + 0xc]
        mov    edi, dword ptr [ebp + 8]
        movzx  esi, byte ptr [esi + 0xb]
loc_1_0921:
        xor    bl, 0x80
        mov    al, bl
        out    dx, al
        cmp    esi, 0
        je     loc_1_0933
        push   esi
loc_1_092D:
        in     al, 0x21
        dec    esi
        jne    loc_1_092D
        pop    esi
loc_1_0933:
        ror    edx, 0x10
        in     al, dx
        mov    ah, al
        and    ah, 0xf0
        ror    edx, 0x10
        xor    bl, 0x80
        mov    al, bl
        out    dx, al
        cmp    esi, 0
        je     loc_1_0951
        push   esi
loc_1_094B:
        in     al, 0x21
        dec    esi
        jne    loc_1_094B
        pop    esi
loc_1_0951:
        ror    edx, 0x10
        in     al, dx
        shr    al, 1
        shr    al, 1
        shr    al, 1
        shr    al, 1
        or     al, ah
        xor    al, 0x88
        stosb  byte ptr es:[edi], al
        xor    bh, al
        ror    edx, 0x10
        loop   loc_1_0921
        mov    esi, dword ptr [0x40d014]     ; -> obj1:0xC014
        mov    byte ptr [esi], bl
        mov    byte ptr [esi + 6], bh
        popal  
        popfd  
        leave  
        ret    
sub_1_0978:
        enter  0, 0
        pushfd 
        pushal 
        cld    
        mov    esi, dword ptr [0x40d014]     ; -> obj1:0xC014
        mov    bh, byte ptr [esi + 6]
        mov    bl, byte ptr [esi + 2]
        xor    edx, edx
        mov    dx, word ptr [esi + 0x18]
        mov    ecx, edx
        add    ecx, 2
        ror    edx, 0x10
        or     edx, ecx
        mov    ch, byte ptr [esi + 0x21]
        cmp    ch, 1
        jne    loc_1_09B8
        and    byte ptr [esi + 0x22], 0x7f
        mov    al, byte ptr [esi + 0x22]
        out    0x65, al
        and    bl, 0xf7
        mov    al, bl
        out    dx, al
        and    byte ptr [esi + 3], 0xf7
        jmp    loc_1_09CF
loc_1_09B8:
        cmp    ch, 4
        jne    loc_1_09C1
        mov    cl, 0x80
        jmp    loc_1_09C3
loc_1_09C1:
        mov    cl, 0x20
loc_1_09C3:
        or     bl, cl
        and    bl, 0xf7
        mov    al, bl
        out    dx, al
        and    byte ptr [esi + 3], 0xf7
loc_1_09CF:
        push   ecx
        movzx  ecx, word ptr [ebp + 0xc]
        mov    edi, dword ptr [ebp + 8]
        movzx  esi, byte ptr [esi + 0xb]
loc_1_09DB:
        xor    bl, 2
        mov    al, bl
        out    dx, al
        cmp    esi, 0
        je     loc_1_09ED
        push   esi
loc_1_09E7:
        in     al, 0x21
        dec    esi
        jne    loc_1_09E7
        pop    esi
loc_1_09ED:
        ror    edx, 0x10
        in     al, dx
        xor    bh, al
        stosb  byte ptr es:[edi], al
        ror    edx, 0x10
        loop   loc_1_09DB
        pop    ecx
        mov    esi, dword ptr [0x40d014]     ; -> obj1:0xC014
        cmp    ch, 1
        jne    loc_1_0A11
        or     cl, 0x80
        mov    al, cl
        out    0x65, al
        mov    byte ptr [esi + 0x22], al
        jmp    loc_1_0A18
loc_1_0A11:
        not    cl
        and    bl, cl
        mov    al, bl
        out    dx, al
loc_1_0A18:
        mov    byte ptr [esi + 2], bl
        mov    byte ptr [esi + 6], bh
        popal  
        popfd  
        leave  
        ret    
        db  0CCh, 0CCh
sub_1_0A24:
        enter  0, 0
        pushfd 
        pushal 
        cld    
        mov    edi, dword ptr [0x40d014]     ; -> obj1:0xC014
        mov    bl, byte ptr [edi + 2]
        mov    bh, byte ptr [edi + 6]
        xor    edx, edx
        mov    dx, word ptr [edi + 0x18]
        mov    ecx, edx
        add    ecx, 2
        ror    edx, 0x10
        or     edx, ecx
        movzx  ecx, word ptr [ebp + 0xc]
        test   bl, 0x28
        je     loc_1_0A5A
        and    bl, 0xd7
        mov    al, bl
        out    dx, al
        and    byte ptr [edi + 3], 0xf7
loc_1_0A5A:
        ror    edx, 0x10
        movzx  edi, byte ptr [edi + 0xa]
        mov    esi, dword ptr [ebp + 8]
loc_1_0A64:
        cmp    edi, 0
        je     loc_1_0A70
        push   edi
loc_1_0A6A:
        in     al, 0x21
        dec    edi
        jne    loc_1_0A6A
        pop    edi
loc_1_0A70:
        lodsb  al, byte ptr [esi]
        xor    bh, al
        out    dx, al
        ror    edx, 0x10
        xor    bl, 2
        mov    al, bl
        out    dx, al
        ror    edx, 0x10
        loop   loc_1_0A64
        mov    edi, dword ptr [0x40d014]     ; -> obj1:0xC014
        dec    esi
        mov    al, byte ptr [esi]
        mov    byte ptr [edi], al
        mov    byte ptr [edi + 2], bl
        mov    byte ptr [edi + 6], bh
        popal  
        popfd  
        leave  
        ret    
        db  0CCh
sub_1_0A98:
        enter  0, 0
        pushfd 
        pushal 
        cld    
        mov    edi, dword ptr [0x40d014]     ; -> obj1:0xC014
        mov    ah, byte ptr [edi]
        mov    bl, byte ptr [edi + 2]
        mov    bh, byte ptr [edi + 6]
        xor    edx, edx
        mov    dx, word ptr [edi + 0x18]
        mov    ecx, edx
        add    ecx, 2
        ror    edx, 0x10
        or     edx, ecx
        movzx  ecx, word ptr [ebp + 0xc]
        test   bl, 0x28
        je     loc_1_0AD0
        and    bl, 0xd7
        mov    al, bl
        out    dx, al
        and    byte ptr [edi + 3], 0xf7
loc_1_0AD0:
        ror    edx, 0x10
        movzx  edi, byte ptr [edi + 9]
        mov    esi, dword ptr [ebp + 8]
loc_1_0ADA:
        cmp    edi, 0
        je     loc_1_0AE6
        push   edi
loc_1_0AE0:
        in     al, 0x21
        dec    edi
        jne    loc_1_0AE0
        pop    edi
loc_1_0AE6:
        lodsb  al, byte ptr [esi]
        xor    bh, al
        cmp    al, ah
        je     loc_1_0AF4
        mov    ah, al
        out    dx, al
        loop   loc_1_0ADA
        jmp    loc_1_0B02
loc_1_0AF4:
        ror    edx, 0x10
        xor    bl, 2
        mov    al, bl
        out    dx, al
        ror    edx, 0x10
        loop   loc_1_0ADA
loc_1_0B02:
        mov    edi, dword ptr [0x40d014]     ; -> obj1:0xC014
        mov    byte ptr [edi], ah
        mov    byte ptr [edi + 2], bl
        mov    byte ptr [edi + 6], bh
        popal  
        popfd  
        leave  
        ret    
sub_1_0B14:
        enter  0, 0
        pushfd 
        push   esi
        mov    esi, dword ptr [ebp + 8]
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
        pop    esi
        popfd  
        leave  
        ret    
        db  0CCh
sub_1_0B28:
        enter  0, 0
        push   ebx
        push   ecx
        push   edx
        push   edi
        push   esi
        mov    edi, dword ptr [0x40d014]     ; -> obj1:0xC014
        mov    ah, byte ptr [edi]
        mov    bl, byte ptr [edi + 2]
        mov    bh, byte ptr [edi + 7]
        xor    edx, edx
        mov    dx, word ptr [edi + 0x18]
        mov    ecx, edx
        add    edx, 2
        ror    ecx, 0x10
        or     edx, ecx
        xor    ecx, ecx
        test   bl, 8
        jne    loc_1_0B5D
        inc    ecx
        or     bl, 8
        mov    al, bl
        out    dx, al
loc_1_0B5D:
        mov    al, byte ptr [ebp + 8]
        or     al, 0x80
        ror    edx, 0x10
        xor    bh, al
        test   byte ptr [edi + 3], 0x10
        je     loc_1_0B72
        cmp    ecx, 0
        jne    loc_1_0BA1
loc_1_0B72:
        cmp    al, ah
        je     loc_1_0B7B
        out    dx, al
        mov    ah, al
        jmp    loc_1_0B87
loc_1_0B7B:
        xor    bl, 2
        mov    al, bl
        ror    edx, 0x10
        out    dx, al
        ror    edx, 0x10
loc_1_0B87:
        test   ah, 0x70
        je     loc_1_0B92
        movzx  ecx, byte ptr [edi + 0xc]
        jmp    loc_1_0B96
loc_1_0B92:
        movzx  ecx, byte ptr [edi + 0xd]
loc_1_0B96:
        cmp    ecx, 0
        je     loc_1_0B9F
loc_1_0B9B:
        in     al, 0x21
        loop   loc_1_0B9B
loc_1_0B9F:
        jmp    loc_1_0BC9
loc_1_0BA1:
        out    dx, al
        mov    ah, al
        ror    edx, 0x10
        test   byte ptr [edi + 3], 3
        je     loc_1_0BB3
        and    bl, 0xdf
        mov    al, bl
        out    dx, al
loc_1_0BB3:
        xor    bl, 2
        mov    al, bl
        out    dx, al
        ror    edx, 0x10
        movzx  ecx, byte ptr [edi + 0xe]
        cmp    ecx, 0
        je     loc_1_0BC9
loc_1_0BC5:
        in     al, 0x21
        loop   loc_1_0BC5
loc_1_0BC9:
        mov    byte ptr [edi + 2], bl
        inc    edx
        in     al, dx
        and    al, 0xf0
        xchg   al, ah
        and    al, 0x7f
        dec    dx
        out    dx, al
        mov    byte ptr [edi], al
        inc    edx
        in     al, dx
        shr    al, 1
        shr    al, 1
        shr    al, 1
        shr    al, 1
        or     al, ah
        xor    al, 0x88
        xor    bh, al
        mov    byte ptr [edi + 7], bh
        xor    ah, ah
        pop    esi
        pop    edi
        pop    edx
        pop    ecx
        pop    ebx
        leave  
        ret    
        db  0CCh, 0CCh, 0CCh
sub_1_0BF8:
        enter  0, 0
        push   ebx
        push   ecx
        push   edx
        push   edi
        mov    edi, dword ptr [0x40d014]     ; -> obj1:0xC014
        mov    bl, byte ptr [edi + 2]
        mov    bh, byte ptr [edi + 7]
        mov    dx, word ptr [edi + 0x18]
        mov    ecx, edx
        add    edx, 2
        test   bl, 8
        jne    loc_1_0C20
        or     bl, 8
        mov    al, bl
        out    dx, al
loc_1_0C20:
        mov    al, byte ptr [ebp + 8]
        or     al, 0x80
        xchg   ecx, edx
        out    dx, al
        xor    bh, al
        xor    bl, 2
        xchg   bl, al
        xchg   ecx, edx
        out    dx, al
        mov    byte ptr [edi + 2], al
        push   ecx
        test   bl, 0x70
        je     loc_1_0C41
        movzx  ecx, byte ptr [edi + 0xc]
        jmp    loc_1_0C45
loc_1_0C41:
        movzx  ecx, byte ptr [edi + 0xd]
loc_1_0C45:
        cmp    ecx, 0
        je     loc_1_0C4E
loc_1_0C4A:
        in     al, 0x21
        loop   loc_1_0C4A
loc_1_0C4E:
        pop    ecx
        add    edx, -1
        in     al, dx
        and    al, 0xf0
        mov    ah, al
        mov    al, bl
        and    al, 0x7f
        xchg   ecx, edx
        out    dx, al
        mov    byte ptr [edi], al
        push   ecx
        movzx  ecx, byte ptr [edi + 0xc]
        cmp    ecx, 0
        je     loc_1_0C6E
loc_1_0C6A:
        in     al, 0x21
        loop   loc_1_0C6A
loc_1_0C6E:
        pop    ecx
        xchg   ecx, edx
        in     al, dx
        shr    al, 1
        shr    al, 1
        shr    al, 1
        shr    al, 1
        or     al, ah
        xor    al, 0x88
        xor    bh, al
        mov    byte ptr [edi + 7], bh
        xor    ah, ah
        pop    edi
        pop    edx
        pop    ecx
        pop    ebx
        leave  
        ret    
        db  0CCh
sub_1_0C8C:
        enter  0, 0
        push   ecx
        push   eax
        mov    eax, dword ptr [0x401fed]     ; -> obj1:0x0FED
        cmp    eax, 0
        je     loc_1_0CB2
        mov    ecx, dword ptr [ebp + 8]
        cmp    ecx, 0
        jne    loc_1_0CAC
        VxDCall  VPICD.VPICD_Physically_Mask     ; cd2008000300
        jmp    loc_1_0CB2
loc_1_0CAC:
        VxDCall  VPICD.VPICD_Physically_Unmask   ; cd2009000300
loc_1_0CB2:
        pop    eax
        pop    ecx
        leave  
        ret    
        db  0CCh, 0CCh
sub_1_0CB8:
        enter  0, 0
        pushfd 
        mov    eax, dword ptr [ebp + 8]
        VMMCall  VMM.Signal_Semaphore            ; cd2028000100
        popfd  
        leave  
        ret    
        db  0CCh, 0CCh, 0CCh
sub_1_0CCC:
        enter  0, 0
        pushfd 
        push   esi
        push   edx
        mov    eax, dword ptr [ebp + 8]
        cmp    eax, 0
        je     loc_1_0CE9
        mov    edx, dword ptr [ebp + 0x10]
        mov    esi, dword ptr [ebp + 0xc]
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    eax, esi
loc_1_0CE9:
        pop    edx
        pop    esi
        popfd  
        leave  
        ret    
        db  0CCh, 0CCh
sub_1_0CF0:
        enter  0, 0
        mov    eax, dword ptr [ebp + 8]
        VxDCall  VTD.VTD_End_Min_Int_Period      ; cd2004000500
        leave  
        ret    
        db  0CCh
sub_1_0D00:
        enter  0, 0
        mov    eax, dword ptr [ebp + 8]
        VxDCall  VTD.VTD_Begin_Min_Int_Period    ; cd2003000500
        leave  
        ret    
        db  0CCh
sub_1_0D10:
        push   edi
        push   esi
        push   ebx
        mov    ecx, dword ptr [esp + 0x10]
        mov    edx, dword ptr [esp + 0x14]
        pushfd 
        push   edx
        cmp    byte ptr [0x40cf81], 1        ; -> loc_1_BF81
        jne    loc_1_0D3E
        mov    al, 0x90
        or     al, cl
        mov    dx, 0x18
        out    dx, al
        mov    al, 0x50
        or     al, cl
        out    dx, al
        mov    dx, 0x1a
        in     al, dx
        mov    ah, al
        in     al, dx
        jmp    loc_1_0D6E
loc_1_0D3E:
        movzx  eax, cx
        mov    edi, 0xe
        mul    edi
        add    eax, 0x402000                 ; -> loc_1_1000
        mov    edi, eax
        mov    al, byte ptr [edi + 0xc]
        or     al, 4
        mov    dx, word ptr [edi]
        out    dx, al
        call   sub_1_3405
        mov    dx, word ptr [edi + 6]
        in     al, dx
        mov    ah, al
        mov    dx, word ptr [edi + 6]
        in     al, dx
        call   sub_1_33EF
loc_1_0D6E:
        xchg   al, ah
        inc    ax
        movzx  eax, ax
        pop    edx
        cmp    dx, 0
        je     loc_1_0D7E
        shl    eax, 1
loc_1_0D7E:
        popfd  
        pop    ebx
        pop    esi
        pop    edi
        ret    
        db  0CCh
sub_1_0D84:
        push   edi
        push   esi
        push   ebx
        mov    edi, dword ptr [esp + 0x10]
        mov    esi, edi
        pushfd 
        cmp    dword ptr [0x40d014], 0       ; -> obj1:0xC014
        je     loc_1_0E58
        mov    edi, dword ptr [0x40d014]     ; -> obj1:0xC014
        movzx  eax, byte ptr [edi + 3]
        and    eax, 0xffffffbf
        push   eax
        push   0x18
        call   sub_1_1D05
        add    esp, 8
        cmp    dword ptr [esi + 8], 0
        je     loc_1_0DCF
        movzx  eax, byte ptr [edi + 3]
        and    eax, 0xffffff7f
        push   eax
        push   0x18
        call   sub_1_1D05
        add    esp, 8
        jmp    loc_1_0DE3
loc_1_0DCF:
        movzx  eax, byte ptr [edi + 3]
        or     eax, 0x80
        push   eax
        push   0x18
        call   sub_1_1D05
        add    esp, 8
loc_1_0DE3:
        test   byte ptr [edi + 8], 2
        je     loc_1_0DEB
        shl    dword ptr [esi], 1
loc_1_0DEB:
        mov    eax, dword ptr [esi]
        push   eax
        push   0x15
        call   sub_1_1D05
        add    esp, 8
        mov    eax, dword ptr [esi]
        shr    eax, 8
        push   eax
        push   0x16
        call   sub_1_1D05
        add    esp, 8
        mov    eax, dword ptr [esi]
        shr    eax, 0x10
        push   eax
        push   0x17
        call   sub_1_1D05
        add    esp, 8
        mov    eax, dword ptr [esi + 4]
        push   eax
        push   0x13
        call   sub_1_1D05
        add    esp, 8
        mov    eax, dword ptr [esi + 4]
        shr    eax, 8
        push   eax
        push   0x14
        call   sub_1_1D05
        add    esp, 8
        push   2
        call   sub_1_3B66
        add    esp, 4
        movzx  eax, byte ptr [edi + 3]
        or     eax, 0x40
        push   eax
        push   0x18
        call   sub_1_1D05
        add    esp, 8
        jmp    loc_1_0F66
loc_1_0E58:
        cmp    byte ptr [0x40cf81], 1        ; -> loc_1_BF81
        jne    loc_1_0EEC
        mov    al, 0x90
        or     al, byte ptr [esi + 0xc]
        mov    dx, 0x18
        out    dx, al
        mov    al, 4
        cmp    dword ptr [esi + 8], 0
        jne    loc_1_0E79
        or     al, 8
loc_1_0E79:
        mov    ebx, dword ptr [esi + 4]
        cmp    dword ptr [esi + 0x14], 0
        je     loc_1_0E86
        shr    ebx, 1
        or     al, 0x40
loc_1_0E86:
        cmp    dword ptr [esi + 0x10], 0
        je     loc_1_0EA5
        or     al, 1
        push   eax
        mov    eax, dword ptr [esi + 0x10]
        mov    ecx, 2
        mov    edx, 0
        or     edx, dword ptr [esi + 0xc]
        call   sub_1_0F6C
        pop    eax
loc_1_0EA5:
        mov    edx, 0x70
        or     edx, dword ptr [esi + 0xc]
        mov    ecx, 1
        call   sub_1_0F6C
        mov    eax, ebx
        dec    eax
        mov    ecx, 2
        mov    edx, 0x40
        or     edx, dword ptr [esi + 0xc]
        call   sub_1_0F6C
        mov    eax, dword ptr [esi]
        mov    ecx, 3
        mov    edx, 0x20
        or     edx, dword ptr [esi + 0xc]
        call   sub_1_0F6C
        mov    al, 0xa0
        or     al, byte ptr [esi + 0xc]
        mov    dx, 0x18
        out    dx, al
        jmp    loc_1_0F66
loc_1_0EEC:
        mov    eax, dword ptr [esi + 0xc]
        mov    edi, 0xe
        mul    edi
        add    eax, 0x402000                 ; -> loc_1_1000
        mov    edi, eax
        mov    al, byte ptr [edi + 0xc]
        or     al, 4
        mov    dx, word ptr [edi]
        out    dx, al
        mov    eax, dword ptr [esi]
        shr    eax, 0x10
        mov    dx, word ptr [edi + 8]
        out    dx, al
        mov    ebx, dword ptr [esi]
        mov    ecx, dword ptr [esi + 4]
        cmp    byte ptr [edi + 0xd], 0
        je     loc_1_0F1F
        shr    ebx, 1
        shr    ecx, 1
loc_1_0F1F:
        call   sub_1_3405
        mov    dx, word ptr [edi + 2]
        out    dx, al
        mov    al, bl
        mov    dx, word ptr [edi + 4]
        out    dx, al
        mov    al, bh
        mov    dx, word ptr [edi + 4]
        out    dx, al
        dec    ecx
        mov    al, cl
        mov    dx, word ptr [edi + 6]
        out    dx, al
        mov    al, ch
        mov    dx, word ptr [edi + 6]
        out    dx, al
        call   sub_1_33EF
        mov    al, 4
        cmp    dword ptr [esi + 8], 0
        je     loc_1_0F55
        mov    al, 8
loc_1_0F55:
        or     al, 0x40
        or     al, byte ptr [edi + 0xc]
        mov    dx, word ptr [edi + 0xa]
        out    dx, al
        mov    al, byte ptr [edi + 0xc]
        mov    dx, word ptr [edi]
        out    dx, al
loc_1_0F66:
        popfd  
        pop    ebx
        pop    esi
        pop    edi
        ret    
        db  0CCh
sub_1_0F6C:
        push   eax
        mov    al, dl
        mov    dx, 0x18
        out    dx, al
        pop    eax
        mov    dx, 0x1a
loc_1_0F79:
        out    dx, al
        shr    eax, 8
        loop   loc_1_0F79
        ret    
sub_1_0F80:
        pushfd 
        push   ebx
        push   ecx
        push   edx
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf000100
        pop    edx
        pop    ecx
        pop    ebx
        popfd  
        ret    
        db  0CCh
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  0030Ah                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  2, 2                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'VQIC117 '                  ; DDB_Name
        dd  080000000h                  ; DDB_Init_Order
        dd  VQIC117_Control          ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h
sub_1_0FE4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0FE8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0FEC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0FF6:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_1000:
        or     al, byte ptr [eax]
        or     al, 0
        add    byte ptr [eax], al
        add    dword ptr [eax], eax
        xchg   dword ptr [eax], eax
        or     eax, dword ptr [eax]
        add    byte ptr [eax], al
        or     al, byte ptr [eax]
        or     al, 0
        add    al, byte ptr [eax]
        add    eax, dword ptr [eax]
        add    dword ptr [eax], 0xb
        add    byte ptr [ecx], al
        add    byte ptr [edx], cl
        add    byte ptr [eax + eax], cl
        add    al, 0
        add    eax, 0xb008100
        add    byte ptr [edx], al
        add    byte ptr [edx], cl
        add    byte ptr [eax + eax], cl
        push   es
        add    byte ptr [edi], al
        add    byte ptr [edx + 0x3000b00], al
        add    ah, dl
        add    al, bl
        add    al, al
        add    dl, al
        add    byte ptr [edi + 0xd600], cl
        add    esp, edx
        add    al, bl
        add    ah, al
        add    dh, al
        add    byte ptr [ebx + 0x100d600], cl
        add    esp, edx
        add    al, bl
        add    al, cl
        add    dl, cl
        add    byte ptr [ecx + 0x200d600], cl
        add    esp, edx
        add    al, bl
        add    ah, cl
        add    dh, cl
        add    byte ptr [edx + 0x300d600], cl
        add    dword ptr [eax], eax
        add    byte ptr [ebx], al
        add    byte ptr [eax], dh            ; -> loc_1_0730
        pop    ss
        inc    eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edi + edx + 0x40], dl ; -> loc_1_0754
        add    byte ptr [eax + 0x17], ah     ; -> loc_1_0760
        inc    eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    ah, dh
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
sub_1_1090:
        cmp    byte ptr [0x40cd82], 0x22     ; -> obj1:0xBD82
        push   ebx
        push   esi
        jne    loc_1_10A8
        push   0x40cd48                      ; -> loc_1_BD48
        call   sub_1_110B
        add    esp, 4
loc_1_10A8:
        cmp    word ptr [0x40cf9c], 0        ; -> obj1:0xBF9C
        je     loc_1_10F4
        mov    bl, 0
        jbe    loc_1_10EB
loc_1_10B6:
        movzx  eax, bl
        shl    eax, 4
        lea    esi, [eax + 0x40cfac]         ; -> obj1:0xBFAC
        mov    eax, dword ptr [esi]
        test   eax, eax
        je     loc_1_10DB
        push   eax
        call   sub_1_0304
        add    esp, 4
        test   eax, eax
        jne    loc_1_10DB
        mov    dword ptr [esi], 0
loc_1_10DB:
        inc    bl
        movzx  ecx, word ptr [0x40cf9c]      ; -> obj1:0xBF9C
        movzx  eax, bl
        cmp    eax, ecx
        jb     loc_1_10B6
loc_1_10EB:
        mov    word ptr [0x40cf9c], 0        ; -> obj1:0xBF9C
loc_1_10F4:
        push   1
        push   0x40ce78                      ; -> obj1:0xBE78
        push   0x40cf80                      ; -> obj1:0xBF80
        call   sub_1_1BF3
        add    esp, 0xc
        pop    esi
        pop    ebx
        ret    
sub_1_110B:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    byte ptr [esi + 0x3a], 0x22
        jne    loc_1_1163
        cmp    dword ptr [esi + 0x10], 0
        je     loc_1_1155
        mov    eax, dword ptr [0x40d014]     ; -> obj1:0xC014
        mov    al, byte ptr [eax + 8]
        or     al, 4
        push   eax
        push   0x1e
        call   sub_1_1D05
        add    esp, 8
        call   sub_1_1CD7
        mov    eax, dword ptr [esi + 8]
        test   eax, eax
        je     loc_1_114C
        push   0x402cd4                      ; -> loc_1_1CD4
        push   dword ptr [esi + 0x10]
        push   3
        call   eax
        add    esp, 0xc
loc_1_114C:
        mov    dword ptr [esi + 0x10], 0
        jmp    loc_1_1177
loc_1_1155:
        push   2
        push   0x1587a
        push   0x1513
        jmp    loc_1_116F
loc_1_1163:
        push   1
        push   0x1587a
        push   0x1514
loc_1_116F:
        call   sub_1_1CB6
        add    esp, 0xc
loc_1_1177:
        pop    esi
        ret    
sub_1_1179:
        push   ebp
        mov    eax, 0xb4
        mov    ebp, esp
        sub    esp, 0xd8
        mov    ecx, 0x76
        push   ebx
        push   esi
        mov    byte ptr [ebp - 0xd8], al
        push   edi
        mov    edx, 0xf6
        mov    ebx, 0xb6
        mov    byte ptr [ebp - 0xd5], cl
        mov    byte ptr [ebp - 0xd4], dl
        mov    byte ptr [ebp - 0xd3], bl
        mov    byte ptr [ebp - 0xd7], 0x34
        mov    byte ptr [ebp - 0xd6], 0x74
        mov    byte ptr [ebp - 0xd1], 0xf4
        mov    byte ptr [ebp - 0xd0], 0xfc
        mov    byte ptr [ebp - 0xd2], dl
        mov    byte ptr [ebp - 0xcc], dl
        mov    byte ptr [ebp - 0xcf], 0x7c
        mov    byte ptr [ebp - 0xce], 0x7e
        mov    byte ptr [ebp - 0xcb], cl
        mov    byte ptr [ebp - 0xc8], al
        mov    byte ptr [ebp - 0xc7], bl
        mov    byte ptr [ebp - 0xc6], al
        mov    byte ptr [ebp - 0xcd], 0xfe
        mov    byte ptr [ebp - 0xca], 0x74
        mov    byte ptr [ebp - 0xc9], 0x34
        mov    byte ptr [ebp - 0xc2], dl
        mov    byte ptr [ebp - 0xc1], bl
        mov    byte ptr [ebp - 0xc0], al
        mov    byte ptr [ebp - 0xbf], al
        mov    byte ptr [ebp - 0xc5], 0xbc
        mov    byte ptr [ebp - 0xc4], 0xfc
        mov    byte ptr [ebp - 0xbc], cl
        mov    byte ptr [ebp - 0xbb], dl
        mov    byte ptr [ebp - 0xba], bl
        mov    byte ptr [ebp - 0xb9], dl
        mov    byte ptr [ebp - 0xc3], 0xf4
        mov    byte ptr [ebp - 0xbe], 0x34
        mov    byte ptr [ebp - 0xbd], 0x74
        mov    byte ptr [ebp - 0xb8], 0xf4
        mov    byte ptr [ebp - 0xb7], 0xfc
        mov    byte ptr [ebp - 0xb3], dl
        mov    byte ptr [ebp - 0xb2], cl
        mov    byte ptr [ebp - 0xb6], 0x7c
        mov    byte ptr [ebp - 0xb5], 0x7e
        mov    byte ptr [ebp - 0xaf], al
        mov    byte ptr [ebp - 0xae], bl
        mov    byte ptr [ebp - 0xb4], 0xfe
        mov    byte ptr [ebp - 0xb1], 0x74
        mov    byte ptr [ebp - 0xb0], 0x34
        mov    byte ptr [ebp - 0xad], dl
        mov    byte ptr [ebp - 0xa9], dl
        mov    byte ptr [ebp - 0xa8], bl
        mov    byte ptr [ebp - 0xa7], al
        mov    byte ptr [ebp - 0xac], 0xf4
        mov    byte ptr [ebp - 0xab], 0xfc
        mov    byte ptr [ebp - 0xaa], 0xf4
        mov    byte ptr [ebp - 0xa6], al
        mov    byte ptr [ebp - 0xa3], cl
        mov    byte ptr [ebp - 0xa2], dl
        mov    byte ptr [ebp - 0xa1], bl
        mov    byte ptr [ebp - 0xa0], dl
        mov    byte ptr [ebp - 0xa5], 0x34
        mov    byte ptr [ebp - 0xa4], 0x74
        mov    byte ptr [ebp - 0x9f], 0xf4
        mov    byte ptr [ebp - 0x9e], 0xfc
        mov    byte ptr [ebp - 0x9b], cl
        mov    byte ptr [ebp - 0x99], cl
        mov    byte ptr [ebp - 0x9d], 0x7c
        mov    byte ptr [ebp - 0x9c], 0x7e
        mov    byte ptr [ebp - 0x96], al
        mov    byte ptr [ebp - 0x95], bl
        mov    byte ptr [ebp - 0x9a], 0x36
        mov    byte ptr [ebp - 0x98], 0x74
        mov    byte ptr [ebp - 0x97], 0x34
        mov    byte ptr [ebp - 0x94], al
        mov    byte ptr [ebp - 0x90], dl
        mov    byte ptr [ebp - 0x8f], bl
        mov    byte ptr [ebp - 0x8e], al
        mov    byte ptr [ebp - 0x93], 0xbc
        mov    byte ptr [ebp - 0x92], 0xfc
        mov    byte ptr [ebp - 0x8d], al
        mov    byte ptr [ebp - 0x8a], cl
        mov    byte ptr [ebp - 0x89], dl
        mov    byte ptr [ebp - 0x88], bl
        mov    byte ptr [ebp - 0x91], 0xf4
        mov    byte ptr [ebp - 0x8c], 0x34
        mov    byte ptr [ebp - 0x8b], 0x74
        mov    byte ptr [ebp - 0x86], 0xf4
        mov    byte ptr [ebp - 0x87], dl
        mov    byte ptr [ebp - 0x82], cl
        mov    byte ptr [ebp - 0x85], 0xfc
        mov    byte ptr [ebp - 0x84], 0x7c
        mov    byte ptr [ebp - 0x83], 0x7e
        mov    byte ptr [ebp - 0x80], cl
        mov    byte ptr [ebp - 0x7d], al
        mov    byte ptr [ebp - 0x7c], bl
        mov    byte ptr [ebp - 0x7b], dl
        mov    byte ptr [ebp - 0x81], 0x36
        mov    byte ptr [ebp - 0x7f], 0x74
        mov    byte ptr [ebp - 0x7e], 0x34
        mov    byte ptr [ebp - 0x77], dl
        mov    byte ptr [ebp - 0x76], bl
        mov    byte ptr [ebp - 0x75], al
        mov    byte ptr [ebp - 0x74], al
        mov    byte ptr [ebp - 0x7a], 0xf4
        mov    byte ptr [ebp - 0x79], 0xfc
        mov    byte ptr [ebp - 0x78], 0xf4
        mov    byte ptr [ebp - 0x71], cl
        mov    byte ptr [ebp - 0x6f], cl
        mov    byte ptr [ebp - 0x73], 0x34
        mov    byte ptr [ebp - 0x72], 0x74
        mov    byte ptr [ebp - 0x70], 0x36
        mov    byte ptr [ebp - 0x6d], 0xf4
        mov    byte ptr [ebp - 0x6c], 0xfc
        mov    byte ptr [ebp - 0x6b], 0x7c
        mov    byte ptr [ebp - 0x6a], 0x7e
        mov    byte ptr [ebp - 0x6e], dl
        mov    byte ptr [ebp - 0x69], 0xfe
        mov    byte ptr [ebp - 0x68], dl
        mov    byte ptr [ebp - 0x67], cl
        mov    byte ptr [ebp - 0x64], al
        mov    byte ptr [ebp - 0x63], bl
        mov    byte ptr [ebp - 0x66], 0x74
        mov    byte ptr [ebp - 0x65], 0x34
        mov    byte ptr [ebp - 0x62], al
        mov    byte ptr [ebp - 0x5e], dl
        mov    byte ptr [ebp - 0x5d], bl
        mov    byte ptr [ebp - 0x5c], al
        mov    byte ptr [ebp - 0x61], 0xbc
        mov    byte ptr [ebp - 0x60], 0xfc
        mov    byte ptr [ebp - 0x5b], al
        mov    byte ptr [ebp - 0x58], cl
        mov    byte ptr [ebp - 0x5f], 0xf4
        mov    byte ptr [ebp - 0x56], cl
        mov    byte ptr [ebp - 0x55], dl
        mov    byte ptr [ebp - 0x5a], 0x34
        mov    byte ptr [ebp - 0x59], 0x74
        mov    byte ptr [ebp - 0x57], 0x36
        mov    byte ptr [ebp - 0x54], 0xf4
        mov    byte ptr [ebp - 0x53], 0xfc
        mov    byte ptr [ebp - 0x4f], dl
        mov    byte ptr [ebp - 0x4e], cl
        mov    byte ptr [ebp - 0x52], 0x7c
        mov    byte ptr [ebp - 0x51], 0x7e
        mov    byte ptr [ebp - 0x4b], al
        mov    byte ptr [ebp - 0x4a], bl
        mov    byte ptr [ebp - 0x50], 0xfe
        mov    byte ptr [ebp - 0x4d], 0x74
        mov    byte ptr [ebp - 0x4c], 0x34
        mov    byte ptr [ebp - 0x49], dl
        mov    byte ptr [ebp - 0x45], dl
        mov    byte ptr [ebp - 0x44], bl
        mov    byte ptr [ebp - 0x43], al
        mov    byte ptr [ebp - 0x48], 0xf4
        mov    byte ptr [ebp - 0x47], 0xfc
        mov    byte ptr [ebp - 0x42], al
        mov    byte ptr [ebp - 0x3f], cl
        mov    byte ptr [ebp - 0x46], 0xf4
        mov    byte ptr [ebp - 0x3d], cl
        mov    byte ptr [ebp - 0x3c], dl
        mov    byte ptr [ebp - 0x41], 0x34
        mov    byte ptr [ebp - 0x40], 0x74
        mov    byte ptr [ebp - 0x3e], 0x36
        mov    byte ptr [ebp - 0x3b], 0xf4
        mov    byte ptr [ebp - 0x3a], 0xfc
        mov    byte ptr [ebp - 0x37], cl
        mov    byte ptr [ebp - 0x35], cl
        mov    byte ptr [ebp - 0x39], 0x7c
        mov    byte ptr [ebp - 0x38], 0x7e
        mov    byte ptr [ebp - 0x32], al
        mov    byte ptr [ebp - 0x31], bl
        mov    byte ptr [ebp - 0x36], 0x36
        mov    byte ptr [ebp - 0x34], 0x74
        mov    byte ptr [ebp - 0x33], 0x34
        mov    byte ptr [ebp - 0x2f], 0xbc
        mov    byte ptr [ebp - 0x2e], 0xfc
        mov    byte ptr [ebp - 0x2d], 0xf4
        mov    byte ptr [ebp - 0x30], al
        mov    byte ptr [ebp - 0x2c], dl
        mov    byte ptr [ebp - 0x2b], bl
        mov    byte ptr [ebp - 0x2a], al
        mov    byte ptr [ebp - 0x29], al
        mov    byte ptr [ebp - 0x26], cl
        mov    byte ptr [ebp - 0x24], cl
        mov    byte ptr [ebp - 0x23], dl
        mov    byte ptr [ebp - 0x28], 0x34
        mov    byte ptr [ebp - 0x27], 0x74
        mov    byte ptr [ebp - 0x25], 0x36
        mov    byte ptr [ebp - 0x22], 0xf4
        mov    byte ptr [ebp - 0x21], 0xfc
        mov    byte ptr [ebp - 0x1e], cl
        mov    byte ptr [ebp - 0x1c], cl
        mov    byte ptr [ebp - 0x20], 0x7c
        mov    byte ptr [ebp - 0x1f], 0x7e
        mov    byte ptr [ebp - 0x19], al
        mov    byte ptr [ebp - 0x18], bl
        mov    byte ptr [ebp - 0x1d], 0x36
        mov    byte ptr [ebp - 0x1b], 0x74
        mov    byte ptr [ebp - 0x1a], 0x34
        mov    byte ptr [ebp - 0x17], dl
        mov    byte ptr [ebp - 0x13], dl
        mov    byte ptr [ebp - 0x12], bl
        mov    byte ptr [ebp - 0x11], al
        mov    byte ptr [ebp - 0x16], 0xf4
        mov    byte ptr [ebp - 0x15], 0xfc
        mov    byte ptr [ebp - 0x14], 0xf4
        mov    byte ptr [ebp - 0x10], 0x38
        mov    byte ptr [ebp - 0xf], 0x28
        mov    byte ptr [ebp - 0xe], 8
        mov    byte ptr [ebp - 0xd], 0
        mov    byte ptr [ebp - 0xc], 0x2a
        mov    byte ptr [ebp - 0xb], 0xa
        mov    esi, dword ptr [ebp + 0xc]
        mov    byte ptr [ebp - 0xa], 2
        cmp    byte ptr [esi + 0x22], 0x22
        jne    loc_1_1622
        mov    dword ptr [ebp - 8], 0x40cd48 ; -> loc_1_BD48
        mov    al, byte ptr [esi + 0xf]
        add    al, 0x30
        mov    byte ptr [0x40cd4b], al       ; -> loc_1_BD4B
        mov    eax, dword ptr [ebp - 8]
        push   eax
        VxDCall  BIOS.Service_0021h              ; cd2021002b00
        add    esp, 4
        mov    dword ptr [ebp - 4], 0x40cd48 ; -> loc_1_BD48
        mov    dword ptr [0x40cd50], eax     ; -> obj1:0xBD50
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [0x40cd54], eax     ; -> obj1:0xBD54
        push   eax
        mov    dword ptr [0x40cd54], eax     ; -> obj1:0xBD54
        VxDCall  BIOS.Service_0022h              ; cd2022002b00
        mov    dword ptr [0x40cd54], eax     ; -> obj1:0xBD54
        add    esp, 4
        mov    edi, 0x40cd60                 ; -> obj1:0xBD60
        mov    ecx, 0x45
        push   0xc8
        mov    dword ptr [0x40cd54], eax     ; -> obj1:0xBD54
        rep movsd dword ptr es:[edi], dword ptr [esi]
        push   0x40cd9d                      ; -> loc_1_BD9D
        lea    eax, [ebp - 0xd8]
        push   eax
        call   sub_1_1D93
        add    esp, 0xc
        lea    eax, [ebp - 0x10]
        push   7
        push   0x40ce65                      ; -> loc_1_BE65
        push   eax
        call   sub_1_1D93
        add    esp, 0xc
        movzx  ax, byte ptr [0x40cd6f]       ; -> loc_1_BD6F
        push   eax
        call   sub_1_0324
        mov    word ptr [0x40cd76], ax       ; -> obj1:0xBD76
        add    esp, 4
        jmp    loc_1_1633
loc_1_1622:
        xor    eax, eax
        mov    dword ptr [0x40cd50], eax     ; -> obj1:0xBD50
        mov    dword ptr [0x40cd54], eax     ; -> obj1:0xBD54
        mov    dword ptr [0x40cd58], eax     ; -> obj1:0xBD58
loc_1_1633:
        mov    eax, dword ptr [ebp + 8]
        pop    edi
        pop    esi
        mov    dword ptr [0x40cd5c], eax     ; -> obj1:0xBD5C
        xor    eax, eax
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_1644:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        cmp    byte ptr [esi + 0x3a], 0x22
        jne    loc_1_16D9
        mov    eax, dword ptr [esi + 8]
        lea    edi, [esi + 0x10]
        test   eax, eax
        je     loc_1_1678
        cmp    dword ptr [edi], 0
        jne    loc_1_1697
        push   1
        push   0x402cd4                      ; -> loc_1_1CD4
        push   dword ptr [esi + 0xc]
        push   1
        call   eax
        add    esp, 0x10
        mov    dword ptr [edi], eax
        jmp    loc_1_1692
loc_1_1678:
        push   2
        mov    dword ptr [edi], 0
        push   0x15878
        push   0x1599
        call   sub_1_1CB6
        add    esp, 0xc
loc_1_1692:
        cmp    dword ptr [edi], 0
        je     loc_1_16CB
loc_1_1697:
        movzx  eax, word ptr [esi + 0x2e]
        push   eax
        add    esi, 0x18
        call   sub_1_0348
        add    esp, 4
        mov    eax, dword ptr [0x40cf90]     ; -> obj1:0xBF90
        push   eax
        push   esi
        call   sub_1_1DDF
        add    esp, 8
        mov    esi, eax
        test   esi, esi
        je     loc_1_16EF
        push   0x40cd48                      ; -> loc_1_BD48
        call   sub_1_110B
        add    esp, 4
        jmp    loc_1_16EF
loc_1_16CB:
        push   3
        push   0x15878
        push   0x15a1
        jmp    loc_1_16E5
loc_1_16D9:
        push   1
        push   0x15878
        push   0x1514
loc_1_16E5:
        call   sub_1_1CB6
        add    esp, 0xc
        mov    esi, eax
loc_1_16EF:
        mov    eax, esi
        pop    edi
        pop    esi
        ret    
sub_1_16F4:
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    dword ptr [0x40d00c], ebx     ; -> obj1:0xC00C
        mov    dword ptr [0x40d010], ebx     ; -> obj1:0xC010
        cmp    dword ptr [esp + 0x14], ebx
        jne    loc_1_17A6
        mov    word ptr [0x40cf9c], bx       ; -> obj1:0xBF9C
        cmp    byte ptr [0x401fff], bl       ; -> obj1:0x0FFF
        je     loc_1_1784
        mov    esi, dword ptr [esp + 0x10]
loc_1_1722:
        movzx  edi, bl
        shl    edi, 4
        lea    eax, [edi + 0x40cfa8]         ; -> obj1:0xBFA8
        lea    ecx, [edi + 0x40cfa4]         ; -> obj1:0xBFA4
        push   eax
        push   ecx
        push   0xe
        push   7
        push   8
        push   esi
        call   sub_1_03C0
        add    esp, 0x18
        test   eax, eax
        je     loc_1_1756
        inc    word ptr [0x40cf9c]           ; -> obj1:0xBF9C
        mov    dword ptr [edi + 0x40cfac], eax ; -> obj1:0xBFAC
loc_1_1756:
        inc    bl
        cmp    bl, 4
        jae    loc_1_1761
        test   eax, eax
        jne    loc_1_1722
loc_1_1761:
        cmp    word ptr [0x40cf9c], 2        ; -> obj1:0xBF9C
        jae    loc_1_1784
        push   1
        push   0x15810
        push   0x1503
        call   sub_1_1CB6
        add    esp, 0xc
        mov    dword ptr [0x40d00c], eax     ; -> obj1:0xC00C
loc_1_1784:
        cmp    dword ptr [esp + 0x18], 0
        je     loc_1_17BF
        push   1
        push   0x15810
        push   0x1583
        call   sub_1_1CB6
        add    esp, 0xc
        mov    dword ptr [0x40d010], eax     ; -> obj1:0xC010
        jmp    loc_1_17BF
loc_1_17A6:
        push   1
        push   0x15810
        push   0x1584
        call   sub_1_1CB6
        add    esp, 0xc
        mov    dword ptr [0x40d00c], eax     ; -> obj1:0xC00C
loc_1_17BF:
        call   sub_1_0388
        and    al, 0xf
        pop    edi
        pop    esi
        mov    byte ptr [0x40cf81], al       ; -> loc_1_BF81
        mov    byte ptr [0x40cf80], 0        ; -> obj1:0xBF80
        pop    ebx
        ret    
sub_1_17D6:
        push   esi
        xor    eax, eax
        mov    esi, dword ptr [esp + 0x10]
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [esi], eax
        mov    edx, dword ptr [ecx]
        test   edx, edx
        je     loc_1_1803
        mov    cx, word ptr [esp + 0xc]
loc_1_17EE:
        cmp    dword ptr [esi], 0
        jne    loc_1_181C
        cmp    word ptr [edx], cx
        jne    loc_1_17FC
        mov    dword ptr [esi], edx
        jmp    loc_1_17FF
loc_1_17FC:
        mov    edx, dword ptr [edx + 0xc]
loc_1_17FF:
        test   edx, edx
        jne    loc_1_17EE
loc_1_1803:
        cmp    dword ptr [esi], 0
        jne    loc_1_181C
        push   1
        push   0x15827
        push   0x1513
        call   sub_1_1CB6
        add    esp, 0xc
loc_1_181C:
        pop    esi
        ret    
sub_1_181E:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi], 0
        je     loc_1_1938
        lea    eax, [ebp - 4]
        mov    ebx, dword ptr [ebp + 0x10]
        mov    cx, word ptr [ebx + 0x1a]
        push   eax
        push   ecx
        push   dword ptr [ebp + 0xc]
        call   sub_1_17D6
        add    esp, 0xc
        mov    edi, eax
        test   edi, edi
        jne    loc_1_194E
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 8]
        mov    edx, dword ptr [ecx + 0x378]
        push   dword ptr [edx]
        push   ecx
        push   dword ptr [esi + 0x10]
        call   sub_1_21E0
        add    esp, 0xc
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ecx + 8]
        mov    eax, dword ptr [edx + 0x378]
        push   dword ptr [eax]
        push   ebx
        call   sub_1_219F
        add    esp, 8
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ecx + 8]
        add    eax, 0x378
        push   eax
        call   sub_1_1F57
        add    esp, 4
        cmp    word ptr [ebx + 0x18], 0x1112
        jne    loc_1_18AE
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 8]
        mov    byte ptr [ecx + 0x4ac], 0xff
loc_1_18AE:
        test   edi, edi
        jne    loc_1_190E
loc_1_18B2:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 8]
        mov    eax, dword ptr [ecx + 0x250]
        test   eax, eax
        je     loc_1_190E
        push   dword ptr [eax]
        push   ecx
        push   dword ptr [esi + 0x10]
        call   sub_1_2001
        add    esp, 0xc
        mov    edi, eax
        test   edi, edi
        jne    loc_1_190E
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [eax + 8]
        mov    eax, dword ptr [edx + 0x250]
        add    edx, 0x128
        push   dword ptr [eax + 4]
        push   dword ptr [eax]
        push   edx
        call   sub_1_1F88
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        add    eax, 0x250
        push   eax
        call   sub_1_1F57
        add    esp, 4
        test   edi, edi
        je     loc_1_18B2
loc_1_190E:
        xor    edi, edi
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        cmp    dword ptr [eax + 0x4a8], edi
        jne    loc_1_194E
        cmp    dword ptr [eax + 0x128], edi
        je     loc_1_194E
        push   1
        push   dword ptr [eax + 0x4a4]
        call   sub_1_1F1A
        add    esp, 8
        jmp    loc_1_194E
loc_1_1938:
        push   1
        push   0x15816
        push   0x1581
        call   sub_1_1CB6
        add    esp, 0xc
        mov    edi, eax
loc_1_194E:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_1957:
        mov    eax, dword ptr [esp + 4]
        sub    esp, 4
        push   esi
        cmp    byte ptr [eax], 0
        push   edi
        je     loc_1_1997
        lea    eax, [esp + 8]
        mov    edi, dword ptr [esp + 0x18]
        mov    cx, word ptr [edi + 0x1a]
        push   eax
        push   ecx
        push   dword ptr [esp + 0x1c]
        call   sub_1_17D6
        add    esp, 0xc
        mov    esi, eax
        test   esi, esi
        jne    loc_1_19AD
        mov    eax, dword ptr [esp + 8]
        push   edi
        push   dword ptr [eax + 4]
        call   sub_1_2310
        add    esp, 8
        jmp    loc_1_19AD
loc_1_1997:
        push   1
        push   0x15815
        push   0x1581
        call   sub_1_1CB6
        add    esp, 0xc
        mov    esi, eax
loc_1_19AD:
        mov    eax, esi
        pop    edi
        pop    esi
        add    esp, 4
        ret    
sub_1_19B5:
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    edi, dword ptr [esp + 0x24]
        push   ebp
        mov    esi, dword ptr [esp + 0x20]
        cmp    byte ptr [esi], bl
        je     loc_1_1BD3
        mov    eax, dword ptr [esp + 0x30]
        cmp    eax, ebx
        je     loc_1_19FF
        push   0x14
        push   eax
        call   sub_1_0450
        add    esp, 8
        mov    ebp, eax
        test   ebp, ebp
        jne    loc_1_1A19
        push   2
        push   0x15814
        push   0x1585
        call   sub_1_1CB6
        add    esp, 0xc
        mov    ebx, eax
        jmp    loc_1_1A19
loc_1_19FF:
        push   1
        push   0x15814
        push   0x1585
        call   sub_1_1CB6
        mov    ebp, dword ptr [esp + 0x24]
        add    esp, 0xc
        mov    ebx, eax
loc_1_1A19:
        test   ebx, ebx
        jne    loc_1_1A39
        mov    cx, word ptr [edi + 0x1a]
        lea    eax, [esp + 0x10]
        push   eax
        push   ecx
        push   dword ptr [esp + 0x2c]
        call   sub_1_17D6
        add    esp, 0xc
        mov    ebx, eax
        test   ebx, ebx
        je     loc_1_1A4D
loc_1_1A39:
        mov    ax, word ptr [edi + 0x18]
        cmp    ax, 0x1101
        je     loc_1_1A4D
        cmp    ax, 0x1100
        jne    loc_1_1BBE
loc_1_1A4D:
        movzx  eax, word ptr [edi + 0x18]
        cmp    eax, 0x1100
        jl     loc_1_1B3E
        cmp    eax, 0x1101
        jle    loc_1_1A7E
        cmp    eax, 0x1102
        je     loc_1_1B09
        cmp    eax, 0x1112
        je     loc_1_1B28
        jmp    loc_1_1B3E
loc_1_1A7E:
        mov    cx, word ptr [edi + 0x1a]
        lea    eax, [esp + 0x10]
        push   eax
        push   ecx
        push   dword ptr [esp + 0x2c]
        call   sub_1_2706
        add    esp, 0xc
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_1_1AE7
        lea    eax, [edi + 0x2c]
        mov    ecx, dword ptr [esp + 0x10]
        push   eax
        push   dword ptr [ecx + 8]
        push   esi
        call   sub_1_2601
        mov    ecx, dword ptr [esp + 0x1c]
        add    esp, 0xc
        push   dword ptr [ecx + 8]
        push   dword ptr [ecx + 4]
        call   sub_1_257D
        mov    edx, dword ptr [esp + 0x18]
        add    esp, 8
        mov    ecx, dword ptr [edx + 8]
        movzx  edx, word ptr [ecx + 0x4da]
        mov    al, byte ptr [ecx + 0x4e4]
        push   eax
        push   edx
        mov    edx, dword ptr [esp + 0x18]
        push   dword ptr [edx + 4]
        call   sub_1_2482
        add    esp, 0xc
        jmp    loc_1_1AF8
loc_1_1AE7:
        push   ebx
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 0x1580
        jne    loc_1_1AF8
        xor    ebx, ebx
loc_1_1AF8:
        push   dword ptr [esp + 0x10]
        push   dword ptr [esi + 0x10]
        call   sub_1_23E0
        add    esp, 8
        jmp    loc_1_1B3E
loc_1_1B09:
        mov    eax, dword ptr [esp + 0x10]
        mov    ecx, dword ptr [eax + 8]
        mov    byte ptr [ecx + 0x4b1], 0xff
        push   dword ptr [esp + 0x10]
        push   dword ptr [esi + 0x10]
        call   sub_1_23E0
        add    esp, 8
        jmp    loc_1_1B3E
loc_1_1B28:
        mov    al, byte ptr [edi + 0x2c]
        mov    ecx, dword ptr [esp + 0x10]
        mov    edx, dword ptr [ecx + 8]
        mov    byte ptr [edx + 0x4ac], al
        mov    dword ptr [edi], 0
loc_1_1B3E:
        test   ebx, ebx
        jne    loc_1_1BBE
        lea    eax, [esp + 0x14]
        push   eax
        push   ebp
        push   edi
        call   sub_1_2364
        add    esp, 0xc
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_1_1BC2
        mov    eax, dword ptr [esp + 0x10]
        mov    eax, dword ptr [eax + 8]
        lea    ecx, [eax + 0x250]
        cmp    dword ptr [ecx], 0
        je     loc_1_1B74
        push   dword ptr [esp + 0x2c]
        push   dword ptr [esp + 0x18]
        push   ecx
        jmp    loc_1_1BA4
loc_1_1B74:
        push   dword ptr [esp + 0x14]
        push   eax
        push   dword ptr [esi + 0x10]
        call   sub_1_2001
        mov    edx, dword ptr [esp + 0x1c]
        add    esp, 0xc
        test   eax, eax
        mov    eax, dword ptr [edx + 8]
        push   dword ptr [esp + 0x2c]
        push   dword ptr [esp + 0x18]
        jne    loc_1_1B9E
        add    eax, 0x128
        jmp    loc_1_1BA3
loc_1_1B9E:
        add    eax, 0x250
loc_1_1BA3:
        push   eax
loc_1_1BA4:
        call   sub_1_1F88
        add    esp, 0xc
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_1_1BC2
        push   dword ptr [esp + 0x10]
        call   sub_1_232C
        add    esp, 4
loc_1_1BBE:
        test   ebx, ebx
        je     loc_1_1BE9
loc_1_1BC2:
        test   ebp, ebp
        je     loc_1_1BE9
        push   0x14
        push   ebp
        call   sub_1_041C
        add    esp, 8
        jmp    loc_1_1BE9
loc_1_1BD3:
        push   1
        push   0x15814
        push   0x1581
        call   sub_1_1CB6
        add    esp, 0xc
        mov    ebx, eax
loc_1_1BE9:
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
sub_1_1BF3:
        push   ebx
        xor    eax, eax
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        push   ebp
        cmp    byte ptr [esi], al
        je     loc_1_1C39
        mov    edi, dword ptr [esp + 0x18]
        mov    ebp, dword ptr [edi]
        test   ebp, ebp
        je     loc_1_1C30
        mov    bx, word ptr [esp + 0x1c]
loc_1_1C10:
        push   ebx
        push   ebp
        push   esi
        call   sub_1_28ED
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_1C29
        push   ebp
        push   edi
        call   sub_1_283E
        add    esp, 8
loc_1_1C29:
        mov    ebp, dword ptr [ebp + 0xc]
        test   ebp, ebp
        jne    loc_1_1C10
loc_1_1C30:
        test   eax, eax
        jne    loc_1_1C4D
        mov    byte ptr [esi], 0
        jmp    loc_1_1C4D
loc_1_1C39:
        push   1
        push   0x15813
        push   0x1581
        call   sub_1_1CB6
        add    esp, 0xc
loc_1_1C4D:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_1C52:
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    esi, dword ptr [esp + 0x10]
        mov    eax, dword ptr [esi + 0x8c]
        test   eax, eax
        jne    loc_1_1CAE
        cmp    byte ptr [esi], bl
        je     loc_1_1C78
        push   1
        push   dword ptr [esp + 0x18]
        push   esi
        call   sub_1_1BF3
        add    esp, 0xc
loc_1_1C78:
        mov    eax, dword ptr [esp + 0x1c]
        lea    edi, [esi + 0x15]
        mov    dword ptr [esi + 0x10], eax
        xor    eax, eax
        mov    word ptr [esi + 2], ax
        mov    dword ptr [esi + 0xc], eax
        stosd  dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        stosb  byte ptr es:[edi], al
        lea    eax, [esi + 0x1c]
        push   eax
        push   1
        call   sub_1_29FE
        add    esp, 8
        push   dword ptr [esp + 0x14]
        call   sub_1_29CB
        add    esp, 4
        mov    byte ptr [esi], 1
        jmp    loc_1_1CB0
loc_1_1CAE:
        mov    ebx, eax
loc_1_1CB0:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_1CB6:
        movzx  ecx, byte ptr [esp + 0xc]
        mov    eax, dword ptr [esp + 8]
        and    eax, 0xfff
        shl    eax, 4
        or     eax, ecx
        movzx  ecx, word ptr [esp + 4]
        shl    ecx, 0x10
        or     eax, ecx
        ret    
loc_1_1CD4:
        xor    eax, eax
        ret    
sub_1_1CD7:
        mov    eax, dword ptr [0x40d014]     ; -> obj1:0xC014
        movzx  eax, byte ptr [eax + 0x22]
        cmp    eax, 1
        je     loc_1_1CF1
        cmp    eax, 2
        je     loc_1_1CF8
        cmp    eax, 3
        je     loc_1_1CFF
        jmp    loc_1_1D04
loc_1_1CF1:
        call   sub_1_2B30
        jmp    loc_1_1D04
loc_1_1CF8:
        call   sub_1_2ADC
        jmp    loc_1_1D04
loc_1_1CFF:
        jmp    sub_1_2A36
loc_1_1D04:
        ret    
sub_1_1D05:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        mov    esi, dword ptr [0x40d014]     ; -> obj1:0xC014
        call   sub_1_05A8
        mov    byte ptr [ebp - 1], al
        test   al, al
        jne    loc_1_1D24
        call   sub_1_2BE8
loc_1_1D24:
        movzx  eax, byte ptr [esi + 0x13]
        test   eax, eax
        je     loc_1_1D4E
        cmp    eax, 1
        je     loc_1_1D5F
        mov    bl, byte ptr [ebp + 0xc]
loc_1_1D34:
        movzx  eax, word ptr [ebp + 8]
        cmp    eax, 0x18
        je     loc_1_1D70
        cmp    eax, 0x19
        je     loc_1_1D75
        cmp    eax, 0x1a
        je     loc_1_1D7A
        cmp    eax, 0x1e
        je     loc_1_1D7F
        jmp    loc_1_1D82
loc_1_1D4E:
        mov    bl, byte ptr [ebp + 0xc]
        push   ebx
        push   dword ptr [ebp + 8]
        call   sub_1_0544
        add    esp, 8
        jmp    loc_1_1D34
loc_1_1D5F:
        mov    bl, byte ptr [ebp + 0xc]
        push   ebx
        push   dword ptr [ebp + 8]
        call   sub_1_0490
        add    esp, 8
        jmp    loc_1_1D34
loc_1_1D70:
        mov    byte ptr [esi + 3], bl
        jmp    loc_1_1D82
loc_1_1D75:
        mov    byte ptr [esi + 4], bl
        jmp    loc_1_1D82
loc_1_1D7A:
        mov    byte ptr [esi + 5], bl
        jmp    loc_1_1D82
loc_1_1D7F:
        mov    byte ptr [esi + 8], bl
loc_1_1D82:
        cmp    byte ptr [ebp - 1], 0
        jne    loc_1_1D8D
        call   sub_1_2C1D
loc_1_1D8D:
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_1D93:
        mov    edx, dword ptr [esp + 4]
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        push   edi
        mov    edi, dword ptr [esp + 0x14]
        mov    ecx, edi
        shr    edi, 2
        and    cx, 3
        mov    eax, edi
        dec    edi
        test   eax, eax
        je     loc_1_1DC2
loc_1_1DB1:
        mov    eax, dword ptr [edx]
        add    edx, 4
        mov    dword ptr [esi], eax
        add    esi, 4
        mov    eax, edi
        dec    edi
        test   eax, eax
        jne    loc_1_1DB1
loc_1_1DC2:
        mov    ax, cx
        dec    cx
        test   ax, ax
        je     loc_1_1DDC
loc_1_1DCC:
        mov    al, byte ptr [edx]
        inc    edx
        mov    byte ptr [esi], al
        inc    esi
        mov    ax, cx
        dec    cx
        test   ax, ax
        jne    loc_1_1DCC
loc_1_1DDC:
        pop    edi
        pop    esi
        ret    
sub_1_1DDF:
        push   ebx
        push   esi
        push   edi
        mov    ebx, dword ptr [0x40d014]     ; -> obj1:0xC014
        push   0x2c
        push   0
        push   ebx
        lea    edi, [ebx + 9]
        call   sub_1_33CF
        mov    esi, dword ptr [esp + 0x1c]
        add    esp, 0xc
        mov    eax, 0x5050505
        mov    dword ptr [esi], ebx
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosb  byte ptr es:[edi], al
        mov    byte ptr [ebx + 0x12], 0
        mov    byte ptr [ebx + 0x13], 0
        mov    eax, 2
        mov    byte ptr [ebx + 0x15], al
        mov    byte ptr [ebx + 0x14], al
        mov    cx, word ptr [esi + 0x16]
        mov    word ptr [ebx + 0x18], cx
        cmp    byte ptr [esi + 0x3c], 1
        jne    loc_1_1E35
        push   esi
        call   sub_1_32DA
        add    esp, 4
        call   sub_1_3288
loc_1_1E35:
        cmp    byte ptr [esi + 0x1f], 1
        jne    loc_1_1E5E
        cmp    byte ptr [esi + 0x3c], 1
        jne    loc_1_1E5E
        mov    di, word ptr [esi + 0x16]
        add    di, 3
        push   edi
        call   sub_1_3281
        add    esp, 4
        and    al, 0xc2
        push   eax
        push   edi
        call   sub_1_3276
        add    esp, 8
loc_1_1E5E:
        push   dword ptr [esp + 0x14]
        push   esi
        call   sub_1_2FD2
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        jne    loc_1_1F14
        cmp    byte ptr [esi + 0x3c], 0
        jne    loc_1_1E9B
        push   1
        push   esi
        call   sub_1_2F18
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        jne    loc_1_1F14
        push   esi
        call   sub_1_2CE7
        add    esp, 4
        mov    edi, eax
loc_1_1E9B:
        test   edi, edi
        jne    loc_1_1F14
        cmp    byte ptr [esi + 0x3c], 1
        jne    loc_1_1F14
        push   1
        push   esi
        call   sub_1_2F18
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        jne    loc_1_1F14
        push   esi
        call   sub_1_2F02
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        jne    loc_1_1F14
        push   esi
        call   sub_1_2CE7
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        jne    loc_1_1F14
        push   0x1c
        call   sub_1_2C87
        add    esp, 4
        test   eax, eax
        je     loc_1_1EF0
        push   3
        push   0x15910
        push   0x1597
        jmp    loc_1_1F0A
loc_1_1EF0:
        push   0x1b
        call   sub_1_2C87
        add    esp, 4
        test   eax, eax
        je     loc_1_1F14
        push   4
        push   0x15910
        push   0x1596
loc_1_1F0A:
        call   sub_1_1CB6
        add    esp, 0xc
        mov    edi, eax
loc_1_1F14:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_1F1A:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   ebx
        push   ecx
        push   edx
        push   edi
        push   esi
        push   ebp
        push   ds
        push   es
        push   fs
        push   gs
        push   ss
        call   sub_1_3405
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [0x40cf8c], esp     ; -> obj1:0xBF8C
        mov    esp, dword ptr [ebp + 8]
        push   eax
        call   sub_1_33EF
        pop    eax
        pop    ss
        pop    gs
        pop    fs
        pop    es
        pop    ds
        pop    ebp
        pop    esi
        pop    edi
        pop    edx
        pop    ecx
        pop    ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
sub_1_1F57:
        push   esi
        call   sub_1_3405
        mov    eax, dword ptr [esp + 8]
        mov    ecx, dword ptr [eax]
        test   ecx, ecx
        je     loc_1_1F81
        lea    edx, [ecx + 8]
        mov    esi, dword ptr [edx]
        mov    dword ptr [eax], esi
        mov    dword ptr [ecx], 0
        mov    dword ptr [ecx + 4], 0
        mov    dword ptr [edx], 0
loc_1_1F81:
        call   sub_1_33EF
        pop    esi
        ret    
sub_1_1F88:
        push   ebx
        push   esi
        xor    ebx, ebx
        call   sub_1_3405
        xor    esi, esi
        xor    cl, cl
        mov    edx, dword ptr [esp + 0xc]
loc_1_1F99:
        test   esi, esi
        jne    loc_1_1FB9
        movzx  eax, cl
        imul   eax, eax, 0xc
        lea    eax, [eax + edx + 8]
        cmp    dword ptr [eax], 0
        jne    loc_1_1FAE
        mov    esi, eax
loc_1_1FAE:
        inc    cl
        cmp    cl, 0x18
        jb     loc_1_1F99
        test   esi, esi
        je     loc_1_1FE1
loc_1_1FB9:
        cmp    dword ptr [edx], 0
        jne    loc_1_1FC2
        mov    dword ptr [edx], esi
        jmp    loc_1_1FC8
loc_1_1FC2:
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [eax + 8], esi
loc_1_1FC8:
        mov    eax, dword ptr [esp + 0x10]
        mov    ecx, dword ptr [esp + 0x14]
        mov    dword ptr [edx + 4], esi
        mov    dword ptr [esi], eax
        mov    dword ptr [esi + 4], ecx
        mov    dword ptr [esi + 8], 0
        jmp    loc_1_1FF7
loc_1_1FE1:
        push   1
        push   0x1582a
        push   0x1501
        call   sub_1_1CB6
        add    esp, 0xc
        mov    ebx, eax
loc_1_1FF7:
        call   sub_1_33EF
        mov    eax, ebx
        pop    esi
        pop    ebx
        ret    
sub_1_2001:
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    edi, dword ptr [esp + 0x24]
        push   ebp
        mov    cx, word ptr [edi + 0x18]
        mov    esi, dword ptr [esp + 0x24]
        cmp    cx, 0x110f
        mov    dword ptr [esp + 0x14], ebx
        jne    loc_1_206C
        cmp    byte ptr [esi + 0x4e6], 0x22
        jne    loc_1_206C
        lea    eax, [edi + 0x10]
        lea    ecx, [edi + 0x14]
        push   eax
        push   ecx
        push   8
        push   ebx
        push   1
        push   dword ptr [esp + 0x34]
        call   sub_1_03C0
        add    esp, 0x18
        mov    dword ptr [esi + 0x4b4], eax
        cmp    eax, ebx
        jne    loc_1_2193
        push   1
        push   0x15821
        push   0x1503
        call   sub_1_1CB6
        add    esp, 0xc
        mov    dword ptr [edi], eax
        jmp    loc_1_2193
loc_1_206C:
        lea    eax, [esp + 0x12]
        lea    edx, [esp + 0x18]
        push   eax
        push   edx
        push   ecx
        call   sub_1_3532
        mov    bp, ax
        add    esp, 0xc
        cmp    byte ptr [esi + 0x4ac], 0xff
        jne    loc_1_2193
        test   byte ptr [esp + 0x18], 1
        je     loc_1_2193
        call   sub_1_3405
        xor    al, al
        mov    ecx, dword ptr [esi + 0x500]
        cmp    word ptr [ecx], 0
        jbe    loc_1_20F6
loc_1_20AD:
        test   bl, bl
        jne    loc_1_20F6
        movzx  edx, al
        shl    edx, 4
        mov    ecx, dword ptr [esi + 0x500]
        lea    ecx, [ecx + edx + 4]
        cmp    byte ptr [ecx], 0
        jne    loc_1_20E5
        mov    byte ptr [ecx], 1
        mov    ecx, dword ptr [esi + 0x500]
        mov    ebx, dword ptr [ecx + edx + 8]
        mov    dword ptr [edi + 0x14], ebx
        mov    bl, 1
        mov    ecx, dword ptr [esi + 0x500]
        mov    edx, dword ptr [ecx + edx + 0xc]
        mov    dword ptr [edi + 0x10], edx
loc_1_20E5:
        inc    al
        mov    edx, dword ptr [esi + 0x500]
        movzx  cx, al
        cmp    cx, word ptr [edx]
        jb     loc_1_20AD
loc_1_20F6:
        call   sub_1_33EF
        test   bl, bl
        je     loc_1_217B
        test   byte ptr [esp + 0x18], 2
        je     loc_1_2193
        push   dword ptr [edi + 0xc]
        push   dword ptr [esp + 0x24]
        call   sub_1_05D4
        add    esp, 8
        cmp    eax, -1
        je     loc_1_214C
        cmp    byte ptr [esi + 0x4e6], 0x22
        jne    loc_1_213A
        push   0
        push   2
        push   ebp
        push   dword ptr [edi + 0x14]
        push   eax
        call   sub_1_3434
        add    esp, 0x14
        jmp    loc_1_2160
loc_1_213A:
        movzx  ecx, bp
        push   ecx
        push   dword ptr [edi + 0x14]
        push   eax
        call   sub_1_1D93
        add    esp, 0xc
        jmp    loc_1_2164
loc_1_214C:
        push   1
        push   0x15821
        push   0x1585
        call   sub_1_1CB6
        add    esp, 0xc
loc_1_2160:
        mov    dword ptr [esp + 0x14], eax
loc_1_2164:
        cmp    dword ptr [esp + 0x14], 0
        je     loc_1_2193
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [esp + 0x14], 0
        mov    dword ptr [edi], eax
        jmp    loc_1_2193
loc_1_217B:
        push   1
        push   0x15821
        push   0x1582
        call   sub_1_1CB6
        mov    dword ptr [esp + 0x20], eax
        add    esp, 0xc
loc_1_2193:
        mov    eax, dword ptr [esp + 0x14]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
sub_1_219F:
        sub    esp, 8
        lea    eax, [esp + 2]
        push   esi
        lea    ecx, [esp + 8]
        push   eax
        mov    esi, dword ptr [esp + 0x14]
        push   ecx
        mov    ax, word ptr [esi + 0x18]
        push   eax
        call   sub_1_3532
        movzx  eax, word ptr [esp + 0x12]
        add    esp, 0xc
        push   eax
        push   esi
        mov    esi, dword ptr [esp + 0x1c]
        push   esi
        call   sub_1_1D93
        add    esp, 0xc
        push   esi
        call   sub_1_05E0
        add    esp, 4
        pop    esi
        add    esp, 8
        ret    
sub_1_21E0:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    edi, dword ptr [esp + 0x20]
        push   ebp
        mov    cx, word ptr [edi + 0x18]
        mov    esi, dword ptr [esp + 0x20]
        cmp    cx, 0x110f
        mov    byte ptr [esp + 0x11], bl
        jne    loc_1_2222
        cmp    byte ptr [esi + 0x4e6], 0x22
        jne    loc_1_2222
        push   dword ptr [esi + 0x4b4]
        call   sub_1_0304
        add    esp, 4
        mov    dword ptr [esi + 0x4b4], ebx
        jmp    loc_1_2306
loc_1_2222:
        lea    eax, [esp + 0x12]
        lea    edx, [esp + 0x14]
        push   eax
        push   edx
        push   ecx
        call   sub_1_3532
        mov    bp, ax
        add    esp, 0xc
        test   byte ptr [esp + 0x14], 1
        je     loc_1_2306
        cmp    byte ptr [esi + 0x4ac], 0xff
        jne    loc_1_22B8
        test   byte ptr [esp + 0x14], 4
        je     loc_1_22B8
        push   dword ptr [edi + 0xc]
        push   dword ptr [esp + 0x20]
        call   sub_1_05D4
        add    esp, 8
        cmp    eax, -1
        je     loc_1_2295
        cmp    byte ptr [esi + 0x4e6], 0x22
        jne    loc_1_2283
        push   0
        push   3
        push   ebp
        push   dword ptr [edi + 0x14]
        push   eax
        call   sub_1_3434
        add    esp, 0x14
        jmp    loc_1_22A9
loc_1_2283:
        movzx  ecx, bp
        push   ecx
        push   eax
        push   dword ptr [edi + 0x14]
        call   sub_1_1D93
        add    esp, 0xc
        jmp    loc_1_22AB
loc_1_2295:
        push   1
        push   0x15822
        push   0x1585
        call   sub_1_1CB6
        add    esp, 0xc
loc_1_22A9:
        mov    ebx, eax
loc_1_22AB:
        test   ebx, ebx
        je     loc_1_22B8
        cmp    dword ptr [edi], 0
        jne    loc_1_22B8
        mov    dword ptr [edi], ebx
        xor    ebx, ebx
loc_1_22B8:
        call   sub_1_3405
        xor    al, al
        mov    edx, dword ptr [esi + 0x500]
        cmp    word ptr [edx], 0
        jbe    loc_1_2301
loc_1_22CB:
        cmp    byte ptr [esp + 0x11], 0
        jne    loc_1_2301
        movzx  edx, al
        shl    edx, 4
        mov    ebp, dword ptr [edi + 0x10]
        add    edx, dword ptr [esi + 0x500]
        cmp    dword ptr [edx + 0xc], ebp
        jne    loc_1_22EF
        mov    byte ptr [edx + 4], 0
        mov    byte ptr [esp + 0x11], 1
loc_1_22EF:
        inc    al
        mov    ebp, dword ptr [esi + 0x500]
        movzx  dx, al
        cmp    dx, word ptr [ebp]
        jb     loc_1_22CB
loc_1_2301:
        call   sub_1_33EF
loc_1_2306:
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
sub_1_2310:
        mov    eax, dword ptr [esp + 4]
        push   esi
        mov    ecx, dword ptr [esp + 0xc]
        push   edi
        lea    esi, [eax + 0x34]
        lea    edi, [ecx + 0x20]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    edx, dword ptr [eax + 0x7c]
        pop    edi
        pop    esi
        mov    dword ptr [ecx + 0x2c], edx
        ret    
sub_1_232C:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 8]
        cmp    byte ptr [eax + 0x4a0], 0
        jne    loc_1_234A
        push   dword ptr [eax + 0x4a4]
        push   ecx
        call   sub_1_362D
        jmp    loc_1_2360
loc_1_234A:
        cmp    dword ptr [eax + 0x4a8], 0
        jne    loc_1_2363
        push   1
        push   dword ptr [eax + 0x4a4]
        call   sub_1_1F1A
loc_1_2360:
        add    esp, 8
loc_1_2363:
        ret    
sub_1_2364:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        lea    eax, [ebp - 2]
        push   esi
        lea    ecx, [ebp - 8]
        push   edi
        xor    esi, esi
        push   eax
        mov    ebx, dword ptr [ebp + 8]
        mov    ax, word ptr [ebx + 0x18]
        push   ecx
        push   eax
        call   sub_1_3532
        movzx  eax, word ptr [ebp - 2]
        add    esp, 0xc
        add    eax, 8
        push   eax
        call   sub_1_05FC
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        je     loc_1_23C1
        movzx  eax, word ptr [ebp - 2]
        push   eax
        push   edi
        push   ebx
        call   sub_1_1D93
        add    esp, 0xc
        mov    eax, dword ptr [ebp + 0x10]
        movzx  ecx, word ptr [ebp - 2]
        mov    dword ptr [eax], edi
        add    edi, ecx
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [edi], eax
        mov    dword ptr [edi + 4], eax
        jmp    loc_1_23D7
loc_1_23C1:
        push   1
        push   0x1582d
        push   0x1503
        call   sub_1_1CB6
        add    esp, 0xc
        mov    esi, eax
loc_1_23D7:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_23E0:
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        mov    eax, dword ptr [esi + 8]
        cmp    byte ptr [eax + 0x4a1], 0
        jne    loc_1_2478
        mov    cl, byte ptr [eax + 0x4e8]
        cmp    cl, 6
        je     loc_1_2445
        cmp    byte ptr [eax + 0x4e9], 2
        je     loc_1_2445
        cmp    byte ptr [eax + 0x4ea], 2
        je     loc_1_2445
        movzx  eax, cl
        push   esi
        push   0x404680                      ; -> loc_1_3680
        push   eax
        push   dword ptr [esp + 0x14]
        call   sub_1_0618
        add    esp, 0x10
        call   sub_1_2BE8
        mov    eax, dword ptr [esi + 8]
        add    eax, 0x4c4
        push   eax
        call   sub_1_3758
        add    esp, 4
        call   sub_1_2C1D
        jmp    loc_1_246E
loc_1_2445:
        push   dword ptr [esp + 8]
        call   sub_1_0698
        add    esp, 4
        mov    ecx, dword ptr [esi + 8]
        movzx  eax, byte ptr [ecx + 0x4e8]
        push   esi
        push   0x404680                      ; -> loc_1_3680
        push   eax
        push   dword ptr [esp + 0x14]
        call   sub_1_0618
        add    esp, 0x10
loc_1_246E:
        mov    eax, dword ptr [esi + 8]
        mov    byte ptr [eax + 0x4a1], 1
loc_1_2478:
        pop    esi
        ret    
sub_1_247A:
        mov    eax, dword ptr [esp + 4]
        shr    eax, 0x10
        ret    
sub_1_2482:
        sub    esp, 4
        push   ebx
        push   esi
        mov    bl, byte ptr [esp + 0x18]
        push   edi
        mov    esi, dword ptr [esp + 0x14]
        push   ebp
        push   dword ptr [esi + 0x110]
        mov    byte ptr [esi + 0xf4], bl
        call   sub_1_384C
        add    esp, 4
        test   al, al
        je     loc_1_24FA
        lea    edi, [esi + 0xe4]
        lea    ebp, [esi + 0xec]
        mov    dword ptr [esi + 0xd8], 7
        lea    edx, [esi + 0xf0]
        mov    dword ptr [edi], 4
        lea    eax, [esi + 0xe8]
        mov    dword ptr [ebp], 2
        mov    dword ptr [esi + 0xdc], 5
        mov    dword ptr [edx], 2
        mov    dword ptr [eax], 3
        mov    dword ptr [esi + 0xe0], 4
        jmp    loc_1_2540
loc_1_24FA:
        mov    ecx, dword ptr [esp + 0x1c]
        lea    edi, [esi + 0xe4]
        lea    edx, [ecx + 7]
        lea    ebp, [ecx + 5]
        lea    eax, [ecx + 4]
        mov    dword ptr [esi + 0xd8], edx
        lea    edx, [esi + 0xf0]
        mov    dword ptr [esi + 0xdc], ebp
        add    ecx, 3
        mov    dword ptr [edi], eax
        lea    ebp, [esi + 0xec]
        mov    dword ptr [esi + 0xe0], eax
        lea    eax, [ecx - 1]
        mov    dword ptr [ebp], eax
        mov    dword ptr [edx], eax
        lea    eax, [esi + 0xe8]
        mov    dword ptr [eax], ecx
loc_1_2540:
        test   bl, bl
        je     loc_1_2562
        xor    byte ptr [edi], 0x80
        xor    byte ptr [edx], 0x80
        xor    byte ptr [esi + 0xd8], 0x80
        xor    byte ptr [esi + 0xdc], 0x80
        xor    byte ptr [esi + 0xe0], 0x80
        xor    byte ptr [eax], 0x80
loc_1_2562:
        push   dword ptr [ebp]
        push   dword ptr [edx]
        push   dword ptr [esi + 0x110]
        call   sub_1_380E
        add    esp, 0xc
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_1_257D:
        push   ebx
        push   esi
        push   0x124
        xor    ebx, ebx
        mov    esi, dword ptr [esp + 0x10]
        push   ebx
        push   esi
        call   sub_1_33CF
        mov    eax, dword ptr [esp + 0x1c]
        add    esp, 0xc
        mov    dword ptr [esi + 0x110], eax
        mov    byte ptr [esi + 0x11b], bl
        mov    eax, 1
        mov    byte ptr [esi + 0x11f], bl
        mov    byte ptr [esi + 0x121], al
        mov    byte ptr [esi + 0x11c], bl
        mov    byte ptr [esi + 0x11d], bl
        mov    byte ptr [esi + 0x3b], al
        mov    word ptr [esi + 0x106], bx
        mov    byte ptr [esi], al
        mov    byte ptr [esi + 0x41], bl
        mov    byte ptr [esi + 0x1c], al
        mov    eax, 2
        mov    byte ptr [esi + 0x10e], bl
        mov    byte ptr [esi + 0x10f], bl
        mov    byte ptr [esi + 0x10d], bl
        mov    byte ptr [esi + 0x3e], al
        mov    byte ptr [esi + 0x3d], bl
        mov    byte ptr [esi + 0x44], al
        mov    byte ptr [esi + 0x45], bl
        mov    byte ptr [esi + 0x19], 0xa
        mov    byte ptr [esi + 0x46], 0xef
        pop    esi
        pop    ebx
        ret    
sub_1_2601:
        push   ebx
        push   esi
        mov    ebx, dword ptr [esp + 0x10]
        push   edi
        push   ebx
        call   sub_1_385F
        add    esp, 4
        lea    eax, [ebx + 0x128]
        push   eax
        call   sub_1_385F
        add    esp, 4
        lea    eax, [ebx + 0x250]
        push   eax
        call   sub_1_385F
        add    esp, 4
        xor    ecx, ecx
        lea    eax, [ebx + 0xd74]
        mov    byte ptr [ebx + 0x4a0], cl
        mov    byte ptr [ebx + 0x4a1], cl
        mov    dword ptr [ebx + 0x4a4], eax
        mov    dword ptr [ebx + 0x4a8], ecx
        mov    byte ptr [ebx + 0x4ad], cl
        mov    byte ptr [ebx + 0x4ae], cl
        mov    byte ptr [ebx + 0x4af], cl
        mov    byte ptr [ebx + 0x4b0], cl
        mov    dword ptr [ebx + 0x4b4], ecx
        lea    edi, [ebx + 0x4c4]
        mov    dword ptr [ebx + 0x4b8], ecx
        mov    esi, dword ptr [esp + 0x18]
        mov    dword ptr [ebx + 0x4bc], ecx
        mov    byte ptr [ebx + 0x4ac], 0xff
        mov    dword ptr [ebx + 0x4c0], ecx
        mov    ecx, 0xf
        rep movsd dword ptr es:[edi], dword ptr [esi]
        cmp    byte ptr [ebx + 0x4e6], 0x22
        jne    loc_1_26D9
        lea    eax, [ebx + 0x504]
        mov    esi, dword ptr [esp + 0x10]
        add    esi, 0x60
        mov    ecx, 0xb
        mov    dword ptr [ebx + 0x500], eax
        lea    eax, [ebx + 0x548]
        mov    edi, eax
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    dword ptr [0x40d014], eax     ; -> obj1:0xC014
        push   dword ptr [ebx + 0x500]
        push   2
        call   sub_1_29FE
        add    esp, 8
        jmp    loc_1_26F0
loc_1_26D9:
        mov    esi, dword ptr [esp + 0x10]
        add    esi, 0x1c
        mov    dword ptr [ebx + 0x500], esi
        mov    dword ptr [0x40d014], 0       ; -> obj1:0xC014
loc_1_26F0:
        lea    edi, [ebx + 0x574]
        mov    eax, 0xcccccccc
        mov    ecx, 0x200
        rep stosd dword ptr es:[edi], eax
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_2706:
        sub    esp, 4
        push   ebx
        push   esi
        mov    ebx, dword ptr [esp + 0x18]
        push   edi
        mov    edi, dword ptr [esp + 0x14]
        push   ebp
        push   ebx
        push   dword ptr [esp + 0x20]
        push   edi
        call   sub_1_17D6
        add    esp, 0xc
        push   eax
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 0x1513
        jne    loc_1_281A
        mov    dword ptr [esp + 0x10], 0
        push   0xd74
        call   sub_1_05FC
        add    esp, 4
        mov    ebp, eax
        call   sub_1_3887
        movzx  eax, ax
        push   eax
        call   sub_1_05FC
        add    esp, 4
        mov    esi, eax
        call   sub_1_3405
        xor    al, al
        xor    edx, edx
        mov    dword ptr [ebx], 0
loc_1_276F:
        cmp    dword ptr [ebx], edx
        jne    loc_1_278B
        movzx  ecx, al
        shl    ecx, 4
        add    ecx, edi
        cmp    dword ptr [ecx + 0xc], edx
        jne    loc_1_2785
        add    ecx, 8
        mov    dword ptr [ebx], ecx
loc_1_2785:
        inc    al
        cmp    al, 0x10
        jb     loc_1_276F
loc_1_278B:
        mov    eax, dword ptr [ebx]
        test   eax, eax
        je     loc_1_27DA
        test   ebp, ebp
        je     loc_1_27CC
        test   esi, esi
        je     loc_1_27CC
        cmp    dword ptr [edi], 0
        jne    loc_1_27A2
        mov    dword ptr [edi], eax
        jmp    loc_1_27A8
loc_1_27A2:
        mov    ecx, dword ptr [edi + 4]
        mov    dword ptr [ecx + 0xc], eax
loc_1_27A8:
        mov    dx, word ptr [esp + 0x1c]
        mov    eax, dword ptr [ebx]
        mov    dword ptr [edi + 4], eax
        mov    ecx, dword ptr [ebx]
        mov    word ptr [ecx], dx
        mov    eax, dword ptr [ebx]
        mov    dword ptr [eax + 4], esi
        mov    eax, dword ptr [ebx]
        mov    dword ptr [eax + 8], ebp
        mov    eax, dword ptr [ebx]
        mov    dword ptr [eax + 0xc], 0
        jmp    loc_1_27F2
loc_1_27CC:
        push   1
        push   0x15825
        push   0x1503
        jmp    loc_1_27E6
loc_1_27DA:
        push   1
        push   0x15825
        push   0x1505
loc_1_27E6:
        call   sub_1_1CB6
        mov    dword ptr [esp + 0x1c], eax
        add    esp, 0xc
loc_1_27F2:
        call   sub_1_33EF
        cmp    dword ptr [esp + 0x10], 0
        je     loc_1_2832
        test   ebp, ebp
        je     loc_1_280B
        push   ebp
        call   sub_1_05E0
        add    esp, 4
loc_1_280B:
        test   esi, esi
        je     loc_1_2832
        push   esi
        call   sub_1_05E0
        add    esp, 4
        jmp    loc_1_2832
loc_1_281A:
        push   1
        push   0x15825
        push   0x1580
        call   sub_1_1CB6
        mov    dword ptr [esp + 0x1c], eax
        add    esp, 0xc
loc_1_2832:
        mov    eax, dword ptr [esp + 0x10]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_1_283E:
        sub    esp, 4
        xor    eax, eax
        mov    dword ptr [esp], eax
        push   ebx
        push   esi
        xor    ebx, ebx
        mov    esi, dword ptr [esp + 0x10]
        push   edi
        mov    edi, dword ptr [esp + 0x18]
        mov    ecx, dword ptr [esi]
        test   ecx, ecx
        je     loc_1_286F
loc_1_285A:
        test   al, al
        jne    loc_1_2873
        cmp    ecx, edi
        jne    loc_1_2866
        mov    al, 1
        jmp    loc_1_286B
loc_1_2866:
        mov    ebx, ecx
        mov    ecx, dword ptr [ecx + 0xc]
loc_1_286B:
        test   ecx, ecx
        jne    loc_1_285A
loc_1_286F:
        test   al, al
        je     loc_1_28CA
loc_1_2873:
        push   dword ptr [edi + 4]
        call   sub_1_05E0
        add    esp, 4
        push   dword ptr [edi + 8]
        call   sub_1_05E0
        add    esp, 4
        call   sub_1_3405
        mov    eax, dword ptr [esi]
        cmp    edi, eax
        jne    loc_1_289B
        mov    eax, dword ptr [eax + 0xc]
        mov    dword ptr [esi], eax
        jmp    loc_1_28A9
loc_1_289B:
        cmp    dword ptr [esi + 4], edi
        jne    loc_1_28A3
        mov    dword ptr [esi + 4], ebx
loc_1_28A3:
        mov    eax, dword ptr [edi + 0xc]
        mov    dword ptr [ebx + 0xc], eax
loc_1_28A9:
        mov    dword ptr [edi + 4], 0
        mov    dword ptr [edi + 8], 0
        mov    dword ptr [edi + 0xc], 0
        mov    word ptr [edi], 0xffff
        call   sub_1_33EF
        jmp    loc_1_28E2
loc_1_28CA:
        push   1
        push   0x15826
        push   0x1513
        call   sub_1_1CB6
        mov    dword ptr [esp + 0x18], eax
        add    esp, 0xc
loc_1_28E2:
        mov    eax, dword ptr [esp + 0xc]
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_1_28ED:
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    esi, dword ptr [esp + 0x14]
        mov    edx, dword ptr [esi + 8]
        lea    ecx, [edx + 0x4a0]
        cmp    byte ptr [ecx], bl
        je     loc_1_29C5
        cmp    word ptr [esp + 0x18], 1
        jne    loc_1_2971
        call   sub_1_3405
        mov    eax, dword ptr [esi + 8]
        cmp    dword ptr [eax + 0x128], ebx
        je     loc_1_296A
        mov    edi, dword ptr [esp + 0x10]
        mov    dword ptr [edi + 4], ebx
        mov    eax, dword ptr [esi + 8]
        mov    byte ptr [eax + 0x4ac], bl
        mov    eax, dword ptr [esi + 8]
        mov    byte ptr [eax + 0x4a0], bl
        call   sub_1_33EF
        mov    eax, dword ptr [esi + 8]
        cmp    dword ptr [eax + 0x4a8], ebx
        je     loc_1_2958
        add    eax, 0x4b8
        push   eax
        call   sub_1_3900
        add    esp, 4
        jmp    loc_1_29B5
loc_1_2958:
        push   1
        push   dword ptr [eax + 0x4a4]
        call   sub_1_1F1A
        add    esp, 8
        jmp    loc_1_29B5
loc_1_296A:
        call   sub_1_33EF
        jmp    loc_1_29B1
loc_1_2971:
        cmp    dword ptr [edx + 0x250], 0
        jne    loc_1_299B
        cmp    dword ptr [edx + 0x128], 0
        jne    loc_1_299B
        push   1
        mov    byte ptr [ecx], 0
        mov    eax, dword ptr [esi + 8]
        push   dword ptr [eax + 0x4a4]
        call   sub_1_1F1A
        add    esp, 8
        jmp    loc_1_29B1
loc_1_299B:
        push   1
        push   0x15834
        push   0x1502
        call   sub_1_1CB6
        add    esp, 0xc
        mov    ebx, eax
loc_1_29B1:
        mov    edi, dword ptr [esp + 0x10]
loc_1_29B5:
        test   ebx, ebx
        jne    loc_1_29C5
        push   esi
        push   dword ptr [edi + 0x10]
        call   sub_1_388C
        add    esp, 8
loc_1_29C5:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_29CB:
        mov    ecx, dword ptr [esp + 4]
        push   esi
        push   edi
        xor    edx, edx
        mov    esi, 0xffff
        xor    edi, edi
        mov    dword ptr [ecx], edx
        mov    dword ptr [ecx + 4], edx
loc_1_29DF:
        movzx  eax, dl
        shl    eax, 4
        inc    dl
        add    eax, ecx
        cmp    dl, 0x10
        mov    word ptr [eax + 8], si
        mov    dword ptr [eax + 0xc], edi
        mov    dword ptr [eax + 0x10], edi
        mov    dword ptr [eax + 0x14], edi
        jb     loc_1_29DF
        pop    edi
        pop    esi
        ret    
sub_1_29FE:
        push   ebx
        xor    ecx, ecx
        mov    ebx, dword ptr [esp + 0xc]
        push   esi
        push   edi
        xor    edx, edx
        mov    edi, dword ptr [esp + 0x10]
loc_1_2A0D:
        movzx  esi, cl
        mov    eax, esi
        shl    eax, 4
        add    eax, ebx
        cmp    edi, 2
        mov    byte ptr [eax + 4], dl
        jne    loc_1_2A2B
        shl    esi, 0xf
        inc    word ptr [ebx]
        mov    dword ptr [eax + 8], esi
        mov    dword ptr [eax + 0xc], esi
loc_1_2A2B:
        inc    cl
        cmp    cl, 4
        jb     loc_1_2A0D
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_2A36:
        push   esi
        mov    esi, dword ptr [0x40d014]     ; -> obj1:0xC014
        mov    al, byte ptr [esi + 0x29]
        push   eax
        push   0x102
        call   sub_1_3276
        add    esp, 8
        call   sub_1_3405
        push   0xfc23
        call   sub_1_3281
        add    esp, 4
        push   0xf023
        call   sub_1_3281
        add    esp, 4
        push   0xc023
        call   sub_1_3281
        add    esp, 4
        push   0x23
        call   sub_1_3281
        add    esp, 4
        push   0x22
        call   sub_1_392C
        add    esp, 4
        test   al, 1
        jne    loc_1_2AD5
        push   0x23
        call   sub_1_3281
        add    esp, 4
        test   al, 0x80
        jne    loc_1_2AD5
        push   0x61
        push   0x24
        call   sub_1_3276
        add    esp, 8
        mov    al, byte ptr [esi + 0x2a]
        push   eax
        push   0x25
        call   sub_1_3276
        add    esp, 8
        push   0xfa
        push   0x24
        call   sub_1_3276
        add    esp, 8
        push   1
        push   0x25
        call   sub_1_3276
        add    esp, 8
loc_1_2AD5:
        call   sub_1_33EF
        pop    esi
        ret    
sub_1_2ADC:
        push   esi
        push   0x94
        mov    esi, dword ptr [0x40d014]     ; -> obj1:0xC014
        call   sub_1_3281
        add    esp, 4
        and    al, 0x7f
        push   eax
        push   0x94
        call   sub_1_3276
        add    esp, 8
        mov    cl, byte ptr [esi + 0x24]
        push   ecx
        push   0x102
        call   sub_1_3276
        add    esp, 8
        push   0x94
        call   sub_1_3281
        add    esp, 4
        or     al, 0x80
        push   eax
        push   0x94
        call   sub_1_3276
        add    esp, 8
        pop    esi
        ret    
sub_1_2B30:
        mov    eax, dword ptr [0x40d014]     ; -> obj1:0xC014
        mov    al, byte ptr [eax + 0x23]
        push   eax
        push   0x65
        call   sub_1_3276
        add    esp, 8
        ret    
sub_1_2B44:
        sub    esp, 4
        mov    word ptr [esp + 2], 0
        push   ebx
        push   esi
        push   edi
        push   ebp
        call   sub_1_05A8
        mov    byte ptr [esp + 0x11], al
        test   al, al
        jne    loc_1_2B64
        call   sub_1_2BE8
loc_1_2B64:
        mov    esi, dword ptr [esp + 0x18]
        mov    edi, dword ptr [esp + 0x1c]
loc_1_2B6C:
        xor    bp, bp
        push   0x20
        push   dword ptr [esp + 0x24]
        push   esi
        cmp    byte ptr [esp + 0x30], 2
        jne    loc_1_2B84
        call   sub_1_39F7
        jmp    loc_1_2B89
loc_1_2B84:
        call   sub_1_398D
loc_1_2B89:
        add    esp, 0xc
        mov    ebx, eax
        call   sub_1_2C1D
        call   sub_1_2BE8
        cmp    byte ptr [esp + 0x28], 0
        jne    loc_1_2BA4
        call   sub_1_0708
loc_1_2BA4:
        test   ebx, ebx
        je     loc_1_2BAC
        mov    bp, 1
loc_1_2BAC:
        test   bp, bp
        je     loc_1_2BD2
        inc    word ptr [esp + 0x12]
        cmp    word ptr [esp + 0x12], 3
        jae    loc_1_2BC9
        push   edi
        call   sub_1_3934
        add    esp, 4
        mov    ebx, eax
loc_1_2BC9:
        test   bp, bp
        je     loc_1_2BD2
        test   ebx, ebx
        je     loc_1_2B6C
loc_1_2BD2:
        cmp    byte ptr [esp + 0x11], 0
        jne    loc_1_2BDE
        call   sub_1_2C1D
loc_1_2BDE:
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_1_2BE8:
        push   ebp
        mov    eax, dword ptr [0x401fed]     ; -> obj1:0x0FED
        mov    ebp, esp
        sub    esp, 4
        test   eax, eax
        je     loc_1_2C03
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        VxDCall  VPICD.VPICD_Physically_Mask     ; cd2008000300
loc_1_2C03:
        xor    eax, eax
        mov    esp, ebp
        mov    dword ptr [0x40d024], eax     ; -> obj1:0xC024
        mov    dword ptr [0x40d028], eax     ; -> obj1:0xC028
        mov    dword ptr [0x40d020], 1       ; -> obj1:0xC020
        pop    ebp
        ret    
sub_1_2C1D:
        push   ebp
        mov    eax, dword ptr [0x401fed]     ; -> obj1:0x0FED
        mov    dword ptr [0x40d020], 0       ; -> obj1:0xC020
        mov    ebp, esp
        sub    esp, 4
        test   eax, eax
        je     loc_1_2C42
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        VxDCall  VPICD.VPICD_Physically_Unmask   ; cd2009000300
loc_1_2C42:
        xor    ecx, ecx
        cmp    dword ptr [0x40d028], ecx     ; -> obj1:0xC028
        je     loc_1_2C65
        mov    dword ptr [0x40d028], ecx     ; -> obj1:0xC028
        mov    dword ptr [0x40d024], ecx     ; -> obj1:0xC024
        mov    edx, dword ptr [0x40d02c]     ; -> obj1:0xC02C
        call   sub_1_3AC2
        jmp    loc_1_2C83
loc_1_2C65:
        cmp    dword ptr [0x40d024], 0       ; -> obj1:0xC024
        je     loc_1_2C83
        mov    dword ptr [0x40d024], 0       ; -> obj1:0xC024
        mov    edx, dword ptr [0x40d02c]     ; -> obj1:0xC02C
        call   sub_1_3A57
loc_1_2C83:
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_2C87:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [0x40d014]     ; -> obj1:0xC014
        push   ebp
        call   sub_1_3405
        mov    di, word ptr [esp + 0x14]
        xor    ebp, ebp
        cmp    di, 0x1b
        jne    loc_1_2CA8
        mov    bl, byte ptr [esi + 6]
        jmp    loc_1_2CAB
loc_1_2CA8:
        mov    bl, byte ptr [esi + 7]
loc_1_2CAB:
        push   edi
        call   sub_1_3B66
        add    esp, 4
        cmp    al, bl
        je     loc_1_2CDB
        cmp    di, 0x1b
        push   1
        push   0x158f0
        jne    loc_1_2CCC
        push   0x1596
        jmp    loc_1_2CD1
loc_1_2CCC:
        push   0x1597
loc_1_2CD1:
        call   sub_1_1CB6
        add    esp, 0xc
        mov    ebp, eax
loc_1_2CDB:
        call   sub_1_33EF
        mov    eax, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_2CE7:
        sub    esp, 0xc
        mov    eax, dword ptr [0x40d014]     ; -> obj1:0xC014
        mov    dword ptr [esp + 8], eax
        push   ebx
        push   esi
        xor    ebx, ebx
        mov    byte ptr [esp + 0xe], bl
        push   edi
        mov    esi, dword ptr [esp + 0x1c]
        push   ebp
        push   ebx
        push   ebx
        push   8
        push   ebx
        push   esi
        call   sub_1_4433
        mov    byte ptr [esp + 0x2b], al
        add    esp, 0x14
        mov    al, byte ptr [esi + 0xd]
        mov    byte ptr [esp + 0x10], al
        cmp    al, 0xff
        jne    loc_1_2D33
        push   ebx
        mov    al, byte ptr [esi + 0xe]
        push   ebx
        push   8
        push   eax
        push   esi
        call   sub_1_4433
        mov    byte ptr [esp + 0x24], al
        add    esp, 0x14
loc_1_2D33:
        push   0x8000
        call   sub_1_05FC
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        jne    loc_1_2D61
        push   1
        push   0x15914
        push   0x159d
        call   sub_1_1CB6
        add    esp, 0xc
        mov    ebx, eax
        jmp    loc_1_2EF8
loc_1_2D61:
        mov    byte ptr [esp + 0x14], 0
        mov    ebp, edi
loc_1_2D68:
        push   dword ptr [esp + 0x14]
        push   0x2000
        push   ebp
        push   0
        add    ebp, 0x2000
        call   sub_1_423F
        inc    byte ptr [esp + 0x24]
        add    esp, 0x10
        cmp    byte ptr [esp + 0x14], 4
        jb     loc_1_2D68
loc_1_2D8D:
        lea    eax, [esp + 0x15]
        lea    ecx, [esp + 0x13]
        lea    edx, [esp + 0x11]
        push   eax
        lea    eax, [esp + 0x16]
        push   ecx
        push   edx
        push   eax
        push   dword ptr [esp + 0x20]
        push   esi
        call   sub_1_3BE7
        add    esp, 0x18
        test   ebx, ebx
        jne    loc_1_2E6E
        mov    eax, dword ptr [esp + 0x18]
        mov    al, byte ptr [eax + 3]
        mov    cl, al
        and    cl, 3
        cmp    cl, byte ptr [esp + 0x12]
        jne    loc_1_2DD3
        and    al, 4
        shr    al, 2
        cmp    al, byte ptr [esp + 0x11]
        je     loc_1_2DF6
loc_1_2DD3:
        push   dword ptr [esp + 0x10]
        push   dword ptr [esp + 0x1b]
        push   esi
        call   sub_1_40A6
        add    esp, 0xc
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_1_2E6E
        mov    al, byte ptr [esp + 0x10]
        mov    byte ptr [esp + 0x17], al
loc_1_2DF6:
        test   ebx, ebx
        jne    loc_1_2E6E
        push   edi
        push   dword ptr [esp + 0x17]
        push   esi
        call   sub_1_3C35
        add    esp, 0xc
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_1_2E18
        mov    byte ptr [esp + 0x16], 1
        jmp    loc_1_2ED2
loc_1_2E18:
        push   ebx
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 0x1595
        jne    loc_1_2ED2
        xor    ebx, ebx
        cmp    byte ptr [esp + 0x10], bl
        je     loc_1_2E6A
        lea    eax, [esp + 0x15]
        lea    ecx, [esp + 0x13]
        lea    edx, [esp + 0x11]
        push   eax
        lea    eax, [esp + 0x16]
        push   ecx
        dec    byte ptr [esp + 0x18]
        push   edx
        push   eax
        push   dword ptr [esp + 0x20]
        push   esi
        call   sub_1_3BE7
        add    esp, 0x18
        cmp    byte ptr [esi + 0xd], 0xff
        je     loc_1_2ED2
        cmp    byte ptr [esp + 0x11], bl
        jne    loc_1_2ED2
        dec    byte ptr [esp + 0x10]
        jmp    loc_1_2ED2
loc_1_2E6A:
        push   1
        jmp    loc_1_2EBE
loc_1_2E6E:
        push   ebx
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 0x1592
        jne    loc_1_2ED2
        xor    ebx, ebx
        cmp    byte ptr [esp + 0x10], bl
        je     loc_1_2EBC
        lea    eax, [esp + 0x15]
        lea    ecx, [esp + 0x13]
        lea    edx, [esp + 0x11]
        push   eax
        lea    eax, [esp + 0x16]
        push   ecx
        dec    byte ptr [esp + 0x18]
        push   edx
        push   eax
        push   dword ptr [esp + 0x20]
        push   esi
        call   sub_1_3BE7
        add    esp, 0x18
        cmp    byte ptr [esi + 0xd], 0xff
        je     loc_1_2ED2
        cmp    byte ptr [esp + 0x11], bl
        jne    loc_1_2ED2
        dec    byte ptr [esp + 0x10]
        jmp    loc_1_2ED2
loc_1_2EBC:
        push   2
loc_1_2EBE:
        push   0x15914
        push   0x1593
        call   sub_1_1CB6
        add    esp, 0xc
        mov    ebx, eax
loc_1_2ED2:
        test   ebx, ebx
        jne    loc_1_2EE1
        cmp    byte ptr [esp + 0x16], 0
        je     loc_1_2D8D
loc_1_2EE1:
        cmp    byte ptr [esp + 0x16], 0
        je     loc_1_2EEF
        mov    al, byte ptr [esp + 0x10]
        mov    byte ptr [esi + 0xd], al
loc_1_2EEF:
        push   edi
        call   sub_1_05E0
        add    esp, 4
loc_1_2EF8:
        mov    eax, ebx
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
sub_1_2F02:
        call   sub_1_44A7
        test   eax, eax
        jne    loc_1_2F17
        push   dword ptr [esp + 4]
        call   sub_1_4485
        add    esp, 4
loc_1_2F17:
        ret    
sub_1_2F18:
        sub    esp, 4
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0x10]
        push   edi
        push   ebp
        mov    edi, dword ptr [0x40d014]     ; -> obj1:0xC014
        xor    ebp, ebp
        mov    dword ptr [esp + 0x10], ebp
loc_1_2F2F:
        mov    byte ptr [edi + 0x12], 0
        mov    byte ptr [edi + 0x13], 0
        mov    bl, byte ptr [edi + 8]
        mov    al, byte ptr [edi + 2]
        and    bl, 1
        and    al, 0xfb
        push   eax
        mov    byte ptr [edi + 2], al
        movzx  eax, word ptr [esi + 0x16]
        add    eax, 2
        push   eax
        call   sub_1_3276
        add    esp, 8
        push   0xa
        call   sub_1_0720
        add    esp, 4
        mov    al, byte ptr [edi + 2]
        or     al, 4
        push   eax
        mov    byte ptr [edi + 2], al
        movzx  eax, word ptr [esi + 0x16]
        add    eax, 2
        push   eax
        call   sub_1_3276
        add    esp, 8
        push   0x1388
        call   sub_1_0720
        add    esp, 4
        cmp    byte ptr [esp + 0x1c], 1
        jne    loc_1_2FB5
        push   esi
        call   sub_1_4679
        add    esp, 4
        mov    ebp, eax
        test   ebp, ebp
        jne    loc_1_2FB9
        test   bl, bl
        je     loc_1_2FB5
        call   sub_1_4656
        mov    al, byte ptr [edi + 8]
        or     al, 2
        push   eax
        push   0x1e
        call   sub_1_1D05
        add    esp, 8
loc_1_2FB5:
        test   ebp, ebp
        je     loc_1_2FC8
loc_1_2FB9:
        inc    dword ptr [esp + 0x10]
        cmp    dword ptr [esp + 0x10], 2
        jb     loc_1_2F2F
loc_1_2FC8:
        mov    eax, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_1_2FD2:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        call   sub_1_4A71
        add    esp, 4
        push   esi
        call   sub_1_49C4
        add    esp, 4
        test   eax, eax
        jne    loc_1_3053
        cmp    byte ptr [esi + 0x3c], 1
        jne    loc_1_3053
        push   esi
        call   sub_1_48FA
        add    esp, 4
        test   eax, eax
        jne    loc_1_3053
        push   dword ptr [esp + 0xc]
        call   sub_1_02C4
        add    esp, 4
        mov    dl, byte ptr [esi + 0x24]
        mov    cl, dl
        shr    ax, cl
        test   al, 1
        je     loc_1_3023
        push   esi
        call   sub_1_47BA
        add    esp, 4
        jmp    loc_1_3053
loc_1_3023:
        movzx  ax, dl
        push   eax
        call   sub_1_3055
        add    esp, 4
        test   eax, eax
        je     loc_1_303F
        push   esi
        call   sub_1_47BA
        add    esp, 4
        jmp    loc_1_3053
loc_1_303F:
        push   1
        push   0x15911
        push   0x159b
        call   sub_1_1CB6
        add    esp, 0xc
loc_1_3053:
        pop    esi
        ret    
sub_1_3055:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   esi
        lea    eax, [ebp - 0xc]
        xor    esi, esi
        push   esi
        push   esi
        push   eax
        call   sub_2_0000
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_309D
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 4]
        push   eax
        mov    dword ptr [ebp - 4], esi
        push   ecx
        mov    dword ptr [ebp - 8], esi
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp - 0xc]
        call   sub_1_3168
        add    esp, 0x10
        cmp    dword ptr [ebp - 8], esi
        je     loc_1_309D
        cmp    dword ptr [ebp - 4], esi
        jne    loc_1_309D
        mov    eax, 1
        jmp    loc_1_309F
loc_1_309D:
        xor    eax, eax
loc_1_309F:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_30A4:
        mov    eax, dword ptr [esp + 4]
        push   ebx
        mov    ecx, dword ptr [esp + 0xc]
loc_1_30AD:
        mov    bl, byte ptr [eax]
        test   bl, bl
        je     loc_1_30DD
        mov    dl, byte ptr [ecx]
        test   dl, dl
        je     loc_1_30DD
        inc    eax
        inc    ecx
        cmp    bl, 0x61
        jl     loc_1_30C8
        cmp    bl, 0x7a
        jg     loc_1_30C8
        sub    bl, 0x20
loc_1_30C8:
        cmp    dl, 0x61
        jl     loc_1_30D5
        cmp    dl, 0x7a
        jg     loc_1_30D5
        sub    dl, 0x20
loc_1_30D5:
        cmp    dl, bl
        je     loc_1_30AD
        xor    eax, eax
        jmp    loc_1_30E7
loc_1_30DD:
        mov    cl, byte ptr [ecx]
        sub    cl, byte ptr [eax]
        cmp    cl, 1
        sbb    eax, eax
        inc    eax
loc_1_30E7:
        pop    ebx
        ret    
sub_1_30E9:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x2c
        push   esi
        lea    eax, [ebp - 4]
        push   0
        lea    ecx, [ebp - 0x2c]
        push   eax
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 4], 0x20
        push   ecx
        push   1
        push   0x40d030                      ; -> loc_1_C030
        push   0
        push   esi
        call   sub_2_0008
        add    esp, 0x1c
        test   eax, eax
        jne    loc_1_3161
        push   0x40d038                      ; -> obj1:0xC038
        lea    eax, [ebp - 0x2c]
        push   eax
        call   sub_1_30A4
        add    esp, 8
        test   eax, eax
        jne    loc_1_3161
        push   1
        lea    eax, [ebp - 4]
        push   eax
        lea    ecx, [ebp - 0xc]
        mov    dword ptr [ebp - 4], 1
        push   ecx
        push   3
        push   0x40d040                      ; -> loc_1_C040
        push   0
        push   esi
        call   sub_2_0008
        add    esp, 0x1c
        test   eax, eax
        jne    loc_1_3161
        cmp    byte ptr [ebp - 0xc], 0
        mov    eax, 1
        je     loc_1_3163
loc_1_3161:
        xor    eax, eax
loc_1_3163:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_3168:
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, dword ptr [esp + 0x2c]
        test   ebp, ebp
        je     loc_1_326E
        mov    si, word ptr [esp + 0x30]
        mov    edi, dword ptr [esp + 0x34]
        mov    ebx, dword ptr [esp + 0x38]
loc_1_3188:
        cmp    dword ptr [edi], 0
        jne    loc_1_326E
        lea    eax, [esp + 0x18]
        push   0
        mov    dword ptr [esp + 0x18], 0
        mov    dword ptr [esp + 0x1c], 0
        push   ebp
        push   eax
        call   sub_2_0030
        add    esp, 0xc
        push   ebx
        push   edi
        push   esi
        push   dword ptr [esp + 0x24]
        call   sub_1_3168
        add    esp, 0x10
        cmp    dword ptr [edi], 0
        jne    loc_1_3252
        lea    eax, [esp + 0x1c]
        push   2
        push   ebp
        push   eax
        call   sub_2_0028
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_3252
        mov    eax, dword ptr [esp + 0x1c]
        push   0
        lea    ecx, [esp + 0x14]
        push   0
        mov    dword ptr [esp + 0x18], eax
        push   4
        push   dword ptr [esp + 0x1c]
        push   ecx
        call   sub_2_0018
        add    esp, 0x14
        test   eax, eax
        jne    loc_1_3252
loc_1_3200:
        lea    eax, [esp + 0x20]
        push   0
        push   8
        push   eax
        push   dword ptr [esp + 0x1c]
        call   sub_2_0020
        add    esp, 0x10
        cmp    word ptr [esp + 0x22], si
        jne    loc_1_3237
        push   ebp
        call   sub_1_30E9
        add    esp, 4
        test   eax, eax
        je     loc_1_3231
        mov    dword ptr [ebx], 1
        jmp    loc_1_3237
loc_1_3231:
        mov    dword ptr [edi], 1
loc_1_3237:
        lea    ecx, [esp + 0x10]
        push   0
        push   0
        push   4
        push   dword ptr [esp + 0x1c]
        push   ecx
        call   sub_2_0018
        add    esp, 0x14
        test   eax, eax
        je     loc_1_3200
loc_1_3252:
        lea    eax, [esp + 0x14]
        push   0
        push   ebp
        push   eax
        call   sub_2_0010
        mov    ebp, dword ptr [esp + 0x20]
        add    esp, 0xc
        test   ebp, ebp
        jne    loc_1_3188
loc_1_326E:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x18
        ret    
sub_1_3276:
        mov    dx, word ptr [esp + 4]
        mov    al, byte ptr [esp + 8]
        out    dx, al
        ret    
sub_1_3281:
        mov    dx, word ptr [esp + 4]
        in     al, dx
        ret    
sub_1_3288:
        mov    ecx, dword ptr [0x40d014]     ; -> obj1:0xC014
        movzx  eax, byte ptr [ecx + 0x22]
        test   eax, eax
        je     loc_1_32AC
        cmp    eax, 1
        je     loc_1_32B3
        cmp    eax, 2
        jl     loc_1_32D9
        cmp    eax, 3
        jle    loc_1_32AC
        cmp    eax, 4
        je     loc_1_32C0
        jmp    loc_1_32D9
loc_1_32AC:
        mov    al, byte ptr [ecx + 2]
        and    al, 0xdf
        jmp    loc_1_32C5
loc_1_32B3:
        mov    al, byte ptr [ecx + 0x23]
        or     al, 0x80
        push   eax
        mov    byte ptr [ecx + 0x23], al
        push   0x65
        jmp    loc_1_32D1
loc_1_32C0:
        mov    al, byte ptr [ecx + 2]
        and    al, 0x7f
loc_1_32C5:
        push   eax
        mov    byte ptr [ecx + 2], al
        movzx  eax, word ptr [ecx + 0x18]
        add    eax, 2
        push   eax
loc_1_32D1:
        call   sub_1_3276
        add    esp, 8
loc_1_32D9:
        ret    
sub_1_32DA:
        sub    esp, 4
        mov    byte ptr [esp + 3], 0
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0x10]
        push   edi
        push   ebp
        mov    edi, dword ptr [0x40d014]     ; -> obj1:0xC014
        mov    byte ptr [esi + 0xe], 0xff
        mov    byte ptr [edi + 0x22], 0
        push   esi
        call   sub_1_4B6B
        add    esp, 4
        test   al, al
        jne    loc_1_3317
        push   esi
        call   sub_1_4B68
        add    esp, 4
        test   al, al
        jne    loc_1_3317
        call   sub_1_4B4E
loc_1_3317:
        movzx  eax, word ptr [esi + 0x16]
        add    eax, 2
        xor    ebx, ebx
        xor    bp, bp
        push   eax
        call   sub_1_3281
        add    esp, 4
        mov    byte ptr [edi + 2], al
        call   sub_1_3288
loc_1_3334:
        mov    ax, word ptr [esi + 0x16]
        push   ebx
        push   eax
        call   sub_1_3276
        mov    ax, word ptr [esi + 0x16]
        add    esp, 8
        push   eax
        call   sub_1_3281
        add    esp, 4
        cmp    al, bl
        jne    loc_1_3355
        inc    bp
loc_1_3355:
        inc    bx
        cmp    bx, 0xff
        jbe    loc_1_3334
        cmp    bp, 0x100
        jne    loc_1_33C2
        mov    byte ptr [esi + 0xe], 0
loc_1_3369:
        xor    bp, bp
        call   sub_1_4AFC
        xor    ebx, ebx
loc_1_3373:
        mov    ax, word ptr [esi + 0x16]
        push   ebx
        push   eax
        call   sub_1_3276
        mov    ax, word ptr [esi + 0x16]
        add    esp, 8
        push   eax
        call   sub_1_3281
        add    esp, 4
        cmp    al, bl
        jne    loc_1_3394
        inc    bp
loc_1_3394:
        inc    bx
        cmp    bx, 0xff
        jbe    loc_1_3373
        cmp    bp, 0x100
        jae    loc_1_33AA
        mov    byte ptr [esi + 0xe], 2
        jmp    loc_1_33B6
loc_1_33AA:
        cmp    byte ptr [edi + 0x22], 0
        jne    loc_1_33B6
        mov    byte ptr [edi + 0x22], 4
        jmp    loc_1_33BB
loc_1_33B6:
        mov    byte ptr [esp + 0x13], 1
loc_1_33BB:
        cmp    byte ptr [esp + 0x13], 0
        je     loc_1_3369
loc_1_33C2:
        call   sub_1_1CD7
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_1_33CF:
        mov    edx, dword ptr [esp + 4]
        push   ebx
        mov    ecx, dword ptr [esp + 0x10]
        mov    eax, ecx
        dec    ecx
        test   eax, eax
        je     loc_1_33ED
        mov    bl, byte ptr [esp + 0xc]
loc_1_33E3:
        mov    eax, ecx
        mov    byte ptr [edx], bl
        inc    edx
        dec    ecx
        test   eax, eax
        jne    loc_1_33E3
loc_1_33ED:
        pop    ebx
        ret    
sub_1_33EF:
        push   eax
        cmp    byte ptr [0x40cf94], 0        ; -> obj1:0xBF94
        jne    loc_1_3403
        dec    word ptr [0x40cf82]           ; -> obj1:0xBF82
        jne    loc_1_3403
        sti    
loc_1_3403:
        pop    eax
        ret    
sub_1_3405:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   eax
        pushfd 
        pop    eax
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        and    al, 1
        mov    byte ptr [0x40cf94], al       ; -> obj1:0xBF94
        jne    loc_1_342F
        cmp    word ptr [0x40cf82], 0        ; -> obj1:0xBF82
        jne    loc_1_3428
        cli    
loc_1_3428:
        inc    word ptr [0x40cf82]           ; -> obj1:0xBF82
loc_1_342F:
        pop    eax
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_3434:
        sub    esp, 8
        mov    eax, dword ptr [0x40d014]     ; -> obj1:0xC014
        mov    dword ptr [esp + 4], eax
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x1c]
        push   ebp
        push   esi
        call   sub_1_3934
        mov    bx, word ptr [esp + 0x28]
        add    esp, 4
        mov    edi, eax
        mov    ebp, dword ptr [esp + 0x1c]
        movzx  eax, bx
        add    eax, esi
        cmp    bx, 0x80
        mov    dword ptr [esp + 0x10], eax
        jb     loc_1_34A5
loc_1_346C:
        test   edi, edi
        jne    loc_1_34A5
        push   dword ptr [esp + 0x2c]
        push   dword ptr [esp + 0x2c]
        push   0x80
        push   esi
        push   ebp
        call   sub_1_2B44
        add    esp, 0x14
        mov    edi, eax
        test   edi, edi
        jne    loc_1_349E
        sub    bx, 0x80
        add    ebp, 0x80
        add    esi, 0x80
loc_1_349E:
        cmp    bx, 0x80
        jae    loc_1_346C
loc_1_34A5:
        test   bx, bx
        je     loc_1_34C3
        test   edi, edi
        jne    loc_1_34C3
        push   dword ptr [esp + 0x2c]
        push   dword ptr [esp + 0x2c]
        push   ebx
        push   esi
        push   ebp
        call   sub_1_2B44
        add    esp, 0x14
        mov    edi, eax
loc_1_34C3:
        mov    eax, dword ptr [esp + 0x14]
        test   byte ptr [eax + 3], 0x10
        je     loc_1_34D2
        add    dword ptr [esp + 0x10], 4
loc_1_34D2:
        mov    eax, dword ptr [esp + 0x14]
        test   byte ptr [eax + 8], 2
        je     loc_1_34E6
        mov    eax, dword ptr [esp + 0x10]
        add    eax, eax
        mov    dword ptr [esp + 0x10], eax
loc_1_34E6:
        push   0x10
        call   sub_1_3B66
        movzx  eax, al
        add    esp, 4
        test   edi, edi
        jne    loc_1_3513
        cmp    byte ptr [esp + 0x10], al
        je     loc_1_3513
        push   1
        push   0x158f9
        push   0x1596
        call   sub_1_1CB6
        add    esp, 0xc
        mov    edi, eax
loc_1_3513:
        mov    eax, dword ptr [esp + 0x14]
        test   byte ptr [eax + 3], 0x10
        je     loc_1_3528
        call   sub_1_4CBF
        test   edi, edi
        jne    loc_1_3528
        mov    edi, eax
loc_1_3528:
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
sub_1_3532:
        movzx  eax, word ptr [esp + 4]
        mov    ecx, dword ptr [esp + 8]
        sub    eax, 0x1100
        cmp    eax, 0x14
        mov    dword ptr [ecx], 0
        ja     loc_1_3559
        movzx  edx, byte ptr [eax + 0x404618] ; -> obj1:0x3618
        jmp    dword ptr [edx*4 + 0x4045e4]  ; -> obj1:0x35E4
loc_1_3559:
        xor    ax, ax
        jmp    loc_1_35E3
loc_1_3561:
        xor    ax, ax
        mov    ecx, dword ptr [esp + 0xc]
        mov    word ptr [ecx], 0x9c
        jmp    loc_1_35E3
loc_1_356F:
        xor    ax, ax
        mov    ecx, dword ptr [esp + 0xc]
        mov    word ptr [ecx], 0x4c
        jmp    loc_1_35E3
loc_1_357D:
        xor    ax, ax
        mov    ecx, dword ptr [esp + 0xc]
        mov    word ptr [ecx], 0x44
        jmp    loc_1_35E3
loc_1_358B:
        mov    ax, 0x8000
        mov    dword ptr [ecx], 5
        mov    ecx, dword ptr [esp + 0xc]
        mov    word ptr [ecx], 0x48
        jmp    loc_1_35E3
loc_1_35A0:
        mov    ax, 0x8000
        mov    dword ptr [ecx], 3
        mov    ecx, dword ptr [esp + 0xc]
        mov    word ptr [ecx], 0x48
        jmp    loc_1_35E3
loc_1_35B5:
        xor    ax, ax
        mov    dword ptr [ecx], 1
        mov    ecx, dword ptr [esp + 0xc]
        mov    word ptr [ecx], 0x44
        jmp    loc_1_35E3
loc_1_35C9:
        xor    ax, ax
        mov    ecx, dword ptr [esp + 0xc]
        mov    word ptr [ecx], 0x40
        jmp    loc_1_35E3
loc_1_35D7:
        xor    ax, ax
        mov    ecx, dword ptr [esp + 0xc]
        mov    word ptr [ecx], 0x30
loc_1_35E3:
        ret    
        ; ---- dispatch table (13 entries) ----
        dd  loc_1_3561
        dd  loc_1_35D7
        dd  loc_1_356F
        dd  loc_1_35D7
        dd  loc_1_357D
        dd  loc_1_358B
        dd  loc_1_35A0
        dd  loc_1_35B5
        dd  loc_1_35D7
        dd  loc_1_35C9
        dd  loc_1_35D7
        dd  loc_1_3561
        dd  loc_1_3559
        db  000h, 000h, 001h, 001h, 002h, 003h, 003h, 003h, 004h, 005h, 005h, 005h, 005h, 006h, 006h, 007h
        db  008h, 009h, 00Ah, 00Ch, 00Bh
sub_1_362D:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   ebx
        push   ecx
        push   edx
        push   edi
        push   esi
        push   ebp
        push   ds
        push   es
        push   fs
        push   gs
        push   ss
        call   sub_1_3405
        mov    dword ptr [0x40cf8c], esp     ; -> obj1:0xBF8C
        mov    esp, dword ptr [ebp + 0xc]
        call   sub_1_33EF
        push   dword ptr [ebp + 8]
        call   sub_1_4D4A
        add    esp, 4
        call   sub_1_3405
        mov    esp, dword ptr [0x40cf8c]     ; -> obj1:0xBF8C
        call   sub_1_33EF
        pop    ss
        pop    gs
        pop    fs
        pop    es
        pop    ds
        pop    ebp
        pop    esi
        pop    edi
        pop    edx
        pop    ecx
        pop    ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
loc_1_3680:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        cld    
        mov    dword ptr [ebp - 4], edx
        mov    eax, dword ptr [ebp - 4]
        push   dword ptr [eax + 8]
        call   sub_1_5368
        add    esp, 4
        test   al, al
        je     loc_1_374D
        mov    eax, dword ptr [ebp - 4]
        mov    esi, dword ptr [eax + 8]
        cmp    dword ptr [esi + 0x4a8], 0
        je     loc_1_374D
        mov    al, byte ptr [esi + 0x4b0]
        mov    ecx, dword ptr [ebp - 4]
        push   eax
        push   dword ptr [ecx + 4]
        call   sub_1_50E0
        add    esp, 8
        mov    dword ptr [esi + 0x4c0], eax
        mov    ecx, dword ptr [ebp - 4]
        push   dword ptr [ecx + 8]
        call   sub_1_505D
        add    esp, 4
        call   sub_1_0790
        mov    ecx, dword ptr [ebp - 4]
        mov    eax, dword ptr [ecx + 8]
        cmp    byte ptr [eax + 0x4b0], 0
        je     loc_1_3752
        mov    ebx, 1
        mov    byte ptr [eax + 0x4af], bl
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 8]
        mov    byte ptr [ecx + 0x4b0], 0
        mov    eax, dword ptr [ebp - 4]
        push   dword ptr [eax + 4]
        call   sub_1_5052
        add    esp, 4
        test   al, al
        je     loc_1_372C
        mov    eax, dword ptr [ebp - 4]
        push   dword ptr [eax + 4]
        call   sub_1_4DA6
        add    esp, 4
        mov    bl, al
loc_1_372C:
        test   bl, bl
        je     loc_1_3752
        push   dword ptr [ebp - 4]
        push   0x404ac2                      ; -> sub_1_3AC2
        push   0x400000
        push   dword ptr [0x40cf90]          ; -> obj1:0xBF90
        call   sub_1_079C
        add    esp, 0x10
        jmp    loc_1_3752
loc_1_374D:
        call   sub_1_0790
loc_1_3752:
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_3758:
        push   esi
        push   edi
        mov    edi, dword ptr [0x40d014]     ; -> obj1:0xC014
        mov    esi, dword ptr [esp + 0xc]
        cmp    byte ptr [esi + 0x1d], 0
        je     loc_1_378D
        cmp    byte ptr [esi + 0x22], 0x22
        je     loc_1_3793
        mov    ax, word ptr [esi]
        push   8
        push   eax
        call   sub_1_3276
        mov    ax, word ptr [esi + 2]
        add    esp, 8
        push   8
        push   eax
        call   sub_1_3276
        add    esp, 8
loc_1_378D:
        cmp    byte ptr [esi + 0x22], 0x22
        jne    loc_1_37D6
loc_1_3793:
        mov    ax, word ptr [esi + 0x16]
        add    ax, 2
        push   eax
        call   sub_1_3281
        add    esp, 4
        and    al, 0x10
        shr    al, 4
        mov    byte ptr [edi + 0x20], al
        mov    al, byte ptr [edi + 4]
        or     al, 0xf
        push   eax
        push   0x19
        call   sub_1_1D05
        add    esp, 8
        mov    al, byte ptr [edi + 2]
        or     al, 0x10
        push   eax
        mov    byte ptr [edi + 2], al
        mov    ax, word ptr [esi + 0x16]
        add    ax, 2
        push   eax
        call   sub_1_3276
        add    esp, 8
loc_1_37D6:
        cmp    byte ptr [esi + 0x1d], 0
        je     loc_1_380B
        cmp    byte ptr [esi + 0x1f], 0
        je     loc_1_37F7
        mov    ax, word ptr [esi + 0x16]
        push   4
        add    ax, 0x12
        push   eax
        call   sub_1_54A6
        add    esp, 8
        jmp    loc_1_380B
loc_1_37F7:
        mov    ax, word ptr [esi + 0x16]
        push   0xff
        push   1
        push   eax
        call   sub_1_53D1
        add    esp, 0xc
loc_1_380B:
        pop    edi
        pop    esi
        ret    
sub_1_380E:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        call   sub_1_384C
        add    esp, 4
        test   al, al
        mov    eax, dword ptr [esp + 0xc]
        je     loc_1_3838
        mov    word ptr [esi + 0x4cc], ax
        mov    ecx, dword ptr [esp + 0x10]
        mov    word ptr [esi + 0x4ce], cx
        jmp    loc_1_384A
loc_1_3838:
        mov    word ptr [esi + 0x4c4], ax
        mov    ecx, dword ptr [esp + 0x10]
        mov    word ptr [esi + 0x4c6], cx
loc_1_384A:
        pop    esi
        ret    
sub_1_384C:
        mov    eax, dword ptr [esp + 4]
        mov    al, byte ptr [eax + 0x4e6]
        sub    al, 0x22
        cmp    al, 1
        sbb    al, al
        neg    al
        ret    
sub_1_385F:
        mov    ecx, dword ptr [esp + 4]
        push   esi
        xor    edx, edx
        xor    esi, esi
        mov    dword ptr [ecx], edx
        mov    dword ptr [ecx + 4], edx
loc_1_386D:
        movzx  eax, dl
        imul   eax, eax, 0xc
        add    eax, ecx
        inc    dl
        cmp    dl, 0x18
        mov    dword ptr [eax + 8], esi
        mov    dword ptr [eax + 0xc], esi
        mov    dword ptr [eax + 0x10], esi
        jb     loc_1_386D
        pop    esi
        ret    
sub_1_3887:
        mov    ax, 0x124
        ret    
sub_1_388C:
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        mov    eax, dword ptr [esi + 8]
        cmp    byte ptr [eax + 0x4a1], 0
        je     loc_1_38FE
        cmp    byte ptr [eax + 0x4e8], 6
        je     loc_1_38BF
        cmp    byte ptr [eax + 0x4e9], 2
        je     loc_1_38BF
        cmp    byte ptr [eax + 0x4ea], 2
        je     loc_1_38BF
        call   sub_1_2BE8
        jmp    loc_1_38CB
loc_1_38BF:
        push   dword ptr [esp + 8]
        call   sub_1_0834
        add    esp, 4
loc_1_38CB:
        mov    eax, dword ptr [esi + 8]
        add    eax, 0x4c4
        push   eax
        call   sub_1_54C5
        add    esp, 4
        mov    ecx, dword ptr [esi + 8]
        movzx  edx, byte ptr [ecx + 0x4e8]
        push   0x404ac2                      ; -> sub_1_3AC2
        push   edx
        call   sub_1_07FC
        add    esp, 8
        mov    ecx, dword ptr [esi + 8]
        mov    byte ptr [ecx + 0x4a1], 0
loc_1_38FE:
        pop    esi
        ret    
sub_1_3900:
        push   esi
        push   0
        call   sub_1_08B8
        mov    esi, dword ptr [esp + 0xc]
        add    esp, 4
        push   eax
        mov    dword ptr [esi], eax
        call   sub_1_08A0
        add    esp, 4
        push   dword ptr [esi]
        call   sub_1_088C
        add    esp, 4
        mov    dword ptr [esi], 0
        pop    esi
        ret    
sub_1_392C:
        mov    dx, word ptr [esp + 4]
        in     ax, dx
        ret    
sub_1_3934:
        push   esi
        xor    eax, eax
        mov    esi, dword ptr [0x40d014]     ; -> obj1:0xC014
        test   byte ptr [esi + 3], 0x10
        je     loc_1_3948
        call   sub_1_4CBF
loc_1_3948:
        test   byte ptr [esi + 8], 2
        mov    esi, dword ptr [esp + 8]
        je     loc_1_3954
        add    esi, esi
loc_1_3954:
        test   eax, eax
        jne    loc_1_398B
        push   esi
        push   0x10
        call   sub_1_1D05
        add    esp, 8
        mov    eax, esi
        shr    eax, 8
        shr    esi, 0x10
        push   eax
        push   0x11
        call   sub_1_1D05
        add    esp, 8
        push   esi
        push   0x12
        call   sub_1_1D05
        add    esp, 8
        push   0x1c
        call   sub_1_2C87
        add    esp, 4
loc_1_398B:
        pop    esi
        ret    
sub_1_398D:
        push   esi
        mov    esi, dword ptr [0x40d014]     ; -> obj1:0xC014
        call   sub_1_55CB
        test   byte ptr [esi + 2], 8
        jne    loc_1_39A5
        test   byte ptr [esi + 3], 0x10
        jne    loc_1_39B3
loc_1_39A5:
        push   dword ptr [esp + 0x10]
        push   3
        call   sub_1_558B
        add    esp, 8
loc_1_39B3:
        mov    ax, word ptr [esp + 0xc]
        test   ax, ax
        je     loc_1_39E6
        movzx  ecx, byte ptr [esi + 0x15]
        cmp    ecx, 1
        je     loc_1_39CD
        cmp    ecx, 2
        je     loc_1_39D9
        jmp    loc_1_39E6
loc_1_39CD:
        push   eax
        push   dword ptr [esp + 0xc]
        call   sub_1_0978
        jmp    loc_1_39E3
loc_1_39D9:
        push   eax
        push   dword ptr [esp + 0xc]
        call   sub_1_08D0
loc_1_39E3:
        add    esp, 8
loc_1_39E6:
        call   sub_1_553D
        push   0x1b
        call   sub_1_2C87
        add    esp, 4
        pop    esi
        ret    
sub_1_39F7:
        push   esi
        mov    esi, dword ptr [0x40d014]     ; -> obj1:0xC014
        test   byte ptr [esi + 2], 8
        jne    loc_1_3A0A
        test   byte ptr [esi + 3], 0x10
        je     loc_1_3A18
loc_1_3A0A:
        push   dword ptr [esp + 0x10]
        push   2
        call   sub_1_558B
        add    esp, 8
loc_1_3A18:
        mov    ax, word ptr [esp + 0xc]
        test   ax, ax
        je     loc_1_3A4B
        movzx  ecx, byte ptr [esi + 0x14]
        cmp    ecx, 1
        je     loc_1_3A32
        cmp    ecx, 2
        je     loc_1_3A3E
        jmp    loc_1_3A4B
loc_1_3A32:
        push   eax
        push   dword ptr [esp + 0xc]
        call   sub_1_0A98
        jmp    loc_1_3A48
loc_1_3A3E:
        push   eax
        push   dword ptr [esp + 0xc]
        call   sub_1_0A24
loc_1_3A48:
        add    esp, 8
loc_1_3A4B:
        push   0x1b
        call   sub_1_2C87
        add    esp, 4
        pop    esi
        ret    
sub_1_3A57:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        cld    
        mov    dword ptr [ebp - 4], edx
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x4a8
        cmp    dword ptr [eax], 0
        je     loc_1_3ABE
        cmp    dword ptr [0x40d020], 0       ; -> obj1:0xC020
        je     loc_1_3A8B
        mov    dword ptr [0x40d024], 1       ; -> obj1:0xC024
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [0x40d02c], eax     ; -> obj1:0xC02C
        jmp    loc_1_3ABE
loc_1_3A8B:
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [ebp - 4]
        add    eax, 0x4ae
        cmp    byte ptr [eax], 0
        jne    loc_1_3ABE
        push   1
        mov    byte ptr [eax], 1
        mov    eax, dword ptr [ebp - 4]
        push   dword ptr [eax + 0x4a4]
        call   sub_1_1F1A
        add    esp, 8
        mov    ecx, dword ptr [ebp - 4]
        mov    byte ptr [ecx + 0x4ae], 0
loc_1_3ABE:
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_3AC2:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        cld    
        mov    dword ptr [ebp - 4], edx
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 8]
        mov    dword ptr [ecx + 0x4a8], 0
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 8]
        mov    eax, dword ptr [ecx + 0x4bc]
        test   eax, eax
        je     loc_1_3B0F
        push   eax
        call   sub_1_0B14
        add    esp, 4
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 8]
        mov    dword ptr [ecx + 0x4bc], 0
        mov    dword ptr [0x40d024], 0       ; -> obj1:0xC024
loc_1_3B0F:
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 8]
        add    eax, 0x4ae
        cmp    byte ptr [eax], 0
        jne    loc_1_3B62
        cmp    dword ptr [0x40d020], 0       ; -> obj1:0xC020
        je     loc_1_3B3C
        mov    dword ptr [0x40d028], 1       ; -> obj1:0xC028
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [0x40d02c], eax     ; -> obj1:0xC02C
        jmp    loc_1_3B62
loc_1_3B3C:
        push   0
        mov    byte ptr [eax], 1
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 8]
        push   dword ptr [ecx + 0x4a4]
        call   sub_1_1F1A
        add    esp, 8
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ecx + 8]
        mov    byte ptr [edx + 0x4ae], 0
loc_1_3B62:
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_3B66:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [0x40d014]     ; -> obj1:0xC014
        xor    bl, bl
        call   sub_1_55CB
        mov    di, word ptr [esp + 0x14]
        mov    byte ptr [esp + 0xf], al
        cmp    di, 0x19
        jne    loc_1_3B8D
        mov    bl, byte ptr [esi + 4]
        jmp    loc_1_3BD9
loc_1_3B8D:
        movzx  eax, byte ptr [esi + 0x12]
        test   eax, eax
        je     loc_1_3B9C
        cmp    eax, 1
        je     loc_1_3BA4
        jmp    loc_1_3BAF
loc_1_3B9C:
        push   edi
        call   sub_1_0BF8
        jmp    loc_1_3BAA
loc_1_3BA4:
        push   edi
        call   sub_1_0B28
loc_1_3BAA:
        add    esp, 4
        mov    bl, al
loc_1_3BAF:
        movzx  eax, di
        cmp    eax, 0x1b
        je     loc_1_3BBE
        cmp    eax, 0x1c
        je     loc_1_3BC4
        jmp    loc_1_3BD9
loc_1_3BBE:
        mov    byte ptr [esi + 6], 0
        jmp    loc_1_3BD9
loc_1_3BC4:
        mov    byte ptr [esi + 7], 0
        cmp    byte ptr [esp + 0xf], 1
        mov    al, 0
        adc    al, 0xff
        and    al, 0x19
        xor    al, bl
        xor    al, 0x9c
        mov    bl, al
loc_1_3BD9:
        call   sub_1_553D
        mov    al, bl
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_1_3BE7:
        mov    al, byte ptr [esp + 8]
        cmp    al, 6
        jbe    loc_1_3BF1
        mov    al, 6
loc_1_3BF1:
        movzx  ecx, al
        mov    eax, dword ptr [esp + 4]
        lea    edx, [ecx + eax + 0x105]
        mov    eax, dword ptr [esp + 0xc]
        mov    cl, byte ptr [edx]
        and    cl, 3
        mov    byte ptr [eax], cl
        mov    cl, byte ptr [edx]
        and    cl, 8
        mov    eax, dword ptr [esp + 0x10]
        shr    cl, 3
        mov    byte ptr [eax], cl
        mov    cl, byte ptr [edx]
        and    cl, 0x10
        mov    eax, dword ptr [esp + 0x14]
        shr    cl, 4
        mov    byte ptr [eax], cl
        mov    cl, byte ptr [edx]
        and    cl, 0x20
        mov    eax, dword ptr [esp + 0x18]
        shr    cl, 5
        mov    byte ptr [eax], cl
        ret    
sub_1_3C35:
        push   ebp
        mov    ecx, dword ptr [0x40d014]     ; -> obj1:0xC014
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        push   esi
        mov    dword ptr [ebp - 0x18], ecx
        push   edi
        xor    esi, esi
        cmp    byte ptr [eax + 0x3c], 0
        jne    loc_1_3CAE
        mov    al, byte ptr [eax + 0x10c]
        mov    edx, dword ptr [ebp - 0x18]
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [edx + 0xd], al
        mov    al, byte ptr [ecx + 0x10c]
        mov    byte ptr [edx + 0xc], al
        mov    al, byte ptr [ecx + 0x10c]
        mov    byte ptr [edx + 0xf], al
        mov    al, byte ptr [ecx + 0x10c]
        mov    byte ptr [edx + 0x10], al
        mov    al, byte ptr [ecx + 0x10c]
        mov    byte ptr [edx + 0xe], al
        mov    al, byte ptr [ecx + 0x10c]
        mov    byte ptr [edx + 0x11], al
        mov    al, byte ptr [ecx + 0x10e]
        mov    byte ptr [edx + 0xa], al
        mov    al, byte ptr [ecx + 0x10e]
        mov    byte ptr [edx + 9], al
        mov    al, byte ptr [ecx + 0x10d]
        mov    byte ptr [edx + 0xb], al
        jmp    loc_1_409D
loc_1_3CAE:
        cmp    byte ptr [ebp + 0xc], 0
        jne    loc_1_3E42
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 2], 0
        mov    al, byte ptr [ecx + 0x10f]
        mov    byte ptr [ebp - 8], al
        test   al, al
        jne    loc_1_3CCF
        inc    byte ptr [ebp - 8]
loc_1_3CCF:
        push   0x19
        call   sub_1_3B66
        add    esp, 4
        mov    byte ptr [ebp - 9], al
        and    al, 0xf7
        push   eax
        push   0x19
        call   sub_1_1D05
        add    esp, 8
        call   sub_1_55CB
loc_1_3CEE:
        xor    bx, bx
loc_1_3CF1:
        test   esi, esi
        jne    loc_1_3DC4
        mov    al, bl
        or     al, 0xf0
        mov    byte ptr [ebp - 0x14], al
        not    al
        push   eax
        push   2
        call   sub_1_1D05
        add    esp, 8
        push   dword ptr [ebp - 0x14]
        push   2
        call   sub_1_1D05
        add    esp, 8
        push   2
        call   sub_1_3B66
        add    esp, 4
        mov    byte ptr [ebp - 1], al
        mov    al, bl
        and    al, 0xf
        mov    byte ptr [ebp - 0x10], al
        not    al
        push   eax
        push   2
        call   sub_1_1D05
        add    esp, 8
        push   dword ptr [ebp - 0x10]
        push   2
        call   sub_1_1D05
        add    esp, 8
        push   2
        call   sub_1_3B66
        add    esp, 4
        mov    cl, byte ptr [ebp - 0x14]
        cmp    byte ptr [ebp - 1], cl
        jne    loc_1_3D7D
        cmp    al, byte ptr [ebp - 0x10]
        jne    loc_1_3D7D
        push   0x1c
        call   sub_1_2C87
        add    esp, 4
        mov    esi, eax
        test   esi, esi
        je     loc_1_3D93
        push   2
        push   0x15934
        push   0x1597
        jmp    loc_1_3D89
loc_1_3D7D:
        push   1
        push   0x15934
        push   0x1595
loc_1_3D89:
        call   sub_1_1CB6
        add    esp, 0xc
        mov    esi, eax
loc_1_3D93:
        inc    bx
        cmp    bx, 0x10
        jb     loc_1_3CF1
        test   esi, esi
        jne    loc_1_3DC4
        mov    ecx, dword ptr [ebp - 0x18]
        mov    al, byte ptr [ecx + 0xd]
        cmp    byte ptr [ebp - 8], al
        jae    loc_1_3DBE
        dec    al
        mov    byte ptr [ecx + 0xd], al
        mov    byte ptr [ecx + 0xc], al
        mov    al, byte ptr [ecx + 0xd]
        mov    byte ptr [ecx + 0x10], al
        jmp    loc_1_3DF4
loc_1_3DBE:
        mov    byte ptr [ebp - 2], 1
        jmp    loc_1_3DF4
loc_1_3DC4:
        mov    ecx, dword ptr [ebp - 0x18]
        mov    al, byte ptr [ecx + 0xd]
        cmp    al, 5
        jne    loc_1_3DE6
        push   3
        push   0x15934
        push   0x1595
        call   sub_1_1CB6
        add    esp, 0xc
        mov    esi, eax
        jmp    loc_1_3DF4
loc_1_3DE6:
        inc    al
        mov    ecx, dword ptr [ebp - 0x18]
        mov    byte ptr [ebp - 2], 1
        xor    esi, esi
        mov    byte ptr [ecx + 0xd], al
loc_1_3DF4:
        test   esi, esi
        jne    loc_1_3E02
        cmp    byte ptr [ebp - 2], 0
        je     loc_1_3CEE
loc_1_3E02:
        push   0
        push   2
        call   sub_1_1D05
        add    esp, 8
        push   0xa
        call   sub_1_0720
        add    esp, 4
        push   4
        push   2
        call   sub_1_1D05
        add    esp, 8
        push   1
        push   0x1a
        call   sub_1_1D05
        add    esp, 8
        push   dword ptr [ebp - 9]
        push   0x19
        call   sub_1_1D05
        add    esp, 8
        call   sub_1_553D
loc_1_3E42:
        test   esi, esi
        jne    loc_1_409D
        cmp    byte ptr [ebp + 0xc], 0
        jne    loc_1_403A
        mov    byte ptr [ebp - 2], 0
        mov    ecx, dword ptr [ebp - 0x18]
        mov    byte ptr [ebp - 1], 5
        test   byte ptr [ecx + 3], 4
        je     loc_1_3E6A
        add    ecx, 0xa
        jmp    loc_1_3E70
loc_1_3E6A:
        mov    ecx, dword ptr [ebp - 0x18]
        add    ecx, 9
loc_1_3E70:
        mov    dword ptr [ebp - 8], ecx
loc_1_3E73:
        mov    eax, dword ptr [ebp - 8]
        mov    dl, byte ptr [ebp - 1]
        mov    edi, dword ptr [ebp + 0x10]
        xor    bl, bl
        mov    byte ptr [eax], dl
loc_1_3E80:
        test   esi, esi
        jne    loc_1_3F77
        push   0
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ecx + 0x112]
        push   2
        dec    ax
        push   eax
        push   0
        push   edi
        call   sub_1_3434
        add    esp, 0x14
        test   eax, eax
        je     loc_1_3EC8
        push   4
        push   0x15934
        push   eax
        call   sub_1_247A
        add    esp, 4
        push   eax
        call   sub_1_1CB6
        add    esp, 0xc
        mov    esi, eax
        jmp    loc_1_3F49
loc_1_3EC8:
        push   0
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ecx + 0x112]
        push   3
        dec    ax
        push   eax
        push   0
        push   edi
        call   sub_1_3434
        add    esp, 0x14
        test   eax, eax
        je     loc_1_3F05
        push   5
        push   0x15934
        push   eax
        call   sub_1_247A
        add    esp, 4
        push   eax
        call   sub_1_1CB6
        add    esp, 0xc
        mov    esi, eax
        jmp    loc_1_3F49
loc_1_3F05:
        push   ebx
        mov    ecx, dword ptr [ebp + 8]
        mov    ax, word ptr [ecx + 0x112]
        dec    ax
        push   eax
        push   edi
        push   1
        call   sub_1_423F
        add    esp, 0x10
        cmp    al, 1
        jne    loc_1_3F49
        push   6
        push   0x15934
        push   0x1596
        call   sub_1_1CB6
        add    esp, 0xc
        mov    esi, eax
        push   ebx
        push   0x2000
        push   edi
        push   0
        call   sub_1_423F
        add    esp, 0x10
loc_1_3F49:
        add    edi, 0x2000
        inc    bl
        cmp    bl, 4
        jb     loc_1_3E80
        test   esi, esi
        jne    loc_1_3F77
        mov    ecx, dword ptr [ebp + 8]
        mov    dl, byte ptr [ebp - 1]
        cmp    byte ptr [ecx + 0x110], dl
        jae    loc_1_3F71
        dec    byte ptr [ebp - 1]
        jmp    loc_1_3FC4
loc_1_3F71:
        mov    byte ptr [ebp - 2], 1
        jmp    loc_1_3FC4
loc_1_3F77:
        push   esi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 0x1596
        je     loc_1_3F95
        push   esi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 0x1597
        jne    loc_1_3FC4
loc_1_3F95:
        cmp    byte ptr [ebp - 1], 5
        jae    loc_1_3FAE
        mov    eax, dword ptr [ebp - 8]
        xor    esi, esi
        mov    byte ptr [ebp - 2], 1
        inc    byte ptr [ebp - 1]
        mov    dl, byte ptr [ebp - 1]
        mov    byte ptr [eax], dl
        jmp    loc_1_3FC4
loc_1_3FAE:
        push   8
        push   0x15934
        push   0x1595
        call   sub_1_1CB6
        add    esp, 0xc
        mov    esi, eax
loc_1_3FC4:
        test   esi, esi
        jne    loc_1_3FF7
        cmp    byte ptr [ebp - 2], 0
        je     loc_1_3E73
        test   esi, esi
        jne    loc_1_4005
        mov    eax, dword ptr [ebp - 0x18]
        sub    eax, dword ptr [ebp - 8]
        cmp    eax, -9
        je     loc_1_3FE6
        cmp    eax, -0xa
        jne    loc_1_3FEA
loc_1_3FE6:
        mov    byte ptr [ebp - 2], 0
loc_1_3FEA:
        mov    byte ptr [ebp - 1], 5
        mov    ecx, dword ptr [ebp - 0x18]
        add    ecx, 0xb
        mov    dword ptr [ebp - 8], ecx
loc_1_3FF7:
        test   esi, esi
        jne    loc_1_403A
        cmp    byte ptr [ebp - 2], 0
        je     loc_1_3E73
loc_1_4005:
        test   esi, esi
        jne    loc_1_409D
        mov    ecx, dword ptr [ebp - 0x18]
        test   byte ptr [ecx + 3], 0x10
        je     loc_1_403A
        call   sub_1_4CBF
        test   eax, eax
        je     loc_1_403A
        push   9
        push   0x15934
        push   eax
        call   sub_1_247A
        add    esp, 4
        push   eax
        call   sub_1_1CB6
        add    esp, 0xc
        mov    esi, eax
loc_1_403A:
        test   esi, esi
        jne    loc_1_409D
        mov    ecx, dword ptr [ebp - 0x18]
        mov    al, byte ptr [ecx + 0xd]
        mov    byte ptr [ecx + 0xc], al
        mov    al, byte ptr [ecx + 0xd]
        mov    byte ptr [ecx + 0xf], al
        mov    al, byte ptr [ecx + 0xd]
        mov    byte ptr [ecx + 0x10], al
        mov    al, byte ptr [ecx + 0xd]
        mov    byte ptr [ecx + 0xe], al
        mov    al, byte ptr [ecx + 0xd]
        mov    byte ptr [ecx + 0x11], al
        test   byte ptr [ecx + 3], 4
        je     loc_1_406F
        lea    eax, [ecx + 9]
        mov    cl, byte ptr [ecx + 0xa]
        mov    byte ptr [eax], cl
        jmp    loc_1_407D
loc_1_406F:
        mov    eax, dword ptr [ebp - 0x18]
        mov    ecx, dword ptr [ebp - 0x18]
        add    eax, 9
        mov    dl, byte ptr [eax]
        mov    byte ptr [ecx + 0xa], dl
loc_1_407D:
        mov    ecx, dword ptr [ebp - 0x18]
        mov    ebx, dword ptr [ebp + 8]
        mov    dl, byte ptr [ecx + 0xc]
        mov    byte ptr [ebx + 0x10c], dl
        mov    dl, byte ptr [ecx + 0xb]
        mov    byte ptr [ebx + 0x10d], dl
        mov    al, byte ptr [eax]
        mov    byte ptr [ebx + 0x10e], al
loc_1_409D:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_40A6:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        mov    eax, 2
        push   ebx
        push   esi
        xor    ebx, ebx
        push   edi
        mov    byte ptr [ebp - 5], bl
        mov    edi, dword ptr [0x40d014]     ; -> obj1:0xC014
        mov    cl, byte ptr [ebp + 0x10]
        mov    byte ptr [ebp - 8], al
        mov    byte ptr [ebp - 7], al
        mov    dl, byte ptr [edi + 0x13]
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 4], bl
        mov    byte ptr [ebp - 1], cl
        mov    byte ptr [ebp - 6], dl
loc_1_40D7:
        lea    eax, [ebp - 9]
        lea    ecx, [ebp - 0xa]
        push   eax
        lea    edx, [ebp - 3]
        push   ecx
        lea    eax, [ebp - 2]
        push   edx
        push   eax
        push   dword ptr [ebp - 1]
        push   esi
        call   sub_1_3BE7
        add    esp, 0x18
        mov    al, byte ptr [edi + 3]
        cmp    byte ptr [ebp - 3], 0
        jne    loc_1_4116
        and    al, 0xf8
        or     al, byte ptr [ebp - 2]
        push   eax
        push   0x18
        call   sub_1_1D05
        mov    byte ptr [edi + 0x12], 1
        mov    byte ptr [edi + 0x13], 1
        add    esp, 8
        jmp    loc_1_4133
loc_1_4116:
        and    al, 0xfc
        mov    cl, byte ptr [ebp - 2]
        add    cl, 4
        or     al, cl
        push   eax
        push   0x18
        call   sub_1_1D05
        mov    byte ptr [edi + 0x12], 0
        mov    byte ptr [edi + 0x13], 0
        add    esp, 8
loc_1_4133:
        push   0x18
        call   sub_1_3B66
        add    esp, 4
        cmp    byte ptr [edi + 3], al
        je     loc_1_41D0
        movzx  eax, byte ptr [ebp - 5]
        test   eax, eax
        je     loc_1_4170
        cmp    eax, 1
        je     loc_1_4197
        push   1
        cmp    eax, 2
        je     loc_1_41A5
        push   0x15933
        push   0x1593
        call   sub_1_1CB6
        add    esp, 0xc
        mov    ebx, eax
        jmp    loc_1_41CB
loc_1_4170:
        push   1
        push   esi
        call   sub_1_2F18
        add    esp, 8
        mov    ebx, eax
        call   sub_1_4656
        mov    al, byte ptr [edi + 8]
        or     al, 2
        push   eax
        push   0x1e
        call   sub_1_1D05
        add    esp, 8
        mov    al, byte ptr [ebp + 0x10]
        jmp    loc_1_41C8
loc_1_4197:
        mov    al, byte ptr [ebp - 6]
        mov    cl, byte ptr [ebp + 0xc]
        mov    byte ptr [edi + 0x13], al
        mov    byte ptr [ebp - 1], cl
        jmp    loc_1_41CB
loc_1_41A5:
        push   esi
        call   sub_1_2F18
        add    esp, 8
        mov    ebx, eax
        call   sub_1_4656
        mov    al, byte ptr [edi + 8]
        or     al, 2
        push   eax
        push   0x1e
        call   sub_1_1D05
        add    esp, 8
        mov    al, byte ptr [ebp + 0xc]
loc_1_41C8:
        mov    byte ptr [ebp - 1], al
loc_1_41CB:
        inc    byte ptr [ebp - 5]
        jmp    loc_1_41F4
loc_1_41D0:
        mov    al, byte ptr [ebp - 1]
        cmp    al, byte ptr [ebp + 0xc]
        jne    loc_1_41F0
        push   1
        push   0x15933
        push   0x1592
        call   sub_1_1CB6
        add    esp, 0xc
        mov    ebx, eax
        jmp    loc_1_41F4
loc_1_41F0:
        mov    byte ptr [ebp - 4], 1
loc_1_41F4:
        test   ebx, ebx
        jne    loc_1_4236
        cmp    byte ptr [ebp - 4], 0
        je     loc_1_40D7
        test   ebx, ebx
        jne    loc_1_4236
        movzx  eax, byte ptr [ebp - 2]
        test   eax, eax
        je     loc_1_4215
        cmp    eax, 2
        je     loc_1_421B
        jmp    loc_1_422A
loc_1_4215:
        mov    byte ptr [ebp - 7], 2
        jmp    loc_1_421F
loc_1_421B:
        mov    byte ptr [ebp - 7], 1
loc_1_421F:
        cmp    byte ptr [ebp - 3], 1
        sbb    al, al
        add    al, 2
        mov    byte ptr [ebp - 8], al
loc_1_422A:
        mov    al, byte ptr [ebp - 8]
        mov    cl, byte ptr [ebp - 7]
        mov    byte ptr [edi + 0x14], al
        mov    byte ptr [edi + 0x15], cl
loc_1_4236:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_423F:
        push   ebp
        xor    al, al
        mov    ebp, esp
        push   ebx
        push   esi
        xor    ebx, ebx
        push   edi
        mov    edx, dword ptr [ebp + 0xc]
        movzx  ecx, byte ptr [ebp + 0x14]
        cmp    byte ptr [ebp + 8], al
        jne    loc_1_430C
        test   ecx, ecx
        je     loc_1_4271
        cmp    ecx, 1
        je     loc_1_4291
        cmp    ecx, 2
        je     loc_1_42B1
        cmp    ecx, 3
        je     loc_1_42E8
        jmp    loc_1_442E
loc_1_4271:
        movzx  ecx, word ptr [ebp + 0x10]
        add    ecx, edx
        cmp    ecx, edx
        jbe    loc_1_442E
loc_1_427F:
        mov    byte ptr [edx], bl
        inc    bl
        inc    edx
        mov    byte ptr [edx], 0
        inc    edx
        cmp    ecx, edx
        ja     loc_1_427F
        jmp    loc_1_442E
loc_1_4291:
        movzx  ecx, word ptr [ebp + 0x10]
        add    ecx, edx
        cmp    ecx, edx
        jbe    loc_1_442E
loc_1_429F:
        mov    byte ptr [edx], bl
        inc    bl
        inc    edx
        mov    byte ptr [edx], 0xff
        inc    edx
        cmp    ecx, edx
        ja     loc_1_429F
        jmp    loc_1_442E
loc_1_42B1:
        movzx  ecx, word ptr [ebp + 0x10]
        add    ecx, edx
        cmp    ecx, edx
        jbe    loc_1_442E
loc_1_42BF:
        mov    byte ptr [edx], 0xf0
        inc    edx
        mov    byte ptr [edx], 0xf
        inc    edx
        mov    byte ptr [edx], 0xf0
        inc    edx
        mov    byte ptr [edx], 0xf
        inc    edx
        mov    byte ptr [edx], 0x55
        inc    edx
        mov    byte ptr [edx], 0xaa
        inc    edx
        mov    byte ptr [edx], 0x5a
        inc    edx
        mov    byte ptr [edx], 0xa5
        inc    edx
        cmp    ecx, edx
        ja     loc_1_42BF
        jmp    loc_1_442E
loc_1_42E8:
        movzx  ecx, word ptr [ebp + 0x10]
        lea    esi, [ecx + edx]
        cmp    esi, edx
        jbe    loc_1_442E
loc_1_42F7:
        mov    cl, bl
        mov    byte ptr [edx], bl
        inc    edx
        inc    bl
        not    cl
        mov    byte ptr [edx], cl
        inc    edx
        cmp    esi, edx
        ja     loc_1_42F7
        jmp    loc_1_442E
loc_1_430C:
        test   ecx, ecx
        je     loc_1_4328
        cmp    ecx, 1
        je     loc_1_435F
        cmp    ecx, 2
        je     loc_1_4396
        cmp    ecx, 3
        je     loc_1_4403
        jmp    loc_1_442E
loc_1_4328:
        movzx  ecx, word ptr [ebp + 0x10]
        lea    esi, [ecx + edx]
        cmp    esi, edx
        jbe    loc_1_442E
loc_1_4337:
        test   al, al
        jne    loc_1_442E
        mov    edi, edx
        mov    cl, bl
        inc    edx
        inc    bl
        cmp    byte ptr [edi], cl
        je     loc_1_434C
        mov    al, 1
loc_1_434C:
        mov    ecx, edx
        inc    edx
        cmp    byte ptr [ecx], 0
        je     loc_1_4356
        mov    al, 1
loc_1_4356:
        cmp    esi, edx
        ja     loc_1_4337
        jmp    loc_1_442E
loc_1_435F:
        movzx  ecx, word ptr [ebp + 0x10]
        lea    esi, [ecx + edx]
        cmp    esi, edx
        jbe    loc_1_442E
loc_1_436E:
        test   al, al
        jne    loc_1_442E
        mov    edi, edx
        mov    cl, bl
        inc    edx
        inc    bl
        cmp    byte ptr [edi], cl
        je     loc_1_4383
        mov    al, 1
loc_1_4383:
        mov    ecx, edx
        inc    edx
        cmp    byte ptr [ecx], 0xff
        je     loc_1_438D
        mov    al, 1
loc_1_438D:
        cmp    esi, edx
        ja     loc_1_436E
        jmp    loc_1_442E
loc_1_4396:
        movzx  ecx, word ptr [ebp + 0x10]
        lea    esi, [ecx + edx]
        cmp    esi, edx
        jbe    loc_1_442E
loc_1_43A5:
        test   al, al
        jne    loc_1_442E
        mov    ecx, edx
        inc    edx
        cmp    byte ptr [ecx], 0xf0
        je     loc_1_43B7
        mov    al, 1
loc_1_43B7:
        mov    ecx, edx
        inc    edx
        cmp    byte ptr [ecx], 0xf
        je     loc_1_43C1
        mov    al, 1
loc_1_43C1:
        mov    ecx, edx
        inc    edx
        cmp    byte ptr [ecx], 0xf0
        je     loc_1_43CB
        mov    al, 1
loc_1_43CB:
        mov    ecx, edx
        inc    edx
        cmp    byte ptr [ecx], 0xf
        je     loc_1_43D5
        mov    al, 1
loc_1_43D5:
        mov    ecx, edx
        inc    edx
        cmp    byte ptr [ecx], 0x55
        je     loc_1_43DF
        mov    al, 1
loc_1_43DF:
        mov    ecx, edx
        inc    edx
        cmp    byte ptr [ecx], 0xaa
        je     loc_1_43E9
        mov    al, 1
loc_1_43E9:
        mov    ecx, edx
        inc    edx
        cmp    byte ptr [ecx], 0x5a
        je     loc_1_43F3
        mov    al, 1
loc_1_43F3:
        mov    ecx, edx
        inc    edx
        cmp    byte ptr [ecx], 0xa5
        je     loc_1_43FD
        mov    al, 1
loc_1_43FD:
        cmp    esi, edx
        ja     loc_1_43A5
        jmp    loc_1_442E
loc_1_4403:
        movzx  ecx, word ptr [ebp + 0x10]
        lea    esi, [ecx + edx]
        cmp    esi, edx
        jbe    loc_1_442E
loc_1_440E:
        test   al, al
        jne    loc_1_442E
        mov    ecx, edx
        inc    edx
        cmp    byte ptr [ecx], bl
        je     loc_1_441B
        mov    al, 1
loc_1_441B:
        mov    edi, edx
        mov    cl, bl
        inc    edx
        inc    bl
        not    cl
        cmp    cl, byte ptr [edi]
        je     loc_1_442A
        mov    al, 1
loc_1_442A:
        cmp    esi, edx
        ja     loc_1_440E
loc_1_442E:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
sub_1_4433:
        mov    cl, byte ptr [esp + 0xc]
        sub    esp, 4
        test   cl, cl
        push   ebx
        push   esi
        je     loc_1_4442
        mov    cl, 8
loc_1_4442:
        mov    al, byte ptr [esp + 0x1c]
        test   al, al
        je     loc_1_444C
        mov    al, 0x10
loc_1_444C:
        mov    bl, byte ptr [esp + 0x20]
        test   bl, bl
        je     loc_1_4456
        mov    bl, 0x20
loc_1_4456:
        xor    dl, dl
        add    bl, al
        add    bl, cl
        mov    al, byte ptr [esp + 0x14]
        add    bl, al
        mov    ecx, dword ptr [esp + 0x10]
        mov    al, byte ptr [esp + 0xb]
loc_1_446A:
        movzx  esi, dl
        cmp    byte ptr [esi + ecx + 0x105], bl
        jne    loc_1_4478
        mov    al, dl
loc_1_4478:
        inc    dl
        cmp    dl, 6
        jbe    loc_1_446A
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_1_4485:
        push   esi
        call   sub_1_4656
        mov    esi, dword ptr [esp + 8]
        push   esi
        call   sub_1_5768
        add    esp, 4
        test   eax, eax
        jne    loc_1_44A5
        push   esi
        call   sub_1_564A
        add    esp, 4
loc_1_44A5:
        pop    esi
        ret    
sub_1_44A7:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        xor    cl, cl
        push   ebx
        push   esi
        xor    ebx, ebx
        push   edi
        mov    esi, dword ptr [0x40d014]     ; -> obj1:0xC014
loc_1_44BA:
        movzx  eax, cl
        mov    byte ptr [ebp + eax - 8], cl
        inc    cl
        cmp    cl, 5
        jb     loc_1_44BA
        mov    al, byte ptr [esi + 4]
        or     al, 0xc
        push   eax
        push   0x19
        call   sub_1_1D05
        add    esp, 8
        lea    ecx, [ebp - 8]
        push   0
        push   5
        push   ecx
        call   sub_1_39F7
        add    esp, 0xc
        mov    edi, eax
        test   edi, edi
        jne    loc_1_462B
        movzx  eax, word ptr [esi + 0x18]
        inc    eax
        push   eax
        call   sub_1_3281
        add    esp, 4
        test   al, 0x40
        jne    loc_1_451F
        push   1
        push   0x15940
        push   0x1594
        call   sub_1_1CB6
        add    esp, 0xc
        mov    ebx, eax
        jmp    loc_1_4627
loc_1_451F:
        movzx  eax, word ptr [esi + 0x18]
        inc    eax
        push   eax
        call   sub_1_3281
        add    esp, 4
        test   al, 8
        jne    loc_1_454C
        push   2
        push   0x15940
        push   0x1594
        call   sub_1_1CB6
        add    esp, 0xc
        mov    ebx, eax
        jmp    loc_1_4627
loc_1_454C:
        push   0
        lea    eax, [ebp - 8]
        push   5
        push   eax
        call   sub_1_398D
        add    esp, 0xc
        mov    edi, eax
        test   edi, edi
        jne    loc_1_462B
        push   4
        push   0x1a
        call   sub_1_1D05
        add    esp, 8
        push   0x1a
        call   sub_1_3B66
        add    esp, 4
        test   al, 4
        je     loc_1_459B
        push   3
        push   0x15940
        push   0x1594
        call   sub_1_1CB6
        add    esp, 0xc
        mov    ebx, eax
        jmp    loc_1_4627
loc_1_459B:
        push   0
        lea    eax, [ebp - 8]
        push   5
        push   eax
        call   sub_1_398D
        add    esp, 0xc
        mov    edi, eax
        test   edi, edi
        jne    loc_1_462B
        movzx  eax, word ptr [esi + 0x18]
        inc    eax
        push   eax
        call   sub_1_3281
        add    esp, 4
        test   al, 0x40
        jne    loc_1_45DB
        push   4
        push   0x15940
        push   0x1594
        call   sub_1_1CB6
        add    esp, 0xc
        mov    ebx, eax
        jmp    loc_1_4627
loc_1_45DB:
        movzx  eax, word ptr [esi + 0x18]
        inc    eax
        push   eax
        call   sub_1_3281
        add    esp, 4
        test   al, 8
        jne    loc_1_4605
        push   5
        push   0x15940
        push   0x1594
        call   sub_1_1CB6
        add    esp, 0xc
        mov    ebx, eax
        jmp    loc_1_4627
loc_1_4605:
        push   0
        lea    eax, [ebp - 8]
        push   5
        push   eax
        call   sub_1_39F7
        add    esp, 0xc
        mov    edi, eax
        test   edi, edi
        jne    loc_1_462B
        push   4
        push   0x1a
        call   sub_1_1D05
        add    esp, 8
loc_1_4627:
        test   edi, edi
        je     loc_1_463D
loc_1_462B:
        push   1
        push   0x15940
        push   edi
        call   sub_1_1CB6
        add    esp, 0xc
        mov    ebx, eax
loc_1_463D:
        mov    al, byte ptr [esi + 4]
        and    al, 0xf3
        push   eax
        push   0x19
        call   sub_1_1D05
        add    esp, 8
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_4656:
        mov    eax, dword ptr [0x40d014]     ; -> obj1:0xC014
        mov    al, byte ptr [eax + 8]
        or     al, 3
        push   eax
        push   0x1e
        call   sub_1_1D05
        add    esp, 8
        push   0x1388
        call   sub_1_0720
        add    esp, 4
        ret    
sub_1_4679:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        mov    ax, word ptr [eax + 0x16]
        push   esi
        mov    word ptr [ebp - 6], ax
        push   edi
        mov    dword ptr [ebp - 0xc], 0
        movzx  eax, ax
        add    ax, 2
        mov    esi, dword ptr [0x40d014]     ; -> obj1:0xC014
        mov    word ptr [ebp - 4], ax
        push   dword ptr [ebp - 4]
        call   sub_1_3281
        add    esp, 4
        mov    bl, al
        mov    byte ptr [esi + 2], 4
        push   4
        push   dword ptr [ebp - 4]
        call   sub_1_3276
        add    esp, 8
        test   bl, 4
        jne    loc_1_46D5
        push   0x1388
        call   sub_1_0720
        add    esp, 4
loc_1_46D5:
        mov    eax, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 1], 0
        mov    al, byte ptr [eax + 0x10]
        movzx  edi, al
        imul   edi, edi, 0x19
loc_1_46E5:
        movzx  eax, byte ptr [ebp - 1]
        add    eax, dword ptr [ebp + 8]
        mov    bl, byte ptr [eax + edi + 0x3d]
        mov    al, bl
        and    bl, 0xe
        or     al, 0xf
        push   eax
        mov    byte ptr [esi], al
        push   dword ptr [ebp - 6]
        call   sub_1_3276
        add    esp, 8
        mov    byte ptr [esi + 2], bl
        push   ebx
        push   dword ptr [ebp - 4]
        call   sub_1_3276
        add    esp, 8
        push   0xa
        call   sub_1_0720
        add    esp, 4
        inc    byte ptr [ebp - 1]
        cmp    byte ptr [ebp - 1], 0x19
        jb     loc_1_46E5
        or     byte ptr [esi + 4], 8
        push   0x18
        call   sub_1_3B66
        add    esp, 4
        and    al, 0xf
        cmp    al, 4
        jne    loc_1_4799
        push   0x1e
        call   sub_1_3B66
        mov    byte ptr [esi + 8], al
        add    esp, 4
        mov    bl, al
        mov    al, bl
        xor    al, 2
        push   eax
        push   0x1e
        call   sub_1_1D05
        add    esp, 8
        push   0x1e
        call   sub_1_3B66
        add    esp, 4
        cmp    byte ptr [esi + 8], al
        jne    loc_1_4795
        push   4
        or     bl, 2
        push   0x18
        call   sub_1_1D05
        add    esp, 8
        push   ebx
        push   0x1e
        call   sub_1_1D05
        add    esp, 8
        push   0
        push   0x19
        call   sub_1_1D05
        add    esp, 8
        call   sub_1_590F
        jmp    loc_1_47B0
loc_1_4795:
        push   1
        jmp    loc_1_479B
loc_1_4799:
        push   2
loc_1_479B:
        push   0x15923
        push   0x1598
        call   sub_1_1CB6
        add    esp, 0xc
        mov    dword ptr [ebp - 0xc], eax
loc_1_47B0:
        mov    eax, dword ptr [ebp - 0xc]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_47BA:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [0x40d014]     ; -> obj1:0xC014
        push   ebp
        xor    ebx, ebx
        xor    edi, edi
        mov    ebp, dword ptr [esp + 0x1c]
        mov    byte ptr [esp + 0x13], bl
        push   ebp
        mov    byte ptr [ebp + 0xe], bl
        mov    byte ptr [esi + 0x22], bl
        call   sub_1_4B6B
        add    esp, 4
        test   al, al
        jne    loc_1_47F8
        push   ebp
        call   sub_1_4B68
        add    esp, 4
        test   al, al
        jne    loc_1_47F8
        call   sub_1_4B4E
loc_1_47F8:
        call   sub_1_3288
        mov    al, byte ptr [esi + 3]
        and    al, 0xfe
        or     al, 2
        push   eax
        push   0x18
        call   sub_1_1D05
        add    esp, 8
        xor    al, al
loc_1_4811:
        movzx  ecx, al
        mov    byte ptr [esp + ecx + 0x14], al
        inc    al
        cmp    al, 4
        jb     loc_1_4811
        push   0
        push   2
        call   sub_1_558B
        lea    eax, [esp + 0x1c]
        add    esp, 8
        mov    byte ptr [esi + 0x14], 2
        push   0
        push   4
        push   eax
        call   sub_1_39F7
        add    esp, 0xc
        test   eax, eax
        je     loc_1_4857
        push   1
        push   0x15922
        push   eax
        call   sub_1_1CB6
        add    esp, 0xc
        mov    edi, eax
        jmp    loc_1_48C4
loc_1_4857:
        push   0
        push   3
        call   sub_1_558B
        lea    eax, [esp + 0x1c]
        add    esp, 8
        mov    byte ptr [esi + 0x15], 1
        push   0
        push   4
        push   eax
        call   sub_1_398D
        add    esp, 0xc
        test   eax, eax
        je     loc_1_4890
        push   2
        push   0x15922
        push   eax
        call   sub_1_1CB6
        add    esp, 0xc
        mov    edi, eax
        jmp    loc_1_48C4
loc_1_4890:
        xor    al, al
loc_1_4892:
        movzx  edx, al
        mov    cl, byte ptr [esp + 0x14]
        add    cl, al
        and    cl, 3
        cmp    byte ptr [esp + edx + 0x14], cl
        je     loc_1_48A6
        mov    bl, 1
loc_1_48A6:
        inc    al
        cmp    al, 4
        jb     loc_1_4892
        cmp    bl, 1
        jne    loc_1_48BF
        cmp    byte ptr [esi + 0x22], 0
        jne    loc_1_48BF
        mov    byte ptr [esi + 0x22], 4
        xor    bl, bl
        jmp    loc_1_48C4
loc_1_48BF:
        mov    byte ptr [esp + 0x13], 1
loc_1_48C4:
        cmp    byte ptr [esp + 0x13], 0
        jne    loc_1_48D3
        test   edi, edi
        je     loc_1_47F8
loc_1_48D3:
        test   edi, edi
        jne    loc_1_48F0
        test   bl, bl
        jne    loc_1_48DF
        mov    byte ptr [ebp + 0xe], 2
loc_1_48DF:
        mov    byte ptr [esi + 0x15], 2
        push   1
        push   ebp
        call   sub_1_2F18
        add    esp, 8
        mov    edi, eax
loc_1_48F0:
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
sub_1_48FA:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        lea    eax, [ebp - 0xc]
        push   esi
        mov    ebx, dword ptr [0x40d014]     ; -> obj1:0xC014
        push   edi
        push   eax
        xor    edi, edi
        call   sub_1_5ADD
        add    esp, 4
        mov    esi, dword ptr [ebp + 8]
        add    esi, 0x24
        lea    eax, [ebp - 1]
        push   eax
        lea    ecx, [ebp - 0xc]
        push   esi
        push   ecx
        call   sub_1_5A69
        add    esp, 0xc
        cmp    byte ptr [esi], 0xff
        jne    loc_1_4963
        lea    eax, [ebp - 1]
        push   eax
        push   7
        push   dword ptr [0x40cf90]          ; -> obj1:0xBF90
        call   sub_1_5953
        add    esp, 0xc
        mov    byte ptr [esi], al
        cmp    al, 0xff
        jne    loc_1_4963
        lea    eax, [ebp - 1]
        push   eax
        push   5
        push   dword ptr [0x40cf90]          ; -> obj1:0xBF90
        call   sub_1_5953
        add    esp, 0xc
        mov    byte ptr [esi], al
loc_1_4963:
        mov    al, byte ptr [esi]
        cmp    al, 0xff
        je     loc_1_49A5
        mov    byte ptr [ebx + 0x16], al
        mov    al, byte ptr [ebx + 8]
        cmp    byte ptr [ebp - 1], 0
        jne    loc_1_4979
        and    al, 0xef
        jmp    loc_1_497B
loc_1_4979:
        or     al, 0x10
loc_1_497B:
        push   eax
        push   0x1e
        call   sub_1_1D05
        add    esp, 8
        push   0x21
        call   sub_1_3281
        add    esp, 4
        mov    dl, 1
        mov    cl, byte ptr [esi]
        shl    dl, cl
        or     al, dl
        push   eax
        push   0x21
        call   sub_1_3276
        add    esp, 8
        jmp    loc_1_49BB
loc_1_49A5:
        push   1
        push   0x15926
        push   0x159b
        call   sub_1_1CB6
        add    esp, 0xc
        mov    edi, eax
loc_1_49BB:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_49C4:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        xor    ebx, ebx
        mov    esi, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 1], bl
        mov    byte ptr [ebp - 2], bl
        cmp    word ptr [esi + 0x16], 0xcc
        jne    loc_1_49E3
        push   2
        jmp    loc_1_4A55
loc_1_49E3:
        mov    byte ptr [esi + 0x10], 0
loc_1_49E7:
        cmp    byte ptr [ebp - 1], 0
        jne    loc_1_4A1D
        cmp    byte ptr [ebp - 2], 0
        je     loc_1_49FE
        push   0
        push   esi
        call   sub_1_2F18
        add    esp, 8
loc_1_49FE:
        push   esi
        call   sub_1_4679
        add    esp, 4
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_1_4A11
        mov    byte ptr [ebp - 1], 1
loc_1_4A11:
        mov    al, byte ptr [esi + 0x10]
        inc    al
        mov    byte ptr [esi + 0x10], al
        cmp    al, 8
        jb     loc_1_49E7
loc_1_4A1D:
        push   ebx
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 0x1598
        jne    loc_1_4A34
        cmp    byte ptr [ebp - 2], 0
        jne    loc_1_4A34
        xor    ebx, ebx
loc_1_4A34:
        inc    byte ptr [ebp - 2]
        test   ebx, ebx
        jne    loc_1_4A47
        cmp    byte ptr [ebp - 2], 2
        jae    loc_1_4A47
        cmp    byte ptr [ebp - 1], 0
        je     loc_1_49E3
loc_1_4A47:
        cmp    byte ptr [ebp - 1], 0
        jne    loc_1_4A69
        cmp    byte ptr [ebp - 2], 2
        jne    loc_1_4A69
        push   1
loc_1_4A55:
        push   0x15921
        push   0x1599
        call   sub_1_1CB6
        add    esp, 0xc
        mov    ebx, eax
loc_1_4A69:
        mov    eax, ebx
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_4A71:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 2], al
        mov    byte ptr [ebp - 1], al
        cmp    byte ptr [ecx + 0x1f], 1
        jne    loc_1_4A8F
        mov    byte ptr [ebp - 2], 2
        jmp    loc_1_4AF4
loc_1_4A8F:
        mov    bl, 1
        call   sub_1_3405
        push   0x81
        call   sub_1_3281
        add    esp, 4
        mov    byte ptr [ebp - 3], al
loc_1_4AA6:
        cmp    byte ptr [ebp - 1], 0
        jne    loc_1_4AD5
        push   ebx
        push   0x81
        call   sub_1_3276
        add    esp, 8
        push   0x81
        call   sub_1_3281
        add    esp, 4
        cmp    al, bl
        je     loc_1_4ACF
        mov    byte ptr [ebp - 1], 1
loc_1_4ACF:
        add    bl, bl
        test   bl, bl
        jne    loc_1_4AA6
loc_1_4AD5:
        push   dword ptr [ebp - 3]
        push   0x81
        call   sub_1_3276
        add    esp, 8
        call   sub_1_33EF
        cmp    byte ptr [ebp - 1], 0
        jne    loc_1_4AF4
        mov    byte ptr [ebp - 2], 1
loc_1_4AF4:
        mov    al, byte ptr [ebp - 2]
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_4AFC:
        mov    ecx, dword ptr [0x40d014]     ; -> obj1:0xC014
        movzx  eax, byte ptr [ecx + 0x22]
        test   eax, eax
        je     loc_1_4B20
        cmp    eax, 1
        je     loc_1_4B27
        cmp    eax, 2
        jl     loc_1_4B4D
        cmp    eax, 3
        jle    loc_1_4B20
        cmp    eax, 4
        je     loc_1_4B34
        jmp    loc_1_4B4D
loc_1_4B20:
        mov    al, byte ptr [ecx + 2]
        or     al, 0x20
        jmp    loc_1_4B39
loc_1_4B27:
        mov    al, byte ptr [ecx + 0x23]
        and    al, 0x7f
        push   eax
        mov    byte ptr [ecx + 0x23], al
        push   0x65
        jmp    loc_1_4B45
loc_1_4B34:
        mov    al, byte ptr [ecx + 2]
        or     al, 0x80
loc_1_4B39:
        push   eax
        mov    byte ptr [ecx + 2], al
        movzx  eax, word ptr [ecx + 0x18]
        add    eax, 2
        push   eax
loc_1_4B45:
        call   sub_1_3276
        add    esp, 8
loc_1_4B4D:
        ret    
sub_1_4B4E:
        sub    esp, 8
        push   esi
        push   edi
        mov    esi, 0x40d058                 ; -> loc_1_C058
        lea    edi, [esp + 8]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
        xor    al, al
        pop    edi
        pop    esi
        add    esp, 8
        ret    
sub_1_4B68:
        xor    al, al
        ret    
sub_1_4B6B:
        push   ebx
        push   esi
        mov    esi, dword ptr [0x40d014]     ; -> obj1:0xC014
        xor    bl, bl
        call   sub_1_3405
        push   0x22
        call   sub_1_3281
        movzx  ax, al
        mov    word ptr [esi + 0x26], ax
        add    esp, 4
        test   al, 1
        jne    loc_1_4CB5
        push   0x23
        call   sub_1_3281
        add    esp, 4
        mov    byte ptr [esi + 0x28], al
        test   al, 0x80
        je     loc_1_4CB5
        push   0xfc23
        call   sub_1_3281
        add    esp, 4
        push   0xf023
        call   sub_1_3281
        add    esp, 4
        push   0xc023
        call   sub_1_3281
        add    esp, 4
        push   0x23
        call   sub_1_3281
        add    esp, 4
        push   0x23
        call   sub_1_3281
        add    esp, 4
        test   al, 0x80
        jne    loc_1_4CB5
        push   0x61
        push   0x24
        call   sub_1_3276
        add    esp, 8
        push   0x25
        call   sub_1_3281
        add    esp, 4
        mov    byte ptr [esi + 0x2a], al
        test   al, 1
        jne    loc_1_4C25
        push   0x61
        push   0x24
        call   sub_1_3276
        add    esp, 8
        mov    al, byte ptr [esi + 0x2a]
        or     al, 1
        push   eax
        push   0x24
        call   sub_1_3276
        add    esp, 8
loc_1_4C25:
        push   0xfa
        push   0x24
        call   sub_1_3276
        add    esp, 8
        push   1
        push   0x25
        call   sub_1_3276
        add    esp, 8
        push   0x102
        call   sub_1_3281
        add    esp, 4
        mov    byte ptr [esi + 0x29], al
        xor    eax, eax
        mov    al, byte ptr [esi + 0x29]
        and    eax, 0x60
        je     loc_1_4C66
        cmp    eax, 0x20
        je     loc_1_4C72
        cmp    eax, 0x40
        je     loc_1_4C7E
        jmp    loc_1_4C8C
loc_1_4C66:
        mov    eax, dword ptr [esp + 0xc]
        cmp    word ptr [eax + 0x16], 0x378
        jmp    loc_1_4C88
loc_1_4C72:
        mov    eax, dword ptr [esp + 0xc]
        cmp    word ptr [eax + 0x16], 0x278
        jmp    loc_1_4C88
loc_1_4C7E:
        mov    eax, dword ptr [esp + 0xc]
        cmp    word ptr [eax + 0x16], 0x3bc
loc_1_4C88:
        jne    loc_1_4C8C
        mov    bl, 1
loc_1_4C8C:
        cmp    bl, 1
        jne    loc_1_4CAE
        mov    byte ptr [esi + 0x22], 3
        mov    al, byte ptr [esi + 0x29]
        test   al, 0x80
        jne    loc_1_4CB5
        or     al, 0x80
        push   eax
        push   0x102
        call   sub_1_3276
        add    esp, 8
        jmp    loc_1_4CB5
loc_1_4CAE:
        xor    bl, bl
        call   sub_1_2A36
loc_1_4CB5:
        call   sub_1_33EF
        mov    al, bl
        pop    esi
        pop    ebx
        ret    
sub_1_4CBF:
        push   esi
        push   edi
        mov    edi, dword ptr [0x40d014]     ; -> obj1:0xC014
        push   0x10
        call   sub_1_3B66
        add    esp, 4
        movzx  esi, al
        push   0x11
        call   sub_1_3B66
        movzx  eax, al
        shl    eax, 8
        add    esp, 4
        or     esi, eax
        mov    al, byte ptr [edi + 8]
        and    al, 2
        cmp    al, 1
        mov    al, byte ptr [edi + 3]
        sbb    ecx, ecx
        and    al, 0xdf
        push   eax
        push   0x18
        lea    edx, [ecx*4 + 8]
        sub    esi, edx
        call   sub_1_1D05
        add    esp, 8
        push   esi
        push   0x10
        shr    esi, 8
        call   sub_1_1D05
        add    esp, 8
        push   esi
        push   0x11
        call   sub_1_1D05
        add    esp, 8
        mov    al, byte ptr [edi + 3]
        and    al, 0xef
        or     al, 0x20
        push   eax
        push   0x18
        call   sub_1_1D05
        add    esp, 8
        push   2
        call   sub_1_0720
        add    esp, 4
        push   0x1c
        call   sub_1_2C87
        add    esp, 4
        pop    edi
        pop    esi
        ret    
sub_1_4D4A:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        mov    eax, dword ptr [esi + 8]
        mov    byte ptr [eax + 0x4a0], 1
        mov    eax, dword ptr [esi + 8]
        cmp    byte ptr [eax + 0x4a0], 0
        je     loc_1_4D92
        xor    ebx, ebx
loc_1_4D68:
        push   esi
        call   sub_1_5CAA
        add    esp, 4
        mov    eax, dword ptr [esi + 8]
        cmp    byte ptr [eax + 0x4a0], bl
        je     loc_1_4D87
        push   ebx
        push   ebx
        push   eax
        call   sub_1_5BDA
        add    esp, 0xc
loc_1_4D87:
        mov    eax, dword ptr [esi + 8]
        cmp    byte ptr [eax + 0x4a0], bl
        jne    loc_1_4D68
loc_1_4D92:
        mov    eax, dword ptr [esi + 8]
        push   dword ptr [eax + 0x4b8]
        call   sub_1_5BC8
        add    esp, 4
        pop    esi
        pop    ebx
        ret    
sub_1_4DA6:
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        xor    bl, bl
        mov    esi, dword ptr [esp + 0x20]
        push   ebp
        push   dword ptr [esi + 0x110]
        call   sub_1_60D4
        add    esp, 4
        cmp    al, 0xff
        je     loc_1_4DE6
        push   1
        push   0x1101f
        push   0x1150
        call   sub_1_1CB6
        add    esp, 0xc
        mov    dword ptr [esi + 0xd4], eax
        mov    al, 1
        jmp    loc_1_504A
loc_1_4DE6:
        cmp    dword ptr [esi + 0x34], 0
        jne    loc_1_4E86
        lea    edi, [esi + 0xd4]
        push   2
        lea    eax, [esp + 0x16]
        push   eax
        push   esi
        call   sub_1_5FD8
        add    esp, 0xc
        mov    dword ptr [edi], eax
        test   eax, eax
        jne    loc_1_4EE1
        test   byte ptr [esp + 0x12], 0xc0
        jne    loc_1_4E78
        push   dword ptr [esi + 0x110]
        call   sub_1_60CE
        add    esp, 4
        cmp    al, 1
        je     loc_1_4E4A
        mov    al, byte ptr [esi + 5]
        or     al, 0x20
        cmp    al, byte ptr [esp + 0x12]
        je     loc_1_4E4A
        push   1
        push   0x1101f
        push   0x115d
        call   sub_1_1CB6
        add    esp, 0xc
        mov    dword ptr [edi], eax
loc_1_4E4A:
        mov    al, byte ptr [esp + 0x13]
        cmp    byte ptr [esi + 0xbb], al
        je     loc_1_4E6C
        push   1
        push   0x1101f
        push   0x1157
        call   sub_1_1CB6
        add    esp, 0xc
        mov    dword ptr [edi], eax
loc_1_4E6C:
        mov    al, byte ptr [esp + 0x13]
        mov    byte ptr [esi + 0x10b], al
        jmp    loc_1_4EE1
loc_1_4E78:
        push   2
        push   0x1101f
        push   0x115d
        jmp    loc_1_4ED7
loc_1_4E86:
        push   dword ptr [esi + 0x94]
        push   dword ptr [esi + 0x90]
        push   dword ptr [esi + 0xcc]
        push   1
        push   dword ptr [esi + 0x110]
        call   sub_1_5F42
        lea    ecx, [esp + 0x2c]
        add    esp, 0x14
        lea    edi, [esi + 0xd4]
        push   7
        push   ecx
        push   esi
        call   sub_1_5FD8
        add    esp, 0xc
        mov    dword ptr [edi], eax
        test   eax, eax
        jne    loc_1_4EE1
        test   byte ptr [esp + 0x18], 0xc0
        je     loc_1_4EE1
        push   1
        push   0x1101f
        push   0x115f
loc_1_4ED7:
        call   sub_1_1CB6
        add    esp, 0xc
        mov    dword ptr [edi], eax
loc_1_4EE1:
        cmp    dword ptr [edi], 0
        jne    loc_1_5046
        movzx  eax, word ptr [esi + 0x7c]
        cmp    eax, dword ptr [esi + 0x34]
        je     loc_1_4FE6
        movzx  eax, word ptr [esi + 0xd0]
        lea    ebp, [esi + 0x94]
        mov    ecx, dword ptr [esi + eax*4 + 0xc4]
        mov    dword ptr [esi + 0x90], ecx
        movzx  eax, word ptr [esi + 0x48]
        shl    eax, 2
        mov    dword ptr [ebp], eax
        push   dword ptr [esi + 0x110]
        call   sub_1_384C
        add    esp, 4
        test   al, al
        je     loc_1_4F48
        mov    ax, word ptr [esi + 0x48]
        push   ebp
        shl    ax, 2
        push   0
        movzx  ecx, word ptr [esi + 0xd0]
        movzx  eax, ax
        imul   ecx, eax
        push   ecx
        jmp    loc_1_4F55
loc_1_4F48:
        push   ebp
        push   dword ptr [esi + 0x90]
        push   dword ptr [esi + 0xcc]
loc_1_4F55:
        push   1
        push   dword ptr [esi + 0x110]
        call   sub_1_5F59
        add    esp, 0x14
        push   dword ptr [esi + 0x110]
        call   sub_1_5E63
        add    esp, 4
        lea    eax, [esi + 0xf8]
        push   1
        push   6
        push   eax
        push   esi
        call   sub_1_5DDF
        add    esp, 0x10
        mov    dword ptr [edi], eax
        test   eax, eax
        je     loc_1_4FAE
        push   dword ptr [ebp]
        push   dword ptr [esi + 0x90]
        push   dword ptr [esi + 0xcc]
        push   1
        push   dword ptr [esi + 0x110]
        call   sub_1_5F42
        add    esp, 0x14
        mov    bl, 1
loc_1_4FAE:
        mov    ax, word ptr [esi + 0xd0]
        mov    cx, word ptr [esi + 0xd2]
        mov    word ptr [esi + 0xd0], cx
        mov    word ptr [esi + 0xd2], ax
        mov    ax, word ptr [esi + 0xd0]
        push   eax
        push   esi
        call   sub_1_5E6F
        add    esp, 8
        test   eax, eax
        je     loc_1_4FE1
        mov    bl, 1
loc_1_4FE1:
        inc    dword ptr [esi + 0x34]
        jmp    loc_1_5048
loc_1_4FE6:
        mov    byte ptr [esi + 0x10e], 0
        mov    al, byte ptr [esi + 0x10b]
        cmp    al, 0x80
        jae    loc_1_4FFB
        add    al, 6
        jmp    loc_1_4FFD
loc_1_4FFB:
        sub    al, 6
loc_1_4FFD:
        mov    byte ptr [esp + 0x14], 0xf
        mov    byte ptr [esp + 0x16], al
        mov    al, byte ptr [esi + 5]
        mov    cl, byte ptr [esp + 0x16]
        mov    byte ptr [esp + 0x15], al
        mov    byte ptr [esi + 0xbb], cl
        push   dword ptr [esi + 0x110]
        call   sub_1_5E63
        lea    ecx, [esp + 0x18]
        add    esp, 4
        push   0
        push   3
        push   ecx
        push   esi
        call   sub_1_5DDF
        add    esp, 0x10
        mov    dword ptr [edi], eax
        test   eax, eax
        jne    loc_1_5046
        mov    byte ptr [esi + 0x10f], 1
        jmp    loc_1_5048
loc_1_5046:
        mov    bl, 1
loc_1_5048:
        mov    al, bl
loc_1_504A:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
sub_1_5052:
        mov    eax, dword ptr [esp + 4]
        mov    al, byte ptr [eax + 0x10e]
        ret    
sub_1_505D:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    byte ptr [esi + 0x4e6], 0x22
        jne    loc_1_508E
        push   1
        push   0x1a
        call   sub_1_1D05
        add    esp, 8
        call   sub_1_553D
        mov    ax, word ptr [esi + 0x4da]
        inc    ax
        push   eax
        call   sub_1_3281
        add    esp, 4
loc_1_508E:
        cmp    byte ptr [esi + 0x4e1], 0
        je     loc_1_50DE
        cmp    byte ptr [esi + 0x4e2], 0
        je     loc_1_50BF
        cmp    byte ptr [esi + 0x4e3], 0
        jne    loc_1_50C8
        mov    ax, word ptr [esi + 0x4da]
        push   0x1f
        push   0
        push   eax
        call   sub_1_53D1
        add    esp, 0xc
        jmp    loc_1_50DE
loc_1_50BF:
        cmp    byte ptr [esi + 0x4e3], 0
        je     loc_1_50DE
loc_1_50C8:
        mov    ax, word ptr [esi + 0x4da]
        push   4
        add    ax, 0x13
        push   eax
        call   sub_1_3276
        add    esp, 8
loc_1_50DE:
        pop    esi
        ret    
sub_1_50E0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        mov    edi, dword ptr [ebp + 8]
        cmp    byte ptr [edi + 0x108], 0
        je     loc_1_5130
        push   dword ptr [edi + 0xe0]
        push   dword ptr [edi + 0x110]
        call   sub_1_60DF
        add    esp, 8
        and    al, 0xc0
        cmp    al, 0xc0
        je     loc_1_51CA
        push   1
        push   0x1103a
        push   0x1171
        call   sub_1_1CB6
        add    esp, 0xc
        mov    esi, eax
        jmp    loc_1_51E4
loc_1_5130:
        mov    ebx, 0x20000
loc_1_5135:
        push   dword ptr [edi + 0xe0]
        push   dword ptr [edi + 0x110]
        call   sub_1_60DF
        add    esp, 8
        and    al, 0xc0
        cmp    al, 0x80
        je     loc_1_5152
        dec    ebx
        jne    loc_1_5135
loc_1_5152:
        test   ebx, ebx
        je     loc_1_5190
        push   8
        mov    ebx, 0x20000
        push   dword ptr [edi + 0xdc]
        push   dword ptr [edi + 0x110]
        call   sub_1_6100
        add    esp, 0xc
loc_1_5171:
        push   dword ptr [edi + 0xe0]
        push   dword ptr [edi + 0x110]
        call   sub_1_60DF
        add    esp, 8
        test   al, 0x10
        jne    loc_1_518C
        dec    ebx
        jne    loc_1_5171
loc_1_518C:
        test   ebx, ebx
        jne    loc_1_51A6
loc_1_5190:
        push   1
        push   0x1103a
        push   0x1171
        call   sub_1_1CB6
        add    esp, 0xc
        mov    esi, eax
loc_1_51A6:
        cmp    byte ptr [ebp + 0xc], 0
        jne    loc_1_51E4
        test   esi, esi
        jne    loc_1_51F6
        push   dword ptr [edi + 0xdc]
        push   dword ptr [edi + 0x110]
        call   sub_1_60DF
        add    esp, 8
        mov    byte ptr [edi + 0x10c], al
loc_1_51CA:
        push   dword ptr [edi + 0xdc]
        push   dword ptr [edi + 0x110]
        call   sub_1_60DF
        add    esp, 8
        mov    byte ptr [edi + 0x10c], al
loc_1_51E4:
        test   esi, esi
        jne    loc_1_51F6
        mov    word ptr [edi + 0x106], 0
        jmp    loc_1_535F
loc_1_51F6:
        mov    ax, word ptr [edi + 0x106]
        cmp    ax, 0x14
        jbe    loc_1_5356
        mov    word ptr [edi + 0x106], 0
        cmp    byte ptr [edi + 0x11b], 0
        jne    loc_1_5244
        mov    byte ptr [edi + 0x11c], 0
        push   8
        push   dword ptr [edi + 0xec]
        push   dword ptr [edi + 0x110]
        call   sub_1_6100
        add    esp, 0xc
        push   0xa
        call   sub_1_0720
        add    esp, 4
        push   0xc
        jmp    loc_1_527B
loc_1_5244:
        cmp    byte ptr [edi + 0x11c], 1
        jne    loc_1_5252
        mov    bl, byte ptr [edi + 3]
        jmp    loc_1_5255
loc_1_5252:
        mov    bl, byte ptr [edi + 4]
loc_1_5255:
        and    bl, 0xfb
        push   ebx
        push   dword ptr [edi + 0xec]
        push   dword ptr [edi + 0x110]
        call   sub_1_6100
        add    esp, 0xc
        or     bl, 0xc
        push   0xa
        call   sub_1_0720
        add    esp, 4
        push   ebx
loc_1_527B:
        push   dword ptr [edi + 0xec]
        push   dword ptr [edi + 0x110]
        call   sub_1_6100
        add    esp, 0xc
        mov    ebx, 0x20000
        push   0x1f4
        call   sub_1_0720
        add    esp, 4
loc_1_52A1:
        push   dword ptr [edi + 0xe0]
        push   dword ptr [edi + 0x110]
        call   sub_1_60DF
        add    esp, 8
        and    al, 0xc0
        cmp    al, 0x80
        je     loc_1_52C8
        push   0xc
        call   sub_1_0720
        add    esp, 4
        dec    ebx
        jne    loc_1_52A1
loc_1_52C8:
        test   ebx, ebx
        je     loc_1_5316
        push   8
        push   dword ptr [edi + 0xdc]
        push   dword ptr [edi + 0x110]
        call   sub_1_6100
        add    esp, 0xc
        mov    ebx, 0x20000
        push   0xc
        call   sub_1_0720
        add    esp, 4
loc_1_52F1:
        push   dword ptr [edi + 0xe0]
        push   dword ptr [edi + 0x110]
        call   sub_1_60DF
        add    esp, 8
        test   al, 0x10
        jne    loc_1_5316
        push   0xc
        call   sub_1_0720
        add    esp, 4
        dec    ebx
        jne    loc_1_52F1
loc_1_5316:
        push   dword ptr [edi + 0xdc]
        push   dword ptr [edi + 0x110]
        call   sub_1_60DF
        add    esp, 8
        mov    byte ptr [edi + 0x10c], al
        push   dword ptr [edi + 0xdc]
        push   dword ptr [edi + 0x110]
        call   sub_1_60DF
        add    esp, 8
        mov    byte ptr [edi + 0x10c], al
        push   0xc
        call   sub_1_0720
        add    esp, 4
        jmp    loc_1_535F
loc_1_5356:
        inc    ax
        mov    word ptr [edi + 0x106], ax
loc_1_535F:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_5368:
        mov    eax, dword ptr [esp + 4]
        push   ebx
        cmp    byte ptr [eax + 0x4e6], 0x22
        jne    loc_1_539C
        mov    ax, word ptr [eax + 0x4da]
        mov    bl, 1
        inc    ax
        push   eax
        call   sub_1_3281
        add    esp, 4
        test   al, 8
        jne    loc_1_5390
        xor    bl, bl
loc_1_5390:
        cmp    bl, 1
        jne    loc_1_53CD
        call   sub_1_55CB
        jmp    loc_1_53CD
loc_1_539C:
        cmp    byte ptr [eax + 0x4e1], 0
        je     loc_1_53CB
        cmp    byte ptr [eax + 0x4e0], 0
        je     loc_1_53CB
        mov    ax, word ptr [eax + 0x4da]
        add    ax, 6
        push   eax
        call   sub_1_3281
        add    esp, 4
        and    al, 0x40
        shr    al, 6
        mov    bl, al
        jmp    loc_1_53CD
loc_1_53CB:
        mov    bl, 1
loc_1_53CD:
        mov    al, bl
        pop    ebx
        ret    
sub_1_53D1:
        mov    cx, word ptr [esp + 4]
        push   ebx
        movzx  eax, cx
        cmp    eax, 0x210
        jg     loc_1_53EC
        je     loc_1_5452
        cmp    eax, 0x180
        je     loc_1_544E
        jmp    loc_1_544A
loc_1_53EC:
        cmp    eax, 0x300
        jg     loc_1_53FE
        je     loc_1_5460
        cmp    eax, 0x2a0
        je     loc_1_5459
        jmp    loc_1_544A
loc_1_53FE:
        cmp    eax, 0x340
        jg     loc_1_5410
        je     loc_1_546E
        cmp    eax, 0x330
        je     loc_1_5467
        jmp    loc_1_544A
loc_1_5410:
        cmp    eax, 0x1fe0
        jg     loc_1_5420
        je     loc_1_544E
        cmp    eax, 0x370
        jmp    loc_1_5448
loc_1_5420:
        cmp    eax, 0x3fe0
        je     loc_1_5452
        cmp    eax, 0x5fe0
        je     loc_1_5459
        cmp    eax, 0x7fe0
        je     loc_1_5460
        cmp    eax, 0x9fe0
        je     loc_1_5467
        cmp    eax, 0xbfe0
        je     loc_1_546E
        cmp    eax, 0xdfe0
loc_1_5448:
        je     loc_1_5475
loc_1_544A:
        xor    eax, eax
        jmp    loc_1_547A
loc_1_544E:
        xor    eax, eax
        jmp    loc_1_547A
loc_1_5452:
        mov    eax, 1
        jmp    loc_1_547A
loc_1_5459:
        mov    eax, 2
        jmp    loc_1_547A
loc_1_5460:
        mov    eax, 3
        jmp    loc_1_547A
loc_1_5467:
        mov    eax, 4
        jmp    loc_1_547A
loc_1_546E:
        mov    eax, 5
        jmp    loc_1_547A
loc_1_5475:
        mov    eax, 6
loc_1_547A:
        mov    bl, byte ptr [eax + 0x40cf95] ; -> loc_1_BF95
        mov    dl, byte ptr [esp + 0x10]
        add    cx, 6
        or     bl, byte ptr [esp + 0xc]
        and    dl, bl
        mov    byte ptr [eax + 0x40cf95], bl ; -> loc_1_BF95
        push   edx
        mov    byte ptr [eax + 0x40cf95], dl ; -> loc_1_BF95
        push   ecx
        call   sub_1_3276
        add    esp, 8
        pop    ebx
        ret    
sub_1_54A6:
        push   esi
        mov    si, word ptr [esp + 8]
        push   esi
        call   sub_1_3281
        or     al, byte ptr [esp + 0x10]
        add    esp, 4
        push   eax
        push   esi
        call   sub_1_3276
        add    esp, 8
        pop    esi
        ret    
sub_1_54C5:
        push   esi
        push   edi
        mov    edi, dword ptr [0x40d014]     ; -> obj1:0xC014
        mov    esi, dword ptr [esp + 0xc]
        mov    ax, word ptr [esi]
        cmp    ax, 0x3f2
        je     loc_1_54EA
        cmp    byte ptr [esi + 0x22], 0x22
        push   0
        push   eax
        jne    loc_1_54FC
        call   sub_1_1D05
        jmp    loc_1_5501
loc_1_54EA:
        push   0xc
        push   eax
        call   sub_1_3276
        mov    ax, word ptr [esi + 2]
        add    esp, 8
        push   0xc
        push   eax
loc_1_54FC:
        call   sub_1_3276
loc_1_5501:
        add    esp, 8
        cmp    byte ptr [esi + 0x22], 0x22
        jne    loc_1_553A
        mov    al, byte ptr [edi + 4]
        and    al, 0xf0
        push   eax
        push   0x19
        call   sub_1_1D05
        add    esp, 8
        cmp    byte ptr [edi + 0x20], 0
        jne    loc_1_553A
        mov    al, byte ptr [edi + 2]
        and    al, 0xef
        push   eax
        mov    byte ptr [edi + 2], al
        mov    ax, word ptr [esi + 0x16]
        add    ax, 2
        push   eax
        call   sub_1_3276
        add    esp, 8
loc_1_553A:
        pop    edi
        pop    esi
        ret    
sub_1_553D:
        push   esi
        mov    esi, dword ptr [0x40d014]     ; -> obj1:0xC014
        mov    ax, word ptr [esi + 0x1a]
        test   ax, ax
        je     loc_1_5589
        dec    ax
        mov    word ptr [esi + 0x1a], ax
        jne    loc_1_5589
        mov    ax, word ptr [esi + 0x1c]
        test   ax, ax
        je     loc_1_5569
        push   eax
        push   0x19
        call   sub_1_1D05
        add    esp, 8
loc_1_5569:
        cmp    word ptr [esi + 0x1e], 0
        jne    loc_1_5589
        mov    ax, word ptr [esi + 0x18]
        inc    ax
        push   eax
        call   sub_1_3281
        add    esp, 4
        push   1
        call   sub_1_0C8C
        add    esp, 4
loc_1_5589:
        pop    esi
        ret    
sub_1_558B:
        mov    eax, dword ptr [0x40d014]     ; -> obj1:0xC014
        cmp    word ptr [esp + 4], 2
        mov    dl, byte ptr [eax + 3]
        mov    cl, dl
        jne    loc_1_55A9
        mov    ax, word ptr [esp + 8]
        and    cl, 0xef
        or     al, 8
        jmp    loc_1_55B9
loc_1_55A9:
        test   cl, 3
        mov    ax, word ptr [esp + 8]
        jne    loc_1_55B7
        or     al, 0x10
        jmp    loc_1_55B9
loc_1_55B7:
        or     al, 0x18
loc_1_55B9:
        or     cl, al
        cmp    cl, dl
        je     loc_1_55CA
        push   ecx
        push   0x18
        call   sub_1_1D05
        add    esp, 8
loc_1_55CA:
        ret    
sub_1_55CB:
        push   ebx
        push   esi
        mov    esi, dword ptr [0x40d014]     ; -> obj1:0xC014
        call   sub_1_3405
        mov    ax, word ptr [esi + 0x1a]
        mov    bl, byte ptr [esi + 4]
        test   ax, ax
        lea    ecx, [eax + 1]
        mov    word ptr [esi + 0x1a], cx
        jne    loc_1_5640
        cmp    dword ptr [0x401fed], 0       ; -> obj1:0x0FED
        jne    loc_1_55FC
        mov    word ptr [esi + 0x1e], 0xffff
        jmp    loc_1_561E
loc_1_55FC:
        call   sub_1_05A8
        test   ax, ax
        jne    loc_1_5618
        mov    word ptr [esi + 0x1e], 0
        push   0
        call   sub_1_0C8C
        add    esp, 4
        jmp    loc_1_561E
loc_1_5618:
        mov    word ptr [esi + 0x1e], 1
loc_1_561E:
        movzx  ax, byte ptr [esi + 4]
        mov    word ptr [esi + 0x1c], ax
        movzx  ax, bl
        mov    word ptr [esi + 0x1c], ax
        test   ax, ax
        je     loc_1_5640
        push   0
        push   0x19
        call   sub_1_1D05
        add    esp, 8
loc_1_5640:
        call   sub_1_33EF
        mov    al, bl
        pop    esi
        pop    ebx
        ret    
sub_1_564A:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x84
        mov    cx, 0x40
        push   ebx
        push   esi
        xor    ebx, ebx
        push   edi
        lea    eax, [ebp - 0x84]
        mov    dword ptr [ebp - 4], ebx
loc_1_5665:
        mov    byte ptr [eax], 0xa5
        inc    eax
        mov    byte ptr [eax], 0xa5
        inc    eax
        dec    cx
        jne    loc_1_5665
        xor    esi, esi
loc_1_5673:
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 4], esi
        jbe    loc_1_575E
        push   esi
        call   sub_1_3934
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        jne    loc_1_573B
        push   0x20
        lea    eax, [ebp - 0x84]
        push   0x80
        push   eax
        call   sub_1_39F7
        add    esp, 0xc
        mov    edi, eax
        test   edi, edi
        jne    loc_1_573B
        push   esi
        call   sub_1_3934
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        jne    loc_1_573B
        push   0x80
        lea    eax, [ebp - 0x84]
        push   0
        push   eax
        call   sub_1_33CF
        add    esp, 0xc
        lea    eax, [ebp - 0x84]
        push   0x20
        push   0x80
        push   eax
        call   sub_1_398D
        add    esp, 0xc
        mov    edi, eax
        test   edi, edi
        jne    loc_1_573B
        mov    cx, 0x80
        lea    eax, [ebp - 0x84]
loc_1_56FD:
        test   bl, bl
        jne    loc_1_571B
        mov    edx, eax
        inc    eax
        cmp    byte ptr [edx], 0xa5
        je     loc_1_570B
        mov    bl, 1
loc_1_570B:
        mov    edx, eax
        inc    eax
        cmp    byte ptr [edx], 0xa5
        je     loc_1_5715
        mov    bl, 1
loc_1_5715:
        sub    cx, 2
        jne    loc_1_56FD
loc_1_571B:
        cmp    bl, 1
        jne    loc_1_5737
        push   1
        push   0x15943
        push   0x1591
        call   sub_1_1CB6
        add    esp, 0xc
        mov    dword ptr [ebp - 4], eax
loc_1_5737:
        test   edi, edi
        je     loc_1_574E
loc_1_573B:
        push   1
        push   0x15943
        push   edi
        call   sub_1_1CB6
        add    esp, 0xc
        mov    dword ptr [ebp - 4], eax
loc_1_574E:
        add    esi, 0x1000
        cmp    dword ptr [ebp - 4], 0
        je     loc_1_5673
loc_1_575E:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_5768:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x20
        mov    eax, 0x9090909
        push   ebx
        push   esi
        xor    ebx, ebx
        push   edi
        mov    esi, dword ptr [0x40d014]     ; -> obj1:0xC014
        lea    edi, [ebp - 0x20]
        mov    ecx, 8
        rep stosd dword ptr es:[edi], eax
        push   0x1b
        call   sub_1_3B66
        add    esp, 4
        push   0x1c
        call   sub_1_3B66
        add    esp, 4
        mov    al, byte ptr [esi + 8]
        or     al, 2
        push   eax
        push   0x1e
        call   sub_1_1D05
        add    esp, 8
        mov    edi, dword ptr [ebp + 8]
        push   ebx
        mov    dword ptr [edi + 4], ebx
        call   sub_1_3934
        add    esp, 4
        test   eax, eax
        jne    loc_1_57D7
        push   0x20
        lea    eax, [ebp - 0x20]
        mov    byte ptr [ebp - 0x20], 0xa5
        push   1
        push   eax
        call   sub_1_39F7
        add    esp, 0xc
        test   eax, eax
        je     loc_1_57E9
loc_1_57D7:
        push   2
        push   0x15942
        push   eax
        call   sub_1_1CB6
        add    esp, 0xc
        mov    ebx, eax
loc_1_57E9:
        test   ebx, ebx
        jne    loc_1_58E1
        push   0x40000
        call   sub_1_3934
        add    esp, 4
        test   eax, eax
        jne    loc_1_581A
        push   0x20
        lea    eax, [ebp - 0x20]
        mov    byte ptr [ebp - 0x20], 0x5a
        push   1
        push   eax
        call   sub_1_39F7
        add    esp, 0xc
        test   eax, eax
        je     loc_1_582C
loc_1_581A:
        push   3
        push   0x15942
        push   eax
        call   sub_1_1CB6
        add    esp, 0xc
        mov    ebx, eax
loc_1_582C:
        test   ebx, ebx
        jne    loc_1_58E1
        push   0
        call   sub_1_3934
        add    esp, 4
        test   eax, eax
        jne    loc_1_5856
        push   0x20
        lea    eax, [ebp - 0x20]
        push   1
        push   eax
        call   sub_1_398D
        add    esp, 0xc
        test   eax, eax
        je     loc_1_5868
loc_1_5856:
        push   4
        push   0x15942
        push   eax
        call   sub_1_1CB6
        add    esp, 0xc
        mov    ebx, eax
loc_1_5868:
        test   ebx, ebx
        jne    loc_1_58E1
        cmp    byte ptr [ebp - 0x20], 0x5a
        jne    loc_1_587B
        mov    dword ptr [edi + 4], 0x40000
        jmp    loc_1_58E1
loc_1_587B:
        cmp    byte ptr [ebp - 0x20], 0xa5
        jne    loc_1_58E1
        push   0x40000
        call   sub_1_3934
        add    esp, 4
        test   eax, eax
        jne    loc_1_58A6
        push   0x20
        lea    eax, [ebp - 0x20]
        push   1
        push   eax
        call   sub_1_398D
        add    esp, 0xc
        test   eax, eax
        je     loc_1_58B8
loc_1_58A6:
        push   5
        push   0x15942
        push   eax
        call   sub_1_1CB6
        add    esp, 0xc
        mov    ebx, eax
loc_1_58B8:
        test   ebx, ebx
        jne    loc_1_58CB
        cmp    byte ptr [ebp - 0x20], 0xa5
        jne    loc_1_58CB
        mov    dword ptr [edi + 4], 0x100000
        jmp    loc_1_58E1
loc_1_58CB:
        push   6
        push   0x15942
        push   0x1590
        call   sub_1_1CB6
        add    esp, 0xc
        mov    ebx, eax
loc_1_58E1:
        test   byte ptr [esi + 8], 2
        je     loc_1_58EA
        shr    dword ptr [edi + 4], 1
loc_1_58EA:
        cmp    dword ptr [edi + 4], 0
        jne    loc_1_5906
        push   7
        push   0x15942
        push   0x1590
        call   sub_1_1CB6
        add    esp, 0xc
        mov    ebx, eax
loc_1_5906:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_590F:
        push   esi
        push   0x18
        mov    esi, dword ptr [0x40d014]     ; -> obj1:0xC014
        call   sub_1_3B66
        add    esp, 4
        mov    byte ptr [esi + 3], al
        push   0x1e
        call   sub_1_3B66
        add    esp, 4
        mov    byte ptr [esi + 8], al
        push   0x1b
        call   sub_1_3B66
        add    esp, 4
        push   0x1c
        call   sub_1_3B66
        add    esp, 4
        push   0x19
        call   sub_1_3B66
        add    esp, 4
        mov    byte ptr [esi + 4], al
        pop    esi
        ret    
sub_1_5953:
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        push   ebp
        mov    edi, dword ptr [0x40d014]     ; -> obj1:0xC014
        mov    bp, 0xff
        push   ebx
        movzx  esi, word ptr [esp + 0x1c]
        push   0x407125                      ; -> loc_1_6125
        push   esi
        push   dword ptr [esp + 0x20]
        call   sub_1_0618
        add    esp, 0x10
        call   sub_1_2BE8
        mov    al, byte ptr [edi + 4]
        or     al, 8
        push   eax
        push   0x19
        call   sub_1_1D05
        add    esp, 8
        mov    al, byte ptr [edi + 8]
        and    al, 0xef
        push   eax
        push   0x1e
        call   sub_1_1D05
        add    esp, 8
        mov    al, byte ptr [edi + 2]
        or     al, 0x10
        push   eax
        mov    byte ptr [edi + 2], al
        mov    ax, word ptr [edi + 0x18]
        add    ax, 2
        push   eax
        call   sub_1_3276
        mov    cl, byte ptr [esp + 0x20]
        add    esp, 8
        mov    al, 1
        shl    al, cl
        not    al
        push   eax
        push   0x21
        call   sub_1_3276
        add    esp, 8
        call   sub_1_2C1D
        mov    dword ptr [0x40d018], ebx     ; -> obj1:0xC018
        mov    dword ptr [0x40d01c], ebx     ; -> obj1:0xC01C
        mov    bl, 0xa
loc_1_59E2:
        mov    al, byte ptr [edi + 8]
        xor    al, 0x10
        push   eax
        mov    byte ptr [edi + 8], al
        push   0x1e
        call   sub_1_1D05
        mov    ax, word ptr [edi + 0x18]
        add    esp, 8
        inc    ax
        push   eax
        call   sub_1_3281
        add    esp, 4
        dec    bl
        jne    loc_1_59E2
        cmp    dword ptr [0x40d018], 0xa     ; -> obj1:0xC018
        jne    loc_1_5A1F
        mov    bp, word ptr [esp + 0x18]
        mov    eax, dword ptr [esp + 0x1c]
        mov    byte ptr [eax], 0
        jmp    loc_1_5A34
loc_1_5A1F:
        cmp    dword ptr [0x40d01c], 0xa     ; -> obj1:0xC01C
        jne    loc_1_5A34
        mov    bp, word ptr [esp + 0x18]
        mov    eax, dword ptr [esp + 0x1c]
        mov    byte ptr [eax], 0x10
loc_1_5A34:
        call   sub_1_2BE8
        push   0x407125                      ; -> loc_1_6125
        push   esi
        call   sub_1_07FC
        add    esp, 8
        mov    al, byte ptr [edi + 2]
        and    al, 0xef
        push   eax
        mov    byte ptr [edi + 2], al
        mov    ax, word ptr [edi + 0x18]
        add    ax, 2
        push   eax
        call   sub_1_3276
        mov    ax, bp
        add    esp, 8
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_5A69:
        push   ebx
        xor    eax, eax
        push   esi
        mov    cl, 0x80
        mov    ebx, dword ptr [esp + 0x14]
        push   edi
        mov    edi, dword ptr [esp + 0x14]
        push   ebp
        xor    dl, dl
        mov    esi, dword ptr [esp + 0x14]
        mov    byte ptr [edi], 0xff
        mov    byte ptr [ebx], al
loc_1_5A84:
        cmp    byte ptr [edi], 0xff
        jne    loc_1_5AD8
        xor    bp, bp
loc_1_5A8C:
        movzx  ebx, bp
        mov    bl, byte ptr [ebx + esi]
        and    bl, cl
        cmp    bl, 1
        sbb    ebx, ebx
        add    ax, ax
        inc    ebx
        or     bx, ax
        mov    ax, bx
        inc    bp
        cmp    bp, 0xa
        jb     loc_1_5A8C
        cmp    ax, 0x2aa
        je     loc_1_5ABE
        cmp    ax, 0x155
        je     loc_1_5ABE
        shr    cl, 1
        xor    ax, ax
        jmp    loc_1_5AD1
loc_1_5ABE:
        mov    bl, 7
        sub    bl, dl
        cmp    ax, 0x155
        mov    byte ptr [edi], bl
        jne    loc_1_5AD1
        mov    ebx, dword ptr [esp + 0x1c]
        mov    byte ptr [ebx], 0x10
loc_1_5AD1:
        inc    dl
        cmp    dl, 8
        jb     loc_1_5A84
loc_1_5AD8:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_5ADD:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [0x40d014]     ; -> obj1:0xC014
        call   sub_1_3405
        push   0x21
        call   sub_1_3281
        mov    byte ptr [esp + 0x13], al
        add    esp, 4
        push   0xff
        push   0x21
        call   sub_1_3276
        add    esp, 8
        push   0xa
        push   0x20
        call   sub_1_3276
        add    esp, 8
        mov    al, byte ptr [esi + 4]
        or     al, 8
        push   eax
        push   0x19
        call   sub_1_1D05
        add    esp, 8
        mov    al, byte ptr [esi + 2]
        or     al, 0x10
        push   eax
        mov    byte ptr [esi + 2], al
        mov    ax, word ptr [esi + 0x18]
        xor    bl, bl
        add    ax, 2
        push   eax
        call   sub_1_3276
        mov    edi, dword ptr [esp + 0x1c]
        add    esp, 8
loc_1_5B47:
        mov    ax, word ptr [esi + 0x18]
        inc    ax
        push   eax
        call   sub_1_3281
        add    esp, 4
        mov    al, byte ptr [esi + 8]
        xor    al, 0x10
        push   eax
        mov    byte ptr [esi + 8], al
        push   0x1e
        call   sub_1_1D05
        add    esp, 8
        push   0x20
        call   sub_1_3281
        add    esp, 4
        push   0x20
        call   sub_1_3281
        add    esp, 4
        push   0x20
        call   sub_1_3281
        movzx  ecx, bl
        add    esp, 4
        inc    bl
        mov    byte ptr [ecx + edi], al
        cmp    bl, 0xa
        jb     loc_1_5B47
        push   dword ptr [esp + 0xf]
        push   0x21
        call   sub_1_3276
        add    esp, 8
        mov    al, byte ptr [esi + 2]
        and    al, 0xef
        push   eax
        mov    byte ptr [esi + 2], al
        mov    ax, word ptr [esi + 0x18]
        add    ax, 2
        push   eax
        call   sub_1_3276
        add    esp, 8
        call   sub_1_33EF
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_1_5BC8:
        mov    eax, dword ptr [esp + 4]
        test   eax, eax
        je     loc_1_5BD9
        push   eax
        call   sub_1_0CB8
        add    esp, 4
loc_1_5BD9:
        ret    
sub_1_5BDA:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        push   edi
        xor    edi, edi
        cmp    dword ptr [esi], edi
        je     loc_1_5C00
        push   esi
        mov    eax, dword ptr [0x40cf90]     ; -> obj1:0xBF90
        push   0x4071c8                      ; -> loc_1_61C8
        push   0x400000
        push   eax
        call   sub_1_079C
        add    esp, 0x10
loc_1_5C00:
        mov    al, byte ptr [esp + 0x18]
        test   al, al
        je     loc_1_5C40
        mov    cl, byte ptr [esi + 0x4af]
        test   cl, cl
        je     loc_1_5C40
        test   al, al
        je     loc_1_5CA4
        test   cl, cl
        je     loc_1_5CA4
        cmp    dword ptr [esi + 0x4c0], 0
        je     loc_1_5CA4
        mov    byte ptr [esi + 0x4b0], 0
        mov    dword ptr [esi + 0x4c0], 0
        push   2
        jmp    loc_1_5C90
loc_1_5C40:
        mov    ebx, dword ptr [esp + 0x14]
        test   ebx, ebx
        je     loc_1_5C5D
        push   esi
        push   0x4071a3                      ; -> loc_1_61A3
        push   ebx
        call   sub_1_0CCC
        add    esp, 0xc
        mov    dword ptr [esi + 0x4bc], eax
loc_1_5C5D:
        lea    eax, [esi + 0x4a4]
        mov    dword ptr [esi + 0x4a8], ebx
        push   eax
        call   sub_1_6169
        add    esp, 4
        cmp    eax, 1
        je     loc_1_5C80
        cmp    dword ptr [esi + 0x4c0], 0
        je     loc_1_5CA4
loc_1_5C80:
        xor    eax, eax
        push   1
        mov    byte ptr [esi + 0x4b0], al
        mov    dword ptr [esi + 0x4c0], eax
loc_1_5C90:
        push   0x1583b
        push   0x15a0
        call   sub_1_1CB6
        add    esp, 0xc
        mov    edi, eax
loc_1_5CA4:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_5CAA:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        push   edi
        push   ebp
        mov    eax, dword ptr [esi + 8]
        cmp    dword ptr [eax + 0x128], 0
        je     loc_1_5DDA
loc_1_5CC2:
        mov    eax, dword ptr [esi + 8]
        mov    edi, dword ptr [eax + 0x128]
        mov    eax, dword ptr [edi]
        cmp    dword ptr [eax], 0x150b
        jne    loc_1_5D2A
        mov    ax, word ptr [eax + 0x18]
        cmp    ax, 0x1112
        je     loc_1_5D2A
        cmp    ax, 0x1101
        je     loc_1_5CF1
        cmp    ax, 0x1100
        je     loc_1_5CF1
        cmp    ax, 0x1104
        jne    loc_1_5CFB
loc_1_5CF1:
        push   3
        call   sub_1_0D00
        add    esp, 4
loc_1_5CFB:
        push   dword ptr [edi]
        push   dword ptr [esi + 4]
        call   sub_1_62C5
        add    esp, 8
        mov    ecx, dword ptr [edi]
        mov    ax, word ptr [ecx + 0x18]
        cmp    ax, 0x1101
        je     loc_1_5D20
        cmp    ax, 0x1100
        je     loc_1_5D20
        cmp    ax, 0x1104
        jne    loc_1_5D2A
loc_1_5D20:
        push   3
        call   sub_1_0CF0
        add    esp, 4
loc_1_5D2A:
        mov    eax, dword ptr [edi]
        mov    bx, word ptr [eax + 0x18]
        mov    ebp, dword ptr [eax]
        cmp    bx, 0x1103
        je     loc_1_5D9A
        cmp    bx, 0x1100
        jne    loc_1_5D44
        test   ebp, ebp
        jne    loc_1_5D9A
loc_1_5D44:
        cmp    bx, 0x1101
        jne    loc_1_5D4F
        test   ebp, ebp
        jne    loc_1_5D9A
loc_1_5D4F:
        cmp    bx, 0x1102
        jne    loc_1_5D87
        test   ebp, ebp
        je     loc_1_5D87
        push   ebp
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 0x1167
        je     loc_1_5D87
        push   ebp
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 0x1164
        je     loc_1_5D87
        push   ebp
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 0x115c
        jne    loc_1_5D9A
loc_1_5D87:
        cmp    bx, 0x1112
        jne    loc_1_5DA9
        mov    eax, dword ptr [esi + 8]
        cmp    byte ptr [eax + 0x4ac], 0
        jne    loc_1_5DA9
loc_1_5D9A:
        push   esi
        mov    eax, dword ptr [0x40cf90]     ; -> obj1:0xBF90
        push   eax
        call   sub_1_388C
        add    esp, 8
loc_1_5DA9:
        push   dword ptr [edi + 4]
        push   dword ptr [edi]
        push   dword ptr [esi + 8]
        call   sub_1_1F88
        add    esp, 0xc
        mov    eax, dword ptr [esi + 8]
        add    eax, 0x128
        push   eax
        call   sub_1_1F57
        add    esp, 4
        mov    ecx, dword ptr [esi + 8]
        cmp    dword ptr [ecx + 0x128], 0
        jne    loc_1_5CC2
loc_1_5DDA:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_5DDF:
        mov    al, byte ptr [esp + 0x10]
        push   ebx
        mov    ebx, dword ptr [esp + 8]
        push   esi
        push   edi
        push   ebp
        xor    edi, edi
        mov    byte ptr [ebx + 0x108], al
        cmp    word ptr [esp + 0x1c], di
        je     loc_1_5E46
        mov    esi, dword ptr [esp + 0x18]
loc_1_5DFE:
        mov    ebp, 0x20000
loc_1_5E03:
        push   dword ptr [ebx + 0xe0]
        push   dword ptr [ebx + 0x110]
        call   sub_1_60DF
        add    esp, 8
        and    al, 0xc0
        cmp    al, 0x80
        je     loc_1_5E20
        dec    ebp
        jne    loc_1_5E03
loc_1_5E20:
        test   ebp, ebp
        je     loc_1_5E4D
        mov    al, byte ptr [esi + edi]
        inc    edi
        push   eax
        push   dword ptr [ebx + 0xdc]
        push   dword ptr [ebx + 0x110]
        call   sub_1_6100
        movzx  ecx, word ptr [esp + 0x28]
        add    esp, 0xc
        cmp    ecx, edi
        ja     loc_1_5DFE
loc_1_5E46:
        xor    eax, eax
loc_1_5E48:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_5E4D:
        push   1
        push   0x11033
        push   0x115d
        call   sub_1_1CB6
        add    esp, 0xc
        jmp    loc_1_5E48
sub_1_5E63:
        mov    eax, dword ptr [esp + 4]
        mov    byte ptr [eax + 0x4b0], 1
        ret    
sub_1_5E6F:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        mov    ebx, dword ptr [esp + 0x14]
        push   ebp
        movzx  ax, byte ptr [ebx + 0xba]
        cmp    word ptr [ebx + 0x4c], ax
        jae    loc_1_5EB1
        mov    byte ptr [ebx + 0xba], 1
        mov    al, byte ptr [ebx + 0xb8]
        inc    al
        mov    byte ptr [ebx + 0xb8], al
        cmp    byte ptr [ebx + 0x50], al
        jne    loc_1_5EB1
        mov    byte ptr [ebx + 0xb8], 0
        inc    byte ptr [ebx + 0xb9]
loc_1_5EB1:
        mov    al, byte ptr [ebx + 0xb8]
        mov    dl, byte ptr [ebx + 0xb9]
        mov    byte ptr [esp + 0x10], al
        mov    byte ptr [esp + 0x11], dl
        mov    byte ptr [esp + 0x13], 3
        movzx  edi, word ptr [esp + 0x1c]
        xor    dx, dx
        lea    ebp, [ebx + edi*4 + 0xbc]
        cmp    word ptr [ebx + 0x48], dx
        mov    ecx, dword ptr [ebp]
        jbe    loc_1_5F05
loc_1_5EE2:
        inc    dx
        mov    al, byte ptr [ebx + 0xba]
        mov    byte ptr [esp + 0x12], al
        inc    al
        mov    byte ptr [ebx + 0xba], al
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [ecx], eax
        add    ecx, 4
        cmp    word ptr [ebx + 0x48], dx
        ja     loc_1_5EE2
loc_1_5F05:
        push   dword ptr [ebx + 0x110]
        call   sub_1_384C
        add    esp, 4
        test   al, al
        je     loc_1_5F38
        mov    ax, word ptr [ebx + 0x48]
        push   1
        shl    ax, 2
        push   2
        push   eax
        movzx  eax, ax
        imul   eax, edi
        push   eax
        push   dword ptr [ebp]
        call   sub_1_3434
        add    esp, 0x14
        mov    esi, eax
loc_1_5F38:
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_1_5F42:
        mov    eax, dword ptr [esp + 4]
        push   0
        movzx  ecx, byte ptr [eax + 0x4e9]
        push   ecx
        call   sub_1_0D10
        add    esp, 8
        ret    
sub_1_5F59:
        push   ebp
        mov    eax, dword ptr [esp + 0x10]
        mov    ebp, esp
        sub    esp, 0x18
        push   esi
        add    eax, dword ptr [ebp + 0x14]
        mov    ecx, dword ptr [ebp + 0x18]
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0x18], eax
        mov    edx, dword ptr [ecx]
        movzx  eax, byte ptr [ebp + 0xc]
        mov    dword ptr [ebp - 0x14], edx
        mov    dword ptr [ebp - 0x10], eax
        movzx  ecx, byte ptr [esi + 0x4e9]
        mov    dword ptr [ebp - 0xc], ecx
        movzx  eax, word ptr [esi + 0x4da]
        mov    dword ptr [ebp - 4], 0
        mov    dword ptr [ebp - 8], eax
        cmp    byte ptr [esi + 0x4e3], 0
        jne    loc_1_5FAF
        lea    eax, [esi + 0x4c4]
        push   eax
        call   sub_1_64DE
        add    esp, 4
loc_1_5FAF:
        lea    eax, [ebp - 0x18]
        push   eax
        call   sub_1_0D84
        add    esp, 4
        cmp    byte ptr [esi + 0x4e3], 0
        je     loc_1_5FD3
        add    esi, 0x4c4
        push   esi
        call   sub_1_64DE
        add    esp, 4
loc_1_5FD3:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_5FD8:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    edi, dword ptr [esp + 0x14]
        push   ebp
        mov    dword ptr [esp + 0x10], ebx
        cmp    byte ptr [edi + 0x108], bl
        je     loc_1_6006
        mov    al, byte ptr [edi + 0x10c]
        mov    ecx, dword ptr [esp + 0x1c]
        mov    ebx, 1
        inc    dword ptr [esp + 0x1c]
        mov    byte ptr [ecx], al
loc_1_6006:
        mov    ebp, 0x20000
loc_1_600B:
        push   dword ptr [edi + 0xe0]
        push   dword ptr [edi + 0x110]
        call   sub_1_60DF
        movzx  esi, al
        add    esp, 8
        dec    ebp
        je     loc_1_602D
        test   esi, 0x80
        je     loc_1_600B
loc_1_602D:
        test   ebp, ebp
        jne    loc_1_603F
        push   1
        push   0x11036
        push   0x115d
        jmp    loc_1_6082
loc_1_603F:
        test   esi, 0x40
        je     loc_1_60A1
        push   dword ptr [edi + 0xdc]
        push   dword ptr [edi + 0x110]
        call   sub_1_60DF
        mov    ecx, dword ptr [esp + 0x24]
        add    esp, 8
        inc    ebx
        mov    byte ptr [ecx], al
        movzx  eax, word ptr [esp + 0x20]
        cmp    eax, ebx
        jb     loc_1_6076
        cmp    ebx, 0x10
        ja     loc_1_6076
        inc    dword ptr [esp + 0x1c]
        jmp    loc_1_608E
loc_1_6076:
        push   1
        push   0x11036
        push   0x1163
loc_1_6082:
        call   sub_1_1CB6
        mov    dword ptr [esp + 0x1c], eax
        add    esp, 0xc
loc_1_608E:
        test   esi, 0x40
        je     loc_1_60A1
        cmp    dword ptr [esp + 0x10], 0
        je     loc_1_6006
loc_1_60A1:
        movzx  eax, word ptr [esp + 0x20]
        cmp    eax, ebx
        je     loc_1_60C2
        push   2
        push   0x11036
        push   0x1163
        call   sub_1_1CB6
        mov    dword ptr [esp + 0x1c], eax
        add    esp, 0xc
loc_1_60C2:
        mov    eax, dword ptr [esp + 0x10]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_1_60CE:
        mov    al, byte ptr [0x40cf81]       ; -> loc_1_BF81
        ret    
sub_1_60D4:
        mov    eax, dword ptr [esp + 4]
        mov    al, byte ptr [eax + 0x4ac]
        ret    
sub_1_60DF:
        mov    eax, dword ptr [esp + 4]
        push   dword ptr [esp + 8]
        cmp    byte ptr [eax + 0x4e6], 0x22
        jne    loc_1_60F7
        call   sub_1_3B66
        jmp    loc_1_60FC
loc_1_60F7:
        call   sub_1_3281
loc_1_60FC:
        add    esp, 4
        ret    
sub_1_6100:
        push   dword ptr [esp + 0xc]
        mov    eax, dword ptr [esp + 8]
        push   dword ptr [esp + 0xc]
        cmp    byte ptr [eax + 0x4e6], 0x22
        jne    loc_1_611C
        call   sub_1_1D05
        jmp    loc_1_6121
loc_1_611C:
        call   sub_1_3276
loc_1_6121:
        add    esp, 8
        ret    
loc_1_6125:
        push   esi
        mov    esi, dword ptr [0x40d014]     ; -> obj1:0xC014
        test   byte ptr [esi + 8], 0x10
        je     loc_1_613A
        inc    dword ptr [0x40d018]          ; -> obj1:0xC018
        jmp    loc_1_6140
loc_1_613A:
        inc    dword ptr [0x40d01c]          ; -> obj1:0xC01C
loc_1_6140:
        mov    al, byte ptr [esi + 8]
        xor    al, 0x10
        push   eax
        mov    byte ptr [esi + 8], al
        push   0x1e
        call   sub_1_1D05
        mov    ax, word ptr [esi + 0x18]
        add    esp, 8
        inc    ax
        push   eax
        call   sub_1_3281
        add    esp, 4
        call   sub_1_0790
        pop    esi
        ret    
sub_1_6169:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        push   ebx
        push   ecx
        push   edx
        push   edi
        push   esi
        push   ebp
        push   ds
        push   es
        push   fs
        push   gs
        push   ss
        call   sub_1_3405
        mov    ebx, dword ptr [ebp + 8]
        mov    dword ptr [ebx], esp
        mov    esp, dword ptr [0x40cf8c]     ; -> obj1:0xBF8C
        call   sub_1_33EF
        pop    ss
        pop    gs
        pop    fs
        pop    es
        pop    ds
        pop    ebp
        pop    esi
        pop    edi
        pop    edx
        pop    ecx
        pop    ebx
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
loc_1_61A3:
        sub    esp, 4
        cld    
        mov    dword ptr [esp], edx
        push   dword ptr [esp]
        push   0x404a57                      ; -> sub_1_3A57
        push   0x400000
        push   dword ptr [0x40cf90]          ; -> obj1:0xBF90
        call   sub_1_079C
        add    esp, 0x14
        ret    
loc_1_61C8:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   esi
        mov    dword ptr [ebp - 8], edx
        call   sub_1_3405
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x4ad
        cmp    byte ptr [eax], 0
        jne    loc_1_62AB
        mov    ecx, dword ptr [ebp - 8]
        cmp    byte ptr [ecx + 0x4a0], 0
        je     loc_1_62AB
        mov    byte ptr [eax], 1
        call   sub_1_33EF
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [eax], 0
        je     loc_1_629F
loc_1_620C:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [eax]
        lea    eax, [ebp - 2]
        push   eax
        mov    esi, dword ptr [ecx]
        mov    dx, word ptr [esi + 0x18]
        lea    ecx, [ebp - 0xc]
        push   ecx
        push   edx
        call   sub_1_3532
        movzx  ecx, word ptr [ebp - 2]
        add    esp, 0xc
        add    ecx, esi
        mov    esi, dword ptr [ecx]
        add    ecx, 4
        test   esi, esi
        je     loc_1_626D
        cmp    dword ptr [ecx], esi
        jne    loc_1_6262
        movzx  eax, ax
        mov    dword ptr [esi + 4], eax
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ecx]
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [esi + 8], eax
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ecx]
        mov    eax, dword ptr [edx]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [esi + 0xc], ecx
        push   dword ptr [esi]
        call   sub_1_62B5
        add    esp, 4
loc_1_6262:
        push   0x14
        push   esi
        call   sub_1_041C
        add    esp, 8
loc_1_626D:
        mov    eax, dword ptr [ebp - 8]
        mov    eax, dword ptr [eax]
        push   dword ptr [eax + 4]
        push   dword ptr [eax]
        mov    eax, dword ptr [ebp - 8]
        add    eax, 0x378
        push   eax
        call   sub_1_1F88
        add    esp, 0xc
        push   dword ptr [ebp - 8]
        call   sub_1_1F57
        add    esp, 4
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [ecx], 0
        jne    loc_1_620C
loc_1_629F:
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [eax + 0x4ad], 0
        jmp    loc_1_62B0
loc_1_62AB:
        call   sub_1_33EF
loc_1_62B0:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_62B5:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    ebx, dword ptr [ebp + 8]
        VxDCall  NDIS.Service_0001h              ; cd2001002a00
        pop    ebx
        pop    ebp
        ret    
sub_1_62C5:
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    edi, dword ptr [esp + 0x10]
        push   dword ptr [edi + 0x110]
        mov    byte ptr [edi + 0x11e], bl
        call   sub_1_60D4
        add    esp, 4
        cmp    al, 0xff
        je     loc_1_62FC
        push   1
        push   0x11032
        push   0x1150
        call   sub_1_1CB6
        add    esp, 0xc
        mov    ebx, eax
loc_1_62FC:
        test   ebx, ebx
        jne    loc_1_636C
        cmp    byte ptr [edi + 0x11b], 0
        mov    esi, dword ptr [esp + 0x14]
        jne    loc_1_6335
        mov    ax, word ptr [esi + 0x18]
        cmp    ax, 0x1101
        je     loc_1_6335
        cmp    ax, 0x1100
        je     loc_1_6335
        push   1
        push   0x11032
        push   0x1159
        call   sub_1_1CB6
        add    esp, 0xc
        mov    ebx, eax
        jmp    loc_1_6370
loc_1_6335:
        cmp    byte ptr [edi + 0x11d], 0
        jne    loc_1_6370
        mov    ax, word ptr [esi + 0x18]
        cmp    ax, 0x1102
        je     loc_1_6370
        cmp    ax, 0x1100
        je     loc_1_6370
        cmp    ax, 0x1101
        je     loc_1_6370
        push   1
        push   0x11032
        push   0x115a
        call   sub_1_1CB6
        add    esp, 0xc
        mov    ebx, eax
        jmp    loc_1_6370
loc_1_636C:
        mov    esi, dword ptr [esp + 0x14]
loc_1_6370:
        test   ebx, ebx
        jne    loc_1_64D6
        cmp    byte ptr [edi + 0x120], 0
        je     loc_1_63B3
        mov    ax, word ptr [esi + 0x18]
        cmp    ax, 0x1101
        je     loc_1_63B3
        cmp    ax, 0x1102
        je     loc_1_63B3
        cmp    ax, 0x1104
        je     loc_1_63B3
        cmp    ax, 0x1107
        je     loc_1_63B3
        push   1
        push   0x11032
        push   0x1164
        call   sub_1_1CB6
        add    esp, 0xc
        mov    ebx, eax
loc_1_63B3:
        test   ebx, ebx
        jne    loc_1_64D6
        push   dword ptr [edi + 0x110]
        call   sub_1_70A6
        add    esp, 4
        mov    ebx, eax
        push   ebx
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 0x15a2
        jne    loc_1_63E5
        push   edi
        xor    ebx, ebx
        call   sub_1_6E6A
        add    esp, 4
loc_1_63E5:
        test   ebx, ebx
        jne    loc_1_64D6
        movzx  eax, word ptr [esi + 0x18]
        cmp    eax, 0x1100
        jl     loc_1_6406
        cmp    eax, 0x1102
        jle    loc_1_6417
        cmp    eax, 0x110c
        je     loc_1_640A
loc_1_6406:
        push   0
        jmp    loc_1_640C
loc_1_640A:
        push   2
loc_1_640C:
        push   edi
        call   sub_1_6CF2
        add    esp, 8
        mov    ebx, eax
loc_1_6417:
        test   ebx, ebx
        jne    loc_1_6427
        push   esi
        push   edi
        call   sub_1_6956
        add    esp, 8
        mov    ebx, eax
loc_1_6427:
        push   dword ptr [edi + 0x110]
        call   sub_1_60D4
        add    esp, 4
        cmp    al, 1
        je     loc_1_6480
        push   dword ptr [edi + 0x110]
        call   sub_1_6943
        add    esp, 4
        test   al, al
        je     loc_1_6489
        cmp    byte ptr [edi + 0x11e], 0
        jne    loc_1_6489
        push   edi
        call   sub_1_66A9
        add    esp, 4
        push   eax
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 0x115c
        jne    loc_1_6489
        cmp    byte ptr [edi + 0x19], 7
        jne    loc_1_6480
        cmp    byte ptr [edi + 0x1a], 4
        jne    loc_1_6480
        push   edi
        call   sub_1_653B
        jmp    loc_1_6486
loc_1_6480:
        push   edi
        call   sub_1_6660
loc_1_6486:
        add    esp, 4
loc_1_6489:
        push   dword ptr [edi + 0x110]
        call   sub_1_60D4
        add    esp, 4
        test   al, al
        jne    loc_1_64D6
        cmp    byte ptr [edi + 0x11c], 0
        je     loc_1_64D6
        push   edi
        call   sub_1_65A2
        add    esp, 4
        push   edi
        call   sub_1_653B
        add    esp, 4
        push   1
        push   edi
        call   sub_1_6500
        add    esp, 8
        push   dword ptr [edi + 0x110]
        call   sub_1_64FF
        mov    byte ptr [edi + 0x11d], 0
        add    esp, 4
loc_1_64D6:
        mov    eax, ebx
        pop    edi
        mov    dword ptr [esi], ebx
        pop    esi
        pop    ebx
        ret    
sub_1_64DE:
        mov    eax, dword ptr [esp + 4]
        cmp    byte ptr [eax + 0x1f], 0
        je     loc_1_64FE
        cmp    byte ptr [eax + 0x1d], 0
        je     loc_1_64FE
        push   1
        push   0xc1
        push   eax
        call   sub_1_70A9
        add    esp, 0xc
loc_1_64FE:
        ret    
sub_1_64FF:
        ret    
sub_1_6500:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        call   sub_1_6E6A
        add    esp, 4
        cmp    byte ptr [esp + 0xc], 0
        je     loc_1_651E
        push   esi
        call   sub_1_7106
        add    esp, 4
loc_1_651E:
        mov    byte ptr [esi + 0x11e], 1
        mov    byte ptr [esi + 0x3a], 0
        push   0
        push   dword ptr [esi + 0x110]
        call   sub_1_7105
        add    esp, 8
        pop    esi
        ret    
sub_1_653B:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        push   esi
        call   sub_1_66A9
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        je     loc_1_655F
        push   edi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 0x115c
        jne    loc_1_658D
loc_1_655F:
        push   0x12
        push   esi
        call   sub_1_7237
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        jne    loc_1_658D
        push   1
        push   0x1388
        push   esi
        call   sub_1_7191
        add    esp, 0xc
        mov    edi, eax
        test   edi, edi
        jne    loc_1_658D
        mov    byte ptr [esi + 0xb4], 0
loc_1_658D:
        push   0
        push   dword ptr [esi + 0x110]
        call   sub_1_7105
        add    esp, 8
        mov    eax, edi
        pop    edi
        pop    esi
        ret    
sub_1_65A2:
        push   esi
        push   edi
        xor    esi, esi
        mov    edi, dword ptr [esp + 0xc]
        cmp    byte ptr [edi + 0x11c], 0
        jne    loc_1_665B
        mov    al, byte ptr [edi + 3]
        push   eax
        push   dword ptr [edi + 0xec]
        push   dword ptr [edi + 0x110]
        call   sub_1_6100
        add    esp, 0xc
        mov    al, byte ptr [edi + 3]
        cmp    al, 0x8f
        je     loc_1_65DE
        cmp    al, 0xd
        je     loc_1_65DE
        cmp    al, 0x2d
        jne    loc_1_663E
loc_1_65DE:
        cmp    byte ptr [edi + 5], 1
        je     loc_1_663E
        cmp    byte ptr [edi + 0x19], 1
        jne    loc_1_661E
        push   0x2e
        push   edi
        call   sub_1_7237
        add    esp, 8
        mov    esi, eax
        test   esi, esi
        jne    loc_1_663E
        push   0
        push   3
        push   dword ptr [edi + 0x110]
        call   sub_1_5BDA
        add    esp, 0xc
        mov    al, byte ptr [edi + 8]
        add    al, 2
        push   eax
        push   edi
        call   sub_1_7237
        add    esp, 8
        mov    esi, eax
loc_1_661E:
        test   esi, esi
        jne    loc_1_6649
        mov    al, byte ptr [edi + 0x19]
        cmp    al, 2
        je     loc_1_6631
        cmp    al, 5
        je     loc_1_6631
        cmp    al, 4
        jne    loc_1_663E
loc_1_6631:
        push   1
        push   edi
        call   sub_1_73B0
        add    esp, 8
        mov    esi, eax
loc_1_663E:
        test   esi, esi
        jne    loc_1_6649
        mov    byte ptr [edi + 0x11c], 1
loc_1_6649:
        push   0
        push   3
        push   dword ptr [edi + 0x110]
        call   sub_1_5BDA
        add    esp, 0xc
loc_1_665B:
        mov    eax, esi
        pop    edi
        pop    esi
        ret    
sub_1_6660:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        push   3
        push   esi
        call   sub_1_7237
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        jne    loc_1_6694
        push   1
        push   0x3e80
        push   esi
        call   sub_1_7191
        add    esp, 0xc
        mov    edi, eax
        test   edi, edi
        jne    loc_1_6694
        mov    byte ptr [esi + 0xb4], 0
loc_1_6694:
        push   0
        push   dword ptr [esi + 0x110]
        call   sub_1_7105
        add    esp, 8
        mov    eax, edi
        pop    edi
        pop    esi
        ret    
sub_1_66A9:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    edi, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 2], bl
        mov    byte ptr [ebp - 4], bl
        mov    byte ptr [edi + 0x116], bl
        mov    byte ptr [edi + 0x117], bl
loc_1_66C9:
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 1]
        push   eax
        mov    byte ptr [ebp - 3], bl
        push   8
        push   ecx
        push   6
        push   edi
        call   sub_1_740C
        add    esp, 0x14
        mov    esi, eax
        test   esi, esi
        je     loc_1_670B
        push   esi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 0x115b
        jne    loc_1_6703
        cmp    byte ptr [ebp - 2], bl
        jne    loc_1_6703
        mov    byte ptr [ebp - 3], 1
        mov    byte ptr [ebp - 2], 1
loc_1_6703:
        test   esi, esi
        jne    loc_1_68E3
loc_1_670B:
        test   byte ptr [ebp - 1], 1
        jne    loc_1_6722
        push   1
        push   0x11022
        push   0x115c
        jmp    loc_1_68D9
loc_1_6722:
        test   byte ptr [ebp - 1], 4
        je     loc_1_67A0
        test   byte ptr [ebp - 1], 0x10
        jne    loc_1_6739
        cmp    byte ptr [edi + 0x19], 7
        jne    loc_1_6740
        cmp    byte ptr [edi + 0x3b], bl
        je     loc_1_6740
loc_1_6739:
        mov    byte ptr [edi + 0x120], 1
loc_1_6740:
        test   byte ptr [ebp - 1], 0x40
        mov    byte ptr [edi + 0xb5], 1
        jne    loc_1_6753
        mov    byte ptr [edi + 0xb5], bl
loc_1_6753:
        test   byte ptr [ebp - 1], 0x80
        mov    byte ptr [edi + 0xb6], 1
        jne    loc_1_6766
        mov    byte ptr [edi + 0xb6], bl
loc_1_6766:
        test   byte ptr [ebp - 1], 0x20
        je     loc_1_6772
        mov    byte ptr [edi + 0x3c], 1
        jmp    loc_1_6788
loc_1_6772:
        mov    byte ptr [edi + 0x3c], bl
        cmp    byte ptr [ebp - 2], bl
        jne    loc_1_6788
        test   byte ptr [ebp - 1], 2
        jne    loc_1_6788
        mov    byte ptr [ebp - 3], 1
        mov    byte ptr [ebp - 2], 1
loc_1_6788:
        test   byte ptr [ebp - 1], 8
        mov    byte ptr [edi + 0x82], 1
        jne    loc_1_679B
        mov    byte ptr [edi + 0x82], bl
loc_1_679B:
        mov    byte ptr [edi + 0x3b], bl
        jmp    loc_1_67BF
loc_1_67A0:
        mov    byte ptr [edi + 0x120], bl
        mov    byte ptr [edi + 0x3c], bl
        mov    byte ptr [edi + 0x82], bl
        mov    byte ptr [edi + 0xb5], bl
        mov    byte ptr [edi + 0x3b], 1
        mov    byte ptr [edi + 0xb6], bl
loc_1_67BF:
        test   byte ptr [ebp - 1], 0x12
        je     loc_1_68E3
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 6]
        push   eax
        push   0x10
        push   ecx
        push   7
        push   edi
        call   sub_1_740C
        add    esp, 0x14
        mov    esi, eax
        test   esi, esi
        je     loc_1_6808
        push   esi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 0x115b
        jne    loc_1_6800
        cmp    byte ptr [ebp - 2], bl
        jne    loc_1_6800
        mov    byte ptr [ebp - 3], 1
        mov    byte ptr [ebp - 2], 1
loc_1_6800:
        test   esi, esi
        jne    loc_1_68E3
loc_1_6808:
        test   byte ptr [ebp - 1], 2
        je     loc_1_683B
        mov    ax, word ptr [ebp - 6]
        mov    cl, byte ptr [ebp - 6]
        mov    byte ptr [edi + 0x117], cl
        mov    byte ptr [edi + 0x116], ah
        cmp    cl, 0xd
        jne    loc_1_6847
        mov    byte ptr [edi + 0x116], bl
        mov    byte ptr [edi + 0x117], bl
        mov    byte ptr [edi + 0x120], 1
        jmp    loc_1_6847
loc_1_683B:
        mov    byte ptr [edi + 0x116], bl
        mov    byte ptr [edi + 0x117], bl
loc_1_6847:
        mov    al, byte ptr [edi + 0x117]
        test   al, al
        je     loc_1_68E3
        movzx  cx, al
        movzx  eax, cx
        cmp    eax, 1
        je     loc_1_6884
        cmp    eax, 2
        je     loc_1_68E3
        cmp    eax, 8
        je     loc_1_6892
        cmp    eax, 0xff
        je     loc_1_68A0
        add    cx, 0x1100
        push   1
        movzx  eax, byte ptr [edi + 0x116]
        push   eax
        push   ecx
        jmp    loc_1_68D9
loc_1_6884:
        push   2
        push   0x11022
        push   0x115c
        jmp    loc_1_68D9
loc_1_6892:
        cmp    byte ptr [ebp - 4], bl
        je     loc_1_68E3
        mov    byte ptr [ebp - 3], 1
        mov    byte ptr [ebp - 4], bl
        jmp    loc_1_68E3
loc_1_68A0:
        push   edi
        call   sub_1_6E6A
        add    esp, 4
        mov    byte ptr [edi + 0x11c], bl
        push   edi
        call   sub_1_65A2
        add    esp, 4
        mov    esi, eax
        cmp    byte ptr [ebp - 2], bl
        jne    loc_1_68CD
        test   esi, esi
        jne    loc_1_68CD
        mov    byte ptr [ebp - 3], 1
        mov    byte ptr [ebp - 2], 1
        jmp    loc_1_68E3
loc_1_68CD:
        push   1
        push   0x11022
        push   0x1165
loc_1_68D9:
        call   sub_1_1CB6
        add    esp, 0xc
        mov    esi, eax
loc_1_68E3:
        cmp    byte ptr [ebp - 3], bl
        jne    loc_1_66C9
        test   esi, esi
        jne    loc_1_693A
        cmp    byte ptr [edi + 0x11d], 0
        mov    al, byte ptr [edi + 0x120]
        mov    byte ptr [edi + 0x3a], al
        je     loc_1_693A
        cmp    byte ptr [edi + 0x3b], 0
        je     loc_1_691E
        push   1
        push   0x11022
        push   0x1167
        call   sub_1_1CB6
        add    esp, 0xc
        mov    esi, eax
loc_1_691E:
        cmp    byte ptr [edi + 0x3a], 0
        je     loc_1_693A
        push   1
        push   0x11022
        push   0x1164
        call   sub_1_1CB6
        add    esp, 0xc
        mov    esi, eax
loc_1_693A:
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_6943:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 0x128]
        cmp    dword ptr [ecx + 8], 1
        sbb    al, al
        neg    al
        ret    
sub_1_6956:
        sub    esp, 4
        mov    dword ptr [esp], 0
        push   ebx
        push   esi
        mov    ebx, dword ptr [esp + 0x14]
        push   edi
        movzx  eax, word ptr [ebx + 0x18]
        push   ebp
        sub    eax, 0x1100
        cmp    eax, 0x14
        ja     loc_1_697E
        jmp    dword ptr [eax*4 + 0x407c9e]  ; -> obj1:0x6C9E
loc_1_697E:
        push   1
        push   0x11047
        push   0x1162
        call   sub_1_1CB6
        mov    dword ptr [esp + 0x1c], eax
        add    esp, 0xc
        jmp    loc_1_6C92
loc_1_699B:
        mov    ebp, dword ptr [esp + 0x18]
        push   ebp
        call   sub_1_6E6A
        add    esp, 4
        push   ebp
        call   sub_1_8A04
        add    esp, 4
        push   ebp
        call   sub_1_8851
        mov    dword ptr [esp + 0x14], eax
        add    esp, 4
        push   dword ptr [ebp + 0x110]
        call   sub_1_64FF
        add    esp, 4
        jmp    loc_1_6C86
loc_1_69D1:
        mov    ebp, dword ptr [esp + 0x18]
        push   ebx
        push   ebp
        call   sub_1_8664
        mov    dword ptr [esp + 0x18], eax
        add    esp, 8
        test   eax, eax
        je     loc_1_6A0A
        push   0
        push   0x3e8
        push   dword ptr [ebp + 0x110]
        call   sub_1_5BDA
        add    esp, 0xc
        push   ebx
        push   ebp
        call   sub_1_8664
        mov    dword ptr [esp + 0x18], eax
        add    esp, 8
loc_1_6A0A:
        push   dword ptr [ebp + 0x110]
        call   sub_1_64FF
        add    esp, 4
        jmp    loc_1_6C86
loc_1_6A1D:
        mov    ebp, dword ptr [esp + 0x18]
        push   ebp
        call   sub_1_6E6A
        add    esp, 4
        push   ebp
        call   sub_1_65A2
        mov    dword ptr [esp + 0x14], eax
        add    esp, 4
        test   eax, eax
        jne    loc_1_6AEE
        push   ebp
        call   sub_1_66A9
        mov    dword ptr [esp + 0x14], eax
        add    esp, 4
        push   eax
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 0x115c
        jne    loc_1_6A6F
        push   0
        push   0x493e0
        push   ebp
        call   sub_1_7191
        mov    dword ptr [esp + 0x1c], eax
        add    esp, 0xc
loc_1_6A6F:
        push   dword ptr [esp + 0x10]
        call   sub_1_247A
        movzx  eax, ax
        add    esp, 4
        test   eax, eax
        je     loc_1_6AAB
        cmp    eax, 0x111e
        je     loc_1_6AB4
        cmp    eax, 0x1164
        je     loc_1_6AEE
        cmp    eax, 0x1167
        je     loc_1_6AEE
        push   1
        cmp    eax, 0x15a0
        push   ebp
        je     loc_1_6ACE
        call   sub_1_6500
        add    esp, 8
        jmp    loc_1_6AEE
loc_1_6AAB:
        mov    byte ptr [ebp + 0x11d], 1
        jmp    loc_1_6AEE
loc_1_6AB4:
        push   ebp
        call   sub_1_66A9
        mov    dword ptr [esp + 0x14], eax
        add    esp, 4
        test   eax, eax
        jne    loc_1_6AEE
        mov    byte ptr [ebp + 0x11d], 1
        jmp    loc_1_6AEE
loc_1_6ACE:
        call   sub_1_6500
        add    esp, 8
        push   1
        push   0x11047
        push   0x1172
        call   sub_1_1CB6
        mov    dword ptr [esp + 0x1c], eax
        add    esp, 0xc
loc_1_6AEE:
        lea    esi, [ebp + 0x34]
        lea    edi, [ebx + 0x20]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        jmp    loc_1_6C92
loc_1_6AFC:
        mov    ebp, dword ptr [esp + 0x18]
        push   1
        push   ebp
        call   sub_1_6500
        add    esp, 8
        lea    esi, [ebp + 0x34]
        lea    edi, [ebx + 0x20]
        push   dword ptr [ebp + 0x110]
        call   sub_1_64FF
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    byte ptr [ebp + 0x11d], 0
        add    esp, 4
        jmp    loc_1_6C92
loc_1_6B2E:
        mov    ebp, dword ptr [esp + 0x18]
        push   ebx
        push   ebp
        lea    edi, [ebx + 0x40]
        lea    esi, [ebp + 0x34]
        call   sub_1_8454
        mov    dword ptr [esp + 0x18], eax
        add    esp, 8
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        jmp    loc_1_6C92
loc_1_6B4E:
        mov    ebp, dword ptr [esp + 0x18]
        lea    edi, [ebx + 0x20]
        push   ebp
        call   sub_1_8374
        mov    dword ptr [esp + 0x14], eax
        add    esp, 4
        lea    esi, [ebp + 0x34]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        jmp    loc_1_6C92
loc_1_6B6D:
        push   dword ptr [ebx + 0x2c]
        mov    ebp, dword ptr [esp + 0x1c]
        lea    edi, [ebx + 0x20]
        push   ebp
        call   sub_1_82BE
        mov    dword ptr [esp + 0x18], eax
        add    esp, 8
        lea    esi, [ebp + 0x34]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        jmp    loc_1_6C92
loc_1_6B8F:
        mov    ebp, dword ptr [esp + 0x18]
        push   ebx
        push   ebp
        call   sub_1_829F
        mov    dword ptr [esp + 0x18], eax
        add    esp, 8
        mov    byte ptr [ebp + 0x11e], 1
        jmp    loc_1_6C92
loc_1_6BAD:
        mov    ebp, dword ptr [esp + 0x18]
        push   ebx
        push   dword ptr [ebx + 0x20]
        push   ebp
        call   sub_1_7E26
        mov    dword ptr [esp + 0x1c], eax
        add    esp, 0xc
        jmp    loc_1_6C92
loc_1_6BC7:
        mov    ebp, dword ptr [esp + 0x18]
        push   ebx
        push   ebp
        lea    edi, [ebx + 0x3c]
        lea    esi, [ebp + 0x34]
        call   sub_1_7A3B
        mov    dword ptr [esp + 0x18], eax
        add    esp, 8
        mov    byte ptr [ebp + 0x3d], 0
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        jmp    loc_1_6C92
loc_1_6BEB:
        mov    ebp, dword ptr [esp + 0x18]
        push   ebx
        push   ebp
        lea    edi, [ebx + 0x20]
        lea    esi, [ebp + 0x70]
        call   sub_1_7855
        mov    dword ptr [esp + 0x18], eax
        add    esp, 8
        mov    ecx, 8
        rep movsd dword ptr es:[edi], dword ptr [esi]
        jmp    loc_1_6C92
loc_1_6C0F:
        mov    ebp, dword ptr [esp + 0x18]
        push   0
        push   ebp
        lea    edi, [ebx + 0x20]
        lea    esi, [ebp + 0x34]
        call   sub_1_7766
        mov    dword ptr [esp + 0x18], eax
        add    esp, 8
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        jmp    loc_1_6C92
loc_1_6C2D:
        add    ebx, 0x20
        mov    ebp, dword ptr [esp + 0x18]
        push   ebx
        push   ebp
        call   sub_1_7665
        mov    dword ptr [esp + 0x18], eax
        add    esp, 8
        jmp    loc_1_6C92
loc_1_6C44:
        mov    ebp, dword ptr [esp + 0x18]
        push   ebp
        call   sub_1_7496
        mov    dword ptr [esp + 0x14], eax
        add    esp, 4
        test   eax, eax
        je     loc_1_6C79
        push   eax
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 0x1167
        je     loc_1_6C79
        mov    dword ptr [ebp + 0xc], 0
        mov    byte ptr [ebp + 0x1d], 0
        mov    word ptr [ebp + 0x14], 0
loc_1_6C79:
        lea    esi, [ebp + 0xc]
        lea    edi, [ebx + 0x68]
        mov    ecx, 0xa
        rep movsd dword ptr es:[edi], dword ptr [esi]
loc_1_6C86:
        lea    esi, [ebp + 0x34]
        lea    edi, [ebx + 0x90]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
loc_1_6C92:
        mov    eax, dword ptr [esp + 0x10]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        ; ---- dispatch table (21 entries) ----
        dd  loc_1_699B
        dd  loc_1_69D1
        dd  loc_1_6A1D
        dd  loc_1_6AFC
        dd  loc_1_6B2E
        dd  loc_1_6B4E
        dd  loc_1_6B6D
        dd  loc_1_6B8F
        dd  loc_1_6BAD
        dd  loc_1_6BC7
        dd  loc_1_6BC7
        dd  loc_1_6BC7
        dd  loc_1_6BC7
        dd  loc_1_6BC7
        dd  loc_1_6BC7
        dd  loc_1_6BEB
        dd  loc_1_6C0F
        dd  loc_1_6C2D
        dd  loc_1_697E
        dd  loc_1_697E
        dd  loc_1_6C44
sub_1_6CF2:
        sub    esp, 4
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0x10]
        push   edi
        mov    bl, byte ptr [esp + 0x18]
        push   ebp
        mov    al, byte ptr [esi + 0x41]
        movzx  edi, bl
        movsx  ecx, al
        cmp    ecx, edi
        jne    loc_1_6D15
        xor    eax, eax
        jmp    loc_1_6E62
loc_1_6D15:
        test   al, al
        je     loc_1_6D21
        cmp    al, 2
        je     loc_1_6D21
        cmp    al, 1
        jne    loc_1_6D46
loc_1_6D21:
        push   esi
        call   sub_1_653B
        add    esp, 4
        mov    ebp, eax
        test   ebp, ebp
        je     loc_1_6D46
        push   ebp
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 0x1167
        je     loc_1_6D46
        mov    eax, ebp
        jmp    loc_1_6E62
loc_1_6D46:
        test   edi, edi
        je     loc_1_6D77
        cmp    edi, 1
        je     loc_1_6D7E
        cmp    edi, 2
        je     loc_1_6D85
        cmp    edi, 3
        je     loc_1_6D8C
        cmp    edi, 4
        je     loc_1_6D93
        push   1
        push   0x11044
        push   0x1162
        call   sub_1_1CB6
        add    esp, 0xc
        jmp    loc_1_6E62
loc_1_6D77:
        mov    byte ptr [esp + 0x13], 0x1e
        jmp    loc_1_6D98
loc_1_6D7E:
        mov    byte ptr [esp + 0x13], 0xf
        jmp    loc_1_6D98
loc_1_6D85:
        mov    byte ptr [esp + 0x13], 0x11
        jmp    loc_1_6D98
loc_1_6D8C:
        mov    byte ptr [esp + 0x13], 0x1c
        jmp    loc_1_6D98
loc_1_6D93:
        mov    byte ptr [esp + 0x13], 0x1d
loc_1_6D98:
        push   dword ptr [esp + 0x13]
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    loc_1_6E62
        push   0
        push   3
        push   dword ptr [esi + 0x110]
        call   sub_1_5BDA
        add    esp, 0xc
        cmp    bl, 3
        je     loc_1_6DFB
        cmp    bl, 4
        je     loc_1_6DFB
        push   esi
        call   sub_1_66A9
        add    esp, 4
        mov    edi, eax
        push   edi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 0x115c
        jne    loc_1_6E20
        push   1
        push   0x11044
        push   0x1170
        call   sub_1_1CB6
        add    esp, 0xc
        mov    edi, eax
        jmp    loc_1_6E20
loc_1_6DFB:
        push   dword ptr [esp + 0x13]
        push   esi
        call   sub_1_7237
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        jne    loc_1_6E60
        push   0
        push   3
        push   dword ptr [esi + 0x110]
        call   sub_1_5BDA
        add    esp, 0xc
loc_1_6E20:
        mov    byte ptr [esi + 0x41], bl
        test   edi, edi
        jne    loc_1_6E60
        cmp    bl, 2
        jne    loc_1_6E60
        cmp    byte ptr [esi + 0x19], 1
        jne    loc_1_6E60
        mov    al, byte ptr [esi + 0x1a]
        cmp    al, 4
        je     loc_1_6E3D
        cmp    al, 5
        jne    loc_1_6E60
loc_1_6E3D:
        push   0xe
        push   esi
        call   sub_1_7237
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        jne    loc_1_6E60
        push   1
        push   0xa3930
        push   esi
        call   sub_1_7191
        add    esp, 0xc
        mov    edi, eax
loc_1_6E60:
        mov    eax, edi
loc_1_6E62:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_1_6E6A:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    byte ptr [ebp - 1], 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    di, 5
        xor    ebx, ebx
        push   dword ptr [esi + 0x110]
        mov    byte ptr [esi + 0x108], bl
        call   sub_1_8CF8
        add    esp, 4
        cmp    byte ptr [esi + 0x11b], bl
        jne    loc_1_6F4A
        push   dword ptr [esi + 0x110]
        mov    byte ptr [esi + 0x11c], bl
        call   sub_1_384C
        add    esp, 4
        test   al, al
        jne    loc_1_6ED0
        push   8
        push   dword ptr [esi + 0xf0]
        push   dword ptr [esi + 0x110]
        call   sub_1_6100
        add    esp, 0xc
        jmp    loc_1_6F16
loc_1_6ED0:
        call   sub_1_55CB
        push   8
        push   dword ptr [esi + 0xec]
        push   dword ptr [esi + 0x110]
        call   sub_1_6100
        add    esp, 0xc
        push   0xa
        call   sub_1_0720
        add    esp, 4
        push   0xa
        call   sub_1_0720
        add    esp, 4
        push   1
        push   0x1a
        push   dword ptr [esi + 0x110]
        call   sub_1_6100
        add    esp, 0xc
        call   sub_1_553D
loc_1_6F16:
        push   0xa
        call   sub_1_0720
        add    esp, 4
        push   0xc
        push   dword ptr [esi + 0xec]
        push   dword ptr [esi + 0x110]
        call   sub_1_6100
        add    esp, 0xc
        cmp    byte ptr [esi + 0xf4], 0
        je     loc_1_700D
        push   0xc
        jmp    loc_1_6FF9
loc_1_6F4A:
        cmp    byte ptr [esi + 0x11c], 1
        jne    loc_1_6F58
        mov    bl, byte ptr [esi + 3]
        jmp    loc_1_6F5B
loc_1_6F58:
        mov    bl, byte ptr [esi + 4]
loc_1_6F5B:
        push   dword ptr [esi + 0x110]
        call   sub_1_384C
        and    bl, 0xfb
        add    esp, 4
        test   al, al
        jne    loc_1_6F87
        push   ebx
        push   dword ptr [esi + 0xf0]
        push   dword ptr [esi + 0x110]
        call   sub_1_6100
        add    esp, 0xc
        jmp    loc_1_6FCC
loc_1_6F87:
        call   sub_1_55CB
        push   ebx
        push   dword ptr [esi + 0xec]
        push   dword ptr [esi + 0x110]
        call   sub_1_6100
        add    esp, 0xc
        push   0xa
        call   sub_1_0720
        add    esp, 4
        push   0xa
        call   sub_1_0720
        add    esp, 4
        push   1
        push   0x1a
        push   dword ptr [esi + 0x110]
        call   sub_1_6100
        add    esp, 0xc
        call   sub_1_553D
loc_1_6FCC:
        push   0xa
        or     bl, 0xc
        call   sub_1_0720
        add    esp, 4
        push   ebx
        push   dword ptr [esi + 0xf0]
        push   dword ptr [esi + 0x110]
        call   sub_1_6100
        add    esp, 0xc
        cmp    byte ptr [esi + 0xf4], 0
        je     loc_1_700D
        push   0xd
loc_1_6FF9:
        push   dword ptr [esi + 0xf0]
        push   dword ptr [esi + 0x110]
        call   sub_1_6100
        add    esp, 0xc
loc_1_700D:
        push   1
        push   0x1f4
        push   dword ptr [esi + 0x110]
        call   sub_1_5BDA
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_7039
        mov    di, 4
        push   2
        lea    eax, [ebp - 4]
        push   eax
        push   esi
        call   sub_1_5FD8
        add    esp, 0xc
loc_1_7039:
        push   0
        lea    eax, [ebp - 1]
        push   1
        push   eax
        push   esi
        call   sub_1_5DDF
        add    esp, 0x10
        test   eax, eax
        jne    loc_1_709D
        dec    di
        push   2
        lea    eax, [ebp - 4]
        push   eax
        push   esi
        call   sub_1_5FD8
        add    esp, 0xc
        push   0
        push   4
        push   dword ptr [esi + 0x110]
        call   sub_1_5BDA
        add    esp, 0xc
        mov    al, byte ptr [ebp - 4]
        and    al, 3
        cmp    al, 3
        jae    loc_1_707F
        test   di, di
        jne    loc_1_7039
loc_1_707F:
        push   esi
        call   sub_1_8BC4
        add    esp, 4
        xor    eax, eax
        mov    byte ptr [esi + 0x10b], al
        mov    byte ptr [esi + 0x10d], al
loc_1_7096:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_709D:
        mov    byte ptr [esi + 0x10b], 0
        jmp    loc_1_7096
sub_1_70A6:
        xor    eax, eax
        ret    
sub_1_70A9:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        mov    ax, word ptr [esi + 0x16]
        add    ax, 0x11
        push   eax
        call   sub_1_3281
        add    esp, 4
        mov    bl, al
        and    al, 0xfc
        push   eax
        mov    ax, word ptr [esi + 0x16]
        add    ax, 0x11
        push   eax
        call   sub_1_3276
        mov    ax, word ptr [esi + 0x16]
        mov    cl, byte ptr [esp + 0x18]
        add    ax, 0x15
        add    esp, 8
        push   ecx
        push   eax
        call   sub_1_54A6
        mov    ax, word ptr [esi + 0x16]
        or     bl, byte ptr [esp + 0x1c]
        add    ax, 0x11
        add    esp, 8
        push   ebx
        push   eax
        call   sub_1_3276
        add    esp, 8
        pop    esi
        pop    ebx
        ret    
sub_1_7105:
        ret    
sub_1_7106:
        push   esi
        push   edi
        xor    esi, esi
        mov    edi, dword ptr [esp + 0xc]
        cmp    byte ptr [edi + 0x11c], 1
        jne    loc_1_718C
        mov    al, byte ptr [edi + 3]
        cmp    al, 0x8f
        je     loc_1_7126
        cmp    al, 0xd
        je     loc_1_7126
        cmp    al, 0x2d
        jne    loc_1_715B
loc_1_7126:
        cmp    byte ptr [edi + 5], 1
        je     loc_1_715B
        cmp    byte ptr [edi + 0x19], 1
        jne    loc_1_713F
        push   0x2f
        push   edi
        call   sub_1_7237
        add    esp, 8
        mov    esi, eax
loc_1_713F:
        mov    al, byte ptr [edi + 0x19]
        cmp    al, 2
        je     loc_1_714E
        cmp    al, 5
        je     loc_1_714E
        cmp    al, 4
        jne    loc_1_715B
loc_1_714E:
        push   0
        push   edi
        call   sub_1_73B0
        add    esp, 8
        mov    esi, eax
loc_1_715B:
        mov    al, byte ptr [edi + 4]
        push   eax
        push   dword ptr [edi + 0xec]
        push   dword ptr [edi + 0x110]
        call   sub_1_6100
        add    esp, 0xc
        mov    byte ptr [edi + 0x11c], 0
        push   0
        push   3
        push   dword ptr [edi + 0x110]
        call   sub_1_5BDA
        add    esp, 0xc
loc_1_718C:
        mov    eax, esi
        pop    edi
        pop    esi
        ret    
sub_1_7191:
        push   ebx
        push   esi
        push   edi
        push   ebp
        call   sub_1_0F80
        mov    esi, eax
        mov    edi, dword ptr [esp + 0x14]
        mov    bl, byte ptr [esp + 0x1c]
loc_1_71A4:
        push   0
        push   0x64
        push   dword ptr [edi + 0x110]
        call   sub_1_5BDA
        add    esp, 0xc
        push   edi
        call   sub_1_66A9
        add    esp, 4
        mov    ebp, eax
        push   ebp
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 0x115c
        jne    loc_1_7213
        test   bl, bl
        jne    loc_1_71E6
        push   dword ptr [edi + 0x110]
        call   sub_1_60D4
        add    esp, 4
        cmp    al, 0xff
        jne    loc_1_7217
loc_1_71E6:
        call   sub_1_0F80
        sub    eax, esi
        cmp    eax, dword ptr [esp + 0x18]
        jb     loc_1_71A4
        push   dword ptr [edi + 0x110]
        call   sub_1_60D4
        add    esp, 4
        cmp    al, 0xff
        push   1
        push   0x1104a
        je     loc_1_7225
        push   0x1150
        jmp    loc_1_722A
loc_1_7213:
        mov    eax, ebp
        jmp    loc_1_7232
loc_1_7217:
        push   1
        push   0x1104a
        push   0x1150
        jmp    loc_1_722A
loc_1_7225:
        push   0x15a0
loc_1_722A:
        call   sub_1_1CB6
        add    esp, 0xc
loc_1_7232:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_7237:
        push   ebp
        mov    al, byte ptr [esp + 0xc]
        mov    ebp, esp
        sub    esp, 8
        cmp    al, 0x80
        push   esi
        push   edi
        jb     loc_1_7260
        push   1
        push   0x11041
        push   0x1162
        call   sub_1_1CB6
        add    esp, 0xc
        jmp    loc_1_73AA
loc_1_7260:
        mov    esi, dword ptr [ebp + 8]
        mov    cl, byte ptr [esi + 0x10b]
        cmp    cl, 0x80
        jae    loc_1_7275
        add    al, cl
        mov    byte ptr [ebp - 6], al
        jmp    loc_1_727A
loc_1_7275:
        sub    cl, al
        mov    byte ptr [ebp - 6], cl
loc_1_727A:
        push   dword ptr [esi + 0x110]
        mov    byte ptr [ebp - 8], 0xf
        mov    al, byte ptr [esi + 5]
        mov    byte ptr [ebp - 7], al
        call   sub_1_8CF8
        add    esp, 4
        lea    ecx, [ebp - 8]
        push   0
        push   3
        push   ecx
        push   esi
        call   sub_1_5DDF
        add    esp, 0x10
        mov    edi, eax
        test   edi, edi
        je     loc_1_72BC
        push   dword ptr [esi + 0x110]
        call   sub_1_8D0B
        add    esp, 4
        jmp    loc_1_739F
loc_1_72BC:
        push   1
        push   0x3e8
        push   dword ptr [esi + 0x110]
        call   sub_1_5BDA
        add    esp, 0xc
        mov    edi, eax
        push   edi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 0x15a0
        jne    loc_1_7307
        push   0
        lea    eax, [ebp - 1]
        mov    byte ptr [ebp - 1], 8
        push   1
        push   eax
        push   esi
        call   sub_1_5DDF
        add    esp, 0x10
        mov    edi, eax
        push   dword ptr [esi + 0x110]
        call   sub_1_505D
        add    esp, 4
loc_1_7307:
        test   edi, edi
        jne    loc_1_739F
        push   2
        lea    eax, [ebp - 4]
        push   eax
        push   esi
        call   sub_1_5FD8
        add    esp, 0xc
        mov    edi, eax
        test   edi, edi
        jne    loc_1_73A8
        test   byte ptr [ebp - 4], 0xc0
        jne    loc_1_7389
        push   dword ptr [esi + 0x110]
        call   sub_1_60CE
        add    esp, 4
        cmp    al, 1
        je     loc_1_7360
        mov    al, byte ptr [esi + 5]
        or     al, 0x20
        cmp    al, byte ptr [ebp - 4]
        je     loc_1_7360
        push   1
        push   0x11041
        push   0x115d
        call   sub_1_1CB6
        add    esp, 0xc
        jmp    loc_1_73AA
loc_1_7360:
        mov    al, byte ptr [ebp - 6]
        cmp    byte ptr [ebp - 3], al
        je     loc_1_737E
        push   1
        push   0x11041
        push   0x1157
        call   sub_1_1CB6
        add    esp, 0xc
        jmp    loc_1_73AA
loc_1_737E:
        mov    al, byte ptr [ebp - 3]
        mov    byte ptr [esi + 0x10b], al
        jmp    loc_1_73A8
loc_1_7389:
        push   2
        push   0x11041
        push   0x115d
        call   sub_1_1CB6
        add    esp, 0xc
        jmp    loc_1_73AA
loc_1_739F:
        push   esi
        call   sub_1_6E6A
        add    esp, 4
loc_1_73A8:
        mov    eax, edi
loc_1_73AA:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_73B0:
        cmp    byte ptr [esp + 8], 0
        push   esi
        push   edi
        je     loc_1_73E4
        mov    esi, dword ptr [esp + 0xc]
        push   0x17
        push   esi
        call   sub_1_7237
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        jne    loc_1_7407
        push   0
        push   3
        push   dword ptr [esi + 0x110]
        call   sub_1_5BDA
        add    esp, 0xc
        push   0x14
        jmp    loc_1_73EA
loc_1_73E4:
        mov    esi, dword ptr [esp + 0xc]
        push   0x18
loc_1_73EA:
        push   esi
        call   sub_1_7237
        add    esp, 8
        mov    edi, eax
        push   0
        push   3
        push   dword ptr [esi + 0x110]
        call   sub_1_5BDA
        add    esp, 0xc
loc_1_7407:
        mov    eax, edi
        pop    edi
        pop    esi
        ret    
sub_1_740C:
        push   ebx
        push   esi
        mov    ebx, dword ptr [esp + 0xc]
        push   edi
        xor    di, di
        push   ebp
        mov    esi, dword ptr [esp + 0x1c]
loc_1_741B:
        cmp    byte ptr [ebx + 0x10f], 0
        je     loc_1_742F
        mov    byte ptr [ebx + 0x10f], 0
        xor    ebp, ebp
        jmp    loc_1_743E
loc_1_742F:
        push   dword ptr [esp + 0x18]
        push   ebx
        call   sub_1_7237
        add    esp, 8
        mov    ebp, eax
loc_1_743E:
        test   ebp, ebp
        jne    loc_1_7483
        push   esi
        push   dword ptr [esp + 0x24]
        push   ebx
        call   sub_1_8D17
        add    esp, 0xc
        mov    ebp, eax
        push   ebp
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 0x1158
        jne    loc_1_7483
        cmp    dword ptr [esp + 0x24], 0
        je     loc_1_7483
        mov    eax, dword ptr [esp + 0x24]
        push   ebx
        mov    byte ptr [eax], 1
        call   sub_1_6E6A
        add    esp, 4
        push   ebx
        call   sub_1_65A2
        add    esp, 4
        mov    ebp, eax
loc_1_7483:
        inc    di
        cmp    di, 6
        jae    loc_1_748F
        test   ebp, ebp
        jne    loc_1_741B
loc_1_748F:
        mov    eax, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_7496:
        sub    esp, 8
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0x14]
        push   edi
        push   ebp
        xor    edi, edi
        cmp    dword ptr [esi + 0xc], edi
        jne    loc_1_7649
        cmp    byte ptr [esi + 0x19], 1
        jne    loc_1_764F
        cmp    word ptr [esi + 0x16], 0x50
        jb     loc_1_7649
        push   3
        push   esi
        call   sub_1_6CF2
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        jne    loc_1_74DF
        push   0x35
        push   esi
        call   sub_1_7237
        add    esp, 8
        mov    edi, eax
loc_1_74DF:
        push   0
        push   3
        push   dword ptr [esi + 0x110]
        call   sub_1_5BDA
        add    esp, 0xc
        test   edi, edi
        jne    loc_1_7502
        push   5
        push   esi
        call   sub_1_7237
        add    esp, 8
        mov    edi, eax
loc_1_7502:
        push   0
        push   3
        push   dword ptr [esi + 0x110]
        call   sub_1_5BDA
        add    esp, 0xc
        test   edi, edi
        jne    loc_1_752E
        lea    eax, [esp + 0x11]
        push   0
        push   8
        push   eax
        push   0x2c
        push   esi
        call   sub_1_740C
        add    esp, 0x14
        mov    edi, eax
loc_1_752E:
        cmp    word ptr [esi + 0x16], 0x6e
        jb     loc_1_7569
        test   edi, edi
        jne    loc_1_7569
        lea    eax, [esp + 0x11]
        push   0
        push   8
        push   eax
        push   0x2c
        push   esi
        call   sub_1_740C
        add    esp, 0x14
        mov    edi, eax
        test   edi, edi
        jne    loc_1_7569
        lea    eax, [esp + 0x11]
        push   0
        push   8
        push   eax
        push   0x2c
        push   esi
        call   sub_1_740C
        add    esp, 0x14
        mov    edi, eax
loc_1_7569:
        mov    bx, 4
loc_1_756D:
        test   edi, edi
        jne    loc_1_758E
        movzx  eax, bx
        push   0
        push   8
        lea    ecx, [esp + eax + 0x1b]
        push   ecx
        push   0x2c
        push   esi
        call   sub_1_740C
        add    esp, 0x14
        mov    edi, eax
        dec    bx
        jne    loc_1_756D
loc_1_758E:
        xor    bx, bx
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [esi + 0xc], eax
loc_1_7598:
        test   edi, edi
        jne    loc_1_75BD
        movzx  eax, bx
        inc    bx
        push   0
        lea    ecx, [esp + eax + 0x16]
        push   8
        push   ecx
        push   0x2c
        push   esi
        call   sub_1_740C
        add    esp, 0x14
        mov    edi, eax
        cmp    bx, 2
        jb     loc_1_7598
loc_1_75BD:
        movzx  ax, byte ptr [esp + 0x12]
        xor    bx, bx
        movzx  cx, byte ptr [esp + 0x13]
        mov    word ptr [esi + 0x14], ax
        shl    ax, 8
        or     cx, ax
        mov    word ptr [esi + 0x14], ax
        mov    word ptr [esi + 0x14], cx
loc_1_75DF:
        test   edi, edi
        jne    loc_1_760A
        movzx  eax, bx
        push   0
        push   8
        lea    ebp, [eax + esi + 0x1d]
        push   ebp
        push   0x2c
        push   esi
        call   sub_1_740C
        add    esp, 0x14
        mov    edi, eax
        cmp    byte ptr [ebp], 0
        je     loc_1_760A
        inc    bx
        cmp    bx, 0x14
        jb     loc_1_75DF
loc_1_760A:
        cmp    word ptr [esi + 0x16], 0x6e
        jb     loc_1_7636
        xor    bx, bx
loc_1_7614:
        test   edi, edi
        jne    loc_1_765B
        inc    bx
        push   0
        lea    eax, [esp + 0x15]
        push   8
        push   eax
        push   0x2c
        push   esi
        call   sub_1_740C
        add    esp, 0x14
        mov    edi, eax
        cmp    bx, 2
        jb     loc_1_7614
loc_1_7636:
        test   edi, edi
        jne    loc_1_765B
        push   0
        push   esi
        call   sub_1_6CF2
        add    esp, 8
        mov    edi, eax
        jmp    loc_1_765B
loc_1_7649:
        cmp    byte ptr [esi + 0x19], 1
        je     loc_1_765B
loc_1_764F:
        xor    eax, eax
        mov    word ptr [esi + 0x14], ax
        mov    dword ptr [esi + 0xc], eax
        mov    byte ptr [esi + 0x1d], al
loc_1_765B:
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
sub_1_7665:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    esi, dword ptr [ebp + 0xc]
        mov    edi, dword ptr [ebp + 8]
        mov    byte ptr [ebp - 2], bl
        mov    al, byte ptr [esi]
        inc    esi
        mov    byte ptr [ebp - 1], al
        cmp    al, bl
        je     loc_1_76B1
loc_1_7683:
        push   0
        push   3
        push   dword ptr [edi + 0x110]
        call   sub_1_5BDA
        add    esp, 0xc
        mov    al, byte ptr [esi]
        inc    esi
        push   eax
        push   edi
        call   sub_1_7237
        add    esp, 8
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_1_775D
        dec    byte ptr [ebp - 1]
        jne    loc_1_7683
loc_1_76B1:
        mov    al, byte ptr [esi]
        inc    esi
        movzx  ecx, al
        cmp    ecx, 0xfd
        je     loc_1_76D4
        cmp    ecx, 0xfe
        je     loc_1_76E3
        cmp    ecx, 0xff
        je     loc_1_76FC
        mov    byte ptr [ebp - 2], al
        jmp    loc_1_770E
loc_1_76D4:
        mov    byte ptr [edi + 0x11e], 1
        mov    al, byte ptr [esi]
        inc    esi
        mov    byte ptr [ebp - 2], al
        jmp    loc_1_770E
loc_1_76E3:
        mov    eax, dword ptr [esi]
        test   eax, eax
        je     loc_1_770E
        push   0
        push   eax
        push   dword ptr [edi + 0x110]
        call   sub_1_5BDA
        add    esp, 0xc
        jmp    loc_1_770E
loc_1_76FC:
        push   1
        push   0x13d620
        push   edi
        call   sub_1_7191
        add    esp, 0xc
        mov    ebx, eax
loc_1_770E:
        cmp    byte ptr [ebp - 2], 0
        je     loc_1_775D
        push   0
        push   3
        push   dword ptr [edi + 0x110]
        call   sub_1_5BDA
        add    esp, 0xc
        mov    al, byte ptr [ebp - 2]
        dec    al
        lea    ecx, [ebp - 4]
        push   ecx
        cmp    al, 1
        sbb    eax, eax
        and    al, 0xf8
        add    ax, 0x10
        push   eax
        push   edi
        call   sub_1_8D17
        add    esp, 0xc
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_1_775D
        cmp    byte ptr [ebp - 2], 1
        jne    loc_1_7756
        mov    al, byte ptr [ebp - 4]
        mov    byte ptr [esi], al
        jmp    loc_1_775D
loc_1_7756:
        mov    ax, word ptr [ebp - 4]
        mov    word ptr [esi], ax
loc_1_775D:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_7766:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        push   0xc
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    loc_1_7852
        cmp    byte ptr [esi + 0x63], 1
        je     loc_1_778A
        mov    eax, dword ptr [esi + 0x6c]
        jmp    loc_1_779D
loc_1_778A:
        mov    al, byte ptr [esi + 0x1a]
        sub    al, 2
        cmp    al, 1
        sbb    eax, eax
        and    eax, 0x88b8
        add    eax, 0x15f90
loc_1_779D:
        push   0
        push   eax
        push   esi
        call   sub_1_7191
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_7852
        push   0xb
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    loc_1_7852
        cmp    byte ptr [esi + 0x63], 1
        push   0
        je     loc_1_77DE
        push   dword ptr [esi + 0x6c]
        push   esi
        call   sub_1_7191
        add    esp, 0xc
        test   eax, eax
        je     loc_1_7849
        jmp    loc_1_7852
loc_1_77DE:
        push   0xd6d8
        push   esi
        call   sub_1_7191
        add    esp, 0xc
        mov    edi, eax
        test   edi, edi
        je     loc_1_782F
        push   edi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 0x15a0
        jne    loc_1_782B
        mov    al, byte ptr [esi + 0x1a]
        push   0
        sub    al, 2
        cmp    al, 1
        sbb    eax, eax
        and    eax, 0x88b8
        add    eax, 0x15f90
        sub    eax, 0xd6d8
        push   eax
        push   esi
        call   sub_1_7191
        add    esp, 0xc
        test   eax, eax
        je     loc_1_7849
        jmp    loc_1_7852
loc_1_782B:
        mov    eax, edi
        jmp    loc_1_7852
loc_1_782F:
        mov    eax, dword ptr [esp + 0x10]
        test   eax, eax
        je     loc_1_7849
        cmp    byte ptr [esi + 0x1a], 2
        mov    dword ptr [eax], 0x44
        je     loc_1_7849
        mov    dword ptr [eax], 0x64
loc_1_7849:
        mov    dword ptr [esi + 0x34], 0
        xor    eax, eax
loc_1_7852:
        pop    edi
        pop    esi
        ret    
sub_1_7855:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    esi, dword ptr [esp + 0x1c]
        push   ebp
        mov    ebp, dword ptr [esp + 0x1c]
        push   esi
        mov    dword ptr [esp + 0x18], ebx
        push   ebp
        call   sub_1_921A
        add    esp, 8
        mov    edi, eax
        cmp    edi, ebx
        jne    loc_1_7A15
        mov    eax, dword ptr [esi + 0x10]
        mov    dword ptr [ebp + 0xcc], eax
        mov    dword ptr [ebp + 0xc4], ebx
        mov    ecx, dword ptr [esi + 0x14]
        mov    dword ptr [ebp + 0xc8], 0x800
        mov    dword ptr [ebp + 0xbc], ecx
        mov    eax, dword ptr [esi + 0x14]
        add    eax, 0x800
        mov    dword ptr [ebp + 0xc0], eax
        mov    bx, word ptr [esi + 0x40]
        mov    ax, word ptr [esi + 0x42]
        mov    byte ptr [ebp + 0xf8], 0x4d
        add    ax, bx
        mov    cl, byte ptr [ebp + 5]
        mov    word ptr [esp + 0x12], ax
        mov    byte ptr [ebp + 0xf9], cl
        mov    byte ptr [ebp + 0xfa], 3
        mov    byte ptr [ebp + 0xfb], 0x20
        mov    byte ptr [ebp + 0xfd], 0x6b
        movzx  eax, byte ptr [ebp + 0x1a]
        mov    word ptr [ebp + 0x38], 0xffff
        cmp    eax, 4
        je     loc_1_7900
        cmp    eax, 5
        je     loc_1_7909
        mov    byte ptr [ebp + 0xfc], 0xe9
        jmp    loc_1_7910
loc_1_7900:
        mov    byte ptr [ebp + 0xfc], 0xf1
        jmp    loc_1_7910
loc_1_7909:
        mov    byte ptr [ebp + 0xfc], 0xf8
loc_1_7910:
        push   1
        push   dword ptr [ebp + 0x110]
        call   sub_1_9217
        add    esp, 8
        test   al, al
        je     loc_1_792D
        push   ebp
        call   sub_1_6E6A
        add    esp, 4
loc_1_792D:
        test   edi, edi
        jne    loc_1_79CE
loc_1_7935:
        push   1
        push   ebp
        call   sub_1_8E15
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        jne    loc_1_7952
        push   ebx
        push   ebp
        call   sub_1_8ED0
        add    esp, 8
        mov    edi, eax
loc_1_7952:
        push   edi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 0x1154
        je     loc_1_7970
        push   edi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 0x115f
        jne    loc_1_79B9
loc_1_7970:
        push   0
        push   ebp
        call   sub_1_6CF2
        add    esp, 8
        push   ebx
        push   ebp
        call   sub_1_9069
        add    esp, 8
        push   1
        push   ebp
        call   sub_1_6CF2
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        jne    loc_1_79B9
        push   1
        push   ebp
        call   sub_1_8E15
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        jne    loc_1_79B9
        mov    eax, dword ptr [esi + 0x14]
        push   ebx
        mov    word ptr [eax], bx
        push   ebp
        call   sub_1_8ED0
        add    esp, 8
        mov    edi, eax
loc_1_79B9:
        inc    bx
        cmp    word ptr [esp + 0x12], bx
        jbe    loc_1_79CA
        test   edi, edi
        je     loc_1_7935
loc_1_79CA:
        test   edi, edi
        je     loc_1_79DD
loc_1_79CE:
        push   edi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 0x1150
        jne    loc_1_7A15
loc_1_79DD:
        push   edi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 0x1150
        jne    loc_1_79F0
        mov    dword ptr [esp + 0x14], edi
loc_1_79F0:
        push   ebp
        call   sub_1_6E6A
        add    esp, 4
        push   0
        push   ebp
        call   sub_1_6CF2
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        jne    loc_1_7A26
        push   ebp
        call   sub_1_8374
        add    esp, 4
        mov    edi, eax
loc_1_7A15:
        test   edi, edi
        jne    loc_1_7A26
        push   0
        push   ebp
        call   sub_1_8E15
        add    esp, 8
        mov    edi, eax
loc_1_7A26:
        cmp    dword ptr [esp + 0x14], 0
        je     loc_1_7A31
        mov    edi, dword ptr [esp + 0x14]
loc_1_7A31:
        mov    eax, edi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
sub_1_7A3B:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    esi, dword ptr [ebp + 0xc]
        mov    edi, dword ptr [ebp + 8]
        mov    ax, word ptr [esi + 0x18]
        mov    dword ptr [ebp - 8], ebx
        push   eax
        mov    dword ptr [ebp - 0xc], ebx
        push   edi
        call   sub_1_9AAF
        mov    word ptr [esi + 0x34], bx
        add    esp, 8
        mov    word ptr [esi + 0x36], bx
        mov    dword ptr [esi + 0x2c], ebx
        mov    word ptr [esi + 0x38], bx
        mov    dword ptr [esi + 0x30], ebx
        mov    ax, word ptr [edi + 0xa6]
        mov    dword ptr [edi + 0x90], ebx
        mov    word ptr [edi + 0xa4], ax
        mov    dword ptr [edi + 0x94], ebx
        mov    byte ptr [edi + 0xb2], 1
        mov    byte ptr [edi + 0xb1], bl
        push   dword ptr [esi + 0x24]
        push   dword ptr [esi + 0x20]
        push   edi
        call   sub_1_99DF
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_7D7D
        cmp    word ptr [esi + 0x18], 0x110c
        jne    loc_1_7ACA
        push   esi
        push   edi
        call   sub_1_991C
        add    esp, 8
        test   eax, eax
        jne    loc_1_7D7D
loc_1_7ACA:
        mov    eax, dword ptr [esi + 0x28]
        xor    cl, cl
        mov    word ptr [edi + 0x9e], 0
        mov    dword ptr [edi + 0x98], eax
        cmp    dword ptr [esi + 0x24], 0
        jbe    loc_1_7B07
loc_1_7AE4:
        mov    eax, dword ptr [edi + 0x98]
        test   al, 1
        jne    loc_1_7AF5
        inc    word ptr [edi + 0x9e]
loc_1_7AF5:
        shr    eax, 1
        inc    cl
        movzx  edx, cl
        mov    dword ptr [edi + 0x98], eax
        cmp    edx, dword ptr [esi + 0x24]
        jb     loc_1_7AE4
loc_1_7B07:
        mov    eax, dword ptr [esi + 0x28]
        mov    word ptr [edi + 0x9c], 0
        mov    dword ptr [edi + 0x98], eax
        mov    ax, word ptr [esi + 0x18]
        cmp    ax, 0x110d
        je     loc_1_7B3A
        cmp    ax, 0x110e
        je     loc_1_7B3A
        mov    byte ptr [ebp - 1], 0
        cmp    byte ptr [edi + 0x8c], 4
        jne    loc_1_7B52
        push   0
        jmp    loc_1_7B49
loc_1_7B3A:
        mov    byte ptr [ebp - 1], 1
        cmp    byte ptr [edi + 0x8c], 4
        jne    loc_1_7B52
        push   1
loc_1_7B49:
        push   edi
        call   sub_1_8E15
        add    esp, 8
loc_1_7B52:
        push   dword ptr [ebp - 1]
        push   dword ptr [edi + 0x110]
        call   sub_1_9217
        add    esp, 8
        test   al, al
        je     loc_1_7B70
        push   edi
        call   sub_1_6E6A
        add    esp, 4
loc_1_7B70:
        cmp    word ptr [edi + 0x9e], 0
        jne    loc_1_7B88
        cmp    word ptr [edi + 0x9c], 0
        je     loc_1_7D84
loc_1_7B88:
        cmp    word ptr [edi + 0x9c], 0
        jne    loc_1_7B9B
        push   edi
        call   sub_1_98BF
        add    esp, 4
loc_1_7B9B:
        mov    ax, word ptr [edi + 0x38]
        cmp    word ptr [edi + 0xac], ax
        jne    loc_1_7BC7
        mov    eax, dword ptr [edi + 0xa8]
        mov    ecx, dword ptr [edi + 0x34]
        cmp    eax, ecx
        jb     loc_1_7BC7
        je     loc_1_7BD8
        cmp    byte ptr [edi + 0xb4], 1
        jne    loc_1_7BC7
        sub    eax, ecx
        cmp    eax, 1
        je     loc_1_7BD8
loc_1_7BC7:
        push   edi
        call   sub_1_96A9
        add    esp, 4
        test   eax, eax
        jne    loc_1_7D7D
loc_1_7BD8:
        movzx  eax, word ptr [edi + 0x9c]
        shl    eax, 0xa
        mov    dword ptr [edi + 0x94], eax
        lea    eax, [edi + 0x94]
        push   eax
        push   dword ptr [edi + 0x90]
        push   dword ptr [esi + 0x10]
        push   dword ptr [ebp - 1]
        push   dword ptr [edi + 0x110]
        call   sub_1_5F59
        movzx  eax, word ptr [esi + 0x18]
        add    esp, 0x14
        cmp    eax, 0x110d
        je     loc_1_7C21
        cmp    eax, 0x110e
        je     loc_1_7C27
        mov    byte ptr [ebp - 0x18], 0x46
        jmp    loc_1_7C2B
loc_1_7C21:
        mov    byte ptr [ebp - 0x18], 0x45
        jmp    loc_1_7C2B
loc_1_7C27:
        mov    byte ptr [ebp - 0x18], 0x49
loc_1_7C2B:
        mov    al, byte ptr [edi + 5]
        mov    cl, byte ptr [edi + 0xae]
        mov    dl, byte ptr [edi + 0xb0]
        push   edi
        mov    byte ptr [ebp - 0x17], al
        mov    byte ptr [ebp - 0x16], cl
        mov    al, byte ptr [edi + 0xaf]
        mov    cl, byte ptr [edi + 0x4c]
        mov    byte ptr [ebp - 0x15], dl
        mov    byte ptr [ebp - 0x14], al
        mov    dl, byte ptr [edi + 0x4e]
        mov    byte ptr [ebp - 0x12], cl
        mov    byte ptr [ebp - 0x13], 3
        mov    byte ptr [ebp - 0x10], 0xff
        mov    byte ptr [ebp - 0x11], dl
        call   sub_1_9682
        add    esp, 4
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_1_7D9B
        push   dword ptr [edi + 0x110]
        call   sub_1_8CF8
        add    esp, 4
        lea    eax, [ebp - 0x18]
        push   1
        push   9
        push   eax
        push   edi
        call   sub_1_5DDF
        add    esp, 0x10
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_1_7DBF
        push   1
        push   0x1388
        push   dword ptr [edi + 0x110]
        call   sub_1_5BDA
        add    esp, 0xc
        mov    ebx, eax
        push   dword ptr [edi + 0x94]
        push   dword ptr [edi + 0x90]
        push   dword ptr [esi + 0x10]
        push   dword ptr [ebp - 1]
        push   dword ptr [edi + 0x110]
        call   sub_1_5F42
        add    esp, 0x14
        push   ebx
        call   sub_1_247A
        movzx  eax, ax
        add    esp, 4
        test   eax, eax
        je     loc_1_7CEC
        cmp    eax, 0x15a0
        je     loc_1_7D27
        jmp    loc_1_7D47
loc_1_7CEC:
        lea    eax, [ebp - 8]
        push   eax
        push   esi
        push   edi
        call   sub_1_94BB
        add    esp, 0xc
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_1_7E0F
        push   dword ptr [ebp - 8]
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 0x1155
        je     loc_1_7E16
        cmp    word ptr [edi + 0xa0], 0
        jne    loc_1_7D47
        mov    eax, ebx
        jmp    loc_1_7D7D
loc_1_7D27:
        lea    eax, [ebp - 8]
        push   eax
        push   esi
        push   edi
        call   sub_1_9408
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_7D7D
        cmp    word ptr [edi + 0xa0], 0
        je     loc_1_7E1E
loc_1_7D47:
        cmp    dword ptr [ebp - 8], 0
        je     loc_1_7D53
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0xc], eax
loc_1_7D53:
        push   dword ptr [edi + 0x110]
        call   sub_1_60D4
        add    esp, 4
        cmp    al, 0xff
        je     loc_1_7B70
        push   1
        push   0x11038
        push   0x1150
        call   sub_1_1CB6
        add    esp, 0xc
loc_1_7D7D:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_7D84:
        mov    ax, word ptr [esi + 0x18]
        push   eax
        push   edi
        call   sub_1_9373
        add    esp, 8
        test   eax, eax
        jne    loc_1_7D7D
        mov    eax, dword ptr [ebp - 0xc]
        jmp    loc_1_7D7D
loc_1_7D9B:
        push   dword ptr [edi + 0x94]
        push   dword ptr [edi + 0x90]
        push   dword ptr [esi + 0x10]
        push   dword ptr [ebp - 1]
        push   dword ptr [edi + 0x110]
        call   sub_1_5F42
        add    esp, 0x14
        mov    eax, ebx
        jmp    loc_1_7D7D
loc_1_7DBF:
        push   dword ptr [edi + 0x110]
        call   sub_1_8D0B
        add    esp, 4
        push   edi
        call   sub_1_6E6A
        add    esp, 4
        push   dword ptr [edi + 0x94]
        push   dword ptr [edi + 0x90]
        push   dword ptr [esi + 0x10]
        push   dword ptr [ebp - 1]
        push   dword ptr [edi + 0x110]
        call   sub_1_5F42
        add    esp, 0x14
        push   edi
        call   sub_1_6E6A
        add    esp, 4
        push   edi
        call   sub_1_6660
        add    esp, 4
        mov    eax, ebx
        jmp    loc_1_7D7D
loc_1_7E0F:
        mov    eax, ebx
        jmp    loc_1_7D7D
loc_1_7E16:
        mov    eax, dword ptr [ebp - 8]
        jmp    loc_1_7D7D
loc_1_7E1E:
        mov    eax, dword ptr [ebp - 8]
        jmp    loc_1_7D7D
sub_1_7E26:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        mov    ecx, dword ptr [ebp + 0xc]
        push   esi
        xor    ebx, ebx
        push   edi
        xor    esi, esi
        mov    edi, dword ptr [ebp + 8]
        movzx  eax, byte ptr [edi + 0x62]
        mov    dword ptr [edi + 0x7c], ecx
        cmp    eax, 1
        jl     loc_1_81C1
        cmp    eax, 2
        jle    loc_1_7E65
        cmp    eax, 3
        jl     loc_1_81C1
        cmp    eax, 4
        jle    loc_1_80CF
        jmp    loc_1_81C1
loc_1_7E65:
        cmp    ecx, 0x64
        ja     loc_1_7E77
        je     loc_1_7F16
        cmp    ecx, 0x44
        je     loc_1_7ED7
        jmp    loc_1_7EBC
loc_1_7E77:
        cmp    ecx, 0x66
        je     loc_1_7F55
        cmp    ecx, 0x8d
        je     loc_1_7F94
        cmp    ecx, 0x96
        je     loc_1_7FD3
        cmp    ecx, 0xcf
        je     loc_1_8012
        cmp    ecx, 0x16d
        je     loc_1_8051
        cmp    ecx, 0x219
        je     loc_1_8090
loc_1_7EBC:
        push   4
        push   0x11013
        push   0x116c
        call   sub_1_1CB6
        add    esp, 0xc
        mov    ebx, eax
        jmp    loc_1_81C1
loc_1_7ED7:
        mov    byte ptr [edi + 0x61], 1
        mov    dword ptr [edi + 0x54], 0x5500
        mov    word ptr [edi + 0x80], 0x14
        mov    dword ptr [edi + 0x64], 0x1fbd0
        mov    word ptr [edi + 0x50], 0xaa
        mov    eax, ecx
        imul   eax, eax, 0x280
        shl    ecx, 5
        mov    dword ptr [edi + 0x58], eax
        mov    eax, 0xfde8
        mov    dword ptr [edi + 0x5c], ecx
        mov    dword ptr [edi + 0x68], eax
        jmp    loc_1_81BE
loc_1_7F16:
        mov    byte ptr [edi + 0x61], 4
        mov    dword ptr [edi + 0x54], 0x4b00
        mov    word ptr [edi + 0x80], 0x1c
        mov    dword ptr [edi + 0x64], 0x186a0
        mov    word ptr [edi + 0x50], 0x96
        mov    eax, ecx
        imul   eax, eax, 0x380
        shl    ecx, 5
        mov    dword ptr [edi + 0x58], eax
        mov    eax, 0xc350
        mov    dword ptr [edi + 0x5c], ecx
        mov    dword ptr [edi + 0x68], eax
        jmp    loc_1_81BE
loc_1_7F55:
        mov    byte ptr [edi + 0x61], 2
        mov    dword ptr [edi + 0x54], 0x7f80
        mov    word ptr [edi + 0x80], 0x14
        mov    dword ptr [edi + 0x64], 0x30d40
        mov    word ptr [edi + 0x50], 0xff
        mov    eax, ecx
        imul   eax, eax, 0x280
        shl    ecx, 5
        mov    dword ptr [edi + 0x58], eax
        mov    eax, 0x186a0
        mov    dword ptr [edi + 0x5c], ecx
        mov    dword ptr [edi + 0x68], eax
        jmp    loc_1_81BE
loc_1_7F94:
        mov    byte ptr [edi + 0x61], 0xc
        mov    dword ptr [edi + 0x54], 0x5500
        mov    word ptr [edi + 0x80], 0x14
        mov    dword ptr [edi + 0x64], 0x3d090
        mov    word ptr [edi + 0x50], 0xaa
        mov    eax, ecx
        imul   eax, eax, 0x280
        shl    ecx, 5
        mov    dword ptr [edi + 0x58], eax
        mov    eax, 0x1e848
        mov    dword ptr [edi + 0x5c], ecx
        mov    dword ptr [edi + 0x68], eax
        jmp    loc_1_81BE
loc_1_7FD3:
        mov    byte ptr [edi + 0x61], 5
        mov    dword ptr [edi + 0x54], 0x4b00
        mov    word ptr [edi + 0x80], 0x1c
        mov    dword ptr [edi + 0x64], 0x1fbd0
        mov    word ptr [edi + 0x50], 0x96
        mov    eax, ecx
        imul   eax, eax, 0x380
        shl    ecx, 5
        mov    dword ptr [edi + 0x58], eax
        mov    eax, 0xfde8
        mov    dword ptr [edi + 0x5c], ecx
        mov    dword ptr [edi + 0x68], eax
        jmp    loc_1_81BE
loc_1_8012:
        mov    byte ptr [edi + 0x61], 0xd
        mov    dword ptr [edi + 0x54], 0x4b00
        mov    word ptr [edi + 0x80], 0x1c
        mov    dword ptr [edi + 0x64], 0x2bf20
        mov    word ptr [edi + 0x50], 0x96
        mov    eax, ecx
        imul   eax, eax, 0x380
        shl    ecx, 5
        mov    dword ptr [edi + 0x58], eax
        mov    eax, 0x15f90
        mov    dword ptr [edi + 0x5c], ecx
        mov    dword ptr [edi + 0x68], eax
        jmp    loc_1_81BE
loc_1_8051:
        mov    byte ptr [edi + 0x61], 3
        mov    dword ptr [edi + 0x54], 0x7f00
        mov    word ptr [edi + 0x80], 0x14
        mov    dword ptr [edi + 0x64], 0xaae60
        mov    word ptr [edi + 0x50], 0xfe
        mov    eax, ecx
        imul   eax, eax, 0x280
        shl    ecx, 5
        mov    dword ptr [edi + 0x58], eax
        mov    eax, 0x55730
        mov    dword ptr [edi + 0x5c], ecx
        mov    dword ptr [edi + 0x68], eax
        jmp    loc_1_81BE
loc_1_8090:
        mov    byte ptr [edi + 0x61], 6
        mov    dword ptr [edi + 0x54], 0x7f00
        mov    word ptr [edi + 0x80], 0x1c
        mov    dword ptr [edi + 0x64], 0x73f78
        mov    word ptr [edi + 0x50], 0xfe
        mov    eax, ecx
        imul   eax, eax, 0x380
        shl    ecx, 5
        mov    dword ptr [edi + 0x58], eax
        mov    eax, 0x3d090
        mov    dword ptr [edi + 0x5c], ecx
        mov    dword ptr [edi + 0x68], eax
        jmp    loc_1_81BE
loc_1_80CF:
        movzx  eax, byte ptr [edi + 0x61]
        cmp    eax, 9
        je     loc_1_80DF
        cmp    eax, 0xb
        je     loc_1_8112
        jmp    loc_1_8144
loc_1_80DF:
        mov    dword ptr [edi + 0x54], 0x7f80
        mov    word ptr [edi + 0x80], 0x28
        mov    word ptr [edi + 0x50], 0xff
        mov    eax, ecx
        mov    esi, ecx
        imul   eax, eax, 0x500
        imul   esi, esi, 0xa5
        mov    dword ptr [edi + 0x58], eax
        mov    eax, ecx
        shl    eax, 5
        mov    dword ptr [edi + 0x5c], eax
        jmp    loc_1_8144
loc_1_8112:
        mov    byte ptr [edi + 0x61], 0xb
        mov    dword ptr [edi + 0x54], 0x7f80
        mov    word ptr [edi + 0x80], 0x28
        mov    word ptr [edi + 0x50], 0xff
        mov    eax, ecx
        mov    esi, ecx
        imul   eax, eax, 0x500
        imul   esi, esi, 0x54
        mov    dword ptr [edi + 0x58], eax
        mov    eax, ecx
        shl    eax, 5
        mov    dword ptr [edi + 0x5c], eax
loc_1_8144:
        mov    al, byte ptr [edi + 0x61]
        cmp    al, 9
        je     loc_1_814F
        cmp    al, 0xb
        jne    loc_1_81C1
loc_1_814F:
        mov    ecx, 0xe2
        mov    eax, esi
        sub    edx, edx
        div    ecx
        mov    ecx, 0x64
        sub    edx, edx
        imul   eax, eax, 0x8a
        add    eax, 0x32
        div    ecx
        mov    ecx, 0x1c4
        sub    edx, edx
        imul   eax, eax, 0x3e8
        mov    dword ptr [edi + 0x64], eax
        mov    eax, esi
        div    ecx
        mov    ecx, 0x64
        sub    edx, edx
        imul   eax, eax, 0x8a
        add    eax, 0x32
        div    ecx
        mov    ecx, 0x384
        sub    edx, edx
        imul   eax, eax, 0x3e8
        mov    dword ptr [edi + 0x68], eax
        mov    eax, esi
        div    ecx
        mov    ecx, 0x64
        sub    edx, edx
        imul   eax, eax, 0x8a
        add    eax, 0x32
        div    ecx
        imul   eax, eax, 0x3e8
loc_1_81BE:
        mov    dword ptr [edi + 0x6c], eax
loc_1_81C1:
        push   edi
        lea    esi, [edi + 0x70]
        call   sub_1_9B44
        movzx  ecx, word ptr [edi + 0x80]
        imul   ecx, dword ptr [edi + 0x7c]
        add    esp, 4
        mov    dword ptr [esi], ecx
        cmp    word ptr [edi + 0x50], 0
        je     loc_1_821F
        movzx  eax, word ptr [edi + 0x50]
        shl    eax, 2
        sub    edx, edx
        mov    dword ptr [ebp - 8], eax
        mov    eax, ecx
        div    dword ptr [ebp - 8]
        sub    edx, edx
        mov    dword ptr [ebp - 4], eax
        mov    eax, ecx
        div    dword ptr [ebp - 8]
        test   edx, edx
        jne    loc_1_8203
        dec    dword ptr [ebp - 4]
loc_1_8203:
        mov    byte ptr [edi + 0x89], 0x80
        mov    al, byte ptr [ebp - 4]
        mov    byte ptr [edi + 0x87], al
        mov    ax, word ptr [edi + 0x50]
        dec    al
        mov    byte ptr [edi + 0x88], al
loc_1_821F:
        movzx  eax, byte ptr [edi + 0x63]
        cmp    eax, 4
        je     loc_1_824A
        cmp    eax, 6
        je     loc_1_827E
        mov    al, byte ptr [edi + 0x61]
        cmp    al, 0xc
        je     loc_1_8241
        cmp    al, 0xd
        je     loc_1_8241
        mov    byte ptr [edi + 0x8c], 2
        jmp    loc_1_8285
loc_1_8241:
        mov    byte ptr [edi + 0x8c], 5
        jmp    loc_1_8285
loc_1_824A:
        mov    eax, 3
        mov    byte ptr [edi + 0x8c], al
        cmp    byte ptr [edi + 0x19], al
        jne    loc_1_825D
        mov    byte ptr [edi + 0x40], al
loc_1_825D:
        cmp    byte ptr [edi + 0x121], 0
        jne    loc_1_8285
        push   1
        push   0x11013
        push   0x1122
        call   sub_1_1CB6
        add    esp, 0xc
        mov    ebx, eax
        jmp    loc_1_8285
loc_1_827E:
        mov    byte ptr [edi + 0x8c], 4
loc_1_8285:
        mov    edi, dword ptr [ebp + 0x10]
        test   edi, edi
        je     loc_1_8296
        add    edi, 0x24
        mov    ecx, 8
        rep movsd dword ptr es:[edi], dword ptr [esi]
loc_1_8296:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_829F:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        push   esi
        call   sub_1_66A9
        mov    edi, dword ptr [esp + 0x14]
        add    esp, 4
        add    esi, 0x34
        add    edi, 0x20
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        pop    edi
        pop    esi
        ret    
sub_1_82BE:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        push   dword ptr [ebp + 0xc]
        push   esi
        call   sub_1_9C5A
        add    esp, 8
        push   esi
        call   sub_1_653B
        add    esp, 4
        test   eax, eax
        jne    loc_1_836F
        push   0x1b
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    loc_1_836F
        push   0
        push   3
        push   dword ptr [esi + 0x110]
        call   sub_1_5BDA
        add    esp, 0xc
        mov    al, byte ptr [esi + 0x44]
        add    al, 2
        push   eax
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    loc_1_836F
        push   0
        push   3
        push   dword ptr [esi + 0x110]
        call   sub_1_5BDA
        add    esp, 0xc
        push   esi
        call   sub_1_8BC4
        add    esp, 4
        lea    eax, [ebp - 1]
        push   0
        push   8
        push   eax
        push   8
        push   esi
        call   sub_1_740C
        add    esp, 0x14
        test   eax, eax
        jne    loc_1_836F
        and    byte ptr [ebp - 1], 0x18
        shr    byte ptr [ebp - 1], 3
        mov    cl, byte ptr [ebp - 1]
        cmp    byte ptr [esi + 0x44], cl
        je     loc_1_836F
        push   1
        push   0x1100e
        push   0x1169
        call   sub_1_1CB6
        add    esp, 0xc
loc_1_836F:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_8374:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   esi
        call   sub_1_653B
        add    esp, 4
        test   eax, eax
        jne    loc_1_8452
        mov    dword ptr [esi + 0x34], 0
        cmp    byte ptr [esi + 0x19], 1
        jne    loc_1_83F6
        cmp    byte ptr [esi + 0x3c], 0
        jne    loc_1_83F6
        cmp    word ptr [esi + 0x16], 0x3c
        jae    loc_1_843C
        push   3
        push   esi
        call   sub_1_6CF2
        add    esp, 8
        test   eax, eax
        jne    loc_1_8452
        push   0x11
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    loc_1_8452
        push   0
        push   0x2710
        push   dword ptr [esi + 0x110]
        call   sub_1_5BDA
        add    esp, 0xc
        push   0
        push   esi
        call   sub_1_6CF2
        mov    word ptr [esi + 0x38], 0xffff
        add    esp, 8
        jmp    loc_1_843C
loc_1_83F6:
        push   0xd
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    loc_1_8452
        push   0
        push   3
        push   dword ptr [esi + 0x110]
        call   sub_1_5BDA
        add    esp, 0xc
        push   2
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    loc_1_8452
        push   0
        push   0x2710
        push   esi
        call   sub_1_7191
        mov    word ptr [esi + 0x38], 0
        add    esp, 0xc
loc_1_843C:
        push   0xb
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    loc_1_8452
        mov    byte ptr [esi + 0x11e], 1
loc_1_8452:
        pop    esi
        ret    
sub_1_8454:
        sub    esp, 4
        xor    eax, eax
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0x10]
        push   edi
        mov    byte ptr [esi + 0x120], al
        mov    byte ptr [esi + 0x3a], al
        mov    ax, word ptr [esi + 0x16]
        cmp    ax, 0x3f
        ja     loc_1_84D3
        cmp    ax, 0x3c
        jb     loc_1_84D3
        cmp    byte ptr [esi + 0x19], 1
        jne    loc_1_84D3
        push   esi
        call   sub_1_66A9
        add    esp, 4
        mov    edi, eax
        push   edi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 0x1122
        je     loc_1_84BD
        cmp    byte ptr [esi + 0x82], 0
        je     loc_1_84C8
        lea    eax, [esp + 0xf]
        push   eax
        push   esi
        call   sub_1_A152
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        jne    loc_1_84CC
        cmp    byte ptr [esp + 0xf], 0
        jne    loc_1_84C8
loc_1_84BD:
        push   esi
        call   sub_1_A104
        add    esp, 4
        mov    edi, eax
loc_1_84C8:
        test   edi, edi
        je     loc_1_84D3
loc_1_84CC:
        mov    eax, edi
        jmp    loc_1_865D
loc_1_84D3:
        push   esi
        call   sub_1_66A9
        add    esp, 4
        cmp    byte ptr [esi + 0x3c], 0
        jne    loc_1_852D
        cmp    byte ptr [esi + 0x3b], 0
        jne    loc_1_85A3
        cmp    byte ptr [esi + 0x19], 1
        jne    loc_1_852D
        mov    ax, word ptr [esi + 0x16]
        cmp    ax, 0x3c
        jb     loc_1_852D
        cmp    ax, 0x3f
        ja     loc_1_852D
        push   0xe
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    loc_1_865D
        push   1
        push   0xa3930
        push   esi
        call   sub_1_7191
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_865D
loc_1_852D:
        cmp    byte ptr [esi + 0x3b], 0
        jne    loc_1_85A3
        mov    al, byte ptr [esi + 0x19]
        cmp    al, 7
        je     loc_1_853E
        cmp    al, 5
        jne    loc_1_85A3
loc_1_853E:
        cmp    byte ptr [esi + 0xb5], 0
        jne    loc_1_8572
        push   0xb
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    loc_1_865D
        push   0
        push   0x9eb10
        push   esi
        call   sub_1_7191
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_865D
loc_1_8572:
        cmp    byte ptr [esi + 0x3c], 0
        jne    loc_1_85A3
        push   0xe
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    loc_1_865D
        push   1
        push   0xa3930
        push   esi
        call   sub_1_7191
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_865D
loc_1_85A3:
        push   0
        push   esi
        call   sub_1_9E52
        add    esp, 8
        test   eax, eax
        jne    loc_1_865D
        push   esi
        call   sub_1_9DFA
        add    esp, 4
        push   esi
        call   sub_1_9CBB
        add    esp, 4
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_1_8644
        cmp    byte ptr [esi + 0x3b], 0
        jne    loc_1_8632
        cmp    byte ptr [esi + 0x3c], 0
        jne    loc_1_8632
        cmp    byte ptr [esi + 0x1a], 4
        jne    loc_1_8632
        cmp    byte ptr [esi + 0x1b], 5
        jne    loc_1_8632
        cmp    byte ptr [esi + 0x19], 1
        jne    loc_1_8632
        push   0x1b
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    loc_1_865D
        push   0
        push   3
        push   dword ptr [esi + 0x110]
        call   sub_1_5BDA
        add    esp, 0xc
        push   5
        push   esi
        call   sub_1_7237
        add    esp, 8
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_1_865B
        push   0
        push   3
        push   dword ptr [esi + 0x110]
        call   sub_1_5BDA
        add    esp, 0xc
        jmp    loc_1_8644
loc_1_8632:
        mov    al, byte ptr [esi + 0x8b]
        push   eax
        push   esi
        call   sub_1_82BE
        add    esp, 8
        mov    ebx, eax
loc_1_8644:
        add    esi, 0x70
        mov    edi, dword ptr [esp + 0x18]
        add    edi, 0x20
        mov    ecx, 8
        mov    word ptr [esi - 0x38], 0xffff
        rep movsd dword ptr es:[edi], dword ptr [esi]
loc_1_865B:
        mov    eax, ebx
loc_1_865D:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_1_8664:
        sub    esp, 0xc
        mov    byte ptr [esp + 3], 0
        mov    eax, dword ptr [esp + 0x14]
        push   ebx
        mov    ebx, dword ptr [esp + 0x14]
        push   esi
        push   edi
        add    eax, 0x20
        push   ebp
        mov    edi, ebx
        mov    esi, eax
        push   ebx
        mov    byte ptr [ebx + 0x11b], 0
        mov    dword ptr [esp + 0x18], eax
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
        lea    esi, [ebx + 0xc]
        call   sub_1_A5B1
        mov    eax, dword ptr [esp + 0x28]
        add    esp, 4
        add    eax, 0x68
        mov    ecx, 0xa
        mov    edi, eax
        push   ebx
        mov    dword ptr [esp + 0x1c], eax
        rep movsd dword ptr es:[edi], dword ptr [esi]
        call   sub_1_6E6A
        add    esp, 4
        push   ebx
        call   sub_1_8A04
        add    esp, 4
        push   ebx
        call   sub_1_A595
        add    esp, 4
        push   ebx
        call   sub_1_8851
        add    esp, 4
        mov    ebp, eax
        test   ebp, ebp
        jne    loc_1_8827
        push   ebx
        call   sub_1_65A2
        add    esp, 4
        mov    ebp, eax
        test   ebp, ebp
        jne    loc_1_8827
        mov    al, 1
        mov    cl, byte ptr [ebx + 3]
        and    cl, 3
        push   ebx
        shl    al, cl
        mov    byte ptr [esp + 0x17], 1
        mov    byte ptr [ebx + 6], al
        call   sub_1_66A9
        add    esp, 4
        mov    ebp, eax
        push   ebp
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 0x115c
        jne    loc_1_872C
        push   0
        push   0x493e0
        push   ebx
        call   sub_1_7191
        add    esp, 0xc
        mov    ebp, eax
loc_1_872C:
        push   ebp
        call   sub_1_247A
        movzx  eax, ax
        add    esp, 4
        cmp    eax, 0x111e
        je     loc_1_874F
        cmp    eax, 0x1122
        je     loc_1_8760
        cmp    eax, 0x15a0
        je     loc_1_876B
        jmp    loc_1_8781
loc_1_874F:
        cmp    byte ptr [ebx + 0x19], 2
        jne    loc_1_8781
        push   ebx
        call   sub_1_66A9
        add    esp, 4
        jmp    loc_1_877F
loc_1_8760:
        push   ebx
        call   sub_1_A104
        add    esp, 4
        jmp    loc_1_877F
loc_1_876B:
        push   1
        push   0x11010
        push   0x1172
        call   sub_1_1CB6
        add    esp, 0xc
loc_1_877F:
        mov    ebp, eax
loc_1_8781:
        test   ebp, ebp
        jne    loc_1_8827
        mov    byte ptr [ebx + 0x41], 3
        push   0
        push   ebx
        call   sub_1_6CF2
        add    esp, 8
        mov    ebp, eax
        test   ebp, ebp
        jne    loc_1_8827
        mov    byte ptr [ebx + 0x19], 0xa
        push   ebx
        call   sub_1_A39C
        add    esp, 4
        mov    ebp, eax
        test   ebp, ebp
        jne    loc_1_87FA
        cmp    byte ptr [ebx + 2], 0
        je     loc_1_87CE
        mov    eax, dword ptr [esp + 0x24]
        mov    al, byte ptr [eax + 0x51]
        push   eax
        push   ebx
        call   sub_1_A1A5
        add    esp, 8
        mov    ebp, eax
loc_1_87CE:
        test   ebp, ebp
        jne    loc_1_87FA
        mov    edi, dword ptr [esp + 0x18]
        lea    esi, [ebx + 0xc]
        mov    byte ptr [ebx + 0x11b], 1
        mov    ecx, 0xa
        rep movsd dword ptr es:[edi], dword ptr [esi]
        cmp    byte ptr [ebx + 2], 0
        je     loc_1_87FA
        mov    edi, dword ptr [esp + 0x14]
        mov    esi, ebx
        mov    byte ptr [ebx + 2], 0
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
loc_1_87FA:
        mov    al, byte ptr [ebx + 0x19]
        cmp    al, 2
        je     loc_1_8811
        cmp    al, 4
        je     loc_1_8811
        cmp    al, 6
        je     loc_1_8811
        cmp    al, 8
        je     loc_1_8811
        cmp    al, 9
        jne    loc_1_8827
loc_1_8811:
        push   2
        push   0x11010
        push   0x1165
        call   sub_1_1CB6
        add    esp, 0xc
        mov    ebp, eax
loc_1_8827:
        push   dword ptr [esp + 0x13]
        push   ebx
        call   sub_1_6500
        add    esp, 8
        lea    eax, [ebx + 0xc]
        push   ebx
        push   eax
        push   dword ptr [ebx + 0x110]
        call   sub_1_A1A4
        add    esp, 0xc
        mov    eax, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
sub_1_8851:
        push   ebx
        push   esi
        xor    si, si
        push   edi
        mov    ebx, dword ptr [esp + 0x10]
loc_1_885B:
        mov    byte ptr [ebx + 0x19], 0xa
        push   ebx
        call   sub_1_6E6A
        add    esp, 4
        push   4
        push   ebx
        call   sub_1_A607
        add    esp, 8
        mov    edi, eax
        push   edi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 0x115b
        je     loc_1_8897
        push   edi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 0x1157
        jne    loc_1_89A0
loc_1_8897:
        mov    byte ptr [ebx + 0x19], 1
        push   ebx
        call   sub_1_6E6A
        add    esp, 4
        push   4
        push   ebx
        call   sub_1_A607
        add    esp, 8
        mov    edi, eax
        push   edi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 0x115b
        je     loc_1_88D3
        push   edi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 0x1157
        jne    loc_1_89A0
loc_1_88D3:
        mov    byte ptr [ebx + 0x19], 2
        push   ebx
        call   sub_1_6E6A
        add    esp, 4
        push   4
        push   ebx
        call   sub_1_A607
        add    esp, 8
        mov    edi, eax
        push   edi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 0x115b
        je     loc_1_890F
        push   edi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 0x1157
        jne    loc_1_89A0
loc_1_890F:
        mov    byte ptr [ebx + 0x19], 0xa
        push   ebx
        call   sub_1_6E6A
        add    esp, 4
        push   3
        push   ebx
        call   sub_1_A607
        add    esp, 8
        mov    edi, eax
        push   edi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 0x115b
        je     loc_1_8947
        push   edi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 0x1157
        jne    loc_1_89A0
loc_1_8947:
        mov    byte ptr [ebx + 0x19], 1
        push   ebx
        call   sub_1_6E6A
        add    esp, 4
        push   3
        push   ebx
        call   sub_1_A607
        add    esp, 8
        mov    edi, eax
        push   edi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 0x115b
        je     loc_1_897F
        push   edi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 0x1157
        jne    loc_1_89A0
loc_1_897F:
        inc    si
        push   0
        push   0x3e8
        push   dword ptr [ebx + 0x110]
        call   sub_1_5BDA
        add    esp, 0xc
        cmp    si, 2
        jb     loc_1_885B
loc_1_89A0:
        push   edi
        call   sub_1_247A
        movzx  eax, ax
        add    esp, 4
        cmp    eax, 0x1157
        jl     loc_1_89D6
        cmp    eax, 0x1158
        jle    loc_1_89DA
        cmp    eax, 0x115b
        je     loc_1_89DA
        cmp    eax, 0x115d
        je     loc_1_89E8
        cmp    eax, 0x1162
        je     loc_1_89FE
        cmp    eax, 0x1163
        je     loc_1_89E8
loc_1_89D6:
        xor    edi, edi
        jmp    loc_1_89FE
loc_1_89DA:
        push   1
        push   0x11017
        push   0x1165
        jmp    loc_1_89F4
loc_1_89E8:
        push   1
        push   0x11017
        push   0x1166
loc_1_89F4:
        call   sub_1_1CB6
        add    esp, 0xc
        mov    edi, eax
loc_1_89FE:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_8A04:
        sub    esp, 8
        mov    byte ptr [esp + 2], 0
        mov    byte ptr [esp + 6], 0x12
        push   ebx
        mov    eax, 0x18
        mov    byte ptr [esp + 8], al
        push   esi
        mov    ebx, dword ptr [esp + 0x14]
        push   edi
        mov    byte ptr [esp + 0xf], al
        push   ebp
        lea    ecx, [esp + 0x15]
        push   0
        mov    byte ptr [esp + 0x1b], 0x80
        mov    byte ptr [esp + 0x19], 0x10
        mov    byte ptr [ebx + 0x1c], 1
        push   1
        push   ecx
        push   ebx
        call   sub_1_5DDF
        add    esp, 0x10
        mov    esi, eax
        test   esi, esi
        jne    loc_1_8A71
        lea    eax, [esp + 0x12]
        push   1
        push   eax
        push   ebx
        call   sub_1_5FD8
        add    esp, 0xc
        mov    esi, eax
        test   esi, esi
        jne    loc_1_8A71
        cmp    byte ptr [esp + 0x12], 0x90
        mov    byte ptr [ebx + 0x1c], 3
        je     loc_1_8A71
        mov    byte ptr [ebx + 0x1c], 2
loc_1_8A71:
        cmp    byte ptr [ebx + 0x1c], 3
        jne    loc_1_8B00
        test   esi, esi
        jne    loc_1_8ABB
        lea    eax, [esp + 0x14]
        push   0
        push   1
        push   eax
        push   ebx
        call   sub_1_5DDF
        add    esp, 0x10
        mov    esi, eax
        test   esi, esi
        jne    loc_1_8ABB
        lea    eax, [esp + 0x12]
        push   1
        push   eax
        push   ebx
        call   sub_1_5FD8
        add    esp, 0xc
        mov    esi, eax
        test   esi, esi
        jne    loc_1_8ABB
        mov    al, byte ptr [esp + 0x12]
        and    al, 0xf0
        cmp    al, 0x70
        jne    loc_1_8ABB
        mov    byte ptr [ebx + 0x1c], 8
loc_1_8ABB:
        cmp    byte ptr [ebx + 0x1c], 3
        jne    loc_1_8B00
        test   esi, esi
        jne    loc_1_8B00
        lea    eax, [esp + 0x16]
        push   0
        push   2
        push   eax
        push   ebx
        call   sub_1_5DDF
        add    esp, 0x10
        mov    esi, eax
        push   esi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 0x115d
        jne    loc_1_8AFC
        lea    eax, [esp + 0x12]
        push   1
        push   eax
        push   ebx
        call   sub_1_5FD8
        add    esp, 0xc
        mov    esi, eax
        jmp    loc_1_8B00
loc_1_8AFC:
        mov    byte ptr [ebx + 0x1c], 4
loc_1_8B00:
        push   dword ptr [ebx + 0x110]
        call   sub_1_384C
        add    esp, 4
        test   al, al
        jne    loc_1_8B6C
        cmp    byte ptr [ebx + 0x1c], 4
        jne    loc_1_8B6C
        xor    ebp, ebp
        test   esi, esi
        jne    loc_1_8B6C
        xor    di, di
loc_1_8B21:
        push   edi
        push   dword ptr [ebx + 0xe8]
        push   dword ptr [ebx + 0x110]
        call   sub_1_6100
        add    esp, 0xc
        push   dword ptr [ebx + 0xe8]
        push   dword ptr [ebx + 0x110]
        call   sub_1_60DF
        add    esp, 8
        xor    ecx, ecx
        mov    cl, al
        movzx  eax, di
        and    ecx, 3
        cmp    ecx, eax
        jne    loc_1_8B5A
        inc    bp
loc_1_8B5A:
        inc    di
        cmp    di, 4
        jb     loc_1_8B21
        cmp    bp, 4
        jne    loc_1_8B6C
        mov    byte ptr [ebx + 0x1c], 5
loc_1_8B6C:
        cmp    byte ptr [ebx + 0x1c], 5
        jne    loc_1_8BBA
        test   esi, esi
        jne    loc_1_8BBA
        lea    eax, [esp + 0x13]
        push   0
        push   1
        push   eax
        push   ebx
        call   sub_1_5DDF
        add    esp, 0x10
        mov    esi, eax
        test   esi, esi
        jne    loc_1_8BBA
        lea    eax, [esp + 0x12]
        push   1
        push   eax
        push   ebx
        call   sub_1_5FD8
        add    esp, 0xc
        mov    esi, eax
        test   esi, esi
        jne    loc_1_8BBA
        mov    al, byte ptr [esp + 0x12]
        and    al, 0xe0
        jne    loc_1_8BB2
        mov    byte ptr [ebx + 0x1c], 7
        jmp    loc_1_8BBA
loc_1_8BB2:
        cmp    al, 0x40
        jne    loc_1_8BBA
        mov    byte ptr [ebx + 0x1c], 6
loc_1_8BBA:
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
sub_1_8BC4:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        xor    bl, bl
        mov    esi, dword ptr [ebp + 8]
        mov    al, byte ptr [esi + 0x45]
        push   eax
        push   esi
        call   sub_1_A6A9
        add    esp, 8
        cmp    byte ptr [esi + 0x1c], 7
        jne    loc_1_8C0A
        mov    byte ptr [ebp - 8], 0x8e
        mov    al, byte ptr [esi + 3]
        push   0
        and    al, 3
        push   3
        shl    al, 5
        lea    ecx, [ebp - 8]
        or     al, 0x18
        push   ecx
        push   esi
        mov    byte ptr [ebp - 7], al
        mov    byte ptr [ebp - 6], 0xc0
        call   sub_1_5DDF
        add    esp, 0x10
loc_1_8C0A:
        mov    al, byte ptr [esi + 0x1c]
        cmp    al, 4
        je     loc_1_8C25
        cmp    al, 5
        je     loc_1_8C25
        cmp    al, 6
        je     loc_1_8C25
        cmp    al, 7
        je     loc_1_8C25
        cmp    al, 8
        jne    loc_1_8CD3
loc_1_8C25:
        cmp    byte ptr [esi + 0x19], 1
        jne    loc_1_8C3F
        movzx  eax, byte ptr [esi + 0x1a]
        cmp    eax, 4
        jl     loc_1_8C39
        cmp    eax, 5
        jle    loc_1_8C3D
loc_1_8C39:
        xor    bl, bl
        jmp    loc_1_8C3F
loc_1_8C3D:
        mov    bl, 0x1c
loc_1_8C3F:
        mov    al, byte ptr [esi + 0x45]
        or     al, bl
        push   eax
        push   dword ptr [esi + 0xe4]
        push   dword ptr [esi + 0x110]
        call   sub_1_6100
        add    esp, 0xc
        push   0
        push   dword ptr [esi + 0xe8]
        push   dword ptr [esi + 0x110]
        call   sub_1_6100
        movzx  eax, byte ptr [esi + 0x45]
        add    esp, 0xc
        test   eax, eax
        je     loc_1_8C7C
        cmp    eax, 2
        jne    loc_1_8C92
loc_1_8C7C:
        push   1
        push   dword ptr [esi + 0xe8]
        push   dword ptr [esi + 0x110]
        call   sub_1_6100
        add    esp, 0xc
loc_1_8C92:
        push   dword ptr [esi + 0x110]
        mov    byte ptr [ebp - 0xc], 0x13
        mov    byte ptr [ebp - 0xa], 0xf
        xor    eax, eax
        mov    byte ptr [ebp - 0xb], al
        mov    byte ptr [ebp - 9], al
        call   sub_1_A696
        add    esp, 4
        test   al, al
        je     loc_1_8CBE
        cmp    byte ptr [esi + 0x1c], 7
        jne    loc_1_8CBE
        or     byte ptr [ebp - 0xc], 0x80
loc_1_8CBE:
        push   0
        lea    eax, [ebp - 0xc]
        push   4
        push   eax
        push   esi
        call   sub_1_5DDF
        add    esp, 0x10
        test   eax, eax
        jne    loc_1_8CF2
loc_1_8CD3:
        mov    al, byte ptr [esi + 0x46]
        push   0
        push   3
        lea    ecx, [ebp - 4]
        push   ecx
        mov    byte ptr [ebp - 3], al
        mov    byte ptr [ebp - 4], 3
        mov    byte ptr [ebp - 2], 2
        push   esi
        call   sub_1_5DDF
        add    esp, 0x10
loc_1_8CF2:
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_8CF8:
        mov    eax, dword ptr [esp + 4]
        mov    byte ptr [eax + 0x4af], 0
        mov    byte ptr [eax + 0x4b0], 1
        ret    
sub_1_8D0B:
        mov    eax, dword ptr [esp + 4]
        mov    byte ptr [eax + 0x4b0], 0
        ret    
sub_1_8D17:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    word ptr [esp + 0xe], bx
        push   ebp
        mov    esi, dword ptr [esp + 0x18]
        push   esi
        call   sub_1_A705
        add    esp, 4
        test   eax, eax
        jne    loc_1_8E0D
        mov    di, word ptr [esp + 0x1c]
        mov    ebp, 0x8000
loc_1_8D44:
        push   2
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    loc_1_8E0D
        push   0
        push   5
        push   dword ptr [esi + 0x110]
        call   sub_1_5BDA
        lea    eax, [esp + 0x1d]
        add    esp, 0xc
        push   eax
        push   esi
        call   sub_1_A6C9
        add    esp, 8
        test   eax, eax
        jne    loc_1_8E0D
        shr    word ptr [esp + 0x12], 1
        test   byte ptr [esp + 0x11], 0x10
        je     loc_1_8D90
        or     word ptr [esp + 0x12], bp
loc_1_8D90:
        inc    bl
        movzx  eax, di
        movzx  ecx, bl
        cmp    ecx, eax
        jb     loc_1_8D44
        cmp    di, 8
        jne    loc_1_8DA8
        shr    word ptr [esp + 0x12], 8
loc_1_8DA8:
        mov    eax, dword ptr [esp + 0x20]
        cmp    di, 0x10
        mov    cl, byte ptr [esp + 0x12]
        mov    byte ptr [eax], cl
        jne    loc_1_8DBF
        mov    cl, byte ptr [esp + 0x13]
        mov    byte ptr [eax + 1], cl
loc_1_8DBF:
        push   2
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    loc_1_8E0D
        push   0
        push   5
        push   dword ptr [esi + 0x110]
        call   sub_1_5BDA
        lea    eax, [esp + 0x1d]
        add    esp, 0xc
        push   eax
        push   esi
        call   sub_1_A6C9
        add    esp, 8
        test   eax, eax
        jne    loc_1_8E0D
        test   byte ptr [esp + 0x11], 0x10
        jne    loc_1_8E0D
        push   1
        push   0x1103b
        push   0x1158
        call   sub_1_1CB6
        add    esp, 0xc
loc_1_8E0D:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_1_8E15:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        xor    edi, edi
        mov    esi, dword ptr [ebp + 8]
        cmp    byte ptr [esi + 0x1a], 5
        jne    loc_1_8EC8
        mov    byte ptr [ebp - 2], 0x12
        mov    al, byte ptr [esi + 6]
        or     al, 0xe0
        shl    al, 2
        cmp    byte ptr [ebp + 0xc], 0
        mov    byte ptr [ebp - 1], al
        je     loc_1_8E85
        cmp    byte ptr [esi + 0x10d], 0
        jne    loc_1_8EC8
        push   edi
        lea    eax, [ebp - 2]
        or     byte ptr [ebp - 1], 1
        or     byte ptr [ebp - 1], 2
        push   2
        push   eax
        push   esi
        call   sub_1_5DDF
        add    esp, 0x10
        mov    edi, eax
        test   edi, edi
        je     loc_1_8E7C
        push   esi
        call   sub_1_6E6A
        add    esp, 4
        push   esi
        call   sub_1_6660
        add    esp, 4
        jmp    loc_1_8EC8
loc_1_8E7C:
        mov    byte ptr [esi + 0x10d], 1
        jmp    loc_1_8EC8
loc_1_8E85:
        cmp    byte ptr [esi + 0x10d], 0
        je     loc_1_8EC8
        push   0
        lea    eax, [ebp - 2]
        and    byte ptr [ebp - 1], 0xfe
        and    byte ptr [ebp - 1], 0xfd
        push   2
        push   eax
        push   esi
        call   sub_1_5DDF
        add    esp, 0x10
        mov    edi, eax
        test   edi, edi
        je     loc_1_8EC1
        push   esi
        call   sub_1_6E6A
        add    esp, 4
        push   esi
        call   sub_1_6660
        add    esp, 4
        jmp    loc_1_8EC8
loc_1_8EC1:
        mov    byte ptr [esi + 0x10d], 0
loc_1_8EC8:
        mov    eax, edi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_8ED0:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        push   edi
        mov    di, word ptr [esp + 0x14]
        push   ebp
        mov    ecx, dword ptr [esi + 0x5c]
        mov    ebx, dword ptr [esi + 0x54]
        mov    byte ptr [esi + 0xb9], 0
        movzx  eax, di
        imul   ecx, eax
        movsx  eax, bx
        cmp    eax, ecx
        jg     loc_1_8F13
        test   ebx, ebx
        je     loc_1_9011
        mov    eax, ecx
        sub    edx, edx
        div    ebx
        sub    edx, edx
        mov    byte ptr [esi + 0xb9], al
        mov    eax, ecx
        div    ebx
        mov    ecx, edx
loc_1_8F13:
        mov    ax, word ptr [esi + 0x4c]
        xor    ebx, ebx
        test   ax, ax
        je     loc_1_9050
        movzx  ecx, cx
        movsx  ebp, ax
        mov    eax, ecx
        sub    edx, edx
        div    ebp
        sub    edx, edx
        push   ebx
        push   esi
        mov    byte ptr [esi + 0xb8], al
        mov    eax, ecx
        div    ebp
        inc    dl
        mov    byte ptr [esi + 0xba], dl
        call   sub_1_5E6F
        add    esp, 8
        test   eax, eax
        jne    loc_1_9040
        mov    word ptr [esi + 0xd0], bx
        push   edi
        mov    word ptr [esi + 0xd2], 1
        push   esi
        call   sub_1_A934
        add    esp, 8
        test   eax, eax
        jne    loc_1_9040
        push   1
        mov    dword ptr [esi + 0xd4], ebx
        push   dword ptr [esi + 0x110]
        mov    byte ptr [esi + 0xb4], 1
        mov    byte ptr [esi + 0x10e], 1
        mov    byte ptr [esi + 0x10f], bl
        call   sub_1_7105
        add    esp, 8
        push   esi
        call   sub_1_A751
        add    esp, 4
        test   eax, eax
        jne    loc_1_9040
        push   esi
        call   sub_1_66A9
        add    esp, 4
        push   eax
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 0x115c
        jne    loc_1_902C
        mov    al, byte ptr [esi + 0x8a]
        push   ebx
        sub    al, byte ptr [esi + 0x3e]
        cmp    al, 1
        sbb    eax, eax
        inc    ax
        movzx  eax, ax
        push   dword ptr [esi + eax*4 + 0x64]
        push   esi
        call   sub_1_7191
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_9015
        mov    byte ptr [esi + 0xb4], bl
        cmp    byte ptr [esi + 0xb5], bl
        jne    loc_1_9040
        cmp    byte ptr [esi + 0xb6], bl
        jne    loc_1_9040
        push   1
        push   0x11020
        push   0x116a
        jmp    loc_1_9038
loc_1_9011:
        push   1
        jmp    loc_1_9052
loc_1_9015:
        push   esi
        call   sub_1_653B
        add    esp, 4
        push   1
        push   0x11020
        push   0x115e
        jmp    loc_1_9038
loc_1_902C:
        push   1
        push   0x11020
        push   0x1154
loc_1_9038:
        call   sub_1_1CB6
        add    esp, 0xc
loc_1_9040:
        xor    ecx, ecx
        mov    byte ptr [esi + 0x10e], cl
        mov    byte ptr [esi + 0x10f], cl
        jmp    loc_1_9064
loc_1_9050:
        push   2
loc_1_9052:
        push   0x11020
        push   0x116c
        call   sub_1_1CB6
        add    esp, 0xc
loc_1_9064:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_9069:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_1_653B
        add    esp, 4
        test   eax, eax
        jne    loc_1_9210
        mov    si, word ptr [ebp + 0xc]
        and    si, 1
        jne    loc_1_90A0
        xor    bx, bx
        push   0xb
        push   edi
        call   sub_1_7237
        add    esp, 8
        jmp    loc_1_90AF
loc_1_90A0:
        mov    bx, 1
        push   0xc
        push   edi
        call   sub_1_7237
        add    esp, 8
loc_1_90AF:
        test   eax, eax
        jne    loc_1_9210
        cmp    word ptr [edi + 0x16], 0x40
        jne    loc_1_90EC
        cmp    byte ptr [edi + 0x19], 1
        jne    loc_1_90EC
        push   0
        push   0xc8
        push   dword ptr [edi + 0x110]
        call   sub_1_5BDA
        add    esp, 0xc
        push   0x48
        push   edi
        call   sub_1_AAC3
        add    esp, 8
        test   eax, eax
        jne    loc_1_9210
loc_1_90EC:
        push   0
        push   dword ptr [edi + 0x6c]
        push   edi
        call   sub_1_7191
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_9210
        cmp    word ptr [edi + 0x16], 0x40
        jne    loc_1_91C4
        cmp    byte ptr [edi + 0x19], 1
        jne    loc_1_91C4
        push   0
        push   0x109
        push   dword ptr [edi + 0x110]
        call   sub_1_5BDA
        add    esp, 0xc
        lea    eax, [ebp - 1]
        push   0
        push   8
        push   eax
        push   0x2c
        push   edi
        call   sub_1_740C
        add    esp, 0x14
        test   eax, eax
        jne    loc_1_9210
        test   byte ptr [ebp - 1], 0x40
        jne    loc_1_91C4
        test   bx, bx
        jne    loc_1_91A6
        push   0x5d
        push   edi
        call   sub_1_AAC3
        add    esp, 8
        test   eax, eax
        jne    loc_1_9210
        push   0
        lea    eax, [ebp - 1]
        push   8
        push   eax
        push   0x2c
        push   edi
        call   sub_1_740C
        add    esp, 0x14
        test   eax, eax
        jne    loc_1_9210
        cmp    byte ptr [ebp - 1], 0
        jne    loc_1_91C4
        push   0x68
        push   edi
        call   sub_1_AAC3
        add    esp, 8
        test   eax, eax
        jne    loc_1_9210
        push   0x23
        push   edi
        call   sub_1_A9CF
        add    esp, 8
        test   eax, eax
        je     loc_1_91C4
        jmp    loc_1_9210
loc_1_91A6:
        push   0x68
        push   edi
        call   sub_1_AAC3
        add    esp, 8
        test   eax, eax
        jne    loc_1_9210
        push   0x29
        push   edi
        call   sub_1_A9CF
        add    esp, 8
        test   eax, eax
        jne    loc_1_9210
loc_1_91C4:
        test   si, si
        jne    loc_1_91E8
        cmp    byte ptr [edi + 0xb5], 0
        jne    loc_1_9207
        push   1
        push   0x1102b
        push   0x1168
        call   sub_1_1CB6
        add    esp, 0xc
        jmp    loc_1_9210
loc_1_91E8:
        cmp    byte ptr [edi + 0xb6], 0
        jne    loc_1_9207
        push   2
        push   0x1102b
        push   0x1168
        call   sub_1_1CB6
        add    esp, 0xc
        jmp    loc_1_9210
loc_1_9207:
        mov    dword ptr [edi + 0x34], 0
        xor    eax, eax
loc_1_9210:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_9217:
        xor    al, al
        ret    
sub_1_921A:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [ebp - 4], 0
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_6E6A
        add    esp, 4
        push   esi
        call   sub_1_653B
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        jne    loc_1_936B
        push   esi
        call   sub_1_ADDD
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        jne    loc_1_936B
        lea    eax, [ebp - 4]
        push   eax
        push   dword ptr [ebp + 0xc]
        push   esi
        call   sub_1_AD34
        add    esp, 0xc
        mov    edi, eax
        test   edi, edi
        jne    loc_1_936B
        movzx  eax, byte ptr [esi + 0x1a]
        cmp    eax, 2
        jl     loc_1_92EC
        cmp    eax, 3
        jle    loc_1_928F
        cmp    eax, 4
        jl     loc_1_92EC
        cmp    eax, 5
        jle    loc_1_92C3
        jmp    loc_1_92EC
loc_1_928F:
        cmp    byte ptr [esi + 0x63], 6
        jne    loc_1_9299
        push   1
        jmp    loc_1_92D8
loc_1_9299:
        push   dword ptr [ebp - 4]
        push   esi
        call   sub_1_9E52
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        jne    loc_1_936B
        cmp    byte ptr [esi + 0x63], 1
        jne    loc_1_92EC
        push   dword ptr [esi + 0x7c]
        push   esi
        call   sub_1_AB6D
        add    esp, 8
        jmp    loc_1_92EA
loc_1_92C3:
        movzx  eax, byte ptr [esi + 0x63]
        cmp    eax, 1
        jl     loc_1_92EC
        cmp    eax, 2
        jle    loc_1_92D6
        cmp    eax, 4
        jne    loc_1_92EC
loc_1_92D6:
        push   2
loc_1_92D8:
        push   0x11015
        push   0x1160
        call   sub_1_1CB6
        add    esp, 0xc
loc_1_92EA:
        mov    edi, eax
loc_1_92EC:
        test   edi, edi
        jne    loc_1_936B
        push   esi
        call   sub_1_ACD6
        add    esp, 4
        mov    edi, eax
        test   edi, edi
        jne    loc_1_936B
        push   0
        push   esi
        call   sub_1_6CF2
        add    esp, 8
        push   dword ptr [ebp - 4]
        push   esi
        call   sub_1_9E52
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        jne    loc_1_936B
        cmp    byte ptr [esi + 0x63], 1
        jne    loc_1_9330
        push   dword ptr [esi + 0x7c]
        push   esi
        call   sub_1_AB6D
        add    esp, 8
        mov    edi, eax
loc_1_9330:
        test   edi, edi
        jne    loc_1_936B
        push   esi
        call   sub_1_9DFA
        add    esp, 4
        mov    al, 8
loc_1_933F:
        test   byte ptr [esi + 7], al
        je     loc_1_9354
        push   eax
        push   esi
        call   sub_1_82BE
        add    esp, 8
        mov    edi, eax
        xor    al, al
        jmp    loc_1_9356
loc_1_9354:
        shr    al, 1
loc_1_9356:
        test   al, al
        jne    loc_1_933F
        test   edi, edi
        jne    loc_1_936B
        push   1
        push   esi
        call   sub_1_6CF2
        add    esp, 8
        mov    edi, eax
loc_1_936B:
        mov    eax, edi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_9373:
        push   esi
        xor    eax, eax
        cmp    word ptr [esp + 0xc], 0x110b
        push   edi
        jne    loc_1_9405
        mov    edi, dword ptr [esp + 0xc]
        cmp    byte ptr [edi + 0xb4], 1
        jne    loc_1_93C0
        push   edi
        call   sub_1_66A9
        add    esp, 4
        mov    esi, eax
        test   esi, esi
        je     loc_1_93B3
        push   esi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 0x115c
        je     loc_1_93B3
        mov    eax, esi
        jmp    loc_1_9405
loc_1_93B3:
        push   edi
        call   sub_1_6660
        add    esp, 4
        test   eax, eax
        jne    loc_1_9405
loc_1_93C0:
        push   0xd
        push   edi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    loc_1_9405
        push   0
        push   3
        push   dword ptr [edi + 0x110]
        call   sub_1_5BDA
        mov    ax, word ptr [edi + 0x38]
        add    esp, 0xc
        add    al, 2
        push   eax
        push   edi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    loc_1_9405
        push   0
        push   0x2710
        push   edi
        call   sub_1_7191
        add    esp, 0xc
loc_1_9405:
        pop    edi
        pop    esi
        ret    
sub_1_9408:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0x10]
        push   edi
        mov    ebx, dword ptr [esp + 0x10]
        push   ebp
        mov    ebp, dword ptr [esp + 0x1c]
        push   ebx
        mov    dword ptr [ebp], 0
        inc    word ptr [esi + 0x38]
        call   sub_1_6E6A
        add    esp, 4
        push   ebx
        call   sub_1_653B
        add    esp, 4
        test   eax, eax
        jne    loc_1_94B6
        mov    ax, word ptr [ebx + 0xac]
        push   eax
        push   ebx
        call   sub_1_A934
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        jne    loc_1_94B4
        cmp    byte ptr [ebx + 0xb5], 0
        jne    loc_1_9462
        cmp    byte ptr [ebx + 0xb6], 0
        je     loc_1_9468
loc_1_9462:
        mov    eax, dword ptr [ebx + 0x7c]
        mov    dword ptr [ebx + 0x34], eax
loc_1_9468:
        dec    word ptr [ebx + 0xa0]
        jne    loc_1_94B4
        mov    ax, word ptr [esi + 0x18]
        push   eax
        push   ebx
        call   sub_1_9373
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        jne    loc_1_94B4
        push   1
        push   0x11035
        push   0x1153
        call   sub_1_1CB6
        add    esp, 0xc
        mov    edx, 0xffffffff
        mov    dword ptr [ebp], eax
        mov    al, byte ptr [ebx + 0xaf]
        sub    al, byte ptr [ebx + 0xb3]
        mov    cl, al
        shl    edx, cl
        mov    dword ptr [esi + 0x2c], edx
loc_1_94B4:
        mov    eax, edi
loc_1_94B6:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_94BB:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 0x1c]
        mov    ebp, dword ptr [esp + 0x14]
        push   7
        lea    ebx, [ebp + 0xfe]
        push   ebx
        mov    dword ptr [edi], 0
        push   ebp
        call   sub_1_5FD8
        add    esp, 0xc
        mov    esi, eax
        push   esi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 0x115d
        jne    loc_1_9500
        push   ebp
        call   sub_1_6660
        add    esp, 4
        mov    eax, esi
        jmp    loc_1_967D
loc_1_9500:
        push   esi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 0x1163
        jne    loc_1_9583
        push   ebp
        call   sub_1_6E6A
        add    esp, 4
        push   ebp
        call   sub_1_66A9
        add    esp, 4
        push   ebp
        call   sub_1_6660
        add    esp, 4
        cmp    word ptr [ebp + 0xa0], 0
        jne    loc_1_957C
        mov    esi, dword ptr [esp + 0x18]
        mov    ax, word ptr [esi + 0x18]
        push   eax
        push   ebp
        call   sub_1_9373
        add    esp, 8
        test   eax, eax
        jne    loc_1_967D
        push   1
        push   0x11034
        push   0x1151
        call   sub_1_1CB6
        add    esp, 0xc
        mov    edx, 0xffffffff
        mov    dword ptr [edi], eax
        mov    al, byte ptr [ebp + 0xaf]
        sub    al, byte ptr [ebp + 0xb3]
        mov    cl, al
        shl    edx, cl
        mov    dword ptr [esi + 0x2c], edx
loc_1_957C:
        xor    eax, eax
        jmp    loc_1_967D
loc_1_9583:
        mov    al, byte ptr [ebp + 0xff]
        test   al, al
        jne    loc_1_95CF
        cmp    byte ptr [ebp + 0x100], 0
        jne    loc_1_95CF
        mov    byte ptr [ebp + 0xb2], 1
        mov    eax, dword ptr [ebp + 0xa8]
        mov    dword ptr [ebp + 0x34], eax
        mov    ax, word ptr [ebp + 0x9c]
        mov    word ptr [ebp + 0x9c], 0
        add    byte ptr [ebp + 0xaf], al
        movzx  eax, ax
        shl    eax, 0xa
        add    dword ptr [ebp + 0x90], eax
        xor    eax, eax
        jmp    loc_1_967D
loc_1_95CF:
        mov    esi, dword ptr [esp + 0x18]
        cmp    word ptr [esi + 0x18], 0x110a
        jne    loc_1_9605
        test   byte ptr [ebp + 0x100], 0x40
        je     loc_1_9605
        mov    dword ptr [esi + 0x2c], 0xffffffff
        push   1
        push   0x11034
        push   0x1155
        call   sub_1_1CB6
        add    esp, 0xc
        mov    dword ptr [edi], eax
        xor    eax, eax
        jmp    loc_1_967D
loc_1_9605:
        test   byte ptr [ebp + 0x100], 0x10
        jne    loc_1_9612
        test   al, 0x15
        je     loc_1_9641
loc_1_9612:
        inc    word ptr [esi + 0x36]
        test   byte ptr [ebp + 0xff], 0x10
        je     loc_1_9623
        inc    word ptr [esi + 0x34]
loc_1_9623:
        cmp    byte ptr [ebp + 0xb2], 1
        jne    loc_1_9641
        mov    byte ptr [ebp + 0xb2], 0
        mov    eax, dword ptr [ebp + 0xa8]
        inc    eax
        mov    dword ptr [ebp + 0x34], eax
        xor    eax, eax
        jmp    loc_1_967D
loc_1_9641:
        push   dword ptr [ebp + 0x110]
        mov    eax, dword ptr [ebp + 0xa8]
        mov    dword ptr [ebp + 0x34], eax
        call   sub_1_384C
        add    esp, 4
        test   al, al
        je     loc_1_9671
        push   0x1b
        call   sub_1_2C87
        add    esp, 4
        mov    dword ptr [edi], eax
        test   eax, eax
        mov    eax, 0
        jne    loc_1_967D
loc_1_9671:
        push   edi
        push   ebx
        push   esi
        push   ebp
        call   sub_1_AE46
        add    esp, 0x10
loc_1_967D:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_9682:
        push   esi
        xor    eax, eax
        mov    esi, dword ptr [esp + 8]
        cmp    byte ptr [esi + 0xb4], al
        jne    loc_1_96A7
        push   0xa
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    loc_1_96A7
        mov    byte ptr [esi + 0xb4], 1
loc_1_96A7:
        pop    esi
        ret    
sub_1_96A9:
        sub    esp, 4
        push   ebx
        push   esi
        mov    si, 0xa
        push   edi
        push   ebp
        xor    ebx, ebx
        mov    edi, dword ptr [esp + 0x18]
        mov    byte ptr [esp + 0x13], bl
        mov    byte ptr [esp + 0x12], bl
loc_1_96C2:
        cmp    byte ptr [edi + 0x11f], 0
        je     loc_1_96DD
        lea    eax, [esp + 0x12]
        push   eax
        push   edi
        call   sub_1_B438
        add    esp, 8
        mov    ebx, eax
        jmp    loc_1_9739
loc_1_96DD:
        cmp    dword ptr [edi + 0xa8], 0
        jne    loc_1_9701
        mov    ax, word ptr [edi + 0xac]
        push   eax
        push   edi
        call   sub_1_9069
        add    esp, 8
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_1_9874
loc_1_9701:
        mov    ax, word ptr [edi + 0xac]
        cmp    word ptr [edi + 0x38], ax
        je     loc_1_9739
        push   eax
        push   edi
        call   sub_1_A934
        add    esp, 8
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_1_9878
        cmp    byte ptr [edi + 0xb5], 0
        jne    loc_1_9739
        cmp    byte ptr [edi + 0xb6], 0
        jne    loc_1_9739
        mov    byte ptr [esp + 0x12], 1
loc_1_9739:
        cmp    dword ptr [edi + 0xa8], 0
        je     loc_1_987C
        cmp    byte ptr [esp + 0x12], 1
        jne    loc_1_9758
        push   edi
        call   sub_1_AFE1
        add    esp, 4
        mov    ebx, eax
loc_1_9758:
        test   ebx, ebx
        jne    loc_1_9776
        push   edi
        call   sub_1_B13E
        add    esp, 4
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_1_9776
        push   edi
        call   sub_1_AFE1
        add    esp, 4
        mov    ebx, eax
loc_1_9776:
        dec    si
        push   ebx
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 0x1168
        je     loc_1_9790
        test   si, si
        jne    loc_1_981E
loc_1_9790:
        cmp    byte ptr [esp + 0x13], 0
        jne    loc_1_981E
        push   edi
        call   sub_1_653B
        add    esp, 4
        test   eax, eax
        jne    loc_1_98B7
        cmp    byte ptr [edi + 0x19], 1
        jne    loc_1_9800
        cmp    word ptr [edi + 0x16], 0x50
        jb     loc_1_9800
        push   0x2d
        push   edi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    loc_1_98B7
        push   1
        push   0xa3930
        push   edi
        call   sub_1_7191
        add    esp, 0xc
        mov    esi, eax
        test   esi, esi
        je     loc_1_9800
        push   esi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 0x1164
        jne    loc_1_9880
        mov    byte ptr [edi + 0x120], 0
        mov    byte ptr [edi + 0x3a], 0
loc_1_9800:
        push   0
        push   edi
        call   sub_1_7766
        add    esp, 8
        test   eax, eax
        jne    loc_1_98B7
        mov    si, 0xa
        xor    ebx, ebx
        mov    byte ptr [esp + 0x13], 1
loc_1_981E:
        test   ebx, ebx
        jne    loc_1_9884
        push   dword ptr [edi + 0x110]
        mov    ebp, dword ptr [edi + 0xa8]
        sub    ebp, dword ptr [edi + 0x34]
        call   sub_1_60D4
        dec    ebp
        add    esp, 4
        cmp    al, 0xff
        jne    loc_1_9888
        test   ebp, ebp
        jl     loc_1_9847
        cmp    ebp, 0xa
        jle    loc_1_9850
loc_1_9847:
        test   si, si
        jg     loc_1_96C2
loc_1_9850:
        test   si, si
        jne    loc_1_989E
        test   ebp, ebp
        jl     loc_1_985E
        cmp    ebp, 0xa
        jle    loc_1_989E
loc_1_985E:
        push   1
        push   0x1103f
        push   0x1168
        call   sub_1_1CB6
        add    esp, 0xc
        jmp    loc_1_98B7
loc_1_9874:
        mov    eax, ebx
        jmp    loc_1_98B7
loc_1_9878:
        mov    eax, ebx
        jmp    loc_1_98B7
loc_1_987C:
        xor    eax, eax
        jmp    loc_1_98B7
loc_1_9880:
        mov    eax, esi
        jmp    loc_1_98B7
loc_1_9884:
        mov    eax, ebx
        jmp    loc_1_98B7
loc_1_9888:
        push   1
        push   0x1103f
        push   0x1150
        call   sub_1_1CB6
        add    esp, 0xc
        jmp    loc_1_98B7
loc_1_989E:
        push   edi
        call   sub_1_AFE1
        add    esp, 4
        test   eax, eax
        jne    loc_1_98B7
        mov    ecx, dword ptr [edi + 0xa8]
        dec    ecx
        cmp    ecx, dword ptr [edi + 0x34]
        jg     loc_1_989E
loc_1_98B7:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_1_98BF:
        mov    edx, dword ptr [esp + 4]
        mov    word ptr [edx + 0x9c], 0
        test   byte ptr [edx + 0x98], 1
        je     loc_1_98ED
loc_1_98D5:
        mov    eax, dword ptr [edx + 0x98]
        inc    byte ptr [edx + 0xaf]
        shr    eax, 1
        mov    dword ptr [edx + 0x98], eax
        test   al, 1
        jne    loc_1_98D5
loc_1_98ED:
        mov    cx, word ptr [edx + 0x9e]
        mov    eax, dword ptr [edx + 0x98]
        shr    eax, 1
        dec    cx
        mov    dword ptr [edx + 0x98], eax
        inc    word ptr [edx + 0x9c]
        test   al, 1
        mov    word ptr [edx + 0x9e], cx
        jne    loc_1_991B
        test   cx, cx
        jne    loc_1_98ED
loc_1_991B:
        ret    
sub_1_991C:
        push   esi
        xor    eax, eax
        xor    esi, esi
        mov    edx, dword ptr [esp + 8]
        mov    cl, byte ptr [edx + 0x86]
        cmp    cl, 3
        jne    loc_1_9935
        mov    esi, 2
loc_1_9935:
        cmp    cl, 4
        jne    loc_1_993F
        mov    esi, 4
loc_1_993F:
        test   esi, esi
        je     loc_1_9992
        movzx  ecx, word ptr [edx + 0xac]
        sub    ecx, 5
        cmp    ecx, 0x16
        ja     loc_1_9992
        movzx  ecx, byte ptr [ecx + 0x40a9c8] ; -> obj1:0x99C8
        jmp    dword ptr [ecx*4 + 0x40a994]  ; -> obj1:0x9994
loc_1_9960:
        mov    ecx, dword ptr [edx + 0xa8]
        cmp    esi, ecx
        ja     loc_1_9973
        mov    edx, dword ptr [edx + 0x7c]
        sub    edx, esi
        cmp    edx, ecx
        ja     loc_1_9992
loc_1_9973:
        mov    eax, dword ptr [esp + 0xc]
        push   1
        push   0x11054
        push   0x1153
        mov    dword ptr [eax + 0x2c], 0xffffffff
        call   sub_1_1CB6
        add    esp, 0xc
loc_1_9992:
        pop    esi
        ret    
        ; ---- dispatch table (13 entries) ----
        dd  loc_1_9960
        dd  loc_1_9960
        dd  loc_1_9960
        dd  loc_1_9960
        dd  loc_1_9960
        dd  loc_1_9960
        dd  loc_1_9960
        dd  loc_1_9960
        dd  loc_1_9960
        dd  loc_1_9960
        dd  loc_1_9960
        dd  loc_1_9960
        dd  loc_1_9992
        db  000h, 00Ch, 001h, 00Ch, 002h, 00Ch, 003h, 00Ch, 004h, 00Ch, 005h, 00Ch, 006h, 00Ch, 007h, 00Ch
        db  008h, 00Ch, 009h, 00Ch, 00Ah, 00Ch, 00Bh
sub_1_99DF:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        push   edi
        mov    ebx, dword ptr [esp + 0x14]
        cmp    dword ptr [esi + 0x58], ebx
        ja     loc_1_9A00
        push   1
        push   0x11002
        push   0x1156
        jmp    loc_1_9AA3
loc_1_9A00:
        mov    byte ptr [esi + 0xb0], 0
        mov    ecx, dword ptr [esi + 0x54]
        test   ecx, ecx
        je     loc_1_9A97
        mov    eax, ebx
        sub    edx, edx
        div    ecx
        sub    edx, edx
        mov    byte ptr [esi + 0xb0], al
        mov    eax, ebx
        div    ecx
        mov    eax, edx
        and    dl, 0x7f
        shr    eax, 7
        inc    dl
        mov    byte ptr [esi + 0xae], al
        mov    cl, dl
        mov    edi, dword ptr [esi + 0x5c]
        mov    byte ptr [esi + 0xaf], cl
        mov    byte ptr [esi + 0xb3], cl
        test   edi, edi
        mov    word ptr [esi + 0xac], 0
        je     loc_1_9A93
        mov    eax, ebx
        sub    edx, edx
        div    edi
        movzx  ecx, cl
        mov    word ptr [esi + 0xac], ax
        sub    edx, edx
        mov    eax, ebx
        dec    ecx
        div    edi
        shr    edx, 5
        and    ecx, 0x1f
        add    ecx, dword ptr [esp + 0x18]
        mov    dword ptr [esi + 0xa8], edx
        movzx  edx, word ptr [esi + 0x48]
        cmp    ecx, edx
        jbe    loc_1_9A8F
        push   2
        push   0x11002
        push   0x1156
        jmp    loc_1_9AA3
loc_1_9A8F:
        xor    eax, eax
        jmp    loc_1_9AAB
loc_1_9A93:
        push   2
        jmp    loc_1_9A99
loc_1_9A97:
        push   1
loc_1_9A99:
        push   0x11002
        push   0x116c
loc_1_9AA3:
        call   sub_1_1CB6
        add    esp, 0xc
loc_1_9AAB:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_9AAF:
        movzx  eax, word ptr [esp + 8]
        cmp    eax, 0x1109
        jl     loc_1_9B43
        cmp    eax, 0x110a
        jle    loc_1_9AE4
        cmp    eax, 0x110b
        je     loc_1_9AF3
        cmp    eax, 0x110c
        je     loc_1_9B15
        cmp    eax, 0x110d
        jl     loc_1_9B43
        cmp    eax, 0x110e
        jle    loc_1_9B2D
        jmp    loc_1_9B43
loc_1_9AE4:
        mov    eax, dword ptr [esp + 4]
        mov    word ptr [eax + 0xa6], 0
        jmp    loc_1_9B3A
loc_1_9AF3:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, 6
        mov    word ptr [eax + 0x114], 0
        mov    word ptr [eax + 0xa6], cx
        mov    word ptr [eax + 0xa0], cx
        jmp    loc_1_9B43
loc_1_9B15:
        mov    eax, dword ptr [esp + 4]
        mov    word ptr [eax + 0xa6], 0
        mov    word ptr [eax + 0xa0], 2
        jmp    loc_1_9B43
loc_1_9B2D:
        mov    eax, dword ptr [esp + 4]
        mov    word ptr [eax + 0xa6], 0xa
loc_1_9B3A:
        mov    word ptr [eax + 0xa0], 3
loc_1_9B43:
        ret    
sub_1_9B44:
        mov    ecx, dword ptr [esp + 4]
        movzx  eax, byte ptr [ecx + 0x1a]
        cmp    eax, 2
        je     loc_1_9B69
        cmp    eax, 3
        je     loc_1_9B7C
        cmp    eax, 4
        je     loc_1_9BBF
        cmp    eax, 5
        je     loc_1_9BF2
        jmp    loc_1_9C2C
loc_1_9B69:
        mov    dword ptr [ecx + 0x78], 0x14
        movzx  eax, word ptr [ecx + 0x7c]
        mov    dword ptr [ecx + 0x74], eax
        jmp    loc_1_9C2C
loc_1_9B7C:
        movzx  eax, byte ptr [ecx + 0x61]
        dec    eax
        cmp    eax, 0xc
        ja     loc_1_9BB6
        movzx  edx, byte ptr [eax + 0x40ac4d] ; -> obj1:0x9C4D
        jmp    dword ptr [edx*4 + 0x40ac2d]  ; -> obj1:0x9C2D
loc_1_9B94:
        mov    dword ptr [ecx + 0x74], 0x64
        jmp    loc_1_9BB6
loc_1_9B9D:
        mov    dword ptr [ecx + 0x74], 0x96
        jmp    loc_1_9BB6
loc_1_9BA6:
        mov    dword ptr [ecx + 0x74], 0x219
        jmp    loc_1_9BB6
loc_1_9BAF:
        mov    dword ptr [ecx + 0x74], 0xcf
loc_1_9BB6:
        mov    dword ptr [ecx + 0x78], 0x1c
        jmp    loc_1_9C2C
loc_1_9BBF:
        movzx  eax, word ptr [ecx + 0x7c]
        mov    dword ptr [ecx + 0x74], eax
        movzx  eax, byte ptr [ecx + 0x62]
        cmp    eax, 1
        je     loc_1_9BE0
        cmp    eax, 2
        je     loc_1_9BE9
        cmp    eax, 3
        je     loc_1_9C25
        cmp    eax, 4
        je     loc_1_9C25
        jmp    loc_1_9C2C
loc_1_9BE0:
        mov    dword ptr [ecx + 0x78], 0x14
        jmp    loc_1_9C2C
loc_1_9BE9:
        mov    dword ptr [ecx + 0x78], 0x1c
        jmp    loc_1_9C2C
loc_1_9BF2:
        movzx  eax, word ptr [ecx + 0x7c]
        mov    dword ptr [ecx + 0x74], eax
        movzx  eax, byte ptr [ecx + 0x62]
        cmp    eax, 1
        je     loc_1_9C13
        cmp    eax, 2
        je     loc_1_9C1C
        cmp    eax, 3
        je     loc_1_9C25
        cmp    eax, 4
        je     loc_1_9C25
        jmp    loc_1_9C2C
loc_1_9C13:
        mov    dword ptr [ecx + 0x78], 0x14
        jmp    loc_1_9C2C
loc_1_9C1C:
        mov    dword ptr [ecx + 0x78], 0x1c
        jmp    loc_1_9C2C
loc_1_9C25:
        mov    dword ptr [ecx + 0x78], 0x28
loc_1_9C2C:
        ret    
        ; ---- dispatch table (8 entries) ----
        dd  loc_1_9B94
        dd  loc_1_9B9D
        dd  loc_1_9BA6
        dd  loc_1_9B94
        dd  loc_1_9B9D
        dd  loc_1_9BA6
        dd  loc_1_9BAF
        dd  loc_1_9BB6
        db  000h, 001h, 002h, 003h, 004h, 005h, 007h, 007h, 007h, 007h, 007h, 006h, 006h
sub_1_9C5A:
        mov    al, byte ptr [esp + 8]
        mov    ecx, dword ptr [esp + 4]
        mov    byte ptr [ecx + 0x3e], al
        movzx  eax, al
        cmp    eax, 1
        je     loc_1_9C7E
        cmp    eax, 2
        je     loc_1_9C8C
        cmp    eax, 4
        je     loc_1_9C9A
        cmp    eax, 8
        je     loc_1_9CAB
        jmp    loc_1_9CBA
loc_1_9C7E:
        mov    byte ptr [ecx + 0x44], 0
        mov    byte ptr [ecx + 0x45], 2
        mov    byte ptr [ecx + 0x46], 0xff
        jmp    loc_1_9CBA
loc_1_9C8C:
        mov    byte ptr [ecx + 0x44], 2
        mov    byte ptr [ecx + 0x45], 0
        mov    byte ptr [ecx + 0x46], 0xef
        jmp    loc_1_9CBA
loc_1_9C9A:
        mov    byte ptr [ecx + 0x46], 0xdf
        mov    eax, 3
        mov    byte ptr [ecx + 0x44], al
        mov    byte ptr [ecx + 0x45], al
        jmp    loc_1_9CBA
loc_1_9CAB:
        mov    byte ptr [ecx + 0x46], 0xcf
        mov    eax, 1
        mov    byte ptr [ecx + 0x44], al
        mov    byte ptr [ecx + 0x45], al
loc_1_9CBA:
        ret    
sub_1_9CBB:
        xor    eax, eax
        mov    edx, dword ptr [esp + 4]
        movzx  ecx, byte ptr [edx + 0x1a]
        cmp    ecx, 2
        je     loc_1_9CFA
        cmp    ecx, 3
        je     loc_1_9D23
        cmp    ecx, 4
        je     loc_1_9D67
        cmp    ecx, 5
        je     loc_1_9DAA
        push   5
        push   0x11055
        push   0x116d
        call   sub_1_1CB6
        add    esp, 0xc
        jmp    loc_1_9DF9
loc_1_9CFA:
        movzx  ecx, byte ptr [edx + 0x86]
        cmp    ecx, 1
        je     loc_1_9DEB
        push   1
        push   0x11055
        push   0x116d
        call   sub_1_1CB6
        add    esp, 0xc
        jmp    loc_1_9DF9
loc_1_9D23:
        movzx  ecx, byte ptr [edx + 0x86]
        cmp    ecx, 1
        je     loc_1_9D51
        cmp    ecx, 2
        je     loc_1_9DEB
        push   2
        push   0x11055
        push   0x116d
        call   sub_1_1CB6
        add    esp, 0xc
        jmp    loc_1_9DF9
loc_1_9D51:
        mov    ecx, 1
        mov    byte ptr [edx + 0x84], cl
        mov    byte ptr [edx + 0x83], cl
        jmp    loc_1_9DF9
loc_1_9D67:
        movzx  ecx, byte ptr [edx + 0x86]
        cmp    ecx, 1
        jl     loc_1_9D82
        cmp    ecx, 2
        jle    loc_1_9DDB
        cmp    ecx, 3
        je     loc_1_9DEB
        cmp    ecx, 4
        je     loc_1_9D86
loc_1_9D82:
        push   3
        jmp    loc_1_9DC7
loc_1_9D86:
        mov    byte ptr [edx + 0x84], 1
        mov    byte ptr [edx + 0x83], 0
        push   3
        push   0x11055
        push   0x116d
        call   sub_1_1CB6
        add    esp, 0xc
        jmp    loc_1_9DF9
loc_1_9DAA:
        movzx  ecx, byte ptr [edx + 0x86]
        cmp    ecx, 1
        jl     loc_1_9DC5
        cmp    ecx, 2
        jle    loc_1_9DDB
        cmp    ecx, 3
        jl     loc_1_9DC5
        cmp    ecx, 4
        jle    loc_1_9DEB
loc_1_9DC5:
        push   4
loc_1_9DC7:
        push   0x11055
        push   0x116d
        call   sub_1_1CB6
        add    esp, 0xc
        jmp    loc_1_9DF9
loc_1_9DDB:
        mov    byte ptr [edx + 0x84], 0
        mov    byte ptr [edx + 0x83], 1
        jmp    loc_1_9DF9
loc_1_9DEB:
        mov    byte ptr [edx + 0x84], 1
        mov    byte ptr [edx + 0x83], 0
loc_1_9DF9:
        ret    
sub_1_9DFA:
        mov    eax, dword ptr [esp + 4]
        mov    cl, 8
        mov    dl, byte ptr [eax + 7]
        and    dl, byte ptr [eax + 0x60]
loc_1_9E06:
        test   dl, cl
        je     loc_1_9E14
        mov    byte ptr [eax + 0x8b], cl
        xor    cl, cl
        jmp    loc_1_9E16
loc_1_9E14:
        shr    cl, 1
loc_1_9E16:
        test   cl, cl
        jne    loc_1_9E06
        mov    cl, 1
loc_1_9E1C:
        test   dl, cl
        je     loc_1_9E2A
        mov    byte ptr [eax + 0x8a], cl
        xor    cl, cl
        jmp    loc_1_9E2C
loc_1_9E2A:
        add    cl, cl
loc_1_9E2C:
        cmp    cl, 8
        ja     loc_1_9E35
        test   cl, cl
        jne    loc_1_9E1C
loc_1_9E35:
        mov    byte ptr [eax + 0x85], 1
        mov    cl, byte ptr [eax + 0x8b]
        cmp    byte ptr [eax + 0x8a], cl
        jne    loc_1_9E51
        mov    byte ptr [eax + 0x85], 0
loc_1_9E51:
        ret    
sub_1_9E52:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        lea    eax, [ebp - 2]
        push   esi
        push   edi
        push   0
        mov    edi, dword ptr [ebp + 8]
        push   8
        push   eax
        push   0x21
        push   edi
        call   sub_1_740C
        add    esp, 0x14
        test   eax, eax
        je     loc_1_9EE0
        push   edi
        call   sub_1_66A9
        add    esp, 4
        lea    eax, [ebp - 1]
        push   0
        push   8
        push   eax
        push   8
        push   edi
        call   sub_1_740C
        add    esp, 0x14
        test   eax, eax
        jne    loc_1_A0FD
        test   byte ptr [ebp - 1], 0x40
        je     loc_1_9EC1
        test   byte ptr [ebp - 1], 0x80
        lea    esi, [edi + 0x62]
        je     loc_1_9EB6
        lea    ebx, [edi + 0x63]
        mov    byte ptr [esi], 2
        mov    byte ptr [ebx], 2
        jmp    loc_1_9F3A
loc_1_9EB6:
        lea    ebx, [edi + 0x63]
        mov    byte ptr [esi], 1
        mov    byte ptr [ebx], 2
        jmp    loc_1_9F3A
loc_1_9EC1:
        test   byte ptr [ebp - 1], 0x80
        lea    esi, [edi + 0x62]
        je     loc_1_9ED5
        lea    ebx, [edi + 0x63]
        mov    byte ptr [esi], 2
        mov    byte ptr [ebx], 1
        jmp    loc_1_9F3A
loc_1_9ED5:
        lea    ebx, [edi + 0x63]
        mov    byte ptr [esi], 1
        mov    byte ptr [ebx], 1
        jmp    loc_1_9F3A
loc_1_9EE0:
        lea    esi, [edi + 0x62]
        mov    cl, byte ptr [ebp - 2]
        and    cl, 0xf
        lea    ebx, [edi + 0x63]
        mov    dl, byte ptr [ebp - 2]
        mov    byte ptr [esi], cl
        shr    dl, 4
        test   cl, cl
        mov    byte ptr [ebx], dl
        jne    loc_1_9F3A
        movzx  ecx, byte ptr [edi + 0x1a]
        cmp    ecx, 2
        je     loc_1_9F28
        cmp    ecx, 3
        je     loc_1_9F2D
        cmp    ecx, 4
        je     loc_1_9F32
        cmp    ecx, 5
        je     loc_1_9F37
        push   1
        push   0x11029
        push   0x116b
        call   sub_1_1CB6
        add    esp, 0xc
        jmp    loc_1_9F3A
loc_1_9F28:
        mov    byte ptr [esi], 1
        jmp    loc_1_9F3A
loc_1_9F2D:
        mov    byte ptr [esi], 2
        jmp    loc_1_9F3A
loc_1_9F32:
        mov    byte ptr [esi], 4
        jmp    loc_1_9F3A
loc_1_9F37:
        mov    byte ptr [esi], 3
loc_1_9F3A:
        mov    word ptr [edi + 0x48], 0x20
        movzx  ecx, byte ptr [esi]
        mov    word ptr [edi + 0x4a], 4
        mov    word ptr [edi + 0x4c], 0x80
        mov    word ptr [edi + 0x4e], 1
        cmp    ecx, 1
        je     loc_1_9F91
        cmp    ecx, 2
        je     loc_1_9FF9
        cmp    ecx, 3
        je     loc_1_A061
        cmp    ecx, 4
        je     loc_1_A0A8
        push   2
        push   0x11029
        push   0x116b
        call   sub_1_1CB6
        add    esp, 0xc
        mov    esi, dword ptr [ebp + 0xc]
        jmp    loc_1_A0ED
loc_1_9F91:
        mov    byte ptr [edi + 0x86], 1
        mov    byte ptr [edi + 0x60], 3
        mov    word ptr [edi + 0x80], 0x14
        mov    esi, dword ptr [ebp + 0xc]
        test   esi, esi
        jne    loc_1_A0ED
        movzx  ecx, byte ptr [ebx]
        cmp    ecx, 1
        je     loc_1_9FDB
        cmp    ecx, 2
        je     loc_1_9FE5
        cmp    ecx, 4
        je     loc_1_9FEF
        push   1
        push   0x11029
        push   0x116c
        call   sub_1_1CB6
        add    esp, 0xc
        jmp    loc_1_A0ED
loc_1_9FDB:
        mov    esi, 0x8d
        jmp    loc_1_A0ED
loc_1_9FE5:
        mov    esi, 0x66
        jmp    loc_1_A0ED
loc_1_9FEF:
        mov    esi, 0x16d
        jmp    loc_1_A0ED
loc_1_9FF9:
        mov    byte ptr [edi + 0x86], 2
        mov    byte ptr [edi + 0x60], 6
        mov    word ptr [edi + 0x80], 0x1c
        mov    esi, dword ptr [ebp + 0xc]
        test   esi, esi
        jne    loc_1_A0ED
        movzx  ecx, byte ptr [ebx]
        cmp    ecx, 1
        je     loc_1_A043
        cmp    ecx, 2
        je     loc_1_A04D
        cmp    ecx, 4
        je     loc_1_A057
        push   2
        push   0x11029
        push   0x116c
        call   sub_1_1CB6
        add    esp, 0xc
        jmp    loc_1_A0ED
loc_1_A043:
        mov    esi, 0xcf
        jmp    loc_1_A0ED
loc_1_A04D:
        mov    esi, 0x96
        jmp    loc_1_A0ED
loc_1_A057:
        mov    esi, 0x219
        jmp    loc_1_A0ED
loc_1_A061:
        mov    byte ptr [edi + 0x86], 4
        mov    byte ptr [edi + 0x60], 0xc
        movzx  ecx, byte ptr [ebx]
        mov    word ptr [edi + 0x80], 0x28
        cmp    ecx, 6
        je     loc_1_A096
        push   4
        push   0x11029
        push   0x116c
        call   sub_1_1CB6
        add    esp, 0xc
        mov    esi, dword ptr [ebp + 0xc]
        jmp    loc_1_A0ED
loc_1_A096:
        mov    byte ptr [edi + 0x61], 0xb
        mov    esi, dword ptr [ebp + 0xc]
        test   esi, esi
        jne    loc_1_A0ED
        mov    esi, 0x1a6
        jmp    loc_1_A0ED
loc_1_A0A8:
        mov    byte ptr [edi + 0x86], 3
        mov    byte ptr [edi + 0x60], 6
        movzx  ecx, byte ptr [ebx]
        mov    word ptr [edi + 0x80], 0x28
        cmp    ecx, 6
        je     loc_1_A0DD
        push   3
        push   0x11029
        push   0x116c
        call   sub_1_1CB6
        add    esp, 0xc
        mov    esi, dword ptr [ebp + 0xc]
        jmp    loc_1_A0ED
loc_1_A0DD:
        mov    byte ptr [edi + 0x61], 9
        mov    esi, dword ptr [ebp + 0xc]
        test   esi, esi
        jne    loc_1_A0ED
        mov    esi, 0x12c
loc_1_A0ED:
        test   eax, eax
        jne    loc_1_A0FD
        push   0
        push   esi
        push   edi
        call   sub_1_7E26
        add    esp, 0xc
loc_1_A0FD:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_A104:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0xc]
        push   0x2d
        push   edi
        call   sub_1_7237
        add    esp, 8
        mov    esi, eax
        test   esi, esi
        jne    loc_1_A14D
        push   1
        push   0xa3930
        push   edi
        call   sub_1_7191
        add    esp, 0xc
        mov    esi, eax
        push   esi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 0x1108
        je     loc_1_A14B
        push   esi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 0x1106
        jne    loc_1_A14D
loc_1_A14B:
        xor    esi, esi
loc_1_A14D:
        mov    eax, esi
        pop    edi
        pop    esi
        ret    
sub_1_A152:
        sub    esp, 4
        push   esi
        push   3
        mov    esi, dword ptr [esp + 0x10]
        push   esi
        call   sub_1_6CF2
        add    esp, 8
        test   eax, eax
        jne    loc_1_A19F
        lea    eax, [esp + 7]
        push   0
        push   8
        push   eax
        push   0x17
        push   esi
        call   sub_1_740C
        add    esp, 0x14
        test   eax, eax
        jne    loc_1_A19F
        test   byte ptr [esp + 7], 0x20
        mov    eax, dword ptr [esp + 0x10]
        je     loc_1_A191
        mov    byte ptr [eax], 1
        jmp    loc_1_A194
loc_1_A191:
        mov    byte ptr [eax], 0
loc_1_A194:
        push   0
        push   esi
        call   sub_1_6CF2
        add    esp, 8
loc_1_A19F:
        pop    esi
        add    esp, 4
        ret    
sub_1_A1A4:
        ret    
sub_1_A1A5:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        lea    eax, [ebp - 2]
        push   esi
        push   0
        push   8
        mov    esi, dword ptr [ebp + 8]
        push   eax
        push   8
        push   esi
        call   sub_1_740C
        add    esp, 0x14
        test   eax, eax
        jne    loc_1_A396
        and    byte ptr [ebp - 2], 0x18
        movzx  eax, byte ptr [esi + 0x1a]
        shr    byte ptr [ebp - 2], 3
        cmp    eax, 2
        je     loc_1_A20B
        cmp    eax, 3
        je     loc_1_A211
        cmp    eax, 4
        je     loc_1_A217
        cmp    eax, 5
        je     loc_1_A21D
        mov    bx, word ptr [ebp - 2]
loc_1_A1EF:
        movzx  eax, byte ptr [esi + 0x1c]
        cmp    eax, 4
        jl     loc_1_A207
        cmp    eax, 6
        jle    loc_1_A223
        cmp    eax, 7
        je     loc_1_A227
        cmp    eax, 8
        je     loc_1_A223
loc_1_A207:
        mov    al, 3
        jmp    loc_1_A271
loc_1_A20B:
        mov    bx, 3
        jmp    loc_1_A1EF
loc_1_A211:
        mov    bx, 6
        jmp    loc_1_A1EF
loc_1_A217:
        mov    bx, 6
        jmp    loc_1_A1EF
loc_1_A21D:
        mov    bx, 0xe
        jmp    loc_1_A1EF
loc_1_A223:
        mov    al, 7
        jmp    loc_1_A271
loc_1_A227:
        push   0
        lea    eax, [ebp - 3]
        mov    byte ptr [ebp - 0x14], 0
        mov    byte ptr [ebp - 3], 0x2e
        push   1
        push   eax
        push   esi
        call   sub_1_5DDF
        add    esp, 0x10
        test   eax, eax
        jne    loc_1_A257
        push   0x10
        lea    eax, [ebp - 0x14]
        push   eax
        push   esi
        call   sub_1_5FD8
        add    esp, 0xc
        test   eax, eax
        je     loc_1_A264
loc_1_A257:
        push   esi
        call   sub_1_6E6A
        mov    byte ptr [ebp - 0x14], 0
        add    esp, 4
loc_1_A264:
        mov    al, byte ptr [ebp - 0x14]
        and    al, 0x80
        cmp    al, 1
        sbb    al, al
        and    al, 0xf8
        add    al, 0xf
loc_1_A271:
        and    bl, al
        movzx  eax, byte ptr [esi + 0x19]
        mov    byte ptr [esi + 7], bl
        cmp    eax, 1
        jl     loc_1_A296
        cmp    eax, 3
        jle    loc_1_A330
        cmp    eax, 5
        je     loc_1_A2D5
        cmp    eax, 7
        je     loc_1_A330
loc_1_A296:
        movzx  eax, byte ptr [ebp - 2]
        test   eax, eax
        je     loc_1_A2C6
        cmp    eax, 1
        je     loc_1_A2CB
        cmp    eax, 2
        je     loc_1_A2D0
        cmp    eax, 3
        je     loc_1_A32A
        push   2
        push   0x11042
        push   0x116e
        call   sub_1_1CB6
        add    esp, 0xc
        jmp    loc_1_A396
loc_1_A2C6:
        and    bl, 1
        jmp    loc_1_A32D
loc_1_A2CB:
        and    bl, 8
        jmp    loc_1_A32D
loc_1_A2D0:
        and    bl, 2
        jmp    loc_1_A32D
loc_1_A2D5:
        mov    ax, word ptr [esi + 0x42]
        test   ax, ax
        je     loc_1_A2EE
        cmp    byte ptr [esi + 0x1a], 3
        jne    loc_1_A330
        test   ah, 8
        jne    loc_1_A330
        and    bl, 2
        jmp    loc_1_A32D
loc_1_A2EE:
        movzx  eax, byte ptr [ebp - 2]
        test   eax, eax
        je     loc_1_A31B
        cmp    eax, 1
        je     loc_1_A320
        cmp    eax, 2
        je     loc_1_A325
        cmp    eax, 3
        je     loc_1_A32A
        push   1
        push   0x11042
        push   0x116e
        call   sub_1_1CB6
        add    esp, 0xc
        jmp    loc_1_A396
loc_1_A31B:
        and    bl, 1
        jmp    loc_1_A32D
loc_1_A320:
        and    bl, 8
        jmp    loc_1_A32D
loc_1_A325:
        and    bl, 2
        jmp    loc_1_A32D
loc_1_A32A:
        and    bl, 4
loc_1_A32D:
        mov    byte ptr [esi + 7], bl
loc_1_A330:
        push   dword ptr [esi + 0x110]
        call   sub_1_B5CF
        add    esp, 4
        test   al, al
        je     loc_1_A34C
        cmp    byte ptr [esi + 0x1a], 3
        jne    loc_1_A34C
        and    byte ptr [esi + 7], 0xf3
loc_1_A34C:
        cmp    byte ptr [ebp + 0xc], 3
        ja     loc_1_A356
        and    byte ptr [esi + 7], 0xf7
loc_1_A356:
        cmp    byte ptr [esi + 7], 0
        jne    loc_1_A372
        push   3
        push   0x11042
        push   0x116e
        call   sub_1_1CB6
        add    esp, 0xc
        jmp    loc_1_A396
loc_1_A372:
        mov    al, 8
loc_1_A374:
        test   byte ptr [esi + 7], al
        je     loc_1_A387
        push   eax
        push   esi
        call   sub_1_9C5A
        add    esp, 8
        xor    al, al
        jmp    loc_1_A389
loc_1_A387:
        shr    al, 1
loc_1_A389:
        test   al, al
        jne    loc_1_A374
        push   esi
        call   sub_1_8BC4
        add    esp, 4
loc_1_A396:
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_A39C:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    word ptr [ebp - 4], bx
        push   ebx
        push   8
        mov    esi, dword ptr [ebp + 8]
        lea    eax, [esi + 0x16]
        push   eax
        push   9
        push   esi
        call   sub_1_740C
        add    esp, 0x14
        test   eax, eax
        jne    loc_1_A58E
        push   0x20
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    loc_1_A58E
        lea    eax, [ebp - 4]
        push   eax
        push   0x10
        push   esi
        call   sub_1_8D17
        add    esp, 0xc
        test   eax, eax
        je     loc_1_A457
        push   esi
        call   sub_1_66A9
        add    esp, 4
        push   0x20
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    loc_1_A58E
        lea    eax, [ebp - 4]
        push   eax
        push   8
        push   esi
        call   sub_1_8D17
        add    esp, 0xc
        test   eax, eax
        je     loc_1_A457
        push   esi
        call   sub_1_66A9
        add    esp, 4
        push   0x18
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    loc_1_A58E
        lea    eax, [ebp - 4]
        push   eax
        push   8
        push   esi
        call   sub_1_8D17
        add    esp, 0xc
        test   eax, eax
        je     loc_1_A457
        push   esi
        mov    bl, 1
        call   sub_1_66A9
        add    esp, 4
loc_1_A457:
        cmp    word ptr [ebp - 4], 0x47
        je     loc_1_A470
        mov    ax, word ptr [ebp - 4]
        and    ax, 0xffc0
        cmp    ax, 0x11c0
        je     loc_1_A470
        test   bl, bl
        je     loc_1_A4D9
loc_1_A470:
        push   3
        push   esi
        call   sub_1_6CF2
        add    esp, 8
        test   eax, eax
        jne    loc_1_A58E
        push   9
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    loc_1_A58E
        lea    eax, [ebp - 1]
        push   eax
        push   8
        push   esi
        call   sub_1_8D17
        add    esp, 0xc
        test   eax, eax
        je     loc_1_A4B2
        push   esi
        call   sub_1_66A9
        add    esp, 4
loc_1_A4B2:
        push   0
        push   esi
        call   sub_1_6CF2
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        je     loc_1_A4D9
        push   edi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 0x1167
        je     loc_1_A4D9
        mov    eax, edi
        jmp    loc_1_A58E
loc_1_A4D9:
        cmp    word ptr [ebp - 4], 0x47
        je     loc_1_A56F
        mov    ax, word ptr [ebp - 4]
        and    ax, 0xffc0
        cmp    ax, 0x11c0
        je     loc_1_A56F
        cmp    word ptr [ebp - 4], 5
        jne    loc_1_A4FF
        mov    byte ptr [esi + 0x19], 5
        jmp    loc_1_A57D
loc_1_A4FF:
        movzx  eax, ax
        test   eax, eax
        je     loc_1_A54B
        cmp    eax, 0x140
        je     loc_1_A551
        cmp    eax, 0x180
        je     loc_1_A557
        cmp    eax, 0x1c0
        je     loc_1_A55D
        cmp    eax, 0x380
        je     loc_1_A563
        cmp    eax, 0x8880
        je     loc_1_A569
        mov    byte ptr [esi + 0x19], 9
        test   bl, bl
        je     loc_1_A57D
        cmp    byte ptr [ebp - 1], 0xa5
        jne    loc_1_A53B
        mov    byte ptr [esi + 0x19], 6
loc_1_A53B:
        test   bl, bl
        je     loc_1_A57D
        cmp    byte ptr [ebp - 1], 0xa5
        je     loc_1_A57D
        mov    byte ptr [esi + 0x19], 3
        jmp    loc_1_A57D
loc_1_A54B:
        mov    byte ptr [esi + 0x19], 4
        jmp    loc_1_A57D
loc_1_A551:
        mov    byte ptr [esi + 0x19], 5
        jmp    loc_1_A57D
loc_1_A557:
        mov    byte ptr [esi + 0x19], 2
        jmp    loc_1_A57D
loc_1_A55D:
        mov    byte ptr [esi + 0x19], 3
        jmp    loc_1_A57D
loc_1_A563:
        mov    byte ptr [esi + 0x19], 8
        jmp    loc_1_A57D
loc_1_A569:
        mov    byte ptr [esi + 0x19], 7
        jmp    loc_1_A57D
loc_1_A56F:
        cmp    byte ptr [ebp - 1], 0xa5
        mov    byte ptr [esi + 0x19], 1
        je     loc_1_A57D
        mov    byte ptr [esi + 0x19], 9
loc_1_A57D:
        push   dword ptr [ebp - 4]
        push   ebx
        push   esi
        call   sub_1_B5F5
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_A58E
loc_1_A58E:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_A595:
        mov    ecx, dword ptr [esp + 4]
        movzx  eax, byte ptr [ecx + 0x1c]
        cmp    eax, 4
        jl     loc_1_A5A7
        cmp    eax, 8
        jle    loc_1_A5AB
loc_1_A5A7:
        xor    al, al
        jmp    loc_1_A5AD
loc_1_A5AB:
        mov    al, 3
loc_1_A5AD:
        mov    byte ptr [ecx + 0x45], al
        ret    
sub_1_A5B1:
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        mov    edi, dword ptr [esp + 0x10]
        push   0x28
        push   ebx
        lea    esi, [edi + 0xc]
        push   esi
        call   sub_1_33CF
        mov    word ptr [edi + 0x14], bx
        add    esp, 0xc
        mov    word ptr [edi + 0x16], bx
        mov    dword ptr [esi], ebx
        mov    byte ptr [edi + 0x18], 3
        mov    byte ptr [edi + 0x19], 0xa
        mov    eax, 1
        mov    byte ptr [edi + 0x1d], bl
        mov    byte ptr [edi + 0x1b], al
        mov    byte ptr [edi + 0x1a], al
        push   2
        mov    byte ptr [edi + 0x1c], al
        mov    word ptr [edi + 0x10], 0x400
        mov    word ptr [edi + 0x12], 0x20
        push   edi
        call   sub_1_9C5A
        add    esp, 8
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_A607:
        movzx  eax, byte ptr [esp + 8]
        push   esi
        cmp    eax, 1
        push   edi
        je     loc_1_A64E
        cmp    eax, 3
        je     loc_1_A660
        cmp    eax, 4
        je     loc_1_A672
        cmp    eax, 5
        je     loc_1_A684
        mov    edi, dword ptr [esp + 0xc]
loc_1_A626:
        push   edi
        call   sub_1_65A2
        add    esp, 4
        mov    esi, eax
        test   esi, esi
        jne    loc_1_A649
        push   edi
        call   sub_1_66A9
        add    esp, 4
        mov    esi, eax
        push   edi
        call   sub_1_7106
        add    esp, 4
loc_1_A649:
        mov    eax, esi
        pop    edi
        pop    esi
        ret    
loc_1_A64E:
        mov    edi, dword ptr [esp + 0xc]
        mov    byte ptr [edi + 3], 0x2d
        mov    byte ptr [edi + 4], 0xc
        mov    byte ptr [edi + 5], 1
        jmp    loc_1_A626
loc_1_A660:
        mov    edi, dword ptr [esp + 0xc]
        mov    byte ptr [edi + 3], 0x8f
        mov    byte ptr [edi + 4], 0xe
        mov    byte ptr [edi + 5], 3
        jmp    loc_1_A626
loc_1_A672:
        mov    edi, dword ptr [esp + 0xc]
        mov    byte ptr [edi + 3], 0xd
        mov    byte ptr [edi + 4], 0xc
        mov    byte ptr [edi + 5], 0
        jmp    loc_1_A626
loc_1_A684:
        mov    edi, dword ptr [esp + 0xc]
        mov    byte ptr [edi + 3], 0x2d
        mov    byte ptr [edi + 4], 0xc
        mov    byte ptr [edi + 5], 0
        jmp    loc_1_A626
sub_1_A696:
        mov    eax, dword ptr [esp + 4]
        mov    al, byte ptr [eax + 0x4e6]
        sub    al, 0x21
        cmp    al, 1
        sbb    al, al
        neg    al
        ret    
sub_1_A6A9:
        mov    al, byte ptr [esp + 8]
        and    al, 3
        push   eax
        mov    eax, dword ptr [esp + 8]
        push   dword ptr [eax + 0xd8]
        push   dword ptr [eax + 0x110]
        call   sub_1_6100
        add    esp, 0xc
        ret    
sub_1_A6C9:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    byte ptr [ebp - 2], 4
        push   esi
        lea    ecx, [ebp - 2]
        mov    esi, dword ptr [ebp + 8]
        push   0
        push   2
        push   ecx
        mov    al, byte ptr [esi + 5]
        push   esi
        mov    byte ptr [ebp - 1], al
        call   sub_1_5DDF
        add    esp, 0x10
        test   eax, eax
        jne    loc_1_A700
        push   1
        push   dword ptr [ebp + 0xc]
        push   esi
        call   sub_1_5FD8
        add    esp, 0xc
loc_1_A700:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_A705:
        sub    esp, 4
        push   esi
        push   0
        mov    esi, dword ptr [esp + 0x10]
        push   5
        push   dword ptr [esi + 0x110]
        call   sub_1_5BDA
        lea    eax, [esp + 0x13]
        add    esp, 0xc
        push   eax
        push   esi
        call   sub_1_A6C9
        add    esp, 8
        test   eax, eax
        jne    loc_1_A74C
        test   byte ptr [esp + 7], 0x10
        jne    loc_1_A74C
        push   1
        push   0x11049
        push   0x115b
        call   sub_1_1CB6
        add    esp, 0xc
loc_1_A74C:
        pop    esi
        add    esp, 4
        ret    
sub_1_A751:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_1_6E6A
        add    esp, 4
        mov    al, byte ptr [esi + 0x10b]
        cmp    al, 0x80
        jae    loc_1_A773
        add    al, 0xa
        jmp    loc_1_A775
loc_1_A773:
        sub    al, 0xa
loc_1_A775:
        push   dword ptr [esi + 0x110]
        mov    byte ptr [ebp - 8], 0xf
        mov    byte ptr [ebp - 6], al
        mov    al, byte ptr [esi + 5]
        mov    byte ptr [ebp - 7], al
        call   sub_1_8CF8
        add    esp, 4
        lea    ecx, [ebp - 8]
        push   0
        push   3
        push   ecx
        push   esi
        call   sub_1_5DDF
        add    esp, 0x10
        mov    edi, eax
        test   edi, edi
        je     loc_1_A7C3
        push   dword ptr [esi + 0x110]
        call   sub_1_8D0B
        add    esp, 4
        push   esi
        call   sub_1_6E6A
        add    esp, 4
        jmp    loc_1_A92C
loc_1_A7C3:
        mov    al, byte ptr [ebp - 6]
        mov    cl, byte ptr [esi + 0x3e]
        push   1
        mov    byte ptr [esi + 0xbb], al
        cmp    byte ptr [esi + 0x8a], cl
        jne    loc_1_A7DE
        push   dword ptr [esi + 0x64]
        jmp    loc_1_A7E1
loc_1_A7DE:
        push   dword ptr [esi + 0x68]
loc_1_A7E1:
        push   dword ptr [esi + 0x110]
        call   sub_1_5BDA
        mov    byte ptr [esi + 0x10e], 0
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_A8BA
        cmp    dword ptr [esi + 0xd4], 0
        jne    loc_1_A8BA
        push   2
        lea    eax, [ebp - 2]
        push   eax
        push   esi
        call   sub_1_5FD8
        add    esp, 0xc
        mov    edi, eax
        test   edi, edi
        jne    loc_1_A8A6
        test   byte ptr [ebp - 2], 0xc0
        jne    loc_1_A888
        push   dword ptr [esi + 0x110]
        call   sub_1_60CE
        add    esp, 4
        cmp    al, 1
        je     loc_1_A85C
        mov    al, byte ptr [esi + 5]
        or     al, 0x20
        cmp    al, byte ptr [ebp - 2]
        je     loc_1_A85C
        push   1
        push   0x11051
        push   0x115d
        call   sub_1_1CB6
        add    esp, 0xc
        mov    edi, eax
loc_1_A85C:
        mov    al, byte ptr [ebp - 1]
        cmp    byte ptr [esi + 0xbb], al
        je     loc_1_A87D
        push   1
        push   0x11051
        push   0x1157
        call   sub_1_1CB6
        add    esp, 0xc
        mov    edi, eax
loc_1_A87D:
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [esi + 0x10b], al
        jmp    loc_1_A89E
loc_1_A888:
        push   1
        push   0x11051
        push   0x115d
        call   sub_1_1CB6
        add    esp, 0xc
        mov    edi, eax
loc_1_A89E:
        test   edi, edi
        je     loc_1_A92C
loc_1_A8A6:
        push   esi
        call   sub_1_6E6A
        add    esp, 4
        push   esi
        call   sub_1_653B
        add    esp, 4
        jmp    loc_1_A92A
loc_1_A8BA:
        push   dword ptr [esi + 0x94]
        push   dword ptr [esi + 0x90]
        push   dword ptr [esi + 0xcc]
        push   1
        push   dword ptr [esi + 0x110]
        mov    byte ptr [esi + 0x10f], 0
        call   sub_1_5F42
        add    esp, 0x14
        push   esi
        call   sub_1_6E6A
        add    esp, 4
        push   esi
        call   sub_1_653B
        add    esp, 4
        mov    edi, eax
        mov    eax, dword ptr [esi + 0xd4]
        test   eax, eax
        je     loc_1_A903
        mov    edi, eax
loc_1_A903:
        test   edi, edi
        je     loc_1_A916
        push   edi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 0x15a0
        jne    loc_1_A92C
loc_1_A916:
        push   1
        push   0x11051
        push   0x1154
        call   sub_1_1CB6
        add    esp, 0xc
loc_1_A92A:
        mov    edi, eax
loc_1_A92C:
        mov    eax, edi
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_A934:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0xc]
        push   edi
        call   sub_1_653B
        add    esp, 4
        test   eax, eax
        jne    loc_1_A9CC
        push   0xd
        push   edi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    loc_1_A9CC
        push   0
        push   3
        push   dword ptr [edi + 0x110]
        call   sub_1_5BDA
        mov    si, word ptr [esp + 0x1c]
        add    esp, 0xc
        mov    ax, si
        add    al, 2
        push   eax
        push   edi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    loc_1_A9CC
        push   0
        push   0x2710
        push   edi
        call   sub_1_7191
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_A9CC
        cmp    byte ptr [edi + 0xb5], 0
        mov    word ptr [edi + 0x38], si
        jne    loc_1_A9AE
        cmp    byte ptr [edi + 0xb6], 0
        je     loc_1_A9CA
loc_1_A9AE:
        push   edi
        call   sub_1_B796
        add    esp, 4
        test   al, al
        je     loc_1_A9C4
        mov    dword ptr [edi + 0x34], 0
        jmp    loc_1_A9CA
loc_1_A9C4:
        mov    eax, dword ptr [edi + 0x7c]
        mov    dword ptr [edi + 0x34], eax
loc_1_A9CA:
        xor    eax, eax
loc_1_A9CC:
        pop    edi
        pop    esi
        ret    
sub_1_A9CF:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        push   edi
        push   0x29
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    loc_1_AABF
        push   0
        push   3
        push   dword ptr [esi + 0x110]
        call   sub_1_5BDA
        add    esp, 0xc
        push   0x29
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    loc_1_AABF
        push   0
        push   3
        push   dword ptr [esi + 0x110]
        call   sub_1_5BDA
        mov    bl, byte ptr [esp + 0x20]
        add    esp, 0xc
        mov    al, bl
        and    al, 0xf
        add    al, 2
        push   eax
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    loc_1_AABF
        shr    bl, 4
        push   0
        push   3
        push   dword ptr [esi + 0x110]
        call   sub_1_5BDA
        add    esp, 0xc
        push   0x28
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    loc_1_AABF
        push   0
        push   3
        push   dword ptr [esi + 0x110]
        call   sub_1_5BDA
        add    esp, 0xc
        push   0x28
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    loc_1_AABF
        push   0
        and    bl, 0xf
        push   3
        add    bl, 2
        push   dword ptr [esi + 0x110]
        call   sub_1_5BDA
        add    esp, 0xc
        push   ebx
        push   esi
        call   sub_1_7237
        add    esp, 8
        mov    edi, eax
        mov    eax, edi
        test   edi, edi
        jne    loc_1_AABF
        push   0
        push   3
        push   dword ptr [esi + 0x110]
        call   sub_1_5BDA
        add    esp, 0xc
        mov    eax, edi
loc_1_AABF:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_AAC3:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        push   edi
        push   0x2b
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    loc_1_AB69
        push   0
        push   3
        push   dword ptr [esi + 0x110]
        call   sub_1_5BDA
        mov    bl, byte ptr [esp + 0x20]
        add    esp, 0xc
        mov    al, bl
        and    al, 0xf
        add    al, 2
        push   eax
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    loc_1_AB69
        shr    bl, 4
        push   0
        push   3
        push   dword ptr [esi + 0x110]
        call   sub_1_5BDA
        add    esp, 0xc
        push   0x2a
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    loc_1_AB69
        push   0
        and    bl, 0xf
        push   3
        add    bl, 2
        push   dword ptr [esi + 0x110]
        call   sub_1_5BDA
        add    esp, 0xc
        push   ebx
        push   esi
        call   sub_1_7237
        add    esp, 8
        mov    edi, eax
        mov    eax, edi
        test   edi, edi
        jne    loc_1_AB69
        push   0
        push   3
        push   dword ptr [esi + 0x110]
        call   sub_1_5BDA
        add    esp, 0xc
        mov    eax, edi
loc_1_AB69:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_AB6D:
        push   ebp
        xor    ecx, ecx
        mov    ebp, esp
        mov    edx, 0xa
        sub    esp, 0x20
        mov    byte ptr [ebp - 0x20], dl
        mov    byte ptr [ebp - 0x1e], cl
        mov    byte ptr [ebp - 0x1c], cl
        mov    byte ptr [ebp - 0x1f], 0x21
        mov    byte ptr [ebp - 0x1d], 0x22
        push   esi
        push   edi
        mov    eax, 0x1f
        mov    byte ptr [ebp - 0x19], cl
        mov    byte ptr [ebp - 0x1b], al
        mov    byte ptr [ebp - 0x1a], al
        mov    eax, 0x20
        lea    edi, [ebp - 0x13]
        mov    byte ptr [ebp - 0x18], al
        mov    byte ptr [ebp - 0x17], al
        xor    eax, eax
        mov    byte ptr [ebp - 0x16], cl
        mov    byte ptr [ebp - 0x15], 0xfd
        mov    byte ptr [ebp - 0x14], cl
        stosw  word ptr es:[edi], ax
        stosb  byte ptr es:[edi], al
        mov    byte ptr [ebp - 0x10], dl
        mov    byte ptr [ebp - 0xe], cl
        mov    edx, 0x28
        mov    byte ptr [ebp - 0xc], cl
        mov    byte ptr [ebp - 0xb], dl
        mov    byte ptr [ebp - 0xa], dl
        mov    eax, 0x29
        mov    byte ptr [ebp - 9], cl
        mov    byte ptr [ebp - 8], al
        mov    byte ptr [ebp - 7], al
        mov    byte ptr [ebp - 6], cl
        mov    byte ptr [ebp - 4], cl
        lea    edi, [ebp - 3]
        xor    eax, eax
        mov    byte ptr [ebp - 0xf], 0x2a
        mov    byte ptr [ebp - 0xd], 0x2b
        mov    byte ptr [ebp - 5], 0xfd
        stosw  word ptr es:[edi], ax
        stosb  byte ptr es:[edi], al
        mov    eax, dword ptr [ebp + 0xc]
        mov    esi, dword ptr [ebp + 8]
        inc    eax
        cmp    byte ptr [esi + 0x19], 1
        jne    loc_1_ACAD
        mov    di, word ptr [esi + 0x16]
        cmp    di, 0x26
        jb     loc_1_AC43
        cmp    di, dx
        ja     loc_1_AC43
        mov    ecx, eax
        shr    eax, 4
        and    cl, 0xf
        add    cl, 2
        and    al, 0xf
        add    al, 2
        mov    byte ptr [ebp - 0x16], cl
        lea    ecx, [ebp - 0x20]
        mov    byte ptr [ebp - 0x19], al
        mov    byte ptr [ebp - 0x1c], 5
        mov    byte ptr [ebp - 0x1e], 6
        push   ecx
        push   esi
        call   sub_1_7665
        add    esp, 8
        jmp    loc_1_ACCC
loc_1_AC43:
        mov    edx, eax
        shr    eax, 4
        and    dl, 0xf
        add    dl, 2
        and    al, 0xf
        add    al, 2
        mov    byte ptr [ebp - 6], dl
        cmp    di, 0x3f
        mov    byte ptr [ebp - 9], al
        jb     loc_1_AC7B
        cmp    di, 0x40
        ja     loc_1_AC7B
        mov    byte ptr [ebp - 0xc], 4
        mov    byte ptr [ebp - 0xe], 8
        lea    eax, [ebp - 0x10]
        push   eax
        push   esi
        call   sub_1_7665
        add    esp, 8
        jmp    loc_1_ACCC
loc_1_AC7B:
        cmp    di, 0x50
        jb     loc_1_AC9E
        cmp    di, 0x6e
        jae    loc_1_AC9E
        mov    byte ptr [ebp - 0xc], 5
        mov    byte ptr [ebp - 0xe], 8
        lea    eax, [ebp - 0x10]
        push   eax
        push   esi
        call   sub_1_7665
        add    esp, 8
        jmp    loc_1_ACCC
loc_1_AC9E:
        mov    al, byte ptr [esi + 0x61]
        cmp    al, 0xc
        je     loc_1_ACA9
        cmp    al, 0xd
        jne    loc_1_ACCE
loc_1_ACA9:
        push   1
        jmp    loc_1_ACBA
loc_1_ACAD:
        mov    al, byte ptr [esi + 0x61]
        cmp    al, 0xc
        je     loc_1_ACB8
        cmp    al, 0xd
        jne    loc_1_ACCE
loc_1_ACB8:
        push   2
loc_1_ACBA:
        push   0x11056
        push   0x1173
        call   sub_1_1CB6
        add    esp, 0xc
loc_1_ACCC:
        mov    ecx, eax
loc_1_ACCE:
        mov    eax, ecx
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_ACD6:
        push   ebx
        push   esi
        xor    si, si
        push   edi
        xor    ebx, ebx
        mov    edi, dword ptr [esp + 0x10]
loc_1_ACE2:
        push   0x10
        push   edi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    loc_1_AD01
        push   1
        push   0x13d620
        push   edi
        call   sub_1_7191
        add    esp, 0xc
loc_1_AD01:
        inc    si
        cmp    si, 2
        jae    loc_1_AD12
        cmp    byte ptr [edi + 0x3c], bl
        jne    loc_1_AD12
        test   eax, eax
        je     loc_1_ACE2
loc_1_AD12:
        test   eax, eax
        jne    loc_1_AD30
        cmp    byte ptr [edi + 0x3c], 0
        jne    loc_1_AD30
        push   1
        push   0x1104d
        push   0x116f
        call   sub_1_1CB6
        add    esp, 0xc
loc_1_AD30:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_AD34:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        push   0
        push   esi
        call   sub_1_6CF2
        add    esp, 8
        mov    al, byte ptr [esi + 0x61]
        cmp    al, 9
        je     loc_1_AD5F
        cmp    al, 0xb
        je     loc_1_AD5F
        push   dword ptr [esp + 0x14]
        push   esi
        call   sub_1_7766
        add    esp, 8
        jmp    loc_1_ADCB
loc_1_AD5F:
        push   0x24
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    loc_1_ADDA
        push   1
        push   0x13d620
        push   esi
        call   sub_1_7191
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_ADDA
        mov    edi, dword ptr [esp + 0x14]
        push   0
        push   0x10
        push   edi
        push   0x25
        push   esi
        call   sub_1_740C
        add    esp, 0x14
        test   eax, eax
        jne    loc_1_ADDA
        mov    ecx, dword ptr [esp + 0x10]
        cmp    word ptr [ecx + 0x40], 0
        je     loc_1_ADC6
        mov    ecx, dword ptr [esi + 0x7c]
        cmp    dword ptr [edi], ecx
        jae    loc_1_ADC2
        push   1
        push   0x11018
        push   0x1161
        call   sub_1_1CB6
        add    esp, 0xc
        jmp    loc_1_ADCB
loc_1_ADC2:
        mov    dword ptr [edi], ecx
        jmp    loc_1_ADCB
loc_1_ADC6:
        mov    ecx, dword ptr [edi]
        mov    dword ptr [esi + 0x7c], ecx
loc_1_ADCB:
        test   eax, eax
        jne    loc_1_ADDA
        push   1
        push   esi
        call   sub_1_6CF2
        add    esp, 8
loc_1_ADDA:
        pop    edi
        pop    esi
        ret    
sub_1_ADDD:
        push   ebx
        xor    eax, eax
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        movzx  ecx, byte ptr [esi + 0x1a]
        cmp    ecx, 4
        je     loc_1_ADF7
        cmp    ecx, 5
        je     loc_1_ADFB
        xor    bl, bl
        jmp    loc_1_ADFD
loc_1_ADF7:
        mov    bl, 0x11
        jmp    loc_1_ADFD
loc_1_ADFB:
        mov    bl, 0xd
loc_1_ADFD:
        test   bl, bl
        je     loc_1_AE43
        push   0x1b
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    loc_1_AE43
        push   0
        add    bl, 2
        push   3
        push   dword ptr [esi + 0x110]
        call   sub_1_5BDA
        add    esp, 0xc
        push   ebx
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    loc_1_AE43
        push   0
        push   0x2710
        push   esi
        call   sub_1_7191
        add    esp, 0xc
loc_1_AE43:
        pop    esi
        pop    ebx
        ret    
sub_1_AE46:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 0xc]
        mov    esi, dword ptr [ebp + 8]
        inc    word ptr [edi + 0x38]
        cmp    word ptr [esi + 0xa6], ax
        je     loc_1_AEB1
        cmp    word ptr [edi + 0x18], 0x110b
        je     loc_1_AE9F
        push   esi
        call   sub_1_66A9
        add    esp, 4
        mov    ebx, eax
        test   ebx, ebx
        je     loc_1_AE8E
        push   ebx
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 0x115c
        je     loc_1_AE8E
        mov    eax, ebx
        jmp    loc_1_AFDC
loc_1_AE8E:
        push   esi
        call   sub_1_6660
        add    esp, 4
        test   eax, eax
        jne    loc_1_AFDC
loc_1_AE9F:
        push   esi
        call   sub_1_AFE1
        add    esp, 4
        test   eax, eax
        je     loc_1_AEB8
        jmp    loc_1_AFDC
loc_1_AEB1:
        mov    byte ptr [esi + 0xb2], 1
loc_1_AEB8:
        mov    dx, word ptr [esi + 0xa6]
        mov    ebx, dword ptr [ebp + 0x10]
        test   dx, dx
        je     loc_1_AF29
        mov    cl, byte ptr [ebx + 5]
        cmp    byte ptr [esi + 0xb1], cl
        je     loc_1_AF29
        mov    edx, 1
        sub    cl, byte ptr [esi + 0xb3]
        shl    edx, cl
        or     dword ptr [edi + 0x30], edx
        mov    di, word ptr [esi + 0xa6]
        mov    cl, byte ptr [ebx + 5]
        mov    word ptr [esi + 0xa4], di
        mov    byte ptr [esi + 0xb1], cl
        mov    dl, byte ptr [ebx + 5]
        movzx  cx, dl
        movzx  bx, byte ptr [esi + 0xaf]
        sub    cx, bx
        mov    byte ptr [esi + 0xaf], dl
        sub    word ptr [esi + 0x9c], cx
        movzx  ecx, cx
        shl    ecx, 0xa
        add    dword ptr [esi + 0x90], ecx
        jmp    loc_1_AFDC
loc_1_AF29:
        mov    byte ptr [esi + 0x3d], 1
        mov    ax, word ptr [esi + 0xa4]
        dec    ax
        mov    word ptr [esi + 0xa4], ax
        test   dx, dx
        je     loc_1_AF57
        test   ax, ax
        je     loc_1_AF57
        mov    ax, word ptr [edi + 0x18]
        push   eax
        push   esi
        call   sub_1_B7C3
        jmp    loc_1_AFD9
loc_1_AF57:
        mov    eax, 1
        push   1
        mov    byte ptr [esi + 0xb2], 1
        mov    cl, byte ptr [ebx + 5]
        push   0x1103e
        sub    cl, byte ptr [esi + 0xb3]
        push   0x1152
        shl    eax, cl
        or     dword ptr [edi + 0x2c], eax
        call   sub_1_1CB6
        add    esp, 0xc
        mov    ecx, dword ptr [ebp + 0x14]
        mov    dword ptr [ecx], eax
        mov    cl, byte ptr [ebx + 5]
        movzx  ax, cl
        movzx  dx, byte ptr [esi + 0xaf]
        sub    ax, dx
        inc    cl
        inc    ax
        mov    byte ptr [esi + 0xaf], cl
        mov    byte ptr [esi + 0xb1], 0
        mov    cx, word ptr [esi + 0xa6]
        sub    word ptr [esi + 0x9c], ax
        movzx  eax, ax
        mov    word ptr [esi + 0xa4], cx
        shl    eax, 0xa
        add    dword ptr [esi + 0x90], eax
        mov    dx, word ptr [edi + 0x18]
        push   edx
        push   esi
        call   sub_1_9373
loc_1_AFD9:
        add    esp, 8
loc_1_AFDC:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
sub_1_AFE1:
        sub    esp, 8
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0x14]
        push   edi
        push   ebp
        xor    ebx, ebx
        mov    bp, 0xa
loc_1_AFF2:
        push   esi
        call   sub_1_9682
        add    esp, 4
        test   eax, eax
        jne    loc_1_B136
        lea    eax, [esp + 0x10]
        push   eax
        push   0x1388
        push   esi
        call   sub_1_B834
        add    esp, 0xc
        mov    edi, eax
        test   edi, edi
        je     loc_1_B033
        push   edi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 0x15a0
        jne    loc_1_B0AF
        test   edi, edi
        jne    loc_1_B053
loc_1_B033:
        test   byte ptr [esp + 0x10], 0xc0
        jne    loc_1_B053
        push   dword ptr [esi + 0x110]
        call   sub_1_60CE
        add    esp, 4
        cmp    al, 1
        jne    loc_1_B09C
        test   byte ptr [esp + 0x11], 1
        je     loc_1_B09C
loc_1_B053:
        mov    ax, word ptr [esi + 0xac]
        push   eax
        push   esi
        call   sub_1_A934
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        je     loc_1_B084
        push   edi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 0x15a0
        jne    loc_1_B0B6
        push   esi
        xor    edi, edi
        call   sub_1_6E6A
        add    esp, 4
loc_1_B084:
        cmp    byte ptr [esi + 0xb5], bl
        jne    loc_1_B0BA
        cmp    byte ptr [esi + 0xb6], bl
        jne    loc_1_B0BA
        dec    bp
        jne    loc_1_AFF2
loc_1_B09C:
        test   bp, bp
        jne    loc_1_B0DC
        push   1
        push   0x11037
        push   0x1168
        jmp    loc_1_B12E
loc_1_B0AF:
        mov    eax, edi
        jmp    loc_1_B136
loc_1_B0B6:
        mov    eax, edi
        jmp    loc_1_B136
loc_1_B0BA:
        push   esi
        call   sub_1_B796
        add    esp, 4
        test   al, al
        je     loc_1_B0D2
        mov    dword ptr [esi + 0x34], 0
        xor    eax, eax
        jmp    loc_1_B136
loc_1_B0D2:
        mov    eax, dword ptr [esi + 0x7c]
        mov    dword ptr [esi + 0x34], eax
        xor    eax, eax
        jmp    loc_1_B136
loc_1_B0DC:
        movzx  ecx, word ptr [esi + 0x50]
        movzx  eax, byte ptr [esp + 0x14]
        imul   ecx, eax
        movzx  edx, byte ptr [esp + 0x13]
        movzx  eax, byte ptr [esp + 0x15]
        add    ecx, edx
        dec    eax
        cdq    
        and    edx, 0x1f
        add    eax, edx
        sar    eax, 5
        lea    ebx, [eax + ecx*4]
        mov    ecx, dword ptr [esi + 0x7c]
        mov    dword ptr [esi + 0x34], ebx
        test   ecx, ecx
        je     loc_1_B122
        mov    eax, ebx
        sub    edx, edx
        div    ecx
        mov    word ptr [esi + 0x38], ax
        sub    edx, edx
        mov    eax, ebx
        div    ecx
        mov    eax, edi
        mov    dword ptr [esi + 0x34], edx
        jmp    loc_1_B136
loc_1_B122:
        push   1
        push   0x11037
        push   0x116c
loc_1_B12E:
        call   sub_1_1CB6
        add    esp, 0xc
loc_1_B136:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
sub_1_B13E:
        sub    esp, 4
        xor    eax, eax
        push   ebx
        push   esi
        push   edi
        mov    bl, 0
        mov    edi, dword ptr [esp + 0x14]
        push   ebp
        mov    ebp, dword ptr [edi + 0xa8]
        sub    ebp, dword ptr [edi + 0x34]
        dec    ebp
        jns    loc_1_B15D
        neg    ebp
        mov    bl, 1
loc_1_B15D:
        cmp    bl, 1
        je     loc_1_B16B
        cmp    ebp, 5
        jle    loc_1_B430
loc_1_B16B:
        push   edi
        call   sub_1_653B
        add    esp, 4
        test   eax, eax
        jne    loc_1_B430
        test   bl, bl
        jne    loc_1_B18E
        cmp    byte ptr [edi + 0x40], 1
        jne    loc_1_B18B
        sub    ebp, 3
        jmp    loc_1_B1E5
loc_1_B18B:
        dec    ebp
        jmp    loc_1_B1E5
loc_1_B18E:
        cmp    byte ptr [edi + 0xb5], 0
        jne    loc_1_B1C4
        cmp    byte ptr [edi + 0xb6], 0
        jne    loc_1_B1C4
        cmp    byte ptr [edi + 0x40], 1
        je     loc_1_B1BF
        cmp    byte ptr [edi + 0x19], 1
        jne    loc_1_B1BC
        mov    ax, word ptr [edi + 0x16]
        cmp    ax, 0x3c
        jb     loc_1_B1BC
        cmp    ax, 0x50
        jb     loc_1_B1BF
loc_1_B1BC:
        inc    ebp
        jmp    loc_1_B1E5
loc_1_B1BF:
        add    ebp, 3
        jmp    loc_1_B1E5
loc_1_B1C4:
        cmp    byte ptr [edi + 0x19], 1
        jne    loc_1_B1D4
        cmp    word ptr [edi + 0x16], 0x58
        jb     loc_1_B1D4
        inc    ebp
        jmp    loc_1_B1E5
loc_1_B1D4:
        mov    eax, dword ptr [edi + 0x7c]
        mov    ecx, 0x3e8
        imul   eax, eax, 0x2d
        sub    edx, edx
        div    ecx
        add    ebp, eax
loc_1_B1E5:
        movzx  eax, byte ptr [edi + 0x40]
        cmp    eax, 2
        je     loc_1_B255
        cmp    eax, 3
        je     loc_1_B3B8
        test   bl, bl
        jne    loc_1_B201
        test   byte ptr [edi + 0x38], 1
        je     loc_1_B20C
loc_1_B201:
        cmp    bl, 1
        jne    loc_1_B220
        test   byte ptr [edi + 0x38], 1
        je     loc_1_B220
loc_1_B20C:
        push   0xc
        push   edi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        je     loc_1_B233
        jmp    loc_1_B430
loc_1_B220:
        push   0xb
        push   edi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    loc_1_B430
loc_1_B233:
        push   ebp
        push   edi
        call   sub_1_B8CE
        add    esp, 8
        test   eax, eax
        jne    loc_1_B430
        push   edi
        call   sub_1_653B
        add    esp, 4
        test   eax, eax
        jmp    loc_1_B430
loc_1_B255:
        mov    al, 0x1a
        cmp    bl, 1
        adc    al, 0xff
        mov    byte ptr [esp + 0x13], al
        push   dword ptr [esp + 0x13]
        push   edi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    loc_1_B430
        push   0
        push   3
        push   dword ptr [edi + 0x110]
        call   sub_1_5BDA
        add    esp, 0xc
        mov    eax, ebp
        and    al, 0xf
        add    al, 2
        push   eax
        push   edi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    loc_1_B430
        sar    ebp, 4
        push   0
        push   3
        push   dword ptr [edi + 0x110]
        call   sub_1_5BDA
        add    esp, 0xc
        mov    eax, ebp
        and    al, 0xf
        add    al, 2
        push   eax
        push   edi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    loc_1_B430
        push   0
        push   3
        push   dword ptr [edi + 0x110]
        call   sub_1_5BDA
        add    esp, 0xc
        push   1
        push   dword ptr [edi + 0x6c]
        push   edi
        call   sub_1_7191
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_B430
        sar    ebp, 4
        test   ebp, ebp
        je     loc_1_B430
loc_1_B2FF:
        xor    si, si
loc_1_B302:
        push   dword ptr [esp + 0x13]
        push   edi
        cmp    si, 1
        sbb    ebx, ebx
        call   sub_1_7237
        and    ebx, 0xfe
        add    esp, 8
        inc    bx
        test   eax, eax
        jne    loc_1_B430
        push   0
        push   3
        push   dword ptr [edi + 0x110]
        call   sub_1_5BDA
        mov    ax, bx
        add    esp, 0xc
        and    al, 0xf
        add    al, 2
        push   eax
        push   edi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    loc_1_B430
        shr    bx, 4
        push   0
        push   3
        add    bl, 2
        push   dword ptr [edi + 0x110]
        call   sub_1_5BDA
        add    esp, 0xc
        push   ebx
        push   edi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    loc_1_B430
        push   0
        push   3
        push   dword ptr [edi + 0x110]
        call   sub_1_5BDA
        add    esp, 0xc
        push   1
        push   dword ptr [edi + 0x6c]
        push   edi
        call   sub_1_7191
        add    esp, 0xc
        test   eax, eax
        jne    loc_1_B430
        inc    si
        cmp    si, 2
        jb     loc_1_B302
        dec    ebp
        jne    loc_1_B2FF
        jmp    loc_1_B430
loc_1_B3B8:
        test   bl, bl
        jne    loc_1_B3CD
        push   0x23
        push   edi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        je     loc_1_B3DC
        jmp    loc_1_B430
loc_1_B3CD:
        push   0x22
        push   edi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    loc_1_B430
loc_1_B3DC:
        xor    si, si
        push   0
        push   3
        push   dword ptr [edi + 0x110]
        call   sub_1_5BDA
        add    esp, 0xc
loc_1_B3F1:
        mov    eax, ebp
        and    al, 0xf
        add    al, 2
        push   eax
        push   edi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    loc_1_B430
        inc    si
        push   0
        sar    ebp, 4
        push   3
        push   dword ptr [edi + 0x110]
        call   sub_1_5BDA
        add    esp, 0xc
        cmp    si, 3
        jb     loc_1_B3F1
        push   1
        push   dword ptr [edi + 0x6c]
        push   edi
        call   sub_1_7191
        add    esp, 0xc
loc_1_B430:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_1_B438:
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        xor    ebx, ebx
        mov    edi, dword ptr [esp + 0x10]
        mov    ax, word ptr [edi + 0x38]
        cmp    word ptr [edi + 0xac], ax
        jne    loc_1_B458
        cmp    dword ptr [edi + 0xa8], esi
        jne    loc_1_B46B
loc_1_B458:
        push   edi
        call   sub_1_653B
        add    esp, 4
        mov    esi, eax
        test   esi, esi
        jne    loc_1_B5C9
loc_1_B46B:
        cmp    dword ptr [edi + 0xa8], 0
        jne    loc_1_B4A8
        test   byte ptr [edi + 0x38], 1
        jne    loc_1_B47E
        push   0xb
        jmp    loc_1_B480
loc_1_B47E:
        push   0xc
loc_1_B480:
        push   edi
        call   sub_1_7237
        add    esp, 8
        mov    esi, eax
        test   esi, esi
        jne    loc_1_B5C9
        mov    ebx, dword ptr [edi + 0x6c]
        push   0
        push   0x19
        push   dword ptr [edi + 0x110]
        call   sub_1_5BDA
        add    esp, 0xc
loc_1_B4A8:
        mov    ax, word ptr [edi + 0x38]
        cmp    word ptr [edi + 0xac], ax
        je     loc_1_B500
        push   0xd
        push   edi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    loc_1_B5CB
        push   0
        push   3
        push   dword ptr [edi + 0x110]
        call   sub_1_5BDA
        mov    ax, word ptr [edi + 0xac]
        add    esp, 0xc
        add    al, 2
        push   eax
        push   edi
        call   sub_1_7237
        add    esp, 8
        mov    esi, eax
        test   esi, esi
        jne    loc_1_B5C9
        test   ebx, ebx
        jne    loc_1_B504
        mov    ebx, 0x2710
loc_1_B500:
        test   ebx, ebx
        je     loc_1_B51A
loc_1_B504:
        push   0
        push   ebx
        push   edi
        call   sub_1_7191
        add    esp, 0xc
        mov    esi, eax
        test   esi, esi
        jne    loc_1_B5C9
loc_1_B51A:
        cmp    dword ptr [edi + 0xa8], 0
        jne    loc_1_B571
        test   byte ptr [edi + 0x38], 1
        jne    loc_1_B54B
        cmp    byte ptr [edi + 0xb5], 0
        jne    loc_1_B56A
        push   1
        push   0x11048
        push   0x1168
        call   sub_1_1CB6
        add    esp, 0xc
        jmp    loc_1_B5CB
loc_1_B54B:
        cmp    byte ptr [edi + 0xb6], 0
        jne    loc_1_B56A
        push   2
        push   0x11048
        push   0x1168
        call   sub_1_1CB6
        add    esp, 0xc
        jmp    loc_1_B5CB
loc_1_B56A:
        mov    dword ptr [edi + 0x34], 0
loc_1_B571:
        mov    ax, word ptr [edi + 0xac]
        cmp    word ptr [edi + 0x38], ax
        je     loc_1_B5C9
        cmp    byte ptr [edi + 0xb5], 0
        mov    word ptr [edi + 0x38], ax
        jne    loc_1_B594
        cmp    byte ptr [edi + 0xb6], 0
        je     loc_1_B5B0
loc_1_B594:
        push   edi
        call   sub_1_B796
        add    esp, 4
        test   al, al
        je     loc_1_B5AA
        mov    dword ptr [edi + 0x34], 0
        jmp    loc_1_B5B0
loc_1_B5AA:
        mov    eax, dword ptr [edi + 0x7c]
        mov    dword ptr [edi + 0x34], eax
loc_1_B5B0:
        cmp    byte ptr [edi + 0xb5], 0
        jne    loc_1_B5C9
        cmp    byte ptr [edi + 0xb6], 0
        jne    loc_1_B5C9
        mov    eax, dword ptr [esp + 0x14]
        mov    byte ptr [eax], 1
loc_1_B5C9:
        mov    eax, esi
loc_1_B5CB:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_B5CF:
        xor    al, al
        mov    ecx, dword ptr [esp + 4]
        cmp    byte ptr [ecx + 0x4e6], 0x22
        jne    loc_1_B5F4
        mov    cl, byte ptr [ecx + 0x4d1]
        test   cl, cl
        je     loc_1_B5F2
        cmp    cl, 1
        je     loc_1_B5F2
        cmp    cl, 4
        jne    loc_1_B5F4
loc_1_B5F2:
        mov    al, 1
loc_1_B5F4:
        ret    
sub_1_B5F5:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [ebp + 8]
        movzx  ecx, byte ptr [esi + 0x19]
        mov    word ptr [esi + 0x42], ax
        cmp    ecx, 1
        mov    byte ptr [esi + 0x40], 1
        je     loc_1_B6BF
        cmp    ecx, 2
        je     loc_1_B6D0
        cmp    ecx, 3
        je     loc_1_B6E1
        cmp    ecx, 4
        je     loc_1_B729
        cmp    ecx, 5
        je     loc_1_B749
        cmp    ecx, 7
        je     loc_1_B754
        cmp    ecx, 8
        je     loc_1_B782
        push   1
        mov    byte ptr [esi], al
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    loc_1_B791
        push   0
        push   0x3e8
        push   dword ptr [esi + 0x110]
        call   sub_1_5BDA
        add    esp, 0xc
        push   esi
        call   sub_1_65A2
        add    esp, 4
        test   eax, eax
        jne    loc_1_B791
        push   0
        lea    eax, [ebp - 1]
        push   8
        push   eax
        push   8
        push   esi
        call   sub_1_740C
        add    esp, 0x14
        test   eax, eax
        jne    loc_1_B791
        test   byte ptr [ebp - 1], 0x80
        je     loc_1_B6B5
        mov    byte ptr [esi + 0x1b], 3
        mov    byte ptr [esi + 0x1a], 3
        jmp    loc_1_B791
loc_1_B6B5:
        mov    ecx, 2
        jmp    loc_1_B78B
loc_1_B6BF:
        push   dword ptr [ebp + 0x10]
        push   esi
        call   sub_1_BB52
        add    esp, 8
        jmp    loc_1_B791
loc_1_B6D0:
        push   dword ptr [ebp + 0x10]
        push   esi
        call   sub_1_BA66
        add    esp, 8
        jmp    loc_1_B791
loc_1_B6E1:
        mov    byte ptr [esi + 0x40], 2
        mov    dl, byte ptr [ebp + 0xc]
        test   dl, dl
        jne    loc_1_B71F
        movzx  ecx, word ptr [ebp + 0x10]
        and    ecx, 0xffff003f
        cmp    ecx, 0xa
        jne    loc_1_B705
        mov    ecx, 3
        jmp    loc_1_B78B
loc_1_B705:
        test   dl, dl
        jne    loc_1_B71F
        movzx  ecx, word ptr [ebp + 0x10]
        and    ecx, 0xffff003f
        cmp    ecx, 0xc
        jne    loc_1_B71F
        mov    ecx, 4
        jmp    loc_1_B78B
loc_1_B71F:
        mov    ecx, 2
        mov    byte ptr [esi], 0
        jmp    loc_1_B78B
loc_1_B729:
        movzx  ecx, word ptr [ebp + 0x10]
        and    ecx, 0xffff003f
        mov    byte ptr [esi], 0
        cmp    ecx, 0x21
        jne    loc_1_B742
        mov    ecx, 3
        jmp    loc_1_B78B
loc_1_B742:
        mov    ecx, 2
        jmp    loc_1_B78B
loc_1_B749:
        push   esi
        call   sub_1_B945
        add    esp, 4
        jmp    loc_1_B791
loc_1_B754:
        movzx  ecx, word ptr [ebp + 0x10]
        and    ecx, 0xffff003f
        je     loc_1_B76C
        cmp    ecx, 1
        je     loc_1_B777
        cmp    ecx, 2
        je     loc_1_B782
        jmp    loc_1_B791
loc_1_B76C:
        mov    byte ptr [esi + 0x40], 2
        mov    ecx, 3
        jmp    loc_1_B78B
loc_1_B777:
        mov    byte ptr [esi + 0x40], 3
        mov    ecx, 4
        jmp    loc_1_B78B
loc_1_B782:
        mov    byte ptr [esi + 0x40], 3
        mov    ecx, 5
loc_1_B78B:
        mov    byte ptr [esi + 0x1b], cl
        mov    byte ptr [esi + 0x1a], cl
loc_1_B791:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_B796:
        mov    ecx, dword ptr [esp + 4]
        mov    ax, word ptr [ecx + 0x38]
        and    ax, 1
        jne    loc_1_B7AD
        cmp    byte ptr [ecx + 0xb5], 0
        jne    loc_1_B7BC
loc_1_B7AD:
        cmp    ax, 1
        jne    loc_1_B7C0
        cmp    byte ptr [ecx + 0xb6], 0
        je     loc_1_B7C0
loc_1_B7BC:
        mov    al, 1
        jmp    loc_1_B7C2
loc_1_B7C0:
        xor    al, al
loc_1_B7C2:
        ret    
sub_1_B7C3:
        push   esi
        xor    eax, eax
        cmp    word ptr [esp + 0xc], 0x110b
        push   edi
        jne    loc_1_B831
        mov    edi, dword ptr [esp + 0xc]
        push   edi
        call   sub_1_66A9
        add    esp, 4
        mov    esi, eax
        test   esi, esi
        je     loc_1_B7F6
        push   esi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 0x115c
        je     loc_1_B7F6
        mov    eax, esi
        jmp    loc_1_B831
loc_1_B7F6:
        inc    word ptr [edi + 0x114]
        cmp    word ptr [edi + 0xa4], 3
        jbe    loc_1_B80B
        push   3
        jmp    loc_1_B80D
loc_1_B80B:
        push   4
loc_1_B80D:
        push   edi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    loc_1_B831
        push   1
        push   0x3e80
        push   edi
        call   sub_1_7191
        mov    byte ptr [edi + 0xb4], 0
        add    esp, 0xc
loc_1_B831:
        pop    edi
        pop    esi
        ret    
sub_1_B834:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    byte ptr [ebp - 2], 0x4a
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        push   dword ptr [esi + 0x110]
        mov    al, byte ptr [esi + 5]
        mov    byte ptr [ebp - 1], al
        call   sub_1_8CF8
        add    esp, 4
        lea    ecx, [ebp - 2]
        push   1
        push   2
        push   ecx
        push   esi
        call   sub_1_5DDF
        add    esp, 0x10
        mov    edi, eax
        test   edi, edi
        je     loc_1_B889
        push   dword ptr [esi + 0x110]
        call   sub_1_8D0B
        add    esp, 4
        push   esi
        call   sub_1_6E6A
        add    esp, 4
        mov    eax, edi
        jmp    loc_1_B8C8
loc_1_B889:
        push   1
        push   dword ptr [ebp + 0xc]
        push   dword ptr [esi + 0x110]
        call   sub_1_5BDA
        add    esp, 0xc
        mov    edi, eax
        push   edi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 0x15a0
        jne    loc_1_B8BA
        push   esi
        call   sub_1_6E6A
        add    esp, 4
        mov    eax, edi
        jmp    loc_1_B8C8
loc_1_B8BA:
        push   7
        push   dword ptr [ebp + 0x10]
        push   esi
        call   sub_1_5FD8
        add    esp, 0xc
loc_1_B8C8:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_B8CE:
        mov    eax, dword ptr [esp + 8]
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0xc]
        cmp    byte ptr [edi + 0x86], 2
        jne    loc_1_B8E9
        imul   eax, eax, 0x109
        jmp    loc_1_B8EF
loc_1_B8E9:
        imul   eax, eax, 0x186
loc_1_B8EF:
        push   0
        push   eax
        push   edi
        call   sub_1_7191
        add    esp, 0xc
        mov    esi, eax
        test   esi, esi
        je     loc_1_B918
        push   esi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 0x15a0
        je     loc_1_B914
        mov    eax, esi
        jmp    loc_1_B942
loc_1_B914:
        test   esi, esi
        jne    loc_1_B940
loc_1_B918:
        cmp    byte ptr [edi + 0xb5], 0
        jne    loc_1_B940
        cmp    byte ptr [edi + 0xb6], 0
        jne    loc_1_B940
        push   1
        push   0x1104c
        push   0x115b
        call   sub_1_1CB6
        add    esp, 0xc
        jmp    loc_1_B942
loc_1_B940:
        xor    eax, eax
loc_1_B942:
        pop    edi
        pop    esi
        ret    
sub_1_B945:
        sub    esp, 4
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0x10]
        push   edi
        push   3
        push   esi
        mov    byte ptr [esi + 0x40], 2
        call   sub_1_6CF2
        add    esp, 8
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_1_BA5D
        push   0x28
        push   esi
        call   sub_1_7237
        add    esp, 8
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_1_BA5D
        lea    edi, [esi + 0x42]
        push   edi
        push   0x10
        push   esi
        call   sub_1_8D17
        add    esp, 0xc
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_1_B9CA
        mov    ax, word ptr [edi]
        test   al, 8
        je     loc_1_B99D
        mov    byte ptr [esi + 0x40], 3
loc_1_B99D:
        test   al, 1
        je     loc_1_B9AC
        mov    ecx, 2
        mov    byte ptr [esi + 0x1b], cl
        mov    byte ptr [esi + 0x1a], cl
loc_1_B9AC:
        test   al, 0xe
        je     loc_1_B9BB
        mov    ecx, 3
        mov    byte ptr [esi + 0x1b], cl
        mov    byte ptr [esi + 0x1a], cl
loc_1_B9BB:
        test   al, 0x20
        je     loc_1_BA4C
        mov    eax, 4
        jmp    loc_1_BA46
loc_1_B9CA:
        push   esi
        call   sub_1_66A9
        add    esp, 4
        mov    ebx, eax
        push   ebx
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 0x1162
        jne    loc_1_BA4C
        push   1
        push   esi
        call   sub_1_7237
        add    esp, 8
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_1_BA5D
        push   0
        push   0x3e8
        push   dword ptr [esi + 0x110]
        call   sub_1_5BDA
        add    esp, 0xc
        push   esi
        call   sub_1_65A2
        add    esp, 4
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_1_BA5D
        lea    eax, [esp + 0xf]
        push   0
        push   8
        push   eax
        push   8
        push   esi
        call   sub_1_740C
        add    esp, 0x14
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_1_BA5D
        test   byte ptr [esp + 0xf], 0x80
        je     loc_1_BA41
        mov    eax, 3
        jmp    loc_1_BA46
loc_1_BA41:
        mov    eax, 2
loc_1_BA46:
        mov    byte ptr [esi + 0x1b], al
        mov    byte ptr [esi + 0x1a], al
loc_1_BA4C:
        test   ebx, ebx
        jne    loc_1_BA5D
        push   0
        push   esi
        call   sub_1_6CF2
        add    esp, 8
        mov    ebx, eax
loc_1_BA5D:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_1_BA66:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        movzx  eax, word ptr [ebp + 0xc]
        push   ebx
        and    eax, 0xffff003f
        push   esi
        push   edi
        xor    edi, edi
        mov    esi, dword ptr [ebp + 8]
        cmp    eax, 0x15
        mov    byte ptr [esi + 0x40], 2
        jne    loc_1_BA9A
        mov    byte ptr [esi + 0x40], 3
        mov    eax, 4
        mov    byte ptr [esi + 0x1b], al
        mov    byte ptr [esi + 0x1a], al
        jmp    loc_1_BB49
loc_1_BA9A:
        push   0x1b
        push   esi
        call   sub_1_7237
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        jne    loc_1_BB49
        push   0
        mov    ebx, 3
        push   ebx
        push   dword ptr [esi + 0x110]
        call   sub_1_5BDA
        add    esp, 0xc
        push   5
        push   esi
        call   sub_1_7237
        add    esp, 8
        mov    edi, eax
        test   edi, edi
        jne    loc_1_BB49
        push   0
        push   0x2710
        push   esi
        call   sub_1_7191
        add    esp, 0xc
        mov    edi, eax
        test   edi, edi
        jne    loc_1_BB2A
        push   0
        lea    eax, [ebp - 1]
        push   8
        push   eax
        push   8
        push   esi
        call   sub_1_740C
        add    esp, 0x14
        mov    edi, eax
        test   edi, edi
        jne    loc_1_BB49
        and    byte ptr [ebp - 1], 0x18
        shr    byte ptr [ebp - 1], 3
        cmp    byte ptr [ebp - 1], bl
        jne    loc_1_BB1A
        mov    byte ptr [esi + 0x1b], bl
        mov    byte ptr [esi + 0x1a], bl
        jmp    loc_1_BB49
loc_1_BB1A:
        mov    eax, 2
        mov    byte ptr [esi], 0
        mov    byte ptr [esi + 0x1b], al
        mov    byte ptr [esi + 0x1a], al
        jmp    loc_1_BB49
loc_1_BB2A:
        push   edi
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 0x116e
        jne    loc_1_BB49
        mov    eax, 2
        xor    edi, edi
        mov    byte ptr [esi], 0
        mov    byte ptr [esi + 0x1b], al
        mov    byte ptr [esi + 0x1a], al
loc_1_BB49:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_BB52:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        xor    ebx, ebx
        mov    esi, dword ptr [ebp + 8]
        mov    ax, word ptr [esi + 0x16]
        mov    byte ptr [esi + 0x11f], bl
        cmp    ax, 0x3c
        mov    byte ptr [esi + 0x121], bl
        jb     loc_1_BB83
        mov    byte ptr [esi + 0x40], 2
        cmp    ax, 0x57
        jb     loc_1_BB83
        mov    byte ptr [esi + 0x40], 3
loc_1_BB83:
        cmp    ax, 0x6e
        jae    loc_1_BC03
        push   0x1b
        push   esi
        call   sub_1_7237
        add    esp, 8
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_1_BC5D
        push   0
        push   3
        push   dword ptr [esi + 0x110]
        call   sub_1_5BDA
        add    esp, 0xc
        push   2
        push   esi
        call   sub_1_7237
        add    esp, 8
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_1_BC5D
        push   0
        push   0x2710
        push   esi
        call   sub_1_7191
        add    esp, 0xc
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_1_BBE5
        mov    byte ptr [esi + 0x1b], 2
        mov    byte ptr [esi + 0x1a], 2
        jmp    loc_1_BC5D
loc_1_BBE5:
        push   ebx
        call   sub_1_247A
        add    esp, 4
        cmp    ax, 0x111f
        jne    loc_1_BC5D
        mov    eax, 3
        xor    ebx, ebx
        mov    byte ptr [esi + 0x1b], al
        mov    byte ptr [esi + 0x1a], al
        jmp    loc_1_BC5D
loc_1_BC03:
        movzx  eax, word ptr [ebp + 0xc]
        and    eax, 0xffff003f
        je     loc_1_BC24
        cmp    eax, 1
        je     loc_1_BC31
        cmp    eax, 2
        je     loc_1_BC3E
        cmp    eax, 3
        je     loc_1_BC4B
        cmp    eax, 4
        je     loc_1_BC31
        jmp    loc_1_BC5D
loc_1_BC24:
        mov    eax, 2
        mov    byte ptr [esi + 0x1b], al
        mov    byte ptr [esi + 0x1a], al
        jmp    loc_1_BC5D
loc_1_BC31:
        mov    eax, 3
        mov    byte ptr [esi + 0x1b], al
        mov    byte ptr [esi + 0x1a], al
        jmp    loc_1_BC5D
loc_1_BC3E:
        mov    eax, 4
        mov    byte ptr [esi + 0x1b], al
        mov    byte ptr [esi + 0x1a], al
        jmp    loc_1_BC5D
loc_1_BC4B:
        mov    byte ptr [esi + 0x1b], 5
        cmp    byte ptr [esi + 0x45], 0
        mov    byte ptr [esi + 0x1a], 4
        je     loc_1_BC5D
        mov    byte ptr [esi + 0x1a], 5
loc_1_BC5D:
        cmp    word ptr [esi + 0x16], 0x3c
        jb     loc_1_BCD7
        push   3
        push   esi
        call   sub_1_6CF2
        add    esp, 8
        test   eax, eax
        jne    loc_1_BCD9
        push   0
        lea    eax, [ebp - 1]
        push   8
        push   eax
        push   0x25
        push   esi
        call   sub_1_740C
        add    esp, 0x14
        test   eax, eax
        jne    loc_1_BCD9
        test   byte ptr [ebp - 1], 2
        je     loc_1_BCA6
        cmp    word ptr [esi + 0x16], 0x6e
        jae    loc_1_BCA6
        push   0x20
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    loc_1_BCD9
loc_1_BCA6:
        push   0
        push   esi
        call   sub_1_6CF2
        add    esp, 8
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_1_BCD7
        test   byte ptr [ebp - 1], 2
        je     loc_1_BCD7
        cmp    word ptr [esi + 0x16], 0x6e
        jb     loc_1_BCD7
        push   8
        push   esi
        call   sub_1_BCDF
        add    esp, 8
        mov    ebx, eax
        mov    eax, ebx
        test   ebx, ebx
        jne    loc_1_BCD9
loc_1_BCD7:
        mov    eax, ebx
loc_1_BCD9:
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_BCDF:
        push   esi
        push   3
        mov    esi, dword ptr [esp + 0xc]
        push   esi
        call   sub_1_6CF2
        add    esp, 8
        test   eax, eax
        jne    loc_1_BD45
        push   0x11
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    loc_1_BD45
        push   0
        push   3
        push   dword ptr [esi + 0x110]
        call   sub_1_5BDA
        mov    al, byte ptr [esp + 0x18]
        add    esp, 0xc
        add    al, 2
        push   eax
        push   esi
        call   sub_1_7237
        add    esp, 8
        test   eax, eax
        jne    loc_1_BD45
        push   0
        push   3
        push   dword ptr [esi + 0x110]
        call   sub_1_5BDA
        add    esp, 0xc
        push   0
        push   esi
        call   sub_1_6CF2
        add    esp, 8
loc_1_BD45:
        pop    esi
        ret    
        db  0CCh
loc_1_BD48:
        dec    esp
        push   eax
        push   esp
loc_1_BD4B:
        xor    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_BD6F:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_BD9D:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_BE65:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_BF81:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_BF95:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
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
loc_1_C030:
        inc    ebx
        insb   byte ptr es:[edi], dx
        popal  
        jae    0xc0a8
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x6f], dh
        jb     0xc0b0
        jae    loc_1_C03E
loc_1_C03E:
        add    byte ptr [eax], al
loc_1_C040:
        push   eax
        outsd  dx, dword ptr [esi]
        jb     0xc0b8
        push   ebx
        jne    0xc0a9
        inc    ebx
        insb   byte ptr es:[edi], dx
        popal  
        jae    0xc0bf
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    ebx
        dec    edi
        dec    ebp
        push   eax
        inc    ecx
        push   ecx
        add    byte ptr [eax], al
loc_1_C058:
        inc    ebx
        dec    edi
        dec    ebp
        push   eax
        inc    ecx
        push   ecx
        add    byte ptr [eax], al
loc_1_C060:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x36  R-X
sub_2_0000:
        VxDCall  CONFIGMG._CONFIGMG_Locate_DevNode ; cd2002803300
        int3   
        int3   
sub_2_0008:
        VxDCall  CONFIGMG._CONFIGMG_Read_Registry_Value ; cd203e803300
        int3   
        int3   
sub_2_0010:
        VxDCall  CONFIGMG._CONFIGMG_Get_Sibling  ; cd2005803300
        int3   
        int3   
sub_2_0018:
        VxDCall  CONFIGMG._CONFIGMG_Get_Next_Res_Des ; cd201f803300
        int3   
        int3   
sub_2_0020:
        VxDCall  CONFIGMG._CONFIGMG_Get_Res_Des_Data ; cd2022803300
        int3   
        int3   
sub_2_0028:
        VxDCall  CONFIGMG._CONFIGMG_Get_First_Log_Conf ; cd201a803300
        int3   
        int3   
sub_2_0030:
        VxDCall  CONFIGMG._CONFIGMG_Get_Child    ; cd2004803300
VxD__LTEXT ENDS

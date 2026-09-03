; WSOCK -- reconstructed 32-bit disassembly (VxD / LE format)
; 2 objects, 3313 instructions recovered by recursive descent

Declare_Virtual_Device WSOCK, 1, 0, WSOCK_Control, 0003Eh, Undefined_Init_Order
;   DDK version 0400h   init_order 054006001h   flags 00000h
;   service table: obj1:0xc44, 5 services

VxD__LTEXT SEGMENT   ; object 1  vsize=0x2294  R-X
        db  053h, 052h, 056h, 08Bh, 054h, 024h, 010h, 08Bh, 05Ah, 00Ch, 08Bh, 0F3h, 003h, 035h
        dd  obj1:0x228C              ; fixup
        db  0F7h, 046h, 008h, 001h, 000h, 000h, 000h, 075h, 009h, 089h, 036h, 089h, 076h, 004h, 083h, 04Eh
        db  008h, 001h, 089h, 032h, 08Bh, 046h, 004h, 089h, 042h, 004h, 089h, 010h, 089h, 056h, 004h, 0F7h
        db  046h, 008h, 002h, 000h, 000h, 000h, 075h, 01Bh, 083h, 04Eh, 008h, 002h, 0B8h, 000h, 010h, 000h
        db  000h, 0B9h, 003h, 000h, 000h, 000h, 08Bh, 0D6h, 0BEh
        dd  loc_1_0059               ; fixup
        db  0CDh, 020h, 014h, 000h, 001h, 000h, 05Eh, 05Ah, 05Bh, 0C3h
loc_1_0059:
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        mov    edi, edx
loc_1_0070:
        mov    edx, dword ptr [edi]
        cmp    edx, edi
        je     loc_1_0098
        call   sub_1_00C6
        jb     loc_1_00B2
        mov    eax, dword ptr [edx + 4]
        mov    ecx, dword ptr [edx]
        mov    dword ptr [eax], ecx
        mov    dword ptr [ecx + 4], eax
        push   edx
        push   dword ptr [0x21e4]            ; -> obj1:0x21E4
        call   sub_1_0C17
        add    esp, 8
        jmp    loc_1_0070
loc_1_0098:
        and    dword ptr [edi + 8], 0xfffffffd
loc_1_009C:
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        ret    
loc_1_00B2:
        mov    eax, 0x1f4
        mov    edx, edi
        mov    esi, 0x59                     ; -> loc_1_0059
        VMMCall  VMM.Set_VM_Time_Out             ; cd203d000100
        jmp    loc_1_009C
sub_1_00C6:
        push   edx
        mov    eax, dword ptr [edx + 8]
        cmp    eax, dword ptr [0x2288]       ; -> obj1:0x2288
        je     loc_1_0113
        mov    eax, dword ptr [edx + 0x10]
        ror    eax, 0x10
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        ror    eax, 0x10
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    eax, dword ptr [edx + 0x14]
        ror    eax, 0x10
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        ror    eax, 0x10
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    cx, word ptr [edx + 0xa]
        movzx  edx, word ptr [edx + 8]
        VMMCall  VMM.Simulate_Far_Call           ; cd2048000100
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        clc    
        pop    edx
        ret    
loc_1_0113:
        movzx  eax, word ptr [edx + 0x14]
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        movzx  eax, word ptr [edx + 0x16]
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    eax, dword ptr [edx + 0x18]
        mov    ecx, eax
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    eax, dword ptr [edx + 0x10]
        ror    eax, 0x10
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        ror    eax, 0x10
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    eax, ecx
        ror    eax, 0x10
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    cx, word ptr [edx + 0xa]
        movzx  edx, word ptr [edx + 8]
        VMMCall  VMM.Simulate_Far_Call           ; cd2048000100
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        cmp    word ptr [ebp + 0x1c], 1
        pop    edx
        ret    
        db  000h, 000h, 000h
loc_1_0170:
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        or     esi, esi
        mov    eax, esi
        jne    loc_1_0180
        call   sub_1_0BB2
loc_1_0180:
        cmp    esi, 1
        sbb    ecx, ecx
        inc    ecx
        push   ecx
        mov    esi, dword ptr [esp + 0xc]
        push   eax
        push   dword ptr [esi + 0xc]
        push   dword ptr [esi + 8]
        push   dword ptr [esi + 4]
        push   dword ptr [esi]
        call   sub_1_01AC
        add    esp, 0x18
        mov    dword ptr [esi + 0x10], eax
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 8
        pop    esi
        ret    
sub_1_01AC:
        push   ebx
        push   esi
        push   edi
        push   ebp
        call   sub_1_0825
        mov    edi, dword ptr [esp + 0x1c]
        cmp    edi, -1
        mov    esi, dword ptr [esp + 0x20]
        jne    loc_1_01E7
        movzx  eax, si
        push   eax
        shr    esi, 0x10
        movzx  eax, si
        mov    ebp, dword ptr [esp + 0x28]
        push   eax
        push   ebp
        call   sub_1_0B2C
        add    esp, 0xc
        mov    esi, eax
        or     esi, esi
        jne    loc_1_01EB
        sub    eax, eax
        jmp    loc_1_029B
loc_1_01E7:
        mov    ebp, dword ptr [esp + 0x24]
loc_1_01EB:
        cmp    dword ptr [0x21e8], 0x21e8    ; -> obj1:0x21E8
        je     loc_1_0222
        mov    eax, dword ptr [0x21e8]       ; -> obj1:0x21E8
        mov    ebx, dword ptr [0x21e8]       ; -> obj1:0x21E8
        mov    ecx, dword ptr [eax]
        sub    ebx, 8
        mov    dword ptr [ecx + 4], 0x21e8   ; -> obj1:0x21E8
        mov    eax, dword ptr [eax]
        mov    dword ptr [0x21e8], eax       ; -> obj1:0x21E8
        mov    eax, dword ptr [0x2270]       ; -> obj1:0x2270
        dec    eax
        mov    dword ptr [0x2270], eax       ; -> obj1:0x2270
        jmp    loc_1_022E
loc_1_0222:
        push   0x2c
        call   sub_1_0C28
        add    esp, 4
        mov    ebx, eax
loc_1_022E:
        or     ebx, ebx
        je     loc_1_0299
        mov    ecx, dword ptr [esp + 0x14]
        mov    dword ptr [ebx + 0x10], ecx
        mov    eax, dword ptr [esp + 0x28]
        mov    dword ptr [ebx + 0x14], eax
        mov    edx, dword ptr [esp + 0x18]
        mov    dword ptr [ebx + 0x18], edx
        mov    dword ptr [ebx + 0x20], ebp
        mov    dword ptr [ebx + 0x24], edi
        mov    dword ptr [ebx + 0x28], esi
        mov    dword ptr [ebx + 8], 0x2274   ; -> obj1:0x2274
        lea    esi, [ebx + 8]
        mov    eax, dword ptr [0x2278]       ; -> obj1:0x2278
        mov    dword ptr [ebx + 0xc], eax
        mov    eax, dword ptr [0x2278]       ; -> obj1:0x2278
        mov    dword ptr [eax], esi
        lea    eax, [ecx + 8]
        mov    dword ptr [0x2278], esi       ; -> obj1:0x2278
        mov    dword ptr [ebx], eax
        mov    eax, dword ptr [ecx + 0xc]
        mov    dword ptr [ebx + 4], eax
        mov    eax, dword ptr [ecx + 0xc]
        mov    dword ptr [eax], ebx
        mov    dword ptr [ecx + 0xc], ebx
        mov    eax, dword ptr [ecx + 0x54]
        not    eax
        and    eax, dword ptr [ecx + 0x50]
        test   edx, eax
        je     loc_1_0299
        push   0
        push   ebx
        call   sub_1_0637
        add    esp, 8
loc_1_0299:
        mov    eax, ebx
loc_1_029B:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_02A0:
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        or     esi, esi
        mov    eax, esi
        jne    loc_1_02B0
        call   sub_1_0BB2
loc_1_02B0:
        cmp    esi, 1
        sbb    ecx, ecx
        inc    ecx
        push   ecx
        push   eax
        mov    eax, dword ptr [esp + 0x10]
        push   dword ptr [eax + 0x1c]
        push   dword ptr [eax + 0x18]
        push   dword ptr [eax + 8]
        push   dword ptr [eax + 0x14]
        push   dword ptr [eax + 4]
        push   dword ptr [eax + 0x10]
        push   dword ptr [eax]
        push   dword ptr [eax + 0xc]
        call   sub_1_02DD
        add    esp, 0x28
        pop    esi
        ret    
sub_1_02DD:
        push   ebx
        push   esi
        push   edi
        push   ebp
        sub    eax, eax
        mov    ecx, dword ptr [esp + 0x14]
        cmp    ecx, eax
        je     loc_1_030E
        mov    ebp, dword ptr [esp + 0x38]
        mov    ebx, dword ptr [esp + 0x34]
        push   ebp
        mov    edi, dword ptr [esp + 0x34]
        push   ebx
        mov    esi, dword ptr [esp + 0x34]
        push   edi
        push   esi
        push   dword ptr [esp + 0x28]
        push   ecx
        call   sub_1_06FF
        add    esp, 0x18
        jmp    loc_1_031E
loc_1_030E:
        mov    esi, dword ptr [esp + 0x2c]
        mov    edi, dword ptr [esp + 0x30]
        mov    ebx, dword ptr [esp + 0x34]
        mov    ebp, dword ptr [esp + 0x38]
loc_1_031E:
        mov    ecx, dword ptr [esp + 0x1c]
        or     ecx, ecx
        je     loc_1_033B
        or     eax, eax
        jne    loc_1_033B
        push   ebp
        push   ebx
        push   edi
        push   esi
        push   dword ptr [esp + 0x30]
        push   ecx
        call   sub_1_06FF
        add    esp, 0x18
loc_1_033B:
        mov    ecx, dword ptr [esp + 0x24]
        or     ecx, ecx
        je     loc_1_0358
        or     eax, eax
        jne    loc_1_0358
        push   ebp
        push   ebx
        push   edi
        push   esi
        push   dword ptr [esp + 0x38]
        push   ecx
        call   sub_1_06FF
        add    esp, 0x18
loc_1_0358:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_035D:
        mov    eax, dword ptr [esp + 4]
        push   dword ptr [eax]
        call   sub_1_036E
        add    esp, 4
        sub    eax, eax
        ret    
sub_1_036E:
        push   esi
        mov    edx, dword ptr [esp + 8]
        mov    eax, dword ptr [edx + 8]
        mov    esi, dword ptr [edx + 0xc]
        lea    ecx, [edx + 8]
        mov    dword ptr [esi], eax
        mov    esi, dword ptr [ecx]
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [esi + 4], eax
        test   byte ptr [edx + 0x14], 8
        jne    loc_1_039B
        mov    esi, dword ptr [edx + 4]
        mov    eax, dword ptr [edx]
        mov    dword ptr [esi], eax
        mov    esi, dword ptr [edx]
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [esi + 4], eax
loc_1_039B:
        mov    esi, dword ptr [0x2270]       ; -> obj1:0x2270
        cmp    esi, 0x40
        jae    loc_1_03CA
        mov    dword ptr [ecx], 0x21e8       ; -> obj1:0x21E8
        mov    eax, dword ptr [0x21ec]       ; -> obj1:0x21EC
        mov    dword ptr [edx + 0xc], eax
        mov    eax, dword ptr [0x21ec]       ; -> obj1:0x21EC
        inc    esi
        mov    dword ptr [eax], ecx
        mov    dword ptr [0x21ec], ecx       ; -> obj1:0x21EC
        mov    dword ptr [0x2270], esi       ; -> obj1:0x2270
        jmp    loc_1_03D3
loc_1_03CA:
        push   edx
        call   sub_1_0C2E
        add    esp, 4
loc_1_03D3:
        sub    eax, eax
        pop    esi
        ret    
loc_1_03D7:
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0xc]
        mov    esi, dword ptr [eax]
        mov    eax, esi
        mov    edi, dword ptr [esi + 8]
        sub    eax, edi
        cmp    eax, -8
        je     loc_1_0401
loc_1_03EB:
        mov    eax, edi
        mov    edi, dword ptr [edi]
        push   eax
        call   sub_1_036E
        add    esp, 4
        mov    eax, esi
        sub    eax, edi
        cmp    eax, -8
        jne    loc_1_03EB
loc_1_0401:
        sub    eax, eax
        pop    edi
        pop    esi
        ret    
loc_1_0406:
        mov    eax, dword ptr [esp + 8]
        or     eax, eax
        jne    loc_1_0413
        call   sub_1_0BB2
loc_1_0413:
        push   eax
        call   sub_1_041F
        add    esp, 4
        sub    eax, eax
        ret    
sub_1_041F:
        push   dword ptr [esp + 4]
        call   sub_1_06C4
        add    esp, 4
        sub    eax, eax
        ret    
loc_1_042E:
        mov    eax, dword ptr [esp + 4]
        push   dword ptr [eax + 8]
        push   dword ptr [eax + 4]
        push   dword ptr [eax]
        call   sub_1_0445
        add    esp, 0xc
        sub    eax, eax
        ret    
sub_1_0445:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 0x1c]
        cmp    edi, 4
        jne    loc_1_0466
        mov    esi, dword ptr [esp + 0x18]
        test   byte ptr [esi + 0x40], 0x10
        je     loc_1_0488
        mov    edi, 1
        jmp    loc_1_0488
loc_1_0466:
        cmp    edi, 0x10
        jne    loc_1_0484
        mov    ebx, dword ptr [esp + 0x20]
        or     ebx, ebx
        mov    esi, dword ptr [esp + 0x18]
        je     loc_1_048C
        cmp    dword ptr [esi + 0x58], 0
        jne    loc_1_048C
        mov    edi, 0x100
        jmp    loc_1_048C
loc_1_0484:
        mov    esi, dword ptr [esp + 0x18]
loc_1_0488:
        mov    ebx, dword ptr [esp + 0x20]
loc_1_048C:
        mov    eax, dword ptr [esi + 0x50]
        mov    dword ptr [esp + 0x10], eax
        mov    ecx, edi
        or     ecx, eax
        mov    dword ptr [esi + 0x50], ecx
        mov    eax, dword ptr [esi + 0x54]
        not    eax
        and    edi, eax
        mov    ebp, dword ptr [esi + 8]
        mov    eax, esi
        sub    eax, ebp
        cmp    eax, -8
        je     loc_1_04DB
loc_1_04AD:
        mov    eax, ebp
        mov    ebp, dword ptr [ebp]
        cmp    dword ptr [eax + 0x24], 0
        jne    loc_1_04C3
        push   eax
        call   sub_1_036E
        add    esp, 4
        jmp    loc_1_04D2
loc_1_04C3:
        test   dword ptr [eax + 0x18], edi
        je     loc_1_04D2
        push   ebx
        push   eax
        call   sub_1_0637
        add    esp, 8
loc_1_04D2:
        mov    eax, esi
        sub    eax, ebp
        cmp    eax, -8
        jne    loc_1_04AD
loc_1_04DB:
        cmp    dword ptr [esi + 0x58], 0
        je     loc_1_0532
        test   dword ptr [esi + 0x60], edi
        je     loc_1_0532
        test   dword ptr [esp + 0x10], edi
        jne    loc_1_0532
        or     ebx, ebx
        jne    loc_1_04FA
        mov    eax, edi
        and    eax, 0x30
        or     dword ptr [esi + 0x54], eax
        jmp    loc_1_0503
loc_1_04FA:
        cmp    edi, 0x10
        jne    loc_1_0503
        and    dword ptr [esi + 0x50], 0xffffffef
loc_1_0503:
        push   ebx
        push   edi
        push   esi
        call   sub_1_05A6
        add    esp, 0xc
        cmp    edi, 0x10
        jne    loc_1_0532
        or     ebx, ebx
        jne    loc_1_0532
        and    dword ptr [esi + 0x54], 0xfffffffd
        or     dword ptr [esi + 0x50], 2
        test   byte ptr [esi + 0x60], 2
        je     loc_1_0532
        push   0
        push   2
        push   esi
        call   sub_1_05A6
        add    esp, 0xc
loc_1_0532:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_1_053A:
        mov    eax, dword ptr [esp + 4]
        push   dword ptr [eax + 4]
        push   dword ptr [eax]
        call   sub_1_054E
        add    esp, 8
        sub    eax, eax
        ret    
sub_1_054E:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 0x14]
        mov    eax, edi
        mov    ebx, dword ptr [edi + 8]
        sub    eax, ebx
        cmp    eax, -8
        je     loc_1_0594
        mov    esi, dword ptr [esp + 0x18]
        sub    ebp, ebp
loc_1_0568:
        mov    eax, ebx
        mov    ebx, dword ptr [ebx]
        cmp    dword ptr [eax + 0x24], ebp
        jne    loc_1_057C
        push   eax
        call   sub_1_036E
        add    esp, 4
        jmp    loc_1_058B
loc_1_057C:
        cmp    dword ptr [eax + 0x18], ebp
        je     loc_1_058B
        push   esi
        push   eax
        call   sub_1_0637
        add    esp, 8
loc_1_058B:
        mov    eax, edi
        sub    eax, ebx
        cmp    eax, -8
        jne    loc_1_0568
loc_1_0594:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_0599:
        push   dword ptr [esp + 4]
        call   sub_1_06C4
        add    esp, 4
        ret    
sub_1_05A6:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 0x58], -1
        jne    loc_1_05D0
        mov    eax, dword ptr [eax + 0x5c]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_1_0632
loc_1_05D0:
        push   dword ptr [0x21e4]            ; -> obj1:0x21E4
        call   sub_1_0C08
        add    esp, 4
        or     eax, eax
        je     loc_1_0632
        mov    ecx, dword ptr [0x2288]       ; -> obj1:0x2288
        mov    dword ptr [eax + 8], ecx
        xor    ebx, ebx
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    dword ptr [ebp - 4], ebx
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0xc], ecx
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0x58]
        mov    word ptr [eax + 0x14], cx
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0x5c]
        mov    word ptr [eax + 0x16], cx
        mov    ecx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ecx + 0x68]
        mov    dword ptr [eax + 0x18], ecx
        movzx  ecx, word ptr [ebp + 0x10]
        shl    ecx, 0x10
        movzx  edx, word ptr [ebp + 0xc]
        or     ecx, edx
        mov    dword ptr [eax + 0x10], ecx
        push   eax
        call   0
        add    esp, 4
loc_1_0632:
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_1_0637:
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0xc]
        mov    edi, dword ptr [esp + 0x10]
        mov    dword ptr [esi + 0x1c], edi
        mov    eax, dword ptr [esi + 0x14]
        test   al, 6
        je     loc_1_0657
        push   edi
        push   esi
        call   sub_1_07A1
        add    esp, 8
        jmp    loc_1_06C1
loc_1_0657:
        test   al, 1
        jne    loc_1_0677
        push   esi
        push   dword ptr [esi + 0x24]
        push   dword ptr [esi + 0x20]
        call   sub_1_0B9C
        add    esp, 0xc
        or     eax, eax
        je     loc_1_06C1
        mov    dword ptr [esi + 0x18], 0
        jmp    loc_1_06C1
loc_1_0677:
        cmp    dword ptr [esi + 0x24], -1
        jne    loc_1_0688
        mov    eax, dword ptr [esi + 0x28]
        mov    dword ptr [eax], edi
        mov    byte ptr [eax + 4], 1
        jmp    loc_1_06B8
loc_1_0688:
        push   dword ptr [0x21e4]            ; -> obj1:0x21E4
        call   sub_1_0C08
        add    esp, 4
        or     eax, eax
        je     loc_1_06C1
        mov    ecx, dword ptr [esi + 0x24]
        mov    dword ptr [eax + 8], ecx
        mov    ecx, dword ptr [esi + 0x20]
        mov    dword ptr [eax + 0xc], ecx
        mov    ecx, dword ptr [esi + 0x28]
        mov    dword ptr [eax + 0x10], ecx
        mov    dword ptr [eax + 0x14], edi
        push   eax
        call   0
        add    esp, 4
loc_1_06B8:
        push   esi
        call   sub_1_036E
        add    esp, 4
loc_1_06C1:
        pop    edi
        pop    esi
        ret    
sub_1_06C4:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [0x2274]       ; -> obj1:0x2274
        cmp    edi, 0x2274                   ; -> obj1:0x2274
        je     loc_1_06FB
        mov    esi, dword ptr [esp + 0x10]
        sub    ebx, ebx
loc_1_06DB:
        lea    eax, [edi - 8]
        mov    edi, dword ptr [edi]
        cmp    dword ptr [eax + 0x24], ebx
        je     loc_1_06EA
        cmp    dword ptr [eax + 0x20], esi
        jne    loc_1_06F3
loc_1_06EA:
        push   eax
        call   sub_1_036E
        add    esp, 4
loc_1_06F3:
        cmp    edi, 0x2274                   ; -> obj1:0x2274
        jne    loc_1_06DB
loc_1_06FB:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_06FF:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 0x10], 0
        mov    edi, dword ptr [esp + 0x18]
        mov    ebx, edi
        imul   ebx, ebx, 0xc
        push   ebx
        push   dword ptr [esp + 0x20]
        call   sub_1_0A54
        mov    dword ptr [esp + 0x24], eax
        add    esp, 8
        or     eax, eax
        jne    loc_1_0733
        mov    eax, 0x271e
        jmp    loc_1_0799
loc_1_0733:
        mov    esi, dword ptr [esp + 0x1c]
        sub    ebp, ebp
        cmp    edi, ebp
        je     loc_1_0788
loc_1_073D:
        push   dword ptr [esp + 0x2c]
        push   dword ptr [esp + 0x2c]
        push   dword ptr [esp + 0x2c]
        push   dword ptr [esp + 0x2c]
        push   dword ptr [esi + 4]
        push   dword ptr [esi]
        call   sub_1_01AC
        add    esp, 0x18
        or     eax, eax
        je     loc_1_0768
        add    esi, 0xc
        inc    ebp
        cmp    edi, ebp
        ja     loc_1_073D
        jmp    loc_1_0788
loc_1_0768:
        or     ebp, ebp
        je     loc_1_0780
loc_1_076C:
        mov    eax, dword ptr [esi - 0xc]
        sub    esi, 0xc
        push   dword ptr [eax + 0xc]
        call   sub_1_036E
        add    esp, 4
        dec    ebp
        jne    loc_1_076C
loc_1_0780:
        mov    dword ptr [esp + 0x10], 0x2747
loc_1_0788:
        push   ebx
        push   dword ptr [esp + 0x20]
        call   sub_1_0A9B
        mov    eax, dword ptr [esp + 0x18]
        add    esp, 8
loc_1_0799:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_1_07A1:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x14]
        mov    ebx, dword ptr [esp + 0x18]
        mov    edi, dword ptr [esi + 0x20]
        test   byte ptr [esi + 0x14], 4
        je     loc_1_07F4
        or     ebx, ebx
        jne    loc_1_07E6
        mov    eax, dword ptr [esi + 0x10]
        lea    ecx, [esp + 0xc]
        push   ecx
        push   0
        push   0
        push   0x80000000
        push   dword ptr [esi + 0x28]
        push   dword ptr [esi + 0x24]
        push   eax
        mov    eax, dword ptr [eax + 0x1c]
        call   dword ptr [eax + 0x1c]
        add    esp, 0x1c
        mov    ebx, eax
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [edi + 4], eax
loc_1_07E6:
        push   dword ptr [esi + 0x28]
        push   dword ptr [esi + 0x24]
        call   sub_1_0A9B
        add    esp, 8
loc_1_07F4:
        or     ebx, ebx
        je     loc_1_07FF
        mov    dword ptr [edi + 4], 0
loc_1_07FF:
        mov    ebx, dword ptr [edi]
        push   0x14
        push   edi
        call   sub_1_0A9B
        add    esp, 8
        push   esi
        call   sub_1_036E
        add    esp, 4
        push   ebx
        call   sub_1_092C
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_1_0825:
        push   esi
        push   edi
        mov    esi, dword ptr [0x2274]       ; -> obj1:0x2274
        cmp    esi, 0x2274                   ; -> obj1:0x2274
        je     loc_1_0852
        sub    edi, edi
loc_1_0837:
        lea    eax, [esi - 8]
        mov    esi, dword ptr [esi]
        cmp    dword ptr [eax + 0x24], edi
        jne    loc_1_084A
        push   eax
        call   sub_1_036E
        add    esp, 4
loc_1_084A:
        cmp    esi, 0x2274                   ; -> obj1:0x2274
        jne    loc_1_0837
loc_1_0852:
        pop    edi
        pop    esi
        ret    
        db  000h, 000h, 000h
WSOCK_Service_00:
        mov    eax, 0x100
        clc    
        ret    
WSOCK_Control:
        cmp    eax, 1
        je     sub_2_01AC
        cmp    eax, 0x1b
        je     loc_2_022C
        cmp    eax, 0x21
        je     loc_1_087D
        cmp    eax, 0x23
        je     loc_1_0887
        clc    
        ret    
loc_1_087D:
        push   edi
        call   sub_1_0599
        add    esp, 4
        ret    
loc_1_0887:
        push   ebx
        push   esi
        push   edi
        add    dword ptr [0xca8], 1          ; -> obj1:0x0CA8
        jne    loc_1_0899
        VxDCall  IFSMgr.IFSMgr_FillHeapSpare     ; cd2011004000
loc_1_0899:
        mov    ecx, dword ptr [esi + 0xc]
        cmp    ecx, 0x100
        jb     loc_1_08FC
        cmp    ecx, 0x11b
        ja     loc_1_08FC
loc_1_08AC:
        mov    edi, dword ptr [ecx*4 + 0x8ac] ; -> loc_1_08AC
        mov    eax, dword ptr [esi + 0x10]
        or     eax, eax
        je     loc_1_08CA
        push   dword ptr [esi + 0x14]
        push   eax
        call   sub_1_0A54
        add    esp, 8
        or     eax, eax
        je     loc_1_0925
loc_1_08CA:
        mov    dword ptr [esi + 0x10], eax
        push   edx
        mov    edx, 0xffffffff
        cmp    edx, dword ptr [esi + 4]
        je     loc_1_08DA
        xor    edx, edx
loc_1_08DA:
        push   edx
        push   eax
        call   edi
        add    esp, 8
        pop    edx
        push   eax
        push   dword ptr [esi + 0x14]
        push   dword ptr [esi + 0x10]
        call   sub_1_0A9B
        add    esp, 8
        pop    eax
loc_1_08F2:
        dec    dword ptr [0xca8]             ; -> obj1:0x0CA8
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_08FC:
        cmp    ecx, 0
        je     loc_1_090D
        cmp    ecx, -1
        je     loc_1_0916
        mov    eax, 1
        jmp    loc_1_08F2
loc_1_090D:
        xor    eax, eax
        mov    ecx, 0
        jmp    loc_1_08F2
loc_1_0916:
        push   dword ptr [esi + 0x2c]
        call   sub_1_1C73
        add    esp, 4
        xor    eax, eax
        jmp    loc_1_08F2
loc_1_0925:
        mov    eax, 0x6f8
        jmp    loc_1_08F2
sub_1_092C:
        push   ebx
        push   edx
        push   esi
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    eax, ebx
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        cmp    eax, ebx
        je     loc_1_0952
        mov    esi, 0x960                    ; -> loc_1_0960
        mov    edx, dword ptr [esp + 0x10]
        VMMCall  VMM.Call_VM_Event               ; cd2011000100
        jmp    loc_1_095C
loc_1_0952:
        mov    eax, dword ptr [esp + 0x10]
        VxDCall  NDIS.Service_000Eh              ; cd200e002a00
loc_1_095C:
        pop    esi
        pop    edx
        pop    ebx
        ret    
loc_1_0960:
        mov    eax, edx
        VxDCall  NDIS.Service_000Eh              ; cd200e002a00
        ret    
WSOCK_PM_API:
        push   ebp
        push   ebx
        push   esi
        push   edi
        add    dword ptr [0xca8], 1          ; -> obj1:0x0CA8
        jne    loc_1_097E
        push   ebx
        VxDCall  IFSMgr.IFSMgr_FillHeapSpare     ; cd2011004000
        pop    ebx
loc_1_097E:
        movzx  eax, word ptr [ebp + 0x10]
        push   eax
        movzx  eax, word ptr [ebp + 0x38]
        push   eax
        push   ebx
        call   sub_1_0B2C
        add    esp, 0xc
        cmp    eax, -1
        je     loc_1_0A0D
        or     eax, eax
        je     loc_1_09AC
        movzx  ecx, word ptr [ebp + 0x18]
        push   ecx
        push   eax
        call   sub_1_0A54
        add    esp, 8
        or     eax, eax
        je     loc_1_0A0D
loc_1_09AC:
        mov    esi, eax
        mov    edx, eax
        movzx  ecx, word ptr [ebp + 0x1c]
        mov    edi, dword ptr [ecx*4 + 0x8ac] ; -> loc_1_08AC
        movzx  ecx, byte ptr [ecx + 0xc1c]   ; -> loc_1_0C1C
        jecxz  loc_1_09E3
loc_1_09C4:
        movzx  eax, word ptr [esi]
        push   eax
        movzx  eax, word ptr [esi + 2]
        push   eax
        push   ebx
        call   sub_1_0B2C
        add    esp, 0xc
        mov    dword ptr [esi], eax
        cmp    eax, -1
        je     loc_1_0A0D
        add    esi, 4
        dec    ecx
        jne    loc_1_09C4
loc_1_09E3:
        mov    esi, edx
        push   ebx
        push   edx
        call   edi
        add    esp, 8
        mov    word ptr [ebp + 0x1c], ax
        movzx  ecx, word ptr [ebp + 0x18]
        push   ecx
        push   esi
        call   sub_1_0A9B
        add    esp, 8
        and    dword ptr [ebp + 0x2c], 0xfffffffe
loc_1_0A02:
        dec    dword ptr [0xca8]             ; -> obj1:0x0CA8
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
loc_1_0A0D:
        mov    word ptr [ebp + 0x1c], 0x271e
        jmp    loc_1_0A02
WSOCK_Service_01:
        test   eax, eax
        je     loc_1_0A1B
        xor    edx, edx
loc_1_0A1B:
        push   edx
        push   ecx
        push   ebx
        push   eax
        call   sub_2_0080
        add    esp, 0x10
        cmp    eax, 1
        ret    
WSOCK_Service_02:
        test   eax, eax
        je     loc_1_0A31
        xor    edx, edx
loc_1_0A31:
        push   edx
        push   eax
        call   sub_2_0131
        add    esp, 8
        ret    
WSOCK_Service_03:
        jmp    sub_1_0445
WSOCK_Service_04:
        jmp    sub_1_054E
loc_1_0A46:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax]
        mov    dword ptr [0x2288], eax       ; -> obj1:0x2288
        xor    eax, eax
        ret    
sub_1_0A54:
        push   ebx
        push   edi
        push   esi
        mov    eax, dword ptr [esp + 0x10]
        mov    ebx, dword ptr [esp + 0x14]
        or     eax, eax
        je     loc_1_0A93
        or     ebx, ebx
        je     loc_1_0A93
        movzx  ecx, ax
        and    ch, 0xf
        mov    esi, ecx
        add    ebx, ecx
        add    ebx, 0xfff
        shr    ebx, 0xc
        shr    eax, 0xc
        push   0x40000000
        push   ebx
        push   eax
        VMMCall  VMM._LinPageLock                ; cd2063000100
        add    esp, 0xc
        or     eax, eax
        je     loc_1_0A97
        add    eax, esi
loc_1_0A93:
        pop    esi
        pop    edi
        pop    ebx
        ret    
loc_1_0A97:
        xor    eax, eax
        jmp    loc_1_0A93
sub_1_0A9B:
        push   ebx
        push   edi
        push   esi
        mov    eax, dword ptr [esp + 0x10]
        mov    ebx, dword ptr [esp + 0x14]
        or     eax, eax
        je     loc_1_0AD6
        or     ebx, ebx
        je     loc_1_0AD6
        movzx  ecx, ax
        and    ch, 0xf
        add    ebx, ecx
        add    ebx, 0xfff
        shr    ebx, 0xc
        shr    eax, 0xc
        push   0x40000000
        push   ebx
        push   eax
        VMMCall  VMM._LinPageUnLock              ; cd2064000100
        add    esp, 0xc
        or     eax, eax
        je     loc_1_0ADF
loc_1_0AD6:
        mov    eax, 1
loc_1_0ADB:
        pop    esi
        pop    edi
        pop    ebx
        ret    
loc_1_0ADF:
        xor    eax, eax
        jmp    loc_1_0ADB
        db  053h, 057h, 056h, 08Bh, 044h, 024h, 010h, 08Bh, 05Ch, 024h, 014h, 00Bh, 0C0h, 074h, 02Bh, 00Bh
        db  0DBh, 074h, 027h, 00Fh, 0B7h, 0C8h, 080h, 0E5h, 00Fh, 003h, 0D9h, 081h, 0C3h, 0FFh, 00Fh, 000h
        db  000h, 0C1h, 0EBh, 00Ch, 0C1h, 0E8h, 00Ch, 08Bh, 0CBh, 06Ah, 000h, 053h, 050h, 0CDh, 020h, 067h
        db  000h, 001h, 000h, 083h, 0C4h, 00Ch, 03Bh, 0C1h, 075h, 009h, 0B8h, 001h, 000h, 000h, 000h, 05Eh
        db  05Fh, 05Bh, 0C3h, 033h, 0C0h, 0EBh, 0F8h, 000h, 000h
sub_1_0B2C:
        push   ebx
        push   ecx
        push   edx
        push   esi
        mov    ebx, dword ptr [esp + 0x14]
        movzx  eax, word ptr [esp + 0x18]
        movzx  esi, word ptr [esp + 0x1c]
        or     eax, eax
        je     loc_1_0B68
        test   dword ptr [ebx], 0x20
        je     loc_1_0B6D
        push   0
        push   eax
        push   ebx
        VMMCall  VMM._SelectorMapFlat            ; cd206e000100
        add    esp, 0xc
        cmp    eax, -1
        je     loc_1_0B68
        add    eax, esi
        cmp    eax, 0x110000
        jae    loc_1_0B68
loc_1_0B65:
        add    eax, dword ptr [ebx + 4]
loc_1_0B68:
        pop    esi
        pop    edx
        pop    ecx
        pop    ebx
        ret    
loc_1_0B6D:
        shl    eax, 4
        add    eax, esi
        jmp    loc_1_0B65
loc_1_0B74:
        ret    
        db  053h, 057h, 056h, 06Ah, 000h, 068h
        dd  loc_1_0B74               ; fixup
        db  0FFh, 074h, 024h, 024h, 0FFh, 074h, 024h, 024h, 0FFh, 074h, 024h, 024h, 0FFh, 074h, 024h, 024h
        db  0CDh, 020h, 006h, 000h, 017h, 000h, 083h, 0C4h, 018h, 05Eh, 05Fh, 05Bh, 0C3h
sub_1_0B9C:
        push   dword ptr [esp + 4]
        push   dword ptr [esp + 0x10]
        push   dword ptr [esp + 0x10]
        VxDCall  NDIS.Service_0002h              ; cd2002002a00
        add    esp, 0xc
        ret    
sub_1_0BB2:
        push   edi
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        mov    eax, edi
        pop    edi
        ret    
sub_1_0BBD:
        VxDCall  NDIS.Service_800Dh              ; cd200d802a00
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_1_0BD0:
        push   esi
        mov    eax, dword ptr [esp + 0xc]
        sub    eax, 4
        sub    edx, edx
        mov    esi, dword ptr [esp + 0x10]
        mov    ecx, dword ptr [esp + 8]
        div    esi
        lea    edx, [ecx + 4]
        mov    dword ptr [ecx], edx
        mov    ecx, eax
        dec    eax
        or     ecx, ecx
        je     loc_1_0BFE
loc_1_0BF0:
        lea    ecx, [edx + esi]
        mov    dword ptr [edx], ecx
        mov    edx, ecx
        mov    ecx, eax
        dec    eax
        or     ecx, ecx
        jne    loc_1_0BF0
loc_1_0BFE:
        sub    edx, esi
        pop    esi
        mov    dword ptr [edx], 0
        ret    
sub_1_0C08:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx]
        or     eax, eax
        je     loc_1_0C16
        mov    edx, dword ptr [eax]
        mov    dword ptr [ecx], edx
loc_1_0C16:
        ret    
sub_1_0C17:
        mov    eax, dword ptr [esp + 4]
        db  08Bh
loc_1_0C1C:
        dec    esp
        and    al, 8
        mov    edx, dword ptr [eax]
        mov    dword ptr [ecx], edx
        mov    dword ptr [eax], ecx
        ret    
        db  000h, 000h
sub_1_0C28:
        VxDCall  IFSMgr.IFSMgr_GetHeap           ; cd200d804000
sub_1_0C2E:
        cmp    dword ptr [esp + 4], 0
        je     loc_1_0C3B
        VxDCall  IFSMgr.IFSMgr_RetHeap           ; cd200e804000
loc_1_0C3B:
        ret    
loc_1_0C3C:
        VxDCall  IFSMgr.IFSMgr_FillHeapSpare     ; cd2011804000
        add    byte ptr [eax], al
        dd  WSOCK_Service_00         ; fixup
        dd  WSOCK_Service_01         ; fixup
        dd  WSOCK_Service_02         ; fixup
        dd  WSOCK_Service_03         ; fixup
        dd  WSOCK_Service_04         ; fixup
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  0003Eh                      ; DDB_Req_Device_Number
        db  1, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'WSOCK   '                  ; DDB_Name
        dd  054006001h                  ; DDB_Init_Order
        dd  WSOCK_Control            ; DDB_Control_Proc
        dd  WSOCK_PM_API             ; DDB_V86_API_Proc
        dd  WSOCK_PM_API             ; DDB_PM_API_Proc
        dd  obj1:0x0C44              ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 0FFh, 0FFh, 0FFh, 0FFh
        dd  loc_1_0D40               ; fixup
        dd  loc_1_1110               ; fixup
        dd  loc_1_1C2B               ; fixup
        dd  loc_1_11E0               ; fixup
        dd  loc_1_1320               ; fixup
        dd  loc_1_13B2               ; fixup
        dd  loc_1_1D30               ; fixup
        dd  loc_1_1440               ; fixup
        dd  loc_1_14C0               ; fixup
        dd  loc_1_1510               ; fixup
        dd  loc_1_1620               ; fixup
        dd  loc_1_1694               ; fixup
        dd  loc_1_16CF               ; fixup
        dd  loc_1_1940               ; fixup
        dd  loc_1_1F85               ; fixup
        dd  loc_1_1A50               ; fixup
        dd  loc_1_1AC0               ; fixup
        dd  loc_1_0170               ; fixup
        dd  loc_1_02A0               ; fixup
        dd  loc_1_035D               ; fixup
        dd  loc_1_03D7               ; fixup
        dd  loc_1_0406               ; fixup
        dd  loc_1_042E               ; fixup
        dd  loc_1_053A               ; fixup
        dd  loc_1_12B0               ; fixup
        dd  loc_1_0A46               ; fixup
        dd  loc_1_0EF0               ; fixup
        dd  loc_1_1022               ; fixup
        db  001h, 001h, 000h, 001h, 001h, 001h, 001h, 000h, 000h, 002h, 003h, 003h, 000h, 002h, 001h, 000h
        db  000h, 000h, 003h, 001h, 000h, 000h, 000h, 000h, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
loc_1_0D40:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 0x1c]
        mov    edi, dword ptr [ebx + 4]
        mov    eax, dword ptr [ebx]
        mov    ebp, dword ptr [ebx + 0xc]
        cmp    dword ptr [edi + 0x2c], 2
        jne    loc_1_0D63
        mov    esi, 0x273d
        jmp    loc_1_0EE0
loc_1_0D63:
        mov    ecx, dword ptr [edi + 0x4c]
        cmp    ecx, 0xa
        jne    loc_1_0D75
        mov    esi, 0x2742
        jmp    loc_1_0EE0
loc_1_0D75:
        cmp    ecx, 3
        je     loc_1_0D84
        mov    esi, 0x2726
        jmp    loc_1_0EE0
loc_1_0D84:
        or     eax, eax
        je     loc_1_0DBB
        cmp    dword ptr [edi + 0x20], ebp
        jle    loc_1_0D97
        mov    esi, 0x271e
        jmp    loc_1_0EE0
loc_1_0D97:
        or     eax, eax
        je     loc_1_0DBB
        or     ebp, ebp
        je     loc_1_0DBB
        push   ebp
        push   eax
        call   sub_1_0A54
        mov    dword ptr [esp + 0x1c], eax
        add    esp, 8
        or     eax, eax
        jne    loc_1_0DC5
        mov    esi, 0x271e
        jmp    loc_1_0EE0
loc_1_0DBB:
        mov    dword ptr [esp + 0x14], 0
        sub    ebp, ebp
loc_1_0DC5:
        lea    eax, [esp + 0x10]
        push   dword ptr [ebx + 0x10]
        push   eax
        mov    eax, dword ptr [edi + 0x1c]
        push   edi
        call   dword ptr [eax + 0x10]
        add    esp, 0xc
        mov    esi, eax
        or     esi, esi
        je     loc_1_0E36
        cmp    esi, 0x2733
        jne    loc_1_0E24
        test   byte ptr [edi + 0x41], 1
        je     loc_1_0E24
        mov    esi, dword ptr [esp + 0x20]
        or     esi, esi
        mov    eax, esi
        jne    loc_1_0DFA
        call   sub_1_0BB2
loc_1_0DFA:
        cmp    esi, 1
        sbb    ecx, ecx
        inc    ecx
        push   ecx
        push   eax
        push   dword ptr [ebx + 0x18]
        push   dword ptr [ebx + 0x14]
        push   8
        push   edi
        call   sub_1_01AC
        add    esp, 0x18
        cmp    eax, 1
        sbb    esi, esi
        and    esi, 0xffff2748
        add    esi, 0xffff
loc_1_0E24:
        push   ebp
        push   dword ptr [esp + 0x18]
        call   sub_1_0A9B
        add    esp, 8
        jmp    loc_1_0EE0
loc_1_0E36:
        mov    eax, dword ptr [esp + 0x10]
        cmp    dword ptr [esp + 0x14], 0
        mov    dword ptr [eax + 0x70], 0xffffffff
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x6c], 0xffffffff
        je     loc_1_0E93
        mov    eax, dword ptr [esp + 0x10]
        mov    edx, dword ptr [eax + 0x24]
        cmp    ebp, edx
        jb     loc_1_0E7F
        mov    edi, dword ptr [esp + 0x14]
        mov    esi, dword ptr [eax + 0x18]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    eax, dword ptr [esp + 0x10]
        mov    eax, dword ptr [eax + 0x24]
        mov    dword ptr [ebx + 0xc], eax
        jmp    loc_1_0E86
loc_1_0E7F:
        mov    dword ptr [ebx + 0xc], 0
loc_1_0E86:
        push   ebp
        push   dword ptr [esp + 0x18]
        call   sub_1_0A9B
        add    esp, 8
loc_1_0E93:
        mov    esi, dword ptr [esp + 0x20]
        or     esi, esi
        jne    loc_1_0EA9
        call   sub_1_0BBD
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [ecx + 0x64], eax
        jmp    loc_1_0EB0
loc_1_0EA9:
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax + 0x64], esi
loc_1_0EB0:
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [eax], 0x227c       ; -> obj1:0x227C
        mov    ecx, dword ptr [esp + 0x10]
        mov    eax, dword ptr [0x2280]       ; -> obj1:0x2280
        mov    dword ptr [ecx + 4], eax
        sub    esi, esi
        mov    ecx, dword ptr [0x2280]       ; -> obj1:0x2280
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [0x2280], eax       ; -> obj1:0x2280
        mov    dword ptr [ebx + 8], eax
loc_1_0EE0:
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h
loc_1_0EF0:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x1c]
        mov    edi, dword ptr [esi + 8]
        mov    eax, dword ptr [esi + 0xc]
        mov    dword ptr [esp + 0x10], eax
        push   eax
        push   dword ptr [esi]
        call   sub_1_0A54
        add    esp, 8
        mov    ebx, eax
        push   0x14
        push   dword ptr [esi + 0x10]
        call   sub_1_0A54
        add    esp, 8
        mov    esi, eax
        or     ebx, ebx
        je     loc_1_1002
        or     esi, esi
        je     loc_1_1002
        mov    ecx, dword ptr [edi + 0x4c]
        cmp    ecx, 0xa
        jne    loc_1_0F43
        mov    ebp, 0x2742
        jmp    loc_1_0FDB
loc_1_0F43:
        mov    eax, dword ptr [edi + 0x40]
        test   al, 0x80
        je     loc_1_0F54
        mov    ebp, 0x274a
        jmp    loc_1_0FDB
loc_1_0F54:
        cmp    dword ptr [edi + 0x2c], 2
        jne    loc_1_0F6B
        cmp    ecx, 2
        je     loc_1_0F8D
        cmp    ecx, 6
        je     loc_1_0F8D
        mov    ebp, 0x2726
        jmp    loc_1_0FDB
loc_1_0F6B:
        cmp    ecx, 5
        jne    loc_1_0F7C
        test   ah, 1
        jne    loc_1_0F7C
        mov    ebp, 0x2733
        jmp    loc_1_0FDB
loc_1_0F7C:
        cmp    ecx, 6
        je     loc_1_0F8D
        cmp    ecx, 7
        je     loc_1_0F8D
        mov    ebp, 0x2749
        jmp    loc_1_0FDB
loc_1_0F8D:
        lea    eax, [esp + 0x14]
        push   eax
        push   0
        push   0
        push   0x80000000
        mov    eax, dword ptr [edi + 0x1c]
        push   dword ptr [esp + 0x20]
        push   ebx
        push   edi
        call   dword ptr [eax + 0x1c]
        add    esp, 0x1c
        mov    ebp, eax
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [esi + 4], eax
        cmp    ebp, 0x2733
        jne    loc_1_0FDB
        push   4
        push   esi
        push   dword ptr [esp + 0x18]
        push   ebx
        push   1
        push   edi
        call   sub_1_01AC
        add    esp, 0x18
        cmp    eax, 1
        sbb    ebp, ebp
        and    ebp, 0x2747
        je     loc_1_1018
loc_1_0FDB:
        or     ebp, ebp
        je     loc_1_0FE6
        mov    dword ptr [esi + 4], 0
loc_1_0FE6:
        push   dword ptr [esi]
        call   sub_1_092C
        add    esp, 4
        push   dword ptr [esp + 0x10]
        push   ebx
        call   sub_1_0A9B
        add    esp, 8
        push   0x14
        push   esi
        jmp    loc_1_1010
loc_1_1002:
        mov    ebp, 0x2747
        or     ebx, ebx
        je     loc_1_1018
        push   dword ptr [esp + 0x10]
        push   ebx
loc_1_1010:
        call   sub_1_0A9B
        add    esp, 8
loc_1_1018:
        mov    eax, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
loc_1_1022:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x18]
        push   0x14
        mov    edi, dword ptr [esi + 8]
        push   dword ptr [esi + 0x10]
        call   sub_1_0A54
        add    esp, 8
        mov    ebx, eax
        or     ebx, ebx
        jne    loc_1_104D
        mov    ebp, 0x2747
        jmp    loc_1_10F8
loc_1_104D:
        mov    ecx, dword ptr [edi + 0x4c]
        cmp    ecx, 0xa
        jne    loc_1_105C
        mov    ebp, 0x2742
        jmp    loc_1_10D8
loc_1_105C:
        mov    eax, dword ptr [edi + 0x40]
        test   al, 0x40
        je     loc_1_106A
        mov    ebp, 0x274a
        jmp    loc_1_10D8
loc_1_106A:
        cmp    dword ptr [edi + 0x2c], 2
        je     loc_1_108D
        cmp    ecx, 5
        jne    loc_1_1081
        test   ah, 1
        jne    loc_1_1081
        mov    ebp, 0x2733
        jmp    loc_1_10D8
loc_1_1081:
        cmp    ecx, 6
        je     loc_1_108D
        mov    ebp, 0x2749
        jmp    loc_1_10D8
loc_1_108D:
        lea    eax, [esp + 0x10]
        push   eax
        push   0
        push   0
        push   0
        push   dword ptr [esi + 0xc]
        push   dword ptr [esi]
        mov    eax, dword ptr [edi + 0x1c]
        push   edi
        call   dword ptr [eax + 0x20]
        add    esp, 0x1c
        mov    ebp, eax
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [ebx + 4], eax
        cmp    ebp, 0x2733
        jne    loc_1_10D8
        push   2
        push   ebx
        push   dword ptr [esi + 0xc]
        push   dword ptr [esi]
        push   2
        push   edi
        call   sub_1_01AC
        add    esp, 0x18
        cmp    eax, 1
        sbb    ebp, ebp
        and    ebp, 0x2747
        je     loc_1_10F8
loc_1_10D8:
        or     ebp, ebp
        je     loc_1_10E3
        mov    dword ptr [ebx + 4], 0
loc_1_10E3:
        push   dword ptr [ebx]
        call   sub_1_092C
        add    esp, 4
        push   0x14
        push   ebx
        call   sub_1_0A9B
        add    esp, 8
loc_1_10F8:
        mov    eax, ebp
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_1110:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        mov    edi, dword ptr [esi + 4]
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [edi + 0x4c]
        cmp    eax, 0xa
        jne    loc_1_112E
        mov    ebx, 0x2742
        jmp    loc_1_11D0
loc_1_112E:
        cmp    eax, 1
        je     loc_1_113D
        mov    ebx, 0x2726
        jmp    loc_1_11D0
loc_1_113D:
        movzx  eax, word ptr [ecx]
        cmp    dword ptr [edi + 0x28], eax
        je     loc_1_114F
        mov    ebx, 0x273f
        jmp    loc_1_11D0
loc_1_114F:
        push   dword ptr [esi + 8]
        push   ecx
        mov    eax, dword ptr [edi + 0x1c]
        push   edi
        call   dword ptr [eax + 0xc]
        add    esp, 0xc
        mov    ebx, eax
        or     ebx, ebx
        jne    loc_1_1185
        cmp    dword ptr [edi + 0x2c], 2
        jne    loc_1_117F
        mov    dword ptr [edi + 0x54], 0
        push   0
        push   2
        push   edi
        call   sub_1_0445
        add    esp, 0xc
        jmp    loc_1_11D0
loc_1_117F:
        and    dword ptr [edi + 0x54], 0xffffffef
        jmp    loc_1_11D0
loc_1_1185:
        cmp    ebx, 0x2733
        jne    loc_1_11D0
        test   byte ptr [edi + 0x41], 1
        je     loc_1_11D0
        and    dword ptr [edi + 0x54], 0xffffffed
        mov    ebx, dword ptr [esp + 0x14]
        or     ebx, ebx
        mov    eax, ebx
        jne    loc_1_11A6
        call   sub_1_0BB2
loc_1_11A6:
        cmp    ebx, 1
        sbb    ecx, ecx
        inc    ecx
        push   ecx
        push   eax
        push   dword ptr [esi + 0x10]
        push   dword ptr [esi + 0xc]
        push   2
        push   edi
        call   sub_1_01AC
        add    esp, 0x18
        cmp    eax, 1
        sbb    ebx, ebx
        and    ebx, 0xffff2748
        add    ebx, 0xffff
loc_1_11D0:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_11E0:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        mov    edi, dword ptr [esi + 4]
        mov    eax, dword ptr [edi + 0x4c]
        cmp    eax, 0xa
        jne    loc_1_11FC
        mov    ebx, 0x2742
        jmp    loc_1_12A6
loc_1_11FC:
        cmp    eax, 6
        jne    loc_1_1211
        cmp    dword ptr [edi + 0x2c], 2
        je     loc_1_1227
        mov    ebx, 0x2748
        jmp    loc_1_12A6
loc_1_1211:
        cmp    eax, 1
        je     loc_1_121B
        cmp    eax, 2
        jne    loc_1_1220
loc_1_121B:
        cmp    eax, 5
        jne    loc_1_1227
loc_1_1220:
        mov    ebx, 0x2726
        jmp    loc_1_12A6
loc_1_1227:
        and    dword ptr [edi + 0x50], 0xfffffeff
        push   dword ptr [esi + 8]
        push   dword ptr [esi]
        mov    eax, dword ptr [edi + 0x1c]
        push   edi
        call   dword ptr [eax + 0x14]
        add    esp, 0xc
        mov    ebx, eax
        cmp    ebx, 0x2733
        jne    loc_1_128B
        test   byte ptr [edi + 0x41], 1
        je     loc_1_12A6
        mov    ebx, dword ptr [esp + 0x14]
        or     ebx, ebx
        mov    eax, ebx
        jne    loc_1_125C
        call   sub_1_0BB2
loc_1_125C:
        cmp    ebx, 1
        sbb    ecx, ecx
        inc    ecx
        push   ecx
        push   eax
        push   dword ptr [esi + 0x10]
        push   dword ptr [esi + 0xc]
        push   0x110
        push   edi
        call   sub_1_01AC
        add    esp, 0x18
        cmp    eax, 1
        sbb    ebx, ebx
        and    ebx, 0xffff2748
        add    ebx, 0xffff
        jmp    loc_1_12A6
loc_1_128B:
        or     ebx, ebx
        jne    loc_1_12A6
        cmp    dword ptr [edi + 0x58], 0
        je     loc_1_12A6
        and    dword ptr [edi + 0x54], 0xfffffffd
        push   0
        push   2
        push   edi
        call   sub_1_0445
        add    esp, 0xc
loc_1_12A6:
        mov    eax, ebx
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  000h, 000h, 000h, 000h
loc_1_12B0:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x14]
        mov    edi, 0x21f0                   ; -> obj1:0x21F0
        mov    ebx, dword ptr [esi + 0x10]
        sub    ebp, ebp
        mov    eax, dword ptr [0x2284]       ; -> obj1:0x2284
        cmp    eax, ebp
        je     loc_1_12EE
loc_1_12CB:
        push   dword ptr [edi + 0xc]
        push   ebx
        push   0
        mov    eax, dword ptr [edi + 8]
        push   0
        call   dword ptr [eax]
        add    esp, 0x10
        or     eax, eax
        mov    eax, dword ptr [0x2284]       ; -> obj1:0x2284
        je     loc_1_12EC
        add    edi, 0x10
        inc    ebp
        cmp    eax, ebp
        ja     loc_1_12CB
loc_1_12EC:
        cmp    eax, ebp
loc_1_12EE:
        jbe    loc_1_1314
        mov    eax, dword ptr [edi + 8]
        mov    ecx, dword ptr [eax + 0x38]
        or     ecx, ecx
        je     loc_1_1314
        push   dword ptr [edi + 0xc]
        lea    eax, [esi + 0xc]
        push   eax
        push   dword ptr [esi + 4]
        lea    eax, [esi + 8]
        push   eax
        push   dword ptr [esi]
        push   dword ptr [esi + 0x14]
        call   ecx
        add    esp, 0x18
        jmp    loc_1_1319
loc_1_1314:
        mov    eax, 0x273b
loc_1_1319:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  000h, 000h
loc_1_1320:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 0x18]
        mov    ebp, dword ptr [ebx + 4]
        mov    ecx, dword ptr [ebx]
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [esp + 0x10], eax
        mov    eax, dword ptr [ebp + 0x4c]
        cmp    eax, 0xa
        jne    loc_1_1346
        mov    eax, 0x2742
        jmp    loc_1_13AA
loc_1_1346:
        cmp    eax, 6
        je     loc_1_1357
        cmp    eax, 7
        je     loc_1_1357
        mov    eax, 0x2749
        jmp    loc_1_13AA
loc_1_1357:
        mov    eax, dword ptr [esp + 0x10]
        cmp    dword ptr [ebp + 0x24], eax
        jbe    loc_1_1367
        mov    eax, 0x271e
        jmp    loc_1_13AA
loc_1_1367:
        push   dword ptr [esp + 0x10]
        push   ecx
        call   sub_1_0A54
        add    esp, 8
        or     eax, eax
        jne    loc_1_137F
        mov    eax, 0x271e
        jmp    loc_1_13AA
loc_1_137F:
        mov    edx, dword ptr [ebp + 0x24]
        mov    edi, eax
        mov    esi, dword ptr [ebp + 0x18]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    ecx, dword ptr [ebp + 0x24]
        mov    dword ptr [ebx + 8], ecx
        push   dword ptr [esp + 0x10]
        push   eax
        call   sub_1_0A9B
        add    esp, 8
        sub    eax, eax
loc_1_13AA:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_1_13B2:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebx, dword ptr [esp + 0x18]
        mov    ebp, dword ptr [ebx + 4]
        mov    ecx, dword ptr [ebx]
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [esp + 0x10], eax
        mov    eax, dword ptr [ebp + 0x4c]
        cmp    eax, 0xa
        jne    loc_1_13D8
        mov    eax, 0x2742
        jmp    loc_1_1437
loc_1_13D8:
        cmp    eax, 1
        jne    loc_1_13E4
        mov    eax, 0x2726
        jmp    loc_1_1437
loc_1_13E4:
        mov    eax, dword ptr [esp + 0x10]
        cmp    dword ptr [ebp + 0x20], eax
        jbe    loc_1_13F4
        mov    eax, 0x271e
        jmp    loc_1_1437
loc_1_13F4:
        push   dword ptr [esp + 0x10]
        push   ecx
        call   sub_1_0A54
        add    esp, 8
        or     eax, eax
        jne    loc_1_140C
        mov    eax, 0x271e
        jmp    loc_1_1437
loc_1_140C:
        mov    edx, dword ptr [ebp + 0x20]
        mov    edi, eax
        mov    esi, dword ptr [ebp + 0x14]
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    ecx, dword ptr [ebp + 0x20]
        mov    dword ptr [ebx + 8], ecx
        push   dword ptr [esp + 0x10]
        push   eax
        call   sub_1_0A9B
        add    esp, 8
        sub    eax, eax
loc_1_1437:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  000h
loc_1_1440:
        sub    esp, 4
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        mov    ecx, dword ptr [esi]
        mov    edx, dword ptr [esi + 4]
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [esp + 4], eax
        cmp    dword ptr [ecx + 0x4c], 0xa
        jne    loc_1_1461
        mov    eax, 0x2742
        jmp    loc_1_14B6
loc_1_1461:
        sub    eax, eax
        cmp    edx, 0x8004667e
        je     loc_1_147D
        lea    eax, [esp + 4]
        push   eax
        push   edx
        mov    eax, dword ptr [ecx + 0x1c]
        push   ecx
        call   dword ptr [eax + 0x30]
        add    esp, 0xc
        jmp    loc_1_14AF
loc_1_147D:
        cmp    dword ptr [esp + 4], 0
        je     loc_1_148D
        and    dword ptr [ecx + 0x40], 0xfffffeff
        jmp    loc_1_14AF
loc_1_148D:
        cmp    dword ptr [ecx + 0x58], 0
        je     loc_1_149A
        mov    eax, 0x2726
        jmp    loc_1_14AF
loc_1_149A:
        mov    dword ptr [ecx + 0x58], 0
        mov    dword ptr [ecx + 0x60], 0
        or     dword ptr [ecx + 0x40], 0x100
loc_1_14AF:
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [esi + 8], ecx
loc_1_14B6:
        pop    esi
        add    esp, 4
        ret    
        db  000h, 000h, 000h, 000h, 000h
loc_1_14C0:
        mov    eax, dword ptr [esp + 4]
        mov    edx, dword ptr [eax]
        mov    ecx, dword ptr [eax + 4]
        mov    eax, dword ptr [edx + 0x4c]
        cmp    eax, 0xa
        jne    loc_1_14D8
        mov    eax, 0x2742
        jmp    loc_1_150D
loc_1_14D8:
        cmp    dword ptr [edx + 0x2c], 2
        jne    loc_1_14E5
        mov    eax, 0x273d
        jmp    loc_1_150D
loc_1_14E5:
        cmp    eax, 6
        je     loc_1_1508
        cmp    eax, 5
        je     loc_1_1508
        cmp    eax, 2
        je     loc_1_14FB
        mov    eax, 0x2726
        jmp    loc_1_150D
loc_1_14FB:
        push   ecx
        mov    eax, dword ptr [edx + 0x1c]
        push   edx
        call   dword ptr [eax + 0x18]
        add    esp, 8
        jmp    loc_1_150D
loc_1_1508:
        mov    eax, 0x2748
loc_1_150D:
        ret    
        db  000h, 000h
loc_1_1510:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x14]
        mov    edi, dword ptr [esi + 8]
        mov    edx, dword ptr [esi + 0x10]
        mov    ecx, dword ptr [edi + 0x4c]
        cmp    ecx, 0xa
        jne    loc_1_1530
        mov    eax, 0x2742
        jmp    loc_1_161A
loc_1_1530:
        mov    eax, dword ptr [edi + 0x40]
        test   al, 0x80
        je     loc_1_1541
        mov    eax, 0x274a
        jmp    loc_1_161A
loc_1_1541:
        cmp    dword ptr [edi + 0x2c], 2
        jne    loc_1_156A
        cmp    ecx, 2
        je     loc_1_155B
        cmp    ecx, 6
        je     loc_1_155B
        mov    eax, 0x2726
        jmp    loc_1_161A
loc_1_155B:
        test   dl, 1
        je     loc_1_159F
        mov    eax, 0x273d
        jmp    loc_1_161A
loc_1_156A:
        test   byte ptr [edi + 0x3c], 1
        je     loc_1_157A
        mov    eax, 0x2746
        jmp    loc_1_161A
loc_1_157A:
        cmp    ecx, 5
        jne    loc_1_158E
        test   ah, 1
        jne    loc_1_158E
        mov    eax, 0x2733
        jmp    loc_1_161A
loc_1_158E:
        cmp    ecx, 6
        je     loc_1_159F
        cmp    ecx, 7
        je     loc_1_159F
        mov    eax, 0x2749
        jmp    loc_1_161A
loc_1_159F:
        lea    ebx, [esi + 0x18]
        push   ebx
        lea    eax, [esi + 0x14]
        push   eax
        push   dword ptr [esi + 4]
        push   edx
        push   dword ptr [esi + 0xc]
        push   dword ptr [esi]
        mov    eax, dword ptr [edi + 0x1c]
        push   edi
        call   dword ptr [eax + 0x1c]
        add    esp, 0x1c
        cmp    eax, 0x2733
        jne    loc_1_1604
        test   byte ptr [edi + 0x41], 1
        je     loc_1_1604
        mov    ebp, dword ptr [esp + 0x18]
        or     ebp, ebp
        mov    eax, ebp
        jne    loc_1_15D6
        call   sub_1_0BB2
loc_1_15D6:
        cmp    ebp, 1
        sbb    ecx, ecx
        inc    ecx
        push   ecx
        push   eax
        push   dword ptr [esi + 0x20]
        push   dword ptr [esi + 0x1c]
        push   1
        push   edi
        call   sub_1_01AC
        add    esp, 0x18
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 0xffff2748
        add    eax, 0xffff
        mov    ecx, dword ptr [edi + 0x6c]
        mov    dword ptr [esi + 0x24], ecx
loc_1_1604:
        or     eax, eax
        jne    loc_1_161A
        cmp    dword ptr [edi + 0x2c], 2
        jne    loc_1_161A
        mov    ecx, dword ptr [esi + 0xc]
        cmp    dword ptr [ebx], ecx
        jbe    loc_1_161A
        mov    eax, 0x2738
loc_1_161A:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  000h
loc_1_1620:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0xc]
        push   dword ptr [edi]
        push   dword ptr [edi + 0xc]
        call   sub_1_1776
        add    esp, 8
        or     eax, eax
        jne    loc_1_1657
        push   dword ptr [edi + 4]
        push   dword ptr [edi + 0x10]
        call   sub_1_1776
        add    esp, 8
        or     eax, eax
        jne    loc_1_1691
        push   dword ptr [edi + 8]
        push   dword ptr [edi + 0x14]
        call   sub_1_1776
        add    esp, 8
loc_1_1657:
        or     eax, eax
        jne    loc_1_1691
        mov    esi, dword ptr [esp + 0x10]
        or     esi, esi
        mov    eax, esi
        jne    loc_1_166A
        call   sub_1_0BB2
loc_1_166A:
        cmp    esi, 1
        sbb    ecx, ecx
        inc    ecx
        push   ecx
        push   eax
        push   dword ptr [edi + 0x1c]
        push   dword ptr [edi + 0x18]
        push   dword ptr [edi + 8]
        push   dword ptr [edi + 0x14]
        push   dword ptr [edi + 4]
        push   dword ptr [edi + 0x10]
        push   dword ptr [edi]
        push   dword ptr [edi + 0xc]
        call   sub_1_02DD
        add    esp, 0x28
loc_1_1691:
        pop    edi
        pop    esi
        ret    
loc_1_1694:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x10]
        mov    esi, dword ptr [esp + 0xc]
        push   edi
        push   dword ptr [esi]
        push   dword ptr [esi + 0xc]
        call   sub_1_181A
        add    esp, 0xc
        push   edi
        push   dword ptr [esi + 4]
        push   dword ptr [esi + 0x10]
        call   sub_1_181A
        add    esp, 0xc
        push   edi
        push   dword ptr [esi + 8]
        push   dword ptr [esi + 0x14]
        call   sub_1_181A
        add    esp, 0xc
        sub    eax, eax
        pop    edi
        pop    esi
        ret    
loc_1_16CF:
        push   ebx
        push   esi
        push   edi
        push   ebp
        cmp    dword ptr [0x2288], 0         ; -> obj1:0x2288
        jne    loc_1_16E6
        mov    eax, 0x276d
        jmp    loc_1_1771
loc_1_16E6:
        mov    esi, dword ptr [esp + 0x14]
        mov    edi, dword ptr [esi]
        mov    ebx, dword ptr [esi + 0xc]
        mov    eax, dword ptr [edi + 0x4c]
        cmp    eax, 0xa
        jne    loc_1_16FE
        mov    eax, 0x2742
        jmp    loc_1_1771
loc_1_16FE:
        or     ebx, ebx
        jne    loc_1_170A
        sub    ecx, ecx
        sub    ebp, ebp
        sub    edx, edx
        jmp    loc_1_172D
loc_1_170A:
        mov    ecx, dword ptr [esi + 4]
        mov    ebp, dword ptr [esi + 8]
        mov    edx, dword ptr [edi + 0x54]
        cmp    eax, 2
        je     loc_1_171F
        cmp    eax, 3
        je     loc_1_172A
        jmp    loc_1_172D
loc_1_171F:
        cmp    dword ptr [edi + 0x2c], 1
        jne    loc_1_172D
        or     edx, 2
        jmp    loc_1_172D
loc_1_172A:
        or     edx, 0x37
loc_1_172D:
        and    dword ptr [edi + 0x40], 0xfffffeff
        mov    dword ptr [edi + 0x60], ebx
        mov    dword ptr [edi + 0x54], edx
        mov    dword ptr [edi + 0x5c], ebp
        not    edx
        mov    dword ptr [edi + 0x58], ecx
        mov    eax, dword ptr [edi + 0x50]
        and    eax, edx
        and    ebx, eax
        mov    ebp, 1
loc_1_174E:
        test   ebp, ebx
        je     loc_1_175E
        push   0
        push   ebp
        push   edi
        call   sub_1_05A6
        add    esp, 0xc
loc_1_175E:
        add    ebp, ebp
        test   ebp, 0x3f
        jne    loc_1_174E
        mov    dword ptr [esi + 0xc], 0
        sub    eax, eax
loc_1_1771:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_1776:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 0x14]
        or     edi, edi
        jne    loc_1_1789
        sub    eax, eax
        jmp    loc_1_1815
loc_1_1789:
        mov    ebx, edi
        imul   ebx, ebx, 0xc
        push   ebx
        push   dword ptr [esp + 0x1c]
        call   sub_1_0A54
        mov    dword ptr [esp + 0x20], eax
        add    esp, 8
        or     eax, eax
        jne    loc_1_17AA
        mov    eax, 0x271e
        jmp    loc_1_1815
loc_1_17AA:
        sub    esi, esi
        mov    ebp, dword ptr [esp + 0x18]
        mov    eax, edi
        dec    edi
        cmp    eax, esi
        je     loc_1_1806
loc_1_17B7:
        mov    eax, dword ptr [ebp]
        or     eax, eax
        je     loc_1_1801
        push   eax
        call   sub_1_18BE
        add    esp, 4
        or     eax, eax
        je     loc_1_1801
        mov    ecx, dword ptr [ebp]
        or     ecx, ecx
        je     loc_1_1801
        mov    eax, dword ptr [ecx + 0x4c]
        cmp    eax, 9
        je     loc_1_1801
        cmp    eax, 8
        je     loc_1_1801
        cmp    eax, 0xa
        je     loc_1_17FA
        test   byte ptr [ebp + 4], 0x20
        je     loc_1_17EE
        and    dword ptr [ecx + 0x54], 0xffffffdf
loc_1_17EE:
        add    ebp, 0xc
        mov    eax, edi
        dec    edi
        or     eax, eax
        jne    loc_1_17B7
        jmp    loc_1_1806
loc_1_17FA:
        mov    esi, 0x2742
        jmp    loc_1_1806
loc_1_1801:
        mov    esi, 0x2736
loc_1_1806:
        push   ebx
        push   dword ptr [esp + 0x1c]
        call   sub_1_0A9B
        add    esp, 8
        mov    eax, esi
loc_1_1815:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_181A:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        cmp    dword ptr [esp + 0x18], 0
        je     loc_1_18B6
        mov    esi, dword ptr [esp + 0x1c]
        mov    eax, dword ptr [esp + 0x20]
        or     eax, eax
        jne    loc_1_183D
        call   sub_1_0BB2
loc_1_183D:
        mov    dword ptr [esp + 0x10], eax
        mov    eax, dword ptr [esp + 0x18]
        dec    dword ptr [esp + 0x18]
        or     eax, eax
        je     loc_1_18B6
loc_1_184D:
        mov    edi, dword ptr [esi]
        mov    ebx, dword ptr [esi + 4]
        push   edi
        call   sub_1_18BE
        add    esp, 4
        or     eax, eax
        jne    loc_1_1864
        add    esi, 0xc
        jmp    loc_1_18AA
loc_1_1864:
        mov    eax, dword ptr [edi + 0x54]
        not    eax
        and    eax, dword ptr [edi + 0x50]
        test   ebx, eax
        jne    loc_1_1876
        mov    dword ptr [esi], 0
loc_1_1876:
        add    esi, 0xc
        mov    ebp, dword ptr [edi + 8]
        mov    eax, edi
        sub    eax, ebp
        cmp    eax, -8
        je     loc_1_18AA
loc_1_1885:
        mov    eax, ebp
        mov    ebp, dword ptr [ebp]
        cmp    dword ptr [eax + 0x18], ebx
        jne    loc_1_18A1
        mov    ecx, dword ptr [esp + 0x10]
        cmp    dword ptr [eax + 0x20], ecx
        jne    loc_1_18A1
        push   eax
        call   sub_1_036E
        add    esp, 4
loc_1_18A1:
        mov    eax, edi
        sub    eax, ebp
        cmp    eax, -8
        jne    loc_1_1885
loc_1_18AA:
        mov    eax, dword ptr [esp + 0x18]
        dec    dword ptr [esp + 0x18]
        or     eax, eax
        jne    loc_1_184D
loc_1_18B6:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_1_18BE:
        mov    eax, dword ptr [esp + 4]
        mov    ecx, dword ptr [eax + 0x4c]
        cmp    ecx, 1
        jb     loc_1_192E
        cmp    ecx, 0xa
        ja     loc_1_192E
        cmp    ecx, 8
        je     loc_1_192E
        cmp    ecx, 9
        je     loc_1_192E
        cmp    ecx, 0xa
        je     loc_1_192E
        test   dword ptr [eax + 0x3c], 0xfffffffc
        jne    loc_1_192E
        test   dword ptr [eax + 0x40], 0xfffffe00
        jne    loc_1_192E
        test   dword ptr [eax + 0x50], 0xffffffc0
        jne    loc_1_192E
        test   dword ptr [eax + 0x54], 0xffffffc0
        jne    loc_1_192E
        test   dword ptr [eax + 0x60], 0xffffffc0
        jne    loc_1_192E
        test   byte ptr [eax + 3], 0x80
        je     loc_1_192E
        test   byte ptr [eax + 7], 0x80
        je     loc_1_192E
        test   byte ptr [eax + 0xb], 0x80
        je     loc_1_192E
        test   byte ptr [eax + 0xf], 0x80
        je     loc_1_192E
        test   byte ptr [eax + 0x1f], 0x80
        mov    eax, 1
        jne    loc_1_1930
loc_1_192E:
        sub    eax, eax
loc_1_1930:
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_1940:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        mov    edi, dword ptr [esi + 8]
        mov    ecx, dword ptr [esi + 0x10]
        mov    edx, dword ptr [esi + 4]
        mov    ebx, dword ptr [edi + 0x4c]
        cmp    ebx, 0xa
        jne    loc_1_1962
        mov    eax, 0x2742
        jmp    loc_1_1A4B
loc_1_1962:
        mov    eax, dword ptr [edi + 0x40]
        test   al, 0x40
        je     loc_1_1973
        mov    eax, 0x274a
        jmp    loc_1_1A4B
loc_1_1973:
        cmp    dword ptr [edi + 0x2c], 2
        jne    loc_1_19BF
        cmp    ebx, 6
        je     loc_1_198C
        or     edx, edx
        jne    loc_1_199A
        mov    eax, 0x2749
        jmp    loc_1_1A4B
loc_1_198C:
        or     edx, edx
        je     loc_1_199A
        mov    eax, 0x2748
        jmp    loc_1_1A4B
loc_1_199A:
        test   cl, 1
        je     loc_1_19A9
        mov    eax, 0x273d
        jmp    loc_1_1A4B
loc_1_19A9:
        or     edx, edx
        je     loc_1_19E9
        movzx  eax, word ptr [edx]
        cmp    dword ptr [edi + 0x28], eax
        je     loc_1_19E9
        mov    eax, 0x273f
        jmp    loc_1_1A4B
loc_1_19BF:
        test   byte ptr [edi + 0x3c], 1
        je     loc_1_19CC
        mov    eax, 0x2746
        jmp    loc_1_1A4B
loc_1_19CC:
        cmp    ebx, 5
        jne    loc_1_19DD
        test   ah, 1
        jne    loc_1_19DD
        mov    eax, 0x2733
        jmp    loc_1_1A4B
loc_1_19DD:
        cmp    ebx, 6
        je     loc_1_19E9
        mov    eax, 0x2749
        jmp    loc_1_1A4B
loc_1_19E9:
        lea    eax, [esi + 0x18]
        push   eax
        push   dword ptr [esi + 0x14]
        push   edx
        push   ecx
        push   dword ptr [esi + 0xc]
        push   dword ptr [esi]
        mov    eax, dword ptr [edi + 0x1c]
        push   edi
        call   dword ptr [eax + 0x20]
        add    esp, 0x1c
        cmp    eax, 0x2733
        jne    loc_1_1A4B
        test   byte ptr [edi + 0x41], 1
        je     loc_1_1A4B
        mov    ebx, dword ptr [esp + 0x14]
        or     ebx, ebx
        mov    eax, ebx
        jne    loc_1_1A1D
        call   sub_1_0BB2
loc_1_1A1D:
        cmp    ebx, 1
        sbb    ecx, ecx
        inc    ecx
        push   ecx
        push   eax
        push   dword ptr [esi + 0x20]
        push   dword ptr [esi + 0x1c]
        push   2
        push   edi
        call   sub_1_01AC
        add    esp, 0x18
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 0xffff2748
        add    eax, 0xffff
        mov    ecx, dword ptr [edi + 0x70]
        mov    dword ptr [esi + 0x24], ecx
loc_1_1A4B:
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  000h
loc_1_1A50:
        mov    eax, dword ptr [esp + 4]
        mov    edx, dword ptr [eax]
        mov    ecx, dword ptr [eax + 4]
        mov    eax, dword ptr [edx + 0x4c]
        cmp    eax, 0xa
        jne    loc_1_1A68
        mov    eax, 0x2742
        jmp    loc_1_1AB5
loc_1_1A68:
        cmp    dword ptr [edx + 0x2c], 1
        jne    loc_1_1A7F
        cmp    eax, 6
        je     loc_1_1A7F
        cmp    eax, 7
        je     loc_1_1A7F
        mov    eax, 0x2749
        jmp    loc_1_1AB5
loc_1_1A7F:
        or     ecx, ecx
        je     loc_1_1A94
        cmp    ecx, 1
        je     loc_1_1A9D
        cmp    ecx, 2
        je     loc_1_1AA3
        mov    eax, 0x2726
        jmp    loc_1_1AB5
loc_1_1A94:
        or     dword ptr [edx + 0x40], 0x80
        jmp    loc_1_1AAA
loc_1_1A9D:
        or     dword ptr [edx + 0x40], 0x40
        jmp    loc_1_1AAA
loc_1_1AA3:
        or     dword ptr [edx + 0x40], 0xc0
loc_1_1AAA:
        push   ecx
        mov    eax, dword ptr [edx + 0x1c]
        push   edx
        call   dword ptr [eax + 0x34]
        add    esp, 8
loc_1_1AB5:
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_1AC0:
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x1c]
        mov    edi, 0x21f0                   ; -> obj1:0x21F0
        mov    eax, dword ptr [esi]
        mov    dword ptr [esp + 0x14], eax
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [esp + 0xc], eax
        sub    ebx, ebx
        mov    eax, dword ptr [esi + 8]
        mov    dword ptr [esp + 0x10], eax
        sub    eax, eax
        mov    ecx, dword ptr [0x2284]       ; -> obj1:0x2284
        cmp    ecx, eax
        je     loc_1_1B1A
loc_1_1AF1:
        push   dword ptr [edi + 0xc]
        mov    eax, dword ptr [edi + 8]
        push   dword ptr [esp + 0x14]
        push   dword ptr [esp + 0x14]
        push   dword ptr [esp + 0x20]
        call   dword ptr [eax]
        add    esp, 0x10
        or     eax, eax
        mov    ecx, dword ptr [0x2284]       ; -> obj1:0x2284
        je     loc_1_1B1A
        add    edi, 0x10
        inc    ebx
        cmp    ebx, ecx
        jb     loc_1_1AF1
loc_1_1B1A:
        cmp    ebx, ecx
        jae    loc_1_1C24
        lea    eax, [esp + 0x10]
        push   eax
        lea    eax, [esp + 0x10]
        push   eax
        lea    eax, [esp + 0x1c]
        push   eax
        mov    eax, dword ptr [edi + 8]
        push   dword ptr [edi + 0xc]
        call   dword ptr [eax + 4]
        add    esp, 0x10
        mov    ebx, eax
        or     ebx, ebx
        jne    loc_1_1B4D
        mov    eax, 0x2747
        jmp    loc_1_1C24
loc_1_1B4D:
        lea    eax, [ebx + 8]
        mov    dword ptr [ebx + 0xc], eax
        mov    dword ptr [eax], eax
        mov    eax, dword ptr [esi + 0x10]
        mov    dword ptr [ebx + 0x68], eax
        mov    dword ptr [ebx + 0x4c], 1
        mov    dword ptr [ebx + 0x40], 0x100
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [ebx + 0x28], eax
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [ebx + 0x2c], eax
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [ebx + 0x30], eax
        mov    dword ptr [ebx + 0x14], 0
        mov    dword ptr [ebx + 0x20], 0
        mov    dword ptr [ebx + 0x18], 0
        mov    dword ptr [ebx + 0x24], 0
        mov    dword ptr [ebx + 0x3c], 0
        mov    dword ptr [ebx + 0x58], 0
        mov    dword ptr [ebx + 0x5c], 0
        mov    dword ptr [ebx + 0x60], 0
        mov    dword ptr [ebx + 0x54], 0x3f
        cmp    dword ptr [esp + 0xc], 2
        jne    loc_1_1BCB
        mov    dword ptr [ebx + 0x54], 0x3d
loc_1_1BCB:
        mov    dword ptr [ebx + 0x50], 2
        mov    eax, dword ptr [edi + 8]
        mov    dword ptr [ebx + 0x1c], eax
        mov    dword ptr [ebx + 0x44], 0
        mov    dword ptr [ebx + 0x48], 0
        mov    dword ptr [ebx + 0x70], 0xffffffff
        mov    dword ptr [ebx + 0x6c], 0xffffffff
        mov    eax, dword ptr [esp + 0x20]
        or     eax, eax
        jne    loc_1_1C01
        call   sub_1_0BBD
loc_1_1C01:
        mov    dword ptr [ebx + 0x64], eax
        mov    dword ptr [ebx], 0x227c       ; -> obj1:0x227C
        mov    eax, dword ptr [0x2280]       ; -> obj1:0x2280
        mov    dword ptr [ebx + 4], eax
        mov    eax, dword ptr [0x2280]       ; -> obj1:0x2280
        mov    dword ptr [eax], ebx
        sub    eax, eax
        mov    dword ptr [0x2280], ebx       ; -> obj1:0x2280
        mov    dword ptr [esi + 0xc], ebx
loc_1_1C24:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
loc_1_1C2B:
        push   esi
        mov    eax, dword ptr [esp + 8]
        mov    esi, dword ptr [eax]
        test   byte ptr [esi + 0x41], 1
        jne    loc_1_1C4D
        cmp    word ptr [esi + 0x10], 0
        je     loc_1_1C4D
        cmp    word ptr [esi + 0x12], 0
        je     loc_1_1C4D
        mov    eax, 0x2733
        jmp    loc_1_1C71
loc_1_1C4D:
        cmp    dword ptr [esi + 0x4c], 9
        mov    eax, 0
        je     loc_1_1C71
        push   0xfffe
        push   esi
        call   sub_1_054E
        add    esp, 8
        push   0
        push   esi
        call   sub_1_1CB8
        add    esp, 8
loc_1_1C71:
        pop    esi
        ret    
sub_1_1C73:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [0x227c]       ; -> obj1:0x227C
        cmp    edi, 0x227c                   ; -> obj1:0x227C
        je     loc_1_1CB3
        mov    esi, dword ptr [esp + 0x14]
        mov    ebx, 1
        sub    ebp, ebp
loc_1_1C90:
        mov    eax, edi
        mov    edi, dword ptr [edi]
        cmp    dword ptr [eax + 0x64], esi
        jne    loc_1_1CAB
        mov    word ptr [eax + 0x10], bx
        mov    word ptr [eax + 0x12], bp
        push   ebx
        push   eax
        call   sub_1_1CB8
        add    esp, 8
loc_1_1CAB:
        cmp    edi, 0x227c                   ; -> obj1:0x227C
        jne    loc_1_1C90
loc_1_1CB3:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_1CB8:
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        mov    eax, dword ptr [esi + 8]
        lea    edi, [esi + 8]
        sub    eax, esi
        cmp    eax, 8
        je     loc_1_1CF7
        mov    ebx, dword ptr [esp + 0x14]
loc_1_1CD0:
        mov    eax, dword ptr [edi]
        mov    edx, dword ptr [edi]
        mov    ecx, dword ptr [eax]
        mov    dword ptr [ecx + 4], edi
        mov    eax, dword ptr [eax]
        mov    dword ptr [edi], eax
        or     dword ptr [edx + 0x14], 8
        or     ebx, ebx
        je     loc_1_1CEE
        push   edx
        call   sub_1_036E
        add    esp, 4
loc_1_1CEE:
        mov    eax, dword ptr [edi]
        sub    eax, esi
        cmp    eax, 8
        jne    loc_1_1CD0
loc_1_1CF7:
        mov    dword ptr [esi + 0x60], 0
        mov    dword ptr [esi + 0x4c], 9
        mov    ecx, dword ptr [esi + 4]
        mov    eax, dword ptr [esi]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [esi + 4]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [esi + 0x1c]
        push   esi
        call   dword ptr [eax + 8]
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_1D30:
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 0x28]
        mov    esi, dword ptr [eax + 4]
        mov    ebx, dword ptr [eax]
        add    eax, 0x10
        mov    dword ptr [esp + 0x18], eax
        mov    eax, dword ptr [eax]
        mov    dword ptr [esp + 0x20], eax
        mov    eax, dword ptr [esp + 0x28]
        cmp    dword ptr [esi + 0x4c], 0xa
        mov    eax, dword ptr [eax + 8]
        mov    dword ptr [esp + 0x1c], eax
        mov    eax, dword ptr [esp + 0x28]
        mov    edi, dword ptr [eax + 0xc]
        jne    loc_1_1D72
        mov    dword ptr [esp + 0x10], 0x2742
        jmp    loc_1_1F79
loc_1_1D72:
        push   edi
        push   dword ptr [esp + 0x20]
        push   esi
        call   sub_1_212C
        add    esp, 0xc
        or     eax, eax
        jne    loc_1_1D91
        mov    dword ptr [esp + 0x10], 0x273a
        jmp    loc_1_1F79
loc_1_1D91:
        push   dword ptr [esp + 0x20]
        push   ebx
        call   sub_1_0A54
        mov    dword ptr [esp + 0x1c], eax
        add    esp, 8
        or     eax, eax
        jne    loc_1_1DB3
        mov    dword ptr [esp + 0x10], 0x271e
        jmp    loc_1_1F79
loc_1_1DB3:
        mov    dword ptr [esp + 0x10], 0
        sub    ebx, ebx
        sub    ebp, ebp
        cmp    dword ptr [esp + 0x1c], 0xffff
        je     loc_1_1DE9
        push   dword ptr [esp + 0x18]
        mov    eax, dword ptr [esi + 0x1c]
        push   dword ptr [esp + 0x18]
        push   edi
        push   dword ptr [esp + 0x28]
        push   esi
        call   dword ptr [eax + 0x28]
        mov    dword ptr [esp + 0x24], eax
        add    esp, 0x14
        jmp    loc_1_1F4E
loc_1_1DE9:
        cmp    edi, 0x1001
        ja     loc_1_1E4B
        je     loc_1_1F23
        cmp    edi, 0x10
        ja     loc_1_1E28
        je     loc_1_1EE6
        cmp    edi, 1
        je     loc_1_1EC3
        cmp    edi, 2
        je     loc_1_1ECB
        cmp    edi, 4
        je     loc_1_1ED6
        cmp    edi, 8
        je     loc_1_1EDE
        jmp    loc_1_1EA3
loc_1_1E28:
        cmp    edi, 0x20
        je     loc_1_1EEE
        cmp    edi, 0x80
        je     loc_1_1EF6
        cmp    edi, 0x100
        je     loc_1_1F1B
        jmp    loc_1_1EA3
loc_1_1E4B:
        cmp    edi, 0x1007
        ja     loc_1_1E7F
        je     loc_1_1F37
        cmp    edi, 0x1002
        je     loc_1_1F28
        cmp    edi, 0x1005
        je     loc_1_1F2D
        cmp    edi, 0x1006
        je     loc_1_1F32
        jmp    loc_1_1EA3
loc_1_1E7F:
        cmp    edi, 0x1008
        je     loc_1_1F3B
        cmp    edi, 0xff7f
        je     loc_1_1F40
        cmp    edi, 0xffffff7f
        je     loc_1_1F40
loc_1_1EA3:
        push   dword ptr [esp + 0x18]
        mov    eax, dword ptr [esi + 0x1c]
        push   dword ptr [esp + 0x18]
        push   edi
        push   dword ptr [esp + 0x28]
        push   esi
        call   dword ptr [eax + 0x28]
        mov    dword ptr [esp + 0x24], eax
        add    esp, 0x14
        jmp    loc_1_1F4E
loc_1_1EC3:
        mov    ebx, dword ptr [esi + 0x40]
        and    ebx, 2
        jmp    loc_1_1F49
loc_1_1ECB:
        mov    eax, dword ptr [esi + 0x4c]
        sub    eax, 3
        cmp    eax, 1
        jmp    loc_1_1F45
loc_1_1ED6:
        mov    ebx, dword ptr [esi + 0x40]
        and    ebx, 0x20
        jmp    loc_1_1F49
loc_1_1EDE:
        mov    ebx, dword ptr [esi + 0x40]
        and    ebx, 8
        jmp    loc_1_1F49
loc_1_1EE6:
        mov    ebx, dword ptr [esi + 0x40]
        and    ebx, 4
        jmp    loc_1_1F49
loc_1_1EEE:
        mov    ebx, dword ptr [esi + 0x40]
        and    ebx, 1
        jmp    loc_1_1F49
loc_1_1EF6:
        cmp    dword ptr [esp + 0x20], 4
        jae    loc_1_1F07
        mov    dword ptr [esp + 0x10], 0x271e
        jmp    loc_1_1F4E
loc_1_1F07:
        add    esi, 0x10
        mov    edi, dword ptr [esp + 0x14]
        movsd  dword ptr es:[edi], dword ptr [esi]
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [eax], 4
        jmp    loc_1_1F4E
loc_1_1F1B:
        mov    ebx, dword ptr [esi + 0x40]
        and    ebx, 0x10
        jmp    loc_1_1F49
loc_1_1F23:
        mov    ebx, dword ptr [esi + 0x34]
        jmp    loc_1_1F49
loc_1_1F28:
        mov    ebx, dword ptr [esi + 0x38]
        jmp    loc_1_1F49
loc_1_1F2D:
        mov    ebx, dword ptr [esi + 0x70]
        jmp    loc_1_1F49
loc_1_1F32:
        mov    ebx, dword ptr [esi + 0x6c]
        jmp    loc_1_1F49
loc_1_1F37:
        sub    ebx, ebx
        jmp    loc_1_1F49
loc_1_1F3B:
        mov    ebx, dword ptr [esi + 0x2c]
        jmp    loc_1_1F49
loc_1_1F40:
        cmp    word ptr [esi + 0x10], 1
loc_1_1F45:
        sbb    ebx, ebx
        neg    ebx
loc_1_1F49:
        mov    ebp, 1
loc_1_1F4E:
        or     ebp, ebp
        je     loc_1_1F69
        mov    eax, dword ptr [esp + 0x28]
        mov    dword ptr [eax + 0x14], ebx
        mov    dword ptr [eax], 0
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [eax], 0
loc_1_1F69:
        push   dword ptr [esp + 0x20]
        push   dword ptr [esp + 0x18]
        call   sub_1_0A9B
        add    esp, 8
loc_1_1F79:
        mov    eax, dword ptr [esp + 0x10]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    
loc_1_1F85:
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ecx, dword ptr [esp + 0x20]
        mov    ebx, dword ptr [ecx + 4]
        mov    esi, dword ptr [ecx]
        mov    eax, dword ptr [ecx + 0x10]
        mov    dword ptr [esp + 0x14], eax
        mov    eax, dword ptr [ecx + 8]
        mov    dword ptr [esp + 0x18], eax
        mov    ebp, dword ptr [ecx + 0xc]
        mov    edi, dword ptr [ecx + 0x14]
        cmp    dword ptr [ebx + 0x4c], 0xa
        jne    loc_1_1FB9
        mov    esi, 0x2742
        jmp    loc_1_2122
loc_1_1FB9:
        push   ebp
        push   dword ptr [esp + 0x1c]
        push   ebx
        call   sub_1_212C
        add    esp, 0xc
        or     eax, eax
        jne    loc_1_1FD5
        mov    esi, 0x273a
        jmp    loc_1_2122
loc_1_1FD5:
        push   dword ptr [esp + 0x14]
        push   esi
        call   sub_1_0A54
        mov    dword ptr [esp + 0x18], eax
        add    esp, 8
        or     eax, eax
        jne    loc_1_1FF4
        mov    esi, 0x271e
        jmp    loc_1_2122
loc_1_1FF4:
        cmp    dword ptr [esp + 0x18], 0xffff
        jne    loc_1_20F9
        cmp    ebp, 0x80
        ja     loc_1_2032
        je     loc_1_20C3
        cmp    ebp, 1
        je     loc_1_2073
        cmp    ebp, 4
        je     loc_1_2083
        cmp    ebp, 8
        je     loc_1_2093
        cmp    ebp, 0x10
        je     loc_1_20A3
        cmp    ebp, 0x20
        je     loc_1_20B3
        jmp    loc_1_20F9
loc_1_2032:
        cmp    ebp, 0x100
        je     loc_1_20D4
        cmp    ebp, 0x1005
        je     loc_1_20E4
        cmp    ebp, 0x1006
        je     loc_1_20E9
        cmp    ebp, 0xff7f
        je     loc_1_20EE
        cmp    ebp, 0xffffff7f
        je     loc_1_20EE
        jmp    loc_1_20F9
loc_1_2073:
        or     edi, edi
        je     loc_1_207D
        or     dword ptr [ebx + 0x40], 2
        jmp    loc_1_20F9
loc_1_207D:
        and    dword ptr [ebx + 0x40], 0xfffffffd
        jmp    loc_1_20F9
loc_1_2083:
        or     edi, edi
        je     loc_1_208D
        or     dword ptr [ebx + 0x40], 0x20
        jmp    loc_1_20F9
loc_1_208D:
        and    dword ptr [ebx + 0x40], 0xffffffdf
        jmp    loc_1_20F9
loc_1_2093:
        or     edi, edi
        je     loc_1_209D
        or     dword ptr [ebx + 0x40], 8
        jmp    loc_1_20F9
loc_1_209D:
        and    dword ptr [ebx + 0x40], 0xfffffff7
        jmp    loc_1_20F9
loc_1_20A3:
        or     edi, edi
        je     loc_1_20AD
        or     dword ptr [ebx + 0x40], 4
        jmp    loc_1_20F9
loc_1_20AD:
        and    dword ptr [ebx + 0x40], 0xfffffffb
        jmp    loc_1_20F9
loc_1_20B3:
        or     edi, edi
        je     loc_1_20BD
        or     dword ptr [ebx + 0x40], 1
        jmp    loc_1_20F9
loc_1_20BD:
        and    dword ptr [ebx + 0x40], 0xfffffffe
        jmp    loc_1_20F9
loc_1_20C3:
        cmp    dword ptr [esp + 0x14], 4
        jb     loc_1_20F9
        lea    edi, [ebx + 0x10]
        mov    esi, dword ptr [esp + 0x10]
        movsd  dword ptr es:[edi], dword ptr [esi]
        jmp    loc_1_20F9
loc_1_20D4:
        or     edi, edi
        je     loc_1_20DE
        or     dword ptr [ebx + 0x40], 0x10
        jmp    loc_1_20F9
loc_1_20DE:
        and    dword ptr [ebx + 0x40], 0xffffffef
        jmp    loc_1_20F9
loc_1_20E4:
        mov    dword ptr [ebx + 0x70], edi
        jmp    loc_1_20F9
loc_1_20E9:
        mov    dword ptr [ebx + 0x6c], edi
        jmp    loc_1_20F9
loc_1_20EE:
        cmp    edi, 1
        sbb    eax, eax
        neg    eax
        mov    word ptr [ebx + 0x10], ax
loc_1_20F9:
        push   dword ptr [esp + 0x14]
        mov    eax, dword ptr [ebx + 0x1c]
        push   dword ptr [esp + 0x14]
        push   ebp
        push   dword ptr [esp + 0x24]
        push   ebx
        call   dword ptr [eax + 0x2c]
        add    esp, 0x14
        mov    esi, eax
        push   dword ptr [esp + 0x14]
        push   dword ptr [esp + 0x14]
        call   sub_1_0A9B
        add    esp, 8
loc_1_2122:
        mov    eax, esi
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
sub_1_212C:
        cmp    dword ptr [esp + 8], 0xffff
        je     loc_1_213D
        mov    eax, 1
        jmp    loc_1_21A6
loc_1_213D:
        mov    eax, dword ptr [esp + 0xc]
        cmp    eax, 0x100
        ja     loc_1_2160
        je     loc_1_2183
        cmp    eax, 2
        je     loc_1_2183
        cmp    eax, 8
        je     loc_1_2183
        cmp    eax, 0x20
        je     loc_1_2195
        cmp    eax, 0x80
        jmp    loc_1_217A
loc_1_2160:
        cmp    eax, 0x1002
        je     loc_1_217C
        cmp    eax, 0x1008
        je     loc_1_217C
        cmp    eax, 0xff7f
        je     loc_1_2183
        cmp    eax, 0xffffff7f
loc_1_217A:
        je     loc_1_2183
loc_1_217C:
        mov    eax, 1
        jmp    loc_1_21A6
loc_1_2183:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 0x2c]
        sub    eax, 2
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        jmp    loc_1_21A6
loc_1_2195:
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 0x2c]
        sub    eax, 2
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
loc_1_21A6:
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_21B0:
        push   ebx
        jns    0x2226
        je     0x221a
        insd   dword ptr es:[edi], dx
        pop    esp
        inc    ebx
        jne    0x222c
        jb     loc_1_2221
        outsb  dx, byte ptr [esi]
        je     0x2202
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        je     loc_1_2235
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        push   ebx
        je     loc_1_2225
        push   ebx
        jb     loc_1_2243
        imul   esp, dword ptr [ebx + 0x65], 0x78565c73
        inc    esp
        pop    esp
        push   edi
        imul   ebp, dword ptr [esi + 0x73], 0x6b636f
        add    byte ptr [eax], al
loc_1_21E0:
        pop    esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2221:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2225:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2235:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_2243:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
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

VxD__LTEXT SEGMENT   ; object 2  vsize=0x494  R-X
        db  0C7h, 005h
        dd  obj1:0x2278              ; fixup
        dd  obj1:0x2274              ; fixup
        db  0C7h, 005h
        dd  obj1:0x2274              ; fixup
        dd  obj1:0x2274              ; fixup
        db  0C7h, 005h
        dd  obj1:0x21EC              ; fixup
        dd  obj1:0x21E8              ; fixup
        db  0C7h, 005h
        dd  obj1:0x21E8              ; fixup
        dd  obj1:0x21E8              ; fixup
        db  0C7h, 005h
        dd  obj1:0x2280              ; fixup
        dd  obj1:0x227C              ; fixup
        db  0C7h, 005h
        dd  obj1:0x227C              ; fixup
        dd  obj1:0x227C              ; fixup
        db  0C7h, 005h
        dd  obj1:0x2270              ; fixup
        db  000h, 000h, 000h, 000h, 068h, 000h, 020h, 000h, 000h, 0E8h
        dd  sub_1_0C28               ; fixup
        db  083h, 0C4h, 004h, 0A3h
        dd  obj1:0x21E4              ; fixup
        db  00Bh, 0C0h, 075h, 004h, 02Bh, 0C0h, 0EBh, 01Fh, 0A3h
        dd  obj1:0x21E4              ; fixup
        db  06Ah, 01Ch, 068h, 000h, 020h, 000h, 000h, 050h, 0E8h
        dd  loc_1_0BD0               ; fixup
        db  083h, 0C4h, 00Ch, 0E8h
        dd  loc_1_0C3C               ; fixup
        db  0B8h, 001h, 000h, 000h, 000h, 0C3h
sub_2_0080:
        push   ebx
        push   esi
        push   edi
        push   ebp
        cmp    dword ptr [esp + 0x18], 1
        je     loc_2_0092
        sub    eax, eax
        jmp    loc_2_012C
loc_2_0092:
        sub    edx, edx
        mov    ecx, 0x21f0                   ; -> obj1:0x21F0
        mov    ebp, dword ptr [0x2284]       ; -> obj1:0x2284
        cmp    ebp, edx
        mov    eax, dword ptr [esp + 0x14]
        je     loc_2_00F5
loc_2_00A7:
        or     eax, eax
        je     loc_2_00B3
        cmp    dword ptr [ecx], eax
        je     loc_2_00ED
        or     eax, eax
        jne    loc_2_00E3
loc_2_00B3:
        mov    esi, dword ptr [ecx + 0xc]
        mov    edi, dword ptr [esp + 0x20]
loc_2_00BA:
        mov    bl, byte ptr [esi]
        cmp    bl, byte ptr [edi]
        jne    loc_2_00DA
        or     bl, bl
        je     loc_2_00D6
        mov    bl, byte ptr [esi + 1]
        cmp    bl, byte ptr [edi + 1]
        jne    loc_2_00DA
        add    esi, 2
        add    edi, 2
        or     bl, bl
        jne    loc_2_00BA
loc_2_00D6:
        sub    esi, esi
        jmp    loc_2_00DF
loc_2_00DA:
        sbb    esi, esi
        sbb    esi, -1
loc_2_00DF:
        or     esi, esi
        je     loc_2_00F1
loc_2_00E3:
        inc    edx
        add    ecx, 0x10
        cmp    ebp, edx
        ja     loc_2_00A7
        jmp    loc_2_00F5
loc_2_00ED:
        sub    eax, eax
        jmp    loc_2_012C
loc_2_00F1:
        sub    eax, eax
        jmp    loc_2_012C
loc_2_00F5:
        cmp    ebp, 8
        jb     loc_2_00FE
        sub    eax, eax
        jmp    loc_2_012C
loc_2_00FE:
        mov    ecx, ebp
        shl    ecx, 4
        add    ecx, 0x21f0                   ; -> obj1:0x21F0
        inc    ebp
        mov    dword ptr [0x2284], ebp       ; -> obj1:0x2284
        mov    dword ptr [ecx], eax
        mov    eax, dword ptr [esp + 0x18]
        mov    dword ptr [ecx + 4], eax
        mov    eax, dword ptr [esp + 0x1c]
        mov    dword ptr [ecx + 8], eax
        mov    eax, dword ptr [esp + 0x20]
        mov    dword ptr [ecx + 0xc], eax
        mov    eax, 1
loc_2_012C:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_2_0131:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, 0x21f0                   ; -> obj1:0x21F0
        sub    ebp, ebp
        mov    ecx, dword ptr [0x2284]       ; -> obj1:0x2284
        cmp    ecx, ebp
        je     loc_2_0190
        mov    eax, dword ptr [esp + 0x14]
loc_2_014A:
        or     eax, eax
        je     loc_2_0156
        cmp    dword ptr [edi], eax
        je     loc_2_018E
        or     eax, eax
        jne    loc_2_0186
loc_2_0156:
        mov    edx, dword ptr [edi + 0xc]
        mov    esi, dword ptr [esp + 0x18]
loc_2_015D:
        mov    bl, byte ptr [edx]
        cmp    bl, byte ptr [esi]
        jne    loc_2_017D
        or     bl, bl
        je     loc_2_0179
        mov    bl, byte ptr [edx + 1]
        cmp    bl, byte ptr [esi + 1]
        jne    loc_2_017D
        add    edx, 2
        add    esi, 2
        or     bl, bl
        jne    loc_2_015D
loc_2_0179:
        sub    edx, edx
        jmp    loc_2_0182
loc_2_017D:
        sbb    edx, edx
        sbb    edx, -1
loc_2_0182:
        or     edx, edx
        je     loc_2_018E
loc_2_0186:
        add    edi, 0x10
        inc    ebp
        cmp    ecx, ebp
        ja     loc_2_014A
loc_2_018E:
        cmp    ecx, ebp
loc_2_0190:
        jbe    loc_2_01A6
        dec    ecx
        mov    dword ptr [0x2284], ecx       ; -> obj1:0x2284
        sub    ecx, ebp
        shl    ecx, 4
        lea    esi, [edi + 0x10]
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
loc_2_01A6:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  000h
sub_2_01AC:
        mov    edi, 0x2290                   ; -> obj1:0x2290
        mov    ecx, 0x2290                   ; -> obj1:0x2290
        sub    ecx, edi
        shr    ecx, 2
        xor    eax, eax
        cld    
        rep stosd dword ptr es:[edi], eax
        mov    eax, 0xd38                    ; -> obj1:0x0D38
        mov    dl, byte ptr [eax]
        or     dl, dl
        jne    loc_2_022A
        inc    byte ptr [eax]
        VMMCall  VMM.Get_VMM_Version             ; cd2000000100
        cmp    ax, 0x400
        jb     loc_2_022A
        VxDCall  APM.Service_0000h               ; cd2000002700
        jb     loc_2_022A
        VxDCall  NDIS.Service_0000h              ; cd2000002a00
        jb     loc_2_022A
        VxDCall  SHELL.SHELL_Get_Version         ; cd2000001700
        jb     loc_2_022A
        VxDCall  IFSMgr.IFSMgr_Get_Version       ; cd2000004000
        jb     loc_2_022A
        push   0
        push   0xc
        VMMCall  VMM._Allocate_Device_CB_Area    ; cd20a7000100
        add    esp, 8
        mov    dword ptr [0x228c], eax       ; -> obj1:0x228C
        or     eax, eax
        je     loc_2_022A
        call   0
        or     eax, eax
        je     loc_2_0219
        ret    
loc_2_0219:
        push   0
        push   dword ptr [0x228c]            ; -> obj1:0x228C
        VMMCall  VMM._Deallocate_Device_CB_Area  ; cd2015010100
        add    esp, 8
loc_2_022A:
        stc    
        ret    
loc_2_022C:
        inc    byte ptr [0x490]              ; -> loc_2_0490
        call   sub_2_01AC
        jb     loc_2_0244
        call   sub_2_02AB
        or     eax, eax
        je     loc_2_0244
        clc    
        ret    
loc_2_0244:
        stc    
        ret    
        db  000h, 000h
sub_2_0248:
        push   ebp
        push   esi
        push   edi
        push   ebx
        VMMCall  VMM.Get_Exec_Path               ; cd20b6000100
        mov    eax, edx
        pop    ebx
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_2_0259:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        mov    eax, 1
        VxDCall  APM.Service_0001h               ; cd2001002700
        sbb    eax, eax
        pop    ebx
        pop    edi
        pop    esi
        pop    ebp
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_0280:
        push   edi
        mov    edx, dword ptr [esp + 8]
        mov    edi, edx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        lea    eax, [edx + ecx - 1]
        cmp    eax, edx
        je     loc_2_02A7
        mov    cl, byte ptr [esp + 0xc]
loc_2_029E:
        cmp    byte ptr [eax], cl
        je     loc_2_02A9
        dec    eax
        cmp    eax, edx
        jne    loc_2_029E
loc_2_02A7:
        sub    eax, eax
loc_2_02A9:
        pop    edi
        ret    
sub_2_02AB:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x350
        push   ebx
        push   esi
        push   edi
        lea    eax, [ebp - 0x34]
        mov    dword ptr [ebp - 0x44], eax
        mov    dword ptr [ebp - 0x40], 0x21b0 ; -> loc_1_21B0
        mov    dword ptr [ebp - 0x3c], 0x80000002
        push   dword ptr [ebp - 0x44]
        push   dword ptr [ebp - 0x40]
        push   dword ptr [ebp - 0x3c]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VMMCall  VMM._RegOpenKey                 ; cd2048010100
        add    esp, 0xc
        or     eax, eax
        je     loc_2_02EE
        sub    eax, eax
        jmp    loc_2_0489
loc_2_02EE:
        call   sub_2_0248
        or     eax, eax
        jne    loc_2_02FE
        sub    eax, eax
        jmp    loc_2_0489
loc_2_02FE:
        mov    edi, eax
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [ebp - 0x350]
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        push   0x5c
        lea    eax, [ebp - 0x350]
        push   eax
        call   sub_2_0280
        add    esp, 8
        mov    byte ptr [eax], 0
        mov    dword ptr [ebp - 4], 0
        mov    dword ptr [ebp - 0xc], 0x104
        mov    dword ptr [ebp - 8], 0x104
loc_2_034C:
        lea    eax, [ebp - 0xc]
        mov    dword ptr [ebp - 0x30], eax
        lea    eax, [ebp - 0x24c]
        mov    dword ptr [ebp - 0x2c], eax
        lea    eax, [ebp - 0x10]
        mov    dword ptr [ebp - 0x28], eax
        mov    dword ptr [ebp - 0x24], 0
        lea    eax, [ebp - 8]
        mov    dword ptr [ebp - 0x20], eax
        lea    eax, [ebp - 0x148]
        mov    dword ptr [ebp - 0x1c], eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0x18], eax
        mov    eax, dword ptr [ebp - 0x34]
        mov    dword ptr [ebp - 0x14], eax
        push   dword ptr [ebp - 0x30]
        push   dword ptr [ebp - 0x2c]
        push   dword ptr [ebp - 0x28]
        push   dword ptr [ebp - 0x24]
        push   dword ptr [ebp - 0x20]
        push   dword ptr [ebp - 0x1c]
        push   dword ptr [ebp - 0x18]
        push   dword ptr [ebp - 0x14]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VMMCall  VMM._RegEnumValue               ; cd2050010100
        add    esp, 0x20
        mov    ebx, eax
        or     ebx, ebx
        jne    loc_2_0444
        cmp    dword ptr [ebp - 0x10], 1
        jne    loc_2_044E
        lea    edi, [ebp - 0x350]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        lea    edi, [ebp - 0x148]
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    esi, 0x21e0                   ; -> loc_1_21E0
        lea    edi, [ebp - 0x148]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        dec    edi
        movsw  word ptr es:[edi], word ptr [esi]
        lea    edi, [ebp - 0x24c]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    edx, ecx
        lea    edi, [ebp - 0x148]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        lea    eax, [ebp - 0x148]
        push   eax
        call   sub_2_0259
        add    esp, 4
        jmp    loc_2_044E
loc_2_0444:
        cmp    ebx, 0xea
        jne    loc_2_044E
        sub    ebx, ebx
loc_2_044E:
        mov    dword ptr [ebp - 0xc], 0x104
        mov    dword ptr [ebp - 8], 0x104
        inc    dword ptr [ebp - 4]
        or     ebx, ebx
        je     loc_2_034C
        mov    eax, dword ptr [ebp - 0x34]
        mov    dword ptr [ebp - 0x38], eax
        push   dword ptr [ebp - 0x38]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VMMCall  VMM._RegCloseKey                ; cd2049010100
        add    esp, 4
        cmp    dword ptr [0x2284], 1         ; -> obj1:0x2284
        sbb    eax, eax
        inc    eax
loc_2_0489:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_0490:
        and    edx, dword ptr [edi + 0x53]
        dec    edi
VxD__LTEXT ENDS

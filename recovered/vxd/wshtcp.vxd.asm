; WSHTCP -- reconstructed 32-bit disassembly (VxD / LE format)
; 2 objects, 1290 instructions recovered by recursive descent

Declare_Virtual_Device WSHTCP, 0, 0, WSHTCP_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 080000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0xfb8  R-X
        db  053h, 056h, 057h, 08Bh, 054h, 024h, 010h, 08Bh, 042h, 04Ch, 083h, 0F8h, 00Ah, 075h, 007h, 0B8h
        db  042h, 027h, 000h, 000h, 0EBh, 04Ah, 083h, 0F8h, 006h, 074h, 00Ch, 083h, 0F8h, 007h, 074h, 007h
        db  0B8h, 049h, 027h, 000h, 000h, 0EBh, 039h, 08Bh, 05Ch, 024h, 018h, 08Bh, 042h, 024h, 039h, 003h
        db  073h, 007h, 0B8h, 01Eh, 027h, 000h, 000h, 0EBh, 027h, 08Bh, 07Ch, 024h, 014h, 00Bh, 0FFh, 075h
        db  007h, 0B8h, 01Eh, 027h, 000h, 000h, 0EBh, 018h, 08Bh, 072h, 018h, 08Bh, 0C8h, 0C1h, 0E9h, 002h
        db  0F3h, 0A5h, 08Bh, 0C8h, 083h, 0E1h, 003h, 0F3h, 0A4h, 08Bh, 042h, 024h, 089h, 003h, 02Bh, 0C0h
        db  05Fh, 05Eh, 05Bh, 0C3h
WSHTCP_Control:
        cmp    eax, 1
        je     loc_2_004B
        cmp    eax, 0x1b
        je     loc_2_003B
        clc    
        ret    
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 8]
        VxDCall  Dev_0488.Service_0008h          ; cd2008008804
        add    esp, 4
        leave  
        ret    
        db  0CDh, 020h, 005h, 080h, 089h, 004h, 000h
sub_1_0090:
        VxDCall  IFSMgr.IFSMgr_GetHeap           ; cd200d804000
sub_1_0096:
        cmp    dword ptr [esp + 4], 0
        je     loc_1_00A3
        VxDCall  IFSMgr.IFSMgr_RetHeap           ; cd200e804000
loc_1_00A3:
        ret    
        db  0CDh, 020h, 011h, 080h, 040h, 000h, 000h, 000h
loc_1_00AC:
        lock add dword ptr [eax], eax
        add    byte ptr [edi + 4], bl        ; -> loc_1_045F
        add    byte ptr [eax], al
        cdq    
        add    al, byte ptr [eax]
        add    byte ptr [ecx - 0x1ffffffc], cl ; -> loc_1_0489
        add    eax, 0x4d10000                ; -> loc_1_04D1
        add    byte ptr [eax], al
        sbb    al, 7
        add    byte ptr [eax], al
        out    dx, eax
        or     al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h, 000h, 000h, 000h, 004h, 000h, 000h, 000h, 000h, 000h, 000h, 057h, 053h, 048h, 054h, 043h
        db  050h, 020h, 020h, 000h, 000h, 000h, 080h
        dd  WSHTCP_Control           ; fixup
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h, 032h, 076h, 073h, 052h
        db  033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h
loc_1_0125:
        and    dword ptr [ecx], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebp + 0x53], cl
        push   esp
        inc    ebx
        push   eax
        add    byte ptr [esi + 0x54], dl
        inc    esp
        dec    ecx
        xor    edi, dword ptr cs:[eax]
        add    byte ptr ss:[eax], al
loc_1_013C:
        push   edi
        push   ebx
        dec    eax
        push   esp
        inc    ebx
        push   eax
        add    byte ptr [eax], al
loc_1_0144:
        dec    ebp
        push   ebx
        push   esp
        inc    ebx
        push   eax
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [esi], al
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [esi], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [esi], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [esi], al
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], dl
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], dl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], dl
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], dl
        add    byte ptr [eax], al
        add    byte ptr [ebx + eax*2 + 0x50], dl
        das    
        dec    ecx
        push   eax
        add    byte ptr [eax], al
loc_1_01CC:
        push   ebp
        inc    esp
        push   eax
        das    
        dec    ecx
        push   eax
        add    byte ptr [eax], al
loc_1_01D4:
        push   esp
        inc    ebx
        push   eax
        das    
        dec    ecx
        push   eax
        add    byte ptr [eax], al
loc_1_01DC:
        push   ebp
        inc    esp
        push   eax
        das    
        dec    ecx
        push   eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_01F0:
        push   edi
        mov    edi, dword ptr [esp + 8]
        cmp    word ptr [edi], 2
        je     loc_1_0205
        mov    eax, 0x273f
        jmp    loc_1_0297
loc_1_0205:
        cmp    dword ptr [esp + 0xc], 0x10
        jae    loc_1_0216
        mov    eax, 0x271e
        jmp    loc_1_0297
loc_1_0216:
        mov    eax, dword ptr [edi + 4]
        or     eax, eax
        jne    loc_1_0229
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [ecx], 1
        jmp    loc_1_0253
loc_1_0229:
        cmp    eax, -1
        jne    loc_1_023A
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [ecx], 2
        jmp    loc_1_0253
loc_1_023A:
        cmp    eax, 0x7f000001
        mov    ecx, dword ptr [esp + 0x10]
        jne    loc_1_024D
        mov    dword ptr [ecx], 3
        jmp    loc_1_0253
loc_1_024D:
        mov    dword ptr [ecx], 0
loc_1_0253:
        mov    ax, word ptr [edi + 2]
        or     ax, ax
        jne    loc_1_0265
        mov    dword ptr [ecx + 4], 1
        jmp    loc_1_0290
loc_1_0265:
        mov    dx, ax
        and    edx, 0xff
        shr    ax, 8
        shl    edx, 8
        movzx  eax, ax
        add    edx, eax
        cmp    edx, 0x7d0
        mov    dword ptr [ecx + 4], 2
        jl     loc_1_0290
        mov    dword ptr [ecx + 4], 0
loc_1_0290:
        add    edi, 8
        sub    eax, eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
loc_1_0297:
        pop    edi
        ret    
loc_1_0299:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 0x24]
        cmp    eax, 0xfffe
        mov    ecx, dword ptr [esp + 0x28]
        jne    loc_1_02E4
        cmp    ecx, 1
        jne    loc_1_02E4
        mov    ebp, dword ptr [esp + 0x2c]
        or     ebp, ebp
        mov    edx, dword ptr [esp + 0x30]
        je     loc_1_02D9
        cmp    dword ptr [edx], 0x20
        jae    loc_1_02CC
        mov    eax, 0x271e
        jmp    loc_1_045A
loc_1_02CC:
        mov    edi, ebp
        mov    esi, dword ptr [esp + 0x14]
        mov    ecx, 8
        rep movsd dword ptr es:[edi], dword ptr [esi]
loc_1_02D9:
        mov    dword ptr [edx], 0x20
        jmp    loc_1_0458
loc_1_02E4:
        cmp    eax, 0xffff
        je     loc_1_02FE
        cmp    eax, 6
        je     loc_1_02FE
        or     eax, eax
        je     loc_1_02FE
        mov    eax, 0x2726
        jmp    loc_1_045A
loc_1_02FE:
        mov    edx, dword ptr [esp + 0x30]
        mov    ebx, dword ptr [edx]
        cmp    ebx, 4
        jae    loc_1_0313
        mov    eax, 0x271e
        jmp    loc_1_045A
loc_1_0313:
        cmp    eax, 6
        jne    loc_1_037F
        mov    esi, dword ptr [esp + 0x14]
        cmp    dword ptr [esi + 4], 2
        jne    loc_1_032C
        mov    eax, 0x273a
        jmp    loc_1_045A
loc_1_032C:
        cmp    ecx, 1
        je     loc_1_0343
        cmp    ecx, 0x7000
        je     loc_1_0361
        mov    eax, 0x2726
        jmp    loc_1_045A
loc_1_0343:
        mov    ebp, dword ptr [esp + 0x2c]
        mov    edi, ebp
        sub    eax, eax
        mov    ecx, ebx
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        mov    ecx, ebx
        and    ecx, 3
        rep stosb byte ptr es:[edi], al
        mov    al, byte ptr [esi + 0x1b]
        jmp    loc_1_044F
loc_1_0361:
        mov    ebp, dword ptr [esp + 0x2c]
        mov    edi, ebp
        sub    eax, eax
        mov    ecx, ebx
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        mov    ecx, ebx
        and    ecx, 3
        rep stosb byte ptr es:[edi], al
        mov    al, byte ptr [esi + 0x1c]
        jmp    loc_1_044F
loc_1_037F:
        or     eax, eax
        jne    loc_1_03F5
        mov    esi, dword ptr [esp + 0x14]
        cmp    dword ptr [esi + 8], 6
        jne    loc_1_0397
        mov    eax, 0x273a
        jmp    loc_1_045A
loc_1_0397:
        cmp    ecx, 2
        je     loc_1_03B0
        cmp    ecx, 3
        je     loc_1_03BF
        cmp    ecx, 4
        je     loc_1_03DA
        mov    eax, 0x273a
        jmp    loc_1_045A
loc_1_03B0:
        mov    ebp, dword ptr [esp + 0x2c]
        mov    eax, dword ptr [esi + 0x14]
        mov    dword ptr [ebp], eax
        jmp    loc_1_0452
loc_1_03BF:
        mov    ebp, dword ptr [esp + 0x2c]
        mov    edi, ebp
        sub    eax, eax
        mov    ecx, ebx
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        mov    ecx, ebx
        and    ecx, 3
        rep stosb byte ptr es:[edi], al
        mov    al, byte ptr [esi + 0x10]
        jmp    loc_1_044F
loc_1_03DA:
        mov    ebp, dword ptr [esp + 0x2c]
        mov    edi, ebp
        sub    eax, eax
        mov    ecx, ebx
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        mov    ecx, ebx
        and    ecx, 3
        rep stosb byte ptr es:[edi], al
        mov    al, byte ptr [esi + 0x18]
        jmp    loc_1_044F
loc_1_03F5:
        cmp    ecx, 8
        je     loc_1_0406
        cmp    ecx, 0x10
        je     loc_1_0432
        mov    eax, 0x273a
        jmp    loc_1_045A
loc_1_0406:
        mov    esi, dword ptr [esp + 0x14]
        cmp    dword ptr [esi + 4], 2
        jne    loc_1_0417
        mov    eax, 0x273a
        jmp    loc_1_045A
loc_1_0417:
        mov    ebp, dword ptr [esp + 0x2c]
        mov    edi, ebp
        sub    eax, eax
        mov    ecx, ebx
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        mov    ecx, ebx
        and    ecx, 3
        rep stosb byte ptr es:[edi], al
        mov    al, byte ptr [esi + 0x19]
        jmp    loc_1_044F
loc_1_0432:
        mov    ebp, dword ptr [esp + 0x2c]
        mov    edi, ebp
        sub    eax, eax
        mov    ecx, ebx
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        mov    ecx, ebx
        and    ecx, 3
        rep stosb byte ptr es:[edi], al
        mov    esi, dword ptr [esp + 0x14]
        mov    al, byte ptr [esi + 0x1a]
loc_1_044F:
        mov    byte ptr [ebp], al
loc_1_0452:
        mov    dword ptr [edx], 4
loc_1_0458:
        sub    eax, eax
loc_1_045A:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_045F:
        push   edi
        mov    eax, dword ptr [esp + 0x10]
        cmp    dword ptr [eax], 0x10
        jae    loc_1_0470
        mov    eax, 0x271e
        jmp    loc_1_0487
loc_1_0470:
        mov    dword ptr [eax], 0x10
        mov    ecx, dword ptr [esp + 0xc]
        mov    edi, ecx
        sub    eax, eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        mov    word ptr [ecx], 2
loc_1_0487:
        pop    edi
        ret    
loc_1_0489:
        push   esi
        push   edi
        cmp    dword ptr [esp + 0x10], 0x80
        mov    eax, 0x80
        jb     loc_1_04CE
        mov    eax, dword ptr [esp + 0xc]
        mov    esi, 0x14c                    ; -> obj1:0x014C
        mov    dword ptr [eax], 0xa
        mov    dword ptr [eax + 4], 3
        lea    edi, [eax + 8]
        mov    ecx, 0xf
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    esi, 0x188                    ; -> obj1:0x0188
        lea    edi, [eax + 0x44]
        mov    ecx, 0xf
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    eax, 0x80
loc_1_04CE:
        pop    edi
        pop    esi
        ret    
loc_1_04D1:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ecx, dword ptr [esp + 0x1c]
        mov    ebp, dword ptr [esp + 0x18]
        push   dword ptr [ecx]
        mov    ebx, dword ptr [esp + 0x18]
        push   dword ptr [ebp]
        push   dword ptr [ebx]
        push   5
        push   0x14c                         ; -> obj1:0x014C
        call   sub_1_0E19
        add    esp, 0x14
        or     al, al
        je     loc_1_051C
        mov    eax, dword ptr [0x14c]        ; -> obj1:0x014C
        mov    dword ptr [ebx], eax
        mov    eax, dword ptr [0x150]        ; -> obj1:0x0150
        mov    dword ptr [ebp], eax
        mov    esi, 0x1c4                    ; -> obj1:0x01C4
        mov    ecx, dword ptr [esp + 0x1c]
        mov    eax, dword ptr [0x154]        ; -> obj1:0x0154
        mov    dword ptr [ecx], eax
        jmp    loc_1_055D
loc_1_051C:
        mov    ecx, dword ptr [esp + 0x1c]
        push   dword ptr [ecx]
        push   dword ptr [ebp]
        push   dword ptr [ebx]
        push   5
        push   0x188                         ; -> obj1:0x0188
        call   sub_1_0E19
        add    esp, 0x14
        or     al, al
        je     loc_1_05D6
        mov    eax, dword ptr [0x188]        ; -> obj1:0x0188
        mov    dword ptr [ebx], eax
        mov    eax, dword ptr [0x18c]        ; -> obj1:0x018C
        mov    dword ptr [ebp], eax
        mov    esi, 0x1cc                    ; -> loc_1_01CC
        mov    ecx, dword ptr [esp + 0x1c]
        mov    eax, dword ptr [0x190]        ; -> obj1:0x0190
        mov    dword ptr [ecx], eax
loc_1_055D:
        mov    edi, dword ptr [esp + 0x20]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
        push   0x20
        call   sub_1_0090
        add    esp, 4
        or     eax, eax
        jne    loc_1_057A
        mov    eax, 0x2747
        jmp    loc_1_05DB
loc_1_057A:
        mov    ecx, dword ptr [ebx]
        mov    dword ptr [eax], ecx
        mov    ecx, dword ptr [ebp]
        mov    dword ptr [eax + 4], ecx
        mov    ecx, dword ptr [esp + 0x1c]
        mov    ecx, dword ptr [ecx]
        mov    dword ptr [eax + 8], ecx
        mov    dword ptr [eax + 0xc], 0x2000
        mov    dword ptr [eax + 0x10], 1
        mov    dword ptr [eax + 0x14], 0
        mov    byte ptr [eax + 0x18], 1
        mov    byte ptr [eax + 0x19], 0
        mov    byte ptr [eax + 0x1a], 0
        mov    byte ptr [eax + 0x1b], 0
        mov    byte ptr [eax + 0x1c], 1
        mov    byte ptr [eax + 0x1d], 0
        mov    byte ptr [eax + 0x1e], 0
        mov    ecx, dword ptr [esp + 0x28]
        mov    byte ptr [eax + 0x1f], 0
        mov    dword ptr [ecx], 0x184
        mov    ecx, dword ptr [esp + 0x24]
        mov    dword ptr [ecx], eax
        sub    eax, eax
        jmp    loc_1_05DB
loc_1_05D6:
        mov    eax, 0x2726
loc_1_05DB:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_05E0:
        sub    esp, 0x1c
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0x38]
        cmp    eax, 4
        jne    loc_1_06CE
        mov    dword ptr [esp + 8], 1
        mov    esi, dword ptr [esp + 0x28]
        mov    dword ptr [esp + 0x10], 0
        cmp    byte ptr [esi + 0x19], 0
        je     loc_1_063C
        push   0
        lea    eax, [esp + 0xc]
        push   4
        push   eax
        push   2
        push   0x300
        push   0x200
        mov    edi, dword ptr [esp + 0x4c]
        push   0x400
        push   edi
        call   sub_1_0E56
        add    esp, 0x20
        or     eax, eax
        je     loc_1_0640
        jmp    loc_1_0716
loc_1_063C:
        mov    edi, dword ptr [esp + 0x34]
loc_1_0640:
        cmp    byte ptr [esi + 0x1b], 0
        je     loc_1_0671
        push   0
        lea    eax, [esp + 0xc]
        push   4
        push   eax
        push   1
        push   0x300
        push   0x200
        push   0x400
        push   edi
        call   sub_1_0E56
        add    esp, 0x20
        or     eax, eax
        jne    loc_1_0716
loc_1_0671:
        lea    eax, [esi + 0xc]
        cmp    dword ptr [eax], 0x2000
        je     loc_1_069F
        push   1
        push   4
        push   eax
        push   6
        push   0x300
        push   0x200
        push   0x400
        push   edi
        call   sub_1_0E56
        add    esp, 0x20
        or     eax, eax
        jne    loc_1_0716
loc_1_069F:
        cmp    byte ptr [esi + 0x1c], 0
        jne    loc_1_070D
        push   0
        lea    eax, [esp + 0x14]
        push   4
        push   eax
        push   4
        push   0x300
        push   0x200
        push   0x400
        push   edi
        call   sub_1_0E56
        add    esp, 0x20
        or     eax, eax
        je     loc_1_070D
        jmp    loc_1_0716
loc_1_06CE:
        cmp    eax, 0x80
        jne    loc_1_06E3
        push   dword ptr [esp + 0x28]
        call   sub_1_0096
        add    esp, 4
        jmp    loc_1_070D
loc_1_06E3:
        cmp    eax, 0x100
        jne    loc_1_0711
        mov    dword ptr [esp + 0xc], 0x10
        lea    eax, [esp + 0xc]
        push   eax
        lea    eax, [esp + 0x18]
        push   eax
        push   dword ptr [esp + 0x34]
        call   0
        add    esp, 0xc
        cmp    eax, -1
        je     loc_1_0716
loc_1_070D:
        sub    eax, eax
        jmp    loc_1_0716
loc_1_0711:
        mov    eax, 0x2726
loc_1_0716:
        pop    edi
        pop    esi
        add    esp, 0x1c
        ret    
sub_1_071C:
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 0x30]
        cmp    eax, 0xfffe
        mov    edi, dword ptr [esp + 0x34]
        jne    loc_1_088C
        cmp    edi, 1
        jne    loc_1_088C
        cmp    dword ptr [esp + 0x3c], 0x20
        jae    loc_1_0750
        mov    eax, 0x2726
        jmp    loc_1_0CE7
loc_1_0750:
        mov    esi, dword ptr [esp + 0x20]
        or     esi, esi
        jne    loc_1_0786
        push   0x20
        call   sub_1_0090
        add    esp, 4
        or     eax, eax
        jne    loc_1_0770
        mov    eax, 0x2747
        jmp    loc_1_0CE7
loc_1_0770:
        mov    edi, eax
        mov    ebx, dword ptr [esp + 0x38]
        mov    esi, ebx
        mov    ecx, 8
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    dword ptr [ebx], eax
        jmp    loc_1_0CE5
loc_1_0786:
        mov    dword ptr [esp + 0x14], 1
        mov    ebx, dword ptr [esp + 0x38]
        mov    dword ptr [esp + 0x18], 0
        cmp    byte ptr [ebx + 0x19], 0
        je     loc_1_07CE
        lea    eax, [esp + 0x14]
        push   4
        push   eax
        push   8
        mov    edi, dword ptr [esp + 0x38]
        push   0xffff
        mov    ebp, dword ptr [esp + 0x38]
        push   edi
        push   ebp
        push   dword ptr [esp + 0x3c]
        push   esi
        call   sub_1_071C
        add    esp, 0x20
        or     eax, eax
        je     loc_1_07D6
        jmp    loc_1_0CE7
loc_1_07CE:
        mov    ebp, dword ptr [esp + 0x28]
        mov    edi, dword ptr [esp + 0x2c]
loc_1_07D6:
        cmp    byte ptr [ebx + 0x1a], 0
        je     loc_1_0801
        lea    eax, [esp + 0x14]
        push   4
        push   eax
        push   0x10
        push   0xffff
        push   edi
        push   ebp
        push   dword ptr [esp + 0x3c]
        push   esi
        call   sub_1_071C
        add    esp, 0x20
        or     eax, eax
        jne    loc_1_0CE7
loc_1_0801:
        cmp    byte ptr [ebx + 0x1b], 0
        je     loc_1_0829
        lea    eax, [esp + 0x14]
        push   4
        push   eax
        push   1
        push   6
        push   edi
        push   ebp
        push   dword ptr [esp + 0x3c]
        push   esi
        call   sub_1_071C
        add    esp, 0x20
        or     eax, eax
        jne    loc_1_0CE7
loc_1_0829:
        lea    eax, [ebx + 0xc]
        cmp    dword ptr [eax], 0x2000
        je     loc_1_0858
        push   4
        push   eax
        push   0x1002
        push   0xffff
        push   edi
        push   ebp
        push   dword ptr [esp + 0x3c]
        push   esi
        call   sub_1_071C
        add    esp, 0x20
        or     eax, eax
        jne    loc_1_0CE7
loc_1_0858:
        cmp    byte ptr [ebx + 0x1c], 0
        jne    loc_1_0CE5
        lea    eax, [esp + 0x18]
        push   4
        push   eax
        push   0x7000
        push   6
        push   edi
        push   ebp
        push   dword ptr [esp + 0x3c]
        push   esi
        call   sub_1_071C
        add    esp, 0x20
        or     eax, eax
        jne    loc_1_0CE7
        jmp    loc_1_0CE5
loc_1_088C:
        cmp    eax, 0xffff
        je     loc_1_08A6
        cmp    eax, 6
        je     loc_1_08A6
        or     eax, eax
        je     loc_1_08A6
        mov    eax, 0x2726
        jmp    loc_1_0CE7
loc_1_08A6:
        mov    edx, dword ptr [esp + 0x3c]
        cmp    edx, 4
        jae    loc_1_08B9
        mov    eax, 0x271e
        jmp    loc_1_0CE7
loc_1_08B9:
        mov    ebx, dword ptr [esp + 0x38]
        cmp    eax, 6
        mov    ecx, dword ptr [ebx]
        mov    dword ptr [esp + 0x10], ecx
        jne    loc_1_0A54
        cmp    edi, 1
        jne    loc_1_098A
        mov    esi, dword ptr [esp + 0x20]
        cmp    dword ptr [esi + 4], 2
        jne    loc_1_08E9
        mov    eax, 0x273a
        jmp    loc_1_0CE7
loc_1_08E9:
        mov    al, byte ptr [esi + 0x1b]
        or     al, al
        jne    loc_1_0943
        cmp    dword ptr [esp + 0x10], 0
        je     loc_1_093B
        mov    dword ptr [esp + 0x10], 1
        mov    edi, dword ptr [esp + 0x2c]
        or     edi, edi
        je     loc_1_0932
        push   1
        lea    eax, [esp + 0x14]
        push   4
        push   eax
        push   1
        push   0x300
        push   0x200
        push   0x400
        push   edi
        call   sub_1_0E56
        add    esp, 0x20
        or     eax, eax
        jne    loc_1_0CE7
loc_1_0932:
        mov    byte ptr [esi + 0x1b], 1
        jmp    loc_1_0CE5
loc_1_093B:
        or     al, al
        je     loc_1_0CE5
loc_1_0943:
        cmp    dword ptr [esp + 0x10], 0
        jne    loc_1_0CE5
        mov    edi, dword ptr [esp + 0x2c]
        or     edi, edi
        je     loc_1_0981
        push   1
        lea    eax, [esp + 0x14]
        push   4
        push   eax
        push   1
        push   0x300
        push   0x200
        push   0x400
        push   edi
        call   sub_1_0E56
        add    esp, 0x20
        or     eax, eax
        jne    loc_1_0CE7
loc_1_0981:
        mov    byte ptr [esi + 0x1b], 0
        jmp    loc_1_0CE5
loc_1_098A:
        cmp    eax, 6
        jne    loc_1_0A54
        cmp    edi, 0x7000
        jne    loc_1_0A54
        mov    esi, dword ptr [esp + 0x20]
        cmp    dword ptr [esi + 4], 2
        jne    loc_1_09B3
        mov    eax, 0x273a
        jmp    loc_1_0CE7
loc_1_09B3:
        mov    al, byte ptr [esi + 0x1c]
        or     al, al
        jne    loc_1_0A0D
        cmp    dword ptr [esp + 0x10], 0
        je     loc_1_0A05
        mov    dword ptr [esp + 0x10], 1
        mov    edi, dword ptr [esp + 0x2c]
        or     edi, edi
        je     loc_1_09FC
        push   1
        lea    eax, [esp + 0x14]
        push   4
        push   eax
        push   4
        push   0x300
        push   0x200
        push   0x400
        push   edi
        call   sub_1_0E56
        add    esp, 0x20
        or     eax, eax
        jne    loc_1_0CE7
loc_1_09FC:
        mov    byte ptr [esi + 0x1c], 1
        jmp    loc_1_0CE5
loc_1_0A05:
        or     al, al
        je     loc_1_0CE5
loc_1_0A0D:
        cmp    dword ptr [esp + 0x10], 0
        jne    loc_1_0CE5
        mov    edi, dword ptr [esp + 0x2c]
        or     edi, edi
        je     loc_1_0A4B
        push   1
        lea    eax, [esp + 0x14]
        push   4
        push   eax
        push   4
        push   0x300
        push   0x200
        push   0x400
        push   edi
        call   sub_1_0E56
        add    esp, 0x20
        or     eax, eax
        jne    loc_1_0CE7
loc_1_0A4B:
        mov    byte ptr [esi + 0x1c], 0
        jmp    loc_1_0CE5
loc_1_0A54:
        or     eax, eax
        jne    loc_1_0BAF
        mov    esi, dword ptr [esp + 0x20]
        cmp    dword ptr [esi + 8], 6
        jne    loc_1_0A70
        mov    eax, 0x273a
        jmp    loc_1_0CE7
loc_1_0A70:
        cmp    edi, 2
        je     loc_1_0A9B
        cmp    edi, 3
        je     loc_1_0AE4
        cmp    edi, 4
        je     loc_1_0B48
        cmp    edi, 5
        jl     loc_1_0A91
        cmp    edi, 6
        jle    loc_1_0B52
loc_1_0A91:
        mov    eax, 0x273a
        jmp    loc_1_0CE7
loc_1_0A9B:
        mov    ebp, dword ptr [esp + 0x28]
        or     ebp, ebp
        je     loc_1_0ADA
        push   1
        lea    eax, [esp + 0x14]
        push   4
        push   eax
        push   3
        push   0x200
        push   0x200
        push   0x401
        push   ebp
        call   sub_1_0E56
        add    esp, 0x20
        or     eax, eax
        jne    loc_1_0CE7
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [esi + 0x14], eax
        jmp    loc_1_0CE5
loc_1_0ADA:
        mov    eax, 0x2726
        jmp    loc_1_0CE7
loc_1_0AE4:
        cmp    dword ptr [esp + 0x10], 0xff
        jg     loc_1_0B3E
        cmp    dword ptr [esp + 0x10], 0
        jl     loc_1_0B3E
        mov    ebp, dword ptr [esp + 0x28]
        or     ebp, ebp
        je     loc_1_0B34
        push   1
        lea    eax, [esp + 0x14]
        push   4
        push   eax
        push   2
        push   0x200
        push   0x200
        push   0x401
        push   ebp
        call   sub_1_0E56
        add    esp, 0x20
        or     eax, eax
        jne    loc_1_0CE7
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [esi + 0x10], eax
        jmp    loc_1_0CE5
loc_1_0B34:
        mov    eax, 0x2726
        jmp    loc_1_0CE7
loc_1_0B3E:
        mov    eax, 0x2726
        jmp    loc_1_0CE7
loc_1_0B48:
        mov    eax, 0x273a
        jmp    loc_1_0CE7
loc_1_0B52:
        cmp    edx, 8
        jae    loc_1_0B61
        mov    eax, 0x2726
        jmp    loc_1_0CE7
loc_1_0B61:
        mov    ebp, dword ptr [esp + 0x28]
        or     ebp, ebp
        je     loc_1_0BA5
        push   1
        push   edx
        push   ebx
        sub    edi, 5
        cmp    edi, 1
        sbb    eax, eax
        add    eax, 7
        push   eax
        push   0x200
        push   0x200
        push   0x401
        push   ebp
        call   sub_1_0E56
        add    esp, 0x20
        or     eax, eax
        jne    loc_1_0CE7
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [esi + 0x14], eax
        jmp    loc_1_0CE5
loc_1_0BA5:
        mov    eax, 0x2726
        jmp    loc_1_0CE7
loc_1_0BAF:
        cmp    edi, 8
        je     loc_1_0BD3
        cmp    edi, 0x10
        je     loc_1_0C81
        cmp    edi, 0x1002
        je     loc_1_0C9C
        mov    eax, 0x273a
        jmp    loc_1_0CE7
loc_1_0BD3:
        mov    esi, dword ptr [esp + 0x20]
        cmp    dword ptr [esi + 4], 2
        jne    loc_1_0BE7
        mov    eax, 0x273a
        jmp    loc_1_0CE7
loc_1_0BE7:
        mov    al, byte ptr [esi + 0x19]
        or     al, al
        jne    loc_1_0C41
        cmp    dword ptr [esp + 0x10], 0
        je     loc_1_0C39
        mov    dword ptr [esp + 0x10], 1
        mov    edi, dword ptr [esp + 0x2c]
        or     edi, edi
        je     loc_1_0C30
        push   1
        lea    eax, [esp + 0x14]
        push   4
        push   eax
        push   2
        push   0x300
        push   0x200
        push   0x400
        push   edi
        call   sub_1_0E56
        add    esp, 0x20
        or     eax, eax
        jne    loc_1_0CE7
loc_1_0C30:
        mov    byte ptr [esi + 0x19], 1
        jmp    loc_1_0CE5
loc_1_0C39:
        or     al, al
        je     loc_1_0CE5
loc_1_0C41:
        cmp    dword ptr [esp + 0x10], 0
        jne    loc_1_0CE5
        mov    edi, dword ptr [esp + 0x2c]
        or     edi, edi
        je     loc_1_0C7B
        push   1
        lea    eax, [esp + 0x14]
        push   4
        push   eax
        push   2
        push   0x300
        push   0x200
        push   0x400
        push   edi
        call   sub_1_0E56
        add    esp, 0x20
        or     eax, eax
        jne    loc_1_0CE7
loc_1_0C7B:
        mov    byte ptr [esi + 0x19], 0
        jmp    loc_1_0CE5
loc_1_0C81:
        cmp    dword ptr [esp + 0x10], 0
        je     loc_1_0C92
        mov    esi, dword ptr [esp + 0x20]
        mov    byte ptr [esi + 0x1a], 1
        jmp    loc_1_0CE5
loc_1_0C92:
        mov    esi, dword ptr [esp + 0x20]
        mov    byte ptr [esi + 0x1a], 0
        jmp    loc_1_0CE5
loc_1_0C9C:
        mov    esi, dword ptr [esp + 0x20]
        mov    eax, dword ptr [esp + 0x10]
        cmp    dword ptr [esi + 0xc], eax
        je     loc_1_0CE5
        cmp    dword ptr [esi + 4], 2
        je     loc_1_0CE5
        mov    edi, dword ptr [esp + 0x2c]
        or     edi, edi
        je     loc_1_0CDE
        push   1
        lea    eax, [esp + 0x14]
        push   4
        push   eax
        push   6
        push   0x300
        push   0x200
        push   0x400
        push   edi
        call   sub_1_0E56
        add    esp, 0x20
        or     eax, eax
        jne    loc_1_0CE7
loc_1_0CDE:
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [esi + 0xc], eax
loc_1_0CE5:
        sub    eax, eax
loc_1_0CE7:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
loc_1_0CEF:
        push   ebx
        push   esi
        push   edi
        sub    edx, edx
        sub    ebx, ebx
        mov    ecx, dword ptr [esp + 0x10]
        cmp    ecx, edx
        je     loc_1_0D22
        cmp    dword ptr [ecx], edx
        je     loc_1_0D29
loc_1_0D02:
        mov    eax, dword ptr [ecx]
        cmp    eax, 6
        jne    loc_1_0D0E
        mov    edx, 1
loc_1_0D0E:
        cmp    eax, 0x11
        jne    loc_1_0D18
        mov    ebx, 1
loc_1_0D18:
        add    ecx, 4
        cmp    dword ptr [ecx], 0
        jne    loc_1_0D02
        jmp    loc_1_0D29
loc_1_0D22:
        mov    edx, 1
        mov    ebx, edx
loc_1_0D29:
        or     edx, edx
        jne    loc_1_0D42
        or     ebx, ebx
        jne    loc_1_0D42
        mov    ecx, dword ptr [esp + 0x1c]
        sub    eax, eax
        mov    dword ptr [ecx], 0
        jmp    loc_1_0E15
loc_1_0D42:
        mov    ecx, dword ptr [esp + 0x1c]
        mov    esi, dword ptr [ecx]
        cmp    esi, 0x4e
        jae    loc_1_0D5D
        mov    dword ptr [ecx], 0x4e
        mov    eax, 0xffffffff
        jmp    loc_1_0E15
loc_1_0D5D:
        or     edx, edx
        mov    eax, dword ptr [esp + 0x18]
        je     loc_1_0DB3
        mov    dword ptr [eax], 0x1066
        mov    dword ptr [eax + 4], 2
        mov    dword ptr [eax + 8], 0x10
        mov    dword ptr [eax + 0xc], 0x10
        lea    edi, [esi + eax - 7]
        mov    dword ptr [eax + 0x10], 1
        mov    dword ptr [eax + 0x14], 6
        mov    dword ptr [eax + 0x18], 0
        mov    dword ptr [eax + 0x1c], edi
        mov    esi, 0x1d4                    ; -> loc_1_01D4
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
        lea    edi, [eax + 0x20]
        mov    eax, dword ptr [eax + 0x1c]
        sub    eax, 7
        mov    dword ptr [edi + 0x1c], eax
        jmp    loc_1_0DBD
loc_1_0DB3:
        mov    edi, eax
        add    esi, eax
        sub    esi, 7
        mov    dword ptr [eax + 0x1c], esi
loc_1_0DBD:
        or     ebx, ebx
        je     loc_1_0DFD
        mov    dword ptr [edi], 0x1609
        mov    dword ptr [edi + 4], 2
        mov    dword ptr [edi + 8], 0x10
        mov    dword ptr [edi + 0xc], 0x10
        mov    esi, 0x1dc                    ; -> loc_1_01DC
        mov    dword ptr [edi + 0x10], 2
        mov    dword ptr [edi + 0x14], 0x11
        mov    dword ptr [edi + 0x18], 0xffbb
        mov    edi, dword ptr [edi + 0x1c]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsw  word ptr es:[edi], word ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
loc_1_0DFD:
        mov    dword ptr [ecx], 0x4e
        or     edx, edx
        je     loc_1_0E10
        or     ebx, ebx
        mov    eax, 2
        jne    loc_1_0E15
loc_1_0E10:
        mov    eax, 1
loc_1_0E15:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_1_0E19:
        push   ebx
        push   esi
        push   edi
        sub    ebx, ebx
        mov    ecx, dword ptr [esp + 0x14]
        cmp    ecx, ebx
        je     loc_1_0E4C
        mov    eax, dword ptr [esp + 0x10]
        mov    edx, dword ptr [esp + 0x18]
        mov    esi, dword ptr [esp + 0x1c]
        mov    edi, dword ptr [esp + 0x20]
loc_1_0E36:
        cmp    dword ptr [eax], edx
        jne    loc_1_0E44
        cmp    dword ptr [eax + 4], esi
        jne    loc_1_0E44
        cmp    dword ptr [eax + 8], edi
        je     loc_1_0E52
loc_1_0E44:
        add    eax, 0xc
        inc    ebx
        cmp    ebx, ecx
        jb     loc_1_0E36
loc_1_0E4C:
        sub    al, al
loc_1_0E4E:
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_0E52:
        mov    al, 1
        jmp    loc_1_0E4E
sub_1_0E56:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x24
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 0x20]
        add    eax, 0x21
        push   eax
        call   sub_1_0090
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        jne    loc_1_0E7E
        mov    eax, 0x2747
        jmp    loc_1_0F7D
loc_1_0E7E:
        cmp    byte ptr [ebp + 0x24], 0
        je     loc_1_0EB9
        mov    dword ptr [ebp - 0x14], 0
        mov    ecx, dword ptr [ebp - 0x14]
        VMMCall  VMM.Create_Semaphore            ; cd2025000100
        cmc    
        sbb    ecx, ecx
        and    eax, ecx
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp - 0x10]
        mov    dword ptr [ebx + 4], eax
        or     eax, eax
        jne    loc_1_0EB9
        push   ebx
        call   sub_1_0096
        add    esp, 4
        mov    eax, 0x2747
        jmp    loc_1_0F7D
loc_1_0EB9:
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [ebx + 8], eax
        mov    dword ptr [ebx + 0xc], 0
        mov    eax, dword ptr [ebp + 0x10]
        lea    edx, [ebx + 0x20]
        mov    dword ptr [ebx + 0x10], eax
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [ebx + 0x14], eax
        mov    eax, dword ptr [ebp + 0x18]
        mov    edi, edx
        mov    dword ptr [ebx + 0x18], eax
        mov    eax, dword ptr [ebp + 0x20]
        mov    esi, dword ptr [ebp + 0x1c]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    eax, dword ptr [ebp + 0x20]
        mov    dword ptr [ebx + 0x1c], eax
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0x24], eax
        lea    eax, [ebx + 8]
        mov    dword ptr [ebp - 0x20], 0xf84 ; -> loc_1_0F84
        mov    dword ptr [ebp - 0x1c], ebx
        mov    dword ptr [ebp - 0x18], 0
        push   dword ptr [ebx + 0x1c]
        push   edx
        push   eax
        lea    eax, [ebp - 0x24]
        push   eax
        mov    eax, dword ptr [0x125]        ; -> loc_1_0125
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 0x4c]
        add    esp, 0x10
        mov    esi, eax
        cmp    dword ptr [ebx + 4], 0
        je     loc_1_0F61
        cmp    esi, 0xff
        jne    loc_1_0F52
        mov    dword ptr [ebp - 8], 1
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 8]
        VMMCall  VMM.Wait_Semaphore              ; cd2027000100
        mov    esi, dword ptr [ebx]
loc_1_0F52:
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        VMMCall  VMM.Destroy_Semaphore           ; cd2026000100
loc_1_0F61:
        cmp    esi, 0xff
        je     loc_1_0F72
        push   ebx
        call   sub_1_0096
        add    esp, 4
loc_1_0F72:
        or     esi, esi
        mov    eax, 0x2747
        jne    loc_1_0F7D
        sub    eax, eax
loc_1_0F7D:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_0F84:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 4]
        or     eax, eax
        je     loc_1_0FA7
        mov    edx, dword ptr [ebp + 0xc]
        mov    dword ptr [ecx], edx
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        VMMCall  VMM.Signal_Semaphore            ; cd2028000100
        jmp    loc_1_0FB0
loc_1_0FA7:
        push   ecx
        call   sub_1_0096
        add    esp, 4
loc_1_0FB0:
        mov    esp, ebp
        pop    ebp
        ret    
        db  000h, 000h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x98  R-X
        push   ebp
        mov    ebp, esp
        push   ebx
        push   ecx
        mov    eax, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 0x10]
        VxDCall  Dev_0495.Service_0001h          ; cd2001009504
        mov    dword ptr [0x125], eax        ; -> loc_1_0125
        pop    ecx
        pop    ebx
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 0x10]
        VxDCall  Dev_0495.Service_0002h          ; cd2002009504
        mov    dword ptr [0x125], 0x121      ; -> loc_1_0125
        leave  
        ret    
loc_2_003B:
        cmp    byte ptr [0x120], 0           ; -> obj1:0x0120
        jne    loc_2_0068
        mov    byte ptr [0x120], 1           ; -> obj1:0x0120
loc_2_004B:
        mov    edi, 0xfb4                    ; -> obj1:0x0FB4
        mov    ecx, 0xfb4                    ; -> obj1:0x0FB4
        sub    ecx, edi
        shr    ecx, 2
        xor    eax, eax
        cld    
        rep stosd dword ptr es:[edi], eax
        cmp    byte ptr [0x120], 0           ; -> obj1:0x0120
        jne    loc_2_006A
loc_2_0068:
        clc    
        ret    
loc_2_006A:
        call   sub_2_0080
        cmp    eax, 1
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_2_0080:
        push   0xac                          ; -> loc_1_00AC
        push   0x144                         ; -> loc_1_0144
        push   0x13c                         ; -> loc_1_013C
        call   0
        add    esp, 0xc
        ret    
VxD__LTEXT ENDS

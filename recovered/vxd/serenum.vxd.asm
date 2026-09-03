; SERENUM -- reconstructed 32-bit disassembly (VxD / LE format)
; 4 objects, 2016 instructions recovered by recursive descent

Declare_Virtual_Device SERENUM, 4, 0, SERENUM_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 080000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x7c  R-X
SERENUM_Control:
        cmp    eax, 0x1b
        je     loc_2_0000
        cmp    eax, 0x1c
        jne    loc_1_0017
        call   sub_4_1572
        cmp    eax, 1
        ret    
loc_1_0017:
        cmp    eax, 0x22
        jne    loc_1_0025
        push   edx
        push   ebx
        call   sub_4_1578
        stc    
        ret    
loc_1_0025:
        clc    
        ret    
        db  000h
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  4, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'SERENUM '                  ; DDB_Name
        dd  080000000h                  ; DDB_Init_Order
        dd  SERENUM_Control          ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
        dd  loc_4_1590               ; fixup
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x20  R-X
loc_2_0000:
        sub    esp, 0x40
        mov    edi, esp
        push   0
        push   edi
        VMMCall  VMM._GetDemandPageInfo          ; cd206f000100
        add    esp, 8
        cmp    dword ptr [edi + 0xc], 0x2ee
        lea    esp, [esp + 0x40]
        ret    
        db  000h, 000h, 000h
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 3  vsize=0x1a4  RW-
        db  053h, 045h, 052h, 045h, 04Eh, 055h, 04Dh, 05Ch, 050h, 04Eh, 050h, 030h, 033h, 030h, 038h, 05Ch
        db  000h, 000h, 000h, 000h, 041h, 06Ch, 074h, 065h, 072h, 06Eh, 061h, 074h, 069h, 076h, 065h, 020h
        db  049h, 06Eh, 070h, 075h, 074h, 020h, 044h, 065h, 076h, 069h, 063h, 065h, 000h, 000h, 000h, 000h
        db  041h, 06Ch, 074h, 065h, 072h, 06Eh, 061h, 074h, 069h, 076h, 065h, 020h, 049h, 06Eh, 070h, 075h
        db  074h, 020h, 044h, 065h, 076h, 069h, 063h, 065h, 000h, 000h, 000h, 000h, 041h, 06Ch, 074h, 065h
        db  072h, 06Eh, 061h, 074h, 069h, 076h, 065h, 020h, 049h, 06Eh, 070h, 075h, 074h, 020h, 044h, 065h
        db  076h, 069h, 063h, 065h, 000h, 000h, 000h, 000h, 044h, 065h, 076h, 069h, 063h, 065h, 044h, 065h
        db  073h, 063h, 000h, 000h, 047h, 049h, 044h, 045h, 049h, 000h, 000h, 000h, 047h, 049h, 044h, 045h
        db  049h, 000h, 000h, 000h, 047h, 049h, 044h, 045h, 049h, 000h, 000h, 000h, 043h, 06Ch, 061h, 073h
        db  073h, 000h, 000h, 000h, 050h, 04Eh, 050h, 030h, 033h, 030h, 038h, 000h, 050h, 04Eh, 050h, 030h
        db  033h, 030h, 038h, 000h, 050h, 04Eh, 050h, 030h, 033h, 030h, 038h, 000h, 043h, 06Fh, 06Dh, 070h
        db  061h, 074h, 069h, 062h, 06Ch, 065h, 049h, 044h, 073h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 045h, 06Eh, 075h, 06Dh
        db  05Ch, 053h, 045h, 052h, 045h, 04Eh, 055h, 04Dh, 000h, 000h, 000h, 000h, 05Ch, 000h, 000h, 000h
        db  043h, 06Ch, 061h, 073h, 073h, 000h, 000h, 000h, 05Ch, 000h, 000h, 000h, 043h, 053h, 043h, 06Fh
        db  06Eh, 066h, 069h, 067h, 046h, 06Ch, 061h, 067h, 073h, 000h, 000h, 000h, 053h, 045h, 052h, 045h
        db  04Eh, 055h, 04Dh, 05Ch, 000h, 000h, 000h, 000h, 04Dh, 06Fh, 064h, 065h, 06Dh, 000h, 000h, 000h
        db  050h, 06Fh, 072h, 074h, 04Eh, 061h, 06Dh, 065h, 000h, 000h, 000h, 000h, 053h, 045h, 052h, 045h
        db  04Eh, 055h, 04Dh, 05Ch, 000h, 000h, 000h, 000h, 05Ch, 000h, 000h, 000h, 044h, 065h, 076h, 069h
        db  063h, 065h, 044h, 065h, 073h, 063h, 000h, 000h, 043h, 06Ch, 061h, 073h, 073h, 000h, 000h, 000h
        db  043h, 06Fh, 06Dh, 070h, 061h, 074h, 069h, 062h, 06Ch, 065h, 049h, 044h, 073h, 000h, 000h, 000h
        db  053h, 065h, 072h, 069h, 061h, 06Ch, 020h, 04Eh, 075h, 06Dh, 062h, 065h, 072h, 000h, 000h, 000h
        db  050h, 06Eh, 050h, 020h, 052h, 065h, 076h, 000h, 04Fh, 074h, 068h, 065h, 072h, 049h, 044h, 000h
        db  000h, 000h, 000h, 000h
VxD__LDATA ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0x17da  R-X
        db  083h, 0ECh, 004h, 089h, 054h, 024h, 000h, 06Ah, 000h, 06Ah, 001h, 0FFh, 074h, 024h, 008h, 0FFh
        db  074h, 024h, 00Ch, 0E8h, 064h, 017h, 000h, 000h, 083h, 0C4h, 010h, 06Ah, 000h, 0FFh, 074h, 024h
        db  004h, 0E8h, 086h, 017h, 000h, 000h, 083h, 0C4h, 00Ch, 0C3h
sub_4_002A:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0x18]
        mov    eax, esi
        dec    esi
        or     eax, eax
        je     loc_4_0057
        mov    eax, dword ptr [esp + 0xc]
        mov    dl, byte ptr [esp + 0x10]
        mov    bl, byte ptr [esp + 0x14]
loc_4_0043:
        mov    cl, byte ptr [eax]
        or     cl, cl
        je     loc_4_0057
        cmp    cl, dl
        jne    loc_4_004F
        mov    byte ptr [eax], bl
loc_4_004F:
        inc    eax
        mov    ecx, esi
        dec    esi
        or     ecx, ecx
        jne    loc_4_0043
loc_4_0057:
        pop    esi
        pop    ebx
        ret    0x10
sub_4_005C:
        sub    esp, 0x168
        push   esi
        push   edi
        mov    esi, 0                        ; -> obj3:0x0000
        lea    edi, [esp + 0x70]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        sub    eax, eax
        movsb  byte ptr es:[edi], byte ptr [esi]
        lea    edi, [esp + 0x81]
        mov    ecx, 0x3b
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        stosb  byte ptr es:[edi], al
        push   eax
        lea    eax, [esp + 0x10]
        push   0x64
        mov    esi, dword ptr [esp + 0x17c]
        push   eax
        push   esi
        call   sub_4_176C
        add    esp, 0x10
        or     eax, eax
        je     loc_4_00A8
        sub    eax, eax
        jmp    loc_4_01CA
loc_4_00A8:
        push   0x64
        push   0x26
        lea    eax, [esp + 0x14]
        push   0x5c
        push   eax
        call   sub_4_002A
        lea    eax, [esp + 0xc]
        push   eax
        lea    eax, [esp + 0x74]
        push   eax
        call   sub_4_1597
        push   0
        push   esi
        call   sub_4_17B4
        add    esp, 8
        push   0
        lea    eax, [esp + 0x74]
        push   esi
        push   eax
        lea    eax, [esp + 0x14]
        push   eax
        call   sub_4_1784
        add    esp, 0x10
        cmp    eax, 2
        jne    loc_4_00F3
        sub    eax, eax
        jmp    loc_4_01CA
loc_4_00F3:
        cmp    eax, 0x10
        jne    loc_4_00FF
        sub    eax, eax
        jmp    loc_4_01CA
loc_4_00FF:
        or     eax, eax
        je     loc_4_010A
        sub    eax, eax
        jmp    loc_4_01CA
loc_4_010A:
        push   0
        push   esi
        call   sub_4_17BC
        add    esp, 8
        push   0x14                          ; -> obj3:0x0014
        call   sub_4_1764
        add    esp, 4
        or     eax, eax
        je     loc_4_0150
        push   0
        push   0x30                          ; -> obj3:0x0030
        call   sub_4_1764
        add    esp, 4
        push   eax
        push   0x4c                          ; -> obj3:0x004C
        push   1
        push   0x68                          ; -> obj3:0x0068
        push   0
        push   dword ptr [esp + 0x20]
        call   sub_4_17CC
        add    esp, 0x1c
loc_4_0150:
        push   0x74                          ; -> obj3:0x0074
        call   sub_4_1764
        add    esp, 4
        or     eax, eax
        je     loc_4_018B
        push   0
        push   0x7c                          ; -> obj3:0x007C
        call   sub_4_1764
        add    esp, 4
        push   eax
        push   0x84                          ; -> obj3:0x0084
        push   1
        push   0x8c                          ; -> obj3:0x008C
        push   0
        push   dword ptr [esp + 0x20]
        call   sub_4_17CC
        add    esp, 0x1c
loc_4_018B:
        push   0x94                          ; -> obj3:0x0094
        call   sub_4_1764
        add    esp, 4
        or     eax, eax
        je     loc_4_01C6
        push   0
        push   0x9c                          ; -> obj3:0x009C
        call   sub_4_1764
        add    esp, 4
        push   eax
        push   0xa4                          ; -> obj3:0x00A4
        push   1
        push   0xac                          ; -> obj3:0x00AC
        push   0
        push   dword ptr [esp + 0x20]
        call   sub_4_17CC
        add    esp, 0x1c
loc_4_01C6:
        mov    eax, dword ptr [esp + 8]
loc_4_01CA:
        pop    edi
        pop    esi
        add    esp, 0x168
        ret    4
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_4_01E0:
        push   ebx
        push   esi
        mov    eax, dword ptr [esp + 0xc]
        mov    esi, dword ptr [esp + 0x10]
loc_4_01EA:
        mov    cl, byte ptr [eax]
        cmp    cl, 0x61
        jl     loc_4_01FE
        cmp    cl, 0x7a
        jg     loc_4_01FE
        movsx  edx, cl
        sub    edx, 0x20
        jmp    loc_4_0201
loc_4_01FE:
        movsx  edx, cl
loc_4_0201:
        mov    bl, byte ptr [esi]
        cmp    bl, 0x61
        jl     loc_4_0215
        cmp    bl, 0x7a
        jg     loc_4_0215
        movsx  ecx, bl
        sub    ecx, 0x20
        jmp    loc_4_0218
loc_4_0215:
        movsx  ecx, bl
loc_4_0218:
        cmp    edx, ecx
        jne    loc_4_0228
        or     bl, bl
        je     loc_4_0224
        inc    eax
        inc    esi
        jmp    loc_4_01EA
loc_4_0224:
        sub    eax, eax
        jmp    loc_4_0230
loc_4_0228:
        movsx  eax, byte ptr [eax]
        movsx  ecx, byte ptr [esi]
        sub    eax, ecx
loc_4_0230:
        pop    esi
        pop    ebx
        ret    8
sub_4_0235:
        sub    esp, 0x220
        push   ebx
        push   esi
        push   edi
        push   ebp
        lea    eax, [esp + 0x20]
        push   eax
        push   0xfc                          ; -> obj3:0x00FC
        push   0x80000002
        call   sub_4_1628
        add    esp, 0xc
        or     eax, eax
        jne    loc_4_0490
        sub    ebx, ebx
        mov    esi, dword ptr [esp + 0x234]
        mov    edi, dword ptr [esp + 0x238]
loc_4_026E:
        mov    dword ptr [esp + 0x14], 0x101
        mov    eax, ebx
        lea    ecx, [esp + 0x12c]
        push   0x101
        push   ecx
        push   eax
        inc    ebx
        push   dword ptr [esp + 0x2c]
        call   sub_4_1638
        add    esp, 0x10
        or     eax, eax
        jne    loc_4_0484
        push   0
        lea    eax, [esp + 0x2c]
        push   0x100
        push   eax
        push   dword ptr [esi]
        call   sub_4_176C
        add    esp, 0x10
        or     eax, eax
        jne    loc_4_0484
        push   0x100
        push   0x26
        lea    eax, [esp + 0x30]
        push   0x5c
        push   eax
        call   sub_4_002A
        lea    eax, [esp + 0x12c]
        push   0x10c                         ; -> obj3:0x010C
        push   eax
        call   sub_4_1597
        lea    eax, [esp + 0x28]
        push   eax
        lea    eax, [esp + 0x130]
        push   eax
        call   sub_4_1597
        lea    eax, [esp + 0x18]
        push   eax
        lea    eax, [esp + 0x130]
        push   eax
        push   dword ptr [esp + 0x28]
        call   sub_4_1628
        add    esp, 0xc
        or     eax, eax
        jne    loc_4_026E
        mov    dword ptr [esp + 0x14], 0x101
        lea    eax, [esp + 0x14]
        push   eax
        lea    eax, [esp + 0x2c]
        push   eax
        push   0
        push   0
        push   0x110                         ; -> obj3:0x0110
        push   dword ptr [esp + 0x2c]
        call   sub_4_1640
        add    esp, 0x18
        or     eax, eax
        jne    loc_4_0467
        lea    eax, [esp + 0x28]
        push   edi
        push   eax
        call   sub_4_01E0
        or     eax, eax
        jne    loc_4_0467
        mov    ebp, 1
        mov    byte ptr [esp + 0x28], 0
        lea    eax, [esp + 0x28]
        push   0xfc                          ; -> obj3:0x00FC
        push   eax
        call   sub_4_1597
        lea    eax, [esp + 0x28]
        push   0x118                         ; -> obj3:0x0118
        push   eax
        call   sub_4_1597
        lea    eax, [esp + 0x12c]
        push   eax
        lea    eax, [esp + 0x2c]
        push   eax
        call   sub_4_1597
        lea    eax, [esp + 0x1c]
        push   eax
        lea    eax, [esp + 0x2c]
        push   eax
        push   0x80000005
        call   sub_4_1628
        add    esp, 0xc
        or     eax, eax
        jne    loc_4_03E8
        mov    dword ptr [esp + 0x14], 4
        lea    eax, [esp + 0x14]
        push   eax
        lea    eax, [esp + 0x28]
        push   eax
        push   0
        push   0
        push   0x11c                         ; -> obj3:0x011C
        push   dword ptr [esp + 0x30]
        call   sub_4_1640
        add    esp, 0x18
        or     eax, eax
        jne    loc_4_03DC
        test   byte ptr [esp + 0x24], 2
        je     loc_4_03DC
        sub    ebp, ebp
loc_4_03DC:
        push   dword ptr [esp + 0x1c]
        call   sub_4_1630
        add    esp, 4
loc_4_03E8:
        or     ebp, ebp
        je     loc_4_0467
        mov    byte ptr [esp + 0x28], 0
        lea    eax, [esp + 0x28]
        push   0x12c                         ; -> obj3:0x012C
        push   eax
        call   sub_4_1597
        lea    eax, [esp + 0x12c]
        push   eax
        lea    eax, [esp + 0x2c]
        push   eax
        call   sub_4_1597
        push   0
        lea    eax, [esp + 0x2c]
        push   dword ptr [esi]
        push   eax
        lea    eax, [esp + 0x1c]
        push   eax
        call   sub_4_1784
        add    esp, 0x10
        or     eax, eax
        jne    loc_4_0467
        mov    ebp, dword ptr [esi + 4]
        push   ebp
        call   sub_4_1728
        or     eax, eax
        je     loc_4_0450
loc_4_0439:
        mov    ecx, dword ptr [esp + 0x10]
        cmp    dword ptr [eax], ecx
        je     loc_4_044C
        push   eax
        push   ebp
        call   sub_4_1738
        or     eax, eax
        jne    loc_4_0439
loc_4_044C:
        or     eax, eax
        jne    loc_4_0467
loc_4_0450:
        push   ebp
        call   sub_4_16E0
        or     eax, eax
        je     loc_4_0478
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [eax], ecx
        push   eax
        push   ebp
        call   sub_4_16F4
loc_4_0467:
        push   dword ptr [esp + 0x18]
        call   sub_4_1630
        add    esp, 4
        jmp    loc_4_026E
loc_4_0478:
        push   dword ptr [esp + 0x18]
        call   sub_4_1630
        add    esp, 4
loc_4_0484:
        push   dword ptr [esp + 0x20]
        call   sub_4_1630
        add    esp, 4
loc_4_0490:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x220
        ret    8
loc_4_049D:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    dword ptr [ebp - 4], edx
        mov    eax, dword ptr [ebp - 4]
        push   dword ptr [eax + 0x48]
        call   sub_4_15E4
        or     eax, eax
        je     loc_4_04E0
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x44]
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [ebp - 8]
        VxDCall  VPICD.VPICD_Get_Virtualization_Count ; cd2016000300
        cmp    eax, 1
        jbe    loc_4_04E0
        mov    eax, dword ptr [ebp - 4]
        mov    eax, dword ptr [eax + 0x44]
        mov    dword ptr [eax*4 + 0xbc], 0   ; -> obj3:0x00BC
        jmp    loc_4_0538
loc_4_04E0:
        mov    eax, dword ptr [ebp - 4]
        mov    esi, dword ptr [eax + 0xc]
        add    eax, 0xc
        or     esi, esi
        je     loc_4_051A
        mov    dword ptr [eax], 0
        push   0
        push   0
        push   6
        push   esi
        call   sub_4_1678
        add    esp, 0x10
        push   0
        push   0
        push   4
        push   esi
        call   sub_4_1678
        add    esp, 0x10
        push   esi
        call   sub_4_1660
        add    esp, 4
loc_4_051A:
        mov    eax, dword ptr [ebp - 4]
        push   0x138                         ; -> obj3:0x0138
        mov    eax, dword ptr [eax + 0x44]
        mov    dword ptr [eax*4 + 0xbc], 0   ; -> obj3:0x00BC
        push   dword ptr [ebp - 4]
        call   sub_4_0235
loc_4_0538:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_4_053D:
        sub    esp, 4
        mov    dword ptr [esp], edx
        mov    eax, dword ptr [esp]
        mov    dword ptr [eax + 0x2c], 0
        push   dword ptr [esp]
        call   sub_4_160A
        add    esp, 4
        ret    
sub_4_055C:
        push   esi
        mov    esi, dword ptr [esp + 8]
        add    esi, 0x2c
        push   dword ptr [esi]
        call   sub_4_16D0
        mov    dword ptr [esi], 0
        mov    edx, dword ptr [esp + 8]
        call   sub_4_053D
        pop    esi
        ret    4
loc_4_057E:
        sub    esp, 4
        mov    dword ptr [esp], edx
        push   0
        mov    eax, dword ptr [esp + 4]
        push   dword ptr [eax]
        call   sub_4_17AC
        add    esp, 0xc
        ret    
loc_4_0596:
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        push   dword ptr [esi + 0x1c]
        call   sub_4_16D0
        mov    dword ptr [esi + 0x1c], 0
        push   esi
        call   sub_4_148C
        pop    esi
        ret    
loc_4_05B2:
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        push   dword ptr [esi + 0x24]
        call   sub_4_16D0
        mov    dword ptr [esi + 0x24], 0
        push   esi
        call   sub_4_148C
        pop    esi
        ret    
loc_4_05CE:
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    eax, dword ptr [esp + 0x14]
        cmp    eax, 4
        je     loc_4_05EA
        cmp    eax, 5
        je     loc_4_0651
        mov    eax, 1
        jmp    loc_4_066D
loc_4_05EA:
        mov    eax, dword ptr [0x1a0]        ; -> obj3:0x01A0
        or     eax, eax
        jne    loc_4_05F5
        jmp    loc_4_066B
loc_4_05F5:
        push   eax
        call   sub_4_1728
        mov    ebx, eax
        or     ebx, ebx
        je     loc_4_066B
        mov    esi, dword ptr [esp + 0x20]
loc_4_0605:
        mov    edi, dword ptr [ebx + 4]
        push   edi
        call   sub_4_1728
        mov    ebp, eax
        or     ebp, ebp
        je     loc_4_063D
loc_4_0614:
        cmp    dword ptr [ebp], esi
        je     loc_4_0626
        push   ebp
        push   edi
        call   sub_4_1738
        mov    ebp, eax
        or     ebp, ebp
        jne    loc_4_0614
loc_4_0626:
        or     ebp, ebp
        je     loc_4_063D
        cmp    dword ptr [ebp], esi
        jne    loc_4_063D
        push   ebp
        push   edi
        call   sub_4_1748
        push   ebp
        push   edi
        call   sub_4_1718
loc_4_063D:
        push   ebx
        push   dword ptr [0x1a0]             ; -> obj3:0x01A0
        call   sub_4_1738
        mov    ebx, eax
        or     ebx, ebx
        jne    loc_4_0605
        jmp    loc_4_066B
loc_4_0651:
        call   sub_4_175C
        cmp    eax, 0x20000000
        mov    eax, 1
        jb     loc_4_066D
        mov    ecx, dword ptr [esp + 0x1c]
        call   sub_4_0672
loc_4_066B:
        sub    eax, eax
loc_4_066D:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_4_0672:
        sub    esp, 0x10c
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 0x10], ecx
        mov    dword ptr [esp + 0x14], 0
        mov    dword ptr [esp + 0x20], 0xc8
        lea    eax, [esp + 0x20]
        push   0
        push   eax
        lea    eax, [esp + 0x5c]
        push   eax
        push   1
        push   0x140                         ; -> obj3:0x0140
        push   0
        push   dword ptr [esp + 0x28]
        call   sub_4_17C4
        add    esp, 0x1c
        or     eax, eax
        je     loc_4_06BC
        sub    eax, eax
        jmp    loc_4_08DC
loc_4_06BC:
        push   dword ptr [esp + 0x10]
        call   sub_4_14C0
        mov    esi, eax
        or     esi, esi
        jne    loc_4_06D2
        sub    eax, eax
        jmp    loc_4_08DC
loc_4_06D2:
        movsx  eax, byte ptr [esp + 0x57]
        sub    eax, 0x30
        push   2
        mov    dword ptr [esi + 0x48], eax
        lea    eax, [esp + 0x20]
        push   dword ptr [esp + 0x14]
        push   eax
        call   sub_4_179C
        add    esp, 0xc
        or     eax, eax
        je     loc_4_06FB
        sub    eax, eax
        jmp    loc_4_08DC
loc_4_06FB:
        push   0
        push   0
        push   4
        lea    eax, [esp + 0x24]
        push   dword ptr [esp + 0x28]
        push   eax
        call   sub_4_17A4
        add    esp, 0x14
        or     eax, eax
        je     loc_4_071D
        sub    eax, eax
        jmp    loc_4_08DC
loc_4_071D:
        mov    eax, dword ptr [esp + 0x18]
        mov    bx, word ptr [eax + 2]
        and    ebx, 0xf
        mov    dword ptr [esi + 0x44], ebx
        cmp    dword ptr [ebx*4 + 0xbc], 0   ; -> obj3:0x00BC
        je     loc_4_074C
        push   esi
        push   0x2710
        push   0x57e                         ; -> loc_4_057E
        call   sub_4_16B4
        sub    eax, eax
        jmp    loc_4_08DC
loc_4_074C:
        push   dword ptr [esi + 4]
        call   sub_4_1728
        mov    edi, eax
        or     edi, edi
        je     loc_4_077B
loc_4_075A:
        push   edi
        push   dword ptr [esi + 4]
        call   sub_4_1738
        mov    ebp, eax
        push   0
        push   dword ptr [edi]
        call   sub_4_178C
        add    esp, 8
        or     eax, eax
        jne    loc_4_07A3
        mov    edi, ebp
        or     ebp, ebp
        jne    loc_4_075A
loc_4_077B:
        mov    edi, dword ptr [esi + 0x40]
        test   edi, 1
        jne    loc_4_07AA
        or     edi, 1
        mov    dword ptr [esi + 0x40], edi
        push   ebx
        call   sub_4_15BB
        or     eax, eax
        je     loc_4_07C9
        or     edi, 2
        mov    dword ptr [esi + 0x40], edi
        sub    eax, eax
        jmp    loc_4_08DC
loc_4_07A3:
        sub    eax, eax
        jmp    loc_4_08DC
loc_4_07AA:
        test   edi, 2
        je     loc_4_07C9
        push   ebx
        call   sub_4_15BB
        or     eax, eax
        je     loc_4_07C3
        sub    eax, eax
        jmp    loc_4_08DC
loc_4_07C3:
        and    edi, 0xfffffffd
        mov    dword ptr [esi + 0x40], edi
loc_4_07C9:
        call   sub_4_16A8
        push   eax
        lea    eax, [esp + 0x58]
        push   eax
        call   sub_4_1648
        add    esp, 8
        mov    edi, eax
        cmp    edi, -0xa
        jb     loc_4_07EA
        sub    eax, eax
        jmp    loc_4_08DC
loc_4_07EA:
        push   0
        push   0
        push   0
        push   0x100
        push   0
        push   edi
        call   sub_4_1658
        add    esp, 0x18
        mov    ecx, dword ptr [esi + 0x44]
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [ecx*4 + 0xbc], eax ; -> obj3:0x00BC
        push   0
        mov    dword ptr [esi + 0xc], edi
        push   dword ptr [esp + 0x14]
        lea    eax, [esp + 0x1c]
        push   dword ptr [esp + 0x18]
        push   eax
        call   sub_4_1774
        add    esp, 0x10
        cmp    dword ptr [esp + 0x14], 0
        je     loc_4_083A
        push   esi
        call   sub_4_005C
        jmp    loc_4_08D7
loc_4_083A:
        mov    dword ptr [esp + 0x24], 0x30
        mov    dword ptr [esp + 0x28], 0x4b0
        mov    dword ptr [esp + 0x2c], 1
        mov    byte ptr [esp + 0x3a], 7
        mov    byte ptr [esp + 0x3b], 0
        mov    byte ptr [esp + 0x3c], 0
        lea    eax, [esp + 0x24]
        push   -1
        push   eax
        push   dword ptr [esi + 0xc]
        call   sub_4_1650
        add    esp, 0xc
        or     eax, eax
        jne    loc_4_0888
        mov    dword ptr [esi + 0x2c], 0
        push   esi
        call   sub_4_055C
        sub    eax, eax
        jmp    loc_4_08DC
loc_4_0888:
        push   0
        push   0
        push   5
        push   edi
        call   sub_4_1678
        add    esp, 0x10
        push   0
        push   0
        push   4
        push   edi
        call   sub_4_1678
        add    esp, 0x10
        push   1
        push   edi
        call   sub_4_1680
        add    esp, 8
        push   esi
        push   0xc8
        push   0x8e7                         ; -> loc_4_08E7
        call   sub_4_16B4
        mov    dword ptr [esi + 0x10], eax
        push   esi
        push   0x2710
        push   0x53d                         ; -> sub_4_053D
        call   sub_4_16B4
        mov    dword ptr [esi + 0x2c], eax
loc_4_08D7:
        mov    eax, 1
loc_4_08DC:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10c
        ret    
loc_4_08E7:
        sub    esp, 8
        push   esi
        mov    dword ptr [esp + 4], edx
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax + 0x10], 0
        mov    esi, dword ptr [esp + 4]
        add    esi, 0xc
        mov    eax, dword ptr [esi]
        or     eax, eax
        je     loc_4_0961
        lea    ecx, [esp + 8]
        push   ecx
        push   eax
        call   sub_4_1670
        add    esp, 8
        test   byte ptr [esp + 8], 0x20
        jne    loc_4_0927
        push   dword ptr [esp + 4]
        call   sub_4_055C
        jmp    loc_4_0961
loc_4_0927:
        mov    esi, dword ptr [esi]
        push   0
        push   0
        push   4
        push   esi
        call   sub_4_1678
        add    esp, 0x10
        push   0
        push   0
        push   6
        push   esi
        call   sub_4_1678
        add    esp, 0x10
        push   dword ptr [esp + 4]
        push   0xc8
        push   0x966                         ; -> loc_4_0966
        call   sub_4_16B4
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx + 0x14], eax
loc_4_0961:
        pop    esi
        add    esp, 8
        ret    
loc_4_0966:
        sub    esp, 4
        mov    dword ptr [esp], edx
        mov    eax, dword ptr [esp]
        mov    dword ptr [eax + 0x14], 0
        mov    eax, dword ptr [esp]
        mov    eax, dword ptr [eax + 0xc]
        or     eax, eax
        je     loc_4_09AC
        push   0
        push   0
        push   5
        push   eax
        call   sub_4_1678
        add    esp, 0x10
        push   dword ptr [esp]
        push   0xc8
        push   0x9b0                         ; -> loc_4_09B0
        call   sub_4_16B4
        mov    ecx, dword ptr [esp]
        mov    dword ptr [ecx + 0x18], eax
loc_4_09AC:
        add    esp, 4
        ret    
loc_4_09B0:
        sub    esp, 4
        mov    dword ptr [esp], edx
        mov    eax, dword ptr [esp]
        mov    dword ptr [eax + 0x18], 0
        mov    eax, dword ptr [esp]
        mov    eax, dword ptr [eax + 0xc]
        or     eax, eax
        je     loc_4_0A10
        push   0
        push   0
        push   3
        push   eax
        call   sub_4_1678
        add    esp, 0x10
        push   dword ptr [esp]
        push   0xc8
        push   0xa14                         ; -> loc_4_0A14
        call   sub_4_16B4
        mov    ecx, dword ptr [esp]
        mov    dword ptr [ecx + 0x1c], eax
        push   dword ptr [esp]
        push   0x596                         ; -> loc_4_0596
        mov    eax, dword ptr [esp + 8]
        push   1
        push   dword ptr [eax + 0xc]
        call   sub_4_1690
        add    esp, 0x10
loc_4_0A10:
        add    esp, 4
        ret    
loc_4_0A14:
        sub    esp, 4
        push   esi
        mov    dword ptr [esp + 4], edx
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax + 0x1c], 0
        mov    eax, dword ptr [esp + 4]
        mov    eax, dword ptr [eax + 0xc]
        or     eax, eax
        je     loc_4_0A80
        push   0
        push   0
        push   -1
        push   eax
        call   sub_4_1690
        mov    eax, dword ptr [esp + 0x14]
        add    esp, 0x10
        mov    esi, dword ptr [eax + 0xc]
        push   0
        push   0
        push   6
        push   esi
        call   sub_4_1678
        add    esp, 0x10
        push   0
        push   0
        push   4
        push   esi
        call   sub_4_1678
        add    esp, 0x10
        push   dword ptr [esp + 4]
        push   0xc8
        push   0xa85                         ; -> loc_4_0A85
        call   sub_4_16B4
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx + 0x20], eax
loc_4_0A80:
        pop    esi
        add    esp, 4
        ret    
loc_4_0A85:
        sub    esp, 4
        push   esi
        mov    dword ptr [esp + 4], edx
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [eax + 0x20], 0
        mov    eax, dword ptr [esp + 4]
        mov    esi, dword ptr [eax + 0xc]
        or     esi, esi
        je     loc_4_0AF5
        push   0
        push   0
        push   3
        push   esi
        call   sub_4_1678
        add    esp, 0x10
        push   0
        push   0
        push   5
        push   esi
        call   sub_4_1678
        add    esp, 0x10
        push   dword ptr [esp + 4]
        push   0xc8
        push   0xafa                         ; -> loc_4_0AFA
        call   sub_4_16B4
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx + 0x24], eax
        push   dword ptr [esp + 4]
        push   0x5b2                         ; -> loc_4_05B2
        mov    eax, dword ptr [esp + 0xc]
        push   1
        push   dword ptr [eax + 0xc]
        call   sub_4_1690
        add    esp, 0x10
loc_4_0AF5:
        pop    esi
        add    esp, 4
        ret    
loc_4_0AFA:
        sub    esp, 4
        mov    dword ptr [esp], edx
        mov    eax, dword ptr [esp]
        mov    dword ptr [eax + 0x24], 0
        mov    eax, dword ptr [esp]
        mov    eax, dword ptr [eax + 0xc]
        or     eax, eax
        je     loc_4_0B2F
        push   0
        push   0
        push   -1
        push   eax
        call   sub_4_1690
        add    esp, 0x10
        push   dword ptr [esp]
        call   sub_4_055C
loc_4_0B2F:
        add    esp, 4
        ret    
sub_4_0B33:
        push   1
        push   0x5ce                         ; -> loc_4_05CE
        push   ecx
        call   sub_4_1794
        add    esp, 0xc
        ret    
sub_4_0B44:
        push   ebx
        push   esi
        push   edi
        mov    ecx, 0x64
        mov    esi, dword ptr [esp + 0x10]
        mov    eax, esi
        cdq    
        idiv   ecx
        mov    ebx, eax
        cmp    ebx, 0xa
        jl     loc_4_0B71
        mov    ecx, 0xa
        mov    eax, ebx
        cdq    
        idiv   ecx
        mov    edi, dword ptr [esp + 0x14]
        add    al, 0x30
        mov    byte ptr [edi], al
        inc    edi
        jmp    loc_4_0B75
loc_4_0B71:
        mov    edi, dword ptr [esp + 0x14]
loc_4_0B75:
        mov    ecx, 0xa
        mov    eax, ebx
        cdq    
        idiv   ecx
        add    dl, 0x30
        mov    byte ptr [edi], dl
        inc    edi
        mov    ecx, 0x64
        mov    byte ptr [edi], 0x2e
        inc    edi
        mov    eax, esi
        cdq    
        idiv   ecx
        mov    esi, edx
        mov    ecx, 0xa
        mov    eax, esi
        cdq    
        idiv   ecx
        add    al, 0x30
        mov    byte ptr [edi], al
        inc    edi
        mov    eax, esi
        cdq    
        idiv   ecx
        add    dl, 0x30
        mov    byte ptr [edi], dl
        mov    byte ptr [edi + 1], 0
        pop    edi
        pop    esi
        pop    ebx
        ret    8
sub_4_0BB8:
        push   ebx
        push   esi
        mov    ecx, dword ptr [esp + 0xc]
        mov    eax, 1
        mov    esi, dword ptr [ecx + 0x3c]
        mov    edx, dword ptr [esp + 0x10]
        mov    bl, byte ptr [esi]
loc_4_0BCC:
        cmp    bl, 0x28
        je     loc_4_0BE2
        cmp    bl, 8
        je     loc_4_0BE2
        mov    byte ptr [edx], bl
        inc    edx
        mov    bl, byte ptr [eax + esi]
        inc    eax
        cmp    eax, 0x11
        jl     loc_4_0BCC
loc_4_0BE2:
        dec    eax
        mov    dword ptr [ecx + 0x38], eax
        mov    byte ptr [edx], 0
        pop    esi
        pop    ebx
        ret    8
sub_4_0BEE:
        mov    al, byte ptr [esp + 4]
        cmp    al, 0x30
        jl     loc_4_0C02
        cmp    al, 0x39
        jg     loc_4_0C02
        movsx  eax, al
        sub    eax, 0x30
        jmp    loc_4_0C27
loc_4_0C02:
        cmp    al, 0x41
        jl     loc_4_0C12
        cmp    al, 0x46
        jg     loc_4_0C12
        movsx  eax, al
        sub    eax, 0x37
        jmp    loc_4_0C27
loc_4_0C12:
        cmp    al, 0x61
        jl     loc_4_0C22
        cmp    al, 0x66
        jg     loc_4_0C22
        movsx  eax, al
        sub    eax, 0x57
        jmp    loc_4_0C27
loc_4_0C22:
        mov    eax, 0xffffffff
loc_4_0C27:
        ret    4
sub_4_0C2A:
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        push   ebp
        cmp    dword ptr [esp + 0x2c], 0
        je     loc_4_0DAD
        mov    esi, dword ptr [esp + 0x28]
        or     esi, esi
        je     loc_4_0DAD
        mov    eax, dword ptr [esp + 0x2c]
        mov    byte ptr [eax], 0
        mov    edi, dword ptr [esi + 0x38]
        mov    ebp, dword ptr [esi + 0x3c]
        mov    bl, byte ptr [edi + ebp]
        or     bl, bl
        jne    loc_4_0C61
        jmp    loc_4_0DAD
loc_4_0C61:
        inc    edi
        cmp    edi, 0x100
        jge    loc_4_0C81
loc_4_0C6A:
        cmp    bl, 0x28
        je     loc_4_0C90
        cmp    bl, 8
        je     loc_4_0C81
        mov    bl, byte ptr [ebp + edi]
        inc    edi
        cmp    edi, 0x100
        jl     loc_4_0C6A
loc_4_0C81:
        cmp    bl, 0x28
        je     loc_4_0C90
        cmp    bl, 8
        je     loc_4_0C90
        jmp    loc_4_0DAD
loc_4_0C90:
        mov    al, 0x28
        sub    al, bl
        mov    byte ptr [esp + 0x13], al
        movsx  ecx, al
        movsx  eax, byte ptr [ebp + edi + 9]
        add    eax, ecx
        cmp    eax, 0x29
        je     loc_4_0D45
        movsx  eax, bl
        mov    dword ptr [esp + 0x1c], eax
        mov    dword ptr [esp + 0x14], edi
        cmp    edi, 0x100
        jge    loc_4_0CE5
loc_4_0CBE:
        mov    eax, 0x29
        movsx  edx, bl
        sub    eax, edx
        cmp    eax, ecx
        je     loc_4_0CE5
        mov    eax, dword ptr [esp + 0x14]
        mov    bl, byte ptr [ebp + eax]
        inc    dword ptr [esp + 0x14]
        movsx  eax, bl
        cmp    dword ptr [esp + 0x14], 0x100
        jl     loc_4_0CBE
loc_4_0CE5:
        mov    edx, dword ptr [esp + 0x14]
        add    edx, ebp
        movsx  eax, byte ptr [edx - 3]
        movsx  edx, byte ptr [edx - 2]
        mov    dword ptr [esp + 0x20], edx
        add    eax, ecx
        add    dword ptr [esp + 0x20], ecx
        push   eax
        call   sub_4_0BEE
        or     eax, eax
        jge    loc_4_0D0C
        jmp    loc_4_0DAD
loc_4_0D0C:
        push   dword ptr [esp + 0x20]
        call   sub_4_0BEE
        or     eax, eax
        jge    loc_4_0D1E
        jmp    loc_4_0DAD
loc_4_0D1E:
        mov    al, byte ptr [esp + 0x14]
        movsx  ecx, bl
        sub    ecx, dword ptr [esp + 0x1c]
        cmp    ecx, 1
        je     loc_4_0D30
        jmp    loc_4_0DAD
loc_4_0D30:
        movsx  ecx, al
        mov    al, 0x29
        sub    al, byte ptr [esp + 0x13]
        mov    byte ptr [ebp + ecx - 3], al
        mov    eax, dword ptr [esi + 0x3c]
        mov    byte ptr [ecx + eax - 2], 0
loc_4_0D45:
        mov    edx, dword ptr [esi + 0x3c]
        lea    eax, [edi + edx]
        mov    cl, byte ptr [eax]
        cmp    cl, 0x3f
        jg     loc_4_0DAD
        cmp    byte ptr [eax + 1], 0x3f
        jg     loc_4_0DAD
        and    ecx, 0x3f
        shl    ecx, 6
        inc    edi
        mov    al, byte ptr [edx + edi]
        and    eax, 0x3f
        or     ecx, eax
        inc    edi
        push   dword ptr [esp + 0x2c]
        push   ecx
        call   sub_4_0B44
        mov    edx, edi
        cmp    edi, 0x100
        jge    loc_4_0DA6
loc_4_0D7C:
        cmp    byte ptr [esp + 0x13], 0
        je     loc_4_0DA6
        mov    eax, dword ptr [esi + 0x3c]
        mov    cl, byte ptr [esp + 0x13]
        add    byte ptr [eax + edx], cl
        mov    eax, dword ptr [esi + 0x3c]
        inc    edx
        mov    al, byte ptr [eax + edx - 1]
        cmp    al, 0x29
        jne    loc_4_0D9E
        mov    byte ptr [esp + 0x13], 0
loc_4_0D9E:
        cmp    edx, 0x100
        jl     loc_4_0D7C
loc_4_0DA6:
        mov    dword ptr [esi + 0x38], edi
        sub    eax, eax
        jmp    loc_4_0DB2
loc_4_0DAD:
        mov    eax, 1
loc_4_0DB2:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x14
        ret    8
sub_4_0DBC:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    edi, dword ptr [esp + 0x1c]
        or     edi, edi
        je     loc_4_0E62
        mov    esi, dword ptr [esp + 0x18]
        or     esi, esi
        je     loc_4_0E62
        mov    ebp, dword ptr [esi + 0x38]
        mov    ebx, dword ptr [esi + 0x3c]
        mov    al, byte ptr [ebp + ebx]
        mov    byte ptr [edi], al
        inc    ebp
        inc    edi
        mov    al, byte ptr [ebx + ebp]
        mov    byte ptr [edi], al
        inc    ebp
        inc    edi
        mov    al, byte ptr [ebx + ebp]
        mov    byte ptr [edi], al
        inc    ebp
        inc    edi
        mov    al, byte ptr [ebx + ebp]
        mov    byte ptr [esp + 0x13], al
        inc    ebp
        push   dword ptr [esp + 0x13]
        call   sub_4_0BEE
        or     eax, eax
        jl     loc_4_0E13
        mov    al, byte ptr [esp + 0x13]
        mov    byte ptr [edi], al
        inc    edi
loc_4_0E13:
        mov    al, byte ptr [ebx + ebp]
        mov    byte ptr [esp + 0x13], al
        inc    ebp
        push   dword ptr [esp + 0x13]
        call   sub_4_0BEE
        or     eax, eax
        jl     loc_4_0E2F
        mov    al, byte ptr [esp + 0x13]
        mov    byte ptr [edi], al
        inc    edi
loc_4_0E2F:
        mov    al, byte ptr [ebx + ebp]
        mov    byte ptr [esp + 0x13], al
        inc    ebp
        push   dword ptr [esp + 0x13]
        call   sub_4_0BEE
        or     eax, eax
        jl     loc_4_0E4B
        mov    al, byte ptr [esp + 0x13]
        mov    byte ptr [edi], al
        inc    edi
loc_4_0E4B:
        mov    bl, byte ptr [ebx + ebp]
        inc    ebp
        push   ebx
        call   sub_4_0BEE
        or     eax, eax
        jl     loc_4_0E5C
        mov    byte ptr [edi], bl
        inc    edi
loc_4_0E5C:
        mov    byte ptr [edi], 0
        mov    dword ptr [esi + 0x38], ebp
loc_4_0E62:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    8
sub_4_0E6C:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, dword ptr [esp + 0x1c]
        or     esi, esi
        je     loc_4_0EDA
        cmp    dword ptr [esp + 0x18], 0
        je     loc_4_0EDA
        mov    eax, dword ptr [esp + 0x18]
        mov    byte ptr [esi], 0
        mov    edi, dword ptr [eax + 0x38]
        mov    ecx, edi
        inc    edi
        mov    eax, dword ptr [eax + 0x3c]
        mov    dword ptr [esp + 0x10], eax
        cmp    byte ptr [ecx + eax], 0x5c
        jne    loc_4_0EDA
        mov    bl, byte ptr [eax + edi]
        inc    edi
        sub    ebp, ebp
loc_4_0EA2:
        cmp    edi, 0x100
        jge    loc_4_0ECF
        cmp    bl, 0x5c
        je     loc_4_0ECF
        cmp    bl, 0x29
        je     loc_4_0ECF
        inc    ebp
        push   ebx
        call   sub_4_0BEE
        or     eax, eax
        jl     loc_4_0ECF
        mov    byte ptr [esi], bl
        inc    esi
        mov    eax, dword ptr [esp + 0x10]
        mov    bl, byte ptr [edi + eax]
        inc    edi
        cmp    ebp, 8
        jl     loc_4_0EA2
loc_4_0ECF:
        mov    byte ptr [esi], 0
        mov    eax, dword ptr [esp + 0x18]
        dec    edi
        mov    dword ptr [eax + 0x38], edi
loc_4_0EDA:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    8
sub_4_0EE4:
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [esp + 0x14]
        or     edx, edx
        je     loc_4_0F35
        mov    ecx, dword ptr [esp + 0x10]
        or     ecx, ecx
        je     loc_4_0F35
        mov    byte ptr [edx], 0
        mov    edi, dword ptr [ecx + 0x38]
        mov    esi, edi
        inc    edi
        mov    eax, dword ptr [ecx + 0x3c]
        cmp    byte ptr [esi + eax], 0x5c
        jne    loc_4_0F35
        mov    bl, byte ptr [eax + edi]
        inc    edi
        cmp    edi, 0x100
        jge    loc_4_0F2E
loc_4_0F15:
        cmp    bl, 0x5c
        je     loc_4_0F2E
        cmp    bl, 0x29
        je     loc_4_0F2E
        mov    byte ptr [edx], bl
        inc    edx
        mov    bl, byte ptr [eax + edi]
        inc    edi
        cmp    edi, 0x100
        jl     loc_4_0F15
loc_4_0F2E:
        mov    byte ptr [edx], 0
        dec    edi
        mov    dword ptr [ecx + 0x38], edi
loc_4_0F35:
        pop    edi
        pop    esi
        pop    ebx
        ret    8
sub_4_0F3B:
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [esp + 0x14]
        or     eax, eax
        je     loc_4_0F8C
        mov    edi, dword ptr [esp + 0x10]
        or     edi, edi
        je     loc_4_0F8C
        mov    byte ptr [eax], 0
        mov    ecx, dword ptr [edi + 0x38]
        mov    esi, ecx
        inc    ecx
        mov    edx, dword ptr [edi + 0x3c]
        cmp    byte ptr [esi + edx], 0x5c
        jne    loc_4_0F8C
        mov    bl, byte ptr [ecx + edx]
        inc    ecx
        cmp    ecx, 0x100
        jge    loc_4_0F85
loc_4_0F6C:
        cmp    bl, 0x5c
        je     loc_4_0F85
        cmp    bl, 0x29
        je     loc_4_0F85
        mov    byte ptr [eax], bl
        inc    eax
        mov    bl, byte ptr [ecx + edx]
        inc    ecx
        cmp    ecx, 0x100
        jl     loc_4_0F6C
loc_4_0F85:
        mov    byte ptr [eax], 0
        dec    ecx
        mov    dword ptr [edi + 0x38], ecx
loc_4_0F8C:
        pop    edi
        pop    esi
        pop    ebx
        ret    8
sub_4_0F92:
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [esp + 0x14]
        or     ecx, ecx
        je     loc_4_0FE3
        mov    eax, dword ptr [esp + 0x10]
        or     eax, eax
        je     loc_4_0FE3
        mov    byte ptr [ecx], 0
        mov    edi, dword ptr [eax + 0x38]
        mov    esi, edi
        inc    edi
        mov    edx, dword ptr [eax + 0x3c]
        cmp    byte ptr [esi + edx], 0x5c
        jne    loc_4_0FE3
        mov    bl, byte ptr [edx + edi]
        inc    edi
        cmp    edi, 0x100
        jge    loc_4_0FDC
loc_4_0FC3:
        cmp    bl, 0x5c
        je     loc_4_0FDC
        cmp    bl, 0x29
        je     loc_4_0FDC
        mov    byte ptr [ecx], bl
        inc    ecx
        mov    bl, byte ptr [edx + edi]
        inc    edi
        cmp    edi, 0x100
        jl     loc_4_0FC3
loc_4_0FDC:
        mov    byte ptr [ecx], 0
        dec    edi
        mov    dword ptr [eax + 0x38], edi
loc_4_0FE3:
        pop    edi
        pop    esi
        pop    ebx
        ret    8
loc_4_0FE9:
        sub    esp, 0x908
        push   ebx
        push   esi
        push   edi
        mov    esi, 0x14c                    ; -> obj3:0x014C
        lea    edi, [esp + 0x110]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
        lea    edi, [esp + 0x119]
        sub    eax, eax
        mov    ecx, 0x3d
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        stosb  byte ptr es:[edi], al
        mov    esi, dword ptr [esp + 0x918]
        cmp    dword ptr [esi + 0x3c], -1
        je     loc_4_13C6
        mov    ebx, dword ptr [esi]
        lea    eax, [esp + 0x710]
        push   eax
        push   esi
        call   sub_4_0BB8
        mov    edi, eax
        cmp    edi, 0x10
        jg     loc_4_13B2
        lea    eax, [esp + 0x410]
        push   eax
        push   esi
        call   sub_4_0C2A
        or     eax, eax
        jne    loc_4_13B2
        lea    eax, [esp + 0x610]
        push   eax
        push   esi
        call   sub_4_0DBC
        lea    eax, [esp + 0x610]
        push   eax
        call   sub_4_1764
        add    esp, 4
        cmp    eax, 7
        jne    loc_4_13B2
        lea    eax, [esp + 0x610]
        push   eax
        lea    eax, [esp + 0x114]
        push   eax
        call   sub_4_1597
        lea    eax, [esp + 0x10]
        push   eax
        push   esi
        call   sub_4_0E6C
        lea    eax, [esp + 0x10]
        push   eax
        call   sub_4_1764
        add    esp, 4
        or     eax, eax
        je     loc_4_10C4
        lea    eax, [esp + 0x10]
        push   eax
        call   sub_4_1764
        add    esp, 4
        cmp    eax, 8
        jne    loc_4_13B2
loc_4_10C4:
        lea    eax, [esp + 0x210]
        push   eax
        push   esi
        call   sub_4_0EE4
        lea    eax, [esp + 0x210]
        push   eax
        call   sub_4_1764
        add    esp, 4
        cmp    eax, 0x20
        ja     loc_4_13B2
        mov    byte ptr [esp + 0x810], 0x2a
        lea    eax, [esp + 0x811]
        push   eax
        push   esi
        call   sub_4_0F3B
        lea    eax, [esp + 0x811]
        push   eax
        call   sub_4_1764
        add    esp, 4
        cmp    eax, 0x28
        ja     loc_4_13B2
        lea    eax, [esp + 0x310]
        push   eax
        push   esi
        call   sub_4_0F92
        lea    eax, [esp + 0x310]
        push   eax
        call   sub_4_1764
        add    esp, 4
        cmp    eax, 0x28
        ja     loc_4_13B2
        lea    eax, [esp + 0x110]
        push   0x158                         ; -> obj3:0x0158
        push   eax
        call   sub_4_1597
        push   0
        lea    eax, [esp + 0x514]
        push   0x64
        push   eax
        push   ebx
        call   sub_4_176C
        add    esp, 0x10
        or     eax, eax
        jne    loc_4_13B2
        push   0x64
        push   0x26
        lea    eax, [esp + 0x518]
        push   0x5c
        push   eax
        call   sub_4_002A
        lea    eax, [esp + 0x510]
        push   eax
        lea    eax, [esp + 0x114]
        push   eax
        call   sub_4_1597
        push   0
        push   ebx
        call   sub_4_17B4
        add    esp, 8
        push   0
        lea    eax, [esp + 0x114]
        push   ebx
        push   eax
        lea    eax, [esp + 0x18]
        push   eax
        call   sub_4_1784
        add    esp, 0x10
        cmp    eax, 2
        je     loc_4_13B2
        cmp    eax, 0x10
        je     loc_4_13B2
        or     eax, eax
        jne    loc_4_13B2
        push   0
        push   ebx
        call   sub_4_17BC
        add    esp, 8
        mov    ebx, dword ptr [esi + 4]
        push   ebx
        call   sub_4_1728
        or     eax, eax
        je     loc_4_1204
loc_4_11ED:
        mov    ecx, dword ptr [esp + 0xc]
        cmp    dword ptr [eax], ecx
        je     loc_4_1200
        push   eax
        push   ebx
        call   sub_4_1738
        or     eax, eax
        jne    loc_4_11ED
loc_4_1200:
        or     eax, eax
        jne    loc_4_121F
loc_4_1204:
        push   ebx
        call   sub_4_16E0
        or     eax, eax
        je     loc_4_13B2
        mov    ecx, dword ptr [esp + 0xc]
        mov    dword ptr [eax], ecx
        push   eax
        push   ebx
        call   sub_4_16F4
loc_4_121F:
        lea    eax, [esp + 0x310]
        push   eax
        call   sub_4_1764
        add    esp, 4
        or     eax, eax
        je     loc_4_1263
        lea    eax, [esp + 0x310]
        push   0
        push   eax
        call   sub_4_1764
        add    esp, 4
        push   eax
        lea    eax, [esp + 0x318]
        push   eax
        push   1
        push   0x15c                         ; -> obj3:0x015C
        push   0
        push   dword ptr [esp + 0x24]
        call   sub_4_17CC
        add    esp, 0x1c
loc_4_1263:
        lea    eax, [esp + 0x210]
        push   eax
        call   sub_4_1764
        add    esp, 4
        or     eax, eax
        je     loc_4_12A7
        lea    eax, [esp + 0x210]
        push   0
        push   eax
        call   sub_4_1764
        add    esp, 4
        push   eax
        lea    eax, [esp + 0x218]
        push   eax
        push   1
        push   0x168                         ; -> obj3:0x0168
        push   0
        push   dword ptr [esp + 0x24]
        call   sub_4_17CC
        add    esp, 0x1c
loc_4_12A7:
        lea    eax, [esp + 0x811]
        push   eax
        call   sub_4_1764
        add    esp, 4
        or     eax, eax
        je     loc_4_130F
        cmp    byte ptr [esp + 0x811], 0x2a
        jne    loc_4_12CE
        lea    eax, [esp + 0x811]
        jmp    loc_4_12D5
loc_4_12CE:
        lea    eax, [esp + 0x810]
loc_4_12D5:
        push   eax
        call   sub_4_1764
        add    esp, 4
        cmp    byte ptr [esp + 0x811], 0x2a
        lea    ecx, [esp + 0x811]
        je     loc_4_12F6
        lea    ecx, [esp + 0x810]
loc_4_12F6:
        push   0
        push   eax
        push   ecx
        push   1
        push   0x170                         ; -> obj3:0x0170
        push   0
        push   dword ptr [esp + 0x24]
        call   sub_4_17CC
        add    esp, 0x1c
loc_4_130F:
        lea    eax, [esp + 0x10]
        push   eax
        call   sub_4_1764
        add    esp, 4
        or     eax, eax
        je     loc_4_134A
        lea    eax, [esp + 0x10]
        push   0
        push   eax
        call   sub_4_1764
        add    esp, 4
        push   eax
        lea    eax, [esp + 0x18]
        push   eax
        push   1
        push   0x180                         ; -> obj3:0x0180
        push   0
        push   dword ptr [esp + 0x24]
        call   sub_4_17CC
        add    esp, 0x1c
loc_4_134A:
        lea    eax, [esp + 0x410]
        push   eax
        call   sub_4_1764
        add    esp, 4
        or     eax, eax
        je     loc_4_138E
        lea    eax, [esp + 0x410]
        push   0
        push   eax
        call   sub_4_1764
        add    esp, 4
        push   eax
        lea    eax, [esp + 0x418]
        push   eax
        push   1
        push   0x190                         ; -> obj3:0x0190
        push   0
        push   dword ptr [esp + 0x24]
        call   sub_4_17CC
        add    esp, 0x1c
loc_4_138E:
        or     edi, edi
        je     loc_4_13B2
        push   0
        lea    eax, [esp + 0x714]
        push   edi
        push   eax
        push   3
        push   0x198                         ; -> obj3:0x0198
        push   0
        push   dword ptr [esp + 0x24]
        call   sub_4_17CC
        add    esp, 0x1c
loc_4_13B2:
        push   0
        push   dword ptr [esi + 0x3c]
        call   sub_4_16A0
        add    esp, 8
        mov    dword ptr [esi + 0x3c], 0xffffffff
loc_4_13C6:
        push   esi
        call   sub_4_055C
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x908
        ret    
loc_4_13D6:
        sub    esp, 8
        mov    dword ptr [esp], edx
        mov    eax, dword ptr [esp]
        cmp    dword ptr [eax + 0xc], 0
        je     loc_4_1488
        push   0
        push   0x100
        call   sub_4_1698
        mov    ecx, dword ptr [esp + 8]
        add    esp, 8
        mov    dword ptr [ecx + 0x3c], eax
        mov    eax, dword ptr [esp]
        cmp    dword ptr [eax + 0x3c], 0
        je     loc_4_147F
        lea    eax, [esp + 4]
        push   eax
        mov    eax, dword ptr [esp + 4]
        push   0
        push   dword ptr [eax + 0xc]
        call   sub_4_1668
        mov    eax, dword ptr [esp + 0xc]
        add    esp, 0xc
        add    eax, 0x34
        push   eax
        mov    eax, dword ptr [esp + 4]
        push   0x100
        push   dword ptr [eax + 0x3c]
        push   dword ptr [eax + 0xc]
        call   sub_4_1688
        add    esp, 0x10
        or     eax, eax
        je     loc_4_1463
        mov    eax, dword ptr [esp]
        cmp    dword ptr [eax + 0x34], 0
        je     loc_4_1463
        push   0
        push   dword ptr [esp + 4]
        push   0xfe9                         ; -> loc_4_0FE9
        call   sub_4_17D4
        add    esp, 0xc
        jmp    loc_4_1488
loc_4_1463:
        mov    eax, dword ptr [esp]
        push   0
        push   dword ptr [eax + 0x3c]
        call   sub_4_16A0
        mov    eax, dword ptr [esp + 8]
        add    esp, 8
        mov    dword ptr [eax + 0x3c], 0xffffffff
loc_4_147F:
        push   dword ptr [esp]
        call   sub_4_055C
loc_4_1488:
        add    esp, 8
        ret    
sub_4_148C:
        push   esi
        mov    esi, dword ptr [esp + 8]
        push   dword ptr [esi + 0x2c]
        call   sub_4_16D0
        mov    dword ptr [esi + 0x2c], 0
        push   esi
        push   0x8fc
        push   0x13d6                        ; -> loc_4_13D6
        call   sub_4_16B4
        mov    dword ptr [esi + 0x30], eax
        pop    esi
        ret    4
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_4_14C0:
        push   esi
        push   edi
        mov    eax, dword ptr [0x1a0]        ; -> obj3:0x01A0
        or     eax, eax
        jne    loc_4_14E4
        push   0x4c
        push   4
        call   sub_4_1704
        mov    dword ptr [0x1a0], eax        ; -> obj3:0x01A0
        or     eax, eax
        jne    loc_4_14E4
        sub    eax, eax
        jmp    loc_4_156D
loc_4_14E4:
        mov    dword ptr [0x1a0], eax        ; -> obj3:0x01A0
        push   eax
        call   sub_4_1728
        mov    edi, eax
        or     edi, edi
        mov    esi, dword ptr [esp + 0xc]
        je     loc_4_151D
loc_4_14F9:
        cmp    dword ptr [edi], esi
        je     loc_4_1511
        push   edi
        push   dword ptr [0x1a0]             ; -> obj3:0x01A0
        call   sub_4_1738
        mov    edi, eax
        or     edi, edi
        jne    loc_4_14F9
        jmp    loc_4_151D
loc_4_1511:
        cmp    dword ptr [edi + 0x3c], -1
        jne    loc_4_1519
        jmp    loc_4_156B
loc_4_1519:
        sub    eax, eax
        jmp    loc_4_156D
loc_4_151D:
        or     edi, edi
        jne    loc_4_156B
        push   dword ptr [0x1a0]             ; -> obj3:0x01A0
        call   sub_4_16E0
        mov    edi, eax
        or     edi, edi
        jne    loc_4_1536
        sub    eax, eax
        jmp    loc_4_156D
loc_4_1536:
        mov    dword ptr [edi + 0x3c], 0xffffffff
        mov    dword ptr [edi], esi
        mov    dword ptr [edi + 0x40], 0
        push   4
        push   4
        call   sub_4_1704
        mov    dword ptr [edi + 4], eax
        or     eax, eax
        push   edi
        push   dword ptr [0x1a0]             ; -> obj3:0x01A0
        jne    loc_4_1566
        call   sub_4_1718
        sub    eax, eax
        jmp    loc_4_156D
loc_4_1566:
        call   sub_4_16F4
loc_4_156B:
        mov    eax, edi
loc_4_156D:
        pop    edi
        pop    esi
        ret    4
sub_4_1572:
        mov    eax, 1
        ret    
sub_4_1578:
        cmp    dword ptr [esp + 8], 0
        mov    eax, 1
        jne    loc_4_158D
        mov    ecx, dword ptr [esp + 4]
        call   sub_4_0B33
loc_4_158D:
        ret    8
loc_4_1590:
        mov    eax, 0x400
        clc    
        ret    
sub_4_1597:
        push   dword ptr [esp + 4]
        VMMCall  VMM._lstrlen                    ; cd207c010100
        add    esp, 4
        push   -1
        push   dword ptr [esp + 0xc]
        push   edx
        VMMCall  VMM._lstrcpyn                   ; cd207b010100
        add    esp, 0xc
        mov    eax, dword ptr [esp + 4]
        ret    8
sub_4_15BB:
        mov    eax, dword ptr [esp + 4]
        mov    edx, eax
        VxDCall  VPICD.VPICD_Get_IRQ_Complete_Status ; cd200b000300
        jb     loc_4_15E1
        and    ecx, 0x14
        xor    ecx, 0x14
        mov    eax, ecx
        jecxz  loc_4_15E1
        movzx  ecx, byte ptr [0x5fc]
        bt     ecx, edx
        jb     loc_4_15E1
        xor    eax, eax
loc_4_15E1:
        ret    4
sub_4_15E4:
        VxDCall  VMD.VMD_Get_Version             ; cd2000000c00
        cmp    eax, 0x400
        jb     loc_4_1605
        VxDCall  VMD.Service_0007h               ; cd2007000c00
        cmp    ah, 2
        jne    loc_4_1605
        movzx  edx, dl
        cmp    edx, dword ptr [esp + 4]
        je     loc_4_1607
loc_4_1605:
        xor    eax, eax
loc_4_1607:
        ret    4
sub_4_160A:
        push   esi
        xor    eax, eax
        xor    ebx, ebx
        mov    ecx, 0x4b
        mov    esi, 0x49d                    ; -> loc_4_049D
        mov    edx, dword ptr [esp + 8]
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
        pop    esi
        ret    4
        db  000h
sub_4_1628:
        VMMCall  VMM._RegOpenKey                 ; cd2048810100
        add    byte ptr [eax], al
sub_4_1630:
        VMMCall  VMM._RegCloseKey                ; cd2049810100
        add    byte ptr [eax], al
sub_4_1638:
        VMMCall  VMM._RegEnumKey                 ; cd204c810100
        add    byte ptr [eax], al
sub_4_1640:
        VMMCall  VMM._RegQueryValueEx            ; cd2051810100
        add    byte ptr [eax], al
sub_4_1648:
        VxDCall  BIOS.Service_8004h              ; cd2004802b00
        add    byte ptr [eax], al
sub_4_1650:
        VxDCall  BIOS.Service_8005h              ; cd2005802b00
        add    byte ptr [eax], al
sub_4_1658:
        VxDCall  BIOS.Service_8007h              ; cd2007802b00
        add    byte ptr [eax], al
sub_4_1660:
        VxDCall  BIOS.Service_8009h              ; cd2009802b00
        add    byte ptr [eax], al
sub_4_1668:
        VxDCall  BIOS.Service_800Bh              ; cd200b802b00
        add    byte ptr [eax], al
sub_4_1670:
        VxDCall  BIOS.Service_800Ch              ; cd200c802b00
        add    byte ptr [eax], al
sub_4_1678:
        VxDCall  BIOS.Service_800Eh              ; cd200e802b00
        add    byte ptr [eax], al
sub_4_1680:
        VxDCall  BIOS.Service_800Fh              ; cd200f802b00
        add    byte ptr [eax], al
sub_4_1688:
        VxDCall  BIOS.Service_8013h              ; cd2013802b00
        add    byte ptr [eax], al
sub_4_1690:
        VxDCall  BIOS.Service_8017h              ; cd2017802b00
        add    byte ptr [eax], al
sub_4_1698:
        VMMCall  VMM._HeapAllocate               ; cd204f800100
        add    byte ptr [eax], al
sub_4_16A0:
        VMMCall  VMM._HeapFree                   ; cd2051800100
        add    byte ptr [eax], al
sub_4_16A8:
        push   ebx
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    eax, ebx
        pop    ebx
        ret    
        db  000h
sub_4_16B4:
        push   esi
        mov    edx, dword ptr [esp + 0x10]
        mov    eax, dword ptr [esp + 0xc]
        mov    esi, dword ptr [esp + 8]
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    eax, esi
        pop    esi
        ret    0xc
        db  000h, 000h, 000h
sub_4_16D0:
        push   esi
        mov    esi, dword ptr [esp + 8]
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
        pop    esi
        ret    4
        db  000h
sub_4_16E0:
        pop    edx
        mov    ecx, esi
        pop    esi
        VMMCall  VMM.List_Allocate               ; cd209d000100
        mov    esi, ecx
        cmc    
        sbb    ecx, ecx
        and    eax, ecx
        jmp    edx
        db  000h
sub_4_16F4:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        VMMCall  VMM.List_Attach                 ; cd209e000100
        mov    esi, ecx
        jmp    edx
        db  000h
sub_4_1704:
        pop    edx
        pop    eax
        pop    ecx
        push   esi
        VMMCall  VMM.List_Create                 ; cd209b000100
        mov    eax, esi
        pop    esi
        cmc    
        sbb    ecx, ecx
        and    eax, ecx
        jmp    edx
sub_4_1718:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        VMMCall  VMM.List_Deallocate             ; cd20a2000100
        mov    esi, ecx
        jmp    edx
        db  000h
sub_4_1728:
        pop    edx
        mov    ecx, esi
        pop    esi
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        mov    esi, ecx
        jmp    edx
        db  000h, 000h
sub_4_1738:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        mov    esi, ecx
        jmp    edx
        db  000h
sub_4_1748:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        VMMCall  VMM.List_Remove                 ; cd20a1000100
        mov    esi, ecx
        sbb    eax, eax
        inc    eax
        jmp    edx
        db  000h, 000h
sub_4_175C:
        VMMCall  VMM.VMM_GetSystemInitState      ; cd2011810100
        add    byte ptr [eax], al
sub_4_1764:
        VMMCall  VMM._lstrlen                    ; cd207c810100
        add    byte ptr [eax], al
sub_4_176C:
        VxDCall  CONFIGMG._CONFIGMG_Get_Device_ID ; cd2007803300
        add    byte ptr [eax], al
sub_4_1774:
        VxDCall  CONFIGMG._CONFIGMG_Get_Private_DWord ; cd2009803300
        add    byte ptr [eax], al
        VxDCall  CONFIGMG._CONFIGMG_Set_Private_DWord ; cd200a803300
        add    byte ptr [eax], al
sub_4_1784:
        VxDCall  CONFIGMG._CONFIGMG_Create_DevNode ; cd200b803300
        add    byte ptr [eax], al
sub_4_178C:
        VxDCall  CONFIGMG._CONFIGMG_Remove_SubTree ; cd200d803300
        add    byte ptr [eax], al
sub_4_1794:
        VxDCall  CONFIGMG._CONFIGMG_Register_Enumerator ; cd200f803300
        add    byte ptr [eax], al
sub_4_179C:
        VxDCall  CONFIGMG._CONFIGMG_Get_First_Log_Conf ; cd201a803300
        add    byte ptr [eax], al
sub_4_17A4:
        VxDCall  CONFIGMG._CONFIGMG_Get_Next_Res_Des ; cd201f803300
        add    byte ptr [eax], al
sub_4_17AC:
        VxDCall  CONFIGMG._CONFIGMG_Reenumerate_DevNode ; cd2033803300
        add    byte ptr [eax], al
sub_4_17B4:
        VxDCall  CONFIGMG._CONFIGMG_Reset_Children_Marks ; cd2035803300
        add    byte ptr [eax], al
sub_4_17BC:
        VxDCall  CONFIGMG._CONFIGMG_Remove_Unmarked_Children ; cd2037803300
        add    byte ptr [eax], al
sub_4_17C4:
        VxDCall  CONFIGMG._CONFIGMG_Read_Registry_Value ; cd203e803300
        add    byte ptr [eax], al
sub_4_17CC:
        VxDCall  CONFIGMG._CONFIGMG_Write_Registry_Value ; cd203f803300
        add    byte ptr [eax], al
sub_4_17D4:
        VxDCall  Dev_2533.Service_804Eh          ; cd204e803325
VxD__LTEXT ENDS

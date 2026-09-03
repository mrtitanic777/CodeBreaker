; BIOS -- reconstructed 32-bit disassembly (VxD / LE format)
; 4 objects, 5028 instructions recovered by recursive descent

Declare_Virtual_Device BIOS, 4, 0, BIOS_Control, 0003Dh, Undefined_Init_Order
;   DDK version 0400h   init_order 016800001h   flags 00000h
;   service table: obj1:0x0, 4 services

VxD__LTEXT SEGMENT   ; object 1  vsize=0x108  R-X
        dd  BIOS_Service_00          ; fixup
        dd  BIOS_Service_01          ; fixup
        dd  BIOS_Service_02          ; fixup
        dd  BIOS_Service_03          ; fixup
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  0003Dh                      ; DDB_Req_Device_Number
        db  4, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'BIOS    '                  ; DDB_Name
        dd  016800001h                  ; DDB_Init_Order
        dd  BIOS_Control             ; DDB_Control_Proc
        dd  BIOS_PM_API              ; DDB_V86_API_Proc
        dd  BIOS_PM_API              ; DDB_PM_API_Proc
        dd  obj1:0x0000              ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
BIOS_Control:
        cmp    eax, 0x22
        jne    loc_1_0076
        push   edx
        push   ebx
        call   sub_2_28CA
loc_1_0074:
        stc    
        ret    
loc_1_0076:
        cmp    eax, 1
        jne    loc_1_0089
        push   0                             ; -> obj1:0x0010
        call   sub_4_0575
loc_1_0085:
        cmp    eax, 1
        ret    
loc_1_0089:
        cmp    eax, 0x23
        jne    loc_1_0098
        push   esi
        call   sub_2_2965
loc_1_0094:
        cmp    eax, 1
        ret    
loc_1_0098:
        clc    
        ret    
loc_1_009A:
        push   ebx
        push   esi
        push   edi
        pushal 
        mov    dx, word ptr [0]              ; -> obj1:0x0060
        in     al, dx
        test   al, 1
        mov    bl, al
        je     loc_1_00CD
        push   0
        and    bl, 0xfe
        push   0
        push   0                             ; -> loc_2_2548
        call   sub_1_0100
        mov    dx, word ptr [0]              ; -> obj1:0x0060
        add    esp, 0xc
        mov    al, bl
        out    dx, al
        stc    
        jmp    loc_1_00CE
loc_1_00CD:
        clc    
loc_1_00CE:
        popal  
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_00D3:
        mov    eax, dword ptr [0]            ; -> obj1:0x0064
        push   0
        test   byte ptr [eax], 1
        je     loc_1_00F0
        push   0
        push   0                             ; -> loc_2_25DF
        call   sub_1_0100
        add    esp, 0xc
        jmp    loc_1_00FF
loc_1_00F0:
        push   0xfa
        push   0                             ; -> loc_1_00D3
        call   sub_2_33D4
loc_1_00FF:
        ret    
sub_1_0100:
        VxDCall  CONFIGMG._CONFIGMG_Call_At_Appy_Time ; cd204e803300
        int3   
        int3   
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x346a  R-X
BIOS_PM_API:
        push   ebp
        call   sub_2_26FC
        pop    ecx
        ret    
        db  055h, 0A1h
        dd  obj3:0x0018              ; fixup
        db  0C1h, 0E8h, 00Ch, 08Bh, 0ECh, 083h, 0ECh, 004h, 056h, 06Ah, 000h, 06Ah, 004h, 050h, 0E8h, 09Bh
        db  032h, 000h, 000h, 083h, 0C4h, 00Ch, 085h, 0C0h, 075h, 00Ah, 0BEh, 0FEh, 000h, 000h, 000h, 0E9h
        db  0B8h, 000h, 000h, 000h, 033h, 0C9h, 033h, 0D2h, 0CDh, 020h, 000h, 000h, 001h, 000h, 066h, 089h
        db  045h, 0FCh, 066h, 081h, 07Dh, 0FCh, 00Ah, 004h, 072h, 023h, 06Ah, 000h, 08Dh, 04Dh, 00Ch, 0FFh
        db  075h, 008h, 051h, 068h
        dd  obj3:0x002C              ; fixup
        db  06Ah, 000h, 0E8h, 04Fh, 032h, 000h, 000h, 083h, 0C4h, 014h, 085h, 0C0h, 074h, 007h, 0BEh, 0FEh
        db  000h, 000h, 000h, 0EBh, 06Ah, 033h, 0C9h, 08Dh, 045h, 00Ch, 051h, 068h, 0FEh, 000h, 000h, 000h
        db  050h, 08Bh, 045h, 008h, 040h, 0D1h, 0E8h, 050h, 051h, 00Fh, 0B7h, 015h
        dd  obj3:0x0000              ; fixup
        db  051h, 00Fh, 0B7h, 005h
        dd  obj3:0x0004              ; fixup
        db  00Fh, 0B7h, 00Dh
        dd  obj3:0x0014              ; fixup
        db  051h, 068h, 000h, 040h, 000h, 000h, 052h, 050h, 0E8h, 012h, 032h, 000h, 000h, 083h, 0C4h, 028h
        db  08Bh, 0F0h, 033h, 0C9h, 033h, 0D2h, 0CDh, 020h, 000h, 000h, 001h, 000h, 066h, 089h, 045h, 0FEh
        db  066h, 081h, 07Dh, 0FEh, 00Ah, 004h, 072h, 018h, 06Ah, 000h, 08Dh, 04Dh, 00Ch, 0FFh, 075h, 008h
        db  051h, 068h
        dd  obj3:0x002C              ; fixup
        db  06Ah, 003h, 0E8h, 0DAh, 031h, 000h, 000h, 083h, 0C4h, 014h, 06Ah, 000h, 0A1h
        dd  obj3:0x0018              ; fixup
        db  0C1h, 0E8h, 00Ch, 06Ah, 004h, 050h, 0E8h, 0BDh, 031h, 000h, 000h, 083h, 0C4h, 00Ch, 08Bh, 0C6h
        db  05Eh, 08Bh, 0E5h, 05Dh, 0C2h, 004h, 000h
sub_2_00F3:
        movzx  eax, word ptr [0]             ; -> obj3:0x0008
        shl    eax, 0x10
        push   ebp
        mov    ebp, esp
        inc    eax
        sub    esp, 4
        mov    dword ptr [ebp - 4], eax
        push   word ptr [0]                  ; -> obj3:0x0010
        push   dword ptr [ebp - 4]
        push   3
        cmp    dword ptr [0], 0              ; -> obj3:0x0028
        je     loc_2_0125
        VxDCall  VXDLDR.VXDLDR_UnloadDevice      ; cd2002002900
        jmp    loc_2_012F
loc_2_0125:
        push   8
        call   8
        add    esp, 8
loc_2_012F:
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_0133:
        movzx  eax, word ptr [0]             ; -> obj3:0x0008
        shl    eax, 0x10
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        lea    ecx, [eax + 2]
        mov    dword ptr [ebp - 4], eax
        mov    dword ptr [ebp - 8], ecx
        push   word ptr [0]                  ; -> obj3:0x0010
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 4]
        push   0
        cmp    dword ptr [0], 0              ; -> obj3:0x0028
        je     loc_2_016D
        VxDCall  VXDLDR.VXDLDR_UnloadDevice      ; cd2002002900
        jmp    loc_2_0177
loc_2_016D:
        push   0xc
        call   8
        add    esp, 0xc
loc_2_0177:
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_017B:
        movzx  eax, word ptr [0]             ; -> obj3:0x0008
        shl    eax, 0x10
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        mov    word ptr [ebp - 4], dx
        mov    edx, dword ptr [0]            ; -> obj3:0x000C
        mov    byte ptr [edx], cl
        lea    ecx, [eax + 0xd]
        mov    dword ptr [ebp - 0xc], ecx
        mov    dword ptr [ebp - 8], eax
        push   0
        push   word ptr [0]                  ; -> obj3:0x0010
        push   word ptr [ebp - 4]
        push   dword ptr [ebp - 0xc]
        push   dword ptr [ebp - 8]
        push   1
        xor    eax, eax
        cmp    dword ptr [0], 0              ; -> obj3:0x0028
        je     loc_2_01CA
        VxDCall  VXDLDR.VXDLDR_UnloadDevice      ; cd2002002900
        jmp    loc_2_01DA
loc_2_01CA:
        push   0x10
        call   8
        mov    edx, dword ptr [0]            ; -> obj3:0x000C
        add    esp, 0x10
loc_2_01DA:
        mov    word ptr [ebp - 2], ax
        cmp    dword ptr [edx + 0x10], 0x6081d041
        jne    loc_2_01FF
        add    edx, 0x19
        cmp    byte ptr [edx], 0x22
        jne    loc_2_01FF
        cmp    byte ptr [edx + 3], 0x47
        jne    loc_2_01FF
        cmp    byte ptr [edx + 0xa], 0x10
        jne    loc_2_01FF
        mov    byte ptr [edx + 0xa], 0x20
loc_2_01FF:
        mov    ax, word ptr [ebp - 2]
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_0207:
        movzx  eax, word ptr [0]             ; -> obj3:0x0008
        shl    eax, 0x10
        push   ebp
        mov    ebp, esp
        add    eax, 0xd
        sub    esp, 8
        mov    dword ptr [ebp - 8], eax
        mov    word ptr [ebp - 4], dx
        mov    word ptr [ebp - 2], cx
        push   word ptr [0]                  ; -> obj3:0x0010
        push   word ptr [ebp - 4]
        push   dword ptr [ebp - 8]
        push   word ptr [ebp - 2]
        push   2
        cmp    dword ptr [0], 0              ; -> obj3:0x0028
        je     loc_2_024B
        VxDCall  VXDLDR.VXDLDR_UnloadDevice      ; cd2002002900
        jmp    loc_2_0255
loc_2_024B:
        push   0xc
        call   8
        add    esp, 0xc
loc_2_0255:
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_0259:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    word ptr [ebp - 2], cx
        push   0
        push   word ptr [0]                  ; -> obj3:0x0010
        push   word ptr [ebp - 2]
        push   4
        cmp    dword ptr [0], 0              ; -> obj3:0x0028
        je     loc_2_0285
        VxDCall  VXDLDR.VXDLDR_UnloadDevice      ; cd2002002900
        jmp    loc_2_028F
loc_2_0285:
        push   8
        call   8
        add    esp, 8
loc_2_028F:
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_0293:
        movzx  eax, word ptr [0]             ; -> obj3:0x0008
        shl    eax, 0x10
        push   ebp
        mov    ebp, esp
        add    eax, 3
        sub    esp, 4
        mov    dword ptr [ebp - 4], eax
        push   word ptr [0]                  ; -> obj3:0x0010
        push   dword ptr [ebp - 4]
        push   5
        cmp    dword ptr [0], 0              ; -> obj3:0x0028
        je     loc_2_02C7
        VxDCall  VXDLDR.VXDLDR_UnloadDevice      ; cd2002002900
        jmp    loc_2_02D1
loc_2_02C7:
        push   8
        call   8
        add    esp, 8
loc_2_02D1:
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_02D5:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        mov    dword ptr [ebp - 8], edx
        mov    dword ptr [ebp - 4], ecx
        push   word ptr [0]                  ; -> obj3:0x0010
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 4]
        push   0xb
        cmp    dword ptr [0], 0              ; -> obj3:0x0028
        je     loc_2_0302
        VxDCall  VXDLDR.VXDLDR_UnloadDevice      ; cd2002002900
        jmp    loc_2_030C
loc_2_0302:
        push   0xc
        call   8
        add    esp, 0xc
loc_2_030C:
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_0310:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [ebp - 4], ecx
        push   word ptr [0]                  ; -> obj3:0x0010
        push   dword ptr [ebp - 4]
        push   9
        cmp    dword ptr [0], 0              ; -> obj3:0x0028
        je     loc_2_0337
        VxDCall  VXDLDR.VXDLDR_UnloadDevice      ; cd2002002900
        jmp    loc_2_0341
loc_2_0337:
        push   8
        call   8
        add    esp, 8
loc_2_0341:
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_0345:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [ebp - 4], ecx
        push   word ptr [0]                  ; -> obj3:0x0010
        push   dword ptr [ebp - 4]
        push   0xa
        cmp    dword ptr [0], 0              ; -> obj3:0x0028
        je     loc_2_036C
        VxDCall  VXDLDR.VXDLDR_UnloadDevice      ; cd2002002900
        jmp    loc_2_0376
loc_2_036C:
        push   8
        call   8
        add    esp, 8
loc_2_0376:
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_037A:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        mov    dword ptr [ebp - 8], edx
        mov    dword ptr [ebp - 4], ecx
        push   word ptr [0]                  ; -> obj3:0x0010
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 4]
        push   0x41
        cmp    dword ptr [0], 0              ; -> obj3:0x0028
        je     loc_2_03AA
        VxDCall  VXDLDR.VXDLDR_UnloadDevice      ; cd2002002900
        jmp    loc_2_03B4
loc_2_03AA:
        push   0x10
        call   8
        add    esp, 0x10
loc_2_03B4:
        mov    esp, ebp
        pop    ebp
        ret    4
sub_2_03BA:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        mov    word ptr [ebp - 2], dx
        mov    dword ptr [ebp - 8], ecx
        push   0
        push   word ptr [0]                  ; -> obj3:0x0010
        push   word ptr [ebp - 2]
        push   dword ptr [ebp - 8]
        push   0x42
        cmp    dword ptr [0], 0              ; -> obj3:0x0028
        je     loc_2_03EC
        VxDCall  VXDLDR.VXDLDR_UnloadDevice      ; cd2002002900
        jmp    loc_2_03F6
loc_2_03EC:
        push   0xc
        call   8
        add    esp, 0xc
loc_2_03F6:
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_03FA:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        mov    word ptr [ebp - 2], dx
        mov    dword ptr [ebp - 8], ecx
        push   0
        push   word ptr [0]                  ; -> obj3:0x0010
        push   word ptr [ebp - 2]
        push   dword ptr [ebp - 8]
        push   0x43
        cmp    dword ptr [0], 0              ; -> obj3:0x0028
        je     loc_2_042C
        VxDCall  VXDLDR.VXDLDR_UnloadDevice      ; cd2002002900
        jmp    loc_2_0436
loc_2_042C:
        push   0xc
        call   8
        add    esp, 0xc
loc_2_0436:
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_043A:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0xc]
        mov    dl, byte ptr [esi]
        test   dl, dl
        jne    loc_2_044D
        xor    eax, eax
        jmp    loc_2_04EA
loc_2_044D:
        mov    cl, byte ptr [esi + 1]
        xor    eax, eax
        mov    al, byte ptr [esi + 2]
        xor    ebx, ebx
        and    eax, 0x1f
        mov    bl, cl
        and    ebx, 7
        shl    ebx, 5
        or     eax, ebx
        xor    ebx, ebx
        shl    eax, 6
        mov    bl, dl
        and    ebx, 0x1f
        and    cl, 0x18
        shr    cl, 3
        or     eax, ebx
        movsx  edx, cl
        shl    eax, 2
        mov    cl, byte ptr [esi + 3]
        or     eax, edx
        cmp    cl, 0x39
        movsx  ecx, cl
        jle    loc_2_048E
        sub    ecx, 0x37
        jmp    loc_2_0491
loc_2_048E:
        sub    ecx, 0x30
loc_2_0491:
        movzx  ecx, cl
        shl    ecx, 0x14
        or     eax, ecx
        mov    cl, byte ptr [esi + 4]
        cmp    cl, 0x39
        movsx  ecx, cl
        jle    loc_2_04A9
        sub    ecx, 0x37
        jmp    loc_2_04AC
loc_2_04A9:
        sub    ecx, 0x30
loc_2_04AC:
        movzx  ecx, cl
        shl    ecx, 0x10
        or     eax, ecx
        mov    cl, byte ptr [esi + 5]
        cmp    cl, 0x39
        movsx  ecx, cl
        jle    loc_2_04C4
        sub    ecx, 0x37
        jmp    loc_2_04C7
loc_2_04C4:
        sub    ecx, 0x30
loc_2_04C7:
        movzx  ecx, cl
        shl    ecx, 0x1c
        or     eax, ecx
        mov    cl, byte ptr [esi + 6]
        cmp    cl, 0x39
        movsx  ecx, cl
        jle    loc_2_04DF
        sub    ecx, 0x37
        jmp    loc_2_04E2
loc_2_04DF:
        sub    ecx, 0x30
loc_2_04E2:
        movzx  ecx, cl
        shl    ecx, 0x18
        or     eax, ecx
loc_2_04EA:
        pop    esi
        pop    ebx
        ret    
sub_2_04ED:
        push   esi
        xor    eax, eax
        mov    edx, dword ptr [esp + 8]
        push   edi
        cmp    byte ptr [edx], al
        je     loc_2_0534
        mov    esi, dword ptr [esp + 0x10]
        cmp    esi, 8
        jle    loc_2_0507
        mov    esi, 8
loc_2_0507:
        cmp    esi, 1
        jge    loc_2_0511
        mov    esi, 1
loc_2_0511:
        xor    edi, edi
        test   esi, esi
        jle    loc_2_0534
loc_2_0517:
        shl    eax, 4
        mov    cl, byte ptr [edx + edi]
        cmp    cl, 0x39
        movsx  ecx, cl
        jle    loc_2_052A
        sub    ecx, 0x37
        jmp    loc_2_052D
loc_2_052A:
        sub    ecx, 0x30
loc_2_052D:
        add    eax, ecx
        inc    edi
        cmp    edi, esi
        jl     loc_2_0517
loc_2_0534:
        pop    edi
        pop    esi
        ret    
sub_2_0537:
        mov    ax, word ptr [esp + 0xc]
        push   esi
        test   ax, ax
        push   edi
        je     loc_2_055C
        mov    edi, dword ptr [esp + 0x10]
        mov    esi, dword ptr [esp + 0xc]
        movzx  eax, ax
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_2_055C:
        pop    edi
        pop    esi
        ret    
sub_2_055F:
        mov    eax, dword ptr [esp + 4]
        push   ebx
        push   esi
        xor    esi, esi
        cmp    eax, esi
        je     loc_2_0578
loc_2_056B:
        mov    bl, byte ptr [edx + esi]
        cmp    byte ptr [ecx + esi], bl
        jne    loc_2_0582
        inc    esi
        cmp    esi, eax
        jb     loc_2_056B
loc_2_0578:
        mov    eax, 1
loc_2_057D:
        pop    esi
        pop    ebx
        ret    4
loc_2_0582:
        xor    eax, eax
        jmp    loc_2_057D
sub_2_0586:
        test   ecx, ecx
        jne    loc_2_058E
        mov    eax, ecx
        jmp    loc_2_05AA
loc_2_058E:
        cmp    byte ptr [ecx], 0
        je     loc_2_059D
loc_2_0593:
        cmp    byte ptr [ecx], dl
        je     loc_2_059D
        inc    ecx
        cmp    byte ptr [ecx], 0
        jne    loc_2_0593
loc_2_059D:
        mov    eax, 0
        cmp    byte ptr [ecx], 1
        adc    eax, -1
        and    eax, ecx
loc_2_05AA:
        ret    
sub_2_05AB:
        push   esi
        push   edi
        mov    edi, edx
        mov    esi, ecx
        test   esi, esi
        je     loc_2_05E3
        cmp    dword ptr [esp + 0xc], 1
        jne    loc_2_05C7
        mov    dl, byte ptr [edi]
        mov    ecx, esi
        call   sub_2_0586
        jmp    loc_2_05E5
loc_2_05C7:
        cmp    byte ptr [esi], 0
        je     loc_2_05E3
loc_2_05CC:
        mov    edx, edi
        mov    ecx, esi
        push   dword ptr [esp + 0xc]
        call   sub_2_055F
        test   eax, eax
        jne    loc_2_05E3
        inc    esi
        cmp    byte ptr [esi], 0
        jne    loc_2_05CC
loc_2_05E3:
        mov    eax, esi
loc_2_05E5:
        pop    edi
        pop    esi
        ret    4
sub_2_05EA:
        push   ebp
        mov    eax, ecx
        mov    ebp, esp
        mov    ah, al
        sub    esp, 0xc
        mov    al, ch
        mov    word ptr [ebp - 2], ax
        mov    dword ptr [ebp - 8], ecx
        mov    dword ptr [ebp - 0xc], 1
        push   ebx
        push   esi
        mov    esi, 6                        ; -> obj3:0x0042
loc_2_060B:
        mov    bx, word ptr [ebp - 2]
        mov    al, 3
        sub    al, byte ptr [ebp - 0xc]
        mov    cl, 5
        imul   cl
        mov    cl, al
        inc    dword ptr [ebp - 0xc]
        shr    bx, cl
        and    bl, 0x1f
        add    bl, 0x40
        mov    byte ptr [esi], bl
        inc    esi
        cmp    dword ptr [ebp - 0xc], 3
        jbe    loc_2_060B
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 8]
        movzx  edx, dl
        shr    ecx, 0x18
        push   edx
        shr    eax, 8
        and    ax, 0xff00
        or     cx, ax
        movzx  eax, cx
        push   eax
        push   0                             ; -> obj3:0x0054
        push   esi
        call   sub_2_32C4
        add    esp, 0x10
        mov    eax, 0                        ; -> obj3:0x003C
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_0663:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        mov    dword ptr [ebp - 0xc], ecx
        push   esi
        xor    ebx, ebx
        push   edi
        mov    esi, edx
        push   ebx
        lea    eax, [ebp - 8]
        push   eax
        push   ebx
        push   ecx
        lea    ecx, [ebp - 0xc]
        push   ecx
        call   sub_2_32CC
        add    esp, 0x14
        test   eax, eax
        jne    loc_2_08DF
loc_2_068E:
        mov    edx, dword ptr [ebp - 0xc]
loc_2_0691:
        mov    edi, 1
        mov    al, byte ptr [esi]
        test   al, 0x80
        je     loc_2_06AB
        mov    byte ptr [ebp - 1], al
        movzx  eax, word ptr [esi + 1]
        add    eax, 3
        mov    dword ptr [ebp - 0x10], eax
        jmp    loc_2_06BE
loc_2_06AB:
        mov    cl, al
        and    cl, 0xf8
        mov    byte ptr [ebp - 1], cl
        xor    ecx, ecx
        mov    cl, al
        and    ecx, 7
        inc    ecx
        mov    dword ptr [ebp - 0x10], ecx
loc_2_06BE:
        mov    ecx, dword ptr [ebp - 8]
        and    byte ptr [ebp - 7], 0x7f
        and    ecx, 0x8000
        shr    ecx, 0xf
        test   ecx, ecx
        je     loc_2_06DE
        cmp    dword ptr [0], ebx            ; -> obj3:0x0050
        jne    loc_2_08F1
loc_2_06DE:
        movzx  eax, byte ptr [ebp - 1]
        cmp    eax, 0x28
        jg     loc_2_06F0
        je     loc_2_075D
        cmp    eax, 0x20
        je     loc_2_0733
        jmp    loc_2_072C
loc_2_06F0:
        cmp    eax, 0x40
        je     loc_2_0782
        cmp    eax, 0x48
        je     loc_2_07CD
        cmp    eax, 0x78
        je     loc_2_0905
        cmp    eax, 0x81
        je     loc_2_0806
        cmp    eax, 0x85
        je     loc_2_0852
        cmp    eax, 0x86
        je     loc_2_088F
loc_2_072C:
        xor    edi, edi
        jmp    loc_2_08B7
loc_2_0733:
        cmp    dword ptr [ebp - 8], 4
        jne    loc_2_08F5
        test   ecx, ecx
        je     loc_2_074A
        mov    word ptr [esi + 1], bx
        jmp    loc_2_08B7
loc_2_074A:
        mov    ax, 1
        mov    cl, byte ptr [edx + 2]
        shl    ax, cl
        mov    word ptr [esi + 1], ax
        jmp    loc_2_08B7
loc_2_075D:
        cmp    dword ptr [ebp - 8], 3
        jne    loc_2_08F9
        test   ecx, ecx
        je     loc_2_0773
        mov    byte ptr [esi + 1], bl
        jmp    loc_2_08B7
loc_2_0773:
        mov    al, 1
        mov    cl, byte ptr [edx + 1]
        shl    al, cl
        mov    byte ptr [esi + 1], al
        jmp    loc_2_08B7
loc_2_0782:
        cmp    dword ptr [ebp - 8], 2
        jne    loc_2_08FD
        test   ecx, ecx
        je     loc_2_0796
        mov    word ptr [esi + 2], bx
        jmp    loc_2_079E
loc_2_0796:
        mov    ax, word ptr [edx + 4]
        mov    word ptr [esi + 2], ax
loc_2_079E:
        test   ecx, ecx
        je     loc_2_07A8
        mov    word ptr [esi + 4], bx
        jmp    loc_2_07B0
loc_2_07A8:
        mov    ax, word ptr [edx + 4]
        mov    word ptr [esi + 4], ax
loc_2_07B0:
        test   ecx, ecx
        je     loc_2_07BC
        mov    byte ptr [esi + 7], bl
        jmp    loc_2_08B7
loc_2_07BC:
        mov    ax, word ptr [edx + 6]
        sub    al, byte ptr [edx + 4]
        inc    al
        mov    byte ptr [esi + 7], al
        jmp    loc_2_08B7
loc_2_07CD:
        cmp    dword ptr [ebp - 8], 2
        jne    loc_2_0901
        test   ecx, ecx
        je     loc_2_07E1
        mov    word ptr [esi + 1], bx
        jmp    loc_2_07E9
loc_2_07E1:
        mov    ax, word ptr [edx + 4]
        mov    word ptr [esi + 1], ax
loc_2_07E9:
        test   ecx, ecx
        je     loc_2_07F5
        mov    byte ptr [esi + 3], bl
        jmp    loc_2_08B7
loc_2_07F5:
        mov    ax, word ptr [edx + 6]
        sub    al, byte ptr [edx + 4]
        inc    al
        mov    byte ptr [esi + 3], al
        jmp    loc_2_08B7
loc_2_0806:
        cmp    dword ptr [ebp - 8], 1
        jne    loc_2_0909
        test   ecx, ecx
        je     loc_2_081A
        mov    word ptr [esi + 4], bx
        jmp    loc_2_0824
loc_2_081A:
        mov    eax, dword ptr [edx + 4]
        shr    eax, 8
        mov    word ptr [esi + 4], ax
loc_2_0824:
        test   ecx, ecx
        je     loc_2_082E
        mov    word ptr [esi + 6], bx
        jmp    loc_2_0838
loc_2_082E:
        mov    eax, dword ptr [edx + 4]
        shr    eax, 8
        mov    word ptr [esi + 6], ax
loc_2_0838:
        test   ecx, ecx
        je     loc_2_0842
        mov    word ptr [esi + 0xa], bx
        jmp    loc_2_08B7
loc_2_0842:
        mov    eax, dword ptr [edx + 8]
        sub    eax, dword ptr [edx + 4]
        inc    eax
        shr    eax, 8
        mov    word ptr [esi + 0xa], ax
        jmp    loc_2_08B7
loc_2_0852:
        cmp    dword ptr [ebp - 8], 1
        jne    loc_2_090D
        test   ecx, ecx
        je     loc_2_0865
        mov    dword ptr [esi + 4], ebx
        jmp    loc_2_086B
loc_2_0865:
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [esi + 4], eax
loc_2_086B:
        test   ecx, ecx
        je     loc_2_0874
        mov    dword ptr [esi + 8], ebx
        jmp    loc_2_087A
loc_2_0874:
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [esi + 8], eax
loc_2_087A:
        test   ecx, ecx
        je     loc_2_0883
        mov    dword ptr [esi + 0x10], ebx
        jmp    loc_2_08B7
loc_2_0883:
        mov    eax, dword ptr [edx + 8]
        sub    eax, dword ptr [edx + 4]
        inc    eax
        mov    dword ptr [esi + 0x10], eax
        jmp    loc_2_08B7
loc_2_088F:
        cmp    dword ptr [ebp - 8], 1
        jne    loc_2_0911
        test   ecx, ecx
        je     loc_2_089E
        mov    dword ptr [esi + 4], ebx
        jmp    loc_2_08A4
loc_2_089E:
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [esi + 4], eax
loc_2_08A4:
        test   ecx, ecx
        je     loc_2_08AD
        mov    dword ptr [esi + 8], ebx
        jmp    loc_2_08B7
loc_2_08AD:
        mov    eax, dword ptr [edx + 8]
        sub    eax, dword ptr [edx + 4]
        inc    eax
        mov    dword ptr [esi + 8], eax
loc_2_08B7:
        add    esi, dword ptr [ebp - 0x10]
        test   edi, edi
        je     loc_2_0691
        push   ebx
        lea    eax, [ebp - 8]
        push   eax
        lea    edx, [ebp - 0xc]
        push   ebx
        push   dword ptr [ebp - 0xc]
        push   edx
        call   sub_2_32CC
        add    esp, 0x14
        test   eax, eax
        je     loc_2_068E
loc_2_08DF:
        mov    dl, byte ptr [esi]
        test   dl, 0x80
        mov    al, dl
        je     loc_2_0915
        movzx  ecx, word ptr [esi + 1]
        add    ecx, 3
        jmp    loc_2_091F
loc_2_08F1:
        xor    eax, eax
        jmp    loc_2_0961
loc_2_08F5:
        xor    eax, eax
        jmp    loc_2_0961
loc_2_08F9:
        xor    eax, eax
        jmp    loc_2_0961
loc_2_08FD:
        xor    eax, eax
        jmp    loc_2_0961
loc_2_0901:
        xor    eax, eax
        jmp    loc_2_0961
loc_2_0905:
        xor    eax, eax
        jmp    loc_2_0961
loc_2_0909:
        xor    eax, eax
        jmp    loc_2_0961
loc_2_090D:
        xor    eax, eax
        jmp    loc_2_0961
loc_2_0911:
        xor    eax, eax
        jmp    loc_2_0961
loc_2_0915:
        and    al, 0xf8
        xor    ecx, ecx
        mov    cl, dl
        and    ecx, 7
        inc    ecx
loc_2_091F:
        movzx  eax, al
        cmp    eax, 0x28
        jg     loc_2_0930
        je     loc_2_0958
        cmp    eax, 0x20
        je     loc_2_0958
        jmp    loc_2_0954
loc_2_0930:
        cmp    eax, 0x40
        je     loc_2_0958
        cmp    eax, 0x48
        je     loc_2_0958
        cmp    eax, 0x78
        je     loc_2_095C
        cmp    eax, 0x81
        je     loc_2_0958
        cmp    eax, 0x85
        jl     loc_2_0954
        cmp    eax, 0x86
        jle    loc_2_0958
loc_2_0954:
        add    esi, ecx
        jmp    loc_2_08DF
loc_2_0958:
        xor    eax, eax
        jmp    loc_2_0961
loc_2_095C:
        mov    eax, 1
loc_2_0961:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_0968:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        mov    dword ptr [ebp - 8], ecx
        push   esi
        xor    ebx, ebx
        mov    word ptr [edx], bx
        push   edi
        mov    word ptr [edx + 0x5c], bx
        lea    edi, [edx + 0xae]
        mov    word ptr [edx + 0xbe], bx
        lea    eax, [edx + 2]
        mov    word ptr [edi], bx
        mov    esi, edx
        mov    dword ptr [ebp - 4], 9
        lea    edx, [esi + 0x4a]
loc_2_099C:
        mov    dword ptr [eax], ebx
        mov    dword ptr [eax + 0x24], ebx
        add    eax, 4
        mov    word ptr [edx], bx
        add    edx, 2
        dec    dword ptr [ebp - 4]
        jne    loc_2_099C
        lea    eax, [esi + 0x5e]
        mov    edx, 0x14
        xor    ebx, ebx
loc_2_09B9:
        mov    word ptr [eax], bx
        add    eax, 2
        mov    word ptr [eax + 0x26], bx
        dec    edx
        jne    loc_2_09B9
        xor    edx, edx
loc_2_09C8:
        mov    byte ptr [esi + edx + 0xb0], 0
        mov    byte ptr [esi + edx + 0xb7], 0
        inc    edx
        cmp    edx, 7
        jl     loc_2_09C8
        xor    edx, edx
        xor    eax, eax
loc_2_09E2:
        mov    byte ptr [esi + edx + 0xc0], al
        inc    edx
        mov    word ptr [esi + edx*2 + 0xc5], ax
        cmp    edx, 7
        jl     loc_2_09E2
        xor    ebx, ebx
        lea    eax, [ebp - 4]
        push   ebx
        lea    edx, [ebp - 8]
        push   eax
        push   ebx
        push   ecx
        push   edx
        call   sub_2_32CC
        add    esp, 0x14
        test   eax, eax
        jne    loc_2_0B05
loc_2_0A14:
        test   byte ptr [ebp - 3], 0x80
        mov    ecx, dword ptr [ebp - 8]
        jne    loc_2_0AE8
        cmp    dword ptr [ebp - 4], 1
        jne    loc_2_0A52
        movzx  edx, word ptr [esi]
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [esi + edx*4 + 2], eax
        mov    edx, dword ptr [ecx + 8]
        sub    edx, eax
        movzx  eax, word ptr [esi]
        inc    edx
        mov    dword ptr [esi + eax*4 + 0x26], edx
        mov    dx, word ptr [ecx + 0xc]
        movzx  ecx, word ptr [esi]
        mov    word ptr [esi + ecx*2 + 0x4a], dx
        inc    word ptr [esi]
        jmp    loc_2_0AE8
loc_2_0A52:
        cmp    dword ptr [ebp - 4], 2
        jne    loc_2_0A80
        mov    ax, word ptr [ecx + 4]
        movzx  edx, word ptr [esi + 0x5c]
        mov    cx, word ptr [ecx + 6]
        sub    cx, ax
        mov    word ptr [esi + edx*2 + 0x5e], ax
        movzx  eax, word ptr [esi + 0x5c]
        inc    cx
        mov    word ptr [esi + eax*2 + 0x86], cx
        inc    word ptr [esi + 0x5c]
        jmp    loc_2_0AE8
loc_2_0A80:
        cmp    dword ptr [ebp - 4], 3
        jne    loc_2_0AAF
        mov    al, byte ptr [ecx + 1]
        movzx  ecx, word ptr [esi + 0xbe]
        mov    byte ptr [ecx + esi + 0xc0], al
        movzx  edx, word ptr [esi + 0xbe]
        mov    word ptr [esi + edx*2 + 0xc7], bx
        inc    word ptr [esi + 0xbe]
        jmp    loc_2_0AE8
loc_2_0AAF:
        cmp    dword ptr [ebp - 4], 4
        jne    loc_2_0AE8
        movzx  edx, word ptr [edi]
        mov    al, byte ptr [ecx + 2]
        mov    byte ptr [edx + esi + 0xb0], al
        mov    al, byte ptr [ecx]
        movzx  ecx, word ptr [edi]
        and    al, 1
        mov    byte ptr [ecx + esi + 0xb7], al
        mov    al, byte ptr [ebp + 8]
        movzx  edx, word ptr [edi]
        sub    al, 4
        cmp    al, 1
        sbb    al, al
        and    al, 2
        or     byte ptr [edx + esi + 0xb7], al
        inc    word ptr [edi]
loc_2_0AE8:
        push   ebx
        lea    eax, [ebp - 4]
        push   eax
        lea    edx, [ebp - 8]
        push   ebx
        push   dword ptr [ebp - 8]
        push   edx
        call   sub_2_32CC
        add    esp, 0x14
        test   eax, eax
        je     loc_2_0A14
loc_2_0B05:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
sub_2_0B0E:
        sub    esp, 4
        push   ebx
        push   esi
        mov    esi, 0x100
        mov    ebx, 0x80
loc_2_0B1D:
        mov    al, byte ptr [ecx]
        mov    dl, al
        and    dl, bl
        mov    byte ptr [esp + 8], dl
        cmp    dl, bl
        jne    loc_2_0B34
        movzx  edx, word ptr [ecx + 1]
        add    edx, 3
        jmp    loc_2_0B3C
loc_2_0B34:
        xor    edx, edx
        mov    dl, al
        and    edx, 7
        inc    edx
loc_2_0B3C:
        add    ecx, edx
        cmp    byte ptr [esp + 8], bl
        je     loc_2_0B4A
        and    al, 0xf8
        cmp    al, 0x78
        je     loc_2_0B4D
loc_2_0B4A:
        dec    esi
        jne    loc_2_0B1D
loc_2_0B4D:
        mov    eax, 0
        cmp    esi, 1
        adc    eax, -1
        pop    esi
        and    eax, ecx
        pop    ebx
        add    esp, 4
        ret    
sub_2_0B60:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        lea    eax, [ebp - 8]
        mov    esi, ecx
        lea    edx, [ebp - 4]
        mov    dword ptr [ebp - 8], 4
        xor    ecx, ecx
        push   ecx
        mov    dword ptr [ebp - 4], ecx
        push   eax
        push   edx
        push   3
        push   0                             ; -> obj3:0x016C
        push   ecx
        push   esi
        call   sub_2_32D4
        add    esp, 0x1c
        mov    eax, 1
        test   byte ptr [ebp - 4], 1
        jne    loc_2_0BCA
        xor    ecx, ecx
        push   0x200
        lea    eax, [ebp - 8]
        lea    edx, [ebp - 4]
        push   eax
        mov    dword ptr [ebp - 4], ecx
        mov    dword ptr [ebp - 8], 4
        push   edx
        push   3
        push   0                             ; -> obj3:0x0178
        push   ecx
        push   esi
        call   sub_2_32D4
        add    esp, 0x1c
        mov    eax, dword ptr [ebp - 4]
        and    eax, 1
loc_2_0BCA:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_0BCF:
        push   esi
        mov    esi, ecx
        mov    ecx, dword ptr [0]            ; -> obj3:0x000C
        add    ecx, 0x19
        call   sub_2_0B0E
        test   eax, eax
        je     loc_2_0BFD
        mov    ecx, eax
        call   sub_2_0B0E
        test   eax, eax
        je     loc_2_0BFD
        push   1
        push   0
        push   esi
        push   eax
        call   sub_2_32DC
        add    esp, 0x10
loc_2_0BFD:
        pop    esi
        ret    
sub_2_0BFF:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x1c
        push   ebx
        push   esi
        push   edi
        call   sub_2_0133
        test   ax, ax
        jne    loc_2_0EBC
        mov    eax, dword ptr [0]            ; -> obj3:0x000C
        movzx  esi, word ptr [eax + 2]
        cmp    dword ptr [0], esi            ; -> obj3:0x0164
        jae    loc_2_0C5F
        lea    edi, [esi*2 + 0x19]
        push   0x202
        push   edi
        push   eax
        call   sub_2_332C
        add    esp, 0xc
        test   eax, eax
        je     loc_2_0EBC
        mov    cx, word ptr [0]              ; -> obj3:0x0008
        push   edi
        mov    edx, eax
        mov    dword ptr [0], esi            ; -> obj3:0x0164
        mov    dword ptr [0], eax            ; -> obj3:0x000C
        call   sub_2_2697
loc_2_0C5F:
        push   0
        mov    eax, dword ptr [0]            ; -> obj3:0x005C
        push   eax
        xor    bl, bl
        xor    esi, esi
        call   sub_2_3324
        add    esp, 8
        mov    edi, 1
loc_2_0C78:
        mov    edx, edi
        mov    ecx, ebx
        call   sub_2_017B
        test   ax, ax
        mov    eax, dword ptr [0]            ; -> obj3:0x000C
        jne    loc_2_0EAC
        mov    edx, ebx
        mov    ecx, dword ptr [eax + 0x10]
        call   sub_2_05EA
        push   esi
        lea    edx, [ebp - 8]
        push   dword ptr [0]                 ; -> obj3:0x005C
        push   eax
        push   edx
        call   sub_2_331C
        add    esp, 0x10
        test   eax, eax
        jne    loc_2_0E9C
        mov    eax, dword ptr [0]            ; -> obj3:0x000C
        push   esi
        movzx  eax, word ptr [eax + 0x17]
        movzx  ecx, bl
        shl    eax, 0x10
        add    eax, ecx
        push   eax
        push   dword ptr [0]                 ; -> obj3:0x005C
        push   dword ptr [ebp - 8]
        call   sub_2_3314
        add    esp, 0x10
        mov    byte ptr [ebp - 1], bl
        lea    ebx, [ebp - 1]
        push   esi
        push   ebx
        push   edi
        push   0x40
        push   dword ptr [ebp - 8]
        call   sub_2_330C
        add    esp, 0x14
        mov    eax, dword ptr [0]            ; -> obj3:0x000C
        add    eax, 0x19
        push   2
        push   esi
        push   dword ptr [ebp - 8]
        push   eax
        call   sub_2_32DC
        add    esp, 0x10
        mov    ecx, dword ptr [0]            ; -> obj3:0x000C
        add    ecx, 0x19
        call   sub_2_0B0E
        mov    edx, dword ptr [0]            ; -> obj3:0x000C
        test   eax, eax
        je     loc_2_0D31
        mov    cl, byte ptr [eax]
        test   cl, 0x80
        jne    loc_2_0D2D
        and    cl, 0xf8
        cmp    cl, 0x78
        je     loc_2_0D31
loc_2_0D2D:
        mov    ebx, edi
        jmp    loc_2_0D33
loc_2_0D31:
        xor    ebx, ebx
loc_2_0D33:
        test   ebx, ebx
        je     loc_2_0D42
        test   byte ptr [edx + 0x17], 2
        mov    ecx, 0
        je     loc_2_0D47
loc_2_0D42:
        mov    ecx, 3
loc_2_0D47:
        test   ebx, ebx
        jne    loc_2_0D4E
        lea    eax, [edx + 0x19]
loc_2_0D4E:
        push   ecx
        push   esi
        push   dword ptr [ebp - 8]
        push   eax
        call   sub_2_32DC
        add    esp, 0x10
        mov    ecx, dword ptr [0]            ; -> obj3:0x000C
        mov    eax, dword ptr [ecx + 0x10]
        cmp    eax, 0x30ed041
        jne    loc_2_0D7D
        push   esi
        push   dword ptr [ebp - 8]
        call   sub_2_3304
        add    esp, 8
        jmp    loc_2_0E9C
loc_2_0D7D:
        cmp    eax, 0x6d041
        jne    loc_2_0DFF
        push   3
        lea    ecx, [ebp - 0x1c]
        push   dword ptr [ebp - 8]
        push   ecx
        call   sub_2_32FC
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_0DFF
        push   esi
        lea    eax, [ebp - 0x18]
        push   eax
        lea    edx, [ebp - 0x10]
        push   esi
        push   dword ptr [ebp - 0x1c]
        push   edx
        call   sub_2_32CC
        add    esp, 0x14
        test   eax, eax
        jne    loc_2_0DFF
        cmp    dword ptr [ebp - 0x18], 4
        jne    loc_2_0DFF
        mov    eax, dword ptr [ebp - 0x10]
        mov    ax, word ptr [eax + 2]
        cmp    ax, 0xe
        je     loc_2_0DCB
        cmp    ax, 0xf
        jne    loc_2_0DFF
loc_2_0DCB:
        push   esi
        lea    ecx, [ebp - 0x10]
        push   esi
        push   esi
        push   dword ptr [ebp - 0x10]
        push   ecx
        call   sub_2_32CC
        add    esp, 0x14
        cmp    eax, 0xf
        jne    loc_2_0DFF
        push   esi
        mov    dword ptr [0], edi            ; -> obj3:0x0168
        push   dword ptr [ebp - 8]
        call   sub_2_3304
        add    esp, 8
        mov    dword ptr [0], esi            ; -> obj3:0x0168
        jmp    loc_2_0E9C
loc_2_0DFF:
        push   3
        push   dword ptr [ebp - 8]
        push   esi
        call   sub_2_32FC
        add    esp, 0xc
        test   eax, eax
        je     loc_2_0E9C
        push   esi
        push   dword ptr [ebp - 8]
        push   esi
        call   sub_2_32FC
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_0E9C
        mov    eax, dword ptr [0]            ; -> obj3:0x000C
        mov    ax, word ptr [eax + 0x17]
        test   al, 2
        je     loc_2_0E42
        push   2
        push   dword ptr [ebp - 8]
        call   sub_2_32F4
        add    esp, 8
        jmp    loc_2_0E9C
loc_2_0E42:
        test   ax, 0x180
        jne    loc_2_0E9C
        mov    ecx, dword ptr [ebp - 8]
        call   sub_2_0B60
        test   eax, eax
        jne    loc_2_0E9C
        mov    ebx, 0x200
        lea    eax, [ebp - 0x14]
        push   ebx
        lea    ecx, [ebp - 0xc]
        push   eax
        mov    dword ptr [ebp - 0xc], esi
        mov    dword ptr [ebp - 0x14], 4
        push   ecx
        push   3
        push   0                             ; -> obj3:0x0188
        push   esi
        push   dword ptr [ebp - 8]
        call   sub_2_32D4
        add    esp, 0x1c
        lea    ecx, [ebp - 0xc]
        or     dword ptr [ebp - 0xc], edi
        push   ebx
        push   4
        push   ecx
        push   3
        push   0                             ; -> obj3:0x0198
        push   esi
        push   dword ptr [ebp - 8]
        call   sub_2_32EC
        add    esp, 0x1c
loc_2_0E9C:
        mov    eax, dword ptr [0]            ; -> obj3:0x000C
        mov    bl, byte ptr [eax]
        cmp    bl, 0xff
        jne    loc_2_0C78
loc_2_0EAC:
        push   0
        mov    eax, dword ptr [0]            ; -> obj3:0x005C
        push   eax
        call   sub_2_32E4
        add    esp, 8
loc_2_0EBC:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_0EC3:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   esi
        lea    eax, [ebp - 8]
        xor    esi, esi
        push   eax
        push   1
        mov    dword ptr [ebp - 4], esi
        push   0                             ; -> obj3:0x01A8
        push   2
        call   sub_2_3344
        add    esp, 0x10
        test   eax, eax
        jne    loc_2_0F17
        lea    eax, [ebp - 0xc]
        lea    ecx, [ebp - 4]
        push   eax
        lea    edx, [ebp - 0x10]
        mov    dword ptr [ebp - 0xc], 4
        push   ecx
        push   edx
        push   esi
        push   0                             ; -> obj3:0x01B0
        push   dword ptr [ebp - 8]
        call   sub_2_333C
        add    esp, 0x18
        push   dword ptr [ebp - 8]
        call   sub_2_3334
        add    esp, 4
loc_2_0F17:
        test   byte ptr [ebp - 4], 1
        jne    loc_2_0F22
        call   sub_2_3178
loc_2_0F22:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_0F27:
        mov    ecx, 0x43
        jmp    sub_2_0259
sub_2_0F31:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        mov    byte ptr [ebp - 1], dl
        push   esi
        lea    eax, [ebp - 8]
        push   edi
        mov    dword ptr [ebp - 0xc], ecx
        push   2
        push   ecx
        push   eax
        call   sub_2_32FC
        add    esp, 0xc
        test   eax, eax
        je     loc_2_0F5A
        xor    eax, eax
        jmp    loc_2_107E
loc_2_0F5A:
        mov    edi, dword ptr [0]            ; -> obj3:0x000C
        add    edi, 0x19
        mov    ecx, edi
        call   sub_2_0B0E
        test   eax, eax
        je     loc_2_1079
        sub    eax, edi
        lea    ebx, [eax - 2]
        test   ebx, ebx
        je     loc_2_1079
        mov    eax, dword ptr [0]            ; -> obj3:0x0164
        cmp    ebx, eax
        jae    loc_2_1079
        mov    ecx, dword ptr [0]            ; -> obj3:0x000C
        push   ebx
        push   edi
        lea    esi, [ecx + eax + 0x19]
        push   esi
        call   sub_2_334C
        add    esp, 0xc
        mov    edx, edi
        mov    ecx, dword ptr [ebp - 8]
        call   sub_2_0663
        test   eax, eax
        je     loc_2_1079
        mov    ecx, ebx
        shr    ecx, 2
        repe cmpsd dword ptr [esi], dword ptr es:[edi]
        jne    loc_2_0FC3
        mov    ecx, ebx
        and    ecx, 3
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
loc_2_0FC3:
        je     loc_2_0FFC
        mov    eax, dword ptr [0]            ; -> obj3:0x000C
        mov    ax, word ptr [eax + 0x17]
        test   al, 2
        jne    loc_2_1079
        xor    ecx, ecx
        mov    cx, ax
        mov    eax, 0x180
        and    ecx, eax
        je     loc_2_1000
        cmp    ecx, 0x80
        je     loc_2_101D
        cmp    ecx, 0x100
        je     loc_2_1079
        cmp    ecx, eax
        je     loc_2_1065
loc_2_0FFC:
        xor    eax, eax
        jmp    loc_2_107E
loc_2_1000:
        movzx  cx, byte ptr [ebp - 1]
        mov    edx, 2
        call   sub_2_0207
        cmp    ax, 1
        sbb    eax, eax
        and    eax, 0xf
        add    eax, 0x13
        jmp    loc_2_107E
loc_2_101D:
        push   4
        lea    ecx, [ebp - 8]
        push   dword ptr [ebp - 0xc]
        push   ecx
        call   sub_2_32FC
        movzx  cx, byte ptr [ebp - 1]
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_104E
        mov    edx, 3
        call   sub_2_0207
        test   ax, ax
        je     loc_2_0FFC
        cmp    ax, 0x7f
        je     loc_2_0FFC
        jmp    loc_2_1079
loc_2_104E:
        mov    edx, 1
        call   sub_2_0207
        test   ax, ax
        je     loc_2_0FFC
        cmp    ax, 0x7f
        je     loc_2_0FFC
        jmp    loc_2_1079
loc_2_1065:
        movzx  cx, byte ptr [ebp - 1]
        mov    edx, 1
        call   sub_2_0207
        test   ax, ax
        je     loc_2_0FFC
loc_2_1079:
        mov    eax, 0x13
loc_2_107E:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_1085:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        mov    byte ptr [ebp - 2], dl
        push   esi
        mov    dword ptr [ebp - 0xc], ecx
        xor    si, si
        push   edi
        push   0
        push   0x100000
        push   ecx
        call   sub_2_3354
        add    esp, 0xc
        mov    ebx, dword ptr [0]            ; -> obj3:0x000C
        mov    ax, word ptr [ebx + 0x17]
        test   al, 1
        jne    loc_2_118E
        xor    ecx, ecx
        mov    cx, ax
        mov    eax, 0x180
        and    ecx, eax
        je     loc_2_10DD
        cmp    ecx, 0x80
        je     loc_2_10ED
        cmp    ecx, 0x100
        je     loc_2_10ED
        cmp    ecx, eax
        je     loc_2_10FC
        jmp    loc_2_1100
loc_2_10DD:
        cmp    dword ptr [ebp + 8], 0
        je     loc_2_118E
        mov    si, 2
        jmp    loc_2_1100
loc_2_10ED:
        cmp    dword ptr [ebp + 8], 1
        sbb    esi, esi
        and    esi, 0xfffffffe
        add    si, 3
        jmp    loc_2_1100
loc_2_10FC:
        mov    si, 1
loc_2_1100:
        add    ebx, 0x19
loc_2_1103:
        mov    al, byte ptr [ebx]
        test   al, 0x80
        je     loc_2_111A
        movzx  edx, word ptr [ebx + 1]
        add    edx, 3
        mov    byte ptr [ebp - 1], al
        mov    eax, 3
        jmp    loc_2_1136
loc_2_111A:
        mov    cl, al
        xor    edx, edx
        and    cl, 0xf8
        mov    dl, al
        and    edx, 7
        mov    eax, 1
        inc    edx
        mov    byte ptr [ebp - 1], cl
        cmp    cl, 8
        jne    loc_2_1136
        mov    eax, edx
loc_2_1136:
        lea    edi, [eax + ebx]
        mov    dword ptr [ebp - 8], edx
        add    ebx, edx
        sub    dword ptr [ebp - 8], eax
        xor    eax, eax
        mov    ecx, dword ptr [ebp - 8]
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        mov    ecx, dword ptr [ebp - 8]
        and    ecx, 3
        rep stosb byte ptr es:[edi], al
        cmp    byte ptr [ebp - 1], 0x78
        jne    loc_2_1103
        movzx  cx, byte ptr [ebp - 2]
        mov    edx, esi
        call   sub_2_0207
        cmp    si, 3
        je     loc_2_1171
        cmp    si, 1
        jne    loc_2_118E
loc_2_1171:
        test   ax, ax
        je     loc_2_117C
        cmp    ax, 0x7f
        je     loc_2_118E
loc_2_117C:
        push   1
        push   0x100000
        push   dword ptr [ebp - 0xc]
        call   sub_2_3354
        add    esp, 0xc
loc_2_118E:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_1197:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        mov    esi, dword ptr [ebp + 0x14]
        cmp    dword ptr [0], esi            ; -> obj3:0x005C
        jne    loc_2_11E3
        cmp    dword ptr [ebp + 8], 5
        jne    loc_2_11B9
        call   sub_2_0BFF
        jmp    loc_2_12DB
loc_2_11B9:
        cmp    dword ptr [ebp + 8], 0x12
        jne    loc_2_11C9
        call   sub_2_0EC3
        jmp    loc_2_12DB
loc_2_11C9:
        cmp    dword ptr [ebp + 8], 0xd
        jne    loc_2_11D9
        call   sub_2_0F27
        jmp    loc_2_12DB
loc_2_11D9:
        mov    eax, 1
        jmp    loc_2_12DD
loc_2_11E3:
        push   0
        lea    eax, [ebp - 4]
        push   esi
        push   eax
        call   sub_2_336C
        add    esp, 0xc
        mov    eax, dword ptr [0]            ; -> obj3:0x005C
        cmp    dword ptr [ebp - 4], eax
        je     loc_2_1206
        mov    eax, 1
        jmp    loc_2_12DD
loc_2_1206:
        push   0
        lea    ecx, [ebp - 4]
        push   eax
        push   esi
        push   ecx
        call   sub_2_3364
        add    esp, 0x10
        mov    edx, 1
        mov    ecx, dword ptr [ebp - 4]
        call   sub_2_017B
        test   ax, ax
        je     loc_2_1232
        mov    eax, 1
        jmp    loc_2_12DD
loc_2_1232:
        mov    eax, dword ptr [ebp + 8]
        cmp    eax, 1
        je     loc_2_125C
        cmp    eax, 2
        je     loc_2_1268
        cmp    eax, 3
        je     loc_2_1285
        cmp    eax, 4
        je     loc_2_1268
        cmp    eax, 6
        je     loc_2_12D4
        mov    eax, 1
        jmp    loc_2_12DD
loc_2_125C:
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, esi
        call   sub_2_0F31
        jmp    loc_2_12DD
loc_2_1268:
        cmp    dword ptr [0], 0              ; -> obj3:0x0168
        jne    loc_2_12DB
        mov    ecx, esi
        call   sub_2_0B60
        push   eax
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, esi
        call   sub_2_1085
        jmp    loc_2_12DB
loc_2_1285:
        push   0
        lea    eax, [ebp - 8]
        push   esi
        push   0
        push   eax
        call   sub_2_335C
        add    esp, 0x10
        test   byte ptr [ebp - 6], 0x40
        je     loc_2_12A3
        mov    eax, 0x13
        jmp    loc_2_12DD
loc_2_12A3:
        mov    eax, dword ptr [ebp + 0xc]
        test   eax, eax
        je     loc_2_12B6
        cmp    eax, 1
        je     loc_2_12C3
loc_2_12AF:
        mov    eax, 1
        jmp    loc_2_12DD
loc_2_12B6:
        test   byte ptr [ebp - 2], 1
        je     loc_2_12AF
        mov    eax, 0x13
        jmp    loc_2_12DD
loc_2_12C3:
        mov    ax, word ptr [ebp - 2]
        and    al, 0xe2
        cmp    al, 2
        jne    loc_2_12AF
        mov    eax, 0x13
        jmp    loc_2_12DD
loc_2_12D4:
        mov    ecx, esi
        call   sub_2_0BCF
loc_2_12DB:
        xor    eax, eax
loc_2_12DD:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_12E2:
        push   1
        push   0                             ; -> loc_2_1197
        push   ecx
        call   sub_2_3374
        add    esp, 0xc
        ret    
sub_2_12F3:
        test   edx, edx
        jle    loc_2_12FD
loc_2_12F7:
        shr    cx, 1
        dec    edx
        jne    loc_2_12F7
loc_2_12FD:
        xor    eax, eax
        mov    ax, cx
        and    eax, 1
        ret    
sub_2_1306:
        xor    ax, ax
        push   esi
        push   edi
        xor    di, di
        test   dx, dx
        je     loc_2_1325
loc_2_1313:
        movzx  esi, di
        inc    di
        movzx  si, byte ptr [esi + ecx]
        add    ax, si
        cmp    di, dx
        jb     loc_2_1313
loc_2_1325:
        neg    ax
        pop    edi
        pop    esi
        ret    
sub_2_132B:
        test   dx, dx
        jne    loc_2_1336
        mov    dx, 0xc
        jmp    loc_2_133D
loc_2_1336:
        movzx  eax, dx
        add    dx, word ptr [eax + ecx]
loc_2_133D:
        mov    ax, dx
        ret    
sub_2_1341:
        mov    ax, word ptr [ecx]
        add    ax, 2
        ret    
sub_2_1349:
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        lea    edi, [edx + 0xc]
        mov    bl, byte ptr [esp + 0x10]
        test   bl, bl
        je     loc_2_136B
loc_2_1359:
        movzx  eax, di
        lea    ecx, [eax + esi]
        call   sub_2_1341
        add    di, ax
        dec    bl
        jne    loc_2_1359
loc_2_136B:
        mov    ax, di
        pop    edi
        pop    esi
        pop    ebx
        ret    4
sub_2_1374:
        push   ebx
        push   esi
        mov    bl, 1
        lea    esi, [ecx + 0xc]
        mov    ecx, esi
        call   sub_2_1341
        movzx  eax, ax
        cmp    word ptr [eax + esi], 0
        je     loc_2_13AB
loc_2_138C:
        mov    ecx, esi
        inc    bl
        call   sub_2_1341
        movzx  eax, ax
        add    esi, eax
        mov    ecx, esi
        call   sub_2_1341
        movzx  eax, ax
        cmp    word ptr [eax + esi], 0
        jne    loc_2_138C
loc_2_13AB:
        cmp    dword ptr [esi + 6], 0x47464341
        jne    loc_2_13B6
        dec    bl
loc_2_13B6:
        mov    al, bl
        pop    esi
        pop    ebx
        ret    
sub_2_13BB:
        push   ebp
        mov    al, byte ptr [ecx + 8]
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        mov    dword ptr [ebp - 8], ecx
        push   esi
        mov    byte ptr [ebp - 1], al
        push   edi
        lea    esi, [ecx + 0xc]
        mov    di, 0xc
        test   al, al
        je     loc_2_1430
loc_2_13D8:
        mov    bx, 0xc
        mov    dword ptr [ebp - 0xc], esi
        mov    dword ptr [ebp - 0x10], esi
        add    esi, 0xc
        cmp    word ptr [esi], 0
        je     loc_2_1407
loc_2_13EB:
        mov    ecx, esi
        call   sub_2_1341
        add    bx, ax
        mov    ecx, esi
        call   sub_2_1341
        movzx  eax, ax
        add    esi, eax
        cmp    word ptr [esi], 0
        jne    loc_2_13EB
loc_2_1407:
        add    bx, 2
        add    esi, 2
        mov    eax, dword ptr [ebp - 0x10]
        mov    edx, ebx
        lea    ecx, [ebx + 2]
        lea    edi, [ebx + edi + 2]
        mov    word ptr [eax], cx
        mov    ecx, dword ptr [ebp - 0xc]
        call   sub_2_1306
        mov    word ptr [esi], ax
        add    esi, 2
        dec    byte ptr [ebp - 1]
        jne    loc_2_13D8
loc_2_1430:
        lea    ecx, [edi + 2]
        mov    eax, dword ptr [ebp - 8]
        mov    word ptr [eax], cx
        mov    edx, edi
        mov    ecx, eax
        call   sub_2_1306
        mov    word ptr [esi], ax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_144C:
        push   esi
        lea    esi, [ecx + 0xc]
        cmp    word ptr [esi], 0
        jne    loc_2_145A
        xor    eax, eax
        jmp    loc_2_1497
loc_2_145A:
        mov    ecx, esi
        call   sub_2_1341
        movzx  eax, ax
        cmp    word ptr [eax + esi], 0
        je     loc_2_1488
loc_2_146B:
        mov    ecx, esi
        call   sub_2_1341
        movzx  eax, ax
        add    esi, eax
        mov    ecx, esi
        call   sub_2_1341
        movzx  ecx, ax
        cmp    word ptr [ecx + esi], 0
        jne    loc_2_146B
loc_2_1488:
        mov    eax, dword ptr [esi + 6]
        sub    eax, 0x47464341
        cmp    eax, 1
        sbb    eax, eax
        and    eax, esi
loc_2_1497:
        pop    esi
        ret    
sub_2_1499:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        mov    dword ptr [ebp - 8], edx
        push   esi
        xor    edx, edx
        push   edi
        mov    edi, ecx
        call   sub_2_132B
        mov    bx, ax
        mov    al, byte ptr [edi + 8]
        mov    byte ptr [ebp - 2], al
        test   al, al
        je     loc_2_152F
loc_2_14BB:
        movzx  eax, bx
        lea    ecx, [eax + edi]
        call   sub_2_144C
        test   eax, eax
        je     loc_2_151E
        movzx  ecx, byte ptr [eax + 0xc]
        cmp    ecx, 4
        je     loc_2_14DA
        cmp    ecx, 0x10
        je     loc_2_150C
        jmp    loc_2_151E
loc_2_14DA:
        lea    esi, [eax + 0x16]
        movzx  eax, bx
        mov    byte ptr [ebp - 1], 0
        lea    ecx, [eax + edi]
        call   sub_2_1374
        test   al, al
        je     loc_2_151E
        mov    dl, byte ptr [ebp - 8]
loc_2_14F3:
        cmp    byte ptr [esi], dl
        jne    loc_2_14FF
        mov    cl, byte ptr [ebp + 8]
        cmp    byte ptr [esi + 1], cl
        je     loc_2_153B
loc_2_14FF:
        add    esi, 8
        inc    byte ptr [ebp - 1]
        cmp    al, byte ptr [ebp - 1]
        ja     loc_2_14F3
        jmp    loc_2_151E
loc_2_150C:
        add    eax, 0x16
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [eax], ecx
        jne    loc_2_151E
        mov    ecx, dword ptr [ebp + 8]
        cmp    dword ptr [eax + 4], ecx
        je     loc_2_1549
loc_2_151E:
        mov    edx, ebx
        mov    ecx, edi
        call   sub_2_132B
        mov    bx, ax
        dec    byte ptr [ebp - 2]
        jne    loc_2_14BB
loc_2_152F:
        xor    ax, ax
loc_2_1532:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_2_153B:
        mov    edx, ebx
        mov    ecx, edi
        push   dword ptr [ebp - 1]
        call   sub_2_1349
        jmp    loc_2_1532
loc_2_1549:
        mov    edx, ebx
        mov    ecx, edi
        push   dword ptr [ebp + 0xc]
        call   sub_2_1349
        jmp    loc_2_1532
sub_2_1557:
        movzx  eax, word ptr [ecx + 0xc]
        push   esi
        add    eax, 0xc
        mov    si, 0xc
        push   edi
        movzx  edi, dx
        cmp    eax, edi
        jge    loc_2_157F
loc_2_156B:
        movzx  eax, si
        add    si, word ptr [eax + ecx]
        movzx  eax, si
        movzx  edx, word ptr [eax + ecx]
        add    edx, eax
        cmp    edx, edi
        jl     loc_2_156B
loc_2_157F:
        movzx  eax, si
        lea    eax, [eax + ecx + 9]
        mov    cl, byte ptr [eax]
        or     cl, 4
        mov    byte ptr [eax], cl
        and    cl, 0xc7
        pop    edi
        mov    byte ptr [eax], cl
        or     cl, 0x40
        pop    esi
        mov    byte ptr [eax], cl
        and    cl, 0x7f
        mov    byte ptr [eax], cl
        ret    
sub_2_159F:
        push   ebx
        push   esi
        mov    si, word ptr [ecx]
        push   edi
        mov    di, dx
        mov    ebx, ecx
        sub    si, 2
        xor    edx, edx
        call   sub_2_132B
        cmp    si, ax
        jbe    loc_2_15D7
        movzx  edi, di
loc_2_15BD:
        movzx  edx, ax
        movzx  ecx, byte ptr [edx + ebx + 2]
        cmp    ecx, edi
        je     loc_2_15DA
        mov    edx, eax
        mov    ecx, ebx
        call   sub_2_132B
        cmp    si, ax
        ja     loc_2_15BD
loc_2_15D7:
        xor    ax, ax
loc_2_15DA:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_2_15DE:
        push   ebx
        push   esi
        mov    bx, word ptr [ecx]
        push   edi
        sub    bx, 2
        mov    edi, edx
        mov    esi, ecx
        xor    edx, edx
        call   sub_2_132B
        cmp    ax, bx
        jae    loc_2_160F
loc_2_15F8:
        movzx  ecx, ax
        cmp    dword ptr [ecx + esi + 4], edi
        je     loc_2_1612
        mov    edx, eax
        mov    ecx, esi
        call   sub_2_132B
        cmp    ax, bx
        jb     loc_2_15F8
loc_2_160F:
        xor    ax, ax
loc_2_1612:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_2_1616:
        sub    esp, 0x10
        mov    edx, 0xffffd041
        push   ebx
        push   esi
        mov    bx, word ptr [ecx]
        push   edi
        sub    bx, 2
        mov    esi, ecx
        call   sub_2_15DE
        test   ax, ax
        je     loc_2_1643
        cmp    bx, ax
        jbe    loc_2_1643
        movzx  ecx, ax
        movzx  eax, byte ptr [ecx + esi + 2]
        jmp    loc_2_1693
loc_2_1643:
        lea    edi, [esp + 0xc]
        xor    eax, eax
        xor    edx, edx
        mov    ecx, esi
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        call   sub_2_132B
        test   ax, ax
        je     loc_2_1682
loc_2_165B:
        cmp    bx, ax
        jbe    loc_2_1682
        movzx  ecx, ax
        mov    cl, byte ptr [ecx + esi + 2]
        cmp    cl, 0x10
        jae    loc_2_1674
        movzx  ecx, cl
        mov    byte ptr [esp + ecx + 0xc], 1
loc_2_1674:
        mov    edx, eax
        mov    ecx, esi
        call   sub_2_132B
        test   ax, ax
        jne    loc_2_165B
loc_2_1682:
        mov    eax, 0xf
loc_2_1687:
        cmp    byte ptr [esp + eax + 0xc], 0
        je     loc_2_1693
        dec    eax
        test   eax, eax
        jg     loc_2_1687
loc_2_1693:
        test   eax, eax
        jg     loc_2_1699
        mov    al, 0xf
loc_2_1699:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x10
        ret    
sub_2_16A0:
        push   esi
        mov    esi, ecx
        call   sub_2_15DE
        test   ax, ax
        jne    loc_2_16B3
        mov    ax, 1
        jmp    loc_2_16E3
loc_2_16B3:
        movzx  edx, ax
        movzx  eax, word ptr [esi]
        movzx  ecx, word ptr [edx + esi]
        sub    eax, ecx
        cmp    eax, edx
        jle    loc_2_16D6
loc_2_16C3:
        lea    eax, [esi + edx]
        mov    al, byte ptr [eax + ecx]
        mov    byte ptr [esi + edx], al
        inc    edx
        movzx  eax, word ptr [esi]
        sub    eax, ecx
        cmp    eax, edx
        jg     loc_2_16C3
loc_2_16D6:
        mov    ecx, esi
        dec    byte ptr [esi + 8]
        call   sub_2_13BB
        xor    ax, ax
loc_2_16E3:
        pop    esi
        ret    
sub_2_16E5:
        xor    eax, eax
        cmp    word ptr [ecx], ax
        jne    loc_2_1704
        cmp    word ptr [ecx + 0x5c], ax
        jne    loc_2_1704
        cmp    word ptr [ecx + 0xae], ax
        jne    loc_2_1704
        cmp    word ptr [ecx + 0xbe], ax
        je     loc_2_1709
loc_2_1704:
        mov    eax, 1
loc_2_1709:
        ret    
sub_2_170A:
        push   esi
        push   edi
        mov    si, word ptr [ecx]
        movzx  eax, si
        cmp    eax, edx
        jle    loc_2_174C
        dec    si
        movzx  edi, si
        cmp    edx, edi
        je     loc_2_1739
        mov    eax, dword ptr [ecx + edi*4 + 2]
        mov    dword ptr [ecx + edx*4 + 2], eax
        mov    eax, dword ptr [ecx + edi*4 + 0x26]
        mov    dword ptr [ecx + edx*4 + 0x26], eax
        mov    ax, word ptr [ecx + edi*2 + 0x4a]
        mov    word ptr [ecx + edx*2 + 0x4a], ax
loc_2_1739:
        lea    eax, [ecx + edi*4]
        xor    edx, edx
        mov    dword ptr [eax + 2], edx
        mov    dword ptr [eax + 0x26], edx
        mov    word ptr [ecx + edi*2 + 0x4a], dx
        mov    word ptr [ecx], si
loc_2_174C:
        pop    edi
        pop    esi
        ret    
sub_2_174F:
        push   ebx
        lea    eax, [ecx + 0xae]
        push   esi
        push   edi
        mov    di, word ptr [eax]
        movzx  esi, di
        cmp    esi, edx
        jle    loc_2_179A
        dec    di
        movzx  esi, di
        cmp    edx, esi
        je     loc_2_1787
        mov    bl, byte ptr [esi + ecx + 0xb0]
        mov    byte ptr [ecx + edx + 0xb0], bl
        mov    bl, byte ptr [esi + ecx + 0xb7]
        mov    byte ptr [ecx + edx + 0xb7], bl
loc_2_1787:
        mov    byte ptr [esi + ecx + 0xb0], 0
        mov    byte ptr [esi + ecx + 0xb7], 0
        mov    word ptr [eax], di
loc_2_179A:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_2_179E:
        push   ebx
        push   esi
        push   edi
        lea    edi, [ecx + 0xbe]
        mov    ax, word ptr [edi]
        movzx  esi, ax
        cmp    esi, edx
        jle    loc_2_17EC
        dec    ax
        movzx  esi, ax
        cmp    esi, edx
        je     loc_2_17D8
        mov    bl, byte ptr [ecx + esi + 0xc0]
        mov    byte ptr [ecx + edx + 0xc0], bl
        mov    bx, word ptr [ecx + esi*2 + 0xc7]
        mov    word ptr [ecx + edx*2 + 0xc7], bx
loc_2_17D8:
        xor    edx, edx
        mov    byte ptr [ecx + esi + 0xc0], dl
        mov    word ptr [ecx + esi*2 + 0xc7], dx
        mov    word ptr [edi], ax
loc_2_17EC:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_2_17F0:
        push   esi
        push   edi
        mov    si, word ptr [ecx + 0x5c]
        movzx  eax, si
        cmp    eax, edx
        jle    loc_2_1834
        dec    si
        movzx  edi, si
        cmp    edx, edi
        je     loc_2_1820
        mov    ax, word ptr [ecx + edi*2 + 0x5e]
        mov    word ptr [ecx + edx*2 + 0x5e], ax
        mov    ax, word ptr [ecx + edi*2 + 0x86]
        mov    word ptr [ecx + edx*2 + 0x86], ax
loc_2_1820:
        lea    eax, [ecx + edi*2]
        xor    edx, edx
        mov    word ptr [eax + 0x5e], dx
        mov    word ptr [eax + 0x86], dx
        mov    word ptr [ecx + 0x5c], si
loc_2_1834:
        pop    edi
        pop    esi
        ret    
sub_2_1837:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        mov    dword ptr [ebp - 0xc], ecx
        push   esi
        push   edi
        mov    edi, edx
loc_2_1845:
        mov    eax, dword ptr [ebp - 0xc]
        mov    esi, dword ptr [eax]
        movzx  ecx, byte ptr [esi + 4]
        movzx  eax, byte ptr [esi + 3]
        shl    ecx, 8
        add    ecx, eax
        movzx  eax, byte ptr [esi + 2]
        shl    ecx, 8
        add    ecx, eax
        shl    ecx, 8
        mov    dword ptr [ebp - 4], ecx
        movzx  eax, byte ptr [esi + 5]
        movzx  ecx, byte ptr [esi + 6]
        shl    ecx, 8
        xor    ebx, ebx
        add    ecx, eax
        mov    eax, dword ptr [ebp - 4]
        shl    ecx, 0xa
        cmp    word ptr [edi], bx
        lea    edx, [ecx + eax - 1]
        mov    dword ptr [ebp - 8], edx
        jbe    loc_2_18CD
loc_2_1887:
        mov    ecx, dword ptr [edi + ebx*4 + 2]
        mov    eax, dword ptr [edi + ebx*4 + 0x26]
        cmp    ecx, dword ptr [ebp - 4]
        lea    eax, [eax + ecx - 1]
        jb     loc_2_189D
        cmp    ecx, dword ptr [ebp - 8]
        jbe    loc_2_18BB
loc_2_189D:
        cmp    eax, dword ptr [ebp - 4]
        jb     loc_2_18A7
        cmp    eax, dword ptr [ebp - 8]
        jbe    loc_2_18BB
loc_2_18A7:
        cmp    ecx, dword ptr [ebp - 4]
        ja     loc_2_18B1
        cmp    eax, dword ptr [ebp - 4]
        jae    loc_2_18BB
loc_2_18B1:
        cmp    ecx, dword ptr [ebp - 8]
        ja     loc_2_18C5
        cmp    eax, dword ptr [ebp - 8]
        jb     loc_2_18C5
loc_2_18BB:
        mov    edx, ebx
        mov    ecx, edi
        dec    ebx
        call   sub_2_170A
loc_2_18C5:
        movzx  eax, word ptr [edi]
        inc    ebx
        cmp    eax, ebx
        jg     loc_2_1887
loc_2_18CD:
        lea    ecx, [esi + 7]
        mov    eax, dword ptr [ebp - 0xc]
        test   byte ptr [esi], 0x80
        mov    dword ptr [eax], ecx
        jne    loc_2_1845
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_18E5:
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        push   ebp
        mov    edi, edx
        lea    ebp, [edx + 0xae]
loc_2_18F3:
        xor    ebx, ebx
        cmp    word ptr [ebp], bx
        jbe    loc_2_191D
loc_2_18FB:
        mov    eax, dword ptr [esi]
        mov    al, byte ptr [eax]
        and    al, 0xf
        cmp    byte ptr [edi + ebx + 0xb0], al
        jne    loc_2_1914
        mov    edx, ebx
        mov    ecx, edi
        dec    ebx
        call   sub_2_174F
loc_2_1914:
        movzx  eax, word ptr [ebp]
        inc    ebx
        cmp    eax, ebx
        jg     loc_2_18FB
loc_2_191D:
        mov    eax, dword ptr [esi]
        test   byte ptr [eax], 0x80
        lea    ecx, [eax + 2]
        mov    dword ptr [esi], ecx
        jne    loc_2_18F3
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_2_192E:
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        push   ebp
        mov    edi, edx
        lea    ebp, [edx + 0xbe]
loc_2_193C:
        xor    ebx, ebx
        cmp    word ptr [ebp], bx
        jbe    loc_2_1966
loc_2_1944:
        mov    eax, dword ptr [esi]
        mov    al, byte ptr [eax]
        and    al, 7
        cmp    byte ptr [edi + ebx + 0xc0], al
        jne    loc_2_195D
        mov    edx, ebx
        mov    ecx, edi
        dec    ebx
        call   sub_2_179E
loc_2_195D:
        movzx  eax, word ptr [ebp]
        inc    ebx
        cmp    eax, ebx
        jg     loc_2_1944
loc_2_1966:
        mov    eax, dword ptr [esi]
        test   byte ptr [eax], 0x80
        lea    ecx, [eax + 2]
        mov    dword ptr [esi], ecx
        jne    loc_2_193C
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_2_1977:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        mov    dword ptr [ebp - 8], ecx
        push   esi
        push   edi
        mov    esi, edx
loc_2_1985:
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ecx]
        mov    di, word ptr [eax + 1]
        mov    al, byte ptr [eax]
        xor    ebx, ebx
        mov    byte ptr [ebp - 0xc], al
        xor    ax, ax
        mov    al, byte ptr [ebp - 0xc]
        and    ax, 0x1f
        add    ax, di
        mov    word ptr [ebp - 2], ax
        cmp    word ptr [esi + 0x5c], bx
        jbe    loc_2_19FD
loc_2_19AC:
        mov    ax, word ptr [esi + ebx*2 + 0x5e]
        mov    cx, word ptr [esi + ebx*2 + 0x86]
        add    cx, ax
        dec    cx
        cmp    ax, di
        jb     loc_2_19C9
        cmp    ax, word ptr [ebp - 2]
        jbe    loc_2_19EA
loc_2_19C9:
        cmp    cx, di
        jb     loc_2_19D4
        cmp    cx, word ptr [ebp - 2]
        jbe    loc_2_19EA
loc_2_19D4:
        cmp    ax, di
        ja     loc_2_19DE
        cmp    cx, di
        jae    loc_2_19EA
loc_2_19DE:
        cmp    ax, word ptr [ebp - 2]
        ja     loc_2_19F4
        cmp    cx, word ptr [ebp - 2]
        jb     loc_2_19F4
loc_2_19EA:
        mov    edx, ebx
        mov    ecx, esi
        dec    ebx
        call   sub_2_17F0
loc_2_19F4:
        movzx  eax, word ptr [esi + 0x5c]
        inc    ebx
        cmp    eax, ebx
        jg     loc_2_19AC
loc_2_19FD:
        mov    ecx, dword ptr [ebp - 8]
        add    dword ptr [ecx], 3
        test   byte ptr [ebp - 0xc], 0x80
        jne    loc_2_1985
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_1A14:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    dword ptr [ebp - 4], ecx
        cmp    word ptr [ecx], 0
        push   edi
        mov    esi, edx
        je     loc_2_1AB2
        add    dword ptr [ebp - 4], 2
        mov    ecx, dword ptr [ebp - 4]
        movzx  eax, byte ptr [ecx]
        inc    eax
        add    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp + 8]
        mov    edi, dword ptr [ebp - 4]
        inc    dword ptr [ebp - 4]
        test   eax, eax
        je     loc_2_1A5D
        test   byte ptr [edi], 0x80
        je     loc_2_1A62
        mov    cx, word ptr [eax + 8]
        mov    edx, dword ptr [ebp + 0xc]
        call   sub_2_12F3
        test   eax, eax
        je     loc_2_1A62
        jmp    loc_2_1AB2
loc_2_1A5D:
        test   byte ptr [edi], 0x80
        jne    loc_2_1AB2
loc_2_1A62:
        mov    al, byte ptr [edi]
        test   al, 0x40
        jne    loc_2_1AB2
        test   al, 1
        je     loc_2_1A76
        mov    ecx, dword ptr [ebp - 4]
        movzx  eax, byte ptr [ecx]
        inc    eax
        add    dword ptr [ebp - 4], eax
loc_2_1A76:
        test   byte ptr [edi], 2
        je     loc_2_1A85
        lea    ecx, [ebp - 4]
        mov    edx, esi
        call   sub_2_1837
loc_2_1A85:
        test   byte ptr [edi], 4
        je     loc_2_1A94
        lea    ecx, [ebp - 4]
        mov    edx, esi
        call   sub_2_18E5
loc_2_1A94:
        test   byte ptr [edi], 8
        je     loc_2_1AA3
        lea    ecx, [ebp - 4]
        mov    edx, esi
        call   sub_2_192E
loc_2_1AA3:
        test   byte ptr [edi], 0x10
        je     loc_2_1AB2
        lea    ecx, [ebp - 4]
        mov    edx, esi
        call   sub_2_1977
loc_2_1AB2:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
sub_2_1ABA:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    ax, word ptr [esi]
        sub    ax, 2
        mov    di, 0xc
        mov    word ptr [ebp - 2], ax
        cmp    ax, di
        jbe    loc_2_1B49
loc_2_1ADA:
        movzx  eax, di
        lea    ebx, [edi + 0xc]
        lea    ecx, [eax + esi]
        call   sub_2_144C
        mov    dword ptr [ebp - 0xc], eax
        movzx  eax, bx
        mov    dword ptr [ebp - 8], 0
        cmp    word ptr [eax + esi], 0
        je     loc_2_1B37
loc_2_1AFC:
        push   dword ptr [ebp - 8]
        movzx  eax, bx
        push   dword ptr [ebp - 0xc]
        lea    ecx, [eax + esi]
        mov    edx, dword ptr [ebp + 0xc]
        call   sub_2_1A14
        mov    ecx, dword ptr [ebp + 0xc]
        call   sub_2_16E5
        test   eax, eax
        je     loc_2_1B49
        movzx  eax, bx
        lea    ecx, [eax + esi]
        call   sub_2_1341
        add    bx, ax
        inc    dword ptr [ebp - 8]
        movzx  ecx, bx
        cmp    word ptr [ecx + esi], 0
        jne    loc_2_1AFC
loc_2_1B37:
        mov    edx, edi
        mov    ecx, esi
        call   sub_2_132B
        cmp    word ptr [ebp - 2], ax
        mov    di, ax
        ja     loc_2_1ADA
loc_2_1B49:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        mov    bx, dx
        push   edi
        movzx  eax, bx
        mov    esi, ecx
        lea    ecx, [eax + ecx]
        call   sub_2_1341
        mov    di, word ptr [esi]
        push   1
        sub    di, bx
        sub    di, ax
        mov    word ptr [ebp - 2], ax
        movzx  eax, di
        add    eax, 2
        push   eax
        call   sub_2_3384
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        je     loc_2_1BEF
        movzx  ecx, word ptr [ebp - 2]
        push   edi
        add    ecx, esi
        push   eax
        movzx  eax, bx
        add    ecx, eax
        push   ecx
        call   sub_2_0537
        add    esp, 0xc
        mov    ecx, dword ptr [ebp + 8]
        call   sub_2_1341
        push   eax
        movzx  eax, bx
        add    eax, esi
        push   eax
        push   dword ptr [ebp + 8]
        call   sub_2_0537
        add    esp, 0xc
        mov    ecx, dword ptr [ebp + 8]
        call   sub_2_1341
        add    bx, ax
        push   edi
        movzx  eax, bx
        add    eax, esi
        push   eax
        push   dword ptr [ebp - 8]
        call   sub_2_0537
        add    esp, 0xc
        mov    ecx, esi
        call   sub_2_13BB
        push   0
        push   dword ptr [ebp - 8]
        call   sub_2_337C
        add    esp, 8
loc_2_1BEF:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
sub_2_1BF8:
        xor    ax, ax
        sub    esp, 8
        push   ebx
        push   esi
        mov    bx, word ptr [edx]
        push   edi
        push   ebp
        xor    esi, esi
        test   bx, bx
        je     loc_2_1CA1
        lea    edi, [edx + 0x4a]
        add    edx, 2
        movzx  ebp, bx
loc_2_1C19:
        mov    bx, word ptr [edi]
        mov    esi, dword ptr [edx]
        shr    esi, 8
        add    edi, 2
        add    edx, 4
        mov    dword ptr [esp + 0x10], esi
        mov    esi, dword ptr [edx + 0x20]
        shr    esi, 0xa
        mov    dword ptr [esp + 0x14], esi
        movzx  esi, ax
        add    ax, 7
        add    esi, ecx
        xor    bl, byte ptr [esi]
        and    bl, 1
        xor    bl, byte ptr [esi]
        mov    byte ptr [esi], bl
        and    bl, 0xfd
        mov    byte ptr [esi], bl
        and    bl, 0xfb
        mov    byte ptr [esi], bl
        and    bl, 0xe7
        mov    byte ptr [esi], bl
        and    bl, 0xdf
        mov    byte ptr [esi], bl
        and    bl, 0xbf
        mov    byte ptr [esi], bl
        or     bl, 0x80
        mov    byte ptr [esi], bl
        mov    bl, byte ptr [esi + 1]
        and    bl, 0xfc
        mov    byte ptr [esi + 1], bl
        and    bl, 0xf3
        mov    byte ptr [esi + 1], bl
        and    bl, 0xf
        mov    byte ptr [esi + 1], bl
        mov    bx, word ptr [esp + 0x10]
        mov    byte ptr [esi + 2], bl
        mov    byte ptr [esi + 3], bh
        mov    ebx, dword ptr [esp + 0x10]
        shr    ebx, 0x10
        dec    ebp
        mov    byte ptr [esi + 4], bl
        mov    bx, word ptr [esp + 0x14]
        mov    byte ptr [esi + 5], bl
        mov    byte ptr [esi + 6], bh
        jne    loc_2_1C19
loc_2_1CA1:
        pop    ebp
        and    byte ptr [esi], 0x7f
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    8
sub_2_1CAE:
        xor    ax, ax
        sub    esp, 4
        push   ebx
        push   esi
        mov    bx, word ptr [edx + 0xae]
        push   edi
        push   ebp
        xor    esi, esi
        xor    edi, edi
        cmp    bx, ax
        je     loc_2_1D46
        movzx  ebp, bx
loc_2_1CCB:
        movzx  esi, ax
        add    esi, ecx
        mov    bl, byte ptr [edi + edx + 0xb0]
        xor    bl, byte ptr [esi]
        and    bl, 0xf
        xor    bl, byte ptr [esi]
        mov    byte ptr [esi], bl
        and    bl, 0xef
        mov    byte ptr [esp + 0x10], bl
        mov    byte ptr [esi], bl
        mov    bl, byte ptr [edi + edx + 0xb7]
        and    bl, 2
        shr    bl, 1
        shl    bl, 5
        xor    bl, byte ptr [esp + 0x10]
        and    bl, 0x20
        xor    bl, byte ptr [esp + 0x10]
        mov    byte ptr [esp + 0x10], bl
        mov    byte ptr [esi], bl
        cmp    dword ptr [0], 0              ; -> obj3:0x01B8
        jne    loc_2_1D1C
        test   bl, 0x20
        jne    loc_2_1D1C
        and    bl, 0xbf
        jmp    loc_2_1D34
loc_2_1D1C:
        mov    bl, byte ptr [edi + edx + 0xb7]
        and    bl, 1
        shl    bl, 6
        xor    bl, byte ptr [esp + 0x10]
        and    bl, 0x40
        xor    bl, byte ptr [esp + 0x10]
loc_2_1D34:
        add    ax, 2
        inc    edi
        mov    byte ptr [esi + 1], 0
        mov    byte ptr [esi], bl
        or     byte ptr [esi], 0x80
        cmp    ebp, edi
        jg     loc_2_1CCB
loc_2_1D46:
        pop    ebp
        and    byte ptr [esi], 0x7f
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    8
sub_2_1D53:
        xor    ax, ax
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        xor    edi, edi
        cmp    word ptr [edx + 0xbe], ax
        je     loc_2_1DCF
loc_2_1D6C:
        movzx  edi, ax
        add    ax, 2
        add    edi, ecx
        mov    bl, byte ptr [edx + esi + 0xc0]
        inc    esi
        xor    bl, byte ptr [edi]
        and    bl, 7
        xor    bl, byte ptr [edi]
        mov    byte ptr [edi], bl
        and    bl, 0xc7
        mov    byte ptr [edi], bl
        and    bl, 0xbf
        mov    byte ptr [edi], bl
        or     bl, 0x80
        mov    byte ptr [edi], bl
        mov    bl, byte ptr [edi + 1]
        and    bl, 0xfc
        mov    byte ptr [ebp - 4], bl
        mov    byte ptr [edi + 1], bl
        mov    bx, word ptr [edx + esi*2 + 0xc5]
        shl    bl, 2
        xor    bl, byte ptr [ebp - 4]
        and    bl, 0xc
        xor    bl, byte ptr [ebp - 4]
        mov    byte ptr [edi + 1], bl
        and    bl, 0xcf
        mov    byte ptr [edi + 1], bl
        and    bl, 0x3f
        mov    byte ptr [edi + 1], bl
        movzx  ebx, word ptr [edx + 0xbe]
        cmp    esi, ebx
        jl     loc_2_1D6C
loc_2_1DCF:
        and    byte ptr [edi], 0x7f
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
sub_2_1DDB:
        xor    ax, ax
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        xor    edi, edi
        cmp    word ptr [edx + 0x5c], ax
        mov    dword ptr [esp + 0x14], edi
        jbe    loc_2_1E82
        lea    esi, [edx + 0x86]
loc_2_1DFB:
        cmp    word ptr [esi], 0x140
        jbe    loc_2_1E07
        mov    word ptr [esi], 0x140
loc_2_1E07:
        cmp    word ptr [esi], 0
        jbe    loc_2_1E6D
loc_2_1E0D:
        movzx  edi, ax
        add    edi, ecx
        cmp    word ptr [esi], 0x20
        jbe    loc_2_1E1D
        or     byte ptr [edi], 0x1f
        jmp    loc_2_1E2B
loc_2_1E1D:
        mov    bx, word ptr [esi]
        dec    bl
        xor    bl, byte ptr [edi]
        and    bl, 0x1f
        xor    bl, byte ptr [edi]
        mov    byte ptr [edi], bl
loc_2_1E2B:
        mov    bp, word ptr [esi - 0x28]
        mov    bl, byte ptr [edi]
        add    ax, 3
        and    bl, 0xdf
        mov    word ptr [edi + 1], bp
        mov    byte ptr [edi], bl
        and    bl, 0xbf
        mov    byte ptr [edi], bl
        or     bl, 0x80
        mov    byte ptr [esp + 0x10], bl
        mov    byte ptr [edi], bl
        xor    bx, bx
        mov    bl, byte ptr [esp + 0x10]
        and    bx, 0x1f
        add    bp, bx
        inc    bp
        mov    word ptr [esi - 0x28], bp
        mov    bp, word ptr [esi]
        sub    bp, bx
        dec    bp
        mov    word ptr [esi], bp
        jne    loc_2_1E0D
loc_2_1E6D:
        add    esi, 2
        inc    dword ptr [esp + 0x14]
        movzx  ebx, word ptr [edx + 0x5c]
        cmp    ebx, dword ptr [esp + 0x14]
        jg     loc_2_1DFB
loc_2_1E82:
        pop    ebp
        and    byte ptr [edi], 0x7f
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    8
sub_2_1E8F:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    ebx, edx
        mov    esi, ecx
        xor    edi, edi
        mov    word ptr [ebp - 0xa], di
        mov    word ptr [ebp - 8], di
        mov    word ptr [ebp - 6], di
        call   sub_2_1341
        mov    word ptr [ebp - 2], ax
        mov    ecx, esi
        call   sub_2_1341
        mov    word ptr [ebp - 4], ax
        push   1
        movzx  eax, word ptr [ebp - 2]
        push   eax
        call   sub_2_3384
        add    esp, 8
        mov    dword ptr [ebp - 0x10], eax
        cmp    eax, edi
        jne    loc_2_1EDD
        mov    ax, word ptr [ebp - 4]
        jmp    loc_2_20B2
loc_2_1EDD:
        push   dword ptr [ebp - 2]
        push   dword ptr [ebp - 0x10]
        push   esi
        call   sub_2_0537
        mov    byte ptr [esi + 2], 1
        mov    byte ptr [esi + 3], 0
        add    esp, 0xc
        cmp    word ptr [ebp - 4], 5
        jae    loc_2_1EFF
        mov    byte ptr [esi + 4], 0
loc_2_1EFF:
        cmp    word ptr [ebx], 1
        mov    al, byte ptr [esi + 4]
        sbb    cl, cl
        mov    byte ptr [ebp - 1], al
        lea    ecx, [ecx*2 + 2]
        xor    cl, al
        and    cl, 2
        xor    cl, al
        cmp    word ptr [ebx + 0xae], 1
        sbb    al, al
        mov    byte ptr [esi + 4], cl
        lea    eax, [eax*4 + 4]
        xor    al, cl
        and    al, 4
        xor    al, cl
        cmp    word ptr [ebx + 0xbe], 1
        sbb    cl, cl
        mov    byte ptr [esi + 4], al
        lea    ecx, [ecx*8 + 8]
        xor    cl, al
        and    cl, 8
        xor    cl, al
        cmp    word ptr [ebx + 0x5c], 1
        sbb    al, al
        mov    byte ptr [esi + 4], cl
        mov    di, 5
        inc    al
        shl    al, 4
        xor    al, cl
        and    al, 0x10
        xor    al, cl
        mov    byte ptr [esi + 4], al
        test   al, 1
        je     loc_2_1F77
        movzx  di, byte ptr [esi + 5]
        add    di, 6
loc_2_1F77:
        test   byte ptr [ebp - 1], 2
        mov    ax, di
        je     loc_2_1F90
loc_2_1F80:
        movzx  edx, ax
        add    ax, 7
        mov    ecx, dword ptr [ebp - 0x10]
        test   byte ptr [edx + ecx], 0x80
        jne    loc_2_1F80
loc_2_1F90:
        test   byte ptr [ebp - 1], 4
        je     loc_2_1FAA
        mov    word ptr [ebp - 0xa], ax
loc_2_1F9A:
        movzx  edx, ax
        add    ax, 2
        mov    ecx, dword ptr [ebp - 0x10]
        test   byte ptr [edx + ecx], 0x80
        jne    loc_2_1F9A
loc_2_1FAA:
        test   byte ptr [ebp - 1], 8
        je     loc_2_1FC4
        mov    word ptr [ebp - 8], ax
loc_2_1FB4:
        movzx  edx, ax
        add    ax, 2
        mov    ecx, dword ptr [ebp - 0x10]
        test   byte ptr [edx + ecx], 0x80
        jne    loc_2_1FB4
loc_2_1FC4:
        test   byte ptr [ebp - 1], 0x10
        je     loc_2_1FCE
        mov    word ptr [ebp - 6], ax
loc_2_1FCE:
        mov    al, byte ptr [esi + 4]
        mov    edx, ebx
        push   dword ptr [ebp - 1]
        push   eax
        movzx  eax, di
        lea    ecx, [eax + esi]
        call   sub_2_1BF8
        add    di, ax
        cmp    word ptr [ebp - 0xa], 0
        je     loc_2_200B
        mov    ax, word ptr [ebp - 4]
        sub    ax, word ptr [ebp - 0xa]
        push   eax
        movzx  eax, di
        add    eax, esi
        push   eax
        movzx  eax, word ptr [ebp - 0xa]
        add    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_2_0537
        add    esp, 0xc
loc_2_200B:
        mov    al, byte ptr [esi + 4]
        mov    edx, ebx
        push   dword ptr [ebp - 1]
        push   eax
        movzx  eax, di
        lea    ecx, [eax + esi]
        call   sub_2_1CAE
        add    di, ax
        cmp    word ptr [ebp - 8], 0
        je     loc_2_2048
        mov    ax, word ptr [ebp - 4]
        sub    ax, word ptr [ebp - 8]
        push   eax
        movzx  eax, di
        add    eax, esi
        push   eax
        movzx  eax, word ptr [ebp - 8]
        add    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_2_0537
        add    esp, 0xc
loc_2_2048:
        mov    al, byte ptr [esi + 4]
        mov    edx, ebx
        push   dword ptr [ebp - 1]
        push   eax
        movzx  eax, di
        lea    ecx, [eax + esi]
        call   sub_2_1D53
        add    di, ax
        cmp    word ptr [ebp - 6], 0
        je     loc_2_2085
        mov    ax, word ptr [ebp - 4]
        sub    ax, word ptr [ebp - 6]
        push   eax
        movzx  eax, di
        add    eax, esi
        push   eax
        movzx  eax, word ptr [ebp - 6]
        add    eax, dword ptr [ebp - 0x10]
        push   eax
        call   sub_2_0537
        add    esp, 0xc
loc_2_2085:
        mov    al, byte ptr [esi + 4]
        mov    edx, ebx
        push   dword ptr [ebp - 1]
        push   eax
        movzx  eax, di
        lea    ecx, [eax + esi]
        call   sub_2_1DDB
        add    di, ax
        push   0
        lea    ecx, [edi - 2]
        push   dword ptr [ebp - 0x10]
        mov    word ptr [esi], cx
        call   sub_2_337C
        mov    ax, di
        add    esp, 8
loc_2_20B2:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_20B9:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        mov    bx, dx
        push   edi
        mov    edi, ecx
        movzx  ecx, bx
        add    ecx, edi
        call   sub_2_1341
        mov    si, ax
        push   1
        mov    ax, word ptr [edi]
        sub    ax, bx
        sub    ax, si
        mov    word ptr [ebp - 2], ax
        movzx  eax, ax
        add    eax, 2
        push   eax
        call   sub_2_3384
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        test   eax, eax
        mov    ax, si
        je     loc_2_2156
        push   dword ptr [ebp - 2]
        movzx  eax, si
        movzx  ecx, bx
        push   dword ptr [ebp - 8]
        add    eax, ecx
        add    eax, edi
        push   eax
        call   sub_2_0537
        movzx  ecx, bx
        add    esp, 0xc
        mov    edx, dword ptr [ebp + 8]
        lea    esi, [edi + ecx]
        mov    ecx, esi
        call   sub_2_1E8F
        add    bx, ax
        push   dword ptr [ebp - 2]
        mov    si, ax
        movzx  eax, bx
        add    eax, edi
        push   eax
        push   dword ptr [ebp - 8]
        call   sub_2_0537
        add    esp, 0xc
        mov    ecx, edi
        call   sub_2_13BB
        push   0
        push   dword ptr [ebp - 8]
        call   sub_2_337C
        mov    ax, si
        add    esp, 8
loc_2_2156:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
sub_2_215F:
        sub    esp, 0xe0
        push   ebx
        push   esi
        mov    bx, dx
        push   edi
        push   ebp
        mov    esi, ecx
        lea    ecx, [esp + 0x14]
        push   2
        push   dword ptr [esp + 0xfc]
        push   ecx
        call   sub_2_32FC
        add    esp, 0xc
        test   eax, eax
        je     loc_2_2191
        mov    ax, 1
        jmp    loc_2_22C6
loc_2_2191:
        lea    edx, [esp + 0x18]
        push   1
        mov    ecx, dword ptr [esp + 0x18]
        call   sub_2_0968
        lea    eax, [esp + 0x18]
        push   eax
        push   esi
        call   sub_2_1ABA
        lea    ecx, [esp + 0x20]
        add    esp, 8
        call   sub_2_16E5
        test   eax, eax
        jne    loc_2_21C4
        mov    ax, 1
        jmp    loc_2_22C6
loc_2_21C4:
        mov    edx, ebx
        mov    ecx, esi
        call   sub_2_159F
        mov    di, ax
        xor    edx, edx
        test   di, di
        jne    loc_2_2281
        mov    di, word ptr [esi]
        inc    byte ptr [esi + 8]
        sub    di, 2
        movzx  eax, di
        mov    byte ptr [eax + esi + 2], bl
        mov    byte ptr [eax + esi + 3], dl
        lea    ecx, [eax + esi]
        lea    ebx, [edi + 4]
        mov    word ptr [ecx], dx
        mov    eax, dword ptr [esp + 0xf4]
        movzx  ebp, bx
        add    bx, 4
        mov    dword ptr [ebp + esi], eax
        movzx  eax, bx
        add    eax, esi
        mov    dword ptr [esp + 0x10], eax
        mov    al, byte ptr [eax]
        and    al, 0xf0
        mov    ebp, dword ptr [esp + 0x10]
        mov    byte ptr [ebp], al
        and    al, 0xcf
        mov    byte ptr [ebp], al
        or     al, 0x40
        mov    byte ptr [ebp], al
        and    al, 0x7f
        mov    byte ptr [ebp], al
        inc    ebp
        mov    al, byte ptr [ebp]
        and    al, 0xfe
        mov    byte ptr [ebp], al
        and    al, 0xfd
        mov    byte ptr [ebp], al
        or     al, 4
        mov    byte ptr [ebp], al
        and    al, 0xc7
        add    bx, 2
        mov    byte ptr [ebp], al
        and    al, 0xbf
        mov    byte ptr [ebp], al
        and    al, 0x7f
        mov    byte ptr [ebp], al
        movzx  ebp, bx
        add    bx, 2
        movzx  eax, bx
        mov    word ptr [ebp + esi], dx
        add    eax, esi
        mov    word ptr [eax], dx
        mov    word ptr [eax + 2], dx
        mov    ax, bx
        sub    ax, di
        add    ax, 6
        mov    word ptr [ecx], ax
        lea    ecx, [ebx + 8]
        mov    word ptr [esi], cx
        jmp    loc_2_229D
loc_2_2281:
        movzx  eax, di
        mov    bx, word ptr [eax + esi]
        xor    ecx, ecx
        add    bx, di
        sub    bx, 4
        movzx  eax, bx
        add    eax, esi
        mov    word ptr [eax], cx
        mov    word ptr [eax + 2], cx
loc_2_229D:
        lea    eax, [esp + 0x18]
        mov    edx, ebx
        push   eax
        mov    ecx, esi
        call   sub_2_20B9
        add    bx, ax
        mov    ecx, esi
        add    bx, 2
        movzx  eax, di
        sub    bx, di
        mov    word ptr [eax + esi], bx
        call   sub_2_13BB
        xor    ax, ax
loc_2_22C6:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xe0
        ret    8
sub_2_22D3:
        mov    ax, 0xc
        push   ebx
        push   esi
        mov    ebx, ecx
        xor    si, si
        push   edi
        cmp    word ptr [ebx], dx
        ja     loc_2_2361
        cmp    dword ptr [ebx + 2], 0x47464341
        jne    loc_2_2361
        cmp    byte ptr [ebx + 7], 2
        jb     loc_2_2361
        xor    edi, edi
        cmp    byte ptr [ebx + 8], 0
        jbe    loc_2_234D
loc_2_22FB:
        mov    edx, esi
        mov    ecx, ebx
        call   sub_2_132B
        mov    si, ax
        movzx  ecx, word ptr [ebx]
        movzx  eax, si
        add    eax, 0xc
        cmp    eax, ecx
        jg     loc_2_2361
        mov    edx, esi
        mov    ecx, ebx
        call   sub_2_132B
        lea    edx, [esi + 0xc]
loc_2_2320:
        movzx  ecx, dx
        mov    cx, word ptr [ecx + ebx]
        test   cx, cx
        je     loc_2_2337
        lea    edx, [ecx + edx + 2]
        cmp    dx, ax
        jb     loc_2_2320
        jmp    loc_2_2361
loc_2_2337:
        movzx  ecx, dx
        movzx  edx, ax
        sub    ecx, edx
        cmp    ecx, -4
        jne    loc_2_2361
        movzx  ecx, byte ptr [ebx + 8]
        inc    edi
        cmp    ecx, edi
        ja     loc_2_22FB
loc_2_234D:
        movzx  ecx, word ptr [ebx]
        movzx  eax, ax
        sub    ecx, eax
        sub    ecx, 2
        cmp    ecx, 1
        sbb    eax, eax
        neg    eax
        jmp    loc_2_2363
loc_2_2361:
        xor    eax, eax
loc_2_2363:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_2_2367:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        sub    esp, 0xc
        push   esi
        push   edi
        push   eax
        push   eax
        push   eax
        call   sub_2_33B4
        add    esp, 0xc
        test   eax, eax
        je     loc_2_2384
        xor    eax, eax
        jmp    loc_2_23F7
loc_2_2384:
        push   0
        mov    eax, dword ptr [0]            ; -> obj3:0x005C
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        call   sub_2_33AC
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_23F2
        xor    esi, esi
loc_2_239E:
        push   esi
        lea    ecx, [ebp - 8]
        push   dword ptr [ebp - 4]
        push   ecx
        call   sub_2_33A4
        add    esp, 0xc
        mov    edi, eax
        lea    edx, [ebp - 0xc]
        push   esi
        push   dword ptr [0]                 ; -> obj3:0x005C
        push   dword ptr [ebp - 4]
        push   edx
        call   sub_2_3364
        add    esp, 0x10
        test   byte ptr [ebp - 0xa], 0x20
        je     loc_2_23E8
        push   esi
        push   dword ptr [ebp - 4]
        call   sub_2_339C
        add    esp, 8
        test   eax, eax
        jne    loc_2_23FD
        push   esi
        push   dword ptr [ebp - 4]
        call   sub_2_3304
        add    esp, 8
loc_2_23E8:
        mov    eax, dword ptr [ebp - 8]
        test   edi, edi
        mov    dword ptr [ebp - 4], eax
        je     loc_2_239E
loc_2_23F2:
        mov    eax, 1
loc_2_23F7:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_23FD:
        push   0
        push   dword ptr [ebp - 4]
        call   sub_2_3394
        add    esp, 8
        mov    eax, dword ptr [0]            ; -> obj3:0x005C
        push   0
        push   eax
        call   sub_2_338C
        add    esp, 8
        xor    eax, eax
        jmp    loc_2_23F7
sub_2_241E:
        push   esi
        push   edi
        call   sub_2_0293
        sub    ax, 0x87
        cmp    ax, 1
        mov    eax, dword ptr [esp + 0xc]
        sbb    esi, esi
        inc    esi
        cmp    eax, 1
        je     loc_2_2460
        cmp    eax, 2
        je     loc_2_24B7
        cmp    eax, 3
        je     loc_2_2535
        cmp    eax, 4
        je     loc_2_2500
        cmp    eax, 0xffff
        je     loc_2_2535
        jmp    loc_2_2545
loc_2_2460:
        test   esi, esi
        jne    loc_2_247F
        call   sub_2_2578
        push   1
        push   0
        push   0
        call   sub_2_33B4
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_249E
        xor    ecx, ecx
        jmp    loc_2_24A3
loc_2_247F:
        cmp    dword ptr [0], 0              ; -> obj3:0x01C0
        je     loc_2_248C
        xor    ecx, ecx
        jmp    loc_2_24A3
loc_2_248C:
        call   sub_2_2367
        test   eax, eax
        je     loc_2_249E
        call   sub_2_2578
        xor    ecx, ecx
        jmp    loc_2_24A3
loc_2_249E:
        mov    ecx, 1
loc_2_24A3:
        call   sub_2_0259
        mov    dword ptr [0], 0              ; -> obj3:0x01C4
        jmp    loc_2_2545
loc_2_24B7:
        xor    ecx, ecx
        test   esi, esi
        mov    dword ptr [0], ecx            ; -> obj3:0x01C0
        jne    loc_2_24CE
        mov    eax, dword ptr [0]            ; -> obj3:0x000C
        mov    dword ptr [eax + 3], ecx
        mov    dword ptr [eax + 7], ecx
loc_2_24CE:
        xor    edi, edi
        call   sub_2_2578
        mov    eax, dword ptr [0]            ; -> obj3:0x01C8
        test   eax, eax
        je     loc_2_24EA
        push   eax
        call   sub_2_33BC
        mov    dword ptr [0], edi            ; -> obj3:0x01C8
loc_2_24EA:
        push   esi
        mov    eax, dword ptr [0]            ; -> obj3:0x000C
        push   dword ptr [eax + 7]
        push   dword ptr [eax + 3]
        call   sub_2_33CC
        add    esp, 0xc
        jmp    loc_2_252E
loc_2_2500:
        xor    esi, esi
        mov    dword ptr [0], esi            ; -> obj3:0x01C0
        call   sub_2_2578
        mov    eax, dword ptr [0]            ; -> obj3:0x01C8
        cmp    eax, esi
        je     loc_2_2522
        push   eax
        call   sub_2_33BC
        mov    dword ptr [0], esi            ; -> obj3:0x01C8
loc_2_2522:
        push   0
        push   0
        call   sub_2_3394
        add    esp, 8
loc_2_252E:
        call   sub_2_25AC
        jmp    loc_2_2545
loc_2_2535:
        push   0
        mov    eax, dword ptr [0]            ; -> obj3:0x005C
        push   eax
        call   sub_2_338C
        add    esp, 8
loc_2_2545:
        pop    edi
        pop    esi
        ret    
loc_2_2548:
        call   sub_2_00F3
        mov    eax, dword ptr [0]            ; -> obj3:0x000C
        movzx  ecx, word ptr [eax + 1]
        push   ecx
        call   sub_2_241E
        add    esp, 4
        ret    
loc_2_2560:
        xor    eax, eax
        push   eax
        mov    dword ptr [0], eax            ; -> obj3:0x01C8
        push   4
        push   0                             ; -> sub_2_241E
        call   sub_1_0100
loc_2_2574:
        add    esp, 0xc
        ret    
sub_2_2578:
        cmp    dword ptr [0], 0              ; -> obj3:0x01BC
        jne    loc_2_25AB
        mov    dword ptr [0], 1              ; -> obj3:0x01BC
        push   0
        call   sub_2_33F0
        add    esp, 4
        push   0
        push   0xea60
        push   0                             ; -> loc_2_2560
        call   sub_2_33D4
        mov    dword ptr [0], eax            ; -> obj3:0x01C8
loc_2_25AB:
        ret    
sub_2_25AC:
        push   esi
        xor    esi, esi
        cmp    dword ptr [0], esi            ; -> obj3:0x01BC
        je     loc_2_25DD
        mov    eax, dword ptr [0]            ; -> obj3:0x01C8
        cmp    eax, esi
        je     loc_2_25CC
        push   eax
        call   sub_2_33BC
        mov    dword ptr [0], esi            ; -> obj3:0x01C8
loc_2_25CC:
        xor    esi, esi
        push   esi
        call   sub_2_33F8
        add    esp, 4
        mov    dword ptr [0], esi            ; -> obj3:0x01BC
loc_2_25DD:
        pop    esi
        ret    
loc_2_25DF:
        push   esi
        mov    eax, dword ptr [0]            ; -> obj1:0x0064
        test   byte ptr [eax], 1
        je     loc_2_2629
        xor    esi, esi
        call   sub_2_00F3
        mov    eax, dword ptr [0]            ; -> obj3:0x000C
        mov    ax, word ptr [eax + 1]
        cmp    ax, 1
        jne    loc_2_2619
        cmp    dword ptr [0], esi            ; -> obj3:0x01C4
        je     loc_2_260F
        mov    esi, 1
        jmp    loc_2_2619
loc_2_260F:
        mov    dword ptr [0], 1              ; -> obj3:0x01C4
loc_2_2619:
        test   esi, esi
        jne    loc_2_2629
        movzx  eax, ax
        push   eax
        call   sub_2_241E
        add    esp, 4
loc_2_2629:
        push   0
        push   0xfa
        push   0                             ; -> loc_1_00D3
        call   sub_2_33D4
        pop    esi
        ret    
sub_2_263C:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        mov    dword ptr [ebp - 8], edx
        mov    dword ptr [ebp - 0xc], ecx
        cmp    dword ptr [ebp + 8], 1
        sbb    eax, eax
        dec    dword ptr [ebp - 8]
        and    al, 0xf8
        add    eax, 0x9b
        mov    dword ptr [ebp - 0x10], eax
        push   1
        push   0
        push   dword ptr [ebp - 0x10]
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 0xc]
        VMMCall  VMM._BuildDescriptorDWORDs      ; cd207a000100
        add    esp, 0x14
        push   0
        push   eax
        push   edx
        VMMCall  VMM._Allocate_GDT_Selector      ; cd2076000100
        add    esp, 0xc
        mov    word ptr [ebp - 2], 0
        cmp    edx, 0
        je     loc_2_268D
        mov    word ptr [ebp - 2], ax
loc_2_268D:
        mov    ax, word ptr [ebp - 2]
        mov    esp, ebp
        pop    ebp
        ret    4
sub_2_2697:
        push   ebp
        dec    dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 8
        movzx  eax, cx
        mov    dword ptr [ebp - 8], edx
        mov    dword ptr [ebp - 4], eax
        push   1
        push   0
        push   0x93
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp - 8]
        VMMCall  VMM._BuildDescriptorDWORDs      ; cd207a000100
        add    esp, 0x14
        push   0
        push   eax
        push   edx
        push   0
        push   dword ptr [ebp - 4]
        VMMCall  VMM._SetDescriptor              ; cd207c000100
        add    esp, 0x14
        mov    esp, ebp
        pop    ebp
        ret    4
sub_2_26DA:
        movzx  eax, cx
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        test   eax, eax
        mov    dword ptr [ebp - 4], eax
        je     loc_2_26F8
        push   0
        push   dword ptr [ebp - 4]
        VMMCall  VMM._Free_GDT_Selector          ; cd2077000100
        add    esp, 8
loc_2_26F8:
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_26FC:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        cmp    dword ptr [0], 0              ; -> obj3:0x01D0
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        jne    loc_2_271F
        or     byte ptr [esi + 0x2c], 1
        mov    word ptr [esi + 0x1c], 0x82
        jmp    loc_2_27A0
loc_2_271F:
        lea    edi, [esi + 0x1c]
        movzx  eax, word ptr [edi]
        test   eax, eax
        je     loc_2_2741
        cmp    eax, 0x100
        je     loc_2_2748
        cmp    eax, 0x200
        je     loc_2_274F
        cmp    eax, 0x300
        je     loc_2_2756
        stc    
        jmp    loc_2_277C
loc_2_2741:
        call   sub_2_27B1
        jmp    loc_2_2779
loc_2_2748:
        call   sub_2_2815
        jmp    loc_2_2779
loc_2_274F:
        call   sub_2_27D3
        jmp    loc_2_2779
loc_2_2756:
        movzx  ecx, word ptr [esi + 0x38]
        movzx  eax, word ptr [esi]
        shl    ecx, 0x10
        add    ecx, eax
        movzx  eax, word ptr [esi + 0x14]
        shl    eax, 0x10
        push   ecx
        movzx  ecx, word ptr [esi + 0x10]
        add    eax, ecx
        push   eax
        call   sub_2_288E
        add    esp, 8
loc_2_2779:
        mov    word ptr [edi], ax
loc_2_277C:
        jb     loc_2_2787
        mov    dword ptr [ebp - 4], 0
        jmp    loc_2_278E
loc_2_2787:
        mov    dword ptr [ebp - 4], 1
loc_2_278E:
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_279A
        or     byte ptr [esi + 0x2c], 1
        jmp    loc_2_27A0
loc_2_279A:
        and    word ptr [esi + 0x2c], 0xfffe
loc_2_27A0:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_27A6:
        clc    
        mov    ax, 0x400
        ret    
BIOS_Service_00:
        jmp    loc_2_27A6
sub_2_27B1:
        jmp    loc_2_27A6
loc_2_27B6:
        call   sub_2_0293
        test   ax, ax
        jne    loc_2_27CC
        mov    eax, dword ptr [0]            ; -> obj3:0x000C
        mov    ax, word ptr [eax + 0xb]
        clc    
        jmp    loc_2_27CD
loc_2_27CC:
        stc    
loc_2_27CD:
        ret    
BIOS_Service_02:
        jmp    loc_2_27B6
sub_2_27D3:
        jmp    loc_2_27B6
loc_2_27D8:
        call   sub_2_2367
        test   eax, eax
        je     loc_2_27FA
        mov    dword ptr [0], 1              ; -> obj3:0x01C0
        call   sub_2_2578
        mov    ecx, 0x40
        jmp    sub_2_0259
loc_2_27FA:
        ret    
loc_2_27FB:
        push   0
        push   0
        push   0                             ; -> loc_2_27D8
        call   sub_2_3400
        xor    ax, ax
        add    esp, 0xc
        ret    
BIOS_Service_01:
        jmp    loc_2_27FB
sub_2_2815:
        jmp    loc_2_27FB
sub_2_281A:
        mov    edx, dword ptr [esp + 8]
        mov    ecx, dword ptr [esp + 4]
        call   sub_2_02D5
        test   ax, ax
        jne    loc_2_282F
        clc    
        jmp    loc_2_2830
loc_2_282F:
        stc    
loc_2_2830:
        ret    
BIOS_Service_03:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        movzx  edx, word ptr [eax]
        mov    ecx, dword ptr [esp + 0xc]
        mov    ebp, esp
        push   esi
        xor    esi, esi
        push   esi
        call   sub_2_263C
        mov    word ptr [ebp + 0xe], ax
        push   esi
        mov    word ptr [ebp + 0xc], si
        mov    edx, 2
        mov    ecx, dword ptr [ebp + 8]
        call   sub_2_263C
        mov    word ptr [ebp + 0xa], ax
        push   dword ptr [ebp + 0xc]
        mov    word ptr [ebp + 8], si
        mov    esi, dword ptr [ebp + 8]
        push   esi
        call   sub_2_281A
        mov    si, ax
        add    esp, 8
        mov    ecx, dword ptr [ebp + 0xa]
        call   sub_2_26DA
        mov    ecx, dword ptr [ebp + 0xe]
        call   sub_2_26DA
        mov    ax, si
        pop    esi
        pop    ebp
        ret    
sub_2_288E:
        push   ebp
        mov    ebp, esp
        push   ebp
        mov    ebp, dword ptr [0]            ; -> obj3:0x01CC
        mov    ax, 0x1410
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        pop    ebp
        mov    dword ptr [ebp + 8], eax
        push   ebp
        mov    ebp, dword ptr [0]            ; -> obj3:0x01CC
        mov    ax, 0x3800
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        pop    ebp
        mov    dword ptr [ebp + 0xc], eax
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   BIOS_Service_03
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_28CA:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        VxDCall  CONFIGMG._CONFIGMG_Get_Version  ; cd2000003300
        mov    word ptr [ebp - 2], ax
        cmp    word ptr [ebp - 2], 0x400
        jae    loc_2_28EA
        mov    eax, 0x13
        jmp    loc_2_295E
loc_2_28EA:
        cmp    dword ptr [ebp + 0xc], 1
        je     loc_2_28F7
        mov    eax, 0x13
        jmp    loc_2_295E
loc_2_28F7:
        call   sub_2_344C
        cmp    eax, 0x20000000
        jb     loc_2_290A
        mov    eax, 0x22
        jmp    loc_2_295E
loc_2_290A:
        cmp    dword ptr [0], 0              ; -> loc_4_0008
        jne    loc_2_291A
        mov    eax, 0x21
        jmp    loc_2_295E
loc_2_291A:
        mov    eax, dword ptr [0]            ; -> obj3:0x005C
        test   eax, eax
        je     loc_2_2935
        push   0
        push   eax
        call   sub_2_338C
        add    esp, 8
        mov    eax, 0x13
        jmp    loc_2_295E
loc_2_2935:
        mov    esi, dword ptr [ebp + 8]
        mov    dword ptr [0], esi            ; -> obj3:0x005C
        call   sub_4_0232
loc_2_2943:
        test   eax, eax
        jne    loc_2_295E
        xor    eax, eax
        push   eax
        push   eax
        push   eax
        push   esi
        call   sub_2_3444
        add    esp, 0x10
        mov    ecx, esi
        call   sub_2_12E2
        xor    eax, eax
loc_2_295E:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
sub_2_2965:
        cmp    dword ptr [0], 0              ; -> obj3:0x01D0
        push   esi
        je     loc_2_29C8
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 0xc]
        test   eax, eax
        je     loc_2_2992
        cmp    eax, 0x100
        je     loc_2_2995
        cmp    eax, 0x200
        je     loc_2_299C
        cmp    eax, 0x300
        je     loc_2_29B2
        clc    
        jmp    loc_2_29A9
loc_2_2992:
        clc    
        jmp    loc_2_29A9
loc_2_2995:
        call   BIOS_Service_01
        jmp    loc_2_29A1
loc_2_299C:
        call   BIOS_Service_02
loc_2_29A1:
        mov    ecx, dword ptr [esi + 0x10]
        movzx  eax, ax
        mov    dword ptr [ecx], eax
loc_2_29A9:
        mov    eax, 0
        adc    eax, eax
        jmp    loc_2_29CB
loc_2_29B2:
        mov    esi, dword ptr [esi + 0x10]
        push   dword ptr [esi + 8]
        push   dword ptr [esi + 4]
        call   BIOS_Service_03
        movzx  ecx, ax
        add    esp, 8
        mov    dword ptr [esi], ecx
loc_2_29C8:
        clc    
        xor    eax, eax
loc_2_29CB:
        pop    esi
        ret    4
sub_2_29CF:
        push   ebx
        push   esi
        push   edi
        mov    ebx, edx
        mov    esi, ecx
        mov    edi, ebx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        dec    ecx
        push   ecx
        mov    ecx, dword ptr [esi]
        call   sub_2_05AB
        cmp    byte ptr [eax], 0
        mov    edx, eax
        mov    dword ptr [esi], eax
        mov    eax, 0
        je     loc_2_2A12
        mov    edi, ebx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        lea    eax, [ecx + edx - 1]
        mov    dword ptr [esi], eax
        mov    eax, 1
loc_2_2A12:
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_2_2A16:
        sub    esp, 4
        mov    edx, 0                        ; -> obj3:0x031C
        mov    dword ptr [esp], ecx
        lea    ecx, [esp]
        call   sub_2_29CF
        test   eax, eax
        mov    eax, 0
        je     loc_2_2A42
        push   2
        push   dword ptr [esp + 4]
        call   sub_2_04ED
        add    esp, 8
loc_2_2A42:
        add    esp, 4
        ret    
sub_2_2A46:
        push   ebp
        mov    edx, 0                        ; -> obj3:0x0324
        mov    ebp, esp
        sub    esp, 4
        push   esi
        mov    dword ptr [ebp - 4], ecx
        lea    ecx, [ebp - 4]
        call   sub_2_29CF
        test   eax, eax
        jne    loc_2_2A65
        xor    eax, eax
        jmp    loc_2_2AAF
loc_2_2A65:
        mov    edx, 0                        ; -> obj3:0x032C
        lea    ecx, [ebp - 4]
        call   sub_2_29CF
        test   eax, eax
        jne    loc_2_2A7A
        xor    eax, eax
        jmp    loc_2_2AAF
loc_2_2A7A:
        push   2
        push   dword ptr [ebp - 4]
        call   sub_2_04ED
        add    esp, 8
        mov    esi, eax
        mov    edx, 0                        ; -> obj3:0x0334
        lea    ecx, [ebp - 4]
        call   sub_2_29CF
        test   eax, eax
        mov    eax, 0
        je     loc_2_2AAF
        push   2
        push   dword ptr [ebp - 4]
        call   sub_2_04ED
        add    esp, 8
        lea    eax, [eax + esi*8]
loc_2_2AAF:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
        db  083h, 0ECh, 004h, 0BAh
        dd  obj3:0x033C              ; fixup
        db  089h, 04Ch, 024h, 000h, 08Dh, 04Ch, 024h, 000h, 0E8h, 006h, 0FFh, 0FFh, 0FFh, 085h, 0C0h, 0B8h
        db  000h, 000h, 000h, 000h, 074h, 00Ch, 0FFh, 074h, 024h, 000h, 0E8h, 05Fh, 0D9h, 0FFh, 0FFh, 083h
        db  0C4h, 004h, 083h, 0C4h, 004h, 0C3h
sub_2_2AE2:
        sub    esp, 4
        mov    edx, 0                        ; -> obj3:0x0340
        mov    dword ptr [esp], ecx
        lea    ecx, [esp]
        call   sub_2_29CF
        test   eax, eax
        mov    eax, 0
        je     loc_2_2B0C
        push   dword ptr [esp]
        call   sub_2_043A
        add    esp, 4
loc_2_2B0C:
        add    esp, 4
        ret    
sub_2_2B10:
        push   ebp
        mov    edx, 0                        ; -> obj3:0x0344
        mov    ebp, esp
        sub    esp, 4
        mov    dword ptr [ebp - 4], ecx
        lea    ecx, [ebp - 4]
        call   sub_2_29CF
        test   eax, eax
        jne    loc_2_2B2E
        xor    eax, eax
        jmp    loc_2_2B4E
loc_2_2B2E:
        mov    edx, 0                        ; -> obj3:0x0348
        lea    ecx, [ebp - 4]
        call   sub_2_29CF
        test   eax, eax
        mov    eax, 0
        je     loc_2_2B4E
        push   8
        push   dword ptr [ebp - 4]
        call   sub_2_04ED
loc_2_2B4E:
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_2B52:
        sub    esp, 4
        mov    edx, 0                        ; -> obj3:0x034C
        mov    dword ptr [esp], ecx
        lea    ecx, [esp]
        call   sub_2_29CF
        test   eax, eax
        mov    al, 0
        je     loc_2_2B7B
        push   4
        push   dword ptr [esp + 4]
        call   sub_2_04ED
        add    esp, 8
loc_2_2B7B:
        add    esp, 4
        ret    
loc_2_2B7F:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x1e8
        push   ebx
        lea    eax, [ebp - 4]
        push   esi
        push   edi
        push   4
        mov    esi, dword ptr [ebp + 8]
        push   esi
        push   eax
        call   sub_2_32FC
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_2CC6
        xor    ebx, ebx
        lea    eax, [ebp - 0x10]
        push   ebx
        lea    ecx, [ebp - 0xc]
        push   esi
        push   eax
        push   ecx
        call   sub_2_335C
        add    esp, 0x10
        test   eax, eax
        jne    loc_2_2CC6
        test   byte ptr [ebp - 0xb], 0x10
        jne    loc_2_2CC6
        push   ebx
        lea    eax, [ebp - 8]
        push   esi
        push   eax
        call   sub_2_345C
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_2CC6
        cmp    dword ptr [ebp - 8], 0xff
        jae    loc_2_2CC6
        push   ebx
        lea    eax, [ebp - 0x110]
        push   0x100
        push   eax
        push   esi
        call   sub_2_3454
        add    esp, 0x10
        test   eax, eax
        jne    loc_2_2CC6
        push   3
        mov    edx, 0                        ; -> obj3:0x0354
        lea    ecx, [ebp - 0x110]
        call   sub_2_055F
        test   eax, eax
        je     loc_2_2C42
        lea    ecx, [ebp - 0x110]
        mov    bl, 4
        call   sub_2_2A16
        mov    esi, eax
        lea    ecx, [ebp - 0x110]
        call   sub_2_2A46
        mov    edi, eax
        xor    al, al
        jmp    loc_2_2C7F
loc_2_2C42:
        push   6
        mov    edx, 0                        ; -> obj3:0x0358
        lea    ecx, [ebp - 0x110]
        call   sub_2_055F
        test   eax, eax
        je     loc_2_2CC6
        lea    ecx, [ebp - 0x110]
        call   sub_2_2AE2
        mov    esi, eax
        lea    ecx, [ebp - 0x110]
        mov    bl, 0x10
        call   sub_2_2B10
        mov    edi, eax
        lea    ecx, [ebp - 0x110]
        call   sub_2_2B52
loc_2_2C7F:
        push   eax
        mov    edx, esi
        push   edi
        mov    ecx, dword ptr [0]            ; -> obj3:0x01D4
        call   sub_2_1499
        mov    si, ax
        test   si, si
        je     loc_2_2CC6
        push   ebx
        lea    edx, [ebp - 0x1e8]
        mov    ecx, dword ptr [ebp - 4]
        call   sub_2_0968
        lea    eax, [ebp - 0x1e8]
        mov    edx, esi
        push   eax
        mov    ecx, dword ptr [0]            ; -> obj3:0x01D4
        call   sub_2_20B9
        mov    edx, esi
        mov    ecx, dword ptr [0]            ; -> obj3:0x01D4
        call   sub_2_1557
loc_2_2CC6:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_2CCD:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        mov    esi, edx
        mov    ebx, ecx
loc_2_2CD9:
        push   0
        lea    eax, [ebp - 8]
        push   ebx
        push   eax
        call   sub_2_33AC
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_2CF6
        mov    edx, esi
        mov    ecx, dword ptr [ebp - 8]
        call   sub_2_2CCD
loc_2_2CF6:
        push   ebx
        call   esi
        add    esp, 4
        lea    eax, [ebp - 4]
        push   0
        push   ebx
        push   eax
        call   sub_2_33A4
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_2D14
        mov    ebx, dword ptr [ebp - 4]
        jmp    loc_2_2CD9
loc_2_2D14:
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_2D1A:
        sub    esp, 4
        xor    eax, eax
        mov    dword ptr [esp], eax
        push   eax
        push   eax
        lea    eax, [esp + 8]
        push   eax
        call   sub_2_3424
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_2D44
        mov    edx, 0                        ; -> loc_2_2B7F
        mov    ecx, dword ptr [esp]
        call   sub_2_2CCD
loc_2_2D44:
        add    esp, 4
        ret    
loc_2_2D48:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x108
        push   esi
        lea    eax, [ebp - 8]
        push   2
        mov    esi, dword ptr [ebp + 8]
        push   esi
        push   eax
        call   sub_2_32FC
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_2DCB
        push   0
        lea    eax, [ebp - 4]
        push   esi
        push   eax
        call   sub_2_345C
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_2DCB
        cmp    dword ptr [ebp - 4], 0xff
        jae    loc_2_2DCB
        push   0
        lea    eax, [ebp - 0x108]
        push   0x100
        push   eax
        push   esi
        call   sub_2_3454
        add    esp, 0x10
        test   eax, eax
        jne    loc_2_2DCB
        cmp    byte ptr [ebp - 0x108], 0
        je     loc_2_2DCB
        push   esi
        mov    ecx, dword ptr [0]            ; -> obj3:0x01D4
        movzx  dx, byte ptr [0]              ; -> obj3:0x01F4
        push   0xffffd041
        call   sub_2_215F
        cmp    ax, 1
        sbb    eax, eax
        neg    eax
        jmp    loc_2_2DCD
loc_2_2DCB:
        xor    eax, eax
loc_2_2DCD:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_2DD2:
        sub    esp, 8
        mov    ecx, 0                        ; -> obj3:0x01DC
        push   ebx
        push   esi
        push   edi
        mov    esi, 2
        push   ebp
        mov    edx, esi
        push   0
        call   sub_2_263C
        mov    di, ax
        push   0
        lea    ecx, [esp + 0x16]
        mov    edx, esi
        call   sub_2_263C
        mov    si, ax
        push   0
        lea    ecx, [esp + 0x18]
        mov    edx, 4
        call   sub_2_263C
        movzx  edx, si
        mov    bx, ax
        movzx  ecx, di
        movzx  eax, bx
        shl    eax, 0x10
        shl    edx, 0x10
        push   eax
        shl    ecx, 0x10
        call   sub_2_037A
        mov    bp, ax
        mov    ecx, edi
        call   sub_2_26DA
        mov    ecx, esi
        call   sub_2_26DA
        mov    ecx, ebx
        call   sub_2_26DA
        test   bp, bp
        je     loc_2_2E50
        mov    ax, 1
        jmp    loc_2_2FB4
loc_2_2E50:
        xor    esi, esi
        cmp    dword ptr [esp + 0x14], esi
        je     loc_2_2E7D
        movzx  eax, word ptr [0]             ; -> obj3:0x01DC
        push   esi
        push   eax
        push   dword ptr [esp + 0x1c]
        call   sub_2_3408
        add    esp, 0xc
        mov    edx, 0x10000
        mov    ecx, eax
        push   esi
        call   sub_2_263C
        mov    si, ax
loc_2_2E7D:
        movzx  eax, word ptr [0]             ; -> obj3:0x01DC
        push   1
        push   eax
        call   sub_2_3384
        add    esp, 8
        mov    dword ptr [0], eax            ; -> obj3:0x01D4
        movzx  eax, word ptr [0]             ; -> obj3:0x01DC
        push   1
        push   eax
        call   sub_2_3384
        add    esp, 8
        mov    ecx, dword ptr [0]            ; -> obj3:0x01D4
        movzx  edx, word ptr [0]             ; -> obj3:0x01DC
        push   0
        mov    dword ptr [0], eax            ; -> obj3:0x01D8
        call   sub_2_263C
        mov    bx, ax
        mov    edx, esi
        movzx  ebp, bx
        shl    ebp, 0x10
        mov    ecx, ebp
        call   sub_2_03BA
        test   ax, ax
        jne    loc_2_2F7F
        mov    dx, word ptr [0]              ; -> obj3:0x01DC
        mov    ecx, dword ptr [0]            ; -> obj3:0x01D4
        call   sub_2_22D3
        test   eax, eax
        je     loc_2_2F7F
        mov    ecx, dword ptr [0]            ; -> obj3:0x01D4
        mov    ax, word ptr [ecx]
        push   eax
        push   dword ptr [0]                 ; -> obj3:0x01D8
        push   ecx
        call   sub_2_0537
        add    esp, 0xc
        mov    edx, 0xffffd041
        mov    ecx, dword ptr [0]            ; -> obj3:0x01D4
        call   sub_2_16A0
        mov    ecx, dword ptr [0]            ; -> obj3:0x01D4
        call   sub_2_1616
        mov    ecx, 0                        ; -> loc_2_2D48
        mov    byte ptr [0], al              ; -> obj3:0x01F4
        call   sub_2_3204
        mov    di, ax
        call   sub_2_2D1A
        mov    ecx, dword ptr [0]            ; -> obj3:0x01D4
        call   sub_2_13BB
        mov    ecx, dword ptr [0]            ; -> obj3:0x01D4
        mov    ax, word ptr [ecx]
        cmp    word ptr [esp + 0x12], ax
        jb     loc_2_2F83
        sub    ax, 2
        movzx  edx, ax
        push   edx
        mov    edx, dword ptr [0]            ; -> obj3:0x01D8
        call   sub_2_055F
        test   eax, eax
        jne    loc_2_2F83
        mov    edx, esi
        mov    ecx, ebp
        call   sub_2_03FA
        test   ax, ax
        je     loc_2_2F83
loc_2_2F7F:
        mov    di, 1
loc_2_2F83:
        mov    ecx, esi
        call   sub_2_26DA
        push   0
        mov    eax, dword ptr [0]            ; -> obj3:0x01D4
        push   eax
        call   sub_2_337C
        add    esp, 8
        mov    eax, dword ptr [0]            ; -> obj3:0x01D8
        push   0
        push   eax
        call   sub_2_337C
        add    esp, 8
        mov    ecx, ebx
        call   sub_2_26DA
        mov    ax, di
loc_2_2FB4:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
loc_2_2FBC:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xdc
        push   ebx
        lea    ecx, [ebp - 0xdc]
        push   esi
        push   edi
        push   0
        push   dword ptr [ebp + 8]
        push   ecx
        call   sub_2_3464
        add    esp, 0xc
        test   eax, eax
        je     loc_2_2FE7
        xor    eax, eax
        jmp    loc_2_30F2
loc_2_2FE7:
        xor    eax, eax
        cmp    word ptr [ebp - 0x80], ax
        jbe    loc_2_3022
        lea    edi, [ebp - 0x7e]
        mov    edx, dword ptr [0]            ; -> obj3:0x01E4
        mov    esi, dword ptr [0]            ; -> obj3:0x01EC
loc_2_2FFE:
        lea    ebx, [edx + esi]
        inc    eax
        add    esi, 4
        mov    byte ptr [ebx], 0x4b
        mov    cx, word ptr [edi]
        add    edi, 2
        mov    word ptr [ebx + 1], cx
        mov    cl, byte ptr [edi + 0x26]
        mov    byte ptr [ebx + 3], cl
        movzx  ecx, word ptr [ebp - 0x80]
        cmp    ecx, eax
        jg     loc_2_2FFE
        jmp    loc_2_302E
loc_2_3022:
        mov    edx, dword ptr [0]            ; -> obj3:0x01E4
        mov    esi, dword ptr [0]            ; -> obj3:0x01EC
loc_2_302E:
        mov    dword ptr [ebp - 4], 0
        cmp    word ptr [ebp - 0xdc], 0
        jbe    loc_2_308F
        lea    edi, [ebp - 0xda]
        lea    eax, [ebp - 0x92]
loc_2_304B:
        lea    ebx, [edx + esi]
        inc    dword ptr [ebp - 4]
        mov    word ptr [ebx + 1], 9
        add    esi, 0xc
        mov    byte ptr [ebx], 0x81
        mov    cx, word ptr [eax]
        add    eax, 2
        and    cl, 1
        mov    byte ptr [ebx + 3], cl
        mov    ecx, dword ptr [edi]
        shr    ecx, 8
        add    edi, 4
        mov    word ptr [ebx + 4], cx
        mov    word ptr [ebx + 6], cx
        mov    ecx, dword ptr [edi + 0x20]
        shr    ecx, 8
        mov    word ptr [ebx + 0xa], cx
        movzx  ecx, word ptr [ebp - 0xdc]
        cmp    ecx, dword ptr [ebp - 4]
        jg     loc_2_304B
loc_2_308F:
        xor    edi, edi
        cmp    word ptr [ebp - 0x2e], di
        jbe    loc_2_30B8
loc_2_3097:
        mov    bx, 1
        lea    eax, [edx + esi]
        add    esi, 3
        inc    edi
        mov    byte ptr [eax], 0x22
        mov    cl, byte ptr [ebp + edi - 0x2d]
        shl    bx, cl
        mov    word ptr [eax + 1], bx
        movzx  eax, word ptr [ebp - 0x2e]
        cmp    eax, edi
        jg     loc_2_3097
loc_2_30B8:
        xor    edi, edi
        mov    dword ptr [0], esi            ; -> obj3:0x01EC
        cmp    word ptr [ebp - 0x1e], di
        jbe    loc_2_30ED
loc_2_30C6:
        lea    ebx, [edx + esi]
        mov    al, 1
        add    esi, 3
        inc    edi
        mov    byte ptr [ebx], 0x2a
        mov    dword ptr [0], esi            ; -> obj3:0x01EC
        mov    cl, byte ptr [ebp + edi - 0x1d]
        shl    al, cl
        mov    byte ptr [ebx + 2], 0
        mov    byte ptr [ebx + 1], al
        movzx  eax, word ptr [ebp - 0x1e]
        cmp    eax, edi
        jg     loc_2_30C6
loc_2_30ED:
        mov    eax, 1
loc_2_30F2:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_30F9:
        push   esi
        push   1
        mov    esi, 0x800
        push   esi
        call   sub_2_3384
        add    esp, 8
        mov    edx, esi
        mov    ecx, eax
        mov    dword ptr [0], eax            ; -> obj3:0x01E4
        push   0
        call   sub_2_263C
        mov    word ptr [0], ax              ; -> obj3:0x01E8
        mov    ecx, 0                        ; -> loc_2_2FBC
        call   sub_2_3204
        test   ax, ax
        jne    loc_2_3159
        mov    eax, dword ptr [0]            ; -> obj3:0x01E4
        mov    ecx, dword ptr [0]            ; -> obj3:0x01EC
        mov    byte ptr [eax + ecx], 0x79
        mov    byte ptr [eax + ecx + 1], 0
        add    dword ptr [0], 2              ; -> obj3:0x01EC
        movzx  ecx, word ptr [0]             ; -> obj3:0x01E8
        shl    ecx, 0x10
        call   sub_2_0310
loc_2_3159:
        mov    ax, word ptr [0]              ; -> obj3:0x01E8
        mov    ecx, eax
        call   sub_2_26DA
        push   0
        mov    eax, dword ptr [0]            ; -> obj3:0x01E4
        push   eax
        call   sub_2_337C
        add    esp, 8
        pop    esi
        ret    
sub_2_3178:
        push   esi
        mov    eax, 0x800
        xor    esi, esi
        push   1
        push   eax
        mov    dword ptr [0], esi            ; -> obj3:0x01D4
        mov    dword ptr [0], esi            ; -> obj3:0x01E4
        mov    dword ptr [0], esi            ; -> obj3:0x01EC
        mov    dword ptr [0], eax            ; -> obj3:0x01F0
        call   sub_2_3384
        add    esp, 8
        mov    dword ptr [0], eax            ; -> obj3:0x01D4
        cmp    eax, esi
        je     loc_2_3202
        push   esi
        mov    edx, dword ptr [0]            ; -> obj3:0x01F0
        mov    ecx, eax
        call   sub_2_263C
        movzx  ecx, ax
        mov    word ptr [0], ax              ; -> obj3:0x01E0
        shl    ecx, 0x10
        call   sub_2_0345
        movzx  eax, ax
        test   eax, eax
        je     loc_2_31DA
        cmp    eax, 0x8d
        je     loc_2_31E1
        jmp    loc_2_31E6
loc_2_31DA:
        call   sub_2_30F9
        jmp    loc_2_31E6
loc_2_31E1:
        call   sub_2_2DD2
loc_2_31E6:
        push   0
        mov    eax, dword ptr [0]            ; -> obj3:0x01D4
        push   eax
        call   sub_2_337C
        mov    cx, word ptr [0]              ; -> obj3:0x01E0
        add    esp, 8
        call   sub_2_26DA
loc_2_3202:
        pop    esi
        ret    
sub_2_3204:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xe4
        push   esi
        push   edi
        mov    esi, ecx
        xor    edi, edi
        push   edi
        lea    eax, [ebp - 4]
        push   edi
        mov    dword ptr [ebp - 4], edi
        push   eax
        call   sub_2_3424
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_3298
        push   edi
        lea    ecx, [ebp - 4]
        push   dword ptr [ebp - 4]
        push   ecx
        call   sub_2_33AC
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_3298
loc_2_323C:
        push   edi
        lea    ecx, [ebp - 0xe4]
        push   dword ptr [ebp - 4]
        push   ecx
        call   sub_2_3464
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_3279
        push   edi
        lea    ecx, [ebp - 0xc]
        push   dword ptr [ebp - 4]
        push   ecx
        lea    edx, [ebp - 8]
        push   edx
        call   sub_2_335C
        add    esp, 0x10
        test   eax, eax
        jne    loc_2_3279
        test   byte ptr [ebp - 7], 0x10
        jne    loc_2_3279
        push   dword ptr [ebp - 4]
        call   esi
        add    esp, 4
loc_2_3279:
        push   edi
        lea    ecx, [ebp - 4]
        push   dword ptr [ebp - 4]
        push   ecx
        call   sub_2_33A4
        add    esp, 0xc
        test   eax, eax
        je     loc_2_323C
        sub    eax, 0xd
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        jmp    loc_2_329C
loc_2_3298:
        mov    ax, 1
loc_2_329C:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
        db  0CCh, 0CCh, 0CDh, 020h, 064h, 080h, 001h, 000h, 0CCh, 0CCh, 0CDh, 020h, 096h, 081h, 001h, 000h
        db  0CCh, 0CCh, 0CDh, 020h, 093h, 081h, 001h, 000h, 0CCh, 0CCh, 0CDh, 020h, 063h, 080h, 001h, 000h
        db  0CCh, 0CCh
sub_2_32C4:
        VMMCall  VMM._Sprintf                    ; cd2074810100
        int3   
        int3   
sub_2_32CC:
        VxDCall  CONFIGMG._CONFIGMG_Get_Next_Res_Des ; cd201f803300
        int3   
        int3   
sub_2_32D4:
        VxDCall  CONFIGMG._CONFIGMG_Read_Registry_Value ; cd203e803300
        int3   
        int3   
sub_2_32DC:
        VxDCall  CONFIGMG._CONFIGMG_ISAPNP_To_CM ; cd2038803300
        int3   
        int3   
sub_2_32E4:
        VxDCall  CONFIGMG._CONFIGMG_Remove_Unmarked_Children ; cd2037803300
        int3   
        int3   
sub_2_32EC:
        VxDCall  CONFIGMG._CONFIGMG_Write_Registry_Value ; cd203f803300
        int3   
        int3   
sub_2_32F4:
        VxDCall  CONFIGMG._CONFIGMG_Disable_DevNode ; cd2040803300
        int3   
        int3   
sub_2_32FC:
        VxDCall  CONFIGMG._CONFIGMG_Get_First_Log_Conf ; cd201a803300
        int3   
        int3   
sub_2_3304:
        VxDCall  CONFIGMG._CONFIGMG_Remove_SubTree ; cd200d803300
        int3   
        int3   
sub_2_330C:
        VxDCall  CONFIGMG._CONFIGMG_Set_Bus_Info ; cd2043803300
        int3   
        int3   
sub_2_3314:
        VxDCall  CONFIGMG._CONFIGMG_Set_Private_DWord ; cd200a803300
        int3   
        int3   
sub_2_331C:
        VxDCall  CONFIGMG._CONFIGMG_Create_DevNode ; cd200b803300
        int3   
        int3   
sub_2_3324:
        VxDCall  CONFIGMG._CONFIGMG_Reset_Children_Marks ; cd2035803300
        int3   
        int3   
sub_2_332C:
        VMMCall  VMM._HeapReAllocate             ; cd2050800100
        int3   
        int3   
sub_2_3334:
        VMMCall  VMM._RegCloseKey                ; cd2049810100
        int3   
        int3   
sub_2_333C:
        VMMCall  VMM._RegQueryValueEx            ; cd2051810100
        int3   
        int3   
sub_2_3344:
        VMMCall  VMM._GetRegistryKey             ; cd206f810100
        int3   
        int3   
sub_2_334C:
        VMMCall  VMM._lmemcpy                    ; cd207d810100
        int3   
        int3   
sub_2_3354:
        VxDCall  CONFIGMG.Service_805Bh          ; cd205b803300
        int3   
        int3   
sub_2_335C:
        VxDCall  CONFIGMG._CONFIGMG_Get_DevNode_Status ; cd2036803300
        int3   
        int3   
sub_2_3364:
        VxDCall  CONFIGMG._CONFIGMG_Get_Private_DWord ; cd2009803300
        int3   
        int3   
sub_2_336C:
        VxDCall  CONFIGMG._CONFIGMG_Get_Parent   ; cd2003803300
        int3   
        int3   
sub_2_3374:
        VxDCall  CONFIGMG._CONFIGMG_Register_Enumerator ; cd200f803300
        int3   
        int3   
sub_2_337C:
        VMMCall  VMM._HeapFree                   ; cd2051800100
        int3   
        int3   
sub_2_3384:
        VMMCall  VMM._HeapAllocate               ; cd204f800100
        int3   
        int3   
sub_2_338C:
        VxDCall  CONFIGMG._CONFIGMG_Reenumerate_DevNode ; cd2033803300
        int3   
        int3   
sub_2_3394:
        VxDCall  CONFIGMG._CONFIGMG_Fail_Change_HW_Prof ; cd204f803300
        int3   
        int3   
sub_2_339C:
        VxDCall  CONFIGMG._CONFIGMG_Query_Remove_SubTree ; cd200c803300
        int3   
        int3   
sub_2_33A4:
        VxDCall  CONFIGMG._CONFIGMG_Get_Sibling  ; cd2005803300
        int3   
        int3   
sub_2_33AC:
        VxDCall  CONFIGMG._CONFIGMG_Get_Child    ; cd2004803300
        int3   
        int3   
sub_2_33B4:
        VxDCall  CONFIGMG._CONFIGMG_Query_Change_HW_Prof ; cd2047803300
        int3   
        int3   
sub_2_33BC:
        push   esi
        mov    esi, dword ptr [esp + 8]
        VMMCall  VMM.Cancel_Time_Out             ; cd203e000100
        pop    esi
        ret    4
        db  0CCh
sub_2_33CC:
        VxDCall  CONFIGMG._CONFIGMG_Recompute_HW_Prof ; cd2046803300
        int3   
        int3   
sub_2_33D4:
        push   esi
        mov    edx, dword ptr [esp + 0x10]
        mov    eax, dword ptr [esp + 0xc]
        mov    esi, dword ptr [esp + 8]
        VMMCall  VMM.Set_Global_Time_Out         ; cd203c000100
        mov    eax, esi
        pop    esi
        ret    0xc
        db  0CCh, 0CCh, 0CCh
sub_2_33F0:
        VxDCall  CONFIGMG._CONFIGMG_Lock         ; cd2016803300
        int3   
        int3   
sub_2_33F8:
        VxDCall  CONFIGMG._CONFIGMG_Unlock       ; cd2017803300
        int3   
        int3   
sub_2_3400:
        VxDCall  CONFIGMG._CONFIGMG_Call_At_Appy_Time ; cd204e803300
        int3   
        int3   
sub_2_3408:
        VMMCall  VMM._MapPhysToLinear            ; cd206c800100
        int3   
        int3   
sub_2_3410:
        push   edi
        mov    edi, dword ptr [esp + 8]
        VxDCall  VPICD.VPICD_Virtualize_IRQ      ; cd2001000300
        cmc    
        sbb    ecx, ecx
        pop    edi
        and    eax, ecx
        ret    4
sub_2_3424:
        VxDCall  CONFIGMG._CONFIGMG_Locate_DevNode ; cd2002803300
        int3   
        int3   
sub_2_342C:
        VMMCall  VMM._CopyPageTable              ; cd2061800100
        int3   
        int3   
sub_2_3434:
        VMMCall  VMM._PageAllocate               ; cd2053800100
        int3   
        int3   
sub_2_343C:
        VxDCall  CONFIGMG._CONFIGMG_Register_DevLoader ; cd2032803300
        int3   
        int3   
sub_2_3444:
        VxDCall  CONFIGMG._CONFIGMG_Register_Device_Driver ; cd200e803300
        int3   
        int3   
sub_2_344C:
        VMMCall  VMM.VMM_GetSystemInitState      ; cd2011810100
        int3   
        int3   
sub_2_3454:
        VxDCall  CONFIGMG._CONFIGMG_Get_Device_ID ; cd2007803300
        int3   
        int3   
sub_2_345C:
        VxDCall  CONFIGMG._CONFIGMG_Get_Device_ID_Size ; cd2006803300
        int3   
        int3   
sub_2_3464:
        VxDCall  CONFIGMG._CONFIGMG_Get_Alloc_Log_Conf ; cd203b803300
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 3  vsize=0x35f  RW-
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 094h, 0CFh, 0E8h
        db  0AEh, 029h, 0CFh, 011h, 097h, 0EAh, 000h, 0AAh, 000h, 034h, 031h, 09Dh, 042h, 049h, 04Fh, 053h
        db  05Ch, 02Ah, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 05Ch, 03Fh, 03Fh, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 025h, 034h, 058h, 05Ch, 025h, 032h, 058h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 043h, 06Fh, 06Eh, 066h
        db  069h, 067h, 046h, 06Ch, 061h, 067h, 073h, 000h, 043h, 053h, 043h, 06Fh, 06Eh, 066h, 069h, 067h
        db  046h, 06Ch, 061h, 067h, 073h, 000h, 000h, 000h, 043h, 053h, 043h, 06Fh, 06Eh, 066h, 069h, 067h
        db  046h, 06Ch, 061h, 067h, 073h, 000h, 000h, 000h, 043h, 053h, 043h, 06Fh, 06Eh, 066h, 069h, 067h
        db  046h, 06Ch, 061h, 067h, 073h, 000h, 000h, 000h, 042h, 049h, 04Fh, 053h, 000h, 000h, 000h, 000h
        db  04Fh, 070h, 074h, 069h, 06Fh, 06Eh, 073h, 000h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 042h, 055h, 053h, 05Fh
        db  000h, 000h, 000h, 000h, 042h, 055h, 053h, 05Fh, 000h, 000h, 000h, 000h, 044h, 045h, 056h, 05Fh
        db  000h, 000h, 000h, 000h, 046h, 055h, 04Eh, 043h, 05Fh, 000h, 000h, 000h, 02Ah, 000h, 000h, 000h
        db  05Ch, 000h, 000h, 000h, 05Ch, 000h, 000h, 000h, 05Ch, 000h, 000h, 000h, 05Fh, 044h, 045h, 056h
        db  000h, 000h, 000h, 000h, 050h, 043h, 049h, 000h, 049h, 053h, 041h, 050h, 04Eh, 050h, 000h
VxD__LDATA ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0x595  R-X
sub_4_0000:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_0004:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_0008:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_000C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_0014:
        inc    edx
        dec    ecx
        dec    edi
        push   ebx
        pop    esp
        sub    dl, byte ptr [eax + 0x4e]
        push   eax
        xor    byte ptr [ebx + 0x30], al
        xor    ebx, dword ptr [eax + esi]
loc_4_0024:
        inc    esp
        insb   byte ptr es:[edi], dx
        insb   byte ptr es:[edi], dx
        and    byte ptr [eax], al
        add    byte ptr [eax], al
loc_4_002C:
        inc    ecx
        xor    byte ptr [eax], al
        add    byte ptr [ebx - 0x74aeeb14], al
        inc    esp
        and    al, 0x1c
        mov    ecx, dword ptr [eax + 8]
        cmp    ecx, dword ptr [0]            ; -> loc_4_0004
        ja     loc_4_00A0
        add    ecx, dword ptr [eax + 0xc]
        cmp    ecx, dword ptr [0]            ; -> loc_4_0004
        jbe    loc_4_00A0
        mov    ecx, dword ptr [eax + 0x10]
        mov    dword ptr [esp + 0x14], ecx
        mov    ecx, dword ptr [eax + 8]
        mov    dword ptr [esp + 0xc], ecx
        sub    ecx, dword ptr [0]            ; -> loc_4_0004
        jecxz  loc_4_007F
        neg    ecx
        mov    dword ptr [esp + 0x10], ecx
        lea    eax, [esp + 4]
        push   dword ptr [esp + 0x20]
        push   eax
        call   sub_4_0000
        pop    ecx
        pop    ecx
        or     eax, eax
        je     loc_4_00AD
loc_4_007F:
        mov    eax, dword ptr [esp + 0x1c]
        mov    ecx, dword ptr [0]            ; -> loc_4_0004
        inc    ecx
        mov    dword ptr [esp + 0xc], ecx
        sub    ecx, dword ptr [eax + 8]
        sub    ecx, dword ptr [eax + 0xc]
        jecxz  loc_4_00AD
        neg    ecx
        mov    dword ptr [esp + 0x10], ecx
        lea    eax, [esp + 4]
loc_4_00A0:
        push   dword ptr [esp + 0x20]
        push   eax
        call   sub_4_0000
        pop    ecx
        pop    ecx
loc_4_00AD:
        pop    ecx
        add    esp, 0x14
        ret    
sub_4_00B2:
        push   ebp
        mov    eax, 0
        mov    ebp, esp
        sub    esp, 8
        mov    dword ptr [ebp - 8], eax
        mov    dword ptr [ebp - 4], eax
        VxDCall  VXDLDR.VXDLDR_GetVersion        ; cd2000002900
        setae  byte ptr [ebp - 8]
        cmp    dword ptr [ebp - 8], eax
        je     loc_4_00DB
        VxDCall  VXDLDR.VXDLDR_LoadDevice        ; cd2001002900
        mov    dword ptr [ebp - 4], eax
loc_4_00DB:
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret    
sub_4_00E2:
        push   esi
        mov    eax, dword ptr [0]            ; -> loc_4_0004
        push   0
        push   1
        push   eax
        call   sub_2_3408
loc_4_00F2:
        add    esp, 0xc
        mov    dword ptr [0], eax            ; -> obj1:0x0064
        cmp    dword ptr [0], 0x110000       ; -> loc_4_0004
        jae    loc_4_011D
        mov    eax, 0x100a6
        lea    esi, [0]                      ; -> obj4:0x0030
        VMMCall  VMM.Hook_Device_Service         ; cd2090000100
        mov    dword ptr [0], esi            ; -> sub_4_0000
loc_4_011D:
        push   0
        push   0xfa
        push   0                             ; -> loc_1_00D3
        call   sub_2_33D4
loc_4_012E:
        pop    esi
        ret    
sub_4_0130:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x2c
        push   esi
        lea    eax, [ebp - 0xc]
        push   edi
        xor    esi, esi
        push   esi
        push   0                             ; -> loc_4_0014
        push   eax
        call   sub_2_3424
loc_4_0149:
        add    esp, 0xc
        test   eax, eax
        jne    loc_4_01D9
        push   2
        lea    ecx, [ebp - 8]
        push   dword ptr [ebp - 0xc]
        push   ecx
        call   sub_2_32FC
loc_4_0162:
        add    esp, 0xc
        test   eax, eax
        jne    loc_4_01D9
        push   esi
        lea    ecx, [ebp - 4]
        push   esi
        push   4
        push   dword ptr [ebp - 8]
        push   ecx
        call   sub_2_32CC
loc_4_0179:
        add    esp, 0x14
        test   eax, eax
        jne    loc_4_01D9
        mov    eax, dword ptr [ebp - 4]
        push   esi
        mov    di, word ptr [eax + 2]
        push   esi
        push   2
        lea    eax, [ebp - 4]
        push   dword ptr [ebp - 8]
        push   eax
        call   sub_2_32CC
loc_4_0197:
        add    esp, 0x14
        test   eax, eax
        jne    loc_4_01D9
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 4]
        mov    word ptr [0], ax              ; -> obj1:0x0060
        mov    dword ptr [ebp - 0x24], esi
        mov    word ptr [ebp - 0x2c], di
        mov    dword ptr [ebp - 0x20], esi
        mov    dword ptr [ebp - 0x1c], esi
        mov    dword ptr [ebp - 0x18], esi
        lea    edi, [ebp - 0x2c]
        mov    dword ptr [ebp - 0x14], esi
        mov    dword ptr [ebp - 0x28], 0     ; -> loc_1_009A
        mov    word ptr [ebp - 0x2a], 1
        push   edi
        call   sub_2_3410
loc_4_01D4:
        mov    dword ptr [0], eax            ; -> loc_4_000C
loc_4_01D9:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_4_01DF:
        sub    esp, 4
        push   esi
        xor    esi, esi
        cmp    dword ptr [0], esi            ; -> obj3:0x0028
        jne    loc_4_0229
        cmp    ecx, 0xc0000000
        jb     loc_4_0229
        cmp    ecx, 0xffbfffff
        ja     loc_4_0229
        shr    edx, 0xc
        push   esi
        lea    eax, [esp + 8]
        push   eax
        push   1
        push   edx
        call   sub_2_342C
loc_4_020E:
        add    esp, 0x10
        test   eax, eax
        je     loc_4_0229
        mov    eax, dword ptr [esp + 4]
        xor    eax, dword ptr [esp + 0xc]
        test   eax, 0xfffff000
        je     loc_4_0229
        mov    esi, 1
loc_4_0229:
        mov    eax, esi
        pop    esi
        add    esp, 4
        ret    4
sub_4_0232:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        call   sub_4_00B2
        test   eax, eax
        je     loc_4_0254
        mov    ecx, 1
        mov    dword ptr [0], ecx            ; -> obj3:0x0028
        mov    dword ptr [ebp - 8], ecx
        jmp    loc_4_026F
loc_4_0254:
        push   0
        push   0x10000
        push   0xf0000
        call   sub_2_3408
loc_4_0265:
        mov    dword ptr [ebp - 8], 0x1000
        add    esp, 0xc
loc_4_026F:
        mov    dword ptr [ebp - 0xc], eax
loc_4_0272:
        cmp    dword ptr [eax], 0x506e5024
        jne    loc_4_0299
        mov    esi, eax
        mov    edi, eax
        mov    cl, byte ptr [eax + 5]
        xor    dl, dl
        test   cl, cl
        je     loc_4_0290
        mov    bl, cl
loc_4_0289:
        add    dl, byte ptr [edi]
        inc    edi
        dec    bl
        jne    loc_4_0289
loc_4_0290:
        test   dl, dl
        jne    loc_4_0299
        cmp    cl, 0x13
        jae    loc_4_02AD
loc_4_0299:
        add    eax, 0x10
        dec    dword ptr [ebp - 8]
        jne    loc_4_0272
loc_4_02A1:
        mov    eax, 0x24
loc_4_02A6:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_4_02AD:
        cmp    cl, 0x21
        jb     loc_4_02A1
        cmp    byte ptr [esi + 4], 0x10
        jb     loc_4_02A1
        lea    edi, [esi + 0x13]
        mov    eax, edi
        mov    edx, edi
        sub    eax, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [edi]
        add    eax, 0xf0000
        push   eax
        call   sub_4_01DF
        test   eax, eax
        je     loc_4_02D7
        mov    edi, dword ptr [edi]
        jmp    loc_4_02EA
loc_4_02D7:
        push   0
        push   0x10000
        push   dword ptr [edi]
        call   sub_2_3408
loc_4_02E5:
        add    esp, 0xc
        mov    edi, eax
loc_4_02EA:
        mov    ax, word ptr [esi + 0x11]
        lea    ebx, [esi + 0x1d]
        mov    word ptr [ebp - 4], ax
        mov    edx, ebx
        mov    eax, ebx
        mov    ecx, dword ptr [ebx]
        sub    eax, dword ptr [ebp - 0xc]
        add    eax, 0xf0000
        push   eax
        call   sub_4_01DF
        test   eax, eax
        je     loc_4_0311
        mov    ebx, dword ptr [ebx]
        jmp    loc_4_0324
loc_4_0311:
        push   0
        push   0x10000
        push   dword ptr [ebx]
        call   sub_2_3408
loc_4_031F:
        add    esp, 0xc
        mov    ebx, eax
loc_4_0324:
        mov    cx, word ptr [esi + 6]
        mov    eax, dword ptr [esi + 0x17]
        mov    word ptr [ebp - 2], cx
        mov    dword ptr [0], eax            ; -> obj3:0x0060
        mov    eax, dword ptr [esi + 9]
        cmp    edi, -1
        mov    dword ptr [0], eax            ; -> loc_4_0004
        je     loc_4_056B
        cmp    ebx, -1
        je     loc_4_056B
        cmp    dword ptr [0], 0              ; -> obj3:0x0060
        jne    loc_4_036B
        movzx  eax, byte ptr [esi + 8]
        mov    ecx, dword ptr [ebp - 8]
        shl    ecx, 8
        add    eax, ecx
        shl    eax, 8
        mov    dword ptr [0], eax            ; -> obj3:0x0060
loc_4_036B:
        mov    eax, dword ptr [0]            ; -> obj3:0x0060
        cmp    eax, 0x7ae110e
        je     loc_4_0385
        cmp    eax, 0xaae110e
        je     loc_4_0385
        cmp    eax, 0xbae110e
        jne    loc_4_038F
loc_4_0385:
        mov    dword ptr [0], 0              ; -> obj3:0x01B8
loc_4_038F:
        cmp    dword ptr [0], 0              ; -> obj3:0x0028
        jne    loc_4_03C9
        test   edi, edi
        je     loc_4_02A1
        push   1
        mov    edx, 0x10000
        mov    ecx, edi
        call   sub_2_263C
loc_4_03AE:
        mov    word ptr [0], ax              ; -> obj3:0x0000
        test   ax, ax
        jne    loc_4_03BF
        VMMCall  VMM.Fatal_Memory_Error          ; cd20bf800100
loc_4_03BF:
        mov    ax, word ptr [ebp - 4]
        mov    word ptr [0], ax              ; -> obj3:0x0004
loc_4_03C9:
        cmp    ebx, -1
        je     loc_4_03ED
        push   0
        mov    edx, 0x10000
        mov    ecx, ebx
        call   sub_2_263C
loc_4_03DC:
        mov    word ptr [0], ax              ; -> obj3:0x0010
        test   ax, ax
        jne    loc_4_03ED
        VMMCall  VMM.Fatal_Memory_Error          ; cd20bf800100
loc_4_03ED:
        push   0x201
        push   4
        call   sub_2_3384
loc_4_03F9:
        add    esp, 8
        mov    dword ptr [0], eax            ; -> obj3:0x000C
        test   eax, eax
        jne    loc_4_040B
        VMMCall  VMM.Fatal_Memory_Error          ; cd20bf800100
loc_4_040B:
        push   0
        mov    edx, 4
        mov    ecx, dword ptr [0]            ; -> obj3:0x000C
        call   sub_2_263C
loc_4_041D:
        mov    word ptr [0], ax              ; -> obj3:0x0008
        test   ax, ax
        jne    loc_4_042E
        VMMCall  VMM.Fatal_Memory_Error          ; cd20bf800100
loc_4_042E:
        xor    esi, esi
        cmp    dword ptr [0], esi            ; -> obj3:0x0028
        jne    loc_4_047D
        push   1
        push   esi
        push   esi
        push   esi
        push   esi
        push   esi
        push   1
        push   4
        call   sub_2_3434
loc_4_0448:
        add    esp, 0x20
        mov    dword ptr [0], eax            ; -> obj3:0x0018
        cmp    eax, esi
        jne    loc_4_045A
        VMMCall  VMM.Fatal_Memory_Error          ; cd20bf800100
loc_4_045A:
        push   0
        mov    edx, 0x4000
        mov    ecx, dword ptr [0]            ; -> obj3:0x0018
        call   sub_2_263C
loc_4_046C:
        mov    word ptr [0], ax              ; -> obj3:0x0014
        test   ax, ax
        jne    loc_4_047D
        VMMCall  VMM.Fatal_Memory_Error          ; cd20bf800100
loc_4_047D:
        call   sub_2_0133
loc_4_0482:
        test   ax, ax
        jne    loc_4_02A1
        mov    eax, dword ptr [0]            ; -> obj3:0x000C
        push   0x202
        movzx  ecx, word ptr [eax + 2]
        mov    dword ptr [0], ecx            ; -> obj3:0x0164
        lea    esi, [ecx*2 + 0x19]
        push   esi
        push   eax
        call   sub_2_332C
loc_4_04AD:
        add    esp, 0xc
        mov    dword ptr [0], eax            ; -> obj3:0x000C
        test   eax, eax
        jne    loc_4_04BF
        VMMCall  VMM.Fatal_Memory_Error          ; cd20bf800100
loc_4_04BF:
        mov    cx, word ptr [0]              ; -> obj3:0x0008
        push   esi
        mov    edx, dword ptr [0]            ; -> obj3:0x000C
        call   sub_2_2697
loc_4_04D2:
        xor    eax, eax
        mov    ax, word ptr [ebp - 2]
        and    eax, 3
        cmp    eax, 1
        je     loc_4_04E7
        cmp    eax, 2
        je     loc_4_04EE
        jmp    loc_4_04F3
loc_4_04E7:
        call   sub_4_00E2
        jmp    loc_4_04F3
loc_4_04EE:
        call   sub_4_0130
loc_4_04F3:
        push   0
        mov    edi, 0                        ; -> loc_4_0024
        push   0x10000
        push   0xf0000
        call   sub_2_3408
loc_4_0509:
        add    esp, 0xc
        lea    esi, [eax + 0xe076]
        mov    ecx, 5
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        jne    loc_4_0550
        cmp    byte ptr [eax + 0xe845], 0x48
        jne    loc_4_0550
        lea    esi, [eax + 0xe842]
        mov    edi, 0                        ; -> loc_4_002C
        mov    ecx, 2
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        jne    loc_4_0550
        mov    al, byte ptr [eax + 0xe844]
        cmp    al, 0x30
        jb     loc_4_0550
        cmp    al, 0x36
        ja     loc_4_0550
        mov    dword ptr [0], 1              ; -> obj3:0x0050
loc_4_0550:
        mov    ecx, 0x42
        call   sub_2_0259
loc_4_055A:
        mov    dword ptr [0], 1              ; -> obj3:0x01D0
        xor    eax, eax
        jmp    loc_4_02A6
loc_4_056B:
        mov    eax, 2
        jmp    loc_4_02A6
sub_4_0575:
        push   esi
        push   0
        push   dword ptr [esp + 0xc]
        mov    esi, 1
        mov    dword ptr [0], esi            ; -> loc_4_0008
        call   sub_2_343C
loc_4_058C:
        add    esp, 8
        mov    eax, esi
        pop    esi
        ret    4
VxD__LTEXT ENDS

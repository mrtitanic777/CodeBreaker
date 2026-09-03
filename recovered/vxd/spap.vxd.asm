; SPAP -- reconstructed 32-bit disassembly (VxD / LE format)
; 2 objects, 2264 instructions recovered by recursive descent

Declare_Virtual_Device SPAP, 1, 0, SPAP_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 080000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x1d74  R-X
SPAP_Control:
        cmp    eax, 0
        jne    loc_1_000F
        push   ebx
        call   sub_2_0040
        cmp    eax, 1
        ret    
loc_1_000F:
        cmp    eax, 1
        jne    loc_1_001E
        push   ebx
        call   sub_2_0050
        cmp    eax, 1
        ret    
loc_1_001E:
        cmp    eax, 2
        jne    loc_1_002D
        push   ebx
        call   sub_2_0060
        cmp    eax, 1
        ret    
loc_1_002D:
        cmp    eax, 0x1b
        jne    loc_1_003B
        call   sub_2_0070
        cmp    eax, 1
        ret    
loc_1_003B:
        cmp    eax, 0x1c
        jne    loc_1_0049
        call   sub_2_0080
        cmp    eax, 1
        ret    
loc_1_0049:
        clc    
        ret    
SPAP_V86_API:
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0x1c]
        call   sub_1_00A0
        mov    dword ptr [ebp + 0x1c], eax
        ret    
SPAP_PM_API:
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0x1c]
        call   sub_1_00E0
        mov    dword ptr [ebp + 0x1c], eax
        ret    
sub_1_006F:
        VMMCall  VMM.Get_Last_Updated_System_Time ; cd20cf000100
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_1_0080:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        call   sub_1_006F
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_00A0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        cmp    eax, 1
        je     loc_1_00B6
        cmp    eax, 2
        je     loc_1_00C3
        sub    eax, eax
        pop    ebp
        ret    0xc
loc_1_00B6:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_0120
        pop    ebp
        ret    0xc
loc_1_00C3:
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_0130
        pop    ebp
        ret    0xc
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_00E0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        cmp    eax, 1
        je     loc_1_00F6
        cmp    eax, 2
        je     loc_1_0103
        sub    eax, eax
        pop    ebp
        ret    0xc
loc_1_00F6:
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_0140
        pop    ebp
        ret    0xc
loc_1_0103:
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        call   sub_1_0150
        pop    ebp
        ret    0xc
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0120:
        mov    eax, 1
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_0130:
        mov    eax, 2
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_0140:
        mov    eax, 1
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_0150:
        mov    eax, 2
        ret    8
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 0CDh, 020h, 000h, 080h, 028h, 000h
sub_1_0166:
        VxDCall  VPOWERD._VPOWERD_W32_Get_System_Power_Status ; cd200c802800
sub_1_016C:
        VxDCall  VPOWERD._VPOWERD_W32_Set_System_Power_State ; cd200d802800
        VxDCall  VPOWERD.Service_800Fh           ; cd200f802800
        VxDCall  VPOWERD.Service_8010h           ; cd2010802800
        VxDCall  VPOWERD.Service_8011h           ; cd2011802800
        VxDCall  VPOWERD.Service_800Eh           ; cd200e802800
sub_1_018A:
        VxDCall  VPOWERD._VPOWERD_Register_Power_Handler ; cd200a802800
sub_1_0190:
        VxDCall  VPOWERD._VPOWERD_Deregister_Power_Handler ; cd200b802800
        VxDCall  VPOWERD.Service_8016h           ; cd2016802800
        VxDCall  VPOWERD.Service_8017h           ; cd2017802800
        VxDCall  VPOWERD.Service_8018h           ; cd2018802800
        VxDCall  VPOWERD.Service_8019h           ; cd2019802800
        VxDCall  VPOWERD.Service_801Ah           ; cd201a802800
        VxDCall  VPOWERD.Service_801Bh           ; cd201b802800
        VxDCall  VPOWERD.Service_801Ch           ; cd201c802800
        VxDCall  VPOWERD.Service_801Dh           ; cd201d802800
        VxDCall  VPOWERD.Service_801Eh           ; cd201e802800
        VxDCall  VPOWERD.Service_801Fh           ; cd201f802800
        VxDCall  VPOWERD.Service_8020h           ; cd2020802800
        VxDCall  VPOWERD.Service_8021h           ; cd2021802800
        VxDCall  VPOWERD.Service_8022h           ; cd2022802800
        VxDCall  VPOWERD.Service_8023h           ; cd2023802800
        VxDCall  VPOWERD.Service_8024h           ; cd2024802800
        VxDCall  VPOWERD.Service_8025h           ; cd2025802800
        VxDCall  VPOWERD.Service_8026h           ; cd2026802800
        VxDCall  VPOWERD.Service_8027h           ; cd2027802800
        VxDCall  VPOWERD.Service_8028h           ; cd2028802800
        VxDCall  VPOWERD._VPOWERD_Set_System_Power_State ; cd2005802800
        VxDCall  VPOWERD._VPOWERD_Restore_Power_On_Defaults ; cd2006802800
        VxDCall  VPOWERD._VPOWERD_Get_Power_Status ; cd2007802800
        VxDCall  VPOWERD.Service_8042h           ; cd2042802800
        VxDCall  VPOWERD.Service_8012h           ; cd2012802800
        VxDCall  VPOWERD.Service_8013h           ; cd2013802800
        VxDCall  VPOWERD.Service_8014h           ; cd2014802800
        VxDCall  VPOWERD.Service_8015h           ; cd2015802800
        VxDCall  VPOWERD.Service_804Ah           ; cd204a802800
        VxDCall  VPOWERD.Service_804Bh           ; cd204b802800
        VxDCall  VPOWERD.Service_804Ch           ; cd204c802800
        VxDCall  VPOWERD.Service_804Dh           ; cd204d802800
        VxDCall  VPOWERD.Service_804Eh           ; cd204e802800
        VxDCall  VPOWERD.Service_8046h           ; cd2046802800
        VxDCall  VPOWERD.Service_8045h           ; cd2045802800
        VxDCall  VPOWERD.Service_8043h           ; cd2043802800
        VxDCall  VPOWERD.Service_8047h           ; cd2047802800
        add    byte ptr [eax], al
sub_1_0270:
        push   esi
        push   edi
        mov    ecx, dword ptr [esp + 0x14]
        mov    al, cl
        and    al, 3
        shr    ecx, 2
        mov    esi, dword ptr [esp + 0x10]
        mov    edi, dword ptr [esp + 0xc]
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    cl, al
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    edi
        pop    esi
        ret    0xc
        db  056h, 057h, 09Ch, 08Bh, 04Ch, 024h, 018h, 08Bh, 074h, 024h, 014h, 08Bh, 07Ch, 024h, 010h, 03Bh
        db  0F7h, 073h, 00Dh, 00Bh, 0C9h, 074h, 01Fh, 0FDh, 049h, 003h, 0F1h, 003h, 0F9h, 041h, 0EBh, 014h
        db  08Bh, 0C6h, 02Bh, 0C7h, 083h, 0F8h, 004h, 072h, 00Bh, 08Ah, 0C1h, 024h, 003h, 0C1h, 0E9h, 002h
        db  0F3h, 0A5h, 08Ah, 0C8h, 0F3h, 0A4h, 09Dh, 05Fh, 05Eh, 0C2h, 00Ch, 000h
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  1, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'SPAP    '                  ; DDB_Name
        dd  080000000h                  ; DDB_Init_Order
        dd  SPAP_Control             ; DDB_Control_Proc
        dd  SPAP_V86_API             ; DDB_V86_API_Proc
        dd  SPAP_PM_API              ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h
sub_1_0320:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        sub    ax, ax
        cmp    byte ptr [ecx], al
        je     loc_1_0335
loc_1_032D:
        inc    ecx
        inc    ax
        cmp    byte ptr [ecx], 0
        jne    loc_1_032D
loc_1_0335:
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0340:
        push   ebp
        mov    ebp, esp
        mov    ax, word ptr [ebp + 8]
        xchg   ah, al
        mov    word ptr [ebp + 8], ax
        mov    ax, word ptr [ebp + 8]
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        mov    al, byte ptr [ecx]
        mov    dl, al
        and    edx, 0xffff00ff
        or     al, al
        je     loc_1_0398
        not    al
        add    al, 0xa
        mov    byte ptr [ecx], al
        inc    ecx
        mov    ax, dx
        dec    dx
        or     ax, ax
        je     loc_1_0398
loc_1_0385:
        mov    al, byte ptr [ecx]
        not    al
        add    al, 6
        mov    byte ptr [ecx], al
        inc    ecx
        mov    ax, dx
        dec    dx
        or     ax, ax
        jne    loc_1_0385
loc_1_0398:
        pop    ebp
        ret    4
        db  08Dh, 064h, 024h, 000h
        push   ebp
        mov    ebp, esp
        push   ebx
        sub    edx, edx
        mov    ecx, dword ptr [ebp + 0x10]
        cmp    ecx, edx
        jle    loc_1_03BD
        mov    eax, dword ptr [ebp + 8]
        mov    bl, byte ptr [ebp + 0xc]
loc_1_03B3:
        cmp    byte ptr [eax], bl
        jne    loc_1_03C7
        inc    edx
        inc    eax
        cmp    edx, ecx
        jl     loc_1_03B3
loc_1_03BD:
        mov    eax, 1
        pop    ebx
        pop    ebp
        ret    0xc
loc_1_03C7:
        sub    eax, eax
        pop    ebx
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    bx, word ptr [ebp + 0xc]
        mov    eax, ebx
        and    eax, 0xffff
        mov    esi, dword ptr [ebp + 8]
        lea    edi, [eax + esi]
        mov    ax, 0x40
        sub    ax, bx
        push   eax
        push   edi
        call   sub_1_06E8
        add    esp, 8
        mov    byte ptr [edi], 0x80
        shl    bx, 3
        mov    byte ptr [esi + 0x38], bl
        mov    byte ptr [esi + 0x39], bh
        mov    dword ptr [ebp - 0x10], 0x67452301
        mov    dword ptr [ebp - 0xc], 0xefcdab89
        mov    edx, esi
        mov    dword ptr [ebp - 8], 0x98badcfe
        mov    dword ptr [ebp - 4], 0x10325476
        mov    ecx, 4
loc_1_042B:
        mov    edi, dword ptr [ebp - 0xc]
        not    edi
        and    edi, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, dword ptr [ebp - 8]
        or     edi, eax
        add    edi, dword ptr [edx]
        add    edi, dword ptr [ebp - 0x10]
        add    edx, 4
        lea    eax, [edi*8]
        shr    edi, 0x1d
        or     eax, edi
        mov    dword ptr [ebp - 0x10], eax
        mov    edi, eax
        not    edi
        and    edi, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 0xc]
        and    eax, dword ptr [ebp - 0x10]
        or     edi, eax
        add    edi, dword ptr [edx]
        add    edi, dword ptr [ebp - 4]
        add    edx, 4
        mov    eax, edi
        shl    eax, 7
        shr    edi, 0x19
        or     eax, edi
        mov    dword ptr [ebp - 4], eax
        mov    edi, eax
        not    edi
        and    edi, dword ptr [ebp - 0xc]
        and    eax, dword ptr [ebp - 0x10]
        or     edi, eax
        add    edi, dword ptr [edx]
        add    edi, dword ptr [ebp - 8]
        add    edx, 4
        mov    eax, edi
        shl    eax, 0xb
        shr    edi, 0x15
        or     eax, edi
        mov    dword ptr [ebp - 8], eax
        mov    edi, eax
        not    edi
        and    edi, dword ptr [ebp - 0x10]
        and    eax, dword ptr [ebp - 4]
        or     edi, eax
        add    edi, dword ptr [edx]
        add    edi, dword ptr [ebp - 0xc]
        add    edx, 4
        mov    eax, edi
        shl    eax, 0x13
        shr    edi, 0xd
        or     eax, edi
        mov    dword ptr [ebp - 0xc], eax
        dec    ecx
        jne    loc_1_042B
        mov    edx, 0x1b34                   ; -> obj1:0x1B34
        mov    ecx, 4
loc_1_04C9:
        mov    edi, dword ptr [ebp - 8]
        or     edi, dword ptr [ebp - 4]
        and    edi, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [ebp - 8]
        and    eax, dword ptr [ebp - 4]
        or     edi, eax
        xor    eax, eax
        mov    al, byte ptr [edx]
        add    edi, dword ptr [esi + eax]
        mov    eax, dword ptr [ebp - 0x10]
        lea    eax, [edi + eax + 0x5a827999]
        inc    edx
        lea    edi, [eax*8]
        shr    eax, 0x1d
        or     edi, eax
        mov    dword ptr [ebp - 0x10], edi
        mov    edi, dword ptr [ebp - 8]
        or     edi, dword ptr [ebp - 0x10]
        and    edi, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [ebp - 8]
        and    eax, dword ptr [ebp - 0x10]
        or     edi, eax
        xor    eax, eax
        mov    al, byte ptr [edx]
        add    edi, dword ptr [esi + eax]
        mov    eax, dword ptr [ebp - 4]
        lea    eax, [edi + eax + 0x5a827999]
        inc    edx
        mov    edi, eax
        shr    edi, 0x1b
        shl    eax, 5
        or     edi, eax
        mov    dword ptr [ebp - 4], edi
        or     edi, dword ptr [ebp - 0x10]
        and    edi, dword ptr [ebp - 0xc]
        mov    eax, dword ptr [ebp - 4]
        and    eax, dword ptr [ebp - 0x10]
        or     edi, eax
        xor    eax, eax
        mov    al, byte ptr [edx]
        add    edi, dword ptr [esi + eax]
        mov    eax, dword ptr [ebp - 8]
        lea    eax, [edi + eax + 0x5a827999]
        inc    edx
        mov    edi, eax
        shr    edi, 0x17
        shl    eax, 9
        or     edi, eax
        mov    dword ptr [ebp - 8], edi
        or     edi, dword ptr [ebp - 0x10]
        and    edi, dword ptr [ebp - 4]
        mov    eax, dword ptr [ebp - 8]
        and    eax, dword ptr [ebp - 0x10]
        or     edi, eax
        xor    eax, eax
        mov    al, byte ptr [edx]
        add    edi, dword ptr [esi + eax]
        mov    eax, dword ptr [ebp - 0xc]
        lea    eax, [edi + eax + 0x5a827999]
        inc    edx
        mov    edi, eax
        shr    edi, 0x13
        shl    eax, 0xd
        or     edi, eax
        mov    dword ptr [ebp - 0xc], edi
        dec    ecx
        jne    loc_1_04C9
        mov    edx, 0x1b44                   ; -> obj1:0x1B44
        mov    ecx, 4
loc_1_0596:
        mov    edi, dword ptr [ebp - 0xc]
        xor    edi, dword ptr [ebp - 8]
        xor    eax, eax
        mov    al, byte ptr [edx]
        mov    ebx, dword ptr [esi + eax]
        mov    eax, dword ptr [ebp - 4]
        xor    eax, edi
        add    ebx, eax
        mov    eax, dword ptr [ebp - 0x10]
        lea    eax, [ebx + eax + 0x6ed9eba1]
        inc    edx
        lea    ebx, [eax*8]
        shr    eax, 0x1d
        or     ebx, eax
        mov    dword ptr [ebp - 0x10], ebx
        xor    eax, eax
        mov    al, byte ptr [edx]
        mov    ebx, dword ptr [esi + eax]
        mov    eax, dword ptr [ebp - 0x10]
        xor    eax, edi
        add    ebx, eax
        mov    eax, dword ptr [ebp - 4]
        lea    eax, [ebx + eax + 0x6ed9eba1]
        inc    edx
        mov    edi, eax
        shr    edi, 0x17
        shl    eax, 9
        or     edi, eax
        mov    dword ptr [ebp - 4], edi
        xor    eax, eax
        mov    al, byte ptr [edx]
        mov    edi, dword ptr [esi + eax]
        mov    eax, dword ptr [ebp - 0xc]
        xor    eax, dword ptr [ebp - 4]
        inc    edx
        xor    eax, dword ptr [ebp - 0x10]
        add    edi, eax
        mov    eax, dword ptr [ebp - 8]
        lea    eax, [edi + eax + 0x6ed9eba1]
        mov    edi, eax
        shl    edi, 0xb
        shr    eax, 0x15
        or     edi, eax
        mov    dword ptr [ebp - 8], edi
        xor    eax, eax
        mov    al, byte ptr [edx]
        mov    edi, dword ptr [esi + eax]
        mov    eax, dword ptr [ebp - 8]
        xor    eax, dword ptr [ebp - 4]
        inc    edx
        xor    eax, dword ptr [ebp - 0x10]
        add    edi, eax
        mov    eax, dword ptr [ebp - 0xc]
        lea    eax, [edi + eax + 0x6ed9eba1]
        mov    edi, eax
        shl    edi, 0xf
        shr    eax, 0x11
        or     edi, eax
        mov    dword ptr [ebp - 0xc], edi
        dec    ecx
        jne    loc_1_0596
        add    dword ptr [ebp - 0x10], 0x67452301
        sub    dword ptr [ebp - 0xc], 0x10325477
        sub    dword ptr [ebp - 8], 0x67452302
        add    dword ptr [ebp - 4], 0x10325476
        lea    edi, [ebp - 0x10]
loc_1_0664:
        sub    ecx, ecx
        mov    eax, dword ptr [edi]
loc_1_0668:
        mov    edx, eax
        shr    edx, cl
        mov    byte ptr [esi], dl
        inc    esi
        add    ecx, 8
        cmp    ecx, 0x20
        jl     loc_1_0668
        add    edi, 4
        lea    eax, [ebp]
        cmp    edi, eax
        jb     loc_1_0664
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_0690:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 4], 0
        jmp    loc_1_06A8
loc_1_06A4:
        inc    word ptr [ebp - 4]
loc_1_06A8:
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 0x10]
        and    ecx, 0xffff
        cmp    eax, ecx
        jge    loc_1_06E3
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 0xc]
        mov    al, byte ptr [eax + ecx]
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0xffff
        mov    edx, dword ptr [ebp + 8]
        mov    byte ptr [ecx + edx], al
        jmp    loc_1_06A4
loc_1_06E3:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_06E8:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    word ptr [ebp - 4], 0
        jmp    loc_1_0700
loc_1_06FC:
        inc    word ptr [ebp - 4]
loc_1_0700:
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 0xffff
        cmp    eax, ecx
        jle    loc_1_072D
        mov    eax, dword ptr [ebp - 4]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 8]
        mov    byte ptr [eax + ecx], 0
        jmp    loc_1_06FC
loc_1_072D:
        pop    edi
        pop    esi
        pop    ebx
        leave  
        ret    
sub_1_0732:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        sub    ecx, ecx
        cmp    dword ptr [ebp + 0x14], ecx
        jle    loc_1_0781
        mov    esi, dword ptr [ebp + 0x18]
        and    esi, 0xffff
loc_1_0748:
        mov    eax, ecx
        cdq    
        idiv   dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp + 8]
        mov    bl, byte ptr [edx + eax]
        lea    eax, [esi + ecx]
        mov    edi, 0x1f
        cdq    
        idiv   edi
        mov    edi, 0x11
        xor    bl, byte ptr [edx + 0x1b54]   ; -> loc_1_1B54
        mov    eax, ecx
        cdq    
        idiv   edi
        mov    eax, dword ptr [ebp + 0x10]
        xor    bl, byte ptr [edx + 0x1b74]   ; -> loc_1_1B74
        mov    byte ptr [eax + ecx], bl
        inc    ecx
        cmp    ecx, dword ptr [ebp + 0x14]
        jl     loc_1_0748
loc_1_0781:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x14
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_0790:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x40
        push   ebx
        push   esi
        push   edi
        push   0x13
        mov    esi, dword ptr [ebp + 0xc]
        push   esi
        call   sub_1_06E8
        add    esp, 8
        push   0x40
        lea    eax, [ebp - 0x40]
        push   eax
        call   sub_1_06E8
        add    esp, 8
        call   sub_1_0080
        mov    di, ax
        push   edi
        push   0x11
        lea    eax, [ebp - 0x40]
        push   eax
        mov    ebx, dword ptr [ebp + 8]
        push   ebx
        call   sub_1_0320
        movsx  eax, ax
        inc    eax
        push   eax
        push   ebx
        call   sub_1_0732
        push   edi
        call   sub_1_0340
        mov    word ptr [esi], ax
        push   0x11
        lea    eax, [ebp - 0x40]
        push   eax
        add    esi, 2
        push   esi
        call   sub_1_0690
        add    esp, 0xc
        mov    eax, 0x13
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0810:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    ax, word ptr [esi]
        push   eax
        call   sub_1_0340
        push   eax
        push   0x11
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        add    esi, 2
        push   0x11
        push   esi
        call   sub_1_0732
        pop    esi
        pop    ebp
        ret    8
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0840:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        cmp    dword ptr [ebp + 0xc], 0
        mov    dword ptr [eax], 0x4c
        mov    word ptr [eax + 4], 1
        mov    word ptr [eax + 6], 0x27c0
        jne    loc_1_0864
        mov    word ptr [eax + 6], 0x23c1
loc_1_0864:
        mov    word ptr [eax + 8], 0
        mov    dword ptr [eax + 0xa], 0x38
        mov    byte ptr [eax + 0xe], 1
        mov    byte ptr [eax + 0xf], 6
        mov    dword ptr [eax + 0x10], 0
        mov    dword ptr [eax + 0x14], 0
        mov    dword ptr [eax + 0x18], 0xc80 ; -> loc_1_0C80
        mov    dword ptr [eax + 0x1c], 0xca0 ; -> loc_1_0CA0
        mov    dword ptr [eax + 0x20], 0xd30 ; -> loc_1_0D30
        mov    dword ptr [eax + 0x24], 0xd40 ; -> loc_1_0D40
        mov    dword ptr [eax + 0x28], 0xd50 ; -> loc_1_0D50
        mov    dword ptr [eax + 0x2c], 0xd80 ; -> loc_1_0D80
        mov    dword ptr [eax + 0x30], 0xd90 ; -> loc_1_0D90
        mov    dword ptr [eax + 0x34], 0xe50 ; -> loc_1_0E50
        mov    dword ptr [eax + 0x38], 0xfc0 ; -> loc_1_0FC0
        mov    dword ptr [eax + 0x3c], 0
        mov    dword ptr [eax + 0x40], 0x950 ; -> loc_1_0950
        mov    dword ptr [eax + 0x44], 0x960 ; -> loc_1_0960
        mov    dword ptr [eax + 0x48], 0x990 ; -> loc_1_0990
        pop    ebp
        ret    8
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_08F0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        add    eax, 0x4b
        mov    word ptr [eax], 1
        mov    dword ptr [eax + 2], 1
        mov    word ptr [eax + 6], 1
        mov    word ptr [eax + 8], 1
        mov    dword ptr [eax + 0xa], 0
        mov    dword ptr [eax + 0xe], 0
        mov    word ptr [eax + 0x12], 0
        mov    word ptr [eax + 0x14], 0
        mov    word ptr [eax + 0x1a], 0
        mov    dword ptr [eax + 0x1c], 0
        mov    word ptr [eax + 0x20], 0
        mov    word ptr [eax + 0x22], 0
        mov    dword ptr [eax + 0x24], 5
        pop    ebp
        ret    4
        db  090h
loc_1_0950:
        mov    eax, 4
        ret    
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0960:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0xc]
        mov    word ptr [eax], 1
        add    dword ptr [ebp + 0xc], 2
        mov    eax, dword ptr [ebp + 0xc]
        mov    word ptr [eax], 0x100
        mov    eax, 4
        add    dword ptr [ebp + 0xc], 2
        pop    ebp
        ret    
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0990:
        push   ebp
        mov    ebp, esp
        mov    ecx, dword ptr [ebp + 8]
        add    ecx, 0x4b
        xor    eax, eax
        mov    al, byte ptr [ebp + 0xc]
        cmp    eax, 1
        je     loc_1_09B1
        cmp    eax, 2
        je     loc_1_09D6
        cmp    eax, 3
        je     loc_1_09DD
        sub    eax, eax
        pop    ebp
        ret    
loc_1_09B1:
        xor    eax, eax
        mov    al, byte ptr [ebp + 0x14]
        sub    eax, 2
        push   eax
        mov    eax, dword ptr [ebp + 0x10]
        add    eax, 2
        push   eax
        push   ecx
        call   sub_1_0BD0
        or     eax, eax
        je     loc_1_09D2
        mov    eax, 1
        pop    ebp
        ret    
loc_1_09D2:
        sub    eax, eax
        pop    ebp
        ret    
loc_1_09D6:
        mov    eax, 1
        pop    ebp
        ret    
loc_1_09DD:
        sub    eax, eax
        pop    ebp
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_09F0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        lea    edx, [eax + 0x4b]
        mov    eax, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 0xc]
        mov    ebx, dword ptr [eax + 0xfe]
        mov    eax, dword ptr [ebp + 0x10]
        mov    word ptr [eax], 0
        mov    ecx, dword ptr [ebx + 0x20]
        or     ecx, ecx
        jne    loc_1_0A20
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
loc_1_0A20:
        mov    eax, dword ptr [ebp + 0xc]
        add    cl, 2
        mov    byte ptr [eax], cl
        mov    esi, eax
        inc    esi
        sub    cx, cx
        cmp    dword ptr [ebx + 0x20], 0
        jbe    loc_1_0A53
loc_1_0A34:
        mov    edi, ecx
        and    edi, 0xffff
        mov    eax, dword ptr [ebx + 0x30]
        mov    al, byte ptr [edi + eax]
        mov    byte ptr [esi], al
        inc    cx
        inc    esi
        mov    eax, ecx
        and    eax, 0xffff
        cmp    eax, dword ptr [ebx + 0x20]
        jb     loc_1_0A34
loc_1_0A53:
        cmp    dword ptr [ebx + 0x28], 0
        je     loc_1_0AA3
        mov    edi, 0x1d34                   ; -> obj1:0x1D34
        sub    eax, eax
        mov    ecx, 0x10
        rep stosd dword ptr es:[edi], eax
        mov    eax, dword ptr [edx + 2]
        push   eax
        push   0x1d34                        ; -> obj1:0x1D34
        mov    eax, dword ptr [ebx + 0x38]
        inc    esi
        push   eax
        call   sub_1_0790
        lea    ecx, [eax + 2]
        mov    byte ptr [esi - 1], cl
        sub    cx, cx
        cmp    ax, cx
        je     loc_1_0AA7
loc_1_0A88:
        mov    edx, ecx
        and    edx, 0xffff
        inc    esi
        mov    dl, byte ptr [edx + 0x1d34]   ; -> obj1:0x1D34
        mov    byte ptr [esi - 1], dl
        inc    cx
        cmp    ax, cx
        ja     loc_1_0A88
        jmp    loc_1_0AA7
loc_1_0AA3:
        mov    byte ptr [esi], 2
        inc    esi
loc_1_0AA7:
        sub    esi, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp + 0x10]
        inc    si
        mov    word ptr [eax], si
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh
sub_1_0AC0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        lea    esi, [eax + 0x4b]
        mov    eax, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 0xc]
        mov    edx, dword ptr [eax + 0xfe]
        mov    eax, dword ptr [ebp + 0xc]
        mov    byte ptr [eax - 4], 6
        mov    eax, dword ptr [ebp + 0x10]
        mov    word ptr [eax], 0
        cmp    dword ptr [edx + 0x20], 0
        jne    loc_1_0AF6
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
loc_1_0AF6:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ebx, eax
        mov    byte ptr [eax], 1
        inc    ebx
        mov    eax, dword ptr [edx + 0x20]
        add    al, 2
        mov    byte ptr [ebx], al
        inc    ebx
        sub    cx, cx
        cmp    dword ptr [edx + 0x20], 0
        jbe    loc_1_0B2E
loc_1_0B10:
        mov    eax, ecx
        and    eax, 0xffff
        mov    edi, dword ptr [edx + 0x30]
        mov    al, byte ptr [eax + edi]
        mov    byte ptr [ebx], al
        inc    cx
        inc    ebx
        mov    eax, ecx
        and    eax, 0xffff
        cmp    eax, dword ptr [edx + 0x20]
        jb     loc_1_0B10
loc_1_0B2E:
        mov    byte ptr [ebx], 2
        inc    ebx
        cmp    dword ptr [edx + 0x28], 0
        je     loc_1_0B82
        mov    edi, 0x1d34                   ; -> obj1:0x1D34
        sub    eax, eax
        mov    ecx, 0x10
        rep stosd dword ptr es:[edi], eax
        mov    eax, dword ptr [esi + 2]
        push   eax
        push   0x1d34                        ; -> obj1:0x1D34
        mov    eax, dword ptr [edx + 0x38]
        inc    ebx
        push   eax
        call   sub_1_0790
        lea    ecx, [eax + 2]
        mov    byte ptr [ebx - 1], cl
        sub    dx, dx
        cmp    ax, dx
        je     loc_1_0B86
loc_1_0B67:
        mov    ecx, edx
        and    ecx, 0xffff
        inc    ebx
        mov    cl, byte ptr [ecx + 0x1d34]   ; -> obj1:0x1D34
        mov    byte ptr [ebx - 1], cl
        inc    dx
        cmp    ax, dx
        ja     loc_1_0B67
        jmp    loc_1_0B86
loc_1_0B82:
        mov    byte ptr [ebx], 2
        inc    ebx
loc_1_0B86:
        mov    byte ptr [ebx], 3
        inc    ebx
        mov    byte ptr [ebx], 4
        inc    ebx
        mov    ax, word ptr [esi + 6]
        mov    ch, al
        mov    cl, ah
        mov    word ptr [ebx], cx
        add    ebx, 2
        mov    byte ptr [ebx], 4
        inc    ebx
        mov    byte ptr [ebx], 4
        inc    ebx
        mov    ax, word ptr [esi + 8]
        mov    ch, al
        mov    cl, ah
        mov    word ptr [ebx], cx
        add    ebx, 2
        sub    ebx, dword ptr [ebp + 0xc]
        mov    eax, dword ptr [ebp + 0x10]
        inc    bx
        mov    word ptr [eax], bx
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0xc
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_0BD0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    eax, dword ptr [ebp + 0x10]
        or     eax, eax
        je     loc_1_0BF0
        cmp    eax, 2
        je     loc_1_0C09
        cmp    eax, 4
        je     loc_1_0C40
        sub    eax, eax
        pop    esi
        pop    ebp
        ret    0xc
        db  08Dh, 064h, 024h, 000h
loc_1_0BF0:
        mov    esi, dword ptr [ebp + 8]
        mov    eax, 1
        mov    dword ptr [esi + 2], 0
        mov    word ptr [esi], 0
        pop    esi
        pop    ebp
        ret    0xc
loc_1_0C09:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    ax, word ptr [ecx]
        mov    cx, ax
        and    ecx, 0xff
        shl    ecx, 8
        shr    ax, 8
        and    eax, 0xffff
        mov    esi, dword ptr [ebp + 8]
        or     ecx, eax
        mov    dword ptr [esi + 2], ecx
        mov    word ptr [esi], 0
        mov    eax, 1
        pop    esi
        pop    ebp
        ret    0xc
        db  005h, 000h, 000h, 000h, 000h
loc_1_0C40:
        mov    ecx, dword ptr [ebp + 0xc]
        mov    ax, word ptr [ecx]
        mov    dx, ax
        and    edx, 0xff
        shl    edx, 8
        shr    ax, 8
        and    eax, 0xffff
        mov    esi, dword ptr [ebp + 8]
        or     edx, eax
        mov    dword ptr [esi + 2], edx
        mov    ax, word ptr [ecx + 4]
        add    ecx, 4
        mov    ch, al
        mov    cl, ah
        mov    word ptr [esi], cx
        mov    eax, 1
        pop    esi
        pop    ebp
        ret    0xc
        db  005h, 000h, 000h, 000h, 000h
loc_1_0C80:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [0x1b94], eax       ; -> obj1:0x1B94
        push   eax
        call   sub_1_08F0
        mov    eax, 0x800
        pop    ebp
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
loc_1_0CA0:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        lea    esi, [ecx + 0x4b]
        mov    eax, dword ptr [ecx + 4]
        mov    edi, dword ptr [eax + 0xc]
        mov    dword ptr [0x1b94], ecx       ; -> obj1:0x1B94
        cmp    dword ptr [edi + 0xdc], ecx
        jne    loc_1_0D00
        mov    dword ptr [edi + 0xd4], 0
        mov    dword ptr [edi + 0xdc], 0
        mov    dword ptr [edi + 0xe0], 0
        mov    eax, dword ptr [edi + 0xd8]
        or     eax, eax
        je     loc_1_0CF6
        push   0
        mov    ecx, dword ptr [eax + 0xc]
        push   ecx
        push   eax
        call   sub_1_0190
loc_1_0CF6:
        mov    dword ptr [edi + 0xd8], 0
loc_1_0D00:
        cmp    dword ptr [esi + 0x34], 0
        je     loc_1_0D1A
        mov    dword ptr [esi + 0x34], 0
        lea    eax, [ebp - 1]
        push   eax
        add    esi, 0x28
        push   esi
        call   sub_1_016C
loc_1_0D1A:
        mov    eax, 0x800
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0D30:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [0x1b94], eax       ; -> obj1:0x1B94
        pop    ebp
        ret    
        db  08Dh, 049h, 000h
loc_1_0D40:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [0x1b94], eax       ; -> obj1:0x1B94
        pop    ebp
        ret    
        db  08Dh, 049h, 000h
loc_1_0D50:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        pop    ebp
        mov    dword ptr [0x1b94], eax       ; -> obj1:0x1B94
        mov    dword ptr [eax + 0x73], 0
        mov    dword ptr [eax + 0x77], 0x1970 ; -> loc_1_1970
        mov    dword ptr [eax + 0x7b], eax
        lea    ecx, [eax + 0x4b]
        mov    word ptr [eax + 0x23], 0x2710
        ret    
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0D80:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [0x1b94], eax       ; -> obj1:0x1B94
        pop    ebp
        ret    
        db  08Dh, 049h, 000h
loc_1_0D90:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        mov    edx, dword ptr [ecx + 4]
        lea    esi, [ecx + 0x4b]
        mov    eax, dword ptr [edx + 0xc]
        cmp    dword ptr [edx + 0x10], 0
        mov    edi, dword ptr [eax + 0xfe]
        je     loc_1_0E38
        xor    eax, eax
        mov    ax, word ptr [esi]
        or     eax, eax
        mov    eax, dword ptr [edi + 0x20]
        je     loc_1_0E04
        mov    edx, dword ptr [ebp + 0xc]
        add    ax, 2
        mov    word ptr [edx], ax
        cmp    dword ptr [edi + 0x28], 0
        jbe    loc_1_0DD3
        add    ax, 0x15
        jmp    loc_1_0DD7
loc_1_0DD3:
        add    ax, 2
loc_1_0DD7:
        mov    word ptr [edx], ax
        mov    di, word ptr [edx]
        add    di, 8
        mov    word ptr [edx], di
        cmp    dword ptr [esi + 0xa], 0
        je     loc_1_0E2C
        mov    ax, word ptr [esi + 0x12]
        add    ax, di
        add    ax, 2
        mov    word ptr [edx], ax
        mov    al, 1
        mov    dword ptr [0x1b94], ecx       ; -> obj1:0x1B94
        pop    edi
        pop    esi
        pop    ebp
        ret    
loc_1_0E04:
        mov    edx, dword ptr [ebp + 0xc]
        inc    ax
        mov    word ptr [edx], ax
        cmp    dword ptr [edi + 0x28], 0
        jbe    loc_1_0E18
        add    ax, 0x14
        jmp    loc_1_0E1A
loc_1_0E18:
        inc    ax
loc_1_0E1A:
        mov    word ptr [edx], ax
        cmp    dword ptr [esi + 0xa], 0
        je     loc_1_0E2C
        mov    ax, word ptr [esi + 0x12]
        inc    ax
        add    word ptr [edx], ax
loc_1_0E2C:
        mov    al, 1
        mov    dword ptr [0x1b94], ecx       ; -> obj1:0x1B94
        pop    edi
        pop    esi
        pop    ebp
        ret    
loc_1_0E38:
        mov    edx, dword ptr [ebp + 0xc]
        sub    al, al
        mov    word ptr [edx], 0
        mov    dword ptr [0x1b94], ecx       ; -> obj1:0x1B94
        pop    edi
        pop    esi
        pop    ebp
        ret    
        db  08Dh, 064h, 024h, 000h
loc_1_0E50:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        mov    ecx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ecx + 4]
        lea    edx, [ecx + 0x4b]
        mov    dword ptr [0x1b94], ecx       ; -> obj1:0x1B94
        cmp    dword ptr [eax + 0x10], 0
        je     loc_1_0E90
        xor    eax, eax
        mov    ax, word ptr [edx]
        or     eax, eax
        lea    eax, [ebp - 2]
        push   eax
        mov    eax, dword ptr [ebp + 0xc]
        push   eax
        push   ecx
        je     loc_1_0E86
        call   sub_1_0AC0
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_0E86:
        call   sub_1_09F0
        mov    esp, ebp
        pop    ebp
        ret    
        db  090h
loc_1_0E90:
        mov    eax, dword ptr [ebp + 0xc]
        mov    byte ptr [eax], 0
        mov    byte ptr [eax + 1], 0
        mov    esp, ebp
        pop    ebp
        ret    
        db  08Bh, 0FFh
sub_1_0EA0:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 8]
        inc    word ptr [edx + 0x5f]
        add    edx, 0x4b
        cmp    word ptr [ebp + 0x10], 4
        jbe    loc_1_0EE0
        mov    eax, dword ptr [ebp + 0xc]
        mov    ax, word ptr [eax]
        mov    cx, ax
        and    ecx, 0xff
        shl    ecx, 8
        shr    ax, 8
        and    eax, 0xffff
        or     ecx, eax
        mov    dword ptr [edx + 0x16], ecx
        mov    eax, 1
        pop    ebp
        ret    0xc
        db  08Dh, 049h, 000h
loc_1_0EE0:
        mov    dword ptr [edx + 0x16], 0
        mov    eax, 1
        pop    ebp
        ret    0xc
        push   ebp
        mov    ebp, esp
        push   esi
        mov    edx, dword ptr [ebp + 8]
        inc    word ptr [edx + 0x65]
        add    edx, 0x4b
        cmp    word ptr [ebp + 0x10], 4
        lea    esi, [edx + 0x1c]
        jbe    loc_1_0F30
        mov    eax, dword ptr [ebp + 0xc]
        mov    ax, word ptr [eax]
        mov    cx, ax
        and    ecx, 0xff
        shl    ecx, 8
        shr    ax, 8
        and    eax, 0xffff
        or     ecx, eax
        mov    dword ptr [esi], ecx
        jmp    loc_1_0F36
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_0F30:
        mov    dword ptr [esi], 0
loc_1_0F36:
        cmp    dword ptr [esi], 3
        jne    loc_1_0F4C
        mov    dword ptr [edx + 0xe], 0
        mov    eax, 1
        pop    esi
        pop    ebp
        ret    0xc
loc_1_0F4C:
        sub    eax, eax
        pop    esi
        pop    ebp
        ret    0xc
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 0B8h, 001h, 000h
        db  000h, 000h, 0C2h, 00Ch, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 0B8h, 001h, 000h
        db  000h, 000h, 0C2h, 00Ch, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_1_0F80:
        push   esi
        sub    esi, esi
        call   sub_2_0090
        push   1
        push   0x1c9c                        ; -> obj1:0x1C9C
        call   sub_1_0840
        call   sub_1_1A10
        cmp    eax, esi
        je     loc_1_0FB6
        push   0x1c9c                        ; -> obj1:0x1C9C
        call   sub_1_1A30
        add    esp, 4
        sub    eax, 0x800
        cmp    eax, 1
        sbb    esi, esi
        neg    esi
loc_1_0FB6:
        mov    eax, esi
        pop    esi
        ret    4
        db  08Dh, 064h, 024h, 000h
loc_1_0FC0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x1c
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [esi + 4]
        lea    eax, [esi + 0x4b]
        mov    dword ptr [ebp - 0x10], edx
        mov    dword ptr [ebp - 8], eax
        mov    eax, dword ptr [esi + 0x2f]
        xor    ecx, ecx
        mov    cl, byte ptr [eax]
        mov    edi, dword ptr [edx + 0xc]
        mov    bl, 0x10
        mov    edx, dword ptr [edi + 0xfe]
        mov    dword ptr [ebp - 0x14], edx
        mov    dword ptr [0x1b94], esi       ; -> obj1:0x1B94
        mov    al, byte ptr [esi + 0x10]
        cmp    al, 3
        mov    byte ptr [ebp - 0xc], al
        jne    loc_1_1007
        mov    byte ptr [esi + 0x11], bl
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_1007:
        cmp    ecx, 5
        jne    loc_1_1022
        mov    edx, dword ptr [esi + 0x2f]
        mov    al, byte ptr [esi + 0x15]
        cmp    byte ptr [edx + 1], al
        je     loc_1_1022
        mov    byte ptr [esi + 0x11], 0x10
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_1022:
        dec    ecx
        cmp    ecx, 5
        ja     loc_1_1030
        jmp    dword ptr [ecx*4 + 0x119c]    ; -> obj1:0x119C
        db  090h
loc_1_1030:
        mov    bl, 0xc
        mov    byte ptr [esi + 0x11], bl
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  08Dh, 064h, 024h, 000h
loc_1_1040:
        mov    bl, 5
        mov    byte ptr [esi + 0x11], bl
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  08Dh, 064h, 024h, 000h
loc_1_1050:
        cmp    byte ptr [ebp - 0xc], 9
        je     loc_1_1093
        sub    al, al
        mov    edx, dword ptr [ebp - 0x14]
        sub    cl, cl
        cmp    dword ptr [edx], 0
        je     loc_1_1064
        mov    al, 1
loc_1_1064:
        mov    edx, dword ptr [ebp - 0x10]
        cmp    dword ptr [edx + 0x10], 0
        je     loc_1_106F
        mov    cl, 1
loc_1_106F:
        cmp    al, cl
        je     loc_1_108F
        mov    byte ptr [esi + 0x11], 6
        mov    word ptr [esi + 0x39], 0
        push   esi
        call   sub_1_1AD0
        add    esp, 4
        push   esi
        call   sub_1_1AF0
        add    esp, 4
loc_1_108F:
        mov    bl, 8
        jmp    loc_1_1095
loc_1_1093:
        mov    bl, 0x10
loc_1_1095:
        mov    ax, word ptr [esi + 0x33]
        push   eax
        mov    eax, dword ptr [esi + 0x2f]
        push   eax
        push   esi
        call   sub_1_0EA0
        mov    byte ptr [esi + 0x11], bl
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  08Bh, 0FFh
loc_1_10B0:
        mov    eax, dword ptr [ebp - 8]
        push   edi
        mov    dword ptr [eax + 0xa], 1
        inc    word ptr [eax + 0x14]
        mov    dword ptr [edi + 0xd0], 0x805
        mov    dword ptr [edi + 0xd8], 0
        mov    dword ptr [edi + 0xdc], esi
        call   sub_1_1B10
        add    esp, 4
        mov    byte ptr [esi + 0x11], bl
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  005h, 000h, 000h, 000h, 000h
loc_1_10F0:
        cmp    byte ptr [esi + 0x22], 0
        je     loc_1_1107
        lea    eax, [ebp - 1]
        push   eax
        lea    eax, [esi + 0x16]
        push   eax
        call   sub_1_016C
        mov    byte ptr [esi + 0x22], 0
loc_1_1107:
        mov    dword ptr [edi + 0xd0], 0x800
        mov    dword ptr [edi + 0xd4], 0x901
        mov    dword ptr [edi + 0xd8], 0
        mov    dword ptr [edi + 0xdc], esi
        mov    dword ptr [edi + 0xe0], 0x19b0 ; -> loc_1_19B0
        push   edi
        call   sub_1_1B10
        add    esp, 4
        mov    byte ptr [esi + 0x11], 0x10
        mov    byte ptr [esi + 0x11], bl
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  08Dh, 064h, 024h, 000h
loc_1_1150:
        mov    eax, dword ptr [ebp - 8]
        cmp    word ptr [eax + 0x22], 0
        je     loc_1_1166
        mov    bl, 0x10
        mov    byte ptr [esi + 0x11], bl
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_1_1166:
        lea    eax, [esi + 0x39]
        push   eax
        mov    eax, dword ptr [esi + 0x35]
        push   eax
        mov    ax, word ptr [esi + 0x33]
        push   eax
        mov    eax, dword ptr [esi + 0x2f]
        push   eax
        push   esi
        call   sub_1_1690
        or     eax, eax
        je     loc_1_1190
        mov    bl, 0x10
        mov    byte ptr [esi + 0x11], bl
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  08Dh, 049h, 000h
loc_1_1190:
        mov    bl, 7
loc_1_1192:
        mov    byte ptr [esi + 0x11], bl
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        ; ---- dispatch table (6 entries) ----
        dd  loc_1_1040
        dd  loc_1_1050
        dd  loc_1_10B0
        dd  loc_1_10F0
        dd  loc_1_1192
        dd  loc_1_1150
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0xc]
        or     eax, eax
        lea    ecx, [eax - 4]
        jne    loc_1_11D3
        sub    eax, eax
        pop    ebp
        ret    0xc
loc_1_11D3:
        mov    eax, dword ptr [ebp + 0x10]
        mov    byte ptr [ecx], 2
        mov    word ptr [eax], 0
        mov    eax, 1
        pop    ebp
        ret    0xc
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_11F0:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 0xc]
        or     eax, eax
        lea    ecx, [eax - 4]
        jne    loc_1_1203
        sub    eax, eax
        pop    ebp
        ret    0x10
loc_1_1203:
        mov    byte ptr [ecx], 4
        mov    cl, byte ptr [ebp + 0x14]
        or     cl, cl
        je     loc_1_1220
        mov    byte ptr [eax], cl
        mov    eax, dword ptr [ebp + 0x10]
        pop    ebp
        mov    word ptr [eax], 1
        mov    eax, 1
        ret    0x10
loc_1_1220:
        mov    eax, dword ptr [ebp + 0x10]
        pop    ebp
        mov    word ptr [eax], 0
        mov    eax, 1
        ret    0x10
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_1240:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        lea    ebx, [esi + 0x4b]
        mov    edi, dword ptr [eax + 0xc]
        mov    ecx, dword ptr [edi + 0xfe]
        dec    word ptr [ebx + 0x22]
        mov    eax, dword ptr [edi + 0xd8]
        mov    dword ptr [ebp - 8], ecx
        mov    cl, byte ptr [eax + 0x14]
        mov    byte ptr [ebp - 1], cl
        mov    dword ptr [edi + 0xd0], 0x801
        mov    dword ptr [edi + 0xd4], 0
        mov    dword ptr [edi + 0xdc], 0
        mov    dword ptr [edi + 0xe0], 0
        push   0
        mov    ecx, dword ptr [eax + 0xc]
        push   ecx
        push   eax
        call   sub_1_0190
        mov    ecx, dword ptr [ebp + 0xc]
        mov    dword ptr [edi + 0xd8], 0
        mov    dword ptr [ecx + 0x10], 0x801
        push   6
        push   edi
        call   sub_1_1A50
        add    esp, 8
        or     eax, eax
        je     loc_1_13A9
        mov    edx, dword ptr [eax + 0x1c]
        mov    ecx, dword ptr [ebp + 0xc]
        add    edx, dword ptr [eax + 0x20]
        cmp    dword ptr [ecx + 0x18], 0
        je     loc_1_1323
        mov    byte ptr [edx], 2
        mov    cl, byte ptr [ebp - 1]
        mov    byte ptr [edx + 1], cl
        mov    word ptr [edx + 2], 0x600
        mov    ecx, dword ptr [ebp - 8]
        mov    word ptr [edx + 4], 0
        mov    dword ptr [ecx + 0x10], 1
        mov    ecx, dword ptr [ebp + 0xc]
        mov    edi, dword ptr [ecx + 0x1c]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0x14], edi
        mov    ecx, dword ptr [ebp + 0xc]
        mov    edi, dword ptr [ecx + 0x20]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0x18], edi
        mov    ecx, dword ptr [ebp + 0xc]
        mov    edi, dword ptr [ecx + 0x24]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [ecx + 0x1c], edi
        lea    edi, [esi + 0x11]
        mov    byte ptr [edi], 6
        jmp    loc_1_133E
loc_1_1323:
        mov    byte ptr [edx], 4
        mov    cl, byte ptr [ebp - 1]
        mov    byte ptr [edx + 1], cl
        mov    word ptr [edx + 2], 0x600
        lea    edi, [esi + 0x11]
        mov    word ptr [edx + 4], 0
        mov    byte ptr [edi], 7
loc_1_133E:
        mov    dword ptr [esi + 0x3b], eax
        mov    dword ptr [esi + 0x35], edx
        mov    word ptr [esi + 0x39], 6
        push   esi
        call   sub_1_13C0
        cmp    byte ptr [edi], 5
        je     loc_1_1367
        push   esi
        call   sub_1_1AD0
        add    esp, 4
        push   esi
        call   sub_1_1AF0
        add    esp, 4
loc_1_1367:
        mov    ecx, dword ptr [ebp + 0xc]
        cmp    dword ptr [ecx + 0x18], 0
        jne    loc_1_13A9
        mov    eax, dword ptr [ebx + 0x24]
        dec    eax
        mov    dword ptr [ebx + 0x24], eax
        jne    loc_1_1394
        push   esi
        call   sub_1_1AD0
        add    esp, 4
        push   esi
        call   sub_1_1AF0
        add    esp, 4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
loc_1_1394:
        push   0xea60
        lea    eax, [ebx + 0x28]
        push   eax
        call   sub_1_0166
        mov    dword ptr [ebx + 0x34], 1
loc_1_13A9:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_13C0:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        mov    eax, dword ptr [eax + 0xc]
        mov    edi, dword ptr [eax + 0xfe]
        mov    al, byte ptr [esi + 0x11]
        cmp    al, 7
        jne    loc_1_1400
        mov    byte ptr [esi + 0x10], 6
        push   esi
        call   sub_1_1AD0
        add    esp, 4
        push   esi
        call   sub_1_1AF0
        add    esp, 4
        mov    byte ptr [esi + 0x11], 5
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
        db  08Dh, 049h, 000h
loc_1_1400:
        cmp    al, 6
        jne    loc_1_1449
        sub    ebx, ebx
        sub    cl, cl
        sub    dl, dl
        cmp    byte ptr [esi + 0x10], 9
        jne    loc_1_1419
        mov    byte ptr [esi + 0x10], 6
        mov    ebx, 1
loc_1_1419:
        cmp    dword ptr [edi], 0
        je     loc_1_1420
        mov    cl, 1
loc_1_1420:
        mov    eax, dword ptr [esi + 4]
        cmp    dword ptr [eax + 0x10], 0
        je     loc_1_142B
        mov    dl, 1
loc_1_142B:
        or     ebx, ebx
        jne    loc_1_1433
        cmp    dl, cl
        je     loc_1_1449
loc_1_1433:
        push   esi
        call   sub_1_1AD0
        add    esp, 4
        push   esi
        call   sub_1_1AF0
        add    esp, 4
        mov    byte ptr [esi + 0x11], 8
loc_1_1449:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    4
sub_1_1450:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    ecx, dword ptr [ebp + 8]
        mov    ebx, dword ptr [ebp + 0xc]
        and    ebx, 0xffff
        mov    eax, dword ptr [ecx + 4]
        mov    ecx, dword ptr [ebp + 0x14]
        mov    esi, dword ptr [eax + 0xc]
        and    ecx, 0xffff
        mov    dword ptr [ebp - 8], ecx
        lea    edi, [ebx + ecx + 0x2a]
        mov    eax, dword ptr [0x1b90]       ; -> obj1:0x1B90
        push   eax
        push   0
        push   edi
        lea    eax, [ebp - 4]
        push   eax
        call   sub_1_018A
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0xc], edi
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 8], 1
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x10], 0x800
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x18], ebx
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax + 0x1c], 0
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 8]
        mov    dword ptr [eax + 0x20], ecx
        xor    eax, eax
        mov    ecx, dword ptr [ebp - 4]
        mov    al, byte ptr [ebp + 0x1c]
        push   ebx
        mov    dword ptr [ecx + 0x14], eax
        mov    edi, dword ptr [ebp - 4]
        add    edi, 0x28
        mov    eax, dword ptr [ebp + 0x10]
        push   eax
        push   edi
        add    edi, ebx
        call   sub_1_0270
        mov    byte ptr [edi], 0
        mov    ecx, dword ptr [ebp - 8]
        push   ecx
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        inc    edi
        push   edi
        call   sub_1_0270
        mov    eax, dword ptr [ebp - 8]
        mov    byte ptr [edi + eax], 0
        mov    dword ptr [esi + 0xd0], 0x800
        mov    dword ptr [esi + 0xd4], 0x900
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0xd8], eax
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [esi + 0xdc], ecx
        mov    dword ptr [esi + 0xe0], 0x1240 ; -> loc_1_1240
        inc    word ptr [ecx + 0x6d]
        push   esi
        call   sub_1_1B10
        mov    ecx, dword ptr [ebp + 8]
        add    esp, 4
        mov    byte ptr [ecx + 0x11], 0x10
        sub    ax, ax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x18
        db  08Bh, 0FFh, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        sub    esp, 0x6c
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        lea    edi, [ebp - 0x6c]
        lea    esi, [edx + 0x4b]
        sub    eax, eax
        mov    ecx, 0x10
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        lea    edi, [ebp - 0x28]
        mov    ecx, 8
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    eax, dword ptr [ebp + 0x10]
        and    eax, 0xffff
        mov    ecx, dword ptr [ebp + 0xc]
        add    eax, ecx
        mov    dword ptr [ebp - 4], eax
        mov    bl, byte ptr [ecx]
        and    ebx, 0xffff00ff
        inc    ecx
        or     bx, bx
        je     loc_1_1669
        cmp    bx, 0x40
        jae    loc_1_1669
        lea    edi, [ebp - 0x6c]
        or     bx, bx
        je     loc_1_15BA
loc_1_15B0:
        mov    al, byte ptr [ecx]
        mov    byte ptr [edi], al
        inc    ecx
        inc    edi
        dec    bx
        jne    loc_1_15B0
loc_1_15BA:
        mov    byte ptr [edi], 0
        inc    edi
        cmp    dword ptr [ebp - 4], ecx
        jae    loc_1_15E0
        push   0
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        push   edx
        call   sub_1_11F0
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Bh, 0FFh
loc_1_15E0:
        mov    bl, byte ptr [ecx]
        and    ebx, 0xffff00ff
        inc    ecx
        cmp    bx, 0x20
        jb     loc_1_160A
        push   0
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        push   edx
        call   sub_1_11F0
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_1_160A:
        or     bx, bx
        je     loc_1_165B
        mov    eax, dword ptr [esi + 2]
        or     eax, eax
        je     loc_1_1629
        cmp    eax, 1
        je     loc_1_1648
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_1_1629:
        or     bx, bx
        je     loc_1_165B
        mov    edi, edi
loc_1_1630:
        mov    al, byte ptr [ecx]
        mov    byte ptr [edi], al
        inc    ecx
        inc    edi
        dec    bx
        jne    loc_1_1630
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_1_1648:
        lea    eax, [ebp - 0x28]
        push   eax
        push   ecx
        call   sub_1_0810
        lea    eax, [ebp - 0x28]
        push   eax
        call   sub_1_0320
loc_1_165B:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_1_1669:
        push   0
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        push   edx
        call   sub_1_11F0
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  005h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1690:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x78
        push   ebx
        push   esi
        push   edi
        mov    edx, dword ptr [ebp + 8]
        add    edx, 0x4b
        mov    dword ptr [ebp - 8], edx
        lea    edi, [ebp - 0x78]
        sub    eax, eax
        mov    ecx, 0x10
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    word ptr [ebp - 4], ax
        lea    edi, [ebp - 0x34]
        mov    ecx, 8
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        mov    edx, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 0x10]
        lea    esi, [edx + 4]
        and    ecx, 0xffff
        lea    ecx, [edx + ecx - 4]
        mov    dword ptr [ebp - 0xc], ecx
        mov    cl, byte ptr [edx + 1]
        mov    byte ptr [ebp - 1], cl
        mov    edi, dword ptr [ebp - 0x10]
        nop    
loc_1_16E0:
        cmp    esi, dword ptr [ebp - 0xc]
        jae    loc_1_1920
        mov    cl, byte ptr [esi]
        and    ecx, 0xffff00ff
        mov    bl, byte ptr [esi + 1]
        inc    esi
        and    ebx, 0xffff00ff
        inc    esi
        and    ecx, 0xffff
        cmp    ecx, 1
        je     loc_1_1745
        cmp    ecx, 2
        je     loc_1_1780
        cmp    ecx, 3
        je     loc_1_17E0
        cmp    ecx, 4
        je     loc_1_1832
        cmp    ecx, 5
        je     loc_1_1900
        push   0
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_11F0
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_1_1745:
        lea    ecx, [ebx - 2]
        mov    word ptr [ebp - 4], cx
        or     cx, cx
        je     loc_1_1884
        cmp    cx, 0x40
        jae    loc_1_1884
        lea    edi, [ebp - 0x78]
        or     cx, cx
        je     loc_1_1774
        mov    dx, cx
loc_1_176A:
        mov    cl, byte ptr [esi]
        mov    byte ptr [edi], cl
        inc    esi
        inc    edi
        dec    dx
        jne    loc_1_176A
loc_1_1774:
        mov    byte ptr [edi], 0
        inc    edi
        jmp    loc_1_16E0
        db  08Dh, 049h, 000h
loc_1_1780:
        lea    eax, [ebx - 2]
        or     ax, ax
        je     loc_1_17CA
        cmp    ax, 0x20
        jae    loc_1_18A2
        mov    edx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [edx + 2]
        or     ecx, ecx
        je     loc_1_17A3
        cmp    ecx, 1
        je     loc_1_17B7
        jmp    loc_1_17CA
loc_1_17A3:
        or     ax, ax
        je     loc_1_17CA
        mov    dx, ax
loc_1_17AB:
        mov    cl, byte ptr [esi]
        mov    byte ptr [edi], cl
        inc    esi
        inc    edi
        dec    dx
        jne    loc_1_17AB
        jmp    loc_1_17CA
loc_1_17B7:
        lea    eax, [ebp - 0x34]
        push   eax
        push   esi
        call   sub_1_0810
        lea    eax, [ebp - 0x34]
        push   eax
        call   sub_1_0320
loc_1_17CA:
        mov    ecx, ebx
        and    ecx, 0xffff
        sub    ecx, 2
        add    esi, ecx
        jmp    loc_1_16E0
        db  08Dh, 064h, 024h, 000h
loc_1_17E0:
        cmp    bx, 4
        jb     loc_1_18C0
        mov    dx, word ptr [esi]
        mov    ch, dl
        mov    cl, dh
        mov    edx, dword ptr [ebp - 8]
        mov    word ptr [edx + 6], cx
        mov    edx, ebx
        and    edx, 0xffff
        sub    edx, 2
        add    esi, edx
        and    ecx, 0xffff
        cmp    ecx, 1
        je     loc_1_16E0
        push   0
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_11F0
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_1_1832:
        cmp    bx, 4
        jb     loc_1_18E0
        mov    dx, word ptr [esi]
        mov    ch, dl
        mov    cl, dh
        mov    edx, dword ptr [ebp - 8]
        mov    word ptr [edx + 8], cx
        mov    edx, ebx
        and    edx, 0xffff
        sub    edx, 2
        add    esi, edx
        and    ecx, 0xffff
        cmp    ecx, 1
        je     loc_1_16E0
        push   0
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_11F0
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_1_1884:
        push   0
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_11F0
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_1_18A2:
        push   0
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_11F0
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_1_18C0:
        push   0
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_11F0
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Bh, 0FFh
loc_1_18E0:
        push   0
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_11F0
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Bh, 0FFh
loc_1_1900:
        push   3
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_11F0
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Bh, 0FFh
loc_1_1920:
        cmp    word ptr [ebp - 4], 0
        jne    loc_1_1945
        push   0
        mov    eax, dword ptr [ebp + 0x18]
        push   eax
        mov    eax, dword ptr [ebp + 0x14]
        push   eax
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_11F0
        sub    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
loc_1_1945:
        mov    cl, byte ptr [ebp - 1]
        push   ecx
        lea    ecx, [ebp - 0x34]
        push   ecx
        push   eax
        lea    eax, [ebp - 0x78]
        push   eax
        mov    cx, word ptr [ebp - 4]
        push   ecx
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_1450
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x14
        db  08Bh, 0FFh
loc_1_1970:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 0xc]
        cmp    dword ptr [esi + 0x7f], 0
        lea    eax, [esi + 0x4b]
        je     loc_1_19A1
        mov    dword ptr [eax + 0x34], 0
        mov    byte ptr [esi + 0x10], 6
        mov    byte ptr [esi + 0x11], 5
        push   esi
        call   sub_1_1AD0
        add    esp, 4
        push   esi
        call   sub_1_1AF0
        add    esp, 4
loc_1_19A1:
        pop    esi
        pop    ebp
        ret    0x10
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
loc_1_19B0:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [esi + 4]
        mov    eax, dword ptr [eax + 0xc]
        mov    dword ptr [eax + 0xd0], 0x801
        mov    dword ptr [eax + 0xd4], 0
        mov    dword ptr [eax + 0xdc], 0
        mov    dword ptr [eax + 0xe0], 0
        mov    eax, dword ptr [ebp + 0xc]
        push   esi
        mov    dword ptr [eax + 8], 0x801
        mov    byte ptr [esi + 0x11], 9
        call   sub_1_1AD0
        add    esp, 4
        push   esi
        call   sub_1_1AF0
        add    esp, 4
        pop    esi
        pop    ebp
        ret    8
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_1_1A10:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        VxDCall  Dev_0499.Service_0000h          ; cd2000009904
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret    
        db  08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1A30:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   dword ptr [ebp + 8]
        VxDCall  Dev_0499.Service_0001h          ; cd2001009904
        add    esp, 4
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret    
        db  08Dh, 064h, 024h, 000h
sub_1_1A50:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        VxDCall  Dev_0499.Service_0002h          ; cd2002009904
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret    
        db  090h
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        VxDCall  Dev_0499.Service_0004h          ; cd2004009904
        add    esp, 8
        mov    dword ptr [ebp - 4], eax
        mov    eax, dword ptr [ebp - 4]
        mov    esp, ebp
        pop    ebp
        ret    
        db  090h
        push   ebp
        mov    ebp, esp
        push   word ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        VxDCall  Dev_0499.Service_0005h          ; cd2005009904
        add    esp, 8
        pop    ebp
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
        push   ebp
        mov    ebp, esp
        push   word ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        VxDCall  Dev_0499.Service_0006h          ; cd2006009904
        add    esp, 8
        pop    ebp
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1AD0:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 8]
        VxDCall  Dev_0499.Service_0007h          ; cd2007009904
        add    esp, 4
        pop    ebp
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1AF0:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 8]
        VxDCall  Dev_0499.Service_0008h          ; cd2008009904
loc_1_1AFC:
        add    esp, 4
        pop    ebp
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_1_1B10:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 8]
        VxDCall  Dev_0499.Service_0009h          ; cd2009009904
        add    esp, 4
        pop    ebp
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 010h, 020h, 030h, 004h, 014h, 024h, 034h, 008h, 018h, 028h, 038h, 00Ch
        db  01Ch, 02Ch, 03Ch, 000h, 020h, 010h, 030h, 008h, 028h, 018h, 038h, 004h, 024h, 014h, 034h, 00Ch
        db  02Ch, 01Ch, 03Ch
loc_1_1B54:
        cdq    
        inc    eax
        pop    ss
        and    bl, dl
        leave  
        or     bl, dh
        or     byte ptr [ecx + 0x79d4da4], ch
        push   2
        inc    edi
        pop    esi
        push   esp
        fsubp  st(4)
        scasb  al, byte ptr es:[edi]
        sti    
        sub    esi, esi
        jge    loc_1_1AFC
        mov    word ptr [esi - 0x52], fs
        db  0BAh, 000h
loc_1_1B74:
        rol    byte ptr [edx + 0x1d29b6de], cl
        db  0DFh, 0D5h, 0C5h, 0DDh, 0ACh, 0BFh, 095h, 097h, 0EAh, 092h, 0F5h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x94  R-X
        push   ebp
        mov    ebp, esp
        cmp    dword ptr [0x1b30], 0         ; -> obj1:0x1B30
        je     loc_2_0015
        mov    eax, 1
        pop    ebp
        ret    4
loc_2_0015:
        mov    eax, dword ptr [ebp + 8]
        push   eax
        call   sub_1_0F80
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        mov    dword ptr [0x1b30], 1         ; -> obj1:0x1B30
        pop    ebp
        ret    4
        db  08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
sub_2_0040:
        mov    eax, 1
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_0050:
        mov    eax, 1
        ret    4
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_0060:
        push   1
        call   0
        ret    4
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h
sub_2_0070:
        push   0
        call   0
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h
sub_2_0080:
        mov    dword ptr [0x1b30], 0         ; -> obj1:0x1B30
        mov    eax, 1
        ret    
sub_2_0090:
        ret    
        db  030h, 045h, 06Eh
VxD__LTEXT ENDS

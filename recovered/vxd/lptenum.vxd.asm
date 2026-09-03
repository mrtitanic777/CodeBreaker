; LPTENUM -- reconstructed 32-bit disassembly (VxD / LE format)
; 4 objects, 1614 instructions recovered by recursive descent

Declare_Virtual_Device LPTENUM, 4, 0, LPTENUM_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 080000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x5e8  R-X
LPTENUM_Control:
        cmp    eax, 0x1b
        jne    loc_1_000E
        call   sub_4_03D0
        cmp    eax, 1
        ret    
loc_1_000E:
        cmp    eax, 0x1c
        jne    loc_1_001C
        call   sub_4_03D6
        cmp    eax, 1
        ret    
loc_1_001C:
        cmp    eax, 0x22
        jne    loc_1_002A
        push   edx
        push   ebx
        call   sub_4_03DC
        stc    
        ret    
loc_1_002A:
        clc    
        ret    
        db  000h, 000h, 000h, 000h
sub_1_0030:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        movzx  edi, word ptr [esp + 0x14]
        lea    esi, [edi + 1]
        add    edi, 2
        push   edi
        call   sub_1_0440
        add    esp, 4
        and    al, 0x40
        mov    bl, al
        or     al, 0xc
        mov    byte ptr [esp + 0xc], al
        push   dword ptr [esp + 0xc]
        push   edi
        call   sub_1_04DE
        add    esp, 8
        cmp    dword ptr [esp + 0x18], 0
        push   0x23
        je     loc_1_00A6
        push   0x10
        push   0x50
        push   esi
        call   sub_1_03E8
        add    esp, 0x10
        or     eax, eax
        jne    loc_1_00B7
loc_1_007B:
        or     bl, 8
        push   ebx
        push   edi
        call   sub_1_04DE
        add    esp, 8
        push   ebx
        push   edi
        call   sub_1_04DE
        add    esp, 8
        push   dword ptr [esp + 0xc]
        push   edi
        call   sub_1_04DE
        add    esp, 8
loc_1_009F:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
loc_1_00A6:
        push   0
        push   0x50
        push   esi
        call   sub_1_03E8
        add    esp, 0x10
        or     eax, eax
        je     loc_1_007B
loc_1_00B7:
        mov    al, bl
        or     al, 0xe
        push   eax
        push   edi
        call   sub_1_04DE
        add    esp, 8
        push   0x23
        push   0x40
        push   0x40
        push   esi
        call   sub_1_03E8
        add    esp, 0x10
        or     eax, eax
        je     loc_1_007B
        push   dword ptr [esp + 0xc]
        push   edi
        call   sub_1_04DE
        add    esp, 8
        push   0x23
        push   0x80
        push   0x80
        push   esi
        call   sub_1_03E8
        add    esp, 0x10
        jmp    loc_1_009F
sub_1_00FC:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        movzx  edi, word ptr [esp + 0x1c]
        lea    ebp, [edi + 1]
        lea    esi, [edi + 2]
        push   esi
        call   sub_1_0440
        add    esp, 4
        and    al, 0x40
        mov    bl, al
        or     al, 0xc
        mov    byte ptr [esp + 0x14], al
        push   dword ptr [esp + 0x14]
        push   esi
        call   sub_1_04DE
        add    esp, 8
        push   dword ptr [esp + 0x20]
        push   edi
        call   sub_1_04DE
        add    esp, 8
        mov    al, bl
        or     al, 6
        push   eax
        push   esi
        call   sub_1_04DE
        add    esp, 8
        push   0x23
        push   0x38
        push   0x78
        push   ebp
        call   sub_1_03E8
        add    esp, 0x10
        or     eax, eax
        jne    loc_1_0173
        push   dword ptr [esp + 0x14]
        push   esi
        call   sub_1_04DE
        add    esp, 8
        mov    eax, 0x100000
        jmp    loc_1_0233
loc_1_0173:
        mov    al, bl
        or     al, 7
        mov    byte ptr [esp + 0x10], al
        push   dword ptr [esp + 0x10]
        push   esi
        call   sub_1_04DE
        add    esp, 8
        or     bl, 4
        push   dword ptr [esp + 0x10]
        push   esi
        call   sub_1_04DE
        add    esp, 8
        push   ebx
        push   esi
        call   sub_1_04DE
        add    esp, 8
        push   ebx
        push   esi
        call   sub_1_04DE
        movzx  eax, byte ptr [esp + 0x28]
        add    esp, 8
        or     eax, eax
        je     loc_1_01CF
        cmp    eax, 4
        je     loc_1_0209
        cmp    eax, 0x10
        je     loc_1_01E9
        cmp    eax, 0x14
        je     loc_1_01E9
loc_1_01C4:
        cmp    byte ptr [esp + 0x20], 0
        jne    loc_1_0223
        push   0
        jmp    loc_1_0225
loc_1_01CF:
        push   0x23
        push   0x40
        push   0x50
        push   ebp
        call   sub_1_03E8
        add    esp, 0x10
        or     eax, eax
        je     loc_1_01E9
        mov    eax, 0x10000
        jmp    loc_1_0233
loc_1_01E9:
        push   0x23
        push   0xd8
        push   0xf8
        push   ebp
        call   sub_1_03E8
        add    esp, 0x10
        or     eax, eax
        je     loc_1_0209
        mov    eax, 0x10000
        jmp    loc_1_0233
loc_1_0209:
        push   0x23
        push   0x50
        push   0x50
        push   ebp
        call   sub_1_03E8
        add    esp, 0x10
        or     eax, eax
        je     loc_1_01C4
        mov    eax, 0x10000
        jmp    loc_1_0233
loc_1_0223:
        push   1
loc_1_0225:
        push   dword ptr [esp + 0x20]
        call   sub_1_0030
        add    esp, 8
        sub    eax, eax
loc_1_0233:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
sub_1_023B:
        push   ebp
        mov    ebp, esp
        push   eax
        mov    al, byte ptr [ebp + 8]
        rcl    al, 1
        jb     loc_1_024D
        bts    ax, 7
        jmp    loc_1_0252
loc_1_024D:
        btr    ax, 7
loc_1_0252:
        shr    al, 4
        mov    ah, byte ptr [ebp + 0xc]
        rcl    ah, 1
        jb     loc_1_0263
        bts    ax, 0xf
        jmp    loc_1_0268
loc_1_0263:
        btr    ax, 0xf
loc_1_0268:
        and    ah, 0xf0
        or     al, ah
        mov    byte ptr [ebp + 0xc], al
        pop    eax
        mov    al, byte ptr [ebp + 0xc]
        pop    ebp
        ret    
sub_1_0276:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        movzx  ebp, word ptr [esp + 0x1c]
        sub    edi, edi
        lea    esi, [ebp + 1]
        add    ebp, 2
        push   ebp
        call   sub_1_0440
        mov    byte ptr [esp + 0x15], al
        add    esp, 4
        test   al, 2
        je     loc_1_02CC
        push   esi
        call   sub_1_03D0
        add    esp, 4
        cmp    eax, edi
        je     loc_1_02B5
        mov    eax, dword ptr [esp + 0x28]
        mov    dword ptr [eax], edi
        sub    eax, eax
        jmp    loc_1_03B7
loc_1_02B5:
        and    byte ptr [esp + 0x11], 0xf0
        mov    al, byte ptr [esp + 0x11]
        or     al, 4
        push   eax
        push   ebp
        call   sub_1_04DE
        add    esp, 8
        jmp    loc_1_02D1
loc_1_02CC:
        and    byte ptr [esp + 0x11], 0xf0
loc_1_02D1:
        mov    al, byte ptr [esp + 0x11]
        or     al, 6
        mov    byte ptr [esp + 0x14], al
loc_1_02DB:
        push   dword ptr [esp + 0x14]
        push   ebp
        call   sub_1_04DE
        add    esp, 8
        push   0x23
        push   0
        push   0x40
        push   esi
        call   sub_1_03E8
        add    esp, 0x10
        or     eax, eax
        je     loc_1_03AC
        push   esi
        call   sub_1_0440
        mov    byte ptr [esp + 0x16], al
        mov    bl, byte ptr [esp + 0x15]
        add    esp, 4
        or     bl, 4
        push   ebx
        push   ebp
        call   sub_1_04DE
        add    esp, 8
        push   0x23
        push   0x40
        push   0x40
        push   esi
        call   sub_1_03E8
        add    esp, 0x10
        or     eax, eax
        je     loc_1_03AC
        push   dword ptr [esp + 0x14]
        push   ebp
        call   sub_1_04DE
        add    esp, 8
        push   0x23
        push   0
        push   0x40
        push   esi
        call   sub_1_03E8
        add    esp, 0x10
        or     eax, eax
        je     loc_1_03AC
        push   esi
        call   sub_1_0440
        mov    byte ptr [esp + 0x17], al
        add    esp, 4
        push   ebx
        push   ebp
        call   sub_1_04DE
        add    esp, 8
        push   dword ptr [esp + 0x13]
        inc    edi
        push   dword ptr [esp + 0x16]
        call   sub_1_023B
        mov    ecx, dword ptr [esp + 0x28]
        add    esp, 8
        mov    byte ptr [ecx], al
        inc    dword ptr [esp + 0x20]
        push   0x23
        push   0x40
        push   0x40
        push   esi
        call   sub_1_03E8
        add    esp, 0x10
        or     eax, eax
        je     loc_1_03AC
        push   esi
        call   sub_1_03D0
        add    esp, 4
        or     eax, eax
        jne    loc_1_03AC
        cmp    edi, dword ptr [esp + 0x24]
        jb     loc_1_02DB
loc_1_03AC:
        mov    eax, dword ptr [esp + 0x28]
        mov    dword ptr [eax], edi
        mov    eax, 1
loc_1_03B7:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  0B8h, 001h, 000h, 000h, 000h, 0C3h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h
sub_1_03D0:
        push   dword ptr [esp + 4]
        call   sub_1_0440
        add    esp, 4
        test   al, 8
        mov    eax, 1
        jne    loc_1_03E7
        sub    eax, eax
loc_1_03E7:
        ret    
sub_1_03E8:
        push   ebx
        push   esi
        push   edi
        push   0x5e0                         ; -> obj1:0x05E0
        call   sub_2_0000
        mov    esi, dword ptr [esp + 0x14]
        mov    bl, byte ptr [esp + 0x18]
        mov    edi, dword ptr [esp + 0x20]
        add    esp, 4
loc_1_0404:
        push   0x5e4                         ; -> obj1:0x05E4
        call   sub_2_0000
        add    esp, 4
        push   esi
        call   sub_1_0440
        add    esp, 4
        and    al, bl
        cmp    al, byte ptr [esp + 0x18]
        je     loc_1_0437
        mov    eax, dword ptr [0x5e4]        ; -> obj1:0x05E4
        sub    eax, dword ptr [0x5e0]        ; -> obj1:0x05E0
        cmp    eax, edi
        jl     loc_1_0404
        sub    eax, eax
loc_1_0433:
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_0437:
        mov    eax, 1
        jmp    loc_1_0433
        db  000h, 000h
sub_1_0440:
        push   ebp
        mov    ebp, esp
        xor    eax, eax
        push   edx
        mov    dx, word ptr [ebp + 8]
        in     al, dx
        pop    edx
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        xor    edx, edx
        xor    ebx, ebx
        mov    dx, word ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 0x10]
loc_1_0461:
        in     al, dx
        mov    byte ptr [ebx + esi], al
        inc    ebx
        loop   loc_1_0461
        pop    ebx
        pop    esi
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        xor    eax, eax
        push   edx
        mov    dx, word ptr [ebp + 8]
        in     ax, dx
        pop    edx
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        xor    edx, edx
        xor    ebx, ebx
        mov    dx, word ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 0x10]
        shr    ecx, 1
loc_1_0490:
        in     ax, dx
        mov    byte ptr [ebx + esi], al
        inc    ebx
        mov    byte ptr [ebx + esi], ah
        inc    ebx
        loop   loc_1_0490
        pop    ebx
        pop    esi
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        xor    eax, eax
        push   edx
        mov    dx, word ptr [ebp + 8]
        in     eax, dx
        pop    edx
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        xor    edx, edx
        xor    ebx, ebx
        mov    dx, word ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 0x10]
        shr    ecx, 2
loc_1_04C4:
        in     eax, dx
        mov    byte ptr [ebx + esi], al
        inc    ebx
        mov    byte ptr [ebx + esi], ah
        inc    ebx
        shr    eax, 0x10
        mov    byte ptr [ebx + esi], al
        inc    ebx
        mov    byte ptr [ebx + esi], ah
        inc    ebx
        loop   loc_1_04C4
        pop    ebx
        pop    esi
        leave  
        ret    
sub_1_04DE:
        push   ebp
        mov    ebp, esp
        push   edx
        xor    edx, edx
        mov    dx, word ptr [ebp + 8]
        mov    al, byte ptr [ebp + 0xc]
        out    dx, al
        pop    edx
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        xor    edx, edx
        xor    ebx, ebx
        mov    dx, word ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 0x10]
loc_1_0502:
        mov    al, byte ptr [ebx + esi]
        out    dx, al
        inc    ebx
        loop   loc_1_0502
        pop    ebx
        pop    esi
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        push   edx
        xor    edx, edx
        mov    dx, word ptr [ebp + 8]
        mov    ax, word ptr [ebp + 0xc]
        out    dx, ax
        pop    edx
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        xor    edx, edx
        xor    ebx, ebx
        mov    dx, word ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 0x10]
        shr    ecx, 1
loc_1_0535:
        mov    al, byte ptr [ebx + esi]
        inc    ebx
        mov    ah, byte ptr [ebx + esi]
        inc    ebx
        out    dx, ax
        loop   loc_1_0535
        pop    ebx
        pop    esi
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        push   edx
        xor    edx, edx
        mov    dx, word ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0xc]
        out    dx, eax
        pop    edx
        leave  
        ret    
        push   ebp
        mov    ebp, esp
        push   esi
        push   ebx
        xor    edx, edx
        xor    ebx, ebx
        mov    dx, word ptr [ebp + 8]
        mov    esi, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 0x10]
        shr    ecx, 2
loc_1_056C:
        mov    al, byte ptr [ebx + esi]
        inc    ebx
        mov    ah, byte ptr [ebx + esi]
        inc    ebx
        shl    eax, 0x10
        mov    al, byte ptr [ebx + esi]
        inc    ebx
        mov    ah, byte ptr [ebx + esi]
        inc    ebx
        ror    eax, 0x10
        out    dx, eax
        loop   loc_1_056C
        pop    ebx
        pop    esi
        leave  
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  4, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'LPTENUM '                  ; DDB_Name
        dd  080000000h                  ; DDB_Init_Order
        dd  LPTENUM_Control          ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x10  R-X
sub_2_0000:
        push   ebp
        mov    ebp, esp
        VMMCall  VMM.Get_System_Time             ; cd203f000100
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ecx], eax
        pop    ebp
        ret    
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 3  vsize=0x119  RW-
        db  045h, 06Eh, 075h, 06Dh, 05Ch, 04Ch, 050h, 054h, 045h, 04Eh, 055h, 04Dh, 000h, 000h, 000h, 000h
        db  04Dh, 06Fh, 064h, 065h, 06Dh, 000h, 000h, 000h, 050h, 06Fh, 072h, 074h, 04Eh, 061h, 06Dh, 065h
        db  000h, 000h, 000h, 000h, 05Ch, 000h, 000h, 000h, 043h, 06Ch, 061h, 073h, 073h, 000h, 000h, 000h
        db  043h, 053h, 043h, 06Fh, 06Eh, 066h, 069h, 067h, 046h, 06Ch, 061h, 067h, 073h, 000h, 000h, 000h
        db  04Ch, 050h, 054h, 045h, 04Eh, 055h, 04Dh, 05Ch, 000h, 000h, 000h, 000h, 025h, 034h, 058h, 000h
        db  04Ch, 050h, 054h, 045h, 04Eh, 055h, 04Dh, 000h, 04Dh, 06Fh, 064h, 065h, 06Ch, 000h, 000h, 000h
        db  04Dh, 061h, 06Eh, 075h, 066h, 061h, 063h, 074h, 075h, 072h, 065h, 072h, 000h, 000h, 000h, 000h
        db  020h, 000h, 000h, 000h, 044h, 065h, 076h, 069h, 063h, 065h, 044h, 065h, 073h, 063h, 000h, 000h
        db  043h, 06Fh, 06Dh, 070h, 061h, 074h, 069h, 062h, 06Ch, 065h, 049h, 044h, 073h, 000h, 000h, 000h
        db  048h, 061h, 072h, 064h, 077h, 061h, 072h, 065h, 049h, 044h, 000h, 000h, 04Dh, 041h, 04Eh, 055h
        db  046h, 041h, 043h, 054h, 055h, 052h, 045h, 052h, 03Ah, 000h, 000h, 000h, 04Dh, 046h, 047h, 03Ah
        db  000h, 000h, 000h, 000h, 04Dh, 04Fh, 044h, 045h, 04Ch, 03Ah, 000h, 000h, 04Dh, 044h, 04Ch, 03Ah
        db  000h, 000h, 000h, 000h, 043h, 04Ch, 041h, 053h, 053h, 03Ah, 000h, 000h, 043h, 04Ch, 053h, 03Ah
        db  000h, 000h, 000h, 000h, 043h, 04Fh, 04Dh, 050h, 041h, 054h, 049h, 042h, 04Ch, 045h, 020h, 049h
        db  044h, 03Ah, 000h, 000h, 043h, 049h, 044h, 03Ah, 000h, 000h, 000h, 000h, 044h, 045h, 053h, 043h
        db  052h, 049h, 050h, 054h, 049h, 04Fh, 04Eh, 03Ah, 000h, 000h, 000h, 000h, 044h, 045h, 053h, 03Ah
        db  000h, 000h, 000h, 000h, 041h, 055h, 054h, 04Fh, 04Dh, 041h, 054h, 049h, 043h, 020h, 049h, 044h
        db  03Ah, 000h, 000h, 000h, 041h, 049h, 044h, 03Ah, 000h
VxD__LDATA ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0xd02  R-X
loc_4_0000:
        push   esi
        cmp    dword ptr [esp + 8], 5
        mov    eax, 1
        jne    loc_4_003E
        mov    esi, dword ptr [esp + 0x10]
        push   0
        push   esi
        call   sub_4_0CE4
        add    esp, 8
        mov    ecx, esi
        call   sub_4_0040
        push   0
        push   esi
        call   sub_4_0CEC
        add    esp, 8
        push   0x10                          ; -> obj3:0x0010
        push   esi
        call   sub_4_019A
        add    esp, 8
        sub    eax, eax
loc_4_003E:
        pop    esi
        ret    
sub_4_0040:
        sub    esp, 0x110
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, ecx
        push   2
        lea    eax, [esp + 0x18]
        push   esi
        push   eax
        call   sub_4_0CCC
        add    esp, 0xc
        or     eax, eax
        je     loc_4_0065
        jmp    loc_4_0126
loc_4_0065:
        mov    eax, dword ptr [esp + 0x14]
        mov    dword ptr [esp + 0x10], eax
        push   0
        push   0
        push   2
        lea    eax, [esp + 0x1c]
        push   dword ptr [esp + 0x20]
        push   eax
        call   sub_4_0CD4
        add    esp, 0x14
        or     eax, eax
        jne    loc_4_0126
        mov    eax, dword ptr [esp + 0x10]
        push   0
        movzx  edi, word ptr [eax + 4]
        mov    dword ptr [esp + 0x1c], 0x104
        lea    eax, [esp + 0x1c]
        push   eax
        lea    eax, [esp + 0x24]
        push   eax
        push   1
        push   0x18                          ; -> obj3:0x0018
        push   0
        push   esi
        call   sub_4_0CF4
        add    esp, 0x1c
        or     eax, eax
        je     loc_4_00C0
        jmp    loc_4_0126
loc_4_00C0:
        lea    eax, [esp + 0x1c]
        push   eax
        call   sub_4_0C8C
        add    esp, 4
        mov    ebx, eax
        or     ebx, ebx
        jne    loc_4_00D5
        jmp    loc_4_0126
loc_4_00D5:
        lea    eax, [esp + 0x1c]
        push   eax
        call   sub_4_0C94
        add    esp, 4
        mov    ebp, eax
        or     ebp, ebp
        jne    loc_4_00EA
        jmp    loc_4_0126
loc_4_00EA:
        push   0
        push   0
        push   0x144                         ; -> loc_4_0144
        push   ebp
        push   1
        call   ebx
        add    esp, 0x14
        or     eax, eax
        jne    loc_4_0101
        jmp    loc_4_0126
loc_4_0101:
        mov    edx, edi
        mov    ecx, esi
        call   sub_4_0530
        or     eax, eax
        mov    esi, 1
        jne    loc_4_0115
        sub    esi, esi
loc_4_0115:
        push   0x144                         ; -> loc_4_0144
        push   ebp
        push   3
        call   ebx
        add    esp, 0xc
        mov    eax, esi
        jmp    loc_4_0128
loc_4_0126:
        sub    eax, eax
loc_4_0128:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x110
        ret    
sub_4_0133:
        push   1
        push   0                             ; -> loc_4_0000
        push   ecx
        call   sub_4_0CC4
        add    esp, 0xc
        ret    
loc_4_0144:
        sub    eax, eax
        ret    
sub_4_0147:
        push   ebx
        push   esi
        mov    eax, dword ptr [esp + 0xc]
        mov    esi, dword ptr [esp + 0x10]
loc_4_0151:
        mov    cl, byte ptr [eax]
        cmp    cl, 0x61
        jl     loc_4_0165
        cmp    cl, 0x7a
        jg     loc_4_0165
        movsx  edx, cl
        sub    edx, 0x20
        jmp    loc_4_0168
loc_4_0165:
        movsx  edx, cl
loc_4_0168:
        mov    bl, byte ptr [esi]
        cmp    bl, 0x61
        jl     loc_4_017C
        cmp    bl, 0x7a
        jg     loc_4_017C
        movsx  ecx, bl
        sub    ecx, 0x20
        jmp    loc_4_017F
loc_4_017C:
        movsx  ecx, bl
loc_4_017F:
        cmp    edx, ecx
        jne    loc_4_018F
        or     bl, bl
        je     loc_4_018B
        inc    eax
        inc    esi
        jmp    loc_4_0151
loc_4_018B:
        sub    eax, eax
        jmp    loc_4_0197
loc_4_018F:
        movsx  eax, byte ptr [eax]
        movsx  ecx, byte ptr [esi]
        sub    eax, ecx
loc_4_0197:
        pop    esi
        pop    ebx
        ret    
sub_4_019A:
        sub    esp, 0xe8
        push   ebx
        push   esi
        push   edi
        push   ebp
        lea    eax, [esp + 0x1c]
        push   eax
        push   0                             ; -> obj3:0x0000
        push   0x80000002
        call   sub_4_0C6C
        add    esp, 0xc
        or     eax, eax
        jne    loc_4_039A
        sub    esi, esi
        mov    edi, dword ptr [esp + 0xfc]
        mov    ebx, dword ptr [esp + 0x100]
loc_4_01D3:
        mov    dword ptr [esp + 0x10], 0x65
        mov    eax, esi
        lea    ecx, [esp + 0x90]
        push   0x65
        push   ecx
        push   eax
        inc    esi
        push   dword ptr [esp + 0x28]
        call   sub_4_0C7C
        add    esp, 0x10
        or     eax, eax
        jne    loc_4_038E
        push   0
        lea    eax, [esp + 0x2c]
        push   0x64
        push   eax
        push   edi
        call   sub_4_0CB4
        add    esp, 0x10
        or     eax, eax
        jne    loc_4_038E
        push   0x64
        lea    ecx, [esp + 0x2c]
        push   0x26
        mov    edx, 0x5c
        call   sub_4_0505
        mov    edx, 0x24                     ; -> obj3:0x0024
        lea    ecx, [esp + 0x90]
        call   sub_4_04A5
        lea    edx, [esp + 0x28]
        lea    ecx, [esp + 0x90]
        call   sub_4_04A5
        lea    eax, [esp + 0x14]
        push   eax
        lea    eax, [esp + 0x94]
        push   eax
        push   dword ptr [esp + 0x24]
        call   sub_4_0C6C
        add    esp, 0xc
        or     eax, eax
        jne    loc_4_01D3
        mov    dword ptr [esp + 0x10], 0x65
        lea    eax, [esp + 0x10]
        push   eax
        lea    eax, [esp + 0x2c]
        push   eax
        push   0
        push   0
        push   0x28                          ; -> obj3:0x0028
        push   dword ptr [esp + 0x28]
        call   sub_4_0C84
        add    esp, 0x18
        or     eax, eax
        jne    loc_4_037D
        lea    eax, [esp + 0x28]
        push   ebx
        push   eax
        call   sub_4_0147
        add    esp, 8
        or     eax, eax
        jne    loc_4_037D
        mov    ebp, 1
        mov    byte ptr [esp + 0x28], 0
        mov    edx, 0                        ; -> obj3:0x0000
        lea    ecx, [esp + 0x28]
        call   sub_4_04A5
        mov    edx, 0x24                     ; -> obj3:0x0024
        lea    ecx, [esp + 0x28]
        call   sub_4_04A5
        lea    edx, [esp + 0x90]
        lea    ecx, [esp + 0x28]
        call   sub_4_04A5
        lea    eax, [esp + 0x18]
        push   eax
        lea    eax, [esp + 0x2c]
        push   eax
        push   0x80000005
        call   sub_4_0C6C
        add    esp, 0xc
        or     eax, eax
        jne    loc_4_0341
        mov    dword ptr [esp + 0x10], 4
        lea    eax, [esp + 0x10]
        push   eax
        lea    eax, [esp + 0x24]
        push   eax
        push   0
        push   0
        push   0x30                          ; -> obj3:0x0030
        push   dword ptr [esp + 0x2c]
        call   sub_4_0C84
        add    esp, 0x18
        or     eax, eax
        jne    loc_4_0335
        test   byte ptr [esp + 0x20], 2
        je     loc_4_0335
        sub    ebp, ebp
loc_4_0335:
        push   dword ptr [esp + 0x18]
        call   sub_4_0C74
        add    esp, 4
loc_4_0341:
        or     ebp, ebp
        je     loc_4_037D
        mov    byte ptr [esp + 0x28], 0
        mov    edx, 0x40                     ; -> obj3:0x0040
        lea    ecx, [esp + 0x28]
        call   sub_4_04A5
        lea    edx, [esp + 0x90]
        lea    ecx, [esp + 0x28]
        call   sub_4_04A5
        push   0
        lea    eax, [esp + 0x2c]
        push   edi
        push   eax
        lea    eax, [esp + 0x30]
        push   eax
        call   sub_4_0CBC
        add    esp, 0x10
loc_4_037D:
        push   dword ptr [esp + 0x14]
        call   sub_4_0C74
        add    esp, 4
        jmp    loc_4_01D3
loc_4_038E:
        push   dword ptr [esp + 0x1c]
        call   sub_4_0C74
        add    esp, 4
loc_4_039A:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xe8
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 066h, 08Bh, 0C1h, 066h, 0C1h
        db  0E8h, 008h, 066h, 0C1h, 0E1h, 008h, 066h, 00Bh, 0C8h, 066h, 08Bh, 0C1h, 0C3h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_4_03D0:
        mov    eax, 1
        ret    
sub_4_03D6:
        mov    eax, 1
        ret    
sub_4_03DC:
        cmp    dword ptr [esp + 8], 0
        mov    eax, 1
        jne    loc_4_03F1
        mov    ecx, dword ptr [esp + 4]
        call   sub_4_0133
loc_4_03F1:
        ret    8
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_4_0400:
        push   edi
        or     ecx, ecx
        mov    eax, 0
        je     loc_4_041A
        mov    edi, ecx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        lea    eax, [ecx - 1]
loc_4_041A:
        pop    edi
        ret    
        db  02Bh, 0C0h, 03Bh, 0C8h, 075h, 002h, 0EBh, 010h, 080h, 039h, 000h, 074h, 00Bh, 038h, 011h, 074h
        db  00Ch, 041h, 040h, 080h, 039h, 000h, 075h, 0F5h, 0B8h, 0FFh, 0FFh, 0FFh, 0FFh, 0C3h
sub_4_043A:
        mov    eax, ecx
        or     eax, eax
        jne    loc_4_0442
        jmp    loc_4_0451
loc_4_0442:
        cmp    byte ptr [eax], 0
        je     loc_4_0451
loc_4_0447:
        cmp    byte ptr [eax], dl
        je     loc_4_0453
        inc    eax
        cmp    byte ptr [eax], 0
        jne    loc_4_0447
loc_4_0451:
        sub    eax, eax
loc_4_0453:
        ret    
sub_4_0454:
        push   esi
        push   edi
        mov    esi, ecx
        or     esi, esi
        je     loc_4_0480
        or     edx, edx
        je     loc_4_0480
        mov    eax, dword ptr [esp + 0xc]
        mov    ecx, eax
        shr    ecx, 2
        mov    edi, edx
        repe cmpsd dword ptr [esi], dword ptr es:[edi]
        jne    loc_4_0476
        mov    ecx, eax
        and    ecx, 3
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
loc_4_0476:
        mov    eax, 0
        sete   al
        jmp    loc_4_0482
loc_4_0480:
        sub    eax, eax
loc_4_0482:
        pop    edi
        pop    esi
        ret    4
        db  00Bh, 0C9h, 074h, 019h, 080h, 039h, 000h, 074h, 014h, 08Ah, 001h, 03Ch, 061h, 07Ch, 008h, 03Ch
        db  07Ah, 07Fh, 004h, 02Ch, 020h, 088h, 001h, 041h, 080h, 039h, 000h, 075h, 0ECh, 0C3h
sub_4_04A5:
        push   ebx
        push   esi
        push   edi
        mov    ebx, ecx
        mov    edi, edx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    esi, edi
        mov    edx, ecx
        mov    edi, ebx
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
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_4_04DB:
        push   ebx
        push   esi
        push   edi
        mov    ebx, ecx
        mov    edi, edx
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        mov    esi, edi
        mov    edi, ebx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_4_0505:
        push   ebx
        push   esi
        push   edi
        sub    esi, esi
        cmp    ecx, esi
        je     loc_4_0528
        mov    bl, byte ptr [esp + 0x10]
        mov    eax, dword ptr [esp + 0x14]
loc_4_0516:
        mov    edi, esi
        inc    esi
        cmp    edi, eax
        jge    loc_4_0528
        cmp    byte ptr [ecx], dl
        jne    loc_4_0523
        mov    byte ptr [ecx], bl
loc_4_0523:
        inc    ecx
        or     ecx, ecx
        jne    loc_4_0516
loc_4_0528:
        pop    edi
        pop    esi
        pop    ebx
        ret    8
        db  000h, 000h
sub_4_0530:
        sub    esp, 0x158
        push   esi
        push   edi
        mov    dword ptr [esp + 0x14], 0
        mov    dword ptr [esp + 0xc], 0
        mov    dword ptr [esp + 8], 0
        mov    dword ptr [esp + 0x20], 0
        mov    esi, ecx
        mov    dword ptr [esp + 0x18], 0
        mov    dword ptr [esp + 0x1c], 0
        mov    dword ptr [esp + 0x24], 0
        lea    eax, [esp + 0x14]
        mov    ecx, edx
        mov    edx, eax
        call   sub_4_08E2
        or     eax, eax
        jne    loc_4_058A
        sub    eax, eax
        jmp    loc_4_08D9
loc_4_058A:
        lea    eax, [esp + 0x1c]
        push   dword ptr [esp + 0x14]
        push   eax
        lea    eax, [esp + 0x20]
        push   eax
        lea    eax, [esp + 0x30]
        push   eax
        lea    eax, [esp + 0x30]
        lea    edx, [esp + 0x18]
        lea    ecx, [esp + 0x1c]
        push   eax
        call   sub_4_09E8
        cmp    dword ptr [esp + 0xc], 0
        je     loc_4_08C0
        cmp    dword ptr [esp + 8], 0
        je     loc_4_08C0
        push   0
        lea    eax, [esp + 0x100]
        push   0x64
        push   eax
        push   esi
        call   sub_4_0CB4
        add    esp, 0x10
        or     eax, eax
        je     loc_4_05E3
        jmp    loc_4_08C0
loc_4_05E3:
        push   0x64
        lea    ecx, [esp + 0x100]
        push   0x26
        mov    edx, 0x5c
        call   sub_4_0505
        lea    ecx, [esp + 0x34]
        mov    edx, dword ptr [esp + 0xc]
        call   sub_4_04DB
        lea    ecx, [esp + 0x34]
        mov    edx, dword ptr [esp + 8]
        call   sub_4_04A5
        mov    dword ptr [esp + 0x28], 0
        lea    eax, [esp + 0x28]
        push   0
        lea    ecx, [esp + 0x38]
        push   eax
        call   sub_4_0400
        push   eax
        lea    eax, [esp + 0x40]
        push   eax
        call   sub_4_0CDC
        add    esp, 0x10
        or     eax, eax
        je     loc_4_0644
        mov    dword ptr [esp + 0x28], 0xdead
loc_4_0644:
        push   dword ptr [esp + 0x28]
        lea    eax, [esp + 0x30]
        push   0x4c                          ; -> obj3:0x004C
        push   eax
        call   sub_4_0CAC
        mov    ecx, dword ptr [esp + 0x14]
        add    esp, 0xc
        call   sub_4_0400
        mov    edi, eax
        mov    ecx, dword ptr [esp + 0xc]
        call   sub_4_0400
        add    eax, edi
        cmp    eax, 0x14
        jbe    loc_4_067A
        mov    eax, 0x14
loc_4_067A:
        mov    byte ptr [esp + eax + 0x34], 0
        lea    ecx, [esp + 0x34]
        call   sub_4_0400
        push   eax
        lea    ecx, [esp + 0x38]
        push   0x5f
        mov    edx, 0x20
        call   sub_4_0505
        lea    edx, [esp + 0x2c]
        lea    ecx, [esp + 0x34]
        call   sub_4_04A5
        mov    edx, 0x50                     ; -> obj3:0x0050
        lea    ecx, [esp + 0x98]
        call   sub_4_04DB
        mov    edx, 0x24                     ; -> obj3:0x0024
        lea    ecx, [esp + 0x98]
        call   sub_4_04A5
        lea    edx, [esp + 0x34]
        lea    ecx, [esp + 0x98]
        call   sub_4_04A5
        mov    edx, 0x24                     ; -> obj3:0x0024
        lea    ecx, [esp + 0x98]
        call   sub_4_04A5
        lea    edx, [esp + 0xfc]
        lea    ecx, [esp + 0x98]
        call   sub_4_04A5
        push   0
        lea    eax, [esp + 0x9c]
        push   esi
        push   eax
        lea    eax, [esp + 0x1c]
        push   eax
        call   sub_4_0CBC
        add    esp, 0x10
        cmp    eax, 2
        jne    loc_4_071E
        jmp    loc_4_08C0
loc_4_071E:
        cmp    eax, 0x10
        jne    loc_4_072D
        mov    esi, 1
        jmp    loc_4_08C2
loc_4_072D:
        or     eax, eax
        je     loc_4_0736
        jmp    loc_4_08C0
loc_4_0736:
        cmp    dword ptr [esp + 8], 0
        je     loc_4_0762
        mov    ecx, dword ptr [esp + 8]
        push   0
        call   sub_4_0400
        push   eax
        push   dword ptr [esp + 0x10]
        push   1
        push   0x58                          ; -> obj3:0x0058
        push   0
        push   dword ptr [esp + 0x28]
        call   sub_4_0CFC
        add    esp, 0x1c
loc_4_0762:
        cmp    dword ptr [esp + 0xc], 0
        je     loc_4_078E
        mov    ecx, dword ptr [esp + 0xc]
        push   0
        call   sub_4_0400
        push   eax
        push   dword ptr [esp + 0x14]
        push   1
        push   0x60                          ; -> obj3:0x0060
        push   0
        push   dword ptr [esp + 0x28]
        call   sub_4_0CFC
        add    esp, 0x1c
loc_4_078E:
        cmp    dword ptr [esp + 0x24], 0
        jne    loc_4_0810
        mov    ecx, dword ptr [esp + 8]
        call   sub_4_0400
        mov    esi, eax
        mov    ecx, dword ptr [esp + 0xc]
        call   sub_4_0400
        lea    eax, [esi + eax + 2]
        push   0x201
        push   eax
        call   sub_4_0C9C
        add    esp, 8
        mov    esi, eax
        or     esi, esi
        je     loc_4_0835
        mov    edx, dword ptr [esp + 0xc]
        mov    ecx, esi
        call   sub_4_04DB
        mov    edx, 0x70                     ; -> obj3:0x0070
        mov    ecx, esi
        call   sub_4_04A5
        mov    edx, dword ptr [esp + 8]
        mov    ecx, esi
        call   sub_4_04A5
        push   0
        mov    ecx, esi
        call   sub_4_0400
        push   eax
        push   esi
        push   1
        push   0x74                          ; -> obj3:0x0074
        push   0
        push   dword ptr [esp + 0x28]
        call   sub_4_0CFC
        add    esp, 0x1c
        push   0
        push   esi
        call   sub_4_0CA4
        add    esp, 8
        jmp    loc_4_0835
loc_4_0810:
        mov    ecx, dword ptr [esp + 0x24]
        push   0
        call   sub_4_0400
        push   eax
        push   dword ptr [esp + 0x2c]
        push   1
        push   0x74                          ; -> obj3:0x0074
        push   0
        push   dword ptr [esp + 0x28]
        call   sub_4_0CFC
        add    esp, 0x1c
loc_4_0835:
        cmp    dword ptr [esp + 0x20], 0
        je     loc_4_0861
        mov    ecx, dword ptr [esp + 0x20]
        push   0
        call   sub_4_0400
        push   eax
        push   dword ptr [esp + 0x28]
        push   1
        push   0x28                          ; -> obj3:0x0028
        push   0
        push   dword ptr [esp + 0x28]
        call   sub_4_0CFC
        add    esp, 0x1c
loc_4_0861:
        cmp    dword ptr [esp + 0x1c], 0
        je     loc_4_088D
        mov    ecx, dword ptr [esp + 0x1c]
        push   0
        call   sub_4_0400
        push   eax
        push   dword ptr [esp + 0x24]
        push   1
        push   0x80                          ; -> obj3:0x0080
        push   0
        push   dword ptr [esp + 0x28]
        call   sub_4_0CFC
        add    esp, 0x1c
loc_4_088D:
        cmp    dword ptr [esp + 0x18], 0
        je     loc_4_08B9
        mov    ecx, dword ptr [esp + 0x18]
        push   0
        call   sub_4_0400
        push   eax
        push   dword ptr [esp + 0x20]
        push   1
        push   0x90                          ; -> obj3:0x0090
        push   0
        push   dword ptr [esp + 0x28]
        call   sub_4_0CFC
        add    esp, 0x1c
loc_4_08B9:
        mov    esi, 1
        jmp    loc_4_08C2
loc_4_08C0:
        sub    esi, esi
loc_4_08C2:
        cmp    dword ptr [esp + 0x14], 0
        je     loc_4_08D7
        push   0
        push   dword ptr [esp + 0x18]
        call   sub_4_0CA4
        add    esp, 8
loc_4_08D7:
        mov    eax, esi
loc_4_08D9:
        pop    edi
        pop    esi
        add    esp, 0x158
        ret    
sub_4_08E2:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    edi, edx
        mov    esi, ecx
        mov    dword ptr [esp + 0xc], 0
        push   4
        push   esi
        call   sub_1_00FC
        add    esp, 8
        cmp    eax, 0x10000
        je     loc_4_090B
        jmp    loc_4_09DF
loc_4_090B:
        lea    ebx, [esi + 1]
        push   ebx
        call   sub_1_03D0
        add    esp, 4
        or     eax, eax
        je     loc_4_093A
        lea    edx, [esi + 2]
        mov    al, 0xe6
        out    dx, al
        out    dx, al
        push   0x23
        push   0
        push   8
        push   ebx
        call   sub_1_03E8
        add    esp, 0x10
        or     eax, eax
        jne    loc_4_093A
        jmp    loc_4_09D4
loc_4_093A:
        lea    eax, [esp + 0x10]
        push   eax
        lea    eax, [esp + 0x10]
        push   2
        push   eax
        push   esi
        call   sub_1_0276
        add    esp, 0x10
        or     eax, eax
        jne    loc_4_0955
        jmp    loc_4_09D4
loc_4_0955:
        mov    eax, dword ptr [esp + 0xc]
        xchg   ah, al
        mov    dword ptr [esp + 0xc], eax
        cmp    dword ptr [esp + 0xc], 0
        je     loc_4_09D4
        cmp    dword ptr [esp + 0x10], 2
        jne    loc_4_09D4
        mov    eax, dword ptr [esp + 0xc]
        push   0x201
        inc    eax
        push   eax
        call   sub_4_0C9C
        add    esp, 8
        mov    dword ptr [edi], eax
        mov    ecx, eax
        or     ecx, ecx
        jne    loc_4_098A
        jmp    loc_4_09D4
loc_4_098A:
        lea    eax, [esp + 0x10]
        push   eax
        mov    eax, dword ptr [esp + 0x10]
        sub    eax, 2
        push   eax
        push   ecx
        push   esi
        call   sub_1_0276
        add    esp, 0x10
        or     eax, eax
        push   0
        push   esi
        jne    loc_4_09BB
        call   sub_1_0030
        add    esp, 8
        push   0
        push   dword ptr [edi]
        call   sub_4_0CA4
        jmp    loc_4_09DC
loc_4_09BB:
        call   sub_1_0030
        add    esp, 8
        mov    eax, dword ptr [edi]
        mov    ecx, dword ptr [esp + 0xc]
        mov    byte ptr [eax + ecx], 0
        mov    eax, 1
        jmp    loc_4_09E1
loc_4_09D4:
        push   0
        push   esi
        call   sub_1_0030
loc_4_09DC:
        add    esp, 8
loc_4_09DF:
        sub    eax, eax
loc_4_09E1:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
sub_4_09E8:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 0x10], edx
        mov    esi, ecx
        mov    ebx, dword ptr [esp + 0x28]
        or     ebx, ebx
        je     loc_4_0C60
loc_4_0A01:
        cmp    byte ptr [ebx], 0x20
        jne    loc_4_0A0C
loc_4_0A06:
        inc    ebx
        cmp    byte ptr [ebx], 0x20
        je     loc_4_0A06
loc_4_0A0C:
        mov    edx, 0x3a
        mov    ecx, ebx
        call   sub_4_043A
        mov    edi, eax
        or     edi, edi
        je     loc_4_0C60
        inc    edi
        mov    ecx, edi
        sub    ecx, ebx
        movsx  eax, byte ptr [ebx]
        cmp    eax, 0x41
        je     loc_4_0A65
        cmp    eax, 0x43
        je     loc_4_0AC6
        cmp    eax, 0x44
        je     loc_4_0B6A
        cmp    eax, 0x4d
        je     loc_4_0BCB
        mov    edx, 0x3b
        mov    ecx, edi
        call   sub_4_043A
        mov    ebx, eax
        or     ebx, ebx
        je     loc_4_0C60
        jmp    loc_4_0C54
loc_4_0A65:
        movzx  ebp, cx
        push   ebp
        mov    edx, 0x104                    ; -> obj3:0x0104
        mov    ecx, ebx
        call   sub_4_0454
        or     eax, eax
        jne    loc_4_0AA5
        push   ebp
        mov    edx, 0x114                    ; -> obj3:0x0114
        mov    ecx, ebx
        call   sub_4_0454
        or     eax, eax
        jne    loc_4_0AA5
        mov    edx, 0x3b
        mov    ecx, edi
        call   sub_4_043A
        mov    ebx, eax
        or     ebx, ebx
        je     loc_4_0C60
        jmp    loc_4_0C54
loc_4_0AA5:
        mov    eax, dword ptr [esp + 0x20]
        mov    edx, 0x3b
        mov    dword ptr [eax], edi
        mov    ecx, edi
        call   sub_4_043A
        mov    ebx, eax
        or     ebx, ebx
        je     loc_4_0C60
        jmp    loc_4_0C54
loc_4_0AC6:
        movzx  ebp, cx
        push   ebp
        mov    edx, 0xc4                     ; -> obj3:0x00C4
        mov    ecx, ebx
        call   sub_4_0454
        or     eax, eax
        jne    loc_4_0B49
        push   ebp
        mov    edx, 0xcc                     ; -> obj3:0x00CC
        mov    ecx, ebx
        call   sub_4_0454
        or     eax, eax
        jne    loc_4_0B49
        push   ebp
        mov    edx, 0xd4                     ; -> obj3:0x00D4
        mov    ecx, ebx
        call   sub_4_0454
        or     eax, eax
        jne    loc_4_0B28
        push   ebp
        mov    edx, 0xe4                     ; -> obj3:0x00E4
        mov    ecx, ebx
        call   sub_4_0454
        or     eax, eax
        jne    loc_4_0B28
        mov    edx, 0x3b
        mov    ecx, edi
        call   sub_4_043A
        mov    ebx, eax
        or     ebx, ebx
        je     loc_4_0C60
        jmp    loc_4_0C54
loc_4_0B28:
        mov    eax, dword ptr [esp + 0x24]
        mov    edx, 0x3b
        mov    dword ptr [eax], edi
        mov    ecx, edi
        call   sub_4_043A
        mov    ebx, eax
        or     ebx, ebx
        je     loc_4_0C60
        jmp    loc_4_0C54
loc_4_0B49:
        mov    eax, dword ptr [esp + 0x18]
        mov    edx, 0x3b
        mov    dword ptr [eax], edi
        mov    ecx, edi
        call   sub_4_043A
        mov    ebx, eax
        or     ebx, ebx
        je     loc_4_0C60
        jmp    loc_4_0C54
loc_4_0B6A:
        movzx  ebp, cx
        push   ebp
        mov    edx, 0xec                     ; -> obj3:0x00EC
        mov    ecx, ebx
        call   sub_4_0454
        or     eax, eax
        jne    loc_4_0BAA
        push   ebp
        mov    edx, 0xfc                     ; -> obj3:0x00FC
        mov    ecx, ebx
        call   sub_4_0454
        or     eax, eax
        jne    loc_4_0BAA
        mov    edx, 0x3b
        mov    ecx, edi
        call   sub_4_043A
        mov    ebx, eax
        or     ebx, ebx
        je     loc_4_0C60
        jmp    loc_4_0C54
loc_4_0BAA:
        mov    eax, dword ptr [esp + 0x1c]
        mov    edx, 0x3b
        mov    dword ptr [eax], edi
        mov    ecx, edi
        call   sub_4_043A
        mov    ebx, eax
        or     ebx, ebx
        je     loc_4_0C60
        jmp    loc_4_0C54
loc_4_0BCB:
        movzx  ebp, cx
        push   ebp
        mov    edx, 0x9c                     ; -> obj3:0x009C
        mov    ecx, ebx
        call   sub_4_0454
        or     eax, eax
        jne    loc_4_0C40
        push   ebp
        mov    edx, 0xac                     ; -> obj3:0x00AC
        mov    ecx, ebx
        call   sub_4_0454
        or     eax, eax
        jne    loc_4_0C40
        push   ebp
        mov    edx, 0xb4                     ; -> obj3:0x00B4
        mov    ecx, ebx
        call   sub_4_0454
        or     eax, eax
        jne    loc_4_0C26
        push   ebp
        mov    edx, 0xbc                     ; -> obj3:0x00BC
        mov    ecx, ebx
        call   sub_4_0454
        or     eax, eax
        jne    loc_4_0C26
        mov    edx, 0x3b
        mov    ecx, edi
        call   sub_4_043A
        mov    ebx, eax
        or     ebx, ebx
        je     loc_4_0C60
        jmp    loc_4_0C54
loc_4_0C26:
        mov    eax, dword ptr [esp + 0x10]
        mov    edx, 0x3b
        mov    dword ptr [eax], edi
        mov    ecx, edi
        call   sub_4_043A
        mov    ebx, eax
        or     ebx, ebx
        je     loc_4_0C60
        jmp    loc_4_0C54
loc_4_0C40:
        mov    dword ptr [esi], edi
        mov    edx, 0x3b
        mov    ecx, edi
        call   sub_4_043A
        mov    ebx, eax
        or     ebx, ebx
        je     loc_4_0C60
loc_4_0C54:
        mov    byte ptr [ebx], 0
        inc    ebx
        or     ebx, ebx
        jne    loc_4_0A01
loc_4_0C60:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    0x14
        db  000h, 000h
sub_4_0C6C:
        VMMCall  VMM._RegOpenKey                 ; cd2048810100
        add    byte ptr [eax], al
sub_4_0C74:
        VMMCall  VMM._RegCloseKey                ; cd2049810100
        add    byte ptr [eax], al
sub_4_0C7C:
        VMMCall  VMM._RegEnumKey                 ; cd204c810100
        add    byte ptr [eax], al
sub_4_0C84:
        VMMCall  VMM._RegQueryValueEx            ; cd2051810100
        add    byte ptr [eax], al
sub_4_0C8C:
        VxDCall  BIOS.Service_8021h              ; cd2021802b00
        add    byte ptr [eax], al
sub_4_0C94:
        VxDCall  BIOS.Service_8022h              ; cd2022802b00
        add    byte ptr [eax], al
sub_4_0C9C:
        VMMCall  VMM._HeapAllocate               ; cd204f800100
        add    byte ptr [eax], al
sub_4_0CA4:
        VMMCall  VMM._HeapFree                   ; cd2051800100
        add    byte ptr [eax], al
sub_4_0CAC:
        VMMCall  VMM._Sprintf                    ; cd2074810100
        add    byte ptr [eax], al
sub_4_0CB4:
        VxDCall  CONFIGMG._CONFIGMG_Get_Device_ID ; cd2007803300
        add    byte ptr [eax], al
sub_4_0CBC:
        VxDCall  CONFIGMG._CONFIGMG_Create_DevNode ; cd200b803300
        add    byte ptr [eax], al
sub_4_0CC4:
        VxDCall  CONFIGMG._CONFIGMG_Register_Enumerator ; cd200f803300
        add    byte ptr [eax], al
sub_4_0CCC:
        VxDCall  CONFIGMG._CONFIGMG_Get_First_Log_Conf ; cd201a803300
        add    byte ptr [eax], al
sub_4_0CD4:
        VxDCall  CONFIGMG._CONFIGMG_Get_Next_Res_Des ; cd201f803300
        add    byte ptr [eax], al
sub_4_0CDC:
        VxDCall  CONFIGMG._CONFIGMG_Get_CRC_CheckSum ; cd2031803300
        add    byte ptr [eax], al
sub_4_0CE4:
        VxDCall  CONFIGMG._CONFIGMG_Reset_Children_Marks ; cd2035803300
        add    byte ptr [eax], al
sub_4_0CEC:
        VxDCall  CONFIGMG._CONFIGMG_Remove_Unmarked_Children ; cd2037803300
        add    byte ptr [eax], al
sub_4_0CF4:
        VxDCall  CONFIGMG._CONFIGMG_Read_Registry_Value ; cd203e803300
        add    byte ptr [eax], al
sub_4_0CFC:
        VxDCall  Dev_2533.Service_803Fh          ; cd203f803325
VxD__LTEXT ENDS

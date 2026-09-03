; EISA -- reconstructed 32-bit disassembly (VxD / LE format)
; 3 objects, 1285 instructions recovered by recursive descent

Declare_Virtual_Device EISA, 4, 0, EISA_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 016800000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x84  R-X
EISA_Control:
        cmp    eax, 1
        jne    loc_1_0013
        push   0x34                          ; -> obj1:0x0034
        call   sub_3_0E10
        cmp    eax, 1
        ret    
loc_1_0013:
        cmp    eax, 0x1b
        jne    loc_1_0023
        push   0
        call   sub_3_0E10
        cmp    eax, 1
        ret    
loc_1_0023:
        cmp    eax, 0x22
        jne    loc_1_0031
        push   edx
        push   ebx
        call   sub_3_0E30
        stc    
        ret    
loc_1_0031:
        clc    
        ret    
        db  000h
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  4, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'EISA    '                  ; DDB_Name
        dd  016800000h                  ; DDB_Init_Order
        dd  EISA_Control             ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 2  vsize=0x140  RW-
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 045h, 049h, 053h, 041h
        db  05Ch, 02Ah, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 03Fh, 05Ch, 03Fh, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 045h, 049h, 053h, 041h, 020h, 063h, 061h, 072h, 064h, 020h, 069h, 06Eh
        db  020h, 073h, 06Ch, 06Fh, 074h, 020h, 023h, 025h, 031h, 064h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 03Fh, 000h, 000h, 000h, 04Dh, 000h, 000h, 000h
        db  055h, 000h, 000h, 000h, 007h, 000h, 000h, 000h, 002h, 000h, 000h, 000h, 002h, 000h, 000h, 000h
        db  003h, 000h, 000h, 000h, 009h, 000h, 000h, 000h, 007h, 000h, 000h, 000h, 004h, 000h, 000h, 000h
        db  014h, 000h, 000h, 000h
sub_2_0074:
        in     al, 2
        add    byte ptr [eax], al
        pop    ebp
        add    eax, dword ptr [eax]
        add    dh, cl                        ; -> loc_3_04CE
        add    al, 0
        add    bh, dl                        ; -> loc_3_03D7
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_008D:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_0091:
        dec    ebx
        inc    ebp
        push   edx
        dec    esi
        inc    ebp
        dec    esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebp + 0x49], al
        push   ebx
        inc    ecx
        push   ebx
        imul   ebp, dword ptr [ebp + 0x75], 0x6574616c
        dec    ecx
        outsb  dx, byte ptr [esi]
        je     loc_2_00DD
        xor    eax, 0x56000000
        push   ebx
        je     loc_2_0120
        je     loc_2_011E
        inc    esp
        and    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [esi + 0x52], dl
        outsd  dx, dword ptr [esi]
        insd   dword ptr es:[edi], dx
        inc    esp
        and    byte ptr [eax], ah
        and    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ebp + 0x76], al
        imul   esp, dword ptr [ebx + 0x65], 0x63736544
        add    byte ptr [eax], al
loc_2_00D4:
        and    eax, 0x252e6433
        xor    esp, dword ptr [eax + eax + 0x48]
loc_2_00DD:
        push   edi
        push   edx
        jbe    0x14b
        jae    0x14d
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        add    byte ptr [eax], al
loc_2_00E8:
        and    eax, 0x5c005832
        and    eax, 0x5831
        add    byte ptr [eax], al
loc_2_00F4:
        inc    ebp
        dec    ecx
        push   ebx
        inc    ecx
        push   edx
        popal  
        outsb  dx, byte ptr [esi]
        jae    loc_2_00FF
loc_2_00FF:
        add    byte ptr [ebp + 0x49], al
        push   ebx
        inc    ecx
        inc    esi
        insb   byte ptr es:[edi], dx
        popal  
        jae    loc_2_010A
loc_2_010A:
        add    byte ptr [eax], al
loc_2_010C:
        inc    ebp
        dec    ecx
        push   ebx
        inc    ecx
        inc    esi
        jne    0x181
        arpl   word ptr [ecx + ebp*2 + 0x6f], si
        outsb  dx, byte ptr [esi]
        jae    loc_2_011A
loc_2_011A:
        add    byte ptr [eax], al
loc_2_011C:
        inc    ebp
        dec    ecx
loc_2_011E:
        push   ebx
        inc    ecx
loc_2_0120:
        inc    esi
        jne    0x191
        arpl   word ptr [ecx + ebp*2 + 0x6f], si
        outsb  dx, byte ptr [esi]
        jae    0x177
        popal  
        jae    0x198
        add    byte ptr [eax], al
        add    byte ptr [ebp + 0x49], al
        push   ebx
        inc    ecx
        pop    esp
        sub    dl, byte ptr [eax + 0x4e]
        push   eax
        xor    byte ptr [ecx + 0x30], al
        xor    byte ptr [eax + esi], bl
VxD__LDATA ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0xf46  R-X
        db  056h, 08Bh, 0F1h, 083h, 03Dh
        dd  obj2:0x0008              ; fixup
        db  000h, 074h, 007h, 0B8h, 013h, 000h, 000h, 000h, 0EBh, 05Ah, 06Ah, 000h, 068h
        dd  obj2:0x0000              ; fixup
        db  0E8h, 0C9h, 00Eh, 000h, 000h, 083h, 0C4h, 008h, 00Bh, 0C0h, 074h, 007h, 0B8h, 002h, 000h, 000h
        db  000h, 0EBh, 040h, 089h, 035h
        dd  obj2:0x0008              ; fixup
        db  06Ah, 000h, 06Ah, 004h, 068h, 0D9h, 0FFh, 00Fh, 000h, 0E8h, 017h, 00Eh, 000h, 000h, 083h, 0C4h
        db  00Ch, 06Ah, 001h, 068h
        dd  loc_3_0AF9               ; fixup
        db  056h, 0E8h, 06Fh, 00Eh, 000h, 000h, 083h, 0C4h, 00Ch, 00Bh, 0C0h, 075h, 015h, 06Ah, 002h, 06Ah
        db  000h, 06Ah, 000h, 056h, 0E8h, 054h, 00Eh, 000h, 000h, 083h, 0C4h, 010h, 00Bh, 0C0h, 075h, 002h
        db  02Bh, 0C0h, 05Eh, 0C3h, 000h
sub_3_0070:
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 0x18], 1
        lea    eax, [esp + 0x18]
        push   1
        push   eax
        push   0x8d                          ; -> loc_2_008D
        push   3
        push   0x9c                          ; -> obj2:0x009C
        push   0
        push   dword ptr [8]                 ; -> obj2:0x0008
        call   sub_3_0F30
        add    esp, 0x1c
        or     eax, eax
        je     loc_3_00D0
        push   0xb0                          ; -> obj2:0x00B0
        push   0
        call   sub_3_0E78
        or     eax, eax
        jne    loc_3_00C6
        push   0xbc                          ; -> obj2:0x00BC
        push   0
        call   sub_3_0E78
        or     eax, eax
        je     loc_3_00D0
loc_3_00C6:
        mov    dword ptr [0x8d], 1           ; -> loc_2_008D
loc_3_00D0:
        call   sub_3_0BFC
        or     eax, eax
        je     loc_3_0227
        mov    esi, dword ptr [4]            ; -> obj2:0x0004
        or     esi, esi
        je     loc_3_00FA
loc_3_00E7:
        mov    eax, esi
        mov    esi, dword ptr [esi]
        push   0
        push   eax
        call   sub_3_0E68
        add    esp, 8
        or     esi, esi
        jne    loc_3_00E7
loc_3_00FA:
        sub    ebp, ebp
        mov    dword ptr [4], ebp            ; -> obj2:0x0004
        mov    byte ptr [esp + 0x12], 1
loc_3_0107:
        lea    eax, [esp + 0x13]
        push   eax
        lea    eax, [esp + 0x18]
        push   eax
        push   dword ptr [esp + 0x1a]
        call   sub_3_0CDE
        mov    byte ptr [esp + 0x1d], al
        add    esp, 0xc
        or     al, al
        je     loc_3_0213
        movzx  eax, byte ptr [esp + 0x11]
        push   0x200
        imul   eax, eax, 0x140
        add    eax, 0xb
        push   eax
        call   sub_3_0E60
        add    esp, 8
        or     eax, eax
        je     loc_3_0222
        mov    edx, eax
        mov    dword ptr [4], edx            ; -> obj2:0x0004
        mov    dword ptr [eax], ebp
        mov    cl, byte ptr [esp + 0x12]
        mov    byte ptr [eax + 9], cl
        sub    bl, bl
        mov    ecx, dword ptr [esp + 0x14]
        mov    dword ptr [eax + 4], ecx
        mov    cl, byte ptr [esp + 0x13]
        mov    byte ptr [eax + 8], cl
        mov    byte ptr [esp + 0x10], 0
        cmp    byte ptr [esp + 0x11], bl
        je     loc_3_01B4
loc_3_0179:
        push   ebx
        push   dword ptr [esp + 0x16]
        call   sub_3_0D78
        add    esp, 8
        or     eax, eax
        mov    edx, dword ptr [4]            ; -> obj2:0x0004
        je     loc_3_01AC
        movzx  ecx, byte ptr [esp + 0x10]
        imul   ecx, ecx, 0x140
        lea    edi, [edx + ecx + 0xb]
        mov    esi, eax
        mov    ecx, 0x50
        rep movsd dword ptr es:[edi], dword ptr [esi]
        inc    byte ptr [esp + 0x10]
loc_3_01AC:
        inc    bl
        cmp    bl, byte ptr [esp + 0x11]
        jb     loc_3_0179
loc_3_01B4:
        mov    al, byte ptr [esp + 0x10]
        cmp    al, byte ptr [esp + 0x11]
        je     loc_3_0204
        or     al, al
        mov    dword ptr [4], edx            ; -> obj2:0x0004
        jne    loc_3_01DB
        push   0
        push   edx
        call   sub_3_0E68
        add    esp, 8
        mov    dword ptr [4], ebp            ; -> obj2:0x0004
        jmp    loc_3_0213
loc_3_01DB:
        movzx  eax, byte ptr [esp + 0x10]
        push   0x201
        imul   eax, eax, 0x140
        add    eax, 0xb
        push   eax
        push   edx
        call   sub_3_0E70
        add    esp, 0xc
        or     eax, eax
        mov    edx, dword ptr [4]            ; -> obj2:0x0004
        je     loc_3_0204
        mov    edx, eax
loc_3_0204:
        mov    dword ptr [4], edx            ; -> obj2:0x0004
        mov    ebp, edx
        mov    al, byte ptr [esp + 0x10]
        mov    byte ptr [edx + 0xa], al
loc_3_0213:
        inc    byte ptr [esp + 0x12]
        cmp    byte ptr [esp + 0x12], 0x10
        jb     loc_3_0107
loc_3_0222:
        call   sub_3_0C77
loc_3_0227:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
sub_3_022F:
        sub    esp, 0x10c
        push   esi
        push   edi
        mov    edi, edx
        mov    dword ptr [esp + 8], 0x100
        lea    eax, [esp + 8]
        push   0
        push   eax
        mov    esi, ecx
        lea    eax, [esp + 0x1c]
        push   eax
        push   1
        push   0xc8                          ; -> obj2:0x00C8
        push   0
        push   esi
        call   sub_3_0F30
        add    esp, 0x1c
        or     eax, eax
        je     loc_3_0297
        movzx  eax, byte ptr [edi + 9]
        push   eax
        lea    eax, [esp + 0x18]
        push   0x24                          ; -> obj2:0x0024
        push   eax
        call   sub_3_0E90
        add    esp, 0xc
        push   0
        lea    eax, [esp + 0x18]
        push   0
        push   eax
        push   1
        push   0xc8                          ; -> obj2:0x00C8
        push   0
        push   esi
        call   sub_3_0F38
        add    esp, 0x1c
loc_3_0297:
        cmp    byte ptr [edi + 0xa], 0
        je     loc_3_02DB
        mov    cx, word ptr [edi + 0x11]
        or     cx, cx
        je     loc_3_02DB
        movzx  eax, cl
        push   eax
        movzx  eax, ch
        push   eax
        lea    eax, [esp + 0x14]
        push   0xd4                          ; -> loc_2_00D4
        push   eax
        call   sub_3_0E90
        add    esp, 0x10
        push   0
        lea    eax, [esp + 0x10]
        push   0
        push   eax
        push   1
        push   0xdc                          ; -> obj2:0x00DC
        push   0
        push   esi
        call   sub_3_0F38
        add    esp, 0x1c
loc_3_02DB:
        pop    edi
        pop    esi
        add    esp, 0x10c
        ret    
loc_3_02E4:
        sub    esp, 0x10
        push   edi
        lea    edi, [esp + 4]
        sub    eax, eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        mov    edx, dword ptr [esp + 0x18]
        mov    word ptr [esp + 6], 0x14
        movzx  eax, byte ptr [edx + 3]
        movzx  ecx, byte ptr [edx + 4]
        shl    ecx, 8
        add    ecx, eax
        shl    ecx, 8
        movzx  eax, byte ptr [edx + 2]
        add    ecx, eax
        shl    ecx, 8
        mov    dword ptr [esp + 8], ecx
        movzx  eax, word ptr [edx + 5]
        shl    eax, 0xa
        add    eax, ecx
        dec    eax
        mov    dword ptr [esp + 0xc], eax
        mov    al, byte ptr [edx]
        and    al, 1
        cmp    al, 1
        push   0
        sbb    eax, eax
        inc    eax
        mov    word ptr [esp + 0x14], ax
        lea    eax, [esp + 8]
        push   0x10
        push   eax
        mov    eax, dword ptr [0x20]         ; -> obj2:0x0020
        or     eax, 1
        push   eax
        push   dword ptr [0x1c]              ; -> obj2:0x001C
        push   0
        call   sub_3_0EE0
        add    esp, 0x18
        pop    edi
        add    esp, 0x10
        ret    
loc_3_035D:
        sub    esp, 8
        push   edi
        lea    edi, [esp + 4]
        sub    eax, eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        mov    eax, dword ptr [esp + 0x10]
        mov    word ptr [esp + 4], 1
        mov    al, byte ptr [eax]
        and    ax, 0xf
        mov    word ptr [esp + 6], ax
        push   0
        lea    eax, [esp + 8]
        push   8
        push   eax
        mov    eax, dword ptr [0x20]         ; -> obj2:0x0020
        or     eax, 4
        push   eax
        push   dword ptr [0x1c]              ; -> obj2:0x001C
        push   0
        call   sub_3_0EE0
        add    esp, 0x18
        pop    edi
        add    esp, 8
        ret    
sub_3_03A6:
        mov    dword ptr [0x40], 1           ; -> obj2:0x0040
        cmp    dword ptr [0x3c], 0           ; -> obj2:0x003C
        jne    loc_3_03D6
        movzx  edx, word ptr [ecx + 1]
        push   0
        mov    eax, dword ptr [0]            ; -> obj2:0x0000
        push   eax
        mov    al, byte ptr [ecx]
        and    eax, 0x1f
        add    eax, edx
        push   eax
        push   edx
        call   sub_3_0EF0
        add    esp, 0x10
loc_3_03D6:
        ret    
loc_3_03D7:
        sub    esp, 0x18
        push   esi
        push   edi
        lea    edi, [esp + 0x14]
        sub    eax, eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        mov    eax, dword ptr [esp + 0x24]
        mov    word ptr [esp + 0x16], 0xc
        movzx  esi, word ptr [eax + 1]
        mov    al, byte ptr [eax]
        and    eax, 0x1f
        lea    edi, [esi + eax]
        cmp    dword ptr [0x3c], 0           ; -> obj2:0x003C
        je     loc_3_040F
        mov    dword ptr [esp + 8], esi
        mov    dword ptr [esp + 0xc], edi
        jmp    loc_3_0479
loc_3_040F:
        lea    eax, [esp + 0x10]
        push   0
        push   eax
        lea    eax, [esp + 0x14]
        push   eax
        lea    eax, [esp + 0x14]
        push   eax
        push   dword ptr [0]                 ; -> obj2:0x0000
        call   sub_3_0F08
        add    esp, 0x14
        or     eax, eax
        jne    loc_3_04C8
loc_3_0436:
        cmp    dword ptr [esp + 8], esi
        ja     loc_3_0442
        cmp    dword ptr [esp + 0xc], edi
        jae    loc_3_0461
loc_3_0442:
        lea    eax, [esp + 0xc]
        push   0
        push   eax
        lea    eax, [esp + 0x10]
        push   eax
        lea    eax, [esp + 0x1c]
        push   eax
        call   sub_3_0F10
        add    esp, 0x10
        or     eax, eax
        je     loc_3_0436
        jmp    loc_3_04C8
loc_3_0461:
        push   0
        push   dword ptr [0]                 ; -> obj2:0x0000
        push   dword ptr [esp + 0x14]
        push   dword ptr [esp + 0x14]
        call   sub_3_0EF8
        add    esp, 0x10
loc_3_0479:
        mov    eax, dword ptr [esp + 8]
        cmp    dword ptr [esp + 8], 0x400
        mov    word ptr [esp + 0x18], ax
        mov    eax, dword ptr [esp + 0xc]
        mov    word ptr [esp + 0x1a], ax
        jae    loc_3_04A1
        mov    byte ptr [esp + 0x1e], 4
        mov    byte ptr [esp + 0x1f], 3
        jmp    loc_3_04A6
loc_3_04A1:
        mov    byte ptr [esp + 0x1e], 0xff
loc_3_04A6:
        push   0
        lea    eax, [esp + 0x18]
        push   0xc
        push   eax
        mov    eax, dword ptr [0x20]         ; -> obj2:0x0020
        or     eax, 2
        push   eax
        push   dword ptr [0x1c]              ; -> obj2:0x001C
        push   0
        call   sub_3_0EE0
        add    esp, 0x18
loc_3_04C8:
        pop    edi
        pop    esi
        add    esp, 0x18
        ret    
loc_3_04CE:
        sub    esp, 4
        push   edi
        lea    edi, [esp + 4]
        sub    eax, eax
        stosd  dword ptr es:[edi], eax
        mov    eax, dword ptr [esp + 0xc]
        push   0
        mov    al, byte ptr [eax]
        and    al, 7
        mov    byte ptr [esp + 9], al
        lea    eax, [esp + 8]
        push   4
        push   eax
        mov    eax, dword ptr [0x20]         ; -> obj2:0x0020
        or     eax, 3
        push   eax
        push   dword ptr [0x1c]              ; -> obj2:0x001C
        push   0
        call   sub_3_0EE0
        add    esp, 0x18
        pop    edi
        add    esp, 4
        ret    
sub_3_050C:
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    byte ptr [esp + 0x11], dl
        mov    eax, ecx
        mov    dword ptr [esp + 0x14], ecx
        mov    al, byte ptr [eax + 0x22]
        mov    byte ptr [esp + 0x13], al
        sub    bl, bl
        test   al, 0x40
        jne    loc_3_05C3
        and    al, 0x80
        cmp    al, 1
        cmc    
        sbb    eax, eax
        and    eax, 0x8000
        mov    dword ptr [0x20], eax         ; -> obj2:0x0020
        mov    byte ptr [esp + 0x12], bl
        mov    ebp, dword ptr [esp + 0x20]
loc_3_0547:
        shr    byte ptr [esp + 0x13], 1
        test   byte ptr [esp + 0x13], 1
        je     loc_3_05B8
        movzx  eax, byte ptr [esp + 0x12]
        lea    esi, [eax*4]
        mov    byte ptr [esp + 0x10], 0
        mov    ecx, dword ptr [esi + 0x44]   ; -> obj2:0x0044
        mov    eax, dword ptr [esp + 0x14]
        lea    edi, [ecx + eax + 0x73]
loc_3_0571:
        cmp    byte ptr [esp + 0x11], bl
        ja     loc_3_057B
        sub    eax, eax
        jmp    loc_3_05A7
loc_3_057B:
        mov    eax, 1
        mov    cl, bl
        shl    eax, cl
        test   ebp, eax
        jne    loc_3_0592
        push   edi
        call   sub_2_0074
        add    esp, 4
loc_3_0592:
        inc    bl
        mov    al, byte ptr [edi]
        and    al, 0x80
        cmp    al, 1
        sbb    eax, eax
        inc    eax
        add    edi, dword ptr [esi + 0x54]   ; -> obj2:0x0054
        inc    byte ptr [esp + 0x10]
loc_3_05A7:
        or     eax, eax
        je     loc_3_05B8
        movzx  eax, byte ptr [esp + 0x10]
        cmp    eax, dword ptr [esi + 0x64]   ; -> obj2:0x0064
        jb     loc_3_0571
loc_3_05B8:
        inc    byte ptr [esp + 0x12]
        cmp    byte ptr [esp + 0x12], 4
        jb     loc_3_0547
loc_3_05C3:
        cmp    byte ptr [esp + 0x11], 0xff
        je     loc_3_05FB
        cmp    byte ptr [esp + 0x11], bl
        jbe    loc_3_05FB
        mov    al, byte ptr [esp + 0x11]
        sub    al, bl
        mov    byte ptr [esp + 0x18], al
loc_3_05DA:
        push   0
        push   0
        push   0
        push   0x8000
        push   dword ptr [0x1c]              ; -> obj2:0x001C
        push   0
        call   sub_3_0EE0
        add    esp, 0x18
        dec    byte ptr [esp + 0x18]
        jne    loc_3_05DA
loc_3_05FB:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    4
sub_3_0605:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    byte ptr [esp + 0x12], dl
        mov    eax, ecx
        mov    dword ptr [esp + 0x14], ecx
        mov    al, byte ptr [eax + 0x22]
        mov    byte ptr [esp + 0x13], al
        sub    bl, bl
        test   al, 0x40
        jne    loc_3_06B3
        mov    byte ptr [esp + 0x11], bl
        mov    ebp, dword ptr [esp + 0x1c]
loc_3_062F:
        shr    byte ptr [esp + 0x13], 1
        test   byte ptr [esp + 0x13], 1
        je     loc_3_06A4
        movzx  eax, byte ptr [esp + 0x11]
        lea    esi, [eax*4]
        mov    byte ptr [esp + 0x10], 0
        mov    ecx, dword ptr [esi + 0x44]   ; -> obj2:0x0044
        mov    eax, dword ptr [esp + 0x14]
        lea    edi, [ecx + eax + 0x73]
loc_3_0659:
        cmp    bl, byte ptr [esp + 0x12]
        jb     loc_3_0663
        sub    eax, eax
        jmp    loc_3_0693
loc_3_0663:
        mov    eax, 1
        mov    cl, bl
        shl    eax, cl
        test   ebp, eax
        jne    loc_3_067E
        cmp    byte ptr [esp + 0x11], 3
        jne    loc_3_067E
        mov    ecx, edi
        call   sub_3_03A6
loc_3_067E:
        inc    bl
        mov    al, byte ptr [edi]
        and    al, 0x80
        cmp    al, 1
        sbb    eax, eax
        inc    eax
        add    edi, dword ptr [esi + 0x54]   ; -> obj2:0x0054
        inc    byte ptr [esp + 0x10]
loc_3_0693:
        or     eax, eax
        je     loc_3_06A4
        movzx  eax, byte ptr [esp + 0x10]
        cmp    dword ptr [esi + 0x64], eax   ; -> obj2:0x0064
        ja     loc_3_0659
loc_3_06A4:
        inc    byte ptr [esp + 0x11]
        cmp    byte ptr [esp + 0x11], 4
        jb     loc_3_062F
loc_3_06B3:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    4
sub_3_06BD:
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    byte ptr [esp + 0xd], dl
        mov    esi, 0x12                     ; -> obj2:0x0012
        mov    dword ptr [esp + 0x10], ecx
        mov    al, ch
        mov    ah, cl
        mov    word ptr [esp + 0xe], ax
        mov    edx, 1
loc_3_06DE:
        mov    bx, word ptr [esp + 0xe]
        mov    al, 3
        sub    al, dl
        mov    cl, 5
        imul   cl
        mov    cl, al
        shr    bx, cl
        and    bl, 0x1f
        add    bl, 0x40
        mov    byte ptr [esi], bl
        inc    esi
        inc    edx
        cmp    edx, 3
        jbe    loc_3_06DE
        sub    edi, edi
loc_3_0701:
        mov    eax, dword ptr [esp + 0x10]
        lea    ecx, [edi*8 + 0x10]
        shr    eax, cl
        and    eax, 0xff
        push   eax
        push   0xe8                          ; -> loc_2_00E8
        push   esi
        call   sub_3_0E90
        add    esp, 0xc
        add    esi, 2
        inc    edi
        cmp    edi, 2
        jb     loc_3_0701
        movzx  eax, byte ptr [esp + 0xd]
        push   eax
        push   0xec                          ; -> obj2:0x00EC
        push   esi
        call   sub_3_0E90
        add    esp, 0xc
        mov    eax, 0xc                      ; -> obj2:0x000C
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
sub_3_074B:
        sub    esp, 0x20
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    esi, ecx
        mov    word ptr [esp + 0x20], 0xc00
        mov    word ptr [esp + 0x22], 0xcff
        lea    edi, [esp + 0x24]
        sub    eax, eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        mov    dword ptr [esp + 0x10], 0x10
        push   eax
        lea    eax, [esp + 0x14]
        push   eax
        lea    eax, [esp + 0x28]
        push   eax
        push   3
        push   0xf4                          ; -> loc_2_00F4
        push   0
        push   edx
        call   sub_3_0F30
        add    esp, 0x1c
        or     eax, eax
        jne    loc_3_079D
        mov    eax, dword ptr [esp + 0x10]
        shr    eax, 2
        jmp    loc_3_07A9
loc_3_079D:
        mov    eax, dword ptr [0x40]         ; -> obj2:0x0040
        cmp    eax, 1
        sbb    al, al
        neg    al
loc_3_07A9:
        or     al, al
        je     loc_3_080E
        lea    ebp, [esp + 0x20]
        movzx  ebx, al
loc_3_07B4:
        lea    edi, [esp + 0x14]
        sub    eax, eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        mov    word ptr [esp + 0x16], 0xc
        movzx  ax, byte ptr [esi + 9]
        shl    ax, 0xc
        add    ax, word ptr [ebp]
        mov    word ptr [esp + 0x18], ax
        movzx  ax, byte ptr [esi + 9]
        shl    ax, 0xc
        add    ax, word ptr [ebp + 2]
        mov    word ptr [esp + 0x1a], ax
        mov    byte ptr [esp + 0x1e], 0xff
        push   0
        lea    eax, [esp + 0x18]
        push   0xc
        add    ebp, 4
        push   eax
        push   2
        push   dword ptr [0x1c]              ; -> obj2:0x001C
        push   0
        call   sub_3_0EE0
        add    esp, 0x18
        dec    ebx
        jne    loc_3_07B4
loc_3_080E:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x20
        ret    
sub_3_0816:
        sub    esp, 0x514
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    dword ptr [esp + 0x14], edx
        mov    dword ptr [esp + 0x18], ecx
        mov    dword ptr [esp + 0x20], 0
        push   3
        push   1
        push   dword ptr [esp + 0x1c]
        push   0x1c                          ; -> obj2:0x001C
        call   sub_3_0EC8
        add    esp, 0x10
        or     eax, eax
        jne    loc_3_09DA
        mov    dword ptr [esp + 0x10], 4
        lea    eax, [esp + 0x10]
        push   0
        push   eax
        mov    dword ptr [0x40], 0           ; -> obj2:0x0040
        lea    eax, [esp + 0x28]
        push   eax
        push   3
        push   0x100                         ; -> obj2:0x0100
        push   0
        push   dword ptr [esp + 0x2c]
        call   sub_3_0F30
        mov    eax, dword ptr [esp + 0x3c]
        add    esp, 0x1c
        and    eax, 1
        mov    dword ptr [0x3c], eax         ; -> obj2:0x003C
        mov    ebp, dword ptr [esp + 0x20]
        and    ebp, 2
        sub    edx, edx
        lea    edi, [esp + 0x24]
        mov    eax, 0xffffffff
        mov    ecx, 0x40
        rep stosd dword ptr es:[edi], eax
        sub    eax, eax
loc_3_08A7:
        mov    dword ptr [esp + edx*4 + 0x124], eax
        inc    edx
        cmp    edx, 0x100
        jb     loc_3_08A7
        mov    dword ptr [esp + 0x10], 0x100
        lea    eax, [esp + 0x10]
        push   0
        push   eax
        lea    eax, [esp + 0x2c]
        push   eax
        push   3
        push   0x10c                         ; -> loc_2_010C
        push   0
        push   dword ptr [esp + 0x2c]
        call   sub_3_0F30
        add    esp, 0x1c
        or     eax, eax
        jne    loc_3_090F
        cmp    dword ptr [esp + 0x10], 0x100
        jae    loc_3_090F
        mov    edx, 0x100
        sub    edx, dword ptr [esp + 0x10]
        mov    eax, dword ptr [esp + 0x10]
        lea    edi, [esp + eax + 0x24]
        sub    eax, eax
        mov    ecx, edx
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        mov    ecx, edx
        and    ecx, 3
        rep stosb byte ptr es:[edi], al
loc_3_090F:
        mov    dword ptr [esp + 0x10], 0x400
        lea    eax, [esp + 0x10]
        push   0
        push   eax
        lea    eax, [esp + 0x12c]
        push   eax
        push   3
        push   0x11c                         ; -> loc_2_011C
        push   0
        push   dword ptr [esp + 0x2c]
        call   sub_3_0F30
        add    esp, 0x1c
        push   0
        push   dword ptr [0]                 ; -> obj2:0x0000
        push   -1
        push   0
        call   sub_3_0EF8
        mov    edi, dword ptr [esp + 0x28]
        add    esp, 0x10
        add    edi, 0xb
        mov    ebx, edi
        mov    eax, dword ptr [esp + 0x18]
        sub    esi, esi
        mov    al, byte ptr [eax + 0xa]
        or     al, al
        je     loc_3_098B
        movzx  eax, al
        mov    dword ptr [esp + 0x1c], eax
loc_3_096C:
        push   dword ptr [esp + esi*4 + 0x124]
        mov    edx, dword ptr [esp + esi + 0x28]
        mov    ecx, ebx
        call   sub_3_0605
        add    ebx, 0x140
        inc    esi
        cmp    esi, dword ptr [esp + 0x1c]
        jb     loc_3_096C
loc_3_098B:
        or     ebp, ebp
        je     loc_3_099C
        mov    edx, dword ptr [esp + 0x14]
        mov    ecx, dword ptr [esp + 0x18]
        call   sub_3_074B
loc_3_099C:
        mov    eax, dword ptr [esp + 0x18]
        sub    esi, esi
        mov    al, byte ptr [eax + 0xa]
        or     al, al
        je     loc_3_09C9
        movzx  ebx, al
loc_3_09AC:
        push   dword ptr [esp + esi*4 + 0x124]
        mov    edx, dword ptr [esp + esi + 0x28]
        mov    ecx, edi
        call   sub_3_050C
        add    edi, 0x140
        inc    esi
        cmp    esi, ebx
        jb     loc_3_09AC
loc_3_09C9:
        or     ebp, ebp
        jne    loc_3_09DA
        mov    edx, dword ptr [esp + 0x14]
        mov    ecx, dword ptr [esp + 0x18]
        call   sub_3_074B
loc_3_09DA:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0x514
        ret    
sub_3_09E5:
        sub    esp, 8
        push   ebx
        push   esi
        call   sub_3_0070
        push   0
        push   dword ptr [8]                 ; -> obj2:0x0008
        push   0x130                         ; -> obj2:0x0130
        push   0
        call   sub_3_0EB0
        add    esp, 0x10
        mov    esi, dword ptr [4]            ; -> obj2:0x0004
        or     esi, esi
        je     loc_3_0AF3
        mov    ebx, 0x40
loc_3_0A19:
        test   byte ptr [esi + 8], bl
        jne    loc_3_0AE9
        mov    ecx, dword ptr [esi + 4]
        cmp    cx, 0x6126
        je     loc_3_0AE9
        cmp    ecx, 0x40026356
        je     loc_3_0AE9
        cmp    cx, 0x8906
        je     loc_3_0AE9
        cmp    ecx, 0x120835c
        je     loc_3_0AE9
        cmp    cx, 0x9004
        jne    loc_3_0A7A
        mov    eax, ecx
        shr    eax, 0x10
        or     ax, ax
        je     loc_3_0AE9
        cmp    ax, 0x100
        je     loc_3_0AE9
        cmp    ax, 0x200
        je     loc_3_0AE9
        cmp    ax, 4
        je     loc_3_0AE9
loc_3_0A7A:
        mov    dl, byte ptr [esi + 9]
        call   sub_3_06BD
        push   0
        push   dword ptr [8]                 ; -> obj2:0x0008
        push   eax
        lea    eax, [esp + 0x14]
        push   eax
        call   sub_3_0EB0
        add    esp, 0x10
        or     eax, eax
        jne    loc_3_0AE9
        mov    al, byte ptr [esi + 9]
        mov    byte ptr [esp + 0xc], al
        mov    byte ptr [esp + 0xd], 0
        movzx  ax, byte ptr [esi + 0x2d]
        mov    word ptr [esp + 0xe], ax
        lea    eax, [esp + 0xc]
        push   0
        push   eax
        push   4
        push   2
        push   dword ptr [esp + 0x18]
        call   sub_3_0F40
        add    esp, 0x14
        push   0
        push   esi
        push   dword ptr [8]                 ; -> obj2:0x0008
        push   dword ptr [esp + 0x14]
        call   sub_3_0EA8
        mov    edx, dword ptr [esp + 0x18]
        add    esp, 0x10
        mov    ecx, esi
        call   sub_3_0816
loc_3_0AE9:
        mov    esi, dword ptr [esi]
        or     esi, esi
        jne    loc_3_0A19
loc_3_0AF3:
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
loc_3_0AF9:
        sub    esp, 0xc
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x24]
        mov    edi, dword ptr [esp + 0x20]
        cmp    esi, edi
        jne    loc_3_0B61
        mov    eax, dword ptr [esp + 0x18]
        cmp    eax, 4
        je     loc_3_0B22
        cmp    eax, 5
        je     loc_3_0B37
        mov    eax, 1
        jmp    loc_3_0BF3
loc_3_0B22:
        push   0
        push   dword ptr [0]                 ; -> obj2:0x0000
        call   sub_3_0F00
        add    esp, 8
        jmp    loc_3_0BF1
loc_3_0B37:
        push   0
        push   dword ptr [8]                 ; -> obj2:0x0008
        call   sub_3_0F20
        add    esp, 8
        call   sub_3_09E5
        push   0
        push   dword ptr [8]                 ; -> obj2:0x0008
        call   sub_3_0F28
        add    esp, 8
        jmp    loc_3_0BF1
loc_3_0B61:
        push   0
        lea    eax, [esp + 0x14]
        push   esi
        push   eax
        call   sub_3_0E98
        add    esp, 0xc
        cmp    dword ptr [esp + 0x10], edi
        je     loc_3_0B7E
        mov    eax, 1
        jmp    loc_3_0BF3
loc_3_0B7E:
        push   0
        push   dword ptr [8]                 ; -> obj2:0x0008
        lea    eax, [esp + 0x10]
        push   esi
        push   eax
        call   sub_3_0EA0
        add    esp, 0x10
        cmp    dword ptr [esp + 8], 0
        jne    loc_3_0BA2
        mov    eax, 1
        jmp    loc_3_0BF3
loc_3_0BA2:
        mov    eax, dword ptr [esp + 0x18]
        cmp    eax, 6
        je     loc_3_0BB7
        cmp    eax, 0xf
        je     loc_3_0BC4
        mov    eax, 1
        jmp    loc_3_0BF3
loc_3_0BB7:
        mov    edx, dword ptr [esp + 8]
        mov    ecx, esi
        call   sub_3_022F
        jmp    loc_3_0BF1
loc_3_0BC4:
        push   3
        lea    eax, [esp + 0x10]
        push   esi
        push   eax
        call   sub_3_0ED8
        add    esp, 0xc
        or     eax, eax
        jne    loc_3_0BE6
        push   0
        push   dword ptr [esp + 0x10]
        call   sub_3_0ED0
        add    esp, 8
loc_3_0BE6:
        mov    edx, esi
        mov    ecx, dword ptr [esp + 8]
        call   sub_3_0816
loc_3_0BF1:
        sub    eax, eax
loc_3_0BF3:
        pop    edi
        pop    esi
        add    esp, 0xc
        ret    
        db  000h, 000h, 000h
sub_3_0BFC:
        VMMCall  VMM.VMM_GetSystemInitState      ; cd2011010100
        cmp    eax, 0x20000000
        jae    loc_3_0C40
        push   0
        push   0x740
        VMMCall  VMM._Allocate_Temp_V86_Data_Area ; cd20a9000100
        pop    ecx
        pop    ecx
        or     eax, eax
        je     loc_3_0C3F
        mov    byte ptr [0x8c], 1            ; -> obj2:0x008C
        shr    eax, 4
loc_3_0C26:
        mov    word ptr [0x88], ax           ; -> obj2:0x0088
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [0x84], eax         ; -> obj2:0x0084
        mov    eax, 1
loc_3_0C3F:
        ret    
loc_3_0C40:
        push   0x740
        mov    eax, esp
        push   eax
        push   4
        push   0xb8
        push   0x91                          ; -> loc_2_0091
        VxDCall  SHELL._SHELL_CallDll            ; cd2018001700
        add    esp, 0x10
        add    esp, 4
        or     ax, ax
        je     loc_3_0C3F
        mov    word ptr [0x8a], ax           ; -> obj2:0x008A
        shr    eax, 0x10
        mov    byte ptr [0x8c], 0            ; -> obj2:0x008C
        jmp    loc_3_0C26
sub_3_0C77:
        cmp    byte ptr [0x8c], 1            ; -> obj2:0x008C
        je     loc_3_0CA3
        push   ax
        push   word ptr [0x8a]               ; -> obj2:0x008A
        mov    eax, esp
        push   eax
        push   2
        push   0xb9
        push   0x91                          ; -> loc_2_0091
        VxDCall  SHELL._SHELL_CallDll            ; cd2018001700
        add    esp, 0x10
        pop    eax
        ret    
loc_3_0CA3:
        VMMCall  VMM._Free_Temp_V86_Data_Area    ; cd20aa000100
        ret    
sub_3_0CAA:
        cmp    dword ptr [0x8d], 0           ; -> loc_2_008D
        jne    loc_3_0CD2
        mov    ax, word ptr [ebp + 0x2c]
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        mov    cx, 0xf000
        mov    dx, 0xf859
        VMMCall  VMM.Simulate_Far_Call           ; cd2048000100
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        ret    
loc_3_0CD2:
        mov    eax, 0x15
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        ret    
sub_3_0CDE:
        push   ebp
        push   ebx
        push   esi
        push   edi
        mov    ebp, dword ptr [0x84]         ; -> obj2:0x0084
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        mov    word ptr [ebp + 0x1c], 0xd800
        mov    al, byte ptr [esp + 0x80]
        mov    byte ptr [ebp + 0x18], al
        mov    ax, word ptr [0x88]           ; -> obj2:0x0088
        mov    word ptr [ebp + 0x34], ax
        mov    word ptr [ebp + 0x30], 0x600
        call   sub_3_0CAA
        xor    al, al
        test   dword ptr [ebp + 0x2c], 1
        jne    loc_3_0D5E
        mov    bx, word ptr [ebp + 4]
        shl    ebx, 0x10
        mov    bx, word ptr [ebp]
        cmp    ebx, 0x1044110e
        je     loc_3_0D46
        test   byte ptr [ebp + 0x1c], 0x30
        jne    loc_3_0D5E
loc_3_0D46:
        mov    ecx, dword ptr [esp + 0x84]
        mov    dword ptr [ecx], ebx
        mov    ecx, dword ptr [esp + 0x88]
        mov    al, byte ptr [ebp + 0x1c]
        mov    byte ptr [ecx], al
        mov    al, byte ptr [ebp + 0x15]
loc_3_0D5E:
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
sub_3_0D78:
        push   ebp
        push   ebx
        push   esi
        push   edi
        mov    ebp, dword ptr [0x84]         ; -> obj2:0x0084
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        mov    word ptr [ebp + 0x1c], 0xd801
        mov    al, byte ptr [esp + 0x80]
        mov    ah, byte ptr [esp + 0x84]
        mov    word ptr [ebp + 0x18], ax
        mov    ax, word ptr [0x88]           ; -> obj2:0x0088
        mov    word ptr [ebp + 0x34], ax
        mov    word ptr [ebp + 0x30], 0x600
        mov    word ptr [ebp + 0x3c], ax
        mov    word ptr [ebp + 4], 0x600
        call   sub_3_0CAA
        xor    eax, eax
        test   dword ptr [ebp + 0x2c], 1
        jne    loc_3_0DE9
        xor    eax, eax
        mov    ax, word ptr [0x88]           ; -> obj2:0x0088
        shl    eax, 4
        add    eax, 0x600
loc_3_0DE9:
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_3_0E10:
        push   0
        mov    dword ptr [0x98], 1           ; -> obj2:0x0098
        push   dword ptr [esp + 8]
        call   sub_3_0F18
        add    esp, 8
        mov    eax, 1
        ret    4
sub_3_0E30:
        cmp    dword ptr [esp + 8], 1
        je     loc_3_0E3E
        mov    eax, 1
        jmp    loc_3_0E55
loc_3_0E3E:
        cmp    dword ptr [0x98], 0           ; -> obj2:0x0098
        mov    eax, 0x21
        je     loc_3_0E55
        mov    ecx, dword ptr [esp + 4]
        call   0
loc_3_0E55:
        ret    8
        db  0CDh, 020h, 06Ch, 080h, 001h, 000h, 000h, 000h
sub_3_0E60:
        VMMCall  VMM._HeapAllocate               ; cd204f800100
        add    byte ptr [eax], al
sub_3_0E68:
        VMMCall  VMM._HeapFree                   ; cd2051800100
        add    byte ptr [eax], al
sub_3_0E70:
        VMMCall  VMM._HeapReAllocate             ; cd2050800100
        add    byte ptr [eax], al
sub_3_0E78:
        push   edi
        mov    edi, dword ptr [esp + 0xc]
        movzx  eax, word ptr [esp + 8]
        VMMCall  VMM.Get_DDB                     ; cd2046010100
        mov    eax, ecx
        pop    edi
        ret    8
        db  000h, 000h
sub_3_0E90:
        VMMCall  VMM._Sprintf                    ; cd2074810100
        add    byte ptr [eax], al
sub_3_0E98:
        VxDCall  CONFIGMG._CONFIGMG_Get_Parent   ; cd2003803300
        add    byte ptr [eax], al
sub_3_0EA0:
        VxDCall  CONFIGMG._CONFIGMG_Get_Private_DWord ; cd2009803300
        add    byte ptr [eax], al
sub_3_0EA8:
        VxDCall  CONFIGMG._CONFIGMG_Set_Private_DWord ; cd200a803300
        add    byte ptr [eax], al
sub_3_0EB0:
        VxDCall  CONFIGMG._CONFIGMG_Create_DevNode ; cd200b803300
        add    byte ptr [eax], al
        VxDCall  CONFIGMG._CONFIGMG_Register_Device_Driver ; cd200e803300
        add    byte ptr [eax], al
        VxDCall  CONFIGMG._CONFIGMG_Register_Enumerator ; cd200f803300
        add    byte ptr [eax], al
sub_3_0EC8:
        VxDCall  CONFIGMG._CONFIGMG_Add_Empty_Log_Conf ; cd2018803300
        add    byte ptr [eax], al
sub_3_0ED0:
        VxDCall  CONFIGMG._CONFIGMG_Free_Log_Conf ; cd2019803300
        add    byte ptr [eax], al
sub_3_0ED8:
        VxDCall  CONFIGMG._CONFIGMG_Get_First_Log_Conf ; cd201a803300
        add    byte ptr [eax], al
sub_3_0EE0:
        VxDCall  CONFIGMG._CONFIGMG_Add_Res_Des  ; cd201c803300
        add    byte ptr [eax], al
        VxDCall  CONFIGMG._CONFIGMG_Create_Range_List ; cd2024803300
        add    byte ptr [eax], al
sub_3_0EF0:
        VxDCall  CONFIGMG._CONFIGMG_Add_Range    ; cd2025803300
        add    byte ptr [eax], al
sub_3_0EF8:
        VxDCall  CONFIGMG._CONFIGMG_Delete_Range ; cd2026803300
        add    byte ptr [eax], al
sub_3_0F00:
        VxDCall  CONFIGMG._CONFIGMG_Free_Range_List ; cd2029803300
        add    byte ptr [eax], al
sub_3_0F08:
        VxDCall  CONFIGMG._CONFIGMG_First_Range  ; cd202c803300
        add    byte ptr [eax], al
sub_3_0F10:
        VxDCall  CONFIGMG._CONFIGMG_Next_Range   ; cd202d803300
        add    byte ptr [eax], al
sub_3_0F18:
        VxDCall  CONFIGMG._CONFIGMG_Register_DevLoader ; cd2032803300
        add    byte ptr [eax], al
sub_3_0F20:
        VxDCall  CONFIGMG._CONFIGMG_Reset_Children_Marks ; cd2035803300
        add    byte ptr [eax], al
sub_3_0F28:
        VxDCall  CONFIGMG._CONFIGMG_Remove_Unmarked_Children ; cd2037803300
        add    byte ptr [eax], al
sub_3_0F30:
        VxDCall  CONFIGMG._CONFIGMG_Read_Registry_Value ; cd203e803300
        add    byte ptr [eax], al
sub_3_0F38:
        VxDCall  CONFIGMG._CONFIGMG_Write_Registry_Value ; cd203f803300
        add    byte ptr [eax], al
sub_3_0F40:
        VxDCall  Dev_2133.Service_8043h          ; cd2043803321
VxD__LTEXT ENDS

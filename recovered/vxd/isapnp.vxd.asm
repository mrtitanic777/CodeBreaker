; ISAPNP -- reconstructed 32-bit disassembly (VxD / LE format)
; 4 objects, 1902 instructions recovered by recursive descent

Declare_Virtual_Device ISAPNP, 4, 0, ISAPNP_Control, 0003Ch, Undefined_Init_Order
;   DDK version 0400h   init_order 016800000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x7c  R-X
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  0003Ch                      ; DDB_Req_Device_Number
        db  4, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'ISAPNP  '                  ; DDB_Name
        dd  016800000h                  ; DDB_Init_Order
        dd  ISAPNP_Control           ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
ISAPNP_Control:
        cmp    eax, 0x1b
        jne    loc_1_006A
        call   sub_4_0000
loc_1_0066:
        cmp    eax, 1
        ret    
loc_1_006A:
        cmp    eax, 0x22
        jne    loc_1_0078
        push   edx
        push   ebx
        call   sub_2_13C1
loc_1_0076:
        stc    
        ret    
loc_1_0078:
        clc    
        ret    
        db  0CCh, 0CCh
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x14ea  R-X
        db  056h, 0B8h, 013h, 000h, 000h, 000h, 083h, 03Dh
        dd  obj3:0x0000              ; fixup
        db  000h, 075h, 054h, 033h, 0F6h, 089h, 00Dh
        dd  obj3:0x0000              ; fixup
        db  056h, 056h, 068h
        dd  obj3:0x0014              ; fixup
        db  0E8h, 0D1h, 013h, 000h, 000h, 083h, 0C4h, 00Ch, 0A1h
        dd  obj3:0x0000              ; fixup
        db  056h, 068h
        dd  loc_2_064B               ; fixup
        db  050h, 0E8h, 0B5h, 013h, 000h, 000h, 083h, 0C4h, 00Ch, 0A1h
        dd  obj3:0x0000              ; fixup
        db  06Ah, 001h, 068h
        dd  loc_2_04B4               ; fixup
        db  050h, 0E8h, 098h, 013h, 000h, 000h, 083h, 0C4h, 00Ch, 0A1h
        dd  obj3:0x0000              ; fixup
        db  06Ah, 002h, 056h, 056h, 050h, 0E8h, 07Eh, 013h, 000h, 000h, 083h, 0C4h, 010h, 033h, 0C0h, 05Eh
        db  0C3h
loc_2_0065:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        cmp    eax, 1
        je     loc_2_0085
        cmp    eax, 2
        je     loc_2_00D3
        cmp    eax, 4
        je     loc_2_00C9
        mov    eax, 1
        jmp    loc_2_00DE
loc_2_0085:
        mov    eax, dword ptr [0]            ; -> obj3:0x0004
        test   eax, eax
        je     loc_2_00D3
        push   2
        lea    ecx, [ebp - 4]
        push   eax
        push   ecx
        call   sub_2_1404
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_00D3
        push   0
        lea    ecx, [ebp - 4]
        push   0
        push   2
        push   dword ptr [ebp - 4]
        push   ecx
        call   sub_2_13FC
        add    esp, 0x14
        test   eax, eax
        jne    loc_2_00D3
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 4]
        mov    word ptr [0], ax              ; -> obj1:0x0058
        jmp    loc_2_00DC
loc_2_00C9:
        mov    dword ptr [0], 0              ; -> obj3:0x0004
loc_2_00D3:
        mov    word ptr [0], 0               ; -> obj1:0x0058
loc_2_00DC:
        xor    eax, eax
loc_2_00DE:
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_00E2:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        lea    eax, [ebp - 0x10]
        push   esi
        push   edi
        push   2
        push   ecx
        push   eax
        call   sub_2_1404
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_03C4
        mov    eax, dword ptr [ebp - 0x10]
        mov    bl, 0x40
        mov    esi, 0x279
        mov    edi, 0xa79
        mov    dword ptr [ebp - 0xc], 0
        mov    dword ptr [ebp - 4], eax
loc_2_011B:
        cmp    dword ptr [ebp - 0xc], 4
        jae    loc_2_01C0
loc_2_0125:
        push   0
        lea    ecx, [ebp - 4]
        push   0
        push   1
        push   dword ptr [ebp - 4]
        push   ecx
        call   sub_2_13FC
        add    esp, 0x14
        test   eax, eax
        jne    loc_2_0181
        mov    ecx, dword ptr [ebp - 4]
        test   byte ptr [ecx + 0xc], 2
        jne    loc_2_0125
        test   eax, eax
        jne    loc_2_0181
        mov    eax, dword ptr [ebp - 4]
        mov    edx, esi
        mov    ecx, dword ptr [eax + 4]
        mov    eax, dword ptr [eax + 8]
        shr    ecx, 8
        shr    eax, 8
        mov    dword ptr [ebp - 8], eax
        lea    eax, [ebx + 2]
        out    dx, al
        mov    dx, word ptr [0]              ; -> obj1:0x0058
        in     al, dx
        test   al, 1
        jne    loc_2_017C
        sub    dword ptr [ebp - 8], ecx
        mov    eax, dword ptr [ebp - 8]
        not    eax
        mov    dword ptr [ebp - 8], eax
        jmp    loc_2_0186
loc_2_017C:
        inc    dword ptr [ebp - 8]
        jmp    loc_2_0186
loc_2_0181:
        xor    ecx, ecx
        mov    dword ptr [ebp - 8], ecx
loc_2_0186:
        lea    eax, [ebx + 1]
        mov    edx, esi
        out    dx, al
        mov    edx, edi
        mov    al, cl
        out    dx, al
        mov    edx, esi
        mov    al, bl
        out    dx, al
        mov    edx, edi
        mov    al, ch
        out    dx, al
        mov    cx, word ptr [ebp - 8]
        lea    eax, [ebx + 4]
        mov    edx, esi
        out    dx, al
        mov    edx, edi
        mov    al, cl
        out    dx, al
        lea    eax, [ebx + 3]
        mov    edx, esi
        add    bl, 8
        inc    dword ptr [ebp - 0xc]
        out    dx, al
        mov    edx, edi
        mov    al, ch
        out    dx, al
        jmp    loc_2_011B
loc_2_01C0:
        mov    eax, dword ptr [ebp - 0x10]
        mov    esi, 0x279
        mov    edi, 0xa79
        mov    dword ptr [ebp - 4], eax
        mov    dword ptr [ebp - 0xc], 0
        mov    byte ptr [ebp - 0x18], 0x70
loc_2_01DB:
        cmp    dword ptr [ebp - 0xc], 4
        jae    loc_2_02CE
loc_2_01E5:
        push   0
        lea    ecx, [ebp - 4]
        push   0
        push   1
        push   dword ptr [ebp - 4]
        push   ecx
        call   sub_2_13FC
        add    esp, 0x14
        test   eax, eax
        jne    loc_2_0207
        mov    ecx, dword ptr [ebp - 4]
        test   byte ptr [ecx + 0xc], 2
        je     loc_2_01E5
loc_2_0207:
        cmp    dword ptr [ebp - 0xc], 1
        sbb    bl, bl
        and    bl, 6
        add    bl, byte ptr [ebp - 0x18]
        test   eax, eax
        jne    loc_2_0247
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [eax + 4]
        mov    edx, dword ptr [eax + 8]
        lea    eax, [ebx + 4]
        mov    dword ptr [ebp - 8], edx
        mov    edx, esi
        out    dx, al
        mov    dx, word ptr [0]              ; -> obj1:0x0058
        in     al, dx
        test   al, 1
        jne    loc_2_0242
        sub    dword ptr [ebp - 8], ecx
        mov    eax, dword ptr [ebp - 8]
        not    eax
        mov    dword ptr [ebp - 8], eax
        jmp    loc_2_024C
loc_2_0242:
        inc    dword ptr [ebp - 8]
        jmp    loc_2_024C
loc_2_0247:
        xor    ecx, ecx
        mov    dword ptr [ebp - 8], ecx
loc_2_024C:
        lea    eax, [ebx + 3]
        mov    edx, esi
        out    dx, al
        mov    edx, edi
        mov    al, cl
        out    dx, al
        lea    eax, [ebx + 2]
        mov    edx, esi
        out    dx, al
        mov    edx, edi
        mov    al, ch
        out    dx, al
        lea    eax, [ebx + 1]
        mov    edx, esi
        out    dx, al
        mov    edx, edi
        mov    dword ptr [ebp - 0x14], ecx
        shr    dword ptr [ebp - 0x14], 0x10
        mov    al, byte ptr [ebp - 0x14]
        out    dx, al
        mov    edx, esi
        mov    al, bl
        out    dx, al
        mov    edx, edi
        mov    dword ptr [ebp - 0x14], ecx
        shr    dword ptr [ebp - 0x14], 0x10
        mov    cx, word ptr [ebp - 8]
        mov    al, byte ptr [ebp - 0x13]
        out    dx, al
        lea    eax, [ebx + 8]
        mov    edx, esi
        out    dx, al
        mov    edx, edi
        mov    al, cl
        out    dx, al
        lea    eax, [ebx + 7]
        mov    edx, esi
        out    dx, al
        mov    edx, edi
        mov    al, ch
        out    dx, al
        lea    eax, [ebx + 6]
        mov    edx, esi
        out    dx, al
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, edi
        shr    ecx, 0x10
        mov    al, cl
        out    dx, al
        lea    eax, [ebx + 5]
        mov    edx, esi
        out    dx, al
        mov    edx, edi
        mov    al, ch
        out    dx, al
        movzx  eax, byte ptr [ebp - 0x18]
        add    al, 0x10
        inc    dword ptr [ebp - 0xc]
        mov    byte ptr [ebp - 0x18], al
        jmp    loc_2_01DB
loc_2_02CE:
        mov    eax, dword ptr [ebp - 0x10]
        mov    bl, 0x60
        mov    esi, 8
        mov    edi, 0x279
        mov    dword ptr [ebp - 4], eax
loc_2_02E0:
        push   0
        lea    ecx, [ebp - 4]
        push   0
        push   2
        push   dword ptr [ebp - 4]
        push   ecx
        call   sub_2_13FC
        add    esp, 0x14
        mov    ecx, 0
        test   eax, eax
        jne    loc_2_0305
        mov    eax, dword ptr [ebp - 4]
        movzx  ecx, word ptr [eax + 4]
loc_2_0305:
        lea    eax, [ebx + 1]
        mov    edx, edi
        out    dx, al
        mov    edx, 0xa79
        mov    al, cl
        out    dx, al
        mov    edx, edi
        mov    al, bl
        out    dx, al
        mov    edx, 0xa79
        mov    al, ch
        out    dx, al
        add    bl, 2
        dec    esi
        jne    loc_2_02E0
        mov    eax, dword ptr [ebp - 0x10]
        mov    bl, 0x70
        mov    esi, 2
        mov    edi, 0x279
        mov    dword ptr [ebp - 4], eax
loc_2_0338:
        push   0
        lea    ecx, [ebp - 4]
        push   0
        push   4
        push   dword ptr [ebp - 4]
        push   ecx
        call   sub_2_13FC
        add    esp, 0x14
        mov    ecx, 0
        test   eax, eax
        jne    loc_2_035D
        mov    eax, dword ptr [ebp - 4]
        movzx  ecx, word ptr [eax + 2]
loc_2_035D:
        mov    edx, edi
        mov    al, bl
        out    dx, al
        mov    edx, 0xa79
        mov    al, cl
        out    dx, al
        lea    eax, [ebx + 1]
        mov    edx, edi
        out    dx, al
        mov    edx, 0xa79
        mov    al, 2
        out    dx, al
        add    bl, al
        dec    esi
        jne    loc_2_0338
        mov    eax, dword ptr [ebp - 0x10]
        mov    bl, 0x74
        mov    esi, 2
        mov    dword ptr [ebp - 4], eax
loc_2_038A:
        push   0
        lea    ecx, [ebp - 4]
        push   0
        push   3
        push   dword ptr [ebp - 4]
        push   ecx
        call   sub_2_13FC
        add    esp, 0x14
        mov    ecx, 4
        test   eax, eax
        jne    loc_2_03AF
        mov    eax, dword ptr [ebp - 4]
        movzx  ecx, byte ptr [eax + 1]
loc_2_03AF:
        mov    edx, 0x279
        mov    al, bl
        out    dx, al
        mov    edx, 0xa79
        mov    al, cl
        out    dx, al
        inc    bl
        dec    esi
        jne    loc_2_038A
loc_2_03C4:
        mov    edx, 0x279
        mov    al, 0x30
        out    dx, al
        mov    edx, 0xa79
        mov    al, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        out    dx, al
        pop    ebp
        ret    
sub_2_03DB:
        push   ebx
        push   esi
        mov    bl, 0x70
        mov    esi, 2
loc_2_03E4:
        mov    edx, 0x279
        mov    al, bl
        out    dx, al
        mov    edx, 0xa79
        xor    al, al
        out    dx, al
        lea    eax, [ebx + 1]
        mov    edx, 0x279
        out    dx, al
        mov    edx, 0xa79
        mov    al, 2
        out    dx, al
        add    bl, al
        dec    esi
        jne    loc_2_03E4
        mov    edx, 0x279
        mov    al, 0x30
        out    dx, al
        mov    edx, 0xa79
        xor    al, al
        out    dx, al
        push   1
        push   0x100000
        push   ecx
        call   sub_2_140C
        add    esp, 0xc
        pop    esi
        pop    ebx
        ret    
sub_2_042D:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc8
        cmp    byte ptr [ebp + 8], 1
        push   ebx
        push   esi
        mov    bl, dl
        mov    esi, ecx
        jne    loc_2_048F
        push   0
        lea    eax, [ebp - 0xc8]
        push   0xc8
        push   eax
        push   esi
        call   sub_2_1424
        add    esp, 0x10
        xor    eax, eax
loc_2_045B:
        cmp    byte ptr [ebp + eax - 0xc8], 0x5c
        je     loc_2_0468
        inc    eax
        jmp    loc_2_045B
loc_2_0468:
        xor    ecx, ecx
        push   ecx
        mov    byte ptr [ebp + eax - 0xc0], cl
        lea    eax, [ebp + eax - 0xc8]
        push   ecx
        push   eax
        push   1
        push   0                             ; -> obj3:0x0008
        mov    byte ptr [eax], 0x2a
        push   ecx
        push   esi
        call   sub_2_141C
        add    esp, 0x1c
loc_2_048F:
        call   sub_2_0917
        movzx  eax, bl
        push   1
        push   eax
        push   esi
        mov    eax, dword ptr [0]            ; -> obj1:0x0054
        add    eax, 9
        push   eax
        call   sub_2_1414
        add    esp, 0x10
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_04B4:
        push   ebp
        mov    eax, dword ptr [0]            ; -> obj3:0x0000
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        cmp    eax, dword ptr [ebp + 0x14]
        push   esi
        push   edi
        jne    loc_2_0543
        mov    eax, dword ptr [ebp + 8]
        cmp    eax, 1
        je     loc_2_04E8
        cmp    eax, 2
        je     loc_2_050E
        cmp    eax, 4
        je     loc_2_052A
        cmp    eax, 5
        je     loc_2_0539
        mov    eax, 1
        jmp    loc_2_0644
loc_2_04E8:
        xor    eax, eax
        push   eax
        push   eax
        push   eax
        push   eax
        push   eax
        push   eax
        push   1
        push   1
        call   sub_2_1444
        add    esp, 0x20
        mov    dword ptr [0], eax            ; -> obj1:0x0054
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 2
        jmp    loc_2_0644
loc_2_050E:
        xor    esi, esi
        mov    eax, dword ptr [0]            ; -> obj1:0x0054
        push   esi
        push   eax
        call   sub_2_143C
        add    esp, 8
        mov    dword ptr [0], esi            ; -> obj1:0x0054
        jmp    loc_2_0642
loc_2_052A:
        mov    dword ptr [0], 0              ; -> obj3:0x0000
        jmp    loc_2_0642
loc_2_0539:
        call   sub_2_1032
        jmp    loc_2_0642
loc_2_0543:
        push   0
        lea    edx, [ebp - 8]
        push   eax
        push   dword ptr [ebp + 0x14]
        push   edx
        call   sub_2_1434
        add    esp, 0x10
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_0565
        mov    eax, 1
        jmp    loc_2_0644
loc_2_0565:
        mov    al, byte ptr [ebp - 8]
        mov    bl, byte ptr [ebp - 7]
        mov    byte ptr [ebp - 1], al
        mov    eax, dword ptr [ebp - 8]
        shr    eax, 0x10
        mov    byte ptr [ebp - 5], al
        mov    eax, dword ptr [ebp + 8]
        cmp    eax, 1
        jb     loc_2_058E
        cmp    eax, 2
        jbe    loc_2_0598
        cmp    eax, 4
        je     loc_2_0598
        cmp    eax, 6
        je     loc_2_0598
loc_2_058E:
        mov    eax, 1
        jmp    loc_2_0644
loc_2_0598:
        mov    esi, 0x279
        call   sub_2_07E1
        mov    edx, esi
        mov    al, 2
        mov    edi, 0xa79
        push   0
        out    dx, al
        mov    edx, edi
        push   0x7d0
        out    dx, al
        call   sub_2_142C
        add    esp, 8
        call   sub_2_07E1
        mov    edx, esi
        mov    al, 3
        out    dx, al
        mov    edx, edi
        mov    al, byte ptr [ebp - 1]
        out    dx, al
        mov    edx, esi
        mov    al, 7
        out    dx, al
        mov    edx, edi
        mov    al, bl
        out    dx, al
        mov    eax, dword ptr [ebp + 8]
        cmp    eax, 1
        je     loc_2_05F1
        cmp    eax, 2
        je     loc_2_060C
        cmp    eax, 4
        je     loc_2_060C
        cmp    eax, 6
        je     loc_2_0616
        jmp    loc_2_0634
loc_2_05F1:
        cmp    word ptr [0], 0               ; -> obj1:0x0058
        jne    loc_2_0602
        mov    eax, 0x13
        jmp    loc_2_0644
loc_2_0602:
        mov    ecx, dword ptr [ebp + 0x14]
        call   sub_2_00E2
        jmp    loc_2_0634
loc_2_060C:
        mov    ecx, dword ptr [ebp + 0x14]
        call   sub_2_03DB
        jmp    loc_2_0634
loc_2_0616:
        cmp    word ptr [0], 0               ; -> obj1:0x0058
        jne    loc_2_0627
        mov    eax, 0x13
        jmp    loc_2_0644
loc_2_0627:
        mov    edx, ebx
        mov    ecx, dword ptr [ebp + 0x14]
        push   dword ptr [ebp - 5]
        call   sub_2_042D
loc_2_0634:
        mov    edx, 0x279
        mov    al, 2
        out    dx, al
        mov    edx, 0xa79
        out    dx, al
loc_2_0642:
        xor    eax, eax
loc_2_0644:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_064B:
        push   ebp
        mov    eax, dword ptr [0]            ; -> obj3:0x0004
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        cmp    dword ptr [ebp + 0x10], eax
        push   esi
        push   edi
        jne    loc_2_0668
        mov    eax, 0x13
        jmp    loc_2_07C3
loc_2_0668:
        cmp    word ptr [0], 0               ; -> obj1:0x0058
        jne    loc_2_067C
        mov    eax, 0x13
        jmp    loc_2_07C3
loc_2_067C:
        push   0
        mov    eax, dword ptr [0]            ; -> obj3:0x0000
        push   eax
        lea    edx, [ebp - 4]
        push   dword ptr [ebp + 0x10]
        push   edx
        call   sub_2_1434
        add    esp, 0x10
        cmp    dword ptr [ebp - 4], 0
        jne    loc_2_06A3
        mov    eax, 0x13
        jmp    loc_2_07C3
loc_2_06A3:
        mov    esi, 0x279
        call   sub_2_07E1
        mov    edx, esi
        mov    al, 2
        mov    edi, 0xa79
        push   0
        out    dx, al
        mov    edx, edi
        push   0x7d0
        out    dx, al
        call   sub_2_142C
        add    esp, 8
        call   sub_2_07E1
        mov    edx, esi
        mov    al, 3
        out    dx, al
        mov    edx, edi
        mov    al, byte ptr [ebp - 4]
        out    dx, al
        mov    edx, esi
        mov    al, 7
        out    dx, al
        mov    edx, edi
        mov    al, byte ptr [ebp - 3]
        out    dx, al
        mov    eax, dword ptr [ebp + 8]
        test   eax, eax
        je     loc_2_0703
        cmp    eax, 1
        je     loc_2_0745
        cmp    eax, 2
        je     loc_2_077E
        mov    ecx, 0x20
        jmp    loc_2_07B3
loc_2_0703:
        mov    ebx, dword ptr [ebp + 0x18]
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, ebx
        cmp    eax, 0x100
        jbe    loc_2_071C
        mov    ecx, 0x13
        jmp    loc_2_07B3
loc_2_071C:
        test   ebx, ebx
        je     loc_2_07B1
        mov    edi, dword ptr [ebp + 0x14]
loc_2_0727:
        mov    edx, 0x279
        mov    al, byte ptr [ebp + 0xc]
        out    dx, al
        dec    ebx
        inc    dword ptr [ebp + 0xc]
        mov    dx, word ptr [0]              ; -> obj1:0x0058
        in     al, dx
        mov    byte ptr [edi], al
        inc    edi
        test   ebx, ebx
        jne    loc_2_0727
        jmp    loc_2_07B1
loc_2_0745:
        mov    ebx, dword ptr [ebp + 0x18]
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, ebx
        cmp    eax, 0x100
        jbe    loc_2_075B
        mov    ecx, 0x13
        jmp    loc_2_07B3
loc_2_075B:
        test   ebx, ebx
        je     loc_2_07B1
        mov    edi, dword ptr [ebp + 0x14]
loc_2_0762:
        mov    edx, 0x279
        mov    al, byte ptr [ebp + 0xc]
        dec    ebx
        inc    dword ptr [ebp + 0xc]
        out    dx, al
        mov    edx, 0xa79
        mov    al, byte ptr [edi]
        out    dx, al
        inc    edi
        test   ebx, ebx
        jne    loc_2_0762
        jmp    loc_2_07B1
loc_2_077E:
        mov    ebx, dword ptr [ebp + 0x18]
        mov    eax, dword ptr [ebp + 0xc]
        add    eax, ebx
        mov    ecx, 0x13
        cmp    eax, 0x1000
        ja     loc_2_07B3
        call   sub_2_0917
        mov    esi, dword ptr [ebp + 0xc]
        mov    edi, dword ptr [ebp + 0x14]
        add    esi, dword ptr [0]            ; -> obj1:0x0054
        mov    ecx, ebx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_2_07B1:
        xor    ecx, ecx
loc_2_07B3:
        mov    edx, 0x279
        mov    al, 2
        out    dx, al
        mov    edx, 0xa79
        out    dx, al
        mov    eax, ecx
loc_2_07C3:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_07CA:
        lea    eax, [ecx*2]
        add    dl, dl
        xor    al, cl
        shr    cl, 1
        and    al, 0xfe
        xor    al, dl
        shl    al, 6
        or     al, cl
        ret    
sub_2_07E1:
        push   ebx
        mov    cl, 0x6a
        mov    edx, 0x279
        xor    al, al
        out    dx, al
        out    dx, al
        mov    bl, 0x20
loc_2_07EF:
        mov    edx, 0x279
        mov    al, cl
        out    dx, al
        xor    edx, edx
        call   sub_2_07CA
        mov    cl, al
        dec    bl
        jne    loc_2_07EF
        pop    ebx
        ret    
sub_2_0806:
        sub    esp, 4
        xor    al, al
        push   ebx
        push   esi
        push   edi
        mov    esi, 1
        push   ebp
        mov    edx, 0x80
        mov    ebp, dword ptr [0]            ; -> obj1:0x0054
        lea    edi, [ebp + 9]
loc_2_0822:
        mov    ecx, edi
        sub    ecx, ebp
        cmp    ecx, 0xffd
        jae    loc_2_087A
        mov    cl, byte ptr [edi]
        mov    bl, cl
        and    bl, dl
        cmp    bl, dl
        jne    loc_2_0841
        movzx  ebx, word ptr [edi + 1]
        add    ebx, 3
        jmp    loc_2_0849
loc_2_0841:
        xor    ebx, ebx
        mov    bl, cl
        and    ebx, 7
        inc    ebx
loc_2_0849:
        mov    dword ptr [esp + 0x10], ebx
        mov    bl, cl
        and    bl, dl
        cmp    bl, dl
        je     loc_2_0858
        and    cl, 0xf8
loc_2_0858:
        test   cl, cl
        je     loc_2_087A
        cmp    cl, 0xff
        je     loc_2_087A
        cmp    cl, 0x78
        je     loc_2_087A
        cmp    cl, 0x10
        jne    loc_2_086D
        inc    al
loc_2_086D:
        add    edi, dword ptr [esp + 0x10]
        inc    esi
        cmp    esi, 0x100
        jbe    loc_2_0822
loc_2_087A:
        test   al, al
        jne    loc_2_0880
        inc    al
loc_2_0880:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_2_0888:
        push   0
        push   0                             ; -> loc_2_0926
        push   0                             ; -> sub_2_0888
        call   sub_2_1454
        add    esp, 0xc
        mov    eax, dword ptr [0]            ; -> obj1:0x0054
        shr    eax, 0xc
        push   0
        push   1
        push   eax
        call   sub_2_144C
        add    esp, 0xc
        cli    
        ret    
loc_2_08B3:
        sti    
        push   0
        push   0                             ; -> loc_2_0926
        push   0                             ; -> sub_2_0888
        call   sub_2_1464
        add    esp, 0xc
        mov    eax, dword ptr [0]            ; -> obj1:0x0054
        shr    eax, 0xc
        push   0
        push   1
        push   eax
        call   sub_2_145C
        add    esp, 0xc
        ret    
sub_2_08DE:
        mov    cx, word ptr [0]              ; -> obj1:0x0058
        push   esi
        push   edi
        mov    esi, 0x1000
        mov    edi, dword ptr [0]            ; -> obj1:0x0054
loc_2_08F2:
        mov    edx, 0x279
        mov    al, 5
        out    dx, al
loc_2_08FA:
        mov    dx, cx
        in     al, dx
        test   al, 1
        je     loc_2_08FA
        mov    edx, 0x279
        mov    al, 4
        out    dx, al
        mov    dx, cx
        in     al, dx
        mov    byte ptr [edi], al
        inc    edi
        dec    esi
        jne    loc_2_08F2
        pop    edi
        pop    esi
        ret    
sub_2_0917:
        call   sub_2_0888
        call   sub_2_08DE
        jmp    loc_2_08B3
loc_2_0926:
        nop    
        ret    
sub_2_0928:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x44
        mov    word ptr [ebp - 0x26], 0xc
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        xor    ebx, ebx
        mov    word ptr [ebp - 0x28], bx
        mov    byte ptr [ebp - 0x1e], bl
        mov    word ptr [ebp - 0x24], bx
        mov    byte ptr [ebp - 0x1d], bl
        mov    word ptr [ebp - 0x22], bx
        mov    dword ptr [ebp - 0x38], ebx
        mov    word ptr [ebp - 0x20], bx
        mov    dword ptr [ebp - 0x34], ebx
        mov    word ptr [ebp - 0x3a], 0x14
        mov    word ptr [ebp - 0x3c], bx
        mov    word ptr [ebp - 0x30], bx
        mov    word ptr [ebp - 0x2e], bx
        mov    byte ptr [ebp - 0x10], bl
        mov    word ptr [ebp - 0x44], bx
        mov    byte ptr [ebp - 0xf], bl
        mov    word ptr [ebp - 0x42], bx
        mov    byte ptr [ebp - 0xe], bl
        mov    word ptr [ebp - 0x40], bx
        mov    edx, 0x279
        mov    word ptr [ebp - 0x3e], bx
        mov    al, 0x30
        out    dx, al
        mov    dx, word ptr [0]              ; -> obj1:0x0058
        mov    byte ptr [ebp - 0xd], bl
        in     al, dx
        test   al, 1
        je     loc_2_0D87
        push   ebx
        lea    eax, [ebp - 0x14]
        push   esi
        push   eax
        call   sub_2_1404
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_0D87
        push   3
        lea    eax, [ebp - 0xc]
        push   1
        push   esi
        push   eax
        call   sub_2_147C
        add    esp, 0x10
        test   eax, eax
        jne    loc_2_0D87
        mov    eax, dword ptr [ebp - 0x14]
        xor    esi, esi
        mov    byte ptr [ebp - 5], 0x40
        mov    dword ptr [ebp - 4], eax
loc_2_09D8:
        mov    edx, 0x279
        mov    al, byte ptr [ebp - 5]
        out    dx, al
        mov    dx, word ptr [0]              ; -> obj1:0x0058
        in     al, dx
        movzx  edi, al
        shl    edi, 8
        mov    al, byte ptr [ebp - 5]
        inc    al
        mov    edx, 0x279
        out    dx, al
        mov    dx, word ptr [0]              ; -> obj1:0x0058
        in     al, dx
        movzx  eax, al
        or     edi, eax
        shl    edi, 8
        test   edi, edi
        je     loc_2_0AB6
loc_2_0A12:
        push   0
        lea    eax, [ebp - 0x2c]
        push   eax
        lea    edx, [ebp - 4]
        push   0
        push   dword ptr [ebp - 4]
        push   edx
        call   sub_2_13FC
        add    esp, 0x14
        test   eax, eax
        je     loc_2_0A2F
        xor    edi, edi
loc_2_0A2F:
        test   edi, edi
        je     loc_2_0A68
        mov    eax, dword ptr [ebp - 0x2c]
        and    ah, 0x7f
        cmp    eax, 1
        jne    loc_2_0A12
        mov    eax, dword ptr [ebp - 4]
        test   byte ptr [eax + 0x20], 2
        jne    loc_2_0A12
        test   edi, edi
        je     loc_2_0AB6
        push   0
        lea    ecx, [ebp - 0x18]
        push   dword ptr [ebp - 4]
        push   ecx
        call   sub_2_1474
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_0A68
        cmp    dword ptr [ebp - 0x18], 0x24
        jae    loc_2_0A68
        xor    edi, edi
loc_2_0A68:
        test   edi, edi
        je     loc_2_0AB6
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0x38], edi
        add    ecx, 0x10
        push   0
        push   0x10
        mov    eax, dword ptr [ecx + 4]
        lea    edx, [edi + eax - 1]
        lea    eax, [ebp - 0x3c]
        push   eax
        mov    dword ptr [ebp - 0x34], edx
        mov    cx, word ptr [ecx + 0x10]
        push   1
        push   dword ptr [ebp - 0xc]
        mov    word ptr [ebp - 0x30], cx
        lea    ecx, [ebp - 0x1c]
        push   ecx
        call   sub_2_146C
        add    esp, 0x18
        test   eax, eax
        jne    loc_2_0D87
        add    byte ptr [ebp - 5], 8
        inc    esi
        cmp    esi, 4
        jb     loc_2_09D8
loc_2_0AB6:
        mov    eax, dword ptr [ebp - 0x14]
        mov    bl, 0x70
        mov    edi, 0x279
        mov    dword ptr [ebp - 4], eax
        mov    dword ptr [ebp - 8], 0
loc_2_0ACA:
        cmp    dword ptr [ebp - 8], 1
        sbb    cl, cl
        mov    edx, edi
        and    cl, 6
        add    cl, bl
        mov    al, cl
        out    dx, al
        mov    dx, word ptr [0]              ; -> obj1:0x0058
        in     al, dx
        movzx  esi, al
        shl    esi, 8
        lea    eax, [ecx + 1]
        mov    edx, edi
        out    dx, al
        mov    dx, word ptr [0]              ; -> obj1:0x0058
        in     al, dx
        movzx  eax, al
        or     esi, eax
        mov    edx, edi
        shl    esi, 8
        lea    eax, [ecx + 2]
        out    dx, al
        mov    dx, word ptr [0]              ; -> obj1:0x0058
        in     al, dx
        movzx  eax, al
        or     esi, eax
        mov    edx, edi
        shl    esi, 8
        lea    eax, [ecx + 3]
        out    dx, al
        mov    dx, word ptr [0]              ; -> obj1:0x0058
        in     al, dx
        movzx  eax, al
        or     esi, eax
        je     loc_2_0BD6
loc_2_0B2D:
        push   0
        lea    eax, [ebp - 0x2c]
        push   eax
        lea    edx, [ebp - 4]
        push   0
        push   dword ptr [ebp - 4]
        push   edx
        call   sub_2_13FC
        add    esp, 0x14
        test   eax, eax
        je     loc_2_0B4A
        xor    esi, esi
loc_2_0B4A:
        test   esi, esi
        je     loc_2_0B83
        mov    eax, dword ptr [ebp - 0x2c]
        and    ah, 0x7f
        cmp    eax, 1
        jne    loc_2_0B2D
        mov    eax, dword ptr [ebp - 4]
        test   byte ptr [eax + 0x20], 2
        je     loc_2_0B2D
        test   esi, esi
        je     loc_2_0BD6
        push   0
        lea    ecx, [ebp - 0x18]
        push   dword ptr [ebp - 4]
        push   ecx
        call   sub_2_1474
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_0B83
        cmp    dword ptr [ebp - 0x18], 0x24
        jae    loc_2_0B83
        xor    esi, esi
loc_2_0B83:
        test   esi, esi
        je     loc_2_0BD6
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0x38], esi
        add    ecx, 0x10
        push   0
        push   0x10
        mov    eax, dword ptr [ecx + 4]
        lea    edx, [esi + eax - 1]
        lea    eax, [ebp - 0x3c]
        push   eax
        mov    dword ptr [ebp - 0x34], edx
        mov    cx, word ptr [ecx + 0x10]
        push   1
        push   dword ptr [ebp - 0xc]
        mov    word ptr [ebp - 0x30], cx
        lea    ecx, [ebp - 0x1c]
        push   ecx
        call   sub_2_146C
        add    esp, 0x18
        test   eax, eax
        jne    loc_2_0D87
        movzx  ebx, bl
        add    bl, 0x10
        inc    dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 8], 4
        jb     loc_2_0ACA
loc_2_0BD6:
        mov    bl, 0x60
        mov    eax, dword ptr [ebp - 0x14]
        xor    esi, esi
        mov    dword ptr [ebp - 4], eax
loc_2_0BE0:
        mov    edx, 0x279
        mov    al, bl
        out    dx, al
        mov    dx, word ptr [0]              ; -> obj1:0x0058
        in     al, dx
        movzx  di, al
        shl    di, 8
        lea    eax, [ebx + 1]
        mov    edx, 0x279
        out    dx, al
        mov    dx, word ptr [0]              ; -> obj1:0x0058
        in     al, dx
        movzx  ax, al
        or     di, ax
        je     loc_2_0CAB
        push   0
        lea    ecx, [ebp - 4]
        push   0
        push   2
        push   dword ptr [ebp - 4]
        push   ecx
        call   sub_2_13FC
        add    esp, 0x14
        test   eax, eax
        jne    loc_2_0C4A
        push   0
        lea    ecx, [ebp - 0x18]
        push   dword ptr [ebp - 4]
        push   ecx
        call   sub_2_1474
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_0C4D
        cmp    dword ptr [ebp - 0x18], 0x18
        jae    loc_2_0C4D
loc_2_0C4A:
        xor    di, di
loc_2_0C4D:
        test   di, di
        je     loc_2_0CAB
        mov    word ptr [ebp - 0x24], di
        mov    eax, dword ptr [ebp - 4]
        mov    cx, word ptr [eax + 0xe]
        add    eax, 0xc
        dec    cx
        push   0
        add    di, cx
        push   0xc
        mov    word ptr [ebp - 0x22], di
        mov    dl, byte ptr [eax + 0xa]
        mov    byte ptr [ebp - 0x1e], dl
        lea    edx, [ebp - 0x28]
        mov    cl, byte ptr [eax + 0xb]
        push   edx
        push   2
        mov    byte ptr [ebp - 0x1d], cl
        mov    ax, word ptr [eax + 8]
        push   dword ptr [ebp - 0xc]
        mov    word ptr [ebp - 0x20], ax
        lea    eax, [ebp - 0x1c]
        push   eax
        call   sub_2_146C
        add    esp, 0x18
        test   eax, eax
        jne    loc_2_0D87
        add    bl, 2
        inc    esi
        cmp    esi, 8
        jb     loc_2_0BE0
loc_2_0CAB:
        mov    eax, dword ptr [ebp - 0x14]
        xor    esi, esi
        mov    byte ptr [ebp - 5], 0x70
        mov    dword ptr [ebp - 4], eax
loc_2_0CB7:
        mov    edx, 0x279
        mov    al, byte ptr [ebp - 5]
        out    dx, al
        mov    dx, word ptr [0]              ; -> obj1:0x0058
        in     al, dx
        mov    bl, al
        test   bl, bl
        je     loc_2_0D1C
        push   0
        lea    ecx, [ebp - 4]
        push   0
        push   4
        push   dword ptr [ebp - 4]
        push   ecx
        call   sub_2_13FC
        add    esp, 0x14
        test   eax, eax
        je     loc_2_0CE9
        xor    bl, bl
loc_2_0CE9:
        test   bl, bl
        je     loc_2_0D1C
        push   0
        lea    ecx, [ebp - 0x44]
        movzx  ax, bl
        mov    word ptr [ebp - 0x42], ax
        push   8
        push   ecx
        lea    eax, [ebp - 0x1c]
        push   4
        push   dword ptr [ebp - 0xc]
        push   eax
        call   sub_2_146C
        add    esp, 0x18
        test   eax, eax
        jne    loc_2_0D87
        add    byte ptr [ebp - 5], 2
        inc    esi
        cmp    esi, 2
        jb     loc_2_0CB7
loc_2_0D1C:
        mov    eax, dword ptr [ebp - 0x14]
        xor    esi, esi
        xor    edi, edi
        mov    dword ptr [ebp - 4], eax
        mov    byte ptr [ebp - 5], 0x74
loc_2_0D2A:
        mov    edx, 0x279
        mov    al, byte ptr [ebp - 5]
        out    dx, al
        mov    dx, word ptr [0]              ; -> obj1:0x0058
        in     al, dx
        cmp    al, 4
        mov    bl, al
        je     loc_2_0D87
        push   edi
        lea    ecx, [ebp - 4]
        push   edi
        push   3
        push   dword ptr [ebp - 4]
        push   ecx
        call   sub_2_13FC
        add    esp, 0x14
        test   eax, eax
        je     loc_2_0D5A
        mov    bl, 4
loc_2_0D5A:
        cmp    bl, 4
        je     loc_2_0D87
        push   edi
        lea    eax, [ebp - 0x10]
        push   4
        lea    edx, [ebp - 0x1c]
        push   eax
        mov    byte ptr [ebp - 0xf], bl
        push   3
        push   dword ptr [ebp - 0xc]
        push   edx
        call   sub_2_146C
        add    esp, 0x18
        test   eax, eax
        jne    loc_2_0D87
        inc    esi
        inc    byte ptr [ebp - 5]
        cmp    esi, 2
        jb     loc_2_0D2A
loc_2_0D87:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_0D8E:
        push   ebp
        mov    edx, 0x279
        mov    ebp, esp
        mov    al, 3
        out    dx, al
        sub    esp, 0xe0
        mov    edx, 0xa79
        mov    al, cl
        mov    byte ptr [ebp - 2], cl
        push   ebx
        push   esi
        push   edi
        xor    bl, bl
        out    dx, al
        call   sub_2_0917
        call   sub_2_0806
        mov    byte ptr [ebp - 1], al
        cmp    al, bl
        je     loc_2_0F16
        mov    ecx, dword ptr [0]            ; -> obj1:0x0054
loc_2_0DCA:
        mov    edx, 0x279
        mov    al, 7
        out    dx, al
        mov    edx, 0xa79
        mov    al, bl
        out    dx, al
        mov    dx, word ptr [0]              ; -> obj1:0x0058
        in     al, dx
        cmp    al, bl
        jne    loc_2_0F16
        cmp    byte ptr [ebp - 1], 1
        jbe    loc_2_0E0D
        movzx  eax, bl
        push   eax
        lea    ecx, [ebp - 0x18]
        push   0                             ; -> obj3:0x0098
        push   ecx
        call   sub_2_149C
        add    esp, 0xc
        mov    ecx, dword ptr [0]            ; -> obj1:0x0054
        jmp    loc_2_0E11
loc_2_0E0D:
        mov    byte ptr [ebp - 0x18], 0
loc_2_0E11:
        push   dword ptr [ecx + 4]
        movzx  eax, byte ptr [ecx + 3]
        mov    dl, byte ptr [ecx + 1]
        lea    esi, [ebp - 0x18]
        push   esi
        push   eax
        movzx  esi, byte ptr [ecx + 2]
        push   esi
        xor    eax, eax
        mov    al, dl
        shr    dl, 5
        and    eax, 0x1f
        add    eax, 0x40
        push   eax
        xor    eax, eax
        mov    al, byte ptr [ecx]
        and    eax, 3
        movzx  edx, dl
        lea    eax, [edx + eax*8 + 0x40]
        push   eax
        mov    al, byte ptr [ecx]
        shr    al, 2
        lea    ecx, [ebp - 0xe0]
        movzx  eax, al
        add    eax, 0x40
        push   eax
        push   0                             ; -> obj3:0x00A0
        push   ecx
        call   sub_2_149C
        add    esp, 0x24
        lea    edx, [ebp - 0xe0]
        lea    eax, [ebp - 8]
        push   0
        push   dword ptr [0]                 ; -> obj3:0x0000
        push   edx
        push   eax
        call   sub_2_1494
        add    esp, 0x10
        mov    esi, eax
        mov    ecx, dword ptr [0]            ; -> obj1:0x0054
        test   esi, esi
        je     loc_2_0E8E
        cmp    esi, 0x10
        jne    loc_2_0F0B
loc_2_0E8E:
        movzx  edi, bl
        movzx  eax, byte ptr [ebp - 1]
        movzx  ecx, byte ptr [ebp - 2]
        shl    eax, 8
        push   0
        add    eax, edi
        shl    eax, 8
        add    eax, ecx
        push   eax
        push   dword ptr [0]                 ; -> obj3:0x0000
        push   dword ptr [ebp - 8]
        call   sub_2_148C
        mov    dx, word ptr [0]              ; -> obj1:0x0058
        add    esp, 0x10
        mov    word ptr [ebp - 0xa], dx
        mov    cl, byte ptr [ebp - 2]
        push   0
        mov    byte ptr [ebp - 0xc], cl
        lea    ecx, [ebp - 0xc]
        mov    byte ptr [ebp - 0xb], bl
        push   ecx
        push   4
        push   0x10
        push   dword ptr [ebp - 8]
        call   sub_2_1484
        add    esp, 0x14
        mov    ecx, dword ptr [0]            ; -> obj1:0x0054
        cmp    esi, 0x10
        je     loc_2_0F0B
        push   0
        add    ecx, 9
        push   edi
        push   dword ptr [ebp - 8]
        push   ecx
        call   sub_2_1414
        add    esp, 0x10
        mov    ecx, dword ptr [ebp - 8]
        call   sub_2_0928
        mov    ecx, dword ptr [0]            ; -> obj1:0x0054
loc_2_0F0B:
        inc    bl
        cmp    byte ptr [ebp - 1], bl
        ja     loc_2_0DCA
loc_2_0F16:
        mov    edx, 0x279
        mov    al, 3
        out    dx, al
        mov    edx, 0xa79
        xor    al, al
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        out    dx, al
        pop    ebp
        ret    
sub_2_0F2D:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        mov    byte ptr [ebp - 2], cl
        push   esi
        xor    eax, eax
        push   edi
        mov    edx, 0x279
        mov    byte ptr [ebp - 1], 0x6a
        lea    edi, [ebp - 0xb]
        push   0
        mov    byte ptr [ebp - 0xc], 0
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        mov    al, 1
        push   0x2ee
        out    dx, al
        xor    bl, bl
        call   sub_2_142C
        mov    byte ptr [ebp - 3], 0
        add    esp, 8
loc_2_0F65:
        push   0
        push   0xfa
        call   sub_2_142C
        mov    dx, word ptr [0]              ; -> obj1:0x0058
        add    esp, 8
        in     al, dx
        cmp    al, 0x55
        mov    cl, al
        in     al, dx
        jne    loc_2_0F89
        mov    dl, 1
        cmp    al, 0xaa
        je     loc_2_0F8B
loc_2_0F89:
        xor    dl, dl
loc_2_0F8B:
        mov    al, bl
        mov    cl, bl
        shr    al, 3
        and    cl, 7
        movzx  esi, al
        mov    al, dl
        or     byte ptr [ebp - 3], dl
        shl    al, cl
        or     byte ptr [ebp + esi - 0xc], al
        cmp    bl, 0x40
        jae    loc_2_0FB3
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_07CA
        mov    byte ptr [ebp - 1], al
loc_2_0FB3:
        inc    bl
        cmp    bl, 0x48
        jb     loc_2_0F65
        cmp    byte ptr [ebp - 3], 0
        je     loc_2_0FD2
        mov    al, byte ptr [ebp - 1]
        cmp    byte ptr [ebp - 4], al
        je     loc_2_0FCE
        cmp    byte ptr [ebp - 4], 0
        jne    loc_2_0FD2
loc_2_0FCE:
        mov    bl, 1
        jmp    loc_2_0FD4
loc_2_0FD2:
        xor    bl, bl
loc_2_0FD4:
        push   0
        push   0xfa
        call   sub_2_142C
        add    esp, 8
        test   bl, bl
        je     loc_2_1028
        mov    edx, 0x279
        mov    al, 6
        out    dx, al
        mov    edx, 0xa79
        mov    al, byte ptr [ebp - 2]
        out    dx, al
        mov    dx, word ptr [0]              ; -> obj1:0x0058
        in     al, dx
        cmp    al, byte ptr [ebp - 2]
        je     loc_2_1009
        xor    bl, bl
        jmp    loc_2_1028
loc_2_1009:
        push   0
        push   0xfa
        call   sub_2_142C
        add    esp, 8
        mov    edx, 0x279
        mov    al, 3
        out    dx, al
        mov    edx, 0xa79
        xor    al, al
        out    dx, al
loc_2_1028:
        movzx  eax, bl
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_1032:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xfc
        mov    byte ptr [ebp - 5], 1
        push   ebx
        push   esi
        xor    esi, esi
        push   1
        cmp    dword ptr [0], esi            ; -> obj3:0x0004
        jne    loc_2_11A7
        mov    eax, dword ptr [0]            ; -> obj3:0x0000
        push   eax
        push   0                             ; -> obj3:0x00BC
        push   0                             ; -> obj3:0x0004
        call   sub_2_1494
        add    esp, 0x10
        test   eax, eax
        jne    loc_2_13BB
        mov    word ptr [ebp - 2], si
        lea    eax, [ebp - 0x14]
        push   eax
        push   1
        push   0                             ; -> obj3:0x00D4
        push   2
        call   sub_2_14E4
        add    esp, 0x10
        test   eax, eax
        jne    loc_2_10BD
        lea    eax, [ebp - 0x20]
        lea    ecx, [ebp - 2]
        push   eax
        lea    edx, [ebp - 0x24]
        mov    dword ptr [ebp - 0x20], 2
        push   ecx
        push   edx
        push   esi
        push   0                             ; -> obj3:0x00DC
        push   dword ptr [ebp - 0x14]
        call   sub_2_14DC
        add    esp, 0x18
        push   dword ptr [ebp - 0x14]
        call   sub_2_14D4
        add    esp, 4
loc_2_10BD:
        xor    ebx, ebx
        cmp    word ptr [ebp - 2], bx
        jne    loc_2_1115
        push   ebx
        mov    eax, dword ptr [0]            ; -> obj3:0x0004
        push   0x2000
        lea    ecx, [ebp - 0xc]
        push   eax
        push   ecx
        call   sub_2_147C
        add    esp, 0x10
        test   eax, eax
        jne    loc_2_13BB
loc_2_10E5:
        movzx  eax, bl
        imul   eax, eax, 0x18
        push   0
        add    eax, 0                        ; -> obj3:0x0020
        push   0x18
        push   eax
        push   2
        push   dword ptr [ebp - 0xc]
        push   0
        call   sub_2_146C
        add    esp, 0x18
        test   eax, eax
        jne    loc_2_13BB
        inc    bl
        cmp    bl, 4
        jb     loc_2_10E5
        jmp    loc_2_111F
loc_2_1115:
        mov    ax, word ptr [ebp - 2]
        mov    word ptr [0x10], ax           ; -> obj3:0x0090
loc_2_111F:
        xor    esi, esi
        mov    eax, dword ptr [0]            ; -> obj3:0x0004
        push   esi
        lea    ecx, [ebp - 0xc]
        push   0x3000
        push   eax
        push   ecx
        call   sub_2_147C
        add    esp, 0x10
        test   eax, eax
        jne    loc_2_13BB
        push   esi
        push   0x18
        push   0                             ; -> obj3:0x0080
        push   2
        push   dword ptr [ebp - 0xc]
        push   esi
        call   sub_2_146C
        add    esp, 0x18
        test   eax, eax
        jne    loc_2_13BB
        push   esi
        mov    eax, dword ptr [0]            ; -> obj3:0x0004
        push   0x5000
        lea    ecx, [ebp - 0xc]
        push   eax
        push   ecx
        call   sub_2_147C
        add    esp, 0x10
        test   eax, eax
        jne    loc_2_13BB
        push   esi
        mov    eax, dword ptr [0]            ; -> obj3:0x0004
        push   esi
        push   eax
        call   sub_2_13E4
        add    esp, 0xc
        mov    eax, dword ptr [0]            ; -> obj3:0x0004
        push   3
        push   esi
        push   0                             ; -> loc_2_0065
        push   eax
        call   sub_2_13DC
        add    esp, 0x10
        jmp    loc_2_11B5
loc_2_11A7:
        mov    eax, dword ptr [0]            ; -> obj3:0x0004
        push   eax
        call   sub_2_14CC
        add    esp, 8
loc_2_11B5:
        push   0
        mov    eax, dword ptr [0]            ; -> obj3:0x0004
        push   eax
        lea    ecx, [ebp - 0xfc]
        push   ecx
        call   sub_2_14C4
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_13BB
        cmp    word ptr [ebp - 0xa0], 0
        je     loc_2_13BB
        push   0
        mov    eax, dword ptr [0]            ; -> obj3:0x0000
        push   eax
        call   sub_2_14BC
        add    esp, 8
        mov    eax, dword ptr [0]            ; -> obj3:0x0004
        push   1
        push   0x200
        push   eax
        call   sub_2_140C
        add    esp, 0xc
        call   sub_2_07E1
        mov    edx, 0x279
        mov    al, 2
        out    dx, al
        mov    edx, 0xa79
        mov    al, 6
        push   0
        xor    bl, bl
        out    dx, al
        push   0x7d0
        call   sub_2_142C
        add    esp, 8
        call   sub_2_07E1
        cmp    word ptr [ebp - 0xa0], 0
        jbe    loc_2_12B8
        mov    esi, 0x279
loc_2_1241:
        movzx  eax, bl
        mov    ax, word ptr [ebp + eax*2 - 0x9e]
        mov    edx, esi
        add    ax, 3
        push   0
        mov    word ptr [0], ax              ; -> obj1:0x0058
        push   0x3e8
        mov    al, 3
        out    dx, al
        mov    edx, 0xa79
        xor    al, al
        out    dx, al
        call   sub_2_142C
        add    esp, 8
        mov    edx, esi
        xor    al, al
        mov    ecx, dword ptr [ebp - 5]
        out    dx, al
        mov    ax, word ptr [0]              ; -> obj1:0x0058
        mov    edx, 0xa79
        shr    ax, 2
        out    dx, al
        call   sub_2_0F2D
        test   eax, eax
        je     loc_2_12A2
loc_2_1293:
        inc    byte ptr [ebp - 5]
        mov    ecx, dword ptr [ebp - 5]
        call   sub_2_0F2D
        test   eax, eax
        jne    loc_2_1293
loc_2_12A2:
        cmp    byte ptr [ebp - 5], 1
        ja     loc_2_12B8
        inc    bl
        movzx  ecx, word ptr [ebp - 0xa0]
        movzx  eax, bl
        cmp    eax, ecx
        jb     loc_2_1241
loc_2_12B8:
        mov    bl, 1
        cmp    byte ptr [ebp - 5], bl
        jbe    loc_2_12CD
loc_2_12BF:
        mov    ecx, ebx
        inc    bl
        call   sub_2_0D8E
        cmp    bl, byte ptr [ebp - 5]
        jb     loc_2_12BF
loc_2_12CD:
        mov    edx, 0x279
        mov    al, 2
        out    dx, al
        mov    edx, 0xa79
        xor    esi, esi
        out    dx, al
        push   esi
        mov    eax, dword ptr [0]            ; -> obj3:0x0000
        push   eax
        call   sub_2_14B4
        add    esp, 8
        cmp    byte ptr [ebp - 5], 1
        jne    loc_2_12F9
        mov    word ptr [0], si              ; -> obj1:0x0058
loc_2_12F9:
        xor    esi, esi
        mov    eax, dword ptr [0]            ; -> obj3:0x0014
        push   esi
        lea    ecx, [ebp - 0x10]
        push   eax
        push   esi
        push   ecx
        call   sub_2_1494
        add    esp, 0x10
        test   eax, eax
        jne    loc_2_13BB
        push   2
        mov    eax, dword ptr [0]            ; -> obj3:0x0004
        push   eax
        lea    ecx, [ebp - 0x1c]
        push   ecx
        call   sub_2_1404
        add    esp, 0xc
        lea    edx, [ebp - 0x18]
        push   2
        push   0x3000
        push   dword ptr [ebp - 0x10]
        push   edx
        call   sub_2_147C
        add    esp, 0x10
        test   eax, eax
        jne    loc_2_13BB
        mov    eax, dword ptr [ebp - 0x1c]
        push   esi
        push   esi
        lea    ecx, [ebp - 8]
        push   2
        mov    dword ptr [ebp - 8], eax
        push   eax
        push   ecx
        call   sub_2_13FC
        add    esp, 0x14
        test   eax, eax
        jne    loc_2_13AE
loc_2_1360:
        mov    eax, dword ptr [ebp - 8]
        movzx  edx, word ptr [0]             ; -> obj1:0x0058
        movzx  ecx, word ptr [eax + 4]
        sub    ecx, edx
        cmp    ecx, -3
        je     loc_2_1397
        push   esi
        push   0xc
        push   eax
        push   2
        push   dword ptr [ebp - 0x18]
        push   esi
        call   sub_2_146C
        add    esp, 0x18
        lea    edx, [ebp - 8]
        push   esi
        push   dword ptr [ebp - 8]
        push   edx
        call   sub_2_14AC
        add    esp, 0xc
loc_2_1397:
        push   esi
        lea    ecx, [ebp - 8]
        push   esi
        push   2
        push   dword ptr [ebp - 8]
        push   ecx
        call   sub_2_13FC
        add    esp, 0x14
        test   eax, eax
        je     loc_2_1360
loc_2_13AE:
        push   0
        push   dword ptr [ebp - 0x10]
        call   sub_2_14A4
        add    esp, 8
loc_2_13BB:
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_13C1:
        cmp    dword ptr [esp + 8], 1
        mov    eax, 1
        jne    loc_2_13D6
        mov    ecx, dword ptr [esp + 4]
        call   0
loc_2_13D6:
        ret    8
        db  0CCh, 0CCh, 0CCh
sub_2_13DC:
        VxDCall  CONFIGMG._CONFIGMG_Register_Device_Driver ; cd200e803300
        int3   
        int3   
sub_2_13E4:
        VxDCall  CONFIGMG._CONFIGMG_Register_Enumerator ; cd200f803300
        int3   
        int3   
        VxDCall  CONFIGMG._CONFIGMG_Register_Enumerator_Function ; cd2053803300
        int3   
        int3   
        VxDCall  CONFIGMG._CONFIGMG_Locate_DevNode ; cd2002803300
        int3   
        int3   
sub_2_13FC:
        VxDCall  CONFIGMG._CONFIGMG_Get_Next_Res_Des ; cd201f803300
        int3   
        int3   
sub_2_1404:
        VxDCall  CONFIGMG._CONFIGMG_Get_First_Log_Conf ; cd201a803300
        int3   
        int3   
sub_2_140C:
        VxDCall  CONFIGMG.Service_805Bh          ; cd205b803300
        int3   
        int3   
sub_2_1414:
        VxDCall  CONFIGMG._CONFIGMG_ISAPNP_To_CM ; cd2038803300
        int3   
        int3   
sub_2_141C:
        VxDCall  CONFIGMG._CONFIGMG_Write_Registry_Value ; cd203f803300
        int3   
        int3   
sub_2_1424:
        VxDCall  CONFIGMG._CONFIGMG_Get_Device_ID ; cd2007803300
        int3   
        int3   
sub_2_142C:
        VxDCall  CONFIGMG._CONFIGMG_Yield        ; cd2015803300
        int3   
        int3   
sub_2_1434:
        VxDCall  CONFIGMG._CONFIGMG_Get_Private_DWord ; cd2009803300
        int3   
        int3   
sub_2_143C:
        VMMCall  VMM._PageFree                   ; cd2055800100
        int3   
        int3   
sub_2_1444:
        VMMCall  VMM._PageAllocate               ; cd2053800100
        int3   
        int3   
sub_2_144C:
        VMMCall  VMM._LinPageLock                ; cd2063800100
        int3   
        int3   
sub_2_1454:
        VMMCall  VMM.Service_8194h               ; cd2094810100
        int3   
        int3   
sub_2_145C:
        VMMCall  VMM._LinPageUnLock              ; cd2064800100
        int3   
        int3   
sub_2_1464:
        VMMCall  VMM.Service_8195h               ; cd2095810100
        int3   
        int3   
sub_2_146C:
        VxDCall  CONFIGMG._CONFIGMG_Add_Res_Des  ; cd201c803300
        int3   
        int3   
sub_2_1474:
        VxDCall  CONFIGMG._CONFIGMG_Get_Res_Des_Data_Size ; cd2021803300
        int3   
        int3   
sub_2_147C:
        VxDCall  CONFIGMG._CONFIGMG_Add_Empty_Log_Conf ; cd2018803300
        int3   
        int3   
sub_2_1484:
        VxDCall  CONFIGMG._CONFIGMG_Set_Bus_Info ; cd2043803300
        int3   
        int3   
sub_2_148C:
        VxDCall  CONFIGMG._CONFIGMG_Set_Private_DWord ; cd200a803300
        int3   
        int3   
sub_2_1494:
        VxDCall  CONFIGMG._CONFIGMG_Create_DevNode ; cd200b803300
        int3   
        int3   
sub_2_149C:
        VMMCall  VMM._Sprintf                    ; cd2074810100
        int3   
        int3   
sub_2_14A4:
        VxDCall  CONFIGMG._CONFIGMG_Remove_SubTree ; cd200d803300
        int3   
        int3   
sub_2_14AC:
        VxDCall  CONFIGMG._CONFIGMG_Free_Res_Des ; cd201e803300
        int3   
        int3   
sub_2_14B4:
        VxDCall  CONFIGMG._CONFIGMG_Remove_Unmarked_Children ; cd2037803300
        int3   
        int3   
sub_2_14BC:
        VxDCall  CONFIGMG._CONFIGMG_Reset_Children_Marks ; cd2035803300
        int3   
        int3   
sub_2_14C4:
        VxDCall  CONFIGMG._CONFIGMG_Get_Alloc_Log_Conf ; cd203b803300
        int3   
        int3   
sub_2_14CC:
        VxDCall  CONFIGMG.Service_805Ch          ; cd205c803300
        int3   
        int3   
sub_2_14D4:
        VMMCall  VMM._RegCloseKey                ; cd2049810100
        int3   
        int3   
sub_2_14DC:
        VMMCall  VMM._RegQueryValueEx            ; cd2051810100
        int3   
        int3   
sub_2_14E4:
        VMMCall  VMM._GetRegistryKey             ; cd206f810100
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 3  vsize=0xe8  RW-
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 048h, 061h, 072h, 064h, 077h, 061h, 072h, 065h
        db  049h, 044h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  001h, 000h, 00Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FCh, 0FFh, 004h, 000h
        db  074h, 002h, 0FFh, 002h, 000h, 000h, 000h, 000h, 001h, 000h, 00Ch, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 0FCh, 0FFh, 004h, 000h, 074h, 003h, 0FFh, 003h, 000h, 000h, 000h, 000h
        db  001h, 000h, 00Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FCh, 0FFh, 004h, 000h
        db  038h, 003h, 07Fh, 003h, 000h, 000h, 000h, 000h, 001h, 000h, 00Ch, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 0FCh, 0FFh, 004h, 000h, 038h, 002h, 07Fh, 002h, 000h, 000h, 000h, 000h
        db  001h, 000h, 00Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FCh, 0FFh, 004h, 000h
        db  000h, 002h, 0FFh, 003h, 000h, 000h, 000h, 000h, 05Fh, 044h, 045h, 056h, 025h, 034h, 058h, 000h
        db  049h, 053h, 041h, 050h, 06Eh, 050h, 05Ch, 025h, 063h, 025h, 063h, 025h, 063h, 025h, 032h, 058h
        db  025h, 032h, 058h, 025h, 073h, 05Ch, 025h, 038h, 058h, 000h, 000h, 000h, 049h, 053h, 041h, 050h
        db  06Eh, 050h, 05Ch, 052h, 065h, 061h, 064h, 044h, 061h, 074h, 061h, 050h, 06Fh, 072h, 074h, 05Ch
        db  030h, 000h, 000h, 000h, 049h, 053h, 041h, 050h, 04Eh, 050h, 000h, 000h, 052h, 044h, 050h, 04Fh
        db  076h, 065h, 072h, 052h, 069h, 064h, 065h, 000h
VxD__LDATA ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0x1d  R-X
sub_4_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        VxDCall  CONFIGMG._CONFIGMG_Get_Version  ; cd2000003300
        mov    word ptr [ebp - 2], ax
        cmp    word ptr [ebp - 2], 0x400
        sbb    eax, eax
        mov    esp, ebp
        inc    eax
        pop    ebp
        ret    
VxD__LTEXT ENDS

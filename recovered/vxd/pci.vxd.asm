; PCI -- reconstructed 32-bit disassembly (VxD / LE format)
; 4 objects, 11502 instructions recovered by recursive descent

Declare_Virtual_Device PCI, 4, 0, PCI_Control, 00043h, Undefined_Init_Order
;   DDK version 0400h   init_order 016800000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x74  R-X
        db  000h, 000h, 000h, 000h
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00043h                      ; DDB_Req_Device_Number
        db  4, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'PCI     '                  ; DDB_Name
        dd  016800000h                  ; DDB_Init_Order
        dd  PCI_Control              ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
PCI_Control:
        db  083h
        db  0F8h, 01Bh, 075h, 009h, 0E8h
        dd  loc_4_00C7               ; fixup
        db  083h, 0F8h, 001h, 0C3h, 083h, 0F8h, 022h, 075h, 009h, 052h, 053h, 0E8h
        dd  loc_2_7046               ; fixup
        db  0F9h, 0C3h, 0F8h, 0C3h, 0CCh, 0CCh
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x7b36  R-X
        db  0A1h
        dd  obj3:0x000C              ; fixup
        db  08Bh, 048h, 010h, 085h, 0C9h, 074h, 010h, 06Ah, 000h, 051h, 0E8h, 0ECh, 078h, 000h, 000h, 083h
        db  0C4h, 008h, 0A1h
        dd  obj3:0x000C              ; fixup
        db  08Bh, 040h, 014h, 085h, 0C0h, 074h, 00Bh, 06Ah, 000h, 050h, 0E8h, 0D5h, 078h, 000h, 000h, 083h
        db  0C4h, 008h, 0C3h
sub_2_002F:
        mov    eax, dword ptr [0]            ; -> obj3:0x0000
        cmp    eax, 1
        je     loc_2_0040
        cmp    eax, 2
        je     loc_2_004A
        jmp    loc_2_005A
loc_2_0040:
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [edx], eax
        mov    edx, dword ptr [ecx + 8]
        jmp    loc_2_0054
loc_2_004A:
        movzx  eax, word ptr [ecx + 4]
        mov    dword ptr [edx], eax
        movzx  edx, word ptr [ecx + 6]
loc_2_0054:
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx], edx
loc_2_005A:
        ret    4
sub_2_005D:
        mov    eax, dword ptr [0]            ; -> obj3:0x0000
        cmp    eax, 1
        je     loc_2_006E
        cmp    eax, 2
        je     loc_2_0073
        jmp    loc_2_0077
loc_2_006E:
        mov    eax, dword ptr [ecx + 0x14]
        jmp    loc_2_0077
loc_2_0073:
        movzx  eax, word ptr [ecx + 0xe]
loc_2_0077:
        ret    
sub_2_0078:
        mov    eax, dword ptr [0]            ; -> obj3:0x0000
        cmp    eax, 1
        je     loc_2_0089
        cmp    eax, 2
        je     loc_2_008E
        jmp    loc_2_0097
loc_2_0089:
        mov    eax, dword ptr [ecx + 0x10]
        jmp    loc_2_0097
loc_2_008E:
        movzx  eax, word ptr [ecx + 0xc]
        sub    eax, 0x10000
loc_2_0097:
        ret    
sub_2_0098:
        push   esi
        push   edi
        mov    edi, edx
        mov    esi, ecx
loc_2_009E:
        mov    eax, dword ptr [esi]
        test   eax, eax
        je     loc_2_00E5
        push   0
        push   0
        push   dword ptr [0]                 ; -> obj3:0x0000
        push   eax
        push   esi
        call   sub_2_7908
        add    esp, 0x14
        test   eax, eax
        jne    loc_2_00E5
        cmp    dword ptr [0], 1              ; -> obj3:0x0000
        jne    loc_2_00DE
        mov    eax, dword ptr [esi]
        mov    ax, word ptr [eax + 0xc]
        and    ax, 4
        cmp    ax, 1
        sbb    eax, eax
        inc    eax
        cmp    eax, dword ptr [0]            ; -> obj3:0x0004
        jne    loc_2_009E
loc_2_00DE:
        mov    eax, 1
        jmp    loc_2_00FA
loc_2_00E5:
        mov    eax, dword ptr [edi]
        mov    eax, dword ptr [eax]
        mov    dword ptr [edi], eax
        test   eax, eax
        je     loc_2_00F8
        mov    ecx, dword ptr [eax + 0xc]
        test   eax, eax
        mov    dword ptr [esi], ecx
        jne    loc_2_009E
loc_2_00F8:
        xor    eax, eax
loc_2_00FA:
        pop    edi
        pop    esi
        ret    
sub_2_00FD:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        push   edi
        mov    ebx, edx
        mov    eax, dword ptr [ecx]
        mov    edi, ecx
        xor    esi, esi
        cmp    dword ptr [ebx + 0xc], eax
        mov    dword ptr [ebp - 0xc], esi
        mov    dword ptr [ebp - 8], esi
        jne    loc_2_0122
        mov    dword ptr [ebp - 0x10], 0xffffffff
        jmp    loc_2_012C
loc_2_0122:
        mov    ecx, dword ptr [edi]
        call   sub_2_005D
        mov    dword ptr [ebp - 0x10], eax
loc_2_012C:
        test   ebx, ebx
        je     loc_2_01C8
loc_2_0134:
        mov    eax, dword ptr [ebx + 0xc]
        mov    dword ptr [ebp - 4], eax
loc_2_013A:
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_01BE
        push   0
        mov    eax, dword ptr [0]            ; -> obj3:0x0000
        push   0
        lea    edx, [ebp - 4]
        push   eax
        push   dword ptr [ebp - 4]
        push   edx
        call   sub_2_7908
        add    esp, 0x14
        test   eax, eax
        jne    loc_2_01BE
        cmp    dword ptr [0], 1              ; -> obj3:0x0000
        jne    loc_2_0180
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0xc]
        and    ax, 4
        cmp    ax, 1
        sbb    eax, eax
        inc    eax
        cmp    eax, dword ptr [0]            ; -> obj3:0x0004
        jne    loc_2_013A
loc_2_0180:
        mov    eax, dword ptr [edi]
        cmp    dword ptr [ebp - 4], eax
        jne    loc_2_0190
        mov    dword ptr [ebp - 8], 1
        jmp    loc_2_013A
loc_2_0190:
        mov    ecx, dword ptr [ebp - 4]
        call   sub_2_005D
        cmp    eax, dword ptr [ebp - 0x10]
        ja     loc_2_013A
        jae    loc_2_01AC
        cmp    dword ptr [ebp - 0xc], eax
        jae    loc_2_013A
        mov    esi, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0xc], eax
        jmp    loc_2_013A
loc_2_01AC:
        cmp    dword ptr [ebp - 8], 0
        je     loc_2_013A
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [edi], eax
        mov    eax, 1
        jmp    loc_2_01D0
loc_2_01BE:
        mov    ebx, dword ptr [ebx]
        test   ebx, ebx
        jne    loc_2_0134
loc_2_01C8:
        mov    dword ptr [edi], esi
        cmp    esi, 1
        sbb    eax, eax
        inc    eax
loc_2_01D0:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_01D7:
        mov    eax, dword ptr [0]            ; -> obj3:0x0000
        cmp    eax, 1
        je     loc_2_01E8
        cmp    eax, 2
        je     loc_2_01F4
        jmp    loc_2_0200
loc_2_01E8:
        mov    eax, dword ptr [esp + 4]
        mov    dword ptr [ecx + 4], edx
        mov    dword ptr [ecx + 8], eax
        jmp    loc_2_0200
loc_2_01F4:
        mov    word ptr [ecx + 4], dx
        mov    eax, dword ptr [esp + 4]
        mov    word ptr [ecx + 6], ax
loc_2_0200:
        ret    4
sub_2_0203:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   esi
        mov    esi, ecx
        test   esi, esi
        je     loc_2_02A0
loc_2_0214:
        push   2
        lea    ecx, [ebp - 4]
        push   dword ptr [esi + 8]
        push   ecx
        call   sub_2_7918
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_0296
loc_2_0229:
        push   0
        mov    eax, dword ptr [0]            ; -> obj3:0x0000
        push   0
        lea    edx, [ebp - 4]
        push   eax
        push   dword ptr [ebp - 4]
        push   edx
        call   sub_2_7908
        add    esp, 0x14
        test   eax, eax
        jne    loc_2_0296
        cmp    dword ptr [0], 1              ; -> obj3:0x0000
        jne    loc_2_0269
        mov    eax, dword ptr [ebp - 4]
        mov    ax, word ptr [eax + 0xc]
        and    ax, 4
        cmp    ax, 1
        sbb    eax, eax
        inc    eax
        cmp    eax, dword ptr [0]            ; -> obj3:0x0004
        jne    loc_2_0229
loc_2_0269:
        lea    eax, [ebp - 0xc]
        lea    edx, [ebp - 8]
        push   eax
        mov    ecx, dword ptr [ebp - 4]
        call   sub_2_002F
        push   0
        mov    eax, dword ptr [0]            ; -> obj3:0x000C
        push   dword ptr [eax + 0x14]
        push   dword ptr [ebp - 0xc]
        push   dword ptr [ebp - 8]
        call   sub_2_7910
        add    esp, 0x10
        test   eax, eax
        je     loc_2_0229
        jmp    loc_2_02A2
loc_2_0296:
        mov    esi, dword ptr [esi]
        test   esi, esi
        jne    loc_2_0214
loc_2_02A0:
        xor    eax, eax
loc_2_02A2:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_02A7:
        push   ebp
        mov    eax, dword ptr [0]            ; -> obj3:0x0018
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        mov    esi, ecx
        push   6
        push   eax
        lea    ecx, [ebp - 0x14]
        push   ecx
        call   sub_2_7918
        add    esp, 0xc
        mov    ecx, dword ptr [0]            ; -> obj3:0x000C
        test   eax, eax
        jne    loc_2_03DF
        or     byte ptr [ecx + 0x1c], 4
        push   0
        push   dword ptr [ecx + 0x14]
        push   -1
        push   0
        call   sub_2_7910
        add    esp, 0x10
        test   eax, eax
        jne    loc_2_0409
        mov    eax, dword ptr [ebp - 0x14]
        xor    bl, bl
        mov    dword ptr [ebp - 4], eax
loc_2_02F8:
        push   0
        lea    eax, [ebp - 0x10]
        push   eax
        lea    edx, [ebp - 4]
        push   0
        push   dword ptr [ebp - 4]
        push   edx
        call   sub_2_7908
        add    esp, 0x14
        test   eax, eax
        jne    loc_2_0353
        inc    bl
        cmp    bl, byte ptr [0]              ; -> obj3:0x0010
        jbe    loc_2_02F8
        mov    eax, dword ptr [0]            ; -> obj3:0x0000
        cmp    dword ptr [ebp - 0x10], eax
        je     loc_2_0367
        mov    ecx, dword ptr [ebp - 0x10]
        and    ch, 0x7f
        cmp    ecx, eax
        jne    loc_2_035D
        mov    eax, dword ptr [0]            ; -> obj3:0x000C
        xor    ecx, ecx
        push   1
        mov    dword ptr [eax + 4], ecx
        push   dword ptr [eax + 0x14]
        push   -1
        push   ecx
        mov    dword ptr [eax + 0xc], ecx
        call   sub_2_7928
        add    esp, 0x10
        jmp    loc_2_0409
loc_2_0353:
        mov    eax, 0x13
        jmp    loc_2_0409
loc_2_035D:
        mov    eax, 0x13
        jmp    loc_2_0409
loc_2_0367:
        lea    eax, [ebp - 8]
        lea    edx, [ebp - 0xc]
        push   eax
        mov    ecx, dword ptr [ebp - 4]
        call   sub_2_002F
        mov    eax, dword ptr [ebp - 8]
        mov    esi, dword ptr [0]            ; -> obj3:0x000C
        sub    eax, dword ptr [ebp - 0xc]
        inc    eax
        mov    dword ptr [esi + 4], eax
        mov    ecx, dword ptr [ebp - 4]
        call   sub_2_0078
        not    eax
        inc    eax
        mov    dword ptr [esi + 0xc], eax
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_2_03D8
        push   1
        mov    eax, dword ptr [0]            ; -> obj3:0x000C
        push   dword ptr [eax + 0x14]
        mov    eax, dword ptr [ebp - 0xc]
        dec    eax
        push   eax
        push   0
        call   sub_2_7928
        add    esp, 0x10
        test   eax, eax
        jne    loc_2_0409
        cmp    dword ptr [ebp - 8], -1
        je     loc_2_0409
        push   1
        mov    eax, dword ptr [0]            ; -> obj3:0x000C
        push   dword ptr [eax + 0x14]
        push   -1
        mov    eax, dword ptr [ebp - 8]
        inc    eax
        push   eax
        call   sub_2_7928
        add    esp, 0x10
        jmp    loc_2_0409
loc_2_03D8:
        mov    eax, 0x13
        jmp    loc_2_0409
loc_2_03DF:
        mov    eax, dword ptr [ecx]
        mov    edx, dword ptr [ecx + 8]
        push   0
        mov    dword ptr [ecx + 4], eax
        push   dword ptr [ecx + 0x14]
        push   dword ptr [ecx + 0x10]
        and    byte ptr [ecx + 0x1c], 0xfb
        mov    dword ptr [ecx + 0xc], edx
        call   sub_2_7920
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_0409
        mov    ecx, esi
        call   sub_2_0203
loc_2_0409:
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_040F:
        sub    esp, 0xcc
        lea    eax, [esp]
        push   0
        push   0xc9
        push   eax
        push   ecx
        call   sub_2_7930
        lea    eax, [esp + 0x10]
        add    esp, 0x10
        mov    ecx, 0                        ; -> obj3:0x001C
loc_2_0433:
        mov    dl, byte ptr [eax]
        cmp    dl, byte ptr [ecx]
        jne    loc_2_0453
        or     dl, dl
        je     loc_2_044F
        mov    dl, byte ptr [eax + 1]
        cmp    dl, byte ptr [ecx + 1]
        jne    loc_2_0453
        add    eax, 2
        add    ecx, 2
        or     dl, dl
        jne    loc_2_0433
loc_2_044F:
        xor    eax, eax
        jmp    loc_2_0458
loc_2_0453:
        sbb    eax, eax
        sbb    eax, -1
loc_2_0458:
        test   eax, eax
        mov    eax, 1
        je     loc_2_0463
        xor    eax, eax
loc_2_0463:
        add    esp, 0xcc
        ret    
sub_2_046A:
        xor    eax, eax
        mov    edx, dword ptr [0]            ; -> obj3:0x0000
        cmp    edx, 1
        je     loc_2_047E
        cmp    edx, 2
        je     loc_2_04A4
        jmp    loc_2_04D1
loc_2_047E:
        mov    edx, dword ptr [ecx + 4]
        mov    ecx, dword ptr [ecx + 8]
        cmp    edx, 0xa0000
        jne    loc_2_0494
        cmp    ecx, 0xaffff
        je     loc_2_04CC
loc_2_0494:
        cmp    edx, 0xb0000
        jne    loc_2_04D1
        cmp    ecx, 0xbffff
        jmp    loc_2_04CA
loc_2_04A4:
        movzx  edx, word ptr [ecx + 4]
        movzx  ecx, word ptr [ecx + 6]
        cmp    edx, 0x3b0
        jne    loc_2_04BC
        cmp    ecx, 0x3bb
        je     loc_2_04CC
loc_2_04BC:
        cmp    edx, 0x3c0
        jne    loc_2_04D1
        cmp    ecx, 0x3df
loc_2_04CA:
        jne    loc_2_04D1
loc_2_04CC:
        mov    eax, 1
loc_2_04D1:
        ret    
sub_2_04D2:
        push   esi
        xor    eax, eax
        cmp    dword ptr [0], 2              ; -> obj3:0x0000
        jne    loc_2_0525
        movzx  esi, word ptr [ecx + 4]
        movzx  edx, word ptr [ecx + 6]
        cmp    esi, 0x1f0
        jne    loc_2_04F6
        cmp    edx, 0x1f7
        je     loc_2_0520
loc_2_04F6:
        mov    ecx, 0x3f6
        cmp    esi, ecx
        jne    loc_2_0503
        cmp    edx, ecx
        je     loc_2_0520
loc_2_0503:
        cmp    esi, 0x170
        jne    loc_2_0513
        cmp    edx, 0x177
        je     loc_2_0520
loc_2_0513:
        mov    ecx, 0x376
        cmp    esi, ecx
        jne    loc_2_0525
        cmp    edx, ecx
        jne    loc_2_0525
loc_2_0520:
        mov    eax, 1
loc_2_0525:
        pop    esi
        ret    
sub_2_0527:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        call   sub_2_02A7
        test   eax, eax
        jne    loc_2_061D
        mov    eax, dword ptr [esi + 0xc]
        lea    ecx, [ebp - 4]
        mov    edx, esi
        mov    dword ptr [ebp - 4], eax
        call   sub_2_00FD
        test   eax, eax
        je     loc_2_061B
loc_2_0557:
        mov    ecx, dword ptr [ebp - 4]
        call   sub_2_046A
        test   eax, eax
        jne    loc_2_0609
        mov    ecx, dword ptr [ebp - 4]
        call   sub_2_04D2
        test   eax, eax
        jne    loc_2_0609
        mov    ecx, dword ptr [ebp - 4]
        call   sub_2_005D
        mov    edi, eax
        mov    ecx, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 8], 0xffffffff
        mov    ebx, 0x100
        call   sub_2_0078
        mov    ecx, dword ptr [0]            ; -> obj3:0x000C
        mov    dword ptr [ebp - 0xc], eax
loc_2_059E:
        push   0
        mov    eax, dword ptr [ebp - 8]
        push   dword ptr [ecx + 0x14]
        push   -1
        inc    eax
        lea    ecx, [ebp - 8]
        push   dword ptr [ebp - 0xc]
        push   edi
        push   eax
        push   ecx
        call   sub_2_7938
        add    esp, 0x1c
        mov    ecx, dword ptr [0]            ; -> obj3:0x000C
        test   eax, eax
        jne    loc_2_061D
        cmp    dword ptr [0], 2              ; -> obj3:0x0000
        jne    loc_2_05E3
        test   byte ptr [ecx + 0x1c], 2
        je     loc_2_05E3
        cmp    edi, ebx
        ja     loc_2_05E3
        mov    eax, dword ptr [ebp - 8]
        and    eax, 0x3ff
        cmp    eax, ebx
        jae    loc_2_059E
loc_2_05E3:
        mov    eax, dword ptr [ebp - 8]
        push   1
        push   dword ptr [ecx + 0x14]
        lea    edi, [eax + edi - 1]
        push   edi
        push   eax
        call   sub_2_7928
        add    esp, 0x10
        test   eax, eax
        jne    loc_2_061D
        push   edi
        mov    edx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        call   sub_2_01D7
loc_2_0609:
        lea    ecx, [ebp - 4]
        mov    edx, esi
        call   sub_2_00FD
        test   eax, eax
        jne    loc_2_0557
loc_2_061B:
        xor    eax, eax
loc_2_061D:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_0624:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   esi
        mov    dword ptr [ebp - 8], ecx
        mov    esi, edx
        call   sub_2_02A7
        test   eax, eax
        jne    loc_2_06BE
        mov    eax, dword ptr [ebp - 8]
        lea    edx, [ebp - 8]
        mov    ecx, dword ptr [eax + 0xc]
        mov    dword ptr [ebp - 4], ecx
        lea    ecx, [ebp - 4]
        call   sub_2_0098
        test   eax, eax
        je     loc_2_06BC
loc_2_0655:
        mov    ecx, dword ptr [ebp - 4]
        call   sub_2_046A
        test   eax, eax
        jne    loc_2_06AD
        mov    ecx, dword ptr [ebp - 4]
        call   sub_2_04D2
        test   eax, eax
        jne    loc_2_06AD
        lea    eax, [ebp - 0x10]
        lea    edx, [ebp - 0xc]
        push   eax
        mov    ecx, dword ptr [ebp - 4]
        call   sub_2_002F
        mov    ecx, dword ptr [0]            ; -> obj3:0x000C
        cmp    esi, 1
        sbb    eax, eax
        neg    eax
        push   eax
        push   dword ptr [ecx + 0x14]
        push   dword ptr [ebp - 0x10]
        push   dword ptr [ebp - 0xc]
        call   sub_2_7928
        add    esp, 0x10
        test   eax, eax
        jne    loc_2_06BE
        mov    edx, dword ptr [ebp - 0xc]
        mov    ecx, dword ptr [ebp - 4]
        push   dword ptr [ebp - 0x10]
        call   sub_2_01D7
loc_2_06AD:
        lea    edx, [ebp - 8]
        lea    ecx, [ebp - 4]
        call   sub_2_0098
        test   eax, eax
        jne    loc_2_0655
loc_2_06BC:
        xor    eax, eax
loc_2_06BE:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_06C3:
        mov    edx, dword ptr [0]            ; -> obj3:0x000C
        mov    eax, dword ptr [edx + 4]
        mov    ecx, dword ptr [edx + 0xc]
        push   0
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [edx + 0x14]
        mov    dword ptr [edx + 8], ecx
        mov    ecx, dword ptr [edx + 0x10]
        push   ecx
        mov    dword ptr [edx + 0x10], eax
        push   -1
        mov    dword ptr [edx + 0x14], ecx
        push   0
        call   sub_2_7910
        add    esp, 0x10
        xor    eax, eax
        ret    
sub_2_06F2:
        push   ebp
        mov    eax, dword ptr [0]            ; -> obj3:0x0008
        mov    ebp, esp
        sub    esp, 0xc
        test   byte ptr [eax + 3], 2
        push   esi
        je     loc_2_072D
        mov    esi, ecx
        push   0
        push   ecx
        lea    eax, [ebp - 4]
        push   eax
        call   sub_2_7948
        add    esp, 0xc
        lea    edx, [ebp - 4]
        push   0
        push   dword ptr [ebp - 4]
        push   edx
        call   sub_2_7948
        add    esp, 0xc
        mov    eax, dword ptr [0]            ; -> obj3:0x0008
        jmp    loc_2_0732
loc_2_072D:
        xor    esi, esi
        mov    dword ptr [ebp - 4], ecx
loc_2_0732:
        movzx  eax, byte ptr [eax + 1]
        cmp    eax, 0xfd
        je     loc_2_0746
        cmp    eax, 0xfe
        je     loc_2_077A
        jmp    loc_2_07A1
loc_2_0746:
        xor    eax, eax
        lea    ecx, [ebp - 8]
        push   eax
        mov    dword ptr [ebp - 0xc], eax
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        push   ecx
        push   dword ptr [ebp - 4]
        call   sub_2_7940
        add    esp, 0x14
        test   eax, eax
        jne    loc_2_07A1
        mov    eax, dword ptr [ebp - 8]
        sub    eax, 4
        cmp    eax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 0x13
        jmp    loc_2_07A6
loc_2_077A:
        mov    ecx, dword ptr [ebp - 4]
        call   sub_2_040F
        test   eax, eax
        jne    loc_2_07A1
        mov    ecx, esi
        call   sub_2_046A
        test   eax, eax
        jne    loc_2_07A1
        mov    ecx, esi
        call   sub_2_04D2
        test   eax, eax
        mov    eax, 0
        je     loc_2_07A6
loc_2_07A1:
        mov    eax, 0x13
loc_2_07A6:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_07AB:
        push   ebp
        mov    dword ptr [0], edx            ; -> obj3:0x0008
        mov    ebp, esp
        test   edx, edx
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [0], eax            ; -> obj3:0x0018
        je     loc_2_0878
        movzx  eax, byte ptr [0]             ; -> obj3:0x0010
        imul   eax, eax, 0x2c
        lea    edx, [eax + edx + 4]
        mov    dword ptr [0], edx            ; -> obj3:0x000C
        mov    eax, dword ptr [edx + 0x20]
        mov    dword ptr [0], eax            ; -> obj3:0x0000
        mov    eax, dword ptr [edx + 0x1c]
        and    eax, 1
        cmp    dword ptr [edx + 0x10], 0
        mov    dword ptr [0], eax            ; -> obj3:0x0004
        jne    loc_2_07F8
        xor    eax, eax
        jmp    loc_2_087D
loc_2_07F8:
        mov    dword ptr [0], edx            ; -> obj3:0x000C
        cmp    ecx, 7
        ja     loc_2_080A
        jmp    dword ptr [ecx*4]             ; -> obj2:0x0881
loc_2_080A:
        mov    eax, 1
        jmp    loc_2_087D
loc_2_0811:
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [eax]
        call   sub_2_0527
        jmp    loc_2_087D
loc_2_081D:
        xor    edx, edx
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [eax]
        call   sub_2_0624
        jmp    loc_2_087D
loc_2_082B:
        call   sub_2_06C3
        jmp    loc_2_087D
loc_2_0832:
        push   0
        mov    dword ptr [0], edx            ; -> obj3:0x000C
        push   dword ptr [edx + 0x14]
        push   -1
        push   0
        call   sub_2_7910
        add    esp, 0x10
        xor    eax, eax
        jmp    loc_2_087D
loc_2_084D:
        call   0
        mov    eax, dword ptr [0]            ; -> obj3:0x0008
        dec    byte ptr [eax]
        xor    eax, eax
        jmp    loc_2_087D
loc_2_085D:
        mov    edx, 1
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [eax]
        call   sub_2_0624
        jmp    loc_2_087D
loc_2_086E:
        mov    ecx, dword ptr [ebp + 0xc]
        call   sub_2_06F2
        jmp    loc_2_087D
loc_2_0878:
        mov    eax, 0x13
loc_2_087D:
        pop    ebp
        ret    8
        ; ---- dispatch table (8 entries) ----
        dd  loc_2_0811
        dd  loc_2_081D
        dd  loc_2_082B
        dd  loc_2_0832
        dd  loc_2_0878
        dd  loc_2_084D
        dd  loc_2_085D
        dd  loc_2_086E
loc_2_08A1:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        mov    dword ptr [0], eax            ; -> obj3:0x0008
        sub    esp, 4
        mov    al, byte ptr [eax]
        mov    byte ptr [ebp - 1], al
        push   ebx
        push   esi
        xor    bl, bl
        mov    esi, 1
        cmp    al, bl
        je     loc_2_08FD
loc_2_08C2:
        test   esi, esi
        je     loc_2_08CB
        cmp    esi, 1
        jne    loc_2_08FD
loc_2_08CB:
        movzx  ecx, bl
        mov    eax, dword ptr [0]            ; -> obj3:0x0008
        imul   ecx, ecx, 0x2c
        cmp    dword ptr [ecx + eax + 0x24], 2
        jne    loc_2_08F6
        mov    edx, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        mov    byte ptr [0], bl              ; -> obj3:0x0010
        call   sub_2_07AB
        mov    esi, eax
loc_2_08F6:
        inc    bl
        cmp    byte ptr [ebp - 1], bl
        ja     loc_2_08C2
loc_2_08FD:
        mov    eax, dword ptr [0]            ; -> obj3:0x0008
        cmp    byte ptr [eax], 0
        jne    loc_2_0912
        push   0
        push   eax
        call   sub_2_7950
        add    esp, 8
loc_2_0912:
        mov    eax, esi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_091A:
        push   ebp
        mov    eax, dword ptr [esp + 0xc]
        mov    ebp, esp
        mov    dword ptr [0], eax            ; -> obj3:0x0008
        sub    esp, 4
        mov    al, byte ptr [eax]
        mov    byte ptr [ebp - 1], al
        push   ebx
        push   esi
        xor    bl, bl
        mov    esi, 1
        cmp    al, bl
        je     loc_2_0976
loc_2_093B:
        test   esi, esi
        je     loc_2_0944
        cmp    esi, 1
        jne    loc_2_0976
loc_2_0944:
        movzx  ecx, bl
        mov    eax, dword ptr [0]            ; -> obj3:0x0008
        imul   ecx, ecx, 0x2c
        cmp    dword ptr [ecx + eax + 0x24], 1
        jne    loc_2_096F
        mov    edx, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        mov    byte ptr [0], bl              ; -> obj3:0x0010
        call   sub_2_07AB
        mov    esi, eax
loc_2_096F:
        inc    bl
        cmp    byte ptr [ebp - 1], bl
        ja     loc_2_093B
loc_2_0976:
        mov    eax, dword ptr [0]            ; -> obj3:0x0008
        cmp    byte ptr [eax], 0
        jne    loc_2_098B
        push   0
        push   eax
        call   sub_2_7950
        add    esp, 8
loc_2_098B:
        mov    eax, esi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_0993:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    eax, [ebp - 4]
        mov    esi, ecx
        push   0
        push   esi
        push   eax
        call   sub_2_7958
        add    esp, 0xc
        test   eax, eax
        je     loc_2_09B3
        xor    eax, eax
        jmp    loc_2_09D9
loc_2_09B3:
        mov    eax, dword ptr [0]            ; -> obj3:0x0DAC
        cmp    eax, dword ptr [ebp - 4]
        je     loc_2_09D7
        mov    esi, dword ptr [ebp - 4]
        push   0
        push   esi
        lea    eax, [ebp - 4]
        push   eax
        call   sub_2_7958
        add    esp, 0xc
        test   eax, eax
        je     loc_2_09B3
        xor    eax, eax
        jmp    loc_2_09D9
loc_2_09D7:
        mov    eax, esi
loc_2_09D9:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_09DE:
        push   ebp
        mov    eax, dword ptr [0]            ; -> obj3:0x0DAC
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        mov    byte ptr [ebp - 2], cl
        mov    bl, 0xff
        push   0
        push   eax
        lea    ecx, [ebp - 8]
        push   ecx
        call   sub_2_7968
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_0A3D
loc_2_0A02:
        push   0
        lea    eax, [ebp - 3]
        push   eax
        lea    edx, [ebp - 1]
        push   0
        mov    ecx, dword ptr [ebp - 8]
        call   sub_2_4212
        mov    al, byte ptr [ebp - 2]
        cmp    al, byte ptr [ebp - 1]
        jae    loc_2_0A28
        dec    byte ptr [ebp - 1]
        cmp    bl, byte ptr [ebp - 1]
        jbe    loc_2_0A28
        mov    bl, byte ptr [ebp - 1]
loc_2_0A28:
        push   0
        lea    ecx, [ebp - 8]
        push   dword ptr [ebp - 8]
        push   ecx
        call   sub_2_7960
        add    esp, 0xc
        test   eax, eax
        je     loc_2_0A02
loc_2_0A3D:
        mov    al, bl
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_0A44:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        lea    eax, [ebp - 1]
        push   esi
        lea    edx, [ebp - 3]
        mov    esi, ecx
        push   0
        push   eax
        xor    bl, bl
        push   edx
        lea    edx, [ebp - 2]
        call   sub_2_4212
        test   byte ptr [ebp - 1], 4
        je     loc_2_0A88
        mov    bl, 1
        push   0
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        call   sub_2_7968
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_0A88
        mov    ecx, dword ptr [ebp - 8]
        call   sub_2_0A44
        lea    ebx, [eax + 1]
loc_2_0A88:
        push   ebx
        mov    edx, dword ptr [ebp - 2]
        and    byte ptr [ebp - 1], 0xf7
        push   dword ptr [ebp - 1]
        push   dword ptr [ebp - 3]
        mov    ecx, esi
        call   sub_2_426A
        push   0
        lea    eax, [ebp - 8]
        push   esi
        push   eax
        call   sub_2_7960
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_0ABA
        mov    ecx, dword ptr [ebp - 8]
        call   sub_2_0A44
        add    bl, al
loc_2_0ABA:
        mov    al, bl
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_0AC2:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        call   sub_2_0993
        mov    dword ptr [ebp - 0xc], eax
        test   eax, eax
        jne    loc_2_0ADD
        xor    eax, eax
        jmp    loc_2_0B6E
loc_2_0ADD:
        xor    esi, esi
        lea    edx, [ebp - 1]
        push   esi
        mov    ecx, dword ptr [ebp - 0xc]
        push   esi
        push   esi
        call   sub_2_4212
        mov    bl, byte ptr [ebp - 1]
        push   esi
        mov    eax, dword ptr [0]            ; -> obj3:0x0DAC
        lea    ecx, [ebp - 0xc]
        push   eax
        push   ecx
        call   sub_2_7968
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_0B61
loc_2_0B07:
        lea    eax, [ebp - 3]
        lea    ecx, [ebp - 2]
        push   eax
        lea    edx, [ebp - 5]
        push   ecx
        push   edx
        lea    edx, [ebp - 4]
        mov    ecx, dword ptr [ebp - 0xc]
        call   sub_2_4212
        mov    cl, byte ptr [ebp - 4]
        cmp    byte ptr [ebp - 1], cl
        jne    loc_2_0B4C
        test   byte ptr [ebp - 2], 4
        je     loc_2_0B4C
        test   byte ptr [ebp - 2], 8
        jne    loc_2_0B74
        mov    al, byte ptr [ebp - 2]
        mov    edx, dword ptr [ebp - 4]
        or     al, 8
        mov    ecx, dword ptr [ebp - 0xc]
        push   dword ptr [ebp - 3]
        push   eax
        push   dword ptr [ebp - 5]
        call   sub_2_426A
        add    bl, byte ptr [ebp - 3]
loc_2_0B4C:
        push   0
        lea    ecx, [ebp - 0xc]
        push   dword ptr [ebp - 0xc]
        push   ecx
        call   sub_2_7960
        add    esp, 0xc
        test   eax, eax
        je     loc_2_0B07
loc_2_0B61:
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_09DE
        cmp    al, bl
        sbb    eax, eax
        inc    eax
loc_2_0B6E:
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_0B74:
        mov    eax, 1
        jmp    loc_2_0B6E
sub_2_0B7B:
        push   esi
        test   cl, cl
        je     loc_2_0B9B
        mov    esi, dword ptr [0]            ; -> loc_3_0368
        mov    eax, dword ptr [0]            ; -> loc_3_036C
        test   eax, eax
        je     loc_2_0B9B
loc_2_0B8F:
        cmp    byte ptr [esi], cl
        jne    loc_2_0B95
        mov    byte ptr [esi], dl
loc_2_0B95:
        add    esi, 0x10
        dec    eax
        jne    loc_2_0B8F
loc_2_0B9B:
        pop    esi
        ret    
sub_2_0B9D:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        test   edx, edx
        push   esi
        mov    esi, ecx
        jne    loc_2_0BC7
        push   0
        lea    eax, [ebp - 8]
        push   esi
        push   eax
        call   sub_2_7960
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_0BC7
        xor    edx, edx
        mov    ecx, dword ptr [ebp - 8]
        call   sub_2_0B9D
loc_2_0BC7:
        push   0
        lea    eax, [ebp - 9]
        push   eax
        lea    ecx, [ebp - 1]
        push   ecx
        lea    edx, [ebp - 5]
        mov    ecx, esi
        call   sub_2_4212
        test   byte ptr [ebp - 9], 4
        je     loc_2_0C09
        push   0
        lea    eax, [ebp - 0xc]
        push   esi
        push   eax
        call   sub_2_7968
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_0BFE
        xor    edx, edx
        mov    ecx, dword ptr [ebp - 0xc]
        call   sub_2_0B9D
loc_2_0BFE:
        mov    edx, dword ptr [ebp - 1]
        mov    ecx, dword ptr [ebp - 5]
        call   sub_2_5868
loc_2_0C09:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_0C0E:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   ebx
        push   esi
        push   edi
        mov    bl, dl
        mov    edi, ecx
        xor    esi, esi
loc_2_0C1D:
        lea    eax, [ebp - 3]
        lea    ecx, [ebp - 2]
        push   eax
        lea    edx, [ebp - 1]
        push   ecx
        push   edx
        lea    edx, [ebp - 4]
        mov    ecx, edi
        call   sub_2_4212
        test   byte ptr [ebp - 2], 0x11
        je     loc_2_0D5C
        cmp    dword ptr [ebp + 0xc], esi
        je     loc_2_0C4B
        cmp    bl, byte ptr [ebp - 4]
        jne    loc_2_0CFF
loc_2_0C4B:
        test   byte ptr [ebp - 2], 0x10
        jne    loc_2_0CFF
        mov    al, byte ptr [ebp - 2]
        mov    edx, ebx
        and    al, 0xf7
        mov    ecx, edi
        push   dword ptr [ebp - 3]
        push   eax
        push   dword ptr [ebp - 1]
        call   sub_2_426A
        mov    word ptr [ebp - 0xa], si
        mov    cl, byte ptr [ebp - 1]
        push   esi
        lea    edx, [ebp - 0xc]
        push   edx
        mov    byte ptr [ebp - 0xc], bl
        push   4
        mov    byte ptr [ebp - 0xb], cl
        push   4
        push   edi
        call   sub_2_7978
        add    esp, 0x14
        test   byte ptr [ebp - 2], 4
        je     loc_2_0CFF
        push   ebx
        mov    edx, dword ptr [ebp - 1]
        push   0x18
        mov    ecx, ebx
        call   sub_2_58EF
        mov    edx, dword ptr [ebp - 1]
        mov    ecx, ebx
        push   dword ptr [ebp + 8]
        push   0x19
        call   sub_2_58EF
        mov    al, byte ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 1]
        add    al, byte ptr [ebp - 3]
        mov    ecx, ebx
        dec    al
        push   eax
        push   0x1a
        call   sub_2_58EF
        push   2
        lea    ecx, [ebp - 8]
        push   edi
        push   ecx
        call   sub_2_7918
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_0CFF
        push   esi
        lea    ecx, [ebp - 8]
        push   esi
        push   0x43
        push   dword ptr [ebp - 8]
        push   ecx
        call   sub_2_7908
        add    esp, 0x14
        test   eax, eax
        jne    loc_2_0CFF
        mov    ecx, dword ptr [ebp - 8]
        mov    edx, dword ptr [ebp + 8]
        mov    cl, byte ptr [ecx]
        call   sub_2_0B7B
        mov    ecx, dword ptr [ebp - 8]
        mov    dl, byte ptr [ebp + 8]
        mov    byte ptr [ecx], dl
loc_2_0CFF:
        push   esi
        lea    eax, [ebp - 0x14]
        push   edi
        push   eax
        call   sub_2_7968
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_0D3C
        push   esi
        lea    eax, [ebp - 0x18]
        push   edi
        push   esi
        push   eax
        call   sub_2_7970
        add    esp, 0x10
        test   eax, eax
        jne    loc_2_0D3C
        test   byte ptr [ebp - 0x16], 1
        jne    loc_2_0D3C
        push   esi
        mov    al, byte ptr [ebp + 8]
        inc    al
        mov    edx, dword ptr [ebp + 8]
        push   eax
        mov    ecx, dword ptr [ebp - 0x14]
        call   sub_2_0C0E
loc_2_0D3C:
        push   esi
        lea    eax, [ebp - 0x10]
        push   edi
        push   eax
        call   sub_2_7960
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_0D5C
        mov    al, byte ptr [ebp - 3]
        mov    edi, dword ptr [ebp - 0x10]
        add    byte ptr [ebp + 8], al
        jmp    loc_2_0C1D
loc_2_0D5C:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
sub_2_0D65:
        push   ebp
        mov    eax, dword ptr [0]            ; -> obj3:0x0DAC
        mov    ebp, esp
        sub    esp, 8
        push   esi
        push   0
        mov    esi, ecx
        push   eax
        lea    ecx, [ebp - 8]
        push   ecx
        call   sub_2_7968
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_0E55
        mov    ecx, dword ptr [ebp - 8]
        call   sub_2_0A44
        test   esi, esi
        je     loc_2_0DAC
loc_2_0D96:
        mov    ecx, dword ptr [esi + 8]
        call   sub_2_0AC2
        test   eax, eax
        je     loc_2_0E5C
        mov    esi, dword ptr [esi]
        test   esi, esi
        jne    loc_2_0D96
loc_2_0DAC:
        push   0
        mov    eax, dword ptr [0]            ; -> obj3:0x0DAC
        push   eax
        lea    ecx, [ebp - 8]
        push   ecx
        call   sub_2_7968
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_0DFD
        xor    esi, esi
loc_2_0DC6:
        push   esi
        lea    eax, [ebp - 1]
        push   eax
        xor    edx, edx
        push   esi
        mov    ecx, dword ptr [ebp - 8]
        call   sub_2_4212
        test   byte ptr [ebp - 1], 8
        je     loc_2_0DE9
        mov    edx, 1
        mov    ecx, dword ptr [ebp - 8]
        call   sub_2_0B9D
loc_2_0DE9:
        push   esi
        lea    ecx, [ebp - 8]
        push   dword ptr [ebp - 8]
        push   ecx
        call   sub_2_7960
        add    esp, 0xc
        test   eax, eax
        je     loc_2_0DC6
loc_2_0DFD:
        push   0
        mov    eax, dword ptr [0]            ; -> obj3:0x0DAC
        push   eax
        lea    ecx, [ebp - 8]
        push   ecx
        call   sub_2_7968
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_0E55
        xor    esi, esi
loc_2_0E17:
        push   esi
        lea    eax, [ebp - 1]
        push   eax
        lea    edx, [ebp - 2]
        push   esi
        mov    ecx, dword ptr [ebp - 8]
        call   sub_2_4212
        test   byte ptr [ebp - 1], 8
        je     loc_2_0E41
        push   1
        mov    al, byte ptr [ebp - 2]
        inc    al
        mov    edx, dword ptr [ebp - 2]
        push   eax
        mov    ecx, dword ptr [ebp - 8]
        call   sub_2_0C0E
loc_2_0E41:
        push   esi
        lea    ecx, [ebp - 8]
        push   dword ptr [ebp - 8]
        push   ecx
        call   sub_2_7960
        add    esp, 0xc
        test   eax, eax
        je     loc_2_0E17
loc_2_0E55:
        xor    eax, eax
loc_2_0E57:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_0E5C:
        mov    eax, 0x13
        jmp    loc_2_0E57
sub_2_0E63:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, edx
        mov    edi, ecx
        xor    ebx, ebx
        push   ebx
        lea    eax, [ebp - 1]
        push   eax
        xor    edx, edx
        push   ebx
        call   sub_2_4212
        test   byte ptr [ebp - 1], 4
        je     loc_2_0EC1
        push   2
        lea    eax, [ebp - 8]
        push   edi
        push   eax
        call   sub_2_7918
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_0EC1
        push   ebx
        lea    ecx, [ebp - 8]
        push   ebx
        push   0x43
        push   dword ptr [ebp - 8]
        push   ecx
        call   sub_2_7908
        add    esp, 0x14
        test   eax, eax
        jne    loc_2_0EC1
        mov    eax, dword ptr [ebp - 8]
        mov    al, byte ptr [eax]
        test   al, al
        je     loc_2_0EC1
        mov    byte ptr [esi], al
        mov    eax, 1
        jmp    loc_2_0EC3
loc_2_0EC1:
        xor    eax, eax
loc_2_0EC3:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_0ECA:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        lea    edx, [ebp - 1]
        push   esi
        xor    bl, bl
        mov    esi, ecx
        call   sub_2_0E63
        test   eax, eax
        je     loc_2_0F1E
        mov    bl, byte ptr [ebp - 1]
        push   0
        push   esi
        lea    eax, [ebp - 8]
        push   eax
        call   sub_2_7968
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_0F1E
loc_2_0EF8:
        mov    ecx, dword ptr [ebp - 8]
        call   sub_2_0ECA
        mov    byte ptr [ebp - 1], al
        cmp    al, bl
        jbe    loc_2_0F09
        mov    bl, al
loc_2_0F09:
        push   0
        lea    ecx, [ebp - 8]
        push   dword ptr [ebp - 8]
        push   ecx
        call   sub_2_7960
        add    esp, 0xc
        test   eax, eax
        je     loc_2_0EF8
loc_2_0F1E:
        mov    al, bl
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_0F26:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        push   esi
        mov    bl, dl
        mov    esi, ecx
        push   eax
        lea    edx, [ebp - 1]
        push   eax
        push   eax
        call   sub_2_4212
        cmp    byte ptr [ebp - 1], bl
        jae    loc_2_108A
        push   0
        lea    eax, [ebp - 0x10]
        push   esi
        push   eax
        call   sub_2_7958
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_108A
        push   0
        lea    ecx, [ebp - 8]
        push   dword ptr [ebp - 0x10]
        push   ecx
        call   sub_2_7968
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_108A
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_09DE
        cmp    al, bl
        jb     loc_2_108A
        mov    byte ptr [ebp - 2], 0
loc_2_0F8C:
        cmp    dword ptr [ebp - 8], esi
        je     loc_2_0FB5
        lea    edx, [ebp - 1]
        mov    ecx, dword ptr [ebp - 8]
        call   sub_2_0E63
        test   eax, eax
        je     loc_2_0FB5
        cmp    byte ptr [ebp - 1], bl
        ja     loc_2_0FB5
        mov    ecx, dword ptr [ebp - 8]
        call   sub_2_0ECA
        cmp    al, byte ptr [ebp - 2]
        jbe    loc_2_0FB5
        mov    byte ptr [ebp - 2], al
loc_2_0FB5:
        push   0
        lea    ecx, [ebp - 8]
        push   dword ptr [ebp - 8]
        push   ecx
        call   sub_2_7960
        add    esp, 0xc
        test   eax, eax
        je     loc_2_0F8C
        cmp    byte ptr [ebp - 2], bl
        jae    loc_2_108A
        push   0
        mov    dword ptr [ebp - 0xc], esi
        push   esi
        lea    esi, [ebp - 0xc]
        push   esi
        call   sub_2_7958
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_1083
        xor    esi, esi
loc_2_0FEF:
        mov    eax, dword ptr [0]            ; -> obj3:0x0DAC
        cmp    dword ptr [ebp - 0xc], eax
        je     loc_2_1083
        push   esi
        lea    ecx, [ebp - 0x10]
        push   dword ptr [ebp - 0xc]
        push   ecx
        call   sub_2_7958
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_108A
        push   esi
        lea    ecx, [ebp - 8]
        push   dword ptr [ebp - 0x10]
        push   ecx
        call   sub_2_7968
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_108A
        lea    edx, [ebp - 2]
        mov    ecx, dword ptr [ebp - 0xc]
        call   sub_2_0E63
        test   eax, eax
        je     loc_2_108A
loc_2_1034:
        mov    eax, dword ptr [ebp - 8]
        cmp    dword ptr [ebp - 0xc], eax
        je     loc_2_1056
        lea    edx, [ebp - 1]
        mov    ecx, eax
        call   sub_2_0E63
        test   eax, eax
        je     loc_2_1056
        mov    al, byte ptr [ebp - 1]
        cmp    byte ptr [ebp - 2], al
        ja     loc_2_1056
        cmp    al, bl
        jbe    loc_2_108A
loc_2_1056:
        push   0
        lea    ecx, [ebp - 8]
        push   dword ptr [ebp - 8]
        push   ecx
        call   sub_2_7960
        add    esp, 0xc
        test   eax, eax
        je     loc_2_1034
        push   esi
        lea    ecx, [ebp - 0xc]
        push   dword ptr [ebp - 0xc]
        push   ecx
        call   sub_2_7958
        add    esp, 0xc
        test   eax, eax
        je     loc_2_0FEF
loc_2_1083:
        mov    eax, 1
        jmp    loc_2_108C
loc_2_108A:
        xor    eax, eax
loc_2_108C:
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_1092:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   edi
        mov    edi, ecx
        mov    esi, ecx
        test   esi, esi
        je     loc_2_10DA
loc_2_10A2:
        mov    eax, dword ptr [edi + 0xc]
        mov    dword ptr [ebp - 4], eax
        test   eax, eax
        je     loc_2_10DA
        push   0
        lea    ecx, [ebp - 4]
        push   0
        push   0x43
        push   eax
        push   ecx
        call   sub_2_7908
        add    esp, 0x14
        test   eax, eax
        jne    loc_2_10D4
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [edi + 8]
        mov    dl, byte ptr [eax]
        call   sub_2_0F26
        test   eax, eax
        je     loc_2_10DA
loc_2_10D4:
        mov    edi, dword ptr [edi]
        test   edi, edi
        jne    loc_2_10A2
loc_2_10DA:
        test   edi, edi
        jne    loc_2_10E2
        xor    eax, eax
        jmp    loc_2_1138
loc_2_10E2:
        mov    ecx, esi
        call   sub_2_0D65
        test   eax, eax
        jne    loc_2_1133
        test   esi, esi
        je     loc_2_112F
loc_2_10F1:
        push   2
        lea    ecx, [ebp - 4]
        push   dword ptr [esi + 8]
        push   ecx
        call   sub_2_7918
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_1129
        push   0
        lea    ecx, [ebp - 4]
        push   0
        push   0x43
        push   dword ptr [ebp - 4]
        push   ecx
        call   sub_2_7908
        add    esp, 0x14
        test   eax, eax
        jne    loc_2_1129
        mov    eax, dword ptr [ebp - 4]
        mov    edx, dword ptr [esi + 0xc]
        mov    cl, byte ptr [eax]
        mov    byte ptr [edx], cl
loc_2_1129:
        mov    esi, dword ptr [esi]
        test   esi, esi
        jne    loc_2_10F1
loc_2_112F:
        xor    eax, eax
        jmp    loc_2_1138
loc_2_1133:
        mov    eax, 0x13
loc_2_1138:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_113E:
        push   ebp
        mov    eax, dword ptr [ecx + 0xc]
        mov    ebp, esp
        sub    esp, 0x3c
        test   eax, eax
        push   ebx
        push   esi
        mov    dword ptr [ebp - 0x20], eax
        push   edi
        je     loc_2_1590
        mov    esi, dword ptr [ecx + 8]
        push   0
        push   0
        lea    ecx, [ebp - 0x20]
        push   0x43
        push   dword ptr [ebp - 0x20]
        push   ecx
        call   sub_2_7908
        add    esp, 0x14
        test   eax, eax
        je     loc_2_117B
        mov    eax, 1
        jmp    loc_2_1592
loc_2_117B:
        xor    ebx, ebx
        push   2
        push   esi
        lea    eax, [ebp - 0x34]
        push   eax
        mov    byte ptr [ebp - 3], bl
        call   sub_2_7918
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_11D2
        push   ebx
        lea    ecx, [ebp - 0x34]
        push   ebx
        push   0x43
        push   dword ptr [ebp - 0x34]
        push   ecx
        call   sub_2_7908
        add    esp, 0x14
        test   eax, eax
        jne    loc_2_11D2
        mov    eax, dword ptr [ebp - 0x34]
        mov    cl, byte ptr [eax]
        mov    byte ptr [ebp - 3], cl
        mov    ecx, esi
        mov    edx, dword ptr [ebp - 3]
        call   sub_2_0F26
        test   eax, eax
        je     loc_2_11D2
        mov    eax, dword ptr [ebp - 0x20]
        mov    cl, byte ptr [ebp - 3]
        mov    byte ptr [eax], cl
        mov    eax, 1
        jmp    loc_2_1592
loc_2_11D2:
        push   0
        lea    eax, [ebp - 0xc]
        push   eax
        call   sub_2_7998
        add    esp, 8
        test   eax, eax
        jne    loc_2_1590
        xor    edi, edi
        lea    edx, [ebp - 1]
        push   edi
        mov    ecx, esi
        push   edi
        push   edi
        call   sub_2_4212
        movzx  ecx, byte ptr [ebp - 1]
        push   1
        push   dword ptr [ebp - 0xc]
        push   ecx
        push   edi
        call   sub_2_7928
        add    esp, 0x10
        test   eax, eax
        je     loc_2_1214
        push   edi
        jmp    loc_2_1585
loc_2_1214:
        push   1
        mov    ecx, dword ptr [ebp - 1]
        push   dword ptr [ebp - 0xc]
        push   0x100
        call   sub_2_09DE
        movzx  eax, al
        inc    eax
        push   eax
        call   sub_2_7928
        add    esp, 0x10
        test   eax, eax
        jne    loc_2_1583
        xor    edi, edi
        xor    ebx, ebx
        mov    dword ptr [ebp - 0x1c], esi
loc_2_1242:
        push   ebx
        lea    ecx, [ebp - 0x1c]
        push   dword ptr [ebp - 0x1c]
        push   ecx
        call   sub_2_7958
        add    esp, 0xc
        test   eax, eax
        mov    eax, dword ptr [0]            ; -> obj3:0x0DAC
        jne    loc_2_1307
        cmp    eax, dword ptr [ebp - 0x1c]
        je     loc_2_1307
        push   ebx
        lea    ecx, [ebp - 0x14]
        push   dword ptr [ebp - 0x1c]
        push   ecx
        call   sub_2_7958
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_1583
        push   ebx
        lea    ecx, [ebp - 0x10]
        push   dword ptr [ebp - 0x14]
        push   ecx
        call   sub_2_7968
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_1583
        lea    edx, [ebp - 7]
        mov    ecx, dword ptr [ebp - 0x1c]
        call   sub_2_0E63
        test   eax, eax
        je     loc_2_1583
loc_2_12AB:
        mov    eax, dword ptr [ebp - 0x10]
        cmp    dword ptr [ebp - 0x1c], eax
        je     loc_2_12EB
        lea    edx, [ebp - 4]
        mov    ecx, eax
        call   sub_2_0E63
        test   eax, eax
        je     loc_2_12EB
        mov    al, byte ptr [ebp - 4]
        cmp    byte ptr [ebp - 7], al
        ja     loc_2_12EB
        mov    edi, 1
        push   ebx
        push   dword ptr [ebp - 0xc]
        push   0xff
        movzx  ecx, al
        push   ecx
        call   sub_2_7928
        add    esp, 0x10
        test   eax, eax
        jne    loc_2_1583
loc_2_12EB:
        push   ebx
        lea    ecx, [ebp - 0x10]
        push   dword ptr [ebp - 0x10]
        push   ecx
        call   sub_2_7960
        add    esp, 0xc
        test   eax, eax
        je     loc_2_12AB
        test   edi, edi
        je     loc_2_1242
loc_2_1307:
        push   0
        lea    eax, [ebp - 0x14]
        push   esi
        push   eax
        call   sub_2_7958
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_1583
        push   0
        lea    ecx, [ebp - 0x10]
        push   dword ptr [ebp - 0x14]
        push   ecx
        call   sub_2_7968
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_1583
loc_2_1337:
        cmp    dword ptr [ebp - 0x10], esi
        je     loc_2_1371
        lea    edx, [ebp - 4]
        mov    ecx, dword ptr [ebp - 0x10]
        call   sub_2_0E63
        test   eax, eax
        je     loc_2_1371
        mov    ecx, dword ptr [ebp - 0x10]
        call   sub_2_0ECA
        movzx  eax, al
        movzx  edx, byte ptr [ebp - 4]
        push   1
        push   dword ptr [ebp - 0xc]
        push   eax
        push   edx
        call   sub_2_7928
        add    esp, 0x10
        test   eax, eax
        jne    loc_2_1583
loc_2_1371:
        push   0
        lea    ecx, [ebp - 0x10]
        push   dword ptr [ebp - 0x10]
        push   ecx
        call   sub_2_7960
        add    esp, 0xc
        test   eax, eax
        je     loc_2_1337
        push   0
        lea    eax, [ebp - 0x28]
        push   eax
        call   sub_2_7998
        add    esp, 8
        test   eax, eax
        jne    loc_2_1583
        xor    edi, edi
        push   edi
        push   0xff
        push   dword ptr [ebp - 0x28]
        push   dword ptr [ebp - 0xc]
        call   sub_2_7990
        add    esp, 0x10
        test   eax, eax
        je     loc_2_13C8
        push   edi
        push   dword ptr [ebp - 0x28]
        call   sub_2_7900
        add    esp, 8
        push   edi
        jmp    loc_2_1585
loc_2_13C8:
        push   0
        lea    eax, [ebp - 0x38]
        push   eax
        lea    ecx, [ebp - 0x24]
        push   ecx
        lea    edx, [ebp - 0x18]
        push   edx
        push   dword ptr [ebp - 0x28]
        call   sub_2_7988
        add    esp, 0x14
        test   eax, eax
        jne    loc_2_1576
        mov    ebx, dword ptr [ebp - 0x18]
        mov    edi, dword ptr [ebp - 0x24]
        push   0
        lea    ecx, [ebp - 0x24]
        push   ecx
        lea    eax, [ebp - 0x18]
        push   eax
        lea    edx, [ebp - 0x38]
        push   edx
        mov    dword ptr [ebp - 0x3c], ebx
        call   sub_2_7980
        add    esp, 0x10
        test   eax, eax
        jne    loc_2_143E
loc_2_140C:
        mov    eax, edi
        mov    ecx, dword ptr [ebp - 0x24]
        sub    eax, dword ptr [ebp - 0x3c]
        sub    ecx, dword ptr [ebp - 0x18]
        cmp    eax, ecx
        jae    loc_2_1424
        mov    eax, dword ptr [ebp - 0x18]
        mov    edi, dword ptr [ebp - 0x24]
        mov    dword ptr [ebp - 0x3c], eax
loc_2_1424:
        push   0
        lea    eax, [ebp - 0x24]
        push   eax
        lea    ecx, [ebp - 0x18]
        push   ecx
        lea    edx, [ebp - 0x38]
        push   edx
        call   sub_2_7980
        add    esp, 0x10
        test   eax, eax
        je     loc_2_140C
loc_2_143E:
        mov    eax, dword ptr [ebp - 0x20]
        push   0
        push   esi
        lea    ecx, [ebp - 0x2c]
        push   ecx
        mov    byte ptr [eax], bl
        call   sub_2_7968
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_14B4
        xor    edi, edi
loc_2_1458:
        lea    eax, [ebp - 6]
        lea    ecx, [ebp - 5]
        push   eax
        lea    edx, [ebp - 2]
        push   ecx
        push   edx
        xor    edx, edx
        mov    ecx, dword ptr [ebp - 0x2c]
        call   sub_2_4212
        mov    edx, ebx
        mov    ecx, dword ptr [ebp - 0x2c]
        push   dword ptr [ebp - 6]
        push   dword ptr [ebp - 5]
        push   dword ptr [ebp - 2]
        call   sub_2_426A
        mov    word ptr [ebp - 0x2e], di
        mov    cl, byte ptr [ebp - 2]
        push   edi
        lea    edx, [ebp - 0x30]
        push   edx
        mov    byte ptr [ebp - 0x30], bl
        push   4
        mov    byte ptr [ebp - 0x2f], cl
        push   4
        push   esi
        call   sub_2_7978
        add    esp, 0x14
        lea    eax, [ebp - 0x2c]
        push   edi
        push   dword ptr [ebp - 0x2c]
        push   eax
        call   sub_2_7960
        add    esp, 0xc
        test   eax, eax
        je     loc_2_1458
loc_2_14B4:
        push   0
        lea    eax, [ebp - 2]
        push   0
        lea    edx, [ebp - 1]
        push   eax
        mov    ecx, esi
        call   sub_2_4212
        mov    edx, dword ptr [ebp - 2]
        mov    ecx, dword ptr [ebp - 1]
        push   dword ptr [ebp - 1]
        push   0x18
        call   sub_2_58EF
        push   ebx
        mov    edx, dword ptr [ebp - 2]
        push   0x19
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_58EF
        mov    ecx, esi
        call   sub_2_0ECA
        cmp    al, bl
        ja     loc_2_14F1
        mov    al, bl
loc_2_14F1:
        push   eax
        mov    edx, dword ptr [ebp - 2]
        push   0x1a
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_58EF
        push   0
        mov    dword ptr [ebp - 0x14], esi
        push   esi
        lea    esi, [ebp - 0x14]
        push   esi
        call   sub_2_7958
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_1565
        xor    esi, esi
loc_2_1517:
        mov    eax, dword ptr [0]            ; -> obj3:0x0DAC
        cmp    eax, dword ptr [ebp - 0x14]
        je     loc_2_1565
        push   esi
        lea    eax, [ebp - 2]
        push   esi
        lea    edx, [ebp - 1]
        push   eax
        mov    ecx, dword ptr [ebp - 0x14]
        call   sub_2_4212
        push   0x1a
        mov    edx, dword ptr [ebp - 2]
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_58C5
        cmp    al, bl
        jae    loc_2_1565
        push   ebx
        mov    edx, dword ptr [ebp - 2]
        push   0x1a
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_58EF
        push   esi
        lea    ecx, [ebp - 0x14]
        push   dword ptr [ebp - 0x14]
        push   ecx
        call   sub_2_7958
        add    esp, 0xc
        test   eax, eax
        je     loc_2_1517
loc_2_1565:
        mov    edx, ebx
        mov    ecx, dword ptr [ebp - 3]
        call   sub_2_0B7B
        mov    eax, 1
        jmp    loc_2_1592
loc_2_1576:
        push   0
        push   dword ptr [ebp - 0x28]
        call   sub_2_7900
        add    esp, 8
loc_2_1583:
        push   0
loc_2_1585:
        push   dword ptr [ebp - 0xc]
        call   sub_2_7900
        add    esp, 8
loc_2_1590:
        xor    eax, eax
loc_2_1592:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_1599:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        push   edi
        mov    edi, ecx
        mov    esi, ecx
        test   esi, esi
        je     loc_2_15BA
loc_2_15A9:
        mov    ecx, edi
        call   sub_2_113E
        test   eax, eax
        je     loc_2_15BA
        mov    edi, dword ptr [edi]
        test   edi, edi
        jne    loc_2_15A9
loc_2_15BA:
        test   edi, edi
        jne    loc_2_15C2
        xor    eax, eax
        jmp    loc_2_1624
loc_2_15C2:
        mov    ecx, esi
        call   sub_2_0D65
        test   eax, eax
        jne    loc_2_161F
        test   esi, esi
        je     loc_2_161B
        xor    edi, edi
loc_2_15D3:
        push   edi
        lea    eax, [ebp - 2]
        push   edi
        lea    edx, [ebp - 1]
        push   eax
        mov    ecx, dword ptr [esi + 8]
        call   sub_2_4212
        mov    eax, dword ptr [esi + 0xc]
        mov    dword ptr [ebp - 8], eax
        cmp    eax, edi
        je     loc_2_161B
        push   edi
        lea    ecx, [ebp - 8]
        push   edi
        push   0x43
        push   eax
        push   ecx
        call   sub_2_7908
        add    esp, 0x14
        test   eax, eax
        jne    loc_2_1615
        push   0x19
        mov    edx, dword ptr [ebp - 2]
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_58C5
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ecx], al
loc_2_1615:
        mov    esi, dword ptr [esi]
        test   esi, esi
        jne    loc_2_15D3
loc_2_161B:
        xor    eax, eax
        jmp    loc_2_1624
loc_2_161F:
        mov    eax, 0x13
loc_2_1624:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_162A:
        mov    eax, dword ptr [esp + 4]
        test   eax, eax
        je     loc_2_1643
        cmp    eax, 1
        je     loc_2_1650
        cmp    eax, 6
        je     loc_2_1650
        mov    eax, 1
        jmp    loc_2_165B
loc_2_1643:
        mov    eax, dword ptr [esp + 0x10]
        mov    ecx, dword ptr [eax]
        call   sub_2_1599
        jmp    loc_2_165B
loc_2_1650:
        mov    eax, dword ptr [esp + 0x10]
        mov    ecx, dword ptr [eax]
        jmp    loc_2_1092
loc_2_165B:
        ret    
sub_2_165C:
        cmp    dword ptr [0], -1             ; -> obj3:0x0CFC
        mov    eax, 0                        ; -> obj3:0x0CFC
        je     loc_2_1684
        mov    edx, eax
        mov    ecx, dword ptr [esp + 4]
loc_2_1670:
        cmp    dword ptr [eax], ecx
        jne    loc_2_167A
        cmp    byte ptr [eax + 0xc], 0
        je     loc_2_1687
loc_2_167A:
        add    edx, 0xe
        mov    eax, edx
        cmp    dword ptr [edx], -1
        jne    loc_2_1670
loc_2_1684:
        xor    eax, eax
loc_2_1686:
        ret    
loc_2_1687:
        cmp    byte ptr [esp + 8], 0
        je     loc_2_1686
        mov    byte ptr [eax + 0xc], 1
        jmp    loc_2_1686
sub_2_1694:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        mov    eax, dword ptr [0]            ; -> obj3:0x00D8
        push   ebx
        push   esi
        push   edi
        mov    edi, edx
        mov    esi, ecx
        push   0
        lea    ecx, [ebp - 4]
        push   eax
        push   ecx
        call   sub_2_7918
        add    esp, 0xc
        test   eax, eax
        je     loc_2_16DA
        push   0
        mov    eax, dword ptr [0]            ; -> obj3:0x00D8
        push   0x3000
        lea    ecx, [ebp - 4]
        push   eax
        push   ecx
        call   sub_2_79A8
        add    esp, 0x10
        test   eax, eax
        jne    loc_2_17B4
loc_2_16DA:
        xor    ebx, ebx
        push   ebx
        push   dword ptr [ebp + 8]
        push   edi
        push   esi
        push   dword ptr [ebp - 4]
        push   ebx
        call   sub_2_79A0
        add    esp, 0x18
        test   eax, eax
        jne    loc_2_17B4
        movzx  eax, byte ptr [0]             ; -> obj3:0x00E8
        cmp    eax, 1
        jl     loc_2_174F
        cmp    eax, 2
        jg     loc_2_174F
        push   ebx
        lea    ecx, [ebp - 8]
        push   dword ptr [ebp - 4]
        push   ecx
        call   sub_2_79B0
        add    esp, 0xc
        test   eax, eax
        je     loc_2_1737
        push   ebx
        mov    eax, dword ptr [0]            ; -> obj3:0x00D8
        push   0xf000
        lea    ecx, [ebp - 8]
        push   eax
        push   ecx
        call   sub_2_79A8
        add    esp, 0x10
        test   eax, eax
        jne    loc_2_17B4
loc_2_1737:
        push   0
        push   dword ptr [ebp + 8]
        push   edi
        push   esi
        push   dword ptr [ebp - 8]
        push   0
        call   sub_2_79A0
        add    esp, 0x18
        test   eax, eax
        jne    loc_2_17B4
loc_2_174F:
        cmp    dword ptr [0], 0              ; -> obj3:0x00D4
        je     loc_2_17B4
        push   3
        mov    eax, dword ptr [0]            ; -> obj3:0x00D8
        push   eax
        lea    ecx, [ebp - 4]
        push   ecx
        call   sub_2_7918
        add    esp, 0xc
        test   eax, eax
        je     loc_2_178A
        push   3
        mov    eax, dword ptr [0]            ; -> obj3:0x00D8
        push   1
        lea    ecx, [ebp - 4]
        push   eax
        push   ecx
        call   sub_2_79A8
        add    esp, 0x10
        test   eax, eax
        jne    loc_2_17B4
loc_2_178A:
        push   0
        mov    eax, 0
        push   dword ptr [ebp + 8]
        push   edi
        cmp    dword ptr [0], 1              ; -> obj3:0x00F4
        adc    eax, -1
        and    eax, 0x8000
        or     eax, esi
        push   eax
        push   dword ptr [ebp - 4]
        push   0
        call   sub_2_79A0
        add    esp, 0x18
loc_2_17B4:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
sub_2_17BD:
        push   ebp
        mov    eax, 0
        mov    ebp, esp
        sub    esp, 8
        mov    word ptr [ebp - 4], dx
        cmp    ecx, 1
        adc    eax, -1
        push   8
        and    ax, 3
        lea    edx, [ebp - 8]
        mov    word ptr [ebp - 2], 0
        movzx  cx, byte ptr [ebp + 8]
        mov    word ptr [ebp - 8], ax
        mov    word ptr [ebp - 6], cx
        mov    ecx, 4
        call   sub_2_1694
        mov    esp, ebp
        pop    ebp
        ret    4
sub_2_17FE:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x1c
        mov    word ptr [ebp - 0x18], cx
        push   esi
        mov    word ptr [ebp - 0x1c], 1
        mov    word ptr [ebp - 0x1a], 0xc
        mov    byte ptr [ebp - 0x12], 0xff
        mov    si, dx
        xor    edx, edx
        mov    word ptr [ebp - 0x14], dx
        mov    byte ptr [ebp - 0x11], dl
        mov    dx, word ptr [ebp + 8]
        lea    eax, [esi + ecx]
        dec    ax
        mov    word ptr [ebp - 0x16], ax
        cmp    byte ptr [ebp + 0xc], 0xff
        mov    word ptr [ebp - 0x10], dx
        mov    word ptr [ebp - 0xe], si
        jne    loc_2_184A
        mov    word ptr [ebp - 0xc], cx
        mov    word ptr [ebp - 0xa], ax
        jmp    loc_2_1856
loc_2_184A:
        mov    word ptr [ebp - 0xc], 0
        mov    word ptr [ebp - 0xa], 0xffff
loc_2_1856:
        xor    eax, eax
        push   0x1c
        mov    word ptr [ebp - 8], ax
        lea    edx, [ebp - 0x1c]
        cmp    si, 1
        mov    byte ptr [ebp - 5], al
        mov    byte ptr [ebp - 6], 0xff
        movzx  ecx, byte ptr [ebp + 0xc]
        mov    dword ptr [ebp - 4], ecx
        sbb    ecx, ecx
        and    ecx, 0x8000
        mov    cl, 2
        call   sub_2_1694
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    8
sub_2_1889:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x28
        mov    word ptr [ebp - 0x28], 1
        push   esi
        mov    dword ptr [ebp - 0x24], ecx
        lea    eax, [ecx + edx]
        mov    esi, edx
        dec    eax
        mov    ecx, 0
        cmp    dword ptr [ebp + 0x10], 1
        mov    word ptr [ebp - 0x26], 0x14
        adc    ecx, -1
        mov    dword ptr [ebp - 0x20], eax
        and    cx, 4
        xor    edx, edx
        mov    word ptr [ebp - 0x1a], dx
        or     cl, 1
        mov    word ptr [ebp - 0x1c], cx
        mov    eax, dword ptr [ebp + 8]
        mov    word ptr [ebp - 8], cx
        mov    dword ptr [ebp - 0x18], eax
        mov    word ptr [ebp - 6], dx
        mov    eax, dword ptr [ebp + 0x14]
        push   0x28
        mov    dword ptr [ebp - 0x14], esi
        lea    edx, [ebp - 0x28]
        mov    dword ptr [ebp - 0x10], eax
        mov    eax, dword ptr [ebp + 0x18]
        cmp    esi, 1
        movzx  ecx, byte ptr [ebp + 0xc]
        mov    dword ptr [ebp - 0xc], eax
        mov    dword ptr [ebp - 4], ecx
        sbb    ecx, ecx
        and    ecx, 0x8000
        mov    cl, 1
        call   sub_2_1694
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0x14
sub_2_1907:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        cmp    dword ptr [0], 0              ; -> obj3:0x0034
        push   ebx
        jne    loc_2_1924
        cmp    byte ptr [0], 0               ; -> obj3:0x0038
        je     loc_2_1A27
loc_2_1924:
        cmp    byte ptr [0], 0               ; -> obj3:0x0038
        jne    loc_2_193A
        cmp    dword ptr [0], 0              ; -> obj3:0x00D4
        jne    loc_2_1A27
loc_2_193A:
        mov    edx, dword ptr [0]            ; -> obj3:0x00D8
        mov    ecx, edx
        call   sub_2_32C0
        test   eax, eax
        jne    loc_2_1A09
        mov    cl, byte ptr [0]              ; -> obj3:0x0038
        test   cl, cl
        je     loc_2_1984
        cmp    dword ptr [0], 0              ; -> obj3:0x0034
        je     loc_2_197A
        mov    dx, 1
        push   ecx
        shl    dx, cl
        mov    ecx, 1
        call   sub_2_17BD
        mov    cl, byte ptr [0]              ; -> obj3:0x0038
loc_2_197A:
        call   sub_2_33D9
        jmp    loc_2_1A27
loc_2_1984:
        xor    ebx, ebx
        cmp    dword ptr [0], ebx            ; -> obj3:0x0034
        je     loc_2_19F7
        push   ebx
        mov    eax, dword ptr [0]            ; -> obj3:0x00D8
        push   eax
        lea    ecx, [ebp - 8]
        push   ecx
        call   sub_2_7958
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_19F7
        mov    eax, dword ptr [0]            ; -> obj3:0x0DAC
        cmp    eax, dword ptr [ebp - 8]
        je     loc_2_19F7
        push   ebx
        lea    eax, [ebp - 2]
        push   ebx
        lea    edx, [ebp - 1]
        push   eax
        mov    ecx, dword ptr [ebp - 8]
        call   sub_2_4212
        mov    edx, dword ptr [ebp - 2]
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_406B
        cmp    al, 2
        jne    loc_2_19F7
        push   0x3c
        mov    edx, dword ptr [ebp - 2]
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_58C5
        cmp    al, bl
        mov    cl, al
        je     loc_2_19F7
        cmp    cl, 0xf
        ja     loc_2_19F7
        mov    dx, 1
        push   ecx
        shl    dx, cl
        mov    byte ptr [0], cl              ; -> obj3:0x0038
        jmp    loc_2_1A1D
loc_2_19F7:
        push   2
        mov    eax, dword ptr [0]            ; -> obj3:0x00D8
        push   eax
        call   sub_2_79B8
        add    esp, 8
        jmp    loc_2_1A27
loc_2_1A09:
        cmp    dword ptr [0], 0              ; -> obj3:0x0034
        je     loc_2_1A27
        mov    dx, word ptr [eax + 2]
        mov    cl, byte ptr [0]              ; -> obj3:0x0038
        push   ecx
loc_2_1A1D:
        mov    ecx, 1
        call   sub_2_17BD
loc_2_1A27:
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_1A2C:
        push   ebx
        push   esi
        xor    bl, bl
        cmp    byte ptr [0], bl              ; -> obj3:0x00A4
        je     loc_2_1A8B
loc_2_1A38:
        movzx  ecx, bl
        mov    edx, dword ptr [ecx*4]        ; -> obj3:0x0054
        lea    esi, [ecx*4]
        test   edx, edx
        je     loc_2_1A81
        cmp    dword ptr [esi], 1            ; -> obj3:0x0084
        sbb    eax, eax
        mov    cl, byte ptr [ecx]            ; -> obj3:0x009C
        and    eax, 0xfff00000
        add    eax, 0xfffff
        push   eax
        push   0
        push   dword ptr [esi]               ; -> obj3:0x006C
        push   ecx
        lea    eax, [edx - 1]
        mov    ecx, dword ptr [esi]          ; -> obj3:0x003C
        not    eax
        push   eax
        call   sub_2_1889
loc_2_1A81:
        inc    bl
        cmp    byte ptr [0], bl              ; -> obj3:0x00A4
        ja     loc_2_1A38
loc_2_1A8B:
        pop    esi
        pop    ebx
        ret    
sub_2_1A8E:
        push   ebx
        xor    bl, bl
        cmp    byte ptr [0], bl              ; -> obj3:0x00C8
        je     loc_2_1AD0
loc_2_1A99:
        movzx  eax, bl
        mov    dx, word ptr [eax*2]          ; -> obj3:0x00B4
        test   dx, dx
        je     loc_2_1AC6
        mov    cl, byte ptr [eax]            ; -> obj3:0x00C0
        push   ecx
        mov    cx, dx
        dec    cx
        not    cx
        push   ecx
        mov    cx, word ptr [eax*2]          ; -> obj3:0x00A8
        call   sub_2_17FE
loc_2_1AC6:
        inc    bl
        cmp    byte ptr [0], bl              ; -> obj3:0x00C8
        ja     loc_2_1A99
loc_2_1AD0:
        pop    ebx
        ret    
loc_2_1AD2:
        cmp    dword ptr [0], 0              ; -> obj3:0x00D0
        je     loc_2_1AFC
        push   -1
        mov    edx, dword ptr [0]            ; -> obj3:0x00CC
        push   0x400000
        xor    ecx, ecx
        push   0
        lea    eax, [edx - 1]
        not    eax
        push   0xfc
        push   eax
        call   sub_2_1889
loc_2_1AFC:
        ret    
sub_2_1AFD:
        push   esi
        mov    esi, 3
loc_2_1B03:
        push   0
        xor    edx, edx
        mov    ecx, 0x8000
        call   sub_2_1694
        dec    esi
        jne    loc_2_1B03
        pop    esi
        ret    
sub_2_1B16:
        push   ebx
        push   esi
        mov    si, dx
        mov    bl, cl
        push   ebx
        mov    dl, byte ptr [0]              ; -> obj3:0x00E0
        mov    cl, byte ptr [0]              ; -> obj3:0x00DC
        call   sub_2_58E1
        and    ax, 0xfffc
        jne    loc_2_1B3C
        call   sub_2_1AFD
        jmp    loc_2_1BAE
loc_2_1B3C:
        push   ebx
        mov    edx, 8
        push   0xffff
        mov    ecx, eax
        call   sub_2_17FE
        mov    al, bl
        mov    dl, byte ptr [0]              ; -> obj3:0x00E0
        add    al, 4
        mov    cl, byte ptr [0]              ; -> obj3:0x00DC
        push   eax
        call   sub_2_58E1
        mov    ecx, eax
        and    cx, 0xfffc
        movzx  eax, cx
        and    al, 0xf8
        cmp    eax, 0x370
        je     loc_2_1B7E
        cmp    eax, 0x3f0
        jne    loc_2_1B8B
loc_2_1B7E:
        and    cx, 0xfff8
        add    cx, 6
        mov    si, 1
loc_2_1B8B:
        cmp    si, 1
        mov    eax, 0xff
        je     loc_2_1B9C
        movzx  eax, bl
        add    eax, 4
loc_2_1B9C:
        push   eax
        mov    edx, esi
        push   0xffff
        call   sub_2_17FE
        call   sub_2_1907
loc_2_1BAE:
        pop    esi
        pop    ebx
        ret    
sub_2_1BB1:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x28
        cmp    dword ptr [0], 0              ; -> obj3:0x00D4
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        je     loc_2_1C5C
        xor    cl, cl
        cmp    byte ptr [0], cl              ; -> obj3:0x00A4
        je     loc_2_1C06
loc_2_1BD3:
        movzx  eax, cl
        shl    eax, 2
        lea    edi, [eax]                    ; -> obj3:0x0054
        mov    ebx, dword ptr [edi]
        test   ebx, ebx
        je     loc_2_1BFC
        mov    eax, dword ptr [eax]          ; -> obj3:0x003C
        cmp    esi, eax
        jb     loc_2_1BFC
        add    eax, ebx
        dec    eax
        cmp    eax, edx
        jb     loc_2_1BFC
        mov    dword ptr [edi], 0
loc_2_1BFC:
        inc    cl
        cmp    byte ptr [0], cl              ; -> obj3:0x00A4
        ja     loc_2_1BD3
loc_2_1C06:
        mov    ecx, 1
        mov    dword ptr [ebp - 0x24], esi
        mov    word ptr [ebp - 0x28], cx
        mov    dword ptr [ebp - 0x20], edx
        mov    word ptr [ebp - 0x26], 0x14
        mov    dword ptr [ebp - 0x18], 0xffffffff
        mov    ebx, edx
        cmp    dword ptr [ebp + 8], ecx
        sbb    eax, eax
        xor    edi, edi
        add    ax, cx
        sub    ebx, esi
        mov    word ptr [ebp - 0x1c], ax
        add    ebx, ecx
        mov    word ptr [ebp - 0x1a], di
        mov    dword ptr [ebp - 0x14], ebx
        mov    word ptr [ebp - 8], ax
        mov    dword ptr [ebp - 0x10], esi
        mov    word ptr [ebp - 6], di
        push   0x28
        mov    dword ptr [ebp - 4], 0xff
        mov    dword ptr [ebp - 0xc], edx
        lea    edx, [ebp - 0x28]
        call   sub_2_1694
loc_2_1C5C:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
sub_2_1C65:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x1c
        cmp    dword ptr [0], 0              ; -> obj3:0x00D4
        push   ebx
        push   esi
        push   edi
        je     loc_2_1D21
        xor    bl, bl
        cmp    byte ptr [0], bl              ; -> obj3:0x00C8
        je     loc_2_1CC3
loc_2_1C85:
        movzx  eax, bl
        add    eax, eax
        lea    edi, [eax]                    ; -> obj3:0x00B4
        mov    si, word ptr [edi]
        test   si, si
        je     loc_2_1CB9
        mov    ax, word ptr [eax]            ; -> obj3:0x00A8
        cmp    cx, ax
        jb     loc_2_1CB9
        movzx  esi, si
        movzx  eax, ax
        add    esi, eax
        movzx  eax, dx
        dec    esi
        cmp    esi, eax
        jl     loc_2_1CB9
        mov    word ptr [edi], 0
loc_2_1CB9:
        inc    bl
        cmp    bl, byte ptr [0]              ; -> obj3:0x00C8
        jb     loc_2_1C85
loc_2_1CC3:
        mov    word ptr [ebp - 0x18], cx
        xor    eax, eax
        mov    byte ptr [ebp - 0x12], 4
        mov    byte ptr [ebp - 0x11], 3
        mov    word ptr [ebp - 0x1c], 1
        mov    word ptr [ebp - 0x1a], 0xc
        mov    word ptr [ebp - 0x10], 0xffff
        mov    si, dx
        sub    si, cx
        inc    si
        mov    word ptr [ebp - 0x16], dx
        mov    word ptr [ebp - 0x14], ax
        mov    byte ptr [ebp - 6], 4
        mov    byte ptr [ebp - 5], 3
        mov    dword ptr [ebp - 4], 0xff
        mov    word ptr [ebp - 0xe], si
        push   0x1c
        mov    word ptr [ebp - 0xc], cx
        mov    ecx, 2
        mov    word ptr [ebp - 0xa], dx
        lea    edx, [ebp - 0x1c]
        mov    word ptr [ebp - 8], ax
        call   sub_2_1694
loc_2_1D21:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_1D28:
        mov    byte ptr [0], 0               ; -> obj3:0x00C8
        mov    byte ptr [0], 0               ; -> obj3:0x00A4
        mov    dword ptr [0], 0              ; -> obj3:0x00D0
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        movzx  eax, cl
        push   ebx
        push   esi
        push   edi
        mov    bl, 0x10
        lea    esi, [eax*4 + 0x10]
        cmp    esi, 0x10
        jle    loc_2_1E89
loc_2_1D5E:
        mov    eax, dword ptr [0]            ; -> obj3:0x00D4
        mov    cl, byte ptr [0]              ; -> obj3:0x00E0
        push   eax
        mov    dl, byte ptr [0]              ; -> obj3:0x00DC
        push   ebx
        push   ecx
        push   edx
        lea    ecx, [ebp - 8]
        lea    edx, [ebp - 4]
        call   sub_2_4F1B
        test   eax, eax
        mov    al, byte ptr [0]              ; -> obj3:0x00A4
        je     loc_2_1E6F
        test   byte ptr [ebp - 4], 1
        je     loc_2_1DDA
        mov    ecx, dword ptr [ebp - 4]
        not    cx
        or     cl, 3
        inc    cx
        movzx  eax, cx
        lea    edx, [eax - 1]
        test   eax, edx
        je     loc_2_1DAA
        mov    cx, 4
loc_2_1DAA:
        movzx  eax, byte ptr [0]             ; -> obj3:0x00C8
        mov    word ptr [eax*2], cx          ; -> obj3:0x00B4
        mov    edx, dword ptr [ebp - 8]
        and    dx, 0xfffc
        mov    byte ptr [eax], bl            ; -> obj3:0x00C0
        mov    word ptr [eax*2], dx          ; -> obj3:0x00A8
        inc    byte ptr [0]                  ; -> obj3:0x00C8
        jmp    loc_2_1E6F
loc_2_1DDA:
        test   byte ptr [ebp - 4], 8
        je     loc_2_1DF9
        test   byte ptr [0], 0x40            ; -> obj3:0x0CB8
        jne    loc_2_1DF9
        movzx  ecx, al
        mov    dword ptr [ecx*4], 1          ; -> obj3:0x006C
        jmp    loc_2_1E07
loc_2_1DF9:
        movzx  ecx, al
        mov    dword ptr [ecx*4], 0          ; -> obj3:0x006C
loc_2_1E07:
        movzx  edx, al
        mov    ecx, dword ptr [ebp - 4]
        and    ecx, 7
        sub    ecx, 2
        cmp    ecx, 1
        sbb    ecx, ecx
        neg    ecx
        mov    dword ptr [edx*4], ecx        ; -> obj3:0x0084
        test   ecx, ecx
        je     loc_2_1E2C
        or     dword ptr [ebp - 4], 0xfff00000
loc_2_1E2C:
        and    byte ptr [ebp - 4], 0xf0
        mov    ecx, dword ptr [ebp - 4]
        not    ecx
        mov    dword ptr [ebp - 4], ecx
        inc    dword ptr [ebp - 4]
        mov    ecx, dword ptr [ebp - 4]
        dec    ecx
        test   dword ptr [ebp - 4], ecx
        je     loc_2_1E4B
        mov    dword ptr [ebp - 4], 0x10
loc_2_1E4B:
        mov    ecx, dword ptr [ebp - 8]
        mov    edi, dword ptr [ebp - 4]
        and    cl, 0xf0
        inc    al
        mov    dword ptr [edx*4], ecx        ; -> obj3:0x003C
        mov    dword ptr [edx*4], edi        ; -> obj3:0x0054
        mov    byte ptr [edx], bl            ; -> obj3:0x009C
        mov    byte ptr [0], al              ; -> obj3:0x00A4
loc_2_1E6F:
        mov    eax, dword ptr [ebp - 8]
        and    al, 7
        cmp    al, 4
        jne    loc_2_1E7B
        add    bl, 4
loc_2_1E7B:
        add    bl, 4
        movzx  eax, bl
        cmp    eax, esi
        jl     loc_2_1D5E
loc_2_1E89:
        push   0x3d
        mov    al, byte ptr [0]              ; -> obj3:0x00E0
        mov    edx, eax
        mov    bl, byte ptr [0]              ; -> obj3:0x00DC
        mov    ecx, ebx
        call   sub_2_58C5
        cmp    al, 1
        sbb    ecx, ecx
        inc    ecx
        cmp    al, 4
        mov    dword ptr [0], ecx            ; -> obj3:0x0034
        jbe    loc_2_1EC3
        push   1
        mov    eax, dword ptr [0]            ; -> obj3:0x00D8
        push   eax
        call   sub_2_79B8
        add    esp, 8
        jmp    loc_2_1F5A
loc_2_1EC3:
        push   0x3c
        mov    dl, byte ptr [0]              ; -> obj3:0x00E0
        mov    cl, byte ptr [0]              ; -> obj3:0x00DC
        call   sub_2_58C5
        cmp    al, 0xf
        jbe    loc_2_1EE0
        cmp    al, 0x15
        ja     loc_2_1EE0
        sub    al, 6
loc_2_1EE0:
        cmp    dword ptr [0], 0              ; -> obj3:0x0034
        mov    byte ptr [0], al              ; -> obj3:0x0038
        je     loc_2_1EF2
        cmp    al, 0xf
        jbe    loc_2_1EF9
loc_2_1EF2:
        mov    byte ptr [0], 0               ; -> obj3:0x0038
loc_2_1EF9:
        mov    ecx, dword ptr [0]            ; -> obj3:0x00D8
        call   sub_2_430E
        test   eax, eax
        je     loc_2_1F2D
        push   1
        mov    eax, dword ptr [0]            ; -> obj3:0x00EC
        push   eax
        call   sub_2_165C
        add    esp, 8
        test   eax, eax
        je     loc_2_1F2D
        mov    ecx, dword ptr [eax + 4]
        push   0
        mov    eax, dword ptr [eax + 8]
        lea    edx, [eax + ecx - 1]
        call   sub_2_1BB1
loc_2_1F2D:
        cmp    dword ptr [0], 0              ; -> obj3:0x00D4
        jne    loc_2_1F5A
        mov    dl, byte ptr [0]              ; -> obj3:0x00E0
        mov    cl, byte ptr [0]              ; -> obj3:0x00DC
        call   sub_2_4FCA
        mov    dword ptr [0], eax            ; -> obj3:0x00CC
        test   eax, eax
        je     loc_2_1F5A
        mov    dword ptr [0], 1              ; -> obj3:0x00D0
loc_2_1F5A:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_1F61:
        call   sub_2_1907
        call   sub_2_1A2C
        call   sub_2_1A8E
        jmp    loc_2_1AD2
sub_2_1F75:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        test   byte ptr [0], 4               ; -> obj3:0x0CB8
        push   ebx
        mov    byte ptr [ebp - 1], dl
        mov    bl, cl
        jne    loc_2_1FCF
        push   0x18
        mov    edx, dword ptr [ebp - 1]
        mov    ecx, ebx
        call   sub_2_58C5
        push   0x19
        mov    edx, dword ptr [ebp - 1]
        mov    ecx, ebx
        mov    byte ptr [ebp - 8], al
        call   sub_2_58C5
        cmp    byte ptr [ebp - 8], al
        jae    loc_2_1FCF
        push   0x19
        mov    edx, dword ptr [ebp - 1]
        mov    ecx, ebx
        call   sub_2_58C5
        push   0x1a
        mov    edx, dword ptr [ebp - 1]
        mov    ecx, ebx
        mov    byte ptr [ebp - 8], al
        call   sub_2_58C5
        cmp    byte ptr [ebp - 8], al
        mov    eax, 1
        jbe    loc_2_1FD1
loc_2_1FCF:
        xor    eax, eax
loc_2_1FD1:
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_1FD6:
        sub    esp, 4
        mov    al, dl
        mov    dword ptr [esp], 1
        push   ebx
        push   esi
        push   edi
        push   ebp
        mov    ebp, ecx
        movzx  ecx, byte ptr [0]             ; -> obj3:0x00E8
        cmp    ecx, 1
        je     loc_2_1FFC
        cmp    ecx, 2
        je     loc_2_202B
        jmp    loc_2_2049
loc_2_1FFC:
        mov    edx, dword ptr [esp + 0x18]
        mov    ecx, eax
        call   sub_2_5256
        test   eax, eax
        je     loc_2_201B
        mov    esi, 0                        ; -> obj3:0x0180
        mov    edi, ebp
        mov    ecx, 0x22
        rep movsd dword ptr es:[edi], dword ptr [esi]
        jmp    loc_2_2049
loc_2_201B:
        mov    esi, 0                        ; -> obj3:0x00F8
        mov    edi, ebp
        mov    ecx, 0x22
        rep movsd dword ptr es:[edi], dword ptr [esi]
        jmp    loc_2_2049
loc_2_202B:
        mov    esi, 0                        ; -> obj3:0x0208
        mov    edi, ebp
        mov    ecx, 0x22
        rep movsd dword ptr es:[edi], dword ptr [esi]
        cmp    dword ptr [0], 0x6031179      ; -> obj3:0x00EC
        jne    loc_2_2049
        add    byte ptr [ebp + 0x28], 2
loc_2_2049:
        xor    bl, bl
        cmp    byte ptr [ebp], bl
        jbe    loc_2_208A
        xor    esi, esi
loc_2_2052:
        movzx  eax, bl
        imul   eax, eax, 0x2c
        push   esi
        lea    ecx, [eax + ebp + 0x14]
        lea    edi, [eax + ebp + 4]
        push   ecx
        call   sub_2_7998
        add    esp, 8
        test   eax, eax
        jne    loc_2_207F
        push   esi
        add    edi, 0x14
        push   edi
        call   sub_2_7998
        add    esp, 8
        test   eax, eax
        je     loc_2_2083
loc_2_207F:
        mov    dword ptr [esp + 0x10], esi
loc_2_2083:
        inc    bl
        cmp    byte ptr [ebp], bl
        ja     loc_2_2052
loc_2_208A:
        cmp    dword ptr [esp + 0x10], 0
        jne    loc_2_20D9
        xor    bl, bl
        cmp    byte ptr [ebp], bl
        je     loc_2_20CE
loc_2_2098:
        movzx  eax, bl
        imul   eax, eax, 0x2c
        lea    edi, [eax + ebp + 4]
        mov    eax, dword ptr [eax + ebp + 0x14]
        test   eax, eax
        je     loc_2_20B5
        push   0
        push   eax
        call   sub_2_7900
        add    esp, 8
loc_2_20B5:
        mov    eax, dword ptr [edi + 0x14]
        test   eax, eax
        je     loc_2_20C7
        push   0
        push   eax
        call   sub_2_7900
        add    esp, 8
loc_2_20C7:
        inc    bl
        cmp    bl, byte ptr [ebp]
        jb     loc_2_2098
loc_2_20CE:
        push   0
        push   ebp
        call   sub_2_7950
        add    esp, 8
loc_2_20D9:
        mov    eax, dword ptr [esp + 0x10]
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    4
sub_2_20E7:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x28
        cmp    dword ptr [0], 0              ; -> obj3:0x00D4
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        je     loc_2_2114
        mov    al, byte ptr [0]              ; -> obj3:0x00E0
        mov    bl, byte ptr [0]              ; -> obj3:0x00DC
        mov    edx, eax
        mov    ecx, ebx
        call   sub_2_1F75
        mov    dword ptr [0], eax            ; -> obj3:0x00D4
loc_2_2114:
        push   0x19
        mov    al, byte ptr [0]              ; -> obj3:0x00E0
        mov    edx, eax
        mov    bl, byte ptr [0]              ; -> obj3:0x00DC
        mov    ecx, ebx
        call   sub_2_58C5
        push   0x1a
        mov    dl, byte ptr [0]              ; -> obj3:0x00E0
        mov    bl, byte ptr [0]              ; -> obj3:0x00DC
        mov    byte ptr [ebp - 1], al
        mov    ecx, ebx
        call   sub_2_58C5
        cmp    al, byte ptr [0]              ; -> obj3:0x0DB4
        jbe    loc_2_214F
        mov    byte ptr [0], al              ; -> obj3:0x0DB4
loc_2_214F:
        push   1
        mov    dl, byte ptr [0]              ; -> obj3:0x00E0
        push   esi
        mov    cl, byte ptr [0]              ; -> obj3:0x00DC
        call   sub_2_4451
        mov    dword ptr [0], eax            ; -> obj3:0x00D8
        test   eax, eax
        je     loc_2_248E
        test   byte ptr [0], 4               ; -> obj3:0x0CB8
        je     loc_2_218A
        mov    dl, byte ptr [0]              ; -> obj3:0x00E0
        mov    cl, byte ptr [0]              ; -> obj3:0x00DC
        call   sub_2_5868
loc_2_218A:
        push   0
        lea    eax, [ebp - 0x1c]
        push   esi
        push   eax
        call   sub_2_79D0
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 0x1c]
        sub    eax, dword ptr [0]            ; -> obj3:0x0DB8
        cmp    eax, 0x10
        ja     loc_2_248E
        push   0x201
        push   0x88
        call   sub_2_79C8
        add    esp, 8
        mov    esi, eax
        test   esi, esi
        jne    loc_2_21CD
        mov    edx, 1
        jmp    loc_2_2483
loc_2_21CD:
        mov    al, byte ptr [0]              ; -> obj3:0x00E0
        mov    dl, byte ptr [0]              ; -> obj3:0x00DC
        push   eax
        mov    ecx, esi
        call   sub_2_1FD6
        test   eax, eax
        jne    loc_2_21EE
        mov    edx, 1
        jmp    loc_2_2483
loc_2_21EE:
        mov    al, byte ptr [esi]
        test   al, al
        je     loc_2_21FE
        movzx  ecx, al
        lea    edi, [ebp - 0x28]
        xor    eax, eax
        rep stosd dword ptr es:[edi], eax
loc_2_21FE:
        mov    edx, esi
        mov    ecx, dword ptr [0]            ; -> obj3:0x00D8
        call   sub_2_42DD
        cmp    byte ptr [esi], 0
        je     loc_2_2259
        xor    ebx, ebx
        mov    al, byte ptr [esi + 3]
        test   al, 1
        je     loc_2_221E
        mov    ebx, 8
loc_2_221E:
        test   al, 2
        je     loc_2_2225
        or     bl, 0x10
loc_2_2225:
        mov    edi, esi
        push   ebx
        mov    eax, dword ptr [0]            ; -> obj3:0x00D8
        push   eax
        push   edi
        push   0                             ; -> loc_2_08A1
        push   2
        push   0
        call   sub_2_79C0
        add    esp, 0x18
        mov    eax, dword ptr [0]            ; -> obj3:0x00D8
        push   ebx
        push   eax
        push   edi
        push   0                             ; -> loc_2_091A
        push   1
        push   0
        call   sub_2_79C0
        add    esp, 0x18
loc_2_2259:
        xor    ebx, ebx
        cmp    dword ptr [0], ebx            ; -> obj3:0x00D4
        je     loc_2_230C
        mov    dword ptr [ebp - 0x14], ebx
        cmp    byte ptr [esi], bl
        jbe    loc_2_2349
        lea    ecx, [ebp - 0x2c]
        lea    edi, [esi + 4]
        mov    dword ptr [ebp - 8], ecx
loc_2_227B:
        lea    eax, [ebp - 0xc]
        lea    ecx, [ebp - 0x10]
        push   eax
        push   ecx
        push   edi
        mov    al, byte ptr [0]              ; -> obj3:0x00E0
        mov    edx, eax
        mov    al, byte ptr [0]              ; -> obj3:0x00DC
        mov    ecx, eax
        call   sub_2_4DD7
        test   byte ptr [edi + 0x1c], 1
        je     loc_2_22C4
        mov    eax, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [ecx], eax
        jne    loc_2_22C4
        push   ebx
        mov    al, byte ptr [0]              ; -> obj3:0x00E0
        push   -1
        mov    edx, eax
        push   edi
        mov    al, byte ptr [0]              ; -> obj3:0x00DC
        mov    ecx, eax
        mov    dword ptr [ebp - 0x10], ebx
        mov    dword ptr [ebp - 0xc], ebx
        call   sub_2_4E43
loc_2_22C4:
        cmp    dword ptr [ebp - 0x10], ebx
        je     loc_2_22F4
        cmp    dword ptr [ebp - 0xc], ebx
        je     loc_2_22F4
        mov    eax, dword ptr [ebp - 0x10]
        cmp    dword ptr [ebp - 0xc], eax
        jb     loc_2_22F4
        sub    dword ptr [ebp - 0xc], eax
        mov    eax, 1
        mov    cl, byte ptr [edi + 0x28]
        shl    eax, cl
        mov    ecx, dword ptr [ebp - 8]
        add    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [ebp - 0x10]
        mov    edx, dword ptr [ebp - 0xc]
        mov    dword ptr [ecx + 4], eax
        mov    dword ptr [edi], edx
loc_2_22F4:
        add    dword ptr [ebp - 8], 4
        movzx  eax, byte ptr [esi]
        add    edi, 0x2c
        inc    dword ptr [ebp - 0x14]
        cmp    eax, dword ptr [ebp - 0x14]
        ja     loc_2_227B
        jmp    loc_2_2349
loc_2_230C:
        mov    al, byte ptr [esi]
        test   al, al
        je     loc_2_2334
        movzx  edi, al
        lea    ebx, [esi + 4]
loc_2_2318:
        mov    eax, dword ptr [ebx + 0x10]
        test   eax, eax
        je     loc_2_232E
        push   0
        push   eax
        push   -1
        push   0
        call   sub_2_7928
        add    esp, 0x10
loc_2_232E:
        add    ebx, 0x2c
        dec    edi
        jne    loc_2_2318
loc_2_2334:
        mov    al, byte ptr [0]              ; -> obj3:0x00E0
        mov    cl, byte ptr [0]              ; -> obj3:0x00DC
        push   eax
        push   ecx
        call   sub_2_5D8F
        add    esp, 8
loc_2_2349:
        push   4
        mov    al, byte ptr [0]              ; -> obj3:0x00E0
        mov    edx, eax
        mov    bl, byte ptr [0]              ; -> obj3:0x00DC
        mov    ecx, ebx
        call   sub_2_58D3
        mov    word ptr [ebp - 6], ax
        mov    al, byte ptr [esi]
        test   al, al
        je     loc_2_240B
        lea    ebx, [ebp - 0x28]
        lea    edi, [esi + 4]
        movzx  eax, al
        mov    dword ptr [ebp - 0x14], eax
loc_2_2379:
        mov    eax, dword ptr [edi + 0x20]
        cmp    eax, 1
        je     loc_2_2388
        cmp    eax, 2
        je     loc_2_23C8
        jmp    loc_2_23FC
loc_2_2388:
        mov    ecx, dword ptr [ebx]
        test   ecx, ecx
        je     loc_2_23A0
        test   byte ptr [ebp - 6], 2
        je     loc_2_23A0
        mov    dword ptr [0], 0              ; -> obj3:0x00F4
        jmp    loc_2_23AA
loc_2_23A0:
        mov    dword ptr [0], 1              ; -> obj3:0x00F4
loc_2_23AA:
        push   -1
        mov    eax, dword ptr [edi + 0x1c]
        push   0
        and    eax, 1
        push   eax
        mov    dl, byte ptr [esi + 1]
        push   edx
        mov    eax, dword ptr [edi + 0x18]
        not    eax
        push   eax
        mov    edx, dword ptr [edi]
        call   sub_2_1889
        jmp    loc_2_23FC
loc_2_23C8:
        mov    ecx, dword ptr [ebx]
        test   ecx, ecx
        je     loc_2_23E0
        test   byte ptr [ebp - 6], 1
        je     loc_2_23E0
        mov    dword ptr [0], 0              ; -> obj3:0x00F4
        jmp    loc_2_23EA
loc_2_23E0:
        mov    dword ptr [0], 1              ; -> obj3:0x00F4
loc_2_23EA:
        mov    al, byte ptr [esi + 1]
        mov    edx, dword ptr [edi]
        push   eax
        mov    eax, dword ptr [edi + 0x18]
        not    ax
        push   eax
        call   sub_2_17FE
loc_2_23FC:
        add    ebx, 4
        add    edi, 0x2c
        dec    dword ptr [ebp - 0x14]
        jne    loc_2_2379
loc_2_240B:
        xor    ebx, ebx
        push   1
        lea    edx, [ebp - 1]
        mov    ecx, 0x43
        mov    dword ptr [0], ebx            ; -> obj3:0x00F4
        call   sub_2_1694
        mov    cl, byte ptr [esi + 2]
        call   sub_2_1D28
        call   sub_2_1F61
        cmp    byte ptr [esi], bl
        je     loc_2_2474
        push   ebx
        mov    eax, dword ptr [0]            ; -> obj3:0x00D8
        push   eax
        lea    ecx, [ebp - 0x18]
        push   ebx
        push   ecx
        call   sub_2_7970
        add    esp, 0x10
        test   byte ptr [ebp - 0x17], 4
        jne    loc_2_2474
        cmp    byte ptr [esi], bl
        jbe    loc_2_2469
        lea    eax, [esi + 4]
        mov    ecx, 0xffffffff
loc_2_2459:
        inc    ebx
        mov    dword ptr [eax], ecx
        mov    dword ptr [eax + 8], ecx
        add    eax, 0x2c
        movzx  edx, byte ptr [esi]
        cmp    edx, ebx
        ja     loc_2_2459
loc_2_2469:
        mov    ecx, dword ptr [0]            ; -> obj3:0x00D8
        call   sub_2_7403
loc_2_2474:
        cmp    byte ptr [0], 1               ; -> obj3:0x00E8
        jne    loc_2_248E
        mov    edx, dword ptr [0]            ; -> obj3:0x00D4
loc_2_2483:
        mov    ecx, dword ptr [0]            ; -> obj3:0x00D8
        call   sub_2_7005
loc_2_248E:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_2495:
        cmp    word ptr [0], 0x605           ; -> obj3:0x00E4
        jne    loc_2_24D4
        test   byte ptr [0], 2               ; -> obj3:0x00F0
        jne    loc_2_24B3
        mov    dword ptr [0], 0              ; -> obj3:0x0034
loc_2_24B3:
        cmp    dword ptr [0], 0x12218086     ; -> obj3:0x00EC
        jne    loc_2_24D4
        push   0
        mov    dl, byte ptr [0]              ; -> obj3:0x00E0
        push   0x50
        mov    cl, byte ptr [0]              ; -> obj3:0x00DC
        call   sub_2_58EF
loc_2_24D4:
        ret    
sub_2_24D5:
        mov    ax, word ptr [0]              ; -> obj3:0x00E4
        push   esi
        cmp    ax, 1
        je     loc_2_24F4
        cmp    ah, 3
        je     loc_2_24F4
        test   byte ptr [1], 2               ; -> obj3:0x00F1
        je     loc_2_25D4
loc_2_24F4:
        xor    eax, eax
        cmp    dword ptr [0], eax            ; -> obj3:0x0034
        je     loc_2_250C
        test   byte ptr [0], 1               ; -> obj3:0x00F0
        je     loc_2_250C
        mov    dword ptr [0], eax            ; -> obj3:0x0034
loc_2_250C:
        push   4
        mov    dl, byte ptr [0]              ; -> obj3:0x00E0
        mov    cl, byte ptr [0]              ; -> obj3:0x00DC
        call   sub_2_58D3
        and    ax, 3
        sub    ax, 3
        cmp    ax, 1
        sbb    esi, esi
        neg    esi
        mov    ax, word ptr [0]              ; -> obj3:0x00E4
        cmp    ax, 1
        je     loc_2_254C
        cmp    ax, 0x300
        je     loc_2_254C
        cmp    ax, 0x301
        mov    ecx, 0
        jne    loc_2_2551
loc_2_254C:
        mov    ecx, 1
loc_2_2551:
        mov    eax, dword ptr [0]            ; -> obj3:0x00F0
        test   ah, 2
        je     loc_2_2563
        mov    ecx, eax
        and    ecx, 0x8000
loc_2_2563:
        test   ecx, ecx
        je     loc_2_25BB
        test   esi, esi
        je     loc_2_25AD
        push   1
        mov    edx, 0xaffff
        mov    ecx, 0xa0000
        call   sub_2_1BB1
        push   1
        mov    edx, 0xbffff
        mov    ecx, 0xb0000
        call   sub_2_1BB1
        mov    edx, 0x3bb
        mov    ecx, 0x3b0
        call   sub_2_1C65
        mov    edx, 0x3df
        mov    ecx, 0x3c0
        call   sub_2_1C65
        jmp    loc_2_25BB
loc_2_25AD:
        test   al, 2
        jne    loc_2_25BB
        mov    dword ptr [0], 0              ; -> obj3:0x0034
loc_2_25BB:
        test   esi, esi
        jne    loc_2_25D4
        mov    al, byte ptr [0]              ; -> obj3:0x00E0
        mov    cl, byte ptr [0]              ; -> obj3:0x00DC
        push   eax
        push   ecx
        call   sub_2_5D8F
        add    esp, 8
loc_2_25D4:
        pop    esi
        ret    
sub_2_25D6:
        push   ebx
        push   esi
        mov    esi, 0xffffffff
        mov    bl, 0xc0
loc_2_25DF:
        push   0
        mov    edx, ebx
        xor    ecx, ecx
        call   sub_2_58E1
        cmp    eax, 0x84c48086
        je     loc_2_25FB
        add    bl, 8
        cmp    bl, 0xe0
        jb     loc_2_25DF
        jmp    loc_2_2609
loc_2_25FB:
        push   8
        mov    edx, ebx
        xor    ecx, ecx
        call   sub_2_58C5
        movzx  esi, al
loc_2_2609:
        mov    eax, 2
        cmp    eax, esi
        pop    esi
        sbb    eax, eax
        pop    ebx
        inc    eax
        ret    
sub_2_2616:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        mov    word ptr [ebp - 4], 4
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        cmp    word ptr [0], 0x101           ; -> obj3:0x00E4
        je     loc_2_2642
        test   byte ptr [1], 1               ; -> obj3:0x00F1
        jne    loc_2_2642
        xor    eax, eax
        jmp    loc_2_2AC3
loc_2_2642:
        push   9
        mov    dl, byte ptr [0]              ; -> obj3:0x00E0
        mov    cl, byte ptr [0]              ; -> obj3:0x00DC
        call   sub_2_58C5
        mov    ebx, dword ptr [0]            ; -> obj3:0x00D4
        mov    byte ptr [ebp - 1], al
        cmp    dword ptr [0], 0x6401095      ; -> obj3:0x00EC
        mov    edi, ebx
        je     loc_2_2678
        cmp    dword ptr [0], 0x6461095      ; -> obj3:0x00EC
        jne    loc_2_26C0
loc_2_2678:
        mov    al, byte ptr [0]              ; -> obj3:0x00E0
        mov    bl, byte ptr [0]              ; -> obj3:0x00DC
        mov    edx, eax
        mov    ecx, ebx
        call   sub_2_57DE
        mov    ebx, eax
        mov    dl, byte ptr [0]              ; -> obj3:0x00E0
        mov    cl, byte ptr [0]              ; -> obj3:0x00DC
        call   sub_2_5819
        cmp    dword ptr [0], 0              ; -> obj3:0x00D4
        mov    edi, eax
        jne    loc_2_26C0
        test   ebx, ebx
        jne    loc_2_26B6
        test   edi, edi
        je     loc_2_2AAE
loc_2_26B6:
        mov    dword ptr [0], 1              ; -> obj3:0x00D4
loc_2_26C0:
        cmp    dword ptr [0], 0x12228086     ; -> obj3:0x00EC
        jne    loc_2_2724
        cmp    dword ptr [0], 0              ; -> obj3:0x00D4
        mov    word ptr [ebp - 4], 8
        mov    esi, 1
        je     loc_2_2724
        push   0xfe
        mov    al, byte ptr [0]              ; -> obj3:0x00E0
        push   0x4c
        mov    edx, eax
        mov    al, byte ptr [0]              ; -> obj3:0x00DC
        mov    ecx, eax
        call   sub_2_58EF
        push   3
        mov    dl, byte ptr [0]              ; -> obj3:0x00E0
        push   0x4d
        mov    cl, byte ptr [0]              ; -> obj3:0x00DC
        call   sub_2_58EF
        push   0
        mov    dl, byte ptr [0]              ; -> obj3:0x00E0
        push   0x50
        mov    cl, byte ptr [0]              ; -> obj3:0x00DC
        call   sub_2_58EF
loc_2_2724:
        cmp    dword ptr [0], 0x12308086     ; -> obj3:0x00EC
        jne    loc_2_275C
        call   sub_2_25D6
        test   eax, eax
        je     loc_2_275C
        push   0
        lea    ecx, [ebp - 5]
        push   1
        mov    eax, dword ptr [0]            ; -> obj3:0x00D8
        mov    byte ptr [ebp - 5], 1
        push   ecx
        push   3
        push   0                             ; -> obj3:0x0290
        push   0
        push   eax
        call   sub_2_79E0
        add    esp, 0x1c
loc_2_275C:
        cmp    dword ptr [0], 0x6401095      ; -> obj3:0x00EC
        je     loc_2_27B5
        cmp    dword ptr [0], 0x6461095      ; -> obj3:0x00EC
        je     loc_2_27B5
        push   0x18
        mov    al, byte ptr [0]              ; -> obj3:0x00E0
        mov    edx, eax
        mov    al, byte ptr [0]              ; -> obj3:0x00DC
        mov    ecx, eax
        call   sub_2_58E1
        and    al, 0xf8
        cmp    eax, 0x170
        jne    loc_2_27B5
        push   0x1c
        mov    al, byte ptr [0]              ; -> obj3:0x00E0
        mov    edx, eax
        mov    al, byte ptr [0]              ; -> obj3:0x00DC
        mov    ecx, eax
        call   sub_2_58E1
        and    al, 0xf0
        cmp    eax, 0x370
        jne    loc_2_27B5
        mov    esi, 1
loc_2_27B5:
        cmp    dword ptr [0], 0xc6211045     ; -> obj3:0x00EC
        jne    loc_2_2801
        push   9
        mov    al, byte ptr [0]              ; -> obj3:0x00E0
        mov    edx, eax
        mov    al, byte ptr [0]              ; -> obj3:0x00DC
        mov    ecx, eax
        call   sub_2_58C5
        mov    byte ptr [ebp - 5], 1
        test   al, 0x80
        jne    loc_2_27E2
        mov    byte ptr [ebp - 5], 0
loc_2_27E2:
        push   0
        lea    eax, [ebp - 5]
        push   1
        push   eax
        push   3
        push   0                             ; -> obj3:0x0298
        push   0
        push   dword ptr [0]                 ; -> obj3:0x00D8
        call   sub_2_79E0
        add    esp, 0x1c
loc_2_2801:
        push   0
        lea    eax, [ebp - 0xc]
        push   eax
        lea    ecx, [ebp - 8]
        mov    dword ptr [ebp - 8], 0
        mov    dword ptr [ebp - 0xc], 4
        push   ecx
        push   3
        push   0                             ; -> obj3:0x02A8
        push   0
        push   dword ptr [0]                 ; -> obj3:0x00D8
        call   sub_2_79D8
        add    esp, 0x1c
        test   byte ptr [ebp - 1], 0xc
        jne    loc_2_2845
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_2845
        test   byte ptr [0], 4               ; -> obj3:0x00F0
        je     loc_2_2883
loc_2_2845:
        mov    eax, dword ptr [0]            ; -> obj3:0x00EC
        cmp    eax, 0x6401095
        je     loc_2_287A
        cmp    eax, 0x6461095
        je     loc_2_287A
        mov    al, byte ptr [0]              ; -> obj3:0x00E0
        mov    bl, byte ptr [0]              ; -> obj3:0x00DC
        mov    edx, eax
        mov    ecx, ebx
        call   sub_2_2C70
        mov    ebx, eax
        and    eax, 2
        shr    eax, 1
        and    ebx, 1
        mov    esi, eax
        jmp    loc_2_2883
loc_2_287A:
        test   edi, edi
        je     loc_2_2883
        mov    esi, 1
loc_2_2883:
        test   esi, esi
        jne    loc_2_2889
        xor    edi, edi
loc_2_2889:
        test   ebx, ebx
        je     loc_2_28D3
        test   byte ptr [ebp - 1], 1
        je     loc_2_28D3
        push   0x10
        mov    al, byte ptr [0]              ; -> obj3:0x00E0
        mov    edx, eax
        mov    al, byte ptr [0]              ; -> obj3:0x00DC
        mov    ecx, eax
        call   sub_2_58E1
        and    al, 0xf8
        cmp    eax, 0x1f0
        jne    loc_2_28D3
        push   0x14
        mov    al, byte ptr [0]              ; -> obj3:0x00E0
        mov    edx, eax
        mov    al, byte ptr [0]              ; -> obj3:0x00DC
        mov    ecx, eax
        call   sub_2_58E1
        and    al, 0xf8
        cmp    eax, 0x3f0
        jne    loc_2_28D3
        and    byte ptr [ebp - 1], 0xfe
loc_2_28D3:
        test   edi, edi
        je     loc_2_291B
        test   byte ptr [ebp - 1], 4
        je     loc_2_291B
        push   0x18
        mov    al, byte ptr [0]              ; -> obj3:0x00E0
        mov    edx, eax
        mov    al, byte ptr [0]              ; -> obj3:0x00DC
        mov    ecx, eax
        call   sub_2_58E1
        and    al, 0xf8
        cmp    eax, 0x170
        jne    loc_2_291B
        push   0x1c
        mov    dl, byte ptr [0]              ; -> obj3:0x00E0
        mov    cl, byte ptr [0]              ; -> obj3:0x00DC
        call   sub_2_58E1
        and    al, 0xf8
        cmp    eax, 0x370
        jne    loc_2_291B
        and    byte ptr [ebp - 1], 0xfb
loc_2_291B:
        push   0
        lea    eax, [ebp - 0xc]
        push   eax
        lea    ecx, [ebp - 5]
        mov    dword ptr [ebp - 0xc], 1
        push   ecx
        push   3
        push   0                             ; -> obj3:0x02B4
        push   0
        push   dword ptr [0]                 ; -> obj3:0x00D8
        call   sub_2_79D8
        add    esp, 0x1c
        test   eax, eax
        jne    loc_2_295B
        xor    ebx, ebx
        mov    al, byte ptr [ebp - 5]
        mov    bl, byte ptr [ebp - 5]
        and    al, 2
        and    ebx, 1
        cmp    al, 1
        sbb    edi, edi
        inc    edi
        mov    esi, edi
loc_2_295B:
        test   ebx, ebx
        je     loc_2_299F
        test   byte ptr [ebp - 1], 1
        je     loc_2_2974
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, 0x10
        call   sub_2_1B16
        jmp    loc_2_29A4
loc_2_2974:
        mov    edx, 0x1f7
        mov    ecx, 0x1f0
        call   sub_2_1C65
        mov    ecx, 0x3f6
        mov    edx, ecx
        call   sub_2_1C65
        push   0xe
        mov    edx, 0x4000
        xor    ecx, ecx
        call   sub_2_17BD
        jmp    loc_2_29A4
loc_2_299F:
        call   sub_2_1AFD
loc_2_29A4:
        test   esi, esi
        je     loc_2_29EC
        test   edi, edi
        je     loc_2_29EC
        test   byte ptr [ebp - 1], 4
        je     loc_2_29C1
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, 0x18
        call   sub_2_1B16
        jmp    loc_2_29F1
loc_2_29C1:
        mov    edx, 0x177
        mov    ecx, 0x170
        call   sub_2_1C65
        mov    ecx, 0x376
        mov    edx, ecx
        call   sub_2_1C65
        push   0xf
        mov    edx, 0x8000
        xor    ecx, ecx
        call   sub_2_17BD
        jmp    loc_2_29F1
loc_2_29EC:
        call   sub_2_1AFD
loc_2_29F1:
        test   byte ptr [ebp - 1], 0x80
        je     loc_2_2AA6
        push   0x20
        mov    dl, byte ptr [0]              ; -> obj3:0x00E0
        mov    cl, byte ptr [0]              ; -> obj3:0x00DC
        call   sub_2_58E1
        mov    byte ptr [ebp - 1], 0
        mov    esi, eax
        and    si, 0xfffc
        cmp    byte ptr [0], 0x20            ; -> obj3:0x00C0
        je     loc_2_2A3D
loc_2_2A22:
        mov    al, byte ptr [ebp - 1]
        cmp    al, byte ptr [0]              ; -> obj3:0x00C8
        jae    loc_2_2A3D
        inc    byte ptr [ebp - 1]
        movzx  eax, byte ptr [ebp - 1]
        cmp    byte ptr [eax], 0x20          ; -> obj3:0x00C0
        jne    loc_2_2A22
loc_2_2A3D:
        test   si, si
        je     loc_2_2AA6
        test   ebx, ebx
        je     loc_2_2A5E
        push   0xff
        mov    edx, 8
        push   0xffff
        mov    ecx, esi
        call   sub_2_17FE
        jmp    loc_2_2A6C
loc_2_2A5E:
        push   0
        xor    edx, edx
        mov    ecx, 0x8000
        call   sub_2_1694
loc_2_2A6C:
        test   edi, edi
        je     loc_2_2A98
        mov    cx, si
        push   0xff
        movzx  eax, byte ptr [ebp - 1]
        add    cx, 8
        push   0xffff
        mov    dx, word ptr [eax*2]          ; -> obj3:0x00B4
        sub    dx, 8
        call   sub_2_17FE
        jmp    loc_2_2AA6
loc_2_2A98:
        push   0
        xor    edx, edx
        mov    ecx, 0x8000
        call   sub_2_1694
loc_2_2AA6:
        test   ebx, ebx
        jne    loc_2_2ABE
        test   edi, edi
        jne    loc_2_2ABE
loc_2_2AAE:
        push   2
        mov    eax, dword ptr [0]            ; -> obj3:0x00D8
        push   eax
        call   sub_2_79B8
        add    esp, 8
loc_2_2ABE:
        mov    eax, 1
loc_2_2AC3:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_2ACA:
        push   ebp
        mov    edx, dword ptr [esp + 0xc]
        mov    ebp, esp
        push   ebx
        push   0
        mov    bl, byte ptr [ebp + 8]
        mov    ecx, ebx
        call   sub_2_58E1
        mov    ecx, dword ptr [ebp + 0x14]
        push   1
        mov    edx, dword ptr [ebp + 0xc]
        mov    dword ptr [0], eax            ; -> obj3:0x00EC
        mov    al, byte ptr [ebp + 0xc]
        mov    byte ptr [0], bl              ; -> obj3:0x00DC
        mov    byte ptr [0], al              ; -> obj3:0x00E0
        mov    dword ptr [0], ecx            ; -> obj3:0x00F0
        mov    ecx, ebx
        call   sub_2_40BC
        test   byte ptr [0], 0x80            ; -> obj3:0x00F0
        mov    dword ptr [0], eax            ; -> obj3:0x00D4
        je     loc_2_2B1E
        mov    dword ptr [0], 1              ; -> obj3:0x00D4
loc_2_2B1E:
        push   0xa
        mov    dl, byte ptr [0]              ; -> obj3:0x00E0
        mov    cl, byte ptr [0]              ; -> obj3:0x00DC
        call   sub_2_58D3
        mov    word ptr [0], ax              ; -> obj3:0x00E4
        mov    dl, byte ptr [0]              ; -> obj3:0x00E0
        mov    cl, byte ptr [0]              ; -> obj3:0x00DC
        call   sub_2_406B
        mov    byte ptr [0], al              ; -> obj3:0x00E8
        mov    ax, word ptr [0]              ; -> obj3:0x00E4
        cmp    ah, 6
        jne    loc_2_2B70
        movzx  eax, ax
        cmp    eax, 0x604
        je     loc_2_2BC1
        cmp    eax, 0x605
        je     loc_2_2BDB
        cmp    eax, 0x607
        je     loc_2_2BEE
loc_2_2B70:
        cmp    byte ptr [0], 0               ; -> obj3:0x00E8
        jne    loc_2_2C6D
        cmp    dword ptr [0], 0x4848086      ; -> obj3:0x00EC
        jne    loc_2_2C1F
        test   byte ptr [0], 8               ; -> obj3:0x0CB8
        jne    loc_2_2C1F
        push   0x41
        mov    al, byte ptr [0]              ; -> obj3:0x00E0
        mov    edx, eax
        mov    bl, byte ptr [0]              ; -> obj3:0x00DC
        mov    ecx, ebx
        call   sub_2_58C5
        test   byte ptr [0], 0x10            ; -> obj3:0x0CB8
        je     loc_2_2C01
        test   al, 1
        jne    loc_2_2C1F
        or     al, 1
        jmp    loc_2_2C07
loc_2_2BC1:
        cmp    byte ptr [0], 1               ; -> obj3:0x00E8
        jne    loc_2_2C6D
        mov    ecx, dword ptr [ebp + 0x10]
        call   sub_2_20E7
        jmp    loc_2_2C6D
loc_2_2BDB:
        cmp    byte ptr [0], 2               ; -> obj3:0x00E8
        jne    loc_2_2B70
        mov    ecx, dword ptr [ebp + 0x10]
        call   sub_2_20E7
        jmp    loc_2_2C6D
loc_2_2BEE:
        cmp    byte ptr [0], 2               ; -> obj3:0x00E8
        jne    loc_2_2C6D
        mov    ecx, dword ptr [ebp + 0x10]
        call   sub_2_20E7
        jmp    loc_2_2C6D
loc_2_2C01:
        test   al, 1
        je     loc_2_2C1F
        and    al, 0xfe
loc_2_2C07:
        push   eax
        mov    cl, byte ptr [0]              ; -> obj3:0x00E0
        push   0x41
        mov    edx, ecx
        mov    bl, byte ptr [0]              ; -> obj3:0x00DC
        mov    ecx, ebx
        call   sub_2_58EF
loc_2_2C1F:
        push   0
        mov    cl, byte ptr [0]              ; -> obj3:0x00E0
        mov    edx, ecx
        mov    bl, byte ptr [0]              ; -> obj3:0x00DC
        push   dword ptr [ebp + 0x10]
        mov    ecx, ebx
        call   sub_2_4451
        mov    dword ptr [0], eax            ; -> obj3:0x00D8
        test   eax, eax
        je     loc_2_2C6D
        test   byte ptr [0], 0x10            ; -> obj3:0x00F0
        jne    loc_2_2C6D
        mov    ecx, 6
        call   sub_2_1D28
        call   sub_2_2495
        call   sub_2_24D5
        call   sub_2_2616
        test   eax, eax
        jne    loc_2_2C6D
        call   sub_2_1F61
loc_2_2C6D:
        pop    ebx
        pop    ebp
        ret    
sub_2_2C70:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xa4
        push   ebx
        mov    byte ptr [ebp - 4], cl
        mov    byte ptr [ebp - 0x9f], dl
        push   esi
        push   edi
        mov    bl, dl
        lea    eax, [ebp - 0x14]
        mov    byte ptr [ebp - 0xa0], cl
        mov    dword ptr [ebp - 0x9a], 0     ; -> obj3:0x02C4
        push   eax
        push   0                             ; -> obj3:0x0320
        push   dword ptr [0]                 ; -> obj3:0x05E0
        call   sub_2_79F0
        add    esp, 0xc
        test   eax, eax
        je     loc_2_2CBC
        mov    eax, 3
        jmp    loc_2_2D6E
loc_2_2CBC:
        push   0
        mov    edx, ebx
        mov    ecx, dword ptr [ebp - 4]
        call   sub_2_58E1
        push   eax
        lea    ecx, [ebp - 0xc]
        push   0                             ; -> obj3:0x0330
        push   ecx
        call   sub_2_79F8
        add    esp, 0xc
        lea    ecx, [ebp - 0x10]
        lea    edx, [ebp - 0xc]
        push   ecx
        push   edx
        push   dword ptr [ebp - 0x14]
        call   sub_2_79F0
        add    esp, 0xc
        mov    esi, eax
        push   dword ptr [ebp - 0x14]
        call   sub_2_79E8
        add    esp, 4
        test   esi, esi
        je     loc_2_2D05
        mov    eax, 3
        jmp    loc_2_2D6E
loc_2_2D05:
        lea    edx, [ebp - 0xa4]
        mov    ecx, dword ptr [ebp - 0x10]
        call   sub_2_3F18
        push   dword ptr [ebp - 0x10]
        mov    esi, eax
        call   sub_2_79E8
        add    esp, 4
        cmp    esi, 0x20
        je     loc_2_2D2C
        mov    eax, 3
        jmp    loc_2_2D6E
loc_2_2D2C:
        lea    ecx, [ebp - 0xa4]
        call   sub_2_3F7D
        cmp    eax, 0x20
        je     loc_2_2D43
        mov    eax, 3
        jmp    loc_2_2D6E
loc_2_2D43:
        mov    edi, 3
        lea    eax, [ebp - 7]
        push   eax
        mov    dword ptr [ebp - 7], edi
        call   sub_3_02D8
        add    esp, 4
        mov    esi, eax
        lea    ecx, [ebp - 0xa4]
        call   sub_2_402B
        mov    eax, edi
        test   esi, esi
        jne    loc_2_2D6E
        mov    eax, dword ptr [ebp - 7]
loc_2_2D6E:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_2D75:
        mov    eax, dword ptr [edx]
        mov    eax, dword ptr [eax + 0xc]
        mov    dword ptr [ecx], eax
        test   eax, eax
        jne    loc_2_2D84
        xor    eax, eax
        jmp    loc_2_2D9B
loc_2_2D84:
        push   0
        push   0
        push   4
        push   eax
        push   ecx
        call   sub_2_7908
        add    esp, 0x14
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
loc_2_2D9B:
        ret    
loc_2_2D9C:
        push   ebp
        mov    edx, dword ptr [esp + 0xc]
        mov    ebp, esp
        sub    esp, 8
        mov    dword ptr [ebp - 4], 0
        push   ebx
        push   esi
        push   edi
        mov    bl, byte ptr [edx + 1]
        test   bl, bl
        jne    loc_2_2DC1
        mov    eax, 0x13
        jmp    loc_2_2EB7
loc_2_2DC1:
        mov    eax, dword ptr [ebp + 8]
        test   eax, eax
        je     loc_2_2DF5
        cmp    eax, 1
        je     loc_2_2E18
        cmp    eax, 4
        je     loc_2_2E41
        cmp    eax, 5
        je     loc_2_2E88
        cmp    eax, 6
        je     loc_2_2E18
        cmp    eax, 7
        je     loc_2_2E9E
        mov    dword ptr [ebp - 4], 1
        jmp    loc_2_2EB4
loc_2_2DF5:
        lea    ecx, [ebp - 8]
        mov    edx, dword ptr [ebp + 0x14]
        call   sub_2_2D75
        test   eax, eax
        je     loc_2_2EB4
        mov    ecx, dword ptr [ebp - 8]
        movzx  ax, bl
        mov    word ptr [ecx + 2], ax
        jmp    loc_2_2EB4
loc_2_2E18:
        lea    ecx, [ebp - 8]
        mov    edx, dword ptr [ebp + 0x14]
        call   sub_2_2D75
        test   eax, eax
        je     loc_2_2EB4
        mov    ecx, dword ptr [ebp - 8]
        movzx  ax, bl
        cmp    word ptr [ecx + 2], ax
        je     loc_2_2EB4
        mov    dword ptr [ebp - 4], 0x13
        jmp    loc_2_2EB4
loc_2_2E41:
        mov    si, 1
        mov    eax, dword ptr [ebp + 0x14]
        mov    cl, bl
        mov    edi, 4                        ; -> obj3:0x033C
        mov    dword ptr [eax + 4], 0x18
        shl    si, cl
        mov    dword ptr [eax], 0            ; -> obj3:0x0338
        xor    eax, eax
        mov    word ptr [0], si              ; -> obj3:0x0338
        mov    word ptr [2], si              ; -> obj3:0x033A
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        movzx  eax, bl
        mov    cl, byte ptr [edx + 8]
        mov    byte ptr [eax + 4], cl        ; -> obj3:0x033C
        mov    word ptr [0x14], si           ; -> obj3:0x034C
        jmp    loc_2_2EB4
loc_2_2E88:
        mov    eax, dword ptr [edx + 8]
        test   eax, eax
        je     loc_2_2EB4
        dec    eax
        mov    dword ptr [edx + 8], eax
        jne    loc_2_2EB4
        mov    ecx, edx
        call   sub_2_36EA
        jmp    loc_2_2EB4
loc_2_2E9E:
        mov    eax, dword ptr [ebp + 0x14]
        mov    ax, word ptr [eax]
        and    ax, 2
        cmp    ax, 1
        sbb    eax, eax
        and    eax, 0x13
        mov    dword ptr [ebp - 4], eax
loc_2_2EB4:
        mov    eax, dword ptr [ebp - 4]
loc_2_2EB7:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_2EBE:
        push   esi
        mov    eax, 0
        push   edi
        mov    esi, ecx
        cmp    dword ptr [esp + 0xc], 1
        adc    eax, -1
        mov    edi, edx
        and    eax, 0x18
        or     al, 2
        push   eax
        push   esi
        push   edi
        push   0                             ; -> loc_2_2D9C
        push   4
        push   0
        call   sub_2_79C0
        add    esp, 0x18
        test   eax, eax
        jne    loc_2_2EF1
        inc    dword ptr [edi + 8]
loc_2_2EF1:
        push   0
        push   esi
        push   dword ptr [edi + 4]
        call   sub_2_7A00
        add    esp, 0xc
        pop    edi
        pop    esi
        ret    4
sub_2_2F04:
        push   ebx
        xor    eax, eax
        push   esi
        cmp    dword ptr [0], eax            ; -> loc_3_036C
        push   edi
        mov    edi, dword ptr [0]            ; -> loc_3_0368
        jbe    loc_2_2F37
loc_2_2F17:
        cmp    byte ptr [edi], cl
        jne    loc_2_2F2B
        mov    bl, byte ptr [edi + 1]
        xor    bl, dl
        movzx  esi, bl
        test   esi, 0xfffffff8
        je     loc_2_2F37
loc_2_2F2B:
        inc    eax
        add    edi, 0x10
        cmp    eax, dword ptr [0]            ; -> loc_3_036C
        jb     loc_2_2F17
loc_2_2F37:
        sub    eax, dword ptr [0]            ; -> loc_3_036C
        cmp    eax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_2_2F4E:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x1c
        push   ebx
        mov    byte ptr [ebp - 3], dl
        push   esi
        mov    dword ptr [ebp - 8], ecx
        push   edi
        mov    esi, 0                        ; -> obj3:0x04DC
        lea    edi, [ebp - 0x1c]
        push   0x3d
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsd  dword ptr es:[edi], dword ptr [esi]
        movsb  byte ptr es:[edi], byte ptr [esi]
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 3]
        call   sub_2_58C5
        mov    bl, al
        test   bl, bl
        jne    loc_2_2F84
        xor    eax, eax
        jmp    loc_2_30D8
loc_2_2F84:
        cmp    dword ptr [0], 0              ; -> loc_3_0368
        jne    loc_2_2F94
        xor    eax, eax
        jmp    loc_2_30D8
loc_2_2F94:
        dec    bl
        call   sub_2_44FB
        xor    edi, edi
loc_2_2F9D:
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 3]
        call   sub_2_2F04
        mov    esi, eax
        test   esi, esi
        jne    loc_2_30B2
        push   0
        lea    ecx, [ebp - 8]
        push   dword ptr [ebp - 8]
        push   ecx
        call   sub_2_7958
        add    esp, 0xc
        test   eax, eax
        mov    eax, dword ptr [0]            ; -> obj3:0x0DAC
        jne    loc_2_30B2
        cmp    dword ptr [ebp - 8], eax
        je     loc_2_30B2
        push   0
        lea    eax, [ebp - 1]
        push   0
        lea    edx, [ebp - 2]
        push   eax
        mov    ecx, dword ptr [ebp - 8]
        call   sub_2_4212
        mov    edx, dword ptr [ebp - 1]
        mov    ecx, dword ptr [ebp - 2]
        call   sub_2_4075
        test   eax, 0x20000
        jne    loc_2_309E
        mov    edx, dword ptr [ebp - 1]
        mov    ecx, dword ptr [ebp - 2]
        movzx  eax, byte ptr [ebp + 8]
        push   eax
        push   0
        call   sub_2_58E1
        push   eax
        lea    ecx, [ebp - 0x1c]
        push   0                             ; -> loc_3_04F0
        push   ecx
        call   sub_2_79F8
        add    esp, 0x10
        lea    ecx, [ebp - 0x1c]
        call   sub_2_4521
        test   eax, eax
        je     loc_2_3051
        lea    ecx, [ebx*8]
        shr    eax, cl
        and    eax, 0xff
        dec    eax
        cmp    eax, 3
        jbe    loc_2_304D
        mov    edi, 1
        jmp    loc_2_309E
loc_2_304D:
        mov    bl, al
        jmp    loc_2_309E
loc_2_3051:
        mov    edx, dword ptr [ebp - 1]
        mov    ecx, dword ptr [ebp - 2]
        call   sub_2_406B
        movzx  eax, al
        cmp    eax, 1
        je     loc_2_3070
        cmp    eax, 2
        je     loc_2_308E
        mov    edi, 1
        jmp    loc_2_309E
loc_2_3070:
        mov    al, byte ptr [ebp + 8]
        shr    al, 3
        movzx  ecx, al
        movzx  eax, bl
        add    eax, ecx
        cdq    
        xor    eax, edx
        sub    eax, edx
        and    eax, 3
        xor    eax, edx
        sub    eax, edx
        mov    bl, al
        jmp    loc_2_309E
loc_2_308E:
        push   0x3d
        mov    edx, dword ptr [ebp - 1]
        mov    ecx, dword ptr [ebp - 2]
        call   sub_2_58C5
        lea    ebx, [eax - 1]
loc_2_309E:
        mov    al, byte ptr [ebp - 2]
        mov    cl, byte ptr [ebp - 1]
        mov    byte ptr [ebp - 3], al
        mov    byte ptr [ebp + 8], cl
        test   edi, edi
        je     loc_2_2F9D
loc_2_30B2:
        call   sub_2_4592
        test   edi, edi
        je     loc_2_30BD
        xor    esi, esi
loc_2_30BD:
        test   esi, esi
        je     loc_2_30D6
        mov    eax, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 0x10]
        mov    dl, byte ptr [ebp - 3]
        mov    byte ptr [eax], bl
        mov    ebx, dword ptr [ebp + 0x14]
        mov    al, byte ptr [ebp + 8]
        mov    byte ptr [ecx], dl
        mov    byte ptr [ebx], al
loc_2_30D6:
        mov    eax, esi
loc_2_30D8:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
sub_2_30E1:
        push   esi
        xor    eax, eax
        mov    esi, dword ptr [0]            ; -> loc_3_0370
        mov    edx, dword ptr [0]            ; -> loc_3_0374
        cmp    eax, edx
        je     loc_2_3102
loc_2_30F4:
        cmp    byte ptr [esi], cl
        je     loc_2_3100
        inc    eax
        add    esi, 0xc
        cmp    edx, eax
        ja     loc_2_30F4
loc_2_3100:
        cmp    eax, edx
loc_2_3102:
        sbb    eax, eax
        and    eax, esi
        pop    esi
        ret    
sub_2_3108:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xdc
        mov    word ptr [ebp - 0x10], 3
        push   ebx
        xor    eax, eax
        mov    word ptr [ebp - 0xe], ax
        push   esi
        mov    word ptr [ebp - 0xa], ax
        push   edi
        mov    esi, ecx
        lea    edx, [ebp - 0xdc]
        movzx  ecx, byte ptr [esi]
        push   ecx
        push   0                             ; -> loc_3_0500
        push   edx
        call   sub_2_79F8
        add    esp, 0xc
        lea    edx, [ebp - 0xdc]
        lea    eax, [ebp - 4]
        push   1
        push   dword ptr [0]                 ; -> obj3:0x0DAC
        push   edx
        push   eax
        call   sub_2_7A18
        add    esp, 0x10
        test   eax, eax
        je     loc_2_3164
        xor    eax, eax
        jmp    loc_2_32B9
loc_2_3164:
        mov    eax, dword ptr [ebp - 4]
        xor    edi, edi
        push   edi
        xor    edx, edx
        push   0x10
        mov    ecx, eax
        push   edi
        mov    dword ptr [esi + 4], eax
        call   sub_2_426A
        push   edi
        lea    eax, [ebp - 8]
        push   0x2000
        push   dword ptr [ebp - 4]
        push   eax
        call   sub_2_79A8
        add    esp, 0x10
        test   eax, eax
        je     loc_2_3199
        xor    eax, eax
        jmp    loc_2_32B9
loc_2_3199:
        mov    di, word ptr [esi + 2]
        push   0
        mov    ax, di
        push   8
        and    ax, word ptr [0]              ; -> obj3:0x0350
        lea    ecx, [ebp - 0x10]
        mov    word ptr [ebp - 0xc], ax
        push   ecx
        push   4
        push   dword ptr [ebp - 8]
        push   0
        call   sub_2_79A0
        add    esp, 0x18
        test   eax, eax
        je     loc_2_31CD
        xor    eax, eax
        jmp    loc_2_32B9
loc_2_31CD:
        push   0
        lea    ecx, [ebp - 8]
        push   0x3000
        push   dword ptr [ebp - 4]
        push   ecx
        call   sub_2_79A8
        add    esp, 0x10
        test   eax, eax
        je     loc_2_31EE
        xor    eax, eax
        jmp    loc_2_32B9
loc_2_31EE:
        mov    word ptr [ebp - 0xc], di
        push   0
        push   8
        lea    eax, [ebp - 0x10]
        push   eax
        push   4
        push   dword ptr [ebp - 8]
        push   0
        call   sub_2_79A0
        add    esp, 0x18
        test   eax, eax
        je     loc_2_3214
        xor    eax, eax
        jmp    loc_2_32B9
loc_2_3214:
        xor    ebx, ebx
        lea    eax, [ebp - 0x14]
        push   eax
        mov    dword ptr [ebp - 0x14], ebx
        call   sub_3_0360
        add    esp, 4
        mov    al, byte ptr [esi]
        lea    ecx, [ebp - 0x15]
        push   eax
        push   ecx
        call   sub_3_0358
        add    esp, 8
        test   eax, eax
        jne    loc_2_3292
        cmp    byte ptr [ebp - 0x15], bl
        je     loc_2_3292
        mov    eax, 1
        mov    cl, byte ptr [ebp - 0x15]
        shl    eax, cl
        test   dword ptr [ebp - 0x14], eax
        je     loc_2_3292
        push   3
        lea    ecx, [ebp - 8]
        push   1
        push   dword ptr [ebp - 4]
        push   ecx
        call   sub_2_79A8
        add    esp, 0x10
        test   eax, eax
        je     loc_2_3269
        xor    eax, eax
        jmp    loc_2_32B9
loc_2_3269:
        push   0
        lea    ecx, [ebp - 0x10]
        movzx  ax, byte ptr [ebp - 0x15]
        mov    word ptr [ebp - 0xe], ax
        push   8
        push   ecx
        push   4
        push   dword ptr [ebp - 8]
        push   0
        call   sub_2_79A0
        add    esp, 0x18
        test   eax, eax
        mov    eax, 0
        jne    loc_2_32B9
loc_2_3292:
        push   0
        push   0
        push   dword ptr [ebp - 4]
        call   sub_2_7A10
        add    esp, 0xc
        push   7
        push   esi
        push   0                             ; -> loc_2_3351
        push   dword ptr [ebp - 4]
        call   sub_2_7A08
        add    esp, 0x10
        mov    eax, 1
loc_2_32B9:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_32C0:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        mov    ecx, edx
        mov    edi, edx
        push   0
        lea    eax, [ebp - 3]
        push   0
        lea    edx, [ebp - 2]
        push   eax
        call   sub_2_4212
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 5]
        push   eax
        lea    edx, [ebp - 1]
        push   ecx
        push   edx
        mov    edx, dword ptr [ebp - 2]
        mov    ecx, edi
        push   dword ptr [ebp - 3]
        call   sub_2_2F4E
        test   eax, eax
        je     loc_2_3348
        movzx  ecx, byte ptr [ebp - 1]
        imul   ecx, ecx, 3
        mov    cl, byte ptr [ecx + eax + 2]
        call   sub_2_30E1
        mov    ebx, eax
        test   ebx, ebx
        je     loc_2_3348
        cmp    dword ptr [ebx + 4], 0
        jne    loc_2_3332
        mov    ecx, ebx
        call   sub_2_3108
        test   eax, eax
        jne    loc_2_3332
        push   2
        push   esi
        call   sub_2_79B8
        add    esp, 8
        mov    eax, ebx
        jmp    loc_2_334A
loc_2_3332:
        sub    edi, esi
        mov    edx, ebx
        mov    ecx, esi
        cmp    edi, 1
        sbb    eax, eax
        inc    eax
        push   eax
        call   sub_2_2EBE
        mov    eax, ebx
        jmp    loc_2_334A
loc_2_3348:
        xor    eax, eax
loc_2_334A:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_3351:
        push   ebp
        mov    eax, 1
        mov    ebp, esp
        sub    esp, 0xd8
        push   ebx
        mov    ecx, dword ptr [ebp + 8]
        push   esi
        cmp    ecx, eax
        mov    esi, dword ptr [ebp + 0x14]
        je     loc_2_3377
        cmp    ecx, 2
        je     loc_2_33C0
        cmp    ecx, 4
        je     loc_2_33C0
        jmp    loc_2_33D3
loc_2_3377:
        push   0
        lea    ecx, [ebp - 0xd8]
        push   dword ptr [ebp + 0x10]
        push   ecx
        call   sub_2_7A20
        add    esp, 0xc
        test   eax, eax
        je     loc_2_3396
        mov    eax, 0x13
        jmp    loc_2_33D3
loc_2_3396:
        cmp    word ptr [ebp - 0x2a], 0
        jne    loc_2_33A4
        mov    eax, 0x13
        jmp    loc_2_33D3
loc_2_33A4:
        mov    bl, byte ptr [ebp - 0x28]
        mov    dl, byte ptr [esi]
        mov    ecx, ebx
        call   sub_2_4CEB
        test   eax, eax
        jne    loc_2_33BB
        mov    eax, 0x13
        jmp    loc_2_33D3
loc_2_33BB:
        mov    byte ptr [esi + 1], bl
        jmp    loc_2_33D1
loc_2_33C0:
        xor    ebx, ebx
        mov    dl, byte ptr [esi]
        xor    ecx, ecx
        mov    dword ptr [esi + 4], ebx
        mov    byte ptr [esi + 1], bl
        call   sub_2_4CEB
loc_2_33D1:
        xor    eax, eax
loc_2_33D3:
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_33D9:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xd4
        mov    word ptr [ebp - 0xc], 3
        push   ebx
        push   esi
        push   edi
        mov    bl, cl
        movzx  si, bl
        movzx  eax, si
        xor    edi, edi
        push   eax
        mov    word ptr [ebp - 0xa], di
        push   0                             ; -> obj3:0x0514
        mov    word ptr [ebp - 6], di
        lea    ecx, [ebp - 0xd4]
        push   ecx
        call   sub_2_79F8
        add    esp, 0xc
        lea    edx, [ebp - 0xd4]
        lea    eax, [ebp - 4]
        push   1
        push   dword ptr [0]                 ; -> obj3:0x0DAC
        push   edx
        push   eax
        call   sub_2_7A18
        add    esp, 0x10
        test   eax, eax
        je     loc_2_3442
        cmp    eax, 0x10
        je     loc_2_34ED
        xor    eax, eax
        jmp    loc_2_34F2
loc_2_3442:
        xor    edi, edi
        xor    edx, edx
        push   edi
        mov    ecx, dword ptr [ebp - 4]
        push   0x10
        push   edi
        call   sub_2_426A
        push   edi
        lea    ecx, [ebp - 0x10]
        push   0x3000
        push   dword ptr [ebp - 4]
        push   ecx
        call   sub_2_79A8
        add    esp, 0x10
        test   eax, eax
        je     loc_2_3472
        xor    eax, eax
        jmp    loc_2_34F2
loc_2_3472:
        mov    ax, 1
        mov    cl, bl
        shl    ax, cl
        mov    word ptr [ebp - 8], ax
        push   0
        push   8
        lea    ecx, [ebp - 0xc]
        push   ecx
        push   4
        push   dword ptr [ebp - 0x10]
        push   0
        call   sub_2_79A0
        add    esp, 0x18
        test   eax, eax
        je     loc_2_349E
        xor    eax, eax
        jmp    loc_2_34F2
loc_2_349E:
        push   3
        lea    ecx, [ebp - 0x10]
        push   1
        push   dword ptr [ebp - 4]
        push   ecx
        call   sub_2_79A8
        add    esp, 0x10
        test   eax, eax
        je     loc_2_34B9
        xor    eax, eax
        jmp    loc_2_34F2
loc_2_34B9:
        mov    word ptr [ebp - 0xa], si
        push   0
        push   8
        lea    eax, [ebp - 0xc]
        push   eax
        push   4
        push   dword ptr [ebp - 0x10]
        push   0
        call   sub_2_79A0
        add    esp, 0x18
        test   eax, eax
        je     loc_2_34DC
        xor    eax, eax
        jmp    loc_2_34F2
loc_2_34DC:
        push   7
        push   0
        push   0
        push   dword ptr [ebp - 4]
        call   sub_2_7A08
        add    esp, 0x10
loc_2_34ED:
        mov    eax, 1
loc_2_34F2:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_34F9:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x114
        xor    eax, eax
        mov    ecx, 0x3f
        mov    byte ptr [ebp - 0x114], 0
        push   ebx
        push   esi
        push   edi
        lea    edi, [ebp - 0x113]
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        stosb  byte ptr es:[edi], al
        cmp    dword ptr [0], eax            ; -> loc_3_0370
        jne    loc_2_368A
        xor    edx, edx
        mov    edi, dword ptr [0]            ; -> loc_3_0368
        mov    dword ptr [0], edx            ; -> loc_3_0374
        cmp    dword ptr [0], edx            ; -> loc_3_036C
        jbe    loc_2_357B
        mov    eax, dword ptr [0]            ; -> loc_3_036C
loc_2_3545:
        cmp    byte ptr [edi], 0
        jne    loc_2_3575
        lea    esi, [edi + 2]
        mov    ecx, 4
loc_2_3552:
        mov    bl, byte ptr [esi]
        test   bl, bl
        je     loc_2_356F
        movzx  ebx, bl
        lea    ebx, [ebp + ebx - 0x114]
        cmp    byte ptr [ebx], 0
        jne    loc_2_356D
        inc    dword ptr [0]                 ; -> loc_3_0374
loc_2_356D:
        inc    byte ptr [ebx]
loc_2_356F:
        add    esi, 3
        dec    ecx
        jne    loc_2_3552
loc_2_3575:
        add    edi, 0x10
        dec    eax
        jne    loc_2_3545
loc_2_357B:
        cmp    dword ptr [0], 0              ; -> loc_3_0374
        je     loc_2_368A
        push   0x201
        mov    eax, dword ptr [0]            ; -> loc_3_0374
        imul   eax, eax, 0xc
        push   eax
        call   sub_2_79C8
        add    esp, 8
        mov    dword ptr [0], eax            ; -> loc_3_0370
        test   eax, eax
        jne    loc_2_35B3
        call   sub_2_3D54
        xor    eax, eax
        jmp    loc_2_368F
loc_2_35B3:
        xor    edx, edx
        mov    dword ptr [0], eax            ; -> loc_3_0370
loc_2_35BA:
        cmp    byte ptr [ebp + edx - 0x114], 0
        je     loc_2_3623
        mov    si, 0xffff
        mov    edi, dword ptr [0]            ; -> loc_3_0368
        mov    byte ptr [eax], dl
        cmp    dword ptr [0], 0              ; -> loc_3_036C
        jbe    loc_2_3612
        mov    ecx, dword ptr [0]            ; -> loc_3_036C
        mov    dword ptr [ebp - 0xc], ecx
loc_2_35E2:
        cmp    byte ptr [edi], 0
        jne    loc_2_360A
        mov    dword ptr [ebp - 0x10], 4
        lea    ecx, [edi + 3]
loc_2_35F1:
        movzx  ebx, byte ptr [ecx - 1]
        cmp    ebx, edx
        jne    loc_2_3602
        cmp    word ptr [ecx], 0
        je     loc_2_3602
        and    si, word ptr [ecx]
loc_2_3602:
        add    ecx, 3
        dec    dword ptr [ebp - 0x10]
        jne    loc_2_35F1
loc_2_360A:
        add    edi, 0x10
        dec    dword ptr [ebp - 0xc]
        jne    loc_2_35E2
loc_2_3612:
        cmp    si, 0xffff
        jne    loc_2_361C
        xor    si, si
loc_2_361C:
        mov    word ptr [eax + 2], si
        add    eax, 0xc
loc_2_3623:
        inc    edx
        cmp    edx, 0x100
        jb     loc_2_35BA
        xor    edi, edi
        lea    eax, [ebp - 8]
        push   eax
        xor    esi, esi
        mov    dword ptr [ebp - 8], edi
        call   sub_3_0360
        add    esp, 4
        mov    ebx, dword ptr [0]            ; -> loc_3_0370
        cmp    dword ptr [0], edi            ; -> loc_3_0374
        je     loc_2_368A
loc_2_364E:
        mov    al, byte ptr [ebx]
        lea    ecx, [ebp - 1]
        push   eax
        push   ecx
        call   sub_3_0358
        add    esp, 8
        test   eax, eax
        jne    loc_2_367E
        cmp    byte ptr [ebp - 1], 0
        je     loc_2_367E
        mov    eax, 1
        mov    cl, byte ptr [ebp - 1]
        shl    eax, cl
        test   dword ptr [ebp - 8], eax
        je     loc_2_367E
        mov    ecx, ebx
        call   sub_2_3108
loc_2_367E:
        inc    esi
        add    ebx, 0xc
        cmp    dword ptr [0], esi            ; -> loc_3_0374
        ja     loc_2_364E
loc_2_368A:
        mov    eax, 1
loc_2_368F:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  056h, 057h, 033h, 0F6h, 08Bh, 03Dh
        dd  loc_3_0370               ; fixup
        db  039h, 035h
        dd  loc_3_0374               ; fixup
        db  074h, 01Eh, 08Bh, 047h, 004h, 085h, 0C0h, 074h, 00Bh, 06Ah, 000h, 050h, 0E8h, 071h, 043h, 000h
        db  000h, 083h, 0C4h, 008h, 046h, 083h, 0C7h, 00Ch, 03Bh, 035h
        dd  loc_3_0374               ; fixup
        db  072h, 0E2h, 05Fh, 05Eh, 0C3h
sub_2_36C9:
        push   esi
        xor    eax, eax
        mov    esi, dword ptr [0]            ; -> loc_3_0370
        mov    edx, dword ptr [0]            ; -> loc_3_0374
        cmp    edx, eax
        je     loc_2_36E8
loc_2_36DC:
        cmp    byte ptr [esi + 1], cl
        jne    loc_2_36E2
        inc    eax
loc_2_36E2:
        add    esi, 0xc
        dec    edx
        jne    loc_2_36DC
loc_2_36E8:
        pop    esi
        ret    
sub_2_36EA:
        sub    esp, 4
        push   ebx
        push   0
        push   dword ptr [ecx + 4]
        mov    bl, byte ptr [ecx + 1]
        call   sub_2_7A30
        add    esp, 8
        mov    ecx, ebx
        call   sub_2_36C9
        test   eax, eax
        jne    loc_2_3733
        lea    eax, [esp + 4]
        push   eax
        call   sub_3_0360
        add    esp, 4
        mov    eax, 1
        mov    cl, bl
        shl    eax, cl
        not    eax
        and    dword ptr [esp + 4], eax
        push   dword ptr [esp + 4]
        call   sub_3_035C
        add    esp, 4
loc_2_3733:
        pop    ebx
        add    esp, 4
        ret    
        db  056h, 057h, 033h, 0FFh, 08Bh, 035h
        dd  loc_3_0370               ; fixup
        db  039h, 03Dh
        dd  loc_3_0374               ; fixup
        db  074h, 01Fh, 083h, 07Eh, 004h, 000h, 074h, 00Dh, 083h, 07Eh, 008h, 000h, 075h, 007h, 08Bh, 0CEh
        db  0E8h, 08Dh, 0FFh, 0FFh, 0FFh, 047h, 083h, 0C6h, 00Ch, 03Bh, 03Dh
        dd  loc_3_0374               ; fixup
        db  072h, 0E1h, 05Fh, 05Eh, 0C3h, 0E8h, 025h, 0FFh, 0FFh, 0FFh, 0E9h, 0C2h, 0FFh, 0FFh, 0FFh
sub_2_3776:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x28
        push   ebx
        mov    dword ptr [ebp - 4], edx
        push   esi
        xor    eax, eax
        push   edi
        xor    esi, esi
        mov    word ptr [ebp - 0x10], si
        lea    edi, [ebp - 0xe]
        mov    dword ptr [ebp - 8], ecx
        lea    ecx, [ebp - 0x28]
        stosd  dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
        lea    edi, [ebp - 0x10]
        mov    dword ptr [ebp - 0x24], esi
        mov    dword ptr [ebp - 0x28], 0xb10e
        mov    dword ptr [ebp - 0x14], edi
        call   sub_2_4745
        cmp    word ptr [ebp - 0x10], si
        jne    loc_2_37B8
        xor    eax, eax
        jmp    loc_2_3843
loc_2_37B8:
        mov    ax, word ptr [ebp - 0x10]
        push   0x201
        shr    ax, 4
        movzx  ebx, ax
        movzx  eax, word ptr [ebp - 0x10]
        add    eax, 0x20
        push   eax
        call   sub_2_79C8
        add    esp, 8
        mov    esi, eax
        test   esi, esi
        jne    loc_2_37E2
        xor    eax, eax
        jmp    loc_2_3843
loc_2_37E2:
        lea    eax, [esi + 0x20]
        mov    dword ptr [ebp - 0xe], eax
        mov    word ptr [ebp - 0xa], ds
        lea    ecx, [ebp - 0x28]
        mov    dword ptr [ebp - 0x14], edi
        mov    dword ptr [ebp - 0x28], 0xb10e
        mov    dword ptr [ebp - 0x24], 0
        call   sub_2_4745
        test   eax, eax
        jne    loc_2_3818
        push   0
        push   esi
        call   sub_2_7950
        add    esp, 8
        xor    eax, eax
        jmp    loc_2_3843
loc_2_3818:
        mov    eax, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        mov    edx, dword ptr [ebp - 0x24]
        mov    dword ptr [eax], esi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ecx], ebx
        mov    word ptr [eax], dx
        mov    ax, word ptr [ebp - 0x10]
        add    ax, 0x20
        mov    word ptr [esi + 6], ax
        mov    eax, 1
        mov    edx, dword ptr [ebp - 0x24]
        mov    word ptr [esi + 0xa], dx
loc_2_3843:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
sub_2_384C:
        push   ebx
        cmp    dword ptr [ecx], 0x52495024
        push   esi
        jne    loc_2_3895
        cmp    word ptr [ecx + 4], 0x100
        jne    loc_2_3895
        mov    ax, word ptr [ecx + 6]
        test   al, 0xf
        jne    loc_2_3895
        cmp    ax, 0x20
        jbe    loc_2_3895
        xor    bl, bl
        xor    esi, esi
        test   ax, ax
        je     loc_2_3880
        movzx  eax, ax
loc_2_3878:
        add    bl, byte ptr [esi + ecx]
        inc    esi
        cmp    esi, eax
        jb     loc_2_3878
loc_2_3880:
        test   bl, bl
        je     loc_2_388E
        cmp    byte ptr [ecx + 0x1f], 0
        jne    loc_2_3895
        test   edx, edx
        je     loc_2_3895
loc_2_388E:
        mov    eax, 1
        jmp    loc_2_3897
loc_2_3895:
        xor    eax, eax
loc_2_3897:
        pop    esi
        pop    ebx
        ret    
sub_2_389A:
        sub    esp, 8
        mov    dword ptr [esp + 4], edx
        push   ebx
        mov    dword ptr [esp + 4], ecx
        push   esi
        push   edi
        push   ebp
        push   0
        push   0x10000
        push   0xf0000
        call   sub_2_7A38
        add    esp, 0xc
        mov    ebx, eax
        lea    esi, [eax + 0x10000]
        cmp    eax, esi
        jae    loc_2_38EF
loc_2_38C9:
        cmp    dword ptr [ebx], 0x52495024
        jne    loc_2_38E8
        movzx  eax, word ptr [ebx + 6]
        add    eax, ebx
        cmp    eax, esi
        ja     loc_2_38E8
        xor    edx, edx
        mov    ecx, ebx
        call   sub_2_384C
        test   eax, eax
        jne    loc_2_38FB
loc_2_38E8:
        add    ebx, 0x10
        cmp    ebx, esi
        jb     loc_2_38C9
loc_2_38EF:
        xor    eax, eax
loc_2_38F1:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    0x10
loc_2_38FB:
        movzx  ebp, word ptr [ebx + 6]
        push   0x201
        push   ebp
        call   sub_2_79C8
        add    esp, 8
        test   eax, eax
        je     loc_2_38EF
        mov    edi, eax
        mov    esi, ebx
        mov    ecx, ebp
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, ebp
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    ecx, dword ptr [esp + 0x10]
        mov    dx, word ptr [ebx + 0xa]
        mov    dword ptr [ecx], eax
        movzx  ecx, word ptr [ebx + 6]
        sub    ecx, 0x20
        mov    eax, dword ptr [esp + 0x14]
        shr    ecx, 4
        mov    dword ptr [eax], ecx
        mov    ecx, dword ptr [esp + 0x1c]
        mov    word ptr [ecx], dx
        mov    eax, dword ptr [esp + 0x20]
        mov    dl, byte ptr [ebx + 8]
        mov    cl, byte ptr [ebx + 9]
        mov    ebx, dword ptr [ebx + 0xc]
        mov    byte ptr [eax], dl
        mov    edx, dword ptr [esp + 0x24]
        mov    eax, dword ptr [esp + 0x28]
        mov    byte ptr [edx], cl
        mov    dword ptr [eax], ebx
        mov    eax, 1
        jmp    loc_2_38F1
sub_2_3966:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        lea    eax, [ebp - 4]
        push   esi
        mov    ebx, edx
        push   edi
        push   eax
        mov    edi, ecx
        push   0                             ; -> loc_3_052C
        push   dword ptr [0]                 ; -> obj3:0x05E0
        call   sub_2_79F0
        add    esp, 0xc
        test   eax, eax
        je     loc_2_3995
        xor    eax, eax
        jmp    loc_2_3A82
loc_2_3995:
        xor    eax, eax
        lea    ecx, [ebp - 8]
        push   ecx
        mov    dword ptr [ebp - 8], eax
        push   eax
        push   eax
        push   eax
        push   0                             ; -> obj3:0x053C
        push   dword ptr [ebp - 4]
        call   sub_2_7A40
        add    esp, 0x18
        test   eax, eax
        je     loc_2_39C7
        push   dword ptr [ebp - 4]
        call   sub_2_79E8
        add    esp, 4
        xor    eax, eax
        jmp    loc_2_3A82
loc_2_39C7:
        push   0x201
        push   dword ptr [ebp - 8]
        call   sub_2_79C8
        add    esp, 8
        mov    esi, eax
        test   esi, esi
        jne    loc_2_39EF
        push   dword ptr [ebp - 4]
        call   sub_2_79E8
        add    esp, 4
        xor    eax, eax
        jmp    loc_2_3A82
loc_2_39EF:
        lea    eax, [ebp - 8]
        push   eax
        push   esi
        push   0
        push   0
        push   0                             ; -> obj3:0x054C
        push   dword ptr [ebp - 4]
        call   sub_2_7A40
        add    esp, 0x18
        test   eax, eax
        push   dword ptr [ebp - 4]
        je     loc_2_3A26
        call   sub_2_79E8
        add    esp, 4
        push   0
        push   esi
        call   sub_2_7950
        add    esp, 8
        xor    eax, eax
        jmp    loc_2_3A82
loc_2_3A26:
        call   sub_2_79E8
        add    esp, 4
        mov    edx, 1
        mov    ecx, esi
        call   sub_2_384C
        test   eax, eax
        jne    loc_2_3A4D
        push   0
        push   esi
        call   sub_2_7950
        add    esp, 8
        xor    eax, eax
        jmp    loc_2_3A82
loc_2_3A4D:
        mov    cx, word ptr [esi + 0xa]
        mov    edx, dword ptr [ebp + 8]
        mov    word ptr [edx], cx
        mov    dword ptr [edi], esi
        movzx  eax, word ptr [esi + 6]
        sub    eax, 0x20
        mov    cl, byte ptr [esi + 9]
        shr    eax, 4
        mov    edx, dword ptr [ebp + 0x10]
        mov    dword ptr [ebx], eax
        mov    bl, byte ptr [esi + 8]
        mov    eax, dword ptr [ebp + 0xc]
        mov    byte ptr [edx], cl
        mov    byte ptr [eax], bl
        mov    ebx, dword ptr [esi + 0xc]
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [eax], ebx
        mov    eax, 1
loc_2_3A82:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
sub_2_3A8B:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        push   ebx
        push   esi
        push   edi
        mov    dword ptr [ecx], eax
        mov    dword ptr [edx], eax
        mov    edi, edx
        mov    esi, ecx
        mov    ebx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp + 0xc]
        mov    word ptr [ebx], ax
        mov    edx, dword ptr [ebp + 0x14]
        mov    eax, dword ptr [ebp + 0x10]
        push   edx
        push   eax
        mov    byte ptr [ecx], 0xff
        push   ecx
        mov    byte ptr [eax], 0xff
        push   ebx
        mov    ecx, esi
        mov    dword ptr [edx], 0xffffffff
        mov    edx, edi
        call   sub_2_3966
        test   eax, eax
        je     loc_2_3ACE
        mov    eax, 0x20
        jmp    loc_2_3B0A
loc_2_3ACE:
        mov    edx, edi
        mov    ecx, esi
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   ebx
        call   sub_2_389A
        test   eax, eax
        je     loc_2_3AEC
        mov    eax, 0x21
        jmp    loc_2_3B0A
loc_2_3AEC:
        test   byte ptr [0], 0x20            ; -> obj3:0x0CB8
        je     loc_2_3B08
        push   ebx
        mov    edx, edi
        mov    ecx, esi
        call   sub_2_3776
        test   eax, eax
        mov    eax, 0x22
        jne    loc_2_3B0A
loc_2_3B08:
        xor    eax, eax
loc_2_3B0A:
        pop    edi
        pop    esi
        pop    ebx
        pop    ebp
        ret    0x10
loc_2_3B11:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        cmp    dword ptr [0], 0              ; -> loc_3_037C
        push   ebx
        jne    loc_2_3B70
        push   0
        mov    edx, dword ptr [ebp + 0xc]
        mov    bl, byte ptr [ebp + 8]
        mov    ecx, ebx
        call   sub_2_58E1
        push   eax
        lea    ecx, [ebp - 0xc]
        push   0                             ; -> obj3:0x055C
        push   ecx
        call   sub_2_79F8
        add    esp, 0xc
        lea    ecx, [ebp - 0xc]
        push   0                             ; -> loc_3_037C
        push   ecx
        push   dword ptr [0]                 ; -> loc_3_0378
        call   sub_2_79F0
        add    esp, 0xc
        cmp    dword ptr [0], 0              ; -> loc_3_037C
        je     loc_2_3B70
        mov    al, byte ptr [ebp + 0xc]
        mov    byte ptr [0], bl              ; -> loc_3_0380
        mov    byte ptr [0], al              ; -> loc_3_0384
loc_2_3B70:
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_3B75:
        push   ebp
        xor    eax, eax
        mov    ebp, esp
        mov    dword ptr [0], eax            ; -> loc_3_0378
        sub    esp, 0xc
        mov    dword ptr [0], eax            ; -> loc_3_037C
        push   ebx
        push   esi
        mov    bl, dl
        mov    esi, ecx
        push   0                             ; -> loc_3_0378
        push   0                             ; -> obj3:0x0564
        push   dword ptr [0]                 ; -> obj3:0x05E0
        call   sub_2_79F0
        add    esp, 0xc
        test   eax, eax
        je     loc_2_3BB3
        mov    eax, 1
        jmp    loc_2_3D1F
loc_2_3BB3:
        push   0                             ; -> loc_3_037C
        mov    eax, dword ptr [0]            ; -> loc_3_0378
        push   0                             ; -> obj3:0x0574
        push   eax
        call   sub_2_79F0
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_3BF6
        mov    edx, esi
        mov    ecx, dword ptr [0]            ; -> loc_3_037C
        call   sub_2_3F18
        mov    esi, eax
        mov    eax, dword ptr [0]            ; -> loc_3_037C
        push   eax
        call   sub_2_79E8
        add    esp, 4
        mov    eax, dword ptr [0]            ; -> loc_3_0378
        jmp    loc_2_3D14
loc_2_3BF6:
        cmp    bl, 0xff
        je     loc_2_3CD2
        cmp    byte ptr [ebp + 8], 0xff
        je     loc_2_3C6A
        push   0
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, ebx
        call   sub_2_58E1
        cmp    eax, -1
        je     loc_2_3C6A
        push   eax
        lea    ecx, [ebp - 0xc]
        push   0                             ; -> obj3:0x0580
        push   ecx
        call   sub_2_79F8
        add    esp, 0xc
        lea    ecx, [ebp - 0xc]
        push   0                             ; -> loc_3_037C
        push   ecx
        push   dword ptr [0]                 ; -> loc_3_0378
        call   sub_2_79F0
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_3C6A
        mov    edx, esi
        mov    ecx, dword ptr [0]            ; -> loc_3_037C
        call   sub_2_3F18
        mov    esi, eax
        mov    eax, dword ptr [0]            ; -> loc_3_037C
        push   eax
        call   sub_2_79E8
        add    esp, 4
        mov    eax, dword ptr [0]            ; -> loc_3_0378
        jmp    loc_2_3D14
loc_2_3C6A:
        cmp    bl, 0xff
        je     loc_2_3CD2
        cmp    byte ptr [ebp + 8], 0xff
        je     loc_2_3CD2
        mov    eax, dword ptr [ebp + 0xc]
        cmp    eax, -1
        je     loc_2_3CD2
        test   eax, eax
        je     loc_2_3CD2
        push   eax
        lea    ecx, [ebp - 0xc]
        push   0                             ; -> loc_3_0588
        push   ecx
        call   sub_2_79F8
        add    esp, 0xc
        lea    ecx, [ebp - 0xc]
        push   0                             ; -> loc_3_037C
        push   ecx
        push   dword ptr [0]                 ; -> loc_3_0378
        call   sub_2_79F0
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_3CD2
        mov    edx, esi
        mov    ecx, dword ptr [0]            ; -> loc_3_037C
        call   sub_2_3F18
        mov    esi, eax
        mov    eax, dword ptr [0]            ; -> loc_3_037C
        push   eax
        call   sub_2_79E8
        add    esp, 4
        mov    eax, dword ptr [0]            ; -> loc_3_0378
        jmp    loc_2_3D14
loc_2_3CD2:
        push   0                             ; -> loc_2_3B11
        xor    edx, edx
        mov    ecx, dword ptr [0]            ; -> obj3:0x0DAC
        call   sub_2_65F2
        mov    eax, dword ptr [0]            ; -> loc_3_0378
        push   eax
        call   sub_2_79E8
        add    esp, 4
        mov    eax, 2
        cmp    dword ptr [0], 0              ; -> loc_3_037C
        je     loc_2_3D1F
        mov    edx, esi
        mov    ecx, dword ptr [0]            ; -> loc_3_037C
        call   sub_2_3F18
        mov    esi, eax
        mov    eax, dword ptr [0]            ; -> loc_3_037C
loc_2_3D14:
        push   eax
        call   sub_2_79E8
        add    esp, 4
        mov    eax, esi
loc_2_3D1F:
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
sub_2_3D27:
        push   esi
        mov    esi, edx
        mov    edx, ecx
        mov    ecx, 0                        ; -> obj3:0x0590
        call   sub_2_4564
        mov    ecx, 0                        ; -> obj3:0x05A0
        mov    edx, esi
        call   sub_2_4564
        mov    ecx, 0                        ; -> loc_3_05B8
        mov    edx, dword ptr [esp + 8]
        call   sub_2_4564
        pop    esi
        ret    4
sub_2_3D54:
        push   esi
        push   edi
        xor    esi, esi
        cmp    dword ptr [0], esi            ; -> loc_3_0368
        je     loc_2_3D7B
        push   esi
        mov    eax, dword ptr [0]            ; -> loc_3_0418
        push   eax
        call   sub_2_7950
        add    esp, 8
        mov    dword ptr [0], esi            ; -> loc_3_0418
        mov    dword ptr [0], esi            ; -> loc_3_0368
loc_2_3D7B:
        cmp    dword ptr [0], 0              ; -> loc_3_0388
        je     loc_2_3D8E
        mov    ecx, 0                        ; -> loc_3_0388
        call   sub_2_402B
loc_2_3D8E:
        mov    edi, 0                        ; -> sub_3_0354
        xor    eax, eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        stosd  dword ptr es:[edi], eax
        pop    edi
        pop    esi
        ret    
sub_2_3D9D:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        call   sub_2_3D54
        test   byte ptr [0], 2               ; -> obj3:0x0CB8
        jne    loc_2_3DBE
        mov    edx, 0xffffffff
        push   edx
        mov    ecx, edx
        jmp    loc_2_3F0C
loc_2_3DBE:
        lea    eax, [esp + 0xc]
        mov    edx, 0                        ; -> loc_3_036C
        push   eax
        mov    ecx, 0                        ; -> loc_3_0418
        push   0                             ; -> loc_3_0384
        push   0                             ; -> loc_3_0380
        push   0                             ; -> obj3:0x0350
        call   sub_2_3A8B
        cmp    eax, 0x20
        mov    esi, eax
        jae    loc_2_3DF4
        push   -1
        mov    edx, 0xffffffff
        jmp    loc_2_3F0A
loc_2_3DF4:
        mov    eax, dword ptr [0]            ; -> loc_3_0418
        mov    dl, byte ptr [0]              ; -> loc_3_0384
        add    eax, 0x20
        mov    ecx, 0                        ; -> loc_3_0388
        push   dword ptr [esp + 0xc]
        push   edx
        mov    dl, byte ptr [0]              ; -> loc_3_0380
        mov    dword ptr [0], eax            ; -> loc_3_0368
        call   sub_2_3B75
        cmp    eax, 0x20
        mov    edi, eax
        jae    loc_2_3E2F
        call   sub_2_3D54
        push   -1
        jmp    loc_2_3F08
loc_2_3E2F:
        mov    al, byte ptr [0]              ; -> loc_3_0380
        mov    cl, byte ptr [0]              ; -> loc_3_0384
        mov    byte ptr [4], al              ; -> loc_3_038C
        mov    byte ptr [5], cl              ; -> obj3:0x038D
        mov    eax, dword ptr [0]            ; -> loc_3_0418
        add    eax, 8
        mov    dword ptr [0xa], 0            ; -> loc_3_0392
        cmp    byte ptr [eax], 0xff
        jne    loc_2_3E64
        mov    cl, byte ptr [0]              ; -> loc_3_0380
        mov    byte ptr [eax], cl
loc_2_3E64:
        mov    eax, dword ptr [0]            ; -> loc_3_0418
        add    eax, 9
        cmp    byte ptr [eax], 0xff
        jne    loc_2_3E79
        mov    cl, byte ptr [0]              ; -> loc_3_0384
        mov    byte ptr [eax], cl
loc_2_3E79:
        mov    ecx, 0                        ; -> loc_3_0388
        call   sub_2_3F7D
        cmp    eax, 0x20
        mov    ebx, eax
        jae    loc_2_3E92
        call   sub_2_3D54
        push   ebx
        jmp    loc_2_3F08
loc_2_3E92:
        mov    eax, dword ptr [0x14]         ; -> loc_3_0430
        mov    ecx, dword ptr [0x1c]         ; -> loc_3_0438
        mov    edx, dword ptr [0x24]         ; -> loc_3_0440
        mov    dword ptr [0], eax            ; -> sub_3_0354
        mov    eax, dword ptr [0x2c]         ; -> loc_3_0448
        mov    dword ptr [4], ecx            ; -> sub_3_0358
        mov    ecx, dword ptr [0x34]         ; -> loc_3_0450
        mov    dword ptr [8], edx            ; -> sub_3_035C
        mov    dword ptr [0xc], eax          ; -> sub_3_0360
        mov    dword ptr [0x10], ecx         ; -> sub_3_0364
        xor    eax, eax
        cmp    esi, 0x22
        jne    loc_2_3ED6
        mov    eax, 1
loc_2_3ED6:
        push   eax
        push   dword ptr [0]                 ; -> loc_3_0418
        call   sub_3_0364
        add    esp, 8
        test   eax, eax
        je     loc_2_3EF3
        call   sub_2_3D54
        push   3
        jmp    loc_2_3F08
loc_2_3EF3:
        call   sub_2_34F9
        test   eax, eax
        jne    loc_2_3F06
        mov    ecx, 0xffffffff
        push   ecx
        mov    edx, ecx
        jmp    loc_2_3F0C
loc_2_3F06:
        push   0x20
loc_2_3F08:
        mov    edx, edi
loc_2_3F0A:
        mov    ecx, esi
loc_2_3F0C:
        call   sub_2_3D27
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_2_3F18:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    eax, [ebp - 4]
        push   edi
        mov    esi, ecx
        lea    ecx, [edx + 0xe]
        mov    edi, edx
        mov    dword ptr [ebp - 4], 0x80
        push   eax
        push   ecx
        push   0
        push   0
        push   0                             ; -> loc_3_05CC
        push   esi
        call   sub_2_7A40
        add    esp, 0x18
        test   eax, eax
        mov    eax, 0
        jne    loc_2_3F77
        lea    eax, [ebp - 4]
        add    edi, 6
        mov    dword ptr [ebp - 4], 4
        push   eax
        push   edi
        push   0
        push   0
        push   0                             ; -> loc_3_05D4
        push   esi
        call   sub_2_7A40
        add    esp, 0x18
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 0x20
loc_2_3F77:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_3F7D:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        mov    dword ptr [ecx], 0
        push   edi
        mov    esi, ecx
        push   0                             ; -> obj1:0x0000
        lea    edx, [ebp - 4]
        lea    ecx, [esi + 0xe]
        push   ecx
        push   edx
        call   sub_2_7A58
        add    esp, 0xc
        test   eax, eax
        je     loc_2_3FAB
        xor    eax, eax
        jmp    loc_2_4024
loc_2_3FAB:
        mov    edi, dword ptr [esi + 0xa]
        cmp    dword ptr [edi], 0
        je     loc_2_3FD4
loc_2_3FB3:
        push   0                             ; -> obj1:0x0000
        push   dword ptr [edi]
        push   dword ptr [ebp - 4]
        call   sub_2_7A50
        add    esp, 0xc
        mov    dword ptr [edi + 4], eax
        test   eax, eax
        je     loc_2_4003
        add    edi, 8
        cmp    dword ptr [edi], 0
        jne    loc_2_3FB3
loc_2_3FD4:
        mov    eax, dword ptr [esi + 0xa]
        mov    cl, byte ptr [esi + 5]
        push   ecx
        mov    dl, byte ptr [esi + 4]
        push   edx
        push   dword ptr [esi + 6]
        call   dword ptr [eax + 4]
        add    esp, 0xc
        test   eax, eax
        je     loc_2_401A
        push   0                             ; -> obj1:0x0000
        push   dword ptr [ebp - 4]
        call   sub_2_7A48
        add    esp, 8
        mov    eax, 2
        jmp    loc_2_4024
loc_2_4003:
        push   0                             ; -> obj1:0x0000
        push   dword ptr [ebp - 4]
        call   sub_2_7A48
        add    esp, 8
        mov    eax, 1
        jmp    loc_2_4024
loc_2_401A:
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [esi], eax
        mov    eax, 0x20
loc_2_4024:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_402B:
        push   esi
        push   edi
        mov    esi, dword ptr [ecx]
        mov    edi, ecx
        test   esi, esi
        je     loc_2_4068
        mov    eax, dword ptr [edi + 0xa]
        call   dword ptr [eax + 0xc]
        push   0                             ; -> obj1:0x0000
        push   esi
        call   sub_2_7A48
        add    esp, 8
        mov    eax, dword ptr [edi + 0xa]
        cmp    dword ptr [eax], 0
        je     loc_2_405D
        xor    ecx, ecx
loc_2_4053:
        mov    dword ptr [eax + 4], ecx
        add    eax, 8
        cmp    dword ptr [eax], ecx
        jne    loc_2_4053
loc_2_405D:
        xor    eax, eax
        mov    ecx, 0x23
        rep stosd dword ptr es:[edi], eax
        stosw  word ptr es:[edi], ax
loc_2_4068:
        pop    edi
        pop    esi
        ret    
sub_2_406B:
        push   0xe
        call   sub_2_58C5
        and    al, 0x7f
        ret    
sub_2_4075:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        mov    byte ptr [ebp - 4], dl
        push   esi
        mov    bl, cl
        call   sub_2_44FB
        push   0
        mov    edx, dword ptr [ebp - 4]
        mov    ecx, ebx
        call   sub_2_58E1
        push   eax
        lea    ecx, [ebp - 0xc]
        push   0                             ; -> obj3:0x0C50
        push   ecx
        call   sub_2_79F8
        add    esp, 0xc
        lea    ecx, [ebp - 0xc]
        call   sub_2_4521
        mov    esi, eax
        call   sub_2_4592
        mov    eax, esi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_40BC:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        mov    byte ptr [ebp - 1], dl
        push   esi
        mov    bl, cl
        push   edi
        mov    edx, dword ptr [ebp - 1]
        push   4
        mov    ecx, ebx
        call   sub_2_58D3
        mov    word ptr [ebp - 4], ax
        mov    eax, 3
        movzx  ecx, word ptr [ebp - 4]
        and    eax, ecx
        mov    edx, dword ptr [ebp - 1]
        mov    ecx, ebx
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
        mov    dword ptr [ebp - 8], eax
        call   sub_2_4075
        cmp    dword ptr [ebp - 8], 0
        je     loc_2_41F2
        test   eax, 0x10000
        je     loc_2_41F2
        mov    edx, dword ptr [ebp - 1]
        mov    ecx, ebx
        call   sub_2_406B
        cmp    al, 2
        jne    loc_2_41F2
        mov    dword ptr [ebp - 8], 0
        test   byte ptr [ebp - 4], 2
        je     loc_2_4195
        push   0x10
        mov    edx, dword ptr [ebp - 1]
        mov    ecx, ebx
        call   sub_2_58E1
        test   eax, eax
        je     loc_2_4146
        mov    eax, 1
        jmp    loc_2_41F5
loc_2_4146:
        push   0x1c
        mov    edx, dword ptr [ebp - 1]
        mov    ecx, ebx
        call   sub_2_58E1
        mov    esi, eax
        push   0x20
        mov    edx, dword ptr [ebp - 1]
        mov    ecx, ebx
        call   sub_2_58E1
        push   0x24
        mov    edx, dword ptr [ebp - 1]
        mov    ecx, ebx
        mov    dword ptr [ebp - 0xc], eax
        call   sub_2_58E1
        mov    edi, eax
        push   0x28
        mov    edx, dword ptr [ebp - 1]
        mov    ecx, ebx
        call   sub_2_58E1
        test   esi, esi
        je     loc_2_4186
        cmp    dword ptr [ebp - 0xc], esi
        jae    loc_2_418E
loc_2_4186:
        test   edi, edi
        je     loc_2_4195
        cmp    eax, edi
        jb     loc_2_4195
loc_2_418E:
        mov    eax, 1
        jmp    loc_2_41F5
loc_2_4195:
        test   byte ptr [ebp - 4], 1
        je     loc_2_41F2
        push   0x2c
        mov    edx, dword ptr [ebp - 1]
        mov    ecx, ebx
        call   sub_2_58D3
        push   0x30
        mov    edx, dword ptr [ebp - 1]
        movzx  esi, ax
        mov    ecx, ebx
        call   sub_2_58D3
        push   0x34
        mov    edx, dword ptr [ebp - 1]
        movzx  eax, ax
        mov    ecx, ebx
        mov    dword ptr [ebp - 0xc], eax
        call   sub_2_58D3
        push   0x38
        mov    edx, dword ptr [ebp - 1]
        movzx  edi, ax
        mov    ecx, ebx
        call   sub_2_58D3
        movzx  eax, ax
        test   esi, esi
        je     loc_2_41E3
        cmp    dword ptr [ebp - 0xc], esi
        jae    loc_2_41EB
loc_2_41E3:
        test   edi, edi
        je     loc_2_41F2
        cmp    eax, edi
        jb     loc_2_41F2
loc_2_41EB:
        mov    eax, 1
        jmp    loc_2_41F5
loc_2_41F2:
        mov    eax, dword ptr [ebp - 8]
loc_2_41F5:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
sub_2_41FE:
        cmp    cx, 0xffff
        je     loc_2_420F
        test   cx, cx
        mov    eax, 1
        jne    loc_2_4211
loc_2_420F:
        xor    eax, eax
loc_2_4211:
        ret    
sub_2_4212:
        push   ebp
        mov    eax, dword ptr [0]            ; -> obj3:0x0DAC
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   0
        mov    esi, edx
        push   eax
        push   ecx
        lea    edx, [ebp - 4]
        push   edx
        call   sub_2_7A60
        add    esp, 0x10
        test   esi, esi
        je     loc_2_4239
        mov    eax, dword ptr [ebp - 4]
        mov    byte ptr [esi], ah
loc_2_4239:
        mov    eax, dword ptr [ebp + 8]
        test   eax, eax
        je     loc_2_4245
        mov    cl, byte ptr [ebp - 4]
        mov    byte ptr [eax], cl
loc_2_4245:
        mov    eax, dword ptr [ebp + 0xc]
        test   eax, eax
        je     loc_2_4254
        mov    ecx, dword ptr [ebp - 4]
        shr    ecx, 0x10
        mov    byte ptr [eax], cl
loc_2_4254:
        mov    eax, dword ptr [ebp + 0x10]
        test   eax, eax
        je     loc_2_4263
        mov    ecx, dword ptr [ebp - 4]
        shr    ecx, 0x10
        mov    byte ptr [eax], ch
loc_2_4263:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    0xc
sub_2_426A:
        movzx  eax, byte ptr [esp + 8]
        push   esi
        push   0
        movzx  esi, byte ptr [esp + 0x14]
        shl    esi, 8
        add    esi, eax
        movzx  eax, dl
        shl    esi, 8
        add    esi, eax
        movzx  eax, byte ptr [esp + 0xc]
        shl    esi, 8
        add    esi, eax
        push   esi
        push   dword ptr [0]                 ; -> obj3:0x0DAC
        push   ecx
        call   sub_2_7A68
        add    esp, 0x10
        pop    esi
        ret    0xc
sub_2_42A2:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        lea    edx, [ebp - 1]
        mov    esi, ecx
        push   0
        push   edx
        push   0
        xor    edx, edx
        call   sub_2_4212
        test   byte ptr [ebp - 1], 4
        mov    eax, 0
        je     loc_2_42D8
        push   0
        lea    eax, [ebp - 4]
        push   esi
        push   esi
        push   eax
        call   sub_2_7A60
        add    esp, 0x10
        mov    eax, dword ptr [ebp - 4]
loc_2_42D8:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_42DD:
        push   0
        push   edx
        push   ecx
        push   ecx
        call   sub_2_7A68
        add    esp, 0x10
        ret    
sub_2_42EB:
        cmp    word ptr [0], 0xffff          ; -> obj3:0x0624
        mov    eax, 0                        ; -> obj3:0x0624
        je     loc_2_430A
loc_2_42FB:
        cmp    word ptr [eax], cx
        je     loc_2_430A
        add    eax, 6
        cmp    word ptr [eax], 0xffff
        jne    loc_2_42FB
loc_2_430A:
        mov    eax, dword ptr [eax + 2]
        ret    
sub_2_430E:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   0
        lea    eax, [ebp - 4]
        push   ecx
        mov    dword ptr [ebp - 4], ecx
        push   eax
        call   sub_2_7958
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 4]
        sub    eax, dword ptr [0]            ; -> obj3:0x0DAC
        mov    esp, ebp
        cmp    eax, 1
        pop    ebp
        sbb    eax, eax
        neg    eax
        ret    
sub_2_433A:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        mov    byte ptr [ebp - 1], dl
        push   esi
        xor    ebx, ebx
        push   edi
        mov    edx, dword ptr [ebp + 8]
        push   ebx
        mov    dword ptr [ebp - 8], ecx
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_58E1
        mov    edx, eax
        cmp    byte ptr [ebp - 1], bl
        je     loc_2_440C
        cmp    dword ptr [ebp - 8], ebx
        je     loc_2_440C
        mov    eax, dword ptr [0]            ; -> obj3:0x0DAC
        cmp    dword ptr [ebp - 8], eax
        je     loc_2_440C
        mov    esi, 0                        ; -> obj3:0x05E8
        xor    eax, eax
        mov    edi, esi
        mov    ecx, 0xe
        rep stosd dword ptr es:[edi], eax
        stosb  byte ptr es:[edi], al
        mov    eax, edx
        shr    eax, 0x10
        movzx  edx, dx
        movzx  ecx, ax
        push   ecx
        push   edx
        push   0                             ; -> obj3:0x0C8C
        push   esi
        xor    esi, esi
        call   sub_2_79F8
        add    esp, 0x10
loc_2_43A7:
        lea    ecx, [ebx + 0x16]             ; -> obj3:0x05FE
        add    ebx, 2
        movzx  eax, byte ptr [ebp + 8]
        push   eax
        push   0                             ; -> obj3:0x0CA4
        push   ecx
        call   sub_2_79F8
        add    esp, 0xc
        mov    ecx, dword ptr [0]            ; -> obj3:0x0DAC
        cmp    dword ptr [ebp - 8], ecx
        je     loc_2_43F1
        push   esi
        lea    eax, [ebp + 8]
        push   esi
        lea    edx, [ebp - 1]
        push   eax
        mov    ecx, dword ptr [ebp - 8]
        call   sub_2_4212
        push   esi
        lea    ecx, [ebp - 8]
        push   dword ptr [ebp - 8]
        push   ecx
        call   sub_2_7958
        add    esp, 0xc
        jmp    loc_2_43A7
loc_2_43F1:
        movzx  eax, byte ptr [ebp - 1]
        push   eax
        add    ebx, 0x16                     ; -> obj3:0x05FE
        push   0                             ; -> obj3:0x0CAC
        push   ebx
        call   sub_2_79F8
        add    esp, 0xc
        jmp    loc_2_4443
loc_2_440C:
        xor    eax, eax
        mov    al, byte ptr [ebp + 8]
        and    eax, 7
        push   eax
        mov    al, byte ptr [ebp + 8]
        shr    al, 3
        movzx  ebx, byte ptr [ebp - 1]
        movzx  ecx, al
        push   ecx
        mov    eax, edx
        shr    eax, 0x10
        push   ebx
        movzx  ecx, ax
        movzx  edx, dx
        push   ecx
        push   edx
        push   0                             ; -> obj3:0x0C58
        push   0                             ; -> obj3:0x05E8
        call   sub_2_79F8
        add    esp, 0x1c
loc_2_4443:
        mov    eax, 0                        ; -> obj3:0x05E8
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
sub_2_4451:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        mov    byte ptr [ebp - 1], dl
        push   esi
        mov    bl, cl
        mov    esi, dword ptr [ebp + 0xc]
        mov    edx, ebx
        mov    ecx, dword ptr [ebp + 8]
        cmp    esi, 1
        sbb    eax, eax
        inc    eax
        push   eax
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp - 1]
        call   sub_2_433A
        push   eax
        lea    ecx, [ebp - 8]
        push   ecx
        call   sub_2_7A18
        add    esp, 0x10
        cmp    eax, 0x10
        jne    loc_2_44AB
        test   esi, esi
        je     loc_2_4492
        xor    eax, eax
        jmp    loc_2_44F3
loc_2_4492:
        push   0
        lea    ecx, [ebp - 0x10]
        push   dword ptr [ebp - 8]
        push   ecx
        call   sub_2_7918
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_44B3
        xor    eax, eax
        jmp    loc_2_44F3
loc_2_44AB:
        test   eax, eax
        je     loc_2_44B3
        xor    eax, eax
        jmp    loc_2_44F3
loc_2_44B3:
        mov    al, 0
        cmp    esi, 1
        adc    al, 0xff
        xor    esi, esi
        and    al, 4
        push   esi
        or     al, 1
        mov    edx, ebx
        push   eax
        mov    ecx, dword ptr [ebp - 8]
        push   dword ptr [ebp - 1]
        call   sub_2_426A
        mov    word ptr [ebp - 0xa], si
        mov    cl, byte ptr [ebp - 1]
        push   esi
        mov    byte ptr [ebp - 0xc], bl
        lea    ebx, [ebp - 0xc]
        mov    byte ptr [ebp - 0xb], cl
        push   ebx
        push   4
        push   4
        push   dword ptr [ebp - 8]
        call   sub_2_7978
        add    esp, 0x14
        mov    eax, dword ptr [ebp - 8]
loc_2_44F3:
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
sub_2_44FB:
        cmp    dword ptr [0], 0              ; -> obj3:0x05E4
        jne    loc_2_451A
        push   0                             ; -> obj3:0x05E0
        push   1
        push   0                             ; -> obj3:0x0CB4
        push   2
        call   sub_2_7A70
        add    esp, 0x10
loc_2_451A:
        inc    dword ptr [0]                 ; -> obj3:0x05E4
        ret    
sub_2_4521:
        push   ebp
        mov    eax, 0
        mov    ebp, esp
        sub    esp, 0xc
        cmp    dword ptr [0], 0              ; -> obj3:0x05E0
        je     loc_2_4560
        xor    edx, edx
        lea    eax, [ebp - 8]
        push   eax
        mov    dword ptr [ebp - 4], edx
        mov    dword ptr [ebp - 8], 4
        lea    eax, [ebp - 4]
        push   eax
        lea    eax, [ebp - 0xc]
        push   eax
        push   edx
        push   ecx
        push   dword ptr [0]                 ; -> obj3:0x05E0
        call   sub_2_7A40
        add    esp, 0x18
        mov    eax, dword ptr [ebp - 4]
loc_2_4560:
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_4564:
        sub    esp, 4
        cmp    dword ptr [0], 0              ; -> obj3:0x05E0
        mov    dword ptr [esp], edx
        je     loc_2_458E
        lea    eax, [esp]
        push   4
        push   eax
        push   3
        push   0
        push   ecx
        push   dword ptr [0]                 ; -> obj3:0x05E0
        call   sub_2_7A78
        add    esp, 0x18
loc_2_458E:
        add    esp, 4
        ret    
sub_2_4592:
        dec    dword ptr [0]                 ; -> obj3:0x05E4
        jne    loc_2_45BB
        cmp    dword ptr [0], 0              ; -> obj3:0x05E0
        je     loc_2_45B1
        mov    eax, dword ptr [0]            ; -> obj3:0x05E0
        push   eax
        call   sub_2_79E8
        add    esp, 4
loc_2_45B1:
        mov    dword ptr [0], 0              ; -> obj3:0x05E0
loc_2_45BB:
        ret    
sub_2_45BC:
        push   ebx
        xor    eax, eax
        push   esi
        xor    bl, bl
        cmp    byte ptr [ecx], al
        je     loc_2_45E9
loc_2_45C6:
        movzx  eax, bl
        imul   eax, eax, 0x2c
        cmp    dword ptr [eax + ecx + 0x24], edx
        lea    eax, [eax + ecx + 4]
        jne    loc_2_45E3
        mov    esi, dword ptr [eax + 0x1c]
        and    esi, dword ptr [esp + 0xc]
        cmp    esi, dword ptr [esp + 0xc]
        je     loc_2_45E9
loc_2_45E3:
        inc    bl
        cmp    bl, byte ptr [ecx]
        jb     loc_2_45C6
loc_2_45E9:
        cmp    bl, byte ptr [ecx]
        jne    loc_2_460F
        xor    bl, bl
        cmp    bl, byte ptr [ecx]
        je     loc_2_460D
loc_2_45F3:
        movzx  eax, bl
        imul   eax, eax, 0x2c
        cmp    dword ptr [eax + ecx + 0x24], edx
        lea    eax, [eax + ecx + 4]
        je     loc_2_4609
        inc    bl
        cmp    bl, byte ptr [ecx]
        jb     loc_2_45F3
loc_2_4609:
        cmp    bl, byte ptr [ecx]
        jne    loc_2_460F
loc_2_460D:
        xor    eax, eax
loc_2_460F:
        pop    esi
        pop    ebx
        ret    4
sub_2_4614:
        cmp    dword ptr [0], 0              ; -> loc_3_0CD0
        jne    loc_2_4646
        push   0
        push   0                             ; -> obj3:0x0CF8
        push   0                             ; -> obj3:0x0CB8
        call   sub_2_7A80
        add    esp, 0xc
        push   0
        push   0                             ; -> loc_2_57DC
        push   0                             ; -> sub_2_4614
        call   sub_2_7A80
        add    esp, 0xc
        cli    
loc_2_4646:
        inc    dword ptr [0]                 ; -> loc_3_0CD0
        ret    
sub_2_464D:
        dec    dword ptr [0]                 ; -> loc_3_0CD0
        jne    loc_2_467E
        sti    
        push   0
        push   0                             ; -> loc_2_57DC
        push   0                             ; -> sub_2_4614
        call   sub_2_7A88
        add    esp, 0xc
        push   0
        push   0                             ; -> obj3:0x0CF8
        push   0                             ; -> obj3:0x0CB8
        call   sub_2_7A88
        add    esp, 0xc
loc_2_467E:
        ret    
sub_2_467F:
        mov    eax, edx
        mov    dx, cx
        movzx  ecx, byte ptr [esp + 4]
        test   ecx, ecx
        je     loc_2_4699
        cmp    ecx, 1
        je     loc_2_469C
        cmp    ecx, 2
        je     loc_2_46A0
        jmp    loc_2_46A1
loc_2_4699:
        out    dx, al
        jmp    loc_2_46A1
loc_2_469C:
        out    dx, ax
        jmp    loc_2_46A1
loc_2_46A0:
        out    dx, eax
loc_2_46A1:
        ret    4
sub_2_46A4:
        movzx  eax, dl
        test   eax, eax
        je     loc_2_46B7
        cmp    eax, 1
        je     loc_2_46C0
        cmp    eax, 2
        je     loc_2_46CA
        jmp    loc_2_46CE
loc_2_46B7:
        mov    dx, cx
        in     al, dx
        movzx  eax, al
        jmp    loc_2_46CE
loc_2_46C0:
        mov    dx, cx
        in     ax, dx
        movzx  eax, ax
        jmp    loc_2_46CE
loc_2_46CA:
        mov    dx, cx
        in     eax, dx
loc_2_46CE:
        ret    
sub_2_46CF:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        cmp    dword ptr [0], 0              ; -> sub_3_0CC8
        push   ebx
        mov    dword ptr [ebp - 8], ecx
        mov    dword ptr [ebp - 4], 1
        push   esi
        push   edi
        je     loc_2_4734
        pushal 
        mov    edi, dword ptr [ebp - 8]
        mov    eax, dword ptr [edi]
        mov    ebx, dword ptr [edi + 4]
        mov    ecx, dword ptr [edi + 8]
        mov    edx, dword ptr [edi + 0xc]
        mov    esi, dword ptr [edi + 0x10]
        mov    edi, dword ptr [edi + 0x14]
        mov    dword ptr [0], esp            ; -> loc_3_0CCC
        push   cs
        call   sub_3_0CC8
        mov    esp, dword ptr [0]            ; -> loc_3_0CCC
        push   edi
        mov    edi, dword ptr [ebp - 8]
        mov    dword ptr [edi], eax
        mov    dword ptr [edi + 4], ebx
        mov    dword ptr [edi + 8], ecx
        mov    dword ptr [edi + 0xc], edx
        mov    dword ptr [edi + 0x10], esi
        pop    dword ptr [edi + 0x14]
        popal  
        jae    loc_2_4732
        mov    dword ptr [ebp - 4], 0
loc_2_4732:
        jmp    loc_2_473B
loc_2_4734:
        mov    dword ptr [ebp - 4], 0
loc_2_473B:
        mov    eax, dword ptr [ebp - 4]
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_4745:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        mov    eax, dword ptr [ecx]
        push   edi
        mov    esi, ecx
        xor    edi, edi
        mov    dword ptr [ebp - 8], eax
        xor    ecx, ecx
        xor    edx, edx
        VMMCall  VMM.Get_VMM_Version             ; cd2000000100
        mov    word ptr [ebp - 2], ax
        cmp    word ptr [ebp - 2], 0x40a
        jb     loc_2_47AF
        push   edi
        lea    eax, [ebp - 8]
        push   4
        mov    ebx, 0                        ; -> obj3:0x0CE8
        push   eax
        push   ebx
        push   edi
        call   sub_2_7A90
        add    esp, 0x14
        test   eax, eax
        jne    loc_2_47C2
        call   sub_2_4614
        mov    ecx, esi
        call   sub_2_46CF
        mov    edi, eax
        call   sub_2_464D
        push   0
        lea    eax, [ebp - 8]
        push   4
        push   eax
        push   ebx
        push   3
        call   sub_2_7A90
        add    esp, 0x14
        jmp    loc_2_47C2
loc_2_47AF:
        call   sub_2_4614
        mov    ecx, esi
        call   sub_2_46CF
        mov    edi, eax
        call   sub_2_464D
loc_2_47C2:
        mov    eax, edi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_47CB:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x1c
        cmp    dword ptr [0], 0              ; -> loc_3_0CD4
        push   ebx
        mov    byte ptr [ebp - 1], dl
        push   esi
        mov    bl, cl
        push   edi
        je     loc_2_47F0
        test   bl, bl
        je     loc_2_47F0
        mov    eax, 0xffffffff
        jmp    loc_2_4A34
loc_2_47F0:
        call   sub_2_4614
        test   byte ptr [0], 1               ; -> obj3:0x0CB8
        je     loc_2_483E
        movzx  eax, byte ptr [ebp + 0xc]
        movzx  edx, byte ptr [ebp + 8]
        add    eax, 0xb108
        mov    dword ptr [ebp - 8], edx
        mov    dword ptr [ebp - 0x1c], eax
        movzx  ecx, byte ptr [ebp - 1]
        movzx  eax, bl
        shl    eax, 8
        or     eax, ecx
        lea    ecx, [ebp - 0x1c]
        mov    dword ptr [ebp - 0x18], eax
        call   sub_2_46CF
        test   eax, eax
        je     loc_2_4834
        mov    edi, dword ptr [ebp - 0x14]
        jmp    loc_2_4A2D
loc_2_4834:
        mov    edi, 0xffffffff
        jmp    loc_2_4A2D
loc_2_483E:
        mov    byte ptr [ebp - 2], 0
        mov    byte ptr [ebp - 3], 0
        in     al, 0x21
        xor    esi, esi
        cmp    dword ptr [0], esi            ; -> obj3:0x0CC0
        jne    loc_2_4946
        xor    edx, edx
        mov    edi, 0xcf8
        mov    ecx, edi
        call   sub_2_46A4
        push   esi
        mov    byte ptr [ebp - 2], al
        mov    al, byte ptr [ebp - 1]
        and    al, 7
        or     al, 0x78
        movzx  ecx, al
        lea    edx, [ecx*2]
        mov    ecx, edi
        call   sub_2_467F
        cmp    dword ptr [0], esi            ; -> loc_3_0CD4
        jne    loc_2_48A4
        xor    edx, edx
        mov    edi, 0xcfa
        mov    ecx, edi
        call   sub_2_46A4
        push   esi
        mov    ecx, edi
        movzx  edx, bl
        mov    byte ptr [ebp - 3], al
        call   sub_2_467F
loc_2_48A4:
        cmp    dword ptr [0], 0              ; -> obj3:0x0CC4
        mov    al, byte ptr [ebp - 1]
        je     loc_2_48FA
        and    al, 0x78
        shr    al, 3
        mov    edx, 2
        movzx  cx, al
        sub    cx, 0x40
        movzx  ax, byte ptr [ebp + 8]
        shl    cx, 8
        and    ax, 0xfffc
        add    cx, ax
        call   sub_2_46A4
        mov    edi, eax
        mov    al, byte ptr [ebp + 8]
        and    al, 3
        movzx  ebx, byte ptr [ebp + 0xc]
        lea    edx, [eax*8]
        mov    eax, dword ptr [ebx*4]        ; -> loc_3_0CDC
        mov    cl, dl
        shl    eax, cl
        and    edi, eax
        shr    edi, cl
        jmp    loc_2_491D
loc_2_48FA:
        and    al, 0x78
        mov    edx, dword ptr [ebp + 0xc]
        shr    al, 3
        movzx  cx, al
        sub    cx, 0x40
        movzx  ax, byte ptr [ebp + 8]
        shl    cx, 8
        add    cx, ax
        call   sub_2_46A4
        mov    edi, eax
loc_2_491D:
        cmp    dword ptr [0], 0              ; -> loc_3_0CD4
        jne    loc_2_4936
        push   0
        mov    ecx, 0xcfa
        movzx  edx, byte ptr [ebp - 3]
        call   sub_2_467F
loc_2_4936:
        push   0
        mov    ecx, 0xcf8
        movzx  edx, byte ptr [ebp - 2]
        jmp    loc_2_4A26
loc_2_4946:
        cmp    dword ptr [0], 0              ; -> loc_3_0CD8
        je     loc_2_495E
        xor    edx, edx
        mov    ecx, 0xcfb
        call   sub_2_46A4
        mov    byte ptr [ebp - 2], al
loc_2_495E:
        mov    edx, 2
        mov    ecx, 0xcf8
        call   sub_2_46A4
        cmp    dword ptr [0], 0              ; -> loc_3_0CD8
        mov    esi, eax
        je     loc_2_4989
        push   0
        mov    edx, 1
        mov    ecx, 0xcfb
        call   sub_2_467F
loc_2_4989:
        movzx  edx, bl
        movzx  eax, byte ptr [ebp - 1]
        shl    eax, 8
        push   2
        or     edx, 0xffff8000
        mov    ecx, 0xcf8
        shl    edx, 0x10
        or     edx, eax
        movzx  eax, byte ptr [ebp + 8]
        and    al, 0xfc
        or     edx, eax
        call   sub_2_467F
        cmp    dword ptr [0], 0              ; -> obj3:0x0CC4
        je     loc_2_49EB
        mov    edx, 2
        mov    ecx, 0xcfc
        call   sub_2_46A4
        movzx  edx, byte ptr [ebp + 0xc]
        mov    edi, eax
        mov    al, byte ptr [ebp + 8]
        and    al, 3
        mov    edx, dword ptr [edx*4]        ; -> loc_3_0CDC
        lea    ecx, [eax*8]
        shl    edx, cl
        and    edi, edx
        shr    edi, cl
        jmp    loc_2_4A04
loc_2_49EB:
        xor    cx, cx
        mov    edx, dword ptr [ebp + 0xc]
        mov    cl, byte ptr [ebp + 8]
        and    cx, 3
        add    cx, 0xcfc
        call   sub_2_46A4
        mov    edi, eax
loc_2_4A04:
        push   2
        mov    edx, esi
        mov    ecx, 0xcf8
        call   sub_2_467F
        cmp    dword ptr [0], 0              ; -> loc_3_0CD8
        je     loc_2_4A2B
        push   0
        mov    ecx, 0xcfb
        movzx  edx, byte ptr [ebp - 2]
loc_2_4A26:
        call   sub_2_467F
loc_2_4A2B:
        in     al, 0x21
loc_2_4A2D:
        call   sub_2_464D
        mov    eax, edi
loc_2_4A34:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
sub_2_4A3D:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x1c
        cmp    dword ptr [0], 0              ; -> loc_3_0CD4
        push   ebx
        mov    byte ptr [ebp - 1], dl
        push   esi
        mov    bl, cl
        push   edi
        je     loc_2_4A5C
        test   bl, bl
        jne    loc_2_4CAF
loc_2_4A5C:
        call   sub_2_4614
        test   byte ptr [0], 1               ; -> obj3:0x0CB8
        je     loc_2_4A9F
        movzx  eax, byte ptr [ebp + 0x10]
        add    eax, 0xb10b
        mov    edx, dword ptr [ebp + 0xc]
        mov    dword ptr [ebp - 0x1c], eax
        mov    dword ptr [ebp - 0x14], edx
        movzx  ecx, byte ptr [ebp - 1]
        movzx  eax, bl
        shl    eax, 8
        or     eax, ecx
        movzx  ecx, byte ptr [ebp + 8]
        mov    dword ptr [ebp - 0x18], eax
        mov    dword ptr [ebp - 8], ecx
        lea    ecx, [ebp - 0x1c]
        call   sub_2_46CF
        jmp    loc_2_4CAA
loc_2_4A9F:
        mov    byte ptr [ebp - 2], 0
        mov    byte ptr [ebp - 3], 0
        in     al, 0x21
        xor    esi, esi
        cmp    dword ptr [0], esi            ; -> obj3:0x0CC0
        jne    loc_2_4BB6
        xor    edx, edx
        mov    edi, 0xcf8
        mov    ecx, edi
        call   sub_2_46A4
        push   esi
        mov    byte ptr [ebp - 2], al
        mov    al, byte ptr [ebp - 1]
        and    al, 7
        or     al, 0x78
        movzx  ecx, al
        lea    edx, [ecx*2]
        mov    ecx, edi
        call   sub_2_467F
        cmp    dword ptr [0], esi            ; -> loc_3_0CD4
        jne    loc_2_4B05
        xor    edx, edx
        mov    edi, 0xcfa
        mov    ecx, edi
        call   sub_2_46A4
        push   esi
        mov    ecx, edi
        movzx  edx, bl
        mov    byte ptr [ebp - 3], al
        call   sub_2_467F
loc_2_4B05:
        cmp    dword ptr [0], 0              ; -> obj3:0x0CC4
        je     loc_2_4B66
        mov    al, byte ptr [ebp - 1]
        and    al, 0x78
        shr    al, 3
        mov    edx, 2
        movzx  si, al
        sub    si, 0x40
        movzx  ax, byte ptr [ebp + 8]
        shl    si, 8
        and    ax, 0xfffc
        add    si, ax
        mov    ecx, esi
        call   sub_2_46A4
        movzx  ebx, byte ptr [ebp + 0x10]
        mov    dl, byte ptr [ebp + 8]
        mov    edi, dword ptr [ebx*4]        ; -> loc_3_0CDC
        and    dl, 3
        push   2
        lea    ecx, [edx*8]
        shl    edi, cl
        not    edi
        shl    dword ptr [ebp + 0xc], cl
        and    eax, edi
        mov    ecx, esi
        or     eax, dword ptr [ebp + 0xc]
        mov    edx, eax
        jmp    loc_2_4B88
loc_2_4B66:
        mov    al, byte ptr [ebp - 1]
        and    al, 0x78
        mov    edx, dword ptr [ebp + 0xc]
        shr    al, 3
        push   dword ptr [ebp + 0x10]
        movzx  cx, al
        sub    cx, 0x40
        movzx  ax, byte ptr [ebp + 8]
        shl    cx, 8
        add    cx, ax
loc_2_4B88:
        call   sub_2_467F
        cmp    dword ptr [0], 0              ; -> loc_3_0CD4
        jne    loc_2_4BA6
        push   0
        mov    ecx, 0xcfa
        movzx  edx, byte ptr [ebp - 3]
        call   sub_2_467F
loc_2_4BA6:
        push   0
        mov    ecx, 0xcf8
        movzx  edx, byte ptr [ebp - 2]
        jmp    loc_2_4CA3
loc_2_4BB6:
        cmp    dword ptr [0], 0              ; -> loc_3_0CD8
        je     loc_2_4BCE
        xor    edx, edx
        mov    ecx, 0xcfb
        call   sub_2_46A4
        mov    byte ptr [ebp - 2], al
loc_2_4BCE:
        mov    edx, 2
        mov    ecx, 0xcf8
        call   sub_2_46A4
        cmp    dword ptr [0], 0              ; -> loc_3_0CD8
        mov    esi, eax
        je     loc_2_4BF9
        push   0
        mov    edx, 1
        mov    ecx, 0xcfb
        call   sub_2_467F
loc_2_4BF9:
        movzx  edx, bl
        movzx  eax, byte ptr [ebp - 1]
        shl    eax, 8
        push   2
        or     edx, 0xffff8000
        mov    ecx, 0xcf8
        shl    edx, 0x10
        or     edx, eax
        movzx  eax, byte ptr [ebp + 8]
        and    al, 0xfc
        or     edx, eax
        call   sub_2_467F
        cmp    dword ptr [0], 0              ; -> obj3:0x0CC4
        je     loc_2_4C67
        mov    edx, 2
        mov    edi, 0xcfc
        mov    ecx, edi
        call   sub_2_46A4
        movzx  ebx, byte ptr [ebp + 0x10]
        mov    edx, eax
        mov    al, byte ptr [ebp + 8]
        and    al, 3
        mov    ebx, dword ptr [ebx*4]        ; -> loc_3_0CDC
        push   2
        lea    ecx, [eax*8]
        shl    ebx, cl
        not    ebx
        shl    dword ptr [ebp + 0xc], cl
        and    edx, ebx
        mov    ecx, edi
        or     edx, dword ptr [ebp + 0xc]
        jmp    loc_2_4C7C
loc_2_4C67:
        xor    cx, cx
        mov    edx, dword ptr [ebp + 0xc]
        mov    cl, byte ptr [ebp + 8]
        push   dword ptr [ebp + 0x10]
        and    cx, 3
        add    cx, 0xcfc
loc_2_4C7C:
        call   sub_2_467F
        push   2
        mov    edx, esi
        mov    ecx, 0xcf8
        call   sub_2_467F
        cmp    dword ptr [0], 0              ; -> loc_3_0CD8
        je     loc_2_4CA8
        push   0
        mov    ecx, 0xcfb
        movzx  edx, byte ptr [ebp - 2]
loc_2_4CA3:
        call   sub_2_467F
loc_2_4CA8:
        in     al, 0x21
loc_2_4CAA:
        call   sub_2_464D
loc_2_4CAF:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_2_4CB8:
        mov    eax, 0x400
        ret    
loc_2_4CBE:
        mov    edx, dword ptr [esp + 8]
        push   2
        push   dword ptr [esp + 0x10]
        mov    ecx, dword ptr [esp + 0xc]
        call   sub_2_47CB
        ret    
loc_2_4CD2:
        push   ebp
        mov    edx, dword ptr [esp + 0xc]
        mov    ebp, esp
        push   2
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp + 8]
        call   sub_2_4A3D
        pop    ebp
        ret    
sub_2_4CEB:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        lea    eax, [ebp - 1]
        push   esi
        mov    bl, cl
        mov    byte ptr [ebp - 2], dl
        push   dword ptr [ebp - 2]
        push   eax
        call   sub_3_0358
        add    esp, 8
        test   eax, eax
        je     loc_2_4D13
        xor    eax, eax
        jmp    loc_2_4DD1
loc_2_4D13:
        lea    eax, [ebp - 8]
        push   eax
        call   sub_3_0360
        add    esp, 4
        test   eax, eax
        je     loc_2_4D2B
        xor    eax, eax
        jmp    loc_2_4DD1
loc_2_4D2B:
        cmp    byte ptr [ebp - 1], 0
        je     loc_2_4D44
        mov    eax, 1
        mov    cl, byte ptr [ebp - 1]
        shl    eax, cl
        test   dword ptr [ebp - 8], eax
        jne    loc_2_4D44
        mov    byte ptr [ebp - 1], 0
loc_2_4D44:
        cmp    byte ptr [ebp - 1], bl
        mov    esi, 0
        test   bl, bl
        je     loc_2_4D59
        mov    esi, 1
        mov    cl, bl
        shl    esi, cl
loc_2_4D59:
        cmp    byte ptr [ebp - 1], 0
        mov    eax, 0
        je     loc_2_4D6E
        mov    eax, 1
        mov    cl, byte ptr [ebp - 1]
        shl    eax, cl
loc_2_4D6E:
        or     esi, eax
        call   sub_2_4614
        in     al, 0xa1
        mov    ah, al
        in     al, 0x21
        mov    word ptr [ebp - 4], ax
        or     al, 0xff
        out    0x21, al
        out    0xa1, al
        mov    eax, esi
        not    eax
        and    dword ptr [ebp - 8], eax
        push   dword ptr [ebp - 8]
        call   sub_3_035C
        add    esp, 4
        push   dword ptr [ebp - 2]
        push   ebx
        call   sub_3_0354
        add    esp, 8
        push   esi
        call   sub_2_7A98
        add    esp, 4
        or     dword ptr [ebp - 8], esi
        push   dword ptr [ebp - 8]
        call   sub_3_035C
        add    esp, 4
        mov    ax, word ptr [ebp - 4]
        out    0x21, al
        mov    al, ah
        out    0xa1, al
        call   sub_2_464D
        mov    eax, 1
loc_2_4DD1:
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_4DD7:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        mov    byte ptr [ebp - 2], dl
        push   esi
        mov    byte ptr [ebp - 1], cl
        call   sub_2_4614
        mov    esi, dword ptr [ebp + 8]
        mov    edx, dword ptr [ebp - 2]
        mov    ecx, dword ptr [ebp - 1]
        mov    bl, byte ptr [esi + 0x26]
        push   ebx
        mov    al, byte ptr [esi + 0x24]
        push   eax
        call   sub_2_47CB
        mov    edx, dword ptr [ebp + 0xc]
        mov    cl, byte ptr [esi + 0x27]
        push   ebx
        mov    dword ptr [edx], eax
        shr    eax, cl
        mov    cl, byte ptr [esi + 0x28]
        mov    dword ptr [edx], eax
        shl    eax, cl
        mov    cl, byte ptr [esi + 0x25]
        mov    dword ptr [edx], eax
        push   ecx
        mov    edx, dword ptr [ebp - 2]
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_47CB
        mov    edx, dword ptr [ebp + 0x10]
        mov    cl, byte ptr [esi + 0x27]
        mov    dword ptr [edx], eax
        shr    eax, cl
        mov    cl, byte ptr [esi + 0x28]
        mov    dword ptr [edx], eax
        shl    eax, cl
        mov    dword ptr [edx], eax
        call   sub_2_464D
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
sub_2_4E43:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        mov    byte ptr [ebp - 2], dl
        push   esi
        mov    byte ptr [ebp - 1], cl
        call   sub_2_4614
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0xc]
        mov    bl, byte ptr [esi + 0x26]
        mov    cl, byte ptr [esi + 0x28]
        shr    eax, cl
        push   ebx
        mov    cl, byte ptr [esi + 0x27]
        shl    eax, cl
        push   eax
        mov    cl, byte ptr [esi + 0x24]
        push   ecx
        mov    edx, dword ptr [ebp - 2]
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_4A3D
        push   ebx
        mov    eax, dword ptr [ebp + 0x10]
        mov    cl, byte ptr [esi + 0x28]
        mov    edx, dword ptr [ebp - 2]
        shr    eax, cl
        mov    cl, byte ptr [esi + 0x27]
        shl    eax, cl
        push   eax
        mov    cl, byte ptr [esi + 0x25]
        push   ecx
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_4A3D
        call   sub_2_464D
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
sub_2_4EA5:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        mov    byte ptr [ebp - 1], dl
        push   esi
        mov    bl, cl
        push   4
        mov    edx, dword ptr [ebp - 1]
        mov    ecx, ebx
        call   sub_2_58D3
        mov    si, ax
        mov    cl, byte ptr [ebp + 8]
        test   cl, cl
        je     loc_2_4ED5
        mov    ax, 1
        shl    ax, cl
        mov    word ptr [ebp - 4], ax
        jmp    loc_2_4EDB
loc_2_4ED5:
        mov    word ptr [ebp - 4], 0
loc_2_4EDB:
        call   sub_2_4614
        in     al, 0xa1
        mov    ah, al
        in     al, 0x21
        push   eax
        or     ax, word ptr [ebp - 4]
        out    0x21, al
        mov    al, ah
        out    0xa1, al
        or     si, word ptr [ebp + 0xc]
        push   2
        movzx  eax, si
        push   eax
        mov    edx, dword ptr [ebp - 1]
        push   4
        mov    ecx, ebx
        call   sub_2_4A3D
        pop    eax
        out    0x21, al
        mov    al, ah
        out    0xa1, al
        call   sub_2_464D
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
sub_2_4F1B:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, ecx
        mov    edi, edx
        push   2
        mov    edx, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        push   dword ptr [ebp + 0x10]
        call   sub_2_47CB
        test   eax, 0xfffffff0
        mov    ebx, eax
        jne    loc_2_4F4B
        cmp    dword ptr [ebp + 0x14], 0
        je     loc_2_4F4B
        xor    eax, eax
        jmp    loc_2_4FA5
loc_2_4F4B:
        call   sub_2_4614
        push   2
        mov    edx, dword ptr [ebp + 0xc]
        push   -1
        mov    ecx, dword ptr [ebp + 8]
        push   dword ptr [ebp + 0x10]
        call   sub_2_4A3D
        push   2
        mov    edx, dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp + 8]
        call   sub_2_47CB
        push   2
        mov    edx, dword ptr [ebp + 0xc]
        push   ebx
        mov    ecx, dword ptr [ebp + 8]
        push   dword ptr [ebp + 0x10]
        mov    dword ptr [ebp - 4], eax
        call   sub_2_4A3D
        call   sub_2_464D
        test   esi, esi
        je     loc_2_4F91
        mov    dword ptr [esi], ebx
loc_2_4F91:
        test   edi, edi
        je     loc_2_4F9A
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [edi], eax
loc_2_4F9A:
        mov    eax, dword ptr [ebp - 4]
        and    al, 0xf0
        cmp    eax, 1
        sbb    eax, eax
        inc    eax
loc_2_4FA5:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
sub_2_4FAE:
        call   sub_2_406B
        movzx  eax, al
        test   eax, eax
        je     loc_2_4FC3
        cmp    eax, 1
        je     loc_2_4FC7
        xor    al, al
        jmp    loc_2_4FC9
loc_2_4FC3:
        mov    al, 0x30
        jmp    loc_2_4FC9
loc_2_4FC7:
        mov    al, 0x38
loc_2_4FC9:
        ret    
sub_2_4FCA:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        mov    byte ptr [ebp - 2], dl
        push   esi
        mov    edx, dword ptr [ebp - 2]
        push   edi
        mov    byte ptr [ebp - 1], cl
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_4FAE
        mov    bl, al
        mov    eax, 0
        test   bl, bl
        je     loc_2_5047
        call   sub_2_4614
        push   2
        mov    edx, dword ptr [ebp - 2]
        push   ebx
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_47CB
        mov    esi, eax
        push   2
        or     eax, 0xfffff800
        mov    edx, dword ptr [ebp - 2]
        push   eax
        mov    ecx, dword ptr [ebp - 1]
        push   ebx
        call   sub_2_4A3D
        push   2
        mov    edx, dword ptr [ebp - 2]
        push   ebx
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_47CB
        mov    edi, eax
        push   2
        push   esi
        mov    edx, dword ptr [ebp - 2]
        push   ebx
        mov    ecx, dword ptr [ebp - 1]
        not    edi
        or     edi, 0x7ff
        call   sub_2_4A3D
        call   sub_2_464D
        lea    eax, [edi + 1]
loc_2_5047:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_504E:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x1c
        push   ebx
        mov    dword ptr [ebp - 0x14], edx
        push   esi
        xor    ebx, ebx
        push   edi
        mov    esi, ecx
        push   ebx
        lea    eax, [ebp - 2]
        push   ebx
        lea    edx, [ebp - 1]
        push   eax
        call   sub_2_4212
        mov    edx, dword ptr [ebp - 2]
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_4FAE
        mov    byte ptr [ebp - 3], al
        cmp    al, bl
        jne    loc_2_5088
        mov    eax, 0x13
        jmp    loc_2_524D
loc_2_5088:
        push   2
        lea    eax, [ebp - 0x1c]
        push   esi
        push   eax
        call   sub_2_7918
        add    esp, 0xc
        test   eax, eax
        je     loc_2_50A5
        mov    eax, 0x13
        jmp    loc_2_524D
loc_2_50A5:
        xor    esi, esi
        mov    eax, dword ptr [ebp - 0x1c]
        mov    dword ptr [ebp - 0x10], eax
loc_2_50AD:
        push   0
        lea    ecx, [ebp - 0x10]
        push   0
        push   1
        push   dword ptr [ebp - 0x10]
        push   ecx
        call   sub_2_7908
        add    esp, 0x14
        test   eax, eax
        jne    loc_2_50DC
        mov    ecx, dword ptr [ebp - 0x10]
        cmp    dword ptr [ecx + 0x24], 0xfc
        jne    loc_2_50AD
        mov    esi, dword ptr [ecx + 4]
        mov    ebx, dword ptr [ecx + 0x14]
        test   eax, eax
        je     loc_2_50E6
loc_2_50DC:
        mov    eax, 0x13
        jmp    loc_2_524D
loc_2_50E6:
        cmp    dword ptr [ebp - 0x14], ebx
        jbe    loc_2_50F5
        mov    eax, 0x13
        jmp    loc_2_524D
loc_2_50F5:
        mov    eax, ebx
        sub    eax, dword ptr [ebp - 0x14]
        cmp    eax, dword ptr [ebp + 0xc]
        jae    loc_2_5109
        mov    eax, 0x13
        jmp    loc_2_524D
loc_2_5109:
        cmp    dword ptr [ebp + 0xc], 0
        jne    loc_2_5116
        xor    eax, eax
        jmp    loc_2_524D
loc_2_5116:
        add    ebx, 0xfff
        push   0
        shr    ebx, 0xc
        push   ebx
        push   0x80080000
        call   sub_2_7AB8
        add    esp, 0xc
        mov    dword ptr [ebp - 0xc], eax
        cmp    eax, -1
        je     loc_2_5235
        mov    edi, eax
        push   0x40000000
        shr    edi, 0xc
        mov    eax, esi
        shr    eax, 0xc
        push   eax
        push   ebx
        push   edi
        call   sub_2_7AB0
        add    esp, 0x10
        test   eax, eax
        je     loc_2_5235
        push   0
        push   ebx
        push   edi
        call   sub_2_7AA8
        add    esp, 0xc
        test   eax, eax
        je     loc_2_5235
        call   sub_2_4614
        push   2
        mov    edx, dword ptr [ebp - 2]
        push   dword ptr [ebp - 3]
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_47CB
        push   1
        mov    edx, dword ptr [ebp - 2]
        push   4
        mov    ecx, dword ptr [ebp - 1]
        or     esi, 1
        mov    dword ptr [ebp - 0x18], eax
        call   sub_2_47CB
        mov    word ptr [ebp - 6], ax
        push   2
        push   esi
        mov    edx, dword ptr [ebp - 2]
        push   dword ptr [ebp - 3]
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_4A3D
        mov    bx, word ptr [ebp - 6]
        and    bx, 2
        jne    loc_2_51D3
        mov    ax, word ptr [ebp - 6]
        push   2
        or     al, 2
        mov    edx, dword ptr [ebp - 2]
        movzx  ecx, ax
        push   ecx
        push   4
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_4A3D
loc_2_51D3:
        cmp    dword ptr [ebp + 0xc], 0
        je     loc_2_51F3
        mov    eax, dword ptr [ebp + 0xc]
        mov    esi, dword ptr [ebp - 0x14]
        add    esi, dword ptr [ebp - 0xc]
        mov    edi, dword ptr [ebp + 8]
        mov    ecx, eax
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_2_51F3:
        test   bx, bx
        jne    loc_2_520C
        push   2
        mov    edx, dword ptr [ebp - 2]
        movzx  eax, word ptr [ebp - 6]
        push   eax
        mov    ecx, dword ptr [ebp - 1]
        push   4
        call   sub_2_4A3D
loc_2_520C:
        push   2
        mov    edx, dword ptr [ebp - 2]
        push   dword ptr [ebp - 0x18]
        push   dword ptr [ebp - 3]
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_4A3D
        call   sub_2_464D
        push   0
        push   dword ptr [ebp - 0xc]
        call   sub_2_7AA0
        add    esp, 8
        xor    eax, eax
        jmp    loc_2_524D
loc_2_5235:
        cmp    dword ptr [ebp - 0xc], -1
        je     loc_2_5248
        push   0
        push   dword ptr [ebp - 0xc]
        call   sub_2_7AA0
        add    esp, 8
loc_2_5248:
        mov    eax, 2
loc_2_524D:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    8
sub_2_5256:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        mov    byte ptr [ebp - 1], dl
        push   esi
        mov    bl, cl
        mov    edx, dword ptr [ebp - 1]
        mov    ecx, ebx
        call   sub_2_406B
        movzx  eax, al
        cmp    eax, 1
        je     loc_2_527E
        cmp    eax, 2
        je     loc_2_52F4
        xor    eax, eax
        jmp    loc_2_52F9
loc_2_527E:
        mov    edx, dword ptr [ebp - 1]
        mov    ecx, ebx
        call   sub_2_4075
        test   eax, 0x40000
        je     loc_2_5293
        xor    eax, eax
        jmp    loc_2_52F9
loc_2_5293:
        call   sub_2_4614
        push   0
        mov    edx, dword ptr [ebp - 1]
        push   0x1c
        mov    ecx, ebx
        call   sub_2_47CB
        test   al, al
        je     loc_2_52B0
        mov    byte ptr [ebp - 2], 0xf0
        jmp    loc_2_52E2
loc_2_52B0:
        xor    esi, esi
        mov    edx, dword ptr [ebp - 1]
        push   esi
        mov    ecx, ebx
        push   0xf0
        push   0x1c
        call   sub_2_4A3D
        push   esi
        mov    edx, dword ptr [ebp - 1]
        push   0x1c
        mov    ecx, ebx
        call   sub_2_47CB
        push   esi
        mov    edx, dword ptr [ebp - 1]
        push   esi
        mov    ecx, ebx
        push   0x1c
        mov    byte ptr [ebp - 2], al
        call   sub_2_4A3D
loc_2_52E2:
        call   sub_2_464D
        mov    al, byte ptr [ebp - 2]
        sub    al, 0xf0
        cmp    al, 1
        sbb    eax, eax
        neg    eax
        jmp    loc_2_52F9
loc_2_52F4:
        mov    eax, 1
loc_2_52F9:
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_52FF:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        mov    byte ptr [ebp - 1], dl
        push   esi
        mov    bl, byte ptr [ebp + 8]
        push   edi
        mov    edx, ebx
        push   0
        mov    byte ptr [ebp - 2], cl
        mov    ecx, dword ptr [ebp - 2]
        call   sub_2_58E1
        mov    edi, eax
        push   0
        mov    edx, ebx
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_58E1
        cmp    edi, eax
        je     loc_2_5336
        xor    eax, eax
        jmp    loc_2_5459
loc_2_5336:
        mov    edx, ebx
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_406B
        test   al, al
        je     loc_2_534C
        cmp    al, 1
        je     loc_2_534C
        cmp    al, 2
        jne    loc_2_53C8
loc_2_534C:
        push   0x3c
        mov    edx, ebx
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_58C5
        push   0x3c
        mov    edx, ebx
        mov    ecx, dword ptr [ebp - 2]
        mov    byte ptr [ebp - 5], al
        call   sub_2_58C5
        cmp    al, byte ptr [ebp - 5]
        je     loc_2_5373
        xor    eax, eax
        jmp    loc_2_5459
loc_2_5373:
        mov    al, byte ptr [ebp - 5]
        mov    edx, ebx
        not    al
        push   eax
        mov    ecx, dword ptr [ebp - 1]
        push   0x3c
        call   sub_2_58EF
        push   0x3c
        mov    edx, ebx
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_58C5
        mov    byte ptr [ebp - 3], al
        cmp    al, byte ptr [ebp - 5]
        je     loc_2_53C8
        push   0x3c
        mov    edx, ebx
        mov    ecx, dword ptr [ebp - 2]
        call   sub_2_58C5
        mov    edx, ebx
        mov    ecx, dword ptr [ebp - 1]
        push   dword ptr [ebp - 5]
        push   0x3c
        mov    byte ptr [ebp - 2], al
        call   sub_2_58EF
        mov    al, byte ptr [ebp - 2]
        sub    al, byte ptr [ebp - 3]
        cmp    al, 1
        sbb    eax, eax
        neg    eax
        jmp    loc_2_5459
loc_2_53C8:
        xor    esi, esi
        call   sub_2_4614
        push   1
        mov    edx, ebx
        push   4
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_47CB
        mov    di, ax
        push   1
        push   4
        mov    edx, ebx
        mov    ecx, dword ptr [ebp - 2]
        call   sub_2_47CB
        cmp    ax, di
        jne    loc_2_5452
        mov    ax, di
        push   2
        xor    ax, 0x30b
        mov    edx, ebx
        movzx  ecx, ax
        push   ecx
        push   4
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_4A3D
        push   1
        mov    edx, ebx
        push   4
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_47CB
        mov    word ptr [ebp - 6], ax
        cmp    ax, di
        je     loc_2_5452
        push   1
        mov    edx, ebx
        push   4
        mov    ecx, dword ptr [ebp - 2]
        call   sub_2_47CB
        mov    si, ax
        push   2
        mov    edx, ebx
        mov    ecx, dword ptr [ebp - 1]
        movzx  eax, di
        push   eax
        push   4
        call   sub_2_4A3D
        sub    si, word ptr [ebp - 6]
        cmp    si, 1
        sbb    esi, esi
        neg    esi
loc_2_5452:
        call   sub_2_464D
        mov    eax, esi
loc_2_5459:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
sub_2_5462:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        call   sub_2_4614
        xor    bl, bl
loc_2_5470:
        xor    ecx, ecx
        xor    edx, edx
        VxDCall  BLOCKDEV.Service_000Dh          ; cd200d001000
        mov    dword ptr [ebp - 4], eax
        cmp    dword ptr [ebp - 4], 0
        je     loc_2_549B
        sti    
        push   0
        push   0x2710
        call   sub_2_7AC0
        add    esp, 8
        cli    
        inc    bl
        cmp    bl, 0x64
        jb     loc_2_5470
loc_2_549B:
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_54A0:
        push   ebx
        lea    eax, [ecx + 7]
        push   esi
        push   edi
        mov    word ptr [edx], ax
        push   ebp
        mov    esi, edx
        xor    edi, edi
        xor    ebp, ebp
loc_2_54B0:
        movzx  edx, cx
        lea    eax, [edx + 7]
        mov    edx, eax
        in     al, dx
        movzx  edx, cx
        mov    bl, al
        add    edx, 0x206
        in     al, dx
        cmp    al, bl
        jne    loc_2_54D5
        cmp    edi, 1
        je     loc_2_54E2
        mov    edi, 1
        jmp    loc_2_54D7
loc_2_54D5:
        xor    edi, edi
loc_2_54D7:
        inc    ebp
        cmp    ebp, 0x1388
        jb     loc_2_54B0
        jmp    loc_2_54EA
loc_2_54E2:
        add    cx, 0x206
        mov    word ptr [esi], cx
loc_2_54EA:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_2_54EF:
        push   esi
        push   edi
        mov    si, cx
        xor    edi, edi
loc_2_54F6:
        mov    dx, si
        in     al, dx
        test   al, 0x80
        je     loc_2_551A
        push   0
        add    edi, 0x64
        push   0x64
        call   sub_2_7AC0
        add    esp, 8
        cmp    edi, 0x1c9c380
        jb     loc_2_54F6
        xor    eax, eax
loc_2_5517:
        pop    edi
        pop    esi
        ret    
loc_2_551A:
        mov    eax, 1
        jmp    loc_2_5517
sub_2_5521:
        push   esi
        push   edi
        mov    si, cx
        xor    edi, edi
loc_2_5528:
        mov    dx, si
        in     al, dx
        test   al, 8
        jne    loc_2_554C
        push   0
        add    edi, 0x64
        push   0x64
        call   sub_2_7AC0
        add    esp, 8
        cmp    edi, 0xf4240
        jb     loc_2_5528
        xor    eax, eax
loc_2_5549:
        pop    edi
        pop    esi
        ret    
loc_2_554C:
        mov    eax, 1
        jmp    loc_2_5549
sub_2_5553:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x204
        mov    word ptr [ebp - 4], cx
        push   esi
        push   edi
        lea    edx, [ebp - 2]
        xor    edi, edi
        mov    ecx, dword ptr [ebp - 4]
        call   sub_2_54A0
        mov    ecx, dword ptr [ebp - 2]
        call   sub_2_54EF
        test   eax, eax
        je     loc_2_55CA
        mov    al, 0xec
        mov    ecx, dword ptr [ebp - 2]
        movzx  esi, word ptr [ebp - 4]
        add    esi, 7
        mov    edx, esi
        out    dx, al
        call   sub_2_54EF
        test   eax, eax
        je     loc_2_55CA
        mov    dx, word ptr [ebp - 2]
        in     al, dx
        test   al, 8
        jne    loc_2_55AC
        test   al, 1
        jne    loc_2_55B1
        mov    ecx, dword ptr [ebp - 2]
        call   sub_2_5521
        test   eax, eax
        je     loc_2_55B1
loc_2_55AC:
        mov    edi, 1
loc_2_55B1:
        test   edi, edi
        je     loc_2_55CA
        mov    edx, esi
        in     al, dx
        mov    ecx, 0x100
        movzx  edx, word ptr [ebp - 4]
        lea    edi, [ebp - 0x204]
        rep insw word ptr es:[edi], dx
loc_2_55CA:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_55D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        mov    word ptr [ebp - 6], cx
        push   ebx
        push   esi
        mov    byte ptr [ebp - 1], dl
        push   edi
        xor    esi, esi
        mov    dword ptr [ebp - 0x14], esi
        call   sub_2_5462
        mov    byte ptr [ebp - 2], 0
        mov    byte ptr [ebp - 0x10], 0xa0
        movzx  ecx, word ptr [ebp - 6]
        add    ecx, 6
        mov    edx, ecx
        mov    dword ptr [ebp - 0xc], ecx
        in     al, dx
        movzx  ecx, word ptr [ebp - 6]
        mov    byte ptr [ebp - 3], al
        lea    edi, [ecx + 4]
loc_2_5609:
        mov    edx, dword ptr [ebp - 0xc]
        mov    al, byte ptr [ebp - 0x10]
        out    dx, al
        mov    edx, edi
        push   0
        in     al, dx
        push   0x64
        mov    byte ptr [ebp - 4], al
        xor    al, al
        out    dx, al
        mov    al, 0x55
        out    dx, al
        call   sub_2_7AC0
        add    esp, 8
        mov    edx, edi
        in     al, dx
        cmp    al, 0x55
        jne    loc_2_56B5
        mov    al, 0xaa
        push   0
        out    dx, al
        push   0x64
        call   sub_2_7AC0
        add    esp, 8
        mov    edx, edi
        in     al, dx
        cmp    al, 0xaa
        jne    loc_2_56B5
        push   0
        mov    edx, dword ptr [ebp + 8]
        push   dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_47CB
        push   0
        mov    edx, dword ptr [ebp + 8]
        push   dword ptr [ebp + 0xc]
        movzx  ebx, byte ptr [ebp + 0x10]
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_47CB
        test   ebx, eax
        jne    loc_2_56B5
        mov    dword ptr [ebp - 0x14], 1
        mov    ecx, dword ptr [ebp - 6]
        call   sub_2_5553
        push   0
        mov    edx, dword ptr [ebp + 8]
        push   dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_47CB
        and    eax, ebx
        cmp    eax, 1
        sbb    esi, esi
        inc    esi
        je     loc_2_56B5
        mov    eax, 1
        mov    cl, byte ptr [ebp + 0x14]
        shl    eax, cl
        push   eax
        call   sub_2_7A98
        movzx  ecx, word ptr [ebp - 6]
        add    esp, 4
        lea    edx, [ecx + 7]
        in     al, dx
loc_2_56B5:
        mov    edx, edi
        mov    al, byte ptr [ebp - 4]
        out    dx, al
        push   0
        push   0x64
        call   sub_2_7AC0
        add    esp, 8
        cmp    dword ptr [ebp - 0x14], 0
        jne    loc_2_56E3
        movzx  eax, byte ptr [ebp - 0x10]
        add    al, 0x10
        inc    byte ptr [ebp - 2]
        cmp    byte ptr [ebp - 2], 2
        mov    byte ptr [ebp - 0x10], al
        jb     loc_2_5609
loc_2_56E3:
        mov    edx, dword ptr [ebp - 0xc]
        mov    al, byte ptr [ebp - 3]
        out    dx, al
        call   sub_2_464D
        mov    eax, esi
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
sub_2_56FA:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        mov    byte ptr [ebp - 2], cl
        push   esi
        xor    bl, bl
        mov    byte ptr [ebp - 1], 0
        push   edi
        call   sub_2_4614
        mov    al, byte ptr [ebp - 2]
        xor    esi, esi
        and    al, 0x78
        mov    edi, 0xcf8
        shr    al, 3
        movzx  ax, al
        shl    ax, 8
        mov    word ptr [ebp - 0x10], ax
loc_2_572C:
        push   2
        mov    edx, dword ptr [ebp - 2]
        push   ebx
        xor    ecx, ecx
        call   sub_2_47CB
        xor    edx, edx
        mov    ecx, edi
        mov    dword ptr [ebp - 0xc], eax
        call   sub_2_46A4
        push   esi
        xor    edx, edx
        mov    ecx, edi
        mov    byte ptr [ebp - 3], al
        call   sub_2_467F
        cmp    dword ptr [0], esi            ; -> loc_3_0CD4
        jne    loc_2_5776
        xor    edx, edx
        mov    ecx, 0xcfa
        call   sub_2_46A4
        push   esi
        xor    edx, edx
        mov    ecx, 0xcfa
        mov    byte ptr [ebp - 1], al
        call   sub_2_467F
loc_2_5776:
        movzx  cx, bl
        add    cx, word ptr [ebp - 0x10]
        mov    edx, 2
        sub    cx, 0x4000
        call   sub_2_46A4
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [0], esi            ; -> loc_3_0CD4
        jne    loc_2_57A7
        push   esi
        mov    ecx, 0xcfa
        movzx  edx, byte ptr [ebp - 1]
        call   sub_2_467F
loc_2_57A7:
        push   esi
        mov    ecx, edi
        movzx  edx, byte ptr [ebp - 3]
        call   sub_2_467F
        mov    eax, dword ptr [ebp - 0xc]
        cmp    dword ptr [ebp - 8], eax
        jne    loc_2_57C7
        add    bl, 4
        cmp    bl, 0x10
        jb     loc_2_572C
loc_2_57C7:
        sub    bl, 0x10
        call   sub_2_464D
        pop    edi
        cmp    bl, 1
        sbb    eax, eax
        pop    esi
        inc    eax
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_57DC:
        nop    
        ret    
sub_2_57DE:
        sub    esp, 4
        mov    byte ptr [esp + 3], dl
        push   ebx
        mov    bl, cl
        push   9
        mov    edx, dword ptr [esp + 0xb]
        mov    ecx, ebx
        call   sub_2_58C5
        test   al, 1
        mov    eax, 1
        jne    loc_2_5814
        push   0xe
        mov    edx, ebx
        push   4
        mov    ecx, 0x1f0
        push   0x50
        push   dword ptr [esp + 0x13]
        call   sub_2_55D0
loc_2_5814:
        pop    ebx
        add    esp, 4
        ret    
sub_2_5819:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        mov    byte ptr [ebp - 1], dl
        mov    bl, cl
        push   0x51
        mov    edx, dword ptr [ebp - 1]
        mov    ecx, ebx
        call   sub_2_58C5
        test   al, 8
        jne    loc_2_5839
        xor    eax, eax
        jmp    loc_2_5863
loc_2_5839:
        push   9
        mov    edx, dword ptr [ebp - 1]
        mov    ecx, ebx
        call   sub_2_58C5
        test   al, 4
        mov    eax, 1
        jne    loc_2_5863
        push   0xf
        mov    edx, ebx
        push   0x10
        mov    ecx, 0x170
        push   0x57
        push   dword ptr [ebp - 1]
        call   sub_2_55D0
loc_2_5863:
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_5868:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        mov    byte ptr [ebp - 1], dl
        mov    bl, cl
        push   0
        push   0x18
        mov    edx, dword ptr [ebp - 1]
        mov    ecx, ebx
        call   sub_2_58EF
        push   0
        mov    edx, dword ptr [ebp - 1]
        push   0x19
        mov    ecx, ebx
        call   sub_2_58EF
        push   0
        mov    edx, dword ptr [ebp - 1]
        push   0x1a
        mov    ecx, ebx
        call   sub_2_58EF
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_58A3:
        sub    esp, 0x18
        mov    dword ptr [esp], 0xb101
        lea    ecx, [esp]
        call   sub_2_4745
        test   eax, eax
        mov    al, 0
        je     loc_2_58C1
        mov    al, byte ptr [esp + 8]
loc_2_58C1:
        add    esp, 0x18
        ret    
sub_2_58C5:
        push   0
        push   dword ptr [esp + 8]
        call   sub_2_47CB
        ret    4
sub_2_58D3:
        push   1
        push   dword ptr [esp + 8]
        call   sub_2_47CB
        ret    4
sub_2_58E1:
        push   2
        push   dword ptr [esp + 8]
        call   sub_2_47CB
        ret    4
sub_2_58EF:
        movzx  eax, byte ptr [esp + 8]
        push   0
        push   eax
        push   dword ptr [esp + 0xc]
        call   sub_2_4A3D
        ret    8
sub_2_5903:
        movzx  eax, word ptr [esp + 8]
        push   1
        push   eax
        push   dword ptr [esp + 0xc]
        call   sub_2_4A3D
        ret    8
sub_2_5917:
        push   2
        push   dword ptr [esp + 0xc]
        push   dword ptr [esp + 0xc]
        call   sub_2_4A3D
        ret    8
sub_2_5929:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x38
        push   ebx
        mov    byte ptr [ebp - 1], dl
        push   esi
        mov    edx, dword ptr [ebp + 8]
        push   edi
        mov    esi, ecx
        mov    byte ptr [ebp - 3], 0
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_4075
        test   ah, 4
        mov    dword ptr [ebp - 0x1c], eax
        je     loc_2_5958
        mov    eax, 0x13
        jmp    loc_2_5D71
loc_2_5958:
        push   2
        lea    eax, [ebp - 0x2c]
        mov    dword ptr [ebp - 0x2c], 0
        push   esi
        push   eax
        call   sub_2_7918
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 0x2c]
        test   byte ptr [ebp - 0x1a], 1
        mov    word ptr [ebp - 6], 4
        mov    dword ptr [ebp - 0xc], eax
        je     loc_2_5986
        mov    word ptr [ebp - 6], 7
loc_2_5986:
        push   0xa
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_58D3
        cmp    ax, 0x605
        je     loc_2_599F
        cmp    ax, 0x607
        jne    loc_2_59A3
loc_2_599F:
        or     byte ptr [ebp - 6], 3
loc_2_59A3:
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_406B
        mov    byte ptr [ebp - 2], al
        movzx  eax, al
        test   eax, eax
        je     loc_2_5C1B
        cmp    eax, 1
        jl     loc_2_59C6
        cmp    eax, 2
        jle    loc_2_59D0
loc_2_59C6:
        mov    eax, 0x13
        jmp    loc_2_5D71
loc_2_59D0:
        cmp    dword ptr [ebp - 0x2c], 0
        jne    loc_2_59E0
        mov    eax, 0x13
        jmp    loc_2_5D71
loc_2_59E0:
        push   0x3e
        mov    edx, dword ptr [ebp + 8]
        cmp    byte ptr [ebp - 2], 1
        mov    ecx, dword ptr [ebp - 1]
        jne    loc_2_59F7
        call   sub_2_58D3
        or     al, 4
        jmp    loc_2_5A00
loc_2_59F7:
        call   sub_2_58D3
        and    ax, 0xfcff
loc_2_5A00:
        push   eax
        mov    edx, dword ptr [ebp + 8]
        push   0x3e
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_5903
        push   0
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_40BC
        test   eax, eax
        jne    loc_2_5A7E
        push   0x18
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_58C5
        push   0x19
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 1]
        mov    byte ptr [ebp - 0x10], al
        call   sub_2_58C5
        cmp    byte ptr [ebp - 0x10], al
        jae    loc_2_5A7E
        push   0x19
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_58C5
        push   0x1a
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 1]
        mov    byte ptr [ebp - 0x10], al
        call   sub_2_58C5
        cmp    byte ptr [ebp - 0x10], al
        ja     loc_2_5A7E
        push   0                             ; -> sub_2_5D8F
        mov    edx, dword ptr [ebp + 8]
        push   0x19
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_58C5
        mov    edx, eax
        mov    ecx, esi
        call   sub_2_65F2
loc_2_5A7E:
        mov    ecx, esi
        xor    ebx, ebx
        call   sub_2_42A2
        mov    dword ptr [ebp - 0x18], eax
        cmp    eax, ebx
        je     loc_2_5C08
        mov    dword ptr [ebp - 0x30], ebx
        cmp    byte ptr [eax], bl
        jbe    loc_2_5C08
        lea    esi, [eax + 4]
loc_2_5AA0:
        push   ebx
        lea    eax, [ebp - 0x34]
        push   eax
        lea    edx, [ebp - 0xc]
        push   ebx
        push   dword ptr [ebp - 0xc]
        push   edx
        call   sub_2_7908
        add    esp, 0x14
        test   eax, eax
        jne    loc_2_5C74
        mov    ecx, dword ptr [ebp - 0x34]
        and    ch, 0x7f
        cmp    dword ptr [esi + 0x20], ecx
        jne    loc_2_5C7E
        mov    eax, dword ptr [ebp - 0x34]
        mov    dword ptr [ebp - 0x10], ebx
        and    eax, 0x8000
        mov    dword ptr [ebp - 0x24], ebx
        mov    dword ptr [ebp - 0x14], 0xffffffff
        cmp    eax, 1
        sbb    eax, eax
        neg    eax
        mov    dword ptr [ebp - 0x28], eax
        cmp    ecx, 1
        je     loc_2_5AFF
        cmp    ecx, 2
        je     loc_2_5B1F
        mov    eax, 0x13
        jmp    loc_2_5D71
loc_2_5AFF:
        cmp    dword ptr [ebp - 0x28], ebx
        je     loc_2_5B45
        mov    ecx, dword ptr [ebp - 0xc]
        or     byte ptr [ebp - 6], 2
        mov    eax, dword ptr [ecx + 4]
        mov    edx, dword ptr [ecx + 8]
        mov    ecx, dword ptr [ecx + 0x10]
        mov    dword ptr [ebp - 0x14], eax
        mov    dword ptr [ebp - 0x10], edx
        mov    dword ptr [ebp - 0x24], ecx
        jmp    loc_2_5B45
loc_2_5B1F:
        cmp    dword ptr [ebp - 0x28], ebx
        je     loc_2_5B45
        mov    ecx, dword ptr [ebp - 0xc]
        or     byte ptr [ebp - 6], 1
        movzx  eax, word ptr [ecx + 4]
        movzx  edx, word ptr [ecx + 6]
        mov    dword ptr [ebp - 0x14], eax
        mov    dword ptr [ebp - 0x10], edx
        movzx  eax, word ptr [ecx + 0xc]
        sub    eax, 0x10000
        mov    dword ptr [ebp - 0x24], eax
loc_2_5B45:
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 1]
        push   dword ptr [ebp - 0x10]
        push   dword ptr [ebp - 0x14]
        push   esi
        call   sub_2_4E43
        test   byte ptr [esi + 0x1c], 4
        jne    loc_2_5BF3
        push   ebx
        mov    dword ptr [esi], ebx
        push   dword ptr [esi + 0x10]
        push   -1
        push   ebx
        mov    dword ptr [esi + 8], ebx
        call   sub_2_7910
        add    esp, 0x10
        test   eax, eax
        jne    loc_2_5D71
        cmp    dword ptr [ebp - 0x28], ebx
        jne    loc_2_5B9B
        push   1
        push   dword ptr [esi + 0x10]
        push   -1
        push   ebx
        call   sub_2_7928
        add    esp, 0x10
        test   eax, eax
        je     loc_2_5BF3
        jmp    loc_2_5D71
loc_2_5B9B:
        mov    eax, dword ptr [ebp - 0x10]
        sub    eax, dword ptr [ebp - 0x14]
        inc    eax
        mov    dword ptr [esi], eax
        mov    eax, dword ptr [ebp - 0x24]
        not    eax
        inc    eax
        cmp    dword ptr [ebp - 0x14], ebx
        mov    dword ptr [esi + 8], eax
        je     loc_2_5C88
        push   1
        mov    eax, dword ptr [ebp - 0x14]
        push   dword ptr [esi + 0x10]
        dec    eax
        push   eax
        push   ebx
        call   sub_2_7928
        add    esp, 0x10
        test   eax, eax
        jne    loc_2_5D71
        cmp    dword ptr [ebp - 0x10], -1
        je     loc_2_5BF3
        push   1
        mov    eax, dword ptr [ebp - 0x10]
        push   dword ptr [esi + 0x10]
        push   -1
        inc    eax
        push   eax
        call   sub_2_7928
        add    esp, 0x10
        test   eax, eax
        jne    loc_2_5D71
loc_2_5BF3:
        add    esi, 0x2c
        mov    ecx, dword ptr [ebp - 0x18]
        movzx  eax, byte ptr [ecx]
        inc    dword ptr [ebp - 0x30]
        cmp    eax, dword ptr [ebp - 0x30]
        ja     loc_2_5AA0
loc_2_5C08:
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_5256
        test   eax, eax
        jne    loc_2_5C1B
        or     byte ptr [ebp - 6], 3
loc_2_5C1B:
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_2_5D1F
        mov    esi, 0xff
        xor    edi, edi
loc_2_5C2C:
        push   edi
        lea    eax, [ebp - 0x38]
        push   eax
        lea    edx, [ebp - 0xc]
        push   edi
        push   dword ptr [ebp - 0xc]
        push   edx
        call   sub_2_7908
        add    esp, 0x14
        test   eax, eax
        jne    loc_2_5D1F
        push   edi
        lea    ecx, [ebp - 0x20]
        push   dword ptr [ebp - 0xc]
        push   ecx
        call   sub_2_7AC8
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 0x38]
        cmp    eax, 1
        je     loc_2_5C92
        cmp    eax, 2
        je     loc_2_5CC6
        cmp    eax, 4
        je     loc_2_5CF3
        jmp    loc_2_5D16
loc_2_5C74:
        mov    eax, 0x13
        jmp    loc_2_5D71
loc_2_5C7E:
        mov    eax, 0x13
        jmp    loc_2_5D71
loc_2_5C88:
        mov    eax, 0x13
        jmp    loc_2_5D71
loc_2_5C92:
        cmp    dword ptr [ebp - 0x20], 0x28
        jne    loc_2_5D7A
        mov    eax, dword ptr [ebp - 0xc]
        or     byte ptr [ebp - 6], 2
        mov    ecx, dword ptr [eax + 0x24]
        cmp    ecx, esi
        je     loc_2_5D16
        cmp    ecx, 0xfc
        je     loc_2_5D16
        mov    eax, dword ptr [eax + 4]
        mov    edx, dword ptr [ebp + 8]
        and    al, 0xf0
        push   eax
        push   ecx
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_5917
        jmp    loc_2_5D16
loc_2_5CC6:
        cmp    dword ptr [ebp - 0x20], 0x1c
        jne    loc_2_5D81
        mov    eax, dword ptr [ebp - 0xc]
        or     byte ptr [ebp - 6], 1
        mov    ecx, dword ptr [eax + 0x18]
        cmp    ecx, esi
        je     loc_2_5D16
        movzx  eax, word ptr [eax + 4]
        and    al, 0xfc
        mov    edx, dword ptr [ebp + 8]
        push   eax
        push   ecx
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_5917
        jmp    loc_2_5D16
loc_2_5CF3:
        cmp    dword ptr [ebp - 0x20], 8
        jne    loc_2_5D88
        mov    eax, dword ptr [ebp - 0xc]
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 1]
        mov    al, byte ptr [eax + 2]
        mov    byte ptr [ebp - 3], al
        push   dword ptr [ebp - 3]
        push   0x3c
        call   sub_2_58EF
loc_2_5D16:
        cmp    dword ptr [ebp - 0xc], edi
        jne    loc_2_5C2C
loc_2_5D1F:
        test   byte ptr [ebp - 0x1b], 0x40
        jne    loc_2_5D36
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 1]
        push   dword ptr [ebp - 6]
        push   dword ptr [ebp - 3]
        call   sub_2_4EA5
loc_2_5D36:
        cmp    byte ptr [ebp - 2], 2
        jne    loc_2_5D6F
        push   0x3e
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_58D3
        mov    si, ax
        mov    edx, dword ptr [ebp + 8]
        and    si, 0xffbf
        or     al, 0x40
        push   eax
        mov    ecx, dword ptr [ebp - 1]
        push   0x3e
        call   sub_2_5903
        push   esi
        mov    edx, dword ptr [ebp + 8]
        push   0x3e
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_5903
loc_2_5D6F:
        xor    eax, eax
loc_2_5D71:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
loc_2_5D7A:
        mov    eax, 0x13
        jmp    loc_2_5D71
loc_2_5D81:
        mov    eax, 0x13
        jmp    loc_2_5D71
loc_2_5D88:
        mov    eax, 0x13
        jmp    loc_2_5D71
sub_2_5D8F:
        push   ebp
        mov    edx, dword ptr [esp + 0xc]
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        mov    bl, byte ptr [ebp + 8]
        mov    ecx, ebx
        call   sub_2_406B
        movzx  esi, al
        test   esi, esi
        je     loc_2_5DBD
        cmp    esi, 1
        je     loc_2_5DC3
        cmp    esi, 2
        je     loc_2_5DC9
        mov    byte ptr [ebp - 1], 0
        jmp    loc_2_5DCD
loc_2_5DBD:
        mov    byte ptr [ebp - 1], 6
        jmp    loc_2_5DCD
loc_2_5DC3:
        mov    byte ptr [ebp - 1], 2
        jmp    loc_2_5DCD
loc_2_5DC9:
        mov    byte ptr [ebp - 1], 1
loc_2_5DCD:
        mov    edx, dword ptr [ebp + 0xc]
        mov    ecx, ebx
        call   sub_2_4075
        test   eax, 0x14000
        jne    loc_2_5DFC
        push   4
        mov    edx, dword ptr [ebp + 0xc]
        mov    ecx, ebx
        call   sub_2_58D3
        mov    edx, dword ptr [ebp + 0xc]
        mov    ecx, ebx
        movzx  eax, ax
        and    al, 0xf8
        push   eax
        push   4
        call   sub_2_5917
loc_2_5DFC:
        cmp    dword ptr [0], 0              ; -> loc_3_0368
        je     loc_2_5E13
        push   0
        mov    edx, dword ptr [ebp + 0xc]
        push   0x3c
        mov    ecx, ebx
        call   sub_2_58EF
loc_2_5E13:
        cmp    byte ptr [ebp - 1], 0
        je     loc_2_5E40
        mov    byte ptr [ebp - 8], 0x10
        mov    al, byte ptr [ebp - 1]
        mov    byte ptr [ebp - 4], al
loc_2_5E23:
        push   0
        mov    edx, dword ptr [ebp + 0xc]
        push   dword ptr [ebp - 8]
        mov    ecx, ebx
        call   sub_2_5917
        movzx  eax, byte ptr [ebp - 8]
        add    al, 4
        dec    byte ptr [ebp - 4]
        mov    byte ptr [ebp - 8], al
        jne    loc_2_5E23
loc_2_5E40:
        cmp    esi, 1
        je     loc_2_5E53
        cmp    esi, 2
        je     loc_2_5F02
        jmp    loc_2_5FA0
loc_2_5E53:
        xor    esi, esi
        mov    edx, dword ptr [ebp + 0xc]
        push   esi
        mov    ecx, ebx
        push   0x2c
        call   sub_2_5917
        push   esi
        mov    edx, dword ptr [ebp + 0xc]
        push   0x28
        mov    ecx, ebx
        call   sub_2_5917
        push   esi
        mov    edx, dword ptr [ebp + 0xc]
        push   0x32
        mov    ecx, ebx
        call   sub_2_5903
        push   esi
        mov    edx, dword ptr [ebp + 0xc]
        push   0x30
        mov    ecx, ebx
        call   sub_2_5903
        push   esi
        mov    edx, dword ptr [ebp + 0xc]
        push   0x1d
        mov    ecx, ebx
        call   sub_2_58EF
        push   0xff
        mov    edx, dword ptr [ebp + 0xc]
        push   0x1c
        mov    ecx, ebx
        mov    edi, 0xffff
        call   sub_2_58EF
        push   esi
        mov    edx, dword ptr [ebp + 0xc]
        push   0x22
        mov    ecx, ebx
        call   sub_2_5903
        push   edi
        mov    edx, dword ptr [ebp + 0xc]
        push   0x20
        mov    ecx, ebx
        call   sub_2_5903
        push   esi
        mov    edx, dword ptr [ebp + 0xc]
        push   0x26
        mov    ecx, ebx
        call   sub_2_5903
        push   edi
        mov    edx, dword ptr [ebp + 0xc]
        push   0x24
        mov    ecx, ebx
        call   sub_2_5903
        push   0x3e
        mov    edx, dword ptr [ebp + 0xc]
        mov    ecx, ebx
        call   sub_2_58D3
        and    ax, 0xfff3
        mov    edx, dword ptr [ebp + 0xc]
        push   eax
        push   0x3e
        mov    ecx, ebx
        call   sub_2_5903
        jmp    loc_2_5FA0
loc_2_5F02:
        xor    esi, esi
        mov    edx, dword ptr [ebp + 0xc]
        push   esi
        mov    ecx, ebx
        push   0x32
        call   sub_2_5903
        push   esi
        mov    edx, dword ptr [ebp + 0xc]
        push   0x2e
        mov    ecx, ebx
        call   sub_2_5903
        push   esi
        mov    edx, dword ptr [ebp + 0xc]
        push   0x3a
        mov    ecx, ebx
        call   sub_2_5903
        push   esi
        mov    edx, dword ptr [ebp + 0xc]
        push   0x36
        mov    ecx, ebx
        call   sub_2_5903
        push   esi
        mov    edx, dword ptr [ebp + 0xc]
        push   0x30
        mov    ecx, ebx
        call   sub_2_5903
        push   esi
        mov    edx, dword ptr [ebp + 0xc]
        push   0x2c
        mov    ecx, ebx
        call   sub_2_5903
        push   esi
        mov    edx, dword ptr [ebp + 0xc]
        push   0x38
        mov    ecx, ebx
        call   sub_2_5903
        push   esi
        mov    edx, dword ptr [ebp + 0xc]
        push   0x34
        mov    ecx, ebx
        call   sub_2_5903
        push   esi
        mov    edx, dword ptr [ebp + 0xc]
        push   0x20
        mov    ecx, ebx
        call   sub_2_5917
        push   esi
        mov    edx, dword ptr [ebp + 0xc]
        push   0x1c
        mov    ecx, ebx
        call   sub_2_5917
        push   esi
        mov    edx, dword ptr [ebp + 0xc]
        push   0x28
        mov    ecx, ebx
        call   sub_2_5917
        push   esi
        mov    edx, dword ptr [ebp + 0xc]
        push   0x24
        mov    ecx, ebx
        call   sub_2_5917
loc_2_5FA0:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_5FA7:
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    bl, dl
        mov    esi, ecx
        push   0
        push   0x100000
        push   esi
        call   sub_2_7AD0
        mov    edx, dword ptr [esp + 0x20]
        add    esp, 0xc
        mov    ecx, ebx
        push   0xa
        call   sub_2_58D3
        cmp    ax, 0x600
        mov    di, ax
        je     loc_2_6024
        cmp    di, 0x601
        je     loc_2_6024
        cmp    di, 0x602
        je     loc_2_6024
        cmp    di, 0x603
        je     loc_2_6024
        lea    eax, [esp + 0xc]
        push   2
        push   esi
        push   eax
        call   sub_2_7918
        add    esp, 0xc
        test   eax, eax
        je     loc_2_6007
        cmp    di, 0x604
        jne    loc_2_6024
loc_2_6007:
        push   dword ptr [esp + 0x14]
        push   ebx
        call   sub_2_5D8F
        add    esp, 8
        push   1
        push   0x100000
        push   esi
        call   sub_2_7AD0
        add    esp, 0xc
loc_2_6024:
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    4
sub_2_602D:
        sub    esp, 4
        xor    edx, edx
        xor    ecx, ecx
        push   ebx
        push   esi
        push   edi
        push   ebp
        xor    edi, edi
        push   0
        call   sub_2_58E1
        mov    ecx, 0x1010101
        or     eax, ecx
        cmp    eax, ecx
        mov    esi, eax
        je     loc_2_6058
        cmp    esi, -1
        je     loc_2_6058
        mov    edi, 1
loc_2_6058:
        mov    bl, 8
        mov    ebp, 0x1010101
        mov    byte ptr [esp + 0x13], 1
loc_2_6064:
        push   0
        mov    edx, ebx
        xor    ecx, ecx
        call   sub_2_58E1
        or     eax, ebp
        cmp    eax, ebp
        je     loc_2_607F
        cmp    eax, -1
        je     loc_2_607F
        cmp    esi, eax
        jne    loc_2_6090
        inc    edi
loc_2_607F:
        movzx  ebx, bl
        add    bl, 8
        inc    byte ptr [esp + 0x13]
        cmp    byte ptr [esp + 0x13], 0x20
        jb     loc_2_6064
loc_2_6090:
        cmp    byte ptr [esp + 0x13], 0x20
        jne    loc_2_60A4
        test   edi, edi
        je     loc_2_60A0
        cmp    edi, 0x20
        jne    loc_2_60A4
loc_2_60A0:
        xor    eax, eax
        jmp    loc_2_60A9
loc_2_60A4:
        mov    eax, 1
loc_2_60A9:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
sub_2_60B1:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        push   esi
        push   edi
        mov    esi, 1
        test   dword ptr [0], esi            ; -> obj3:0x0CB8
        jne    loc_2_613B
        xor    edi, edi
        mov    dword ptr [0], esi            ; -> obj3:0x0CC0
        mov    dword ptr [0], edi            ; -> loc_3_0CD8
        call   sub_2_602D
        test   eax, eax
        jne    loc_2_613B
        mov    dword ptr [0], esi            ; -> loc_3_0CD8
        call   sub_2_602D
        test   eax, eax
        jne    loc_2_613B
        push   edi
        xor    edx, edx
        xor    ecx, ecx
        mov    dword ptr [0], edi            ; -> obj3:0x0CC0
        call   sub_2_58E1
        cmp    eax, 0x4838086
        mov    esi, eax
        je     loc_2_610E
        cmp    esi, 0x4858086
        jne    loc_2_613B
loc_2_610E:
        push   8
        xor    edx, edx
        xor    ecx, ecx
        call   sub_2_58C5
        cmp    esi, 0x4838086
        jne    loc_2_6125
        cmp    al, 1
        jbe    loc_2_6131
loc_2_6125:
        cmp    esi, 0x4858086
        jne    loc_2_613B
        test   al, al
        jne    loc_2_613B
loc_2_6131:
        mov    dword ptr [0], 1              ; -> loc_3_0CD4
loc_2_613B:
        mov    byte ptr [0], 0x20            ; -> obj3:0x0CF8
        xor    bl, bl
        mov    byte ptr [ebp - 8], bl
loc_2_6147:
        push   0
        mov    edx, dword ptr [ebp - 8]
        xor    ecx, ecx
        call   sub_2_58E1
        mov    esi, eax
        test   esi, esi
        je     loc_2_615E
        cmp    esi, -1
        jne    loc_2_616E
loc_2_615E:
        movzx  eax, byte ptr [ebp - 8]
        add    al, 8
        inc    bl
        mov    byte ptr [ebp - 8], al
        cmp    bl, 0x20
        jb     loc_2_6147
loc_2_616E:
        cmp    bl, 0x20
        je     loc_2_6217
        test   byte ptr [0], 1               ; -> obj3:0x0CB8
        jne    loc_2_6199
        cmp    dword ptr [0], 0              ; -> obj3:0x0CC0
        jne    loc_2_6199
        lea    ecx, [ebx*8]
        call   sub_2_56FA
        test   eax, eax
        je     loc_2_6217
loc_2_6199:
        lea    eax, [ebx + 1]
        mov    byte ptr [ebp - 5], al
        cmp    al, 0x20
        jae    loc_2_6227
        shl    al, 3
        mov    byte ptr [ebp - 0xc], al
loc_2_61AD:
        push   0
        mov    edx, dword ptr [ebp - 0xc]
        xor    ecx, ecx
        call   sub_2_58E1
        cmp    eax, esi
        jne    loc_2_61F7
        mov    byte ptr [ebp - 1], 4
loc_2_61C1:
        mov    edx, dword ptr [ebp - 0xc]
        xor    ecx, ecx
        push   dword ptr [ebp - 1]
        call   sub_2_58E1
        mov    edi, eax
        xor    ecx, ecx
        push   dword ptr [ebp - 1]
        lea    eax, [ebx*8]
        mov    edx, eax
        call   sub_2_58E1
        cmp    edi, eax
        jne    loc_2_61F1
        add    byte ptr [ebp - 1], 4
        cmp    byte ptr [ebp - 1], 0x40
        jb     loc_2_61C1
loc_2_61F1:
        cmp    byte ptr [ebp - 1], 0x40
        je     loc_2_620B
loc_2_61F7:
        movzx  eax, byte ptr [ebp - 0xc]
        add    al, 8
        inc    byte ptr [ebp - 5]
        cmp    byte ptr [ebp - 5], 0x20
        mov    byte ptr [ebp - 0xc], al
        jb     loc_2_61AD
        jmp    loc_2_6227
loc_2_620B:
        mov    al, byte ptr [ebp - 5]
        sub    al, bl
        mov    byte ptr [0], al              ; -> obj3:0x0CF8
        jmp    loc_2_6227
loc_2_6217:
        push   2
        mov    eax, dword ptr [0]            ; -> obj3:0x0DAC
        push   eax
        call   sub_2_79B8
        add    esp, 8
loc_2_6227:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_622E:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        push   0
        push   0x20000
        push   0xe0000
        call   sub_2_7A38
        add    esp, 0xc
        mov    edx, eax
        mov    dword ptr [ebp - 4], eax
        add    dword ptr [ebp - 4], 0x1fff0
        cmp    dword ptr [ebp - 4], edx
        jb     loc_2_628E
loc_2_625C:
        mov    eax, dword ptr [ebp - 4]
        mov    ecx, dword ptr [0]            ; -> obj3:0x0D94
        cmp    dword ptr [eax], ecx
        jne    loc_2_6280
        xor    ecx, ecx
        xor    esi, esi
loc_2_626D:
        mov    eax, dword ptr [ebp - 4]
        movzx  eax, byte ptr [eax + esi]
        add    ecx, eax
        inc    esi
        cmp    esi, 0x10
        jb     loc_2_626D
        test   cl, cl
        je     loc_2_6289
loc_2_6280:
        sub    dword ptr [ebp - 4], 0x10
        cmp    dword ptr [ebp - 4], edx
        jae    loc_2_625C
loc_2_6289:
        cmp    dword ptr [ebp - 4], edx
        jae    loc_2_6292
loc_2_628E:
        xor    eax, eax
        jmp    loc_2_62FE
loc_2_6292:
        push   0
        mov    eax, dword ptr [ebp - 4]
        push   0x2000
        push   dword ptr [eax + 4]
        call   sub_2_7A38
        add    esp, 0xc
        mov    dword ptr [ebp - 4], eax
        mov    al, byte ptr [eax]
        cmp    al, 0xff
        jne    loc_2_62B4
        xor    eax, eax
        jmp    loc_2_62FE
loc_2_62B4:
        test   al, al
        jne    loc_2_62BC
        xor    eax, eax
        jmp    loc_2_62FE
loc_2_62BC:
        pushal 
        mov    eax, 0x49435024
        xor    ebx, ebx
        mov    dword ptr [0], esp            ; -> loc_3_0CCC
        push   cs
        call   dword ptr [ebp - 4]
        mov    esp, dword ptr [0]            ; -> loc_3_0CCC
        sub    al, 1
        sbb    ecx, ecx
        add    ebx, edx
        and    ebx, ecx
        mov    dword ptr [ebp - 4], ebx
        popal  
        mov    eax, dword ptr [ebp - 4]
        cmp    eax, 0xa0000
        jae    loc_2_62EE
        xor    eax, eax
        jmp    loc_2_62FE
loc_2_62EE:
        push   0
        push   0x2000
        push   eax
        call   sub_2_7A38
        add    esp, 0xc
loc_2_62FE:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_6305:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x18
        push   edi
        call   sub_2_44FB
        mov    ecx, 0                        ; -> obj3:0x0D9C
        call   sub_2_4521
        mov    edi, eax
        mov    dword ptr [0], eax            ; -> obj3:0x0CB8
        cmp    dword ptr [0], 0              ; -> sub_3_0CC8
        jne    loc_2_636E
        cmp    dword ptr [0], 0              ; -> obj3:0x0CBC
        jne    loc_2_636E
        call   sub_2_622E
        mov    dword ptr [0], eax            ; -> sub_3_0CC8
        test   eax, eax
        je     loc_2_635D
        xor    eax, eax
        lea    ecx, [ebp - 0x18]
        mov    dword ptr [ebp - 0x14], eax
        mov    dword ptr [ebp - 4], eax
        mov    dword ptr [ebp - 0x18], 0xb10a
        call   sub_2_4745
        test   eax, eax
        jne    loc_2_636E
loc_2_635D:
        mov    dword ptr [0], 1              ; -> obj3:0x0CBC
        and    byte ptr [0], 0xfe            ; -> obj3:0x0CB8
loc_2_636E:
        call   sub_2_60B1
        call   sub_2_3D9D
        cmp    dword ptr [0], edi            ; -> obj3:0x0CB8
        je     loc_2_6390
        mov    ecx, 0                        ; -> obj3:0x0DA4
        mov    edx, dword ptr [0]            ; -> obj3:0x0CB8
        call   sub_2_4564
loc_2_6390:
        call   sub_2_4592
        pop    edi
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_639A:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x114
        push   ebx
        push   esi
        push   edi
        mov    bl, dl
        mov    esi, ecx
        push   0
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, ebx
        call   sub_2_58E1
        push   8
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, ebx
        mov    dword ptr [ebp - 8], eax
        call   sub_2_58C5
        push   0xa
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, ebx
        mov    byte ptr [ebp - 1], al
        call   sub_2_58D3
        mov    word ptr [ebp - 4], ax
        push   9
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, ebx
        call   sub_2_58C5
        movzx  eax, al
        movzx  ecx, word ptr [ebp - 4]
        shl    ecx, 8
        mov    edx, dword ptr [ebp + 8]
        add    eax, ecx
        mov    ecx, ebx
        mov    dword ptr [ebp - 0x10], eax
        call   sub_2_406B
        movzx  eax, al
        test   eax, eax
        je     loc_2_6412
        cmp    eax, 1
        je     loc_2_6416
        cmp    eax, 2
        je     loc_2_641A
        xor    edi, edi
        jmp    loc_2_6428
loc_2_6412:
        push   0x2c
        jmp    loc_2_641C
loc_2_6416:
        xor    edi, edi
        jmp    loc_2_6428
loc_2_641A:
        push   0x40
loc_2_641C:
        mov    edx, dword ptr [ebp + 8]
        mov    ecx, ebx
        call   sub_2_58E1
        mov    edi, eax
loc_2_6428:
        xor    ebx, ebx
        lea    eax, [ebp - 0xc]
        push   ebx
        lea    ecx, [ebp - 0x114]
        mov    dword ptr [ebp - 0xc], 0x100
        push   eax
        push   ecx
        push   1
        push   0                             ; -> obj3:0x0E10
        push   ebx
        push   esi
        call   sub_2_79D8
        add    esp, 0x1c
        test   eax, eax
        je     loc_2_646E
        push   ebx
        mov    ecx, dword ptr [ebp - 4]
        push   ebx
        call   sub_2_42EB
        push   eax
        push   1
        push   0                             ; -> obj3:0x0E1C
        push   ebx
        push   esi
        call   sub_2_79E0
        add    esp, 0x1c
loc_2_646E:
        movzx  eax, byte ptr [ebp - 1]
        push   eax
        lea    ecx, [ebp - 0x14]
        push   0                             ; -> obj3:0x0E28
        xor    ebx, ebx
        push   ecx
        call   sub_2_79F8
        add    esp, 0xc
        lea    ecx, [ebp - 0x14]
        push   ebx
        push   ebx
        push   ecx
        push   1
        push   0                             ; -> obj3:0x0E30
        push   ebx
        push   esi
        call   sub_2_79E0
        add    esp, 0x1c
        lea    eax, [ebp - 0x114]
        push   dword ptr [ebp - 0x10]
        push   0                             ; -> obj3:0x0E3C
        push   eax
        call   sub_2_79F8
        add    esp, 0xc
        lea    ecx, [ebp - 0x114]
        push   1
        push   ecx
        push   esi
        call   sub_2_7AD8
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 0x10]
        and    eax, 0xffff80
        cmp    eax, 0x10180
        jne    loc_2_6525
        push   ebx
        lea    eax, [ebp - 0xc]
        push   eax
        lea    ecx, [ebp - 0x114]
        mov    dword ptr [ebp - 0xc], 0x100
        push   ecx
        push   1
        push   0                             ; -> obj3:0x0E48
        push   ebx
        push   esi
        call   sub_2_79D8
        add    esp, 0x1c
        test   eax, eax
        je     loc_2_6515
        push   ebx
        push   ebx
        push   0                             ; -> obj3:0x0E54
        push   1
        push   0                             ; -> obj3:0x0E74
        push   ebx
        push   esi
        call   sub_2_79E0
        add    esp, 0x1c
loc_2_6515:
        push   1
        push   0                             ; -> obj3:0x0E80
        push   esi
        call   sub_2_7AD8
        add    esp, 0xc
loc_2_6525:
        movzx  eax, word ptr [ebp - 4]
        push   eax
        lea    ecx, [ebp - 0x114]
        push   0                             ; -> obj3:0x0E90
        push   ecx
        call   sub_2_79F8
        add    esp, 0xc
        lea    ecx, [ebp - 0x114]
        push   1
        push   ecx
        push   esi
        call   sub_2_7AD8
        add    esp, 0xc
        mov    eax, dword ptr [ebp - 8]
        movzx  ecx, byte ptr [ebp - 1]
        shr    eax, 0x10
        push   ecx
        push   edi
        lea    ecx, [ebp - 0x114]
        movzx  ebx, ax
        movzx  eax, word ptr [ebp - 8]
        push   ebx
        push   eax
        push   0                             ; -> obj3:0x0E9C
        push   ecx
        call   sub_2_79F8
        add    esp, 0x18
        lea    ecx, [ebp - 0x114]
        push   0
        push   ecx
        push   esi
        call   sub_2_7AD8
        add    esp, 0xc
        lea    ecx, [ebp - 0x114]
        movzx  eax, word ptr [ebp - 8]
        push   edi
        push   ebx
        push   eax
        push   0                             ; -> obj3:0x0EC8
        push   ecx
        call   sub_2_79F8
        add    esp, 0x14
        lea    ecx, [ebp - 0x114]
        push   0
        push   ecx
        push   esi
        call   sub_2_7AD8
        movzx  ecx, byte ptr [ebp - 1]
        add    esp, 0xc
        movzx  eax, word ptr [ebp - 8]
        push   ecx
        push   ebx
        push   eax
        lea    ecx, [ebp - 0x114]
        push   0                             ; -> obj3:0x0EEC
        push   ecx
        call   sub_2_79F8
        add    esp, 0x14
        lea    ecx, [ebp - 0x114]
        push   0
        push   ecx
        push   esi
        call   sub_2_7AD8
        add    esp, 0xc
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
sub_2_65F2:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        push   ebx
        mov    byte ptr [ebp - 2], dl
        push   esi
        push   edi
        mov    esi, ecx
        call   sub_2_44FB
        mov    al, byte ptr [0]              ; -> obj3:0x0CF8
        cmp    esi, dword ptr [0]            ; -> obj3:0x0DAC
        mov    byte ptr [ebp - 4], al
        je     loc_2_6656
        xor    edi, edi
        lea    eax, [ebp - 3]
        push   edi
        lea    edx, [ebp - 1]
        push   edi
        mov    ecx, esi
        push   eax
        call   sub_2_4212
        push   edi
        mov    edx, dword ptr [ebp - 3]
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_58E1
        push   eax
        lea    ecx, [ebp - 0x10]
        push   0                             ; -> obj3:0x0F0C
        push   ecx
        call   sub_2_79F8
        add    esp, 0xc
        lea    ecx, [ebp - 0x10]
        call   sub_2_4521
        test   ah, 0x20
        je     loc_2_6656
        mov    byte ptr [ebp - 4], 1
loc_2_6656:
        mov    byte ptr [ebp - 1], 0
        cmp    byte ptr [ebp - 4], 0
        je     loc_2_6786
loc_2_6664:
        xor    bl, bl
loc_2_6666:
        cmp    bl, 1
        jne    loc_2_6683
        push   0xe
        mov    dl, byte ptr [ebp - 1]
        shl    dl, 3
        mov    ecx, dword ptr [ebp - 2]
        call   sub_2_58C5
        test   al, 0x80
        je     loc_2_6777
loc_2_6683:
        mov    al, byte ptr [ebp - 1]
        push   0
        mov    ecx, dword ptr [ebp - 2]
        lea    eax, [ebx + eax*8]
        mov    byte ptr [ebp - 3], al
        mov    edx, dword ptr [ebp - 3]
        call   sub_2_58E1
        mov    ecx, eax
        mov    edi, eax
        call   sub_2_41FE
        test   eax, eax
        jne    loc_2_66B3
        test   bl, bl
        je     loc_2_6777
        jmp    loc_2_676C
loc_2_66B3:
        push   edi
        lea    eax, [ebp - 0x10]
        push   0                             ; -> obj3:0x0F14
        push   eax
        call   sub_2_79F8
        add    esp, 0xc
        lea    ecx, [ebp - 0x10]
        call   sub_2_4521
        test   eax, 0x20
        mov    edi, eax
        je     loc_2_66E0
        mov    dword ptr [0], 1              ; -> obj3:0x0CC4
loc_2_66E0:
        test   edi, 8
        je     loc_2_66F2
        test   bl, bl
        je     loc_2_6777
        jmp    loc_2_676C
loc_2_66F2:
        cmp    byte ptr [ebp - 2], 0
        je     loc_2_673C
        test   edi, 0x800
        je     loc_2_6706
        test   bl, bl
        je     loc_2_6777
        jmp    loc_2_676C
loc_2_6706:
        cmp    esi, dword ptr [0]            ; -> obj3:0x0DAC
        jne    loc_2_6714
        mov    byte ptr [ebp - 5], 0
        jmp    loc_2_6724
loc_2_6714:
        push   0
        lea    edx, [ebp - 5]
        push   0
        mov    ecx, esi
        push   0
        call   sub_2_4212
loc_2_6724:
        mov    edx, dword ptr [ebp - 2]
        mov    ecx, dword ptr [ebp - 5]
        push   dword ptr [ebp - 3]
        call   sub_2_52FF
        test   eax, eax
        je     loc_2_673C
        test   bl, bl
        je     loc_2_6777
        jmp    loc_2_676C
loc_2_673C:
        test   edi, 0x1000
        je     loc_2_6750
        cmp    byte ptr [ebp - 1], 0x10
        jb     loc_2_6750
        test   bl, bl
        je     loc_2_6777
        jmp    loc_2_676C
loc_2_6750:
        test   byte ptr [ebp - 3], 7
        je     loc_2_675E
        test   edi, 0x40
        jne    loc_2_6777
loc_2_675E:
        push   edi
        push   esi
        push   dword ptr [ebp - 3]
        push   dword ptr [ebp - 2]
        call   dword ptr [ebp + 8]
        add    esp, 0x10
loc_2_676C:
        inc    bl
        cmp    bl, 7
        jbe    loc_2_6666
loc_2_6777:
        inc    byte ptr [ebp - 1]
        mov    al, byte ptr [ebp - 1]
        cmp    al, byte ptr [ebp - 4]
        jb     loc_2_6664
loc_2_6786:
        call   sub_2_4592
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
sub_2_6794:
        push   ebx
        push   esi
        cmp    byte ptr [0], dl              ; -> obj3:0x0DB4
        push   edi
        mov    bl, dl
        mov    esi, ecx
        jae    loc_2_67A9
        mov    byte ptr [0], bl              ; -> obj3:0x0DB4
loc_2_67A9:
        mov    edi, dword ptr [esp + 0x10]
        test   edi, edi
        jne    loc_2_67BC
        push   0
        push   esi
        call   sub_2_7AE8
        add    esp, 8
loc_2_67BC:
        push   0                             ; -> loc_2_2ACA
        mov    edx, ebx
        mov    ecx, esi
        call   sub_2_65F2
        test   edi, edi
        jne    loc_2_67D9
        push   0
        push   esi
        call   sub_2_7AE0
        add    esp, 8
loc_2_67D9:
        pop    edi
        pop    esi
        pop    ebx
        ret    4
sub_2_67DF:
        push   ebp
        mov    eax, dword ptr [0]            ; -> obj3:0x0DAC
        mov    ebp, esp
        sub    esp, 8
        push   ebx
        xor    ebx, ebx
        push   ebx
        push   eax
        call   sub_2_7AE8
        add    esp, 8
        call   sub_2_58A3
        push   ebx
        lea    ecx, [ebp - 8]
        mov    byte ptr [0], al              ; -> obj3:0x0DB0
        mov    byte ptr [0], bl              ; -> obj3:0x0DB4
        mov    eax, dword ptr [0]            ; -> obj3:0x0DAC
        push   eax
        push   ecx
        call   sub_2_7968
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_685A
loc_2_681E:
        push   ebx
        lea    eax, [ebp - 1]
        push   eax
        xor    edx, edx
        push   ebx
        mov    ecx, dword ptr [ebp - 8]
        call   sub_2_4212
        test   byte ptr [ebp - 1], 0x10
        je     loc_2_6846
        push   1
        push   0x200
        push   dword ptr [ebp - 8]
        call   sub_2_7AD0
        add    esp, 0xc
loc_2_6846:
        push   ebx
        lea    ecx, [ebp - 8]
        push   dword ptr [ebp - 8]
        push   ecx
        call   sub_2_7960
        add    esp, 0xc
        test   eax, eax
        je     loc_2_681E
loc_2_685A:
        push   1
        mov    al, byte ptr [0]              ; -> obj3:0x0DB4
        mov    edx, eax
        mov    ecx, dword ptr [0]            ; -> obj3:0x0DAC
        call   sub_2_6794
        cmp    byte ptr [0], 0xff            ; -> obj3:0x0DB4
        je     loc_2_688A
        mov    al, byte ptr [0]              ; -> obj3:0x0DB0
        inc    byte ptr [0]                  ; -> obj3:0x0DB4
        cmp    byte ptr [0], al              ; -> obj3:0x0DB4
        jbe    loc_2_685A
loc_2_688A:
        push   0
        mov    eax, dword ptr [0]            ; -> obj3:0x0DAC
        push   eax
        call   sub_2_7AE0
        add    esp, 8
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_689F:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   ebx
        mov    dword ptr [ebp - 8], ecx
        push   esi
        mov    ebx, edx
        push   edi
        mov    esi, dword ptr [ebp + 0xc]
        push   0
        lea    eax, [ebp - 2]
        push   0
        lea    edx, [ebp - 1]
        push   eax
        mov    ecx, dword ptr [ebp + 8]
        call   sub_2_4212
        mov    edi, dword ptr [ebp + 0x10]
        lea    eax, [ebx + edi]
        cmp    eax, 0x100
        jbe    loc_2_68DA
        mov    eax, 0x13
        jmp    loc_2_69EA
loc_2_68DA:
        test   edi, edi
        je     loc_2_690C
        test   bl, 1
        je     loc_2_690C
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_68F9
        push   ebx
        mov    edx, dword ptr [ebp - 2]
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_58C5
        mov    byte ptr [esi], al
        jmp    loc_2_6908
loc_2_68F9:
        mov    al, byte ptr [esi]
        mov    edx, dword ptr [ebp - 2]
        push   eax
        mov    ecx, dword ptr [ebp - 1]
        push   ebx
        call   sub_2_58EF
loc_2_6908:
        inc    esi
        inc    bl
        dec    edi
loc_2_690C:
        cmp    edi, 2
        jb     loc_2_6949
        mov    al, bl
        and    al, 3
        cmp    al, 2
        jne    loc_2_6949
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_6930
        push   ebx
        mov    edx, dword ptr [ebp - 2]
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_58D3
        mov    word ptr [esi], ax
        jmp    loc_2_6940
loc_2_6930:
        mov    ax, word ptr [esi]
        mov    edx, dword ptr [ebp - 2]
        push   eax
        mov    ecx, dword ptr [ebp - 1]
        push   ebx
        call   sub_2_5903
loc_2_6940:
        add    esi, 2
        add    bl, 2
        sub    edi, 2
loc_2_6949:
        cmp    edi, 4
        jb     loc_2_698A
        mov    eax, edi
        shr    eax, 2
        mov    dword ptr [ebp - 0xc], eax
        shl    eax, 2
        sub    edi, eax
loc_2_695B:
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_6971
        push   ebx
        mov    edx, dword ptr [ebp - 2]
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_58E1
        mov    dword ptr [esi], eax
        jmp    loc_2_697F
loc_2_6971:
        mov    edx, dword ptr [ebp - 2]
        mov    ecx, dword ptr [ebp - 1]
        push   dword ptr [esi]
        push   ebx
        call   sub_2_5917
loc_2_697F:
        add    esi, 4
        add    bl, 4
        dec    dword ptr [ebp - 0xc]
        jne    loc_2_695B
loc_2_698A:
        cmp    edi, 2
        jb     loc_2_69BF
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_69A6
        push   ebx
        mov    edx, dword ptr [ebp - 2]
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_58D3
        mov    word ptr [esi], ax
        jmp    loc_2_69B6
loc_2_69A6:
        mov    ax, word ptr [esi]
        mov    edx, dword ptr [ebp - 2]
        push   eax
        mov    ecx, dword ptr [ebp - 1]
        push   ebx
        call   sub_2_5903
loc_2_69B6:
        add    esi, 2
        add    bl, 2
        sub    edi, 2
loc_2_69BF:
        test   edi, edi
        je     loc_2_69E8
        cmp    dword ptr [ebp - 8], 0
        jne    loc_2_69D9
        push   ebx
        mov    edx, dword ptr [ebp - 2]
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_58C5
        mov    byte ptr [esi], al
        jmp    loc_2_69E8
loc_2_69D9:
        mov    al, byte ptr [esi]
        mov    edx, dword ptr [ebp - 2]
        push   eax
        mov    ecx, dword ptr [ebp - 1]
        push   ebx
        call   sub_2_58EF
loc_2_69E8:
        xor    eax, eax
loc_2_69EA:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0xc
loc_2_69F3:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 4
        cmp    eax, 1
        jbe    loc_2_6A18
        cmp    eax, 2
        je     loc_2_6A2E
        cmp    eax, 3
        je     loc_2_6A4F
        cmp    eax, 4
        je     loc_2_6A83
        mov    eax, 0x20
        jmp    loc_2_6A94
loc_2_6A18:
        mov    edx, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        call   sub_2_689F
        jmp    loc_2_6A94
loc_2_6A2E:
        cmp    dword ptr [ebp + 0x18], 8
        je     loc_2_6A3B
        mov    eax, 0x13
        jmp    loc_2_6A94
loc_2_6A3B:
        mov    ecx, dword ptr [ebp + 0x14]
        xor    eax, eax
        mov    dword ptr [ecx + 4], 0        ; -> loc_2_69F3
        mov    dword ptr [ecx], 0            ; -> loc_2_6D8F
        jmp    loc_2_6A94
loc_2_6A4F:
        cmp    dword ptr [ebp + 0x18], 4
        je     loc_2_6A5C
        mov    eax, 0x13
        jmp    loc_2_6A94
loc_2_6A5C:
        push   0
        lea    eax, [ebp - 2]
        push   0
        lea    edx, [ebp - 1]
        push   eax
        mov    ecx, dword ptr [ebp + 0x10]
        call   sub_2_4212
        mov    edx, dword ptr [ebp - 2]
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_4FCA
        mov    ecx, dword ptr [ebp + 0x14]
        mov    dword ptr [ecx], eax
        xor    eax, eax
        jmp    loc_2_6A94
loc_2_6A83:
        mov    edx, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        call   sub_2_504E
loc_2_6A94:
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_6A98:
        push   esi
        cmp    edx, 1
        mov    esi, ecx
        je     loc_2_6AAB
        cmp    edx, 2
        je     loc_2_6ADF
loc_2_6AA5:
        xor    eax, eax
loc_2_6AA7:
        pop    esi
        ret    4
loc_2_6AAB:
        push   0
        mov    edx, 1
        mov    ecx, dword ptr [esp + 0xc]
        call   sub_2_45BC
        test   eax, eax
        je     loc_2_6AA5
        movzx  ecx, word ptr [esi]
        add    esi, 0x10
        test   ecx, ecx
        je     loc_2_6AA5
        mov    edx, dword ptr [esi + 0xc]
        mov    eax, dword ptr [eax + 0x18]
loc_2_6ACF:
        cmp    eax, edx
        jae    loc_2_6AD8
        dec    ecx
        jne    loc_2_6ACF
        jmp    loc_2_6AA5
loc_2_6AD8:
        mov    eax, 1
        jmp    loc_2_6AA7
loc_2_6ADF:
        push   0
        mov    edx, 2
        mov    ecx, dword ptr [esp + 0xc]
        call   sub_2_45BC
        test   eax, eax
        je     loc_2_6AA5
        movzx  edx, word ptr [esi]
        add    esi, 0xc
        test   edx, edx
        je     loc_2_6AA5
        mov    ecx, dword ptr [eax + 0x18]
        movzx  eax, word ptr [esi + 6]
loc_2_6B04:
        cmp    eax, ecx
        jbe    loc_2_6B0D
        dec    edx
        jne    loc_2_6B04
        jmp    loc_2_6AA5
loc_2_6B0D:
        mov    eax, 1
        jmp    loc_2_6AA7
sub_2_6B14:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   esi
        lea    eax, [ebp - 4]
        push   edi
        mov    esi, edx
        push   2
        push   ecx
        push   eax
        call   sub_2_7918
        add    esp, 0xc
        test   eax, eax
        je     loc_2_6B38
        xor    eax, eax
        jmp    loc_2_6BFA
loc_2_6B38:
        push   0
        lea    ecx, [ebp - 4]
        push   0
        push   4
        push   dword ptr [ebp - 4]
        push   ecx
        call   sub_2_7908
        add    esp, 0x14
        test   eax, eax
        je     loc_2_6B58
        xor    eax, eax
        jmp    loc_2_6BFA
loc_2_6B58:
        xor    eax, eax
        push   eax
        push   eax
        push   4
        push   dword ptr [ebp - 4]
        push   eax
        call   sub_2_7908
        add    esp, 0x14
        test   eax, eax
        jne    loc_2_6B75
        xor    eax, eax
        jmp    loc_2_6BFA
loc_2_6B75:
        mov    di, 1
        mov    eax, dword ptr [ebp - 4]
        push   1
        push   esi
        mov    cl, byte ptr [eax + 2]
        lea    eax, [ebp - 8]
        shl    di, cl
        push   eax
        call   sub_2_7918
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_6BF5
        xor    esi, esi
loc_2_6B97:
        mov    eax, dword ptr [ebp - 8]
        push   esi
        push   esi
        lea    ecx, [ebp - 4]
        push   4
        mov    dword ptr [ebp - 4], eax
        push   eax
        push   ecx
        call   sub_2_7908
        add    esp, 0x14
        test   eax, eax
        jne    loc_2_6BE1
loc_2_6BB2:
        mov    eax, dword ptr [ebp - 4]
        test   byte ptr [eax], 2
        je     loc_2_6BC0
        and    word ptr [eax + 4], di
        jmp    loc_2_6BCA
loc_2_6BC0:
        mov    cx, di
        not    cx
        and    word ptr [eax + 4], cx
loc_2_6BCA:
        push   esi
        lea    ecx, [ebp - 4]
        push   esi
        push   4
        push   dword ptr [ebp - 4]
        push   ecx
        call   sub_2_7908
        add    esp, 0x14
        test   eax, eax
        je     loc_2_6BB2
loc_2_6BE1:
        push   esi
        lea    ecx, [ebp - 8]
        push   dword ptr [ebp - 8]
        push   ecx
        call   sub_2_79B0
        add    esp, 0xc
        test   eax, eax
        je     loc_2_6B97
loc_2_6BF5:
        mov    eax, 1
loc_2_6BFA:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_6C00:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x1c
        push   ebx
        lea    eax, [ebp - 4]
        push   esi
        lea    edx, [ebp - 1]
        push   edi
        mov    esi, ecx
        push   eax
        lea    ebx, [ebp - 3]
        push   edx
        push   ebx
        lea    edx, [ebp - 2]
        call   sub_2_4212
        test   byte ptr [ebp - 1], 0x20
        jne    loc_2_6CD0
        push   0
        lea    eax, [ebp - 0x18]
        push   esi
        push   eax
        call   sub_2_7958
        add    esp, 0xc
        mov    edx, dword ptr [ebp - 0x18]
        mov    ecx, esi
        call   sub_2_32C0
        test   eax, eax
        je     loc_2_6C5F
        mov    edx, dword ptr [ebp - 2]
        mov    ecx, esi
        push   dword ptr [ebp - 4]
        or     byte ptr [ebp - 1], 0x20
        push   dword ptr [ebp - 1]
        push   dword ptr [ebp - 3]
        call   sub_2_426A
        jmp    loc_2_6CD0
loc_2_6C5F:
        cmp    dword ptr [0], 0              ; -> loc_3_0368
        jne    loc_2_6C76
        mov    edx, esi
        mov    ecx, dword ptr [ebp - 0x18]
        call   sub_2_6B14
        test   eax, eax
        jne    loc_2_6CD0
loc_2_6C76:
        push   1
        lea    eax, [ebp - 0xc]
        push   esi
        push   eax
        call   sub_2_7918
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_6CD0
loc_2_6C89:
        mov    edi, dword ptr [ebp - 0xc]
        push   0
        push   edi
        lea    ecx, [ebp - 0xc]
        push   ecx
        call   sub_2_79B0
        add    esp, 0xc
        mov    ebx, eax
        mov    dword ptr [ebp - 8], edi
loc_2_6CA0:
        push   0
        lea    ecx, [ebp - 8]
        push   0
        push   4
        push   dword ptr [ebp - 8]
        push   ecx
        call   sub_2_7908
        add    esp, 0x14
        test   eax, eax
        jne    loc_2_6CCC
        mov    eax, dword ptr [ebp - 8]
        test   byte ptr [eax], 2
        je     loc_2_6CA0
        push   0
        push   edi
        call   sub_2_7AF0
        add    esp, 8
loc_2_6CCC:
        test   ebx, ebx
        je     loc_2_6C89
loc_2_6CD0:
        push   0
        lea    eax, [ebp - 0x10]
        push   esi
        push   eax
        call   sub_2_7958
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_6D83
loc_2_6CE7:
        mov    eax, dword ptr [0]            ; -> obj3:0x0DAC
        cmp    eax, dword ptr [ebp - 0x10]
        je     loc_2_6D83
        mov    ecx, dword ptr [ebp - 0x10]
        call   sub_2_42A2
        mov    dword ptr [ebp - 0x14], eax
        test   eax, eax
        je     loc_2_6D6A
        push   1
        lea    eax, [ebp - 0xc]
        push   esi
        push   eax
        call   sub_2_7918
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_6D6A
loc_2_6D17:
        mov    edi, dword ptr [ebp - 0xc]
        push   0
        push   edi
        lea    ecx, [ebp - 0xc]
        push   ecx
        call   sub_2_79B0
        add    esp, 0xc
        mov    ebx, eax
        mov    dword ptr [ebp - 8], edi
loc_2_6D2E:
        push   0
        lea    eax, [ebp - 0x1c]
        push   eax
        lea    edx, [ebp - 8]
        push   0
        push   dword ptr [ebp - 8]
        push   edx
        call   sub_2_7908
        add    esp, 0x14
        test   eax, eax
        jne    loc_2_6D66
        mov    edx, dword ptr [ebp - 0x1c]
        mov    ecx, dword ptr [ebp - 8]
        push   dword ptr [ebp - 0x14]
        call   sub_2_6A98
        test   eax, eax
        je     loc_2_6D2E
        push   0
        push   edi
        call   sub_2_7AF0
        add    esp, 8
loc_2_6D66:
        test   ebx, ebx
        je     loc_2_6D17
loc_2_6D6A:
        push   0
        lea    ecx, [ebp - 0x10]
        push   dword ptr [ebp - 0x10]
        push   ecx
        call   sub_2_7958
        add    esp, 0xc
        test   eax, eax
        je     loc_2_6CE7
loc_2_6D83:
        mov    eax, 1
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_6D8F:
        push   ebp
        mov    ecx, dword ptr [0]            ; -> obj3:0x0DAC
        mov    ebp, esp
        sub    esp, 0xc
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 0x14]
        cmp    ecx, esi
        jne    loc_2_6E53
        mov    eax, dword ptr [ebp + 8]
        cmp    eax, 1
        je     loc_2_6DD7
        cmp    eax, 3
        je     loc_2_6DF1
        cmp    eax, 4
        je     loc_2_6E08
        cmp    eax, 5
        je     loc_2_6E35
        cmp    eax, 0x10
        je     loc_2_6E3F
        cmp    eax, 0x13
        je     loc_2_6FFD
        mov    eax, 1
        jmp    loc_2_6FFF
loc_2_6DD7:
        push   0
        push   ecx
        push   0                             ; -> obj3:0x0DB8
        call   sub_2_79D0
        add    esp, 0xc
        call   sub_2_6305
        jmp    loc_2_6FFD
loc_2_6DF1:
        mov    eax, dword ptr [ebp + 0xc]
        dec    eax
        cmp    eax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 0x13
        jmp    loc_2_6FFF
loc_2_6E08:
        xor    esi, esi
        cmp    dword ptr [ebp + 0xc], esi
        jne    loc_2_6FFD
        call   sub_2_3D54
        cmp    dword ptr [0], esi            ; -> obj3:0x0DBC
        je     loc_2_6FFD
        push   esi
        mov    eax, dword ptr [0]            ; -> obj3:0x0DBC
        push   eax
        call   sub_2_7B08
        jmp    loc_2_6FFA
loc_2_6E35:
        call   sub_2_67DF
        jmp    loc_2_6FFD
loc_2_6E3F:
        call   sub_2_6305
        push   1
        push   dword ptr [ebp + 0x10]
        call   sub_2_7B00
        jmp    loc_2_6FFA
loc_2_6E53:
        push   0
        lea    eax, [ebp - 1]
        push   eax
        lea    ecx, [ebp - 3]
        push   ecx
        lea    edx, [ebp - 2]
        mov    ecx, esi
        call   sub_2_4212
        mov    edi, dword ptr [ebp + 0x10]
        cmp    esi, edi
        jne    loc_2_6F17
        test   byte ptr [ebp - 1], 1
        je     loc_2_6F17
        mov    eax, dword ptr [ebp + 8]
        cmp    eax, 3
        je     loc_2_6E98
        cmp    eax, 5
        je     loc_2_6EC9
        cmp    eax, 9
        je     loc_2_6EE6
        mov    eax, 1
        jmp    loc_2_6FFF
loc_2_6E98:
        cmp    dword ptr [ebp + 0xc], 0
        je     loc_2_6EA8
        mov    eax, 1
        jmp    loc_2_6FFF
loc_2_6EA8:
        mov    ecx, edi
        call   sub_2_42A2
        test   eax, eax
        je     loc_2_6EBF
        mov    ecx, edi
        call   sub_2_77D9
        jmp    loc_2_6FFF
loc_2_6EBF:
        mov    eax, 1
        jmp    loc_2_6FFF
loc_2_6EC9:
        push   0
        mov    edx, dword ptr [ebp - 3]
        push   0x19
        mov    ecx, dword ptr [ebp - 2]
        call   sub_2_58C5
        mov    edx, eax
        mov    ecx, edi
        call   sub_2_6794
        jmp    loc_2_6FFD
loc_2_6EE6:
        cmp    dword ptr [ebp + 0xc], 0
        je     loc_2_6EF6
        mov    eax, 1
        jmp    loc_2_6FFF
loc_2_6EF6:
        mov    ecx, edi
        call   sub_2_42A2
        test   eax, eax
        je     loc_2_6F0D
        mov    ecx, edi
        call   sub_2_77FE
        jmp    loc_2_6FFF
loc_2_6F0D:
        mov    eax, 1
        jmp    loc_2_6FFF
loc_2_6F17:
        push   0
        lea    eax, [ebp - 0xc]
        push   esi
        push   eax
        call   sub_2_7958
        add    esp, 0xc
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 6]
        lea    edx, [ebp - 5]
        push   0
        push   eax
        push   ecx
        mov    ecx, dword ptr [ebp - 0xc]
        call   sub_2_4212
        cmp    dword ptr [0], edi            ; -> obj3:0x0DAC
        jne    loc_2_6F7E
        cmp    dword ptr [ebp + 8], 0
        jne    loc_2_6F7E
        test   byte ptr [ebp - 1], 0x11
        jne    loc_2_6F7E
        test   byte ptr [ebp - 4], 1
        je     loc_2_6F7E
        test   byte ptr [ebp - 4], 4
        je     loc_2_6F7E
        mov    edx, dword ptr [ebp - 6]
        mov    ecx, dword ptr [ebp - 5]
        call   sub_2_406B
        cmp    al, 2
        jne    loc_2_6F7E
        mov    ecx, esi
        call   sub_2_6C00
        cmp    eax, 1
        sbb    eax, eax
        and    eax, 0x13
        jmp    loc_2_6FFF
loc_2_6F7E:
        cmp    dword ptr [ebp - 0xc], edi
        je     loc_2_6F8A
        mov    eax, 1
        jmp    loc_2_6FFF
loc_2_6F8A:
        test   byte ptr [ebp - 1], 1
        jne    loc_2_6F97
        mov    eax, 1
        jmp    loc_2_6FFF
loc_2_6F97:
        mov    eax, dword ptr [ebp + 8]
        cmp    eax, 1
        je     loc_2_6FBA
        cmp    eax, 2
        je     loc_2_6FC9
        cmp    eax, 4
        je     loc_2_6FC9
        cmp    eax, 6
        je     loc_2_6FD8
        cmp    eax, 0xf
        je     loc_2_6FE7
        mov    eax, 1
        jmp    loc_2_6FFF
loc_2_6FBA:
        mov    edx, dword ptr [ebp - 2]
        mov    ecx, esi
        push   dword ptr [ebp - 3]
        call   sub_2_5929
        jmp    loc_2_6FFF
loc_2_6FC9:
        mov    edx, dword ptr [ebp - 2]
        mov    ecx, esi
        push   dword ptr [ebp - 3]
        call   sub_2_5FA7
        jmp    loc_2_6FFD
loc_2_6FD8:
        mov    edx, dword ptr [ebp - 2]
        mov    ecx, esi
        push   dword ptr [ebp - 3]
        call   sub_2_639A
        jmp    loc_2_6FFD
loc_2_6FE7:
        test   byte ptr [ebp - 1], 4
        mov    eax, 0
        jne    loc_2_6FFF
        push   0
        push   esi
        call   sub_2_7AF8
loc_2_6FFA:
        add    esp, 8
loc_2_6FFD:
        xor    eax, eax
loc_2_6FFF:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_7005:
        push   esi
        push   edi
        mov    esi, edx
        mov    edi, ecx
        push   0
        push   0                             ; -> loc_2_69F3
        push   edi
        call   sub_2_7B10
        add    esp, 0xc
        push   1
        push   0                             ; -> loc_2_6D8F
        push   edi
        call   sub_2_7A10
        add    esp, 0xc
        cmp    esi, 1
        sbb    eax, eax
        and    eax, 4
        or     al, 3
        push   eax
        push   0
        push   0
        push   edi
        call   sub_2_7A08
        add    esp, 0x10
        pop    edi
        pop    esi
        ret    
loc_2_7046:
        cmp    dword ptr [0], 0              ; -> obj3:0x0DAC
        je     loc_2_7056
        mov    eax, 0x13
        jmp    loc_2_70C1
loc_2_7056:
        cmp    dword ptr [esp + 8], 1
        mov    eax, 0x13
        jne    loc_2_70C1
        mov    eax, dword ptr [esp + 4]
        push   0                             ; -> obj1:0x0000
        push   0                             ; -> obj3:0x0DD4
        mov    dword ptr [0], eax            ; -> obj3:0x0DAC
        push   0                             ; -> obj3:0x0DCC
        push   0                             ; -> obj3:0x0DC0
        push   1
        push   3
        push   3
        push   0                             ; -> obj3:0x0F1C
        push   0                             ; -> obj3:0x0DBC
        call   sub_2_7B18
        add    esp, 0x24
        xor    eax, eax
        push   eax
        push   dword ptr [0]                 ; -> obj3:0x0DAC
        push   eax
        push   0                             ; -> loc_2_162A
        push   0x43
        push   eax
        call   sub_2_79C0
        mov    ecx, dword ptr [esp + 0x1c]
        add    esp, 0x18
        mov    edx, 1
        call   sub_2_7005
        xor    eax, eax
loc_2_70C1:
        ret    8
sub_2_70C4:
        push   ebp
        mov    ebp, esp
        push   dword ptr [esp + 8]
        call   sub_2_45BC
        test   eax, eax
        je     loc_2_70F6
        test   byte ptr [eax + 0x1c], 2
        mov    ecx, dword ptr [ebp + 0xc]
        je     loc_2_70E8
        cmp    ecx, 0x100
        ja     loc_2_70E8
        shl    ecx, 2
loc_2_70E8:
        add    dword ptr [eax + 4], ecx
        mov    ecx, dword ptr [ebp + 0x10]
        cmp    dword ptr [eax + 0xc], ecx
        jae    loc_2_70F6
        mov    dword ptr [eax + 0xc], ecx
loc_2_70F6:
        pop    ebp
        ret    0xc
loc_2_70FA:
        push   ebp
        mov    ecx, dword ptr [esp + 0x10]
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        call   sub_2_42A2
        mov    edx, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 0xc], eax
        call   sub_2_406B
        movzx  eax, al
        test   eax, eax
        je     loc_2_7130
        cmp    eax, 1
        je     loc_2_7134
        cmp    eax, 2
        je     loc_2_7138
        jmp    loc_2_72DD
loc_2_7130:
        mov    bl, 6
        jmp    loc_2_713A
loc_2_7134:
        mov    bl, 2
        jmp    loc_2_713A
loc_2_7138:
        mov    bl, 1
loc_2_713A:
        push   1
        mov    edx, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        call   sub_2_40BC
        mov    esi, eax
        movzx  eax, bl
        mov    byte ptr [ebp - 1], 0x10
        lea    edi, [eax*4 + 0x10]
        cmp    edi, 0x10
        jle    loc_2_7228
        mov    ebx, 1
loc_2_7165:
        push   esi
        lea    edx, [ebp - 8]
        push   dword ptr [ebp - 1]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        lea    ecx, [ebp - 0x14]
        call   sub_2_4F1B
        test   eax, eax
        je     loc_2_720B
        test   dword ptr [ebp - 8], ebx
        je     loc_2_71B3
        mov    eax, dword ptr [ebp - 8]
        not    eax
        or     al, 3
        mov    dword ptr [ebp - 8], eax
        inc    dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 8]
        dec    eax
        test   dword ptr [ebp - 8], eax
        je     loc_2_71A4
        mov    dword ptr [ebp - 8], 4
loc_2_71A4:
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 8]
        push   0
        mov    edx, 2
        jmp    loc_2_7203
loc_2_71B3:
        test   byte ptr [ebp - 8], 8
        je     loc_2_71C4
        test   byte ptr [0], 0x40            ; -> obj3:0x0CB8
        mov    ecx, ebx
        je     loc_2_71C6
loc_2_71C4:
        xor    ecx, ecx
loc_2_71C6:
        mov    eax, dword ptr [ebp - 8]
        and    al, 7
        cmp    al, 2
        jne    loc_2_71D6
        or     dword ptr [ebp - 8], 0xfff00000
loc_2_71D6:
        and    byte ptr [ebp - 8], 0xf0
        mov    eax, dword ptr [ebp - 8]
        not    eax
        mov    dword ptr [ebp - 8], eax
        inc    dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 8]
        dec    eax
        test   dword ptr [ebp - 8], eax
        je     loc_2_71F5
        mov    dword ptr [ebp - 8], 0x10
loc_2_71F5:
        mov    edx, ebx
        cmp    ecx, ebx
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 8]
        sbb    eax, eax
        inc    eax
        push   eax
loc_2_7203:
        mov    ecx, dword ptr [ebp - 0xc]
        call   sub_2_70C4
loc_2_720B:
        mov    eax, dword ptr [ebp - 0x14]
        and    al, 7
        cmp    al, 4
        jne    loc_2_7218
        add    byte ptr [ebp - 1], 4
loc_2_7218:
        add    byte ptr [ebp - 1], 4
        movzx  eax, byte ptr [ebp - 1]
        cmp    eax, edi
        jl     loc_2_7165
loc_2_7228:
        test   esi, esi
        jne    loc_2_725F
        mov    edx, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        call   sub_2_4FCA
        mov    esi, eax
        test   esi, esi
        je     loc_2_725F
        push   0
        mov    edx, 1
        mov    ecx, dword ptr [ebp - 0xc]
        call   sub_2_45BC
        test   eax, eax
        je     loc_2_72DD
        add    dword ptr [eax + 4], esi
        cmp    dword ptr [eax + 0xc], esi
        jae    loc_2_725F
        mov    dword ptr [eax + 0xc], esi
loc_2_725F:
        mov    edx, dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 8]
        call   sub_2_406B
        movzx  eax, al
        cmp    eax, 1
        jl     loc_2_72DD
        cmp    eax, 2
        jg     loc_2_72DD
        push   0
        mov    edx, dword ptr [ebp + 8]
        push   dword ptr [ebp + 0xc]
        mov    ecx, dword ptr [ebp + 0x10]
        call   sub_2_433A
        push   eax
        lea    ecx, [ebp - 0x10]
        push   ecx
        call   sub_2_7B20
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_72DD
        mov    ecx, dword ptr [ebp - 0x10]
        call   sub_2_42A2
        test   eax, eax
        je     loc_2_72DD
        mov    cl, byte ptr [eax]
        test   cl, cl
        je     loc_2_72DD
        movzx  edi, cl
        lea    esi, [eax + 4]
loc_2_72B0:
        mov    eax, dword ptr [esi + 0x1c]
        mov    edx, dword ptr [esi + 0x20]
        and    eax, 1
        mov    ecx, dword ptr [ebp - 0xc]
        push   eax
        call   sub_2_45BC
        test   eax, eax
        je     loc_2_72D7
        mov    ecx, dword ptr [esi + 4]
        add    dword ptr [eax + 4], ecx
        mov    ecx, dword ptr [esi + 0xc]
        cmp    dword ptr [eax + 0xc], ecx
        jae    loc_2_72D7
        mov    dword ptr [eax + 0xc], ecx
loc_2_72D7:
        add    esi, 0x2c
        dec    edi
        jne    loc_2_72B0
loc_2_72DD:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_72E4:
        push   ebx
        push   esi
        push   edi
        xor    esi, esi
        mov    edi, dword ptr [esp + 0x14]
        cmp    edx, 1
        mov    dword ptr [edi], 0xffffffff
        je     loc_2_72FF
        cmp    edx, 2
        je     loc_2_731F
        jmp    loc_2_7345
loc_2_72FF:
        movzx  ebx, word ptr [ecx]
        add    ecx, 0x10
        test   ebx, ebx
        je     loc_2_7345
        mov    edx, dword ptr [ecx]
        mov    eax, dword ptr [ecx + 4]
loc_2_730E:
        cmp    dword ptr [edi], edx
        jbe    loc_2_7314
        mov    dword ptr [edi], edx
loc_2_7314:
        cmp    esi, eax
        jae    loc_2_731A
        mov    esi, eax
loc_2_731A:
        dec    ebx
        jne    loc_2_730E
        jmp    loc_2_7345
loc_2_731F:
        movzx  ebx, word ptr [ecx]
        add    ecx, 0xc
        test   ebx, ebx
        je     loc_2_7345
        movzx  edx, word ptr [ecx]
        movzx  eax, word ptr [ecx + 2]
        or     edx, 0xffff0000
loc_2_7336:
        cmp    dword ptr [edi], edx
        jbe    loc_2_733C
        mov    dword ptr [edi], edx
loc_2_733C:
        cmp    esi, eax
        jae    loc_2_7342
        mov    esi, eax
loc_2_7342:
        dec    ebx
        jne    loc_2_7336
loc_2_7345:
        mov    eax, dword ptr [esp + 0x10]
        pop    edi
        add    dword ptr [eax], esi
        pop    esi
        pop    ebx
        ret    8
sub_2_7351:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        mov    dword ptr [ebp - 0x14], ecx
        push   esi
        lea    eax, [ebp - 0xc]
        push   edi
        xor    esi, esi
        xor    edi, edi
        push   1
        push   edx
        push   eax
        call   sub_2_7918
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_73EB
        xor    ebx, ebx
loc_2_7376:
        mov    eax, dword ptr [ebp - 0xc]
        push   ebx
        push   ebx
        lea    edx, [ebp - 4]
        mov    dword ptr [ebp - 0x10], ebx
        mov    dword ptr [ebp - 4], eax
        push   dword ptr [ebp + 8]
        push   eax
        push   edx
        call   sub_2_7908
        add    esp, 0x14
        test   eax, eax
        jne    loc_2_73CF
loc_2_7395:
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0x10]
        push   eax
        mov    edx, dword ptr [ebp + 8]
        push   ecx
        mov    ecx, dword ptr [ebp - 4]
        call   sub_2_72E4
        mov    eax, dword ptr [ebp - 8]
        not    eax
        inc    eax
        mov    dword ptr [ebp - 8], eax
        cmp    edi, eax
        jae    loc_2_73B7
        mov    edi, eax
loc_2_73B7:
        push   ebx
        lea    edx, [ebp - 4]
        push   ebx
        push   dword ptr [ebp + 8]
        push   dword ptr [ebp - 4]
        push   edx
        call   sub_2_7908
        add    esp, 0x14
        test   eax, eax
        je     loc_2_7395
loc_2_73CF:
        cmp    esi, dword ptr [ebp - 0x10]
        jae    loc_2_73D7
        mov    esi, dword ptr [ebp - 0x10]
loc_2_73D7:
        push   ebx
        lea    ecx, [ebp - 0xc]
        push   dword ptr [ebp - 0xc]
        push   ecx
        call   sub_2_79B0
        add    esp, 0xc
        test   eax, eax
        je     loc_2_7376
loc_2_73EB:
        push   edi
        mov    edx, dword ptr [ebp + 8]
        push   esi
        mov    ecx, dword ptr [ebp - 0x14]
        push   0
        call   sub_2_70C4
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    4
sub_2_7403:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x90
        push   ebx
        mov    dword ptr [ebp - 0xc], ecx
        push   esi
        xor    ebx, ebx
        push   edi
        call   sub_2_42A2
        mov    esi, eax
        push   ebx
        push   ebx
        lea    eax, [ebp - 2]
        push   eax
        lea    edx, [ebp - 1]
        mov    ecx, dword ptr [ebp - 0xc]
        call   sub_2_4212
        cmp    byte ptr [esi], bl
        jbe    loc_2_7445
        lea    ecx, [esi + 8]
        xor    eax, eax
loc_2_7435:
        inc    ebx
        mov    dword ptr [ecx], eax
        mov    dword ptr [ecx + 8], eax
        add    ecx, 0x2c
        movzx  edx, byte ptr [esi]
        cmp    edx, ebx
        ja     loc_2_7435
loc_2_7445:
        push   0x19
        mov    edx, dword ptr [ebp - 2]
        mov    ecx, dword ptr [ebp - 1]
        call   sub_2_58C5
        test   al, al
        je     loc_2_746A
        cmp    byte ptr [esi], 0
        je     loc_2_746A
        push   0                             ; -> loc_2_70FA
        mov    edx, eax
        mov    ecx, dword ptr [ebp - 0xc]
        call   sub_2_65F2
loc_2_746A:
        push   0
        lea    ecx, [ebp - 0x10]
        push   dword ptr [ebp - 0xc]
        push   ecx
        call   sub_2_7968
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_74C5
        xor    edi, edi
loc_2_7481:
        push   edi
        lea    eax, [ebp - 0x44]
        push   edi
        push   edi
        push   eax
        push   dword ptr [ebp - 0x10]
        call   sub_2_7940
        add    esp, 0x14
        cmp    dword ptr [ebp - 0x44], 4
        je     loc_2_74B1
        push   2
        mov    edx, dword ptr [ebp - 0x10]
        mov    ecx, esi
        call   sub_2_7351
        push   1
        mov    edx, dword ptr [ebp - 0x10]
        mov    ecx, esi
        call   sub_2_7351
loc_2_74B1:
        push   edi
        lea    ecx, [ebp - 0x10]
        push   dword ptr [ebp - 0x10]
        push   ecx
        call   sub_2_7960
        add    esp, 0xc
        test   eax, eax
        je     loc_2_7481
loc_2_74C5:
        xor    ecx, ecx
        mov    al, byte ptr [esi]
        test   al, al
        je     loc_2_7517
        movzx  eax, al
        lea    edx, [esi + 4]
        mov    dword ptr [ebp - 0x14], eax
loc_2_74D6:
        mov    edi, dword ptr [edx + 0x18]
        mov    eax, dword ptr [edx + 4]
        add    eax, edi
        mov    ebx, edi
        mov    dword ptr [ebp - 8], eax
        mov    dword ptr [edx + 4], eax
        not    ebx
        mov    eax, ebx
        and    eax, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 8], eax
        mov    dword ptr [edx + 4], eax
        mov    eax, dword ptr [edx + 0xc]
        add    eax, edi
        and    ebx, eax
        mov    dword ptr [edx + 0xc], eax
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [edx + 0xc], ebx
        cmp    dword ptr [edx], eax
        jne    loc_2_750C
        cmp    dword ptr [edx + 8], ebx
        je     loc_2_750F
loc_2_750C:
        or     cl, 1
loc_2_750F:
        add    edx, 0x2c
        dec    dword ptr [ebp - 0x14]
        jne    loc_2_74D6
loc_2_7517:
        test   ecx, ecx
        jne    loc_2_7522
        xor    eax, eax
        jmp    loc_2_7780
loc_2_7522:
        push   0
        lea    ecx, [ebp - 0x20]
        push   dword ptr [ebp - 0xc]
        push   ecx
        call   sub_2_7918
        add    esp, 0xc
        test   eax, eax
        je     loc_2_753E
        xor    eax, eax
        jmp    loc_2_7780
loc_2_753E:
        push   0
        lea    ecx, [ebp - 0x1c]
        push   dword ptr [ebp - 0x20]
        push   ecx
        call   sub_2_79B0
        add    esp, 0xc
        test   eax, eax
        je     loc_2_755A
        xor    eax, eax
        jmp    loc_2_7780
loc_2_755A:
        lea    eax, [ebp - 0x90]
        lea    edi, [ebp - 0x20]
        mov    dword ptr [ebp - 8], eax
loc_2_7566:
        mov    dword ptr [ebp - 0xc], 0
        mov    eax, dword ptr [edi]
        mov    dword ptr [ebp - 0x18], eax
        cmp    byte ptr [esi], 0
        jbe    loc_2_75C4
        mov    eax, dword ptr [ebp - 8]
        lea    ebx, [ebp - 0x78]
        mov    dword ptr [ebp - 0x14], eax
loc_2_7580:
        mov    eax, edi
        lea    ecx, [ebp - 0x20]
        sub    eax, ecx
        push   0
        lea    edx, [ebp - 0x18]
        cmp    eax, 1
        sbb    eax, eax
        and    eax, ebx
        push   eax
        push   0
        push   dword ptr [ebp - 0x18]
        push   edx
        call   sub_2_7908
        add    esp, 0x14
        test   eax, eax
        jne    loc_2_7656
        mov    ecx, dword ptr [ebp - 0x18]
        mov    eax, dword ptr [ebp - 0x14]
        add    ebx, 4
        inc    dword ptr [ebp - 0xc]
        mov    dword ptr [eax], ecx
        add    dword ptr [ebp - 0x14], 8
        movzx  ecx, byte ptr [esi]
        cmp    ecx, dword ptr [ebp - 0xc]
        ja     loc_2_7580
loc_2_75C4:
        add    edi, 4
        lea    eax, [ebp - 0x18]
        add    dword ptr [ebp - 8], 4
        cmp    edi, eax
        jb     loc_2_7566
        mov    dword ptr [ebp - 0xc], 0
        cmp    byte ptr [esi], 0
        jbe    loc_2_7627
        lea    ebx, [ebp - 0x90]
        lea    edi, [ebp - 0x78]
loc_2_75E7:
        push   0
        lea    ecx, [ebp - 0x40]
        push   dword ptr [ebx]
        push   ecx
        call   sub_2_7AC8
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_765D
        lea    ecx, [ebx + 4]
        push   0
        push   dword ptr [ebp - 0x40]
        push   dword ptr [ebx]
        push   dword ptr [edi]
        push   dword ptr [ecx]
        push   ecx
        call   sub_2_7B28
        add    esp, 0x18
        test   eax, eax
        jne    loc_2_7664
        add    ebx, 8
        add    edi, 4
        movzx  eax, byte ptr [esi]
        inc    dword ptr [ebp - 0xc]
        cmp    eax, dword ptr [ebp - 0xc]
        ja     loc_2_75E7
loc_2_7627:
        xor    ebx, ebx
        mov    dword ptr [ebp - 0xc], ebx
        cmp    byte ptr [esi], bl
        jbe    loc_2_777B
        lea    eax, [ebp - 0x90]
        lea    edi, [esi + 4]
        mov    dword ptr [ebp - 8], eax
loc_2_7640:
        mov    eax, dword ptr [edi + 0x20]
        cmp    eax, 1
        je     loc_2_766B
        cmp    eax, 2
        je     loc_2_76F0
        jmp    loc_2_7765
loc_2_7656:
        xor    eax, eax
        jmp    loc_2_7780
loc_2_765D:
        xor    eax, eax
        jmp    loc_2_7780
loc_2_7664:
        xor    eax, eax
        jmp    loc_2_7780
loc_2_766B:
        push   ebx
        lea    edx, [ebp - 0x6c]
        push   0x28
        mov    dword ptr [ebp - 0x68], ebx
        push   edx
        mov    dword ptr [ebp - 0x64], ebx
        mov    word ptr [ebp - 0x6c], 1
        mov    word ptr [ebp - 0x6a], 0x14
        mov    eax, dword ptr [edi + 0x1c]
        mov    word ptr [ebp - 0x5e], bx
        and    eax, 1
        shl    eax, 2
        or     al, 1
        mov    word ptr [ebp - 0x60], ax
        mov    eax, dword ptr [edi + 0xc]
        dec    eax
        not    eax
        mov    dword ptr [ebp - 0x5c], eax
        mov    ecx, dword ptr [edi + 4]
        mov    dword ptr [ebp - 0x58], ecx
        mov    dword ptr [ebp - 0x54], ebx
        mov    dword ptr [ebp - 0x50], 0xffffffff
        mov    eax, dword ptr [edi + 0x1c]
        mov    word ptr [ebp - 0x4a], bx
        and    eax, 1
        shl    eax, 2
        or     al, 1
        mov    word ptr [ebp - 0x4c], ax
        movzx  ecx, byte ptr [esi + 1]
        mov    dword ptr [ebp - 0x48], ecx
        mov    ecx, dword ptr [ebp - 8]
        cmp    dword ptr [edi + 4], 1
        sbb    eax, eax
        and    eax, 0x8000
        inc    eax
        push   eax
        push   dword ptr [ecx]
        push   ecx
        call   sub_2_7B28
        add    esp, 0x18
        test   eax, eax
        je     loc_2_7765
        xor    eax, eax
        jmp    loc_2_7780
loc_2_76F0:
        mov    word ptr [ebp - 0x38], bx
        push   ebx
        mov    word ptr [ebp - 0x36], bx
        push   0x1c
        mov    word ptr [ebp - 0x34], bx
        mov    byte ptr [ebp - 0x32], bl
        mov    word ptr [ebp - 0x3c], 1
        mov    word ptr [ebp - 0x3a], 0xc
        mov    byte ptr [ebp - 0x31], bl
        mov    eax, dword ptr [edi + 0xc]
        dec    ax
        not    ax
        mov    word ptr [ebp - 0x30], ax
        lea    eax, [ebp - 0x3c]
        mov    ecx, dword ptr [edi + 4]
        push   eax
        mov    word ptr [ebp - 0x2e], cx
        mov    byte ptr [ebp - 0x25], bl
        mov    word ptr [ebp - 0x2c], bx
        mov    ecx, dword ptr [ebp - 8]
        mov    byte ptr [ebp - 0x26], 0xff
        mov    word ptr [ebp - 0x2a], 0xffff
        mov    word ptr [ebp - 0x28], bx
        movzx  edx, byte ptr [esi + 1]
        mov    dword ptr [ebp - 0x24], edx
        cmp    dword ptr [edi + 4], 1
        sbb    eax, eax
        and    eax, 0x8000
        add    eax, 2
        push   eax
        push   dword ptr [ecx]
        push   ecx
        call   sub_2_7B28
        add    esp, 0x18
        test   eax, eax
        jne    loc_2_7787
loc_2_7765:
        add    dword ptr [ebp - 8], 8
        movzx  eax, byte ptr [esi]
        add    edi, 0x2c
        inc    dword ptr [ebp - 0xc]
        cmp    eax, dword ptr [ebp - 0xc]
        ja     loc_2_7640
loc_2_777B:
        mov    eax, 1
loc_2_7780:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_2_7787:
        xor    eax, eax
        jmp    loc_2_7780
sub_2_778B:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        push   esi
        push   1
        mov    esi, ecx
        push   0x200000
        push   esi
        call   sub_2_7AD0
        add    esp, 0xc
        lea    eax, [ebp - 4]
        push   0
        push   esi
        push   eax
        call   sub_2_7968
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_77D4
loc_2_77B7:
        mov    ecx, dword ptr [ebp - 4]
        call   sub_2_778B
        push   0
        lea    ecx, [ebp - 4]
        push   dword ptr [ebp - 4]
        push   ecx
        call   sub_2_7960
        add    esp, 0xc
        test   eax, eax
        je     loc_2_77B7
loc_2_77D4:
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_2_77D9:
        push   esi
        mov    esi, ecx
        call   sub_2_7403
        test   eax, eax
        mov    eax, 0
        je     loc_2_77FC
        mov    ecx, esi
        call   sub_2_778B
        push   0
        push   esi
        call   sub_2_7B30
        add    esp, 8
loc_2_77FC:
        pop    esi
        ret    
sub_2_77FE:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x40
        push   ebx
        push   esi
        push   edi
        mov    edi, ecx
        call   sub_2_42A2
        mov    esi, eax
        push   0
        push   edi
        lea    eax, [ebp - 0x18]
        push   eax
        call   sub_2_7918
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_78F5
        push   0
        lea    ecx, [ebp - 0x14]
        push   dword ptr [ebp - 0x18]
        push   ecx
        call   sub_2_79B0
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_78F5
        lea    eax, [ebp - 0x40]
        lea    edi, [ebp - 0x18]
        mov    dword ptr [ebp - 0xc], eax
loc_2_7849:
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [edi]
        mov    dword ptr [ebp - 8], eax
        cmp    byte ptr [esi], 0
        jbe    loc_2_7895
        mov    eax, dword ptr [ebp - 0xc]
        lea    ebx, [ebp - 0x28]
        mov    dword ptr [ebp - 0x10], eax
loc_2_7863:
        push   0
        lea    ecx, [ebp - 8]
        push   ebx
        push   0
        push   dword ptr [ebp - 8]
        push   ecx
        call   sub_2_7908
        add    esp, 0x14
        test   eax, eax
        jne    loc_2_78F5
        mov    ecx, dword ptr [ebp - 8]
        mov    eax, dword ptr [ebp - 0x10]
        add    ebx, 4
        inc    dword ptr [ebp - 4]
        mov    dword ptr [eax], ecx
        add    dword ptr [ebp - 0x10], 8
        movzx  ecx, byte ptr [esi]
        cmp    ecx, dword ptr [ebp - 4]
        ja     loc_2_7863
loc_2_7895:
        add    edi, 4
        lea    eax, [ebp - 0x10]
        add    dword ptr [ebp - 0xc], 4
        cmp    edi, eax
        jb     loc_2_7849
        mov    dword ptr [ebp - 4], 0
        cmp    byte ptr [esi], 0
        jbe    loc_2_78F5
        lea    ebx, [ebp - 0x3c]
        lea    edi, [ebp - 0x28]
loc_2_78B5:
        push   0
        lea    ecx, [ebp - 0x1c]
        push   dword ptr [ebx]
        push   ecx
        call   sub_2_7AC8
        add    esp, 0xc
        test   eax, eax
        jne    loc_2_78F5
        lea    ecx, [ebx - 4]
        push   0
        push   dword ptr [ebp - 0x1c]
        push   dword ptr [ebx]
        push   dword ptr [edi]
        push   dword ptr [ecx]
        push   ecx
        call   sub_2_7B28
        add    esp, 0x18
        test   eax, eax
        jne    loc_2_78F5
        add    ebx, 8
        add    edi, 4
        movzx  eax, byte ptr [esi]
        inc    dword ptr [ebp - 4]
        cmp    eax, dword ptr [ebp - 4]
        ja     loc_2_78B5
loc_2_78F5:
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  0CCh, 0CCh
sub_2_7900:
        VxDCall  CONFIGMG._CONFIGMG_Free_Range_List ; cd2029803300
        int3   
        int3   
sub_2_7908:
        VxDCall  CONFIGMG._CONFIGMG_Get_Next_Res_Des ; cd201f803300
        int3   
        int3   
sub_2_7910:
        VxDCall  CONFIGMG._CONFIGMG_Delete_Range ; cd2026803300
        int3   
        int3   
sub_2_7918:
        VxDCall  CONFIGMG._CONFIGMG_Get_First_Log_Conf ; cd201a803300
        int3   
        int3   
sub_2_7920:
        VxDCall  CONFIGMG._CONFIGMG_Dup_Range_List ; cd2028803300
        int3   
        int3   
sub_2_7928:
        VxDCall  CONFIGMG._CONFIGMG_Add_Range    ; cd2025803300
        int3   
        int3   
sub_2_7930:
        VxDCall  CONFIGMG._CONFIGMG_Get_Device_ID ; cd2007803300
        int3   
        int3   
sub_2_7938:
        VxDCall  CONFIGMG._CONFIGMG_Find_Range   ; cd2056803300
        int3   
        int3   
sub_2_7940:
        VxDCall  CONFIGMG._CONFIGMG_Get_Bus_Info ; cd2044803300
        int3   
        int3   
sub_2_7948:
        VxDCall  CONFIGMG.Service_806Ch          ; cd206c803300
        int3   
        int3   
sub_2_7950:
        VMMCall  VMM._HeapFree                   ; cd2051800100
        int3   
        int3   
sub_2_7958:
        VxDCall  CONFIGMG._CONFIGMG_Get_Parent   ; cd2003803300
        int3   
        int3   
sub_2_7960:
        VxDCall  CONFIGMG._CONFIGMG_Get_Sibling  ; cd2005803300
        int3   
        int3   
sub_2_7968:
        VxDCall  CONFIGMG._CONFIGMG_Get_Child    ; cd2004803300
        int3   
        int3   
sub_2_7970:
        VxDCall  CONFIGMG._CONFIGMG_Get_DevNode_Status ; cd2036803300
        int3   
        int3   
sub_2_7978:
        VxDCall  CONFIGMG._CONFIGMG_Set_Bus_Info ; cd2043803300
        int3   
        int3   
sub_2_7980:
        VxDCall  CONFIGMG._CONFIGMG_Next_Range   ; cd202d803300
        int3   
        int3   
sub_2_7988:
        VxDCall  CONFIGMG._CONFIGMG_First_Range  ; cd202c803300
        int3   
        int3   
sub_2_7990:
        VxDCall  CONFIGMG._CONFIGMG_Invert_Range_List ; cd202a803300
        int3   
        int3   
sub_2_7998:
        VxDCall  CONFIGMG._CONFIGMG_Create_Range_List ; cd2024803300
        int3   
        int3   
sub_2_79A0:
        VxDCall  CONFIGMG._CONFIGMG_Add_Res_Des  ; cd201c803300
        int3   
        int3   
sub_2_79A8:
        VxDCall  CONFIGMG._CONFIGMG_Add_Empty_Log_Conf ; cd2018803300
        int3   
        int3   
sub_2_79B0:
        VxDCall  CONFIGMG._CONFIGMG_Get_Next_Log_Conf ; cd201b803300
        int3   
        int3   
sub_2_79B8:
        VxDCall  CONFIGMG._CONFIGMG_Disable_DevNode ; cd2040803300
        int3   
        int3   
sub_2_79C0:
        VxDCall  CONFIGMG._CONFIGMG_Register_Arbitrator ; cd2010803300
        int3   
        int3   
sub_2_79C8:
        VMMCall  VMM._HeapAllocate               ; cd204f800100
        int3   
        int3   
sub_2_79D0:
        VxDCall  CONFIGMG._CONFIGMG_Get_Depth    ; cd2008803300
        int3   
        int3   
sub_2_79D8:
        VxDCall  CONFIGMG._CONFIGMG_Read_Registry_Value ; cd203e803300
        int3   
        int3   
sub_2_79E0:
        VxDCall  CONFIGMG._CONFIGMG_Write_Registry_Value ; cd203f803300
        int3   
        int3   
sub_2_79E8:
        VMMCall  VMM._RegCloseKey                ; cd2049810100
        int3   
        int3   
sub_2_79F0:
        VMMCall  VMM._RegOpenKey                 ; cd2048810100
        int3   
        int3   
sub_2_79F8:
        VMMCall  VMM._Sprintf                    ; cd2074810100
        int3   
        int3   
sub_2_7A00:
        VxDCall  CONFIGMG.Service_805Fh          ; cd205f803300
        int3   
        int3   
sub_2_7A08:
        VxDCall  CONFIGMG._CONFIGMG_Register_Device_Driver ; cd200e803300
        int3   
        int3   
sub_2_7A10:
        VxDCall  CONFIGMG._CONFIGMG_Register_Enumerator ; cd200f803300
        int3   
        int3   
sub_2_7A18:
        VxDCall  CONFIGMG._CONFIGMG_Create_DevNode ; cd200b803300
        int3   
        int3   
sub_2_7A20:
        VxDCall  CONFIGMG._CONFIGMG_Get_Alloc_Log_Conf ; cd203b803300
        int3   
        int3   
        VxDCall  CONFIGMG.Service_805Eh          ; cd205e803300
        int3   
        int3   
sub_2_7A30:
        VxDCall  CONFIGMG._CONFIGMG_Remove_SubTree ; cd200d803300
        int3   
        int3   
sub_2_7A38:
        VMMCall  VMM._MapPhysToLinear            ; cd206c800100
        int3   
        int3   
sub_2_7A40:
        VMMCall  VMM._RegQueryValueEx            ; cd2051810100
        int3   
        int3   
sub_2_7A48:
        VxDCall  APM.Service_800Eh               ; cd200e802700
        int3   
        int3   
sub_2_7A50:
        VxDCall  APM.Service_800Bh               ; cd200b802700
        int3   
        int3   
sub_2_7A58:
        VxDCall  APM.Service_8007h               ; cd2007802700
        int3   
        int3   
sub_2_7A60:
        VxDCall  CONFIGMG._CONFIGMG_Get_Private_DWord ; cd2009803300
        int3   
        int3   
sub_2_7A68:
        VxDCall  CONFIGMG._CONFIGMG_Set_Private_DWord ; cd200a803300
        int3   
        int3   
sub_2_7A70:
        VMMCall  VMM._GetRegistryKey             ; cd206f810100
        int3   
        int3   
sub_2_7A78:
        VMMCall  VMM._RegSetValueEx              ; cd2052810100
        int3   
        int3   
sub_2_7A80:
        VMMCall  VMM.Service_8194h               ; cd2094810100
        int3   
        int3   
sub_2_7A88:
        VMMCall  VMM.Service_8195h               ; cd2095810100
        int3   
        int3   
sub_2_7A90:
        VMMCall  VMM.Service_8196h               ; cd2096810100
        int3   
        int3   
sub_2_7A98:
        VxDCall  VPICD.Service_8019h             ; cd2019800300
        int3   
        int3   
sub_2_7AA0:
        VMMCall  VMM._PageFree                   ; cd2055800100
        int3   
        int3   
sub_2_7AA8:
        VMMCall  VMM._LinPageLock                ; cd2063800100
        int3   
        int3   
sub_2_7AB0:
        VMMCall  VMM._PageCommitPhys             ; cd2028810100
        int3   
        int3   
sub_2_7AB8:
        VMMCall  VMM._PageReserve                ; cd201d810100
        int3   
        int3   
sub_2_7AC0:
        VxDCall  CONFIGMG._CONFIGMG_Yield        ; cd2015803300
        int3   
        int3   
sub_2_7AC8:
        VxDCall  CONFIGMG._CONFIGMG_Get_Res_Des_Data_Size ; cd2021803300
        int3   
        int3   
sub_2_7AD0:
        VxDCall  CONFIGMG.Service_805Bh          ; cd205b803300
        int3   
        int3   
sub_2_7AD8:
        VxDCall  CONFIGMG._CONFIGMG_Add_ID       ; cd2055803300
        int3   
        int3   
sub_2_7AE0:
        VxDCall  CONFIGMG._CONFIGMG_Remove_Unmarked_Children ; cd2037803300
        int3   
        int3   
sub_2_7AE8:
        VxDCall  CONFIGMG._CONFIGMG_Reset_Children_Marks ; cd2035803300
        int3   
        int3   
sub_2_7AF0:
        VxDCall  CONFIGMG._CONFIGMG_Free_Log_Conf ; cd2019803300
        int3   
        int3   
sub_2_7AF8:
        VxDCall  CONFIGMG.Service_805Ch          ; cd205c803300
        int3   
        int3   
sub_2_7B00:
        VxDCall  CONFIGMG._CONFIGMG_Reenumerate_DevNode ; cd2033803300
        int3   
        int3   
sub_2_7B08:
        VxDCall  APM.Service_800Dh               ; cd200d802700
        int3   
        int3   
sub_2_7B10:
        VxDCall  CONFIGMG._CONFIGMG_Register_Enumerator_Function ; cd2053803300
        int3   
        int3   
sub_2_7B18:
        VxDCall  APM.Service_800Ch               ; cd200c802700
        int3   
        int3   
sub_2_7B20:
        VxDCall  CONFIGMG._CONFIGMG_Locate_DevNode ; cd2002803300
        int3   
        int3   
sub_2_7B28:
        VxDCall  CONFIGMG._CONFIGMG_Modify_Res_Des ; cd201d803300
        int3   
        int3   
sub_2_7B30:
        VxDCall  CONFIGMG.Service_8061h          ; cd2061803300
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 3  vsize=0xf24  RW-
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 049h, 053h, 041h, 050h
        db  04Eh, 050h, 05Ch, 052h, 045h, 041h, 044h, 044h, 041h, 054h, 041h, 050h, 04Fh, 052h, 054h, 05Ch
        db  030h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
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
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FEh, 002h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  003h, 0FEh, 002h, 003h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 00Fh, 000h, 000h
        db  002h, 000h, 000h, 000h, 002h, 000h, 000h, 000h, 01Ch, 01Dh, 000h, 004h, 00Ch, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 0FFh, 0FFh, 00Fh, 000h, 000h, 000h, 000h, 000h
        db  001h, 000h, 000h, 000h, 020h, 022h, 001h, 004h, 014h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 0FFh, 0FFh, 00Fh, 000h, 001h, 000h, 000h, 000h, 001h, 000h, 000h, 000h
        db  024h, 026h, 001h, 004h, 014h, 000h, 000h, 000h, 002h, 0FDh, 001h, 001h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 003h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 002h, 000h, 000h, 000h
        db  02Ch, 030h, 001h, 002h, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  0FFh, 00Fh, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 000h, 000h, 01Ch, 020h, 002h, 00Ch
        db  00Ch, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  04Eh, 06Fh, 044h, 04Dh, 041h, 000h, 000h, 000h, 049h, 044h, 045h, 04Eh, 06Fh, 053h, 065h, 072h
        db  069h, 061h, 06Ch, 069h, 07Ah, 065h, 000h, 000h, 050h, 043h, 049h, 044h, 075h, 061h, 06Ch, 049h
        db  044h, 045h, 000h, 000h, 043h, 068h, 061h, 06Eh, 06Eh, 065h, 06Ch, 04Fh, 070h, 074h, 069h, 06Fh
        db  06Eh, 073h, 000h, 000h
        dd  loc_3_02E4               ; fixup
        db  000h, 000h, 000h, 000h
        dd  obj3:0x02F8              ; fixup
        db  000h, 000h, 000h, 000h
        dd  loc_3_0308               ; fixup
sub_3_02D8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_02E4:
        push   eax
        inc    ebx
        dec    ecx
        dec    ebp
        push   eax
        pop    edi
        dec    ecx
        inc    esp
        inc    ebp
        dec    ecx
        outsb  dx, byte ptr [esi]
        imul   esi, dword ptr [ecx + ebp*2 + 0x61], 0x657a696c
        add    byte ptr [eax + 0x43], dl
        dec    ecx
        dec    ebp
        push   eax
        pop    edi
        dec    ecx
        inc    esp
        inc    ebp
        inc    ebp
        js     0x36d
        je     loc_3_0306
loc_3_0306:
        add    byte ptr [eax], al
loc_3_0308:
        push   eax
        inc    ebx
        dec    ecx
        dec    ebp
        push   eax
        pop    edi
        inc    edi
        je     0x355
        push   0x656e6e61
        insb   byte ptr es:[edi], dx
        push   ebx
        je     loc_3_037C
        je     loc_3_0392
        jae    loc_3_031F
loc_3_031F:
        add    byte ptr [ecx + 0x44], cl
        inc    ebp
        dec    ebp
        imul   ebp, dword ptr [esi + 0x69], 0x74726f70
        jae    loc_3_032D
loc_3_032D:
        add    byte ptr [eax], al
        add    byte ptr [0x583830], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
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
sub_3_0354:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_3_0358:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_3_035C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_3_0360:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_3_0364:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_0368:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_036C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_0370:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_0374:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_0378:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_037C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_0380:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_0384:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_0388:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_038C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_0392:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_0418:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_041C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_0430:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_0438:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_0440:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_0448:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_0450:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_045C:
        push   eax
        inc    ebx
        dec    ecx
        dec    ebp
        push   eax
        pop    edi
        dec    ecx
        push   edx
        push   ecx
        dec    ecx
        outsb  dx, byte ptr [esi]
        imul   esi, dword ptr [ecx + ebp*2 + 0x61], 0x657a696c
        add    byte ptr [eax + 0x43], dl
        dec    ecx
        dec    ebp
        push   eax
        pop    edi
        dec    ecx
        push   edx
        push   ecx
        inc    ebp
        js     0x4e5
        je     loc_3_047E
loc_3_047E:
        add    byte ptr [eax], al
loc_3_0480:
        push   eax
        inc    ebx
        dec    ecx
        dec    ebp
        push   eax
        pop    edi
        push   ebx
        je     0x4d3
        push   edx
        push   ecx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_0490:
        push   eax
        inc    ebx
        dec    ecx
        dec    ebp
        push   eax
        pop    edi
        inc    edi
        je     0x4e3
        push   edx
        push   ecx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_04A0:
        push   eax
        inc    ebx
        dec    ecx
        dec    ebp
        push   eax
        pop    edi
        push   ebx
        je     loc_3_04FE
        jb     0x515
        jb     loc_3_04B1
loc_3_04B1:
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x43], dl
        dec    ecx
        dec    ebp
        push   eax
        pop    edi
        inc    edi
        je     0x512
        jb     0x529
        jb     loc_3_04C5
loc_3_04C5:
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x43], dl
        dec    ecx
        dec    ebp
        push   eax
        pop    edi
        push   esi
        popal  
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [ecx + 0x74], 0x62615465
        insb   byte ptr es:[edi], dx
        add    byte ptr gs:[ecx + 0x52], cl
        push   ecx
        dec    edi
        push   edx
        push   esp
        xor    byte ptr [ecx], dh
        xor    dh, byte ptr [ebx]
        xor    al, 0x35
        aaa    
        xor    byte ptr [ecx], dh
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_04F0:
        dec    ecx
        push   edx
        push   ecx
        dec    edi
        push   edx
        push   esp
        and    eax, 0x25583830
        xor    byte ptr [edx], dh
        pop    eax
loc_3_04FE:
        add    byte ptr [eax], al
loc_3_0500:
        push   eax
        inc    ebx
        dec    ecx
        pop    esp
        dec    ecx
        push   edx
        push   ecx
        dec    eax
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        jb     0x56a
        and    eax, 0x583230
        add    byte ptr [eax + 0x43], dl
        dec    ecx
        pop    esp
        dec    ecx
        push   edx
        push   ecx
        dec    eax
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        jb     0x57e
        dec    ecx
        push   edx
        push   ecx
        and    eax, 0x583230
        add    byte ptr [eax], al
loc_3_052C:
        dec    ecx
        push   edx
        push   ecx
        push   edx
        outsd  dx, dword ptr [esi]
        jne    0x5a7
        imul   ebp, dword ptr [esi + 0x67], 0x6c626154
        add    byte ptr gs:[ebx + 0x75], al
        jb     loc_3_05B2
        outsb  dx, byte ptr gs:[esi]
        je     0x596
        outsd  dx, dword ptr [esi]
        jne    loc_3_05BB
        imul   ebp, dword ptr [esi + 0x67], 0x75430000
        jb     loc_3_05C2
        outsb  dx, byte ptr gs:[esi]
        je     0x5a6
        outsd  dx, dword ptr [esi]
        jne    0x5cb
        imul   ebp, dword ptr [esi + 0x67], 0x30250000
        cmp    byte ptr [eax], bl
        add    byte ptr [eax], al
        add    byte ptr [ecx + 0x52], cl
        push   ecx
        dec    ebp
        imul   ebp, dword ptr [esi + 0x69], 0x74726f70
        jae    loc_3_0571
loc_3_0571:
        add    byte ptr [eax], al
        add    byte ptr [edi + 0x76], cl
        jb     loc_3_05EB
        imul   esp, dword ptr [ebp], 0x25000000
        xor    byte ptr [eax], bh
        pop    eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_0588:
        and    eax, 0x583830
        add    byte ptr [eax], al
        add    byte ptr [ecx + 0x52], cl
        push   ecx
        push   esp
        popal  
        bound  ebp, qword ptr [ebp + 0x53]
        je     0x5fc
        je     0x612
        jae    loc_3_059F
loc_3_059F:
        add    byte ptr [ecx + 0x52], cl
        push   ecx
        dec    ebp
        imul   ebp, dword ptr [esi + 0x69], 0x74726f70
        inc    esp
        popal  
        je     0x610
        push   ebx
        je     loc_3_0613
loc_3_05B2:
        je     loc_3_0629
        jae    loc_3_05B6
loc_3_05B6:
        add    byte ptr [eax], al
loc_3_05B8:
        dec    ecx
        push   edx
        push   ecx
loc_3_05BB:
        dec    ebp
        dec    ecx
        dec    esi
        dec    ecx
        push   eax
        dec    edi
        push   edx
loc_3_05C2:
        push   esp
        push   ebx
        je     loc_3_0627
        je     loc_3_063D
        jae    loc_3_05CA
loc_3_05CA:
        add    byte ptr [eax], al
loc_3_05CC:
        push   eax
        popal  
        je     0x638
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_05D4:
        dec    ecx
        outsb  dx, byte ptr [esi]
        jae    loc_3_064C
        popal  
        outsb  dx, byte ptr [esi]
        arpl   word ptr [ebp], sp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_05EB:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_0613:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al            ; -> obj3:0x077C
loc_3_0627:
        add    byte ptr [eax], al
loc_3_0629:
        add    byte ptr [ecx], al
        add    byte ptr [eax], al            ; -> obj3:0x0798
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [eax], eax          ; -> obj3:0x07B0
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    dword ptr [eax], eax          ; -> obj3:0x07C8
        add    byte ptr [eax], al
        add    byte ptr [edx], al
loc_3_063D:
        add    dword ptr [eax], eax          ; -> obj3:0x07DC
        add    byte ptr [eax], al
        add    byte ptr [ebx], al
        add    dword ptr [eax], eax          ; -> obj3:0x07F8
        add    byte ptr [eax], al
        add    byte ptr [ecx + eax], al
        add    byte ptr [eax], al
loc_3_064C:
        add    byte ptr [eax], al
        add    byte ptr [ecx], 0             ; -> obj3:0x0824
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, byte ptr [eax]            ; -> obj3:0x0840
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    al, byte ptr [eax]            ; -> obj3:0x0858
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        add    al, byte ptr [eax]            ; -> obj3:0x0874
        add    byte ptr [eax], al
        add    byte ptr [ebx], al
        add    al, byte ptr [eax]            ; -> obj3:0x0888
        add    byte ptr [eax], al
        add    byte ptr [eax + 2], al        ; -> obj3:0x089C
        add    byte ptr [eax], al
        add    eax, dword ptr [eax]          ; -> obj3:0x08B4
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    eax, dword ptr [eax]          ; -> obj3:0x08D8
        add    byte ptr [eax], al
        add    byte ptr [eax + 3], al        ; -> obj3:0x08F0
        add    byte ptr [eax], al
        add    al, 0                         ; -> obj3:0x0904
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    al, 0                         ; -> obj3:0x0920
        add    byte ptr [eax], al
        add    byte ptr [eax + 4], al        ; -> obj3:0x093C
        add    byte ptr [eax], al
        add    eax, 0                        ; -> obj3:0x0954
        add    dword ptr [0], eax            ; -> obj3:0x095C
        add    byte ptr [0], 0               ; -> obj3:0x0968
        push   es
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [esi], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, byte ptr [esi]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    eax, dword ptr [esi]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    al, 6
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    eax, 6                        ; -> obj3:0x09D8
        add    byte ptr [esi], al
        push   es
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        pop    es
        push   es
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [esi], 0             ; -> obj3:0x0A14
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        pop    es
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [edi], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [edi], 0             ; -> obj3:0x0A50
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        or     byte ptr [eax], al            ; -> obj3:0x0A6C
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        or     byte ptr [eax], al            ; -> obj3:0x0A94
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        or     byte ptr [eax], al            ; -> obj3:0x0AB8
        add    byte ptr [eax], al
        add    byte ptr [ebx], al
        or     byte ptr [eax], al            ; -> obj3:0x0ACC
        add    byte ptr [eax], al
        add    byte ptr [eax + 8], al        ; -> obj3:0x0AE0
        add    byte ptr [eax], al
        or     dword ptr [eax], eax          ; -> obj3:0x0AF8
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        or     dword ptr [eax], eax          ; -> obj3:0x0B10
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        or     dword ptr [eax], eax          ; -> obj3:0x0B20
        add    byte ptr [eax], al
        add    byte ptr [eax + 9], al        ; -> obj3:0x0B38
        add    byte ptr [eax], al
        or     al, byte ptr [eax]            ; -> obj3:0x0B50
        add    byte ptr [eax], al
        add    byte ptr [eax + 0xa], al      ; -> obj3:0x0B64
        add    byte ptr [eax], al
        or     eax, dword ptr [eax]          ; -> obj3:0x0B80
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        or     eax, dword ptr [eax]          ; -> obj3:0x0B8C
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        or     eax, dword ptr [eax]          ; -> obj3:0x0B98
        add    byte ptr [eax], al
        add    byte ptr [eax], dl
        or     eax, dword ptr [eax]          ; -> obj3:0x0BA8
        add    byte ptr [eax], al
        add    byte ptr [eax], ah
        or     eax, dword ptr [eax]          ; -> obj3:0x0BB8
        add    byte ptr [eax], al
        add    byte ptr [eax + 0xb], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], cl      ; -> obj3:0x0BE0
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        or     al, 0                         ; -> obj3:0x0BFC
        add    byte ptr [eax], al
        add    byte ptr [edx], al
        or     al, 0                         ; -> obj3:0x0C0C
        add    byte ptr [eax], al
        add    byte ptr [ebx], al
        or     al, 0                         ; -> obj3:0x0C14
        add    byte ptr [eax], al
        add    byte ptr [esp + ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  0FFh, 0FFh
        dd  obj3:0x0C44              ; fixup
        db  000h, 000h, 050h, 043h, 049h, 020h, 045h, 061h, 072h, 06Ch, 079h, 020h, 06Eh, 06Fh, 06Eh, 02Dh
        db  056h, 047h, 041h, 020h, 044h, 065h, 076h, 069h, 063h, 065h, 000h, 000h, 000h, 000h, 050h, 043h
        db  049h, 020h, 045h, 061h, 072h, 06Ch, 079h, 020h, 056h, 047h, 041h, 020h, 044h, 065h, 076h, 069h
        db  063h, 065h, 000h, 000h, 000h, 000h, 050h, 043h, 049h, 020h, 053h, 043h, 053h, 049h, 020h, 042h
        db  075h, 073h, 020h, 043h, 06Fh, 06Eh, 074h, 072h, 06Fh, 06Ch, 06Ch, 065h, 072h, 000h, 050h, 043h
        db  049h, 020h, 049h, 044h, 045h, 020h, 043h, 06Fh, 06Eh, 074h, 072h, 06Fh, 06Ch, 06Ch, 065h, 072h
        db  000h, 000h, 050h, 043h, 049h, 020h, 046h, 06Ch, 06Fh, 070h, 070h, 079h, 020h, 044h, 069h, 073h
        db  06Bh, 020h, 043h, 06Fh, 06Eh, 074h, 072h, 06Fh, 06Ch, 06Ch, 065h, 072h, 000h, 000h, 050h, 043h
        db  049h, 020h, 049h, 050h, 049h, 020h, 042h, 075h, 073h, 020h, 043h, 06Fh, 06Eh, 074h, 072h, 06Fh
        db  06Ch, 06Ch, 065h, 072h, 000h, 000h, 050h, 043h, 049h, 020h, 052h, 041h, 049h, 044h, 020h, 043h
        db  06Fh, 06Eh, 074h, 072h, 06Fh, 06Ch, 06Ch, 065h, 072h, 000h, 050h, 043h, 049h, 020h, 04Dh, 061h
        db  073h, 073h, 020h, 053h, 074h, 06Fh, 072h, 061h, 067h, 065h, 020h, 043h, 06Fh, 06Eh, 074h, 072h
        db  06Fh, 06Ch, 06Ch, 065h, 072h, 000h, 050h, 043h, 049h, 020h, 045h, 074h, 068h, 065h, 072h, 06Eh
        db  065h, 074h, 020h, 043h, 06Fh, 06Eh, 074h, 072h, 06Fh, 06Ch, 06Ch, 065h, 072h, 000h, 050h, 043h
        db  049h, 020h, 054h, 06Fh, 06Bh, 065h, 06Eh, 020h, 052h, 069h, 06Eh, 067h, 020h, 043h, 06Fh, 06Eh
        db  074h, 072h, 06Fh, 06Ch, 06Ch, 065h, 072h, 000h, 000h, 000h, 050h, 043h, 049h, 020h, 046h, 044h
        db  044h, 049h, 020h, 043h, 06Fh, 06Eh, 074h, 072h, 06Fh, 06Ch, 06Ch, 065h, 072h, 000h, 050h, 043h
        db  049h, 020h, 041h, 054h, 04Dh, 020h, 043h, 06Fh, 06Eh, 074h, 072h, 06Fh, 06Ch, 06Ch, 065h, 072h
        db  000h, 000h, 050h, 043h, 049h, 020h, 04Eh, 065h, 074h, 077h, 06Fh, 072h, 06Bh, 020h, 043h, 06Fh
        db  06Eh, 074h, 072h, 06Fh, 06Ch, 06Ch, 065h, 072h, 000h, 000h, 050h, 043h, 049h, 020h, 056h, 047h
        db  041h, 02Dh, 043h, 06Fh, 06Dh, 070h, 061h, 074h, 069h, 062h, 06Ch, 065h, 020h, 044h, 069h, 073h
        db  070h, 06Ch, 061h, 079h, 020h, 041h, 064h, 061h, 070h, 074h, 065h, 072h, 000h, 000h, 050h, 043h
        db  049h, 020h, 058h, 047h, 041h, 020h, 044h, 069h, 073h, 070h, 06Ch, 061h, 079h, 020h, 041h, 064h
        db  061h, 070h, 074h, 065h, 072h, 000h, 050h, 043h, 049h, 020h, 044h, 069h, 073h, 070h, 06Ch, 061h
        db  079h, 020h, 041h, 064h, 061h, 070h, 074h, 065h, 072h, 000h, 050h, 043h, 049h, 020h, 04Dh, 075h
        db  06Ch, 074h, 069h, 06Dh, 065h, 064h, 069h, 061h, 020h, 056h, 069h, 064h, 065h, 06Fh, 020h, 044h
        db  065h, 076h, 069h, 063h, 065h, 000h, 050h, 043h, 049h, 020h, 04Dh, 075h, 06Ch, 074h, 069h, 06Dh
        db  065h, 064h, 069h, 061h, 020h, 041h, 075h, 064h, 069h, 06Fh, 020h, 044h, 065h, 076h, 069h, 063h
        db  065h, 000h, 050h, 043h, 049h, 020h, 04Dh, 075h, 06Ch, 074h, 069h, 06Dh, 065h, 064h, 069h, 061h
        db  020h, 044h, 065h, 076h, 069h, 063h, 065h, 000h, 000h, 000h, 050h, 043h, 049h, 020h, 052h, 041h
        db  04Dh, 000h, 050h, 043h, 049h, 020h, 046h, 06Ch, 061h, 073h, 068h, 000h, 000h, 000h, 050h, 043h
        db  049h, 020h, 04Dh, 065h, 06Dh, 06Fh, 072h, 079h, 020h, 043h, 06Fh, 06Eh, 074h, 072h, 06Fh, 06Ch
        db  06Ch, 065h, 072h, 000h, 000h, 000h, 050h, 043h, 049h, 020h, 048h, 06Fh, 073h, 074h, 020h, 042h
        db  072h, 069h, 064h, 067h, 065h, 000h, 050h, 043h, 049h, 020h, 049h, 053h, 041h, 020h, 042h, 072h
        db  069h, 064h, 067h, 065h, 000h, 000h, 050h, 043h, 049h, 020h, 045h, 049h, 053h, 041h, 020h, 042h
        db  072h, 069h, 064h, 067h, 065h, 000h, 050h, 043h, 049h, 020h, 04Dh, 043h, 041h, 020h, 042h, 072h
        db  069h, 064h, 067h, 065h, 000h, 000h, 050h, 043h, 049h, 020h, 050h, 043h, 049h, 02Dh, 074h, 06Fh
        db  02Dh, 050h, 043h, 049h, 020h, 042h, 072h, 069h, 064h, 067h, 065h, 000h, 000h, 000h, 050h, 043h
        db  049h, 020h, 050h, 043h, 04Dh, 043h, 049h, 041h, 020h, 042h, 072h, 069h, 064h, 067h, 065h, 000h
        db  000h, 000h, 050h, 043h, 049h, 020h, 04Eh, 075h, 042h, 075h, 073h, 020h, 042h, 072h, 069h, 064h
        db  067h, 065h, 000h, 000h, 000h, 000h, 050h, 043h, 049h, 020h, 043h, 061h, 072h, 064h, 042h, 075h
        db  073h, 020h, 042h, 072h, 069h, 064h, 067h, 065h, 000h, 000h, 050h, 043h, 049h, 020h, 042h, 072h
        db  069h, 064h, 067h, 065h, 000h, 000h, 050h, 043h, 049h, 020h, 053h, 065h, 072h, 069h, 061h, 06Ch
        db  020h, 043h, 06Fh, 06Eh, 074h, 072h, 06Fh, 06Ch, 06Ch, 065h, 072h, 000h, 000h, 000h, 050h, 043h
        db  049h, 020h, 050h, 061h, 072h, 061h, 06Ch, 06Ch, 065h, 06Ch, 020h, 043h, 06Fh, 06Eh, 074h, 072h
        db  06Fh, 06Ch, 06Ch, 065h, 072h, 000h, 050h, 043h, 049h, 020h, 043h, 06Fh, 06Dh, 06Dh, 075h, 06Eh
        db  069h, 063h, 061h, 074h, 069h, 06Fh, 06Eh, 020h, 044h, 065h, 076h, 069h, 063h, 065h, 000h, 000h
        db  000h, 000h, 050h, 043h, 049h, 020h, 050h, 072h, 06Fh, 067h, 072h, 061h, 06Dh, 06Dh, 061h, 062h
        db  06Ch, 065h, 020h, 049h, 06Eh, 074h, 065h, 072h, 072h, 075h, 070h, 074h, 020h, 043h, 06Fh, 06Eh
        db  074h, 072h, 06Fh, 06Ch, 06Ch, 065h, 072h, 000h, 000h, 000h, 050h, 043h, 049h, 020h, 044h, 069h
        db  072h, 065h, 063h, 074h, 020h, 04Dh, 065h, 06Dh, 06Fh, 072h, 079h, 020h, 041h, 063h, 063h, 065h
        db  073h, 073h, 020h, 043h, 06Fh, 06Eh, 074h, 072h, 06Fh, 06Ch, 06Ch, 065h, 072h, 000h, 050h, 043h
        db  049h, 020h, 053h, 079h, 073h, 074h, 065h, 06Dh, 020h, 054h, 069h, 06Dh, 065h, 072h, 000h, 000h
        db  000h, 000h, 050h, 043h, 049h, 020h, 052h, 065h, 061h, 06Ch, 020h, 054h, 069h, 06Dh, 065h, 020h
        db  043h, 06Ch, 06Fh, 063h, 06Bh, 000h, 050h, 043h, 049h, 020h, 053h, 079h, 073h, 074h, 065h, 06Dh
        db  020h, 050h, 065h, 072h, 069h, 070h, 068h, 065h, 072h, 061h, 06Ch, 000h, 000h, 000h, 050h, 043h
        db  049h, 020h, 04Bh, 065h, 079h, 062h, 06Fh, 061h, 072h, 064h, 020h, 043h, 06Fh, 06Eh, 074h, 072h
        db  06Fh, 06Ch, 06Ch, 065h, 072h, 000h, 050h, 043h, 049h, 020h, 044h, 069h, 067h, 069h, 074h, 069h
        db  07Ah, 065h, 072h, 000h, 000h, 000h, 050h, 043h, 049h, 020h, 04Dh, 06Fh, 075h, 073h, 065h, 020h
        db  043h, 06Fh, 06Eh, 074h, 072h, 06Fh, 06Ch, 06Ch, 065h, 072h, 000h, 000h, 000h, 000h, 050h, 043h
        db  049h, 020h, 049h, 06Eh, 070h, 075h, 074h, 020h, 043h, 06Fh, 06Eh, 074h, 072h, 06Fh, 06Ch, 06Ch
        db  065h, 072h, 000h, 000h, 000h, 000h, 050h, 043h, 049h, 020h, 044h, 06Fh, 063h, 06Bh, 069h, 06Eh
        db  067h, 020h, 053h, 074h, 061h, 074h, 069h, 06Fh, 06Eh, 000h, 050h, 043h, 049h, 020h, 04Fh, 074h
        db  068h, 065h, 072h, 020h, 044h, 06Fh, 063h, 06Bh, 069h, 06Eh, 067h, 020h, 053h, 074h, 061h, 074h
        db  069h, 06Fh, 06Eh, 000h, 000h, 000h, 050h, 043h, 049h, 020h, 033h, 038h, 036h, 020h, 043h, 050h
        db  055h, 000h, 050h, 043h, 049h, 020h, 034h, 038h, 036h, 020h, 043h, 050h, 055h, 000h, 050h, 043h
        db  049h, 020h, 050h, 065h, 06Eh, 074h, 069h, 075h, 06Dh, 020h, 043h, 050h, 055h, 000h, 050h, 043h
        db  049h, 020h, 041h, 06Ch, 070h, 068h, 061h, 020h, 043h, 050h, 055h, 000h, 000h, 000h, 050h, 043h
        db  049h, 020h, 050h, 06Fh, 077h, 065h, 072h, 050h, 043h, 020h, 043h, 050h, 055h, 000h, 050h, 043h
        db  049h, 020h, 043h, 06Fh, 02Dh, 070h, 072h, 06Fh, 063h, 065h, 073h, 073h, 06Fh, 072h, 020h, 043h
        db  050h, 055h, 000h, 000h, 000h, 000h, 050h, 043h, 049h, 020h, 046h, 069h, 072h, 065h, 057h, 069h
        db  072h, 065h, 020h, 028h, 049h, 045h, 045h, 045h, 020h, 031h, 033h, 039h, 034h, 029h, 000h, 000h
        db  000h, 000h, 050h, 043h, 049h, 020h, 041h, 063h, 063h, 065h, 073h, 073h, 020h, 042h, 075h, 073h
        db  000h, 000h, 050h, 043h, 049h, 020h, 053h, 053h, 041h, 000h, 050h, 043h, 049h, 020h, 055h, 06Eh
        db  069h, 076h, 065h, 072h, 073h, 061h, 06Ch, 020h, 053h, 065h, 072h, 069h, 061h, 06Ch, 020h, 042h
        db  075h, 073h, 000h, 000h, 000h, 000h, 050h, 043h, 049h, 020h, 046h, 069h, 062h, 072h, 065h, 020h
        db  043h, 068h, 061h, 06Eh, 06Eh, 065h, 06Ch, 000h, 000h, 000h, 050h, 043h, 049h, 020h, 043h, 061h
        db  072h, 064h, 000h, 000h, 000h, 000h, 025h, 030h, 038h, 058h, 000h, 000h, 000h, 000h, 050h, 043h
        db  049h, 05Ch, 056h, 045h, 04Eh, 05Fh, 025h, 030h, 034h, 058h, 026h, 044h, 045h, 056h, 05Fh, 025h
        db  030h, 034h, 058h, 05Ch, 042h, 055h, 053h, 05Fh, 025h, 030h, 032h, 058h, 026h, 044h, 045h, 056h
        db  05Fh, 025h, 030h, 032h, 058h, 026h, 046h, 055h, 04Eh, 043h, 05Fh, 025h, 030h, 032h, 058h, 000h
        db  000h, 000h, 050h, 043h, 049h, 05Ch, 056h, 045h, 04Eh, 05Fh, 025h, 030h, 034h, 058h, 026h, 044h
        db  045h, 056h, 05Fh, 025h, 030h, 034h, 058h, 05Ch, 000h, 000h, 025h, 030h, 032h, 058h, 000h, 000h
        db  000h, 000h, 025h, 030h, 032h, 058h, 000h, 000h, 000h, 000h, 050h, 043h, 049h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_3_0CC8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_0CCC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_0CD0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_0CD4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_0CD8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_0CDC:
        inc    dword ptr [eax]
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 000h, 000h, 0FFh, 0FFh, 0FFh, 0FFh, 080h, 0CFh, 0D8h, 0F1h, 0AEh, 029h, 0CFh, 011h
        db  097h, 0EAh, 000h, 0AAh, 000h, 034h, 031h, 09Dh, 020h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 05Fh, 033h, 032h, 05Fh, 000h, 000h, 000h, 000h, 04Fh, 070h, 074h, 069h
        db  06Fh, 06Eh, 073h, 000h, 04Fh, 070h, 074h, 069h, 06Fh, 06Eh, 073h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        dd  obj3:0x0DE0              ; fixup
        dd  obj3:0x0DF0              ; fixup
        dd  obj3:0x0E00              ; fixup
        db  000h, 000h, 001h, 000h, 002h, 000h, 000h, 000h
        dd  loc_2_4CB8               ; fixup
        dd  loc_2_4CBE               ; fixup
        dd  loc_2_4CD2               ; fixup
        db  050h, 043h, 049h, 047h, 065h, 074h, 056h, 065h, 072h, 073h, 069h, 06Fh, 06Eh, 000h, 000h, 000h
        db  050h, 043h, 049h, 052h, 065h, 061h, 064h, 043h, 06Fh, 06Eh, 066h, 069h, 067h, 000h, 000h, 000h
        db  050h, 043h, 049h, 057h, 072h, 069h, 074h, 065h, 043h, 06Fh, 06Eh, 066h, 069h, 067h, 000h, 000h
        db  044h, 065h, 076h, 069h, 063h, 065h, 044h, 065h, 073h, 063h, 000h, 000h, 044h, 065h, 076h, 069h
        db  063h, 065h, 044h, 065h, 073h, 063h, 000h, 000h, 025h, 030h, 033h, 064h, 000h, 000h, 000h, 000h
        db  048h, 057h, 052h, 065h, 076h, 069h, 073h, 069h, 06Fh, 06Eh, 000h, 000h, 050h, 043h, 049h, 05Ch
        db  043h, 043h, 05Fh, 025h, 030h, 036h, 058h, 000h, 044h, 065h, 076h, 069h, 063h, 065h, 044h, 065h
        db  073h, 063h, 000h, 000h, 050h, 043h, 049h, 020h, 042h, 075h, 073h, 020h, 04Dh, 061h, 073h, 074h
        db  065h, 072h, 020h, 049h, 044h, 045h, 020h, 043h, 06Fh, 06Eh, 074h, 072h, 06Fh, 06Ch, 06Ch, 065h
        db  072h, 000h, 000h, 000h, 044h, 065h, 076h, 069h, 063h, 065h, 044h, 065h, 073h, 063h, 000h, 000h
        db  050h, 043h, 049h, 05Ch, 043h, 043h, 05Fh, 030h, 031h, 030h, 031h, 038h, 030h, 000h, 000h, 000h
        db  050h, 043h, 049h, 05Ch, 043h, 043h, 05Fh, 025h, 030h, 034h, 058h, 000h, 050h, 043h, 049h, 05Ch
        db  056h, 045h, 04Eh, 05Fh, 025h, 030h, 034h, 058h, 026h, 044h, 045h, 056h, 05Fh, 025h, 030h, 034h
        db  058h, 026h, 053h, 055h, 042h, 053h, 059h, 053h, 05Fh, 025h, 030h, 038h, 058h, 026h, 052h, 045h
        db  056h, 05Fh, 025h, 030h, 032h, 058h, 000h, 000h, 050h, 043h, 049h, 05Ch, 056h, 045h, 04Eh, 05Fh
        db  025h, 030h, 034h, 058h, 026h, 044h, 045h, 056h, 05Fh, 025h, 030h, 034h, 058h, 026h, 053h, 055h
        db  042h, 053h, 059h, 053h, 05Fh, 025h, 030h, 038h, 058h, 000h, 000h, 000h, 050h, 043h, 049h, 05Ch
        db  056h, 045h, 04Eh, 05Fh, 025h, 030h, 034h, 058h, 026h, 044h, 045h, 056h, 05Fh, 025h, 030h, 034h
        db  058h, 026h, 052h, 045h, 056h, 05Fh, 025h, 030h, 032h, 058h, 000h, 000h, 025h, 030h, 038h, 058h
        db  000h, 000h, 000h, 000h, 025h, 030h, 038h, 058h, 000h, 000h, 000h, 000h, 050h, 043h, 049h, 02Eh
        db  044h, 04Ch, 04Ch, 000h
VxD__LDATA ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0xf2  R-X
loc_4_0000:
        push   eax
        inc    ebx
        dec    ecx
        push   edx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   ebx
        xor    eax, eax
        push   esi
        mov    dword ptr [0], eax            ; -> obj3:0x0D88
        mov    dword ptr [0], 0xffffffff     ; -> obj3:0x0CFC
        push   edi
        push   ebp
        push   eax
        push   0x28000
        push   0xc0000
        call   sub_2_7A38
loc_4_002D:
        add    esp, 0xc
        mov    esi, eax
        lea    edi, [eax + 0x20000]
        cmp    eax, edi
        ja     loc_4_00C2
        mov    ecx, dword ptr [0]            ; -> obj3:0x0D88
        imul   ecx, ecx, 0xe
        lea    edx, [ecx]                    ; -> obj3:0x0CFC
loc_4_004F:
        cmp    word ptr [esi], 0xaa55
        jne    loc_4_00B8
        movzx  ebx, byte ptr [esi + 2]
        shl    ebx, 9
        test   ebx, ebx
        jne    loc_4_0066
        mov    ebx, 0x800
loc_4_0066:
        movzx  ecx, word ptr [esi + 0x18]
        lea    ebp, [ecx + 3]
        cmp    ebp, ebx
        jbe    loc_4_0075
        add    esi, ebx
        jmp    loc_4_00BE
loc_4_0075:
        add    ecx, esi
        mov    ebp, dword ptr [0]            ; -> loc_4_0000
        cmp    dword ptr [ecx], ebp
        jne    loc_4_00A9
        mov    ecx, dword ptr [ecx + 4]
        inc    dword ptr [0]                 ; -> obj3:0x0D88
        mov    dword ptr [edx], ecx
        mov    ecx, esi
        sub    ecx, eax
        add    edx, 0xe
        add    ecx, 0xc0000
        mov    dword ptr [edx - 0xa], ecx
        mov    dword ptr [edx - 6], ebx
        mov    byte ptr [edx - 2], 0
        mov    dword ptr [edx], 0xffffffff
loc_4_00A9:
        lea    esi, [ebx + esi + 0x7ff]
        and    esi, 0xfffff800
        jmp    loc_4_00BE
loc_4_00B8:
        add    esi, 0x800
loc_4_00BE:
        cmp    esi, edi
        jbe    loc_4_004F
loc_4_00C2:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_4_00C7:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        VxDCall  CONFIGMG._CONFIGMG_Get_Version  ; cd2000003300
        mov    word ptr [ebp - 2], ax
        cmp    word ptr [ebp - 2], 0x400
        mov    eax, 0
        jb     loc_4_00EE
        call   8
        mov    eax, 1
loc_4_00EE:
        mov    esp, ebp
        pop    ebp
        ret    
VxD__LTEXT ENDS

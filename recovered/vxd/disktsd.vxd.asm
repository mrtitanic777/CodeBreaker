; DiskTSD -- reconstructed 32-bit disassembly (VxD / LE format)
; 3 objects, 2942 instructions recovered by recursive descent

Declare_Virtual_Device DiskTSD, 3, 10, DiskTSD_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 080000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x2c0  R-X
DiskTSD_Control:
        cmp    eax, 0x1b
        je     loc_2_0000
        clc    
        ret    
        db  000h
loc_1_000C:
        mov    ebx, dword ptr [esp + 4]
        mov    word ptr [ebx + 2], 0
        mov    si, word ptr [ebx]
        cmp    si, 0
        je     loc_3_0000
        cmp    si, 3
        je     loc_3_008C
        cmp    si, 4
        je     loc_3_0155
        cmp    si, 2
        je     loc_3_021D
        call   sub_1_0047
        ret    
sub_1_0047:
        mov    word ptr [ebx + 2], 0xffff
        ret    
        db  0C3h, 000h
sub_1_0050:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_005C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_0064:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_006C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0084:
        push   ebx
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x10]
        mov    bx, word ptr [edi + 0x68]
        cmp    bl, 2
        ja     loc_1_009D
        call   sub_1_0120
loc_1_0099:
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_1_009D:
        cmp    bl, 0xc
        je     loc_1_00AE
        cmp    bl, 0xd
        je     loc_1_00C9
loc_1_00A7:
        call   sub_1_0135
        jmp    loc_1_0099
loc_1_00AE:
        mov    esi, dword ptr [edi + 4]
        push   edi
        call   sub_3_05A8
        pop    edi
loc_1_00B8:
        mov    eax, dword ptr [edi + 0x14]
        sub    eax, 8
        mov    dword ptr [edi + 0x14], eax
        push   edi
        call   dword ptr [eax]
        add    esp, 4
        jmp    loc_1_0099
loc_1_00C9:
        test   byte ptr [edi + 0x6d], 0x40
        jne    loc_1_00E1
        mov    ebx, dword ptr [edi + 4]
        test   byte ptr [ebx + 0xb8], 4
        jne    loc_1_00E1
        test   byte ptr [ebx + 0x22], 8
        jne    loc_1_0107
loc_1_00E1:
        movzx  ebx, word ptr [edi + 0x92]
        and    ebx, 0xff
        cmp    ebx, 0x12
        jg     loc_1_00A7
        call   sub_3_12E8
        or     al, al
        je     loc_1_00B8
        dec    al
        je     loc_1_0099
        jmp    loc_1_00A7
loc_1_0107:
        mov    ebx, 0x1f
        cmp    word ptr [edi + 0x6a], 0x18
        jne    loc_1_0118
        mov    ebx, 0x15
loc_1_0118:
        mov    dword ptr [edi + 0xa0], ebx
        jmp    loc_1_00B8
sub_1_0120:
        mov    eax, dword ptr [edi + 8]
        test   byte ptr [eax + 0x21], 0x40
        je     loc_1_012F
        test   byte ptr [eax + 0x16], 0x80
        jne    loc_1_0159
loc_1_012F:
        test   byte ptr [edi + 0x6e], 8
        jne    loc_1_0147
sub_1_0135:
        mov    eax, dword ptr [edi + 0x10]
        mov    eax, dword ptr [eax + 0xc]
        mov    dword ptr [edi + 0x10], eax
        push   edi
        call   dword ptr [eax]
        add    esp, 4
        xor    eax, eax
        ret    
loc_1_0147:
        bts    dword ptr [edi + 0x6c], 0x14
        jb     sub_1_0135
        mov    ebx, dword ptr [edi + 8]
        mov    eax, dword ptr [ebx + 0x28]
        add    dword ptr [edi + 0x74], eax
        jmp    sub_1_0135
loc_1_0159:
        mov    word ptr [edi + 0x6a], 0x20
        mov    eax, dword ptr [edi + 0x14]
        sub    eax, 8
        mov    dword ptr [edi + 0x14], eax
        push   edi
        call   dword ptr [eax]
        add    esp, 4
        ret    
loc_1_016F:
        enter  0, 0
        push   edi
        mov    edi, dword ptr [ebp + 8]
        cmp    word ptr [edi + 0x6a], 2
        ja     loc_1_01A7
        cmp    word ptr [edi + 0x68], 0xe
        je     loc_1_01EF
loc_1_0185:
        mov    dword ptr [edi + 0xa0], 0
loc_1_018F:
        mov    word ptr [edi + 0x68], 0xd
        mov    eax, dword ptr [edi + 0x14]
        sub    eax, 8
        mov    dword ptr [edi + 0x14], eax
        push   edi
        call   dword ptr [eax]
        add    esp, 4
loc_1_01A4:
        pop    edi
        leave  
        ret    
loc_1_01A7:
        mov    ebx, dword ptr [edi + 0x14]
        mov    ebx, dword ptr [ebx + 4]
        dec    word ptr [ebx]
        je     loc_1_01DA
        cmp    word ptr [edi + 0x68], 2
        jne    loc_1_01C9
        cmp    word ptr [edi + 0x92], 0x42
        jne    loc_1_01C9
        mov    word ptr [edi + 0x68], 0xe
loc_1_01C9:
        push   esi
        mov    esi, 0x22d                    ; -> loc_1_022D
        mov    edx, edi
        VMMCall  VMM.Schedule_Global_Event       ; cd200e000100
        pop    esi
        jmp    loc_1_01A4
loc_1_01DA:
        movzx  ecx, word ptr [edi + 0x6a]
        VxDCall  BLOCKDEV.Service_000Fh          ; cd200f001000
        mov    dword ptr [edi + 0xa0], eax
        jmp    loc_1_018F
loc_1_01EC:
        pop    esi
        jmp    loc_1_0185
loc_1_01EF:
        push   esi
        mov    esi, dword ptr [edi + 8]
        test   byte ptr [esi + 0x24], 0x40
        jne    loc_1_01EC
        movzx  eax, word ptr [edi + 0x5e]
        mov    ecx, dword ptr [esi + 0x60]
        mul    ecx
        or     edx, edx
        mov    ecx, dword ptr [edi + 0x60]
        mov    dword ptr [edi + 0x7c], ecx
        mul    ecx
        or     edx, edx
        mov    ebx, eax
        movzx  eax, byte ptr [edi + 0x5c]
        mul    ecx
        or     edx, edx
        add    eax, ebx
        mov    dword ptr [edi + 0x74], eax
        mov    dword ptr [edi + 0x78], 0
        mov    word ptr [edi + 0x68], 2
        pop    esi
        jmp    loc_1_01C9
loc_1_022D:
        push   esi
        push   edi
        mov    esi, edx
        call   sub_1_0244
        add    dword ptr [esi + 0x14], 8
        mov    edi, esi
        call   sub_1_0135
        pop    edi
        pop    esi
        ret    
sub_1_0244:
        push   ebx
        mov    ebx, dword ptr [esi + 8]
        mov    eax, dword ptr [ebx + 8]
loc_1_024B:
        cmp    dword ptr [eax], 0x84         ; -> loc_1_0084
        je     loc_1_025A
        mov    eax, dword ptr [eax + 0xc]
        or     eax, eax
        jne    loc_1_024B
loc_1_025A:
        mov    dword ptr [esi + 0x10], eax
        pop    ebx
        ret    
        db  000h
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  3, 10                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'DiskTSD '                  ; DDB_Name
        dd  080000000h                  ; DDB_Init_Order
        dd  DiskTSD_Control          ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
loc_1_02BC:
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x174  R-X
loc_2_0000:
        mov    ah, 0x30
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        xchg   ah, al
        cmp    ax, 0x70a
        jb     loc_2_003B
        call   sub_2_003C
        mov    dword ptr [0x145], 0xc        ; -> obj2:0x0145
        mov    dword ptr [0x149], 0x50       ; -> obj2:0x0149
        push   0x139                         ; -> loc_2_0139
        VxDCall  BLOCKDEV.Service_0007h          ; cd2007001000
        add    esp, 4
        xor    eax, eax
loc_2_003B:
        ret    
sub_2_003C:
        sub    esp, 0x10
        lea    esi, [0xdc]                   ; -> loc_2_00DC
        lea    edi, [esp]
        push   edi
        push   esi
        push   0x80000002
        VMMCall  VMM._RegOpenKey                 ; cd2048010100
        add    esp, 0xc
        or     eax, eax
        jne    loc_2_00D7
        lea    ebx, [0x119]                  ; -> loc_2_0119
loc_2_0062:
        mov    ecx, dword ptr [ebx + 4]
        or     ecx, ecx
        je     loc_2_00C5
        mov    eax, dword ptr [ebx + 8]
        lea    edx, [esp + 4]
        lea    esi, [esp + 0xc]
        lea    edi, [esp + 8]
        mov    dword ptr [edi], ecx
        mov    ecx, dword ptr [esp]
        push   edi
        push   esi
        push   edx
        push   0
        push   eax
        push   ecx
        VMMCall  VMM._RegQueryValueEx            ; cd2051010100
        add    esp, 0x18
        or     eax, eax
        jne    loc_2_00C0
        mov    eax, dword ptr [esp + 4]
        cmp    eax, dword ptr [ebx]
        je     loc_2_00A9
        cmp    eax, 4
        jne    loc_2_00A7
        cmp    dword ptr [ebx], 3
        jne    loc_2_00A7
        cmp    dword ptr [ebx + 4], 4
loc_2_00A7:
        jne    loc_2_00C0
loc_2_00A9:
        mov    edi, dword ptr [ebx + 0xc]
        mov    ecx, dword ptr [ebx + 4]
        sub    al, al
        rep stosb byte ptr es:[edi], al
        lea    esi, [esp + 0xc]
        mov    edi, dword ptr [ebx + 0xc]
        mov    ecx, dword ptr [esp + 8]
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_2_00C0:
        add    ebx, 0x10
        jmp    loc_2_0062
loc_2_00C5:
        mov    eax, dword ptr [esp]
        push   eax
        VMMCall  VMM._RegCloseKey                ; cd2049010100
        add    esp, 4
        add    esp, 0x10
        clc    
        ret    
loc_2_00D7:
        add    esp, 0x10
        stc    
        ret    
loc_2_00DC:
        push   ebx
        jns    0x152
        je     loc_2_0146
        insd   dword ptr es:[edi], dx
        pop    esp
        inc    ebx
        jne    0x158
        jb     0x14d
        outsb  dx, byte ptr [esi]
        je     loc_2_012E
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        je     loc_2_0161
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        push   ebx
        je     loc_2_0151
        inc    ebx
        outsd  dx, dword ptr [esi]
        outsb  dx, byte ptr [esi]
        je     0x16c
        outsd  dx, dword ptr [esi]
        insb   byte ptr es:[edi], dx
        pop    esp
        inc    esi
        imul   ebp, dword ptr [ebp + 0x53], 0x65747379
        insd   dword ptr es:[edi], dx
        add    byte ptr [edi + 0x65], al
        je     0x150
        jbe    loc_2_015F
        popal  
        jb     0x17f
        jae    loc_2_015D
        outsd  dx, dword ptr [esi]
        arpl   word ptr [esp + ebp*2], si
loc_2_0119:
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [eax], al
        or     byte ptr [ecx], al
        add    byte ptr [eax], al
        mov    esp, 2
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_012E:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_2_0139:
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        add    byte ptr [eax], 0
        add    byte ptr [eax], al
loc_2_0146:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [esi + 0x69], al
        js     0x1b6
loc_2_0151:
        and    byte ptr fs:[ecx + ebp*2 + 0x73], al
        imul   esp, dword ptr [eax], 0x54
        push   ebx
        inc    esp
        and    byte ptr [eax], ah
loc_2_015D:
        add    byte ptr [eax], al
loc_2_015F:
        add    byte ptr [eax], al
loc_2_0161:
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

VxD__LTEXT SEGMENT   ; object 3  vsize=0x213a  R-X
loc_3_0000:
        call   sub_3_0236
        push   ebp
        sub    esp, 0xc
        mov    ebp, esp
        mov    word ptr [ebp], 0
        mov    word ptr [ebp + 4], 0x90
        push   ebp
        call   sub_1_0050
        mov    edi, dword ptr [ebp + 6]
        add    esp, 0x10
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    ebp, dword ptr [ebx + 8]
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_V86_Exec         ; cd2082000100
        mov    eax, 0x803
        mov    word ptr [ebp + 0x1c], 0x803
        mov    eax, 0x2f
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        movzx  ecx, word ptr [ebp + 0x3c]
        shl    ecx, 4
        movzx  eax, word ptr [ebp]
        add    ecx, eax
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        add    ecx, dword ptr [ebx + 4]
        mov    dword ptr [0x2b3], ecx        ; -> obj1:0x02B3
        pop    ebp
        ret    
        db  000h
loc_3_008C:
        mov    esi, dword ptr [ebx + 0xc]
        test   dword ptr [esi + 0x20], 0x6000
        jne    loc_3_0154
        cmp    byte ptr [esi + 0x40], 0
        je     loc_3_00AC
        cmp    byte ptr [esi + 0x40], 0xa
        jne    loc_3_0154
loc_3_00AC:
        test   dword ptr [esi + 0x1c], 0x23
        je     loc_3_00BD
        mov    eax, dword ptr [esi + 0x1c]
        jmp    loc_3_0154
loc_3_00BD:
        push   ebp
        sub    esp, 0x18
        mov    ebp, esp
        mov    dword ptr [ebp + 4], esi
        mov    dword ptr [ebp + 8], 0x84     ; -> loc_1_0084
        mov    al, byte ptr [ebx + 8]
        mov    byte ptr [ebp + 0x16], al
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [ebp + 0xc], eax
        mov    word ptr [ebp + 0x10], 4
        mov    eax, dword ptr [esi + 0x1c]
        and    eax, 0xffffffe3
        mov    dword ptr [ebp + 0x12], eax
        mov    word ptr [ebp], 5
        push   ebp
        call   sub_1_0050
        add    esp, 0x1c
        pop    ebp
        or     byte ptr [esi + 0x20], 2
        mov    dword ptr [esi + 0x30], 0
        cmp    byte ptr [esi + 0x40], 0xa
        jne    loc_3_0144
        call   sub_3_02AB
        xor    eax, eax
        bt     dword ptr [esi + 0x24], 6
        jae    loc_3_011D
        call   sub_3_027D
loc_3_011D:
        test   byte ptr [esi + 0x17], 2
        je     loc_3_0154
        call   sub_3_0565
        test   byte ptr [esi + 0x1d], 0x20
        je     loc_3_0154
        mov    eax, dword ptr [esi + 0x12d]
        push   dword ptr [esi + 0x30]
        mov    dword ptr [esi + 0x30], eax
        call   sub_3_0565
        pop    dword ptr [esi + 0x30]
        jmp    loc_3_0154
loc_3_0144:
        inc    byte ptr [0x2b0]              ; -> obj1:0x02B0
        call   sub_3_027D
        call   sub_3_05DE
loc_3_0154:
        ret    
loc_3_0155:
        mov    esi, dword ptr [ebx + 0xc]
        test   byte ptr [esi + 0x21], 0x20
        jne    loc_3_0214
        cmp    byte ptr [esi + 0x40], 0xa
        je     loc_3_017C
        test   byte ptr [esi + 0x21], 0x40
        jne    loc_3_0214
        cmp    byte ptr [esi + 0x40], 0
        je     loc_3_0215
loc_3_017C:
        dec    byte ptr [0x2b1]              ; -> obj1:0x02B1
        movzx  eax, byte ptr [esi + 0x15]
        btr    dword ptr [0x29f], eax        ; -> obj3:0x029F
        test   byte ptr [esi + 0x21], 0x40
        je     loc_3_01F9
        movzx  eax, byte ptr [esi + 0x14]
        btr    dword ptr [0x2a3], eax        ; -> obj3:0x02A3
        test   byte ptr [esi + 0x1d], 0x20
        je     loc_3_01C8
        sub    esp, 8
        mov    edx, esp
        mov    word ptr [edx], 0xf
        mov    dword ptr [edx + 4], 1
        push   edx
        call   sub_1_0050
        add    esp, 0xc
        and    byte ptr [0x2a3], 0xfd        ; -> obj3:0x02A3
        jmp    loc_3_01F9
loc_3_01C8:
        cmp    eax, 1
        jne    loc_3_01F9
        test   byte ptr [0x2a3], 1           ; -> obj3:0x02A3
        je     loc_3_01F9
        push   esi
        xor    eax, eax
        call   sub_3_200B
        or     eax, eax
        jne    loc_3_01F8
        or     byte ptr [esi + 0x1d], 0x20
        xor    edx, edx
        inc    dl
        call   sub_3_0A5F
        mov    byte ptr [esi + 0x14], 0
        call   sub_3_1FAD
loc_3_01F8:
        pop    esi
loc_3_01F9:
        test   byte ptr [esi + 0x17], 2
        je     loc_3_0214
        xor    eax, eax
        xchg   dword ptr [esi + 0x30], eax
        or     eax, eax
        je     loc_3_0214
        push   0
        push   eax
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
loc_3_0214:
        ret    
loc_3_0215:
        dec    byte ptr [0x2b0]              ; -> obj1:0x02B0
        jmp    loc_3_01F9
loc_3_021D:
        cmp    byte ptr [0x2b1], 0           ; -> obj1:0x02B1
        jne    loc_3_0235
        cmp    byte ptr [0x2b0], 0           ; -> obj1:0x02B0
        jne    loc_3_0235
        mov    word ptr [ebx + 2], 0xffff
loc_3_0235:
        ret    
sub_3_0236:
        xor    ecx, ecx
        xor    edi, edi
loc_3_023A:
        mov    al, 1
        call   sub_3_0258
        je     loc_3_0257
        cmp    byte ptr [edi + 0xc], 0x10
        jne    loc_3_0254
        movzx  eax, byte ptr [edi + 5]
        bts    dword ptr [0x2a7], eax        ; -> obj3:0x02A7
loc_3_0254:
        inc    ecx
        jmp    loc_3_023A
loc_3_0257:
        ret    
sub_3_0258:
        or     edi, edi
        jne    loc_3_026D
        mov    edi, dword ptr [0x6c]         ; -> loc_1_006C
        mov    edi, dword ptr [edi + 0x2c]
        or     edi, edi
        je     loc_3_027A
loc_3_0269:
        cmp    byte ptr [edi], al
        je     loc_3_027A
loc_3_026D:
        cmp    dword ptr [edi + 1], 0
        je     loc_3_0278
        add    edi, dword ptr [edi + 1]
        jmp    loc_3_0269
loc_3_0278:
        xor    edi, edi
loc_3_027A:
        or     edi, edi
        ret    
sub_3_027D:
        push   eax
        test   dword ptr [esi + 0x1c], 0x40000
        je     loc_3_0292
        xor    ebx, ebx
        mov    dx, 6
        call   sub_3_1EDB
loc_3_0292:
        xor    ebx, ebx
        mov    dx, 0xc
        call   sub_3_1EDB
        pop    eax
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_3_02AB:
        test   byte ptr [esi + 0x23], 0x10
        je     loc_3_0397
        or     byte ptr [esi + 0x23], 2
        inc    byte ptr [0x2b1]              ; -> obj1:0x02B1
        movzx  edx, byte ptr [esi + 0x15]
        or     byte ptr [esi + 0x20], 1
        call   sub_3_0A72
        bts    dword ptr [0x29f], edx        ; -> obj3:0x029F
        call   sub_3_210C
        jb     loc_3_037C
        push   esi
        movzx  eax, byte ptr [ecx + 5]
loc_3_02E3:
        mov    edx, eax
        call   sub_3_200B
        or     eax, eax
        mov    eax, esi
        pop    esi
        jne    loc_3_030F
        cmp    eax, esi
        or     byte ptr [eax + 0xb8], 0x40
        test   byte ptr [eax + 0x22], 0x40
        jne    loc_3_0359
        test   byte ptr [eax + 0x1d], 0x20
        jne    loc_3_035A
loc_3_0306:
        push   esi
        mov    esi, eax
        call   sub_3_2070
        pop    esi
loc_3_030F:
        and    byte ptr [esi + 0x23], 0xfd
        or     byte ptr [esi + 0x21], 0x40
        and    byte ptr [esi + 0x1d], 0xdf
        bts    dword ptr [0x2a3], edx        ; -> obj3:0x02A3
        call   sub_3_0A72
        call   sub_3_1FAD
        cmp    dl, 1
        ja     loc_3_0359
        je     loc_3_0341
        test   byte ptr [0x2a3], 2           ; -> obj3:0x02A3
        jne    loc_3_0359
        jmp    loc_3_0539
loc_3_0341:
        test   byte ptr [0x2a3], 1           ; -> obj3:0x02A3
        je     loc_3_0359
        xor    eax, eax
        call   sub_3_200B
        or     eax, eax
        jne    loc_3_0359
        and    byte ptr [esi + 0x1d], 0xdf
loc_3_0359:
        ret    
loc_3_035A:
        cmp    edx, 1
        jne    loc_3_0306
        sub    esp, 8
        mov    eax, esp
        mov    word ptr [eax], 0xf
        mov    dword ptr [eax + 4], 1
        push   eax
        call   sub_1_0050
        add    esp, 0xc
        jmp    loc_3_030F
loc_3_037C:
        call   sub_3_03B9
        jae    loc_3_0391
        xor    eax, eax
        call   sub_3_202D
        or     ah, ah
        jne    loc_3_0359
        movzx  eax, al
loc_3_0391:
        push   esi
        jmp    loc_3_02E3
loc_3_0397:
        test   byte ptr [esi + 0x24], 0x40
        je     loc_3_037C
        bsf    eax, dword ptr [0x2a7]        ; -> obj3:0x02A7
        je     loc_3_037C
        btr    dword ptr [0x2a7], eax        ; -> obj3:0x02A7
        mov    byte ptr [esi + 0x15], al
        or     byte ptr [esi + 0x23], 0x10
        jmp    sub_3_02AB
sub_3_03B9:
        enter  0x2c, 0
        push   esi
        push   edi
        lea    eax, [ebp - 4]
        push   0
        push   dword ptr [esi + 0x70]
        push   eax
        VxDCall  CONFIGMG._CONFIGMG_Get_Parent   ; cd2003003300
        add    esp, 0xc
        or     eax, eax
        jne    loc_3_04E8
        mov    edi, dword ptr [0x6c]         ; -> loc_1_006C
        mov    edi, dword ptr [edi + 0x4c]
        or     edi, edi
        je     loc_3_04E8
loc_3_03EA:
        mov    dword ptr [ebp - 0x18], edi
        mov    edi, dword ptr [edi]
loc_3_03EF:
        or     edi, edi
        je     loc_3_04CE
        call   sub_3_051F
        mov    dword ptr [ebp - 0x14], edi
        cmp    byte ptr [edi], 8
        jne    loc_3_04C3
        mov    edi, dword ptr [edi + 6]
        or     edi, edi
        je     loc_3_04C3
        call   sub_3_051F
        lea    eax, [ebp - 0xc]
        push   2
        push   dword ptr [ebp - 4]
        push   eax
        VxDCall  CONFIGMG._CONFIGMG_Get_First_Log_Conf ; cd201a003300
        add    esp, 0xc
        or     eax, eax
        jne    loc_3_04C3
        push   edi
        lea    edi, [ebp - 0x2c]
        xor    eax, eax
        cld    
        mov    ecx, 5
        rep stosd dword ptr es:[edi], eax
        lea    edi, [ebp - 0x2c]
        mov    ecx, 5
loc_3_0448:
        lea    eax, [ebp - 8]
        lea    edx, [ebp - 0x10]
        push   ecx
        push   0
        push   edx
        push   2
        push   dword ptr [ebp - 0xc]
        push   eax
        VxDCall  CONFIGMG._CONFIGMG_Get_Next_Res_Des ; cd201f003300
        add    esp, 0x14
        pop    ecx
        or     eax, eax
        jne    loc_3_0483
        mov    ebx, dword ptr [ebp - 8]
        mov    dword ptr [ebp - 0xc], ebx
        dec    ecx
        je     loc_3_0483
        mov    ax, word ptr [ebx + 4]
        mov    word ptr [edi], ax
        mov    ax, word ptr [ebx + 6]
        mov    word ptr [edi + 2], ax
        add    edi, 4
        jmp    loc_3_0448
loc_3_0483:
        pop    edi
        jecxz  loc_3_04B6
loc_3_0486:
        lea    ebx, [ebp - 0x2c]
        call   sub_3_04ED
        jb     loc_3_04B6
        movzx  ecx, byte ptr [edi + 0x20]
        jecxz  loc_3_04B6
        lea    ebx, [edi + 0x21]
loc_3_0499:
        mov    al, byte ptr [ebx + 1]
        cmp    byte ptr [esi + 0x7e], al
        jne    loc_3_04B1
        mov    al, byte ptr [ebx + 2]
        cmp    byte ptr [esi + 0x7c], al
        jne    loc_3_04B1
        mov    al, byte ptr [ebx + 3]
        cmp    byte ptr [esi + 0x7d], al
        je     loc_3_04E2
loc_3_04B1:
        add    ebx, 6
        loop   loc_3_0499
loc_3_04B6:
        mov    edi, dword ptr [edi]
        or     edi, edi
        je     loc_3_04C3
        call   sub_3_051F
        jmp    loc_3_0486
loc_3_04C3:
        mov    edi, dword ptr [ebp - 0x14]
        mov    edi, dword ptr [edi + 1]
        jmp    loc_3_03EF
loc_3_04CE:
        mov    edi, dword ptr [ebp - 0x18]
        mov    edi, dword ptr [edi + 4]
        or     edi, edi
        je     loc_3_04E8
        call   sub_3_051F
        jmp    loc_3_03EA
loc_3_04E2:
        movzx  eax, byte ptr [ebx]
        clc    
        jmp    loc_3_04E9
loc_3_04E8:
        stc    
loc_3_04E9:
        pop    edi
        pop    esi
        leave  
        ret    
sub_3_04ED:
        push   ecx
        push   edi
        mov    ecx, dword ptr [edi + 0x14]
        jecxz  loc_3_051B
        mov    edi, dword ptr [edi + 0x18]
        or     edi, edi
        je     loc_3_051B
        call   sub_3_051F
loc_3_0500:
        movzx  eax, word ptr [ebx]
        or     eax, eax
        je     loc_3_051C
        push   ecx
loc_3_0508:
        cmp    dword ptr [edi], eax
        je     loc_3_0511
        add    edi, 0xc
        loop   loc_3_0508
loc_3_0511:
        or     ecx, ecx
        pop    ecx
        je     loc_3_051B
        add    ebx, 4
        jmp    loc_3_0500
loc_3_051B:
        stc    
loc_3_051C:
        pop    edi
        pop    ecx
        ret    
sub_3_051F:
        push   ebx
        movzx  ebx, di
        rol    edi, 0x10
        movzx  edi, di
        shl    edi, 4
        add    edi, ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        add    edi, dword ptr [ebx + 4]
        pop    ebx
        ret    
loc_3_0539:
        or     byte ptr [esi + 0x1d], 0x20
        push   esi
        mov    al, 1
        movzx  eax, al
        call   sub_3_200B
        or     eax, eax
        jne    loc_3_0551
        call   sub_3_2070
loc_3_0551:
        pop    esi
        xor    edx, edx
        inc    dl
        call   sub_3_0A5F
        mov    byte ptr [esi + 0x14], 0
        call   sub_3_1FAD
        ret    
sub_3_0565:
        push   edi
        push   esi
        mov    ecx, 6
        lea    edi, [esi + 0xd3]
        lea    esi, [esi + 0x54]
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    edi, dword ptr [esp]
        test   byte ptr [edi + 0x24], 0x40
        jne    loc_3_0596
        call   sub_3_1B35
loc_3_0585:
        pop    esi
        pop    edi
        mov    eax, dword ptr [esi + 0x64]
        mov    ecx, dword ptr [esi + 0x30]
        mov    word ptr [ecx + 0x41], ax
        or     byte ptr [esi + 0x21], 0x10
        ret    
loc_3_0596:
        call   sub_3_1C34
        jmp    loc_3_0585
        db  000h, 000h, 000h, 000h
loc_3_05A1:
        dec    ecx
        outsb  dx, byte ptr [esi]
        je     0x5d6
        xor    eax, dword ptr [eax]
        db  000h
sub_3_05A8:
        enter  8, 0
        mov    dword ptr [ebp - 8], edi
        xor    ebx, ebx
        mov    dx, 0xc
        call   sub_3_1EDB
        mov    edi, dword ptr [ebp - 8]
        mov    esi, dword ptr [edi + 4]
        and    word ptr [esi + 0x16], 0xfffe
        call   sub_3_05DE
        mov    edi, dword ptr [ebp - 8]
        mov    word ptr [edi + 0x6a], ax
        test   byte ptr [esi + 0x24], 0x40
        je     loc_3_05DC
        call   sub_3_124D
loc_3_05DC:
        leave  
        ret    
sub_3_05DE:
        sub    esp, 0x14
        mov    dword ptr [esp + 4], 0
        mov    dword ptr [esp], 0
        and    byte ptr [esi + 0x24], 0xef
        mov    edx, dword ptr [esi + 0xdb]
        add    edx, 0x44
        call   sub_3_1A35
        or     ebx, ebx
        je     loc_3_09F0
        mov    dword ptr [esp + 0xc], ebx
        add    ebx, 0x44
        mov    dword ptr [esp + 0x10], ebx
        xor    eax, eax
        mov    dx, 0
        call   sub_3_1EDB
        cmp    ax, 0x10
        jb     loc_3_06D6
        jmp    loc_3_09F4
loc_3_062F:
        test   byte ptr [esi + 0x16], 0x20
        jne    loc_3_0857
        lea    edx, [0x5a0]                  ; -> obj3:0x05A0
        sub    esp, 0x18
        mov    ecx, esp
        mov    word ptr [ecx], 0x11
        mov    dword ptr [ecx + 4], esi
        mov    dword ptr [ecx + 0xc], 3
        mov    dword ptr [ecx + 0x10], 1
        mov    dword ptr [ecx + 0x14], edx
        mov    dword ptr [ecx + 8], 0x5a1    ; -> loc_3_05A1
        push   ecx
        call   sub_1_0050
        movzx  ecx, word ptr [ecx + 2]
        add    esp, 0x1c
        or     ecx, ecx
        jne    loc_3_0857
        cmp    byte ptr [0x5a0], 0           ; -> obj3:0x05A0
        je     loc_3_0857
        cmp    dword ptr [esi + 0xe7], 0
        je     loc_3_0857
        cmp    dword ptr [esi + 0xdf], 0
        je     loc_3_0857
        cmp    dword ptr [esi + 0xe3], 0
        je     loc_3_0857
        mov    ecx, dword ptr [esp]
        mov    al, 0x7f
        or     ecx, ecx
        je     loc_3_06C0
        mov    cl, byte ptr [ecx]
        test   cl, 0x80
        je     loc_3_06C0
        mov    al, cl
loc_3_06C0:
        inc    byte ptr [0x5a7]              ; -> obj3:0x05A7
        add    al, byte ptr [0x5a7]          ; -> obj3:0x05A7
        or     word ptr [esi + 0x16], 0x48
        jmp    loc_3_0787
loc_3_06D6:
        cmp    dword ptr [esi + 0xdb], 0x200
        je     loc_3_071F
        mov    ebx, dword ptr [esp + 0x10]
        cmp    dword ptr [ebx + 0x20], 0x70616441
        jne    loc_3_071F
        cmp    dword ptr [ebx + 0x24], 0x2c636574
        jne    loc_3_071F
        or     word ptr [esi + 0x16], 1
        or     byte ptr [esi + 0x24], 0x10
        mov    eax, dword ptr [0x6c]         ; -> loc_1_006C
        test   byte ptr [eax + 0x2a], 0x10
        je     loc_3_0862
        cmp    dword ptr [esi + 0x10], 0
        jne    loc_3_0862
        jmp    loc_3_09DB
loc_3_071F:
        bts    dword ptr [esi + 0x20], 0x18
        jb     loc_3_0857
        cmp    byte ptr [esi + 0x40], 0xa
        je     loc_3_0857
        mov    ebx, dword ptr [esp + 0x10]
        mov    eax, dword ptr [0x6c]         ; -> loc_1_006C
        mov    eax, dword ptr [eax + 0x38]
loc_3_0740:
        mov    ecx, dword ptr [ebx + 0xdc]
        cmp    byte ptr [eax], 0xff
        je     loc_3_062F
        mov    dword ptr [esp], eax
        cmp    dword ptr [eax + 1], ecx
        jne    loc_3_077C
        test   word ptr [eax + 9], 2
        jne    loc_3_077C
        test   word ptr [eax + 9], 1
        je     loc_3_0781
        xor    edx, edx
        mov    ecx, 0x200
loc_3_076E:
        add    edx, dword ptr [ecx + ebx - 4]
        sub    ecx, 4
        jne    loc_3_076E
        cmp    dword ptr [eax + 5], edx
        je     loc_3_0781
loc_3_077C:
        add    eax, 0xb
        jmp    loc_3_0740
loc_3_0781:
        or     byte ptr [eax + 9], 2
        mov    al, byte ptr [eax]
loc_3_0787:
        mov    byte ptr [esi + 0x15], al
        sub    esp, 0xc
        mov    ecx, esp
        mov    byte ptr [ecx + 8], al
        mov    byte ptr [ecx + 9], 0
        mov    dword ptr [ecx + 4], esi
        mov    word ptr [ecx], 6
        push   ecx
        call   sub_1_0050
        add    esp, 0x10
        or     dword ptr [esi + 0x20], 1
        test   word ptr [esi + 0x16], 8
        jne    loc_3_0857
        mov    edi, dword ptr [0x6c]         ; -> loc_1_006C
        mov    edi, dword ptr [edi + 0x2c]
        or     edi, edi
        je     loc_3_0825
        mov    al, byte ptr [esi + 0x15]
loc_3_07C8:
        cmp    byte ptr [edi], 0
        jne    loc_3_07D2
        cmp    al, byte ptr [edi + 5]
        je     loc_3_07DD
loc_3_07D2:
        cmp    dword ptr [edi + 1], 0
        je     loc_3_0825
        add    edi, dword ptr [edi + 1]
        jmp    loc_3_07C8
loc_3_07DD:
        test   word ptr [esi + 0x16], 0x8000
        je     loc_3_07E9
        cmp    al, 0x80
        jne    loc_3_0825
loc_3_07E9:
        movzx  eax, word ptr [edi + 6]
        mov    dword ptr [esi + 0xe3], eax
        movzx  eax, word ptr [edi + 8]
        mov    dword ptr [esi + 0xdf], eax
        movzx  eax, word ptr [edi + 0xa]
        mov    dword ptr [esi + 0xe7], eax
        mul    dword ptr [esi + 0xdf]
        mul    dword ptr [esi + 0xe3]
        mov    dword ptr [esi + 0xd3], eax
        mov    dword ptr [esi + 0xd7], edx
        or     word ptr [esi + 0x16], 0x1000
loc_3_0825:
        test   word ptr [esi + 0x16], 0x4000
        jne    loc_3_0857
        mov    eax, dword ptr [esi + 0xe3]
        mov    dword ptr [esi + 0x64], eax
        mov    eax, dword ptr [esi + 0xdf]
        mov    dword ptr [esi + 0x60], eax
        mov    eax, dword ptr [esi + 0xe7]
        mov    dword ptr [esi + 0x68], eax
        mov    eax, dword ptr [esi + 0xd3]
        mov    dword ptr [esi + 0x54], eax
        or     word ptr [esi + 0x16], 0x2000
loc_3_0857:
        cmp    word ptr [ebx + 0x1fe], 0xaa55
        je     loc_3_0880
loc_3_0862:
        or     word ptr [esi + 0x16], 1
        cmp    byte ptr [esi + 0x40], 0xa
        je     loc_3_09DB
        xor    ebx, ebx
        mov    edi, dword ptr [esi + 0x10]
        call   sub_3_0BAA
        jmp    loc_3_09DB
loc_3_0880:
        cmp    byte ptr [esi + 0x40], 0xa
        je     loc_3_08D3
        mov    eax, esi
        mov    edi, dword ptr [esp + 0xc]
        mov    esi, ebx
        add    esi, 0x1be
        mov    ecx, 0x42
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    esi, eax
        add    ebx, 0x1be
        mov    al, 2
        call   sub_3_1208
        or     eax, eax
        je     loc_3_08B0
        jmp    loc_3_0904
loc_3_08B0:
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [esp + 8], eax
        mov    ebx, dword ptr [esp + 0x10]
        mov    dx, 0
        call   sub_3_1EDB
        cmp    ax, 0x10
        jb     loc_3_08CF
        jmp    loc_3_09DB
loc_3_08CF:
        mov    ebx, dword ptr [esp + 0x10]
loc_3_08D3:
        test   byte ptr [esi + 0x21], 0x40
        je     loc_3_0904
        test   byte ptr [esi + 0x23], 2
        jne    loc_3_0904
        push   esi
        push   edi
        mov    edi, dword ptr [esi + 0x30]
        lea    esi, [ebx + 0xb]
        lea    edi, [edi + 6]
        mov    ecx, 0x19
        cmp    word ptr [esi + 0xb], 0
        jne    loc_3_08FB
        mov    ecx, 0x35
loc_3_08FB:
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    edi
        pop    esi
        jmp    loc_3_09DB
loc_3_0904:
        cmp    byte ptr [esi + 0x40], 0xa
        je     loc_3_09DB
        mov    ebx, dword ptr [esp + 0xc]
        mov    al, 0
        call   sub_3_1208
        mov    edi, dword ptr [esi + 0x10]
        or     eax, eax
        jne    loc_3_093D
        xor    eax, eax
        call   sub_3_0BAA
        or     byte ptr [esp + 4], 1
        or     edi, edi
        je     loc_3_0985
        mov    edi, dword ptr [edi + 0x10]
        or     edi, edi
        je     loc_3_09DB
        jmp    loc_3_0985
loc_3_093D:
        mov    ebx, dword ptr [esp + 0xc]
        mov    ecx, 4
loc_3_0946:
        mov    al, 1
        call   sub_3_120D
        or     eax, eax
        je     loc_3_095B
        bts    dword ptr [esp + 4], 0
        jae    loc_3_0985
        jmp    loc_3_09A0
loc_3_095B:
        push   ecx
        push   ebx
        xor    eax, eax
        call   sub_3_0BAA
        pop    ebx
        pop    ecx
        or     edi, edi
        je     loc_3_0971
        mov    edi, dword ptr [edi + 0x10]
        or     edi, edi
        je     loc_3_09DB
loc_3_0971:
        mov    byte ptr [ebx + 4], 0xff
        bts    dword ptr [esp + 4], 0
        jae    loc_3_0985
        add    ebx, 0x10
        dec    ecx
        je     loc_3_09A0
        jmp    loc_3_0946
loc_3_0985:
        mov    ebx, dword ptr [esp + 0xc]
        call   sub_3_11E9
        or     eax, eax
        jne    loc_3_093D
        call   sub_3_0AA0
        mov    edi, eax
        cmp    eax, -1
        jne    loc_3_093D
        xor    edi, edi
loc_3_09A0:
        or     edi, edi
        je     loc_3_09B6
        or     word ptr [edi + 0x16], 0x81
        mov    dword ptr [edi + 0x28], 0
        mov    edi, dword ptr [edi + 0x10]
        jmp    loc_3_09A0
loc_3_09B6:
        test   byte ptr [esi + 0x20], 4
        je     loc_3_09DB
        mov    edi, dword ptr [esi + 0x10]
        or     edi, edi
        je     loc_3_09CF
        test   byte ptr [edi + 0x16], 1
        je     loc_3_09DB
        cmp    dword ptr [edi + 0x28], 0
        jne    loc_3_09DB
loc_3_09CF:
        mov    ebx, 0xffffffff
        xor    eax, eax
        call   sub_3_0BAA
loc_3_09DB:
        call   sub_3_0A1A
        call   sub_3_10DA
        xor    eax, eax
loc_3_09E7:
        mov    edi, dword ptr [esp + 0xc]
        call   sub_3_1A61
loc_3_09F0:
        add    esp, 0x14
        ret    
loc_3_09F4:
        or     word ptr [esi + 0x16], 0x21
        cmp    byte ptr [esi + 0x40], 0xa
        je     loc_3_0A0E
        xor    ebx, ebx
        mov    edi, dword ptr [esi + 0x10]
        call   sub_3_0BAA
        call   sub_3_0A1A
loc_3_0A0E:
        cmp    ax, 0x1c
        je     loc_3_09E7
        mov    ax, 0x18
        jmp    loc_3_09E7
sub_3_0A1A:
        test   byte ptr [esi + 0x20], 4
        je     loc_3_0A5E
        bts    word ptr [esi + 0x16], 1
        jb     loc_3_0A5E
        cmp    byte ptr [esi + 0x40], 0xa
        je     loc_3_0A5E
loc_3_0A2E:
        mov    al, 1
        call   sub_3_202D
        or     ah, ah
        jne    loc_3_0A5E
        push   eax
        call   sub_3_2088
        pop    eax
        mov    word ptr [edi + 0x16], 0x81
        call   sub_3_2051
        lea    eax, [esi + 0x10]
loc_3_0A4E:
        cmp    dword ptr [eax], 0
        je     loc_3_0A5A
        mov    eax, dword ptr [eax]
        lea    eax, [eax + 0x10]
        jmp    loc_3_0A4E
loc_3_0A5A:
        mov    dword ptr [eax], edi
        jmp    loc_3_0A2E
loc_3_0A5E:
        ret    
sub_3_0A5F:
        test   dword ptr [esi + 0x20], 0x4000
        jne    sub_3_0A72
        or     dword ptr [esi + 0x20], 0x4000
        mov    byte ptr [esi + 0x15], dl
sub_3_0A72:
        sub    esp, 0xc
        mov    ebx, esp
        mov    byte ptr [ebx + 8], dl
        mov    dword ptr [ebx + 4], esi
        mov    word ptr [ebx], 6
        mov    byte ptr [ebx + 9], 0
        test   byte ptr [esi + 0x1d], 0x20
        je     loc_3_0A95
        cmp    dl, 1
        jne    loc_3_0A95
        or     byte ptr [ebx + 9], 1
loc_3_0A95:
        push   ebx
        call   sub_1_0050
        add    esp, 0x10
        ret    
sub_3_0AA0:
        sub    esp, 0x24
        mov    dword ptr [esp + 0x18], edi
        mov    eax, dword ptr [ebx + 8]
        mov    dword ptr [esp + 0x14], eax
        mov    dword ptr [esp + 0x10], 0
        mov    ecx, ebx
        mov    edx, dword ptr [esi + 0xdb]
        call   sub_3_1A35
        or     ebx, ebx
        je     loc_3_0BA2
        mov    dword ptr [esp + 0x1c], ebx
loc_3_0ACF:
        mov    edi, esi
        mov    ebx, dword ptr [esp + 0x1c]
        mov    eax, dword ptr [esp + 0x14]
        add    eax, dword ptr [esp + 0x10]
        mov    dx, 0
        call   sub_3_1EDB
        cmp    ax, 0x10
        jae    loc_3_0AF7
        cmp    word ptr [ebx + 0x1fe], 0xaa55
        je     loc_3_0B21
loc_3_0AF7:
        mov    ebx, ecx
        mov    ch, byte ptr [ebx + 1]
        mov    cl, byte ptr [ebx + 2]
        mov    al, cl
        and    cl, 0x3f
        and    ax, 0xc0
        shl    ax, 2
        mov    al, byte ptr [ebx + 3]
        call   sub_3_1A7B
        mov    ebx, dword ptr [esp + 0x1c]
        mov    dx, 0
        call   sub_3_1EDB
loc_3_0B21:
        cmp    ax, 0x10
        jae    loc_3_0B97
        add    ebx, 0x1be
        push   ebx
        call   sub_3_11E9
        mov    dword ptr [esp + 0x24], eax
        push   esi
        push   edi
        mov    esi, ebx
        lea    edi, [esp + 0xc]
        mov    ecx, 0x10
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    edi
        pop    esi
        pop    ebx
        mov    al, 2
        call   sub_3_1208
        or     eax, eax
        jne    loc_3_0B81
        mov    edi, dword ptr [esp + 0x18]
        or     edi, edi
        je     loc_3_0B74
        cmp    edi, -1
        je     loc_3_0B97
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esp + 0x18], eax
        or     eax, eax
        jne    loc_3_0B74
        mov    dword ptr [esp + 0x18], 0xffffffff
loc_3_0B74:
        mov    eax, dword ptr [esp + 0x14]
        add    eax, dword ptr [esp + 0x10]
        call   sub_3_0BAA
loc_3_0B81:
        cmp    dword ptr [esp + 0x20], 0
        jne    loc_3_0B97
        lea    ecx, [esp]
        mov    eax, dword ptr [ecx + 8]
        mov    dword ptr [esp + 0x10], eax
        jmp    loc_3_0ACF
loc_3_0B97:
        push   edi
        mov    edi, dword ptr [esp + 0x20]
        call   sub_3_1A61
        pop    edi
loc_3_0BA2:
        mov    eax, dword ptr [esp + 0x18]
        add    esp, 0x24
        ret    
sub_3_0BAA:
        sub    esp, 0x1c
        push   edi
        mov    dword ptr [esp + 0x18], esi
        mov    dword ptr [esp + 0x14], ebx
        mov    dword ptr [esp + 0xc], eax
        mov    byte ptr [esp + 8], 0xff
        mov    byte ptr [esp + 4], 0
        mov    dword ptr [esp + 0x1c], 0
        cmp    dword ptr [esp + 0x14], 0
        je     loc_3_0D1E
        push   ebx
        mov    edx, dword ptr [esi + 0xdb]
        call   sub_3_1A35
        mov    dword ptr [esp + 0x20], ebx
        or     ebx, ebx
        pop    ebx
        je     loc_3_103C
        mov    ebx, dword ptr [esp + 0x1c]
        mov    eax, dword ptr [esp + 0x14]
        cmp    eax, -1
        jne    loc_3_0C01
        xor    eax, eax
        jmp    loc_3_0C08
loc_3_0C01:
        mov    eax, dword ptr [eax + 8]
        add    eax, dword ptr [esp + 0xc]
loc_3_0C08:
        mov    dword ptr [esp + 0x10], eax
        mov    dx, 0
        call   sub_3_1EDB
        cmp    ax, 0x10
        jae    loc_3_0C24
        call   sub_3_19DC
        or     eax, eax
        je     loc_3_0C6B
loc_3_0C24:
        mov    ebx, dword ptr [esp + 0x14]
        cmp    ebx, -1
        je     loc_3_0D1E
        mov    ch, byte ptr [ebx + 1]
        mov    cl, byte ptr [ebx + 2]
        mov    al, cl
        and    cl, 0x3f
        and    ax, 0xc0
        shl    ax, 2
        mov    al, byte ptr [ebx + 3]
        xchg   esi, edi
        call   sub_3_1A7B
        xchg   esi, edi
        mov    dword ptr [esp + 0x10], eax
        mov    ebx, dword ptr [esp + 0x1c]
        mov    dx, 0
        call   sub_3_1EDB
        cmp    ax, 0x10
        jae    loc_3_0D1E
loc_3_0C6B:
        or     edi, edi
        jne    loc_3_0EC4
        mov    ebx, dword ptr [esp + 0x1c]
        call   sub_3_19DC
        or     eax, eax
        jne    loc_3_0D1E
        mov    edi, dword ptr [esp + 0x10]
        cmp    word ptr [ebx + 0x16], 0
        jne    loc_3_0C9C
        cmp    byte ptr [ebx + 0x42], 0x29
        jne    loc_3_0D08
        mov    eax, dword ptr [ebx + 0x43]
        or     eax, eax
        jne    loc_3_0CA9
loc_3_0C9C:
        cmp    byte ptr [ebx + 0x26], 0x29
        jne    loc_3_0D08
        mov    eax, dword ptr [ebx + 0x27]
        or     eax, eax
        je     loc_3_0D08
loc_3_0CA9:
        mov    ebx, dword ptr [0x6c]         ; -> loc_1_006C
        mov    ebx, dword ptr [ebx + 0x34]
loc_3_0CB2:
        cmp    byte ptr [ebx], 0xff
        je     loc_3_0E4C
        cmp    dword ptr [ebx + 2], eax
        jne    loc_3_0D03
        cmp    byte ptr [ebx + 1], 0xff
        je     loc_3_0CD3
        mov    cl, byte ptr [esi + 0x15]
        cmp    byte ptr [ebx + 1], cl
        jne    loc_3_0D03
        cmp    edi, dword ptr [ebx + 6]
        jne    loc_3_0D03
loc_3_0CD3:
        mov    cx, word ptr [esi + 0x7c]
        cmp    byte ptr [ebx + 0xa], 0xff
        je     loc_3_0CE7
        cmp    byte ptr [ebx + 0xa], cl
        jne    loc_3_0D03
        cmp    byte ptr [ebx + 0xb], ch
        jne    loc_3_0D03
loc_3_0CE7:
        cmp    byte ptr [ebx + 0xd], 0xff
        je     loc_3_0CF7
        cmp    byte ptr [ebx + 0xd], cl
        jne    loc_3_0D03
        cmp    byte ptr [ebx + 0xe], ch
        jne    loc_3_0D03
loc_3_0CF7:
        bts    word ptr [ebx + 0x10], 0
        jae    loc_3_0E20
loc_3_0D03:
        add    ebx, 0x12
        jmp    loc_3_0CB2
loc_3_0D08:
        xor    eax, eax
        mov    edx, dword ptr [esp + 0x1c]
        mov    ecx, 0x200
loc_3_0D13:
        add    eax, dword ptr [ecx + edx - 4]
        sub    ecx, 4
        jne    loc_3_0D13
        jmp    loc_3_0CA9
loc_3_0D1E:
        or     edi, edi
        jne    loc_3_0EC4
        mov    esi, dword ptr [esp + 0x18]
        mov    al, 3
        call   sub_3_202D
        or     ah, ah
        jne    loc_3_0D3D
        movzx  eax, al
        jmp    loc_3_0E23
loc_3_0D3D:
        mov    ebx, dword ptr [0x6c]         ; -> loc_1_006C
        mov    ebx, dword ptr [ebx + 0x34]
loc_3_0D46:
        cmp    byte ptr [ebx], 0xff
        je     loc_3_0DF0
        test   byte ptr [ebx + 0x10], 2
        je     loc_3_0DE8
        cmp    byte ptr [esi + 0x74], 1
        jne    loc_3_0D6B
        cmp    byte ptr [ebx + 0xd], 0xff
        jne    loc_3_0D70
        cmp    byte ptr [ebx + 0xa], 0xff
        jne    loc_3_0D70
loc_3_0D6B:
        or     byte ptr [esp + 4], 2
loc_3_0D70:
        test   byte ptr [esi + 0x20], 1
        je     loc_3_0D9A
        cmp    byte ptr [ebx + 1], 0xff
        je     loc_3_0D9A
        mov    cl, byte ptr [esi + 0x15]
        cmp    byte ptr [ebx + 1], cl
        jne    loc_3_0DE8
loc_3_0D84:
        movzx  eax, byte ptr [ebx]
        bts    word ptr [ebx + 0x10], 0
        setb   ah
        or     ah, ah
        je     loc_3_0E23
        jmp    loc_3_0DE8
loc_3_0D9A:
        cmp    byte ptr [esi + 0x74], 1
        jne    loc_3_0DE8
        mov    cx, word ptr [esi + 0x7c]
        cmp    byte ptr [ebx + 0xd], 0xff
        je     loc_3_0DC6
        cmp    byte ptr [ebx + 0xd], cl
        jne    loc_3_0DE8
        cmp    byte ptr [ebx + 0xe], ch
        jne    loc_3_0DE8
        mov    al, 3
        call   sub_3_1045
        jb     loc_3_0DE8
        mov    cl, byte ptr [ebx + 0xf]
        cmp    byte ptr [eax], cl
        jne    loc_3_0DE8
        jmp    loc_3_0D84
loc_3_0DC6:
        cmp    byte ptr [ebx + 0xa], 0xff
        je     loc_3_0DE8
        cmp    byte ptr [ebx + 0xa], cl
        jne    loc_3_0DE8
        cmp    byte ptr [ebx + 0xb], ch
        jne    loc_3_0DE8
        mov    al, 1
        call   sub_3_1045
        jb     loc_3_0DE8
        mov    cl, byte ptr [ebx + 0xc]
        cmp    byte ptr [eax], cl
        jne    loc_3_0DE8
        jmp    loc_3_0D84
loc_3_0DE8:
        add    ebx, 0x12
        jmp    loc_3_0D46
loc_3_0DF0:
        test   byte ptr [esp + 4], 2
        jne    loc_3_102F
        cmp    dword ptr [esp + 0x14], 0
        jne    loc_3_0E0C
        test   byte ptr [esi + 0x20], 4
        je     loc_3_102F
loc_3_0E0C:
        xor    al, al
        call   sub_3_202D
        or     ah, ah
        jne    loc_3_102F
        movzx  eax, al
        jmp    loc_3_0E23
loc_3_0E20:
        movzx  eax, byte ptr [ebx]
loc_3_0E23:
        mov    byte ptr [esp + 8], al
        mov    esi, dword ptr [0x6c]         ; -> loc_1_006C
        call   sub_3_200B
        xchg   esi, edi
        or     eax, eax
        jne    loc_3_0E4C
        test   dword ptr [edi + 0x20], 0x400000
        jne    loc_3_102F
        mov    esi, edi
        call   sub_3_2070
loc_3_0E4C:
        mov    esi, dword ptr [esp + 0x18]
        call   sub_3_2088
        mov    al, byte ptr [esp + 8]
        cmp    al, 0xff
        jne    loc_3_0E89
        bts    word ptr [esi + 0x16], 2
        jb     loc_3_0E76
        mov    al, 1
        call   sub_3_202D
        or     ah, ah
        jne    loc_3_0E76
        or     byte ptr [esi + 0x16], 0x10
        jmp    loc_3_0E89
loc_3_0E76:
        xor    al, al
        test   byte ptr [esi + 0x16], 0x10
        je     loc_3_0E80
        mov    al, 1
loc_3_0E80:
        call   sub_3_202D
        or     ah, ah
        jne    loc_3_0E94
loc_3_0E89:
        call   sub_3_2051
        inc    byte ptr [0x2b2]              ; -> obj1:0x02B2
loc_3_0E94:
        mov    esi, dword ptr [esi + 8]
        call   sub_3_20C9
        mov    dword ptr [edi + 8], edx
        mov    esi, dword ptr [esp + 0x18]
        mov    dword ptr [edi], esi
        lea    eax, [esi + 0x10]
loc_3_0EA8:
        cmp    dword ptr [eax], 0
        je     loc_3_0EB4
        mov    eax, dword ptr [eax]
        lea    eax, [eax + 0x10]
        jmp    loc_3_0EA8
loc_3_0EB4:
        mov    dword ptr [eax], edi
        mov    dword ptr [edi + 0x10], 0
        mov    esi, edi
        call   sub_3_1FAD
loc_3_0EC4:
        cmp    dword ptr [esp + 0x14], 0
        je     loc_3_0FE4
        mov    ecx, dword ptr [edi + 0x30]
        mov    ebx, dword ptr [esp + 0x14]
        cmp    ebx, -1
        je     loc_3_0F2F
        mov    al, byte ptr [ebx + 4]
        mov    byte ptr [edi + 0x3d], al
        mov    eax, dword ptr [ebx + 8]
        test   byte ptr [edi + 0x20], 4
        jne    loc_3_0EF0
        test   byte ptr [edi + 0x17], 2
        je     loc_3_0EF6
loc_3_0EF0:
        mov    dword ptr [ecx + 0x17], eax
        mov    dword ptr [ecx + 0x54], eax
loc_3_0EF6:
        mov    word ptr [ecx + 0xe], 0
        mov    word ptr [ecx + 0x4b], 0
        mov    dword ptr [ecx + 0x1b], 0
        mov    dword ptr [ecx + 0x58], 0
        mov    ax, word ptr [ebx + 0xe]
        or     ax, ax
        je     loc_3_0F24
        mov    eax, dword ptr [ebx + 0xc]
        mov    dword ptr [ecx + 0x1b], eax
        mov    dword ptr [ecx + 0x58], eax
        jmp    loc_3_0F2F
loc_3_0F24:
        mov    eax, dword ptr [ebx + 0xc]
        mov    word ptr [ecx + 0xe], ax
        mov    word ptr [ecx + 0x4b], ax
loc_3_0F2F:
        mov    eax, dword ptr [esp + 0x10]
        mov    dword ptr [edi + 0x28], eax
        and    word ptr [edi + 0x16], 0xfffe
        mov    ebx, dword ptr [esp + 0x1c]
        call   sub_3_19DC
        or     eax, eax
        jne    loc_3_0FE4
        and    word ptr [edi + 0x16], 0xff7f
        test   byte ptr [edi + 0x20], 4
        jne    loc_3_0F62
        test   byte ptr [edi + 0x17], 2
        je     loc_3_102F
loc_3_0F62:
        push   edi
        lea    esi, [ebx + 0xb]
        mov    edi, dword ptr [edi + 0x30]
        lea    edi, [edi + 6]
        mov    ecx, 0x19
        cmp    word ptr [esi + 0xb], 0
        jne    loc_3_0F7D
        mov    ecx, 0x35
loc_3_0F7D:
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    edi
        mov    esi, dword ptr [esp + 0x18]
        push   edi
        mov    edi, dword ptr [edi + 0x30]
        mov    ecx, dword ptr [esi + 0xe7]
        or     ecx, ecx
        je     loc_3_0FDB
        mov    word ptr [edi + 0x13], cx
        mov    ecx, dword ptr [esi + 0xdf]
        mov    word ptr [edi + 0x15], cx
        or     ecx, ecx
        je     loc_3_0FDB
        xor    edx, edx
        mov    ax, word ptr [edi + 0xe]
        or     ax, ax
        jne    loc_3_0FB7
        mov    dx, word ptr [edi + 0x1d]
        mov    ax, word ptr [edi + 0x1b]
loc_3_0FB7:
        push   edx
        push   eax
        mov    ax, word ptr [edi + 0x15]
        mul    word ptr [edi + 0x13]
        mov    ecx, eax
        pop    eax
        pop    edx
        push   eax
        mov    eax, edx
        xor    edx, edx
        div    cx
        pop    eax
        div    cx
        or     dx, dx
        je     loc_3_0FD7
        inc    eax
loc_3_0FD7:
        mov    word ptr [edi + 0x41], ax
loc_3_0FDB:
        pop    edi
        test   byte ptr [edi + 0x17], 2
        jne    loc_3_102A
        jmp    loc_3_102F
loc_3_0FE4:
        or     edi, edi
        je     loc_3_102F
        mov    ax, word ptr [edi + 0x16]
        or     ax, 1
        and    ax, 0x201
        mov    word ptr [edi + 0x16], ax
        cmp    dword ptr [esp + 0x14], 0
        jne    loc_3_102A
        mov    dword ptr [edi + 0x28], 0
        mov    eax, dword ptr [edi + 0x30]
        mov    word ptr [eax + 0xe], 0
        mov    dword ptr [eax + 0x1b], 0
        mov    word ptr [eax + 0x4b], 0
        mov    dword ptr [eax + 0x58], 0
        mov    dword ptr [eax + 0x54], 0
loc_3_102A:
        call   sub_3_1C34
loc_3_102F:
        mov    edi, dword ptr [esp + 0x1c]
        or     edi, edi
        je     loc_3_103C
        call   sub_3_1A61
loc_3_103C:
        mov    esi, dword ptr [esp + 0x18]
        pop    edi
        add    esp, 0x1c
        ret    
sub_3_1045:
        sub    esp, 4
        mov    byte ptr [esp], al
        pushal 
        mov    ecx, dword ptr [0x6c]         ; -> loc_1_006C
        mov    edi, dword ptr [ecx + 0x50]
loc_3_1055:
        or     edi, edi
        je     loc_3_10A8
        mov    al, byte ptr [esp + 0x20]
        cmp    byte ptr [edi], al
        jne    loc_3_10A3
        mov    edx, dword ptr [edi + 5]
loc_3_1064:
        or     edx, edx
        je     loc_3_10A3
        cmp    byte ptr [edx + 4], 0
        jne    loc_3_109E
        mov    cx, word ptr [esi + 0x7c]
        cmp    word ptr [edx + 1], cx
        jne    loc_3_109E
        lea    ebx, [esi + 0x82]
        movzx  eax, byte ptr [ebx]
        mov    ecx, 0x23
loc_3_1086:
        add    al, byte ptr [ecx + ebx]
        adc    ah, 0
        loop   loc_3_1086
        cmp    word ptr [edx + 0xc], ax
        jne    loc_3_10A3
        clc    
        mov    dword ptr [esp + 0x1c], edx
loc_3_1099:
        popal  
        add    esp, 4
        ret    
loc_3_109E:
        mov    edx, dword ptr [edx + 0x10]
        jmp    loc_3_1064
loc_3_10A3:
        mov    edi, dword ptr [edi + 1]
        jmp    loc_3_1055
loc_3_10A8:
        stc    
        jmp    loc_3_1099
sub_3_10AB:
        movzx  ecx, byte ptr [edi + 0x15]
        push   ecx
        movzx  ecx, byte ptr [edi + 0x14]
        push   ecx
        push   dword ptr [edi + 0xc]
        push   edi
        push   esi
        mov    ecx, 0x50
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    esi
        pop    edi
        pop    dword ptr [edi + 0xc]
        pop    ecx
        mov    byte ptr [edi + 0x14], cl
        pop    ecx
        mov    byte ptr [edi + 0x15], cl
        xor    dword ptr [edi + 0x20], 0xc000
        and    byte ptr [edi + 0x21], 0xdf
        ret    
sub_3_10DA:
        sub    esp, 0xc
        push   ebx
        push   edi
        push   esi
        mov    word ptr [esp + 0xc], 0xb
        mov    dword ptr [esp + 0x10], esi
        lea    eax, [esp + 0xc]
        push   eax
        call   sub_1_0050
        add    esp, 4
        cmp    word ptr [esp + 0xe], 0
        jne    loc_3_11E2
loc_3_1105:
        bsf    eax, dword ptr [esp + 0x14]
        je     loc_3_11E2
        btr    dword ptr [esp + 0x14], eax
        mov    ebx, dword ptr [0x6c]         ; -> loc_1_006C
        mov    ebx, dword ptr [ebx + 0x34]
loc_3_111E:
        cmp    byte ptr [ebx], 0xff
        je     loc_3_1105
        cmp    byte ptr [ebx], al
        je     loc_3_112C
        add    ebx, 0x12
        jmp    loc_3_111E
loc_3_112C:
        test   word ptr [ebx + 0x10], 2
        bts    word ptr [ebx + 0x10], 0
        jb     loc_3_1105
        mov    ecx, dword ptr [0x6c]         ; -> loc_1_006C
        bts    dword ptr [ecx + 0x44], eax
        call   sub_3_200B
        or     eax, eax
        jne    loc_3_1105
        mov    edi, esi
        inc    byte ptr [0x2b2]              ; -> obj1:0x02B2
        test   byte ptr [edi + 0x22], 0x40
        jne    loc_3_1105
        mov    esi, dword ptr [0x6c]         ; -> loc_1_006C
        mov    esi, dword ptr [esi + 0x3c]
        mov    eax, dword ptr [esi + 0x10]
loc_3_1167:
        or     eax, eax
        je     loc_3_1105
        cmp    eax, edi
        je     loc_3_1176
        mov    esi, eax
        mov    eax, dword ptr [esi + 0x10]
        jmp    loc_3_1167
loc_3_1176:
        mov    eax, dword ptr [edi + 0x10]
        mov    dword ptr [esi + 0x10], eax
        mov    esi, dword ptr [edi + 8]
loc_3_117F:
        or     esi, esi
        je     loc_3_119A
        cmp    byte ptr [esi + 0x13], 0x1f
        je     loc_3_1195
        push   0
        push   esi
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
loc_3_1195:
        mov    esi, dword ptr [esi + 0xc]
        jmp    loc_3_117F
loc_3_119A:
        mov    esi, dword ptr [esp + 0x10]
        call   sub_3_10AB
        mov    esi, dword ptr [esi + 8]
        call   sub_3_20C9
        mov    dword ptr [edi + 8], edx
        mov    esi, dword ptr [esp + 0x10]
        mov    dword ptr [edi], esi
        lea    eax, [esi + 0x10]
loc_3_11B7:
        mov    ecx, dword ptr [eax]
        jecxz  loc_3_11C0
        lea    eax, [ecx + 0x10]
        jmp    loc_3_11B7
loc_3_11C0:
        mov    dword ptr [eax], edi
        mov    dword ptr [edi + 0x10], ecx
        mov    esi, edi
        call   sub_3_1FAD
        test   byte ptr [edi + 0x17], 2
        je     loc_3_11D7
        call   sub_3_1C34
loc_3_11D7:
        and    word ptr [edi + 0x16], 0xff7e
        jmp    loc_3_1105
loc_3_11E2:
        pop    esi
        pop    edi
        pop    ebx
        add    esp, 0xc
        ret    
sub_3_11E9:
        mov    ecx, 4
loc_3_11EE:
        cmp    byte ptr [ebx + 4], 5
        je     loc_3_11FA
        cmp    byte ptr [ebx + 4], 0xf
        jne    loc_3_11FD
loc_3_11FA:
        xor    eax, eax
        ret    
loc_3_11FD:
        add    ebx, 0x10
        loop   loc_3_11EE
        mov    eax, 1
        ret    
sub_3_1208:
        mov    ecx, 4
sub_3_120D:
        cmp    al, 2
        je     loc_3_1221
        cmp    al, 0
        jne    loc_3_121C
        cmp    byte ptr [ebx], 0
        je     loc_3_1245
        jmp    loc_3_1221
loc_3_121C:
        cmp    byte ptr [ebx], 0
        jne    loc_3_1245
loc_3_1221:
        mov    dl, byte ptr [ebx + 4]
        cmp    dl, 1
        je     loc_3_1242
        cmp    dl, 4
        je     loc_3_1242
        cmp    dl, 6
        je     loc_3_1242
        cmp    dl, 0xb
        je     loc_3_1242
        cmp    dl, 0xc
        je     loc_3_1242
        cmp    dl, 0xe
        jne    loc_3_1245
loc_3_1242:
        xor    eax, eax
        ret    
loc_3_1245:
        add    ebx, 0x10
        loop   sub_3_120D
        mov    al, 1
        ret    
sub_3_124D:
        test   byte ptr [esi + 0x16], 1
        je     loc_3_127F
        mov    eax, dword ptr [esi + 0x5c]
        mov    dword ptr [esi + 0xdb], eax
        bsf    eax, eax
        mov    byte ptr [esi + 0x3c], al
        mov    eax, dword ptr [esi + 0x60]
        mov    dword ptr [esi + 0xdf], eax
        mov    eax, dword ptr [esi + 0x64]
        mov    dword ptr [esi + 0xe3], eax
        mov    eax, dword ptr [esi + 0x68]
        mov    dword ptr [esi + 0xe7], eax
        jmp    loc_3_12C6
loc_3_127F:
        mov    ecx, dword ptr [esi + 0x30]
        jecxz  loc_3_12C6
        lea    ecx, [ecx + 6]
        movzx  eax, word ptr [ecx]
        mov    dword ptr [esi + 0xdb], eax
        bsf    eax, eax
        mov    byte ptr [esi + 0x3c], al
        movzx  eax, word ptr [ecx + 0xf]
        mov    dword ptr [esi + 0xdf], eax
        movzx  eax, word ptr [ecx + 0xd]
        mov    dword ptr [esi + 0xe7], eax
        mul    dword ptr [esi + 0xdf]
        mov    ecx, eax
        mov    eax, dword ptr [esi + 0xd3]
        mov    edx, dword ptr [esi + 0xd7]
        div    ecx
        mov    dword ptr [esi + 0xe3], eax
loc_3_12C6:
        and    byte ptr [esi + 0x1e], 0xdf
        cmp    dword ptr [esi + 0xd7], 0
        jne    loc_3_12E3
        cmp    dword ptr [esi + 0xd3], 0x1680
        ja     loc_3_12E3
        or     byte ptr [esi + 0x1e], 0x20
loc_3_12E3:
        and    byte ptr [esi + 0x1e], 0xfe
        ret    
sub_3_12E8:
        mov    dword ptr [eax], ebx
        add    byte ptr [eax], al
        mov    dword ptr [eax], ebx
        add    byte ptr [eax], al
        mov    dword ptr [eax], ebx
        add    byte ptr [eax], al
        mov    dword ptr [eax], ebx
        add    byte ptr [eax], al
        mov    dword ptr [eax], ebx
        add    byte ptr [eax], al
        mov    dword ptr [eax], ebx
        add    byte ptr [eax], al
        mov    dword ptr [eax], ebx
        add    byte ptr [eax], al
        mov    dword ptr [eax], ebx
        add    byte ptr [eax], al
        mov    dword ptr [eax], ebx
        add    byte ptr [eax], al
        mov    dword ptr [eax], ebx
        add    byte ptr [eax], al
        mov    dword ptr [eax], ebx
        add    byte ptr [eax], al
        mov    dword ptr [eax], ebx
        add    byte ptr [eax], al
        mov    dword ptr [eax], ebx
        add    byte ptr [eax], al
        push   ebx
        adc    eax, dword ptr [eax]
        add    byte ptr [ecx - 0x76ffffe8], cl ; -> loc_3_1889
        sbb    byte ptr [eax], al
        add    byte ptr [ecx + 0x7f000018], cl ; -> loc_3_1889
        adc    eax, dword ptr [eax]
loc_3_132F:
        add    byte ptr [eax + 0x68], al
        pushal 
        inc    edx
        inc    ecx
        db  062h, 061h
loc_3_1337:
        retf   
        db  016h, 000h, 000h
        dd  loc_3_1372               ; fixup
        dd  loc_3_18ED               ; fixup
        dd  loc_3_13A1               ; fixup
        dd  loc_3_14D0               ; fixup
        dd  loc_3_1656               ; fixup
        dd  loc_3_14D6               ; fixup
loc_3_1353:
        xor    ecx, ecx
        mov    cl, 7
        mov    ebx, dword ptr [edi + 0x94]
loc_3_135D:
        cmp    bl, byte ptr [ecx + 0x132f]   ; -> loc_3_132F
        je     loc_3_136A
        loop   loc_3_135D
        mov    al, 2
        ret    
loc_3_136A:
        dec    ecx
        jmp    loc_3_1337
loc_3_1372:
        mov    dword ptr [edi + 0xa0], 0x1a
        mov    al, 2
        ret    
loc_3_137F:
        mov    ecx, 7
        mov    ebx, dword ptr [edi + 0x94]
loc_3_138A:
        cmp    bl, byte ptr [ecx + 0x132f]   ; -> loc_3_132F
        je     loc_3_1397
        loop   loc_3_138A
        mov    al, 2
        ret    
loc_3_1397:
        dec    ecx
        mov    word ptr [edi + 0x6a], 0
        xor    al, al
        ret    
loc_3_13A1:
        push   esi
        mov    edx, dword ptr [edi + 0x98]
        mov    esi, dword ptr [edi + 8]
        test   byte ptr [edx], 1
        jne    loc_3_1444
        cmp    byte ptr [esi + 0x40], 0
        je     loc_3_1473
        test   byte ptr [esi + 0x24], 0x40
        je     loc_3_13E1
        cmp    dword ptr [esi + 0xd7], 0
        jne    loc_3_1473
        cmp    dword ptr [esi + 0xd3], 0x1680
        ja     loc_3_1473
loc_3_13E1:
        mov    ebx, dword ptr [esi + 0x2c]
        or     ebx, ebx
        je     loc_3_14A9
loc_3_13EC:
        mov    ecx, dword ptr [esi + 0xe7]
        movzx  eax, word ptr [edx + 1]
        movzx  edx, word ptr [edx + 3]
        mov    dword ptr [edi + 0x60], ecx
        or     ecx, ecx
        jne    loc_3_1404
        int3   
        mov    cl, 1
loc_3_1404:
        mov    byte ptr [ebx + ecx*4 - 4], dl
        mov    byte ptr [ebx + ecx*4 - 3], al
        loop   loc_3_1404
        mov    word ptr [edi + 0x68], 0xe
        mov    dword ptr [edi + 0xac], ebx
        mov    byte ptr [edi + 0x5c], al
        mov    word ptr [edi + 0x5e], dx
        mov    eax, dword ptr [edi + 0x14]
        mov    ebx, dword ptr [edi + 0x10]
        movsx  ebx, word ptr [ebx + 0x10]
        add    ebx, edi
        mov    dword ptr [eax + 4], ebx
        mov    word ptr [ebx], 2
        mov    dword ptr [eax], 0x16f        ; -> loc_1_016F
        add    dword ptr [edi + 0x14], 8
        pop    esi
        mov    al, 2
        cld    
        ret    
loc_3_1444:
        mov    ecx, dword ptr [esi]
        mov    eax, dword ptr [ecx + 0x68]
        cmp    dword ptr [ecx + 0xe7], eax
        ja     loc_3_1494
        mov    eax, dword ptr [ecx + 0x54]
        cmp    dword ptr [ecx + 0xd3], eax
        ja     loc_3_1494
        mov    dword ptr [edi + 0xa0], 0
        mov    byte ptr [edx], 0
        mov    word ptr [edi + 0x6a], 0
loc_3_146F:
        pop    esi
        xor    al, al
        ret    
loc_3_1473:
        mov    esi, dword ptr [edi + 4]
        test   byte ptr [esi + 0x24], 0x10
        je     loc_3_148E
        mov    dword ptr [edi + 0xa0], 5
        mov    word ptr [edi + 0x6a], 0x10
        jmp    loc_3_146F
loc_3_148E:
        pop    esi
        jmp    loc_3_1656
loc_3_1494:
        mov    dword ptr [edi + 0xa0], 1
        mov    word ptr [edi + 0x6a], 0x10
        mov    byte ptr [edx], 1
        jmp    loc_3_146F
loc_3_14A9:
        push   edi
        mov    edx, 0xfc
        call   sub_3_1A35
        mov    dword ptr [esi + 0x2c], ebx
        mov    edi, ebx
        mov    ecx, dword ptr [esi + 0xe7]
loc_3_14BF:
        mov    byte ptr [edi + ecx*4 - 2], cl
        mov    byte ptr [edi + ecx*4 - 3], 2
        loop   loc_3_14BF
        pop    edi
        jmp    loc_3_13EC
loc_3_14D0:
        mov    cx, 1
        jmp    loc_3_14DA
loc_3_14D6:
        mov    cx, 0
loc_3_14DA:
        mov    word ptr [edi + 0x68], cx
        mov    esi, 0x150f                   ; -> loc_3_150F
loc_3_14E3:
        mov    edx, edi
        VMMCall  VMM.Get_Crit_Status_Thread      ; cd2045010100
        xor    eax, eax
        inc    eax
        mov    ebx, dword ptr [edx + 0x1c]
        jecxz  loc_3_1508
        cmp    edi, ebx
        jne    loc_3_1508
        mov    ecx, 0x100
loc_3_14FC:
        or     ecx, 8
        VMMCall  VMM.Call_Restricted_Event       ; cd205a010100
        mov    al, 1
        ret    
loc_3_1508:
        mov    ecx, 0x102
        jmp    loc_3_14FC
loc_3_150F:
        push   dword ptr [edx + 0x6c]
        push   dword ptr [edx + 0x14]
        lea    esi, [edx + 0x2c]
        mov    ecx, 0x30
        sub    esp, ecx
        mov    edi, esp
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    edi, edx
        mov    byte ptr [edi + 0xb0], 1
        mov    esi, dword ptr [edi + 8]
        xchg   esi, edi
        mov    edx, dword ptr [esi + 0x98]
        mov    ax, word ptr [edx + 3]
        mov    ch, byte ptr [edx + 1]
        mov    cl, byte ptr [edx + 5]
        inc    cl
        mov    ebx, dword ptr [edi]
        xchg   edi, ebx
        call   sub_3_1A7B
        mov    ecx, dword ptr [esi + 0x98]
        add    eax, dword ptr [ebx + 0x28]
        mov    dword ptr [esi + 0x74], eax
        movzx  eax, word ptr [ecx + 7]
        mov    dword ptr [esi + 0x7c], eax
        mul    dword ptr [edi + 0xdb]
        push   eax
        mov    edi, ebx
        mov    eax, dword ptr [ecx + 9]
        test   byte ptr [esi + 0x6c], 0x10
        mov    dword ptr [esi + 0x6c], 0xd00
        je     loc_3_158B
        movzx  eax, word ptr [ecx + 0xb]
        movzx  ebx, word ptr [ecx + 9]
        mov    ecx, dword ptr [esi + 0x1c]
        mov    ecx, dword ptr [ecx + 0x14]
        call   sub_3_20B9
loc_3_158B:
        mov    dword ptr [esi + 0x80], eax
        pop    ecx
        dec    ecx
        mov    edx, eax
        and    edx, 0xfff
        add    ecx, edx
        shr    ecx, 0xc
        inc    ecx
        shr    eax, 0xc
        push   0x40000000
        push   ecx
        push   edx
        push   0xc0000000
        push   ecx
        push   eax
        VMMCall  VMM._LinPageLock                ; cd2063000100
        add    esp, 0xc
        pop    edx
        test   eax, eax
        je     loc_3_164D
        add    edx, eax
        shr    eax, 0xc
        push   eax
        mov    dword ptr [esi + 0x80], edx
        push   esi
        call   sub_1_0064
        add    esp, 4
        or     eax, eax
        jne    loc_3_1646
loc_3_15DE:
        mov    ebx, edi
        mov    edi, esi
        mov    edx, 0x84                     ; -> loc_1_0084
        call   sub_1_005C
        test   dword ptr [edi + 0x6c], 0x40
        jne    loc_3_15FF
        VMMCall  VMM._LinPageUnLock              ; cd2064000100
        add    esp, 0xc
loc_3_15FF:
        mov    dword ptr [edi + 0xa0], 0
        movzx  ecx, word ptr [edi + 0x6a]
        cmp    ecx, 2
        jbe    loc_3_161E
        VxDCall  BLOCKDEV.Service_000Fh          ; cd200f001000
        mov    dword ptr [edi + 0xa0], eax
loc_3_161E:
        mov    edx, edi
        lea    edi, [edx + 0x2c]
        mov    ecx, 0x30
        mov    ebx, ecx
        mov    esi, esp
        rep movsb byte ptr es:[edi], byte ptr [esi]
        add    esp, ebx
        mov    edi, edx
        pop    dword ptr [edi + 0x14]
        pop    dword ptr [edi + 0x6c]
        mov    word ptr [edi + 0x68], 0xd
        push   ebx
        push   esi
        push   edi
        jmp    loc_1_00B8
loc_3_1646:
        VMMCall  VMM._LinPageUnLock              ; cd2064000100
        pop    eax
loc_3_164D:
        add    esp, 8
        or     dword ptr [esi + 0x6c], 0x40
        jmp    loc_3_15DE
loc_3_1656:
        mov    esi, 0x1660                   ; -> loc_3_1660
        jmp    loc_3_14E3
loc_3_1660:
        push   dword ptr [edx + 0x6c]
        push   dword ptr [edx + 0x14]
        lea    esi, [edx + 0x2c]
        mov    ecx, 0x30
        sub    esp, ecx
        mov    edi, esp
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    edi, edx
        mov    word ptr [edi + 0x68], 2
        mov    dword ptr [edi + 0x6c], 0xd00
        mov    byte ptr [edi + 0xb0], 0
        mov    esi, dword ptr [edi + 4]
        xchg   esi, edi
        mov    edx, dword ptr [esi + 0x98]
        mov    ax, word ptr [edx + 3]
        mov    ch, byte ptr [edx + 1]
        mov    cl, 1
        call   sub_3_1A7B
        mov    edx, edi
        mov    edi, dword ptr [esi + 8]
        add    eax, dword ptr [edi + 0x28]
        mov    dword ptr [esi + 0x74], eax
        mov    eax, dword ptr [edx + 0xe7]
        mov    dword ptr [esi + 0x7c], eax
        mov    ebx, edi
        mov    edi, esi
        mov    edx, 0x84                     ; -> loc_1_0084
        call   sub_1_005C
        jmp    loc_3_15FF
loc_3_16CB:
        enter  4, 0
        push   esi
        mov    dword ptr [ebp - 4], edi
        mov    eax, dword ptr [edi + 8]
        mov    ebx, dword ptr [eax + 0x2c]
        or     ebx, ebx
        je     loc_3_1835
loc_3_16E1:
        mov    ecx, dword ptr [edi + 0x98]
        test   byte ptr [ecx], 2
        jne    loc_3_17C2
        push   ecx
        mov    edx, dword ptr [eax + 0x30]
        or     edx, edx
        je     loc_3_1726
        mov    si, word ptr [edx + 0x3f]
        and    si, 0xfdf7
        test   byte ptr [edx + 4], 0x80
        jne    loc_3_1722
        and    si, 0xfffc
        test   byte ptr [eax + 0x20], 4
        jne    loc_3_1715
        or     si, 1
loc_3_1715:
        test   dword ptr [eax + 0x20], 0x30000
        je     loc_3_1722
        or     si, 2
loc_3_1722:
        mov    word ptr [edx + 0x3f], si
loc_3_1726:
        mov    esi, dword ptr [edi + 4]
        test   byte ptr [ecx], 1
        je     loc_3_185B
        or     byte ptr [eax + 0x45], 1
        or     byte ptr [esi + 0xb8], 4
        mov    edi, dword ptr [eax + 0x30]
        lea    edi, [edi + 6]
        lea    esi, [ecx + 7]
        cmp    byte ptr [eax + 0x40], 0
        je     loc_3_1796
        movzx  ecx, word ptr [esi]
        mov    dword ptr [eax + 0xdb], ecx
        bsf    ecx, ecx
        mov    byte ptr [eax + 0x3c], cl
        movzx  ecx, word ptr [esi + 8]
        or     ecx, ecx
        jne    loc_3_1766
        mov    ecx, dword ptr [esi + 0x15]
loc_3_1766:
        mov    dword ptr [eax + 0xd3], ecx
        push   ebx
        mov    ebx, eax
        mov    eax, ecx
        movzx  ecx, word ptr [esi + 0xd]
        mov    dword ptr [ebx + 0xe7], ecx
        xor    edx, edx
        div    ecx
        movzx  ecx, word ptr [esi + 0xf]
        mov    dword ptr [ebx + 0xdf], ecx
        xor    edx, edx
        div    ecx
        mov    dword ptr [ebx + 0xe3], eax
        mov    eax, ebx
        pop    ebx
loc_3_1796:
        mov    ecx, 0x19
        cmp    byte ptr [eax + 0x3d], 0xb
        je     loc_3_17A7
        cmp    byte ptr [eax + 0x3d], 0xc
        jne    loc_3_17BE
loc_3_17A7:
        mov    ecx, dword ptr [ebp - 4]
        mov    ecx, dword ptr [ecx + 0x94]
        cmp    ch, 0x48
        jne    loc_3_1874
        mov    ecx, 0x35
loc_3_17BE:
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    ecx
loc_3_17C2:
        mov    esi, dword ptr [eax + 0x30]
        and    word ptr [esi + 0x3f], 0xfff7
        test   byte ptr [ecx], 4
        je     loc_3_17D4
        or     word ptr [esi + 0x3f], 8
loc_3_17D4:
        lea    esi, [ecx + 0x28]
        mov    edi, ebx
        movzx  ecx, word ptr [ecx + 0x26]
        cmp    ecx, 0x3f
        ja     loc_3_181F
        or     ecx, ecx
        je     loc_3_1807
        cmp    byte ptr [eax + 0x40], 0
        je     loc_3_17F2
        mov    dword ptr [eax + 0xe7], ecx
loc_3_17F2:
        cld    
loc_3_17F3:
        add    edi, 2
        lodsw  ax, word ptr [esi]
        stosb  byte ptr es:[edi], al
        lodsw  ax, word ptr [esi]
        mov    al, 3
        cmp    ah, 2
        jg     loc_3_1804
        mov    al, ah
loc_3_1804:
        stosb  byte ptr es:[edi], al
        loop   loc_3_17F3
loc_3_1807:
        mov    edi, dword ptr [ebp - 4]
        mov    dword ptr [edi + 0xa0], 0
        mov    word ptr [edi + 0x6a], 0
loc_3_181A:
        xor    al, al
        pop    esi
        leave  
        ret    
loc_3_181F:
        mov    eax, 0xc
        mov    edi, dword ptr [ebp - 4]
        mov    dword ptr [edi + 0xa0], eax
        mov    word ptr [edi + 0x6a], 0x10
        jmp    loc_3_181A
loc_3_1835:
        mov    edx, 0xfc
        call   sub_3_1A35
        mov    dword ptr [eax + 0x2c], ebx
        mov    ecx, 0x3f
        mov    edx, 0x2000000
loc_3_184C:
        mov    dword ptr [ebx + ecx*4 - 4], edx
        mov    byte ptr [ebx + ecx*4 - 2], cl
        loop   loc_3_184C
        jmp    loc_3_16E1
loc_3_185B:
        and    byte ptr [eax + 0x45], 0xfe
        and    byte ptr [esi + 0xb8], 0xfb
        mov    edi, dword ptr [eax + 0x30]
        lea    edi, [edi + 0x43]
        lea    esi, [ecx + 7]
        jmp    loc_3_1796
loc_3_1874:
        mov    edi, dword ptr [ebp - 4]
        mov    dword ptr [edi + 0xa0], 0x16
        mov    word ptr [edi + 0x6a], 0x16
        jmp    loc_3_181A
loc_3_1889:
        mov    al, 2
        ret    
        db  080h, 002h, 0D0h, 002h, 0A0h, 005h, 060h, 009h, 040h, 00Bh, 080h, 016h
loc_3_1898:
        add    eax, 0x7070706
        pop    es
        pop    es
        or     byte ptr [edi], al
        pop    es
        inc    dword ptr [eax]
        add    byte ptr [edx], al
        add    dword ptr [edi], eax
        db  009h
sub_3_18A9:
        push   edi
        movzx  eax, byte ptr [ebx + 0x40]
        cmp    eax, 0xb
        jg     loc_3_18E5
        mov    al, byte ptr [eax + 0x1898]   ; -> loc_3_1898
        cmp    al, 0xff
        jne    loc_3_18E1
        mov    eax, edx
        lea    edi, [0x1896]                 ; -> obj3:0x1896
        mov    ecx, 6
        std    
        repne scasw ax, word ptr es:[edi]
        jne    loc_3_18E5
        cmp    ecx, 6
        jge    loc_3_18E5
        movzx  eax, byte ptr [ecx + 0x18a3]  ; -> obj3:0x18A3
loc_3_18E1:
        clc    
loc_3_18E2:
        cld    
        pop    edi
        ret    
loc_3_18E5:
        mov    eax, 7
        stc    
        jmp    loc_3_18E2
loc_3_18ED:
        enter  4, 0
        push   esi
        mov    dword ptr [ebp - 4], edi
        mov    ebx, dword ptr [edi + 8]
        mov    edx, dword ptr [ebx + 0x54]
        call   sub_3_18A9
        mov    ebx, dword ptr [edi + 8]
        mov    edx, dword ptr [edi + 0x98]
        mov    byte ptr [edx + 1], al
        xor    ecx, ecx
        mov    eax, dword ptr [ebx + 0x20]
        test   eax, 0x30000
        je     loc_3_191C
        mov    cx, 2
loc_3_191C:
        test   eax, 4
        jne    loc_3_1926
        or     cl, 1
loc_3_1926:
        test   byte ptr [edx], 1
        je     loc_3_1937
        test   byte ptr [ebx + 0x24], 4
        je     loc_3_1937
        or     ecx, 0x800
loc_3_1937:
        mov    word ptr [edx + 2], cx
        mov    ecx, dword ptr [ebx + 0x30]
        mov    ax, word ptr [ecx + 0x41]
        mov    word ptr [edx + 4], ax
        xor    al, al
        mov    byte ptr [edx + 6], al
        mov    esi, dword ptr [ebx + 0x30]
        lea    ecx, [esi + 6]
        test   byte ptr [edx], 1
        jne    loc_3_1959
        lea    ecx, [esi + 0x43]
loc_3_1959:
        mov    esi, ecx
        lea    edi, [edx + 7]
        mov    ecx, dword ptr [ebp - 4]
        cmp    byte ptr [ecx + 0x95], 0x48
        mov    ecx, 0x35
        je     loc_3_197B
        mov    ecx, 0x19
        cmp    word ptr [esi + 0xb], 0
        je     loc_3_1994
loc_3_197B:
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_3_197E:
        mov    edi, dword ptr [ebp - 4]
        mov    dword ptr [edi + 0xa0], 0
        mov    word ptr [edi + 0x6a], 0
loc_3_1991:
        pop    esi
        leave  
        ret    
loc_3_1994:
        cmp    dword ptr [0x2bc], 0          ; -> loc_1_02BC
        je     loc_3_19C5
        mov    ecx, 0x19
        cld    
        push   edi
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    edi
        mov    word ptr [edi + 6], 0x200
        mov    esi, dword ptr [ebx + 0x30]
        mov    ecx, dword ptr [esi + 0x1f]
        cmp    ecx, 0xffff
        jbe    loc_3_19BF
        mov    cx, 0xffff
loc_3_19BF:
        mov    word ptr [edi + 0xb], cx
        jmp    loc_3_197E
loc_3_19C5:
        mov    edi, dword ptr [ebp - 4]
        mov    dword ptr [edi + 0xa0], 0x16
        mov    word ptr [edi + 0x6a], 0x16
        jmp    loc_3_1991
        db  000h, 000h
sub_3_19DC:
        push   esi
        lea    esi, [ebx + 0xb]
        mov    al, byte ptr [esi + 0xa]
        and    al, 0xf0
        cmp    al, 0xf0
        je     loc_3_19EB
        jmp    loc_3_1A2D
loc_3_19EB:
        mov    al, byte ptr [esi + 2]
        or     al, al
        je     loc_3_1A00
        shr    al, 1
loc_3_19F4:
        jb     loc_3_19FA
        shr    al, 1
        jmp    loc_3_19F4
loc_3_19FA:
        or     al, al
        je     loc_3_1A00
        jmp    loc_3_1A2D
loc_3_1A00:
        xor    dx, dx
        movzx  eax, word ptr [esi + 8]
        or     eax, eax
        jne    loc_3_1A16
        mov    eax, dword ptr [esi + 0x15]
        or     eax, eax
        jne    loc_3_1A16
        jmp    loc_3_1A2D
        db  0EBh, 017h
loc_3_1A16:
        movzx  ecx, word ptr [esi + 0xb]
        or     ecx, ecx
        je     loc_3_1A31
        shl    ecx, 1
        sub    eax, ecx
        movzx  ecx, word ptr [esi + 6]
        shr    ecx, 4
        sub    eax, ecx
        jae    loc_3_1A31
loc_3_1A2D:
        mov    al, 1
        pop    esi
        ret    
loc_3_1A31:
        pop    esi
        xor    eax, eax
        ret    
sub_3_1A35:
        pushal 
        sub    esp, 0x10
        mov    eax, esp
        mov    word ptr [eax + 4], dx
        mov    word ptr [eax + 0xa], 9
        mov    word ptr [eax], 3
        mov    byte ptr [eax + 0xc], 8
        push   eax
        call   sub_1_0050
        mov    eax, dword ptr [eax + 6]
        add    esp, 0x14
        mov    dword ptr [esp + 0x10], eax
        popal  
        ret    
sub_3_1A61:
        pushal 
        sub    esp, 8
        mov    eax, esp
        mov    dword ptr [eax + 4], edi
        mov    word ptr [eax], 4
        push   eax
        call   sub_1_0050
        add    esp, 0xc
        popal  
        ret    
sub_3_1A7B:
        push   ebx
        movzx  ebx, ax
        mov    eax, ebx
        mov    ebx, dword ptr [edi + 0xdf]
        mul    ebx
        movzx  ebx, ch
        add    eax, ebx
        mov    ebx, dword ptr [edi + 0xe7]
        mul    ebx
        movzx  ebx, cl
        dec    ebx
        add    eax, ebx
        pop    ebx
        ret    
        db  000h, 002h, 002h, 001h, 000h, 002h, 070h, 000h, 0D0h, 002h, 0FDh, 002h, 000h, 009h, 000h, 002h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 090h, 000h, 002h, 001h, 001h, 000h, 002h
        db  0E0h, 000h, 060h, 009h, 0F9h, 007h, 000h, 00Fh, 000h, 002h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 090h, 000h, 002h, 002h, 001h, 000h, 002h, 070h, 000h, 0A0h, 005h, 0F9h, 003h
        db  000h, 009h, 000h, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 090h, 000h, 002h
        db  002h, 001h, 000h, 002h, 0F0h, 000h, 080h, 016h, 0F0h, 009h, 000h, 024h, 000h, 002h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 090h, 000h, 002h, 001h, 001h, 000h, 002h, 0E0h, 000h
        db  000h, 000h, 0F0h, 002h, 000h, 009h, 000h, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h
loc_3_1B1F:
        sahf   
        sbb    al, byte ptr [eax]
        add    byte ptr [eax - 0x2dffffe6], bh ; -> obj3:0x1AB8
        sbb    al, byte ptr [eax]
        add    ah, ch                        ; -> obj3:0x1AEC
        sbb    al, byte ptr [eax]
        db  000h
loc_3_1B2F:
        add    al, byte ptr [eax]
loc_3_1B31:
        add    eax, dword ptr [eax]
loc_3_1B33:
        add    byte ptr [edx], al
sub_3_1B35:
        mov    eax, edi
        mov    edx, esi
        lea    esi, [0x1b06]                 ; -> obj3:0x1B06
        mov    edi, dword ptr [edi + 0x30]
        mov    ecx, 0x19
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    edi, eax
        mov    esi, edx
        mov    ecx, dword ptr [edi + 0x30]
        mov    eax, dword ptr [edi + 0x60]
        mov    word ptr [ecx + 0xf], ax
        mov    eax, dword ptr [edi + 0x68]
        mov    word ptr [ecx + 0xd], ax
        mov    eax, dword ptr [edi + 0x54]
        cmp    eax, 0xffff
        ja     loc_3_1B6E
        mov    word ptr [edi + 0x38], ax
        jmp    loc_3_1B71
loc_3_1B6E:
        mov    dword ptr [edi + 0x45], eax
loc_3_1B71:
        mov    edx, 0
        cmp    dword ptr [edi + 0x64], 0x28
        jne    loc_3_1BC1
        cmp    dword ptr [edi + 0x68], 9
        jbe    loc_3_1BE1
loc_3_1B82:
        mov    eax, dword ptr [edi + 0x54]
        dec    eax
        mov    dl, 1
loc_3_1B88:
        cmp    eax, 0xff6
        jb     loc_3_1B95
        shr    eax, 1
        shl    dl, 1
        jmp    loc_3_1B88
loc_3_1B95:
        cmp    dl, 1
        je     loc_3_1BA0
        mov    word ptr [ecx + 6], 0xf0
loc_3_1BA0:
        mov    byte ptr [ecx + 2], dl
        mul    word ptr [0x1b31]             ; -> loc_3_1B31
        div    word ptr [0x1b2f]             ; -> loc_3_1B2F
        xor    edx, edx
        div    word ptr [0x1b33]             ; -> loc_3_1B33
        inc    eax
        mov    word ptr [ecx + 0xb], ax
        jmp    loc_3_1BF7
loc_3_1BC1:
        cmp    dword ptr [edi + 0x64], 0x50
        jne    loc_3_1B82
        mov    dl, 3
        cmp    dword ptr [edi + 0x68], 0x24
        je     loc_3_1BE1
        cmp    dword ptr [edi + 0x68], 0xf
        je     loc_3_1BDF
        cmp    dword ptr [edi + 0x68], 9
        jne    loc_3_1B82
        mov    dl, 2
        jmp    loc_3_1BE1
loc_3_1BDF:
        mov    dl, 1
loc_3_1BE1:
        push   edi
        push   esi
        mov    esi, 0x1b1f                   ; -> loc_3_1B1F
        mov    esi, dword ptr [esi + edx*4]
        mov    edi, dword ptr [edi + 0x30]
        mov    ecx, 0x19
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    esi
        pop    edi
loc_3_1BF7:
        ret    
loc_3_1BF8:
        test   al, 0x7f
        add    ecx, dword ptr [eax]
        db  0FFh, 0FFh, 002h, 004h
loc_3_1C00:
        add    eax, dword ptr [eax]
        add    al, byte ptr [edi + eax]
        add    byte ptr [ebx], al
        or     byte ptr [edi], cl
        add    byte ptr [eax + edx], al
        pop    ds
        add    byte ptr [0x6003f20], al
        inc    eax
        jg     loc_3_1C16
loc_3_1C16:
        pop    es
        cmp    bh, 0xff
        pop    es
        db  080h
loc_3_1C1C:
        or     byte ptr [eax], al
        add    byte ptr [ecx], al
        and    byte ptr [eax], al
        add    al, byte ptr [eax + eax*8]
        add    byte ptr [ebx], al
        or     byte ptr [eax], al
        add    al, byte ptr [eax + edx]
        add    byte ptr [eax + 0x6ffff20], al
        inc    eax
sub_3_1C34:
        push   ebx
        push   esi
        push   edi
        mov    eax, dword ptr [edi]
        mov    edi, dword ptr [edi + 0x30]
        mov    esi, dword ptr [eax + 0xe7]
        mov    word ptr [edi + 0x50], si
        mov    esi, dword ptr [eax + 0xdf]
        mov    word ptr [edi + 0x52], si
        mov    word ptr [edi + 0x43], 0x200
        mov    byte ptr [edi + 0x45], 1
        mov    word ptr [edi + 0x46], 1
        mov    byte ptr [edi + 0x48], 2
        mov    word ptr [edi + 0x49], 0xe0
        mov    ecx, eax
        movzx  eax, word ptr [edi + 0xe]
        or     eax, eax
        jne    loc_3_1CBA
        mov    eax, dword ptr [edi + 0x1b]
        or     eax, eax
        jne    loc_3_1C88
        mov    eax, dword ptr [ecx + 0xd3]
        cmp    eax, 0xffff
        jbe    loc_3_1CBA
loc_3_1C88:
        mov    dword ptr [edi + 0x58], eax
        mov    word ptr [edi + 0x4b], 0
        lea    esi, [0x1c1c]                 ; -> loc_3_1C1C
        push   eax
        cmp    eax, 0x400000
        ja     loc_3_1CB5
        mov    ebx, dword ptr [esp + 4]
        cmp    byte ptr [ebx + 0x3d], 0xb
        je     loc_3_1CB5
        cmp    byte ptr [ebx + 0x3d], 0xc
        je     loc_3_1CB5
        lea    esi, [0x1c00]                 ; -> loc_3_1C00
loc_3_1CB5:
        rol    eax, 0x10
        jmp    loc_3_1CCC
loc_3_1CBA:
        push   eax
        mov    dword ptr [edi + 0x58], 0
        mov    word ptr [edi + 0x4b], ax
        lea    esi, [0x1bf8]                 ; -> loc_3_1BF8
loc_3_1CCC:
        cmp    ax, word ptr [esi]
        jbe    loc_3_1CD6
        add    esi, 4
        jmp    loc_3_1CCC
loc_3_1CD6:
        pop    eax
        mov    ebx, eax
        rol    ebx, 0x10
        mov    cx, word ptr [esi + 2]
        mov    esi, dword ptr [esp]
        cmp    eax, 0x400000
        ja     loc_3_1DD6
        cmp    byte ptr [esi + 0x3d], 0xb
        je     loc_3_1DD6
        cmp    byte ptr [esi + 0x3d], 0xc
        je     loc_3_1DD6
        mov    esi, dword ptr [esi]
        mov    dx, 0x200
        mov    word ptr [edi + 0x49], dx
        mov    byte ptr [edi + 0x45], ch
        mov    byte ptr [edi + 0x4d], 0xf8
        push   eax
        mov    eax, dword ptr [esi + 0xdb]
        mov    word ptr [edi + 0x43], ax
        cmp    eax, 0x200
        pop    eax
        jne    loc_3_1D64
        mov    word ptr [edi + 0x43], 0x200
        or     bx, bx
        jne    loc_3_1D6B
        cmp    ax, 0x7fa8
        ja     loc_3_1D6B
        mov    bl, ch
        dec    bx
        add    bx, ax
        shr    bx, cl
        inc    bx
        and    bl, 0xfe
        add    bx, 2
        mov    si, bx
        shr    bx, 1
        add    bx, si
        add    bx, 0x1ff
        shr    bh, 1
        xor    bl, bl
        xchg   bl, bh
        mov    word ptr [edi + 0x4e], bx
        jmp    loc_3_1DD2
loc_3_1D64:
        call   sub_3_1E3A
        jmp    loc_3_1DD2
loc_3_1D6B:
        shr    dx, 4
        sub    ax, dx
        sbb    bx, 0
        sub    ax, 1
        sbb    bx, 0
        xchg   bx, dx
        push   eax
        push   edx
        mov    bl, 2
        mov    bh, byte ptr [edi + 0x45]
        add    ax, bx
        adc    dx, 0
        sub    ax, 1
        sbb    dx, 0
        div    bx
        mov    word ptr [edi + 0x4e], ax
        pop    edx
        pop    ebx
        shl    ax, 1
        xchg   bx, ax
        sub    ax, bx
        sbb    dx, 0
        xor    bh, bh
        mov    bl, ch
        shl    bx, 1
        add    ax, bx
        adc    dx, 0
        shl    bx, 7
        div    bx
        or     dx, dx
        je     loc_3_1DC8
        inc    ax
loc_3_1DC8:
        cmp    ax, word ptr [edi + 0x4e]
        jbe    loc_3_1DD2
        mov    word ptr [edi + 0x4e], ax
loc_3_1DD2:
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_3_1DD6:
        mov    esi, dword ptr [esi]
        push   eax
        push   ecx
        sub    eax, 0x20
        mov    ebx, dword ptr [esi + 0xdb]
        mov    edx, ebx
        shl    edx, cl
        add    edx, 8
        mov    ecx, edx
        mul    ebx
        div    ecx
        shl    eax, 2
        lea    eax, [ebx + eax - 1]
        xor    edx, edx
        mov    ebx, dword ptr [esi + 0xdb]
        div    ebx
        pop    ecx
        pop    edx
        and    word ptr [edi + 0x49], 0
        and    word ptr [edi + 0x4e], 0
        mov    word ptr [edi + 0x46], 0x20
        mov    byte ptr [edi + 0x48], 2
        mov    dword ptr [edi + 0x64], 2
        mov    word ptr [edi + 0x68], 1
        mov    word ptr [edi + 0x6a], 6
        mov    byte ptr [edi + 0x45], ch
        mov    byte ptr [edi + 0x4d], 0xf8
        mov    word ptr [edi + 0x43], bx
        mov    dword ptr [edi + 0x5c], eax
        jmp    loc_3_1DD2
sub_3_1E3A:
        or     bx, bx
        jne    loc_3_1E6F
        cmp    ax, 0x7fa8
        ja     loc_3_1E6F
        movzx  ebx, ch
        dec    ebx
        movzx  eax, ax
        add    ebx, eax
        shr    ebx, cl
        inc    ebx
        and    bl, 0xfe
        add    ebx, 2
        mov    eax, ebx
        shr    ebx, 1
        add    ebx, eax
        movzx  eax, word ptr [edi + 0x43]
        dec    eax
        xchg   ebx, eax
        add    eax, ebx
        inc    ebx
        xor    edx, edx
        div    ebx
        mov    word ptr [edi + 0x4e], ax
        ret    
loc_3_1E6F:
        rol    eax, 0x10
        mov    ax, bx
        rol    eax, 0x10
        push   ecx
        movzx  ecx, word ptr [edi + 0x43]
        shr    ecx, 5
        push   eax
        movzx  eax, dx
        xor    edx, edx
        div    ecx
        mov    ecx, eax
        pop    eax
        sub    eax, ecx
        sub    eax, 2
        push   eax
        movzx  ecx, word ptr [edi + 0x43]
        shr    ecx, 1
        movzx  eax, byte ptr [edi + 0x45]
        mul    ecx
        add    eax, 2
        mov    ebx, eax
        pop    eax
        push   eax
        xor    edx, edx
        div    ebx
        inc    eax
        mov    word ptr [edi + 0x4e], ax
        pop    edx
        dec    edx
        shl    eax, 1
        sub    edx, eax
        pop    ecx
        movzx  ebx, ch
        shl    ebx, 1
        add    edx, ebx
        movzx  eax, word ptr [edi + 0x43]
        shr    eax, 2
        mov    ecx, edx
        xor    edx, edx
        mul    ebx
        xchg   ecx, eax
        div    ecx
        or     edx, edx
        je     loc_3_1ED0
        inc    eax
loc_3_1ED0:
        cmp    ax, word ptr [edi + 0x43]
        jbe    loc_3_1EDA
        mov    word ptr [edi + 0x43], ax
loc_3_1EDA:
        ret    
sub_3_1EDB:
        push   edi
        push   esi
        push   edx
        push   ebx
        push   ecx
        mov    ecx, dword ptr [esi + 4]
        add    ecx, 0x64
        sub    esp, 0x10
        mov    edi, esp
        mov    dword ptr [edi + 6], ecx
        add    ecx, 0x68
        mov    word ptr [edi + 4], cx
        mov    byte ptr [edi + 0xe], 0
        mov    word ptr [edi], 2
        push   edi
        call   sub_1_0050
        mov    edi, dword ptr [edi + 0xa]
        add    esp, 0x14
        or     edi, edi
        je     loc_3_1FA7
        add    edi, dword ptr [esi + 4]
        mov    word ptr [edi + 0x68], dx
        xor    edx, edx
        mov    dword ptr [edi + 0x78], edx
        mov    dword ptr [edi + 0xa8], edx
        mov    dword ptr [edi + 0x64], 0
        mov    dword ptr [edi + 0x6c], 0xd01
        or     ebx, ebx
        je     loc_3_1F38
        inc    edx
loc_3_1F38:
        mov    dword ptr [edi + 0x7c], edx
        mov    dword ptr [edi + 0x74], eax
        mov    dword ptr [edi + 0xac], edi
        add    dword ptr [edi + 0xac], 0xbc
        mov    dword ptr [edi + 0x80], ebx
        or     ebx, ebx
        je     loc_3_1F6D
        mov    dword ptr [edi + 8], esi
        push   edi
        call   sub_1_0064
        add    esp, 4
        or     eax, eax
        je     loc_3_1F6D
        or     dword ptr [edi + 0x6c], 0x40
loc_3_1F6D:
        mov    dl, byte ptr [esi + 0x15]
        mov    byte ptr [edi + 0xb1], dl
        mov    ebx, esi
        mov    edx, 0x84                     ; -> loc_1_0084
        call   sub_1_005C
        movzx  eax, word ptr [edi + 0x6a]
        sub    edi, dword ptr [ebx + 4]
        sub    esp, 8
        mov    esi, esp
        mov    dword ptr [esi + 4], edi
        mov    word ptr [esi], 4
        push   esi
        call   sub_1_0050
        add    esp, 0xc
loc_3_1FA1:
        pop    ecx
        pop    ebx
        pop    edx
        pop    esi
        pop    edi
        ret    
loc_3_1FA7:
        mov    ax, 0x1d
        jmp    loc_3_1FA1
sub_3_1FAD:
        push   ebx
        push   eax
        push   ecx
        mov    eax, dword ptr [0x2b3]        ; -> obj1:0x02B3
        mov    cl, byte ptr [esi + 0x14]
loc_3_1FB8:
        cmp    byte ptr [eax + 5], cl
        je     loc_3_1FF6
        mov    eax, dword ptr [eax]
        cmp    ax, -1
        je     loc_3_1FE0
        push   ecx
        movzx  ecx, ax
        rol    eax, 0x10
        movzx  eax, ax
        shl    eax, 4
        add    eax, ecx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        add    eax, dword ptr [ebx + 4]
        pop    ecx
        jmp    loc_3_1FB8
loc_3_1FE0:
        push   1
        push   0x96
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        or     eax, eax
        or     byte ptr [esi + 0x17], 2
loc_3_1FF6:
        cmp    dword ptr [esi + 0x30], 0
        jne    loc_3_2003
        mov    dword ptr [esi + 0x30], eax
loc_3_1FFF:
        pop    ecx
        pop    eax
        pop    ebx
        ret    
loc_3_2003:
        mov    dword ptr [esi + 0x12d], eax
        jmp    loc_3_1FFF
sub_3_200B:
        push   ebp
        sub    esp, 0xc
        mov    ebp, esp
        mov    word ptr [ebp], 7
        mov    dword ptr [ebp + 8], eax
        push   ebp
        call   sub_1_0050
        mov    esi, dword ptr [ebp + 4]
        movzx  eax, word ptr [ebp + 2]
        add    esp, 0x10
        pop    ebp
        ret    
sub_3_202D:
        sub    esp, 0xc
        mov    ebx, esp
        mov    dword ptr [ebx + 4], esi
        mov    byte ptr [ebx + 0xa], al
        mov    word ptr [ebx], 0x10
        push   ebx
        call   sub_1_0050
        mov    ax, word ptr [ebx + 2]
        or     ah, al
        mov    al, byte ptr [ebx + 8]
        add    esp, 0x10
        ret    
sub_3_2051:
        sub    esp, 0xc
        mov    ebx, esp
        mov    byte ptr [ebx + 8], al
        mov    byte ptr [ebx + 9], 0
        mov    dword ptr [ebx + 4], edi
        mov    word ptr [ebx], 6
        push   ebx
        call   sub_1_0050
        add    esp, 0x10
        ret    
sub_3_2070:
        sub    esp, 8
        mov    eax, esp
        mov    word ptr [eax], 0xa
        mov    dword ptr [eax + 4], esi
        push   eax
        call   sub_1_0050
        add    esp, 0xc
        ret    
sub_3_2088:
        sub    esp, 0xc
        mov    edi, esp
        mov    word ptr [edi], 1
        mov    word ptr [edi + 4], 0x50
        push   edi
        call   sub_1_0050
        mov    edi, dword ptr [edi + 6]
        add    esp, 0x10
        call   sub_3_10AB
        mov    dword ptr [edi + 0xc], 0
        mov    dword ptr [edi + 0x10], 0
        ret    
sub_3_20B9:
        push   ecx
        push   ebx
        shl    eax, 4
        movzx  ebx, bx
        add    eax, ebx
        add    eax, dword ptr [ecx + 4]
        pop    ebx
        pop    ecx
        ret    
sub_3_20C9:
        push   edi
        push   esi
        push   ebx
        xor    edx, edx
        or     esi, esi
        je     loc_3_2104
        xor    ebx, ebx
loc_3_20D4:
        push   edx
        push   0
        push   0x14
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        pop    edx
        or     eax, eax
        je     loc_3_2109
        mov    edi, eax
        mov    ecx, 5
        rep movsd dword ptr es:[edi], dword ptr [esi]
        or     edx, edx
        jne    loc_3_20F8
        mov    edx, eax
        jmp    loc_3_20FB
loc_3_20F8:
        mov    dword ptr [ebx + 0xc], eax
loc_3_20FB:
        mov    ebx, eax
        mov    esi, dword ptr [esi - 8]
        or     esi, esi
        jne    loc_3_20D4
loc_3_2104:
        clc    
loc_3_2105:
        pop    ebx
        pop    esi
        pop    edi
        ret    
loc_3_2109:
        stc    
        jmp    loc_3_2105
sub_3_210C:
        mov    ecx, dword ptr [0x2b3]        ; -> obj1:0x02B3
loc_3_2112:
        cmp    byte ptr [ecx + 4], dl
        je     loc_3_2139
        mov    eax, dword ptr [ecx]
        cmp    ax, -1
        je     loc_3_2138
        movzx  ecx, ax
        rol    eax, 0x10
        movzx  eax, ax
        shl    eax, 4
        add    ecx, eax
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        add    ecx, dword ptr [ebx + 4]
        jmp    loc_3_2112
loc_3_2138:
        stc    
loc_3_2139:
        ret    
VxD__LTEXT ENDS

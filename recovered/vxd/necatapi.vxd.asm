; NECATAPI -- reconstructed 32-bit disassembly (VxD / LE format)
; 4 objects, 406 instructions recovered by recursive descent

Declare_Virtual_Device NECATAPI, 4, 0, NECATAPI_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 080000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x3a8  R-X
NECATAPI_Control:
        cmp    eax, 0x1b
        je     loc_2_0000
        clc    
        ret    
        db  000h
loc_1_000C:
        mov    ebx, dword ptr [esp + 4]
        mov    word ptr [ebx + 2], 0
        mov    si, word ptr [ebx]
        cmp    si, 3
        je     loc_3_0000
        cmp    si, 2
        je     loc_3_0081
        mov    word ptr [ebx + 2], 0xffff
        ret    
loc_1_0034:
        push   edi
        push   esi
        push   ebx
        mov    esi, dword ptr [esp + 0x10]
        cmp    word ptr [esi + 0x6a], 0x10
        jae    loc_1_005F
        mov    edi, dword ptr [esi + 8]
        cmp    dword ptr [edi + 0xa2], 0x31302e31
        jne    loc_1_0056
        call   sub_1_0094
loc_1_0056:
        call   sub_1_0084
loc_1_005B:
        pop    ebx
        pop    esi
        pop    edi
        ret    
loc_1_005F:
        call   sub_1_0081
        or     al, al
        je     loc_1_0056
        cmp    al, 2
        je     loc_1_005B
        add    dword ptr [esi + 0x14], 8
        mov    eax, dword ptr [esi + 0x10]
        mov    eax, dword ptr [eax + 0xc]
        mov    dword ptr [esi + 0x10], eax
        push   esi
        call   dword ptr [eax]
        add    esp, 4
        jmp    loc_1_005B
sub_1_0081:
        xor    al, al
        ret    
sub_1_0084:
        mov    edi, dword ptr [esi + 0x14]
        sub    edi, 8
        mov    dword ptr [esi + 0x14], edi
        push   esi
        call   dword ptr [edi]
        add    esp, 4
        ret    
sub_1_0094:
        push   ebx
        mov    ebx, dword ptr [esi + 0x20]
        cmp    byte ptr [ebx + 0x30], 0x43
        jne    loc_1_00BA
        cmp    byte ptr [ebx + 0x39], 0x40
        jne    loc_1_00AB
        call   sub_1_00E9
        jmp    loc_1_00E7
loc_1_00AB:
        cmp    byte ptr [ebx + 0x39], 0
        jne    loc_1_00B8
        call   sub_1_0107
        jmp    loc_1_00E7
loc_1_00B8:
        jmp    loc_1_00E7
loc_1_00BA:
        cmp    byte ptr [ebx + 0x30], 0x42
        jne    loc_1_00C7
        call   sub_1_013D
        jmp    loc_1_00E7
loc_1_00C7:
        cmp    byte ptr [ebx + 0x30], 0x47
        jne    loc_1_00D4
        call   sub_1_0169
        jmp    loc_1_00E7
loc_1_00D4:
        cmp    byte ptr [ebx + 0x30], 0x44
        jne    loc_1_00E7
        test   byte ptr [ebx + 0x31], 2
        je     loc_1_00E7
        call   sub_1_017A
        jmp    loc_1_00E7
loc_1_00E7:
        pop    ebx
        ret    
sub_1_00E9:
        mov    edi, dword ptr [esi + 0x80]
        cmp    byte ptr [edi + 3], 1
        je     loc_1_00F9
        mov    byte ptr [edi + 3], 2
loc_1_00F9:
        add    edi, 8
        mov    ecx, 4
        call   sub_1_018E
        ret    
sub_1_0107:
        push   ebx
        mov    ebx, 0x64
        mov    edi, dword ptr [esi + 0x80]
        add    edi, 4
loc_1_0116:
        mov    ecx, 4
loc_1_011B:
        mov    al, byte ptr [edi + 4]
        call   sub_1_019B
        mov    byte ptr [edi + 4], al
        inc    edi
        loop   loc_1_011B
        sub    edi, 4
        mov    al, byte ptr [edi + 2]
        cmp    al, 0xaa
        je     loc_1_013B
        dec    ebx
        je     loc_1_013B
        add    edi, 8
        jmp    loc_1_0116
loc_1_013B:
        pop    ebx
        ret    
sub_1_013D:
        mov    eax, dword ptr [esi + 0x7c]
        cmp    eax, 0xc
        jl     loc_1_015A
        mov    edi, dword ptr [esi + 0x80]
        cmp    word ptr [edi + 2], 0
        je     loc_1_015A
        add    edi, 4
        cmp    byte ptr [edi], 1
        je     loc_1_015B
loc_1_015A:
        ret    
loc_1_015B:
        mov    ecx, 4
        add    ecx, 4
        call   sub_1_018E
        ret    
sub_1_0169:
        mov    ecx, 3
        add    ecx, 3
        lea    edi, [ebx + 0x33]
        call   sub_1_018E
        ret    
sub_1_017A:
        mov    edi, dword ptr [esi + 0x80]
        add    edi, 4
        mov    ecx, 4
        call   sub_1_018E
        ret    
sub_1_018E:
        mov    al, byte ptr [edi]
        call   sub_1_019B
        mov    byte ptr [edi], al
        inc    edi
        loop   sub_1_018E
        ret    
sub_1_019B:
        push   ecx
        mov    ch, al
        mov    cl, 4
        shr    al, cl
        mov    cl, 0xa
        mul    cl
        and    ch, 0xf
        add    al, ch
        pop    ecx
        ret    
        db  000h, 000h, 000h
sub_1_01B0:
        pop    ds
        add    eax, dword ptr [eax]
        add    byte ptr [edi], bl            ; -> loc_1_031F
        add    eax, dword ptr [eax]
        add    byte ptr [edi], bl            ; -> loc_1_031F
        add    eax, dword ptr [eax]
        add    byte ptr [edi], bl            ; -> loc_1_031F
        add    eax, dword ptr [eax]
        add    byte ptr [edi], bl            ; -> loc_1_031F
        add    eax, dword ptr [eax]
        add    byte ptr [edi], bl            ; -> loc_1_031F
        add    eax, dword ptr [eax]
        add    byte ptr [edi], bl            ; -> loc_1_031F
        add    eax, dword ptr [eax]
        add    byte ptr [edi], bl            ; -> loc_1_031F
        add    eax, dword ptr [eax]
        add    byte ptr [edi], bl            ; -> loc_1_031F
        add    eax, dword ptr [eax]
        add    byte ptr [edi], bl            ; -> loc_1_031F
        add    eax, dword ptr [eax]
        add    byte ptr [edi], bl            ; -> loc_1_031F
        add    eax, dword ptr [eax]
        add    byte ptr [edi], bl            ; -> loc_1_031F
        add    eax, dword ptr [eax]
        add    byte ptr [edi], bl            ; -> loc_1_031F
        add    eax, dword ptr [eax]
        add    byte ptr [edi], bl            ; -> loc_1_031F
        add    eax, dword ptr [eax]
        db  000h
loc_1_01E8:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        mov    ebx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebx + 0x14]
        mov    dword ptr [eax], 0x34         ; -> loc_1_0034
        mov    edx, dword ptr [ebx + 0x10]
        mov    dword ptr [eax + 4], edx
        add    dword ptr [ebx + 0x14], 8
        mov    esi, dword ptr [ebx + 4]
        test   byte ptr [ebx + 0x6d], 0x80
        jne    loc_1_0269
        movzx  eax, word ptr [ebx + 0x68]
        cmp    eax, 0xe
        jae    loc_1_0222
        call   sub_1_01B0
loc_1_021E:
        or     al, al
        jne    loc_1_0235
loc_1_0222:
        mov    eax, dword ptr [ebx + 0x10]
        mov    eax, dword ptr [eax + 0xc]
        mov    dword ptr [ebx + 0x10], eax
        push   ebx
        call   dword ptr [eax]
        add    esp, 4
loc_1_0231:
        pop    esi
        pop    edi
        pop    ebp
        ret    
loc_1_0235:
        dec    al
        jne    loc_1_024A
        mov    eax, dword ptr [ebx + 0x14]
        sub    eax, 0x10
        mov    dword ptr [ebx + 0x14], eax
        push   ebx
        call   dword ptr [eax]
        add    esp, 4
        jmp    loc_1_0231
loc_1_024A:
        dec    al
        je     loc_1_0231
        mov    al, 1
        mov    word ptr [ebx + 0x6a], 0x16
        cmp    word ptr [ebx + 0x68], 0xd
        jne    loc_1_0235
        mov    dword ptr [ebx + 0xa0], 0x1f
        jmp    loc_1_0235
loc_1_0269:
        mov    edi, dword ptr [ebx + 0x20]
        mov    al, byte ptr [edi + 0x30]
        cmp    dword ptr [esi + 0xa2], 0x31302e31
        jne    loc_1_0280
        call   sub_1_0284
loc_1_0280:
        xor    al, al
        jmp    loc_1_021E
sub_1_0284:
        cmp    al, 0xbe
        jne    loc_1_028E
        mov    byte ptr [edi + 0x30], 0xd4
        jmp    loc_1_02F1
loc_1_028E:
        cmp    al, 0x47
        jne    loc_1_0299
        call   sub_1_02F2
        jmp    loc_1_02F1
loc_1_0299:
        cmp    al, 0xd5
        jne    loc_1_02A4
        call   sub_1_02F2
        jmp    loc_1_02F1
loc_1_02A4:
        cmp    al, 0x1a
        jne    loc_1_02BF
        mov    al, byte ptr [edi + 0x32]
        mov    ah, al
        and    ah, 0xc0
        and    al, 0x3f
        cmp    al, 0x2a
        jne    loc_1_02B8
        mov    al, 0xf
loc_1_02B8:
        or     al, ah
        mov    byte ptr [edi + 0x32], al
        jmp    loc_1_02F1
loc_1_02BF:
        cmp    al, 0x43
        jne    loc_1_02CF
        cmp    byte ptr [edi + 0x39], 0x80
        jne    loc_1_02CF
        mov    byte ptr [edi + 0x39], 0
        jmp    loc_1_02F1
loc_1_02CF:
        cmp    al, 0x49
        jne    loc_1_02E0
        mov    al, byte ptr [edi + 0x36]
        call   sub_1_030C
        mov    byte ptr [edi + 0x36], al
        jmp    loc_1_02F1
loc_1_02E0:
        cmp    al, 0xa9
        jne    loc_1_02F1
        mov    al, byte ptr [edi + 0x3a]
        call   sub_1_030C
        mov    byte ptr [edi + 0x3a], al
        jmp    loc_1_02F1
loc_1_02F1:
        ret    
sub_1_02F2:
        push   edi
        push   ebx
        push   ecx
        mov    ecx, 6
loc_1_02FA:
        mov    al, byte ptr [edi + 0x33]
        call   sub_1_030C
        mov    byte ptr [edi + 0x33], al
        inc    edi
        loop   loc_1_02FA
        pop    ecx
        pop    ebx
        pop    edi
        ret    
sub_1_030C:
        push   ebx
        xor    ah, ah
        mov    bl, 0xa
        div    bl
        shl    al, 1
        shl    al, 1
        shl    al, 1
        shl    al, 1
        add    al, ah
        pop    ebx
        ret    
loc_1_031F:
        xor    al, al
        ret    
        db  000h, 000h
sub_1_0324:
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
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  4, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'NECATAPI'                  ; DDB_Name
        dd  080000000h                  ; DDB_Init_Order
        dd  NECATAPI_Control         ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x5c  R-X
loc_2_0000:
        push   0x24                          ; -> loc_2_0024
        VxDCall  BLOCKDEV.Service_0007h          ; cd2007001000
        add    esp, 4
        cmp    word ptr [0x50], 1            ; -> loc_2_0050
        je     loc_2_0023
        cmp    word ptr [0x50], 2            ; -> loc_2_0050
        je     loc_2_0023
        stc    
loc_2_0023:
        ret    
loc_2_0024:
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], cl      ; -> loc_1_000C
        add    byte ptr [eax], al
        and    al, 3
        add    byte ptr [eax], al
        dec    esi
        inc    ebp
        inc    ebx
        and    byte ptr [ebx + 0x44], al
        push   edx
        xor    dh, byte ptr [esi]
        xor    byte ptr [eax], ah
        and    byte ptr [eax], ah
        push   esi
        push   ebx
        inc    esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_2_0050:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x94  R-X
loc_3_0000:
        push   esi
        mov    esi, dword ptr [ebx + 0xc]
        test   dword ptr [esi + 0x20], 0x6000
        jne    loc_3_007F
        cmp    byte ptr [esi + 0x74], 1
        jne    loc_3_007F
        cmp    byte ptr [esi + 0x40], 5
        jne    loc_3_007F
        cmp    dword ptr [esi + 0x8a], 0x2043454e
        jne    loc_3_007F
        cmp    dword ptr [esi + 0x8e], 0x20202020
        jne    loc_3_007F
        cmp    dword ptr [esi + 0xa2], 0x31302e31
        jne    loc_3_007F
        inc    byte ptr [0]                  ; -> obj4:0x0000
        mov    eax, dword ptr [esi + 0x1c]
        push   esi
        push   edi
        sub    esp, 0x18
        mov    edi, esp
        mov    word ptr [edi], 5
        mov    dword ptr [edi + 0x12], eax
        mov    dword ptr [edi + 4], esi
        mov    dword ptr [edi + 8], 0x1e8    ; -> loc_1_01E8
        mov    al, byte ptr [ebx + 8]
        mov    byte ptr [edi + 0x16], al
        mov    dword ptr [edi + 0xc], 0
        xor    eax, eax
        mov    word ptr [edi + 0x10], ax
        push   edi
        call   sub_1_0324
        add    esp, 0x1c
        pop    edi
        pop    esi
loc_3_007F:
        pop    esi
        ret    
loc_3_0081:
        cmp    byte ptr [0], 0               ; -> obj4:0x0000
        jne    loc_3_0090
        mov    word ptr [ebx + 2], 0xffff
loc_3_0090:
        ret    
        db  025h, 04Eh, 045h
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 4  vsize=0x4  RW-
VxD__LDATA ENDS

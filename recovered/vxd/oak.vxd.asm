; OAK -- reconstructed 32-bit disassembly (VxD / LE format)
; 4 objects, 355 instructions recovered by recursive descent

Declare_Virtual_Device OAK, 3, 1, OAK_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 028000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x120  R-X
OAK_Control:
        sub    eax, 1
        cmp    eax, 0x1b
        jae    loc_1_000F
        jmp    dword ptr [eax*4 + 0x11]      ; -> obj1:0x0011
loc_1_000F:
        clc    
        ret    
        ; ---- dispatch table (27 entries) ----
        dd  loc_2_0000
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_2_0000
        db  000h, 000h, 000h
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  3, 1                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'OAK     '                  ; DDB_Name
        dd  028000000h                  ; DDB_Init_Order
        dd  OAK_Control              ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x168  R-X
loc_2_0000:
        push   ebx
        mov    dword ptr [0xd0], ebx         ; -> obj1:0x00D0
        mov    byte ptr [0x11d], 0           ; -> obj1:0x011D
        mov    edx, 0x3de
        in     al, dx
        ror    eax, 8
        mov    al, 9
        out    dx, al
        inc    edx
        in     al, dx
        ror    eax, 8
        xor    al, al
        out    dx, al
        jmp    loc_2_0024
loc_2_0024:
        in     al, dx
        or     al, al
        jne    loc_2_004C
        mov    al, 0x55
        out    dx, al
        jmp    loc_2_002E
loc_2_002E:
        in     al, dx
        cmp    al, 0x55
        jne    loc_2_004C
        dec    edx
        mov    al, 0
        out    dx, al
        inc    dx
        mov    byte ptr [0x11d], 0x67        ; -> obj1:0x011D
        in     al, dx
        cmp    al, 1
        jne    loc_2_004C
        mov    byte ptr [0x11d], 0x87        ; -> obj1:0x011D
loc_2_004C:
        mov    edx, 0x3de
        mov    al, 9
        out    dx, al
        rol    eax, 8
        inc    edx
        out    dx, al
        dec    edx
        rol    eax, 8
        out    dx, al
        cmp    byte ptr [0x11d], 0           ; -> obj1:0x011D
        je     loc_2_015C
        push   0
        push   0x39
        VMMCall  VMM._Allocate_Device_CB_Area    ; cd20a7000100
        add    esp, 8
        mov    dword ptr [0xd4], eax         ; -> obj1:0x00D4
        or     eax, eax
        je     loc_2_015C
        VxDCall  VDD.VDD_Get_Mini_Dispatch_Table ; cd200e000a00
        cmp    ecx, 0x31
        jb     loc_2_015C
        mov    dword ptr [edi], 0            ; -> loc_3_0000
        mov    dword ptr [edi + 0x20], 0     ; -> sub_4_0000
        mov    dword ptr [edi + 0xa8], 0x15f ; -> loc_2_015F
        mov    dword ptr [edi + 0x80], 0x1e0 ; -> loc_4_01E0
        mov    dword ptr [edi + 0x84], 0x1e0 ; -> loc_4_01E0
        mov    dword ptr [edi + 0x88], 0x219 ; -> loc_4_0219
        mov    dword ptr [edi + 0x90], 0x26e ; -> loc_4_026E
        cmp    byte ptr [0x11d], 0x87        ; -> obj1:0x011D
        je     loc_2_0117
        mov    dword ptr [edi + 0x24], 0x4b  ; -> loc_4_004B
        mov    dword ptr [edi + 0x4c], 0x198 ; -> loc_4_0198
        mov    dword ptr [edi + 0x48], 0x1bc ; -> loc_4_01BC
        mov    dword ptr [edi + 0x18], 0xd4  ; -> loc_4_00D4
        mov    dword ptr [edi + 0x1c], 0xe9  ; -> loc_4_00E9
        mov    dword ptr [edi + 0x10], 0xd4  ; -> loc_4_00D4
        mov    dword ptr [edi + 0x14], 0xe9  ; -> loc_4_00E9
        mov    dword ptr [edi + 0xc0], 0xc6  ; -> loc_4_00C6
loc_2_0117:
        mov    edx, 0x3de
        mov    ecx, 0xfe                     ; -> loc_4_00FE
        VxDCall  VDD.VDD_Takeover_VGA_Port       ; cd2013000a00
        mov    edx, 0x3df
        mov    ecx, 0xfe                     ; -> loc_4_00FE
        VxDCall  VDD.VDD_Takeover_VGA_Port       ; cd2013000a00
        VxDCall  VDD.VDD_Get_Special_VM_IDs      ; cd2011000a00
        mov    dword ptr [0xd8], esi         ; -> obj1:0x00D8
        mov    dword ptr [0xdc], edi         ; -> obj1:0x00DC
        mov    ebx, 0xe0                     ; -> obj1:0x00E0
        sub    ebx, dword ptr [0xd4]         ; -> obj1:0x00D4
        call   sub_4_0000
        clc    
        jmp    loc_2_015D
loc_2_015C:
        stc    
loc_2_015D:
        pop    ebx
        ret    
loc_2_015F:
        movzx  eax, byte ptr [0x11d]         ; -> obj1:0x011D
        ret    
        db  000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x13  R-X
loc_3_0000:
        mov    eax, dword ptr [ebp + 0x18]
        mov    dword ptr [0x119], eax        ; -> obj1:0x0119
        movzx  eax, word ptr [ebp + 0x14]
        VxDCall  VDD.VDD_Register_Extra_Screen_Selector ; cd2012000a00
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0x276  R-X
sub_4_0000:
        push   edx
        push   edi
        mov    edx, 0x3de
        in     al, dx
        ror    eax, 8
        mov    edi, ebx
        add    edi, dword ptr [0xd4]         ; -> obj1:0x00D4
        xor    ecx, ecx
loc_4_0015:
        mov    al, cl
        out    dx, al
        inc    edx
        in     al, dx
        mov    byte ptr [ecx + edi], al
        dec    edx
        inc    cl
        cmp    cl, 0x37
        jb     loc_4_0015
        rol    eax, 8
        out    dx, al
        mov    byte ptr [edi + 0x37], al
        cmp    byte ptr [0x11d], 0x87        ; -> obj1:0x011D
        je     loc_4_0048
        mov    dl, 0xc4
        in     al, dx
        ror    eax, 8
        mov    al, 4
        out    dx, al
        inc    edx
        in     al, dx
        mov    byte ptr [edi + 0x38], al
        dec    edx
        rol    eax, 8
        out    dx, al
loc_4_0048:
        pop    edi
        pop    edx
        ret    
loc_4_004B:
        cmp    byte ptr [0x11f], 0           ; -> obj1:0x011F
        jne    loc_4_00C5
        push   edx
        mov    edx, 0x3c4
        in     al, dx
        ror    eax, 8
        cmp    ecx, dword ptr [0xd8]         ; -> obj1:0x00D8
        jne    loc_4_006A
        mov    ah, 6
        jmp    loc_4_008A
loc_4_006A:
        cmp    ecx, dword ptr [0xdc]         ; -> obj1:0x00DC
        jne    loc_4_007D
        mov    ecx, 0xe0                     ; -> obj1:0x00E0
        sub    ecx, dword ptr [0xd4]         ; -> obj1:0x00D4
loc_4_007D:
        add    ecx, dword ptr [0xd4]         ; -> obj1:0x00D4
        mov    ah, byte ptr [ecx + 0x38]
        or     ah, ah
        je     loc_4_00C0
loc_4_008A:
        cmp    esi, dword ptr [0xd8]         ; -> obj1:0x00D8
        jne    loc_4_0096
        mov    al, 6
        jmp    loc_4_00B6
loc_4_0096:
        cmp    esi, dword ptr [0xdc]         ; -> obj1:0x00DC
        jne    loc_4_00A9
        mov    esi, 0xe0                     ; -> obj1:0x00E0
        sub    esi, dword ptr [0xd4]         ; -> obj1:0x00D4
loc_4_00A9:
        add    esi, dword ptr [0xd4]         ; -> obj1:0x00D4
        mov    al, byte ptr [esi + 0x38]
        or     al, al
        je     loc_4_00C0
loc_4_00B6:
        and    ax, 0x8f7
        or     ah, al
        mov    al, 4
        out    dx, ax
loc_4_00C0:
        rol    eax, 8
        out    dx, al
        pop    edx
loc_4_00C5:
        ret    
loc_4_00C6:
        push   edx
        mov    edx, 0x3f1
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        pop    edx
        ret    
loc_4_00D4:
        push   edx
        mov    edx, 0x3f1
        VMMCall  VMM.Disable_Global_Trapping     ; cd2099000100
        mov    byte ptr [0x11f], 0xff        ; -> obj1:0x011F
        pop    edx
        ret    
loc_4_00E9:
        push   edx
        mov    edx, 0x3f1
        VMMCall  VMM.Enable_Global_Trapping      ; cd2097000100
        mov    byte ptr [0x11f], 0           ; -> obj1:0x011F
        pop    edx
        ret    
loc_4_00FE:
        push   edi
        push   esi
        VxDCall  VDD.VDD_Get_VM_Info             ; cd2010000a00
        cmp    edi, dword ptr [0xd0]         ; -> obj1:0x00D0
        jne    loc_4_0112
        cmp    ebx, edi
        jne    loc_4_0126
loc_4_0112:
        cmp    ecx, 4
        je     loc_4_0123
        cmp    ecx, 0xc
        je     loc_4_011F
        in     al, dx
        jmp    loc_4_0195
loc_4_011F:
        out    dx, ax
        jmp    loc_4_0195
loc_4_0123:
        out    dx, al
        jmp    loc_4_0195
loc_4_0126:
        mov    edi, ebx
        add    edi, dword ptr [0xd4]         ; -> obj1:0x00D4
        cmp    ecx, 4
        je     loc_4_017D
        cmp    ecx, 0xc
        je     loc_4_016E
        cmp    edx, 0x3de
        jne    loc_4_0145
        mov    al, byte ptr [edi + 0x37]
        jmp    loc_4_0195
loc_4_0145:
        movzx  eax, byte ptr [edi + 0x37]
        and    al, 0x3f
        or     al, al
        je     loc_4_0163
        cmp    al, 7
        je     loc_4_0163
        cmp    al, 8
        je     loc_4_0163
        cmp    al, 0xf
        je     loc_4_0163
        cmp    al, 0x10
        je     loc_4_0163
        cmp    al, 0x12
        jne    loc_4_0169
loc_4_0163:
        dec    edx
        out    dx, al
        inc    edx
        in     al, dx
        jmp    loc_4_0195
loc_4_0169:
        mov    al, byte ptr [eax + edi]
        jmp    loc_4_0195
loc_4_016E:
        mov    byte ptr [edi + 0x37], al
        and    al, 0x3f
        mov    cl, ah
        movzx  eax, al
        mov    byte ptr [eax + edi], cl
        jmp    loc_4_0195
loc_4_017D:
        cmp    edx, 0x3de
        jne    loc_4_018A
        mov    byte ptr [edi + 0x37], al
        jmp    loc_4_0195
loc_4_018A:
        mov    cl, al
        movzx  eax, byte ptr [edi + 0x37]
        and    al, 0x3f
        mov    byte ptr [eax + edi], cl
loc_4_0195:
        pop    esi
        pop    edi
        ret    
loc_4_0198:
        pushf  
        cmp    cl, 4
        jne    loc_4_01B9
        mov    edi, ebx
        add    edi, dword ptr [0xd4]         ; -> obj1:0x00D4
        mov    byte ptr [edi + 0x38], al
        VxDCall  VDD.VDD_Get_VM_Info             ; cd2010000a00
        cmp    ebx, edi
        je     loc_4_01B9
        popf   
        stc    
        jmp    loc_4_01BB
loc_4_01B9:
        popf   
loc_4_01BB:
        ret    
loc_4_01BC:
        pushf  
        cmp    cl, 4
        jne    loc_4_01DD
        VxDCall  VDD.VDD_Get_VM_Info             ; cd2010000a00
        cmp    ebx, edi
        je     loc_4_01DD
        mov    edi, ebx
        add    edi, dword ptr [0xd4]         ; -> obj1:0x00D4
        mov    al, byte ptr [edi + 0x38]
        popf   
        stc    
        jmp    loc_4_01DF
loc_4_01DD:
        popf   
loc_4_01DF:
        ret    
loc_4_01E0:
        mov    edx, 0x3de
        in     al, dx
        ror    eax, 8
        cmp    byte ptr [0x11d], 0x87        ; -> obj1:0x011D
        je     loc_4_01FC
        mov    al, 0x11
        out    dx, al
        inc    dl
        in     al, dx
        xor    ah, ah
        jmp    loc_4_020B
loc_4_01FC:
        mov    al, 0x23
        out    dx, al
        inc    edx
        in     al, dx
        mov    ah, al
        dec    dl
        mov    al, 0x25
        out    dx, al
        inc    dl
        in     al, dx
loc_4_020B:
        dec    dl
        rol    eax, 8
        out    dx, al
        ror    eax, 8
        movzx  edx, ax
        stc    
        ret    
loc_4_0219:
        push   eax
        push   edx
        cmp    byte ptr [0x11d], 0x87        ; -> obj1:0x011D
        je     loc_4_0247
        test   dl, 0xf0
        jne    loc_4_0230
        mov    dh, dl
        shl    dh, 4
        or     dl, dh
loc_4_0230:
        mov    ah, dl
        mov    edx, 0x3de
        in     al, dx
        ror    eax, 8
        mov    ah, al
        mov    al, 0x11
        out    dx, ax
        rol    eax, 8
        out    dx, al
        jmp    loc_4_026A
loc_4_0247:
        push   ecx
        or     dh, dh
        jne    loc_4_024E
        mov    dh, dl
loc_4_024E:
        mov    ecx, edx
        mov    edx, 0x3de
        in     al, dx
        ror    eax, 8
        mov    al, 0x23
        mov    ah, ch
        out    dx, ax
        mov    al, 0x24
        mov    ah, cl
        out    dx, ax
        rol    eax, 8
        out    dx, al
        pop    ecx
loc_4_026A:
        pop    edx
        pop    eax
        stc    
        ret    
loc_4_026E:
        mov    ecx, dword ptr [0x119]        ; -> obj1:0x0119
        stc    
        ret    
VxD__LTEXT ENDS

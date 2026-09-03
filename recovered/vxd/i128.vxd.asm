; I128 -- reconstructed 32-bit disassembly (VxD / LE format)
; 4 objects, 128 instructions recovered by recursive descent

Declare_Virtual_Device I128, 3, 1, I128_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 028000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x190  R-X
I128_Control:
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
        dd  sub_4_0000
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
sub_1_007D:
        mov    esi, dword ptr [0x182]        ; -> obj1:0x0182
        mov    eax, edx
        cmp    al, 1
        jne    loc_1_009C
        mov    byte ptr [esi + 0x18], 0x2c
        mov    byte ptr [esi + 0x1c], 2
        mov    al, 0x2c
        add    al, dl
        mov    byte ptr [esi + 0x18], al
        mov    byte ptr [esi + 0x1c], 2
loc_1_009C:
        mov    byte ptr [esi + 0x18], 0x2c
        mov    byte ptr [esi + 0x1c], 0
        mov    al, 0x2c
        add    al, dl
        mov    byte ptr [esi + 0x18], al
        mov    byte ptr [esi + 0x1c], ch
        mov    eax, ecx
        shr    eax, 0x10
        mov    byte ptr [esi + 0x1c], al
        mov    byte ptr [esi + 0x1c], cl
        ret    
sub_1_00BA:
        mov    edx, dword ptr [0x186]        ; -> obj1:0x0186
        add    dx, 0x1c
        in     eax, dx
        push   eax
        or     al, 2
        out    dx, eax
        mov    cx, 0xa
loc_1_00CD:
        in     eax, dx
        loop   loc_1_00CD
        pop    eax
        out    dx, eax
        ret    
sub_1_00D3:
        mov    ecx, 0x9c40
loc_1_00D8:
        mov    edx, dword ptr [0x186]        ; -> obj1:0x0186
        in     ax, dx
        loop   loc_1_00D8
        ret    
        db  000h
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  3, 1                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'I128    '                  ; DDB_Name
        dd  028000000h                  ; DDB_Init_Order
        dd  I128_Control             ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x2c  R-X
loc_2_0000:
        mov    dword ptr [0x134], ebx        ; -> obj1:0x0134
        VxDCall  VDD.VDD_Get_Mini_Dispatch_Table ; cd200e000a00
        cmp    ecx, 0x31
        jg     loc_2_0028
        mov    dword ptr [edi], 0            ; -> loc_3_0000
        mov    dword ptr [edi + 0x10], 0xcc  ; -> loc_4_00CC
        mov    dword ptr [edi + 0x68], 0x48  ; -> loc_3_0048
        clc    
        jmp    loc_2_0029
loc_2_0028:
        stc    
loc_2_0029:
        ret    
        db  000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x4e  R-X
loc_3_0000:
        mov    eax, dword ptr [ebp + 0x18]
        mov    dword ptr [0x186], eax        ; -> obj1:0x0186
        mov    eax, dword ptr [ebp + 0x14]
        mov    dword ptr [0x182], eax        ; -> obj1:0x0182
        mov    eax, dword ptr [ebp + 4]
        mov    dword ptr [0x18a], eax        ; -> obj1:0x018A
        mov    eax, 0x138                    ; -> obj1:0x0138
        mov    ecx, 0x4a
        mov    word ptr [eax], cx
        VxDCall  VDD.VDD_Get_DISPLAYINFO         ; cd2014000a00
        mov    word ptr [ebp + 0x10], 0
        mov    word ptr [ebp + 0x18], 0
        mov    di, word ptr [0x158]          ; -> obj1:0x0158
        or     di, di
        je     loc_3_0047
        mov    word ptr [ebp + 0x18], di
loc_3_0047:
        ret    
loc_3_0048:
        call   sub_4_0000
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0xd2  R-X
sub_4_0000:
        mov    eax, dword ptr [0x18a]        ; -> obj1:0x018A
        mov    esi, dword ptr [0x182]        ; -> obj1:0x0182
        cmp    al, 4
        jne    loc_4_0068
        mov    byte ptr [esi + 0x18], 0x18
        mov    byte ptr [esi + 0x1c], 0x80
        mov    byte ptr [esi + 0x18], 0x19
        mov    byte ptr [esi + 0x1c], 0x98
        mov    byte ptr [esi + 0x18], 0x1a
        mov    byte ptr [esi + 0x1c], 0
        mov    byte ptr [esi + 0x18], 0x1b
        mov    byte ptr [esi + 0x1c], 0x3e
        mov    byte ptr [esi + 0x18], 0x1e
        mov    byte ptr [esi + 0x1c], 0
        mov    byte ptr [esi + 0x18], 0x1d
        mov    byte ptr [esi + 0x1c], 0
        mov    byte ptr [esi + 0x18], 0x29
        mov    byte ptr [esi + 0x1c], 9
        mov    byte ptr [esi + 0x18], 0x38
        mov    byte ptr [esi + 0x1c], 0x10
        mov    byte ptr [esi + 0x18], 6
        mov    byte ptr [esi + 0x1c], 0
        mov    ecx, 0x171880
        mov    edx, 2
        call   sub_1_007D
        jmp    loc_4_00C0
loc_4_0068:
        mov    byte ptr [esi + 0x14], 0
        mov    byte ptr [esi + 0x1c], 1
        mov    byte ptr [esi + 0x10], 0x30
        mov    byte ptr [esi + 0x18], 0
        mov    byte ptr [esi + 0x10], 0x70
        mov    byte ptr [esi + 0x18], 0
        mov    byte ptr [esi + 0x18], 0
        mov    byte ptr [esi + 0x18], 0
        mov    byte ptr [esi + 0x18], 0
        mov    byte ptr [esi + 0x10], 2
        mov    byte ptr [esi + 0x18], 0
        mov    byte ptr [esi + 0x18], 0
        mov    byte ptr [esi + 0x18], 0
        mov    byte ptr [esi + 0x18], 0
        mov    byte ptr [esi + 0x18], 2
        mov    byte ptr [esi + 0x18], 0
        mov    byte ptr [esi + 0x18], 1
        mov    byte ptr [esi + 0x10], 0x10
        mov    byte ptr [esi + 0x18], 0
        mov    byte ptr [esi + 0x10], 0x15
        mov    byte ptr [esi + 0x18], 8
        mov    byte ptr [esi + 0x18], 0x41
loc_4_00C0:
        call   sub_1_00D3
        call   sub_1_00BA
        clc    
        ret    
loc_4_00CC:
        call   0
        ret    
VxD__LTEXT ENDS

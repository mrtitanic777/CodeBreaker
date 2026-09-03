; QUARTZ -- reconstructed 32-bit disassembly (VxD / LE format)
; 3 objects, 85 instructions recovered by recursive descent

Declare_Virtual_Device QUARTZ, 1, 0, QUARTZ_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 080000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x70  R-X
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  1, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'QUARTZ  '                  ; DDB_Name
        dd  080000000h                  ; DDB_Init_Order
        dd  QUARTZ_Control           ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
QUARTZ_Control:
        db  083h
        db  0F8h, 023h, 00Fh, 084h
        dd  loc_2_0004               ; fixup
        db  083h, 0F8h, 01Bh, 00Fh, 084h
        dd  loc_3_0000               ; fixup
        db  083h, 0F8h, 01Ch, 00Fh, 084h
        dd  loc_2_0000               ; fixup
        db  0F8h, 0C3h, 0CCh, 0CCh, 0CCh
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0xcd  R-X
loc_2_0000:
        xor    eax, eax
        clc    
        ret    
loc_2_0004:
        jecxz  loc_2_0016
        dec    ecx
        je     loc_2_0039
        dec    ecx
        je     loc_2_00A6
        mov    eax, 1
        ret    
loc_2_0016:
        push   edi
        push   esi
        cmp    dword ptr [esi + 0x1c], 4
        jb     loc_2_0034
        mov    edi, dword ptr [esi + 0x20]
        mov    esi, dword ptr [esi + 0x18]
        mov    dword ptr [esi], 0x100
        or     edi, edi
        je     loc_2_0034
        mov    dword ptr [edi], 4
loc_2_0034:
        pop    esi
        pop    edi
        xor    eax, eax
        ret    
loc_2_0039:
        push   edi
        push   esi
        cmp    dword ptr [esi + 0x1c], 4
        jb     loc_2_00A1
        mov    edi, dword ptr [esi + 0x20]
        mov    dword ptr [edi], 4
        mov    eax, dword ptr [esi + 0x10]
        mov    edi, dword ptr [esi + 0x10]
        mov    eax, dword ptr [edi]
        mov    edi, eax
        add    eax, eax
        push   0
        push   eax
        push   0x80000400
        VMMCall  VMM._PageReserve                ; cd201d010100
        add    esp, 0xc
        mov    esi, dword ptr [esi + 0x18]
        mov    dword ptr [esi], eax
        cmp    eax, -1
        je     loc_2_00A1
        shr    eax, 0xc
        mov    esi, eax
        push   0x60000
        push   0
        push   1
        push   edi
        push   eax
        VMMCall  VMM._PageCommit                 ; cd201e010100
        add    esp, 0x14
        VMMCall  VMM._GetCurrentContext          ; cd2057010100
        mov    ebx, esi
        add    ebx, edi
        push   edi
        push   ebx
        push   eax
        push   esi
        VMMCall  VMM._PageAttach                 ; cd2025010100
        add    esp, 0x10
loc_2_00A1:
        pop    esi
        pop    edi
        xor    eax, eax
        ret    
loc_2_00A6:
        push   edi
        push   esi
        cmp    dword ptr [esi + 0x14], 4
        jb     loc_2_00C8
        mov    edi, dword ptr [esi + 0x20]
        mov    dword ptr [edi], 0
        mov    eax, dword ptr [esi + 0x10]
        mov    eax, dword ptr [eax]
        push   0
        push   eax
        VMMCall  VMM._PageFree                   ; cd2055000100
        add    esp, 8
loc_2_00C8:
        pop    esi
        pop    edi
        xor    eax, eax
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x4  R-X
loc_3_0000:
        xor    eax, eax
        clc    
        ret    
VxD__LTEXT ENDS

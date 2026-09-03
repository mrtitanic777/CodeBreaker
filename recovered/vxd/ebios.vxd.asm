; EBIOS -- reconstructed 32-bit disassembly (VxD / LE format)
; 6 objects, 122 instructions recovered by recursive descent

Declare_Virtual_Device EBIOS, 1, 0, EBIOS_Control, 00012h, Undefined_Init_Order
;   DDK version 0400h   init_order 026000000h   flags 00000h
;   service table: obj1:0x20, 2 services

VxD__LTEXT SEGMENT   ; object 1  vsize=0x78  R-X
EBIOS_Control:
        cmp    eax, 1
        je     loc_2_0000
        cmp    eax, 3
        je     loc_6_0000
        cmp    eax, 7
        je     loc_6_0000
        clc    
        ret    
        db  000h, 000h, 000h
        dd  EBIOS_Get_Version        ; fixup
        dd  EBIOS_Get_Unused_Mem     ; fixup
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00012h                      ; DDB_Req_Device_Number
        db  1, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'EBIOS   '                  ; DDB_Name
        dd  026000000h                  ; DDB_Init_Order
        dd  EBIOS_Control            ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  obj1:0x0020              ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x11c  R-X
loc_2_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x24
        mov    edi, esp
        push   0
        push   edi
        push   0
        VMMCall  VMM._Get_Device_V86_Pages_Array ; cd2074000100
        add    esp, 0xc
        mov    edx, dword ptr [0x54]         ; -> obj1:0x0054
        movzx  edx, dx
        mov    ecx, 1
        VMMCall  VMM._GetFirstV86Page            ; cd206b000100
        xchg   edx, eax
        cmp    eax, edx
        ja     loc_2_004F
        mov    dword ptr [8], ecx            ; -> obj3:0x0008
        inc    byte ptr [0xc]                ; -> obj3:0x000C
        add    eax, ecx
        dec    eax
        cmp    eax, edx
        jbe    loc_2_00D6
        sub    eax, edx
        mov    ecx, eax
        mov    eax, edx
        inc    eax
        jmp    loc_2_00AE
loc_2_004F:
        cmp    eax, 0xa0
        jb     loc_2_008A
        mov    dword ptr [8], ecx            ; -> obj3:0x0008
        push   eax
        VxDCall  V86MMGR.V86MMGR_Get_Version     ; cd2000000600
        cmp    eax, 0x30a
        jb     loc_2_00AD
        pop    ecx
        xor    ebx, ebx
        VxDCall  V86MMGR.V86MMGR_GetPgStatus     ; cd2010000600
        xchg   ecx, eax
        test   ecx, 0x1000
        mov    ecx, dword ptr [8]            ; -> obj3:0x0008
        je     loc_2_00AE
        inc    byte ptr [0xc]                ; -> obj3:0x000C
        jmp    loc_2_00D6
loc_2_008A:
        mov    edx, eax
        VMMCall  VMM._GetLastV86Page             ; cd20d5000100
        xchg   edx, eax
        cmp    eax, edx
        jbe    loc_2_0111
        cmp    eax, 0x96
        jb     loc_2_00A5
        mov    ecx, 0xa0
        sub    ecx, eax
loc_2_00A5:
        mov    dword ptr [8], ecx            ; -> obj3:0x0008
        jmp    loc_2_00AE
loc_2_00AD:
        pop    eax
loc_2_00AE:
        bt     dword ptr [edi], eax
        jb     loc_2_0111
        inc    eax
        loop   loc_2_00AE
        mov    eax, dword ptr [0x54]         ; -> obj1:0x0054
        movzx  eax, ax
        push   0
        push   0
        push   dword ptr [8]                 ; -> obj3:0x0008
        push   eax
        VMMCall  VMM._Assign_Device_V86_Pages    ; cd2072000100
        add    esp, 0x10
        or     eax, eax
        je     loc_2_0111
loc_2_00D6:
        mov    edx, dword ptr [0x54]         ; -> obj1:0x0054
        VMMCall  VMM._GetFirstV86Page            ; cd206b000100
        cmp    dx, ax
        jb     loc_2_010C
        cmp    dx, 0xa0
        jae    loc_2_010C
        test   edx, 0xffff0000
        je     loc_2_010C
        mov    eax, edx
        shr    eax, 0x10
        movzx  edx, dx
        shl    edx, 8
        mov    dword ptr [0], edx            ; -> obj3:0x0000
        mov    dword ptr [4], eax            ; -> obj3:0x0004
loc_2_010C:
        mov    esp, ebp
        pop    ebp
        clc    
        ret    
loc_2_0111:
        pushal 
        xor    esi, esi
        VMMCall  VMM.Fatal_Error_Handler         ; cd20be000100
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 3  vsize=0xd  RW-
        db  0F7h, 0C3h, 001h, 000h, 00Fh, 085h, 09Bh, 000h, 0F9h, 0B4h, 0C0h, 0CDh, 015h
VxD__LDATA ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0xc0  R-X
        db  0F7h, 0C3h, 001h, 000h, 00Fh, 085h, 09Bh, 000h, 0F9h, 0B4h, 0C0h, 0CDh, 015h, 072h, 05Fh, 00Ah
        db  0E4h, 075h, 05Bh, 026h, 0F6h, 047h, 005h, 004h, 074h, 054h, 033h, 0C0h, 08Eh, 0C0h, 0B4h, 0C1h
        db  0CDh, 015h, 072h, 04Ah, 08Ch, 0C0h, 00Bh, 0C0h, 074h, 044h, 050h, 0CDh, 012h, 0C1h, 0E0h, 006h
        db  05Ah, 03Bh, 0D0h, 08Bh, 0C2h, 074h, 002h, 033h, 0D2h, 0C1h, 0E8h, 008h, 081h, 0E2h, 0FFh, 000h
        db  066h, 0C1h, 0E2h, 010h, 08Bh, 0D0h, 0BBh, 0AEh, 000h, 0B9h, 001h, 000h, 03Dh, 096h, 000h, 072h
        db  014h, 03Dh, 0A0h, 000h, 073h, 00Fh, 0B9h, 0A0h, 000h, 02Bh, 0C8h, 053h, 089h, 007h, 043h, 043h
        db  0E2h, 0FAh, 05Bh, 0EBh, 002h, 033h, 0DBh, 033h, 0F6h, 0B8h, 000h, 000h, 0EBh, 03Fh, 0B8h, 000h
        db  051h, 0CDh, 021h, 08Eh, 0C3h, 026h, 0A1h, 002h, 000h, 03Dh, 000h, 0A0h, 073h, 025h, 03Dh, 000h
        db  096h, 072h, 020h, 08Bh, 0D0h, 081h, 0E2h, 0FFh, 000h, 081h, 0FAh, 0FFh, 000h, 075h, 0AAh, 006h
        db  08Eh, 0C0h, 026h, 080h, 03Eh, 000h, 000h, 04Dh, 075h, 006h, 026h, 083h, 03Eh, 001h, 000h, 008h
        db  007h, 075h, 096h, 033h, 0DBh, 033h, 0F6h, 066h, 033h, 0D2h, 0B8h, 001h, 080h, 0C3h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 5  vsize=0x24  R-X
EBIOS_Get_Version:
        mov    eax, 0x200
        mov    edx, dword ptr [0x54]         ; -> obj1:0x0054
        movzx  edx, dx
        mov    ecx, dword ptr [8]            ; -> obj3:0x0008
        clc    
        ret    
EBIOS_Get_Unused_Mem:
        xor    eax, eax
        mov    eax, dword ptr [0]            ; -> obj3:0x0000
        mov    ecx, dword ptr [4]            ; -> obj3:0x0004
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 6  vsize=0x27  R-X
loc_6_0000:
        cmp    byte ptr [0xc], 0             ; -> obj3:0x000C
        jne    loc_6_0025
        mov    eax, dword ptr [0x54]         ; -> obj1:0x0054
        movzx  eax, ax
        push   0
        push   dword ptr [8]                 ; -> obj3:0x0008
        push   eax
        push   ebx
        push   eax
        VMMCall  VMM._PhysIntoV86                ; cd205e000100
        add    esp, 0x14
loc_6_0025:
        clc    
        ret    
VxD__LTEXT ENDS

; VXDLDR -- reconstructed 32-bit disassembly (VxD / LE format)
; 7 objects, 3884 instructions recovered by recursive descent

Declare_Virtual_Device VXDLDR, 3, 0, VXDLDR_Control, 00027h, Undefined_Init_Order
;   DDK version 0400h   init_order 016000000h   flags 00000h
;   service table: obj1:0x48, 18 services

VxD__LTEXT SEGMENT   ; object 1  vsize=0xf0  R-X
VXDLDR_Control:
        cmp    eax, 1
        je     loc_3_0000
        cmp    eax, 2
        je     loc_3_0044
        cmp    eax, 0x23
        je     loc_7_026D
        cmp    eax, 3
        je     loc_7_0000
        test   dword ptr [0x2a], 0x400000    ; -> obj1:0x002A
        je     loc_1_0039
        cmp    eax, 0x14
        je     loc_6_0000
loc_1_0039:
        clc    
        ret    
VXDLDR_GetVersion:
        mov    eax, 0x100
        clc    
        ret    
VXDLDR_GetDeviceList:
        mov    eax, dword ptr [0xe0]         ; -> obj1:0x00E0
        ret    
        dd  VXDLDR_GetVersion        ; fixup
        dd  VXDLDR_LoadDevice        ; fixup
        dd  VXDLDR_UnloadDevice      ; fixup
        dd  VXDLDR_DevInitSucceeded  ; fixup
        dd  VXDLDR_DevInitFailed     ; fixup
        dd  VXDLDR_GetDeviceList     ; fixup
        dd  VXDLDR_UnloadMe          ; fixup
        dd  _PELDR_LoadModule        ; fixup
        dd  _PELDR_GetModuleHandle   ; fixup
        dd  _PELDR_GetModuleUsage    ; fixup
        dd  _PELDR_GetEntryPoint     ; fixup
        dd  _PELDR_GetProcAddress    ; fixup
        dd  _PELDR_AddExportTable    ; fixup
        dd  _PELDR_RemoveExportTable ; fixup
        dd  _PELDR_FreeModule        ; fixup
        dd  VXDLDR_Notify            ; fixup
        dd  _PELDR_InitCompleted     ; fixup
        dd  _PELDR_LoadModuleEx      ; fixup
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00027h                      ; DDB_Req_Device_Number
        db  3, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'VXDLDR  '                  ; DDB_Name
        dd  016000000h                  ; DDB_Init_Order
        dd  VXDLDR_Control           ; DDB_Control_Proc
        dd  VXDLDR_PM_API            ; DDB_V86_API_Proc
        dd  VXDLDR_PM_API            ; DDB_PM_API_Proc
        dd  obj1:0x0048              ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x1f2  R-X
loc_2_0000:
        adc    dword ptr [eax], eax
        add    byte ptr [eax], al
loc_2_0004:
        xor    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
loc_2_000A:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        adc    al, 0
        das    
        add    byte ptr [eax], al
        push   0xa7007e00
        add    al, al
        add    cl, dl
        add    dh, ch
        add    byte ptr [esi], cl
        add    dword ptr [0x5a013f01], esp
        add    dword ptr [ecx + eax - 0x72], esi
        add    dword ptr [eax + 0x4f01c201], ebp
        jne    0xa7
        and    byte ptr [edi + 0x66], ch
        and    byte ptr [ebp + 0x65], ch
        insd   dword ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        jb     loc_2_00B6
        sub    al, 0x31
        xor    ebp, dword ptr [ecx + esi]
        xor    byte ptr [eax], al
        inc    ecx
        je     0xbb
        insd   dword ptr es:[edi], dx
        jo     loc_2_00BF
        and    byte ptr fs:[edi + ebp*2 + 0x20], dh
        jb     0xb8
        outsb  dx, byte ptr gs:[esi]
        je     0xbc
        jb     loc_2_0079
        inc    esp
        dec    edi
        push   ebx
        or     eax, 0x6946000a
        insb   byte ptr es:[edi], dx
        and    byte ptr gs:[esi + 0x6f], ch
        je     loc_2_0088
        outsw  dx, word ptr [esi]
        jne    0xda
        or     eax, 0x7245000a
        jb     loc_2_00E3
        jb     0x96
        jb     0xdd
        popal  
loc_2_0079:
        imul   ebp, dword ptr fs:[esi + 0x67], 0x6c696620
        and    byte ptr gs:[0x736f7020], ch
loc_2_0088:
        jae    0xf3
        bound  ebp, qword ptr [ecx + edi*2 + 0x20]
        arpl   word ptr [edi + 0x72], bp
        jb     loc_2_0108
        jo     0x109
        or     eax, 0x7544000a
        jo     loc_2_0108
        imul   esp, dword ptr [ebx + 0x61], 0x64206574
        jbe    0x10f
        arpl   word ptr [ebp + 0x20], sp
        dec    ecx
        inc    esp
        or     eax, 0x6f4e000a
        je     loc_2_00D2
        popal  
        and    byte ptr [esi + 0x61], dh
loc_2_00B6:
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [eax + 0x64], 0x63697665
loc_2_00BF:
        and    byte ptr gs:[eax], ah
        sub    eax, 0x736f7020
        jae    0x132
        bound  ebp, qword ptr [ecx + edi*2 + 0x20]
        arpl   word ptr [edi + 0x72], bp
        jb     0x147
loc_2_00D2:
        jo     0x148
        or     eax, 0x6544000a
        jbe    0x144
        arpl   word ptr [ebp + 0x20], sp
        jb     0x145
        jne    0x156
loc_2_00E3:
        and    byte ptr fs:[edi + ebp*2 + 0x20], dh
        insb   byte ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        popal  
        or     eax, 0x6f4e000a
        and    byte ptr [ebx + 0x75], dh
        arpl   word ptr [eax + 0x20], bp
        jbe    loc_2_0165
        arpl   word ptr [ebp + 0xd], sp
        or     al, byte ptr [eax]
        inc    esp
        jbe    0x16e
        arpl   word ptr [ebp + 0x20], sp
loc_2_0108:
        arpl   word ptr [ecx + 0x6e], sp
        and    byte ptr [esi + 0x6f], ch
        je     0x130
        bound  esp, qword ptr [ebp + 0x20]
        jne    0x183
        insb   byte ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        popal  
        or     eax, 0x6e55000a
        popal  
        bound  ebp, qword ptr [ebp + 0x20]
        je     loc_2_0196
        and    byte ptr [ecx + 0x6c], ah
        insb   byte ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        arpl   word ptr [ecx + 0x74], sp
        and    byte ptr gs:[esi + 0x38], dl
        and    byte ptr ss:[edx + 0x75], ah
        jb     0x149
        or     al, byte ptr [eax]
        dec    ecx
        outsb  dx, byte ptr [esi]
        jbe    0x1a3
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [eax + 0x41], 0x66204950
        jne    loc_2_01BB
        arpl   word ptr [ecx + ebp*2 + 0x6f], si
        outsb  dx, byte ptr [esi]
        or     eax, 0x7944000a
        outsb  dx, byte ptr [esi]
        popal  
        insd   dword ptr es:[edi], dx
        imul   esp, dword ptr [ebx + 0x20], 0x64616f6c
        and    byte ptr [ebp + 0x76], ah
loc_2_0165:
        imul   esp, dword ptr [ebx + 0x65], 0x73252020
        or     eax, 0x7944000a
        outsb  dx, byte ptr [esi]
        popal  
        insd   dword ptr es:[edi], dx
        imul   esp, dword ptr [ebx + 0x20], 0x64616f6c
        and    byte ptr [esi + 0x61], ah
        imul   ebp, dword ptr [ebp + 0x64], 0x73252020
        and    byte ptr [edx], bh
        and    byte ptr [eax], al
        inc    esp
        jns    0x1fb
        popal  
        insd   dword ptr es:[edi], dx
        imul   esp, dword ptr [ebx + 0x20], 0x64616f6c
loc_2_0196:
        and    byte ptr [ebx + 0x75], dh
        arpl   word ptr [ebx + 0x65], sp
        jae    0x211
        and    byte ptr [0xa0d73], ah
        inc    esp
        jns    0x215
        popal  
        insd   dword ptr es:[edi], dx
        imul   esp, dword ptr [ebx + 0x20], 0x74696e69
        and    byte ptr [ebp + 0x76], ah
        imul   esp, dword ptr [ebx + 0x65], 0x73252020
loc_2_01BB:
        or     eax, 0x7944000a
        outsb  dx, byte ptr [esi]
        popal  
        insd   dword ptr es:[edi], dx
        imul   esp, dword ptr [ebx + 0x20], 0x74696e69
        and    byte ptr [esi + 0x61], ah
        imul   ebp, dword ptr [ebp + 0x64], 0x73252020
        or     eax, 0x7944000a
        outsb  dx, byte ptr [esi]
        popal  
        insd   dword ptr es:[edi], dx
        imul   esp, dword ptr [ebx + 0x20], 0x74696e69
        and    byte ptr [ebx + 0x75], dh
        arpl   word ptr [ebx + 0x65], sp
        jae    0x25f
        and    byte ptr [0xa0d73], ah
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x19c  R-X
loc_3_0000:
        xor    ebp, ebp
        mov    edi, 0xe0                     ; -> obj1:0x00E0
        jmp    loc_3_00FF
loc_3_000C:
        cmp    ebp, 0x14
        jbe    loc_3_0035
        xor    ebp, ebp
        push   0x10f                         ; -> obj6:0x010F
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        VMMCall  VMM.In_Debug_Chr                ; cd20c4000100
        push   0x10c                         ; -> obj6:0x010C
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        je     loc_3_0109
loc_3_0035:
        mov    ax, word ptr [edi + 9]
        mov    esi, dword ptr [edi + 5]
        mov    dl, byte ptr [edi + 4]
        push   0x136                         ; -> obj6:0x0136
loc_3_0044:
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        push   dword ptr [edi + 0xb]
sub_3_004D:
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        push   0x13d                         ; -> obj6:0x013D
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        test   esi, esi
        jns    loc_3_006D
        push   0x161                         ; -> obj6:0x0161
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
loc_3_006D:
        jle    loc_3_007A
        push   0x16e                         ; -> obj6:0x016E
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
loc_3_007A:
        jne    loc_3_0087
        push   0x17c                         ; -> obj6:0x017C
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
loc_3_0087:
        inc    ebp
        cmp    dword ptr [edi + 0xf], 0x444c5658
        je     sub_3_009E
        push   0x187                         ; -> obj6:0x0187
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        jmp    loc_3_00FF
sub_3_009E:
        mov    ecx, dword ptr [edi + 0x13]
        add    ebp, ecx
        jecxz  loc_3_00FF
        mov    esi, dword ptr [edi + 0x17]
        test   esi, esi
        jne    loc_3_00B9
        push   0x1ae                         ; -> obj6:0x01AE
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        jmp    loc_3_00FF
loc_3_00B9:
        mov    eax, dword ptr [esi]
        mov    ebx, dword ptr [esi + 4]
        mov    edx, dword ptr [esi + 8]
        push   0x1c1                         ; -> obj6:0x01C1
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        cmp    dword ptr [esi + 0xc], 0
loc_3_00D0:
        je     loc_3_00DD
        push   0x1dc                         ; -> obj6:0x01DC
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
loc_3_00DD:
        inc    edx
        cmp    edx, 0x20
        jbe    loc_3_00EE
        push   0x1e8                         ; -> obj6:0x01E8
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
loc_3_00EE:
        push   0x10c                         ; -> obj6:0x010C
        VMMCall  VMM._Trace_Out_Service          ; cd20f3000100
        inc    ebp
        add    esi, 0x10
        loop   loc_3_00B9
loc_3_00FF:
        mov    edi, dword ptr [edi]
        test   edi, edi
        jne    loc_3_000C
loc_3_0109:
        ret    
        db  000h, 000h, 00Dh, 00Ah, 000h, 050h, 072h, 065h, 073h, 073h, 020h, 053h, 050h, 041h, 043h, 045h
        db  020h, 074h, 06Fh, 020h, 073h, 065h, 065h, 020h, 06Dh, 06Fh, 072h, 065h, 02Ch, 020h, 045h, 053h
        db  043h, 020h, 074h, 06Fh, 020h, 071h, 075h, 069h, 074h, 03Ah, 020h, 000h, 023h, 045h, 044h, 049h
        db  020h, 05Bh, 000h, 05Dh, 020h, 044h, 065h, 076h, 069h, 063h, 065h, 020h, 049h, 044h, 020h, 03Dh
        db  020h, 023h, 041h, 058h, 02Ch, 020h
loc_3_0150:
        push   edx
        inc    bx
        outsd  dx, dword ptr [esi]
        jne    0x1c5
        je     0x179
        cmp    eax, 0x4c442320
        sub    al, 0x20
        add    byte ptr [esp + eax*2 + 0x42], al
        and    byte ptr [0x53452320], bh
        dec    ecx
        or     eax, 0x544e000a
        and    byte ptr [ebp + 0x69], ch
        outsb  dx, byte ptr [esi]
        imul   esi, dword ptr [eax + 0x6f], 0xa0d7472
        add    byte ptr [ebp + 0x6e], dl
        insb   byte ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        popal  
        or     eax, 0x2020000a
        sub    ch, byte ptr [edx]
        sub    ch, byte ptr [edx]
        and    byte ptr [ecx + eax*2 + 0x4d], al
        inc    ecx
        inc    edi
        inc    ebp
loc_3_0194:
        inc    esp
        and    byte ptr [0x4142202d], ch
        inc    esp
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0x1ae8  R-X
        db  0CDh, 020h, 0B6h, 000h, 001h, 000h, 08Bh, 0F2h, 0BFh
        dd  loc_5_00F0               ; fixup
        db  0F3h, 0A4h, 06Ah, 000h, 06Ah, 010h, 0CDh, 020h, 001h, 001h, 001h, 000h, 059h, 059h, 0A3h
        dd  obj5:0x0000              ; fixup
        db  085h, 0C0h, 074h, 01Ah, 0CDh, 020h, 0C1h, 000h, 001h, 000h, 074h, 007h, 0C6h, 005h
        dd  obj5:0x00C5              ; fixup
        db  001h, 06Ah, 001h, 0CDh, 020h, 001h, 000h, 033h, 000h, 059h, 0F8h, 0C3h, 0CDh, 020h, 0BFh, 000h
        db  001h, 000h, 0C6h, 005h
        dd  obj1:0x00E4              ; fixup
        db  001h, 0F8h, 0C3h, 0CDh, 020h, 003h, 000h, 001h, 000h, 0CDh, 020h, 00Ah, 000h, 006h, 000h, 089h
        db  01Dh
        dd  obj3:0x018C              ; fixup
        db  0A3h
        dd  obj3:0x0190              ; fixup
        db  089h, 00Dh
        dd  loc_3_0194               ; fixup
        db  089h, 015h
        dd  obj3:0x0198              ; fixup
        db  00Bh, 0D2h, 075h, 028h, 06Ah, 000h, 068h, 000h, 000h, 001h, 000h, 0CDh, 020h, 0A9h, 000h, 001h
        db  000h, 059h, 059h, 00Bh, 0C0h, 074h, 015h, 083h, 0C0h, 00Fh, 0C1h, 0E8h, 004h, 0B9h, 000h, 000h
        db  001h, 000h, 08Bh, 0D1h, 0CDh, 020h, 00Bh, 000h, 006h, 000h, 0F8h, 0C3h, 0F9h, 0C3h, 050h, 052h
        db  08Bh, 01Dh
        dd  obj3:0x018C              ; fixup
        db  0A1h
        dd  obj3:0x0190              ; fixup
        db  08Bh, 00Dh
        dd  loc_3_0194               ; fixup
        db  08Bh, 015h
        dd  obj3:0x0198              ; fixup
        db  0CDh, 020h, 00Bh, 000h, 006h, 000h, 0CDh, 020h, 0AAh, 000h, 001h, 000h, 05Ah, 058h, 0C3h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 083h, 0ECh, 00Ch, 056h, 057h, 08Bh, 074h
        db  024h, 01Ch, 08Bh, 07Ch, 024h, 018h, 056h, 057h, 0E8h
        dd  obj4:0x1AA6              ; fixup
        db  083h, 0C4h, 008h, 083h, 03Eh, 000h, 076h, 00Bh, 0B8h, 001h, 000h, 000h, 000h, 05Fh, 05Eh, 083h
        db  0C4h, 00Ch, 0C3h, 06Ah, 000h, 068h, 000h, 000h, 001h, 000h, 0E8h, 07Dh, 000h, 000h, 000h, 083h
        db  0C4h, 008h, 00Bh, 0C0h, 074h, 02Eh, 0C1h, 0E8h, 004h, 066h, 089h, 044h, 024h, 010h, 0C7h, 044h
        db  024h, 00Ch, 000h, 000h, 001h, 000h, 0C7h, 044h, 024h, 008h, 000h, 000h, 001h, 000h, 08Dh, 044h
        db  024h, 008h, 050h, 057h, 0E8h
        dd  loc_4_1AC6               ; fixup
        db  083h, 0C4h, 008h, 02Bh, 0C0h, 05Fh, 05Eh, 083h, 0C4h, 00Ch, 0C3h, 0B8h, 002h, 000h, 000h, 000h
        db  05Fh, 05Eh, 083h, 0C4h, 00Ch, 0C3h, 08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h
        db  000h, 000h, 000h, 08Bh, 044h, 024h, 008h, 050h, 08Bh, 044h, 024h, 008h, 050h, 0E8h
        dd  loc_4_1AC6               ; fixup
        db  083h, 0C4h, 008h, 0E8h, 01Fh, 000h, 000h, 000h, 00Bh, 0C0h, 074h, 005h, 02Bh, 0C0h, 0C3h, 08Bh
        db  0FFh, 0B8h, 001h, 000h, 000h, 000h, 0C3h, 08Dh, 049h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h
        db  000h, 0CDh, 020h, 0A9h, 080h, 001h, 000h, 0CDh, 020h, 0AAh, 080h, 001h, 000h, 000h, 000h, 000h
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
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h
_PELDR_LoadModule:
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
_PELDR_LoadModuleEx:
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
_PELDR_GetModuleHandle:
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
_PELDR_GetModuleUsage:
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
_PELDR_GetEntryPoint:
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
_PELDR_GetProcAddress:
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
_PELDR_AddExportTable:
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
_PELDR_RemoveExportTable:
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
_PELDR_FreeModule:
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
_PELDR_InitCompleted:
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
        push   esi
        mov    eax, dword ptr [0xe8]         ; -> loc_5_00E8
        or     eax, eax
        jne    loc_4_1010
        sub    eax, eax
        pop    esi
        ret    
        db  08Bh, 0FFh
loc_4_1010:
        push   eax
        call   0x2a3a
        add    esp, 4
        or     eax, eax
        je     loc_4_103F
        mov    ecx, dword ptr [0xe8]         ; -> loc_5_00E8
        mov    esi, dword ptr [esp + 8]
loc_4_1027:
        cmp    esi, eax
        je     loc_4_1043
        push   eax
        push   ecx
        call   0x2a4a
        add    esp, 8
        or     eax, eax
        mov    ecx, dword ptr [0xe8]         ; -> loc_5_00E8
        jne    loc_4_1027
loc_4_103F:
        sub    eax, eax
        pop    esi
        ret    
loc_4_1043:
        mov    eax, 1
        pop    esi
        ret    
        db  08Dh, 09Bh, 000h, 000h, 000h, 000h, 056h, 057h, 08Bh, 07Ch, 024h, 010h, 08Bh, 007h, 00Bh, 0C0h
        db  075h, 005h, 02Bh, 0C0h, 05Fh, 05Eh, 0C3h, 050h
loc_4_1062:
        call   0x2a3a
        add    esp, 4
        or     eax, eax
        je     loc_4_1086
        db  08Bh, 074h
loc_4_1070:
        and    al, 0xc
loc_4_1072:
        cmp    eax, esi
loc_4_1074:
        je     loc_4_1090
        push   eax
        mov    eax, dword ptr [edi]
        push   eax
        call   0x2a4a
        add    esp, 8
        or     eax, eax
        jne    loc_4_1072
loc_4_1086:
        sub    eax, eax
        pop    edi
        pop    esi
        ret    
        db  005h, 000h, 000h, 000h, 000h
loc_4_1090:
        mov    eax, 1
        pop    edi
        pop    esi
        ret    
        db  08Dh, 064h, 024h, 000h, 08Dh, 064h, 024h, 000h, 053h, 056h, 057h, 055h, 08Bh, 05Ch, 024h, 014h
        db  08Bh, 083h, 008h, 002h, 000h, 000h, 033h, 0F6h, 050h, 0E8h, 0EAh, 015h, 000h, 000h, 083h, 0C4h
        db  004h, 033h, 0C9h, 066h, 08Bh, 048h, 014h, 066h
loc_4_10C0:
        mov    esi, dword ptr [eax + 6]
        lea    edx, [eax + ecx + 0x18]
        lea    ecx, [esi + esi*4]
        lea    ecx, [edx + ecx*8]
        sub    ecx, 0x28
loc_4_10D0:
        mov    edi, 0x1f4                    ; -> obj5:0x01F4
        mov    eax, dword ptr [ecx]
        cmp    eax, dword ptr [edi]
        jne    loc_4_10EE
        mov    ax, word ptr [ecx + 4]
        db  066h
loc_4_10E0:
        cmp    eax, dword ptr [edi + 4]
        jne    loc_4_10EE
        mov    al, byte ptr [ecx + 6]
        cmp    al, byte ptr [edi + 6]
        jne    loc_4_10EE
        dec    esi
loc_4_10EE:
        sub    edi, edi
        cmp    esi, edi
        je     loc_4_1131
        mov    eax, dword ptr [esp + 0x18]
loc_4_10F8:
        mov    ecx, dword ptr [edx + 8]
        mov    ebx, dword ptr [edx + 0x10]
        test   cl, 1
        je     loc_4_1104
        inc    ecx
loc_4_1104:
        test   bl, 1
        je     loc_4_110A
        inc    ebx
loc_4_110A:
        or     ecx, ecx
        jne    loc_4_1110
        mov    ecx, ebx
loc_4_1110:
        mov    ebx, dword ptr [esp + 0x14]
        mov    ebp, dword ptr [edx + 0xc]
        mov    ebx, dword ptr [ebx + 0x208]
        add    ebx, ebp
        cmp    ebx, eax
        ja     loc_4_1129
        add    ecx, ebx
        cmp    ecx, eax
        ja     loc_4_1138
loc_4_1129:
        add    edx, 0x28
        inc    edi
        cmp    esi, edi
        ja     loc_4_10F8
loc_4_1131:
        sub    eax, eax
loc_4_1133:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
loc_4_1138:
        mov    ecx, 0x1fc                    ; -> obj5:0x01FC
        mov    esi, dword ptr [edx]
        cmp    esi, dword ptr [ecx]
        jne    loc_4_1133
        mov    dl, byte ptr [edx + 4]
        cmp    dl, byte ptr [ecx + 4]
        jne    loc_4_1133
        mov    ebx, dword ptr [esp + 0x14]
        cmp    dword ptr [ebx + 0x210], 0
        je     loc_4_1133
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
        db  090h, 053h, 056h, 057h, 055h, 08Bh, 04Ch, 024h, 014h, 08Bh, 081h, 008h, 002h, 000h, 000h, 050h
        db  0E8h, 02Ch, 015h, 000h, 000h, 083h, 0C4h, 004h, 08Bh, 0E8h, 033h, 0C0h, 066h, 08Bh, 045h, 014h
        db  033h, 0FFh, 066h, 08Bh, 07Dh, 006h, 08Dh, 054h, 005h, 018h, 08Dh, 034h, 0BFh, 08Dh, 034h, 0F2h
        db  083h, 0EEh, 028h, 0B9h
        dd  obj5:0x0204              ; fixup
        db  08Bh, 006h, 03Bh, 001h, 075h, 013h, 066h, 08Bh, 046h, 004h, 066h, 03Bh, 041h, 004h, 075h, 009h
        db  08Ah, 046h, 006h, 03Ah, 041h, 006h, 075h, 001h, 04Fh, 02Bh, 0F6h, 03Bh, 0FEh, 00Fh, 084h, 0C2h
        db  000h, 000h, 000h, 08Bh, 0CAh, 08Bh, 05Ch, 024h, 018h, 08Ah, 001h, 03Ah, 003h, 075h, 01Ah, 00Ah
        db  0C0h, 074h, 012h, 08Ah, 041h, 001h, 03Ah, 043h, 001h, 075h, 00Eh, 083h, 0C1h, 002h, 083h, 0C3h
        db  002h, 00Ah, 0C0h, 075h, 0E4h, 02Bh, 0C0h, 0EBh, 005h, 01Bh, 0C0h, 083h, 0D8h, 0FFh, 00Bh, 0C0h
        db  074h, 00Dh, 083h, 0C2h, 028h, 046h, 03Bh, 0FEh, 077h, 0C9h, 05Dh, 05Fh, 05Eh, 05Bh, 0C3h, 08Bh
        db  042h, 008h, 08Bh, 04Ah, 010h, 0A8h, 001h, 074h, 001h, 040h, 0F6h, 0C1h, 001h, 074h, 001h, 041h
        db  00Bh, 0C0h, 075h, 002h, 08Bh, 0C1h, 08Bh, 04Ch, 024h, 014h, 08Bh, 052h, 00Ch, 003h, 091h, 008h
        db  002h, 000h, 000h, 08Dh, 08Ah, 0FFh, 00Fh, 000h, 000h, 0C1h, 0E9h, 00Ch, 003h, 0D0h, 0C1h, 0EAh
        db  00Ch, 03Bh, 0D1h, 073h, 004h, 02Bh, 0D2h, 0EBh, 01Bh, 02Bh, 0D1h, 0C1h, 0EAh, 00Ch, 00Bh, 0D2h
        db  075h, 016h, 08Bh, 0DFh, 02Bh, 0DEh, 083h, 0FBh, 001h, 075h, 009h, 00Bh, 0C0h, 074h, 005h, 0BAh
        db  001h, 000h, 000h, 000h, 00Bh, 0D2h, 074h, 022h, 06Ah, 000h, 052h, 051h, 0E8h, 028h, 018h, 000h
        db  000h, 083h, 0C4h, 00Ch, 00Bh, 0C0h, 074h, 012h, 08Bh, 044h, 024h, 018h, 08Bh, 04Ch, 024h, 014h
        db  050h, 051h, 0E8h, 032h, 013h, 000h, 000h, 083h, 0C4h, 008h, 02Bh, 0FEh, 083h, 0FFh, 001h, 075h
        db  004h, 066h, 0FFh, 04Dh, 006h, 05Dh, 05Fh, 05Eh, 05Bh, 0C3h, 08Dh, 064h, 024h, 000h, 08Dh, 064h
        db  024h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h, 08Bh, 044h, 024h, 00Ch, 06Ah, 000h, 050h
        db  08Bh, 044h, 024h, 010h, 050h, 08Bh, 044h, 024h, 010h, 050h, 0E8h, 00Ah, 000h, 000h, 000h, 083h
        db  0C4h, 010h, 0C3h, 08Dh, 09Bh, 000h, 000h, 000h, 000h, 083h, 0ECh, 03Ch, 053h, 056h, 057h, 055h
        db  08Bh, 04Ch, 024h, 050h, 0C7h, 044h, 024h, 034h, 001h, 000h, 000h, 000h, 0C7h, 001h, 000h, 000h
        db  000h, 000h, 0C7h, 044h, 024h, 014h, 000h, 000h, 000h, 000h, 0C7h, 044h, 024h, 02Ch, 000h, 000h
        db  000h, 000h, 0C7h, 044h, 024h, 028h, 000h, 000h, 000h, 000h, 0C7h, 044h, 024h, 020h, 000h, 000h
        db  000h, 000h, 0C7h, 044h, 024h, 024h, 000h, 000h, 000h, 000h, 06Ah, 001h, 068h, 000h, 001h, 000h
        db  000h, 0E8h, 0DEh, 016h, 000h, 000h, 089h, 044h, 024h, 038h, 083h, 0C4h, 008h, 00Bh, 0C0h, 075h
        db  00Eh, 0BEh, 0FEh, 0FFh, 0FFh, 0FFh, 08Bh, 06Ch, 024h, 03Ch, 0E9h, 0B6h, 006h, 000h, 000h, 06Ah
        db  001h, 068h, 000h, 001h, 000h, 000h, 0E8h, 0B9h, 016h, 000h, 000h, 089h, 044h, 024h, 030h, 083h
        db  0C4h, 008h, 00Bh, 0C0h, 075h, 013h, 0BEh, 0FEh, 0FFh, 0FFh, 0FFh, 08Bh, 06Ch, 024h, 03Ch, 0E9h
        db  091h, 006h, 000h, 000h, 005h, 000h, 000h, 000h, 000h, 06Ah, 001h, 068h, 000h, 002h, 000h, 000h
        db  0E8h, 08Fh, 016h, 000h, 000h, 089h, 044h, 024h, 02Ch, 083h, 0C4h, 008h, 00Bh, 0C0h, 075h, 00Eh
        db  0BEh, 0FEh, 0FFh, 0FFh, 0FFh, 08Bh, 06Ch, 024h, 03Ch, 0E9h, 067h, 006h, 000h, 000h, 06Ah, 001h
        db  068h, 000h, 002h, 000h, 000h, 0E8h, 06Ah, 016h, 000h, 000h, 089h, 044h, 024h, 028h, 083h, 0C4h
        db  008h, 00Bh, 0C0h, 075h, 014h, 0BEh, 0FEh, 0FFh, 0FFh, 0FFh, 08Bh, 06Ch, 024h, 03Ch, 0E9h, 042h
        db  006h, 000h, 000h, 08Dh, 09Bh, 000h, 000h, 000h, 000h, 083h, 07Ch, 024h, 058h, 000h, 075h, 008h
        db  0C7h, 044h, 024h, 058h
        dd  obj5:0x00EC              ; fixup
        db  08Dh, 044h, 024h, 040h, 08Bh, 05Ch, 024h, 054h, 0C7h, 000h, 000h, 000h, 000h, 000h, 0C7h, 040h
        db  004h, 000h, 000h, 000h, 000h, 066h, 0C7h, 040h, 008h, 000h, 000h, 00Bh, 0DBh, 075h, 00Dh, 0B8h
        db  0FDh, 0FFh, 0FFh, 0FFh, 05Dh, 05Fh, 05Eh, 05Bh, 083h, 0C4h, 03Ch, 0C3h, 08Bh, 044h, 024h, 028h
        db  050h, 08Bh, 044h, 024h, 034h, 050h, 08Bh, 044h, 024h, 028h, 050h, 06Ah, 000h, 053h, 0E8h, 08Eh
        db  014h, 000h, 000h, 08Bh, 07Ch, 024h, 044h, 083h, 0C4h, 014h, 0B9h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh
        db  0C0h, 0F2h, 0AEh, 0F7h, 0D1h, 02Bh, 0F9h, 08Bh, 0C1h, 08Bh, 0F7h, 08Bh, 07Ch, 024h, 024h, 0C1h
        db  0E9h, 002h, 0F3h, 0A5h, 08Bh, 0C8h, 083h, 0E1h, 003h, 0F3h, 0A4h, 08Bh, 044h, 024h, 028h, 080h
        db  038h, 000h, 074h, 02Fh, 08Bh, 0F8h, 0B9h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0C0h, 0F2h, 0AEh, 0F7h
        db  0D1h, 02Bh, 0F9h, 08Bh, 0F7h, 08Bh, 0D1h, 08Bh, 07Ch, 024h, 024h, 0B9h, 0FFh, 0FFh, 0FFh, 0FFh
        db  02Bh, 0C0h, 0F2h, 0AEh, 04Fh, 08Bh, 0CAh, 0C1h, 0E9h, 002h, 0F3h, 0A5h, 08Bh, 0CAh, 083h, 0E1h
        db  003h, 0F3h, 0A4h, 08Bh, 044h, 024h, 024h, 050h, 0E8h, 064h, 006h, 000h, 000h, 083h, 0C4h, 004h
        db  00Bh, 0C0h, 074h, 01Dh, 08Bh, 04Ch, 024h, 050h, 0FFh, 080h, 000h, 002h, 000h, 000h, 089h, 001h
        db  02Bh, 0C0h, 05Dh, 05Fh, 05Eh, 05Bh, 083h, 0C4h, 03Ch, 0C3h, 08Dh, 0A4h, 024h, 000h, 000h, 000h
        db  000h, 083h, 03Dh
        dd  loc_5_00E8               ; fixup
        db  000h, 075h, 027h, 068h, 047h, 002h, 000h, 000h, 06Ah, 000h, 0E8h, 089h, 015h, 000h, 000h, 083h
        db  0C4h, 008h, 0A3h
        dd  loc_5_00E8               ; fixup
        db  00Bh, 0C0h, 075h, 00Fh, 0B8h, 0FEh, 0FFh, 0FFh, 0FFh, 05Dh, 05Fh, 05Eh, 05Bh, 083h, 0C4h, 03Ch
        db  0C3h, 08Bh, 0FFh, 0A1h
        dd  loc_5_00E8               ; fixup
        db  050h, 0E8h, 03Ch, 015h, 000h, 000h, 083h, 0C4h, 004h, 08Bh, 0E8h, 00Bh, 0EDh, 075h, 00Dh, 0B8h
        db  0FEh, 0FFh, 0FFh, 0FFh, 05Dh, 05Fh, 05Eh, 05Bh, 083h, 0C4h, 03Ch, 0C3h, 0C7h, 085h, 010h, 002h
        db  000h, 000h, 000h, 000h, 000h, 000h, 08Bh, 044h, 024h, 020h, 0C7h, 085h, 008h, 002h, 000h, 000h
        db  000h, 000h, 000h, 000h, 080h, 038h, 000h, 075h, 05Bh, 0BFh
        dd  loc_5_00F0               ; fixup
        db  0B9h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0C0h, 0F2h, 0AEh, 0F7h, 0D1h, 02Bh, 0F9h, 08Bh, 0C1h, 08Bh
        db  0F7h, 08Bh, 07Ch, 024h, 020h, 0C1h, 0E9h, 002h, 0F3h, 0A5h, 08Bh, 0C8h, 083h, 0E1h, 003h, 0F3h
        db  0A4h, 08Bh, 07Ch, 024h, 024h, 0B9h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh, 0C0h, 0F2h, 0AEh, 0F7h, 0D1h
        db  02Bh, 0F9h, 08Bh, 0F7h, 08Bh, 0D1h, 08Bh, 07Ch, 024h, 020h, 0B9h, 0FFh, 0FFh, 0FFh, 0FFh, 02Bh
        db  0C0h, 0F2h, 0AEh, 04Fh, 08Bh, 0CAh, 0C1h, 0E9h, 002h, 0F3h, 0A5h, 08Bh, 0CAh, 083h, 0E1h, 003h
        db  0F3h, 0A4h, 08Bh, 05Ch, 024h, 020h, 080h, 03Dh
        dd  obj1:0x00E4              ; fixup
        db  000h, 075h, 026h, 08Dh, 044h, 024h, 040h, 050h, 0E8h, 084h, 014h, 000h, 000h, 050h, 0E8h
        dd  loc_3_00D0               ; fixup
        db  089h, 044h, 024h, 03Ch, 083h, 0C4h, 008h, 083h, 0F8h, 002h, 075h, 00Ah, 0BEh, 0FEh, 0FFh, 0FFh
        db  0FFh, 0E9h, 064h, 004h, 000h, 000h, 06Ah, 020h, 053h, 0E8h, 028h, 012h, 000h, 000h, 089h, 044h
        db  024h, 01Ch, 083h, 0C4h, 008h, 00Bh, 0C0h, 00Fh, 084h, 048h, 004h, 000h, 000h, 06Ah, 001h, 068h
        db  000h, 004h, 000h, 000h, 0E8h, 050h, 014h, 000h, 000h, 089h, 044h, 024h, 034h, 083h, 0C4h, 008h
        db  00Bh, 0C0h, 075h, 00Ah, 0BEh, 0FEh, 0FFh, 0FFh, 0FFh, 0E9h, 02Ch, 004h, 000h, 000h, 08Bh, 044h
        db  024h, 02Ch, 050h, 08Bh, 044h, 024h, 018h, 068h, 000h, 004h, 000h, 000h, 050h, 0E8h, 049h, 012h
        db  000h, 000h, 083h, 0C4h, 00Ch, 03Dh, 000h, 004h, 000h, 000h, 074h, 00Ah, 0BEh, 0FBh, 0FFh, 0FFh
        db  0FFh, 0E9h, 004h, 004h, 000h, 000h, 08Bh, 044h, 024h, 02Ch, 050h, 0E8h, 0CEh, 010h, 000h, 000h
        db  083h, 0C4h, 004h, 08Bh, 0F0h, 00Bh, 0F6h, 00Fh, 084h, 0E1h, 003h, 000h, 000h, 066h, 081h, 07Eh
        db  004h, 04Ch, 001h, 00Fh, 085h, 0D5h, 003h, 000h, 000h, 033h, 0C0h, 066h, 08Bh, 046h, 006h, 089h
        db  044h, 024h, 01Ch, 08Bh, 046h, 050h, 005h, 0FFh, 00Fh, 000h, 000h, 0C1h, 0E8h, 00Ch, 089h, 044h
        db  024h, 038h, 033h, 0C0h, 066h, 08Bh, 046h, 014h, 08Dh, 04Ch, 006h, 018h, 08Bh, 044h, 024h, 01Ch
        db  08Dh, 03Ch, 080h, 08Dh, 03Ch, 0F9h, 083h, 0EFh, 028h, 0BAh
        dd  loc_5_020C               ; fixup
        db  08Bh, 007h, 03Bh, 002h, 075h, 031h, 066h, 08Bh, 047h, 004h, 066h, 03Bh, 042h, 004h, 075h, 027h
        db  08Ah, 047h, 006h, 03Ah, 042h, 006h, 075h, 01Fh, 0FFh, 04Ch, 024h, 01Ch, 08Bh, 044h, 024h, 01Ch
        db  08Bh, 056h, 050h, 08Dh, 004h, 080h, 02Bh, 054h, 0C1h, 010h, 081h, 0C2h, 0FFh, 00Fh, 000h, 000h
        db  0C1h, 0EAh, 00Ch, 089h, 054h, 024h, 038h, 08Bh, 04Ch, 024h, 038h, 06Ah, 000h, 051h, 068h, 000h
        db  000h, 008h, 080h, 0E8h, 024h, 014h, 000h, 000h, 083h, 0C4h, 00Ch, 089h, 085h, 008h, 002h, 000h
        db  000h, 083h, 0F8h, 0FFh, 075h, 00Ah, 0BEh, 0FEh, 0FFh, 0FFh, 0FFh, 0E9h, 04Ch, 003h, 000h, 000h
        db  06Ah, 008h, 06Ah, 000h, 08Bh, 04Ch, 024h, 040h, 06Ah, 003h, 0C1h, 0E8h, 00Ch, 051h, 050h, 0E8h
        db  0E6h, 013h, 000h, 000h, 083h, 0C4h, 014h, 00Bh, 0C0h, 075h, 00Ah, 0BEh, 0FEh, 0FFh, 0FFh, 0FFh
        db  0E9h, 027h, 003h, 000h, 000h, 06Ah, 000h, 08Bh, 044h, 024h, 018h, 06Ah, 000h, 050h, 0E8h, 017h
        db  011h, 000h, 000h, 083h, 0C4h, 00Ch, 08Bh, 076h, 054h, 08Bh, 085h, 008h, 002h, 000h, 000h, 050h
        db  08Bh, 044h, 024h, 018h, 056h, 050h, 0E8h, 032h, 011h, 000h, 000h, 083h, 0C4h, 00Ch, 03Bh, 0C6h
        db  074h, 00Eh, 0BEh, 0FCh, 0FFh, 0FFh, 0FFh, 0E9h, 0F0h, 002h, 000h, 000h, 08Dh, 064h, 024h, 000h
        db  08Bh, 085h, 008h, 002h, 000h, 000h, 050h, 0E8h, 0B4h, 00Fh, 000h, 000h, 083h, 0C4h, 004h, 033h
        db  0C9h, 066h, 08Bh, 048h, 014h, 08Dh, 05Ch, 008h, 018h, 08Bh, 07Ch, 024h, 024h, 0B9h, 0FFh, 0FFh
        db  0FFh, 0FFh, 02Bh, 0C0h, 0F2h, 0AEh, 0F7h, 0D1h, 02Bh, 0F9h, 08Bh, 0C1h, 08Bh, 0F7h, 08Bh, 0FDh
        db  0C1h, 0E9h, 002h, 0F3h, 0A5h, 08Bh, 0C8h, 083h, 0E1h, 003h, 0F3h, 0A4h, 0C7h, 044h, 024h, 018h
        db  000h, 000h, 000h, 000h, 083h, 07Ch, 024h, 01Ch, 000h, 00Fh, 084h, 019h, 001h, 000h, 000h, 090h
        db  08Bh, 043h, 008h, 08Bh, 04Bh, 010h, 089h, 04Ch, 024h, 010h, 0A8h, 001h, 074h, 001h, 040h, 0F6h
        db  044h, 024h, 010h, 001h, 074h, 004h, 0FFh, 044h, 024h, 010h, 00Bh, 0C0h, 075h, 004h, 08Bh, 044h
        db  024h, 010h, 083h, 07Bh, 014h, 000h, 075h, 00Ah, 0C7h, 044h, 024h, 010h, 000h, 000h, 000h, 000h
        db  0EBh, 00Ah, 039h, 044h, 024h, 010h, 076h, 004h, 089h, 044h, 024h, 010h, 00Bh, 0C0h, 074h, 079h
        db  08Bh, 00Bh, 081h, 0F9h, 050h, 041h, 047h, 045h, 074h, 008h, 081h, 0F9h, 02Eh, 065h, 064h, 061h
        db  075h, 067h, 08Bh, 04Bh, 00Ch, 003h, 08Dh, 008h, 002h, 000h, 000h, 08Dh, 0B1h, 0FFh, 00Fh, 000h
        db  000h, 0C1h, 0EEh, 00Ch, 08Dh, 03Ch, 001h, 0C1h, 0EFh, 00Ch, 03Bh, 0FEh, 073h, 004h, 02Bh, 0FFh
        db  0EBh, 01Fh, 02Bh, 0FEh, 0C1h, 0EFh, 00Ch, 00Bh, 0FFh, 075h, 016h, 08Bh, 04Ch, 024h, 01Ch, 02Bh
        db  04Ch, 024h, 018h, 083h, 0F9h, 001h, 075h, 009h, 00Bh, 0C0h, 074h, 005h, 0BFh, 001h, 000h, 000h
        db  000h, 06Ah, 000h, 057h, 056h, 0E8h, 0B6h, 012h, 000h, 000h, 083h, 0C4h, 00Ch, 085h, 0C7h, 074h
        db  018h, 06Ah, 000h, 06Ah, 000h, 06Ah, 001h, 057h, 056h, 0E8h, 09Ch, 012h, 000h, 000h, 083h, 0C4h
        db  014h, 00Bh, 0C0h, 00Fh, 084h, 0C7h, 001h, 000h, 000h, 083h, 07Ch, 024h, 010h, 000h, 074h, 039h
        db  06Ah, 000h, 08Bh, 043h, 014h, 050h, 08Bh, 044h, 024h, 01Ch, 050h, 0E8h, 0CAh, 00Fh, 000h, 000h
        db  083h, 0C4h, 00Ch, 08Bh, 043h, 00Ch, 003h, 085h, 008h, 002h, 000h, 000h, 08Bh, 04Ch, 024h, 010h
        db  050h, 08Bh, 044h, 024h, 018h, 051h, 050h, 0E8h, 0E1h, 00Fh, 000h, 000h, 083h, 0C4h, 00Ch, 03Bh
        db  044h, 024h, 010h, 00Fh, 085h, 08Eh, 001h, 000h, 000h, 053h, 055h, 0E8h, 000h, 00Ch, 000h, 000h
        db  083h, 0C4h, 008h, 083h, 0C3h, 028h, 0FFh, 044h, 024h, 018h, 08Bh, 044h, 024h, 01Ch, 03Bh, 044h
        db  024h, 018h, 00Fh, 087h, 0E8h, 0FEh, 0FFh, 0FFh, 08Bh, 044h, 024h, 058h, 050h, 055h, 0E8h, 06Dh
        db  00Eh, 000h, 000h, 083h, 0C4h, 008h, 08Bh, 0F0h, 00Bh, 0F6h, 00Fh, 085h, 06Ch, 001h, 000h, 000h
        db  068h
        dd  obj5:0x0214              ; fixup
        db  055h, 0E8h, 0F5h, 0F8h, 0FFh, 0FFh, 083h, 0C4h, 008h, 0F6h, 044h, 024h, 05Ch, 001h, 074h, 00Eh
        db  068h
        dd  obj5:0x021C              ; fixup
        db  055h, 0E8h, 0E0h, 0F8h, 0FFh, 0FFh, 083h, 0C4h, 008h, 0C7h, 085h, 000h, 002h, 000h, 000h, 001h
        db  000h, 000h, 000h, 055h, 0A1h
        dd  loc_5_00E8               ; fixup
        db  050h, 0E8h, 062h, 011h, 000h, 000h, 083h, 0C4h, 008h, 0C6h, 085h, 018h, 002h, 000h, 000h, 001h
        db  0C7h, 085h, 019h, 002h, 000h, 000h, 001h, 000h, 000h, 000h, 066h, 0C7h, 085h, 01Dh, 002h, 000h
        db  000h, 000h, 000h, 089h, 0ADh, 01Fh, 002h, 000h, 000h, 08Dh, 085h, 037h, 002h, 000h, 000h, 0C7h
        db  085h, 023h, 002h, 000h, 000h, 058h, 056h, 04Ch, 044h, 0C7h, 085h, 027h, 002h, 000h, 000h, 001h
        db  000h, 000h, 000h, 089h, 085h, 02Bh, 002h, 000h, 000h, 0C7h, 085h, 02Fh, 002h, 000h, 000h, 000h
        db  000h, 000h, 000h, 0C7h, 085h, 033h, 002h, 000h, 000h, 000h, 000h, 000h, 000h, 08Bh, 08Dh, 008h
        db  002h, 000h, 000h, 089h, 008h, 08Bh, 044h, 024h, 038h, 0C1h, 0E0h, 00Ch, 089h, 085h, 03Bh, 002h
        db  000h, 000h, 0C7h, 085h, 03Fh, 002h, 000h, 000h, 001h, 000h, 000h, 000h, 0C7h, 085h, 043h, 002h
        db  000h, 000h, 000h, 000h, 000h, 000h, 08Dh, 085h, 014h, 002h, 000h, 000h, 050h, 0E8h
        dd  sub_7_01CD               ; fixup
        db  08Bh, 04Ch, 024h, 050h, 06Ah, 000h, 089h, 029h, 08Bh, 044h, 024h, 034h, 050h, 0E8h, 0AAh, 010h
        db  000h, 000h, 083h, 0C4h, 008h, 08Bh, 044h, 024h, 028h, 06Ah, 000h, 050h, 0E8h, 09Bh, 010h, 000h
        db  000h, 083h, 0C4h, 008h, 08Bh, 044h, 024h, 020h, 06Ah, 000h, 050h, 0E8h, 08Ch, 010h, 000h, 000h
        db  083h, 0C4h, 008h, 08Bh, 044h, 024h, 024h, 06Ah, 000h, 050h, 0E8h, 07Dh, 010h, 000h, 000h, 083h
        db  0C4h, 008h, 08Bh, 044h, 024h, 02Ch, 06Ah, 000h, 050h, 0E8h, 06Eh, 010h, 000h, 000h, 08Bh, 044h
        db  024h, 01Ch, 083h, 0C4h, 008h, 050h, 0E8h, 030h, 00Eh, 000h, 000h, 083h, 0C4h, 004h, 080h, 03Dh
        dd  obj1:0x00E4              ; fixup
        db  000h, 075h, 01Ah, 083h, 07Ch, 024h, 034h, 000h, 075h, 013h, 08Dh, 044h, 024h, 040h, 050h, 0E8h
        db  033h, 010h, 000h, 000h, 050h, 0E8h
        dd  loc_3_0150               ; fixup
        db  083h, 0C4h, 008h, 02Bh, 0C0h, 05Dh, 05Fh, 05Eh, 05Bh, 083h, 0C4h, 03Ch, 0C3h, 0BEh, 0FEh, 0FFh
        db  0FFh, 0FFh, 0EBh, 015h, 0BEh, 0FCh, 0FFh, 0FFh, 0FFh, 0EBh, 00Eh, 08Bh, 0FFh, 0BEh, 0FAh, 0FFh
        db  0FFh, 0FFh, 0EBh, 005h, 0BEh, 0FDh, 0FFh, 0FFh, 0FFh, 083h, 07Ch, 024h, 024h, 000h, 074h, 00Fh
        db  08Bh, 044h, 024h, 024h, 06Ah, 000h, 050h, 0E8h, 002h, 010h, 000h, 000h, 083h, 0C4h, 008h, 083h
        db  07Ch, 024h, 020h, 000h, 074h, 00Fh, 08Bh, 044h, 024h, 020h, 06Ah, 000h, 050h, 0E8h, 0ECh, 00Fh
        db  000h, 000h, 083h, 0C4h, 008h, 083h, 07Ch, 024h, 028h, 000h, 074h, 00Fh, 08Bh, 044h, 024h, 028h
        db  06Ah, 000h, 050h, 0E8h, 0D6h, 00Fh, 000h, 000h, 083h, 0C4h, 008h, 083h, 07Ch, 024h, 030h, 000h
        db  074h, 00Fh, 08Bh, 044h, 024h, 030h, 06Ah, 000h, 050h, 0E8h, 0C0h, 00Fh, 000h, 000h, 083h, 0C4h
        db  008h, 083h, 07Ch, 024h, 014h, 000h, 074h, 00Dh, 08Bh, 044h, 024h, 014h, 050h, 0E8h, 07Bh, 00Dh
        db  000h, 000h, 083h, 0C4h, 004h, 080h, 03Dh
        dd  obj1:0x00E4              ; fixup
        db  000h, 075h, 01Ah, 083h, 07Ch, 024h, 034h, 000h, 075h, 013h, 08Dh, 044h, 024h, 040h, 050h, 0E8h
        db  07Eh, 00Fh, 000h, 000h, 050h, 0E8h
        dd  loc_3_0150               ; fixup
        db  083h, 0C4h, 008h, 083h, 07Ch, 024h, 02Ch, 000h, 074h, 00Fh, 08Bh, 044h, 024h, 02Ch, 06Ah, 000h
        db  050h, 0E8h, 073h, 00Fh, 000h, 000h, 083h, 0C4h, 008h, 00Bh, 0EDh, 074h, 024h, 08Bh, 085h, 008h
        db  002h, 000h, 000h, 00Bh, 0C0h, 074h, 00Bh, 06Ah, 000h, 050h, 0E8h, 0FFh, 00Fh, 000h, 000h, 083h
        db  0C4h, 008h, 055h, 0A1h
        dd  loc_5_00E8               ; fixup
        db  050h, 0E8h, 093h, 00Fh, 000h, 000h, 083h, 0C4h, 008h, 08Bh, 0C6h, 05Dh, 05Fh, 05Eh, 05Bh, 083h
        db  0C4h, 03Ch, 0C3h, 08Dh, 09Bh, 000h, 000h, 000h, 000h, 08Dh, 0A4h, 024h, 000h, 000h, 000h, 000h
        db  056h, 057h, 0A1h
        dd  loc_5_00E8               ; fixup
        db  00Bh, 0C0h, 075h, 005h, 02Bh, 0C0h, 05Fh, 05Eh, 0C3h, 050h, 0E8h, 074h, 00Fh, 000h, 000h
loc_4_1AC6:
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        je     0x1af6
        mov    esi, dword ptr [esp + 0xc]
        push   esi
        push   edi
        call   0x2750
        add    esp, 8
        or     eax, eax
        mov    eax, dword ptr [0xe8]         ; -> loc_5_00E8
        je     0x1afb
        push   edi
        push   eax
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 5  vsize=0x234  RW-
        db  08Ch, 0FFh, 0FFh, 0FFh, 083h, 0C4h, 010h, 08Bh, 0D8h, 083h, 07Ch, 024h, 010h, 000h, 00Fh, 084h
        db  095h, 000h, 000h, 000h, 08Bh, 0EBh, 083h, 0C3h, 008h, 08Bh, 045h, 004h, 029h, 044h, 024h, 010h
        db  08Dh, 070h, 0F8h, 0D1h, 0EEh, 08Bh, 0C6h, 04Eh, 00Bh, 0C0h, 074h, 072h, 08Bh, 045h, 000h, 050h
        db  08Bh, 044h, 024h, 01Ch, 050h, 0E8h, 096h, 0FFh, 0FFh, 0FFh, 083h, 0C4h, 008h, 066h, 08Bh, 013h
        db  066h, 08Bh, 0CAh, 081h, 0E1h, 0FFh, 00Fh, 000h, 000h, 066h, 0C1h, 0EAh, 00Ch, 003h, 0C8h, 08Bh
        db  0C2h, 025h, 0FFh, 0FFh, 000h, 000h, 083h, 0F8h, 005h, 077h, 058h, 0FFh, 024h, 085h
        dd  loc_4_10E0               ; fixup
        db  033h, 0C0h, 066h, 08Bh, 001h, 0C1h, 0E0h, 010h, 003h, 0C7h, 0EBh, 020h, 08Bh, 0FFh, 001h, 039h
        db  0EBh, 020h, 033h, 0C0h, 066h, 08Bh, 001h, 0C1h, 0E0h, 010h, 083h, 0C3h, 002h, 033h, 0D2h, 066h
        db  08Bh, 013h, 04Eh, 003h, 0C2h, 08Dh, 084h, 007h, 000h, 080h, 000h, 000h, 0C1h, 0F8h, 010h, 066h
        db  089h, 001h, 083h, 0C3h, 002h, 08Bh, 0C6h, 04Eh, 00Bh, 0C0h, 075h, 08Eh, 083h, 07Ch, 024h, 010h
        db  000h, 00Fh, 085h, 06Bh, 0FFh, 0FFh, 0FFh, 02Bh, 0C0h, 05Dh, 05Fh, 05Eh, 05Bh, 083h, 0C4h, 004h
        db  0C3h, 0B8h, 0F9h, 0FFh, 0FFh, 0FFh, 05Dh, 05Fh, 05Eh, 05Bh, 083h, 0C4h, 004h, 0C3h, 0B8h, 0F8h
        db  0FFh, 0FFh, 0FFh, 05Dh, 05Fh, 05Eh, 05Bh, 083h, 0C4h, 004h, 0C3h, 08Dh, 049h, 000h, 0B8h, 0F8h
        db  0FFh, 0FFh
sub_5_00D4:
        call   ptr [ebp + 0x5f]
        pop    esi
        pop    ebx
        add    esp, 4
        ret    
        db  08Dh, 049h, 000h
sub_5_00E0:
        xchg   esp, eax
        adc    byte ptr [eax], al
        add    byte ptr [edx + 0x10], ah     ; -> loc_4_1062
        add    byte ptr [eax], al
loc_5_00E8:
        rcl    byte ptr [eax], 0
        add    byte ptr [eax + 0x10], dh     ; -> loc_4_1070
        add    byte ptr [eax], al
loc_5_00F0:
        je     0x102
        add    byte ptr [eax], al
        rcl    byte ptr [eax]
        add    byte ptr [eax], al
        lea    esp, [esp]
        lea    esp, [esp]
        sub    esp, 8
        push   ebx
        push   esi
        push   edi
        push   ebp
        lea    eax, [esp + 0x14]
        push   eax
        mov    eax, dword ptr [esp + 0x28]
        push   0
        mov    esi, dword ptr [esp + 0x28]
        push   eax
        push   esi
        call   0xffffff90
        mov    dword ptr [esp + 0x20], eax
        add    esp, 0x10
        or     eax, eax
        jne    loc_5_0132
        sub    eax, eax
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
loc_5_0132:
        mov    ecx, dword ptr [esp + 0x28]
        cmp    dword ptr [ecx], 0
        jne    loc_5_0160
        push   0x24
        push   0
        call   0xa0e
        mov    ecx, dword ptr [esp + 0x30]
        add    esp, 8
        mov    dword ptr [ecx], eax
        or     eax, eax
        jne    loc_5_0160
        mov    eax, 0xfffffffe
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
        db  08Bh, 0FFh
loc_5_0160:
        mov    ecx, dword ptr [esp + 0x28]
        mov    eax, dword ptr [ecx]
        push   eax
        call   0x9e7
        add    esp, 4
        mov    edi, eax
        or     edi, edi
        jne    loc_5_0182
        mov    eax, 0xfffffffe
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
loc_5_0182:
        mov    eax, dword ptr [esp + 0x1c]
        mov    dword ptr [edi + 8], eax
        mov    eax, dword ptr [esp + 0x10]
        mov    eax, dword ptr [eax + 0xc]
        push   eax
        push   esi
        call   0xffffffd0
        add    esp, 8
        mov    dword ptr [edi + 4], eax
        mov    eax, dword ptr [esp + 0x10]
        mov    eax, dword ptr [eax + 0x14]
        mov    dword ptr [edi + 0xc], eax
        mov    eax, dword ptr [esp + 0x10]
        mov    eax, dword ptr [eax + 0x18]
        mov    dword ptr [edi + 0x10], eax
        mov    eax, dword ptr [esp + 0x10]
        mov    eax, dword ptr [eax + 0x10]
        mov    dword ptr [edi + 0x14], eax
        mov    eax, dword ptr [esp + 0x10]
        mov    eax, dword ptr [eax + 0x20]
        sub    ebx, ebx
        push   eax
        push   esi
        call   0xffffffd0
        add    esp, 8
        mov    dword ptr [edi + 0x18], eax
        mov    ebp, eax
        cmp    dword ptr [edi + 0x10], ebx
        jbe    loc_5_01F1
loc_5_01D8:
        mov    eax, dword ptr [ebp]
        push   eax
        push   esi
        call   0xffffffd0
        add    esp, 8
        mov    dword ptr [ebp], eax
        inc    ebx
        add    ebp, 4
        cmp    dword ptr [edi + 0x10], ebx
        ja     loc_5_01D8
loc_5_01F1:
        mov    eax, dword ptr [esp + 0x10]
        mov    eax, dword ptr [eax + 0x24]
        push   eax
        push   esi
        call   0xffffffd0
        add    esp, 8
        mov    dword ptr [edi + 0x1c], eax
        mov    eax, dword ptr [esp + 0x10]
        mov    eax, dword ptr [eax + 0x1c]
loc_5_020C:
        push   eax
        sub    ebx, ebx
        push   esi
        call   0xffffffd0
        add    esp, 8
        mov    dword ptr [edi + 0x20], eax
        mov    ebp, eax
        cmp    dword ptr [edi + 0xc], ebx
        jbe    0x23b
        mov    eax, dword ptr [ebp]
        push   eax
        push   esi
        call   0xffffffd0
loc_5_022C:
        add    esp, 8
        mov    dword ptr [ebp], eax
        inc    ebx
        db  083h
VxD__LDATA ENDS

VxD__LTEXT SEGMENT   ; object 6  vsize=0x210  R-X
loc_6_0000:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    dword ptr [edi], edx
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [edi], dl
        add    dword ptr [eax], eax
        adc    al, 0x20
        add    byte ptr [eax], al
        adc    dword ptr [eax], eax
        add    byte ptr [eax], al
        je     6
        add    byte ptr [eax], al
        pushal 
        and    byte ptr [eax], al
        add    byte ptr [edx], al
        add    byte ptr [eax], al
        add    byte ptr [edi], dh
        add    byte ptr [eax], al
        xor    byte ptr [eax], ah
        add    byte ptr [eax], al
        adc    al, byte ptr [eax]
        add    byte ptr [eax], al
        push   esp
        out    0, eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        je     0x2a
        add    byte ptr [eax], al
        and    byte ptr [eax], ah
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [eax], al
        adc    al, 0xe7
        add    byte ptr [eax], al
        add    al, 0x22
        add    byte ptr [eax], al
        add    eax, 0x34000000
        out    0, eax
        add    byte ptr [eax], ah
        and    al, byte ptr [eax]
        add    byte ptr [esi], al
        add    byte ptr [eax], al
        add    byte ptr [edi], dl
        add    byte ptr [eax + eax], al
        add    byte ptr [edi], al
        add    byte ptr [eax], al
        add    byte ptr [edi], dl
        add    byte ptr [eax + eax], dl
        add    byte ptr [ebx], dl
        add    byte ptr [eax], al
        add    byte ptr [edi], al
        add    byte ptr [eax], al
        add    al, 0
        add    byte ptr [eax], al
        db  0FFh, 0FFh, 0FFh, 0FFh, 054h, 0E7h, 000h, 000h, 044h, 0A0h, 000h, 000h, 008h, 000h, 000h, 000h
        db  014h, 0E7h, 000h, 000h, 014h, 0A0h, 000h, 000h, 014h, 000h, 000h, 000h, 054h, 0E7h, 000h, 000h
        db  004h, 0A0h, 000h, 000h, 009h, 000h, 000h, 000h, 054h, 0E7h, 000h, 000h, 044h, 060h, 000h, 000h
        db  00Bh, 000h, 000h, 000h, 074h, 0E7h, 000h, 000h, 060h, 060h, 000h, 000h, 00Ch, 000h, 000h, 000h
        db  000h, 000h, 008h, 005h, 002h, 00Bh, 055h, 00Bh, 005h, 002h, 005h, 008h, 001h, 000h, 000h
        dd  loc_7_0238               ; fixup
        dd  loc_7_0240               ; fixup
        dd  loc_7_0257               ; fixup
        dd  loc_7_02BE               ; fixup
        dd  loc_7_031B               ; fixup
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
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 02Eh, 064h, 065h, 062h
        db  075h, 067h, 000h, 000h, 049h, 04Eh, 049h, 054h, 000h, 000h, 000h, 000h, 02Eh, 064h, 065h, 062h
        db  075h, 067h, 000h, 000h, 02Eh, 064h, 065h, 062h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 7  vsize=0x1104  R-X
loc_7_0000:
        dec    byte ptr [4]                  ; -> obj5:0x0004
        clc    
        ret    
VXDLDR_LoadDevice:
        pushal 
        push   edx
        call   sub_7_0383
        jb     loc_7_0095
        cmp    byte ptr [0xe4], 0            ; -> obj1:0x00E4
        jne    loc_7_0028
        xor    esi, esi
        call   sub_3_004D
        jb     loc_7_0028
        inc    esi
loc_7_0028:
        mov    edx, dword ptr [esp + 0x18]
        call   sub_7_0FE0
        call   sub_7_033D
        pushfd 
        cmp    byte ptr [0xe4], 0            ; -> obj1:0x00E4
        jne    loc_7_0049
        or     esi, esi
        je     loc_7_0049
        call   sub_3_009E
loc_7_0049:
        popfd  
        jb     loc_7_0088
        or     byte ptr [eax + 0xb], 0x80
        mov    dword ptr [esp + 0x18], edx
        xchg   dword ptr [esp + 0x20], eax
        cmp    byte ptr [edx + 4], 0
        je     loc_7_0069
        inc    byte ptr [edx + 4]
        jne    loc_7_0067
        mov    byte ptr [edx + 4], 0xff
loc_7_0067:
        jmp    loc_7_0088
loc_7_0069:
        call   sub_7_0C5B
        jb     loc_7_00A1
        call   sub_7_0E9B
        mov    edi, dword ptr [esp + 0x20]
        VMMCall  VMM.VMM_Add_DDB                 ; cd20f7000100
        test   al, 1
        je     loc_7_0088
        call   sub_7_0C1B
loc_7_0088:
        pushfd 
        movzx  eax, byte ptr [ebp + 0xe]
        call   sub_7_0AAA
        popfd  
        jae    loc_7_0099
loc_7_0095:
        mov    dword ptr [esp + 0x20], eax
loc_7_0099:
        pop    edx
        call   sub_7_1014
        popal  
        ret    
loc_7_00A1:
        mov    dword ptr [esp + 0x20], ecx
        xor    edi, edi
        xchg   dword ptr [esp + 0x18], edi
        mov    esi, 2
        call   sub_7_0D4C
        call   sub_7_0DB8
        clc    
        jmp    loc_7_0088
VXDLDR_UnloadDevice:
        jmp    loc_7_0C75
sub_7_00C2:
        push   0
        push   edx
        call   sub_7_019D
        je     loc_7_00D1
        mov    edi, dword ptr [edx + 5]
        test   edi, edi
loc_7_00D1:
        ret    
VXDLDR_DevInitSucceeded:
        xor    eax, eax
        pushal 
        call   sub_7_00C2
        jns    loc_7_0141
        or     byte ptr [edi + 0xa], 2
        inc    byte ptr [edx + 4]
        jne    loc_7_00E9
        mov    byte ptr [edx + 4], 0xff
loc_7_00E9:
        cmp    dword ptr [edi + 0x1c], 0
        je     loc_7_0108
        cmp    dword ptr [edi + 0x24], 0
        jne    loc_7_0108
        mov    esi, 0xf71                    ; -> loc_7_0F71
        VMMCall  VMM.Allocate_V86_Call_Back      ; cd200b000100
        jb     loc_7_0108
        mov    dword ptr [edi + 0x24], eax
        mov    dword ptr [edx + 0x1b], eax
loc_7_0108:
        cmp    dword ptr [edi + 0x20], 0
        je     loc_7_0127
        cmp    dword ptr [edi + 0x28], 0
        jne    loc_7_0127
        mov    esi, 0xf8e                    ; -> loc_7_0F8E
        VMMCall  VMM.Allocate_PM_Call_Back       ; cd200c000100
        jb     loc_7_0127
        mov    dword ptr [edi + 0x28], eax
        mov    dword ptr [edx + 0x1f], eax
loc_7_0127:
        VMMCall  VMM.VMM_GetSystemInitState      ; cd2011010100
        cmp    eax, 0x40000000
        jb     loc_7_013E
        mov    edi, edx
        xor    esi, esi
        inc    esi
        call   sub_7_0D55
loc_7_013E:
        clc    
        popal  
        ret    
loc_7_0141:
        popal  
        mov    al, 8
        stc    
        ret    
VXDLDR_DevInitFailed:
        xor    eax, eax
        pushal 
        call   sub_7_00C2
        jns    loc_7_0141
        or     byte ptr [edi + 0xa], 2
        VMMCall  VMM.VMM_Remove_DDB              ; cd20f8000100
        mov    edi, edx
        push   2
        pop    esi
        call   sub_7_0D4C
        call   sub_7_0C08
        call   sub_7_0DB8
        popal  
        clc    
        ret    
        db  061h, 0B0h, 008h, 0C3h
sub_7_0175:
        pushal 
        push   5
        push   dword ptr [0]                 ; -> obj5:0x0000
        VMMCall  VMM._EnterMutex                 ; cd202e010100
        pop    ecx
        pop    ecx
        popal  
        ret    
sub_7_0188:
        pushal 
        push   dword ptr [0]                 ; -> obj5:0x0000
        VMMCall  VMM._LeaveMutex                 ; cd202f010100
        pop    ecx
        popal  
        ret    
VXDLDR_UnloadMe:
        jmp    loc_7_0F18
sub_7_019D:
        mov    edx, dword ptr [esp + 4]
        call   sub_7_0175
        mov    eax, 0xe0                     ; -> obj1:0x00E0
loc_7_01AB:
        mov    ecx, eax
        mov    eax, dword ptr [ecx]
        test   eax, eax
        je     loc_7_01C3
        cmp    eax, edx
        jne    loc_7_01AB
        test   byte ptr [esp + 8], 1
        je     loc_7_01C3
        mov    eax, dword ptr [edx]
        mov    dword ptr [ecx], eax
        inc    eax
loc_7_01C3:
        call   sub_7_0188
        test   eax, eax
        ret    8
sub_7_01CD:
        mov    edx, dword ptr [esp + 4]
        call   sub_7_0175
        mov    ecx, dword ptr [0xe0]         ; -> obj1:0x00E0
        mov    dword ptr [edx], ecx
        mov    dword ptr [0xe0], edx         ; -> obj1:0x00E0
        call   sub_7_0188
        ret    4
        db  0E8h, 084h, 0FFh, 0FFh, 0FFh, 051h, 0B9h
        dd  obj1:0x00E0              ; fixup
        db  08Bh, 009h, 0E3h, 004h, 03Bh, 0D1h, 075h, 0F8h, 0E8h, 084h, 0FFh, 0FFh, 0FFh, 083h, 0F9h, 001h
        db  059h, 0F5h, 0C3h
VXDLDR_PM_API:
        movzx  eax, word ptr [ebp + 0x1c]
        cmp    eax, 2
        ja     loc_7_0232
        call   sub_5_00D4
        jb     loc_7_0229
        mov    word ptr [ebp + 0x1c], 0
        and    byte ptr [ebp + 0x2c], 0xfe
        ret    
loc_7_0229:
        mov    word ptr [ebp + 0x1c], ax
        or     byte ptr [ebp + 0x2c], 1
        ret    
loc_7_0232:
        mov    ax, 0xb
        jmp    loc_7_0229
loc_7_0238:
        mov    word ptr [ebp + 0x14], 0x100
        clc    
        ret    
loc_7_0240:
        mov    ax, 0x3c14
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   edx, eax
        mov    eax, 1
        VxDCall  APM.Service_0001h               ; cd2001002700
        ret    
loc_7_0257:
        mov    ax, 0x3c14
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   edx, eax
        mov    bx, word ptr [ebp + 0x10]
        VxDCall  APM.Service_0002h               ; cd2002002700
        ret    
loc_7_026D:
        jecxz  loc_7_02B6
        push   ebx
        push   ecx
        push   edx
        push   esi
        dec    ecx
        cmp    ecx, 1
        ja     loc_7_02A2
        call   sub_5_00E0
        mov    dword ptr [esi + 0x20], ecx
        jb     loc_7_028A
        xor    eax, eax
loc_7_028A:
        pushfd 
        cmp    eax, 0xb
        jbe    loc_7_0295
        mov    eax, 0xb
loc_7_0295:
        movzx  eax, byte ptr [eax + 0xc6]    ; -> obj5:0x00C6
        popfd  
        pop    esi
        pop    edx
        pop    ecx
        pop    ebx
        ret    
loc_7_02A2:
        mov    edx, dword ptr [esi + 0x18]
        mov    eax, 0xb
        mov    word ptr [edx], ax
        mov    dword ptr [esi + 0x20], 2
        jmp    loc_7_028A
loc_7_02B6:
        xor    eax, eax
        mov    ecx, 0x100
        ret    
loc_7_02BE:
        push   esi
        push   edi
        mov    edx, dword ptr [esi + 0x10]
        mov    eax, 1
        VxDCall  APM.Service_0001h               ; cd2001002700
        mov    edi, dword ptr [esi + 0x18]
        mov    word ptr [edi], ax
        mov    ecx, 2
        jb     loc_7_0308
        mov    word ptr [edi], 0
        or     edx, edx
        je     loc_7_030B
        mov    ax, word ptr [edx + 9]
        mov    word ptr [edi + 2], ax
        mov    eax, dword ptr [esi + 0x1c]
        mov    esi, dword ptr [edx + 0xb]
        mov    ecx, 4
        lea    edi, [edi + 4]
loc_7_02FA:
        cmp    eax, ecx
        jbe    loc_7_0306
        movsb  byte ptr es:[edi], byte ptr [esi]
        inc    ecx
        cmp    byte ptr [esi - 1], 0
        jne    loc_7_02FA
loc_7_0306:
        xor    eax, eax
loc_7_0308:
        pop    edi
        pop    esi
        ret    
loc_7_030B:
        xor    eax, eax
        mov    ecx, 5
        mov    word ptr [edi + 2], ax
        mov    byte ptr [edi + 4], al
        jmp    loc_7_0308
loc_7_031B:
        push   edi
        mov    edx, dword ptr [esi + 0x10]
        mov    bx, word ptr [edx + 2]
        lea    edx, [edx + 4]
        VxDCall  APM.Service_0002h               ; cd2002002700
        mov    edi, dword ptr [esi + 0x18]
        jb     loc_7_0333
        xor    eax, eax
loc_7_0333:
        mov    word ptr [edi], ax
        mov    ecx, 2
        pop    edi
        ret    
sub_7_033D:
        push   esi
        call   sub_7_03A0
        jb     loc_7_037A
        call   sub_7_03CD
        jb     loc_7_037A
        call   sub_7_0599
        jb     loc_7_037A
        call   sub_7_064C
        jb     loc_7_0376
        call   sub_7_065F
        jb     loc_7_037A
        call   sub_7_0730
        jb     loc_7_037A
        call   sub_7_075A
        jb     loc_7_037A
        call   sub_7_0ACA
        jb     loc_7_037A
loc_7_0376:
        clc    
        mov    eax, dword ptr [edx + 5]
loc_7_037A:
        pushfd 
        call   sub_7_03B7
        popfd  
        pop    esi
        ret    
sub_7_0383:
        push   1
        push   0x417
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        pop    ecx
        pop    ecx
        or     eax, eax
        je     loc_7_039D
        mov    ebp, eax
        dec    word ptr [ebp]
        ret    
loc_7_039D:
        inc    eax
        stc    
        ret    
sub_7_03A0:
        lea    edi, [ebp + 0x313]
        VMMCall  VMM.OpenFile                    ; cd20b8000100
        mov    byte ptr [ebp + 0xe], 3
        jb     loc_7_03B6
        mov    word ptr [ebp], ax
loc_7_03B6:
        ret    
sub_7_03B7:
        push   eax
        mov    bx, word ptr [ebp]
        cmp    bx, -1
        je     loc_7_03CB
        mov    ax, 0x3e00
        call   sub_7_0FD3
loc_7_03CB:
        pop    eax
        ret    
sub_7_03CD:
        mov    byte ptr [ebp + 0xe], 4
        mov    ax, 0x3f00
        mov    bx, word ptr [ebp]
        mov    ecx, 0x40
        lea    edx, [ebp + 0xd3]
        call   sub_7_0FAF
        jb     loc_7_04B6
        cmp    ax, 0x40
        jne    loc_7_04B6
        mov    byte ptr [ebp + 0xe], 6
        cmp    word ptr [ebp + 0xd3], 0x5a4d
        jne    loc_7_04B6
        mov    edx, dword ptr [ebp + 0x10f]
loc_7_0412:
        mov    byte ptr [ebp + 0xe], 4
        mov    ecx, edx
        shr    ecx, 0x10
        and    edx, 0xffff
        mov    ax, 0x4200
        call   sub_7_0FBE
        jb     loc_7_04B6
        mov    word ptr [ebp + 0xa], ax
        mov    word ptr [ebp + 0xc], dx
        mov    ax, 0x3f00
        mov    ecx, 0xc4
        lea    edx, [ebp + 0xf]
        call   sub_7_0FAF
        jb     loc_7_04B6
        cmp    ax, 0xc4
        jne    loc_7_04B6
        mov    byte ptr [ebp + 0xe], 6
        cmp    word ptr [ebp + 0xf], 0x454c
        jne    loc_7_04B8
        cmp    word ptr [ebp + 0x19], 4
        jne    loc_7_04B6
        cmp    word ptr [ebp + 0x17], 2
        jb     loc_7_04B6
        mov    eax, dword ptr [ebp + 0x1f]
        test   eax, 0x8000
        je     loc_7_04B6
        and    eax, 0x38000
        cmp    eax, 0x38000
        jne    loc_7_04B6
        cmp    word ptr [ebp + 0xd1], 0x400
        ja     loc_7_04B6
        cmp    word ptr [ebp + 0xd1], 0x300
        jb     loc_7_04B6
        mov    eax, dword ptr [ebp + 0x307]
        add    dword ptr [ebp + 0x8f], eax
        clc    
        ret    
loc_7_04A5:
        mov    eax, dword ptr [ebp + 0x30b]
        push   0
        push   eax
        VMMCall  VMM._HeapFree                   ; cd2051000100
        pop    ecx
        pop    ecx
loc_7_04B6:
        stc    
        ret    
loc_7_04B8:
        cmp    dword ptr [ebp + 0x307], 0
        jne    loc_7_04B6
        cmp    word ptr [ebp + 0xf], 0x454e
        jne    loc_7_04B6
        mov    ax, word ptr [ebp + 0x33]
        mov    dx, ax
        sub    dx, word ptr [ebp + 0x35]
        je     loc_7_04B6
        neg    dx
        push   edx
        mov    dx, 0xff3c
        mov    cx, 0xffff
        add    dx, ax
        adc    cx, 0
        mov    ax, 0x4201
        call   sub_7_0FBE
        pop    ecx
        jb     loc_7_04B6
        movzx  ecx, cx
        push   ecx
        push   0
        push   ecx
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        pop    ecx
        pop    ecx
        pop    ecx
        or     eax, eax
        je     loc_7_04B6
        mov    dword ptr [ebp + 0x30b], eax
        mov    edx, eax
        push   edx
        mov    ax, 0x3f00
        call   sub_7_0FAF
        pop    ebx
        jb     loc_7_04A5
        cmp    ax, cx
        jne    loc_7_04A5
        add    ebx, 2
loc_7_0526:
        movzx  eax, word ptr [ebx]
        or     eax, eax
        je     loc_7_04A5
        movzx  ecx, word ptr [ebx + 2]
        add    ebx, 8
        test   ah, 0x80
        je     loc_7_0545
        and    ah, 0x7f
        cmp    eax, 0x14
        je     loc_7_054C
loc_7_0545:
        imul   ecx, ecx, 0xc
        add    ebx, ecx
        jmp    loc_7_0526
loc_7_054C:
        or     ecx, ecx
        je     loc_7_04A5
loc_7_0554:
        mov    ax, word ptr [ebx + 6]
        test   ah, 0x80
        je     loc_7_0565
        and    ah, 0x7f
        cmp    eax, 1
        je     loc_7_056F
loc_7_0565:
        add    ebx, 0xc
        loop   loc_7_0554
        jmp    loc_7_04A5
loc_7_056F:
        movzx  edx, word ptr [ebx]
        mov    ebx, dword ptr [ebp + 0x30b]
        mov    cx, word ptr [ebx]
        shl    edx, cl
        mov    dword ptr [ebp + 0x307], edx
        push   edx
        push   0
        push   ebx
        VMMCall  VMM._HeapFree                   ; cd2051000100
        pop    ecx
        pop    ecx
        pop    edx
        mov    bx, word ptr [ebp]
        jmp    loc_7_0412
sub_7_0599:
        mov    dword ptr [ebp + 2], 0
        mov    eax, dword ptr [ebp + 0x8f]
        sub    eax, dword ptr [ebp + 0xa]
        mov    dword ptr [ebp + 6], eax
        mov    byte ptr [ebp + 0xe], 1
        push   1
        push   eax
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        pop    ecx
        pop    ecx
        or     eax, eax
        je     loc_7_0636
        mov    dword ptr [ebp + 2], eax
        mov    byte ptr [ebp + 0xe], 4
        xor    edx, edx
        mov    ecx, edx
        mov    dx, word ptr [ebp + 0xa]
        mov    cx, word ptr [ebp + 0xc]
        mov    bx, word ptr [ebp]
        mov    ax, 0x4200
        call   sub_7_0FBE
        jb     loc_7_0636
        mov    edx, dword ptr [ebp + 2]
        mov    ecx, dword ptr [ebp + 6]
        mov    ax, 0x3f00
        call   sub_7_0FAF
        cmp    eax, dword ptr [ebp + 6]
        jne    loc_7_0636
        mov    esi, dword ptr [ebp + 0x67]
        add    esi, dword ptr [ebp + 2]
        cmp    byte ptr [esi], 0
        je     loc_7_0636
        mov    byte ptr [ebp + 0xe], 6
        mov    esi, dword ptr [ebp + 0x6b]
        add    esi, dword ptr [ebp + 2]
        cmp    byte ptr [esi], 0
        je     loc_7_0636
        mov    al, byte ptr [esi + 1]
        and    al, 0x7f
        cmp    al, 3
        jne    loc_7_0636
        movzx  eax, word ptr [esi + 2]
        cmp    eax, dword ptr [ebp + 0x53]
        ja     loc_7_0636
        mov    word ptr [ebp + 0x301], ax
        add    esi, 4
        mov    eax, dword ptr [esi + 1]
        mov    dword ptr [ebp + 0x303], eax
        clc    
        ret    
loc_7_0636:
        xor    eax, eax
        xchg   dword ptr [ebp + 2], eax
        or     eax, eax
        je     loc_7_064A
        push   0
        push   eax
        VMMCall  VMM._HeapFree                   ; cd2051000100
        pop    ecx
        pop    ecx
loc_7_064A:
        stc    
        ret    
sub_7_064C:
        call   sub_7_0E64
        jb     loc_7_065D
        cmp    byte ptr [edi + 4], 0
        je     loc_7_065D
        mov    edx, edi
        stc    
        ret    
loc_7_065D:
        clc    
        ret    
sub_7_065F:
        lea    edi, [ebp + 0x113]
        mov    ecx, dword ptr [ebp + 0x53]
        mov    esi, dword ptr [ebp + 0x4f]
        add    esi, dword ptr [ebp + 2]
        xor    edx, edx
loc_7_0670:
        inc    edx
        mov    eax, dword ptr [esi + 8]
        push   ecx
        push   esi
        call   sub_7_0711
        pop    esi
        pop    ecx
        mov    byte ptr [ebp + 0xe], 6
        jb     loc_7_06E1
        cmp    word ptr [ebp + 0x301], dx
        jne    loc_7_0696
        cmp    eax, 3
        jb     loc_7_0696
        cmp    eax, 6
        jbe    loc_7_06E1
loc_7_0696:
        cmp    eax, 0xb
        je     loc_7_06C5
        cmp    eax, 0xc
        je     loc_7_06C5
        cmp    eax, -1
        mov    dword ptr [edi + 8], eax
        je     loc_7_06BA
loc_7_06A8:
        mov    byte ptr [ebp + 0xe], 1
        call   sub_7_0DE7
        jb     loc_7_06E1
        mov    dword ptr [edi], eax
        mov    eax, dword ptr [esi]
        mov    dword ptr [edi + 4], eax
loc_7_06BA:
        add    edi, 0x10
        add    esi, 0x18
        dec    ecx
        jne    loc_7_0670
        clc    
        ret    
loc_7_06C5:
        mov    dword ptr [edi + 8], eax
        cmp    dword ptr [esi], 0
        je     loc_7_06D9
        test   dword ptr [0x6d3], 0x400000   ; -> obj7:0x06D3
        jne    loc_7_06A8
loc_7_06D9:
        mov    dword ptr [edi], 0xffc00000
        jmp    loc_7_06BA
loc_7_06E1:
        sub    ecx, dword ptr [ebp + 0x53]
        neg    ecx
        jecxz  loc_7_070F
        lea    esi, [ebp + 0x113]
loc_7_06EE:
        cmp    dword ptr [esi + 8], -1
        je     loc_7_070A
        cmp    dword ptr [esi + 0xc], 0
        jne    loc_7_070A
        push   ecx
        push   0
        push   dword ptr [esi]
        VMMCall  VMM._HeapFree                   ; cd2051000100
        pop    ecx
        pop    ecx
        pop    ecx
        or     eax, eax
loc_7_070A:
        add    esi, 0x10
        loop   loc_7_06EE
loc_7_070F:
        stc    
        ret    
sub_7_0711:
        mov    esi, 5                        ; -> obj5:0x0005
        mov    ecx, 0x10
loc_7_071B:
        push   eax
        and    eax, dword ptr [esi]
        cmp    eax, dword ptr [esi + 4]
        pop    eax
        je     loc_7_072B
        add    esi, 0xc
        loop   loc_7_071B
        stc    
        ret    
loc_7_072B:
        mov    eax, dword ptr [esi + 8]
        clc    
        ret    
sub_7_0730:
        mov    ecx, dword ptr [ebp + 0x83]
        jecxz  loc_7_0758
        mov    esi, dword ptr [ebp + 0x7f]
        add    esi, dword ptr [ebp + 2]
        cld    
        xor    ebx, ebx
loc_7_0741:
        VMMCall  VMM.VMMAddImportModuleName      ; cd20f6000100
        mov    dword ptr [ebx + ebp + 0x2a3], eax
        xor    eax, eax
        lodsb  al, byte ptr [esi]
        add    esi, eax
        add    ebx, 4
        loop   loc_7_0741
loc_7_0758:
        clc    
        ret    
sub_7_075A:
        mov    esi, dword ptr [ebp + 0x4f]
        add    esi, dword ptr [ebp + 2]
        mov    ecx, dword ptr [ebp + 0x53]
        lea    edi, [ebp + 0x113]
        mov    byte ptr [ebp + 0xe], 6
        xor    edx, edx
loc_7_076F:
        inc    edx
        cmp    dword ptr [edi + 8], -1
        je     loc_7_079D
        cmp    dword ptr [edi + 8], 0xb
        je     loc_7_07D3
        cmp    dword ptr [edi + 8], 0xc
        je     loc_7_07D3
loc_7_0782:
        mov    eax, dword ptr [edi + 0xc]
        mov    dword ptr [ebp + 0x2e3], eax
        push   edi
        push   edx
        call   sub_7_080A
        pop    edx
        pop    edi
        jb     loc_7_07A7
        call   sub_7_07E1
        jb     loc_7_07A7
loc_7_079D:
        add    esi, 0x18
        add    edi, 0x10
        loop   loc_7_076F
        clc    
        ret    
loc_7_07A7:
        lea    esi, [ebp + 0x113]
        mov    ecx, dword ptr [ebp + 0x53]
loc_7_07B0:
        cmp    dword ptr [esi + 8], -1
        je     loc_7_07CA
        cmp    dword ptr [esi + 0xc], 0
        jne    loc_7_07CA
        push   ecx
        push   0
        push   dword ptr [esi]
        VMMCall  VMM._HeapFree                   ; cd2051000100
        pop    ecx
        pop    ecx
        pop    ecx
loc_7_07CA:
        add    esi, 0x10
        or     eax, eax
        loop   loc_7_07B0
        stc    
        ret    
loc_7_07D3:
        test   dword ptr [0x7d9], 0x400000   ; -> obj7:0x07D9
        je     loc_7_079D
        jmp    loc_7_0782
sub_7_07E1:
        cmp    word ptr [ebp + 0xd1], 0x400
        jne    loc_7_0808
        cmp    word ptr [ebp + 0x301], dx
        jne    loc_7_0808
        mov    eax, dword ptr [ebp + 0x303]
        add    eax, dword ptr [edi]
        cmp    dword ptr [eax + 0x3c], 0x50726576
        je     loc_7_0808
        stc    
        ret    
loc_7_0808:
        clc    
        ret    
sub_7_080A:
        push   ecx
        mov    ebx, dword ptr [ebp + 0x57]
        add    ebx, dword ptr [ebp + 2]
        mov    ecx, dword ptr [esi + 0x10]
        mov    eax, dword ptr [esi + 0xc]
        dec    eax
        lea    ebx, [ebx + eax*4]
        mov    edx, dword ptr [edi + 4]
        mov    dword ptr [ebp + 0x30f], edx
        mov    edi, dword ptr [edi]
        cmp    dword ptr [ebp + 0x2e3], 0
        jne    loc_7_0836
        call   sub_7_083D
        jb     loc_7_083B
loc_7_0836:
        call   sub_7_089A
loc_7_083B:
        pop    ecx
        ret    
sub_7_083D:
        push   ebx
        push   ecx
        push   esi
        push   edi
loc_7_0841:
        jecxz  loc_7_087D
        mov    al, byte ptr [ebx + 3]
        and    al, 3
        cmp    al, 0
        je     loc_7_0890
        cmp    al, 3
        jne    loc_7_0897
        push   ecx
        call   sub_7_094F
        mov    ecx, dword ptr [ebp + 0x37]
        jne    loc_7_085E
        mov    ecx, dword ptr [ebp + 0x3b]
loc_7_085E:
        cmp    ecx, dword ptr [ebp + 0x30f]
        jbe    loc_7_086C
        mov    ecx, dword ptr [ebp + 0x30f]
loc_7_086C:
        sub    dword ptr [ebp + 0x30f], ecx
        cld    
        xor    al, al
        rep stosb byte ptr es:[edi], al
        pop    ecx
        add    ebx, 4
        loop   loc_7_0841
loc_7_087D:
        mov    ecx, dword ptr [ebp + 0x30f]
        jecxz  loc_7_088A
        xor    al, al
        cld    
        rep stosb byte ptr es:[edi], al
loc_7_088A:
        clc    
loc_7_088B:
        pop    edi
        pop    esi
        pop    ecx
        pop    ebx
        ret    
loc_7_0890:
        call   sub_7_08D0
        jae    loc_7_0841
loc_7_0897:
        stc    
        jmp    loc_7_088B
sub_7_089A:
        push   ebx
        push   esi
        jecxz  loc_7_08C9
loc_7_089E:
        call   sub_7_094F
        dec    eax
        js     loc_7_08C7
        mov    esi, dword ptr [ebp + 2]
        add    esi, dword ptr [ebp + 0x77]
        lea    esi, [esi + eax*4]
        mov    edx, dword ptr [esi + 4]
        mov    esi, dword ptr [esi]
        add    edx, dword ptr [ebp + 0x7b]
        add    esi, dword ptr [ebp + 0x7b]
        call   sub_7_095F
        jb     loc_7_08CD
        add    edi, dword ptr [ebp + 0x37]
        add    ebx, 4
loc_7_08C7:
        loop   loc_7_089E
loc_7_08C9:
        clc    
loc_7_08CA:
        pop    esi
        pop    ebx
        ret    
loc_7_08CD:
        stc    
        jmp    loc_7_08CA
sub_7_08D0:
        push   ebx
        call   sub_7_094F
        mov    esi, eax
        xor    edx, edx
loc_7_08DA:
        call   sub_7_094F
        cmp    esi, eax
        jne    loc_7_08FF
        mov    al, byte ptr [ebx + 3]
        and    al, 3
        cmp    al, 0
        jne    loc_7_08FF
        cmp    dword ptr [ebp + 0x23], esi
        mov    eax, dword ptr [ebp + 0x37]
        jne    loc_7_08F7
        mov    eax, dword ptr [ebp + 0x3b]
loc_7_08F7:
        add    edx, eax
        inc    esi
        add    ebx, 4
        loop   loc_7_08DA
loc_7_08FF:
        pop    esi
        push   ebx
        push   ecx
        push   edx
        mov    ebx, esi
        call   sub_7_094F
        dec    eax
        mul    dword ptr [ebp + 0x37]
        add    eax, dword ptr [ebp + 0x8f]
        xchg   edx, eax
        shld   ecx, edx, 0x10
        mov    ax, 0x4200
        mov    bx, word ptr [ebp]
        call   sub_7_0FBE
        pop    edx
        mov    ecx, edx
        cmp    ecx, dword ptr [ebp + 0x30f]
        jbe    loc_7_0937
        mov    ecx, dword ptr [ebp + 0x30f]
loc_7_0937:
        sub    dword ptr [ebp + 0x30f], ecx
        mov    edx, edi
        add    edi, ecx
        mov    ah, 0x3f
        call   sub_7_0FAF
        jb     loc_7_094C
        cmp    eax, ecx
loc_7_094C:
        pop    ecx
        pop    ebx
        ret    
sub_7_094F:
        movzx  eax, byte ptr [ebx]
        shl    eax, 0x10
        mov    ax, word ptr [ebx + 1]
        xchg   al, ah
        cmp    dword ptr [ebp + 0x23], eax
        ret    
sub_7_095F:
        push   ebx
        push   ecx
        add    esi, dword ptr [ebp + 2]
        add    edx, dword ptr [ebp + 2]
        cld    
loc_7_0968:
        push   edx
        cmp    esi, edx
        jae    loc_7_09D9
        lodsw  ax, word ptr [esi]
        mov    dx, ax
        shl    edx, 0x10
        xor    dx, dx
        lodsb  al, byte ptr [esi]
        mov    dl, al
        test   edx, 0x200000
        jne    loc_7_0986
        lodsb  al, byte ptr [esi]
        mov    dh, al
loc_7_0986:
        xor    ebx, ebx
        lodsb  al, byte ptr [esi]
        mov    bl, al
        test   edx, 0x40000000
        je     loc_7_0996
        lodsb  al, byte ptr [esi]
        mov    bh, al
loc_7_0996:
        mov    eax, edx
        and    eax, 0x3000000
        cmp    eax, 0
        jne    loc_7_09AB
        call   sub_7_09E3
        jmp    loc_7_09B7
loc_7_09A9:
        jmp    loc_7_09DE
loc_7_09AB:
        cmp    eax, 0x1000000
        jne    loc_7_09A9
        call   sub_7_09F8
loc_7_09B7:
        mov    ecx, 1
        test   edx, 0x200000
        je     loc_7_09CD
        movzx  ecx, dx
loc_7_09C7:
        mov    dx, word ptr [esi]
        add    esi, 2
loc_7_09CD:
        call   sub_7_0A41
        jb     loc_7_09DE
        loop   loc_7_09C7
        pop    edx
        jmp    loc_7_0968
loc_7_09D9:
        pop    edx
        pop    ecx
        pop    ebx
        clc    
        ret    
loc_7_09DE:
        pop    edx
        pop    ecx
        pop    ebx
        stc    
        ret    
sub_7_09E3:
        xor    eax, eax
        lodsw  ax, word ptr [esi]
        test   edx, 0x10000000
        je     loc_7_09F7
        ror    eax, 0x10
        lodsw  ax, word ptr [esi]
        ror    eax, 0x10
loc_7_09F7:
        ret    
sub_7_09F8:
        xor    eax, eax
        lodsb  al, byte ptr [esi]
        test   edx, 0x80000000
        jne    loc_7_0A1C
        ror    ax, 8
        lodsb  al, byte ptr [esi]
        ror    ax, 8
        test   edx, 0x10000000
        je     loc_7_09F7
        ror    eax, 0x10
        lodsw  ax, word ptr [esi]
        ror    eax, 0x10
loc_7_0A1C:
        cmp    eax, 2
        xor    eax, eax
        test   edx, 0x4000000
        je     loc_7_0A3B
        lodsw  ax, word ptr [esi]
        test   edx, 0x20000000
        je     loc_7_0A3B
        ror    eax, 0x10
        lodsw  ax, word ptr [esi]
        ror    eax, 0x10
loc_7_0A3B:
        test   eax, 0xffff0000
        ret    
sub_7_0A41:
        push   eax
        push   ebx
        push   edx
        push   esi
        cmp    ebx, dword ptr [ebp + 0x53]
        ja     loc_7_0A70
        movsx  esi, dx
        shr    edx, 0x10
        and    dh, 3
        cmp    dh, 0
        jne    loc_7_0A8B
        dec    ebx
        shl    ebx, 4
        add    eax, dword ptr [ebx + ebp + 0x113]
        and    dl, 0xf
        cmp    dl, 7
        je     loc_7_0A76
        cmp    dl, 8
        je     loc_7_0A7B
loc_7_0A70:
        stc    
        pop    esi
        pop    edx
        pop    ebx
        pop    eax
        ret    
loc_7_0A76:
        mov    dword ptr [esi + edi], eax
        jmp    loc_7_0A85
loc_7_0A7B:
        sub    eax, edi
        sub    eax, esi
        sub    eax, 4
        mov    dword ptr [esi + edi], eax
loc_7_0A85:
        clc    
        pop    esi
        pop    edx
        pop    ebx
        pop    eax
        ret    
loc_7_0A8B:
        cmp    ebx, dword ptr [ebp + 0x83]
        ja     loc_7_0A70
        mov    ebx, dword ptr [ebp + ebx*4 + 0x29f]
        or     bx, 0xf000
        mov    word ptr [esi + edi], ax
        mov    word ptr [esi + edi + 2], bx
        jmp    loc_7_0A85
sub_7_0AAA:
        push   eax
        mov    eax, dword ptr [ebp + 2]
        or     eax, eax
        je     loc_7_0ABD
        push   0
        push   eax
        VMMCall  VMM._HeapFree                   ; cd2051000100
        pop    ecx
        pop    ecx
loc_7_0ABD:
        push   0
        push   ebp
        VMMCall  VMM._HeapFree                   ; cd2051000100
        pop    ecx
        pop    ecx
        pop    eax
        ret    
sub_7_0ACA:
        call   sub_7_0E64
        jb     loc_7_0AF9
        cmp    dword ptr [edi + 0x17], 0
        je     loc_7_0AE9
        mov    byte ptr [ebp + 0xe], 6
        mov    eax, dword ptr [ebp + 0x53]
        cmp    dword ptr [edi + 0x13], eax
        jne    loc_7_0BD1
        jmp    loc_7_0B5D
loc_7_0AE9:
        call   sub_7_0BD3
        je     loc_7_0BD1
        mov    dword ptr [edi + 0x17], eax
        jmp    loc_7_0B5D
loc_7_0AF9:
        mov    byte ptr [ebp + 0xe], 1
        push   1
        push   0x23
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        pop    ecx
        pop    ecx
        or     eax, eax
        mov    edi, eax
        je     loc_7_0BAC
        mov    dword ptr [edi + 0xf], 0x444c5658
        mov    eax, dword ptr [ebp + 0x53]
        mov    dword ptr [edi + 0x13], eax
        call   sub_7_0BD3
        je     loc_7_0BAC
        mov    dword ptr [edi + 0x17], eax
        mov    esi, dword ptr [ebp + 0x67]
        add    esi, dword ptr [ebp + 2]
        movzx  ecx, byte ptr [esi]
        push   ecx
        inc    ecx
        push   1
        push   ecx
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        pop    ecx
        pop    ecx
        pop    ecx
        mov    dword ptr [edi + 0xb], eax
        or     eax, eax
        je     loc_7_0BAC
        push   edi
        inc    esi
        mov    edi, eax
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        xor    al, al
        stosb  byte ptr es:[edi], al
        pop    edi
        push   edi
        call   sub_7_01CD
loc_7_0B5D:
        push   edi
        mov    ax, word ptr [ebp + 0xcf]
        mov    word ptr [edi + 9], ax
        mov    esi, dword ptr [ebp + 0x6b]
        add    esi, dword ptr [ebp + 2]
        movzx  eax, word ptr [esi + 2]
        add    esi, 4
        mov    esi, dword ptr [esi + 1]
        dec    eax
        imul   eax, eax, 0x10
        add    eax, ebp
        add    eax, 0x113
        add    esi, dword ptr [eax]
        mov    dword ptr [edi + 5], esi
        mov    eax, dword ptr [edi + 0x1b]
        mov    dword ptr [esi + 0x24], eax
        mov    eax, dword ptr [edi + 0x1f]
        mov    dword ptr [esi + 0x28], eax
        mov    eax, dword ptr [ebp + 0x53]
        imul   eax, eax, 0x10
        mov    ecx, eax
        lea    esi, [ebp + 0x113]
        mov    edi, dword ptr [edi + 0x17]
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    edx
        clc    
        ret    
loc_7_0BAC:
        or     edi, edi
        je     loc_7_0BD1
        cmp    dword ptr [edi + 0x17], 0
        je     loc_7_0BC3
        push   0
        push   dword ptr [edi + 0x17]
        VMMCall  VMM._HeapFree                   ; cd2051000100
        pop    ecx
        pop    ecx
loc_7_0BC3:
        inc    dword ptr [edi + 0xf]
        push   0
        push   edi
        VMMCall  VMM._HeapFree                   ; cd2051000100
        pop    ecx
        pop    ecx
loc_7_0BD1:
        stc    
        ret    
sub_7_0BD3:
        mov    eax, dword ptr [ebp + 0x53]
        mov    ebx, 0x10
        mul    ebx
        or     edx, edx
        jne    loc_7_0BEF
        push   1
        push   eax
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        pop    ecx
        pop    ecx
        or     eax, eax
        ret    
loc_7_0BEF:
        xor    eax, eax
        ret    
sub_7_0BF2:
        mov    ecx, dword ptr [edi + 0x13]
        mov    ebx, dword ptr [edi + 0x17]
loc_7_0BF8:
        mov    eax, dword ptr [ebx + 8]
        cmp    al, 5
        je     loc_7_0C1A
        cmp    al, 6
        je     loc_7_0C1A
        add    ebx, 0x10
        loop   loc_7_0BF8
sub_7_0C08:
        xor    eax, eax
        xchg   dword ptr [edi + 0x17], eax
        push   edx
        push   0
        push   eax
        VMMCall  VMM._HeapFree                   ; cd2051000100
        pop    ecx
        pop    ecx
        pop    edx
loc_7_0C1A:
        ret    
sub_7_0C1B:
        call   sub_7_107D
        mov    eax, 0x1b
        pushal 
        mov    ecx, edi
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        VMMCall  VMM.Directed_Sys_Control        ; cd2047010100
        popal  
        jb     loc_7_0C4A
        call   sub_7_10A5
        VxDCall  APM.Service_0003h               ; cd2003002700
        clc    
        ret    
loc_7_0C4A:
        call   sub_7_10A5
        VxDCall  APM.Service_0004h               ; cd2004002700
        mov    byte ptr [ebp + 0xe], 7
        stc    
        ret    
sub_7_0C5B:
        push   edi
        push   eax
        mov    edi, dword ptr [edx + 5]
        movzx  eax, word ptr [edi + 6]
        lea    edi, [edi + 0xc]
        VMMCall  VMM.Get_DDB                     ; cd2046010100
        or     ecx, ecx
        je     loc_7_0C72
        stc    
loc_7_0C72:
        pop    eax
        pop    edi
        ret    
loc_7_0C75:
        pushal 
        call   sub_7_0CFA
        mov    eax, 8
        jb     loc_7_0CF3
        cmp    byte ptr [edi + 4], 0
        je     loc_7_0CF3
        mov    edx, edi
        mov    edi, dword ptr [edx + 5]
        mov    eax, 9
        cmp    dword ptr [edi + 0x30], 0
        je     loc_7_0CA0
        test   word ptr [edi + 0xa], 0x2000
        jne    loc_7_0CF3
loc_7_0CA0:
        cmp    byte ptr [edx + 4], 1
        jne    loc_7_0CED
        mov    eax, 0x1c
        pushal 
        mov    ecx, edi
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        VMMCall  VMM.Directed_Sys_Control        ; cd2047010100
        popal  
        mov    eax, 7
        jb     loc_7_0CF3
        VMMCall  VMM.VMM_Remove_DDB              ; cd20f8000100
        mov    eax, 8
        jb     loc_7_0CF3
        xor    esi, esi
        mov    edi, edx
        dec    byte ptr [edx + 4]
        call   sub_7_0D4C
        call   sub_7_0BF2
        call   sub_7_0DB8
        jmp    loc_7_0CF0
loc_7_0CED:
        dec    byte ptr [edx + 4]
loc_7_0CF0:
        popal  
        clc    
        ret    
loc_7_0CF3:
        mov    dword ptr [esp + 0x1c], eax
        popal  
        stc    
        ret    
sub_7_0CFA:
        call   sub_7_0175
        mov    esi, 0xd3b                    ; -> loc_7_0D3B
        or     bx, bx
        je     loc_7_0D0F
        cmp    bx, -1
        jne    loc_7_0D21
loc_7_0D0F:
        mov    esi, 0xd40                    ; -> loc_7_0D40
        mov    edi, edx
        cld    
        xor    ecx, ecx
        dec    ecx
        xor    al, al
        repne scasb al, byte ptr es:[edi]
        neg    ecx
        dec    ecx
loc_7_0D21:
        mov    eax, 0xe0                     ; -> obj1:0x00E0
loc_7_0D26:
        mov    eax, dword ptr [eax]
        or     eax, eax
        je     loc_7_0D32
        call   esi
        jne    loc_7_0D26
        mov    edi, eax
loc_7_0D32:
        call   sub_7_0188
        cmp    eax, 1
        ret    
loc_7_0D3B:
        cmp    word ptr [eax + 9], bx
        ret    
loc_7_0D40:
        push   esi
        push   ecx
        mov    edi, edx
        mov    esi, dword ptr [eax + 0xb]
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        pop    ecx
        pop    esi
        ret    
sub_7_0D4C:
        xor    eax, eax
        xchg   dword ptr [edi + 5], eax
        and    dword ptr [eax + 0xc], 0
sub_7_0D55:
        pushal 
        mov    ecx, dword ptr [edi + 0x13]
        mov    ebx, dword ptr [edi + 0x17]
        xor    edx, edx
loc_7_0D5E:
        inc    edx
        cmp    dword ptr [ebx + 8], -1
        je     loc_7_0DB1
        xor    eax, eax
        cmp    dword ptr [ebx + 8], 0x11
        jae    loc_7_0D6E
        inc    eax
loc_7_0D6E:
        xor    eax, esi
        je     loc_7_0DB1
        cmp    dword ptr [ebx + 8], 5
        je     loc_7_0DB1
        cmp    dword ptr [ebx + 8], 6
        je     loc_7_0DB1
        xor    eax, eax
        VxDCall  APM.Service_000Fh               ; cd200f002700
        cmp    byte ptr [0xc5], 0            ; -> obj5:0x00C5
        je     loc_7_0DA1
        pushal 
        mov    bx, dx
        mov    edi, dword ptr [edi + 0xb]
        mov    dx, ds
        mov    eax, 0x152
        int    0x41
        popal  
loc_7_0DA1:
        push   ecx
        push   edx
        push   0
        push   dword ptr [ebx]
        VMMCall  VMM._HeapFree                   ; cd2051000100
        pop    ecx
        pop    ecx
        pop    edx
        pop    ecx
loc_7_0DB1:
        add    ebx, 0x10
        loop   loc_7_0D5E
        popal  
        ret    
sub_7_0DB8:
        mov    eax, dword ptr [edi + 0x1b]
        or     eax, dword ptr [edi + 0x1f]
        or     eax, dword ptr [edi + 0x17]
        jne    loc_7_0DE6
        push   1
        push   edi
        call   sub_7_019D
        inc    dword ptr [edi + 0xf]
        push   0
        push   dword ptr [edi + 0xb]
        VMMCall  VMM._HeapFree                   ; cd2051000100
        pop    ecx
        pop    ecx
        push   0
        push   edi
        VMMCall  VMM._HeapFree                   ; cd2051000100
        pop    ecx
        pop    ecx
loc_7_0DE6:
        ret    
sub_7_0DE7:
        push   ecx
        push   edx
        mov    eax, dword ptr [edi + 8]
        cmp    al, 5
        je     loc_7_0E2E
        cmp    al, 6
        je     loc_7_0E2E
loc_7_0DF4:
        xor    ecx, ecx
        cmp    al, 4
        je     loc_7_0E03
        cmp    al, 3
        jne    loc_7_0E06
        mov    ecx, 0x800
loc_7_0E03:
        or     ch, 1
loc_7_0E06:
        cmp    al, 0x11
        jb     loc_7_0E1C
        push   ecx
        VMMCall  VMM.VMM_GetSystemInitState      ; cd2011010100
        pop    ecx
        cmp    eax, 0x40000000
        ja     loc_7_0E1C
        or     ch, 4
loc_7_0E1C:
        push   ecx
        push   dword ptr [esi]
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        pop    ecx
        pop    ecx
        or     eax, eax
        je     loc_7_0E3E
loc_7_0E2B:
        pop    edx
        pop    ecx
        ret    
loc_7_0E2E:
        call   sub_7_0E41
        jb     loc_7_0DF4
        mov    dword ptr [edi + 0xc], 1
        jmp    loc_7_0E2B
loc_7_0E3E:
        stc    
        jmp    loc_7_0E2B
sub_7_0E41:
        push   edi
        call   sub_7_0E64
        jb     loc_7_0E62
        push   esi
        mov    esi, dword ptr [edi + 0x17]
        mov    ecx, dword ptr [edi + 0x13]
loc_7_0E50:
        cmp    dword ptr [esi + 8], eax
        je     loc_7_0E5E
        add    esi, 0x10
        loop   loc_7_0E50
        pop    esi
        stc    
        jmp    loc_7_0E62
loc_7_0E5E:
        mov    eax, dword ptr [esi]
        pop    esi
        clc    
loc_7_0E62:
        pop    edi
        ret    
sub_7_0E64:
        push   esi
        push   ebx
        mov    esi, dword ptr [ebp + 0x67]
        add    esi, dword ptr [ebp + 2]
        call   sub_7_0175
        mov    ebx, 0xe0                     ; -> obj1:0x00E0
loc_7_0E76:
        mov    ebx, dword ptr [ebx]
        or     ebx, ebx
        je     loc_7_0E90
        push   esi
        movzx  ecx, byte ptr [esi]
        inc    esi
        mov    edi, dword ptr [ebx + 0xb]
        cld    
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        pop    esi
        jne    loc_7_0E76
        cmp    byte ptr [edi], cl
        jne    loc_7_0E76
        mov    edi, ebx
loc_7_0E90:
        call   sub_7_0188
        cmp    ebx, 1
        pop    ebx
        pop    esi
        ret    
sub_7_0E9B:
        pushal 
        mov    edi, edx
        mov    ecx, dword ptr [edx + 0x13]
        mov    ebx, dword ptr [edx + 0x17]
        lea    ebp, [ebp + 0x2e7]
        xor    edx, edx
loc_7_0EAC:
        inc    edx
        cmp    dword ptr [ebx + 8], -1
        je     loc_7_0F11
        cmp    byte ptr [0xc5], 0            ; -> obj5:0x00C5
        je     loc_7_0F08
        mov    word ptr [ebp], dx
        mov    eax, dword ptr [ebx]
        mov    dword ptr [ebp + 4], eax
        mov    eax, dword ptr [ebx + 4]
        mov    dword ptr [ebp + 8], eax
        mov    eax, dword ptr [edi + 0xb]
        mov    dword ptr [ebp + 0xc], eax
        mov    word ptr [ebp + 0x10], ds
        mov    dword ptr [ebp + 0x12], eax
        mov    word ptr [ebp + 0x16], ds
        push   ebx
        push   edx
        xor    esi, esi
        test   byte ptr [ebx + 8], 1
        mov    ebx, 0x300028
        jne    loc_7_0EEF
        inc    esi
        ror    ebx, 0x10
loc_7_0EEF:
        mov    word ptr [ebp + 2], bx
        shr    ebx, 0x10
        mov    word ptr [ebp + 0x18], bx
        mov    dx, ds
        mov    ebx, ebp
        mov    eax, 0x150
        int    0x41
        pop    edx
        pop    ebx
loc_7_0F08:
        xor    eax, eax
        inc    eax
        VxDCall  APM.Service_000Fh               ; cd200f002700
loc_7_0F11:
        add    ebx, 0x10
        loop   loc_7_0EAC
        popal  
        ret    
loc_7_0F18:
        pushal 
        call   sub_7_0CFA
        mov    eax, 8
        jb     loc_7_0CF3
        cmp    byte ptr [edi + 4], 0
        je     loc_7_0F60
        mov    edx, edi
        mov    edi, dword ptr [edx + 5]
        mov    eax, 9
        cmp    dword ptr [edi + 0x30], 0
        jne    loc_7_0F60
        VMMCall  VMM.VMM_Remove_DDB              ; cd20f8000100
        jb     loc_7_0F60
        dec    byte ptr [edx + 4]
        jne    loc_7_0F5D
        mov    esi, 0xf67                    ; -> loc_7_0F67
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        VMMCall  VMM.Schedule_VM_Event           ; cd200f000100
loc_7_0F5D:
        popal  
        clc    
        ret    
loc_7_0F60:
        mov    dword ptr [esp + 0x1c], eax
        popal  
        stc    
        ret    
loc_7_0F67:
        mov    edi, edx
        xor    esi, esi
        call   sub_7_0D4C
        ret    
loc_7_0F71:
        VMMCall  VMM.Simulate_Far_Ret            ; cd204a000100
        cmp    byte ptr [edx + 4], 0
        je     loc_7_0F83
        mov    edx, dword ptr [edx + 5]
        jmp    dword ptr [edx + 0x1c]
loc_7_0F83:
        mov    word ptr [ebp + 0x1c], 0xffff
        or     dword ptr [ebp + 0x2c], 1
        ret    
loc_7_0F8E:
        VMMCall  VMM.Simulate_Far_Ret            ; cd204a000100
        cmp    byte ptr [edx + 4], 0
        je     loc_7_0F83
        mov    edx, dword ptr [edx + 5]
        jmp    dword ptr [edx + 0x20]
loc_7_0FA0:
        pushal 
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        mov    dword ptr [esp + 0x1c], eax
        popal  
        ret    
sub_7_0FAF:
        pushal 
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        mov    dword ptr [esp + 0x1c], eax
        popal  
        ret    
sub_7_0FBE:
        pushal 
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        mov    word ptr [esp + 0x1c], ax
        mov    word ptr [esp + 0x14], dx
        popal  
        ret    
sub_7_0FD3:
        pushal 
        push   0x21
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        popal  
        ret    
VXDLDR_Notify:
        ret    
        db  000h
sub_7_0FE0:
        test   byte ptr [4], 0xff            ; -> obj5:0x0004
        je     loc_7_1013
        push   edx
        push   edx
        VMMCall  VMM._lstrlen                    ; cd207c010100
        pop    ecx
        pop    edx
        cmp    eax, 0x104
        je     loc_7_1013
        push   edx
        push   0xc000c
        push   0                             ; -> loc_2_0000
        VMMCall  VMM._LocalizeStackSprintf       ; cd2059010100
        call   sub_7_10DC
        add    esp, dword ptr [esp]
loc_7_1013:
        ret    
sub_7_1014:
        pushfd 
        push   eax
        push   ecx
        mov    ecx, eax
        mov    eax, 0xd
        jb     loc_7_1027
        xor    ecx, ecx
        mov    eax, 0xe
loc_7_1027:
        test   byte ptr [4], 0xff            ; -> obj5:0x0004
        je     loc_7_1079
        push   eax
        push   ecx
        push   edx
        push   edx
        VMMCall  VMM._lstrlen                    ; cd207c010100
        pop    ecx
        pop    edx
        pop    ecx
        cmp    eax, 0x104
        pop    eax
        je     loc_7_1079
        add    eax, 0xc0000
        push   edx
        push   eax
        push   0                             ; -> loc_2_0000
        VMMCall  VMM._LocalizeStackSprintf       ; cd2059010100
        call   sub_7_10DC
        add    esp, dword ptr [esp]
        jecxz  loc_7_1079
        cmp    ecx, 0xb
        ja     loc_7_1079
        movzx  eax, word ptr [ecx*2 + 0xa]   ; -> loc_2_000A
        add    eax, dword ptr [4]            ; -> loc_2_0004
        call   sub_7_10DC
loc_7_1079:
        pop    ecx
        pop    eax
        popfd  
        ret    
sub_7_107D:
        test   byte ptr [4], 0xff            ; -> obj5:0x0004
        je     loc_7_10A4
        push   edi
        mov    edi, dword ptr [edx + 0xb]
        push   edi
        push   0xc000f
        push   0                             ; -> loc_2_0000
        VMMCall  VMM._LocalizeStackSprintf       ; cd2059010100
        call   sub_7_10DC
        add    esp, dword ptr [esp]
        pop    edi
loc_7_10A4:
        ret    
sub_7_10A5:
        push   edi
        push   eax
        mov    eax, 0x10
        jb     loc_7_10B3
        mov    eax, 0x11
loc_7_10B3:
        test   byte ptr [4], 0xff            ; -> obj5:0x0004
        je     loc_7_10D9
        mov    edi, dword ptr [edx + 0xb]
        add    eax, 0xc0000
        push   edi
        push   eax
        push   0                             ; -> loc_2_0000
        VMMCall  VMM._LocalizeStackSprintf       ; cd2059010100
        call   sub_7_10DC
        add    esp, dword ptr [esp]
loc_7_10D9:
        pop    eax
        pop    edi
        ret    
sub_7_10DC:
        pushal 
        test   byte ptr [4], 0xff            ; -> obj5:0x0004
        je     loc_7_1102
        VMMCall  VMM.Open_Boot_Log               ; cd2088010100
        or     eax, eax
        jne    loc_7_1102
        mov    edx, dword ptr [esp + 0x1c]
        xor    ecx, ecx
        VMMCall  VMM.Write_Boot_Log              ; cd2089010100
        VMMCall  VMM.Close_Boot_Log              ; cd208a010100
loc_7_1102:
        popal  
        ret    
VxD__LTEXT ENDS

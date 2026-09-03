; JAVASUP -- reconstructed 32-bit disassembly (VxD / LE format)
; 3 objects, 161 instructions recovered by recursive descent

Declare_Virtual_Device JAVASUP, 1, 0, JAVASUP_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 080000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x64  R-X
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  1, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'JAVASUP '                  ; DDB_Name
        dd  080000000h                  ; DDB_Init_Order
        dd  JAVASUP_Control          ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h
JAVASUP_Control:
        db  083h
        db  0F8h, 031h, 00Fh, 084h
        dd  loc_2_0019               ; fixup
        db  083h, 0F8h, 000h, 00Fh, 084h
        dd  loc_3_0000               ; fixup
        db  0F8h, 0C3h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x1e2  R-X
loc_2_0000:
        push   edi
        popal  
        imul   esi, dword ptr [esi + eax*2 + 0x6f], 0x6c754d72
        je     0x75
        jo     0x7a
        dec    edi
        bound  ebp, qword ptr [edx + 0x65]
        arpl   word ptr [ebx + esi*2 + 0x45], si
        js     loc_2_0019
loc_2_0019:
        call   sub_2_0115
        or     eax, eax
        je     loc_2_0085
        cmp    dword ptr [eax + 0x18], 0xe904c483
        jne    loc_2_0085
        mov    ebx, dword ptr [eax + 0x1c]
        lea    eax, [ebx + eax + 0x20]
        cmp    dword ptr [eax], 0x8bc03355
        jne    loc_2_0085
        cmp    dword ptr [eax + 0x10], 0x570c5d8b
        jne    loc_2_0087
        mov    edi, 0xf6
loc_2_0048:
        add    edi, eax
        cmp    dword ptr [edi], 0x1f47d81
        jne    loc_2_0085
        cmp    dword ptr [edi + 4], 0xf400000
        jne    loc_2_0085
        call   sub_2_0097
        jb     loc_2_0085
        mov    esi, edi
        add    edi, 8
        and    edi, 0xfffff000
        cmp    edi, esi
        jbe    loc_2_0078
        call   sub_2_0097
        jb     loc_2_0085
loc_2_0078:
        mov    dword ptr [esi], 0x40f77d80
        mov    dword ptr [esi + 4], 0xf0c5d8b
loc_2_0085:
        clc    
        ret    
loc_2_0087:
        cmp    dword ptr [eax + 0x10], 0x57f44589
        mov    edi, 0xfd
        je     loc_2_0048
        jmp    loc_2_0085
sub_2_0097:
        pushal 
        mov    ebp, 0x1000
        and    edi, 0xfffff000
        xor    eax, eax
        push   0x20000000
        push   eax
        push   eax
        push   eax
        push   eax
        push   eax
        push   1
        push   1
        VMMCall  VMM._PageAllocate               ; cd2053000100
        add    esp, 0x20
        test   eax, eax
        je     0x112
        xchg   esi, eax
        push   ebp
        push   edi
        push   esi
        VMMCall  VMM._lmemcpy                    ; cd207d010100
        add    esp, 0xc
        shr    edi, 0xc
        push   0x20000000
        push   1
        push   edi
        VMMCall  VMM._PageDecommit               ; cd201f010100
        add    esp, 0xc
        push   0x20040000
        push   0
        push   2
        push   1
        push   edi
        VMMCall  VMM._PageCommit                 ; cd201e010100
        add    esp, 0x14
        shl    edi, 0xc
        push   ebp
        push   esi
        push   edi
        VMMCall  VMM._lmemcpy                    ; cd207d010100
        add    esp, 0xc
        push   0
        push   esi
        VMMCall  VMM._PageFree                   ; cd2055000100
        add    esp, 8
        or     al, 0xf9
        popal  
        ret    
sub_2_0115:
        sub    esp, 8
        mov    eax, 0xbff70000
        cmp    word ptr [eax], 0x5a4d
        push   ebx
        push   esi
        push   edi
        push   ebp
        jne    loc_2_01BC
        mov    eax, 0xbff7003c
        mov    eax, dword ptr [eax]
        sub    eax, 0x40090000
        cmp    dword ptr [eax], 0x4550
        jne    loc_2_01BC
        mov    eax, dword ptr [eax + 0x78]
        mov    ecx, dword ptr [eax - 0x4008ffe0]
        mov    ebp, dword ptr [eax - 0x4008ffe8]
        sub    eax, 0x40090000
        sub    ecx, 0x40090000
        dec    ebp
        mov    dword ptr [esp + 0x14], ecx
        xor    ecx, ecx
loc_2_0161:
        lea    edx, [ebp + ecx]
        mov    esi, dword ptr [esp + 0x14]
        shr    edx, 1
        mov    edi, 0                        ; -> loc_2_0000
        mov    esi, dword ptr [esi + edx*4]
        sub    esi, 0x40090000
loc_2_0179:
        mov    bl, byte ptr [esi]
        mov    byte ptr [esp + 0x10], bl
        cmp    bl, byte ptr [edi]
        jne    loc_2_01A5
        or     byte ptr [esp + 0x10], bl
        je     loc_2_01A1
        mov    bl, byte ptr [esi + 1]
        mov    byte ptr [esp + 0x10], bl
        cmp    bl, byte ptr [edi + 1]
        jne    loc_2_01A5
        add    esi, 2
        add    edi, 2
        or     byte ptr [esp + 0x10], bl
        jne    loc_2_0179
loc_2_01A1:
        xor    esi, esi
        jmp    loc_2_01AA
loc_2_01A5:
        sbb    esi, esi
        sbb    esi, -1
loc_2_01AA:
        test   esi, esi
        jle    loc_2_01B3
        lea    ebp, [edx - 1]
        jmp    loc_2_01B8
loc_2_01B3:
        jge    loc_2_01C6
        lea    ecx, [edx + 1]
loc_2_01B8:
        cmp    ecx, ebp
        jbe    loc_2_0161
loc_2_01BC:
        xor    eax, eax
loc_2_01BE:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 8
        ret    
loc_2_01C6:
        mov    ecx, dword ptr [eax + 0x24]
        mov    eax, dword ptr [eax + 0x1c]
        movzx  edx, word ptr [ecx + edx*2 - 0x40090000]
        mov    eax, dword ptr [eax + edx*4 - 0x40090000]
        sub    eax, 0x40090000
        jmp    loc_2_01BE
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0xd  R-X
loc_3_0000:
        VxDCall  NDIS.Service_0000h              ; cd2000002a00
        cmp    eax, 0x103
        cmc    
        ret    
VxD__LTEXT ENDS

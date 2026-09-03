; MSMINI -- reconstructed 32-bit disassembly (VxD / LE format)
; 6 objects, 1338 instructions recovered by recursive descent

Declare_Virtual_Device MSMINI, 1, 0, MSMINI_Control, 00000h, Undefined_Init_Order
;   DDK version 0400h   init_order 034000001h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x56c  R-X
MSMINI_Control:
        cmp    eax, 1
        je     loc_2_0000
        cmp    eax, 2
        je     loc_3_00FB
        cmp    eax, 9
        je     loc_3_00FB
        cmp    eax, 0xf
        je     loc_3_00EB
        cmp    eax, 0x23
        je     loc_3_04D0
        clc    
        ret    
        db  090h
loc_1_0030:
        push   edx
        push   ebp
        mov    word ptr [0x547], si          ; -> obj1:0x0547
        mov    word ptr [0x549], di          ; -> obj1:0x0549
        mov    ebx, dword ptr [0x4c4]        ; -> obj1:0x04C4
        mov    edx, ebx
        add    edx, dword ptr [0x4d0]        ; -> obj1:0x04D0
        mov    byte ptr [edx + 0x12], al
        push   ebx
        call   sub_1_0074
        call   sub_1_00CC
        call   sub_1_0155
        call   sub_1_029A
        pop    ebx
        or     al, 0x80
        pop    ebp
        pop    edx
        jmp    loc_1_04D8
        db  02Eh, 08Bh, 0C0h
sub_1_0074:
        cmp    word ptr [edx + 6], 0
        je     loc_1_00CA
        push   eax
        push   edx
        mov    ebp, 0xff
        movsx  ebx, word ptr [edx + 8]
        movsx  ecx, word ptr [edx + 0xa]
        push   edi
        mov    eax, esi
        imul   eax, ecx
        xchg   edi, eax
        imul   eax, ebx
        sub    edi, eax
        movsx  eax, word ptr [0x4ea]         ; -> loc_1_04EA
        add    eax, edi
        cdq    
        idiv   ebp
        mov    word ptr [0x4ea], dx          ; -> loc_1_04EA
        xchg   esi, eax
        imul   eax, ebx
        xchg   edi, eax
        pop    eax
        imul   eax, ecx
        add    edi, eax
        movsx  eax, word ptr [0x4ec]         ; -> loc_1_04EC
        add    eax, edi
        cdq    
        idiv   ebp
        mov    word ptr [0x4ec], dx          ; -> loc_1_04EC
        xchg   edi, eax
        pop    edx
        pop    eax
loc_1_00CA:
        ret    
        db  090h
sub_1_00CC:
        cmp    byte ptr [0x4f4], 0           ; -> loc_1_04F4
        je     loc_1_0118
        push   eax
        push   edx
        call   sub_1_011C
        xchg   esi, eax
        imul   eax, ecx
        movsx  ebx, word ptr [0x4ee]         ; -> loc_1_04EE
        add    eax, ebx
        cdq    
        mov    ebx, 0x10
        idiv   ebx
        mov    word ptr [0x4ee], dx          ; -> loc_1_04EE
        xchg   esi, eax
        xchg   edi, eax
        imul   eax, ecx
        movsx  ebx, word ptr [0x4f0]         ; -> loc_1_04F0
        add    eax, ebx
        cdq    
        mov    ebx, 0x10
        idiv   ebx
        mov    word ptr [0x4f0], dx          ; -> loc_1_04F0
        xchg   edi, eax
        pop    edx
        pop    eax
loc_1_0118:
        ret    
        db  02Eh, 08Bh, 0C0h
sub_1_011C:
        push   edi
        mov    eax, esi
        cdq    
        xor    eax, edx
        sub    eax, edx
        xchg   edi, eax
        cdq    
        xor    eax, edx
        sub    eax, edx
        cmp    eax, edi
        jae    loc_1_012F
        xchg   edi, eax
loc_1_012F:
        mov    edi, 0x4f5                    ; -> obj1:0x04F5
        movzx  ecx, byte ptr [0x4f4]         ; -> loc_1_04F4
        cld    
        inc    edi
        dec    ecx
        je     loc_1_0145
loc_1_0140:
        scasb  al, byte ptr es:[edi]
        jb     loc_1_0145
        loop   loc_1_0140
loc_1_0145:
        sub    cl, byte ptr [0x4f4]          ; -> loc_1_04F4
        not    cl
        mov    cl, byte ptr [ecx + 0x515]    ; -> obj1:0x0515
        pop    edi
        ret    
sub_1_0155:
        cmp    byte ptr [edx + 0xd], 0
        jne    loc_1_015C
        ret    
loc_1_015C:
        push   ebx
        mov    bh, al
        and    bh, 0x3c
        mov    bl, 0x3c
        xor    bl, byte ptr [edx + 0x11]
        xor    bl, byte ptr [edx + 0x10]
        cmp    byte ptr [0x4f2], 0           ; -> loc_1_04F2
        je     loc_1_01B9
        cmp    byte ptr [0x4f2], 1           ; -> loc_1_04F2
        je     loc_1_01E0
        cmp    byte ptr [0x4f2], 2           ; -> loc_1_04F2
        je     loc_1_01FA
        cmp    byte ptr [0x4f2], 4           ; -> loc_1_04F2
        je     loc_1_0235
        cmp    byte ptr [0x4f2], 0x84        ; -> loc_1_04F2
        je     loc_1_0246
        cmp    byte ptr [0x4f2], 8           ; -> loc_1_04F2
        je     loc_1_0257
        cmp    byte ptr [0x4f2], 0x10        ; -> loc_1_04F2
        je     loc_1_026B
loc_1_01B9:
        test   bh, 0x3c
        jne    loc_1_01C3
        jmp    loc_1_0293
loc_1_01C3:
        test   bl, bh
        jne    loc_1_01CE
        mov    al, bh
        jmp    loc_1_0298
loc_1_01CE:
        mov    byte ptr [0x4f2], 1           ; -> loc_1_04F2
        mov    byte ptr [0x4f3], bh          ; -> obj1:0x04F3
        jmp    loc_1_0293
loc_1_01E0:
        mov    bl, byte ptr [0x4f3]          ; -> obj1:0x04F3
        test   bl, bh
        jne    loc_1_0293
        mov    byte ptr [0x4f2], 2           ; -> loc_1_04F2
        jmp    loc_1_0293
loc_1_01FA:
        cmp    byte ptr [0x4f2], 2           ; -> loc_1_04F2
        jne    loc_1_0235
        test   bl, bh
        jne    loc_1_0221
        test   bh, 0x3c
        je     loc_1_0293
        and    bh, bl
        mov    byte ptr [0x4f3], bh          ; -> obj1:0x04F3
        mov    byte ptr [0x4f2], 4           ; -> loc_1_04F2
        jmp    loc_1_0293
loc_1_0221:
        test   bh, 0x3c
        je     loc_1_0293
        mov    byte ptr [0x4f3], bh          ; -> obj1:0x04F3
        mov    byte ptr [0x4f2], 0x84        ; -> loc_1_04F2
        jmp    loc_1_0293
loc_1_0235:
        and    bh, byte ptr [0x4f3]          ; -> obj1:0x04F3
        jne    loc_1_0293
        mov    byte ptr [0x4f2], 8           ; -> loc_1_04F2
        jmp    loc_1_027C
loc_1_0246:
        and    bh, byte ptr [0x4f3]          ; -> obj1:0x04F3
        jne    loc_1_027C
        mov    byte ptr [0x4f2], 0           ; -> loc_1_04F2
        jmp    loc_1_027C
loc_1_0257:
        and    bh, 0x3c
        je     loc_1_027C
        mov    byte ptr [0x4f3], bh          ; -> obj1:0x04F3
        mov    byte ptr [0x4f2], 0x10        ; -> loc_1_04F2
        jmp    loc_1_027C
loc_1_026B:
        and    bh, byte ptr [0x4f3]          ; -> obj1:0x04F3
        jne    loc_1_027C
        mov    byte ptr [0x4f2], 0           ; -> loc_1_04F2
        jmp    loc_1_0293
loc_1_027C:
        or     bh, byte ptr [edx + 0x11]
        push   cx
        mov    ch, byte ptr [edx + 0x10]
        not    ch
        and    bh, ch
        pop    cx
        and    bl, 0xc3
        or     bl, bh
        mov    al, bl
        jmp    loc_1_0298
loc_1_0293:
        and    bl, 0xc3
        mov    al, bl
loc_1_0298:
        pop    ebx
        ret    
sub_1_029A:
        mov    ah, al
        xor    al, al
        test   byte ptr [edx + 0x11], ah
        je     loc_1_02A5
        or     al, 0x20
loc_1_02A5:
        test   byte ptr [edx + 0x10], ah
        je     loc_1_02AC
        or     al, 0x10
loc_1_02AC:
        ret    
sub_1_02AD:
        push   esi
        push   eax
        push   ecx
        movzx  eax, word ptr [ebp + 0x10]
        cmp    eax, 0
        jne    loc_1_02D9
        mov    word ptr [0x541], ax          ; -> obj1:0x0541
        mov    esi, 0x55b                    ; -> obj1:0x055B
loc_1_02C4:
        mov    byte ptr [esi], al
        add    esi, 4
        cmp    esi, 0x56b                    ; -> obj1:0x056B
        jne    loc_1_02C4
        dec    eax
        mov    byte ptr [0x545], al          ; -> obj1:0x0545
        jmp    loc_1_031E
loc_1_02D9:
        mov    eax, 1
        movzx  ecx, word ptr [ebp]
        cmp    ecx, 4
        jae    loc_1_0320
        mov    eax, 4
        mul    cx
        add    eax, 0x55b                    ; -> obj1:0x055B
        xchg   esi, eax
        cli    
        push   eax
        push   eax
        mov    byte ptr [esi + 3], cl
        movzx  eax, word ptr [ebp + 0x14]
        mov    byte ptr [esi], al
        movzx  eax, word ptr [ebp + 4]
        mov    word ptr [esi + 1], ax
        movzx  eax, word ptr [ebp + 0x18]
        mov    word ptr [0x543], ax          ; -> obj1:0x0543
        movzx  eax, word ptr [ebp + 0x10]
        mov    word ptr [0x541], ax          ; -> obj1:0x0541
        sti    
loc_1_031E:
        xor    eax, eax
loc_1_0320:
        mov    word ptr [ebp + 0x1c], ax
        pop    ecx
        pop    eax
        pop    esi
        ret    
sub_1_0328:
        cli    
        movzx  ecx, word ptr [ebp + 0x10]
        mov    word ptr [0x53b], cx          ; -> obj1:0x053B
        movzx  ecx, word ptr [ebp + 4]
        mov    word ptr [0x539], cx          ; -> obj1:0x0539
        movzx  ecx, word ptr [ebp + 0x18]
        mov    word ptr [0x53f], cx          ; -> obj1:0x053F
        movzx  ecx, word ptr [ebp]
        mov    word ptr [0x53d], cx          ; -> obj1:0x053D
        sti    
        mov    eax, ecx
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        mov    cx, word ptr [0x4e8]          ; -> loc_1_04E8
        mov    edx, dword ptr [0x4e4]        ; -> loc_1_04E4
        VMMCall  VMM.Simulate_Far_Call           ; cd2048000100
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        ret    
sub_1_039C:
        movzx  eax, word ptr [ebp + 4]
        mov    dword ptr [0x4e4], eax        ; -> loc_1_04E4
        mov    ax, word ptr [ebp + 0x38]
        mov    word ptr [0x4e8], ax          ; -> loc_1_04E8
        ret    
        db  055h, 053h, 045h, 052h, 000h, 047h, 065h, 074h, 041h, 073h, 079h, 06Eh, 063h, 04Bh, 065h, 079h
        db  053h, 074h, 061h, 074h, 065h, 000h, 050h, 06Fh, 073h, 074h, 04Dh, 065h, 073h, 073h, 061h, 067h
        db  065h, 000h, 0A1h
        dd  obj1:0x0539              ; fixup
        db  00Bh, 0C0h, 074h, 007h, 0A1h
        dd  obj1:0x053D              ; fixup
        db  00Bh, 0C0h, 0C3h, 066h, 083h, 03Dh
        dd  obj1:0x0541              ; fixup
        db  000h, 00Fh, 084h, 081h, 000h, 000h, 000h, 080h, 03Dh
        dd  obj1:0x0545              ; fixup
        db  0FFh, 075h, 063h, 053h, 051h, 056h, 0BEh
        dd  obj1:0x055B              ; fixup
        db  084h, 006h, 074h, 031h, 08Bh, 01Dh
        dd  obj1:0x04C4              ; fixup
        db  0CDh, 020h, 011h, 000h, 00Dh, 000h, 00Fh, 0B7h, 04Eh, 001h, 083h, 0F9h, 010h, 075h, 006h, 066h
        db  0A9h, 001h, 000h, 075h, 026h, 083h, 0F9h, 011h, 075h, 006h, 066h, 0A9h, 080h, 000h, 075h, 01Bh
        db  083h, 0F9h, 012h, 075h, 006h, 066h, 0A9h, 000h, 001h, 075h, 010h, 083h, 0C6h, 004h, 081h, 0FEh
        dd  obj1:0x056B              ; fixup
        db  075h, 0C0h, 05Eh, 059h, 05Bh, 0EBh, 02Bh, 00Fh, 0B6h, 006h, 0A2h
        dd  loc_1_0546               ; fixup
        db  0F6h, 0D0h, 0A2h
        dd  obj1:0x0545              ; fixup
        db  00Fh, 0B6h, 046h, 003h, 05Eh, 059h, 05Bh, 084h, 005h
        dd  loc_1_0546               ; fixup
        db  075h, 007h, 0C6h, 005h
        dd  obj1:0x0545              ; fixup
        db  0FFh, 022h, 005h
        dd  obj1:0x0545              ; fixup
        db  0C3h, 000h
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00000h                      ; DDB_Req_Device_Number
        db  1, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'MSMINI  '                  ; DDB_Name
        dd  034000001h                  ; DDB_Init_Order
        dd  MSMINI_Control           ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_04D8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_04DC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_04E4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_04E8:
        add    byte ptr [eax], al
loc_1_04EA:
        add    byte ptr [eax], al
loc_1_04EC:
        add    byte ptr [eax], al
loc_1_04EE:
        add    byte ptr [eax], al
loc_1_04F0:
        add    byte ptr [eax], al
loc_1_04F2:
        add    byte ptr [eax], al
loc_1_04F4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    bh, bh
loc_1_0546:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x134  R-X
loc_2_0000:
        VxDCall  VMD.VMD_Get_Version             ; cd2000000c00
        cmp    eax, 0x400
        jb     loc_2_0131
        mov    dword ptr [0x4c4], ebx        ; -> obj1:0x04C4
        mov    esi, 0x275                    ; -> obj4:0x0275
        VMMCall  VMM.Get_Environment_String      ; cd20b5000100
        jb     loc_2_002C
        inc    dword ptr [0x27d]             ; -> obj4:0x027D
        jmp    loc_2_0039
loc_2_002C:
        mov    esi, 0x260                    ; -> obj4:0x0260
        VMMCall  VMM.Get_Environment_String      ; cd20b5000100
        jb     loc_2_004E
loc_2_0039:
        mov    esi, edx
        mov    edi, 0x1d0                    ; -> obj4:0x01D0
        movzx  ecx, byte ptr [esi]
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    eax, 0
        mov    dword ptr [edi], eax
        jmp    loc_2_0077
loc_2_004E:
        mov    esi, 0x266                    ; -> obj4:0x0266
        mov    edi, 0x26c                    ; -> obj4:0x026C
        xor    edx, edx
        VMMCall  VMM.Get_Profile_String          ; cd20b3000100
        or     edx, edx
        je     loc_2_0077
        mov    esi, edx
        mov    edi, 0x1d0                    ; -> obj4:0x01D0
        movzx  ecx, byte ptr [esi]
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    eax, 0
        mov    dword ptr [edi], eax
loc_2_0077:
        push   0
        push   0x1c
        VMMCall  VMM._Allocate_Device_CB_Area    ; cd20a7000100
        pop    ecx
        pop    ecx
        or     eax, eax
        jne    loc_2_008D
        VMMCall  VMM.Fatal_Memory_Error          ; cd20bf000100
loc_2_008D:
        mov    dword ptr [0x4d0], eax        ; -> obj1:0x04D0
        push   0x100
        push   0x1e4
        VMMCall  VMM._Allocate_Global_V86_Data_Area ; cd20a8000100
        pop    ecx
        pop    ecx
        or     eax, eax
        jne    loc_2_00AE
        VMMCall  VMM.Fatal_Memory_Error          ; cd20bf000100
loc_2_00AE:
        mov    dword ptr [0x4d4], eax        ; -> obj1:0x04D4
        mov    eax, 0x33
        mov    esi, 0x20                     ; -> sub_3_0020
        VMMCall  VMM.Hook_V86_Int_Chain          ; cd2041000100
        VMMCall  VMM.Get_PM_Int_Vector           ; cd2044000100
        mov    dword ptr [0x4c8], ecx        ; -> obj1:0x04C8
        mov    dword ptr [0x4cc], edx        ; -> obj1:0x04CC
        mov    esi, 0                        ; -> loc_3_0000
        VMMCall  VMM.Allocate_PM_Call_Back       ; cd200c000100
        movzx  edx, ax
        mov    ecx, eax
        shr    ecx, 0x10
        mov    eax, 0x33
        VMMCall  VMM.Set_PM_Int_Vector           ; cd2045000100
        mov    eax, 0xc0008
        mov    esi, 0x30                     ; -> loc_1_0030
        VMMCall  VMM.Hook_Device_Service         ; cd2090000100
        mov    dword ptr [0x4d8], esi        ; -> loc_1_04D8
        mov    eax, 0xc
        mov    esi, 0x9c                     ; -> loc_3_009C
        VMMCall  VMM.Hook_Device_PM_API          ; cd2092000100
        mov    dword ptr [0x4dc], esi        ; -> loc_1_04DC
        VxDCall  VMD.Service_0007h               ; cd2007000c00
        or     cx, cx
        je     loc_2_0130
        inc    byte ptr [0]                  ; -> obj4:0x0000
loc_2_0130:
        clc    
loc_2_0131:
        ret    
        db  000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x8ea  R-X
loc_3_0000:
        call   sub_3_0020
        jae    loc_3_0019
        mov    ecx, dword ptr [0x4c8]        ; -> obj1:0x04C8
        mov    edx, dword ptr [0x4cc]        ; -> obj1:0x04CC
        VMMCall  VMM.Simulate_Far_Jmp            ; cd2049800100
loc_3_0019:
        VMMCall  VMM.Simulate_Iret               ; cd2047000100
        ret    
sub_3_0020:
        movzx  eax, word ptr [ebp + 0x1c]
        cmp    eax, 0x1a
        jne    loc_3_006D
        movzx  eax, word ptr [ebp + 0x10]
        test   eax, 0x800
        je     loc_3_0050
        and    ax, 0xff
        mov    word ptr [ebp + 0x10], ax
        movzx  eax, word ptr [ebp + 0x14]
        mov    dword ptr [0x535], eax        ; -> obj1:0x0535
        mov    eax, 0x64
        mov    word ptr [ebp + 0x14], ax
        jmp    loc_3_009A
loc_3_0050:
        movzx  eax, word ptr [ebp + 0x10]
        cmp    eax, dword ptr [0x535]        ; -> obj1:0x0535
        jne    loc_3_0064
        xor    eax, eax
        mov    dword ptr [0x535], eax        ; -> obj1:0x0535
        ret    
loc_3_0064:
        xor    eax, eax
        mov    dword ptr [0x535], eax        ; -> obj1:0x0535
        jmp    loc_3_009A
loc_3_006D:
        movzx  eax, word ptr [ebp + 0x1c]
        cmp    eax, 0x3a
        jne    loc_3_007C
        call   sub_1_039C
        ret    
loc_3_007C:
        movzx  eax, word ptr [ebp + 0x1c]
        sub    eax, 0x2b
        jb     loc_3_009A
        cmp    eax, 0xb
        jae    loc_3_009A
        mov    edx, ebx
        add    edx, dword ptr [0x4d0]        ; -> obj1:0x04D0
        call   sub_4_0001
        ret    
loc_3_009A:
        stc    
        ret    
loc_3_009C:
        movzx  eax, word ptr [ebp + 0x1c]
        cmp    eax, 0x36
        jne    loc_3_00AC
        call   sub_1_0328
        jmp    loc_3_00E5
loc_3_00AC:
        cmp    eax, 0x37
        jne    loc_3_00B8
        call   sub_1_02AD
        jmp    loc_3_00E5
loc_3_00B8:
        cmp    eax, 0x102
        jne    loc_3_00CA
        mov    word ptr [ebp + 0x1c], 0
        and    byte ptr [ebp + 0x2c], 0xfe
        ret    
loc_3_00CA:
        movzx  eax, word ptr [ebp + 0x1c]
        cmp    eax, 4
        jne    loc_3_00E5
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_3_00E5
        push   ebx
        mov    ebx, dword ptr [ebp + 0x10]
        call   sub_3_01A6
        pop    ebx
loc_3_00E5:
        jmp    loc_1_04DC
loc_3_00EB:
        or     edx, edx
        je     loc_3_00F4
        cmp    edx, 0xc
        jne    loc_3_00F9
loc_3_00F4:
        call   sub_3_01A6
loc_3_00F9:
        clc    
        ret    
loc_3_00FB:
        mov    edx, ebx
        add    edx, dword ptr [0x4d0]        ; -> obj1:0x04D0
        mov    edi, ebx
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        cmp    ebx, edi
        je     loc_3_0148
        mov    edi, dword ptr [edi + 4]
        add    edi, dword ptr [0x4d4]        ; -> obj1:0x04D4
        mov    esi, 0x8c                     ; -> obj4:0x008C
        mov    ecx, 0x51
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    esi, 0x1d0                    ; -> obj4:0x01D0
        mov    ecx, 0x24
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    edi, edx
        mov    esi, ebx
        add    esi, dword ptr [0x4d0]        ; -> obj1:0x04D0
        mov    ecx, 0x1c
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    byte ptr [edx + 0xc], 1
        clc    
        ret    
loc_3_0148:
        call   sub_3_046D
        mov    byte ptr [edx + 0xc], 3
        call   sub_3_01B4
        mov    edi, dword ptr [ebx + 4]
        add    edi, dword ptr [0x4d4]        ; -> obj1:0x04D4
        add    edi, 0x144
        mov    esi, 0x1d0                    ; -> obj4:0x01D0
        mov    ecx, 0x24
        rep movsd dword ptr es:[edi], dword ptr [esi]
        xor    eax, eax
        mov    byte ptr [edx], al
        mov    byte ptr [edx + 1], al
        mov    byte ptr [edx + 2], al
        mov    byte ptr [edx + 3], al
        mov    word ptr [edx + 4], ax
        mov    word ptr [edx + 6], ax
        mov    word ptr [edx + 8], ax
        mov    word ptr [edx + 0xa], 0xff
        mov    byte ptr [edx + 0xd], al
        mov    byte ptr [edx + 0xe], al
        mov    byte ptr [edx + 0xf], 0xc
        mov    byte ptr [edx + 0x10], 0x10
        mov    byte ptr [edx + 0x11], 0x20
        mov    byte ptr [edx + 0x12], al
        ret    
sub_3_01A6:
        cmp    ebx, dword ptr [0x4c4]        ; -> obj1:0x04C4
        je     loc_3_01D3
        mov    dword ptr [0x4c4], ebx        ; -> obj1:0x04C4
sub_3_01B4:
        call   sub_3_0484
        xor    eax, eax
        mov    word ptr [0x4ea], ax          ; -> loc_1_04EA
        mov    word ptr [0x4ec], ax          ; -> loc_1_04EC
        mov    word ptr [0x4ee], ax          ; -> loc_1_04EE
        mov    word ptr [0x4f0], ax          ; -> loc_1_04F0
loc_3_01D3:
        ret    
loc_3_01D4:
        clc    
        ret    
loc_3_01D6:
        movsx  eax, word ptr [ebp + 0x10]
        cmp    eax, -1
        jne    loc_3_01EB
        call   sub_3_046D
        call   sub_3_0484
        jmp    loc_3_0216
loc_3_01EB:
        dec    eax
        cmp    eax, 4
        jae    loc_3_021C
        mov    byte ptr [edx + 0xc], al
        xchg   esi, eax
        mov    ax, 0x3804
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   esi, eax
        mov    edi, dword ptr [0x4d4]        ; -> obj1:0x04D4
        add    edi, dword ptr [ebx + 4]
        mov    ecx, 0x51
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        call   sub_3_0484
loc_3_0216:
        mov    byte ptr [ebp + 0x1c], 0
        clc    
        ret    
loc_3_021C:
        mov    byte ptr [ebp + 0x1c], 0xff
        clc    
        ret    
loc_3_0222:
        xor    eax, eax
        mov    word ptr [ebp + 0x1c], ax
        mov    al, byte ptr [edx + 0xc]
        inc    eax
        mov    word ptr [ebp + 0x10], ax
        mov    eax, dword ptr [0x4d4]        ; -> obj1:0x04D4
        add    eax, dword ptr [ebx + 4]
        mov    ecx, 0x143
        VMMCall  VMM.Map_Lin_To_VM_Addr          ; cd201d000100
        mov    word ptr [ebp + 0x38], cx
        mov    word ptr [ebp + 4], dx
        clc    
        ret    
loc_3_024D:
        xor    ecx, ecx
        movzx  eax, byte ptr [edx + 0xc]
        movsx  esi, word ptr [ebp + 0x10]
        cmp    esi, -1
        je     loc_3_0273
        dec    esi
        dec    ecx
        dec    ecx
        cmp    esi, 4
        jae    loc_3_0273
        xchg   esi, eax
        mov    byte ptr [edx + 0xc], al
        call   sub_3_0484
        xor    ecx, ecx
        movzx  eax, byte ptr [edx + 0xc]
loc_3_0273:
        inc    eax
        mov    word ptr [ebp + 0x1c], cx
        mov    word ptr [ebp + 0x10], ax
        dec    eax
        shl    eax, 4
        add    eax, dword ptr [0x4d4]        ; -> obj1:0x04D4
        lea    eax, [eax + 0x104]
        mov    ecx, 0xf
        VMMCall  VMM.Map_Lin_To_VM_Addr          ; cd201d000100
        mov    word ptr [ebp + 0x38], cx
        mov    word ptr [ebp + 4], dx
        clc    
        ret    
loc_3_02A1:
        xor    eax, eax
        dec    eax
        mov    word ptr [ebp + 0x1c], ax
        ret    
loc_3_02A9:
        movzx  ecx, word ptr [ebp + 0x18]
        jecxz  loc_3_02E2
        mov    al, 0x3c
        cmp    byte ptr [0], 0               ; -> obj4:0x0000
        jne    loc_3_02BC
        mov    al, 0x30
loc_3_02BC:
        and    ch, al
        je     loc_3_02D9
        and    cl, al
        je     loc_3_02D9
        test   ch, cl
        jne    loc_3_02D9
        mov    word ptr [edx + 0x10], cx
        or     cl, ch
        xor    al, cl
        mov    byte ptr [edx + 0xf], al
        mov    al, byte ptr [ebp + 0x14]
        mov    byte ptr [edx + 0xd], al
loc_3_02D9:
        movsx  eax, word ptr [ebp + 0x10]
        call   sub_3_0418
loc_3_02E2:
        mov    al, byte ptr [edx + 0x12]
        mov    bx, word ptr [edx + 6]
        mov    cx, word ptr [edx + 0x10]
        mov    dl, byte ptr [edx + 0xd]
        cmp    byte ptr [0], 0               ; -> obj4:0x0000
        jne    loc_3_0303
        xchg   edx, ebx
        mov    bh, bl
        mov    bl, al
        mov    ax, 0xffff
loc_3_0303:
        cbw    
        mov    word ptr [ebp + 0x1c], ax
        mov    word ptr [ebp + 0x10], bx
        mov    word ptr [ebp + 0x18], cx
        mov    word ptr [ebp + 0x14], dx
        clc    
        ret    
loc_3_0317:
        xor    eax, eax
        mov    word ptr [ebp + 0x1c], ax
        mov    word ptr [ebp + 0x10], ax
        mov    eax, 0x400
        mov    word ptr [ebp + 0x18], ax
        mov    word ptr [ebp + 0x14], ax
        ret    
loc_3_032F:
        mov    word ptr [ebp + 0x1c], 0xffbf
        mov    word ptr [ebp + 0x10], 0x8000
        xor    eax, eax
        mov    word ptr [ebp + 0x14], ax
        mov    word ptr [ebp + 0x18], ax
        ret    
loc_3_0346:
        xor    eax, eax
        mov    word ptr [ebp + 0x1c], ax
        mov    ecx, 0x15a
        xchg   word ptr [ebp + 0x18], cx
        or     ecx, ecx
        jne    loc_3_035A
        ret    
loc_3_035A:
        xchg   edi, eax
        mov    ax, 0x3814
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   edi, eax
        cld    
        stosb  byte ptr es:[edi], al
        stosb  byte ptr es:[edi], al
        mov    ax, 0x32
        stosb  byte ptr es:[edi], al
        stosb  byte ptr es:[edi], al
        xor    eax, eax
        stosb  byte ptr es:[edi], al
        mov    al, byte ptr [edx + 0xc]
        inc    eax
        stosb  byte ptr es:[edi], al
        xor    eax, eax
        stosb  byte ptr es:[edi], al
        stosb  byte ptr es:[edi], al
        stosb  byte ptr es:[edi], al
        stosb  byte ptr es:[edi], al
        stosb  byte ptr es:[edi], al
        mov    ax, word ptr [edx + 6]
        stosw  word ptr es:[edi], ax
        mov    al, byte ptr [edx + 0x11]
        stosb  byte ptr es:[edi], al
        mov    al, byte ptr [edx + 0x10]
        stosb  byte ptr es:[edi], al
        mov    al, byte ptr [edx + 0xd]
        stosb  byte ptr es:[edi], al
        mov    esi, dword ptr [0x4d4]        ; -> obj1:0x04D4
        add    esi, dword ptr [ebx + 4]
        mov    ecx, 0x51
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    al, byte ptr [edx]
        stosb  byte ptr es:[edi], al
        mov    al, byte ptr [edx + 1]
        stosb  byte ptr es:[edi], al
        mov    al, byte ptr [edx + 2]
        stosb  byte ptr es:[edi], al
        mov    al, byte ptr [edx + 3]
        stosb  byte ptr es:[edi], al
        mov    al, byte ptr [edx + 4]
        stosb  byte ptr es:[edi], al
        xor    eax, eax
        stosb  byte ptr es:[edi], al
        ret    
loc_3_03B7:
        mov    eax, dword ptr [0x4d4]        ; -> obj1:0x04D4
        add    eax, dword ptr [ebx + 4]
        add    eax, 0x144
        mov    ecx, 0x8f
        VMMCall  VMM.Map_Lin_To_VM_Addr          ; cd201d000100
        mov    word ptr [ebp + 0x38], cx
        mov    word ptr [ebp + 0x14], dx
        xor    ax, ax
        clc    
        ret    
loc_3_03DC:
        movsx  eax, word ptr [ebp + 0x10]
        cmp    eax, -1
        jne    loc_3_03FC
        mov    eax, dword ptr [edx]
        mov    word ptr [ebp + 0x10], ax
        shr    eax, 0x10
        mov    word ptr [ebp + 0x18], ax
        mov    ax, word ptr [edx + 4]
        mov    word ptr [ebp + 0x14], ax
        jmp    loc_3_0411
loc_3_03FC:
        mov    ax, word ptr [ebp + 0x18]
        shl    eax, 0x10
        mov    ax, word ptr [ebp + 0x10]
        mov    dword ptr [edx], eax
        mov    ax, word ptr [ebp + 0x14]
        mov    word ptr [edx + 4], ax
loc_3_0411:
        xor    eax, eax
        mov    word ptr [ebp + 0x1c], ax
        ret    
sub_3_0418:
        pushal 
        mov    edi, edx
        mov    ebx, 0x168
        cdq    
        idiv   ebx
        xchg   edx, eax
        or     eax, eax
        jns    loc_3_042A
        add    eax, ebx
loc_3_042A:
        mov    word ptr [edi + 6], ax
        mov    bx, 0x5a
        idiv   bl
        sub    bl, ah
        movzx  ecx, byte ptr [ebx + 0x31]    ; -> loc_4_0031
        mov    bl, ah
        movzx  ebx, byte ptr [ebx + 0x31]    ; -> loc_4_0031
        movzx  eax, al
        test   al, 1
        je     loc_3_044F
        xchg   ecx, ebx
loc_3_044F:
        mov    al, byte ptr [eax + 0x2d]     ; -> loc_4_002D
        shr    al, 1
        jae    loc_3_045B
        neg    ebx
loc_3_045B:
        shr    al, 1
        jae    loc_3_0461
        neg    ecx
loc_3_0461:
        cli    
        mov    word ptr [edi + 8], bx
        mov    word ptr [edi + 0xa], cx
        sti    
        popal  
        ret    
sub_3_046D:
        mov    esi, 0x8c                     ; -> obj4:0x008C
        mov    edi, dword ptr [0x4d4]        ; -> obj1:0x04D4
        add    edi, dword ptr [ebx + 4]
        mov    ecx, 0x51
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        ret    
sub_3_0484:
        cmp    ebx, dword ptr [0x4c4]        ; -> obj1:0x04C4
        jne    loc_3_04CE
        mov    edx, ebx
        add    edx, dword ptr [0x4d0]        ; -> obj1:0x04D0
        movzx  eax, byte ptr [edx + 0xc]
        mov    esi, dword ptr [0x4d4]        ; -> obj1:0x04D4
        add    esi, dword ptr [ebx + 4]
        movzx  ecx, byte ptr [eax + esi]
        cli    
        mov    byte ptr [0x4f4], cl          ; -> loc_1_04F4
        push   ecx
        push   esi
        shl    eax, 5
        mov    edi, 0x4f5                    ; -> obj1:0x04F5
        lea    esi, [eax + esi + 4]
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    esi
        pop    ecx
        mov    edi, 0x515                    ; -> obj1:0x0515
        lea    esi, [eax + esi + 0x84]
        rep movsb byte ptr es:[edi], byte ptr [esi]
        sti    
loc_3_04CE:
        ret    
        db  000h
loc_3_04D0:
        mov    eax, 0xffffffff
        cmp    ecx, 3
        jae    loc_3_04E6
        shl    ecx, 2
        call   sub_4_0284
loc_3_04E6:
        clc    
        ret    
loc_3_04E8:
        mov    ecx, 0x100
        xor    eax, eax
        ret    
sub_3_04F0:
        mov    eax, 0xffffffff
        ret    
loc_3_04F6:
        mov    eax, 0xffffffff
        cmp    dword ptr [esi + 0x10], 0
        je     loc_3_056E
        cmp    dword ptr [esi + 0x14], 0x10
        jne    loc_3_056E
        cmp    dword ptr [esi + 0x18], 0
        je     loc_3_056E
        cmp    dword ptr [esi + 0x1c], 0x16
        jne    loc_3_056E
        cmp    dword ptr [esi + 0x20], 0
        je     loc_3_056E
        mov    edi, dword ptr [esi + 0x20]
        or     edi, edi
        je     loc_3_056E
        mov    dword ptr [edi], 0x16
        mov    edi, dword ptr [esi + 0x18]
        mov    esi, dword ptr [esi + 0x10]
        mov    word ptr [edi], 0
        movzx  ecx, word ptr [esi + 2]
        xor    ebx, ebx
        xor    edx, edx
        test   ecx, 1
        je     loc_3_0546
        mov    edx, 4
loc_3_0546:
        or     ecx, ecx
        je     loc_3_056C
        inc    ebx
        shr    ecx, 1
        jae    loc_3_0546
        push   edi
        push   esi
        push   ebx
        push   ecx
        push   edx
        shl    ebx, 3
        add    ebx, edx
        call   dword ptr [ebx + 0x290]       ; -> obj4:0x0290
        pop    edx
        pop    ecx
        pop    ebx
        pop    esi
        pop    edi
        jae    loc_3_0546
        or     ax, bx
        mov    word ptr [edi], ax
loc_3_056C:
        xor    eax, eax
loc_3_056E:
        ret    
loc_3_056F:
        mov    ax, 0x900
        stc    
        ret    
        db  066h, 0B8h, 000h, 003h, 0F9h, 0C3h
loc_3_057B:
        mov    ax, 0x100
        stc    
        ret    
loc_3_0581:
        push   ecx
        push   edx
        VxDCall  VMD.Service_0007h               ; cd2007000c00
        shr    ax, 8
        and    ax, 0xf
        or     cx, cx
        je     loc_3_059A
        or     ax, 0x2000
loc_3_059A:
        mov    word ptr [edi + 4], ax
        pop    edx
        pop    ecx
        xor    ax, ax
        clc    
        ret    
loc_3_05A5:
        push   ebx
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        add    ebx, dword ptr [0x4d0]        ; -> obj1:0x04D0
        cli    
        mov    al, byte ptr [ebx + 0x12]
        push   ecx
        mov    bl, al
        and    al, 0xc
        mov    cl, 4
        shr    bl, cl
        or     al, bl
        mov    byte ptr [edi + 6], al
        pop    ecx
        sti    
        pop    ebx
        xor    ax, ax
        clc    
        ret    
loc_3_05CC:
        cli    
        mov    ax, word ptr [0x547]          ; -> obj1:0x0547
        mov    word ptr [edi + 8], ax
        mov    ax, word ptr [0x549]          ; -> obj1:0x0549
        mov    word ptr [edi + 0xa], ax
        xor    ax, ax
        mov    word ptr [0x547], ax          ; -> obj1:0x0547
        mov    word ptr [0x549], ax          ; -> obj1:0x0549
        sti    
        xor    ax, ax
        clc    
        ret    
loc_3_05F6:
        mov    ax, 0xa0a
        mov    word ptr [edi + 0xc], ax
        xor    ax, ax
        clc    
        ret    
loc_3_0603:
        push   ebx
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        add    ebx, dword ptr [0x4d0]        ; -> obj1:0x04D0
        cli    
        mov    ax, word ptr [ebx + 6]
        mov    word ptr [edi + 0xe], ax
        sti    
        pop    ebx
        xor    ax, ax
        clc    
        ret    
loc_3_0620:
        push   ebx
        push   ecx
        push   edx
        mov    ax, 0x30
        mov    cx, 0
        push   0x33
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        push   ecx
        mov    al, ch
        mov    bl, al
        and    al, 0xc
        mov    cl, 4
        shr    bl, cl
        or     al, bl
        mov    byte ptr [edi + 0x10], al
        mov    ch, al
        pop    ecx
        mov    al, cl
        mov    bl, al
        and    al, 0xc
        mov    cl, 4
        shr    bl, cl
        or     al, bl
        mov    byte ptr [edi + 0x11], al
        pop    edx
        pop    ecx
        pop    ebx
        xor    ax, ax
        clc    
        ret    
loc_3_065D:
        push   ebx
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        add    ebx, dword ptr [0x4d0]        ; -> obj1:0x04D0
        cli    
        mov    al, byte ptr [ebx + 0xd]
        mov    byte ptr [edi + 0x12], al
        sti    
        pop    ebx
        xor    ax, ax
        clc    
        ret    
loc_3_0678:
        push   ebx
        push   ecx
        push   edx
        mov    ax, 0x1b
        push   0x33
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        mov    ax, bx
        inc    ax
        inc    ax
        mov    dl, 5
        idiv   dl
        and    ax, 0xff
        mov    byte ptr [edi + 0x13], al
        mov    ax, cx
        inc    ax
        inc    ax
        mov    dl, 5
        idiv   dl
        and    ax, 0xff
        mov    byte ptr [edi + 0x14], al
        pop    edx
        pop    ecx
        pop    ebx
        xor    ax, ax
        clc    
        ret    
loc_3_06B3:
        push   ebx
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        add    ebx, dword ptr [0x4d0]        ; -> obj1:0x04D0
        cli    
        mov    al, byte ptr [ebx + 0xc]
        mov    byte ptr [edi + 0x15], al
        sti    
        pop    ebx
        xor    ax, ax
        clc    
        ret    
loc_3_06CE:
        clc    
        ret    
loc_3_06D0:
        push   ebx
        push   ecx
        push   edx
        mov    ax, 0x30
        mov    cx, 0
        push   0x33
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        cmp    ax, -1
        jne    loc_3_06EB
        mov    dl, bh
loc_3_06EB:
        mov    bx, word ptr [esi + 4]
        mov    ax, 0x30
        push   0x33
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        cmp    ax, -1
        jne    loc_3_0707
        mov    word ptr [edi + 0xe], dx
        jmp    loc_3_070B
loc_3_0707:
        mov    word ptr [edi + 0xe], bx
loc_3_070B:
        pop    edx
        pop    ecx
        pop    ebx
        xor    ax, ax
        clc    
        ret    
loc_3_0713:
        push   ebx
        push   ecx
        push   edx
        mov    ax, 0x30
        mov    cx, 0
        push   0x33
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        push   eax
        push   ebx
        mov    al, byte ptr [esi + 6]
        mov    bl, al
        and    al, 3
        and    bl, 0xc
        mov    cl, 4
        shl    al, cl
        or     al, bl
        mov    ch, al
        mov    al, byte ptr [esi + 7]
        mov    bl, al
        and    al, 3
        and    bl, 0xc
        mov    cl, 4
        shl    al, cl
        or     al, bl
        mov    cl, al
        pop    ebx
        pop    eax
        cmp    ax, -1
        jne    loc_3_075E
        mov    al, bh
        xchg   dx, bx
        xor    dx, dx
        mov    dl, al
loc_3_075E:
        mov    ax, 0x30
        push   0x33
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        pop    edx
        pop    ecx
        pop    ebx
        xor    ax, ax
        clc    
        ret    
loc_3_0772:
        push   ebx
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        add    ebx, dword ptr [0x4d0]        ; -> obj1:0x04D0
        cli    
        mov    al, byte ptr [esi + 8]
        mov    byte ptr [ebx + 0xd], al
        cmp    al, 0x21
        jne    loc_3_0791
        mov    byte ptr [0x4f2], 2           ; -> loc_1_04F2
loc_3_0791:
        sti    
        pop    ebx
        xor    ax, ax
        clc    
        ret    
loc_3_0798:
        push   ebx
        push   ecx
        push   edx
        mov    ax, 0x1b
        push   0x33
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        push   dx
        mov    al, byte ptr [esi + 9]
        mov    dl, 5
        imul   dl
        and    ax, 0xff
        mov    bx, ax
        mov    al, byte ptr [esi + 0xa]
        mov    dl, 5
        imul   dl
        and    ax, 0xff
        mov    cx, ax
        pop    dx
        or     bx, 0x800
        xor    ax, ax
        mov    al, byte ptr [esi + 9]
        mov    dx, ax
        mov    ax, 0x1a
        push   0x33
        VMMCall  VMM.Exec_VxD_Int                ; cd208f000100
        pop    edx
        pop    ecx
        pop    ebx
        xor    ax, ax
        clc    
        ret    
loc_3_07E9:
        mov    al, byte ptr [esi + 0xb]
        cmp    al, 0x20
        jne    loc_3_082D
        mov    ax, word ptr [esi + 0xc]
        cmp    ax, 0
        je     loc_3_0812
        mov    word ptr [0x4dc], ax          ; -> obj4:0x04DC
        mov    ax, word ptr [esi + 0xe]
        cmp    ax, 0
        je     loc_3_0812
        mov    word ptr [0x4de], ax          ; -> obj4:0x04DE
        jmp    loc_3_0824
loc_3_0812:
        mov    word ptr [0x4dc], 0x280       ; -> obj4:0x04DC
        mov    word ptr [0x4de], 0x1e0       ; -> obj4:0x04DE
loc_3_0824:
        cli    
        call   sub_3_0848
        sti    
        jmp    loc_3_0843
loc_3_082D:
        push   ebx
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        add    ebx, dword ptr [0x4d0]        ; -> obj1:0x04D0
        cli    
        mov    al, byte ptr [esi + 0xb]
        mov    byte ptr [ebx + 0xc], al
        sti    
        pop    ebx
loc_3_0843:
        xor    ax, ax
        clc    
        ret    
sub_3_0848:
        push   ebx
        push   ecx
        push   edx
        push   esi
        push   edi
        movzx  eax, word ptr [0x4dc]         ; -> obj4:0x04DC
        movzx  ebx, word ptr [0x4de]         ; -> obj4:0x04DE
        add    ax, bx
        shr    ax, 1
        xchg   bx, ax
        mov    esi, 0x8c                     ; -> obj4:0x008C
        mov    edi, 0x398                    ; -> obj4:0x0398
        mov    ecx, 0x41
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    esi, 0x318                    ; -> obj4:0x0318
        mov    ecx, 0x110                    ; -> obj4:0x0110
        mov    eax, 0x90                     ; -> obj4:0x0090
        sub    ecx, eax
        mov    edi, 0x39c                    ; -> obj4:0x039C
        cmp    word ptr [0x4dc], 0x280       ; -> obj4:0x04DC
        jne    loc_3_08A0
        cmp    word ptr [0x4de], 0x1e0       ; -> obj4:0x04DE
        je     loc_3_08C1
loc_3_08A0:
        xor    eax, eax
        mov    al, byte ptr [esi]
        cmp    al, 0x7f
        je     loc_3_08B5
        xor    edx, edx
        push   ecx
        mov    ecx, 0x230
        imul   ecx
        idiv   ebx
        pop    ecx
loc_3_08B5:
        jne    loc_3_08B9
        inc    al
loc_3_08B9:
        mov    byte ptr [edi], al
        inc    esi
        inc    edi
        loop   loc_3_08A0
        jmp    loc_3_08C3
loc_3_08C1:
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_3_08C3:
        mov    esi, 0x398                    ; -> obj4:0x0398
        mov    edi, dword ptr [0x4d4]        ; -> obj1:0x04D4
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        add    edi, dword ptr [ebx + 4]
        mov    ecx, 0x41
        cld    
        rep movsd dword ptr es:[edi], dword ptr [esi]
        call   sub_3_0484
        pop    edi
        pop    esi
        pop    edx
        pop    ecx
        pop    ebx
        ret    
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 4  vsize=0x4e0  RW-
        db  000h
sub_4_0001:
        salc   
        add    dword ptr [eax], eax
        add    byte ptr [edx], ah            ; -> loc_3_0222
        add    al, byte ptr [eax]
        add    byte ptr [ebp + 2], cl        ; -> loc_3_024D
        add    byte ptr [eax], al
        aam    1
        add    byte ptr [eax], al
        mov    eax, dword ptr [0xa9000002]   ; -> loc_3_02A9
        add    al, byte ptr [eax]
        add    byte ptr [edi], dl            ; -> loc_3_0317
        add    eax, dword ptr [eax]
        add    byte ptr [edi], ch            ; -> loc_3_032F
        add    eax, dword ptr [eax]
        add    byte ptr [esi + 3], al        ; -> loc_3_0346
        add    byte ptr [eax], al
        mov    bh, 3
        add    byte ptr [eax], al
        fadd   qword ptr [ebx]
        add    byte ptr [eax], al
loc_4_002D:
        add    byte ptr [edx], al
        add    eax, dword ptr [ecx]
loc_4_0031:
        add    byte ptr [ecx + ecx], al
        or     eax, 0x1f1b1612
        and    ebp, dword ptr [eax]
        sub    al, 0x31
        xor    eax, 0x46423e39
        dec    ebx
        dec    edi
        push   ebx
        push   edi
        pop    ebx
        pushal 
        push   0x7874706c
        jl     0xffffffd0
        add    dword ptr [edi - 0x696d7075], -0x67
        popfd  
        mov    al, byte ptr [0xaeaba7a4]
        mov    cl, 0xb4
        mov    bh, 0xba
        mov    esi, 0xc9c6c3c0
        int3   
        into   
        rcl    ebx, 1
        salc   
        fcomp  st(3)
        fstp   st(7)
        loope  0x54
        in     eax, 0xe7
        jmp    0xf0eeed63
        db  0F1h, 0F3h, 0F4h, 0F5h, 0F6h, 0F7h, 0F8h, 0F9h, 0FAh, 0FBh, 0FCh, 0FDh, 0FDh, 0FEh, 0FEh, 0FEh
        db  0FFh, 0FFh, 0FFh, 0FFh, 009h, 00Dh, 015h, 001h, 001h, 005h, 007h, 009h, 00Bh, 025h, 027h, 029h
        db  02Bh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh
        db  07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 001h, 009h, 00Ch, 00Fh, 012h, 015h, 018h, 01Bh
        db  01Eh, 021h, 025h, 029h, 02Eh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh
        db  07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 001h, 002h, 003h, 004h, 005h, 007h, 009h, 00Bh
        db  00Dh, 010h, 013h, 016h, 019h, 01Ch, 01Fh, 023h, 027h, 02Bh, 030h, 035h, 03Ch, 07Fh, 07Fh, 07Fh
        db  07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 001h, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh
        db  07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh
        db  07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 010h, 014h, 018h, 01Ch, 020h, 024h, 028h, 02Ch
        db  030h, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh
        db  07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 010h, 014h, 018h, 01Ch, 020h, 024h, 028h, 02Ch
        db  030h, 034h, 038h, 03Ch, 040h, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh
        db  07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 010h, 014h, 018h, 01Ch, 020h, 024h, 028h, 02Ch
        db  030h, 034h, 038h, 03Ch, 040h, 044h, 048h, 04Ch, 050h, 054h, 058h, 05Ch, 060h, 07Fh, 07Fh, 07Fh
        db  07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 010h, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh
        db  07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh
        db  07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 053h, 06Ch, 06Fh, 077h, 020h, 020h, 020h, 020h
        db  020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 04Dh, 06Fh, 064h, 065h, 072h, 061h, 074h, 065h
        db  020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 046h, 061h, 073h, 074h, 020h, 020h, 020h, 020h
        db  020h, 020h, 020h, 020h, 020h, 020h, 020h, 020h, 055h, 06Eh, 061h, 063h, 063h, 065h, 06Ch, 065h
        db  072h, 061h, 074h, 065h, 064h, 020h, 020h, 020h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 04Dh, 04Fh, 055h, 053h, 045h, 000h, 04Dh, 06Fh
        db  075h, 073h, 065h, 000h, 04Dh, 06Fh, 075h, 073h, 065h, 049h, 06Eh, 069h, 000h, 04Dh, 053h, 049h
        db  04Eh, 050h, 055h, 054h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
sub_4_0284:
        call   sub_3_04F0
        add    al, 0
        add    dh, dh                        ; -> loc_3_04F6
        add    al, 0
        add    byte ptr [edi + 5], ch        ; -> loc_3_056F
        add    byte ptr [eax], al
        outsd  dx, dword ptr [esi]
        add    eax, 0x56f0000                ; -> loc_3_056F
        add    byte ptr [eax], al
        into   
        push   es
        add    byte ptr [eax], al
        add    dword ptr [0x57b0000], 0x5a50000 ; -> loc_3_057B
        add    byte ptr [eax], al
        jnp    0x2b3
        add    byte ptr [eax], al
        int3   
        add    eax, 0x57b0000                ; -> loc_3_057B
        add    byte ptr [eax], al
        test   byte ptr [0x57b0000], 0       ; -> loc_3_057B
        add    byte ptr [ebx], al            ; -> loc_3_0603
        push   es
        add    byte ptr [eax], al
        rol    byte ptr [esi], 1
        add    byte ptr [eax], al
        and    byte ptr [esi], al
        add    byte ptr [eax], al
        adc    eax, dword ptr [edi]
        add    byte ptr [eax], al
        pop    ebp
        push   es
        add    byte ptr [eax], al
        jb     loc_4_02DD
        add    byte ptr [eax], al
        js     loc_4_02E0
        add    byte ptr [eax], al
        cwde   
loc_4_02DD:
        pop    es
        add    byte ptr [eax], al
loc_4_02E0:
        outsd  dx, dword ptr [esi]
        add    eax, 0x56f0000                ; -> loc_3_056F
        add    byte ptr [eax], al
        outsd  dx, dword ptr [esi]
        add    eax, 0x56f0000                ; -> loc_3_056F
        add    byte ptr [eax], al
        outsd  dx, dword ptr [esi]
        add    eax, 0x56f0000                ; -> loc_3_056F
        add    byte ptr [eax], al
        mov    bl, 6
        add    byte ptr [eax], al
        jmp    loc_3_056F
        db  005h, 000h, 000h
        dd  loc_3_056F               ; fixup
        dd  loc_3_056F               ; fixup
        dd  loc_3_056F               ; fixup
        dd  loc_3_056F               ; fixup
        dd  loc_3_056F               ; fixup
        db  001h, 005h, 007h, 009h, 00Bh, 025h, 027h, 029h, 02Bh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh
        db  07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh
        db  001h, 009h, 00Ch, 00Fh, 012h, 015h, 018h, 01Bh, 01Eh, 021h, 025h, 029h, 02Eh, 07Fh, 07Fh, 07Fh
        db  07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh
        db  001h, 002h, 003h, 004h, 005h, 007h, 009h, 00Bh, 00Dh, 010h, 013h, 016h, 019h, 01Ch, 01Fh, 023h
        db  027h, 02Bh, 030h, 035h, 03Ch, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh
        db  001h, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh
        db  07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh, 07Fh
        db  023h, 04Dh, 053h, 04Dh, 049h, 04Eh, 049h, 020h, 056h, 069h, 072h, 074h, 075h, 061h, 06Ch, 020h
        db  044h, 065h, 076h, 069h, 063h, 065h, 009h, 028h, 056h, 065h, 072h, 073h, 069h, 06Fh, 06Eh, 020h
        db  034h, 02Eh, 030h, 029h, 000h, 000h, 00Ah, 04Dh, 053h, 04Dh, 049h, 04Eh, 049h, 05Fh, 044h, 044h
        db  042h, 001h, 000h, 000h, 0FFh, 010h, 000h, 0FFh, 001h, 000h, 030h, 000h, 0E4h, 001h, 000h, 000h
        db  0E4h, 001h, 034h, 000h, 056h, 053h, 05Fh, 056h, 045h, 052h, 053h, 049h, 04Fh, 04Eh, 05Fh, 049h
        db  04Eh, 046h, 04Fh, 000h, 0BDh, 004h, 0EFh, 0FEh, 000h, 000h, 001h, 000h, 000h, 000h, 004h, 000h
        db  0B6h, 003h, 000h, 000h, 000h, 000h, 004h, 000h, 0B6h, 003h, 000h, 000h, 03Fh, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 001h, 000h, 001h, 000h, 005h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 075h, 001h, 000h, 000h, 053h, 074h, 072h, 069h
        db  06Eh, 067h, 046h, 069h, 06Ch, 065h, 049h, 06Eh, 066h, 06Fh, 000h, 000h, 061h, 001h, 000h, 000h
        db  030h, 034h, 030h, 039h, 030h, 034h, 045h, 034h, 000h, 000h, 000h, 000h, 026h, 000h, 016h, 000h
        db  043h, 06Fh, 06Dh, 070h, 061h, 06Eh, 079h, 04Eh, 061h, 06Dh, 065h, 000h, 04Dh, 069h, 063h, 072h
        db  06Fh, 073h, 06Fh, 066h, 074h, 020h, 043h, 06Fh, 072h, 070h, 06Fh, 072h, 061h, 074h, 069h, 06Fh
        db  06Eh, 000h, 000h, 000h, 039h, 000h, 025h, 000h, 046h, 069h, 06Ch, 065h, 044h, 065h, 073h, 063h
        db  072h, 069h, 070h, 074h, 069h, 06Fh, 06Eh, 000h, 04Dh, 053h, 04Dh, 049h, 04Eh, 049h, 020h, 056h
        db  069h, 072h, 074h, 075h, 061h, 06Ch, 020h, 044h, 065h, 076h, 069h, 063h, 065h, 020h, 020h, 028h
        db  056h, 065h, 072h, 073h, 069h, 06Fh, 06Eh, 020h, 034h, 02Eh, 030h, 029h, 000h, 000h, 000h, 000h
        db  019h, 000h, 009h, 000h, 046h, 069h, 06Ch, 065h, 056h, 065h, 072h, 073h, 069h, 06Fh, 06Eh, 000h
        db  034h, 02Eh, 030h, 030h, 02Eh, 039h, 035h, 030h, 000h, 000h, 000h, 000h, 01Bh, 000h, 007h, 000h
        db  049h, 06Eh, 074h, 065h, 072h, 06Eh, 061h, 06Ch, 04Eh, 061h, 06Dh, 065h, 000h, 000h, 000h, 000h
        db  04Dh, 053h, 04Dh, 049h, 04Eh, 049h, 000h, 000h
VxD__LDATA ENDS

VxD__LDATA SEGMENT   ; object 5  vsize=0x40  RW-
VxD__LDATA ENDS

VxD__LDATA SEGMENT   ; object 6  vsize=0x40  RW-
VxD__LDATA ENDS

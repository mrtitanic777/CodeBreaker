; DOSNET -- reconstructed 32-bit disassembly (VxD / LE format)
; 4 objects, 542 instructions recovered by recursive descent

Declare_Virtual_Device DOSNET, 1, 0, DOSNET_Control, 0001Ah, Undefined_Init_Order
;   DDK version 0400h   init_order 05C000000h   flags 00000h
;   service table: obj1:0x1b4, 3 services

VxD__LTEXT SEGMENT   ; object 1  vsize=0x224  R-X
DOSNET_Control:
        sub    eax, 1
        cmp    eax, 5
        jae    loc_1_000F
        jmp    dword ptr [eax*4 + 0x11]      ; -> obj1:0x0011
loc_1_000F:
        clc    
        ret    
        ; ---- dispatch table (5 entries) ----
        dd  loc_2_0000
        dd  loc_1_000F
        dd  loc_1_000F
        dd  loc_3_002E
        dd  loc_3_000C
        db  02Eh, 08Bh, 0C0h
loc_1_0028:
        cmp    byte ptr [ebp + 0x1d], 0x5f
        jne    loc_1_0126
        mov    al, byte ptr [ebp + 0x1c]
        cmp    al, 0x47
        jae    loc_1_0128
        cmp    byte ptr [ebp + 0x1c], 4
        je     loc_1_00BF
        cmp    byte ptr [ebp + 0x1c], 3
        jne    loc_1_0126
        cmp    byte ptr [ebp + 0x10], 0x84
        je     loc_1_005D
        cmp    byte ptr [ebp + 0x10], 4
        jne    loc_1_0126
loc_1_005D:
        movzx  esi, word ptr [ebp + 0x3c]
        shl    esi, 4
        movzx  eax, word ptr [ebp + 4]
        add    esi, eax
        movzx  edi, word ptr [ebp + 0x38]
        shl    edi, 4
        movzx  eax, word ptr [ebp]
        add    edi, eax
loc_1_0077:
        add    esi, dword ptr [ebx + 4]
        add    edi, dword ptr [ebx + 4]
        cmp    byte ptr [esi], 0
        je     loc_1_0126
        cmp    word ptr [esi + 1], 0x3a
        jne    loc_1_0126
        movzx  eax, byte ptr [esi]
        or     al, 0x20
        sub    al, 0x61
        jb     loc_1_0126
        cmp    al, 0x1a
        jae    loc_1_0126
        push   eax
        mov    ecx, 1
        call   sub_3_005E
        pop    eax
        jb     loc_1_0126
        mov    dword ptr [edx], eax
        mov    dword ptr [edx + 8], 0
        jmp    loc_1_0116
loc_1_00BF:
        movzx  esi, word ptr [ebp + 0x3c]
        shl    esi, 4
        movzx  eax, word ptr [ebp + 4]
        add    esi, eax
loc_1_00CC:
        add    esi, dword ptr [ebx + 4]
        cmp    word ptr [esi], 0x2f2f
        je     loc_1_0126
        cmp    word ptr [esi], 0x5c5c
        je     loc_1_0126
        cmp    word ptr [esi], 0x5c2f
        je     loc_1_0126
        cmp    word ptr [esi], 0x2f5c
        je     loc_1_0126
        cmp    word ptr [esi + 1], 0x3a
        jne    loc_1_0126
        movzx  eax, byte ptr [esi]
        or     al, 0x20
        sub    al, 0x61
        jb     loc_1_0126
        cmp    al, 0x1a
        jae    loc_1_0126
        push   eax
        mov    ecx, 1
        call   sub_3_005E
        pop    eax
        jb     loc_1_0126
        mov    dword ptr [edx], eax
        mov    dword ptr [edx + 8], 1
loc_1_0116:
        mov    eax, 0x4e20
        mov    esi, 0xa1                     ; -> loc_3_00A1
        VMMCall  VMM.Call_When_VM_Returns        ; cd200d000100
loc_1_0126:
        stc    
        ret    
loc_1_0128:
        je     loc_1_017B
        cmp    al, 0x48
        je     loc_1_019F
        cmp    al, 0x55
        jne    loc_1_0126
        mov    ecx, dword ptr [0x220]        ; -> obj1:0x0220
        call   sub_3_005E
        jb     loc_1_0126
        mov    dword ptr [edx], 0xffffffff
        mov    dword ptr [edx + 8], 1
        mov    dword ptr [edx + 4], 0
        mov    ecx, dword ptr [0x220]        ; -> obj1:0x0220
        mov    esi, dword ptr [0x218]        ; -> obj1:0x0218
        add    esi, dword ptr [ebx + 4]
        xor    eax, eax
loc_1_0164:
        test   word ptr [esi + 0x43], 0x8000
        je     loc_1_0170
        bts    dword ptr [edx + 4], eax
loc_1_0170:
        inc    eax
        add    esi, dword ptr [0x21c]        ; -> loc_1_021C
        loop   loc_1_0164
        jmp    loc_1_0116
loc_1_017B:
        movzx  esi, word ptr [ebp + 0x38]
        shl    esi, 4
        movzx  eax, word ptr [ebp]
        add    esi, eax
        mov    edi, dword ptr [esi + 0xa]
        mov    eax, edi
        movzx  eax, ax
        shr    edi, 0x10
        shl    edi, 4
        add    edi, eax
        lea    esi, [esi]
        jmp    loc_1_0077
loc_1_019F:
        movzx  esi, word ptr [ebp + 0x38]
        shl    esi, 4
        movzx  eax, word ptr [ebp]
        add    esi, eax
        jmp    loc_1_00CC
        db  000h, 000h, 000h
        dd  DOSNET_Get_Version       ; fixup
        dd  DOSNET_Send_FILESYSCHANGE ; fixup
        dd  DOSNET_Do_PSP_Adjust     ; fixup
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  0001Ah                      ; DDB_Req_Device_Number
        db  1, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'DOSNET  '                  ; DDB_Name
        dd  05C000000h                  ; DDB_Init_Order
        dd  DOSNET_Control           ; DDB_Control_Proc
        dd  0                        ; DDB_V86_API_Proc
        dd  0                        ; DDB_PM_API_Proc
        dd  obj1:0x01B4              ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
loc_1_021C:
        pop    eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0xa4  R-X
loc_2_0000:
        push   0
        push   0x6c
        VMMCall  VMM._Allocate_Device_CB_Area    ; cd20a7000100
        pop    ecx
        pop    ecx
        or     eax, eax
        je     loc_2_009C
        mov    dword ptr [0x214], eax        ; -> obj1:0x0214
        mov    eax, 0x21
        mov    esi, 0x28                     ; -> loc_1_0028
        VMMCall  VMM.Hook_V86_Int_Chain          ; cd2041000100
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        mov    esi, dword ptr [ebx + 8]
        mov    word ptr [esi + 0x1c], 0x5200
        mov    eax, 0x21
        VMMCall  VMM.Exec_Int                    ; cd2084000100
        movzx  edi, word ptr [esi + 0x38]
        shl    edi, 4
        movzx  ecx, word ptr [esi + 0x10]
        add    edi, ecx
        mov    eax, dword ptr [edi + 0x16]
        movzx  ecx, ax
        shr    eax, 0x10
        shl    eax, 4
        add    eax, ecx
        mov    dword ptr [0x218], eax        ; -> obj1:0x0218
        movzx  eax, byte ptr [edi + 0x21]
        mov    dword ptr [0x220], eax        ; -> obj1:0x0220
        mov    dword ptr [0x21c], 0x58       ; -> loc_1_021C
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        clc    
        ret    
loc_2_009C:
        VMMCall  VMM.Fatal_Memory_Error          ; cd20bf000100
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x3cb  R-X
DOSNET_Get_Version:
        mov    eax, 0x300
        clc    
        ret    
DOSNET_Do_PSP_Adjust:
        xor    eax, eax
        ret    
DOSNET_Send_FILESYSCHANGE:
        clc    
        ret    
loc_3_000C:
        mov    eax, dword ptr [0x220]        ; -> obj1:0x0220
        mul    dword ptr [0x21c]             ; -> loc_1_021C
        mov    ecx, eax
        mov    esi, dword ptr [0x210]        ; -> obj1:0x0210
        or     esi, esi
        je     loc_3_002C
        mov    edi, dword ptr [0x218]        ; -> obj1:0x0218
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_3_002C:
        clc    
        ret    
loc_3_002E:
        mov    eax, dword ptr [0x220]        ; -> obj1:0x0220
        mul    dword ptr [0x21c]             ; -> loc_1_021C
        mov    esi, eax
        push   0
        push   eax
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        pop    ecx
        pop    ecx
        or     eax, eax
        je     loc_3_005C
        mov    dword ptr [0x210], eax        ; -> obj1:0x0210
        mov    edi, eax
        mov    ecx, esi
        mov    esi, dword ptr [0x218]        ; -> obj1:0x0218
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_3_005C:
        clc    
        ret    
sub_3_005E:
        push   ecx
        push   1
        push   0x14
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        pop    ecx
        pop    ecx
        pop    ecx
        or     eax, eax
        stc    
        je     loc_3_0092
        mov    edx, eax
        jecxz  loc_3_0091
        push   edx
        mov    eax, ecx
        mul    dword ptr [0x21c]             ; -> loc_1_021C
        push   0
        push   eax
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        pop    ecx
        pop    ecx
        pop    edx
        or     eax, eax
        je     loc_3_0093
        mov    dword ptr [edx + 0xc], eax
loc_3_0091:
        clc    
loc_3_0092:
        ret    
loc_3_0093:
        push   0
        push   edx
        VMMCall  VMM._HeapFree                   ; cd2051000100
        pop    ecx
        pop    ecx
        stc    
        jmp    loc_3_0092
loc_3_00A1:
        jb     loc_3_016F
        test   dword ptr [ebp + 0x2c], 1
        jne    loc_3_016F
        mov    eax, dword ptr [edx]
        inc    eax
        je     loc_3_01F1
        dec    eax
        cmp    eax, dword ptr [0x220]        ; -> obj1:0x0220
        jae    loc_3_016F
        mov    eax, ebx
        VMMCall  VMM.Get_Next_VM_Handle          ; cd203b000100
        push   eax
        cmp    ebx, eax
        je     loc_3_016E
        push   edx
        mov    esi, dword ptr [eax + 4]
        mov    eax, dword ptr [edx]
        mul    dword ptr [0x21c]             ; -> loc_1_021C
        add    eax, dword ptr [0x218]        ; -> obj1:0x0218
        add    esi, eax
        pop    edx
        mov    edi, dword ptr [edx + 0xc]
        mov    ecx, dword ptr [0x21c]        ; -> loc_1_021C
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_3_00FC:
        test   dword ptr [ebx], 0x10
        jne    loc_3_0163
        test   dword ptr [ebx], 8
        jne    loc_3_0195
        mov    edi, dword ptr [0x214]        ; -> obj1:0x0214
        add    edi, ebx
        mov    ecx, dword ptr [edx]
        xor    esi, esi
        xchg   dword ptr [edi + ecx*4 + 4], esi
        or     esi, esi
        je     loc_3_0152
        cmp    dword ptr [esi + 0x10], 0
        je     loc_3_012F
        dec    dword ptr [esi + 0x10]
        jne    loc_3_0152
loc_3_012F:
        push   eax
        push   edx
        xor    eax, eax
        xchg   dword ptr [esi + 0xc], eax
        or     eax, eax
        je     loc_3_0145
        push   0
        push   eax
        VMMCall  VMM._HeapFree                   ; cd2051000100
        pop    ecx
        pop    ecx
loc_3_0145:
        push   0
        push   esi
        VMMCall  VMM._HeapFree                   ; cd2051000100
        pop    ecx
        pop    ecx
        pop    edx
        pop    eax
loc_3_0152:
        mov    edi, dword ptr [ebx + 4]
        add    edi, eax
        mov    esi, dword ptr [edx + 0xc]
        mov    ecx, dword ptr [0x21c]        ; -> loc_1_021C
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_3_0163:
        VMMCall  VMM.Get_Next_VM_Handle          ; cd203b000100
        cmp    ebx, dword ptr [esp]
        jne    loc_3_00FC
loc_3_016E:
        pop    ebx
loc_3_016F:
        cmp    dword ptr [edx + 0x10], 0
        jne    loc_3_0194
        mov    eax, dword ptr [edx + 0xc]
        or     eax, eax
        je     loc_3_0189
        push   edx
        push   0
        push   eax
        VMMCall  VMM._HeapFree                   ; cd2051000100
        pop    ecx
        pop    ecx
        pop    edx
loc_3_0189:
        push   0
        push   edx
        VMMCall  VMM._HeapFree                   ; cd2051000100
        pop    ecx
        pop    ecx
loc_3_0194:
        ret    
loc_3_0195:
        mov    edi, dword ptr [0x214]        ; -> obj1:0x0214
        add    edi, ebx
        mov    ecx, dword ptr [edx]
        mov    esi, edx
        xchg   dword ptr [edi + ecx*4 + 4], esi
        inc    dword ptr [edx + 0x10]
        or     esi, esi
        je     loc_3_01DA
        cmp    dword ptr [esi + 0x10], 0
        je     loc_3_01B7
        dec    dword ptr [esi + 0x10]
        jne    loc_3_01DA
loc_3_01B7:
        push   eax
        push   edx
        xor    eax, eax
        xchg   dword ptr [esi + 0xc], eax
        or     eax, eax
        je     loc_3_01CD
        push   0
        push   eax
        VMMCall  VMM._HeapFree                   ; cd2051000100
        pop    ecx
        pop    ecx
loc_3_01CD:
        push   0
        push   esi
        VMMCall  VMM._HeapFree                   ; cd2051000100
        pop    ecx
        pop    ecx
        pop    edx
        pop    eax
loc_3_01DA:
        cmp    dword ptr [edi], 0
        jne    loc_3_0163
        mov    esi, 0x345                    ; -> loc_3_0345
        VMMCall  VMM.Schedule_VM_Event           ; cd200f000100
        mov    dword ptr [edi], esi
        jmp    loc_3_0163
loc_3_01F1:
        mov    ecx, ebx
        VMMCall  VMM.Get_Next_VM_Handle          ; cd203b000100
        cmp    ebx, ecx
        push   ecx
        je     loc_3_016E
        pop    ecx
        cmp    dword ptr [edx + 4], 0
        je     loc_3_016F
        mov    esi, dword ptr [ecx + 4]
        add    esi, dword ptr [0x218]        ; -> obj1:0x0218
        mov    edi, dword ptr [edx + 0xc]
        mov    eax, dword ptr [0x220]        ; -> obj1:0x0220
        push   edx
        mul    dword ptr [0x21c]             ; -> loc_1_021C
        pop    edx
        push   ecx
        mov    ecx, eax
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
loc_3_022C:
        mov    edi, dword ptr [0x214]        ; -> obj1:0x0214
        add    edi, ebx
        test   dword ptr [ebx], 0x10
        jne    loc_3_02CF
        test   dword ptr [ebx], 8
        jne    loc_3_02E3
        xor    eax, eax
        mov    ecx, dword ptr [0x220]        ; -> obj1:0x0220
loc_3_0254:
        bt     dword ptr [edx + 4], eax
        jae    loc_3_0294
        xor    esi, esi
        xchg   dword ptr [edi + eax*4 + 4], esi
        or     esi, esi
        je     loc_3_0294
        cmp    dword ptr [esi + 0x10], 0
        je     loc_3_026F
        dec    dword ptr [esi + 0x10]
        jne    loc_3_0294
loc_3_026F:
        push   eax
        push   edx
        push   ecx
        xor    eax, eax
        xchg   dword ptr [esi + 0xc], eax
        or     eax, eax
        je     loc_3_0286
        push   0
        push   eax
        VMMCall  VMM._HeapFree                   ; cd2051000100
        pop    ecx
        pop    ecx
loc_3_0286:
        push   0
        push   esi
        VMMCall  VMM._HeapFree                   ; cd2051000100
        pop    ecx
        pop    ecx
        pop    ecx
        pop    edx
        pop    eax
loc_3_0294:
        inc    eax
        loop   loc_3_0254
        mov    edi, dword ptr [ebx + 4]
        add    edi, dword ptr [0x218]        ; -> obj1:0x0218
        mov    esi, dword ptr [edx + 0xc]
        xor    eax, eax
        mov    ecx, dword ptr [0x220]        ; -> obj1:0x0220
loc_3_02AB:
        bt     dword ptr [edx + 4], eax
        jae    loc_3_02C0
        push   ecx
        push   esi
        push   edi
        mov    ecx, dword ptr [0x21c]        ; -> loc_1_021C
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    edi
        pop    esi
        pop    ecx
loc_3_02C0:
        inc    eax
        add    esi, dword ptr [0x21c]        ; -> loc_1_021C
        add    edi, dword ptr [0x21c]        ; -> loc_1_021C
        loop   loc_3_02AB
loc_3_02CF:
        VMMCall  VMM.Get_Next_VM_Handle          ; cd203b000100
        cmp    ebx, dword ptr [esp]
        jne    loc_3_022C
        jmp    loc_3_016E
loc_3_02E3:
        mov    ecx, dword ptr [0x220]        ; -> obj1:0x0220
        xor    eax, eax
loc_3_02EB:
        bt     dword ptr [edx + 4], eax
        jae    loc_3_032E
        mov    esi, edx
        xchg   dword ptr [edi + eax*4 + 4], esi
        inc    dword ptr [edx + 0x10]
        or     esi, esi
        je     loc_3_032E
        cmp    dword ptr [esi + 0x10], 0
        je     loc_3_0309
        dec    dword ptr [esi + 0x10]
        jne    loc_3_032E
loc_3_0309:
        push   eax
        push   ecx
        push   edx
        xor    eax, eax
        xchg   dword ptr [esi + 0xc], eax
        or     eax, eax
        je     loc_3_0320
        push   0
        push   eax
        VMMCall  VMM._HeapFree                   ; cd2051000100
        pop    ecx
        pop    ecx
loc_3_0320:
        push   0
        push   esi
        VMMCall  VMM._HeapFree                   ; cd2051000100
        pop    ecx
        pop    ecx
        pop    edx
        pop    ecx
        pop    eax
loc_3_032E:
        inc    eax
        loop   loc_3_02EB
        cmp    dword ptr [edi], 0
        jne    loc_3_02CF
        mov    esi, 0x345                    ; -> loc_3_0345
        VMMCall  VMM.Schedule_VM_Event           ; cd200f000100
        mov    dword ptr [edi], esi
        jmp    loc_3_02CF
loc_3_0345:
        mov    edi, dword ptr [0x214]        ; -> obj1:0x0214
        add    edi, ebx
        lea    esi, [edi + 4]
        mov    ecx, 0x1a
        xor    eax, eax
loc_3_0357:
        xor    edx, edx
        xchg   dword ptr [esi], edx
        or     edx, edx
        je     loc_3_03BD
        push   esi
        push   edi
        push   ecx
        push   eax
        push   edx
        mul    dword ptr [0x21c]             ; -> loc_1_021C
        mov    edi, dword ptr [ebx + 4]
        add    edi, dword ptr [0x218]        ; -> obj1:0x0218
        add    edi, eax
        pop    edx
        mov    esi, dword ptr [edx + 0xc]
        cmp    dword ptr [edx], -1
        jne    loc_3_0380
        add    esi, eax
loc_3_0380:
        mov    ecx, dword ptr [0x21c]        ; -> loc_1_021C
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    eax
        pop    ecx
        pop    edi
        pop    esi
        cmp    dword ptr [edx + 0x10], 0
        je     loc_3_0398
        dec    dword ptr [edx + 0x10]
        jne    loc_3_03BD
loc_3_0398:
        push   eax
        push   ecx
        push   edx
        xor    eax, eax
        xchg   dword ptr [edx + 0xc], eax
        or     eax, eax
        je     loc_3_03AF
        push   0
        push   eax
        VMMCall  VMM._HeapFree                   ; cd2051000100
        pop    ecx
        pop    ecx
loc_3_03AF:
        pop    edx
        push   0
        push   edx
        VMMCall  VMM._HeapFree                   ; cd2051000100
        pop    ecx
        pop    ecx
        pop    ecx
        pop    eax
loc_3_03BD:
        inc    eax
        add    esi, 4
        dec    ecx
        jne    loc_3_0357
        mov    dword ptr [edi], 0
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0xb4  R-X
        db  0B8h, 000h, 030h, 0CDh, 021h, 086h, 0E0h, 03Dh, 00Ah, 003h, 072h, 023h, 0B8h, 002h, 05Fh, 033h
        db  0DBh, 00Eh, 01Fh, 00Eh, 007h, 0BEh, 034h, 000h, 0BFh, 034h, 000h, 0CDh, 021h, 073h, 005h, 083h
        db  0F8h, 001h, 074h, 00Bh, 0B8h, 000h, 000h, 033h, 0DBh, 066h, 033h, 0D2h, 033h, 0F6h, 0C3h, 0B8h
        db  001h, 080h, 0EBh, 0F3h, 023h, 044h, 04Fh, 053h, 04Eh, 045h, 054h, 020h, 056h, 069h, 072h, 074h
        db  075h, 061h, 06Ch, 020h, 044h, 065h, 076h, 069h, 063h, 065h, 009h, 028h, 056h, 065h, 072h, 073h
        db  069h, 06Fh, 06Eh, 020h, 034h, 02Eh, 030h, 029h, 000h, 000h, 00Ah, 044h, 04Fh, 053h, 04Eh, 045h
        db  054h, 05Fh, 044h, 044h, 042h, 001h, 000h, 000h, 0FFh, 010h, 000h, 0FFh, 001h, 000h, 030h, 000h
        db  0E4h, 001h, 000h, 000h, 0E4h, 001h, 034h, 000h, 056h, 053h, 05Fh, 056h, 045h, 052h, 053h, 049h
        db  04Fh, 04Eh, 05Fh, 049h, 04Eh, 046h, 04Fh, 000h, 0BDh, 004h, 0EFh, 0FEh, 000h, 000h, 001h, 000h
        db  000h, 000h, 004h, 000h, 0B6h, 003h, 000h, 000h, 000h, 000h, 004h, 000h, 0B6h, 003h, 000h, 000h
        db  03Fh, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 001h, 000h, 001h, 000h, 005h, 000h, 000h, 000h
        db  01Ah, 000h, 000h, 000h
VxD__LTEXT ENDS

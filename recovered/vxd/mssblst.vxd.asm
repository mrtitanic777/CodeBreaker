; mssblst -- reconstructed 32-bit disassembly (VxD / LE format)
; 7 objects, 3678 instructions recovered by recursive descent

Declare_Virtual_Device mssblst, 4, 0, mssblst_Control, 0044Eh, Undefined_Init_Order
;   DDK version 0400h   init_order 080000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x348  R-X
        db  083h, 0FAh, 09Ah, 027h, 081h, 049h, 0CEh, 011h, 0A5h, 021h, 000h, 020h, 0AFh, 00Bh, 0E5h, 060h
        db  085h, 0FAh, 09Ah, 027h, 081h, 049h, 0CEh, 011h, 0A5h, 021h, 000h, 020h, 0AFh, 00Bh, 0E5h, 060h
        db  084h, 0FAh, 09Ah, 027h, 081h, 049h, 0CEh, 011h, 0A5h, 021h, 000h, 020h, 0AFh, 00Bh, 0E5h, 060h
        db  086h, 0FAh, 09Ah, 027h, 081h, 049h, 0CEh, 011h, 0A5h, 021h, 000h, 020h, 0AFh, 00Bh, 0E5h, 060h
loc_1_0040:
        sal    byte ptr [ebx], 0x42
        mov    esp, cs
        mov    ah, 0xce
        adc    dword ptr [edx + 0x53454494], edx
        push   esp
        add    byte ptr [eax], al
loc_1_0050:
        sal    dword ptr [ebx], 0x42
        mov    esp, cs
        mov    ah, 0xce
        adc    dword ptr [edx + 0x53454494], edx
        push   esp
        add    byte ptr [eax], al
loc_1_0060:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        rol    byte ptr [eax], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esi
        add    dword ptr [eax], eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        rol    byte ptr [eax], 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        inc    esi
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  0044Eh                      ; DDB_Req_Device_Number
        db  4, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'mssblst '                  ; DDB_Name
        dd  080000000h                  ; DDB_Init_Order
        dd  mssblst_Control          ; DDB_Control_Proc
        dd  mssblst_PM_API           ; DDB_V86_API_Proc
        dd  mssblst_PM_API           ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_00D8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_00DC:
        inc    eax
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax + eax], ah
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   es
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], cl
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        db  000h
mssblst_Control:
        cmp    eax, 8
        je     loc_5_10EB
loc_1_0105:
        cmp    eax, 0xb
        je     loc_5_108C
loc_1_010E:
        cmp    eax, 3
        je     loc_5_10EB
loc_1_0117:
        cmp    eax, 0x1b
        je     loc_7_0000
loc_1_0120:
        cmp    eax, 0x1c
        je     loc_3_0000
loc_1_0129:
        cmp    eax, 0x22
        je     loc_4_066A
loc_1_0132:
        clc    
        ret    
sub_1_0134:
        push   ebp
        mov    ebp, esp
        push   eax
        push   ebx
        push   edx
        push   esi
        pushfd 
        mov    eax, dword ptr [ebp + 0xc]
        shl    eax, 3
        mov    edx, dword ptr [eax]          ; -> loc_1_00DC
        mov    eax, dword ptr [eax + 4]      ; -> obj1:0x00E0
        mov    dword ptr [ebp + 0xc], eax
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, dword ptr [0]            ; -> obj1:0x00D4
        or     esi, esi
        je     loc_1_018B
        cli    
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_1_018B
loc_1_0167:
        mov    edi, dword ptr [eax]
        test   dword ptr [ebp + 0xc], 1
        jne    loc_1_017B
        cmp    dword ptr [edx + edi], ebx
        jne    loc_1_0181
loc_1_0177:
        popfd  
        clc    
        jmp    loc_1_018F
loc_1_017B:
        cmp    word ptr [edx + edi], bx
        je     loc_1_0177
loc_1_0181:
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        je     loc_1_018B
        jmp    loc_1_0167
loc_1_018B:
        popfd  
        xor    edi, edi
        stc    
loc_1_018F:
        pop    esi
        pop    edx
        pop    ebx
        pop    eax
        pop    ebp
        ret    
sub_1_0195:
        push   eax
        push   ebx
        pushfd 
        add    ebx, dword ptr [0]            ; -> obj1:0x00D0
        mov    esi, dword ptr [ebx]
        or     esi, esi
        je     loc_1_01C2
        cli    
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_1_01C2
loc_1_01AD:
        cmp    dword ptr [eax + 4], edi
        jne    loc_1_01B8
        mov    esi, eax
        popfd  
        clc    
        jmp    loc_1_01C6
loc_1_01B8:
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        je     loc_1_01C2
        jmp    loc_1_01AD
loc_1_01C2:
        popfd  
        xor    esi, esi
        stc    
loc_1_01C6:
        pop    ebx
        pop    eax
        ret    
        db  02Eh, 08Bh, 0C0h, 0EBh, 006h, 0FFh, 025h
        dd  loc_1_00D8               ; fixup
loc_1_01D4:
        pushal 
        pushfd 
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_1_0223
        mov    esi, dword ptr [0]            ; -> obj1:0x00D4
        or     esi, esi
        je     loc_1_0223
        cli    
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        popfd  
        pushfd 
        or     eax, eax
        je     loc_1_0223
loc_1_01F5:
        xor    ecx, ecx
        mov    edi, dword ptr [eax]
        cmp    dword ptr [edi + 0x28], ebx
        jne    loc_1_0201
        or     ecx, 1
loc_1_0201:
        cmp    dword ptr [edi + 0x30], ebx
        jne    loc_1_0209
        or     ecx, 2
loc_1_0209:
        jecxz  loc_1_0214
        push   eax
        mov    eax, ecx
        call   sub_5_1149
loc_1_0213:
        pop    eax
loc_1_0214:
        cli    
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        popfd  
        pushfd 
        or     eax, eax
        je     loc_1_0223
        jmp    loc_1_01F5
loc_1_0223:
        popfd  
        popal  
        jmp    loc_1_00D8
        db  090h
loc_1_022C:
        VxDCall  VDMAD.VDMAD_Default_Handler     ; cd2012800400
        mov    eax, eax
loc_1_0234:
        push   edi
        push   edx
        push   ebx
        push   1
        push   eax
        call   sub_1_0134
        add    esp, 8
        or     edi, edi
        je     loc_1_025E
        mov    ebx, dword ptr [edi + 0x28]
        or     ebx, ebx
        je     loc_1_0254
        cmp    word ptr [edi + 0x50], 0
        je     loc_1_0266
loc_1_0254:
        push   eax
        movzx  edx, word ptr [edi + 6]
        add    edx, 0xe
        in     al, dx
        pop    eax
loc_1_025E:
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
        jmp    loc_1_026C
loc_1_0266:
        VxDCall  VPICD.VPICD_Set_Int_Request     ; cd2002000300
loc_1_026C:
        pop    ebx
        pop    edx
        pop    edi
        clc    
        ret    
        db  02Eh, 08Bh, 0C0h
loc_1_0274:
        VxDCall  VPICD.VPICD_Clear_Int_Request   ; cd2003000300
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
        clc    
        ret    
        db  08Bh, 0C0h
loc_1_0284:
        push   edi
        push   1
        push   eax
        call   sub_1_0134
        add    esp, 8
        or     edi, edi
        je     loc_1_02AB
        cmp    dword ptr [edi + 0x28], ebx
        jne    loc_1_02AB
        jecxz  loc_1_02A3
loc_1_029B:
        VxDCall  VPICD.VPICD_Physically_Mask     ; cd2008000300
        jmp    loc_1_02B5
loc_1_02A3:
        VxDCall  VPICD.VPICD_Physically_Unmask   ; cd2009000300
        jmp    loc_1_02B5
loc_1_02AB:
        test   word ptr [edi + 4], 0x8000
        jne    loc_1_02A3
        je     loc_1_029B
loc_1_02B5:
        pop    edi
        clc    
        ret    
sub_1_02B8:
        push   ecx
        push   edx
        add    edx, 0xe
        mov    ecx, 0x5dc
loc_1_02C2:
        in     al, dx
        or     al, al
        js     loc_1_02CC
        loop   loc_1_02C2
        stc    
        jmp    loc_1_02D1
loc_1_02CC:
        sub    edx, 4
        in     al, dx
        clc    
loc_1_02D1:
        pop    edx
        pop    ecx
        ret    
sub_1_02D4:
        push   eax
        push   ecx
        push   edx
        add    edx, 0xc
        push   eax
        mov    ah, 0x14
loc_1_02DD:
        mov    ecx, 0x7fff
loc_1_02E2:
        in     al, dx
        or     al, al
        jns    loc_1_02F1
        loop   loc_1_02E2
        dec    ah
        jne    loc_1_02DD
        pop    eax
        stc    
        jmp    loc_1_02F4
loc_1_02F1:
        pop    eax
        out    dx, al
        clc    
loc_1_02F4:
        pop    edx
        pop    ecx
        pop    eax
        ret    
sub_1_02F8:
        push   eax
        push   edx
        push   ecx
        add    edx, 6
        mov    al, 1
        out    dx, al
        mov    ecx, 0xa
loc_1_0306:
        in     al, dx
        dec    ecx
        jne    loc_1_0306
        xor    al, al
        out    dx, al
        mov    ecx, 0xa
loc_1_0312:
        in     al, dx
        dec    ecx
        jne    loc_1_0312
        sub    edx, 6
        call   sub_1_02B8
        pop    ecx
        pop    edx
        pop    eax
        ret    
sub_1_0322:
        push   eax
        in     al, dx
        jmp    loc_1_0326
loc_1_0326:
        in     al, dx
        jmp    loc_1_0329
loc_1_0329:
        pop    eax
        ret    
sub_1_032B:
        push   ebx
        push   edx
        push   eax
        mov    eax, ebx
        shl    ah, 1
        add    dl, ah
        out    dx, al
        sub    dl, ah
        call   sub_1_0322
        pop    eax
        inc    dl
        out    dx, al
        call   sub_1_0322
        pop    edx
        pop    ebx
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0xdf  R-X
        db  003h, 000h, 000h, 000h
        dd  obj2:0x0014              ; fixup
        db  000h, 001h, 000h, 000h, 000h, 000h
loc_2_000E:
        dec    ebx
        add    byte ptr [esi + 0x5500cb00], ah
        outsb  dx, byte ptr [esi]
        popal  
        bound  ebp, qword ptr [ebp + 0x20]
        je     0x8c
        and    byte ptr [eax + 0x6c], dh
        popal  
        jns    0x43
        jae    loc_2_0094
        jne    0x95
        and    byte ptr fs:[0x65687420], ch
        and    byte ptr [ebx + 0x6f], dl
        jne    loc_2_00A1
        and    byte ptr fs:[edx + 0x6c], al
        popal  
        jae    loc_2_00AE
        jb     loc_2_005D
        imul   esi, dword ptr [ebx + 0x20], 0x75206e69
        jae    0xab
        and    byte ptr [edx + 0x79], ah
        and    byte ptr [ecx + 0x6e], ah
        outsd  dx, dword ptr [esi]
        je     0xb7
        jb     loc_2_0072
        popal  
        jo     0xc5
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [ebx + 0x61], 0x6e6f6974
loc_2_005D:
        add    byte ptr cs:[ebp + 0x6e], dl
        popal  
        bound  ebp, qword ptr [ebp + 0x20]
        je     0xd7
        and    byte ptr [eax + 0x6c], dh
        popal  
        jns    loc_2_008E
        dec    ebp
        dec    ecx
        inc    esp
        dec    ecx
loc_2_0072:
        and    byte ptr [0x65687420], ch
        and    byte ptr [ebx + 0x6f], dl
        jne    0xeb
        and    byte ptr fs:[edx + 0x6c], al
        popal  
        jae    0xf8
        jb     loc_2_00A7
        dec    ebp
        dec    ecx
        inc    esp
        dec    ecx
        and    byte ptr [ebx + 0x79], dl
loc_2_008E:
        outsb  dx, byte ptr [esi]
        je     0xf9
        jae    0xfd
loc_2_0094:
        jp     0xfb
        jb     loc_2_00B8
        imul   esi, dword ptr [ebx + 0x20], 0x75206e69
        jae    0x106
loc_2_00A1:
        and    byte ptr [edx + 0x79], ah
        and    byte ptr [ecx + 0x6e], ah
loc_2_00A7:
        outsd  dx, dword ptr [esi]
        je     0x112
        jb     0xcd
        popal  
loc_2_00AE:
        jo     0x120
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [ebx + 0x61], 0x6e6f6974
loc_2_00B8:
        add    byte ptr cs:[ebx + 0x6f], dl
        jne    0x12c
        inc    edx
        insb   byte ptr es:[edi], dx
        popal  
        jae    0x138
        jb     0xe7
        push   eax
        jb     0x139
        and    byte ptr [ecx + ebp*2 + 0x72], al
        arpl   word ptr gs:[eax + 0x53], si
        outsd  dx, dword ptr [esi]
        jne    0x144
        and    byte ptr fs:[edx + esi*2 + 0x69], al
        jbe    0x142
        jb     0xdf
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x4c  R-X
loc_3_0000:
        mov    eax, 0x15000e
        mov    esi, 0                        ; -> loc_1_01D4
        VMMCall  VMM.Unhook_Device_Service       ; cd201c010100
        xor    esi, esi
        xchg   dword ptr [0], esi            ; -> obj1:0x00D4
        or     esi, esi
        je     loc_3_004A
        VMMCall  VMM.List_Destroy                ; cd209c000100
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    edi, dword ptr [0]            ; -> obj1:0x00D0
        push   ebx
loc_3_002F:
        xor    esi, esi
        xchg   dword ptr [edi + ebx], esi
        or     esi, esi
        je     loc_3_003E
        VMMCall  VMM.List_Destroy                ; cd209c000100
loc_3_003E:
        VMMCall  VMM.Get_Next_VM_Handle          ; cd203b000100
        cmp    ebx, dword ptr [esp]
        jne    loc_3_002F
        pop    ebx
loc_3_004A:
        clc    
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0x920  R-X
sub_4_0000:
        push   ebp
        mov    ebp, esp
        push   edi
        push   0
        push   dword ptr [ebp + 8]
        call   sub_1_0134
loc_4_000E:
        add    esp, 8
        or     edi, edi
        je     loc_4_0021
        cmp    dword ptr [edi + 0x28], 0
        jne    loc_4_0028
        cmp    dword ptr [edi + 0x30], 0
        jne    loc_4_0028
loc_4_0021:
        mov    eax, 0
        jmp    loc_4_002D
loc_4_0028:
        mov    eax, 0xffffffff
loc_4_002D:
        pop    edi
        pop    ebp
        ret    
sub_4_0030:
        cmp    eax, 0x40
        jle    loc_4_003A
        mov    eax, 0x40
loc_4_003A:
        add    eax, 3
        shr    eax, 2
loc_4_0040:
        mov    ebx, eax
        dec    eax
        bsr    cx, ax
        inc    cl
        mov    eax, 1
        shl    eax, cl
        dec    eax
        mov    ecx, ebx
        push   ecx
        sub    esp, 4
        mov    ebx, esp
        push   0xe
        push   ebx
        push   0xfff
        push   0
        push   eax
        push   0
        push   1
        push   ecx
        VMMCall  VMM._PageAllocate               ; cd2053000100
        add    esp, 0x20
        pop    ebx
        pop    ecx
        or     eax, eax
        jne    loc_4_0082
        mov    eax, ecx
        shr    eax, 1
        jne    loc_4_0040
        xor    ecx, ecx
        stc    
        ret    
loc_4_0082:
        shl    ecx, 0xc
        clc    
        ret    
sub_4_0087:
        push   eax
        push   ecx
        push   esi
        push   edi
        mov    edi, dword ptr [0]            ; -> obj1:0x00D0
        mov    esi, dword ptr [edi + ebx]
        or     esi, esi
        je     loc_4_00C2
        VMMCall  VMM.List_Allocate               ; cd209d000100
        jb     loc_4_00C2
        push   eax
        mov    edi, eax
        xor    eax, eax
        mov    ecx, 8
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        pop    eax
        mov    edi, dword ptr [esp]
        mov    dword ptr [eax + 4], edi
        VMMCall  VMM.List_Attach_Tail            ; cd209f000100
        clc    
loc_4_00BD:
        pop    edi
        pop    esi
        pop    ecx
        pop    eax
        ret    
loc_4_00C2:
        stc    
        jmp    loc_4_00BD
sub_4_00C5:
        push   eax
        push   ecx
        push   esi
        push   edi
        pushfd 
        mov    esi, dword ptr [0]            ; -> obj1:0x00D0
        mov    esi, dword ptr [esi + ebx]
        or     esi, esi
        je     loc_4_00FD
        cli    
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_4_00FD
loc_4_00E0:
        cmp    dword ptr [eax + 4], edi
        je     loc_4_00EF
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        je     loc_4_00FD
        jmp    loc_4_00E0
loc_4_00EF:
        push   eax
        VMMCall  VMM.List_Remove                 ; cd20a1000100
        pop    eax
        VMMCall  VMM.List_Deallocate             ; cd20a2000100
loc_4_00FD:
        popfd  
        pop    edi
        pop    esi
        pop    ecx
        pop    eax
        ret    
sub_4_0103:
        jmp    loc_4_010F
loc_4_0105:
        mov    esi, 0
        jmp    loc_4_01C8
loc_4_010F:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        pushal 
        mov    dword ptr [ebp - 0xc], edx
        movzx  ecx, word ptr [edi]
        mov    eax, ecx
        shl    eax, 2
        add    eax, ecx
        shl    eax, 1
        mov    dword ptr [ebp - 4], eax
        push   0x101
        push   eax
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        or     eax, eax
        je     loc_4_01C2
        mov    dword ptr [ebp - 8], eax
        push   edi
        movzx  ecx, word ptr [edi]
        push   ecx
        mov    edi, dword ptr [ebp - 8]
loc_4_014A:
        push   ecx
        mov    ecx, 0xa
        lea    esi, [0]                      ; -> loc_4_0105
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    ecx
        loop   loc_4_014A
        pop    ecx
        mov    edi, dword ptr [esp]
        mov    esi, dword ptr [ebp - 8]
        add    edi, 2
loc_4_0166:
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [esi + 1], eax
        mov    eax, dword ptr [edi + 2]
        sub    eax, esi
        sub    eax, 0xa
        mov    dword ptr [esi + 6], eax
        movzx  edx, word ptr [edi]
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        jb     loc_4_0199
        add    edi, 6
        add    esi, 0xa
        loop   loc_4_0166
        pop    edi
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [esp + 0x1c], eax
        jmp    loc_4_01C8
loc_4_0199:
        pop    edi
        movzx  eax, word ptr [edi]
        sub    ecx, eax
        neg    ecx
        jecxz  loc_4_01B4
        add    edi, 2
loc_4_01A6:
        movzx  edx, word ptr [edi]
        VMMCall  VMM.Remove_IO_Handler           ; cd2016010100
        add    edi, 6
        loop   loc_4_01A6
loc_4_01B4:
        push   0
        push   dword ptr [ebp - 8]
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
loc_4_01C2:
        xor    eax, eax
        mov    dword ptr [esp + 0x1c], eax
loc_4_01C8:
        popal  
        add    esp, 0xc
        pop    ebp
        ret    
sub_4_01CE:
        push   ecx
        push   edx
        push   edi
        push   eax
        movzx  ecx, word ptr [edi]
        add    edi, 2
loc_4_01D8:
        movzx  edx, word ptr [edi]
        VMMCall  VMM.Remove_IO_Handler           ; cd2016010100
        add    edi, 6
        loop   loc_4_01D8
        pop    eax
        push   0
        push   eax
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
        pop    edi
        pop    edx
        pop    ecx
        ret    
sub_4_01F7:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        pushal 
        mov    dword ptr [ebp - 4], 0
        mov    esi, dword ptr [0]            ; -> obj1:0x00D4
        or     esi, esi
        je     loc_4_041E
        movzx  edx, word ptr [ebp + 0xc]
        push   2
        push   edx
        call   sub_1_0134
loc_4_021F:
        add    esp, 8
        or     edi, edi
        jne    loc_4_041E
        VMMCall  VMM.List_Allocate               ; cd209d000100
        jb     loc_4_041E
        mov    dword ptr [ebp - 4], eax
        VMMCall  VMM.List_Attach_Tail            ; cd209f000100
        mov    dword ptr [eax], 0
        push   1
        push   0x52
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        or     eax, eax
        je     loc_4_041E
        mov    edi, eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax], edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [edi + 0x40], eax
        mov    ax, word ptr [ebp + 0x24]
        mov    word ptr [edi + 4], ax
        mov    ax, word ptr [ebp + 0xc]
        mov    word ptr [edi + 6], ax
        mov    ax, word ptr [ebp + 0x10]
        mov    word ptr [edi + 0xc], ax
        mov    ax, word ptr [ebp + 0x14]
        mov    word ptr [edi + 8], ax
        mov    ax, word ptr [ebp + 0x18]
        mov    byte ptr [edi + 0xa], al
        mov    ax, word ptr [ebp + 0x1c]
        mov    byte ptr [edi + 0xb], al
        call   sub_5_123E
loc_4_029A:
        mov    word ptr [edi + 0xe], ax
        push   edi
        movzx  ecx, byte ptr [edi + 0xa]
        mov    word ptr [0], cx              ; -> obj6:0x01B4
        mov    edi, 0                        ; -> obj6:0x01B4
        VxDCall  VPICD.VPICD_Virtualize_IRQ      ; cd2001000300
        pop    edi
        jae    loc_4_02CA
        push   0                             ; -> obj6:0x0203
        call   sub_5_0A84
loc_4_02C2:
        add    esp, 4
        jmp    loc_4_041E
loc_4_02CA:
        mov    dword ptr [edi + 0x24], eax
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        VxDCall  VPICD.VPICD_Get_Complete_Status ; cd2005000300
        test   ecx, 4
        jne    loc_4_02E7
        or     word ptr [edi + 4], 0x8000
loc_4_02E7:
        mov    esi, 0                        ; -> obj6:0x0138
        movzx  ecx, word ptr [esi]
        movzx  edx, word ptr [ebp + 0xc]
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_02F9:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_02F9
        mov    edx, dword ptr [esp]
        call   sub_4_0103
        pop    edi
        mov    dword ptr [edi + 0x44], eax
        or     eax, eax
        jne    loc_4_0324
        push   0                             ; -> obj6:0x01D4
        call   sub_5_0A84
loc_4_031C:
        add    esp, 4
        jmp    loc_4_041E
loc_4_0324:
        movsx  edx, word ptr [ebp + 0x14]
        cmp    edx, -1
        je     loc_4_0370
        mov    word ptr [edi + 8], dx
        or     word ptr [edi + 4], 0x100
        mov    esi, 0                        ; -> obj6:0x019A
        movzx  ecx, word ptr [esi]
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_0345:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_0345
        mov    edx, dword ptr [esp]
        call   sub_4_0103
        pop    edi
        mov    dword ptr [edi + 0x48], eax
        or     eax, eax
        jne    loc_4_0370
        push   0                             ; -> obj6:0x01EC
        call   sub_5_0A84
loc_4_0368:
        add    esp, 4
        jmp    loc_4_041E
loc_4_0370:
        mov    eax, dword ptr [ebp + 0x20]
        call   sub_4_0030
        mov    dword ptr [edi + 0x10], eax
        mov    dword ptr [edi + 0x14], ebx
        mov    dword ptr [edi + 0x1c], ecx
        mov    dword ptr [edi + 0x18], edx
        jae    loc_4_0398
        push   0                             ; -> obj6:0x0233
        call   sub_5_0A84
loc_4_0390:
        add    esp, 4
        jmp    loc_4_041E
loc_4_0398:
        push   0
        push   0
        push   0x92
        push   ecx
        push   edx
        VMMCall  VMM._BuildDescriptorDWORDs      ; cd207a000100
        add    esp, 0x14
        push   0
        push   eax
        push   edx
        VMMCall  VMM._Allocate_GDT_Selector      ; cd2076000100
        add    esp, 0xc
        mov    word ptr [edi + 0x20], ax
        movzx  eax, word ptr [ebp + 0x1c]
        mov    esi, 0                        ; -> loc_1_022C
        VxDCall  VDMAD.VDMAD_Virtualize_Channel  ; cd2001000400
        mov    dword ptr [edi + 0x38], eax
        jae    loc_4_03E0
        push   0                             ; -> obj6:0x021B
        call   sub_5_0A84
loc_4_03DB:
        add    esp, 4
        jmp    loc_4_041E
loc_4_03E0:
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        push   ebx
loc_4_03E7:
        call   sub_4_0087
        jb     loc_4_041D
        VMMCall  VMM.Get_Next_VM_Handle          ; cd203b000100
        cmp    ebx, dword ptr [esp]
        jne    loc_4_03E7
        pop    ebx
        mov    dword ptr [esp + 0x1c], 0xffffffff
        mov    eax, dword ptr [edi + 0x24]
        VxDCall  VPICD.VPICD_Physically_Mask     ; cd2008000300
        VxDCall  VPICD.VPICD_Clear_Int_Request   ; cd2003000300
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
loc_4_0417:
        popal  
        add    esp, 4
        pop    ebp
        ret    
loc_4_041D:
        pop    ebx
loc_4_041E:
        mov    edi, dword ptr [ebp - 4]
        or     edi, edi
        je     loc_4_042A
        call   sub_4_0434
loc_4_042A:
        mov    dword ptr [esp + 0x1c], 0
        jmp    loc_4_0417
sub_4_0434:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        pushal 
        mov    dword ptr [ebp - 4], edi
        mov    edi, dword ptr [edi]
        or     edi, edi
        je     loc_4_04FC
        xor    esi, esi
        xchg   dword ptr [edi + 0x3c], esi
        or     esi, esi
        je     loc_4_0457
        VMMCall  VMM.List_Destroy                ; cd209c000100
loc_4_0457:
        mov    eax, dword ptr [edi + 0x38]
        or     eax, eax
        je     loc_4_0464
        VxDCall  VDMAD.VDMAD_Unvirtualize_Channel ; cd201a000400
loc_4_0464:
        movzx  eax, word ptr [edi + 0x20]
        or     eax, eax
        je     loc_4_0487
        push   0
        push   eax
        VMMCall  VMM._Free_GDT_Selector          ; cd2077000100
        add    esp, 8
        mov    eax, dword ptr [edi + 0x10]
        push   0
        push   eax
        VMMCall  VMM._PageFree                   ; cd2055000100
        add    esp, 8
loc_4_0487:
        mov    eax, dword ptr [edi + 0x48]
        or     eax, eax
        je     loc_4_04B5
        mov    esi, 0                        ; -> obj6:0x019A
        movzx  ecx, word ptr [esi]
        movzx  edx, word ptr [edi + 8]
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_04A0:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_04A0
        mov    eax, dword ptr [esp]
        mov    eax, dword ptr [eax + 0x48]
        call   sub_4_01CE
        pop    edi
loc_4_04B5:
        mov    eax, dword ptr [edi + 0x44]
        or     eax, eax
        je     loc_4_04E3
        mov    esi, 0                        ; -> obj6:0x0138
        movzx  ecx, word ptr [esi]
        movzx  edx, word ptr [edi + 6]
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_04CE:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_04CE
        mov    eax, dword ptr [esp]
        mov    eax, dword ptr [eax + 0x44]
        call   sub_4_01CE
        pop    edi
loc_4_04E3:
        mov    eax, dword ptr [edi + 0x24]
        or     eax, eax
        je     loc_4_04F0
        VxDCall  VPICD.VPICD_Force_Default_Behavior ; cd2011000300
loc_4_04F0:
        push   0
        push   edi
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
loc_4_04FC:
        mov    esi, dword ptr [0]            ; -> obj1:0x00D4
        mov    eax, dword ptr [ebp - 4]
        push   eax
        VMMCall  VMM.List_Remove                 ; cd20a1000100
        pop    eax
        VMMCall  VMM.List_Deallocate             ; cd20a2000100
        popal  
        add    esp, 4
        pop    ebp
        ret    
sub_4_0519:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        pushal 
        mov    dword ptr [esp + 0x1c], 0x13
        movzx  edx, word ptr [ebp + 0xc]
        call   sub_4_0680
        jb     loc_4_05A7
        test   ax, 0x8000
        je     loc_4_0544
        or     dword ptr [ebp + 0x20], 0x400
        and    ax, 0x7fff
loc_4_0544:
        mov    word ptr [ebp - 4], ax
        test   dword ptr [ebp + 0x20], 0x10
        jne    loc_4_0575
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0xc]
        call   sub_4_071B
        add    esp, 8
        or     eax, eax
        je     loc_4_05A1
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0xc]
        call   sub_4_08B5
        add    esp, 8
        or     eax, eax
        je     loc_4_05A1
loc_4_0575:
        push   dword ptr [ebp + 0x20]
        push   dword ptr [ebp + 0x1c]
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_4_01F7
        add    esp, 0x20
        or     eax, eax
        je     loc_4_05A1
        mov    dword ptr [esp + 0x1c], 0
loc_4_05A1:
        popal  
        add    esp, 4
        pop    ebp
        ret    
loc_4_05A7:
        mov    dword ptr [esp + 0x1c], 0x24
        jmp    loc_4_05A1
sub_4_05B1:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        push   ebx
        pushfd 
        push   dword ptr [ebp + 8]
        call   0
        add    esp, 4
        or     eax, eax
        je     loc_4_05FA
        push   0
        push   dword ptr [ebp + 8]
        call   sub_1_0134
loc_4_05D1:
        add    esp, 8
        or     edi, edi
        je     loc_4_05FA
        mov    ebx, dword ptr [edi + 0x28]
        or     ebx, ebx
        je     loc_4_05E9
        mov    eax, 1
        call   sub_5_1149
loc_4_05E9:
        mov    ebx, dword ptr [edi + 0x30]
        or     ebx, ebx
        je     loc_4_05FA
        mov    eax, 2
        call   sub_5_1149
loc_4_05FA:
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, dword ptr [0]            ; -> obj1:0x00D4
        or     esi, esi
        je     loc_4_0647
        cli    
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_4_0647
loc_4_0610:
        mov    edi, dword ptr [eax]
        cmp    dword ptr [edi + 0x40], ebx
        je     loc_4_0621
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        je     loc_4_0647
        jmp    loc_4_0610
loc_4_0621:
        popfd  
        pushfd 
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        push   ebx
loc_4_062A:
        call   sub_4_00C5
        VMMCall  VMM.Get_Next_VM_Handle          ; cd203b000100
        cmp    ebx, dword ptr [esp]
        jne    loc_4_062A
        pop    ebx
        mov    edi, eax
        call   sub_4_0434
        mov    eax, 0xffffffff
loc_4_0647:
        popfd  
        pop    ebx
        pop    edi
        pop    esi
        pop    ebp
        ret    
loc_4_064D:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_5_0B79
loc_4_0664:
        add    esp, 0x14
        leave  
        clc    
        ret    
loc_4_066A:
        mov    eax, ebx
        mov    ebx, 0                        ; -> loc_4_064D
        VxDCall  VFAT.Service_0000h              ; cd2000004a04
        mov    eax, 0
        stc    
        ret    
        db  0CCh, 0CCh
sub_4_0680:
        push   eax
        push   edx
        add    edx, 6
        mov    al, 1
        out    dx, al
        in     al, dx
        in     al, dx
        xor    al, al
        out    dx, al
        sub    edx, 6
        call   sub_1_02B8
loc_4_0695:
        jb     loc_4_070E
        cmp    al, 0xaa
        jne    loc_4_070E
        mov    al, 0xe1
        call   sub_1_02D4
loc_4_06A2:
        jb     loc_4_070E
        call   sub_1_02B8
loc_4_06A9:
        jb     loc_4_070E
        mov    ah, al
        call   sub_1_02B8
loc_4_06B2:
        jb     loc_4_070E
        movzx  eax, ax
        mov    dword ptr [esp + 4], eax
        mov    al, 0xe1
        call   sub_1_02D4
loc_4_06C2:
        jb     loc_4_070E
        call   sub_1_02B8
loc_4_06C9:
        jb     loc_4_070E
        mov    ah, al
        call   sub_1_02B8
loc_4_06D2:
        jb     loc_4_070E
        movzx  eax, ax
        cmp    dword ptr [esp + 4], eax
        je     loc_4_06E5
        or     dword ptr [esp + 4], 0x8000
loc_4_06E5:
        cmp    ax, 0x100
        jb     loc_4_070E
        cmp    ax, 0x400
        jae    loc_4_070E
        mov    al, 0xe0
        call   sub_1_02D4
loc_4_06F8:
        jb     loc_4_070E
        mov    al, 0xaa
        call   sub_1_02D4
loc_4_0701:
        jb     loc_4_070E
        call   sub_1_02B8
loc_4_0708:
        jb     loc_4_070E
        cmp    al, 0x55
        je     loc_4_0717
loc_4_070E:
        xor    eax, eax
        mov    dword ptr [esp + 4], eax
        stc    
        jmp    loc_4_0718
loc_4_0717:
        clc    
loc_4_0718:
        pop    edx
        pop    eax
        ret    
sub_4_071B:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   ecx
        push   edx
        movzx  edx, word ptr [ebp + 8]
        movzx  eax, word ptr [ebp + 0xc]
        xor    ecx, ecx
        bts    ecx, eax
        pushfd 
        cli    
        in     al, 0xa1
        mov    ah, al
        in     al, 0x21
        push   eax
        or     eax, ecx
        xchg   ah, al
        out    0xa1, al
        xchg   ah, al
        out    0x21, al
        mov    al, 0xa
        out    0xa0, al
        jmp    loc_4_0747
loc_4_0747:
        out    0x20, al
        jmp    loc_4_074B
loc_4_074B:
        add    edx, 0xe
        in     al, dx
        sub    edx, 0xe
        in     al, 0xa0
        mov    ah, al
        in     al, 0x20
        test   ecx, eax
        jne    loc_4_0779
        add    edx, 0xc
        mov    al, 0xf2
        out    dx, al
        sub    edx, 0xc
        push   ecx
        mov    ecx, 0x1f4
loc_4_076B:
        in     al, dx
        loop   loc_4_076B
        pop    ecx
        in     al, 0xa0
        mov    ah, al
        in     al, 0x20
        test   ecx, eax
        jne    loc_4_0785
loc_4_0779:
        push   ecx
        push   eax
        call   sub_5_0AD5
loc_4_0780:
        add    esp, 8
        xor    eax, eax
loc_4_0785:
        add    edx, 0xe
        in     al, dx
        sub    edx, 0xe
        mov    edx, eax
        pop    eax
        xchg   ah, al
        out    0xa1, al
        xchg   ah, al
        out    0x21, al
        mov    eax, edx
        popfd  
        pop    edx
        pop    ecx
        pop    ebx
        pop    ebp
        ret    
sub_4_079F:
        push   ebp
        mov    ebp, esp
        movzx  edx, word ptr [ebp + 8]
        pushfd 
        mov    eax, 0xffffffff
        pushal 
        cli    
        movzx  ebx, word ptr [ebp + 0xc]
        and    ebx, 3
        mov    eax, ebx
        or     al, 4
        out    0xa, al
        xor    al, al
        out    0xc, al
        mov    eax, ebx
        test   dword ptr [ebp + 0x14], 1
        jne    loc_4_07CE
        add    al, 0x48
        jmp    loc_4_07D0
loc_4_07CE:
        add    al, 0x44
loc_4_07D0:
        out    0xb, al
        mov    edx, ebx
        shl    edx, 1
        add    edx, 0
        mov    eax, dword ptr [ebp + 0x10]
        out    dx, al
        shr    eax, 8
        out    dx, al
        movzx  ebx, word ptr [ebp + 0xc]
        movzx  edx, byte ptr [ebx]           ; -> obj6:0x0250
        shr    eax, 8
        out    dx, al
        and    ebx, 3
        mov    edx, ebx
        shl    edx, 1
        add    edx, 1
        mov    al, 0xf
        out    dx, al
        xor    al, al
        out    dx, al
        mov    eax, ebx
        out    0xa, al
        movzx  edx, word ptr [ebp + 8]
        add    edx, 4
        mov    al, 4
        out    dx, al
        inc    dx
        xor    al, al
        out    dx, al
        sub    dx, 5
        mov    al, 0xd3
        call   sub_1_02D4
loc_4_081E:
        mov    al, 0x40
        call   sub_1_02D4
loc_4_0825:
        mov    al, 0x2d
        call   sub_1_02D4
loc_4_082C:
        movzx  edx, word ptr [ebp + 8]
        mov    al, 0x14
        call   sub_1_02D4
loc_4_0837:
        mov    al, 0xf
        call   sub_1_02D4
loc_4_083E:
        xor    al, al
        call   sub_1_02D4
loc_4_0845:
        mov    edx, ebx
        shl    edx, 1
        add    edx, 1
        mov    ecx, 0xffff
loc_4_0851:
        xor    al, al
        out    0xc, al
        jmp    loc_4_0857
loc_4_0857:
        in     al, dx
        mov    ah, al
        jmp    loc_4_085C
loc_4_085C:
        in     al, dx
        cmp    ax, -1
        je     loc_4_0890
        loop   loc_4_0851
        push   eax
        push   ebx
        call   sub_5_0B02
loc_4_086C:
        add    esp, 8
        mov    dword ptr [esp + 0x1c], 0
        movzx  edx, word ptr [ebp + 8]
        add    edx, 6
        mov    al, 1
        out    dx, al
        in     al, dx
        in     al, dx
        xor    al, al
        out    dx, al
        sub    edx, 6
        call   sub_1_02B8
loc_4_088E:
        jmp    loc_4_08B1
loc_4_0890:
        movzx  edx, word ptr [ebp + 8]
        mov    al, 0xd0
        call   sub_1_02D4
loc_4_089B:
        mov    eax, ebx
        and    eax, 3
        or     eax, 4
        out    0xa, al
        mov    ecx, 0x64
loc_4_08AA:
        in     al, dx
        loop   loc_4_08AA
        add    edx, 0xe
        in     al, dx
loc_4_08B1:
        popal  
        popfd  
        pop    ebp
        ret    
sub_4_08B5:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        pushal 
        lea    ebx, [ebp - 4]
        push   0xe
        push   ebx
        push   0xfff
        push   0
        push   0
        push   0
        push   1
        push   1
        VMMCall  VMM._PageAllocate               ; cd2053000100
        add    esp, 0x20
        mov    dword ptr [ebp - 8], eax
        or     eax, eax
        je     loc_4_0903
        mov    edi, edx
        mov    ecx, 0x400
        mov    eax, 0x80808080
        cld    
        rep stosd dword ptr es:[edi], eax
        push   0
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_4_079F
        add    esp, 0x10
loc_4_0903:
        mov    dword ptr [esp + 0x1c], eax
        mov    eax, dword ptr [ebp - 8]
        or     eax, eax
        je     loc_4_091A
        push   0
        push   eax
        VMMCall  VMM._PageFree                   ; cd2055000100
        add    esp, 8
loc_4_091A:
        popal  
        add    esp, 8
        pop    ebp
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 5  vsize=0x1a2a  R-X
        db  00Fh, 0B7h, 005h
        dd  obj2:0x0010              ; fixup
        db  003h, 005h
        dd  obj2:0x0004              ; fixup
        db  0C3h
sub_5_000E:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 8]
        add    edx, dword ptr [ebp + 0xc]
        xor    eax, eax
        in     al, dx
        pop    ebp
        ret    
sub_5_001C:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0xc]
        call   sub_1_02D4
loc_5_002A:
        jb     loc_5_0030
        xor    eax, eax
        jmp    loc_5_0033
loc_5_0030:
        xor    eax, eax
        inc    eax
loc_5_0033:
        pop    ebp
        ret    
sub_5_0035:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 8]
        call   sub_1_02F8
loc_5_0040:
        pop    ebp
        ret    
sub_5_0042:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 8]
        add    edx, 4
        mov    eax, dword ptr [ebp + 0xc]
        out    dx, al
        jmp    loc_5_0051
loc_5_0051:
        inc    edx
        in     al, dx
        pop    ebp
        ret    
sub_5_0055:
        push   ebp
        mov    ebp, esp
        mov    edx, dword ptr [ebp + 8]
        add    edx, 4
        mov    eax, dword ptr [ebp + 0xc]
        out    dx, al
        jmp    loc_5_0064
loc_5_0064:
        inc    edx
        mov    eax, dword ptr [ebp + 0x10]
        out    dx, al
        pop    ebp
        ret    
sub_5_006B:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   ecx
        mov    eax, dword ptr [ebp + 8]
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        VxDCall  VDMAD.VDMAD_Get_Phys_Count      ; cd201c000400
        movzx  eax, cx
        pop    ecx
        pop    ebx
        pop    ebp
        ret    
sub_5_0086:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   ecx
        mov    ebx, dword ptr [ebp + 8]
        add    ebx, 4
        cmp    ebx, 7
        jbe    loc_5_0099
        add    ebx, 4
loc_5_0099:
        VMMCall  VMM.Get_System_Time             ; cd203f000100
        mov    ecx, eax
loc_5_00A1:
        VxDCall  VDMAD.VDMAD_Get_Phys_Status     ; cd201d000400
        bt     eax, ebx
        jae    loc_5_00B9
        VMMCall  VMM.Get_System_Time             ; cd203f000100
        sub    eax, ecx
        cmp    eax, 0x64
        jb     loc_5_00A1
loc_5_00B9:
        pop    ecx
        pop    ebx
        pop    ebp
        ret    
sub_5_00BD:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   ecx
        push   edx
        VMMCall  VMM.Get_System_Time             ; cd203f000100
        mov    edx, eax
loc_5_00CB:
        mov    eax, dword ptr [ebp + 8]
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        VxDCall  VDMAD.VDMAD_Get_Phys_Count      ; cd201c000400
        cmp    ecx, 0xffff
        je     loc_5_00EF
        VMMCall  VMM.Get_System_Time             ; cd203f000100
        sub    eax, edx
        cmp    eax, dword ptr [ebp + 0xc]
        jb     loc_5_00CB
loc_5_00EF:
        pop    edx
        pop    ecx
        pop    ebx
        pop    ebp
        ret    
sub_5_00F4:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        VxDCall  VDMAD.VDMAD_Phys_Mask_Channel   ; cd2018000400
        pop    ebp
        ret    
sub_5_0102:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        VxDCall  VDMAD.VDMAD_Phys_Unmask_Channel ; cd2019000400
        pop    ebx
        pop    ebp
        ret    
sub_5_0118:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   esi
        push   ecx
        mov    eax, dword ptr [ebp + 8]
        xor    bx, bx
        mov    bh, 1
        xor    esi, esi
        mov    ecx, dword ptr [ebp + 0x10]
        mov    edx, dword ptr [ebp + 0xc]
        VxDCall  VDMAD.VDMAD_Set_Region_Info     ; cd2003000400
        pop    ecx
        pop    esi
        pop    ebx
        pop    ebp
        ret    
sub_5_0139:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edx
        mov    eax, dword ptr [ebp + 8]
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        xor    dx, dx
        or     dl, 8
        or     dl, 0x10
        or     dl, 0x40
        VxDCall  VDMAD.VDMAD_Set_Phys_State      ; cd2006000400
        pop    edx
        pop    ebx
        pop    ebp
        ret    
sub_5_015D:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   edx
        mov    eax, dword ptr [ebp + 8]
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        xor    dx, dx
        or     dl, 8
        or     dl, 0x40
        VxDCall  VDMAD.VDMAD_Set_Phys_State      ; cd2006000400
        pop    edx
        pop    ebx
        pop    ebp
        ret    
sub_5_017E:
        push   ebp
        mov    ebp, esp
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        push   0
        push   eax
        call   sub_1_0134
loc_5_018E:
        add    esp, 8
        mov    word ptr [edi + 0x50], 0
        pop    ebx
        pop    edi
        pop    ebp
        ret    
sub_5_019B:
        push   ebp
        mov    ebp, esp
        push   edi
        push   edx
        push   ebx
        mov    edx, 0xffff
        mov    eax, dword ptr [ebp + 8]
        push   0
        push   eax
        call   sub_1_0134
loc_5_01B1:
        add    esp, 8
        mov    eax, dword ptr [edi + 0x28]
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        cmp    eax, ebx
        je     loc_5_01C3
        jmp    loc_5_01CE
loc_5_01C3:
        mov    word ptr [edi + 0x50], 1
        mov    edx, 0
loc_5_01CE:
        mov    eax, edx
        pop    ebx
        pop    edx
        pop    edi
        pop    ebp
        ret    
sub_5_01D5:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   0
        push   eax
        call   sub_1_0134
loc_5_01E5:
        add    esp, 8
        or     edi, edi
        je     loc_5_01F2
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [edi + 0x4c], eax
loc_5_01F2:
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_5_01F6:
        push   ebp
        mov    ebp, esp
        push   esi
        push   edi
        mov    eax, dword ptr [ebp + 8]
        push   0
        push   eax
        call   sub_1_0134
loc_5_0206:
        add    esp, 8
        xor    eax, eax
        or     edi, edi
        je     loc_5_0212
        mov    eax, dword ptr [edi + 0x4c]
loc_5_0212:
        pop    edi
        pop    esi
        pop    ebp
        ret    
sub_5_0216:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0xc]
        push   0
        push   eax
        call   sub_1_0134
loc_5_0229:
        add    esp, 8
        mov    ax, word ptr [edi + 0xc]
        mov    word ptr [esi + 0x18], ax
        pop    esi
        pop    edi
        pop    ebp
        ret    
sub_5_0238:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0xc]
        push   0
        push   eax
        call   sub_1_0134
loc_5_024B:
        add    esp, 8
        mov    ax, word ptr [edi + 0xc]
        mov    word ptr [esi + 0x34], ax
        mov    ax, word ptr [edi + 6]
        mov    word ptr [esi + 0x36], ax
        mov    eax, dword ptr [edi + 0x14]
        mov    dword ptr [esi + 0xc], eax
        mov    eax, dword ptr [edi + 0x18]
        mov    dword ptr [esi + 0x10], eax
        mov    eax, dword ptr [edi + 0x1c]
        mov    dword ptr [esi + 0x14], eax
        mov    eax, dword ptr [edi + 0x38]
        mov    dword ptr [esi + 0x18], eax
        mov    al, byte ptr [edi + 0xb]
        mov    byte ptr [esi + 0x38], al
        pop    esi
        pop    edi
        pop    ebp
        ret    
loc_5_0280:
        mov    edx, dword ptr [esp + 0xc]
        push   esi
        push   edi
        mov    ecx, 0x10
        mov    edi, 0                        ; -> loc_1_0040
        mov    eax, dword ptr [esp + 0x10]
        mov    esi, eax
        mov    dword ptr [edx], 0
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        je     loc_5_02B5
        mov    edi, 0                        ; -> loc_1_0060
        mov    ecx, 0x10
        mov    esi, eax
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        mov    eax, 0x80004002
        jne    loc_5_02C3
loc_5_02B5:
        mov    eax, dword ptr [esp + 0xc]
        push   eax
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 4]
        xor    eax, eax
loc_5_02C3:
        pop    edi
        pop    esi
        ret    0xc
loc_5_02C8:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 0xc]
        inc    eax
        mov    dword ptr [ecx + 0xc], eax
        ret    4
loc_5_02D6:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 0xc]
        dec    eax
        mov    dword ptr [ecx + 0xc], eax
        jne    loc_5_02F0
        push   0
        push   ecx
        call   sub_5_1928
        add    esp, 8
        xor    eax, eax
loc_5_02F0:
        ret    4
loc_5_02F3:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0x10]
        push   edi
        lea    edi, [esi + 0x104]
        mov    dword ptr [esi], 0xb
        call   0
        mov    ebx, eax
        push   0xff
        push   0                             ; -> obj6:0x0024
        push   edi
        call   sub_5_1930
        add    esp, 0xc
        lea    eax, [esi + 4]
        push   0xff
        push   ebx
        push   eax
        call   sub_5_1930
        mov    eax, dword ptr [esp + 0x1c]
        add    esp, 0xc
        xor    ecx, ecx
        mov    dword ptr [esi + 0x210], ecx
        mov    edx, dword ptr [eax + 4]
        mov    eax, 0xb3
        mov    dword ptr [esi + 0x204], edx
        mov    dword ptr [esi + 0x20c], ecx
        mov    word ptr [esi + 0x208], 0x44e
        cmp    byte ptr [edi], cl
        je     loc_5_036A
loc_5_035F:
        movsx  ecx, byte ptr [edi]
        add    eax, ecx
        inc    edi
        cmp    byte ptr [edi], 0
        jne    loc_5_035F
loc_5_036A:
        mov    ecx, 0xf7
        sub    edx, edx
        div    ecx
        mov    word ptr [esi + 0x20a], dx
        xor    eax, eax
        pop    edi
        pop    esi
        pop    ebx
        ret    8
loc_5_0382:
        push   esi
        push   edi
        mov    edi, 0x8878000a
        mov    esi, dword ptr [esp + 0xc]
        cmp    dword ptr [esi + 0x14], 0
        jne    loc_5_03A7
        push   dword ptr [esi + 4]
        call   sub_5_019B
        add    esp, 4
        test   eax, eax
        jne    loc_5_03A7
        xor    edi, edi
        inc    dword ptr [esi + 0x14]
loc_5_03A7:
        mov    eax, edi
        pop    edi
        pop    esi
        ret    4
loc_5_03AE:
        mov    eax, dword ptr [esp + 4]
        cmp    word ptr [eax + 0x18], 0x300
        mov    eax, dword ptr [esp + 8]
        jb     loc_5_03C6
        mov    dword ptr [eax], 7
        jmp    loc_5_03CC
loc_5_03C6:
        mov    dword ptr [eax], 5
loc_5_03CC:
        mov    dword ptr [eax + 0xc], 1
        xor    ecx, ecx
        mov    dword ptr [eax + 4], ecx
        mov    dword ptr [eax + 8], ecx
        mov    dword ptr [eax + 0x10], ecx
        mov    dword ptr [eax + 0x14], ecx
        mov    dword ptr [eax + 0x18], ecx
        mov    dword ptr [eax + 0x1c], ecx
        mov    dword ptr [eax + 0x20], ecx
        mov    dword ptr [eax + 0x24], ecx
        mov    dword ptr [eax + 0x28], ecx
        mov    dword ptr [eax + 0x2c], ecx
        mov    dword ptr [eax + 0x30], ecx
        mov    dword ptr [eax + 0x34], ecx
        mov    dword ptr [eax + 0x38], ecx
        mov    dword ptr [eax + 0x3c], ecx
        mov    dword ptr [eax + 0x40], ecx
        mov    dword ptr [eax + 0x44], ecx
        mov    dword ptr [eax + 0x48], ecx
        xor    eax, eax
        ret    8
loc_5_040D:
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        push   ebp
        mov    edi, 0x8007000e
        mov    esi, dword ptr [esp + 0x2c]
        test   byte ptr [esp + 0x1c], 0xf0
        mov    dword ptr [esi], ebx
        je     loc_5_042F
        mov    eax, 0x8878001e
        jmp    loc_5_04DF
loc_5_042F:
        mov    ebp, dword ptr [esp + 0x14]
        cmp    dword ptr [ebp + 0x10], 0
        jne    loc_5_04BE
        push   0x201
        push   0x39
        call   sub_5_1938
        add    esp, 8
        mov    ebx, eax
        test   ebx, ebx
        je     loc_5_04B7
        mov    dword ptr [ebx], 0            ; -> obj6:0x0040
        mov    dword ptr [ebx + 4], ebp
        push   dword ptr [ebp + 4]
        push   ebx
        call   sub_5_0238
        add    esp, 8
        mov    eax, dword ptr [ebx + 0x14]
        cmp    eax, 0x1000
        jbe    loc_5_04C3
        cmp    eax, 0xffff
        jae    loc_5_04C3
        cmp    dword ptr [ebx + 0xc], 0
        je     loc_5_04C3
        cmp    dword ptr [ebx + 0x10], 0
        je     loc_5_04C3
        cmp    dword ptr [ebx + 0x18], 0
        je     loc_5_04C3
        push   dword ptr [esp + 0x24]
        push   dword ptr [esp + 0x2c]
        push   ebx
        xor    edi, edi
        call   sub_5_0642
        mov    ecx, dword ptr [esp + 0x24]
        add    esp, 0xc
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [ebx + 0x24], eax
        movzx  edx, word ptr [ecx + 0xe]
        mov    dword ptr [ebx + 0x28], edx
        movzx  ecx, word ptr [ecx + 2]
        mov    dword ptr [ebx + 0x2c], ecx
        jmp    loc_5_04C3
loc_5_04B7:
        mov    edi, 0x8007000e
        jmp    loc_5_04C3
loc_5_04BE:
        mov    edi, 0x8878000a
loc_5_04C3:
        test   edi, edi
        je     loc_5_04D8
        test   ebx, ebx
        je     loc_5_04DD
        push   0
        push   ebx
        call   sub_5_1928
        add    esp, 8
        jmp    loc_5_04DD
loc_5_04D8:
        mov    dword ptr [esi], ebx
        inc    dword ptr [ebp + 0x10]
loc_5_04DD:
        mov    eax, edi
loc_5_04DF:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    0x1c
loc_5_04E6:
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [eax], 0
        mov    eax, 0x80004001
        ret    0xc
loc_5_04F8:
        mov    eax, 0x80004005
        mov    edx, dword ptr [esp + 4]
        mov    ecx, dword ptr [edx + 0x14]
        dec    ecx
        mov    dword ptr [edx + 0x14], ecx
        jne    loc_5_051D
        cmp    dword ptr [edx + 0x10], 0
        jne    loc_5_051D
        push   dword ptr [edx + 4]
        call   sub_5_017E
        add    esp, 4
        xor    eax, eax
loc_5_051D:
        ret    4
sub_5_0520:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        push   0x201
        mov    dword ptr [ebp - 4], ebx
        push   0x1a
        call   sub_5_1938
        add    esp, 8
        mov    esi, eax
        test   esi, esi
        je     loc_5_05AF
        mov    edi, dword ptr [ebp + 8]
        push   1
        push   0                             ; -> obj6:0x0030
        lea    eax, [ebp - 4]
        push   eax
        lea    ecx, [ebp - 0x14]
        push   ecx
        mov    dword ptr [esi + 4], edi
        mov    dword ptr [esi], 0            ; -> obj6:0x0000
        call   sub_5_1954
        test   eax, eax
        jne    loc_5_05AF
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [ebp - 0x10], ebx
        mov    dword ptr [esi + 8], eax
        mov    dword ptr [ebp - 0xc], esi
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 0x10]
        push   dword ptr [ebp - 0xc]
        VxDCall  Dev_357E.Service_0001h          ; cd2001007e35
        add    esp, 8
        mov    dword ptr [ebp - 8], eax
        cmp    dword ptr [ebp - 8], ebx
        jne    loc_5_05AF
        push   esi
        push   edi
        call   sub_5_01D5
        add    esp, 8
        push   edi
        push   esi
        mov    ebx, 1
        call   sub_5_0216
        add    esp, 8
        mov    eax, dword ptr [esi]
        push   esi
        call   dword ptr [eax + 4]
loc_5_05AF:
        test   ebx, ebx
        jne    loc_5_05D8
        cmp    dword ptr [ebp - 4], 0
        je     loc_5_05C9
        mov    eax, dword ptr [ebp - 4]
        push   dword ptr [eax + 0xb]
        mov    ax, word ptr [eax + 9]
        push   eax
        call   sub_5_1940
loc_5_05C9:
        test   esi, esi
        je     loc_5_05D8
        push   0
        push   esi
        call   sub_5_1928
        add    esp, 8
loc_5_05D8:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_05DF:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_5_01F6
        add    esp, 4
        mov    esi, eax
        test   esi, esi
        je     loc_5_063C
        push   0
        push   edi
        call   sub_5_01D5
        add    esp, 8
        mov    dword ptr [ebp - 4], esi
        mov    dword ptr [ebp - 8], 0
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp - 4]
        VxDCall  Dev_357E.Service_0002h          ; cd2002007e35
        add    esp, 8
        mov    dword ptr [ebp - 0xc], eax
        mov    eax, dword ptr [esi + 8]
        push   dword ptr [eax + 0xb]
        mov    ax, word ptr [eax + 9]
        push   eax
        call   sub_5_1940
        push   esi
        mov    edi, dword ptr [esi]
        call   dword ptr [edi + 8]
loc_5_063C:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_0642:
        mov    edx, dword ptr [esp + 0xc]
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 0x14]
        cmp    dword ptr [edx], eax
        jbe    loc_5_0654
        mov    dword ptr [edx], eax
loc_5_0654:
        mov    eax, dword ptr [esp + 0xc]
        and    dword ptr [edx], 0xfffc
        mov    ecx, dword ptr [esi + 0x10]
        mov    dword ptr [eax], ecx
        mov    edx, dword ptr [edx]
        mov    dword ptr [esi + 0x1c], edx
        inc    dword ptr [esi + 0x20]
        mov    dword ptr [esi + 8], 0
        pop    esi
        ret    
loc_5_0674:
        mov    eax, dword ptr [esp + 4]
        push   esi
        mov    esi, dword ptr [eax + 0x20]
        dec    esi
        mov    dword ptr [eax + 0x20], esi
        jne    loc_5_0699
        cmp    dword ptr [eax + 8], 0
        jne    loc_5_0699
        mov    ecx, dword ptr [eax + 4]
        push   0
        push   eax
        dec    dword ptr [ecx + 0x10]
        call   sub_5_1928
        add    esp, 8
loc_5_0699:
        mov    eax, esi
        pop    esi
        ret    4
loc_5_069F:
        mov    edx, dword ptr [esp + 0xc]
        push   esi
        push   edi
        mov    ecx, 0x10
        mov    edi, 0                        ; -> loc_1_0050
        mov    eax, dword ptr [esp + 0x10]
        mov    esi, eax
        mov    dword ptr [edx], 0
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        je     loc_5_06D4
        mov    edi, 0                        ; -> loc_1_0060
        mov    ecx, 0x10
        mov    esi, eax
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        mov    eax, 0x80004002
        jne    loc_5_06E2
loc_5_06D4:
        mov    eax, dword ptr [esp + 0xc]
        push   eax
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 4]
        xor    eax, eax
loc_5_06E2:
        pop    edi
        pop    esi
        ret    0xc
loc_5_06E7:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 0x20]
        inc    eax
        mov    dword ptr [ecx + 0x20], eax
        ret    4
loc_5_06F5:
        xor    eax, eax
        ret    0x20
loc_5_06FA:
        xor    eax, eax
        ret    0x14
loc_5_06FF:
        mov    ecx, dword ptr [esp + 8]
        mov    edx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 4]
        mov    dword ptr [edx + 0x24], eax
        movzx  eax, word ptr [ecx + 0xe]
        mov    dword ptr [edx + 0x28], eax
        mov    eax, 0x80004001
        movzx  ecx, word ptr [ecx + 2]
        mov    dword ptr [edx + 0x2c], ecx
        ret    8
loc_5_0723:
        mov    eax, 0x80004001
        ret    8
loc_5_072B:
        mov    eax, 0x80004001
        ret    8
loc_5_0733:
        mov    eax, 0x80004001
        ret    8
loc_5_073B:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi + 8], 1
        jne    loc_5_076D
        push   dword ptr [esi + 0x18]
        call   sub_5_006B
        add    esp, 4
        inc    eax
        cmp    byte ptr [esi + 0x38], 3
        jbe    loc_5_075A
        add    eax, eax
loc_5_075A:
        mov    ecx, dword ptr [esi + 0x1c]
        mov    edx, dword ptr [esp + 0x10]
        sub    ecx, eax
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [eax], ecx
        mov    dword ptr [edx], ecx
        jmp    loc_5_0781
loc_5_076D:
        mov    eax, dword ptr [esp + 0xc]
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [eax], 0
        mov    dword ptr [ecx], 0
loc_5_0781:
        xor    eax, eax
        pop    esi
        ret    0xc
loc_5_0787:
        push   ebp
        mov    eax, 0x80004005
        mov    ebp, esp
        sub    esp, 4
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        cmp    dword ptr [esi + 8], 0
        jne    loc_5_099F
        mov    eax, dword ptr [esi + 0x24]
        mov    ebx, dword ptr [esi + 0x2c]
        push   0xe
        mov    dword ptr [ebp - 4], eax
        movzx  edi, word ptr [esi + 0x36]
        push   edi
        call   sub_5_000E
        add    esp, 8
        push   dword ptr [esi + 0x18]
        call   sub_5_00F4
        imul   ebx, dword ptr [ebp - 4]
        mov    dword ptr [esi + 0x30], 0
        add    esp, 4
        cmp    ebx, 0x5622
        ja     loc_5_07EB
        mov    eax, ebx
        sub    edx, edx
        shr    eax, 1
        add    eax, 0xf4240
        div    ebx
        neg    al
        mov    bl, al
        jmp    loc_5_0804
loc_5_07EB:
        mov    eax, ebx
        sub    edx, edx
        mov    dword ptr [esi + 0x30], 1
        shr    eax, 1
        add    eax, 0xf424000
        div    ebx
        neg    ax
        mov    bl, ah
loc_5_0804:
        push   edi
        call   sub_5_0035
        add    esp, 4
        push   0x40
        push   edi
        call   sub_5_001C
        add    esp, 8
        test   eax, eax
        je     loc_5_0830
        push   edi
        call   sub_5_0035
        add    esp, 4
        push   0x40
        push   edi
        call   sub_5_001C
        add    esp, 8
loc_5_0830:
        movzx  eax, bl
        push   eax
        push   edi
        call   sub_5_001C
        add    esp, 8
        push   0xd1
        push   edi
        call   sub_5_001C
        add    esp, 8
        cmp    word ptr [esi + 0x34], 0x300
        jb     loc_5_0888
        push   0xe
        push   edi
        call   sub_5_0042
        add    esp, 8
        cmp    dword ptr [esi + 0x2c], 2
        jne    loc_5_0868
        or     al, 2
        jmp    loc_5_0877
loc_5_0868:
        and    al, 0xfd
        cmp    dword ptr [ebp - 4], 0x4650
        jae    loc_5_0877
        and    al, 0xdf
        jmp    loc_5_0879
loc_5_0877:
        or     al, 0x20
loc_5_0879:
        movzx  eax, al
        push   eax
        push   0xe
        push   edi
        call   sub_5_0055
        add    esp, 0xc
loc_5_0888:
        cmp    dword ptr [esi + 0x2c], 2
        jne    loc_5_08FA
        mov    eax, dword ptr [esi + 0x10]
        push   1
        mov    bl, byte ptr [eax]
        mov    byte ptr [eax], 0x80
        push   dword ptr [esi + 0xc]
        push   dword ptr [esi + 0x18]
        call   sub_5_0118
        add    esp, 0xc
        push   dword ptr [esi + 0x18]
        call   sub_5_015D
        add    esp, 4
        push   dword ptr [esi + 0x18]
        call   sub_5_0102
        add    esp, 4
        push   0x14
        push   edi
        call   sub_5_001C
        add    esp, 8
        push   0
        push   edi
        call   sub_5_001C
        add    esp, 8
        push   0
        push   edi
        call   sub_5_001C
        add    esp, 8
        push   0x64
        push   dword ptr [esi + 0x18]
        call   sub_5_00BD
        add    esp, 8
        push   dword ptr [esi + 0x18]
        call   sub_5_00F4
        add    esp, 4
        mov    eax, dword ptr [esi + 0x10]
        mov    byte ptr [eax], bl
loc_5_08FA:
        push   dword ptr [esi + 0x1c]
        push   dword ptr [esi + 0xc]
        push   dword ptr [esi + 0x18]
        call   sub_5_0118
        add    esp, 0xc
        push   dword ptr [esi + 0x18]
        call   sub_5_0139
        add    esp, 4
        push   dword ptr [esi + 0x18]
        call   sub_5_0102
        mov    bx, word ptr [esi + 0x1c]
        add    esp, 4
        cmp    word ptr [esi + 0x34], 0x200
        jb     loc_5_096A
        push   0x48
        push   edi
        call   sub_5_001C
        mov    ax, bx
        add    esp, 8
        dec    bx
        dec    al
        movzx  ecx, al
        push   ecx
        push   edi
        call   sub_5_001C
        add    esp, 8
        movzx  eax, bh
        push   eax
        push   edi
        call   sub_5_001C
        add    esp, 8
        cmp    dword ptr [esi + 0x30], 0
        je     loc_5_0966
        push   0x90
        jmp    loc_5_098D
loc_5_0966:
        push   0x1c
        jmp    loc_5_098D
loc_5_096A:
        push   0x14
        push   edi
        call   sub_5_001C
        mov    ax, bx
        add    esp, 8
        dec    bx
        dec    al
        movzx  ecx, al
        push   ecx
        push   edi
        call   sub_5_001C
        add    esp, 8
        movzx  eax, bh
        push   eax
loc_5_098D:
        push   edi
        call   sub_5_001C
        add    esp, 8
        xor    eax, eax
        mov    dword ptr [esi + 8], 1
loc_5_099F:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    0x10
loc_5_09A8:
        push   ebx
        mov    eax, 0x80004005
        push   esi
        push   edi
        mov    esi, dword ptr [esp + 0x10]
        cmp    dword ptr [esi + 8], 0
        je     loc_5_0A7E
        cmp    dword ptr [esi + 0x30], 0
        movzx  edi, word ptr [esi + 0x36]
        je     loc_5_09D3
        push   edi
        call   sub_5_0035
        add    esp, 4
        jmp    loc_5_09E1
loc_5_09D3:
        push   0xd0
        push   edi
        call   sub_5_001C
        add    esp, 8
loc_5_09E1:
        mov    ebx, 0x46
loc_5_09E6:
        push   0xc
        push   edi
        call   sub_5_000E
        add    esp, 8
        dec    ebx
        jne    loc_5_09E6
        movzx  eax, byte ptr [esi + 0x38]
        push   eax
        call   sub_5_0086
        add    esp, 4
        push   dword ptr [esi + 0x18]
        call   sub_5_00F4
        add    esp, 4
        push   0xe
        push   edi
        call   sub_5_000E
        add    esp, 8
        cmp    word ptr [esi + 0x34], 0x300
        jb     loc_5_0A67
        push   0xc
        push   edi
        call   sub_5_0042
        add    esp, 8
        or     al, 0x20
        movzx  ecx, al
        push   ecx
        push   0xc
        push   edi
        call   sub_5_0055
        add    esp, 0xc
        push   0xe
        push   edi
        call   sub_5_0042
        add    esp, 8
        or     al, 0x20
        and    eax, 0xfd
        push   eax
        push   0xe
        push   edi
        call   sub_5_0055
        add    esp, 0xc
        push   0xa0
        push   edi
        call   sub_5_001C
        add    esp, 8
loc_5_0A67:
        push   0xd3
        push   edi
        call   sub_5_001C
        add    esp, 8
        xor    eax, eax
        mov    dword ptr [esi + 8], 0
loc_5_0A7E:
        pop    edi
        pop    esi
        pop    ebx
        ret    4
sub_5_0A84:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        VMMCall  VMM.Open_Boot_Log               ; cd2088010100
        test   eax, eax
        jne    loc_5_0AD1
        mov    dword ptr [ebp - 0xc], 0
        mov    dword ptr [ebp - 0x10], 0     ; -> obj6:0x0070
        push   ecx
        mov    edx, dword ptr [ebp - 0x10]
        mov    ecx, dword ptr [ebp - 0xc]
        VMMCall  VMM.Write_Boot_Log              ; cd2089010100
        pop    ecx
        mov    dword ptr [ebp - 4], 0
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [ebp - 8], eax
        push   ecx
        mov    edx, dword ptr [ebp - 8]
        mov    ecx, dword ptr [ebp - 4]
        VMMCall  VMM.Write_Boot_Log              ; cd2089010100
        pop    ecx
        VMMCall  VMM.Close_Boot_Log              ; cd208a010100
loc_5_0AD1:
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_0AD5:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x80
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        push   0                             ; -> obj6:0x007C
        lea    edx, [ebp - 0x80]
        push   edx
        call   sub_5_1978
        add    esp, 0x10
        lea    ecx, [ebp - 0x80]
        push   ecx
        call   sub_5_0A84
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_0B02:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x80
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        push   0                             ; -> obj6:0x0098
        lea    edx, [ebp - 0x80]
        push   edx
        call   sub_5_1978
        add    esp, 0x10
        lea    ecx, [ebp - 0x80]
        push   ecx
        call   sub_5_0A84
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_0B2F:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        push   1
        lea    eax, [ebp - 8]
        push   eax
        lea    ecx, [ebp - 4]
        mov    dword ptr [ebp - 8], 4
        push   ecx
        push   3
        push   0                             ; -> obj6:0x00BC
        push   0                             ; -> obj6:0x00B4
        push   dword ptr [ebp + 8]
        call   sub_5_1980
        add    esp, 0x1c
        test   eax, eax
        je     loc_5_0B68
        mov    dword ptr [ebp - 4], 1
loc_5_0B68:
        cmp    dword ptr [ebp - 4], 0
        jne    loc_5_0B75
        mov    eax, dword ptr [ebp + 0xc]
        or     byte ptr [eax + 1], 2
loc_5_0B75:
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_0B79:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0x224
        test   eax, eax
        push   ebx
        push   esi
        push   edi
        je     loc_5_0BB7
        cmp    eax, 1
        je     loc_5_0BBE
        cmp    eax, 2
        je     loc_5_0DD1
        cmp    eax, 3
        je     loc_5_0E03
        cmp    eax, 4
        je     loc_5_0DD1
        mov    eax, 1
        jmp    loc_5_0E2F
loc_5_0BB7:
        xor    eax, eax
        jmp    loc_5_0E2F
loc_5_0BBE:
        push   0
        lea    eax, [ebp - 0x224]
        push   0x100
        mov    esi, dword ptr [ebp + 0x10]
        push   eax
        push   0
        push   esi
        call   sub_5_1998
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_0E2F
        push   0
        lea    eax, [ebp - 0x124]
        push   esi
        push   eax
        call   sub_5_1990
        add    esp, 0xc
        test   eax, eax
        jne    loc_5_0E2F
        mov    dword ptr [ebp - 8], 0
        movzx  eax, word ptr [ebp - 0xc8]
        mov    bx, 0xffff
        cmp    eax, 1
        je     loc_5_0C22
        cmp    eax, 2
        je     loc_5_0C3F
        mov    eax, 0x13
        jmp    loc_5_0E2F
loc_5_0C22:
        cmp    word ptr [ebp - 0x9e], 0x10
        je     loc_5_0C36
        mov    eax, 0x13
        jmp    loc_5_0E2F
loc_5_0C36:
        mov    di, word ptr [ebp - 0xc6]
        jmp    loc_5_0C8E
loc_5_0C3F:
        cmp    word ptr [ebp - 0x9e], 0x10
        je     loc_5_0C53
        mov    eax, 0x13
        jmp    loc_5_0E2F
loc_5_0C53:
        movzx  eax, word ptr [ebp - 0x9c]
        mov    di, word ptr [ebp - 0xc6]
        cmp    eax, 2
        je     loc_5_0C7A
        cmp    eax, 4
        je     loc_5_0C7A
        cmp    eax, 0x10
        je     loc_5_0C83
        mov    eax, 0x13
        jmp    loc_5_0E2F
loc_5_0C7A:
        mov    bx, word ptr [ebp - 0xc4]
        jmp    loc_5_0C8E
loc_5_0C83:
        mov    bx, word ptr [ebp - 0xc4]
        add    bx, 8
loc_5_0C8E:
        cmp    word ptr [ebp - 0x76], 1
        jb     loc_5_0DCA
        cmp    word ptr [ebp - 0x66], 1
        jb     loc_5_0DCA
        movzx  ax, byte ptr [ebp - 0x74]
        movzx  edx, di
        mov    word ptr [ebp - 2], ax
        mov    dword ptr [ebp - 0xc], edx
        movzx  cx, byte ptr [ebp - 0x64]
        mov    word ptr [ebp - 4], cx
        cmp    edx, -1
        jne    loc_5_0CCB
        mov    eax, 0x13
        jmp    loc_5_0E2F
loc_5_0CCB:
        push   0
        lea    eax, [ebp - 0x2c]
        push   0
        lea    ecx, [ebp - 0x10]
        mov    dword ptr [ebp - 0x2c], 0
        push   eax
        push   ecx
        push   esi
        call   sub_5_1988
        add    esp, 0x14
        test   eax, eax
        je     loc_5_0CF2
        mov    dword ptr [ebp - 0x10], 0
loc_5_0CF2:
        mov    eax, dword ptr [ebp - 0x10]
        cmp    eax, 8
        je     loc_5_0D01
        cmp    eax, 0x10
        je     loc_5_0D07
        jmp    loc_5_0D0B
loc_5_0D01:
        or     byte ptr [ebp - 8], 0x20
        jmp    loc_5_0D0B
loc_5_0D07:
        or     byte ptr [ebp - 8], 0x10
loc_5_0D0B:
        lea    eax, [ebp - 8]
        push   eax
        push   esi
        call   sub_5_0B2F
        add    esp, 8
        push   dword ptr [ebp - 8]
        push   0x10
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp - 2]
        push   ebx
        push   edi
        push   esi
        call   sub_4_0519
loc_5_0D2B:
        add    esp, 0x1c
        mov    edi, eax
        test   edi, edi
        jne    loc_5_0DB9
        sub    bx, 0xffff
        mov    dword ptr [ebp - 0x20], esi
        mov    dword ptr [ebp - 0x24], 0     ; -> obj6:0x00E4
        cmp    bx, 1
        sbb    eax, eax
        inc    eax
        mov    dword ptr [ebp - 0x28], eax
        push   dword ptr [ebp - 0x28]
        push   dword ptr [ebp - 0x24]
        push   dword ptr [ebp - 0x20]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VxDCall  VFAT.Service_0001h              ; cd2001004a04
        add    esp, 0xc
        test   edi, edi
        jne    loc_5_0DC6
        cmp    word ptr [ebp - 2], 9
        mov    eax, 2
        je     loc_5_0D7D
        movzx  eax, word ptr [ebp - 2]
loc_5_0D7D:
        movzx  ecx, word ptr [ebp - 4]
        push   ecx
        push   eax
        push   dword ptr [ebp - 0xc]
        push   0                             ; -> obj6:0x00D4
        lea    ecx, [ebp - 0x4c]
        push   ecx
        call   sub_5_1978
        add    esp, 0x14
        lea    ecx, [ebp - 0x4c]
        mov    dword ptr [ebp - 0x18], 0     ; -> obj6:0x00CC
        mov    dword ptr [ebp - 0x1c], ecx
        push   dword ptr [ebp - 0x1c]
        push   dword ptr [ebp - 0x18]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VxDCall  VFAT.Service_0002h              ; cd2002004a04
        add    esp, 8
loc_5_0DB9:
        test   edi, edi
        jne    loc_5_0DC6
        push   esi
        call   sub_5_0520
        add    esp, 4
loc_5_0DC6:
        mov    eax, edi
        jmp    loc_5_0E2F
loc_5_0DCA:
        mov    eax, 0x13
        jmp    loc_5_0E2F
loc_5_0DD1:
        mov    esi, dword ptr [ebp + 0x10]
        push   esi
        call   sub_5_05DF
        add    esp, 4
        push   esi
        call   sub_4_05B1
loc_5_0DE3:
        mov    dword ptr [ebp - 0x14], 0     ; -> obj6:0x00CC
        add    esp, 4
        push   dword ptr [ebp - 0x14]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VxDCall  VFAT.Service_0004h              ; cd2004004a04
        add    esp, 4
        xor    eax, eax
        jmp    loc_5_0E2F
loc_5_0E03:
        mov    eax, dword ptr [ebp + 0xc]
        test   eax, eax
        je     loc_5_0E16
        cmp    eax, 1
        je     loc_5_0E16
        mov    eax, 1
        jmp    loc_5_0E2F
loc_5_0E16:
        push   dword ptr [ebp + 0x10]
        call   sub_4_0000
loc_5_0E1E:
        add    esp, 4
        cmp    eax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 0x13
loc_5_0E2F:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  0CCh, 0CCh
loc_5_0E38:
        push   eax
        mov    eax, dword ptr [esi + 0x28]
        cmp    eax, ebx
        je     loc_5_0E63
        or     eax, eax
        jne    loc_5_0E76
        push   ebx
        push   ecx
        push   edx
        mov    edi, esi
        mov    eax, 1
        test   word ptr [esi + 4], 0x100
        je     loc_5_0E59
        or     eax, 2
loc_5_0E59:
        call   sub_5_11B9
        pop    edx
        pop    ecx
        pop    ebx
        jb     loc_5_0E76
loc_5_0E63:
        pop    eax
        cmp    ecx, 4
        jbe    loc_5_0E6F
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_5_0E6F:
        je     loc_5_0E74
        in     al, dx
        jmp    loc_5_0E75
loc_5_0E74:
        out    dx, al
loc_5_0E75:
        ret    
loc_5_0E76:
        mov    eax, 1
        mov    edi, esi
        call   sub_5_1036
        pop    eax
        xor    eax, eax
        dec    eax
        ret    
sub_5_0E87:
        push   eax
        push   edx
        push   ebx
        push   ecx
        movzx  edx, word ptr [edi + 8]
        mov    ebx, 0x105
        mov    al, 1
        call   sub_1_032B
loc_5_0E9B:
        mov    ebx, 4
        mov    al, 0x60
        call   sub_1_032B
loc_5_0EA7:
        mov    ebx, 0x104
        mov    al, 0x3f
        call   sub_1_032B
loc_5_0EB3:
        mov    ebx, 8
        xor    al, al
        call   sub_1_032B
loc_5_0EBF:
        mov    ebx, 0xbd
        mov    al, 0xc0
        call   sub_1_032B
loc_5_0ECB:
        mov    ecx, 0x15
        mov    ebx, 0x40
        mov    al, 0x3f
loc_5_0ED7:
        xor    bh, bh
        call   sub_1_032B
loc_5_0EDE:
        mov    bh, 1
        call   sub_1_032B
loc_5_0EE5:
        inc    bl
        loop   loc_5_0ED7
        mov    ecx, 8
        mov    ebx, 0xb0
        xor    eax, eax
loc_5_0EF5:
        xor    bh, bh
        call   sub_1_032B
loc_5_0EFC:
        mov    bh, 1
        call   sub_1_032B
loc_5_0F03:
        inc    bl
        loop   loc_5_0EF5
        mov    ebx, 0x104
        xor    eax, eax
        call   sub_1_032B
loc_5_0F13:
        mov    ebx, 0x105
        call   sub_1_032B
loc_5_0F1D:
        call   sub_1_0322
loc_5_0F22:
        call   sub_1_0322
loc_5_0F27:
        call   sub_1_0322
loc_5_0F2C:
        pop    ecx
        pop    ebx
        pop    edx
        pop    eax
        ret    
        db  02Eh, 08Bh, 0C0h
loc_5_0F34:
        push   eax
        test   word ptr [esi + 4], 0x100
        je     loc_5_0F61
        mov    eax, dword ptr [esi + 0x30]
        cmp    eax, ebx
        je     loc_5_0F61
        or     eax, eax
        jne    loc_5_0F74
        push   ebx
        push   ecx
        push   edx
        mov    edi, esi
        mov    eax, 2
        call   sub_5_11B9
        pop    edx
        pop    ecx
        pop    ebx
        jb     loc_5_0F74
        call   sub_5_0E87
loc_5_0F61:
        pop    eax
        cmp    ecx, 4
        jbe    loc_5_0F6D
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_5_0F6D:
        je     loc_5_0F72
        in     al, dx
        jmp    loc_5_0F73
loc_5_0F72:
        out    dx, al
loc_5_0F73:
        ret    
loc_5_0F74:
        mov    eax, 2
        mov    edi, esi
        call   sub_5_1036
        pop    eax
        xor    eax, eax
        dec    eax
        ret    
sub_5_0F85:
        push   esi
        push   eax
        push   ecx
        push   edx
        mov    si, 0xf
        xor    ecx, ecx
        movzx  edx, word ptr [edi + 6]
loc_5_0F93:
        VMMCall  VMM.Enable_Local_Trapping       ; cd2098000100
        inc    dx
        cmp    cx, si
        je     loc_5_0FA4
        inc    cx
        jmp    loc_5_0F93
loc_5_0FA4:
        pop    edx
        pop    ecx
        pop    eax
        pop    esi
        clc    
        ret    
sub_5_0FAA:
        push   esi
        push   eax
        push   ecx
        push   edx
        mov    si, 0xf
        xor    ecx, ecx
        movzx  edx, word ptr [edi + 6]
loc_5_0FB8:
        VMMCall  VMM.Disable_Local_Trapping      ; cd209a000100
        inc    dx
        cmp    cx, si
        je     loc_5_0FC9
        inc    cx
        jmp    loc_5_0FB8
loc_5_0FC9:
        pop    edx
        pop    ecx
        pop    eax
        pop    esi
        clc    
        ret    
sub_5_0FCF:
        push   eax
        push   ebx
        push   edx
        movzx  edx, word ptr [edi + 8]
        mov    ebx, 4
        mov    eax, 0x60
        call   sub_1_032B
loc_5_0FE5:
        call   sub_1_0322
loc_5_0FEA:
        pop    edx
        pop    ebx
        pop    eax
        ret    
sub_5_0FEE:
        push   esi
        push   eax
        push   ecx
        mov    si, 3
        xor    ecx, ecx
        movzx  edx, word ptr [edi + 8]
loc_5_0FFB:
        VMMCall  VMM.Enable_Local_Trapping       ; cd2098000100
        inc    dx
        cmp    cx, si
        je     loc_5_100C
        inc    cx
        jmp    loc_5_0FFB
loc_5_100C:
        pop    ecx
        pop    eax
        pop    esi
        clc    
        ret    
sub_5_1011:
        push   esi
        push   eax
        push   ecx
        push   edx
        mov    si, 3
        xor    ecx, ecx
        movzx  edx, word ptr [edi + 8]
loc_5_101F:
        VMMCall  VMM.Disable_Local_Trapping      ; cd209a000100
        inc    dx
        cmp    cx, si
        je     loc_5_1030
        inc    cx
        jmp    loc_5_101F
loc_5_1030:
        pop    edx
        pop    ecx
        pop    eax
        pop    esi
        clc    
        ret    
sub_5_1036:
        test   word ptr [edi + 4], 1
        jne    loc_5_108B
        call   sub_1_0195
loc_5_1043:
        jb     loc_5_108B
        cmp    eax, 2
        je     loc_5_1064
        test   dword ptr [esi], 1
        jne    loc_5_108B
        or     dword ptr [esi], 1
        movzx  ecx, word ptr [0xc]           ; -> obj2:0x000C
        add    ecx, dword ptr [4]            ; -> obj2:0x0004
        jmp    loc_5_107C
loc_5_1064:
        test   dword ptr [esi], 2
        jne    loc_5_108B
        or     dword ptr [esi], 2
        movzx  ecx, word ptr [0xe]           ; -> loc_2_000E
        add    ecx, dword ptr [4]            ; -> obj2:0x0004
loc_5_107C:
        mov    eax, 0x30
        xor    esi, esi
        xor    edi, edi
        VxDCall  SHELL.SHELL_Message             ; cd2004001700
loc_5_108B:
        ret    
loc_5_108C:
        pushfd 
        mov    esi, dword ptr [0]            ; -> obj1:0x00D4
        or     esi, esi
        je     loc_5_10D3
        cli    
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_5_10D3
loc_5_10A0:
        popfd  
        pushfd 
        push   eax
        mov    edi, dword ptr [eax]
        xor    eax, eax
        cmp    dword ptr [edi + 0x28], ebx
        jne    loc_5_10B6
        or     eax, 1
        mov    dword ptr [edi + 0x2c], 0xffffffff
loc_5_10B6:
        cmp    dword ptr [edi + 0x30], ebx
        jne    loc_5_10BE
        or     eax, 2
loc_5_10BE:
        or     eax, eax
        je     loc_5_10C7
        call   sub_5_1149
loc_5_10C7:
        pop    eax
        cli    
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        je     loc_5_10D3
        jmp    loc_5_10A0
loc_5_10D3:
        mov    edi, dword ptr [0]            ; -> obj1:0x00D0
        xor    esi, esi
        xchg   dword ptr [edi + ebx], esi
        or     esi, esi
        je     loc_5_10E8
        VMMCall  VMM.List_Destroy                ; cd209c000100
loc_5_10E8:
        popfd  
        clc    
        ret    
loc_5_10EB:
        pushal 
        pushfd 
        mov    edi, dword ptr [0]            ; -> obj1:0x00D0
        mov    eax, 5
        mov    ecx, 8
        cli    
        VMMCall  VMM.List_Create                 ; cd209b000100
        jb     loc_5_1141
        mov    dword ptr [edi + ebx], esi
        mov    esi, dword ptr [0]            ; -> obj1:0x00D4
        or     esi, esi
        je     loc_5_1145
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_5_1145
loc_5_111B:
        mov    edi, dword ptr [eax]
        call   sub_4_0087
loc_5_1122:
        jb     loc_5_112E
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        je     loc_5_1145
        jmp    loc_5_111B
loc_5_112E:
        mov    edi, dword ptr [0]            ; -> obj1:0x00D0
        mov    esi, dword ptr [edi + ebx]
        or     esi, esi
        je     loc_5_1141
        VMMCall  VMM.List_Destroy                ; cd209c000100
loc_5_1141:
        popfd  
        stc    
        jmp    loc_5_1147
loc_5_1145:
        popfd  
        clc    
loc_5_1147:
        popal  
        ret    
sub_5_1149:
        push   eax
        push   edx
        push   esi
        call   sub_1_0195
loc_5_1151:
        jb     loc_5_11B4
        test   eax, 2
        je     loc_5_1173
        cmp    dword ptr [edi + 0x30], ebx
        jne    loc_5_1173
        call   sub_5_0FCF
        call   sub_5_0FEE
        mov    dword ptr [edi + 0x30], 0
        and    dword ptr [esi], 0xfffffffd
loc_5_1173:
        test   eax, 1
        je     loc_5_11B4
        cmp    dword ptr [edi + 0x28], ebx
        jne    loc_5_11B4
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_5_1190
        mov    eax, dword ptr [edi + 0x38]
        VxDCall  VDMAD.VDMAD_Enable_Translation  ; cd2014000400
loc_5_1190:
        call   sub_5_0F85
        mov    dword ptr [edi + 0x28], 0
        mov    eax, dword ptr [edi + 0x24]
        VxDCall  VPICD.VPICD_Physically_Mask     ; cd2008000300
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
        VxDCall  VPICD.VPICD_Clear_Int_Request   ; cd2003000300
        and    dword ptr [esi], 0xfffffffe
loc_5_11B4:
        pop    esi
        pop    edx
        pop    eax
        clc    
        ret    
sub_5_11B9:
        push   eax
        push   ecx
        push   edx
        test   eax, 2
        je     loc_5_11DB
        cmp    dword ptr [edi + 0x30], 0
        je     loc_5_11D0
        cmp    dword ptr [edi + 0x30], ebx
        je     loc_5_11DB
        jmp    loc_5_11ED
loc_5_11D0:
        mov    dword ptr [edi + 0x30], ebx
        call   sub_5_1011
        mov    dword ptr [edi + 0x34], ebx
loc_5_11DB:
        test   eax, 1
        je     loc_5_123A
        cmp    dword ptr [edi + 0x28], 0
        je     loc_5_11F0
        cmp    dword ptr [edi + 0x28], ebx
        je     loc_5_1239
loc_5_11ED:
        stc    
        jb     loc_5_123A
loc_5_11F0:
        mov    dword ptr [edi + 0x28], ebx
        cmp    dword ptr [edi + 0x2c], ebx
        je     loc_5_1201
        movzx  edx, word ptr [edi + 6]
        call   sub_1_02F8
loc_5_1201:
        mov    dword ptr [edi + 0x2c], ebx
        call   sub_5_0FAA
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_5_121A
        mov    eax, dword ptr [edi + 0x38]
        VxDCall  VDMAD.VDMAD_Disable_Translation ; cd2013000400
loc_5_121A:
        mov    eax, dword ptr [edi + 0x24]
        VxDCall  VPICD.VPICD_Get_Complete_Status ; cd2005000300
        test   ecx, 0x10
        je     loc_5_1233
        VxDCall  VPICD.VPICD_Physically_Mask     ; cd2008000300
        jmp    loc_5_1239
loc_5_1233:
        VxDCall  VPICD.VPICD_Physically_Unmask   ; cd2009000300
loc_5_1239:
        clc    
loc_5_123A:
        pop    edx
        pop    ecx
        pop    eax
        ret    
sub_5_123E:
        mov    eax, 0x400
        clc    
        ret    
        db  0CCh, 0CCh, 0CCh
mssblst_PM_API:
        movzx  eax, word ptr [ebp + 0x14]
        cmp    ah, 3
        jae    loc_5_1272
        movzx  ecx, ah
        shl    ecx, 3
        lea    ecx, [ecx]                    ; -> obj6:0x0120
        movzx  eax, al
        cmp    eax, dword ptr [ecx]
        jae    loc_5_1272
        mov    ecx, dword ptr [ecx + 4]
        call   dword ptr [ecx + eax*4]
        jb     loc_5_1272
        and    word ptr [ebp + 0x2c], 0xfffe
        ret    
loc_5_1272:
        or     word ptr [ebp + 0x2c], 1
        ret    
loc_5_1278:
        call   sub_5_123E
        mov    word ptr [ebp + 0x1c], ax
        clc    
        ret    
loc_5_1283:
        movzx  eax, word ptr [ebp + 0x10]
        movzx  edx, word ptr [ebp + 0x1c]
        or     edx, edx
        je     loc_5_12C5
        test   eax, 1
        je     loc_5_12AA
        push   2
        push   edx
        call   sub_1_0134
loc_5_129E:
        add    esp, 8
        or     edi, edi
        je     loc_5_12C5
        mov    ecx, dword ptr [edi + 0x2c]
        jmp    loc_5_12CD
loc_5_12AA:
        test   eax, 2
        je     loc_5_12C5
        push   3
        push   edx
        call   sub_1_0134
loc_5_12B9:
        add    esp, 8
        or     edi, edi
        je     loc_5_12C5
        mov    ecx, dword ptr [edi + 0x34]
        jmp    loc_5_12CD
loc_5_12C5:
        mov    word ptr [ebp + 0x1c], 1
loc_5_12CB:
        stc    
        ret    
loc_5_12CD:
        mov    word ptr [ebp + 0x1c], 0
        call   sub_5_11B9
        jb     loc_5_12DB
        ret    
loc_5_12DB:
        mov    word ptr [ebp + 0x1c], 2
        jmp    loc_5_12CB
loc_5_12E3:
        movzx  eax, word ptr [ebp + 0x10]
        movzx  edx, word ptr [ebp + 0x1c]
        or     edx, edx
        je     loc_5_133F
        test   eax, 1
        je     loc_5_1324
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_5_1310
        push   2
        push   edx
        call   sub_1_0134
loc_5_1306:
        add    esp, 8
        or     edi, edi
        jne    loc_5_131F
        add    edx, 4
loc_5_1310:
        push   2
        push   edx
        call   sub_1_0134
loc_5_1318:
        add    esp, 8
        or     edi, edi
        je     loc_5_133F
loc_5_131F:
        mov    ecx, dword ptr [edi + 0x28]
        jmp    loc_5_1347
loc_5_1324:
        test   eax, 2
        je     loc_5_133F
        push   3
        push   edx
        call   sub_1_0134
loc_5_1333:
        add    esp, 8
        or     edi, edi
        je     loc_5_133F
        mov    ecx, dword ptr [edi + 0x30]
        jmp    loc_5_1347
loc_5_133F:
        mov    word ptr [ebp + 0x1c], 1
loc_5_1345:
        stc    
        ret    
loc_5_1347:
        cmp    ecx, ebx
        je     loc_5_1353
        mov    word ptr [ebp + 0x1c], 2
        jmp    loc_5_1345
loc_5_1353:
        call   sub_5_1149
        mov    word ptr [ebp + 0x1c], 0
        clc    
        ret    
loc_5_1360:
        pushfd 
        mov    ax, word ptr [ebp + 0x1c]
        and    ax, 0xfffe
        jne    loc_5_13CE
        xor    ecx, ecx
        mov    ax, word ptr [ebp + 0x1c]
        and    ax, 1
        je     loc_5_137A
        mov    ecx, dword ptr [ebp + 0x18]
loc_5_137A:
        cli    
        mov    esi, dword ptr [0]            ; -> obj1:0x00D4
        or     esi, esi
        je     loc_5_13CE
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_5_13CE
        mov    edi, dword ptr [eax]
        jecxz  loc_5_13A2
loc_5_1391:
        cmp    dword ptr [edi + 0x40], ecx
        je     loc_5_13A2
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        je     loc_5_13CE
        mov    edi, dword ptr [eax]
        jmp    loc_5_1391
loc_5_13A2:
        popfd  
        pushfd 
        mov    esi, edi
        xchg   edi, eax
        mov    ax, 0x3810
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   edi, eax
        cmp    edi, -1
        je     loc_5_13CE
        mov    ecx, dword ptr [edi]
        or     ecx, ecx
        je     loc_5_13CE
        cmp    ecx, 0x52
        ja     loc_5_13CE
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    word ptr [ebp + 0x1c], 1
        popfd  
        clc    
        ret    
loc_5_13CE:
        mov    word ptr [ebp + 0x1c], 0
        popfd  
        stc    
        ret    
loc_5_13D7:
        push   edi
        mov    eax, dword ptr [ebp + 0x18]
        push   0
        push   eax
        call   sub_1_0134
loc_5_13E3:
        add    esp, 8
        jb     loc_5_13FE
        mov    eax, dword ptr [edi + 0x38]
        or     eax, eax
        je     loc_5_13FE
        push   ecx
        VxDCall  VDMAD.VDMAD_Get_Phys_Count      ; cd201c000400
        mov    word ptr [ebp + 0x1c], cx
        pop    ecx
        clc    
        jmp    loc_5_1405
loc_5_13FE:
        mov    word ptr [ebp + 0x1c], 0xffff
        stc    
loc_5_1405:
        pop    edi
        ret    
loc_5_1407:
        movzx  edx, word ptr [ebp + 0x1c]
        or     edx, edx
        je     loc_5_143C
        push   3
        push   edx
        call   sub_1_0134
loc_5_1417:
        add    esp, 8
        or     edi, edi
        je     loc_5_143C
        mov    ecx, dword ptr [edi + 0x34]
        mov    word ptr [ebp + 0x1c], 0
        mov    eax, 2
        call   sub_5_11B9
        jb     loc_5_1434
        ret    
loc_5_1434:
        mov    word ptr [ebp + 0x1c], 2
        jmp    loc_5_1442
loc_5_143C:
        mov    word ptr [ebp + 0x1c], 1
loc_5_1442:
        stc    
        ret    
loc_5_1444:
        movzx  edx, word ptr [ebp + 0x1c]
        or     edx, edx
        je     loc_5_147C
        push   3
        push   edx
        call   sub_1_0134
loc_5_1454:
        add    esp, 8
        or     edi, edi
        je     loc_5_147C
        mov    ecx, dword ptr [edi + 0x30]
        cmp    ecx, ebx
        je     loc_5_146A
        mov    word ptr [ebp + 0x1c], 3
        jmp    loc_5_1482
loc_5_146A:
        mov    eax, 2
        call   sub_5_1149
        mov    word ptr [ebp + 0x1c], 0
        clc    
        ret    
loc_5_147C:
        mov    word ptr [ebp + 0x1c], 1
loc_5_1482:
        stc    
        ret    
loc_5_1484:
        pushfd 
        mov    ax, word ptr [ebp + 0x1c]
        cmp    ax, 1
        jne    loc_5_1507
        mov    ecx, dword ptr [ebp + 0x18]
        cli    
        mov    esi, dword ptr [0]            ; -> obj1:0x00D4
        or     esi, esi
        je     loc_5_1507
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_5_1507
loc_5_14A5:
        mov    edi, dword ptr [eax]
        cmp    dword ptr [edi + 0x40], ecx
        je     loc_5_14B6
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        je     loc_5_1507
        jmp    loc_5_14A5
loc_5_14B6:
        popfd  
        pushfd 
        cmp    word ptr [edi + 8], -1
        je     loc_5_1507
        mov    esi, edi
        xchg   edi, eax
        mov    ax, 0x3810
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   edi, eax
        cmp    edi, -1
        je     loc_5_1507
        mov    ecx, dword ptr [edi]
        or     ecx, ecx
        je     loc_5_1507
        cmp    ecx, 0x1c
        ja     loc_5_1507
        xor    ax, ax
        cmp    word ptr [esi + 0xc], 0x302
        jbe    loc_5_14EC
        mov    ax, 1
loc_5_14EC:
        mov    word ptr [edi + 4], ax
        mov    ax, word ptr [esi + 8]
        mov    word ptr [edi + 6], ax
        mov    eax, dword ptr [esi + 0x40]
        mov    dword ptr [edi + 0xc], eax
        mov    word ptr [ebp + 0x1c], 1
        popfd  
        clc    
        ret    
loc_5_1507:
        mov    word ptr [ebp + 0x1c], 0
        popfd  
        stc    
        ret    
loc_5_1510:
        xchg   esi, eax
        mov    ax, 0x3430
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   esi, eax
        push   dword ptr [ebp + 0x3c]
        push   dword ptr [ebp + 0x14]
        mov    eax, dword ptr [esi + 4]
        mov    word ptr [ebp + 0x14], ax
        shr    eax, 0x10
        mov    word ptr [ebp + 0x3c], ax
        mov    ax, 0x3c14
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        mov    dword ptr [esi + 4], eax
        mov    eax, dword ptr [esi + 8]
        mov    word ptr [ebp + 0x14], ax
        shr    eax, 0x10
        mov    word ptr [ebp + 0x3c], ax
        mov    ax, 0x3c14
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        mov    dword ptr [esi + 8], eax
        pop    dword ptr [ebp + 0x14]
        pop    dword ptr [ebp + 0x3c]
        mov    eax, dword ptr [esi]
        push   0
        push   eax
        call   sub_1_0134
loc_5_1568:
        add    esp, 8
        or     edi, edi
        je     loc_5_1594
        lea    eax, [edi + 0x3c]
        push   dword ptr [esi + 8]
        push   dword ptr [esi + 4]
        push   eax
        push   dword ptr [esi]
        call   sub_5_16C7
        add    esp, 0x10
        or     eax, eax
        je     loc_5_1594
        mov    word ptr [ebp + 0x1c], ax
        shr    eax, 0x10
        mov    word ptr [ebp + 0x14], ax
        clc    
        ret    
loc_5_1594:
        mov    word ptr [ebp + 0x1c], 0
        mov    word ptr [ebp + 0x14], 0
        stc    
        ret    
loc_5_15A2:
        xchg   esi, eax
        mov    ax, 0x3430
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   esi, eax
        mov    eax, dword ptr [esi]
        push   0
        push   eax
        call   sub_1_0134
loc_5_15B8:
        add    esp, 8
        or     edi, edi
        je     loc_5_15CE
        lea    eax, [edi + 0x3c]
        push   dword ptr [esi + 4]
        push   eax
        call   sub_5_1857
        add    esp, 8
loc_5_15CE:
        mov    word ptr [ebp + 0x1c], 0
        clc    
        ret    
sub_5_15D6:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x6c
        mov    eax, 0xffffffff
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        test   esi, esi
        je     loc_5_16C0
        lea    eax, [ebp - 0x6c]
        push   eax
        call   sub_5_18DE
        add    esp, 4
        call   sub_5_18CC
        call   sub_5_191B
        mov    edi, dword ptr [eax + 8]
        mov    ebx, dword ptr [ebp + 0xc]
        mov    eax, ebx
        shr    eax, 0x10
        movzx  ecx, ax
        push   ecx
        call   sub_5_190D
        add    esp, 4
        movzx  ecx, bx
        push   ecx
        call   sub_5_190D
        add    esp, 4
        mov    ebx, dword ptr [ebp + 0x10]
        mov    eax, ebx
        shr    eax, 0x10
        movzx  ecx, ax
        push   ecx
        call   sub_5_190D
        add    esp, 4
        movzx  ecx, bx
        push   ecx
        call   sub_5_190D
        add    esp, 4
        mov    ebx, dword ptr [ebp + 0x14]
        mov    eax, ebx
        shr    eax, 0x10
        movzx  ecx, ax
        push   ecx
        call   sub_5_190D
        add    esp, 4
        movzx  ecx, bx
        push   ecx
        call   sub_5_190D
        add    esp, 4
        mov    ebx, dword ptr [ebp + 0x18]
        mov    eax, ebx
        shr    eax, 0x10
        movzx  ecx, ax
        push   ecx
        call   sub_5_190D
        add    esp, 4
        movzx  ecx, bx
        push   ecx
        call   sub_5_190D
        movzx  ecx, si
        shr    esi, 0x10
        add    esp, 4
        movzx  eax, si
        push   ecx
        push   eax
        call   sub_5_18FE
        add    esp, 8
        call   sub_5_18D8
        movzx  esi, word ptr [edi + 0x14]
        shl    esi, 0x10
        movzx  eax, word ptr [edi + 0x1c]
        or     esi, eax
        call   sub_5_18D2
        lea    ecx, [ebp - 0x6c]
        push   ecx
        call   sub_5_18EE
        add    esp, 4
        mov    eax, esi
loc_5_16C0:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_16C7:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0x10]
        push   edi
        push   ebp
        mov    ebp, dword ptr [esi]
        test   ebp, ebp
        jne    loc_5_16ED
        push   0x15
        push   4
        call   sub_5_19E4
        mov    ebp, eax
        test   ebp, ebp
        jne    loc_5_16EB
        xor    eax, eax
        jmp    loc_5_1852
loc_5_16EB:
        mov    dword ptr [esi], ebp
loc_5_16ED:
        mov    edi, dword ptr [esp + 0x1c]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        dec    ecx
        cmp    ecx, 8
        jbe    loc_5_170A
        mov    eax, dword ptr [esp + 0x1c]
        mov    byte ptr [eax + 8], 0
loc_5_170A:
        push   ebp
        call   sub_5_19D4
        mov    ebx, eax
        test   ebx, ebx
        je     loc_5_17FE
loc_5_171A:
        mov    ecx, dword ptr [esp + 0x1c]
        lea    edx, [ebx + 0xc]
loc_5_1721:
        mov    al, byte ptr [ecx]
        cmp    al, byte ptr [edx]
        jne    loc_5_1741
        or     al, al
        je     loc_5_173D
        mov    al, byte ptr [ecx + 1]
        cmp    al, byte ptr [edx + 1]
        jne    loc_5_1741
        add    ecx, 2
        add    edx, 2
        or     al, al
        jne    loc_5_1721
loc_5_173D:
        xor    eax, eax
        jmp    loc_5_1746
loc_5_1741:
        sbb    eax, eax
        sbb    eax, -1
loc_5_1746:
        test   eax, eax
        je     loc_5_1757
        push   ebx
        push   ebp
        call   sub_5_19C4
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_5_171A
loc_5_1757:
        test   ebx, ebx
        je     loc_5_17FE
        cmp    dword ptr [ebx + 4], 0
        je     loc_5_176C
        xor    eax, eax
        jmp    loc_5_1852
loc_5_176C:
        push   0
        push   dword ptr [ebx + 8]
        push   0
        push   ebx
        push   dword ptr [ebx]
        call   sub_5_15D6
        add    esp, 0x14
        test   eax, eax
        je     loc_5_1799
        push   ebx
        mov    dword ptr [ebx], 0
        push   esi
        call   sub_5_1857
        add    esp, 8
        xor    eax, eax
        jmp    loc_5_1852
loc_5_1799:
        mov    eax, dword ptr [esp + 0x20]
        push   0
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ebx + 4], ecx
        push   dword ptr [ebx + 8]
        push   0
        push   ebx
        push   ecx
        call   sub_5_15D6
        add    esp, 0x14
        test   eax, eax
        je     loc_5_17D0
        mov    dword ptr [ebx + 4], 0
        push   ebx
        push   esi
        call   sub_5_1857
        add    esp, 8
        xor    eax, eax
        jmp    loc_5_1852
loc_5_17D0:
        push   dword ptr [ebx + 4]
        push   dword ptr [ebx + 8]
        push   0x10000
        push   ebx
        push   dword ptr [ebx]
        call   sub_5_15D6
        add    esp, 0x14
        push   dword ptr [ebx]
        push   dword ptr [ebx + 8]
        push   0x10000
        push   ebx
        push   dword ptr [ebx + 4]
        call   sub_5_15D6
        add    esp, 0x14
        jmp    loc_5_1850
loc_5_17FE:
        push   ebp
        call   sub_5_19B0
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_5_180E
        xor    eax, eax
        jmp    loc_5_1852
loc_5_180E:
        mov    eax, dword ptr [esp + 0x14]
        mov    ecx, dword ptr [esp + 0x20]
        mov    edi, dword ptr [esp + 0x1c]
        mov    dword ptr [ebx + 8], eax
        mov    edx, dword ptr [ecx + 4]
        sub    eax, eax
        mov    ecx, 0xffffffff
        mov    dword ptr [ebx], edx
        mov    dword ptr [ebx + 4], 0
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    eax, ecx
        shr    ecx, 2
        mov    esi, edi
        lea    edi, [ebx + 0xc]
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, eax
        push   ebx
        and    ecx, 3
        push   ebp
        rep movsb byte ptr es:[edi], byte ptr [esi]
        call   sub_5_19A0
loc_5_1850:
        mov    eax, ebx
loc_5_1852:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_5_1857:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x10]
        test   edi, edi
        je     loc_5_1896
        mov    eax, dword ptr [edi]
        test   eax, eax
        je     loc_5_187B
        push   0
        push   dword ptr [edi + 8]
        push   0x40000
        push   edi
        push   eax
        call   sub_5_15D6
        add    esp, 0x14
loc_5_187B:
        mov    eax, dword ptr [edi + 4]
        test   eax, eax
        je     loc_5_1896
        push   0
        push   dword ptr [edi + 8]
        push   0x40000
        push   edi
        push   eax
        call   sub_5_15D6
        add    esp, 0x14
loc_5_1896:
        mov    esi, dword ptr [esp + 0xc]
        mov    eax, dword ptr [esi]
        test   eax, eax
        je     loc_5_18C7
        push   edi
        push   eax
        call   sub_5_1A18
        push   edi
        push   dword ptr [esi]
        call   sub_5_1A08
        push   dword ptr [esi]
        call   sub_5_19D4
        test   eax, eax
        jne    loc_5_18C1
        push   dword ptr [esi]
        call   sub_5_19F8
loc_5_18C1:
        mov    dword ptr [esi], 0
loc_5_18C7:
        pop    edi
        pop    esi
        ret    
        db  0CCh, 0CCh
sub_5_18CC:
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083800100
sub_5_18D2:
        VMMCall  VMM.End_Nest_Exec               ; cd2086800100
sub_5_18D8:
        VMMCall  VMM.Resume_Exec                 ; cd2085800100
sub_5_18DE:
        push   ebp
        mov    ebp, esp
        push   edi
        mov    edi, dword ptr [ebp + 8]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        pop    ebp
        ret    
sub_5_18EE:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        pop    ebp
        ret    
sub_5_18FE:
        movzx  ecx, word ptr [esp + 4]
        mov    edx, dword ptr [esp + 8]
        VMMCall  VMM.Simulate_Far_Call           ; cd2048800100
sub_5_190D:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        pop    ebp
        ret    
sub_5_191B:
        push   ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    eax, ebx
        pop    ebx
        ret    
        db  0CCh, 0CCh
sub_5_1928:
        VMMCall  VMM._HeapFree                   ; cd2051800100
        int3   
        int3   
sub_5_1930:
        VMMCall  VMM._lstrcpyn                   ; cd207b810100
        int3   
        int3   
sub_5_1938:
        VMMCall  VMM._HeapAllocate               ; cd204f800100
        int3   
        int3   
sub_5_1940:
        pop    ecx
        pop    eax
        pop    edx
        push   ebx
        mov    ebx, eax
        VxDCall  APM.Service_0002h               ; cd2002002700
        sbb    edx, edx
        and    eax, edx
        pop    ebx
        jmp    ecx
        db  0CCh
sub_5_1954:
        mov    eax, dword ptr [esp + 0x10]
        mov    edx, dword ptr [esp + 0xc]
        VxDCall  APM.Service_0001h               ; cd2001002700
        jb     loc_5_1972
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [ecx], edx
        xor    eax, eax
loc_5_1972:
        ret    0x10
        db  0CCh, 0CCh, 0CCh
sub_5_1978:
        VMMCall  VMM._Sprintf                    ; cd2074810100
        int3   
        int3   
sub_5_1980:
        VxDCall  CONFIGMG._CONFIGMG_Read_Registry_Value ; cd203e803300
        int3   
        int3   
sub_5_1988:
        VxDCall  CONFIGMG._CONFIGMG_Get_Bus_Info ; cd2044803300
        int3   
        int3   
sub_5_1990:
        VxDCall  CONFIGMG._CONFIGMG_Get_Alloc_Log_Conf ; cd203b803300
        int3   
        int3   
sub_5_1998:
        VxDCall  CONFIGMG._CONFIGMG_Get_DevNode_Key ; cd203d803300
        int3   
        int3   
sub_5_19A0:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        VMMCall  VMM.List_Attach_Tail            ; cd209f000100
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_5_19B0:
        pop    edx
        mov    ecx, esi
        pop    esi
        VMMCall  VMM.List_Allocate               ; cd209d000100
        mov    esi, ecx
        cmc    
        sbb    ecx, ecx
        and    eax, ecx
        jmp    edx
        db  0CCh
sub_5_19C4:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_5_19D4:
        pop    edx
        mov    ecx, esi
        pop    esi
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        mov    esi, ecx
        jmp    edx
        db  0CCh, 0CCh
sub_5_19E4:
        pop    edx
        pop    eax
        pop    ecx
        push   esi
        VMMCall  VMM.List_Create                 ; cd209b000100
        mov    eax, esi
        pop    esi
        cmc    
        sbb    ecx, ecx
        and    eax, ecx
        jmp    edx
sub_5_19F8:
        pop    edx
        mov    ecx, esi
        pop    esi
        VMMCall  VMM.List_Destroy                ; cd209c000100
        mov    esi, ecx
        jmp    edx
        db  0CCh, 0CCh
sub_5_1A08:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        VMMCall  VMM.List_Deallocate             ; cd20a2000100
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_5_1A18:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        VMMCall  VMM.List_Remove                 ; cd20a1000100
        mov    esi, ecx
        sbb    eax, eax
        inc    eax
        jmp    edx
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 6  vsize=0x25e  RW-
        dd  loc_5_0280               ; fixup
        dd  loc_5_02C8               ; fixup
        dd  loc_5_02D6               ; fixup
        dd  loc_5_02F3               ; fixup
        dd  loc_5_0382               ; fixup
        dd  loc_5_04F8               ; fixup
        dd  loc_5_03AE               ; fixup
        dd  loc_5_040D               ; fixup
        dd  loc_5_04E6               ; fixup
        db  04Dh, 053h, 053h, 042h, 04Ch, 053h, 054h, 02Eh, 056h, 058h, 044h, 000h, 064h, 073h, 06Fh, 075h
        db  06Eh, 064h, 02Eh, 076h, 078h, 064h, 000h, 000h, 000h, 000h, 000h, 000h
        dd  loc_5_069F               ; fixup
        dd  loc_5_06E7               ; fixup
        dd  loc_5_0674               ; fixup
        dd  loc_5_06F5               ; fixup
        dd  loc_5_06FA               ; fixup
        dd  loc_5_06FF               ; fixup
        dd  loc_5_0723               ; fixup
        dd  loc_5_072B               ; fixup
        dd  loc_5_0733               ; fixup
        dd  loc_5_073B               ; fixup
        dd  loc_5_0787               ; fixup
        dd  loc_5_09A8               ; fixup
        db  04Dh, 053h, 053h, 042h, 04Ch, 053h, 054h, 03Ah, 020h, 000h, 000h, 000h, 049h, 052h, 051h, 020h
        db  06Eh, 06Fh, 074h, 020h, 072h, 065h, 073h, 070h, 06Fh, 06Eh, 064h, 069h, 06Eh, 067h, 020h, 028h
        db  025h, 078h, 03Ah, 025h, 078h, 029h, 00Ah, 000h, 044h, 04Dh, 041h, 020h, 06Eh, 06Fh, 074h, 020h
        db  072h, 065h, 073h, 070h, 06Fh, 06Eh, 064h, 069h, 06Eh, 067h, 020h, 028h, 025h, 078h, 03Ah, 025h
        db  078h, 029h, 00Ah, 000h, 043h, 06Fh, 06Eh, 066h, 069h, 067h, 000h, 000h, 053h, 075h, 070h, 070h
        db  06Fh, 072h, 074h, 073h, 020h, 041h, 044h, 043h, 000h, 000h, 000h, 000h, 042h, 04Ch, 041h, 053h
        db  054h, 045h, 052h, 000h, 041h, 025h, 030h, 033h, 058h, 020h, 049h, 025h, 064h, 020h, 044h, 025h
        db  064h, 000h, 000h, 000h, 06Dh, 069h, 064h, 069h, 05Ch, 06Dh, 073h, 06Fh, 070h, 06Ch, 02Eh, 064h
        db  072h, 076h, 000h, 000h
        dd  loc_5_1278               ; fixup
        dd  loc_5_1360               ; fixup
        dd  loc_5_1283               ; fixup
        dd  loc_5_12E3               ; fixup
        dd  loc_5_13D7               ; fixup
        dd  loc_5_1278               ; fixup
        dd  loc_5_1484               ; fixup
        dd  loc_5_1407               ; fixup
        dd  loc_5_1444               ; fixup
        dd  loc_5_1510               ; fixup
        dd  loc_5_15A2               ; fixup
        db  005h, 000h, 000h, 000h
        dd  obj6:0x00F4              ; fixup
        db  004h, 000h, 000h, 000h
        dd  obj6:0x0108              ; fixup
        db  002h, 000h, 000h, 000h
        dd  obj6:0x0118              ; fixup
        db  010h, 000h, 000h, 000h
        dd  loc_5_0E38               ; fixup
        db  001h, 000h
        dd  loc_5_0E38               ; fixup
        db  002h, 000h
        dd  loc_5_0E38               ; fixup
        db  003h, 000h
        dd  loc_5_0E38               ; fixup
        db  004h, 000h
        dd  loc_5_0E38               ; fixup
        db  005h, 000h
        dd  loc_5_0E38               ; fixup
        db  006h, 000h
        dd  loc_5_0E38               ; fixup
        db  007h, 000h
        dd  loc_5_0E38               ; fixup
        db  008h, 000h
        dd  loc_5_0F34               ; fixup
        db  009h, 000h
        dd  loc_5_0F34               ; fixup
        db  00Ah, 000h
        dd  loc_5_0E38               ; fixup
        db  00Bh, 000h
        dd  loc_5_0E38               ; fixup
        db  00Ch, 000h
        dd  loc_5_0E38               ; fixup
        db  00Dh, 000h
        dd  loc_5_0E38               ; fixup
        db  00Eh, 000h
        dd  loc_5_0E38               ; fixup
        db  00Fh, 000h
        dd  loc_5_0E38               ; fixup
        db  004h, 000h, 000h, 000h
        dd  loc_5_0F34               ; fixup
        db  001h, 000h
        dd  loc_5_0F34               ; fixup
        db  002h, 000h
        dd  loc_5_0F34               ; fixup
        db  003h, 000h
        dd  loc_5_0F34               ; fixup
        db  000h, 000h, 000h, 000h
        dd  loc_1_0234               ; fixup
        db  000h, 000h, 000h, 000h
        dd  loc_1_0274               ; fixup
        dd  loc_1_0284               ; fixup
        db  000h, 000h, 000h, 000h, 0F4h, 001h, 000h, 000h, 000h, 000h, 000h, 000h, 075h, 06Eh, 061h, 062h
        db  06Ch, 065h, 020h, 074h, 06Fh, 020h, 061h, 06Ch, 06Ch, 06Fh, 063h, 061h, 074h, 065h, 020h, 044h
        db  053h, 050h, 00Ah, 000h, 075h, 06Eh, 061h, 062h, 06Ch, 065h, 020h, 074h, 06Fh, 020h, 061h, 06Ch
        db  06Ch, 06Fh, 063h, 061h, 074h, 065h, 020h, 046h, 04Dh, 00Ah, 000h, 075h, 06Eh, 061h, 062h, 06Ch
        db  065h, 020h, 074h, 06Fh, 020h, 061h, 06Ch, 06Ch, 06Fh, 063h, 061h, 074h, 065h, 020h, 049h, 052h
        db  051h, 00Ah, 000h, 075h, 06Eh, 061h, 062h, 06Ch, 065h, 020h, 074h, 06Fh, 020h, 061h, 06Ch, 06Ch
        db  06Fh, 063h, 061h, 074h, 065h, 020h, 044h, 04Dh, 041h, 00Ah, 000h, 075h, 06Eh, 061h, 062h, 06Ch
        db  065h, 020h, 074h, 06Fh, 020h, 061h, 06Ch, 06Ch, 06Fh, 063h, 061h, 074h, 065h, 020h, 062h, 075h
        db  066h, 066h, 065h, 072h, 00Ah, 000h, 000h, 000h, 087h, 083h, 081h, 082h, 000h, 08Bh, 089h, 08Ah
        db  003h, 005h, 007h, 009h, 00Ah, 0FFh
VxD__LDATA ENDS

VxD__LTEXT SEGMENT   ; object 7  vsize=0x73  R-X
loc_7_0000:
        push   0
        push   4
        VMMCall  VMM._Allocate_Device_CB_Area    ; cd20a7000100
        add    esp, 8
        or     eax, eax
        jne    loc_7_0013
loc_7_0011:
        stc    
        ret    
loc_7_0013:
        mov    dword ptr [0], eax            ; -> obj1:0x00D0
        mov    eax, 5
        mov    ecx, 4
        VMMCall  VMM.List_Create                 ; cd209b000100
        jb     loc_7_0011
        mov    dword ptr [0], esi            ; -> obj1:0x00D4
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    edi, dword ptr [0]            ; -> obj1:0x00D0
        push   ebx
loc_7_003D:
        mov    eax, 5
        mov    ecx, 8
        VMMCall  VMM.List_Create                 ; cd209b000100
        jb     loc_7_0070
        mov    dword ptr [edi + ebx], esi
        VMMCall  VMM.Get_Next_VM_Handle          ; cd203b000100
        cmp    ebx, dword ptr [esp]
        jne    loc_7_003D
        pop    ebx
        mov    eax, 0x15000e
        mov    esi, 0                        ; -> loc_1_01D4
        VMMCall  VMM.Hook_Device_Service         ; cd2090000100
        clc    
        ret    
loc_7_0070:
        pop    ebx
        jmp    loc_7_0011
VxD__LTEXT ENDS

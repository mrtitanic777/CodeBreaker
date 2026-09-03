; AUDDRV -- reconstructed 32-bit disassembly (VxD / LE format)
; 7 objects, 3367 instructions recovered by recursive descent

Declare_Virtual_Device AUDDRV, 4, 0, AUDDRV_Control, 03035h, Undefined_Init_Order
;   DDK version 0400h   init_order 080000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x378  R-X
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
        dw  03035h                      ; DDB_Req_Device_Number
        db  4, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'AUDDRV  '                  ; DDB_Name
        dd  080000000h                  ; DDB_Init_Order
        dd  AUDDRV_Control           ; DDB_Control_Proc
        dd  AUDDRV_PM_API            ; DDB_V86_API_Proc
        dd  AUDDRV_PM_API            ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_00D8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_00DC:
        inc    esp
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], ch
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [esi], al
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        add    byte ptr [eax], cl
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        db  000h
AUDDRV_Control:
        cmp    eax, 8
        je     loc_5_05A9
loc_1_0105:
        cmp    eax, 0xb
        je     loc_5_0545
loc_1_010E:
        cmp    eax, 3
        je     loc_5_05A9
loc_1_0117:
        cmp    eax, 0x1b
        je     loc_7_0000
loc_1_0120:
        cmp    eax, 0x1c
        je     loc_3_0000
loc_1_0129:
        cmp    eax, 0x22
        je     loc_4_065F
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
        cmp    dword ptr [edi + 0x2c], ebx
        jne    loc_1_0201
        or     ecx, 1
loc_1_0201:
        cmp    dword ptr [edi + 0x34], ebx
        jne    loc_1_0209
        or     ecx, 2
loc_1_0209:
        jecxz  loc_1_0214
        push   eax
        mov    eax, ecx
        call   sub_5_060A
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
        cld    
        push   edi
        push   edx
        push   ebx
        mov    edi, edx
        mov    ebx, dword ptr [edi + 0x2c]
        or     ebx, ebx
        je     loc_1_0248
        cmp    word ptr [edi + 0x64], 0
        je     loc_1_025A
loc_1_0248:
        push   eax
        movzx  edx, word ptr [edi + 6]
        add    edx, 0xe
        in     al, dx
        pop    eax
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
        jmp    loc_1_0260
loc_1_025A:
        VxDCall  VPICD.VPICD_Set_Int_Request     ; cd2002000300
loc_1_0260:
        pop    ebx
        pop    edx
        pop    edi
        clc    
        ret    
        db  02Eh, 08Bh, 0C0h
loc_1_0268:
        VxDCall  VPICD.VPICD_Clear_Int_Request   ; cd2003000300
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
        clc    
        ret    
        db  08Bh, 0C0h
loc_1_0278:
        push   edi
        push   1
        push   eax
        call   sub_1_0134
        add    esp, 8
        or     edi, edi
        je     loc_1_02A9
        cmp    dword ptr [edi + 0x2c], ebx
        jne    loc_1_029F
        jecxz  loc_1_0297
loc_1_028F:
        VxDCall  VPICD.VPICD_Physically_Mask     ; cd2008000300
        jmp    loc_1_02A9
loc_1_0297:
        VxDCall  VPICD.VPICD_Physically_Unmask   ; cd2009000300
        jmp    loc_1_02A9
loc_1_029F:
        test   word ptr [edi + 4], 0x8000
        jne    loc_1_0297
        je     loc_1_028F
loc_1_02A9:
        pop    edi
        clc    
        ret    
sub_1_02AC:
        push   ecx
        push   edx
        add    edx, 0xe
        mov    ecx, 0x5dc
loc_1_02B6:
        in     al, dx
        or     al, al
        js     loc_1_02C0
        loop   loc_1_02B6
        stc    
        jmp    loc_1_02C5
loc_1_02C0:
        sub    edx, 4
        in     al, dx
        clc    
loc_1_02C5:
        pop    edx
        pop    ecx
        ret    
sub_1_02C8:
        push   eax
        push   ecx
        push   edx
        add    edx, 0xc
        push   eax
        mov    ecx, 0xa0000
loc_1_02D4:
        in     al, dx
        or     al, al
        jns    loc_1_02DF
        loop   loc_1_02D4
        pop    eax
        stc    
        jmp    loc_1_02E2
loc_1_02DF:
        pop    eax
        out    dx, al
        clc    
loc_1_02E2:
        pop    edx
        pop    ecx
        pop    eax
        ret    
sub_1_02E6:
        push   eax
        push   edx
        add    edx, 6
        mov    al, 1
        out    dx, al
        in     al, dx
        in     al, dx
        in     al, dx
        sub    edx, 6
        jmp    loc_1_02F6
loc_1_02F6:
        jmp    loc_1_02F8
loc_1_02F8:
        add    edx, 6
        xor    al, al
        out    dx, al
        sub    edx, 6
        call   sub_1_02AC
        pop    edx
        pop    eax
        ret    
sub_1_0309:
        push   eax
        add    edx, 0xc
        in     al, dx
        sub    edx, 0xc
        or     al, al
        jns    loc_1_031C
        call   sub_1_02E6
        jb     loc_1_032C
loc_1_031C:
        mov    al, 0x10
        call   sub_1_02C8
        jb     loc_1_032C
        mov    al, 0x80
        call   sub_1_02C8
loc_1_032C:
        pop    eax
        ret    
sub_1_032E:
        pushal 
        mov    edi, dword ptr [esi + 4]
        movzx  edx, word ptr [edi + 6]
        pushfd 
        cli    
        popfd  
        clc    
        popal  
        ret    
sub_1_033C:
        pushal 
        mov    edi, dword ptr [esi + 4]
        movzx  edx, word ptr [edi + 6]
        pushfd 
        cli    
        popfd  
        clc    
        popal  
        ret    
sub_1_034A:
        push   eax
        push   ecx
        mov    ecx, 0x32
loc_1_0351:
        in     al, dx
        jmp    loc_1_0354
loc_1_0354:
        loop   loc_1_0351
        pop    ecx
        pop    eax
        ret    
sub_1_0359:
        push   ebx
        push   edx
        push   eax
        mov    eax, ebx
        shl    ah, 1
        add    dl, ah
        out    dx, al
        sub    dl, ah
        call   sub_1_034A
        pop    eax
        inc    dl
        out    dx, al
        call   sub_1_034A
        pop    edx
        pop    ebx
        ret    
        db  0CCh, 0CCh
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0xc9  R-X
        db  003h, 000h, 000h, 000h
        dd  obj2:0x0014              ; fixup
        db  000h, 001h, 000h, 000h, 000h, 000h
loc_2_000E:
        dec    esp
        db  000h
loc_2_0010:
        wait   
        add    byte ptr [ebp + 0x616e5500], dh
        bound  ebp, qword ptr [ebp + 0x20]
        je     0x8c
        and    byte ptr [eax + 0x6c], dh
        popal  
        jns    loc_2_0043
        jae    loc_2_0094
        jne    0x95
        and    byte ptr fs:[0x65687420], ch
        and    byte ptr [ebp + 0x53], al
        push   ebx
        and    byte ptr [ecx + 0x75], al
        imul   ebp, dword ptr fs:[edi + 0x44], 0x65766972
        and    byte ptr [ecx + 0x73], ch
        and    byte ptr [ecx + 0x6e], ch
loc_2_0043:
        and    byte ptr [ebp + 0x73], dh
        and    byte ptr gs:[edx + 0x79], ah
        and    byte ptr [ecx + 0x6e], ah
        outsd  dx, dword ptr [esi]
        je     0xb8
        jb     loc_2_0073
        popal  
        jo     0xc6
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [ebx + 0x61], 0x6e6f6974
        add    byte ptr cs:[ebp + 0x6e], dl
        popal  
        bound  ebp, qword ptr [ebp + 0x20]
        je     0xd8
        and    byte ptr [eax + 0x6c], dh
        popal  
        jns    0x8f
        dec    ebp
        dec    ecx
        inc    esp
        dec    ecx
loc_2_0073:
        and    byte ptr [0x65687420], ch
        and    byte ptr [ebp + 0x53], al
        push   ebx
        and    byte ptr [esi + 0x4d], al
        and    byte ptr [ebx + 0x79], dl
        outsb  dx, byte ptr [esi]
        je     0xee
        jae    0xf2
        jp     0xf0
        jb     loc_2_00AD
        imul   esi, dword ptr [ebx + 0x20], 0x75206e69
loc_2_0094:
        jae    0xfb
        and    byte ptr [edx + 0x79], ah
        and    byte ptr [ecx + 0x6e], ah
        outsd  dx, dword ptr [esi]
        je     0x107
        jb     0xc2
        popal  
        jo     0x115
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [ebx + 0x61], 0x6e6f6974
loc_2_00AD:
        add    byte ptr cs:[ebp + 0x53], al
        xor    al, 0x38
        cmp    byte ptr [eax], ah
        inc    esp
        imul   esi, dword ptr [edx + 0x65], 0x53207463
        outsd  dx, dword ptr [esi]
        jne    0x12e
        and    byte ptr fs:[edx + esi*2 + 0x69], al
        jbe    0x12c
        jb     0xc9
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x13f  R-X
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
sub_3_004C:
        push   ebp
        mov    ebp, esp
        push   eax
        push   ebx
        push   edi
        push   esi
        push   0
        push   dword ptr [ebp + 8]
        call   sub_1_0134
loc_3_005D:
        add    esp, 8
        or     edi, edi
        je     loc_3_00BD
        mov    ebx, dword ptr [edi + 0x2c]
        or     ebx, ebx
        je     loc_3_00BD
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_3_00BD
        call   sub_1_0195
loc_3_0078:
        jb     loc_3_00BD
        call   sub_1_033C
loc_3_007F:
        push   ecx
        push   edx
        push   edi
        mov    eax, dword ptr [edi + 0x3c]
        or     eax, eax
        je     loc_3_00AB
        mov    edi, esi
        VxDCall  VDMAD.VDMAD_Get_Virt_State      ; cd2004000400
        mov    word ptr [edi + 0x20], dx
        mov    dword ptr [edi + 0x18], ecx
        mov    dword ptr [edi + 0x1c], esi
        mov    esi, edi
        test   word ptr [esi + 0x20], 1
        jne    loc_3_00AB
        VxDCall  VDMAD.VDMAD_Mask_Channel        ; cd2007000400
loc_3_00AB:
        pop    edi
        movzx  edx, word ptr [edi + 6]
        call   sub_1_02E6
loc_3_00B5:
        pop    edx
        pop    ecx
        or     word ptr [edi + 4], 0x100
loc_3_00BD:
        pop    esi
        pop    edi
        pop    ebx
        pop    eax
        pop    ebp
        ret    
sub_3_00C3:
        push   ebp
        mov    ebp, esp
        push   eax
        push   ebx
        push   edi
        push   esi
        push   0
        push   dword ptr [ebp + 8]
        call   sub_1_0134
loc_3_00D4:
        add    esp, 8
        or     edi, edi
        je     loc_3_0139
        mov    ebx, dword ptr [edi + 0x2c]
        or     ebx, ebx
        je     loc_3_0139
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_3_0139
        test   word ptr [edi + 4], 0x100
        je     loc_3_0139
        call   sub_1_0195
loc_3_00F7:
        jb     loc_3_0139
        call   sub_1_032E
loc_3_00FE:
        push   ecx
        push   edx
        push   edi
        mov    eax, dword ptr [edi + 0x3c]
        or     eax, eax
        je     loc_3_0127
        mov    edi, esi
        mov    dx, word ptr [edi + 0x20]
        mov    ecx, dword ptr [edi + 0x18]
        mov    esi, dword ptr [edi + 0x1c]
        VxDCall  VDMAD.VDMAD_Set_Virt_State      ; cd2005000400
        mov    esi, edi
        test   dl, 1
        jne    loc_3_0127
        VxDCall  VDMAD.VDMAD_UnMask_Channel      ; cd2008000400
loc_3_0127:
        pop    edi
        movzx  edx, word ptr [edi + 6]
        call   sub_1_02E6
loc_3_0131:
        pop    edx
        pop    ecx
        and    word ptr [edi + 4], 0xfeff
loc_3_0139:
        pop    esi
        pop    edi
        pop    ebx
        pop    eax
        pop    ebp
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0x762  R-X
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
        cmp    dword ptr [edi + 0x2c], 0
        jne    loc_4_0028
        cmp    dword ptr [edi + 0x34], 0
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
        mov    ecx, 0x24
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
        jmp    loc_4_01C7
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
        je     loc_4_01C1
        mov    dword ptr [ebp - 8], eax
        push   edi
        movzx  ecx, word ptr [edi]
        push   ecx
        mov    edi, dword ptr [ebp - 8]
loc_4_014A:
        push   ecx
        mov    ecx, 0xa
        mov    esi, 0                        ; -> loc_4_0105
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    ecx
        loop   loc_4_014A
        pop    ecx
        mov    edi, dword ptr [esp]
        mov    esi, dword ptr [ebp - 8]
        add    edi, 2
loc_4_0165:
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [esi + 1], eax
        mov    eax, dword ptr [edi + 2]
        sub    eax, esi
        sub    eax, 0xa
        mov    dword ptr [esi + 6], eax
        movzx  edx, word ptr [edi]
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        jb     loc_4_0198
        add    edi, 6
        add    esi, 0xa
        loop   loc_4_0165
        pop    edi
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [esp + 0x1c], eax
        jmp    loc_4_01C7
loc_4_0198:
        pop    edi
        movzx  eax, word ptr [edi]
        sub    ecx, eax
        neg    ecx
        jecxz  loc_4_01B3
        add    edi, 2
loc_4_01A5:
        movzx  edx, word ptr [edi]
        VMMCall  VMM.Remove_IO_Handler           ; cd2016010100
        add    edi, 6
        loop   loc_4_01A5
loc_4_01B3:
        push   0
        push   dword ptr [ebp - 8]
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
loc_4_01C1:
        xor    eax, eax
        mov    dword ptr [esp + 0x1c], eax
loc_4_01C7:
        popal  
        add    esp, 0xc
        pop    ebp
        ret    
sub_4_01CD:
        push   ecx
        push   edx
        push   edi
        push   eax
        movzx  ecx, word ptr [edi]
        add    edi, 2
loc_4_01D7:
        movzx  edx, word ptr [edi]
        VMMCall  VMM.Remove_IO_Handler           ; cd2016010100
        add    edi, 6
        loop   loc_4_01D7
        pop    eax
        push   0
        push   eax
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
        pop    edi
        pop    edx
        pop    ecx
        ret    
sub_4_01F6:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        pushal 
        mov    dword ptr [ebp - 4], 0
        mov    esi, dword ptr [0]            ; -> obj1:0x00D4
        or     esi, esi
        je     loc_4_0426
        movzx  edx, word ptr [ebp + 0xc]
        push   2
        push   edx
        call   sub_1_0134
loc_4_021E:
        add    esp, 8
        or     edi, edi
        jne    loc_4_0426
        VMMCall  VMM.List_Allocate               ; cd209d000100
        jb     loc_4_0426
        mov    dword ptr [ebp - 4], eax
        VMMCall  VMM.List_Attach_Tail            ; cd209f000100
        mov    dword ptr [eax], 0
        push   1
        push   0x66
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        or     eax, eax
        je     loc_4_0426
        mov    edi, eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax], edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [edi + 0x44], eax
        mov    word ptr [edi + 0x64], 0
        mov    ax, word ptr [ebp + 0x28]
        mov    word ptr [edi + 4], ax
        mov    ax, word ptr [ebp + 0xc]
        mov    word ptr [edi + 6], ax
        mov    ax, word ptr [ebp + 0x10]
        mov    word ptr [edi + 0x10], ax
        mov    byte ptr [edi + 0xc], 4
        mov    ax, word ptr [ebp + 0x18]
        mov    byte ptr [edi + 0xd], al
        mov    ax, word ptr [ebp + 0x14]
        mov    word ptr [edi + 8], ax
        mov    ax, word ptr [ebp + 0x1c]
        mov    byte ptr [edi + 0xa], al
        mov    ax, word ptr [ebp + 0x20]
        mov    byte ptr [edi + 0xb], al
        call   sub_5_0720
loc_4_02AA:
        mov    word ptr [edi + 0x12], ax
        push   edi
        movzx  ecx, byte ptr [edi + 0xa]
        mov    word ptr [0], cx              ; -> obj6:0x014A
        mov    dword ptr [0x1c], edi         ; -> obj6:0x0166
        mov    edi, 0                        ; -> obj6:0x014A
        VxDCall  VPICD.VPICD_Virtualize_IRQ      ; cd2001000300
        pop    edi
        jae    loc_4_02E0
        push   0                             ; -> obj6:0x01B1
        call   sub_5_0000
loc_4_02D8:
        add    esp, 4
        jmp    loc_4_0426
loc_4_02E0:
        mov    dword ptr [edi + 0x28], eax
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        VxDCall  VPICD.VPICD_Get_Complete_Status ; cd2005000300
        test   ecx, 4
        jne    loc_4_02FD
        or     word ptr [edi + 4], 0x8000
loc_4_02FD:
        mov    esi, 0                        ; -> obj6:0x00DA
        cmp    word ptr [ebp + 0x14], -1
        jne    loc_4_030E
        mov    esi, 0                        ; -> obj6:0x0078
loc_4_030E:
        movzx  ecx, word ptr [esi]
        movzx  edx, word ptr [ebp + 0xc]
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_031B:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_031B
        mov    edx, dword ptr [esp]
        call   sub_4_0103
        pop    edi
        mov    dword ptr [edi + 0x48], eax
        or     eax, eax
        jne    loc_4_0346
        push   0                             ; -> obj6:0x016A
        call   sub_5_0000
loc_4_033E:
        add    esp, 4
        jmp    loc_4_0426
loc_4_0346:
        movsx  edx, word ptr [ebp + 0x14]
        cmp    edx, -1
        je     loc_4_0390
        mov    word ptr [edi + 8], dx
        mov    dx, 0x388
        mov    esi, 0                        ; -> obj6:0x013C
        movzx  ecx, word ptr [esi]
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_0365:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_0365
        mov    edx, dword ptr [esp]
        call   sub_4_0103
        pop    edi
        mov    dword ptr [edi + 0x4c], eax
        or     eax, eax
        jne    loc_4_0390
        push   0                             ; -> obj6:0x019A
        call   sub_5_0000
loc_4_0388:
        add    esp, 4
        jmp    loc_4_0426
loc_4_0390:
        mov    eax, dword ptr [ebp + 0x24]
        call   sub_4_0030
        mov    dword ptr [edi + 0x14], eax
        mov    dword ptr [edi + 0x18], ebx
        mov    dword ptr [edi + 0x20], ecx
        mov    dword ptr [edi + 0x1c], edx
        jae    loc_4_03B5
        push   0                             ; -> obj6:0x01FC
        call   sub_5_0000
loc_4_03B0:
        add    esp, 4
        jmp    loc_4_0426
loc_4_03B5:
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
        mov    word ptr [edi + 0x24], ax
        movzx  eax, word ptr [ebp + 0x20]
        mov    esi, 0                        ; -> loc_1_022C
        VxDCall  VDMAD.VDMAD_Virtualize_Channel  ; cd2001000400
        mov    dword ptr [edi + 0x3c], eax
        jae    loc_4_03FD
        push   0                             ; -> obj6:0x01E4
        call   sub_5_0000
loc_4_03F8:
        add    esp, 4
        jmp    loc_4_0426
loc_4_03FD:
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        push   ebx
loc_4_0404:
        call   sub_4_0087
        jb     loc_4_0425
        VMMCall  VMM.Get_Next_VM_Handle          ; cd203b000100
        cmp    ebx, dword ptr [esp]
        jne    loc_4_0404
        pop    ebx
        mov    dword ptr [esp + 0x1c], 0xffffffff
loc_4_041F:
        popal  
        add    esp, 4
        pop    ebp
        ret    
loc_4_0425:
        pop    ebx
loc_4_0426:
        mov    edi, dword ptr [ebp - 4]
        or     edi, edi
        je     loc_4_0432
        call   sub_4_043C
loc_4_0432:
        mov    dword ptr [esp + 0x1c], 0
        jmp    loc_4_041F
sub_4_043C:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        pushal 
        mov    dword ptr [ebp - 4], edi
        mov    edi, dword ptr [edi]
        or     edi, edi
        je     loc_4_0511
        xor    esi, esi
        xchg   dword ptr [edi + 0x40], esi
        or     esi, esi
        je     loc_4_045F
        VMMCall  VMM.List_Destroy                ; cd209c000100
loc_4_045F:
        mov    eax, dword ptr [edi + 0x3c]
        or     eax, eax
        je     loc_4_046C
        VxDCall  VDMAD.VDMAD_Unvirtualize_Channel ; cd201a000400
loc_4_046C:
        movzx  eax, word ptr [edi + 0x24]
        or     eax, eax
        je     loc_4_048F
        push   0
        push   eax
        VMMCall  VMM._Free_GDT_Selector          ; cd2077000100
        add    esp, 8
        mov    eax, dword ptr [edi + 0x14]
        push   0
        push   eax
        VMMCall  VMM._PageFree                   ; cd2055000100
        add    esp, 8
loc_4_048F:
        mov    eax, dword ptr [edi + 0x4c]
        or     eax, eax
        je     loc_4_04BE
        mov    esi, 0                        ; -> obj6:0x013C
        movzx  ecx, word ptr [esi]
        mov    edx, 0x388
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_04A9:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_04A9
        mov    eax, dword ptr [esp]
        mov    eax, dword ptr [eax + 0x4c]
        call   sub_4_01CD
        pop    edi
loc_4_04BE:
        mov    eax, dword ptr [edi + 0x48]
        or     eax, eax
        je     loc_4_04F8
        mov    esi, 0                        ; -> obj6:0x00DA
        cmp    word ptr [edi + 8], -1
        jne    loc_4_04D6
        mov    esi, 0                        ; -> obj6:0x0078
loc_4_04D6:
        movzx  ecx, word ptr [esi]
        movzx  edx, word ptr [edi + 6]
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_04E3:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_04E3
        mov    eax, dword ptr [esp]
        mov    eax, dword ptr [eax + 0x48]
        call   sub_4_01CD
        pop    edi
loc_4_04F8:
        mov    eax, dword ptr [edi + 0x28]
        or     eax, eax
        je     loc_4_0505
        VxDCall  VPICD.VPICD_Force_Default_Behavior ; cd2011000300
loc_4_0505:
        push   0
        push   edi
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
loc_4_0511:
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
sub_4_052E:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        pushal 
        mov    dword ptr [esp + 0x1c], 0x13
        movzx  edx, word ptr [ebp + 0xc]
        call   sub_4_0674
        jb     loc_4_059E
        mov    word ptr [ebp - 4], ax
        mov    word ptr [ebp - 8], dx
        movsx  edx, word ptr [ebp + 0x10]
        cmp    edx, -1
        je     loc_4_0560
        call   sub_4_06F7
        jb     loc_4_0598
loc_4_0560:
        test   dword ptr [ebp + 0x28], 0x50
        jne    loc_4_0569
loc_4_0569:
        push   dword ptr [ebp + 0x28]
        push   dword ptr [ebp + 0x24]
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp - 8]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp - 4]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_4_01F6
        add    esp, 0x24
        or     eax, eax
        je     loc_4_0598
        mov    dword ptr [esp + 0x1c], 0
loc_4_0598:
        popal  
        add    esp, 8
        pop    ebp
        ret    
loc_4_059E:
        mov    dword ptr [esp + 0x1c], 0x24
        jmp    loc_4_0598
sub_4_05A8:
        push   ebp
        mov    ebp, esp
        push   edi
        push   ebx
        pushfd 
        push   dword ptr [ebp + 8]
        call   0
        add    esp, 4
        or     eax, eax
        je     loc_4_05F0
        push   0
        push   dword ptr [ebp + 8]
        call   sub_1_0134
loc_4_05C7:
        add    esp, 8
        or     edi, edi
        je     loc_4_05F0
        mov    ebx, dword ptr [edi + 0x2c]
        or     ebx, ebx
        je     loc_4_05DF
        mov    eax, 1
        call   sub_5_060A
loc_4_05DF:
        mov    ebx, dword ptr [edi + 0x34]
        or     ebx, ebx
        je     loc_4_05F0
        mov    eax, 2
        call   sub_5_060A
loc_4_05F0:
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, dword ptr [0]            ; -> obj1:0x00D4
        or     esi, esi
        je     loc_4_063D
        cli    
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_4_063D
loc_4_0606:
        mov    edi, dword ptr [eax]
        cmp    dword ptr [edi + 0x44], ebx
        je     loc_4_0617
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        je     loc_4_063D
        jmp    loc_4_0606
loc_4_0617:
        popfd  
        pushfd 
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        push   ebx
loc_4_0620:
        call   sub_4_00C5
        VMMCall  VMM.Get_Next_VM_Handle          ; cd203b000100
        cmp    ebx, dword ptr [esp]
        jne    loc_4_0620
        pop    ebx
        mov    edi, eax
        call   sub_4_043C
        mov    eax, 0xffffffff
loc_4_063D:
        popfd  
        pop    ebx
        pop    edi
        pop    ebp
        ret    
loc_4_0642:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_5_0051
loc_4_0659:
        add    esp, 0x14
        leave  
        clc    
        ret    
loc_4_065F:
        mov    eax, ebx
        mov    ebx, 0                        ; -> loc_4_0642
        VxDCall  VFAT.Service_0000h              ; cd2000004a04
        mov    eax, 0
        stc    
        ret    
        db  0CCh
sub_4_0674:
        push   eax
        push   edx
        add    edx, 6
        mov    al, 1
        out    dx, al
        in     al, dx
        in     al, dx
        in     al, dx
        xor    al, al
        out    dx, al
        sub    edx, 6
        call   sub_1_02AC
loc_4_068A:
        jb     loc_4_06E4
        cmp    al, 0xaa
        jne    loc_4_06E4
        mov    al, 0xe1
        call   sub_1_02C8
loc_4_0697:
        jb     loc_4_06E4
        call   sub_1_02AC
loc_4_069E:
        jb     loc_4_06E4
        mov    ah, al
        call   sub_1_02AC
loc_4_06A7:
        jb     loc_4_06E4
        movzx  eax, ax
        mov    dword ptr [esp + 4], eax
        mov    al, 0xe7
        call   sub_1_02C8
loc_4_06B7:
        jb     loc_4_06E4
        call   sub_1_02AC
loc_4_06BE:
        jae    loc_4_06CE
        mov    al, 0xe0
        call   sub_1_02C8
loc_4_06C7:
        jb     loc_4_06E4
        call   sub_1_02AC
loc_4_06CE:
        jb     loc_4_06E4
        mov    ah, al
        call   sub_1_02AC
loc_4_06D7:
        jb     loc_4_06E4
        cmp    ah, 0x48
        jne    loc_4_06E4
        and    al, 0xf0
        cmp    al, 0x80
        je     loc_4_06ED
loc_4_06E4:
        xor    eax, eax
        mov    dword ptr [esp + 4], eax
        stc    
        jmp    loc_4_06F4
loc_4_06ED:
        movzx  eax, ah
        mov    dword ptr [esp], eax
        clc    
loc_4_06F4:
        pop    edx
        pop    eax
        ret    
sub_4_06F7:
        push   eax
        push   ebx
        push   ecx
        pushfd 
        cli    
        mov    ebx, 4
        mov    al, 0x60
        call   sub_1_0359
loc_4_0708:
        mov    ebx, 4
        mov    al, 0x80
        call   sub_1_0359
loc_4_0714:
        mov    ebx, 2
        mov    al, 0x80
        call   sub_1_0359
loc_4_0720:
        mov    ebx, 4
        mov    al, 1
        call   sub_1_0359
loc_4_072C:
        mov    cx, 0x4000
loc_4_0730:
        in     al, dx
        test   al, 0x40
        jne    loc_4_0737
        loop   loc_4_0730
loc_4_0737:
        push   eax
        mov    ebx, 4
        mov    al, 0x60
        call   sub_1_0359
loc_4_0744:
        mov    ebx, 4
        mov    al, 0x80
        call   sub_1_0359
loc_4_0750:
        pop    eax
        popfd  
        test   al, 0x40
        je     loc_4_075D
        test   al, 6
        je     loc_4_075D
        clc    
        jmp    loc_4_075E
loc_4_075D:
        stc    
loc_4_075E:
        pop    ecx
        pop    ebx
        pop    eax
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 5  vsize=0x17a9  R-X
sub_5_0000:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        VMMCall  VMM.Open_Boot_Log               ; cd2088010100
        test   eax, eax
        jne    loc_5_004D
        mov    dword ptr [ebp - 0xc], 0
        mov    dword ptr [ebp - 0x10], 0     ; -> obj6:0x0000
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
loc_5_004D:
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_0051:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0x130
        test   eax, eax
        push   ebx
        push   esi
        push   edi
        je     loc_5_036A
        cmp    eax, 1
        je     loc_5_00A5
        cmp    eax, 2
        je     loc_5_0355
        cmp    eax, 3
        je     loc_5_02C7
        cmp    eax, 4
        je     loc_5_0313
        cmp    eax, 8
        je     loc_5_031B
        cmp    eax, 0xd
        je     loc_5_0355
        mov    eax, 1
        jmp    loc_5_036C
loc_5_00A5:
        mov    eax, 0xfffe
        mov    ecx, 0xffff
        mov    word ptr [ebp - 0x38], ax
        mov    word ptr [ebp - 0x36], ax
        mov    word ptr [ebp - 0x34], ax
        mov    word ptr [ebp - 0x32], ax
        mov    word ptr [ebp - 0x30], cx
        push   0
        mov    word ptr [ebp - 0x2c], ax
        mov    esi, dword ptr [ebp + 0x10]
        mov    word ptr [ebp - 0x2a], ax
        push   esi
        mov    word ptr [ebp - 0x28], ax
        lea    eax, [ebp - 0x130]
        mov    word ptr [ebp - 0x26], cx
        push   eax
        call   sub_5_16C8
        add    esp, 0xc
        test   eax, eax
        jne    loc_5_036C
        movzx  eax, word ptr [ebp - 0xd4]
        mov    bx, 0xffff
        cmp    eax, 1
        je     loc_5_010F
        cmp    eax, 2
        je     loc_5_0130
        mov    eax, 0x13
        jmp    loc_5_036C
loc_5_010F:
        cmp    word ptr [ebp - 0xaa], 0x10
        je     loc_5_0123
        mov    eax, 0x13
        jmp    loc_5_036C
loc_5_0123:
        mov    ax, word ptr [ebp - 0xd2]
        mov    word ptr [ebp - 2], ax
        jmp    loc_5_016C
loc_5_0130:
        cmp    word ptr [ebp - 0xaa], 0x10
        je     loc_5_0144
        mov    eax, 0x13
        jmp    loc_5_036C
loc_5_0144:
        movzx  ecx, word ptr [ebp - 0xa8]
        mov    ax, word ptr [ebp - 0xd2]
        cmp    ecx, 2
        mov    word ptr [ebp - 2], ax
        je     loc_5_0165
        mov    eax, 0x13
        jmp    loc_5_036C
loc_5_0165:
        mov    bx, word ptr [ebp - 0xd0]
loc_5_016C:
        cmp    word ptr [ebp - 0x82], 1
        jb     loc_5_02BD
        cmp    word ptr [ebp - 0x72], 1
        jb     loc_5_02BD
        movzx  ax, byte ptr [ebp - 0x80]
        movzx  cx, byte ptr [ebp - 0x70]
        mov    word ptr [ebp - 4], ax
        cmp    word ptr [ebp - 2], 0xffff
        mov    word ptr [ebp - 6], cx
        jne    loc_5_01A9
        mov    eax, 0x13
        jmp    loc_5_036C
loc_5_01A9:
        cmp    bx, 0xffff
        je     loc_5_01B8
        mov    bx, word ptr [ebp - 2]
        add    bx, 8
loc_5_01B8:
        xor    edi, edi
        lea    eax, [ebp - 0x24]
        push   edi
        lea    ecx, [ebp - 0xc]
        push   edi
        mov    dword ptr [ebp - 0x24], edi
        push   eax
        push   ecx
        push   esi
        call   sub_5_16C0
        add    esp, 0x14
        test   eax, eax
        je     loc_5_01D7
        mov    dword ptr [ebp - 0xc], edi
loc_5_01D7:
        mov    eax, dword ptr [ebp - 0xc]
        cmp    eax, 8
        je     loc_5_01EB
        cmp    eax, 0x10
        je     loc_5_01F2
        cmp    eax, 0x40
        je     loc_5_01F9
        jmp    loc_5_01FE
loc_5_01EB:
        mov    edi, 0x20
        jmp    loc_5_01FE
loc_5_01F2:
        mov    edi, 0x10
        jmp    loc_5_01FE
loc_5_01F9:
        mov    edi, 0x40
loc_5_01FE:
        push   edi
        lea    eax, [ebp - 0x2c]
        push   0x20
        lea    ecx, [ebp - 0x38]
        push   eax
        push   ecx
        push   dword ptr [ebp - 6]
        push   dword ptr [ebp - 4]
        push   ebx
        push   dword ptr [ebp - 2]
        push   esi
        call   sub_4_052E
loc_5_0219:
        add    esp, 0x24
        mov    edi, eax
        test   edi, edi
        jne    loc_5_02A9
        sub    bx, 0xffff
        mov    dword ptr [ebp - 0x18], esi
        mov    dword ptr [ebp - 0x1c], 0     ; -> obj6:0x0024
        cmp    bx, 1
        sbb    eax, eax
        inc    eax
        mov    dword ptr [ebp - 0x20], eax
        push   dword ptr [ebp - 0x20]
        push   dword ptr [ebp - 0x1c]
        push   dword ptr [ebp - 0x18]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VxDCall  VFAT.Service_0001h              ; cd2001004a04
        add    esp, 0xc
        test   edi, edi
        jne    loc_5_02B6
        cmp    word ptr [ebp - 4], 9
        mov    eax, 2
        je     loc_5_026B
        movzx  eax, word ptr [ebp - 4]
loc_5_026B:
        movzx  ecx, word ptr [ebp - 6]
        movzx  edx, word ptr [ebp - 2]
        push   ecx
        push   eax
        push   edx
        lea    ecx, [ebp - 0x58]
        push   0                             ; -> obj6:0x0014
        push   ecx
        call   sub_5_16B8
        add    esp, 0x14
        lea    ecx, [ebp - 0x58]
        mov    dword ptr [ebp - 0x10], 0     ; -> obj6:0x000C
        mov    dword ptr [ebp - 0x14], ecx
        push   dword ptr [ebp - 0x14]
        push   dword ptr [ebp - 0x10]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VxDCall  VFAT.Service_0002h              ; cd2002004a04
        add    esp, 8
loc_5_02A9:
        test   edi, edi
        jne    loc_5_02B6
        push   esi
        call   sub_5_10DD
        add    esp, 4
loc_5_02B6:
        mov    eax, edi
        jmp    loc_5_036C
loc_5_02BD:
        mov    eax, 0x13
        jmp    loc_5_036C
loc_5_02C7:
        mov    eax, dword ptr [ebp + 0xc]
        test   eax, eax
        je     loc_5_02DD
        cmp    eax, 1
        je     loc_5_02F8
        mov    eax, 1
        jmp    loc_5_036C
loc_5_02DD:
        push   dword ptr [ebp + 0x10]
        call   sub_4_0000
loc_5_02E5:
        add    esp, 4
        cmp    eax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 0x13
        jmp    loc_5_036C
loc_5_02F8:
        push   dword ptr [ebp + 0x10]
        call   sub_4_0000
loc_5_0300:
        add    esp, 4
        cmp    eax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 0x13
        jmp    loc_5_036C
loc_5_0313:
        cmp    dword ptr [ebp + 0xc], 1
        jne    loc_5_0355
        jmp    loc_5_036A
loc_5_031B:
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0x7fffffff
        cmp    eax, 4
        jb     loc_5_034E
        cmp    eax, 5
        jbe    loc_5_0339
        cmp    eax, 6
        jb     loc_5_034E
        cmp    eax, 8
        jbe    loc_5_0343
        jmp    loc_5_034E
loc_5_0339:
        push   dword ptr [ebp + 0x10]
        call   sub_3_004C
loc_5_0341:
        jmp    loc_5_034B
loc_5_0343:
        push   dword ptr [ebp + 0x10]
        call   sub_3_00C3
loc_5_034B:
        add    esp, 4
loc_5_034E:
        mov    eax, 1
        jmp    loc_5_036C
loc_5_0355:
        mov    esi, dword ptr [ebp + 0x10]
        push   esi
        call   sub_5_118C
        add    esp, 4
        push   esi
        call   sub_4_05A8
loc_5_0367:
        add    esp, 4
loc_5_036A:
        xor    eax, eax
loc_5_036C:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  0CCh
loc_5_0374:
        push   eax
        mov    eax, dword ptr [esi + 0x2c]
        cmp    eax, ebx
        je     loc_5_0394
        or     eax, eax
        jne    loc_5_03A7
        push   ebx
        push   ecx
        push   edx
        mov    edi, esi
        mov    eax, 1
        call   sub_5_068A
        pop    edx
        pop    ecx
        pop    ebx
        jb     loc_5_03A7
loc_5_0394:
        pop    eax
        cmp    ecx, 4
        jbe    loc_5_03A0
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_5_03A0:
        je     loc_5_03A5
        in     al, dx
        jmp    loc_5_03A6
loc_5_03A5:
        out    dx, al
loc_5_03A6:
        ret    
loc_5_03A7:
        mov    eax, 1
        mov    edi, esi
        call   sub_5_04EF
        pop    eax
        xor    eax, eax
        dec    eax
        ret    
loc_5_03B8:
        push   eax
        mov    eax, dword ptr [esi + 0x34]
        cmp    eax, ebx
        je     loc_5_03D8
        or     eax, eax
        jne    loc_5_03EB
        push   ebx
        push   ecx
        push   edx
        mov    edi, esi
        mov    eax, 2
        call   sub_5_068A
        pop    edx
        pop    ecx
        pop    ebx
        jb     loc_5_03EB
loc_5_03D8:
        pop    eax
        cmp    ecx, 4
        jbe    loc_5_03E4
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_5_03E4:
        je     loc_5_03E9
        in     al, dx
        jmp    loc_5_03EA
loc_5_03E9:
        out    dx, al
loc_5_03EA:
        ret    
loc_5_03EB:
        mov    eax, 2
        mov    edi, esi
        call   sub_5_04EF
        pop    eax
        xor    eax, eax
        dec    eax
        ret    
sub_5_03FC:
        push   esi
        push   eax
        push   ecx
        push   edx
        mov    si, 0xf
        xor    ecx, ecx
        movzx  edx, word ptr [edi + 6]
loc_5_040A:
        cmp    ecx, 8
        je     loc_5_041A
        cmp    ecx, 9
        je     loc_5_041A
        VMMCall  VMM.Enable_Local_Trapping       ; cd2098000100
loc_5_041A:
        inc    dx
        cmp    cx, si
        je     loc_5_0425
        inc    cx
        jmp    loc_5_040A
loc_5_0425:
        pop    edx
        pop    ecx
        pop    eax
        pop    esi
        clc    
        ret    
sub_5_042B:
        push   esi
        push   eax
        push   ecx
        push   edx
        mov    si, 0xf
        xor    ecx, ecx
        movzx  edx, word ptr [edi + 6]
loc_5_0439:
        cmp    ecx, 8
        je     loc_5_0449
        cmp    ecx, 9
        je     loc_5_0449
        VMMCall  VMM.Disable_Local_Trapping      ; cd209a000100
loc_5_0449:
        inc    dx
        cmp    cx, si
        je     loc_5_0454
        inc    cx
        jmp    loc_5_0439
loc_5_0454:
        pop    edx
        pop    ecx
        pop    eax
        pop    esi
        clc    
        ret    
sub_5_045A:
        push   eax
        push   ebx
        push   edx
        movzx  edx, word ptr [edi + 8]
        mov    ebx, 4
        mov    eax, 0x60
        call   sub_1_0359
loc_5_0470:
        call   sub_1_034A
loc_5_0475:
        pop    edx
        pop    ebx
        pop    eax
        ret    
sub_5_0479:
        push   esi
        push   eax
        push   ecx
        movzx  edx, word ptr [edi + 6]
        add    dx, 8
        VMMCall  VMM.Enable_Local_Trapping       ; cd2098000100
        inc    dx
        VMMCall  VMM.Enable_Local_Trapping       ; cd2098000100
        mov    si, 1
        xor    ecx, ecx
        mov    edx, 0x388
loc_5_049D:
        VMMCall  VMM.Enable_Local_Trapping       ; cd2098000100
        inc    dx
        cmp    cx, si
        je     loc_5_04AE
        inc    cx
        jmp    loc_5_049D
loc_5_04AE:
        pop    ecx
        pop    eax
        pop    esi
        clc    
        ret    
sub_5_04B3:
        push   esi
        push   eax
        push   ecx
        push   edx
        movzx  edx, word ptr [edi + 6]
        add    dx, 8
        VMMCall  VMM.Disable_Local_Trapping      ; cd209a000100
        inc    dx
        VMMCall  VMM.Disable_Local_Trapping      ; cd209a000100
        mov    si, 1
        xor    ecx, ecx
        mov    edx, 0x388
loc_5_04D8:
        VMMCall  VMM.Disable_Local_Trapping      ; cd209a000100
        inc    dx
        cmp    cx, si
        je     loc_5_04E9
        inc    cx
        jmp    loc_5_04D8
loc_5_04E9:
        pop    edx
        pop    ecx
        pop    eax
        pop    esi
        clc    
        ret    
sub_5_04EF:
        test   word ptr [edi + 4], 1
        jne    loc_5_0544
        call   sub_1_0195
loc_5_04FC:
        jb     loc_5_0544
        cmp    eax, 2
        je     loc_5_051D
        test   dword ptr [esi], 1
        jne    loc_5_0544
        or     dword ptr [esi], 1
        movzx  ecx, word ptr [0xc]           ; -> obj2:0x000C
        add    ecx, dword ptr [4]            ; -> obj2:0x0004
        jmp    loc_5_0535
loc_5_051D:
        test   dword ptr [esi], 2
        jne    loc_5_0544
        or     dword ptr [esi], 2
        movzx  ecx, word ptr [0xe]           ; -> loc_2_000E
        add    ecx, dword ptr [4]            ; -> obj2:0x0004
loc_5_0535:
        mov    eax, 0x30
        xor    esi, esi
        xor    edi, edi
        VxDCall  SHELL.SHELL_Message             ; cd2004001700
loc_5_0544:
        ret    
loc_5_0545:
        mov    esi, dword ptr [0]            ; -> obj1:0x00D4
        or     esi, esi
        je     loc_5_0592
        cli    
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        sti    
        je     loc_5_0592
loc_5_0559:
        push   eax
        mov    edi, dword ptr [eax]
        xor    eax, eax
        cmp    dword ptr [edi + 0x2c], ebx
        jne    loc_5_056D
        or     eax, 1
        mov    dword ptr [edi + 0x30], 0xffffffff
loc_5_056D:
        cmp    dword ptr [edi + 0x34], ebx
        jne    loc_5_057C
        or     eax, 2
        mov    dword ptr [edi + 0x38], 0xffffffff
loc_5_057C:
        or     eax, eax
        je     loc_5_0585
        call   sub_5_060A
loc_5_0585:
        pop    eax
        cli    
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        sti    
        je     loc_5_0592
        jmp    loc_5_0559
loc_5_0592:
        mov    edi, dword ptr [0]            ; -> obj1:0x00D0
        xor    esi, esi
        xchg   dword ptr [edi + ebx], esi
        or     esi, esi
        je     loc_5_05A7
        VMMCall  VMM.List_Destroy                ; cd209c000100
loc_5_05A7:
        clc    
        ret    
loc_5_05A9:
        pushal 
        pushfd 
        mov    edi, dword ptr [0]            ; -> obj1:0x00D0
        mov    eax, 5
        mov    ecx, 0x24
        VMMCall  VMM.List_Create                 ; cd209b000100
        jb     loc_5_0602
        mov    dword ptr [edi + ebx], esi
        mov    esi, dword ptr [0]            ; -> obj1:0x00D4
        or     esi, esi
        je     loc_5_0606
        cli    
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        sti    
        je     loc_5_0606
loc_5_05DA:
        mov    edi, dword ptr [eax]
        call   sub_4_0087
loc_5_05E1:
        jb     loc_5_05EF
        cli    
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        sti    
        je     loc_5_0606
        jmp    loc_5_05DA
loc_5_05EF:
        mov    edi, dword ptr [0]            ; -> obj1:0x00D0
        mov    esi, dword ptr [edi + ebx]
        or     esi, esi
        je     loc_5_0602
        VMMCall  VMM.List_Destroy                ; cd209c000100
loc_5_0602:
        popfd  
        stc    
        jmp    loc_5_0608
loc_5_0606:
        popfd  
        clc    
loc_5_0608:
        popal  
        ret    
sub_5_060A:
        push   eax
        push   edx
        push   esi
        call   sub_1_0195
loc_5_0612:
        jb     loc_5_0685
        test   eax, 2
        je     loc_5_0634
        cmp    dword ptr [edi + 0x34], ebx
        jne    loc_5_0634
        call   sub_5_045A
        call   sub_5_0479
        mov    dword ptr [edi + 0x34], 0
        and    dword ptr [esi], 0xfffffffd
loc_5_0634:
        test   eax, 1
        je     loc_5_0669
        cmp    dword ptr [edi + 0x2c], ebx
        jne    loc_5_0669
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_5_0651
        mov    eax, dword ptr [edi + 0x3c]
        VxDCall  VDMAD.VDMAD_Enable_Translation  ; cd2014000400
loc_5_0651:
        call   sub_5_03FC
        movzx  edx, word ptr [edi + 6]
        call   sub_1_0309
loc_5_065F:
        mov    dword ptr [edi + 0x2c], 0
        and    dword ptr [esi], 0xfffffffe
loc_5_0669:
        mov    eax, dword ptr [edi + 0x2c]
        or     eax, eax
        jne    loc_5_0685
        mov    eax, dword ptr [edi + 0x28]
        VxDCall  VPICD.VPICD_Physically_Mask     ; cd2008000300
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
        VxDCall  VPICD.VPICD_Clear_Int_Request   ; cd2003000300
loc_5_0685:
        pop    esi
        pop    edx
        pop    eax
        clc    
        ret    
sub_5_068A:
        push   edx
        push   ecx
        push   eax
        test   eax, 2
        je     loc_5_06B3
        cmp    word ptr [edi + 8], -1
        je     loc_5_06C5
        cmp    dword ptr [edi + 0x34], 0
        je     loc_5_06A8
        cmp    dword ptr [edi + 0x34], ebx
        je     loc_5_06B3
        jmp    loc_5_06C5
loc_5_06A8:
        mov    dword ptr [edi + 0x34], ebx
        call   sub_5_04B3
        mov    dword ptr [edi + 0x38], ebx
loc_5_06B3:
        test   eax, 1
        je     loc_5_06F2
        cmp    dword ptr [edi + 0x2c], 0
        je     loc_5_06C8
        cmp    dword ptr [edi + 0x2c], ebx
        je     loc_5_071B
loc_5_06C5:
        stc    
        jmp    loc_5_071C
loc_5_06C8:
        mov    dword ptr [edi + 0x2c], ebx
        cmp    dword ptr [edi + 0x30], ebx
        je     loc_5_06D9
        movzx  edx, word ptr [edi + 6]
        call   sub_1_02E6
loc_5_06D9:
        mov    dword ptr [edi + 0x30], ebx
        call   sub_5_042B
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_5_06F2
        mov    eax, dword ptr [edi + 0x3c]
        VxDCall  VDMAD.VDMAD_Disable_Translation ; cd2013000400
loc_5_06F2:
        mov    eax, dword ptr [esp]
        test   eax, 1
        je     loc_5_071B
        mov    eax, dword ptr [edi + 0x28]
        VxDCall  VPICD.VPICD_Get_Complete_Status ; cd2005000300
        test   ecx, 0x10
        je     loc_5_0715
        VxDCall  VPICD.VPICD_Physically_Mask     ; cd2008000300
        jmp    loc_5_071B
loc_5_0715:
        VxDCall  VPICD.VPICD_Physically_Unmask   ; cd2009000300
loc_5_071B:
        clc    
loc_5_071C:
        pop    eax
        pop    ecx
        pop    edx
        ret    
sub_5_0720:
        mov    eax, 0x400
        clc    
        ret    
sub_5_0727:
        push   ebp
        mov    ebp, esp
        push   edi
        push   0
        push   dword ptr [ebp + 8]
        call   sub_1_0134
loc_5_0735:
        add    esp, 8
        xor    eax, eax
        or     edi, edi
        je     loc_5_0741
        mov    eax, dword ptr [edi + 0x60]
loc_5_0741:
        pop    edi
        pop    ebp
        ret    
sub_5_0744:
        push   ebp
        mov    ebp, esp
        push   edi
        push   0
        push   dword ptr [ebp + 8]
        call   sub_1_0134
loc_5_0752:
        add    esp, 8
        or     edi, edi
        je     loc_5_075F
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [edi + 0x60], eax
loc_5_075F:
        pop    edi
        pop    ebp
        ret    
        db  0CCh, 0CCh
AUDDRV_PM_API:
        movzx  eax, word ptr [ebp + 0x14]
        cmp    ah, 3
        jae    loc_5_078E
        movzx  ecx, ah
        shl    ecx, 3
        lea    ecx, [ecx]                    ; -> obj6:0x0060
        movzx  eax, al
        cmp    eax, dword ptr [ecx]
        jae    loc_5_078E
        mov    ecx, dword ptr [ecx + 4]
        call   dword ptr [ecx + eax*4]
        jb     loc_5_078E
        and    word ptr [ebp + 0x2c], 0xfffe
        ret    
loc_5_078E:
        or     word ptr [ebp + 0x2c], 1
        ret    
loc_5_0794:
        push   edi
        mov    eax, dword ptr [ebp + 0x18]
        push   0
        push   eax
        call   sub_1_0134
loc_5_07A0:
        add    esp, 8
        jb     loc_5_07C6
        movzx  eax, word ptr [ebp + 0x10]
        test   eax, 0xffffffff
        jne    loc_5_07C6
        mov    eax, dword ptr [edi + 0x3c]
        or     eax, eax
        je     loc_5_07C6
        push   ecx
        VxDCall  VDMAD.VDMAD_Get_Phys_Count      ; cd201c000400
        mov    word ptr [ebp + 0x1c], cx
        pop    ecx
        clc    
        jmp    loc_5_07CD
loc_5_07C6:
        mov    word ptr [ebp + 0x1c], 0xffff
        stc    
loc_5_07CD:
        pop    edi
        ret    
loc_5_07CF:
        call   sub_5_0720
        mov    word ptr [ebp + 0x1c], ax
        clc    
        ret    
loc_5_07DA:
        movzx  eax, word ptr [ebp + 0x10]
        movzx  edx, word ptr [ebp + 0x1c]
        or     edx, edx
        je     loc_5_081C
        test   eax, 1
        je     loc_5_0801
        push   2
        push   edx
        call   sub_1_0134
loc_5_07F5:
        add    esp, 8
        or     edi, edi
        je     loc_5_081C
        mov    ecx, dword ptr [edi + 0x30]
        jmp    loc_5_0824
loc_5_0801:
        test   eax, 2
        je     loc_5_081C
        push   3
        push   edx
        call   sub_1_0134
loc_5_0810:
        add    esp, 8
        or     edi, edi
        je     loc_5_081C
        mov    ecx, dword ptr [edi + 0x38]
        jmp    loc_5_0824
loc_5_081C:
        mov    word ptr [ebp + 0x1c], 1
loc_5_0822:
        stc    
        ret    
loc_5_0824:
        mov    word ptr [ebp + 0x1c], 0
        call   sub_5_068A
        jb     loc_5_0832
        ret    
loc_5_0832:
        mov    word ptr [ebp + 0x1c], 2
        jmp    loc_5_0822
loc_5_083A:
        movzx  eax, word ptr [ebp + 0x10]
        movzx  edx, word ptr [ebp + 0x1c]
        or     edx, edx
        je     loc_5_0896
        test   eax, 1
        je     loc_5_087B
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_5_0867
        push   2
        push   edx
        call   sub_1_0134
loc_5_085D:
        add    esp, 8
        or     edi, edi
        jne    loc_5_0876
        add    edx, 4
loc_5_0867:
        push   2
        push   edx
        call   sub_1_0134
loc_5_086F:
        add    esp, 8
        or     edi, edi
        je     loc_5_0896
loc_5_0876:
        mov    ecx, dword ptr [edi + 0x2c]
        jmp    loc_5_089E
loc_5_087B:
        test   eax, 2
        je     loc_5_0896
        push   3
        push   edx
        call   sub_1_0134
loc_5_088A:
        add    esp, 8
        or     edi, edi
        je     loc_5_0896
        mov    ecx, dword ptr [edi + 0x34]
        jmp    loc_5_089E
loc_5_0896:
        mov    word ptr [ebp + 0x1c], 1
loc_5_089C:
        stc    
        ret    
loc_5_089E:
        cmp    ecx, ebx
        je     loc_5_08AA
        mov    word ptr [ebp + 0x1c], 2
        jmp    loc_5_089C
loc_5_08AA:
        call   sub_5_060A
        mov    word ptr [ebp + 0x1c], 0
        clc    
        ret    
loc_5_08B7:
        pushfd 
        mov    ax, word ptr [ebp + 0x1c]
        and    ax, 0xfffe
        jne    loc_5_0925
        xor    ecx, ecx
        mov    ax, word ptr [ebp + 0x1c]
        and    ax, 1
        je     loc_5_08D1
        mov    ecx, dword ptr [ebp + 0x18]
loc_5_08D1:
        cli    
        mov    esi, dword ptr [0]            ; -> obj1:0x00D4
        or     esi, esi
        je     loc_5_0925
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_5_0925
        mov    edi, dword ptr [eax]
        jecxz  loc_5_08F9
loc_5_08E8:
        cmp    dword ptr [edi + 0x44], ecx
        je     loc_5_08F9
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        je     loc_5_0925
        mov    edi, dword ptr [eax]
        jmp    loc_5_08E8
loc_5_08F9:
        popfd  
        pushfd 
        mov    esi, edi
        xchg   edi, eax
        mov    ax, 0x3810
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   edi, eax
        cmp    edi, -1
        je     loc_5_0925
        mov    ecx, dword ptr [edi]
        or     ecx, ecx
        je     loc_5_0925
        cmp    ecx, 0x66
        ja     loc_5_0925
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    word ptr [ebp + 0x1c], 1
        popfd  
        clc    
        ret    
loc_5_0925:
        mov    word ptr [ebp + 0x1c], 0
        popfd  
        stc    
        ret    
loc_5_092E:
        movzx  edx, word ptr [ebp + 0x1c]
        or     edx, edx
        je     loc_5_0963
        push   3
        push   edx
        call   sub_1_0134
loc_5_093E:
        add    esp, 8
        or     edi, edi
        je     loc_5_0963
        mov    ecx, dword ptr [edi + 0x38]
        mov    word ptr [ebp + 0x1c], 0
        mov    eax, 2
        call   sub_5_068A
        jb     loc_5_095B
        ret    
loc_5_095B:
        mov    word ptr [ebp + 0x1c], 2
        jmp    loc_5_0969
loc_5_0963:
        mov    word ptr [ebp + 0x1c], 1
loc_5_0969:
        stc    
        ret    
loc_5_096B:
        movzx  edx, word ptr [ebp + 0x1c]
        or     edx, edx
        je     loc_5_09A3
        push   3
        push   edx
        call   sub_1_0134
loc_5_097B:
        add    esp, 8
        or     edi, edi
        je     loc_5_09A3
        mov    ecx, dword ptr [edi + 0x34]
        cmp    ecx, ebx
        je     loc_5_0991
        mov    word ptr [ebp + 0x1c], 3
        jmp    loc_5_09A9
loc_5_0991:
        mov    eax, 2
        call   sub_5_060A
        mov    word ptr [ebp + 0x1c], 0
        clc    
        ret    
loc_5_09A3:
        mov    word ptr [ebp + 0x1c], 1
loc_5_09A9:
        stc    
        ret    
loc_5_09AB:
        pushfd 
        mov    ax, word ptr [ebp + 0x1c]
        cmp    ax, 1
        jne    loc_5_0A21
        mov    ecx, dword ptr [ebp + 0x18]
        cli    
        mov    esi, dword ptr [0]            ; -> obj1:0x00D4
        or     esi, esi
        je     loc_5_0A21
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_5_0A21
loc_5_09CC:
        mov    edi, dword ptr [eax]
        cmp    dword ptr [edi + 0x44], ecx
        je     loc_5_09DD
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        je     loc_5_0A21
        jmp    loc_5_09CC
loc_5_09DD:
        popfd  
        pushfd 
        cmp    word ptr [edi + 8], -1
        je     loc_5_0A21
        mov    esi, edi
        xchg   edi, eax
        mov    ax, 0x3810
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   edi, eax
        cmp    edi, -1
        je     loc_5_0A21
        mov    ecx, dword ptr [edi]
        or     ecx, ecx
        je     loc_5_0A21
        cmp    ecx, 0x1c
        ja     loc_5_0A21
        mov    word ptr [edi + 4], 0
        mov    ax, word ptr [esi + 8]
        mov    word ptr [edi + 6], ax
        mov    eax, dword ptr [esi + 0x44]
        mov    dword ptr [edi + 0xc], eax
        mov    word ptr [ebp + 0x1c], 1
        popfd  
        clc    
        ret    
loc_5_0A21:
        mov    word ptr [ebp + 0x1c], 0
        popfd  
        stc    
        ret    
loc_5_0A2A:
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
loc_5_0A82:
        add    esp, 8
        or     edi, edi
        je     loc_5_0AAE
        lea    eax, [edi + 0x40]
        push   dword ptr [esi + 8]
        push   dword ptr [esi + 4]
        push   eax
        push   dword ptr [esi]
        call   sub_5_0BE1
        add    esp, 0x10
        or     eax, eax
        je     loc_5_0AAE
        mov    word ptr [ebp + 0x1c], ax
        shr    eax, 0x10
        mov    word ptr [ebp + 0x14], ax
        clc    
        ret    
loc_5_0AAE:
        mov    word ptr [ebp + 0x1c], 0
        mov    word ptr [ebp + 0x14], 0
        stc    
        ret    
loc_5_0ABC:
        xchg   esi, eax
        mov    ax, 0x3430
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   esi, eax
        mov    eax, dword ptr [esi]
        push   0
        push   eax
        call   sub_1_0134
loc_5_0AD2:
        add    esp, 8
        or     edi, edi
        je     loc_5_0AE8
        lea    eax, [edi + 0x40]
        push   dword ptr [esi + 4]
        push   eax
        call   sub_5_0D71
        add    esp, 8
loc_5_0AE8:
        mov    word ptr [ebp + 0x1c], 0
        clc    
        ret    
sub_5_0AF0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x6c
        mov    eax, 0xffffffff
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        test   esi, esi
        je     loc_5_0BDA
        lea    eax, [ebp - 0x6c]
        push   eax
        call   sub_5_0E06
        add    esp, 4
        call   sub_5_0DF4
        call   sub_5_0E43
        mov    edi, dword ptr [eax + 8]
        mov    ebx, dword ptr [ebp + 0xc]
        mov    eax, ebx
        shr    eax, 0x10
        movzx  ecx, ax
        push   ecx
        call   sub_5_0E35
        add    esp, 4
        movzx  ecx, bx
        push   ecx
        call   sub_5_0E35
        add    esp, 4
        mov    ebx, dword ptr [ebp + 0x10]
        mov    eax, ebx
        shr    eax, 0x10
        movzx  ecx, ax
        push   ecx
        call   sub_5_0E35
        add    esp, 4
        movzx  ecx, bx
        push   ecx
        call   sub_5_0E35
        add    esp, 4
        mov    ebx, dword ptr [ebp + 0x14]
        mov    eax, ebx
        shr    eax, 0x10
        movzx  ecx, ax
        push   ecx
        call   sub_5_0E35
        add    esp, 4
        movzx  ecx, bx
        push   ecx
        call   sub_5_0E35
        add    esp, 4
        mov    ebx, dword ptr [ebp + 0x18]
        mov    eax, ebx
        shr    eax, 0x10
        movzx  ecx, ax
        push   ecx
        call   sub_5_0E35
        add    esp, 4
        movzx  ecx, bx
        push   ecx
        call   sub_5_0E35
        movzx  ecx, si
        shr    esi, 0x10
        add    esp, 4
        movzx  eax, si
        push   ecx
        push   eax
        call   sub_5_0E26
        add    esp, 8
        call   sub_5_0E00
        movzx  esi, word ptr [edi + 0x14]
        shl    esi, 0x10
        movzx  eax, word ptr [edi + 0x1c]
        or     esi, eax
        call   sub_5_0DFA
        lea    ecx, [ebp - 0x6c]
        push   ecx
        call   sub_5_0E16
        add    esp, 4
        mov    eax, esi
loc_5_0BDA:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_0BE1:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0x10]
        push   edi
        push   ebp
        mov    ebp, dword ptr [esi]
        test   ebp, ebp
        jne    loc_5_0C07
        push   0x15
        push   4
        call   sub_5_1714
        mov    ebp, eax
        test   ebp, ebp
        jne    loc_5_0C05
        xor    eax, eax
        jmp    loc_5_0D6C
loc_5_0C05:
        mov    dword ptr [esi], ebp
loc_5_0C07:
        mov    edi, dword ptr [esp + 0x1c]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        dec    ecx
        cmp    ecx, 8
        jbe    loc_5_0C24
        mov    eax, dword ptr [esp + 0x1c]
        mov    byte ptr [eax + 8], 0
loc_5_0C24:
        push   ebp
        call   sub_5_1704
        mov    ebx, eax
        test   ebx, ebx
        je     loc_5_0D18
loc_5_0C34:
        mov    ecx, dword ptr [esp + 0x1c]
        lea    edx, [ebx + 0xc]
loc_5_0C3B:
        mov    al, byte ptr [ecx]
        cmp    al, byte ptr [edx]
        jne    loc_5_0C5B
        or     al, al
        je     loc_5_0C57
        mov    al, byte ptr [ecx + 1]
        cmp    al, byte ptr [edx + 1]
        jne    loc_5_0C5B
        add    ecx, 2
        add    edx, 2
        or     al, al
        jne    loc_5_0C3B
loc_5_0C57:
        xor    eax, eax
        jmp    loc_5_0C60
loc_5_0C5B:
        sbb    eax, eax
        sbb    eax, -1
loc_5_0C60:
        test   eax, eax
        je     loc_5_0C71
        push   ebx
        push   ebp
        call   sub_5_16F4
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_5_0C34
loc_5_0C71:
        test   ebx, ebx
        je     loc_5_0D18
        cmp    dword ptr [ebx + 4], 0
        je     loc_5_0C86
        xor    eax, eax
        jmp    loc_5_0D6C
loc_5_0C86:
        push   0
        push   dword ptr [ebx + 8]
        push   0
        push   ebx
        push   dword ptr [ebx]
        call   sub_5_0AF0
        add    esp, 0x14
        test   eax, eax
        je     loc_5_0CB3
        push   ebx
        mov    dword ptr [ebx], 0
        push   esi
        call   sub_5_0D71
        add    esp, 8
        xor    eax, eax
        jmp    loc_5_0D6C
loc_5_0CB3:
        mov    eax, dword ptr [esp + 0x20]
        push   0
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ebx + 4], ecx
        push   dword ptr [ebx + 8]
        push   0
        push   ebx
        push   ecx
        call   sub_5_0AF0
        add    esp, 0x14
        test   eax, eax
        je     loc_5_0CEA
        mov    dword ptr [ebx + 4], 0
        push   ebx
        push   esi
        call   sub_5_0D71
        add    esp, 8
        xor    eax, eax
        jmp    loc_5_0D6C
loc_5_0CEA:
        push   dword ptr [ebx + 4]
        push   dword ptr [ebx + 8]
        push   0x10000
        push   ebx
        push   dword ptr [ebx]
        call   sub_5_0AF0
        add    esp, 0x14
        push   dword ptr [ebx]
        push   dword ptr [ebx + 8]
        push   0x10000
        push   ebx
        push   dword ptr [ebx + 4]
        call   sub_5_0AF0
        add    esp, 0x14
        jmp    loc_5_0D6A
loc_5_0D18:
        push   ebp
        call   sub_5_16E0
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_5_0D28
        xor    eax, eax
        jmp    loc_5_0D6C
loc_5_0D28:
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
        call   sub_5_16D0
loc_5_0D6A:
        mov    eax, ebx
loc_5_0D6C:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_5_0D71:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x10]
        test   edi, edi
        je     loc_5_0DBD
        mov    eax, dword ptr [edi]
        test   eax, eax
        je     loc_5_0D9B
        push   0
        push   dword ptr [edi + 8]
        push   0x40000
        push   edi
        push   eax
        call   sub_5_0AF0
        add    esp, 0x14
        mov    dword ptr [edi], 0
loc_5_0D9B:
        mov    eax, dword ptr [edi + 4]
        test   eax, eax
        je     loc_5_0DBD
        push   0
        push   dword ptr [edi + 8]
        push   0x40000
        push   edi
        push   eax
        call   sub_5_0AF0
        mov    dword ptr [edi + 4], 0
        add    esp, 0x14
loc_5_0DBD:
        mov    esi, dword ptr [esp + 0xc]
        mov    eax, dword ptr [esi]
        test   eax, eax
        je     loc_5_0DEE
        push   edi
        push   eax
        call   sub_5_1748
        push   edi
        push   dword ptr [esi]
        call   sub_5_1738
        push   dword ptr [esi]
        call   sub_5_1704
        test   eax, eax
        jne    loc_5_0DE8
        push   dword ptr [esi]
        call   sub_5_1728
loc_5_0DE8:
        mov    dword ptr [esi], 0
loc_5_0DEE:
        pop    edi
        pop    esi
        ret    
        db  0CCh, 0CCh, 0CCh
sub_5_0DF4:
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083800100
sub_5_0DFA:
        VMMCall  VMM.End_Nest_Exec               ; cd2086800100
sub_5_0E00:
        VMMCall  VMM.Resume_Exec                 ; cd2085800100
sub_5_0E06:
        push   ebp
        mov    ebp, esp
        push   edi
        mov    edi, dword ptr [ebp + 8]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        pop    ebp
        ret    
sub_5_0E16:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        pop    ebp
        ret    
sub_5_0E26:
        movzx  ecx, word ptr [esp + 4]
        mov    edx, dword ptr [esp + 8]
        VMMCall  VMM.Simulate_Far_Call           ; cd2048800100
sub_5_0E35:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        pop    ebp
        ret    
sub_5_0E43:
        push   ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    eax, ebx
        pop    ebx
        ret    
loc_5_0E4E:
        mov    edx, dword ptr [esp + 0xc]
        push   esi
        push   edi
        mov    ecx, 0x10
        mov    edi, 0                        ; -> loc_1_0040
        mov    eax, dword ptr [esp + 0x10]
        mov    esi, eax
        mov    dword ptr [edx], 0
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        je     loc_5_0E83
        mov    edi, 0                        ; -> loc_1_0060
        mov    ecx, 0x10
        mov    esi, eax
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        mov    eax, 0x80004002
        jne    loc_5_0E91
loc_5_0E83:
        mov    eax, dword ptr [esp + 0xc]
        push   eax
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 4]
        xor    eax, eax
loc_5_0E91:
        pop    edi
        pop    esi
        ret    0xc
loc_5_0E96:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 0xc]
        inc    eax
        mov    dword ptr [ecx + 0xc], eax
        ret    4
loc_5_0EA4:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 0xc]
        dec    eax
        mov    dword ptr [ecx + 0xc], eax
        ret    4
loc_5_0EB2:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x10]
        push   0xff
        push   0                             ; -> obj6:0x0244
        lea    esi, [edi + 0x104]
        push   esi
        mov    dword ptr [edi], 9
        call   sub_5_175C
        add    esp, 0xc
        call   sub_5_1444
        push   0xff
        lea    ecx, [edi + 4]
        push   eax
        push   ecx
        call   sub_5_175C
        mov    edx, dword ptr [esp + 0x18]
        add    esp, 0xc
        xor    ecx, ecx
        mov    dword ptr [edi + 0x210], ecx
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [edi + 0x204], eax
        mov    dword ptr [edi + 0x20c], ecx
        mov    word ptr [edi + 0x208], 0x3035
        mov    eax, 0xb3
        cmp    byte ptr [esi], cl
        je     loc_5_0F26
loc_5_0F1B:
        movsx  ecx, byte ptr [esi]
        add    eax, ecx
        inc    esi
        cmp    byte ptr [esi], 0
        jne    loc_5_0F1B
loc_5_0F26:
        mov    ecx, 0xf7
        sub    edx, edx
        div    ecx
        mov    word ptr [edi + 0x20a], dx
        xor    eax, eax
        pop    edi
        pop    esi
        ret    8
loc_5_0F3D:
        push   esi
        push   edi
        mov    edi, 0x8878000a
        mov    esi, dword ptr [esp + 0xc]
        cmp    dword ptr [esi + 0xc], 0
        jne    loc_5_0F62
        push   dword ptr [esi + 4]
        call   sub_5_1525
        add    esp, 4
        test   eax, eax
        jne    loc_5_0F62
        xor    edi, edi
        inc    dword ptr [esi + 0xc]
loc_5_0F62:
        mov    eax, edi
        pop    edi
        pop    esi
        ret    4
loc_5_0F69:
        mov    eax, dword ptr [esp + 8]
        xor    ecx, ecx
        mov    dword ptr [eax], 5
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
        mov    dword ptr [eax + 0xc], 1
        xor    eax, eax
        ret    8
loc_5_0FB4:
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        push   ebp
        mov    edi, 0x8007000e
        mov    esi, dword ptr [esp + 0x2c]
        test   byte ptr [esp + 0x1c], 0xf0
        mov    dword ptr [esi], ebx
        je     loc_5_0FD6
        mov    eax, 0x8878001e
        jmp    loc_5_109C
loc_5_0FD6:
        mov    ebp, dword ptr [esp + 0x14]
        cmp    dword ptr [ebp + 0x10], 0
        jne    loc_5_107B
        push   0x201
        push   0x31
        call   sub_5_176C
        add    esp, 8
        mov    ebx, eax
        test   ebx, ebx
        je     loc_5_1074
        mov    dword ptr [ebx], 0            ; -> obj6:0x0260
        mov    dword ptr [ebx + 4], ebp
        push   dword ptr [ebp + 4]
        push   ebx
        call   sub_5_1567
        add    esp, 8
        mov    eax, dword ptr [ebx + 0x14]
        cmp    eax, 0x1000
        jbe    loc_5_1080
        cmp    eax, 0xffff
        jae    loc_5_1080
        cmp    dword ptr [ebx + 0xc], 0
        je     loc_5_1080
        cmp    dword ptr [ebx + 0x10], 0
        je     loc_5_1080
        cmp    dword ptr [ebx + 0x18], 0
        je     loc_5_1080
        push   dword ptr [esp + 0x24]
        push   dword ptr [esp + 0x2c]
        push   ebx
        xor    edi, edi
        call   sub_5_11F4
        mov    ecx, dword ptr [esp + 0x24]
        add    esp, 0xc
        movzx  eax, word ptr [ecx + 0xe]
        mov    dword ptr [ebx + 0x24], eax
        movzx  edx, word ptr [ecx + 2]
        mov    dword ptr [ebx + 0x28], edx
        movzx  eax, word ptr [ecx + 2]
        movzx  edx, word ptr [ecx + 0xe]
        push   eax
        push   edx
        push   dword ptr [ecx + 4]
        push   1
        movzx  eax, word ptr [ebx + 0x2e]
        push   eax
        call   sub_5_15AE
        add    esp, 0x14
        jmp    loc_5_1080
loc_5_1074:
        mov    edi, 0x8007000e
        jmp    loc_5_1080
loc_5_107B:
        mov    edi, 0x8878000a
loc_5_1080:
        test   edi, edi
        je     loc_5_1095
        test   ebx, ebx
        je     loc_5_109A
        push   0
        push   ebx
        call   sub_5_1764
        add    esp, 8
        jmp    loc_5_109A
loc_5_1095:
        mov    dword ptr [esi], ebx
        inc    dword ptr [ebp + 0x10]
loc_5_109A:
        mov    eax, edi
loc_5_109C:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    0x1c
loc_5_10A3:
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [eax], 0
        mov    eax, 0x80004001
        ret    0xc
loc_5_10B5:
        mov    eax, 0x80004005
        mov    ecx, dword ptr [esp + 4]
        mov    edx, dword ptr [ecx + 0xc]
        dec    edx
        mov    dword ptr [ecx + 0xc], edx
        jne    loc_5_10DA
        cmp    dword ptr [ecx + 0x10], 0
        jne    loc_5_10DA
        push   dword ptr [ecx + 4]
        call   sub_5_1508
        add    esp, 4
        xor    eax, eax
loc_5_10DA:
        ret    4
sub_5_10DD:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x14
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        push   0x201
        mov    dword ptr [ebp - 4], ebx
        push   0x14
        call   sub_5_176C
        add    esp, 8
        mov    esi, eax
        test   esi, esi
        je     loc_5_115C
        mov    edi, dword ptr [ebp + 8]
        push   1
        push   0                             ; -> obj6:0x0250
        lea    eax, [ebp - 4]
        push   eax
        lea    ecx, [ebp - 0x14]
        push   ecx
        mov    dword ptr [esi + 4], edi
        mov    dword ptr [esi], 0            ; -> obj6:0x0220
        call   sub_5_1788
        test   eax, eax
        jne    loc_5_115C
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
        jne    loc_5_115C
        push   esi
        mov    ebx, 1
        push   edi
        call   sub_5_0744
        add    esp, 8
loc_5_115C:
        test   ebx, ebx
        jne    loc_5_1185
        cmp    dword ptr [ebp - 4], 0
        je     loc_5_1176
        mov    eax, dword ptr [ebp - 4]
        push   dword ptr [eax + 0xb]
        mov    ax, word ptr [eax + 9]
        push   eax
        call   sub_5_1774
loc_5_1176:
        test   esi, esi
        je     loc_5_1185
        push   0
        push   esi
        call   sub_5_1764
        add    esp, 8
loc_5_1185:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_118C:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_5_0727
        add    esp, 4
        mov    esi, eax
        test   esi, esi
        je     loc_5_11EE
        push   0
        push   edi
        call   sub_5_0744
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
        call   sub_5_1774
        push   0
        push   esi
        call   sub_5_1764
        add    esp, 8
loc_5_11EE:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_11F4:
        mov    edx, dword ptr [esp + 0xc]
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 0x14]
        cmp    dword ptr [edx], eax
        jbe    loc_5_1206
        mov    dword ptr [edx], eax
loc_5_1206:
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
loc_5_1226:
        mov    eax, dword ptr [esp + 4]
        push   esi
        mov    esi, dword ptr [eax + 0x20]
        dec    esi
        mov    dword ptr [eax + 0x20], esi
        jne    loc_5_124B
        cmp    dword ptr [eax + 8], 0
        jne    loc_5_124B
        mov    ecx, dword ptr [eax + 4]
        push   0
        push   eax
        dec    dword ptr [ecx + 0x10]
        call   sub_5_1764
        add    esp, 8
loc_5_124B:
        mov    eax, esi
        pop    esi
        ret    4
loc_5_1251:
        mov    edx, dword ptr [esp + 0xc]
        push   esi
        push   edi
        mov    ecx, 0x10
        mov    edi, 0                        ; -> loc_1_0050
        mov    eax, dword ptr [esp + 0x10]
        mov    esi, eax
        mov    dword ptr [edx], 0
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        je     loc_5_1286
        mov    edi, 0                        ; -> loc_1_0060
        mov    ecx, 0x10
        mov    esi, eax
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        mov    eax, 0x80004002
        jne    loc_5_1294
loc_5_1286:
        mov    eax, dword ptr [esp + 0xc]
        push   eax
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 4]
        xor    eax, eax
loc_5_1294:
        pop    edi
        pop    esi
        ret    0xc
loc_5_1299:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 0x20]
        inc    eax
        mov    dword ptr [ecx + 0x20], eax
        ret    4
loc_5_12A7:
        mov    eax, 0x80004001
        ret    0x20
loc_5_12AF:
        mov    eax, 0x80004001
        ret    0x14
loc_5_12B7:
        mov    eax, dword ptr [esp + 8]
        test   eax, eax
        je     loc_5_1304
        cmp    word ptr [eax], 1
        jne    loc_5_1304
        mov    edx, dword ptr [eax + 4]
        cmp    edx, 0x5622
        ja     loc_5_1304
        mov    cx, word ptr [eax + 0xe]
        cmp    cx, 8
        jne    loc_5_1304
        mov    ax, word ptr [eax + 2]
        cmp    ax, 1
        jne    loc_5_1304
        movzx  eax, ax
        movzx  ecx, cx
        push   eax
        push   ecx
        mov    eax, dword ptr [esp + 0xc]
        push   edx
        movzx  ecx, word ptr [eax + 0x2e]
        push   1
        push   ecx
        call   sub_5_15AE
        add    esp, 0x14
        xor    eax, eax
        jmp    loc_5_1309
loc_5_1304:
        mov    eax, 0x88780064
loc_5_1309:
        ret    8
loc_5_130C:
        mov    eax, dword ptr [esp + 8]
        test   eax, eax
        je     loc_5_1331
        cmp    eax, 0xafc8
        jae    loc_5_1331
        mov    ecx, dword ptr [esp + 4]
        push   eax
        movzx  edx, word ptr [ecx + 0x2e]
        push   edx
        call   sub_5_15E0
        add    esp, 8
        xor    eax, eax
        jmp    loc_5_1336
loc_5_1331:
        mov    eax, 0x88780064
loc_5_1336:
        ret    8
loc_5_1339:
        mov    eax, 0x80004001
        ret    8
loc_5_1341:
        mov    eax, 0x80004001
        ret    8
loc_5_1349:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi + 8], 1
        jne    loc_5_137B
        push   dword ptr [esi + 0x18]
        call   sub_5_1452
        add    esp, 4
        inc    eax
        cmp    byte ptr [esi + 0x30], 3
        jbe    loc_5_1368
        add    eax, eax
loc_5_1368:
        mov    ecx, dword ptr [esi + 0x1c]
        mov    edx, dword ptr [esp + 0x10]
        sub    ecx, eax
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [eax], ecx
        mov    dword ptr [edx], ecx
        jmp    loc_5_138F
loc_5_137B:
        mov    eax, dword ptr [esp + 0xc]
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [eax], 0
        mov    dword ptr [ecx], 0
loc_5_138F:
        xor    eax, eax
        pop    esi
        ret    0xc
loc_5_1395:
        push   esi
        mov    eax, 0x80004005
        mov    esi, dword ptr [esp + 8]
        push   edi
        cmp    dword ptr [esi + 8], 0
        jne    loc_5_140A
        push   dword ptr [esi + 0x18]
        movzx  edi, word ptr [esi + 0x2e]
        call   sub_5_146D
        add    esp, 4
        push   dword ptr [esi + 0x1c]
        push   dword ptr [esi + 0xc]
        push   dword ptr [esi + 0x18]
        call   sub_5_1491
        add    esp, 0xc
        push   dword ptr [esi + 0x18]
        call   sub_5_14B2
        add    esp, 4
        push   dword ptr [esi + 0x18]
        call   sub_5_147B
        mov    ax, word ptr [esi + 0x1c]
        add    esp, 4
        cmp    dword ptr [esi + 0x24], 0x10
        jne    loc_5_13E9
        shr    ax, 1
loc_5_13E9:
        cmp    dword ptr [esi + 0x28], 2
        jne    loc_5_13F2
        shr    ax, 1
loc_5_13F2:
        dec    ax
        movzx  ecx, ax
        push   ecx
        push   edi
        call   sub_5_161B
        add    esp, 8
        xor    eax, eax
        mov    dword ptr [esi + 8], 1
loc_5_140A:
        pop    edi
        pop    esi
        ret    0x10
loc_5_140F:
        push   esi
        mov    eax, 0x80004005
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi + 8], 0
        je     loc_5_1440
        movzx  eax, word ptr [esi + 0x2e]
        push   eax
        call   sub_5_1666
        add    esp, 4
        push   dword ptr [esi + 0x18]
        call   sub_5_146D
        add    esp, 4
        xor    eax, eax
        mov    dword ptr [esi + 8], 0
loc_5_1440:
        pop    esi
        ret    4
sub_5_1444:
        movzx  eax, word ptr [0x10]          ; -> loc_2_0010
        add    eax, dword ptr [4]            ; -> obj2:0x0004
        ret    
sub_5_1452:
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
sub_5_146D:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        VxDCall  VDMAD.VDMAD_Phys_Mask_Channel   ; cd2018000400
        pop    ebp
        ret    
sub_5_147B:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        VxDCall  VDMAD.VDMAD_Phys_Unmask_Channel ; cd2019000400
        pop    ebx
        pop    ebp
        ret    
sub_5_1491:
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
sub_5_14B2:
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
        push   ebp
        mov    ebp, esp
        push   edx
        mov    edx, dword ptr [ebp + 8]
        mov    ah, byte ptr [ebp + 0xc]
        mov    al, byte ptr [ebp + 0x10]
        call   sub_1_02C8
loc_5_14E8:
        xor    eax, eax
        jae    loc_5_14F1
        mov    eax, 1
loc_5_14F1:
        pop    edx
        pop    ebp
        ret    
        push   ebp
        mov    ebp, esp
        push   edx
        mov    edx, dword ptr [ebp + 8]
        mov    ah, byte ptr [ebp + 0xc]
        call   sub_1_02AC
loc_5_1503:
        xor    ah, ah
        pop    edx
        pop    ebp
        ret    
sub_5_1508:
        push   ebp
        mov    ebp, esp
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        push   0
        push   eax
        call   sub_1_0134
loc_5_1518:
        add    esp, 8
        mov    word ptr [edi + 0x64], 0
        pop    ebx
        pop    edi
        pop    ebp
        ret    
sub_5_1525:
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
loc_5_153B:
        add    esp, 8
        mov    eax, dword ptr [edi + 0x2c]
        or     eax, eax
        je     loc_5_1551
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        cmp    eax, ebx
        je     loc_5_1551
        jmp    loc_5_1560
loc_5_1551:
        mov    word ptr [edi + 0x64], 1
        movzx  edx, word ptr [edi + 6]
        mov    edx, 0
loc_5_1560:
        mov    eax, edx
        pop    ebx
        pop    edx
        pop    edi
        pop    ebp
        ret    
sub_5_1567:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0xc]
        push   0
        push   eax
        call   sub_1_0134
loc_5_157A:
        add    esp, 8
        mov    ax, word ptr [edi + 6]
        mov    word ptr [esi + 0x2e], ax
        mov    ah, byte ptr [edi + 0xb]
        mov    byte ptr [esi + 0x30], ah
        mov    eax, dword ptr [edi + 0x18]
        mov    dword ptr [esi + 0xc], eax
        mov    eax, dword ptr [edi + 0x1c]
        mov    dword ptr [esi + 0x10], eax
        mov    eax, dword ptr [edi + 0x20]
        mov    dword ptr [esi + 0x14], eax
        mov    eax, dword ptr [edi + 0x3c]
        mov    dword ptr [esi + 0x18], eax
        xor    ax, ax
        mov    word ptr [esi + 0x2c], ax
        pop    esi
        pop    edi
        pop    ebp
        ret    
sub_5_15AE:
        push   ebp
        mov    ebp, esp
        push   eax
        push   edi
        and    word ptr [0], 0xfff9          ; -> obj6:0x0290
        cmp    word ptr [ebp + 0x14], 0x10
        jne    loc_5_15CA
        or     word ptr [0], 2               ; -> obj6:0x0290
loc_5_15CA:
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 8]
        call   sub_5_15E0
        add    esp, 8
        clc    
        jmp    loc_5_15DC
        db  0F9h
loc_5_15DC:
        pop    edi
        pop    eax
        pop    ebp
        ret    
sub_5_15E0:
        push   ebp
        mov    ebp, esp
        push   eax
        push   ebx
        push   edx
        push   edi
        mov    edx, dword ptr [ebp + 8]
        push   edx
        mov    eax, 0xf4240
        movzx  ebx, word ptr [ebp + 0xc]
        xor    edx, edx
        idiv   ebx
        shr    ebx, 1
        cmp    edx, ebx
        jle    loc_5_15FF
        inc    eax
loc_5_15FF:
        neg    al
        pop    edx
        push   eax
        mov    al, 0x40
        call   sub_1_02C8
loc_5_160A:
        pop    eax
        jb     loc_5_1614
        call   sub_1_02C8
loc_5_1612:
        jmp    loc_5_1615
loc_5_1614:
        stc    
loc_5_1615:
        pop    edi
        pop    edx
        pop    ebx
        pop    eax
        pop    ebp
        ret    
sub_5_161B:
        push   ebp
        mov    ebp, esp
        push   eax
        push   edx
        push   edi
        mov    edx, dword ptr [ebp + 8]
        mov    al, 0xd1
        call   sub_1_02C8
loc_5_162B:
        jb     loc_5_1660
        mov    al, 0x48
        call   sub_1_02C8
loc_5_1634:
        jb     loc_5_1660
        mov    eax, dword ptr [ebp + 0xc]
        call   sub_1_02C8
loc_5_163E:
        jb     loc_5_1660
        mov    al, ah
        call   sub_1_02C8
loc_5_1647:
        jb     loc_5_1660
        mov    al, 0x1c
        test   word ptr [0], 2               ; -> obj6:0x0290
        je     loc_5_1659
        or     eax, 1
loc_5_1659:
        call   sub_1_02C8
loc_5_165E:
        jmp    loc_5_1661
loc_5_1660:
        stc    
loc_5_1661:
        pop    edi
        pop    edx
        pop    eax
        pop    ebp
        ret    
sub_5_1666:
        push   ebp
        mov    ebp, esp
        push   eax
        push   edx
        push   edi
        mov    edx, dword ptr [ebp + 8]
        mov    al, 0xd3
        call   sub_1_02C8
loc_5_1676:
        jb     loc_5_1681
        mov    al, 0xd0
        call   sub_1_02C8
loc_5_167F:
        jmp    loc_5_1682
loc_5_1681:
        stc    
loc_5_1682:
        pop    edi
        pop    edx
        pop    eax
        pop    ebp
        ret    
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_5_161B
        add    esp, 0xc
        pop    ebp
        ret    
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_5_1666
        add    esp, 8
        pop    ebp
        ret    
        db  050h, 052h, 0F8h, 05Ah, 058h, 0C3h, 0CCh, 0CCh
sub_5_16B8:
        VMMCall  VMM._Sprintf                    ; cd2074810100
        int3   
        int3   
sub_5_16C0:
        VxDCall  CONFIGMG._CONFIGMG_Get_Bus_Info ; cd2044803300
        int3   
        int3   
sub_5_16C8:
        VxDCall  CONFIGMG._CONFIGMG_Get_Alloc_Log_Conf ; cd203b803300
        int3   
        int3   
sub_5_16D0:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        VMMCall  VMM.List_Attach_Tail            ; cd209f000100
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_5_16E0:
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
sub_5_16F4:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_5_1704:
        pop    edx
        mov    ecx, esi
        pop    esi
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        mov    esi, ecx
        jmp    edx
        db  0CCh, 0CCh
sub_5_1714:
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
sub_5_1728:
        pop    edx
        mov    ecx, esi
        pop    esi
        VMMCall  VMM.List_Destroy                ; cd209c000100
        mov    esi, ecx
        jmp    edx
        db  0CCh, 0CCh
sub_5_1738:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        VMMCall  VMM.List_Deallocate             ; cd20a2000100
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_5_1748:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        VMMCall  VMM.List_Remove                 ; cd20a1000100
        mov    esi, ecx
        sbb    eax, eax
        inc    eax
        jmp    edx
        db  0CCh, 0CCh
sub_5_175C:
        VMMCall  VMM._lstrcpyn                   ; cd207b810100
        int3   
        int3   
sub_5_1764:
        VMMCall  VMM._HeapFree                   ; cd2051800100
        int3   
        int3   
sub_5_176C:
        VMMCall  VMM._HeapAllocate               ; cd204f800100
        int3   
        int3   
sub_5_1774:
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
sub_5_1788:
        mov    eax, dword ptr [esp + 0x10]
        mov    edx, dword ptr [esp + 0xc]
        VxDCall  APM.Service_0001h               ; cd2001002700
        jb     loc_5_17A6
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [ecx], edx
        xor    eax, eax
loc_5_17A6:
        ret    0x10
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 6  vsize=0x292  RW-
        db  041h, 055h, 044h, 044h, 052h, 056h, 03Ah, 020h, 000h, 000h, 000h, 000h, 042h, 04Ch, 041h, 053h
        db  054h, 045h, 052h, 000h, 041h, 025h, 030h, 033h, 058h, 020h, 049h, 025h, 064h, 020h, 044h, 025h
        db  064h, 000h, 000h, 000h, 06Dh, 069h, 064h, 069h, 05Ch, 065h, 073h, 073h, 066h, 06Dh, 02Eh, 064h
        db  072h, 076h, 000h, 000h
        dd  loc_5_07CF               ; fixup
        dd  loc_5_08B7               ; fixup
        dd  loc_5_07DA               ; fixup
        dd  loc_5_083A               ; fixup
        dd  loc_5_0794               ; fixup
        dd  loc_5_07CF               ; fixup
        dd  loc_5_09AB               ; fixup
        dd  loc_5_092E               ; fixup
        dd  loc_5_096B               ; fixup
        dd  loc_5_0A2A               ; fixup
        dd  loc_5_0ABC               ; fixup
        db  005h, 000h, 000h, 000h
        dd  obj6:0x0034              ; fixup
        db  004h, 000h, 000h, 000h
        dd  obj6:0x0048              ; fixup
        db  002h, 000h, 000h, 000h
        dd  obj6:0x0058              ; fixup
        db  010h, 000h, 000h, 000h
        dd  loc_5_0374               ; fixup
        db  001h, 000h
        dd  loc_5_0374               ; fixup
        db  002h, 000h
        dd  loc_5_0374               ; fixup
        db  003h, 000h
        dd  loc_5_0374               ; fixup
        db  004h, 000h
        dd  loc_5_0374               ; fixup
        db  005h, 000h
        dd  loc_5_0374               ; fixup
        db  006h, 000h
        dd  loc_5_0374               ; fixup
        db  007h, 000h
        dd  loc_5_0374               ; fixup
        db  008h, 000h
        dd  loc_5_0374               ; fixup
        db  009h, 000h
        dd  loc_5_0374               ; fixup
        db  00Ah, 000h
        dd  loc_5_0374               ; fixup
        db  00Bh, 000h
        dd  loc_5_0374               ; fixup
        db  00Ch, 000h
        dd  loc_5_0374               ; fixup
        db  00Dh, 000h
        dd  loc_5_0374               ; fixup
        db  00Eh, 000h
        dd  loc_5_0374               ; fixup
        db  00Fh, 000h
        dd  loc_5_0374               ; fixup
        db  010h, 000h, 000h, 000h
        dd  loc_5_0374               ; fixup
        db  001h, 000h
        dd  loc_5_0374               ; fixup
        db  002h, 000h
        dd  loc_5_0374               ; fixup
        db  003h, 000h
        dd  loc_5_0374               ; fixup
        db  004h, 000h
        dd  loc_5_0374               ; fixup
        db  005h, 000h
        dd  loc_5_0374               ; fixup
        db  006h, 000h
        dd  loc_5_0374               ; fixup
        db  007h, 000h
        dd  loc_5_0374               ; fixup
        db  008h, 000h
        dd  loc_5_03B8               ; fixup
        db  009h, 000h
        dd  loc_5_03B8               ; fixup
        db  00Ah, 000h
        dd  loc_5_0374               ; fixup
        db  00Bh, 000h
        dd  loc_5_0374               ; fixup
        db  00Ch, 000h
        dd  loc_5_0374               ; fixup
        db  00Dh, 000h
        dd  loc_5_0374               ; fixup
        db  00Eh, 000h
        dd  loc_5_0374               ; fixup
        db  00Fh, 000h
        dd  loc_5_0374               ; fixup
        db  002h, 000h, 000h, 000h
        dd  loc_5_03B8               ; fixup
        db  001h, 000h
        dd  loc_5_03B8               ; fixup
        db  000h, 000h, 004h, 000h
        dd  loc_1_0234               ; fixup
        db  000h, 000h, 000h, 000h
        dd  loc_1_0268               ; fixup
        dd  loc_1_0278               ; fixup
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 075h, 06Eh, 061h, 062h
        db  06Ch, 065h, 020h, 074h, 06Fh, 020h, 061h, 06Ch, 06Ch, 06Fh, 063h, 061h, 074h, 065h, 020h, 044h
        db  053h, 050h, 00Ah, 000h, 075h, 06Eh, 061h, 062h, 06Ch, 065h, 020h, 074h, 06Fh, 020h, 061h, 06Ch
        db  06Ch, 06Fh, 063h, 061h, 074h, 065h, 020h, 04Dh, 050h, 055h, 00Ah, 000h, 075h, 06Eh, 061h, 062h
        db  06Ch, 065h, 020h, 074h, 06Fh, 020h, 061h, 06Ch, 06Ch, 06Fh, 063h, 061h, 074h, 065h, 020h, 046h
        db  04Dh, 00Ah, 000h, 075h, 06Eh, 061h, 062h, 06Ch, 065h, 020h, 074h, 06Fh, 020h, 061h, 06Ch, 06Ch
        db  06Fh, 063h, 061h, 074h, 065h, 020h, 049h, 052h, 051h, 00Ah, 000h, 075h, 06Eh, 061h, 062h, 06Ch
        db  065h, 020h, 074h, 06Fh, 020h, 061h, 06Ch, 06Ch, 06Fh, 063h, 061h, 074h, 065h, 020h, 04Dh, 050h
        db  055h, 049h, 052h, 051h, 00Ah, 000h, 075h, 06Eh, 061h, 062h, 06Ch, 065h, 020h, 074h, 06Fh, 020h
        db  061h, 06Ch, 06Ch, 06Fh, 063h, 061h, 074h, 065h, 020h, 044h, 04Dh, 041h, 00Ah, 000h, 075h, 06Eh
        db  061h, 062h, 06Ch, 065h, 020h, 074h, 06Fh, 020h, 061h, 06Ch, 06Ch, 06Fh, 063h, 061h, 074h, 065h
        db  020h, 062h, 075h, 066h, 066h, 065h, 072h, 00Ah, 000h, 000h, 087h, 083h, 081h, 082h, 000h, 08Bh
        db  089h, 08Ah
        dd  loc_5_0E4E               ; fixup
        dd  loc_5_0E96               ; fixup
        dd  loc_5_0EA4               ; fixup
        dd  loc_5_0EB2               ; fixup
        dd  loc_5_0F3D               ; fixup
        dd  loc_5_10B5               ; fixup
        dd  loc_5_0F69               ; fixup
        dd  loc_5_0FB4               ; fixup
        dd  loc_5_10A3               ; fixup
        db  045h, 053h, 034h, 038h, 038h, 02Eh, 056h, 058h, 044h, 000h, 000h, 000h, 064h, 073h, 06Fh, 075h
        db  06Eh, 064h, 02Eh, 076h, 078h, 064h, 000h, 000h, 000h, 000h, 000h, 000h
        dd  loc_5_1251               ; fixup
        dd  loc_5_1299               ; fixup
        dd  loc_5_1226               ; fixup
        dd  loc_5_12A7               ; fixup
        dd  loc_5_12AF               ; fixup
        dd  loc_5_12B7               ; fixup
        dd  loc_5_130C               ; fixup
        dd  loc_5_1339               ; fixup
        dd  loc_5_1341               ; fixup
        dd  loc_5_1349               ; fixup
        dd  loc_5_1395               ; fixup
        dd  loc_5_140F               ; fixup
        db  006h, 000h
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
        mov    ecx, 0x24
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

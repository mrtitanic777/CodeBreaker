; msmpu401 -- reconstructed 32-bit disassembly (VxD / LE format)
; 7 objects, 1190 instructions recovered by recursive descent

Declare_Virtual_Device msmpu401, 4, 0, msmpu401_Control, 0044Ch, Undefined_Init_Order
;   DDK version 0400h   init_order 080000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x1f8  R-X
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  0044Ch                      ; DDB_Req_Device_Number
        db  4, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'msmpu401'                  ; DDB_Name
        dd  080000000h                  ; DDB_Init_Order
        dd  msmpu401_Control         ; DDB_Control_Proc
        dd  msmpu401_PM_API          ; DDB_V86_API_Proc
        dd  msmpu401_PM_API          ; DDB_PM_API_Proc
        dd  0                        ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 000h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
loc_1_0058:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_005C:
        adc    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_0060:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        adc    al, 0
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   es
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
        db  000h
msmpu401_Control:
        cmp    eax, 8
        je     loc_5_01D9
loc_1_007D:
        cmp    eax, 0xb
        je     loc_5_0189
loc_1_0086:
        cmp    eax, 3
        je     loc_5_01D9
loc_1_008F:
        cmp    eax, 0x1b
        je     loc_7_0000
loc_1_0098:
        cmp    eax, 0x1c
        je     loc_3_0000
loc_1_00A1:
        cmp    eax, 0x22
        je     loc_4_0440
loc_1_00AA:
        clc    
        ret    
sub_1_00AC:
        push   ebp
        mov    ebp, esp
        push   eax
        push   ebx
        push   edx
        push   esi
        pushfd 
        mov    eax, dword ptr [ebp + 0xc]
        shl    eax, 3
        mov    edx, dword ptr [eax]          ; -> loc_1_005C
        mov    eax, dword ptr [eax + 4]      ; -> loc_1_0060
        mov    dword ptr [ebp + 0xc], eax
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, dword ptr [0]            ; -> obj1:0x0054
        or     esi, esi
        je     loc_1_0103
        cli    
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_1_0103
loc_1_00DF:
        mov    edi, dword ptr [eax]
        test   dword ptr [ebp + 0xc], 1
        jne    loc_1_00F3
        cmp    dword ptr [edx + edi], ebx
        jne    loc_1_00F9
loc_1_00EF:
        popfd  
        clc    
        jmp    loc_1_0107
loc_1_00F3:
        cmp    word ptr [edx + edi], bx
        je     loc_1_00EF
loc_1_00F9:
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        je     loc_1_0103
        jmp    loc_1_00DF
loc_1_0103:
        popfd  
        xor    edi, edi
        stc    
loc_1_0107:
        pop    esi
        pop    edx
        pop    ebx
        pop    eax
        pop    ebp
        ret    
sub_1_010D:
        push   eax
        push   ebx
        pushfd 
        add    ebx, dword ptr [0]            ; -> obj1:0x0050
        mov    esi, dword ptr [ebx]
        or     esi, esi
        je     loc_1_013A
        cli    
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_1_013A
loc_1_0125:
        cmp    dword ptr [eax + 4], edi
        jne    loc_1_0130
        mov    esi, eax
        popfd  
        clc    
        jmp    loc_1_013E
loc_1_0130:
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        je     loc_1_013A
        jmp    loc_1_0125
loc_1_013A:
        popfd  
        xor    esi, esi
        stc    
loc_1_013E:
        pop    ebx
        pop    eax
        ret    
        db  02Eh, 08Bh, 0C0h, 0EBh, 006h, 0FFh, 025h
        dd  loc_1_0058               ; fixup
loc_1_014C:
        pushal 
        pushfd 
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_1_0188
        mov    esi, dword ptr [0]            ; -> obj1:0x0054
        or     esi, esi
        je     loc_1_0188
        cli    
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        popfd  
        pushfd 
        or     eax, eax
        je     loc_1_0188
loc_1_016D:
        mov    edi, dword ptr [eax]
        cmp    dword ptr [edi + 0x18], ebx
        jne    loc_1_0179
        call   sub_5_023A
loc_1_0179:
        cli    
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        popfd  
        pushfd 
        or     eax, eax
        je     loc_1_0188
        jmp    loc_1_016D
loc_1_0188:
        popfd  
        popal  
        jmp    loc_1_0058
loc_1_0190:
        push   eax
        push   edi
        push   1
        push   eax
        call   sub_1_00AC
        add    esp, 8
        or     edi, edi
        je     loc_1_01AA
        mov    eax, dword ptr [edi + 0x18]
        or     eax, eax
        je     loc_1_01AA
        mov    ebx, eax
loc_1_01AA:
        pop    edi
        pop    eax
        VxDCall  VPICD.VPICD_Set_Int_Request     ; cd2002000300
        clc    
        ret    
loc_1_01B4:
        push   ecx
        VxDCall  VPICD.VPICD_Clear_Int_Request   ; cd2003000300
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
        pop    ecx
        clc    
        ret    
loc_1_01C4:
        push   edi
        push   1
        push   eax
        call   sub_1_00AC
        add    esp, 8
        or     edi, edi
        je     loc_1_01EB
        cmp    dword ptr [edi + 0x18], ebx
        jne    loc_1_01EB
        jecxz  loc_1_01E3
loc_1_01DB:
        VxDCall  VPICD.VPICD_Physically_Mask     ; cd2008000300
        jmp    loc_1_01F5
loc_1_01E3:
        VxDCall  VPICD.VPICD_Physically_Unmask   ; cd2009000300
        jmp    loc_1_01F5
loc_1_01EB:
        test   word ptr [edi + 0xc], 0x8000
        jne    loc_1_01E3
        je     loc_1_01DB
loc_1_01F5:
        pop    edi
        clc    
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x5b  R-X
        db  001h, 000h, 000h, 000h
        dd  loc_2_0010               ; fixup
        db  000h, 001h, 000h, 000h, 000h, 000h, 04Bh, 000h
loc_2_0010:
        push   ebp
        outsb  dx, byte ptr [esi]
        popal  
        bound  ebp, qword ptr [ebp + 0x20]
        je     0x88
        and    byte ptr [ecx + 0x63], ah
        arpl   word ptr [ebp + 0x73], sp
        jae    0x41
        dec    ebp
        dec    ecx
        inc    esp
        dec    ecx
        and    byte ptr [eax + 0x6f], dh
        jb     0x9e
        and    byte ptr [0x65687420], ch
        and    byte ptr [ebp + 0x50], cl
        push   ebp
        sub    eax, 0x20313034
        imul   esi, dword ptr [ebx + 0x20], 0x75206e69
        jae    0xa7
        and    byte ptr [edx + 0x79], ah
        and    byte ptr [ecx + 0x6e], ah
        outsd  dx, dword ptr [esi]
        je     0xb3
        jb     0x6e
        popal  
        jo     0xc1
        insb   byte ptr es:[edi], dx
        imul   esp, dword ptr [ebx + 0x61], 0x6e6f6974
        db  02Eh, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x4c  R-X
loc_3_0000:
        mov    eax, 0x15000e
        mov    esi, 0                        ; -> loc_1_014C
        VMMCall  VMM.Unhook_Device_Service       ; cd201c010100
        xor    esi, esi
        xchg   dword ptr [0], esi            ; -> obj1:0x0054
        or     esi, esi
        je     loc_3_004A
        VMMCall  VMM.List_Destroy                ; cd209c000100
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    edi, dword ptr [0]            ; -> obj1:0x0050
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

VxD__LTEXT SEGMENT   ; object 4  vsize=0x5b0  R-X
sub_4_0000:
        push   ebp
        mov    ebp, esp
        push   edi
        push   0
        push   dword ptr [ebp + 8]
        call   sub_1_00AC
loc_4_000E:
        add    esp, 8
        or     edi, edi
        je     loc_4_001B
        cmp    dword ptr [edi + 0x18], 0
        jne    loc_4_0022
loc_4_001B:
        mov    eax, 0
        jmp    loc_4_0027
loc_4_0022:
        mov    eax, 0xffffffff
loc_4_0027:
        pop    edi
        pop    ebp
        ret    
sub_4_002A:
        push   eax
        push   ecx
        push   esi
        push   edi
        mov    edi, dword ptr [0]            ; -> obj1:0x0050
        mov    esi, dword ptr [edi + ebx]
        or     esi, esi
        je     loc_4_0069
        cli    
        VMMCall  VMM.List_Allocate               ; cd209d000100
        sti    
        jb     loc_4_0069
        push   eax
        mov    edi, eax
        xor    eax, eax
        mov    ecx, 8
        shr    ecx, 2
        rep stosd dword ptr es:[edi], eax
        pop    eax
        mov    edi, dword ptr [esp]
        mov    dword ptr [eax + 4], edi
        cli    
        VMMCall  VMM.List_Attach_Tail            ; cd209f000100
        sti    
        clc    
loc_4_0064:
        pop    edi
        pop    esi
        pop    ecx
        pop    eax
        ret    
loc_4_0069:
        stc    
        jmp    loc_4_0064
sub_4_006C:
        push   eax
        push   ecx
        push   esi
        push   edi
        pushfd 
        mov    esi, dword ptr [0]            ; -> obj1:0x0050
        mov    esi, dword ptr [esi + ebx]
        or     esi, esi
        je     loc_4_00A4
        cli    
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_4_00A4
loc_4_0087:
        cmp    dword ptr [eax + 4], edi
        je     loc_4_0096
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        je     loc_4_00A4
        jmp    loc_4_0087
loc_4_0096:
        push   eax
        VMMCall  VMM.List_Remove                 ; cd20a1000100
        pop    eax
        VMMCall  VMM.List_Deallocate             ; cd20a2000100
loc_4_00A4:
        popfd  
        pop    edi
        pop    esi
        pop    ecx
        pop    eax
        ret    
sub_4_00AA:
        jmp    loc_4_00B6
loc_4_00AC:
        mov    esi, 0
        jmp    loc_4_016F
loc_4_00B6:
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
        je     loc_4_0169
        mov    dword ptr [ebp - 8], eax
        push   edi
        movzx  ecx, word ptr [edi]
        push   ecx
        mov    edi, dword ptr [ebp - 8]
loc_4_00F1:
        push   ecx
        mov    ecx, 0xa
        lea    esi, [0]                      ; -> loc_4_00AC
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        pop    ecx
        loop   loc_4_00F1
        pop    ecx
        mov    edi, dword ptr [esp]
        mov    esi, dword ptr [ebp - 8]
        add    edi, 2
loc_4_010D:
        mov    eax, dword ptr [ebp - 0xc]
        mov    dword ptr [esi + 1], eax
        mov    eax, dword ptr [edi + 2]
        sub    eax, esi
        sub    eax, 0xa
        mov    dword ptr [esi + 6], eax
        movzx  edx, word ptr [edi]
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        jb     loc_4_0140
        add    edi, 6
        add    esi, 0xa
        loop   loc_4_010D
        pop    edi
        mov    eax, dword ptr [ebp - 8]
        mov    dword ptr [esp + 0x1c], eax
        jmp    loc_4_016F
loc_4_0140:
        pop    edi
        movzx  eax, word ptr [edi]
        sub    ecx, eax
        neg    ecx
        jecxz  loc_4_015B
        add    edi, 2
loc_4_014D:
        movzx  edx, word ptr [edi]
        VMMCall  VMM.Remove_IO_Handler           ; cd2016010100
        add    edi, 6
        loop   loc_4_014D
loc_4_015B:
        push   0
        push   dword ptr [ebp - 8]
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
loc_4_0169:
        xor    eax, eax
        mov    dword ptr [esp + 0x1c], eax
loc_4_016F:
        popal  
        add    esp, 0xc
        pop    ebp
        ret    
sub_4_0175:
        push   ecx
        push   edx
        push   edi
        push   eax
        movzx  ecx, word ptr [edi]
        add    edi, 2
loc_4_017F:
        movzx  edx, word ptr [edi]
        VMMCall  VMM.Remove_IO_Handler           ; cd2016010100
        add    edi, 6
        loop   loc_4_017F
        pop    eax
        push   0
        push   eax
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
        pop    edi
        pop    edx
        pop    ecx
        ret    
sub_4_019E:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        pushal 
        mov    dword ptr [ebp - 4], 0
        mov    esi, dword ptr [0]            ; -> obj1:0x0054
        or     esi, esi
        je     loc_4_02B8
        movzx  edx, word ptr [ebp + 0xc]
        push   2
        push   edx
        call   sub_1_00AC
loc_4_01C6:
        add    esp, 8
        or     edi, edi
        jne    loc_4_02B8
        VMMCall  VMM.List_Allocate               ; cd209d000100
        jb     loc_4_02B8
        mov    dword ptr [ebp - 4], eax
        VMMCall  VMM.List_Attach_Tail            ; cd209f000100
        mov    dword ptr [eax], 0
        push   1
        push   0x24
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        or     eax, eax
        je     loc_4_02B8
        mov    edi, eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax], edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [edi + 0x10], eax
        mov    ax, word ptr [ebp + 0xc]
        mov    word ptr [edi + 6], ax
        mov    ax, word ptr [ebp + 0x10]
        mov    byte ptr [edi + 8], al
        call   sub_5_02C3
loc_4_0222:
        mov    word ptr [edi + 0xa], ax
        xor    eax, eax
        movsx  ecx, byte ptr [edi + 8]
        cmp    ecx, -1
        je     loc_4_0264
        push   edi
        mov    word ptr [0], cx              ; -> obj6:0x003E
        mov    edi, 0                        ; -> obj6:0x003E
        VxDCall  VPICD.VPICD_Virtualize_IRQ      ; cd2001000300
        pop    edi
        jb     loc_4_02B8
        mov    dword ptr [edi + 0x14], eax
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        VxDCall  VPICD.VPICD_Get_Complete_Status ; cd2005000300
        test   ecx, 4
        jne    loc_4_0264
        or     word ptr [edi + 0xc], 0x8000
loc_4_0264:
        mov    esi, 0                        ; -> obj6:0x0030
        movzx  ecx, word ptr [esi]
        movzx  edx, word ptr [ebp + 0xc]
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_0276:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_0276
        mov    edx, dword ptr [esp]
        call   sub_4_00AA
        pop    edi
        mov    dword ptr [edi + 0x20], eax
        or     eax, eax
        je     loc_4_02B8
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        push   ebx
loc_4_0296:
        call   sub_4_002A
        jb     loc_4_02B7
        VMMCall  VMM.Get_Next_VM_Handle          ; cd203b000100
        cmp    ebx, dword ptr [esp]
        jne    loc_4_0296
        pop    ebx
        mov    dword ptr [esp + 0x1c], 0xffffffff
loc_4_02B1:
        popal  
        add    esp, 4
        pop    ebp
        ret    
loc_4_02B7:
        pop    ebx
loc_4_02B8:
        mov    edi, dword ptr [ebp - 4]
        or     edi, edi
        je     loc_4_02C4
        call   sub_4_02CE
loc_4_02C4:
        mov    dword ptr [esp + 0x1c], 0
        jmp    loc_4_02B1
sub_4_02CE:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        pushal 
        mov    dword ptr [ebp - 4], edi
        mov    edi, dword ptr [edi]
        or     edi, edi
        je     loc_4_0325
        mov    eax, dword ptr [edi + 0x20]
        or     eax, eax
        je     loc_4_030C
        mov    esi, 0                        ; -> obj6:0x0030
        movzx  ecx, word ptr [esi]
        movzx  edx, word ptr [edi + 6]
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_02F7:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_02F7
        mov    eax, dword ptr [esp]
        mov    eax, dword ptr [eax + 0x20]
        call   sub_4_0175
        pop    edi
loc_4_030C:
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_4_0319
        VxDCall  VPICD.VPICD_Force_Default_Behavior ; cd2011000300
loc_4_0319:
        push   0
        push   edi
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
loc_4_0325:
        mov    esi, dword ptr [0]            ; -> obj1:0x0054
        mov    eax, dword ptr [ebp - 4]
        push   eax
        VMMCall  VMM.List_Remove                 ; cd20a1000100
        pop    eax
        VMMCall  VMM.List_Deallocate             ; cd20a2000100
        popal  
        add    esp, 4
        pop    ebp
        ret    
sub_4_0342:
        push   ebp
        mov    ebp, esp
        pushal 
        mov    dword ptr [esp + 0x1c], 0x13
        movsx  edx, word ptr [ebp + 0xc]
        call   sub_4_0454
        jb     loc_4_0393
        movsx  eax, word ptr [ebp + 0x10]
        or     eax, eax
        js     loc_4_0373
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        call   sub_4_053A
        add    esp, 8
        or     eax, eax
        je     loc_4_0390
loc_4_0373:
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_4_019E
        add    esp, 0xc
        or     eax, eax
        je     loc_4_0390
        mov    dword ptr [esp + 0x1c], 0
loc_4_0390:
        popal  
        pop    ebp
        ret    
loc_4_0393:
        mov    dword ptr [esp + 0x1c], 0x24
        jmp    loc_4_0390
sub_4_039D:
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
        je     loc_4_03D0
        push   0
        push   dword ptr [ebp + 8]
        call   sub_1_00AC
loc_4_03BD:
        add    esp, 8
        or     edi, edi
        je     loc_4_03D0
        mov    ebx, dword ptr [edi + 0x18]
        or     ebx, ebx
        je     loc_4_03D0
        call   sub_5_023A
loc_4_03D0:
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, dword ptr [0]            ; -> obj1:0x0054
        or     esi, esi
        je     loc_4_041D
        cli    
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_4_041D
loc_4_03E6:
        mov    edi, dword ptr [eax]
        cmp    dword ptr [edi + 0x10], ebx
        je     loc_4_03F7
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        je     loc_4_041D
        jmp    loc_4_03E6
loc_4_03F7:
        popfd  
        pushfd 
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        push   ebx
loc_4_0400:
        call   sub_4_006C
        VMMCall  VMM.Get_Next_VM_Handle          ; cd203b000100
        cmp    ebx, dword ptr [esp]
        jne    loc_4_0400
        pop    ebx
        mov    edi, eax
        call   sub_4_02CE
        mov    eax, 0xffffffff
loc_4_041D:
        popfd  
        pop    ebx
        pop    edi
        pop    esi
        pop    ebp
        ret    
loc_4_0423:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_5_0000
loc_4_043A:
        add    esp, 0x14
        leave  
        clc    
        ret    
loc_4_0440:
        mov    eax, ebx
        mov    ebx, 0                        ; -> loc_4_0423
        VxDCall  VFAT.Service_0000h              ; cd2000004a04
        mov    eax, 0
        stc    
        ret    
sub_4_0454:
        enter  8, 0
        push   eax
        push   ecx
        push   edx
        mov    dword ptr [ebp - 8], 0
loc_4_0462:
        mov    dword ptr [ebp - 4], 0
        mov    ecx, 0x40
loc_4_046E:
        inc    edx
        in     al, dx
        dec    edx
        test   al, 0x80
        jne    loc_4_0478
        in     al, dx
        loop   loc_4_046E
loc_4_0478:
        mov    ecx, 0x6000
loc_4_047D:
        inc    edx
        in     al, dx
        dec    edx
        test   al, 0x40
        je     loc_4_049A
        loop   loc_4_047D
        cmp    dword ptr [ebp - 4], 0
        jne    loc_4_04C4
        mov    dword ptr [ebp - 4], 0xffffffff
        inc    edx
        mov    al, 0xff
        out    dx, al
        dec    edx
        jmp    loc_4_0478
loc_4_049A:
        mov    al, 0xff
        inc    edx
        out    dx, al
        dec    edx
        mov    ecx, 0x6000
loc_4_04A4:
        inc    edx
        in     al, dx
        dec    edx
        test   al, 0x80
        je     loc_4_04BC
        loop   loc_4_04A4
        cmp    dword ptr [ebp - 8], 0
        jne    loc_4_04C4
        mov    dword ptr [ebp - 8], 0xffffffff
        jmp    loc_4_0462
loc_4_04BC:
        in     al, dx
        cmp    al, 0xfe
        jne    loc_4_04C4
        clc    
        jmp    loc_4_04C5
loc_4_04C4:
        stc    
loc_4_04C5:
        pop    edx
        pop    ecx
        pop    eax
        leave  
        ret    
sub_4_04CA:
        push   ebp
        mov    ebp, esp
        mov    ecx, 0x6000
        movzx  edx, word ptr [ebp + 8]
        in     al, dx
        inc    edx
loc_4_04D8:
        in     al, dx
        test   al, 0x40
        je     loc_4_04E1
        loop   loc_4_04D8
        jmp    loc_4_04E5
loc_4_04E1:
        mov    al, byte ptr [ebp + 0xc]
        out    dx, al
loc_4_04E5:
        pop    ebp
        ret    
sub_4_04E7:
        push   ebp
        mov    ebp, esp
        push   eax
        push   ebx
        push   ecx
        push   0xff
        push   dword ptr [ebp + 8]
        call   sub_4_04CA
        add    esp, 8
        movzx  edx, word ptr [ebp + 8]
        inc    edx
        mov    ecx, 0x6000
loc_4_0507:
        in     al, dx
        test   al, 0x80
        je     loc_4_050E
        loop   loc_4_0507
loc_4_050E:
        mov    ecx, 0x2000
loc_4_0513:
        in     al, dx
        loop   loc_4_0513
        pop    ecx
        pop    ebx
        pop    eax
        pop    ebp
        ret    
sub_4_051B:
        push   ebp
        mov    ebp, esp
        push   eax
        push   ecx
        push   edx
        movzx  edx, word ptr [ebp + 8]
        inc    edx
        mov    ecx, 0x6000
loc_4_052B:
        in     al, dx
        test   al, 0x80
        jne    loc_4_0535
        dec    edx
        in     al, dx
        inc    edx
        loop   loc_4_052B
loc_4_0535:
        pop    edx
        pop    ecx
        pop    eax
        pop    ebp
        ret    
sub_4_053A:
        push   ebp
        mov    ebp, esp
        push   ebx
        push   ecx
        push   edx
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
        jmp    loc_4_0562
loc_4_0562:
        out    0x20, al
        jmp    loc_4_0566
loc_4_0566:
        push   dword ptr [ebp + 8]
        call   sub_4_051B
        add    esp, 4
        in     al, 0xa0
        mov    ah, al
        in     al, 0x20
        test   ecx, eax
        jne    loc_4_0590
        push   dword ptr [ebp + 8]
        call   sub_4_04E7
        add    esp, 4
        in     al, 0xa0
        mov    ah, al
        in     al, 0x20
        test   ecx, eax
        jne    loc_4_0592
loc_4_0590:
        xor    eax, eax
loc_4_0592:
        push   dword ptr [ebp + 8]
        call   sub_4_051B
        add    esp, 4
        mov    edx, eax
        pop    eax
        xchg   ah, al
        out    0xa1, al
        xchg   ah, al
        out    0x21, al
        mov    edx, eax
        popfd  
        pop    edx
        pop    ecx
        pop    ebx
        pop    ebp
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 5  vsize=0x3fe  R-X
sub_5_0000:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0xd8
        test   eax, eax
        je     loc_5_002F
        cmp    eax, 1
        je     loc_5_0036
        cmp    eax, 2
        je     loc_5_008F
        cmp    eax, 3
        je     loc_5_009E
        cmp    eax, 4
        je     loc_5_008F
        mov    eax, 1
        jmp    loc_5_00CA
loc_5_002F:
        xor    eax, eax
        jmp    loc_5_00CA
loc_5_0036:
        push   0
        lea    ecx, [ebp - 0xd8]
        push   dword ptr [ebp + 0x10]
        push   ecx
        call   sub_5_03F8
        add    esp, 0xc
        test   eax, eax
        jne    loc_5_00CA
        cmp    word ptr [ebp - 0x7c], 1
        je     loc_5_005C
        mov    eax, 0x13
        jmp    loc_5_00CA
loc_5_005C:
        mov    ax, word ptr [ebp - 0x7a]
        cmp    word ptr [ebp - 0x2a], 0
        mov    cx, 0xffff
        je     loc_5_0070
        movzx  cx, byte ptr [ebp - 0x28]
loc_5_0070:
        movzx  edx, word ptr [ebp - 0x7a]
        cmp    edx, -1
        jne    loc_5_0080
        mov    eax, 0x13
        jmp    loc_5_00CA
loc_5_0080:
        push   ecx
        push   eax
        push   dword ptr [ebp + 0x10]
        call   sub_4_0342
loc_5_008A:
        add    esp, 0xc
        jmp    loc_5_00CA
loc_5_008F:
        push   dword ptr [ebp + 0x10]
        call   sub_4_039D
loc_5_0097:
        add    esp, 4
        xor    eax, eax
        jmp    loc_5_00CA
loc_5_009E:
        mov    eax, dword ptr [ebp + 0xc]
        test   eax, eax
        je     loc_5_00B1
        cmp    eax, 1
        je     loc_5_00B1
        mov    eax, 1
        jmp    loc_5_00CA
loc_5_00B1:
        push   dword ptr [ebp + 0x10]
        call   sub_4_0000
loc_5_00B9:
        add    esp, 4
        cmp    eax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 0x13
loc_5_00CA:
        mov    esp, ebp
        pop    ebp
        ret    
        db  0CCh, 0CCh
loc_5_00D0:
        push   eax
        mov    eax, dword ptr [esi + 0x18]
        cmp    eax, ebx
        je     loc_5_00EB
        or     eax, eax
        jne    loc_5_00FE
        push   ebx
        push   ecx
        push   edx
        mov    edi, esi
        call   sub_5_0276
        pop    edx
        pop    ecx
        pop    ebx
        jb     loc_5_00FE
loc_5_00EB:
        pop    eax
        cmp    ecx, 4
        jbe    loc_5_00F7
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_5_00F7:
        je     loc_5_00FC
        in     al, dx
        jmp    loc_5_00FD
loc_5_00FC:
        out    dx, al
loc_5_00FD:
        ret    
loc_5_00FE:
        mov    edi, esi
        call   sub_5_0152
        pop    eax
        xor    eax, eax
        dec    eax
        ret    
sub_5_010A:
        push   esi
        push   eax
        push   ecx
        mov    si, 1
        xor    ecx, ecx
        movzx  edx, word ptr [edi + 6]
loc_5_0117:
        VMMCall  VMM.Enable_Local_Trapping       ; cd2098000100
        inc    dx
        cmp    cx, si
        je     loc_5_0128
        inc    cx
        jmp    loc_5_0117
loc_5_0128:
        pop    ecx
        pop    eax
        pop    esi
        clc    
        ret    
sub_5_012D:
        push   esi
        push   eax
        push   ecx
        push   edx
        mov    si, 1
        xor    ecx, ecx
        movzx  edx, word ptr [edi + 6]
loc_5_013B:
        VMMCall  VMM.Disable_Local_Trapping      ; cd209a000100
        inc    dx
        cmp    cx, si
        je     loc_5_014C
        inc    cx
        jmp    loc_5_013B
loc_5_014C:
        pop    edx
        pop    ecx
        pop    eax
        pop    esi
        clc    
        ret    
sub_5_0152:
        test   word ptr [edi + 0xc], 1
        jne    loc_5_0188
        call   sub_1_010D
loc_5_015F:
        jb     loc_5_0188
        test   dword ptr [esi], 1
        jne    loc_5_0188
        movzx  ecx, word ptr [0xc]           ; -> obj2:0x000C
        add    ecx, dword ptr [4]            ; -> obj2:0x0004
        or     dword ptr [esi], 1
        mov    eax, 0x30
        xor    esi, esi
        xor    edi, edi
        VxDCall  SHELL.SHELL_Message             ; cd2004001700
loc_5_0188:
        ret    
loc_5_0189:
        pushfd 
        mov    esi, dword ptr [0]            ; -> obj1:0x0054
        or     esi, esi
        je     loc_5_01C1
        cli    
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_5_01C1
loc_5_019D:
        popfd  
        pushfd 
        push   eax
        mov    edi, dword ptr [eax]
        xor    eax, eax
        cmp    dword ptr [edi + 0x18], ebx
        jne    loc_5_01B5
        mov    dword ptr [edi + 0x1c], 0xffffffff
        call   sub_5_023A
loc_5_01B5:
        pop    eax
        cli    
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        je     loc_5_01C1
        jmp    loc_5_019D
loc_5_01C1:
        mov    edi, dword ptr [0]            ; -> obj1:0x0050
        xor    esi, esi
        xchg   dword ptr [edi + ebx], esi
        or     esi, esi
        je     loc_5_01D6
        VMMCall  VMM.List_Destroy                ; cd209c000100
loc_5_01D6:
        popfd  
        clc    
        ret    
loc_5_01D9:
        pushal 
        pushfd 
        mov    edi, dword ptr [0]            ; -> obj1:0x0050
        mov    eax, 5
        mov    ecx, 8
        VMMCall  VMM.List_Create                 ; cd209b000100
        jb     loc_5_0232
        mov    dword ptr [edi + ebx], esi
        mov    esi, dword ptr [0]            ; -> obj1:0x0054
        or     esi, esi
        je     loc_5_0236
        cli    
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        sti    
        je     loc_5_0236
loc_5_020A:
        mov    edi, dword ptr [eax]
        call   sub_4_002A
loc_5_0211:
        jb     loc_5_021F
        cli    
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        sti    
        je     loc_5_0236
        jmp    loc_5_020A
loc_5_021F:
        mov    edi, dword ptr [0]            ; -> obj1:0x0050
        mov    esi, dword ptr [edi + ebx]
        or     esi, esi
        je     loc_5_0232
        VMMCall  VMM.List_Destroy                ; cd209c000100
loc_5_0232:
        popfd  
        stc    
        jmp    loc_5_0238
loc_5_0236:
        popfd  
        clc    
loc_5_0238:
        popal  
        ret    
sub_5_023A:
        push   eax
        push   edx
        push   esi
        call   sub_1_010D
loc_5_0242:
        jb     loc_5_0271
        cmp    dword ptr [edi + 0x18], ebx
        jne    loc_5_0271
        call   sub_5_010A
        mov    dword ptr [edi + 0x18], 0
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_5_026E
        VxDCall  VPICD.VPICD_Physically_Mask     ; cd2008000300
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
        VxDCall  VPICD.VPICD_Clear_Int_Request   ; cd2003000300
loc_5_026E:
        and    dword ptr [esi], 0xfffffffe
loc_5_0271:
        pop    esi
        pop    edx
        pop    eax
        clc    
        ret    
sub_5_0276:
        push   eax
        push   ecx
        push   edx
        cmp    dword ptr [edi + 0x18], 0
        je     loc_5_0287
        cmp    dword ptr [edi + 0x18], ebx
        je     loc_5_02BE
        stc    
        jb     loc_5_02BF
loc_5_0287:
        mov    dword ptr [edi + 0x18], ebx
        cmp    dword ptr [edi + 0x1c], ebx
        je     loc_5_0293
        movzx  edx, word ptr [edi + 6]
loc_5_0293:
        mov    dword ptr [edi + 0x1c], ebx
        call   sub_5_012D
        mov    eax, dword ptr [edi + 0x14]
        or     eax, eax
        je     loc_5_02BE
        VxDCall  VPICD.VPICD_Get_Complete_Status ; cd2005000300
        test   ecx, 0x10
        je     loc_5_02B8
        VxDCall  VPICD.VPICD_Physically_Mask     ; cd2008000300
        jmp    loc_5_02BE
loc_5_02B8:
        VxDCall  VPICD.VPICD_Physically_Unmask   ; cd2009000300
loc_5_02BE:
        clc    
loc_5_02BF:
        pop    edx
        pop    ecx
        pop    eax
        ret    
sub_5_02C3:
        mov    eax, 0x400
        clc    
        ret    
        db  0CCh, 0CCh
msmpu401_PM_API:
        movzx  eax, word ptr [ebp + 0x14]
        cmp    ah, 4
        jae    loc_5_02F6
        movzx  ecx, ah
        shl    ecx, 3
        lea    ecx, [ecx]                    ; -> obj6:0x0010
        movzx  eax, al
        cmp    eax, dword ptr [ecx]
        jae    loc_5_02F6
        mov    ecx, dword ptr [ecx + 4]
        call   dword ptr [ecx + eax*4]
        jb     loc_5_02F6
        and    word ptr [ebp + 0x2c], 0xfffe
        ret    
loc_5_02F6:
        or     word ptr [ebp + 0x2c], 1
        ret    
loc_5_02FC:
        call   sub_5_02C3
        mov    word ptr [ebp + 0x1c], ax
        clc    
        ret    
loc_5_0307:
        movzx  edx, word ptr [ebp + 0x1c]
        or     edx, edx
        je     loc_5_0337
        push   2
        push   edx
        call   sub_1_00AC
loc_5_0317:
        add    esp, 8
        or     edi, edi
        je     loc_5_0337
        mov    ecx, dword ptr [edi + 0x1c]
        mov    word ptr [ebp + 0x1c], 0
        call   sub_5_0276
        jb     loc_5_032F
        ret    
loc_5_032F:
        mov    word ptr [ebp + 0x1c], 2
        jmp    loc_5_033D
loc_5_0337:
        mov    word ptr [ebp + 0x1c], 1
loc_5_033D:
        stc    
        ret    
loc_5_033F:
        movzx  edx, word ptr [ebp + 0x1c]
        or     edx, edx
        je     loc_5_0372
        push   2
        push   edx
        call   sub_1_00AC
loc_5_034F:
        add    esp, 8
        or     edi, edi
        je     loc_5_0372
        mov    ecx, dword ptr [edi + 0x18]
        cmp    ecx, ebx
        je     loc_5_0365
        mov    word ptr [ebp + 0x1c], 3
        jmp    loc_5_0378
loc_5_0365:
        call   sub_5_023A
        mov    word ptr [ebp + 0x1c], 0
        clc    
        ret    
loc_5_0372:
        mov    word ptr [ebp + 0x1c], 1
loc_5_0378:
        stc    
        ret    
loc_5_037A:
        pushfd 
        mov    ax, word ptr [ebp + 0x1c]
        cmp    ax, 1
        jne    loc_5_03EF
        mov    ecx, dword ptr [ebp + 0x18]
        cli    
        mov    esi, dword ptr [0]            ; -> obj1:0x0054
        or     esi, esi
        je     loc_5_03EF
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_5_03EF
loc_5_039B:
        mov    edi, dword ptr [eax]
        cmp    dword ptr [edi + 0x10], ecx
        je     loc_5_03AC
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        je     loc_5_03EF
        jmp    loc_5_039B
loc_5_03AC:
        popfd  
        pushfd 
        mov    esi, edi
        xchg   edi, eax
        mov    ax, 0x3810
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   edi, eax
        cmp    edi, -1
        je     loc_5_03EF
        mov    ecx, dword ptr [edi]
        or     ecx, ecx
        je     loc_5_03EF
        cmp    ecx, 0x24
        ja     loc_5_03EF
        mov    word ptr [edi + 4], 0
        mov    ax, word ptr [esi + 6]
        mov    word ptr [edi + 6], ax
        mov    al, byte ptr [esi + 8]
        mov    byte ptr [edi + 8], al
        mov    eax, dword ptr [esi + 0x10]
        mov    dword ptr [edi + 0x10], eax
        mov    word ptr [ebp + 0x1c], 1
        popfd  
        clc    
        ret    
loc_5_03EF:
        mov    word ptr [ebp + 0x1c], 0
        popfd  
        stc    
        ret    
sub_5_03F8:
        VxDCall  CONFIGMG._CONFIGMG_Get_Alloc_Log_Conf ; cd203b803300
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 6  vsize=0x5e  RW-
        dd  loc_5_02FC               ; fixup
        dd  loc_5_037A               ; fixup
        dd  loc_5_0307               ; fixup
        dd  loc_5_033F               ; fixup
        db  001h, 000h, 000h, 000h
        dd  obj6:0x0000              ; fixup
        db  000h, 000h, 000h, 000h
        dd  loc_5_02F6               ; fixup
        db  000h, 000h, 000h, 000h
        dd  loc_5_02F6               ; fixup
        db  004h, 000h, 000h, 000h
        dd  obj6:0x0000              ; fixup
        db  002h, 000h, 000h, 000h
        dd  loc_5_00D0               ; fixup
        db  001h, 000h
        dd  loc_5_00D0               ; fixup
        db  000h, 000h, 000h, 000h
        dd  loc_1_0190               ; fixup
        db  000h, 000h, 000h, 000h
        dd  loc_1_01B4               ; fixup
        dd  loc_1_01C4               ; fixup
        db  000h, 000h, 000h, 000h, 0F4h, 001h, 000h, 000h, 000h, 000h, 000h, 000h
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
        mov    dword ptr [0], eax            ; -> obj1:0x0050
        mov    eax, 5
        mov    ecx, 4
        VMMCall  VMM.List_Create                 ; cd209b000100
        jb     loc_7_0011
        mov    dword ptr [0], esi            ; -> obj1:0x0054
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        mov    edi, dword ptr [0]            ; -> obj1:0x0050
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
        mov    esi, 0                        ; -> loc_1_014C
        VMMCall  VMM.Hook_Device_Service         ; cd2090000100
        clc    
        ret    
loc_7_0070:
        pop    ebx
        jmp    loc_7_0011
VxD__LTEXT ENDS

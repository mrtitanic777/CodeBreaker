; VFBACKUP -- reconstructed 32-bit disassembly (VxD / LE format)
; 4 objects, 1526 instructions recovered by recursive descent

Declare_Virtual_Device VFBACKUP, 4, 0, VFBACKUP_Control, 00036h, Undefined_Init_Order
;   DDK version 0400h   init_order 0A0810000h   flags 00000h
;   service table: obj1:0x3c4, 6 services

VxD__LTEXT SEGMENT   ; object 1  vsize=0x4d0  R-X
        db  000h, 000h, 000h, 000h
VFBACKUP_Control:
        cmp    eax, 0
        je     loc_3_0000
        cmp    eax, 1
        je     loc_3_0109
        cmp    eax, 0xc
        je     loc_1_0022
        cmp    eax, 0x21
        je     loc_1_004E
        clc    
        ret    
loc_1_0022:
        mov    ecx, dword ptr [0x3f4]        ; -> loc_1_03F4
        mov    ecx, dword ptr [ecx]
        cmp    ebx, ecx
        je     loc_1_0030
        clc    
        ret    
loc_1_0030:
        call   VFBACKUP_UnLock_NEC
        pushfd 
        movzx  edx, word ptr [0x3e0]         ; -> loc_1_03E0
        mov    al, 0xc
        out    dx, al
        mov    edx, dword ptr [0x3f8]        ; -> loc_1_03F8
        mov    dword ptr [edx], 0
        popfd  
        ret    
loc_1_004E:
        mov    ebx, edi
        jmp    loc_1_0022
VFBACKUP_V86_API:
        ret    
VFBACKUP_PM_API:
        ret    
VFBACKUP_Get_Version:
        mov    eax, 0x400
        clc    
        ret    
loc_1_005B:
        cmp    eax, 1
        je     loc_3_036F
        clc    
        ret    
        db  000h, 000h
sub_1_0068:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_009C:
        mov    ebx, dword ptr [esp + 4]
        mov    word ptr [ebx + 2], 0
        cmp    word ptr [ebx], 8
        je     loc_1_00B9
        cmp    word ptr [ebx], 2
        je     loc_1_00B8
        mov    word ptr [ebx + 2], 0xffff
loc_1_00B8:
        ret    
loc_1_00B9:
        mov    ebx, dword ptr [0x3f4]        ; -> loc_1_03F4
        cmp    dword ptr [ebx], 0
        jne    loc_1_00C5
loc_1_00C4:
        ret    
loc_1_00C5:
        mov    ecx, dword ptr [0x3fc]        ; -> loc_1_03FC
        test   byte ptr [ecx], 1
        je     loc_1_00C4
        dec    dword ptr [0x49c]             ; -> obj1:0x049C
        jne    loc_1_00C4
        mov    esi, 0xe6                     ; -> loc_1_00E6
        mov    edx, dword ptr [ebx]
        VMMCall  VMM.Schedule_Global_Event       ; cd200e000100
        ret    
loc_1_00E6:
        mov    ebx, edx
        call   VFBACKUP_UnLock_NEC
        ret    
        db  000h, 000h
loc_1_00F0:
        test   byte ptr [ebp + 0x14], 0x80
        jne    loc_1_010F
        call   sub_1_019F
        or     byte ptr [0x3f0], 0x40        ; -> obj1:0x03F0
        xor    eax, eax
        mov    esi, 0x111                    ; -> loc_1_0111
        VMMCall  VMM.Call_When_VM_Returns        ; cd200d000100
loc_1_010F:
        stc    
        ret    
loc_1_0111:
        call   sub_1_0185
        and    byte ptr [0x3f0], 0xbf        ; -> obj1:0x03F0
        mov    eax, dword ptr [0x3f8]        ; -> loc_1_03F8
        mov    dword ptr [eax], 0
        ret    
loc_1_0129:
        push   ebx
        VMMCall  VMM.Get_Crit_Status_No_Block    ; cd20f1000100
        test   byte ptr [0x3f0], 0x40        ; -> obj1:0x03F0
        jne    loc_1_016E
        mov    ecx, dword ptr [0x3f8]        ; -> loc_1_03F8
        mov    ecx, dword ptr [ecx]
        jecxz  loc_1_015C
        mov    ebx, ecx
        mov    dword ptr [0x49c], 0x1e       ; -> obj1:0x049C
        mov    ecx, dword ptr [0x400]        ; -> loc_1_0400
        mov    ecx, dword ptr [ecx]
        jecxz  loc_1_015C
        pop    ebx
        jmp    ecx
loc_1_015C:
        push   eax
        mov    eax, dword ptr [0x3fc]        ; -> loc_1_03FC
        test   dword ptr [eax], 0x100
        pop    eax
        je     loc_1_016E
        mov    ebx, dword ptr [ebx + 0x14]
loc_1_016E:
        VxDCall  VPICD.VPICD_Set_Int_Request     ; cd2002000300
        pop    ebx
        clc    
        ret    
loc_1_0177:
        VxDCall  VPICD.VPICD_Clear_Int_Request   ; cd2003000300
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
        clc    
        ret    
sub_1_0185:
        mov    ecx, 6
        mov    edi, 0x3dc                    ; -> obj1:0x03DC
loc_1_018F:
        movzx  edx, word ptr [edi]
        VMMCall  VMM.Enable_Local_Trapping       ; cd2098000100
        add    edi, 2
        loop   loc_1_018F
        clc    
        ret    
sub_1_019F:
        mov    ecx, 6
        mov    edi, 0x3dc                    ; -> obj1:0x03DC
loc_1_01A9:
        movzx  edx, word ptr [edi]
        VMMCall  VMM.Disable_Local_Trapping      ; cd209a000100
        add    edi, 2
        loop   loc_1_01A9
        clc    
        ret    
loc_1_01B9:
        mov    edi, dword ptr [0x3fc]        ; -> loc_1_03FC
        test   byte ptr [edi], 8
        jne    loc_1_02A1
        cmp    word ptr [ebp + 0x28], 0xf000
        je     loc_1_0275
        cmp    edx, 0x3f2
        je     loc_1_0263
loc_1_01E0:
        mov    edi, dword ptr [0x3f4]        ; -> loc_1_03F4
        mov    edi, dword ptr [edi]
        or     edi, edi
        je     loc_1_01FE
        push   eax
        mov    eax, dword ptr [0x3fc]        ; -> loc_1_03FC
        test   dword ptr [eax], 0x100
        pop    eax
        je     loc_1_01FE
        mov    edi, dword ptr [edi + 0x14]
loc_1_01FE:
        cmp    edi, ebx
        mov    edi, 1
        jne    loc_1_0217
        mov    edi, dword ptr [0x3fc]        ; -> loc_1_03FC
        mov    edi, dword ptr [edi]
        mov    ebx, dword ptr [0x3f4]        ; -> loc_1_03F4
        mov    ebx, dword ptr [ebx]
loc_1_0217:
        push   eax
        xor    eax, eax
        test   edi, 1
        je     loc_1_0223
        inc    eax
loc_1_0223:
        push   ecx
        xor    ecx, ecx
        call   VFBACKUP_Lock_NEC
        pop    ecx
        pop    eax
        pushfd 
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        popfd  
        jb     loc_1_025D
        mov    esi, dword ptr [0x3fc]        ; -> loc_1_03FC
        or     dword ptr [esi], edi
        mov    dword ptr [0x49c], 0x1e       ; -> obj1:0x049C
loc_1_0249:
        test   ecx, 0x58
        jne    loc_1_025A
        test   ecx, 4
        jne    loc_1_025B
        in     al, dx
loc_1_025A:
        ret    
loc_1_025B:
        out    dx, al
        ret    
loc_1_025D:
        VMMCall  VMM.Crash_Cur_VM                ; cd202f000100
loc_1_0263:
        movzx  edi, al
        and    di, 0xf8
        cmp    di, 8
        jne    loc_1_01E0
loc_1_0275:
        mov    edi, dword ptr [0x3f8]        ; -> loc_1_03F8
        cmp    dword ptr [edi], 0
        jne    loc_1_029F
        mov    edi, dword ptr [0x3fc]        ; -> loc_1_03FC
        test   byte ptr [edi], 4
        jne    loc_1_029F
        mov    edi, dword ptr [0x3f4]        ; -> loc_1_03F4
        cmp    dword ptr [edi], 0
        jne    loc_1_029F
        test   byte ptr [0x3f0], 0x10        ; -> obj1:0x03F0
        jne    loc_1_025A
loc_1_029F:
        jmp    loc_1_0249
loc_1_02A1:
        test   ecx, 0x58
        jne    loc_1_025A
        test   ecx, 4
        jne    loc_1_025A
        mov    al, 0xff
        ret    
loc_1_02B4:
        cmp    eax, 1
        je     0xffff0420                    ; -> obj3:0x0420
        clc    
        ret    
loc_1_02BF:
        bts    dword ptr [0x4ac], 1          ; -> loc_1_04AC
        jb     loc_1_02E5
        xchg   dword ptr [0x4a4], esp        ; -> loc_1_04A4
        call   sub_1_04B0
        xchg   dword ptr [0x4a4], esp        ; -> loc_1_04A4
        pushfd 
        and    byte ptr [0x4ac], 0xfd        ; -> loc_1_04AC
        popfd  
        ret    
loc_1_02E5:
        jmp    sub_1_04B0
loc_1_02EB:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        jmp    loc_1_02F7
        db  0FFh, 025h
        dd  loc_1_02EB               ; fixup
loc_1_02F7:
        cmp    word ptr [edi], 6
        jne    loc_1_032F
        mov    eax, dword ptr [0x3f4]        ; -> loc_1_03F4
        cmp    dword ptr [eax], ebx
        jne    loc_1_030B
        call   VFBACKUP_UnLock_NEC
loc_1_030B:
        pushal 
        mov    ecx, dword ptr [edi + 4]
        mov    dword ptr [0x4b0], ecx        ; -> sub_1_04B0
        mov    ecx, 0x2bf                    ; -> loc_1_02BF
        xor    eax, eax
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        mov    ebx, edi
        call   VFBACKUP_Lock_NEC
        mov    dword ptr [esp + 0x1c], eax
        popal  
        ret    
loc_1_032F:
        jmp    loc_1_02EB
loc_1_0335:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        jmp    loc_1_0341
        db  0FFh, 025h
        dd  loc_1_0335               ; fixup
loc_1_0341:
        cmp    eax, dword ptr [0x414]        ; -> obj1:0x0414
        jne    loc_1_035B
        pushal 
        xor    eax, eax
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        mov    ebx, edi
        call   VFBACKUP_UnLock_NEC
        popal  
        ret    
loc_1_035B:
        jmp    loc_1_0335
        db  000h, 000h, 000h
loc_1_0364:
        cmp    eax, 1
        je     loc_3_0520
        clc    
        ret    
loc_1_036F:
        mov    ecx, dword ptr [0x3f4]        ; -> loc_1_03F4
        mov    ecx, dword ptr [ecx]
        jecxz  loc_1_039C
        movzx  eax, byte ptr [ebp + 0x1c]
        cmp    eax, dword ptr [0x418]        ; -> obj1:0x0418
        jae    loc_1_039C
        mov    ax, 0x804
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        test   word ptr [eax + 4], 0x8000
        jne    loc_1_039C
        call   sub_1_039E
loc_1_039C:
        stc    
        ret    
sub_1_039E:
        push   ebx
        cmp    dword ptr [0x4bc], 0          ; -> loc_1_04BC
        je     loc_1_03C2
        mov    eax, 0x10
        mov    ebx, ecx
        xor    ecx, ecx
        mov    esi, 0x6cc                    ; -> loc_4_06CC
        VMMCall  VMM.Call_Priority_VM_Event      ; cd2014000100
        mov    dword ptr [0x4c0], esi        ; -> loc_1_04C0
loc_1_03C2:
        pop    ebx
        ret    
        dd  VFBACKUP_Get_Version     ; fixup
        dd  VFBACKUP_Lock_NEC        ; fixup
        dd  VFBACKUP_UnLock_NEC      ; fixup
        dd  VFBACKUP_Register_NEC    ; fixup
        dd  VFBACKUP_Register_VFD    ; fixup
        dd  VFBACKUP_Lock_All_Ports  ; fixup
        db  0F0h, 003h, 0F1h, 003h
loc_1_03E0:
        add    esi, esp
        add    esi, ebp
        add    esi, edi
        add    eax, dword ptr [eax]
        add    byte ptr [ecx + 0x20600], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
loc_1_03F4:
        add    al, 4
        add    byte ptr [eax], al
loc_1_03F8:
        or     byte ptr [eax + eax], al
        db  000h
loc_1_03FC:
        or     al, 4
        add    byte ptr [eax], al
loc_1_0400:
        adc    byte ptr [eax + eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        db  000h
sub_1_043C:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_0440:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        ; ---- Device Descriptor Block ----
        dd  0                          ; DDB_Next
        dw  00400h                      ; DDB_SDK_Version
        dw  00036h                      ; DDB_Req_Device_Number
        db  4, 0                       ; DDB_Dev_Major/Minor
        dw  00000h                      ; DDB_Flags
        db  'VFBACKUP'                  ; DDB_Name
        dd  0A0810000h                  ; DDB_Init_Order
        dd  VFBACKUP_Control         ; DDB_Control_Proc
        dd  VFBACKUP_V86_API         ; DDB_V86_API_Proc
        dd  VFBACKUP_PM_API          ; DDB_PM_API_Proc
        dd  obj1:0x03C4              ; DDB_Service_Table_Ptr
        db  000h, 000h, 000h, 000h, 076h, 065h, 072h, 050h, 050h, 000h, 000h, 000h, 031h, 076h, 073h, 052h
        db  032h, 076h, 073h, 052h, 033h, 076h, 073h, 052h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h
loc_1_04A0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_04A4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_04A8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_04AC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_04B0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_04B4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_04B8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_04BC:
        add    byte ptr [eax], al
loc_1_04BE:
        add    byte ptr [eax], al
loc_1_04C0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_04C4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_1_04C8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
sub_1_04CC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 2  vsize=0x92  R-X
        db  002h, 000h, 000h, 000h
loc_2_0004:
        adc    al, 0
        add    byte ptr [eax], al
        add    byte ptr [ecx], al
        add    byte ptr [eax], al
loc_2_000C:
        add    byte ptr [eax], al
loc_2_000E:
        inc    esp
        add    byte ptr [esi], bh
        add    byte ptr [eax], al
loc_2_0014:
        push   esi
        inc    esi
        inc    edx
        inc    ecx
        inc    ebx
        dec    ebx
        push   ebp
        push   eax
        and    byte ptr [ebx + 0x6f], ah
        jne    0x8d
        and    byte ptr fs:[esi + 0x6f], ch
        je     0x47
        popal  
        insb   byte ptr es:[edi], dx
        insb   byte ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        arpl   word ptr [ecx + 0x74], sp
        and    byte ptr gs:[esi + 0x65], ch
        and    byte ptr fs:[ebp + 0x65], ch
        insd   dword ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        jb     0xb6
        and    byte ptr cs:[eax + 0x6c], dl
        popal  
        jae    0xaa
        and    byte ptr [edx + 0x75], dh
        outsb  dx, byte ptr [esi]
        and    byte ptr [ebx + 0x65], dh
        je     0xc3
        jo     0x70
        popal  
        popal  
        imul   ebp, dword ptr [esi + 0x2e], 0x46560000
        inc    edx
        inc    ecx
        inc    ebx
        dec    ebx
        push   ebp
        push   eax
        and    byte ptr [ebx + 0x6f], ah
        jne    0xd1
        and    byte ptr fs:[esi + 0x6f], ch
        je     loc_2_008B
        insb   byte ptr es:[edi], dx
        outsd  dx, dword ptr [esi]
        popal  
        and    byte ptr fs:[esi + 0x46], dl
        inc    esp
        push   esi
        pop    eax
        inc    esp
        and    byte ptr cs:[eax], ah
        push   eax
        insb   byte ptr es:[edi], dx
        popal  
        jae    0xe5
        and    byte ptr [edx + 0x75], dh
        outsb  dx, byte ptr [esi]
        and    byte ptr [ebx + 0x65], dh
        je     0xfe
        jo     0xab
loc_2_008B:
        popal  
        popal  
        db  069h, 06Eh, 000h, 000h
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x704  R-X
loc_3_0000:
        mov    eax, 0x2200
        VMMCall  VMM.Get_DDB                     ; cd2046010100
        or     ecx, ecx
        jne    loc_3_0047
loc_3_000F:
        mov    eax, 0x2760
        VMMCall  VMM.Get_DDB                     ; cd2046010100
        or     ecx, ecx
        jne    loc_3_008F
loc_3_001E:
        mov    eax, 0x299c
        VMMCall  VMM.Get_DDB                     ; cd2046010100
        or     ecx, ecx
        jne    loc_3_00CC
loc_3_0031:
        mov    eax, 0x1b
        VMMCall  VMM.Get_DDB                     ; cd2046010100
        jecxz  loc_3_0045
        mov    eax, ecx
        call   sub_3_0079
loc_3_0045:
        clc    
        ret    
loc_3_0047:
        mov    eax, ecx
        mov    ecx, 2
        lea    esi, [0x5dc]                  ; -> loc_3_05DC
        lea    edi, [eax + 0xc]
        repe cmpsd dword ptr [esi], dword ptr es:[edi]
        jne    loc_3_000F
        movzx  ecx, word ptr [0x5d8]         ; -> loc_3_05D8
        movzx  ebx, word ptr [eax + 8]
        xchg   bl, bh
        cmp    bx, cx
        ja     loc_3_000F
        mov    dword ptr [0x6c0], eax        ; -> loc_3_06C0
        call   sub_3_0079
        jmp    loc_3_000F
sub_3_0079:
        mov    edi, eax
        VMMCall  VMM.VMM_Remove_DDB              ; cd20f8000100
        mov    dword ptr [edi + 0x14], 0xa0910000
        VMMCall  VMM.VMM_Add_DDB                 ; cd20f7000100
        ret    
loc_3_008F:
        mov    eax, ecx
        mov    ecx, 2
        lea    esi, [0x62c]                  ; -> loc_3_062C
        lea    edi, [eax + 0xc]
        repe cmpsd dword ptr [esi], dword ptr es:[edi]
        jne    loc_3_001E
        movzx  ecx, word ptr [0x628]         ; -> loc_3_0628
        movzx  ebx, word ptr [eax + 8]
        xchg   bl, bh
        cmp    bx, cx
        ja     loc_3_001E
        mov    dword ptr [0x6c4], eax        ; -> loc_3_06C4
        call   sub_3_0079
        jmp    loc_3_001E
loc_3_00CC:
        mov    eax, ecx
        mov    ecx, 2
        lea    esi, [0x67c]                  ; -> loc_3_067C
        lea    edi, [eax + 0xc]
        repe cmpsd dword ptr [esi], dword ptr es:[edi]
        jne    loc_3_0031
        movzx  ecx, word ptr [0x678]         ; -> obj3:0x0678
        movzx  ebx, word ptr [eax + 8]
        xchg   bl, bh
        cmp    bx, cx
        ja     loc_3_0031
        mov    dword ptr [0x6c8], eax        ; -> loc_3_06C8
        call   sub_3_0079
        jmp    loc_3_0031
loc_3_0109:
        mov    eax, dword ptr [0x6c0]        ; -> loc_3_06C0
        or     eax, eax
        jne    loc_3_01A2
loc_3_0116:
        mov    eax, dword ptr [0x6c4]        ; -> loc_3_06C4
        or     eax, eax
        jne    loc_3_0227
loc_3_0123:
        mov    eax, dword ptr [0x6c8]        ; -> loc_3_06C8
        or     eax, eax
        jne    loc_3_025A
loc_3_0130:
        call   sub_3_03BC
        jb     loc_3_0177
        call   sub_3_03EC
        jb     loc_3_02F5
        test   dword ptr [0x3f0], 0x10       ; -> obj1:0x03F0
        je     loc_3_028D
        movzx  eax, byte ptr [0x3ed]         ; -> obj1:0x03ED
        mov    esi, 0x584                    ; -> loc_4_0584
        VxDCall  VDMAD.VDMAD_Virtualize_Channel  ; cd2001000400
        jb     loc_3_02F5
        test   dword ptr [0x3f0], 1          ; -> obj1:0x03F0
        jne    loc_3_0178
        clc    
loc_3_0177:
        ret    
loc_3_0178:
        push   0xe
        push   0x430                         ; -> obj1:0x0430
        push   0xfff
        push   0
        push   0xf
        push   0
        push   1
        push   8
        VMMCall  VMM._PageAllocate               ; cd2053000100
        add    esp, 0x20
        or     edx, edx
        mov    dword ptr [0x434], edx        ; -> obj1:0x0434
        ret    
loc_3_01A0:
        stc    
        ret    
loc_3_01A2:
        test   byte ptr [0x3f0], 0x10        ; -> obj1:0x03F0
        je     loc_3_01A0
        or     byte ptr [0x3f0], 1           ; -> obj1:0x03F0
        mov    esi, dword ptr [eax + 0x20]
        mov    dword ptr [0x43c], esi        ; -> sub_1_043C
        push   eax
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        push   ebp
        mov    ebp, dword ptr [ebx + 8]
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        mov    byte ptr [ebp + 0x1d], 0
        call   esi
        movzx  ecx, word ptr [ebp + 0x1c]
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        pop    ebp
        pop    eax
        mov    esi, dword ptr [eax + 0x18]
        mov    dword ptr [0x440], esi        ; -> sub_1_0440
        mov    edx, 0x318                    ; -> loc_4_0318
        cmp    ecx, 0x324
        jbe    loc_3_020B
        mov    edx, 0x32c                    ; -> loc_4_032C
loc_3_020B:
        mov    dword ptr [0x340], edx        ; -> loc_4_0340
        lea    esi, [0x5e8]                  ; -> loc_3_05E8
        push   0x116                         ; -> loc_3_0116
loc_3_021C:
        lea    edi, [eax + 0x18]
        mov    ecx, 5
        rep movsd dword ptr es:[edi], dword ptr [esi]
        ret    
loc_3_0227:
        test   byte ptr [0x3f0], 0x10        ; -> obj1:0x03F0
        je     loc_3_01A0
        or     byte ptr [0x3f0], 2           ; -> obj1:0x03F0
        mov    esi, dword ptr [eax + 0x20]
        mov    dword ptr [0x4c4], esi        ; -> loc_1_04C4
        mov    esi, dword ptr [eax + 0x18]
        mov    dword ptr [0x4cc], esi        ; -> sub_1_04CC
        lea    esi, [0x638]                  ; -> loc_3_0638
        push   0x123                         ; -> loc_3_0123
        jmp    loc_3_021C
loc_3_025A:
        test   byte ptr [0x3f0], 0x10        ; -> obj1:0x03F0
        je     loc_3_01A0
        or     byte ptr [0x3f0], 4           ; -> obj1:0x03F0
        mov    esi, dword ptr [eax + 0x20]
        mov    dword ptr [0x4a0], esi        ; -> loc_1_04A0
        mov    esi, dword ptr [eax + 0x18]
        mov    dword ptr [0x4b8], esi        ; -> sub_1_04B8
        lea    esi, [0x688]                  ; -> obj3:0x0688
        push   0x130                         ; -> loc_3_0130
        jmp    loc_3_021C
loc_3_028D:
        VxDCall  VFD.VFD_Get_Version             ; cd2000001b00
        jb     loc_3_029D
        cmp    ax, 0x400
        jb     loc_3_02F5
        jmp    loc_3_02D1
loc_3_029D:
        VMMCall  VMM.Get_Exec_Path               ; cd20b6000100
        sub    esp, 8
        sub    esp, ecx
        mov    eax, ecx
        mov    esi, edx
        mov    edi, esp
        rep movsb byte ptr es:[edi], byte ptr [esi]
        lea    ecx, [eax + 8]
        mov    dword ptr [edi], 0x2e444656
        mov    dword ptr [edi + 4], 0x445856
        mov    edx, esp
        mov    al, 1
        VxDCall  APM.Service_0001h               ; cd2001002700
        lahf   
        add    esp, ecx
        rcr    al, 1
        jb     loc_3_0312
loc_3_02D1:
        movzx  eax, byte ptr [0x3ec]         ; -> obj1:0x03EC
        call   sub_4_05D9
        jb     loc_3_02F5
        mov    dword ptr [0x414], eax        ; -> obj1:0x0414
        mov    eax, 0x13
        mov    esi, 0xf0                     ; -> loc_1_00F0
        VMMCall  VMM.Hook_V86_Int_Chain          ; cd2041000100
        ret    
loc_3_02F5:
        stc    
        ret    
        db  00Fh, 0B7h, 005h
        dd  loc_2_000C               ; fixup
        db  003h, 005h
        dd  loc_2_0004               ; fixup
        db  060h, 08Bh, 0F0h, 0B8h, 001h, 000h, 000h, 000h, 0CDh, 020h, 0BEh, 000h, 001h, 000h
loc_3_0312:
        movzx  eax, word ptr [0xe]           ; -> loc_2_000E
        add    eax, dword ptr [4]            ; -> loc_2_0004
        pushal 
        mov    esi, eax
        mov    eax, 1
        VMMCall  VMM.Fatal_Error_Handler         ; cd20be000100
VFBACKUP_Register_NEC:
        mov    dword ptr [0x414], edx        ; -> obj1:0x0414
        mov    dword ptr [0x3f8], ecx        ; -> loc_1_03F8
        mov    dword ptr [0], ebx            ; -> obj1:0x0000
        mov    dword ptr [0x418], eax        ; -> obj1:0x0418
        mov    dword ptr [0x3fc], edi        ; -> loc_1_03FC
        mov    dword ptr [0x400], esi        ; -> loc_1_0400
        mov    eax, 0x49c                    ; -> obj1:0x049C
        or     byte ptr [0x3f0], 0x10        ; -> obj1:0x03F0
        ret    
VFBACKUP_Register_VFD:
        test   byte ptr [0x3f0], 0x10        ; -> obj1:0x03F0
        stc    
        jne    loc_3_036E
        or     byte ptr [0x3f0], 0x20        ; -> obj1:0x03F0
loc_3_036E:
        ret    
loc_3_036F:
        xor    ecx, ecx
        cmp    dword ptr [0x340], 0x32c      ; -> loc_4_0340
        jne    loc_3_03AA
        mov    eax, 0x10099
        mov    esi, 0x3b8                    ; -> loc_3_03B8
        VMMCall  VMM.Hook_Device_Service         ; cd2090000100
        mov    eax, 1
        call   sub_1_0440
        pushfd 
        mov    eax, 0x10099
        mov    esi, 0x3b8                    ; -> loc_3_03B8
        VMMCall  VMM.Unhook_Device_Service       ; cd201c010100
        popfd  
loc_3_03AA:
        clc    
        ret    
        db  000h, 000h, 000h, 000h, 0EBh, 006h, 0FFh, 025h
        dd  obj3:0x03AC              ; fixup
loc_3_03B8:
        clc    
        ret    
        db  000h, 000h
sub_3_03BC:
        mov    dword ptr [0x6d8], 0x9c       ; -> loc_3_06D8
        mov    dword ptr [0x6dc], 0x68       ; -> loc_3_06DC
        push   0x6cc                         ; -> loc_3_06CC
        VxDCall  BLOCKDEV.Service_0007h          ; cd2007001000
        add    esp, 4
        cmp    word ptr [0x6f8], 1           ; -> loc_3_06F8
        jne    loc_3_03E9
        ret    
loc_3_03E9:
        stc    
        ret    
        db  000h
sub_3_03EC:
        mov    ecx, 6
        mov    edi, 0x3dc                    ; -> obj1:0x03DC
        mov    esi, 0x1b9                    ; -> loc_1_01B9
loc_3_03FB:
        movzx  edx, word ptr [edi]
        VMMCall  VMM.Install_IO_Handler          ; cd2096000100
        jb     loc_3_040C
        add    edi, 2
        loop   loc_3_03FB
        clc    
loc_3_040C:
        ret    
        db  000h, 000h, 000h, 041h, 000h, 001h, 000h
loc_3_0414:
        xchg   esi, eax
        add    byte ptr [ecx], al
        add    cl, ch                        ; -> loc_3_04E9
        add    al, 0
        add    cl, dh                        ; -> loc_3_04F1
        add    al, 0
        add    byte ptr [ecx + 2], bh
loc_3_0425:
        mov    eax, dword ptr [ecx*4 + 0x40c] ; -> loc_3_040C
        mov    esi, dword ptr [ecx*4 + 0x414] ; -> loc_3_0414
        VMMCall  VMM.Hook_Device_Service         ; cd2090000100
        jb     loc_3_046F
        bts    dword ptr [0x4b4], ecx        ; -> loc_1_04B4
        loop   loc_3_0425
        mov    eax, 0x30001
        mov    esi, 0x2f7                    ; -> loc_1_02F7
        VMMCall  VMM.Hook_Device_Service         ; cd2090000100
        mov    eax, 0x30011
        mov    esi, 0x341                    ; -> loc_1_0341
        VMMCall  VMM.Hook_Device_Service         ; cd2090000100
        mov    eax, 1
        call   sub_1_04B8
loc_3_046F:
        pushfd 
        mov    ecx, 2
loc_3_0475:
        btr    dword ptr [0x4b4], ecx        ; -> loc_1_04B4
        jae    loc_3_0492
        mov    eax, dword ptr [ecx*4 + 0x40c] ; -> loc_3_040C
        mov    esi, dword ptr [ecx*4 + 0x414] ; -> loc_3_0414
        VMMCall  VMM.Unhook_Device_Service       ; cd201c010100
loc_3_0492:
        loop   loc_3_0475
        mov    dword ptr [0x4b4], 0          ; -> loc_1_04B4
        push   8
        push   0
        push   0
        push   0
        push   0
        push   0
        push   1
        push   2
        VMMCall  VMM._PageAllocate               ; cd2053000100
        add    esp, 0x20
        or     edx, edx
        je     loc_3_04D3
        mov    dword ptr [0x4ac], 1          ; -> loc_1_04AC
        add    edx, 0x2000
        mov    dword ptr [0x4a4], edx        ; -> loc_1_04A4
        popfd  
        ret    
loc_3_04D3:
        VMMCall  VMM.Fatal_Memory_Error          ; cd20bf000100
loc_3_04D9:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_04DD:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        jmp    loc_3_04E9
        db  0FFh, 025h
        dd  loc_3_04D9               ; fixup
loc_3_04E9:
        jmp    loc_3_04F1
        db  0FFh, 025h
        dd  loc_3_04DD               ; fixup
loc_3_04F1:
        xor    esi, esi
        clc    
loc_3_04F4:
        ret    
        db  000h, 000h, 000h, 041h, 000h, 001h, 000h, 001h, 000h, 003h, 000h, 081h, 000h, 001h, 000h, 07Fh
        db  000h, 001h, 000h
loc_3_0508:
        add    byte ptr [eax], 1
        add    byte ptr [ebp + eax + 0x5b40000], ch ; -> loc_3_05AC
        add    byte ptr [eax], al
        mov    esp, 0xc4000005               ; -> loc_3_05C4
        add    eax, 0x5cc0000                ; -> loc_3_05CC
        add    byte ptr [eax], al
loc_3_0520:
        mov    eax, 0x24
        mov    esi, 0x36f                    ; -> loc_1_036F
        VMMCall  VMM.Hook_V86_Int_Chain          ; cd2041000100
        mov    ecx, 5
loc_3_0535:
        mov    eax, dword ptr [ecx*4 + 0x4f4] ; -> loc_3_04F4
        mov    esi, dword ptr [ecx*4 + 0x508] ; -> loc_3_0508
        VMMCall  VMM.Hook_Device_Service         ; cd2090000100
        jb     loc_3_055F
        bts    dword ptr [0x4c8], ecx        ; -> loc_1_04C8
        loop   loc_3_0535
        mov    eax, 1
        call   sub_1_04CC
loc_3_055F:
        pushfd 
        mov    ecx, 5
loc_3_0565:
        btr    dword ptr [0x4c8], ecx        ; -> loc_1_04C8
        jae    loc_3_0582
        mov    eax, dword ptr [ecx*4 + 0x4f4] ; -> loc_3_04F4
        mov    esi, dword ptr [ecx*4 + 0x508] ; -> loc_3_0508
        VMMCall  VMM.Unhook_Device_Service       ; cd201c010100
loc_3_0582:
        loop   loc_3_0565
        mov    dword ptr [0x4c8], 0          ; -> loc_1_04C8
        popfd  
        ret    
        db  000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h, 000h
        db  000h, 000h, 000h, 000h, 0EBh, 006h, 0FFh, 025h
        dd  obj3:0x0590              ; fixup
loc_3_05AC:
        jmp    loc_3_05B4
        db  0FFh, 025h
        dd  obj3:0x0594              ; fixup
loc_3_05B4:
        jmp    loc_3_05BC
        db  0FFh, 025h
        dd  obj3:0x0598              ; fixup
loc_3_05BC:
        jmp    loc_3_05C4
        db  0FFh, 025h
        dd  obj3:0x059C              ; fixup
loc_3_05C4:
        jmp    loc_3_05CC
        db  0FFh, 025h
        dd  obj3:0x05A0              ; fixup
loc_3_05CC:
        xor    esi, esi
        clc    
        ret    
        db  000h, 000h, 000h, 000h, 000h, 004h, 000h, 022h
loc_3_05D8:
        add    dword ptr [ebx], eax
        add    byte ptr [eax], al
loc_3_05DC:
        push   esi
        inc    esi
        dec    ecx
        dec    esi
        push   esp
        inc    esp
        and    byte ptr [eax], ah
        add    byte ptr [eax], al
        db  000h, 080h
loc_3_05E8:
        pop    ebx
        add    byte ptr [eax], al
        add    byte ptr [0x6000003], al      ; -> loc_4_0305
        add    eax, dword ptr [eax]
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [esi + 0x65], dh
        jb     loc_3_0660
        push   eax
        add    byte ptr [eax], al
        add    byte ptr [ecx], dh
        jbe    0x68a
        push   edx
        xor    dh, byte ptr [esi + 0x73]
        push   edx
        xor    esi, dword ptr [esi + 0x73]
        push   edx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        daa    
loc_3_0628:
        add    dword ptr [ebx], eax
        add    byte ptr [eax], al
loc_3_062C:
        inc    esi
        inc    edx
        push   edi
        pop    edi
        push   esi
        pop    eax
        inc    esp
        and    byte ptr [eax], al
        add    byte ptr [eax], al
        db  080h
loc_3_0638:
        add    eax, dword ptr fs:[eax]
        add    byte ptr [esi + eax + 0x6aa0000], al ; -> loc_4_0684
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        jbe    0x6c3
        jb     loc_3_06B0
loc_3_0660:
        push   eax
        add    byte ptr [eax], al
        add    byte ptr [ecx], dh
        jbe    loc_3_06DA
        push   edx
        xor    dh, byte ptr [esi + 0x73]
        push   edx
        xor    esi, dword ptr [esi + 0x73]
        push   edx
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [esp + ebx*4], al
        sub    dword ptr [eax], eax
        add    al, 0
        db  000h
loc_3_067C:
        inc    ebx
        dec    ebp
        push   ebx
        push   edi
        push   esp
        inc    ecx
        push   eax
        inc    ebp
        add    byte ptr [eax], al
        add    byte ptr [eax + 0x2b4], al    ; -> loc_1_02B4
        and    al, 6
        add    byte ptr [eax], al
        push   es
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        jbe    0x713
        jb     loc_3_0700
loc_3_06B0:
        push   eax
        add    byte ptr [eax], al
        add    byte ptr [ecx], dh
        jbe    0x72a
        push   edx
        xor    dh, byte ptr [esi + 0x73]
        push   edx
        xor    esi, dword ptr [esi + 0x73]
        push   edx
loc_3_06C0:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_06C4:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_06C8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_06CC:
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        pop    eax
        add    al, 0
        add    byte ptr [eax], al
loc_3_06D8:
        add    byte ptr [eax], al
loc_3_06DA:
        add    byte ptr [eax], al
loc_3_06DC:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        push   esi
        inc    esi
        inc    edx
        inc    ecx
        inc    ebx
        dec    ebx
        push   ebp
        push   eax
        and    byte ptr [esi + 0x58], dl
        inc    esp
        and    byte ptr [eax], ah
        and    byte ptr [eax], ah
        add    byte ptr [eax], al
        and    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_06F8:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        add    byte ptr [eax], al
loc_3_0700:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0x787  R-X
VFBACKUP_Lock_NEC:
        pushal 
        cli    
        mov    edx, dword ptr [0x400]        ; -> loc_1_0400
        mov    dword ptr [edx], ecx
        mov    edx, dword ptr [0x3f4]        ; -> loc_1_03F4
        mov    ebp, dword ptr [edx]
        xor    al, 1
        shl    eax, 8
        test   byte ptr [0x3f0], 0x10        ; -> obj1:0x03F0
        je     loc_4_01E3
        cmp    ebp, ebx
        je     loc_4_012B
        or     ebp, ebp
        jne    loc_4_01EA
        mov    dword ptr [edx], ebx
        sti    
        push   eax
        xor    ecx, ecx
        sub    esp, 0xc
        mov    word ptr [esp], 7
        mov    dword ptr [esp + 8], 0x40000000
        push   esp
        call   sub_1_0068
        mov    ebx, dword ptr [esp + 8]
        movzx  eax, word ptr [esp + 6]
        add    esp, 0x10
        or     eax, eax
        jne    loc_4_007E
        test   byte ptr [ebx + 0x23], 2
        je     loc_4_006E
        test   byte ptr [ebx + 0x24], 8
        jne    loc_4_007E
loc_4_006E:
        push   ecx
        push   1
        push   0
        VxDCall  IFSMgr._VolFlush                ; cd204c004000
        add    esp, 8
        pop    ecx
        inc    ecx
loc_4_007E:
        bt     ecx, 0
        rcl    ecx, 1
        sub    esp, 0xc
        mov    word ptr [esp], 7
        mov    dword ptr [esp + 8], 0x40000000
        push   esp
        call   sub_1_0068
        mov    ebx, dword ptr [esp + 8]
        movzx  eax, word ptr [esp + 6]
        add    esp, 0x10
        or     eax, eax
        jne    loc_4_00BE
        or     ecx, 1
        test   byte ptr [ebx + 0x23], 2
        je     loc_4_00BE
        test   byte ptr [ebx + 0x24], 8
        je     loc_4_00BE
        and    ecx, 2
loc_4_00BE:
        test   ecx, 1
        je     loc_4_00D5
        push   ecx
        push   1
        push   1
        VxDCall  IFSMgr._VolFlush                ; cd204c004000
        add    esp, 8
        pop    ecx
loc_4_00D5:
        pop    eax
        mov    edx, dword ptr [0x3f4]        ; -> loc_1_03F4
        mov    ebx, dword ptr [edx]
        push   ecx
        mov    cl, 1
        VxDCall  BLOCKDEV.Service_0009h          ; cd2009001000
        pop    ecx
        jb     loc_4_01EA
        or     eax, 0x40000000
        push   ecx
        mov    cl, 1
        VxDCall  BLOCKDEV.Service_0009h          ; cd2009001000
        pop    ecx
        and    eax, 0xbfffffff
        cmp    dword ptr [0x418], 1          ; -> obj1:0x0418
        je     loc_4_012B
        inc    eax
        push   ecx
        mov    cl, 1
        VxDCall  BLOCKDEV.Service_0009h          ; cd2009001000
        pop    ecx
        jb     loc_4_01EA
        or     eax, 0x40000000
        push   ecx
        mov    cl, 1
        VxDCall  BLOCKDEV.Service_0009h          ; cd2009001000
        pop    ecx
loc_4_012B:
        mov    ebp, eax
        mov    eax, dword ptr [0x3f8]        ; -> loc_1_03F8
        cmp    dword ptr [eax], 0
        sti    
        jne    loc_4_0179
        xor    ebx, ebx
loc_4_013A:
        xor    ebx, 1
        bt     ecx, ebx
        pushfd 
        xor    ebx, 1
        popfd  
        jae    loc_4_0173
        push   ecx
        call   sub_4_04E4
        pop    ecx
        or     esi, esi
        je     loc_4_01EA
        push   ecx
        VxDCall  BLOCKDEV.BlockDev_Send_Command  ; cd2004001000
        pop    ecx
        cmp    word ptr [esi + 6], 0x10
        jb     loc_4_016C
        cmp    word ptr [esi + 6], 0x16
        jne    loc_4_0179
loc_4_016C:
        push   ecx
        call   sub_4_0568
        pop    ecx
loc_4_0173:
        inc    ebx
        cmp    bl, 1
        jbe    loc_4_013A
loc_4_0179:
        mov    ebx, dword ptr [0x3f4]        ; -> loc_1_03F4
        mov    ebx, dword ptr [ebx]
        push   ebx
        test   ebp, 0x100
        je     loc_4_018D
        mov    ebx, dword ptr [ebx + 0x14]
loc_4_018D:
        cli    
        mov    ecx, 6
        mov    edi, 0x3dc                    ; -> obj1:0x03DC
loc_4_0198:
        movzx  edx, word ptr [edi]
        VMMCall  VMM.Disable_Local_Trapping      ; cd209a000100
        add    edi, 2
        loop   loc_4_0198
        pop    ebx
        mov    ecx, dword ptr [0]            ; -> obj1:0x0000
        jecxz  loc_4_01B1
        call   ecx
loc_4_01B1:
        mov    eax, dword ptr [0x3f8]        ; -> loc_1_03F8
        mov    dword ptr [eax], ebx
        and    ebp, 0x100
        mov    eax, dword ptr [0x3fc]        ; -> loc_1_03FC
        and    dword ptr [eax], 0xfffffefe
        or     dword ptr [eax], ebp
        mov    ecx, dword ptr [0x400]        ; -> loc_1_0400
        cmp    dword ptr [ecx], 0
        je     loc_4_01E0
        mov    eax, dword ptr [0x414]        ; -> obj1:0x0414
        mov    dword ptr [esp + 0x1c], eax
        clc    
loc_4_01E0:
        sti    
loc_4_01E1:
        popal  
        ret    
loc_4_01E3:
        sti    
        mov    ebp, eax
        mov    dword ptr [edx], ebx
        jmp    loc_4_0179
loc_4_01EA:
        mov    dword ptr [edx], ebp
        xor    eax, eax
        xor    ecx, ecx
        VxDCall  BLOCKDEV.Service_0009h          ; cd2009001000
        inc    eax
        xor    ecx, ecx
        VxDCall  BLOCKDEV.Service_0009h          ; cd2009001000
        stc    
        jmp    loc_4_01E1
VFBACKUP_Lock_All_Ports:
        mov    edx, dword ptr [0x3f4]        ; -> loc_1_03F4
        cmp    dword ptr [edx], ebx
        stc    
        jne    loc_4_0224
        mov    edx, dword ptr [0x3fc]        ; -> loc_1_03FC
        or     byte ptr [edx], 8
        test   byte ptr [edx + 1], 1
        je     loc_4_021F
        mov    ebx, dword ptr [ebx + 0x14]
loc_4_021F:
        call   sub_1_0185
loc_4_0224:
        ret    
VFBACKUP_UnLock_NEC:
        pushal 
        mov    edx, dword ptr [0x3f4]        ; -> loc_1_03F4
        test   byte ptr [0x3f0], 0x10        ; -> obj1:0x03F0
        je     loc_4_0270
        cmp    dword ptr [edx], ebx
        stc    
        jne    loc_4_02FF
        xor    eax, eax
        xor    ecx, ecx
        VxDCall  BLOCKDEV.Service_0009h          ; cd2009001000
        or     eax, 0x40000000
        VxDCall  BLOCKDEV.Service_0009h          ; cd2009001000
        cmp    dword ptr [0x418], 1          ; -> obj1:0x0418
        je     loc_4_0270
        inc    eax
        xor    ecx, ecx
        VxDCall  BLOCKDEV.Service_0009h          ; cd2009001000
        and    eax, 0xbfffffff
        VxDCall  BLOCKDEV.Service_0009h          ; cd2009001000
loc_4_0270:
        mov    eax, dword ptr [0x3fc]        ; -> loc_1_03FC
        and    byte ptr [eax], 0xf7
        cli    
        mov    ecx, 6
        mov    edi, 0x3dc                    ; -> obj1:0x03DC
        test   dword ptr [eax], 0x100
        je     loc_4_028E
        mov    ebx, dword ptr [ebx + 0x14]
loc_4_028E:
        VMMCall  VMM.Validate_VM_Handle          ; cd2005000100
        jb     loc_4_02A4
loc_4_0296:
        movzx  edx, word ptr [edi]
        VMMCall  VMM.Enable_Local_Trapping       ; cd2098000100
        add    edi, 2
        loop   loc_4_0296
loc_4_02A4:
        mov    edx, dword ptr [0x3f4]        ; -> loc_1_03F4
        mov    dword ptr [edx], 0
        mov    ecx, dword ptr [0]            ; -> obj1:0x0000
        jecxz  loc_4_02BA
        call   ecx
loc_4_02BA:
        mov    ecx, dword ptr [0x400]        ; -> loc_1_0400
        mov    dword ptr [ecx], 0
        test   byte ptr [0x4ac], 8           ; -> loc_1_04AC
        jne    loc_4_02FD
        test   byte ptr [0x4ac], 4           ; -> loc_1_04AC
        je     loc_4_02FD
        xor    edx, edx
        xchg   dword ptr [0x4a8], edx        ; -> loc_1_04A8
        sub    edx, 0x2000
        push   0
        push   edx
        VMMCall  VMM._PageFree                   ; cd2055000100
        add    esp, 8
        or     eax, eax
        je     loc_4_02FD
        and    byte ptr [0x4ac], 0xfb        ; -> loc_1_04AC
loc_4_02FD:
        clc    
        sti    
loc_4_02FF:
        popal  
        ret    
        db  000h, 000h, 000h, 000h
loc_4_0305:
        ret    
loc_4_0306:
        movzx  eax, byte ptr [ebp + 0x1d]
        cmp    al, 4
        ja     0x344
        mov    ecx, dword ptr [0x340]        ; -> loc_4_0340
        jmp    dword ptr cs:[ecx + eax*4]
loc_4_0318:
        push   esi
        add    eax, dword ptr [eax]
        add    byte ptr [ebp + 3], bl        ; -> loc_4_035D
        add    byte ptr [eax], al
        jecxz  loc_4_0325
        add    byte ptr [eax], al
        pop    es
loc_4_0325:
        add    al, 0
        add    byte ptr [ebx + 4], dh        ; -> loc_4_0473
        add    byte ptr [eax], al
loc_4_032C:
        push   esi
        add    eax, dword ptr [eax]
        add    byte ptr [ebp + 3], bl        ; -> loc_4_035D
        add    byte ptr [eax], al
        jecxz  loc_4_0339
        add    byte ptr [eax], al
        pop    es
loc_4_0339:
        add    al, 0
        add    byte ptr [ebx + 4], dh        ; -> loc_4_0473
        add    byte ptr [eax], al
loc_4_0340:
        faddp  st(0)
        lodsd  eax, dword ptr [esi]
        mov    edx, 0x840f103c
        dec    edx
        add    dword ptr [eax], eax
        add    byte ptr [ecx + edx], bh
        je     loc_4_04C4
        jmp    sub_4_0356
sub_4_0356:
        call   sub_1_043C
        ret    
loc_4_035D:
        movzx  eax, byte ptr [ebp + 0x1c]
        and    al, 0xf
        or     eax, eax
        jne    loc_4_0369
        mov    al, 6
loc_4_0369:
        cmp    al, 6
        jne    loc_4_039F
        push   ebx
        push   edi
        mov    eax, dword ptr [0x3f4]        ; -> loc_1_03F4
        cmp    dword ptr [eax], ebx
        jne    loc_4_037D
        call   VFBACKUP_UnLock_NEC
loc_4_037D:
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        mov    ebx, edi
        xor    ecx, ecx
        mov    eax, ecx
        call   0
        pop    edi
        pop    ebx
        mov    word ptr [ebp + 0x1c], 1
        jb     loc_4_039E
        mov    word ptr [ebp + 0x1c], 0
loc_4_039E:
        ret    
loc_4_039F:
        cmp    al, 0xa
        jb     sub_4_0356
        cmp    al, 0xc
        ja     sub_4_0356
        mov    eax, 0x30001
        mov    esi, 0x3d9                    ; -> loc_4_03D9
        VMMCall  VMM.Hook_Device_Service         ; cd2090000100
        call   sub_4_0356
        mov    eax, 0x30001
        mov    esi, 0x3d9                    ; -> loc_4_03D9
        VMMCall  VMM.Unhook_Device_Service       ; cd201c010100
        ret    
loc_4_03CD:
        add    byte ptr [eax], al
        add    byte ptr [eax], al
        jmp    loc_4_03D9
        db  0FFh, 025h
        dd  loc_4_03CD               ; fixup
loc_4_03D9:
        and    byte ptr [edi + 2], 0xfd
        jmp    loc_4_03CD
loc_4_03E3:
        push   edi
        push   ebx
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        mov    ebx, edi
        call   VFBACKUP_UnLock_NEC
        pop    ebx
        pop    edi
        xor    eax, eax
        xchg   dword ptr [0x438], eax        ; -> obj1:0x0438
        or     eax, eax
        je     loc_4_0406
        VxDCall  VPICD.VPICD_Force_Default_Behavior ; cd2011000300
loc_4_0406:
        ret    
loc_4_0407:
        cmp    dword ptr [0x430], 0          ; -> obj1:0x0430
        je     loc_4_046C
        push   0
        push   0
        push   0x92
        push   0x8000
        push   dword ptr [0x434]             ; -> obj1:0x0434
        VMMCall  VMM._BuildDescriptorDWORDs      ; cd207a000100
        add    esp, 0x14
        push   0
        push   1
        push   eax
        push   edx
        push   ebx
        VMMCall  VMM._Allocate_LDT_Selector      ; cd2078000100
        add    esp, 0x14
        or     eax, eax
        je     loc_4_046C
        mov    dword ptr [0x301], eax        ; -> obj4:0x0301
        xchg   edi, eax
        mov    ax, 0x3800
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   edi, eax
        mov    eax, dword ptr [0x301]        ; -> obj4:0x0301
        shl    eax, 0x10
        mov    dword ptr [edi], eax
        mov    eax, dword ptr [0x430]        ; -> obj1:0x0430
        mov    dword ptr [edi + 4], eax
        mov    word ptr [ebp + 0x1c], 0
        jmp    loc_4_0472
loc_4_046C:
        mov    word ptr [ebp + 0x1c], 1
loc_4_0472:
        ret    
loc_4_0473:
        mov    word ptr [ebp + 0x1c], 0
        push   0
        push   dword ptr [0x301]             ; -> obj4:0x0301
        push   ebx
        VMMCall  VMM._Free_LDT_Selector          ; cd2079000100
        add    esp, 0xc
        or     eax, eax
        jne    loc_4_0495
        mov    word ptr [ebp + 0x1c], 1
loc_4_0495:
        ret    
        db  066h, 0C7h, 045h, 01Ch, 001h, 000h, 0A1h
        dd  loc_1_03F4               ; fixup
        db  039h, 018h, 075h, 005h, 0E8h, 07Bh, 0FDh, 0FFh, 0FFh, 053h, 057h, 0CDh, 020h, 008h, 001h, 001h
        db  000h, 08Bh, 0DFh, 033h, 0C9h, 08Bh, 0C1h, 0E8h, 043h, 0FBh, 0FFh, 0FFh, 05Fh, 05Bh, 066h, 089h
        db  04Dh, 01Ch, 0C3h
loc_4_04C4:
        mov    word ptr [ebp + 0x1c], 1
        xor    ecx, ecx
        push   edi
        push   ebx
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        mov    ebx, edi
        call   VFBACKUP_UnLock_NEC
        pop    ebx
        pop    edi
        jb     loc_4_04E3
        mov    word ptr [ebp + 0x1c], cx
loc_4_04E3:
        ret    
sub_4_04E4:
        push   ebx
        sub    esp, 0xc
        mov    word ptr [esp], 7
        mov    dword ptr [esp + 8], ebx
        push   esp
        call   sub_1_0068
        mov    ebx, dword ptr [esp + 8]
        movzx  eax, word ptr [esp + 6]
        add    esp, 0x10
        or     eax, eax
        jne    loc_4_0564
        sub    esp, 0x10
        mov    esi, esp
        mov    ecx, dword ptr [ebx + 4]
        mov    eax, ecx
        add    ecx, 0xbc
        add    eax, 0x64
        mov    dword ptr [esi + 6], eax
        mov    word ptr [esi + 4], cx
        mov    byte ptr [esi + 0xe], 0
        mov    word ptr [esi], 2
        push   esi
        call   sub_1_0068
        mov    esi, dword ptr [esi + 0xa]
        add    esi, eax
        mov    dword ptr [esi + 0x20], eax
        add    esp, 0x14
        or     esi, esi
        je     loc_4_0563
        xor    eax, eax
        mov    dword ptr [esi], eax
        mov    dword ptr [esi + 0x44], eax
        mov    dword ptr [esi + 0x18], eax
        mov    dword ptr [esi + 8], 0x14508
        mov    dword ptr [esi], 0
        mov    word ptr [esi + 4], 0x10
        pop    ebx
        mov    byte ptr [esi + 0x4d], bl
loc_4_0563:
        ret    
loc_4_0564:
        pop    ebx
        xor    esi, esi
        ret    
sub_4_0568:
        sub    esp, 8
        mov    edi, esp
        sub    esi, dword ptr [esi + 0x20]
        mov    dword ptr [edi + 4], esi
        mov    word ptr [edi], 4
        push   edi
        call   sub_1_0068
        add    esp, 0xc
        ret    
        db  000h
loc_4_0584:
        push   ecx
        mov    ecx, dword ptr [0x3f4]        ; -> loc_1_03F4
        mov    ecx, dword ptr [ecx]
        jecxz  loc_4_05A1
        push   eax
        mov    eax, dword ptr [0x3fc]        ; -> loc_1_03FC
        test   dword ptr [eax], 0x100
        pop    eax
        je     loc_4_05A1
        mov    ecx, dword ptr [ecx + 0x14]
loc_4_05A1:
        cmp    ebx, ecx
        jne    loc_4_05C0
        xor    edi, edi
loc_4_05A7:
        mov    ecx, dword ptr [0x3fc]        ; -> loc_1_03FC
        or     dword ptr [ecx], edi
        mov    dword ptr [0x49c], 0x1e       ; -> obj1:0x049C
        pop    ecx
        VxDCall  VDMAD.VDMAD_Default_Handler     ; cd2012800400
loc_4_05C0:
        push   eax
        xor    ecx, ecx
        xor    eax, eax
        inc    eax
        call   0
        mov    edi, 1
        pop    eax
        jae    loc_4_05A7
        VMMCall  VMM.Crash_Cur_VM                ; cd202f000100
sub_4_05D9:
        push   edi
        sub    esp, 0x20
        mov    edi, esp
        mov    word ptr [edi], ax
        mov    word ptr [edi + 2], 2
        mov    dword ptr [edi + 4], 0x129    ; -> loc_1_0129
        mov    dword ptr [edi + 8], 0
        mov    dword ptr [edi + 0xc], 0x177  ; -> loc_1_0177
        mov    dword ptr [edi + 0x10], 0
        mov    dword ptr [edi + 0x14], 0
        mov    dword ptr [edi + 0x18], 0x1f4
        VxDCall  VPICD.VPICD_Virtualize_IRQ      ; cd2001000300
        mov    edi, eax
        lahf   
        add    esp, 0x20
        rcr    ah, 1
        mov    eax, edi
        pop    edi
        ret    
loc_4_0624:
        clc    
        ret    
loc_4_0626:
        bts    dword ptr [0x4ac], 2          ; -> loc_1_04AC
        jb     loc_4_0659
        push   8
        push   0
        push   0
        push   0
        push   0
        push   0
        push   1
        push   2
        VMMCall  VMM._PageAllocate               ; cd2053000100
        add    esp, 0x20
        or     edx, edx
        je     sub_4_067E
        add    edx, 0x2000
        mov    dword ptr [0x4a8], edx        ; -> loc_1_04A8
loc_4_0659:
        bts    dword ptr [0x4ac], 3          ; -> loc_1_04AC
        jb     sub_4_067E
        xchg   dword ptr [0x4a8], esp        ; -> loc_1_04A8
        call   sub_4_067E
        xchg   dword ptr [0x4a8], esp        ; -> loc_1_04A8
        btr    dword ptr [0x4ac], 3          ; -> loc_1_04AC
        clc    
        ret    
sub_4_067E:
        jmp    loc_1_04A0
loc_4_0684:
        clc    
        ret    
loc_4_0686:
        sbb    eax, dword ptr [edi]
        add    byte ptr [eax], al
        and    al, byte ptr [edi]
        add    byte ptr [eax], al
        pop    esi
        pop    es
        add    byte ptr [eax], al
        xchg   byte ptr [edi], al
        add    byte ptr [eax], al
        mov    byte ptr [esi], 0
        add    dh, al                        ; -> loc_4_06C6
        push   es
        add    byte ptr [eax], al
        mov    byte ptr [esi], 0
        add    dh, al                        ; -> loc_4_06C6
        push   es
        add    byte ptr [eax], al
        mov    byte ptr [esi], 0
        add    byte ptr [esi - 0x7d], ah
        sub    al, 0xfe
        movzx  edx, byte ptr [ebp + 0x1d]
        cmp    dl, 8
        ja     loc_4_06BF
        jmp    loc_4_0686
loc_4_06BF:
        or     word ptr [ebp + 0x2c], 1
        clc    
        ret    
loc_4_06C6:
        jmp    loc_1_04C4
loc_4_06CC:
        mov    dword ptr [0x4c0], 0          ; -> loc_1_04C0
        sub    esp, 0x6c
        push   edi
        lea    edi, [esp + 4]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083000100
        movzx  ecx, word ptr [0x4be]         ; -> loc_1_04BE
        movzx  edx, word ptr [0x4bc]         ; -> loc_1_04BC
        VMMCall  VMM.Simulate_Far_Call           ; cd2048000100
        VMMCall  VMM.Resume_Exec                 ; cd2085000100
        VMMCall  VMM.End_Nest_Exec               ; cd2086000100
        push   esi
        lea    esi, [esp + 4]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        add    esp, 0x6c
        ret    
loc_4_071B:
        mov    word ptr [ebp + 0x1c], 0x200
        ret    
loc_4_0722:
        xor    ecx, ecx
        mov    eax, ecx
        push   ebx
        push   edi
        mov    eax, dword ptr [0x3f4]        ; -> loc_1_03F4
        cmp    dword ptr [eax], ebx
        jne    loc_4_0736
        call   VFBACKUP_UnLock_NEC
loc_4_0736:
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        mov    ebx, edi
        call   0
        pop    edi
        pop    ebx
        jb     loc_4_0758
        movzx  eax, word ptr [ebp + 0x18]
        shl    eax, 0x10
        mov    ax, word ptr [ebp + 0x14]
        mov    dword ptr [0x4bc], eax        ; -> loc_1_04BC
        ret    
loc_4_0758:
        or     word ptr [ebp + 0x2c], 1
        ret    
loc_4_075E:
        push   edi
        push   ebx
        VMMCall  VMM.Get_Cur_Thread_Handle       ; cd2008010100
        mov    ebx, edi
        call   VFBACKUP_UnLock_NEC
        pop    ebx
        pop    edi
        jb     loc_4_0758
        mov    dx, word ptr [0x3e0]          ; -> loc_1_03E0
        mov    al, 0xc
        out    dx, al
        mov    dword ptr [0x4bc], 0          ; -> loc_1_04BC
        ret    
loc_4_0786:
        ret    
VxD__LTEXT ENDS

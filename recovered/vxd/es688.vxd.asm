; AUDDRV -- reconstructed 32-bit disassembly (VxD / LE format)
; 7 objects, 4300 instructions recovered by recursive descent

Declare_Virtual_Device AUDDRV, 4, 0, AUDDRV_Control, 033F4h, Undefined_Init_Order
;   DDK version 0400h   init_order 080000000h   flags 00000h

VxD__LTEXT SEGMENT   ; object 1  vsize=0x538  R-X
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
        dw  033F4h                      ; DDB_Req_Device_Number
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
        add    byte ptr [edx + ebx], dl
        xor    dh, byte ptr [esi]
        cmp    byte ptr [esi], bh
        add    ah, cl
AUDDRV_Control:
        cmp    eax, 8
        je     loc_5_0C7C
loc_1_010D:
        cmp    eax, 0xb
        je     loc_5_0C18
loc_1_0116:
        cmp    eax, 3
        je     loc_5_0C7C
loc_1_011F:
        cmp    eax, 0x1b
        je     loc_7_0000
loc_1_0128:
        cmp    eax, 0x1c
        je     loc_3_0000
loc_1_0131:
        cmp    eax, 0x22
        je     loc_4_068F
loc_1_013A:
        clc    
        ret    
sub_1_013C:
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
        je     loc_1_0193
        cli    
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_1_0193
loc_1_016F:
        mov    edi, dword ptr [eax]
        test   dword ptr [ebp + 0xc], 1
        jne    loc_1_0183
        cmp    dword ptr [edx + edi], ebx
        jne    loc_1_0189
loc_1_017F:
        popfd  
        clc    
        jmp    loc_1_0197
loc_1_0183:
        cmp    word ptr [edx + edi], bx
        je     loc_1_017F
loc_1_0189:
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        je     loc_1_0193
        jmp    loc_1_016F
loc_1_0193:
        popfd  
        xor    edi, edi
        stc    
loc_1_0197:
        pop    esi
        pop    edx
        pop    ebx
        pop    eax
        pop    ebp
        ret    
sub_1_019D:
        push   eax
        push   ebx
        pushfd 
        add    ebx, dword ptr [0]            ; -> obj1:0x00D0
        mov    esi, dword ptr [ebx]
        or     esi, esi
        je     loc_1_01CA
        cli    
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_1_01CA
loc_1_01B5:
        cmp    dword ptr [eax + 4], edi
        jne    loc_1_01C0
        mov    esi, eax
        popfd  
        clc    
        jmp    loc_1_01CE
loc_1_01C0:
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        je     loc_1_01CA
        jmp    loc_1_01B5
loc_1_01CA:
        popfd  
        xor    esi, esi
        stc    
loc_1_01CE:
        pop    ebx
        pop    eax
        ret    
        db  02Eh, 08Bh, 0C0h, 0EBh, 006h, 0FFh, 025h
        dd  loc_1_00D8               ; fixup
loc_1_01DC:
        pushal 
        pushfd 
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_1_022B
        mov    esi, dword ptr [0]            ; -> obj1:0x00D4
        or     esi, esi
        je     loc_1_022B
        cli    
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        popfd  
        pushfd 
        or     eax, eax
        je     loc_1_022B
loc_1_01FD:
        xor    ecx, ecx
        mov    edi, dword ptr [eax]
        cmp    dword ptr [edi + 0x2c], ebx
        jne    loc_1_0209
        or     ecx, 1
loc_1_0209:
        cmp    dword ptr [edi + 0x34], ebx
        jne    loc_1_0211
        or     ecx, 2
loc_1_0211:
        jecxz  loc_1_021C
        push   eax
        mov    eax, ecx
        call   sub_5_0CDD
loc_1_021B:
        pop    eax
loc_1_021C:
        cli    
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        popfd  
        pushfd 
        or     eax, eax
        je     loc_1_022B
        jmp    loc_1_01FD
loc_1_022B:
        popfd  
        popal  
        jmp    loc_1_00D8
        db  090h
loc_1_0234:
        VxDCall  VDMAD.VDMAD_Default_Handler     ; cd2012800400
        mov    eax, eax
loc_1_023C:
        cld    
        push   edi
        push   edx
        push   ebx
        mov    edi, edx
        mov    ebx, dword ptr [edi + 0x2c]
        or     ebx, ebx
        je     loc_1_0250
        cmp    word ptr [edi + 0x70], 0
        je     loc_1_0262
loc_1_0250:
        push   eax
        movzx  edx, word ptr [edi + 6]
        add    edx, 0xe
        in     al, dx
        pop    eax
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
        jmp    loc_1_0268
loc_1_0262:
        VxDCall  VPICD.VPICD_Set_Int_Request     ; cd2002000300
loc_1_0268:
        pop    ebx
        pop    edx
        pop    edi
        clc    
        ret    
        db  02Eh, 08Bh, 0C0h
loc_1_0270:
        VxDCall  VPICD.VPICD_Clear_Int_Request   ; cd2003000300
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
        clc    
        ret    
        db  08Bh, 0C0h
loc_1_0280:
        push   edi
        push   1
        push   eax
        call   sub_1_013C
        add    esp, 8
        or     edi, edi
        je     loc_1_02B1
        cmp    dword ptr [edi + 0x2c], ebx
        jne    loc_1_02A7
        jecxz  loc_1_029F
loc_1_0297:
        VxDCall  VPICD.VPICD_Physically_Mask     ; cd2008000300
        jmp    loc_1_02B1
loc_1_029F:
        VxDCall  VPICD.VPICD_Physically_Unmask   ; cd2009000300
        jmp    loc_1_02B1
loc_1_02A7:
        test   word ptr [edi + 4], 0x8000
        jne    loc_1_029F
        je     loc_1_0297
loc_1_02B1:
        pop    edi
        clc    
        ret    
sub_1_02B4:
        push   ecx
        push   edx
        add    edx, 0xe
        mov    ecx, 0x5dc
loc_1_02BE:
        in     al, dx
        or     al, al
        js     loc_1_02C8
        loop   loc_1_02BE
        stc    
        jmp    loc_1_02CD
loc_1_02C8:
        sub    edx, 4
        in     al, dx
        clc    
loc_1_02CD:
        pop    edx
        pop    ecx
        ret    
sub_1_02D0:
        push   eax
        push   ecx
        push   edx
        add    edx, 0xc
        push   eax
        mov    ecx, 0xa0000
loc_1_02DC:
        in     al, dx
        or     al, al
        jns    loc_1_02E7
        loop   loc_1_02DC
        pop    eax
        stc    
        jmp    loc_1_02EA
loc_1_02E7:
        pop    eax
        out    dx, al
        clc    
loc_1_02EA:
        pop    edx
        pop    ecx
        pop    eax
        ret    
sub_1_02EE:
        push   eax
        push   edx
        add    edx, 6
        mov    al, 1
        out    dx, al
        in     al, dx
        in     al, dx
        in     al, dx
        sub    edx, 6
        jmp    loc_1_02FE
loc_1_02FE:
        jmp    loc_1_0300
loc_1_0300:
        add    edx, 6
        xor    al, al
        out    dx, al
        sub    edx, 6
        call   sub_1_02B4
        pop    edx
        pop    eax
        ret    
sub_1_0311:
        push   eax
        call   sub_1_02EE
        jb     loc_1_0329
        mov    al, 0x10
        call   sub_1_02D0
        jb     loc_1_0329
        mov    al, 0x80
        call   sub_1_02D0
loc_1_0329:
        pop    eax
        ret    
sub_1_032B:
        push   eax
        mov    al, 0xc6
        call   sub_1_02D0
        jb     loc_1_0359
        mov    al, 0xc3
        call   sub_1_02D0
        jb     loc_1_0359
        call   sub_1_02B4
        jb     loc_1_0359
        test   al, 1
        jne    loc_1_0359
        mov    al, 0xc2
        call   sub_1_02D0
        jb     loc_1_0359
        mov    al, 1
        call   sub_1_02D0
loc_1_0359:
        pop    eax
        ret    
sub_1_035B:
        push   eax
        push   edx
        push   edi
        push   esi
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_1_0394
        cld    
        movzx  edx, word ptr [edi + 6]
        call   sub_1_032B
        jb     loc_1_0378
        call   sub_1_0451
loc_1_0378:
        add    dx, 4
        add    edi, 0x50
        mov    esi, 0                        ; -> obj1:0x00FC
loc_1_0384:
        lodsb  al, byte ptr [esi]
        or     al, al
        je     loc_1_0394
        out    dx, al
        inc    dx
        mov    al, byte ptr [edi]
        inc    edi
        out    dx, al
        dec    dx
        jmp    loc_1_0384
loc_1_0394:
        pop    esi
        pop    edi
        pop    edx
        pop    eax
        clc    
        ret    
sub_1_039A:
        push   eax
        push   edx
        push   edi
        push   esi
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_1_03C5
        cld    
        movzx  edx, word ptr [edi + 6]
        add    dx, 4
        add    edi, 0x50
        mov    esi, 0                        ; -> obj1:0x00FC
loc_1_03B7:
        lodsb  al, byte ptr [esi]
        or     al, al
        je     loc_1_03C5
        out    dx, al
        inc    dx
        in     al, dx
        stosb  byte ptr es:[edi], al
        dec    dx
        jmp    loc_1_03B7
loc_1_03C5:
        pop    esi
        pop    edi
        pop    edx
        pop    eax
        clc    
        ret    
sub_1_03CB:
        push   eax
        push   ecx
        push   esi
        cld    
        xor    cl, cl
loc_1_03D1:
        lodsw  ax, word ptr [esi]
        cmp    ax, bx
        je     loc_1_03E1
        inc    cl
        cmp    ax, -1
        jne    loc_1_03D1
        stc    
loc_1_03E1:
        mov    bl, cl
        pop    esi
        pop    ecx
        pop    eax
        ret    
sub_1_03E7:
        push   eax
        push   ebx
        call   sub_1_03CB
        jb     loc_1_0414
        mov    al, bl
        shl    al, 2
        or     al, 0x10
        push   eax
        mov    al, 0xc6
        call   sub_1_02D0
        pop    eax
        jb     loc_1_0414
        push   eax
        mov    al, 0xb1
        call   sub_1_02D0
        pop    eax
        jb     loc_1_0414
        call   sub_1_02D0
        jae    loc_1_0417
loc_1_0414:
        stc    
        jmp    loc_1_0418
loc_1_0417:
        clc    
loc_1_0418:
        pop    ebx
        pop    eax
        ret    
sub_1_041B:
        push   eax
        push   ebx
        call   sub_1_03CB
        jb     loc_1_044A
        mov    al, bl
        inc    al
        shl    al, 2
        or     al, 0x10
        push   eax
        mov    al, 0xc6
        call   sub_1_02D0
        pop    eax
        jb     loc_1_044A
        push   eax
        mov    al, 0xb2
        call   sub_1_02D0
        pop    eax
        jb     loc_1_044A
        call   sub_1_02D0
        jae    loc_1_044D
loc_1_044A:
        stc    
        jmp    loc_1_044E
loc_1_044D:
        clc    
loc_1_044E:
        pop    ebx
        pop    eax
        ret    
sub_1_0451:
        push   eax
        push   edx
        movzx  edx, word ptr [edi + 6]
        mov    al, 0xc6
        call   sub_1_02D0
        jb     loc_1_0498
        mov    al, 0xb1
        call   sub_1_02D0
        jb     loc_1_0498
        mov    al, byte ptr [edi + 0x5a]
        call   sub_1_02D0
        jb     loc_1_0498
        mov    al, 0xb2
        call   sub_1_02D0
        jb     loc_1_0498
        mov    al, byte ptr [edi + 0x5b]
        call   sub_1_02D0
        jb     loc_1_0498
        mov    al, 0xe1
        call   sub_1_02D0
        call   sub_1_02B4
        call   sub_1_02B4
        clc    
loc_1_0498:
        pop    edx
        pop    eax
        ret    
sub_1_049B:
        push   eax
        push   edx
        movzx  edx, word ptr [edi + 6]
        call   sub_1_02EE
        jb     loc_1_04EA
        mov    al, 0xc6
        call   sub_1_02D0
        jb     loc_1_04EA
        mov    al, 0xc0
        call   sub_1_02D0
        jb     loc_1_04EA
        mov    al, 0xb1
        call   sub_1_02D0
        jb     loc_1_04EA
        call   sub_1_02B4
        jb     loc_1_04EA
        mov    byte ptr [edi + 0x5a], al
        mov    al, 0xc0
        call   sub_1_02D0
        jb     loc_1_04EA
        mov    al, 0xb2
        call   sub_1_02D0
        jb     loc_1_04EA
        call   sub_1_02B4
        jb     loc_1_04EA
        mov    byte ptr [edi + 0x5b], al
        clc    
loc_1_04EA:
        pop    edx
        pop    eax
        ret    
sub_1_04ED:
        pushal 
        mov    edi, dword ptr [esi + 4]
        movzx  edx, word ptr [edi + 6]
        pushfd 
        cli    
        popfd  
        clc    
        popal  
        ret    
sub_1_04FB:
        pushal 
        mov    edi, dword ptr [esi + 4]
        movzx  edx, word ptr [edi + 6]
        pushfd 
        cli    
        popfd  
        clc    
        popal  
        ret    
sub_1_0509:
        push   eax
        push   ecx
        mov    ecx, 6
loc_1_0510:
        in     al, dx
        jmp    loc_1_0513
loc_1_0513:
        loop   loc_1_0510
        pop    ecx
        pop    eax
        ret    
sub_1_0518:
        push   ebx
        push   edx
        push   eax
        mov    eax, ebx
        shl    ah, 1
        add    dl, ah
        out    dx, al
        sub    dl, ah
        call   sub_1_0509
        pop    eax
        inc    dl
        out    dx, al
        call   sub_1_0509
        pop    edx
        pop    ebx
        ret    
        db  0CCh, 0CCh, 0CCh
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
        je     loc_2_00B8
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
        cmp    byte ptr ss:[eax], bh
        and    byte ptr [ecx + ebp*2 + 0x72], al
loc_2_00B8:
        arpl   word ptr gs:[eax + 0x53], si
        outsd  dx, dword ptr [esi]
        jne    0x12e
        and    byte ptr fs:[edx + esi*2 + 0x69], al
        jbe    0x12c
        jb     0xc9
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 3  vsize=0x151  R-X
loc_3_0000:
        mov    eax, 0x15000e
        mov    esi, 0                        ; -> loc_1_01DC
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
        call   sub_1_013C
loc_3_005D:
        add    esp, 8
        or     edi, edi
        je     loc_3_00BD
        mov    ebx, dword ptr [edi + 0x2c]
        or     ebx, ebx
        je     loc_3_00BD
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_3_00BD
        call   sub_1_019D
loc_3_0078:
        jb     loc_3_00BD
        call   sub_1_04FB
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
        call   sub_1_02EE
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
        call   sub_1_013C
loc_3_00D4:
        add    esp, 8
        or     edi, edi
        je     loc_3_014B
        push   edx
        movzx  edx, word ptr [edi + 6]
        call   sub_1_02EE
loc_3_00E5:
        pop    edx
        jb     loc_3_00ED
        call   sub_1_0451
loc_3_00ED:
        mov    ebx, dword ptr [edi + 0x2c]
        or     ebx, ebx
        je     loc_3_014B
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_3_014B
        test   word ptr [edi + 4], 0x100
        je     loc_3_014B
        call   sub_1_019D
loc_3_0109:
        jb     loc_3_014B
        call   sub_1_04ED
loc_3_0110:
        push   ecx
        push   edx
        push   edi
        mov    eax, dword ptr [edi + 0x3c]
        or     eax, eax
        je     loc_3_0139
        mov    edi, esi
        mov    dx, word ptr [edi + 0x20]
        mov    ecx, dword ptr [edi + 0x18]
        mov    esi, dword ptr [edi + 0x1c]
        VxDCall  VDMAD.VDMAD_Set_Virt_State      ; cd2005000400
        mov    esi, edi
        test   dl, 1
        jne    loc_3_0139
        VxDCall  VDMAD.VDMAD_UnMask_Channel      ; cd2008000400
loc_3_0139:
        pop    edi
        movzx  edx, word ptr [edi + 6]
        call   sub_1_02EE
loc_3_0143:
        pop    edx
        pop    ecx
        and    word ptr [edi + 4], 0xfeff
loc_3_014B:
        pop    esi
        pop    edi
        pop    ebx
        pop    eax
        pop    ebp
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 4  vsize=0x78c  R-X
sub_4_0000:
        push   ebp
        mov    ebp, esp
        push   edi
        push   0
        push   dword ptr [ebp + 8]
        call   sub_1_013C
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
        je     loc_4_0431
        movzx  edx, word ptr [ebp + 0xc]
        push   2
        push   edx
        call   sub_1_013C
loc_4_021E:
        add    esp, 8
        or     edi, edi
        jne    loc_4_0431
        VMMCall  VMM.List_Allocate               ; cd209d000100
        jb     loc_4_0431
        mov    dword ptr [ebp - 4], eax
        VMMCall  VMM.List_Attach_Tail            ; cd209f000100
        mov    dword ptr [eax], 0
        push   1
        push   0x72
        VMMCall  VMM._HeapAllocate               ; cd204f000100
        add    esp, 8
        or     eax, eax
        je     loc_4_0431
        mov    edi, eax
        mov    eax, dword ptr [ebp - 4]
        mov    dword ptr [eax], edi
        mov    eax, dword ptr [ebp + 8]
        mov    dword ptr [edi + 0x44], eax
        mov    word ptr [edi + 0x70], 0
        mov    ax, word ptr [ebp + 0x28]
        mov    word ptr [edi + 4], ax
        mov    ax, word ptr [ebp + 0xc]
        mov    word ptr [edi + 6], ax
        mov    ax, word ptr [ebp + 0x10]
        mov    word ptr [edi + 0x10], ax
        mov    byte ptr [edi + 0xc], 6
        mov    ax, word ptr [ebp + 0x18]
        mov    byte ptr [edi + 0xd], al
        mov    ax, word ptr [ebp + 0x14]
        mov    word ptr [edi + 8], ax
        mov    ax, word ptr [ebp + 0x1c]
        mov    byte ptr [edi + 0xa], al
        mov    ax, word ptr [ebp + 0x20]
        mov    byte ptr [edi + 0xb], al
        call   sub_5_0DFF
loc_4_02AA:
        mov    word ptr [edi + 0x12], ax
        call   sub_1_049B
loc_4_02B3:
        jb     loc_4_0431
        push   edi
        movzx  ecx, byte ptr [edi + 0xa]
        mov    word ptr [0], cx              ; -> obj6:0x01D6
        mov    dword ptr [0x1c], edi         ; -> obj6:0x01F2
        mov    edi, 0                        ; -> obj6:0x01D6
        VxDCall  VPICD.VPICD_Virtualize_IRQ      ; cd2001000300
        pop    edi
        jae    loc_4_02EB
        push   0                             ; -> obj6:0x023D
        call   sub_5_05AC
loc_4_02E3:
        add    esp, 4
        jmp    loc_4_0431
loc_4_02EB:
        mov    dword ptr [edi + 0x28], eax
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        VxDCall  VPICD.VPICD_Get_Complete_Status ; cd2005000300
        test   ecx, 4
        jne    loc_4_0308
        or     word ptr [edi + 4], 0x8000
loc_4_0308:
        mov    esi, 0                        ; -> obj6:0x015A
        cmp    word ptr [ebp + 0x14], -1
        jne    loc_4_0319
        mov    esi, 0                        ; -> obj6:0x00F8
loc_4_0319:
        movzx  ecx, word ptr [esi]
        movzx  edx, word ptr [ebp + 0xc]
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_0326:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_0326
        mov    edx, dword ptr [esp]
        call   sub_4_0103
        pop    edi
        mov    dword ptr [edi + 0x48], eax
        or     eax, eax
        jne    loc_4_0351
        push   0                             ; -> obj6:0x01F6
        call   sub_5_05AC
loc_4_0349:
        add    esp, 4
        jmp    loc_4_0431
loc_4_0351:
        movsx  edx, word ptr [ebp + 0x14]
        cmp    edx, -1
        je     loc_4_039B
        mov    word ptr [edi + 8], dx
        mov    dx, 0x388
        mov    esi, 0                        ; -> obj6:0x01BC
        movzx  ecx, word ptr [esi]
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_0370:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_0370
        mov    edx, dword ptr [esp]
        call   sub_4_0103
        pop    edi
        mov    dword ptr [edi + 0x4c], eax
        or     eax, eax
        jne    loc_4_039B
        push   0                             ; -> obj6:0x0226
        call   sub_5_05AC
loc_4_0393:
        add    esp, 4
        jmp    loc_4_0431
loc_4_039B:
        mov    eax, dword ptr [ebp + 0x24]
        call   sub_4_0030
        mov    dword ptr [edi + 0x14], eax
        mov    dword ptr [edi + 0x18], ebx
        mov    dword ptr [edi + 0x20], ecx
        mov    dword ptr [edi + 0x1c], edx
        jae    loc_4_03C0
        push   0                             ; -> obj6:0x0288
        call   sub_5_05AC
loc_4_03BB:
        add    esp, 4
        jmp    loc_4_0431
loc_4_03C0:
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
        mov    esi, 0                        ; -> loc_1_0234
        VxDCall  VDMAD.VDMAD_Virtualize_Channel  ; cd2001000400
        mov    dword ptr [edi + 0x3c], eax
        jae    loc_4_0408
        push   0                             ; -> obj6:0x0270
        call   sub_5_05AC
loc_4_0403:
        add    esp, 4
        jmp    loc_4_0431
loc_4_0408:
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        push   ebx
loc_4_040F:
        call   sub_4_0087
        jb     loc_4_0430
        VMMCall  VMM.Get_Next_VM_Handle          ; cd203b000100
        cmp    ebx, dword ptr [esp]
        jne    loc_4_040F
        pop    ebx
        mov    dword ptr [esp + 0x1c], 0xffffffff
loc_4_042A:
        popal  
        add    esp, 4
        pop    ebp
        ret    
loc_4_0430:
        pop    ebx
loc_4_0431:
        mov    edi, dword ptr [ebp - 4]
        or     edi, edi
        je     loc_4_043D
        call   sub_4_0447
loc_4_043D:
        mov    dword ptr [esp + 0x1c], 0
        jmp    loc_4_042A
sub_4_0447:
        push   ebp
        mov    ebp, esp
        sub    esp, 4
        pushal 
        mov    dword ptr [ebp - 4], edi
        mov    edi, dword ptr [edi]
        or     edi, edi
        je     loc_4_051C
        xor    esi, esi
        xchg   dword ptr [edi + 0x40], esi
        or     esi, esi
        je     loc_4_046A
        VMMCall  VMM.List_Destroy                ; cd209c000100
loc_4_046A:
        mov    eax, dword ptr [edi + 0x3c]
        or     eax, eax
        je     loc_4_0477
        VxDCall  VDMAD.VDMAD_Unvirtualize_Channel ; cd201a000400
loc_4_0477:
        movzx  eax, word ptr [edi + 0x24]
        or     eax, eax
        je     loc_4_049A
        push   0
        push   eax
        VMMCall  VMM._Free_GDT_Selector          ; cd2077000100
        add    esp, 8
        mov    eax, dword ptr [edi + 0x14]
        push   0
        push   eax
        VMMCall  VMM._PageFree                   ; cd2055000100
        add    esp, 8
loc_4_049A:
        mov    eax, dword ptr [edi + 0x4c]
        or     eax, eax
        je     loc_4_04C9
        mov    esi, 0                        ; -> obj6:0x01BC
        movzx  ecx, word ptr [esi]
        mov    edx, 0x388
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_04B4:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_04B4
        mov    eax, dword ptr [esp]
        mov    eax, dword ptr [eax + 0x4c]
        call   sub_4_01CD
        pop    edi
loc_4_04C9:
        mov    eax, dword ptr [edi + 0x48]
        or     eax, eax
        je     loc_4_0503
        mov    esi, 0                        ; -> obj6:0x015A
        cmp    word ptr [edi + 8], -1
        jne    loc_4_04E1
        mov    esi, 0                        ; -> obj6:0x00F8
loc_4_04E1:
        movzx  ecx, word ptr [esi]
        movzx  edx, word ptr [edi + 6]
        push   edi
        mov    edi, esi
        add    esi, 2
loc_4_04EE:
        mov    word ptr [esi], dx
        inc    edx
        add    esi, 6
        loop   loc_4_04EE
        mov    eax, dword ptr [esp]
        mov    eax, dword ptr [eax + 0x48]
        call   sub_4_01CD
        pop    edi
loc_4_0503:
        mov    eax, dword ptr [edi + 0x28]
        or     eax, eax
        je     loc_4_0510
        VxDCall  VPICD.VPICD_Force_Default_Behavior ; cd2011000300
loc_4_0510:
        push   0
        push   edi
        VMMCall  VMM._HeapFree                   ; cd2051000100
        add    esp, 8
loc_4_051C:
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
sub_4_0539:
        push   ebp
        mov    ebp, esp
        sub    esp, 8
        pushal 
        mov    dword ptr [esp + 0x1c], 0x13
        movzx  edx, word ptr [ebp + 0xc]
        call   sub_4_06A4
        jb     loc_4_05CE
        mov    word ptr [ebp - 4], ax
        mov    word ptr [ebp - 8], dx
        movzx  edx, word ptr [ebp + 0xc]
        call   sub_1_032B
loc_4_0564:
        movsx  edx, word ptr [ebp + 0x10]
        cmp    edx, -1
        je     loc_4_0574
        call   sub_4_0721
        jb     loc_4_05C8
loc_4_0574:
        test   dword ptr [ebp + 0x28], 0x50
        jne    loc_4_0599
        movzx  edx, word ptr [ebp + 0xc]
        mov    esi, dword ptr [ebp + 0x1c]
        mov    bx, word ptr [ebp + 0x14]
        call   sub_1_03E7
loc_4_058D:
        mov    esi, dword ptr [ebp + 0x20]
        mov    bx, word ptr [ebp + 0x18]
        call   sub_1_041B
loc_4_0599:
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
        je     loc_4_05C8
        mov    dword ptr [esp + 0x1c], 0
loc_4_05C8:
        popal  
        add    esp, 8
        pop    ebp
        ret    
loc_4_05CE:
        mov    dword ptr [esp + 0x1c], 0x24
        jmp    loc_4_05C8
sub_4_05D8:
        push   ebp
        mov    ebp, esp
        push   edi
        push   ebx
        pushfd 
        push   dword ptr [ebp + 8]
        call   0
        add    esp, 4
        or     eax, eax
        je     loc_4_0620
        push   0
        push   dword ptr [ebp + 8]
        call   sub_1_013C
loc_4_05F7:
        add    esp, 8
        or     edi, edi
        je     loc_4_0620
        mov    ebx, dword ptr [edi + 0x2c]
        or     ebx, ebx
        je     loc_4_060F
        mov    eax, 1
        call   sub_5_0CDD
loc_4_060F:
        mov    ebx, dword ptr [edi + 0x34]
        or     ebx, ebx
        je     loc_4_0620
        mov    eax, 2
        call   sub_5_0CDD
loc_4_0620:
        mov    ebx, dword ptr [ebp + 8]
        mov    esi, dword ptr [0]            ; -> obj1:0x00D4
        or     esi, esi
        je     loc_4_066D
        cli    
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_4_066D
loc_4_0636:
        mov    edi, dword ptr [eax]
        cmp    dword ptr [edi + 0x44], ebx
        je     loc_4_0647
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        je     loc_4_066D
        jmp    loc_4_0636
loc_4_0647:
        popfd  
        pushfd 
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        push   ebx
loc_4_0650:
        call   sub_4_00C5
        VMMCall  VMM.Get_Next_VM_Handle          ; cd203b000100
        cmp    ebx, dword ptr [esp]
        jne    loc_4_0650
        pop    ebx
        mov    edi, eax
        call   sub_4_0447
        mov    eax, 0xffffffff
loc_4_066D:
        popfd  
        pop    ebx
        pop    edi
        pop    ebp
        ret    
loc_4_0672:
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0x18]
        push   dword ptr [ebp + 0x14]
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_5_05FD
loc_4_0689:
        add    esp, 0x14
        leave  
        clc    
        ret    
loc_4_068F:
        mov    eax, ebx
        mov    ebx, 0                        ; -> loc_4_0672
        VxDCall  VFAT.Service_0000h              ; cd2000004a04
        mov    eax, 0
        stc    
        ret    
        db  0CCh
sub_4_06A4:
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
        call   sub_1_02B4
loc_4_06BA:
        jb     loc_4_070E
        cmp    al, 0xaa
        jne    loc_4_070E
        mov    al, 0xe1
        call   sub_1_02D0
loc_4_06C7:
        jb     loc_4_070E
        call   sub_1_02B4
loc_4_06CE:
        jb     loc_4_070E
        mov    ah, al
        call   sub_1_02B4
loc_4_06D7:
        jb     loc_4_070E
        movzx  eax, ax
        mov    dword ptr [esp + 4], eax
        mov    al, 0xe7
        call   sub_1_02D0
loc_4_06E7:
        jb     loc_4_070E
        call   sub_1_02B4
loc_4_06EE:
        jb     loc_4_070E
        mov    ah, al
        call   sub_1_02B4
loc_4_06F7:
        jb     loc_4_070E
        cmp    ah, 0x68
        jne    loc_4_070E
        mov    ah, al
        and    ah, 0xf
        cmp    ah, 4
        jb     loc_4_070E
        and    al, 0xf0
        cmp    al, 0x80
        je     loc_4_0717
loc_4_070E:
        xor    eax, eax
        mov    dword ptr [esp + 4], eax
        stc    
        jmp    loc_4_071E
loc_4_0717:
        movzx  eax, ah
        mov    dword ptr [esp], eax
        clc    
loc_4_071E:
        pop    edx
        pop    eax
        ret    
sub_4_0721:
        push   eax
        push   ebx
        push   ecx
        pushfd 
        cli    
        mov    ebx, 4
        mov    al, 0x60
        call   sub_1_0518
loc_4_0732:
        mov    ebx, 4
        mov    al, 0x80
        call   sub_1_0518
loc_4_073E:
        mov    ebx, 2
        mov    al, 0x80
        call   sub_1_0518
loc_4_074A:
        mov    ebx, 4
        mov    al, 1
        call   sub_1_0518
loc_4_0756:
        mov    cx, 0x4000
loc_4_075A:
        in     al, dx
        test   al, 0x40
        jne    loc_4_0761
        loop   loc_4_075A
loc_4_0761:
        push   eax
        mov    ebx, 4
        mov    al, 0x60
        call   sub_1_0518
loc_4_076E:
        mov    ebx, 4
        mov    al, 0x80
        call   sub_1_0518
loc_4_077A:
        pop    eax
        popfd  
        test   al, 0x40
        je     loc_4_0787
        test   al, 6
        jne    loc_4_0787
        clc    
        jmp    loc_4_0788
loc_4_0787:
        stc    
loc_4_0788:
        pop    ecx
        pop    ebx
        pop    eax
        ret    
VxD__LTEXT ENDS

VxD__LTEXT SEGMENT   ; object 5  vsize=0x210d  R-X
        db  083h, 07Ch, 024h, 008h, 000h, 08Ah, 044h, 024h, 004h, 074h, 00Ah, 03Ch, 030h, 07Ch, 025h, 03Ch
        db  039h, 07Fh, 021h, 0EBh, 018h, 03Ch, 030h, 07Ch, 004h, 03Ch, 039h, 07Eh, 010h, 03Ch, 061h, 07Ch
        db  004h, 03Ch, 066h, 07Eh, 008h, 03Ch, 041h, 07Ch, 00Bh, 03Ch, 046h, 07Fh, 007h, 0B8h, 001h, 000h
        db  000h, 000h, 0EBh, 002h, 033h, 0C0h, 0C3h
sub_5_0037:
        mov    al, byte ptr [esp + 4]
        cmp    al, 0x30
        jl     loc_5_004D
        cmp    al, 0x39
        jg     loc_5_004D
        movsx  ax, al
        sub    ax, 0x30
        jmp    loc_5_0067
loc_5_004D:
        cmp    al, 0x61
        jl     loc_5_005F
        cmp    al, 0x66
        jg     loc_5_005F
        movsx  ax, al
        sub    ax, 0x57
        jmp    loc_5_0067
loc_5_005F:
        movsx  ax, al
        sub    ax, 0x37
loc_5_0067:
        ret    
sub_5_0068:
        sub    esp, 0xc
        xor    eax, eax
        mov    dword ptr [esp + 8], eax
        push   ebx
        mov    dword ptr [esp + 8], eax
        push   esi
        push   edi
        mov    esi, 1
        mov    edi, dword ptr [esp + 0x1c]
        push   ebp
        cmp    byte ptr [edi], al
        je     loc_5_012A
        mov    eax, dword ptr [esp + 0x24]
        mov    ebx, dword ptr [esp + 0x2c]
        mov    dword ptr [esp + 0x10], eax
loc_5_0096:
        push   ebx
        mov    al, byte ptr [edi]
        push   eax
        call   0
        add    esp, 8
        test   eax, eax
        jne    loc_5_00AE
        cmp    byte ptr [edi], 0x2c
        je     loc_5_00B3
        inc    edi
        jmp    loc_5_0121
loc_5_00AE:
        cmp    byte ptr [edi], 0x2c
        jne    loc_5_00C5
loc_5_00B3:
        inc    edi
        mov    eax, esi
        mov    esi, 1
        test   eax, eax
        je     loc_5_0121
        mov    bp, 0xfffe
        jmp    loc_5_010A
loc_5_00C5:
        xor    bp, bp
        cmp    ebx, 1
        sbb    esi, esi
        and    esi, 6
        add    si, 0xa
loc_5_00D4:
        mov    eax, edi
        inc    edi
        imul   bp, si
        mov    al, byte ptr [eax]
        push   eax
        call   sub_5_0037
        add    bp, ax
        add    esp, 4
        cmp    bp, word ptr [esp + 0x28]
        ja     loc_5_015C
        push   ebx
        mov    al, byte ptr [edi]
        push   eax
        call   0
        add    esp, 8
        test   eax, eax
        jne    loc_5_00D4
        mov    dword ptr [esp + 0x14], 1
        xor    esi, esi
loc_5_010A:
        mov    eax, dword ptr [esp + 0x10]
        cmp    word ptr [eax], 0xffff
        je     loc_5_0163
        add    dword ptr [esp + 0x10], 2
        mov    word ptr [eax], bp
        inc    dword ptr [esp + 0x18]
loc_5_0121:
        cmp    byte ptr [edi], 0
        jne    loc_5_0096
loc_5_012A:
        mov    eax, dword ptr [esp + 0x24]
        mov    ecx, dword ptr [esp + 0x18]
        lea    eax, [eax + ecx*2]
        cmp    word ptr [eax], 0xffff
        je     loc_5_014B
loc_5_013C:
        mov    word ptr [eax], 0xfffe
        add    eax, 2
        cmp    word ptr [eax], 0xffff
        jne    loc_5_013C
loc_5_014B:
        cmp    dword ptr [esp + 0x14], 1
        sbb    eax, eax
        neg    eax
loc_5_0154:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        add    esp, 0xc
        ret    
loc_5_015C:
        mov    eax, 1
        jmp    loc_5_0154
loc_5_0163:
        mov    eax, 1
        jmp    loc_5_0154
sub_5_016A:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x250
        push   ebx
        lea    eax, [ebp - 0x150]
        push   esi
        mov    ebx, dword ptr [ebp + 8]
        push   edi
        push   0
        push   0x100
        push   eax
        push   0
        push   ebx
        call   sub_5_2014
        add    esp, 0x14
        test   eax, eax
        jne    loc_5_03D3
        mov    edi, 0                        ; -> obj6:0x0078
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        sub    edi, ecx
        mov    edx, ecx
        mov    esi, edi
        mov    ecx, 0xffffffff
        lea    edi, [ebp - 0x150]
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        dec    edi
        mov    ecx, edx
        shr    ecx, 2
        rep movsd dword ptr es:[edi], dword ptr [esi]
        mov    ecx, edx
        and    ecx, 3
        rep movsb byte ptr es:[edi], byte ptr [esi]
        lea    eax, [ebp - 8]
        lea    ecx, [ebp - 0x150]
        push   eax
        push   ecx
        push   0x80000002
        call   sub_5_2004
        add    esp, 0xc
        test   eax, eax
        je     loc_5_01F1
        xor    eax, eax
        jmp    loc_5_03D3
loc_5_01F1:
        push   1
        lea    eax, [ebp - 0x250]
        push   0x100
        push   eax
        push   0
        push   ebx
        call   sub_5_2014
        add    esp, 0x14
        mov    esi, eax
        test   esi, esi
        je     loc_5_0218
        push   dword ptr [ebp - 8]
        jmp    loc_5_03C9
loc_5_0218:
        lea    eax, [ebp - 0x10]
        lea    ecx, [ebp - 0x250]
        push   eax
        push   ecx
        push   0x80000002
        call   sub_5_2004
        add    esp, 0xc
        test   eax, eax
        je     loc_5_023E
        mov    esi, 0x1d
        jmp    loc_5_039E
loc_5_023E:
        lea    eax, [ebp - 0xc]
        lea    ecx, [ebp - 0x50]
        mov    dword ptr [ebp - 0xc], 0x40
        push   eax
        push   ecx
        push   0
        push   0
        push   0                             ; -> obj6:0x0068
        push   dword ptr [ebp - 8]
        call   sub_5_200C
        add    esp, 0x18
        test   eax, eax
        je     loc_5_026F
        mov    esi, 0x1d
        jmp    loc_5_039E
loc_5_026F:
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_5_02D4
        lea    eax, [ebp - 4]
        push   eax
        push   0                             ; -> obj6:0x0050
        push   dword ptr [ebp - 0x10]
        call   sub_5_2004
        add    esp, 0xc
        test   eax, eax
        je     loc_5_02A1
        lea    eax, [ebp - 4]
        push   eax
        push   0                             ; -> obj6:0x0050
        push   dword ptr [ebp - 0x10]
        call   sub_5_1FFC
        add    esp, 0xc
loc_5_02A1:
        cmp    dword ptr [ebp - 4], 0
        je     loc_5_0305
        push   dword ptr [ebp - 0xc]
        lea    ecx, [ebp - 0x50]
        push   ecx
        push   1
        push   0
        push   0                             ; -> obj6:0x0014
        push   dword ptr [ebp - 4]
        call   sub_5_1FF4
        add    esp, 0x18
        push   dword ptr [ebp - 4]
        call   sub_5_1FE4
        mov    dword ptr [ebp - 4], 0
        add    esp, 4
loc_5_02D4:
        lea    eax, [ebp - 0xc]
        lea    ecx, [ebp - 0x50]
        mov    dword ptr [ebp - 0xc], 0x40
        push   eax
        push   ecx
        push   0
        push   0
        push   0                             ; -> obj6:0x0040
        push   dword ptr [ebp - 8]
        call   sub_5_200C
        add    esp, 0x18
        test   eax, eax
        je     loc_5_030F
        mov    esi, 0x1d
        jmp    loc_5_039E
loc_5_0305:
        mov    esi, 0x1d
        jmp    loc_5_039E
loc_5_030F:
        cmp    dword ptr [ebp - 0xc], 0
        je     loc_5_0378
        lea    eax, [ebp - 4]
        push   eax
        push   0                             ; -> obj6:0x0028
        push   dword ptr [ebp - 0x10]
        call   sub_5_2004
        add    esp, 0xc
        test   eax, eax
        je     loc_5_0341
        lea    eax, [ebp - 4]
        push   eax
        push   0                             ; -> obj6:0x0028
        push   dword ptr [ebp - 0x10]
        call   sub_5_1FFC
        add    esp, 0xc
loc_5_0341:
        cmp    dword ptr [ebp - 4], 0
        je     loc_5_03DA
        push   dword ptr [ebp - 0xc]
        lea    ecx, [ebp - 0x50]
        push   ecx
        push   1
        push   0
        push   0                             ; -> obj6:0x0014
        push   dword ptr [ebp - 4]
        call   sub_5_1FF4
        add    esp, 0x18
        push   dword ptr [ebp - 4]
        call   sub_5_1FE4
        mov    dword ptr [ebp - 4], 0
        add    esp, 4
loc_5_0378:
        push   dword ptr [ebp - 8]
        call   sub_5_1FE4
        add    esp, 4
        lea    eax, [ebp - 0x150]
        mov    dword ptr [ebp - 8], 0
        push   eax
        push   0x80000002
        call   sub_5_1FEC
        add    esp, 8
loc_5_039E:
        cmp    dword ptr [ebp - 8], 0
        je     loc_5_03AF
        push   dword ptr [ebp - 8]
        call   sub_5_1FE4
        add    esp, 4
loc_5_03AF:
        cmp    dword ptr [ebp - 0x10], 0
        je     loc_5_03C0
        push   dword ptr [ebp - 0x10]
        call   sub_5_1FE4
        add    esp, 4
loc_5_03C0:
        cmp    dword ptr [ebp - 4], 0
        je     loc_5_03D1
        push   dword ptr [ebp - 4]
loc_5_03C9:
        call   sub_5_1FE4
        add    esp, 4
loc_5_03D1:
        mov    eax, esi
loc_5_03D3:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
loc_5_03DA:
        mov    esi, 0x1d
        jmp    loc_5_039E
sub_5_03E1:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10c
        test   byte ptr [ebp + 0x14], 0x50
        push   ebx
        push   esi
        push   edi
        je     loc_5_03FA
        xor    eax, eax
        jmp    loc_5_05A5
loc_5_03FA:
        push   1
        lea    eax, [ebp - 0x10c]
        push   0x100
        push   eax
        push   0
        push   dword ptr [ebp + 8]
        call   sub_5_2014
        add    esp, 0x14
        mov    esi, eax
        test   esi, esi
        jne    loc_5_05A3
        lea    eax, [ebp - 0xc]
        lea    ecx, [ebp - 0x10c]
        push   eax
        push   ecx
        push   0x80000002
        call   sub_5_2004
        add    esp, 0xc
        test   eax, eax
        je     loc_5_0445
        mov    eax, 0x13
        jmp    loc_5_05A5
loc_5_0445:
        lea    eax, [ebp - 8]
        push   eax
        push   0                             ; -> obj6:0x0050
        push   dword ptr [ebp - 0xc]
        call   sub_5_2004
        add    esp, 0xc
        test   eax, eax
        je     loc_5_0472
        push   dword ptr [ebp - 0xc]
        call   sub_5_1FE4
        add    esp, 4
        mov    eax, 0x13
        jmp    loc_5_05A5
loc_5_0472:
        mov    edi, 0                        ; -> obj6:0x0000
        mov    ebx, 0x100
loc_5_047C:
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0x10c]
        push   eax
        mov    dword ptr [ebp - 4], ebx
        push   ecx
        push   0
        push   0
        push   dword ptr [edi]
        push   dword ptr [ebp - 8]
        call   sub_5_200C
        add    esp, 0x18
        test   eax, eax
        jne    loc_5_04A5
        cmp    dword ptr [ebp - 4], 0
        jne    loc_5_04BA
loc_5_04A5:
        add    edi, 4
        cmp    edi, 0xc                      ; -> obj6:0x000C
        jb     loc_5_047C
        cmp    dword ptr [ebp - 4], 0
        je     loc_5_0588
loc_5_04BA:
        push   1
        mov    edi, dword ptr [ebp + 0xc]
        push   0xf
        lea    eax, [ebp - 0x10c]
        push   edi
        push   eax
        call   sub_5_0068
        add    esp, 0x10
        test   eax, eax
        jne    loc_5_0588
        cmp    word ptr [edi], 0xffff
        je     loc_5_04F5
loc_5_04E0:
        cmp    word ptr [edi], 2
        jne    loc_5_04EB
        mov    word ptr [edi], 9
loc_5_04EB:
        add    edi, 2
        cmp    word ptr [edi], 0xffff
        jne    loc_5_04E0
loc_5_04F5:
        push   dword ptr [ebp - 8]
        call   sub_5_1FE4
        add    esp, 4
        lea    eax, [ebp - 8]
        push   eax
        push   0                             ; -> obj6:0x0028
        push   dword ptr [ebp - 0xc]
        call   sub_5_2004
        add    esp, 0xc
        test   eax, eax
        je     loc_5_052A
        push   dword ptr [ebp - 0xc]
        call   sub_5_1FE4
        add    esp, 4
        mov    eax, 0x13
        jmp    loc_5_05A5
loc_5_052A:
        mov    edi, 0                        ; -> obj6:0x0000
        mov    ebx, 0x100
loc_5_0534:
        lea    eax, [ebp - 4]
        lea    ecx, [ebp - 0x10c]
        push   eax
        mov    dword ptr [ebp - 4], ebx
        push   ecx
        push   0
        push   0
        push   dword ptr [edi]
        push   dword ptr [ebp - 8]
        call   sub_5_200C
        add    esp, 0x18
        test   eax, eax
        jne    loc_5_055D
        cmp    dword ptr [ebp - 4], 0
        jne    loc_5_056E
loc_5_055D:
        add    edi, 4
        cmp    edi, 0xc                      ; -> obj6:0x000C
        jb     loc_5_0534
        cmp    dword ptr [ebp - 4], 0
        je     loc_5_0588
loc_5_056E:
        push   1
        lea    ecx, [ebp - 0x10c]
        push   3
        push   dword ptr [ebp + 0x10]
        push   ecx
        call   sub_5_0068
        add    esp, 0x10
        test   eax, eax
        je     loc_5_058D
loc_5_0588:
        mov    esi, 0x13
loc_5_058D:
        push   dword ptr [ebp - 8]
        call   sub_5_1FE4
        add    esp, 4
        push   dword ptr [ebp - 0xc]
        call   sub_5_1FE4
        add    esp, 4
loc_5_05A3:
        mov    eax, esi
loc_5_05A5:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_05AC:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x10
        VMMCall  VMM.Open_Boot_Log               ; cd2088010100
        test   eax, eax
        jne    loc_5_05F9
        mov    dword ptr [ebp - 0xc], 0
        mov    dword ptr [ebp - 0x10], 0     ; -> obj6:0x0080
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
loc_5_05F9:
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_05FD:
        push   ebp
        mov    eax, dword ptr [esp + 8]
        mov    ebp, esp
        sub    esp, 0x130
        test   eax, eax
        push   ebx
        push   esi
        push   edi
        je     loc_5_093D
        cmp    eax, 1
        je     loc_5_0651
        cmp    eax, 2
        je     loc_5_0928
        cmp    eax, 3
        je     loc_5_089A
        cmp    eax, 4
        je     loc_5_08E6
        cmp    eax, 8
        je     loc_5_08EE
        cmp    eax, 0xd
        je     loc_5_0928
        mov    eax, 1
        jmp    loc_5_093F
loc_5_0651:
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
        call   sub_5_202C
        add    esp, 0xc
        test   eax, eax
        jne    loc_5_093F
        movzx  eax, word ptr [ebp - 0xd4]
        mov    di, 0xffff
        cmp    eax, 1
        je     loc_5_06BB
        cmp    eax, 2
        je     loc_5_06DC
        mov    eax, 0x13
        jmp    loc_5_093F
loc_5_06BB:
        cmp    word ptr [ebp - 0xaa], 0x10
        je     loc_5_06CF
        mov    eax, 0x13
        jmp    loc_5_093F
loc_5_06CF:
        mov    ax, word ptr [ebp - 0xd2]
        mov    word ptr [ebp - 2], ax
        jmp    loc_5_0718
loc_5_06DC:
        cmp    word ptr [ebp - 0xaa], 0x10
        je     loc_5_06F0
        mov    eax, 0x13
        jmp    loc_5_093F
loc_5_06F0:
        movzx  ecx, word ptr [ebp - 0xa8]
        mov    ax, word ptr [ebp - 0xd2]
        cmp    ecx, 4
        mov    word ptr [ebp - 2], ax
        je     loc_5_0711
        mov    eax, 0x13
        jmp    loc_5_093F
loc_5_0711:
        mov    di, word ptr [ebp - 0xd0]
loc_5_0718:
        cmp    word ptr [ebp - 0x82], 1
        jb     loc_5_0890
        cmp    word ptr [ebp - 0x72], 1
        jb     loc_5_0890
        movzx  ax, byte ptr [ebp - 0x80]
        movzx  cx, byte ptr [ebp - 0x70]
        mov    word ptr [ebp - 4], ax
        cmp    word ptr [ebp - 2], 0xffff
        mov    word ptr [ebp - 6], cx
        jne    loc_5_0755
        mov    eax, 0x13
        jmp    loc_5_093F
loc_5_0755:
        cmp    di, 0xffff
        je     loc_5_0760
        mov    di, word ptr [ebp - 2]
loc_5_0760:
        xor    ebx, ebx
        lea    eax, [ebp - 0x24]
        push   ebx
        lea    ecx, [ebp - 0xc]
        push   ebx
        mov    dword ptr [ebp - 0x24], ebx
        push   eax
        push   ecx
        push   esi
        call   sub_5_2024
        add    esp, 0x14
        test   eax, eax
        je     loc_5_077F
        mov    dword ptr [ebp - 0xc], ebx
loc_5_077F:
        mov    eax, dword ptr [ebp - 0xc]
        cmp    eax, 8
        je     loc_5_0793
        cmp    eax, 0x10
        je     loc_5_079A
        cmp    eax, 0x40
        je     loc_5_07A1
        jmp    loc_5_07A6
loc_5_0793:
        mov    ebx, 0x20
        jmp    loc_5_07A6
loc_5_079A:
        mov    ebx, 0x10
        jmp    loc_5_07A6
loc_5_07A1:
        mov    ebx, 0x40
loc_5_07A6:
        push   esi
        call   sub_5_016A
        add    esp, 4
        test   eax, eax
        jne    loc_5_093F
        push   ebx
        lea    eax, [ebp - 0x2c]
        push   eax
        lea    ecx, [ebp - 0x38]
        push   ecx
        push   esi
        call   sub_5_03E1
        add    esp, 0x10
        test   eax, eax
        jne    loc_5_093F
        push   ebx
        lea    eax, [ebp - 0x2c]
        push   0x20
        lea    ecx, [ebp - 0x38]
        push   eax
        push   ecx
        push   dword ptr [ebp - 6]
        push   dword ptr [ebp - 4]
        push   edi
        push   dword ptr [ebp - 2]
        push   esi
        call   sub_4_0539
loc_5_07EC:
        add    esp, 0x24
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_5_087C
        sub    di, 0xffff
        mov    dword ptr [ebp - 0x18], esi
        mov    dword ptr [ebp - 0x1c], 0     ; -> obj6:0x00A4
        cmp    di, 1
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
        test   ebx, ebx
        jne    loc_5_0889
        cmp    word ptr [ebp - 4], 9
        mov    eax, 2
        je     loc_5_083E
        movzx  eax, word ptr [ebp - 4]
loc_5_083E:
        movzx  ecx, word ptr [ebp - 6]
        movzx  edx, word ptr [ebp - 2]
        push   ecx
        push   eax
        push   edx
        lea    ecx, [ebp - 0x58]
        push   0                             ; -> obj6:0x0094
        push   ecx
        call   sub_5_201C
        add    esp, 0x14
        lea    ecx, [ebp - 0x58]
        mov    dword ptr [ebp - 0x10], 0     ; -> obj6:0x008C
        mov    dword ptr [ebp - 0x14], ecx
        push   dword ptr [ebp - 0x14]
        push   dword ptr [ebp - 0x10]
        xor    eax, eax
        xor    ecx, ecx
        xor    edx, edx
        VxDCall  VFAT.Service_0002h              ; cd2002004a04
        add    esp, 8
loc_5_087C:
        test   ebx, ebx
        jne    loc_5_0889
        push   esi
        call   sub_5_17BD
        add    esp, 4
loc_5_0889:
        mov    eax, ebx
        jmp    loc_5_093F
loc_5_0890:
        mov    eax, 0x13
        jmp    loc_5_093F
loc_5_089A:
        mov    eax, dword ptr [ebp + 0xc]
        test   eax, eax
        je     loc_5_08B0
        cmp    eax, 1
        je     loc_5_08CB
        mov    eax, 1
        jmp    loc_5_093F
loc_5_08B0:
        push   dword ptr [ebp + 0x10]
        call   sub_4_0000
loc_5_08B8:
        add    esp, 4
        cmp    eax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 0x13
        jmp    loc_5_093F
loc_5_08CB:
        push   dword ptr [ebp + 0x10]
        call   sub_4_0000
loc_5_08D3:
        add    esp, 4
        cmp    eax, 1
        mov    eax, 0
        adc    eax, -1
        and    eax, 0x13
        jmp    loc_5_093F
loc_5_08E6:
        cmp    dword ptr [ebp + 0xc], 1
        jne    loc_5_0928
        jmp    loc_5_093D
loc_5_08EE:
        mov    eax, dword ptr [ebp + 0xc]
        and    eax, 0x7fffffff
        cmp    eax, 4
        jb     loc_5_0921
        cmp    eax, 5
        jbe    loc_5_090C
        cmp    eax, 6
        jb     loc_5_0921
        cmp    eax, 8
        jbe    loc_5_0916
        jmp    loc_5_0921
loc_5_090C:
        push   dword ptr [ebp + 0x10]
        call   sub_3_004C
loc_5_0914:
        jmp    loc_5_091E
loc_5_0916:
        push   dword ptr [ebp + 0x10]
        call   sub_3_00C3
loc_5_091E:
        add    esp, 4
loc_5_0921:
        mov    eax, 1
        jmp    loc_5_093F
loc_5_0928:
        mov    esi, dword ptr [ebp + 0x10]
        push   esi
        call   sub_5_186C
        add    esp, 4
        push   esi
        call   sub_4_05D8
loc_5_093A:
        add    esp, 4
loc_5_093D:
        xor    eax, eax
loc_5_093F:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
        db  0CCh, 0CCh
sub_5_0948:
        push   eax
        push   edx
        push   ebx
        push   ecx
        movzx  edx, word ptr [edi + 8]
        mov    ebx, 0x105
        mov    al, 1
        call   sub_1_0518
loc_5_095C:
        mov    ebx, 4
        mov    al, 0x60
        call   sub_1_0518
loc_5_0968:
        mov    ebx, 0x104
        mov    al, 0x3f
        call   sub_1_0518
loc_5_0974:
        mov    ebx, 8
        xor    al, al
        call   sub_1_0518
loc_5_0980:
        mov    ebx, 0xbd
        mov    al, 0xc0
        call   sub_1_0518
loc_5_098C:
        mov    ecx, 0x15
        mov    ebx, 0x40
        mov    al, 0x3f
loc_5_0998:
        xor    bh, bh
        call   sub_1_0518
loc_5_099F:
        mov    bh, 1
        call   sub_1_0518
loc_5_09A6:
        inc    bl
        loop   loc_5_0998
        mov    ecx, 8
        mov    ebx, 0xb0
        xor    eax, eax
loc_5_09B6:
        xor    bh, bh
        call   sub_1_0518
loc_5_09BD:
        mov    bh, 1
        call   sub_1_0518
loc_5_09C4:
        inc    bl
        loop   loc_5_09B6
        mov    ebx, 0x104
        xor    eax, eax
        call   sub_1_0518
loc_5_09D4:
        mov    ebx, 0x105
        call   sub_1_0518
loc_5_09DE:
        call   sub_1_0509
loc_5_09E3:
        call   sub_1_0509
loc_5_09E8:
        call   sub_1_0509
loc_5_09ED:
        pop    ecx
        pop    ebx
        pop    edx
        pop    eax
        ret    
        db  08Bh, 0C0h
loc_5_09F4:
        push   eax
        mov    eax, dword ptr [esi + 0x2c]
        cmp    eax, ebx
        je     loc_5_0A14
        or     eax, eax
        jne    loc_5_0A27
        push   ebx
        push   ecx
        push   edx
        mov    edi, esi
        mov    eax, 1
        call   sub_5_0D64
        pop    edx
        pop    ecx
        pop    ebx
        jb     loc_5_0A27
loc_5_0A14:
        pop    eax
        cmp    ecx, 4
        jbe    loc_5_0A20
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_5_0A20:
        je     loc_5_0A25
        in     al, dx
        jmp    loc_5_0A26
loc_5_0A25:
        out    dx, al
loc_5_0A26:
        ret    
loc_5_0A27:
        mov    eax, 1
        mov    edi, esi
        call   sub_5_0BC2
        pop    eax
        xor    eax, eax
        dec    eax
        ret    
loc_5_0A38:
        push   eax
        mov    eax, dword ptr [esi + 0x34]
        cmp    eax, ebx
        je     loc_5_0A5D
        or     eax, eax
        jne    loc_5_0A70
        push   ebx
        push   ecx
        push   edx
        mov    edi, esi
        mov    eax, 2
        call   sub_5_0D64
        pop    edx
        pop    ecx
        pop    ebx
        jb     loc_5_0A70
        call   sub_5_0948
loc_5_0A5D:
        pop    eax
        cmp    ecx, 4
        jbe    loc_5_0A69
        VMMCall  VMM.Simulate_IO                 ; cd2094800100
loc_5_0A69:
        je     loc_5_0A6E
        in     al, dx
        jmp    loc_5_0A6F
loc_5_0A6E:
        out    dx, al
loc_5_0A6F:
        ret    
loc_5_0A70:
        mov    eax, 2
        mov    edi, esi
        call   sub_5_0BC2
        pop    eax
        xor    eax, eax
        dec    eax
        ret    
sub_5_0A81:
        push   esi
        push   eax
        push   ecx
        push   edx
        mov    si, 0xf
        xor    ecx, ecx
        movzx  edx, word ptr [edi + 6]
loc_5_0A8F:
        cmp    ecx, 3
        jbe    loc_5_0AA4
        cmp    ecx, 8
        je     loc_5_0AA4
        cmp    ecx, 9
        je     loc_5_0AA4
        VMMCall  VMM.Enable_Local_Trapping       ; cd2098000100
loc_5_0AA4:
        inc    dx
        cmp    cx, si
        je     loc_5_0AAF
        inc    cx
        jmp    loc_5_0A8F
loc_5_0AAF:
        pop    edx
        pop    ecx
        pop    eax
        pop    esi
        clc    
        ret    
sub_5_0AB5:
        push   esi
        push   eax
        push   ecx
        push   edx
        mov    si, 0xf
        xor    ecx, ecx
        movzx  edx, word ptr [edi + 6]
loc_5_0AC3:
        cmp    ecx, 3
        jbe    loc_5_0AD8
        cmp    ecx, 8
        je     loc_5_0AD8
        cmp    ecx, 9
        je     loc_5_0AD8
        VMMCall  VMM.Disable_Local_Trapping      ; cd209a000100
loc_5_0AD8:
        inc    dx
        cmp    cx, si
        je     loc_5_0AE3
        inc    cx
        jmp    loc_5_0AC3
loc_5_0AE3:
        pop    edx
        pop    ecx
        pop    eax
        pop    esi
        clc    
        ret    
sub_5_0AE9:
        push   eax
        push   ebx
        push   edx
        movzx  edx, word ptr [edi + 8]
        mov    ebx, 4
        mov    eax, 0x60
        call   sub_1_0518
loc_5_0AFF:
        call   sub_1_0509
loc_5_0B04:
        pop    edx
        pop    ebx
        pop    eax
        ret    
sub_5_0B08:
        push   esi
        push   eax
        push   ecx
        movzx  edx, word ptr [edi + 6]
        VMMCall  VMM.Enable_Local_Trapping       ; cd2098000100
        inc    dx
        VMMCall  VMM.Enable_Local_Trapping       ; cd2098000100
        inc    dx
        VMMCall  VMM.Enable_Local_Trapping       ; cd2098000100
        inc    dx
        VMMCall  VMM.Enable_Local_Trapping       ; cd2098000100
        movzx  edx, word ptr [edi + 6]
        add    dx, 8
        VMMCall  VMM.Enable_Local_Trapping       ; cd2098000100
        inc    dx
        VMMCall  VMM.Enable_Local_Trapping       ; cd2098000100
        mov    si, 3
        xor    ecx, ecx
        mov    edx, 0x388
loc_5_0B4E:
        VMMCall  VMM.Enable_Local_Trapping       ; cd2098000100
        inc    dx
        cmp    cx, si
        je     loc_5_0B5F
        inc    cx
        jmp    loc_5_0B4E
loc_5_0B5F:
        pop    ecx
        pop    eax
        pop    esi
        clc    
        ret    
sub_5_0B64:
        push   esi
        push   eax
        push   ecx
        push   edx
        movzx  edx, word ptr [edi + 6]
        VMMCall  VMM.Disable_Local_Trapping      ; cd209a000100
        inc    dx
        VMMCall  VMM.Disable_Local_Trapping      ; cd209a000100
        inc    dx
        VMMCall  VMM.Disable_Local_Trapping      ; cd209a000100
        inc    dx
        VMMCall  VMM.Disable_Local_Trapping      ; cd209a000100
        movzx  edx, word ptr [edi + 6]
        add    dx, 8
        VMMCall  VMM.Disable_Local_Trapping      ; cd209a000100
        inc    dx
        VMMCall  VMM.Disable_Local_Trapping      ; cd209a000100
        mov    si, 3
        xor    ecx, ecx
        mov    edx, 0x388
loc_5_0BAB:
        VMMCall  VMM.Disable_Local_Trapping      ; cd209a000100
        inc    dx
        cmp    cx, si
        je     loc_5_0BBC
        inc    cx
        jmp    loc_5_0BAB
loc_5_0BBC:
        pop    edx
        pop    ecx
        pop    eax
        pop    esi
        clc    
        ret    
sub_5_0BC2:
        test   word ptr [edi + 4], 1
        jne    loc_5_0C17
        call   sub_1_019D
loc_5_0BCF:
        jb     loc_5_0C17
        cmp    eax, 2
        je     loc_5_0BF0
        test   dword ptr [esi], 1
        jne    loc_5_0C17
        or     dword ptr [esi], 1
        movzx  ecx, word ptr [0xc]           ; -> obj2:0x000C
        add    ecx, dword ptr [4]            ; -> obj2:0x0004
        jmp    loc_5_0C08
loc_5_0BF0:
        test   dword ptr [esi], 2
        jne    loc_5_0C17
        or     dword ptr [esi], 2
        movzx  ecx, word ptr [0xe]           ; -> loc_2_000E
        add    ecx, dword ptr [4]            ; -> obj2:0x0004
loc_5_0C08:
        mov    eax, 0x30
        xor    esi, esi
        xor    edi, edi
        VxDCall  SHELL.SHELL_Message             ; cd2004001700
loc_5_0C17:
        ret    
loc_5_0C18:
        mov    esi, dword ptr [0]            ; -> obj1:0x00D4
        or     esi, esi
        je     loc_5_0C65
        cli    
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        sti    
        je     loc_5_0C65
loc_5_0C2C:
        push   eax
        mov    edi, dword ptr [eax]
        xor    eax, eax
        cmp    dword ptr [edi + 0x2c], ebx
        jne    loc_5_0C40
        or     eax, 1
        mov    dword ptr [edi + 0x30], 0xffffffff
loc_5_0C40:
        cmp    dword ptr [edi + 0x34], ebx
        jne    loc_5_0C4F
        or     eax, 2
        mov    dword ptr [edi + 0x38], 0xffffffff
loc_5_0C4F:
        or     eax, eax
        je     loc_5_0C58
        call   sub_5_0CDD
loc_5_0C58:
        pop    eax
        cli    
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        sti    
        je     loc_5_0C65
        jmp    loc_5_0C2C
loc_5_0C65:
        mov    edi, dword ptr [0]            ; -> obj1:0x00D0
        xor    esi, esi
        xchg   dword ptr [edi + ebx], esi
        or     esi, esi
        je     loc_5_0C7A
        VMMCall  VMM.List_Destroy                ; cd209c000100
loc_5_0C7A:
        clc    
        ret    
loc_5_0C7C:
        pushal 
        pushfd 
        mov    edi, dword ptr [0]            ; -> obj1:0x00D0
        mov    eax, 5
        mov    ecx, 0x24
        VMMCall  VMM.List_Create                 ; cd209b000100
        jb     loc_5_0CD5
        mov    dword ptr [edi + ebx], esi
        mov    esi, dword ptr [0]            ; -> obj1:0x00D4
        or     esi, esi
        je     loc_5_0CD9
        cli    
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        sti    
        je     loc_5_0CD9
loc_5_0CAD:
        mov    edi, dword ptr [eax]
        call   sub_4_0087
loc_5_0CB4:
        jb     loc_5_0CC2
        cli    
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        sti    
        je     loc_5_0CD9
        jmp    loc_5_0CAD
loc_5_0CC2:
        mov    edi, dword ptr [0]            ; -> obj1:0x00D0
        mov    esi, dword ptr [edi + ebx]
        or     esi, esi
        je     loc_5_0CD5
        VMMCall  VMM.List_Destroy                ; cd209c000100
loc_5_0CD5:
        popfd  
        stc    
        jmp    loc_5_0CDB
loc_5_0CD9:
        popfd  
        clc    
loc_5_0CDB:
        popal  
        ret    
sub_5_0CDD:
        push   eax
        push   edx
        push   esi
        call   sub_1_019D
loc_5_0CE5:
        jb     loc_5_0D5F
        test   eax, 2
        je     loc_5_0D07
        cmp    dword ptr [edi + 0x34], ebx
        jne    loc_5_0D07
        call   sub_5_0AE9
        call   sub_5_0B08
        mov    dword ptr [edi + 0x34], 0
        and    dword ptr [esi], 0xfffffffd
loc_5_0D07:
        test   eax, 1
        je     loc_5_0D43
        cmp    dword ptr [edi + 0x2c], ebx
        jne    loc_5_0D43
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_5_0D24
        mov    eax, dword ptr [edi + 0x3c]
        VxDCall  VDMAD.VDMAD_Enable_Translation  ; cd2014000400
loc_5_0D24:
        call   sub_5_0A81
        movzx  edx, word ptr [edi + 6]
        call   sub_1_0311
loc_5_0D32:
        jb     loc_5_0D39
        call   sub_1_035B
loc_5_0D39:
        mov    dword ptr [edi + 0x2c], 0
        and    dword ptr [esi], 0xfffffffe
loc_5_0D43:
        mov    eax, dword ptr [edi + 0x2c]
        or     eax, eax
        jne    loc_5_0D5F
        mov    eax, dword ptr [edi + 0x28]
        VxDCall  VPICD.VPICD_Physically_Mask     ; cd2008000300
        VxDCall  VPICD.VPICD_Phys_EOI            ; cd2004000300
        VxDCall  VPICD.VPICD_Clear_Int_Request   ; cd2003000300
loc_5_0D5F:
        pop    esi
        pop    edx
        pop    eax
        clc    
        ret    
sub_5_0D64:
        push   edx
        push   ecx
        push   eax
        test   eax, 2
        je     loc_5_0D8D
        cmp    word ptr [edi + 8], -1
        je     loc_5_0D9F
        cmp    dword ptr [edi + 0x34], 0
        je     loc_5_0D82
        cmp    dword ptr [edi + 0x34], ebx
        je     loc_5_0D8D
        jmp    loc_5_0D9F
loc_5_0D82:
        mov    dword ptr [edi + 0x34], ebx
        call   sub_5_0B64
        mov    dword ptr [edi + 0x38], ebx
loc_5_0D8D:
        test   eax, 1
        je     loc_5_0DD1
        cmp    dword ptr [edi + 0x2c], 0
        je     loc_5_0DA2
        cmp    dword ptr [edi + 0x2c], ebx
        je     loc_5_0DFA
loc_5_0D9F:
        stc    
        jmp    loc_5_0DFB
loc_5_0DA2:
        mov    dword ptr [edi + 0x2c], ebx
        cmp    dword ptr [edi + 0x30], ebx
        je     loc_5_0DB3
        movzx  edx, word ptr [edi + 6]
        call   sub_1_02EE
loc_5_0DB3:
        mov    dword ptr [edi + 0x30], ebx
        call   sub_1_039A
loc_5_0DBB:
        call   sub_5_0AB5
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        jne    loc_5_0DD1
        mov    eax, dword ptr [edi + 0x3c]
        VxDCall  VDMAD.VDMAD_Disable_Translation ; cd2013000400
loc_5_0DD1:
        mov    eax, dword ptr [esp]
        test   eax, 1
        je     loc_5_0DFA
        mov    eax, dword ptr [edi + 0x28]
        VxDCall  VPICD.VPICD_Get_Complete_Status ; cd2005000300
        test   ecx, 0x10
        je     loc_5_0DF4
        VxDCall  VPICD.VPICD_Physically_Mask     ; cd2008000300
        jmp    loc_5_0DFA
loc_5_0DF4:
        VxDCall  VPICD.VPICD_Physically_Unmask   ; cd2009000300
loc_5_0DFA:
        clc    
loc_5_0DFB:
        pop    eax
        pop    ecx
        pop    edx
        ret    
sub_5_0DFF:
        mov    eax, 0x400
        clc    
        ret    
sub_5_0E06:
        push   ebp
        mov    ebp, esp
        push   edi
        push   0
        push   dword ptr [ebp + 8]
        call   sub_1_013C
loc_5_0E14:
        add    esp, 8
        xor    eax, eax
        or     edi, edi
        je     loc_5_0E20
        mov    eax, dword ptr [edi + 0x6c]
loc_5_0E20:
        pop    edi
        pop    ebp
        ret    
sub_5_0E23:
        push   ebp
        mov    ebp, esp
        push   edi
        push   0
        push   dword ptr [ebp + 8]
        call   sub_1_013C
loc_5_0E31:
        add    esp, 8
        or     edi, edi
        je     loc_5_0E3E
        mov    eax, dword ptr [ebp + 0xc]
        mov    dword ptr [edi + 0x6c], eax
loc_5_0E3E:
        pop    edi
        pop    ebp
        ret    
        db  0CCh, 0CCh, 0CCh
AUDDRV_PM_API:
        movzx  eax, word ptr [ebp + 0x14]
        cmp    ah, 3
        jae    loc_5_0E6E
        movzx  ecx, ah
        shl    ecx, 3
        lea    ecx, [ecx]                    ; -> obj6:0x00E0
        movzx  eax, al
        cmp    eax, dword ptr [ecx]
        jae    loc_5_0E6E
        mov    ecx, dword ptr [ecx + 4]
        call   dword ptr [ecx + eax*4]
        jb     loc_5_0E6E
        and    word ptr [ebp + 0x2c], 0xfffe
        ret    
loc_5_0E6E:
        or     word ptr [ebp + 0x2c], 1
        ret    
loc_5_0E74:
        push   edi
        mov    eax, dword ptr [ebp + 0x18]
        push   0
        push   eax
        call   sub_1_013C
loc_5_0E80:
        add    esp, 8
        jb     loc_5_0EA6
        movzx  eax, word ptr [ebp + 0x10]
        test   eax, 0xffffffff
        jne    loc_5_0EA6
        mov    eax, dword ptr [edi + 0x3c]
        or     eax, eax
        je     loc_5_0EA6
        push   ecx
        VxDCall  VDMAD.VDMAD_Get_Phys_Count      ; cd201c000400
        mov    word ptr [ebp + 0x1c], cx
        pop    ecx
        clc    
        jmp    loc_5_0EAD
loc_5_0EA6:
        mov    word ptr [ebp + 0x1c], 0xffff
        stc    
loc_5_0EAD:
        pop    edi
        ret    
loc_5_0EAF:
        call   sub_5_0DFF
        mov    word ptr [ebp + 0x1c], ax
        clc    
        ret    
loc_5_0EBA:
        movzx  eax, word ptr [ebp + 0x10]
        movzx  edx, word ptr [ebp + 0x1c]
        or     edx, edx
        je     loc_5_0EFC
        test   eax, 1
        je     loc_5_0EE1
        push   2
        push   edx
        call   sub_1_013C
loc_5_0ED5:
        add    esp, 8
        or     edi, edi
        je     loc_5_0EFC
        mov    ecx, dword ptr [edi + 0x30]
        jmp    loc_5_0F04
loc_5_0EE1:
        test   eax, 2
        je     loc_5_0EFC
        push   3
        push   edx
        call   sub_1_013C
loc_5_0EF0:
        add    esp, 8
        or     edi, edi
        je     loc_5_0EFC
        mov    ecx, dword ptr [edi + 0x38]
        jmp    loc_5_0F04
loc_5_0EFC:
        mov    word ptr [ebp + 0x1c], 1
loc_5_0F02:
        stc    
        ret    
loc_5_0F04:
        mov    word ptr [ebp + 0x1c], 0
        call   sub_5_0D64
        jb     loc_5_0F12
        ret    
loc_5_0F12:
        mov    word ptr [ebp + 0x1c], 2
        jmp    loc_5_0F02
loc_5_0F1A:
        movzx  eax, word ptr [ebp + 0x10]
        movzx  edx, word ptr [ebp + 0x1c]
        or     edx, edx
        je     loc_5_0F76
        test   eax, 1
        je     loc_5_0F5B
        VMMCall  VMM.Test_Sys_VM_Handle          ; cd2004000100
        je     loc_5_0F47
        push   2
        push   edx
        call   sub_1_013C
loc_5_0F3D:
        add    esp, 8
        or     edi, edi
        jne    loc_5_0F56
        add    edx, 4
loc_5_0F47:
        push   2
        push   edx
        call   sub_1_013C
loc_5_0F4F:
        add    esp, 8
        or     edi, edi
        je     loc_5_0F76
loc_5_0F56:
        mov    ecx, dword ptr [edi + 0x2c]
        jmp    loc_5_0F7E
loc_5_0F5B:
        test   eax, 2
        je     loc_5_0F76
        push   3
        push   edx
        call   sub_1_013C
loc_5_0F6A:
        add    esp, 8
        or     edi, edi
        je     loc_5_0F76
        mov    ecx, dword ptr [edi + 0x34]
        jmp    loc_5_0F7E
loc_5_0F76:
        mov    word ptr [ebp + 0x1c], 1
loc_5_0F7C:
        stc    
        ret    
loc_5_0F7E:
        cmp    ecx, ebx
        je     loc_5_0F8A
        mov    word ptr [ebp + 0x1c], 2
        jmp    loc_5_0F7C
loc_5_0F8A:
        call   sub_5_0CDD
        mov    word ptr [ebp + 0x1c], 0
        clc    
        ret    
loc_5_0F97:
        pushfd 
        mov    ax, word ptr [ebp + 0x1c]
        and    ax, 0xfffe
        jne    loc_5_1005
        xor    ecx, ecx
        mov    ax, word ptr [ebp + 0x1c]
        and    ax, 1
        je     loc_5_0FB1
        mov    ecx, dword ptr [ebp + 0x18]
loc_5_0FB1:
        cli    
        mov    esi, dword ptr [0]            ; -> obj1:0x00D4
        or     esi, esi
        je     loc_5_1005
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_5_1005
        mov    edi, dword ptr [eax]
        jecxz  loc_5_0FD9
loc_5_0FC8:
        cmp    dword ptr [edi + 0x44], ecx
        je     loc_5_0FD9
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        je     loc_5_1005
        mov    edi, dword ptr [eax]
        jmp    loc_5_0FC8
loc_5_0FD9:
        popfd  
        pushfd 
        mov    esi, edi
        xchg   edi, eax
        mov    ax, 0x3810
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   edi, eax
        cmp    edi, -1
        je     loc_5_1005
        mov    ecx, dword ptr [edi]
        or     ecx, ecx
        je     loc_5_1005
        cmp    ecx, 0x72
        ja     loc_5_1005
        cld    
        rep movsb byte ptr es:[edi], byte ptr [esi]
        mov    word ptr [ebp + 0x1c], 1
        popfd  
        clc    
        ret    
loc_5_1005:
        mov    word ptr [ebp + 0x1c], 0
        popfd  
        stc    
        ret    
loc_5_100E:
        movzx  edx, word ptr [ebp + 0x1c]
        or     edx, edx
        je     loc_5_1043
        push   3
        push   edx
        call   sub_1_013C
loc_5_101E:
        add    esp, 8
        or     edi, edi
        je     loc_5_1043
        mov    ecx, dword ptr [edi + 0x38]
        mov    word ptr [ebp + 0x1c], 0
        mov    eax, 2
        call   sub_5_0D64
        jb     loc_5_103B
        ret    
loc_5_103B:
        mov    word ptr [ebp + 0x1c], 2
        jmp    loc_5_1049
loc_5_1043:
        mov    word ptr [ebp + 0x1c], 1
loc_5_1049:
        stc    
        ret    
loc_5_104B:
        movzx  edx, word ptr [ebp + 0x1c]
        or     edx, edx
        je     loc_5_1083
        push   3
        push   edx
        call   sub_1_013C
loc_5_105B:
        add    esp, 8
        or     edi, edi
        je     loc_5_1083
        mov    ecx, dword ptr [edi + 0x34]
        cmp    ecx, ebx
        je     loc_5_1071
        mov    word ptr [ebp + 0x1c], 3
        jmp    loc_5_1089
loc_5_1071:
        mov    eax, 2
        call   sub_5_0CDD
        mov    word ptr [ebp + 0x1c], 0
        clc    
        ret    
loc_5_1083:
        mov    word ptr [ebp + 0x1c], 1
loc_5_1089:
        stc    
        ret    
loc_5_108B:
        pushfd 
        mov    ax, word ptr [ebp + 0x1c]
        cmp    ax, 1
        jne    loc_5_1101
        mov    ecx, dword ptr [ebp + 0x18]
        cli    
        mov    esi, dword ptr [0]            ; -> obj1:0x00D4
        or     esi, esi
        je     loc_5_1101
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        je     loc_5_1101
loc_5_10AC:
        mov    edi, dword ptr [eax]
        cmp    dword ptr [edi + 0x44], ecx
        je     loc_5_10BD
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        je     loc_5_1101
        jmp    loc_5_10AC
loc_5_10BD:
        popfd  
        pushfd 
        cmp    word ptr [edi + 8], -1
        je     loc_5_1101
        mov    esi, edi
        xchg   edi, eax
        mov    ax, 0x3810
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   edi, eax
        cmp    edi, -1
        je     loc_5_1101
        mov    ecx, dword ptr [edi]
        or     ecx, ecx
        je     loc_5_1101
        cmp    ecx, 0x1c
        ja     loc_5_1101
        mov    word ptr [edi + 4], 1
        mov    ax, word ptr [esi + 8]
        mov    word ptr [edi + 6], ax
        mov    eax, dword ptr [esi + 0x44]
        mov    dword ptr [edi + 0xc], eax
        mov    word ptr [ebp + 0x1c], 1
        popfd  
        clc    
        ret    
loc_5_1101:
        mov    word ptr [ebp + 0x1c], 0
        popfd  
        stc    
        ret    
loc_5_110A:
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
        call   sub_1_013C
loc_5_1162:
        add    esp, 8
        or     edi, edi
        je     loc_5_118E
        lea    eax, [edi + 0x40]
        push   dword ptr [esi + 8]
        push   dword ptr [esi + 4]
        push   eax
        push   dword ptr [esi]
        call   sub_5_12C1
        add    esp, 0x10
        or     eax, eax
        je     loc_5_118E
        mov    word ptr [ebp + 0x1c], ax
        shr    eax, 0x10
        mov    word ptr [ebp + 0x14], ax
        clc    
        ret    
loc_5_118E:
        mov    word ptr [ebp + 0x1c], 0
        mov    word ptr [ebp + 0x14], 0
        stc    
        ret    
loc_5_119C:
        xchg   esi, eax
        mov    ax, 0x3430
        VMMCall  VMM.Map_Flat                    ; cd201c000100
        xchg   esi, eax
        mov    eax, dword ptr [esi]
        push   0
        push   eax
        call   sub_1_013C
loc_5_11B2:
        add    esp, 8
        or     edi, edi
        je     loc_5_11C8
        lea    eax, [edi + 0x40]
        push   dword ptr [esi + 4]
        push   eax
        call   sub_5_1451
        add    esp, 8
loc_5_11C8:
        mov    word ptr [ebp + 0x1c], 0
        clc    
        ret    
sub_5_11D0:
        push   ebp
        mov    ebp, esp
        sub    esp, 0x6c
        mov    eax, 0xffffffff
        push   ebx
        push   esi
        push   edi
        mov    esi, dword ptr [ebp + 8]
        test   esi, esi
        je     loc_5_12BA
        lea    eax, [ebp - 0x6c]
        push   eax
        call   sub_5_14E6
        add    esp, 4
        call   sub_5_14D4
        call   sub_5_1523
        mov    edi, dword ptr [eax + 8]
        mov    ebx, dword ptr [ebp + 0xc]
        mov    eax, ebx
        shr    eax, 0x10
        movzx  ecx, ax
        push   ecx
        call   sub_5_1515
        add    esp, 4
        movzx  ecx, bx
        push   ecx
        call   sub_5_1515
        add    esp, 4
        mov    ebx, dword ptr [ebp + 0x10]
        mov    eax, ebx
        shr    eax, 0x10
        movzx  ecx, ax
        push   ecx
        call   sub_5_1515
        add    esp, 4
        movzx  ecx, bx
        push   ecx
        call   sub_5_1515
        add    esp, 4
        mov    ebx, dword ptr [ebp + 0x14]
        mov    eax, ebx
        shr    eax, 0x10
        movzx  ecx, ax
        push   ecx
        call   sub_5_1515
        add    esp, 4
        movzx  ecx, bx
        push   ecx
        call   sub_5_1515
        add    esp, 4
        mov    ebx, dword ptr [ebp + 0x18]
        mov    eax, ebx
        shr    eax, 0x10
        movzx  ecx, ax
        push   ecx
        call   sub_5_1515
        add    esp, 4
        movzx  ecx, bx
        push   ecx
        call   sub_5_1515
        movzx  ecx, si
        shr    esi, 0x10
        add    esp, 4
        movzx  eax, si
        push   ecx
        push   eax
        call   sub_5_1506
        add    esp, 8
        call   sub_5_14E0
        movzx  esi, word ptr [edi + 0x14]
        shl    esi, 0x10
        movzx  eax, word ptr [edi + 0x1c]
        or     esi, eax
        call   sub_5_14DA
        lea    ecx, [ebp - 0x6c]
        push   ecx
        call   sub_5_14F6
        add    esp, 4
        mov    eax, esi
loc_5_12BA:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_12C1:
        push   ebx
        push   esi
        mov    esi, dword ptr [esp + 0x10]
        push   edi
        push   ebp
        mov    ebp, dword ptr [esi]
        test   ebp, ebp
        jne    loc_5_12E7
        push   0x15
        push   4
        call   sub_5_2078
        mov    ebp, eax
        test   ebp, ebp
        jne    loc_5_12E5
        xor    eax, eax
        jmp    loc_5_144C
loc_5_12E5:
        mov    dword ptr [esi], ebp
loc_5_12E7:
        mov    edi, dword ptr [esp + 0x1c]
        mov    ecx, 0xffffffff
        sub    eax, eax
        repne scasb al, byte ptr es:[edi]
        not    ecx
        dec    ecx
        cmp    ecx, 8
        jbe    loc_5_1304
        mov    eax, dword ptr [esp + 0x1c]
        mov    byte ptr [eax + 8], 0
loc_5_1304:
        push   ebp
        call   sub_5_2068
        mov    ebx, eax
        test   ebx, ebx
        je     loc_5_13F8
loc_5_1314:
        mov    ecx, dword ptr [esp + 0x1c]
        lea    edx, [ebx + 0xc]
loc_5_131B:
        mov    al, byte ptr [ecx]
        cmp    al, byte ptr [edx]
        jne    loc_5_133B
        or     al, al
        je     loc_5_1337
        mov    al, byte ptr [ecx + 1]
        cmp    al, byte ptr [edx + 1]
        jne    loc_5_133B
        add    ecx, 2
        add    edx, 2
        or     al, al
        jne    loc_5_131B
loc_5_1337:
        xor    eax, eax
        jmp    loc_5_1340
loc_5_133B:
        sbb    eax, eax
        sbb    eax, -1
loc_5_1340:
        test   eax, eax
        je     loc_5_1351
        push   ebx
        push   ebp
        call   sub_5_2058
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_5_1314
loc_5_1351:
        test   ebx, ebx
        je     loc_5_13F8
        cmp    dword ptr [ebx + 4], 0
        je     loc_5_1366
        xor    eax, eax
        jmp    loc_5_144C
loc_5_1366:
        push   0
        push   dword ptr [ebx + 8]
        push   0
        push   ebx
        push   dword ptr [ebx]
        call   sub_5_11D0
        add    esp, 0x14
        test   eax, eax
        je     loc_5_1393
        push   ebx
        mov    dword ptr [ebx], 0
        push   esi
        call   sub_5_1451
        add    esp, 8
        xor    eax, eax
        jmp    loc_5_144C
loc_5_1393:
        mov    eax, dword ptr [esp + 0x20]
        push   0
        mov    ecx, dword ptr [eax + 4]
        mov    dword ptr [ebx + 4], ecx
        push   dword ptr [ebx + 8]
        push   0
        push   ebx
        push   ecx
        call   sub_5_11D0
        add    esp, 0x14
        test   eax, eax
        je     loc_5_13CA
        mov    dword ptr [ebx + 4], 0
        push   ebx
        push   esi
        call   sub_5_1451
        add    esp, 8
        xor    eax, eax
        jmp    loc_5_144C
loc_5_13CA:
        push   dword ptr [ebx + 4]
        push   dword ptr [ebx + 8]
        push   0x10000
        push   ebx
        push   dword ptr [ebx]
        call   sub_5_11D0
        add    esp, 0x14
        push   dword ptr [ebx]
        push   dword ptr [ebx + 8]
        push   0x10000
        push   ebx
        push   dword ptr [ebx + 4]
        call   sub_5_11D0
        add    esp, 0x14
        jmp    loc_5_144A
loc_5_13F8:
        push   ebp
        call   sub_5_2044
        mov    ebx, eax
        test   ebx, ebx
        jne    loc_5_1408
        xor    eax, eax
        jmp    loc_5_144C
loc_5_1408:
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
        call   sub_5_2034
loc_5_144A:
        mov    eax, ebx
loc_5_144C:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    
sub_5_1451:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x10]
        test   edi, edi
        je     loc_5_149D
        mov    eax, dword ptr [edi]
        test   eax, eax
        je     loc_5_147B
        push   0
        push   dword ptr [edi + 8]
        push   0x40000
        push   edi
        push   eax
        call   sub_5_11D0
        add    esp, 0x14
        mov    dword ptr [edi], 0
loc_5_147B:
        mov    eax, dword ptr [edi + 4]
        test   eax, eax
        je     loc_5_149D
        push   0
        push   dword ptr [edi + 8]
        push   0x40000
        push   edi
        push   eax
        call   sub_5_11D0
        mov    dword ptr [edi + 4], 0
        add    esp, 0x14
loc_5_149D:
        mov    esi, dword ptr [esp + 0xc]
        mov    eax, dword ptr [esi]
        test   eax, eax
        je     loc_5_14CE
        push   edi
        push   eax
        call   sub_5_20AC
        push   edi
        push   dword ptr [esi]
        call   sub_5_209C
        push   dword ptr [esi]
        call   sub_5_2068
        test   eax, eax
        jne    loc_5_14C8
        push   dword ptr [esi]
        call   sub_5_208C
loc_5_14C8:
        mov    dword ptr [esi], 0
loc_5_14CE:
        pop    edi
        pop    esi
        ret    
        db  0CCh, 0CCh, 0CCh
sub_5_14D4:
        VMMCall  VMM.Begin_Nest_Exec             ; cd2083800100
sub_5_14DA:
        VMMCall  VMM.End_Nest_Exec               ; cd2086800100
sub_5_14E0:
        VMMCall  VMM.Resume_Exec                 ; cd2085800100
sub_5_14E6:
        push   ebp
        mov    ebp, esp
        push   edi
        mov    edi, dword ptr [ebp + 8]
        VMMCall  VMM.Save_Client_State           ; cd208d000100
        pop    edi
        pop    ebp
        ret    
sub_5_14F6:
        push   ebp
        mov    ebp, esp
        push   esi
        mov    esi, dword ptr [ebp + 8]
        VMMCall  VMM.Restore_Client_State        ; cd208e000100
        pop    esi
        pop    ebp
        ret    
sub_5_1506:
        movzx  ecx, word ptr [esp + 4]
        mov    edx, dword ptr [esp + 8]
        VMMCall  VMM.Simulate_Far_Call           ; cd2048800100
sub_5_1515:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        VMMCall  VMM.Simulate_Push               ; cd204d000100
        pop    ebp
        ret    
sub_5_1523:
        push   ebx
        VMMCall  VMM.Get_Cur_VM_Handle           ; cd2001000100
        mov    eax, ebx
        pop    ebx
        ret    
loc_5_152E:
        mov    edx, dword ptr [esp + 0xc]
        push   esi
        push   edi
        mov    ecx, 0x10
        mov    edi, 0                        ; -> loc_1_0040
        mov    eax, dword ptr [esp + 0x10]
        mov    esi, eax
        mov    dword ptr [edx], 0
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        je     loc_5_1563
        mov    edi, 0                        ; -> loc_1_0060
        mov    ecx, 0x10
        mov    esi, eax
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        mov    eax, 0x80004002
        jne    loc_5_1571
loc_5_1563:
        mov    eax, dword ptr [esp + 0xc]
        push   eax
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 4]
        xor    eax, eax
loc_5_1571:
        pop    edi
        pop    esi
        ret    0xc
loc_5_1576:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 0xc]
        inc    eax
        mov    dword ptr [ecx + 0xc], eax
        ret    4
loc_5_1584:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 0xc]
        dec    eax
        mov    dword ptr [ecx + 0xc], eax
        ret    4
loc_5_1592:
        push   esi
        push   edi
        mov    edi, dword ptr [esp + 0x10]
        push   0xff
        push   0                             ; -> obj6:0x02D4
        lea    esi, [edi + 0x104]
        push   esi
        mov    dword ptr [edi], 9
        call   sub_5_20C0
        add    esp, 0xc
        call   sub_5_1B30
        push   0xff
        lea    ecx, [edi + 4]
        push   eax
        push   ecx
        call   sub_5_20C0
        mov    edx, dword ptr [esp + 0x18]
        add    esp, 0xc
        xor    ecx, ecx
        mov    dword ptr [edi + 0x210], ecx
        mov    eax, dword ptr [edx + 4]
        mov    dword ptr [edi + 0x204], eax
        mov    dword ptr [edi + 0x20c], ecx
        mov    word ptr [edi + 0x208], 0x33f4
        mov    eax, 0xb3
        cmp    byte ptr [esi], cl
        je     loc_5_1606
loc_5_15FB:
        movsx  ecx, byte ptr [esi]
        add    eax, ecx
        inc    esi
        cmp    byte ptr [esi], 0
        jne    loc_5_15FB
loc_5_1606:
        mov    ecx, 0xf7
        sub    edx, edx
        div    ecx
        mov    word ptr [edi + 0x20a], dx
        xor    eax, eax
        pop    edi
        pop    esi
        ret    8
loc_5_161D:
        push   esi
        push   edi
        mov    edi, 0x8878000a
        mov    esi, dword ptr [esp + 0xc]
        cmp    dword ptr [esi + 0xc], 0
        jne    loc_5_1642
        push   dword ptr [esi + 4]
        call   sub_5_1C11
        add    esp, 4
        test   eax, eax
        jne    loc_5_1642
        xor    edi, edi
        inc    dword ptr [esi + 0xc]
loc_5_1642:
        mov    eax, edi
        pop    edi
        pop    esi
        ret    4
loc_5_1649:
        mov    eax, dword ptr [esp + 8]
        xor    ecx, ecx
        mov    dword ptr [eax], 0xf
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
loc_5_1694:
        push   ebx
        push   esi
        push   edi
        xor    ebx, ebx
        push   ebp
        mov    edi, 0x8007000e
        mov    esi, dword ptr [esp + 0x2c]
        test   byte ptr [esp + 0x1c], 0xf0
        mov    dword ptr [esi], ebx
        je     loc_5_16B6
        mov    eax, 0x8878001e
        jmp    loc_5_177C
loc_5_16B6:
        mov    ebp, dword ptr [esp + 0x14]
        cmp    dword ptr [ebp + 0x10], 0
        jne    loc_5_175B
        push   0x201
        push   0x31
        call   sub_5_20D0
        add    esp, 8
        mov    ebx, eax
        test   ebx, ebx
        je     loc_5_1754
        mov    dword ptr [ebx], 0            ; -> obj6:0x02F0
        mov    dword ptr [ebx + 4], ebp
        push   dword ptr [ebp + 4]
        push   ebx
        call   sub_5_1C53
        add    esp, 8
        mov    eax, dword ptr [ebx + 0x14]
        cmp    eax, 0x1000
        jbe    loc_5_1760
        cmp    eax, 0xffff
        jae    loc_5_1760
        cmp    dword ptr [ebx + 0xc], 0
        je     loc_5_1760
        cmp    dword ptr [ebx + 0x10], 0
        je     loc_5_1760
        cmp    dword ptr [ebx + 0x18], 0
        je     loc_5_1760
        push   dword ptr [esp + 0x24]
        push   dword ptr [esp + 0x2c]
        push   ebx
        xor    edi, edi
        call   sub_5_18D4
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
        call   sub_5_1C9A
        add    esp, 0x14
        jmp    loc_5_1760
loc_5_1754:
        mov    edi, 0x8007000e
        jmp    loc_5_1760
loc_5_175B:
        mov    edi, 0x8878000a
loc_5_1760:
        test   edi, edi
        je     loc_5_1775
        test   ebx, ebx
        je     loc_5_177A
        push   0
        push   ebx
        call   sub_5_20C8
        add    esp, 8
        jmp    loc_5_177A
loc_5_1775:
        mov    dword ptr [esi], ebx
        inc    dword ptr [ebp + 0x10]
loc_5_177A:
        mov    eax, edi
loc_5_177C:
        pop    ebp
        pop    edi
        pop    esi
        pop    ebx
        ret    0x1c
loc_5_1783:
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [eax], 0
        mov    eax, 0x80004001
        ret    0xc
loc_5_1795:
        mov    eax, 0x80004005
        mov    ecx, dword ptr [esp + 4]
        mov    edx, dword ptr [ecx + 0xc]
        dec    edx
        mov    dword ptr [ecx + 0xc], edx
        jne    loc_5_17BA
        cmp    dword ptr [ecx + 0x10], 0
        jne    loc_5_17BA
        push   dword ptr [ecx + 4]
        call   sub_5_1BF4
        add    esp, 4
        xor    eax, eax
loc_5_17BA:
        ret    4
sub_5_17BD:
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
        call   sub_5_20D0
        add    esp, 8
        mov    esi, eax
        test   esi, esi
        je     loc_5_183C
        mov    edi, dword ptr [ebp + 8]
        push   1
        push   0                             ; -> obj6:0x02E0
        lea    eax, [ebp - 4]
        push   eax
        lea    ecx, [ebp - 0x14]
        push   ecx
        mov    dword ptr [esi + 4], edi
        mov    dword ptr [esi], 0            ; -> obj6:0x02B0
        call   sub_5_20EC
        test   eax, eax
        jne    loc_5_183C
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
        jne    loc_5_183C
        push   esi
        mov    ebx, 1
        push   edi
        call   sub_5_0E23
        add    esp, 8
loc_5_183C:
        test   ebx, ebx
        jne    loc_5_1865
        cmp    dword ptr [ebp - 4], 0
        je     loc_5_1856
        mov    eax, dword ptr [ebp - 4]
        push   dword ptr [eax + 0xb]
        mov    ax, word ptr [eax + 9]
        push   eax
        call   sub_5_20D8
loc_5_1856:
        test   esi, esi
        je     loc_5_1865
        push   0
        push   esi
        call   sub_5_20C8
        add    esp, 8
loc_5_1865:
        pop    edi
        pop    esi
        pop    ebx
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_186C:
        push   ebp
        mov    ebp, esp
        sub    esp, 0xc
        push   esi
        push   edi
        mov    edi, dword ptr [ebp + 8]
        push   edi
        call   sub_5_0E06
        add    esp, 4
        mov    esi, eax
        test   esi, esi
        je     loc_5_18CE
        push   0
        push   edi
        call   sub_5_0E23
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
        call   sub_5_20D8
        push   0
        push   esi
        call   sub_5_20C8
        add    esp, 8
loc_5_18CE:
        pop    edi
        pop    esi
        mov    esp, ebp
        pop    ebp
        ret    
sub_5_18D4:
        mov    edx, dword ptr [esp + 0xc]
        push   esi
        mov    esi, dword ptr [esp + 8]
        mov    eax, dword ptr [esi + 0x14]
        cmp    dword ptr [edx], eax
        jbe    loc_5_18E6
        mov    dword ptr [edx], eax
loc_5_18E6:
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
loc_5_1906:
        mov    eax, dword ptr [esp + 4]
        push   esi
        mov    esi, dword ptr [eax + 0x20]
        dec    esi
        mov    dword ptr [eax + 0x20], esi
        jne    loc_5_192B
        cmp    dword ptr [eax + 8], 0
        jne    loc_5_192B
        mov    ecx, dword ptr [eax + 4]
        push   0
        push   eax
        dec    dword ptr [ecx + 0x10]
        call   sub_5_20C8
        add    esp, 8
loc_5_192B:
        mov    eax, esi
        pop    esi
        ret    4
loc_5_1931:
        mov    edx, dword ptr [esp + 0xc]
        push   esi
        push   edi
        mov    ecx, 0x10
        mov    edi, 0                        ; -> loc_1_0050
        mov    eax, dword ptr [esp + 0x10]
        mov    esi, eax
        mov    dword ptr [edx], 0
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        je     loc_5_1966
        mov    edi, 0                        ; -> loc_1_0060
        mov    ecx, 0x10
        mov    esi, eax
        repe cmpsb byte ptr [esi], byte ptr es:[edi]
        mov    eax, 0x80004002
        jne    loc_5_1974
loc_5_1966:
        mov    eax, dword ptr [esp + 0xc]
        push   eax
        mov    dword ptr [edx], eax
        mov    eax, dword ptr [eax]
        call   dword ptr [eax + 4]
        xor    eax, eax
loc_5_1974:
        pop    edi
        pop    esi
        ret    0xc
loc_5_1979:
        mov    ecx, dword ptr [esp + 4]
        mov    eax, dword ptr [ecx + 0x20]
        inc    eax
        mov    dword ptr [ecx + 0x20], eax
        ret    4
loc_5_1987:
        mov    eax, 0x80004001
        ret    0x20
loc_5_198F:
        mov    eax, 0x80004001
        ret    0x14
loc_5_1997:
        mov    eax, dword ptr [esp + 8]
        test   eax, eax
        je     loc_5_19F0
        cmp    word ptr [eax], 1
        jne    loc_5_19F0
        mov    edx, dword ptr [eax + 4]
        cmp    edx, 0xafc8
        jae    loc_5_19F0
        mov    cx, word ptr [eax + 0xe]
        cmp    cx, 8
        je     loc_5_19C0
        cmp    cx, 0x10
        jne    loc_5_19F0
loc_5_19C0:
        mov    ax, word ptr [eax + 2]
        cmp    ax, 1
        je     loc_5_19D0
        cmp    ax, 2
        jne    loc_5_19F0
loc_5_19D0:
        movzx  eax, ax
        movzx  ecx, cx
        push   eax
        push   ecx
        mov    eax, dword ptr [esp + 0xc]
        push   edx
        movzx  ecx, word ptr [eax + 0x2e]
        push   1
        push   ecx
        call   sub_5_1C9A
        add    esp, 0x14
        xor    eax, eax
        jmp    loc_5_19F5
loc_5_19F0:
        mov    eax, 0x88780064
loc_5_19F5:
        ret    8
loc_5_19F8:
        mov    eax, dword ptr [esp + 8]
        test   eax, eax
        je     loc_5_1A1D
        cmp    eax, 0xafc8
        jae    loc_5_1A1D
        mov    ecx, dword ptr [esp + 4]
        push   eax
        movzx  edx, word ptr [ecx + 0x2e]
        push   edx
        call   sub_5_1D0E
        add    esp, 8
        xor    eax, eax
        jmp    loc_5_1A22
loc_5_1A1D:
        mov    eax, 0x88780064
loc_5_1A22:
        ret    8
loc_5_1A25:
        mov    eax, 0x80004001
        ret    8
loc_5_1A2D:
        mov    eax, 0x80004001
        ret    8
loc_5_1A35:
        push   esi
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi + 8], 1
        jne    loc_5_1A67
        push   dword ptr [esi + 0x18]
        call   sub_5_1B3E
        add    esp, 4
        inc    eax
        cmp    byte ptr [esi + 0x30], 3
        jbe    loc_5_1A54
        add    eax, eax
loc_5_1A54:
        mov    ecx, dword ptr [esi + 0x1c]
        mov    edx, dword ptr [esp + 0x10]
        sub    ecx, eax
        mov    eax, dword ptr [esp + 0xc]
        mov    dword ptr [eax], ecx
        mov    dword ptr [edx], ecx
        jmp    loc_5_1A7B
loc_5_1A67:
        mov    eax, dword ptr [esp + 0xc]
        mov    ecx, dword ptr [esp + 0x10]
        mov    dword ptr [eax], 0
        mov    dword ptr [ecx], 0
loc_5_1A7B:
        xor    eax, eax
        pop    esi
        ret    0xc
loc_5_1A81:
        push   esi
        mov    eax, 0x80004005
        mov    esi, dword ptr [esp + 8]
        push   edi
        cmp    dword ptr [esi + 8], 0
        jne    loc_5_1AF6
        push   dword ptr [esi + 0x18]
        movzx  edi, word ptr [esi + 0x2e]
        call   sub_5_1B59
        add    esp, 4
        push   dword ptr [esi + 0x1c]
        push   dword ptr [esi + 0xc]
        push   dword ptr [esi + 0x18]
        call   sub_5_1B7D
        add    esp, 0xc
        push   dword ptr [esi + 0x18]
        call   sub_5_1B9E
        add    esp, 4
        push   dword ptr [esi + 0x18]
        call   sub_5_1B67
        mov    ax, word ptr [esi + 0x1c]
        add    esp, 4
        cmp    dword ptr [esi + 0x24], 0x10
        jne    loc_5_1AD5
        shr    ax, 1
loc_5_1AD5:
        cmp    dword ptr [esi + 0x28], 2
        jne    loc_5_1ADE
        shr    ax, 1
loc_5_1ADE:
        dec    ax
        movzx  ecx, ax
        push   ecx
        push   edi
        call   sub_5_1D72
        add    esp, 8
        xor    eax, eax
        mov    dword ptr [esi + 8], 1
loc_5_1AF6:
        pop    edi
        pop    esi
        ret    0x10
loc_5_1AFB:
        push   esi
        mov    eax, 0x80004005
        mov    esi, dword ptr [esp + 8]
        cmp    dword ptr [esi + 8], 0
        je     loc_5_1B2C
        movzx  eax, word ptr [esi + 0x2e]
        push   eax
        call   sub_5_1F50
        add    esp, 4
        push   dword ptr [esi + 0x18]
        call   sub_5_1B59
        add    esp, 4
        xor    eax, eax
        mov    dword ptr [esi + 8], 0
loc_5_1B2C:
        pop    esi
        ret    4
sub_5_1B30:
        movzx  eax, word ptr [0x10]          ; -> loc_2_0010
        add    eax, dword ptr [4]            ; -> obj2:0x0004
        ret    
sub_5_1B3E:
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
sub_5_1B59:
        push   ebp
        mov    ebp, esp
        mov    eax, dword ptr [ebp + 8]
        VxDCall  VDMAD.VDMAD_Phys_Mask_Channel   ; cd2018000400
        pop    ebp
        ret    
sub_5_1B67:
        push   ebp
        mov    ebp, esp
        push   ebx
        mov    edx, dword ptr [ebp + 8]
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        VxDCall  VDMAD.VDMAD_Phys_Unmask_Channel ; cd2019000400
        pop    ebx
        pop    ebp
        ret    
sub_5_1B7D:
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
sub_5_1B9E:
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
        call   sub_1_02D0
loc_5_1BD4:
        xor    eax, eax
        jae    loc_5_1BDD
        mov    eax, 1
loc_5_1BDD:
        pop    edx
        pop    ebp
        ret    
        push   ebp
        mov    ebp, esp
        push   edx
        mov    edx, dword ptr [ebp + 8]
        mov    ah, byte ptr [ebp + 0xc]
        call   sub_1_02B4
loc_5_1BEF:
        xor    ah, ah
        pop    edx
        pop    ebp
        ret    
sub_5_1BF4:
        push   ebp
        mov    ebp, esp
        push   edi
        push   ebx
        mov    eax, dword ptr [ebp + 8]
        push   0
        push   eax
        call   sub_1_013C
loc_5_1C04:
        add    esp, 8
        mov    word ptr [edi + 0x70], 0
        pop    ebx
        pop    edi
        pop    ebp
        ret    
sub_5_1C11:
        push   ebp
        mov    ebp, esp
        push   edi
        push   edx
        push   ebx
        mov    edx, 0xffff
        mov    eax, dword ptr [ebp + 8]
        push   0
        push   eax
        call   sub_1_013C
loc_5_1C27:
        add    esp, 8
        mov    eax, dword ptr [edi + 0x2c]
        or     eax, eax
        je     loc_5_1C3D
        VMMCall  VMM.Get_Sys_VM_Handle           ; cd2003000100
        cmp    eax, ebx
        je     loc_5_1C3D
        jmp    loc_5_1C4C
loc_5_1C3D:
        mov    word ptr [edi + 0x70], 1
        movzx  edx, word ptr [edi + 6]
        mov    edx, 0
loc_5_1C4C:
        mov    eax, edx
        pop    ebx
        pop    edx
        pop    edi
        pop    ebp
        ret    
sub_5_1C53:
        push   ebp
        mov    ebp, esp
        push   edi
        push   esi
        mov    esi, dword ptr [ebp + 8]
        mov    eax, dword ptr [ebp + 0xc]
        push   0
        push   eax
        call   sub_1_013C
loc_5_1C66:
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
sub_5_1C9A:
        push   ebp
        mov    ebp, esp
        push   eax
        push   edi
        cmp    word ptr [ebp + 0xc], 1
        jne    loc_5_1D09
        cmp    word ptr [ebp + 0x10], 0xfa0
        jb     loc_5_1D09
        cmp    word ptr [ebp + 0x10], 0xafc8
        ja     loc_5_1D09
        cmp    word ptr [ebp + 0x14], 8
        je     loc_5_1CC4
        cmp    word ptr [ebp + 0x14], 0x10
        jne    loc_5_1D09
loc_5_1CC4:
        cmp    word ptr [ebp + 0x18], 1
        jb     loc_5_1D09
        cmp    word ptr [ebp + 0x18], 2
        ja     loc_5_1D09
        and    word ptr [0], 0xfff9          ; -> obj6:0x0320
        cmp    word ptr [ebp + 0x14], 0x10
        jne    loc_5_1CE9
        or     word ptr [0], 2               ; -> obj6:0x0320
loc_5_1CE9:
        cmp    word ptr [ebp + 0x18], 2
        jne    loc_5_1CF8
        or     word ptr [0], 4               ; -> obj6:0x0320
loc_5_1CF8:
        push   dword ptr [ebp + 0x10]
        push   dword ptr [ebp + 8]
        call   sub_5_1D0E
        add    esp, 8
        clc    
        jmp    loc_5_1D0A
loc_5_1D09:
        stc    
loc_5_1D0A:
        pop    edi
        pop    eax
        pop    ebp
        ret    
sub_5_1D0E:
        push   ebp
        mov    ebp, esp
        push   eax
        push   ebx
        push   edx
        push   edi
        mov    edx, dword ptr [ebp + 8]
        mov    al, 0xc6
        call   sub_1_02D0
loc_5_1D1F:
        jb     loc_5_1D6B
        mov    bx, word ptr [ebp + 0xc]
        push   edx
        mov    dx, 6
        mov    ax, 0x1198
        div    bx
        mov    ah, al
        mov    al, 0x80
        sub    al, ah
        pop    edx
        push   eax
        mov    al, 0xa1
        call   sub_1_02D0
loc_5_1D40:
        pop    eax
        jb     loc_5_1D6B
        call   sub_1_02D0
loc_5_1D48:
        jb     loc_5_1D6B
        push   edx
        mov    dx, 3
        mov    ax, 0xb9e
        div    bx
        neg    al
        pop    edx
        push   eax
        mov    al, 0xa2
        call   sub_1_02D0
loc_5_1D61:
        pop    eax
        jb     loc_5_1D6B
        call   sub_1_02D0
loc_5_1D69:
        jmp    loc_5_1D6C
loc_5_1D6B:
        stc    
loc_5_1D6C:
        pop    edi
        pop    edx
        pop    ebx
        pop    eax
        pop    ebp
        ret    
sub_5_1D72:
        push   ebp
        mov    ebp, esp
        push   eax
        push   edx
        push   edi
        mov    edx, dword ptr [ebp + 8]
        mov    al, 0xd1
        call   sub_1_02D0
loc_5_1D82:
        jb     loc_5_1F4A
        add    edx, 6
        mov    al, 2
        out    dx, al
        in     al, dx
        in     al, dx
        in     al, dx
        sub    edx, 6
        jmp    loc_5_1D96
loc_5_1D96:
        jmp    loc_5_1D98
loc_5_1D98:
        add    edx, 6
        xor    al, al
        out    dx, al
        sub    edx, 6
        mov    al, 0xc6
        call   sub_1_02D0
loc_5_1DA8:
        jb     loc_5_1F4A
        mov    al, 0xa4
        call   sub_1_02D0
loc_5_1DB5:
        jb     loc_5_1F4A
        mov    eax, dword ptr [ebp + 0xc]
        neg    ax
        call   sub_1_02D0
loc_5_1DC6:
        jb     loc_5_1F4A
        push   eax
        mov    al, 0xa5
        call   sub_1_02D0
loc_5_1DD4:
        pop    eax
        jb     loc_5_1F4A
        mov    al, ah
        call   sub_1_02D0
loc_5_1DE2:
        jb     loc_5_1F4A
        mov    al, 0xa8
        call   sub_1_02D0
loc_5_1DEF:
        jb     loc_5_1F4A
        mov    al, 0xf5
        test   word ptr [0], 4               ; -> obj6:0x0320
        jne    loc_5_1E04
        mov    al, 0xf6
loc_5_1E04:
        call   sub_1_02D0
loc_5_1E09:
        jb     loc_5_1F4A
        mov    al, 0xc0
        call   sub_1_02D0
loc_5_1E16:
        jb     loc_5_1F4A
        mov    al, 0xb1
        call   sub_1_02D0
loc_5_1E23:
        jb     loc_5_1F4A
        call   sub_1_02B4
loc_5_1E2E:
        jb     loc_5_1F4A
        and    al, 0xf
        or     al, 0x50
        push   eax
        mov    al, 0xb1
        call   sub_1_02D0
loc_5_1E40:
        pop    eax
        jb     loc_5_1F4A
        call   sub_1_02D0
loc_5_1E4C:
        jb     loc_5_1F4A
        mov    al, 0xc0
        call   sub_1_02D0
loc_5_1E59:
        jb     loc_5_1F4A
        mov    al, 0xb2
        call   sub_1_02D0
loc_5_1E66:
        jb     loc_5_1F4A
        call   sub_1_02B4
loc_5_1E71:
        jb     loc_5_1F4A
        and    al, 0xf
        or     al, 0x50
        push   eax
        mov    al, 0xb2
        call   sub_1_02D0
loc_5_1E83:
        pop    eax
        jb     loc_5_1F4A
        call   sub_1_02D0
loc_5_1E8F:
        jb     loc_5_1F4A
        test   word ptr [0], 2               ; -> obj6:0x0320
        je     loc_5_1EEF
        mov    al, 0xb6
        call   sub_1_02D0
loc_5_1EA7:
        jb     loc_5_1F4A
        xor    al, al
        call   sub_1_02D0
loc_5_1EB4:
        jb     loc_5_1F4A
        mov    al, 0xb7
        call   sub_1_02D0
loc_5_1EC1:
        jb     loc_5_1F4A
        mov    al, 0x71
        call   sub_1_02D0
loc_5_1ECE:
        jb     loc_5_1F4A
        mov    al, 0xb7
        call   sub_1_02D0
loc_5_1ED7:
        jb     loc_5_1F4A
        mov    al, 0xbc
        test   word ptr [0], 4               ; -> obj6:0x0320
        jne    loc_5_1EE8
        mov    al, 0xf4
loc_5_1EE8:
        call   sub_1_02D0
loc_5_1EED:
        jb     loc_5_1F4A
loc_5_1EEF:
        mov    al, 0xb7
        call   sub_1_02D0
loc_5_1EF6:
        jb     loc_5_1F4A
        mov    al, 0xd0
        test   word ptr [0], 4               ; -> obj6:0x0320
        je     loc_5_1F07
        mov    al, 0x98
loc_5_1F07:
        test   word ptr [0], 2               ; -> obj6:0x0320
        je     loc_5_1F14
        or     al, 0x24
loc_5_1F14:
        call   sub_1_02D0
loc_5_1F19:
        jb     loc_5_1F4A
        mov    al, 0xc0
        call   sub_1_02D0
loc_5_1F22:
        jb     loc_5_1F4A
        mov    al, 0xb8
        call   sub_1_02D0
loc_5_1F2B:
        jb     loc_5_1F4A
        call   sub_1_02B4
loc_5_1F32:
        jb     loc_5_1F4A
        and    al, 0x30
        or     al, 5
        push   eax
        mov    al, 0xb8
        call   sub_1_02D0
loc_5_1F40:
        pop    eax
        jb     loc_5_1F4A
        call   sub_1_02D0
loc_5_1F48:
        jmp    loc_5_1F4B
loc_5_1F4A:
        stc    
loc_5_1F4B:
        pop    edi
        pop    edx
        pop    eax
        pop    ebp
        ret    
sub_5_1F50:
        push   ebp
        mov    ebp, esp
        push   eax
        push   edx
        push   edi
        mov    edx, dword ptr [ebp + 8]
        mov    al, 0xd3
        call   sub_1_02D0
loc_5_1F60:
        jb     loc_5_1F98
        mov    al, 0xc0
        call   sub_1_02D0
loc_5_1F69:
        jb     loc_5_1F98
        mov    al, 0xb8
        call   sub_1_02D0
loc_5_1F72:
        jb     loc_5_1F98
        call   sub_1_02B4
loc_5_1F79:
        jb     loc_5_1F98
        and    al, 0x30
        push   eax
        mov    al, 0xb8
        call   sub_1_02D0
loc_5_1F85:
        pop    eax
        jb     loc_5_1F98
        call   sub_1_02D0
loc_5_1F8D:
        jmp    loc_5_1F99
        db  0B0h, 0D0h, 0E8h
        dd  sub_1_02D0               ; fixup
        db  0EBh, 001h
loc_5_1F98:
        stc    
loc_5_1F99:
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
        call   sub_5_1D72
        add    esp, 0xc
        pop    ebp
        ret    
        push   ebp
        mov    ebp, esp
        push   dword ptr [ebp + 0xc]
        push   dword ptr [ebp + 8]
        call   sub_5_1F50
        add    esp, 8
        pop    ebp
        ret    
        db  050h, 052h, 0C1h, 0E8h, 008h, 024h, 0F0h, 0C1h, 0EBh, 00Ch, 00Ah, 0C3h, 083h, 0C2h, 004h, 050h
        db  0B0h, 014h, 0EEh, 058h, 066h, 042h, 0EEh, 0F8h, 05Ah, 058h, 0C3h, 0CCh, 0CCh
sub_5_1FE4:
        VMMCall  VMM._RegCloseKey                ; cd2049810100
        int3   
        int3   
sub_5_1FEC:
        VMMCall  VMM._RegDeleteKey               ; cd204b810100
        int3   
        int3   
sub_5_1FF4:
        VMMCall  VMM._RegSetValueEx              ; cd2052810100
        int3   
        int3   
sub_5_1FFC:
        VMMCall  VMM._RegCreateKey               ; cd204a810100
        int3   
        int3   
sub_5_2004:
        VMMCall  VMM._RegOpenKey                 ; cd2048810100
        int3   
        int3   
sub_5_200C:
        VMMCall  VMM._RegQueryValueEx            ; cd2051810100
        int3   
        int3   
sub_5_2014:
        VxDCall  CONFIGMG._CONFIGMG_Get_DevNode_Key ; cd203d803300
        int3   
        int3   
sub_5_201C:
        VMMCall  VMM._Sprintf                    ; cd2074810100
        int3   
        int3   
sub_5_2024:
        VxDCall  CONFIGMG._CONFIGMG_Get_Bus_Info ; cd2044803300
        int3   
        int3   
sub_5_202C:
        VxDCall  CONFIGMG._CONFIGMG_Get_Alloc_Log_Conf ; cd203b803300
        int3   
        int3   
sub_5_2034:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        VMMCall  VMM.List_Attach_Tail            ; cd209f000100
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_5_2044:
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
sub_5_2058:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        VMMCall  VMM.List_Get_Next               ; cd20a4000100
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_5_2068:
        pop    edx
        mov    ecx, esi
        pop    esi
        VMMCall  VMM.List_Get_First              ; cd20a3000100
        mov    esi, ecx
        jmp    edx
        db  0CCh, 0CCh
sub_5_2078:
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
sub_5_208C:
        pop    edx
        mov    ecx, esi
        pop    esi
        VMMCall  VMM.List_Destroy                ; cd209c000100
        mov    esi, ecx
        jmp    edx
        db  0CCh, 0CCh
sub_5_209C:
        pop    edx
        mov    ecx, esi
        pop    esi
        pop    eax
        VMMCall  VMM.List_Deallocate             ; cd20a2000100
        mov    esi, ecx
        jmp    edx
        db  0CCh
sub_5_20AC:
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
sub_5_20C0:
        VMMCall  VMM._lstrcpyn                   ; cd207b810100
        int3   
        int3   
sub_5_20C8:
        VMMCall  VMM._HeapFree                   ; cd2051800100
        int3   
        int3   
sub_5_20D0:
        VMMCall  VMM._HeapAllocate               ; cd204f800100
        int3   
        int3   
sub_5_20D8:
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
sub_5_20EC:
        mov    eax, dword ptr [esp + 0x10]
        mov    edx, dword ptr [esp + 0xc]
        VxDCall  APM.Service_0001h               ; cd2001002700
        jb     loc_5_210A
        mov    ecx, dword ptr [esp + 4]
        mov    dword ptr [ecx], eax
        mov    ecx, dword ptr [esp + 8]
        mov    dword ptr [ecx], edx
        xor    eax, eax
loc_5_210A:
        ret    0x10
VxD__LTEXT ENDS

VxD__LDATA SEGMENT   ; object 6  vsize=0x322  RW-
        dd  obj6:0x0020              ; fixup
        dd  obj6:0x0014              ; fixup
        dd  obj6:0x000C              ; fixup
        db  044h, 065h, 066h, 061h, 075h, 06Ch, 074h, 000h, 044h, 065h, 074h, 065h, 063h, 074h, 065h, 064h
        db  000h, 000h, 000h, 000h, 046h, 06Fh, 072h, 063h, 065h, 064h, 000h, 000h, 043h, 06Fh, 06Eh, 066h
        db  069h, 067h, 05Ch, 044h, 04Dh, 041h, 020h, 053h, 065h, 06Ch, 065h, 063h, 074h, 069h, 06Fh, 06Eh
        db  073h, 000h, 000h, 000h, 044h, 04Dh, 041h, 020h, 053h, 065h, 06Ch, 065h, 063h, 074h, 069h, 06Fh
        db  06Eh, 073h, 000h, 000h, 043h, 06Fh, 06Eh, 066h, 069h, 067h, 05Ch, 049h, 052h, 051h, 020h, 053h
        db  065h, 06Ch, 065h, 063h, 074h, 069h, 06Fh, 06Eh, 073h, 000h, 000h, 000h, 049h, 052h, 051h, 020h
        db  053h, 065h, 06Ch, 065h, 063h, 074h, 069h, 06Fh, 06Eh, 073h, 000h, 000h, 05Ch, 043h, 06Fh, 06Eh
        db  066h, 069h, 067h, 000h, 041h, 055h, 044h, 044h, 052h, 056h, 03Ah, 020h, 000h, 000h, 000h, 000h
        db  042h, 04Ch, 041h, 053h, 054h, 045h, 052h, 000h, 041h, 025h, 030h, 033h, 058h, 020h, 049h, 025h
        db  064h, 020h, 044h, 025h, 064h, 000h, 000h, 000h, 06Dh, 069h, 064h, 069h, 05Ch, 065h, 073h, 073h
        db  066h, 06Dh, 02Eh, 064h, 072h, 076h, 000h, 000h
        dd  loc_5_0EAF               ; fixup
        dd  loc_5_0F97               ; fixup
        dd  loc_5_0EBA               ; fixup
        dd  loc_5_0F1A               ; fixup
        dd  loc_5_0E74               ; fixup
        dd  loc_5_0EAF               ; fixup
        dd  loc_5_108B               ; fixup
        dd  loc_5_100E               ; fixup
        dd  loc_5_104B               ; fixup
        dd  loc_5_110A               ; fixup
        dd  loc_5_119C               ; fixup
        db  005h, 000h, 000h, 000h
        dd  obj6:0x00B4              ; fixup
        db  004h, 000h, 000h, 000h
        dd  obj6:0x00C8              ; fixup
        db  002h, 000h, 000h, 000h
        dd  obj6:0x00D8              ; fixup
        db  010h, 000h, 000h, 000h
        dd  loc_5_09F4               ; fixup
        db  001h, 000h
        dd  loc_5_09F4               ; fixup
        db  002h, 000h
        dd  loc_5_09F4               ; fixup
        db  003h, 000h
        dd  loc_5_09F4               ; fixup
        db  004h, 000h
        dd  loc_5_09F4               ; fixup
        db  005h, 000h
        dd  loc_5_09F4               ; fixup
        db  006h, 000h
        dd  loc_5_09F4               ; fixup
        db  007h, 000h
        dd  loc_5_09F4               ; fixup
        db  008h, 000h
        dd  loc_5_09F4               ; fixup
        db  009h, 000h
        dd  loc_5_09F4               ; fixup
        db  00Ah, 000h
        dd  loc_5_09F4               ; fixup
        db  00Bh, 000h
        dd  loc_5_09F4               ; fixup
        db  00Ch, 000h
        dd  loc_5_09F4               ; fixup
        db  00Dh, 000h
        dd  loc_5_09F4               ; fixup
        db  00Eh, 000h
        dd  loc_5_09F4               ; fixup
        db  00Fh, 000h
        dd  loc_5_09F4               ; fixup
        db  010h, 000h, 000h, 000h
        dd  loc_5_0A38               ; fixup
        db  001h, 000h
        dd  loc_5_0A38               ; fixup
        db  002h, 000h
        dd  loc_5_0A38               ; fixup
        db  003h, 000h
        dd  loc_5_0A38               ; fixup
        db  004h, 000h
        dd  loc_5_09F4               ; fixup
        db  005h, 000h
        dd  loc_5_09F4               ; fixup
        db  006h, 000h
        dd  loc_5_09F4               ; fixup
        db  007h, 000h
        dd  loc_5_09F4               ; fixup
        db  008h, 000h
        dd  loc_5_0A38               ; fixup
        db  009h, 000h
        dd  loc_5_0A38               ; fixup
        db  00Ah, 000h
        dd  loc_5_09F4               ; fixup
        db  00Bh, 000h
        dd  loc_5_09F4               ; fixup
        db  00Ch, 000h
        dd  loc_5_09F4               ; fixup
        db  00Dh, 000h
        dd  loc_5_09F4               ; fixup
        db  00Eh, 000h
        dd  loc_5_09F4               ; fixup
        db  00Fh, 000h
        dd  loc_5_09F4               ; fixup
        db  004h, 000h, 000h, 000h
        dd  loc_5_0A38               ; fixup
        db  001h, 000h
        dd  loc_5_0A38               ; fixup
        db  002h, 000h
        dd  loc_5_0A38               ; fixup
        db  003h, 000h
        dd  loc_5_0A38               ; fixup
        db  000h, 000h, 004h, 000h
        dd  loc_1_023C               ; fixup
        db  000h, 000h, 000h, 000h
        dd  loc_1_0270               ; fixup
        dd  loc_1_0280               ; fixup
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
        db  089h, 08Ah, 000h, 000h, 000h, 000h
        dd  loc_5_152E               ; fixup
        dd  loc_5_1576               ; fixup
        dd  loc_5_1584               ; fixup
        dd  loc_5_1592               ; fixup
        dd  loc_5_161D               ; fixup
        dd  loc_5_1795               ; fixup
        dd  loc_5_1649               ; fixup
        dd  loc_5_1694               ; fixup
        dd  loc_5_1783               ; fixup
        db  045h, 053h, 036h, 038h, 038h, 02Eh, 056h, 058h, 044h, 000h, 000h, 000h, 064h, 073h, 06Fh, 075h
        db  06Eh, 064h, 02Eh, 076h, 078h, 064h, 000h, 000h, 000h, 000h, 000h, 000h
        dd  loc_5_1931               ; fixup
        dd  loc_5_1979               ; fixup
        dd  loc_5_1906               ; fixup
        dd  loc_5_1987               ; fixup
        dd  loc_5_198F               ; fixup
        dd  loc_5_1997               ; fixup
        dd  loc_5_19F8               ; fixup
        dd  loc_5_1A25               ; fixup
        dd  loc_5_1A2D               ; fixup
        dd  loc_5_1A35               ; fixup
        dd  loc_5_1A81               ; fixup
        dd  loc_5_1AFB               ; fixup
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
        mov    esi, 0                        ; -> loc_1_01DC
        VMMCall  VMM.Hook_Device_Service         ; cd2090000100
        clc    
        ret    
loc_7_0070:
        pop    ebx
        jmp    loc_7_0011
VxD__LTEXT ENDS
